#if !defined(AFX_TMENU_H__9DC72AB6_9B01_4741_AEF0_11A1E3AA61A2__INCLUDED_)
#define AFX_TMENU_H__9DC72AB6_9B01_4741_AEF0_11A1E3AA61A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TMenu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTMenu window

#define	CTRL_SCREEN	1

class AFX_EXT_CLASS CTMenu : public CDialogBar
{
// Construction
public:
	CTMenu(CWnd* parent, CString home, int height = 32/*GetSystemMetrics(SM_CYMENU)*/);
	virtual ~CTMenu();

// Attributes
public:

protected:
	CWnd*		m_parent;
	class CIMenu*	m_iMenu;
	class CXcom*	m_pXcom;	


	CButton*	m_pButton;
	CComboBox*	m_pCombo;

	int		m_height;
	int		m_cmdBase;
	CString		m_home;

	bool	(*m_axiscall)(int, WPARAM, LPARAM);


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTMenu)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	MeasureMenuItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	BOOL	PreTranslateMsg(MSG* pMsg);

	BOOL	InitMenu(UINT nResource, int cmdBase = 0);
	BOOL	ReloadMenu(UINT nResource, int cmdBase = 0);

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

	void	ShowFullMenu(CPoint pt, bool bTray = false);
	CMenu*	GetPopupMenu(int index);
	void	fitMenu();


	void SetCall(bool (*callback)(int, WPARAM, LPARAM)){ m_axiscall = callback; }
	void SetHome();
	void set_Maps(CString mapname = _T(""));
	void add_History(CString dat);
	void set_Owner();
	void IsInputFocus();

	void set_List(CStringArray& array);

protected:
	void	Init(int commandBase = 0);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTMenu)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnClickUserSpace();
	afx_msg void OnComboChange();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMENU_H__9DC72AB6_9B01_4741_AEF0_11A1E3AA61A2__INCLUDED_)
