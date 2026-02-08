#pragma once
// StgBoard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStgBoard window

class CStgBoard : public CWnd
{
// Construction
public:
	CStgBoard(CWnd* parent);

// Attributes
public:
	CWnd	*m_pParent=nullptr;

	int		m_Scroll=0;
	CRect	m_rect;

	CFont	*m_pFont=nullptr;
	std::unique_ptr<CBitmap> m_bmpBoard=nullptr;

// Operations
public:
	COLORREF getIndexColor(int index);
	void drawSelection(CDC *dc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStgBoard)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStgBoard();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStgBoard)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

