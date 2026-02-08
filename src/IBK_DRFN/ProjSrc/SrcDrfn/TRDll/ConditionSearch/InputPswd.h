#if !defined(AFX_INPUTPSWD_H__2B9C0DF2_D544_4479_BBA0_CF03E736EB9B__INCLUDED_)
#define AFX_INPUTPSWD_H__2B9C0DF2_D544_4479_BBA0_CF03E736EB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPswd.h : header file
//
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CInputPswd dialog

class CInputPswd : public CDialog
{
// Construction
public:
	CInputPswd(CWnd* pParent, CString account);   // standard constructor



// Dialog Data
	//{{AFX_DATA(CInputPswd)
	enum { IDD = IDD_DIALOGPW };
	CEdit	m_editPassword;
	CString	m_strPassword;
	CString	m_strAccount;
	//}}AFX_DATA

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputPswd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputPswd)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPSWD_H__2B9C0DF2_D544_4479_BBA0_CF03E736EB9B__INCLUDED_)
