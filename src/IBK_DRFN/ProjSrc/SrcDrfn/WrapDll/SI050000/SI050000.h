// SI050000.h : main header file for the SI050000 DLL
//

#if !defined(AFX_SI050000_H__8BBC8F9C_830A_4357_BDF9_13CCB0D43717__INCLUDED_)
#define AFX_SI050000_H__8BBC8F9C_830A_4357_BDF9_13CCB0D43717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSI050000App
// See SI050000.cpp for the implementation of this class
//

class CSI050000App : public CWinApp
{
public:
	CSI050000App();

	CString m_szDllName;
	void*		m_pWinixParam;
	CPCTrMngHelper  *m_pPcTrHelper;

	BOOL DOChartOCXRegister();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSI050000App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSI050000App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CSI050000App theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SI050000_H__8BBC8F9C_830A_4357_BDF9_13CCB0D43717__INCLUDED_)
