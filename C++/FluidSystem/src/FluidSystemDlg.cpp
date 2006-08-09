// FluidSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FluidSystem.h"
#include "FluidSystemDlg.h"
#include "data.h"
#include "graph.h"
#include "simu.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ActionGraph g_actiongraph;
FluxGraph g_fluxgraph;

#define DRAWFLUX() g_fluxgraph.Draw_Flux(m_OilCount,m_All_Flux,m_Pump_Flux,m_box_checked,m_all_checked,m_pump_checked)
#define DRAWACTIONS()	{if( m_CrockIndex>0 )g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions); }

/////////////////////////////////////////////////////////////////////////////
// CFluidSystemDlg dialog

CFluidSystemDlg::CFluidSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFluidSystemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFluidSystemDlg)
	m_all_checked = TRUE;
	m_pump_checked = TRUE;
	m_effective = 0.0f;
	m_maxp = 0.0f;
	m_midp = 0.0f;
	m_minp = 0.0f;
	m_pump_count = 0;
	m_quality = 0.0f;
	m_rotate = 0.0f;
	m_vol = 0.0f;
	m_airp = 0.0f;
	m_CrockIndex = -1;
	b_addLine = FALSE;
	b_deleteLine = FALSE;
	b_moveLine = TRUE;
	m_box_checked = TRUE;
	m_boxpress_checked = FALSE;
	m_strStart = _T("0.0");
	m_strEnd = _T("0.0");
	m_strVel = _T("0.0");
	m_strPeriod = _T("0.0");
	b_modifyLine = FALSE;
	p_zoomDlg=NULL;
	m_msg = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFluidSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFluidSystemDlg)
	DDX_Control(pDX, IDC_PERIOD, m_ctrlPeriod);
	DDX_Control(pDX, IDC_START, m_ctrlStart);
	DDX_Control(pDX, IDC_END, m_ctrlEnd);
	DDX_Control(pDX, IDC_VEL, m_ctrlVel);
	DDX_Control(pDX, IDC_CROCKLIST, m_CrockList);
	DDX_Check(pDX, IDC_CHECK_ALLFLUX, m_all_checked);
	DDX_Check(pDX, IDC_CHECK_PUMPFLUX, m_pump_checked);
	DDX_Text(pDX, IDC_EFFECTIVE, m_effective);
	DDV_MinMaxFloat(pDX, m_effective, 0.f, 100.f);
	DDX_Text(pDX, IDC_MAXP, m_maxp);
	DDV_MinMaxFloat(pDX, m_maxp, 1.e-005f, 100000.0f);
	DDX_Text(pDX, IDC_MIDP, m_midp);
	DDV_MinMaxFloat(pDX, m_midp, 1.e-005f, 100000.0f);
	DDX_Text(pDX, IDC_MINP, m_minp);
	DDV_MinMaxFloat(pDX, m_minp, 1.e-005f, 100000.0f);
	DDX_Text(pDX, IDC_PUMP_COUNT, m_pump_count);
	DDV_MinMaxInt(pDX, m_pump_count, 0, 10000);
	DDX_Text(pDX, IDC_QUALITY, m_quality);
	DDV_MinMaxFloat(pDX, m_quality, 1.e-005f, 100000.0f);
	DDX_Text(pDX, IDC_ROTATE, m_rotate);
	DDV_MinMaxFloat(pDX, m_rotate, 1.e-005f, 100000.0f);
	DDX_Text(pDX, IDC_VOL, m_vol);
	DDV_MinMaxFloat(pDX, m_vol, 1.e-005f, 10000.0f);
	DDX_Text(pDX, IDC_AIRP, m_airp);
	DDV_MinMaxFloat(pDX, m_airp, 1.e-005f, 100000.0f);
	DDX_LBIndex(pDX, IDC_CROCKLIST, m_CrockIndex);
	DDX_Check(pDX, IDC_ADDLINE, b_addLine);
	DDX_Check(pDX, IDC_DELETELINE, b_deleteLine);
	DDX_Check(pDX, IDC_MOVELINE, b_moveLine);
	DDX_Check(pDX, IDC_CHECK_BOXOIL, m_box_checked);
	DDX_Check(pDX, IDC_CHECK_BOXPRESS, m_boxpress_checked);
	DDX_Text(pDX, IDC_START, m_strStart);
	DDV_MaxChars(pDX, m_strStart, 5);
	DDX_Text(pDX, IDC_END, m_strEnd);
	DDX_Text(pDX, IDC_VEL, m_strVel);
	DDX_Text(pDX, IDC_PERIOD, m_strPeriod);
	DDX_Check(pDX, IDC_MODIFYLINE, b_modifyLine);
	DDX_Text(pDX, IDC_MSG, m_msg);
	//}}AFX_DATA_MAP
}

