
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include "mapView.h"
#include "FileView.h"
#include "symbolView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "scriptWnd.h"
#include "mdiTabs.h"
#include "tips.h"
#include "TrLayOut.h"
#include "Rtsinfo.h"

#include "h/mapvar.h"
#include "h/mapform.h"
#include "EmulCtrl.h"
#include "amDlg/login.h"
#include "LayOutDlg.h"

#define	MS_MODAL	0x00000001
#define CONTROL_POP_ID	100000

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
	CString		m_root;
	CString		m_EmulPath;
	CString		m_SourcePath;
	CString		m_MergePath;
	struct _rsmH*	m_sndH;
	class	CADBDlg			*m_pADBDlg;
	double		m_EmulResolution;

	struct	_mapH	m_clipMAP;
	DWORD	m_status;
	CArray	< class CChildFrame*, CChildFrame* > m_children;
	CArray  <CRect, CRect> m_Line;

	class	CGroup*	m_pGroup;
	class	CResourceHistDlg	*m_resouceDlg;
	CString		m_sGroup;
	CString		m_currentPath;
	CString		m_sDefFontName;
	CString		m_sDefFontPnt;
	CString		m_initMap;
	CMDITabs	m_wndMDITabs;

	CString		m_userID;
	CString		m_pass;
	CString		m_projectName;

	CTrLayOut	*m_trLayout;
	CRtsinfo	*m_rtsInfo;
	bool		m_bsettrlayout;
	bool		m_bLayoutMode;

	HHOOK		m_hook;
	FARPROC		m_callproc;

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
	CMapView	m_wndSymbolView;	// symbol

	CPropertiesWnd	m_wndProperties;
	CScriptWnd	m_wndScript;
	COutputWnd	m_wndOutput;

	bool	m_bShowFlag;	// Main (show / hide)
	bool	m_showFORM;	// show / hide form
	CString	m_curDir;
	CString	m_openDir;
	class	CAmPalette*	m_palette;
	class	CAmDraw*	m_draw;

	CPoint	m_childPnt;
	CPoint	m_dropPnt;
	
	struct	_block	m_block;
	CLogin	m_loginDlg;
	class CwSock*	m_pSock;
	class CSearchDlg* m_pSearchDlg;

public:
	void	trlayout(CString path = _T(""), bool breset = false);
	BOOL	IsTRLayout(CString path);
	CString	Parser(CString &srcstr, CString substr);
	void	SettTrRtsDataset(int kind, CString data);
	void	rtsInfo();
	void	DownMapDlg(CString sName, int mode = 0, int nSel = 0);
	void	DownMapDlgEx(CString sName, CString ex, int nSel);
	bool	OpenServerMap();
	void	DownTRlayOut(CString sName, int nSel);
	void	CheckOutDlg(CString idno, CString sName);
	void	GetHistoryDlg(CString sMap);
	CChildFrame* openFile(CString path, bool bDefault = false);
	void	moveMobileDevice();
	bool	loadLanguage();
	CStringW ParserW(CStringW &srcstr, CStringW substr);
	BOOL    IsLanguage(CString str){ CString sval;  return m_mapLanguage.Lookup(str, sval); }
	void	ShowADBViewer();
	void	ShowDiff(CString sName, int nSel);
	bool	DownDiff(CString sName, int nSel, int nPre);
	CString GetMergePath(){ return m_MergePath; }
	void	SetMasterlayout();
	void    FormSearch();
	void	DestroyEmulator();
private:
	CMapStringToPtr m_controlEvent;
	int		m_displayMode;
	bool		m_offline;
	CTips		m_tips;
	CEmulCtrl*	m_pEmul;
	CMap<CChildFrame*, CChildFrame*, CChildFrame*,CChildFrame*>	m_mapEmul;
	CMapStringToString	m_mapLanguage;
	CString		m_strDiffViewer;
//	CLayOutDlg	m_layoutDlg;
protected:
	BOOL	createToolBars();
	BOOL	createDockingWindows();
	void	setDockingWindowIcons();

	void	setToolbarPosition();
	int	getToolbarPriority(CString info);
	CRect	parseRect(CString str);
	void	parseToolBar(int idx, CString info);
	CString	getToolBarInfo(int idx);
	void	pressButton(UINT id);

	void	loadScriptColors();
	int	initDir(CMapStringToString& maps);
	void	createNewChild(int mapK);
	CChildFrame*	createEmulChild(CChildFrame* childOld);
	struct _mapH	CopyMAPH(int nCnt, struct _mapH* mapH);
	void	openLocalFile();	
	CChildFrame*	openMap(CString fileN);
	void	saveLocalFile();
	void	saveLocalPath();
	void	saveMap(CChildFrame* child);
	void	saveToServer();
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

	CString	m_path;
	CString	setMapPath(CString mapN);
	void	amSockopen(enum nSatatus);
	void	Login(CString sUserID, CString sPass);
	void	OfflineMode();
	void	OpenLayOut();
private:
	void	startWorkshop();
	void	MakeDocking();		// 화면 내리면 도킹레지스트리가 깨짐 방지를 위해 생성
	BOOL	LoginEmule();
	void	changeMap(CString src);
	void	reCreateMDITabs();
	bool	checkInEditing(CString mapN, BYTE mapK);

	void	makeDir(CString root);
	CString getClientIP();
	int	getPort();
	CString	getControlName(int kind, int type);
	CString	getErrString(int nCode);

	CString	parse(CString &srcstr, CString substr);
	CString	TrackPopupControl();

	void	ExecuteCreateProcess(CString sAppName, CString sCommand = _T(""));
	void	OnViewStandardline();
	void	OnViewLanguage();


// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

public:
	void	OnColumnEdit();
	void	DownMapLoad();
	CString	UTF8ToAnsi(char* pszUTF8);
	void	EmulatorRun(CChildFrame *child);

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
	afx_msg LRESULT OnFireEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrace(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT trMaptree(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onsockmessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnOpenedSock(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT UploadRecvData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

};


