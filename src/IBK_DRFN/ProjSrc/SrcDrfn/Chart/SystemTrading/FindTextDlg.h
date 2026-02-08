////////////////////////////////////////////////////////////////////////////
//	Declaration of the CFindTextDlg dialog, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINDTEXTDLG_H__0A901F01_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_)
#define AFX_FINDTEXTDLG_H__0A901F01_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_

#include "CEDef.h"
#include "EditCmd.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrystalTextView;

/////////////////////////////////////////////////////////////////////////////
// CFindTextDlg dialog

class CFindTextDlg : public CDialog
{
// Construction
public:
	CFindTextDlg(CCrystalTextView *pBuddy);

// Attributes
private:
	CCrystalTextView *m_pBuddy;

public:
	CPoint m_ptCurrentPos;

// Dialog Data
	//{{AFX_DATA(CFindTextDlg)
	enum { IDD = IDD_EDIT_FIND };
	CXTButton	m_btnCancel;
	CXTButton	m_btnFind;
	int		m_nDirection;
	BOOL	m_bMatchCase;
	CString	m_sText;
	BOOL	m_bWholeWord;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFindTextDlg)
	virtual void OnOK();
	afx_msg void OnChangeEditText();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTEXTDLG_H__0A901F01_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_)
