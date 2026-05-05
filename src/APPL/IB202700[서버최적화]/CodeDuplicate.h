#pragma once
// CodeDuplicate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeDuplicate dialog

class CCodeDuplicate : public CDialog
{
// Construction
public:
	CCodeDuplicate(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCodeDuplicate)
	enum { IDD = IDD_DLG_CODDULICATE };
	//}}AFX_DATA

	BOOL m_bAlert;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeDuplicate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCodeDuplicate)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

