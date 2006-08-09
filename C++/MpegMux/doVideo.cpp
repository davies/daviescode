#include "muxer.h"
#include "windows.h"

#pragma comment(lib,"VideoConvertLib/videoconvert.lib")

extern "C" __declspec(dllimport)  int video_process(HANDLE in,HANDLE out,int horizontal,int vertical,int bitrate,double *pFrame_rate);

enum 
{
	Succeeded,
	E_InputFile, // ��������ļ�����
	E_HeightWidth, // �����ߴ���
	E_Bitrate, // bitrate̫С��С��64K��
	E_Combinations, //����Ĳ�����ϣ�ʹ�������޷���ʼ����
	E_Libavcodec, // �޷��ҵ�Libavcodec.dll
	E_Unknown, // δ֪����
};

char error_msg[7][100] = {
	"�ɹ�!",
	"��������ļ�����",
	"�����ߴ���",
	"bitrate̫С��С��64K��",
	"����Ĳ�����ϣ�ʹ�������޷���ʼ����",
	"�޷��ҵ�Libavcodec.dll",
	"δ֪����"
};

bool Muxer::doVideo()
{
	fprintf(stdout,"start processing video to %dx%d@%dKbps...",horizontal,vertical,video_new_bit_rate>>10);

	VideoStream *newVideoBS;

	if ( m2v_out ) {
		//����m2v�ļ���
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