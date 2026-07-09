#if !defined(AFX_SCHILD_H__0A1CF687_7458_44AF_88ED_6432B208BC61__INCLUDED_)
#define AFX_SCHILD_H__0A1CF687_7458_44AF_88ED_6432B208BC61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SChild.h : header file
//
#include "XCaption.h"
/////////////////////////////////////////////////////////////////////////////
// CSChild frame

class CSChild : public CFrameWnd
{
public:
//	DECLARE_DYNCREATE(CSChild)
	CSChild(CWnd* mainframe, bool stealth = false);           // protected constructor used by dynamic creation
	virtual ~CSChild();

// Attributes
public:
	CWnd*		m_mainframe;
	CPoint		m_mdiPt;
	CPoint		m_defaultpos;

	CRect		m_rectMDI;			// 단독실행시의 윈도우 위치 
	CRect		m_restoreRc;
	int		m_width;
	int		m_height;
	int		m_key;
	int		m_vwTYPE;
	int		m_cursor;

	bool		m_tView;
	bool		m_bActive;
	bool		m_bCaption;
	bool		m_bIconic;
	bool		m_bstealth;
	bool		m_sdiMode;
	bool		m_fixW;
	bool		m_fixH;
	bool		(*m_axiscall)(int, WPARAM, LPARAM);
	CXCaption	m_xcaption;
	class CLinkbar*	m_linkBar;
	
	CString		m_mapN;
	class _axisRes*		m_axisres;
	class CNPalette*	m_palette;

	CWnd*		m_wndHook;
	bool		m_enableHook;

	int		m_cxBorder, m_cyBorder, m_cxEdge, m_cyEdge, m_cyCaption;
	HRGN		m_rgn;
	int		m_level;

	int		m_szType;

	CString		m_strSetConfig;
protected:
	CWnd		m_wndInvisible;
// Operations
public:
	void	SetMisc(UINT msg, CWnd* parent, bool (*axiscall)(int, WPARAM, LPARAM),
			int fontsize, bool sdiMode, class _axisRes* axisres = NULL, bool bCaption = true);
	void	DrawFrame(CDC* pDC);
	void	ChangeSkin(CString skinName);
	CView*	AttachView(CCreateContext context);
	void	sMax();
	void	SetRestore();
	void	SetMax();
	bool	createLink(class CTMenu* menu, CString mapN, bool (*axiscall)(int, WPARAM, LPARAM),
			CString home, CString userName, CString skinName, bool bShow);
	long	runHookedKey(WPARAM wParam, LPARAM lParam);

	int	osLevel();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSChild)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSChild)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LPARAM OnLinkOpen(WPARAM wp, LPARAM lp);
	afx_msg	LPARAM OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg	LPARAM OnCHILDMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHILD_H__0A1CF687_7458_44AF_88ED_6432B208BC61__INCLUDED_)
