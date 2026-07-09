#if !defined(AFX_LOCKDLG_H__CF36B63E_561C_4447_9985_5F44191FD455__INCLUDED_)
#define AFX_LOCKDLG_H__CF36B63E_561C_4447_9985_5F44191FD455__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LockDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLockDlg dialog

class CLockDlg : public CDialog
{
// Construction
public:
	CLockDlg(CWnd* pParent = NULL, CString user = _T(""), CString pass = _T(""), CString certify = _T(""));  // standard constructor

// Dialog Data
	//{{AFX_DATA(CLockDlg)
	enum { IDD = IDD_LOCK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	CWnd	*m_pParent;
	CString	m_user;
	CString	m_pass;
	CString	m_certify;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLockDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLockDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCKDLG_H__CF36B63E_561C_4447_9985_5F44191FD455__INCLUDED_)
