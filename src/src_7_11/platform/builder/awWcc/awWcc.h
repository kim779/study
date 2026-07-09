// awWcc.h : main header file for the AWWCC DLL
//

#if !defined(AFX_AWWCC_H__42825618_45FC_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_AWWCC_H__42825618_45FC_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "libwcc.h"
#include "compile.h"

/////////////////////////////////////////////////////////////////////////////
// CAwWccApp
// See awWcc.cpp for the implementation of this class
//

class CAwWccApp : public CWinApp
{
public:
	CAwWccApp();
	~CAwWccApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAwWccApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAwWccApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CCompile	m_compile;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AWWCC_H__42825618_45FC_11D4_A024_00001CD7F9BE__INCLUDED_)
