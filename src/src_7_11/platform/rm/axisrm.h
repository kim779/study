// axisrm.h : main header file for the AXISRM application
//

#if !defined(AFX_AXISRM_H__C29ACDAD_5A23_4E4A_BD46_5FED3AD097ED__INCLUDED_)
#define AFX_AXISRM_H__C29ACDAD_5A23_4E4A_BD46_5FED3AD097ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "transdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAxisrmApp:
// See axisrm.cpp for the implementation of this class
//

class CAxisrmApp : public CWinApp
{
public:
	CAxisrmApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisrmApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

public:
	int	m_mode;

// Implementation
protected:
	void	parsingCommandStatus();

	//{{AFX_MSG(CAxisrmApp)
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

#endif // !defined(AFX_AXISRM_H__C29ACDAD_5A23_4E4A_BD46_5FED3AD097ED__INCLUDED_)
