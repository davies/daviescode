#include "converter.h"

Converter::Converter(const char* inputFile, const char *outputFile)
:ac3File(inputFile),mpaFile(outputFile)
{
}

Converter::Converter(FILE* inputFile, FILE *outputFile)
:ac3File(inputFile),mpaFile(outputFile)
{
}

void Converter::init()
{
	ac3File.init();
	//根据AC3文件的格式设定mpa文件的格式
	mpaFile.init();
}


void Converter::convert()
{
	int i;
	bool init_flag = false;
	int count  = 0;
    while(true){
		for (i = 0; i < BLOCKS_PER_BUFFER; i++){
			while ( ! ac3File.block() ){
				if (ac3File.eof() )	return ;
			}
			memcpy(swap_buf[0]+i*NSAMPLES,ac3File.getSamples(0),NSAMPLES*sizeof(sample_t));
			memcpy(swap_buf[1]+i*NSAMPLES,ac3File.getSamples(1),NSAMPLES*sizeof(sample_t));
		}
		//编码
		if( !init_flag ){
			mpaFile.setSampleRate(ac3File.getSampleRate());
			init_flag = true;
		}
		int samples_per_frame;
		if ( mpaFile.getLayer() == 1 ) {
			samples_per_frame = 384;
			for( i = 0; i< FRAMES_PER_BUFFER*3; i++){
				memcpy(mpaFile.getBuffer(0),&swap_buf[0][i*samples_per_frame],samples_per_frame*sizeof(sample_t));
				memcpy(mpaFile.getBuffer(1),&swap_buf[1][i*samples_per_frame],samples_per_frame*sizeof(sample_t));
				mpaFile.encodeAFrame();
			}
		}else{
			samples_per_frame = 1152;
			for( i = 0; i< FRAMES_PER_BUFFER; i++){
				memcpy(mpaFile.getBuffer(0),&swap_buf[0][i*samples_per_frame],samples_per_frame*sizeof(sample_t));
				memcpy(mpaFile.getBuffer(1),&swap_buf[1][i*samples_per_frame],samples_per_frame*sizeof(sample_t));
				mpaFile.encodeAFrame();
			}
		}
		fprintf(stdout,"\r%d frames in AC3 are processed ... ", ++count*BLOCKS_PER_BUFFER);
    }
//	fprintf(stdout,"\n");
}

//////////////////////////////////////////////////////////////////////////

void convert(FILE* inAC3,FILE* outMPA){
	Converter converter(inAC3,outMPA);
	converter.init();
	converter.convert();
}