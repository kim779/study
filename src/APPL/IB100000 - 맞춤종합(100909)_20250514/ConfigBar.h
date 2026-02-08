#pragma once
// ConfigBar.h : header file
//
#define	MODE_DEFAULT	0
#define	MODE_EXPAND	1

#define	BAR_HEIGHT	24
#define	WM_XMSG2		WM_USER+778
/////////////////////////////////////////////////////////////////////////////
// CConfigBar window
#include <afxcoll.h>
class CConfigBar : public CWnd
{
// Construction
public:
	CConfigBar(CWnd* base, CString home, CString user);
	virtual ~CConfigBar();

// Attributes
protected:
	CWnd*		m_base;
	CString		m_home;
	CString		m_user;
	int		m_hover;
	int		m_push;
	int		m_mode;

	bool		m_mouseIN;
	bool		m_pushON;
	bool		m_menuS;

	CRect		m_iRc[4];
	CBitmap		m_son;
	CBitmap		m_soff;
	CBitmap		m_bitmap[3];

	COLORREF	m_bColor;

	CStringArray	m_arM;
	std::unique_ptr<class CXTab>	m_xTab;

	double		m_sizeRate[2];
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigBar)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void	LoadInfo(bool clear);
	int	GetMode() { return m_mode; }
	int	GetWidth();
	void	SetSkinColor(COLORREF clrBk, COLORREF clrTabBk, COLORREF clrTabS, bool draw = true);
	void	ChangeFont(int pointfont, CString fontName);

	// Generated message map functions
protected:
	void	DrawImg();
	void	DrawImg(int index, int stat = 0);
	void	HoverItem(CPoint point);
	void	Operation(int comm);
	void	MakeXTab();
	//{{AFX_MSG(CConfigBar)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

