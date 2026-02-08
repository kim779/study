// ChartSectionSearch.h : main header file for the ChartSectionSearch DLL
//

#if !defined(AFX_ChartSectionSearch_H__DB8E51CD_6563_431A_B11E_13708FDEA3B6__INCLUDED_)
#define AFX_ChartSectionSearch_H__DB8E51CD_6563_431A_B11E_13708FDEA3B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartSectionSearchApp
// See ChartSectionSearch.cpp for the implementation of this class
//

class CChartSectionSearchApp : public CWinApp
{
public:
	CChartSectionSearchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartSectionSearchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartSectionSearchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern CChartSectionSearchApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ChartSectionSearch_H__DB8E51CD_6563_431A_B11E_13708FDEA3B6__INCLUDED_)
