// IB200300.h : main header file for the IB200300 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define	ACNT_MSG	WM_USER+1
	#define	AM_ACNTCHANGE	100

/////////////////////////////////////////////////////////////////////////////
// CIB200300App
// See IB200300.cpp for the implementation of this class
//

class CIB200300App : public CWinApp
{
public:
	CIB200300App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB200300App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB200300App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