void CFluidSystemDlg::UpdateData(BOOL bSaveAndValidate = TRUE)
{
	if( !bSaveAndValidate ){
		//Box
		m_vol = g_Box.v*1e3;
		m_airp = g_Box.p_air*1e-6;
		m_maxp = g_Box.p_max*1e-6;
		m_midp = g_Box.p_hypoMin*1e-6;
		m_minp = g_Box.p_min*1e-6;
		//Pump
		m_quality = g_Pump.quality*1e6;
		m_effective = g_Pump.efficiency*100;
		m_rotate = g_Pump.rotateSpeed*60;
		m_pump_count = g_Pump.count;
	}

	CDialog::UpdateData( bSaveAndValidate );
	
	if( bSaveAndValidate ){
		//Box
		g_Box.v = m_vol*1e-3;
		g_Box.p_air = m_airp*1e6;
		g_Box.p_max = m_maxp*1e6;
		g_Box.p_hypoMin = m_midp*1e6 ;
		g_Box.p_min = m_minp*1e6;
		g_Box.Update();

		//Pump
		g_Pump.quality = m_quality*1e-6;
		g_Pump.efficiency = m_effective/100;
		g_Pump.rotateSpeed = m_rotate/60;
		g_Pump.count = m_pump_count;
	}
}

BEGIN_MESSAGE_MAP(CFluidSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CFluidSystemDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_OPEN, OnOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_QUIT, OnQuit)
	ON_COMMAND(ID_SIMULATE, OnSimulate)
	ON_BN_CLICKED(IDC_CHECK_ALLFLUX, OnCheckAllflux)
	ON_BN_CLICKED(IDC_CHECK_PUMPFLUX, OnCheckPumpflux)
	ON_BN_CLICKED(IDC_CHECK_BOX, OnCheckOilcount)
	ON_EN_CHANGE(IDC_VOL, OnChangeVol)
	ON_EN_CHANGE(IDC_MAXP, OnChangeMaxp)
	ON_EN_CHANGE(IDC_MIDP, OnChangeMidp)
	ON_EN_CHANGE(IDC_MINP, OnChangeMinp)
	ON_EN_CHANGE(IDC_AIRP, OnChangeAirp)
	ON_EN_CHANGE(IDC_QUALITY, OnChangeQuality)
	ON_EN_CHANGE(IDC_ROTATE, OnChangeRotate)
	ON_EN_CHANGE(IDC_EFFECTIVE, OnChangeEffective)
	ON_EN_CHANGE(IDC_PUMP_COUNT, OnChangePumpCount)
	ON_LBN_SELCHANGE(IDC_CROCKLIST, OnSelchangeCrocklist)
	ON_COMMAND(ID_CLEAR, OnClear)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_COMMAND(ID_PASTE, OnPaste)
	ON_COMMAND(ID_CUT, OnCut)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_BN_CLICKED(IDC_ADDLINE, OnAddline)
	ON_BN_CLICKED(IDC_MOVELINE, OnMoveline)
	ON_BN_CLICKED(IDC_DELETELINE, OnDeleteline)
	ON_BN_CLICKED(IDC_CHECK_BOXPRESS, OnCheckBoxpress)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_PRINT, OnPrint)
	ON_BN_CLICKED(IDC_MODIFYLINE, OnModifyline)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFluidSystemDlg message handlers

