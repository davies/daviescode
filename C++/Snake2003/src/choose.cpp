//*************************************************************************
//2003������������������򡪡�����̰��
//by Davies , 2003-3-20
//*************************************************************************

//*************************************************************************
//�Ƚϲ�ͬ��ͼ�ķ�����������ض�ID�������Ľ��
//*************************************************************************

#include "search.h"

//����ѡ��
void Choose(int ID,MapStatus &best,MapStatus &newVal,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	//�ٶ�bestΪ��id��������״̬��
	//�ȽϺ����best��newVal��������ֱ�ӷ��أ�����newVal�����ݿ�����best

	PlayerStatus * best1 = &best.status[ID];	//���ȷ������״̬
	PlayerStatus * best2 = &best.status[1-ID];
	PlayerStatus * newVal1 = &newVal.status[ID];
	PlayerStatus * newVal2 = &newVal.status[1-ID];
	//�ȱȽ��������
	if( best1->dead != newVal1->dead ){
		if( !best1->dead ) return ;
	}else if(best2->dead != newVal2->dead ){
		if( best2->dead ) return;
	}else if( best1->dead ) { //�ұ���
		if( best2->dead || newVal2->dead ) { //�Է�Ҳ��
			if( best2->dead ) return ;
		}else{//�Է�����
			if( best.score != newVal.score ){	//���ȳԹ���
				if( best.score >= newVal.score ) return ;
			}else if( best1->canEatFruit != newVal1->canEatFruit ){
				if( best1->canEatFruit ) return ;
			}else if( best1->space != newVal1->space ){	//ȡ�ռ���Ϊ��
				if(best1->space >= newVal1->space ) return ;
			}else if( best1->living != newVal1->living ){
				if(best1->living >= newVal1->living ) return ;
			}
		}
	}else if( best2->dead ) {		//�Է���
		if( best.score != newVal.score ){	//�����Ѿ��Ե����ӵ����
			if(best.score >= newVal.score) return ;
		}else if( best2->space != newVal2->space ){  //ѹ���Է��ռ�
			if(best2->space < newVal2->space ) return ;
		}else if(best2->living != newVal2->living ){ //��С�Է��Ĵ���ʱ��
			if( best2->living < newVal2->living ) return ;
		}else if( best1->canEatFruit != newVal1->canEatFruit ){//�Լ��ܷ�Ե�����
			if(best1->canEatFruit ) return ;
		}else if( best1->canEatFruit && best1->distToFruit != newVal1->distToFruit){
			if( best1->distToFruit <= newVal1->distToFruit )	return ;
		}else if(best1->living != newVal1->living ){
			if( best1->living >= newVal1->living ) return ;
		}
	//��󿴳Թ������
	}else if( best.score != newVal.score ){		//�����Ѿ��Ե����ӵ����
		if(best.score > newVal.score )return ;
	}else if( best2->safe != newVal2->safe ){	//Ȼ�����Ƿ�ȫ��ȡ�Լ���ȫ���Է�Σ�յķ���
		if( !best2->safe ) return ;
	}else if( best1->safe != newVal1->safe ){
		if( best1->safe ) return ;
	}else if(!best1->safe || !best2->safe ){	//˫��������ȫ������ѹ���Է��������󼺷�����
		if( best1->space - best2->space != newVal1->space - newVal2->space ){
			if( best1->space - best2->space > newVal1->space - newVal2->space ) return ;
		}else if(best2->space != newVal2->space ){
			if( best2->space < newVal2->space ) return ;
		}
	}else {
		//���ݲ����������ԣ�
		if(info->SnakeArr[ID].Length < ps->MaxLength 
			|| (info->ResultInfo[ID].Score < info->ResultInfo[1-ID].Score && info->CurTime > ps->ii->TotalTime/3)
			|| ps->ii->TotalTime-info->CurTime<200 || info->Fruit.Value > 25 ){
			//�����ǰ�Լ��ĳ���С����󳤶ȣ������Լ��ķ���С�ڶԷ��ķ�����
			//���߱���ֻʣ�²���200�غϣ��ٻ��߹��ӵķ�������25����������
			if( best1->canEatFruit != newVal1->canEatFruit ){
				if( best1->canEatFruit ) return ;
			}else if(best1->canEatFruit ) {
				if( best1->distToFruit != newVal1->distToFruit ){
					if( best1->distToFruit <= newVal1->distToFruit) return ;
				}else if( best1->space != newVal1->space ){
					if( best1->space >= newVal1->space ) return ;
				}
			}else {
				if(best2->space != newVal2->space ){ //ѹ���Է��Ŀ�������
					if(best2->space <= newVal2->space ) return ;
				}else if( best1->space!= newVal1->space){
					if(best1->space >= newVal1->space) return ;
				}
			}
		}else { //���������������Щ���ʱ������ѹ���Է��Ŀռ䣬Ѱ��ɱ��
			if( best2->space != newVal2->space ){
				if( best2->space < newVal2->space ) return ;
			}else if( best1->canEatFruit != newVal1->canEatFruit ){
				if( best1->canEatFruit ) return ;
			}else if( best1->space != newVal1->space){
				if( best1->space> newVal1->space) return ;
			}else if( best1->canEatFruit && best1->distToFruit != newVal1->distToFruit ){
				if( best1->distToFruit < newVal1->distToFruit ) return ;
			}
		}
	}
	//newVal �� best ���ã����� best �� newVal
	memcpy(&best,&newVal,sizeof(MapStatus));
}