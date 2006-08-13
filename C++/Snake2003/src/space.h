#ifndef SPACE_H
#define SPACE_H

#include "playerstruct.h"
#include "analyze.h"
#include "search.h"

#define MARK 999
#define DEADEARA 10
#define CONNECTION 2

void markDouMin(int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps);
int markMax(int id,int** pMap,MAP_INFO *info,PLAYER_STRUCT* ps);
int getSpace(int **pMap,int mark,PLAYER_STRUCT *ps);


#endif
