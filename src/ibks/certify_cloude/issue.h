#if !defined(AFX_ISSUE_H__E822BE1F_C8E5_40B7_B8AA_80F6CE2AA201__INCLUDED_)
#define AFX_ISSUE_H__E822BE1F_C8E5_40B7_B8AA_80F6CE2AA201__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// issue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIssue dialog

class CIssue : public CDialog
{
// Construction
public:
	CIssue(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIssue)
	enum { IDD = IDD_ISSUE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIssue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIssue)
	virtual BOOL OnInitDialog();
	afx_msg void OnIssue();
	afx_msg void OnOther();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISSUE_H__E822BE1F_C8E5_40B7_B8AA_80F6CE2AA201__INCLUDED_)
