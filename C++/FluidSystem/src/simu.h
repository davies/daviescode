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
	float period;	//����	

	Flux(){
		period = 0.0;
	};
	Flux(const OilCrock & oilCrock);	//���͸׵Ķ������ڱ�����������
	friend Flux& operator += (Flux &a, const Flux &b);
};

//���в���ȫ���ù��ʵ�λ��
struct OilCrock{		//�͸�
	String name;	//
	float D;			//�͸�ֱ��
	float pD;		//�˾�
	float length;		//�г�
	int		count;

	Actions actions;	//�������ڱ�
	float period;		//��������

};
typedef ::std::vector<OilCrock> OilCrocks;

//��
struct Pump{
	float quality;//����
	float rotateSpeed;//ת��
	float efficiency;//Ч��
	int count;
};

//������
struct OilBox{
	float v;	//�ݻ�
	float  p_max,//���ѹǿ
		   p_min,//���ѹǿ
		   p_hypoMin,//�ε�ѹǿ
	       p_air;//����ѹǿ
	float  maxOil,minOil,midOil;//���,��С,��С����
	void Update(){
		if ( p_air == 0 ) return ;
		maxOil = (1-p_air/p_max)*v;
		midOil = (1-p_air/p_hypoMin)*v;
		minOil = (1-p_air/p_min)*v;
	}
};

typedef ::std::vector<Node> OilCount;

//����
OilCount Simulation(const OilCrocks &crocks,const OilBox &box,const Pump &pump,Flux &pumpFlux,Flux &allflux);

#endif