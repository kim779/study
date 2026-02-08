#pragma once
// STimeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSTimeCtrl window

class CSTimeCtrl : public CDateTimeCtrl
{
// Construction
public:
	CSTimeCtrl();

// Attributes
public:
	CBrush m_bk;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTimeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTimeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSTimeCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

