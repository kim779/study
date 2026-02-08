// CX_AES.h : main header file for the CX_AES DLL
//

#if !defined(AFX_CX_AES_H__E3F0F1A5_E1A1_464B_BCD9_FF2B09757C10__INCLUDED_)
#define AFX_CX_AES_H__E3F0F1A5_E1A1_464B_BCD9_FF2B09757C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_AESApp
// See CX_AES.cpp for the implementation of this class
//

class CCX_AESApp : public CWinApp
{
public:
	CCX_AESApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_AESApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_AESApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_AES_H__E3F0F1A5_E1A1_464B_BCD9_FF2B09757C10__INCLUDED_)
