// ChartGuideLineAddIn.h : main header file for the CHARTGUIDELINEADDIN DLL
//

#if !defined(AFX_CHARTGUIDELINEADDIN_H__2A3133F2_7DEF_4E90_8262_2AE2BD3614BA__INCLUDED_)
#define AFX_CHARTGUIDELINEADDIN_H__2A3133F2_7DEF_4E90_8262_2AE2BD3614BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define ADDIN_API extern "C" __declspec(dllexport)

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartGuideLineAddInApp
// See ChartGuideLineAddIn.cpp for the implementation of this class
//

class CChartGuideLineAddInApp : public CWinApp
{
public:
	CChartGuideLineAddInApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartGuideLineAddInApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartGuideLineAddInApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTGUIDELINEADDIN_H__2A3133F2_7DEF_4E90_8262_2AE2BD3614BA__INCLUDED_)