BOOL CFluidSystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	((CButton*)GetDlgItem(IDC_CHECK_BOXOIL))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_BOXPRESS))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_ALLFLUX))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_PUMPFLUX))->SetCheck(TRUE);

	((CButton*)GetDlgItem(IDC_ADDLINE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_DELETELINE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_MOVELINE))->SetCheck(TRUE);

	m_ctrlEnd.SetReadOnly( TRUE );
	m_ctrlStart.SetReadOnly( TRUE );
	m_ctrlPeriod.SetReadOnly( TRUE );
	m_ctrlVel.SetReadOnly( TRUE );

	//添加工具条
	if (!m_toolbar.CreateEx( this,TBSTYLE_FLAT,  WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS ,
		CRect(4,4,0,0)) ||	!m_toolbar.LoadToolBar(IDD_TOOLBAR) )
	{
		TRACE0("failed to create toolbar\n");
		return FALSE;
	}
	m_toolbar.ShowWindow(SW_SHOW);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	g_actiongraph.InitialGraph(::GetDlgItem(this->GetSafeHwnd(),IDC_STATIC_ACTION));
	g_fluxgraph.InitialGraph(::GetDlgItem(this->GetSafeHwnd(),IDC_STATIC_FLUX));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFluidSystemDlg::OnPaint() 
{
	if(m_CrockIndex < g_OilCrocks.size() ){
		if(!b_modifyLine)
			g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions);
		else 
			g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions,status.curActionIndex);
	}
	g_fluxgraph.Draw_Flux(m_OilCount,m_All_Flux,m_Pump_Flux,m_box_checked,m_boxpress_checked,m_all_checked,m_pump_checked,g_fluxgraph.GetHwnd());

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CFluidSystemDlg::OnOK()
{
	UpdateData(TRUE);
	if( ! b_modifyLine ) return ;

	if( m_CrockIndex <0 || m_CrockIndex >= g_OilCrocks.size() ) 
		return ; //没有油缸对象
	Actions & actions=g_OilCrocks[m_CrockIndex].actions;
	CString str;
	float period=0;
	float leftLimit=0;
	float rightLimit=g_actiongraph.GetScale().time;
	if(status.curActionIndex==-2){
		return;
	}else if(status.curActionIndex==-1){
		period=atof(m_strPeriod);
		if(actions.size()>0)
			leftLimit=actions.back().endTime;
		rightLimit=g_actiongraph.GetScale().time;
		if(period < leftLimit ){
			str.Format("请输入一个大于%3.1f的数",leftLimit);
			AfxMessageBox(str);
			period =actions.period;
		}
		ASSERT(leftLimit<rightLimit);
		if(period> rightLimit){
			str.Format("请输入一个大于%3.1f的数",rightLimit);
			AfxMessageBox(str);
			period =actions.period;
		}
		m_strPeriod.Format("%3.1f",period);
		actions.period = int(period*10)/10.0f;
		UpdateData(FALSE);
	}else {
		float start=0,end=0,vel=0;
		start=atof(m_strStart);
		end=atof(m_strEnd);
		vel=atof(m_strVel) /1000.0f;
		
		if( status.curActionIndex > 0 ){
			leftLimit = actions[status.curActionIndex-1].endTime;
		}
		if( status.curActionIndex < actions.size()-1 ){
			rightLimit = actions[status.curActionIndex].endTime;
		}
		
		//对超出移动范围的处理
		if( start < leftLimit  || start> rightLimit){//从左边超出
			str.Format("请在起点中输入一个介于%3.1f和%3.1f之间的数",leftLimit,rightLimit);
			AfxMessageBox(str);
			start = actions[status.curActionIndex].startTime;
			//start=leftLimit;
			
		}
		
		if( status.curActionIndex > 0 ){
			leftLimit = actions[status.curActionIndex].startTime;
		}
		if( status.curActionIndex < actions.size()-1 ){
			rightLimit = actions[status.curActionIndex+1].startTime;
		}
		
		if( end > rightLimit || end<leftLimit){//从右边超出
			str.Format("请在起点中输入一个介于%3.1f和%3.1f之间的数",leftLimit,rightLimit);
			AfxMessageBox(str);
			end = actions[status.curActionIndex].endTime;
		}
		if(fabs(vel) >g_actiongraph.GetScale().value){
			str.Format("请在速度中输入一个绝对值小于%4.1f的数",g_actiongraph.GetScale().value * 1000);
			AfxMessageBox(str);
			if( vel> 0 )
				vel = g_actiongraph.GetScale().value ;
			else
				vel = -g_actiongraph.GetScale().value ;
			vel=actions[status.curActionIndex].vel;
		}
		actions[status.curActionIndex].startTime = start;
		actions[status.curActionIndex].endTime = end;
		actions[status.curActionIndex].vel = vel;
		m_strStart.Format("%3.1f",start) ;
		m_strEnd.Format("%3.1f",end);
		m_strVel.Format("%4.1f",vel*1000);
		UpdateData(FALSE);
	}
	g_actiongraph.DrawActions(actions,status.curActionIndex);
	OnSimulate();
	
	UpdateData(FALSE);	

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFluidSystemDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFluidSystemDlg::OnOpen() 
{
	char szFileFilter[]=
		"EXCEL 文件(*.xls)|*.xls||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,szFileFilter);
	if( dlg.DoModal() == IDOK ){
		filename=dlg.GetPathName();
		if( OpenExcelFile(filename) ){
			int i;
			while( m_CrockList.GetCount() > 0)
				m_CrockList.DeleteString(0);
			for( i=0;i<g_OilCrocks.size();i++){
				String name = g_OilCrocks[i].name;
				m_CrockList.AddString( name.c_str() );
			}
			m_CrockIndex = 0;
			UpdateData( FALSE );
			
			if(g_OilCrocks.size()>0){
				m_strPeriod.Format("%3.1f",g_OilCrocks[m_CrockIndex].actions.period);
				g_actiongraph.SetScale(g_OilCrocks[m_CrockIndex].actions);
				g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions);
				OnSimulate();
			}
		}

	}
}	

