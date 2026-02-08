#if !defined(AFX_LISTCHARTDATADLG_H__0FD2E8B8_B435_40D8_AAC8_23D934E3BC6A__INCLUDED_)
#define AFX_LISTCHARTDATADLG_H__0FD2E8B8_B435_40D8_AAC8_23D934E3BC6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListChartDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListChartDataDlg dialog

class CListChartDataDlg : public CDialog
{
// Construction
public:
	CListChartDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CListChartDataDlg)
	enum { IDD = IDD_DLG_CHARTDATA };
	CListCtrl	m_listData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListChartDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CListChartDataDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCHARTDATADLG_H__0FD2E8B8_B435_40D8_AAC8_23D934E3BC6A__INCLUDED_)
