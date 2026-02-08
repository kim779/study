#if !defined(AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_)
#define AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RscDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRscDlg dialog

class CRscDlg : public CDialog
{
// Construction
public:
	CRscDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL ); // standard constructor


// Dialog Data
	//{{AFX_DATA(CRscDlg)
	enum { IDD = NULL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRscDlg)
	public:
	virtual BOOL Create(UINT nID,CWnd* pParentWnd);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRscDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_)
