#include "muxer.h"

double Muxer::getMaxVol( MpaStream *bs,int ch)
{
	double maxVol = 0.0;
	double vol = 0.0;
	MpaFrame frame;
	
	while ( bs->seek_sync(0xfff,12) ) {
		frame.parse(*bs);
		vol = frame.getMaxVol(ch);
		if( vol > maxVol)
			maxVol = vol;
	}
	
	bs->reset();
	return maxVol;
}

#define max(a,b) ( (a) > (b) ? (a) : (b) )

MpaStream* Muxer::normalize(MpaStream *inMS,double leftLevel,double rightLevel,
							bool swap)
{
	//不进行normalize
	MpaFrame frame;
	if ( leftLevel == 0.0 && rightLevel == 0.0 && !swap ) {
		inMS->frameCount = 0;
		while ( inMS->seek_sync(0xfff,12) ) {
			inMS->frameIndex[inMS->frameCount++] = inMS->getCurrPos();	//建立帧索引
			if( !frame.parse(*inMS) )
				break;
		}
		inMS->frameIndex[inMS->frameCount++] = inMS->getCurrPos();	//建立帧索引
		inMS->reset(false);
		inMS->updateInfo();
		return inMS;
	}
	
	MpaStream* outBS = new MpaStream;

	double leftScale = (leftLevel==0) ? 1 : leftLevel / getMaxVol(inMS,0);
	double rightScale = (rightLevel==0) ? 1 : rightLevel / getMaxVol(inMS,1);

	int left_dscalefactor = log10(leftScale)/log10(2.0) * 3.0;
	int right_dscalefactor = log10(rightScale)/log10(2.0) * 3.0;

	outBS->frameCount = 0;
	outBS->frameIndex[outBS->frameCount++] = 0;
	while ( inMS->seek_sync(0xfff,12) ) {
		if( !frame.parse(*inMS) )
			break;
		frame.multi(left_dscalefactor,right_dscalefactor);
		if ( swap ) {
			frame.swapCh();
		}
		frame.encode(*outBS);
		outBS->frameIndex[outBS->frameCount++] = outBS->getCurrPos();	//建立帧索引
	}

	outBS->frameIndex[outBS->frameCount] = _filelength(outBS->getFILE()->_file);

	outBS->reset();
	outBS->updateInfo();

	delete inMS;
	return outBS;
}


void Muxer::normalize( )
{
	fprintf(stdout,"start normalizing audio(s)  ... ");

	for(int i=0;i<MAX_AUDIO_STREAMS;i++){
		if( audioBS[i] && audioType[i] == AUDIOSTREAM_TYPE_MPA ){
			if ( audio_stream_count == 1 ) {
				audioBS[i] = normalize( audioBS[i] ,leftLevel,rightLevel, leftChIndex > rightChIndex );
			}else{
				double level = i ? rightLevel : leftLevel;
				audioBS[i] = normalize( audioBS[i] ,level,level );
			}
		}
	}

	fprintf(stdout,"done.\n");
}