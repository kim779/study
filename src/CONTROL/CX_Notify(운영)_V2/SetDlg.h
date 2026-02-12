#pragma once
// SetDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog

class CSetDlg : public CDialog
{
// Construction
public:
	CSetDlg(CWnd* pParent = NULL);   // standard constructor
	CString		m_sData;
// Dialog Data
	//{{AFX_DATA(CSetDlg)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_SetList;
	CListCtrl	m_OrgList;
	int		m_gubnRdo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddBtn();
	virtual void OnOK();
	afx_msg void OnDblclkOrgList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSetList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdoClick();
	afx_msg void OnDelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};