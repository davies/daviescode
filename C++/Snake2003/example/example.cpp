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

	//分配内存和资源

	return ps;
}

int CALLBACK player_judge(PLAYER_STRUCT* ps, PLAYER_INFO* pi)
{
	int direction = 0;
	//在这里加入你自己的判断代码，决定走的方向direction
	//...

	return direction;
}


void CALLBACK player_destroy(PLAYER_STRUCT* ps)
{
	//释放你自己分配的内存和资源
	free(ps);
}
