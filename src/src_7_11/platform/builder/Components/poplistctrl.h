#if !defined(AFX_POPLISTCTRL_H__EA9852E3_B2BA_40D9_9B20_639553057D2A__INCLUDED_)
#define AFX_POPLISTCTRL_H__EA9852E3_B2BA_40D9_9B20_639553057D2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopListCtrl.h : header file
//
#include "EditList.h"
/////////////////////////////////////////////////////////////////////////////
// CPopListCtrl window

class CPopListCtrl : public CWnd
{
// Construction
public:
	CPopListCtrl();

// Attributes
public:
	BOOL		m_fDone;
	CEditList	*m_pListCtrl;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	long OnMessage1(WPARAM wParam, LPARAM lParam);
	virtual ~CPopListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPopListCtrl)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPLISTCTRL_H__EA9852E3_B2BA_40D9_9B20_639553057D2A__INCLUDED_)
