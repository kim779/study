// axiscodx.h : main header file for the AXISCODX DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "code.h"

/////////////////////////////////////////////////////////////////////////////
// CAxiscodxApp
// See axiscodx.cpp for the implementation of this class
//

class CAxiscodxApp : public CWinApp
{
public:
	CAxiscodxApp();

public:
	std::unique_ptr<CCode>	m_code;
	CString	m_master;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxiscodxApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxiscodxApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


