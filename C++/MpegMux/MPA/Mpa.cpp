#include "mpa.h"

static int base[3][9] = {
		{ 1 , 0, 2 , } ,
		{ 17, 18, 0 , 19, 20 , } ,
		{ 21, 1, 22, 23, 0, 24, 25, 2, 26 } 
	};
static int trans[10] = {0,0,0,0,1,1,2,2,2,2	};

real MpaFrame::muls[27][64];	/* also used by layer 1 */

/*
 *初始化muls[][]
 */
void MpaFrame::init(void)
{
	static double mulmul[27] = {
		0.0 , -2.0/3.0 , 2.0/3.0 ,
		2.0/7.0 , 2.0/15.0 , 2.0/31.0, 2.0/63.0 , 2.0/127.0 , 2.0/255.0 ,
		2.0/511.0 , 2.0/1023.0 , 2.0/2047.0 , 2.0/4095.0 , 2.0/8191.0 ,
		2.0/16383.0 , 2.0/32767.0 , 2.0/65535.0 ,
		-4.0/5.0 , -2.0/5.0 , 2.0/5.0, 4.0/5.0 ,
		-8.0/9.0 , -4.0/9.0 , -2.0/9.0 , 2.0/9.0 , 4.0/9.0 , 8.0/9.0 
	};
	int i,j,k;
	real *table;

	for(k=0;k<27;k++)
	{
		double m=mulmul[k];
		table = muls[k];
		for(j=3,i=0;i<63;i++,j--)
			*table++ = m * pow(2.0,(double) j / 3.0);
		*table++ = 0.0;
	}
}



//////////////////////////////////////////////////////////////////////////
/*
 *	MpaFrame
 */
MpaFrame::MpaFrame()
{
	memset(allocation,0,sizeof(allocation));
	memset(scalefactor,0,sizeof(scalefactor));
	memset(sample,0,sizeof(sample));
}

inline unsigned char MpaFrame::getLEI(unsigned char dscf)
{
	if( dscf <= -3 ){
		return 1;
	}else if(dscf < 0){
		return 2;
	}else if(dscf == 0){
		return 3;
	}else if(dscf < 3){
		return 4;
	}else {
		return 5;
	}
}

unsigned char MpaFrame::getSCFSI( unsigned char scalefactor[3])
{
	static unsigned char scfsi_table[5][5] = {
		{0,3,3,3,0},
		{1,2,2,2,1},
		{1,2,2,2,1},
		{1,2,2,2,1},
		{0,3,3,3,0}
	};
	
	unsigned char ld1 = getLEI(scalefactor[0] - scalefactor[1]);
	unsigned char ld2 = getLEI(scalefactor[1] - scalefactor[2]);
	
	return scfsi_table[ld1-1][ld2-1];
}

