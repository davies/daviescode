//*************************************************************************
//2003年智能体大赛参赛程序——决不贪吃
//by Davies , 2003-3-20
//*************************************************************************

//*************************************************************************
//用博弈树进行搜索
//*************************************************************************

#include "search.h"
#include "analyze.h"

//初始化地图，在地图上分别标记地板、墙壁以及蛇的身体
void InitMap( int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	int Height = ps->Height;
	int Width = ps->Width;

	//Copy Snake info
	//将结构pi中的数据拷贝到自己定义的结构info中，便于更改数据
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

	//标记墙壁和地板
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
			int Row = snake->Pos[j].Row; 
			int Col = snake->Pos[j].Col; 
			pMap[Row][Col] = -1-j;//负表示是障碍物，它的绝对值表示蛇的身体的存在时间
		}
	}
}


//用博弈树搜索最佳行动方式
//depth/2 为当前所处的深度， ID为当前行动的蛇
//返回行动的方式以及评估结果
MapStatus Search (int depth,int ID,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	MAP_INFO *info = ps->info;
	int **pMap	=	ps->pMap;
	if(depth==0)		
		InitMap(pMap,info,ps,pi);

	AISNAKE * mySnake = &info->SnakeArr[ID] ;

	//初始化我的最初选择,作最坏的估计：直接撞墙并且对方吃到最大果子
	MapStatus best;
	memset(&best,0,sizeof(MapStatus));
	best.status[ID].dead = true;
	best.score = -100;
	best.move[ID] = -1;
	int myRow = mySnake->Pos[mySnake->HeadPos].Row; //蛇头的位置
	int myCol = mySnake->Pos[mySnake->HeadPos].Col;
	int fruitRow = info->Fruit.Pos.Row;
	int fruitCol = info->Fruit.Pos.Col;
	for(int i=0;i<3;i++)
	{
		//移动到新位置,如果是墙壁，或者是蛇的身体，或出界，则continue
		int row1 = myRow +Direct[(mySnake->Direction+i+3) % 4][0];
		int col1 = myCol +Direct[(mySnake->Direction+i+3) % 4][1];
		if( row1 <0 || row1 >= ps->Height || col1<0 || col1>=ps->Width  //出界
			|| pMap[row1][col1] < -1-(int)(depth/2) )//当为-1时，追尾
		{
			//如果是我方行动阶段，看对方是否能在下一步吃到果子
			//如果自己是必死的，则先撞死以不让对方吃到果子
			if( depth %2 == 0){ 
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
			}
			continue;
		}
		//假定走到新的状态
		mySnake->HeadPos ++;
		mySnake->Pos[mySnake->HeadPos].Row = row1;
		mySnake->Pos[mySnake->HeadPos].Col = col1;

		int oldDir1 = mySnake->Direction;
		mySnake->Direction = (mySnake->Direction+i+3) % 4;

		int old1 = pMap[row1][col1];
		pMap[row1][col1] = -mySnake->Length-(int)(depth/2)-1;

		//看是否吃到果子
		int score = 0;
		if( info->Fruit.Value > 0 && row1 == fruitRow && col1 == fruitCol) //eat fruit
		{
			//吃到了果子，则要跟新蛇的状态，长度增加，蛇的身体存在的时间增长
			int nail = mySnake->HeadPos - mySnake->Length+1; //尾巴的位置
			int start = -pMap[mySnake->Pos[nail].Row][mySnake->Pos[nail].Col]-depth/2-1-1;
			for (int k=start;k<mySnake->Length;k++) //update map data
				pMap[mySnake->Pos[nail+k].Row][mySnake->Pos[nail+k].Col] -=info->Fruit.Value; //增加蛇的身体的存在时间
			
			//移动蛇的身体的的节点
			for(int j=mySnake->HeadPos;j>=nail;j--)
				mySnake->Pos[j+info->Fruit.Value] = mySnake->Pos[j];

			mySnake->HeadPos += info->Fruit.Value;
			for(j=mySnake->HeadPos-mySnake->Length;j>mySnake->HeadPos-mySnake->Length-info->Fruit.Value;j--)
				mySnake->Pos[j] = mySnake->Pos[j+1]; //延长蛇的尾部
			mySnake->Length += info->Fruit.Value;

			score = info->Fruit.Value ;//吃了果子得分
			info->Fruit.Value = 0;	//果子被吃掉，地图上果子消失
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
			//做两种情况的分析，一种是吃果子，一种是不吃果子，选取最好的
			//先假定不吃果子
			int fruitRow = info->Fruit.Pos.Row;
			int fruitCol = info->Fruit.Pos.Col;
			//假定没有果子,只能在果子消失后才能通过
			if (info->Fruit.Value >0)
				pMap[fruitRow][fruitCol] = info->Fruit.ExistTime - MAXEXISTTIME-2 -ps->depth;
			getLiving(newVal,pMap,info,ps,pi);  //求解死活及生存空间
			//恢复
			if (info->Fruit.Value >0)
				pMap[fruitRow][fruitCol] = FLOOR;

			int who = ps->tmpMap[1][fruitRow][fruitCol]+2; //能吃到果子的蛇的id
			//假定一方用最快的方式吃了果子
			if( who < 2 && newVal.status[who].canEatFruit ){
				//假定以最快速度吃果子,跟新蛇的身体的存在时间，延长存在时间				
				AISNAKE *snake = &info->SnakeArr[who];
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
				snake->Length += value;
				
				MapStatus newVal2;
				memset(&newVal2,0,sizeof(MapStatus));
				getLiving(newVal2,pMap,info,ps,pi);
				Choose(who,newVal,newVal2,info,ps);

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
			//递归调用博弈树搜索
			newVal = Search(depth+1,1-ID,ps,pi);
		}
		//恢复当前回合中的行动策略
		newVal.move[ID] = i;
		newVal.status[ID].living ++;
		newVal.status[ID].space ++;
		newVal.score =-newVal.score + score;

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
			//已经吃了果子，恢复到吃果子前的状态
			info->Fruit.Value = score;	//no fruits
			int value = score;
			mySnake->Length -= value;
			mySnake->HeadPos -= value;
			
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
		}
		//恢复蛇原来的状态
		mySnake->HeadPos --;
		mySnake->Direction = oldDir1;
		pMap[row1][col1] = old1;
	}

	return best;
}