#include <windows.h>
#include "playerstruct.h"
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

	//�����ڴ����Դ

	return ps;
}

int CALLBACK player_judge(PLAYER_STRUCT* ps, PLAYER_INFO* pi)
{
	int direction = 0;
	//������������Լ����жϴ��룬�����ߵķ���direction
	//...

	return direction;
}


void CALLBACK player_destroy(PLAYER_STRUCT* ps)
{
	//�ͷ����Լ�������ڴ����Դ
	free(ps);
}
