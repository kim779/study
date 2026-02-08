// DrChartSet.h : main header file for the DRCHARTSET application
//

#if !defined(AFX_DRCHARTSET_H__A6EF8176_0F20_4039_8A68_6F07BE0B0A3D__INCLUDED_)
#define AFX_DRCHARTSET_H__A6EF8176_0F20_4039_8A68_6F07BE0B0A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDrChartSetApp:
// See DrChartSet.cpp for the implementation of this class
//

class CDrChartSetApp : public CWinApp
{
public:
	CDrChartSetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrChartSetApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDrChartSetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDrChartSetApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRCHARTSET_H__A6EF8176_0F20_4039_8A68_6F07BE0B0A3D__INCLUDED_)
