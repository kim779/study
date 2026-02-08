#if !defined(AFX_SETINDEXDLG_H__F2D5D823_6A81_4487_9273_085AA778E95A__INCLUDED_)
#define AFX_SETINDEXDLG_H__F2D5D823_6A81_4487_9273_085AA778E95A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetIndexDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetIndexDlg dialog

class CSetIndexDlg : public CDialog
{
// Construction
public:
	CSetIndexDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetIndexDlg)
	enum { IDD = IDD_DIALOG_SET_INDEX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetIndexDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetIndexDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETINDEXDLG_H__F2D5D823_6A81_4487_9273_085AA778E95A__INCLUDED_)
