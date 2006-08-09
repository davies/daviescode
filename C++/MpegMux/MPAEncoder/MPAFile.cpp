 #include "mpafile.h"

MPAFileEncoder::MPAFileEncoder(const char *filename)
{
    close_f = true;
	
    if ((bs.pt = fopen(filename, "wb")) == NULL) {
		printf("Could not create \"%s\".\n", filename);
		exit(1);
    }
    alloc_buffer(&bs, BUFFER_SIZE);
    bs.buf_byte_idx = BUFFER_SIZE-1;
    bs.buf_bit_idx=8;
    bs.totbit=0;
    bs.mode = WRITE_MODE;
    bs.eob = FALSE;
    bs.eobs = FALSE;

	sb_sample = (SBS FAR *) mem_alloc(sizeof(SBS), "sb_sample");
    j_sample = (JSBS FAR *) mem_alloc(sizeof(JSBS), "j_sample");
    win_que = (IN FAR *) mem_alloc(sizeof(IN), "Win_que");
    subband = (SUB FAR *) mem_alloc(sizeof(SUB),"subband");
	win_buf = (sample_t **) mem_alloc(sizeof(sample_t*)*2, "win_buf");
}

MPAFileEncoder::MPAFileEncoder(FILE *file)
{
	close_f = FALSE;
	bs.pt = file;
    alloc_buffer(&bs, BUFFER_SIZE);
    bs.buf_byte_idx = BUFFER_SIZE-1;
    bs.buf_bit_idx=8;
    bs.totbit=0;
    bs.mode = WRITE_MODE;
    bs.eob = FALSE;
    bs.eobs = FALSE;

	sb_sample = (SBS FAR *) mem_alloc(sizeof(SBS), "sb_sample");
    j_sample = (JSBS FAR *) mem_alloc(sizeof(JSBS), "j_sample");
    win_que = (IN FAR *) mem_alloc(sizeof(IN), "Win_que");
    subband = (SUB FAR *) mem_alloc(sizeof(SUB),"subband");
	win_buf = (sample_t **) mem_alloc(sizeof(sample_t*)*2, "win_buf");
}

MPAFileEncoder::~MPAFileEncoder()
{
    if( close_f && bs.pt )
		fclose(bs.pt);

	mem_free((void**)&sb_sample);
	mem_free((void**)&j_sample);
	mem_free((void**)&win_que);
	mem_free((void**)&subband);
	mem_free((void**)&win_buf);
}

bool MPAFileEncoder::init(  )
{
    /* clear buffers */
    memset((char *) buffer, 0, sizeof(buffer));
    memset((char *) bit_alloc, 0, sizeof(bit_alloc));
    memset((char *) scalar, 0, sizeof(scalar));
    memset((char *) j_scale, 0, sizeof(j_scale));
    memset((char *) scfsi, 0, sizeof(scfsi));
    memset((char *) ltmin, 0, sizeof(ltmin));
    memset((char *) lgmin, 0, sizeof(lgmin));
    memset((char *) max_sc, 0, sizeof(max_sc));
    memset((char *) snr32, 0, sizeof(snr32));
    memset((char *) sam, 0, sizeof(sam));
	
    fr_ps.header = &info;
    fr_ps.alloc = NULL;

    info.version = MPEG_AUDIO_ID;   /* Default: MPEG-1 */
	info.sampling_frequency = 0;
    info.bitrate_index = 11;
    info.lay = 2;
    info.mode = MPG_MD_STEREO;
    info.mode_ext = 0;
    info.emphasis = 0; 
    info.copyright = 1; 
    info.original = 0; 
    info.error_protection = FALSE;

    num_samples = MAX_U_32_NUM;

    hdr_to_frps(&fr_ps);
    extra_slot = 0;
	frameNum = 0;
    sentBits = 0;

    stereo = fr_ps.stereo;
    error_protection = info.error_protection;

	bitsPerSlot = 8;  
	samplesPerFrame = 1152; 

	return true;
}

