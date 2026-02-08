#pragma once
// NewGroup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewGroup dialog

class CNewGroup : public CDialog
{
// Construction
public:
		CNewGroup(CWnd* pParent = NULL, int dlgMode = 0, CString defName = _T(""));   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewGroup)
	enum { IDD = IDD_NEWGROUP };
	CEdit	m_grpname;
	//}}AFX_DATA

public:
	int	m_dlgMode;
	CString	m_defName;
	CString	m_title;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewGroup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

