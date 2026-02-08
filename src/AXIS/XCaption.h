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
#define XCAP_HEIGHT	20

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

//#define _MARK_TITLE
#define _MARK_TEXT 1
#define _MARK_TITLE 0
#define _MARK_	(_MARK_TEXT + _MARK_TITLE)

#define BUTTON_MASK_COLOR Axis::maskColor
/////////////////////////////////////////////////////////////////////////////
// CXCaption window
#include "useMaindefine.h"
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
	bool		m_single;
	bool		m_sdiMode;
	bool		m_pinON;
	bool		m_bIconic;
	bool		m_mouseOver;
	bool		m_bMax;
	BOOL		m_activeCap;

	CString		m_title;
	CString		m_sMapnum{};
#ifdef DF_USE_CPLUS17
	std::unique_ptr <CToolTipCtrl>m_ToolTip;
#else
	CToolTipCtrl*	m_ToolTip;
#endif

	COLORREF	m_aVSColor;		// active color
	COLORREF	m_aVEColor;		// active color
	COLORREF	m_aHSColor;		// active color
	COLORREF	m_aHEColor;		// active color
	
	COLORREF	m_mVSColor;		// active color
	COLORREF	m_mVEColor;		// active color
	COLORREF	m_mHSColor;		// active color
	COLORREF	m_mHEColor;		// active color
	
	COLORREF	m_iVSColor;		// inactive color
	COLORREF	m_iVEColor;		// inactive color
	COLORREF	m_iHSColor;		// inactive color
	COLORREF	m_iHEColor;		// inactive color

	DWORD		m_hideflag;		// hide button flag
	int		m_key;			// child window key
	int		m_fontSize;		// screen font size
	int		m_groupN;		// group N
	int		m_vsN;			// virtual screen number
	CArray	< CRect, CRect > m_arRc;	// button rect

	bool		m_bNcLDown;
public:
	void	SetToolTipText(CString text, BOOL bActivate = TRUE);
	void	InitToolTip();
	void	HoverItem(CPoint point);

	void	ChangeSkin();
	void	ChangeVS(int vsN);
	void	SetText(LPCTSTR lpszNew);
	void	SetFunc(int index, bool on = false);
	void	ShowButton(DWORD flag);
	void	HideButton(DWORD flag);
	void	SetFontSize(int fSize)	{ m_fontSize = fSize; }
	int		GetFontSize()	{ return m_fontSize;  }
	int		GetGroup()	{ return m_groupN; }
	void	SetGroup(int group);
	void	SetMisc(CWnd* mainframe, UINT nMsg, int key, int fontsize, bool sdi);
	int		ChangeFont();
	int		ChangeGroup();
	int 	DrawBitmap(CDC* pDC, int index, CRect bRC, bool bDown = false);
	virtual int	DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown = false, int maskcolor = BUTTON_MASK_COLOR) = 0;// { return 0;};
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
	int		GetLuminosity(COLORREF color) const;
	int		GetSysCommand(CPoint point);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	CBitmap* GetButtonImage(int index);

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
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
#if _MARK_TITLE
	COLORREF GetMColor(COLORREF color1, COLORREF color2);
#endif

protected:
	BOOL m_gradientDraw;
	CBitmap* m_capAct;
	CBitmap* m_capInact;
	CBitmap* m_capSideAct;
	CBitmap* m_capSideInact;

	CSize m_szCaptionBitmap;
	virtual void LoadBitmap();
protected:
	virtual void Initialize(){}
	virtual void Release(){}

	virtual void OnLButtonDown(CDC* dc, CRect& rc, int nItem) = 0;
	virtual void OnLButtonDblClk(int nItem) = 0;
	virtual void DrawTitle(CDC* dc) = 0;
	virtual CSize OnCalcFixedLayout(BOOL bStretch, BOOL bHorz) = 0;
	virtual CString GetItemTipText(int nItem) = 0;
	virtual void FocusNotify() = 0;
	virtual void SetRegion() = 0;
public:
	int DefaultFont();
	virtual bool ActivateApp(BOOL bActive) = 0;
	virtual int	 GetSelectItem(CPoint point) = 0;

	virtual void DrawFrame(CDC* dc, CRect rc) {}

#ifdef DF_MK_CAPTION
	CString m_slog{};
	BOOL	m_MkLock = 0;
	int		m_iMkPermission{};
	int		m_marketN = 1; //modi NXT   1:K krx  2:N nxt  3 : 통합 4 : 선택불가
	int		ClickMK();  //modi NXT 
	void		ChangeLock() {
		m_MkLock = !m_MkLock;
		Invalidate();
	}
	int		GetMarkgetType() { return m_marketN; }
	void    	SetMarket(int iMarket);//modi NXT 
	void     SetMapNum(CString smap);
	CString GetMapNum() {
		return m_sMapnum;
	}
	void SetMapPermission(int ival);
#endif

};

//** CXChildCaption
class CXChildCaption: public CXCaption
{
public:
	CXChildCaption();
	virtual ~CXChildCaption();
protected:
	virtual void Initialize();
	virtual void Release();

	virtual int	DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown = false, int maskcolor = BUTTON_MASK_COLOR);
	virtual void OnLButtonDown(CDC* dc, CRect& rc, int nItem);
	virtual void OnLButtonDblClk(int nItem);

	virtual void DrawTitle(CDC* dc);
	virtual CSize OnCalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CString GetItemTipText(int nItem);
	virtual void FocusNotify();
	virtual void SetRegion();
	void LoadBitmap();

	CBitmap* GetButtonImage(int index);

public:
	virtual bool ActivateApp(BOOL bActive);
	virtual int  GetSelectItem(CPoint point);	

	virtual void DrawFrame(CDC* dc, CRect rc);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCAPTION_H__29AE6E88_A9E1_4C63_BE4F_7F919EE129B5__INCLUDED_)
