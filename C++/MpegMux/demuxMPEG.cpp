  #include "muxer.h"

void Muxer::demux()
{
	int i;
	Pack pack;
	int packCount = 0;
	
	fprintf(stdout,"start demuxing %s ...",inFileName.c_str());

	while ( inMPEG->seek_sync(PACK_START_CODE,32) ) {
		pack.reset();
		if (!pack.parse(*inMPEG) ) {
			continue;
		}
		packCount ++;
		if ( pack.hasSystemHeader() ) {
			version = pack.getVersion();
			audio_stream_count = pack.getSystemHeader().getAudioStreamCount();
			video_PSTD_buffer_size_bound = pack.getSystemHeader().getVideoBufferBound();
		}
		if( pack.isAudioStream() ){
			int audioIndex = pack.getAudioIndex();
			if ( audioIndex < 0 ) {
				continue;
			}
			if ( audio_stream_count == 1 ) {
				if ( !audioBS[0] ) {
					audioType[0] = pack.getAudeoStreamType();
					audioBS[0] = new MpaStream;
					delay[0] = pack.getPTS();
				}
				pack.writeData( *audioBS[audioIndex] );
			}else{
				if ( audioIndex == leftChIndex ) {
					//左声道
					if ( !audioBS[0] ) {
						audioType[0] = pack.getAudeoStreamType();
						audioBS[0] = new MpaStream;
						delay[0] = pack.getPTS();
					} 
					pack.writeData( *audioBS[0] );
				}else if ( audioIndex == rightChIndex ) {
					//右声道
					if ( !audioBS[1] ) {
						audioType[1] = pack.getAudeoStreamType();
						audioBS[1] = new MpaStream;
						delay[1] = pack.getPTS();
					} 
					pack.writeData( *audioBS[1] );
				}
			}
		}
		if ( pack.isVideoStream() ) {
			static bool last_not_found = false;
			static unsigned int last_word  = 0;
			double PTS_value = pack.getPTS();
			if ( !videoBS ) {
				videoBS = new VideoStream;
				//video_delay = pack.getPTS();
			}
			bool found_sync = false;

			//处理同步字跨越两个packet的情况
			if ( last_not_found ) {
				uint32 first_word = pack.getPacket(0)->getFirstBytes(3);
				if ( ( (last_word&0xff)<<24 | first_word) == GROUP_START_CODE ) {
					videoBS->seqIndex[videoBS->seqCount++] = videoBS->getCurrPos() - 1;
					found_sync = true;
				}else if ( ( (last_word&0xffff)<<16 | (first_word>>8))== GROUP_START_CODE ) {
					videoBS->seqIndex[videoBS->seqCount++] = videoBS->getCurrPos() - 2;
					found_sync = true;
				}else if ( ( (last_word&0xffffff)<<8 | (first_word>>16))== GROUP_START_CODE ) {
					videoBS->seqIndex[videoBS->seqCount++] = videoBS->getCurrPos() - 3;
					found_sync = true;
				}
				last_not_found = false;
			}
			int firstSyncPos = pack.getPacket(0)->getFirstSyncPos(GROUP_START_CODE,32);
			if ( firstSyncPos >= 0 ) {
				videoBS->seqIndex[videoBS->seqCount++] = videoBS->getCurrPos() + firstSyncPos;
				found_sync = true;
			}else{
				last_not_found = true;
				last_word = pack.getPacket(0)->getLastBytes(3);
			}
			if ( found_sync ) {
				if ( video_dPTS == 0 && videoBS->seqCount > 5 && PTS_value > 0 ) {
					video_dPTS = (PTS_value - videoBS->seqTime[0]) / (videoBS->seqCount-1);
					video_PTS_DTS = pack.getPacket(0)->getDTS() - PTS_value;
				}
				videoBS->seqTime[videoBS->seqCount-1] = PTS_value;
			}
			if ( video_delay == 0 ) {
				int firstSyncPos = pack.getPacket(0)->getFirstSyncPos(GROUP_START_CODE,32);
				if ( firstSyncPos >=0 ) {
					video_delay = pack.getPTS();
				}
			}
			pack.writeData( *videoBS );
		}
	}
	//恢复数据流
	videoBS->seqIndex[videoBS->seqCount] = videoBS->getCurrPos();
	videoBS->seqTime[videoBS->seqCount] = 
		videoBS->seqTime[videoBS->seqCount-1]*2 - videoBS->seqTime[videoBS->seqCount-2];
	{
		int first_seq = 0;
		while (videoBS->seqTime[first_seq] == 0) {
			first_seq ++;
		}
		int last_seq = videoBS->seqCount-2;
		while ( videoBS->seqTime[last_seq] == 0) {
			last_seq --;
		}
		double dTime = (videoBS->seqTime[last_seq]-videoBS->seqTime[first_seq])
			/ (last_seq-first_seq);
		//处理时间为零的序列
		for(i=first_seq+1;i<last_seq;i++){
			if ( videoBS->seqTime[i] == 0.0 ) {
				videoBS->seqTime[i] = videoBS->seqTime[i-1]+dTime;
			}
		}
	}
	videoBS->reset();
	video_bit_rate = getVideoBitrate(videoBS);
//	videoBS->CreateIndex()

	//	{
//	int last_pos = 0;
//	int curr_pos = 0;
//	double last_time = 0;
//	for (int j=0;j<=videoBS->seqCount;j++){
//		curr_pos = videoBS->seqIndex[j];
//		fprintf(stdout,"%3d = %7d(+%4d)  %5f[+%5f]\n",j,curr_pos,curr_pos-last_pos,
//			videoBS->seqTime[j],videoBS->seqTime[j]-last_time);
//		last_pos = curr_pos;
//		last_time = videoBS->seqTime[j];
//	}
//	}

	for(i=0;i<MAX_AUDIO_STREAMS;i++){
		if( audioBS[i] ){
			audioBS[i]->reset();
		}
	}

	delete inMPEG;
	inMPEG = NULL;

	fprintf(stdout,"done.\n");
}

