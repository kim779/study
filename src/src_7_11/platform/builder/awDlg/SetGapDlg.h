#if !defined(AFX_SETGAPDLG_H__19C79C77_7DC1_4052_8288_5DFDC0147C90__INCLUDED_)
#define AFX_SETGAPDLG_H__19C79C77_7DC1_4052_8288_5DFDC0147C90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetGapDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSetGapDlg dialog

class AFX_EXT_CLASS CSetGapDlg : public CDialog
{
// Construction
public:
	CSetGapDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetGapDlg)
	enum { IDD = IDD_SETGAP };
	int		m_nGap;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetGapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetGapDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETGAPDLG_H__19C79C77_7DC1_4052_8288_5DFDC0147C90__INCLUDED_)
