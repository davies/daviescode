#include "muxer.h"

bool Muxer::mux()
{
	fprintf(stdout,"start muxing video and audio(s) to %s ...",outFileName.c_str());

	int i;
	SystemHeader header;
	header.addStreamInfo(VIDEO_STREAM_0,1,video_PSTD_buffer_size_bound*1.5);
	unsigned long audio_bit_rate_sum = 0;
	for(i=0;i<MAX_AUDIO_STREAMS;i++){
		if( audioBS[i]){
			audio_bit_rate_sum += audioBS[i]->bitrate;
			header.addStreamInfo(AUDIO_STREAM_0+i,0,40);
		}
	}
	int rate_bound = (long double)(audio_bit_rate_sum+video_bit_rate) / 8.0 / 50.0 * 1.05;
	header.setRateBound( rate_bound );

	int next_group_index = 0;
	long double next_cycle[MAX_AUDIO_STREAMS];
	long next_frame_index[MAX_AUDIO_STREAMS];
	int bytes_count[MAX_AUDIO_STREAMS];
	for( i=0;i<MAX_AUDIO_STREAMS;i++){
		next_cycle[i] = delay[i];
		next_frame_index[i] = 0;
		bytes_count[i] = 0;
	}

	long double SCR = video_delay - 0.5;
	for( i=0;i<MAX_AUDIO_STREAMS;i++){
		if ( audioBS[i] && delay[i]-0.2 < SCR ) {
			SCR = delay[i]-0.2;
		}
	}
	if ( SCR < 0.0 ) {
		SCR = 0.0;
	}
	
	int last_pack_length = 0;
	bool finished = false;
	bool needPadding = false;
	int padding_count  = 0;
	//下面的程序使用的 程序流的比特率，会变化
	double mux_rate = (audio_bit_rate_sum/8.0 + 
		(videoBS->seqIndex[1] - videoBS->seqIndex[0]) / ( videoBS->seqTime[1] - videoBS->seqTime[0] )) * 1.05;

	Pack headerPack;
	headerPack.setSystemHeader( header );
	headerPack.setVersion(version);
	headerPack.setProgramMuxRate(rate_bound);
	headerPack.setSCR(SCR);
	headerPack.encode(*outMPEG);	
	SCR += (long double )headerPack.length() / mux_rate;

	Pack audioPack;
	audioPack.setVersion(version);
	audioPack.setProgramMuxRate(rate_bound);
	Pack videoPack;
	videoPack.setVersion(version);
	videoPack.setProgramMuxRate(rate_bound);
	Pack padding;
	padding.setVersion(version);
	padding.addPaddingPacket();
	padding.setProgramMuxRate(rate_bound);
	
	int video_group_count = 0;
	while ( !finished ) {
		finished = true;
		needPadding = true;
		//look for next pack
		for(i=0;i<MAX_AUDIO_STREAMS;i++)
		{
			if ( audioBS[i] && !audioBS[i]->end_bs() ) 
			{
				finished = false;
				if ( next_cycle[i] != delay[i] &&  next_cycle[i] - SCR > 0.2  ) continue;

				//打包
				audioPack.reset();
				int data_length = 0;
				if( (data_length = audioPack.packData(*audioBS[i])) > 0){
					bytes_count[i] += data_length;
					audioPack.setStreamID(i+AUDIO_STREAM_0);
					audioPack.setPTS(next_cycle[i]);
					audioPack.setSCR(SCR);
					last_pack_length = audioPack.length();
					audioPack.encode(*outMPEG);	

					SCR += (long double )last_pack_length / mux_rate;
					int currPos = audioBS[i]->getCurrPos();
					while ( audioBS[i]->frameIndex[ next_frame_index[i] ] < currPos ) {
						next_frame_index[i] ++;
						if ( next_frame_index[i] > audioBS[i]->frameCount ) {
							break;
						}
					}
					next_cycle[i] = (long double)next_frame_index[i]/audioBS[i]->sampling_frequency*1152 + delay[i];

					needPadding = false;
				};
			}
		}
		if ( videoBS ) 
		{
			static bool got = false;
			if ( !got ) {
				videoPack.reset();
				const int MAX_SEQ_HEADER_POS = 1800;
				bool has_seq_header = false;
				int data_length = 2048;  //in fact data length: MPEG1-2029,MPEG2-2025
				//确保序列头在pack的前面
				int dist_to_seq_header = videoBS->seqIndex[next_group_index]-videoBS->getCurrPos();
				if ( dist_to_seq_header <= MAX_SEQ_HEADER_POS) {
					has_seq_header = true;
					if ( next_group_index < videoBS->seqCount ) {
						next_group_index ++;        
					}
				}else if (  dist_to_seq_header > MAX_SEQ_HEADER_POS && dist_to_seq_header < 2025 ) {
					data_length = MAX_SEQ_HEADER_POS;
				}
				if ( (data_length = videoPack.packData(*videoBS,data_length)) > 0 ) {
					got = true;
					videoPack.setStreamID(VIDEO_STREAM_0);
					int currPos = videoBS->getCurrPos();
					if ( has_seq_header ) {
						videoPack.getPacket(0)->setPTS(videoBS->seqTime[next_group_index-1]);
						videoPack.getPacket(0)->setDTS(videoBS->seqTime[next_group_index-1]+video_PTS_DTS);
						double curr_rate = (videoBS->seqIndex[next_group_index]-videoBS->seqIndex[next_group_index-1])  
							/ ( (videoBS->seqTime[next_group_index]-videoBS->seqTime[next_group_index-1]) );
						mux_rate = (audio_bit_rate_sum/8.0 + curr_rate ) * 1.05;
						if ( mux_rate < 0 ) {
							mux_rate = rate_bound * 50.0;
						}
					}
				}
			}
			if ( got ){
				finished = false;

				double PTS_value = videoPack.getPTS();
				if ( PTS_value - SCR < 1.0 ){
					videoPack.setSCR(SCR);
					videoPack.encode(*outMPEG);	
					SCR += (long double )videoPack.length() / mux_rate;
					if ( PTS_value > 0) {
						video_group_count ++;
					}
					got = false;
					needPadding = false;
				}
			}
		}
		//add padding
		if( !finished && needPadding ){
//			padding.setSCR(SCR);
//			padding.encode(*outMPEG);	
//			padding_count ++;
			SCR += (long double )PACK_SIZE / mux_rate ;
		}
	}

//	fprintf(stdout,"add %d padding\n",padding_count );
	
	outMPEG->setbits(MPEG_PROGRAM_END_CODE,32);

	fprintf(stdout,"done.\n");

	return true;
}
