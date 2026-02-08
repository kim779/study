#if !defined(AFX_INPUTRENAMEDLG_H__85C4452E_CBBC_41CF_BD46_15680E0E13A9__INCLUDED_)
#define AFX_INPUTRENAMEDLG_H__85C4452E_CBBC_41CF_BD46_15680E0E13A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRenameDlg.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInputRenameDlg dialog

class AFX_EXT_CLASS CInputRenameDlg : public CDialog
{
public:
	CPoint	m_point;
// Construction
public:
	CInputRenameDlg(CWnd* pParent = NULL, CString sName = "");   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputRenameDlg)
	enum { IDD = IDD_ITEM_RENAME };
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRenameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRenameDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTRENAMEDLG_H__85C4452E_CBBC_41CF_BD46_15680E0E13A9__INCLUDED_)
