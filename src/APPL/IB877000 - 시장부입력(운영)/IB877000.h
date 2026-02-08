// IB877000.h : main header file for the IB877000 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB877000App
// See IB877000.cpp for the implementation of this class
//

class CIB877000App : public CWinApp
{
public:
	CIB877000App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB877000App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB877000App)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


