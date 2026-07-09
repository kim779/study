#if !defined(AFX_LINKBAR_H__2665FD8E_67ED_4171_B041_33D1D21B162C__INCLUDED_)
#define AFX_LINKBAR_H__2665FD8E_67ED_4171_B041_33D1D21B162C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Linkbar.h : header file
//
#define LBAR_HEIGHT	23

#include <afxtempl.h>
#define	WM_LINKOPEN	WM_USER + 7777
class linkinfo
{
public:
	CString		mapN;
	CString		desc;
	CString		name;
};
/////////////////////////////////////////////////////////////////////////////
// CLinkbar window

class CLinkbar : public CControlBar
{
// Construction
public:
	CLinkbar(class CTMenu* menu, CString mapN, bool (*axiscall)(int, WPARAM, LPARAM),
			CString home, CString userName, CString skinName);
	virtual ~CLinkbar();

// Attributes
public:
	CBrush		m_patbrush;
	CString		m_home;
	CString		m_mapname;
	CString		m_userName;

	int		m_itemW;
	int		m_itemH;
	int		m_expandW;
	int		m_hover;
	int		m_push;

	bool		m_pushON;
	bool		m_mouseOver;

	CFont		m_font;
	CBitmap		m_bitmap;
	CBitmap		m_imgB;
	CBitmap		m_exImg;
	bool		(*m_axiscall)(int, WPARAM, LPARAM);

	class CTMenu*	m_tmenu;
	class CNButton*	m_linkEdit;
	CToolTipCtrl*	m_ToolTip;
	CArray	<class linkinfo*, class linkinfo* >	m_arInfo;
// Operations
public:
	void	ChangeSkin(CString skinName);
	void	loadinfo(CString mapN);
	void	removeAry();
	int	displayCnt();
	CRect	GetItemRect(int idx);
	void	ChangeInfo(CString mapname);
	CString	GetSName(CString mapN);

protected:
	void	InitToolTip();
	void	SetToolTipText(CString text, BOOL bActivate = TRUE);
	
	void	drawLinkinfo(CDC* pDC);
	void	drawItem(CDC* pDC, int idx, int mode = 0);
	void	drawImg(CDC* pDC, CRect bRC, int mode = 0, int maskcolor = RGB(255, 0, 255));
	void	HoverItem(CPoint point);
	int	GetSelectItem(CPoint pt);
	void	draw_Expand(CDC* pDC, int mode = 0, int maskcolor = RGB(255, 0, 255));
	void	showMenu();

	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkbar)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CLinkbar)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
	afx_msg	LRESULT OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKBAR_H__2665FD8E_67ED_4171_B041_33D1D21B162C__INCLUDED_)
