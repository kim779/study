// axisdialog.h : main header file for the AXISDIALOG DLL
//

#if !defined(AFX_AXISDIALOG_H__543EA56D_34B8_4223_8743_5CF6F986D71A__INCLUDED_)
#define AFX_AXISDIALOG_H__543EA56D_34B8_4223_8743_5CF6F986D71A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAxisdialogApp
// See axisdialog.cpp for the implementation of this class
//

class CAxisdialogApp : public CWinApp
{
public:
	CAxisdialogApp();

	CString	m_returns;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisdialogApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisdialogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISDIALOG_H__543EA56D_34B8_4223_8743_5CF6F986D71A__INCLUDED_)
