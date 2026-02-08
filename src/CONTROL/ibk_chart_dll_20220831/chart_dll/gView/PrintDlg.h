#if !defined(AFX_PRINTDLG_H__5C821C32_C946_4F6C_8F5D_AA2F0BE5406A__INCLUDED_)
#define AFX_PRINTDLG_H__5C821C32_C946_4F6C_8F5D_AA2F0BE5406A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintDlg.h : header file
//

#define	PRINT_CHART	1
#define	PRINT_WINDOW	2

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg dialog

class CPrintDlg : public CDialog
{
// Construction
public:
	CPrintDlg(CWnd* pParent = NULL);   // standard constructor

	int	m_select;
// Dialog Data
	//{{AFX_DATA(CPrintDlg)
	enum { IDD = IDD_PRINTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTDLG_H__5C821C32_C946_4F6C_8F5D_AA2F0BE5406A__INCLUDED_)
