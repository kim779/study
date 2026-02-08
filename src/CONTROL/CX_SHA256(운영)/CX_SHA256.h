// CX_SHA256.h : main header file for the CX_SHA256 DLL
//

#if !defined(AFX_CX_SHA256_H__3103744E_2D05_40F1_A4B9_A48C5146C64C__INCLUDED_)
#define AFX_CX_SHA256_H__3103744E_2D05_40F1_A4B9_A48C5146C64C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_SHA256App
// See CX_SHA256.cpp for the implementation of this class
//

class CCX_SHA256App : public CWinApp
{
public:
	CCX_SHA256App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_SHA256App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_SHA256App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_SHA256_H__3103744E_2D05_40F1_A4B9_A48C5146C64C__INCLUDED_)
