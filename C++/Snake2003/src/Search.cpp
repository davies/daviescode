#include "search.h"
#include "analyze.h"
#include "getFruit.h"

void InitMap( int** pMap,PLAYER_STRUCT *ps,PLAYER_INFO *pi );
int execute (int depth,int myDir,int hisDir,PLAYER_STRUCT *ps,PLAYER_INFO *pi);

void trySearch (int depth,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	MAP_INFO *info = ps->info[depth];
	int **pMap	=	ps->pMap[depth];
	int Height = ps->mfs->mfh.Height;
	int Width = ps->mfs->mfh.Width;
	
	if(depth==0)		//copy player info
	{
		info->CurTime = pi->CurTime;
		info->Fruit = pi->Fruit;
		for(int i=0;i<ps->ii->SnakeCount;i++)
		{
			info->SnakeArr[i].Direction = pi->SnakeArr[i]->Direction;	//Copy Snake info
			int length = pi->SnakeArr[i]->Length;
			info->SnakeArr[i].Length = length;
			info->SnakeArr[i].HeadPos = length-1;
			for(int j=0;j<length;j++)
				info->SnakeArr[i].Pos[length-j-1] = pi->SnakeArr[i]->Pos[j];
		}
		InitMap(pMap,ps,pi);
	}

	AISNAKE * mySnake = &info->SnakeArr[ps->ii->ID] ;
	AISNAKE * hisSnake;
	int ID = ps->ii->ID;//我的id ,1-ID为对方的ID
	if(ps->ii->ID ==0) 
		hisSnake = &info->SnakeArr[1];
	else
		hisSnake = &info->SnakeArr[0];
	for(int i=0;i<3;i++)
	{
		//移动到新位置,如果是墙壁，或者是蛇的身体，或出界，则continue
		int row1 = mySnake->Pos[mySnake->HeadPos].Row +Direct[(mySnake->Direction+i+3) % 4][0];
		int col1 = mySnake->Pos[mySnake->HeadPos].Col +Direct[(mySnake->Direction+i+3) % 4][1];
		if( row1 <0 || row1 >= Width || col1<0 || col1>=Height 
			|| pMap[row1][col1] < -1 ) //当为-1时，追尾
		{
			continue;
		}
		for(int j=0;j<3;j++) 		//对方行动的三个方向
		{
			int row2 = hisSnake->Pos[hisSnake->HeadPos].Row +Direct[(hisSnake->Direction+j+3) % 4][0];
			int col2 = hisSnake->Pos[hisSnake->HeadPos].Col +Direct[(hisSnake->Direction+j+3) % 4][1];
			if( row2 <0 || row2 >= Width || col2<0 || col2>=Height 
				|| pMap[row2][col2] < -1 )//当为-1时，追尾
			{
				continue;
			}
			execute(depth+1,i,j,ps,pi);	//按照当前的行动更新地图信息
			if(depth == ps->depth-1)
				ps->paths ++;
			else
				trySearch(depth+1,ps,pi);
		}
	}
}

//获取在当前情况下适合搜索的深度
int getDepth( PLAYER_STRUCT *ps,PLAYER_INFO *pi )
{
	int oldPaths ;
	for(int i=3;i<MAXDEPTH;i++)
	{
		ps->depth = i;
		ps->paths = 0;
		trySearch(0,ps,pi);
		if( ps->paths*(pi->SnakeArr[ps->ii->ID]->Length/100+1) > MaxPath ){
			ps->paths = oldPaths;
			return i-1 ;
		}else{
			oldPaths = ps->paths;
		}
	}
	return MAXDEPTH-1;
}

void InitMap( int** pMap,PLAYER_STRUCT *ps,PLAYER_INFO *pi )
{
	int Height = ps->Height;
	int Width = ps->Width;

	//put marks of floor and wall
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
			if( ps->mfs->pMap[i][j] >=0 )
				pMap[i][j] = FLOOR;
			else
				pMap[i][j] = WALL;

	//加入蛇的位置
	int count = ps->ii->SnakeCount;
	for(i=0;i<count;i++)
	{
		SNAKE* snake = pi->SnakeArr[i];
		int length = snake->Length;
		for(int j=length-1;j>=0;j--)
		{
			int Row = snake->Pos[j].Row; //蛇的每一段身体的行
			int Col = snake->Pos[j].Col; //列
			pMap[Row][Col] = j-length;	//数值表示蛇的身体还能保持多长时间
		}
	}
	//writeMap(pMap,ps);
}

