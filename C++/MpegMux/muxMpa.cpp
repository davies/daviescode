#include "muxer.h"

MpaStream* Muxer::muxMpa(MpaStream* leftCh,MpaStream* rightCh)
{
	MpaStream *outMPA = new MpaStream;	//tmp file
	MpaFrame leftFrame,rightFrame,unitedFrame;
	
	outMPA->frameCount = 0;
	outMPA->frameIndex[outMPA->frameCount++] = 0;

	while ( leftCh->seek_sync(0xfff,12) && rightCh->seek_sync(0xfff,12)) {
		leftFrame.parse(*leftCh);
		rightFrame.parse(*rightCh);
		
		//混合左右声道
//		leftFrame.mix();
//		rightFrame.mix();

		//合并两条音轨到一个音轨的两个声道
		unitedFrame.mux(leftFrame,rightFrame);
		
		unitedFrame.encode(*outMPA);
		outMPA->frameIndex[outMPA->frameCount++] = outMPA->getCurrPos(); //建立帧索引
	}

	leftCh->reset( false );  //not save
	rightCh->reset( false ); //not save
	outMPA->reset();

	outMPA->updateInfo();
	
	return outMPA;
}

bool Muxer::muxMpa()
{
	if ( audio_stream_count < 2 ) {
		fprintf(stdout,"There is only %d audio stream ,no uniting needed. \n",audio_stream_count);
		return false;
	}
	if ( audioBS[0] && audioBS[1] ) {
		mixedAudioIndex = 2;//getUnusedIndex();

		fprintf(stdout,"start uniting audio %d and audio %d to audio %d ... ",leftChIndex+1,rightChIndex+1,mixedAudioIndex+1);
		
		audioBS[mixedAudioIndex] = muxMpa(audioBS[0],audioBS[1]);

		delay[mixedAudioIndex] = delay[0] < delay[1] ? 
			delay[0] : delay[1];
		audio_stream_count ++;

		fprintf(stdout,"done.\n");
	}
	return true;
}
