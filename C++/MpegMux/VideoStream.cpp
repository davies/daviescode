#include "common/def.h"
#include "VideoStream.h"

VideoStream::VideoStream(const string _filename /* = "" */,bool write /* = false */)
	:BitStream(_filename,write)
{
	seqCount = 0;
	bit_rate = 0.0;
	frame_rate = 0.0;
}

VideoStream::~VideoStream()
{
}

void VideoStream::CreateIndex(double delay )
{
	double frame_time = 1.0 / frame_rate;
	long pic_count = 0;
	seqCount = 0;
	
	while (seek_sync(START_CODE,24)) {
		getbits(24);
		if ( nextbits(8) == (GROUP_START_CODE & 0x0ff) ) {
			//GOP
			seqIndex[seqCount] = getCurrPos();
			seqTime[seqCount] = pic_count * frame_time + delay;
			seqCount ++;
		}else if ( nextbits(8) == (PICTURE_START_CODE & 0x0ff) ) {
			pic_count ++;
		}
	}
	
	seqIndex[seqCount] = getCurrPos();
	seqTime[seqCount] = pic_count * frame_time + delay;
	//videoBS->seqCount ++;
	
	reset(false);
}