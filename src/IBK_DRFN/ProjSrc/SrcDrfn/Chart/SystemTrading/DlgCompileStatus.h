#if !defined(AFX_DLGCOMPILESTATUS_H__A67434F2_E87A_4844_8197_3C9C114B71CA__INCLUDED_)
#define AFX_DLGCOMPILESTATUS_H__A67434F2_E87A_4844_8197_3C9C114B71CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCompileStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCompileStatus dialog

class CDlgCompileStatus : public CDialog
{
// Construction
public:
	CDlgCompileStatus(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCompileStatus)
	enum { IDD = IDD_DLG_COMPILESTATUS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCompileStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCompileStatus)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMPILESTATUS_H__A67434F2_E87A_4844_8197_3C9C114B71CA__INCLUDED_)
