// FluidSystemDlg.h : header file
//

#if !defined(AFX_FLUIDSYSTEMDLG_H__2726190C_3749_4CDB_B44A_A993CF436EF0__INCLUDED_)
#define AFX_FLUIDSYSTEMDLG_H__2726190C_3749_4CDB_B44A_A993CF436EF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "graph.h"
#include "simu.h"
/////////////////////////////////////////////////////////////////////////////
// CFluidSystemDlg dialog
class CZoomDlg;
class CFluidSystemDlg : public CDialog
{
// Construction
public:
	CZoomDlg    *p_zoomDlg;
	OilCount	m_OilCount;
	Flux		m_All_Flux;
	Flux		m_Pump_Flux;
	bool		m_changed;

	Status status;
	CFluidSystemDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFluidSystemDlg)
	enum { IDD = IDD_FLUIDSYSTEM_DIALOG };
	CEdit	m_ctrlPeriod;
	CEdit	m_ctrlStart;
	CEdit	m_ctrlEnd;
	CEdit	m_ctrlVel;
	CListBox	m_CrockList;
	BOOL	m_all_checked;
	BOOL	m_pump_checked;
	float	m_effective;
	float	m_maxp;
	float	m_midp;
	float	m_minp;
	int		m_pump_count;
	float	m_quality;
	float	m_rotate;
	float	m_vol;
	float	m_airp;
	int		m_CrockIndex;
	BOOL	b_addLine;
	BOOL	b_deleteLine;
	BOOL	b_moveLine;
	BOOL	m_box_checked;
	BOOL	m_boxpress_checked;
	CString	m_strStart;
	CString	m_strEnd;
	CString	m_strVel;
	CString	m_strPeriod;
	BOOL	b_modifyLine;
	CString	m_msg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFluidSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CToolBar m_toolbar;
	Actions	m_shareActions;
	CString filename;

	void CFluidSystemDlg::UpdateData(BOOL bSaveAndValidate);

	// Generated message map functions
	//{{AFX_MSG(CFluidSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnOK();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnQuit();
	afx_msg void OnSimulate();
	afx_msg void OnCheckAllflux();
	afx_msg void OnCheckPumpflux();
	afx_msg void OnCheckOilcount();
	afx_msg void OnChangeVol();
	afx_msg void OnChangeMaxp();
	afx_msg void OnChangeMidp();
	afx_msg void OnChangeMinp();
	afx_msg void OnChangeAirp();
	afx_msg void OnChangeQuality();
	afx_msg void OnChangeRotate();
	afx_msg void OnChangeEffective();
	afx_msg void OnChangePumpCount();
	afx_msg void OnSelchangeCrocklist();
	afx_msg void OnClear();
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnCut();
	afx_msg void OnSave();
	afx_msg void OnAddline();
	afx_msg void OnMoveline();
	afx_msg void OnDeleteline();
	afx_msg void OnCheckBoxpress();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPrint();
	afx_msg void OnModifyline();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


// ZoomDlg.h : header file
class CZoomDlg : public CDialog
{
// Construction
public:
	CFluidSystemDlg * p_dlg;
	CZoomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CZoomDlg)
	enum { IDD = IDD_ZOOM_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZoomDlg)
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLUIDSYSTEMDLG_H__2726190C_3749_4CDB_B44A_A993CF436EF0__INCLUDED_)
