#ifndef __MPASTREAM_H__
#define __MPASTREAM_H__

#include "..\common\BitStream.h"
//#include "mpa.h"

#define MAX_FRAMES	150000 //48000*60*60/1152

class MpaStream : public BitStream
{
public:
	MpaStream(const string _filename = "",bool write = false);
	~MpaStream();
	
	unsigned int frameCount;
	unsigned int frameIndex[MAX_FRAMES];
	int bitrate;
	int sampling_frequency;
	bool updateInfo();
private:
};

#endif
