#pragma once
// ShareGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShareGroupDlg dialog

class CShareGroupDlg : public CDialog
{
// Construction
public:
	CShareGroupDlg(CWnd* pParent = nullptr, CString root = _T(""), CString name = _T(""), BOOL bImport = TRUE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShareGroupDlg)
	enum { IDD = IDD_SHAREGROUP };
	CListCtrl	m_listGroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShareGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShareGroupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL	addColumn(LPCTSTR strItem, int nItem, int nWidth);
	CString	parseX(CString &srcstr, CString substr);
	int		addItem(const CString& strItem, int nItem);

	void LoadImportGroup();
	void LoadExportGroup();

private:
	BOOL m_bImport;

	CString m_strUserDir;
	CString m_strTempFile;
};

