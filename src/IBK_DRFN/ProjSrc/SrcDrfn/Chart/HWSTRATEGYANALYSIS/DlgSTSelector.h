#if !defined(AFX_DLGSTSELECTOR_H__D785C1E5_B441_45C8_BEEC_7AE3B7A3A7EB__INCLUDED_)
#define AFX_DLGSTSELECTOR_H__D785C1E5_B441_45C8_BEEC_7AE3B7A3A7EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSTSelector.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSTSelector dialog

class CDlgSTSelector : public CDialog
{
// Construction
public:
	CDlgSTSelector(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSTSelector)
	enum { IDD = IDD_DLG_STSELECTOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSTSelector)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSTSelector)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTSELECTOR_H__D785C1E5_B441_45C8_BEEC_7AE3B7A3A7EB__INCLUDED_)
