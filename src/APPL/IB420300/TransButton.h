#pragma once
// TransButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransButton window

class CTransButton : public CButton
{
// Construction
public:
	CTransButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTransButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTransButton)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

