// FluidSystem.h : main header file for the FLUIDSYSTEM application
//

#if !defined(AFX_FLUIDSYSTEM_H__AB31DD2E_1B8E_48CC_BD9B_EBECC729EB15__INCLUDED_)
#define AFX_FLUIDSYSTEM_H__AB31DD2E_1B8E_48CC_BD9B_EBECC729EB15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFluidSystemApp:
// See FluidSystem.cpp for the implementation of this class
//

class CFluidSystemApp : public CWinApp
{
public:
	CFluidSystemApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFluidSystemApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFluidSystemApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLUIDSYSTEM_H__AB31DD2E_1B8E_48CC_BD9B_EBECC729EB15__INCLUDED_)
