#include "search.h"
#include "analyze.h"
#include "getFruit.h"

void InitMap( int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	int Height = ps->Height;
	int Width = ps->Width;

	//Copy Snake info
	info->CurTime = pi->CurTime;
	info->Fruit = pi->Fruit;
	for(int i=0;i<ps->ii->SnakeCount;i++)
	{
		info->ResultInfo[i] = pi->ResultInfo[i];	//copy ResultInfo
		info->SnakeArr[i].Direction = pi->SnakeArr[i]->Direction;	
		int length = pi->SnakeArr[i]->Length;
		info->SnakeArr[i].Length = length;
		info->SnakeArr[i].HeadPos = length-1;
		for(int j=0;j<length;j++)
			info->SnakeArr[i].Pos[length-j-1] = pi->SnakeArr[i]->Pos[j];
	}
	//读取纪录
	if( ps->load )
		loadMap(ps,info);

	//put marks of floor and wall
	for(i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
			if( ps->mfs->pMap[i][j] >=0 )
				pMap[i][j] = FLOOR;
			else
				pMap[i][j] = WALL;

	//加入蛇的位置
	int count = ps->ii->SnakeCount;
	for(i=0;i<count;i++)
	{
		AISNAKE* snake = &info->SnakeArr[i];
		int length = snake->Length;
		for(int j=0;j<length;j++)//刚开始时，蛇的尾部在0位置
		{
			int Row = snake->Pos[j].Row; //蛇的每一段身体的行
			int Col = snake->Pos[j].Col; //列
			pMap[Row][Col] = -1-j;	//数值表示蛇的身体还能保持多长时间
		}
	}
}


//搜索当前的行动方式，并评价
MapStatus Search (int depth,int ID,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	MAP_INFO *info = ps->info[0];
	int **pMap	=	ps->pMap[0];
	int Height = ps->Height;
	int Width = ps->Width;
	//writeMap(pMap,ps);
	if(depth==0)		//copy player info
		InitMap(pMap,info,ps,pi);
#ifdef LOG
	if( depth == 0)
	{
		Log("\n",ps);
		Log(info->CurTime,ps);
	}
#endif

	AISNAKE * mySnake = &info->SnakeArr[ID] ;

	//初始化我的最初选择,选择作最坏的估计,相当于直接撞墙
	MapStatus best;
	memset(&best,0,sizeof(MapStatus));
	best.status[ID].dead = true;
	best.score = -100;
	best.move[ID] = -1;
	int myRow = mySnake->Pos[mySnake->HeadPos].Row;
	int myCol = mySnake->Pos[mySnake->HeadPos].Col;
	int fruitRow = info->Fruit.Pos.Row;
	int fruitCol = info->Fruit.Pos.Col;
	for(int i=0;i<3;i++)
	{
	//writeMap(pMap,ps);
		//移动到新位置,如果是墙壁，或者是蛇的身体，或出界，则continue
		int row1 = myRow +Direct[(mySnake->Direction+i+3) % 4][0];
		int col1 = myCol +Direct[(mySnake->Direction+i+3) % 4][1];
		if( row1 <0 || row1 >= Width || col1<0 || col1>=Height 
			|| pMap[row1][col1] < -1-(int)(depth/2) )//当为-1时，追尾
		{
			MapStatus newVal;
			memset(&newVal,0,sizeof(MapStatus));
			newVal.move[ID] = i;
			newVal.status[ID].dead = true;
			newVal.status[1-ID].dead = false;
			newVal.status[ID].living = -1;
			newVal.status[ID].space = -1;
			AISNAKE *hisSnake = &info->SnakeArr[1-ID];
			int hisRow = hisSnake->Pos[hisSnake->HeadPos].Row;
			int hisCol = hisSnake->Pos[hisSnake->HeadPos].Col;
			if( info->Fruit.Value > 0 && abs(hisRow-fruitRow) + abs(hisCol-fruitCol) <2 ){
				newVal.score = -info->Fruit.Value;
			}else{
				newVal.score = 0;
			}
			Choose(ID,best,newVal,info,ps);
			//writeMap(pMap,ps);
			continue;
		}
		//假定走到新的状态
		mySnake->HeadPos ++;
		//writeMap(pMap,ps);
		mySnake->Pos[mySnake->HeadPos].Row = row1;
		mySnake->Pos[mySnake->HeadPos].Col = col1;

		int oldDir1 = mySnake->Direction;
		mySnake->Direction = (mySnake->Direction+i+3) % 4;

		int old1 = pMap[row1][col1];
		pMap[row1][col1] = -mySnake->Length-(int)(depth/2)-1;

		//看是否吃到果子
		int score = 0;
		//
		if( info->Fruit.Value > 0 && row1 == fruitRow && col1 == fruitCol) //eat fruit
		{
			int nail = mySnake->HeadPos - mySnake->Length+1; //尾巴的位置
			int start = -pMap[mySnake->Pos[nail].Row][mySnake->Pos[nail].Col]-depth/2-1-1;
			for (int k=start;k<mySnake->Length;k++) //update map data
				pMap[mySnake->Pos[nail+k].Row][mySnake->Pos[nail+k].Col] -=info->Fruit.Value;
			
			//move node of snake body
			for(int j=mySnake->HeadPos;j>=nail;j--)
				mySnake->Pos[j+info->Fruit.Value] = mySnake->Pos[j];

			mySnake->HeadPos += info->Fruit.Value;
			for(j=mySnake->HeadPos-mySnake->Length;j>mySnake->HeadPos-mySnake->Length-info->Fruit.Value;j--)
				mySnake->Pos[j] = mySnake->Pos[j+1];
			mySnake->Length += info->Fruit.Value;

			score = info->Fruit.Value -depth/2;
			info->Fruit.Value = 0;	//no fruits
		}

		int oldValue = info->Fruit.Value;
		if( depth % 2 == 1){
			info->CurTime ++;
			//更新果子状态
			if(info->Fruit.Value >0 )	// fruit exist
				info->Fruit.ExistTime ++;
			if(info->Fruit.ExistTime > 50)
				info->Fruit.Value = 0; //果子消失
		}
		
		MapStatus newVal;
		memset(&newVal,0,sizeof(MapStatus));
		newVal.move[ID] = i;
		if(depth == ps->depth * 2-1)
		{
			//做两种情况的分析，选取最好的
			//先假定不吃果子
			int fruitRow = info->Fruit.Pos.Row;
			int fruitCol = info->Fruit.Pos.Col;
			//假定没有果子,只能在果子消失后才能通过
			if (info->Fruit.Value >0)
				pMap[fruitRow][fruitCol] = info->Fruit.ExistTime - MAXEXISTTIME-2 -ps->depth;
			getLiving(newVal,pMap,info,ps,pi);
			//恢复
			if (info->Fruit.Value >0)
				pMap[fruitRow][fruitCol] = FLOOR;

			int who = ps->tmpMap[1][fruitRow][fruitCol]+2;
			//假定一方用最快的方式吃了果子
			if( who < 2 && newVal.status[who].canEatFruit ){
				//假定以最快速度吃果子				
				AISNAKE *snake = &info->SnakeArr[who];
				//writeMap(pMap,ps);
				int value = info->Fruit.Value;
				int dist = newVal.status[who].distToFruit;
				int last = snake->HeadPos+1; //身体增长的最后一个
				for(int i= snake->HeadPos;i>snake->HeadPos-snake->Length;i--)
				{
					int row = snake->Pos[i].Row;
					int col = snake->Pos[i].Col;
					if(pMap[row][col]+dist+ps->depth < 0){
						pMap[row][col]-= value;
						last = i;
					}else 
						break ;
				}
				//writeMap(pMap,ps);
				snake->Length += value;
				snake->HeadPos +=value;

				MapStatus newVal2;
				memset(&newVal2,0,sizeof(MapStatus));
				getLiving(newVal2,pMap,info,ps,pi);
				Choose(who,newVal,newVal2,info,ps);

				snake->HeadPos -=value;
				snake->Length -= value;
				//消除吃果子的影响
				for( i= snake->HeadPos;i>=last;i--)
				{
					int row = snake->Pos[i].Row;
					int col = snake->Pos[i].Col;
					pMap[row][col] += value;
				}
			}
		}else{
			//writeMap(pMap,ps);
			newVal = Search(depth+1,1-ID,ps,pi);
			//writeMap(pMap,ps);
		//恢复当前回合中的行动策略
		}
		newVal.move[ID] = i;
		//writeMap(pMap,ps);
		newVal.status[ID].living ++;
		newVal.status[ID].space ++;
		newVal.score =-newVal.score + score;
#ifdef LOG
		//Log("hisBest",ps);
		if( depth == 0)
			Log(newVal,ps);
		//Log("",ps);
#endif
		//选取己方在当前形势的最好评价
		Choose(ID,best,newVal,info,ps);

		if( depth % 2 == 1){ //对方的回合
			//恢复果子状态
			info->CurTime --;
			if ( info->Fruit.ExistTime == 51 )
				info->Fruit.Value = oldValue;
			if (info->Fruit.Value > 0)
				info->Fruit.ExistTime --;
		}
		//看是否吃了果子
		if( score > 0 ) //eat fruit
		{
			info->Fruit.Value = score;	//no fruits
			int value = score;
			mySnake->Length -= value;
			mySnake->HeadPos -= value;
			
			//move node of mySnake body
			int nail = mySnake->HeadPos - mySnake->Length+1; //尾巴的位置
			for(int j=nail;j<=mySnake->HeadPos;j++)
				mySnake->Pos[j] = mySnake->Pos[j+value];
			//恢复地图到没有吃果子的情形
			for (int k=mySnake->Length-1;k>=0;k--) //update map data
			{
				if(pMap[mySnake->Pos[nail+k].Row][mySnake->Pos[nail+k].Col] == -k-depth/2-2-value ) 
					pMap[mySnake->Pos[nail+k].Row][mySnake->Pos[nail+k].Col] +=value;
				else 
					break;
			}
			//writeMap(pMap,ps);
		}
		//恢复我的蛇原来的状态
		mySnake->HeadPos --;
		mySnake->Direction = oldDir1;
		pMap[row1][col1] = old1;
	}
#ifdef LOG
	if( depth == 0 ){
		//writeMap(ps->pMap[depth],ps);
		Log("Best",ps);
		Log(best,ps);
		Log("",ps);
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

	return best;
}