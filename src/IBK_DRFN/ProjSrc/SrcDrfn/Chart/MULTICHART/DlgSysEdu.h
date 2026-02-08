#if !defined(AFX_DLGSYSEDU_H__17FD63E0_ED4C_46a5_BC02_ED5CF9998FE6__INCLUDED_)
#define AFX_DLGSYSEDU_H__17FD63E0_ED4C_46a5_BC02_ED5CF9998FE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysEdu.h : header file
//
#include "RscDlg.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysEdu dialog

class CDlgSysEdu : public CRscDlg
{
// Construction
public:
	CDlgSysEdu(CWnd* pParent = NULL);   // standard constructor
	~CDlgSysEdu();

// Dialog Data
	//{{AFX_DATA(CDlgSysEdu)
	enum { IDD = IDD_DLG_SYSEDU };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysEdu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysEdu)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSEDU_H__17FD63E0_ED4C_46a5_BC02_ED5CF9998FE6__INCLUDED_)
