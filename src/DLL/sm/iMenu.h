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
		isWeb = isNew = isRec = isEdit = FALSE;
	}

	BOOL		isWeb, isNew, isRec, isEdit;
	CString		disp;	// display name
	CString		mapN;	// map name
	CString		sName;	// short name
	CString		disN;	// display number(4)
	CString		info;	// info (url.......)
	CString		menu1;	// menu big
	CString		menu2;  // menu middle
};
class CNButton;
/////////////////////////////////////////////////////////////////////////////
// CIMenu window

class CIMenu : public CWnd
{
// Construction
public:
	CIMenu(CWnd* parent, int cmdBase);
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
	CBitmap*	m_back;
	CMenuXP*	m_popM[MAX];
	CMenuXP*	m_pMenu;
	CMenuXP*	m_pNewMenu;

	CString	m_expandSYM;
	CString	m_hKey;
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

	HICON		m_hMedalIcon;
	HICON		m_hNewIcon;
	HICON		m_hWebIcon;
	HICON		m_hEditIcon;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIMenu)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight);
	void	MeasureMenuItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	BOOL	PreTranslateMsg(MSG* pMsg);
	BOOL	HookKeyboard(WPARAM wParam, LPARAM lParam);
	void	HookProcess(CPoint point);
	void	LoadFramemenu(UINT nIDResource);
	void	LoadMenufromfile();
	void	HookLbutton(CPoint point);
	void	HookSyskey();
	void	ShowFullMenu(CPoint pt);

	int	GetMCount()	{ return m_arMap.GetSize(); }//return m_arItem.GetCount();
	CMenu*	GetNewMenu()	{ return m_pNewMenu; }

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
	CString GetMenuName(CString mapN);

	CMenu*	GetPopupMenu(int index);
	void	makeLinkFile();

	//void	AddMenu(CString dat) { makeMenu(dat); }; //test AI
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
	_itemM*	saveinfo(CString desc, CString dat);
	_itemM*	saveinfo(CString desc, CString dat, CString menu1, CString menu2);

	void	removeLinkFile();
	int	GetScreenPos(int idx = 0);
	void WriteFile(char* pBytes, int nBytes);

	//{{AFX_MSG(CIMenu)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangePopup(WPARAM wParam, LPARAM lParam);
	LRESULT OnSelectMenu(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
//	 특정사용자에게만 메뉴 오픈
// 2006 11 08
	CString	m_id;
/////////////////////////////////////////////////////////////////////////////
private:
	CBitmap m_bitmapMenuTerminator;
	void drawTerm(CDC* dc, CRect rc);
	
	CBitmap* m_bmpMenumode[2][2];
	CBitmap* m_bmpExpand;
	CSize m_szMenuMode;
	CSize m_szMenuExpand;

	int m_nShow;

	CNButton* m_moveBar;
#ifdef DF_MK_CAPTION
	CRect m_rcSelectMarket;
	CNButton* m_btSelectMarket{};
	void ToggleMarketMenu();
#endif
	void DrawMenuMode(CDC* dc);
	CRect GetMenuModeRect();
	CRect GetToolbarHideRect();
	CRect GetXComRect();
	CNButton* CreateNButton(unsigned int id, const char* title, const char* button, BOOL eachButton);

	int m_menuMode; // 0: normal, 1: hide, show menumode button only
	int m_menuModeHover; // 0: normal 1: in hover state
	void ToggleMenuMode();

	void CheckMenuModeStatus(CPoint point);

	BOOL IsWebMenu(const char* menuName);
public:
	void change_Palette(CString skinN);
	CString GetCurrentSkinName();
	int GetMenuMode() const {
		return m_menuMode;
	}
	int GetSmallHeight();
};

#define BMS_UP		0
#define BMS_DOWN	1
#define BMS_NORMAL	0
#define BMS_HOVER	1


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMENU_H__4C830FED_E1FF_441D_9E13_F26114FA939A__INCLUDED_)
