#pragma once

#include "resource.h"
// CSetupDialog dialog

class CSetupDialog : public CDialog
{
	DECLARE_DYNAMIC(CSetupDialog)

public:
	CSetupDialog(UINT idResource, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupDialog();

// Dialog Data
	enum { IDD = IDD_SETUPDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

public:
	UINT	m_resourceID;
	CString m_caption;
	virtual void DefaultSetup();
	virtual void ApplySetup();
};
