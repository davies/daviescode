// VConvertTest.cpp : Defines the entry point for the console application.
//
#include "windows.h"
#include "conio.h"
#include "io.h"
#pragma comment(lib,"videoconvert.lib");

extern "C" __declspec(dllimport)  int video_process(HANDLE in,HANDLE out,int horizontal,int vertical,int bitrate);

int main(int argc, _TCHAR* argv[])
{
	FILE *in = NULL,*out=NULL;
	in = fopen("c:\\test.m2v","rb");
	out = fopen("c:\\out.m2v", "wb+");

	HANDLE hFileIn = (HANDLE)_get_osfhandle(in->_file);
	HANDLE hFileOut = (HANDLE)_get_osfhandle(out->_file);
	printf("begin...");
	int nRet = video_process(hFileIn, hFileOut, 320, 240, 2000000);
	fclose(in);
	fclose(out);
	printf("done\nPress any key to quit!");
	getch();
	
	return 0;
}

