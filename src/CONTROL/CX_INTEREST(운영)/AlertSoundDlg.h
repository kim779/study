#pragma once
// AlertSoundDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlertSoundDlg dialog

class CAlertSoundDlg : public CDialog
{
// Construction
public:
	CAlertSoundDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlertSoundDlg)
	enum { IDD = IDD_NEWS_ALERT };
	CButton	m_cbApply;
	CStatic	m_staticFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlertSoundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	CString m_strFile;
	BOOL	m_bApply;

// Operators
private:
	void EnableConrol(BOOL bEnable);
	void PlayWave(CString strFile);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlertSoundDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckApply();
	afx_msg void OnButtonFile();
	afx_msg void OnButtonListen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};