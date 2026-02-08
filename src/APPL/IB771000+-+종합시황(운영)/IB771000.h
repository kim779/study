// IB771000.h : main header file for the IB771000 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols



/////////////////////////////////////////////////////////////////////////////
// CIB771000App
// See IB771000.cpp for the implementation of this class
//

class CIB771000App : public CWinApp
{
public:
	CIB771000App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB771000App)
	public:
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB771000App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


