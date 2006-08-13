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
	int ID = ps->ii->ID;//�ҵ�id ,1-IDΪ�Է���ID
	if(ps->ii->ID ==0) 
		hisSnake = &info->SnakeArr[1];
	else
		hisSnake = &info->SnakeArr[0];
	for(int i=0;i<3;i++)
	{
		//�ƶ�����λ��,�����ǽ�ڣ��������ߵ����壬����磬��continue
		int row1 = mySnake->Pos[mySnake->HeadPos].Row +Direct[(mySnake->Direction+i+3) % 4][0];
		int col1 = mySnake->Pos[mySnake->HeadPos].Col +Direct[(mySnake->Direction+i+3) % 4][1];
		if( row1 <0 || row1 >= Width || col1<0 || col1>=Height 
			|| pMap[row1][col1] < -1 ) //��Ϊ-1ʱ��׷β
		{
			continue;
		}
		for(int j=0;j<3;j++) 		//�Է��ж�����������
		{
			int row2 = hisSnake->Pos[hisSnake->HeadPos].Row +Direct[(hisSnake->Direction+j+3) % 4][0];
			int col2 = hisSnake->Pos[hisSnake->HeadPos].Col +Direct[(hisSnake->Direction+j+3) % 4][1];
			if( row2 <0 || row2 >= Width || col2<0 || col2>=Height 
				|| pMap[row2][col2] < -1 )//��Ϊ-1ʱ��׷β
			{
				continue;
			}
			execute(depth+1,i,j,ps,pi);	//���յ�ǰ���ж����µ�ͼ��Ϣ
			if(depth == ps->depth-1)
				ps->paths ++;
			else
				trySearch(depth+1,ps,pi);
		}
	}
}

