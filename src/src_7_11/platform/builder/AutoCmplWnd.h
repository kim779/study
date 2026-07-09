#if !defined(AFX_AUTOCMPLWND_H__60FEF5E3_B699_439D_A6D3_F5EE92CE160F__INCLUDED_)
#define AFX_AUTOCMPLWND_H__60FEF5E3_B699_439D_A6D3_F5EE92CE160F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCmplWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoCmplWnd window

class CAutoCmplWnd : public CWnd
{
// Construction
public:
	CAutoCmplWnd();

// Attributes
public:

// Operations
public:

	BOOL Create(CWnd* pParentWnd = NULL);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCmplWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAutoCmplWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAutoCmplWnd)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCMPLWND_H__60FEF5E3_B699_439D_A6D3_F5EE92CE160F__INCLUDED_)
