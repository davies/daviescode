//*************************************************************************
//2003年智能体大赛参赛程序——决不贪吃
//by Davies , 2003-3-20
//*************************************************************************

#ifndef SPACE_H
#define SPACE_H

#include "playerstruct.h"
#include "analyze.h"
#include "search.h"

#define MARK 999			//活动空间的标记
#define DEADEARA 10			//死去的标记

//在一张地图同时标记双方的最短路径
void markDouMin(int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps);
//标记到达地图某一点的最远距离
int markMax(int id,int** pMap,MAP_INFO *info,PLAYER_STRUCT* ps);
//计算一块连通区域的大小
int getSpace(int **pMap,int mark,PLAYER_STRUCT *ps);

#endif
