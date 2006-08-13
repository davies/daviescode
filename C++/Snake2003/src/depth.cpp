#include "search.h"
#include "analyze.h"
#include "getFruit.h"

void trySearch (int depth,PLAYER_STRUCT *ps,PLAYER_INFO *pi)
{
	MAP_INFO *info = ps->info[0];
	int **pMap	=	ps->pMap[0];
	int Height = ps->Height;
	int Width = ps->Width;
	
	//��ʼ����ͼ
	if(depth==0)		//copy player info
		InitMap(pMap,info,ps,pi);

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
			|| pMap[row1][col1] < -1-depth ) //��Ϊ-1ʱ��׷β
		{
			continue;
		}
		//�ٶ��ߵ����λ��
		mySnake->HeadPos ++;
		mySnake->Pos[mySnake->HeadPos].Row = row1;
		mySnake->Pos[mySnake->HeadPos].Col = col1;
		int oldDir1 = mySnake->Direction;
		mySnake->Direction = (mySnake->Direction+i+3) % 4;
		int old1 = pMap[row1][col1];
		pMap[row1][col1] = -mySnake->Length-depth-1;

		for(int j=0;j<3;j++) 		//�Է��ж�����������
		{
			int row2 = hisSnake->Pos[hisSnake->HeadPos].Row +Direct[(hisSnake->Direction+j+3) % 4][0];
			int col2 = hisSnake->Pos[hisSnake->HeadPos].Col +Direct[(hisSnake->Direction+j+3) % 4][1];
			if( row2 <0 || row2 >= Width || col2<0 || col2>=Height 
				|| pMap[row2][col2] < -1-depth )//��Ϊ-1ʱ��׷β
			{
				continue;
			}
			if(depth == ps->depth-1)
				ps->paths ++;
			else
			{
				//�ٶ��ߵ����λ��
				hisSnake->HeadPos ++;
				hisSnake->Pos[hisSnake->HeadPos].Row = row2;
				hisSnake->Pos[hisSnake->HeadPos].Col = col2;
				int oldDir2 = hisSnake->Direction;
				hisSnake->Direction = (hisSnake->Direction+j+3) % 4;
				int old2 = pMap[row2][col2];
				pMap[row2][col2] = -hisSnake->Length-depth-1;

				trySearch(depth+1,ps,pi);

				//�ָ�ԭ����ֵ
				hisSnake->HeadPos --;
				hisSnake->Direction = oldDir2;
				pMap[row2][col2] = old2;
			}
		}
		//�ָ�ԭ����ֵ
		mySnake->HeadPos --;
		mySnake->Direction = oldDir1;
		pMap[row1][col1] = old1;
	}
}

//��ȡ�ڵ�ǰ������ʺ����������
int getDepth( PLAYER_STRUCT *ps,PLAYER_INFO *pi )
{
	int oldPaths =0;
	for(int i=3;i<MAXDEPTH;i++)
	{
		ps->depth = i;
		ps->paths = 0;
		trySearch(0,ps,pi);
		if( ps->paths > ps->MaxPaths ){
			ps->paths = oldPaths;
			return i-1 ;
		}else{
			oldPaths = ps->paths;
		}
	}
	return MAXDEPTH-1;
}