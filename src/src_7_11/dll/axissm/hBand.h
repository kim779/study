#if !defined(AFX_HBAND_H__4CEC94AE_46D2_4C12_A7F3_9476504337B7__INCLUDED_)
#define AFX_HBAND_H__4CEC94AE_46D2_4C12_A7F3_9476504337B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// hBand.h : header file
//

#define	ID_TOOLTIP	1

#define COLOR_ITCOOL	RGB( 43,    43,    43)
#define COLOR_ITOVER	RGB( 15,   15,   15)
#define COLOR_ITDOWN	RGB( 43,  43,  43)

#define	ID_ADDTOOL	1001
#define ID_DELTOOL	1002
#define ID_CANCELTOOL	1003

#define	MODE_COOL	0
#define	MODE_OVER	1
#define	MODE_DOWN	2

#include <afxtempl.h>

#define	BAND_ITEM	0
#define	BAND_LIST	1

class CBandItem
{
public:
	CString	mapN;
	CString	tooltip;
	CString	text;
	int	kind;
};
/////////////////////////////////////////////////////////////////////////////
// CHBand window

class CHBand : public CWnd
{
// Construction
public:
	CHBand(bool (*axiscall)(int, WPARAM, LPARAM), CString home);
	virtual ~CHBand();

// Attributes
public:
	CArray	<CBandItem*, CBandItem*> m_arItem;
	CArray	<CBandItem*, CBandItem*> m_arCommonItem;
	CArray	<CBandItem*, CBandItem*> m_arsubItem;
	CStringArray	m_commandList;
private:
	CString	m_home;

	int	m_hindex;
	int	m_dispN;
	int	m_hover;
	int	m_push;
	int	m_itemW;
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

	CBitmap		m_bitmap;
	CBitmap		m_img[2];
	CToolTipCtrl*	m_ToolTip;

	bool		m_bDrag;
	CRect		m_rcSelected;
	CImageList*	m_pDragImage;

// Operations
public:
	bool	ItemADD(CString dat, bool list = false);
	bool	CommonItemADD();
	bool	ItemSubADD(CString dat);
	bool	ItemDEL(int index);
	void	removeALL();
	void	ReCalc();
	void	change_Skin(CString skinName);
	void	drawCommonItem(CDC* pDC);

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

	// Generated message map functions
protected:
	void	HoverItem(CPoint point);
	void	InitToolTip();
	void	drawBackGround(CDC* pDC);
	void	drawBandItem(CDC* pDC);
	void	drawItem(CDC* pDC, int idx, int mode = MODE_COOL);
	void	drawCommon(CDC* pDC, int idx, int mode = MODE_COOL);
	void	draw_Img(CDC* pDC, CRect iRc, int kind, int mode = MODE_COOL);
	void	draw_Expand(CDC* pDC = NULL, int mode = MODE_COOL);

	void	FitTextRect(CRect& tRc);
	void	ShowItemList(CString key, CPoint point);
	CRect	GetToolRect(int index = 0);
	CRect	GetCommonRect(int index = 0);
	void	LoadSubItem(CString section);
	CString	GetUserName();
	void	InsertSubMenu(CMenu* menu, CString section, int& index);
	int	GetSelectItem(CPoint pt);
	void	showMenu();
	void	WriteToolItem();
	int	GetDropIndex(CRect rc, CPoint point);
	CImageList*	MakeDragImage(CString strImage, CWnd *pWnd);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CHBand)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HBAND_H__4CEC94AE_46D2_4C12_A7F3_9476504337B7__INCLUDED_)
