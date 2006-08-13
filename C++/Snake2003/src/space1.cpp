//*************************************************************************
//2003������������������򡪡�����̰��
//by Davies , 2003-3-20
//*************************************************************************

//*************************************************************************
//�ٶ�˫���������·�������Լ���˫����������Χ
//*************************************************************************

#include "space.h"

//���˫�������·���ܹ����Ƶ�����
//���ұ�ǵ���ĳһ������·��
void markDouMin(int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	int **minMap = ps->tmpMap[0];
	int **markMap = ps->tmpMap[1]; //��ǿ��Ե���ĵ�
	for(int i=0;i<ps->Height;i++)
	{
		memcpy(minMap[i],pMap[i],sizeof(int)*ps->Width);
		memset(markMap[i],0,sizeof(int)*ps->Width); //id=0
	}

	POSITION **nodes = (POSITION **)ps->nodes;
	int *count = ps->count;
	int snakeCount = ps->ii->SnakeCount;
	int firstNode[2];
	int lastNode[2];
	
	//��ʼ��
	for(i=0;i<snakeCount;i++)
	{
		firstNode[i] = 0;
		lastNode[i] = 1;
		count[i] = 1;
		nodes[i][0] = info->SnakeArr[i].Pos[info->SnakeArr[i].HeadPos];
		minMap[nodes[i][0].Row][nodes[i][0].Col] = 1; //��ͷ�����Ϊ1����ذ�������
		markMap[nodes[i][0].Row][nodes[i][0].Col] = -2+i;
	}

	//��������Ϊ�����߸���һ����
	//��������ͷ��λ��������չ��ֱ����������
	bool done = false;
	while( !done )
	{
		done = true;
		for(int id=0;id<snakeCount;id++)
		{
			for ( ;firstNode[id]<lastNode[id];firstNode[id]++)
			{
				done = false;
				int Row = nodes[id][firstNode[id]].Row;		//old
				int Col = nodes[id][firstNode[id]].Col;
				for(int i=0;i<4;i++)
				{
					if(firstNode[id]==0 && info->SnakeArr[id].Length == 2 
						&& (i+4-info->SnakeArr[id].Direction)%4 == 2 ) 
						continue;  //��ֹ����Ϊ2���ߺ���
					int row = Row + Direct[i][0];		//new
					int col = Col + Direct[i][1];
					if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //�ж�Խ��
					//������ӳ�û����������������������ߵ���֮ǰ���Ѿ���ʧ������Ե���
					if( minMap[row][col]<=0 && minMap[row][col] + minMap[Row][Col] >= -ps->depth )	//
					{
						nodes[id][count[id]].Row = row;
						nodes[id][count[id]].Col = col;
						count[id] ++;
						minMap[row][col] = minMap[Row][Col]+1; //�����̾���
						markMap[row][col] = -2+id; //��ǿ�������
					}
				}
			}
			lastNode[id] = count[id];
		}
	}
}