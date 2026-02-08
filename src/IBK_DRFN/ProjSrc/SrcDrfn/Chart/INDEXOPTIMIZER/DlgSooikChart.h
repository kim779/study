#if !defined(AFX_DLGSOOIKCHART_H__E1F53DBA_B44C_4A79_88E7_0B73BCD0F2C4__INCLUDED_)
#define AFX_DLGSOOIKCHART_H__E1F53DBA_B44C_4A79_88E7_0B73BCD0F2C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSooikChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSooikChart dialog
#include "resource.h"
#include "DlgOptimizer.h"
//#include "./EzpTChart/EzpTChartCtrl.h"		// for CEzpTChartCtrl
#define CLR_BACKGROUND		RGB( 211, 223, 241)

class CDlgOptimizer;
class CTAxis;
class CDlgSooikChart : public CDialog
{
// Construction
public:
	CDlgSooikChart(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSooikChart)
	enum { IDD = IDD_DLG_TAP_SOOIKCHART };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSooikChart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSooikChart)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRType1();
	afx_msg void OnRType2();

	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// (2005/9/21 - Seung-Won, Bae) Manage Brush for Background Color
protected:
	CBrush		m_brBackground;
	public:
	CIStrategyItem *m_pStrategyItem;
	CDlgOptimizer* m_pDlgOptimizer;
	CTGraph *		m_pTGraphBar;

// (2005/9/23 - Seung-Won, Bae) Update New Result Data
protected:
	int					m_nQuerySection;
public:
	void		GuganBtnUpdate();
public:
	void	RemoveAllGraph();
// (2005/9/27 - Seung-Won, Bae) Earnings Ratio Chart
protected:
	class CEarningsRatioChartSection
	{
		// (2005/9/21 - Seung-Won, Bae) Manage EzpTChart
		protected:
			//CEzpTChartCtrl	m_chartEzp;

		// (2005/9/23 - Seung-Won, Bae) Manage Axis
		protected:
			CTAxis *		m_pTAxisLeft;
			CTAxis *		m_pTAxisRight;
			CTAxis *		m_pTAxisTop;
			CTAxis *		m_pTAxisBottom;

		// (2005/9/26 - Seung-Won, Bae) Manage Zero Line for Adding before Other Data Line
		protected:
			CTGraph *		m_pTGraphZero;

		// (2005/9/27 - Seung-Won, Bae) Inner Operation.
		public:
			CEarningsRatioChartSection();
		public:
			BOOL	CreateChart( CWnd *p_pParent, CRect p_rect, UINT p_uID);
			void	ShowControl( BOOL p_bShow)		{	m_chartEzp.ShowControl( p_bShow);	}
		public:
			void	ShowFullChart(CDlgOptimizer* pOptimizer);
			void	ShowPartialChart(CDlgOptimizer* pOptimizer);
			BOOL	ShowZeroLine( void);
			void	RemoveGraph();
	} m_chartSection;
protected:
	class CEarningsRatioChartAccumulation
	{
		// (2005/9/21 - Seung-Won, Bae) Manage EzpTChart
		protected:
			//CEzpTChartCtrl	m_chartEzp;

		// (2005/9/23 - Seung-Won, Bae) Manage Axis
		protected:
			CTAxis *		m_pTAxisLeft;
			CTAxis *		m_pTAxisRight;
			CTAxis *		m_pTAxisTop;
			CTAxis *		m_pTAxisBottom;

		// (2005/9/26 - Seung-Won, Bae) Manage Zero Line for Adding before Other Data Line
		protected:
			CTGraph *		m_pTGraphZero;

		// (2005/9/27 - Seung-Won, Bae) Inner Operation.
		public:
			CEarningsRatioChartAccumulation();
		public:
			BOOL	CreateChart( CWnd *p_pParent, CRect p_rect, UINT p_uID);
			void	ShowControl( BOOL p_bShow)		{	m_chartEzp.ShowControl( p_bShow);	}
		public:
			void	ShowChart( int p_nSection,CDlgOptimizer* pOptimizer);
			BOOL	ShowZeroLine( void);
			void	RemoveGraph(void);
	} m_chartAccumulation;
protected:
	class CEarningsRatioChartCase
	{
		// (2005/9/21 - Seung-Won, Bae) Manage EzpTChart
		protected:
			// CEzpTChartCtrl	m_chartEzp;

		// (2005/9/23 - Seung-Won, Bae) Manage Axis
		protected:
			CTAxis *		m_pTAxisLeft;
			CTAxis *		m_pTAxisRight;
			CTAxis *		m_pTAxisTop;
			CTAxis *		m_pTAxisBottom;

		// (2005/9/26 - Seung-Won, Bae) Manage Zero Line for Adding before Other Data Line
		protected:
			CTGraph *		m_pTGraphZero;

		// (2005/9/27 - Seung-Won, Bae) Inner Operation.
		public:
			CEarningsRatioChartCase();
		public:
			BOOL	CreateChart( CWnd *p_pParent, CRect p_rect, UINT p_uID);
			void	ShowControl( BOOL p_bShow)		{	m_chartEzp.ShowControl( p_bShow);	}
		public:
			void	ShowChart( int p_nSection,CDlgOptimizer* pOptimizer);
			BOOL	ShowZeroLine( void);
			void	RemoveGraph();
	} m_chartCase;

	// (Handumaru) Manage Bar Graph for Fill Zero Bar.

public:
		void SetStrategyItem(CIStrategyItem* pStrat);
		void SetOptimizer(CDlgOptimizer* Opt);

		// 차트에 데이터를 처리한다.
		CTChart		m_ctlBarTChart;
		void ShowData1Chart(long);
		void ShowData2Chart(CStringArray &stNujukArray, int nShowRow);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSOOIKCHART_H__E1F53DBA_B44C_4A79_88E7_0B73BCD0F2C4__INCLUDED_)
