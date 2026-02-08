// ChartPatternAddin.h : main header file for the CHARTPATTERNADDIN DLL
//

#if !defined(AFX_CHARTPATTERNADDIN_H__EFBA62D2_F08F_48E8_9C6E_5DBA7A33500D__INCLUDED_)
#define AFX_CHARTPATTERNADDIN_H__EFBA62D2_F08F_48E8_9C6E_5DBA7A33500D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


// (2004.10.07, 배승원) AddIn DLL의 Interface를 정의하기 위한 Export Type을 정의한다.
//#ifdef ADDIN_EXPORTS
#define ADDIN_API extern "C" __declspec(dllexport)
//#else
//#define ADDIN_API extern "C" __declspec(dllimport)
//#endif


#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartPatternAddinApp
// See ChartPatternAddin.cpp for the implementation of this class
//

class CChartPatternAddinApp : public CWinApp
{
public:
	CChartPatternAddinApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartPatternAddinApp)
	public:
		virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartPatternAddinApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTPATTERNADDIN_H__EFBA62D2_F08F_48E8_9C6E_5DBA7A33500D__INCLUDED_)
