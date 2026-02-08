#pragma once
// NewGroup.h : header file
//
#include "Page1.h"

/////////////////////////////////////////////////////////////////////////////
// CNewGroup dialog

class CNewGroup : public CDialog
{
// Construction
public:
	CNewGroup(CWnd* pParent = nullptr, int dlgMode = 0, CString defName = _T(""));   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewGroup)
	enum { IDD = IDD_NEWGROUP };
	CEdit	m_grpname;
	//}}AFX_DATA

// Attributes
public:
	int	m_dlgMode;
	CString	m_defName;
	CString	m_title;
	
	CWnd*	m_pParent;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewGroup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMaxtextGrpname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

