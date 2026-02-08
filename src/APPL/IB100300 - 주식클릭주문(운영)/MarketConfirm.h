#pragma once
// MarketConfirm.h : header file
//
#include "ControlWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CMarketConfirm dialog

class CMarketConfirm : public CDialog
{
// Construction
public:
	CMarketConfirm(CControlWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMarketConfirm)
	enum { IDD = IDD_DIALOG_MARKET };
	CButton	m_chkMarket;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarketConfirm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CControlWnd*	m_pCtlWnd;

protected:

	// Generated message map functions
	//{{AFX_MSG(CMarketConfirm)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

