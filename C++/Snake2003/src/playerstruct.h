#ifndef GREEDY_SNAKE_HPP
#define GREEDY_SNAKE_HPP

#include <windows.h>
#include <stdio.h>

#define MAXSNAKECOUNT	8				//�ߵ�������� 
#define	MAXEXISTTIME	50				//һ��Fruit�����Դ��ڶ೤ʱ��
#define LEFT			0				//0��ʾ���
#define UP				1				//1��ʾ�ϱ�
#define RIGHT			2				//2��ʾ�Ҳ�
#define DOWN			3				//3��ʾ�±�

#define MAXLENGTH 400
#define MAXDEPTH 6

struct POSITION							//������Ľṹ��ʹ�ã�������ʾһ������
{
    	int	        Row;				//��
	    int	        Col;				//��
};


struct INITINFO							//��player��ʼ��ʱ�����ṹ�����룬����֪ͨplayerһЩ��Ϣ
{
        DWORD       TotalTime;			//��Ϸ���ܻغ���
        DWORD       SnakeCount;			//һ���ж�������
        DWORD       TimeOut;			//ÿһ�غϵ�ʱ�䣬��λ:ms��
        DWORD       ID;					//�����ߵ���š���ŵ��ô����¡�
};

struct FRUIT							//��������Fruit�Ľṹ
{
	   DWORD		Value;				//Fruit�ļ�ֵ
	   POSITION	    Pos;				//Fruit��λ��
	   DWORD		ExistTime;			//Fruit�Ѿ����ڵ�ʱ��
};

struct SNAKE							//��������Snake�Ľṹ
{
	   DWORD		Direction;			//Snakeͷ����
	   DWORD		Length;				//Snake�ĳ���
	   POSITION	    Pos[0];				//���մ�ͷ��β��˳��,�洢��Snake��λ��
										//��Ȼ�ӱ����Ͽ�,����һ���㳤����,������԰��������
										//����ΪLength������.
};

struct SNAKERESULT						//������ߵķ�����һЩս��ͳ��
{
    	DWORD		TimesOfDie;			//�����Ĵ���
	    DWORD		FruitsEatten;		//�Թ���Fruit�ĸ���
	    DWORD		MaxScore;			//���ﵽ����߷���
	    DWORD		MaxLength;			//���ﵽ�������
	    int		    Score;				//Ŀǰ�ķ���
};

struct MAPFILEHEAD									//����ŵ�ͼ��һЩ����
{
        WORD        reserved1;
        WORD        reserved2;
		DWORD		reserved3;
		DWORD		Height;							//��ͼ�ĸ߶�
		DWORD		Width;							//��ͼ�Ŀ��
		DWORD		reserved4;	
		POSITION	StartPosition[MAXSNAKECOUNT];	//ÿ���ߵ���ʼ�ص�,StartPosition[INITINFO::ID]Ϊ�����ߵ���ʼ�ص�.
		DWORD		StartDirection[MAXSNAKECOUNT];	//ÿ���ߵ���ʼ����,StartDirection[INITINFO::ID]Ϊ�����ߵ���ʼ����
};

struct PLAYER_INFO									//ÿ���غϴ���ѡ�ֵĽṹ
{
	SNAKE*			SnakeArr[MAXSNAKECOUNT];		//ָ��ÿ���ߵ�ָ������
	FRUIT			Fruit;							//Fruit��Ϣ
	SNAKERESULT		ResultInfo[MAXSNAKECOUNT];		//
	DWORD			CurTime;						//��ǰ�Ļغ���
};

struct MAPFILESTRUCT								//player��ʼ��ʱ����player�Ľṹ��
{
	char**			pMap;							//��ά���飬pMap[Row][Col]��ʾ��Row�е�Col�е���Ʒ��
													//���ڵ���0��ʾ�ذ壬С��0��ʾǽ
	MAPFILEHEAD		mfh;							//��ͼ��Ϣ
};

struct AISNAKE							//��������Snake�Ľṹ
{
	   DWORD		Direction;			//Snakeͷ����
	   DWORD		Length;				//Snake�ĳ���
	   DWORD		HeadPos;			//Snake��ͷ��λ��
	   POSITION	    Pos[MAXLENGTH];		//���մ�ͷ��β��˳��,�洢��Snake��λ��
										//������HeadPos��ͷ��һ����ʾ�ߵ�λ��
};

struct MAP_INFO									//ÿ���غϴ���ѡ�ֵĽṹ
{
	AISNAKE			SnakeArr[2];		//ָ��ÿ���ߵ�ָ������
	FRUIT			Fruit;							//Fruit��Ϣ
	SNAKERESULT		ResultInfo[MAXSNAKECOUNT];		//
	DWORD			CurTime;						//��ǰ�Ļغ���
};

struct PLAYER_STRUCT{
		INITINFO*		ii;
		MAPFILESTRUCT*	mfs;
		
		//�������������Ҫ��ȫ�ֱ���
		DWORD	Height;
		DWORD	Width;
		int** pMap[MAXDEPTH+1];			//�����õ�ͼ
		int** tmpMap[3];				//��ʱ��ͼ
		MAP_INFO *info[MAXDEPTH+1];		//�����ĵ�ͼ��Ϣ
		POSITION *nodes[3];				//��¼�����Ľڵ�
		int count[2];
		int depth;						//��ǰ�������
		int MaxPaths;
		int MaxLength;
		int paths;						//��ǰ����·����
		FILE *logFile;
		bool load;
};

#endif
