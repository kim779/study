#if !defined(AFX_CONFIGDLG_H__8FED3F52_3B9E_44BB_8158_1475C76B0EC0__INCLUDED_)
#define AFX_CONFIGDLG_H__8FED3F52_3B9E_44BB_8158_1475C76B0EC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// configDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(CWnd* pParent = NULL, CString root = _T(""));   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	int	m_interval;
	int	m_CONinterval;
	bool	m_flash;
	CString	m_waveF;

	void SearchFile();
	void Soundplay();

protected:
	CString	m_path;
	DWORD	m_status;

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
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnScreenKillFocus();
	afx_msg void OnIntervalKillFocus();
	afx_msg void OnMdconSearch();
	afx_msg void OnMdconPlay();
	afx_msg void OnMdcon();
	//}}AFX_MSG
	afx_msg void OnEditScreenCount(UINT cmdID);
	afx_msg void OnEditInterval(UINT cmdID);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLG_H__8FED3F52_3B9E_44BB_8158_1475C76B0EC0__INCLUDED_)
