// CX_SimpleAuth.h : main header file for the CX_SIMPLEAUTH DLL
//

#if !defined(AFX_CX_SIMPLEAUTH_H__14388178_0FC0_4FD5_9A2B_AB3E27F66A63__INCLUDED_)
#define AFX_CX_SIMPLEAUTH_H__14388178_0FC0_4FD5_9A2B_AB3E27F66A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_SimpleAuthApp
// See CX_SimpleAuth.cpp for the implementation of this class
//

class CCX_SimpleAuthApp : public CWinApp
{
public:
	CCX_SimpleAuthApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_SimpleAuthApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_SimpleAuthApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_SIMPLEAUTH_H__14388178_0FC0_4FD5_9A2B_AB3E27F66A63__INCLUDED_)
