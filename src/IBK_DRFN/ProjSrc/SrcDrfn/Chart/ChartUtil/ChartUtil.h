// ChartUtil.h : main header file for the CHARTUTIL DLL
//

#if !defined(AFX_CHARTUTIL_H__7D6DC965_700B_4F0C_8962_4EE21A4DDDF8__INCLUDED_)
#define AFX_CHARTUTIL_H__7D6DC965_700B_4F0C_8962_4EE21A4DDDF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartUtilApp
// See ChartUtil.cpp for the implementation of this class
//

class CChartUtilApp : public CWinApp
{
public:
	CChartUtilApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartUtilApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartUtilApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTUTIL_H__7D6DC965_700B_4F0C_8962_4EE21A4DDDF8__INCLUDED_)
