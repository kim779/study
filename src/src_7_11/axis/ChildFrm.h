// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__E37F074E_A1EB_4AF5_A4C2_A7297C213DAA__INCLUDED_)
#define AFX_CHILDFRM_H__E37F074E_A1EB_4AF5_A4C2_A7297C213DAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	POS_CHILDMOVE		5000

#include "XCaption.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:
	class _axisRes*		m_axisres;
	class CLinkbar*		m_linkBar;
	CXCaption		m_xcaption;

	int	m_key;
	int	m_vwTYPE;
	int	m_width;
	int	m_height;
	int	m_cursor;
	

	int	m_szType;

	CString	m_mapN;

	bool	m_suspend;
	bool	m_bIconic;
	bool	m_tView;
	bool	m_bCaption;
	bool	m_bSingle;
	bool	m_fixW;
	bool	m_fixH;
	BOOL	m_bActive;

	CPoint	m_iconpos;
	CPoint	m_sdiPt;			// sdi left top point
	CRect	m_restoreRc;			
	CRect	m_rectSDI;			// 단독실행시의 윈도우 위치 
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	HHOOK	m_hook;
	CWnd*	m_wndHook;
	bool	m_enableHook;
	CString	m_strSetConfig;

	int	m_cxBorder, m_cyBorder, m_cxEdge, m_cyEdge, m_cyCaption;
	HRGN	m_rgn;
	int	m_level;

protected:
	CPoint	m_SP;
// Operations
public:
	CPoint	GetSP() { return m_SP; }
	void	SetMisc(UINT msg, CWnd* parent, bool (*axiscall)(int, WPARAM, LPARAM),
			 int fontsize, class _axisRes* axisres = NULL, bool bCaption = true);
	void	HideButton(DWORD dwHide)	{ m_xcaption.HideButton(dwHide); }
	void	ChangeSkin(CString skinName);
	void	DrawFrame(CDC* pDC);
	void	actionCaption(int action, CPoint pt);
	bool	createLink(class CTMenu* menu, CString mapN, bool (*axiscall)(int, WPARAM, LPARAM), 
			CString home, CString userName, CString skinName, bool bShow);	
	int	osLevel();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();

	void	SetMinimize();
	void	SetRestore();
	void	SetMax();
	CSize	GetSize();
	long	runHookedKey(WPARAM wParam, LPARAM lParam);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	bool	runCommand(int comm, LPARAM lParam);
	
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnGroup(UINT ua);
	afx_msg	LRESULT OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnCHILDMSG(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLinkOpen(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__E37F074E_A1EB_4AF5_A4C2_A7297C213DAA__INCLUDED_)
