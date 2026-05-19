
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

//#include <afxcoll.h>
#include "mapView.h"
#include "FileView.h"
#include "symbolView.h"
#include "tmplView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "scriptWnd.h"
#include "mdiTabs.h"
#include "tips.h"

#include "h/mapvar.h"
#include "h/mapform.h"
#include "awWcc/libWcc.h"
#include "awDlg/login.h"
#include "h/mapxml.H"
#include "MTLayout.h"

#define	MS_MODAL	0x00000001

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CString	m_root;

	CArray	< class CObjectLoad*, CObjectLoad* > m_AryCloneObject;
	DWORD	m_status;
	CArray	< class CChildFrame*, CChildFrame* > m_children;

	CMapToken* m_pToken;
	CNode	m_ElementNode;
	CNode	m_CellPropNode;
	class	CGroup*	m_pGroup;
	CString	m_sGroup;
	CString	m_currentPath;
	CString	m_sDefFontName;
	CString	m_sDefFontPnt;
	CString	m_initMap;
	CMDITabs	m_wndMDITabs;

	CString	m_userID;
	CString	m_pass;
	CString	m_domain;
	CString	m_projectName;

	HHOOK	m_hook;
	FARPROC	m_callproc;
	class	CResourceHistDlg	*m_resouceDlg;
	class	CADBDlg			*m_pCADBDlg;
	bool	m_bBKLine;

	bool		m_bLayoutMode;
	CString m_strCtmObj;
	CPoint m_dropPt;

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar	m_wndMenuBar;		// menu

	CMFCToolBar	m_mainTB;
	CMFCToolBar	m_drawTB;
	CMFCToolBar	m_alignTB;
	CMFCToolBar	m_etcTB;

	CMFCStatusBar	m_statusBar;

	CTabbedPane*	m_tabBar;
	CMapView	m_wndMapView;		// server map
	CFileView	m_wndFileView;		// local pc
	CSymbolView	m_wndSymbolView;	// symbol
	CMapView	m_wndTrView;		// trlayout
	CTmplView	m_wndTmplView;		// template map

	CPropertiesWnd	m_wndProperties;

	CScriptWnd	m_wndScript;

	COutputWnd	m_wndOutput;

	CWnd*	m_wizard;

	bool	m_showFORM;	// show / hide form
	CString	m_curDir;
	CString	m_openDir;
	class	CAxisPalette*	m_palette;
	class	CAxisDraw*	m_draw;
	class	CiForm*		m_piForm;

	CPoint	m_childPnt;
	CPoint	m_dropPnt;
	
	CLogin	m_loginDlg;
	class CSearchDlg* m_SearchDlg;
	class CbuilderLogin* m_builderLogin;
	CString m_strDiffViewer;

private:
	CMapStringToPtr m_controlEvent;
	int	m_displayMode;
	bool	m_offline;
	CTips	m_tips;
	class	CTrLayout*	m_trDlg;
	class	CRtsInfo*	m_rtsDlg;
	bool	m_bMasterLayout;
	CMTLayout m_MTLayoutDlg;
	CObArray m_ArryNewMap;
	CStringArray m_aryFile;

// 함수
public:
	void	ObjClone(CObjectLoad** dstObj, CObjectLoad* srcObj);
	bool	deleteAryCloneObj();
	void	GetHistoryDlg(CString sMap);
	void	CheckOutDlg(CString sName);
	void	openFile(CString path, bool bDefault = false);
	bool	DownMapDlg(CString sName, int mode = 0, int nSel = 0);
	bool	DownDiff(CString sName, int nNewSel, int nOldSel);
	void	ShowDiff(CString sName, int nNewSel, int nOldSel);
	CString setMapPath(CString mapN);
protected:
	BOOL	createToolBars();
	BOOL	createDockingWindows();
	void	setDockingWindowIcons();

	void	BuilderLogin();
	void	BuilderLogout();

	void	setToolbarPosition();
	int	getToolbarPriority(CString info);
	CRect	parseRect(CString str);
	void	parseToolBar(int idx, CString info);
	CString	getToolBarInfo(int idx);
	void	pressButton(UINT id);

	void	loadScriptColors();
	int	initDir(CMapStringToString& maps);
	void	createNewChild(int mapK);
	void	openLocalFile();
	CChildFrame*	openMap(CString fileN);
	void	openForm(CString fileN, CObjectLoad* ObjLoad);
	void	FormResize(CformProp* dstForm, CWnd* srcWnd, mapForm* mapH);
	void	loadTemplate(CPoint pt);
	void	saveLocalFile();
	void	saveLocalPath();
	void	SetMasterlayout();
	void	ResetMasterlayout();
	void	saveMap(CChildFrame* child);
	void	saveToServer();
	void	saveTmplToServer();
	void	closeChild();
	void	removeChild(void* rChild);
	int	generateMap(bool alarm = true, CChildFrame *child = NULL);
	void	compileMap(bool all = false);
	void	applyAXIS(bool all = false);
	void	reloadMap();
	void	showFORM();
	void	setSourceViewer();
	void	showSource();
	void	setServerIP();
	void	batchCompile();

	bool	dropInChild();
	void	loadControlEvent();
	bool	SearchElement(int startIdx, CString Element, CString& strData);
	void	MakeCellProp(CformProp* Prop, CString Element, int cnt);
	void	setDataset(CChildFrame* child, int type, CString info);
	void	editTrLayout(CString path = _T(""), bool breset = false);
	BOOL	isTrLayout(CString path);
	CString	UTF8ToAnsi(char* pszUTF8);
	void	editRtsInfo();

private:
	bool	registerCtrl(CString name, bool showMsg = false);
	void	startWorkshop();
	BOOL	login();
	void	loginWizard();
	void	changeMap(CString src);
	void	reCreateMDITabs();
	bool	checkInEditing(CString mapN, BYTE mapK);

	void	makeDir(CString root);
	CString getClientIP();
	int	getPort();
	CString	getObjectName(int kind, int type);
	char	getObjectKind(CString strName, CString strSubName);
	CString	getErrString(int nCode);

	CString	parse(CString &srcstr, CString substr);
	void	SetCellProp(CformProp* Prop, int iCnt);
	bool	CheckFile(CString fileN);
	void	SetDiffViewer();
	void	ShowADBViewer();
	void	ShowPreview(int iStart);
	void	ShowCtmObjDlg();
	void	SetCtmObjProp(DWORD idx, CString strProp);

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

public:
	void	OnColumnEdit();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnShowPopupMenu(CMFCPopupMenu* pMenuPopup);

	afx_msg void OnClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditFindReplace(CCmdUI *pCmdUI);

	afx_msg void OnMainCommand(UINT cmdID);
	afx_msg void OnEtcCommand(UINT cmdID);
	afx_msg void OnAlignCommand(UINT cmdID);
	afx_msg void OnDrawCommand(UINT cmdID);
	afx_msg void OnBookmark(UINT cmdID);
	afx_msg void OnUpdateMainCommand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEtcCOmmand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAlignCommand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawCommand(CCmdUI *pCmdUI);

	afx_msg	LRESULT OnOpenObject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnLoginMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);

	afx_msg void OnFireEvent(long type, WPARAM wParam, LPARAM lParam);
	DECLARE_EVENTSINK_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditFormsearch();
};


