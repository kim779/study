#if !defined(AFX_DLGSONIKCHART_H__2807FB0B_5598_42FD_870C_83C201F1CA30__INCLUDED_)
#define AFX_DLGSONIKCHART_H__2807FB0B_5598_42FD_870C_83C201F1CA30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSonikChart.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSonikChart dialog
#include "DlgOptimizer.h"
#include "./EzpTChart/EzpTChartCtrl.h"		// for CEzpTChartCtrl
#define CLR_BACKGROUND		RGB( 211, 223, 241)
class CTAxis;
class CDlgOptimizer;
class CDlgSonikChart : public CDialog
{
// Construction
public:
	CDlgSonikChart(CWnd* pParent = NULL);   // standard constructor
	~CDlgSonikChart();						// standard destructor
// Dialog Data
	//{{AFX_DATA(CDlgSonikChart)
	enum { IDD = IDD_DLG_TAP_SONIKCHART };
	CStatic	m_stcTotalCount;
	CStatic	m_stcLossCount;
	CStatic	m_stcEarningRatio;
	CStatic	m_stcEarningCount;
	int		m_nRangeUnitWidth;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSonikChart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSonikChart)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		// (2005/9/21 - Seung-Won, Bae) Manage Brush for Background Color
protected:
	CBrush		m_brBackground;

// (2005/9/23 - Seung-Won, Bae) Update New Result Data
public:
	void		GuganBtnUpdate();

// (2005/9/21 - Seung-Won, Bae) Manage EzpTChart
protected:
	CEzpTChartCtrl	m_chartEzp;

// (2005/9/23 - Seung-Won, Bae) Manage Axis
protected:
	CTAxis *		m_pTAxisRight;	// (2005/9/23 - Seung-Won, Bae) Manage Right Axis to set a Graph.
	CTAxis *		m_pTAxisBottom;	// (2005/9/29 - Seung-Won, Bae) Manage Bottom Axis to reset scale.

// (Handumaru) Manage Bar Graph for Fill Zero Bar.
protected:
	CIStrategyItem *m_pStrategyItem;
	CDlgOptimizer* m_pDlgOptimizer;
	CTGraph *		m_pTGraphBar;

public:
		void SetStrategyItem(CIStrategyItem* pStrat);
		void SetOptimizer(CDlgOptimizer* Opt);
public:
	void	RemoveAllGraph();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSONIKCHART_H__2807FB0B_5598_42FD_870C_83C201F1CA30__INCLUDED_)
