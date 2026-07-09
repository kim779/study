#if !defined(AFX_SMSHEET_H__F1200946_E1AB_456A_9061_D96807EE9FE3__INCLUDED_)
#define AFX_SMSHEET_H__F1200946_E1AB_456A_9061_D96807EE9FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmSheet

class CSmSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSmSheet)

// Construction
public:
	CSmSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSmSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSmSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSmSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSHEET_H__F1200946_E1AB_456A_9061_D96807EE9FE3__INCLUDED_)
