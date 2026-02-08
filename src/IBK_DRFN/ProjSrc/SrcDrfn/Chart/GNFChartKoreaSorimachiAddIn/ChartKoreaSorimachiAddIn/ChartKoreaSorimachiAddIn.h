// ChartKoreaSorimachiAddIn.h : main header file for the ChartKoreaSorimachiAddIn DLL
//

#if !defined(AFX_ChartKoreaSorimachiAddIn_H__9308B31A_9CD9_43A7_AB1C_968DB993620C__INCLUDED_)
#define AFX_ChartKoreaSorimachiAddIn_H__9308B31A_9CD9_43A7_AB1C_968DB993620C__INCLUDED_

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
// CChartKoreaSorimachiAddInApp
// See ChartKoreaSorimachiAddIn.cpp for the implementation of this class
//

class CChartKoreaSorimachiAddInApp : public CWinApp
{
public:
	CChartKoreaSorimachiAddInApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartKoreaSorimachiAddInApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartKoreaSorimachiAddInApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ChartKoreaSorimachiAddIn_H__9308B31A_9CD9_43A7_AB1C_968DB993620C__INCLUDED_)