void CFluidSystemDlg::OnLButtonDown(UINT nFlags, CPoint point) //相对对话框左上角
{
	CDialog::OnLButtonDown(nFlags, point);

	CRect rect_Action;
	::GetWindowRect(g_actiongraph.GetHwnd(),&rect_Action);
	CRect rect_Flux;
	::GetWindowRect(g_fluxgraph.GetHwnd(),&rect_Flux);
	CPoint pt_client(point);
	ClientToScreen(&pt_client);
	
	if( rect_Flux.PtInRect(pt_client)){//点击位置不在picture控件上
		UpdateData( TRUE );
		
		float time,press,oilCount,flux;
		::ScreenToClient(g_fluxgraph.GetHwnd(),&pt_client);//得到客户坐标
		g_fluxgraph.GetValue(pt_client,time,press,oilCount,flux);
		
		m_msg = "当前值为：";
		CString str;
		str.Format("%2.1fs,",time);
		m_msg += str;
		if( m_all_checked || m_pump_checked ){
			str.Format("%1.1fL/min,",flux);
			m_msg += str;
		}
		if( m_box_checked ){
			str.Format("%2.2fL",oilCount);
			m_msg += str;
		}
		if( m_boxpress_checked ){
			str.Format( "%2.2fMPa",press);
			m_msg += str;
		}

		UpdateData( FALSE );
		return;
	}

	if(! rect_Action.PtInRect(pt_client)){//点击位置不在picture控件上
		return;
	}

	if( m_CrockIndex <0 || m_CrockIndex >= g_OilCrocks.size() ) return ; //没有油缸对象

	Actions & actions=g_OilCrocks[m_CrockIndex].actions;
	::ScreenToClient(g_actiongraph.GetHwnd(),&pt_client);//得到客户坐标
	Node pt;
	pt=g_actiongraph.ClientToReal(pt_client);

	if(g_actiongraph.ChangeTimeScale(pt_client) 
		|| g_actiongraph.ChangeValueScale(pt_client) ){//是否点击在放缩上
		g_actiongraph.DrawActions(actions);
		return;
	}
	
	status.curActionIndex=-2;
	m_ctrlEnd.SetReadOnly( TRUE );
	m_ctrlStart.SetReadOnly(TRUE);
	m_ctrlPeriod.SetReadOnly(TRUE);
	m_ctrlVel.SetReadOnly(TRUE);

	status.mouse_Pos=space;
	int i=0;

	const float precision_X=0.1*g_actiongraph.GetScale().time;
	const float precision_Y=0.05*g_actiongraph.GetScale().value;
	if(b_addLine){
		//画新线	//对画线初始位置进行判断
		//判断起始点是否在绘图区,如果不在直接返回
		if(pt.time<0 || pt.time>actions.period || fabs(pt.value)>g_actiongraph.GetScale().value ){
			SetCursor(LoadCursor(NULL,IDC_NO));
			return;	
		}
		for(i=0; i<=actions.size() ; i++){				
			if( pt.time >= actions[i].startTime && pt.time <= actions[i].endTime ){
				//与某根线段重叠
				SetCursor(LoadCursor(NULL,IDC_NO));
				return;	
			}
		}
		Actions::iterator p;
		for(p=actions.begin(),i=0; p<actions.end() ; p++,i++){				
			if( pt.time < p->startTime ){
				actions.insert(p,Action(pt.time,pt.time,pt.value));
				status.curActionIndex = i;
				status.Last_Pos=pt;
				return;	
			}
		}
		actions.push_back(Action(pt.time,pt.time,pt.value));
		status.curActionIndex = actions.size()-1;
		status.Last_Pos=pt;
	}else if( b_deleteLine ){//删除
		Actions::iterator i;
		for(i=actions.begin();i<actions.end();i++){
			float start=i->startTime;
			float end=i->endTime ;
			float vel=i->vel ;
			if( pt.time >= start && pt.time <= end && fabs(pt.value-vel) < precision_Y ){
				actions.erase(i);
				g_actiongraph.DrawActions(actions);
				OnSimulate();
				break ;
			}
		}
	}else if(b_moveLine ){//如果不是画新线
		status.mouse_Pos=space;
		//周期线
		if(fabs(pt.time-actions.period ) < precision_X && fabs(pt.value) < 0.5*g_actiongraph.GetScale().value ){
			status.mouse_Pos=period;
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			return;
		}
		//动作线
		for(i=0;i<actions.size();i++){
			float start=actions[i].startTime;
			float end=actions[i].endTime ;
			float vel=actions[i].vel ;
			float length=end-start;
			CString str;
			str.Format("start=%f,end=%f,vel=%f",start,end,vel);
			TRACE(str);
			//ASSERT(length >= 0);
			if(start  <= pt.time && end  >= pt.time && fabs(vel  - pt.value) <precision_Y){//如果在某直线横坐标范围内 //如果离直线比较近
				float mid = (start+end) / 2;
				if(fabs(pt.time) < mid + 0.3*length && fabs(pt.time) > mid -0.3*length    ){//如果在直线的中间
					SetCursor(LoadCursor(NULL,IDC_SIZEALL));//鼠标改为移动的形状
					status.orig_action=Action(actions[i].startTime,actions[i].endTime,actions[i].vel);
					status.mouse_Pos = middle;
					status.Last_Pos = pt;
					status.curActionIndex = i;	
				}else if(pt.time-start < 0.1 *length && pt.time-start > -0.2 *length){//如果在直线的左端，
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));//鼠标改为向左拉动的形状
					status.mouse_Pos=left;
					status.curActionIndex = i;	
					status.orig_action=Action(actions[i].startTime,actions[i].endTime,actions[i].vel);
				} else if( pt.time-end > -0.1 *length && pt.time-end < 0.2 *length ){//如果在直线的右端
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));//鼠标为向右拉伸的形状
					status.mouse_Pos=right;
					status.curActionIndex = i;	
					status.orig_action=Action(actions[i].startTime,actions[i].endTime,actions[i].vel);
				}
				m_strEnd.Format("%3.1f",actions[i].endTime);
				m_strStart.Format("%3.1f",actions[i].startTime);
				m_strVel.Format("%3.1f",actions[i].vel * 1000);
				m_strPeriod.Format("%3.1f",actions.period);
				UpdateData(FALSE);
			}
		}
	}
	else if(b_modifyLine){
		if(fabs(pt.time-actions.period ) < precision_X/2.0 && fabs(pt.value) < 0.5*g_actiongraph.GetScale().value ){
			status.curActionIndex =-1;
			m_strEnd=m_strVel=m_strStart=_T("");
			m_ctrlEnd.SetReadOnly( TRUE );
			m_ctrlStart.SetReadOnly( TRUE );
			m_ctrlVel.SetReadOnly( TRUE );
			m_ctrlPeriod.SetReadOnly(0);
			
		}
		else{
			for(i=0;i<actions.size();i++){
				float start=actions[i].startTime;
				float end=actions[i].endTime ;
				float vel=actions[i].vel ;
				float length=end-start;
				CString str;
				str.Format("start=%f,end=%f,vel=%f",start,end,vel);
				TRACE(str);
				//ASSERT(length >= 0);
				if(start  <= pt.time && end  >= pt.time && fabs(vel  - pt.value) <precision_Y){
					//如果在某直线横坐标范围内 //如果离直线比较近				
					status.curActionIndex = i;
					m_ctrlEnd.SetReadOnly(0);
					m_ctrlStart.SetReadOnly(0);
					m_ctrlVel.SetReadOnly(0);
					m_ctrlPeriod.SetReadOnly( TRUE );
					m_strEnd.Format("%3.1f",actions[status.curActionIndex ].endTime);
					m_strStart.Format("%3.1f",actions[status.curActionIndex ].startTime);
					m_strVel.Format("%3.1f",actions[status.curActionIndex ].vel*1000);
				}
			}
			
		}
		m_strPeriod.Format("%3.1f",actions.period);
		g_actiongraph.DrawActions(actions,status.curActionIndex);
		UpdateData(FALSE);
	}
}

void CFluidSystemDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonUp(nFlags,point);
	//清除时间特别短的动作
	if( m_CrockIndex >=0 && m_CrockIndex < g_OilCrocks.size() ){
		Actions & actions = g_OilCrocks[m_CrockIndex].actions;	
		Actions::iterator i;
		for( i=actions.begin();i<actions.end();i++){
			if( (*i).endTime - (*i).startTime <= 0.1 ){
				actions.erase(i);
			}
		}
	}
	if(!b_modifyLine)
		status.curActionIndex = -2;

	m_msg = "";
	UpdateData( FALSE );
}

void CFluidSystemDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags,point);

	if( ! (nFlags & MK_LBUTTON) ){ //如果左键是否按下
		return ;
	}

	CRect rect_Action;
	::GetWindowRect(g_actiongraph.GetHwnd(),&rect_Action);
	CPoint pt_client(point);
	ClientToScreen(&pt_client);
	if(! rect_Action.PtInRect(pt_client)){//点击位置不在picture控件上
		return;
	}

	if( m_CrockIndex <0 || m_CrockIndex >= g_OilCrocks.size() ) return ; //没有油缸对象
	Actions & actions = g_OilCrocks[m_CrockIndex].actions;	
	
	if( status.mouse_Pos != period && (status.curActionIndex <0 || status.curActionIndex >= actions.size()) ){
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		return ;
	}

	float temp;
	float leftLimit,rightLimit;
	float upLimit,downLimit;
	//转为逻辑坐标
	::ScreenToClient(g_actiongraph.GetHwnd(),&pt_client);
	Node pt=g_actiongraph.ClientToReal(pt_client);

	if(b_addLine){
		float tempX=pt.time;
		SetCursor(LoadCursor(NULL,IDC_CROSS));

		leftLimit = 0.0;
		rightLimit = actions.period;
		if( status.curActionIndex > 0 )
			leftLimit=actions[status.curActionIndex-1].endTime;
		if( status.curActionIndex < actions.size()-1 )
			rightLimit=actions[status.curActionIndex+1].startTime;

		if( pt.time < leftLimit ){
			tempX = leftLimit;
			SetCursor(LoadCursor(NULL,IDC_NO));
		}
		if( pt.time > rightLimit ){
			tempX = rightLimit ;
			SetCursor(LoadCursor(NULL,IDC_NO));
		}
		
		
		Action &action = actions[status.curActionIndex];
		if( tempX >= status.Last_Pos.time ){
			action.endTime = tempX;
			action.startTime=status.Last_Pos.time;
		}
		else if( tempX <= status.Last_Pos.time ){
			action.startTime = tempX;
			action.endTime=status.Last_Pos.time;
		}

	}else if(b_moveLine){//移动
		float destStart,destEnd;
		float destX =(float)pt.time;
		float destY =(float)pt.value;
		float sourceX=(float)status.Last_Pos.time;
		
		leftLimit=0;
		rightLimit=actions.period;
		upLimit = g_actiongraph.GetScale().value;
		downLimit = -g_actiongraph.GetScale().value;

		switch(status.mouse_Pos ){
		case period:
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			
			temp=pt.time;
			if( actions.size() > 0 )
				leftLimit=actions.back().endTime;
			rightLimit=g_actiongraph.GetScale().time;

			if(pt.time< leftLimit ){
				SetCursor(LoadCursor(NULL,IDC_NO));
				temp =leftLimit;
			}
			if(pt.time> rightLimit){
				SetCursor(LoadCursor(NULL,IDC_NO));
				temp =rightLimit;
			}

			actions.period = int(temp*10)/10.0f;
			m_strEnd=m_strStart=m_strVel=_T("");
			m_strPeriod.Format("%3.1f",actions.period);
			UpdateData(FALSE);
			break;
		case left:
			if(status.curActionIndex>0)
				leftLimit=actions[status.curActionIndex-1].endTime;
			rightLimit=actions[status.curActionIndex].endTime;

			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			if(destX<leftLimit ){
				destX=leftLimit;
				SetCursor(LoadCursor(NULL,IDC_NO));
			}
			if(destX>rightLimit){
				destX=rightLimit;
				SetCursor(LoadCursor(NULL,IDC_NO));
			}	
			actions[status.curActionIndex].startTime =destX;
			break;
		case right: 
			if(status.curActionIndex < actions.size()-1)
				rightLimit=actions[status.curActionIndex+1].startTime;
			leftLimit=actions[status.curActionIndex].startTime;
		
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			if(destX<leftLimit){
				destX=leftLimit;	
				SetCursor(LoadCursor(NULL,IDC_NO));
			}
			if(destX>rightLimit){
				destX=rightLimit;	
				SetCursor(LoadCursor(NULL,IDC_NO));
			}
			actions[status.curActionIndex].endTime =destX;
			break;
		case middle:
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			//确定移动范围
			if( status.curActionIndex > 0 ){
				leftLimit = actions[status.curActionIndex-1].endTime;
			}
			if( status.curActionIndex < actions.size()-1 ){
				rightLimit = actions[status.curActionIndex+1].startTime;
			}

			destStart= status.orig_action.startTime +destX- sourceX;
			destEnd = status.orig_action.endTime + destX - sourceX;

			//对超出移动范围的处理
			if(destX-leftLimit < sourceX-status.orig_action.startTime ){//从左边超出
				SetCursor(LoadCursor(NULL,IDC_NO));
				destStart=leftLimit;
				destEnd=leftLimit+status.orig_action.endTime-status.orig_action.startTime;
			}
			if( rightLimit-destX < status.orig_action.endTime-sourceX){//从右边超出
				SetCursor(LoadCursor(NULL,IDC_NO));
				destStart=rightLimit-status.orig_action.endTime+status.orig_action.startTime;
				destEnd=rightLimit;
			}
			if(fabs(destY) >g_actiongraph.GetScale().value){
				SetCursor(LoadCursor(NULL,IDC_NO));
				if( destY > 0 )
					destY = g_actiongraph.GetScale().value;
				else
					destY = -g_actiongraph.GetScale().value;
			}
			actions[status.curActionIndex].startTime = destStart;
			actions[status.curActionIndex].endTime = destEnd;
			actions[status.curActionIndex].vel =destY;
			break;
		default:
			return ;
		}
	}else if(b_modifyLine){
		return;
	}

	m_strStart.Format("%3.1f", actions[status.curActionIndex].startTime );
	m_strEnd.Format("%3.1f", actions[status.curActionIndex].endTime );
	m_strVel.Format("%3.1f" ,actions[status.curActionIndex].vel*1000 );
	m_strPeriod.Format("%3.1f",actions.period);
	UpdateData(FALSE);

	g_actiongraph.DrawActions(actions);
	OnSimulate();
}

void CFluidSystemDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDblClk(nFlags, point);
	CRect rect_Flux;
	::GetWindowRect(g_fluxgraph.GetHwnd(),&rect_Flux);
	CPoint pt_client(point);
	ClientToScreen(&pt_client);
	if(! rect_Flux.PtInRect(pt_client)){//点击位置不在picture控件上
		return;
	}
	if( m_CrockIndex <0 || m_CrockIndex >= g_OilCrocks.size() ) return ; //没有油缸对象
	
	if(p_zoomDlg){
		delete p_zoomDlg;
	}
	
	p_zoomDlg = new CZoomDlg;
	p_zoomDlg->Create(IDD_ZOOM_DIALOG,this);
	p_zoomDlg->ShowWindow(SW_SHOW);
	p_zoomDlg->p_dlg=this;

	HWND hwnd = p_zoomDlg->GetSafeHwnd();
	g_fluxgraph.Draw_Flux(m_OilCount,m_All_Flux,m_Pump_Flux,m_box_checked,m_boxpress_checked,m_all_checked,m_pump_checked,hwnd);

}

void CFluidSystemDlg::OnQuit() 
{
	DestroyWindow();
}

void CFluidSystemDlg::OnSimulate() 
{
	UpdateData(TRUE);
	if( g_OilCrocks.size() )
		m_OilCount = Simulation(g_OilCrocks,g_Box,g_Pump,m_Pump_Flux,m_All_Flux);
	g_fluxgraph.Draw_Flux(m_OilCount,m_All_Flux,m_Pump_Flux,m_box_checked,m_boxpress_checked,m_all_checked,m_pump_checked,g_fluxgraph.GetHwnd());
}

