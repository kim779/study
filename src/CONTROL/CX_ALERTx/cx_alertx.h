// cx_alertx.h : main header file for the CX_ALERTX DLL
//

#if !defined(AFX_CX_ALERTX_H__238FB419_54D5_4A61_8811_2F786C90C6DC__INCLUDED_)
#define AFX_CX_ALERTX_H__238FB419_54D5_4A61_8811_2F786C90C6DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCx_alertxApp
// See cx_alertx.cpp for the implementation of this class
//

class CCx_alertxApp : public CWinApp
{
public:
	CCx_alertxApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCx_alertxApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCx_alertxApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_ALERTX_H__238FB419_54D5_4A61_8811_2F786C90C6DC__INCLUDED_)
