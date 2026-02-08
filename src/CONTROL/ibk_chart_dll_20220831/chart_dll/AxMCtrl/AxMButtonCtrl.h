#if !defined(AFX_AXMBUTTONCTRL_H__0CFD4027_B6A0_47AB_B2E2_E8370E2B221C__INCLUDED_)
#define AFX_AXMBUTTONCTRL_H__0CFD4027_B6A0_47AB_B2E2_E8370E2B221C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMButtonCtrl.h : header file
//

#include "AxButton.h"

/////////////////////////////////////////////////////////////////////////////
// CAxMButtonCtrl window

class AFX_EXT_CLASS CAxMButtonCtrl : public CAxShadeButton
{
// Construction
public:
	CAxMButtonCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMButtonCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMButtonCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMButtonCtrl)
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_down;
protected:
	CWnd*	m_messageHandler;
	LRESULT Notify(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
public:
	CWnd*	GetMessageHandler() const { return m_messageHandler; }
	void	SetMessageHandler(CWnd* wnd) { m_messageHandler = wnd; }
public:
	void	SetDown(BOOL down);
	BOOL	GetDown();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMBUTTONCTRL_H__0CFD4027_B6A0_47AB_B2E2_E8370E2B221C__INCLUDED_)
