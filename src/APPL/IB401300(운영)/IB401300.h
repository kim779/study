// IB401300.h : main header file for the IB401300 DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB401300App
// See IB401300.cpp for the implementation of this class
//

class CIB401300App : public CWinApp
{
public:
	CIB401300App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB401300App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB401300App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


