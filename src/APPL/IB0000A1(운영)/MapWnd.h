#pragma once
// MapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd();
	virtual ~CMapWnd();

// Attributes
public:
	CWnd*	m_parent;
	CString	m_home;
	class CMapForm*	m_form;
// Operations
public:
	BOOL	CreateMap(CWnd* parent);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