void CFluidSystemDlg::OnCheckAllflux() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnCheckPumpflux()
{
	OnSimulate();
}

void CFluidSystemDlg::OnCheckOilcount() 
{
	UpdateData(TRUE);
	if( m_box_checked ){
		m_boxpress_checked = FALSE;
		UpdateData(FALSE);
	}
	OnSimulate();
}

void CFluidSystemDlg::OnCheckBoxpress() 
{
	UpdateData(TRUE);
	if( m_boxpress_checked ){
		m_box_checked = FALSE;
		UpdateData(FALSE);
	}
	OnSimulate();
}

void CFluidSystemDlg::OnChangeVol() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnChangeMaxp() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnChangeMidp() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnChangeMinp() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnChangeAirp() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnChangeQuality() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnChangeRotate() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnChangeEffective() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnChangePumpCount() 
{
	OnSimulate();
}

void CFluidSystemDlg::OnSelchangeCrocklist() 
{
	UpdateData( TRUE );
	g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions);
}

void CFluidSystemDlg::OnClear() 
{
	if(  m_CrockIndex >=0 && m_CrockIndex < g_OilCrocks.size() ){
		g_OilCrocks[m_CrockIndex].actions.clear();
		g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions); 
		OnSimulate();
	}
}

void CFluidSystemDlg::OnCopy() 
{
	if( m_CrockIndex >=0 && m_CrockIndex < g_OilCrocks.size() ){
		m_shareActions = g_OilCrocks[m_CrockIndex].actions;	
	}
}

