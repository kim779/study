// IB100000.h : main header file for the IB100000 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB100000App
// See IB100000.cpp for the implementation of this class
//

class CIB100000App : public CWinApp
{
public:
	CIB100000App();
	CWnd*	loadMap(CWnd *parent);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB100000App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB100000App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


