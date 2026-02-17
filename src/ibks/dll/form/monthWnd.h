#if !defined(AFX_MONTHWND_H__44758D81_8122_4B78_B986_36049A13532B__INCLUDED_)
#define AFX_MONTHWND_H__44758D81_8122_4B78_B986_36049A13532B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// monthWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CmonthWnd window

class CmonthWnd : public CWnd
{
// Construction
public:
	CmonthWnd();

// Attributes
public:
	class	Cmonth*	m_month;

protected:
	CWnd*	m_parent;
	CString	m_string;

// Operations
public:
	BOOL	Create(CWnd* parent, CRect rect, class CfmEdit* edit);
	void	ShowMonthCal(CRect rect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CmonthWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CmonthWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CmonthWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg	LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONTHWND_H__44758D81_8122_4B78_B986_36049A13532B__INCLUDED_)
