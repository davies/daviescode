#include "playerstruct.h"
#include "analyze.h"
#include "search.h"

//判断蛇ID还有多少活动空间
//根据ps->pMap中的信息
//******************************************
//先看双方肯定不会死的情况
//******************************************
bool AllAlive( MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	for(int i=0;i<ps->Height;i++)
	{
		memcpy(ps->tmpMap[0][i],pMap[i],sizeof(int)*ps->Width);
	}
	pMap = ps->tmpMap[0];

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
		pMap[nodes[i][0].Row][nodes[i][0].Col] = 1;
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
					if(firstNode[id]==0 && info->SnakeArr[id].Length == 2 && (i+4-info->SnakeArr[id].Direction)%4 == 2 ) continue;  //长度为２的蛇不能后退
					int row = Row + Direct[i][0];		//new
					int col = Col + Direct[i][1];
					if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
					if( pMap[row][col]==FLOOR || pMap[row][col]<0 && pMap[row][col] + pMap[Row][Col] >= 0 )	//
					{
						nodes[id][count[id]].Row = row;
						nodes[id][count[id]].Col = col;
						count[id] ++;
						pMap[row][col] = pMap[Row][Col]+1;
					}
				}
			}
			lastNode[id] = count[id];
		}
	}
	//去掉单向的死胡同,计算蛇的活动空间
	//****************************************************************
	//用是否追尾判断是否安全还有问题，因为可能在你追上尾后，别人截杀
	//******************************************************************
#define MAXID 300
	PlayerStatus *status = (PlayerStatus *)current.status;
	for (int id=0;id<snakeCount;id++)
	{
		bool touch = false;
		for(int i=1;i<count[id];i++)
			pMap[nodes[id][i].Row][nodes[id][i].Col] = id + MAXID ; //区域填充
		//是否可以走到任何一条蛇的身体,则不会死
		for( int newid = 0;newid<snakeCount;newid++)
		{
			AISNAKE *snake =  &info->SnakeArr[newid];
			for( int i=1;i<snake->Length;i++)
				if ( pMap[snake->Pos[snake->HeadPos-i].Row][snake->Pos[snake->HeadPos-i].Col] == id + MAXID)
				{
					touch = true;
					break;
				}
			if( touch ) break; 
		}

		int oldCount = count[id]-1;
		bool canEatFruit = false;
		if( pMap[info->Fruit.Pos.Row][info->Fruit.Pos.Col] == id+MAXID ) canEatFruit = true;

		//去掉死胡同
		for( i=count[id]-1;i>=0;i--)
		{
			int num = 0;
			int Row = nodes[id][i].Row;		//old
			int Col = nodes[id][i].Col;
			for(int j=0;j<4;j++)
			{
				int row = Row + Direct[j][0];		//new
				int col = Col + Direct[j][1];
				if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
				if( pMap[row][col] == MAXID + id) num++;
			}
			if( num < 2 ) {
				pMap[Row][Col] = 0;
				count[id] -- ;
			}
		}
		//不能单看保守的值就判死刑
		if( count[id] < info->SnakeArr[id].Length / 4 +1 ) {
			status[id].dead = true;
			status[id].living = oldCount+1;//防止出现在周围追，但不杀的情况
			status[id].safe = false;
			continue;
		}
		if( count[id] >= info->SnakeArr[id].Length )
			status[id].safe = true;
		else
			status[id].safe = false;
		status[id].living = count[id]; 
	}

	return (status[0].safe && status[1].safe );
}


