#pragma once
// MultiJumunDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMultiJumunDlg dialog

class CMultiJumunDlg : public CDialog
{
// Construction
public:
	CMultiJumunDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiJumunDlg)
	enum { IDD = IDD_MULTIJUMUN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiJumunDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultiJumunDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

