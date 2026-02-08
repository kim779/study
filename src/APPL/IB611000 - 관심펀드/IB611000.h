// IB611000.h : main header file for the IB611000 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"

/////////////////////////////////////////////////////////////////////////////
// CIB611000App
// See IB611000.cpp for the implementation of this class
//

const int maxGROUP = 100, maxJONGMOK = 100;

class CIB611000App : public CWinApp
{
public:
	CIB611000App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB611000App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB611000App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


