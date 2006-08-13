#include "space.h"

int getSpace(int **pMap,int mark,PLAYER_STRUCT *ps)
{
	//去掉单向的死胡同,计算蛇的活动空间
	int **tmpMap = ps->tmpMap[2];
	for(int i=0;i<ps->Height;i++)
		memset(tmpMap[i],0,sizeof(int)*ps->Width);

	int count = 0;
	for(int Row=0;Row<ps->Height;Row++){
		for(int Col=0;Col<ps->Width;Col++)
		{
			if( pMap[Row][Col] != mark ) continue;
			count++;
			for(int k=0;k<4;k++)
			{
				int row = Row + Direct[k][0];
				int col = Col + Direct[k][1];
				if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
				if( pMap[row][col] == mark )tmpMap[row][col] ++;
			}
		}
	}
	for(Row=0;Row<ps->Height;Row++)
	{
		for(int Col=0;Col<ps->Width;Col++)
		{
			if( tmpMap[Row][Col] == 0 ){
				continue;
			}else if( tmpMap[Row][Col] == 1 ){ 
				//开始去掉死区
				bool done = false;
				int inRow = Row;
				int inCol = Col;
				while( !done )
				{
					done = true;
					tmpMap[inRow][inCol] = 0;
					count --;
					for(int k=0;k<4;k++)
					{
						int row = inRow + Direct[k][0];
						int col = inCol + Direct[k][1];
						if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
						if( tmpMap[row][col] > 0) tmpMap[row][col] --;
						if( tmpMap[row][col] == 1 ){
							done = false;
							inRow = row;
							inCol = col;
							break;
						}
					}
				}
			}
		}
	}
	return count;
}

