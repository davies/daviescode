#ifndef SEARCH_H
#define SEARCH_H

#include "playerstruct.h"
#include "analyze.h"

#define WALL  -120
#define FLOOR 0
#define SNAKEBODY -1
#define FRUITBODY 120 //¾àÀëÎª0
const int DEATH = -50;

#define MaxPath 100

const int Direct[4][2] = { {0,-1},{-1,0},{0,1},{1,0} };

int getDepth( PLAYER_STRUCT *ps,PLAYER_INFO *pi );
MapStatus Search (int depth,PLAYER_STRUCT *ps,PLAYER_INFO *pi);

#endif
