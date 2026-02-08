#if !defined(AFX_TABSONIKCHART_H__41BF094E_3725_4477_A71A_D931770BB611__INCLUDED_)
#define AFX_TABSONIKCHART_H__41BF094E_3725_4477_A71A_D931770BB611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabSonikChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabSonikChart dialog
#include "resource.h"
#include "DlgInterface.h"
#include "../Common_TeeChart/tchart.h"
#include "../Common_TeeChart/axis.h"
#include "../Common_TeeChart/axes.h"

class CTabSonikChart : public CDlgInterface
{
	// Construction
public:
	CTabSonikChart(CWnd* pParent = NULL);   // standard constructor
	~CTabSonikChart();						// standard destructor

	// Dialog Data
	//{{AFX_DATA(CTabSonikChart)
	enum { IDD = IDD_DLG_TAP_SONIKCHART };
	CStatic	m_stcTotalCount;
	CStatic	m_stcLossCount;
	CStatic	m_stcEarningRatio;
	CStatic	m_stcEarningCount;
	int		m_nRangeUnitWidth;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSonikChart)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabSonikChart)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEdtWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBrush		m_brBackground;

protected:
//	virtual void		GuganBtnUpdate();

// (2005/9/23 - Seung-Won, Bae) Manage Axis
protected:
//	CTAxis *		m_pTAxisRight;	// (2005/9/23 - Seung-Won, Bae) Manage Right Axis to set a Graph.
//	CTAxis *		m_pTAxisBottom;	// (2005/9/29 - Seung-Won, Bae) Manage Bottom Axis to reset scale.

	
public:
	CTChart	m_ctlBarTChart;
	void ShowData1Chart(long);
	int SortData(int* pInData, int nCount);
	void CountData(int* pAllData, int &nAllCount, int* pInData, int nInCount, int &nMin);
	void DisplayData2Chart(int* pAllData, int nAllCount, int nMin);
	

	int m_nInCount;
	int *m_pIndata;
    int	RoundToUnit( int p_nIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSONIKCHART_H__41BF094E_3725_4477_A71A_D931770BB611__INCLUDED_)
