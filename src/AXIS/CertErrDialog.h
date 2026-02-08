#if !defined(AFX_CERTERRDIALOG_H__35413EEB_A5C5_401D_BA0A_7429C9E07E59__INCLUDED_)
#define AFX_CERTERRDIALOG_H__35413EEB_A5C5_401D_BA0A_7429C9E07E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CertErrDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCertErrDialog dialog

class CCertErrDialog : public CDialog
{
// Construction
public:
	CCertErrDialog(CWnd* pParent = NULL);   // standard constructor

public:
	void SetErrCount(CString cnt);

// Dialog Data
	//{{AFX_DATA(CCertErrDialog)
	enum { IDD = IDD_DLG_CERTERR };
	CString	m_strCnt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCertErrDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCertErrDialog)
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CERTERRDIALOG_H__35413EEB_A5C5_401D_BA0A_7429C9E07E59__INCLUDED_)
