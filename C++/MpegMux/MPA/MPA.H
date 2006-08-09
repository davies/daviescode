#ifndef __MPA_H__
#define __MPA_H__

//#include "bitstream.h"
#include "MpaStream.h"
#include "MpaHeader.h"

class MpaFrame
{
public:
	MpaFrame();
	~MpaFrame(){};

	static void init();

	bool parse(MpaStream &ms);
	bool encode(MpaStream &ms);
	unsigned int getFrameSize();
	void reSample(int ch,al_table *old_alloc,al_table *new_alloc,
			  int old_sblimit,int new_sblimit);
	void mix();
	bool mux(const MpaFrame& leftFrame,const MpaFrame& rightFrame);

	double getMaxVol(int ch);
	void multi(int left,int right);
	
	void swapCh();

	MpaHeaderInfo info;

protected:
	unsigned char getLEI(unsigned char dscf);
	unsigned char getSCFSI( unsigned char scalefactor[3]);

private:

	unsigned char allocation[2][SBLIMIT];
	unsigned char scalefactor[2][SBLIMIT][3];
	unsigned int sample[2][SBLIMIT][3][SCALE_BLOCK];

	static real muls[27][64];
};

#endif