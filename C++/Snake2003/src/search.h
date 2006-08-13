#ifndef SEARCH_H
#define SEARCH_H

#include "playerstruct.h"
#include "analyze.h"

#define WALL  -999
#define FLOOR 0

const int Direct[4][2] = { {0,-1},{-1,0},{0,1},{1,0} };

void InitMap( int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi);
int getDepth( PLAYER_STRUCT *ps,PLAYER_INFO *pi );
void Choose(int ID,MapStatus &best,MapStatus &newVal,MAP_INFO *info,PLAYER_STRUCT *ps);
MapStatus Search (int depth,int id,PLAYER_STRUCT *ps,PLAYER_INFO *pi);

#endif
