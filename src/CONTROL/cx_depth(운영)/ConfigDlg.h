#pragma once
// ConfigDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(struct _config *pConfig, CWnd* pParent = NULL);   // standard constructor
	int m_iVersion{};
	BOOL m_bEnableMid{};
// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG };
	CButton	m_chkBOLD3;
	CButton	m_chkBOLD2;
	CButton	m_chkSHL;
	CButton	m_chkDBONG;
	CButton	m_chkSGRP;
	CButton	m_chkDYUL;
	CButton	m_chkCURR;
	CButton	m_chkBOLD;
	CButton	m_chkMIDPRC;
	//}}AFX_DATA

	CButton m_radios[9]{};
	CFont m_boldFont;
	CString   m_sRoot{};
	CBitmap m_bitmapLeft{};
	CBitmap m_bitmapRight{};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	afx_msg void OnChkdyul();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChkcurr();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	struct _config	*m_pConfig;
	struct _config* m_pOldconfig{};
public:
	afx_msg void OnBnClickedDefault();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedLrNo();
	afx_msg void OnBnClickedLrGraph();
	afx_msg void OnBnClickedLrChe();
	afx_msg void OnBnClickedRrNo();
	afx_msg void OnBnClickedRrSgjj();
	afx_msg void OnBnClickedRrPivot();
	afx_msg void OnBnClickedRrCurr();
	afx_msg void OnBnClickedRrJvi();
	afx_msg void OnBnClickedRrMid();
};