// CX_WEBEDGE.h : main header file for the CX_WEBEDGE DLL
//

#if !defined(AFX_CX_WEBEDGE_H__4C4F9D57_78EA_41AC_A90C_EAC14ACC87F9__INCLUDED_)
#define AFX_CX_WEBEDGE_H__4C4F9D57_78EA_41AC_A90C_EAC14ACC87F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_WEBEDGEApp
// See CX_WEBEDGE.cpp for the implementation of this class
//

class CCX_WEBEDGEApp : public CWinApp
{
public:
	CCX_WEBEDGEApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_WEBEDGEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_WEBEDGEApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_WEBEDGE_H__4C4F9D57_78EA_41AC_A90C_EAC14ACC87F9__INCLUDED_)
