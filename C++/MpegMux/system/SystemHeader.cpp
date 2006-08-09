#include "systemHeader.h"

#pragma warning(disable:4800)

/*
 *	System Header
 */
SystemHeader::SystemHeader()
{
	rate_bound = 0;
	audio_bound = 0;
	video_bound = 0;
	system_audio_lock_flag = true;
	system_video_lock_flag = true;
	fixed_flag = false;
	CSPS_flag = false;

	header_length = 6;  //最少的字节数
	stream_count = 0;
	for( int i=0;i<STREAM_COUNT;i++){
		stream_id[i] = 0;
		PSTD_buffer_bound_scale[i] = 0;
		PSTD_buffer_size_bound[i] = 0;
	}
}

bool SystemHeader::parse(BitStream &bs)
{
	if ( bs.getbits(32) != SYSTEM_HEADER_START_CODE ) {
		return false;
	}

	header_length = bs.getbits(16);
	bs.get1bit();
	rate_bound = bs.getbits(22);
	bs.get1bit();
	audio_bound = bs.getbits(6);
	fixed_flag = bs.get1bit();
	CSPS_flag = bs.get1bit();
	system_audio_lock_flag = bs.get1bit();
	system_video_lock_flag = bs.get1bit();
	bs.get1bit();
	video_bound = bs.getbits(5);
	bs.getbits(8);

	stream_count = 0;
	while ( bs.nextbits(1) == 1) {
		stream_id[stream_count] = bs.getbits(8);
		bs.getbits(2);
		PSTD_buffer_bound_scale[stream_count] = bs.get1bit();
		PSTD_buffer_size_bound[stream_count] = bs.getbits(13);
		stream_count ++;
	}

	return true;
}

bool SystemHeader::addStreamInfo(unsigned char _stream_id,
		unsigned char _PSTD_buffer_bound_scale,
		unsigned short _PSTD_buffer_size_bound)
{
	stream_id[stream_count] = _stream_id;
	PSTD_buffer_bound_scale[stream_count] = _PSTD_buffer_bound_scale;
	PSTD_buffer_size_bound[stream_count] = _PSTD_buffer_size_bound;
	stream_count ++;
	header_length += 3;

	if( _stream_id >= AUDIO_STREAM_0 && _stream_id <= AUDIO_STREAM_1F){
		audio_bound ++;
	}

	if ( _stream_id >= VIDEO_STREAM_0 && _stream_id <= VIDEO_STREAM_F ) {
		video_bound ++;
	}

	return true;
}

unsigned int SystemHeader::getVideoBufferBound()
{
	for(int i=0;i<stream_count;i++){
		if( stream_id[i] == 0xB9 || stream_id[i] >= VIDEO_STREAM_0 && stream_id[i] <= VIDEO_STREAM_F){
			return PSTD_buffer_size_bound[i];
		}
	}
	return 0;
}

void SystemHeader::setRateBound(unsigned int _rate_bound)
{
	rate_bound = _rate_bound;
}

int SystemHeader::length()
{
	return header_length + 4 +2;
}

bool SystemHeader::encode(BitStream &bs)
{
	bs.setbits(SYSTEM_HEADER_START_CODE,32);

	bs.setbits(header_length,16);
	bs.set1bit();
	bs.setbits(rate_bound,22);
	bs.set1bit();
	bs.setbits(audio_bound,6);
	bs.set1bit(fixed_flag);
	bs.set1bit(CSPS_flag);
	bs.set1bit(system_audio_lock_flag);
	bs.set1bit(system_video_lock_flag);
	bs.set1bit();
	bs.setbits(video_bound,5);
	bs.setbits(0xff,8);

	for(int i=0;i< stream_count;i++){
		bs.setbits(stream_id[i],8);
		bs.setbits(3,2);
		bs.set1bit(PSTD_buffer_bound_scale[i]);
		bs.setbits(PSTD_buffer_size_bound[i],13);
	}

	return true;
}
