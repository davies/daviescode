#ifndef ANALYZE_H
#define ANALYZE_H

struct PlayerStatus{
	int space;
	int living;			//�ߵ�����ռ䣬�����ж��ߵ�����
	bool safe;
	bool dead;		//0
	int distToFruit;	//�����ӵ���̾��룬�����ж��Ƿ��ܳԵ�����
	bool canEatFruit;	//�Ƿ��ܳԵ�����
	bool safeFruit;		//�����Ƿ�ȫ
};

struct MapStatus{
	PlayerStatus status[2];		//������״̬
	int	move[2];				//�������ж�
	int score;					//�Թ��ӵĵ÷�
};

void getLiving(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi);	//�õ���ǰ˫��������ռ䣬

#endif