// IB871000.h : main header file for the IB871000 DLL
//


#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB871000App
// See IB871000.cpp for the implementation of this class
//

class CIB871000App : public CWinApp
{
public:
	CIB871000App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB871000App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB871000App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


