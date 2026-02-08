#if !defined(AFX_DLGCHARTINVERTSETTING_H__E29C8882_71F3_4646_A558_DD3442AC8199__INCLUDED_)
#define AFX_DLGCHARTINVERTSETTING_H__E29C8882_71F3_4646_A558_DD3442AC8199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChartInvertSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChartInvertSetting dialog

class CChartLink;
class CDlgChartInvertSetting : public CDialog
{
// Construction
public:
	CDlgChartInvertSetting( CChartLink *pBlockItem, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChartInvertSetting)
	enum { IDD = IDD_CHART_INVERT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChartInvertSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChartInvertSetting)
	afx_msg void OnBtnReset();
	afx_msg void OnBtnReverse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CChartLink *	m_pBlockItem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHARTINVERTSETTING_H__E29C8882_71F3_4646_A558_DD3442AC8199__INCLUDED_)
