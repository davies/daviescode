#include "playerstruct.h"
#include "analyze.h"
#include "search.h"
#include "space.h"
//获取一条蛇的生存空间的大小
//求双方的生存空间，即寿命，并看是否会死
void getLiving(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	PlayerStatus *status = (PlayerStatus *)current.status;
	//writeMap(pMap,ps);
	//粗略计算各自的空间
	markDouMin(pMap,info,ps);
	//writeMap(ps->tmpMap[0],ps);
	//看是否需要再精确求解
	for(int id=0;id<ps->ii->SnakeCount;id++){
		if( ps->count[id] < 200 ) {
			status[id].living = markMax(id,pMap,info,ps); //得到living
		}
	}
	//writeMap(ps->tmpMap[0],ps);
	//writeMap(ps->tmpMap[1],ps);

	//计算各自的空间
	for( id=0;id<ps->ii->SnakeCount;id++){
		status[id].space = getSpace(ps->tmpMap[1],-2+id,ps);
	}

	//更新吃果子情况
	//看谁最先到达果子处
	int fruitRow = info->Fruit.Pos.Row;
	int fruitCol = info->Fruit.Pos.Col;
	int minDist = 51;
	int who = 2;
	for(int i=0;i<4;i++)
	{
		int row = fruitRow + Direct[i][0];
		int col = fruitCol + Direct[i][1];
		if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
		int dist = ps->tmpMap[0][row][col];
		if(  dist > 0 && dist < minDist){
			minDist = dist ;
			who = ps->tmpMap[1][row][col] +2;
		}
	}
	
	if( info->Fruit.Value > 0 && minDist + info->Fruit.ExistTime <= MAXEXISTTIME+1 ){
			status[who].canEatFruit = true;
			status[who].distToFruit = minDist;
			ps->tmpMap[1][fruitRow][fruitCol] = -2+who;
	}

	//比较生死
	for(id=0;id<ps->ii->SnakeCount;id++)
	{
		if( status[id].space > info->SnakeArr[id].Length*2 || status[id].space >= ps->ii->TotalTime - info->CurTime){
			status[id].safe = true;
			status[id].dead = false;
		}else if( status[id].space < info->SnakeArr[id].Length && status[id].living <=ps->ii->TotalTime-info->CurTime)
			status[id].dead = true;
		else 
			status[id].dead = false;
		//如果能头接着尾，则很安全
		AISNAKE *snake = &info->SnakeArr[id];
		int nail = snake->HeadPos - snake->Length+1;
		if( abs(snake->Pos[snake->HeadPos].Row -snake->Pos[nail].Row)+
			abs(snake->Pos[snake->HeadPos].Col -snake->Pos[nail].Col) == 1){
			status[id].safe = true;
		}
	}

	//比较看谁先死
	if( status[0].dead && status[1].dead)
	{
		if( status[0].living < status[1].living)
			status[1].dead = false;
		else if ( status[0].living > status[1].living )
			status[0].dead = false;
	}
}
