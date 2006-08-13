//*************************************************************************
//2003年智能体大赛参赛程序——决不贪吃
//by Davies , 2003-3-20
//*************************************************************************

//*************************************************************************
//动态链接库的外部接口
//*************************************************************************

#include <windows.h>
#include "playerstruct.h"
#include "search.h"
#include <fstream.h>
#include <time.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT PLAYER_STRUCT* CALLBACK player_create(MAPFILESTRUCT* mfs, INITINFO* ii);
EXPORT int CALLBACK player_judge(PLAYER_STRUCT* ps, PLAYER_INFO* pi);
EXPORT void CALLBACK player_destroy(PLAYER_STRUCT* ps);

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

	ps->info = new MAP_INFO;
	ps->pMap = new int*[Height];
	for(int i=0;i<Height;i++)
		ps->pMap[i] = new int[Width];//Col

	for(i=0;i<3;i++) //假定蛇的数目等于2
	{
		ps->nodes[i] = new POSITION[Height*Width];
		ps->tmpMap[i] = new int*[Height];
		for(int j=0;j<Height;j++)
			ps->tmpMap[i][j] = new int[Width];//Col
	}

	//根据地图调整参数：最大长度和最大支路数
	//最大支路数：地图所能搜索的最多支路数，在满足当前局面的博弈支路树的数目小于最大支路树的情况下，尽量深的搜索
	if( Height <= 28 ){
		ps->MaxPaths = 250;
	}else if( Height <= 31 ){
		ps->MaxPaths = 200;
	}else {
		ps->MaxPaths = 150;
	}
	//最大长度：当蛇的长度达到这个值时就不再贪吃
	if( Height == 28 )
		ps->MaxLength = 300;
	else 
		ps->MaxLength = 20;

	return ps;
}

int CALLBACK player_judge(PLAYER_STRUCT* ps, PLAYER_INFO* pi)
{
	//粗略求解当前形势下合适的搜索深度
	ps->depth = getDepth(ps,pi);
	//用博弈的方式搜索最佳策略
	MapStatus result = Search(0,ps->ii->ID,ps,pi);
	return result.move[ps->ii->ID]-1;
}


void CALLBACK player_destroy(PLAYER_STRUCT* ps)
{

	//释放你自己分配的内存和资源
	int Height = ps->mfs->mfh.Height;
	delete ps->info;
	for(int j=0;j<Height;j++)
		delete ps->pMap[j];
	delete ps->pMap;

	for(int i=0;i<3;i++)
	{
		delete ps->nodes[i];
		for(int j=0;j<Height;j++)
			delete ps->tmpMap[i][j];
		delete ps->tmpMap[i];
	}
	free(ps);
}
