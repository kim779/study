#pragma once
// RealCheckDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRealCheckDlg dialog

class CGridWnd;

class CRealCheckDlg : public CDialog
{
// Construction
public:
	CRealCheckDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRealCheckDlg)
	enum { IDD = IDD_REALTIME_CHECK };
	CEdit	m_edData;
	CEdit	m_edSymbol;
	CListBox	m_listGrid;
	CStatic	m_stcTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	CGridWnd*	m_pGridWnd;
	CString		m_strSymbol;
	CRect		m_rcWnd;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealCheckDlg)
	afx_msg void OnClose();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};