#pragma once
// JumunCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJumunCheck dialog

class CJumunCheck : public CDialog
{
// Construction
public:
	void SetMsg(CString msg);
	CJumunCheck(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJumunCheck)
	enum { IDD = IDD_JUMUNCHECK_DIALOG };
	CString	m_msg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJumunCheck)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJumunCheck)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

