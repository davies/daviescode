//*************************************************************************
//2003������������������򡪡�����̰��
//by Davies , 2003-3-20
//*************************************************************************

#ifndef SEARCH_H
#define SEARCH_H

#include "playerstruct.h"
#include "analyze.h"

#define WALL  -999										//��ͼ�е�ǽ�ڣ������Ե���ĸ���
#define FLOOR 0											//��ͼ�еĵذ壬�����ж��ĸ���

const int Direct[4][2] = { {0,-1},{-1,0},{0,1},{1,0} }; //�ж��ķ���

//��ʼ����ͼ����ǳ���ͼ�е�ǽ�ں͵ذ壬��������������ϱ��
void InitMap( int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi);

//��⵱ǰ�����º��ʵ��������
int getDepth( PLAYER_STRUCT *ps,PLAYER_INFO *pi );

//ѡ���ID���еĽ��
void Choose(int ID,MapStatus &best,MapStatus &newVal,MAP_INFO *info,PLAYER_STRUCT *ps);

//�ò�������������ж�����
MapStatus Search (int depth,int id,PLAYER_STRUCT *ps,PLAYER_INFO *pi);

#endif
