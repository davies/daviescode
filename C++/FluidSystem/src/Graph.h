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
	Graph();//最大20s,20mm/s

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
	int width,height;	//图形区域的高度和宽度
	int xOffset,yOffset; //画图时预留宽度
	HDC m_hDC;//设备描述表不同, 小心不要release掉
	HWND m_hwnd;//窗口相同
};

class ActionGraph : public Graph  
{
public:
	ActionGraph();
	Node GetScale()const  {return scale;}
	void SetScale(Actions &actions); //根据动作周期表设置坐标大小
	BOOL ChangeTimeScale( CPoint p); //改变时间轴的坐标大小
	BOOL ChangeValueScale( CPoint p); //改变速度轴的坐标大小

	CPoint RealToClient( const Node &p); //时间速度到屏幕点的转换
	Node ClientToReal( const CPoint &p);//屏幕点到时间、速度轴的转换

	void DrawActions(Actions actions,int action_selected=-2 ); //画动作周期表
protected:
	Node scale;//时间、速度坐标的大小 
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
	int gLength ; //刻度线长度

	float DTime;
	float DPress;
	float DOilCount;
	float DFlux;
};

#endif // __GRAPH_H__
