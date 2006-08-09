#ifndef MPAFILE_H
#define MPAFILE_H

#include "common.h"
#include "encoder.h"

#include <io.h>
    
typedef double SBS[2][3][SCALE_BLOCK][SBLIMIT];
typedef double JSBS[3][SCALE_BLOCK][SBLIMIT];
typedef double IN[2][HAN_SIZE];
typedef unsigned int SUB[2][3][SCALE_BLOCK][SBLIMIT];

class MPAFileEncoder
{
public:
    MPAFileEncoder(const char *filename);
	MPAFileEncoder(FILE *file);
    ~MPAFileEncoder();
    
	bool init( );

    bool ok() { return bs.pt != 0; };
    int filesize()    { return filelength(bs.pt->_file); };
	
	int getLayer(){ return info.lay;}
	bool setSampleRate(int sample_rate) ;
	sample_t* getBuffer(int ch) { return buffer[ch];}

    void encodeAFrame();
	//bool setBitrate();
	//
private:
    Bit_stream_struc   bs;
    bool     close_f;

	SBS  FAR        *sb_sample;
	JSBS FAR        *j_sample;
	IN   FAR        *win_que;
	SUB  FAR        *subband;
   
    unsigned int bit_alloc[2][SBLIMIT], scfsi[2][SBLIMIT];
    unsigned int scalar[2][3][SBLIMIT], j_scale[3][SBLIMIT];
    double FAR ltmin[2][SBLIMIT], lgmin[2][SBLIMIT], max_sc[2][SBLIMIT];
    FLOAT snr32[32];
    short sam[2][1344]; /* was [1056]; */
    int whole_SpF;
    int extra_slot;
    double avg_slots_per_frame, frac_SpF, slot_lag;
    int stereo, error_protection;
    unsigned int crc;
    int i, j, k, adb;
    unsigned long bitsPerSlot, samplesPerFrame;
	unsigned long frameNum;
    unsigned long frameBits, sentBits ;
    unsigned long num_samples;

protected:
    frame_params fr_ps;
    layer info;
    sample_t **win_buf;
    sample_t buffer[2][1152];
 
};

#endif