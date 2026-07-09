#if !defined(AFX_MYFINDDLG_H__6A14273E_EB1E_49B6_AA8B_3CCD4FDE23CD__INCLUDED_)
#define AFX_MYFINDDLG_H__6A14273E_EB1E_49B6_AA8B_3CCD4FDE23CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFindDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFindDlg dialog

class CMyFindDlg : public CFindReplaceDialog
{
// Construction
public:
	CMyFindDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyFindDlg)
	enum { IDD = IDD_MYFINDDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyFindDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFINDDLG_H__6A14273E_EB1E_49B6_AA8B_3CCD4FDE23CD__INCLUDED_)
