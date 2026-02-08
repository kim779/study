#if !defined(AFX_DLGGROUPNAME_H__550B6EAF_42FF_4992_A703_2FF779BEA3FE__INCLUDED_)
#define AFX_DLGGROUPNAME_H__550B6EAF_42FF_4992_A703_2FF779BEA3FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGroupName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGroupName dialog

class CDlgGroupName : public CDialog
{
// Construction
public:
	CDlgGroupName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGroupName)
	enum { IDD = IDD_DLGGROUPNAME };
	CString	m_szName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGroupName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGroupName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGROUPNAME_H__550B6EAF_42FF_4992_A703_2FF779BEA3FE__INCLUDED_)
