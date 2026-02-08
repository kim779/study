#pragma once
// StgInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStgInfo window

class CStgInfo : public CWnd
{
// Construction
public:
	CStgInfo(CWnd* parent, CFont* pfont);

// Attributes
public:
	CWnd	*m_pParent;

	CString	m_sRoot;
	int	m_Scroll;

	CRect	m_rect;
	CFont*	m_pFont=nullptr;
	CPoint	m_prvPoint;
	std::unique_ptr<CBitmap> m_pBitmap;
//	std::unique_ptr<class CToolTip> m_pToolTip;
	class CToolTip* m_pToolTip;

// Operations
public:
	COLORREF getIndexColor(int index);
	BOOL LoadImage();
	void DrawInfo(CDC* pDC);
	CPoint GetCell(CPoint pt);
	void ShowToolTip(bool show);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStgInfo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStgInfo();

	// Generated message map functions
protected:
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam) ;
	//{{AFX_MSG(CStgInfo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

