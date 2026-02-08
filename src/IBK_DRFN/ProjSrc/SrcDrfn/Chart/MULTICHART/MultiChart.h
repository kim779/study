// MultiChart.h : main header file for the MULTICHART DLL
//

#if !defined(AFX_MULTICHART_H__F502D68B_6C49_470F_A99A_3BAF1250273D__INCLUDED_)
#define AFX_MULTICHART_H__F502D68B_6C49_470F_A99A_3BAF1250273D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMultiChartApp
// See MultiChart.cpp for the implementation of this class
//

class CMultiChartApp : public CWinApp
{
public:
	CMultiChartApp();
	HWND m_hPlatform;		//직접적으로는 사용하지 말고. CStdDialog의 m_hPlatform사용할 것.

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiChartApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMultiChartApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CMultiChartApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTICHART_H__F502D68B_6C49_470F_A99A_3BAF1250273D__INCLUDED_)