int execute (int depth,int myDir,int hisDir,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	//copy player info
	MAP_INFO *info = ps->info[depth];
	memcpy(info,ps->info[depth-1],sizeof(MAP_INFO));
	info->CurTime ++;

	int** pMap = ps->pMap[depth];
	for(DWORD i=0;i<ps->Height;i++)
		memcpy(pMap[i],ps->pMap[depth-1][i],ps->Width*sizeof(int));

	int score=0;
	if(info->Fruit.Value >0 )	// fruit exist
		info->Fruit.ExistTime ++;
	if(info->Fruit.ExistTime > 50)
		info->Fruit.Value = 0; //果子消失
	for (i=0;i<ps->ii->SnakeCount;i++)
	{
		AISNAKE* snake = &info->SnakeArr[i];
		//update the body
		//move the head position of snake
		if(snake->HeadPos == MAXLENGTH-1) //move snake body
		{
			int steps = snake->HeadPos+1 -snake->Length;
			for(int j=0;j<=snake->HeadPos;j++)
				snake->Pos[j] = snake->Pos[j+steps];
			snake->HeadPos = snake->Length;
		}else
			snake->HeadPos ++;
		int nail = snake->HeadPos - snake->Length; //尾巴的位置
		int start = -pMap[snake->Pos[nail].Row][snake->Pos[nail].Col]-1;
		for(int j=start;j<snake->Length;j++)	//身体的值加一 //bug!!!!!!!!!!!
		{
			pMap[snake->Pos[nail+j].Row][snake->Pos[nail+j].Col] ++;
		}
		//update the head
		//new position of head
		if(i==ps->ii->ID)
		{
			snake->Direction = (snake->Direction+myDir+3)%4;
			snake->Pos[snake->HeadPos].Row = snake->Pos[snake->HeadPos-1].Row + Direct[ snake->Direction][0];
			snake->Pos[snake->HeadPos].Col = snake->Pos[snake->HeadPos-1].Col + Direct[ snake->Direction][1];
		}else{
			snake->Direction = (snake->Direction+hisDir+3)%4;
			snake->Pos[snake->HeadPos].Row = snake->Pos[snake->HeadPos-1].Row + Direct[ snake->Direction ][0];
			snake->Pos[snake->HeadPos].Col = snake->Pos[snake->HeadPos-1].Col + Direct[ snake->Direction ][1];
		}
		pMap[snake->Pos[snake->HeadPos].Row]
			[snake->Pos[snake->HeadPos].Col] = - snake->Length ; //head
		
		//check if eat fruit
		if( info->Fruit.Value > 0 &&		//fruit exist
			snake->Pos[snake->HeadPos].Row == info->Fruit.Pos.Row && 
			snake->Pos[snake->HeadPos].Col == info->Fruit.Pos.Col) //eat fruit
		{
			int nail = snake->HeadPos - snake->Length+1; //尾巴的位置
			int start = -pMap[snake->Pos[nail].Row][snake->Pos[nail].Col]-1;
			for (int k=start;k<snake->Length;k++) //update map data
			{
				pMap[snake->Pos[nail+k].Row][snake->Pos[nail+k].Col] -=info->Fruit.Value;
			}
			//writeMap(pMap,ps);
			//move node of snake body
			for(int j=snake->HeadPos;j>=0;j--)
			{
				snake->Pos[j+info->Fruit.Value] = snake->Pos[j];
			}
			snake->HeadPos += info->Fruit.Value;
			for(j=snake->HeadPos-snake->Length;j>snake->HeadPos-snake->Length-info->Fruit.Value;j--)
				snake->Pos[j] = snake->Pos[j+1];
			snake->Length += info->Fruit.Value;
			score = info->Fruit.Value;
			if(i != ps->ii->ID) score = - score;
			info->Fruit.Value = 0;	//no fruits
		}
	}
	return score;
}

