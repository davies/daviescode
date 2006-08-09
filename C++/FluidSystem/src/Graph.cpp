#include "stdafx.h"
#include "FluidSystem.h"
#include "Graph.h"
#include "math.h"
#include "data.h"

extern OilCrocks g_OilCrocks;

Graph::Graph()
{
	m_hwnd=NULL;
}

float Graph::GetStepSize(float length,int count = 7)
{
	static double steps[] = {1e-6,2e-6,5e-6,1e-5,2e-5,5e-5,1e-4,2e-4,5e-4,1e-3,2e-3,5e-3,0.01,0.02,0.05,0.1,0.2,0.5,
		1,2,5,10,20,50,100,200,500,1000,2000,5000,1e4,2e4,5e4,1e5,2e5,5e5,1e6,2e6,5e6,1e7,2e7,5e7,1e8,2e8,5e8,1e9,2e9,5e9,1e10,2e10,5e10};
	int i = 0;
	float multi=length / steps[i];
	while( multi > count ){
		i++;
		multi=length / steps[i];
	}
	return float(steps[i]);
}


ActionGraph::ActionGraph(){
	xOffset = 30;
	yOffset = 15;
	scale.time = 10;
	scale.value = 0.1f;
}

void ActionGraph::SetScale(Actions &actions) {
	if( actions.size() > 0){
		scale.time = actions.period *1.2 ;
		scale.value = 0.0;
		for( int i=0;i< actions.size();i++){
			if( fabs(actions[i].vel) > scale.value ) scale.value = fabs(actions[i].vel);
		}
		scale.value *= 1.2f;
	}
}

Node ActionGraph::ClientToReal(const CPoint &_p)
{
	Node p;
	p.time = GetScale().time *(_p.x -xOffset)/(width-xOffset*2);
	p.value = GetScale().value * (height/2-_p.y  )/(height/2 - yOffset);//此处有误，已改
	return p;
}

CPoint ActionGraph::RealToClient(const Node & _p)
{
	CPoint p;
	p.x = _p.time/GetScale().time * (width - xOffset*2) + xOffset;
	p.y = _p.value /GetScale().value * (height/2 - yOffset) + height/2;
	return p;
}

BOOL ActionGraph::ChangeTimeScale( CPoint p)
{
	if( scale.time <100 && p.x > width-xOffset && p.x < width-xOffset/2  && p.y < height/2+yOffset/2 && p.y > height/2 - yOffset/2 ){
		scale.time *= 1.5;
		return TRUE;
	}else if (scale.time > 2 && p.x > width-xOffset/2 && p.x < width && p.y < height/2+yOffset/2 && p.y > height/2 - yOffset/2  ){
		scale.time /= 1.5;
		return TRUE;
	}
	return FALSE;
}

BOOL ActionGraph::ChangeValueScale( CPoint p)
{
	if( scale.value > 0.010 && p.x > xOffset/2 && p.x < xOffset*3/2 && p.y > height-yOffset && p.y < height - yOffset/2 ){
		scale.value /= 1.5;
		return TRUE;
	}else if (scale.value < 0.600 && p.x > xOffset/2 && p.x < xOffset*3/2 && p.y < yOffset && p.y > yOffset/2  ){
		scale.value *= 1.5;
		return TRUE;
	}
	return FALSE;
}

