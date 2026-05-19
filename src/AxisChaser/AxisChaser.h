// AxisChaser.h : main header file for the AXISCHASER application
//

#if !defined(AFX_AXISCHASER_H__E5549AEE_C010_4D32_B8E6_B2BB767344D6__INCLUDED_)
#define AFX_AXISCHASER_H__E5549AEE_C010_4D32_B8E6_B2BB767344D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAxisChaserApp:
// See AxisChaser.cpp for the implementation of this class
//

class CAxisChaserApp : public CWinApp
{
public:
	CAxisChaserApp();

	CString	m_class;
	CString	m_regkey;
	CString	m_TargetKey;
	char	m_flag;

	HANDLE	m_hMutex;

	bool	IsFirstInstance();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisChaserApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CAxisChaserApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//---------------------------------------------------------------------------
//
//	CCommParam
//
class CCommParam : public CCommandLineInfo
{
// Construction
public:
	CCommParam();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommParam)
	virtual void ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommParam();
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISCHASER_H__E5549AEE_C010_4D32_B8E6_B2BB767344D6__INCLUDED_)
