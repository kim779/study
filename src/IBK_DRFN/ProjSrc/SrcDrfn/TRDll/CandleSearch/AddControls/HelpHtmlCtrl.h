#if !defined(AFX_HELPHTMLCTRL_H__97976F6C_6976_4230_A06D_356B5B19FB12__INCLUDED_)
#define AFX_HELPHTMLCTRL_H__97976F6C_6976_4230_A06D_356B5B19FB12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpHtmlCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelpHtmlCtrl html view
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CHelpHtmlCtrl : public CHtmlView
{
public:
	CHelpHtmlCtrl();           // protected constructor used by dynamic creation
	~CHelpHtmlCtrl();

	static CHelpHtmlCtrl* GetHtmlView();
	BOOL CreateWnd(CRect rcWnd, CWnd *pParent, UINT nID);
	
	virtual void PostNcDestroy() {}
	DECLARE_DYNCREATE(CHelpHtmlCtrl)

// html Data
public:
	//{{AFX_DATA(CHelpHtmlCtrl)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpHtmlCtrl)
	public:
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CHelpHtmlCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPHTMLCTRL_H__97976F6C_6976_4230_A06D_356B5B19FB12__INCLUDED_)
