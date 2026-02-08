// ChartLanguageTable.h : main header file for the CHARTLANGUAGETABLE DLL
//

#if !defined(AFX_CHARTLANGUAGETABLE_H__FD74A1D2_D356_48AC_9685_52ECEF92EECF__INCLUDED_)
#define AFX_CHARTLANGUAGETABLE_H__FD74A1D2_D356_48AC_9685_52ECEF92EECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartLanguageTableApp
// See ChartLanguageTable.cpp for the implementation of this class
//

class CChartLanguageTableApp : public CWinApp
{
public:
	CChartLanguageTableApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartLanguageTableApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartLanguageTableApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CMetaTable;
extern CMetaTable g_metaTable;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTLANGUAGETABLE_H__FD74A1D2_D356_48AC_9685_52ECEF92EECF__INCLUDED_)