//我方的策略选择
void Choose(int ID,MapStatus &best,MapStatus &newVal,PLAYER_STRUCT *ps)
{
	PlayerStatus * best1 = &best.status[ID];
	PlayerStatus * best2 = &best.status[1-ID];
	PlayerStatus * newVal1 = &newVal.status[ID];
	PlayerStatus * newVal2 = &newVal.status[1-ID];
	int score ,newScore;
	if(ID == ps->ii->ID ) 
	{
		score = best.score;
		newScore = newVal.score;
	}else{
		score = -best.score;
		newScore = -newVal.score;
	}

	//先看生死情况
	if( best1->dead != newVal1->dead ){
		if( !best1->dead ) return ;
	}else if(best2->dead != newVal2->dead ){
		if( best2->dead ) return;
	}else if( best1->dead ) { //我必死
		if( best2->dead || newVal2->dead ) { //对方也死
			if( best2->dead ) return ;
		}else{//对方不死
			if( score != newScore ){
				if( score > newScore ) return ;
			}else if( best1->canEatFruit != newVal1->canEatFruit ){
				if( best1->canEatFruit ) return ;
			}else if( best1->living != newVal1->living ){
				if(best1->living > newVal1->living ) return ;
			}
		}
	}else if( best2->dead ) {
		//对方死
		if( score != newScore ){
			if(score > newScore) return ;
		}else if(best2->living != newVal2->living ){
			if( best2->living < newVal2->living ) return ;
		}else if( best1->canEatFruit != newVal1->canEatFruit ){
			if(best1->canEatFruit ) return ;
		}else if(best2->living != newVal2->living ){
			if( best2->living < newVal2->living ) return ;
		}else if( best1->distToFruit != newVal1->distToFruit ){
			if( best1->distToFruit < newVal1->distToFruit )	return ;
		}
	}//最后看是否安全
	else if( best1->safe != newVal1->safe ){
		if( best1->safe ) return ;
	}else if( best2->safe != newVal2->safe ){
		if( !best2->safe ) return ;
	}else if( !best2->safe ){
		if( best2->living < newVal2->living ) return ;
	}/*else if( !best1->safe ){
		if( best1->living > newVal1->living ) return ;
	}*/
	//最后看吃果子情况
	else {
		if( score != newScore ){
			 if(score > newScore )return ;
		}else if( best1->canEatFruit != newVal1->canEatFruit ){
			if( best1->canEatFruit ) return ;
		}else/* if(best1->canEatFruit ){
			if( best1->distToFruit != newVal1->distToFruit ) {
				if( best1->distToFruit < newVal1->distToFruit) return ;
			}else if(best2->living!=newVal2->living ){ //压缩对方的控制区域
				if(best2->living < newVal2->living ) return ;
			}else if( best1->living-best2->living != newVal1->living - newVal2->living){
				if(best1->living-best2->living > newVal1->living - newVal2->living) return ;
			}
		}else */{
			 if(best2->living!=newVal2->living ){ //压缩对方的控制区域
				if(best2->living < newVal2->living ) return ;
			}else if( best1->living!= newVal1->living){
				if(best1->living> newVal1->living) return ;
			}
		}
	
	}

	//newVal 比 best 更好，交换 best 和 newVal
	memcpy(&best,&newVal,sizeof(MapStatus));
}

