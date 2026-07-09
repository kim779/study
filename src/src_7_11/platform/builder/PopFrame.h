#if !defined(AFX_POPFRAME_H__03433E08_4EF8_4FE6_95AF_AC4294FDE696__INCLUDED_)
#define AFX_POPFRAME_H__03433E08_4EF8_4FE6_95AF_AC4294FDE696__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopFrame frame

class CPopFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPopFrame)
public:
	CPopFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CWnd		*m_pVirtual;
	CMenu		*m_pSMenu;
	CSize		m_initSize;
	int		m_wndType;
	
	class CChildFrame*	m_pParent;
	class CMainFrame*	m_mainFRM;
protected:
	HACCEL m_hAccel;
// Operations
public:
	void SetParent(CChildFrame* pChildWnd, CWnd* pVirtualWnd);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPopFrame();

	// Generated message map functions
	//{{AFX_MSG(CPopFrame)
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPFRAME_H__03433E08_4EF8_4FE6_95AF_AC4294FDE696__INCLUDED_)
