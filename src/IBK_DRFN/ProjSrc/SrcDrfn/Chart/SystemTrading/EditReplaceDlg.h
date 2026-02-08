////////////////////////////////////////////////////////////////////////////
//	Declaration of the CEditReplaceDlg dialog, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITREPLACEDLG_H__0A901F03_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_)
#define AFX_EDITREPLACEDLG_H__0A901F03_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_

#include "CEDef.h"
#include "EditCmd.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrystalEditView;

/////////////////////////////////////////////////////////////////////////////
// CEditReplaceDlg dialog

class CEditReplaceDlg : public CDialog
{
// Construction
public:
	CEditReplaceDlg(CCrystalEditView *pBuddy);

// Attributes
private:
	CCrystalEditView *m_pBuddy;

	BOOL	m_bFound;
	CPoint	m_ptFoundAt;

public:
	BOOL	m_bEnableScopeSelection;
	CPoint	m_ptCurrentPos;
	CPoint	m_ptBlockBegin, m_ptBlockEnd;

// Dialog Data
	//{{AFX_DATA(CEditReplaceDlg)
	enum { IDD = IDD_EDIT_REPLACE };
	CXTButton	m_btnCancel;
	CXTButton	m_btnFind;
	CXTButton	m_btnReplaceAll;
	CXTButton	m_btnReplace;
	BOOL	m_bMatchCase;
	BOOL	m_bWholeWord;
	CString	m_sText;
	CString	m_sNewText;
	int		m_nScope;
	//}}AFX_DATA

// Operation
private:
	BOOL	DoHighlightText();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditReplaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditReplaceDlg)
	afx_msg void OnChangeEditText();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditReplace();
	afx_msg void OnEditReplaceAll();
	afx_msg void OnEditSkip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITREPLACEDLG_H__0A901F03_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_)
