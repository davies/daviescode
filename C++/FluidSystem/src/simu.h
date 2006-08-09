#ifndef __SIMU_H__
#define __SIMU_H__

#include <vector>
#include <algorithm>

typedef ::std::string String;

struct Action{
	float vel;
	float endTime;
	float startTime;
	
	Action( float _startTime =0.0,float _endTime = 0.0,float _vel = 0.0){
		startTime = _startTime;
		endTime = _endTime;
		vel = _vel;
	}
	bool operator < ( const Action &p){
		if( startTime < p.startTime )
			return true;
		else 
			return false;
	}
	bool operator > ( const Action &p ){
		if( startTime > p.startTime )
			return true;
		else 
			return false;
	}
};

class Actions : public ::std::vector<Action>
{
public:
	float period;
	Actions(){
		period = 0.0f;
	}
};

struct Node{
public:
	float value;
	float time;
	
	Node( float _time = 0.0, float _value = 0.0){
		time = _time;
		value = _value;
	}
	bool operator < (const Node & p) const {
		if( time < p.time ) 
			return true;
		else 
			return false;
	}
	bool operator > (const Node &p) const {
		if( time > p.time )
			return true;
		else 
			return false;
	}
};

struct OilCrock;

class Flux : public ::std::vector<Node> {
public:
	float period;	//周期	

	Flux(){
		period = 0.0;
	};
	Flux(const OilCrock & oilCrock);	//由油缸的动作周期表生成流量表
	friend Flux& operator += (Flux &a, const Flux &b);
};

//所有参数全部用国际单位制
struct OilCrock{		//油缸
	String name;	//
	float D;			//油缸直径
	float pD;		//杆径
	float length;		//行程
	int		count;

	Actions actions;	//动作周期表
	float period;		//动作周期

};
typedef ::std::vector<OilCrock> OilCrocks;

//泵
struct Pump{
	float quality;//排量
	float rotateSpeed;//转速
	float efficiency;//效率
	int count;
};

//蓄能器
struct OilBox{
	float v;	//容积
	float  p_max,//最高压强
		   p_min,//最低压强
		   p_hypoMin,//次低压强
	       p_air;//充气压强
	float  maxOil,minOil,midOil;//最大,最小,次小油量
	void Update(){
		if ( p_air == 0 ) return ;
		maxOil = (1-p_air/p_max)*v;
		midOil = (1-p_air/p_hypoMin)*v;
		minOil = (1-p_air/p_min)*v;
	}
};

typedef ::std::vector<Node> OilCount;

//仿真
OilCount Simulation(const OilCrocks &crocks,const OilBox &box,const Pump &pump,Flux &pumpFlux,Flux &allflux);

#endif