#include "search.h"

//我方的策略选择
void Choose(int ID,MapStatus &best,MapStatus &newVal,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	PlayerStatus * best1 = &best.status[ID];
	PlayerStatus * best2 = &best.status[1-ID];
	PlayerStatus * newVal1 = &newVal.status[ID];
	PlayerStatus * newVal2 = &newVal.status[1-ID];
	//先看生死情况
	if( best1->dead != newVal1->dead ){
		if( !best1->dead ) return ;
	}else if(best2->dead != newVal2->dead ){
		if( best2->dead ) return;
	}else if( best1->dead ) { //我必死
		if( best2->dead || newVal2->dead ) { //对方也死
			if( best2->dead ) return ;
		}else{//对方不死
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
		//对方死
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
		}else if(best2->space != newVal2->space ){ //压缩对方的控制区域
			if(best2->space <= newVal2->space ) return ;
		}
	//最后看吃果子情况
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
			//如果长度较短，则尽快吃果子
			if( best1->canEatFruit != newVal1->canEatFruit ){
				if( best1->canEatFruit ) return ;
			}else if(best1->canEatFruit ) {
				if( best1->distToFruit != newVal1->distToFruit ){
					if( best1->distToFruit <= newVal1->distToFruit) return ;
				}else if( best1->space != newVal1->space ){
					if( best1->space >= newVal1->space ) return ;
				}
			}else {
				if(best2->space != newVal2->space ){ //压缩对方的控制区域
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
	//newVal 比 best 更好，交换 best 和 newVal
	memcpy(&best,&newVal,sizeof(MapStatus));
}