//搜索当前的行动方式，并评价
MapStatus Search (int depth,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	MAP_INFO *info = ps->info[depth];
	int **pMap	=	ps->pMap[depth];
	int Height = ps->mfs->mfh.Height;
	int Width = ps->mfs->mfh.Width;
	
	if(depth==0)		//copy player info
	{
		info->CurTime = pi->CurTime;
		info->Fruit = pi->Fruit;
		for(int i=0;i<ps->ii->SnakeCount;i++)
		{
			//info->ResultInfo[i] = pi->ResultInfo[i];	//copy ResultInfo
			info->SnakeArr[i].Direction = pi->SnakeArr[i]->Direction;	//Copy Snake info
			int length = pi->SnakeArr[i]->Length;
			info->SnakeArr[i].Length = length;
			info->SnakeArr[i].HeadPos = length-1;
			for(int j=0;j<length;j++)
				info->SnakeArr[i].Pos[length-j-1] = pi->SnakeArr[i]->Pos[j];
		}
		InitMap(pMap,ps,pi);
	}

	AISNAKE * mySnake = &info->SnakeArr[ps->ii->ID] ;
	AISNAKE * hisSnake;
	int ID = ps->ii->ID;//我的id ,1-ID为对方的ID
	if(ps->ii->ID ==0) 
		hisSnake = &info->SnakeArr[1];
	else
		hisSnake = &info->SnakeArr[0];
#ifdef LOG
	if( depth == 0)
	{
		Log("\n",ps);
		Log(info->CurTime,ps);
	}
#endif

	//初始化我的最初选择,选择作最坏的估计,相当于直接撞墙
	MapStatus myBest;
	memset(&myBest,0,sizeof(MapStatus));
	myBest.status[ID].dead = true;
	//myBest.status[1-ID].dead = false;
	myBest.score = -50;
	myBest.move[ID] = -1;
	int myRow = mySnake->Pos[mySnake->HeadPos].Row;
	int myCol = mySnake->Pos[mySnake->HeadPos].Col;
	int fruitRow = info->Fruit.Pos.Row;
	int fruitCol = info->Fruit.Pos.Col;
	for(int i=0;i<3;i++)
	{
		//移动到新位置,如果是墙壁，或者是蛇的身体，或出界，则continue
		//int row = ps->ii->ID
		int row1 = myRow +Direct[(mySnake->Direction+i+3) % 4][0];
		int col1 = myCol +Direct[(mySnake->Direction+i+3) % 4][1];
		int hisRow = hisSnake->Pos[hisSnake->HeadPos].Row;
		int hisCol = hisSnake->Pos[hisSnake->HeadPos].Col;
		if( row1 <0 || row1 >= Width || col1<0 || col1>=Height 
			|| pMap[row1][col1] < -1 )//当为-1时，追尾
		{
			if( abs(hisRow-fruitRow) + abs(hisCol-fruitRow) == 2 && myBest.score == -50  && info->Fruit.Value >0) //如果它能吃到果子,并且我没有更好的应对策略，则直接撞墙死
			{
				myBest.score = 0;
				myBest.move[ID] = i;
			}
			continue;
		}
		//初始化最初选择,选择作最坏的估计,相当于直接撞墙，并且对方吃到最大果子
		MapStatus hisBest;
		memset(&hisBest,0,sizeof(MapStatus));
		hisBest.status[1-ID].dead = true;
		//hisBest.status[ID].dead = false;
		hisBest.score = 50;
		hisBest.move[ID] = i;

		for(int j=0;j<3;j++) 		//对方行动的三个方向
		{
			int row2 =  hisRow+Direct[(hisSnake->Direction+j+3) % 4][0];
			int col2 =  hisCol+Direct[(hisSnake->Direction+j+3) % 4][1];
			if( row2 <0 || row2 >= Width || col2<0 || col2>=Height 
				|| pMap[row2][col2] < -1 )//当为-1时，追尾
			{
				if( hisBest.score == 50  && fruitRow == row1 && fruitCol == col1 )
					hisBest.score = info->Fruit.Value ;
				continue;
			}
			int score=0;
			MapStatus newVal;
			memset(&newVal,0,sizeof(MapStatus));
			newVal.move[ID] = i;
			newVal.move[1-ID] = j;

			//对方移动到新位置,如果是墙壁，或者是蛇的身体，或出界，则死

			if(row1 == row2 && col1==col2 )//看双方是否头碰头
			{
				//两条蛇头碰头撞死
				newVal.status[ID].dead = true;
				newVal.status[ID].living = -1;
				newVal.status[ID].canEatFruit = false;
				newVal.status[1-ID].dead = true;
				newVal.status[1-ID].living = -1;
				newVal.status[1-ID].canEatFruit = false;
			}else{
				score = execute(depth+1,i,j,ps,pi);	//按照当前的行动更新地图信息
				if(depth == ps->depth-1)
				{
					//假定一方用最快的方式吃了果子
					if(ps->info[depth+1]->Fruit.Value >0 ) //如果果子还没有被吃
						eatFruit(newVal,ps->pMap[depth+1],ps->info[depth+1],ps);
					getLiving(newVal,ps->pMap[depth+1],ps->info[depth+1],ps,pi);
					newVal.score = 0;
				}else{
					newVal = Search(depth+1,ps,pi);
					//恢复当前回合中的行动策略
					newVal.move[ID] = i;
					newVal.move[1-ID] = j;
					newVal.status[0].living ++;
					newVal.status[1].living ++;
				}
				newVal.score +=score;
			}
#ifdef LOG
			//Log(newVal,ps);
#endif
			//对方选择对我最有害方式
			Choose(1-ID,hisBest,newVal,ps);
		}
#ifdef LOG
		//Log("hisBest",ps);
		if( depth == 0)
			Log(hisBest,ps);
		//Log("",ps);
#endif
		//选取己方在当前形势的最好评价
		Choose(ID,myBest,hisBest,ps);
	}
#ifdef LOG
	if( depth == 0 ){
		Log("Best",ps);
		Log(myBest,ps);
	}
	if( depth == 0 && pi->CurTime == ps->ii->TotalTime ){
		FILE * fp = ps->logFile ;
		fprintf(fp,"\n\n\n战术统计:\n");
		for(int i=0;i<ps->ii->SnakeCount;i++){
			fprintf(fp,"ID%d:\n",i);
			fprintf(fp,"Dead:%d\n",pi->ResultInfo[i].TimesOfDie);
			fprintf(fp,"Eat:%d\n",pi->ResultInfo[i].FruitsEatten);
			fprintf(fp,"MaxLength:%d\n",pi->ResultInfo[i].MaxLength);
			fprintf(fp,"MaxScore:%d\n",pi->ResultInfo[i].MaxScore);
			fprintf(fp,"Score:%d\n",pi->ResultInfo[i].Score);
		}
	}
#endif
	return myBest;
}