#pragma once
// Pane.h : header file
//

#include <afxcoll.h>

#define	WM_XMSG		(WM_USER + 777)

/**
#define	EX_NONE		0
#define	EX_VERT		1
#define	EX_HORI		2
**/

#define	STAT_NONE	0
#define	STAT_PUSH	1
#define	STAT_HIDE	2

#define BM_ACTIVE	0
#define BM_INACTIVE	1

#define BM_LEFT		0
#define BM_BODY		1
#define BM_RIGHT	2

struct TabList
{
	CRect rc;
	int		iTab;

	TabList(CRect r, int i)
	{
		rc.CopyRect(&r);
		iTab = i;
	}
	TabList()
	{
		iTab = 0;
		rc = CRect(0, 0, 0, 0);
	}

};
typedef CArray<TabList, TabList> CTabListArray;


class CBase;
/////////////////////////////////////////////////////////////////////////////
// CPane window
class CBase;
class CPane : public CWnd
{
// Construction
public:
	CPane(CWnd* parent, CBase* base, CString home, int type, int pos);

// Attributes
public:
	int			m_key;

// Operations
public:
	//CWnd*		m_screen;
	std::unique_ptr<CWnd> m_screen;
	BOOL		m_bExpendClick, m_bTabClick;
	int			m_iLeft, m_iTop, m_iTabBottom, m_iVSpace, m_iHSpace;
	int			m_iexLastIdx;
	COLORREF	m_TabLineColor;
	CRect		m_pExpendRect;

	void		LoadRoundBitmap();
	void		ChangePallete();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPane)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
// attributes
protected:
	
	int			m_cur;
	int			m_pos;
	int			m_type;
	//**int			m_exM;
	int			m_index;
	int			m_dir;
	int			m_stat[2];

	bool		m_nosize;
	CString		m_home;

	CRect		m_rect;
	CRect		m_dRc;

	CWnd*		m_parent;
	CBase*		m_base;
	CFont*		m_font;
	COLORREF	m_bColor;
	COLORREF	m_sColor;
	COLORREF	m_btnColor;
	COLORREF	m_sbtnColor;
	CStringArray	m_list;
	HBITMAP		m_hRoundBitmap[8];

	CTabListArray	m_arTabList;
	int				m_iSelectTab, m_iTabExBtnSize;
	
// methods
protected:
	
	void	SetIndex();
	void	DrawButton(CDC* pDC = NULL);
	void	DrawExpand(CRect eRc, UINT expandStat);
	void	DrawDirect(CRect eRc, int dir, bool push = true);

	CString		GetShortName(CString mapN);
	bool		IsExpand(CPoint point, CRect eRc);


	void	DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);
	void	DrawCornerRoundBitmap(CDC* pDC, CRect rc);

	void	RedrawTabButton();
	void	SelectTabbutton(int index, bool bInit=false);



public:
	void	Initialize();
	void	Initialize(int type, int pos);
	void	Release();
	
	void	LoadScreen();
	void	LoadScreen(CString mapN);

	void	SetSkinColor(COLORREF color, bool draw = true);
	void	SetScreen(CString dat);
	void	SetRect(CRect rc) { m_rect.CopyRect(&rc); }

	BYTE	GetType();
	CSize	GetMapInfo();
	CRect	GetRect()	const { return m_rect; }
	int		GetPos()	const { return m_pos; }
	int		GetKey()	const { return m_key; }

	void	ChangeFont(int pointfont, CString fontName);
	void	ChangeSize(int cx, int cy);
	void SaveLog(CString sData);

};

/////////////////////////////////////////////////////////////////////////////

