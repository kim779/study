#pragma once
// GRProcDlg.h : header file
//

#include "MapForm.h"

/////////////////////////////////////////////////////////////////////////////
// CGRProcDlg dialog

class CGRProcDlg : public CDialog
{
// Construction
public:
	CMapForm* m_pParentWnd;
	CGRProcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGRProcDlg)
	enum { IDD = IDD_GRPROC_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGRProcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGRProcDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

