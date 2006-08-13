#include "space.h"

//从某一点开始标志最少步数
int markMin(int inRow,int inCol,int dist,int id,int **pMap,int** minMap,int **markMap,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	//准备工作
	POSITION *nodes = ps->nodes[2];
	int last=1;
	nodes[0].Row = inRow;
	nodes[0].Col = inCol;
	minMap[inRow][inCol] = dist;
	markMap[inRow][inCol] = -2+id;

	for ( int firstNode=0;firstNode<last;firstNode++)
	{
		int Row = nodes[firstNode].Row;		//old
		int Col = nodes[firstNode].Col;
		for(int i=0;i<4;i++)
		{
			if(firstNode==0 && info->SnakeArr[id].Length == 2 
				&& (i+4-info->SnakeArr[id].Direction)%4 == 2 ) 
				continue;  //长度为２的蛇不能后退
			int row = Row + Direct[i][0];		//new
			int col = Col + Direct[i][1];
			if( row <0 || row >=ps->Height || col <0 || col>=ps->Width ) continue;  //判断越界
			if(	(pMap[row][col]<=0 && pMap[row][col]+minMap[Row][Col] >= -ps->depth )
				&&(minMap[row][col]<=0 && minMap[row][col]+minMap[Row][Col] >= -ps->depth  //以前没有到达
				|| minMap[row][col] > minMap[Row][Col]+1) )//重新标记
			{
				nodes[last].Row = row;
				nodes[last].Col = col;
				last ++;
				minMap[row][col] = minMap[Row][Col]+1;
				if( markMap[row][col] == 0 || markMap[row][col] == -1-id ) 
					markMap[row][col] = -2+id;
			}
		}
	}

	return last;
}

