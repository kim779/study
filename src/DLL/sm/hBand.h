#if !defined(AFX_HBAND_H__4CEC94AE_46D2_4C12_A7F3_9476504337B7__INCLUDED_)
#define AFX_HBAND_H__4CEC94AE_46D2_4C12_A7F3_9476504337B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// hBand.h : header file
//

#define	ID_TOOLTIP	1

#define COLOR_ITCOOL	RGB(69,   69,    71)
#define COLOR_ITOVER	RGB(69,   69,    71)
#define COLOR_ITDOWN	RGB( 15,   15,   15)

#define	ID_ADDTOOL	1001
#define ID_DELTOOL	1002
#define ID_CANCELTOOL	1003

#define	MODE_COOL	0
#define	MODE_OVER	1
#define	MODE_DOWN	2
#define	MODE_DRAG	3

#include <afxtempl.h>
#include <algorithm>

#define	BAND_ITEM	0
#define	BAND_LIST	1

#include "usedefine.h"

class CBandItem
{
public:
	CBandItem() : draw(0) {}
public:
	CString	mapN;
	CString	tooltip;
	CString	text;
	int	kind{};
	int draw{};	// 0 : Normal, 1 : Reverse
};
/////////////////////////////////////////////////////////////////////////////
// CHBand window
class CHBand : public CWnd
{
// Construction
public:
	CHBand(bool (*axiscall)(int, WPARAM, LPARAM));
	virtual ~CHBand();

// Attributes
public:

#ifdef DF_USESTL_HBAND
	CArray	<std::shared_ptr<CBandItem> , std::shared_ptr<CBandItem>> m_arItem;
	CArray	<std::shared_ptr<CBandItem>, std::shared_ptr<CBandItem>> m_arsubItem;
#else
	CArray	<CBandItem*, CBandItem*> m_arItem;
	CArray	<CBandItem*, CBandItem*> m_arsubItem;
#endif
	
	
	CStringArray	m_commandList;

	// drag & drop
#ifdef DF_USESTL_HBAND
	std::unique_ptr<CImageList> m_pDragImage;
#else
	CImageList* m_pDragImage;
#endif
	
	BOOL		m_bDragEnter;
	int		m_nDragItem;
	HICON		m_hMedalIcon;
	HICON		m_hNewIcon;
	HICON		m_hWebIcon;
	HICON		m_hEditIcon;
private:
	int	m_hindex;
	int	m_dispN;
	int	m_hover;
	int	m_push;
	int	m_itemW;
	int m_itemH;
	int	m_expandW;

	bool	m_mouseIN;
	bool	m_pushON;
	bool	m_defaultColor;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	CFont	m_font;
	CRect	m_expandRc;
	
	COLORREF	m_itcolorCool;
	COLORREF	m_itcolorOver;
	COLORREF	m_itcolorDown;

	CBitmap*	m_bmpToolbox;
	CBitmap*		m_img[2];

#ifdef DF_USESTL_HBAND
	std::unique_ptr<CToolTipCtrl> m_ToolTip;
#else
	CToolTipCtrl*	m_ToolTip;
#endif

// Operations
public:
	bool	ItemADD(CString dat, bool list = false);
	bool	ItemINSERT(int nIdx, CString dat, bool list = false);
	bool	ItemSubADD(CString dat);
	bool	ItemDEL(int index);
	void	removeALL();
	void	ReCalc();
	void	change_Skin(CString skinName);
	void	SaveItems();

//	void	SetItemWidth(int width) { m_itemW = width; }
//	void	Set_Color(int, COLORREF, COLORREF, COLORREF);
	void	SetToolTipText(CString text, BOOL bActivate = TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHBand)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsWebMenu(const char *MapName);
	HICON GetScreenIcon(CString Desc, CString scrNo = "");

	// Generated message map functions
protected:
	void	HoverItem(CPoint point);
	void	InitToolTip();
	void	drawBackGround(CDC* pDC);
	void	drawBandItem(CDC* pDC);
	void	drawItem(CDC* pDC, int idx, int mode = MODE_COOL);
	void	draw_Img(CDC* pDC, CRect iRc, int kind, int mode = MODE_COOL);
	void	draw_Expand(CDC* pDC = NULL, int mode = MODE_COOL);

	void	FitTextRect(CRect& tRc);
	void	ShowItemList(CString key, CPoint point);
	CRect	GetToolRect(int index = 0);
	void	LoadSubItem(CString section);
	CString GetSubItemData(CString section, int idx);
	void	InsertSubMenu(CMenu* menu, CString section, int& index);
	int		GetSelectItem(CPoint pt);
	void	showMenu();
	CString GetNewChart(CString old);

	void	LoadToolBox();
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CHBand)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HBAND_H__4CEC94AE_46D2_4C12_A7F3_9476504337B7__INCLUDED_)
