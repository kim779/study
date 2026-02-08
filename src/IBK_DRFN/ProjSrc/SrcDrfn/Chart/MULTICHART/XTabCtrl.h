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
//@유진삭제
//	int		m_nChartType;
//	int		ChangeTabIndexByChartGubun(int nIndex);
//	//enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, FOREIGN_CHART}; /*05.05.16추가 - 0:주식/1:업종/2:선옵 차트 구분자*/
//	// 상품선물옵션차트 - ojtaso (20071001)
//	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART, COMMODITY_CHART};/*05.05.16추가 - 0:주식/1:업종/2:선옵 차트 구분자*/
//@유진삭제
//@유진추가
	int		m_nMarketType;
	int		ChangeTabIndexByMarketType(int nIndex);
//@유진추가
	void Init(CWnd* pMultiChartWnd,CWnd *pChartMng, int nChartType = 0/*05.05.16추가 - 0:주식/1:업종/2:선옵 차트 구분자*/);
	void ShowTab(int nIndex);
//	void EnableTab(int iIndex, BOOL bEnable = TRUE);
//	BOOL SelectTab(int iIndex);
//	void DeleteAllTabs();
//	void DeleteTab(int iIndex);
	void SetTopLeftCorner(CPoint pt);

	void SetType(short nType = 0) {m_nType = nType;}
//	void DrawGradientTab(CDC *pDC, CRect rect, BOOL bSelectedItem=TRUE);
//	void DrawLine(CPoint ptMove, CPoint ptTo, CDC* pDC, COLORREF cr);

	long GetTabHeight() { RECT rect; GetItemRect(0,&rect); return(rect.bottom-rect.top);}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTabCtrl)
	protected:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
//	BOOL SelectNextTab(BOOL bForward);
//	BOOL SelectTabByName(LPCTSTR lpszTabCaption);
//	void ChangeTab(int iIndex, CWnd* pNewTab, LPTSTR lpszCaption, int iImage);
	virtual ~CXTabCtrl();

	CTabDlg *GetWndFocused() {return m_pTabDlgFocused;}
	CTabDlg *GetWndCodeSearch() {return m_pTabDlgCodeSearch;}
	void SetpMultiChartWnd(CWnd *pWnd) { m_pMultiChartWnd = pWnd;}
	void ChangeSkinColor(CWnd* pStdDialog);

	// 시스템트레이딩 전략 저장/불러오기
	CTabDlg *GetWndSystemTabEx() {return m_pTabDlgSystemTabEx;}
	void	SetWndSystemTabEx();

	// Generated message map functions
private:
	void DeleteCurrentDlg();
	CTabDlgCreator m_tabDlgCreator;
	CWnd *m_pMultiChartWnd;
protected:
//	CArray<BOOL, BOOL> m_arrayStatusTab; //** enabled Y\N
	int m_iSelectedTab;
	POINT m_ptTabs;

	BOOL	m_bDown;
	CRect	m_rcRound;
	CPoint	m_ptPoint;
	int		m_nPrevIndex;
	short	m_nType;

	int		m_nRowCnt;
	CTabDlg	*m_pTabDlgFocused;
	CTabDlg	*m_pTabDlgCodeSearch;
	CTabDlg *m_pTabDlgSystemTabEx;		//@@@@@06.11.20
	CTabDlg *m_pTabIndices;				//@@@@@06.12.07
	//{{AFX_MSG(CXTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XTABCTRL_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_)
