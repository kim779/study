#if !defined(AFX_SAVEDLG_H__BEE53900_7B3A_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_SAVEDLG_H__BEE53900_7B3A_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// saveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveDlg dialog

#include "resource.h"

class AFX_EXT_CLASS CSaveDlg : public CDialog
{
// Construction
public:
	CSaveDlg(CWnd* pParent, CString path);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveDlg)
	enum { IDD = IDD_PATH };
	CString	m_path;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEDLG_H__BEE53900_7B3A_11D4_A024_00001CD7F9BE__INCLUDED_)
