#if !defined(AFX_BASE_H__E2688868_C80F_4FA7_9279_7C2E6AA71F13__INCLUDED_)
#define AFX_BASE_H__E2688868_C80F_4FA7_9279_7C2E6AA71F13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// base.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CBase window

class CBase : public CWnd
{
// Construction
public:
	CBase();
	virtual ~CBase();

// Attributes
protected:
	CWnd*		m_parent;

	CString		m_home;
public:
// Operations
public:
	BOOL	create_Map(CWnd*, CRect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBase)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	CString	variant(int comm, CString data = _T(""));
	//{{AFX_MSG(CBase)
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg	LONG OnUSER(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASE_H__E2688868_C80F_4FA7_9279_7C2E6AA71F13__INCLUDED_)