//int getMax
int getSpace(PlayerStatus &mySta,int id,int** pMap,MAP_INFO *info,PLAYER_STRUCT* ps)
{
#define MaxSpace 500
	//backup map data
	for(int i=0;i<ps->Height;i++)
	{
		memcpy(ps->tmpMap[0][i],pMap[i],sizeof(int)*ps->Width);
	}
	pMap = ps->tmpMap[0];
	POSITION *nodes = ps->nodes[0];

	//恢复到没有吃果子的状态
	if( mySta.canEatFruit == true)
	{
		//消去以最快速度吃果子的假定的干扰
		AISNAKE *snake = &info->SnakeArr[id];
		int nail = snake->HeadPos - snake->Length +1 ;
		int existTime = info->Fruit.Value ;
		for(int i= mySta.distToFruit;i<snake->Length;i++)
			pMap[snake->Pos[nail+ i].Row][snake->Pos[nail+i].Col] += existTime;
		//果子所在的地方不能穿过
		pMap[info->Fruit.Pos.Row][info->Fruit.Pos.Col] = WALL;
	}

	//假定用最短路径到达的空间
	int count=1;
	nodes[0] = info->SnakeArr[id].Pos[info->SnakeArr[id].HeadPos];
	int Row = nodes[0].Row;
	int Col = nodes[0].Col;
	int old = pMap[Row][Col];
	pMap[Row][Col] = 1;
	for ( int firstNode=0;firstNode<count;firstNode++)
	{
		for(int i=0;i<4;i++)
		{
			int Row = nodes[firstNode].Row;		//old
			int Col = nodes[firstNode].Col;
			int row = Row + Direct[i][0];		//new
			int col = Col + Direct[i][1];
			if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
			if(firstNode==0 && old == -2 && pMap[row][col]==-1 ) //can't go back,防止长度为2的蛇倒退
				continue;
			if( pMap[row][col]==FLOOR || pMap[row][col]<0 && pMap[row][col] + pMap[Row][Col] >= 0)	//
			{
				nodes[count].Row = row;
				nodes[count].Col = col;
				count ++;
				if( pMap[row][col] < 0)
					return MaxSpace;
				pMap[row][col] = pMap[Row][Col]+1;
			}
		}
	}

	if(count<=2)  return count-1;
#define MARK 400
	//填充区域
	for(i=0;i<count;i++)
		pMap[nodes[i].Row][nodes[i].Col] = MARK; 
	
	//********************************************************
	//如果有果子在区域中，则要加高边界，
	//********************************************************

	POSITION *pNode = ps->nodes[1];
	pNode[0]  = info->SnakeArr[id].Pos[info->SnakeArr[id].HeadPos];  //第一点为蛇头的后一点，便于后面的统一分析
	pMap[pNode[0].Row][pNode[0].Col] = 1;  //为实际距离+2 //蛇头
	int first = 0;
	int last  = 1;
	count = 1;
	for( ;first < last;first++)
	{
		int inRow = pNode[first].Row;
		int inCol = pNode[first].Col;
		//周围有多少个可以走的
		int num = 0;
		int firstDir = 0;
		int lastDir = 0;
		bool oppsite = false;
		for( int j=0;j<4;j++)
		{
			int newRow = inRow + Direct[j][0];		//new
			int newCol = inCol + Direct[j][1];
			if( newRow <0 || newRow >=ps->Height || newCol <0 || newCol>=ps->Width ) continue;  //判断越界
			//看周围是否有可以突破的地方
			if( pMap[newRow][newCol]<0 && pMap[newRow][newCol]+pMap[inRow][inCol]>=0 ) 
			{
					return MaxSpace;  //如果重新计算后可以突破
			}
			if( pMap[newRow][newCol] == MARK ){
				num++;
				if( num == 2  && lastDir == j-2) oppsite = true;
				lastDir = j;
			}
		}
		//根据情况判断
		const int dir[10][2] = {{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
		bool continous = false;
		firstDir = 10;
		lastDir = 10;
		for( j=0;j<10;j++) //看前方5个中有没有连续的3个的
		{
			int newRow = inRow + dir[j][0];		//new
			int newCol = inCol + dir[j][1];
			if( newRow <0 || newRow >=ps->Height || newCol <0 || newCol>=ps->Width ) continue;  //判断越界
			if( pMap[newRow][newCol] == MARK ){
				if( lastDir == j-1 ) continous = true;
				if(	!continous ) firstDir = j;
				lastDir = j;
			}
		}
		if ( num <=1 || num==2&&oppsite || lastDir-firstDir < 2  ) { //没有连续三个的，直接
			for( int j=0;j<4;j++)
			{
				int newRow = inRow + Direct[j][0];		//new
				int newCol = inCol + Direct[j][1];
				if( newRow <0 || newRow >=ps->Height || newCol <0 || newCol>=ps->Width ) continue;  //判断越界
				if( pMap[newRow][newCol] == MARK ){ 
					pNode[last].Row = newRow;
					pNode[last].Col = newCol;
					last++;
					pMap[newRow][newCol] = pMap[inRow][inCol] +1;
				}
			}
			continue;
		}
		//寻找当前的活动区域
		int qFirst = last;
		int oldLast = last;
		//writeMap(pMap,ps);
		//以入口点为基点向外扩展
		for( j=firstDir/2;j<=lastDir/2;j++) //连续的区域为firstDir 到 lastDir
		{
			int row2 = inRow + Direct[j][0];		//new
			int col2 = inCol + Direct[j][1];
			if( row2 <0 || row2 >=ps->Height || col2 <0 || col2>=ps->Width ) continue;  //判断越界
			//只能加入连续得点
			if( pMap[row2][col2] == MARK) {
				pNode[last].Row = row2;
				pNode[last].Col = col2;
				pMap[row2][col2] = MARK + last;
				last++;
				//向外扩展
				for(int k=1;k<4;k=k+2) //k=1,3
				{
					//writeMap(pMap,ps);
					int row3 = inRow + Direct[(j+k)%4][0];		//new
					int col3 = inCol + Direct[(j+k)%4][1];
					if( row3 <0 || row3 >=ps->Height || col3 <0 || col3>=ps->Width ) continue;  //判断越界
					if( pMap[row3][col3] < MARK ) continue;
					
					int row4 = row2 + Direct[(j+k)%4][0];		//new
					int col4 = col2 + Direct[(j+k)%4][1];
					if( row4 <0 || row4 >=ps->Height || col4 <0 || col4>=ps->Width ) continue;  //判断越界
					if( pMap[row4][col4] < MARK ) continue;
					
					if( pMap[row3][col3] ==MARK )
					{
						pNode[last].Row = row3;
						pNode[last].Col = col3;
						pMap[row3][col3] = MARK+last;
						last++;
					}
					if(pMap[row4][col4] ==MARK ){
						pNode[last].Row = row4;
						pNode[last].Col = col4;
						pMap[row4][col4] = MARK+last;
						last++;
					}//if
				}//for
			}
		}
		//writeMap(pMap,ps);
		//开始搜索连通区域
		for(;qFirst<last;qFirst++)
		{
			int row1 = pNode[qFirst].Row;
			int col1 = pNode[qFirst].Col;
			for( int j=0;j<4;j++)
			{
				int row2 = row1 + Direct[j][0];		//new
				int col2 = col1 + Direct[j][1];
				if( row2 <0 || row2 >=ps->Height || col2 <0 || col2>=ps->Width ) continue;  //判断越界
				//new addd
				if( pMap[row2][col2] <= MARK || pMap[row2][col2] > pMap[row1][col1])
					continue;//只有当下一个点大于当前的标记值
				for(int k=1;k<4;k=k+2) //k=1,3
				{
					int row3 = row1 + Direct[(j+k)%4][0];		//new
					int col3 = col1 + Direct[(j+k)%4][1];
					if( row3 <0 || row3 >=ps->Height || col3 <0 || col3>=ps->Width ) continue;  //判断越界
					if( pMap[row3][col3] < MARK ) continue;
					int row4 = row2 + Direct[(j+k)%4][0];		//new
					int col4 = col2 + Direct[(j+k)%4][1];
					if( row4 <0 || row4 >=ps->Height || col4 <0 || col4>=ps->Width ) continue;  //判断越界
					if( pMap[row4][col4] < MARK ) continue;
					if( pMap[row3][col3] ==MARK )
					{
						pNode[last].Row = row3;
						pNode[last].Col = col3;
						pMap[row3][col3] = MARK+last;
						last++;
					}
					if(pMap[row4][col4] ==MARK ){
						pNode[last].Row = row4;
						pNode[last].Col = col4;
						pMap[row4][col4] = MARK+last;
						last++;
					}//if
				}//for
			}//for
		}//for
		//搜索连通区域结束
		//writeMap(pMap,ps);
		for(qFirst = oldLast;qFirst<last;qFirst++)
		{
			int num = last - oldLast;
			pMap[pNode[qFirst].Row][pNode[qFirst].Col] = pMap[inRow][inCol] + num ;
			if( (num + pNode[qFirst].Row - inRow +pNode[qFirst].Col-inCol)%2 == 1)
				pMap[pNode[qFirst].Row][pNode[qFirst].Col] --;
		}//for
	}//for
	int max = 0;
	for(i=0;i<last;i++)
	{
		int tmp = pMap[pNode[i].Row][pNode[i].Col];
		if( tmp > max) max = tmp;
	}
	return max-1;
}

//*******************************************************
//再看双方肯定会死的情况
//*******************************************************
bool WillDie(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps)
{

	//求双方的生存空间，即寿命，并看是否会死
	PlayerStatus *status = (PlayerStatus *)current.status;
	for(int id=0;id<ps->ii->SnakeCount;id++)
	{
		if( status[id].dead == true ) continue;
		if( status[id].safe == true )
		{
			status[id].dead = false;
			continue;
		}
		status[id].living = getSpace(status[id],id,pMap,info,ps);
		if( status[id].living != MaxSpace && status[id].living <= ps->ii->TotalTime-info->CurTime)
			status[id].dead = true;
		else
			status[id].dead = false;
		//跟新吃果子情况
		if( status[1-id].canEatFruit && status[1-id].distToFruit > status[id].living ){
			status[1-id].canEatFruit = false;
		}
	}
	//比较看谁先死
	if( status[0].dead && status[1].dead)
	{
		if( status[0].living  < status[1].living)
		{
			status[1].dead = false;
			status[1].safe = true;
		}else if ( status[0].living  > status[1].living )
		{
			status[0].dead = false;
			status[0].safe = true;
		}
	}
	return (status[0].dead || status[1].dead);
}


//获取一条蛇的生存空间的大小
void getLiving(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	PlayerStatus *status = (PlayerStatus *)current.status;
	if (AllAlive(current,pMap,info,ps))
	{
		for(int i=0;i<ps->ii->SnakeCount;i++)
			if(status[i].safe) status[i].dead = false;
		return;
	}
	WillDie(current,pMap,info,ps);
}
