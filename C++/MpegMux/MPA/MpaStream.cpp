#include "MpaStream.h"
#include "mpa.h"

MpaStream::MpaStream(const string _filename /* = "" */,bool write /* = false */)
	:BitStream(_filename,write)
{
	frameCount = 0;
	bitrate = 0;
	sampling_frequency = 0;
}

MpaStream::~MpaStream()
{
}

bool MpaStream::updateInfo()
{
	if( ! seek_sync(0xfff,12) )
		return false;

	MpaFrame frame;
	frame.parse(*this);
	bitrate = frame.info.bitrate;
	sampling_frequency = frame.info.sampling_frequency;

	reset( false );

	return true;
}