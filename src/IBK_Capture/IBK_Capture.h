// IBK_Capture.h : main header file for the IBK_CAPTURE application
//

#if !defined(AFX_IBK_CAPTURE_H__AE11F57B_E381_4EF5_A91A_CFDA61BF6491__INCLUDED_)
#define AFX_IBK_CAPTURE_H__AE11F57B_E381_4EF5_A91A_CFDA61BF6491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIBK_CaptureApp:
// See IBK_Capture.cpp for the implementation of this class
//

class CIBK_CaptureApp : public CWinApp
{
public:
	CIBK_CaptureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIBK_CaptureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIBK_CaptureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IBK_CAPTURE_H__AE11F57B_E381_4EF5_A91A_CFDA61BF6491__INCLUDED_)
