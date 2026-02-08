#pragma once

#include "ControlWnd.h"
// MarketConfirm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarketConfirm dialog

class CMarketConfirm : public CDialog
{
// Construction
public:
	CMarketConfirm(CControlWnd* pParent = NULL);   // standard constructor

	CControlWnd* m_pCtlWnd;

// Dialog Data
	//{{AFX_DATA(CMarketConfirm)
	enum { IDD = IDD_DIALOG_MARKET };
	CButton	m_chkAgain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarketConfirm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMarketConfirm)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckAgain();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

