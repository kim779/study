#if !defined(AFX_MAINWND_H__CF5B77B1_F2CD_4E0E_BFE8_3177CCF8B8FF__INCLUDED_)
#define AFX_MAINWND_H__CF5B77B1_F2CD_4E0E_BFE8_3177CCF8B8FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
class CControlWnd;
class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();

// Attributes
public:
	CControlWnd*	m_pControl;
	struct	_param* m_param;
	CWnd*			m_pAxisWnd;

// Operations
public:
	void			SetParam(struct	_param* param, CWnd *parent);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__CF5B77B1_F2CD_4E0E_BFE8_3177CCF8B8FF__INCLUDED_)
