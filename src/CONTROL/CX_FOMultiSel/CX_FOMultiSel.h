// CX_FOMultiSel.h : main header file for the CX_FOMULTISEL DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_FOMultiSelApp
// See CX_FOMultiSel.cpp for the implementation of this class
//

class CCX_FOMultiSelApp : public CWinApp
{
public:
	CCX_FOMultiSelApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_FOMultiSelApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_FOMultiSelApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};