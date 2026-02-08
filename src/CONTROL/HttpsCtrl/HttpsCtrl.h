// HttpsCtrl.h : main header file for the HttpsCtrl DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHttpsCtrlApp
// See HttpsCtrl.cpp for the implementation of this class
//

class CHttpsCtrlApp : public CWinApp
{
public:
	CHttpsCtrlApp();

	CString	GetRoot(gsl::not_null<CWnd*> pWizard);
	CString	GetName(gsl::not_null<CWnd*> pWizard);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpsCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHttpsCtrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


