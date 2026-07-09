// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "axis.h"
#include "comdef.h"
#include "mmsystem.h"

#include "MainFrm.h"
#include "childFrm.h"
#include "axisdoc.h"
#include "axisview.h"
#include "axscrollview.h"
#include "axtabview.h"
#include "connect.h"
#include "axmsg.hxx"
#include "axMisc.h"
#include "GPop.h"
#include "MPop.h"
#include "fname.h"
#include "lockdlg.h"
#include "screendlg.h"
#include "ExitD.h"
#include "PrinterSettings.h"
#include "ManageInfo.h"
#include "alarm.h"
#include "ndib.h"
#include "NPalette.h"
#include "passdlg.h"
#include "Chaser.h"
#include "NClock.h"
#include "SChild.h"
#include "Linkbar.h"
#include "TTip.h"
#include "DlgModaless.h"
// custom toolbar include

#include "PinchDLG.h"
#include "configDlg.h"
#include "HogaFilterDLG.h"
#include "SetBufferDlg.h"	

#include "../h/axis.h"
#include "../h/axisvar.h"
#include "../h/axiserr.h"
#include "../h/axisfire.h"
#include "../dll/axissm/smheader.h"
#include "../dll/axissm/axCommon.hxx"
#include "../appl/appl.h"
#include "../h/jmcode.h"


#include <io.h>
#include <math.h>
#include <fcntl.h>
#include <winspool.h>
#include <Windows.h>
#include <winuser.h>
#include <windef.h>
#include <mshtml.h>
#define COMPILE_MULTIMON_STUBS
#include "multimon.h"
#include <afxpriv.h>
#include <Mmsystem.h>
#pragma comment(lib, "winmm")
#include <afxinet.h>

#define DEV_IP "10.200.14.141|15201"
#define REAL1_IP "10.200.14.141|15201"
#define READ2_IP "10.200.14.141|15201"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_DLLDATA	WM_USER + 9111
#define WM_TRAYEVENT	WM_USER + 9222
#define WM_AXISCLOSE	WM_USER + 9444
#define	TRIGGERN	1

#define	SEP		0x7f
#define	USERSEP		'\t'
#define	WM_CLOSESM	WM_USER+702

#define	MENU_NOT	0x00
#define	MENU_NONE	0x01
#define	MENU_NORMAL	0x02
#define	MENU_SCROLL	0x04

//	init popup 
#define POP_SECURE	0x01
#define POP_AGREE	0x02
#define POP_STOCKHOLDER	0x04
#define POP_UNFAIRNESS	0x08
#define POP_INVALIDCHAR	0x10
#define POP_PERCREDIT	0x20

#define	TM_IDLE		9009
#define	TM_PURL		9010
#define TM_AUTOMNGINFO	9011
#define TM_MAINFOCUS	9012
#define TM_MAINMENU	9013
#define TM_MESSANGERLOGIN 9014
#define TM_CHECKTR 9015
#define TM_INTERMOVE	9016
#define TM_SHOWMNG	9008

#define	COLOR_TB	RGB(238, 238, 238)

#define	UMENUN		10
#define USERTOOLMENU	9

#define USERDEFSYM	"USRDEFDATA"

#define KEY_CODE	0xff-1
#define KEY_STOPLOSS	0xff-2	
#define KEY_MEMO	0xff-3

#define MODE_MDI	1
#define MODE_SDI	2
#define MODE_TRAY	3

#define TRAY_SHOWICON	0
#define TRAY_HIDEICON	1

#define TOOLSTATUS_MDI	TB_MAINBAR|TB_LISTBAR
#define TOOLSTATUS_SDI	TB_SDIBAR|TB_SDIMAINBAR|TB_LISTBAR

#define	GAP	2
#define SRCMASK	0x00220326    // mask
#define	XGAP	GetSystemMetrics(SM_CXFRAME)
#define	YGAP	GetSystemMetrics(SM_CYFRAME)

#define SYM_MNG		"XJANG"
#define MNG_GUBN	"047"		// 0:장운영정보 1:알림메세지
#define MSG_BELLCODE	"600"		// wave 파일
#define MNG_MSG		"023"		// 메세지
#define MNG_KEYVALUE	"601"	

#define LOGIN       0   //로그인
#define GONGJITR	1   //공지팝업
#define MENUTR		2   //사용자별 메뉴
#define LOGOUT		3   //로그아웃
#define USEMAP		4   //사용한 화면

#define LENPUSH 30

#define GONGJIMAP _T("FS603000")
#define ALARMMAP  _T("FS604000")

struct	_userWH64	{
	char		type;			// window type
	unsigned char	key;		// window key
	char		group;			// trigger group
	POINT		pos;			// window position
						// pos.x == -1. pos.y : pos value
	char		maps[8];		// MAPs(8) + domino DATAs
};
#define	L_userWH64	sizeof(struct _userWH64)

static CRect	g_monRc(0, 0, 0, 0);
static CMainFrame* m_pMain = NULL;
static bool axiscall(int, WPARAM, LPARAM);
#pragma	comment(lib, "Winmm.lib")

#include <MMSystem.h>

static CPassDlg* pPWD = NULL;

#define	ENPIA		// ip maste
#define COLOR_DIALOG_BK		91
#define COLOR_DIALOG_TEXT	63

#define osVISTA			6

const char gnCHAR	= 0x7F;
const char tabCHAR	= 0x09;
const char lfCHAR	= 0x0A;
const char crCHAR	= 0x0D;

#define charToint	0x30

 #pragma comment (lib, "version.lib")
#define BONDCODE	_T("IssBondMast.txt")

#define HANDLESEND			0   //HTS 핸들전송
#define EXITMGSSEND			1   //HTS종료 메시지 전송
#define SHOWMSGSEND			2   //메신저 SHOW 메시지 전송
#define RUNANDLOGINSEND		3   //메신저 실행 및 로그인
#define LOGINSEND			4   //메신저 로그인
#define LOGINCANCELSEND		5	//메신저 로그인 취소
#define NOTESEND			6   //쪽지창 띄우기
#define PROFILESEND			7   //사용자 프로파일 띄우기
#define ONETOONESEND		8   //1:1일 대화

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
#define USERFRAME		// user frame draw.........Don't call CMDIFrameWnd::OnNcPaint();
#define WM_NCMOUSELEAVE		0x02A2
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_NCACTIVATE()
	ON_WM_NCPAINT()
	ON_WM_MEASUREITEM()
	ON_WM_ACTIVATE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_APP_BASE, ID_APP_END, OnAppDummy)
	ON_COMMAND_RANGE(ID_MENU_BASE, ID_MENU_END, OnAppDummy)
	ON_UPDATE_COMMAND_UI_RANGE(ID_APP_BASE, ID_APP_END, OnUpdateRunCommand)
	ON_MESSAGE(WM_AXIS, OnAXIS)
	ON_MESSAGE(WM_USER, OnUSER)
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
	ON_MESSAGE(WM_CHASER, OnCHASER)
	ON_MESSAGE(WM_TRAYEVENT, OnTrayEvent)
	ON_MESSAGE(WM_AXISCLOSE, OnAxisClose)
	ON_MESSAGE(WM_GETPINCH, OnGetPinch)
	ON_MESSAGE(WM_TRACE, OnTrace)
	ON_MESSAGE(WM_USER + 1, OnMessangerhandle)
	ON_MESSAGE(WM_USER + 2, OnMessangerend)
	ON_MESSAGE(WM_USER + 3, OnMessangerlogin)
	ON_WM_NCCALCSIZE()
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainFrame, CFrameWnd)
	//{{AFX_EVENTSINK_MAP(CFrameWnd)
	ON_EVENT(CMainFrame, -1, 1, OnFireRec, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

BOOL CALLBACK MyInfoEnumProc(HMONITOR hMonitor,  HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFO lpmi ;
	memset(&lpmi,0,sizeof(MONITORINFO));
	lpmi.cbSize = sizeof(MONITORINFO);
	if(!GetMonitorInfo(hMonitor,  &lpmi ))
		return FALSE;

	if (g_monRc.left  > lpmi.rcWork.left)	
		g_monRc.left   = lpmi.rcWork.left;
	if (g_monRc.top   > lpmi.rcWork.top)	
		g_monRc.top    = lpmi.rcWork.top;
	if (g_monRc.right < lpmi.rcWork.right)	
		g_monRc.right  = lpmi.rcWork.right;
	if (g_monRc.bottom< lpmi.rcWork.bottom)	
		g_monRc.bottom = lpmi.rcWork.bottom;

	return TRUE;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		MSG*	msg = (MSG *) lParam;
		//if (/*m_pMain->m_bar1 && m_pMain->m_bar1->IsInputFocus() && */!m_pMain->m_bSDI)
		if (m_pMain->m_bar1 && m_pMain->m_bar1->IsInputFocus() && !m_pMain->m_bSDI)   //m_bar1 읜 유저툴바인데 여기 에디트에서 키보드누른것은 그냥 통과
			return CallNextHookEx(m_pMain->m_hHook, nCode, wParam, lParam);
		else
		{   //아닌경우
			if ((wParam >= VK_BACK && wParam <= VK_HELP) || (wParam >= VK_F1 && wParam <= VK_F12)
						|| (wParam >= 'A' && wParam <= 'Z') || (wParam >= 'a' && wParam <= 'z')
						|| (wParam >= '0' && wParam <= '9') || (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
						|| (wParam == VK_OEM_7) || (wParam == VK_OEM_1))
			{
				if (nCode == HC_ACTION && !(lParam & 0x80000000))
				{
					if (m_pMain->DoHotKey(wParam, lParam)) 
						return 1L;
					
					long type = m_pMain->DoPopupKey(wParam, lParam);

					if (type == 1)
 						return 1L;

					else if(type == 2)
						return CallNextHookEx(m_pMain->m_hHook, nCode, wParam, lParam);

					if (m_pMain->m_bSDI)
					{
						type = m_pMain->DoSDIKey(wParam, lParam);
						if (type == 1)
 							return 1L;
						else if(type == 2)
							return CallNextHookEx(m_pMain->m_hHook, nCode, wParam, lParam);
					}

					if (m_pMain->DoChildKey(wParam, lParam))
						return 1L;
				}
			}
		}
	}

	return CallNextHookEx(m_pMain->m_hHook, nCode, wParam, lParam);
}
// protect input for check readding 
LRESULT CALLBACK viewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CView* view = (CView*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
	if (view == NULL)	return 0;
	switch (msg)
	{
		// for check reader
	case WM_KEYUP:
	case WM_KEYDOWN:
	case WM_CHAR:
		if (m_pMain->m_view)	
			return TRUE;
		break;
	}

	return ::CallWindowProc((WNDPROC)m_pMain->m_callproc, hwnd, msg, wParam, lParam);
}

bool axiscall(int msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMain == (CMainFrame *) 0) return false;

	CString		tmpS;
	switch (msg)
	{
	case AXI_CHANGEVIEW:
		return m_pMain->m_pFormMgr->ChangeChild((char *)lParam, TRIGGERN, (int)wParam) ? true : false;
	case AXI_CHANGEFOCUS:	m_pMain->m_pFormMgr->Setfocus_Child((int)wParam, (int)lParam ? false : true);	break;
	case AXI_ACTIVEMAP:	break;
	case AXI_SELCLIST:		return m_pMain->select_SName((char *)lParam);
	case AXI_SHOWHELP:	
		m_pMain->m_helpKey = (int) wParam;
		m_pMain->PostMessage(WM_AXIS, MAKEWPARAM(axSHOWHELP, 0), 0);
		break;
	case AXI_APPSWITCH:	m_pMain->AppSwitch();			break;
//	case AXI_CHANGEBAR2:
		if (!wParam)	m_pMain->change_Skin();
		else		m_pMain->m_pFormMgr->change_VirtualScreen((int) lParam);
		break;
	case AXI_SETTRIGGER:	m_pMain->m_pFormMgr->set_Trigger((int) wParam, (int) lParam);	break;
	case AXI_RESTORE:	m_pMain->restore((int) wParam);		break;
	case AXI_HOTKEY:	return m_pMain->HotKey(wParam, lParam);
	//case AXI_CONSEE:	m_pMain->process_ConSee((int) lParam);	break;
	case AXI_SMCALL:	m_pMain->SmCall((int) lParam);		break;
	case AXI_FUNCKEY:	m_pMain->funcKey(LOWORD(wParam), (int) lParam);		break;
	case AXI_BAR2ACTION:	m_pMain->Action(wParam, lParam);	break;
	case AXI_SETALL:	m_pMain->m_pFormMgr->childAll((char *) lParam);	break;
	case AXI_GETMAPNUM:	m_pMain->GetDispN((char *) lParam);	break;
	case AXI_APPENDALLMAP:	m_pMain->AppendAllMap();		break;
	case AXI_SENDTICKINFO:	break;
	case AXI_INPUTSCREENNO:	return m_pMain->InputScreenNo((char *)lParam) ? true : false;
	case AXI_EDITUSERTOOL:	
		{
			m_pMain->EditTool();
		//m_pMain->EditTool();			
		}
		break;
	case AXI_TICKSETUP:	m_pMain->SetTicker((int)wParam);	break;
	case AXI_GETCAPTIONINFO:m_pMain->GetCaptionInfo(wParam, lParam);break;
	case AXI_HIDECAPTIONBUTTON:
		m_pMain->HideChildButton((int) lParam);		break;
	case AXI_LINKOPEN:	m_pMain->LinkOPEN((int) wParam, (char *) lParam);	break;
	case AXI_SETBORDER:	m_pMain->SetBZ();			break;
	case AXI_HIDETICKER:
		if (lParam == 1)	m_pMain->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_TICKER1, 0), 0);
		else if (lParam == 2)	m_pMain->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_TICKER2, 0), 0);
		else if (lParam == 4)	m_pMain->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_DB1, 0), 0);
		else if (lParam == 5)	m_pMain->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_SCLIST, 0), 0);
		break;
	case AXI_CLOSESCREEN:
		if (wParam)	m_pMain->m_pFormMgr->closeChild((int) wParam);
		else		m_pMain->m_pFormMgr->closeAll(false);
		break;
	default:	break;
	}
	return true;
}

void CMainFrame::outputtrace(CString strdata)
{
	if ( m_wndOutput.GetSafeHwnd() )
	{
		WPARAM wParam  = 0;
		strdata += _T("\n");
		OnTrace(wParam, (LPARAM)(LPSTR)(LPCTSTR)strdata);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	int ii = 0;
	m_pMain		= (CMainFrame *) 0;

	m_mapN		= _T("");
	m_update	= false;
	m_fullmode	= false;
	m_bExit = true;
	m_bSrnSave	= true;
	m_runAxis	= false;
	m_dev		= false;
	m_noActive	= true;
	m_block		= false;
	m_forceClose	= false;
	m_bSDI		= false;
	m_saveALLVS	= true;
	m_blinkbar	= true;
	m_bCustomer	= true;
	m_mngSound	= false;
	m_mngPopup	= false;
	m_activeCap	= FALSE;
	m_bpcenable     = false;

	m_xtrKEY	= 0;
	m_maxChild	= 16;
	m_fontSize	= 9;
	m_idletime	= 0;
	m_posChild	= 0;
	m_toolStatus	= 0;
	m_SDIHEIGHT	= 0;
	m_activeKey	= 0;
	m_linkbar	= 0;
	m_downI		= -1;
	m_appMode	= MODE_MDI;

	m_conKIND	= CON_NONE;
	m_conHISTORY	= 0;
	m_conAUTOTIME	= 0;

//	m_waveF		= _T("");
	
	m_connectBy	= byNONE;
	m_step		= axNONE;

	m_vsN		= V_SCREEN1;
	m_axis		= AfxGetApp();

#ifdef _DEBUG
	m_home		= m_axis->GetProfileString(ENVIRONMENT, ROOTDIR);//_T("C:\\src\\금융투자협회");
#else
	m_home		= m_axis->GetProfileString(ENVIRONMENT, ROOTDIR);
#endif
	char temp_path[MAX_PATH];
	WORD majorVer=0, minorVer =0, buildNum=0, revisionNum=0;

	// 현재 실행된 프로그램의 경로를 얻는다.
	GetModuleFileName(AfxGetInstanceHandle(), temp_path, sizeof(temp_path));

	int position	= m_axis->GetProfileInt(ENVIRONMENT, CHILDPOS, 0);

	m_regkey	= ((CAxisApp*)m_axis)->m_regkey;
	m_axMisc	= new CAxMisc(m_home, m_regkey);
	m_axGuide	= new CAxGuide(LOWORD(position), m_home);
	m_MClient	= new CMDIClient(axiscall, m_home);
	m_axisres	= new _axisRes(m_home);
	m_pFormMgr      = new CFormManager();
	m_wizard	= NULL;
	m_tMenu		= NULL;
//	m_bar0		= NULL;
	m_bar1		= NULL;
	m_bar2		= NULL;
//	m_tInfo1	= NULL;
//	m_tInfo2	= NULL;
	m_smain		= NULL;
	m_sdibar	= NULL;
	m_axConnectD	= NULL;
	m_modal		= NULL;
	m_WBWnd		= NULL;
	m_hHook		= NULL;
	m_view		= NULL;
	m_mngInfo	= NULL;
	m_smcall	= NULL;
	m_wb		= NULL;
	m_chaser	= NULL;
	m_category	= NULL;
	m_memo		= NULL;
	m_tip		= NULL;
	m_stoploss	= NULL;

	m_GuideDLG	= NULL;
	m_ConCDLG	= NULL;

	m_lockDlg = NULL;

	m_pAccountTotal	= NULL;
	m_nAccountLength= 0;

	m_printsetup = new CPrinterSettings();
	m_printsetup->CopyDefaultMfcPrinter();

	m_waitlist.RemoveAll();

	for (ii = V_SCREEN1; ii <= V_SCREEN4; ii++)	m_hooklist[ii].RemoveAll();
	for (ii = V_SCREEN1; ii <= V_SCREEN4; ii++)	m_sdiZORDER[ii].RemoveAll();

	m_progK = ((CAxisApp *) m_axis)->m_progK;
	if (((CAxisApp *) m_axis)->m_mode == MD_DEV)
		m_dev = true;
	if (((CAxisApp *) m_axis)->m_exeName.IsEmpty())
		m_dev = true;

	m_axisres->m_hIcon = (HICON) AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_axisres->m_hTray = (HICON) AfxGetApp()->LoadIcon(IDI_CLIENT);

	for (ii = 0; ii < MBMP_CNT; ii++)
		m_arRc.Add(CRect(0, 0, 0, 0));

	CString file;
	file.Format("%s\\image\\caption_act.bmp", m_home);
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (m_bmCaption[0].GetSafeHandle())
		m_bmCaption[0].DeleteObject();
	if (hBitmap)	m_bmCaption[0].Attach(hBitmap);

	//file.Format("%s\\image\\caption_inact.bmp", m_home);
	file.Format("%s\\image\\caption_act.bmp", m_home);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (m_bmCaption[1].GetSafeHandle())
		m_bmCaption[1].DeleteObject();
	if (hBitmap)	m_bmCaption[1].Attach(hBitmap);

	m_enableAkey = m_enableBkey = m_enableCkey = m_enableDkey = m_enableEkey = m_enableFkey = m_enableGkey = m_enableHkey = m_enableA3Mkey = m_enableNkey = false;
	m_xvalueAkey = m_xvalueBkey = m_xvalueCkey = m_xvalueDkey = m_xvalueEkey = m_xvalueFkey = m_xvalueGkey = m_xvalueHkey = m_xvalueA3Mkey = m_xvalueNkey = 0;

	m_enableHogakey = false;
	m_xvalueHogakey = 0;

	m_nConTickCount = 0;

	// 2011.08.29 scbang : 추가
	m_iIdle = 0;
	m_iGridDrawing = 0;
	m_iIdleDefault = 0;
	m_iGridDrawingDefault = 0;
	// 2011.08.29 scbang : 추가끝

	m_iVS = MIDX_V1;	
	m_pInterWnd = NULL;
	m_interkey = 0;
	m_pBondWnd = NULL;
	m_pUnBondWnd = NULL;
	m_paxiscode = NULL;
	m_mngPos = 3;
	m_mapGongji.RemoveAll();
	m_bSetMenu = false;

	m_errCode = -1;
	m_messangerTry = 0;
	m_bdupc = false;
	m_btmpuse = false;
	m_buserst = false;
	m_buuidrec = false;
	m_iMenuCnt = 0;
	m_ip.Empty();
	m_bMngShow = false;
	m_bChekcTr = false;

	m_mngPopup = true;
	m_mngGongji = true;
	m_mngSuyo = true;
	m_mngBal = true;

	m_pSocket = NULL;
	m_bServerError = false;
	m_plogindata = NULL;
	m_arrIP.RemoveAll();
	m_bMainStart = false;
}

CMainFrame::~CMainFrame()
{
	int ii = 0;
	ExitWebBrowser();
	DeleteObject(m_rgn);
	if (m_hHook)	UnhookWindowsHookEx(m_hHook);

	SAFE_DELETE(m_MClient);	
	SAFE_DELETE(m_tMenu);
	SAFE_DELETE(m_bar1);
	SAFE_DELETE(m_bar2);
	SAFE_DELETE(m_smain);
	SAFE_DELETE(m_sdibar);
	SAFE_DELETE(m_axConnectD);
	SAFE_DELETE(m_wizard);
	SAFE_DELETE(m_axMisc);
	SAFE_DELETE(m_axGuide);
	SAFE_DELETE(m_printsetup);
	SAFE_DELETE(m_mngInfo);
	SAFE_DELETE(m_smcall);
	SAFE_DELETE(m_chaser);
	SAFE_DELETE(m_tip);
	SAFE_DELETE(m_axisres);
	SAFE_DELETE(m_pFormMgr);

	m_waitlist.RemoveAll();
	for (ii = V_SCREEN1; ii <= V_SCREEN4; ii++)
	{
		m_hooklist[ii].RemoveAll();
		m_sdiZORDER[ii].RemoveAll();
	}
	
	if (m_bmCaption[0].GetSafeHandle())
		m_bmCaption[0].DeleteObject();
	if (m_bmCaption[1].GetSafeHandle())
		m_bmCaption[1].DeleteObject();

	if(m_plogindata != NULL)
		delete m_plogindata;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
#ifndef _DEBUG
	m_bServerError = CheckUrl();
	if(!m_bServerError)  //장애가 아니면 무작위 아이피 선택
		SelectServerIP();
#endif

	m_pFormMgr->Init(this, axiscall);

	SetIcon(m_axisres->m_hIcon, TRUE);
	GetWindowPlacement(&m_switchpl);

	m_tip = new CTTip(this);
	if (!m_tip->Create(NULL, "CTTip", WS_CHILD|WS_BORDER, CRect(0, 0, 0, 0), GetDesktopWindow(), (int) m_tip))
	{
		delete m_tip;
		m_tip = NULL;
	}
	else	m_tip->ModifyStyleEx(0, WS_EX_TOOLWINDOW);
//	ModifyStyle(0, WS_SYSMENU);  //khs test
	AfxInitRichEdit2();

	m_GuideDLG = new CGuideMsgDLG(this, m_home);
	m_GuideDLG->Create(IDD_GUIDE_MSG, this);
	m_GuideDLG->ShowWindow(SW_HIDE);


	m_ConCDLG = new CConclusionDLG(this, m_home);;
	m_ConCDLG->Create(IDD_CONCLUSION_LIST, this);
	m_ConCDLG->ShowWindow(SW_HIDE);
	
	
	//HINSTANCE ins =  ShellExecute(NULL, _T("open"), _T("http:\\bispfm.kofia.or.kr:49876"), NULL,NULL, SW_SHOW);

	return Initialize();
}

CString CMainFrame::ReplaceExpectSymbol(CString sym)
{
	CString	ret = _T("");
	
	m_mapExpectSymbol.Lookup(sym, ret);
	return ret;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//FWS_ADDTOTITLE  SDI 프로젝트 생성시 기본적으로 추가됨. 프레임워크에서 창의 캡션에 문서제목 추가하도록 함
	cs.style &= ~(WS_SYSMENU|FWS_ADDTOTITLE);   
	//cs.style &= WS_EX_APPWINDOW ;
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

#include "ScreenSetDlg.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case 'f':
			case 'F':
			{	
				//this->addCJCode("KKKAAABBBCCC국꼬01250-1912(16-7)                                                                                1000      2016-12-102019-12-10   2.5     1.250이표채           1.677     1.67720170306170505");
				/*
				CString tmp;
				if ((::GetKeyState(VK_SHIFT) & 0x8000) && (::GetKeyState(VK_CONTROL) & 0x8000))
				{		
					int vsN, value, key;
					for (vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
					{
						for (value = 0; value < m_hooklist[vsN].GetSize(); value++)
						{
							tmp.Format(_T("@@@ mdi [%d]%d \n"), vsN, m_hooklist[vsN].GetAt(value));
							TRACE(tmp);
						}

						for (value = 0; value < m_sdiZORDER[vsN].GetSize(); value++)
						{
							tmp.Format(_T("@@@ sdi [%d]%d \n"), vsN, m_sdiZORDER[vsN].GetAt(value));
							TRACE(tmp);
						}
					}
				}
				*/
			}
			break;
			case 'z':
			case 'Z':
			{	
				if ((::GetKeyState(VK_SHIFT) & 0x8000) && (::GetKeyState(VK_CONTROL) & 0x8000))
				{
			
				}
			}
			break;
			case 'x':
			case 'X':
				{	
					if ((::GetKeyState(VK_SHIFT) & 0x8000) && (::GetKeyState(VK_CONTROL) & 0x8000))
					{
						
					}
				}
				break;  
			case 'c':
			case 'C':
				{	
					if ((::GetKeyState(VK_SHIFT) & 0x8000) && (::GetKeyState(VK_CONTROL) & 0x8000))
					{
					}
			}
			break;
		}
	}

	if (m_tMenu && m_tMenu->PreTranslateMsg(pMsg))
		return TRUE;
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::CheckTR()
{
	CString strmap;
	char* psub = NULL;
	strmap = "testTR";
	struct usemapmid  *pmid = new struct usemapmid;
	memset(pmid, 0, sizeof(struct usemapmid) + 1);
	memcpy(pmid->userid, m_user, m_user.GetLength());
	memcpy(pmid->mapnum, (LPSTR)(LPCTSTR)strmap, strmap.GetLength());
	psub = (char*)pmid;

	if(sendTR("PIBOPFRM", psub, sizeof(struct usemapmid), USEMAP))
		SetTimer(TM_CHECKTR, 3000, NULL);
}

void CMainFrame::SendUseMap(CString strmap)
{
	char* psub = NULL;
	strmap = strmap.Mid(2, 6);
	struct usemapmid  *pmid = new struct usemapmid;
	memset(pmid, 0, sizeof(struct usemapmid) + 1);
	memcpy(pmid->userid, m_user, m_user.GetLength());
	memcpy(pmid->mapnum, (LPSTR)(LPCTSTR)strmap, strmap.GetLength());
	psub = (char*)pmid;

	if(!sendTR("PIBOPFRM", psub, sizeof(struct usemapmid), USEMAP))
		OutputDebugString("send usemap tr fail");
}

bool CMainFrame::sendTR(CString trC, char* pBytes, int nBytes, int trGubn)
{
	int	len = nBytes + sizeof(struct  _ledgerH);
	char*	sndB = new char[len];
	ZeroMemory(sndB, sizeof(sndB));
	
	struct  _ledgerH ledgerH, *pledgerH;	
	pledgerH = new _ledgerH;
	
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');
	GetledgerH(&ledgerH, trGubn);
	CopyMemory(&sndB[0], &ledgerH, sizeof(struct  _ledgerH));
	CopyMemory(&sndB[sizeof(struct  _ledgerH)], pBytes, nBytes);

	bool iret = m_wizard->sendTR(trC, sndB, len, 0, '0');
	delete [] sndB;
	return iret;
}

void CMainFrame::GetledgerH(struct _ledgerH* pLedgerH, int trGubn)
{
	CString strtemp;
	strtemp = "FITS-HTS";
	memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  
	/*
	#define LOGIN       0   //로그인
#define GONGJITR	1   //공지팝업
#define MENUTR		2   //사용자별 메뉴
#define LOGOUT		3   //로그아웃
#define USEMAP		4   //사용한 화면
	*/
	switch(trGubn)
	{
		case LOGIN:
			{
				strtemp = "FITSComLoginSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = " selectLoginInfo";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
		case MENUTR:   //사용자별 메뉴
			{
				strtemp = "FITSComLoginSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = " selectMenuList";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
		case GONGJITR:  //공지팝업
			{
				strtemp = "FITSPopUpNotifySO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = "selectPopUpNotifyAp";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
		case USEMAP:   //사용한 맵
			{
				strtemp = "FITSComLoginSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = "menuLog";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
		case LOGOUT:  //로그아웃
			{
				strtemp = "FITSComLoginSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = "logout";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
	}
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CWindowDC dc(this);
	CRect	rc;
	int	nItem;
	CPoint	point = CPoint(LOWORD(lParam), HIWORD(lParam));
	
	switch (message)
	{
	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_MAXIMIZE:
			if(m_bSDI)
				return TRUE;
			break;
 		case SC_MINIMIZE:
 			break;
		case 0xF032:
			if(m_bSDI)
				return TRUE;
			break;
		case SC_RESTORE:
			if(m_bSDI)
			{
				CWnd* pWnd = GetActiveWindow();

				int		key;
				CSChild*	schild = NULL;

				CArray	<int, int>	aryK;

				for (int value = (int)m_sdiZORDER[m_vsN].GetUpperBound(); value >= 0; value--)
				{
					key = m_sdiZORDER[m_vsN].GetAt(value);
					aryK.Add(key);
				}
	
				for (int ii = 0; ii < aryK.GetSize(); ii++)
				{		
					key = aryK.GetAt(ii);
					if (!m_arSDI[m_vsN].Lookup(key, schild))	continue;
					 
				}
				
				CGPop* popup = NULL;

				for (POSITION pos = m_arGPOP.GetStartPosition(); pos; )
				{
					m_arGPOP.GetNextAssoc(pos, key, popup);
					popup->ShowWindow(SW_SHOW);
				}

			}
			break;
		}
		break;
	case WM_SYNCPAINT:
		break;
	case WM_NCLBUTTONDOWN:
		switch (wParam)
		{
		case HTCAPTION:
			GetCursorPos(&point);
			nItem = GetSelectItem(point);
			
			m_downI = -1;
			if (nItem != -1)
			{
				rc = m_arRc.GetAt(nItem);
				rc.OffsetRect(XGAP, YGAP);
				switch (nItem)
				{
				case MIDX_MIN:	case MIDX_MAX:	case MIDX_CLOSE:
				case MIDX_RESTORE:
				case MIDX_DUAL:
				case MIDX_V1:
				case MIDX_V2:
				case MIDX_V3:
					m_downI = nItem;
					m_action = nItem;
					DrawBitmapByMask(&dc, nItem, rc, true, m_activeCap);
					break;
				default:
					break;
				}
				return TRUE;
			}
			break;
		case HTCLOSE:
		case HTMAXBUTTON:
		case HTMINBUTTON:
			GetCursorPos(&point);
			nItem = GetSelectItem(point);
			m_downI = -1;
			if (nItem != -1)
			{
				rc = m_arRc.GetAt(nItem);
				rc.OffsetRect(XGAP, YGAP);
				switch (nItem)
				{
				case MIDX_MIN:	case MIDX_MAX:	case MIDX_CLOSE:
				case MIDX_RESTORE:
				case MIDX_DUAL:
				case MIDX_V1:
				case MIDX_V2:
				case MIDX_V3:
					m_downI = nItem;
					m_action = nItem;
					DrawBitmapByMask(&dc, nItem, rc, true);
					break;
				default:
					break;
				}
				return TRUE;
			}
			return TRUE;
		}
		break;
	case WM_NCMOUSEMOVE:
		switch (wParam)
		{
		case HTCLOSE:
		case HTMAXBUTTON:
		case HTMINBUTTON:
			return TRUE;
		}
		break;
	case WM_NCLBUTTONDBLCLK:
		if (wParam == HTCAPTION)
		{
			GetCursorPos(&point);
			nItem = GetSelectItem(point);
			switch (nItem)
			{
			case -1:
				break;
			case MIDX_ICON: case MIDX_CLOSE:
				PostMessage(WM_SYSCOMMAND, SC_CLOSE);
			default:
				return TRUE;
			}
		}
		break;
	}
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint		pt;
	int		nItem;
	CRect		rc;
	CWindowDC	dc(this);
	
	drawTitle(&dc);

	if (m_downI != -1)	
	{
		switch (message)
		{
		case WM_LBUTTONUP:	// WM_NCLBUTTONUP process
			GetCursorPos(&pt);
			nItem = GetSelectItem(pt);
			if (nItem == m_downI)
			{ 
				m_action = nItem;
				PostMessage(WM_AXIS, MAKEWPARAM(axCAPTION, (int) m_action), -1);
			}

			rc = m_arRc.GetAt(m_downI);
			rc.OffsetRect(XGAP, YGAP);
			DrawBitmapByMask(&dc, m_downI, rc, false, m_activeCap);

			m_action = -1;
			m_downI = -1;
			break;
		case WM_NCMOUSEMOVE:
			break;
		case WM_MOUSEMOVE:
			GetCursorPos(&pt);
			nItem = GetSelectItem(pt);
		
			if (nItem == m_downI)
			{
				rc = m_arRc.GetAt(nItem);
				rc.OffsetRect(XGAP, YGAP);
				switch (nItem)
				{
				case MIDX_MIN:	case MIDX_MAX:	case MIDX_CLOSE:
				case MIDX_RESTORE:
					DrawBitmapByMask(&dc, nItem, rc, false, m_activeCap);
					break;
				default:
					break;
				}
			}
			else
			{
				rc = m_arRc.GetAt(m_downI);
				rc.OffsetRect(XGAP, YGAP);
				DrawBitmapByMask(&dc, m_downI, rc);
			}
			
			ShowToolTip(nItem, pt);
			break;
		}
	}

	switch (message)
	{
	case WM_MOUSEMOVE:
		GetCursorPos(&pt);
		nItem = GetSelectItem(pt);
		ShowToolTip(nItem, pt);
		break;
	}
	return CMDIFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainFrame::ShowToolTip(int nIndex, CPoint pt)
{
	if (nIndex < 0 || !m_tip)	
		return;
	if (!(GetStyle() & WS_CAPTION))	
		return;
	
	CString		tipS;
	switch(nIndex)
	{
		case MIDX_MAX:
			tipS =  _T("전체화면");
			break;
		case MIDX_MIN:
			tipS =  _T("최소화");
			break;
		case MIDX_RESTORE:
			tipS =  _T("이전크기로");
			break;
		case MIDX_CLOSE:
			tipS =  _T("닫기");
			break;
		case MIDX_DUAL:
			tipS =  _T("듀얼모니터");
			break;
		default:
			if (  nIndex >= MIDX_V1 && nIndex <= MIDX_V3)
			{
				int index = nIndex - MIDX_V1;
				tipS.Format("가상화면 %d", index + 1);
			}
			else
			{
				m_tip->HideTips();
				return;
			}
			break;
	}

	CRect	wRc;
	CRect	tipRc = m_arRc.GetAt(nIndex);
	GetWindowRect(wRc);
	tipRc.OffsetRect(wRc.left, wRc.top);
	m_tip->ShowTips(tipRc,  tipS);
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
static long m_dwStyle = 0;

BOOL CMainFrame::OnNcActivate(BOOL bActive) 
{
#ifndef USERFRAME
	CMDIFrameWnd::OnNcActivate(bActive);
#endif

	m_activeCap = bActive;
	DrawFrame();


	return TRUE;
}

void CMainFrame::OnNcPaint() 
{
#ifndef USERFRAME
	CMDIFrameWnd::OnNcPaint();
#endif

	DrawFrame();
}

void CMainFrame::DrawFrame() 
{
	CWindowDC dc(this);
	SetRegion();

#ifdef USERFRAME
	CRect		winRc, rc;
	COLORREF	color, colorline;
	CArray	<COLORREF, COLORREF > arColor;
	GetWindowRect(winRc);

	winRc -= winRc.TopLeft();
	rc = winRc;
	
	if (m_axisres)
	{
		if (m_activeCap)
		{
			color = m_axisres->GetColor(ACTFLINE_5);
			arColor.Add(color);

			color = m_axisres->GetColor(ACTFLINE_4);	
			arColor.Add(color);

			if (!m_bSDI)
			{
				color = m_axisres->GetColor(ACTFLINE_2);
				arColor.Add(color);
			}

			color = m_axisres->GetColor(ACTFLINE_1);	
			arColor.Add(color);
		}
		else
		{
			color = m_axisres->GetColor(INACTFLINE_5);	
			arColor.Add(color);

			color = m_axisres->GetColor(INACTFLINE_4);	
			arColor.Add(color);

			if (!m_bSDI)
			{
				color = m_axisres->GetColor(INACTFLINE_2);
				arColor.Add(color);
			}

			color = m_axisres->GetColor(INACTFLINE_1);	
			arColor.Add(color);	// vista
		}
	}

	while (arColor.GetSize() < XGAP)	// vista
	{
		arColor.Add(color);
	}

	for (int ii = 0; ii < arColor.GetSize(); ii++)
	{
		color = RGB(255, 255, 255);//arColor.GetAt(ii);
		colorline = RGB(76, 81, 90);
		if (ii == (arColor.GetSize() - 1))
		{
		//	dc.Draw3dRect(rc, RGB(141, 156, 247), RGB(141, 156, 247)); //전체화면의 가장 바깥쪽 테두보다 1픽셀씩 줄어든 크기
			dc.Draw3dRect(rc,colorline, colorline); //전체화면의 가장 바깥쪽 테두보다 1픽셀씩 줄어든 크기
		}
		else if (ii == (arColor.GetSize() - 2))
		{
			//dc.Draw3dRect(rc, RGB(46, 54, 139), RGB(46, 54, 139));  //전체화면의 가장 바깥쪽 테두리
			dc.Draw3dRect(rc, colorline, colorline);  //전체화면의 가장 바깥쪽 테두리
		}
		else
			dc.Draw3dRect(rc, colorline, colorline);
		//	dc.Draw3dRect(rc, color, color);
				
		rc.DeflateRect(1, 1);
	}
	CRect	capRc = GetCapRect();
	rc.bottom = capRc.bottom + 1;
	//dc.Draw3dRect(rc, RGB(159, 161, 167), RGB(159, 161, 167));  
	dc.Draw3dRect(rc, colorline, colorline);  
	//메인프레임타이틀 가장 하단 약간 보이게 된다 안에서 drawtitle 해서 대부분 가려짐

	arColor.RemoveAll();
#endif
	drawTitle(&dc);

}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	index = LOWORD(wParam);
	if (index < ID_MENU_BASE || index > ID_MENU_END)
	{
		if (runCommand(LOWORD(wParam), lParam))
			return TRUE;
		return CMDIFrameWnd::OnCommand(wParam, lParam);
	}

	ChangeChild(index);

	return TRUE;
}

void CMainFrame::OnAppDummy(UINT ua) {}

bool CMainFrame::runCommand(int comm, LPARAM lParam)
{
	BOOL	visible = FALSE;
	CString	str;

	switch (comm)
	{
	case ID_ESCAPEWINODW:
			m_pFormMgr->closeChild();
		break;
	case ID_SCREENNUM:	
	//	SetHome();	
		break;	// set home
	case ID_SHOWTRACER:	showChaser();	break;	// alt + F1
	case ID_FULLNAME:	
		showfname();	break;	// alt + F2
	case ID_RELOADMAP:
		if (m_dev)	refreshMap(m_activeKey);	break;	// alt + F3
	case ID_FIRSTTAB:	// end key
#ifdef _DEBUG
		AppSwitch();
#endif
		//firstTabOrder();
		break;
	case ID_APP_TOTALSETUP: TotalSetup();	break;
	case ID_APP_PINCH:	SetPinchData(); break;
	case ID_APP_ESETUP:	ESetup();	break;
	case ID_APP_EDITTOOL:	EditTool();	break;
	case ID_APP_EDITHOTKEY:	EditHotkey();	break;
	case ID_APP_ORDERSETUP:
		{
			OrderSetup();
		}
		break;
	case ID_APP_TICKERSETUP:SetTicker();	break;
	case ID_APP_MNGSETUP:	SetMng();	break;
	case ID_APP_IMAGESETUP:	ImageSetup();	break;
	case ID_APP_SAVECLIP_A:  saveClip(1); break;
	case ID_APP_SAVECLIP:   saveClip(); break;
	case ID_APP_SAVEBMP_A:		saveImg(1);		break;
	case ID_APP_SAVEBMP:		saveImg();		break;
	case ID_APP_PRINT_SETUP:	m_printsetup->PrinterSetup(this);	break;
	case ID_APP_PRINT:
		if (m_progK == PK_BUSINESS && m_connectBy == byTCP)
		{
			SYSTEMTIME	lTime;
			CString		time;
			GetLocalTime(&lTime);
			time.Format(" %02d:%02d:%02d", lTime.wHour, lTime.wMinute, lTime.wSecond);
			GetWindowText(str);
			str += time;
		}

		m_printS = str;
		PostMessage(WM_CANCELMODE);
		PostMessage(WM_AXIS, MAKEWPARAM(axPRINTIMG, 0));
		break;
	case ID_APP_LOCK:	LockProg();	break;   //화면 잠금
//	case ID_VIEW_KONGI:	
//		m_MClient->ShowBB();		break;
	case ID_VIEW_DB1:
		visible = !(m_bar1->GetStyle() & WS_VISIBLE);
		ShowControlBar(m_bar1, visible, FALSE);
		saveToolStatus();

		break;
	case ID_VIEW_SCLIST:
		visible = !(m_bar2->GetStyle() & WS_VISIBLE);
		ShowControlBar(m_bar2, visible, FALSE);
// 		if (visible && (m_bar0->GetStyle() & WS_VISIBLE))
// 			ShowControlBar(m_bar0, TRUE, FALSE);
		saveToolStatus();
		SetSDIChangeHeight();
		break;
	case ID_VIEW_TR_MSG:
		if (m_GuideDLG->IsWindowVisible())
			m_GuideDLG->ShowWindow(SW_HIDE);
		else
		{
			m_GuideDLG->ShowWindow(SW_SHOW);
			m_GuideDLG->SetActiveWindow();
		}
		break;
	case ID_VIEW_TRAY:
		if (m_appMode != MODE_TRAY)
			ChangeTRAY();
		else
		{
			m_appMode = MODE_SDI;
			TrayCmd(TRAY_HIDEICON);
			ShowWindow(SW_SHOW);
		}
		break;
	case ID_VIEW_DUAL:
		fitDual();
		break;
	case ID_VIEW_VSCREEN1:	m_pFormMgr->change_VirtualScreen(V_SCREEN1);	break;
	case ID_VIEW_VSCREEN2:	m_pFormMgr->change_VirtualScreen(V_SCREEN2);	break;
	case ID_VIEW_VSCREEN3:	m_pFormMgr->change_VirtualScreen(V_SCREEN3);	break;
	case ID_VIEW_VSCREEN4:	m_pFormMgr->change_VirtualScreen(V_SCREEN4);	break;
	case ID_VIEW_FULLSCREEN:fullScreenMode(!m_fullmode);		break;
	case ID_VIEW_CASCADE:	
		MDICascade();				break;
	case ID_VIEW_TILE_HORZ:	
		MDITile(MDITILE_HORIZONTAL);		break;
	case ID_VIEW_TILE_VERT:	
		MDITile(MDITILE_VERTICAL);		break;
	case ID_APP_CLOSEA:	m_pFormMgr->closeChild();	break;
	case ID_APP_CLOSEALL:	m_pFormMgr->closeAll();	break;
	case ID_APP_CHATHIDE:
		{
			HWND hwn = ::FindWindow("TfrmMain", NULL);
			if(hwn == NULL)
			{
				SendMsgToMessanger(RUNANDLOGINSEND);
				Sleep(2000);
				SendMsgToMessanger(SHOWMSGSEND);
			}
			else
			{
				if(!CheckMessangerLogin())
					SendMsgToMessanger(LOGINSEND);
				Sleep(2000);
				SendMsgToMessanger(SHOWMSGSEND);
			}
	}
	default:
		return false;
	}

	return true;
}

void CMainFrame::OnUpdateRunCommand(CCmdUI* pCmdUI) 
{
	BOOL	enable = TRUE;
	switch (pCmdUI->m_nID)
	{
	case ID_APP_SAVECLIP_A:
	case ID_APP_SAVECLIP:
	case ID_APP_SAVEBMP_A:
	case ID_APP_SAVEBMP:
	case ID_APP_PRINT:
		if (MDIGetActive())
			pCmdUI->Enable();
		else	
			pCmdUI->Enable(FALSE);
		return;
	case ID_VIEW_DB1:
		if (m_bSDI)	
			enable = false;

		//test
		if (m_bar1->GetStyle() & WS_VISIBLE)
			pCmdUI->SetCheck(1);
		else	
			pCmdUI->SetCheck(0);
		break;
	case ID_VIEW_KONGI:
		{
			CWnd* view = m_MClient->get_Ctrl(1);
			if (view)
			{
				if (view->IsWindowVisible())
					pCmdUI->SetCheck(1);
				else
					pCmdUI->SetCheck(0);
			}
		}
		break;
	case ID_VIEW_SCLIST:
		if (m_bSDI)	enable = false;
		if (m_bar2->GetStyle() & WS_VISIBLE)
			pCmdUI->SetCheck(1);
		else	pCmdUI->SetCheck(0);
		break;
	case ID_APP_CHATHIDE:
		if (m_bSDI)
			enable = false;
		if(m_wndMyBar.IsWindowVisible())
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;
	case ID_VIEW_TR_MSG:
		if (m_GuideDLG->IsWindowVisible())
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;
// 	case ID_VIEW_MDI:
// 		if (m_appMode == MODE_MDI)
// 			pCmdUI->SetCheck(1);
// 		else	pCmdUI->SetCheck(0);
// 		break;
// 	case ID_VIEW_SDI:
// 		if (m_appMode == MODE_SDI)
// 			pCmdUI->SetCheck(1);
// 		else	pCmdUI->SetCheck(0);
// 		break;
	case ID_VIEW_TRAY:
		if (m_appMode == MODE_TRAY)
			pCmdUI->SetCheck(1);
		else	pCmdUI->SetCheck(0);
		break;
	case ID_VIEW_VSCREEN1:
		if (m_vsN == V_SCREEN1)
			pCmdUI->SetCheck(1);
		else	pCmdUI->SetCheck(0);
		break;
	case ID_VIEW_VSCREEN2:
		if (m_vsN == V_SCREEN2)
			pCmdUI->SetCheck(1);
		else	pCmdUI->SetCheck(0);
		break;
	case ID_VIEW_VSCREEN3:
		if (m_vsN == V_SCREEN3)
			pCmdUI->SetCheck(1);
		else	pCmdUI->SetCheck(0);
		break;
	case ID_VIEW_VSCREEN4:
		if (m_vsN == V_SCREEN4)
			pCmdUI->SetCheck(1);
		else	pCmdUI->SetCheck(0);
		break;
// 	case ID_APP_CLOSEA:
// 	case ID_APP_CLOSEALL:
// 		if (m_bSDI)
// 		{
// 			if (!m_arSDI[m_vsN].GetCount())
// 				enable = false;
// 		}
// 		else
// 		{
// 			if (!m_arMDI[m_vsN].GetCount())
// 				enable = false;
// 		}
// 		break;
	case ID_VIEW_DUAL:
	case ID_VIEW_FULLSCREEN:
		if (m_bSDI)	enable = false;
		if (m_fullmode)
			pCmdUI->SetCheck(1);
		else	pCmdUI->SetCheck(0);
		break;
	case ID_VIEW_CASCADE:
	case ID_VIEW_TILE_HORZ:
	case ID_VIEW_TILE_VERT:
		if (!MDIGetActive())	enable = false;
		break;
	default:	break;
	}
	pCmdUI->Enable(enable);
}

void CMainFrame::OnClose() 
{	
	if (!closeX())	return;

	if (m_bExit && m_runAxis)	//	exit window
	{
		if (((CAxisApp *)m_axis)->m_progK != 'R')
		{
//			SendMessage(WM_SYSCOMMAND, SC_RESTORE);
			CExitD exit(m_home);
			exit.SetTime(m_connectT);
			if (exit.DoModal() == IDCANCEL)
				return;
		}
	}

	SendMsgToMessanger(EXITMGSSEND);  //메신저종료
	if (m_bSrnSave)
	{
		saveExitMap();
		save_history();
	}

	int		key;
	CSChild*	schild;

	int vsN = 0;
	for (vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++);
	{
		for (POSITION pos = m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			schild->SendMessage(WM_CLOSE);
		}
		m_arSDI[vsN].RemoveAll();
	}

	m_pFormMgr->closeAll();
	for (int ii = 0; ii < m_arHide.GetSize(); ii++)
	{
		CChildFrame* child = m_arHide.GetAt(ii);
		if (child && child->GetSafeHwnd())
			child->SendMessage(WM_CLOSE);
	}

	if (m_connectBy == byUDD || m_connectBy == byUDP)
		cleanUDP();

	if (m_wizard)
		m_wizard->LogOff();


	if (m_MClient)	m_MClient->UnsubclassWindow();

	if (m_bSrnSave && m_runAxis)
	{
		CRect	wRc;
		GetWindowRect(wRc);

		int screenX = GetSystemMetrics(SM_CXMAXIMIZED);
		int screenY = GetSystemMetrics(SM_CYMAXIMIZED);

		int	x = wRc.left;
		int	y = wRc.top;
		int	cx = wRc.Width();
		int	cy = wRc.Height();

		m_axis->WriteProfileInt(INFORMATION, "SDI", m_bSDI ? 1 : 0);

		if (!IsIconic())
		{
			if (m_bSDI)
			{
				m_axis->WriteProfileInt(INFORMATION, "win_sx", x);
				m_axis->WriteProfileInt(INFORMATION, "win_sy", y);
				m_axis->WriteProfileInt(INFORMATION, "win_scx", cx);
				m_axis->WriteProfileInt(INFORMATION, "win_scy", cy);

				x = m_switchpl.rcNormalPosition.left;
				y = m_switchpl.rcNormalPosition.top;
				cx = m_switchpl.rcNormalPosition.right - m_switchpl.rcNormalPosition.left;
				cy = m_switchpl.rcNormalPosition.bottom - m_switchpl.rcNormalPosition.top;
				m_axis->WriteProfileInt(INFORMATION, "win_x", x);
				m_axis->WriteProfileInt(INFORMATION, "win_y", y);
				m_axis->WriteProfileInt(INFORMATION, "win_cx", cx);
				m_axis->WriteProfileInt(INFORMATION, "win_cy", cy);
			}
			else
			{
				m_axis->WriteProfileInt(INFORMATION, "win_x", x);
				m_axis->WriteProfileInt(INFORMATION, "win_y", y);
				m_axis->WriteProfileInt(INFORMATION, "win_cx", cx);
				m_axis->WriteProfileInt(INFORMATION, "win_cy", cy);

				x = m_sdipl.rcNormalPosition.left;
				y = m_sdipl.rcNormalPosition.top;
				cx = m_sdipl.rcNormalPosition.right - m_sdipl.rcNormalPosition.left;
				cy = m_sdipl.rcNormalPosition.bottom - m_sdipl.rcNormalPosition.top;
				m_axis->WriteProfileInt(INFORMATION, "win_sx", x);
				m_axis->WriteProfileInt(INFORMATION, "win_sy", y);
				m_axis->WriteProfileInt(INFORMATION, "win_scx", cx);
				m_axis->WriteProfileInt(INFORMATION, "win_scy", cy);
			}
		}

		m_axis->WriteProfileInt(INFORMATION, "toolstatus", m_toolStatus);

	}
	TrayCmd(TRAY_HIDEICON);
	DeleteChaser();
	CMDIFrameWnd::OnClose();
}
/*
#define axBONDON 90
#define axBONDOFF 91
#define axINTERSETON 92
#define axINTERSETOFF 93
#define axADDINTERSET 94
*/
LRESULT CMainFrame::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	char		wb[128];
	int		value, key;

	CChildFrame*	child;
	CStringArray	arr;
	CString		tmps = _T(""), title = _T(""), log =_T("");
	LONG_PTR	lvalue = 0;

	/*
	switch(LOWORD(wParam))
	{
		case axSTART:
			OutputDebugString(_T("		@@OnAXIS axSTART\n"));
				break;
		case axRUN:
			OutputDebugString(_T("		@@OnAXIS axRUN\n"));
				break;
		case axAXIS:
			OutputDebugString(_T("		@@OnAXIS axAXIS\n"));
				FEV_SIZE;
		case axSIGNON:
			OutputDebugString(_T("		@@OnAXIS axSIGNON\n"));
				break;
		case axCLOSEV:
			OutputDebugString(_T("		@@OnAXIS axCLOSEV\n"));
				break;
		case axRELOAD:
			OutputDebugString(_T("		@@OnAXIS axRELOAD\n"));
				break;
		case axFOCUS:
			OutputDebugString(_T("		@@OnAXIS axFOCUS\n"));
				break;	
		case axGUIDE:
			OutputDebugString(_T("		@@OnAXIS axGUIDE\n"));
				break;
		case axTRIGGER:
			OutputDebugString(_T("		@@OnAXIS axTRIGGER\n"));
				break;
		case axSCREEN:
			OutputDebugString(_T("		@@OnAXIS axSCREEN\n"));
				break;
		case axMODAL:
			OutputDebugString(_T("		@@OnAXIS axMODAL\n"));
				break;
		case axDEVICE:
			OutputDebugString(_T("		@@OnAXIS axDEVICE\n"));
				break;	
		case axXTR:
			OutputDebugString(_T("		@@OnAXIS axXTR\n"));
				break;
		case axURL:
			OutputDebugString(_T("		@@OnAXIS axURL\n"));
				break;
		case axCLIP:
			OutputDebugString(_T("		@@OnAXIS axCLIP\n"));
				break;
		case axWRITEIMG:
			OutputDebugString(_T("		@@OnAXIS axWRITEIMG\n"));
				break;
		case axPRINTIMG:
			OutputDebugString(_T("		@@OnAXIS axPRINTIMG\n"));
				break;
		case axSHOWHELP:
			OutputDebugString(_T("		@@OnAXIS axSHOWHELP\n"));
				break;
		case axALARM:
			OutputDebugString(_T("		@@OnAXIS axALARM\n"));
				break;
		case axPWDCHK:
			OutputDebugString(_T("		@@OnAXIS axPWDCHK\n"));
				break;
		case axDIALOG:
			OutputDebugString(_T("		@@OnAXIS axDIALOG\n"));
				break;
		case axKONG:
			OutputDebugString(_T("		@@OnAXIS axKONG\n"));
				break;
		case axCAPTION:
			OutputDebugString(_T("		@@OnAXIS axCAPTION\n"));
				break;
		case axLINKEDIT:
			OutputDebugString(_T("		@@OnAXIS axLINKEDIT\n"));
				break;
		case axChaserON:
			OutputDebugString(_T("		@@OnAXIS axChaserON\n"));
				break;
		case axChaserOFF:
			OutputDebugString(_T("		@@OnAXIS axChaserOFF\n"));
				break;
		case apSETTG:
			OutputDebugString(_T("		@@OnAXIS apSETTG\n"));
				break;
		case apPINCH:
			OutputDebugString(_T("		@@OnAXIS apPINCH\n"));
				break;
		case apGUIDE1:
			OutputDebugString(_T("		@@OnAXIS apGUIDE1\n"));
				break;
		case apGUIDE2:
			OutputDebugString(_T("		@@OnAXIS apGUIDE2\n"));
				break;
		case axLOADUSER:
			OutputDebugString(_T("		@@OnAXIS axLOADUSER\n"));
				break;
	}
	*/
	switch (LOWORD(wParam))
	{
	case 9892:
		{
		//	SetLogindata((struct _signR *)lParam);
		}
		break;
	case axLOGINMSG:  //fx_edit, cx_griddata 에서 메신저 로그인할때
			SendMsgToMessanger(LOGINSEND);
		break;
	case axSTARTMSG:   //fx_edit에서 메신저프로그램 실행할때
			SendMsgToMessanger(RUNANDLOGINSEND);
		break;
	case axSLIDEEND:
			m_bMngShow = false;
		break;
	case axFITSMNG:   //알람팝업을 클릭시 화면 띄우기 위해
		{
			CString tempS ,tmpS, map , sym, data;
			tempS.Format(_T("%s"), (char*)lParam);
			tmpS = Parser(tempS, _T("\t"));
			data.Format(_T("%s%s%s\t%s\n"), "FS" ,tmpS,"pushParam",tempS);
			m_pFormMgr->CreateChild(data, 1, 0, 0, CPoint(100,100));
		}
		break;
	case axCLONE:
		{
			CString str;
		}
		break;
	case axRELOADINTER:
		{
			if(m_pUnBondWnd->GetSafeHwnd() && m_pUnBondWnd != NULL)   //FS900100 화면 관심그룹 리로드
			{
				m_pUnBondWnd->SendMessage(WM_USER + 999, axRELOADINTER, 0);
			}
		}
		break;
	case axTMenu:
		tmps.Format(_T("%s"), (char*)lParam);
		m_bar1->set_Maps(tmps);
		break;
	case axINTERSDIMOVE:
		{
			bool bmdi = true;
			CWnd* pinter = GetMapWmd("FS205001", bmdi);
			CWnd* pbond = GetMapWmd("FS900100", bmdi);

			if(pinter != NULL && pbond != NULL)
			{
				CString tmpS;
				CRect maprc, tmprc, bondrc;

				if(bmdi)
				{
					pinter->GetWindowRect(&maprc);	
					pbond->GetClientRect(&bondrc);
					ScreenToClient(bondrc);

					ScreenToClient(maprc);

					tmprc.left = maprc.right;
					tmprc.top = maprc.top;
					tmprc.right = tmprc.left + bondrc.Width();
					tmprc.bottom = tmprc.top + bondrc.Height() ;
					tmprc.OffsetRect(-16, -95); 
				}
				else
				{
					pinter->GetWindowRect(&maprc);	
					pbond->GetClientRect(&bondrc);
					ScreenToClient(bondrc);

					tmprc.left = maprc.right;
					tmprc.top = maprc.top;
					tmprc.right = tmprc.left + bondrc.Width();
					tmprc.bottom = tmprc.top + bondrc.Height() ;
					tmprc.OffsetRect(-13, 0); 
				}	
				pbond->SetWindowPos(&wndTop, tmprc.left, tmprc.top, tmprc.Width(), tmprc.Height(), SWP_NOSIZE);		
			}
		}
		break;
	case axINTERMOVE:   //관심설정화면 이동
		{
			bool bmdi = true;
			CWnd* pinter = GetMapWmd("FS205001", bmdi);
			CWnd* pbond = GetMapWmd("FS900100", bmdi);

			if(pinter != NULL && pbond != NULL)
			{
				CString tmpS;
				CRect maprc, tmprc, bondrc;

				if(bmdi)
				{
					pinter->GetWindowRect(&maprc);	
					pbond->GetClientRect(&bondrc);
					ScreenToClient(bondrc);

					ScreenToClient(maprc);

					tmprc.left = maprc.right;
					tmprc.top = maprc.top;
					tmprc.right = tmprc.left + bondrc.Width();
					tmprc.bottom = tmprc.top + bondrc.Height() ;
					tmprc.OffsetRect(-16, -95); 
				}
				else
				{
					pinter->GetWindowRect(&maprc);	
					pbond->GetClientRect(&bondrc);
					ScreenToClient(bondrc);

					tmprc.left = maprc.right;
					tmprc.top = maprc.top;
					tmprc.right = tmprc.left + bondrc.Width();
					tmprc.bottom = tmprc.top + bondrc.Height() ;
					tmprc.OffsetRect(-13, 0); 
				}

				pbond->SetWindowPos(&wndTop, tmprc.left, tmprc.top, tmprc.Width(), tmprc.Height(), SWP_NOSIZE);		
			}
					
			/*
			if(m_pBondWnd != NULL && m_Bondkey > 0 && m_pInterWnd != NULL)
			{
				if(m_pBondWnd->GetSafeHwnd())
				{
					//m_pBondWnd FS900100    m_pInterWnd FS2005001
					if(m_pBondWnd->IsWindowVisible() && m_pInterWnd->GetSafeHwnd())
					{
						//SetTimer(TM_INTERMOVE, 30, NULL);	
						if(m_pBondWnd == NULL)
							return false;

						CString tmpS;
						CRect maprc, tmprc, bondrc;
						m_pInterWnd->GetWindowRect(&maprc);
					
						m_pBondWnd->GetClientRect(bondrc);
						//m_pBondWnd->GetWindowRect(&bondrc);
						ScreenToClient(maprc);

//log.Format(_T("[관심]이동 m_pInterWnd(FX900100)=%x (%d %d %d %d)\n"), m_pBondWnd, maprc.left, maprc.top, maprc.right, maprc.bottom);
//OutputDebugString(log);
						tmprc.left = maprc.right;
						tmprc.top = maprc.top;
						tmprc.right = tmprc.left + bondrc.Width();
						tmprc.bottom = tmprc.top + bondrc.Height() ;
						tmprc.OffsetRect(-6, -133); //test
						//ScreenToClient(tmprc);
						m_pBondWnd->SetWindowPos(&wndTop, tmprc.left, tmprc.top, tmprc.Width(), tmprc.Height(), SWP_NOSIZE);												
					}
				}
			}	
			*/
		}
		break;
	case axADDINTERSET:  //종목다이알로그에서 관심설정으로 종목 보내기 //8787
		{
			if(m_pInterWnd != NULL)
			{
				m_pInterWnd->SendMessage(WM_USER + 999, axADDINTERSET, lParam);	
			}	
		}
		break;
	case axDELINTERSET:
		{
			if(m_pInterWnd != NULL)
				m_pInterWnd->SendMessage(WM_USER + 999, axDELINTERSET, lParam);				
		}
		break;
	case axINTERSETOFF:  //관심종목설정 닫기 
//log.Format(_T("[관심] m_pInterWnd = %x 관심종목설정 닫기 \n"), m_pInterWnd);
//OutputDebugString(log);
			m_pInterWnd = NULL;  //FS205001
			break;
	case axSETBONDWND:  //관심설정용 종목다이알로그 시작 FS900100
		{
			CString strlog;
			m_pUnBondWnd = (CWnd*)lParam;
//strlog.Format(_T("[관심][%x] 종목설정다이알로그9001 시작 본인 주소를 알려줬다"), lParam);
//OutputDebugString(strlog);
		}
		break;
	case axINTERSETPT:  //관심설정 시작
		{
			m_pInterWnd = (CWnd*)lParam;  
//log.Format(_T("[관심] m_pInterWnd = %x 관심종목설정 시작 \n"), m_pInterWnd);
//OutputDebugString(log);
			this->PostMessage(WM_AXIS, axINTERMOVE, 0);
		}
		break;
	case axSETINTERKEY:
		{
			m_interkey = lParam;
		}
		break;
	case axINTERSETON: //관심설정 시작
		{  //!m_pBondWnd->IsWindowVisible() || 
			if(m_pBondWnd == NULL  )
			{
				m_pFormMgr->ChangeChild(BONDMAP);
				return -1;
			}
			CString tmpS;
			CRect maprc, tmprc, bondrc;
			CRect* rc = (CRect*)lParam;
			
			maprc.left = rc->left;
			maprc.right = rc->right;
			maprc.top = rc->top;
			maprc.bottom = rc->bottom;

			m_pBondWnd->GetClientRect(bondrc);
			
			tmprc.left = maprc.right;
			tmprc.top = maprc.top;
			tmprc.right = tmprc.left + bondrc.Width();
			tmprc.bottom = tmprc.top + bondrc.Height() + 20;
			tmprc.OffsetRect(0, -20);
			
			m_pBondWnd->SetWindowPos(&wndTop, tmprc.left, tmprc.top, tmprc.Width(), tmprc.Height(), SWP_NOSIZE);
			m_pBondWnd->ShowWindow(SW_SHOW);
			m_pBondWnd->SendMessage(WM_USER + 999, axBONDON, (LPARAM)this);
			return 1;
		}
		break;
	case axSTART:	// start step
		if (HIWORD(wParam) && m_step != axNONE)
		{
			m_step = axNONE;
			m_wizard->LogOff();
		}

		switch (m_step)
		{
		case axNONE:	
			m_connectBy = m_axis->GetProfileInt(INFORMATION, CONNECTBY, byNONE);
			break;
		case axOPENPPP:	break;
		case axOPENSIGN:
			m_axConnectD->SetChoice(false);
			PostMessage(WM_AXIS, MAKEWPARAM(axSIGNON, 0));		return 0;
		default:return 0;
		}
		
	//	m_wizard->set_optSOCK(m_axis->GetProfileInt(INFORMATION, "STEP", 0));  //test

		if (m_connectBy == byUDD || m_connectBy == byUDP)
			startUDP();
	
		if (!getConnectInfo(tmps, value))
		{ 
			::MessageBox(m_hWnd, "설치정보를 확인하세요", "오류", MB_ICONINFORMATION);
			return 0;
		}

		strcpy_s(wb, tmps);
		m_step = axOPENRSM;

		m_ip.Format("%s", wb); 
		m_port.Format("%d", value);

		if (m_wizard->LoginAXIS(m_ip, value))
		{
			m_axMisc->GetGuide(AE_CONNECT, tmps);
			m_axConnectD->SetGuide(tmps);
		}
		else
		{
			m_step = axNONE;
			m_axConnectD->SetProgress(false);
			m_axConnectD->SetStatus(SM_EXIT);
		}
		break;

	case axRUN:	
		break;
	case axAXIS:
		{
		//	if(m_step == axNONE  || m_step == axOPENRSM)
		//		return -1;
			CString tmp;
			tmp = m_axis->GetProfileString(INFORMATION, "SVRNM");
			m_SVRNM = tmp.Right(4);

			SetHTSTitle();
			int screenX = GetSystemMetrics(SM_CXMAXIMIZED);
			int screenY = GetSystemMetrics(SM_CYMAXIMIZED);
			int x, y, cx, cy;

			if (m_bSDI)
			{
				x  = m_axis->GetProfileInt(INFORMATION, "win_sx", 0);
				y  = m_axis->GetProfileInt(INFORMATION, "win_sy", 0);
				cx = m_axis->GetProfileInt(INFORMATION, "win_scx", screenX);
				cy = m_axis->GetProfileInt(INFORMATION, "win_scy", screenY);
				
				SetWindowPos(&wndTop, x, y, cx, m_SDIHEIGHT, SWP_SHOWWINDOW);

				x  = m_axis->GetProfileInt(INFORMATION, "win_x", 0);
				y  = m_axis->GetProfileInt(INFORMATION, "win_y", 0);
				cx = m_axis->GetProfileInt(INFORMATION, "win_cx", screenX);
				cy = m_axis->GetProfileInt(INFORMATION, "win_cy", screenY);

				m_switchpl.rcNormalPosition.left = x;
				m_switchpl.rcNormalPosition.top = y;
				m_switchpl.rcNormalPosition.right = x + cx;
				m_switchpl.rcNormalPosition.bottom = y + cy;
			}
			else
			{
				x  = m_axis->GetProfileInt(INFORMATION, "win_x", 0);
				y  = m_axis->GetProfileInt(INFORMATION, "win_y", 0);
				cx = m_axis->GetProfileInt(INFORMATION, "win_cx", screenX);
				cy = m_axis->GetProfileInt(INFORMATION, "win_cy", screenY);

				if (cx <= 300 || cy <= 100)
				{
					SetWindowPlacement(&m_switchpl);
					m_axis->m_nCmdShow |= SW_SHOWMAXIMIZED;
					ShowWindow(m_axis->m_nCmdShow);
				}
				else	
					SetWindowPos(&wndTop, x, y, cx, cy, SWP_SHOWWINDOW);

				x  = m_axis->GetProfileInt(INFORMATION, "win_sx", 0);
				y  = m_axis->GetProfileInt(INFORMATION, "win_sy", 0);
				cx = m_axis->GetProfileInt(INFORMATION, "win_scx", screenX);
				cy = m_axis->GetProfileInt(INFORMATION, "win_scy", screenY);

				m_sdipl.rcNormalPosition.left = x;
				m_sdipl.rcNormalPosition.top = y;
				m_sdipl.rcNormalPosition.right = x + cx;
				m_sdipl.rcNormalPosition.bottom = y + cy;
			}

			UpdateWindow();
			//SendMsgToMessanger(EXITMGSSEND);
			CString	file, tmpS;
			file.Format("%s\\%s\\%s\\%s", m_home, "user", GetUserName(), "usersetup.ini");
			WritePrivateProfileString("messanger", "connect", _T(""), file);
			SendMsgToMessanger(RUNANDLOGINSEND, m_user + _T("\t") + m_pass);
			SendMsgToMessanger(HANDLESEND);

			endWorkstation();
#if 0
			SetHome();
#else
			SetForegroundWindow();
#endif

			if (m_axConnectD)
			{
				delete m_axConnectD;
				m_axConnectD = NULL;
			}

		//	m_bar1->drawingColor(RGB(228, 235, 254));  

			if(m_bSDI)
			{
				m_bar1->SetSDI();  //test
				m_bar2->SetSDI();
			}
			else
			{
				m_bar1->SetMDI();  //test
				m_bar2->SetMDI();
			}

			SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

			load_eninfomation();
			m_vsN = -1;
			m_pFormMgr->change_VirtualScreen(0);

			CreateBonddlg();	 
			sendTR("PIBOPFRM", NULL, 0, GONGJITR);
			SetFileVisible(m_home + _T("\\tab\\uuid.ini"), false);
			m_bMainStart = true;
		//	SendMsgToMessanger(0);
		}
		break;
	case axSIGNON:	
		signOn();		
		break;
	case axCLOSEV:	  //화면을 닫을때
		m_pFormMgr->OnCloseForm(HIWORD(wParam));
		if(m_interkey == HIWORD(wParam))
		{
			m_interkey = -1;
			m_pFormMgr->closeChild(m_Bondkey);
		}
		else if(m_Bondkey == HIWORD(wParam))
		{
			m_Bondkey = -1;
			m_pBondWnd = NULL;
		}
		break;
	case axRELOAD:	refreshMap(m_activeKey);	
		break;
	case axFOCUS:
		m_activeKey = HIWORD(wParam);
		axFocus(m_activeKey);
		break;
	case axGUIDE:
		if (lParam)	displayGuide((char *)lParam);
		else
		{
			m_axMisc->GetGuide(HIWORD(wParam), tmps);
			displayGuide(tmps);
		}
		break;
	case axTRIGGER:
		m_wizard->setGroupTrigger((WORD)lParam, HIWORD(wParam));
		break;
	case axSCREEN:		
		save_user((char*)lParam);
		break;
		//return modifyWorkspace(HIWORD(wParam) ? false : true, (char *)lParam, m_saveALLVS);
	case axMODAL:		
		loadMap(HIWORD(wParam), (int)lParam);		
		break;
	case axDEVICE:
		if (m_modal)	key = m_modal->m_vwKEY;
		else
		{
			bool	match = false;
			CGPop*	pop;		
			for (POSITION pos = m_arGPOP.GetStartPosition(); pos;)
			{
				m_arGPOP.GetNextAssoc(pos, key, pop);
				if (!pop->m_active)	continue;
				match = true;
			}

			if (!match)
			{
				child = (CChildFrame *) MDIGetActive();
				if (!child)	return 0;
				key = child->m_key;
			}
		}
		if (m_wizard)
			m_wizard->sendDeviceEvent(key);
		break;
	case axXTR:
		key = HIWORD(wParam);
		value = (int) lParam;
		switch (key)
		{
		case REGGROUP:
// 		case REGCODE:
// 			if (value)	m_bar0->reload_Code();
// 			break;
		case USERLOAD:
			load_eninfomation(false);
			makeUserScreenMenu();
			m_bar1->Refresh();
			m_smain->Refresh();
			break;
		default:
			break;
		}
		m_pFormMgr->closeChild(m_xtrKEY);
		break;
	case axURL:
		{
			CString	file, key;
			char	buf[1024];

			if (HIWORD(wParam))
				key = "ECN";
			else	
				key = "EMERGENCY";
			file.Format("%s\\%s\\link.ini", m_home, TABDIR);
			GetPrivateProfileString("LINKINFOMATION", key, "", buf, sizeof(buf), file);
			CString	url = buf;
			url.TrimRight();
			if (url.IsEmpty())	return 0;
			PopupWeb(url);
		}
		break;
	case axCLIP:
		writeImg(m_home, HIWORD(wParam), TRUE);
		break;
	case axWRITEIMG:	
		writeImg(m_home, HIWORD(wParam));	break;
	case axPRINTIMG:	printImg(m_printS);			break;
	case axSHOWHELP:	HelpLink(m_helpKey);			break;
	case axALARM:
		if (m_alarmMsg.Left(1) == "{" || m_alarmMsg.Left(1) == "}" )
			m_GuideDLG->InsertMsg(m_alarmMsg, HIWORD(wParam));
		else if (m_runAxis && m_alarmMsg.Left(1) != "{" && m_alarmMsg.Left(1) != "}")
		{
			CAlarm	dlg(m_home);
			dlg.SetMsg(m_alarmMsg);
			dlg.DoModal();
		}
		break;
	case axPWDCHK:
		{
			int	pos = 10;
			CString	account, sEnc;
			tmps = (char *) lParam;
			account = tmps.Left(pos);
			tmps = tmps.Mid(pos);
			sprintf_s(wb, "%s\t%s", tmps, account);

			sEnc = m_wizard->encryptPASS(CString(wb));
			
			sprintf_s(wb, "%s%s", account, sEnc.GetString());
			m_wizard->sendTR("chboachk", wb, (int)strlen(wb), US_PASS, 'p');
		}
		break;
	case axDIALOG:
		{
			CString	file;

			file.Format("%s\\%s\\%s\\axisensetup.ini", m_home, USRDIR, GetUserName());
			tmps = m_alarmMsg;
			switch (HIWORD(wParam))
			{
			default:	break;
			case 0x96:	break;
			case 0x97:	
				displayMsgBox("이중접속 알림", tmps);	
				m_bExit = false;
				m_forceClose = true;
				PostMessage(WM_CLOSE);
				break;
			}
		}
		break;
	case axKONG:	break;
	case axCAPTION:
		key = (int) lParam;
		actionCaption(key, HIWORD(wParam));
		break;
	case axLINKEDIT:
		{
			CLink	dlg(m_tMenu, m_home, (char *)lParam);
			if (dlg.DoModal() == IDOK)
				changeAllLinkInfo((char *)lParam);
		}
		break;
	case axChaserON:	CreateChaser();	break;
	case axChaserOFF:	DeleteChaser();	break;
	case apSETTG:
		ALLTriggerSend((char*) lParam);		
		break;
	case apPINCH:
		{
			m_sSS = m_strVirtualID[m_vsN] + tabCHAR + m_strVirtualNAME[m_vsN];
			return (long)m_sSS.operator LPCTSTR();
		}
		break;

	case apGUIDE1:
		displayGuide((char *)lParam);
		break;

	case apGUIDE2:
		//level(1, 2, 3)
		m_GuideDLG->InsertMsg((char *)lParam, HIWORD(wParam));
		//m_GuideDLG->ShowWindow(SW_SHOW);
		break;
	case axLOADUSER:
		tmps = (char *) lParam;
		LoadUserScreen(tmps);
		break;
	}

	return 0;
}

LRESULT CMainFrame::OnUSER(WPARAM wParam, LPARAM lParam)
{
	CString strlog;
	strlog.Format(_T("CMainFrame::OnUSER   wParam=[%d]  lParam=[%d] \n"), wParam, lParam);
	/*
	if (LOWORD(wParam) == CLOSEMAP)
	{
		CString		mapN;
		int		ii = 0;
		int		key = -1;
		CChildFrame*	child = NULL;
		
		mapN = CString((char*)lParam);
		TRACE("CLOAEMAP_%d\t%s\n", LOWORD(wParam), mapN);
		switch (HIWORD(wParam))
		{
		case 0:	// Close all
			for (ii = V_SCREEN1; ii <= V_SCREEN6; ii++)
			{
				for (POSITION pos = m_arMDI[ii].GetStartPosition(); pos; )
				{
					m_arMDI[ii].GetNextAssoc(pos, key, child);
					child->PostMessage(WM_CLOSE);
				}
			}
			break;
		case 1:	// close exception map
			for (ii = V_SCREEN1; ii <= V_SCREEN6; ii++)
			{
				for (POSITION pos = m_arMDI[ii].GetStartPosition(); pos; )
				{
					m_arMDI[ii].GetNextAssoc(pos, key, child);
					
					if (child->m_mapN.Left(L_MAPN).CompareNoCase(mapN))
						child->PostMessage(WM_CLOSE);
				}
			}
			break;
		case 2:	// close map
			for (ii = V_SCREEN1; ii <= V_SCREEN6; ii++)
			{
				for (POSITION pos = m_arMDI[ii].GetStartPosition(); pos; )
				{
					m_arMDI[ii].GetNextAssoc(pos, key, child);
					
					if (child->m_mapN.Left(L_MAPN).CompareNoCase(mapN))
						continue;
					child->PostMessage(WM_CLOSE);
					break;
				}
			}
			break;
		}		
		
	}
	*/
	switch (wParam)
	{
		case MMSG_APPLYSETUP:
			return ApplySetup(lParam);
	}
	/*
	switch (wParam)
	{
	case MMSG_GETPASSWORD:
		return (LRESULT)(LPCTSTR)GetUserPassword();
	case MMSG_GETCERTPASSWORD:
		return (LRESULT)(LPCTSTR)GetCertPassword();
	case MMSG_APPLYSETUP:
		return ApplySetup(lParam);
	case MMSG_LOADXTR:
		LoadXTR((int) lParam);
		break;
	case CTIMSG:		// Call Center - Axis 연동 처리(CTI)
		{
			if (m_connectBy != byTCP)	return 0;
			
			HANDLE hMapping;
			LPVOID pViewMap;

			hMapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, "AXISMAP");
			pViewMap = MapViewOfFile(hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
			
			CString dat, mapN = (LPSTR)pViewMap;
			
			if (mapN.IsEmpty())	return 0;
			SetActiveMain();
			
			m_mapHelper->ChangeChild(mapN);
			
			CloseHandle(hMapping);
		}
	case TCPIPSERVER:
		OpenConnection (TCPIPSERVER, "0.0.0.0");
		break;
	case TCPIPCLIENT:
		{
			int	pos = -1;
			CString ip, tmp, data;
			
			tmp = CString((char*)lParam);
			
			pos = tmp.Find('\t');
			
			if (pos > 0)
			{
				ip = tmp.Left(pos);
				data = tmp.Mid(pos +1);
				
				SetTcpIpMapSymbol(data);
				
				OpenConnection (TCPIPCLIENT, ip);
			}
		}
		break;
	case TCPIPSERVERSEND:
		{
			// 화면번호\tsymbol
			SetTcpIpMapSymbol(CString((char*)lParam));
			
			if (g_comm)
				g_comm->Send((LPSTR)(LPCSTR)m_TCPIPMapName, m_TCPIPMapName.GetLength());
		}
		break;
	case TCPIPSERVERERROR:
		break;
	case TCPIPRECIVE:
	case TCPIPSTANDBY:
	case TCPIPSRECONNECT:
		TcpIpTriggerMapMsg(wParam);
		break;
	case TCPIPCLIENTCLOSE:
		CloseConnection();
		TcpIpTriggerMapMsg(TCPIPSNOTCONNECT);
		break;
	case EXCUTE_OPENMAP:
		{
			CString mapN;

			mapN = CString((char*)lParam);

			if (mapN.GetLength() != L_MAPN)
				return 0;
			
			m_mapHelper->CreateChild(mapN, 1, 0, 0, 0, true);
		}
		break;
	case REDRAWTITLE:
		m_HiNuriInfo = HiNuriInfo();
		InvalidateRect(m_rctitle);
		break;
	}
	*/

	return 0;
}

int CMainFrame::OnFireRec(INT_PTR type, WPARAM wParam, LPARAM lParam)
{
	BOOL	rc;
	int igubn = -1;
	CString	str, msg, title, logs;
	
	if (m_forceClose)	
		return 0;

	switch (type)
	{
	case FEV_SIGNR:
		{
			m_plogindata = new char[L_signR + 1];
			memset(m_plogindata, 0x00, L_signR);
			memcpy(m_plogindata, (char*)lParam, L_signR);
			SetLogindata(m_plogindata);
		}
		break;

	case FEV_OPEN:
		if (!lParam)
		{
			m_axConnectD->SetGuide(_T("서버와 연결되었습니다."));
			str = m_axis->GetProfileString(INFORMATION, "Port");
			break;
		}
	case FEV_CLOSE:
		m_bExit = false;
		m_forceClose = true;
		rc = m_wizard->LogOff();

		if(m_step == axOPENRSM)
		{
			CString temp, tmps;
			int icnt = 0;
			m_forceClose = false;
			CWinApp* app = AfxGetApp();

			if(m_arrIP.GetSize() == 0)
				PostMessage(WM_AXISCLOSE);
	
			for(int ii = 0 ; ii < m_arrIP.GetSize() ; ii++)
			{
				temp = m_arrIP.GetAt(ii);
				if(temp.Find(m_ip) >= 0)
				{
					m_arrIP.RemoveAt(ii);
					break;
				}		
			}

			if(m_arrIP.GetSize() == 0)
			{
				char buf[1024];
				CString file, errmsg;
				file.Format("%s\\%s\\FISMSG.INI", m_home, TABDIR);
				GetPrivateProfileString("msg", "06", "", buf, sizeof(buf), file);
				errmsg.Format(_T("%s"), buf);
				errmsg.TrimRight();

				if(errmsg.IsEmpty())
					errmsg = "HTS접속 불가.. 메신저 로그인 하십시요.";

				m_axConnectD->SetGuide(str);
				//PostMessage(WM_AXISCLOSE);
			}

			temp = m_arrIP.GetAt(0);
			icnt = temp.Find("=");
			tmps = temp.Mid(icnt + 1 , temp.GetLength() - icnt);
			app->WriteProfileString(INFORMATION, "Server", tmps);

			PostMessage(WM_AXIS, MAKEWPARAM(axSTART, 1));
		}
		else
			PostMessage(WM_AXISCLOSE);
		break;

	case FEV_RUN:
		if (lParam)
		{
			if (m_update)	
				break;

			m_update = true;
			m_axMisc->GetGuide(AE_UPDATE, str);
			m_axConnectD->SetProgress(false);
			m_axConnectD->SetGuide(str);
			rc = m_wizard->LogOff();

			Sleep(300);
			if (!m_axMisc->RunVers(verUPDATE, m_axConnectD->GetUserID(), m_axConnectD->GetPassword(), m_axConnectD->GetCPass()))
			{
				m_axMisc->GetGuide(AE_EUPDATE, str);
				m_axConnectD->SetGuide(str);
				Sleep(700);
			}
			m_axConnectD->EndDialog(IDCANCEL);
			return 0;
		}
		else
		{
			SetForegroundWindow();
			signOn();
		}
		break;

	case FEV_SIZE:
		m_axMisc->GetGuide(AE_COMPONENT, msg);
		m_axConnectD->SetGuide(msg);
		if (m_axConnectD)
		{
			m_axConnectD->SetProgress(true);
			m_axConnectD->SetTotal((char *)lParam);
		}
		break;

	case FEV_ANM:
		update_ticker((int)wParam, (char *)lParam);
		break;

	case FEV_AXIS:
		igubn = LOWORD(wParam);
		switch (LOWORD(wParam))
		{	
		case runAXIS:
			{
				m_arrloginPopMsg.RemoveAll();
				CString errmsg;
				if(m_ip == "0.0.0.0")
					m_errCode = 0;
	
				if(m_errCode != 0)  //로그인을 못하는 경우
				{		
					switch(m_errCode)
					{
						case -1:
							{
							//	::MessageBox(m_axConnectD->m_hWnd, "m_errMsg = -1", "K-Bond", MB_OK);
							}
							break;
						case 1:      //ID/PASS 오류
							{
								::MessageBox(m_axConnectD->m_hWnd, m_errMsg, "K-Bond", MB_OK);
							}
							break;
						case 2:      //이용제한자
							{
								::MessageBox(m_axConnectD->m_hWnd, m_errMsg, "K-Bond", MB_OK);
							}
							break;
						case 3:       //사용자 고유키 초기화 요청(미승인. 당일아님)
							{
								::MessageBox(m_axConnectD->m_hWnd, m_errMsg, "K-Bond", MB_OK);
							}			
							break;
						case 4:      //소속회사 변경 요청(미승인, 당일아님) 
							{
								::MessageBox(m_axConnectD->m_hWnd, m_errMsg, "K-Bond", MB_OK);
							}
							break;
						case 5:
							{
								//사용자 고유키 오류-분실로 PC자동 재생성-고유키 초기화 필요
								::MessageBox(m_axConnectD->m_hWnd, m_errMsg, "K-Bond", MB_OK);
								m_axConnectD->SetGuide("고유키 초기화를 위해 HTS를 재실행 해주십시요");

								CString	file, tmpS;
								file.Format("%s\\%s\\%s", m_home, "tab", "uuid.ini");
								tmpS = m_axConnectD->GetUserID();
								WritePrivateProfileString("UUID", m_axConnectD->GetUserID(), _T(""), file);					
							}
							break;
						case 6:
							{
								//사용자고유키불일치)
								::MessageBox(m_axConnectD->m_hWnd, m_errMsg, "K-Bond", MB_OK);
							}
							break;
						case 7:
							{
								if(!m_mapMsg.Lookup("07", errmsg))
									errmsg = _T("처리 지연중...");	
								
								::MessageBox(m_axConnectD->m_hWnd, errmsg, "K-Bond", MB_OK);
								//CString		file;
								//file.Format("%s\\%s\\%s", m_home, "tab", "uuid.ini");
								//WritePrivateProfileString("UUID", m_user, _T(""), file);
							}
							break;
						default:
							{
								::MessageBox(m_axConnectD->m_hWnd, m_errMsg, "K-Bond", MB_OK);
							}
					}
					m_axConnectD->SetGuide(m_errMsg.operator LPCSTR());			
					m_axConnectD->SetStatus(SM_EXIT);
					return -1;
				}
				else
				{			
					if(m_bdupc)   //동시접속
					{	 
						char buf[1024];
						CString file, msg;
						file.Format("%s\\%s\\FISMSG.INI", m_home, TABDIR);
						GetPrivateProfileString("msg", "02", "", buf, sizeof(buf), file);
						errmsg.Format(_T("%s"), buf);
						errmsg.TrimRight();
						msg = errmsg;
						msg.Replace("\\n", "\r\n");

						if(msg.IsEmpty())
							msg = _T("동시 접속된 사용자가 존재합니다 기존 접속을 종료한 후 로그인합니다");
						::MessageBox(m_axConnectD->m_hWnd, msg, "K-Bond", MB_OK);
					}
		 
					if(m_btmpuse) //임시사용여부
					{
						m_arrloginPopMsg.Add("03");
					}
					if(m_buserst) //이용제한 요청여부
					{
						m_arrloginPopMsg.Add("04");
					}

					if(m_bemailfrm)  //이메일 확인
					{
						m_arrloginPopMsg.Add("05");
					}

					if(m_bpwch)    //비밀번호 변경
					{
						m_arrloginPopMsg.Add("01");
					}
						
					errmsg.Replace("\\n", "");
					m_axConnectD->SetGuide(errmsg);

					if(m_buuidrec)   //서버에서 내려온 사용자고유키 재생성 여부
						m_buuidrec = false;	   
				}
				
				char* psub = NULL;
				struct menumid  *pmid = new struct menumid;
				memset(pmid, 0, sizeof(struct menumid));
				memcpy(pmid->userid, (char*)m_user.operator LPCSTR(), m_user.GetLength());
				psub = (char*)pmid;

				if(sendTR("PIBOPFRM", psub, sizeof(struct menumid), MENUTR))
				{
					m_iMenuCnt = 0;
					m_bSetMenu = false;	
					SetTimer(TM_MAINMENU, 300, NULL);		
				}
				else
				{
					if(m_ip = _T("0.0.0.0"))
					{
						m_bSetMenu = true;	
						SetTimer(TM_MAINMENU, 1000, NULL);		
					}
				}
			}
			break;
		case newVIEW:
			return create_Newview(HIWORD(wParam), (char *)lParam);
		case renVIEW:
			if (HIWORD(wParam) & winK_POPUP)
				return m_pFormMgr->ChangePopup((char *)lParam, TRIGGERN, HIWORD(wParam));
			else	return m_pFormMgr->ChangeChild((char *)lParam, TRIGGERN, HIWORD(wParam));
			break; 
		case delVIEW:
			if (HIWORD(wParam) & winK_POPUP)
			{
				CMPop*	Mpop;
				CGPop*	Gpop;
				if (m_arMPOP.Lookup(HIWORD(wParam), Mpop))
					Mpop->EndDialog(IDCANCEL);
				else if (m_arGPOP.Lookup(HIWORD(wParam), Gpop))
					Gpop->SendMessage(WM_CLOSE, 0, 0);

				if(m_bSDI)
					axSFocus();
			}
			else
				m_pFormMgr->closeChild(HIWORD(wParam));
			break;
		case htmlVIEW:	// key : HIWORD(wParam), html : lParam
			str = (char *) lParam;
			if (m_runAxis)	RunWebBrowser(str);
			else		ShellExecute(NULL, _T("open"), str, NULL,NULL, SW_SHOWNORMAL);
			break;
		case titleVIEW:	setTitle(HIWORD(wParam), (char *)lParam);	break;
		case autoVIEW:	// ### : HIWORD(wParam) : cnt, items : lParam
			setCodeReg(HIWORD(wParam), (char *) lParam);
			break;
		case mapNAME:			break;
		case waitPAN:
			{
				if (lParam)	beginWait(HIWORD(wParam));
				else		endWait(HIWORD(wParam));
			}
			break;
		case hangulPAN:	// key : HIWORD(wParam), hangul : lParam
		case doublePAN:	// key : HIWORD(wParam), double : lParam
			break;
		case alarmPAN:	// dialog ? : HIWORD(wParam), text : lParam
			if (HIWORD(wParam))
			{
				str = parseData((char *) lParam);
				if (!str.IsEmpty())
				{
					m_alarmMsg = str;
					PostMessage(WM_AXIS, MAKEWPARAM(axALARM, 0), 0);
				}
			}
			else	displayGuide((char *) lParam);
			break;
		case noticePAN:
			str = m_user + '|' + (char*)lParam;
//			ConclusionNotice((char *) lParam, str);
			break;
		case dialogPAN:	// type : HIWORD(wParam), data  : lParam
			str = (char *) lParam;
			switch (HIWORD(wParam))
			{
			default:
			case 0x01:
			case 0x02:
				break;
			case 0x96:	// 긴급공지
			case 0x97:	// 중복사용자..............
				{
					CString key;
					key = m_axis->GetProfileString(WORKSTATION, "test");
					if(key == "1")
						break;
					m_alarmMsg = str;
					PostMessage(WM_AXIS, MAKEWPARAM(axDIALOG, HIWORD(wParam)), lParam);
				}
				break;
			case 0x99:
				{
					m_bExit = false;
					m_forceClose = true;
					
					HWND	hWnd = m_hWnd;
					//재접속 공지
					if (::MessageBox(hWnd, str, "재접속 공지", MB_OKCANCEL) == IDOK)
						m_axMisc->RunVers(verRETRY, m_user, m_pass, m_cpass);

					PostMessage(WM_CLOSE);
				}
				break;
			case 0x98:
				{
					m_bExit = false;
					m_forceClose = true;
					Sleep(1000);
					m_wizard->LogOff();
					HWND	hWnd = m_hWnd;
					::MessageBox(hWnd, str, "고객 알림메세지", MB_OK);
					PostMessage(WM_CLOSE);
				}
				break;
			}
			break;
		case sizeVIEW:	changeSize(HIWORD(wParam), CSize(LOWORD(lParam), HIWORD(lParam)));break;
		case userINFO:		break;
		case printVIEW:
			{
				m_printS =(char *) lParam;
				PostMessage(WM_AXIS, MAKEWPARAM(axPRINTIMG, 0));
			}
			break;
		case menuAXIS:		break;
		case linkVIEW:
			{
				CLink	dlg(m_tMenu, m_home, (char *)lParam);
				if (dlg.DoModal() == IDOK)
					changeAllLinkInfo((char *)lParam);
			}
			break;
		case closeAXIS:
			//HIWORD(wParam) = true  일때 Reboot하여 Login Dialog 상태로..
			//HIWORD(wParam) = false 일때 Axis 종료
			if (HIWORD(wParam))
				m_axMisc->RunVers(verRETRY);
			m_bExit = false;
			PostMessage(WM_CLOSE);
			break;
		case nameVIEW:	SetAMap(HIWORD(wParam), (char *) lParam);	break;
		case accnINFO:
			m_wizard->applyAccount();
			break;
		case userPASS:	
			sprintf_s((char *) lParam, m_pass.GetLength(), m_pass );	break;
		case reloadACCN:
			{
//				m_bAccnRE = TRUE;
//				m_bAccnFirst = TRUE;
//				m_bAccnTrigger = TRUE;
//				m_nAccnCount = 0;
//				for (int ii = 0; ii < 4; ii++)
//				{
//					m_bAccnVSN[ii] = TRUE;
//				}
//				getAccount(0);
			}

			break;
		}
		break;

	case FEV_STAT:
		if (m_axConnectD)
			m_axConnectD->SetProgress((char *)lParam, (int)wParam);
		break;
	case FEV_ERROR:
		switch (m_step)
		{
		case axOPENRSM:
			if (m_axConnectD)
				m_axConnectD->SetProgress((char*)lParam, (int)wParam, TRUE);
			break;
		case axOPENSIGN:
			if (lParam && m_axConnectD)
			{
				m_axConnectD->SetGuide((char *)lParam);
				if (wParam)
				{
					m_axConnectD->SetChoice(true);
					m_axConnectD->ClearPassword();
				}
				else
				{
					m_step = axNONE; // ?
					m_axConnectD->SetStatus(SM_EXIT);
				}
			}
			break;
		case axOPENWSH:
			if (lParam)
			{
				m_step = axNONE;	// ?
				if (m_axConnectD)	break;
				m_axConnectD->SetGuide((char*)lParam);
				m_axConnectD->SetStatus(SM_EXIT);
			}
			break;
		case axNONE:
		case axDONE:
			if (lParam) 
			{
				if (LOWORD(wParam))
				{
					if (HIWORD(wParam) == '4')	// messagebox
						m_axMisc->MsgBox((char *)lParam);
					else
					{
						if (m_axConnectD)
							m_axConnectD->SetGuide((char*)lParam);
						else	displayGuide((char *) lParam);
					}
				}
				else
				{
					if (m_axConnectD)
						m_axConnectD->SetGuide((char*)lParam);
					else	displayGuide((char *) lParam);
				}
			}
			break;
		}
		break;
	case FEV_GUIDE:
		m_axMisc->GetGuide((int)lParam, str);
		switch (m_step)
		{
		case axDONE:
			if (LOWORD(wParam))
			{
				if (HIWORD(wParam) == '4')	// messagebox
					m_axMisc->MsgBox(str);

				if (LOWORD(wParam) & 0x80)	displayGuide(str);
				else				displayGuide(str);
			}
			else
			{
				if ((int) lParam != 6)
					displayGuide(str);
			}
			break;
		case axOPENSIGN:
		case axOPENWSH:
			if (m_axConnectD)	m_axConnectD->SetChoice(false);
		default:
			if (m_axConnectD)
			{
				//m_axConnectD->SetChoice(true);  //test
				if (LOWORD(wParam)) m_axConnectD->SetGuide((char*)lParam);
				else m_axConnectD->SetGuide(str);
			}
			break;
		}
		break;
	case FEV_FMX:	
		processFMX(wParam, lParam);	
		break;	
	default:	break;
	}
	return 0;
}

int CMainFrame::Initialize()
{
	CloseChaserAPP();

	m_pMain = (CMainFrame *) this;
	SetWindowText(m_axMisc->m_regkey);
	m_MClient->SubclassWindow(m_hWndMDIClient);

	registerControl();
	m_axGuide->Create(this);

	m_user = m_axis->GetProfileString(WORKSTATION, SIGNONID);
	int pos = m_user.Find('|');
	if (pos >= 0)
	{
		m_user = m_user.Left(pos);
		m_axis->WriteProfileString(WORKSTATION, SIGNONID, m_user +"|");
	}

	m_axis->WriteProfileString(WORKSTATION, SITEID, "W");		// for synthesis HTS
	m_axConnectD = new CConnect(this, m_home, m_user);
	
	m_wizard = new CAxWizard();

	if (!m_wizard->CreateWizard(this))
	{
		int iret = GetLastError();
		AfxMessageBox(_T("axiswiard create fail"));
		SAFE_DELETE(m_wizard);
		PostMessage(WM_CLOSE);
		return 0;
	}
	//CheckTR();
	return 0;
}

void CMainFrame::CreateTB()
{
	SetMenu(NULL);
	m_sdibar = new CSdibar(axiscall, m_home, GetUserName());
	if (!m_sdibar->Create(this, IDD_SDIBAR, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|CBRS_ALIGN_LEFT, IDD_SDIBAR))
	{
		delete m_sdibar;
		m_sdibar = NULL;
		return;
	}
	
	m_tMenu = new CTMenu(this, m_home);
	m_tMenu->SetCall(axiscall);

	if (!m_tMenu->Create(this, IDD_TMENU, WS_DLGFRAME|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|CBRS_ALIGN_TOP, IDD_TMENU))
	{
		delete m_tMenu;
		m_tMenu = NULL;
		return;
	}

	m_smain = new CSmain(axiscall, m_home, GetUserName());
	if (!m_smain->Create(this, IDD_SMAIN, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|CBRS_ALIGN_TOP, IDD_SMAIN))
	{
		delete m_smain;
		m_smain = NULL;
		return;
	}
	
	m_bar1 = new CDbar1(axiscall, m_home);
	if (!m_bar1->Create(this, IDD_DBAR1, WS_DLGFRAME|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|CBRS_ALIGN_TOP, IDD_DBAR1))
	{
		delete m_bar1;
		m_bar1 = NULL;
		return;
	}

	m_bar2 = new CDbar2(axiscall, m_home);
	if (!m_bar2->Create(this, IDD_DBAR2, WS_CHILD|WS_DLGFRAME|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|CBRS_ALIGN_TOP, IDD_DBAR2))
	{
		delete m_bar2;
		m_bar2 = NULL;
		return;
	}

	//messanger
	if (!m_wndMyBar.Create(_T("My Bar"), this, CSize(250, 80), TRUE, IDD_CONTROLBAR))
	{
		return;      // fail to create
	}
	m_wndMyBar.ShowWindow(SW_HIDE);
	//output
	m_wndOutput.m_font = m_axisres->GetFont();
	if (!m_wndOutput.Create(_T("Outputwnd"), this, CSize(250, 80), TRUE, IDD_CONTROLBAR))
	{
		return;      // fail to create
	}
	
	m_tMenu->SetBarStyle(m_tMenu->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_tMenu->EnableDocking(CBRS_ALIGN_TOP);
	
	m_tMenu->InitMenu(IDR_MAINFRAME, ID_MENU_BASE);
	makeUserScreenMenu();

	EnableDocking(CBRS_ALIGN_ANY);

	//--test
	//messanger
	m_wndMyBar.SetBarStyle(m_wndMyBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
        m_wndMyBar.EnableDocking(CBRS_ALIGN_ANY);
        DockControlBar(&m_wndMyBar, AFX_IDW_DOCKBAR_RIGHT);

	//outputwnd
	m_wndOutput.SetBarStyle(m_wndOutput.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
    m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndOutput, AFX_IDW_DOCKBAR_BOTTOM);
	ShowControlBar(&m_wndOutput,FALSE, FALSE);

//	LOGFONT	lf;
//	lf = setFont(9, "", false, false);
//	m_font.CreatePointFontIndirect(&lf);

	m_bar1->SetBarStyle(m_bar1->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC|CBRS_BORDER_3D);
	m_bar1->EnableDocking(CBRS_ALIGN_TOP);
	ShowControlBar(m_bar1,TRUE, TRUE);
	m_bar1->make_Ctrl();
	//test

	m_bar2->SetBarStyle(m_bar2->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_bar2->EnableDocking(CBRS_ALIGN_TOP/*|CBRS_ALIGN_BOTTOM*/);
	m_bar2->make_Ctrl();
		
	m_sdibar->SetBarStyle(m_sdibar->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_smain->SetBarStyle(m_smain->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_smain->make_Ctrl();

	m_appMode = MODE_MDI;
	m_bSDI = m_axis->GetProfileInt(INFORMATION, "SDI", 0) ? true : false;
	if (m_bSDI) m_appMode = MODE_SDI;
	
	loadToolStatus();
	m_toolStatus = m_axis->GetProfileInt(INFORMATION, "toolstatus", TOOLSTATUS_SDI);
	if (m_bSDI) SetSDIHeight();

	m_toolStatus = m_axis->GetProfileInt(INFORMATION, "toolstatus", TOOLSTATUS_MDI);

	ShowControlBar(m_sdibar, FALSE, FALSE);
	ShowControlBar(m_smain, FALSE, FALSE);
	Invalidate();
}

bool CMainFrame::Start(CString user)
{
	if (m_wizard == nullptr)
		return false;

	if (!user.IsEmpty())
	{
		int	pos  = user.Find('\t');
		if (pos >= 0)
		{
			m_user = user.Left(pos++);
			user = user.Mid(pos);
			pos  = user.Find('#');
			if (pos == -1)
			{
				m_pass = user;
			}
			else
			{
				m_pass = user.Left(pos++);
				m_cpass = user.Mid(pos);
			}
		}
		m_user.TrimLeft(); m_user.TrimRight();
		m_pass.TrimLeft(); m_pass.TrimRight();
		m_cpass.TrimLeft(); m_cpass.TrimRight();
		if (!m_user.IsEmpty())	m_axConnectD->SetUserID(m_user);
		if (!m_pass.IsEmpty())	m_axConnectD->SetPassword(m_pass);
		if (!m_cpass.IsEmpty())	m_axConnectD->SetCPass(m_cpass);
	}

	m_axConnectD->SetProgK(m_progK);
	
	if (m_axConnectD->DoModal() == IDCANCEL)
	{
		m_wizard->LogOff();
		return false;
	}
	return true;
}

void CMainFrame::UnregisterControl()
{
	char*	control[] = { "axWizard.ocx", "axSock.ocx", "axXecure.ocx", NULL };
//	char*	control[] = { "axWizard.ocx", "axSock.ocx", NULL };
	CString	path; 
	HINSTANCE hLib;

	for (int ii = 0; control[ii] != NULL; ii++)
	{
		path.Format("%s\\%s\\%s", m_home, RUNDIR, control[ii]);
		hLib = LoadLibrary(path);
		if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		{
			TRACE("LoadLibrary error....[%s] error=[%d]\n", path, GetLastError());
			continue;
		}

		FARPROC	lpDllEntryPoint;
		(FARPROC &)lpDllEntryPoint = GetProcAddress(hLib, _T("DllUnregisterServer"));
		if (lpDllEntryPoint == NULL)
		{
			FreeLibrary(hLib);
			continue;
		}

		(*lpDllEntryPoint)();
		FreeLibrary(hLib);
	}
}

void CMainFrame::registerControl()
{
	UnregisterControl();
	CString log;
	char*	control[] = { "axWizard.ocx", "axSock.ocx", "axXecure.ocx", NULL };
//	char*	control[] = { "axWizard.ocx", "axSock.ocx", NULL };
	CString	path;
	HINSTANCE hLib;

	for (int ii = 0; control[ii] != NULL; ii++)
	{
		path.Format("%s\\%s\\%s", m_home, RUNDIR, control[ii]);
		hLib = LoadLibrary(path);
		if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		{
			log.Format("@@ LoadLibrary error....[%s] error=[%d]\n", path, GetLastError());
			OutputDebugString(log);
			continue;
		}

		FARPROC	lpDllEntryPoint;
		(FARPROC &)lpDllEntryPoint = GetProcAddress(hLib, _T("DllRegisterServer"));
		if (lpDllEntryPoint == NULL)
		{
			FreeLibrary(hLib);
			
			log.Format(_T("@@ register ocx fail [%s]\n"), control[ii]);
			OutputDebugString(log);	
			continue;
		}

		(*lpDllEntryPoint)();
		FreeLibrary(hLib);
	}
}

BOOL CMainFrame::getConnectInfo(CString& ips, int& port)
{
	switch (m_connectBy)
	{
	case byUDP:
		port = 15201;
		break;
	case byUDD:
		port = 15203;
		break;
	case byTCP:
	default:
		port = 15201;
		break;
	}

	CString	info = m_axis->GetProfileString(INFORMATION, byTCPs);
	int pos = info.Find('|');
	if (pos >= 0)
	{
		ips = info.Left(pos);
		port = atoi(info.Mid(pos+1));
	}
	else
	{
		ips = _T("121.189.59.93");
		port = 15101;
	}

	return TRUE;
}

void CMainFrame::endWorkstation()
{
	ProcessSecure();
	if (!m_bExit)	return;

	m_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, AfxGetInstanceHandle(), GetCurrentThreadId());
	m_axisres->ChangeRES(GetSkinName());
	if(m_smain == NULL)
		return;
	m_smain->set_MenuInfo(m_tMenu);
	m_smain->Change_Skin(GetSkinName());

	m_axMisc->LoadGuide();

	m_MClient->ShowWindow(SW_SHOW);
	SetForegroundWindow();

	load_history();
	load_MngSetup();

	m_smcall = new CSmcall();
	m_smcall->Set_Infomation(axiscall, m_tMenu, m_home, m_userN);



	deleteNewsfile();
}

CString CMainFrame::GetUUidByID(CString strid)
{
	CString	file, tmpS;
	char	buf[1024];

	file.Format("%s\\%s\\uuid.ini", m_home, TABDIR);
	GetPrivateProfileString("UUID", strid, "", buf, sizeof(buf), file);
	tmpS.Format(_T("%s"), buf);
	return tmpS;
}

void CMainFrame::signOn()
{
	ResetUserstat();
	m_axConnectD->SetGuide(_T("사용자정보 확인 중 입니다."));
		
	CString ips, tmpS;
	int port; 
	getConnectInfo(ips, port);

	BOOL	rc = FALSE;
	CString	str;

	char	wb[128], encPass[32];

	ZeroMemory(encPass, sizeof(encPass));
	struct	_signM {
		char	usid[30];
		char	pswd[16];
		char	uuid[36];
		char	uuidrec[1];
		char	lcip[15];
	} signM;
	
	m_step  = axOPENSIGN;
	m_user  = m_axConnectD->GetUserID();
	m_pass  = m_axConnectD->GetPassword();
	m_cpass = m_axConnectD->GetCPass();
	m_uuid = GetUUidByID(m_user);
	CopyMemory(encPass, m_pass, m_pass.GetLength());
	if (m_connectBy == byTCP)
	{
		LONG_PTR	ret = 0;
		sprintf_s(wb, "%s\t%s", m_pass, m_user);
		CString ss = m_wizard->encryptPASS(CString(wb), 1);
		memset(encPass, ' ', sizeof(encPass));
		CopyMemory(encPass, ss, ss.GetLength());
	}
	
	str.Format("%10s", _T(" "));
	m_axConnectD->SetProgress(false);
	m_axConnectD->SetStatus(SM_WSH);
	m_axConnectD->SetChoice(false);

	memset(&signM, ' ', sizeof(struct _signM));
	CopyMemory(signM.usid, m_user, m_user.GetLength());
	CopyMemory(signM.pswd, encPass, sizeof(encPass));

	CopyMemory(signM.uuid, m_uuid, m_uuid.GetLength());
	CopyMemory(signM.uuidrec, m_buuidcreate == true ? "Y":"N", 1);
	GetLocalIP();
	CopyMemory(signM.lcip, m_ipAddr, m_ipAddr.GetLength());
	
	int nBytes = sizeof(_signM);
	CopyMemory(wb, (char *)&signM, nBytes);
	wb[nBytes] = '\0';

	m_wizard->setSignUser(wb, nBytes);

	if (!rc)
	{
		m_step = axNONE;
		m_axMisc->GetGuide(AE_ESIGNON, str);
		m_axConnectD->SetGuide(str);
	}
}

void CMainFrame::GetLocalIP()
{
	char szHostName[64] = {0};

	::gethostname(szHostName, sizeof(szHostName));

	if(lstrcmp(szHostName, "") != 0)
	{
		HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
		CString	tmps, serverips = m_ip;
/*
		if (m_isHiNuri || m_nIPCC == 2)
			tmps = m_axis->GetProfileString(INFORMATION, "Port", Format("%d", m_bCustomer? portProxy: portHinuri));
		else
			tmps = m_axis->GetProfileString(INFORMATION, "Port", Format("%d", m_bCustomer? portProxy: portEmployee));
*/
		UINT	port = atoi(tmps);

		CSocket	sock;
		sock.Create();

		if (sock.Connect(serverips, port))
		{
			IN_ADDR	in;
			SOCKADDR_IN sockAddr;
			int nSize = sizeof(SOCKADDR_IN);
			getsockname(sock.m_hSocket, (PSOCKADDR)&sockAddr, &nSize);
			sock.Close();

			CopyMemory(&in, &sockAddr.sin_addr.s_addr, sizeof(IN_ADDR));
			tmps = inet_ntoa(in);
			tmps.TrimRight();
			m_ipAddr = tmps;
		}
		else
		{
			for (int ii = 0; lphostent; ii++)
			{
				if (!lphostent->h_addr_list[ii])
					break;
				sprintf_s(szHostName, "%u.%u.%u.%u",
					0xff & lphostent->h_addr_list[ii][0],
					0xff & lphostent->h_addr_list[ii][1],
					0xff & lphostent->h_addr_list[ii][2],
					0xff & lphostent->h_addr_list[ii][3]);
				m_ipAddr = szHostName;
			}
		}
	}

}

void CMainFrame::setTitle(int key, CString title)
{
	CString	mapN = _T("");
	int	pos = title.Find('\t');
	if (pos != -1)
	{
		mapN = title.Left(pos++);
		title = title.Mid(pos);
	}

	if (key & winK_POPUP)
	{
		if (!m_arMPOP.IsEmpty())
		{
			CMPop*	Mpop;
			if (m_arMPOP.Lookup(key, Mpop))
			{
				if (!mapN.IsEmpty())
					Mpop->m_mapN = mapN;
				SetSCN((CWnd*) Mpop, key, Mpop->m_mapN, title);
				return;
			}
		}

		CGPop* Gpop;
		if (!m_arGPOP.Lookup(key, Gpop))	return;
		SetSCN((CWnd*) Gpop, key, Gpop->m_mapN, title);
	}
	else
	{
		bool		match = false;
		CSChild*	schild;
		CChildFrame*	child;
		int vsN = 0;
		for (vsN = V_SCREEN1; vsN <= V_SCREEN4 && !match; vsN++)
		{
			if (!m_arMDI[vsN].Lookup(key, child))
				continue;
			
			match = true;
			if (!mapN.IsEmpty())
			{
				child->m_mapN = mapN;
				m_bar1->set_Maps(mapN);   //test
				m_tMenu->set_Maps(mapN);
				m_bar2->change_Info(key, mapN, m_tMenu);
				changeLinkInfo(child->m_linkBar, child->m_mapN);
				if (!ExistMenu(mapN.Left(L_MAPN)))
				{
					CString	string = child->m_xcaption.GetTitle();
					m_bar2->change_ButtonText(key, string, vsN);
				}
			}
			SetSCN((CWnd*) child, key, child->m_mapN, title);
		}

		if (!match)
		{
			for (vsN = V_SCREEN1; vsN <= V_SCREEN4 && !match; vsN++)
			{
				if (!m_arSDI[vsN].Lookup(key, schild))
					continue;
				match = true;

				if (!mapN.IsEmpty())
				{
					schild->m_mapN = mapN; 
					m_bar1->set_Maps(mapN);  //test
					m_tMenu->set_Maps(mapN);
					m_bar2->change_Info(key, mapN, m_tMenu);
					changeLinkInfo(schild->m_linkBar, schild->m_mapN);
					if (!ExistMenu(mapN.Left(L_MAPN)))
					{
						CString	string = schild->m_xcaption.GetTitle();
						m_bar2->change_ButtonText(key, string, vsN);
					}
				}
				SetSCN((CWnd*) schild, key, schild->m_mapN, title, true);
			}
		}
	}
}

void CMainFrame::SetSCN(CWnd* wnd, int key, CString mapN, CString title, bool bSDI)
{
	if (!wnd->GetSafeHwnd() || title.IsEmpty())
		return;

	CString	tmps;

#ifdef _DEBUG
	tmps.Format("(%d-%s) %s", key, mapN.Left(L_MAPN), title);
	title = tmps;
#else
	title.TrimLeft();
	if (title.GetAt(0) != '[')
	{
		tmps.Format("[%s] %s", m_tMenu->GetDispN(mapN), title);
		title = tmps;
	}
#endif	
	if (WK_NORM <= key && key < WK_POPUP)
	{
		if (bSDI)
			((CSChild*) wnd)->m_xcaption.SetTitle(title);
		else	((CChildFrame*) wnd)->m_xcaption.SetTitle(title);
	}
	else	wnd->SetWindowText(title);

	CString	mpN = m_tMenu->GetDispN(mapN);
	if (m_tMenu->IsMappinged(mapN))
		m_wizard->setScreenNumber(key, mpN);		
}

bool CMainFrame::setFormPos(CWnd* pChild, CString sMap, int key, int position, CPoint point, int nCx, int nCy)
{
	CPoint	sp;

	if (m_bSDI)
		sp = ((CSChild*)pChild)->m_defaultpos;
	else
		sp = ((CChildFrame*)pChild)->GetSP();

	UINT	nFlag = m_bSDI ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;

	CRect	clientRc;

	switch (m_posChild)
	{
	default:
	case cpDEFAULT:
		if (!position)
			pChild->SetWindowPos(NULL, sp.x, sp.y, nCx, nCy, nFlag);
		else if (position == -1)
			pChild->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else	pChild->SetWindowPos(NULL, 0, 0/*POS_CHILDMOVE*/, nCx, nCy, nFlag);
		break;
	case cpTOP:
	{
		if (position == -1)
			pChild->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else
		{
			m_MClient->GetWindowRect(clientRc);
			pChild->SetWindowPos(NULL, (clientRc.Width() / 2) - (nCy / 2), 0, nCx, nCy, SWP_SHOWWINDOW);
		}
	}
	break;
	case cpCENTER:
	{
		if (position == -1)
			pChild->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else
		{
			m_MClient->GetWindowRect(clientRc);
			pChild->SetWindowPos(NULL, 0, (clientRc.Height() / 2) - (nCy / 2), nCx, nCy, SWP_SHOWWINDOW);
		}
	}
	break;
	case cpMAX:
	{
		if (position == -1)
			pChild->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else
		{
			m_MClient->GetWindowRect(clientRc);
			pChild->SetWindowPos(NULL, clientRc.right - nCx, (clientRc.Height() / 2) - (nCy / 2), nCx, nCy, SWP_SHOWWINDOW);
		}
	}
	break;
	}
	pChild->ShowWindow(SW_SHOW);

	return true;
}

int CMainFrame::CopyScreen(CString mapN, int group, int fontsize, CPoint sp)
{
	//return m_pFormMgr->CopyScreen(mapN, group, fontsize, -1, sp);
	return m_pFormMgr->CreateChild(mapN, group, 0, -1, sp);
}

int CMainFrame::ChangeChild(int index)
{
	CString	mapN, args, title;
	if (index < ID_MENU_TB)
	{
		index -= ID_MENU_BASE;
		if (index >= 0 && index < m_tMenu->GetMCount())
		{
			mapN = m_tMenu->GetMap(index);
			if (mapN.Find(WEB) == 0)
			{
				char	wb[1024];
				CString url = m_tMenu->GetInfo(mapN);
				if (url.IsEmpty())	return 0;
				sprintf_s(wb, "%s", url);

				RunWebBrowser(wb);
				return 0;
			}
		}
	}
	else if (index < ID_MENU_USERTOOL)
	{
		index -= ID_MENU_TB;
		title.LoadString(ST_TEXT_OK);
		::MessageBox(m_hWnd, "ID_MENU_TB", title, MB_OK);
		return 0;
	}
	else if (index < ID_MENU_END)
	{
		index -= ID_MENU_USER;
		createUserScreen(index);
		return 0;
	}
	else
	{
		title.LoadString(ST_TEXT_OK);
		::MessageBox(m_hWnd, "Error select", title, MB_OK);
		return 0;
	}

	if (mapN.GetLength() < L_MAPN)
		return 0;
	return m_pFormMgr->ChangeChild(mapN.Left(L_MAPN));
}

int CMainFrame::CreateModal(CString mapN, int trigger, int key, int position, int actkey, CPoint point)
{
	CString mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	mapname = m_mapN;

	int vtype = 0, vwTYPE = 0, size = 0;
	vtype = m_wizard->getFormInfo(mapname, size);

	switch (vtype)
	{
	case vtypeERR:
		return 0;
	case vtypeHTM:
	default:
		break;
	}
	
	CWnd*	parent = this;
	if (actkey & winK_POPUP)
	{
		m_arMPOP.Lookup(actkey, (CMPop *&) parent);
		m_arGPOP.Lookup(actkey, (CGPop *&) parent);
	}
	else
		m_arMDI[m_vsN].Lookup(actkey, (CChildFrame *&) parent);

	int	cyBorder  = GetSystemMetrics(SM_CYBORDER);
	int	cxBorder  = GetSystemMetrics(SM_CXBORDER);
	int	cxEdge    = GetSystemMetrics(SM_CXEDGE);
	int	cyEdge    = GetSystemMetrics(SM_CYEDGE);
	int	cyCaption = GetSystemMetrics(SM_CYSMCAPTION);

	CRect windowRC(0, 0, 0, 0);
	windowRC.top   -= (HIWORD(size) + cyCaption + (cyBorder*2) + (cyEdge*4));
	windowRC.right += (LOWORD(size) + (cxBorder*2) + (cxEdge*4)); 

	CRect	clientRC, rcWA;
	int	xPosition = 0, yPosition = 0;
	if (m_bSDI)
	{
		g_monRc.SetRectEmpty();
		EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);
		clientRC = g_monRc;
		if(clientRC.left < 0) clientRC.left = 0;
	}
	else
		m_MClient->GetWindowRect(clientRC);

	//::GetWindowRect(m_hWndMDIClient, &clientRC);
	windowRC.SetRect(0, 0, LOWORD(size), HIWORD(size));

	switch (position)
	{
	case LtopPOS:		// left_top
		xPosition = clientRC.left;
		yPosition = clientRC.top;
		break;

	case RtopPOS:		// right_top
		xPosition = clientRC.right - windowRC.Width();
		yPosition = clientRC.top;
		break;

	case LbottomPOS:	// left_bottom
		xPosition = clientRC.left;
		yPosition = clientRC.bottom - windowRC.Height();
		break;

	case RbottomPOS:	// right_bottom
		xPosition = clientRC.right - windowRC.Width();
		yPosition = clientRC.bottom - windowRC.Height();
		break;
	default:		// center
		xPosition = clientRC.CenterPoint().x - windowRC.Width()/2;
		yPosition = clientRC.CenterPoint().y - windowRC.Height()/2;
		break;		
	}

	CMPop*	pop = new CMPop(parent);

	pop->m_mapN   = mapN;
	pop->m_vwKEY  = key;
	pop->m_vwTYPE = vtype;
	pop->m_wpos   = position;

 	if (position == -1)
 		pop->SetPos(point.x, point.y);
 	else
		pop->SetPos(xPosition, yPosition);

 	pop->SetSize(LOWORD(size), HIWORD(size));

	m_modal = pop;
	m_activeMapN = pop->m_mapN;
	if (pop->DoModal() == IDOK)
	{
		if (!m_MMapN.IsEmpty())
		{
			m_wizard->detachForm(pop->m_vwKEY);
			m_arMPOP.RemoveKey(pop->m_vwKEY);
			removeWait(pop->m_vwKEY);
			delete pop;
			m_modal = NULL;

			return -1;
		}
	}

	m_wizard->detachForm(pop->m_vwKEY);
	m_arMPOP.RemoveKey(pop->m_vwKEY);
	removeWait(pop->m_vwKEY);
	delete pop;
	m_modal = NULL;

	if (m_bSDI)
		axSFocus();

	return key;
}

bool CMainFrame::closeX()
{
	if (m_arMPOP.GetCount() > 0)
	{
		CString	title, msg;
		title.LoadString(ST_TEXT_OK);
		msg.LoadString(ST_MSG_MODALEXIT);
		::MessageBox(m_hWnd, msg,
				title, MB_ICONINFORMATION);
		return false;
	}

	int	key;
	CGPop*	pop;	
	for (POSITION pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, pop);
		if (pop->GetSafeHwnd())
			pop->SendMessage(WM_CLOSE, 0, 0);
	}
	return true;
}

bool CMainFrame::closePopup()
{
	CGPop*	pop;	
	int	key;
	for (POSITION pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, pop);
		if (!pop->m_active)	
			continue;
		pop->SendMessage(WM_CLOSE, 0, 0);
		m_arGPOP.RemoveKey(key);
		return true;
	}
	
	return false;
}

bool CMainFrame::closePopup(int key)
{
	CGPop*	pop;
	if (!m_arGPOP.Lookup(key, pop))		return false;
	pop->SendMessage(WM_CLOSE, 0, 0);	return true;
}

int CMainFrame::create_Newview(int actkey, char* data)
{
	int	 key, position;
	CString	 mapN;
	CPoint	 point;
	_userWH64* userWH64;

	userWH64 = (struct _userWH64 *)data;
	mapN = CString(userWH64->maps);
	
	if (userWH64->pos.x == -1)
		position = userWH64->pos.y;
	else	
		position = 0;

	point = CPoint(abs(userWH64->pos.x), userWH64->pos .y);

	switch (userWH64->type)
	{
	case typeVIEW:
		key = m_pFormMgr->CreateChild(mapN, userWH64->group, userWH64->key, position, point);
		if (position != -1)	positionWindow(actkey, key, position);
		return key;
	case typeMODAL:
		key = CreateModal(mapN, userWH64->group, userWH64->key, position, actkey, point);
		if (position != -1)	positionWindow(actkey, key, position);
		return key;
	case typePOPUP:
		key = m_pFormMgr->CreatePopup(mapN, userWH64->group, userWH64->key, position, actkey, point);
		if (position != -1)	positionWindow(actkey, key, position);
		return key;
	case typePOPUPX:
		key = m_pFormMgr->CreatePopup(mapN, userWH64->group, userWH64->key, position, actkey, point, true);
		if (position != -1)	positionWindow(actkey, key, position);
		return key;
	}
	return 0;
}

void CMainFrame::loadMap(int key, int size)
{
	if (!m_modal)	return;
	key = m_modal->m_vwKEY;

	key = m_wizard->AttachForm((CView*)m_modal->m_wnd, m_modal->m_vwTYPE, key);


	m_modal->m_vwKEY = key;
	m_arMPOP.SetAt(key, m_modal);
 
	if (!m_wizard->loadMap(m_modal->m_mapN, key, size))
		m_modal->PostMessage(WM_CLOSE, 0, 0);
	else
		m_modal->SetFocus();
}

void CMainFrame::showfname()
{
	CFname	dlg;

	if (dlg.DoModal() == IDOK)
	{
		CString mapN = dlg.GetMapName();
		if (mapN.GetLength() == L_MAPN)
			m_pFormMgr->ChangeChild(mapN);																																																																														}
}

void CMainFrame::WriteFile(char* pBytes, int nBytes)
{
#if 0
	CFile		Dfile;
	DWORD		pos;
	CString		filename, time;

	filename.Format("%s\\log", m_home);
	BOOL bExist = Dfile.Open(filename, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);
	if (!bExist)
	{
		Dfile.Open(filename, CFile::modeCreate|CFile::modeReadWrite);
		Dfile.Write(pBytes, nBytes);
	}
	else
	{
		pos = Dfile.SeekToEnd();
		Dfile.Seek((long) pos, CFile::begin);
		Dfile.Write(pBytes, nBytes);
	}
	Dfile.Close();
#endif
}

void CMainFrame::write_err()
{
	CString	err;
	DWORD	dw = GetLastError();
	err.Format("[GDI_ERROR] [%u]\n", dw);
	WriteFile((char *)err.operator LPCTSTR(), err.GetLength());
}

void CMainFrame::update_ticker(int kind, CString dat)
{
	CString	key, symbol, value, tmps;
	int	pos = dat.Find('\t');
	symbol = dat.Left(pos++);
	
	if(dat.Find(_T("PUSHALARM")) >= 0 && dat.Find(_T("501")) >= 0)
	{
		ShowFITSInfo(dat);
		return;
	}
	tmps = Parser(dat, _T("\t"));
	symbol = Parser(dat, _T("\t"));
	if(tmps.GetLength() == 12 && symbol == _T("401"))
		addCJCode(dat);
	return;
}

void CMainFrame::change_Skin()
{
	DrawFrame();
	change_BackGround();
	change_Resource();
	if (m_mngInfo)		
		m_mngInfo->ChangePalette();

	m_wizard->changeSkin();
}

void CMainFrame::change_BackGround()
{

}

void CMainFrame::restore(int key)
{
	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))	return;
	child->SetWindowPos(NULL, 0, 0, child->m_width, child->m_height, SWP_NOMOVE);
}

void CMainFrame::positionWindow(int actkey, int poskey, int pos, bool child)
{
	CWnd* posW;
	if (pos < LtopPOS)	return;
	if (!m_arMDI[m_vsN].Lookup(poskey, (CChildFrame *&) posW))
	{
		if (!m_arSDI[m_vsN].Lookup(poskey, (CSChild *&) posW))
			return;
	}
	positionWindow(actkey, posW, pos, child);
}

void CMainFrame::positionWindow(int actkey, CWnd* posW, int pos, bool child)
{
	CWnd* actW = NULL;

	if (pos < LtopPOS || (actkey == 0 && pos > CenterPOS))
	{
		posW->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
		return;
	}

	if (actkey & 0x80)	;
	else
	{
		if (!m_arMDI[m_vsN].Lookup(actkey, (CChildFrame *&) actW))
		{
			if (!m_arSDI[m_vsN].Lookup(actkey, (CSChild *&) actW))
				return;
		}
	}

	if (!actW && pos > CenterPOS)
	{
		posW->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
		return;
	}
	positionWindow(actW, posW, pos, child);
}

void CMainFrame::positionWindow(CWnd* actW, CWnd* posW, int pos, bool child)
{
	if (pos < LtopPOS || !posW || !::IsWindow(posW->GetSafeHwnd()))
		return;

	CRect	rect, clientRc;
	CPoint	point;

	switch (pos)
	{
	case LtopPOS:case RtopPOS:case LbottomPOS:case RbottomPOS:case CenterPOS:
	case LtopLup:case LtopRup:case LtopLdown:case LtopRdown:case RtopLup:
	case RtopRup:case RtopLdown:case RtopRdown:case LbottomLup:case LbottomRup:
	case LbottomLdown:case LbottomRdown:case RbottomLup:case RbottomRup:case RbottomLdown:
	case RbottomRdown:case CenterView:	break;
	default:pos = CenterPOS;		break;
	}

	int	SubY = 0;
	if (actW)
	{
		actW->GetWindowRect(&rect);
		if (!m_bSDI)
		{
			if (child)
				m_MClient->ScreenToClient(&rect);
		}
	}

	switch (pos / 10)
	{
	case 0:
		posW->GetWindowRect(&rect);

		if (m_bSDI)
		{
			g_monRc.SetRectEmpty();
			EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);
			clientRc = g_monRc;
			if(clientRc.left < 0) clientRc.left = 0;
			SubY = 107 + (GetSystemMetrics(SM_CYCAPTION) - 19);
		}
		else
		{
			if (child)	m_MClient->GetClientRect(&clientRc);
			else		m_MClient->GetWindowRect(&clientRc);
		}

		switch (pos)
		{
		case LtopPOS:
			point.x = clientRc.left;
			point.y = clientRc.top;
			break;

		case RtopPOS:
			point.x = clientRc.right - rect.Width();
			point.y = clientRc.top;
			break;

		case LbottomPOS:
			point.x = clientRc.left;
			point.y = clientRc.bottom - rect.Height();
			break;

		case RbottomPOS:
			point.x = clientRc.right - rect.Width();
			point.y = clientRc.bottom - rect.Height();
			break;

		case CenterPOS:
		default:
			if(m_bSDI)
				posW->MoveWindow((clientRc.Width() /2) - (rect.Width()/2), (clientRc.Height() /2) - (rect.Height()/2), rect.Width(), rect.Height());
			else
				posW->CenterWindow(m_MClient);
			return;
		}
		posW->MoveWindow(point.x, point.y, rect.Width(), rect.Height());
		return;

	case 9:	// CenterView
		posW->CenterWindow(actW);
		return;

	case 1:	// Ltop
		point = CPoint(rect.left,  rect.top);		break;
	case 2:	// Rtop
		point = CPoint(rect.right, rect.top);		break;
	case 3:	// Lbottom
		point = CPoint(rect.left,  rect.bottom);	break;
	case 4:	// Rbottom
		point = CPoint(rect.right, rect.bottom);	break;
	}

	posW->GetWindowRect(&rect);
//	m_axMDIClient->ScreenToClient(&rect);
	switch (pos % 10)
	{
	case 1:	// Lup
		point.x -= rect.Width();
		point.y -= rect.Height();	break;

	case 2:	// Rup
		point.y -= rect.Height();	break;

	case 3:	// Ldown
		point.x -= rect.Width();	break;

	case 4:	// Rdown
		break;
	}
	if (child)	posW->MoveWindow(point.x, point.y, rect.Width(), rect.Height());
	else		posW->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
}

bool CMainFrame::Agree_Duplication(CString mapN)
{
	int		key;
	POSITION	pos;
	WINDOWPLACEMENT	pl;
	if (m_bSDI)	return !FindSDIMap(mapN);
	else
	{
		CChildFrame* pChild = NULL;
		for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);
			if (getSMap(pChild->m_mapN.Left(L_MAPN)) != getSMap(mapN.Left(L_MAPN)))
				continue;
			if (IsSelectMap(mapN))	loadDomino(pChild->m_key,  mapN);
		
			pChild->GetWindowPlacement(&pl);
			if (pl.showCmd & SW_SHOWMINIMIZED)
			{
				pl.showCmd = SW_RESTORE;
				pChild->SetWindowPlacement(&pl);
			}
			else
			{
				CString	data = mapN.Mid(L_MAPN);
				while (!data.IsEmpty())
				{
					CString	dat;
					int	po = data.Find('\n');
					if (po == -1)
					{
						dat = data;
						data.Empty();
					}
					else
					{
						dat = data.Left(po++);
						data = data.Mid(po);
					}
					m_wizard->setTrigger(key, dat);					
				}
				pChild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

				if(pChild->m_bIconic)  //test 최소화일때
				{
					pChild->actionCaption(IDX_RESTORE, CPoint(0, 0));
				}
			}
			return false;
		}

		if (FindSDIMap(mapN))	
			return false;
	}

	return true;
}

bool CMainFrame::FindSDIMap(CString mapName, int vsN)
{
	int		key;
	POSITION	pos;
	WINDOWPLACEMENT	pl;
	CSChild*	schild;

	if (vsN == -1)	vsN = m_vsN;
	for (pos = m_arSDI[vsN].GetStartPosition(); pos; )
	{
		m_arSDI[vsN].GetNextAssoc(pos, key, schild);
		if (getSMap(schild->m_mapN.Left(L_MAPN)) != getSMap(mapName.Left(L_MAPN)))
			continue;
		if (IsSelectMap(mapName))	loadDomino(schild->m_key,  mapName, true);
		schild->GetWindowPlacement(&pl);
		if (pl.showCmd & SW_SHOWMINIMIZED)
		{
			pl.showCmd = SW_RESTORE;
			//schild->SetWindowPlacement(&pl);

			if(schild->m_bIconic)  //test 최소화일때
				m_pFormMgr->Setfocus_Child(schild->m_key, schild->m_bIconic);
		}
		else
		{
			CString	data = mapName.Mid(L_MAPN);
			while (!data.IsEmpty())
			{
				CString	dat;
				int	po = data.Find('\n');
				if (po == -1)
				{
					dat = data;
					data.Empty();
				}
				else
				{
					dat = data.Left(po++);
					data = data.Mid(po);
				}
				m_wizard->setTrigger(key, dat);
			}
			schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		}
		return true;
	}
	return false;
}


void CMainFrame::changeSize(int key, CSize size)
{
	if (size.cx == 0 && size.cy == 0)	return;

	int	nCx, nCy, gapX, gapY, viewType;
	gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
	gapX += GetSystemMetrics(SM_CXBORDER) * 2;

	gapY  = GetSystemMetrics(SM_CYFRAME) * 2;
	gapY += GetSystemMetrics(SM_CYBORDER) * 2;

	m_fit = true;
	if (key & winK_POPUP)
	{
		CMPop*	Mpop;
		CGPop*	Gpop;
		if (m_arMPOP.Lookup(key, Mpop))
		{			
			gapY += GetSystemMetrics(SM_CYCAPTION);

			nCx = size.cx + GetSystemMetrics(SM_CXFRAME) * 2;
			nCy = size.cy + GetSystemMetrics(SM_CYFRAME) * 2;
			nCy += GetSystemMetrics(SM_CYCAPTION);
			Mpop->SetSize(size.cx, size.cy);
			if (Mpop->GetSafeHwnd())
				Mpop->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
		}
		else if (m_arGPOP.Lookup(key, Gpop))
		{			
			gapY += GetSystemMetrics(SM_CYCAPTION);

			Gpop->m_width  = nCx = size.cx + gapX;
			Gpop->m_height = nCy = size.cy + gapY;
			Gpop->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
		}
		return;
	}

	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		CSChild*	schild;
		if (!m_arSDI[m_vsN].Lookup(key, schild))
			return;

		if (isShowLinkbar(schild->m_mapN))
			gapY += LBAR_HEIGHT;
			gapY += XCAP_HEIGHT + 2;
		if (schild->m_tView)	gapY +=	TABH;

		viewType = schild->m_vwTYPE & vtypeMSK;
		if (viewType == vtypeRSZ)
		{
			if (size.cx == 0)
			{
				nCx = schild->m_width;
				schild->m_height = nCy = size.cy + gapY;
				schild->m_fixW = true;
				schild->m_fixH = false;
			}
			else if (size.cy == 0)
			{
				schild->m_width = nCx = size.cx + gapX;
				nCy = schild->m_height;
				schild->m_fixW = false;
				schild->m_fixH = true;
			}
			else
			{
				schild->m_width = nCx = size.cx + gapX;
				schild->m_height = nCy = size.cy + gapY;
			}
		}
		else
		{
			schild->m_width = nCx = size.cx + gapX;
			schild->m_height = nCy = size.cy + gapY;
		}

		CView*	view = schild->GetActiveView();
		schild->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
		if (viewType == vtypeSCR)
			((CAxScrollView *)view)->SetSizeView(size.cx, size.cy);
		if (schild->m_tView)	((CAxTabView *) view)->SizeWindow(size);
		return;
	}

	gapY += XCAP_HEIGHT + 2;
	
	if (isShowLinkbar(child->m_mapN))		gapY += LBAR_HEIGHT;
	if (child->m_tView)	gapY +=	TABH;

	viewType = child->m_vwTYPE & vtypeMSK;
	if (viewType == vtypeRSZ)
	{
		if (size.cx == 0)
		{
			nCx = child->m_width;
			child->m_height = nCy = size.cy + gapY;
			child->m_fixW = true;
			child->m_fixH = false;
		}
		else if (size.cy == 0)
		{
			child->m_width = nCx = size.cx + gapX;
			nCy = child->m_height;
			child->m_fixW = false;
			child->m_fixH = true;
		}
		else
		{
			child->m_width = nCx = size.cx + gapX;
			child->m_height = nCy = size.cy + gapY;
		}
	}
	else
	{
		child->m_width = nCx = size.cx + gapX;
		child->m_height = nCy = size.cy + gapY;
	}

	CView*	view = child->GetActiveView();
	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	if (viewType == vtypeSCR)
			((CAxScrollView *)view)->SetSizeView(size.cx, size.cy);
	if (child->m_tView)	((CAxTabView *)view)->SizeWindow(size);
}

bool CMainFrame::HotKey(WPARAM wParam, LPARAM lParam)
{
	int	ctrlkey = HIWORD(lParam);
	int	shiftkey  = LOWORD(lParam);

	CString	file, key, mapN, screenN;
	int	index = 0;
	switch (wParam)
	{
	case VK_F12:	index++;	case VK_F11:	index++;
	case VK_F10:	index++;	case VK_F9:	index++;
	case VK_F8:	index++;	case VK_F7:	index++;
	case VK_F6:	index++;	case VK_F5:	index++;
	case VK_F4:	index++;	case VK_F3:	index++;
	case VK_F2:	index++;	case VK_F1:	index++;	break;
	default:	return false;
	}

	if (ctrlkey)	key.Format("CTRL+F%d", index);
	else if (shiftkey)key.Format("SHIFT+F%d", index);
	else		key.Format("F%d", index);

	char	buf[256];
	file.Format("%s\\%s\\%s\\hkey.ini", m_home, USRDIR, GetUserName());
	DWORD dw = GetPrivateProfileString("HKEY", key, "", buf, sizeof(buf), file);
	if (dw < 4)	return false;
	screenN = buf;
	screenN = screenN.Left(4);

	mapN = m_tMenu->GetMap(screenN);
	m_pFormMgr->ChangeChild(mapN);
	return true;
}

LRESULT CMainFrame::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
{
	return 0;
	CString	info = m_bar1->screenInfo();
	CString desc = m_tMenu->GetDesc(info);

	info += "\t" + desc;
	CopyMemory((char *)lParam, info, info.GetLength());
	return 0;
}

void CMainFrame::LockProg()
{
	bool	visblebar = false;
	m_block = true;

	m_lockDlg = new CLockDlg(this, m_user, m_pass, "");
	//CLockDlg lockDlg(NULL, m_user, m_pass);

	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	EnableWindow(FALSE);

	//ShowSingleWindows(FALSE);
	
	ShowWindow(SW_HIDE);
	
	bool	bConclusion = false;

	if (m_bSDI)
	{
		CWnd* pWnd = GetActiveWindow();

		int		key;
		CSChild*	schild = NULL;

		for (POSITION pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
			schild->ShowWindow(SW_HIDE);
		}
	}

	int key; CGPop* popup = NULL;

	for (POSITION pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, popup);

		popup->ShowWindow(SW_HIDE);
	}

	KillTimer(TM_IDLE);

	bool guidedlg = false;
	bool accndlg = false;
	bool concdlg = false;

	if (m_GuideDLG->IsWindowVisible())
	{
		guidedlg = true;
		m_GuideDLG->ShowWindow(SW_HIDE);
	}
	

	if (m_ConCDLG->IsWindowVisible())
	{
		concdlg = true;
		m_ConCDLG->ShowWindow(SW_HIDE);
	}

	m_lockDlg->DoModal();
	
	//lockDlg.DoModal();
	if (m_bSDI)
	{
		CWnd* pWnd = GetActiveWindow();

		int		key;
		CSChild*	schild = NULL;

		for (POSITION pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
			schild->ShowWindow(SW_SHOW);
		}
	}

	popup = NULL;

	for (POSITION pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, popup);

		popup->ShowWindow(SW_SHOW);
	}
	
	m_noActive = false;
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
	ShowWindow(SW_SHOW);
	m_noActive = true;
	m_block = false;

	if (guidedlg)
		m_GuideDLG->ShowWindow(SW_SHOW);


	if (concdlg)
		m_ConCDLG->ShowWindow(SW_SHOW);

	if (m_idletime > 0)
		SetTimer(TM_IDLE, m_idletime*60000, NULL);	
}

void CMainFrame::createUserScreen(int key)
{
	char	wb[512];
	CString	file, tmps;

	tmps = GetUserName();

	if(m_bSDI)
		file.Format("%s\\%s\\%s\\%sSDI.ini", m_home, USRDIR, tmps, tmps);
	else
		file.Format("%s\\%s\\%s\\%sMDI.ini", m_home, USRDIR, tmps, tmps);

	tmps.Format("%02d", key);
	DWORD dwRc = GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), file);
	if (dwRc <= 0)	return;

	tmps = wb;
	createUserScreen(tmps);
}

void CMainFrame::createUserScreen(CString mapN, bool allVS)
{
	int	pos, key, trigger, sdi;
	char	wb[512];
	CString	file, tmps, tmpx;
	CString	data, mapName;
	WINDOWPLACEMENT wndpl;
	CStringArray maparr;

	CString	SetConfig;

	tmps = GetUserName();

	if(m_bSDI)
		file.Format("%s\\%s\\%s\\%sSDI.ini", m_home, USRDIR, tmps, tmps);
	else
		file.Format("%s\\%s\\%s\\%sMDI.ini", m_home, USRDIR, tmps, tmps);

	m_pFormMgr->closeAll(false);

	DWORD dwRc;
	CArray	< int , int > ary;
	int vsN;
	for (vsN = V_SCREEN1; vsN <= V_SCREEN4 && allVS; vsN++)
	{
		if (vsN == m_vsN)	continue;
		ary.Add(vsN);
	}
	ary.Add(m_vsN);
	bool	showChild = false;
	for (;ary.GetSize();)
	{
		vsN = ary.GetAt(0);
		ary.RemoveAt(0);

		if (allVS)
		{
			m_wizard->setvirtualScreen(vsN);
		
		}
		
		if (!ary.GetSize())	
			showChild = true;

		tmpx.Format("%d",vsN);
		maparr.RemoveAll();
		ReadUserLast(tmpx, maparr);

		//if(maparr.GetSize() == 0)
		//	break;

		for (int ii = 0;  ii < maparr.GetSize() ; ii++)
		{
		//	tmpx.Format("%d%02d", allVS ? vsN : 0, ii);
		//	dwRc = GetPrivateProfileString(mapN, tmpx, "", wb, sizeof(wb), file);
		//	if (dwRc <= 0)	break;

			data  = maparr.GetAt(ii);
			data.Replace(";", "\n");
			for (int jj = 0; jj < 17; jj++)
			{
				pos = data.Find('|');
				if (pos < 0)
				{
					tmpx = data;
					data.Empty();
				}
				else
				{
					tmpx = data.Left(pos);
					data = data.Mid(pos+1);
				}

				tmpx.TrimLeft(); tmpx.TrimRight();
				switch (jj)
				{
				case 0:	mapName = tmpx;					break;	// mapN
				case 1:	sdi = atoi(tmpx);				break;	// 0 - mdi, 1 - sdi
				case 2:	wndpl.length  = atoi(tmpx);			break;	// WINDOWPLACEMENT.length
				case 3:	wndpl.flags   = atoi(tmpx);			break;	// .flags
				case 4:	wndpl.showCmd = 0	;			break;	// .showCmd
				case 5:	wndpl.ptMinPosition.x = atoi(tmpx);		break;	// .ptMinPosition.x
				case 6:	wndpl.ptMinPosition.y = atoi(tmpx);		break;	// .ptMinPosition.y
				case 7:	wndpl.ptMaxPosition.x = atoi(tmpx);		break;	// .ptMaxPosition.x
				case 8:	wndpl.ptMaxPosition.y = atoi(tmpx);		break;	// .ptMaxPosition.y
				case 9:	wndpl.rcNormalPosition.left = atoi(tmpx);	break;	// .rcNormalPosition.left
				case 10:wndpl.rcNormalPosition.top  = atoi(tmpx);	break;	// .rcNormalPosition.top
				case 11:wndpl.rcNormalPosition.right  = atoi(tmpx);	break;	// .rcNormalPosition.right
				case 12:wndpl.rcNormalPosition.bottom = atoi(tmpx);	break;	// .rcNormalPosition.bottom
				case 13:trigger = atoi(tmpx);				break;	// trigger
				case 14:if (!tmpx.IsEmpty())	mapName += tmpx;	break;	// symbol data
				case 15:
					{
						if (!tmpx.IsEmpty())	m_fontSize = atoi(tmpx);
						else	m_fontSize = 9;
					}
					break; // fontsize
				case 16:
					SetConfig = tmpx;
					break;
				}
			}

			if (isShowLinkbar(mapName))
				wndpl.rcNormalPosition.bottom += LBAR_HEIGHT;
			
			if(!CheckGonjiMap(mapName))
			{
				if(!IsExistInMenuFile(mapName.Left(8)))
					continue;
				key = m_pFormMgr->CreateChild(mapName, trigger, wndpl, showChild, vsN);
			
				CString symbol;
				symbol.Format("CONFIG\t%s",SetConfig);
				m_wizard->setTrigger(key, symbol);

				if (!m_bSDI)
				{
					CString mapN;
					mapN = mapName.Left(L_MAPN);
					if (getPopupMap(mapN) || sdi)
						actionCaption(key, IDX_SINGLE);
				}
			}
		}
		/*
		for (int ii = 1; ; ii++)
		{
			tmpx.Format("%d%02d", allVS ? vsN : 0, ii);
			dwRc = GetPrivateProfileString(mapN, tmpx, "", wb, sizeof(wb), file);
			if (dwRc <= 0)	break;

			data  = wb;
			data.Replace(";", "\n");
			for (int jj = 0; jj < 17; jj++)
			{
				pos = data.Find('|');
				if (pos < 0)
				{
					tmpx = data;
					data.Empty();
				}
				else
				{
					tmpx = data.Left(pos);
					data = data.Mid(pos+1);
				}

				tmpx.TrimLeft(); tmpx.TrimRight();
				switch (jj)
				{
				case 0:	mapName = tmpx;					break;	// mapN
				case 1:	sdi = atoi(tmpx);				break;	// 0 - mdi, 1 - sdi
				case 2:	wndpl.length  = atoi(tmpx);			break;	// WINDOWPLACEMENT.length
				case 3:	wndpl.flags   = atoi(tmpx);			break;	// .flags
				case 4:	wndpl.showCmd = 0	;			break;	// .showCmd
				case 5:	wndpl.ptMinPosition.x = atoi(tmpx);		break;	// .ptMinPosition.x
				case 6:	wndpl.ptMinPosition.y = atoi(tmpx);		break;	// .ptMinPosition.y
				case 7:	wndpl.ptMaxPosition.x = atoi(tmpx);		break;	// .ptMaxPosition.x
				case 8:	wndpl.ptMaxPosition.y = atoi(tmpx);		break;	// .ptMaxPosition.y
				case 9:	wndpl.rcNormalPosition.left = atoi(tmpx);	break;	// .rcNormalPosition.left
				case 10:wndpl.rcNormalPosition.top  = atoi(tmpx);	break;	// .rcNormalPosition.top
				case 11:wndpl.rcNormalPosition.right  = atoi(tmpx);	break;	// .rcNormalPosition.right
				case 12:wndpl.rcNormalPosition.bottom = atoi(tmpx);	break;	// .rcNormalPosition.bottom
				case 13:trigger = atoi(tmpx);				break;	// trigger
				case 14:if (!tmpx.IsEmpty())	mapName += tmpx;	break;	// symbol data
				case 15:
					{
						if (!tmpx.IsEmpty())	m_fontSize = atoi(tmpx);
						else	m_fontSize = 9;
					}
					break; // fontsize
				case 16:
					SetConfig = tmpx;
					break;
				}
			}

			if (isShowLinkbar(mapName))
				wndpl.rcNormalPosition.bottom += LBAR_HEIGHT;
			
			if(!CheckGonjiMap(mapName))
			{
				if(!IsExistInMenuFile(mapName.Left(8)))
					continue;
				key = m_pFormMgr->CreateChild(mapName, trigger, wndpl, showChild, vsN);
			
				CString symbol;
				symbol.Format("CONFIG\t%s",SetConfig);
				m_wizard->setTrigger(key, symbol);

				if (!m_bSDI)
				{
					CString mapN;
					mapN = mapName.Left(L_MAPN);
					if (getPopupMap(mapN) || sdi)
						actionCaption(key, IDX_SINGLE);
				}
			}
		}
		*/
	}

	CRect rect;

	dwRc = GetPrivateProfileString("GUIDE", "INFO", "", wb, sizeof(wb), file);
	if (dwRc <= 0 || wb[0] == '0')
		m_GuideDLG->ShowWindow(SW_HIDE);
	else
	{
		data  = wb;

		pos = data.Find('|');
		rect.left = atoi(data.Left(pos));
		data = data.Mid(pos + 1);

		pos = data.Find('|');
		rect.top = atoi(data.Left(pos));
		data = data.Mid(pos + 1);

		pos = data.Find('|');
		rect.right = atoi(data.Left(pos));
		rect.bottom = atoi(data.Mid(pos + 1));

		m_GuideDLG->SetWindowPos(&CWnd::wndTopMost, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
	}
	DeleteFile(file);
}

void CMainFrame::createUserScreenEx(CString sUser, CString mapN, bool allVS)
{
	int	pos, key, trigger, sdi;
	char	wb[512];
	CString	file, tmps, tmpx;
	CString	data, mapName;
	WINDOWPLACEMENT wndpl;

	CString	SetConfig;

	tmps = GetUserName();

	file.Format("%s\\%s\\%s\\%s.ini", m_home, USRDIR, tmps, sUser);

	m_pFormMgr->closeAll(false);

	DWORD dwRc;
	CArray	< int , int > ary;
	int vsN;
	for (vsN = V_SCREEN1; vsN <= V_SCREEN4 && allVS; vsN++)
	{
		if (vsN == m_vsN)	continue;
		ary.Add(vsN);
	}
	ary.Add(m_vsN);

	bool	showChild = false;
	for (;ary.GetSize();)
	{
		vsN = ary.GetAt(0);
		ary.RemoveAt(0);

		if (allVS)
		{
			m_wizard->setvirtualScreen(vsN);
		}
		
		if (!ary.GetSize())	showChild = true;
		for (int ii = 1; ; ii++)
		{
			tmpx.Format("%d%02d", allVS ? vsN : 0, ii);
			dwRc = GetPrivateProfileString(mapN, tmpx, "", wb, sizeof(wb), file);
			if (dwRc <= 0)	break;

			data  = wb;
			data.Replace(";", "\n");
			for (int jj = 0; jj < 17; jj++)
			{
				pos = data.Find('|');
				if (pos < 0)
				{
					tmpx = data;
					data.Empty();
				}
				else
				{
					tmpx = data.Left(pos);
					data = data.Mid(pos+1);
				}

				tmpx.TrimLeft(); tmpx.TrimRight();
				switch (jj)
				{
				case 0:	mapName = tmpx;					break;	// mapN
				case 1:	sdi = atoi(tmpx);				break;	// 0 - mdi, 1 - sdi
				case 2:	wndpl.length  = atoi(tmpx);			break;	// WINDOWPLACEMENT.length
				case 3:	wndpl.flags   = atoi(tmpx);			break;	// .flags
				case 4:	wndpl.showCmd = 0	;			break;	// .showCmd
				case 5:	wndpl.ptMinPosition.x = atoi(tmpx);		break;	// .ptMinPosition.x
				case 6:	wndpl.ptMinPosition.y = atoi(tmpx);		break;	// .ptMinPosition.y
				case 7:	wndpl.ptMaxPosition.x = atoi(tmpx);		break;	// .ptMaxPosition.x
				case 8:	wndpl.ptMaxPosition.y = atoi(tmpx);		break;	// .ptMaxPosition.y
				case 9:	wndpl.rcNormalPosition.left = atoi(tmpx);	break;	// .rcNormalPosition.left
				case 10:wndpl.rcNormalPosition.top  = atoi(tmpx);	break;	// .rcNormalPosition.top
				case 11:wndpl.rcNormalPosition.right  = atoi(tmpx);	break;	// .rcNormalPosition.right
				case 12:wndpl.rcNormalPosition.bottom = atoi(tmpx);	break;	// .rcNormalPosition.bottom
				case 13:trigger = atoi(tmpx);				break;	// trigger
				case 14:if (!tmpx.IsEmpty())	mapName += tmpx;	break;	// symbol data
				case 15:
					{
						if (!tmpx.IsEmpty())	m_fontSize = atoi(tmpx);
						else	m_fontSize = 9;
					}
					break; // fontsize
				case 16:
					SetConfig = tmpx;
					break;
				}
			}

			if (isShowLinkbar(mapName))
				wndpl.rcNormalPosition.bottom += LBAR_HEIGHT;
			
			key = m_pFormMgr->CreateChild(mapName, trigger, wndpl, showChild, vsN);
			
			CString symbol;
			symbol.Format("CONFIG\t%s",SetConfig);
			
			m_wizard->setTrigger(key, symbol);
			if (!m_bSDI)
			{
				CString mapN;
				mapN = mapName.Left(L_MAPN);
				if (getPopupMap(mapN) || sdi)
					actionCaption(key, IDX_SINGLE);
			}
		}
	}

	CRect rect;
	dwRc = GetPrivateProfileString("ACCN", "INFO", "", wb, sizeof(wb), file);


	dwRc = GetPrivateProfileString("GUIDE", "INFO", "", wb, sizeof(wb), file);

	if (dwRc <= 0 || wb[0] == '0')
		m_GuideDLG->ShowWindow(SW_HIDE);
	else
	{
		data  = wb;

		pos = data.Find('|');
		rect.left = atoi(data.Left(pos));
		data = data.Mid(pos + 1);

		pos = data.Find('|');
		rect.top = atoi(data.Left(pos));
		data = data.Mid(pos + 1);

		pos = data.Find('|');
		rect.right = atoi(data.Left(pos));
		rect.bottom = atoi(data.Mid(pos + 1));

		m_GuideDLG->SetWindowPos(&CWnd::wndTopMost, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
	}
}

void CMainFrame::makeUserScreenMenu()
{
	char	wb[512];
	int	count;
	CString	file, tmps, value;
	int ii = 0;

	tmps =GetUserName();
	file.Format("%s\\%s\\%s\\%s.ini", m_home, USRDIR, tmps, tmps); 
	
	CMenuXP*	mainM = (CMenuXP*) m_tMenu->GetPopupMenu(0);
	CMenuXP*	thisM = (CMenuXP*) mainM->GetSubMenu(UMENUN);	// fix menu position
	if (!thisM)	return;

	count = (int)thisM->GetMenuItemCount();
	for (ii = 0; ii < count - 2; ii++)
	{
		MENUITEMINFO	info;
		memset(&info, 0, sizeof(MENUITEMINFO));
		info.cbSize = sizeof(MENUITEMINFO);
		info.fMask = MIIM_DATA | MIIM_TYPE;
		thisM->GetMenuItemInfo(thisM->GetMenuItemID(2), &info);
		
		thisM->RemoveMenu(2, MF_BYPOSITION);
		CMenuXPItem *pData = (CMenuXPItem *)info.dwItemData;
		if ((info.fType & MFT_OWNERDRAW) && pData && pData->IsMyData())
			delete pData;
	}


	CMapStringToString	ary;
	for (ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		DWORD dwRc = GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), file);
		if (dwRc <= 0)
			break;

		tmps = wb;
		if (ary.Lookup(tmps, value))
			continue;
		ary.SetAt(tmps, "1");
		tmps.TrimLeft(); tmps.TrimRight();
		thisM->AppendMenuX(MF_STRING|MF_ENABLED, ID_MENU_USER+ii, tmps);
	}
	ary.RemoveAll();
}

bool CMainFrame::modifyWorkspace(bool add, CString keys, bool allvs)
{
	int	pos, key, index;
	CString	mpN, file, tmps, data, info;
	int ii = 0;

	tmps = GetUserName();
	file.Format("%s\\%s\\%s\\%s.tmp", m_home, USRDIR, tmps, tmps);
	if (add)
	{
		pos  = keys.Find('=');
		tmps = keys.Left(pos);
		keys = keys.Mid(pos+1);
		WritePrivateProfileString("ROOT", tmps, keys, file);

		WINDOWPLACEMENT	wndpl;
		CArray	<int, int>	aryK;

		wndpl.length = sizeof(WINDOWPLACEMENT);
		WritePrivateProfileSection(keys, "", file);
		
		CChildFrame*	child  = NULL;
		CSChild*	schild = NULL;
		for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
		{
			if (!allvs && vsN != m_vsN)	continue;

			for (ii = (int)m_hooklist[vsN].GetSize() - 1; 0 <= ii; ii--)
			{
				key = m_hooklist[vsN].GetAt(ii);
				aryK.Add(key);
			}

			for (index = 1, ii = 0; ii < aryK.GetSize(); ii++)
			{
				key = aryK.GetAt(ii);
				if (!m_arMDI[vsN].Lookup(key, child))	continue;

				mpN.Format("%s", child->m_mapN.Left(L_MAPN));
				if (!ExistMenu(mpN))	continue;

				data.Empty();
				child->GetWindowPlacement(&wndpl);
				getScreenData(child->m_key, data);
				
				// dll data append.........
				if (IsDll(child->m_mapN))
				{
					CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
					if (base)
					{
						char	wb[256];
						sprintf_s(wb, keys);
						base->SendMessage(WM_DLLDATA, 0, (long) &wb);
						if (!data.IsEmpty())	data += ";";
						tmps.Format("%s\t%s", USERDEFSYM, wb);
						data += tmps;
					}
				}

				bool	bShowlb = isShowLinkbar(mpN);
				tmps.Format("%d%02d", m_saveALLVS ? vsN : 0, index++);
				info.Format("%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|",
						mpN,
						0,
						wndpl.length,
						wndpl.flags,
						wndpl.showCmd,
						wndpl.ptMinPosition.x,
						wndpl.ptMinPosition.y,
						wndpl.ptMaxPosition.x,
						wndpl.ptMaxPosition.y,
						wndpl.rcNormalPosition.left,
						wndpl.rcNormalPosition.top,
						wndpl.rcNormalPosition.right,
						bShowlb ? wndpl.rcNormalPosition.bottom - LBAR_HEIGHT : wndpl.rcNormalPosition.bottom,
						child->m_xcaption.GetGroup(),
						data);
				WritePrivateProfileString(keys, tmps, info, file);
			}
			aryK.RemoveAll();

			for (ii = (int)m_sdiZORDER[vsN].GetSize() - 1; 0 <= ii; ii--)
			{
				key = m_sdiZORDER[vsN].GetAt(ii);
				aryK.Add(key);
			}
			
			for (ii = 0; ii < aryK.GetSize(); ii++)
			{
				key = aryK.GetAt(ii);
				if (!m_arSDI[vsN].Lookup(key, schild))	continue;
				
				mpN.Format("%s", schild->m_mapN.Left(L_MAPN));
				if (!ExistMenu(mpN))	continue;

				data.Empty();
				schild->GetWindowPlacement(&wndpl);
				getScreenData(schild->m_key, data);

				// dll data append.........
				if (IsDll(schild->m_mapN))
				{
					CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
					if (base)
					{
						char	wb[256];
						sprintf_s(wb, keys);
						base->SendMessage(WM_DLLDATA, 0, (long) &wb);
						if (!data.IsEmpty())	data += ";";
						tmps.Format("%s\t%s", USERDEFSYM, wb);
						data += tmps;
					}
				}

				bool	bShowlb = isShowLinkbar(mpN);
				tmps.Format("%d%02d", vsN, index++);
				info.Format("%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|",
						mpN,
						1,
						wndpl.length,
						wndpl.flags,
						wndpl.showCmd,
						wndpl.ptMinPosition.x,
						wndpl.ptMinPosition.y,
						wndpl.ptMaxPosition.x,
						wndpl.ptMaxPosition.y,
						wndpl.rcNormalPosition.left,
						wndpl.rcNormalPosition.top,
						wndpl.rcNormalPosition.right,
						bShowlb ? wndpl.rcNormalPosition.bottom - LBAR_HEIGHT : wndpl.rcNormalPosition.bottom,
						schild->m_xcaption.GetGroup(),
						data);
				WritePrivateProfileString(keys, tmps, info, file);
			}
			aryK.RemoveAll();
		}
		return true;
	}

	char	wb[512];
	CStringArray	arr;
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		if (!tmps.CompareNoCase(keys))
			continue;

		DWORD dwRc = GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), file);
		if (dwRc <= 0)
			break;

		tmps = wb;
		arr.Add(tmps);
	}

	WritePrivateProfileSection("ROOT", "", file);
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		tmps.Format("%02d", ii+1);
		WritePrivateProfileString("ROOT", tmps, arr.GetAt(ii), file);
	}

	arr.RemoveAll();
	return true;
}

void CMainFrame::save_user(CString sSave)
{
int	key, index;
	CString	mpN, file, tmps, data, info, keys = sSave;

	tmps = GetUserName();
	
	file.Format("%s\\%s\\%s\\%s.ini", m_home, USRDIR, tmps, sSave);
	WritePrivateProfileString("ROOT", "00", keys, file);

	WINDOWPLACEMENT	wndpl;
	CArray	<int, int>	aryK;

	wndpl.length = sizeof(WINDOWPLACEMENT);
	WritePrivateProfileSection(keys, "", file);
	
	CChildFrame*	child = NULL;
	CSChild*	schild = NULL;
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		int ii = 0;
		if (!m_saveALLVS && vsN != m_vsN)	continue;
		for (int value = (int)m_sdiZORDER[vsN].GetUpperBound(); value >= 0; value--)

		//for (int value = 0; value < m_sdiZORDER[vsN].GetSize(); value++)
		{
			key = m_sdiZORDER[vsN].GetAt(value);
			aryK.Add(key);
		}
		

		for (index = 1, ii = 0; ii < aryK.GetSize(); ii++)
		{
			key = aryK.GetAt(ii);
			if (!m_arSDI[vsN].Lookup(key, schild))	continue;
			
			mpN.Format("%s", schild->m_mapN.Left(L_MAPN));
			if (!ExistMenu(mpN))	continue;

			data.Empty();
			schild->GetWindowPlacement(&wndpl);
			getScreenData(schild->m_key, data);

			CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
			if (base)
			{
				char	wb[256];
				sprintf_s(wb, keys);
				base->SendMessage(WM_DLLDATA, 0, (long) &wb);
				if (!data.IsEmpty())	data += ";";
				tmps.Format("%s\t%s", USERDEFSYM, wb);
				data += tmps;
			}
			
			bool	bShowlb = isShowLinkbar(mpN);
			tmps.Format("%d%02d", vsN, index++);
			info.Format("%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%s",
					mpN,
					1,
					wndpl.length,
					wndpl.flags,
					wndpl.showCmd,
					wndpl.ptMinPosition.x,
					wndpl.ptMinPosition.y,
					wndpl.ptMaxPosition.x,
					wndpl.ptMaxPosition.y,
					wndpl.rcNormalPosition.left,
					wndpl.rcNormalPosition.top,
					wndpl.rcNormalPosition.right,
					bShowlb ? wndpl.rcNormalPosition.bottom - LBAR_HEIGHT : wndpl.rcNormalPosition.bottom,
					schild->m_xcaption.GetGroup(),
					data,
					schild->m_xcaption.GetFontSize(),
					schild->m_strSetConfig);
			WritePrivateProfileString(keys, tmps, info, file);
		}
		aryK.RemoveAll();

		for (ii = (int)m_hooklist[vsN].GetSize() - 1; 0 <= ii; ii--)
		{
			key = m_hooklist[vsN].GetAt(ii);
			aryK.Add(key);
		}

		for (ii = 0; ii < aryK.GetSize(); ii++)
		{
			key = aryK.GetAt(ii);
			if (!m_arMDI[vsN].Lookup(key, child))	continue;
			if (!child->GetSafeHwnd())		continue;

			mpN = child->m_mapN.Left(L_MAPN);
		
			data.Empty();
			child->GetWindowPlacement(&wndpl);
			getScreenData(child->m_key, data);

			CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
			if (base)
			{
				char	wb[256];
				sprintf_s(wb, keys);
				base->SendMessage(WM_DLLDATA, 0, (long) &wb);
				if (!data.IsEmpty())	data += ";";
				tmps.Format("%s\t%s", USERDEFSYM, wb);
				data += tmps;
			}

			bool	bShowlb = isShowLinkbar(mpN);
			tmps.Format("%d%02d", m_saveALLVS ? vsN : 0, index++);
			info.Format("%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%s",
					mpN,
					0,
					wndpl.length,
					wndpl.flags,
					wndpl.showCmd,
					wndpl.ptMinPosition.x,
					wndpl.ptMinPosition.y,
					wndpl.ptMaxPosition.x,
					wndpl.ptMaxPosition.y,
					wndpl.rcNormalPosition.left,
					wndpl.rcNormalPosition.top,
					wndpl.rcNormalPosition.right,
					bShowlb ? wndpl.rcNormalPosition.bottom - LBAR_HEIGHT : wndpl.rcNormalPosition.bottom,
					child->m_xcaption.GetGroup(),
					data,
					child->m_xcaption.GetFontSize(),
					child->m_strSetConfig);
			WritePrivateProfileString(keys, tmps, info, file);
		}
		aryK.RemoveAll();
	}
}

void CMainFrame::save_laststat()
{
	int	key, index;
	CString user = GetUserName();
	CString	mpN, file, tmps, data, info, keys = "LASTSTAT";	
	WritePrivateProfileString("ROOT", "00", keys, file);

	WINDOWPLACEMENT	wndpl;
	CArray	<int, int>	aryK;

	wndpl.length = sizeof(WINDOWPLACEMENT);
	WritePrivateProfileSection(keys, "", file);
	
	CChildFrame*	child = NULL;
	CSChild*	schild = NULL;
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		int ii = 0;
		if (!m_saveALLVS && vsN != m_vsN)	continue;


		for (int value = (int)m_sdiZORDER[vsN].GetUpperBound(); value >= 0; value--)
		{
			key = m_sdiZORDER[vsN].GetAt(value);
			aryK.Add(key);
		}
		
		for (index = 1, ii = 0; ii < aryK.GetSize(); ii++)
		{
			key = aryK.GetAt(ii);
			if (!m_arSDI[vsN].Lookup(key, schild))	continue;
			
			mpN.Format("%s", schild->m_mapN.Left(L_MAPN));
			if(mpN == BONDMAP) continue;  //관심설정은 저장 안함
			if(mpN == GONGJIMAP) continue;  //공지화면은 저장 안함
			if(mpN == ALARMMAP) continue; //공지알람화면은 저장안함

			if (!ExistMenu(mpN))	continue;

			data.Empty();
			schild->GetWindowPlacement(&wndpl);
			getScreenData(schild->m_key, data);

			CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
			if (base)
			{
				char	wb[256];
				sprintf_s(wb, keys);
				base->SendMessage(WM_DLLDATA, 0, (long) &wb);
				if (!data.IsEmpty())	data += ";";
				tmps.Format("%s\t%s", USERDEFSYM, wb);
				data += tmps;
			}
			
			file.Format("%s\\%s\\%s\\%sSDI.ini", m_home, USRDIR, user, user);
			bool	bShowlb = isShowLinkbar(mpN);
			tmps.Format("%d%02d", vsN, index++);
			info.Format("%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%s",
					mpN,
					1,
					wndpl.length,
					wndpl.flags,
					wndpl.showCmd,
					wndpl.ptMinPosition.x,
					wndpl.ptMinPosition.y,
					wndpl.ptMaxPosition.x,
					wndpl.ptMaxPosition.y,
					wndpl.rcNormalPosition.left,
					wndpl.rcNormalPosition.top,
					wndpl.rcNormalPosition.left + schild->m_width,
					wndpl.rcNormalPosition.top + schild->m_height,
				//	wndpl.rcNormalPosition.right,
				//	bShowlb ? wndpl.rcNormalPosition.bottom - LBAR_HEIGHT : wndpl.rcNormalPosition.bottom,
					schild->m_xcaption.GetGroup(),
					data,
					schild->m_xcaption.GetFontSize(),
					schild->m_strSetConfig);
			WritePrivateProfileString(keys, tmps, info, file);
		}
		aryK.RemoveAll();
		
		for (ii = (int)m_hooklist[vsN].GetSize() - 1; 0 <= ii; ii--)
		{
			key = m_hooklist[vsN].GetAt(ii);
			aryK.Add(key);
		}

		for (ii = 0; ii < aryK.GetSize(); ii++)
		{
			key = aryK.GetAt(ii);
			if (!m_arMDI[vsN].Lookup(key, child))	continue;
			if (!child->GetSafeHwnd())		continue;

			mpN = child->m_mapN.Left(L_MAPN);

			if(mpN == BONDMAP) continue;  //관심설정은 저장 안함
			if(mpN == GONGJIMAP) continue;  //공지화면은 저장 안함
			if(mpN == ALARMMAP) continue; //공지알람화면은 저장안함
		
			data.Empty();
			child->GetWindowPlacement(&wndpl);
			getScreenData(child->m_key, data);

			CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
			if (base)
			{
				char	wb[256];
				sprintf_s(wb, keys);
				base->SendMessage(WM_DLLDATA, 0, (long) &wb);
				if (!data.IsEmpty())	data += ";";
				tmps.Format("%s\t%s", USERDEFSYM, wb);
				data += tmps;
			}

			bool bmini = child->m_bIconic;
			WINDOWPLACEMENT pl;	
			GetWindowPlacement(&pl);

			bool bShowlb = isShowLinkbar(mpN);
			file.Format("%s\\%s\\%s\\%sMDI.ini", m_home, USRDIR, user, user);
			tmps.Format("%d%02d", m_saveALLVS ? vsN : 0, index++);
			info.Format("%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%s",
					mpN,
					0,
					wndpl.length,
					wndpl.flags,
					wndpl.showCmd,
					wndpl.ptMinPosition.x,
					wndpl.ptMinPosition.y,
					wndpl.ptMaxPosition.x,
					wndpl.ptMaxPosition.y,
					wndpl.rcNormalPosition.left,
					//wndpl.rcNormalPosition.top,
					bmini == true ? pl.rcNormalPosition.top + 100 : wndpl.rcNormalPosition.top,
					//wndpl.rcNormalPosition.right,
					wndpl.rcNormalPosition.left + child->m_width,
					//bShowlb ? wndpl.rcNormalPosition.bottom - LBAR_HEIGHT : wndpl.rcNormalPosition.bottom,
					//wndpl.rcNormalPosition.top + child->m_height,
					bmini == true ? pl.rcNormalPosition.top + 100 + child->m_height : wndpl.rcNormalPosition.top + child->m_height,
					child->m_xcaption.GetGroup(),
					data,
					child->m_xcaption.GetFontSize(),
					child->m_strSetConfig);
			WritePrivateProfileString(keys, tmps, info, file);
		}
		aryK.RemoveAll();
	}


	if(m_GuideDLG->IsWindowVisible())
	{
		m_GuideDLG->GetWindowPlacement(&wndpl);
		tmps.Format("%d|%d|%d|%d",	wndpl.rcNormalPosition.left,
						wndpl.rcNormalPosition.top,
						wndpl.rcNormalPosition.right,
						wndpl.rcNormalPosition.bottom);

		WritePrivateProfileString("GUIDE","INFO", tmps, file);
	}
	else
		WritePrivateProfileString("GUIDE","INFO", "0", file);
}

void CMainFrame::getScreenData(int key, CString& data, BOOL save)
{
	CString		file, value, tmps, symbol = "CODX, CODJ, 1601_ONE, 1601_TWO, 1601_THR, 1601_FOR, 0116";
	char		wb[512];
	int ii = 0;

	file.Format("%s\\%s\\axis.ini", m_home, TABDIR);
	GetPrivateProfileString("LinkEx", "codesymbols", symbol, wb, sizeof(wb), file);
	symbol = wb;

	CStringArray		ary;
	CMapStringToString	arSymbyData;
	
	parseAry(symbol, ary, ",");
	
	LONG_PTR	rc = 0;
	for (ii = 0; ii < ary.GetSize(); ii++)
	{
		symbol = ary.GetAt(ii);	
		m_wizard->getTrigger(key, symbol);
		arSymbyData.SetAt(symbol, value);
	}

	if (save)
	{
		for (ii = 0; ii < ary.GetSize(); ii++)
		{
			symbol = ary.GetAt(ii);
			arSymbyData.Lookup(symbol, value);
			value.TrimLeft();value.TrimRight();

			if (value.IsEmpty())	continue;
			tmps.Format("%s\t%s", symbol, value);
			if (!data.IsEmpty())
				data += ";";
			data += tmps;
		}
	}
	else	// link datas
	{
		CMapStringToString	arSymbylink;
		CStringArray		arsym;
		CString			sym;
		for (ii = 0; ii < ary.GetSize(); ii++) 
		{
			symbol = ary.GetAt(ii);
			arSymbyData.Lookup(symbol, value);
			value.TrimLeft();value.TrimRight();

			if (value.IsEmpty())	continue;
			arSymbylink.SetAt(symbol, value);
			if (GetPrivateProfileString("Link", symbol, "", wb, sizeof(wb), file))
			{
				tmps = wb;
				parseAry(tmps, arsym, ",");
				for (int jj = 0; jj < arsym.GetSize(); jj++)
				{
					sym = arsym.GetAt(jj);
					if (arSymbylink.Lookup(sym, tmps))
						continue;
					arSymbylink.SetAt(sym, value);
				}
				arsym.RemoveAll();
			}
		}

		for (ii = 0; ii < ary.GetSize(); ii++)
		{
			symbol = ary.GetAt(ii);
			if (!arSymbylink.Lookup(symbol, value))
				continue;

			if (value.IsEmpty())	continue;
			tmps.Format("%s\t%s", symbol, value);
			if (!data.IsEmpty())
				data += ";";
			data += tmps;
		}

		arSymbylink.RemoveAll();
	}
	ary.RemoveAll();
}

BOOL CMainFrame::getPopupMap(CString name)
{
	CString		file, value, tmps, mapName;
	char		wb[512];

	file.Format("%s\\%s\\axis.ini", m_home, TABDIR);
	GetPrivateProfileString("LinkMap", "popup ", mapName, wb, sizeof(wb), file);
	mapName = wb;

	CStringArray		ary;
	
	parseAry(mapName, ary, ",");
	
	long	rc = 0;
	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		mapName = ary.GetAt(ii);
		if (name == mapName)
		{
			ary.RemoveAll();
			return TRUE;
		}
	}
	
	ary.RemoveAll();
	return FALSE;
}

void CMainFrame::saveImg(int entire)
{
	PostMessage(WM_CANCELMODE);
	PostMessage(WM_AXIS, MAKEWPARAM(axWRITEIMG, entire));
	//writeImg(m_home);
}

void CMainFrame::saveClip(int entire)
{
		PostMessage(WM_CANCELMODE);
		PostMessage(WM_AXIS, MAKEWPARAM(axCLIP, entire));
}

enum Win32Type { Win32s, WinNT3, Win95, Win98, WinME, WinNT4, Win2000, WinXP };
Win32Type IsShellType()
{
	Win32Type  ShellType;
	DWORD winVer;
	OSVERSIONINFO *osvi;
	
	winVer=GetVersion();
	if(winVer<0x80000000){/*NT */
		ShellType=WinNT3;
		osvi= (OSVERSIONINFO *)malloc(sizeof(OSVERSIONINFO));
		if (osvi!=NULL){
			memset(osvi,0,sizeof(OSVERSIONINFO));
			osvi->dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			GetVersionEx(osvi);
			if(osvi->dwMajorVersion==4L)ShellType=WinNT4;
			else if(osvi->dwMajorVersion==5L&&osvi->dwMinorVersion==0L)ShellType=Win2000;
			else if(osvi->dwMajorVersion==5L&&osvi->dwMinorVersion==1L)ShellType=WinXP;
			free(osvi);
		}
	}
	else if  (LOBYTE(LOWORD(winVer)) < 4)
		ShellType=Win32s;
	else{
		ShellType=Win95;
		osvi= (OSVERSIONINFO *)malloc(sizeof(OSVERSIONINFO));
		if (osvi!=NULL){
			memset(osvi,0,sizeof(OSVERSIONINFO));
			osvi->dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			GetVersionEx(osvi);
			if(osvi->dwMajorVersion==4L&&osvi->dwMinorVersion==10L)ShellType=Win98;
			else if(osvi->dwMajorVersion==4L&&osvi->dwMinorVersion==90L)ShellType=WinME;
			free(osvi);
		}
	}
	return ShellType;
}

#if 1
BOOL CMainFrame::printImg(CString dat)
{
	CChildFrame* pChild = (CChildFrame*) MDIGetActive();
	if (!pChild)
	{
		::MessageBox(m_hWnd, "인쇄할 화면이 없습니다.", "확인", MB_ICONINFORMATION);
		return TRUE;
	}

	CString		file, tmps;
	CBitmap 	bitmap;
	CWindowDC	dc(pChild);
	CDC 		memDC;
	CRect		rect;
	bool		bWIN98 = false; 

	if (IsShellType() == Win98)
	{
		UINT set98 = GetProfileInt("Information", "win98", 0);
		if (set98)	bWIN98 = true;
	}

	memDC.CreateCompatibleDC(&dc); 
	pChild->GetWindowRect(&rect);

	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY); 

	// Create logical palette if device support a palette
	CPalette pal;
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);

		// Create the palette
		pal.CreatePalette(pLP);

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// Convert the bitmap to a DIB
	HANDLE hDIB = writeDDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)	return FALSE;

	if (bWIN98)
		file.Format("%s\\print.bmp", m_home);
	else	file.Format("%s\\%d", m_home, (int)pChild);

	writeDIB((LPSTR)(LPCTSTR)file, hDIB);
	GlobalFree(hDIB);

	if (bWIN98)
	{
		ShellExecute(NULL, _T("print"), file, NULL,NULL, SW_SHOW);
	}
	else
	{
		CNDib*	nDib = new CNDib();
		if (nDib->SetNDIB(file))
		{
			DeleteFile(file);
			delete nDib;
			return FALSE;
		}

		HANDLE	pDib = nDib->GetNDIB();
		BITMAP	bm;
		HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
					IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);

		CDC	printDC, *pDC = pChild->GetDC();

		m_printsetup->SetThisPrinter();
		AfxGetApp()->CreatePrinterDC(printDC);
		if (!printDC)
		{
			DeleteFile(file);
			ReleaseDC(pDC);
			delete nDib;
			return FALSE;
		}
		
		SetMapMode(printDC.m_hDC, MM_TEXT);
		DOCINFO docinfo = { sizeof(DOCINFO), "화면인쇄", NULL};
 		if(!StartDoc(printDC.m_hDC, &docinfo))
		{
			DeleteFile(file);
			printDC.DeleteDC();
			ReleaseDC(pDC);
			delete nDib;
			return FALSE;
		}

		int	paperX = GetDeviceCaps(printDC.m_hDC, HORZRES);
		int	paperY = GetDeviceCaps(printDC.m_hDC, VERTRES);

		int	printX = GetDeviceCaps(printDC.m_hDC, LOGPIXELSX);
		int	printY = GetDeviceCaps(printDC.m_hDC, LOGPIXELSY);
		int	deviceX = GetDeviceCaps(pDC->m_hDC, LOGPIXELSX);
		int	deviceY = GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);
		
		int	width  = (int) (bm.bmWidth*printX/deviceX);
		int	height = (int) (bm.bmHeight*printY/deviceY);
		int	pagexN = (int) ((width+paperX-1)/paperX);
		int	pageyN = (int) ((height+paperY-1)/paperY);

		int	xDest, yDest, nDestW, nDestH;
		int	xSrc, ySrc, nSrcW, nSrcH;

		::SetStretchBltMode(printDC.m_hDC, COLORONCOLOR);

		LPBITMAPINFOHEADER lpbi;
		LPSTR        pBuf;
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(pDib);
		if (!lpbi)
		{
			DeleteFile(file);
			printDC.DeleteDC();
			ReleaseDC(pDC);
			delete nDib;
			return FALSE;
		}
		pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + nDib->PaletteSize(lpbi);

		CFont	Font, *font;

		int	fontS = 90*printX/deviceX;
		Font.CreatePointFont(fontS, "굴림");
		font = printDC.SelectObject(&Font);
		int	ret;
		if (pagexN == 1 && pageyN == 1)
		{
			if(StartPage(printDC.m_hDC))
			{
				xDest = paperX/2 - width/2;
				yDest = paperY/2 - height/2;

				ret = StretchDIBits(printDC.m_hDC, xDest, yDest, width ,height, 0, 0, 
					bm.bmWidth, bm.bmHeight, pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
				if (!dat.IsEmpty())
					TextOut(printDC.m_hDC, xDest, yDest+height+20, dat, dat.GetLength());
				EndPage(printDC.m_hDC);
			}
		}
		else
		{
			xDest = 0;	yDest = 0;
			for (int ii = 0; ii < pageyN; ii++)
			{
				for (int jj = 0; jj < pagexN; jj++)
				{
					if(!StartPage(printDC.m_hDC))
						break;
					if (jj == pagexN - 1)
					{
						nDestW = width - paperX*jj;
						nSrcW = bm.bmWidth - ((int)(deviceX*paperX/printX))*jj;
						if (ii == pageyN - 1)
						{	// 2 of 2(hori), 2 of 2(vert), 4 of 4
							nDestH = height - paperY*ii;
							xSrc = (int)(deviceX*paperX/printX)*jj;
							ySrc = 0;
							nSrcH = bm.bmHeight - ((int)(deviceY*paperY/printY))*ii;
						}
						else
						{	// 1 of 2(vert), 2 of 4
							nDestH = paperY;
							xSrc = (int)(deviceX*paperX/printX)*jj;
							ySrc = bm.bmHeight - (int)(deviceY*paperY/printY)*(ii+1);
							nSrcH = (int)(deviceY*paperY/printY);
						}
						ret = StretchDIBits(printDC.m_hDC, xDest, yDest,
							nDestW, nDestH, xSrc, ySrc, nSrcW, nSrcH,
							pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
					}
					else
					{
						if (ii == pageyN - 1)
						{	// 1 of 2(hori), 3 of 4
							nDestW = paperX;
							nDestH = height - paperY*ii;
							xSrc = (int)(deviceX*paperX/printX)*jj;
							ySrc = 0;
							nSrcW = ((int)(deviceX*paperX/printX));
							nSrcH = bm.bmHeight - ((int)(deviceY*paperY/printY))*ii;
						}
						else
						{	// 1 of 4
							nDestW = paperX;
							nDestH = paperY;
							xSrc = (int)(deviceX*paperX/printX)*jj;
							ySrc = bm.bmHeight - (int)(deviceY*paperY/printY)*(ii+1);
							nSrcW = ((int)(deviceX*paperX/printX));
							nSrcH = (int)(deviceY*paperY/printY);
						}
						ret = StretchDIBits(printDC.m_hDC, xDest, yDest, 
							nDestW, nDestH, xSrc, ySrc, nSrcW, nSrcH,
							pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
					}
					EndPage(printDC.m_hDC);
				}
			}
		}
		EndDoc(printDC.m_hDC);
		m_printsetup->RestorePrinter();

		
		printDC.SelectObject(font);
		DeleteObject(Font);
		DeleteObject(hBitmap);
		printDC.DeleteDC();
		ReleaseDC(pDC);
		delete nDib;
		DeleteFile(file);
	}
	return TRUE;
}
#else
BOOL CMainFrame::printImg(CString dat)
{
	CWnd*	pChild;
	CString	title, fontName;

	title.LoadString(ST_TEXT_OK);
	CChildFrame* child = (CChildFrame*) MDIGetActive();
	if (!child)
	{
		::MessageBox(m_hWnd, "No screen for print", title, MB_ICONINFORMATION);
		return TRUE;
	}
	pChild = child;

	for (int ii = 0; ii < 100; ii++)
	{
		Sleep(0);
		Sleep(30);
	}

	CString		file;
	CBitmap 	bitmap;
	CWindowDC	dc(pChild);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc); 
	pChild->GetWindowRect(&rect);
	CRect	mRc, rc;

	rc.CopyRect(&rect);
	m_MClient->GetWindowRect(mRc);
	rect.IntersectRect (mRc, rect);

	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY); 

	// Create logical palette if device support a palette
	CPalette pal;
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);

		// Create the palette
		pal.CreatePalette(pLP);

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// Convert the bitmap to a DIB
	HANDLE hDIB = writeDDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)	return FALSE;

	file.Format("%s\\%d", m_home, (int)pChild);
	writeDIB((LPSTR)(LPCTSTR)file, hDIB);
	GlobalFree(hDIB);

	CNDib*	nDib = new CNDib();
	if (nDib->SetNDIB(file))
	{
		DeleteFile(file);
		delete nDib;
		::MessageBox(m_hWnd, "Error print", title, MB_ICONINFORMATION);
		return FALSE;
	}

	HANDLE	pDib = nDib->GetNDIB();
	BITMAP	bm;
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
				IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);


	CDC	printDC, *pDC = pChild->GetDC();

	m_printsetup->SetThisPrinter();
	AfxGetApp()->CreatePrinterDC(printDC);
	if (!printDC)
	{
		DeleteFile(file);
		ReleaseDC(pDC);
		delete nDib;
		::MessageBox(m_hWnd, "Error print", title, MB_ICONINFORMATION);
		return FALSE;
	}
	
	SetMapMode(printDC.m_hDC, MM_TEXT);
	DOCINFO docinfo = { sizeof(DOCINFO), "Print screen", NULL};
 	if(!StartDoc(printDC.m_hDC, &docinfo))
	{
		DeleteFile(file);
		printDC.DeleteDC();
		ReleaseDC(pDC);
		delete nDib;
		::MessageBox(m_hWnd, "Error print", title, MB_ICONINFORMATION);
		return FALSE;
	}

	int	paperX = GetDeviceCaps(printDC.m_hDC, HORZRES);
	int	paperY = GetDeviceCaps(printDC.m_hDC, VERTRES);

	int	printX = GetDeviceCaps(printDC.m_hDC, LOGPIXELSX);
	int	printY = GetDeviceCaps(printDC.m_hDC, LOGPIXELSY);
	int	deviceX = GetDeviceCaps(pDC->m_hDC, LOGPIXELSX);
	int	deviceY = GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);
	
	int	width  = (int) (bm.bmWidth*printX/deviceX);
	int	height = (int) (bm.bmHeight*printY/deviceY);
	int	pagexN = (int) ((width+paperX-1)/paperX);
	int	pageyN = (int) ((height+paperY-1)/paperY);

	int	xDest, yDest, nDestW, nDestH;
	int	xSrc, ySrc, nSrcW, nSrcH;

	::SetStretchBltMode(printDC.m_hDC, COLORONCOLOR);

	LPBITMAPINFOHEADER lpbi;
	LPSTR        pBuf;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(pDib);
	if (!lpbi)
	{
		DeleteFile(file);
		printDC.DeleteDC();
		ReleaseDC(pDC);
		delete nDib;
		::MessageBox(m_hWnd, "Error print", title, MB_ICONINFORMATION);
		return FALSE;
	}
	pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + nDib->PaletteSize(lpbi);

	CFont	Font, *font;
	fontName.LoadString(ST_FONTNAME_DEFAULT);
	int	fontS = 90*printX/deviceX;
	Font.CreatePointFont(fontS, fontName);
	font = printDC.SelectObject(&Font);
	int	ret;
	if (pagexN == 1 && pageyN == 1)
	{
		if(StartPage(printDC.m_hDC))
		{
			xDest = paperX/2 - width/2;
			yDest = paperY/2 - height/2;
			ret = StretchDIBits(printDC.m_hDC, xDest, yDest, width ,height, 0, 0, 
				bm.bmWidth, bm.bmHeight, pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
			if (!dat.IsEmpty())
				TextOut(printDC.m_hDC, xDest, yDest+height+20, dat, dat.GetLength());
			EndPage(printDC.m_hDC);
		}
	}
	else
	{
		xDest = 0;	yDest = 0;
		for (int ii = 0; ii < pageyN; ii++)
		{
			for (int jj = 0; jj < pagexN; jj++)
			{
				if(!StartPage(printDC.m_hDC))
					break;
				if (jj == pagexN - 1)
				{
					nDestW = width - paperX*jj;
					nSrcW = bm.bmWidth - ((int)(deviceX*paperX/printX))*jj;
					if (ii == pageyN - 1)
					{	// 2 of 2(hori), 2 of 2(vert), 4 of 4
						nDestH = height - paperY*ii;
						xSrc = (int)(deviceX*paperX/printX)*jj;
						ySrc = 0;
						nSrcH = bm.bmHeight - ((int)(deviceY*paperY/printY))*ii;
					}
					else
					{	// 1 of 2(vert), 2 of 4
						nDestH = paperY;
						xSrc = (int)(deviceX*paperX/printX)*jj;
						ySrc = bm.bmHeight - (int)(deviceY*paperY/printY)*(ii+1);
						nSrcH = (int)(deviceY*paperY/printY);
					}
					ret = StretchDIBits(printDC.m_hDC, xDest, yDest,
						nDestW, nDestH, xSrc, ySrc, nSrcW, nSrcH,
						pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
				}
				else
				{
					if (ii == pageyN - 1)
					{	// 1 of 2(hori), 3 of 4
						nDestW = paperX;
						nDestH = height - paperY*ii;
						xSrc = (int)(deviceX*paperX/printX)*jj;
						ySrc = 0;
						nSrcW = ((int)(deviceX*paperX/printX));
						nSrcH = bm.bmHeight - ((int)(deviceY*paperY/printY))*ii;
					}
					else
					{	// 1 of 4
						nDestW = paperX;
						nDestH = paperY;
						xSrc = (int)(deviceX*paperX/printX)*jj;
						ySrc = bm.bmHeight - (int)(deviceY*paperY/printY)*(ii+1);
						nSrcW = ((int)(deviceX*paperX/printX));
						nSrcH = (int)(deviceY*paperY/printY);
					}
					ret = StretchDIBits(printDC.m_hDC, xDest, yDest, 
						nDestW, nDestH, xSrc, ySrc, nSrcW, nSrcH,
						pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
				}
				EndPage(printDC.m_hDC);
			}
		}
	}
	EndDoc(printDC.m_hDC);
	m_printsetup->RestorePrinter();

	
	printDC.SelectObject(font);
	DeleteObject(Font);
	DeleteObject(hBitmap);
	DeleteFile(file);
	printDC.DeleteDC();
	ReleaseDC(pDC);
	delete nDib;
	return TRUE;
}
#endif
BOOL CMainFrame::writeImg(CString dir, int entire, BOOL bClip)
{
	/*
	CWnd* pChild = NULL;
	CString mapN;

	if(entire)
		pChild = this;
	else
	{
		CChildFrame* child = (CChildFrame*)MDIGetActive();
		if(!child || child->IsWindowVisible()  == FALSE)
		{
			::MessageBox(m_hWnd, "실행중인 화면이 없습니다", "AXIS", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		mapN = child->m_mapN;
		ASSERT(mapN.GetLength() >= L_MAPN);
		pChild = child;
	}

	if(!pChild)
	{
		::MessageBox(m_hWnd, "실행중인 화면이 없습니다", "AXIS", MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	for (int ii = 0; ii < 10; ii++)	
		Sleep(10);

	CString file;
	CBitmap bitmap;
	CWindowDC dc(pChild);
	CDC memDC;
	CRect rect;
	CBitmap* pOldBitmap;

	memDC.CreateCompatibleDC(&dc);
	pChild->GetWindowRect(&rect);

	if(!entire)
	{
		CRect mRc, rc;
		rc.CopyRect(&rect);
		m_MClient->GetWindowRect(mRc);
		rect.IntersectRect(mRc, rect);
	}

	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

	CPalette pal;
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);

		// Create the palette
		pal.CreatePalette(pLP);

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	if (bClip)
	{
		OpenClipboard();
		EmptyClipboard();
		
		if ( ::SetClipboardData(CF_BITMAP, bitmap) == NULL )
		{
			AfxMessageBox("Unable to set Clipboard data");
			CloseClipboard();
			return FALSE;
		}
		else
		{
			CloseClipboard();
			displayGuide("클립보드에 저장되었습니다");
			return TRUE;
		}
	}

	// Convert the bitmap to a DIB
	HANDLE hDIB = writeDDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)	
	{
		::MessageBox(m_hWnd, "bitmap to DIB Failed.", "AXIS", MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	file.Format("%s\\%s.bmp", dir, entire ? "Full_Screen" : m_tMenu->GetDesc(mapN.Left(L_MAPN)));
	CFileDialog	dlg(FALSE, "bmp", file, OFN_OVERWRITEPROMPT, 
		"BMP Files(*.bmp)|*.bmp||", NULL);

	if (dlg.DoModal() == IDOK)
	{
		file = dlg.GetPathName();
		if (dlg.GetFileName().IsEmpty())
		{
			GlobalFree(hDIB);
			return FALSE;	
		}
	}
	else
	{
		GlobalFree(hDIB);
		return FALSE;
	}

	//szFile = (LPSTR)(LPCTSTR)path;
	// Write it to file
	writeDIB((LPSTR)(LPCTSTR)file, hDIB);

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree(hDIB);


	return TRUE;
	*/
	
	CWnd*	pChild = NULL;
	CString	mapN;

	if (entire)	
		pChild = this;
	else
	{
		CChildFrame* child = (CChildFrame*) MDIGetActive();
		if (!child)	mapN = child->m_mapN;
		pChild = child;
	}

	if (!pChild)	
		return false;

	for (int ii = 0; ii < 10; ii++)	Sleep(10);
	CString		file;
	CBitmap 	bitmap;
	CWindowDC	dc(pChild);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc); 
	pChild->GetWindowRect(&rect);
	if (!entire)
	{
		CRect	mRc, rc;

		rc.CopyRect(&rect);
		m_MClient->GetWindowRect(mRc);
		rect.IntersectRect (mRc, rect);
	}

	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY); 

	// Create logical palette if device support a palette
	CPalette pal;
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = 
			GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);

		// Create the palette
		pal.CreatePalette(pLP);

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	if (bClip)
	{
		OpenClipboard();
		EmptyClipboard();
		
		if ( ::SetClipboardData(CF_BITMAP, bitmap) == NULL )
		{
			AfxMessageBox("Unable to set Clipboard data");
			CloseClipboard();
			return FALSE;
		}
		else
		{
			CloseClipboard();
			displayGuide("클립보드에 저장되었습니다");
			return TRUE;
		}
	}

	// Convert the bitmap to a DIB
	HANDLE hDIB = writeDDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)	return FALSE;

	file.Format("%s\\%s.bmp", dir, entire ? "Full_Screen" : m_tMenu->GetDesc(mapN.Left(L_MAPN)));
	CFileDialog	dlg(FALSE, "bmp", file, OFN_OVERWRITEPROMPT, "BMP Files(*.bmp)|*.bmp||", NULL);

	if (dlg.DoModal() == IDOK)
	{
		file = dlg.GetPathName();
		if (dlg.GetFileName().IsEmpty())
		{
			GlobalFree(hDIB);
			return FALSE;	
		}
	}
	else
	{
		GlobalFree(hDIB);
		return FALSE;
	}

	//szFile = (LPSTR)(LPCTSTR)path;
	// Write it to file
	writeDIB((LPSTR)(LPCTSTR)file, hDIB);

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree(hDIB);
	return TRUE;
}

HANDLE CMainFrame::writeDDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;

	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if (dwCompression == BI_BITFIELDS)	return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal == NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = int(pow((double)2, (double)(bi.biBitCount)));
	if (nColors > 256)
		nColors = 0;

	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED, dwLen);
	if (!hDIB)
	{
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;
	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;
		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;

	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
	{
		hDIB = handle;
	}
	else
	{
		GlobalFree(hDIB);
		SelectPalette(hDC, hPal, FALSE);	// Reselect the original palette
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if (!bGotBits)
	{
		GlobalFree(hDIB);
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	SelectPalette(hDC, hPal, FALSE);
	::ReleaseDC(NULL, hDC);

	return hDIB;
}

// WriteDIB		- Writes a DIB to file
// Returns		- TRUE on success
// szFile		- Name of file to write to
// hDIB			- Handle of the DIB
BOOL CMainFrame::writeDIB(LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	CFile file;
	if(!file.Open(szFile, CFile::modeWrite|CFile::modeCreate))
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	int nColors = int(pow((double)2, (double)lpbi->biBitCount));
	if (nColors > 256)
		nColors = 0;

	// Fill in the fields of the file header 
	hdr.bfType	= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize	= (int)GlobalSize (hDIB) + sizeof(hdr);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits	= (DWORD) (sizeof(hdr) + lpbi->biSize +	nColors * sizeof(RGBQUAD));

	// Write the file header 
	file.Write(&hdr, sizeof(hdr));

	// Write the DIB header and the bits 
	file.Write(lpbi, (int)GlobalSize(hDIB));
	file.Close();
	return TRUE;
}

LPCTSTR	CMainFrame::getPrintName()
{
	PRINTDLG pd;
	LPCTSTR	name;
	CWinApp* app = AfxGetApp();

	pd.lStructSize = (DWORD) sizeof(PRINTDLG);
	if (!app->GetPrinterDeviceDefaults(&pd))
		return NULL;
	LPDEVMODE pDevMode = (LPDEVMODE)::GlobalLock(pd.hDevMode); 
	if (NULL == pDevMode) 
	{
		::GlobalUnlock(pd.hDevMode);
		return NULL;
	}
	name = (LPCTSTR)pDevMode->dmDeviceName;
	::GlobalUnlock(pd.hDevMode);
	return name;
}

LPDEVMODE CMainFrame::getPrintMode()
{
	PRINTDLG pd;
	CWinApp* app = AfxGetApp();

	pd.lStructSize = (DWORD) sizeof(PRINTDLG);
	if (!app->GetPrinterDeviceDefaults(&pd))
		return NULL;
	LPDEVMODE pDevMode = (LPDEVMODE)::GlobalLock(pd.hDevMode); 
	if (NULL == pDevMode)
	{
		::GlobalUnlock(pd.hDevMode);
		return NULL;
	}
	::GlobalUnlock(pd.hDevMode);
	return pDevMode;
}

void CMainFrame::setUDP()
{
	char		wb[128];
	int		value;
	CString		tmps = _T("");

	getConnectInfo(tmps, value);
	value = 15200;
	strcpy_s(wb, tmps);
	m_wizard->setUDP(wb, value);
}

void CMainFrame::beginWait(int key)
{
	int	waitkey;
	for (int ii = 0; ii < m_waitlist.GetSize(); ii++)
	{
		waitkey = m_waitlist.GetAt(ii);
		if (waitkey == key)
			m_waitlist.RemoveAt(ii--);
	}

	if (key & 0x80)
	{
		CMPop*	Mpop;
		CGPop*	Gpop;
		if (m_arMPOP.Lookup(key, Mpop))		Mpop->m_cursor = 1;
		else if (m_arGPOP.Lookup(key, Gpop))	Gpop->m_cursor = 1;
	}
	else
	{
		CChildFrame*	child;
		if (!m_arMDI[m_vsN].Lookup(key, child))
		{
			CSChild*	schild;
			if (!m_arSDI[m_vsN].Lookup(key, schild))
				return;
			schild->m_cursor = 1;
		}
		else	child->m_cursor = 1;
	}

	m_waitlist.Add(key);
	if (key == m_activeKey)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

void CMainFrame::endWait(int key)
{
	int	endkey;
	for (int ii = 0; ii < m_waitlist.GetSize(); ii++)
	{
		endkey = m_waitlist.GetAt(ii);
		if (endkey == key)
			m_waitlist.RemoveAt(ii--);
	}

	if (key & 0x80)
	{
		CMPop*	Mpop;
		CGPop*	Gpop;
		if (m_arMPOP.Lookup(key, Mpop))		Mpop->m_cursor = 0;
		else if (m_arGPOP.Lookup(key, Gpop))	Gpop->m_cursor = 0;
	}
	else
	{
		CChildFrame*	child;
		if (!m_arMDI[m_vsN].Lookup(key, child))
		{
			CSChild*	schild;
			if (!m_arSDI[m_vsN].Lookup(key, schild))
				return;
			schild->m_cursor = 0;
		}
		else	child->m_cursor = 0;
	}

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CMainFrame::removeWait(int key)
{
	int	removekey;
	for (int ii = 0; ii < m_waitlist.GetSize(); ii++)
	{
		removekey = m_waitlist.GetAt(ii);
		if (removekey == key)
			m_waitlist.RemoveAt(ii--);
	}
}

bool CMainFrame::IsSelectMap(CString mapN)
{
	CString smapN, file, key = mapN.Mid(2, 4);
	char	buf[256];

	file.Format("%s\\%s\\axis.ini", m_home, TABDIR);
	GetPrivateProfileString("Select", key, "", buf, sizeof(buf), file);

	smapN = buf;
	if (smapN.IsEmpty())
		return false;
	return true;
}

CString CMainFrame::getSMap(CString mapN)
{
	if (mapN.IsEmpty())	return _T("");
	CString smapN, file, key = m_tMenu->GetDispN(mapN);
	char	buf[256];

	file.Format("%s\\%s\\axis.ini", m_home, TABDIR);
	GetPrivateProfileString("Select", key, "", buf, sizeof(buf), file);

	smapN = buf;
	if (smapN.IsEmpty())	return mapN;
	return mapN.Left(2) + smapN;
}

void CMainFrame::loadDomino(int key, CString mapN, bool bSDI)
{
	if (bSDI)
	{
		CSChild*	schild;
		if (!m_arSDI[m_vsN].Lookup(key, schild))	return;

		int	size, vwTYPE;
		vwTYPE = m_wizard->getFormInfo(getSMap(mapN), size);


		CRect	rect;
		CView*	view = schild->GetActiveView();

		int	gapX, gapY;
		gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
		gapX += GetSystemMetrics(SM_CXBORDER) * 2;

		gapY  = GetSystemMetrics(SM_CYFRAME) * 2;
		gapY += GetSystemMetrics(SM_CYBORDER) * 2;
		
		//if ((vwTYPE & vtypeMSK ) != vtypeWND)
		
		gapY += XCAP_HEIGHT;

		switch (vwTYPE)
		{
		case 0:
		default:
			if ((vwTYPE & vtypeMSK) == vtypeSCR)
				((CAxScrollView *)view)->SetInfo(key, m_axisres);
			else	((CAxisView *)view)->SetInfo(key, m_axisres);
			break;
		}

		if (!m_wizard->loadMap(mapN, key, size))	return;

		//m_bar1->set_Maps(mapN.Left(L_MAPN));
	}
	else
	{
		class CChildFrame* child;
		if (!m_arMDI[m_vsN].Lookup(key, child))	return;

		int	size, vwTYPE;
		vwTYPE = m_wizard->getFormInfo(getSMap(mapN), size);



		CRect	rect;
		CView*	view = child->GetActiveView();

		int	gapX, gapY;
		gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
		gapX += GetSystemMetrics(SM_CXBORDER) * 2;

		gapY  = GetSystemMetrics(SM_CYFRAME) * 2;
		gapY += GetSystemMetrics(SM_CYBORDER) * 2;
		//if ((vwTYPE & vtypeMSK ) != vtypeWND)
		
		gapY += XCAP_HEIGHT;

		switch (vwTYPE)
		{
		case 0:
		default:
			if ((vwTYPE & vtypeMSK) == vtypeSCR)
				((CAxScrollView *)view)->SetInfo(key, m_axisres);
			else	((CAxisView *)view)->SetInfo(key, m_axisres);
			break;
		}

		if (!m_wizard->loadMap(mapN, key, size))	return;

//		m_bar1->set_Maps(mapN.Left(L_MAPN));
		m_tMenu->set_Maps(mapN.Left(L_MAPN));

	}

}

// control bar code data receive
void CMainFrame::setCodeReg(int cnt, CString dat)
{
	CFile		file;
	CString		path, path_, code, name, string;
	path.Format("%s\\%s\\%s\\codereg", m_home, USRDIR, GetUserName());
	path_.Format("%s\\%s\\%s\\codereg.tmp", m_home, USRDIR, GetUserName());

	CopyFile(path, path_, FALSE);
	DeleteFile(path);
	if (!file.Open(path, CFile::modeCreate|CFile::modeWrite))
	{
		::MessageBox(m_hWnd, "Error. Don't save.", "Error.", MB_ICONASTERISK);
		MoveFile(path_, path);
		return;
	}

	struct _regCode	regCode;
	for (int ii = 0; ii < cnt; ii++)
	{
		int	pos = dat.Find('\n');
		if (pos == -1)	break;
		string = dat.Left(pos++);
		dat = dat.Mid(pos);

		memset(&regCode, ' ', sizeof(regCode));
		pos = string.Find('\t');
		if (pos == -1)	continue;
		code = string.Left(pos++);
		code.TrimRight();
		if (code.IsEmpty())	continue;
		name = string.Mid(pos);
		sprintf_s(regCode.code, code);
		sprintf_s(regCode.name, name);

		file.Write(&regCode, sizeof(regCode));
	}

	file.Close();
	DeleteFile(path_);

}

void CMainFrame::RunWebBrowser(CString HomePage, int cx, int cy)
{
	if (HomePage.Find("@") != -1)
	{
		ShellExecute(NULL, _T("open"), HomePage, NULL,NULL, SW_SHOWNORMAL);
		return;
	}

	CString	url = HomePage;
	HomePage = url;
	VARIANT vFlags = {0},
	vTargetFrameName = {0},
	vPostData = {0},
	vHeaders = {0};

	HRESULT hr;
	_bstr_t	bstr(HomePage);
	UINT	nFlag = SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW;
	WINDOWPLACEMENT	pl;

	if (::IsWindow(m_WBWnd))
	{
		m_pWBApp->Stop();
		if (HomePage.IsEmpty())
			m_pWBApp->GoHome();
		else
			m_pWBApp->Navigate(bstr, &vFlags, &vTargetFrameName, &vPostData, &vHeaders);

		m_pWBApp->put_Top(VARIANT_TRUE);
		m_pWBApp->put_Visible(VARIANT_TRUE);
		VariantClear(&vPostData);
		::GetWindowPlacement(m_WBWnd, &pl);
		if (pl.showCmd & SW_SHOWMINIMIZED)
		{
			pl.showCmd = SW_RESTORE;
			::SetWindowPlacement(m_WBWnd, &pl);
		}
		else	::SetWindowPos(m_WBWnd, HWND_TOP, 0, 0, 0, 0, nFlag);
		::PostMessage(m_WBWnd, WM_SETFOCUS, 0, 0);
		return;
	}

	if (FAILED(hr = CoInitialize(NULL)))	return;
	if (FAILED(hr = CoCreateInstance(CLSID_InternetExplorer,
			NULL, CLSCTX_SERVER, IID_IWebBrowserApp,
			(LPVOID*)&m_pWBApp)))	return;

	cx += GetSystemMetrics(SM_CXFRAME) * 2;
	cx += GetSystemMetrics(SM_CXBORDER) * 4;
	cy += GetSystemMetrics(SM_CYFRAME) * 2;
	cy += GetSystemMetrics(SM_CYBORDER) * 4;
	cy += GetSystemMetrics(SM_CYCAPTION);

	m_pWBApp->put_Width(cx);
	m_pWBApp->put_Height(cy);
	m_pWBApp->put_Left(VARIANT_TRUE);
	m_pWBApp->put_Top(VARIANT_TRUE);
	m_pWBApp->put_ToolBar(VARIANT_FALSE);
	m_pWBApp->put_StatusBar(VARIANT_FALSE);

	hr = m_pWBApp->Navigate(bstr, &vFlags, &vTargetFrameName, &vPostData, &vHeaders);
	if (HomePage.IsEmpty())	m_pWBApp->GoHome();
	m_pWBApp->put_Visible(VARIANT_TRUE);
	VariantClear(&vPostData);

	m_pWBApp->get_HWND((SHANDLE_PTR*)&m_WBWnd);
	::GetWindowPlacement(m_WBWnd, &pl);
	if (pl.showCmd & SW_SHOWMINIMIZED)
	{
		pl.showCmd = SW_RESTORE;
		::SetWindowPlacement(m_WBWnd, &pl);
	}
	else	::SetWindowPos(m_WBWnd, HWND_TOP, 0, 0, 0, 0, nFlag);
	CoUninitialize();
}

void CMainFrame::PopupWeb(CString url, int cx, int cy)
{
	if (url.Find("@") != -1)
	{
		ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOWNORMAL);
		return;
	}

	CString HomePage = url;
	VARIANT vFlags = {0},
	vTargetFrameName = {0},
	vPostData = {0},
	vHeaders = {0};

	HRESULT hr;
	_bstr_t	bstr(HomePage);
	UINT	nFlag = SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW;
	WINDOWPLACEMENT	pl;

	if (FAILED(hr = CoInitialize(NULL)))
		return;

	struct	IWebBrowserApp* pWEBApp;
	HWND	hWEB;
	if (FAILED(hr = CoCreateInstance(CLSID_InternetExplorer,
			NULL, CLSCTX_SERVER, IID_IWebBrowserApp,
			(LPVOID*)&pWEBApp)))
	{
		return;
	}

	cx += GetSystemMetrics(SM_CXFRAME) * 2;
	cx += GetSystemMetrics(SM_CXBORDER) * 4;
	cy += GetSystemMetrics(SM_CYFRAME) * 2;
	cy += GetSystemMetrics(SM_CYBORDER) * 4;
	cy += GetSystemMetrics(SM_CYCAPTION);

	pWEBApp->put_Width(cx);
	pWEBApp->put_Height(cy);
	pWEBApp->put_Left(VARIANT_TRUE);
	pWEBApp->put_Top(VARIANT_TRUE);
	pWEBApp->put_ToolBar(VARIANT_FALSE);
	pWEBApp->put_StatusBar(VARIANT_FALSE);
	hr = pWEBApp->Navigate(bstr, &vFlags, &vTargetFrameName, &vPostData, &vHeaders);
	if (HomePage.IsEmpty())	pWEBApp->GoHome();
	pWEBApp->put_Visible(VARIANT_TRUE);
	VariantClear(&vPostData);

	pWEBApp->get_HWND((SHANDLE_PTR*)&hWEB);
	::GetWindowPlacement(hWEB, &pl);
	if (pl.showCmd & SW_SHOWMINIMIZED)
	{
		pl.showCmd = SW_RESTORE;
		::SetWindowPlacement(hWEB, &pl);
	}
	else	::SetWindowPos(hWEB, HWND_TOPMOST, 0, 0, 0, 0, nFlag);
	CoUninitialize();
}

void CMainFrame::ExitWebBrowser()
{
	if (::IsWindow(m_WBWnd))
	{
		m_WBWnd = NULL;
		m_pWBApp->Stop();
		m_pWBApp->Quit();
		m_pWBApp->Release();
	}
}

void CMainFrame::funcKey(int funcID, int index)
{
	switch (funcID)
	{
	case CTRL_EDITTOOL:	EditTool();	break;
	case CTRL_REGISTER:
		{
	//		CTotalSetup dlg(m_home, m_userN, m_tMenu);
	//		dlg.DoModal();
	//		m_smain->Refresh();
		}
		break;
	case CTRL_SCREENPRINT:	printImg();	break;
	case CTRL_LOCK:		LockProg();	break;
	case CTRL_ENVIRONMENT:	ESetup();	break;
	case CTRL_HOMEPAGE:
		{
			CString	file;
			DWORD	dw;
			char	web[512];
			CString dat;
			
			file.Format("%s\\%s\\axis.ini", m_home, TABDIR);
			dw = GetPrivateProfileString("WebLink", "home", "www.pru.co.kr", web, sizeof(web), file);
			if (dw <= 0)	return;
			dat = web;
			if (dat.Mid(0, 4) == "http")
				ShellExecute(NULL, _T("open"), dat, NULL,NULL, SW_SHOWNORMAL);
			else
				m_pFormMgr->ChangeChild(dat);
		}
		break;
	case CTRL_TV:
		{
			CString	file;
			DWORD	dw;
			char	web[512];
			CString dat;
			
			file.Format("%s\\%s\\axis.ini", m_home, TABDIR);
			dw = GetPrivateProfileString("WebLink", "tv", "", web, sizeof(web), file);
			if (dw <= 0)	return;
			dat = web;
			if (dat.Mid(0, 4) == "http")
				ShellExecute(NULL, _T("open"), dat, NULL,NULL, SW_SHOWNORMAL);
			else
				m_pFormMgr->ChangeChild(dat);
		}
		break;
	case CTRL_RADIO:
		{
			CString	file;
			DWORD	dw;
			char	web[512];
			CString dat;
			
			file.Format("%s\\%s\\axis.ini", m_home, TABDIR);
			dw = GetPrivateProfileString("WebLink", "radio", "", web, sizeof(web), file);
			if (dw <= 0)	return;
			dat = web;
			if (dat.Mid(0, 4) == "http")
				ShellExecute(NULL, _T("open"), dat, NULL,NULL, SW_SHOWNORMAL);
			else
				m_pFormMgr->ChangeChild(dat);
		}
		break;
	case CTRL_WORKSPACELIST:
		ShowUScreenMenu();		break;
	case CTRL_REGISTERALL:
		toolRegisterAll(index);		break;
	default:
		DoFunc(funcID);
			break;
	}
}

void CMainFrame::DoFunc(int funcID)
{
	switch (funcID - CTRL_FUNC)
	{
		case -2:
			{
				CString dllPath;
				dllPath.Format(_T("%s\\exe\\AxisCode.dll"), m_home);
				HINSTANCE	m_hReg;
				m_hReg = LoadLibrary(dllPath);

				if(!m_hReg)
				{
					AfxMessageBox(_T("createbondpop fail"));
					FreeLibrary(m_hReg);
					return;
				}

				void (APIENTRY *ShowCodeDlg)( void*);
				ShowCodeDlg = (void (APIENTRY*)(void*))GetProcAddress(m_hReg, "ShowCodeDlg");
				if(ShowCodeDlg == NULL)
				{
					AfxMessageBox(_T("ShowCodeDlg fail"));
					FreeLibrary(m_hReg);
					return;
				}

				ShowCodeDlg((void*)m_home.operator LPCSTR());
				FreeLibrary(m_hReg);
			}
			break;
		case -1:
			{
				HWND hwn = ::FindWindow("TfrmMain", NULL);
				if(hwn == NULL)
				{
					SendMsgToMessanger(RUNANDLOGINSEND);
					Sleep(500);
					SendMsgToMessanger(SHOWMSGSEND);
				}
				else
				{
					if(!CheckMessangerLogin())
						SendMsgToMessanger(LOGINSEND);
					Sleep(100);
					SendMsgToMessanger(SHOWMSGSEND);
				}
			}
		break;
		case 0:  
				TotalSetup();
		break;
		case 1:
				saveClip(1);
			break;
		case 2:
			{
				printImg();
			}
			break;
		case 3:
			{
				m_pMain->m_pFormMgr->minimizeAll(true);
			}
			break;
		case 4:
			{
				m_pMain->m_pFormMgr->closeAll(true);	
				break;	
			}
			break;
		case 5:
				m_pFormMgr->ChangeChild(_T("FS205001"));
			break;
		case 6:
			{
					bool visible = !(m_bar1->GetStyle() & WS_VISIBLE);
					ShowControlBar(m_bar1, visible, FALSE);
					saveToolStatus();
			}
			break;
		case 7:
			{	
				bool visible = !(m_bar2->GetStyle() & WS_VISIBLE);
				ShowControlBar(m_bar2, visible, FALSE);
				saveToolStatus();
				SetSDIChangeHeight();
			}
			break;
	}
}

bool CMainFrame::changeMAX(CString mapN)
{
	bool	bMax = false;
	int	vtype = 0, vwTYPE = 0, size = 0, key = 0;
	vtype = m_wizard->getFormInfo(mapN, size);

	WINDOWPLACEMENT	pl;
	for (int ii = 0; ii < 4; ii++)
	{
		int		key;
		CChildFrame*	pChild;
		for (POSITION pos = m_arMDI[ii].GetStartPosition(); pos; )
		{
			m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);
			pChild->GetWindowPlacement(&pl);
			if (pl.showCmd & SW_SHOWMINIMIZED)
			{
				bMax = true;
				if (vtype == vtypeNRM || vtype == vtypeERR)
					continue;
				pl.showCmd = SW_HIDE|SW_RESTORE;
				pChild->GetWindowPlacement(&pl);
			}
		}
	}

	return bMax;
}

CString CMainFrame::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CMainFrame::load_eninfomation(bool first)
{
	char		buffer[128];
	memset(buffer,0, 128);

	CString		file,tmpS,dat,section,key,cpfile,mapN,userN,strpath;
	LONG_PTR	value, rc = 0;

	CWinApp* app = AfxGetApp();
	file.Format(_T("%s\\%s\\%s\\%s"),m_home, USRDIR, GetUserName(), SETUPFILE);

	//윈도우시작시 자동시작
	GetPrivateProfileString(_T("SCREEN"), _T("WINSTART") , _T("0"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);

	if(tmpS == _T("1"))
	{
		HKEY hkey;
		CString key_name, file_name;
		char file_path[100];

		strpath.Format(_T("%s\\exe\\axis.exe"), m_home);
		key_name = _T("bondaxis");

		LONG reg = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0L,KEY_WRITE,&hkey);
		if(ERROR_SUCCESS == reg)
		{
			strcpy_s(file_path, strpath);
			// 레지스트리의 run 항목에 자동 실행될 프로그램의 경로를 저장한다.
		//	reg = RegSetValueEx(hkey, key_name, 0, REG_SZ, (BYTE *)file_path, REG_SZ);
			reg = ::RegSetValueEx(hkey, key_name, 0, REG_SZ, (BYTE *)file_path, strpath.GetLength());
 //ONG nResult = ::RegSetValueEx(m_rRegistry.m_hKey, m_sValueName, 0, dwType, (const BYTE*)lpValue, nSize);
			// 오픈한 키를 닫는다.
			RegCloseKey(hkey);
		}
	}

	//프로그램 시작시 자동로그인
	GetPrivateProfileString(_T("SCREEN"), _T("AUTOLOGIN") , _T("0"), buffer, sizeof(buffer), file);

	//최대 팝업맵 화면 갯수
	GetPrivateProfileString(_T("SCREEN"), _T("MAXMAP") , _T("24"), buffer, sizeof(buffer), file);
	m_maxChild = atoi(buffer);
	if (m_maxChild > 99)	m_maxChild = 99;
	if (m_maxChild <= 0)	m_maxChild = 1;

	GetPrivateProfileString(_T("SCREEN"), _T("mapop") , _T("0"), buffer, sizeof(buffer), file); //0 최소조회 닫기 1 메시지창
	tmpS.Format(_T("%s"), buffer);  
	m_imaxpop = atoi(tmpS);   //0 최초띄운거 닫기 1 메시지띄우기

	//0 음소거소리켜기 1 음소거 소리 끄기
	GetPrivateProfileString(_T("SCREEN"), _T("soundoff") , _T("0"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		m_mngSound = true;
	else 
		m_mngSound = false;

	file.Format("%s\\%s\\config.ini", m_home, TABDIR);
	value = app->GetProfileInt(ENVIRONMENT, CHILDPOS, 0);
	m_posChild = HIWORD(value);

	if (GetPrivateProfileInt("MESSAGE", "IntervalSet", 0, file))
	{
		m_bGuideTimer = TRUE;

		m_nGuideInterval = GetPrivateProfileInt("MESSAGE", "Interval", 0, file);

		switch(m_nGuideInterval)
		{
		case 0:
			m_bGuideSet = FALSE;
			break;
		default:
			m_bGuideSet = TRUE;
			break;
		}
	}
	else
	{
		m_bGuideTimer = FALSE;
		m_bGuideSet = TRUE;
	}

	//폰트 사이즈 
	file.Format(_T("%s\\%s\\%s\\%s"),m_home, USRDIR, GetUserName(), SETUPFILE);
	GetPrivateProfileString(_T("SCREEN"), _T("FONTSIZE") , _T("1"), buffer, sizeof(buffer), file);


	//0 
	//1
	//2
	switch(atoi(buffer))
	{
		case 0:  //작게
			m_fontSize = 8;
			break;
		case 1:  //보통
			m_fontSize = 9;
			break;
		case 2:  //크게 
			m_fontSize = 10;
			break;
	}

	//알람기능 전체 켜기 끄기
	GetPrivateProfileString(_T("ALARM"), _T("ALL") , _T("1"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		m_mngPopup = true;
	else 
		m_mngPopup = false;

	//공지 알람 켜기 끄기
	GetPrivateProfileString(_T("ALARM"), _T("GONGJI") , _T("1"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		m_mngGongji = true;
	else 
		m_mngGongji = false;
	
	//수요예측 알람 켜기 끄기
	GetPrivateProfileString(_T("ALARM"), _T("SUYO") , _T("1"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		m_mngSuyo = true;
	else 
		m_mngSuyo = false;

	//발행공시 알람 켜기 끄기
	GetPrivateProfileString(_T("ALARM"), _T("BAL") , _T("1"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		m_mngBal = true;
	else 
		m_mngBal = false;

	//팝업창대기시간
	GetPrivateProfileString(_T("ALARM"), _T("SECOND") , _T("3"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	m_iTimeMng = atoi(tmpS) * 1000;

	m_axGuide->SetPos(GetPrivateProfileInt("MESSAGE", "POS", 0, file));

	value = GetPrivateProfileInt("DISPLAY", "FLASH", 0, file) ? 0 : 1;
	m_wizard->enableBlink(value);


	value = GetPrivateProfileInt("SCREEN", "CLOSE", 1, file) ? 1 : 0;

	m_nPopupPos = GetPrivateProfileInt("POPORDER", "POS", 0, file);

	char	sndB[64];
	DWORD dw = GetPrivateProfileString("Common", "SOUNDFILE_MDCON", "", sndB, sizeof(sndB), file);
	if (dw <= 0)	m_waveF.Empty();
	else		m_waveF = sndB;
	
	m_bwaveF = GetPrivateProfileInt("Common", "SOUNDON_MDCON", 0, file);
	m_nConInterval = GetPrivateProfileInt("Common", "Interval", 0, file);

	if (first)
		createUserScreen(_T("LASTSTAT"));

	changeFontSize();
	DrawFrame();
}

bool CMainFrame::CheckGonjiMap(CString strmap)
{
	for(int ii = 0; ii < m_mapGongji.GetSize() ; ii++)
	{
		if(strmap == m_mapGongji.GetAt(ii))
			return true;
	}
	return false;
}

void CMainFrame::Gongjiopen()
{
	CString tmpS, tempS;
	CChildFrame*	child;
	CRect clientRc;
	WINDOWPLACEMENT	pl;
	POSITION pos;
	CPoint sp;
	int value = 0;
	int vsN = 0;
	int key;
	int cnt = m_mapGongji.GetSize();

	for(int ii = 0 ; ii < cnt; ii++)
	{
		if(ii == 0)
		{
			tempS = m_mapGongji.GetAt(ii);
			Parser(tempS, _T("\t"));
			tmpS.Format(_T("%s%s\t%s\n"), GONGJIMAP,"title",tempS);
			key = m_pFormMgr->CreateChild(tmpS, 1, 0, 0, CPoint(100,100));
		}
		else
		{	
			child = NULL;
			for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
			{
				m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
				if(child->m_mapN == GONGJIMAP)
					break;
			}

			if(child == NULL)
				break;

			child->GetWindowPlacement(&pl);
			value = GetSystemMetrics(SM_CYCAPTION);
			value += GetSystemMetrics(SM_CXFRAME);
			value += GetSystemMetrics(SM_CXBORDER);
			sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
			sp.Offset(value, value);

			value = child->m_xcaption.GetGroup();
			switch (value)
			{
			case 0:
			case 99:	break;
			case 1:case 2: case 3:case 4: case 5:
				value++;
				if (value > 5)	value = 1;
				break;
			default:	value = 1;	break;
			}		

			tempS = m_mapGongji.GetAt(ii);
			Parser(tempS, _T("\t"));
			tmpS.Format(_T("%s%s\t%s\n"), GONGJIMAP,"title",tempS);
			CopyScreen(tmpS, value, child->m_xcaption.GetFontSize(), sp);
		}
	}

	CheckMsgType();
}

void CMainFrame::load_MngSetup()
{
	/*
	m_mapAlarmList.RemoveAll();

	CString file;
	file.Format("%s\\%s\\%s\\mngsetup.ini", m_home, USRDIR, m_userN);
	m_mngPos = 1;

	char	ssb[1024*4];
	CString tmpS;
	GetPrivateProfileString(_T("SCREEN"), _T("soundoff") , _T("0"), ssb, sizeof(ssb), file);
	tmpS.Format(_T("%s"), ssb);
	if(tmpS == _T("1"))
		m_mngSound = true;
	else 
		m_mngSound = false;

	memset(ssb, 0, 1024*4);
	DWORD ssL = GetPrivateProfileSection("Manage", ssb, sizeof(ssb), file);

	
	if (ssL <= 0)	
		return;

	CString subitem, keys, value, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem  = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		keys = subitem.Left(idx++);
		value = subitem.Mid(idx);

		if (atoi(value))
			m_mapAlarmList.SetAt(keys, value);
	}

	m_mngPos = GetPrivateProfileInt("Setup", "Pos", 0, file);
	m_mngPopup = GetPrivateProfileInt("Setup", "Popup", 0, file) ? true : false;
	m_mngSound = GetPrivateProfileInt("Setup", "Sound", 0, file) ? true : false;
	*/
	
}

void CMainFrame::load_ImageSetup()
{
	change_Skin();
}

bool CMainFrame::maxChild()
{
	int	key, index, count = 0;
	int vsN;
	for (vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		count += (int)m_arMDI[vsN].GetCount();
		count += (int)m_arSDI[vsN].GetCount();
	}
	
	if (count >= m_maxChild)
	{
		vsN = m_vsN+1;
		for (int jj = 0; jj <= V_SCREEN4; jj++, vsN++)
		{
			bool sdi = false;
			if (vsN > V_SCREEN4)	vsN = V_SCREEN1;
			if (m_arMDI[vsN].GetCount() <= 0)
			{
				if (m_arSDI[vsN].GetCount() <= 0)
					continue;
				sdi = true;
			}

			if (sdi)
			{
#if 1
				CSChild*	schild = NULL;

				index = (int)m_sdiZORDER[vsN].GetSize() - 1;
				key = m_sdiZORDER[vsN].GetAt(index);
				
				if (!m_arSDI[vsN].Lookup(key, schild))
					continue;

				if(m_imaxpop == 0)  //0 최초 1 메세지
				{
					schild->PostMessage(WM_CLOSE);
					return true;
				}
				else
					return false;
#else
				POSITION pos = m_arSDI[vsN].GetStartPosition();
				m_arSDI[vsN].GetNextAssoc(pos, key, schild);
				if (!schild)	
					continue;
				schild->PostMessage(WM_CLOSE);
#endif
			}
			else
			{
				index = (int)m_hooklist[vsN].GetSize() - 1;

				key = m_hooklist[vsN].GetAt(index);
				
				CChildFrame*	child;
				if (!m_arMDI[vsN].Lookup(key, child))
					continue;
				if(m_imaxpop == 0)  //0 최초 1 메세지
				{
					child->PostMessage(WM_CLOSE);
					return true;
				}
				else
					return false;
			}
			break;
		}
	}

	return true; //true 화면을 연다 false 화면을 열지 않고 메시지 받스
}

void CMainFrame::detachChild(int key)
{
	if (key < 0)	return;

	CChildFrame*	pChild = NULL;
	int	vsN;
	for (vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		if (!m_arMDI[vsN].Lookup(key, pChild))
			continue;
		m_arMDI[vsN].RemoveKey(key);
		break;
	}

	if (!pChild)	return;
	m_wizard->detachForm(key);

	if (MDIGetActive() == NULL)
	{
		if (m_tMenu)	m_tMenu->set_Maps();

		m_hooklist[m_vsN].RemoveAll();
	}
	if (m_bar2)	m_bar2->del_Button(key, vsN);
	for (int ii = 0; ii < m_hooklist[vsN].GetSize(); ii++)
	{
		if (key != m_hooklist[vsN].GetAt(ii))
			continue;
		m_hooklist[vsN].RemoveAt(ii);
	}
}

void CMainFrame::saveExitMap()
{
	CString		file, mapN, userN;
	char		buf[256];

	userN = GetUserName();

	file.Format("%s\\%s\\%s\\axisensetup.ini", m_home, USRDIR, userN);
	GetPrivateProfileString("SCREEN", "STARTMAP", "0", buf, sizeof(buf), file);
	//if (atoi(buf) != 1)	return;  //무조건 저장
	 
	save_laststat();
}

bool CMainFrame::CheckSoundOnOFF(CString strgubn)
{
	int igubn = atoi(strgubn);
	int iSoundCnt = 0 ;
	char		buffer[128];
	memset(buffer,0, 128);
	CString		file, tmpS, dat, temp, data, wavepath, wavfile;
	file.Format(_T("%s\\%s\\%s\\%s"), m_home, USRDIR, GetUserName(), SETUPFILE);

	GetPrivateProfileString(_T("SOUND"), _T("count") , _T("0"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);  //음향파일의 갯수 
	iSoundCnt = _ttoi(tmpS);

	if(iSoundCnt == 0)
	{
		file.Format(_T("%s\\%s\\%s"),m_home, MTBLDIR,SETUPFILE);
		GetPrivateProfileString(_T("SOUND"), _T("count") , _T("0"), buffer, sizeof(buffer), file);
		tmpS.Format(_T("%s"), buffer);
	}

	for(int ii = 0 ; ii < iSoundCnt ; ii++)
	{
		dat.Format(_T("%.2d"), ii);
		GetPrivateProfileString(_T("SOUND"), dat , _T("00"), buffer, sizeof(buffer), file);
		data.Format(_T("%s"), buffer);   
		temp = Parser(data, _T("\t"));   //
		dat =  Parser(data, _T("\t"));   //소리낸다 1  소리안낸다 0
		wavepath = Parser(data, _T("\t"));  //소리파일명 WAV1.wav

		if(ii == 0 && (igubn == 3 || igubn == 4)&& dat == "1")  //수요예측 신호음
		{
			wavfile.Format("%s\\%s\\%s", m_home, IMAGEDIR, wavepath);
			sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
			sndPlaySound(wavfile, SND_NODEFAULT|SND_ASYNC);
		}
	}

	return true;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	switch (nIDEvent)
	{
		case TM_CHECKTR:
			{
				KillTimer(TM_CHECKTR);
				if(!m_bChekcTr)
				{
				
				}
			}
			break;
		case TM_SHOWMNG:
			{
				if(!m_bMngShow  && m_msgArr.GetSize() > 0)
				{
					m_bMngShow = true;
					CString strdata, tmps, temp, title;
					strdata = m_msgArr.GetAt(0);

					tmps = Parser(strdata, _T("\n"));   //실제표시될 내용
					temp = Parser(strdata, _T("\t"));   //장구분자
					CheckSoundOnOFF(temp);  //1r공지사항 2 발행공지  3 4 수요예측 

					if(strdata == "1")   //장문 단문 구분자
					{
						title = Parser(tmps, _T("\t"));
						temp = Parser(tmps, _T("\t"));
						title += _T("\t");
					
						title += "test";
						title += _T("\t");

						title += Parser(tmps, _T("\t"));
						title += _T("\t");

						m_mngInfo->SetContent(temp);
						m_mngInfo->SetData(title, 0, m_mngPos);
					}
					else
					{
						m_mngInfo->SetContent("");
						m_mngInfo->SetData(tmps, 0, m_mngPos);
					}
					m_msgArr.RemoveAt(0);

					CRect		mRc;
					if (m_bSDI)
						GetDesktopWindow()->GetWindowRect(mRc);
					else	
						m_MClient->GetWindowRect(mRc);

					m_mngInfo->ShowSlide(mRc, m_iTimeMng);

					if(m_msgArr.GetSize() == 0)
						KillTimer(TM_SHOWMNG);
				}
			}
			break;
		case TM_MESSANGERLOGIN:
			{
				
			}
			break;
		case TM_MAINMENU:
			{
				m_iMenuCnt++;
				if( m_iMenuCnt == 3)
				{	
					KillTimer(TM_MAINMENU);
					m_iMenuCnt = 0;
					m_bSetMenu = true;
					ConfigFrame();
					if (m_step != axDONE)
					{
						m_step = axDONE;
						GetLocalTime(&m_connectT);
						convertIndex();
						ConfigTicker();
						m_runAxis = true;
						m_axConnectD->SetFrameRun();
						m_axConnectD->PostMessage(WM_CLOSE);
					}
				}
			}
			break;
		case TM_AUTOMNGINFO:
			m_mngInfo->HideSlide();
			break;
		case TM_IDLE:
			KillTimer(nIDEvent);
			LockProg();
			break;
		case TM_PURL:	
			KillTimer(nIDEvent);		break;
		case TM_MAINFOCUS:
			KillTimer(nIDEvent);
			break;
	}
	CMDIFrameWnd::OnTimer(nIDEvent);
}

CWnd* CMainFrame::getActiveView(int key)
{
	CChildFrame*	child;
	CSChild*	schild;

	if (m_arMDI[m_vsN].Lookup(key, child))
		return child->GetActiveView();
	if (m_arSDI[m_vsN].Lookup(key, schild))
		return schild->GetActiveView();
	return NULL;
}

void CMainFrame::load_history()
{
	if (!m_tMenu)	
		return;

	CString	file, maps, tmpS;
	char	buf[1024];

	m_savelist.RemoveAll();
	file.Format("%s\\%s\\axisuser.ini", m_home, TABDIR);
	DWORD dw = GetPrivateProfileString("System", "maps", "", buf, sizeof(buf), file);
	if (dw <= 0)	
		return;

	maps = buf;
	while (!maps.IsEmpty())
	{
		int pos = maps.Find(',');
		if (pos == -1)
		{
			maps.TrimLeft();
			maps.TrimRight();
			if (maps.GetLength() == L_MAPN)
			{
				add_history(maps);
				addTool_history(maps);
			}
			break;
		}
		tmpS = maps.Left(pos);
		maps = maps.Mid(pos+1);
		tmpS.TrimLeft();	tmpS.TrimRight();
		if (tmpS.GetLength() == L_MAPN)
		{
			add_history(tmpS);
			addTool_history(tmpS);
		}
	}
}

void CMainFrame::save_history()
{
	CString	maps, string, file;
	file.Format("%s\\%s\\axisuser.ini", m_home, TABDIR);
	for (int ii = 0; ii < m_savelist.GetSize(); ii++)
	{
		if (!ii)
			maps = m_savelist.GetAt(ii);
		else	maps.Format(", %s", m_savelist.GetAt(ii));
		
		string += maps;
	}

	WritePrivateProfileString("System", "maps", string, file);
}

void CMainFrame::add_history(CString mapN)
{
	if (mapN.GetLength() != L_MAPN)	return;

	CString	file, maps, tmpS, tail;
	for (int ii = 0; ii < m_savelist.GetSize(); ii++)
	{
		maps = m_savelist.GetAt(ii);
		tail = maps.Mid(6, 2);
		if (tail.CompareNoCase("00"))	continue;
		if (!mapN.CompareNoCase(maps))	return;
	}
	m_savelist.Add(mapN);
}

void CMainFrame::addTool_history(CString mapName)
{
	CString	dat, mapN = mapName.Left(L_MAPN);
	CString	tail = mapN.Mid(6);

	if (tail.CompareNoCase("00"))	
		return;
	dat.Format("%s%s%s", m_tMenu->GetDispN(mapN), mapN, m_tMenu->GetDesc(mapN));
	m_bar1->add_History(dat);
	m_tMenu->add_History(dat);
	m_smain->add_History(dat);
}

void CMainFrame::change_Resource()
{
	CString		skinName = GetSkinName();
	if (m_axisres)	m_axisres->ChangeRES(skinName);

	int		key;
	CSChild*	schild;
	CChildFrame*	child;
	POSITION pos;
	
	for (int ii = V_SCREEN1; ii <= V_SCREEN4; ii++)
	{
		for (pos = m_arMDI[ii].GetStartPosition(); pos; )
		{
			m_arMDI[ii].GetNextAssoc(pos, key, child);
			child->ChangeSkin(skinName);
			if (!child->m_tView)	continue;
			CView* view = child->GetActiveView();
			((CAxTabView *) view)->ChangeSkin();
		}

		for (pos = m_arSDI[ii].GetStartPosition(); pos; )
		{
			m_arSDI[ii].GetNextAssoc(pos, key, schild);
			schild->ChangeSkin(skinName);
			if (!schild->m_tView)	continue;
			CView* view = schild->GetActiveView();
			((CAxTabView *) view)->ChangeSkin();
		}
	}
}

void CMainFrame::ShowFITSInfo(CString dat)
{
	if (m_mngInfo)
	{
		CMapStringToString	fms;
		CString		sym, val, str;
		int		pos = 0;
		CString		wavfile;
		val = Parser(dat, _T("\t"));
		val.Empty();
		while(!dat.IsEmpty())
		{
			pos = dat.Find("\t");

			if (pos != -1)
			{
				sym = dat.Left(pos++);
				dat = dat.Mid(pos);
				pos = dat.Find("\t");
				if (pos != -1)
				{
					val = dat.Left(pos++);
					dat = dat.Mid(pos);
				}
				else
				{
					val = dat;
					dat = _T("");
				}
			}
			else
			{
				sym = dat;
				val = _T("");
				dat = _T("");
			}
			
			
			if (sym.IsEmpty())
				break;

			fms.SetAt(sym, val);
		}

		CString gubn, date, title, content, checkmore, menu, key, temp;
		
		if(!fms.Lookup("501", gubn))
			return;

		//공지구분에 따른 체크  	
		if(gubn == "1")  //1 공지사항   
		{
			if(!m_mngGongji)
				return;
		}

		if(gubn == "2")  //2 발행공지   
		{
			if(!m_mngBal)
				return;
		}

		if(gubn == "3" || gubn == "4")  //3 수요예측정기공지  4 수요예측등록 
		{
			if(!m_mngSuyo)
				return;
		}

		if(!m_mngPopup)
			return;

		if(!fms.Lookup("503", title))
			return;

		if(!fms.Lookup("504", content))
			return;
		
		if(!fms.Lookup("505", checkmore))
			return;

		if(!fms.Lookup("506", menu))
			return;
	
		if(!fms.Lookup("507", key))
			return;
	
		title += _T("\t");         //타이틀

		WCHAR* pwdata = new WCHAR[content.GetLength()];
		memset(pwdata, 0, content.GetLength()*2);
		MultiByteToWideChar(CP_ACP, 0, (LPSTR)content.operator LPCSTR(), -1, pwdata, content.GetLength()*2);
		bool blong = false;
		if(checkmore == "Y" && wcslen(pwdata) > LENPUSH)
		{
			blong = true;
			CString strLongcontent;
			int icnt = 0, ilen;
		
			for(int ii = 0 ; ii < wcslen(pwdata) / LENPUSH + 1 ; ii++)
			{
				if(icnt * LENPUSH <= content.GetLength())
					ilen = LENPUSH;
				else
					ilen = content.GetLength() % LENPUSH;

				WCHAR* pdata = new WCHAR[LENPUSH + 1];
				wmemset(pdata, 0x00, LENPUSH + 1);
				wmemcpy(pdata, &pwdata[icnt * LENPUSH], ilen);
				char *str2;   //TCHAR to char
			    int len = WideCharToMultiByte(CP_ACP,0,pdata,-1,NULL,NULL,NULL,NULL);
			    str2 = new char[len];
			    len = WideCharToMultiByte(CP_ACP,0,pdata,-1,str2,len,NULL,NULL);
				CString tmps;
				tmps.Format(_T("%s"), str2);
			
				strLongcontent += tmps.Left(len);
				strLongcontent += _T("#@");
				icnt++;
			}
			title += strLongcontent; //내용
			delete pwdata;
		}
		else
		{
			//m_mngInfo->SetContent("");
			title += content; //내용
		}	
		title += _T("\t");        

		title += checkmore;        //더보기
		title += _T("\t");
		title += menu;             //열화면
		title += _T("\t");
		title += key;			   //화면에 전달해줄 키값
		title += _T("\n");
		title += gubn;

		if(blong)
		{
			title += _T("\t");
			title += "1";
		}
		else
		{
			title += _T("\t");
			title += "0";
		}
		//titile = 내용(#@) + "\t" + 더보기 + "\t" + 열화면 + "\t" + 화면에 전달해줄 키값 + "\n" + 장구분자 "\t" + 장문단문 구분자
		m_msgArr.Add(title);
		SetTimer(TM_SHOWMNG,100,NULL);
	}
}

void CMainFrame::ShowMngInfo(CString dat)
{
	
}

BOOL CMainFrame::ConclusionNotice(CString dat, CString& title)
{
 	if (m_chaser && m_chaser->GetSafeHwnd())
 	{
 		OnCHASER(MAKEWPARAM(dat.GetLength(), x_CONs),
 			(LPARAM) (char *) dat.operator LPCTSTR());
 	}

	if (!m_runAxis)	return FALSE;

	CString			str, value, value2, file, jcode, strGubn;
	CMapStringToString	ary;
	NoticeParse(dat, ary);

	if (!ary.Lookup("947", str))		// user ID
	{
		ary.RemoveAll();
		return FALSE;
	}
	strGubn = str;
	if (!ary.Lookup("905", str))		// user ID
	{
		ary.RemoveAll();
		return FALSE;
	}
	if (str.CompareNoCase(m_user))		// check userID
	{
		ary.RemoveAll();
		return FALSE;
	}
	bool	bCon;
	if (!ary.Lookup("940", str))
	{
		ary.RemoveAll();
		return FALSE;
	}
	if (!ary.Lookup("941", str))
	{
		ary.RemoveAll();
		return FALSE;
	}
	if (strGubn != "D")
	{
		if (atoi(str) > 0)			// conclusion count
		{	// 체결
			bCon = true;
			if (ary.Lookup("931", value))	// conclusion time
				dat = value;
			dat += "\t";

			if (ary.Lookup("903", value))	// order number
				dat += value;
			dat += "\t";

			if (ary.Lookup("920", value))	// original order number
				dat += value;
			dat += "\t";

			if (ary.Lookup("907", value))	// account Name
				dat += value;
			dat += "\t";

			if (ary.Lookup("021", value))	// code Name
				dat += value;
			dat += "\t";

			if (ary.Lookup("909", value))	// mmgb
			{
				if (value == "1") value = "매도";
				else value = "매수";
				dat += value;
			}
			else	dat += " ";
			dat += "\t";

			if (ary.Lookup("941", value))	// conclusion count
				dat += value;
			dat += "\t";

			if (ary.Lookup("940", value))	// conclusion price
				dat += value;
			
			value=_T("");
		}
	}

	if (m_bwaveF)
	{
		DWORD tick = GetTickCount();

		if ((int)(tick - m_nConTickCount) > m_nConInterval)
		{
			m_nConTickCount = GetTickCount();
			if (m_waveF.IsEmpty())	MessageBeep(MB_OK);
			else
			{
				sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
				sndPlaySound(m_waveF, SND_NODEFAULT|SND_ASYNC);
			}
		}
	}

	if (strGubn != "D")
		m_ConCDLG->InsertMsg(dat);

	return TRUE;
}

void CMainFrame::NoticeParse(CString dat, CMapStringToString& ary)
{
	int	pos, pos2;
	CString	str, symbol, value;

	pos = dat.Find('\n');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	while (!str.IsEmpty())
	{
		pos = str.Find('\t');
		if (pos == -1)	break;

		symbol = str.Left(pos++);
		str    = str.Mid(pos);

		pos = str.Find('\t');
		if (pos == -1)
		{
			value = str;
			str.Empty();
		}
		else
		{
			pos2 = pos;
			value = str.Left(pos2++);
			pos = value.Find('\r');
			if (pos != -1)
			{
				value = value.Left(pos++);
				str   = str.Mid(pos);
			}
			else
				str = str.Mid(pos2);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
	}

	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;

		symbol = dat.Left(pos++);
		dat    = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat   = dat.Mid(pos);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
	}
}

BOOL CMainFrame::HideGuide()
{
	if (!m_axGuide)	return FALSE;
	if (m_axGuide->GetStyle() & WS_VISIBLE)
	{
		displayGuide("");
		return TRUE;
	}
	return FALSE;
}

BOOL CMainFrame::ExistMenu(CString mapN)
{
	if (!m_tMenu)	return FALSE;

		return TRUE;

}

int CMainFrame::CheckMapping(CString mapN)
{
	int	key = -1;
	if (m_tMenu->IsMappinged(mapN))
		return 0;
	return key;
}

void CMainFrame::SetAMap(int key, char* mapN)
{
	if (key & winK_POPUP)
	{
		if (!m_arMPOP.IsEmpty())
		{
			CMPop*	Mpop;
			if (m_arMPOP.Lookup(key, Mpop))
			{
				CopyMemory(mapN, Mpop->m_mapN, L_MAPN);
				return;
			}
		}

		CGPop* Gpop;
		if (!m_arGPOP.Lookup(key, Gpop))
			return;
		CopyMemory(mapN, Gpop->m_mapN, L_MAPN);
	}
	else
	{
		CChildFrame*	child;
		if (!m_arMDI[m_vsN].Lookup(key, child))
		{
			CSChild*	schild;
			if (!m_arSDI[m_vsN].Lookup(key, schild))
			{
				//sprintf(mapN, "");
				mapN[0] = '0';
				return;
			}
			CopyMemory(mapN, schild->m_mapN, L_MAPN);
		}
		else	CopyMemory(mapN, child->m_mapN, L_MAPN);
	}
}

void CMainFrame::SetSearchMap(char* mapN)
{
	CTrSearch	dlg(m_tMenu, m_home);
	if (dlg.DoModal() == IDOK)
		sprintf_s(mapN, dlg.getTR().GetLength(),(const char*)dlg.getTR());
}

CString CMainFrame::parseData(CString dat)
{
	int	pos = dat.Find(SEP);
	if (pos == -1)	return dat;

	CString	user, string;

	string = dat.Left(pos++);
	dat = dat.Mid(pos);

	while (!string.IsEmpty())
	{
		pos = string.Find(USERSEP);
		if (pos == -1)	break;
		user = string.Left(pos++);
		string = string.Mid(pos);

		if (!user.CompareNoCase(m_user))
			return dat;
	}
	return _T("");
}

void CMainFrame::HelpLink(int key)
{
	CString	mpN;
	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		HelpLinkS(key);
		return;
	}
	mpN = m_tMenu->GetMapping(child->m_mapN);
	if (mpN.GetLength() != L_MAPN)	return;

	CString	file, link, dat, dispN = mpN.Mid(2, 4);

	file.Format("%s\\help\\help.dat", m_home);
	FILE *fp;
	errno_t err = fopen_s(&fp, file, "r");
	if (err)	return;

	char	buf[1024];
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		link = buf;
		int pos = link.Find('=');
		if (pos == -1)	continue;
		dat = link.Left(pos++);

		dat.TrimLeft();
		dat.TrimRight();
		if (dispN.CompareNoCase(dat))	continue;

		link = link.Mid(pos);
		link.TrimLeft();
		link.TrimRight();

		RunWebBrowser(link);
		break;
	}
	fclose(fp);
}

void CMainFrame::HelpLinkS(int key)
{
	CString		mpN;
	CSChild*	child;
	if (!m_arSDI[m_vsN].Lookup(key, child))
		return;
	mpN = m_tMenu->GetMapping(child->m_mapN);
	if (mpN.GetLength() != L_MAPN)	return;

	CString	file, link, dat, dispN = mpN.Mid(2, 4);

	file.Format("%s\\help\\help.dat", m_home);
	FILE *fp;
	
	errno_t err = fopen_s(&fp, file, "r");
	if (err)	return;
	char	buf[1024];
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		link = buf;
		int pos = link.Find('=');
		if (pos == -1)	continue;
		dat = link.Left(pos++);

		dat.TrimLeft();
		dat.TrimRight();
		if (dispN.CompareNoCase(dat))	continue;

		link = link.Mid(pos);
		link.TrimLeft();
		link.TrimRight();

		RunWebBrowser(link);
		break;
	}
	fclose(fp);
}

CString CMainFrame::GetUserName()
{
	CString	userN, dir;
	userN = m_axis->GetProfileString(WORKSTATION, SIGNONID);
	int pos = userN.Find('|');

//	if (pos < 0)	
//		userN = _T("Anonymous");
//	else		
		userN = userN.Mid(pos+1);

//	if (userN.IsEmpty())	
//		userN = _T("Anonymous");

	return userN;
}

void CMainFrame::SmCall(int idx)
{
	if (m_smcall)
	{
		m_smcall->Show_Environment(idx);
		load_eninfomation(false);
	//	m_bar1->Refresh();
	}
}

void CMainFrame::toolRegister()
{
	CString	mapN;
	CChildFrame* child = (CChildFrame*) MDIGetActive();
	if (!child)	return;
	mapN.Format("%s00", child->m_mapN.Left(L_MAPN-2));
	if (!ExistMenu(mapN))	return;

	char	ssb[1024*4];
	CString	file, tmpS, str;
	CString user = GetUserName();

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, user);
	DWORD ssL = GetPrivateProfileString(USERSN, USEREN, "0, 0", ssb, sizeof(ssb), file);
	tmpS = CString(ssb, ssL);
	int	idx = tmpS.Find(',');
	if (idx == -1)	idx = 0;
	else
	{
		tmpS = tmpS.Left(idx);
		idx = atoi(tmpS);
	}

	tmpS.Format("%02d", idx);
	ssL = GetPrivateProfileString(USERSN, tmpS, "", ssb, sizeof(ssb), file);
	if (ssL <= 0)	return;
	ssL = GetPrivateProfileString(tmpS, mapN, "", ssb, sizeof(ssb), file);
	if (ssL > 0)	return;

	ssL = GetPrivateProfileSection(tmpS, ssb, sizeof(ssb), file);
	str.Format("%s;%s", m_tMenu->GetDesc(mapN), m_tMenu->GetSName(mapN));
	if (ssL <= 0)
		WritePrivateProfileString(tmpS, mapN, str, file);
	else
	{
		sprintf_s(&ssb[ssL], mapN.GetLength() + str.GetLength() + 2, "%s=%s", mapN, str);
		ssL += str.GetLength() + L_MAPN +2;
		WritePrivateProfileSection(tmpS, CString(ssb, ssL), file);
	}
}

void CMainFrame::toolRegisterAll(int index)
{
	switch (index)
	{
	case 0:		// 모든화면 툴바등록
		AllScreenRegister();
		break;
	default:
		break;
	}
}

void CMainFrame::AllScreenRegister()
{
	int	key, idx;
	char	ssb[1024*4];
	DWORD	ssL;
	CString	file, section, str, mpN;
	CString user = GetUserName();
	CMapStringToString	ary;

	CChildFrame* child = (CChildFrame*) MDIGetActive();
	if (!child)	return;

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, user);
	ssL = GetPrivateProfileString(USERSN, USEREN, "0, 0", ssb, sizeof(ssb), file);
	section = CString(ssb, ssL);
	idx = section.Find(',');
	if (idx == -1)	idx = 0;
	else
	{
		section = section.Left(idx);
		idx = atoi(section);
	}

	section.Format("%02d", idx);
	ssL = GetPrivateProfileString(USERSN, section, "", ssb, sizeof(ssb), file);
	if (ssL <= 0)	return;
	ssL = GetPrivateProfileSection(section, ssb, sizeof(ssb), file);
	if (ssL > 0)
	{
		CString item, tmps, mapN, string = CString(ssb, ssL);
		for (; !string.IsEmpty(); )
		{
			idx = string.Find('\0');
			if (idx == -1)	break;

			item  = string.Left(idx++);
			string = string.Mid(idx);

			idx = item.Find('=');
			if (idx == -1)	continue;
			mapN = item.Left(L_MAPN);
			if (!ary.Lookup(mapN, tmps))
				ary.SetAt(mapN, "1");
		}
	}

	for (POSITION pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
		mpN.Format("%s00", child->m_mapN.Left(L_MAPN-2));
		if (ary.Lookup(mpN, str))	continue;
		if (!ExistMenu(mpN))		continue;
		str.Format("%s=%s;%s", mpN, m_tMenu->GetDesc(mpN), m_tMenu->GetSName(mpN));

	//	m_bar1->AddItem(str);
		sprintf_s(&ssb[ssL], str.GetLength(), "%s", str);
		ssL += str.GetLength()+1;
		ary.SetAt(mpN, "1");
	}
	WritePrivateProfileSection(section, CString(ssb, ssL), file);
	ary.RemoveAll();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CRect	rc;

	GetClientRect(&rc);
	rc.DeflateRect(6, 6);

	DeleteObject(m_rgn);
	m_rgn = ::CreateRectRgn(rc.left, rc.top, rc.right + 16, rc.bottom + 38);

	if (DisplaySystemVersion() >= osVISTA)
		SetWindowRgn(m_rgn, TRUE);

	CMDIFrameWnd::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED && m_bSDI)
	{	
		int		key;
		CSChild*	schild = NULL;
		POSITION pos;

		for (pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
			schild->ShowWindow(SW_HIDE);
		}

		CGPop* popup = NULL;

		for (pos = m_arGPOP.GetStartPosition(); pos; )
		{
			m_arGPOP.GetNextAssoc(pos, key, popup);

			popup->ShowWindow(SW_HIDE);
		}
	}

	

	Invalidate();
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CMDIFrameWnd::OnSizing(fwSide, pRect);

	if (!m_bSDI)	return;
	CRect*	prect = (CRect *) pRect;
	if (prect->Height() != m_SDIHEIGHT)
	{
		switch (fwSide)
		{
		case WMSZ_TOP:case WMSZ_TOPLEFT:case WMSZ_TOPRIGHT:
			prect->top = prect->bottom - m_SDIHEIGHT;	break;
		default:prect->bottom = prect->top + m_SDIHEIGHT;	break;
		}
	}
}

void CMainFrame::AppendAllMap()
{
	int		key;
	char		buf[128];
	CString		file, str;
	CChildFrame*	child;
	CMapStringToString	arMap;

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, GetUserName(), "screenreg.ini");
	for (int ii = 0; ; ii++)
	{
		str.Format("%04d", ii);
		DWORD dw = GetPrivateProfileString("SCREEN", str, "", buf, sizeof(buf), file);
		if (dw <= 0)	break;
		CString mapN = buf;
		arMap.SetAt(mapN, str);
	}

	for (POSITION pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
		if (arMap.Lookup(child->m_mapN.Left(L_MAPN), str))
			continue;

		str.Format("%04d", arMap.GetCount());
		WritePrivateProfileString("SCREEN", str, child->m_mapN.Left(L_MAPN), file);
		arMap.SetAt(child->m_mapN.Left(L_MAPN), str);
	}
}

bool CMainFrame::axSFocus(int vsN)
{
	if (vsN == -1)	vsN = m_vsN;

	if(m_bSDI)
	{
		CSChild *schild = NULL;
		for (int ii = 0; ii < m_sdiZORDER[vsN].GetSize(); ii++)
		{
			int key = m_sdiZORDER[vsN].GetAt(ii);

			if (m_arSDI[vsN].Lookup(key, schild))
			{
				schild->SetFocus();
				return true;
			}
		}
	}

	return false;
}

bool CMainFrame::axFocus(int key)
{
	KillTimer(TM_MAINFOCUS);
	if (m_bSDI)
	{
		CSChild*	schild;
		if (!m_arSDI[m_vsN].Lookup(key, schild))
			return false;
		if (m_bar2)	m_bar2->change_Button(schild->m_key);

		m_activeMapN = schild->m_mapN;
	}
	else
	{
		CChildFrame*	child;
		if (!m_arMDI[m_vsN].Lookup(key, child))
			return false;
		if (m_bar2)	m_bar2->change_Button(child->m_key);
		m_activeMapN = child->m_mapN;		
	}

	return true;
}

bool CMainFrame::IsvalidMap(CString mapN)
{
	if (!m_dev)
	{
		CString	tail = mapN.Mid(6, 2);
		if (tail == "00")
		{
			if (!ExistMenu(mapN.Left(L_MAPN)))
				return false;
		}
	}

	return true;
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	switch( nState )
	{
	case WA_INACTIVE:
		OnNcActivate(FALSE);
		break;
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		OnNcActivate(TRUE);
		if (pWndOther && pWndOther->GetOwner() == this)
			return;

		if (m_activeCap && m_bSDI && m_noActive)
		{
			CSChild*	child;
			WINDOWPLACEMENT	pl;
			GetWindowPlacement(&pl);
			int key;

			if (!(pl.showCmd & SW_SHOWMINIMIZED))
			{
				CArray	<int, int>	aryK;

				for (int value = (int)m_sdiZORDER[m_vsN].GetUpperBound(); value >= 0; value--)
				{
					key = m_sdiZORDER[m_vsN].GetAt(value);
					aryK.Add(key);
				}

				for (int ii = 0; ii < aryK.GetSize(); ii++)
				{		
					key = aryK.GetAt(ii);
					if (!m_arSDI[m_vsN].Lookup(key, child))	continue;
					 
					child->SetWindowPos(NULL, 0, 0, 0, 0, 
 						SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
				}

				m_noActive = false;
				SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
				m_noActive = true;
			}
		}
 		break;
	}
}

void CMainFrame::fullScreenMode(bool full)
{
	m_fullmode = full;
	if (m_fullmode)
	{
		m_toolStatus = 0;
		GetWindowPlacement(&m_pl);
		SetToolStatus();

		ShowControlBar(m_bar2, FALSE, FALSE);
		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}
	else
	{	
		if (m_toolStatus & TB_LISTBAR)		ShowControlBar(m_bar2, TRUE, FALSE);	
		SetWindowPlacement(&m_pl);
	}
}

void CMainFrame::changeFontSize()
{
	int	key;
	LONG_PTR	rc = 0;
	POSITION pos;

	m_wizard->setFontSize(-1, m_fontSize);

	CChildFrame*	child;
	CSChild*	schild;
	for (int ii = V_SCREEN1; ii <= V_SCREEN4; ii++)
	{
		for (pos = m_arMDI[ii].GetStartPosition(); pos; )
		{
			m_arMDI[ii].GetNextAssoc(pos, key, child);			
			m_wizard->setFontSize(key, m_fontSize);
			child->m_xcaption.SetFontSize(m_fontSize);
			child->Invalidate();
		}

		for (pos = m_arSDI[ii].GetStartPosition(); pos; )
		{
			m_arSDI[ii].GetNextAssoc(pos, key, schild);
			m_wizard->setFontSize(key, m_fontSize);
			schild->m_xcaption.SetFontSize(m_fontSize);
			schild->Invalidate();
		}
	}

	CGPop*	Gpop;
	for (pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, Gpop);
		m_wizard->setFontSize(key, m_fontSize);
		Gpop->Invalidate();
	}
}

/*
CString path, sWrite, tmpS;
	CFile file;
	path = m_home + "\\tab\\" + BONDCODE ; 
	
	CFile pFile;
//	if (file.Open(path, CFile::modeReadWrite|CFile::typeBinary))
	if (file.Open(path, CFile::modeReadWrite))
	{
		DWORD	pos = (DWORD)file.SeekToEnd();
		file.Seek((long) pos, CFile::begin);
		file.Write(sWrite, sWrite.GetLength() );
		file.Close();
	}
*/

void CMainFrame::MakeMenuFile(char *pdata, int icnt)
{
	CStringArray arr;
	CString strdata, path;
	CFile file;
	path = m_home + "\\tab\\" + "axis.menu" ; 
	char* pbuf = new char[113];
	DeleteFile(path);
	
	if(file.Open(path, CFile::modeCreate|CFile::modeReadWrite))
	{
		for(int ii = 0 ; ii < icnt; ii++)
		{
			strdata.Empty();
			memset(pbuf, 0, 113);
			memcpy(pbuf, &pdata[112 * ii], 112);
		
			strdata.Format(_T("%s\t%s\t%s\t%s\t%s\n"), CString(&pbuf[0], 40).TrimRight(), CString(&pbuf[40], 40).TrimRight(), CString(&pbuf[80], 8).TrimRight(),
												CString(&pbuf[88], 20).TrimRight(), CString(&pbuf[108], 4).TrimRight());
			//TRACE(strdata);
			//arr.Add(strdata);
			DWORD	pos = (DWORD)file.SeekToEnd();
			file.Seek((long) pos, CFile::begin);
			file.Write(strdata, strdata.GetLength() );
		}
		file.Close();
		MakeMenuArray();
	}
	

	m_bSetMenu = true;
}

void CMainFrame::processFMX(WPARAM wParam, LPARAM lParam)
{
	CString tmpS, tempS, strresult;
	int key = LOWORD(wParam);
	int len = HIWORD(wParam);
	
	char* pbuf= (char*)lParam;
	char* pdata = new char[len - sizeof(struct  _ledgerH)];
	memset(pdata, 0, len - sizeof(struct  _ledgerH));
	memcpy(pdata, (char*)&pbuf[sizeof(struct  _ledgerH)], len - sizeof(struct  _ledgerH));
	
	CString ikey(pdata, 2);

	if(atoi(ikey) == 1)
	{
		CString icnt(&pdata[22], 10);
		MakeMenuFile(&pdata[32], atoi(icnt));
		return;
	}
	else if(atoi(ikey) == 2)
	{
		bool bfind = false;
		bool bsameday = false;
		CString tmpS;
		CString strdata;
		CString strResult;
		CString struser, strpath;
		CString strUserPath(_T(""));
		CString strFilePath(_T(""));\
		CStringArray arr;
		CTime time = CTime::GetCurrentTime();
		CString sTime;
		sTime.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

		char* pBuffer = new char[1024];
		strFilePath.Format(_T("%s\\user\\%s\\%s"), m_home, GetUserName(), _T("NOTICE.ini"));
		GetPrivateProfileString("NOTICE", "TODAY", "", pBuffer, 1024, strFilePath);
	
		strdata.Format(_T("%s"), pBuffer);
//strresult.Format(_T("1[파일내용 %s  [%s]"), strFilePath, strdata);

		if(atoi(sTime) == atoi(Parser(strdata, _T("\t"))))  //파일에 써있는 내용이 오늘인지
			bsameday = true;

		while(1)
		{
			arr.Add(Parser(strdata, _T("\t")));
			if(strdata.GetLength() == 0)
				break;
		}
	
		m_mapGongji.RemoveAll();

		/*
		struct mod
		{
			char gubn[2];  //tr 구분자
			char cnt[10];  //title과 date의 반복 횟수
			char title[1000];
			char date[8];
		};
		*/

		CString icnt(&pdata[2], 10);
		for(int ii = 0 ; ii < atoi(icnt); ii++)
		{
			bfind = false;
			tmpS.Format(_T("%.19s"), &pdata[12 + (19 * ii) + (1000 * ii) + (8 * ii) ]); //title
			tmpS.TrimRight();
			tempS.Format(_T("%.8s"), &pdata[12 + (19 * ii) + (1000 * ii) + (8 * ii) + 1019]);  //day
			tempS.TrimRight();  //fs603000
//outputtrace("2 수신내용]  " + tmpS  + _T("  ") + tempS);			
			for(int jj = 0 ; jj < arr.GetSize(); jj++)
			{
				if(arr.GetAt(jj).Find(tmpS) >= 0)
					bfind = true;	
			}
strresult.Format("3 [%s] [%s] [%s] [%d] \n", tempS, bsameday == true ? _T("같은날"): _T("다른날"),  tmpS, bfind);
//outputtrace(strresult);

			if(bsameday && bfind)   //오늘은 보지 않겠다고 파일에 써두고 그날이 실제 당일일때
				continue;

			strresult.Format(_T("%s\t%s\t%s"), GONGJIMAP, tmpS, tempS);
			m_mapGongji.Add(strresult);	
		}

strresult.Format(_T("4 [%d]"), 	m_mapGongji.GetSize());
//outputtrace(strresult);

		Gongjiopen();
		return;
	}
	
	if (m_chaser && m_chaser->GetSafeHwnd())
		OnCHASER(MAKEWPARAM(len, x_RCVs), (LPARAM) (char *) lParam);

	switch (key)
	{
	case sysACCOUNT:
		break;
	case sysAllCancel:
		{
			struct mod1* outAllCancel = (struct mod1*)lParam;
			if (outAllCancel->mgfg[0] == '1')	// Guide Message 출력
			{
				CString strMsg(outAllCancel->mesg, sizeof(outAllCancel->mesg));
				strMsg.TrimLeft(); strMsg.TrimRight();
				displayGuide(strMsg);
			}
			
		}
		break;
	default:	TRACE("CMainFrame::processFMX[%d]\n", key);	break;
	}
}

void CMainFrame::processCONNECTINFO(char *dat)
{
	struct  _modCT {
		char    date[8];                /* 최종접속일           */
		char    time[6];                /* 최종접속시간         */
		char    emsg[80];               /* Message              */
	};
	
	struct _modCT* mod = (struct _modCT *) dat;
	CString	date, time, lastTime;
	
	date = CString(mod->date, sizeof(mod->date));
	time = CString(mod->time, sizeof(mod->time));
	date.TrimRight();
	if (!date.IsEmpty())
	{
		lastTime.Format("고객님의 최근 접속일: %s년%s월%s일 %s:%s:%s",
			date.Left(4), date.Mid(4, 2), date.Right(2), time.Left(2), time.Mid(2, 2), time.Right(2));
	}
	else 
		lastTime.Format("고객님의 최근 접속일 : ");
	m_MClient->SetLastTime(lastTime);
}

void CMainFrame::processTICK(CString dat)
{
	CString tmps, value, sym, yesym;
	int	pos;

	pos = dat.GetLength();
}

void CMainFrame::processINTER(char* dat)
{
	struct	_grid {
		char	code[12];		 /* 종목코드			 */
		char    hnam[20];                /* 종목명                       */
		char    curr[8];                 /* 현재가                       */
		char    diff[6];                 /* 전일대비      (9999V99)      */
		char    gvol[12];                /* 거래량                       */
	};

	struct  _interMod {
		char	keyf[2];		// ticker id
		char	nrec[3];		// count jcode
		struct	_grid	grid[1];	// data
	};
	
	CString		code, name, price, diff, gvol;
	_interMod*	interMod = (struct _interMod*) dat;
	int	cnt = atoi(CString(interMod->nrec, sizeof(interMod->nrec)));
	for (int ii = 0; ii < cnt; ii++)
	{
		code = CString(interMod->grid[ii].code, sizeof(interMod->grid[ii].code));	code.TrimRight();
		name = CString(interMod->grid[ii].hnam, sizeof(interMod->grid[ii].hnam));	name.TrimRight();
		price = CString(interMod->grid[ii].curr, sizeof(interMod->grid[ii].curr));	price.TrimRight();
		diff = CString(interMod->grid[ii].diff, sizeof(interMod->grid[ii].diff));	diff.TrimLeft();
		gvol = CString(interMod->grid[ii].gvol, sizeof(interMod->grid[ii].gvol));	gvol.TrimLeft();
// 		if (m_tInfo1)	m_tInfo1->SetInter((char*) &interMod->grid[ii]);
// 		if (m_tInfo2)	m_tInfo2->SetInter((char*) &interMod->grid[ii]);
	}
}

void CMainFrame::sendTicInfo()
{
	DWORD	dw;
	CString	file, section, keys;
	char	buf[1024*2], ticInfo[128];
	int	len, idx = 0, kk;
	int ii = 0;
	CMapStringToString	interAry;

	ZeroMemory(ticInfo, sizeof(ticInfo));
	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, GetUserName(), TICKSETUP);
	for (ii = 0; ii < TICKBAR_CNT; ii++)
	{
		keys.Format("BAR%02d", ii);
		int splitN = GetPrivateProfileInt("GENERAL", keys, SPLIT_CNT, file);
		for (int jj = 0; jj < splitN; jj++)
		{
			section.Format("BAR_INFO_%02d", jj + ii*10);
			switch (GetPrivateProfileInt(section, "INDEXK", TKIND_INDEX, file))
			{
			case TKIND_INDEX:
				{
					CString	value;
					for (kk = 0; ; kk++)
					{
						keys.Format("%03d", kk);
						dw = GetPrivateProfileString(section, keys, "", buf, sizeof(buf), file);
						if (dw <= 0)	break;
						len = GetTicKey(atoi(buf));
						ticInfo[len] = '1';
					}
				}
				break;
			case TKIND_UPDOWN:
				kk = GetPrivateProfileInt(section, "SELECT", 0, file);
				switch (kk)
				{
				case 0:	case 1:
					ticInfo[UPDN_KOSPI+kk] = '1';		// kospi or kosdaq
					break;
				case 2:
					ticInfo[UPDN_KOSPI] = '1';		// kospi
					ticInfo[UPDN_KOSDAQ] = '1';		// kosdaq
					break;
				default:
					ticInfo[UPDN_KOSPI] = '1';		// kospi
					break;
				}
				break;
			case TKIND_NEWS:
				ticInfo[NEWS_BASE] = '1';
				break;
			case TKIND_INTER:
				{
					CString	value;
					for (kk = 0; ; kk++)
					{
						keys.Format("%02d", kk);
						dw = GetPrivateProfileString(section, keys, "", buf, sizeof(buf), file);
						if (dw <= 0)	break;
						if (interAry.Lookup(buf, value))
							continue;
						interAry.SetAt(buf, "reg");
					}
				}
				break;
			default:
				continue;
			}
		}
	}
	
	struct _rtick {
		char	acts[1];	// 0 - unregister, 1- register
		char	code[3];	// ticker info;
	};
	struct _rtick* rtick = (struct _rtick *) buf;

	memset(buf, '0', sizeof(buf));
	for (ii = 1 ; ii < sizeof(ticInfo); ii++)
	{
		if (ticInfo[ii] == 0x00)
			continue;
		section.Format("%03d", ii);
		rtick[idx].acts[0] = '1';
		CopyMemory(rtick[idx++].code, section, sizeof(rtick->code));
	}

	if (!idx)
	{
		rtick[idx].acts[0] = '1';
		CopyMemory(rtick[idx++].code, "002", sizeof(rtick->code));
	}

	len = idx*sizeof(struct _rtick);
//	sendTR("CHBOTICK", buf, len, US_PASS, 't');
	if (interAry.GetCount())
	{
		const	int	L_code = 12;
		const	int	codeCnt = 5;/* 2 - key, 3 - code count */
		CString	codes;

		int	cnt = (int)interAry.GetCount();
		int	sndL = codeCnt + L_code*cnt + 1;
		char*	sndB = new char [sndL];
		ZeroMemory(sndB, sndL);
		len = idx = codeCnt;
		sprintf_s(sndB, sndL,"%05d", cnt);
		for (POSITION pos = interAry.GetStartPosition(); pos; )
		{
			interAry.GetNextAssoc(pos, codes, keys);
			if (codes.GetAt(0) == 'A')
				sprintf_s(&sndB[idx], sndL, "%s", codes.Mid(1));
			else
				sprintf_s(&sndB[idx], sndL, "%s", codes);
			idx += L_code;
		}
//		sendTR("CHBOTSIS", sndB, idx, US_PASS, 'I');
		delete [] sndB;
	}
	else
	{
		char*	sndB = new char [1];
		ZeroMemory(sndB, 1);
//		sendTR("CHBOTSIS", sndB, 1, US_PASS, 'I');
		delete [] sndB;
	}
}

void CMainFrame::sendConnectInfo()
{
	struct  _midCT {
		char    func[1];                /* Function 구분        */
		/* 'Q':조회, 'U':등록   */
		char    usid[8];                /* 사용자 ID            */
	};
	
	char	sndB[sizeof(struct _midCT) + 1];
	memset(sndB, ' ', sizeof(sndB));
	sndB[sizeof(struct _midCT)] = '\0';
	sprintf_s(sndB, sizeof(sndB), "Q%s", m_user);

	m_wizard->sendTR("CHHOCNCT", sndB, sizeof(sndB), US_PASS, 'c');
}

void CMainFrame::ParseSym(CString dat, CMapStringToString& ary)
{
	CString sym, value;
	int	pos;
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}

		ary.SetAt(sym, value);
	}
}

void CMainFrame::actionCaption(int key, int action)
{
	if (key == -1)	// for main caption
	{
		switch (action)
		{
		case MIDX_MIN:
			if (!m_bSDI)	
				PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
			else 
				ChangeTRAY();	
			break;
		case MIDX_RESTORE:	
				PostMessage(WM_SYSCOMMAND, SC_RESTORE);	
			break;
		case MIDX_MAX:
			if (!m_bSDI)	
			{
				PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);	

				CRect	wRc;
				GetWindowRect(wRc);
				int screenX = GetSystemMetrics(SM_CXMAXIMIZED);
				int screenY = GetSystemMetrics(SM_CYMAXIMIZED);
				
				int	x = wRc.left;
				int	y = wRc.top;
				int	cx = wRc.Width();
				int	cy = wRc.Height();

				if(wRc.Width() > screenX)
					SetWindowPos(NULL, x, y, screenX, screenY, SWP_NOZORDER|SWP_SHOWWINDOW);
				
				/*
				MONITORINFO lpmi ;
				memset(&lpmi,0,sizeof(MONITORINFO));
				lpmi.cbSize = sizeof(MONITORINFO);
				if(!GetMonitorInfo(NULL,  &lpmi ))
					return ;

				

				DISPLAY_DEVICE dd;
				DEVMODE dm;

				int MonitorCount =GetSystemMetrics(SM_CMONITORS);

				CRect* MonitorRect = NULL;
				MonitorRect = new CRect[MonitorCount];

				for(int i = 0, j = 0 ; i <MonitorCount; i++)
				{
					memset(&dd, 0,  sizeof(DISPLAY_DEVICE));
					dd.cb = sizeof(DISPLAY_DEVICE);

					EnumDisplayDevices(NULL, i, &dd, 0);
					memset(&dm, 0, sizeof(DEVMODE));
					if(EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
					{
						MonitorRect[j].left = dm.dmPosition.x;
						MonitorRect[j].top = dm.dmPosition.y;
						MonitorRect[j].right = dm.dmPosition.x + dm.dmPelsWidth;
						MonitorRect[j].bottom = dm.dmPosition.y + dm.dmPelsHeight;
						j++;
					}
				}
				*/
			}
			break;
		case MIDX_CLOSE:
		case MIDX_ICON:		PostMessage(WM_SYSCOMMAND, SC_CLOSE);		break;

		case MIDX_V1:
			m_iVS = action;
			m_pFormMgr->change_VirtualScreen(V_SCREEN1);
			break;
		case MIDX_V2:
			m_iVS = action;
			m_pFormMgr->change_VirtualScreen(V_SCREEN2);
			break;
		case MIDX_V3:
			m_iVS = action;
			m_pFormMgr->change_VirtualScreen(V_SCREEN3);
			break;
	//	case MIDX_V4:
	//		m_iVS = action;
	//		m_pFormMgr->change_VirtualScreen(V_SCREEN4);
	//		break;
		case MIDX_DUAL:
			if (!m_bSDI)
				fitDual();
		default:
			break;
		}
		return;
	}

	CChildFrame*	child  = NULL;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		actionSCaption(key, action);
		return;
	}

	int		value;
	LONG_PTR		rc = 0;
	CRect		sdiRc, mdiRc;
	CPoint		sp(-1, -1);
	CString		mapname;
	WINDOWPLACEMENT	pl;
	
	GetWindowPlacement(&pl);
	switch (action)
	{
	case IDX_SINGLE:	m_pFormMgr->convertSDI(key, m_vsN);		break;
	case IDX_GROUP:
		value = child->m_xcaption.ChangeGroup();
		if (value >= 0)	m_pFormMgr->set_Trigger(key, value);
		break;
	case IDX_COPY:
		child->GetWindowPlacement(&pl);
		value = GetSystemMetrics(SM_CYCAPTION);
		value += GetSystemMetrics(SM_CXFRAME);
		value += GetSystemMetrics(SM_CXBORDER);
		sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
		sp.Offset(value, value);

		value = child->m_xcaption.GetGroup();
		/*
		switch (value)
		{
		case 0:    //nongroup
		case 99:	break;  //allgroup
		case 1:case 2: case 3:case 4: case 5:
			value++;
			if (value > 5)	value = 1;
			break;
		default:	value = 1;	break;
		}		
		*/
		value = 1;  //그룹은 무조건 1
		CopyScreen(child->m_mapN, value, child->m_xcaption.GetFontSize(), sp);
		break;
	case IDX_DESIZE:
		{
			CRect rc;
			child->GetWindowRect(&rc);
			ScreenToClient(rc);
			rc.OffsetRect(-2, -71);
			child->SetWindowPos(NULL,rc.left, rc.top, child->m_width, child->m_height,  SWP_SHOWWINDOW);
		}
		break;
		/*
	case IDX_FONT:
		value = child->m_xcaption.ChangeFont();
		if (value > 0)
		{
			if (value == INITMAPSIZE)
			{
				child->m_xcaption.SetFontSize(m_fontSize);
				m_wizard->setFontSize(key, m_fontSize);
			}
			else
			{
				m_wizard->setFontSize(key, value);
			}
			child->m_xcaption.SetMax(false);
		}
		break;
		*/
	case IDX_MIN:
		sp = getMinimizePos(child->m_key);
		child->actionCaption(action, sp);
		MDINext();
		break;
	case IDX_RESTORE:
	case IDX_MAX:
		if(!IsDll(child->m_mapN))
			child->actionCaption(action, sp);	break;
	case IDX_CLOSE:
	case IDX_ICON:	
		{
			if(key == m_Bondkey)
			{
			//	delete m_pBondWnd;
				m_pFormMgr->closeChild(key);
				m_pBondWnd = NULL;
				m_Bondkey = -1;
			}
			else
				m_pFormMgr->closeChild(key);			
		}break;
	case IDX_CAPTURE :
	//	child->SetWindowPos(NULL,200, 200, 500, 500,  SWP_SHOWWINDOW);
		writeImg(m_home,0,0);
		break;
	default:
		break;
	}
}

void CMainFrame::actionSCaption(int key, int action, bool bPopup)
{
	CSChild*	schild = NULL;
	if (!m_arSDI[m_vsN].Lookup(key, schild))
		return;

	int		value;
	CRect		wRc;
	LONG_PTR		rc = 0;
	CPoint		sp(-1, -1);
	WINDOWPLACEMENT	pl;
	
	GetWindowPlacement(&pl);
	switch (action)
	{
	case IDX_PIN:
		if (!bPopup && getPopupMap(schild->m_mapN))
			return;
		
		if (schild->m_xcaption.changePin())
			schild->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		else
			schild->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		break;
	case IDX_SINGLE:	m_pFormMgr->convertMDI(key, m_vsN);		break;
	case IDX_GROUP:
		value = schild->m_xcaption.ChangeGroup();
		if (value >= 0)	m_pFormMgr->set_Trigger(key, value, m_vsN);
		break;
	case IDX_COPY:
		schild->GetWindowPlacement(&pl);
		value = XCAP_HEIGHT;
		value += GetSystemMetrics(SM_CXFRAME);
		value += GetSystemMetrics(SM_CXBORDER);
		sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
		sp.Offset(value, value);

		value = schild->m_xcaption.GetGroup();
		switch (value)
		{
		case 0:/*none group*/
		case 99:/*all group*/	break;
		case 1:case 2: case 3:case 4: case 5:
			value++;
			if (value > 5)	value = 1;
			break;
		default:	value = 1;	break;
		}		
		CopyScreen(schild->m_mapN, value, schild->m_xcaption.GetFontSize(), sp);
		break;
	//case IDX_FONT:
	case IDX_DESIZE:
		{
			CString strdata;
			CRect rc, temRc;
			schild->GetWindowRect(&rc);
			temRc = rc;

			ScreenToClient(temRc);

			rc.OffsetRect(-2, -71);
			schild->SetWindowPos(NULL,rc.left, rc.top, schild->m_width, schild->m_height,  SWP_SHOWWINDOW);
		}
		break;
		/*
		value = schild->m_xcaption.ChangeFont();
		if (value > 0)
		{
			if (value == INITMAPSIZE)
			{
				schild->m_xcaption.SetFontSize(m_fontSize);
				m_wizard->setFontSize(key, m_fontSize);

			}
			else
			{
				m_wizard->setFontSize(key, value);
			}
			schild->m_xcaption.SetMax(false);
		}
		*/
		break;
	case IDX_MIN:
		if (!schild->m_xcaption.IsMax())
		{
			schild->GetWindowRect(wRc);
			schild->m_restoreRc.CopyRect(&wRc);
		}
		schild->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
		break;
	case IDX_RESTORE:
		schild->SetRestore();
		break;
	case IDX_MAX:
		if(!IsDll(schild->m_mapN))
			schild->SetMax();
		break;
	case IDX_CLOSE:
	case IDX_ICON:	schild->PostMessage(WM_CLOSE);	break;
	case IDX_CAPTURE :
//	child->SetWindowPos(NULL,200, 200, 500, 500,  SWP_SHOWWINDOW);
	if(!m_bSDI)
		writeImg(m_home,0,0);
	break;
	default:
		break;
	}
}

void CMainFrame::GetDispN(char* mapN)
{
	if (!m_tMenu)	return;
	CString dispN = m_tMenu->GetDispN(mapN);
	sprintf_s(mapN, dispN.GetLength() * 2, dispN);
}

int CMainFrame::InputScreenNo(CString dispN)
{
	if (m_tMenu)
	{
		CString mapN = m_tMenu->GetMap(dispN);
		if (!ExistMenu(mapN))	return 0;
		return m_pFormMgr->ChangeChild(mapN);
	}
	return 0;
}

void CMainFrame::ConfigFrame()
{
	CString	file, cpfile, port;
	m_userN = GetUserName();

	file.Format("%s\\%s\\%s", m_home, USRDIR, m_userN);
	::CreateDirectory(file, NULL);

	file.Format("%s\\%s\\%s\\axisensetup.ini", m_home, USRDIR, m_userN);
	cpfile.Format("%s\\%s\\axisensetup.ini", m_home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_userN);
	//cpfile.Format("%s\\%s\\usertool.ini", m_home, MTBLDIR);
	cpfile.Format("%s\\%s\\USERTOOL.INI", m_home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_userN, TICKSETUP);
	cpfile.Format("%s\\%s\\%s", m_home, MTBLDIR, TICKSETUP);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\hkey.ini", m_home, USRDIR, m_userN);
	cpfile.Format("%s\\%s\\hkey.ini", m_home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\ordcfg.ini", m_home, USRDIR, m_userN);
	cpfile.Format("%s\\%s\\ordcfg.ini", m_home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\mngsetup.ini", m_home, USRDIR, m_userN);
	cpfile.Format("%s\\%s\\mngsetup.ini", m_home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	m_mngInfo = new CManageInfo(m_home, this);
	if (!m_mngInfo->Create(IDD_MNGINFO))
	{
		delete m_mngInfo;
		m_mngInfo = NULL;
	}
	else	m_mngInfo->Init();
	

	change_BackGround();
	CreateTB();
}

void CMainFrame::ProcessSecure()
{
	CString initmap = m_axis->GetProfileString(WORKSTATION, INITMAP);
	if (initmap.Find('#') != -1)
	{
		m_mapN = initmap.Mid(1);
	}
	else
		return;

	int	iHexa = atoi(m_mapN);
	int	chkBit = 0x01;
	CString	sFile, sItem;
	char	buf[32];
	sFile.Format("%s\\%s\\axis.ini", m_home, TABDIR);
	do
	{
		if (iHexa & chkBit)
		{
			sItem.Format("0x%02x", chkBit);
			GetPrivateProfileString("Secure", sItem, "", buf, sizeof(buf), sFile);

			m_MMapN = buf;
			if (m_MMapN.IsEmpty())
				return;

			if (CreateModal(m_MMapN, 0, winK_POPUP, 99) < 0)
			{
				m_MMapN = "";
				m_bExit = false;
				m_bSrnSave = false;
				PostMessage(WM_CLOSE);
				return;
			}
		}
		chkBit = chkBit << 1;
	}while(chkBit <= 0x80);
}

void CMainFrame::TotalSetup(int index, int tick)
{
	if(!index)
		index = IDD_DSETUP;

	CTotalSetup dlg(m_tMenu, m_home, GetUserName(), index, 0, m_bpcenable, 1 /* m_nWhoi*/);
	dlg.DoModal();
	/*  //--test
	CConfigDlg config(this, m_home);
	if (config.DoModal() == IDOK)
		load_eninfomation(false);
		*/
}

void CMainFrame::ESetup()
{
	TotalSetup(E_SETUP);
}

void CMainFrame::SetMng()
{
	TotalSetup(MNG_SETUP);
}

void CMainFrame::EditTool()
{
	TotalSetup(IDD_TSETUP);
}

void CMainFrame::EditHotkey()
{
	TotalSetup(HOTKEY_SETUP);
}

void CMainFrame::SetTicker(int id)
{
	TotalSetup(TICK_SETUP, id);
}

void CMainFrame::OrderSetup()
{
	TotalSetup(ORDER_SETUP);
}

void CMainFrame::ImageSetup()
{
	TotalSetup(IMAGE_SETUP);
}

BOOL CMainFrame::Change_VsKey(int index)
{
	if (0 > index || index >= 6)
		return FALSE;
	m_pFormMgr->change_VirtualScreen(index);
	return TRUE;
}

void CMainFrame::ConfigTicker(bool reconfig/*=false*/)
{
	CString	file;

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_userN, TICKSETUP);
}

void CMainFrame::CreateChaser()
{
	LONG_PTR	rc = 0;
	if (m_chaser)
	{
		m_wizard->setTrace(m_chaser->GetSafeHwnd());
		return;
	}

	m_chaser = new CChaser(this);
	if (!m_chaser->Create(NULL, "axis chaser", WS_CHILD, CRect(0), this, (int) m_chaser))
	{

		delete m_chaser;
		m_chaser = NULL;
		return;
	}

	m_wizard->setTrace(m_chaser->GetSafeHwnd());
}

void CMainFrame::DeleteChaser()
{
#ifndef _CHASERDEBUG

	if (m_wizard)
		m_wizard->setTrace(NULL);
	CloseChaserAPP();
#endif
}

void CMainFrame::CloseChaserAPP()
{
	CString	winCaption;
	winCaption.Format("AxisChaser for %s", m_regkey);
	CWnd* wnd = CWnd::FindWindow(NULL, winCaption);
	 if (wnd->GetSafeHwnd())
		::PostMessage(wnd->m_hWnd, WM_CLOSE, 0, 0);
}

void CMainFrame::showChaser()
{
	if (!m_runAxis)		return;

	CString	aps, cmds, exes;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	CAxisApp* app = (class CAxisApp *) AfxGetApp();
	char	buffer[1024];
	GetClassName(m_hWnd, buffer, sizeof(buffer));
	cmds.Format(" /c %s /r \"%s\"", buffer, m_regkey);
	aps.Format("%s\\%s\\AxisChaser.exe", m_home, RUNDIR);
	CreateChaser();
	
	BOOL bRc = CreateProcess(
			aps,				// application name
			(char *)cmds.operator LPCTSTR(),// command line
			NULL,				// process attribute
			NULL,				// thread attribute
			FALSE,				// is inherit handle
			0,				// creation flags
			NULL,				// environment
			NULL,				// current directory
			&si,				// STARTUPINFO
			&pi);				// PROCESS_INFORMATION
			
}

LRESULT CMainFrame::OnCHASER(WPARAM wParam, LPARAM lParam)
{
	m_sync.Lock();
	int	len = L_cds + LOWORD(wParam);
	char	bufx[1024*128];

	ZeroMemory(bufx, sizeof(bufx));
	struct _exeCDSS* cds = (struct _exeCDSS *) bufx;

	cds->flag = HIWORD(wParam);
	cds->len  = LOWORD(wParam);
	CopyMemory(&bufx[L_cds], (char *) lParam, len);

	COPYDATASTRUCT cs;
	cs.cbData = len + L_cds;
	cs.dwData = 0;
	cs.lpData = bufx;

	CString	winCaption;
	winCaption.Format("AxisChaser for %s", m_regkey);
	CWnd* wnd = CWnd::FindWindow(NULL, winCaption);
	 if (wnd->GetSafeHwnd())
		::SendMessage(wnd->m_hWnd, WM_COPYDATA, 0, (LPARAM) &cs);
	m_sync.Unlock();
	return 0;
}

LRESULT CMainFrame::OnAxisClose(WPARAM wParam, LPARAM lParam)
{
	CString	str, msg, title, logs;
	
	switch (m_step)
	{
	case axOPENRSM:
		m_axConnectD->SetProgress(false);
	case axOPENWSH:
	case axOPENSIGN:
		m_step = axNONE;
		m_axConnectD->SetStatus(SM_EXIT);
		if (m_axConnectD->GetStyle() & WS_VISIBLE)
			m_axConnectD->EndDialog(IDCANCEL);

		msg.LoadString(ST_MSG_DISCONNECT_EXIT);
		title.LoadString(ST_TEXT_PROGRAMEXIT);
		::MessageBox(m_hWnd, msg, title, MB_ICONINFORMATION);
		break;
	case axNONE:
		msg.LoadString(ST_MSG_LOGINERROR);
		title.LoadString(ST_TEXT_OK);
		if (m_axMisc->MsgBox(msg, title, MB_OKCANCEL) == IDOK)
		{
			if (!m_axMisc->RunVers(verRETRY, m_user, m_pass, m_cpass))
				return 0;
		}
		
		if (m_axConnectD && IsWindow(m_axConnectD->GetSafeHwnd()) && m_axConnectD->ContinueModal())
			m_axConnectD->EndDialog(IDCANCEL);
		break;
	case axDONE:
		msg.LoadString(ST_MSG_DISCONNECT_RETRY);
		title.LoadString(ST_TEXT_OK);
		if (m_axMisc->MsgBox(msg, title, MB_OKCANCEL) == IDOK)
		{
			if (!m_axMisc->RunVers(verRETRY, m_user, m_pass, m_cpass))
				return 0;
		}
		
		break;
	}
	m_step = axCLOSE;
	if (GetSafeHwnd())	PostMessage(WM_CLOSE);
	return 1;
}

LRESULT CMainFrame::OnTrayEvent(WPARAM wParam, LPARAM lParam)
{
	UINT	msg = LOWORD(lParam);
	switch (msg)
	{
	case WM_RBUTTONDOWN:	// display menu
		{
			CPoint pt;
			GetCursorPos(&pt);
			m_tMenu->ShowFullMenu(pt, true);
		}
		break;
	case WM_LBUTTONDBLCLK:	// restore SDI 
		m_appMode = MODE_SDI;
		TrayCmd(TRAY_HIDEICON);
		ShowWindow(SW_SHOW);
		break;
	}
		
	return 1;
}

void CMainFrame::Action(WPARAM wParam, LPARAM lParam)
{
	int	key = LOWORD(wParam);
	int	actK = HIWORD(wParam);
	
	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
		return;

	//	Group(0);Copy(1);Help(2);Font(3);Min(4);Max(5);Close(6)
	switch (actK)
	{
	case 1:	actionCaption(key, 2);	break;
	case 2:	actionCaption(key, 1);	break;
	case 4:	child->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);	break;
	case 5:	child->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);	break;
	case 6:	m_pFormMgr->closeChild(child->m_key);			break;
	default:
		if (actK >= GROUPBASE && actK < FONTBASE)
		{
			actK -= GROUPBASE;
			switch (actK)
			{
			case 0:case 1:case 2:case 3: case 4:case 5:case 99:
				m_pFormMgr->set_Trigger(key, actK);	break;
			default:	break;
			}
		}
		else if (actK >= FONTBASE)
		{
			actK -= FONTBASE;
			LONG_PTR	rc = 0;
			switch (actK)
			{
			case 8:case 9:case 10:case 11:case 12:
				child->m_xcaption.SetFontSize(actK);
				m_wizard->setFontSize(key, actK);

				break;
			default:
				break;
			}
		}
		return;
	}
}

void CMainFrame::GetCaptionInfo(WPARAM wParam, LPARAM lParam)
{
	int	infoK =  LOWORD(wParam);
	int	key = HIWORD(wParam);
	char*	buf = (char *) lParam;
	
	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		sprintf_s((char *) lParam, sizeof(buf), "-1");
		return;
	}

	switch(infoK)
	{
	case 0:	//	group
		sprintf_s(buf, sizeof(buf),"%d", child->m_xcaption.GetGroup());
		break;
	case 1:
		sprintf_s(buf, sizeof(buf),"%d", child->m_xcaption.GetFontSize());
		break;
	default:
		break;
	}
}

// must to call after call SetMisc (..)
void CMainFrame::HideChildButton(int key)
{
	CString		file;

	CChildFrame*	child;
	CSChild*	schild;
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		if (!m_arMDI[vsN].Lookup(key, child))
		{
			if (!m_arSDI[vsN].Lookup(key, schild))
				continue;
			if (IsDll(schild->m_mapN))
				schild->m_xcaption.HideButton(HIDE_FONT);
			int value = IsHideCaption(schild->m_mapN);
			if (value)	schild->m_xcaption.HideButton(value);
			break;
		}
		if (IsDll(child->m_mapN))
			child->m_xcaption.HideButton(HIDE_FONT);
		int value = IsHideCaption(child->m_mapN);
		if (value)	child->m_xcaption.HideButton(value);
	}
}

void CMainFrame::changePassword(CString newpassword)
{
	int ii = 0;
	newpassword.TrimRight();
	m_pass = newpassword;

	LONG_PTR	rc = 0;
	CString	data = m_wizard->loadAcc();
	if (data.IsEmpty())	return;

	int		pos;
	Caccount*	acc;
	CString		tmps, tmpx;
	CArray <Caccount*, Caccount* > acs;

	for ( ; !data.IsEmpty(); )
	{
		pos = data.Find('\n');
		if (pos < 0)
			break;

		tmps = data.Left(pos);
		data = (data.GetLength() > pos+1) ? data.Mid(pos+1) : _T("");

		acc = new Caccount;
		for (int ii = 0; ii < 3; ii++)
		{
			pos = tmps.Find('\t');
			if (pos >= 0)
			{
				tmpx = tmps.Left(pos);
				tmps = (tmps.GetLength() > pos+1) ? tmps.Mid(pos+1) : _T("");
			}
			else
			{
				tmpx = tmps;
				tmps = _T("");
			}

			switch (ii)
			{
			case 0:	acc->m_account = tmpx;	break;
			case 1:	acc->m_pass = tmpx;	break;
			case 2:	acc->m_name = tmpx;	break;
			}
		}

		if (acc->m_account.GetLength() != 10)
		{
			delete acc;
			continue;
		}
		acs.Add(acc);
	}
	
	for (ii = 0; ii < acs.GetSize(); ii++)
	{
		rc = 0;
		acc = acs.GetAt(ii);
		if (acc->m_name.IsEmpty())
			continue;
		data.Format("%s\t%s\t%s", acc->m_account, m_pass, acc->m_name);	
		m_wizard->saveAcc(data);	
	
	}

	for (ii = 0; ii < acs.GetSize(); ii++)
		delete acs.GetAt(ii);
	acs.RemoveAll();
}

CPoint CMainFrame::getMinimizePos(int childKey)
{
	int		key, cnt;
	CRect		iconicRc, cRc, mRc;
	CPoint		pt;
	POSITION	pos;
	CChildFrame*	child = NULL;
	CArray < CChildFrame*, CChildFrame* > ary;

	m_arMDI[m_vsN].Lookup(childKey, child);
	if (child->m_iconpos.x != -1 || child->m_iconpos.y != -1)
		return child->m_iconpos;

	m_MClient->GetClientRect(cRc);
	int cx = GetSystemMetrics(SM_CXMINIMIZED);
	int cy = GetSystemMetrics(SM_CYMINIMIZED);

	mRc.SetRect(cRc.left, cRc.bottom - cy, cRc.left + cx, cRc.bottom);
	pt = mRc.TopLeft();
	for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
		if (key == childKey)	continue;
		if (!child->m_bIconic)	continue;
		ary.Add(child);
	}

	bool	matchpoint = false;

	cnt = (int)ary.GetSize();
	while (cnt)
	{
		for (int ii = 0; ii < cnt; ii++)
		{
			child = ary.GetAt(ii);
			child->GetWindowRect(iconicRc);

			m_MClient->ScreenToClient(&iconicRc);
			iconicRc.IntersectRect(mRc, iconicRc);
			if (!iconicRc.IsRectEmpty())
			{
				matchpoint = true;
				break;
			}
		}
		
		if (matchpoint)
		{
			mRc.OffsetRect(cx, 0);
			if (mRc.right > cRc.right)
			{
				mRc.OffsetRect(0, -cy);
				mRc.left = cRc.left;
				mRc.right = mRc.left + cx;
			}
			pt = mRc.TopLeft();
			matchpoint = false;
		}
		else	cnt = 0;
	}

	return pt;
}

CView* CMainFrame::GetNewView(int vwKIND)
{
	POSITION pos = m_axis->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate;
	for (int ii = 0; ii < vwKIND; ii++)
		pTemplate = m_axis->GetNextDocTemplate(pos);

	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CDocument* pDocument = pTemplate->OpenDocumentFile(NULL);
	ASSERT_VALID(pDocument);

	CAxisDoc* doc = (CAxisDoc *) pDocument;
	pos = pDocument->GetFirstViewPosition();
	return pDocument->GetNextView(pos);	
}

CView* CMainFrame::GetSDIView(int vwKIND)
{
	CString strWndClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, ::LoadCursor(NULL, IDC_ARROW),
			      (HBRUSH) ::GetStockObject(WHITE_BRUSH), m_axisres->m_hIcon);

	int col = (int)m_arSDI[m_vsN].GetCount()/10;
	int row = (int)m_arSDI[m_vsN].GetCount()%10;
	col *= XCAP_HEIGHT;	row *=	XCAP_HEIGHT;
	CSChild*	schild = new CSChild(this);
	if (m_bSDI)
	{
		if(!schild->Create(strWndClass, AXISKEY, WS_OVERLAPPEDWINDOW, CRect(col+row, row, col+row, row), NULL,NULL,WS_EX_TOOLWINDOW | WS_EX_APPWINDOW,NULL))
		{
			delete schild;
			return NULL;
		}
	}
	else
	{
	//	if(!schild->Create(strWndClass, AXISKEY, WS_OVERLAPPEDWINDOW, CRect(col+row, row, col+row, row), this,NULL,0,NULL))
		if(!schild->Create(strWndClass, AXISKEY, WS_OVERLAPPEDWINDOW, CRect(col+row, row, col+row, row), this,NULL,WS_EX_TOOLWINDOW | WS_EX_APPWINDOW,NULL))
		{
			delete schild;
			return NULL;
		}
	}
	schild->SetOwner(this);	//확인사항
	
	CCreateContext	context;
	CRuntimeClass*	pNewViewClass;
	switch (vwKIND)
	{
	default:
	case NORMALVIEW:	pNewViewClass = RUNTIME_CLASS(CAxisView);	break;
	case SCROLLVIEW:	pNewViewClass = RUNTIME_CLASS(CAxScrollView);	break;
	case TABLISTVIEW:	pNewViewClass = RUNTIME_CLASS(CAxTabView);	break;
	}

	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = GetActiveDocument();

	CView*	view = schild->AttachView(context);

	CRect	vRc;
	view->GetWindowRect(vRc);
	return view;
}

CSize CMainFrame::GetFrameGap(bool mdichild/*= true*/)
{
	int	gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
	int	gapY  = GetSystemMetrics(SM_CYFRAME) * 2;

	if (mdichild)
	{
		gapX += GetSystemMetrics(SM_CXBORDER) * 2;
		gapY += GetSystemMetrics(SM_CYBORDER) * 2;
	}
	else
	{
		gapX += GetSystemMetrics(SM_CXBORDER) * 4;
		gapY += GetSystemMetrics(SM_CYBORDER) * 4;
	}

	return CSize(gapX, gapY);
}

void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (m_tMenu)
		m_tMenu->MeasureMenuItem(nIDCtl,lpMeasureItemStruct); 
}

void CMainFrame::ShowUScreenMenu()
{
	return;
	if (!m_bar1)	return;

	char		wb[512];
	CMenuXP		menu;
	CString		file, tmps, value;

	menu.CreatePopupMenu();
	file.Format("%s\\%s\\%s\\%s.ini", m_home, USRDIR, m_userN, m_userN); 

	CMapStringToString	ary;
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		DWORD dwRc = GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), file);
		if (dwRc <= 0)	break;

		tmps = wb;
		if (ary.Lookup(tmps, value))	continue;
		ary.SetAt(tmps, "1");
		tmps.TrimLeft(); tmps.TrimRight();
		menu.AppendMenuX(MF_STRING|MF_ENABLED, ID_MENU_USER+ii, tmps);
	}
	ary.RemoveAll();

	CPoint	pt = m_bar1->GetPos(2);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
}

CString CMainFrame::GetSkinName()
{
	char		buffer[128];
	CString		file, skin;

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	DWORD dw = GetPrivateProfileString(GENERALSN, "Palette", "", buffer, sizeof(buffer), file);
	if (dw <= 0)	skin = _T("");
	else		skin = buffer;

	skin.TrimLeft();
	skin.TrimRight();
	return skin;
}

void CMainFrame::AppSwitch()
{
	if (m_bSDI)
		ChangeMDI();
	else	
		ChangeSDI();
}

void CMainFrame::SetToolStatus()
{
	m_toolStatus = 0;
	if (m_bar2->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_LISTBAR;
	if (m_sdibar->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_SDIBAR;
	if (m_smain->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_SDIMAINBAR;
}

void CMainFrame::SetSDIHeight()
{
	m_SDIHEIGHT = HEIGHT_CTRLBAR1 - 2/*border size*/;		// toolbar height
	m_SDIHEIGHT += GetSystemMetrics(SM_CYFRAME);
	m_SDIHEIGHT += GetSystemMetrics(SM_CYBORDER)*2;
	m_SDIHEIGHT += LISTBAR_HEIGHT - 2 + 10/*border size*/;

	TRACE("CAPTION DATA[%d]\n", GetSystemMetrics(SM_CYCAPTION));

	if (GetSystemMetrics(SM_CYFRAME) == 8)	m_SDIHEIGHT += 4;	// vista

	m_SDIHEIGHT = (m_SDIHEIGHT * 1 - 4) + (GetSystemMetrics(SM_CYCAPTION) + 1);


	int	width;
	width = GetSystemMetrics(SM_CXFULLSCREEN);
	m_SDIHEIGHT  += 15;
	m_SDIHEIGHT  += 22;
	SetWindowPos(&wndTop, 0, 0, width, m_SDIHEIGHT, SWP_NOZORDER);
}

void CMainFrame::SetSDIChangeHeight()
{
	if (!m_bSDI)	return;
	
	m_SDIHEIGHT = HEIGHT_CTRLBAR1 - 2/*border size*/;		// toolbar height
	m_SDIHEIGHT += GetSystemMetrics(SM_CYFRAME);
	m_SDIHEIGHT += GetSystemMetrics(SM_CYBORDER)*2;

	if (m_bar2->GetStyle() & WS_VISIBLE)
	{
		m_toolStatus |= TB_LISTBAR;
		m_SDIHEIGHT += LISTBAR_HEIGHT - 2/*border size*/;
	}
	else	m_toolStatus &= ~TB_LISTBAR;

	if (GetSystemMetrics(SM_CYFRAME) == 8)	m_SDIHEIGHT += 4;	// vista

	WINDOWPLACEMENT pl;
	GetWindowPlacement(&pl);
	int	width = pl.rcNormalPosition.right - pl.rcNormalPosition.left;
	SetWindowPos(&wndTop, 0, 0, width, m_SDIHEIGHT, SWP_NOMOVE|SWP_NOZORDER);
}

void CMainFrame::displayGuide(CString guide, CWnd* owner)
{
	if (!m_bGuideSet)
		return;

	if (m_bSDI)
	{
		if (m_bGuideTimer)
			m_axGuide->SetTime(m_nGuideInterval);
		CWnd* pWnd = GetActiveWindow();
		if (!guide.IsEmpty())
			m_axGuide->SetGuide(guide, pWnd);
	}
	else
	{
		if (m_bGuideTimer)
			m_axGuide->SetTime(m_nGuideInterval);
		CChildFrame*	child = (CChildFrame *) MDIGetActive();
		m_axGuide->SetGuide(guide, child);
	}
}

void CMainFrame::displayMsgBox(CString caption, CString guide)
{
	::MessageBox(m_hWnd, guide, caption, MB_OK);
}

void CMainFrame::SetHome()
{
	return;//
	if (m_tMenu)	
		m_tMenu->SetHome();
}

bool CMainFrame::IsDll(CString mapN)
{
	CString	file, file2, sMapN;
	char	wb[128];
	DWORD	dw = 0;
	file.Format("%s\\%s\\vtcode.ini", m_home, TABDIR);
	file2.Format("%s\\%s\\axis.ini", m_home, TABDIR);

	if (mapN.GetLength() >= L_MAPN)
	{
		sMapN = mapN.Mid(2, 4);
		dw = GetPrivateProfileString("SELECT", sMapN, "", wb, sizeof(wb), file2);
		if (dw > 0)
		{
			sMapN = wb;	sMapN.TrimLeft();	sMapN.TrimRight();
			mapN = mapN.Left(2) + sMapN;
		}
	}

	dw = GetPrivateProfileString("DLL Controls", mapN, "", wb, sizeof(wb), file);
	if (dw > 0)	
		return true;

	dw = GetPrivateProfileString("GRX Controls", mapN, "", wb, sizeof(wb), file);
	if (dw > 0)	
		return true;

	return false;
}

int CMainFrame::IsHideCaption(CString mapN)
{
	CString	file;
	file.Format("%s\\%s\\axis.ini", m_home, TABDIR);

	int value = GetPrivateProfileInt("Caption", mapN, 0, file);
	return value;
}

void CMainFrame::keyArray(CArray <int, int> &ary, int vsN, bool bSDI/*=false*/)
{
	CArray	<int, int>	*aryZ;

	if (bSDI)
		aryZ = &m_sdiZORDER[vsN];
	else	
		aryZ = &m_hooklist[vsN];

	for (int ii = (int)aryZ->GetSize() - 1; 0 <= ii; ii--)
	{
		int key = aryZ->GetAt(ii);
		ary.Add(key);
	}
}

void CMainFrame::LinkOPEN(int dat, CString mapN)
{
	int	key   = HIWORD(dat);
	int	group = LOWORD(dat);
	CString	data, mapname;

	getScreenData(key, data, FALSE);
	data.Replace(";", "\n");
	mapname.Format("%s%s", mapN, data);
	m_pFormMgr->ChangeChild(mapname, group);
}

void CMainFrame::changeLinkInfo(class CLinkbar* linkBar, CString mapname, bool bKeepbar)
{
	if (!linkBar || mapname.GetLength() < L_MAPN)	return;
	CFrameWnd* parent = (CFrameWnd*) linkBar->GetParent();
	if (!bKeepbar)
	{
		if (isShowLinkbar(mapname))
			parent->ShowControlBar(linkBar, TRUE, FALSE);
		else	parent->ShowControlBar(linkBar, FALSE, FALSE);
	}
	linkBar->ChangeInfo(mapname);
}

void CMainFrame::changeAllLinkInfo(CString mapname)
{
	CChildFrame*	child;
	CSChild*	schild;
	CView*		view;
	CRect		wRc;
	bool		change;
	int		key;
	POSITION pos;

	UINT	nFlags = SWP_NOMOVE|SWP_NOZORDER;
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		for (pos = m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_arMDI[vsN].GetNextAssoc(pos, key, child);
			if (getSMap(child->m_mapN.Left(L_MAPN)) != getSMap(mapname))
				continue;
		
			change = false;
			changeLinkInfo(child->m_linkBar, mapname, true);
			child->GetWindowRect(wRc);
			if (isShowLinkbar(mapname))
			{
				if (!(child->m_linkBar->GetStyle() & WS_VISIBLE))
				{
					change = true;
					wRc.bottom += LBAR_HEIGHT;
					child->m_height += LBAR_HEIGHT;
					child->ShowControlBar(child->m_linkBar, TRUE, FALSE);
					child->SetWindowPos(NULL, 0, 0, wRc.Width(), wRc.Height(), nFlags);
				}
			}
			else
			{
				if (child->m_linkBar->GetStyle() & WS_VISIBLE)
				{
					change = true;
					wRc.bottom -= LBAR_HEIGHT;
					child->m_height -= LBAR_HEIGHT;
					child->ShowControlBar(child->m_linkBar, FALSE, FALSE);
					child->SetWindowPos(NULL, 0, 0, wRc.Width(), wRc.Height(), nFlags);
				}
			}

			if (child->m_tView && change)
			{
				int	vtype = 0, size = 0;
				view = child->GetActiveView();
				vtype = m_wizard->getFormInfo(mapname, size);


				((CAxTabView *)view)->SizeWindow(CSize(LOWORD(size), HIWORD(size)));
			}
		}

		for (pos = m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			if (getSMap(schild->m_mapN.Left(L_MAPN)) != getSMap(mapname))
				continue;
		
			change = false;
			changeLinkInfo(schild->m_linkBar, mapname, true);
			schild->GetWindowRect(wRc);
			if (isShowLinkbar(mapname))
			{
				if (!(schild->m_linkBar->GetStyle() & WS_VISIBLE))
				{
					change = true;
					wRc.bottom += LBAR_HEIGHT;
					schild->m_height += LBAR_HEIGHT;
					schild->ShowControlBar(schild->m_linkBar, TRUE, FALSE);
					schild->SetWindowPos(NULL, 0, 0, wRc.Width(), wRc.Height(), nFlags);
				}
			}
			else
			{
				if (schild->m_linkBar->GetStyle() & WS_VISIBLE)
				{
					change = true;
					wRc.bottom -= LBAR_HEIGHT;
					schild->m_height -= LBAR_HEIGHT;
					schild->ShowControlBar(schild->m_linkBar, FALSE, FALSE);
					schild->SetWindowPos(NULL, 0, 0, wRc.Width(), wRc.Height(), nFlags);
				}
			}

			if (schild->m_tView && change)
			{
				int	vtype = 0, size = 0;
				view = schild->GetActiveView();
				vtype = m_wizard->getFormInfo(mapname, size);
				((CAxTabView *)view)->SizeWindow(CSize(LOWORD(size), HIWORD(size)));
			}
		}
	}
}

bool CMainFrame::isShowLinkbar(CString mapN)
{ 
	if (!m_linkbar)		return false;

	CString		file;
	if (mapN.GetLength() < L_MAPN)	return false;

	CString	tail = mapN.Mid(6, 2);
	if (tail != "00")	return false;
	
	file.Format("%s\\%s\\linkbar.ini", m_home, TABDIR);
	if (GetPrivateProfileInt("SHOWBAR", mapN.Left(L_MAPN), 1, file))
		return true;
	return false;
}

void CMainFrame::parseAry(CString data, CStringArray& ary, CString separate)
{
	CString	value;
	while (!data.IsEmpty())
	{
		int pos = data.Find(',');
		if (pos == -1)
		{
			data.TrimLeft();
			data.TrimRight();
			ary.Add(data);
			break;
		}
		value = data.Left(pos++);
		data = data.Mid(pos);
		value.TrimLeft();
		value.TrimRight();
		ary.Add(value);
	}
}

void CMainFrame::refreshMap(int key)
{
	int		size, vwTYPE, nCx, nCy;
	BOOL		rc = FALSE;
	CSize		frameGap;
	CChildFrame*	child;
	CSChild*	schild;
	CString		mapname;

	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		if (!m_arSDI[m_vsN].Lookup(key, schild))	return;

		mapname = schild->m_mapN.Left(L_MAPN);
		vwTYPE = m_wizard->getFormInfo(mapname, size);

		frameGap = GetFrameGap();
		if (isShowLinkbar(mapname))		
			frameGap.cy += LBAR_HEIGHT;
		frameGap.cy += XCAP_HEIGHT;

		schild->m_width  = nCx = LOWORD(size) + frameGap.cx;
		schild->m_height = nCy = HIWORD(size) + frameGap.cy;
		
		if (!m_wizard->loadMap(mapname, key, size))	return;

		nCx = schild->m_width;
		nCy = schild->m_height;
		
		schild->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_NOZORDER);

		return;
	}
	
	mapname = child->m_mapN.Left(L_MAPN);
	vwTYPE = m_wizard->getFormInfo(mapname, size);

	frameGap = GetFrameGap();
	if (isShowLinkbar(mapname))		frameGap.cy += LBAR_HEIGHT;

	frameGap.cy += XCAP_HEIGHT;

	child->m_width  = nCx = LOWORD(size) + frameGap.cx;
	child->m_height = nCy = HIWORD(size) + frameGap.cy;

	if (!m_wizard->loadMap(mapname, key, size))	return;

	nCx = child->m_width;
	nCy = child->m_height;
	
	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_NOZORDER);

}

void CMainFrame::ChangeMDI()
{
	GetWindowPlacement(&m_sdipl);

	int			key;
	CArray	<int, int>	ary;
	
	m_bSDI	  = false;
	m_appMode = MODE_MDI;

	ModifyStyle(0, WS_CAPTION);
	ShowControlBar(m_tMenu, TRUE, FALSE);

	ShowControlBar(m_sdibar, FALSE, FALSE);
	ShowControlBar(m_smain, FALSE, FALSE);

	SetWindowPlacement(&m_switchpl);
	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		keyArray(ary, vsN, true);
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			key = ary.GetAt(ii);
			m_pFormMgr->convertMDI(key, vsN);
		}
		ary.RemoveAll();
	}
	TrayCmd(TRAY_HIDEICON);
	m_bar2->SetMDI();
}

void CMainFrame::ChangeSDI(bool hide)
{
	int			key;
	CArray	<int, int>	ary;
	CSChild*		schild;
	int vsN;
	m_bSDI	  = true;
	m_appMode = hide ? MODE_TRAY : MODE_SDI;

	GetWindowPlacement(&m_switchpl);
	ShowWindow(SW_HIDE);

	if (m_switchpl.showCmd == SW_SHOWMAXIMIZED)
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);

	for (vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		keyArray(ary, vsN);
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			key = ary.GetAt(ii);
			m_pFormMgr->convertSDI(key, vsN);
		}
		ary.RemoveAll();
	}

	for (vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		for (POSITION pos = m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			schild->m_xcaption.SetSDI();
		}
	}

	SetSDIHeight();

	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);
	CPoint	sdiPt = CPoint(pl.rcNormalPosition.left, m_switchpl.rcNormalPosition.top + m_SDIHEIGHT);

	if (!hide)	
		ShowWindow(SW_SHOW);
	TrayCmd(TRAY_HIDEICON);
	m_smain->Refresh();
	m_sdibar->Refresh();
	ShowControlBar(m_bar2, TRUE, FALSE);

	m_bar2->SetSDI();
}

void CMainFrame::convertSDI(int key, int vsN)
{
//	m_pFormMgr->TransformMDIChildToSingleWindow(key, vsN);
}

void CMainFrame::ChangeTRAY()
{
	PostMessage(WM_CANCELMODE);

	ShowWindow(SW_HIDE);
	m_appMode = MODE_TRAY;
	TrayCmd(TRAY_SHOWICON);
}

void CMainFrame::TrayCmd(int cmd)
{
	m_tray.cbSize	= sizeof(NOTIFYICONDATA);
	m_tray.hWnd	= GetSafeHwnd();
	m_tray.uID	= IDI_CLIENT;
	m_tray.hIcon	= m_axisres->m_hTray;
	m_tray.uCallbackMessage = WM_TRAYEVENT;

	switch (cmd)
	{
	case TRAY_SHOWICON:
		m_tray.uFlags	= NIF_MESSAGE | NIF_ICON | NIF_TIP;
		strcpy_s(m_tray.szTip, m_regkey);

		Shell_NotifyIcon(NIM_ADD, &m_tray);
		break;
	case TRAY_HIDEICON:
		m_tray.uFlags = 0;
		Shell_NotifyIcon(NIM_DELETE, &m_tray);
		break;
	default:return;
	}
}

void CMainFrame::drawTitle(CDC* pDC)
{
	CDC		memDC;
	CRect		cRc, iRc, tRc, clipRc;
	int		round;

	cRc = GetCapRect();
	tRc.CopyRect(&cRc);
	round		= cRc.Height()/2;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(pDC, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		DrawTitleBitmap(&memDC, CRect(0, 0, cRc.Width(), cRc.Height()), m_activeCap);

		if (m_axisres)
		{
// draw icon
			iRc = m_arRc.GetAt(MIDX_ICON);
			if (iRc.IsRectEmpty())
			{
				SetRegion();
				iRc = m_arRc.GetAt(MIDX_ICON);
			}
			DrawBitmapByMask(&memDC, MIDX_ICON, iRc, false, m_activeCap);
			
// draw system button
			iRc = m_arRc.GetAt(MIDX_CLOSE);	
			DrawBitmapByMask(&memDC, MIDX_CLOSE, iRc, false, m_activeCap);

		
			int ii = 0;
	//		for(ii = MIDX_V1 ; ii <= MIDX_V4; ii++)
			for(ii = MIDX_V1 ; ii <= MIDX_V3; ii++)
			{
				iRc = m_arRc.GetAt(ii);
				
				if (m_iVS == ii)
					DrawBitmapByMask(&memDC, ii, iRc, true, m_activeCap);
				else
					DrawBitmapByMask(&memDC, ii, iRc, false, m_activeCap);				
			}
		
			if (IsZoomed())
			{
				iRc = m_arRc.GetAt(MIDX_RESTORE);
				DrawBitmapByMask(&memDC, MIDX_RESTORE, iRc, false, m_activeCap);
			}
			else
			{
				iRc = m_arRc.GetAt(MIDX_MAX);
				DrawBitmapByMask(&memDC, MIDX_MAX, iRc, false, m_activeCap);
			}

			iRc = m_arRc.GetAt(MIDX_MIN);	
			DrawBitmapByMask(&memDC, MIDX_MIN, iRc, false, m_activeCap);

			iRc = m_arRc.GetAt(MIDX_DUAL);	
			DrawBitmapByMask(&memDC, MIDX_DUAL, iRc, false, m_activeCap);
		
// draw text
			tRc.top	  = 0;
			tRc.left  = m_arRc.GetAt(MIDX_ICON).right + GAP;
			tRc.right = m_arRc.GetAt(MIDX_MIN).left - GAP;
			tRc.bottom= tRc.top + cRc.Height();
			CFont*	font = memDC.SelectObject(m_axisres->GetFont());
		
			if (m_activeCap)
				memDC.SetTextColor(RGB(255, 255, 255));
			else	memDC.SetTextColor(RGB(192, 192, 192));
		
			CString	title;
			GetWindowText(title);
			memDC.SetBkMode(TRANSPARENT);

			memDC.DrawText(title, tRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
			
		}

		pDC->BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);

		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
}

CRect CMainFrame::GetCapRect()
{
	// Get size of frame around window
	CString tmpS;
	CRect	rc;
	DWORD dwStyle = GetStyle();
	CSize szFrame = CSize(GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME));

	int cxIcon = GetSystemMetrics(SM_CXSIZE);		// width of caption icon/button

	// Compute rectangle
	GetWindowRect(&rc);					// window rect in screen coords
	rc -= rc.TopLeft();					// shift origin to (0,0)
	rc.left  += szFrame.cx;					// frame
	rc.right -= szFrame.cx;					// frame
	rc.top   += szFrame.cy;					// top = end of frame
	rc.bottom = rc.top + GetSystemMetrics(SM_CYCAPTION)	// height of caption
		- GetSystemMetrics(SM_CYBORDER);		// minus gray shadow border

	rc.bottom += 3;
	return rc;
}

void CMainFrame::SetRegion()
{
	if (!m_axisres)	
		return;
	CRect	cRc, iRc, xRc;
	CPoint	pt;
	BITMAP	bm, bmv;

	cRc = GetCapRect();
	m_axisres->GetBmp(IDX_MIN)->GetBitmap(&bm);

	pt = CPoint(cRc.right - bm.bmWidth/4, (cRc.Height() - bm.bmHeight)/2);
	iRc.SetRect(pt.x, pt.y, pt.x + bm.bmWidth/4, pt.y + bm.bmHeight);

	if (DisplaySystemVersion() >= osVISTA)
		iRc.OffsetRect(-GAP*5, 0);
	else 
		iRc.OffsetRect(-GAP*3, 0);

	xRc.CopyRect(iRc);
	xRc.left -= 1;

	int ii = 0;
	for (ii = MIDX_CLOSE; ii > MIDX_ICON; ii--)
	{
		switch (ii)
		{
		case MIDX_MIN:
			m_arRc.SetAt(MIDX_MIN, iRc);
			break;
		case MIDX_MAX:
			m_arRc.SetAt(MIDX_RESTORE, iRc);
			m_arRc.SetAt(MIDX_MAX, iRc);	
			break;
		case MIDX_CLOSE:
			m_arRc.SetAt(MIDX_CLOSE, xRc);
			iRc.OffsetRect(-1, 0);		
			break;
		default:		continue;
		}

		iRc.OffsetRect(-bm.bmWidth/4, 0);	
	}
	m_arRc.SetAt(MIDX_DUAL, iRc); 

	m_axisres->GetBmp(IDX_V1)->GetBitmap(&bmv);
	iRc.right += 2;
	iRc.OffsetRect(-bmv.bmWidth/4, 0);	
	iRc.OffsetRect(-GAP*5, 0);
	
	for (ii = MIDX_V3; ii > MIDX_RESTORE; ii--)
	{
		switch (ii)
		{
		case MIDX_V1:
			m_arRc.SetAt(MIDX_V1, iRc);
			iRc.OffsetRect(-1, 0);	
			break;
		case MIDX_V2:
			m_arRc.SetAt(MIDX_V2, iRc);
			iRc.OffsetRect(-1, 0);	
			break;
		case MIDX_V3:
			m_arRc.SetAt(MIDX_V3, iRc);
			iRc.OffsetRect(-1, 0);	
			break;
		default:
			break;
		}
		iRc.OffsetRect(-bmv.bmWidth/4, 0);	
	}

	int	cx = GetSystemMetrics(SM_CXSMICON); 
	int	cy = GetSystemMetrics(SM_CYSMICON);
	
	m_axisres->GetBmp(IDX_ICON)->GetBitmap(&bm);
	
	pt = CPoint(0, 0);
	iRc.SetRect(pt.x, pt.y, pt.x + bm.bmWidth / 2, pt.y + bm.bmHeight);
	m_arRc.SetAt(MIDX_ICON, iRc);
}

int CMainFrame::DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown, BOOL bActive, int maskcolor)
{
	if (bRC.IsRectEmpty())	
		return 0;

	BITMAP		bm;
	CBitmap*	bitmap;
	int		bx, by, sx;

	bx = bRC.Width();
	by = bRC.Height();

	if(index == MIDX_CLOSE  && !bActive)
		bx = 16;

	sx = bDown ? bActive ? bx : 3*bx : bActive ? 0 : 2*bx;


	switch (index)
	{
	case MIDX_CLOSE:
		bitmap = m_axisres->GetBmp(IDX_CLOSE);
		bitmap->GetBitmap(&bm);
		
		sx = bDown ? bx : bActive ? 0 : bx * 3;
		
		break;
	case MIDX_MAX:
		bitmap = m_axisres->GetBmp(IDX_MAX);
		bitmap->GetBitmap(&bm);
		sx = bDown ? bx : bActive ? 0 : bx * 3;

		break;
	case MIDX_MIN:
		bitmap = m_axisres->GetBmp(IDX_MIN);
		bitmap->GetBitmap(&bm);
		sx = bDown ? bx : bActive ? 0 : bx * 3;

		break;
	case MIDX_RESTORE:
		bitmap = m_axisres->GetBmp(IDX_RESTORE);
		bitmap->GetBitmap(&bm);
		if (bDown)
			sx = bx;

		break;
	case MIDX_ICON:
		bitmap = m_axisres->GetIcon();
		sx = bActive ? 0 : bx;
		sx = 0; //메인타이틀 inactive 없음
		break;
	case MIDX_V1:
		bitmap = m_axisres->GetBmp(IDX_V1);
		bitmap->GetBitmap(&bm);
		break;

	case MIDX_V2:
		bitmap = m_axisres->GetBmp(IDX_V2);
		bitmap->GetBitmap(&bm);
		break;

	case MIDX_V3:
		bitmap = m_axisres->GetBmp(IDX_V3);
		bitmap->GetBitmap(&bm);
		break;
//	case MIDX_V4:
//		bitmap = m_axisres->GetBmp(IDX_V4);
//		bitmap->GetBitmap(&bm);
//		break;
	case MIDX_DUAL:
		bitmap = m_axisres->GetBmp(IDX_DUAL);
		bitmap->GetBitmap(&bm);
		break;
	default:
		return 0;
	}
	
	CDC		buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap		*oldbuffBitmap, *oldmaskBitmap, *oldmemoryBitmap, *oldcopyBitmap;

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	CRect		maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(bitmap);
	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, sx/*0*/, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bx, by, pDC,  bRC.left, bRC.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	pDC->BitBlt(bRC.left, bRC.top, bx, by, &copyDC, 0/*sx*/, 0, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
	return bx;
}

int CMainFrame::GetSelectItem(CPoint pt)
{
	CRect	bRc, wRc;

	GetWindowRect(wRc);
	
	pt.x -= wRc.left;// + XGAP;
	pt.y -= wRc.top ;// + YGAP;

	CWindowDC	dc(this);
	for (int ii = 0; ii < m_arRc.GetSize(); ii++)
	{
		bRc = m_arRc.GetAt(ii);
		bRc.OffsetRect(XGAP, YGAP);
		if (!bRc.PtInRect(pt))
			continue;

		if (ii == MIDX_MAX && IsZoomed())
			return MIDX_RESTORE;
		else	return ii;
	}
	return -1;
}

void CMainFrame::fitDual()
{
	int moniterN = GetSystemMetrics(SM_CMONITORS);

	if (moniterN <= 1)
	{
		::MessageBox(m_hWnd, "듀얼모니터가 아닙니다.", "확인", MB_ICONINFORMATION);
		return;
	}

	WINDOWPLACEMENT	pl;

	GetWindowPlacement(&pl);
	g_monRc.SetRectEmpty();
	EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);

	if (pl.showCmd & SW_SHOWMAXIMIZED)
	{
		pl.showCmd = SW_RESTORE;
		pl.rcNormalPosition.left   = g_monRc.left;
		pl.rcNormalPosition.top    = g_monRc.top;
		pl.rcNormalPosition.right  = g_monRc.right;
		pl.rcNormalPosition.bottom = g_monRc.bottom;
		SetWindowPlacement(&pl);
	}
	else	
		SetWindowPos(NULL, g_monRc.left, g_monRc.top, g_monRc.Width(), g_monRc.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
}

void CMainFrame::DrawTitleBitmap(CDC *pDC, CRect drawRC, BOOL bActive)
{//메인 상단 타이틀 영역 그리기. 활성화 비활성화 일때
	if (bActive)
	{
		BITMAP	bm;
		m_bmCaption[0].GetBitmap(&bm);
		
		CDC	memDC;
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(&m_bmCaption[0]);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->StretchBlt(drawRC.left, drawRC.top, drawRC.Width(), drawRC.Height(),	
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			else	memDC.DeleteDC();
		}
	}
	else
	{
		BITMAP	bm;
		m_bmCaption[1].GetBitmap(&bm);
		
		CDC	memDC;
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(&m_bmCaption[1]);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->StretchBlt(drawRC.left, drawRC.top, drawRC.Width(), drawRC.Height(),
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			else	memDC.DeleteDC();
		}
	}
}

void CMainFrame::DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth, nHeight;

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs,gs,bs;
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re,ge,be;
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep, gStep, bStep;
	CPen	*sPen;
	if (bRight)
	{
		if (!nWidth)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left + ii, drawRC.top);
				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left, drawRC.top + ii);
				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}

#include <nb30.h>
#include <Iphlpapi.h>

#pragma comment (lib, "IpHlpApi.lib")

int CMainFrame::GetMacAddr(char* ipaddr, char* data)
{
	int returnL = 12;
	CString	ip  = _T("");
	CString ip2 = _T("");
	CString	checkIP;
	FillMemory(data, returnL, ' '); data[returnL] = 0x00;
	ip.Format("%s", ipaddr);
	ip.TrimLeft(), ip.TrimRight();

	int n = ip.ReverseFind('.');
	ip = ip.Left(n);

	n = ip.ReverseFind('.');
	ip2 = ip.Left(n);

	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (dwStatus == ERROR_SUCCESS)
	{
		for (int ii = 0; ii < 16; ii++)
		{
			checkIP = (((AdapterInfo[ii]).IpAddressList).IpAddress).String;
			checkIP.TrimLeft(), checkIP.TrimRight();
			if (false && checkIP.Find(ip) == -1 && checkIP.Find(ip2) == -1)
				continue;

			sprintf(data, "%.2X%.2X%.2X%.2X%.2X%.2X",
					AdapterInfo[ii].Address[0], 
					AdapterInfo[ii].Address[1], 
					AdapterInfo[ii].Address[2], 
					AdapterInfo[ii].Address[3], 
					AdapterInfo[ii].Address[4], 
					AdapterInfo[ii].Address[5]);
			break;
		}
	}
	else
	{
		sprintf(data, "NOT FOUND   ");
	}

	return returnL;
}

void CMainFrame::SetBZ()
{
	m_MClient->SetBZ();
}

bool CMainFrame::CheckSDIMap(int key)
{
	CSChild*	child = NULL;
	if (m_arSDI[m_vsN].Lookup(key, child))
		return true;
	else 
		return false;
}

int CMainFrame::DisplaySystemVersion()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
	
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	//
	// If that fails, try using the OSVERSIONINFO structure.
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return 0;
	}
	
	CString version, build;

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:

		if ( osvi.dwMajorVersion <= 4 )
			version = _T("Microsoft Windows NT ");
		
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			version = _T ("Microsoft Windows 2000 ");
		
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
			version = _T ("Microsoft Windows XP ");

		if ( osvi.dwMajorVersion == 6 )
			version = _T ("Vista ");
		
		// Test for product type.
		
		if( bOsVersionInfoEx )
		{
		}
		else
		{
			HKEY hKey;
			char szProductType[80];
			DWORD dwBufLen;
			
			RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				"SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
				0, KEY_QUERY_VALUE, &hKey );
			RegQueryValueEx( hKey, "ProductType", NULL, NULL,
				(LPBYTE) szProductType, &dwBufLen);
			RegCloseKey( hKey );
			if ( lstrcmpi( "WINNT", szProductType) == 0 )
				version += _T( "Workstation " );
			if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
				version += _T( "Server " );
		}
		
		
		if ( osvi.dwMajorVersion <= 4 )
		{
			build.Format("version %d.%d %s (Build %d)\n",
				osvi.dwMajorVersion,
				osvi.dwMinorVersion,
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}
		else
		{ 
			build.Format("%s (Build %d)\n",
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}
		break;
		
	case VER_PLATFORM_WIN32_WINDOWS:
		
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			version = _T("Microsoft Windows 95 ");
			if ( osvi.szCSDVersion[1] == 'C' )
				version += _T("OSR2 " );
		} 
		
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			version = _T("Microsoft Windows 98 ");
			if ( osvi.szCSDVersion[1] == 'A' )
				version += _T("SE " );
		}

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			version = _T("Microsoft Windows Me ");
		} 
		break;
		
	case VER_PLATFORM_WIN32s:
		
		version = _T ("Microsoft Win32s ");
		break;
	}

	return osvi.dwMajorVersion;
}

BOOL CMainFrame::startUDP()
{
	int	value;
	char	wb[128];
	CString	ipaddr;

	getConnectInfo(ipaddr, value);
	strcpy_s(wb, ipaddr); value = 15298;     //15299;
	return m_wizard->setUDP(wb, value);
}

void CMainFrame::cleanUDP()
{
	m_wizard->cleanUDP();
}



long CMainFrame::DoChildKey(WPARAM wParam, LPARAM lParam)
{
	CChildFrame*	child = (CChildFrame *) MDIGetActive();
	if (!child)	return 0;
	return child->runHookedKey(wParam, lParam);
}

long CMainFrame::DoPopupKey(WPARAM wParam, LPARAM lParam)
{
	if (m_modal != NULL)
		return 0;

	CWnd* pWnd = GetActiveWindow();
	int key; CGPop* popup = NULL;

	for (POSITION pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, popup);
		if (popup == pWnd)
		{
			if (popup->runHookedKey(wParam, lParam))
				return 1;
			else
				return 2;
		}
	}
	return 0;
}


long CMainFrame::DoSDIKey(WPARAM wParam, LPARAM lParam)
{
	CWnd* pWnd = GetActiveWindow();

	int		key;
	CSChild*	schild = NULL;


	if (wParam == VK_HOME)
	{
		if (m_tMenu)	m_tMenu->SetHome();
		return 1;
	}

	for (POSITION pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
		if (schild == pWnd)
		{
			if (schild->runHookedKey(wParam, lParam))
				return 1;
			else
				return 2;
		}
	}

	return 0;
}

long CMainFrame::DoHotKey(WPARAM wParam, LPARAM lParam)
{
	if (m_enableHogakey && m_xvalueHogakey && wParam == (WPARAM)m_xvalueHogakey)
	{
	}
	return 0;
}

void CMainFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMDIFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CMainFrame::addCJCode(CString strdata)
{
	CString tmps;
	strdata = strdata.Left(L_CJcode); //L_CJcode = 204
	struct CJcode* pdata = new struct CJcode;
	pdata = (struct CJcode*)(char*)strdata.operator LPCSTR();
	if(strdata.GetLength() != L_CJcode)
		return;

	/*
	tmps = CString(pdata->code, 12);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 12)
	{
		tmps = _T("@@실시간 푸쉬 종목코드(12) 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->update, 14);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 업데이트시각(14) 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}
	
	tmps = CString(pdata->name, 100);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 100)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->kind, 4);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->idct, 6);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->valday, 10);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->mtday, 10);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->mtry, 6);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->itrt, 10);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->mtday, 10);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}

	tmps = CString(pdata->mtday, 10);
	tmps.TrimLeft(); tmps.TrimRight();
	if(tmps.GetLength() != 14)
	{
		tmps = _T("@@실시간 푸쉬 종목코드 자리수 이상");
		OutputDebugString(tmps);
		AfxMessageBox(tmps);
		return;
	}
	*/
	strdata.TrimLeft();
	strdata.TrimRight();
	CString path, sWrite, tmpS, temps;
	CFile file;
	path = m_home + "\\tab\\" + _T("IssBondMast.txt") ; 

	CFile pFile;
	if (file.Open(path, CFile::modeReadWrite))
	{
		int ilen = (int)file.GetLength();
		DWORD	pos = (DWORD)file.SeekToEnd();
		file.Seek((long) pos, CFile::begin);
		file.Write(pdata, L_CJcode);
		file.Close();
		m_wizard->ReloadMaster();
		CreateBonddlg();
	}
}

void CMainFrame::addCJCode(struct CJcode   *pcode)
{
	CString path, sWrite, tmpS, temps;
	CFile file;
	path = m_home + "\\tab\\" + _T("IssBondMast.txt") ; 

	CFile pFile;
//	if (file.Open(path, CFile::modeReadWrite|CFile::typeBinary))
	if (file.Open(path, CFile::modeReadWrite))
	{
		DWORD	pos = (DWORD)file.SeekToEnd();
		file.Seek((long) pos, CFile::begin);
		file.Write(pcode, sizeof(struct CJcode));
		file.Close();
	}
}

void CMainFrame::LoadCJCode()
{
	CString path;
	int codeN;
	
	struct CJcode code;
	AAGCODE* acode = NULL;
	CJCODE* cjCode;

	CFile file;
	path = m_home + "\\tab\\" + _T("IssBondMast.txt") ; 
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return ;
	
	int ilen = (int)file.GetLength();
	codeN = (int)file.GetLength() / sizeof(struct CJcode);
	
	char* pdata = NULL;
	CString strtemp, tmpS, tmps;
	m_arrayCJCode.RemoveAll();
	for (int ii = 0; ii < codeN; ii++)
	{
		cjCode = new CJCODE;
	
		memset(cjCode, 0, L_CJcode);
		
		file.Read(&code, sizeof(struct CJcode));
		
		memcpy(cjCode->code, code.code, 12);
		tmps = CString(code.name, 100);
		tmps.TrimRight();
		if(tmps.Find(_T("ELS")) >= 0 )
			continue;
		memcpy(cjCode->name, tmps, tmps.GetLength());
		memcpy(cjCode->kind, code.kind, 4);
		memcpy(cjCode->idct, code.idct, 6);
		tmps = CString(code.idct, 6);
		memcpy(cjCode->valday, code.valday, 10);
		memcpy(cjCode->mtday, code.mtday, 10);
		memcpy(cjCode->mtry, code.mtry, 6);
		memcpy(cjCode->itrt, code.itrt, 10);
		memcpy(cjCode->intertype, code.intertype, 6);
		memcpy(cjCode->credit, code.credit, 6);
		memcpy(cjCode->minfore, code.minfore, 10);
		memcpy(cjCode->minthree, code.minthree, 10);
		tmps = CString(code.update, 14);
		memcpy(cjCode->update, tmps, 14);
		
		m_arrayCJCode.Add(cjCode);
	}
	
	file.Close();
	return;
}

void CMainFrame::CreateBonddlg()
{
	CString dllPath;
	dllPath.Format(_T("%s\\exe\\AxisCode.dll"), m_home);
	HINSTANCE	m_hReg;
	m_hReg = LoadLibrary(dllPath);

	if(!m_hReg)
	{
		AfxMessageBox(_T("createbondpop fail"));
		FreeLibrary(m_hReg);
		return;
	}

	CWnd* (APIENTRY *axCreateBondDlg)(CWnd* , void*);
	axCreateBondDlg = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hReg, "axCreateBondDlg");
	if(axCreateBondDlg == NULL)
	{
		AfxMessageBox(_T("createbonddlg fail"));
		FreeLibrary(m_hReg);
		return;
	}

	CString tmpS;
	tmpS = GetUserName();
	m_paxiscode = axCreateBondDlg(this, (void*)tmpS.operator LPCSTR());
	FreeLibrary(m_hReg);
	
	return;
}

LRESULT CMainFrame::OnMessangerhandle(WPARAM wParam, LPARAM lParam)
{
	HWND hnd = (HWND)wParam;
	return 0;
}

LRESULT CMainFrame::OnMessangerend(WPARAM wParam, LPARAM lParam)
{
	CString	file, tmpS;
	tmpS.Format(_T("%d"), lParam);  
	file.Format("%s\\%s\\%s\\%s", m_home, "user",  GetUserName(), "usersetup.ini");
	WritePrivateProfileString("messanger", "connect", "N", file);

	tmpS.Format(_T("%s"), this->m_bServerError == true ? "N" : "Y");
	WritePrivateProfileString("messanger", "Serer_error", tmpS, file);

	tmpS.Format(_T("%d"), this->m_hWnd);
	WritePrivateProfileString("messanger", "hwnd", tmpS, file);
	return 0;
}
LRESULT CMainFrame::OnMessangerlogin(WPARAM wParam, LPARAM lParam)
{
	CString	file, tmpS;
	tmpS.Format(_T("%d"), lParam);   //0 성공 1 실패 가입되지 않은 아이디 2 실패 패스워드 틀림
	file.Format("%s\\%s\\%s\\%s", m_home, "user",  GetUserName(), "usersetup.ini");
	WritePrivateProfileString("messanger", "connect", tmpS, file);

	tmpS.Format(_T("%s"), this->m_bServerError == true ? "N" : "Y");
	WritePrivateProfileString("messanger", "Serer_error", tmpS, file);

	tmpS.Format(_T("%d"), this->m_hWnd);
	WritePrivateProfileString("messanger", "hwnd", tmpS, file);
	return 0;
}

//log window trace
LRESULT CMainFrame::OnTrace(WPARAM wParam, LPARAM lParam)
{
	if ( m_wndOutput.GetSafeHwnd() )
		m_wndOutput.SendMessage(WM_USER, wParam, lParam);
	return 0;
}

//mainsetup save event
LRESULT CMainFrame::ApplySetup(UINT id)
{
	switch (id)
	{
		/*
	case IDD_CAPTIONSETUP:
//		load_eninfomation(false);
//		break;
	case IDD_MNGSETUP:
//		load_mngSetup();
		break;
	case IDD_ORDER_SETUP:
//	    SetConclusion();
		break;
	case IDD_IMAGESETUP:
//		change_Skin();
		break;
	case IDD_SETTICKINFO:
//		load_tickInfo();
		break;
	case IDD_HOTKEY:
//		load_hkey();
		break;
	case IDD_TSETUP:
//		m_bar1->Refresh();
//		m_smain->Refresh();
		break;
	case IDD_FUNCTIONS:
//		m_bar1->RefreshFunc();
		break;
	case IDD_NEWSALRAM:
//		m_newArm->loadInfomation();
		break;
		*/
	case IDD_DSETUP:
		load_eninfomation(false);
		break;
	case IDD_ESETUP:
		TRACE(_T("IDD_ESETUP \n"));
		break;
	case IDD_DALMDLG:
		load_eninfomation(false);
		break;
	case 5030:   //전체환경 설정에서 사용자 설정 변경 선택시 
		m_pFormMgr->ChangeChild(_T("FS503000"));
		break;
	}
	return 0;
}

//maintoolbar screen name search event 
bool CMainFrame::select_SName(CString input)
{
	CStringArray	array;
	if (!m_tMenu)		
		return false;

	if (m_tMenu->GetSelectList(input, array))
	{
		m_bar1->set_List(array);
	//	m_tMenu->set_List(array);
		m_smain->set_List(array);
		return true;
	}

	CString	msg;
	msg.LoadString(ST_MSG_NOTHING_SELECTED);
	displayGuide(msg);

	return false;
}

void CMainFrame::SetHTSTitle()
{
	char temp_path[MAX_PATH];
	WORD majorVer, minorVer, buildNum, revisionNum;

	// 현재 실행된 프로그램의 경로를 얻는다.
	GetModuleFileName(AfxGetInstanceHandle(), temp_path, sizeof(temp_path));

	// 버전 정보를 얻기 위해 사용할 핸들값을 저장하는 변수이다.
	DWORD h_version_handle;
	// 버전정보는 항목을 사용자가 추가/삭제 할수 있기 때문에 고정된 크기가 아니다.
	// 따라서 현재 프로그램의 버전정보에 대한 크기를 얻어서 그 크기에 맞는 메모리를 할당하고 작업해야한다.
	DWORD version_info_size = GetFileVersionInfoSize(temp_path, &h_version_handle);

	// 버전정보를 저장하기 위한 시스템 메모리를 생성한다. ( 핸들 형식으로 생성 )
	HANDLE h_memory = GlobalAlloc(GMEM_MOVEABLE, version_info_size); 
	// 핸들 형식의 메모리를 사용하기 위해서 해당 핸들에 접근할수 있는 주소를 얻는다.
	LPVOID p_info_memory = GlobalLock(h_memory);

	// 현재 프로그램의 버전 정보를 가져온다.
	if(GetFileVersionInfo(temp_path, h_version_handle, version_info_size, p_info_memory))
	{
		VS_FIXEDFILEINFO* pFineInfo = NULL;
		UINT bufLen = 0;
		// buffer로 부터 VS_FIXEDFILEINFO 정보를 가져옵니다.
		if(VerQueryValue(p_info_memory,"\\",(LPVOID*)&pFineInfo, &bufLen) !=0)
		{    	
			majorVer = HIWORD(pFineInfo->dwFileVersionMS);
			minorVer = LOWORD(pFineInfo->dwFileVersionMS);
			buildNum = HIWORD(pFineInfo->dwFileVersionLS);
			revisionNum = LOWORD(pFineInfo->dwFileVersionLS);
		}
	}

	GlobalUnlock(h_memory); 
	GlobalFree(h_memory);

	CString strdata, tmpS;
	tmpS = m_axis->GetProfileString(WORKSTATION, _T("SIGNONID"));
	tmpS = Parser(tmpS, _T("|"));
	strdata.Format(_T("ID=%s Server=%s Ver.=%d.%d.%d.%d"), tmpS, 
		Parser(m_ip, _T("|")), majorVer, minorVer, buildNum, revisionNum);
	SetWindowText(strdata);
}

CString CMainFrame::Onl_seedenc(CString s_text, BOOL bBase64 /*= TRUE*/) 
{
	// TODO: Add your control notification handler code here
//	return s_text;  //test
	m_crypt.Encrypt((BYTE*)s_text.operator LPCSTR(), s_text.GetLength());
	return m_crypt.m_outB;
	CString log, path;
	path.Format("%s\\%s\\%s", m_home, "exe", "AxMPCrypt.dll");
	OutputDebugString("@@" + path);
	HMODULE hModule = LoadLibrary(path);
	if (!hModule)
	{
		OutputDebugString(_T("AxMPCrypt.dll load error !!\n"));
		return s_text;
	}
	if(bBase64 == TRUE)
	{
		char*	(APIENTRY *axEncrypt)	(char* , int, int&);
		axEncrypt = (char*(APIENTRY *)(char* , int, int&)) GetProcAddress(hModule, _T("axEncrypt"));
		if(axEncrypt == NULL)
		{
		//	AfxMessageBox(_T("!axEncrypt"));

			TRACE(_T("NULL"));
			return s_text;
		}
		
		int isize = s_text.GetLength();
		int iOutSize = isize*1000;
		CString senc = axEncrypt((LPSTR)(LPCTSTR)s_text, isize, iOutSize);
		log.Format(_T("@@ Onl_seedenc [%s] [%s]\n"),   s_text, senc);
		OutputDebugString(log);
		return senc;
	}
	else
	{
		char*	(APIENTRY *axSEEDEnc)	(char* , int, int&);
		axSEEDEnc = (char*(APIENTRY *)(char* , int, int&)) GetProcAddress(hModule, _T("axSEEDEnc"));
		if(axSEEDEnc == NULL)
		{
		//	AfxMessageBox(_T("!axSEEDEnc"));

			TRACE(_T("NULL"));
			return s_text;
		}
		
		int isize = s_text.GetLength();
		int iOutSize = isize*100;
		CString senc = axSEEDEnc((LPSTR)(LPCTSTR)s_text, isize, iOutSize);
		return senc;		
	}
	FreeLibrary(hModule);
}

CString CMainFrame::Onl_seeddec(CString s_text, BOOL bBase64 /*= TRUE*/) 
{
//	return s_text;  //test
	m_crypt.Decrypt((BYTE*)s_text.operator LPCSTR(), s_text.GetLength());
	return m_crypt.m_outB;
	CString log;
	CString path;
	path.Format("%s\\%s\\%s", m_home, "exe", "AxMPCrypt.dll");
	OutputDebugString("@@" + path);
	HMODULE hModule = LoadLibrary(path);
	if (!hModule)
	{
	//	AfxMessageBox(_T("!AxMPCrypt.dll"));

		OutputDebugString(_T("AxMPCrypt.dll load error !!\n"));
		return s_text;
	}
	if(bBase64 == TRUE)
	{	
		char*	(APIENTRY *axDecrypt)	(char* , int, int&);
		axDecrypt = (char*(APIENTRY *)(char* , int, int&)) GetProcAddress(hModule, _T("axDecrypt"));
		if(axDecrypt == NULL)
		{
		//	AfxMessageBox(_T("!axDecrypt"));

			TRACE(_T("NULL"));
			return s_text;
		}
		
		int isize = s_text.GetLength();
		int iOutSize = isize*1000;
		CString sdec = axDecrypt((LPSTR)(LPCTSTR)s_text, isize, iOutSize);
		log.Format(_T("@@ Onl_seeddec [%s] [%s]\n"),   s_text, sdec);
		OutputDebugString(log);
		return sdec;
	}
	else
	{
		char*	(APIENTRY *axSEEDDec)	(char* , int, int&);
		axSEEDDec = (char*(APIENTRY *)(char* , int, int&)) GetProcAddress(hModule, _T("axSEEDDec"));
		if(axSEEDDec == NULL)
		{
		//	AfxMessageBox(_T("!axSEEDDec"));

			TRACE(_T("NULL"));
			return s_text;
		}
		
		int isize = s_text.GetLength();
		int iOutSize = isize*100;
		CString senc = axSEEDDec((LPSTR)(LPCTSTR)s_text, isize, iOutSize);
		return senc;		
	}
	FreeLibrary(hModule);
}	

CString CMainFrame::CreateUUID()
{
	//WCHAR       GuidText[250] ={0};
	char		 GuidText[250] ={0}; 
	UUID        uuid;

	CoCreateGuid (&uuid);
	wsprintf(
		GuidText,
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		uuid.Data1,
		uuid.Data2,
		uuid.Data3,
		uuid.Data4[0], uuid.Data4[1],
		uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]
		);


	CString tmps;
	tmps.Format(_T("%s"), GuidText);
	return tmps;
}

void CMainFrame::RemoveMdi(int vsn, int key)
{
	m_arMDI[vsn].RemoveKey(key);	

	int	ncnt = m_arMDI[m_vsN].GetCount() + m_arSDI[m_vsN].GetCount();	
	if (ncnt == 0)
		DrawFrame();
}

void CMainFrame::SetLogindata(char* pdata)
{
    struct	_signR*	signR = (struct	_signR*)pdata;
	CString text, tmpS, string;
	int pos = 0 ;
	m_errMsg.Format(_T("%s"), signR->guide);
	m_errMsg.TrimRight();
	text = CString(signR->infox, sizeof(signR->infox));
	text.TrimRight();

	struct loginmod* plogout;
	plogout = (struct loginmod*)text.operator LPCSTR();

	if(*plogout->errc >= 48 && *plogout->errc <= 57)
		m_errCode = atoi(plogout->errc);
	else
		m_errCode = -1;
	
	if(m_errMsg.Find("9999") >= 0 && m_errMsg.Find("처리지연중"))
		m_errCode = 7;

OutputDebugString("\n\n\n------------------------------------------------------------\n");
string.Format(_T("@loginoutput userid(아이디)(30)=[%s]\n"), CString(plogout->userid, 30).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput name(이름)(30)=[%s]\n"), CString(plogout->name, 30).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput auth(권한)(7)=[%s]\n"), CString(plogout->auth, 7).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput comp(회사코드)(6)=[%s]\n"), CString(plogout->comp, 6).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput upjong(업종)(2)=[%s]\n"), CString(plogout->upjong, 2).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput expivt(전문투자자여부)(1)=[%s]\n"), CString(plogout->expivt, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput dept(부서)(6)=[%s]\n"), CString(plogout->dept, 6).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput bonddealer(채권딜러전문여부)(1)=[%s]\n"), CString(plogout->bonddealer, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput pwch(비밀번호변경)(1)=[%s]\n"), CString(plogout->pwch, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput dupc(동접여부)(1)=[%s]\n"), CString(plogout->dupc, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput dupc(임시사용여부)(1)=[%s]\n"), CString(plogout->tmpuse, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput tmpuse(이용제한요청여부)(1)=[%s]\n"), CString(plogout->userst, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput emailfrm(이메일 액티베이트여부)(1)=[%s]\n"), CString(plogout->emailfrm, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput notice(공지팝업존재유무)(1)=[%s]\n"), CString(plogout->notice, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput uudierec(사용자고유키 재생성여부)(1)=[%s]\n"), CString(plogout->uudierec, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput reserve(예비용)(1)=[%s]\n"), CString(plogout->reserve, 20).TrimRight());
OutputDebugString(string);
m_strMsginfo = CString(plogout->reserve, 20).TrimRight();

string.Format(_T("@loginoutput errc(에러코드)(1)=[%s]\n"), CString(plogout->errc, 1).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput ldat(라스트날짜)(1)=[%s]\n"), CString(plogout->ldat, 8).TrimRight());
OutputDebugString(string);
string.Format(_T("@loginoutput ltim(라스트시간)(1)=[%s]\n"), CString(plogout->ltim, 8).TrimRight());
OutputDebugString(string);
OutputDebugString("\n\n\n--------------------------------------------------");
	//임시사용여부(고유키 초기화요청 또는 회사변경오청시 당일 임시사용여부 3,4
	if(CString(plogout->tmpuse, 1).TrimRight() == _T("Y"))  
		m_btmpuse = true;
	else
		m_btmpuse = false;

	if(CString(plogout->dupc, 1).TrimRight() == _T("Y"))  //동시접속 여부 
		m_bdupc = true;
	else
		m_bdupc = false;

	if(CString(plogout->userst, 1).TrimRight() == _T("Y"))  //이용제한요청 여부 2
		m_buserst = true;
	else
		m_buserst = false;

	if(CString(plogout->emailfrm, 1).TrimRight() == _T("Y"))  //이메일activate 여부 
		m_bemailfrm = true;
	else
		m_bemailfrm = false;

	if(CString(plogout->notice, 1).TrimRight() == _T("Y"))  //공지팝업 존재유무
		m_bnotice = true;
	else
		m_bnotice = false;

	if(CString(plogout->uudierec, 1).TrimRight() == _T("Y"))  //사용자고유키 재생성 여부
		m_buuidrec = true;
	else
		m_buuidrec = false;

	if(CString(plogout->pwch, 1).TrimRight() == _T("Y"))    //비밀번호 변경요청
		m_bpwch = true;
	else
		m_bpwch = false;
}

void CMainFrame::ResetUserstat()
{	
	m_btmpuse = false;
	m_bdupc = false;
	m_buserst = false;
	m_bemailfrm = false;
	m_bnotice = false;
	m_buuidrec = false;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	 switch(pCopyDataStruct->cbData)
	 {
        case 9999:
			{       
			//	SetLogindata((struct _signR *)pCopyDataStruct->lpData);
			}
			break;
		case 9998:
			{	
				//m_axConnectD->CenterWindow(GetDesktopWindow());
				if(m_bMainStart && m_axConnectD == NULL)
					SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
				else
				{
					if(m_axConnectD->IsWindowVisible() && m_axConnectD != NULL)
						m_axConnectD->CenterWindow(GetDesktopWindow());
				}
			}
			break;
		case WM_USER + 2:
			{

			}
			break;
        case WM_USER + 3:
			{

			}
			break;
	 }
	return CMDIFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}
#include <Windows.h>
void CMainFrame::SetFileVisible(CString strpath, bool bShow)
{
	/*
	if (!find.FindFile(strpath))
	{
		CString tmps;
		tmps.Format(_T("%s"), CreateUUID());
		HANDLE hFile = CreateFile(strpath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hFile)
		{
			WritePrivateProfileString("UUID", m_user, tmps, strpath);
		}
	}
	*/

	if(bShow)
		SetFileAttributes(strpath,FILE_ATTRIBUTE_NORMAL);
	else
		SetFileAttributes(strpath, FILE_ATTRIBUTE_HIDDEN);

}

//로그인 버튼 클릭시 바로 uuid 여부를 체크해서 axStart를 날릴지 결정한다  
bool CMainFrame::IsUUidCreated(CString struser)
{
	char	wb[512];
	CString		file, section, keys, value, uuid, tmpS;
	tmpS = m_axis->GetProfileString(WORKSTATION, "FirstLoginAfterInstall");
	file.Format("%s\\%s\\%s", m_home, "tab", "uuid.ini");
	//tmpS == _T("1");
	
	if(tmpS == "1")   //인스톨한뒤 최초 로그인 시도 
	{	
		uuid = CreateUUID();
		
		tmpS.Format(_T("%s"), CreateUUID());
		WritePrivateProfileString("UUID", struser, uuid, file);
		m_uuid.Format(_T("%s"), tmpS);
		m_uuid.TrimRight();
		m_uuid.TrimLeft();
		m_buuidcreate = true;
		m_axis->WriteProfileString(WORKSTATION, "FirstLoginAfterInstall", "0");
		return true;
	}
	else            //uuid 생성후 파일에 적어두었다
	{
		CString strpath;
		strpath = m_home + _T("\\tab\\uuid.ini");

		CFileFind find;
		if (!find.FindFile(strpath))
		{
		//	if (::MessageBox(m_axConnectD->m_hWnd, "사용자 고유키가 존재하지 않습니다 재생성 하시겠습니까?", "고유키재생성", MB_OKCANCEL) == IDOK)		
			{
				m_uuid = CreateUUID();
				WritePrivateProfileString("UUID", struser, m_uuid, file);
				m_buuidcreate = true;
				return true;
			}	
			/*
			else
			{
				::MessageBox(m_axConnectD->m_hWnd, "HTS를 종료합니다", "종료", MB_OK);
				PostMessage(WM_AXISCLOSE);
				return false;
			}
			*/
			
		}		
     	else  //uuid.ini 파일이 존재하는 경우
		{
			file.Format("%s\\%s\\%s", m_home, "tab", "uuid.ini");
			DWORD dwRc = GetPrivateProfileString("UUID", struser, "", wb, sizeof(wb), file);
			if (dwRc <= 0)	
			{
			//	if (::MessageBox(m_axConnectD->m_hWnd, "사용자 고유키가 존재하지 않습니다 재생성 하시겠습니까?", "고유키재생성", MB_OKCANCEL) == IDOK)
				{
					m_uuid = CreateUUID();
					WritePrivateProfileString("UUID", struser, m_uuid, file);
					m_buuidcreate = true;
				}
				/*
				else
				{
					::MessageBox(m_axConnectD->m_hWnd, "HTS를 종료합니다", "종료", MB_OK);
					PostMessage(WM_CLOSE);
					return false;
				}
				*/
			}
			else
			{
				m_uuid.Format(_T("%s"), wb);
				m_uuid.TrimRight();
				m_uuid.TrimLeft();
			}
			return true;
		}
	}
	return false;
}

void CMainFrame::set_Maps(CString mapN)
{
	//m_bar1->set_Maps(mapN);
}

void CMainFrame::LoadUserScreen(CString sUser)
{
	char	wb[512];
	CString	file, tmps;
	tmps = GetUserName();
	file.Format("%s\\%s\\%s\\%s.ini", m_home, USRDIR, tmps, sUser);
	tmps.Format("00");
	DWORD dwRc = GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), file);
	if (dwRc <= 0)	return;

	tmps = wb;
	createUserScreenEx(sUser, tmps);
	m_vsN = -1;
	m_pFormMgr->change_VirtualScreen(0);
}

void CMainFrame::SetActivate(UINT nState)
{
	CString	str;
	if (m_bSDI && m_noActive)
	{
		CSChild*	schild;
		int		key, gapX, gapY; 
		
		gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
		gapX += GetSystemMetrics(SM_CXBORDER) * 4;

		gapY  = GetSystemMetrics(SM_CYFRAME) * 2;
		gapY += GetSystemMetrics(SM_CYBORDER) * 4;

		WINDOWPLACEMENT	pl;
		switch (nState)
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			GetWindowPlacement(&pl);
			if (!(pl.showCmd & SW_SHOWMINIMIZED))
			{
				for (POSITION pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
				{
					m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
					schild->SetWindowPos(NULL, 0, 0, 0, 0, 
						SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
				}

				m_noActive = false;
				SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
				DrawFrame();
				m_noActive = true;
			}
			break;
		case WA_INACTIVE:
			break;
		}
	}
}

void CMainFrame::ALLTriggerSend(CString symbol)  
{
	for (int ii = 0; ii < 4; ii++)
	{
		if (m_bSDI)
		{
			CWnd* pWnd = GetActiveWindow();

			int		key;
			CSChild*	schild = NULL;

			for (POSITION pos = m_arSDI[ii].GetStartPosition(); pos; )
			{
				m_arSDI[ii].GetNextAssoc(pos, key, schild);

				//key |= 0xff00;
				m_wizard->setTrigger(key, symbol);
			}

		}
		else
		{
			int		key;
			POSITION	pos;
			CChildFrame*	pChild;

			for (pos = m_arMDI[ii].GetStartPosition(); pos; )
			{
				m_arMDI[ii].GetNextAssoc(pos, key, pChild);

				//key |= 0xff00;
				m_wizard->setTrigger(key, symbol);
			}
		}
	}

	int key; CGPop* popup = NULL;

	for (POSITION pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, popup);

		//key |= 0xff00;
		m_wizard->setTrigger(key, symbol);
	}
}

void CMainFrame::ConfigTriggerSend()
{
	CString	symbol = _T("RECONFIG\t0");

	for (int ii = 0; ii < 4; ii++)
	{
		if (m_bSDI)
		{
			CWnd* pWnd = GetActiveWindow();

			int		key;
			CSChild*	schild = NULL;

			for (POSITION pos = m_arSDI[ii].GetStartPosition(); pos; )
			{
				m_arSDI[ii].GetNextAssoc(pos, key, schild);

				//key |= 0xff00;		
				m_wizard->setTrigger(key, symbol);
			
			}

		}
		else
		{
			int		key;
			POSITION	pos;
			CChildFrame*	pChild;

			for (pos = m_arMDI[ii].GetStartPosition(); pos; )
			{
				m_arMDI[ii].GetNextAssoc(pos, key, pChild);

				//key |= 0xff00;
				m_wizard->setTrigger(key, symbol);
			}
		}
	}

	int key; CGPop* popup = NULL;

	for (POSITION pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, popup);

		//key |= 0xff00;
		m_wizard->setTrigger(key, symbol);
	}
}

void CMainFrame::SetPinchData()
{

}

LRESULT CMainFrame::OnGetPinch(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	static	CString	pinchCombo;
	pinchCombo = _T("");

	for (i = 0; i < 4; i++)
	{
		pinchCombo = pinchCombo + m_strVirtualNAME[i] + "\t";
	}

	return (long)pinchCombo.operator LPCTSTR();
}

void CMainFrame::SetPichUser(BOOL hogafilter)
{
	CString str, ipinfo;

	if (hogafilter)
		ipinfo.Format("접속IP [%s], 가상화면 유저 [%s] 호가 필터링 [ON]", m_ip, m_strVirtualNAME[m_vsN]);
	else
		ipinfo.Format("접속IP [%s], 가상화면 유저 [%s]", m_ip, m_strVirtualNAME[m_vsN]);

// updateX__
	ipinfo.Format("접속서버IP:[%s] 사용자ID:[%s]", m_ip, m_userN);

	str += ipinfo;
	SetWindowText(str);
	DrawFrame();
}

void CMainFrame::GetDialogColor(COLORREF &clrDialog, COLORREF clrText)
{
	clrDialog = m_axisres->GetColor(COLOR_DIALOG_BK);
	clrText = m_axisres->GetColor(COLOR_DIALOG_TEXT);
}

void CMainFrame::deleteNewsfile()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	CFile		cfile;
	CString		allfile, filename;

	allfile.Format("%s\\%s\\*.html", m_home, USRDIR);
	hFind = FindFirstFile(allfile, &FindFileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		filename.Format("%s\\%s\\%s", m_home, USRDIR, FindFileData.cFileName);
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			DeleteFile(filename);
		if (!FindNextFile(hFind, &FindFileData))	break;
	}
}

void CMainFrame::convertIndex()
{
	CString		file, section, keys, value;
	char		buf[16];
	CStringArray	ary;
	int idx = 0;

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, GetUserName(), TICKSETUP);
	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			section.Format("BAR_INFO_%d%d", ii, jj);
			int tKIND = GetPrivateProfileInt(section, "INDEXK", TKIND_INDEX, file);
			if (tKIND != TKIND_INDEX)	continue;

			keys = _T("00");
			DWORD dw = GetPrivateProfileString(section, keys, "", buf, sizeof(buf), file);
			if (dw <= 0)	continue;

			ary.RemoveAll();
			for (idx = 0; ; idx++)
			{
				keys.Format("%02d", idx);
				dw = GetPrivateProfileString(section, keys, "", buf, sizeof(buf), file);
				if (dw <= 0)	break;
				ary.Add(buf);
			}

			WritePrivateProfileSection(section, "", file);
			WritePrivateProfileString(section, "INDEXK", "0", file);

			for (idx = 0; idx < ary.GetSize(); idx++)
			{
				keys.Format("%03d", idx);
				value.Format("%02d", ConvertIndexKey(atoi(ary.GetAt(idx))));
				WritePrivateProfileString(section, keys, value, file);
			}
			ary.RemoveAll();
		}
	}
}

int CMainFrame::GetTicKey(int index)
{
	int	key = 0;
	switch (index)
	{
	default:
	case 0:		key = 1;	break;
	case 1:		key = 2;	break;
	case 2:		key = 3;	break;
	case 3:		key = 4;	break;
	case 4:		key = 5;	break;	
	case 5:		key = 6;	break;
	case 6:		key = 7;	break;
	case 7:		key = 8; 	break;
	case 8:		key = 9; 	break;
	case 9:		key = 10;	break;
	case 10:	key = 11;	break;
	case 11:	key = 12;	break;
	case 12:	key = 13;	break;
	case 13:	key = 17;	break;
	case 14:	key = 18;	break;
	}
	return key;
}

int CMainFrame::ConvertIndexKey(int index)
{
	int	key = 1;
	switch (index)
	{
	case 19:	key = 24;	break;	//	Nikkei
	case 20:	key = 25;	break;	//	Weight
	case 21:	key = 26;	break;	//	ASHARE	
	case 22:	key = 27;	break;	//	HANGSE
	case 23:	key = 28;	break;	//	HSCE
	case 24:	key = 20;	break;	//	S&P 500
	case 25:	key = 23;	break;	//	DOWJONES
	case 26:	key = 29;	break;	//	환율
	case 27:	key = 21;	break;	//	S&P 500 선물
	default:	key = index;	break;	//	no change
	}
	return key;
}

void CMainFrame::saveToolStatus()
{
	CString		file, string;

	file.Format("%s\\user\\%s\\%s", m_home, GetUserName(), TICKSETUP);

	int value;
	value = m_bar2->GetStyle() & WS_VISIBLE ? 1 : 0;
	string.Format("%d", value);
	WritePrivateProfileString("GENERAL", "screenbar", string, file);
}

void CMainFrame::loadToolStatus()
{
	CString		file;

	if (m_bSDI) return;

	file.Format("%s\\user\\%s\\%s", m_home, GetUserName(), TICKSETUP);

	BOOL visible = GetPrivateProfileInt("GENERAL", "screenbar", 1, file);
	ShowControlBar(m_bar2, visible, FALSE);

}

void CMainFrame::convertSingle(int key, int vsN)
{
	CChildFrame*	child  = NULL;
	if (!m_arMDI[vsN].Lookup(key, child))
		return;

	int		groupN, fontN;
	CRect		mdiRc, sdiRc;
	CString		mapname;

	child->GetWindowRect(mdiRc);
	if (!child->m_rectSDI.IsRectEmpty())
		sdiRc.CopyRect(child->m_rectSDI);
	else	sdiRc.CopyRect(&mdiRc);

	m_MClient->ScreenToClient(&mdiRc);
	
	mapname = child->m_mapN;
	groupN  = child->m_xcaption.GetGroup();
	fontN	= child->m_xcaption.GetFontSize();
	child->SendMessage(WM_CLOSE);
	m_pFormMgr->CreatePopup(mapname, groupN, WK_POPUP, mdiRc.top, key, mdiRc.TopLeft());
}

WCHAR* converTowide(char* pdata)
{
	WCHAR* data = new WCHAR[strlen(pdata)*2];
	memset(data, 0, strlen(pdata)*2);
	MultiByteToWideChar(CP_ACP, 0, pdata, -1, data, strlen(pdata)*2);
	return data;
}

void CMainFrame::SendMsgToMessanger(int gubn, CString strdata)
{
	switch(gubn)
	{
		case HANDLESEND:      //HTS 핸들전송
			{
				COPYDATASTRUCT tip;
				tip.cbData = WM_USER + 1001;
				tip.lpData = this->m_hWnd;

				HWND hwn = ::FindWindow("TfrmMain", NULL);
				//::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	
				::PostMessage(hwn, WM_USER + 1001, (WPARAM)this->m_hWnd , 0);	
			}
			break;
		case EXITMGSSEND:    //HTS종료 메시지 전송
			{
				HWND hwn = ::FindWindow("TfrmMain", NULL);
				SendMsgToMessanger(HANDLESEND);
				::PostMessage(hwn, WM_USER + 1002, 0 , 0);	
			}
			break;
		case SHOWMSGSEND:    //메신저 SHOW 메시지 전송  메신저를 화면 최상위로 띄움
			{
				HWND hwn = ::FindWindow("TfrmMain", NULL);
				SendMsgToMessanger(HANDLESEND);
				::PostMessage(hwn, WM_USER + 1003, 0 , 0);	
			}
			break;
		case RUNANDLOGINSEND:  //메신저 실행 및 로그인
			{
				CString path,data, pass; 
				path.Format(_T("%s\\%s"), m_home, _T("KBondMessenger\\KBondMessenger.exe"));
				data.Format(_T("BTS|%d %s|%s %s"),this->m_hWnd, m_user, m_pass, m_bServerError == true ?_T("N") : _T("Y"));	
				ShellExecute(this->m_hWnd, "open",  path, data, NULL, SW_SHOWNORMAL);			
			}
			break;
		case LOGINSEND:     //메신저 로그인
			{
				CString tmp, temps;
				struct _st
				{
					char id[32];
					char pass[32];
					char type[32];
				};
				CString path,data, pass;
				struct _st  *pst = new struct _st;
				memset(pst,0, sizeof(struct _st));
				tmp.Format(_T("t%s"), m_user);
				memcpy(&pst->id[0], (char*)tmp.operator LPCSTR(), tmp.GetLength());
				tmp.Format(_T("tt%s"), m_pass);
				memcpy(&pst->pass[0], (char*)tmp.operator LPCSTR(), tmp.GetLength());
			
				if(m_bServerError)  //장애
					tmp.Format(_T("ttt%s"), "N");
				else
					tmp.Format(_T("ttt%s"), "Y");

				memcpy(pst->type, (char*)tmp.operator LPCSTR(), m_user.GetLength());
				
				COPYDATASTRUCT tip;
				tip.cbData = 96;
				tip.dwData = 28673;
				tip.lpData = pst;

				HWND hwn = ::FindWindow("TfrmMain", NULL);
				SendMsgToMessanger(HANDLESEND);
				::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	
			}
			break;  
		case LOGINCANCELSEND:  //메신저 로그인 취소
			{
				COPYDATASTRUCT tip;
				tip.cbData = 0;
				ULONG_PTR pdata = 28674;
				tip.dwData = pdata;
				tip.lpData = NULL;

				HWND hwn = ::FindWindow("TfrmMain", NULL);
				SendMsgToMessanger(HANDLESEND);
				::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	
			}
			break;
		case NOTESEND:   //쪽지창 띄우기 보내고픈사람 이름 아이디순서 
			{
				
			}
			break;
		case PROFILESEND:  //사용자 프로파일 띄우기 보고픈 사람 아이디
			{
			
			}
			break;
		case ONETOONESEND:   //1:1 대화 띄우기  대화하고픈 사람 이름 아이디
			{
			
			}
			break;
	}
}

ULONG CMainFrame::ProcIDFromWnd(HWND hwnd)
{
	ULONG idProc;
	::GetWindowThreadProcessId(hwnd, &idProc);
	return idProc;
}

HWND CMainFrame::GetWinHandle(ULONG pid)
{
	HWND tempHwnd = ::FindWindowA(NULL, NULL); //최상위 윈도우 핸들 찾기
	while(tempHwnd != NULL)
	{
		if(::GetParent(tempHwnd) == NULL)
		{
			if(pid == ProcIDFromWnd(tempHwnd))
			{
				return tempHwnd;
			}
		}
		tempHwnd = ::GetWindow(tempHwnd, GW_HWNDNEXT);

	}

	return 0;
}

HWND CMainFrame::GetProcessHwnd(CString prcessName)
{
	CString strParameter;
	char szDir[1024*2] = {0,};
	BOOL bFlag = TRUE;
	HANDLE handle;
	PROCESSENTRY32 Process32;

	Process32.dwSize = sizeof(PROCESSENTRY32);
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		
	if (Process32First(handle, &Process32))
	{
		while(Process32Next(handle, &Process32))
		{
			CString szExe = Process32.szExeFile;
			szExe.MakeUpper();
			HWND ttt = GetWinHandle(Process32.th32ProcessID);

			TRACE(szExe + _T("\n"));
			if (szExe.Find(prcessName.MakeUpper()) >= 0)
			{
				return GetWinHandle(Process32.th32ProcessID);
			}
		}
	}
		
	CloseHandle(handle);
	return NULL;
}

void CMainFrame::ErrReport(CString eMsg)		//e_log
{
	CTime timeWrite;
	timeWrite = CTime::GetCurrentTime();
	CString strdata;
	strdata.Format("\n\nmon=%d day=%d hour=%d min=%d  --- ", timeWrite.GetMonth(), timeWrite.GetDay(), 
		timeWrite.GetHour(), timeWrite.GetMinute());
	eMsg = strdata + eMsg;
	eMsg += "\r\n\n";
	CString m_strLogFile;
	m_strLogFile.Format("%s\\%s\\%s", m_home, TABDIR, "errorlog.INI");
	
	CFile pFile;
	if (pFile.Open(m_strLogFile, CFile::modeReadWrite))
	{
		DWORD	pos = (int)pFile.SeekToEnd();
		pFile.Seek((long) pos, CFile::begin);
	}
	else
		pFile.Open(m_strLogFile, CFile::modeCreate|CFile::modeReadWrite);

	pFile.Write(eMsg, eMsg.GetLength() );
	pFile.Close();
}

void CMainFrame::PopAlarm(CString strdata)
{
	CString tmpS, tempS;
	CChildFrame*	child;
	CRect clientRc;
	WINDOWPLACEMENT	pl;
	POSITION pos;
	CPoint sp;
	int value = 0;
	int vsN = 0;
	int key;
	int cnt = m_mapGongji.GetSize();

	bool bfind = false;
	child = NULL;
	for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
		if(child->m_mapN == ALARMMAP)
		{
			bfind = true;
			break;
		}
	}

	if(!bfind)
	{
		tmpS.Format(_T("%s%s\t%s\n"), ALARMMAP,"title",strdata);
		key = m_pFormMgr->CreateChild(tmpS, 1, 0, 0, CPoint(100,100));
	}
	else
	{
		if(child == NULL)
				return;

		child->GetWindowPlacement(&pl);
		value = GetSystemMetrics(SM_CYCAPTION);
		value += GetSystemMetrics(SM_CXFRAME);
		value += GetSystemMetrics(SM_CXBORDER);
		sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
		sp.Offset(value, value);

		value = child->m_xcaption.GetGroup();
		switch (value)
		{
		case 0:
		case 99:	break;
		case 1:case 2: case 3:case 4: case 5:
			value++;
			if (value > 5)	value = 1;
			break;
		default:	value = 1;	break;
		}		

	
		tmpS.Format(_T("%s%s\t%s\n"), ALARMMAP,"title",strdata);
		CopyScreen(tmpS, value, child->m_xcaption.GetFontSize(), sp);
	}

	/*
	for(int ii = 0 ; ii < cnt; ii++)
	{
		if(ii == 0)
		{
			tempS = m_mapGongji.GetAt(ii);
			Parser(tempS, _T("\t"));
			tmpS.Format(_T("%s%s\t%s\n"), GONGJIMAP,"title",tempS);
			key = m_pFormMgr->CreateChild(tmpS, 1, 0, 0, CPoint(100,100));
		}
		else
		{	
			child = NULL;
			for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
			{
				m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
				if(child->m_mapN == GONGJIMAP)
					break;
			}

			if(child == NULL)
				break;

			child->GetWindowPlacement(&pl);
			value = GetSystemMetrics(SM_CYCAPTION);
			value += GetSystemMetrics(SM_CXFRAME);
			value += GetSystemMetrics(SM_CXBORDER);
			sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
			sp.Offset(value, value);

			value = child->m_xcaption.GetGroup();
			switch (value)
			{
			case 0:
			case 99:	break;
			case 1:case 2: case 3:case 4: case 5:
				value++;
				if (value > 5)	value = 1;
				break;
			default:	value = 1;	break;
			}		

			tempS = m_mapGongji.GetAt(ii);
			Parser(tempS, _T("\t"));
			tmpS.Format(_T("%s%s\t%s\n"), GONGJIMAP,"title",tempS);
			CopyScreen(tmpS, value, child->m_xcaption.GetFontSize(), sp);
		}
	}
	*/
}

void CMainFrame::CheckMsgType()
{
	CString	file, tmpS, tempS;
	int cnt = 0;
	char	buf[1024];
	file.Format("%s\\%s\\FISMSG.INI", m_home, TABDIR);
	
	m_mapMsg.RemoveAll();
	m_arrNoticemap.RemoveAll();
	while(1)
	{
		tmpS = m_strMsginfo.Mid(cnt, 3);

		if(tmpS.IsEmpty())
			break;

		if(tmpS.Left(1) == "0") //메시지 구분
		{
			tmpS = tmpS.Mid(1, 2);
			GetPrivateProfileString("msg", tmpS, "", buf, sizeof(buf), file);
			m_arrPopMsg.Add(tmpS);
		}
		else if(tmpS.Left(1) == "1")  //팝업창
		{
			tmpS = tmpS.Mid(1, 2);
			GetPrivateProfileString("notice", tmpS, "", buf, sizeof(buf), file);
			m_arrNoticemap.Add(buf);
		}

		cnt+=3;
		if(cnt >= 20)  //20바이트 이상은 주지 않는다
			break;
	}
	
	CString errmsg, tmps, temp;
	CAlarm* pdlg[20] = {0,};
	file.Format("%s\\%s\\FISMSG.INI", m_home, TABDIR);
	
	CRect rc;
	cnt = 0;
	for(int ii = 0 ; ii < m_arrPopMsg.GetSize(); ii++)
	{
		tmps = m_arrPopMsg.GetAt(ii);
		GetPrivateProfileString("FitsMsg", tmps, "", buf, sizeof(buf), file);
		tempS.Format(_T("%s"), buf);
		CStringArray arr;
		while(1)
		{
			if(tempS.IsEmpty())
				break;
			tmpS = Parser(tempS, _T("\\n"));
			arr.Add(tmpS);
		}

		errmsg.Format(_T("%s"),"\t");
		for(int ii = 0 ; ii < arr.GetSize() ; ii++)
		{
			tempS.Format(_T("%s%s"),  arr.GetAt(ii), "\t");
			errmsg += tempS;
		}
		errmsg.TrimRight();

		PopAlarm(errmsg);
	}

	for(int ii = 0 ; ii < m_arrloginPopMsg.GetSize(); ii++)
	{	
		tmps = m_arrloginPopMsg.GetAt(ii);
		GetPrivateProfileString("msg", tmps, "", buf, sizeof(buf), file);

		tempS.Format(_T("%s"), buf);
		CStringArray arr;
		while(1)
		{
			if(tempS.IsEmpty())
				break;
			tmpS = Parser(tempS, _T("\\n"));
			arr.Add(tmpS);
		}

		errmsg.Format(_T("%s"),"\t");
		for(int ii = 0 ; ii < arr.GetSize() ; ii++)
		{
			tempS.Format(_T("%s%s"),  arr.GetAt(ii), "\t");
			errmsg += tempS;
		}
		errmsg.TrimRight();
		PopAlarm(errmsg);
	}

	/*
	for(int ii = 0 ; ii < m_arrPopMsg.GetSize(); ii++)
	{
		tmps = m_arrPopMsg.GetAt(ii);
		GetPrivateProfileString("FitsMsg", tmps, "", buf, sizeof(buf), file);
		errmsg.Format(_T("%s"), buf);
		errmsg.TrimRight();

		pdlg[ii] = new CAlarm(m_home);
		pdlg[ii]->m_msg = errmsg;
		pdlg[ii]->Create(IDD_NOTICE, this);
		pdlg[ii]->ShowWindow(SW_SHOW);
							
		pdlg[ii]->CenterWindow();
		pdlg[ii]->GetWindowRect(rc);
		pdlg[ii]->SetWindowPos(&wndTop, rc.left + (ii *20), rc.top + (ii *20), rc.Width() , rc.Height(), SWP_SHOWWINDOW);
		cnt++;
	}

	m_arrloginPopMsg.RemoveAll();
	m_arrloginPopMsg.Add("01");
	for(int ii = 0 ; ii < m_arrloginPopMsg.GetSize(); ii++)
	{	
		tmps = m_arrloginPopMsg.GetAt(ii);
		GetPrivateProfileString("msg", tmps, "", buf, sizeof(buf), file);
		errmsg.Format(_T("%s"), buf);
		errmsg.TrimRight();

		pdlg[cnt] = new CAlarm(m_home);
		pdlg[cnt]->m_msg = errmsg;
		pdlg[cnt]->Create(IDD_NOTICE, this);
		pdlg[cnt]->ShowWindow(SW_SHOW);		
		pdlg[cnt]->CenterWindow();
		pdlg[cnt]->GetWindowRect(rc);
		pdlg[cnt]->SetWindowPos(&wndTop, rc.left + (cnt *20), rc.top + (cnt *20), rc.Width() , rc.Height(), SWP_SHOWWINDOW);
		cnt++;
	}
	*/
	for(int ii = 0 ; ii < m_arrNoticemap.GetSize() ; ii++)
	{
		tempS = m_arrNoticemap.GetAt(ii);
		m_pFormMgr->CreateChild(tempS, 1, 0, 0, CPoint(500,100));
	}
}

BOOL CMainFrame::Init_SharedMemory()
{
	CWnd	*pMain = this;
	CString	strFileName;
	strFileName.Format("%s%x", _T("STOCKChukManager"), pMain->m_hWnd);
	HANDLE hMapping = ::CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 64, strFileName);

	if (GetLastError() == ERROR_INVALID_HANDLE)
		return FALSE;

	LPVOID lpData = ::MapViewOfFile (hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 64);
/*
	SHAREDDATA MemData;
	MemData.pWnd = this;
	CopyMemory(lpData, &MemData, sizeof(_SHAREDDATA));
	TRACE("%s\n", lpData);
	::UnmapViewOfFile (lpData);
	*/
	return TRUE;
}

bool CMainFrame::CheckServer(CString data, CStringArray& arr)
{
	CWinApp* app = AfxGetApp();
	CString ip, tmp, tmps;
	int port;

	tmp.Format(_T("%s"), data.Mid( data.Find("=") + 1, data.GetLength() - data.Find("=")));
	tmps = tmp;
	ip = Parser(tmp, "|");
	port = atoi(tmp);

TRACE("-----------------" + ip + _T("\n"));

	CSocket	sock;
	sock.Create();
	if (sock.Connect(ip, port))
	{
		app->WriteProfileString(INFORMATION, "Server", tmps);
		sock.Close();
		return true;
	}
	else
	{
		sock.Close();
		for(int ii = 0 ; ii < arr.GetSize() ; ii++)
		{
			if(arr.GetAt(ii).Find(ip) >= 0)
			{
				arr.RemoveAt(ii);
				break;
			}
		}

		if(arr.GetSize() == 0)
			return false;

		CheckServer(arr.GetAt(0),arr);
	}

	return false;
}

void CMainFrame::SelectServerIP()
{
	char	wb[1024*4];
	int	value;
	int ii = 0;

	CString	path, keys, tmps, tmpx, tmpS, strip, sTime, devip, regip;
	CStringArray  m_ips;
	
	CWinApp* app = AfxGetApp();	
	regip = m_axis->GetProfileString(INFORMATION, byTCPs);

	if(regip.Find("0.0.0.0") >= 0)//로그인안하고 화면만 띄울경우
	{
		app->WriteProfileString(INFORMATION, "Server", "0.0.0.0|15201");
		return;
	}
	
	if(regip.Find("10.200.14.141") >= 0)  //개발붙는사람들이면
	{
		path.Format("%s\\%s\\yellowPage.ini", m_home, TABDIR);
		GetPrivateProfileString("Server_IP", "real", "", wb, sizeof(wb), path);
		regip.Format(_T("%s"), wb);
		app->WriteProfileString(INFORMATION, "Server", regip);
		return;
	}
	
	path.Format("%s\\%s\\yellowPage.ini", m_home, TABDIR);
	tmpx  = _T("Server_IP");

	value = GetPrivateProfileSection(tmpx, wb, sizeof(wb), path);

	for (ii = 0; ii < value; ii++)
	{
		tmps = &wb[ii];
		if (tmps.IsEmpty())
			break;
		m_arrIP.Add(tmps);
		ii += tmps.GetLength();
	}

	if(m_arrIP.GetSize() == 0)  //파일이 없는 사람은 레지스트리 아이피로
	{
		app->WriteProfileString(INFORMATION, "Server", regip);
		return;
	}

	int port;
	char buffer[128] = {0,};
	CTime time = CTime::GetCurrentTime();
	sTime.Format("%d", time.GetSecond());

	tmpS = m_arrIP.GetAt(atoi(sTime) % m_arrIP.GetSize());
	int icnt = tmpS.Find("=");
	tmps = tmpS.Mid(icnt + 1 , tmpS.GetLength() - icnt);
	app->WriteProfileString(INFORMATION, "Server", tmps);

	bool bfind = false;
	for(int ii = 0 ; ii < m_arrIP.GetSize() ;ii++)
	{
		if(m_arrIP.GetAt(ii).Find("rea") >= 0)
			bfind= true;
	}

	if(bfind == false)   //이상한 yellowpage 를 가지고 있는 사람은 리얼
		app->WriteProfileString(INFORMATION, "Server", regip);
}

bool CMainFrame::CheckUrl()
{
    CInternetSession session;
	
	CString		url, file,tmpS,dat,section,key,cpfile,strpath;
	LONG_PTR	value = 0, rc = 0;
	char buffer[128] = {0,};
	file.Format(_T("%s\\%s\\%s"),m_home, TABDIR, _T("axis.ini"));

	/*
	//url 판단사용할지 여부
	GetPrivateProfileString(_T("checkurl"), _T("use") , _T("0"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);

	if(tmpS == "0")
		return false;
	*/

	//장애판단 URL  
	CString	strUrl;
	GetPrivateProfileString(_T("checkurl"), _T("url") , _T("http://k-bond.kofia.or.kr/jsp/kbondErr.jsp"), buffer, sizeof(buffer), file);
	strUrl.Format(_T("%s"), buffer);
	strUrl.TrimRight();
	
    try {
        CInternetFile *p_html = (CInternetFile *)session.OpenURL(strUrl);

        if(p_html != NULL){
            CString str, html_text;
			
            p_html->SetReadBufferSize(200);
            while(1){
                if(!p_html->ReadString(str)) break;
                html_text += str;
            }
			
            p_html->Close();
			str.Format(_T("result=%s"), "N");
			if(html_text.Find(str) >= 0)
			{
				m_bServerError = true;
				return true;
			}
			
            delete p_html;
            session.Close();
        }
		
    } catch(CInternetException *){

		return false;
    }
	
	return false;
}

bool CMainFrame::CheckMessangerLogin()
{
	CString	file, key, tmpS;
	char	buf[1024];
	file.Format("%s\\%s\\%s\\%s", m_home, "user",  GetUserName(), "usersetup.ini");
	GetPrivateProfileString("messanger", "connect", "", buf, sizeof(buf), file);
	
	tmpS.Format(_T("%s"), buf);
	tmpS.TrimLeft();
	tmpS.TrimRight();

	if(tmpS.IsEmpty())
		return false;
	else
	{
		if(tmpS == "N" || tmpS == "1" || tmpS == "2")
			return false;
	}

	return true;
}

void CMainFrame::ReadUserLast(CString strkey, CStringArray& arr)
{
	char	wb[1024*4];
	int	value, ii;
	CString file, tmps, temps;
	tmps = GetUserName();

	if(m_bSDI)
		file.Format("%s\\%s\\%s\\%sSDI.ini", m_home, USRDIR, tmps, tmps);
	else
		file.Format("%s\\%s\\%s\\%sMDI.ini", m_home, USRDIR, tmps, tmps);

	value = GetPrivateProfileSection("LASTSTAT", wb, sizeof(wb), file);

	for (ii = 0; ii < value; ii++)
	{
		tmps = &wb[ii];
		if (tmps.IsEmpty())
			break;
		if(tmps.Left(1) == strkey)
		{
			temps = tmps.Mid(tmps.Find(_T("=")) + 1);
			arr.Add(temps);
		}
		ii += tmps.GetLength();
	}
}

void CMainFrame::SetRealIP()
{
//만약 리얼IP가 바뀔 예정이라면...
//미리!!!!!!! yellowpage 를 배포한다 이 yellowpage 에는 Date 와 NewServer_IP 항목이 
//추가 되있다. 특정 날짜가 지나면 여기 아이피로 접속하게끔 한다
	char	wb[1024*4];
	int	value;
	int ii = 0;

	CString	path, keys, tmps, tmpx, tmpS, strip, sTime, devip, regip;
	CStringArray  m_ips;
	
	CWinApp* app = AfxGetApp();	
	CTime time = CTime::GetCurrentTime();
	sTime.Format("%.4d%.2d%.2d", time.GetYear(), time.GetMonth(), time.GetDay());

	path.Format("%s\\%s\\yellowPage.ini", m_home, TABDIR);
	value = GetPrivateProfileSection("Date", wb, sizeof(wb), path);
	GetPrivateProfileString("Date", "day", "", wb, sizeof(wb), path);
	tmpS.Format(_T("%s"), wb);

	if(atoi(sTime) < atoi(tmpS))  //현재접속하는 날짜가 파일에 적어놓은 날짜보다 작으면 리턴
		return;

	tmpx  = _T("NewServer_IP");
	value = GetPrivateProfileSection(tmpx, wb, sizeof(wb), path);
	for (ii = 0; ii < value; ii++)
	{
		tmps = &wb[ii];
		if (tmps.IsEmpty())
			break;
		m_arrIP.Add(tmps);
		ii += tmps.GetLength();
	}

	int port;
	char buffer[128] = {0,};
	sTime.Format("%d", time.GetSecond());

	tmpS = m_arrIP.GetAt(atoi(sTime) % m_arrIP.GetSize());
	int icnt = tmpS.Find("=");
	tmps = tmpS.Mid(icnt + 1 , tmpS.GetLength() - icnt);
	app->WriteProfileString(INFORMATION, "Server", tmps);
}

void CMainFrame::MakeMenuArray()
{
	m_arrMenu.RemoveAll();

	CString strdata, path;
	CFile file;
	path = m_home + "\\tab\\" + "axis.menu" ; 
	char	buf[1024*2];

	FILE *fp = NULL;
	errno_t err = fopen_s(&fp, path, "r");
	
	if (!fp)
	{
		Invalidate();
		return ;
	}

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		strdata.Format(_T("%s"), buf);
		strdata.TrimRight();
		m_arrMenu.Add(strdata);
	}
	fclose(fp);
}

BOOL CMainFrame::IsExistInMenuFile(CString strmap)
{
	for(int ii = 0 ; ii < m_arrMenu.GetSize() ; ii++)
	{
		if(m_arrMenu.GetAt(ii).Find(strmap) >= 0)
			return TRUE;
	}
	return FALSE;
}

CWnd* CMainFrame::GetMapWmd(CString strmap, bool& bmdi)
{
	CChildFrame*	child;
	CSChild*	schild;
	int		key;
	POSITION pos;

	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		for (pos = m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_arMDI[vsN].GetNextAssoc(pos, key, child);
			if(child->m_mapN == strmap)
			{
				bmdi = true;
				return (CWnd*)child;
			}
		}

		for (pos = m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			if(schild->m_mapN == strmap)
			{
				bmdi = false;
				return (CWnd*)schild;
			}
		}
	}

	return NULL;
}