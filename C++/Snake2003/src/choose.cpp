//*************************************************************************
//2003年智能体大赛参赛程序——决不贪吃
//by Davies , 2003-3-20
//*************************************************************************

//*************************************************************************
//比较不同地图的分析结果，返回对ID更有利的结果
//*************************************************************************

#include "search.h"

//策略选择
void Choose(int ID,MapStatus &best,MapStatus &newVal,MAP_INFO *info,PLAYER_STRUCT *ps)
{
	//假定best为对id更有利的状态，
	//比较后，如果best比newVal有利，则直接返回，否则将newVal的内容拷贝到best

	PlayerStatus * best1 = &best.status[ID];	//优先方的最好状态
	PlayerStatus * best2 = &best.status[1-ID];
	PlayerStatus * newVal1 = &newVal.status[ID];
	PlayerStatus * newVal2 = &newVal.status[1-ID];
	//先比较生死情况
	if( best1->dead != newVal1->dead ){
		if( !best1->dead ) return ;
	}else if(best2->dead != newVal2->dead ){
		if( best2->dead ) return;
	}else if( best1->dead ) { //我必死
		if( best2->dead || newVal2->dead ) { //对方也死
			if( best2->dead ) return ;
		}else{//对方不死
			if( best.score != newVal.score ){	//优先吃果子
				if( best.score >= newVal.score ) return ;
			}else if( best1->canEatFruit != newVal1->canEatFruit ){
				if( best1->canEatFruit ) return ;
			}else if( best1->space != newVal1->space ){	//取空间大的为优
				if(best1->space >= newVal1->space ) return ;
			}else if( best1->living != newVal1->living ){
				if(best1->living >= newVal1->living ) return ;
			}
		}
	}else if( best2->dead ) {		//对方死
		if( best.score != newVal.score ){	//优先已经吃到果子的情况
			if(best.score >= newVal.score) return ;
		}else if( best2->space != newVal2->space ){  //压缩对方空间
			if(best2->space < newVal2->space ) return ;
		}else if(best2->living != newVal2->living ){ //减小对方的存在时间
			if( best2->living < newVal2->living ) return ;
		}else if( best1->canEatFruit != newVal1->canEatFruit ){//自己能否吃到果子
			if(best1->canEatFruit ) return ;
		}else if( best1->canEatFruit && best1->distToFruit != newVal1->distToFruit){
			if( best1->distToFruit <= newVal1->distToFruit )	return ;
		}else if(best1->living != newVal1->living ){
			if( best1->living >= newVal1->living ) return ;
		}
	//最后看吃果子情况
	}else if( best.score != newVal.score ){		//优先已经吃到果子的情况
		if(best.score > newVal.score )return ;
	}else if( best2->safe != newVal2->safe ){	//然后考虑是否安全，取自己安全，对方危险的方案
		if( !best2->safe ) return ;
	}else if( best1->safe != newVal1->safe ){
		if( best1->safe ) return ;
	}else if(!best1->safe || !best2->safe ){	//双方都不安全，则尽量压缩对方区域，扩大己方区域
		if( best1->space - best2->space != newVal1->space - newVal2->space ){
			if( best1->space - best2->space > newVal1->space - newVal2->space ) return ;
		}else if(best2->space != newVal2->space ){
			if( best2->space < newVal2->space ) return ;
		}
	}else {
		//根据参数调整策略，
		if(info->SnakeArr[ID].Length < ps->MaxLength 
			|| (info->ResultInfo[ID].Score < info->ResultInfo[1-ID].Score && info->CurTime > ps->ii->TotalTime/3)
			|| ps->ii->TotalTime-info->CurTime<200 || info->Fruit.Value > 25 ){
			//如果当前自己的长度小于最大长度，或者自己的分数小于对方的分数，
			//或者比赛只剩下不到200回合，再或者果子的分数大于25，则尽量果子
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
		}else { //当不满足上面的那些情况时，尽量压缩对方的空间，寻找杀机
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