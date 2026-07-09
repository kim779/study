#if !defined(AFX_IMENU_H__4C830FED_E1FF_441D_9E13_F26114FA939A__INCLUDED_)
#define AFX_IMENU_H__4C830FED_E1FF_441D_9E13_F26114FA939A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// iMenu.h : header file
//
#include <afxtempl.h>
#include <afxcoll.h>
#include "MenuXP.h"

#define	MAX	5
#define	WEB	"WWW#"

#define M_COOL	0
#define M_OVER	1
#define M_DOWN	2

class _itemM
{
public:
	_itemM()
	{
		isWeb = FALSE;
	}

	BOOL		isWeb;
	CString		disp;	// display name
	CString		mapN;	// map name
	CString		sName;	// short name
	CString		disN;	// display number(4)
	CString		info;	// info (url.......)
};
/////////////////////////////////////////////////////////////////////////////
// CIMenu window

class CIMenu : public CWnd
{
// Construction
public:
	CIMenu(CWnd* parent, CString home, int cmdBase);
	virtual ~CIMenu();

// Attributes
public:
protected:
	CWnd*	m_parent;
	CFont	m_font;

	int	m_commandBase;
	int	m_commandCnt;
	int	m_hover;
	int	m_showMenuN;
	int	m_hideMenuN;
	int	m_moveMenuN;
	bool	m_mouseIN;
	bool	m_showPopup;
	bool	m_menuKeyOn;

	CPoint	m_curPoint;
	CRect	m_expandRc;
	CBitmap	m_back;
	CMenuXP*	m_popM[MAX];
	CMenuXP*	m_pMenu;

	CString	m_expandSYM;
	CString	m_hKey;
	CString	m_home;
	CString	m_menuN[MAX];

	CStringArray		m_arMap;
	CStringArray		m_arsMenuT;
	CMapStringToOb		m_arItem;
	CMapStringToString	m_maplist;
	CMapStringToString	m_mapedlist;

	CArray	< CRect, CRect>	m_arItemRect;
	CArray	< CMenuXP*, CMenuXP* > m_arMenu;
	CArray	< CMenuXP*, CMenuXP* > m_arMenuT;
	CArray	< CMenuXP*, CMenuXP* > m_arFrameMenu;

	CArray	< int, int >	m_arIndex;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIMenu)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	MeasureMenuItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	BOOL	PreTranslateMsg(MSG* pMsg);
	BOOL	HookKeyboard(WPARAM wParam, LPARAM lParam);
	void	HookProcess(CPoint point);
	void	LoadFramemenu(UINT nIDResource);
	void	LoadMenufromfile();
	void	HookLbutton(CPoint point);
	void	HookSyskey();
	void	ShowFullMenu(CPoint pt, bool bTray = false);

	int	GetMCount()	{ return (int)m_arMap.GetSize(); }//return m_arItem.GetCount();

	BOOL	ExistMenu(CString mapN);
	BOOL	IsWeb(CString mapN);
	BOOL	IsMappinged(CString mapN);
	BOOL	GetSelectList(CString input, CStringArray& array);
	CString	GetURL(CString mapN);
	CString	GetSName(CString mapN);
	CString	GetDesc(CString mapN);
	CString	GetInfo(CString mapN);
	CString	GetMap(int index);
	CString	GetMap(CString dispN);
	CString	GetMapping(CString mapN);
	CString	GetDispN(CString mapN);

	CMenu*	GetPopupMenu(int index);
	void	makeLinkFile();

	// Generated message map functions
protected:
	void	drawMenu(CDC* pDC = NULL);
	void	drawBk(CDC* pDC);
	void	drawExpand(CDC* pDC, int mode = 0);
	void	drawSelectMenu(bool select = true);
	
	void	hoverMenu(CPoint point);
	void	processButtonDown(CPoint point);
	void	processArrow(TCHAR key = VK_LEFT);

	void	makeMenu(CString dat);
	CString	saveinfo(CString desc, CString dat);

	void	removeLinkFile();
	int	GetScreenPos(int idx = 0);

	//{{AFX_MSG(CIMenu)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangePopup(WPARAM wParam, LPARAM lParam);
	LRESULT OnSelectMenu(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMENU_H__4C830FED_E1FF_441D_9E13_F26114FA939A__INCLUDED_)
