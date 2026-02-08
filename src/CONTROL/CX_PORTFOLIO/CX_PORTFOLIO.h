// CX_PORTFOLIO.h : main header file for the CX_PORTFOLIO DLL
//

#if !defined(AFX_CX_PORTFOLIO_H__3C3A2F13_44F7_4670_8FD0_914837220605__INCLUDED_)
#define AFX_CX_PORTFOLIO_H__3C3A2F13_44F7_4670_8FD0_914837220605__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_PORTFOLIOApp
// See CX_PORTFOLIO.cpp for the implementation of this class
//

class CCX_PORTFOLIOApp : public CWinApp
{
public:
	CCX_PORTFOLIOApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_PORTFOLIOApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_PORTFOLIOApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_PORTFOLIO_H__3C3A2F13_44F7_4670_8FD0_914837220605__INCLUDED_)
