#if !defined(AFX_CONTROLWND_H__2CD65963_B85D_42E9_AA07_552C89E6B7EC__INCLUDED_)
#define AFX_CONTROLWND_H__2CD65963_B85D_42E9_AA07_552C89E6B7EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlWnd.h : header file
//

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(CWnd* pParent, _param* pParam);

// Attributes
protected:
	CWnd*	m_pParent;
	CString	m_name;
	int	m_key;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	CString m_strCode;
	afx_msg void OnStrCodeChanged();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLWND_H__2CD65963_B85D_42E9_AA07_552C89E6B7EC__INCLUDED_)
