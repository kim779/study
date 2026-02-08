#pragma once
// IHCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIHCheck window

class CIHCheck : public CButton
{
// Construction
public:
	CIHCheck(CString szTip = _T(""));

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIHCheck)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIHCheck();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIHCheck)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	CString		m_szTip;
	std::unique_ptr<CToolTipCtrl>	m_pToolTip;
};

