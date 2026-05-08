// CX_NEWPHONEPAD.h : main header file for the CX_NEWPHONEPAD DLL
//

#if !defined(AFX_CX_NEWPHONEPAD_H__B251444A_F8D7_480F_9378_C7372280991E__INCLUDED_)
#define AFX_CX_NEWPHONEPAD_H__B251444A_F8D7_480F_9378_C7372280991E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_NEWPHONEPADApp
// See CX_NEWPHONEPAD.cpp for the implementation of this class
//

class CCX_NEWPHONEPADApp : public CWinApp
{
public:
	CCX_NEWPHONEPADApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_NEWPHONEPADApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_NEWPHONEPADApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_NEWPHONEPAD_H__B251444A_F8D7_480F_9378_C7372280991E__INCLUDED_)
