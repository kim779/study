#pragma once
// ColoredListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColoredListCtrl window

class CColoredListCtrl : public CListCtrl
{
// Construction
public:
	CColoredListCtrl();

// Attributes
public:
	COLORREF m_colRow1;
	COLORREF m_colRow2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoredListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColoredListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	//}}AFX_MSG
	afx_msg void CColoredListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

