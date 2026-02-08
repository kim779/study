#pragma once
// ItemRtsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemRtsDlg dialog

class CItemRtsDlg : public CDialog
{
// Construction
public:
	CItemRtsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItemRtsDlg)
	enum { IDD = IDD_ITEMRTS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemRtsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemRtsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
