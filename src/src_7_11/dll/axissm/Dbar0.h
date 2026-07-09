#if !defined(AFX_DBAR0_H__18285C9E_A1BA_499E_BAFB_777551B5FAAB__INCLUDED_)
#define AFX_DBAR0_H__18285C9E_A1BA_499E_BAFB_777551B5FAAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dbar0.h : header file
//
#include <afxcoll.h>
#include "ReportCtrl.h"
#include "resource.h"

#define	VARWIDTH	200
#define	BANDGAP		4
#define	BANDHEIGHT	24
#define	CONTROLH	110

#define	CON_HIDE	1
#define	CON_PREV	2
#define	CON_NEXT	3
#define	CON_PALY	4
#define	CON_PAUSE	5
#define	CON_CONTINUE	6
#define	CON_STOP	7

/////////////////////////////////////////////////////////////////////////////
// CDbar0 window
class AFX_EXT_CLASS CDbar0 : public CDialog
{
// Construction
public:
	CDbar0(bool (*callback)(int, WPARAM, LPARAM), CString home, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDbar0)
	enum { IDD = IDD_DBAR0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
protected:
	CWnd*		m_parent;	// mainframe

	CString		m_home;
	CString		m_user;
	CString		m_code;
	CString		m_mapN;

	CFont		m_font;
	CRect		m_tRc[3];
	CRect		m_hRc;

	COLORREF	m_bk;
	COLORREF	m_color;
	CReportCtrl	m_codeList;
	CListCtrl	m_mapList;

	class CWnd*	m_cbar;
	class CRunX*	m_runX;
	class CNButton*	m_closeX;
	class CNButton*	m_groupS;
	class CNButton*	m_codeS;
	class CNButton*	m_mapA;
	class CNButton*	m_mapS;
	class CNButton*	m_check;

	class CTMenu*	m_menu;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
	bool		m_bCheck;
	CStringArray	m_arMap;

// Operations
public:
	void	make_Ctrl();
	void	change_BandInfo();
	void	set_MenuInfo(class CTMenu* menu, CString user);
	void	reload_Code();

	CString	GetCode();
	CString	GetMapN();
	CWnd*	GetCategoryWnd() { return (CWnd*) m_groupS; }
	COLORREF	GetNaviColor() { return m_color; }
	void	SetCBar(CWnd *bar) { m_cbar = (CWnd *)bar; }

protected:
	void	draw_Band(CDC* pDC = NULL);
	void	draw_Img(CDC* pDC);
	void	change_Position();
	void	fit(CListCtrl& list);
	BOOL	addColumn(CListCtrl& list, LPCTSTR columns, int col, int width = 0);
	BOOL	addItem(CListCtrl& list, CString items, int item, int subi = 0);
	void	setCodeListCheck();
	int	getCheckCodeCount();

	void	reload_Map();
	void	enable_Button(int kind);

	void	Prev();
	void	Next();
	void	Play();
	void	Pause();
	void	Continue();
	void	Stop();
	void	ChangeMap(bool code = false);
	int	GetOrder();
	int	GetKind();
	int	GetInterval();

	COLORREF GetBkColor();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbar0)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDbar0();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDbar0)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg	LRESULT OnXMSG(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeLIST(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBAR0_H__18285C9E_A1BA_499E_BAFB_777551B5FAAB__INCLUDED_)