void ActionGraph::DrawActions(Actions actions ,int action_selected)
{
	HDC hMemDC;              
    HBITMAP hBitmap, hOldBitmap; 
    hMemDC = CreateCompatibleDC(m_hDC);
	ASSERT(hMemDC);
	hBitmap = CreateCompatibleBitmap (m_hDC,width, height);
	ASSERT(hBitmap);
	hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);

	int gLength = 3; //刻度线长度
	float maxTime = scale.time;
	float maxValue = scale.value;
	float dTime,dValue;
	float timeCount,valueCount;
	float dx,dy;
	int i;

	//填充白色背景
	CRect rcArea;
	GetClientRect(this->GetHwnd(),&rcArea);
	FillRect( hMemDC, &rcArea, (HBRUSH)RGB(0,0,0));
	
	//画横坐标
	MoveToEx(hMemDC,xOffset,height/2,NULL);
	LineTo(hMemDC,width-xOffset,height/2);
	TextOut(hMemDC,width-xOffset*2,height/2+15,"时间(s)",7);

	dTime = GetStepSize(maxTime);
	timeCount = maxTime / dTime;
	dx = (width-xOffset*2) / timeCount;
	CString str;
	for(i=0;i<timeCount;i++)
	{
		MoveToEx(hMemDC,i*dx+xOffset,height/2,NULL);
		LineTo(hMemDC,i*dx+xOffset,height/2-gLength);
		str.Format("%f",i*dTime);
		TextOut(hMemDC,i*dx+xOffset,height/2+1,str,3);
	}

	//画纵坐标
	MoveToEx(hMemDC,xOffset,height-yOffset,NULL);
	LineTo(hMemDC,xOffset,yOffset);
	TextOut(hMemDC,xOffset*1.5,yOffset/2,"速度(mm/s)",10);

	dValue = GetStepSize(maxValue,4);
	valueCount = maxValue / dValue ;
	dy = (height/2-yOffset) / valueCount;

	for(i=0;i<valueCount;i++)
	{
		MoveToEx(hMemDC,xOffset,height/2-i*dy,NULL);
		LineTo(hMemDC,xOffset+gLength,height/2-i*dy);
		str.Format("%4.0f",i*dValue*1e3);
		TextOut(hMemDC,0,height/2-i*dy-8,str,4);

		MoveToEx(hMemDC,xOffset,height/2+i*dy,NULL);
		LineTo(hMemDC,xOffset+gLength,height/2+i*dy);
		str.Format("%4.0f",-i*dValue*1e3);
		TextOut(hMemDC,0,height/2+i*dy-8,str,4);
	}

	//画放缩箭头
	CPoint pt[3];
	HBRUSH brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	HBRUSH oldbrush = (HBRUSH)SelectObject(hMemDC,brush);
	//向上箭头
	pt[0]=CPoint(xOffset, yOffset/2);
	pt[1]=CPoint(xOffset/2,yOffset);
	pt[2]=CPoint(xOffset*3/2, yOffset);
	Polygon(hMemDC,pt,3);
	// 向下
	pt[0]=CPoint(xOffset, height-yOffset/2);
	pt[1]=CPoint(xOffset/2,height-yOffset);
	pt[2]=CPoint(xOffset*3/2, height-yOffset);
	Polygon(hMemDC,pt,3);
	//左
	pt[0]=CPoint(width-xOffset, height/2);
	pt[1]=CPoint(width-xOffset/2-1,height/2-yOffset/2);
	pt[2]=CPoint(width-xOffset/2-1, height/2+yOffset/2);
	Polygon(hMemDC,pt,3);
	//右
	pt[0]=CPoint(width, height/2);
	pt[1]=CPoint(width-xOffset/2+1,height/2-yOffset/2);
	pt[2]=CPoint(width-xOffset/2+1, height/2+yOffset/2);
	Polygon(hMemDC,pt,3);
	SelectObject(hMemDC,oldbrush);
	DeleteObject(brush);

	//画周期线
	HPEN hRedPen = CreatePen(PS_SOLID,3,RGB(255,0,0));
	HPEN hGreenPen = CreatePen(PS_SOLID,3,RGB(0,255,0));
	HPEN hBluePen = CreatePen(PS_SOLID,3,RGB(0,0,255));
	HPEN hOldPen;

	if(action_selected== -1 )
		hOldPen = (HPEN)SelectObject(hMemDC,hGreenPen);
	else
		hOldPen = (HPEN)SelectObject(hMemDC,hRedPen);
	MoveToEx(hMemDC,actions.period/dTime*dx+xOffset,height/4,NULL);	//画周期线
	LineTo(hMemDC,actions.period/dTime*dx+xOffset,height*3/4);

	for(i=0; i<actions.size(); i++){ //画周期表
		if(action_selected==i)
			SelectObject(hMemDC,hGreenPen);
		else
			SelectObject(hMemDC,hBluePen);
		MoveToEx(hMemDC,actions[i].startTime/dTime*dx+xOffset, -actions[i].vel/dValue*dy+height/2,NULL);
		LineTo(hMemDC,actions[i].endTime/dTime*dx+xOffset,-actions[i].vel/dValue*dy+height/2);
	}

	SelectObject(hMemDC,hOldPen);
	DeleteObject(hRedPen);
	DeleteObject(hGreenPen);
	DeleteObject(hBluePen);

	BitBlt(m_hDC,0,0,width,height,hMemDC,0,0,SRCCOPY);// 将内存位图复制到设备
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	
	//绘图完成后的清理
	DeleteObject(hMemDC);
}

