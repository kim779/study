#if !defined(AFX_DLGLOADSCRIPT_H__93E29DCC_3192_4A1D_B7A6_F29D3CB27386__INCLUDED_)
#define AFX_DLGLOADSCRIPT_H__93E29DCC_3192_4A1D_B7A6_F29D3CB27386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLoadScript.h : header file
//

#include "../common_ST/Label.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadScript dialog

class CDlgLoadScript : public CDialog
{
// Construction
public:
	CDlgLoadScript(LPCSTR lpszName, CWnd* pParent = NULL);   // standard constructor

	BOOL	VerifyScriptName();

// Dialog Data
	//{{AFX_DATA(CDlgLoadScript)
	enum { IDD = IDD_DLG_LOADSCRIPT };
	CXTButton	m_btnOK;
	CXTButton	m_btnCancel;
	CLabel	m_stsResult;
	CEdit	m_editName;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoadScript)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoadScript)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOADSCRIPT_H__93E29DCC_3192_4A1D_B7A6_F29D3CB27386__INCLUDED_)
