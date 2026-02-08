
#pragma once
// listCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ClistCtrl window

class ClistCtrl : public CListCtrl
{
// Construction
public:
	ClistCtrl();

// Attributes
protected:

// Operations
public:
	void	Selected(int nItem, BOOL scroll = TRUE);	// selected item position
	void	MouseUpEvent();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClistCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ClistCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(ClistCtrl)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	bool	m_bMouseDown;
};

