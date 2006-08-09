#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <math.h>
#include <STRING>

using namespace std;

#define MAXFRAME_SIZE		2048
#define BITSTREAM_BUFF_SIZE (MAXFRAME_SIZE*16) 

class BitStream
{
public:
	BitStream(const string _filename = "",bool write = false);
	~BitStream();

	bool isOK(){ return bitfile != NULL; };
//	string getFileName(){ return filename;};
	FILE *getFILE( ){return bitfile;}
	bool end_bs(){ return eobs; };

	bool get1bit();
	void set1bit(bool val = true);
	unsigned int getbits(unsigned int N);
	void setbits(unsigned int val,unsigned int length);
	unsigned int nextbits(unsigned int N);
	bool seek_sync(unsigned int sync, unsigned int length);
	void alignbits();
	int read(unsigned char* _buf, unsigned int length);
	int write(unsigned char* _buf,unsigned int length);
//	void set_sync(unsigned int sync,int length);

	unsigned long getCurrPos(){
		return lastBytes + byteidx;
	}
	unsigned long getBitsCount(){
		return (lastBytes + byteidx)*8 + bitidx;
	}

	unsigned long getBytesLast(){
		return _filelength(bitfile->_file) - getCurrPos();
	}
	void reset(bool flush = true );

protected:
//	string filename;
	FILE *bitfile;
	unsigned char buff[BITSTREAM_BUFF_SIZE];
	bool writable;
	bool eobs;

	unsigned int byteidx;
	unsigned int bitidx;
	unsigned int bufcount;
	unsigned long lastBytes;
	unsigned long filelength;

	bool refill_buffer();
	void flush_buffer();
};
#endif