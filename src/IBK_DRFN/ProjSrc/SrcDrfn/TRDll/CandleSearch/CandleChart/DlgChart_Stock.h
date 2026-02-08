#if !defined(AFX_DLGCHART_H__3FD5AA2D_091E_4140_A2EF_A92180003939__INCLUDED_)
#define AFX_DLGCHART_H__3FD5AA2D_091E_4140_A2EF_A92180003939__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChart.h : header file
//

#include "../Resource.h"
//#include "Tr_Drds/ChartTrDataDef.h"
#include "ChartItem_Stock.h"

#include "../RscDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgChart_Stock dialog

class CDlgChart_Stock : public CRscDlg
{
// Construction
public:
	CDlgChart_Stock(CWnd* pParent = NULL);   // standard constructor

	CChartItem_Stock		m_ChartItem;

	void			SetBaseDesk(long lKey) { m_pBaseDesk = lKey;}	// 메인Dlg 베이스
	void			SetMChartInterface(long lChartInterface)		{ m_lChartInterface = lChartInterface; }
	long			m_pBaseDesk;
	long	m_lKey;	
	
// Dialog Data
	//{{AFX_DATA(CDlgChart_Stock)
	enum { IDD = IDD_DLG_CHART };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChart_Stock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	
protected:
	long m_lChartInterface;
	// Generated message map functions
	//{{AFX_MSG(CDlgChart_Stock)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHART_H__3FD5AA2D_091E_4140_A2EF_A92180003939__INCLUDED_)
