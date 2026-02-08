// PortAnalyst.h : main header file for the PORTANALYST DLL
//

#if !defined(AFX_PORTANALYST_H__23CBD577_9B98_43E7_8F8B_520454F4B69F__INCLUDED_)
#define AFX_PORTANALYST_H__23CBD577_9B98_43E7_8F8B_520454F4B69F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPortAnalystApp
// See PortAnalyst.cpp for the implementation of this class
//

class CPortAnalystApp : public CWinApp
{
public:
	CPortAnalystApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortAnalystApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPortAnalystApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTANALYST_H__23CBD577_9B98_43E7_8F8B_520454F4B69F__INCLUDED_)
