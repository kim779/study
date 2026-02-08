#pragma once
// XTab.h : header file
//
#define	WM_XMSG3		WM_USER+779

/////////////////////////////////////////////////////////////////////////////
// CXTab window

class CBase;

class CXTab : public CWnd
{
// Construction
public:
	CXTab(CWnd* base, CString home, CString user);
	virtual ~CXTab();

// Attributes
protected:
	int		m_cur;
	int		m_dir;
	int		m_index;

	CBase*		m_base;
	CString		m_home;
	CString		m_user;
	CRect		m_dRc;
	CFont		m_font;
	COLORREF	m_sColor;
	COLORREF	m_bColor;
	CStringArray	m_list;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	SetSkinColor(COLORREF bColor, COLORREF sColor, bool draw = true);
	void	LoadInfo();
	void	ClearSelect();
	void	ChangeFont(int ifontsize, CString sFont);
	// Generated message map functions
protected:
	void	DrawButton(bool calculate = true);
	void	DrawExpand(CRect eRc, bool expand);
	void	Drawdirect(CRect eRc, int dir, bool push = true);
	bool	CalculateHideButton(int& visibleN);
	//{{AFX_MSG(CXTab)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


