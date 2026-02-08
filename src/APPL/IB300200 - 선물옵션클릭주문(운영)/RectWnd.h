#pragma once

// RectWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRectWnd window

class CRectWnd : public CWnd
{
// Construction
public:
	CRectWnd();
	CRectWnd(CWnd* pParent);

	CWnd* m_pParent{};

public:
	int		m_color{};

// Attributes
public:
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRectWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
//	void MouseUp();
//	void MouseClick();
	
	virtual ~CRectWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRectWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

