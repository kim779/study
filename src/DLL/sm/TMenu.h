#if !defined(AFX_TMENU_H__9DC72AB6_9B01_4741_AEF0_11A1E3AA61A2__INCLUDED_)
#define AFX_TMENU_H__9DC72AB6_9B01_4741_AEF0_11A1E3AA61A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TMenu.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CTMenu window
#include "usedefine.h"
class AFX_EXT_CLASS CTMenu : public CDialogBar
{
// Construction
public:
	CTMenu(CWnd* parent, int height = 22/*GetSystemMetrics(SM_CYMENU)*/);
	virtual ~CTMenu();

// Attributes
public:
protected:
	CWnd*		m_parent;
#ifdef DF_USESTL_TMENU
	std::unique_ptr<class CIMenu>m_iMenu;
#else
	class CIMenu* m_iMenu;
#endif

	
	int		m_height{};
	int		m_cmdBase{};

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTMenu)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight);
	void	MeasureMenuItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	BOOL	PreTranslateMsg(MSG* pMsg);

	BOOL	InitMenu(UINT nResource, int cmdBase = 0);
	BOOL	ReloadMenu(UINT nResource, int cmdBase = 0);
	//void     AddMenu(CString smenu);  //test AI

	int	GetMCount();
	BOOL	IsvalidMap(CString mapN);
	BOOL	ExistMenu(CString mapN);
	BOOL	IsWeb(CString mapN);
	BOOL	IsMappinged(CString mapN);
	BOOL	GetSelectList(CString input, CStringArray& array);
	CString	GetURL(CString mapN);
	CString	GetSName(CString mapN);
	CString	GetDesc(CString mapN);
	CString	GetInfo(CString mapN);
	CString	GetMap(int index);
	CString	GetMap(CString dispN);
	CString	GetMapping(CString mapN);
	CString	GetDispN(CString mapN);
	CString GetMenuName(CString mapN);

	void	ShowFullMenu(CPoint pt);
	CMenu*	GetPopupMenu(int index);
	CMenu*	GetNewMenu();
	void	fitMenu();
protected:
	void	Init(int commandBase = 0);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTMenu)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	int GetMenuMode();
	int GetHeight();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMENU_H__9DC72AB6_9B01_4741_AEF0_11A1E3AA61A2__INCLUDED_)
