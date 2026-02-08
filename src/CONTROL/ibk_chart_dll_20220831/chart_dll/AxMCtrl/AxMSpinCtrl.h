#if !defined(AFX_AXMSPINCTRL_H__A07C8FBC_2305_47AE_9D1A_8C289C31E3D9__INCLUDED_)
#define AFX_AXMSPINCTRL_H__A07C8FBC_2305_47AE_9D1A_8C289C31E3D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMSpinCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxMSpinCtrl window

class AFX_EXT_CLASS CAxMSpinCtrl : public CSpinButtonCtrl
{
// Construction
public:
	CAxMSpinCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMSpinCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMSpinCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMSpinCtrl)
	LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	BOOL OnDeltapos(NMHDR*, LRESULT*);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	CWnd* m_messageHandler;
	LRESULT Notify(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
public:
	void	SetMessageHandler(CWnd* wnd);
	CWnd*	GetMessageHandler() const { return m_messageHandler; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMSPINCTRL_H__A07C8FBC_2305_47AE_9D1A_8C289C31E3D9__INCLUDED_)
