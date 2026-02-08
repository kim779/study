#if !defined(AFX_XTABCTRL_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_)
#define AFX_XTABCTRL_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XTabCtrl.h : header file
//
#include <Afxtempl.h>
#include "TabDlgCreator.h"
#include "../../inc/ExOCXDLL.h"

/////////////////////////////////////////////////////////////////////////////
// CXTabCtrl window
class CTabDlg;
class CXTabCtrl : public CExTabCtrl
{
// Construction
public:
	CXTabCtrl();

// Attributes
public:

// Operations
public:
//	void AddTab(CTabDlg* pWnd, LPTSTR lpszCaption, int iImage =0);
	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, FOREIGN_CHART}; /*05.05.16추가 - 0:주식/1:업종/2:선옵 차트 구분자*/	
	void Init(CWnd* pCompareChartWnd,CWnd *pChartMng, int nChartType = 0/*05.05.16추가 - 0:주식/1:업종/2:선옵 차트 구분자*/, void* m_pDBMgr = NULL/*05.06.13추가 CodeInput용*/);
	void ShowTab(int nIndex);
//	void EnableTab(int iIndex, BOOL bEnable = TRUE);
//	BOOL SelectTab(int iIndex);
//	void DeleteAllTabs();
//	void DeleteTab(int iIndex);
	void SetTopLeftCorner(CPoint pt);
//	BOOL IsTabEnabled(int iIndex);

//	void SetDisabledColor(COLORREF cr);
//	void SetSelectedColor(COLORREF cr, COLORREF crTabStart, COLORREF crTabEnd);
//	void SetMouseOverColor(COLORREF cr, COLORREF crTabStart, COLORREF crTabEnd);

//	void SetNormalColor(COLORREF cr);
//	void SetMouseOverColor(COLORREF cr);

	
//	void SetPrevSelect();
	void SetType(short nType = 0) {m_nType = nType;}
//	void SetTabBkColor(COLORREF clTabBk) {m_clTabBk = clTabBk;}
//	void DrawGradientTab(CDC *pDC, CRect rect, BOOL bSelectedItem=TRUE);
//	void DrawLine(CPoint ptMove, CPoint ptTo, CDC* pDC, COLORREF cr);

	long GetTabHeight() { RECT rect; GetItemRect(0,&rect); return(rect.bottom-rect.top);}

	void ChangeSkinColor();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTabCtrl)
	protected:
//	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
//	BOOL SelectNextTab(BOOL bForward);
//	BOOL SelectTabByName(LPCTSTR lpszTabCaption);
//	void ChangeTab(int iIndex, CWnd* pNewTab, LPTSTR lpszCaption, int iImage);
	virtual ~CXTabCtrl();

	CTabDlg *GetWndFocused() {return m_pTabDlgFocused;}
	CTabDlg *GetWndCodeSearch() {return m_pTabDlgCodeSearch;}
	void SetpCompareChartWnd(CWnd *pWnd) { m_pCompareChartWnd = pWnd;}
	// Generated message map functions
private:
	void DeleteCurrentDlg();
	CTabDlgCreator m_tabDlgCreator;
	CWnd *m_pCompareChartWnd;
protected:
//	CArray<BOOL, BOOL> m_arrayStatusTab; //** enabled Y\N
	int m_iSelectedTab;
	POINT m_ptTabs;
	COLORREF m_crSelected;
	COLORREF m_crDisabled;
	COLORREF m_crNormal;
	COLORREF m_crMouseOver;
	COLORREF m_clTabBk;

	int m_iIndexMouseOver;
	int m_iLastIndexMouseOver;
	
	bool m_bMouseOver;
	bool m_bColorMouseOver;
	bool m_bColorNormal;
	bool m_bColorDisabled;
	bool m_bColorSelected;

	
	BOOL	m_bDown;
	CRect	m_rcRound;
	CPoint	m_ptPoint;
	int		m_nPrevIndex;
	short	m_nType;
	COLORREF m_clSelTabStart;
	COLORREF m_clSelTabEnd;
	COLORREF m_clOverTabStart;
	COLORREF m_clOverTabEnd;

	int		m_nRowCnt;
	CTabDlg	*m_pTabDlgFocused;
	CTabDlg	*m_pTabDlgCodeSearch;
	//{{AFX_MSG(CXTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XTABCTRL_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_)
