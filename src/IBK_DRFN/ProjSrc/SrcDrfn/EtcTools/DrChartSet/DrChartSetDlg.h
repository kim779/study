// DrChartSetDlg.h : header file
//

#if !defined(AFX_DRCHARTSETDLG_H__ADDCC87B_6311_4B97_B8AF_B64DC82E06AD__INCLUDED_)
#define AFX_DRCHARTSETDLG_H__ADDCC87B_6311_4B97_B8AF_B64DC82E06AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDrChartSetDlg dialog

class CDrChartSetDlg : public CDialog
{
// Construction
public:
	CDrChartSetDlg(CWnd* pParent = NULL);	// standard constructor
	void DoShowVip();

// Dialog Data
	//{{AFX_DATA(CDrChartSetDlg)
	enum { IDD = IDD_DRCHARTSET_DIALOG };
	CListBox	m_ResultList;
	CListBox	m_FileList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrChartSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CString m_szTargetDir;

	int  _GetFileList(LPCSTR pszDir, CStringList& rList);
	BOOL _DoPreDirectoryCheck(CString &rString);

	// Generated message map functions
	//{{AFX_MSG(CDrChartSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtndirlist();
	afx_msg void OnBtnchange();
	afx_msg void OnChangeEditdir();
	afx_msg void OnBtnchartsetlup();
	afx_msg void OnBtnchartsetlup2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRCHARTSETDLG_H__ADDCC87B_6311_4B97_B8AF_B64DC82E06AD__INCLUDED_)
