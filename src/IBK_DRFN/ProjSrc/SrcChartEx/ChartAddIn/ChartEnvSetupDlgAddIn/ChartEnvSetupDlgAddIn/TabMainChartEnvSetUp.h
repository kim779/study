#if !defined(AFX_TABMAINCHARTENVSETUP_H__2201DED6_4D71_4F3B_AF57_55040565BBD7__INCLUDED_)
#define AFX_TABMAINCHARTENVSETUP_H__2201DED6_4D71_4F3B_AF57_55040565BBD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabMainChartEnvSetUp.h : header file
//

#include <AFXTEMPL.H>

#include "Include_AddIn.h"		// for IChartOCX
#include "BaseTabDlg.h"

class CDlgEnvSetUpAll;
/////////////////////////////////////////////////////////////////////////////
// CTabMainChartEnvSetUp window

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 8
// Comments		: 
// Using way	: 
// See			: class CTabCtrl
//-----------------------------------------------------------------------------
class CTabMainChartEnvSetUp : public CTabCtrl
{
// Construction & Destruction
public:
	CTabMainChartEnvSetUp();
	~CTabMainChartEnvSetUp();


// Attributes
public:
	int m_nTotalTabPages;		// 전체 TabPage 개수 
	int m_nCurTabPage;			// 현재 TabPage
	
	// Tab이 active된 순서를 저장하는 List. 가장 마지막에 Active된 Tab이 가장 마지막에 연결된다.
	CList < int , int > m_lstTabActiveOrder;		

private:
	IChartOCX *m_pIChartOCX;
	CBaseTabDlg *m_aTabPages[4];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabMainChartEnvSetUp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


// Implementation
public:
	CDlgEnvSetUpAll *m_pDlgEnvSetUpAll;
	
	BOOL Init( IChartOCX *p_pIChartOCX, CDlgEnvSetUpAll *pParentDlg, int nType, int nActiveTabIdx );
	CDialog* GetTabPages( int index );
	
private:
	BOOL CreateTabPage( void );
	void SetTabPagePosition( void );

	bool ChangeDataInTab(CBaseTabDlg* pSourceTabDlg);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabMainChartEnvSetUp)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2006/8/22 - Seung-Won, Bae) Shere the Analysis Tool Color within Analysis and Style Setting.
public:
	void	TabPageMoveWindow(BOOL	bResult);
	void	UpdateAnalysisColorInStyle(			COLORREF p_clrAnalysis);
	void	UpdateAnalysisColorInAnalysisTool(	COLORREF p_clrAnalysis);

// ( 2006 / 10 / 31 - Sang-Woo, Cho ) 
private:
	int m_nUIType;

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
public:
	void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	BOOL m_bPriceChart;
	void	CheckPriceChart();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABMAINCHARTENVSETUP_H__2201DED6_4D71_4F3B_AF57_55040565BBD7__INCLUDED_)
