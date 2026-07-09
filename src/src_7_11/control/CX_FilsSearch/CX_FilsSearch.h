// CX_FilsSearch.h : main header file for the CX_FILSSEARCH DLL
//

#if !defined(AFX_CX_FILSSEARCH_H__6F95BAA3_9ACC_4760_B148_837859691F9D__INCLUDED_)
#define AFX_CX_FILSSEARCH_H__6F95BAA3_9ACC_4760_B148_837859691F9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_FilsSearchApp
// See CX_FilsSearch.cpp for the implementation of this class
//

class CCX_FilsSearchApp : public CWinApp
{
public:
	CCX_FilsSearchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_FilsSearchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_FilsSearchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_FILSSEARCH_H__6F95BAA3_9ACC_4760_B148_837859691F9D__INCLUDED_)
