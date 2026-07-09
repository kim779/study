// CX_FILESEARCH.h : main header file for the CX_FILESEARCH DLL
//

#if !defined(AFX_CX_FILESEARCH_H__F151086A_1AE1_4A5D_A85E_D032E9D1CE05__INCLUDED_)
#define AFX_CX_FILESEARCH_H__F151086A_1AE1_4A5D_A85E_D032E9D1CE05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_FILESEARCHApp
// See CX_FILESEARCH.cpp for the implementation of this class
//

class CCX_FILESEARCHApp : public CWinApp
{
public:
	CCX_FILESEARCHApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_FILESEARCHApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_FILESEARCHApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_FILESEARCH_H__F151086A_1AE1_4A5D_A85E_D032E9D1CE05__INCLUDED_)
