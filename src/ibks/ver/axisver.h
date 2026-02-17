// axisver.h : main header file for the AXISVER application
//

#if !defined(AFX_AXISVER_H__EA07ACDF_1464_46E1_AA98_67E077CE525A__INCLUDED_)
#define AFX_AXISVER_H__EA07ACDF_1464_46E1_AA98_67E077CE525A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAxisverApp:
// See axisver.cpp for the implementation of this class
//

class CAxisverApp : public CWinApp
{
public:
	CAxisverApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisverApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

public:
	enum class argACT {argNONE, argMANAGER, argUSER, argRETRY, argMOVE, argCLASS, argAPNAME, argDIR, argRSV} m_argument;
	CString	m_user;
	CString	m_pass;
	CString	m_root;
	CString	m_apname;
	CString	m_keys;
	CString	m_params;
	HWND	m_axis;

	CStringArray m_files;

// Implementation
protected:
	void	parsingCommandStatus();
	void	updateObjectAXIS();
	void	retryAXIS();
	BOOL	getDownDirFileName();

	//{{AFX_MSG(CAxisverApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//---------------------------------------------------------------------------
//
//	CCommLine
//
class CCommLine : public CCommandLineInfo
{
// Construction
public:
	CCommLine();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommLine)
	virtual void ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommLine();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISVER_H__EA07ACDF_1464_46E1_AA98_67E077CE525A__INCLUDED_)