//��ȡ�ڵ�ǰ������ʺ����������
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

	//�����ߵ�λ��
	int count = ps->ii->SnakeCount;
	for(i=0;i<count;i++)
	{
		SNAKE* snake = pi->SnakeArr[i];
		int length = snake->Length;
		for(int j=length-1;j>=0;j--)
		{
			int Row = snake->Pos[j].Row; //�ߵ�ÿһ���������
			int Col = snake->Pos[j].Col; //��
			pMap[Row][Col] = j-length;	//��ֵ��ʾ�ߵ����廹�ܱ��ֶ೤ʱ��
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
		info->Fruit.Value = 0; //������ʧ
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
		int nail = snake->HeadPos - snake->Length; //β�͵�λ��
		int start = -pMap[snake->Pos[nail].Row][snake->Pos[nail].Col]-1;
		for(int j=start;j<snake->Length;j++)	//�����ֵ��һ //bug!!!!!!!!!!!
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
			int nail = snake->HeadPos - snake->Length+1; //β�͵�λ��
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

//�ҷ��Ĳ���ѡ��
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

	//�ȿ��������
	if( best1->dead != newVal1->dead ){
		if( !best1->dead ) return ;
	}else if(best2->dead != newVal2->dead ){
		if( best2->dead ) return;
	}else if( best1->dead ) { //�ұ���
		if( best2->dead || newVal2->dead ) { //�Է�Ҳ��
			if( best2->dead ) return ;
		}else{//�Է�����
			if( score != newScore ){
				if( score > newScore ) return ;
			}else if( best1->canEatFruit != newVal1->canEatFruit ){
				if( best1->canEatFruit ) return ;
			}else if( best1->living != newVal1->living ){
				if(best1->living > newVal1->living ) return ;
			}
		}
	}else if( best2->dead ) {
		//�Է���
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
	}//����Ƿ�ȫ
	else if( best1->safe != newVal1->safe ){
		if( best1->safe ) return ;
	}else if( best2->safe != newVal2->safe ){
		if( !best2->safe ) return ;
	}else if( !best2->safe ){
		if( best2->living < newVal2->living ) return ;
	}/*else if( !best1->safe ){
		if( best1->living > newVal1->living ) return ;
	}*/
	//��󿴳Թ������
	else {
		if( score != newScore ){
			 if(score > newScore )return ;
		}else if( best1->canEatFruit != newVal1->canEatFruit ){
			if( best1->canEatFruit ) return ;
		}else/* if(best1->canEatFruit ){
			if( best1->distToFruit != newVal1->distToFruit ) {
				if( best1->distToFruit < newVal1->distToFruit) return ;
			}else if(best2->living!=newVal2->living ){ //ѹ���Է��Ŀ�������
				if(best2->living < newVal2->living ) return ;
			}else if( best1->living-best2->living != newVal1->living - newVal2->living){
				if(best1->living-best2->living > newVal1->living - newVal2->living) return ;
			}
		}else */{
			 if(best2->living!=newVal2->living ){ //ѹ���Է��Ŀ�������
				if(best2->living < newVal2->living ) return ;
			}else if( best1->living!= newVal1->living){
				if(best1->living> newVal1->living) return ;
			}
		}
	
	}

	//newVal �� best ���ã����� best �� newVal
	memcpy(&best,&newVal,sizeof(MapStatus));
}

//������ǰ���ж���ʽ��������
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
	int ID = ps->ii->ID;//�ҵ�id ,1-IDΪ�Է���ID
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

	//��ʼ���ҵ����ѡ��,ѡ������Ĺ���,�൱��ֱ��ײǽ
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
		//�ƶ�����λ��,�����ǽ�ڣ��������ߵ����壬����磬��continue
		//int row = ps->ii->ID
		int row1 = myRow +Direct[(mySnake->Direction+i+3) % 4][0];
		int col1 = myCol +Direct[(mySnake->Direction+i+3) % 4][1];
		int hisRow = hisSnake->Pos[hisSnake->HeadPos].Row;
		int hisCol = hisSnake->Pos[hisSnake->HeadPos].Col;
		if( row1 <0 || row1 >= Width || col1<0 || col1>=Height 
			|| pMap[row1][col1] < -1 )//��Ϊ-1ʱ��׷β
		{
			if( abs(hisRow-fruitRow) + abs(hisCol-fruitRow) == 2 && myBest.score == -50  && info->Fruit.Value >0) //������ܳԵ�����,������û�и��õ�Ӧ�Բ��ԣ���ֱ��ײǽ��
			{
				myBest.score = 0;
				myBest.move[ID] = i;
			}
			continue;
		}
		//��ʼ�����ѡ��,ѡ������Ĺ���,�൱��ֱ��ײǽ�����ҶԷ��Ե�������
		MapStatus hisBest;
		memset(&hisBest,0,sizeof(MapStatus));
		hisBest.status[1-ID].dead = true;
		//hisBest.status[ID].dead = false;
		hisBest.score = 50;
		hisBest.move[ID] = i;

		for(int j=0;j<3;j++) 		//�Է��ж�����������
		{
			int row2 =  hisRow+Direct[(hisSnake->Direction+j+3) % 4][0];
			int col2 =  hisCol+Direct[(hisSnake->Direction+j+3) % 4][1];
			if( row2 <0 || row2 >= Width || col2<0 || col2>=Height 
				|| pMap[row2][col2] < -1 )//��Ϊ-1ʱ��׷β
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

			//�Է��ƶ�����λ��,�����ǽ�ڣ��������ߵ����壬����磬����

			if(row1 == row2 && col1==col2 )//��˫���Ƿ�ͷ��ͷ
			{
				//������ͷ��ͷײ��
				newVal.status[ID].dead = true;
				newVal.status[ID].living = -1;
				newVal.status[ID].canEatFruit = false;
				newVal.status[1-ID].dead = true;
				newVal.status[1-ID].living = -1;
				newVal.status[1-ID].canEatFruit = false;
			}else{
				score = execute(depth+1,i,j,ps,pi);	//���յ�ǰ���ж����µ�ͼ��Ϣ
				if(depth == ps->depth-1)
				{
					//�ٶ�һ�������ķ�ʽ���˹���
					if(ps->info[depth+1]->Fruit.Value >0 ) //������ӻ�û�б���
						eatFruit(newVal,ps->pMap[depth+1],ps->info[depth+1],ps);
					getLiving(newVal,ps->pMap[depth+1],ps->info[depth+1],ps,pi);
					newVal.score = 0;
				}else{
					newVal = Search(depth+1,ps,pi);
					//�ָ���ǰ�غ��е��ж�����
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
			//�Է�ѡ��������к���ʽ
			Choose(1-ID,hisBest,newVal,ps);
		}
#ifdef LOG
		//Log("hisBest",ps);
		if( depth == 0)
			Log(hisBest,ps);
		//Log("",ps);
#endif
		//ѡȡ�����ڵ�ǰ���Ƶ��������
		Choose(ID,myBest,hisBest,ps);
	}
#ifdef LOG
	if( depth == 0 ){
		Log("Best",ps);
		Log(myBest,ps);
	}
	if( depth == 0 && pi->CurTime == ps->ii->TotalTime ){
		FILE * fp = ps->logFile ;
		fprintf(fp,"\n\n\nս��ͳ��:\n");
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