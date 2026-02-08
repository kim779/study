#if !defined(AFX_TOOLTIPDLG_H__FC9C5C58_4435_4AEC_BB28_7DA891E6026B__INCLUDED_)
#define AFX_TOOLTIPDLG_H__FC9C5C58_4435_4AEC_BB28_7DA891E6026B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TooltipDlg.h : header file
//
#include "RscDlg.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTooltipDlg dialog

class CTooltipDlg : public CRscDlg
{
// Construction
public:
	CTooltipDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL ShowWindow(int nCmdShow);

// Dialog Data
	//{{AFX_DATA(CTooltipDlg)
	enum { IDD = IDD_DIGTOOLTIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTooltipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	int  m_nLastCmdShow;
protected:

	// Generated message map functions
	//{{AFX_MSG(CTooltipDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPDLG_H__FC9C5C58_4435_4AEC_BB28_7DA891E6026B__INCLUDED_)
