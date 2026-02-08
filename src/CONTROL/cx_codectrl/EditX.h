#pragma once
// EditX.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditX window

class CEditX : public CEdit
{
// Construction
public:
	CEditX();

// Attributes
public:
	CBrush	m_brush;

// Operations
public:
	BOOL	Create(CWnd* parent, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditX)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditX();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditX)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};