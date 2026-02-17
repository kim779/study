#if !defined(AFX_INVALID_H__75F2198D_D4CE_4F5B_9A20_958B6E9B0691__INCLUDED_)
#define AFX_INVALID_H__75F2198D_D4CE_4F5B_9A20_958B6E9B0691__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// invalid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInvalid dialog

class CInvalid : public CDialog
{
// Construction
public:
	CInvalid(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInvalid)
	enum { IDD = IDD_INVALID };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvalid)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInvalid)
	virtual BOOL OnInitDialog();
	afx_msg void OnIssue();
	afx_msg void OnOther();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVALID_H__75F2198D_D4CE_4F5B_9A20_958B6E9B0691__INCLUDED_)
