#if !defined(AFX_USERINPUTDLG_H__22DBCEF3_13C6_459B_A6C3_3D94293BC29B__INCLUDED_)
#define AFX_USERINPUTDLG_H__22DBCEF3_13C6_459B_A6C3_3D94293BC29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserInputDlg dialog

class CUserInputDlg : public CDialog
{
// Construction
public:
	CUserInputDlg(CString str, CString title = "");

	CString	m_title;

	CString	GetDisplayString() { return m_Input; }
// Dialog Data
	//{{AFX_DATA(CUserInputDlg)
	enum { IDD = IDD_USRINPUT };
	CString	m_Input;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERINPUTDLG_H__22DBCEF3_13C6_459B_A6C3_3D94293BC29B__INCLUDED_)
