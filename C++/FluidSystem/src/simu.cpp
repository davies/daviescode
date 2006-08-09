#include "stdafx.h"
#include <iostream>
#include <math.h>
#include "simu.h"

using namespace std;

#define PI	3.1415926

Flux::Flux(const OilCrock & oilCrock)
{	
	Actions actions = oilCrock.actions;
	sort(actions.begin(),actions.end());//排序

	period = actions.period;
	push_back(Node()); //加入初始值(0,0)

	int count = actions.size();
	for(int i = 0;i < count;i++){
		float vel = actions[i].vel;
		float flux;
		if( vel >0 )
			flux = vel * PI/4 * oilCrock.D * oilCrock.D * oilCrock.count;
		else 
			flux = -vel * PI/4 *(oilCrock.D*oilCrock.D-oilCrock.pD*oilCrock.pD) * oilCrock.count;
		if( i==0 && actions[i].startTime == 0){
			pop_back();
		}
		push_back( Node( actions[i].startTime, flux) ); //流量恒为正
		push_back( Node( actions[i].endTime,0.0 ) );
	}

	if( size() && back().time < period ){
		push_back( Node(period,0.0) );
	}
//	if( !empty() && front().time > 0){ //如果没有0起点，则增加
//		push_back(Node());
//	}
}

int getMaxDivisor(int a,int b)
{
	a = abs(a);
	b = abs(b);
	if( a < b) swap(a,b);

	while( b ){
		a = a % b;
		swap(a,b);
	}

	return a;
}

int getMinMulti(int a,int b)
{
	return a*b/getMaxDivisor(a,b);
}

Flux& operator += (Flux &fluxA, const Flux &_fluxB)
{
	//sort(fluxA.begin(),fluxA.end());
	//sort(fluxA.begin(),fluxB.end());
	if( _fluxB.period == 0) 
		return fluxA;
	if( fluxA.period == 0 ){
		fluxA = _fluxB;
		return fluxA;
	}
	//先将两组信号化成周期相同的，然后再相加
	Flux fluxB = _fluxB;
	float period = (float)getMinMulti( int(fluxA.period*10),int(fluxB.period*10) ) / 10;
	int i;
	int count = (int(period/fluxA.period)-1)*fluxA.size();
	for(i=0;i<count;i++){
		fluxA.push_back(Node(fluxA[i].time+fluxA.period,fluxA[i].value));
	}
	fluxA.period = period;
	count = (int(period/fluxB.period)-1)*fluxB.size();
	for(i=0;i<count;i++){
		fluxB.push_back(Node(fluxB[i].time+fluxB.period,fluxB[i].value));
	}
	fluxB.period = period;

	Flux sum;
	Flux::iterator pA;
	Flux::iterator pB;
	pA = fluxA.begin();
	pB = fluxB.begin();
	double lastA = 0.0,lastB = 0.0;

	//
	while( pA < fluxA.end() && pB < fluxB.end() )
	{
		if( *pA < *pB ){ // < 
			sum.push_back( Node( (*pA).time,(*pA).value + lastB ) );
			lastA = (*pA).value;
			pA++;
		}else if ( *pA > *pB ){ // > 
			sum.push_back( Node( (*pB).time, (*pB).value + lastA ) );
			lastB = (*pB).value;
			pB++;
		}else { // == 
			sum.push_back( Node( (*pA).time,(*pA).value + (*pB).value ) );
			lastA = (*pA).value;
			lastB = (*pB).value;
			pA++;
			pB++;
		}
	}

	for(;pA<fluxA.end();pA++){
		sum.push_back( Node( (*pA).time,(*pA).value + lastB ) );
	}
	for(;pB<fluxB.end();pB++){
		sum.push_back( Node( (*pB).time, (*pB).value + lastA ) );
	}

	sum.period = period;
	fluxA = sum;
	return fluxA;
}

OilCount Simulation(const OilCrocks & crocks,const OilBox &oilBox,const Pump &pump,Flux &pumpFlux, Flux & _sumFlux)
{
	int i=0;

	pumpFlux.clear();
	Flux sumFlux;
	sumFlux.push_back(Node());
	OilCrocks::const_iterator pCrock;
	for(pCrock=crocks.begin();pCrock<crocks.end();pCrock++)
	{
		Flux flux( *pCrock );
		sumFlux += flux;
	}

	//如果周期很短，则多延拓几个周期
/*	int maxPeriod = 15;
	if( sumFlux.period < maxPeriod ){
	}
*/
	int plus = 1;
	int count = (plus-1)*sumFlux.size();
	for(i=0;i<count;i++){
		sumFlux.push_back(Node(sumFlux[i].time+sumFlux.period,sumFlux[i].value));
	}
	sumFlux.period = plus * sumFlux.period;

	_sumFlux = sumFlux;

	//初始化
	OilCount oilCount;
	oilCount.push_back(Node(0,oilBox.maxOil)); 
	float currOil = oilBox.maxOil;
	pumpFlux.push_back(Node());
	int currPump = 0;

	float pumpFluxMax = pump.quality*pump.rotateSpeed*pump.efficiency*pump.count;

	i=0;
	while( i < sumFlux.size()-1 ) //流量的最后一点为0
	{
		float predOil = currOil + (currPump*pumpFluxMax-sumFlux[i].value)*(sumFlux[i+1].time-sumFlux[i].time);
		if( currPump ==0 && currOil > oilBox.midOil && predOil < oilBox.midOil ){
			//求解转折点
			float crossTime = sumFlux[i].time + (currOil-oilBox.midOil) / (sumFlux[i].value-currPump*pumpFluxMax);
			currPump = 1;
			pumpFlux.push_back(Node(crossTime,currPump*pumpFluxMax));
			currOil = oilBox.midOil;
			oilCount.push_back(Node(crossTime,oilBox.midOil));
			sumFlux[i].time = crossTime;
		}else if( currPump == 1 && currOil < oilBox.maxOil && predOil > oilBox.maxOil ){ //油泵必须停止
			float crossTime = sumFlux[i].time + (oilBox.maxOil-currOil) / (currPump*pumpFluxMax - sumFlux[i].value);
			currPump = 0;
			pumpFlux.push_back(Node(crossTime,currPump*pumpFluxMax));
			currOil = oilBox.maxOil;
			oilCount.push_back(Node(crossTime,oilBox.maxOil));
			sumFlux[i].time = crossTime;
		}else{
			currOil = predOil;
			oilCount.push_back(Node(sumFlux[i+1].time,predOil));
			i++;
		}
	}
	pumpFlux.push_back(Node(sumFlux.period,currPump*pumpFluxMax));
	return oilCount;
}