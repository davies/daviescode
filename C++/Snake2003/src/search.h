//*************************************************************************
//2003年智能体大赛参赛程序——决不贪吃
//by Davies , 2003-3-20
//*************************************************************************

#ifndef SEARCH_H
#define SEARCH_H

#include "playerstruct.h"
#include "analyze.h"

#define WALL  -999										//地图中的墙壁，不可以到达的格子
#define FLOOR 0											//地图中的地板，可以行动的格子

const int Direct[4][2] = { {0,-1},{-1,0},{0,1},{1,0} }; //行动的方向

//初始化地图，标记出地图中的墙壁和地板，还给舍得身体做上标记
void InitMap( int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi);

//求解当前形势下合适的搜索深度
int getDepth( PLAYER_STRUCT *ps,PLAYER_INFO *pi );

//选择对ID更有的结果
void Choose(int ID,MapStatus &best,MapStatus &newVal,MAP_INFO *info,PLAYER_STRUCT *ps);

//用博弈数搜索最佳行动策略
MapStatus Search (int depth,int id,PLAYER_STRUCT *ps,PLAYER_INFO *pi);

#endif
