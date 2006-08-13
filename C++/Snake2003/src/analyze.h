//*************************************************************************
//2003������������������򡪡�����̰��
//by Davies , 2003-3-20
//*************************************************************************

#ifndef ANALYZE_H
#define ANALYZE_H

//һ���ķ������
struct PlayerStatus{
	bool safe;							//���Ƿ�ȫ
	bool dead;							//���Ƿ����
	int space;							//�ߵĿ��������С
	int living;							//��������������ʾ�߻��ܴ��Ļغ���
	int distToFruit;					//�����ӵ���̾��룬�����ж��Ƿ��ܳԵ�����
	bool canEatFruit;					//�Ƿ��ܳԵ�����
};

//��ͼ�ķ������
struct MapStatus{
	PlayerStatus status[2];				//˫����״̬
	int	move[2];						//˫�����ж�
	int score;							//��ǰ�ж����ߵĵ÷֣�
										//����Ե����ӣ������Ϊ��������Է��Ե����ӣ������Ϊ��
};

//���˫���������Լ�����ռ�
void getLiving(MapStatus& current,int** pMap,MAP_INFO *info,PLAYER_STRUCT *ps,PLAYER_INFO *pi);	//�õ���ǰ˫��������ռ䣬

#endif