// IB110600.h : main header file for the IB110600 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB110600App
// See IB110600.cpp for the implementation of this class
//

class CIB110600App : public CWinApp
{
public:
	CIB110600App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB110600App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB110600App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


