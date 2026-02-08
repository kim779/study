// CX_MARKETMAP.h : main header file for the CX_MARKETMAP DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#include "../../h/axiscgrp.h"

/////////////////////////////////////////////////////////////////////////////
// CCX_MARKETMAPApp
// See CX_MARKETMAP.cpp for the implementation of this class
//

class CCX_MARKETMAPApp : public CWinApp
{
public:
	CCX_MARKETMAPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_MARKETMAPApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_MARKETMAPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};