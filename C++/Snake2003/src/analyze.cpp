//*************************************************************************
//2003年智能体大赛参赛程序——决不贪吃
//by Davies , 2003-3-20
//*************************************************************************

//*************************************************************************
//分析当前地图的形势，看双方的死活、是否吃到果子等
//*************************************************************************

#include "playerstruct.h"
#include "analyze.h"
#include "search.h"
#include "space.h"

void getLiving(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	PlayerStatus *status = (PlayerStatus *)current.status;
	//粗略计算各自的空间
	markDouMin(pMap,info,ps);
	//看是否需要再精确求解
	for(int id=0;id<ps->ii->SnakeCount;id++){
		if( ps->count[id] < 200 ) {
			status[id].living = markMax(id,pMap,info,ps); //得到living
		}
	}

	//计算各自的空间
	for( id=0;id<ps->ii->SnakeCount;id++)
		status[id].space = getSpace(ps->tmpMap[1],-2+id,ps);

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
	
	//看能否在果子消失前或者比赛结束前吃到果子
	if( info->Fruit.Value > 0 && minDist + info->Fruit.ExistTime <= MAXEXISTTIME
		&& minDist <= ps->ii->TotalTime-info->CurTime ){
			status[who].canEatFruit = true;
			status[who].distToFruit = minDist;
			ps->tmpMap[1][fruitRow][fruitCol] = -2+who;
	}

	//在我身体较短的时候，我不吃使自己的身体的长度变成奇数的果子
	AISNAKE *mySnake= &info->SnakeArr[ps->ii->ID];
	if( who == ps->ii->ID && mySnake->Length < ps->MaxLength 
		&& (mySnake->Length+info->Fruit.Value)%2 == 1
		&& ps->ii->TotalTime-info->CurTime > 200 )
	{
		status[who].canEatFruit = false;
		status[who].distToFruit = 51;
	}

	//比较双方的生死
	for(id=0;id<ps->ii->SnakeCount;id++)
	{
		//如果所控制的空间小于身体长度，则会死，
		//如果大于两倍身体长度，则认为很安全，否则认为很危险
		if( status[id].space > info->SnakeArr[id].Length*2 || status[id].space >= ps->ii->TotalTime - info->CurTime){
			status[id].safe = true;
			status[id].dead = false;
		}else if( status[id].space < info->SnakeArr[id].Length && status[id].living <=ps->ii->TotalTime-info->CurTime)
			status[id].dead = true;
		else 
			status[id].dead = false;

		//如果能头接着尾，则很安全
		AISNAKE *snake = &info->SnakeArr[id];
		int headRow = snake->Pos[snake->HeadPos].Row;
		int headCol = snake->Pos[snake->HeadPos].Col;
		bool nearToNail = false; //是否在蛇的旁边存在一个尾巴
		for(int i=0;i<4;i++){
			int row = headRow +Direct[i][0];
			int col = headCol +Direct[i][1];
			if(row < 0 || row >= ps->Height || col < 0 || col >= ps->Width ) continue;
			if ( pMap[row][col] == -1-ps->depth ) {
				nearToNail = true;
				break;
			}
		}
		if( nearToNail && snake->Length > 2 ){
			status[id].safe = true;
		}
		if( snake->Length > 100 || snake->Length > 50 && snake->Length%2 == 0 ) status[id].safe = true;
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
