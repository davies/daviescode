#include "search.h"

//�ҷ��Ĳ���ѡ��
void Choose(int ID,MapStatus &best,MapStatus &newVal,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	PlayerStatus * best1 = &best.status[ID];
	PlayerStatus * best2 = &best.status[1-ID];
	PlayerStatus * newVal1 = &newVal.status[ID];
	PlayerStatus * newVal2 = &newVal.status[1-ID];
	//�ȿ��������
	if( best1->dead != newVal1->dead ){
		if( !best1->dead ) return ;
	}else if(best2->dead != newVal2->dead ){
		if( best2->dead ) return;
	}else if( best1->dead ) { //�ұ���
		if( best2->dead || newVal2->dead ) { //�Է�Ҳ��
			if( best2->dead ) return ;
		}else{//�Է�����
			if( best.score != newVal.score ){
				if( best.score >= newVal.score ) return ;
			}else if( best1->canEatFruit != newVal1->canEatFruit ){
				if( best1->canEatFruit ) return ;
			}else if( best1->space != newVal1->space ){
				if(best1->space >= newVal1->space ) return ;
			}else if( best1->living != newVal1->living ){
				if(best1->living >= newVal1->living ) return ;
			}
		}
	}else if( best2->dead ) {
		//�Է���
		if( best.score != newVal.score ){
			if(best.score >= newVal.score) return ;
		}else if( best2->space != newVal2->space ){
			if(best2->space < newVal2->space ) return ;
		}else if(best2->living != newVal2->living ){
			if( best2->living < newVal2->living ) return ;
		}else if( best1->canEatFruit != newVal1->canEatFruit ){
			if(best1->canEatFruit ) return ;
		}else if( best1->canEatFruit && best1->distToFruit != newVal1->distToFruit){
			if( best1->distToFruit <= newVal1->distToFruit )	return ;
		}else if(best1->living != newVal1->living ){
			if( best1->living >= newVal1->living ) return ;
		}else if(best2->space != newVal2->space ){ //ѹ���Է��Ŀ�������
			if(best2->space <= newVal2->space ) return ;
		}
	//��󿴳Թ������
	}else if( best.score != newVal.score ){
		if(best.score > newVal.score )return ;
	}else if( best2->safe != newVal2->safe ){
		if( !best2->safe ) return ;
	}else if( best1->safe != newVal1->safe ){
		if( best1->safe ) return ;
	}else if(!best1->safe || !best2->safe ){
		if( best1->space - best2->space != newVal1->space - newVal2->space ){
			if( best1->space - best2->space > newVal1->space - newVal2->space ) return ;
		}else if(best2->space != newVal2->space ){
			if( best2->space < newVal2->space ) return ;
		}
	}else {
		if(info->SnakeArr[ID].Length < ps->MaxLength 
			|| (info->ResultInfo[ID].Score < info->ResultInfo[1-ID].Score && info->CurTime > ps->ii->TotalTime/3)
			|| ps->ii->TotalTime-info->CurTime<200 || info->Fruit.Value > 25 ){
			//������Ƚ϶̣��򾡿�Թ���
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
		}else {
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