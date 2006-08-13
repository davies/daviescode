#ifndef ANALYZE_H
#define ANALYZE_H

struct PlayerStatus{
	int space;
	int living;			//蛇的生存空间，用以判断蛇的生死
	bool safe;
	bool dead;		//0
	int distToFruit;	//到果子的最短距离，用以判断是否能吃到果子
	bool canEatFruit;	//是否能吃到果子
	bool safeFruit;		//果子是否安全
};

struct MapStatus{
	PlayerStatus status[2];		//己方的状态
	int	move[2];				//己方的行动
	int score;					//吃果子的得分
};

void getLiving(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi);	//得到当前双方的生存空间，

#endif