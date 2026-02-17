#if !defined(AFX_COUNTPASS_H__2BB19F13_02B5_4C08_B94C_45617CEC079B__INCLUDED_)
#define AFX_COUNTPASS_H__2BB19F13_02B5_4C08_B94C_45617CEC079B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// countPass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCountPass dialog

class CCountPass : public CDialog
{
// Construction
public:
	CCountPass(int count, bool retry, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCountPass)
	enum { IDD = IDD_COUNT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private:
	int	m_count;
	bool	m_retry;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCountPass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCountPass)
	virtual BOOL OnInitDialog();
	afx_msg void OnIssue();
	afx_msg void OnOther();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COUNTPASS_H__2BB19F13_02B5_4C08_B94C_45617CEC079B__INCLUDED_)
