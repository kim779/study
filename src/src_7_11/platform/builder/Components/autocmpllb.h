#if !defined(AFX_AUTOCMPLLB_H__60846307_BA18_4330_B86C_9278977697E1__INCLUDED_)
#define AFX_AUTOCMPLLB_H__60846307_BA18_4330_B86C_9278977697E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// autocmpllb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoCmplLB window

class CAutoCmplLB : public CListBox
{
// Construction
public:
	CAutoCmplLB();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCmplLB)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAutoCmplLB();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAutoCmplLB)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCMPLLB_H__60846307_BA18_4330_B86C_9278977697E1__INCLUDED_)
