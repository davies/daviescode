#ifndef __VIDEOSTREAM_H__
#define __VIDEOSTREAM_H__

#include "common\BitStream.h"

#define MAX_SEQUENCES	120000 //30*60*60

class VideoStream : public BitStream
{
public:
	VideoStream(const string _filename = "",bool write = false);
	~VideoStream();
	void CreateIndex(double delay = 0.0);

	unsigned int seqCount;
	unsigned int seqIndex[MAX_SEQUENCES];
	double		 seqTime[MAX_SEQUENCES];
	double bit_rate;
	double frame_rate;
private:
};

#endif
