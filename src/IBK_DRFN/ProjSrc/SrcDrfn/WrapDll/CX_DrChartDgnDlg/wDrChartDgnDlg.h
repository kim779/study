// wDrChartDgnDlg.h : main header file for the WDRCHARTDGNDLG DLL
//

#if !defined(AFX_WDRCHARTDGNDLG_H__58625FC1_CBC8_4E08_8EB2_B8312C38F71D__INCLUDED_)
#define AFX_WDRCHARTDGNDLG_H__58625FC1_CBC8_4E08_8EB2_B8312C38F71D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWDrChartDgnDlgApp
// See wDrChartDgnDlg.cpp for the implementation of this class
//

class CWDrChartDgnDlgApp : public CWinApp
{
public:
	CWDrChartDgnDlgApp();

	BOOL DOChartOCXRegister();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWDrChartDgnDlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWDrChartDgnDlgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDRCHARTDGNDLG_H__58625FC1_CBC8_4E08_8EB2_B8312C38F71D__INCLUDED_)
