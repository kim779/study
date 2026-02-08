#if !defined(AFX_DLGFINDSCRIPT_H__4A0975D2_613A_41A7_B737_E9065DC73282__INCLUDED_)
#define AFX_DLGFINDSCRIPT_H__4A0975D2_613A_41A7_B737_E9065DC73282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFindScript.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFindScript dialog

class CDlgFindScript : public CDialog
{
// Construction
public:
	CDlgFindScript(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFindScript)
	enum { IDD = IDD_DLG_FINDSCRIPT };
	CXTButton	m_btnCancel;
	CXTButton	m_btnFind;
	BOOL	m_bFindFunction;
	BOOL	m_bFindStrategy;
	BOOL	m_bMatchCase;
	BOOL	m_bWholeWord;
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFindScript)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFindScript)
	virtual void OnOK();
	afx_msg void OnChangeEditText();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFINDSCRIPT_H__4A0975D2_613A_41A7_B737_E9065DC73282__INCLUDED_)
