#ifndef AC3FILE_H
#define AC3FILE_H

#include <stdio.h>
#include "decoder.h"

class AC3FileDecoder : public PullDecoder
{
protected:
	bool     close_f;
	virtual long get(uint8_t *buf, int len);
	FILE    *f;
	
public:
	
	AC3FileDecoder(FILE *_f,             Speakers spk = Speakers(MODE_STEREO));
	AC3FileDecoder(const char *filename, Speakers spk = Speakers(MODE_STEREO));
	~AC3FileDecoder();
	
	virtual bool ok() { return f != 0; };
	bool eof() { return feof(f);}
	int filesize()    { return filelength(f->_file); };
	
	void init();
};


#endif
