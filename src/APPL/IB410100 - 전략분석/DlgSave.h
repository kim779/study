#pragma once
// DlgSave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSave dialog

class CDlgSave : public CDialog
{
// Construction
public:
	CDlgSave(CWnd* pParent = NULL);   // standard constructor

public:
	CWnd*		m_pParent;
	CString		m_sPath;
	CIB410100App	*m_pApp;
	
public:
	BOOL	addColumn(CListCtrl *LC, LPCTSTR str, int item, int width);
	void	setList();

// Dialog Data
	//{{AFX_DATA(CDlgSave)
	enum { IDD = IDD_DLGSAVE };
	CListCtrl	m_ListStrategy;
	CEdit	m_Edit_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSave)
	virtual BOOL OnInitDialog();
	afx_msg void OnSave();
	afx_msg void OnClickListStrategy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

