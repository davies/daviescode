#include "muxer.h"
#include "windows.h"

#pragma comment(lib,"VideoConvertLib/videoconvert.lib")

extern "C" __declspec(dllimport)  int video_process(HANDLE in,HANDLE out,int horizontal,int vertical,int bitrate,double *pFrame_rate);

enum 
{
	Succeeded,
	E_InputFile, // 输入输出文件错误
	E_HeightWidth, // 输入宽高错误
	E_Bitrate, // bitrate太小（小于64K）
	E_Combinations, //错误的参数组合（使编码器无法初始化）
	E_Libavcodec, // 无法找到Libavcodec.dll
	E_Unknown, // 未知错误
};

char error_msg[7][100] = {
	"成功!",
	"输入输出文件错误",
	"输入宽高错误",
	"bitrate太小（小于64K）",
	"错误的参数组合（使编码器无法初始化）",
	"无法找到Libavcodec.dll",
	"未知错误"
};

bool Muxer::doVideo()
{
	fprintf(stdout,"start processing video to %dx%d@%dKbps...",horizontal,vertical,video_new_bit_rate>>10);

	VideoStream *newVideoBS;

	if ( m2v_out ) {
		//生成m2v文件名
		string outM2VFileName = inFileName.substr(0,inFileName.find_last_of('.')) + ".m2v";
		newVideoBS = new VideoStream(outM2VFileName.c_str(),true);
	}else{
		newVideoBS = new VideoStream;
	}

	HANDLE hFileIn = (HANDLE)_get_osfhandle(videoBS->getFILE()->_file);
	HANDLE hFileOut = (HANDLE)_get_osfhandle(newVideoBS->getFILE()->_file);

	double frame_rate = 0;
	int nRet = 0;
	try{
		nRet = video_process(hFileIn, hFileOut, horizontal, vertical, video_new_bit_rate, &frame_rate);
	}catch ( ... ) {
		fprintf(stdout,"\nerror while processing video.\n");
		return false;
	}

	if ( nRet == Succeeded ) {
		if ( frame_rate == 0.0 ) {
			fprintf(stdout,"\nerror:wrong frame rate %f !\n",frame_rate);
			return false;
		}

		fprintf(stdout,"done.\n");

		//create index
		fprintf(stdout,"start creating index for video ... ");
		delete videoBS;
		videoBS = newVideoBS;
		videoBS->reset(false);
		videoBS->frame_rate = frame_rate;
		videoBS->CreateIndex(video_delay);

		fprintf(stdout,"done .\n");
		return true;
	}else{
		fprintf(stdout,"\nerror: %s!",error_msg[min((unsigned int)nRet,E_Unknown)]);
		return false;
	}
}