bool MpaFrame::parse(MpaStream &bs)
{
	info.parse( bs );
	//看这一帧是否完整
	if( bs.getBytesLast() < info.getFrameSize() ){
		return false;
	}

	if ( info.error_protection ) {
		bs.getbits(16); //CRC check
	}
	
	int sb,ch,s,k;
	int bits;
	int nch = info.stereo;
    int sblimit = info.II_sblimit;
    int bound = info.jsbound;
    struct al_table *alloc = info.alloc;

	//比特分配
	for ( sb=0;sb< bound;sb ++ ){
		bits = alloc->bits;
		for( ch=0;ch<nch;ch++){
			allocation[ch][sb] = bs.getbits(bits);
		}
		alloc += 1 << bits;
	}
	for( sb=bound;sb < sblimit;sb ++){
		bits = alloc->bits;
		allocation[0][sb] = allocation[1][sb] = bs.getbits(bits);
		alloc += 1 << bits;
	}

	//scfsi
	unsigned char scfsi[2][SBLIMIT];
	for(sb=0;sb<sblimit;sb++){
		for( ch=0;ch<nch;ch++){
			if( allocation[ch][sb] != 0){
				scfsi[ch][sb] = bs.getbits(2);
			}
		}
	}

	//比例因子
	for(sb=0;sb<sblimit;sb++){
		for( ch=0;ch<nch;ch++){
			if( allocation[ch][sb] != 0){
				if( scfsi[ch][sb] == 0 ){
					scalefactor[ch][sb][0] = bs.getbits(6);
					scalefactor[ch][sb][1] = bs.getbits(6);
					scalefactor[ch][sb][2] = bs.getbits(6);
				}else if( scfsi[ch][sb] == 1) {
					scalefactor[ch][sb][0] = scalefactor[ch][sb][1] = bs.getbits(6);
					scalefactor[ch][sb][2] = bs.getbits(6);
				}else if( scfsi[ch][sb] == 2 ){
					scalefactor[ch][sb][0] = scalefactor[ch][sb][1] = scalefactor[ch][sb][2] = bs.getbits(6);
				}else {
					scalefactor[ch][sb][0] = bs.getbits(6);
					scalefactor[ch][sb][2] = scalefactor[ch][sb][1] = bs.getbits(6);
				}
			}
		}
	}

	//样点
	for(s=0;s<SCALE_BLOCK;s++)
	{
		alloc = info.alloc;
		for(sb=0;sb<bound;sb++)
		{
			for(ch=0;ch<nch;ch++)
			{
				int n = allocation[ch][sb];
				if( n ){
					if( alloc[n].d  < 0 ){
						for( k=0;k<3;k++){
							sample[ch][sb][k][s] = bs.getbits(alloc[n].bits);
						}
					}else{
						unsigned int samplecode = bs.getbits(alloc[n].bits);
						int clevel = alloc[n].d ;
						for( k=0;k<3;k++){
							sample[ch][sb][k][s] = samplecode % clevel;
							if( sample[ch][sb][k][s] < 0){
								sample[ch][sb][k][s] = 0;
							}
							samplecode /= clevel;
						}
					}
				}
			}
			alloc += 1<<alloc->bits;
		}
		for( sb=bound;sb < sblimit;sb ++){
			int n = allocation[0][sb];
			if( n ){
				if( alloc[n].d  < 0 ){
					real cm=muls[ alloc[n].bits ][ scalefactor[ch][sb][s>>2] ];
					for( int k=0;k<3;k++)
						sample[0][sb][k][s] = sample[1][sb][k][s] = bs.getbits(alloc[n].bits);
				}else{
					unsigned char samplecode = bs.getbits(alloc[n].bits);
					int clevel = alloc[n].d ;
					for( k=0;k<3;k++){
						sample[0][sb][k][s] = sample[1][sb][k][s] = samplecode % clevel;
						samplecode /= clevel;
					}
				}
			}
			alloc += 1<<alloc->bits;
		}
	}

	return true;
}

