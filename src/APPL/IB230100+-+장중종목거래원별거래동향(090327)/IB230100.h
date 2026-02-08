// IB230100.h : main header file for the IB230100 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB230100App
// See IB230100.cpp for the implementation of this class
//

class CIB230100App : public CWinApp
{
public:
	CIB230100App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB230100App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB230100App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


