#include "getFruit.h"

//假定蛇以最快的方式吃果子，跟新蛇的身体的存在时间
void eatFruit(MapStatus &mapSta,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	int** tmpMap = ps->tmpMap[0];		//backup map data
	for(int i=0;i<ps->Height;i++)
	{
		memcpy(tmpMap[i],pMap[i],sizeof(int)*ps->Width);
	}

	int snakeCount = ps->ii->SnakeCount;
	POSITION **nodes = ps->nodes;
	int firstNode[MAXSNAKECOUNT];
	int lastNode[MAXSNAKECOUNT];
	int count[MAXSNAKECOUNT];
	//初始化
	for(i=0;i<snakeCount;i++)
	{
		firstNode[i] = 0;
		lastNode[i] = 1;
		count[i] = 1;
		nodes[i][0] = info->SnakeArr[i].Pos[info->SnakeArr[i].HeadPos];
		tmpMap[nodes[i][0].Row][nodes[i][0].Col] = 1;
	}
	bool done = false;
	bool canEatFruit = true;
	bool haveEaten = false;
	int distToFruit = 0;
	int who = 0;
	while( !done && canEatFruit && !haveEaten )
	{
		done = true;
		for(int id=0;id<snakeCount;id++)
		{
			for ( ;firstNode[id]<lastNode[id];firstNode[id]++)
			{
				done = false;
				int Row = nodes[id][firstNode[id]].Row;		//old
				int Col = nodes[id][firstNode[id]].Col;
				//判断果子是否消失
				if( tmpMap[Row][Col] + info->Fruit.ExistTime > 51 )
				{
					canEatFruit = false;
					break;
				}
				for(int i=0;i<4;i++)
				{
					if(firstNode[id]==0 && info->SnakeArr[id].Length == 2 && (i+4-info->SnakeArr[id].Direction)%4 == 2 ) continue;  //长度为２的蛇不能后退
					int row = Row + Direct[i][0];		//new
					int col = Col + Direct[i][1];
					if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
					if( tmpMap[row][col]==FLOOR || tmpMap[row][col]<0 && tmpMap[row][col] + tmpMap[Row][Col] >= 0 )	//
					{
						if(row == info->Fruit.Pos.Row && col == info->Fruit.Pos.Col ){
							haveEaten = true;
							who = id;
							distToFruit = tmpMap[Row][Col];
							break;
						}
						nodes[id][count[id]].Row = row;
						nodes[id][count[id]].Col = col;
						count[id] ++;
						tmpMap[row][col] = tmpMap[Row][Col]+1;
					}
				}
			}
			lastNode[id] = count[id];
		}
	}

	if( haveEaten ){
		AISNAKE * snake= &info->SnakeArr[who];
		mapSta.status[who].canEatFruit = true;
		mapSta.status[who].distToFruit = distToFruit;
		mapSta.status[1-who].canEatFruit = false;
		mapSta.status[1-who].distToFruit = 51;
		int nail = snake->HeadPos - snake->Length +1 ;
		int existTime = info->Fruit.Value ;
		for(int i= distToFruit;i<snake->Length;i++)
			pMap[snake->Pos[nail+ i].Row][snake->Pos[nail+i].Col] -= existTime;
	}
}
