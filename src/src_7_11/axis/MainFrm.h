// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////


#pragma once
#include <afxtempl.h>
#include <afxmt.h>
#include <TlHelp32.h>

#define LM_TRAY_TIP_MESSAGE            28000
#define WM_DIALOG_SHOW    WM_USER + 101  //다이얼로그감추기/보이기
#define WM_APP_EXIT               WM_USER + 102  //다이얼로그종료
#define WM_TRAYICON               WM_USER + 103  //트레이아이콘메시지


#define NCAP_CLOSE		0
#define NCAP_MAX		1
#define	NCAP_RESTORE		2
#define NCAP_MIN		3
#define NCAP_VSCREEN		4
#define NCAP_FUNC		5
#define NCAP_MAXCNT		6

#include "ioformat.h"
#include "GuideMsgDLG.h"
#include "AccnAdminDLG.h"
#include "ConclusionDLG.h"
#include "../dll/axissm/svband.h"
#include "AxWizard.h"
#include "FormManager.h"
#include "component\mybar.h"
#include "component\OutputWnd.h"
#include "../h/jmcode.h"
#include "Crypt.h"

//char* g_logindata ;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
	friend class CFormManager;
public:
	CMainFrame();

// Attributes
public:

	//ofstream	m_dFile;	// by NMH
	CFormManager*   m_pFormMgr;
	CString		m_mapN;
	CString		m_regkey;
	CAxWizard*	m_wizard;
	CWnd*		m_category;
	CWnd*		m_memo;

	int		m_idletime;
	int		m_helpKey;
	int		m_linkbar;
	int		m_iMenuCnt;
	

	bool		m_block;
	bool		m_blinkbar;
	bool		m_saveALLVS;		// save option userworkspace
	bool		m_bCustomer;
	bool		m_bSwitch;
	bool		m_bpcenable;
	bool		m_bSetMenu;

	HICON		m_dtIcon;
	HHOOK		m_hHook;
	class	CAxMisc*	m_axMisc;

	bool		m_bSDI;
	CString		 m_sSS;

// view hook
	CView*		m_view;
	FARPROC		m_callproc;

	CGuideMsgDLG*	m_GuideDLG;
//	CAccnAdminDLG*	m_AccnDLG;
	CConclusionDLG*	m_ConCDLG;

	int	GetVSN() {return m_vsN;}

	class _axisRes*		m_axisres;
	class CDbar1*		m_bar1;   //메인툴바
	HRGN			m_rgn;

	Crypt			m_crypt;

	bool			m_bMngShow;

	char*			m_plogindata;

	CStringArray    m_arrIP;
