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
//@유진추가
	int		m_nMarketType;
	int		ChangeTabIndexByMarketType(int nIndex);
//@유진추가
	void Init(CWnd* pMultiChartWnd,CWnd *pChartMng, int nChartType = 0/*05.05.16추가 - 0:주식/1:업종/2:선옵 차트 구분자*/);
	void ShowTab(int nIndex);
	void SetTopLeftCorner(CPoint pt);

	void SetType(short nType = 0) {m_nType = nType;}

	long GetTabHeight() { RECT rect; GetItemRect(0,&rect); return(rect.bottom-rect.top);}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTabCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXTabCtrl();

	CTabDlg *GetWndFocused() {return m_pTabDlgFocused;}
	CTabDlg *GetWndCodeSearch() {return m_pTabDlgCodeSearch;}
	void SetpMultiChartWnd(CWnd *pWnd) { m_pMultiChartWnd = pWnd;}
	void ChangeSkinColor(CWnd* pStdDialog);
	
	// Generated message map functions
private:
	void DeleteCurrentDlg();
	CTabDlgCreator m_tabDlgCreator;
	CWnd *m_pMultiChartWnd;
protected:
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
