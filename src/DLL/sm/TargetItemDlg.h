#if !defined(AFX_TARGETITEMDLG_H__C73E64F5_E1FB_4927_8E4B_61A6D83EC1B3__INCLUDED_)
#define AFX_TARGETITEMDLG_H__C73E64F5_E1FB_4927_8E4B_61A6D83EC1B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TargetItemDlg.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTargetItemDlg dialog

class CTargetItemDlg : public CDialog
{
// Construction
public:
	CTargetItemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTargetItemDlg)
	enum { IDD = IDD_TARGETITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTargetItemDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARGETITEMDLG_H__C73E64F5_E1FB_4927_8E4B_61A6D83EC1B3__INCLUDED_)
