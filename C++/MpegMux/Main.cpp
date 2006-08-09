  #include "muxer.h"

void Version()
{
	fprintf(stdout,"\nMpegMux version 1.44 by davies \n\n");
}

void Usage()
{
	fprintf(stdout,"\nUSAGE: MpegMux [jobs_file_name] [-v] [-a] \n");
	fprintf(stdout,"   v: output m2v file\n");
	fprintf(stdout,"   a: do not include the 3td audio channel\n");
	fprintf(stdout,"   example: MpegMux jobs.txt\n");
	fprintf(stdout,"   the jobs file must be text file.\n");
}

void readline(char* str,FILE *infile)
{
	str[0] = 0;
	while ( fscanf(infile,"%s",str) && strlen(str) ) {
		if ( str[0] != '/' && str[1] != '/') {
			return ;
		}
		fgets(str,_MAX_PATH,infile);
		str[0] = 0;
	}
	exit(0);
}

int main(int argc, char* argv[])
{
	Version();
	
	if ( argc <= 1 ) {
		Usage();
		return 0;
	}
	
	FILE *infile = fopen(argv[1],"r");
	if ( !infile ) {
		fprintf(stderr,"Can not open the file %s\n",argv[1]);
		return -1;
	}

	bool m2v_out = false;
	bool au3_inc = true;

	int p = 2;
	while ( argc > p ) {
		if ( argv[p][0] == '-' ) {
			if ( argv[p][1] == 'v' ) {
				m2v_out = true;
			}else if ( argv[p][1] == 'a' ) {
				au3_inc = false;
			}
		}
		p ++;
	}

	MpaFrame::init();

	int count = 0;
	while ( !feof(infile) ) {
		char buf[_MAX_PATH] = "";
		char inFileName[_MAX_PATH] = "";
		int horizontal = 0,vertical = 0;
		int bit_rate = 0;
		int leftChIndex = 1,rightChIndex = 2;
		float leftLevel = 1.0,rightLevel = 1.0;

		// get file name
		readline(buf,infile);
		sscanf(buf,"%s",inFileName);
		FILE *tryOpen=fopen(inFileName,"r");
		if ( !tryOpen ) {
			fprintf(stderr,"can not open the file %s\n",inFileName);
			break;
		}else{
			fclose(tryOpen);
		}

		//info for video
		readline(buf,infile);
		sscanf(buf,"%d",&horizontal);
		readline(buf,infile);
		sscanf(buf,"%d",&vertical);
		readline(buf,infile);
		sscanf(buf,"%d",&bit_rate);

		// “ÙπÏ1
		readline(buf,infile);
		sscanf(buf,"%d",&leftChIndex);
		readline(buf,infile);
		if ( !strcmp(strlwr(buf),"copy")  ){
			leftLevel = 0.0;
		}else if ( strcmp(strlwr(buf),"auto") ) {
			sscanf(buf,"%f",&leftLevel);
		}
		
		//“ÙπÏ2
		readline(buf,infile);
		sscanf(buf,"%d",&rightChIndex);
		readline(buf,infile);
		if ( !strcmp(strlwr(buf),"copy")  ){
			rightLevel = 0.0;
		}else if ( strcmp(strlwr(buf),"auto")  ) {
			sscanf(buf,"%f",&rightLevel);
		}

		fprintf(stdout,"========================================================\n");
		fprintf(stdout,"JOB %d:\nINPUT: %s\n",++count,inFileName);
		fprintf(stdout,"========================================================\n");

		Muxer muxer(inFileName,"",
			horizontal,vertical,bit_rate,
			leftChIndex,leftLevel,rightChIndex,rightLevel,
			m2v_out,au3_inc);

		muxer.process();

		fprintf(stdout,"\n");
	}

	return 0;
}