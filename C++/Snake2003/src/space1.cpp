#include "space.h"

void markDouMin(int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	int **minMap = ps->tmpMap[0];
	int **markMap = ps->tmpMap[1]; //标记可以到达的点
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
	
	//初始化
	for(i=0;i<snakeCount;i++)
	{
		firstNode[i] = 0;
		lastNode[i] = 1;
		count[i] = 1;
		nodes[i][0] = info->SnakeArr[i].Pos[info->SnakeArr[i].HeadPos];
		minMap[nodes[i][0].Row][nodes[i][0].Col] = 1;
		markMap[nodes[i][0].Row][nodes[i][0].Col] = -2+i;
	}

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
						continue;  //长度为２的蛇不能后退
					int row = Row + Direct[i][0];		//new
					int col = Col + Direct[i][1];
					if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
					if( minMap[row][col]<=0 && minMap[row][col] + minMap[Row][Col] >= -ps->depth )	//
					{
						nodes[id][count[id]].Row = row;
						nodes[id][count[id]].Col = col;
						count[id] ++;
						minMap[row][col] = minMap[Row][Col]+1; //标记最短距离
						markMap[row][col] = -2+id; //做标记
					}
				}
			}
			lastNode[id] = count[id];
		}
	}
}

