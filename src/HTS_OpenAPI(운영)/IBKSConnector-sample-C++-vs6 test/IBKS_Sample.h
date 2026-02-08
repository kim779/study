// IBKS_Sample.h : main header file for the IBKS_SAMPLE application
//

#if !defined(AFX_IBKS_SAMPLE_H__6864AD57_8E8D_4FB8_8567_A9C6308EFBD2__INCLUDED_)
#define AFX_IBKS_SAMPLE_H__6864AD57_8E8D_4FB8_8567_A9C6308EFBD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIBKS_SampleApp:
// See IBKS_Sample.cpp for the implementation of this class
//

class CIBKS_SampleApp : public CWinApp
{
public:
	CIBKS_SampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIBKS_SampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIBKS_SampleApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IBKS_SAMPLE_H__6864AD57_8E8D_4FB8_8567_A9C6308EFBD2__INCLUDED_)
