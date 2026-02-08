// C_UserGrpDlg.h : main header file for the C_UserGrpDlg DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CC_UserGrpDlgApp
// See C_UserGrpDlg.cpp for the implementation of this class
//

class CC_UserGrpDlgApp : public CWinApp
{
public:
	CC_UserGrpDlgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC_UserGrpDlgApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC_UserGrpDlgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};