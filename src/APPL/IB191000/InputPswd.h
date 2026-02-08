#pragma once
// InputPswd.h : header file
//

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