FluxGraph::FluxGraph()
{
	xOffset = 35;
	yOffset = 25;
	gLength = 3; //刻度线长度

	DTime = 0.0f;
	DPress = 0.0f;
	DOilCount = 0.0f;
	DFlux = 0.0f;
}

void FluxGraph::GetValue( CPoint p, float &time,float &press,float &oilCount,float &flux)
{
	time = (p.x - xOffset)*DTime;
	press = (height - p.y - yOffset)*DPress + g_Box.p_min*1e-6;
	oilCount = (height - p.y - yOffset)* DOilCount + g_Box.minOil*1e3;
	flux = (height - p.y - yOffset) * DFlux;
}

void FluxGraph::Draw_Flux(OilCount Box,Flux allFlux,Flux pumpFlux,BOOL is_BoxOilChecked,BOOL is_BoxPressChecked,BOOL is_CrockChecked,BOOL is_PumpChecked,HWND hwnd)
{
	if( Box.size() <= 0 ) return ;

	m_hDC=::GetDC(hwnd);

	HDC hMemDC;              
    HBITMAP hBitmap, hOldBitmap; 
    hMemDC = CreateCompatibleDC(m_hDC);
	ASSERT(hMemDC);

	CRect rcArea;
	GetClientRect(hwnd,&rcArea);
	int width = rcArea.Width();
	int height = rcArea.Height();
	hBitmap = CreateCompatibleBitmap (m_hDC,width, height);
	hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
	
	int i;

	FillRect( hMemDC, &rcArea, (HBRUSH)RGB(0,0,0) );
	
	//画横坐标
	MoveToEx(hMemDC,xOffset,height-yOffset,NULL);
	LineTo(hMemDC,width-xOffset,height-yOffset);
	TextOut(hMemDC,width-xOffset*1.5,height-yOffset+5,"时间(s)",7);

	float maxTime = Box.back().time*1.2;
	float dTime = GetStepSize(maxTime);
	float count = maxTime / dTime;
	float dx = (width-xOffset*2) / count;
	DTime = dTime / dx;
	CString str;
	for(i=0;i<count;i++)
	{
		MoveToEx(hMemDC,i*dx+xOffset,height-yOffset,NULL);
		LineTo(hMemDC,i*dx+xOffset,height-yOffset-gLength);
		str.Format("%f",i*dTime);
		TextOut(hMemDC,i*dx+xOffset,height-yOffset+1,str,3);
	}

	//画左侧纵坐标
	if( is_BoxOilChecked ){
		MoveToEx(hMemDC,xOffset,height-yOffset,NULL);
		LineTo(hMemDC,xOffset,yOffset);
		TextOut(hMemDC,xOffset,0,"油量(L)",7);

		float maxValue = (g_Box.maxOil-g_Box.minOil)*1.2*1e3;
		float dValue = GetStepSize(maxValue);
		float count = maxValue / dValue;
		float dy = (height-yOffset*2) / count;
		DOilCount = dValue / dy;
		for(i=0;i<count;i++)
		{
			MoveToEx(hMemDC,xOffset,height-yOffset-i*dy,NULL);
			LineTo(hMemDC,xOffset+gLength,height-yOffset-i*dy);
			str.Format("%f",i*dValue+g_Box.minOil*1e3);
			TextOut(hMemDC,0,height-yOffset-i*dy-8,str,4);
		}
		//向上箭头
		CPoint pt[3];
		HBRUSH hBrush,oldbrush;
		hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		oldbrush=(HBRUSH)SelectObject(hMemDC,hBrush);
		pt[0]=CPoint(xOffset, yOffset*3/4);
		pt[1]=CPoint(xOffset*3/4,yOffset);
		pt[2]=CPoint(xOffset*5/4, yOffset);
		Polygon(hMemDC,pt,3);
		SelectObject(hMemDC,oldbrush);
		DeleteObject(hBrush);

		//画蓄能器油量曲线
		HPEN hPen,hOldPen;
		hPen = CreatePen(PS_SOLID,1,RGB(255,0,0));
		hOldPen = (HPEN)SelectObject(hMemDC,hPen);

		TextOut(hMemDC,100,0,"蓄能器油量",10);
		MoveToEx(hMemDC,100,17,NULL);
		LineTo(hMemDC,175,17);

		MoveToEx(hMemDC,xOffset,height-(g_Box.maxOil-g_Box.minOil)*1e3/dValue*dy-yOffset,NULL);
		for(i=0;i<Box.size()-1;i++){
			LineTo(hMemDC,Box[i+1].time/dTime*dx+xOffset,height-(Box[i+1].value-g_Box.minOil)*1e3/dValue*dy-yOffset );
		}
		SelectObject(hMemDC,hOldPen);
		DeleteObject(hPen);
	}
	
	if( is_BoxPressChecked ){
		MoveToEx(hMemDC,xOffset,height-yOffset,NULL);
		LineTo(hMemDC,xOffset,yOffset);
		TextOut(hMemDC,xOffset,0,"压强(MPa)",9);

		float maxValue = (g_Box.p_max-g_Box.p_min)*1.2*1e-6;
		float dValue = GetStepSize(maxValue);
		float count = maxValue / dValue;
		float dy = (height-yOffset*2) / count;
		DPress = dValue / dy;
		for(i=0;i<count;i++)
		{
			MoveToEx(hMemDC,xOffset,height-yOffset-i*dy,NULL);
			LineTo(hMemDC,xOffset+gLength,height-yOffset-i*dy);
			str.Format("%f",i*dValue+g_Box.p_min*1e-6);
			TextOut(hMemDC,0,height-yOffset-i*dy-8,str,4);
		}

		//向上箭头
		CPoint pt[3];
		HBRUSH hBrush,oldbrush;
		hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		oldbrush=(HBRUSH)SelectObject(hMemDC,hBrush);
		pt[0]=CPoint(xOffset, yOffset*3/4);
		pt[1]=CPoint(xOffset*3/4,yOffset);
		pt[2]=CPoint(xOffset*5/4, yOffset);
		Polygon(hMemDC,pt,3);
		SelectObject(hMemDC,oldbrush);
		DeleteObject(hBrush);

		//画蓄能器压强曲线
		HPEN hPen,hOldPen;
		hPen = CreatePen(PS_SOLID,1,RGB(255,0,0));
		hOldPen = (HPEN)SelectObject(hMemDC,hPen);

		TextOut(hMemDC,100,0,"蓄能器压力",10);
		MoveToEx(hMemDC,100,17,NULL);
		LineTo(hMemDC,175,17);

		int lastX = xOffset;
		float lastOil = g_Box.maxOil;
		MoveToEx(hMemDC,lastX,height-yOffset-(g_Box.p_max-g_Box.p_min)*1e-6/dValue*dy,NULL);
		for(i=0;i<Box.size();i++){
			int nextX = int(Box[i].time/dTime*dx)+xOffset;
			float nextOil = Box[i].value;
			for( int x=lastX;x<nextX;x++){
				float oil = lastOil + float(x-lastX) / (nextX-lastX) * (nextOil-lastOil);
				float p = g_Box.p_air / (1 - oil / g_Box.v) - g_Box.p_min;
				int y = height - yOffset - p*1e-6/dValue*dy;
				LineTo(hMemDC,x,y);
			}
			lastX = nextX;
			lastOil = nextOil;
		}
		SelectObject(hMemDC,hOldPen);
		DeleteObject(hPen);
	}

	//画右侧纵坐标
	//求三个坐标轴的最大值
	if( is_CrockChecked || is_PumpChecked ){
		MoveToEx(hMemDC,width-xOffset,height-yOffset,NULL);
		LineTo(hMemDC,width-xOffset,yOffset);
		TextOut(hMemDC,width-xOffset*2.5,0,"流量(L/min)",11);

		float maxValue = 0;
		for(i=0;i<pumpFlux.size();i++){
			if( maxValue < pumpFlux[i].value ) maxValue = pumpFlux[i].value;
		}
		for(i=0;i<allFlux.size();i++){
			if( maxValue < allFlux[i].value ) maxValue = allFlux[i].value;
		}
		maxValue *= 1.2*1e3*60;
		float dValue = GetStepSize(maxValue);
		float count = maxValue / dValue;
		float dy = (height-yOffset*2) / count;
		DFlux = dValue / dy;
		for(i=0;i<count;i++)
		{
			MoveToEx(hMemDC,width-xOffset,height-yOffset-i*dy,NULL);
			LineTo(hMemDC,width-xOffset-gLength,height-yOffset-i*dy);
			str.Format("%f",i*dValue);
			TextOut(hMemDC,width-xOffset+1,height-yOffset-i*dy-8,str,4);
		}
		//向上箭头
		CPoint pt[3];
		HBRUSH hBrush,oldbrush;
		hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		oldbrush=(HBRUSH)SelectObject(hMemDC,hBrush);
		pt[0]=CPoint(width-xOffset, yOffset*3/4);
		pt[1]=CPoint(width-xOffset*3/4,yOffset);
		pt[2]=CPoint(width-xOffset*5/4, yOffset);
		Polygon(hMemDC,pt,3);
		SelectObject(hMemDC,oldbrush);
		DeleteObject(hBrush);
		
		//画油缸总流量曲线
		if(is_CrockChecked){
			HPEN hPen,hOldPen;
			hPen = CreatePen(PS_SOLID,3,RGB(0,0,0));
			hOldPen = (HPEN)SelectObject(hMemDC,hPen);
			TextOut(hMemDC,200,0,"油缸总流量",10);
			MoveToEx(hMemDC,200,17,NULL);
			LineTo(hMemDC,275,17);
			float last = 0.0;
			MoveToEx(hMemDC,xOffset,height-yOffset,NULL);
			for(i=0;i<allFlux.size();i++){
				float time = allFlux[i].time;
				int x = time/dTime*dx;
				LineTo(hMemDC,int(allFlux[i].time/dTime*dx)+xOffset,height-int(allFlux[i].value*1e3*60/dValue*dy)-yOffset);
				LineTo(hMemDC,int(allFlux[i+1].time/dTime*dx)+xOffset,height-int(allFlux[i].value*1e3*60/dValue*dy)-yOffset);
				last = allFlux[i].value*1e3*60;
			}
			SelectObject(hMemDC,hOldPen);
			DeleteObject(hPen);
		}

		//画油泵的流量
		if(is_PumpChecked){
			HPEN hPen,hOldPen;
			hPen = CreatePen(PS_DASH,1,RGB(0,0,255));
			hOldPen = (HPEN)SelectObject(hMemDC,hPen);
			TextOut(hMemDC,300,0,"油泵流量",8);
			MoveToEx(hMemDC,300,17,NULL);
			LineTo(hMemDC,360,17);
			float last = 0.0;
			for(i=0;i<pumpFlux.size()-1;i++){
				MoveToEx(hMemDC,int(pumpFlux[i].time/dTime*dx)+xOffset,height-int(last/dValue*dy)-yOffset,NULL);
				LineTo(hMemDC,int(pumpFlux[i].time/dTime*dx)+xOffset,height-int(pumpFlux[i].value*1e3*60/dValue*dy)-yOffset);
				LineTo(hMemDC,int(pumpFlux[i+1].time/dTime*dx)+xOffset,height-int(pumpFlux[i].value*1e3*60/dValue*dy)-yOffset);
				last = pumpFlux[i].value*1e3*60;
			}
			SelectObject(hMemDC,hOldPen);
			DeleteObject(hPen);
		}
	}

	BitBlt(m_hDC,0,0,width,height,hMemDC,0,0,SRCCOPY);// 将内存位图复制到设备
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteObject(hMemDC);
	ReleaseDC(hwnd,m_hDC);
}

