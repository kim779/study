#if !defined(AFX_DLGSHARESET_H__703BD581_B234_11D4_97B6_0048543A0D30__INCLUDED_)
#define AFX_DLGSHARESET_H__703BD581_B234_11D4_97B6_0048543A0D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShareSet.h : header file
//
#include "resource.h"

#define WND_STATE(localID,localSTATE)	GetDlgItem(localID)->EnableWindow(localSTATE)
/////////////////////////////////////////////////////////////////////////////
// CDlgShareSet dialog

class CDlgShareSet : public CDialog
{
// Construction
public:
	CDlgShareSet(CWnd* pParent = NULL);   // standard constructor

	BOOL m_bSName;
	void SetWndState(int nType);

// Dialog Data
	//{{AFX_DATA(CDlgShareSet)
	enum { IDD = IDD_DLGSHARESET };
	CEdit	m_ctrlSName;
	BOOL	m_bGet;
	BOOL	m_bSet;
	CString	m_szSharedName;
	BOOL	m_bStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShareSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShareSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSharedtype();
	afx_msg void OnChangeIndex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHARESET_H__703BD581_B234_11D4_97B6_0048543A0D30__INCLUDED_)
