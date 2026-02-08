#if !defined(AFX_XTABCTRL_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_)
#define AFX_XTABCTRL_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XTabCtrl.h : header file
//
#include <Afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CXTabCtrl window

class CXTabCtrl : public CTabCtrl
{
// Construction
public:
	CXTabCtrl();

// Attributes
public:

// Operations
public:
	void AddTab(CWnd* pWnd, LPTSTR lpszCaption, int iImage =0);
	void EnableTab(int iIndex, BOOL bEnable = TRUE);
	BOOL SelectTab(int iIndex);
	void DeleteAllTabs();
	void DeleteTab(int iIndex);
	void SetTopLeftCorner(CPoint pt);
	BOOL IsTabEnabled(int iIndex);

	void SetDisabledColor(COLORREF cr);
	void SetSelectedColor(COLORREF cr, COLORREF crTab);
	void SetNormalColor(COLORREF cr, COLORREF crTabStart, COLORREF crTabEnd);
	void SetMouseOverColor(COLORREF cr);

	void SetPrevSelect();
	void SetType(short nType = 0) {m_nType = nType;}
	void SetTabBkColor(COLORREF clTabBk) {m_clTabBk = clTabBk;}
	void DrawGradientTab(CDC *pDC, CRect rect);
	void DrawLine(CPoint ptMove, CPoint ptTo, CDC* pDC, COLORREF cr);
	void SetUpperOffset(int nUpperOffset) { m_nUpperOffset = nUpperOffset;}
	void SetLeftOffset(int nLeftOffset) { m_nLeftOffset = nLeftOffset;}
	COLORREF m_clrBorder;
//	COLORREF m_clrBkGrnd;
	COLORREF m_clrHilight;
	COLORREF m_clrShadow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTabCtrl)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SelectNextTab(BOOL bForward);
	BOOL SelectTabByName(LPCTSTR lpszTabCaption);
	void ChangeTab(int iIndex, CWnd* pNewTab, LPTSTR lpszCaption, int iImage);
	virtual ~CXTabCtrl();

	// Generated message map functions
protected:
	CArray<BOOL, BOOL> m_arrayStatusTab; //** enabled Y\N
	int m_iSelectedTab;
	POINT m_ptTabs;
	COLORREF m_clSelTabBK;
	COLORREF m_crSelected;
	COLORREF m_crDisabled;
	COLORREF m_crNormal;
	COLORREF m_crMouseOver;
	COLORREF m_clTabBk;

	int m_iIndexMouseOver;
	
	bool m_bMouseOver;
	bool m_bColorMouseOver;
	bool m_bColorNormal;
	bool m_bColorDisabled;
	bool m_bColorSelected;
	bool m_bColorUnselected;

	BOOL	m_bDown;
	CRect	m_rcRound;
	CPoint	m_ptPoint;
	int		m_nPrevIndex;
	short	m_nType;
	COLORREF m_clNormalTabStart;
	COLORREF m_clNormalTabEnd;

	int		m_nUpperOffset;
	int		m_nLeftOffset;

	//{{AFX_MSG(CXTabCtrl)
	afx_msg BOOL OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XTABCTRL_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_)
