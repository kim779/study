#pragma once
// MultiAccSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMultiAccSetDlg dialog

class CMultiAccSetDlg : public CDialog
{
// Construction
public:
	CMultiAccSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiAccSetDlg)
	enum { IDD = IDD_MULTIACCSET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiAccSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultiAccSetDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

