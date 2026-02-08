// IB876100.h : main header file for the IB876100 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
/////////////////////////////////////////////////////////////////////////////
// CIB876100App
// See IB876100.cpp for the implementation of this class
//

class CIB876100App : public CWinApp
{
public:
	CIB876100App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB876100App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB876100App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


