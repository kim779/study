#pragma once
// AutoSaveSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoSaveSetupDlg dialog

class CAutoSaveSetupDlg : public CDialog
{
// Construction
public:
	CAutoSaveSetupDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoSaveSetupDlg)
	enum { IDD = IDD_AUTOSAVE_SETUP };
	CButton	m_cbChg;
	CButton	m_cbAllApply;
	CButton	m_cbAdd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoSaveSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	BOOL m_bAddCnd;
	BOOL m_bChgCnd;
	BOOL m_bAllApply;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoSaveSetupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStaticChg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};