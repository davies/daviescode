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
	//��pMap����ռ�
	int Height = ps->mfs->mfh.Height;
	int Width = ps->mfs->mfh.Width;
	ps->Height = Height;
	ps->Width = Width;

	ps->info = new MAP_INFO;
	ps->pMap = new int*[Height];
	for(int i=0;i<Height;i++)
		ps->pMap[i] = new int[Width];//Col

	for(i=0;i<3;i++) //�ٶ��ߵ���Ŀ����2
	{
		ps->nodes[i] = new POSITION[Height*Width];
		ps->tmpMap[i] = new int*[Height];
		for(int j=0;j<Height;j++)
			ps->tmpMap[i][j] = new int[Width];//Col
	}

	if( Height <= 28 ){
		ps->MaxPaths = 250;
	}else if( Height <= 31 ){
		ps->MaxPaths = 200;
	}else {
		ps->MaxPaths = 150;
	}
	ps->MaxLength = 20;

	return ps;
}

int CALLBACK player_judge(PLAYER_STRUCT* ps, PLAYER_INFO* pi)
{
	//������������Լ����жϴ��룬�����ߵķ���direction

	ps->depth = getDepth(ps,pi);

	ps->depth = 1;
	MapStatus result = Search(0,ps->ii->ID,ps,pi);

	return result.move[ps->ii->ID]-1;
}


void CALLBACK player_destroy(PLAYER_STRUCT* ps)
{

	//�ͷ����Լ�������ڴ����Դ
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
