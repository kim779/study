#if !defined(AFX_ADDQUERYDLG_H__C376D095_57B2_408F_8EB2_8E1E775B0686__INCLUDED_)
#define AFX_ADDQUERYDLG_H__C376D095_57B2_408F_8EB2_8E1E775B0686__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddQueryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddQueryDlg dialog

class CAddQueryDlg : public CDialog
{
// Construction
public:
	CAddQueryDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL	m_bRename;
	CString	m_strQueryName;

	void	SetQueryName(CString strName)	{ m_strQueryName = strName;	};
	CString	GetQueryName()		{ return m_strQueryName;	};

public:

// Dialog Data
	//{{AFX_DATA(CAddQueryDlg)
	enum { IDD = IDD_ADDQUERYDLG };
	CEdit	m_editQryName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddQueryDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDQUERYDLG_H__C376D095_57B2_408F_8EB2_8E1E775B0686__INCLUDED_)
