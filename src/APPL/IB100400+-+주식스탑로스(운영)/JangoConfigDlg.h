#pragma once
// JangoConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJangoConfigDlg dialog

class CJangoConfigDlg : public CDialog
{
// Construction
public:
	CJangoConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJangoConfigDlg)
	enum { IDD = IDD_JANGOCONFIGDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJangoConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJangoConfigDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

