#if !defined(AFX_TABEARNCHART_H__38CF3FD6_A480_4104_86DA_E1DB12A149A7__INCLUDED_)
#define AFX_TABEARNCHART_H__38CF3FD6_A480_4104_86DA_E1DB12A149A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabEarnChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabSoonikChart dialog

#include "resource.h"
#include "DlgInterface.h"
#include "../Common_TeeChart/mschart1.h"
#include "../Common_TeeChart/tchart.h"


#define	CTabEarnChart		CTabSoonikChart


class CTAxis;
//class CInputLeft;
class CTabSoonikChart : public CDlgInterface
{
// Construction
public:
	CTabSoonikChart(CWnd* pParent = NULL);	// standard constructor
	~CTabSoonikChart();						// standard destructor

// Dialog Data
	//{{AFX_DATA(CTabSoonikChart)
	enum { IDD = IDD_DLG_TAP_SOOIKCHART};
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSoonikChart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabSoonikChart)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

protected:
	CBrush		m_brBackground;

public:
	CTChart		m_ctlBarTChart;
	CMschart1	m_ctlLineMsChart;

	void ShowData1Chart(long);
	void ShowData2Chart(CStringArray &stNujukArray, int nShowRow);
	void PrepareShow(int nShowCount);	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABEARNCHART_H__38CF3FD6_A480_4104_86DA_E1DB12A149A7__INCLUDED_)
