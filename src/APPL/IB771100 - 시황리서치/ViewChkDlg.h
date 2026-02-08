#pragma once
// ViewChkDlg.h : header file
//
#include "MapDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CViewChkDlg dialog
class CMapDlg;

class CViewChkDlg : public CDialog
{
// Construction
public:
	CViewChkDlg(CWnd* pParent, CWnd* pWizard);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewChkDlg)
	enum { IDD = IDD_DLG_VIEWSET };
	BOOL	m_bChkAutoTrigger;
	BOOL	m_bChkOnlyjongmok;
	BOOL	m_bChkInitSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewChkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	CWnd*	m_pParent;
	CWnd*	m_pWizard;

	CString m_szRootDir, m_szUser;

	CString GetAxVariant(int cmd, CString data=_T(""));
	void SaveScreenView();
	void loadScreenView();

protected:

	// Generated message map functions
	//{{AFX_MSG(CViewChkDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInitsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

