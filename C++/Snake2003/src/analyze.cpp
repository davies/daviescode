#include "playerstruct.h"
#include "analyze.h"
#include "search.h"
#include "space.h"
//��ȡһ���ߵ�����ռ�Ĵ�С
//��˫��������ռ䣬�������������Ƿ����
void getLiving(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	PlayerStatus *status = (PlayerStatus *)current.status;
	//writeMap(pMap,ps);
	//���Լ�����ԵĿռ�
	markDouMin(pMap,info,ps);
	//writeMap(ps->tmpMap[0],ps);
	//���Ƿ���Ҫ�پ�ȷ���
	for(int id=0;id<ps->ii->SnakeCount;id++){
		if( ps->count[id] < 200 ) {
			status[id].living = markMax(id,pMap,info,ps); //�õ�living
		}
	}
	//writeMap(ps->tmpMap[0],ps);
	//writeMap(ps->tmpMap[1],ps);

	//������ԵĿռ�
	for( id=0;id<ps->ii->SnakeCount;id++){
		status[id].space = getSpace(ps->tmpMap[1],-2+id,ps);
	}

	//���³Թ������
	//��˭���ȵ�����Ӵ�
	int fruitRow = info->Fruit.Pos.Row;
	int fruitCol = info->Fruit.Pos.Col;
	int minDist = 51;
	int who = 2;
	for(int i=0;i<4;i++)
	{
		int row = fruitRow + Direct[i][0];
		int col = fruitCol + Direct[i][1];
		if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //�ж�Խ��
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

	//�Ƚ�����
	for(id=0;id<ps->ii->SnakeCount;id++)
	{
		if( status[id].space > info->SnakeArr[id].Length*2 || status[id].space >= ps->ii->TotalTime - info->CurTime){
			status[id].safe = true;
			status[id].dead = false;
		}else if( status[id].space < info->SnakeArr[id].Length && status[id].living <=ps->ii->TotalTime-info->CurTime)
			status[id].dead = true;
		else 
			status[id].dead = false;
		//�����ͷ����β����ܰ�ȫ
		AISNAKE *snake = &info->SnakeArr[id];
		int nail = snake->HeadPos - snake->Length+1;
		if( abs(snake->Pos[snake->HeadPos].Row -snake->Pos[nail].Row)+
			abs(snake->Pos[snake->HeadPos].Col -snake->Pos[nail].Col) == 1){
			status[id].safe = true;
		}
	}

	//�ȽϿ�˭����
	if( status[0].dead && status[1].dead)
	{
		if( status[0].living < status[1].living)
			status[1].dead = false;
		else if ( status[0].living > status[1].living )
			status[0].dead = false;
	}
}
