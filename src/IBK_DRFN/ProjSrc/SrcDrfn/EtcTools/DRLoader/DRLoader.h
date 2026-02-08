// DRLoader.h : main header file for the DRLOADER application
//

#if !defined(AFX_DRLOADER_H__78B65671_776D_4A9D_9DAC_E7E2E6FFAB87__INCLUDED_)
#define AFX_DRLOADER_H__78B65671_776D_4A9D_9DAC_E7E2E6FFAB87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDRLoaderApp:
// See DRLoader.cpp for the implementation of this class
//

class CDRLoaderApp : public CWinApp
{
public:
	CDRLoaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDRLoaderApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	//{{AFX_MSG(CDRLoaderApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnButton32772();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRLOADER_H__78B65671_776D_4A9D_9DAC_E7E2E6FFAB87__INCLUDED_)