bool MPAFileEncoder::setSampleRate(int sample_rate)
{
	if( sample_rate == 44100 ){
		info.sampling_frequency = 0;
	}else if ( sample_rate == 48000 ) {
		info.sampling_frequency = 1;
	}else if ( sample_rate == 32000 ) {
		info.sampling_frequency = 2;
	}else{
		fprintf(stderr,"Not supported sampling frequency %d Hz .",sample_rate );
		return false;
	}
	
    hdr_to_frps(&fr_ps);
	return true;
}

void MPAFileEncoder::encodeAFrame()
{
	frameNum++;
	win_buf[0] = buffer[0];
	win_buf[1] = buffer[1];
 
	avg_slots_per_frame = ((double)samplesPerFrame /s_freq[info.version][info.sampling_frequency]) *
		((double)bitrate[info.version][info.lay-1][info.bitrate_index] /(double)bitsPerSlot);
    whole_SpF = (int) avg_slots_per_frame;
    frac_SpF  = avg_slots_per_frame - (double)whole_SpF;
    slot_lag  = -frac_SpF;
   
	info.padding = (frac_SpF !=0 );

	if (frac_SpF != 0) {
		if (slot_lag > (frac_SpF-1.0) ) {
			slot_lag -= frac_SpF;
			extra_slot = 0;
			info.padding = 0;
			/*  printf("No padding for this frame\n"); */
		}else {
			extra_slot = 1;
			info.padding = 1;
			slot_lag += (1-frac_SpF);
			/*  printf("Padding for this frame\n");    */
		}
	}
	adb = (whole_SpF+extra_slot) * bitsPerSlot;
	
	for (i=0;i<3;i++) for (j=0;j<SCALE_BLOCK;j++){
		for (k=0;k<stereo;k++) {
			window_subband(&win_buf[k], &(*win_que)[k][0], k);
			filter_subband(&(*win_que)[k][0], &(*sb_sample)[k][i][j][0]);
		}
	}
	
	II_scale_factor_calc(*sb_sample, scalar, stereo, fr_ps.sblimit);
	pick_scale(scalar, &fr_ps, max_sc);
	if(fr_ps.actual_mode == MPG_MD_JOINT_STEREO) {
		II_combine_LR(*sb_sample, *j_sample, fr_ps.sblimit);
		II_scale_factor_calc(j_sample, &j_scale, 1, fr_ps.sblimit);
	}       /* this way we calculate more mono than we need */
	/* but it is cheap */
	
	for (k=0;k<stereo;k++) {
		psycho_anal(&buffer[k][0],&sam[k][0], k, 
			info.lay, snr32,
			(FLOAT)s_freq[info.version][info.sampling_frequency]*1000);
		for (i=0;i<SBLIMIT;i++) ltmin[k][i] = (double) snr32[i];
	}
	
	II_transmission_pattern(scalar, scfsi, &fr_ps);
	II_main_bit_allocation(ltmin, scfsi, bit_alloc, &adb, &fr_ps);
	
	encode_info(&fr_ps, &bs);
	
	II_encode_bit_alloc(bit_alloc, &fr_ps, &bs);
	II_encode_scale(bit_alloc, scfsi, scalar, &fr_ps, &bs);
	II_subband_quantization(scalar, *sb_sample, j_scale,
		*j_sample, bit_alloc, *subband, &fr_ps);
	II_sample_encoding(*subband, bit_alloc, &fr_ps, &bs);
	for (i=0;i<adb;i++) 
		put1bit(&bs, 0);
	
	frameBits = sstell( &bs ) - sentBits;
	if ( frameBits % bitsPerSlot )   /* a program failure */
		fprintf( stderr, "Sent %ld bits = %ld slots plus %ld\n",
		frameBits, frameBits/bitsPerSlot,
		frameBits%bitsPerSlot );
	sentBits += frameBits;
}
