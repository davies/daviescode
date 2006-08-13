#ifndef GREEDY_SNAKE_HPP
#define GREEDY_SNAKE_HPP

#include <windows.h>
#include <stdio.h>

#define MAXSNAKECOUNT	8				//蛇的最大数量 
#define	MAXEXISTTIME	50				//一个Fruit最多可以存在多长时间
#define LEFT			0				//0表示左侧
#define UP				1				//1表示上边
#define RIGHT			2				//2表示右侧
#define DOWN			3				//3表示下边

#define MAXLENGTH 400
#define MAXDEPTH 6

struct POSITION							//在下面的结构中使用，用来表示一个坐标
{
    	int	        Row;				//行
	    int	        Col;				//列
};


struct INITINFO							//当player初始化时，本结构被传入，用于通知player一些信息
{
        DWORD       TotalTime;			//游戏的总回合数
        DWORD       SnakeCount;			//一共有多少条蛇
        DWORD       TimeOut;			//每一回合的时间，单位:ms。
        DWORD       ID;					//本条蛇的序号。序号的用处见下。
};

struct FRUIT							//用于描述Fruit的结构
{
	   DWORD		Value;				//Fruit的价值
	   POSITION	    Pos;				//Fruit的位置
	   DWORD		ExistTime;			//Fruit已经存在的时间
};

struct SNAKE							//用于描述Snake的结构
{
	   DWORD		Direction;			//Snake头方向
	   DWORD		Length;				//Snake的长度
	   POSITION	    Pos[0];				//按照从头到尾的顺序,存储着Snake的位置
										//虽然从表面上看,这是一个零长数组,但你可以把它想象成
										//长度为Length的数组.
};

struct SNAKERESULT						//存放着蛇的分数及一些战术统计
{
    	DWORD		TimesOfDie;			//死过的次数
	    DWORD		FruitsEatten;		//吃过的Fruit的个数
	    DWORD		MaxScore;			//曾达到的最高分数
	    DWORD		MaxLength;			//曾达到的最长长度
	    int		    Score;				//目前的分数
};

struct MAPFILEHEAD									//存放着地图的一些参数
{
        WORD        reserved1;
        WORD        reserved2;
		DWORD		reserved3;
		DWORD		Height;							//地图的高度
		DWORD		Width;							//地图的宽度
		DWORD		reserved4;	
		POSITION	StartPosition[MAXSNAKECOUNT];	//每条蛇的起始地点,StartPosition[INITINFO::ID]为本条蛇的起始地点.
		DWORD		StartDirection[MAXSNAKECOUNT];	//每条蛇的起始方向,StartDirection[INITINFO::ID]为本条蛇的起始方向。
};

struct PLAYER_INFO									//每个回合传给选手的结构
{
	SNAKE*			SnakeArr[MAXSNAKECOUNT];		//指向每条蛇的指针数组
	FRUIT			Fruit;							//Fruit信息
	SNAKERESULT		ResultInfo[MAXSNAKECOUNT];		//
	DWORD			CurTime;						//当前的回合数
};

struct MAPFILESTRUCT								//player初始化时传给player的结构。
{
	char**			pMap;							//二维数组，pMap[Row][Col]表示第Row行第Col列的物品。
													//大于等于0表示地板，小于0表示墙
	MAPFILEHEAD		mfh;							//地图信息
};

struct AISNAKE							//用于描述Snake的结构
{
	   DWORD		Direction;			//Snake头方向
	   DWORD		Length;				//Snake的长度
	   DWORD		HeadPos;			//Snake的头的位置
	   POSITION	    Pos[MAXLENGTH];		//按照从头到尾的顺序,存储着Snake的位置
										//其中以HeadPos开头得一串表示蛇的位置
};

struct MAP_INFO									//每个回合传给选手的结构
{
	AISNAKE			SnakeArr[2];		//指向每条蛇的指针数组
	FRUIT			Fruit;							//Fruit信息
	SNAKERESULT		ResultInfo[MAXSNAKECOUNT];		//
	DWORD			CurTime;						//当前的回合数
};

struct PLAYER_STRUCT{
		INITINFO*		ii;
		MAPFILESTRUCT*	mfs;
		
		//在这里加入你需要的全局变量
		DWORD	Height;
		DWORD	Width;
		int** pMap[MAXDEPTH+1];			//搜索用地图
		int** tmpMap[3];				//临时地图
		MAP_INFO *info[MAXDEPTH+1];		//搜索的地图信息
		POSITION *nodes[3];				//纪录经过的节点
		int count[2];
		int depth;						//当前搜索深度
		int MaxPaths;
		int MaxLength;
		int paths;						//当前搜索路径数
		FILE *logFile;
		bool load;
};

#endif
