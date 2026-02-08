#pragma once
// ConfigDlg.h : header file
//
#include "3DEdit.h"
#include "MapWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(CString file, CWnd* pParent = NULL);   // standard constructor

	struct _config	m_config;

	void	InitData();

	CString m_file;

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_DIALOG_CONFIG };
	C3DEdit	m_user;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	SetConfigData();
	
	void	SetData(CString major, CString minor, CString data, CString file);
	CString	GetDataString(CString major, CString minor, CString init, CString file);
	int	GetDataInt(CString major, CString minor, int init, CString file);

	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

