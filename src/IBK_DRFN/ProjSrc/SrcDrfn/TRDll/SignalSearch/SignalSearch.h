// SignalSearch.h : main header file for the SIGNALSEARCH DLL
//

#if !defined(AFX_SIGNALSEARCH_H__49852E9D_E7AB_4200_85E4_0C88C5668223__INCLUDED_)
#define AFX_SIGNALSEARCH_H__49852E9D_E7AB_4200_85E4_0C88C5668223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSignalSearchApp
// See SignalSearch.cpp for the implementation of this class
//

class CSignalSearchApp : public CWinApp
{
public:
	CSignalSearchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSignalSearchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSignalSearchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern CSignalSearchApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNALSEARCH_H__49852E9D_E7AB_4200_85E4_0C88C5668223__INCLUDED_)
