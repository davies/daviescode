#ifndef __SYSTEMHEADER_H__
#define __SYSTEMHEADER_H__

#include "..\common\def.h"
#include "..\common\BitStream.h"

#define STREAM_COUNT 16

class SystemHeader
{
public:
	SystemHeader();
	~SystemHeader(){};
	
	bool parse(BitStream &bs); //for MPEG1
	bool addStreamInfo(unsigned char _stream_id,
		unsigned char _PSTD_buffer_bound_scale,
		unsigned short _PSTD_buffer_size_bound);

	int getAudioStreamCount(){ return audio_bound;}
	unsigned int getRateBound(){ return rate_bound;}
	void setRateBound( unsigned int _rate_bound);
	unsigned int getVideoBufferBound();
	
	int length();

	bool encode(BitStream &bs);

protected:
private:
	unsigned short header_length;
	unsigned int   rate_bound;
	unsigned char  audio_bound;
	bool fixed_flag;
	bool CSPS_flag;
	bool system_audio_lock_flag,system_video_lock_flag;
	unsigned char video_bound;
//	bool packet_rate_restriction_flag;
		
	unsigned int stream_count;
	unsigned char stream_id[STREAM_COUNT];
	bool PSTD_buffer_bound_scale[STREAM_COUNT];
	unsigned short PSTD_buffer_size_bound[STREAM_COUNT];
};

#endif