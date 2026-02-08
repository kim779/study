// axislogin.h : main header file for the AXISLOGIN DLL
//

#if !defined(AFX_AXISLOGIN_H__51B9A03F_733D_489F_9194_5473E6B4CA57__INCLUDED_)
#define AFX_AXISLOGIN_H__51B9A03F_733D_489F_9194_5473E6B4CA57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "login.h"

/////////////////////////////////////////////////////////////////////////////
// CAxisloginApp
// See axislogin.cpp for the implementation of this class
//

class CAxisloginApp : public CWinApp
{
public:
	CAxisloginApp();
	virtual	~CAxisloginApp();

	CLogin*	m_login;
	CString	m_host;
	CString	m_user;
	CString	m_term;
	CString	m_brno;
	CString	m_cogb;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisloginApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisloginApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISLOGIN_H__51B9A03F_733D_489F_9194_5473E6B4CA57__INCLUDED_)
