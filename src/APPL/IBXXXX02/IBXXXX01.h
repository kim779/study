// IBXXXX01.h : main header file for the IBXXXX01 DLL
//

#if !defined(AFX_WNDPROJ_H__8328516F_EEC1_424F_9B29_EB2AD6B96BB4__INCLUDED_)
#define AFX_WNDPROJ_H__8328516F_EEC1_424F_9B29_EB2AD6B96BB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIBXXXX01App
// See IBXXXX01.cpp for the implementation of this class
//

class CIBXXXX01App : public CWinApp
{
public:
	CIBXXXX01App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIBXXXX01App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIBXXXX01App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDPROJ_H__8328516F_EEC1_424F_9B29_EB2AD6B96BB4__INCLUDED_)
