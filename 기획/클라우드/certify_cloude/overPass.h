#if !defined(AFX_OVERPASS_H__B7C67FBA_7229_4D9A_85E5_59C8308A3C49__INCLUDED_)
#define AFX_OVERPASS_H__B7C67FBA_7229_4D9A_85E5_59C8308A3C49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// overPass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COverPass dialog

class COverPass : public CDialog
{
// Construction
public:
	COverPass(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COverPass)
	enum { IDD = IDD_OVER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverPass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COverPass)
	virtual BOOL OnInitDialog();
	afx_msg void OnIssue();
	afx_msg void OnOther();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERPASS_H__B7C67FBA_7229_4D9A_85E5_59C8308A3C49__INCLUDED_)
