#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "simu.h"

#pragma once

class Graph  
{
public:
	static BOOL PtInRect(CRect rc,CPoint pt){
		if ((pt.x-rc.left)*(pt.x-rc.right) < 0 && (pt.y-rc.bottom)*(pt.y-rc.top) < 0)    return TRUE;
		return FALSE;
	}
	Graph();//���20s,20mm/s

	inline HDC GetDC() {return m_hDC;}
	inline HWND GetHwnd() const {return m_hwnd; }
	inline void SetHwnd(const HWND _hwnd) {  m_hwnd=_hwnd; }

	inline void InitialGraph(const HWND _hwnd){
		m_hDC=::GetDC(_hwnd);
		SetHwnd(_hwnd);

		CRect rcArea;
		GetClientRect(GetHwnd(),&rcArea);
		width = rcArea.Width();
		height = rcArea.Height();
	}	
protected:
	float GetStepSize(float length,int stepCount);
	int width,height;	//ͼ������ĸ߶ȺͿ��
	int xOffset,yOffset; //��ͼʱԤ�����
	HDC m_hDC;//�豸������ͬ, С�Ĳ�Ҫrelease��
	HWND m_hwnd;//������ͬ
};

class ActionGraph : public Graph  
{
public:
	ActionGraph();
	Node GetScale()const  {return scale;}
	void SetScale(Actions &actions); //���ݶ������ڱ����������С
	BOOL ChangeTimeScale( CPoint p); //�ı�ʱ����������С
	BOOL ChangeValueScale( CPoint p); //�ı��ٶ���������С

	CPoint RealToClient( const Node &p); //ʱ���ٶȵ���Ļ���ת��
	Node ClientToReal( const CPoint &p);//��Ļ�㵽ʱ�䡢�ٶ����ת��

	void DrawActions(Actions actions,int action_selected=-2 ); //���������ڱ�
protected:
	Node scale;//ʱ�䡢�ٶ�����Ĵ�С 
};

enum Pos {left,middle,right,space,period} ;
typedef struct tag_status{
	int curActionIndex;
	Action orig_action;
	Pos mouse_Pos;
	Node Last_Pos;
} Status;

class FluxGraph : public Graph  
{
public:
	FluxGraph();
	void GetValue( CPoint p, float &time,float &press,float &oilCount,float &flux);
	void Draw_Flux(OilCount Box,Flux OilCrocks,Flux Pump,BOOL is_BoxOilChecked,BOOL is_BoxPressChecked,BOOL is_CrockChecked,BOOL is_PumpChecked,HWND hwnd);
protected:
	int gLength ; //�̶��߳���

	float DTime;
	float DPress;
	float DOilCount;
	float DFlux;
};

#endif // __GRAPH_H__
