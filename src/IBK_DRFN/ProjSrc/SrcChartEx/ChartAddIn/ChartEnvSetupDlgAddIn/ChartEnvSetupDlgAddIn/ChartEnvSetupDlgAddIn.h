// ChartEnvSetupDlgAddIn.h : main header file for the CHARTENVSETUPDLGADDIN DLL
//

#if !defined(AFX_CHARTENVSETUPDLGADDIN_H__B4542F93_C389_4A48_9C87_A27812346FE6__INCLUDED_)
#define AFX_CHARTENVSETUPDLGADDIN_H__B4542F93_C389_4A48_9C87_A27812346FE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// (2004.10.07, 배승원) AddIn DLL의 Interface를 정의하기 위한 Export Type을 정의한다.
#ifdef ADDIN_EXPORTS
#define ADDIN_API extern "C" __declspec(dllexport)
#else
#define ADDIN_API extern "C" __declspec(dllimport)
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartEnvSetupDlgAddInApp
// See ChartEnvSetupDlgAddIn.cpp for the implementation of this class
//

class CChartEnvSetupDlgAddInApp : public CWinApp
{
public:
	CChartEnvSetupDlgAddInApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartEnvSetupDlgAddInApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartEnvSetupDlgAddInApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

ADDIN_API int GetAddInIVersion(void);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTENVSETUPDLGADDIN_H__B4542F93_C389_4A48_9C87_A27812346FE6__INCLUDED_)
