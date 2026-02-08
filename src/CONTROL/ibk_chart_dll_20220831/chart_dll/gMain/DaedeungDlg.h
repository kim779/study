#if !defined(AFX_DAEDEUNGDLG_H__2E164966_0FDF_437F_848B_0F882C8C0EB5__INCLUDED_)
#define AFX_DAEDEUNGDLG_H__2E164966_0FDF_437F_848B_0F882C8C0EB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DaedeungDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDaedeungDlg dialog

class CDaedeungDlg : public CDialog
{
// Construction
public:
	CDaedeungDlg(CString lgap, CString rgap);

	CString	GetLeftGap()	{ return m_leftGap;	}
	CString	GetRightGap()	{ return m_rightGap;	}

// Dialog Data
	//{{AFX_DATA(CDaedeungDlg)
	enum { IDD = IDD_DAEDEUNG };
	CEdit	m_leftGapCtrl;
	CString	m_leftGap;
	CString	m_rightGap;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaedeungDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDaedeungDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAEDEUNGDLG_H__2E164966_0FDF_437F_848B_0F882C8C0EB5__INCLUDED_)
