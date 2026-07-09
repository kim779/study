#if !defined(AFX_XCAPTION_H__29AE6E88_A9E1_4C63_BE4F_7F919EE129B5__INCLUDED_)
#define AFX_XCAPTION_H__29AE6E88_A9E1_4C63_BE4F_7F919EE129B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XCaption.h : header file
//
#define INITMAPSIZE	100
#define TM_LOCALTIME	100
#define SYS_MENU	-1
#define SYS_NONE	0
#define SYS_MIN		1
#define SYS_MAX		2
#define SYS_EXIT	3
#define XCAP_HEIGHT	30  //23

#define	FUNCNO			2
#define FUNC_ALL		-1
#define FUNC_SCRCOPY		0
#define FUNC_MODECHANGE		1

#define ACTCAP_S	RGB( 96, 157, 182)
#define ACTCAP_E	RGB( 46,  62, 132)
#define INACTCAP_S	RGB(183, 183, 183)
#define INACTCAP_E	RGB(153, 153, 153)

#define SHOW_ALL	0x00
#define HIDE_MAX	0x01
#define HIDE_FONT	0x02
#define HIDE_HELP	0x04
#define HIDE_COPY	0x08
#define HIDE_GROUP	0x10
#define HIDE_SINGLE	0x20

#include <afxtempl.h>
#include "AxBitmap.h"
/////////////////////////////////////////////////////////////////////////////
// CXCaption window


class CXCaption : public CControlBar
{
// Construction
public:
	CXCaption();
	virtual ~CXCaption();

// Attributes
protected:
	CWnd*		m_mainframe;
	UINT		m_nMsg;
	int		m_action;		// user button action
	int		m_hoverI;		// hover item index
	int		m_downI;		// button down index
	int		m_syscom;
	int		m_func;
	bool		m_main;
	bool		m_single;
	bool		m_sdiMode;
	bool		m_pinON;
	bool		m_bIconic;
	bool		m_mouseOver;
	bool		m_bMax;
	BOOL		m_activeCap;

	CBitmap		m_mainbitmap[2];
	CString		m_title;
	CToolTipCtrl*	m_ToolTip;

	COLORREF	m_aVSColor;		// active color
	COLORREF	m_aVEColor;		// active color
	COLORREF	m_aHSColor;		// active color
	COLORREF	m_aHEColor;		// active color
	COLORREF	m_iVSColor;		// inactive color
	COLORREF	m_iVEColor;		// inactive color
	COLORREF	m_iHSColor;		// inactive color
	COLORREF	m_iHEColor;		// inactive color

	DWORD		m_hideflag;		// hide button flag
	int		m_key;			// child window key
	int		m_fontSize;		// screen font size
	int		m_groupN;		// group N
	int		m_vsN;			// virtual screen number
	class _axisRes*	m_axisres;		// resource bmp
	CArray	< CRect, CRect > m_arRc;	// button rect

	CAxBitmap			m_bmCaption[2];
	CAxBitmap			m_bmVS[4];
	CAxBitmap			m_bmCapright[2];

public:
	void	SetMain();
	void	SetToolTipText(CString text, BOOL bActivate = TRUE);
	void	InitToolTip();
	void	HoverItem(CPoint point);

	void	DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight = false);

	BOOL	ShowBmp(CDC* pDC, int x, int y, CString filename, int maskcolor);
	BOOL	ShowBmp(CDC* pDC, int x, int y, UINT nResID, int maskcolor);
	
	void	ChangeSkin();
	void	ChangeVS(int vsN);
	void	SetText(LPCTSTR lpszNew);
	void	SetFunc(int index, bool on = false);

	bool	ActiveateApp(BOOL bActive);
	void	HideButton(DWORD flag);
	void	SetFontSize(int fSize)	{ m_fontSize = fSize; }
	int	GetFontSize()	{ return m_fontSize;  }
	int	GetGroup()	{ return m_groupN; }
	void	SetGroup(int group);
	void	SetMisc(CWnd* mainframe, UINT nMsg, int key, int fontsize, class _axisRes* axisres, bool sdi);
	int	ChangeFont();
	int	ChangeGroup();
	int 	DrawBitmap(CDC* pDC, int index, CRect bRC, BOOL bActive = TRUE,  bool bDown = false);
	int	DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown = false, BOOL bActive = true, int maskcolor = RGB(255, 0, 255));
	int	GetSelectItem(CPoint point);
	void	SetIconic(bool bIconic) { m_bIconic = bIconic;}
	bool	GetIconic() { return m_bIconic; }
	void	SetMax(bool bMax) { m_bMax = bMax; }
	bool	IsMax()	{ return m_bMax; }
	void	SetTitle(CString title);
	void	SetSingle()	{ m_single = true; }
	CString	GetTitle()	{ return m_title; }
	bool	changePin();
	void	SetSDI();
	BOOL	IsActive()	{ return m_activeCap; }
protected:
	int	GetLuminosity(COLORREF color) const;
	int	GetSysCommand(CPoint point);
	void	SetRegion();
	void	drawMainTitle(CDC* pDC);
	void	drawTitleBitmap(CDC *pDC, CRect drawRC, BOOL bActive);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXCaption)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CXCaption)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCAPTION_H__29AE6E88_A9E1_4C63_BE4F_7F919EE129B5__INCLUDED_)
