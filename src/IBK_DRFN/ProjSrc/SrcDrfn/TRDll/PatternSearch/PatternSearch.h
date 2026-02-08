// PatternSearch.h : main header file for the PATTERNSEARCH DLL
//

#if !defined(AFX_PATTERNSEARCH_H__64DD3376_1D3F_46DF_B37F_AC2B4E9F84A9__INCLUDED_)
#define AFX_PATTERNSEARCH_H__64DD3376_1D3F_46DF_B37F_AC2B4E9F84A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPatternSearchApp
// See PatternSearch.cpp for the implementation of this class
//

class CPatternSearchApp : public CWinApp
{
public:
	CPatternSearchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatternSearchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPatternSearchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERNSEARCH_H__64DD3376_1D3F_46DF_B37F_AC2B4E9F84A9__INCLUDED_)