bool MpaFrame::encode(MpaStream &bs)
{
	//
	while( getFrameSize() > info.getFrameSize()*8 ) {
		if (info.bitrate_index >= 14 ) {
			return false;
		}
		al_table * old_alloc = info.alloc;
		int old_sblimit = info.II_sblimit;
		info.bitrate_index += 1;
		info.select_table();
		if( old_alloc != info.alloc){
			reSample(0,old_alloc,info.alloc,old_sblimit,info.II_sblimit);
			reSample(1,old_alloc,info.alloc,old_sblimit,info.II_sblimit);
		}
	}

	info.error_protection = false;
	info.encode(bs);
	
	int sb,ch,s,k;
	int nch = info.stereo;
    int sblimit = info.II_sblimit;
    int bound = info.jsbound;
    struct al_table *alloc = info.alloc;
	
	//比特分配
	int bits;
	for ( sb=0;sb< bound;sb ++ ){
		bits = alloc->bits;
		alloc += 1 << bits;
		for( ch=0;ch<nch;ch++){
			bs.setbits(allocation[ch][sb],bits);
		}
	}
	for( sb=bound;sb < sblimit;sb ++){
		bits = alloc->bits;
		alloc += 1 << bits;
		bs.setbits(allocation[0][sb],bits);
	}

	//scfsi
	unsigned char scfsi[2][SBLIMIT] = {0,};
	for(sb=0;sb<sblimit;sb++){
		for( ch=0;ch<nch;ch++){
			if( allocation[ch][sb] != 0){
				scfsi[ch][sb] = getSCFSI( scalefactor[ch][sb] ); //
				bs.setbits(scfsi[ch][sb],2);
			}
		}
	}

	//比例因子
	for(sb=0;sb<sblimit;sb++){
		for( ch=0;ch<nch;ch++){
			if( allocation[ch][sb] != 0){
				if( scfsi[ch][sb] == 0 ){
					bs.setbits(scalefactor[ch][sb][0],6);
					bs.setbits(scalefactor[ch][sb][1],6);
					bs.setbits(scalefactor[ch][sb][2],6);
				}else if( scfsi[ch][sb] == 2 ){
					bs.setbits(scalefactor[ch][sb][0],6);
				}else {
					bs.setbits(scalefactor[ch][sb][0],6);
					bs.setbits(scalefactor[ch][sb][2],6);
				}
			}
		}
	}

	//样点
	for(s=0;s<SCALE_BLOCK;s++){
		alloc = info.alloc;
		for(sb=0;sb<bound;sb++){
			for(ch=0;ch<nch;ch++){
				int n = allocation[ch][sb];
				if( n ){
					if( alloc[n].d < 0 ){
						for( k=0;k<3;k++)
							bs.setbits(sample[ch][sb][k][s],alloc[n].bits);
					}else{
						unsigned int clevel = alloc[n].d ;
						unsigned int samplecode = sample[ch][sb][0][s];
						samplecode += sample[ch][sb][1][s]*clevel;
						samplecode += sample[ch][sb][2][s]*clevel*clevel;
						bs.setbits(samplecode,alloc[n].bits);
					}
				}
			}
			alloc += 1<<alloc->bits;
		}
		for( sb=bound;sb < sblimit;sb ++){
			int n = allocation[0][sb];
			if( n ){
				if( alloc[n].d  < 0 ){
					for( int k=0;k<3;k++)
						bs.setbits(sample[0][sb][k][s],alloc[n].bits);
				}else{
					unsigned int clevel = alloc[n].d ;
					unsigned int samplecode = sample[0][sb][0][s];
					samplecode += sample[0][sb][1][s]*clevel;
					samplecode += sample[0][sb][2][s]*clevel*clevel;
					bs.setbits(samplecode,alloc[n].bits);
				}
			}
			alloc += 1<<alloc->bits;
		}
	}

	int length = getFrameSize();
	if( length & 0x7){
		bs.setbits(0, 8 - length & 0x7 );
		length += 8 - length & 0x7;
	}
	length >>= 3;
	for( int i=length;i<info.getFrameSize();i++)
		bs.setbits(0,8);

	return true;
}

/*
 *根据音频数据计算所生成的数据流的长度
 */
