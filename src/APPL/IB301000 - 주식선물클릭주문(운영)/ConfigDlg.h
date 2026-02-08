#pragma once

#include "AxisExt.h"

class CConfigDlg : public CDialog
{
public:
	CConfigDlg(CWnd* pParent, CString profile, CAxisExt *pAxisExt);   // standard constructor

public:
	CString m_profile;
	CAxisExt *m_pAxisExt;

	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG_DLG };
	UINT	m_rsvd_width;
	UINT	m_jumn_width;
	UINT	m_size_width;
	UINT	m_remain_width;
	UINT	m_hoga_width;
	UINT	m_rate_width;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CConfigDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnOdrConfirm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

