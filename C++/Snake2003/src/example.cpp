#include <windows.h>
#include "playerstruct.h"
#include "search.h"
#include <fstream.h>
#include <time.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT PLAYER_STRUCT* CALLBACK player_create(MAPFILESTRUCT* mfs, INITINFO* ii);
EXPORT int CALLBACK player_judge(PLAYER_STRUCT* ps, PLAYER_INFO* pi);
EXPORT void CALLBACK player_destroy(PLAYER_STRUCT* ps);

int getCommand(PLAYER_STRUCT* ps, PLAYER_INFO* pi);

PLAYER_STRUCT* CALLBACK player_create(MAPFILESTRUCT* mfs, INITINFO* ii)
{
	PLAYER_STRUCT*	ps;

	ps=(PLAYER_STRUCT*)malloc(sizeof(PLAYER_STRUCT));
	ps->ii=ii;
	ps->mfs=mfs;

	//init code
	//给pMap分配空间
	int Height = ps->mfs->mfh.Height;
	int Width = ps->mfs->mfh.Width;
	ps->Height = Height;
	ps->Width = Width;
	
	for(int d=0;d<=MAXDEPTH;d++)
	{
		ps->info[d] = new MAP_INFO;
		ps->pMap[d] = new int*[Height];
		for(int i=0;i<Height;i++)
			ps->pMap[d][i] = new int[Width];//Col
	}

	for(int i=0;i<3;i++) //假定蛇的数目等于2
	{
		ps->nodes[i] = new POSITION[Height*Width];
		ps->tmpMap[i] = new int*[Height];
		for(int j=0;j<Height;j++)
			ps->tmpMap[i][j] = new int[Width];//Col
	}

	ps->MaxPaths = 300;
	ps->MaxLength = 20;

	ps->logFile = fopen("log.txt","a");
	time_t currTime;
	time(&currTime);
	fprintf(ps->logFile,"\n\n\n%s",ctime(&currTime));

	return ps;
}

int CALLBACK player_judge(PLAYER_STRUCT* ps, PLAYER_INFO* pi)
{
	//在这里加入你自己的判断代码，决定走的方向direction
#ifdef LOG
	saveMap(ps,pi);
#endif
	ps->load = false;
	//ps->load = true;
	//int last  = clock();
	ps->depth = getDepth(ps,pi);

	//fprintf(ps->logFile,"\n%4d",pi->CurTime);
	//fprintf(ps->logFile,"\tDepth=%d,paths=%3d",ps->depth,ps->paths);
	//fprintf(ps->logFile,"\ttry=%3d",clock()-last);
	//ps->paths=0;
	//last = clock();

	//ps->depth = 2;
	MapStatus result = Search(0,ps->ii->ID,ps,pi);
	//fprintf(ps->logFile,"\tReal paths=%3d",ps->paths);
	//fprintf(ps->logFile,"\ttime=%3d",clock()-last);
	return result.move[ps->ii->ID]-1;
}


void CALLBACK player_destroy(PLAYER_STRUCT* ps)
{

	//释放你自己分配的内存和资源
	int Height = ps->mfs->mfh.Height;
	for(int d=0;d<=MAXDEPTH;d++)
	{
		delete (ps->info[d]);
		for(int i=0;i<Height;i++)
			delete (ps->pMap[d][i]);
		delete (ps->pMap[d]);
	}
	
	for(int i=0;i<3;i++)
	{
		delete ps->nodes[i];
		for(int j=0;j<Height;j++)
			delete ps->tmpMap[i][j];
		delete ps->tmpMap[i];
	}
	fclose(ps->logFile);
	free(ps);
}