unsigned int MpaFrame::getFrameSize()
{
	int sb,ch,s;
	int nch = info.stereo;
    int sblimit = info.II_sblimit;
    int bound = info.jsbound;
    struct al_table *alloc = info.alloc;
	
	unsigned long sum = 0;

	//比特分配
	int bits;
	for ( sb=0;sb< bound;sb ++ ){
		bits = alloc->bits;
		alloc += 1 << bits;
		sum += bits*nch;
	}
	for( sb=bound;sb < sblimit;sb ++){
		bits = alloc->bits;
		alloc += 1 << bits;
		sum += bits;
	}

	//scfsi
	unsigned char scfsi[2][SBLIMIT] = {0,};
	for(sb=0;sb<sblimit;sb++){
		for( ch=0;ch<nch;ch++){
			if( allocation[ch][sb] != 0){
				scfsi[ch][sb] = getSCFSI( scalefactor[ch][sb] ); //
				sum += 2; 
			}
		}
	}

	//比例因子
	for(sb=0;sb<sblimit;sb++){
		for( ch=0;ch<nch;ch++){
			if( allocation[ch][sb] != 0){
				if( scfsi[ch][sb] == 0 ){
					sum += 6*3;
				}else if( scfsi[ch][sb] == 2 ){
					sum += 6;
				}else {
					sum += 6*2;
				}
			}
		}
	}

	//样点
	for(s=0;s<SCALE_BLOCK;s++){
		alloc = info.alloc;
		for(sb=0;sb<bound;sb++){
			for(ch=0;ch<nch;ch++){
				int n = allocation[ch][sb];
				if( n ){
					if( alloc[n].d < 0 )
						sum += alloc[n].bits * 3;
					else
						sum += alloc[n].bits;
				}
			}
			alloc += 1<<alloc->bits;
		}
		for( sb=bound;sb < sblimit;sb ++){
			int n = allocation[0][sb];
			if( n ){
				if( alloc[n].d  < 0 ){
					sum += alloc[n].bits * 3;
				}else{
					sum += alloc[n].bits;
				}
			}
			alloc += 1<<alloc->bits;
		}
	}

	return sum;//(sum >> 3) + ((sum & 0x07)?1:0);
}

/*
 *用新的量化表重新量化处理
 */
void MpaFrame::reSample(int ch,al_table *old_alloc,al_table *new_alloc,
			  int old_sblimit,int new_sblimit)
{

	real fraction[SCALE_BLOCK] = {0.0,};
	int sb,s,k;
	int sblimit = old_sblimit < new_sblimit ? old_sblimit : new_sblimit ;
	int bits,d;

	for(sb=0;sb<sblimit;sb++)
	{
		if( !allocation[ch][sb] ) continue;
		int n = allocation[ch][sb];
		for(k=0;k<3;k++){
			bits = old_alloc[n].bits;
			d = old_alloc[n].d;
			real max = 0.0;
			for(s=0;s<12;s++){
				if( d < 0){
					fraction[s] = (real)( (signed int)sample[ch][sb][k][s] + d )
										* muls[ bits ][ scalefactor[ch][sb][k] ] ;
				}else{
					int tab = base[ trans[d] ][ sample[ch][sb][k][s] ];
					fraction[s] = muls[tab][ scalefactor[ch][sb][k] ] ;
				}
				if( fabs(fraction[s]) > max )
					max = fabs(fraction[s]);
			}

			//获得比例因子
			bits = new_alloc[n].bits;
			d = new_alloc[n].d;
			if( d < 0 ){
				max /= -d +1;
				for (int i=63;i>=0;i--){
					if( muls[bits][i] > max ){
						scalefactor[ch][sb][k] = i;
						break;
					}
				}
				for (s=0;s<SCALE_BLOCK;s++)
					sample[ch][sb][k][s] = (int)(fraction[s]/muls[bits][ scalefactor[ch][sb][k] ]) - d;
			}else{
				int index = trans[d];
				int tab = base[index][d-1];
				for (int i=63;i>=0;i--){
					if( muls[tab][i] >= max ){
						scalefactor[ch][sb][k] = i;
						break;
					}
				}
				for (s=0;s<SCALE_BLOCK;s++){
					real min = fabs(fraction[s]);
					for( int i=0;i<d;i++){
						if( fabs(muls[ base[index][i] ][ scalefactor[ch][sb][k] ] - fraction[s]) <= min ){
							sample[ch][sb][k][s] = i;
							min = fabs(muls[ base[index][i] ][ scalefactor[ch][sb][k] ] - fraction[s] );
						}
					}
				}
			}
		}
		old_alloc += 1 << old_alloc->bits;	//移动指针
		new_alloc += 1 << new_alloc->bits;
	}

	//原来没有的数据,作零处理
	if( new_sblimit > old_sblimit ){
		for( sb=old_sblimit;sb<new_sblimit;sb++ )
			allocation[ch][sb] = 0;
	}
}

