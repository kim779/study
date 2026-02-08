// cx_libctrl.h : main header file for the CX_LIBCTRL DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"

/////////////////////////////////////////////////////////////////////////////
// CCx_libctrlApp
// See cx_libctrl.cpp for the implementation of this class
//

class CCx_libctrlApp : public CWinApp
{
public:
	CCx_libctrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCx_libctrlApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCx_libctrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};