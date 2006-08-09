    #include "muxer.h"

Muxer::Muxer( string _inFileName,string _outFileName,
	 		int _horizontal,int _vertical,int _video_bit_rate,
			 int _leftChIndex,double _leftScale,
			 int _rightChIndex,double _rightScale,
			 bool _m2v_out, bool _au3_inc)
{
	//参数
	m2v_out = _m2v_out;
	au3_inc = _au3_inc;

	version = MPEG2;

	inFileName = _inFileName;
	inMPEG = new BitStream( inFileName );
	
	if( _outFileName.length() <= 0){
		outFileName = _inFileName.substr(0,_inFileName.find_last_of('.')) + ".new.mpg";
	}else
		outFileName = _outFileName;
	outMPEG = new BitStream( outFileName, true );

	horizontal = _horizontal ;
	vertical = _vertical ;
	video_new_bit_rate = _video_bit_rate;

	leftChIndex		= _leftChIndex-1;
	leftLevel		= _leftScale;
	rightChIndex	= _rightChIndex-1;
	rightLevel		= _rightScale;
	mixedAudioIndex = -1; //not used

	video_stream_id = 0;
	video_PSTD_buffer_size_bound = 0;
	video_bit_rate = 0;
	video_dPTS = 0.0;
	video_PTS_DTS = 0.0;
	videoBS = NULL;
	video_delay = 0.0;

	audio_stream_count = 0;
	for(int i=0;i<MAX_AUDIO_STREAMS;i++){
		delay[i] = 0.0;
		audioType[i] = NOT_A_AUDIOSTREAM;
		audioBS[i] = NULL;
	}

}

Muxer::~Muxer()
{
	if (inMPEG ) {
		delete inMPEG;
	}
	if (outMPEG ){
		delete outMPEG;
	}
	if ( videoBS) {
		delete videoBS;
	}
	for( int i=0;i<MAX_AUDIO_STREAMS;i++){
		if( audioBS[i] ){
			delete audioBS[i];
		}
	}
}

unsigned int Muxer::getVideoBitrate(BitStream *bs)
{
	if ( !bs->seek_sync(SEQUENCE_HEADER_CODE,32) ){
		return 0;
	}
	bs->getbits(32);
	int horizontal_size_value = bs->getbits(12);
	int vertical_size_value = bs->getbits(12);
	int aspect_ratio_information = bs->getbits(4);
	int frame_rate_code = bs->getbits(4);
	unsigned long bit_rate_value = bs->getbits(18);
	bs->get1bit();
	int vbv_buffer_size_value = bs->getbits(10);
	bool constrained_parameters_flag = bs->get1bit();
	bool load_intra_quantiser_matrix = bs->get1bit();
	if ( load_intra_quantiser_matrix ) {
		for(int i=0;i<64;i++){
			bs->getbits(8);
		}
	}
	bool load_non_intra_quantiser_matrix = bs->get1bit();
	if ( load_non_intra_quantiser_matrix ) {
		for(int i=0;i<64;i++){
			bs->getbits(8);
		}
	}
	
	//extension sequence header
	if( bs->nextbits(32) == EXTENSION_START_CODE ){
		bs->getbits(32);
		int extension_start_code_identifier = bs->getbits(4);
		int profile_and_level_indication = bs->getbits(8);
		bool progressivve_sequence = bs->get1bit();
		int chroma_format = bs->getbits(2);
		int horizontal_size_extension = bs->getbits(2);
		int vertical_size_extension = bs->getbits(2);
		unsigned long bit_rate_extension = bs->getbits(12);
		bit_rate_value |= bit_rate_extension << 18;
	}
	bit_rate_value *= 400;
	bs->reset(false);
	return bit_rate_value;
}

extern void convert(FILE* inAC3,FILE* outMPA);

int Muxer::getUnusedIndex()
{
	for(int i=0;i<MAX_AUDIO_STREAMS;i++){
		if( !audioBS[i] ){
			return  i;
		}
	}
	return -1;	
}

bool Muxer::convert()
{
	for(int i=0;i<MAX_AUDIO_STREAMS;i++){
		if( audioBS[i] && audioType[i] == AUDIOSTREAM_TYPE_AC3 ){
			fprintf(stdout,"converting AC3 audio %d to MP2 ....\n",i+1);

//			int newIndex = getUnusedIndex();
			MpaStream * newAudio = new MpaStream;

			::convert(audioBS[i]->getFILE(),newAudio->getFILE() );
			
			delete audioBS[i];
			audioBS[i] = newAudio;

			audioBS[i]->reset();
			audioBS[i]->updateInfo();
			audioType[i] = AUDIOSTREAM_TYPE_MPA;

			fprintf(stdout,"converting is done.\n");
		}
	}
	return true;
}

void Muxer::writeScale()
{
	//将音量系数写入文件
	string outFileName = inFileName.substr(0,inFileName.find_last_of('.')) + ".txt";
	FILE *fp = fopen(outFileName.c_str(),"w");
	for(int i=0;i<MAX_AUDIO_STREAMS;i++){
		if( audioBS[i] && audioType[i] == AUDIOSTREAM_TYPE_MPA ){
			fprintf(fp,"%d %4.2f %4.2f\n",i,
				1.0/getMaxVol(audioBS[i],0),
				1.0/getMaxVol(audioBS[i],1));
		}
	}
	fclose(fp);
}

bool Muxer::process()
{
	demux();
	
	if ( version == MPEG2 ) {
		convert();
	}

	normalize();

	if ( au3_inc &&  version == MPEG2 ) {
		muxMpa();
	}

	writeScale();

	if ( video_new_bit_rate >=0 && !doVideo() ) {
		return false;
	}

	mux();

	return true;
}
