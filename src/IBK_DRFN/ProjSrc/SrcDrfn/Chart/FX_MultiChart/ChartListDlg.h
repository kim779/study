#if !defined(AFX_CHARTLISTDLG_H__B9C69A11_C818_47CA_91FE_CC9C5F64AD27__INCLUDED_)
#define AFX_CHARTLISTDLG_H__B9C69A11_C818_47CA_91FE_CC9C5F64AD27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartListDlg.h : header file
//
#include "RscDlg.h"
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CChartListDlg dialog
class CChartMng;
class CChartListDlg : public CRscDlg
{
// Construction
public:
	int m_nMarketType;

	CChartListDlg(CWnd* pParent = NULL);   // standard constructor
	void SetUserPath(LPCTSTR lpUserPath) {m_strUserPath = lpUserPath;}
// Dialog Data
	//{{AFX_DATA(CChartListDlg)
	enum { IDD = IDD_DLGCHARTLIST };
	CListCtrl	m_listChart;
	//}}AFX_DATA

	CString m_strChartSelected;
	int m_nInfoType; // 0: load, 1: save
	CPoint *m_pPtTopLeftOfDlg;
	void AddRow(LPCTSTR szNo, LPCTSTR szName);
	void SwapData(int srcIdx, int dstIdx);
	CChartMng* m_pChartMng;
	void WriteIndexFile();
	void DeleteFile(int nIndex);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CString m_strUserPath;
protected:

	// Generated message map functions
	//{{AFX_MSG(CChartListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListchart();
	virtual void OnOK();
	afx_msg void OnBtnnew();
	afx_msg void OnClickListchart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemup();
	afx_msg void OnItemdown();
	afx_msg void OnAdd();
	afx_msg void OnBtndelete();
	afx_msg void OnRename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTLISTDLG_H__B9C69A11_C818_47CA_91FE_CC9C5F64AD27__INCLUDED_)
