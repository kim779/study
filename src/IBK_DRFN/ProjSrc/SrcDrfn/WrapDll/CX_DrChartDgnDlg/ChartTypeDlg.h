#if !defined(AFX_CHARTTYPEDLG_H__3776461A_49AE_4525_B2E7_97AEBEAD64C1__INCLUDED_)
#define AFX_CHARTTYPEDLG_H__3776461A_49AE_4525_B2E7_97AEBEAD64C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartTypeDlg.h : header file
//
#include "ImageViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CChartTypeDlg dialog

class CChartTypeDlg : public CDialog
{
// Construction
public:
	CChartTypeDlg(CWnd* pParent = NULL);   // standard constructor

	void DoShowImage(int nSel);
	CString m_szSelectKey;

	static void GetBasePath(CString& rString);

// Dialog Data
	//{{AFX_DATA(CChartTypeDlg)
	enum { IDD = IDD_CHARTTYPEDLG };
	CEdit	m_ctrlTxt;
	CListBox	m_CTList;
	CImageViewer	m_ImageViewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_szBaseDir;
	int _DoLoadList(LPCSTR szCFG);

	// Generated message map functions
	//{{AFX_MSG(CChartTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCtList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTTYPEDLG_H__3776461A_49AE_4525_B2E7_97AEBEAD64C1__INCLUDED_)
