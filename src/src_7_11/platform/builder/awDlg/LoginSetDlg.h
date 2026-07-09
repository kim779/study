#if !defined(AFX_LOGINSETDLG_H__F7F82A70_C351_40A5_8D0E_138D6D9C2B60__INCLUDED_)
#define AFX_LOGINSETDLG_H__F7F82A70_C351_40A5_8D0E_138D6D9C2B60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginSetDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLoginSetDlg dialog

class CLoginSetDlg : public CDialog
{
// Construction
public:
	void getAddress(BYTE &field0, BYTE &field1, BYTE &field2, BYTE &field3);
	CLoginSetDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CString		m_ips, m_ProjectName, m_currentPath;
public:


// Dialog Data
	//{{AFX_DATA(CLoginSetDlg)
	enum { IDD = IDD_LOGINSETDLG };
	CIPAddressCtrl	m_addr;
	CString	m_sProjectName;
	int		m_nPort;
	BOOL	m_bFloat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginSetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINSETDLG_H__F7F82A70_C351_40A5_8D0E_138D6D9C2B60__INCLUDED_)
