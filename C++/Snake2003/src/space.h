//*************************************************************************
//2003������������������򡪡�����̰��
//by Davies , 2003-3-20
//*************************************************************************

#ifndef SPACE_H
#define SPACE_H

#include "playerstruct.h"
#include "analyze.h"
#include "search.h"

#define MARK 999			//��ռ�ı��
#define DEADEARA 10			//��ȥ�ı��

//��һ�ŵ�ͼͬʱ���˫�������·��
void markDouMin(int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps);
//��ǵ����ͼĳһ�����Զ����
int markMax(int id,int** pMap,MAP_INFO *info,PLAYER_STRUCT* ps);
//����һ����ͨ����Ĵ�С
int getSpace(int **pMap,int mark,PLAYER_STRUCT *ps);

#endif
