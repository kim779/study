// axislogin.h : main header file for the AXISLOGIN DLL
//

#if !defined(AFX_AXISLOGIN_H__4FE143E6_F48D_40A6_B9D1_9E60EA05DA97__INCLUDED_)
#define AFX_AXISLOGIN_H__4FE143E6_F48D_40A6_B9D1_9E60EA05DA97__INCLUDED_

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
	CString	m_user;
	CString	m_name;
	CString	m_dept;
	CString	m_term;
	CString	m_whoi;
	CString	m_wrapwhoi;

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

#endif // !defined(AFX_AXISLOGIN_H__4FE143E6_F48D_40A6_B9D1_9E60EA05DA97__INCLUDED_)
