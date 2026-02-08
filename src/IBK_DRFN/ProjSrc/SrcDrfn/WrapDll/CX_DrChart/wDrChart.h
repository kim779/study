// wDrChart.h : main header file for the WDRCHART DLL
//

#if !defined(AFX_WDRCHART_H__4501F2FF_955B_42AC_B709_7B0288656D10__INCLUDED_)
#define AFX_WDRCHART_H__4501F2FF_955B_42AC_B709_7B0288656D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "../common/DrChartCtl.h"

/////////////////////////////////////////////////////////////////////////////
// CWDrChartApp
// See wDrChart.cpp for the implementation of this class
//

class CWDrChartApp : public CWinApp
{
public:
	CWDrChartApp();

	CPCTrMngHelper  *m_pPcTrHelper;

//	CDrChartCtl		*m_pChart;

	CWnd*		CreateChart(CWnd* pParent, _param* pInfo);
	BOOL		DOChartOCXRegister();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWDrChartApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWDrChartApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDRCHART_H__4501F2FF_955B_42AC_B709_7B0288656D10__INCLUDED_)
