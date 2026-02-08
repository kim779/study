// ChartCommonAddIn.h : main header file for the CHARTCOMMONADDIN DLL
//

#if !defined(AFX_CHARTCOMMONADDIN_H__A0AEA811_B4DD_46E8_B3C0_996879675688__INCLUDED_)
#define AFX_CHARTCOMMONADDIN_H__A0AEA811_B4DD_46E8_B3C0_996879675688__INCLUDED_

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
// CChartCommonAddInApp
// See ChartCommonAddIn.cpp for the implementation of this class
//

// (2005/8/18 - Seung-Won, Bae) Define AddIn Event ID
typedef enum {	m_eAskingPriceValue = 0};

// (2008/2/15 - Seung-Won, Bae) for TEST of Multi-Language Version in OLD AUP.
#define _ML_TEST_NOT_USED()		AfxMessageBox( "I did not prepare this operation in Multi-Language Version.\r\nPlease, call chart developer. thanks.");

class CChartCommonAddInApp : public CWinApp
{
public:
	CChartCommonAddInApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartCommonAddInApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartCommonAddInApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTCOMMONADDIN_H__A0AEA811_B4DD_46E8_B3C0_996879675688__INCLUDED_)
