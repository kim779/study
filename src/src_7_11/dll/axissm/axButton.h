#if !defined(AFX_AXBUTTON_H__DDCFE205_42AC_435D_A738_6F6E96C76522__INCLUDED_)
#define AFX_AXBUTTON_H__DDCFE205_42AC_435D_A738_6F6E96C76522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// axButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxButton window

class CAxButton : public CButton
{
// Construction
public:
	CAxButton(int buttonID = -1, bool repeatLBDN = false);

// Attributes
protected:
	CSize   m_szIcon;

	CRect	m_rcButton;
	CRect	m_rcIcon;

	bool	m_mouseDN;
	bool	m_flate;
	bool	m_hover;

	int	m_buttonID;
	bool	m_repeatLBDN;

	COLORREF m_bkGround;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void SetIcon(HICON hIcon,CSize cSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	void	disableFlate() { m_flate = false; }
	void	disableHover() { m_hover = false; }
	void	setIconRect();
	void	setIconColor(COLORREF bkGround) { m_bkGround = bkGround; }
	virtual ~CAxButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXBUTTON_H__DDCFE205_42AC_435D_A738_6F6E96C76522__INCLUDED_)