protected:
	CWinApp* m_axis;
	CWnd*   m_pBondWnd;   //관심종목 FS900100 화면의 주소
	CWnd*   m_paxiscode;
	char	m_progK;

	CString	m_home;
	CString	m_user;
	CString	m_pass;
	CString	m_cpass;
	CString	m_userN;
	CString	m_MMapN;
	BOOL	m_bwaveF;
	CString	m_waveF;
	CString	m_alarmMsg;
	CString	m_printS;
	CString	m_activeMapN;
	CString m_uuid;

	CString m_errMsg;     //에러메시지
	CString	m_strcomp;    //회사코드
	CString m_strauth;    //사용자권한
	CString m_strupjong;  //업종
	CString m_strdept;    //부서
	CString m_strreserve; //예비용
	CString m_strldat;    //마지막 날짜
	CString m_strliem;    //마지막 시간

	CString		m_strMsginfo;  //예비용 메시지 정보
	CStringArray  m_arrPopMsg;
	CStringArray  m_arrloginPopMsg;
	CStringArray  m_arrNoticemap;

	bool m_buuidcreate; //사용자고유키 재생성여부 (login input data)
	
	CString	m_ip;
	CString	m_port;
	CString	m_regMR;	// market Radar regstry key
	CString	m_cnameMR;	// market Radar Class Name;
	HWND	m_hMR;
	CString	m_SVRNM;

	CStringArray m_msgArr;

	bool	m_dev;
	bool	m_update;
	bool	m_fullmode;
	bool	m_bExit;
	bool	m_bSrnSave;
	bool	m_forceClose;
	bool	m_runAxis;
	bool	m_noActive;
	bool	m_fit;
	
	bool	m_mngSound;	// 알림 및 장운영정보 소리알림 옵션
	bool	m_mngPopup;
	bool	m_mngGongji;
	bool	m_mngSuyo;
	bool	m_mngBal;

	bool	m_bexpivt;      //전문투자자여부 (Y/N)
	bool	m_bbonddeale;   //채권전문딜러여부 (Y/N)
	bool	m_bpwch;        //비밀번호변경(1:변경필요)
	bool	m_bdupc;		//동시접속 여부
	bool	m_btmpuse;      //임시사용여부 (사용자 고유키 초기화 요청 혹은 회사변경 오청시 당일 임시사용 여부
	bool	m_buserst;      //이용제한 요청여부 (이용제한요청되었으나 승인이 안되었을경우)
	bool    m_bemailfrm;    //email activate 여부
	bool	m_bnotice;      //공지팝업 존재유무
	bool	m_buuidrec;      //사용자고유키 재생성여부
	BOOL	m_activeCap;

	bool    m_bChekcTr;     //프로프레임 서버 정상여부를 판단하기 위해 프로그래 시작시 날려본다


	DWORD	m_toolStatus;
	DWORD	m_conKIND;
	int	m_conHISTORY;
	int	m_conAUTOTIME;
	int	m_conPOS;
	int	m_mngPos;
	int	m_activeKey;
	
	int	m_SDIHEIGHT;
	int	m_maxChild;
	int	m_fontSize;
	int	m_posChild;

	int m_messangerTry;

	int m_iTimeMng;
	
	UINT	m_appMode;

	int	m_xtrKEY;
	int	m_vsN;
	int	m_connectBy;
	int	m_downI;
	int	m_action;
	int m_errCode;  //0 : 정상
					//1 : ID/PW 오류
					//2 : 이용제한자
					//3 : 사용자 고유키 초기화 오쳥(미승인 -> 당일 아님)
					//4 : 소속회사 변경 요청(미승인 -> 당일 아님)
					//5 : 사용자 고유키 오류(분실로 PC 자동 재생성 및 고유키 초기화 필요)
					//6 : 사용자 고유키 불일치
	char*	m_wb;
	SYSTEMTIME		m_connectT;
	class CMPop*		m_modal;
	class CTMenu*		m_tMenu;
	class CCodebar*		m_codebar;
	class CDbar2*		m_bar2;     //화면제어바
	class CSmain*		m_smain;
	class CSdibar*		m_sdibar;
	class CAxGuide*		m_axGuide;
	class CConnect*		m_axConnectD;
	class CPrinterSettings* m_printsetup;
	class CGPop*		m_stoploss;
	class CManageInfo*	m_mngInfo;
	class CChaser*		m_chaser;

	class CSmcall*		m_smcall;
	class CNPalette*	m_palette;
	class CTTip*		m_tip;

	class CLockDlg*		m_lockDlg;

	CMyBar		m_wndMyBar;
	COutputWnd        m_wndOutput;
	
	//CMapStringToString	m_tabviewlist;
	CMapStringToString	m_mapExpectSymbol;
	CMapStringToString	m_mapMsg;
	CStringArray		m_savelist;
	CMapStringToString	m_mapAlarmList;	
	CStringArray		m_mapGongji;
	CStringArray        m_arrMenu;

	CMap	<int, int, class CChildFrame*, class CChildFrame*>	m_arMDI[4];
	CMap	<int, int, class CSChild*, class CSChild*>		m_arSDI[4];
	CMap	<int, int, class CGPop*, class CGPop*>			m_arGPOP;
	CMap	<int, int, class CMPop*, class CMPop*>			m_arMPOP;

	CArray <CJCODE*, CJCODE*>	m_arrayCJCode;
	//CArray <AAGCODE*, AAGCODE*> m_arrayAGCode;

	CArray	<class CChildFrame*, class CChildFrame* >		m_arHide;
	CArray	<int, int>	m_hooklist[6];
	CArray	<int, int>	m_sdiZORDER[6];
	CArray	<int, int>	m_waitlist;
	enum	{axNONE, axOPEN, axOPENPPP, axOPENRSM, axOPENWSH, axOPENSIGN, axDONE, axCLOSE} m_step;

	struct	IWebBrowserApp* m_pWBApp;
	HWND			m_WBWnd;
	HICON			m_hIcon;

	NOTIFYICONDATA		m_tray;
	CArray	< CRect, CRect > m_arRc;

	CCriticalSection	m_sync;
	CBitmap			m_bmCaption[2];

	BOOL	startUDP();
	void	cleanUDP();

	char*	m_pAccountTotal;
	int	m_nAccountLength;

	HINSTANCE m_hMISC;
	HINSTANCE m_hGrid;


	void SetActivate(UINT nState);
	class CSVband*	m_Vband;

