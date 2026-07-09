#if !defined(AFX_ACCNADMINDLG_H__83CF2E9A_5480_4E51_BF09_53585BFE142A__INCLUDED_)
#define AFX_ACCNADMINDLG_H__83CF2E9A_5480_4E51_BF09_53585BFE142A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccnAdminDLG.h : header file
//

#include "AccnTreeDLG.h"
#include "AccnInterestDLG.h"

/////////////////////////////////////////////////////////////////////////////
// CAccnAdminDLG dialog

class CAccnAdminDLG : public CDialog
{
// Construction
public:
	CAccnAdminDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccnAdminDLG)
	enum { IDD = IDD_ACCN_ADMIN };
	CTabCtrl	m_Tab;
	//}}AFX_DATA

	CAccnTreeDLG*		m_accnTreeDlg;
	CAccnInterestDLG*	m_accnInterestDlg;

	int			m_nTabIndex;

	void	SetTab(int index);
	BOOL	SetTreeAccn(int vsn);
	void	RedrawCtrl(bool bResize);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccnAdminDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccnAdminDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCNADMINDLG_H__83CF2E9A_5480_4E51_BF09_53585BFE142A__INCLUDED_)
