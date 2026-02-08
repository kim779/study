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
	CString m_slog{};
// Attributes
private:
	bool		m_tView;
protected:
	int		m_width;
	int		m_height;
public:
	CSize   GetSize();
	void	SetSize(CSize sz);
	void	SetSize(int width, int height, BOOL revise = TRUE);
public:
	BOOL IsActive() { return m_xcaption.IsActive(); }	// cej 20070129
	CWnd*		m_mainframe;
	CPoint		m_mdiPt;
	CPoint		m_defaultpos;

	CRect		m_rectMDI;			// 단독실행시의 윈도우 위치 
	CRect		m_restoreRc;
	int		m_key;
	int		m_vwTYPE;
	int		m_cursor;

	bool		m_bActive;
	bool		m_bCaption;
	bool		m_bBackGround;
	bool		m_bIconic;
	bool		m_bstealth;
	bool		m_sdiMode;
	bool		m_fixW;
	bool		m_fixH;
	bool		(*m_axiscall)(int, WPARAM, LPARAM);
	CXChildCaption	m_xcaption;
	
	CString		m_mapN;
	CString		m_usrData;
	class CNPalette*	m_palette;

	void		SetTabView(bool isTabView);
	bool		IsTabView();
protected:
	CWnd		m_wndInvisible;
// Operations
public:
	void SetMapName(bool (*axiscall)(int, WPARAM, LPARAM), CString sMapName);
	void	SetMisc(UINT msg, CWnd* parent, bool (*axiscall)(int, WPARAM, LPARAM),
			int fontsize, bool sdiMode, bool bCaption = true);
	void	DrawFrame(CDC* pDC);
	void	ChangeSkin(CString skinName);
	CView*	AttachView(CCreateContext context);
	void	sMax();
	void	SetRestore();
	void	SetMax();
	void	RedrawCaption() { m_xcaption.Invalidate(FALSE); }
	CString	GetTitle() { return m_xcaption.GetTitle(); }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSChild)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	afx_msg	LONG OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnCHILDMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	MINMAXINFO	m_minMaxInfo;
	BOOL m_revised;
public:
	void ShowBGCaption(bool f_show);
	void	SetMinMaxInfo(MINMAXINFO* lpMinMaxInfo);
	afx_msg BOOL OnNcActivate(BOOL bActive);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHILD_H__0A1CF687_7458_44AF_88ED_6432B208BC61__INCLUDED_)