void CFluidSystemDlg::OnPaste() 
{
	if( m_CrockIndex >=0 &&  m_CrockIndex < g_OilCrocks.size() ){
		g_OilCrocks[m_CrockIndex].actions = m_shareActions;
		g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions); 
		OnSimulate();
	}
}

void CFluidSystemDlg::OnCut() 
{
	if( m_CrockIndex >=0 && m_CrockIndex < g_OilCrocks.size() ){
		m_shareActions = g_OilCrocks[m_CrockIndex].actions;	
		g_OilCrocks[m_CrockIndex].actions.clear();
		g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions); 
		OnSimulate();
	}
}

void CFluidSystemDlg::OnSave() 
{
	if( ! filename.IsEmpty() ){
		SaveExcelFile(filename);
	}
}

void CFluidSystemDlg::OnAddline() 
{
	b_addLine=TRUE;
	b_deleteLine=FALSE;
	b_moveLine=FALSE;
	b_modifyLine=FALSE;

	m_ctrlEnd.SetReadOnly( TRUE );
	m_ctrlStart.SetReadOnly( TRUE );
	m_ctrlPeriod.SetReadOnly( TRUE );
	m_ctrlVel.SetReadOnly( TRUE );
	
	status.curActionIndex=-2;
	m_strEnd=m_strStart=m_strVel=_T("");
	if(g_OilCrocks.size()>0)
		g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions);

	CDialog::UpdateData(FALSE);
}

void CFluidSystemDlg::OnMoveline() 
{
	b_addLine=FALSE;
	b_deleteLine=FALSE;
	b_moveLine=TRUE;
	b_modifyLine=FALSE;
	m_ctrlEnd.SetReadOnly( TRUE );
	m_ctrlStart.SetReadOnly( TRUE );
	m_ctrlPeriod.SetReadOnly( TRUE );
	m_ctrlVel.SetReadOnly( TRUE );

	status.curActionIndex=-2;
	m_strEnd=m_strStart=m_strVel=_T("");
	if(g_OilCrocks.size()>0)
		g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions);

	CDialog::UpdateData(FALSE);
}

void CFluidSystemDlg::OnDeleteline() 
{
	b_addLine=FALSE;
	b_deleteLine=TRUE;
	b_moveLine=FALSE;
	b_modifyLine=FALSE;
	m_ctrlEnd.SetReadOnly( TRUE );
	m_ctrlStart.SetReadOnly( TRUE );
	m_ctrlPeriod.SetReadOnly( TRUE );
	m_ctrlVel.SetReadOnly( TRUE );
	status.curActionIndex=-2;
	m_strEnd=m_strStart=m_strVel=_T("");
	if(g_OilCrocks.size()>0)
		g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions);
	CDialog::UpdateData(FALSE);
}
void CFluidSystemDlg::OnModifyline() 
{
	b_addLine=FALSE;
	b_deleteLine=FALSE;
	b_moveLine=FALSE;
	b_modifyLine=TRUE;
	m_ctrlEnd.SetReadOnly( TRUE );
	m_ctrlStart.SetReadOnly( TRUE );
	m_ctrlPeriod.SetReadOnly( TRUE );
	m_ctrlVel.SetReadOnly( TRUE );

	status.curActionIndex=-2;
	m_strEnd=m_strStart=m_strVel=_T("");
	if(g_OilCrocks.size()>0)
		g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions);
	CDialog::UpdateData(FALSE);
}

void CFluidSystemDlg::OnPrint() 
{
	PrintExcel(g_OilCrocks,g_Box,g_Pump);	
}

BOOL CFluidSystemDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	
	// TODO: Add your message handler code here and/or call default
	g_fluxgraph.Draw_Flux(m_OilCount,m_All_Flux,m_Pump_Flux,m_box_checked,m_boxpress_checked,m_all_checked,m_pump_checked,g_fluxgraph.GetHwnd());
	if(m_CrockIndex < g_OilCrocks.size() ){
		if(b_modifyLine)
			g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions,status.curActionIndex);
		else	
			g_actiongraph.DrawActions(g_OilCrocks[m_CrockIndex].actions);
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


/////////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog


CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZoomDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
	//{{AFX_MSG_MAP(CZoomDlg)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg message handlers

void CZoomDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	g_fluxgraph.Draw_Flux(p_dlg->m_OilCount,p_dlg->m_All_Flux,p_dlg->m_Pump_Flux,
		p_dlg->m_box_checked,p_dlg->m_boxpress_checked,p_dlg->m_all_checked,
		p_dlg->m_pump_checked,GetSafeHwnd());
}

void CZoomDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}

