#if !defined(AFX_ETICKER_H__66124481_AE92_4AF7_933F_863893C2324E__INCLUDED_)
#define AFX_ETICKER_H__66124481_AE92_4AF7_933F_863893C2324E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// eticker.h : header file
//
#define BUTTON_OK	0x01
#define BUTTON_CANCEL	0x02
#define BUTTON_ALL	BUTTON_OK|BUTTON_CANCEL

#include "usedefine.h"

class _Index
{
public:
	CString	Code;
	CString	Name;
};
/////////////////////////////////////////////////////////////////////////////
// CETicker window
class CETicker : public CWnd
{
// Construction
public:
	CETicker(CWnd* parent, int kind, int id, CString home, CString usrName, bool (*axiscall)(int, WPARAM, LPARAM));
	virtual ~CETicker();

// Attributes
protected:
	CWnd*	m_parent;
	CFont	m_font;
	int	m_kind;
	int	m_id;
	CRect	m_btnRect[2];
	CString	m_home;
	CString	m_userName;
	int	m_down;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
	
	CObArray	m_ary;
	CMapStringToString	m_arCodeByIndex;
#ifdef DF_USESTL_ETICKER
	std::unique_ptr<class CReportCtrl> m_report;
#else 
	class CReportCtrl* m_report;
#endif
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETicker)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	Init();
	// Generated message map functions
protected:
	void	loadItem();
	void	loadINDEX();
	void	loadFCODE();

	void	draw_Btn(CDC* pDC = NULL, int idx = BUTTON_ALL, bool bDown = false);
	void	save();
	//{{AFX_MSG(CETicker)
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETICKER_H__66124481_AE92_4AF7_933F_863893C2324E__INCLUDED_)
