#if !defined(AFX_TABANALCHART_H__BA8D543B_8DEF_4FA5_A155_52723347F124__INCLUDED_)
#define AFX_TABANALCHART_H__BA8D543B_8DEF_4FA5_A155_52723347F124__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabAnalChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabAnalChart dialog
#include "resource.h"
//#include "Control/ChartItem.h"
#include "DlgInterface.h"



class CInputLeft;
class CTabAnalChart : public CDlgInterface
{
// Construction
public:
	CTabAnalChart(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabAnalChart)
	enum { IDD = IDD_DLG_TAP_ANALCHART };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabAnalChart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	CChartItem m_Chart;

	// Generated message map functions
	//{{AFX_MSG(CTabAnalChart)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LRESULT OnUpdateDataIndex( WPARAM wParam, LPARAM lParam);		
	LRESULT OnSelectDataIndex( WPARAM wParam, LPARAM lParam);		
public:
	COLORREF m_crBk;

	CBrush*	m_pBrBkColor;
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}
	void UpdateDlg();
	void SetChartFocus(int nFocusIndex);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABANALCHART_H__BA8D543B_8DEF_4FA5_A155_52723347F124__INCLUDED_)
