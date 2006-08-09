#ifndef CONVERTER_H
#define CONVERTER_H

#include "vac3dec\ac3file.h"
#include "MpaEncoder\mpafile.h"

#define BLOCKS_PER_BUFFER	6*3
#define FRAMES_PER_BUFFER	4

typedef sample_t sample_swap_buff[2][NSAMPLES*BLOCKS_PER_BUFFER];

class Converter 
{
public:
    Converter( const char* inputAC3File, const char *outputMPAFile);
    Converter( FILE* inputAC3File, FILE *outputMPAFile);

	bool ok() { return ac3File.ok() && mpaFile.ok(); };
	
	void init();
    void convert();
	
private:
	AC3FileDecoder ac3File;
	MPAFileEncoder mpaFile;
	sample_swap_buff swap_buf;
};

#endif
