#if !defined(AFX_SDIBAND_H__25726204_CF2E_49C6_92DB_7DF060813910__INCLUDED_)
#define AFX_SDIBAND_H__25726204_CF2E_49C6_92DB_7DF060813910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SDIBand.h : header file
//

#define SCOLOR_ITCOOL	RGB(  0,    0,    0)
#define SCOLOR_ITOVER	RGB( 15,   15,   15)
#define SCOLOR_ITDOWN	RGB(255,  255,  255)

#define	SID_ADDTOOL	1001
#define SID_DELTOOL	1002
#define SID_CANCELTOOL	1003

#define STYPE_BACK	1
#define STYPE_TEXT	2

#define	SMODE_COOL	0
#define	SMODE_OVER	1
#define	SMODE_DOWN	2

#include <afxtempl.h>
#define	SBAND_ITEM	0
#define	SBAND_LIST	1
#define	SID_TOOLTIP	1

#include "hBand.h"

/////////////////////////////////////////////////////////////////////////////
// CSDIBand window
#include "usedefine.h"
class CSDIBand : public CWnd
{
// Construction
public:
	CSDIBand(bool (*axiscall)(int, WPARAM, LPARAM), bool bSDI);
	virtual ~CSDIBand();

// Attributes
public:
#ifdef DF_USESTL_SDIBAND
	std::vector<std::shared_ptr<CBandItem>>	m_arItem;
	std::vector<std::shared_ptr<CBandItem>>	m_arsubItem;
#else
	CArray	<CBandItem*, CBandItem*> m_arItem;
	CArray	<CBandItem*, CBandItem*> m_arsubItem;
#endif
	
	CStringArray	m_commandList;
private:

	int	m_hindex{};
	int	m_dispN{};
	int	m_hover{};
	int	m_push{};
	int	m_itemW{};
	int	m_itemH{};
	int	m_expandW{};

	bool	m_bSDI{};
	bool	m_mouseIN{};
	bool	m_pushON{};
	bool	m_defaultColor{};
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	CFont	m_font;
	CRect	m_expandRc;
	
	COLORREF	m_itcolorCool;
	COLORREF	m_itcolorOver;
	COLORREF	m_itcolorDown;

	CBitmap*		m_bmToolbox{};
	CBitmap* m_bmExpand{};

#ifdef DF_USESTL_SDIBAND
	std::unique_ptr<CToolTipCtrl> m_ToolTip;
#else
	CToolTipCtrl*	m_ToolTip{};
#endif

	bool		m_bDrag;
	CRect		m_rcSelected;
	CImageList*	m_pDragImage;

// Operations
public:
	bool	ItemADD(CString dat, bool list = false);
	bool	ItemSubADD(CString dat);
	bool	ItemDEL(int index);
	void	removeALL();
	void	ReCalc();
	void	change_Skin(CString skinName);

	void	SetToolTipText(CString text, BOOL bActivate = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDIBand)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	void	HoverItem(CPoint point);
	void	InitToolTip();
	void	drawBackGround(CDC* pDC);
	void	drawBandItem(CDC* pDC);
	void	drawItem(CDC* pDC, int idx, int mode = SMODE_COOL);
	void	draw_Img(CDC* pDC, CRect iRc, int kind, int mode = SMODE_COOL);
	void	draw_Expand(CDC* pDC = NULL, int mode = SMODE_COOL);

	void	FitTextRect(CRect& tRc);
	void	ShowItemList(CString key, CPoint point);
	CRect	GetToolRect(int index = 0);
	void	LoadSubItem(CString section);
	CString	GetUserName();
	void	InsertSubMenu(CMenu* menu, CString section, int& index);
	int	GetSelectItem(CPoint pt);
	void	showMenu();
	void	WriteToolItem();
	int	GetDropIndex(CRect rc, CPoint point);
	CImageList*	MakeDragImage(CString strImage, CWnd *pWnd);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CSDIBand)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDIBAND_H__25726204_CF2E_49C6_92DB_7DF060813910__INCLUDED_)
