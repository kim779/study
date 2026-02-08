// CX_EDGE.h : main header file for the CX_EDGE DLL
//

#if !defined(AFX_CX_EDGE_H__84B9268A_FA7F_4A15_81D1_6A739F8000F5__INCLUDED_)
#define AFX_CX_EDGE_H__84B9268A_FA7F_4A15_81D1_6A739F8000F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_EDGEApp
// See CX_EDGE.cpp for the implementation of this class
//

class CCX_EDGEApp : public CWinApp
{
public:
	CCX_EDGEApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_EDGEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_EDGEApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_EDGE_H__84B9268A_FA7F_4A15_81D1_6A739F8000F5__INCLUDED_)
