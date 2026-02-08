#if !defined(AFX_ACCSAVE_H__3165789C_F292_4E23_B733_C15B4E1D8571__INCLUDED_)
#define AFX_ACCSAVE_H__3165789C_F292_4E23_B733_C15B4E1D8571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccSave.h : header file
//
#define WM_SAVEACC	WM_USER+5555
/////////////////////////////////////////////////////////////////////////////
// CAccSave dialog

class CAccSave : public CDialog
{
// Construction
public:
	CAccSave(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccSave)
	enum { IDD = IDD_ACCSAVE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccSave)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd*	m_parent;
	// Generated message map functions
	//{{AFX_MSG(CAccSave)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LONG OnSaveACC(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCSAVE_H__3165789C_F292_4E23_B733_C15B4E1D8571__INCLUDED_)
