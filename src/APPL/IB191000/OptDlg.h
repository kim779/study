#pragma once
// OptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptDlg dialog

class COptDlg : public CDialog
{
// Construction
public:
	COptDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptDlg)
	enum { IDD = IDD_DLG_OPT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int GetOption();
	void AddOption(int opt);
	void DelOption(int opt);
	void SetOption(int opt);
	void ClearOption();
};

