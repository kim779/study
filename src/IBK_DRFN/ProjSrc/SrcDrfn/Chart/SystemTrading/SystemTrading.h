// SystemTrading.h : main header file for the SYSTEMTRADING application
//

#if !defined(AFX_SYSTEMTRADING_H__76D66C01_24CA_422D_A916_A9EACC375DEC__INCLUDED_)
#define AFX_SYSTEMTRADING_H__76D66C01_24CA_422D_A916_A9EACC375DEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingApp:
// See SystemTrading.cpp for the implementation of this class
//

class CSystemTradingApp : public CWinApp
{
public:
	CSystemTradingApp();

	CMultiDocTemplate* m_pDocTemplate;
	CMultiDocTemplate* m_pDocTemplateBrowserPage;

	void		SetRootPath();
	LPSTR		GetRootPath();
	LPSTR		GetUserPath();
	BOOL		GetDebugMode();
	void		VerifySystemTradingFolder();

	BOOL		FindSystemTrading();
	short		CopyBaseScriptFile();

private:
	char	m_szRootPath[MAX_PATH], m_szUserPath[MAX_PATH];
	CString m_szScriptFile;
	BOOL	m_bDebugCompile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSystemTradingApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnGotoURL(NMHDR* pNMHDR, LRESULT* pResult);
};

extern CSystemTradingApp theApp;

inline LPSTR CSystemTradingApp::GetRootPath()
{ return m_szRootPath; }

inline LPSTR CSystemTradingApp::GetUserPath()
{ return m_szUserPath; }

inline BOOL	CSystemTradingApp::GetDebugMode()
{ return m_bDebugCompile; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADING_H__76D66C01_24CA_422D_A916_A9EACC375DEC__INCLUDED_)
