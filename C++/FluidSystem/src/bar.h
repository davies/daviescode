#ifndef __DLGBARS_H__
#define __DLGBARS_H__

class CDlgToolBar : public CToolBar
{
// Construction
public:
	CDlgToolBar();

// Implementation
public:
	virtual ~CDlgToolBar();

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgToolBar)
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CDlgStatusBar : public CStatusBar
{
// Construction
public:
	CDlgStatusBar();

// Implementation
public:
	virtual ~CDlgStatusBar();

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgStatusBar)
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



#endif //__DLGBARS_H__
