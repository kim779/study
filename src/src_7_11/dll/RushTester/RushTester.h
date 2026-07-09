// RushTester.h : main header file for the RUSHTESTER application
//

#if !defined(AFX_RUSHTESTER_H__D3C315E0_C84D_44B4_B6BE_F4947BE6B67D__INCLUDED_)
#define AFX_RUSHTESTER_H__D3C315E0_C84D_44B4_B6BE_F4947BE6B67D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRushTesterApp:
// See RushTester.cpp for the implementation of this class
//

class CRushTesterApp : public CWinApp
{
public:
	CRushTesterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRushTesterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRushTesterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUSHTESTER_H__D3C315E0_C84D_44B4_B6BE_F4947BE6B67D__INCLUDED_)