public:
	bool	m_bServerError;  //url 을 통해서 장애여부판단 구분자 : 장애시 N  정상 Y
	
	bool	m_enableAkey;
	int	m_xvalueAkey;		// all cancel key value
	bool	m_enableBkey;
	int	m_xvalueBkey;		// all code cancel key value
	bool	m_enableCkey;
	int	m_xvalueCkey;		// before cancel key value
	bool	m_enableDkey;
	int	m_xvalueDkey;		// before edit key value
	int	m_tickBeEdit;
	bool	m_enableEkey;
	int	m_xvalueEkey;		// sel order key value
	int	m_tickSelOrder;
	bool	m_enableFkey;
	int	m_xvalueFkey;		// buy order key value
	int	m_tickBuyOrder;
	bool	m_enableGkey;
	int	m_xvalueGkey;		// all order key value
	int	m_tickAllOrder;
	bool	m_enableHkey;	
	int	m_xvalueHkey;		// hogawnd sort key value
	bool	m_enableHogakey;
	int	m_xvalueHogakey;
	bool	m_enableA3Mkey;	
	int	m_xvalueA3Mkey;		// 자동3수동초기화 key value

	bool	m_enableNkey;
	int	m_xvalueNkey;		// 야간 all cancel key value

	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	class CMDIClient*	m_MClient;
	WINDOWPLACEMENT		m_pl, m_switchpl, m_sdipl;

