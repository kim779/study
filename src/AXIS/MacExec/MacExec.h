// MacExec.h : main header file for the MACEXEC application
//

#if !defined(AFX_MACEXEC_H__7599ACA6_8529_4BF0_B6EC_9627270AF1FA__INCLUDED_)
#define AFX_MACEXEC_H__7599ACA6_8529_4BF0_B6EC_9627270AF1FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMacExecApp:
// See MacExec.cpp for the implementation of this class
//

class CMacExecApp : public CWinApp
{
public:
	CMacExecApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacExecApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMacExecApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACEXEC_H__7599ACA6_8529_4BF0_B6EC_9627270AF1FA__INCLUDED_)
