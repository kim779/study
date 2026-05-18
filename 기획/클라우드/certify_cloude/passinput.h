#if !defined(AFX_PASSINPUT_H__965683F0_491F_4367_9BC4_6C97F4EF39E2__INCLUDED_)
#define AFX_PASSINPUT_H__965683F0_491F_4367_9BC4_6C97F4EF39E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PassInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPassInput dialog

class CPassInput : public CDialog
{
// Construction
public:
	CPassInput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPassInput)
	enum { IDD = IDD_PASSIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	CString	m_pass;

private:
	CWnd*	m_parent;
	CString	m_msg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void	SetMessage(CString msg);

protected:
	// Generated message map functions
	//{{AFX_MSG(CPassInput)
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangePass();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSINPUT_H__965683F0_491F_4367_9BC4_6C97F4EF39E2__INCLUDED_)
