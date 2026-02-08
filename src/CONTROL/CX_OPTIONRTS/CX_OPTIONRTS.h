// CX_OPTIONRTS.h : main header file for the CX_OPTIONRTS DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_OPTIONRTSApp
// See CX_OPTIONRTS.cpp for the implementation of this class
//

class CCX_OPTIONRTSApp : public CWinApp
{
public:
	CCX_OPTIONRTSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_OPTIONRTSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_OPTIONRTSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};