#if !defined(AFX_SETYSHOWDLG_H__916C449C_BD99_4860_8773_F509DABD9CAB__INCLUDED_)
#define AFX_SETYSHOWDLG_H__916C449C_BD99_4860_8773_F509DABD9CAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetYShowDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetYShowDlg dialog

class CSetYShowDlg : public CDialog
{
// Construction
public:
	CSetYShowDlg(CWnd* pParent = NULL);   // standard constructor

public:
	int	m_iRet;

protected:

public:

protected:

public:
// Dialog Data
	//{{AFX_DATA(CSetYShowDlg)
	enum { IDD = IDD_DIALOG_SET_YSHOW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetYShowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetYShowDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETYSHOWDLG_H__916C449C_BD99_4860_8773_F509DABD9CAB__INCLUDED_)
