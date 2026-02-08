#pragma once
// ConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(CWnd* pParent, LPCSTR propfile);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG };
	BOOL	m_bEnterOrder;
	BOOL	m_bOrderConfirm;
	int		m_iClickMode;
	BOOL	m_bPutReverse;
	BOOL	m_bShowBefore;
	BOOL	m_bSpaceOrder;
	UINT	m_iCntWidth;
	UINT	m_iHogaWidth;
	UINT	m_iVolWidth;
	UINT	m_iOdrWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_profile;
	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

