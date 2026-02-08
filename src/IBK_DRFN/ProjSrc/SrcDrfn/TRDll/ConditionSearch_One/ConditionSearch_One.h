// ConditionSearch.h : main header file for the CONDITIONSEARCH DLL
//

#if !defined(AFX_CONDITIONSEARCH_ONE_H__B890D062_10C5_45CF_9463_5A34DA704764__INCLUDED_)
#define AFX_CONDITIONSEARCH_ONE_H__B890D062_10C5_45CF_9463_5A34DA704764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CConditionSearch_OneApp
// See ConditionSearch_One.cpp for the implementation of this class
//

class CConditionSearch_OneApp : public CWinApp
{
public:
	CConditionSearch_OneApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionSearchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CConditionSearchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern CConditionSearch_OneApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONSEARCH_ONE_H__B890D062_10C5_45CF_9463_5A34DA704764__INCLUDED_)
