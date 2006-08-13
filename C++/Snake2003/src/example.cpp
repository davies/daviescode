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
	for(int i=0;i<ii->SnakeCount;i++)
	{
		ps->nodes[i] = new POSITION[Height*Width];
		ps->tmpMap[i] = new int*[Height];
		for(int j=0;j<Height;j++)
			ps->tmpMap[i][j] = new int[Width];//Col
	}
	return ps;
}

int CALLBACK player_judge(PLAYER_STRUCT* ps, PLAYER_INFO* pi)
{
	//在这里加入你自己的判断代码，决定走的方向direction
	ps->depth = getDepth(ps,pi);
	MapStatus result = Search(0,ps,pi);
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

	for(int i=0;i<ps->ii->SnakeCount;i++)
		delete ps->nodes[i];

	free(ps);
}