void MpaFrame::mix()
{
	real fraction[SCALE_BLOCK] = {0.0,};
	int sb,s,k;
    int sblimit = info.II_sblimit;
    struct al_table *alloc = info.alloc;

	for(sb=0;sb<sblimit;sb++)
	{
		int n= allocation[0][sb];
		if( allocation[1][sb] > n ){
			n = allocation[1][sb];
		}
		if( !n ) continue;

		for(k=0;k<3;k++){
			real max = 0.0;
			for(s=0;s<12;s++)
			{
				//合并通道,并求每12个样点的最大值
				int n0 = allocation[0][sb];
				int n1 = allocation[1][sb];
				fraction[s] = 0.0;
				if( n0 ){
					int bits0 = alloc[n0].bits;
					int d0 = alloc[n0].d;
					if( d0 < 0)
						fraction[s] += (real)( (signed int)sample[0][sb][k][s] + d0 )
											* muls[ bits0 ][ scalefactor[0][sb][k] ] ;
					else{
						int tab = base[ trans[d0] ][ sample[0][sb][k][s] ];
						fraction[s] += muls[tab][ scalefactor[0][sb][k] ] ;
					}
				}
				if( n1 ){
					int bits1 = alloc[n1].bits;
					int d1 = alloc[n1].d;
					if( d1 < 0)
						fraction[s] += (real)( (signed int)sample[1][sb][k][s] + d1 ) 
											* muls[ bits1 ][ scalefactor[1][sb][k] ] ;
					else{
						int tab = base[ trans[d1] ][ sample[1][sb][k][s] ];
						fraction[s] += muls[tab][ scalefactor[1][sb][k] ];
					}
				}
				fraction[s] /= 2.0;

				if( fabs(fraction[s]) > max )
					max = fabs(fraction[s]);
			}

			//获得比例因子
			int bits = alloc[n].bits;
			int d = alloc[n].d;
			if( d < 0 ){
				max /= -d +1;
				for (int i=63;i>=0;i--){
					if( muls[bits][i] > max ){
						scalefactor[0][sb][k] = i;
						break;
					}
				}
				for (s=0;s<SCALE_BLOCK;s++)
					sample[0][sb][k][s] = (int)(fraction[s]/muls[bits][ scalefactor[0][sb][k] ]) - d;
			}else{
				int index = trans[d];
				int tab = base[index][d-1];
				for (int i=63;i>=0;i--){
					if( muls[tab][i] >= max ){
						scalefactor[0][sb][k] = i;
						break;
					}
				}
				for (s=0;s<SCALE_BLOCK;s++){
					real min = fabs(fraction[s]);
					for( int i=0;i<d;i++){
						if( fabs(muls[ base[index][i] ][ scalefactor[0][sb][k] ] - fraction[s]) <= min ){
							sample[0][sb][k][s] = i;
							min = fabs(muls[ base[index][i] ][ scalefactor[0][sb][k] ] - fraction[s] );
						}
					}
				}
			}
		}
		allocation[0][sb] = n;
		alloc += 1 << alloc->bits;	//移动指针
	}
}

