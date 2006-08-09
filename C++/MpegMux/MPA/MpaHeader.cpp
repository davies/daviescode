#include "MpaHeader.h"
#include "l2tables.h"

//sampling frequency
long MpaHeaderInfo::freqs[9] = { 44100, 48000, 32000,
						22050, 24000, 16000 ,
						11025 , 12000 , 8000 };
//bitrate index for layer 1/2/3
int MpaHeaderInfo::bitrating_123[3][16] = {
    {0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,},
     {0,32,48,56, 64, 80, 96,112,128,160,192,224,256,320,384,},
     {0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320,}
};

/*
 *根据比特律和采样频率等信息选择合适的量化表
 */
void MpaHeaderInfo::select_table()
{
	static int translate[3][2][16] ={ 
		{ { 0,2,2,2,2,2,2,0,0,0,1,1,1,1,1,0 } ,
		{ 0,2,2,0,0,0,1,1,1,1,1,1,1,1,1,0 } } ,
		{ { 0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0 } ,
		{ 0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0 } } ,
		{ { 0,3,3,3,3,3,3,0,0,0,1,1,1,1,1,0 } ,
		{ 0,3,3,0,0,0,1,1,1,1,1,1,1,1,1,0 } } 
	};
	
	int table,sblim;
	static struct al_table *tables[5] = { alloc_0, alloc_1, alloc_2, alloc_3 , alloc_4 };
	static int sblims[5] = { 27 , 30 , 8, 12 , 30 };
	
	table = translate[sampling_frequency_index][2-stereo][bitrate_index];
	sblim = sblims[table];
	
	alloc      = tables[table];
	II_sblimit = sblim;
	jsbound = (mode == MPG_MD_JOINT_STEREO) ?(mode_ext<<2)+4 : II_sblimit;
}

bool MpaHeaderInfo::parse(MpaStream &bs)
{
	if( bs.getbits(12) != 0xfff )
		return false;
	bs.get1bit();
	lay = 4 - bs.getbits(2);

	error_protection = ! bs.get1bit();

    bitrate_index = bs.getbits(4);
    if( ! bitrate_index ){
      fprintf(stderr,"Free format not supported.\n");
      return false;
    }

    sampling_frequency_index = bs.getbits(2);
    padding   = bs.get1bit();
    extension = bs.get1bit();

    mode      = bs.getbits(2);
    mode_ext  = bs.getbits(2);
	copyright = bs.get1bit();
	original_or_copy = bs.get1bit();
	emphasis = bs.getbits(2);
	
    stereo    = (mode == MPG_MD_MONO) ? 1 : 2;
	bitrate = bitrating_123[lay-1][bitrate_index] * 1000;
	sampling_frequency = freqs[sampling_frequency_index];

    switch(lay)
    {
      case 1:
	    jsbound = (mode == MPG_MD_JOINT_STEREO) ? (mode_ext<<2)+4 : 32;
        break;
      case 2:
		select_table();
        break;
      default:
        fprintf(stderr,"Sorry, unknown layer type.\n"); 
        return (false);
    }
    return true;
}

unsigned int MpaHeaderInfo::getFrameSize()
{
	unsigned int framesize;
    switch(lay)
    {
      case 1:
        framesize  = (long) bitrating_123[0][bitrate_index] * 12000;
        framesize /= freqs[sampling_frequency_index];
        framesize  = ((framesize+padding)<<2)-4;
        break;
      case 2:
        framesize = (long) bitrating_123[1][bitrate_index] * 144000;
        framesize /= freqs[sampling_frequency_index];
        framesize += padding - 4;
        break;
      default:
        fprintf(stderr,"Sorry, unknown layer type.\n"); 
        return 0;
    }

	return framesize;
}

bool MpaHeaderInfo::encode(MpaStream &bs)
{
	bs.setbits(0xfff,12);
	bs.set1bit();
	bs.setbits(4-lay,2);
	bs.set1bit(!error_protection);

	bs.setbits(bitrate_index,4);
	bs.setbits(sampling_frequency_index,2);
	bs.set1bit(padding);
	bs.set1bit(extension);

	bs.setbits(mode,2);
	bs.setbits(mode_ext,2);
	bs.set1bit(copyright);
	bs.set1bit(original_or_copy);
	bs.setbits(emphasis,2);

	return true;
}