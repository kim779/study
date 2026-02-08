#pragma once
// JumunCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJumunCheck dialog

class CJumunCheck : public CDialog
{
// Construction
public:
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

