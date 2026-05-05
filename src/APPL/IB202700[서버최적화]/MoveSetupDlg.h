#pragma once
// MoveSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoveSetupDlg dialog

class CMoveSetupDlg : public CDialog
{
// Construction
public:
	CMoveSetupDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMoveSetupDlg)
	enum { IDD = IDD_ROW_MOVE };
	CButton	m_cbAlert;
	int		m_nOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoveSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes;
public:
	BOOL m_bAlert;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMoveSetupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

