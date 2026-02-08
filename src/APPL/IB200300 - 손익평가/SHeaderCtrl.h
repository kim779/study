#pragma once
// SHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSHeaderCtrl window

class CSHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CSHeaderCtrl(CMapWnd* pMapWnd);

// Attributes
public:
	CFont m_Font;
	COLORREF m_clrBk{};
	COLORREF m_clrEdge{};
	COLORREF m_clrText{};
	int			m_nHideColIdx{};
	class CMapWnd* m_pMapWnd{};
// Operations
public:
	inline void HideColumnOver(int nColIdx){
		m_nHideColIdx = nColIdx;
	}
	inline void SetBkColor(COLORREF bk){
		m_clrBk = bk;
	}
	inline void SetEdgeColor(COLORREF clr){
		m_clrEdge = clr;
	}
	inline void SetTextColor(COLORREF clr){
		m_clrText = clr;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSHeaderCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSHeaderCtrl)
	afx_msg void OnPaint();
	afx_msg LRESULT OnLayout(WPARAM wparam, LPARAM lparam);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

