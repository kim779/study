// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__428255A8_45FC_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_MAINFRM_H__428255A8_45FC_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxtempl.h>
#include "mapvar.h"
#include "awWcc/libwcc.h"
#include "WorkSpaceBar.h"
#include "PropertiesBar.h"
#include "PreViewBar.h"
#include "ScriptBar.h"
#include "TraceBar.h"
#include "SelectionBar.h"
#include "Group.h"

#include "PreviewWnd.h"
#include "awDlg/Login.h"

#include "Components/MDITabs.h"
#include "Components/TrueColorToolBar.h"
#include "Components/BCMenu.h"
#include "Components/Tips.h"


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
// Attributes
public:
	struct  _mapH		m_clipMAP;
	CSize	m_delta;
	DWORD	m_status;
	CWorkSpaceBar	m_wndWorkBar;

	CArray	<class CChildFrame*, CChildFrame*> m_children;

	CMapToken	*m_pToken;
	CGroup		*m_pGroup;
	CString		m_sGroup, m_currentPath, m_sDefFontName, m_sDefFontPnt, m_sInitMap;
	CMDITabs	m_wndMDITabs;

	CString		m_rtnStr, m_sUserID, m_sPass, m_sProjectName;
	CPreviewWnd	*m_pWizardCtrl;

	BCMenu		m_defaultMenu;

	BOOL		m_bSDI;
protected:
private:
	int		m_nDisplayMode;


// Operations
public:
	HMENU		NewDefaultMenu();
	void		RemoveChild(int key);
	BOOL		PressButton(UINT id);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


// Implementation
public:
	CString getClientIP();
	int	getPort();
	BOOL	Login();
	void	LoginWizard();
	void	StartWorkshop();
	bool	UnRegisterCtrl(CString name);
	bool	RegisterCtrl(CString name, bool bMsg = false);
	void	changeMap(CString src);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	void	ReCreateMDITabs();
	bool	IsInEditing(CString mapN, BYTE mapK);
	void	MakeDir(CString root);
	bool	dropInChild();
	CString	getErrString(int nCode);
	void	OnDropdownCommand(UINT cmdID);
	void	loadTemplate(CPoint pt);
	void	saveServerTmpl();
	CString	getControlName(int kind, int type);
	void	fileOpen(CString path, bool bDefault = false);
	virtual ~CMainFrame();
protected:
private:
	CString	Parser(CString &srcstr, CString substr);
	void	loadControlEvent();
public:
	void MakeDrawBar();
	void MakeExBar();
	void MakeAlignBar();
	void MakeMainToolBar();
	long GetCYSDIFrame();
	void SDIShowChild(BOOL bShow = TRUE);
	void SetToolbarPosition();
	int GetToolbarPriority(CString info);
	CRect ParseRect(CString str);
	void ParseToolBar(int idx, CString info);
	void SetToolBarInfo(int idx, CRect rc, DWORD style, int nState, int nRows);
	CString GetToolBarInfo(int idx);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	bool		m_showFORM;	// show / hide form
	CString		m_root, m_ips, m_srcviewer;
	CString		m_curDir;
	CString		m_openDir;
	class	CAxisPalette*	m_palette;
	class	CAxisDraw*	m_draw;

	CStatusBar	m_statusBar;
	CTrueColorToolBar	m_mainTB;
	CTrueColorToolBar	m_drawTB;
	CTrueColorToolBar	m_alignTB;
	CTrueColorToolBar	m_exTB;
	CPropertiesBar	m_wndPropertiesBar;
	CScriptBar	m_wndScriptBar;
	CPreViewBar	m_wndPreviewBar;
	CTraceBar	m_wndTraceBar;
	CSelectionBar	m_wndSelectionBar;
	CPoint		m_childPnt, m_dropPnt;
	
	struct	_block		m_block;

	bool		m_request, m_bOffline;
	CString		m_memory;
	bool		m_copied;	// enable paste
	int		m_copiedFORM;
	int		m_copiedREP;
	int		m_copiedGR;
	HANDLE		m_memFORM;
	HANDLE		m_memREP;

	CLogin		m_LoginDlg;

private:
	CMapStringToPtr m_controlEvent;
	CMapStringToString m_backupSSMap;
	CTips		m_tips;

// Generated message map functions
protected:
	void LoadScriptColors();
	int	initDir();
	void	setExChecked(int edit);
	void	createNewChild(int mapK);
	void	openLocalFILE();
	class	CChildFrame*	openMAP(CString fileN);
	void	saveLocalMAP();
	void	saveLocalPath();
	void	saveMAP(CChildFrame* child);
	void	closeChild();
	int	generateMAP(bool alarm = true, CChildFrame *child = NULL);
	void	compileMAP(int flag = 0);
	void	applyAXIS(int flag = 0);
	void	reloadMAP();
	void	showFORM();
	void	setSourceViewer();
	void	showSource();
	void	orderFORMs();
	void	setServerIP();
	void	saveServerMAP();
	void	batchCompile();

	void	GetChildStack(CArray <CChildFrame*, CChildFrame*>&Stack);

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToolbarDropDown(NMHDR* pnmtb, LRESULT *plr);
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditFindReplace(CCmdUI* pCmdUI);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	afx_msg void OnUpdateMainCommand(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExCommand(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCommand(CCmdUI* pCmdUI);

	afx_msg void OnMainCommand(UINT cmdID);
	afx_msg void OnAlignCommand(UINT cmdID);
	afx_msg void OnExCommand(UINT cmdID);
	afx_msg void OnDrawCommand(UINT cmdID);

	afx_msg	LRESULT OnStatus(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnOpenObject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnLoginMessage(WPARAM wParam, LPARAM lParam);

	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnFindReplaceMessage(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnFireEvent(long type, long pBytes, long nBytes);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__428255A8_45FC_11D4_A024_00001CD7F9BE__INCLUDED_)
