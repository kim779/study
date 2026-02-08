#if !defined(AFX_OPTIMIZESHOWDLG_H__0E596C90_6E62_40DE_8DF5_D9B14A219D1E__INCLUDED_)
#define AFX_OPTIMIZESHOWDLG_H__0E596C90_6E62_40DE_8DF5_D9B14A219D1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptiMizeShowDlg.h : header file
//
//Dlg
#include "DlgResult.h"
#include "DlgSoIkChart.h"
#include "DlgSooikChart.h"
#include "DlgTrade.h"

//Skin And Control
#include "Control/XTabCtrl.h"
#include "Control/xSkinButton.h"
/////////////////////////////////////////////////////////////////////////////
// COptiMizeShowDlg dialog

class COptiMizeShowDlg : public CDialog
{
// Construction
public:
	COptiMizeShowDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptiMizeShowDlg)
	enum { IDD = IDD_DLG_SHOWOPT };
	CXTabCtrl	m_CtlTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptiMizeShowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptiMizeShowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//TabDlg
	CDlgResult		*m_DlgResult;
	CDlgSooikChart  *m_DlgSooik;
	CDlgSoIkChart   *m_DlgSonik;
	CDlgTrade		*m_DlgTrade;
	CArray<CDialog*,CDialog*> m_arrayTab;
	void InitTab();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIMIZESHOWDLG_H__0E596C90_6E62_40DE_8DF5_D9B14A219D1E__INCLUDED_)
