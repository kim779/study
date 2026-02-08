#pragma once
// LogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog

class CLogDlg : public CDialog
{
// Construction
public:
	CLogDlg(CString sFile, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogDlg)
	enum { IDD = IDD_LOGDIALOG };
	CListCtrl	m_LogList;
	//}}AFX_DATA

	CString			m_strLogPath;
	
	void			LoadLogFile();
	void			LoadLogFile(int istart, int icount);
	void			InsertLog(CString sDate, CString sTime, CString sType, CString sCode, CString sItem, CString sLog, int nItem=0, int ikey=0) ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNextbutton();
	afx_msg void OnSearchbutton();
	afx_msg void OnClosebutton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnResetLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

