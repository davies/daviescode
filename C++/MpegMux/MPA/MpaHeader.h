#ifndef __MPAHEADER_H__
#define __MPAHEADER_H__

#include "MpaStream.h"

#define		real double

#define		MAXFRAMESIZE		1792*2

#define         SBLIMIT                 32
#define			SCALE_BLOCK				12

#define         MPG_MD_STEREO           0
#define         MPG_MD_JOINT_STEREO     1
#define         MPG_MD_DUAL_CHANNEL     2
#define         MPG_MD_MONO             3

struct al_table 
{
  short bits;
  short d;
};

class MpaHeaderInfo
{
public:
	MpaHeaderInfo(){};
	bool check_head_code(unsigned int head);
	bool parse(MpaStream &bs);
	void select_table();
	bool encode(MpaStream &bs);
	unsigned int getFrameSize();

public:
	unsigned long head;

    int lay;
    int error_protection;
    int bitrate_index;
    int sampling_frequency_index;
    bool padding;
    bool extension;
    int mode;
    int mode_ext;
	bool copyright;
	bool original_or_copy;
	unsigned char emphasis;

	int bitrate;
	int sampling_frequency;

 	al_table *alloc;
    int II_sblimit;
    int stereo;
    int jsbound;

	static long freqs[9];
	static int bitrating_123[3][16];
};

#endif

