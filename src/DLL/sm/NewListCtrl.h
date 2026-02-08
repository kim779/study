#if !defined(AFX_NEWLISTCTRL_H__FBBDF81B_6720_4DB2_AFDE_0A2DA9366BCF__INCLUDED_)
#define AFX_NEWLISTCTRL_H__FBBDF81B_6720_4DB2_AFDE_0A2DA9366BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewListCtrl window
#include "usedefine.h"
class CNewListCtrl : public CListCtrl
{
// Construction
public:
	CNewListCtrl();
	void EnableHighlighting(HWND hWnd, int row, bool bHighlight);
	bool IsRowSelected(HWND hWnd, int row);
	bool IsRowHighlighted(HWND hWnd, int row);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void PositionScrollBars();
	void Init();
	virtual ~CNewListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNewListCtrl)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnCustomDrawList ( NMHDR* pNMHDR, LRESULT* pResult );

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWLISTCTRL_H__FBBDF81B_6720_4DB2_AFDE_0A2DA9366BCF__INCLUDED_)
