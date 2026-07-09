#if !defined(AFX_NCLOCK_H__5EFC6009_609E_4BC4_BC23_155930D601D1__INCLUDED_)
#define AFX_NCLOCK_H__5EFC6009_609E_4BC4_BC23_155930D601D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NClock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNClock window

class CNClock : public CWnd
{
// Construction
public:
	CNClock(CString home);
	virtual ~CNClock();

	void	Init();
	BOOL IsMini() { return m_bSmall; }
	void SetMini(BOOL bMini = FALSE);
// Attributes
protected:
	int	m_gap;
	CFont	m_font;
	CFont	m_font2;
	CString	m_home;
	WORD	m_langID;
	CBitmap	m_bitmap;
	BOOL	m_bSmall;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNClock)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	void	DrawImg(CDC* pDC);
	void	DrawClock(CDC* pDC);
	HRGN	DIBToRgn(HBITMAP hBmp,COLORREF BkColor,BOOL Direct);
	//{{AFX_MSG(CNClock)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCLOCK_H__5EFC6009_609E_4BC4_BC23_155930D601D1__INCLUDED_)
