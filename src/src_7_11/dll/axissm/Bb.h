#if !defined(AFX_BB_H__9A8C3D61_723C_4FA0_A307_5A65FDFA12C0__INCLUDED_)
#define AFX_BB_H__9A8C3D61_723C_4FA0_A307_5A65FDFA12C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Bb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBb window

#define	IMAX		11
#define	IMG_GAP		8

class AFX_EXT_CLASS CBb : public CWnd
{
// Construction
public:
	CBb(bool (*axiscall)(int, WPARAM, LPARAM), CString home, COLORREF color = GetSysColor(COLOR_APPWORKSPACE));
	virtual ~CBb();

// Attributes
public:
	class CBCtrl1*	m_ctrl1;
	class CBCtrl2*	m_ctrl2;
	class CBCtrl3*	m_ctrl3;

protected:
	int		m_hover;
	int		m_cursor;
	int		m_bIndex;
	int		m_bCnt;
	int		m_delay;
	bool		m_mouseIN;
	bool		m_bList;
	CString		m_home;
	CString		m_bottoms[5];
	COLORREF	m_bk;

	CRect		m_imgRc[IMAX+1];
	CRect		m_imgRc2[5];
	CRect		m_closeRc;

	bool	(*m_axiscall)(int, WPARAM, LPARAM);
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBb)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	init();
	void	fit();
	void	Toggle();
	BOOL	GetReload(int id);
	void	SetLastTime(CString time);

	// Generated message map functions
protected:
	void	make_Ctrl();
	void	draw_Img(CDC* pDC);
	void	draw_Benner(CDC* pDC = NULL);
	void	draw_CoolImg(CDC* pDC = NULL);
	void	draw_HotImg(int index, CDC* pDC = NULL);
	void	draw_Etc(CDC* pDC);

	void	HoverItem(CPoint point);
	CString	ChangeCursor(CPoint);
	int	NextBenner(CPoint point);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CBb)
	afx_msg void OnChildActivate();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BB_H__9A8C3D61_723C_4FA0_A307_5A65FDFA12C0__INCLUDED_)
