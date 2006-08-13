#ifndef SEARCH_H
#define SEARCH_H

#include "playerstruct.h"
#include "analyze.h"
#include "log.h"

#define WALL  -999
#define FLOOR 0
#define SNAKEBODY -1
#define FRUITBODY 120 //¾àÀëÎª0
const int DEATH = -50;

//#define MaxPath 250

const int Direct[4][2] = { {0,-1},{-1,0},{0,1},{1,0} };

void InitMap( int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi);
int getDepth( PLAYER_STRUCT *ps,PLAYER_INFO *pi );
void Choose(int ID,MapStatus &best,MapStatus &newVal,MAP_INFO *info,PLAYER_STRUCT *ps);
MapStatus Search (int depth,int id,PLAYER_STRUCT *ps,PLAYER_INFO *pi);

#endif
