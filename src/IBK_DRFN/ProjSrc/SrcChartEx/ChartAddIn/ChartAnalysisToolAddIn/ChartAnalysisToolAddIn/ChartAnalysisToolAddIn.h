// ChartAnalysisToolAddIn.h : main header file for the CHARTANALYSISTOOLADDIN DLL
//

#if !defined(AFX_CHARTANALYSISTOOLADDIN_H__C705850A_807A_45B4_8571_ED7BB7F0C65C__INCLUDED_)
#define AFX_CHARTANALYSISTOOLADDIN_H__C705850A_807A_45B4_8571_ED7BB7F0C65C__INCLUDED_

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
// CChartAnalysisToolAddInApp
// See ChartAnalysisToolAddIn.cpp for the implementation of this class
//

class CChartAnalysisToolAddInApp : public CWinApp
{
public:
	CChartAnalysisToolAddInApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartAnalysisToolAddInApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartAnalysisToolAddInApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTANALYSISTOOLADDIN_H__C705850A_807A_45B4_8571_ED7BB7F0C65C__INCLUDED_)
