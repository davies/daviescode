#include <stdio.h>
#include "common/mpeg2api.h"
#include "common/ipl.h"
#include "muxer.h"

extern int __main(int argc,char* argv[] );


void testVideo()
{
	BitStream video("f:/4.m2v");
	MpaStream *newVideoBS = new MpaStream("f:/5.m2v",true);
	if ( !newVideoBS->isOK() ) {
		return;
	}
	SequenceParam param = {0,};
	VideoData swapBuf = {0,};

	DecoderInitialize(&video,&param);
	int old_width = param.horizontal_size;
	int old_height = param.vertical_size;
	int new_width = old_width;
	int new_height = old_height;

	param.horizontal_size = new_width;
	param.vertical_size = new_height;
//	param.bit_rate = 2000000.0;
	param.N = 15;
	param.M = 3;
	EncoderInitialize(newVideoBS,&param);
	
	newVideoBS->frameCount = 0;
	newVideoBS->frameIndex[0] = 0;
	while ( DecodeSequence( &swapBuf ) ) {

		for( int i=0;i<swapBuf.nframes;i++){
			for(int j=0;j<3;j++){
				int old_buf_width = swapBuf.buf_width[j];
				int old_buf_height = swapBuf.buf_height[j];
				int new_buf_width = old_buf_width * new_width / old_width  ;
				int new_buf_height = old_buf_height * new_height / old_height ;
				unsigned char* dst = (unsigned char*)malloc(new_buf_width*new_buf_height);
				if ( !dst ) {
					fprintf(stderr,"Not enough memory!\n");
					exit(1);
				}
				memset(dst,0,new_buf_width*new_buf_height);
				IplImage *srcImg = iplCreateImageHeader(1,0,IPL_DEPTH_8U,"GRAY","GRAY",IPL_DATA_ORDER_PIXEL,IPL_ORIGIN_TL,
					IPL_ALIGN_QWORD,swapBuf.buf_width[j],swapBuf.buf_height[j],NULL,NULL,0,NULL);
				srcImg->imageData = (char*)swapBuf.buffer[i][j];
				IplImage *dstImg = iplCreateImageHeader(1,0,IPL_DEPTH_8U,"GRAY","GRAY",IPL_DATA_ORDER_PIXEL,IPL_ORIGIN_TL,
					IPL_ALIGN_QWORD,new_buf_width,new_buf_height,NULL,NULL,0,NULL);
				dstImg->imageData = (char*)dst;
				iplResize(srcImg,dstImg,new_buf_width,old_buf_width,new_buf_height,old_buf_height,IPL_INTER_LINEAR);

				free(swapBuf.buffer[i][j]);
				swapBuf.buffer[i][j] = dst;
			}
		}

//		for(i=0;i<128*8;i++){
//			if ( i % 16 == 0) {
//				printf("\n");
//			}
//			printf("%x ",swapBuf.buffer[0][0][i]);
//		}

		EncodeSequence(&swapBuf);
		newVideoBS->frameCount ++;
		newVideoBS->frameIndex[newVideoBS->frameCount] = newVideoBS->getCurrPos();
		fprintf(stdout,"frame %d\r",newVideoBS->frameCount);
	}

	int last_pos = 0;
	int curr_pos = 0;
	for (int j=0;j<=newVideoBS->frameCount;j++){
		curr_pos = newVideoBS->frameIndex[j];
		fprintf(stdout,"%3d = %7d(+%4d)\n",j,curr_pos,curr_pos-last_pos);
		last_pos = curr_pos;
	}

	delete newVideoBS;
}

void main(int argc,char* argv[])
{

//	testVideo();
	__main(argc,argv);

//	MpaStream *test = new MpaStream("f:\\4.mp2");
//	if ( !test->isOK() ) {
//		return ;
//	}
//	MpaFrame::init();
//	Muxer::normalize(test,1.0,1.0);
//	delete test;
}