// Generated message map functions
public:
	bool	Start(CString user);	
	bool	setFormPos(CWnd* pChild, CString sMap, int key, int position, CPoint point, int width, int height);
	int	CopyScreen(CString mapN, int group, int fontsize, CPoint sp);
	int	ChangeChild(int index);	
	int	CreateModal(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, int actkey = 0, CPoint point = CPoint(-1, -1));	
	void	change_Skin(/*CString skinN*/);
	
	void	restore(int key);
	void	ShowMngInfo(CString dat);
	void	ShowFITSInfo(CString dat);
	BOOL	ConclusionNotice(CString dat, CString& title);
	BOOL	HideGuide();	
	bool	HotKey(WPARAM wParam, LPARAM lParam);
	
	//void	process_ConSee(int option);
	void	funcKey(int funcID, int index = -1);
	int	CheckMapping(CString mapN);
	void	HelpLink(int key);
	void	HelpLinkS(int key);

	void	SmCall(int idx);
	void	AppendAllMap();
	void	sendTicInfo();
	void	sendConnectInfo();

	void	GetDispN(char* mapN);
	int	InputScreenNo(CString dispN);

	void	TotalSetup(int index = 0, int tick = 0);
	void	ESetup();
	void	SetMng();
	void	EditTool();
	void	EditHotkey();
	void	SetTicker(int id = 0);
	void	OrderSetup();
	void	ImageSetup();
	BOOL	Change_VsKey(int index);
	void	ConfigTicker(bool reconfig = false);
	void	Action(WPARAM wParam, LPARAM lParam);
	void	GetCaptionInfo(WPARAM wParam, LPARAM lParam);
	void	HideChildButton(int key);
	CString	GetSkinName();
	void	convertSingle(int key, int vsN);
	bool	FindSDIMap(CString mapName, int vsN =-1);
	bool	CheckSDIMap(int key);
	void	AppSwitch();
	void	ChangeMDI();
	void	ChangeSDI(bool hide = false);
	void	ChangeTRAY();
	void	TrayCmd(int cmd);
	void	displayGuide(CString guide, CWnd* owner = NULL);
	void	displayMsgBox(CString caption, CString guide);
	void	SetHome();
	bool	IsDll(CString mapN);
	int	IsHideCaption(CString mapN);
	void	LinkOPEN(int dat, CString mapN);
	void	changeLinkInfo(class CLinkbar* linkBar, CString mapname, bool bKeepbar = false);
	void	changeAllLinkInfo(CString mapname);
	bool	isShowLinkbar(CString mapN);
	void	parseAry(CString data, CStringArray& ary, CString separate);
	void	refreshMap(int key);
	bool	select_SName(CString input);
	void  DoFunc(int funcID);

	void	SetBZ();
	void	GetDialogColor(COLORREF &clrDialog, COLORREF clrText);
	int	DisplaySystemVersion();
	long	DoChildKey(WPARAM wParam, LPARAM lParam);
	long	DoPopupKey(WPARAM wParam, LPARAM lParam);
	long	DoSDIKey(WPARAM wParam, LPARAM lParam);
	long	DoHotKey(WPARAM wParam, LPARAM lParam);

	void	LoadCJCode();
	void    addCJCode(struct CJcode   *pcode);
	void    addCJCode(CString strdata);
	void	CreateBonddlg();
	CString Onl_seedenc(CString s_text, BOOL bBase64 = TRUE);	
	CString Onl_seeddec(CString s_text, BOOL bBase64 = TRUE);
	CString	CreateUUID();
	void	convertSDI(int key, int vsN);
	void	RemoveMdi(int vsn, int key);
	void	SetFileVisible(CString strpath, bool bShow = false);
	bool	IsUUidCreated(CString struser);
	void	set_Maps(CString mapN);
	void	GetLocalIP();
	void	SetLogindata(char*	pdata);
	void	SendUseMap(CString strmap);
	void	CheckTR();
	void	ResetUserstat();
	void	SendMsgToMessanger(int gubn, CString strdata = _T(""));
	void	ErrReport(CString eMsg);		//e_log
	void	CheckMsgType();

	BOOL	Init_SharedMemory();
	BOOL	IsExistInMenuFile(CString strmap);
	void	MakeMenuArray();


	void	ReadUserLast(CString strkey, CStringArray& arr);
