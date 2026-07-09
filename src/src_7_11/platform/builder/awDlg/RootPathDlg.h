#if !defined(AFX_ROOTPATHDLG_H__8E05235B_3EF6_4D23_BC56_487C5B5B0D6D__INCLUDED_)
#define AFX_ROOTPATHDLG_H__8E05235B_3EF6_4D23_BC56_487C5B5B0D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RootPathDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CRootPathDlg dialog

class AFX_EXT_CLASS CRootPathDlg : public CDialog
{
// Construction
public:
	CRootPathDlg(CWnd* pParent, CString path);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CRootPathDlg)
	enum { IDD = IDD_ROOTPATH };
	CString	m_path;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRootPathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRootPathDlg)
	afx_msg void OnBrowser();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROOTPATHDLG_H__8E05235B_3EF6_4D23_BC56_487C5B5B0D6D__INCLUDED_)
