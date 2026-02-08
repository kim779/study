// IB200200.h : main header file for the IB200200 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

//
//	control event
//
#define	WM_TOPARENT		(WM_USER + 16)

//
//	wparam
//
#define	cpCATEGROY	0x99

//
//	control transaction key
//
const int categoryKEY	= 110;

/////////////////////////////////////////////////////////////////////////////
// CIB200200App
// See IB200200.cpp for the implementation of this class
//

class CIB200200App : public CWinApp
{
public:
	CIB200200App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB200200App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB200200App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


