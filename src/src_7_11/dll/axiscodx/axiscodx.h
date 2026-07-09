// axiscodx.h : main header file for the AXISCODX DLL
//

#if !defined(AFX_AXISCODX_H__2874F5D6_4A7F_4D1F_9955_7AB15DC11669__INCLUDED_)
#define AFX_AXISCODX_H__2874F5D6_4A7F_4D1F_9955_7AB15DC11669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "code.h"

/////////////////////////////////////////////////////////////////////////////
// CAxiscodxApp
// See axiscodx.cpp for the implementation of this class
//

class CAxiscodxApp : public CWinApp
{
public:
	CAxiscodxApp();
	virtual	~CAxiscodxApp();

public:
	CCode*	m_code;
	CString	m_master;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxiscodxApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxiscodxApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISCODX_H__2874F5D6_4A7F_4D1F_9955_7AB15DC11669__INCLUDED_)