//标记最最晚到达的步数
//前提是可以到达的区域已经标记好，标记为Mark+id
int markMax(int id,int** pMap,MAP_INFO *info,PLAYER_STRUCT* ps)
{
	int **minMap = ps->tmpMap[0];
	int **maxMap = ps->tmpMap[1];
	POSITION *pNode = ps->nodes[id]; //在getSpace1()中已经记录了可以到达的区域
	int *count = ps->count;

	AISNAKE *snake = &info->SnakeArr[id];
	pNode[0] = snake->Pos[snake->HeadPos];
	int first = 0;
	int last  = 1;
	maxMap[pNode[0].Row][pNode[0].Col] = 1;

	int mark = -2+id;
	for( ;first < last;first++)
	{
		int inRow = pNode[first].Row;
		int inCol = pNode[first].Col;

		//周围有多少个可以走的
		int num = 0;
		int lastDir = 0;
		bool oppsite = false;
		for( int j=0;j<4;j++)
		{
			int newRow = inRow + Direct[j][0];		//new
			int newCol = inCol + Direct[j][1];
			if( newRow <0 || newRow >=ps->Height || newCol <0 || newCol>=ps->Width ) continue;  //判断越界
			//看周围是否有可以突破的地方
			if( (maxMap[newRow][newCol] == 0 || maxMap[newRow][newCol]==-1-id ) //还没有被包括进来
				&& pMap[newRow][newCol]<0 && pMap[newRow][newCol]+maxMap[inRow][inCol]>= -ps->depth  )
			{
				//修改最短距离
				int dist = -pMap[newRow][newCol] - ps->depth + 1;
				POSITION head = info->SnakeArr[id].Pos[info->SnakeArr[id].HeadPos]; 
				if( (newRow+newCol-head.Row-head.Col)%2 == dist %2 ) dist ++; //奇偶性修正
				if( dist < minMap[inRow][inCol]+1 ) dist =minMap[inRow][inCol]+1;
		
				if( minMap[newRow][newCol] <= 0 || minMap[newRow][newCol]>dist ) 
				{
					int newCount = markMin(newRow,newCol,dist,id,pMap,minMap,maxMap,info,ps);
					if( newCount+last > snake->Length * 2 ){
						for(int i=0;i<last;i++)
							maxMap[pNode[i].Row][pNode[i].Col] = mark;
						return 0;
					}
					
				}
			}
			if( maxMap[newRow][newCol] == mark ){
				num++;
				if( num == 2  && lastDir == j-2) oppsite = true;
				lastDir = j;
			}
		}
		if( num == 0 ) continue;

		//根据情况判断
		const int dir[10][2] = {{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};

		unsigned char bits=0x00;
		unsigned char bit = 0x80; //第一位
		for( j=0;j<8;j++) //看前方5个中有没有连续的3个的
		{
			int newRow = inRow + dir[j][0];		//new
			int newCol = inCol + dir[j][1];
			if( newRow <0 || newRow >=ps->Height || newCol <0 || newCol>=ps->Width ){
				bit >>= 1;
				continue;  //判断越界
			}
			if( maxMap[newRow][newCol] == mark ){
				bits |= bit; //如果该方向的格子可以到达，则将该位置为1
			}
			bit >>= 1;
		}
		//调整次序将连续的排在最前面
		unsigned char max = bits;
		int maxDir = 0;
		for(j=1;j<8;j++)
		{
			//循环移位,左移
			unsigned char high = bits & 0x80;
			bits <<= 1;
			high >>= 7;
			bits |= high;
			if(bits > max){
				max = bits;
				maxDir = j;
			}
		}
		if ( num <=1 || num==2&&oppsite || max < 0xe0  ) { //没有连续三个的，直接
			for( int j=0;j<4;j++)
			{
				int newRow = inRow + Direct[j][0];		//new
				int newCol = inCol + Direct[j][1];
				if( newRow <0 || newRow >=ps->Height || newCol <0 || newCol>=ps->Width ) continue;  //判断越界
				if( maxMap[newRow][newCol] == mark ){ 
					pNode[last].Row = newRow;
					pNode[last].Col = newCol;
					last++;
					maxMap[newRow][newCol] = maxMap[inRow][inCol] +1;
				}
			}
			continue;
		}
		//加入非连续区域的点
		bit = 0x80;
		int firstZero = 7;
		for( j=0;j<8;j++){
			if( (max | bit) != max ){
				firstZero = j;
				break;
			}
			bit >>=1;
		}
		for( j=firstZero+1;j<8;j++)
		{
			if( (j+maxDir)%2 == 1 ) continue;
			int dir = (j+maxDir)/2%4;
			int newRow = inRow + Direct[dir][0];		//new
			int newCol = inCol + Direct[dir][1];
			if( newRow <0 || newRow >=ps->Height || newCol <0 || newCol>=ps->Width ) continue;  //判断越界
			if( maxMap[newRow][newCol] == mark ){ 
				pNode[last].Row = newRow;
				pNode[last].Col = newCol;
				last++;
				maxMap[newRow][newCol] = maxMap[inRow][inCol] +1;
			}
		}

		int oldLast = last;
		//寻找当前的活动区域
		//以入口点为基点向外扩展
		for( j=(maxDir+1)/2;j<=(firstZero-1+maxDir)/2;j++) //连续的区域为firstDir 到 lastDir
		{
			int row2 = inRow + Direct[j%4][0];		//new
			int col2 = inCol + Direct[j%4][1];
			if( row2 <0 || row2 >=ps->Height || col2 <0 || col2>=ps->Width ) continue;  //判断越界
			//只能加入连续得点
			if( maxMap[row2][col2] == mark) {
				pNode[last].Row = row2;
				pNode[last].Col = col2;
				maxMap[row2][col2] = MARK + last;
				last++;
				//向外扩展
				for(int k=1;k<4;k=k+2) //k=1,3
				{
					int row3 = inRow + Direct[(j+k)%4][0];		//new
					int col3 = inCol + Direct[(j+k)%4][1];
					if( row3 <0 || row3 >=ps->Height || col3 <0 || col3>=ps->Width ) continue;  //判断越界
					if( maxMap[row3][col3] == 0 || maxMap[row3][col3] == -1-id ) continue;
					
					int row4 = row2 + Direct[(j+k)%4][0];		//new
					int col4 = col2 + Direct[(j+k)%4][1];
					if( row4 <0 || row4 >=ps->Height || col4 <0 || col4>=ps->Width ) continue;  //判断越界
					if( maxMap[row4][col4] == 0 || maxMap[row4][col4] == -1-id ) continue;
					
					if( maxMap[row3][col3] ==mark )
					{
						pNode[last].Row = row3;
						pNode[last].Col = col3;
						maxMap[row3][col3] = MARK+last;
						last++;
					}
					if(maxMap[row4][col4] ==mark ){
						pNode[last].Row = row4;
						pNode[last].Col = col4;
						maxMap[row4][col4] = MARK+last;
						last++;
					}//if
				}//for
			}
		}
		//开始搜索连通区域
		for(int qFirst = oldLast;qFirst<last;qFirst++)
		{
			int row1 = pNode[qFirst].Row;
			int col1 = pNode[qFirst].Col;
			for( int j=0;j<4;j++)
			{
				int row2 = row1 + Direct[j][0];		//new
				int col2 = col1 + Direct[j][1];
				if( row2 <0 || row2 >=ps->Height || col2 <0 || col2>=ps->Width ) continue;  //判断越界
				//new addd
				if( maxMap[row2][col2] <= MARK || maxMap[row2][col2] > maxMap[row1][col1])
					continue;//只有当下一个点小于当前的标记值
				for(int k=1;k<4;k=k+2) //k=1,3
				{
					int row3 = row1 + Direct[(j+k)%4][0];		//new
					int col3 = col1 + Direct[(j+k)%4][1];
					if( row3 <0 || row3 >=ps->Height || col3 <0 || col3>=ps->Width ) continue;  //判断越界
					if( maxMap[row3][col3] == 0 || maxMap[row3][col3] == -1-id ) continue;

					int row4 = row2 + Direct[(j+k)%4][0];		//new
					int col4 = col2 + Direct[(j+k)%4][1];
					if( row4 <0 || row4 >=ps->Height || col4 <0 || col4>=ps->Width ) continue;  //判断越界
					if( maxMap[row4][col4] == 0 || maxMap[row4][col4] == -1-id ) continue;

					if( maxMap[row3][col3] ==mark )
					{
						pNode[last].Row = row3;
						pNode[last].Col = col3;
						maxMap[row3][col3] = MARK+last;
						last++;
					}
					if(maxMap[row4][col4] ==mark ){
						pNode[last].Row = row4;
						pNode[last].Col = col4;
						maxMap[row4][col4] = MARK+last;
						last++;
					}//if
				}//for
			}//for
		}//for
		//搜索连通区域结束
		//标记最慢到达时间
		for(qFirst = oldLast;qFirst<last;qFirst++)
		{
			int num = last - oldLast;
			maxMap[pNode[qFirst].Row][pNode[qFirst].Col] = maxMap[inRow][inCol] + num ;
			if( (num + pNode[qFirst].Row - inRow +pNode[qFirst].Col-inCol)%2 == 1)
				maxMap[pNode[qFirst].Row][pNode[qFirst].Col] --;
		}//for
	}//for

	//最大存活时间
	int max = maxMap[pNode[last-1].Row][pNode[last-1].Col];
	for(int i=0;i<last;i++)
	{
		int row = pNode[i].Row;
		int col = pNode[i].Col;
		int tmp = maxMap[row][col];
		if( tmp > max) max = tmp;
		maxMap[row][col] = mark;
	}
	return max-1;
}

