#if !defined(AFX_NOSYSTEMMENUEDIT_H__8FC275F3_F8E1_4980_BE2C_7F04DBAD8C4E__INCLUDED_)
#define AFX_NOSYSTEMMENUEDIT_H__8FC275F3_F8E1_4980_BE2C_7F04DBAD8C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoSystemMenuEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoSystemMenuEdit window

class CNoSystemMenuEdit : public CEdit
{
// Construction
public:
	CNoSystemMenuEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoSystemMenuEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNoSystemMenuEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNoSystemMenuEdit)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOSYSTEMMENUEDIT_H__8FC275F3_F8E1_4980_BE2C_7F04DBAD8C4E__INCLUDED_)
