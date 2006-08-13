//*************************************************************************
//2003年智能体大赛参赛程序——决不贪吃
//by Davies , 2003-3-20
//*************************************************************************

#ifndef ANALYZE_H
#define ANALYZE_H

//一方的分析结果
struct PlayerStatus{
	bool safe;							//蛇是否安全
	bool dead;							//蛇是否会死
	int space;							//蛇的控制区域大小
	int living;							//如果舍会死，它表示蛇还能存活的回合数
	int distToFruit;					//到果子的最短距离，用以判断是否能吃到果子
	bool canEatFruit;					//是否能吃到果子
};

//地图的分析结果
struct MapStatus{
	PlayerStatus status[2];				//双方的状态
	int	move[2];						//双方的行动
	int score;							//当前行动的蛇的得分，
										//如果吃到果子，则分数为正，如果对方吃到果子，则分数为负
};

//求解双方的生死以及生存空间
void getLiving(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi);	//得到当前双方的生存空间，

#endif