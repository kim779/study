// CX_FDS.h : main header file for the CX_FDS DLL
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_FDSApp
// See CX_FDS.cpp for the implementation of this class
//

class CCX_FDSApp : public CWinApp
{
public:
	CCX_FDSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_FDSApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_FDSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};