protected:
	int	Initialize();
	void	ConfigFrame();
	void	DrawFrame();	
	void DrawFrame(CDC* dc);
	void	ProcessSecure();
	void	UnregisterControl();
	void	registerControl();
	void	signOn();
	void	endWorkstation();

	void	CreateTB();

	bool	closeX();

	bool	closePopup();
	bool	closePopup(int key);
	bool	runCommand(int comm, LPARAM lParam);
	bool	Agree_Duplication(CString mapN);

	int	create_Newview(int actkey, char* data);
	void	setTitle(int key, CString title);
	void	SetSCN(CWnd* wnd, int key, CString mapN, CString title, bool bSDI = false);
	void	loadMap(int key, int size);
	void	showfname();
	void	changeSize(int key, CSize size);

	void	positionWindow(int actkey, int poskey, int pos, bool child = true);
	void	positionWindow(int actkey, CWnd* posW, int pos, bool child = true);
	void	positionWindow(CWnd* actW, CWnd* posW, int pos, bool child = true);

	void	positionSWindow(int actkey, CWnd* posW, int pos, bool child = true);
	void	positionSWindow(CWnd* actW, CWnd* posW, int pos, bool child = true);

	BOOL	getConnectInfo(CString& ips, int& port);
	void	update_ticker(int kind, CString dat);
	void	change_BackGround();

	void	LockProg();
	void	fullScreenMode(bool full = true);

	void	createUserScreen(int key);
	void	createUserScreen(CString mapN, bool allVS = true);
	void	makeUserScreenMenu();
	bool	modifyWorkspace(bool add, CString keys, bool allvs = false);
	void	save_user(CString sSave);
	void	save_laststat();
	void	getScreenData(int key, CString& data, BOOL save = TRUE);
	BOOL	getPopupMap(CString name);
	void	loadDomino(int key, CString mapN, bool bSDI = false);

	void  saveClip(int entire = 0);
	void	saveImg(int entire = 0);
	BOOL	printImg(CString dat = _T(""));
	//BOOL	writeImg(CString dir, int entire = 0);
	BOOL	writeImg(CString dir, int entire = 0, BOOL bClip = FALSE);
	BOOL	writeDIB(LPTSTR szFile, HANDLE hDIB);
	HANDLE	writeDDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal);

	bool	IsSelectMap(CString mapN);
	CString	getSMap(CString mapN);
	LPCTSTR	getPrintName();
	LPDEVMODE getPrintMode();

	void	setUDP();
	void	beginWait(int key = -1);
	void	endWait(int key = -1);
	void	removeWait(int key);
	void	setCodeReg(int cnt, CString dat);
	
	void	RunWebBrowser(CString HomePage, int cx = 820, int cy = 600);
	void	ExitWebBrowser();
	bool	changeMAX(CString mapN);
	void	load_eninfomation(bool first = true);
	void	load_MngSetup();
	void	load_ImageSetup();
	bool	maxChild();
	void	detachChild(int key);
	void	saveExitMap();
	CWnd*	getActiveView(int key);
	void	load_history();
	void	save_history();
	void	add_history(CString mapN);
	void	addTool_history(CString mapName);
	void	change_Resource();
	BOOL	ExistMenu(CString mapN);
	void	SetAMap(int key, char* mapN);
	void	SetSearchMap(char* mapN);
	void	NoticeParse(CString dat, CMapStringToString& ary);
	CString	parseData(CString dat);
	CString	GetUserName();
	CView*	GetNewView(int vwKIND);
	CView*	GetSDIView(int vwKIND);
	CSize	GetFrameGap(bool mdichild = true);

	void	toolRegister();
	void	toolRegisterAll(int index);
	void	AllScreenRegister();
	void	changeFontSize();
	void	processFMX(WPARAM wParam, LPARAM lParam);
	void	processTICK(CString dat);
	void	processINTER(char* dat);
	void	processCONNECTINFO(char *dat);
	void	PopupWeb(CString url, int cx = 780, int cy = 538);
	void	ParseSym(CString dat, CMapStringToString& ary);
	void	actionCaption(int key, int action);
	void	actionSCaption(int key, int action, bool bPopup = false);
	void	CreateChaser();
	void	DeleteChaser();
	void	CloseChaserAPP();
	void	showChaser();
	CString Parser(CString &srcstr, CString substr);
	void	WriteFile(char* pBytes, int nBytes);
	void	write_err();
	CPoint	getMinimizePos(int childKey);
	bool	IsvalidMap(CString mapN);
	bool	axFocus(int key);
	bool	axSFocus(int vsN = -1);

	void	changePassword(CString newpassword);
	void	ShowUScreenMenu();
	void	loadToolStatus();
	void	saveToolStatus();
	void	SetToolStatus();
	void	SetSDIHeight();
	void	SetSDIChangeHeight();
	CChildFrame*	load_hidescreen(CString mapname);
	void	keyArray(CArray <int, int> &ary, int vsN, bool bSDI = false);

	void	drawTitle(CDC* pDC);
	CRect	GetCapRect();
	void	SetRegion();
	int		DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown = false, BOOL bActive = FALSE, int maskcolor = RGB(255, 0, 255));
	int		GetSelectItem(CPoint pt);
	void	ShowToolTip(int nIndex, CPoint pt);
	void	fitDual();
	int		GetTicKey(int index);
	int		ConvertIndexKey(int index);
	void	convertIndex();
	void	deleteNewsfile();
	void	DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight = false);
	void	DrawTitleBitmap(CDC *pDC, CRect drawRC, BOOL bActive);
	int		GetMacAddr(char* ipaddr, char* data);

	CString	ReplaceExpectSymbol(CString sym);
	void	SetPichUser(BOOL hogafilter = FALSE);

	void	SetPinchData();
	void	ConfigTriggerSend();
	void	ALLTriggerSend(CString symbol);
	void	LoadUserScreen(CString sUser);
	void	createUserScreenEx(CString sUser, CString mapN, bool allVS = true);

	int	m_iIdle;
	int	m_iGridDrawing;
	int	m_iIdleDefault;
	int	m_iGridDrawingDefault;
	// 2011.08.29 scbang : 추가끝
	
	CString	m_strVirtualID[4];
	CString	m_strVirtualNAME[4];

	BOOL	m_bGuideTimer;
	BOOL	m_bGuideSet;
	int	m_nGuideInterval;
	int	m_nConInterval;
	DWORD	m_nConTickCount;
	
	int	m_nPopupPos;
	int m_iVS;	// virtual screen
	int m_imaxpop;  //0 최소 맵화면 닫기 1 메시지창 띄우기

	CString m_ipAddr;

	void SetHTSTitle();
	void GetledgerH(struct _ledgerH* pLedgerH, int trGubn = 0);
	bool sendTR(CString trC, char* pBytes, int nBytes, int trGubn = 0);
	void outputtrace(CString strdata);
	CString GetUUidByID(CString strid);
	LRESULT ApplySetup(UINT id);
	void Gongjiopen(CString mapN, int cnt);
	bool CheckGonjiMap(CString strmap);
	void MakeMenuFile(char *pdata, int icnt);
	void Gongjiopen();
	HWND GetWinHandle(ULONG pid);
	ULONG ProcIDFromWnd(HWND hwnd);
	HWND GetProcessHwnd(CString prcessName);
	bool CheckMessangerLogin();
	void SetRealIP();
	CWnd* GetMapWmd(CString strmap, bool& bmdi);
	CAsyncSocket* m_pSocket;
	bool CheckSoundOnOFF(CString strgubn);  //사운드 재생여부확인해서 재생 (현재 수요예측관련사운드만 있음)
	bool CheckUrl();	  //프로그램 시작시 접속서버장애여부를 url로 확인한다
	void SelectServerIP();  //접속하는 서버IP 2개중 하나를 선택한다
	bool CheckServer(CString data, CStringArray& arr);  //서버가 열려있는지 확인한다
	CWnd* m_pUnBondWnd;
	CWnd* m_pInterWnd;   //관심설정화면 주소
	int	m_interkey; // 종목검색다이알로그를 띄우는 관심설정화면키
	int	m_Bondkey; // 관심설정화면키
	bool m_bMainStart;
	
	void	PopAlarm(CString strdata);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcPaint();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnAppDummy(UINT ua);
	afx_msg void OnUpdateRunCommand(CCmdUI* pCmdUI);
	afx_msg	LRESULT OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnUSER(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnCHILDMSG(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnCHASER(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAxisClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrayEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetPinch(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrace(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessangerlogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessangerhandle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessangerend(WPARAM wParam, LPARAM lParam);
	DECLARE_EVENTSINK_MAP()
	afx_msg int OnFireRec(INT_PTR type, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	
		/*
	#define LOGIN       0   //로그인
#define GONGJITR	1   //공지팝업
#define MENUTR		2   //사용자별 메뉴
#define LOGOUT		3   //로그아웃
#define USEMAP		4   //사용한 화면
	*/
	int m_key;
	struct menumid  //사용자별 메뉴
	{
		char userid[30];
	};

	struct logoutmid  //로그아웃
	{
		char userid[30];
	};

	struct usemapmid //사용한 화면
	{
		char userid[30];
		char mapnum[6];
	};

	struct loginmid //사용한 화면
	{
		char userid[30];
		char password[16];
		char uuid[30];
		char uuidrect[1];
		char ip[1];
	};

	struct loginmod //사용한 화면
	{
		char userid[30];
		char name[30];
		char auth[7];
		char comp[6];
		char upjong[2];
		char expivt[1];
		char dept[6];
		char bonddealer[1];
		char pwch[1];
		char dupc[1];
		char tmpuse[1];
		char userst[1];
		char emailfrm[1];
		char notice[1];
		char uudierec[1];
		char reserve[20];
		char errc[1];
	//	char emsg[200];
		char ldat[8];
		char ltim[6];
		char compnm[50];
	};
};

/////////////////////////////////////////////////////////////////////////////

