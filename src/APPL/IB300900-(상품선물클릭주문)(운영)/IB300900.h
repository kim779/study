// IB191000.h : main header file for the IB191000 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "IB300900_i.h"

/////////////////////////////////////////////////////////////////////////////
// CIB191000App
// See IB191000.cpp for the implementation of this class
//

class CIB191000App : public CWinApp
{
public:
	CIB191000App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB191000App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB191000App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL InitATL();
};