bool MpaFrame::mux(const MpaFrame& leftFrame,const MpaFrame& rightFrame)
{
	if (leftFrame.info.sampling_frequency != rightFrame.info.sampling_frequency ) {
		fprintf(stderr,"Can't unite two audio with different sampling frequencies!\n");
		return false;
	}
	memcpy(allocation[0],leftFrame.allocation[0],sizeof(allocation[0]));
	memcpy(scalefactor[0],leftFrame.scalefactor[0],sizeof(scalefactor[0]));
	memcpy(sample[0],leftFrame.sample[0],sizeof(sample[0]));

	memcpy(allocation[1],rightFrame.allocation[0],sizeof(allocation[1]));
	memcpy(scalefactor[1],rightFrame.scalefactor[0],sizeof(scalefactor[1]));
	memcpy(sample[1],rightFrame.sample[0],sizeof(sample[0]));
	
	//调整比特律，如果量化表发生改变则重新量化
	if( leftFrame.info.bitrate_index >= leftFrame.info.bitrate_index ){
		info = leftFrame.info;
		if( info.alloc != rightFrame.info.alloc ){
			reSample(1,rightFrame.info.alloc,leftFrame.info.alloc,
				rightFrame.info.II_sblimit,leftFrame.info.II_sblimit);	//只需根据新的量化表重新量化
		}
	}else{
		info = rightFrame.info;
		if( info.alloc != leftFrame.info.alloc ){
			reSample(0,leftFrame.info.alloc,rightFrame.info.alloc,
				leftFrame.info.II_sblimit,rightFrame.info.II_sblimit);
		}
	}

	//因为混合的两个声道的声音不同，不用混合立体声
	info.mode = 0;
	info.mode_ext = 0;
	info.select_table();

	return true;
}

double MpaFrame::getMaxVol(int ch)
{
	double maxVol = 0.0,vol;
	int sb,k;
    struct al_table *alloc = info.alloc;

	for(sb=0;sb<info.II_sblimit;sb++){
		int n = allocation[ch][sb];
		int bits = alloc[n].bits;
		int d = alloc[n].d;
		if( !n ) continue;
		for(k=0;k<3;k++){
			if ( d < 0) {
				if( (vol = -d * muls[ bits ][ scalefactor[ch][sb][k] ]) > maxVol ){
					maxVol = vol;
				}
			}else{
				if ( (vol = -muls[ base[trans[d]][0] ][ scalefactor[ch][sb][k] ] ) > maxVol ) {
					maxVol = vol;
				}
			}
		}
	}

	return maxVol;
}

void MpaFrame::multi(int leftD,int rightD)
{
	int ch,sb,k;
	int dscalefactor[2]; 
	dscalefactor[0] = leftD;
	dscalefactor[1] = rightD;

	for(ch=0;ch<info.stereo;ch++){
		for(sb=0;sb<info.II_sblimit;sb++){
			if( !allocation[ch][sb] ) continue;
			for(k=0;k<3;k++){
				scalefactor[ch][sb][k] -= dscalefactor[ch];
				if (scalefactor[ch][sb][k] <0) {
					scalefactor[ch][sb][k] = 0;
				}
				if (scalefactor[ch][sb][k] > 62) {
					scalefactor[ch][sb][k] = 62;
				}
			}
		}
	}
}

void MpaFrame::swapCh()
{
	unsigned char tmp_allocation[SBLIMIT];
	unsigned char tmp_scalefactor[SBLIMIT][3];
	unsigned int tmp_sample[SBLIMIT][3][SCALE_BLOCK];

	memcpy(tmp_allocation,allocation[0],sizeof(allocation[0]));
	memcpy(tmp_scalefactor,scalefactor[0],sizeof(scalefactor[0]));
	memcpy(tmp_sample,sample[0],sizeof(sample[0]));
	
	memcpy(allocation[0],allocation[1],sizeof(allocation[0]));
	memcpy(scalefactor[0],scalefactor[1],sizeof(scalefactor[0]));
	memcpy(sample[0],sample[1],sizeof(sample[0]));

	memcpy(allocation[1],tmp_allocation,sizeof(allocation[0]));
	memcpy(scalefactor[1],tmp_scalefactor,sizeof(scalefactor[0]));
	memcpy(sample[1],tmp_sample,sizeof(sample[0]));
}
