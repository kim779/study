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
	virtual ~CEditX();

// Attributes
public:
	CBrush	m_brush;
	CWnd	*m_pParent;

// Operations
public:
	BOOL	Create(CWnd* parent, UINT nID);
	void	SetPopupParent(CWnd *pParent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditX)
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditX)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};