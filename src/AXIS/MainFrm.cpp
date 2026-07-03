// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "axis.h"
#include "../h/ledger.h"
#include "comdef.h"
#include "mmsystem.h"

#include "MainFrm.h"
#include "childFrm.h"
#include "axisdoc.h"
#include "axisview.h"
#include "axscrollview.h"
#include "axtabview.h"
#include "Dtconnect.h"
#include "CertLogin.h"
#include "LogoDlg.h"
#include "axmsg.hxx"
#include "axMisc.h"
#include "GPop.h"
#include "MPop.h"
#include "fname.h"
#include "lockdlg.h"
#include "LockPass.h"
#include "screendlg.h"
#include "ExitD.h"
#include "PrinterSettings.h"
#include "conclusion.h"
#include "econclusion.h"
#include "KobaElwNotify.h"
#include "ManageInfo.h"
#include "alarm.h"
#include "ndib.h"
#include "NPalette.h"
#include "passdlg.h"
#include "Chaser.h"
#include "NClock.h"
#include "Infofile.h"
#include "SChild.h"
#include "ProgramDlg.h"
#include "AccTool.h"
#include "AccSave.h"
#include "NewsViewer.h"
#include "Stoploss.h"
#include "TTip.h"
#include "HelpOK.h"
#include "StopWarn.h"
#include "SUiPreDef.h"
#include "dtInfo.h"
#include "dlgMisuAlarm.h"
#include "sysInfo.h"
#include "TestInfoDlg.h"
#include "BkWnd.h"
#include "trsmain.h"
#include "Info_Interest.h"
#include "SecureDlg.h"
//#include "PhonePad.h"
// custom toolbar include
#include "Wininet.h"
#include "APIHookEx.h"
#include "ImageNotice.h"
#include "TOP10Dialog.h"
#include "CertErrDialog.h"
#include "DlgPB.h"
#include "CUploadFile.h"
#include "CSlideWnd.h"
#include "CDlgServerOrder.h"

#include "../dll/sm/smheader.h"
#include "../dll/sm/TransparentMgr.h"
#include "../dll/axiscp/cpheader.h"
#include "XZip.h"
#include "XUnzip.h"
#include "CDlg_MSGBOX.h"

#include <io.h>
#include <math.h>
#include <fcntl.h>
#include <winspool.h>
#include <Windows.h>
#include <winuser.h>
#include <windef.h>
#define COMPILE_MULTIMON_STUBS
#include "multimon.h"
#include <Tlhelp32.h>		// process사용
//#include <WinAble.h>  //vc2019 주석
#include <afxinet.h>
#include <uxtheme.h>
#include <comutil.h>
//#include <Vsstyle.h>
//#include <Vssym32.h>
//** macho add begin
#include "Helper.h"
#include "AccountConfig.h"
#include "MapHelper.h"
#include "DuplicateLoginConfirmDlg.h"

#include "EmpPassChangeDlg.h"
#include "EmpPassChangeNotifyDlg.h"

#include "CDLG_Notice.h"

#include "enc.h"
#include "FirstJob.h"
#include "../H/interMSG.h"


#include <lm.h>
#include <assert.h>
#pragma comment(lib, "Netapi32.lib")

//** macho add end

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CRect	g_monRc(0, 0, 0, 0);
static CMainFrame* m_pMain = NULL;

#ifdef DF_MAIN_RTS
std::unordered_map<std::string, int> g_codeToIndex;
TickSnapshot g_tickSlots[MAX_SLOT];
int g_nextIndex = 0;
std::shared_mutex  g_codeMapLock;
int g_nextSlot{};
#endif

//std::unordered_set<int>  g_dirtySlots;  // ← 추가
//std::mutex        g_dirtyMtx;    // ← 추가

#pragma	comment(lib, "Winmm.lib")
#pragma	comment(lib, "SUiPre.lib")
#pragma comment(lib, "iphlpapi.lib")

//** macho begin 2008-03-20
#define CALLCENTER1		"고객센터 : 1588-0030 , 1544-0050"
#define CALLCENTER2		"고객센터 : 1544-0050 , 1588-0030"
#define CALLCENTER3		"  해외주식 : 02-6915-2900"
#define IBK_HOME_URL	"www.ibks.com"
#define IDC_BTN_LOGO    12345
#define SDIHEIGHT	91//84->89
#define SETUPFILE	"axisensetup.ini"
#define TICKSETUP	"axisticker.ini"
#define MAXARRAY	1000
#define	dnACTION	2
#define saveFILE "portfolio.ini"
#define tempFILE "portfolio.ini.tmp"

#define	UPMAXSIZE	1024*15
#define TM_DNINTEREST 9050
#define TM_SLIDE	9051

#define TM_MAIN_TEST_RTS 9052

#define CNT_MAXPB 10

const char* CBTN_CLOSE	= "창닫기";
const char* CBTN_MAX	= "최대화";
const char* CBTN_MIN	= "최소화";
const char* CBTN_RESTORE= "창복원";

//64 bit 구분
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

//간편인증
//#define DF_SIMPLEAUTH 1
#pragma warning (disable : 4477)

//** macho end

CString MarketToString(CString smarket)
{
	if (smarket =="1")
		return "KRX";
	else if (smarket == "2")
		return "NXT";
	else if (smarket == "3")
		return "통합";
	else
		return "KRX";
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
#define WM_NCMOUSELEAVE		0x02A2
#define WM_REFRESH813		WM_USER+2428
#define WM_SECUREDLG		WM_USER+2429

static BOOL GetVersion(OSVERSIONINFOEX* os) {
	HMODULE hMod{};
	RtlGetVersion_FUNC func{};

	OSVERSIONINFOEXW o{};
	OSVERSIONINFOEXW* osw = &o;

	hMod = LoadLibrary(TEXT("ntdll.dll"));
	if (hMod)
	{
		func = (RtlGetVersion_FUNC)GetProcAddress(hMod, "RtlGetVersion");
		if (func == 0)
		{
			FreeLibrary(hMod);
			return FALSE;
		}
		ZeroMemory(osw, sizeof(*osw));
		osw->dwOSVersionInfoSize = sizeof(*osw);
		func(osw);

		os->dwBuildNumber = osw->dwBuildNumber;
		os->dwMajorVersion = osw->dwMajorVersion;
		os->dwMinorVersion = osw->dwMinorVersion;
		os->dwPlatformId = osw->dwPlatformId;
		os->dwOSVersionInfoSize = sizeof(*os);
	}
	else
		return FALSE;
	FreeLibrary(hMod);
	return  TRUE;
}


//#define DF_SLIDEWND			//스티커
#define DF_SERVERAUTO    //서버자동
#define DF_GLBFILE_CNVS  //axglb.ini 파일 변환

#define DF_SERVERORDER_LOG "serverOrder.ini"
#define SERVER_ORDER DF_SERVERORDER_LOG
#define SERVER_ORDERMAP "IB102000"
#define SERVER_ORDER_MSGEDIT  "edServerMsg"

#define DF_POPSDI

#define DF_SHOWITGYLOG

#define DF_NUSE 0					    //#0x00 사용불가      0
#define DF_YUSE 1					     //#0x01 사용가능      1
#define DF_YUSE_NOTICEPOP 2  //#0x02 공지창도 같이 띄우기    2
#define DF_YUSE_AFTERDAY 4  //#0x02 특정날짜이후 사용중지    4
#define DF_YUSE_MANAGER 8  //#0x08 관리자전용  8
#define DF_YUSE_DEBUG   16   //#0x10 디버그기능 포함   16
#define DF_NUSE_HTSOPEN   32  //#HTS 시작할때는 안띄움
#define DF_YUSE_GUIDEPOP   64   //특정시간에는 대체팝업
/*
	#0x00 사용불가      0
	#0x01 사용가능      1
	#0x02 특정날짜이후 사용중지    2
	#0x04 특정날짜이후 사용가능    4
	#0x08 관리자전용                        8
	#0x10 디버그기능 포함                16
	#0x20 시작할때만 안띄움             32
	*/

void WriteLog_File(CString sFile, CString sData, ...)
{
	return;
	sFile.TrimRight();
	if (sFile.IsEmpty())
		sFile = DF_SERVERORDER_LOG;

	CString spath;
	spath.Format("%s%s%s", Axis::home, "\\exe\\", sFile);

	FILE* fp;
	fopen_s(&fp, spath, "a+");
	if (!fp)
		return;

	const CTime time = CTime::GetCurrentTime();
	fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

	va_list argptr;
	va_start(argptr, sData);
	vfprintf(fp, sData, argptr);
	va_end(argptr);
	fprintf(fp, "\n");

	fclose(fp);
}

void WriteUpLog(LPCSTR sfile, LPCSTR log, ...)
{
	return;
	TRY
	{
		CString stfFile;
		stfFile = Axis::home + "\\user\\" + Axis::user + "\\Crashlog\\" + sfile;

		FILE* fp;
		fopen_s(&fp, stfFile, "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
}

void WriteLog( LPCSTR log, ... )
{
	CString slog;

	va_list args;
	va_start(args, log);
	slog.FormatV(log, args);   // log를 포맷 문자열로, 뒤 인자들을 채운다
	va_end(args);

	output_DebugString(slog);
#if 0
	TRY
	{
		CString slog;
	slog.Format("[AXIS][WriteLog] [%s]\n", log);
	//OutputDebugString(slog);
	
		FILE * fp;
	    fopen_s(&fp, Axis::home + "\\exe\\axis.log", "a+");
		if (!fp) return;
		
		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));
		
		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");
		
		fclose(fp);
	}
	CATCH (CMemoryException, e)
	{
		
	}
	END_CATCH
#endif
}

BOOL DeleteDirectory( LPCTSTR lpDirPath )
{
	if( lpDirPath == NULL )
		return FALSE;
	
	BOOL bRval = FALSE;
	const int nRval = 0;
	CString szNextDirPath = _T("");
	CString szRoot = _T("");
	CFileFind find;
	
	// 폴더가 존재 하는 지 확인 검사
	bRval = find.FindFile( lpDirPath );
	
	if( bRval == FALSE )
		return bRval;
	
	while( bRval )
	{
		bRval = find.FindNextFile();
		
		if( find.IsDots() == TRUE )
			continue;
		
		if( find.IsDirectory() )
		{
			szNextDirPath.Format(_T("%s\\*.*") , find.GetFilePath() );
			DeleteDirectory( szNextDirPath );
		}
		else
		{
			::DeleteFile( find.GetFilePath() );
		}
	}
	szRoot = find.GetRoot();
	find.Close();
	bRval = RemoveDirectory( szRoot );         	
	return bRval;
}

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
	ON_WM_DESTROY()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
	ON_WM_COPYDATA()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_APP_BASE, ID_APP_END, OnAppDummy)
	ON_COMMAND_RANGE(ID_MENU_BASE, ID_MENU_END, OnAppDummy)
	ON_UPDATE_COMMAND_UI_RANGE(ID_APP_BASE, ID_APP_END, OnUpdateRunCommand)
	ON_MESSAGE(WM_AXIS, OnAXIS)
	ON_MESSAGE(WM_USER, OnUSER)
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
	ON_MESSAGE(WM_CHASER, OnCHASER)
	ON_MESSAGE(WM_ELOG, OnELOG)
	ON_MESSAGE(WM_TRAYEVENT, OnTrayEvent)
	ON_MESSAGE(WM_APPLYACC, OnApplyACC)
	ON_MESSAGE(WM_SAVEACC, OnSaveACC)
	ON_MESSAGE(WM_AXISCLOSE, OnAxisClose)
	ON_MESSAGE(WM_PHONEPAD, OnPhonePad)
	ON_MESSAGE(WM_REFRESH813, OnRefresh813)
	ON_MESSAGE(WM_SECUREDLG, OnSecureDlg)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_MESSAGE(WM_SECURETOOLS, OnSecureChange)
	ON_MESSAGE(WD_MINICLOSE, OnMiniClose)
	ON_MESSAGE(WD_HTSCLOSEFORMINI, OnHTSCloseFMini)
	ON_MESSAGE(WD_ISDEV, OnIsDev)
	ON_MESSAGE(WD_STAFF, OnStaff)
	ON_MESSAGE(WM_IBK, OnIBKWine)
	ON_MESSAGE(WM_IBK_CODE, OnIBKCode)
	ON_MESSAGE(WM_IBKWINES, OnIBKWineS)
	ON_MESSAGE(WM_IBK_CODEWINES, OnIBKCodeS)
	ON_MESSAGE(WM_IBK_NEWSWINES, OnIBKWineSNews)
	ON_MESSAGE(WM_IBK_KIUPWINES, OnIBKWineSKiup)
	ON_MESSAGE(WX_HOTTRADE, OnHotTrade)
	ON_MESSAGE(WD_FOCUSNOTICE, OnFocusNotice)
	ON_MESSAGE(WD_WEBHWND, OnWebHwnd)
	ON_MESSAGE(WD_CLOSEWEB, OnCloseWeb)
	ON_MESSAGE(WD_GOODORDER, OnGoodOrder)
	ON_MESSAGE(WD_PHONEPAD, OnPhonePadNating)
	ON_MESSAGE(WM_SECURE, OnLoadSecure)
	ON_MESSAGE(WM_RESINTEREST, OnResInterest)
	ON_MESSAGE(WD_TOP10, OnTop10)
	ON_MESSAGE(WD_LOCKPASS, OnLockPass)
	ON_MESSAGE(WM_INTERMSG, OnInterMsg)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainFrame, CFrameWnd)
	//{{AFX_EVENTSINK_MAP(CFrameWnd)
	ON_EVENT(CMainFrame, (UINT)-1, 1, OnFireRec, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//ASTx Callback Function
int __stdcall STSDKEX_EventCallback(long lCode, void* pParam, long lParamSize)
{
	UNREFERENCED_PARAMETER(pParam);
	UNREFERENCED_PARAMETER(lParamSize);

	switch(lCode)
	{
					
		case STSDKEX_PB_CALLBACK_ABNORMAL_MEMORY_ACCESS:
			{
				if(pParam)
				{
					PST_STSDKEX_PB_PROCESS_PATH phpf = (PST_STSDKEX_PB_PROCESS_PATH)pParam;
#pragma warning (disable : 26496)					
					USES_CONVERSION;
#pragma warning (default : 26496)		
					TRACE(_T("[STSDKEX_PB_Callback] STSDKEX_PB_CALLBACK_ABNORMAL_MEMORY_ACCESS(#%ld,'%s')\n"),  
						phpf->ulPID, A2T(phpf->szProcessName));
				}
				AfxMessageBox(_T("메모리에 허가되지 않은 접근을 시도했습니다."));
				
				// 프로그램 종료 코드
				//theApp.quitApplication();
			}
			break;
		case STSDKEX_PB_CALLBACK_REMOTE_DETECT:
			{
				AfxMessageBox(_T("원격 접근을 감지하였습니다."));
			}
			break;
		case STSDKEX_PB_CALLBACK_REMOTE_BLOCKD:
			{
				AfxMessageBox(_T("원격 접근을 차단하였습니다."));
			}
			break;

		case STSDKEX_PB_CALLBACK_START:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_PB_CALLBACK_START dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);
			}
			break;
			
		case STSDKEX_PB_CALLBACK_STOP:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_PB_CALLBACK_STOP dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);
			}
			break;
			
		case STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);
			}
			break;
			
		case STSDKEX_PB_CALLBACK_PROTECT_PROCESS_INS:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_PB_CALLBACK_PROTECT_PROCESS_INS dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);
			}
			break;
			
		case STSDKEX_PB_CALLBACK_PROTECT_PROCESS_DEL:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_PB_CALLBACK_PROTECT_PROCESS_DEL dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);

			}
			break;
			
		case STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS_INS:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS_INS dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);
			}
			break;
			
		case STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS_DEL:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS_DEL dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);
			}
			break;
			
		case STSDKEX_FW_CALLBACK_START:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_FW_CALLBACK_START dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);
			}
			break;
			
		case STSDKEX_FW_CALLBACK_STOP:
			{
			    const DWORD dwErrCode = (DWORD)pParam;
				CString strMsg;
				strMsg.Format(_T("STSDKEX_FW_CALLBACK_STOP dwErrCode:(%x)"), dwErrCode);
				OutputDebugString(strMsg);
			}
			break;

		default :
			{
				CString strMsg;
				strMsg.Format(_T("해킹방지 기능에 문제가 발생했습니다(lCode=0x%08x)"), lCode);
				OutputDebugString(strMsg);
			}
			break;
	} // end of switch

	return 0;
}

BOOL CALLBACK MyInfoEnumProc(HMONITOR hMonitor,  HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFO lpmi ;
	memset(&lpmi,0,sizeof(MONITORINFO));
	lpmi.cbSize = sizeof(MONITORINFO);
	if(!GetMonitorInfo(hMonitor,  &lpmi ))
		return FALSE;

	if (g_monRc.left  > lpmi.rcWork.left)	g_monRc.left   = lpmi.rcWork.left;
	if (g_monRc.top   > lpmi.rcWork.top)	g_monRc.top    = lpmi.rcWork.top;
	if (g_monRc.right < lpmi.rcWork.right)	g_monRc.right  = lpmi.rcWork.right;
	if (g_monRc.bottom< lpmi.rcWork.bottom)	g_monRc.bottom = lpmi.rcWork.bottom;
	return TRUE;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		const MSG*	msg = (MSG *) lParam;
		switch (msg->message)
		{
		case WM_LBUTTONDOWN:
			m_pMain->HideGuide();
		case WM_KEYDOWN:
		case WM_KEYUP:			
		case WM_MOUSEMOVE:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			if (!m_pMain->m_block)
			{
				if (m_pMain->m_idletime > 0)
				{
					m_pMain->KillTimer(TM_IDLE);
					m_pMain->SetTimer(TM_IDLE, m_pMain->m_idletime*60000, NULL);
				}
			}
			break;
		default:
			break;
		}

		const int	index = 0;
		switch (msg->message)
		{
			case WM_KEYDOWN:
				if (msg->wParam == VK_RETURN)
				{
					if (!(msg->lParam & 0x1000000))	// center enter key
					{
						if (m_pMain->TransKey(msg->wParam))
							return TRUE;
					}
				}
				if (msg->wParam == VK_TAB && (GetKeyState(VK_CONTROL) & 0x8000))
				{
					//m_pMain->MDINextChild();
					return TRUE;
				}
				if (msg->wParam == VK_F8/*byte('P')*/ && (GetKeyState(VK_CONTROL) & 0x8000))
				{
					//m_pMain->update_ticker(0,"S0000	047	1	014	506727	048	20100527	301	 	015	윤흡 한백 대표 `이달의 기능한국인` 선정                                                                                 	016	20100527110009    273724	041	13	042	9	044	173339	022	 	045	인물/동정	046	헤럴	");
					//폰패드장비에 비밀번호 읽어옴
					if(!Axis::isCustomer)
					{	
						//m_pMain->ReadPhonePad(0);
						m_pMain->RunPhonePad();
						/*
						if (!m_pMain->m_phone_dlgs)
						{
							m_pMain->m_phone_dlgs = new CPhonePad(NULL);
							m_pMain->m_phone_dlgs->DoModal();
							m_pMain->m_phone_dlgs = NULL;
						}
						*/
					
						return TRUE;
					}
				}
				break;
			case WM_SYSKEYDOWN:
				switch (msg->wParam)
				{
					case 49:case 50:case 51:case 52:case 53:case 54:
						if (!(GetKeyState(VK_MENU) & 0x8000))	
							break;
						m_pMain->Change_VsKey(msg->wParam - 49);
						return TRUE;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	return CallNextHookEx(m_pMain->m_hHook, nCode, wParam, lParam);
}

// protect input for check readding 
LRESULT CALLBACK viewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const CView* view = (CView *)GetWindowLong(hwnd, GWL_USERDATA);
	if (view == NULL)	return 0;
	switch (msg)
	{
		// for check reader
	case WM_KEYUP: case WM_KEYDOWN: case WM_CHAR:
		if (m_pMain->m_view)	
			return TRUE;
		break;
	default:
		break;
	}

	return ::CallWindowProc((long (__stdcall *)(HWND, UINT, WPARAM, LPARAM))m_pMain->m_callproc, hwnd, msg, wParam, lParam);
}

bool axiscall(int msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMain == (CMainFrame *) 0) return false;
	//TRACE("[%d]\n",msg);
	CString		tmpS;
	switch (msg)
	{
	case AXI_CHANGEVIEW:	
		//OutputDebugString("AXI_CHANGEVIEW\n");
		if(m_pMain->ScreenCheck((char*)lParam) == 0)
			return false;
		return (m_pMain->m_mapHelper->ChangeChild((char *)lParam, TRIGGERN, wParam) 
									? true : false);
	case AXI_SETACCOUNT:		
		return (m_pMain->SetToolAccount((char *)lParam, wParam) 
									? true : false);
	case AXI_CHANGEPOPUP:		
		return m_pMain->m_mapHelper->CreatePopup((char *)lParam, TRIGGERN, winK_POPUP, 
			1, 0, CPoint(-1, -1), false)
									? true : false;
	case AXI_SELCLIST:			return m_pMain->select_SName((char *)lParam);
	case AXI_CHANGEFOCUS:	
		{
		   const int nFlag = (int)lParam;

			if(nFlag == 0)
				m_pMain->Setfocus_Child(wParam);
			else
				m_pMain->Setfocus_Child(wParam,TRUE);
		}
		break;
	case AXI_ACTIVEMAP:			break;
	case AXI_SHOWHELP:	
		m_pMain->m_helpKey = (int) wParam;
		m_pMain->PostMessage(WM_AXIS, MAKEWPARAM(axSHOWHELP, 0), 0);
		break;
	case AXI_APPSWITCH:	m_pMain->AppSwitch();			break;
	case AXI_NEWSLIST:	m_pMain->show_Newslist(wParam, lParam);	break;
	case AXI_CHANGEBAR2:
		if (!wParam)	
			m_pMain->change_Skin();
		else		
			m_pMain->change_VirtualScreen((int) lParam);
		break;
	case AXI_SETTRIGGER:		m_pMain->set_Trigger((int) wParam, (int) lParam);	break;
	case AXI_RESTORE:			m_pMain->restore((int) wParam);		break;
	case AXI_HOTKEY:			return m_pMain->HotKey(wParam, lParam);
	case AXI_CONSEE:			m_pMain->process_ConSee((int) lParam);	break;
	case AXI_SMCALL:			m_pMain->SmCall((int) lParam);		break;
	case AXI_FUNCKEY:	
		{
			if(LOWORD(wParam) == CTRL_FUNC + 16)
			{
				const CRect rc = (LPRECT)lParam;
				
// 				CString s;
// 				s.Format("TOP10 AXI [%d] [%d] [%d] [%d]\n",rc.top,rc.left,rc.right,rc.bottom);
// 				OutputDebugString(s);
				m_pMain->m_TOP10Rect.top = rc.top;
				m_pMain->m_TOP10Rect.left = rc.left;
				m_pMain->m_TOP10Rect.right = rc.right;
				m_pMain->m_TOP10Rect.bottom = rc.bottom;
			}

			m_pMain->funcKey(LOWORD(wParam), (int) lParam);		
		}
		break;
	case AXI_TABCHANGE:
		m_pMain->ChangeTabView((LPCSTR)lParam, (int)wParam);
		break;
	case AXI_SHOWDEAL:			
	{
		m_pMain->ShowConclusion();
#ifdef DF_SERVERAUTO
		m_pMain->ShowServerOrdDlg();
#endif
	}		
	break;
	case AXI_BAR2ACTION:		m_pMain->Action(wParam, lParam);	break;
	case AXI_SETALL:			m_pMain->childAll((char *) lParam);	break;
	case AXI_GETMAPNUM:			m_pMain->GetDispN((char *) lParam);	break;
	case AXI_APPENDALLMAP:		m_pMain->AppendAllMap();		break;
	case AXI_SENDTICKINFO:		m_pMain->sendTicInfo();			break;		// lwj 2007.09.10 티커 재조회를 위해
	case AXI_INPUTSCREENNO:		return m_pMain->InputScreenNo((char *)lParam) ? true : false;
	case AXI_EDITUSERTOOL:		m_pMain->EditTool();			break;
	case AXI_TICKSETUP:			m_pMain->SetTicker();			break;
	case AXI_GETCAPTIONINFO:	m_pMain->GetCaptionInfo(wParam, lParam);break;
	case AXI_HIDECAPTIONBUTTON:	m_pMain->HideChildButton((int) lParam);		break;
	case AXI_EACHTICKER:		m_pMain->SettingEticker((int) lParam);	break;
	case AXI_LINKOPEN:			m_pMain->LinkOPEN((int) wParam, (char *) lParam);	break;
	case AXI_STOPLOSS:			m_pMain->StopLoss();			break;
	case AXI_FOSTOPLOSS:		m_pMain->FOStopLoss();			break;
	case AXI_HELPCOM:			m_pMain->RunHelpCom();			break;
	case AXI_REPORT:			m_pMain->ErrReport("사용자 장애신고");	break;
	case AXI_CLOSECHILD:
		if (lParam) // 선택창 제외 모든창 닫기인 경우
		{
			m_pMain->Setfocus_Child(wParam); 
			m_pMain->m_mapHelper->closeAllWithoutThis();  //** 선택창을 제외한 모든 창 닫기.
		}
		else
		{
			m_pMain->m_mapHelper->closeChild((int) wParam);	// cej
		}
		break;
	case AXI_CLOSESCREEN:		m_pMain->DeleteAllScreen(false);	break;		// cej
	case AXI_MINIMIZESCREEN:
		{
			//AfxMessageBox("Minimize All");
			m_pMain->m_mapHelper->minimizeAll(false);
		}
		break;
	case AXI_CUSTOMER:			return m_pMain->m_bCustomer;		break;		// cej
	case AXI_SHOWTICKER:		m_pMain->show_Ticker(lParam);		
		break;
	case AXI_HIDETICKER:		m_pMain->hide_Ticker(lParam);
		break;
	case AXI_PRINTSCREEN:		m_pMain->SendMessage(WM_COMMAND, ID_APP_PRINT, 0);	break;
	case AXI_GETVOIDRECT:		m_pMain->VoidRect((CRect*)lParam);			break;
	case AXI_MDINEXT:			m_pMain->MDINextChild();		break;
	case AXI_LASTMAPS:			m_pMain->SetLastMaps((char*)wParam, (char*)lParam);	break;
	case AXI_GETSCRBUTTONPOS:	m_pMain->GetScrButtonPos((int)wParam, (LPRECT)lParam); break;
	case AXI_RECALCLAYOUT:		m_pMain->RecalcFrame();
	case AXI_ROTATEVIEW:		m_pMain->RotateView((int)lParam);
	case AXI_CHANGETRIGGER:		m_pMain->SendTrigger((char*)wParam); break;
	case AXI_DELHISTORY	:		m_pMain->deleteHistory((char*)wParam); break;
	case AXI_DELHISTORYALL:		m_pMain->HideHistoryBar(); break;//m_pMain->deleteAllHistory(); break;
	case AXI_SHOWMAP:	m_pMain->ShowHistoryMap(wParam,lParam); break;
	case AXI_CLOSEMAP:	m_pMain->CloseHistoryMap(wParam,lParam); break;
	case AXI_ADDINTERHISTORY : m_pMain->AddCodehistoryToInter((int)wParam,(char*)lParam); break;
	case AXI_SEARCH_GROUPLIST: m_pMain->sendTR("PIDOMYST", (char*)lParam, HIWORD(wParam), US_PASS, 251); break;
	case AXI_SEARCH_GROUPCODE: m_pMain->sendTR("PIDOMYST", (char*)lParam, HIWORD(wParam), US_PASS, 250); break;
		default:				break;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
static CMainFrame* g_pMainFrame = nullptr;
CMainFrame::CMainFrame()
{
	m_pMain				= (CMainFrame *) 0;
	m_resourceHelper	= ResourceHelper();
#ifdef DF_USE_CPLUS17
	m_mapHelper = std::make_unique<CMapHelper>(this);
#else
	m_mapHelper			= new CMapHelper(this);
	m_logodlg = NULL;
#endif
	m_axConnect = NULL;
	m_axConnectOld = NULL;
	
	m_TotalAcc = NULL;

	//srand( (unsigned)time( NULL ) );
	m_rndkey = GetTickCount();

	m_preFontSize = 0;
	bBackFont     = false;
	
	//m_hjcode.RemoveAll();
	//m_fjcode.RemoveAll();
	//m_ojcode.RemoveAll();
	//m_pjcode.RemoveAll();
	//m_upcode.RemoveAll();
	//m_elwbase.RemoveAll();

	//loadingHJcode();
	//loadingFJcode();
	//loadingOJcode();
	//loadingPJcode();
	//loadingUPcode();
	//loadingELWcode();
	
	// 다른ID로 재접속, 20060117
	m_bReconnect = false;

	m_hMRadar	= NULL;
	m_mapN		= _T("");
	m_update	= false;
	m_fullmode	= false;
	m_bExit		= true;
	m_runAxis	= false;
	m_noActive	= true;
	m_block		= false;
	m_forceClose	= false;
	m_bSDI		= false;
	m_saveALLVS	= true;
	//m_saveALLVS	= false;
	m_blinkbar	= true;
	m_bCustomer	= true;
	m_bSound	= false;
	m_activeCap	= FALSE;
	m_nInfoPos      = 2;
	m_bDelayIn	= false;
	m_bEnca		= false;
	m_sLastMapInfo.Empty();
	m_sDelayData.Empty();
	m_nDelayPeak	= 0;

	m_bHome = m_bEnd = TRUE;
	m_bdnInterest = true;

	m_maxChild	= 16;
	m_fontSize	= 9;

	m_idletime	= 0;
	m_posChild	= 0;
	m_toolStatus	= 0;
	m_SDIHEIGHT	= 0;
	m_activeKey	= 0;
	m_downI		= -1;
	m_appMode	= MODE_MDI;
	m_nBkMode	= BKMODE_NORMAL;

	m_conKIND	= CON_NONE;
	m_conHISTORY	= 0;
	m_conAUTOTIME	= 0;
	for (int ii = 0; ii < MAXSOUND; ii++)
		m_waveF[ii]   = _T("");
	
	m_connectBy	= byNONE;
	m_step		= axNONE;

	m_vsN		= V_SCREEN1;
	m_axis		= AfxGetApp();
	Axis::home		= m_axis->GetProfileString(ENVIRONMENT, ROOTDIR);
	const int	position= m_axis->GetProfileInt(ENVIRONMENT, CHILDPOS, 0);

	m_regkey	= ((CAxisApp*)m_axis)->m_regkey;
#ifdef DF_USE_CPLUS17
	m_axMisc = std::make_unique<CAxMisc>(Axis::home, m_regkey);
	m_axMisc->m_pMain = this;
	m_axGuide = std::make_unique<CAxGuide>(LOWORD(position), Axis::home);
#else
	m_axMisc	= new CAxMisc(Axis::home, m_regkey);
	m_axGuide = new CAxGuide(LOWORD(position), Axis::home);
	m_logodlg   = NULL;
#endif
	
	m_MClient	= new CMDIClient(axiscall);
	m_wizard	= NULL;
	m_tMenu		= NULL;
	m_bar0		= NULL;
	m_bar1		= NULL;
	m_bar2		= NULL;
	m_bar3		= NULL;
	m_tInfo1	= NULL;
	m_tInfo2	= NULL;
	m_tInfo3	= NULL;
	m_smain		= NULL;
	m_sdibar	= NULL;
	m_axConnect	= NULL;
	m_axConnectOld = NULL;
	
	m_TotalAcc	= NULL;
	m_modal		= NULL;
	m_WBWnd		= NULL;
	m_hHook		= NULL;
	m_view		= NULL;
	m_conclusion	= NULL;
	m_kobanotify	= NULL;
	m_Econclusion	= NULL;
	m_mngInfo	= NULL;
	m_top10		= NULL;
	m_smcall	= NULL;
	m_Nclock	= NULL;
	m_infofile	= NULL;
	m_wb		= NULL;
	m_chaser	= NULL;
	m_accTool	= NULL;
	m_accSave	= NULL;
	m_category	= NULL;
	m_newsviewer	= NULL;
	m_stoploss	= NULL;
	m_foStoploss	= NULL;
	m_tip		= NULL;
	m_misuAlarm	= NULL;
	m_pDoctor	= NULL;
	m_info_interest = NULL;
	m_EmpPassChangeDlg = NULL;

	m_printsetup = new CPrinterSettings();
	m_printsetup->CopyDefaultMfcPrinter();
	m_tabviewlist.RemoveAll();
	m_waitlist.RemoveAll();

	for (int ii = V_SCREEN1; ii <= V_SCREEN6; ii++)	m_hooklist[ii].RemoveAll();
	for (int ii = V_SCREEN1; ii <= V_SCREEN6; ii++)	m_sdiZORDER[ii].RemoveAll();
	m_progK = ((CAxisApp *) m_axis)->m_progK;

	/**
	if (((CAxisApp *) m_axis)->m_mode == MD_DEV)	m_dev = true;
	if (((CAxisApp *) m_axis)->m_exeName.IsEmpty())	m_dev = true;
	**/

	for (int ii = 0; ii < MBMP_CNT; ii++)
		m_arRc.Add(CRect(0, 0, 0, 0));

	m_szRTime = _T("");

	m_saveTitle = _T("");
	m_bRTSQueue = false;

	m_bFirstOpen = true;		// 20070627 kwon
	m_bLastClose = false;		// 20070627 kwon
	m_sysInfo = new CSysInfo;	// 20070627 kwon
	m_sysInfo->Init();			// 20070627 kwon
	m_ipAddr = _T("");		
	m_sWin = _T("");
	m_sCpu = _T("");
	m_sMem = _T("");

	//m_pAosSB = NULL;

	m_winVer = 0;			// 20070724
	m_minSaveRC.SetRectEmpty();

	m_matchToolBar = FALSE;
	m_matchToolCount = 3;

	m_theme = GetMyThemeName();
	m_bfitdual = false;

	m_bKobaElwNotify = false;

	m_slideMsg = "";

	m_bInit = FALSE;

	m_miniWid = NULL;
	m_miniSWid = NULL;
	m_bMiniMode = FALSE;
	m_groupBymapN.RemoveAll();

	m_bHistory = FALSE;

	m_bMustRetry = FALSE;

	m_bClose = FALSE;

	m_bNoProtect = FALSE;

	m_bStart = false;

	m_bItgy = TRUE;

	m_b2018 = false;
	m_bLoadScreen = false;

	CString PathConf;
	PathConf.Format("%s\\%s\\CERT.INI", Axis::home, "tab");

	m_bUseNewLogin = GetPrivateProfileInt("CERTLOGIN","USE",0,PathConf);

	m_mapPBWnd.RemoveAll();

	m_ShowSlide = 1;   //촉앤토크 슬라이드 0 슬라이드 안보여줌, 1 슬라이드 보여줌

	m_viewHist = NULL;

	m_iGlbIndex = 0;

	m_arrGlb.RemoveAll();

	m_DInstallASTx = STSDKEX_ERROR_SUCCESS;

	m_bitmapBtn = nullptr;

	m_iCErrCnt = 0;

	m_bSdkInitialized = FALSE;
}

CMainFrame::~CMainFrame()
{
#ifdef _DEBUG
	return;
#endif
	if(uninitASTx())
	{
		stopPB();
		stopFW();
		stopAK();
	}
}
void CMainFrame::Check_HTS_Verstion()
{
	const int iver = AfxGetApp()->GetProfileInt(WORKSTATION, "VerMode", 1);
	if (iver < 5)
	{
		PostMessage(WM_CLOSE, 0, 0);
	}
}
void CMainFrame::SetPCData()
{
	CString	sfile, stmp, smap;
	sfile.Format("%s\\tab\\axis.ini", Axis::home);

	CString file;
	file.Format("%s\\exe\\CX_PCIdentity.dll", Axis::home);

	typedef char* (*GetPCDATA)();
	HMODULE hModule = LoadLibrary(file);
	CString sGetMAc, sGetPCData_4, sGetPCData_13, sPhyMac;   //맥주소, 하드디스크 정보식별자, MAC 정보식별자
	if (hModule)
	{
		GetPCDATA func = (GetPCDATA)GetProcAddress(hModule, "axGetPCDATA");
		char* pdata = func();
		if (pdata)
		{
			CString sdata;
			sdata.Format("%s", pdata);
			sGetMAc = Parser(sdata, "|");
			sGetPCData_4 = Parser(sdata, "|");
			sGetPCData_13 = Parser(sdata, "|");
			sPhyMac = Parser(sdata, "|");

			WritePrivateProfileString("PCDATA", "sGetMAc", sGetMAc, sfile);
			WritePrivateProfileString("PCDATA", "sGetPCData_4", sGetPCData_4, sfile);
			WritePrivateProfileString("PCDATA", "sGetPCData_13", sGetPCData_13, sfile);
			WritePrivateProfileString("PCDATA", "sPhyMac", sPhyMac, sfile);
		}

		FreeLibrary(hModule);
	}


	//DWORD dw = GetPrivateProfileString("NoTwoPop", "map", "", buf, sizeof(buf), file);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	g_pMainFrame = this;
	//if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
	//{
	//	OutputDebugString("[SetPriorityClass] Fail");
	//}

	::DeleteFile(Axis::home + "\\exe\\axis.log");

	const char obExe[] = "\x61\x78\x69\x73\x2E\x65\x78\x65";
	char wb[32]{};
	sprintf(wb, "%s", obExe);

	m_sMainName.Format("%s", wb);
	m_sMainName.TrimRight();
	m_sMainName.MakeUpper();

	m_slog.Format("[AXIS][core] OnCreate regkey=[%s] m_sMainName=[%s]", m_regkey, m_sMainName);
WriteLog(m_slog);
	InitMapHK();
	WriteMainInfo();

	CString fname;
	fname = Axis::home + "\\exe\\BLDINFO.INI";
	CFileFind finder;
	if (!finder.FindFile(fname))
	{
		WritePrivateProfileString("Build", "BuildNumber", "1.7.0.83", fname);
		WritePrivateProfileString("Build", "State", "3", fname);
		WritePrivateProfileString("Build", "Version", "1.7.0.3", fname);
	}
	else
	{
		char buf[512]{};
		DWORD dw = GetPrivateProfileString("Build", "BuildNumber", "", buf, sizeof(buf), fname);
		CString stmp;
		stmp.Format("%s", buf);
		stmp.TrimRight();
		if (stmp != "1.7.0.83")
		{
			WritePrivateProfileString("Build", "BuildNumber", "1.7.0.83", fname);
			WritePrivateProfileString("Build", "State", "3", fname);
			WritePrivateProfileString("Build", "Version", "1.7.0.3", fname);
		}
	}

	Axis::sFiller = "";
	Axis::mainWnd  = this;
	Axis::SetSkin(GetSkinName());

	m_reportCaption.Format("장애신고 119 for %s", m_regkey);	
	GetClassName(m_hWnd, m_classN, sizeof(m_classN));

	GetWindowPlacement(&m_switchpl);

#ifdef DF_USE_CPLUS17
	m_tip = std::make_unique<CTTip>(this);
	if (!m_tip->Create(NULL, "CTTip", WS_CHILD | WS_BORDER, CRect(0, 0, 0, 0), GetDesktopWindow(), (int)m_tip.get()))
	{
#else
	m_tip = new CTTip(this);
	if (!m_tip->Create(NULL, "CTTip", WS_CHILD | WS_BORDER, CRect(0, 0, 0, 0), GetDesktopWindow(), (int)m_tip))
	{
		delete m_tip;
#endif
		m_tip = NULL;
	}
	else	
		m_tip->ModifyStyleEx(0, WS_EX_TOOLWINDOW);

	m_BackGroundKey = -1;
	ModifyStyle(0, WS_SYSMENU);  
	AfxInitRichEdit();
	MakeExpectSymbolTable();
	
	const int result = Initialize();

	ResourceHelper()->LoadIcon();
	SetIcon(ResourceHelper()->GetIcon(), TRUE);

//#ifdef USE_AHNLAB_SECUREBROWSER
//
//	CString filename;
//	filename.Format("%s\\%s\\%s", Axis::home, "exe", "NOAOS.TXT");
//	FILE* fp;
//	fopen_s(&fp, filename, "rb");
//
//	int nInternalMember;
//
//	nInternalMember = 0;
//
//	if (fp)
//	{
//		nInternalMember = 1;
//		m_bNoProtect = TRUE;
//		fclose(fp);
//	}
//	else
//	{
//		GetLocalIP();
//
//		if(isIPInRange(m_ipAddr,"172.17.0.0") || isIPInRange(m_ipAddr,"172.20.0.0"))
//		{
//			nInternalMember = 1;
//			m_bNoProtect = TRUE;   
//		}
//	}
//
//	if(nInternalMember == 1)
//	{
//		AfxGetApp()->WriteProfileInt(INFORMATION, "AOS", 0);
//		AfxGetApp()->WriteProfileInt(INFORMATION, "PCFirewall", 0);
//	}
//#endif

	GetASTxInstall();
	LoadNoTwoPOP_NoSaveLast();
	m_slog.Format("[axis] oncreatend result=[%d]", result);
	OutputDebugString(m_slog);
	return result;
}

void CMainFrame::MakeExpectSymbolTable()
{
	char*	sym[] = {"X5001", "X0001", "X2001", "XQSTR", "XQ001", "XQ044", "XQ047"};
	const int	ncnt = sizeof(sym)/sizeof(char*);
	CString	str = _T("");
	
	for (int ii = 0 ; ii < ncnt ; ii++ )
	{
		str.Format("%s", sym[ii]);
		str = str.Mid(1);
		str = "K" + str;
		m_mapExpectSymbol.SetAt(sym[ii], str);
	}
}

void CMainFrame::ShowHistoryMap(WPARAM wParam, LPARAM lParam)
{
	//if (1)
	{
			if (!wParam || !lParam)
				return;

			LPCSTR xCode = reinterpret_cast<LPCSTR>(wParam);
			RECT* pRc = reinterpret_cast<RECT*>(lParam);

			if (!xCode || !pRc)
				return;

			if (!m_bar3 || !m_bar3->GetSafeHwnd())
				return;

			if (m_codeHist.CompareNoCase(xCode) == 0)
				return;

			m_codeHist = xCode;

			// ---------------------------------
			// lParam 포인터 방어 (SEH)
			// ---------------------------------
			CRect rc;
			__try
			{
				rc = *pRc;   // 여기서 AV 나면 except로 빠짐
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				m_codeHist.Empty();
				return;
			}

			// lParam은 client 좌표라고 가정 → screen 좌표로 변환
			ClientToScreen(&rc);

			// ---------------------------------
			// bar 위치 (screen 좌표)
			// ---------------------------------
			CRect barRc;
			m_bar3->GetWindowRect(&barRc); // 이미 screen 좌표

			if (m_bar3->GetBarStyle() & CBRS_ALIGN_TOP)
				rc.top = barRc.bottom;
			else
				rc.top = barRc.top - 20;

			RECT openRC{};
			openRC.left = rc.left;
			openRC.top = rc.top;
			openRC.right = openRC.left + rc.Width();
			openRC.bottom = openRC.top + 25;

			// ---------------------------------
			// 현재 프레임이 속한 모니터 기준 보정
			// ---------------------------------
			HMONITOR hMonitor = MonitorFromWindow(this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi{};
			mi.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(hMonitor, &mi);

			if (openRC.right > mi.rcWork.right)
			{
				int w = openRC.right - openRC.left;
				openRC.left = mi.rcWork.right - w;
				openRC.right = mi.rcWork.right;
			}

			if (openRC.bottom > mi.rcWork.bottom)
			{
				int h = openRC.bottom - openRC.top;
				openRC.top = mi.rcWork.bottom - h;
				openRC.bottom = mi.rcWork.bottom;
			}

			// ---------------------------------
			// view / window 체크
			// ---------------------------------
			if (!m_miniWid || !m_miniWid->GetSafeHwnd())
			{
				m_codeHist.Empty();
				return;
			}

			if (!m_viewHist || !m_viewHist->GetSafeHwnd())
				m_viewHist = m_miniWid->GetActiveView();

			if (!m_viewHist || !m_viewHist->GetSafeHwnd())
			{
				m_codeHist.Empty();
				return;
			}

			CWnd* base = m_viewHist->GetWindow(GW_CHILD);
			if (base && base->GetSafeHwnd())
			{
				// 여기서 죽는 경우는 base 내부 로직 문제
				base->SendMessage(WD_HISTORYVIEW, wParam, (LPARAM)&openRC);
			}

			m_codeHist.Empty();
	}
	/*else
	{
		LPCSTR xCode = (LPCSTR)wParam;
		CRect rc = (LPRECT)lParam;
		CRect barRc;
		CRect mainRc;

		if (xCode == m_codeHist)
			return;

		m_codeHist = xCode;
		m_bar3->GetWindowRect(barRc);
		GetClientRect(mainRc);
		ClientToScreen(barRc);
		ScreenToClient(mainRc);
		if (m_bar3->GetBarStyle() & CBRS_ALIGN_TOP)
			rc.top = mainRc.top + barRc.top + barRc.Height();
		else
			rc.top = mainRc.top + barRc.top - 20;

		RECT openRC;

		openRC.top = rc.top;
		openRC.left = barRc.left / 2 + rc.left;
		openRC.right = openRC.left + rc.Width();
		openRC.bottom = rc.top + 25;

		if (m_miniWid == NULL || !m_miniWid->GetSafeHwnd())
		{
			const char* trust = "IB0000X8";
			m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
			return;
		}
		else
		{
			if (m_viewHist == NULL || !m_viewHist->GetSafeHwnd())
			{
				m_viewHist = m_miniWid->GetActiveView();
			}
		}

		if (m_viewHist == NULL)
		{
			OutputDebugString("VIEWHIST IS NULL\n");
			return;
		}

		if (!m_bSDI)
		{
			if (m_viewHist != NULL || m_viewHist->GetSafeHwnd())
			{
				const CWnd* base{};
				try
				{
					base = m_viewHist->GetWindow(GW_CHILD);
				}
				catch (const std::exception&)
				{
					WriteLog("---------------------CMainFrame::ShowHistoryMap  try catch--------");
					return;
				}

				if (base != NULL && base->GetSafeHwnd())
					base->SendMessage(WD_HISTORYVIEW, wParam, (LPARAM)&openRC);
				else
					OutputDebugString("BASE HIST NOT SAFE\n");

				m_codeHist = "";
			}
		}
	}*/
}

void CMainFrame::CloseHistoryMap(WPARAM wParam,LPARAM lParam)
{
	if(!m_bMiniMode)
	{
		m_codeHist = "";

		if(!m_bSDI)
		{
			if(m_viewHist->GetSafeHwnd() && m_viewHist != NULL)
			{
				const CWnd* base = m_viewHist->GetWindow(GW_CHILD);

				if(base != NULL && base->GetSafeHwnd())
					base->SendMessage(WD_HISTORYCLOSE,wParam,lParam);
			}
		}
	}
}

void CMainFrame::SendTrigger(CString code)
{
	m_slog.Format("[axis][CMainFrame][SendTrigger] code=[%s]\n", code);
	OutputDebugString(m_slog);

	CString sdat;
	sdat = "1301\t"+code;

	int nActGroup = 0;

	CChildFrame* actChild{};
	CSChild* actSchild{};

	if(m_arMDI[m_vsN].Lookup(m_activeKey,actChild))
	{
		nActGroup = actChild->m_xcaption.GetGroup();
	}
	else if(m_arSDI[m_vsN].Lookup(m_activeKey,actSchild))
	{
		nActGroup = actSchild->m_xcaption.GetGroup();
	}

	CChildFrame* child{};
	CSChild* schild{};

	int	key{};
	const long	rc = 0;
	POSITION pos;

	for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
		
		if(nActGroup == child->m_xcaption.GetGroup())
		{
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sdat);
		}
	}
	
	for (pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
		
		if(nActGroup == schild->m_xcaption.GetGroup())
		{
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
				(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sdat);
		}
	}

// 	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
}

void CMainFrame::deleteHistory(CString code)
{
	CString strRet(_T(""));
	long rc;
	
	CString sdat;
	sdat = "1301\t"+code;
	
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(delHISTORY, 0), (LPARAM)(LPCTSTR)sdat);
	
	strRet = rc ? (char *)rc : _T("");

	m_bar3->del_Button(code);
}

void CMainFrame::deleteAllHistory()
{
	CString strRet(_T(""));
	long rc;
	
	CString sdat;
	sdat = "1301\t";
	
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(delHISTORY, 0), (LPARAM)(LPCTSTR)sdat);
	
	strRet = rc ? (char *)rc : _T("");

	m_bar3->del_AllButton();
	
	CreateHistoryBar();
}

void CMainFrame::HideHistoryBar()
{
	const BOOL visible = FALSE;
#ifdef DF_USE_CPLUS17
	ShowControlBar(m_bar3.get(), visible, FALSE);
#else
	ShowControlBar(m_bar3, visible, FALSE);
#endif
	saveToolStatus();
	SetSDIChangeHeight();
}

CString CMainFrame::ReplaceExpectSymbol(CString sym)
{
	CString	ret = _T("");
	
	m_mapExpectSymbol.Lookup(sym, ret);
	return ret;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(WS_SYSMENU |FWS_ADDTOTITLE);
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{//if (msg->wParam == VK_F8/*byte('P')*/ && (GetKeyState(VK_CONTROL) & 0x8000))
	if(pMsg->message == WM_KEYDOWN)
	{
		const bool bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
		const bool bShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;

		switch(pMsg->wParam)
		{
				case VK_F5:
				{
				
				}
				break;
				case 'F':
				{
					if (bCtrl && bShift)
					{
						CString	Path;
						Path.Format("%s\\%s\\ACCNTDEPT.INI", Axis::home, "tab");

						char readB[1024];
						int readL;
						readL = GetPrivateProfileString("ACCNTDEPT", "DEPT", "811", readB, sizeof(readB), Path);
						CString tDept(readB, readL);
						tDept.TrimLeft(); tDept.TrimRight();

						readL = GetPrivateProfileString("ACCNTDEPT", "KEYINPUT", "0", readB, sizeof(readB), Path);
						CString stmp(readB, readL);
						m_slog.Format("[AXIS] tDept =[%s] m_Dept=[%s] keyin=[%s]", tDept, m_dept, stmp);
						WriteLog(m_slog);

						if (stmp == "1")
						{
							if (ScreenCheck("GROUPACCSAVEKEY"))
							{
								//ShowControlBar(m_TotalAcc, TRUE, FALSE);
								//m_TotalAcc->Refresh813(1);
							}
						}
					}

				/*	if (!m_pUpload)
						m_pUpload = std::make_unique<class CUploadFile>(m_wizard.get());

					if (1)
					{
						CString sfile;
						sfile.Format("%s\\%s.ini", Axis::home + "\\user\\" + Axis::user, Axis::user);
						m_pUpload->uploadFile(sfile);		
					}*/


					/*	CString str, title;
						str= "901	00110012107	902	devilswo \
							904	29 \
							996	11	988	체결	923	14420233	906	테스트계좌	922	3	905	0	997	HTS	912	매도	907	A005930	908	삼성전자                                	992	453	931	1	916	72100	909	1	910	0	921	0	975	   0	924 - 현금매도	925	시장가	926	KSE	993	00	911	00	994		995	0	933	0	999	1	974	0	984	10	023 - 55500";
						ConclusionNotice(str, title);*/
						//RunHelpCom();
						//	m_axMisc->RunVers(verRETRY, Axis::userID, m_pass, m_cpass);
						//m_axMisc->RunVers(verUPDATE, "", "", "");
				}
				break;
				case 'X':
				{
					if (bCtrl && bShift)
					{
						char	buf[512];
						CString	file, stmp;
						file.Format("%s\\tab\\axis.ini", Axis::home);
						DWORD dw = GetPrivateProfileString("PHONEPAD", "HOTKEY", "", buf, sizeof(buf), file);

						if (dw > 0)
						{
							stmp.Format("%s", buf);
							stmp.TrimRight();
							if (stmp != "1") return 0;

							CString sdat, dat;
							dat = m_strPhone;
							sdat = "zPwd\t" + dat;

							CChildFrame* actChild{};
							CSChild* actSchild{};

							if (m_arMDI[m_vsN].Lookup(m_activeKey, actChild))
							{
								m_slog.Format("[phonepad] actChild->m_mapN = [%s]", actChild->m_mapN);

							}

							m_slog.Format("[phonepad] OnPhonePadNating dat=[%s] m_activeKey=[%d] ", dat, m_activeKey);
							OutputDebugString(m_slog);

							m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
								(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
							sdat = "pswd\t" + dat;
							m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
								(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
							sdat = "ed_pswd\t" + dat;
							m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
								(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
						}
					}
				}
				break;
				case 'D':
				{
					/*	int nCount = m_mapAlarmList.GetCount();
						m_slog.Format("[mng][main]] Map Count = %d\n", nCount);
						OutputDebugString(m_slog);

						POSITION pos = m_mapAlarmList.GetStartPosition();
						while (pos != NULL)
						{
							CString key, value;
							m_mapAlarmList.GetNextAssoc(pos, key, value);

							if (value == "1")
							{
								m_slog.Format("[mng][main]Key = %s Value = %s\n", key, value);
								OutputDebugString(m_slog);
							}
						}*/
				}
				break;
				case 'Z':
				{
					//CString sPath;
					//CString strFilePath;
					//CFile	file;
					//CString sBuf;
					//TCHAR	chFileName[128]{};
					//GetModuleFileName(NULL, chFileName, MAX_PATH);

					//strFilePath.Format(_T("%s"), chFileName);
					//strFilePath = strFilePath.Left(strFilePath.ReverseFind('\\'));
					//strFilePath.Replace("\\exe", "\\user");

					//DecryptAllUserIni(strFilePath);
					/*	CString stmp, stitle;
						stmp.Format("950\t3\t951\t20240708173000\t952\t조건 만족 주문내역 확인");
						ConclusionNotice(stmp, stitle);
						stmp.Format("950\t6\t951\t20240708173000\t952\t조건만료 감시내역 확인");
						ConclusionNotice(stmp, stitle);*/
						//ref 테스트코드
						//enum en_gnbn { ACC = 0, AI, AUTOORDERLIST, SERVERORDADD };  //계좌입력기  ,  AI버튼,   자동주문리스트
						//int ival = AUTOORDERLIST;
						//switch (ival)
						//{
						//	case ACC:
						//	{
						//		ShowControlBar(m_TotalAcc, TRUE, FALSE);
						//	}
						//	break;
						//	case AI:
						//	{
						//		char	buf[256]{};
						//		CString	file, strmap, strname;
						//		file.Format("%s\\tab\\axisAI.ini", Axis::home);
						//		DWORD dw = GetPrivateProfileString("MAP", "num", "IB999900", buf, sizeof(buf), file);
						//		strmap.Format("%s", buf);
						//		strmap.TrimRight();

						//		memset(buf, 0x00, 256);
						//		dw = GetPrivateProfileString("MAP", "name", "IBK AI", buf, sizeof(buf), file);
						//		strname.Format("%s", buf);
						//		strname.TrimRight();

						//		CString strmenu;
						//		strmenu.Format("도우미\t#%s\t%s\t%s", strname, strmap, strname);
				}
				break;
				case 'S':
				{
					if (bCtrl && bShift)
					{
						/*std::wstring plain = L"!O7#8aksjdf67h53";
						DATA_BLOB in;
						in.pbData = (BYTE*)plain.data();
						in.cbData = static_cast<DWORD>(plain.size() * sizeof(wchar_t));

						DATA_BLOB out;
						if (!CryptProtectData(&in, nullptr, nullptr, nullptr, nullptr, 0, &out))
						{
							AfxMessageBox("CryptProtectData error");
							break;
						}*/
						//DumpAllSlots(g_tickSlots, MAX_SLOT, 100, 50);
						//CreateAgentProcess();
						//DumpAllSlots();
					}
				}
				break;
				case 'A':
				{
					if (bCtrl && bShift)
					{
						//	/*		m_slog.Format("msg=%u wParam=%u (0x%X)\n", pMsg->message, pMsg->wParam, pMsg->wParam);
						//			AfxMessageBox(m_slog);*/

						//	if (GetKeyState(VK_CONTROL) & 0x8000)
						//	{
						//		if (GetKeyState(VK_SHIFT) & 0x8000)
						//		{
						//			if (m_pSharedMemory)
						//				(int)(m_pSharedMemory->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(MMSG_SHARED_DUMP, 1), 1), 0));
						//			//_vMngInfo.clear();
						//			//_vMngInfo.emplace_back(std::move("881"), std::move("881"));  //장마감 프리
						//			//_vMngInfo.emplace_back(std::move("851"), std::move("851"));  //시간외 프리
						//			//_vMngInfo.emplace_back(std::move("803"), std::move("803")); //장마감 프리
						//			//_vMngInfo.emplace_back(std::move("883"), std::move("883"));  //장마감 장마감
						//			//_vMngInfo.emplace_back(std::move("801"), std::move("801"));  //정규장 정규장
						//			//_vMngInfo.emplace_back(std::move("886"), std::move("886"));  //정규장 장마감
						//			//_vMngInfo.emplace_back(std::move("887"), std::move("887"));  //장마감 단일가
						//			//_vMngInfo.emplace_back(std::move("888"), std::move("888"));   //시간외 애프터 
						//			//_vMngInfo.emplace_back(std::move("805"), std::move("805"));  //단일가 애프터
						//			//_vMngInfo.emplace_back(std::move("806"), std::move("806"));  //장마감 애프터
						//			//_vMngInfo.emplace_back(std::move("889"), std::move("889"));  //장마감 장마감
						//			//
						//			//std::thread([this]()
						//			//{
						//			//	CString slog;
						//			//	for (const auto& [key, value] : _vMngInfo)
						//			//	{
						//			//		CString stmp;
						//			//		stmp.Format("%s", value);

						//			//		slog.Format("[mnginfo][%s][%d] before postmessage stmp = [%s]", __FUNCTION__, __LINE__, stmp);
						//			//		OutputDebugString(slog);

						//			//		CString* pstr = new CString(stmp);
						//			//		PostMessage(WM_USER, MMSG_MAIN_TEST, reinterpret_cast<LPARAM>(pstr));
						//			//		Sleep(1000);
						//			//	}
						//			//}).detach();
						//		}
						//	}
					}
				}
				break;
				default:
					break;
			}	
	}
	

	if (m_tMenu && m_tMenu->PreTranslateMsg(pMsg))
		return TRUE;

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CWindowDC dc(this);
	CRect	rc;
	int	nItem{};
	CPoint	point = CPoint(LOWORD(lParam), HIWORD(lParam));

	switch (message)
	{
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
				/*
				if (Axis::isVista)
				{
					rc.left    += VistaFrame;
					rc.right   += VistaFrame;
					rc.top     += VistaFrame;
					rc.bottom  += VistaFrame;
				}
				*/
				//***rc.OffsetRect(XGAP, YGAP);
				switch (nItem)
				{
				case MIDX_HOME:	case MIDX_FULL:	case MIDX_DUAL:
				case MIDX_MIN:	case MIDX_MAX:	case MIDX_CLOSE:
				case MIDX_RESTORE:
					m_downI = nItem;
					m_action = nItem;

					DrawBitmapByMask(&dc, nItem, rc, true);
					break;
				case MIDX_VS1:	case MIDX_VS2:	case MIDX_VS3:
				case MIDX_VS4:	case MIDX_VS5:	case MIDX_VS6:
					m_action = nItem;
					PostMessage(WM_AXIS, MAKEWPARAM(axCAPTION, (int) m_action), -1);
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
				//***rc.OffsetRect(XGAP, YGAP);
				switch (nItem)
				{
				case MIDX_HOME:	case MIDX_FULL:	case MIDX_DUAL:
				case MIDX_MIN:	case MIDX_MAX:	case MIDX_CLOSE:
				case MIDX_RESTORE:
					m_downI = nItem;
					m_action = nItem;
					DrawBitmapByMask(&dc, nItem, rc, true);
					break;
				case MIDX_VS1:	case MIDX_VS2:	case MIDX_VS3:
				case MIDX_VS4:	case MIDX_VS5:	case MIDX_VS6:
					m_action = nItem;
					PostMessage(WM_AXIS, MAKEWPARAM(axCAPTION, (int) m_action), -1);
					break;
				default:
					break;
				}
				return TRUE;
			}
			return TRUE;
		default:
			break;
		}
		break;
	case WM_NCMOUSEMOVE:
		switch (wParam)
		{
		case HTCLOSE:
		case HTMAXBUTTON:
		case HTMINBUTTON:
			return TRUE;
		default:
			break;
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
	default:
		break;
// 	case WM_QUERYENDSESSION
// 		{
// 			//save_laststat();
// 		}
// 		break;
	}
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint		pt;
	int		nItem{};
	CRect		rc;
	CWindowDC	dc(this);

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
			/*
			if (Axis::isVista)
			{
				rc.left    += VistaFrame;
				rc.right   += VistaFrame;
				rc.top     += VistaFrame;
				rc.bottom  += VistaFrame;
			}
			*/
			//***rc.OffsetRect(XGAP, YGAP);
			DrawBitmapByMask(&dc, m_downI, rc);

			m_action = -1;
			m_downI = -1;
			break;
		case WM_NCMOUSEMOVE:
			break;
		case WM_MOUSEMOVE:
			GetCursorPos(&pt);

			if(m_top10 != nullptr && m_top10->GetSafeHwnd())
			{
				CRect rc;

				m_top10->GetWindowRect(rc);

				if(rc.PtInRect(pt))
				{
					if(m_top10->m_bShown)
					{
						m_top10->ShowWindow(SW_HIDE);
					}
				}
			}

			nItem = GetSelectItem(pt);
			if (nItem == m_downI)
			{
				rc = m_arRc.GetAt(nItem);
				/*
				if (Axis::isVista)
				{
					rc.left    += VistaFrame;
					rc.right   += VistaFrame;
					rc.top     += VistaFrame;
					rc.bottom  += VistaFrame;
				}
				*/
	
				//***rc.OffsetRect(XGAP, YGAP);
				switch (nItem)
				{
				case MIDX_HOME:	case MIDX_FULL:	case MIDX_DUAL:
				case MIDX_MIN:	case MIDX_MAX:	case MIDX_CLOSE:
				case MIDX_RESTORE:
					DrawBitmapByMask(&dc, nItem, rc, true);
					break;
				default:
					break;
				}
			}
			else
			{
				rc = m_arRc.GetAt(m_downI);
				/*
				if (Axis::isVista)
				{
					rc.left    += VistaFrame;
					rc.right   += VistaFrame;
					rc.top     += VistaFrame;
					rc.bottom  += VistaFrame;
				}
				*/
				//***rc.OffsetRect(XGAP, YGAP);
				DrawBitmapByMask(&dc, m_downI, rc);
			}

			ShowToolTip(nItem, pt);
			break;
		}
	}

	switch (message)
	{
	case WM_MOUSEMOVE:
	//case WM_NCMOUSEMOVE:
		GetCursorPos(&pt);
		nItem = GetSelectItem(pt);
		ShowToolTip(nItem, pt);
		break;
	}
	return CMDIFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainFrame::ShowToolTip(int nIndex, CPoint pt)
{
	if (nIndex < 0 || !m_tip)	return;
	//if (!(GetStyle() & WS_CAPTION))	return;
	
	CString		tipS;

	switch (nIndex)
	{
	case MIDX_HOME:	tipS =  _T("홈페이지");		break;
	case MIDX_FULL:	tipS =  _T("전체화면");		break;
	case MIDX_DUAL:	tipS =  _T("듀얼모니터");	break;
	default:
		if (nIndex >= MIDX_VS1 && nIndex <= MIDX_VS6)
		{
			const int index = nIndex - MIDX_VS1;
			tipS.Format("가상화면 %d(ALT+%d)", index + 1, index + 1);
			tipS += GetScreenList(index);
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
	m_tip->ShowTips(tipRc, wRc, tipS);
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
BOOL CMainFrame::OnNcActivate(BOOL bActive) 
{
	CMDIFrameWnd::OnNcActivate(bActive);
	m_activeCap = bActive;
	OnNcPaint();
	if (Axis::isVista) Invalidate(); // for VISTA
	return TRUE;//*/return CMDIFrameWnd::OnNcActivate(bActive);
}

void CMainFrame::OnNcPaint() 
{
	if (Axis::isVista && GetTheme() == 1)
		VistaFrame = GetSystemMetrics(SM_CXFRAME)-4;

	DrawFrame();
}

void CMainFrame::RedrawVS(bool bMinus /* = false */)
{
	const int	ncnt = m_arMDI[m_vsN].GetCount() + m_arSDI[m_vsN].GetCount();
	if (bMinus)
	{
		if (ncnt == 0)
			DrawFrame();
	}
	else
	{
		if (ncnt)
			DrawFrame();
	}
}


const int szFrame = 3;

void CMainFrame::DrawFrame() 
{
	CWindowDC dc(this);
	SetRegion();

	DrawFrame(&dc);
	
	CRect wrc, rc;

	GetWindowRect(&wrc);
	rc = wrc;
	rc.OffsetRect(-rc.TopLeft());	

	rc.top -= 5;
	
	if (!m_bSDI)
	{
		class SideDraw {
		public:
			SideDraw(CDC* dc, CRect& rc, CPoint offset, CDialogBar* bar, CBitmap* bmp, BOOL stretch = FALSE, int hoff = 0)
			{
				CRect wrc;

				bar->GetWindowRect(&wrc);
				wrc.OffsetRect(-offset);

				CDC mdc;
				mdc.CreateCompatibleDC(dc);
				CBitmap* oldBmp = mdc.SelectObject(bmp);

				BITMAP bm;
				bmp->GetBitmap(&bm);

				if (stretch)
				{
					dc->StretchBlt(rc.left, wrc.top, szFrame, wrc.Height(), &mdc, 0, 0, bm.bmWidth, 
						bm.bmHeight-hoff, SRCCOPY);

					dc->StretchBlt(rc.right - szFrame, wrc.top, szFrame, wrc.Height(), &mdc, 0, 0, bm.bmWidth, 
						bm.bmHeight-hoff, SRCCOPY);
				}
				else
				{
					dc->BitBlt(rc.left, wrc.top, szFrame, wrc.Height(), &mdc, 0, 0, SRCCOPY);
					dc->BitBlt(rc.right-szFrame, wrc.top, szFrame, wrc.Height(), &mdc, 0, 0, SRCCOPY);
				}
				mdc.SelectObject(oldBmp);
			}
		};

		if (m_tMenu)
		{
#ifdef DF_USE_CPLUS17
			SideDraw(&dc, rc, wrc.TopLeft(), m_tMenu.get(), Axis::GetBitmap("MENUBACK"));
#else
			SideDraw(&dc, rc, wrc.TopLeft(), m_tMenu, Axis::GetBitmap("MENUBACK"));		
#endif
		}

		if (m_bar1 && m_bar1->IsWindowVisible())
		{
#ifdef DF_USE_CPLUS17
			SideDraw(&dc, rc, wrc.TopLeft(), m_bar1.get(), Axis::GetBitmap("Band"));
#else
			SideDraw(&dc, rc, wrc.TopLeft(), m_bar1, Axis::GetBitmap("Band"));
#endif
		}

		if (m_tInfo3 && m_tInfo3->IsWindowVisible())
		{
#ifdef DF_USE_CPLUS17
			SideDraw(&dc, rc, wrc.TopLeft(), m_tInfo3.get(), Axis::GetBitmap("BAR_BG"), TRUE);
#else
			SideDraw(&dc, rc, wrc.TopLeft(), m_tInfo3, Axis::GetBitmap("BAR_BG"), TRUE);
#endif
		}

		if (m_bar2 && m_bar2->IsWindowVisible())
		{
#ifdef DF_USE_CPLUS17
			if (m_bar2->GetBarStyle() & CBRS_ALIGN_TOP)
				SideDraw(&dc, rc, wrc.TopLeft(), m_bar2.get(), Axis::GetSkinBitmap("BAR_BG"), TRUE);
			else
				SideDraw(&dc, rc, wrc.TopLeft(), m_bar2.get(), Axis::GetSkinBitmap("하단_BAR_BG"), TRUE);
#else
			if( m_bar2->GetBarStyle() & CBRS_ALIGN_TOP )
				SideDraw(&dc, rc, wrc.TopLeft(), m_bar2, Axis::GetSkinBitmap("BAR_BG"), TRUE);
			else
				SideDraw(&dc, rc, wrc.TopLeft(), m_bar2, Axis::GetSkinBitmap("하단_BAR_BG"), TRUE);
#endif
		}

		if (m_bar3 && m_bar3->IsWindowVisible())
		{
#ifdef DF_USE_CPLUS17
			if (m_bar3->GetBarStyle() & CBRS_ALIGN_TOP)
				SideDraw(&dc, rc, wrc.TopLeft(), m_bar3.get(), Axis::GetSkinBitmap("BAR_BG"), TRUE);
			else
				SideDraw(&dc, rc, wrc.TopLeft(), m_bar3.get(), Axis::GetSkinBitmap("하단_BAR_BG"), TRUE);
#else
			if( m_bar3->GetBarStyle() & CBRS_ALIGN_TOP )
				SideDraw(&dc, rc, wrc.TopLeft(), m_bar3, Axis::GetSkinBitmap("BAR_BG"), TRUE);
			else
				SideDraw(&dc, rc, wrc.TopLeft(), m_bar3, Axis::GetSkinBitmap("하단_BAR_BG"), TRUE);
#endif
		}

		if (m_tInfo2 && m_tInfo2->IsWindowVisible())
		{
#ifdef DF_USE_CPLUS17
			SideDraw(&dc, rc, wrc.TopLeft(), m_tInfo2.get(), m_tInfo2->GetBackImage(), TRUE);
#else
			SideDraw(&dc, rc, wrc.TopLeft(), m_tInfo2, m_tInfo2->GetBackImage(), TRUE);
#endif
		}
		if (m_tInfo1 && m_tInfo1->IsWindowVisible())
		{
#ifdef DF_USE_CPLUS17
			SideDraw(&dc, rc, wrc.TopLeft(), m_tInfo1.get(), m_tInfo1->GetBackImage(), TRUE);
#else
			SideDraw(&dc, rc, wrc.TopLeft(), m_tInfo1, m_tInfo1->GetBackImage(), TRUE);
#endif
		}
		
		drawTitle(&dc);
	}
	
	DrawBorder(&dc);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	const int	index = LOWORD(wParam);
	if (index < ID_MENU_BASE || index > ID_MENU_END)
	{
		if (runCommand(LOWORD(wParam), lParam))
			return TRUE;
		return CMDIFrameWnd::OnCommand(wParam, lParam);
	}
	if (index == IDC_BTN_LOGO)
	{
		CallRecommendedBoard();
	}

// 	CString s;
// 	s.Format("[USER SCREEN INDEX : %d]\n",index);
	// 	OutputDebugString(s);

//	save_laststat();

	m_mapHelper->ChangeChild(index);
	return TRUE;
}

void CMainFrame::CallRecommendedBoard()
{
	//m_mapHelper->CreateChild("IB0000X1", 0);
}

void CMainFrame::OnAppDummy(UINT uid) {}

bool CMainFrame::runCommand(int comm, LPARAM lParam)
{
	BOOL	visible = FALSE;
	CString	str;

	switch (comm)
	{
	case ID_CODEFOCUS: 
		{
			return true;
			CString sdat;
			sdat = _T("1301\tFOCUS");
			
			int	key{};
			const long	rc = 0;
			POSITION pos{};
			CChildFrame* child{};

			for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
			{
				m_arMDI[m_vsN].GetNextAssoc(pos, key, child);

// 				CString s;
// 				s.Format("ACTIVE TR [%d][%d]\n",key,child->IsActive());
// 				OutputDebugString(s);
				
				if(child->IsActive())
				{
					if(child->m_mapN.Find("IB0001") > -1)
						sdat = _T("jcod\tFOCUS");

					m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sdat);
					break;
				}
			}
// 			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 				(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		}
		break;
	case ID_SCREENNUM:	SetHome();	break;	// set home
	case ID_STAFF_PWD:
		m_mapHelper->ChangeChild("IB985000");
		break;
	case ID_SETACCOUNT:
		{
			if (ScreenCheck("GROUPACCSAVEKEY"))
			{
				//visible = !(m_TotalAcc->GetStyle() & WS_VISIBLE);
				//ShowControlBar(m_TotalAcc, visible, FALSE);
			}
		}
		break;
	case ID_CLEARCHART:	ClearGex();	break;		// 차트초기화기능 추가 2011.03.02 by LKM
	case ID_ESCAPEWINODW: 
		{
			m_mapHelper->closeChild();	
		}
		break;	// esc key
		
	case ID_SHOWTRACER:	showChaser();	break;	// alt + F1
	case ID_FULLNAME:	showfname();	break;	// alt + F2
	case ID_RELOADMAP:
		if (Axis::devMode)
		{
			CWnd *child = refleshMap(m_activeKey);
			if (child)
			{
				child->Invalidate();
				child->SetFocus();
			}
		}
		break;	// alt + F3
	case ID_FIRSTTAB:	// end key
		if (m_bEnd)
			AppSwitch();
		break;
	case ID_NATINGTEL:  // F4
		{
			if(Axis::devMode || !Axis::isCustomer)
				RunPhonePad();
		}
		break;
	case ID_TRANSUP:
		if(m_bMiniMode)
		{
			//OutputDebugString("DDDDDDDD");
			const CWnd* base = m_miniSWid->GetActiveView()->GetWindow(GW_CHILD);
			base->SendMessage(WD_TRANSMAX,(WPARAM)0,(LPARAM)0);
		}
		break;
	case ID_TRANSDOWN:
		if(m_bMiniMode)
		{
			//OutputDebugString("SSSSSSS");
			const CWnd* base = m_miniSWid->GetActiveView()->GetWindow(GW_CHILD);
			base->SendMessage(WD_TRANSMIN,(WPARAM)0,(LPARAM)0);
		}
		break;
	case ID_APP_TOTALSETUP: TotalSetup(); break;
	case ID_APP_RECOM:	CallRecommendedBoard();	break;
	case ID_APP_ESETUP:		TotalSetup(IDD_ESETUP); break;
	case ID_APP_MNGSETUP:	TotalSetup(IDD_MNGSETUP); break;
	case ID_APP_ORDERSETUP: TotalSetup(IDD_ORDER_SETUP); break;
	case ID_APP_THEMESETUP: TotalSetup(IDD_IMAGESETUP); break;
	case ID_APP_TICKERSETUP:TotalSetup(IDD_SETTICKINFO); break;
	case ID_APP_EDITHOTKEY:	TotalSetup(IDD_HOTKEY); break;
	case ID_APP_EDITTOOL:	TotalSetup(IDD_TSETUP); break;
	case ID_APP_TOOLBARSETUP: TotalSetup(IDD_FUNCTIONS); break;
	case ID_APP_CHEGEQUAL: TotalSetup(IDD_EQUALIZER); break;

	//** macho begin
	//**case ID_APP_ACCSETTING:	AccSetting();	break;
	case ID_APP_ACCTGROUP:
		AcctGroupConfig(); 
		break;
	case ID_APP_ACCTPASSWORD:
		AcctPasswordConfig();
		break;
	/**
	case ID_APP_ACCTALIAS:
		AcctAliasConfig();
		break;
	**/
	//** macho end
	case ID_APP_FOCONF: RunFOConfig(); break;	
	case ID_APP_HELPCOM:	RunHelpCom();	break;
	case ID_APP_CALC:
		ExecuteCalculator();
		break;
	case ID_APP_TRSEARCH:
		{
#ifdef DF_USE_CPLUS17
		CTrSearch	dlg(m_tMenu.get(), axiscall);
#else
			CTrSearch	dlg(m_tMenu, axiscall);
#endif
			dlg.setPos(m_bar1->GetPos(1));
			if (dlg.DoModal() == IDOK)
				m_mapHelper->ChangeChild(dlg.getTR());
		}
		break;
	case ID_APP_USERSCREEN_SAVE:	
		saveUserScreen();	
		break;
	case ID_APP_USERPROGRAM:
		{
			CProgramDlg	dlg;
			if (dlg.DoModal() == IDOK)
				appendUserToolMenu();
		}
		break;
	case ID_APP_LINKEXCEL:
		m_mapHelper->ChangeChild(MAPN_LINKEXCEL);
		break;
	case ID_APP_CATCH:
		m_mapHelper->ChangeChild(MAPN_SISECATCH);
		break;
	case ID_VIEW_KOBAELW_SCREEN:
		m_mapHelper->ChangeChild((LPCSTR)lParam);
		break;
	case ID_APP_USETUPUPLOAD:	ProcessInfofile();	break;
	case ID_APP_USETUPDOWN:		ProcessInfofile(false);	break;
	case ID_APP_SAVEBMP_A:		
		saveImg(1);		
		break;
	case ID_APP_SAVEBMP:		
		saveImg();		
		break;
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
	case ID_APP_STOCK_STOPLOSS:
		StopLoss();
		break;
	case ID_APP_FO_STOPLOSS:
		FOStopLoss();
		break;
	case ID_APP_LOCK:	LockProg();	break;
	case ID_APP_RECONNECT:
		{
			{
				const HWND	hWnd = m_hWnd;
				CString	msg, title;
				msg.LoadString(ST_MSG_RECONNECT_OTHERUSER);
				title.LoadString(ST_TEXT_RECONNECT_OTHERUSER);

				if (Axis::MessageBox(this, msg, MB_ICONQUESTION | MB_YESNO) == IDYES)
					ChangeUser();
			}
		}
		break;
	case ID_APP_CONNECT:	// 2007.07.05 동시접속
		m_mapHelper->ChangeChild(MAPN_MULTICONNECT);
		break;
	case ID_VIEW_DB1:
		visible = !(m_bar1->GetStyle() & WS_VISIBLE);
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_bar1.get(), visible, FALSE);
#else
		ShowControlBar(m_bar1, visible, FALSE);
#endif			
		saveToolStatus();

		ChangeLogo();	
		break;
	case ID_VIEW_SCLIST:
		visible = !(m_bar2->GetStyle() & WS_VISIBLE);
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_bar2.get(), visible, FALSE);
		if (visible && (m_bar0->GetStyle() & WS_VISIBLE))
			ShowControlBar(m_bar0.get(), TRUE, FALSE);
#else
		ShowControlBar(m_bar2, visible, FALSE);
		if (visible && (m_bar0->GetStyle() & WS_VISIBLE))
			ShowControlBar(m_bar0, TRUE, FALSE);
#endif
		
		saveToolStatus();
		SetSDIChangeHeight();
		
		break;
	case ID_VIEW_TICKER1:
		visible = !(m_tInfo1->GetStyle() & WS_VISIBLE);
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_tInfo1.get(), visible, FALSE);
#else
		ShowControlBar(m_tInfo1, visible, FALSE);
#endif
		saveToolStatus();
		SetSDIChangeHeight();
		break;
	case ID_VIEW_TICKER2:
		visible = !(m_tInfo2->GetStyle() & WS_VISIBLE);
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_tInfo2.get(), visible, FALSE);
#else
		ShowControlBar(m_tInfo2, visible, FALSE);
#endif
		saveToolStatus();
		SetSDIChangeHeight();
		break;
	case ID_VIEW_TICKER3:
		if (!m_tInfo3)
			break;
		visible = !(m_tInfo3->GetStyle() & WS_VISIBLE);
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_tInfo3.get(), visible, FALSE);
#else
		ShowControlBar(m_tInfo3, visible, FALSE);
#endif
		saveToolStatus();
		SetSDIChangeHeight();
		break;
	case ID_VIEW_HISTORY:
		{
			visible = !(m_bar3->GetStyle() & WS_VISIBLE);
#ifdef DF_USE_CPLUS17
			ShowControlBar(m_bar3.get(), visible, FALSE);
#else
			ShowControlBar(m_bar3, visible, FALSE);
#endif
			saveToolStatus();
			SetSDIChangeHeight();
		}
		break;
	case ID_VIEW_JCONTROL:
		visible = !(m_bar0->GetStyle() & WS_VISIBLE);
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_bar0.get(), visible, FALSE);
#else
		ShowControlBar(m_bar0, visible, FALSE);
#endif

		break;
	case ID_VIEW_ACCTOOL:
		visible = !(m_accTool->GetStyle() & WS_VISIBLE);
		m_accTool->ShowWindow(visible ? SW_SHOW : SW_HIDE);
		break;
	case ID_VIEW_MDI:
		if (m_appMode != MODE_MDI)
			ChangeMDI();
		break;
	case ID_VIEW_SDI:
		// 20070117
		if (m_appMode != MODE_SDI)
		{
			if (m_appMode == MODE_TRAY)
			{
				TrayCmd(TRAY_HIDEICON);
				ShowWindow(SW_SHOW);
			}
			else
				ChangeSDI();
		}
		
		//if (m_appMode != MODE_SDI)
		//	ChangeSDI();
		break;
	case ID_VIEW_TRAY:
		if (m_appMode != MODE_TRAY)
			ChangeTRAY();
		break;
	case ID_VIEW_VSCREEN1:	change_VirtualScreen(V_SCREEN1);	break;
	case ID_VIEW_VSCREEN2:	change_VirtualScreen(V_SCREEN2);	break;
	case ID_VIEW_VSCREEN3:	change_VirtualScreen(V_SCREEN3);	break;
	case ID_VIEW_VSCREEN4:	change_VirtualScreen(V_SCREEN4);	break;
	case ID_VIEW_VSCREEN5:	change_VirtualScreen(V_SCREEN5);	break;
	case ID_VIEW_VSCREEN6:	change_VirtualScreen(V_SCREEN6);	break;
	case ID_VIEW_FULLSCREEN:fullScreenMode(!m_fullmode);		break;
	case ID_VIEW_CASCADE:	MDICascade();				break;
	case ID_VIEW_TILE_HORZ:	MDITile(MDITILE_HORIZONTAL);		break;
	case ID_VIEW_TILE_VERT:	MDITile(MDITILE_VERTICAL);		break;
	case ID_VIEW_CLOCK:
		{
			if (m_Nclock)
			{
				if (m_Nclock->GetStyle() & WS_VISIBLE)
					m_Nclock->ShowWindow(SW_HIDE);
				else
				{
					CRect	rect, rcClock;
					CPoint	pt;
					GetWindowRect(rect);
					pt = rect.CenterPoint();
					m_Nclock->GetWindowRect(rcClock);
					pt.x -= rcClock.Width() / 2;
					pt.y = rect.top + rcClock.Height();					
					m_Nclock->SetWindowPos(&wndTopMost, pt.x, pt.y, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOACTIVATE);		
					m_Nclock->SetMini(FALSE);
					m_Nclock->ShowWindow(SW_SHOW);
					SetFocus();
				}
			}
		}
		break;

	case ID_APP_CLOSEA:	
		m_mapHelper->closeChild();	
		break;
	case ID_APP_CLOSEALLWITHOUTTHIS: 
		m_mapHelper->closeAllWithoutThis(); 
		break;
	case ID_APP_CLOSEALL:	
		m_mapHelper->closeAll();	
		break;
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
	//case ID_APP_USERSCREEN_SAVE:
	case ID_APP_STOCK_STOPLOSS:
	case ID_APP_FO_STOPLOSS:
		enable = !m_bOnlySise;
		break;
	case ID_APP_SAVEBMP_A:
	case ID_APP_SAVEBMP:
	case ID_APP_PRINT:
		pCmdUI->Enable(MDIGetActive() != NULL);
		return;
	case ID_VIEW_DB1:
		if (m_bSDI)	enable = false;
		pCmdUI->SetCheck(m_bar1->GetStyle() & WS_VISIBLE);
		break;
	case ID_VIEW_SCLIST:
		if (m_bSDI)	enable = false;
		pCmdUI->SetCheck(m_bar2->GetStyle() & WS_VISIBLE);
		break;
	case ID_VIEW_TICKER1:
		if (m_bSDI)	enable = false;
		pCmdUI->SetCheck(m_tInfo1->GetStyle() & WS_VISIBLE);
		break;
	case ID_VIEW_TICKER2:
		if (m_bSDI)	enable = false;
		pCmdUI->SetCheck(m_tInfo2->GetStyle() & WS_VISIBLE);
		break;
	case ID_VIEW_TICKER3:
		if (m_bSDI)	enable = false;
		if (!m_tInfo3) 
			break;
		pCmdUI->SetCheck(m_tInfo3->GetStyle() & WS_VISIBLE);
		break;
	case ID_VIEW_JCONTROL:
		pCmdUI->SetCheck(m_bar0->GetStyle() & WS_VISIBLE);
		break;
	case ID_SETACCOUNT:
		pCmdUI->SetCheck(m_TotalAcc->GetStyle() & WS_VISIBLE);
		break;
	case ID_VIEW_ACCTOOL:
		/**
		pCmdUI->Enable(FALSE);
		return;
		**/
		if (!m_accTool)
		{
			pCmdUI->Enable(FALSE);
			return;
		}
		pCmdUI->SetCheck(m_accTool->GetStyle() & WS_VISIBLE);
		break;
	case ID_VIEW_HISTORY:
		{
			if(m_bSDI) enable = false;
			pCmdUI->SetCheck(m_bar3->GetStyle() & WS_VISIBLE);
		}
		break;
	case ID_VIEW_MDI:
		pCmdUI->SetCheck(m_appMode == MODE_MDI);
		break;
	case ID_VIEW_SDI:
		pCmdUI->SetCheck(m_appMode == MODE_SDI);
		break;
	case ID_VIEW_TRAY:
		pCmdUI->SetCheck(m_appMode == MODE_TRAY);
		break;
	case ID_VIEW_VSCREEN1:
		pCmdUI->SetCheck(m_vsN == V_SCREEN1);
		break;
	case ID_VIEW_VSCREEN2:
		pCmdUI->SetCheck(m_vsN == V_SCREEN2);
		break;
	case ID_VIEW_VSCREEN3:
		pCmdUI->SetCheck(m_vsN == V_SCREEN3);
		break;
	case ID_VIEW_VSCREEN4:
		pCmdUI->SetCheck(m_vsN == V_SCREEN4);
		break;
	case ID_VIEW_VSCREEN5:
		pCmdUI->SetCheck(m_vsN == V_SCREEN5);
		break;
	case ID_VIEW_VSCREEN6:
		pCmdUI->SetCheck(m_vsN == V_SCREEN6);
		break;
	case ID_APP_CLOSEA:
	case ID_APP_CLOSEALLWITHOUTTHIS:
	case ID_APP_CLOSEALL:
		if (m_bSDI)
			if (!m_arSDI[m_vsN].GetCount())
				enable = false;
		else
			if (!m_arMDI[m_vsN].GetCount())
				enable = false;
		break;
	case ID_VIEW_FULLSCREEN:
		if (m_bSDI)	enable = false;
		pCmdUI->SetCheck(m_fullmode);
		break;
	case ID_VIEW_CASCADE:
	case ID_VIEW_TILE_HORZ:
	case ID_VIEW_TILE_VERT:
		if (!MDIGetActive())	
			enable = false;
		break;
	case ID_VIEW_CLOCK:
		if (m_Nclock && m_Nclock->GetSafeHwnd())
		{
			pCmdUI->SetCheck(m_Nclock->GetStyle() & WS_VISIBLE);
		}
		break;
	default:	
		break;
	}

	pCmdUI->Enable(enable);
}

void CMainFrame::OnClose() 
 {
	if (!m_bCLOSE_ASTx)
	{
		MemoUpload();  //memo
		//DumpUpload();
	}

	SetEvent(m_hStopEvent);
	CloseHandle(m_hStopEvent);

	CString file;
	m_iAxisState = AXIS_STATE_ONCLOSE;
	m_slog.Format("--------------------[Axis_State][onClose start]  m_iAxisState =[%d]-------------", m_iAxisState);
	OutputDebugString(m_slog);
	const char* noticeMapName = "IB780100";
	CProfile pout(pkUserConfig);
	const CWnd* wnd = FindWindow(NULL,"실시간해외지수");
	if (wnd)
	{
		pout.Write(noticeMapName, "OnLoad", 1);
	}
	else
	{
		pout.Write(noticeMapName, "OnLoad", 0);
	}
	
// 	if (m_bExit && m_runAxis)
// 		saveExitMap();

	if (!m_mapHelper->closeX())
	{
		return;
	}

//#ifdef NDEBUG
	if (m_bExit && m_runAxis)	//	exit window
	{
		if (((CAxisApp *) m_axis)->m_progK != 'R')
		{
			OutputDebugString("ONCLOSE EXIT\n");
// 			if(m_mapHelper->CloseInterest())
// 			{
// 				CString s;
// 				s.Format("INTEREST CLOSE COUNT [%d]\n",m_nInterest);
// 				OutputDebugString(s);
// 	// 			if (IDOK != MessageBox("저장되지 않은 관심종목이 있읍니다. \n정말 종료하시겠습니까?", "종료확인", MB_OKCANCEL | MB_ICONQUESTION))
// 	// 				return;
// 
// 				m_bExit = false;
// 				
// 				m_mapHelper->SendInterSignal();
// 				
// 				return;
// 			}
			saveExitMap();
			SendScrap();
//			SendConfig(0);   //khs 우선보류
//			SendConfig(1);

			if(m_top10 != nullptr && m_top10->GetSafeHwnd())
				m_top10->ShowWindow(SW_HIDE);

			CExitD exit(NULL, IsNewExitImage());
			exit.SetTime(m_connectT);
			const UINT modalResult = exit.DoModal();
			if (modalResult == IDCANCEL)
				return;
			
			//UploadFile();
			//SendConfig();
		}
	}

	if(m_bExit != false)
	{
		m_nInterest = 0;

		//AfxMessageBox("INTEREST PROCESSING....\n");

		if(m_mapHelper->CloseInterest())
		{
			CString s;
			s.Format("INTEREST CLOSE COUNT [%d]\n",m_nInterest);
			OutputDebugString(s);
// 			if (IDOK != MessageBox("저장되지 않은 관심종목이 있읍니다. \n정말 종료하시겠습니까?", "종료확인", MB_OKCANCEL | MB_ICONQUESTION))
// 				return;

			m_bExit = false;
			
			m_mapHelper->SendInterSignal();
			
			return;
		}
	}

	TRY
	{
		file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "AccList.dat");
		::DeleteFile(file);
		file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "GrpList.dat");
		::DeleteFile(file);
	}
	CATCH (CFileException, e) 
	{
	}
	
	END_CATCH;

	if (m_MClient->m_pBkMapWnd)
	{
		((CBkWnd*)m_MClient->m_pBkMapWnd)->SendMessage(WM_CLOSE, 0, 0);
		delete m_MClient->m_pBkMapWnd;
		m_MClient->m_pBkMapWnd = NULL;
	}
	FreeFirewall();
	m_bLastClose = true;	// 20070627 kwon
	KillTimer(TM_SCRLOG);	// 20070710 cej
	//OutputDebugString("FREEFIREWALL\n");
	//FreeFirewall();
	CloseAgent();
	DeleteChaser();
	//saveExitMap();
	saveToolStatus();
	save_history();

	// 2010.09.03 열었던 화면목록 Report by warship
	save_scr_counter();
	scr_counter_report();

	removeOpenedList();	// 20070627 kwon
	
	int		key{};
	CSChild* schild{};
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		for (POSITION pos = m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			schild->SendMessage(WM_CLOSE);
		}

		// dtitle m_arSDI[vsN].RemoveAll();
		RemoveAllChild(vsN, true);
	}
	
	m_mapHelper->closeAll();
	for (int ii = 0; ii < m_arHide.GetSize(); ii++)
	{
		const CChildFrame* child = m_arHide.GetAt(ii);
		if (child && child->GetSafeHwnd())
			child->SendMessage(WM_CLOSE);
	}

	//웹링크화면 브라우져 종료되게 수정 - dkkim 2015.01.02
	for (int jj = 0; jj < m_arWEB.GetSize(); jj++)
	{
 		HWND hwnd = m_arWEB.GetAt(jj);
		
		::PostMessage(hwnd, WM_CLOSE, 0, 0);
 
//  	DWORD processId;
// 
// 		DWORD threadId = GetWindowThreadProcessId(hwnd,&processId);
// 		
// 		CString s;
// 		s.Format("IE ID [%d]\n",processId);
// 		OutputDebugString(s);
// 
// 		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,processId);
// 
// 		if(hProcess)
// 		{
// 			TerminateProcess(hProcess,-1);
// 			CloseHandle(hProcess);
// 		}
	}

	m_arWEB.RemoveAll();

	//	code register screen
	BOOL	rc{};
	if (m_category)
	{
		m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
				(void *)NULL, (BYTE *)(VTS_I4), (short) KEY_CODE);
	}
	//dkkim 2018.11.12 기능제외
//	CString strFile;
// 	strFile.Format("%s\\tab\\RSCMONITOR.ini", Axis::home); 
// 	
// 	int nUse = GetPrivateProfileInt("RSCMONITOR","CPU",0,strFile);
// 	
// 	if(nUse > 0)
// 	{
// 		SendCPUInfo();
// 	}

	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
	
	if (m_MClient)	m_MClient->UnsubclassWindow();

	if (/*m_bExit && */m_runAxis/* && !m_bSDI*/)
	{
		CRect	wRc;
		GetWindowRect(wRc);
		
		const int screenX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		const int screenY = GetSystemMetrics(SM_CYVIRTUALSCREEN);

		int	x  = wRc.left;
		int	y  = wRc.top;
		const int	cx = wRc.Width();
		const int	cy = wRc.Height();

		const char* information = INFORMATION;
		
		if(!m_bMiniMode)
			m_axis->WriteProfileInt(information, "SDI", m_bSDI ? 1 : 0);

		if(screenX < x || screenY < y)
		{
			x = 0;
			y = 0;
		}

		m_axis->WriteProfileInt(information, "win_x", x);
		m_axis->WriteProfileInt(information, "win_y", y);
		m_axis->WriteProfileInt(information, "win_cx", cx);
		m_axis->WriteProfileInt(information, "win_cy", cy);
		m_axis->WriteProfileInt(information, "toolstatus", m_toolStatus);

		if (m_Nclock && m_Nclock->GetSafeHwnd())
		{
			CRect	wRc;
			m_Nclock->GetWindowRect(wRc);

			m_axis->WriteProfileInt(information, "clock_visible", m_Nclock->GetStyle() & WS_VISIBLE ? 1 : 0);
			m_axis->WriteProfileInt(information, "clock_mini", m_Nclock->IsMini() ? 1 : 0);
			m_axis->WriteProfileInt(information, "clock_x", wRc.left);
			m_axis->WriteProfileInt(information, "clock_y", wRc.top);

			//m_Nclock->SetWindowPos(&wndTopMost, x, y, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE);
		}
	}

	KillTimer(TM_PRNIMG);

	//ID 암호화 레지스트리 저장
// 	CString encID = EncryptAES(Axis::userID);
// 
// 	if (AfxGetApp())
// 		AfxGetApp()->WriteProfileString(WORKSTATION, "BackUserID", "!&$"+encID);
// 
// 	CString signID = m_axis->GetProfileString(WORKSTATION, SIGNONID);
// 	int pos = signID.Find('|');
// 	if (pos >= 0)
// 	{
// 		CString name = signID.Mid(pos + 1);
// 
// 		CString encSignOn;
// 		encSignOn.Format("!&$%s|%s",encID,name);
// 
// 		if (AfxGetApp())
// 			AfxGetApp()->WriteProfileString(WORKSTATION, SIGNONID, encSignOn);
// 	}

	// WINE 종료
// 	if(m_winechartHwnd)
// 		::SendMessage(m_winechartHwnd,  WM_IBK_CLOSE, 0, 0);
// 	
// 	if(m_wineSchartHwnd)
// 		::SendMessage(m_wineSchartHwnd,  WM_IBK_CLOSEWINES, 0, 0);

	if (m_bReconnect)
	{
		m_axMisc->RunVers(verUSERID);
		m_bReconnect = false;
	}
	
	m_bClose = TRUE;

	OSVERSIONINFOEX osvi;   
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOA);
	//GetVersionExA((LPOSVERSIONINFO)&osvi);
	GetVersion(&osvi);
	
	if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
	{
		// os 윈도우 8 적용
	}
	else
	{
		// os 윈도우 7 이하 적용 
		if (m_hMNews) 
		{
			FreeLibrary(m_hMNews);
			m_hMNews = NULL;
		}

		if (m_hSHA256)
		{
			FreeLibrary(m_hSHA256);
			m_hSHA256 = NULL;
		}
	}

	//필요없는 파일 삭제 2018.10.18
	if(!Axis::devMode)
	{
		char*	control[] = { "AXISCHASER.EXE","AXISBUILDER.EXE", "IBKSCONNECTORPTS.OCX", NULL };
		CString	path;
		
		for (int ii = 0; control[ii] != NULL; ii++)
		{
			path.Format("%s\\%s\\%s", Axis::home, RUNDIR, control[ii]);

			CFileFind finder;
			
			if(finder.FindFile(path))
			{
				::DeleteFile(path);
			}
		}
	}
	
	if (m_hSharedLib)
	{
		FreeLibrary(m_hSharedLib);
		m_hSharedLib = nullptr;
	}
	
	m_slog.Format("--------------------[Axis_State][onClose end]  m_iAxisState =[%d]-------------", m_iAxisState);
	OutputDebugString(m_slog);
	CMDIFrameWnd::OnClose();
}

void CMainFrame::UserFileCrypt(bool bFlag)
{
	char	wb[1024 * 4]{};
	CString filename;
	CStdioFile cfile;
	CString strLine;
	CString strTotal;
	CString s;

	filename.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, Axis::user, Axis::user);

	if(bFlag)
	{
		GetPrivateProfileSection("ACCOUNTHISTORY",wb,sizeof(wb),filename);

		CString strData;

		strData.Format("%s",wb);

		strData.TrimRight();
// 		s.Format("ENC SECTION [%s]\n",strData);
// 		OutputDebugString(s);

		CString encData = EncryptAES(strData);

		WritePrivateProfileSection("ACCOUNTHISTORY","",filename);

		WritePrivateProfileString("ACCOUNTHISTORY","ENC",encData,filename);
	}
	else
	{
		GetPrivateProfileString("ACCOUNTHISTORY","ENC","",wb,sizeof(wb),filename);
		
		CString strData(wb,sizeof(wb));
		
		strData.TrimRight();
		s.Format("DEC SECTION [%s]\n",strData);
		OutputDebugString(s);
		
		CString decData = DecryptAES(strData);
		
		WritePrivateProfileSection("ACCOUNTHISTORY","",filename);
		
		WritePrivateProfileSection("ACCOUNTHISTORY",decData,filename);
	}
}

void CMainFrame::CloseWebHandle()
{
	OutputDebugString("$MAIN CLOSEWEBHANDLE\n");
	//웹링크화면 브라우져 종료되게 수정 - dkkim 2015.01.02
	for (int jj = 0; jj < m_arWEB.GetSize(); jj++)
	{
		HWND hwnd = m_arWEB.GetAt(jj);
		
 		::PostMessage(hwnd, WM_CLOSE, 0, 0);
		
		//  	DWORD processId;
		// 
		// 		DWORD threadId = GetWindowThreadProcessId(hwnd,&processId);
		// 		
		// 		CString s;
		// 		s.Format("IE ID [%d]\n",processId);
		// 		OutputDebugString(s);
		// 
		// 		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,processId);
		// 
		// 		if(hProcess)
		// 		{
		// 			TerminateProcess(hProcess,-1);
		// 			CloseHandle(hProcess);
		// 		}
	}
}

LONG CMainFrame::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	BOOL		rc{};
	char		wb[128]{};
	int		value{}, key{};
	CChildFrame* child{};
	CStringArray	arr;
	CString		tmps = _T(""), title = _T("");
	CString axisfile;
	CString slog;
	char buff[64]{};

	switch (LOWORD(wParam))
	{
	case axUPLODING_LOG:
	{
		if (m_pUpload)
		{
			if (m_pUpload.get()->m_arrFile.GetSize() == 0)
				m_bUploadComplet = true;
			else
				m_bUploadComplet = false;
		}

slog.Format("[AXIS] uaxUPLODING_LOG m_bUploadComplet = [%d]", m_bUploadComplet);
WriteLog(slog);
		
		return m_bUploadComplet;
	}
		break;
	case axSTART:	// start step
		{
			if(m_bUseNewLogin)
			{
				m_bCertLogin = m_axConnect->GetCertLogin();
			}

			if (HIWORD(wParam) && m_step != axNONE)
			{
				m_step = axNONE;
				m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
						(BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
			}

			switch (m_step)
			{
			case axNONE:	m_connectBy = m_axis->GetProfileInt(INFORMATION, CONNECTBY, byNONE);break;
			case axOPENPPP:	break;
			case axOPENSIGN:
				SetEncriptFile();
				if(m_bUseNewLogin)
					m_axConnect->SetChoice(false);
				else
					m_axConnectOld->SetChoice(false);
				PostMessage(WM_AXIS, MAKEWPARAM(axSIGNON, 0));		return 0;
			default:return 0;
			}

			if (!getConnectInfo(tmps, value)) 
			{
				if (m_bUseNewLogin)
					m_axConnect->SetGuide("");
				else
					m_axConnectOld->SetGuide("");

				Axis::MessageBox(this, "설치정보를 확인하세요(GLB)", MB_ICONINFORMATION);
				if (m_bUseNewLogin)
					m_axConnect->SetLoginBtnEnable(true);
				/*m_slog.Format("접속 실패하였습니다 [%d]", GetLastError());
				if (m_bUseNewLogin)
					m_axConnect->SetGuide(m_slog);
				else
					m_axConnectOld->SetGuide(m_slog);*/
				return 0;
			}
			strcpy_s(wb, 128, tmps);
			m_step = axOPENRSM;

			m_ip = tmps;
			m_port.Format("%d", value);
			
			((CAxisApp*)m_axis)->m_conIP = m_ip;

			CString file, axisfile, usnm = Axis::user;
			file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 
			axisfile.Format("%s\\%s\\Axis.ini", Axis::home, TABDIR);

			if ((m_ip == "172.16.202.106") || (m_ip == "172.16.205.20") || (m_ip == "211.255.204.134")|| (m_ip == "211.255.204.19") ||  ( m_ip == "211.255.204.104") ||
				m_ip=="172.16.205.30" || m_ip=="211.255.204.136" || m_ip=="211.255.204.85" || m_ip=="172.16.202.150" || m_ip=="172.16.202.171" || m_ip == "172.16.202.130")
			{
				Axis::devMode = TRUE;

				WritePrivateProfileString("MODE", "DEV", "1", file);
				WritePrivateProfileString("MODE", "DEV", "1", axisfile);
			}
			else
			{
				WritePrivateProfileString("MODE", "DEV", "0", file);
				WritePrivateProfileString("MODE", "DEV", "0", axisfile);
			}

			WritePrivateProfileString("MODE", "PORT",m_port, file);

			tmps.Format("%d", m_bOnlySise);
			WritePrivateProfileString("MODE", "OnlySise", tmps, file);
			
			WritePrivateProfileString("MODE", "reg", m_regkey, file);
			
			tmps.Format("%d", (int)this->m_hWnd);
			
			WritePrivateProfileString("MODE", "main", tmps, file);
			
			tmps.Empty();

		
			// 연결 버튼 클릭시에. 리소스 다운여부 결정.
			if (AfxGetApp())
			{
				if (AfxGetApp()->GetProfileInt(WORKSTATION, "GetAllResource", 0))
				{
					CString	path, filename;
					path.Format("%s\\%s\\", Axis::home, TABDIR);
					filename.Format("%s\\%s", path, "infoAXIS");
					::DeleteFile(filename);
					filename.Format("%s\\%s", path, "infoRSC");
					::DeleteFile(filename);
					AfxGetApp()->WriteProfileInt(WORKSTATION, "GetAllResource", 0);
				}
			}

			m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
						(BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, (long)wb, value);

			if (rc)
			{
				m_axMisc->GetGuide(AE_CONNECT, tmps);
				if(m_bUseNewLogin)
					m_axConnect->SetGuide(tmps);
				else
					m_axConnectOld->SetGuide(tmps);
			}
			else
			{
				m_step = axNONE;
				if(m_bUseNewLogin)
				{
					m_axConnect->SetProgress(false);
					m_axConnect->SetStatus(SM_EXIT);
				}
				else
				{
					m_axConnectOld->SetProgress(false);
					m_axConnectOld->SetStatus(SM_EXIT);
				}
			}
		}
		break;
	case axRUN:	break;
	case axAXIS:
		{
		   //SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

			CheckEdgeInstalled();
			if (Axis::isCustomer)
				SendPIBOpopu("S", 231);
			//ref 보안프로그래(ASTx) 가동 확인
			int iRet = Self_VerifyIntegrity();
			if (!IsASTxRunning(TRUE)|| iRet > 0)
			{
				m_bExit = false;
				m_forceClose = true;
				m_bCLOSE_ASTx = TRUE;

				m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&rc,
					(BYTE*)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);

				PostMessage(WM_AXISCLOSE, 99, 0);
				return 0;
			}

			char	buf[512];
			CString	file, stmp, smap;
			file.Format("%s\\tab\\axis.ini", Axis::home);
			DWORD dw = GetPrivateProfileString("ASTx", "interval", "30000", buf, sizeof(buf), file);
			stmp.Format("%s", buf); stmp.TrimRight();
m_slog.Format("[ASTx] interval = [%s] ", stmp);
WriteLog(m_slog);
			SetTimer(TM_AOS_ALIVE, atoi(stmp), NULL);

#ifdef DF_MK_CAPTION
		 ////장운영 타이머
		 //// 현재 시간 가져오기
		     file.Format("%s\\%s\\AXISFILE.INI", Axis::home, TABDIR);
			GetPrivateProfileString("Market", "CSAT", "", buff, sizeof(buff), file);
			stmp.Format("%s", buff);
			stmp.TrimRight();

			CTime dnow = CTime::GetCurrentTime();
			CString today;
			today.Format("%04d%02d%02d", dnow.GetYear(), dnow.GetMonth(), dnow.GetDay());

			if (today == stmp)
				m_bCSAT = TRUE;
			else
				m_bCSAT = FALSE;



			memset(buff, 0x00, 64);
			GetPrivateProfileString("Market", "EarlyYear", "", buff, sizeof(buff), file);
			stmp.Format("%s", buff);
			stmp.TrimRight();

			if (m_bCSAT == FALSE)
			{
				if (today == stmp)
					m_bCSAT = TRUE;
			}

			CheckMarketStat();

			CTime now = CTime::GetCurrentTime();
			int curMin = now.GetMinute();
			int curSec = now.GetSecond();

			// 다음 10분 단위 시각까지 남은 초 계산
			int nextTenMin = ((curMin / 10) + 1) * 10; // 다음 10분 단위 분값 (ex: 8:04 -> 10, 8:30->40)
			if (nextTenMin >= 60)
				nextTenMin = 0; // 자정 넘어갈 때 처리

			// 현재 시각을 분+초로 환산
			int curTotalSec = curMin * 60 + curSec;
			int nextTotalSec = nextTenMin * 60; // 다음 10분 단위의 초 환산

			// 남은 초 계산 (다음 10분 단위까지)
			int diffSec = 0;
			if (nextTotalSec > curTotalSec)
				diffSec = nextTotalSec - curTotalSec;
			else
				diffSec = (60 * 60) - curTotalSec + nextTotalSec; // 1시간 넘어갈 때 처리

			UINT initialInterval = diffSec * 1000; // 밀리초 단위
#endif
			//ref

			
			file.Format("%s\\%s\\%s\\Crashlog", Axis::home, USRDIR, Axis::user);

			CFileFind ff;
			if (!ff.FindFile(file))
				::CreateDirectory(file, NULL);

		    const bool onlysise = m_axis->GetProfileInt(WORKSTATION, "OnlySise", 0) ? TRUE : FALSE;
			if(m_bCertLogin && onlysise)
				m_axis->WriteProfileInt(WORKSTATION, "OnlySise", 0);

			m_bInit = TRUE;
WriteLog("[AXIS] OnAxis-axAXIS - Step 1\n");
			const int screenX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
			const int screenY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
			int x  = m_axis->GetProfileInt(INFORMATION, "win_x", 0);
			int y  = m_axis->GetProfileInt(INFORMATION, "win_y", 0);
			const int cx = m_axis->GetProfileInt(INFORMATION, "win_cx", 0);
			const int cy = m_axis->GetProfileInt(INFORMATION, "win_cy", 0);


			const int x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);  //서브 모니터가 음수이면 좌우가 뒤바낀것이다
			const int y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);  //서브 모니터가 음수이면 위아래가 뒤바낀것이다
			
			const int iFull_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
			const int iFull_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	
slog.Format("[MIAN_MONITOR] x=[%d] y=[%d] x1=[%d] y1=[%d] iFull_width=[%d] iFull_height=[%d]\n", x, y, x1, y1, iFull_width, iFull_height);
//OutputDebugString(slog);
			
			const int iuse = m_axis->GetProfileInt(INFORMATION, "NoPosition", 1);  //자동위치지정 기능을 안쓰고 싶을때는 0
			if(iuse)
			{
				if(x < 0  && y < 0 )
				{
slog.Format("[MIAN_MONITOR] x[%d] < 0 or  y[%d] < 0", x, y);
//OutputDebugString(slog);
					x = 0;
					y = 0;
				}
				else if(iFull_width < x || iFull_height < y)
				{
slog.Format("[MIAN_MONITOR] iFull_width[%d] < x[%d]  or  iFull_height[%d] < y[%d]", iFull_width, x, iFull_height, y);
//OutputDebugString(slog);
					x = 0;
					y = 0;
				}
			}
			
			if(!m_bMiniMode)
			{
				if (m_bSDI)
				{
					if(Axis::isVista)
					{
						m_SDIHEIGHT += 3;
					}

 					SetWindowPos(&wndTop, x, y, cx, m_SDIHEIGHT, SWP_SHOWWINDOW);
				}
				else
				{
					if (cx <= 300 || cy <= 100)
					{
						SetWindowPlacement(&m_switchpl);
						m_axis->m_nCmdShow = SW_SHOWMAXIMIZED;
						ShowWindow(m_axis->m_nCmdShow);
					}
					else	
					{
						x -= GetSystemMetrics(SM_CXFRAME);
						x -= GetSystemMetrics(SM_CXBORDER);
						y -= GetSystemMetrics(SM_CYFRAME);
						y -= GetSystemMetrics(SM_CYBORDER);
						SetWindowPos(&wndTop, x, y, cx, cy, SWP_SHOWWINDOW);
					}
				}
			}

			UpdateWindow();
WriteLog("[AXIS] OnAxis-axAXIS - Step 2\n");

			if(m_bUseNewLogin)
			{
				if (m_axConnect->GetUserID().CompareNoCase("guest"))
				{
					if (AfxGetApp())
					{
						AfxGetApp()->WriteProfileString(WORKSTATION, "BackUserID", m_axConnect->GetUserID());
					}
				}

				if (m_axConnect)
				{
#ifndef DF_USE_CPLUS17
					delete m_axConnect;
#endif
					m_axConnect = NULL;
				}
			}
			else
			{
				if (m_axConnectOld->GetUserID().CompareNoCase("guest"))
				{
					if (AfxGetApp())
					{
						AfxGetApp()->WriteProfileString(WORKSTATION, "BackUserID", m_axConnectOld->GetUserID());
					}
				}
				
				if (m_axConnectOld)
				{
#ifndef DF_USE_CPLUS17
					delete m_axConnectOld;
#endif
					m_axConnectOld = NULL;
				}
			}

WriteLog("[AXIS] OnAxis-axAXIS - Step 3");

			ClearRepositoryLog();

			endWorkstation();

WriteLog("[AXIS] OnAxis-axAXIS - Step 4");
			
			if (m_bdnInterest && !Axis::isCustomer)      //test 핵심
				dnloadAction();
			//초기 관심종목 다운로드 안되게 수정시 아래 주석제거
			//SetTimer(TM_DNINTEREST, 1000, NULL);
	
WriteLog("[AXIS] OnAxis-axAXIS - Step 5");
			
			OpenAnnouncement();

WriteLog("[AXIS] OnAxis-axAXIS - Step 6");
			SetHome();

WriteLog("[AXIS] OnAxis-axAXIS - Step 7");

			checkOpenedList();	// 20070627 kwon

WriteLog("[AXIS] OnAxis-axAXIS - Step 8");

			//checkDelayList();	// 20071205 won

WriteLog("[AXIS] OnAxis-axAXIS - Step 9");

			m_tInfo1->RedrawWindow();

			// 2010.06.01 보안프로그램이 모두 해제되었을 경우 && 고객일경우
			//modi ASTx 202409  보안모듈 미실행시 위에서 HTS 강제 종료
			//const BOOL pcAOS = AfxGetApp()->GetProfileInt(INFORMATION, "AOS", 1);
			//const BOOL pcFirewall = AfxGetApp()->GetProfileInt(INFORMATION, "PCFirewall", 0);
			//const BOOL pcKeyProtect = AfxGetApp()->GetProfileInt(ENVIRONMENT, "KeyProtect", 0);
			//if ( (!pcFirewall || !pcKeyProtect || !pcAOS) && Axis::isCustomer )
			//{
			//	load_secure_agree(pcAOS, pcFirewall, pcKeyProtect);
			//}

WriteLog("[AXIS] OnAxis-axAXIS - Step 10");

			// 2010.06.01 사용자 OS를 Report한다.
			// 2012.02.13 박현철과장 제거 지시.BY DUKKI
			//os_report();

WriteLog("[AXIS] OnAxis-axAXIS - Step 11");

			// 2010.09.03 접속 Ethernet Card의 MAC을 Report 한다.
			//mac_report();

			// 2010.09.27 접속시 기존 화면Counter 정보를 로딩한다.
			// 2012.03.22 박현철과장 제거 지시.BY DUKKI
			load_scr_counter();

			// 2010.07.21 숨김처리 Hotkey 로딩
			LoadHotkeySetting();

WriteLog("[AXIS] OnAxis-axAXIS - Step 12");

			// 공지사항 체크
			//sendTR("PIHONOTI", "Y", 1, US_PASS, 'l');

WriteLog("[AXIS] OnAxis-axAXIS - Step 13");

			CreateHistoryBar();

		//	if (atoi(m_axis->GetProfileString(INFORMATION, "Port")) == 80)
			trouble_shooting(m_axis->GetProfileString(INFORMATION, "Port"), "Port", "Port");
			
			//CString strFile;
			////7805 팝업 
			//strFile.Format("%s\\tab\\NOTICECOOKIE.ini", Axis::home); 
			//
			//char buff[1024];
			//GetPrivateProfileString("7805","VISIBLE","Y",buff,sizeof(buff),strFile);
			//
			//CString str;
			//str = buff;
			//
			//if(str == "Y")
			//{
			//	GetPrivateProfileString("7805","TODAY","0",buff,sizeof(buff),strFile);
			//	
			//	str = buff;
			//	
			//	const int nFirst = GetPrivateProfileInt("7805","FIRST",1,strFile);
			//	
			//	const CTime tm(CTime::GetCurrentTime());
			//	CString today;
			//	today.Format("%04d%02d%02d",tm.GetYear(),tm.GetMonth(),tm.GetDay());
			//	
			//	if(str != today)
			//	{
			//		WritePrivateProfileString("7805","FIRST","1",strFile);
			//	}
			//	
			//	if((str == today && nFirst == 1) || str != today)
			//	{
			//		CTime time;
			//		time = CTime::GetCurrentTime();
			//
			//		char readB[1024];
			//		int readL;
		
			//		readL =	GetPrivateProfileString("7805","FROM","",readB,sizeof(readB),strFile);

			//		if(readL == 0)  //파일이 없거나 데이터를 못읽으면 그냥 띄운다
			//		{
			//			SetTimer(TM_POPUP_JISU, 7000, NULL);
			//			m_bInit = FALSE;
			//			break;
			//		}

			//		CString strFrom(readB,readL);
			//		const CTime FromTime(atoi(strFrom.Mid(0,4)), atoi(strFrom.Mid(4,2)), atoi(strFrom.Mid(6,2)), atoi(strFrom.Mid(8,2)), atoi(strFrom.Mid(10,2)), atoi(strFrom.Mid(12,2)));
			//		
			//		readL =	GetPrivateProfileString("7805","TO","",readB,sizeof(readB),strFile);

			//		if(readL == 0)  //파일이 없거나 데이터를 못읽으면 그냥 띄운다
			//		{
			//			SetTimer(TM_POPUP_JISU, 7000, NULL);
			//			m_bInit = FALSE;
			//			break;
			//		}

			//		CString strTo(readB,readL);
			//		const CTime ToTime(atoi(strTo.Mid(0,4)), atoi(strTo.Mid(4,2)), atoi(strTo.Mid(6,2)), atoi(strTo.Mid(8,2)), atoi(strTo.Mid(10,2)), atoi(strTo.Mid(12,2)));
			//		
			//		if(time <= FromTime || time >= ToTime)
			//		{
			//			SetTimer(TM_POPUP_JISU, 7000, NULL);	
			//		}
			//	}
			//}
			////////////////////////////////////////////////
			//dkkim 2018.11.12 AXSOCK의 원복으로 기능 제외.아래 기능 활성화를 위해서는 AXSOCK의 기능이 필요함.
// 			strFile.Format("%s\\tab\\RSCMONITOR.ini", Axis::home); 
// 			
// 			int nUse = GetPrivateProfileInt("RSCMONITOR","CPU",0,strFile);
// 			int nStaffOnly = GetPrivateProfileInt("RSCMONITOR","STAFFONLY",0,strFile);
// 			int nTerm = GetPrivateProfileInt("RSCMONITOR","TERM",0,strFile);
// 
// 			if(nUse)
// 			{
// 				switch(nStaffOnly)
// 				{
// 					case 0:
// 						{
// 							if(m_n2022Cnt > 0 || Axis::devMode)
// 							{				
// 								SetTimer(TM_RTSQUEUE, nTerm, NULL);
// 							}
// 						}
// 						break;
// 					case 1:
// 						{
// 							if((!Axis::isCustomer && m_n2022Cnt > 0) || Axis::devMode)
// 							{				
// 								SetTimer(TM_RTSQUEUE, nTerm, NULL);
// 							}
// 						}
// 						break;
// 					case 2:
// 						{
// 							if((Axis::isCustomer && m_n2022Cnt > 0) || Axis::devMode)
// 							{				
// 								SetTimer(TM_RTSQUEUE, nTerm, NULL);
// 							}
// 						}
// 						break;
// 					case 3:
// 						{
// 							char buff[MAX_PATH+1];
// 							
// 							GetPrivateProfileString("RSCMONITOR","ID","",buff,MAX_PATH+1,strFile);
// 
// 							CString IDs;
// 							IDs.Format("%s",buff);
// 
// 							if(IDs.Find(Axis::userID) > -1)
// 							{
// 								if(m_n2022Cnt > 0 || Axis::devMode)
// 								{				
// 									SetTimer(TM_RTSQUEUE, nTerm, NULL);
// 								}
// 							}
// 						}
// 						break;
// 				}
// 			}

			m_bInit = FALSE;
			CreateAgentProcess(); 
		}
		break;
	case axSIGNON:	
//		OutputDebugString("GLB case axSIGNON");
		if(m_bCertLogin)
			signOnCert();
		else
			signOn();			
		break;
	case axCLOSEV:	axCloseV(HIWORD(wParam));	break;
	case axRELOAD:	refleshMap(m_activeKey);	break;
	case axFOCUS:
		m_activeKey = HIWORD(wParam);
		{
			int	key = 0;
			CChildFrame*	pChild = NULL;
			CSChild*	sChild = NULL;
			for ( int ii = 0 ; ii < 6 ; ii++ )
			{
				POSITION	pos = m_arMDI[ii].GetStartPosition();
				while (pos)
				{
					m_arMDI[ii].GetNextAssoc(pos, key, pChild);
					pChild->RedrawCaption();
				}

				pos = m_arSDI[ii].GetStartPosition();
				while (pos)
				{
					m_arSDI[ii].GetNextAssoc(pos, key, sChild);
					sChild->RedrawCaption();
				}
			}
		}
		//**TRACE("=================================================axFocus[%d]====================================================\n", m_activeKey);
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
		value = 0;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&value, (BYTE *)(VTS_I4 VTS_I4),
					setGID, (long)MAKELONG((WORD)lParam, HIWORD(wParam)));
		break;
	case axSCREEN:		return modifyWorkspace(HIWORD(wParam) ? false : true, (char *)lParam, m_saveALLVS);
	case axMODAL:		loadMap(HIWORD(wParam), (int)lParam);		break;
	case axDEVICE:
		if (m_modal)	key = m_modal->m_vwKEY;
		else
		{
			bool	match = false;
			CGPop* pop{};
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
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&value,
						(BYTE *)(VTS_I4 VTS_I4), MAKELONG(devEV, key), 0);
		break;
	case axURL:
		{
			CProfile profile(pkLink);
			CString url(profile.GetString("LINKINFOMATION", HIWORD(wParam) ? "ECN": "EMERGENCY"));
			Trim(url);
			if (url.IsEmpty())
				return 0L;
			PopupWeb(url);
		}
		break;
	case axWRITEIMG:	writeImg(Axis::home, HIWORD(wParam));	break;
	case axPRINTIMG:	printImg(m_printS);			break;
	case axSHOWHELP:	HelpLink(m_helpKey);			break;
	case axALARM:
		if (m_runAxis)
		{
			if(m_alarmMsg.GetAt(0) == 'Y')
			{
				m_alarmMsg = m_alarmMsg.Mid(1);

				m_alarmMsg.Replace("\\r"," ");
				m_alarmMsg.Replace("\\n","");

				ShowMngInfo(m_alarmMsg,5);  //onAXIS  case axALARM
			}
			else
			{
				CAlarm	dlg;
				dlg.SetMsg(m_alarmMsg);
				dlg.DoModal();
			}
		}
		break;
	case axDIALOG:
		{
			tmps = m_alarmMsg;
			switch (HIWORD(wParam))
			{
			default:	break;
			case 0x96:	break;
			case 0x97:	displayMsgBox("이중접속 알림", tmps);	break;
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
		    const int	nType = HIWORD(wParam);
			switch (nType)
			{
			case 0:
				{
#ifdef DF_USE_CPLUS17
					CLink	dlg(m_tMenu.get(), (char*)lParam);
#else
					CLink	dlg(m_tMenu, (char *)lParam);
#endif
					if (dlg.DoModal() == IDOK)
					{
					}
				}
				break;
			}
		}
		break;
	case axChaserON:	CreateChaser();	break;
	case axChaserOFF:	DeleteChaser();	break;
	case axGetActiveKey:	return m_activeKey;
	case axMRADAR:
		{
			switch (HIWORD(wParam))
			{
			case 0 :	// 아이피정보
				return (LONG)(char*)(const char*)m_ip;
			case 1 :	// 가상화면 번호
				return (LONG)m_vsN;
			case 2 :	// 라이브러리 인스턴스
				if (m_hMRadar == NULL)
				{
					m_hMRadar = LoadLibrary("MRTick.dll");					
				}
				
				return (LONG)m_hMRadar;
			}			
		}
		break;
	case axTEST:		
		ShowMngInfo(Format("%s\t%d\t%s\t%d\t%s\t%s\t", 
					MNG_FLAG, NO_CHECK+1, MNG_KIND, HIWORD(wParam), MNG_MSG, (char*)lParam));
		break;	
	case axFixedFrame:	// 이인호 fixed옵션
		{
			key = HIWORD(wParam);
			const CSize	size(LOWORD(lParam), HIWORD(lParam));
			int	nCx{}, nCy{}, gapX{}, gapY{}, viewType{};
			gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
			gapX += GetSystemMetrics(SM_CXBORDER) * 2;

			gapY  = GetSystemMetrics(SM_CYFRAME) * 2;
			gapY += GetSystemMetrics(SM_CYBORDER) * 2;

			CChildFrame*	child;
			if (!m_arMDI[m_vsN].Lookup(key, child))
			{
				CSChild*	schild;
				if (!m_arSDI[m_vsN].Lookup(key, schild))
					break;

				if ((schild->m_vwTYPE & vtypeMSK) != vtypeWND)
					gapY += XCAP_HEIGHT;
				if (schild->IsTabView())	
					gapY +=	TABH;

				viewType = schild->m_vwTYPE & vtypeMSK;
				nCx = size.cx + gapX;
				nCy = size.cy + gapY;
				schild->SetSize(nCx, nCy);
				
				schild->m_fixH = true;
				schild->m_fixW = true;
				
				CView*	view = schild->GetActiveView();
				schild->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
				if (viewType == vtypeSCR)
					((CAxScrollView *)view)->SetSizeView(size.cx, size.cy);
				if (schild->IsTabView())	
					((CAxTabView *) view)->SizeWindow(size);
				break;
			}

			if ((child->m_vwTYPE & vtypeMSK) != vtypeWND)	gapY += XCAP_HEIGHT;
			if (child->IsTabView())	
				gapY +=	TABH;

			viewType = child->m_vwTYPE & vtypeMSK;
			nCx = size.cx + gapX;
			nCy = size.cy + gapY;
			child->SetSize(nCx, nCy);
			
			child->m_fixH = true;
			child->m_fixW = true;
			
			CView*	view = child->GetActiveView();
			child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
			if (viewType == vtypeSCR)
				((CAxScrollView *)view)->SetSizeView(size.cx, size.cy);
			if (child->IsTabView())	
				((CAxTabView *)view)->SizeWindow(size);	
		}
		break;
	case axRTSQueue:	// 20070621
		// RTS 테스트
		{
			long	rc = 0;
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
								(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(getRTS, 0), 0);
			return rc;
		}

		break;
	case axMinMaxInfo:
		return SetChildMinMaxInfo(HIWORD(wParam), (MINMAXINFO*)lParam);
	case axMenuMode:
		return SetMenuMode(HIWORD(wParam), lParam);
	case axIsDLL:
		return m_mapHelper->IsDLL((LPCTSTR)lParam);
	case axSkinChange :
		{
			m_pMain->change_Skin();
			//AfxMessageBox("SKIN CHANGE!!!");

		}
		break;
	case axToolbarHide :
		{
	//		OutputDebugString("axToolbarHide\n");
			BOOL	visible = FALSE;

			visible = !(m_bar1->GetStyle() & WS_VISIBLE);
#ifdef DF_USE_CPLUS17
			ShowControlBar(m_bar1.get(), visible, FALSE);
#else
			ShowControlBar(m_bar1, visible, FALSE);
#endif			
			saveToolStatus();
			
			ChangeLogo();
		}
		break;
#ifdef DF_MK_CAPTION
		case axGetMarKetType:
		{
			int permissionFlag = LOWORD(wParam);

			CString* pMapNum = reinterpret_cast<CString*>(lParam);
			if (pMapNum == nullptr)
				return FALSE; 

			CString screenNo = *pMapNum;
			delete pMapNum;

			int selectable = GetMarketType(screenNo);

			return selectable;
		}
		break;
		case axSetMarKetType:
		{
			CString* pMapInfo = reinterpret_cast<CString*>(lParam);
			if (pMapInfo == nullptr)
				return FALSE;

			CString sInfo = *pMapInfo;
			delete pMapInfo;

			CString screenNo;
			screenNo = Parse(sInfo, "\t");

			CString tmp;
			int	key = 0;
			CChildFrame* pChild = NULL;
			CSChild* sChild = NULL;
			for (int ii = 0; ii < 6; ii++)
			{
				POSITION	pos = m_arMDI[ii].GetStartPosition();
				while (pos)
				{
					m_arMDI[ii].GetNextAssoc(pos, key, pChild);
					if (pChild->m_mapN == screenNo)
					{
						tmp.Format("edMarketTrigger\t%s", sInfo);
						m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY,
							(void*)NULL, (BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, pChild->m_key), (LPARAM)(const char*)tmp);

						break;
					}
				}

				pos = m_arSDI[ii].GetStartPosition();
				while (pos)
				{
					m_arSDI[ii].GetNextAssoc(pos, key, sChild);
					tmp.Format("edMarketTrigger\t%s", sInfo);

					if (sChild->m_mapN == screenNo)
					{
						m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY,
							(void*)NULL, (BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, sChild->m_key), (LPARAM)(const char*)tmp);
					}
				}
			}
			

		}
		break;
#endif
	}
	return 0;
}

CString CMainFrame::EncryptAES(CString enc)
{
	CString rtnEnc;
	CString path;

	path.Format("%s\\%s\\%s", Axis::home, DEVDIR, "CX_AES.DLL");
	HMODULE hModule = LoadLibrary(path);
	char* data = (LPSTR)(LPCTSTR)enc;
	char* aes{};

	CString s;
	if(hModule)
	{
		typedef long (WINAPI *GETSHAFUNC)(char*, int);
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axEncrypt");
		aes = (char*)func(data, 6);
		char* pdata = new char[strlen(aes) + 1];
		memset(pdata, 0x00, strlen(aes) + 1);
		memcpy(pdata, aes, strlen(aes));
		if (func)
			rtnEnc.Format("%s", pdata);
	}

	if(hModule)
		FreeLibrary(hModule);

	hModule = NULL;
	return rtnEnc;
}

CString CMainFrame::DecryptAES(CString dec)
{
	CString s;
	s.Format("[axis][mainframe] DecryptAES  dec [%s]\n", dec);
//	OutputDebugString(s);

	CString rtnEnc;
	
	CString path;
	path.Format("%s\\%s\\%s", Axis::home, DEVDIR, "CX_AES.DLL");

	HMODULE hModule = LoadLibrary(path);
	
	char* data = (LPSTR)(LPCTSTR)dec;
	
	char* aes{};
	
	if(hModule)
	{
		typedef long (WINAPI *GETSHAFUNC)(char*);
		
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axDecrypt");

	//	OutputDebugString("FUNCTION CALL\n");

		if (func)
		{
			aes =  (char*)func(data);
			
			rtnEnc.Format("%s",aes);
			
			s.Format("AES SRC [%s] DEC [%s]\n",data,aes);
		//	OutputDebugString(s);
		}
	}

	if(hModule)
		FreeLibrary(hModule);

	s.Format("!!!!!!!!!!!!!!! [axis][mainframe] rtnEnc=[%s]\n", rtnEnc);
//	OutputDebugString(s);
	return rtnEnc;
}

void CMainFrame::InitMapHK()
{
	m_mapHWndToKey.RemoveAll();

	for(int ii = 237 ; ii < 249; ii++)
		m_mapHWndToKey.SetAt(ii, "");

#ifdef DF_MAIN_RTS
	InitPool();
#endif
}

int CMainFrame::GetKeyByHWnd(HWND hwnd, int igubn)
{
	int key{};
	CString temp, sdata;
	sdata.Format("%d\t%d", hwnd, igubn);
	for (POSITION pos = m_mapHWndToKey.GetStartPosition(); pos;)
	{
		m_mapHWndToKey.GetNextAssoc(pos, key, temp);
		if (temp.IsEmpty())
		{
			m_mapHWndToKey.SetAt(key, sdata);
			return key;
		}
	}
	return 0;
}
HWND CMainFrame::GetHWndByKey(int key, int& msg)
{
	HWND hwnd;
	CString temp;
	if (m_mapHWndToKey.Lookup(key, temp))
	{
		hwnd =(HWND)atoi(Parser(temp, "\t"));
		msg = atoi(temp);
		return hwnd;
	}
	return nullptr;
}

LONG CMainFrame::OnInterMsg(WPARAM wParam, LPARAM lParam)
{   //::SendMessage(m_pMain->m_hWnd, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_SEARCH_INTERGROUP, 0), this->m_hWnd), 0);
	//		stmp.Format("%s", (char*)lParam);
	//		m_hInter = (HWND)atoi(stmp);
	//		HWND hwd = (HWND)HIWORD(wParam); 
	/*CString stemp;
	CString strHwnd;
	strHwnd.Format("%d", this->m_hWnd);
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_SET_INTEREMAP, -1), this->m_hWnd), (LPARAM)(LPCTSTR)strHwnd);*/

	CString stmp;
	switch (LOWORD(wParam))
	{
		case MMSG_TIME_INVESTTUJA:
		{
			sendTicInfo();
		}
		break;
		case MMSG_SEARCH_INTERGROUP:		//관심그룹 조회 axiscp::SetTickinfo
		{
				stmp.Format("%s", (char*)lParam);
				m_hInter = (HWND)atoi(stmp);
				int key = GetKeyByHWnd(m_hInter, LOWORD(wParam));

				int	sendL = 0;
				char	sendB[16 * 1024] = { 0, }, tempB[32];
				int isz_updn = sz_uinfo;
				struct _updn* updn = (struct _updn*)&sendB[sendL];
				sendL += isz_updn;

				FillMemory((char*)updn, isz_updn, ' ');

				CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
				updn->uinfo.dirt[0] = 'U';
				updn->uinfo.cont[0] = 'g';
				CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
				updn->uinfo.retc[0] = 'U';

			    sendTR("PIDOMYST", sendB, sendL, US_PASS, key);
		}
		break;
		case MMSG_SEARCH_GROUPCODE:
		{
			stmp.Format("%s", (const char*)lParam);
			m_hInter = (HWND)atoi(Parser(stmp, "\t"));
			int key = GetKeyByHWnd(m_hInter, LOWORD(wParam));

			int	sendL = 0;
			char	sendB[16 * 1024] = { 0, }, tempB[32];
			int isz_updn = sz_uinfo + 2;
			struct _updn* updn = (struct _updn*)&sendB[sendL];
			sendL += isz_updn;

			FillMemory((char*)updn, isz_updn, ' ');

			CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
			updn->uinfo.dirt[0] = 'U';
			updn->uinfo.cont[0] = 'j';
			CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
			updn->uinfo.retc[0] = 'U';

			memcpy((char*)&sendB[sz_uinfo], stmp, 2);

			sendTR("PIDOMYST", sendB, sendL, US_PASS, key);
		}
		break;
		case MMSG_SEARCH_SISE:
		{
			stmp.Format("%s", (const char*)lParam);
			m_hInter = (HWND)atoi(Parser(stmp, "\t"));
			int key = GetKeyByHWnd(m_hInter, LOWORD(wParam));
			int ilen = HIWORD(wParam);
		
			sendTR("PIBOSISE", (LPSTR)(LPCTSTR)stmp, ilen, US_PASS, key);
		}
		break;
	//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	case MMSG_INTER_NEWGROUP:			//관심 새그룹 등록
	//	case MMSG_INTER_DBBACKUP:				//관심DB백업
	//	case MMSG_SEARCH_GROUPCODE:      //관심그룹 종목코드 조회   [main->hidden]
	//	case MMSG_SEARCH_INTERGROUP:		//관심그룹 조회  [main->hidden]
	//	case MMSG_INTER_GROUPCODESAVE:   //관심그룹저장  [main->hidden]
	//	{
	//		::SendMessage(m_hInter, WM_MSG_MAINTOMAP, wParam, lParam);
	//	}
	//	break;
	//	
	//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	case MMSG_RET_INTER_NEWGROUP:						 //숨은 화면이  새그룹등록한 결과를 요청화면에게  < hidden->main>
	//	case MMSG_RET_SEARCH_GROUPCODE:				//숨은 화면이  관심그룹 종목 조회한 결과를 요청화면에게  < hidden->main>
	//	case MMSG_RET_INTER_DBBACKUP:						//DB백업 결과를 요청화면에게  < hidden->main>
	//	case MMSG_RET_SEARCH_INTERGROUP:				//숨은 화면이  관심그룹 조회한 결과를 요청화면에게  < hidden->main>
	//	case 	MMSG_RET_INTER_GROUPCODESAVE:          //관심그룹저장결과를 요청화면에
	//	{  
	//		HWND hwnd{};
	//		hwnd = (HWND)HIWORD(wParam);
	//		const int isize = HIBYTE(LOWORD(wParam));
	//		const int hkey = LOBYTE(LOWORD(wParam));
	//		::SendMessage(hwnd, WM_MSG_MAINTOMAP, MAKEWPARAM(MAKEWORD(hkey, isize), 0), lParam);
	//	}
	//	break;
	}
	return 0;
}

LONG CMainFrame::OnUSER(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case MMSG_GETPASSWORD:
			return (LRESULT)(LPCTSTR)GetUserPassword();
		case MMSG_GETCERTPASSWORD:
			return (LRESULT)(LPCTSTR)GetCertPassword();
		case MMSG_APPLYSETUP:
			return ApplySetup(lParam);
		case MMSG_SCREENOPEN:
			return m_pMain->InputScreenNo((char*)lParam) ? true : false;
		case MMSG_SAVEINTEREST:
		{
			//CString tmp = CString((char*)lParam, HIWORD(wParam));
			sendTR("PIDOMYST", (char*)lParam, HIWORD(wParam), US_PASS, 'S');
		}
		return true;
		case MMSG_NEWINTEREST:
		{
			const char* trust = (char*)lParam;//"IB202201appendCODE\t005930\n";
			m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
		}
		return true;
		case MMSG_RESTORECONDLG:
			ShowConclusion();
			break;
		case MMSG_RESTORESERVERORD:  //mod 서버
		{
#ifdef DF_SERVERAUTO
			switch ((int)lParam)
			{
			case SERVERORDER_MSG_MAPPOP:
				ShowServerOrdDlg();
				break;
			case SERVERORDER_MSG_SVCREGI: break;  //서비스 신청되었음
			case SERVERORDER_MSG_SVCTERMINATE: break; //서비스 해지
			case SERVERORDER_MSG_SELLCONOK:  //(매도) 감시조건을 충족  /주문내역보기
			case SERVERORDER_MSG_BUYCONOK:  //(매수) 감시조건을 충족  /주문내역보기 
			case SERVERORDER_MSG_SELLNEWCONOK: //(신규편입매도) 감시조건을 충족  /주문내역보기
			case SERVERORDER_MSG_CONEXPD: //조건만료  /감시내역보기
			case SERVERORDER_MSG_MISORDER: //착오주문
				ServerOrderMsgToMap((int)lParam, true);  //서버자동주문 내역창 클릭시 오는 메시지이기 때문에 관련맵화면이 열려있지 않으면 팝업한다
			break;
			case SERVERORDER_MSG_CONSTATUE:  //감시현황
			{
			}
			break;
			default:

				break;
			}
#endif
		}
		break;
		case MMSG_SHARED_GETKEY:  //cx_log 에 공유메모리키 
		{
			return (LONG)m_sMSharedkey.GetBuffer(0);
		}
		break;
		case MMSG_SHARED_REGWND:  //cx_shared 에 관리를 할 윈도우 주소 등록
		{
			if (m_pSharedMemory)
				m_pSharedMemory->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(MMSG_SHARED_REGWND,1 ), 1), (LPARAM)lParam);
		}
		break;
		case MMSG_SHARED_BROADCAST:  //cx_shared 에 관리를 하고 있는 윈도우에게 문자열 브로드 캐스팅
		{
			if (m_pSharedMemory)
				m_pSharedMemory->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(MMSG_SHARED_BROADCAST, 1), 1), (LPARAM)lParam);
		}
		break;
		case MMSG_SHARED_CTRLDESTROY:
		{
			if (m_pSharedMemory)
				m_pSharedMemory->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(MMSG_SHARED_CTRLDESTROY, 1), 1), (LPARAM)lParam);
		}
		break;
		case MMSG_SHARED_GETHANDLECNT:
		{
			if (m_pSharedMemory)
				return (int)(m_pSharedMemory->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(MMSG_SHARED_CTRLDESTROY, 1), 1), (LPARAM)lParam));
		}
		break;
		case MMSG_SHARED_GUIDEMESSAGE:
		{
			displayGuide((char*)lParam);
			return 1;
		}
		break;
		case MMSG_POP_MAP:
		{
			switch (HIWORD(wParam))
			{
				case 0:
				{
					m_slog.Format("%s", (char*)lParam);
					m_mapHelper->CreatePopup((char*)lParam, 1, WK_POPUP, CenterPOS);
				}
				break;
				case 1:
				{
					//m_mapHelper->CreateModal((char*)lParam, 0, 0x9f - 4, 99);
					SendSACMT279();
				}
				break;
				case 2:
				{
					m_slog.Format("%s", (char*)lParam);
					m_mapHelper->CreateModal((char*)lParam, 0, 0x9f - 4, 6);
				}
				break;
				case 3:
				{
					CString sval;
					sval.Format("%s", (char*)lParam);
					sval.TrimRight();
					InputScreenNo(sval);
				}
				break;
			}
		}
		break;
		case MMSG_GETDN_CERTIFY:
		{
			m_slog.Format("[%s]<%d> [FDS] m_strDN=[%s]\n", __FUNCTION__, __LINE__, m_strDN);
			WriteLog(m_slog);
			return (LRESULT)(LPCTSTR)m_strDN;
		}
		break;
#ifdef DF_MK_CAPTION
		case MMSG_MKMSG_FROM_MAPNMAP:
		{
			CString sMapN, sMarket, sPermission, stmp, stemp;
			int major{};
			stmp.Format("%s", lParam);
			stemp = stmp;
			sMapN = Parser(stemp, "\t"); sMapN.TrimRight();
			sMarket = Parser(stemp, "\t");  sMarket.TrimRight();
			sPermission = Parser(stemp, "\t");  sPermission.TrimRight();
			major = atoi(Parser(stemp, "\t"));

			if (sMapN.IsEmpty())
				return 0;

			m_slog.Format("[AXIS][MARKET] 시작 MMSG_MKMSG_FROM_MAPNMAP");
			OutputDebugString(m_slog);
m_slog.Format("[AXIS][MARKET] MMSG_MKMSG_FROM_MAPNMAP    sMapN=[%s] sMarket=[%s] sPermission=[%s] major=[%d]", 
						sMapN, MarketToString(sMarket), sPermission, major);
OutputDebugString(m_slog);

			int nActGroup = 0;

			CChildFrame* actChild{};
			CSChild* actSchild{};

			if (m_arMDI[m_vsN].Lookup(m_activeKey, actChild))
			{
				nActGroup = actChild->m_xcaption.GetGroup();
			}
			else if (m_arSDI[m_vsN].Lookup(m_activeKey, actSchild))
			{
				nActGroup = actSchild->m_xcaption.GetGroup();
			}

			CChildFrame* child{};
			CSChild* schild{};

			int	key{};
			const long	rc = 0;
			POSITION pos;

			for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
			{
				m_arMDI[m_vsN].GetNextAssoc(pos, key, child);

				//if (nActGroup == child->m_xcaption.GetGroup())
				{
					if (sMapN == child->m_xcaption.GetMapNum())
					{
						if (!child->m_xcaption.m_MkLock && child->m_xcaption.m_marketN != 4)   //자물통이거나 선택불가 아니고
						{
							//if(major == key)   //우선 1000번 예외
							//if (major == key || sMapN == "IB100000")
							if (major == key )
							{
								if ((int)atoi(sMarket) != child->m_xcaption.m_marketN)
									child->m_xcaption.SetMarket((int)atoi(sMarket));

							m_slog.Format("[AXIS][MARKET] ---찾음--- MMSG_MKMSG_FROM_MAPNMAP  sPermission =[%s] sMarket=%s] sMapN=[%s][%d] GetMapNum() =[%s][%d]  ", 
								sPermission, MarketToString(sMarket), sMapN, major, child->m_xcaption.GetMapNum(), key);
								OutputDebugString(m_slog);

								if (0)
									child->m_xcaption.SetMapPermission(atoi(sPermission));
								else
									child->m_xcaption.m_iMkPermission = atoi(sPermission);
							}
						}
					}
				}
			}

			for (pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
			{
				m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);

				//if (nActGroup == schild->m_xcaption.GetGroup())
				{
					if (!schild->m_xcaption.m_MkLock && schild->m_xcaption.m_marketN != 4)
						if ((int)atoi(sMarket) != schild->m_xcaption.m_marketN)
							schild->m_xcaption.SetMarket((int)atoi(sMarket));
			
					if (0)
						schild->m_xcaption.SetMapPermission(atoi(sPermission));
					else
						schild->m_xcaption.m_iMkPermission = atoi(sPermission);
				}
			}
		}
		break;
		case MMSG_MKMSG_FROM_MAP:
		{
			CChildFrame* actChild{};
			CSChild* actSchild{};

			if (m_arMDI[m_vsN].Lookup(m_activeKey, actChild))
			{
				if (!actChild->m_xcaption.m_MkLock && actChild->m_xcaption.m_marketN != 4)
				{

					m_slog.Format("[AXIS][MARKET] ---찾음2--- MMSG_MKMSG_FROM_MAP  maketType = [%s]  ", MarketToString(CString((char*)lParam)));
					OutputDebugString(m_slog);

					CString stmp;
					stmp.Format("%s", (char*)lParam);
					stmp.TrimRight();
					actChild->m_xcaption.SetMarket(atoi(stmp));
				}
			}
			else if (m_arSDI[m_vsN].Lookup(m_activeKey, actSchild))
			{
				if (!actSchild->m_xcaption.m_MkLock && actSchild->m_xcaption.m_marketN != 4)
					actSchild->m_xcaption.SetMarket((int)lParam);
			}
		}
		break;
		case MMSG_MKMSG_FROM_MAPSMAP:
		{
			CString sMapN, sMarket, sPermission, stmp, stemp;
			int major{};
			stmp.Format("%s", lParam);
			stemp = stmp;
			sMapN = Parser(stemp, "\t"); sMapN.TrimRight();
			sMarket = Parser(stemp, "\t");  sMarket.TrimRight();
		/*	sPermission = Parser(stemp, "\t");  sPermission.TrimRight();
			major = atoi(Parser(stemp, "\t"));*/

			CChildFrame* actChild{};
			CSChild* actSchild{};

			if (m_arMDI[m_vsN].Lookup(m_activeKey, actChild))
			{
				if (!actChild->m_xcaption.m_MkLock || actChild->m_xcaption.m_marketN != 4)
				{

					m_slog.Format("[AXIS][MARKET] ---찾음3--- MMSG_MKMSG_FROM_MAP  maketType = [%s]  ", MarketToString(CString((char*)lParam)));
					OutputDebugString(m_slog);

					CString stmp;
					stmp.Format("%s", (char*)lParam);
					stmp.TrimRight();
					actChild->m_xcaption.SetMarket(atoi(stmp));
				}
			}
			else if (m_arSDI[m_vsN].Lookup(m_activeKey, actSchild))
			{
				if (!actSchild->m_xcaption.m_MkLock || actSchild->m_xcaption.m_marketN != 4)
					actSchild->m_xcaption.SetMarket((int)lParam);
			}

		}
		break;
		case MMSG_MKMSG_GET_MAP_BLOCK:  //화면 내부의 cxMarket 클릭시 거래소선택기능 락 여부 확인
		{
			CChildFrame* actChild{};
			CSChild* actSchild{};

			if (m_arMDI[m_vsN].Lookup(m_activeKey, actChild))
				return actChild->m_xcaption.m_MkLock;
			else if (m_arSDI[m_vsN].Lookup(m_activeKey, actSchild))
				return actSchild->m_xcaption.m_MkLock;	
		}
		break;
		case MMSG_MKMSG_FROM_SM:   //HTS 툴바에서 전체 장구분자 바꾸기 메시지 관련
		{
			
			CString tmp;
			int	key = 0;
			CChildFrame* pChild = NULL;
			CSChild* sChild = NULL;
			for (int ii = 0; ii < 6; ii++)
			{
				POSITION	pos = m_arMDI[ii].GetStartPosition();
				while (pos)
				{
					m_arMDI[ii].GetNextAssoc(pos, key, pChild);

					if (pChild->m_xcaption.m_MkLock || pChild->m_xcaption.m_marketN == 4)
						continue;

					int mkgubn = HIWORD(wParam);
					int ibit{};
					if (mkgubn == 1)
						ibit = 1;
					else if (mkgubn == 2)
						ibit = 2;
					else if (mkgubn == 3)
						ibit = 4;
			
					if ((pChild->m_xcaption.m_iMkPermission & ibit) == 0)
					{
					/*	if (pChild->m_xcaption.m_iMkPermission & 1) mkgubn = 1;
						else if (pChild->m_xcaption.m_iMkPermission & 2) mkgubn = 2;
						else if (pChild->m_xcaption.m_iMkPermission & 4) mkgubn = 3;*/
						
						 if (pChild->m_xcaption.m_iMkPermission & 4) mkgubn = 3;
						 else if (pChild->m_xcaption.m_iMkPermission & 1) mkgubn = 1;
						 else if (pChild->m_xcaption.m_iMkPermission & 2) mkgubn = 2;
					}

					pChild->m_xcaption.SetMarket(mkgubn);

					tmp.Format("edMarketTrigger\t%s", mkgubn == 1 ? "KRX" : mkgubn == 2 ? "NXT" : "통합");
					OutputDebugString(tmp);
					m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY,
						(void*)NULL, (BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, pChild->m_key), (LPARAM)(const char*)tmp);
				}

				pos = m_arSDI[ii].GetStartPosition();
				while (pos)
				{
					int mkgubn = HIWORD(wParam);
					m_arSDI[ii].GetNextAssoc(pos, key, sChild);

					if (sChild->m_xcaption.m_MkLock || sChild->m_xcaption.m_marketN == 4)
						continue;

					sChild->m_xcaption.SetMarket(mkgubn);

					tmp.Format("edMarketTrigger\t%s", mkgubn == 1 ? "KRX" : mkgubn == 2 ? "NXT" : "통합");
					OutputDebugString(tmp);
					m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY,
						(void*)NULL, (BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, sChild->m_key), (LPARAM)(const char*)tmp);
				}
			}
		}
		break;
		case MMSG_SET_USER_ABORAD_AUTH:
		{
			m_sCustomerAuth.Format("%s", (char*)lParam);
			m_sCustomerAuth.TrimRight();
		}
		break;
		case MMSG_GET_USER_ABORAD_AUTH:
		{
			return (LRESULT)(LPCTSTR)m_sCustomerAuth;
		}
		break;
		case MMSG_MKMSG_GET_MAP_INFO:
		{
			CChildFrame* actChild{};
			CSChild* actSchild{};
			
			if (m_arMDI[m_vsN].Lookup(m_activeKey, actChild))
				m_slog.Format("%s\t%d", actChild->m_mapN, actChild->m_xcaption.m_marketN);
			else if (m_arSDI[m_vsN].Lookup(m_activeKey, actSchild))
				m_slog.Format("%s\t%d", actSchild->m_mapN, actSchild->m_xcaption.m_marketN);

			return (LRESULT)(LPCTSTR)m_slog;
		}
		break;
		case MMSG_MKMSG_GET_MAP_MARKET:
		{
			CString mapN;
			mapN.Format("%s", (char*)lParam);
			mapN.TrimRight();
			return GetMarketType(mapN);
		}
		break;
		case MMSG_MKMSG_GET_MAIN:
		{
			return reinterpret_cast<LRESULT>(this);
		}
		break;
		case MMSG_MAIN_TEST:
		{
			CString smp;
			smp.Format("%s", (char*)lParam);
			CheckMarketByMNG(smp);
			ShowMngInfo(smp, 1);
		}
		break;
		case MMSG_MAIN_TEST + 1:
		{
			m_slog.Format("[mnginfo][%s][%d] OnUser m_sMngMsg = [%s]", __FUNCTION__, __LINE__, m_sMngMsg);
			OutputDebugString(m_slog);
			m_mngInfo->SetData(m_sMngMsg, 1);

			WINDOWPLACEMENT	pl;
			GetWindowPlacement(&pl);
			m_mngInfo->ShowWindow(SW_HIDE);

			MngInfoPos();
			m_mngInfo->HideSlide();
		}
		break;
		case MMSG_GETMAIN_STATE:
		{
			return m_iAxisState;
		}
		break;
		case MMSG_SETMAIN_STATE:
		{
			m_iAxisState++;

			m_slog.Format("--------------------[Axis_State][MMSG_SETMAIN_STATE]  m_iAxisState =[%d]-------------", m_iAxisState);
			OutputDebugString(m_slog);
		}
		break;
#ifdef DF_MAIN_RTS
		case MMSG_SETSENDTR_TO_MAIN:  //관심등 화면에서 메인이 대신 sendtr 하도록...
		{
		// poolKey 할당
		int poolKey = AllocPoolKey();
		if (poolKey == -1)
		{
			OutputDebugString("TR poolKey 부족\n");
			return 0;
		}

		// poolKey → (HWND + key) 매핑
		ST_SEND_TR* pTr = (ST_SEND_TR*)lParam;
		TR_ROUTE_INFO info;
		info.hWnd = pTr->hSender;
		info.key = pTr->key;
		info.tick = GetTickCount();

		g_mapRoute[poolKey] = info;
		m_slog.Format("[2022][AXIS][%s]<%d> .... 메인[%x] 에서  key[%d] ", __FUNCTION__, __LINE__, info.hWnd, info.key);
		output_DebugString(m_slog);
		// 서버로 보낼 key = poolKey
		sendTR(pTr->trname, pTr->datB, pTr->datL, pTr->stat, poolKey);
		}
		break;
		case MMSG_GETRTS_FROM_MAIN:  //메인에게 RTS 데이터 얻어올때
		{

		}
		break;
		case MMSG_RT_REGISTER_CODES:
		{
			const RTS_REGISTER_REQ* req = (const RTS_REGISTER_REQ*)lParam;
			HWND hWnd = req->hWnd;
			if (!::IsWindow(req->hWnd) || !req)
				return 0;

			// -----------------------------
			// Debug Log 시작
			// -----------------------------
			{
				CString log;
				log.Format(_T("[MAIN][RTS등록] HWND=0x%p, codeCount=%d, symbolCount=%d\r\n"),
					hWnd, req->codeCount, req->symbolCount);
				OutputDebugString(log);

				// 코드 목록 출력
				for (int i = 0; i < req->codeCount; ++i)
				{
					if (req->codes[i][0] == '\0') continue;

#ifdef UNICODE
					CString code(req->codes[i]);
#else
					CString code(req->codes[i]);
#endif
					CString line;
					line.Format(_T("[AXIS][RTS등록]  CODE[%d] = %s\r\n"), i, code.GetString());
					OutputDebugString(line);
				}

				// 심볼 목록 출력
				for (int i = 0; i < req->symbolCount; ++i)
				{
				/*	CString line;
					line.Format(_T("[AXIS][RTS등록]  SYMBOL[%d] = %d\r\n"), i, req->symbols[i]);
					OutputDebugString(line);*/
				}

				OutputDebugString(_T("--------------------------------------------------\r\n"));
			}
			// -----------------------------
			// Debug Log 끝
			// -----------------------------

			// 실제 등록 로직
			RtsSubscription sub;

			sub.codes.reserve(req->codeCount);
			for (int i = 0; i < req->codeCount; ++i)
			{
				if (req->codes[i][0] == '\0') continue;
				sub.codes.emplace_back(req->codes[i]);
				EnsureSlotIndexForCode(req->codes[i]);
			}

			sub.symbols.reserve(req->symbolCount);
			for (int i = 0; i < req->symbolCount; ++i)
			{
				int s = req->symbols[i];
				if (s < 0 || s >= 1000) continue;
				if (!sub.symbolMask.test(s))
				{
					sub.symbolMask.set(s);
					sub.symbols.push_back(s);
				}
			}

			{
				std::unique_lock lk(g_subMtx);
				g_subByHwnd[hWnd] = std::move(sub);
			}

			CString slog;
			slog.Format("[EnsureSlot] 신규등록  총갯수=[%d]\n",
				(int)g_codeToIndex.size());
			OutputDebugString(slog);

			return 1;
		}
		break;
		case MMSG_RT_UNREGISTER_WND:
		{
			
		}
		break;
#endif //DF_MAIN_RTS
#endif //DF_MK_CAPTION
	}
	return 0;
}

void CMainFrame::onFireLog(int type, WPARAM wParam, LPARAM lParam)
{
	return;
	CString m_slog, str;
	m_slog = "\r\n \r\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ [OnFireRec]";
	switch (type)
	{
	case FEV_OPEN:
		if (!lParam)
		{
			m_slog += "FEV_OPEN";
			break;
		}
	case FEV_CLOSE:
	{
		m_slog += "FEV_CLOSE";
	}
	break;

	case FEV_RUN:
		if (lParam)
		{
			m_slog += "FEV_RUN";
			m_slog += "lParam Exist return";

		}
		else
		{
			m_slog += "FEV_RUN";
			m_slog += "lParam no ";
		}
		break;
	case FEV_SIZE:
	{
		m_slog += "FEV_SIZE";
	}
	break;
	case FEV_ANM:
		m_slog += "FEV_ANM";
		break;
	case FEV_AXIS:
		switch (LOWORD(wParam))
		{
		case newHISTORY:
		{
			m_slog += "FEV_AXIS  newHISTORY";
		}
		break;
		case runAXIS:
		{

			if (m_step != axDONE)
			{
				m_slog += "FEV_AXIS  runAXIS m_step = axDONE";
				m_slog += "\t";
			}

			m_slog += "FEV_AXIS  runAXIS ";
			m_slog += "\t";
		}
		break;
		case newVIEW:

			m_slog += "FEV_AXIS  newVIEW ";
			m_slog += "\t";

		case renVIEW:

			m_slog += "FEV_AXIS  renVIEW ";
			m_slog += "\t";
			break;
		case delVIEW:
			if (HIWORD(wParam) & winK_POPUP)
			{

				m_slog += "FEV_AXIS  delVIEW  HIWORD(wParam)  winK_POPUP";
				m_slog += "\t";
			}
			else
				m_slog += "FEV_AXIS  delVIEW ";

			break;
		case htmlVIEW: // key : HIWORD(wParam), html : lParam

			m_slog += "FEV_AXIS  htmlVIEW";
			break;
		case titleVIEW:

			// m_slog +=  "FEV_AXIS  titleVIEW"; 
			break;
		case autoVIEW: // ### : HIWORD(wParam) : cnt, items : lParam
			m_slog += "FEV_AXIS  autoVIEW";
			break;
		case mapNAME:   break;
		case waitPAN:
			// m_slog +=  "FEV_AXIS  mapNAME waitPAN"; 
			break;
		case hangulPAN: // key : HIWORD(wParam), hangul : lParam
		case doublePAN: // key : HIWORD(wParam), double : lParam
			m_slog += "FEV_AXIS  hangulPAN doublePAN";
			break;
		case alarmPAN: // dialog ? : HIWORD(wParam), text : lParam
			m_slog += "FEV_AXIS  alarmPAN";
			break;
		case noticePAN: // text : lParam
			m_slog += "FEV_AXIS  noticePAN";
			break;
		case userName:
			m_slog += "FEV_AXIS  userName";
			break;
		case dialogPAN: // type : HIWORD(wParam), data  : lParam
			m_slog += "FEV_AXIS  dialogPAN";
			str = (char*)lParam;

			switch (HIWORD(wParam))
			{
			default:
			case 0x01:

				if (m_b2018 && str.GetAt(0) == '@')
				{

				}
				break;
			case 0x02:  //서킷브레이크 관련
			{

			}
			break;
			case 0x96: // 긴급공지 -> 긴급 강제 다운로드로 변경(포맷은 추후에)! 2010.07.21 by LKM

				break;
			case 0x97: // 중복사용자..............

				break;
			case 0x99:

				if (str.GetLength() > 0)
				{
					switch (str.GetAt(0))
					{
					case '$':  // 재접속 여부를 묻지 않고 종료

						break;
					case '!':      //버전처리관련
					{

					}
					break;
					case '#':       //버전처리관련
					{
						str = str.Mid(1);

					}
					break;
					default:  // 재접속 여부를 묻고, 재접속/종료 시킴
					{

					}
					}
				}


				PostMessage(WM_CLOSE);
				break;
			case 0x98:  //이중접속
			{

				PostMessage(WM_CLOSE);
			}
			break;
			}
			break;
		case sizeVIEW:
			//  m_slog +=  "FEV_AXIS  sizeVIEW"; 
			break;
		case userINFO:
			
			m_slog += "FEV_AXIS  userINFO";
			break;
		case printVIEW:
		{
			m_slog += "FEV_AXIS  printVIEW";
		}
		break;
		case initVIEW:
		{
			m_slog += "FEV_AXIS  initVIEW";
		}
		break;
		case copyVIEW:
		{
			m_slog += "FEV_AXIS  copyVIEW";
		}
		break;
		case printVIEWx:
		{
			m_slog += "FEV_AXIS  printVIEWx";
		}
		break;
		case menuAXIS:
			m_slog += "FEV_AXIS  menuAXIS";
			break;
		case linkVIEW:
		{
			m_slog += "FEV_AXIS  linkVIEW";
		}
		break;
		case closeAXIS:
			m_slog += "FEV_AXIS  closeAXIS";
			if (HIWORD(wParam))
			{
				m_slog += "  RunVers";
				m_axMisc->RunVers(verRETRY);
			}

			PostMessage(WM_CLOSE);
			break;
		case nameVIEW:
			m_slog += "FEV_AXIS  nameVIEW";
			break;
		case accnINFO:
			m_slog += "FEV_AXIS  accnINFO";

			break;

		case userPASS:
			m_slog += "FEV_AXIS  userPASS";

			break;
		case captionVIEW: //** HIWORD(wParam): key, lParam: string buffer
			m_slog += "FEV_AXIS  captionVIEW";

			break;
		default:

			break;
		}
		break;

	case FEV_STAT:
	{
		m_slog += "FEV_STAT  ";
	}
	break;
	case FEV_ERROR:
		switch (m_step)
		{
		case axOPENRSM:
		{
			m_slog += "FEV_ERROR  axOPENRSM";
		}
		break;
		case axOPENSIGN:
		{
			if (m_bUseNewLogin)
			{
				m_slog += "FEV_ERROR  axOPENSIGN";
				if (lParam && m_axConnect)
				{
					// 직원 중복접속일 경우.
					CString msg((const char*)lParam);
					if (msg.GetLength() > 4 && atoi(msg.Left(4)) == 3192)
					{


					}



					{
						// 2011.08.02 비밀번호 오류일 경우 앞의 시간을 없앰(오류횟수표기를위해서)
						CString emsg = (char*)lParam;
						int ecod = atoi(emsg.Left(4));
						do {
							if (ecod == 4368 && Axis::isCustomer)
							{

							}
							else if (ecod == 9998)
							{

							}
							else if (ecod == 9999)
							{

							}
							else if (ecod == 4259)
							{

							}
						} while (FALSE);

					}

					if (wParam)
					{

					}
					else
					{

					}
				}  //m_slog += "FEV_ERROR  axOPENSIGN";
			}  //if (m_bUseNewLogin)

		}//case FEV_ERROR
		break;
		case axOPENWSH:
			m_slog += "FEV_ERROR  axOPENWSH";
			if (lParam)
			{
				m_slog += "axNONE";
			}
			break;
		case axNONE:
		case axDONE:
			m_slog += "FEV_ERROR  axNONE axDONE";
			if (lParam)
			{
				if (LOWORD(wParam))
				{

				}
				else
				{

				}
			}
			break;

		}
		break;
	case FEV_GUIDE:
		m_slog += "FEV_GUIDE   ";
		m_axMisc->GetGuide((int)lParam, str);
		m_slog += str;
		if (((int)lParam) == 57)
		{

		}
		else
		{

		}

		switch (m_step)
		{
		case axDONE:
			m_slog += "axDONE";
			if (LOWORD(wParam))
			{

			}
			else
			{

			}
			break;
		case axOPENSIGN:
		case axOPENWSH:
		{
			m_slog += "axOPENSIGN  axOPENWSH";
		}
		default:
		{

		}
		break;
		}
		break;
	case FEV_FMX:

		break;
	case FEV_VERS:
		m_slog += "FEV_VERS";
		break;
	default: break;
	}

	OutputDebugString(m_slog);
	///////////////////////////////////////////////////fireRect test/////////////////////////////////////////////////////////

}

int CMainFrame::OnFireRec(int type, WPARAM wParam, LPARAM lParam)
{
	BOOL	rc{};
	CString	str, msg, title, logs;
	CString s;
	if (m_forceClose)	return 0;

	//onFireLog(type, wParam, lParam);

	switch (type)
	{
	case FEV_OPEN:
		if (!lParam)
		{
			if(m_bUseNewLogin)
			{
				if (m_axConnect && IsWindow(m_axConnect->m_hWnd))
					m_axConnect->SetGuide(_T("서버와 연결되었습니다."));
			}
			else
			{
				if (m_axConnectOld && IsWindow(m_axConnectOld->m_hWnd))
					m_axConnectOld->SetGuide(_T("서버와 연결되었습니다."));
			}
			str = m_axis->GetProfileString(INFORMATION, "Port");
WriteLog("[AXIS] CMainFrame::OnFireRec FEV_OPEN Port=[%s]", str);
			if (atoi(str) == portEmployee)
				m_bCustomer = false;

			m_axis->WriteProfileInt(INFORMATION, "ConnectBy", Axis::isCustomer ? 0 : 1);
			break;
		}
	case FEV_CLOSE:
		{
WriteLog("[AXIS] CMainFrame::OnFireRec  FEV_CLOSE m_step=[%d] m_arrGlb size=[%d] m_iGlbIndex=[%d] \n", m_step, m_arrGlb.GetSize(), m_iGlbIndex);
	
			if(m_step == axOPENRSM && m_arrGlb.GetSize() - 1> m_iGlbIndex)
			{
				m_iGlbIndex++;
				logs.Format("GLB m_step = %d m_ip = %s", m_step, m_ip);
				OutputDebugString(logs);
				PostMessage(WM_AXIS, MAKEWPARAM(axSTART, 1));
				return 0;
			}
			
		
			m_bExit = false;
			m_forceClose = true;
			
			m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
					
			PostMessage(WM_AXISCLOSE);
		}
		break;

	case FEV_RUN:
//run:
		if (lParam)
		{
			
			if (m_update)	break;

			m_update = true;
			m_axMisc->GetGuide(AE_UPDATE, str);

WriteLog("[AXIS] CMainFrame::OnFireRec FEV_RUN  lParame EXIST [%s]\n", str);

			if(m_bUseNewLogin)
			{
				m_axConnect->SetProgress(false);
				m_axConnect->SetGuide(str);
			}
			else
			{
				m_axConnectOld->SetProgress(false);
				m_axConnectOld->SetGuide(str);
			}

			m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
						(BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
			Sleep(300);

			if(m_bUseNewLogin)
			{
				if (!m_axMisc->RunVers(verUPDATE, m_axConnect->GetUserID(), m_axConnect->GetPassword(), m_axConnect->GetCPass()))
				{
					m_axMisc->GetGuide(AE_EUPDATE, str);
					m_axConnect->SetGuide(str);

WriteLog("[AXIS] CMainFrame::OnFireRec FEV_RUN  lParame RunVers [%s]\n", str);

					Sleep(700);  //test2019
				}
				m_axConnect->EndDialog(IDCANCEL);
			}
			else
			{
				if (!m_axMisc->RunVers(verUPDATE, m_axConnectOld->GetUserID(), m_axConnectOld->GetPassword(), m_axConnectOld->GetCPass()))
				{
					m_axMisc->GetGuide(AE_EUPDATE, str);
					m_axConnectOld->SetGuide(str);
					Sleep(700); //test2019
				}
				m_axConnectOld->EndDialog(IDCANCEL);
			}
			return 0;
		}
		else
		{
WriteLog("[AXIS] CMainFrame::OnFireRec (lparam null )     FEV_RUN\n");
			SetForegroundWindow();
			// 무결성 검증 프로세스 
			//signOn();
			// 무결성 파일리스트 요청
			QueryPihoitgyList();
		}
		break;
	case FEV_SIZE:
		{
			m_axMisc->GetGuide(AE_COMPONENT, msg);

			if(m_bUseNewLogin)
			{
				m_axConnect->SetGuide(msg);

				if (m_axConnect)
				{
					m_axConnect->SetProgress(true);
					m_axConnect->SetTotal((char *)lParam);
				}
			}
			else
			{
				m_axConnectOld->SetGuide(msg);

				if (m_axConnectOld)
				{
					m_axConnectOld->SetProgress(true);
					m_axConnectOld->SetTotal((char *)lParam);
				}
			}
// 			CString s1;
// 			s1.Format("FEV_SIZE : [%s]\n",msg);
// 			OutputDebugString(s1);
		}
		break;
	case FEV_ANM:	
		{
			update_ticker((int)wParam, (struct _alertR*)lParam);
			//ForwardTickerToAgent(wParam, lParam);
		}
		break;
	case FEV_AXIS:
		switch (LOWORD(wParam))
		{	
		case newHISTORY:
			{
				CString sTemp = (char*)lParam;

				if(sTemp != "1301")
				{
					return 0;
				}

				if(!m_bHistory)
				{
					m_bHistory = TRUE;

					SetTimer(TM_HISTORY,150,NULL);
				}
			}
			break;
		case runAXIS:
			{
WriteLog("[AXIS] CMainFrame::OnFireRec  FEV_AXIS  runAXIS\n");
				//SendPiboStaf();
				// 인증 로그인시 사용자 아이디 설정
				if(m_bCertLogin)
				{
					m_bEnca = true;

					SendEnca(m_strDN,true);

					Axis::userID = m_axis->GetProfileString(WORKSTATION, SIGNONID);
					m_axis->WriteProfileInt(WORKSTATION, "HTS", 1);
					const int pos = Axis::userID.Find('|');
					if (pos >= 0)
					{
						Axis::userNM = Axis::userID.Mid(pos + 1);
						Axis::userID = Axis::userID.Left(pos);
						
						if(Axis::userID.Mid(0,3) == "!&$")
						{
							Axis::userID = Axis::userID.Mid(3);
							
							CString decID = DecryptAES(Axis::userID);
							
							Axis::userID = decID;
						}
					}
				}

				CheckEncryptDirectory();
				//FixUserConfigDirectory();

				//CheckEncryptDirectory();
				CheckSoundConfig();
				CheckNewsSetting();
				ConfigFrame();

				//히스토리 파일이 없을때(처음 설치시) 기업은행 디폴트세팅 해주게 추가
				CString file;
				file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, AXISUSER);
				
				CFileFind ff;
				
				if(!ff.FindFile(file))
				{
					file.Format("%s\\%s\\%s", Axis::home, USRDIR, Axis::user);
					::CreateDirectory(file, NULL);
					
					file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, AXISUSER);
					
					WritePrivateProfileString("System","1301","024110",file);
					WritePrivateProfileString("System","save_code","024110",file);
					WritePrivateProfileString("System","Codes","1301,save_code",file);
				}

				CProfile profile(pkPalette);
				CString pal = profile.GetString(GENERALSN, "Palette");
				
				if (!pal.CompareNoCase("Green") || !pal.CompareNoCase("Violet"))

				{
					pal = "Blue";
					profile.Write(GENERALSN, "Palette","Blue");
					//AfxMessageBox("SKIN");
				}
				
				if (m_step != axDONE)
				{
					m_step = axDONE;

					//AXISTICKER.INI에서 BAR_02 디폴트를 4(코스닥벤처)에서 3(코스닥)으로 변경
					// 2016.12.08 dkkim
// 					CProfile profile(pkAxisTicker);
// 					
// 					CString section,keys,dval;
// 					char	buf[1024*2];
// 					
// 					section.Format("BAR_INFO_%02d", 2);
// 					
// 					int nRepair = profile.GetInt(section, "REPAIR", 0);
// 					
// 					if(nRepair == 0)
// 					{
// 						profile.Write(section,"REPAIR", 1);
// 						
// 						for(int kk=0;kk<5;kk++)
// 						{
// 							keys.Format("%03d", kk);
// 							dval = profile.GetString(section, keys);
// 							
// 							strcpy(buf, dval);
// 							
// 							if (strlen(buf) < 1)
// 								break;
// 						}
// 						
// 						if(kk < 2)
// 						{
// 							dval = profile.GetString(section, "000");
// 
// 							if(dval == "4")
// 								profile.Write(section,"000",4);
// 						}
// 					}
					// 코스닥 변경 ======================================================

					GetLocalTime(&m_connectT);
					convertIndex();
					ConfigTicker();

					m_runAxis = true;

					sendRTime();
					sendTicInfo();

					if(m_bUseNewLogin)
					{
						m_axConnect->SetFrameRun();
						m_axConnect->PostMessage(WM_CLOSE);
					}
					else
					{
						m_axConnectOld->SetFrameRun();
						m_axConnectOld->PostMessage(WM_CLOSE);
					}

					//LoadSecureTools();
				}
			}
			break;
		case newVIEW:			
			return create_Newview(HIWORD(wParam), (char *)lParam);
		case renVIEW:
			if (HIWORD(wParam) & winK_POPUP)
				return m_mapHelper->ChangePopup((char *)lParam, TRIGGERN, HIWORD(wParam));
			else	
				return m_mapHelper->ChangeChild((char *)lParam, TRIGGERN, HIWORD(wParam));
			break;
		case delVIEW:
			if (HIWORD(wParam) & winK_POPUP)
			{
				CMPop* Mpop{};
				CGPop* Gpop{};
				if (m_arMPOP.Lookup(HIWORD(wParam), Mpop))
					Mpop->EndDialog(IDCANCEL);
				else if (m_arGPOP.Lookup(HIWORD(wParam), Gpop))
					Gpop->SendMessage(WM_CLOSE, 0, 0);
			}
			else
				m_mapHelper->closeChild(HIWORD(wParam));
			break;
		case htmlVIEW:	// key : HIWORD(wParam), html : lParam
			str = (char *) lParam;
			if (m_runAxis)	
				RunWebBrowser(str);
			else		
				ShellExecute(NULL, _T("open"), str, NULL,NULL, SW_SHOWNORMAL);
			break;
		case titleVIEW:	
			setTitle(HIWORD(wParam), (char *)lParam);
			break;
		case autoVIEW:	// ### : HIWORD(wParam) : cnt, items : lParam
			setCodeReg(HIWORD(wParam), (char *) lParam);
			break;
		case mapNAME:			break;
		case waitPAN:
			if (lParam)	beginWait(HIWORD(wParam));
			else		endWait(HIWORD(wParam));			
			break;
		case hangulPAN:	// key : HIWORD(wParam), hangul : lParam
		case doublePAN:	// key : HIWORD(wParam), double : lParam
			break;
		case alarmPAN:	// dialog ? : HIWORD(wParam), text : lParam
			if (HIWORD(wParam))
			{
				str = parseData((char *) lParam);

				if (str.Find("Upload") >= 0)
				{
					DumpUpload();
					break;
				}

				
				if (!str.IsEmpty())
				{
					m_alarmMsg = str;
					PostMessage(WM_AXIS, MAKEWPARAM(axALARM, 0), 0);
				}
			}
			else	displayGuide((char *) lParam);
			break;
		case noticePAN:	// text : lParam
			ConclusionNotice((char *) lParam, str);
			break;
		case userName:	sprintf((char*)lParam, Axis::user, Axis::user.GetLength());  
			break;
		case dialogPAN:	// type : HIWORD(wParam), data  : lParam
			str = (char *) lParam;
//			WriteLog("CMainFrame::OnFireRect  dialogPAN [%s] ", str);
// 			s.Format("이중접속 : [%s] [%d]\n",str,HIWORD(wParam));
// 			OutputDebugString(s);
			switch (HIWORD(wParam))
			{
			default:
			case 0x01:
				//OutputDebugString("TOP10 REAL DATA RCV\n");
				if(m_b2018 && str.GetAt(0) == '@')
				{
					str = str.Mid(1);
					//Axis::MessageBox(this,str,MB_ICONINFORMATION);
					CString strFile;
					strFile.Format("%s\\tab\\TOP10.ini", Axis::home); 
					//OutputDebugString("TOP10 REAL DATA\n");
					
					const int nUse = GetPrivateProfileInt("TOP10","USE",0,strFile);
					
					if((nUse == 2 && Axis::isCustomer) || (nUse == 1 && !Axis::isCustomer) || nUse == 3)
					{
						if(m_top10 != nullptr && m_top10->GetSafeHwnd())
							Parse2018((char*)(const char *)str,str.GetLength());
					}
				}
				break;
			case 0x02:  //서킷브레이크 관련
				{
					/*
					if(m_mngInfo)  //이구분자는 확인을 해보자 체크여부 상관없이 해야할듯...
					{
CString slog;
slog.Format("[cb] 0x02 receive %s\n" , str);
OutputDebugString(slog);
						//내려주는 메시지를 보고 구분을 하자
						//1 코스피 발동
						//2 코스닥 발동
						//3 코스피, 코스닥 발동
						//ParseRTSMessage((LPSTR)(LPCTSTR)str, str.GetLength());
					}
					*/
				}
				break;
			case 0x96:	// 긴급공지 -> 긴급 강제 다운로드로 변경(포맷은 추후에)! 2010.07.21 by LKM
				ProcessRexp( (struct rexp_mid*)lParam );
				break;
			case 0x97:	// 중복사용자..............
				m_alarmMsg = str;
				PostMessage(WM_AXIS, MAKEWPARAM(axDIALOG, HIWORD(wParam)), lParam);
				break;
			case 0x99:
WriteLog("[AXIS] CMainFrame::OnFireRect  0x99  [%s] ", str);
				if (str.GetLength()>0)
				{
					switch(str.GetAt(0))
					{
					case '$':		// 재접속 여부를 묻지 않고 종료
						m_bExit = false;
						m_forceClose = true;
						m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc, (BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
						Axis::MessageBox(this, str.Mid(1), MB_OK);
						break;
					case '!':
						{
							if(!m_bMustRetry)
							{
								return 0;
							}

							str = str.Mid(1);

							m_bExit = false;
							m_forceClose = true;
							m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc, (BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
							
							if (Axis::MessageBox(this, str, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
									m_axMisc->RunVers(verRETRY, Axis::userID, m_pass, m_cpass);
						}
						break;
					case '#':
						{
							str = str.Mid(1);
							
							if (Axis::MessageBox(this, str, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
							{
								m_bExit = false;
								m_forceClose = true;
								m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc, (BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
								m_axMisc->RunVers(verRETRY, Axis::userID, m_pass, m_cpass);
							}
							else
							{
								return 0; 
							}
						}
						break;
					default:		// 재접속 여부를 묻고, 재접속/종료 시킴
						{
							m_bExit = false;
							m_forceClose = true;
							m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc, (BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);

							if (Axis::MessageBox(this, str, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
									m_axMisc->RunVers(verRETRY, Axis::userID, m_pass, m_cpass);
						}
					}
				}

				//20170210 dkkim
				//닫기전에 화면저장
				saveExitMap();

				PostMessage(WM_CLOSE);
				break;
			case 0x98:
				{
					
					m_bExit = false;
					m_forceClose = true;
					Sleep(1000);
					m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
								(BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);

					const HWND	hWnd = m_hWnd;

					str.Replace("접속ID", "고객명");
					str.Replace(Axis::userID, Axis::userNM);

					Axis::MessageBox(this, str +
						"\n이중접속설정은 아래화면에서 설정가능합니다."
						"\n        [8211] 이중접속 설정/해지" );

					saveExitMap();
					
					PostMessage(WM_CLOSE);
				}
				break;
			}
			break;
		case sizeVIEW:	
			changeSize(HIWORD(wParam), CSize(LOWORD(lParam), HIWORD(lParam)));   
			break;
		case userINFO:	
			GetWindowText(title);
			if (title.Find("고객만족팀") == -1)
			{
				str = (m_rndkey%2) ? CALLCENTER1 : CALLCENTER2;
				title += str;
				title += CALLCENTER3;
				m_saveTitle = title;
				m_titleS = title;
			
				SetWindowText(title);
				
			}

			if (title.Find("최종접속일") == -1)
			{
				str.Format("   최종접속일 : %s", (char *) lParam);
				title += str;
				m_saveTitle = title;
				m_titleS = title;
				SetWindowText(title);
			}					
			break;
		case printVIEW:
			{
				//로그인시 플랫폼이 보내는 프린트 메세지 때문에 메인이 뻗는 현상 발생
				//m_printS =(char *) lParam;
				//AfxMessageBox("printVIEW");
				//PostMessage(WM_AXIS, MAKEWPARAM(axPRINTIMG, 0));
			}
			break;
		case initVIEW:
			{
				//AfxMessageBox("initVIEW");
				actionCaption(m_activeKey, IDX_FONTX);
			}
			break;
		case copyVIEW:
			{
				actionCaption(m_activeKey, IDX_COPY);
				//m_activeKey
			}
			break;
		case printVIEWx:
			{
				OutputDebugString("Main Revceived [PRINTVIEWX]\n");
				PostMessage(WM_AXIS, MAKEWPARAM(axPRINTIMG, 0));
			}
			break;
		case menuAXIS:		
			break;
		case linkVIEW:
			{
#ifdef DF_USE_CPLUS17
				CLink	dlg(m_tMenu.get(), (char*)lParam);
#else
				CLink	dlg(m_tMenu, (char *)lParam);
#endif
				if (dlg.DoModal() == IDOK)
				{
				}
			}
			break;
		case closeAXIS:
WriteLog("[AXIS] CMainFrame::OnFireRect  closeAXIS RunVers [%d] ",  HIWORD(wParam));
			//HIWORD(wParam) = true  일때 Reboot하여 Login Dialog 상태로..
			//HIWORD(wParam) = false 일때 Axis 종료
			if (HIWORD(wParam))
				m_axMisc->RunVers(verRETRY);
			m_bExit = false;
			saveExitMap();
			PostMessage(WM_CLOSE);
			break;
		case nameVIEW:	
			SetAMap(HIWORD(wParam), (char *) lParam);	
			break;
		case accnINFO:
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(applyACC, 0), 0);
			break;
		//** temp case menuVIEW:	SetSearchMap((char *) lParam);	break;
		case userPASS:	sprintf((char *) lParam, m_pass, m_pass.GetLength()
							);	break;
		case captionVIEW: //** HIWORD(wParam): key, lParam: string buffer
			strcpy((char*)lParam, GetChildWindowCaption(HIWORD(wParam))); 
			break;
		default :
			
				//CString tmp; tmp.Format("[%d]", LOWORD(wParam));
				//AfxMessageBox(tmp);
			break;

			
		}
		break;

	case FEV_STAT:
		{
			if(m_bUseNewLogin)
			{
				if (m_axConnect) 
				{
					m_axConnect->SetProgress((char *)lParam, (int)wParam);
					//**TRACE("FEV_STAT: %s (%d)\n", (char*)lParam, wParam);
				}
				else
				{
					m_axConnectOld->SetProgress((char*)lParam, (int)wParam);
				}
			}
		}
		break;		
	case FEV_ERROR:

WriteLog("[AXIS] CMainFrame::OnFireRect  FEV_ERROR  m_step= [%d] msg=[%s] enum	{axNONE[0], axOPEN[1], axOPENPPP[2], \
																																			 axOPENRSM[3], axOPENWSH[4], axOPENSIGN[5],\
																																			 axDONE[6], axCLOSE[7]} m_step;", m_step, (const char*)lParam);
		switch (m_step)
		{
		case axOPENRSM:
			{
				if(m_bUseNewLogin)
				{
					if (m_axConnect)
						m_axConnect->SetProgress((char*)lParam, (int)wParam, TRUE);
				}
				else
				{
					if (m_axConnectOld)
						m_axConnectOld->SetProgress((char*)lParam, (int)wParam, TRUE);
				}
			}
			break;
		case axOPENSIGN:
			{
				if(m_bUseNewLogin)
				{
					if (lParam && m_axConnect)
					{
						WriteLog("[AXIS] axOPENSIGN=======================+=======axOPENSIGN\n");
						{	// 직원 중복접속일 경우.
							CString msg((const char*)lParam);
							if (msg.GetLength() > 4 && atoi(msg.Left(4)) == 3192)
							{
								CDuplicateLoginConfirmDlg dlg(this, msg);
								dlg.DoModal();
								
								switch (dlg.GetResult())
								{
								case IDIGNORE:
									m_axConnect->m_forceLogin = 2;
									break;
								case IDOK:
									m_axConnect->m_forceLogin = 1;
									break;
								case IDCANCEL:
									m_step = axNONE;
									m_axConnect->SetStatus(SM_EXIT);
									return 0;
								}

								m_step = axNONE;
								m_axConnect->SetStatus(SM_NO);
								m_axConnect->DoRun();
								return 0;
							}

						}
						
						{
							// 2011.08.02 비밀번호 오류일 경우 앞의 시간을 없앰(오류횟수표기를위해서)
							CString emsg = (char*)lParam;
							int ecod = atoi(emsg.Left(4));
							do {
								if (ecod==4368 && Axis::isCustomer)
								{
									int pos[2]{}, nTotal{}, nRemain{};
									
									pos[0] = emsg.Find("회");
									if (pos[0]==-1) break;
									nTotal = atoi(emsg.Mid(pos[0]-1,1));

									pos[1] = emsg.Find("회", pos[0]+2);
									if (pos[1]==-1) break;
									nRemain = atoi(emsg.Mid(pos[1]-1,1));

									emsg.Format("접속비밀번호를 %d회 잘못입력하셨습니다.", nTotal-nRemain);
									// 3회 이상 오류
									if (nRemain==0)
									{
										CString mmsg;
										mmsg.Format(
											"접속비밀번호를 총%d회 잘못입력하여 사용이 정지되었습니다.\n\n"
											"홈페이지 혹은 지점에 내방하여 오류횟수를 초기화 하시기 바랍니다.", nTotal);
										m_axConnect->ShowMessageBox(mmsg, "IBK투자증권", MB_OK|MB_ICONERROR);
									}
									else if ((nTotal-nRemain)>=3)
									{
										CString mmsg;
										mmsg.Format("%s\n\n총 %d회 오류시 사용정지됩니다.", emsg, nTotal);
										m_axConnect->ShowMessageBox(mmsg, "IBK투자증권", MB_OK|MB_ICONERROR);
									}

									m_axis->WriteProfileInt(WORKSTATION, "itgy", 1);
								}
								else if (ecod==9998)
								{
									CString path;
									path.Format("%s\\%s\\NOTICE_POPUP.BMP", Axis::home, IMAGEDIR);
									CImageNotice dlg(this, path);
									dlg.DoModal();
								}
								else if (ecod==9999)
								{
									emsg = emsg.Mid(5);
									m_axConnect->ShowMessageBox(emsg, "IBK투자증권", MB_OK|MB_ICONERROR);
								}
								else if (ecod==4259)
								{
									//emsg = emsg;
									emsg = "[4259]선택하신 인증서는 타기관 인증서 등록이 필요합니다.";
								}
							} while(FALSE);
							m_axConnect->SetGuide(emsg);
						}

						if (wParam)
						{
							m_axConnect->SetChoice(true);
							m_axConnect->ClearPassword();
						}
						else
						{
							m_step = axNONE; //
							m_axConnect->SetStatus(SM_EXIT);
						}
					}
				}
				else
				{
					if (lParam && m_axConnectOld)
					{
						WriteLog("[AXIS] FEV_ERROR  axOPENSIGN\n");
						{	// 직원 중복접속일 경우.
							CString msg((const char*)lParam);
							if (msg.GetLength() > 4 && atoi(msg.Left(4)) == 3192)
							{
								CDuplicateLoginConfirmDlg dlg(this, msg);
								dlg.DoModal();
								
								switch (dlg.GetResult())
								{
								case IDIGNORE:
									m_axConnectOld->m_forceLogin = 2;
									break;
								case IDOK:
									m_axConnectOld->m_forceLogin = 1;
									break;
								case IDCANCEL:
									m_step = axNONE;
									m_axConnectOld->SetStatus(SM_EXIT);
									return 0;
								}
								
								m_step = axNONE;
								m_axConnectOld->SetStatus(SM_NO);
								m_axConnectOld->DoRun();
								return 0;
							}
							
						}
						
						{
							// 2011.08.02 비밀번호 오류일 경우 앞의 시간을 없앰(오류횟수표기를위해서)
							CString emsg = (char*)lParam;
							int ecod = atoi(emsg.Left(4));
							do {
								if (ecod==4368 && Axis::isCustomer)
								{
									int pos[2]{}, nTotal{}, nRemain{};
									
									pos[0] = emsg.Find("회");
									if (pos[0]==-1) break;
									nTotal = atoi(emsg.Mid(pos[0]-1,1));
									
									pos[1] = emsg.Find("회", pos[0]+2);
									if (pos[1]==-1) break;
									nRemain = atoi(emsg.Mid(pos[1]-1,1));
									
									emsg.Format("접속비밀번호를 %d회 잘못입력하셨습니다.", nTotal-nRemain);
									// 3회 이상 오류
									if (nRemain==0)
									{
										CString mmsg;
										mmsg.Format(
											"접속비밀번호를 총%d회 잘못입력하여 사용이 정지되었습니다.\n\n"
											"홈페이지 혹은 지점에 내방하여 오류횟수를 초기화 하시기 바랍니다.", nTotal);
										m_axConnect->ShowMessageBox(mmsg, "IBK투자증권", MB_OK|MB_ICONERROR);
									}
									else if ((nTotal-nRemain)>=3)
									{
										CString mmsg;
										mmsg.Format("%s\n\n총 %d회 오류시 사용정지됩니다.", emsg, nTotal);
										m_axConnect->ShowMessageBox(mmsg, "IBK투자증권", MB_OK|MB_ICONERROR);
									}
									
									m_axis->WriteProfileInt(WORKSTATION, "itgy", 1);
								}
								else if (ecod==9998)
								{
									CString path;
									path.Format("%s\\%s\\NOTICE_POPUP.BMP", Axis::home, IMAGEDIR);
									CImageNotice dlg(this, path);
									dlg.DoModal();
								}
								else if (ecod==9999)
								{
									emsg = emsg.Mid(5);
									m_axConnectOld->ShowMessageBox(emsg, "IBK투자증권", MB_OK|MB_ICONERROR);
								}
								else
								{
									//emsg = emsg;
								}
							} while(FALSE);
							m_axConnectOld->SetGuide(emsg);
						}
						
						if (wParam)
						{
							m_axConnectOld->SetChoice(true);
							m_axConnectOld->ClearPassword();
						}
						else
						{
							m_step = axNONE; //
							m_axConnectOld->SetStatus(SM_EXIT);
						}
					}
				}
			}
			break;
		case axOPENWSH:
			if (lParam)
			{
				m_step = axNONE;	//

				if(m_bUseNewLogin)
				{
					if (m_axConnect)	break;
					m_axConnect->SetGuide((char*)lParam);
					m_axConnect->SetStatus(SM_EXIT);
				}
				else
				{
					if (m_axConnectOld)	break;
					m_axConnectOld->SetGuide((char*)lParam);
					m_axConnectOld->SetStatus(SM_EXIT);
				}
			}
			break;
		case axNONE:
		case axDONE:
			if (lParam) 
			{
				WriteLog("[AXIS] axDONE==============================\n");
				if (LOWORD(wParam))
				{
					if (HIWORD(wParam) == '4')	// messagebox
						m_axMisc->MsgBox((char *)lParam);
					else
					{
						if(m_bUseNewLogin)
						{
							if (m_axConnect)
								m_axConnect->SetGuide((char*)lParam);
							else	displayGuide((char *) lParam);
						}
						else
						{
							if (m_axConnectOld)
								m_axConnectOld->SetGuide((char*)lParam);
							else	displayGuide((char *) lParam);
						}
					}
				}
				else
				{
					if(m_bUseNewLogin)
					{
						if (m_axConnect)
							m_axConnect->SetGuide((char*)lParam);
						else	displayGuide((char *) lParam);
					}
					else
					{
						if (m_axConnectOld)
							m_axConnectOld->SetGuide((char*)lParam);
						else	displayGuide((char *) lParam);
					}
				}
			}
			break;

		}
		break;
	case FEV_GUIDE:
		if(((int)lParam) == 57)
		{	
			COleDispatchDriver	excel;
			
			if (!excel.CreateDispatch("Excel.Application"))
			{
				str = _T("엑셀을 시작할 수 없습니다.");
			}
			else
			{
				excel.ReleaseDispatch();
			}
		}
		else
		{
			m_axMisc->GetGuide((int)lParam, str);
		}
		
		switch (m_step)
		{
		case axDONE:
			if (LOWORD(wParam))
			{
				if (HIWORD(wParam) == '4')	// messagebox
					m_axMisc->MsgBox(str);

				if (LOWORD(wParam) & 0x80)	
					displayGuide(str);
				else				
					displayGuide(str);
			}
			else
			{
				if ((int) lParam != 6)
					displayGuide(str);
			}
			break;
		case axOPENSIGN:
		case axOPENWSH:
			{
				if(m_bUseNewLogin)
				{
					if (m_axConnect)	
						m_axConnect->SetChoice(false);
				}
				else
				{
					if (m_axConnectOld)
						m_axConnectOld->SetChoice(false);
				}
			}
		default:
			{
				if (m_bUseNewLogin)
				{
					if (m_axConnect)
					{
						m_axConnect->SetChoice(true);
						if (LOWORD(wParam)) m_axConnect->SetGuide((char*)lParam);
						else m_axConnect->SetGuide(str);
					}
				}
				else
				{
					if (m_axConnectOld)
					{
						m_axConnectOld->SetChoice(true);
						if (LOWORD(wParam)) m_axConnectOld->SetGuide((char*)lParam);
						else m_axConnectOld->SetGuide(str);
					}
				}
			}
			break;
		}
		break;
	case FEV_FMX:	processFMX(wParam, lParam);	break;	
	case FEV_VERS:	processMapVersionInfo(wParam, lParam); break;
	default:	break;
	}
	return 0;
}

bool CMainFrame::CreateWizard()
{
	registerControl();
	OutputDebugString("[axis]CMainFrame::CreateWizard ");
	m_wizard = std::make_unique<CWnd>();

	if (!m_wizard->CreateControl("AxisWizard.WizardCtrl.IBK2019", NULL, WS_CHILD, CRect(0, 0, 0, 0), this, -1))
	{
		m_wizard = nullptr;
		WriteLog("[AXIS] ---------------[axis]createwizard fail---------------");
		return false;
	}
	return true;
}

int CMainFrame::Initialize()
{
	if (m_regkey.Find("직원") >= 0 && !m_axis->GetProfileInt(INFORMATION, "staff_change_flag", 0))
	{
		m_axis->WriteProfileInt(INFORMATION, "staff_change_flag", 1);
		m_axis->WriteProfileString(INFORMATION, "Server", "172.16.205.20");
	}
	CloseChaserAPP();
	CloseAgent();

	CString filename;
	filename.Format("%s\\%s\\%s", Axis::home, "tab", "axis.ini");
	WritePrivateProfileString("AXIS", "reg", m_regkey, filename);

//	m_hHook = SetWindowsHookEx(WH_GETMESSAGE, KeyboardProc, 
//			AfxGetInstanceHandle(), GetCurrentThreadId());
	m_pMain = (CMainFrame *) this;

	m_saveTitle = m_axMisc->m_regkey;
	SetWindowText(m_saveTitle);

	m_titleS = m_saveTitle;
	::SetWindowLong(m_hWndMDIClient, GWL_EXSTYLE, ::GetWindowLong(m_hWndMDIClient, GWL_EXSTYLE) & ~WS_EX_CLIENTEDGE);
	::SetWindowPos(m_hWndMDIClient, NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_FRAMECHANGED);
	m_MClient->SubclassWindow(m_hWndMDIClient);
	m_MClient->m_nBkMode = m_nBkMode;
	
	m_axGuide->Create(this);
	OutputDebugString("[axis] initialize 1");
	CString backupID(Axis::userID);
	SetUserInfo();
	if (!backupID.IsEmpty())
		Axis::userID = backupID;
	OutputDebugString("[axis] initialize 2");
#ifdef DF_USE_CPLUS17
	m_axConnectOld = std::make_unique<CDtconnect>(this, Axis::userID, IsNewLoginImage((m_regkey.Find("직원") >= 0)));
	m_axConnect = std::make_unique<CCertLogin>(this, Axis::userID);
	m_logodlg = std::make_unique<CLogoDlg>(this);
#else
	m_axConnectOld = new CDtconnect(this, Axis::userID, IsNewLoginImage((m_regkey.Find("직원") >= 0)));
	m_axConnect = new CCertLogin(this, Axis::userID);
	m_logodlg = new CLogoDlg(this);
	m_wizard = new CWnd();
#endif 
	m_logodlg->Create(NULL, "CLogoDlg", WS_CHILD | WS_BORDER | WS_OVERLAPPEDWINDOW, CRect(0, 0, 0, 0), this, -1);
	OutputDebugString("[axis] initialize 3");

#ifdef DF_USE_CPLUS17
	if (!CreateWizard())
	{
		m_wizard = nullptr;
		return -1;
	}
	//m_wizard = std::make_unique<CWnd>();
	//if (!m_wizard->CreateControl("AxisWizard.WizardCtrl.IBK2019", NULL, WS_CHILD, CRect(0, 0, 0, 0), this, -1))
	//{
	//	m_wizard = nullptr;
	//	return -1;
	//}
#else
	m_wizard = new CWnd();
	if (!m_wizard->CreateControl("AxisWizard.WizardCtrl.IBK2019", NULL, WS_CHILD, CRect(0,0,0,0), this, -1))
	{
		delete m_wizard;
		m_wizard = NULL;
		return -1;
	}
#endif

	if (m_axis->GetProfileInt(ENVIRONMENT, "KeyProtect", 0))
	{
		((CAxisApp*)m_axis)->protectKey(Axis::home, true);
	}
	if (m_axMisc->m_regkey == "IBK")
		SetWindowText("IBK개발");
	else if (m_axMisc->m_regkey == "IBK_STAFF")
		SetWindowText("[직원]IBK개발");
	else if (m_axMisc->m_regkey == "IBKMAC_STAFF")
		SetWindowText("[직원]IBK hot Trading");
	else
		SetWindowText("IBK hot Trading");

	m_iClickEPBBanner = 0;
	m_iClickCROWDBanner = 0;

	OutputDebugString("[axis] initialize 4");
	ReadAxisGlb();
	return 0;
}

void CMainFrame::CreateTB()
{	
	OutputDebugString("[axis][createTB] start\n");
	//ModifyStyle(WS_CAPTION, 0); 
#ifdef DF_USE_CPLUS17
	m_bitmapBtn = std::make_unique<CBmpButton>();
	if (!m_bitmapBtn->Create("", BS_BITMAP/*BS_OWNERDRAW*/, CRect(5, -1, 76, 57), this, IDC_BTN_LOGO))
	{
		m_bitmapBtn = nullptr;
		return;
	}

	m_bitmapBtn->SetImgBitmap(Axis::GetSkinBitmap("LOGO", true), 1);
	m_bitmapBtn->ShowWindow(SW_HIDE);

	SetMenu(NULL);

	m_sdibar = std::make_unique<CSdibar>(axiscall);
	if (!m_sdibar->Create(this, IDD_SDIBAR, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_LEFT, IDD_SDIBAR))
	{
		m_sdibar = nullptr;
		return;
	}

	m_tMenu = std::make_unique<CTMenu>(this, 22);
	if (!m_tMenu->Create(this, IDD_TMENU, WS_DLGFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_TOP, IDD_TMENU))
	{
		m_tMenu = nullptr;
		return;
	}

	m_tMenu->ModifyStyle(WS_THICKFRAME, 0);

	m_bar1 = std::make_unique<CDbar1>(axiscall);
	if (!m_bar1->Create(this, IDD_DBAR1, WS_DLGFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_TOP, IDD_DBAR1))
	{
		m_bar1 = NULL;
		return;
	}

	m_bar1->set_MenuInfo(m_tMenu.get());

	const int btn_tmp = 0;
	m_smain = std::make_unique <CSmain>(axiscall);
	if (!m_smain->Create(this, IDD_SMAIN, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_BOTTOM, IDD_SMAIN))
	{
		m_smain = NULL;
		return;
	}

	m_tInfo2 = std::make_unique <CTInfo2>(axiscall);
	if (!m_tInfo2->Create(this, IDD_INFO2, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_BOTTOM, IDD_INFO2))
	{
		m_tInfo2 = NULL;
		return;
	}

	m_tInfo1 = std::make_unique <CTInfo1>(axiscall);
	if (!m_tInfo1->Create(this, IDD_INFO1, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_BOTTOM, IDD_INFO1))
	{
		m_tInfo1 = NULL;
		return;
	}
#else
	m_bitmapBtn = new CBmpButton();
	if(!m_bitmapBtn->Create("", BS_BITMAP/*BS_OWNERDRAW*/, CRect(5,-1, 76,57), this, IDC_BTN_LOGO))
	{
		delete m_bitmapBtn;
		m_bitmapBtn = NULL;
		return;
	}

	m_bitmapBtn->SetImgBitmap(Axis::GetSkinBitmap("LOGO", true), 1);
	m_bitmapBtn->ShowWindow(SW_HIDE);

	SetMenu(NULL);

	m_sdibar = new CSdibar(axiscall);
	if (!m_sdibar->Create(this, IDD_SDIBAR, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_LEFT, IDD_SDIBAR))
	{
		delete m_sdibar;
		m_sdibar = NULL;
		return;
	}

	m_tMenu = new CTMenu(this, 22);
	if (!m_tMenu->Create(this, IDD_TMENU, WS_DLGFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_TOP, IDD_TMENU))
		//if (!m_tMenu->Create(this, IDD_TMENU, WS_DLGFRAME|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, IDD_TMENU))
	{
		delete m_tMenu;
		m_tMenu = NULL;
		return;
	}

	m_tMenu->ModifyStyle(WS_THICKFRAME, 0);

	m_bar1 = new CDbar1(axiscall);
	if (!m_bar1->Create(this, IDD_DBAR1, WS_DLGFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_TOP, IDD_DBAR1))
	{
		delete m_bar1;
		m_bar1 = NULL;
		return;
	}

	m_bar1->set_MenuInfo(m_tMenu);

	const int btn_tmp = 0;

	m_smain = new CSmain(axiscall);
	if (!m_smain->Create(this, IDD_SMAIN, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_BOTTOM, IDD_SMAIN))
	{
		delete m_smain;
		m_smain = NULL;
		return;
	}

	m_tInfo2 = new CTInfo2(axiscall);
	if (!m_tInfo2->Create(this, IDD_INFO2, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_BOTTOM, IDD_INFO2))
	{
		delete m_tInfo2;
		m_tInfo2 = NULL;
		return;
	}

	m_tInfo1 = new CTInfo1(axiscall);
	if (!m_tInfo1->Create(this, IDD_INFO1, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_BOTTOM, IDD_INFO1))
	{
		delete m_tInfo1;
		m_tInfo1 = NULL;
		return;
	}
#endif


	//DWORD dwListBarStyle = m_axis->GetProfileInt(INFORMATION, "listbar_pos", CBRS_ALIGN_BOTTOM);
	DWORD dwListBarStyle = m_axis->GetProfileInt(INFORMATION, "listbar_pos", 0);
	if (dwListBarStyle==0)
	{
		CProfile p(pkUserSetup);
		dwListBarStyle = (DWORD)p.GetDouble(INFORMATION, "listbar_pos", CBRS_ALIGN_BOTTOM);
	}
	else
	{
		m_axis->WriteProfileInt(INFORMATION, "listbar_pos", 0);
		CProfile(pkUserSetup).Write(INFORMATION, "listbar_pos", (double)dwListBarStyle);
	}

	DWORD dwListBarStyle3 = m_axis->GetProfileInt(INFORMATION, "listbar3_pos", 0);
	if (dwListBarStyle3==0)
	{
		CProfile p(pkUserSetup);
		dwListBarStyle3 = (DWORD)p.GetDouble(INFORMATION, "listbar3_pos", CBRS_ALIGN_BOTTOM);
	}
	else
	{
		m_axis->WriteProfileInt(INFORMATION, "listbar3_pos", 0);
		CProfile(pkUserSetup).Write(INFORMATION, "listbar3_pos", (double)dwListBarStyle3);
	}

#ifdef DF_USE_CPLUS17
	m_bar2 = std::make_unique < CDbar2>(axiscall);
	if (!m_bar2->Create(this, IDD_DBAR2, WS_CHILD | WS_DLGFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | dwListBarStyle, IDD_DBAR2))
	{
		m_bar2 = NULL;
		return;
	}

	m_bar3 = std::make_unique < CDbar3>(axiscall);
	if (!m_bar3->Create(this, IDD_DBAR3, WS_CHILD | WS_DLGFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | dwListBarStyle3, IDD_DBAR3))
	{
		m_bar3 = NULL;
		return;
	}

	m_tInfo3 = std::make_unique < CTInfo3>(axiscall);   
	if (!m_tInfo3->Create(this, IDD_INFO3, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_BOTTOM, IDD_INFO3))
	{
		m_tInfo3 = NULL;
		return;
	}
#else
	m_bar2 = new CDbar2(axiscall);
	if (!m_bar2->Create(this, IDD_DBAR2, WS_CHILD|WS_DLGFRAME|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|dwListBarStyle, IDD_DBAR2))
	{
		delete m_bar2;
		m_bar2 = NULL;
		return;
	}

	m_bar3 = new CDbar3(axiscall);
	if (!m_bar3->Create(this, IDD_DBAR3, WS_CHILD|WS_DLGFRAME|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|dwListBarStyle3, IDD_DBAR3))
	{
		delete m_bar3;
		m_bar3 = NULL;
		return;
	}
	
	m_tInfo3 = new CTInfo3(axiscall);
	if (!m_tInfo3->Create(this, IDD_INFO3, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_BOTTOM, IDD_INFO3))
	{
		delete m_tInfo3;
		m_tInfo3 = NULL;
		return;
	}
#endif

	if (m_tInfo3)
	{
		m_tInfo3->SetCols(m_matchToolCount);
		m_tInfo3->make_Ctrl();
	}

	m_tMenu->SetBarStyle(m_tMenu->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_tMenu->EnableDocking(CBRS_ALIGN_TOP);
	
	m_tMenu->InitMenu(IDR_MAINFRAME, ID_MENU_BASE);
	
	if (Axis::isCustomer)
	{
		
		CMenuXP*	mainM = (CMenuXP*) m_tMenu->GetPopupMenu(0);
		//고객이면 고개비밀번호 변경 메뉴 삭제

		mainM->DeleteMenu(ID_STAFF_PWD, MF_BYCOMMAND);
		

		MENUITEMINFO	info;
		memset(&info, 0, sizeof(MENUITEMINFO));
		info.cbSize = sizeof(MENUITEMINFO);
		info.fMask = MIIM_DATA | MIIM_TYPE;
		mainM->GetMenuItemInfo(mainM->GetMenuItemID(1), &info);		// 계좌설정 텍스트 변경.
		
		const CMenuXPItem *pData = (CMenuXPItem *)info.dwItemData;
		/**
		pData->m_strText = _T("계좌설정");
		**/

		/*  //20191205 고객인경우 수수료 팝업 메뉴가 보여져야 함 기존처리 주석
		mainM = (CMenuXP*) m_tMenu->GetPopupMenu(-1);

		mainM->GetMenuItemInfo(mainM->GetMenuItemID(6), &info);		// 계좌툴바 삭제.
		mainM->RemoveMenu(6, MF_BYPOSITION);
		pData = (CMenuXPItem *)info.dwItemData;
		if ((info.fType & MFT_OWNERDRAW) && pData && pData->IsMyData())
			delete pData;
		*/
		
		
		mainM = (CMenuXP*) m_tMenu->GetPopupMenu(0);
		memset(&info, 0, sizeof(MENUITEMINFO));
		info.cbSize = sizeof(MENUITEMINFO);
		info.fMask = MIIM_DATA | MIIM_TYPE;
		mainM->GetMenuItemInfo(mainM->GetMenuItemID(4), &info);
		
		CMenuXPItem *pData2 = (CMenuXPItem *)info.dwItemData;
		//AfxMessageBox(pData2->m_strText);
		
		pData2->m_strText = "조회계좌설정";
		
		//if (pData2) delete pData2;
	}

	OutputDebugString("[axis][createTB] end \n");

	makeUserScreenMenu();
	appendUserToolMenu();

	EnableDocking(CBRS_ALIGN_ANY);

	m_bar1->SetBarStyle(m_bar1->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_bar1->EnableDocking(CBRS_ALIGN_TOP);
	m_bar1->make_Ctrl();

	m_bar2->SetBarStyle(m_bar2->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_bar2->EnableDocking(CBRS_ALIGN_BOTTOM|CBRS_ALIGN_TOP);
	m_bar2->make_Ctrl();
	
	m_bar3->SetBarStyle(m_bar3->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_bar3->EnableDocking(CBRS_ALIGN_BOTTOM|CBRS_ALIGN_TOP);
	m_bar3->make_Ctrl();
	
	m_sdibar->SetBarStyle(m_sdibar->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);

	if (m_tInfo3)
		m_tInfo3->SetBarStyle(m_tInfo3->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);

	m_tInfo1->SetBarStyle(m_tInfo1->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_tInfo2->SetBarStyle(m_tInfo2->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);

	m_smain->SetBarStyle(m_smain->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_smain->make_Ctrl();

#ifdef DF_USE_CPLUS17
	m_bar0 = std::make_unique<CCoolDialogBar>(this, axiscall, m_wizard.get());
#else
	m_bar0 = new CCoolDialogBar(this, axiscall, m_wizard);
#endif
	if (!m_bar0->Create(this, IDD_DBAR0))
	{
#ifndef DF_USE_CPLUS17
		delete m_bar0;
#endif
		m_bar0 = NULL;
		return;
	}
	
	m_bar0->make_Ctrl();
	m_bar0->SetBarStyle(m_bar0->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT);

	m_bar0->EnableDocking(CBRS_ALIGN_LEFT);

#ifdef DF_USE_CPLUS17
	DockControlBar(m_bar0.get());
	ShowControlBar(m_bar0.get(), FALSE, FALSE);
#else
	DockControlBar(m_bar0);
	ShowControlBar(m_bar0, FALSE, FALSE);
#endif


#ifdef DF_USE_CPLUS17
	m_TotalAcc = new CAcntDialogBar(this, axiscall, m_wizard.get());
#else
	m_TotalAcc = new CAcntDialogBar(this, axiscall, m_wizard);
#endif	

	m_TotalAcc->SetAuthInfo(Axis::userID, m_pass);
	if (!m_TotalAcc->Create(this, IDD_TOTALACC))
	{
		//AfxMessageBox("Failed");
		delete m_TotalAcc;
		m_TotalAcc = NULL;
		return;
	}

	m_TotalAcc->SetBarStyle(m_TotalAcc->GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_FIXED|CBRS_ALIGN_RIGHT);
	m_TotalAcc->EnableDocking(CBRS_ALIGN_RIGHT);
	
	DockControlBar(m_TotalAcc);
	
	ShowControlBar(m_TotalAcc, FALSE, FALSE);
	ModifyStyle(WS_CAPTION, 0); //vc2019? test
#if 1
	m_bSDI = m_axis->GetProfileInt(INFORMATION, "SDI", 0) ? true : false;

	if (m_bSDI)
		m_appMode = MODE_SDI;
	else	
		m_appMode = MODE_MDI;
	if(m_bMiniMode)
	{
		m_bSDI = m_bMiniMode;
	}
	
	loadToolStatus();

	if (m_bSDI || m_bMiniMode)
	{
		m_toolStatus = m_axis->GetProfileInt(INFORMATION, "toolstatus", TOOLSTATUS_SDI);

		ModifyStyle(WS_CAPTION, 0);
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_tMenu.get(), FALSE, FALSE);
		ShowControlBar(m_bar1.get(), FALSE, FALSE);
		ShowControlBar(m_bar2.get(), FALSE, FALSE);
		ShowControlBar(m_bar0.get(), FALSE, FALSE);
#else
		ShowControlBar(m_tMenu, FALSE, FALSE);
		ShowControlBar(m_bar1, FALSE, FALSE);
		ShowControlBar(m_bar2, FALSE, FALSE);
		ShowControlBar(m_bar0, FALSE, FALSE);
#endif
		
		
		
		
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_sdibar.get(), TRUE, FALSE);
		ShowControlBar(m_smain.get(), TRUE, FALSE);
#else
		ShowControlBar(m_sdibar, TRUE, FALSE);
		ShowControlBar(m_smain, TRUE, FALSE);
#endif
		
		SetSDIChangeHeight();
		ChangeLogo();

		if (m_bitmapBtn) m_bitmapBtn->ShowWindow(SW_HIDE);

		if(m_bMiniMode)
			ShowWindow(SW_HIDE);
	}
	else
	{
		m_toolStatus = m_axis->GetProfileInt(INFORMATION, "toolstatus", TOOLSTATUS_MDI);
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_sdibar.get(), FALSE, FALSE);
		ShowControlBar(m_smain.get(), FALSE, FALSE);
#else
		ShowControlBar(m_sdibar, FALSE, FALSE);
		ShowControlBar(m_smain, FALSE, FALSE);
#endif
		
	}
#else
	if (!m_bSDI)
	{
		ShowControlBar(m_sdibar, FALSE, FALSE);
		ShowControlBar(m_smain, FALSE, FALSE);
	}
	loadToolStatus();
	SetToolStatus();
#endif
	RecalcLayout();
}

bool CMainFrame::Start(CString user)
{	
	OutputDebugString("[axis]CMainFrame::Start");
#ifdef USE_AHNLAB_SECUREBROWSER
	//Delete_AsisICon();

	CString filename;
	filename.Format("%s\\%s\\%s", Axis::home, "exe", "NOAOS.TXT");
	FILE* fp;
	fopen_s(&fp, filename, "rb");

	int nInternalMember;

	nInternalMember = 0;
	AfxGetApp()->WriteProfileInt(INFORMATION, "INTERNAL", 0);

	if (fp)
	{
		nInternalMember = 1;
		m_bNoProtect = TRUE;
		fclose(fp);

		m_slog.Format("[AXIS] Start  NOAOS.TXT exist");
		WriteLog(m_slog);
	}

	GetLocalIP();
	if (isIPInRange(m_ipAddr, "172.17.0.0") || isIPInRange(m_ipAddr, "172.20.0.0"))
	{
		nInternalMember = 1;
		m_bNoProtect = TRUE;
		m_bLOCALIP_172 = TRUE;

		m_slog.Format("[AXIS] Start  Intranet");
		WriteLog(m_slog);
	}
	
	if (nInternalMember == 1)
	{
		AfxGetApp()->WriteProfileInt(INFORMATION, "AOS", 0);
		AfxGetApp()->WriteProfileInt(INFORMATION, "PCFirewall", 0);
		AfxGetApp()->WriteProfileInt(INFORMATION, "INTERNAL", 1);
	}
	else
	{
		AfxGetApp()->WriteProfileInt(INFORMATION, "AOS", 1);
		AfxGetApp()->WriteProfileInt(INFORMATION, "PCFirewall", 1);
		AfxGetApp()->WriteProfileInt(ENVIRONMENT, "KeyProtect", 1);
	}
#endif

	GetLocalIP();
	
	CString s;
	s.Format("PASSWORD START [%s]\n",user);
	OutputDebugString(s);
	Check_XECUREPATH();
	if (!user.IsEmpty())
	{
#ifdef DF_ENCUSER
		int	pos = user.Find('\t');

		char	buf[512];
		CString spath{};
		spath.Format("%s\\%s\\axisENC.ini", Axis::home, "tab");
		DWORD dw = GetPrivateProfileString("RETRY", "retry", "0", buf, sizeof(buf), spath);
		CString sRetry{};
		sRetry.Format("%s", buf);
		sRetry.TrimRight();

		m_slog.Format("[CX_SecureDataEngine]  시작!!! sRetry =[%s] pos=[%d] user = [%s] Axis::userID=[%s] m_pass = [%s] m_cpass = [%s]", sRetry, pos, user, Axis::userID, m_pass, m_cpass);
		OutputDebugString(m_slog);

#ifdef DF_AUTOLOGV2
		if (pos >= 2 && sRetry == "1")
#else	
		if (pos >= 2)
#endif
		{
			CString sDEC{}, sENC;
			sENC = user.Left(pos++);
			sENC.TrimRight();

#ifdef DF_WEAKPOINT
			CString skey, stmp;
			skey = AfxGetApp()->GetProfileString(WORKSTATION, "UPDATE");

			m_slog.Format("[CX_SecureDataEngine][dec] 시작!!!  skey=[%s]  pos =[%d] user=[%s] user len = [%d]", skey, pos, user, user.GetLength());
			OutputDebugString(m_slog);

			m_slog.Format("[CX_SecureDataEngine][dec] 1.sENC =[%s]", sENC);
			//OutputDebugString(m_slog);
			Axis::userID.Format("%s", ibks_decrypt(sENC.GetString(), skey.GetString()).c_str());

			m_slog.Format("[CX_SecureDataEngine][dec] 2. Axis::userID =[%s]", Axis::userID);
			//OutputDebugString(m_slog);
#else
			if (AxStd::axDECAES((LPSTR)(LPCTSTR)sENC, sDEC))
				Axis::userID = sDEC;
#endif

			user = user.Mid(pos);
			pos = user.Find('#');

			if (pos == -1)
			{
#ifdef DF_WEAKPOINT
				m_pass.Format("%s", ibks_decrypt(user.GetString(), skey.GetString()).c_str());
#else
				if (AxStd::axDECAES((LPSTR)(LPCTSTR)user, sDEC))
					m_pass = sDEC;
#endif
			}
			else
			{
				sENC = user.Left(pos++);
				sENC.TrimRight();

#ifdef DF_WEAKPOINT
				m_pass.Format("%s", ibks_decrypt(sENC.GetString(), skey.GetString()).c_str());
#else
				if (AxStd::axDECAES((LPSTR)(LPCTSTR)sENC, sDEC))
					m_pass = sDEC;
#endif

				sENC = user.Mid(pos);
				sENC.TrimRight();
#ifdef DF_WEAKPOINT
				m_slog.Format("[CX_SecureDataEngine][dec] 4.sENC =[%s]  pos=[%d]", sENC, pos);
				//OutputDebugString(m_slog);

				m_cpass.Format("%s", ibks_decrypt(sENC.GetString(), skey.GetString()).c_str());

				s.Format("[dec][%s][%s]<%d>  user =[%s]         m_pass=[%s]          m_cpass=[%s]", DF_LOGKEY, __FUNCTION__, __LINE__, Axis::userID, m_pass, m_cpass);
				//OutputDebugString(s);
				s.Format("[%s]", DF_LOGKEY);
				//OutputDebugString(s);
#else
				if (AxStd::axDECAES((LPSTR)(LPCTSTR)sENC, sDEC))
					m_cpass = sDEC;
#endif
			}
		}
		else
		{
#ifdef DF_AUTOLOGV2
			if (pos > 0)
			{
				Axis::userID = user.Left(pos++);

				user = user.Mid(pos);
				pos = user.Find('#');

				if (pos == -1)
				{
					m_pass = user;
				}
				else
				{
					m_pass = user.Left(pos++);
					m_cpass = user.Mid(pos);
				}

				m_slog.Format("[CX_SecureDataEngine][nodec] 2.시작!!! user = [%s] Axis::userID=[%s] m_pass = [%s] m_cpass = [%s]",  user, Axis::userID, m_pass, m_cpass);
				OutputDebugString(m_slog);
			}
#endif		
		}
#else
		int	pos  = user.Find('\t');

		if (pos >= 0)
		{
			Axis::userID = user.Left(pos++);
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
#endif
		Axis::userID.TrimLeft(); 
		Axis::userID.TrimRight();
		m_pass.TrimLeft(); 
		m_pass.TrimRight();
		m_cpass.TrimLeft(); 
		m_cpass.TrimRight();

		AfxGetApp()->WriteProfileString(WORKSTATION, "UPDATE", "");

		memset(buf, 512, 0x00);
		spath.Format("%s\\%s\\AXISAI.ini", Axis::home, "tab");
		dw = GetPrivateProfileString("MAINRTS", "use", "0", buf, sizeof(buf), spath);
		s.Format("%s", buf);
		s.TrimRight();
		m_bMainRTS = (_ttoi(s) == 1) ? TRUE : FALSE;

		memset(buf, 512, 0x00);
		spath.Format("%s\\%s\\AXISAI.ini", Axis::home, "tab");
		dw = GetPrivateProfileString("AxisAgent", "use", "0", buf, sizeof(buf), spath);
		s.Format("%s", buf);
		s.TrimRight();
		m_bAxisAgent = (_ttoi(s) == 1) ? TRUE : FALSE;

		memset(buf, 512, 0x00);
		spath.Format("%s\\%s\\AXISAI.ini", Axis::home, "tab");
		dw = GetPrivateProfileString("AxisAgent", "show", "0", buf, sizeof(buf), spath);
		s.Format("%s", buf);
		s.TrimRight();
		m_bShowAxisAgent = (_ttoi(s) == 1) ? TRUE : FALSE;
		
	
		if(m_bUseNewLogin)
		{
#ifdef DF_ENCUSER
			CString file, usnm = Axis::user;
			char	buf[512];
			file.Format("%s\\%s\\axisENC.ini", Axis::home, "tab");
			DWORD dw = GetPrivateProfileString("RETRY", "retry", "0", buf, sizeof(buf), file);
			CString stemp{};
			stemp.Format("%s", buf);
			stemp.TrimRight();

			s.Format("[%s][%s]<%d>  retry =[%s]   nInternalMember =[%d]    file=[%s]", DF_LOGKEY, __FUNCTION__, __LINE__, stemp, nInternalMember, file);
			OutputDebugString(s);

			if (stemp == "1")
			{
				WritePrivateProfileString("RETRY", "retry", "0", file);
				if (!Axis::userID.IsEmpty())
					m_axConnect->SetUserID(Axis::userID);
				if (!m_pass.IsEmpty())
					m_axConnect->SetPassword(m_pass);  //여기타면서 자동로그인 활성화
				if (!m_cpass.IsEmpty())
					m_axConnect->SetCPass(m_cpass);
			}
			else
			{
#ifdef DF_AUTOLOGV2
					if (!Axis::userID.IsEmpty())
						m_axConnect->SetUserID(Axis::userID);
					if (!m_pass.IsEmpty())
						m_axConnect->SetPassword(m_pass);  //여기타면서 자동로그인 활성화
					if (!m_cpass.IsEmpty())
						m_axConnect->SetCPass(m_cpass);
#else	
					s.Format("[%s][%s]<%d>  비번,  공동비번 모두 지움   ", DF_LOGKEY, __FUNCTION__, __LINE__);
					OutputDebugString(s);
					Axis::userID = "";
					m_pass.Empty();
					m_cpass.Empty();
#endif				
			}

#else
			if (!Axis::userID.IsEmpty())	
				m_axConnect->SetUserID(Axis::userID);
			if (!m_pass.IsEmpty())	
				m_axConnect->SetPassword(m_pass);
			if (!m_cpass.IsEmpty())	
				m_axConnect->SetCPass(m_cpass);
#endif
		}
		else
		{
			if (!Axis::userID.IsEmpty())	
				m_axConnectOld->SetUserID(Axis::userID);
			if (!m_pass.IsEmpty())	
				m_axConnectOld->SetPassword(m_pass);
			if (!m_cpass.IsEmpty())	
				m_axConnectOld->SetCPass(m_cpass);
		}
	}

	OutputDebugString("MAIN START 1\n");
	//2015.10.10 dkkim
	//보안프로그램 구동 방법 변경
	//LoadSecureTools();
	OutputDebugString("MAIN START 2\n");

	if(m_bUseNewLogin)
	{
		m_axConnect->SetProgK(m_progK);
		if (m_regkey.Find("직원") >= 0)
			m_axConnect->SetStaff(true);

		if (m_regkey.Find("STAFF") >= 0)
		{
			m_axConnect->SetStaff(true);
			Axis::isCustomer = false;	
		}else
		{
			Axis::isCustomer = true;   
		}
  
		if (m_axConnect)
		{
			// 2013.01.14 김덕기
			// 윈도우8일 경우 설정 안내
			OSVERSIONINFOEX osvi;
			osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOA);
			//GetVersionExA((LPOSVERSIONINFO)&osvi);
			GetVersion(&osvi);
			
			CString osv;
			osv.Format("MAJOR [%d] MINOR [%d]\n",osvi.dwMajorVersion,osvi.dwMinorVersion);
			OutputDebugString(osv);
			
	// 		if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
	// 		{
	// 			char	wb[512];
	// 			CString	file, tmps;
	// 			
	// 			tmps = Axis::user;
	// 			file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, tmps, tmps);
	// 			
	// 			DWORD dwRc = GetPrivateProfileString("WIN8", "NOTICE", "NO", wb, sizeof(wb), file);
	// 			
	// 			CString strNote(wb);
	// 			
	// 			osv.Format("OS NOTICE [%s]\n",strNote);
	// 			OutputDebugString(osv);
	// 			
	// 			if(strNote.Find("NO") >= 0)
	// 			{
	// 				CString	str;
	// 				str.Format("[PC 운영체제 WINDOWS8 이상 이용 안내]"\
	// 					"\n\n"\
	// 					"■ WINDOWS8이상에서의 한글/영어 전환기능의 제약과 관련하여 ID 입력이\n 되지 않는 오류가 발생되어 다음과 같은 기능변경이 필요하오니\n 참고 바랍니다."\
	// 					"\n\n"\
	// 					"■ 경 로 : 제어판-시계,언어 및 국가별 옵션-언어-고급설정 열기"\
	// 					"\n\n"\
	// 					"■ 변경 방법 : 고급 설정 화면에서 입력 방법 전환 아래에 있는 \n [각 앱 창에 다른 입력 방법을 직접 설정] 을 체크."\
	// 					"\n\n");
	// 				
	// 				Axis::MessageBox(this, str, MB_OK | MB_ICONINFORMATION);
	// 				
	// 				WritePrivateProfileString("WIN8","NOTICE","YES",file);
	// 			}
	// 		}
			//2012.02.06 김덕기 - 로그인창 뜰때부터 태스크바에 표시하기 위해서
	// 		int screenX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	// 		int screenY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	// 		int x  = m_axis->GetProfileInt(INFORMATION, "win_x", 0);
	// 		int y  = m_axis->GetProfileInt(INFORMATION, "win_y", 0);
	// 		int cx = m_axis->GetProfileInt(INFORMATION, "win_cx", 0);
	// 		int cy = m_axis->GetProfileInt(INFORMATION, "win_cy", 0);
	// 
	// 		SetWindowPos(&wndTop, x, y, cx, cy, SWP_HIDEWINDOW);

			UINT modalResult = IDCANCEL;

			try
			{
				modalResult = m_axConnect->DoModal();
			}
			catch (...) {
				//OutputDebugString(CDebug::Get());
				CDebug::Out("Exception on Connect Dialog");
				CString file;
				file.Format("%s\\tab\\CERT.INI", Axis::home);
				
// 				CString sdat;
// 				CTime	time;
// 				time = time.GetCurrentTime();
// 				sdat.Format("%04d%02d%02d %02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(),  time.GetMinute());
// 				
// 				WritePrivateProfileString("CERTLOGIN","USE","0",file);
// 				WritePrivateProfileString("CERTLOGIN","DATE",sdat,file);
			}

			if (modalResult == IDCANCEL)
			{
				//**m_mapHelper->DirectClose();
				return false;
			}
		}
	}
	else
	{
		m_axConnectOld->SetProgK(m_progK);
		if (m_regkey.Find("직원") >= 0)
			m_axConnectOld->SetStaff(true);

		if (m_regkey.Find("STAFF") >= 0)
		{
			m_axConnectOld->SetStaff(true);
			Axis::isCustomer = false;	
		}else
		{
			Axis::isCustomer = true;  
		}

		if (m_axConnectOld)
		{
			// 2013.01.14 김덕기
			// 윈도우8일 경우 설정 안내
			OSVERSIONINFOEX osvi;
			osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOA);
			//GetVersionExA((LPOSVERSIONINFO)&osvi);
			GetVersion(&osvi);
			
			CString osv;
			osv.Format("MAJOR [%d] MINOR [%d]\n",osvi.dwMajorVersion,osvi.dwMinorVersion);
			OutputDebugString(osv);
			
			UINT modalResult = IDCANCEL;

			try
			{
				modalResult = m_axConnectOld->DoModal();
			}
			catch (...) {
				CDebug::Out("Exception on Connect Dialog");

			}

			if (modalResult == IDCANCEL)
			{
				//**m_mapHelper->DirectClose();
				return false;
			}
		}
	}
	return true;
}

void CMainFrame::UnregisterControl()
{
	return;

	char*	control[] = { "axWizard.ocx", "axSock.ocx", "axComCtl.ocx", "axXecure.ocx", "axCertify.ocx", NULL };
	CString	path;
	HINSTANCE hLib{};

	for (int ii = 0; control[ii] != NULL; ii++)
	{
		path.Format("%s\\%s\\%s", Axis::home, RUNDIR, control[ii]);
		hLib = LoadLibrary(path);
		if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		{
			TRACE("LoadLibrary error....[%s] error=[%d]\n", path, GetLastError());
			continue;
		}

		FARPROC	lpDllEntryPoint;
		(FARPROC &)lpDllEntryPoint = GetProcAddress(hLib, _T("DllUnRegisterServer"));
		if (lpDllEntryPoint == NULL)
		{
			FreeLibrary(hLib);
			continue;
		}

		(*lpDllEntryPoint)();
		FreeLibrary(hLib);
	}
}
#pragma warning (disable : 26430)
void CMainFrame::registerControl()
{
	UnregisterControl();

	//char*	control[] = { "axWizard.ocx", "axSock.ocx", "axComCtl.ocx", "axXecure.ocx", "axCertify.ocx", "vbscript.dll", "IBKSConnector.ocx", NULL };
	char*	control[] = { "axWizard.ocx", "axSock.ocx", "axComCtl.ocx", "axXecure.ocx", "axCertify.ocx", NULL };

	CString	path;
	HINSTANCE hLib{};

	for (int ii = 0; control[ii] != NULL; ii++)
	{
		path.Format("%s\\%s\\%s", Axis::home, RUNDIR, control[ii]);

		if (control[ii]=="vbscript.dll")
			path = control[ii];

		char buf1[256];
		char buf2[256];
		GetSystemDirectory(buf1, 256);
		GetCurrentDirectory(256, buf2);
		m_slog.Format("[axis][registerControl] registerControl....[%s] [%s]\n", buf1, buf2);


		hLib = LoadLibrary(path);
		if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		{
			TRACE("LoadLibrary error....[%s] error=[%d]\n", path, GetLastError());
			m_slog.Format("[AXIS] [registerControl] LoadLibrary error....[%s] error=[%d]\n", path, GetLastError());
			OutputDebugString(m_slog);
			WriteLog(m_slog);
			continue;
		}

		FARPROC	lpDllEntryPoint;
		(FARPROC &)lpDllEntryPoint = GetProcAddress(hLib, _T("DllRegisterServer"));

		if (lpDllEntryPoint == nullptr)
		{
			FreeLibrary(hLib);
			continue;
		}

		(*lpDllEntryPoint)();
		FreeLibrary(hLib);

		m_slog.Format("[AXIS] [registerControl] LoadLibrary%s] \n", path);
		OutputDebugString(m_slog);
		WriteLog(m_slog);
	}
}
#pragma warning (default : 26430)

BOOL CMainFrame::getConnectInfo(CString& ips, int& port)
{
	CString slog;
	CString Port = m_axis->GetProfileString(INFORMATION, "Port");
	//** macho temp   ips = m_axis->GetProfileString(INFORMATION, "Server");

	char	wb[128], macaddr[20], ip[16];
	memset(macaddr, 0x00, sizeof(macaddr));
	memset(ip, 0x00, sizeof(ip));
	
	GetMacAddr(wb, macaddr);
	if (m_ipAddr.IsEmpty())
		GetLocalIP();
	memcpy(ip, m_ipAddr, m_ipAddr.GetLength());

	CString ss;

	if (((CAxisApp*)m_axis)->m_forceIP.IsEmpty() == FALSE)
	{
		ips = ((CAxisApp*)m_axis)->m_forceIP;
		port = ((CAxisApp*)m_axis)->m_forcePort;

		m_forceIP = ips;
		CheckServer(ips);

		if (Axis::isCustomer && port == portEmployee)
			port = portCustomer;
		if (!Axis::isCustomer && ((port == portCustomer)||(port ==portProxy)))
			port = portEmployee;
		
		if(m_bUseNewLogin)
		{
			if (m_axConnect)
			{
				CString shashID{};
				shashID.Format("%08u", HashDataAXIS(Axis::userID.Left(5)));

				if(shashID.Find("42882716") == -1)
				{
					shashID.Empty();
					shashID.Format("%08u", HashDataAXIS(Axis::userID));

					if(shashID.Find("3729776228") == -1)
					{
						if (m_axConnect->IsNumber(Axis::userID))
							port = portEmployee;
						else if ((port != portCustomer) && (port != portProxy))
							port = portCustomer;
					}
				}
			}
			//modi 강제서버 IP 설정하면 무조건 15101,15201 이었는데 이제 설정 따라가게 직원은 그대로 항상15101
			if (Axis::isCustomer)
				port = atoi(m_axis->GetProfileString(INFORMATION, "Port"));
ss.Format("[AXIS] GLB getConnectInfo [%s] FORCE IP=[%s]  port=[%d]\n", Axis::isCustomer==TRUE?"customer":"staff",((CAxisApp*)m_axis)->m_forceIP, port);
WriteLog(ss);
		}
		else
		{
			if (m_axConnectOld)
			{
				CString shashID{};
				shashID.Format("%08u", HashDataAXIS(Axis::userID.Left(5)));

				if (shashID.Find("42882716") == -1)
				{
					shashID.Empty();
					shashID.Format("%08u", HashDataAXIS(Axis::userID));
					if (shashID.Find("3729776228") == -1)
					{
						if (m_axConnectOld->IsNumber(Axis::userID))
							port = portEmployee;
						else if ((port != portCustomer) && (port != portProxy))
							port = portCustomer;
					}
				}
			}
		}
	}
	else
	{
		if (Port.IsEmpty())	
			Port.Format("%d", portEmployee);

		port = atoi(Port);	

		if (Axis::isCustomer && port == portEmployee)
			port = portCustomer;

		if (!Axis::isCustomer)// && ((port == portCustomer)||(port ==portProxy)))
			port = portEmployee;

		m_axis->WriteProfileInt(WORKSTATION, "cust", Axis::isCustomer ? 1: 0);

slog.Format("[AXIS] GLB getConnectInfo m_iGlbIndex = %d  port=[%d]", m_iGlbIndex , port);
OutputDebugString(slog);

		if(m_iGlbIndex == 0)
			ips = get_glb_addr(ip, (LPSTR)(LPCTSTR)m_ipAddr);  
		else
			ips = get_glb_addr_Index(ip, (LPSTR)(LPCTSTR)m_ipAddr);

CString str;
str.Format("[AXIS] GLB getConnectInfo(..)  GLB로 받은IP [%s]  m_ipAddr = [%s] port=[%d]",ips,  m_ipAddr, port);
WriteLog(str);

		//**m_axis->WriteProfileString(INFORMATION, "Server", ips);	// 변경되면 서버저장

		if (ips.IsEmpty())	
		{
			OutputDebugString("GLB glb moudule fail");
			WriteLog("[AXIS] GLB glb moudule fail");
			return FALSE;
		}

		
	}
	
	return TRUE;
}

void CMainFrame::load_start_notice( BOOL bReload )
{
	CString file;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);

	const char* noticeMapName{};

	int nStartScreen =  GetPrivateProfileInt("SCREEN", "STARTSCREEN", 1, file );

	nStartScreen = 0;

	if( nStartScreen )
	{
 		closeMapByName("IB7700");	
 
		CProfile profile(pkPalette);
		CString pal = profile.GetString(GENERALSN, "Palette");

		if (!pal.CompareNoCase("Green") || !pal.CompareNoCase("Violet"))
		{
			pal = "Blue";
			profile.Write(GENERALSN, "Palette","Blue");
		}
		
		pal.MakeUpper();
		if (pal == "BLUE")
			noticeMapName = "IB770050";
		else if (pal == "GREEN")
			noticeMapName = "IB770060";
		else if (pal == "BROWN")
			noticeMapName = "IB770070";
		else if (pal == "VIOLET")
			noticeMapName = "IB770080";
		else if (pal == "GRAY")
			noticeMapName = "IB770090";
		else
			noticeMapName = "IB770050";
		if (!IsExistMap(noticeMapName) || bReload )
		{
			CProfile p(pkUserConfig);
			CString date(p.GetString(noticeMapName, "DATE", "20080601"));
			//if (atoi(date) < atoi(CTime::GetCurrentTime().Format("%Y%m%d")))
				m_mapHelper->ChangeChild(noticeMapName, 1, 0, CenterPOS);
		}

		
		const CWnd* wnd = FindWindow(NULL,"실시간해외지수");

		if (!wnd)
		{
			noticeMapName = "IB780100";

			CProfile pout(pkUserConfig);

			const int piout = pout.GetInt(noticeMapName, "OnLoad", 0);

			if (piout > 0)
			{
				m_mapHelper->ChangeChild(noticeMapName, 1, 0, CenterPOS);
			}
		}
	}	
}

void CMainFrame::closeMapByName(CString strName)
{
	int key{};
	POSITION pos{};

	CChildFrame* child{};

	for( int i = 0; i < 6; i++ )
	{
		pos = m_arMDI[i].GetStartPosition();
		while( pos )
		{
			m_arMDI[i].GetNextAssoc( pos, key, child );
			if( child->m_mapN.Find("IB7700") >= 0 )
			{
				m_mapHelper->closeChild( key );
				break;
			}
		}
	}
}

void CMainFrame::endWorkstation()
{
	ProcessFileManager("FILEMANAGER.INI");
	AccEncrypt();
	Sendpibojggb();
#ifdef DF_CDDUSE
	CheckCDDEDD();   //test CDD
#endif
#ifdef DF_MAIN_RTS
	//SetTimer(TM_MAIN_RTS_PUSH, 100, nullptr);
#endif
	initShared();
	SetPCData();
	WriteLog("[AXIS] endWorkstation - Step 1");
	m_dept = Variant(getDEPT);
	m_bar1->setDept(m_dept);
	//CString str = Variant(accountCC);

// 	CString s;
// 	s.Format("HTS DEPT [%s]\n",str);
// 	OutputDebugString(s);
	CString	Path;
	Path.Format("%s\\%s\\ACCNTDEPT.INI", Axis::home, "tab");
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("ACCNTDEPT","DEPT","811",readB,sizeof(readB),Path);

	CString tDept(readB,readL);
	tDept.TrimLeft();tDept.TrimRight();

CString s;
s.Format("[AXIS] ACCTEST Main endWorkstation 로그인부서=[%s] 파일부서=[%s] \n",m_dept, tDept);
OutputDebugString(s);
WriteLog(s);
///	if ( (m_dept != "813") && (m_dept != "828") && m_dept != "812" && m_dept != tDept)
	if(m_dept != tDept)  
	{
		CMenuXP*	mainM = (CMenuXP*) m_tMenu->GetPopupMenu(0);
		//고객이면 고개비밀번호 변경 메뉴 삭제
		mainM->DeleteMenu(ID_SETACCOUNT, MF_BYCOMMAND); 

// 		if(!Axis::isCustomer)
// 			ShowControlBar(m_TotalAcc, TRUE, FALSE);
	}
	else
	{
		// 811 법인영업팀
		// 813, 828 파생상품영업 1/2팀
// 		if (m_dept != "812")
// 			ShowControlBar(m_TotalAcc, TRUE, FALSE);

		/*if(!Axis::isCustomer)*/
		//if (m_dept == "813" || m_dept == tDept)
		if (m_dept == tDept)
		{
			//ReadManageMapInfo();
			//if (ScreenCheck("GROUPACCSAVE"))
			//	ShowControlBar(m_TotalAcc, TRUE, FALSE);
		}
	}
	
	Axis::SetSkin(GetSkinName());

	WriteLog("[AXIS] endWorkstation - Step 2");

	ChangeLogo();

	WriteLog("[AXIS] endWorkstation - Step 3");

	m_bOnlySise = m_axis->GetProfileInt(WORKSTATION, "OnlySise", 0);
	deleteNewsfile();
	m_hHook = SetWindowsHookEx(WH_GETMESSAGE, KeyboardProc, 
			AfxGetInstanceHandle(), GetCurrentThreadId());
	m_resourceHelper->ChangeRES(Axis::skinName);
	ResourceHelper()->ChangeRES(Axis::skinName);

	WriteLog("[AXIS] endWorkstation - Step 4");

	m_axMisc->LoadGuide();
#ifdef DF_USE_CPLUS17
	m_bar0->set_MenuInfo(m_tMenu.get(), Axis::user);
	m_smain->set_MenuInfo(m_tMenu.get());
#else
	m_bar0->set_MenuInfo(m_tMenu, Axis::user);
	m_smain->set_MenuInfo(m_tMenu);
#endif
	m_bar1->Change_Skin(Axis::skinName);
	m_bar2->Change_Skin(Axis::skinName);
//	m_bar3->Change_Skin(Axis::skinName);
	m_smain->Change_Skin(Axis::skinName);

	WriteLog("[AXIS] endWorkstation - Step 5");

	if (!m_bExit)	
		return;

	WriteLog("[AXIS] endWorkstation - Step 6");
	
	preload_screen();

	CString sfile, tmps, usnm = Axis::user;
	sfile.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm);
	tmps.Format("%d", (int)this->m_hWnd);
	WritePrivateProfileString("MODE", "main", tmps, sfile);

	// 직원의 패스워드 변경작업
	// axlogon -> misf 에 SBPLI301의 [로그인비밀번호구분] 플래그([W]arning, e[X]pired)
	// IB0000AA 에서 misf플래그를 ini파일에 저장
	// 메인에서 해당 플래그를 읽어 아래와 같이 처리한다.
	if (!Axis::isCustomer)   
	{
		CProfile pk(pkUserSetup);
		CString val = pk.GetString("LOGIN", "MISF", "0");
		if (val=="X" || val=="W")  
		{
			CEmpPassChangeNotifyDlg dlg(this, (val=="X") ? PNT_MUST : PNT_ADVISE);
			if (dlg.DoModal()==IDOK)
			{
				// 만료되었을경우 닫을수 없도록 한다.
#ifdef DF_USE_CPLUS17
				m_EmpPassChangeDlg = std::make_unique<CEmpPassChangeDlg>(this, (val == "X") ? FALSE : TRUE);
				m_EmpPassChangeDlg->DoModal();
				m_EmpPassChangeDlg = nullptr;
#else
				m_EmpPassChangeDlg = new CEmpPassChangeDlg(this, (val=="X") ? FALSE : TRUE);
				m_EmpPassChangeDlg->DoModal();
				delete m_EmpPassChangeDlg;
				m_EmpPassChangeDlg = NULL;
#endif
			}
		}
	}

	//맵배포시 레파지토리 리셋을 위한 부분 추가
	//dkkim 2016.03.21
	//[MAP]
	//COUNT=1
	//01=IB000011
	CString file,repo, key, dat;
	
	file.Format("%s\\tab\\Repoclear", Axis::home);
	repo.Format("%s\\tab\\Repository",Axis::home);

	if(IsFileExist(file))
	{
		char buff[1024]{};
		const int count = GetPrivateProfileInt("MAP","COUNT",0,file);

		CString strNm;

		for(int i=0;i<count;i++)
		{
			strNm.Format("%02d",i);

			GetPrivateProfileString("MAP",strNm,"",buff,sizeof(buff),file);

			CString strMap;
			strMap = buff;

			WritePrivateProfileSection(strMap,"",repo);
		}
		
		::DeleteFile(file);
	}
	/////////////////////////////////////////////////////////

	WriteLog("[AXIS] endWorkstation - Step 7");
	load_tabview();

	WriteLog("[AXIS] endWorkstation - Step 8");
	load_history();
	
	WriteLog("[AXIS] endWorkstation - Step 9");
	if (! m_bdnInterest)
	{	
		WriteLog("[AXIS] endWorkstation - Step 10-1");
		load_eninfomation();

		WriteLog("[AXIS] endWorkstation - Step 10-2");

		//** 신용정보제공 동의 여부 점검 화면

		const char* trust = "IB820850";
		if (!IsExistMap(trust))
		{
			//CString date(p.GetString(noticeMapName, "DATE", "20080601"));
			//if (atoi(date) < atoi(CTime::GetCurrentTime().Format("%Y%m%d")))
			//m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
			load_hidescreen(trust);
		}
		
		
		WriteLog("[AXIS] endWorkstation - Step 10-3");
		
		//** 초기공지사항 OPEN	
		load_start_notice();

		WriteLog("[AXIS] endWorkstation - Step 10-4");
	}

// 	const char* trust = "IB0000X8";
// 	if (!IsExistMap(trust))
// 	{
// 		//CString date(p.GetString(noticeMapName, "DATE", "20080601"));
// 		//if (atoi(date) < atoi(CTime::GetCurrentTime().Format("%Y%m%d")))
// 		m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
// 	}

	load_mngSetup();

	WriteLog("[AXIS] endWorkstation - Step 11");

	load_hkey();

	WriteLog("[AXIS] endWorkstation - Step 12");
	
	SetConclusion();

	WriteLog("[AXIS] endWorkstation - Step 13");

	m_smcall = new CSmcall();

#ifdef DF_USE_CPLUS17
	m_smcall->Set_Infomation(axiscall, m_tMenu.get());
#else
	m_smcall->Set_Infomation(axiscall, m_tMenu);
#endif

	WriteLog("[AXIS] endWorkstation - Step 14");

	if (m_Nclock)
	{
		CRect fullRc = GetFullRect(), clockRc;
		int	x = m_axis->GetProfileInt(INFORMATION, "clock_x", 0);
		int	y = m_axis->GetProfileInt(INFORMATION, "clock_y", 0);

		m_Nclock->GetClientRect(&clockRc);
		clockRc.OffsetRect(x, y);

		clockRc.IntersectRect(clockRc, fullRc);
		if (clockRc.IsRectEmpty())
		{
			x = y = 0;
		}

		if (m_axis->GetProfileInt(INFORMATION, "clock_visible", 0))
			m_Nclock->SetWindowPos(&wndTopMost, x, y, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOACTIVATE);
		else	
			m_Nclock->SetWindowPos(&wndTopMost, x, y, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE);

		if (m_axis->GetProfileInt(INFORMATION, "clock_mini", 0))
			m_Nclock->SetMini(TRUE);
	}

	WriteLog("[AXIS] endWorkstation - Step 15");
#ifdef NDEBUG
	PostMessage(WM_ELOG);
	if (!m_bCustomer && m_accTool)
		m_accTool->ShowWindow(SW_SHOW);
#endif

	SetForegroundWindow();

	WriteLog("[AXIS] endWorkstation - Step 16");

	CChildFrame*	child = NULL;
	child = load_hidescreen(MAPN_SISECATCH1);
	if(child != nullptr)
		m_arHide.Add(child);

	WriteLog("[AXIS] endWorkstation - Step 17");

	m_winVer = m_cpu.GetPlatform();	

	m_bar2->ShowInformation();

	WriteLog("[AXIS] endWorkstation - Step 18");	

	if (! m_bdnInterest)
	{	
		const char* trust = "IB770000";
		m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
	}	


	WriteLog("[AXIS] endWorkstation - Step 19");

	CFirstJob fj;
	
	ProcessInitMap();	

	WriteLog("[AXIS] endWorkstation - Step 20");

	//CreateHistoryBar();

	//업데이트 에이전트 관련
// 	CString strUpdatePath;
// 	strUpdatePath.Format("%s\\exe\\UPDATEAGENT.EXE",Axis::home);
// 	
// 	ShellExecute(NULL, _T("open"), strUpdatePath, (LPTSTR)(LPCTSTR)_T(""), NULL, SW_SHOW); 
	ReadNoticeMap();
	
	//axFocus(m_activeKey);
	/// 기능 추가 부분/////
	// dkkim 2016.04.11
	//SendProcessList();//프로세스 리스트 전송

	CString strFile;
	strFile.Format("%s\\tab\\EXECSCREEN.ini", Axis::home); 
	
	const int conf = GetPrivateProfileInt("USER","ENC",0,strFile);

	if(conf != 1)
	{
		IsSuperUser();
	}

	ScrapInformation();
	///////////////////////
	SetTimer(TM_INITSIZE, 1000, NULL);

	strFile.Format("%s\\tab\\TOP10.ini", Axis::home); 
	
	const int nInterval = GetPrivateProfileInt("TOP10","SENDTIME",60000,strFile);

	const int nUse = GetPrivateProfileInt("TOP10","USE",0,strFile);

	const int nMode = GetPrivateProfileInt("TOP10","MODE",0,strFile);

	char buff[1024];
	CString strTarget;

	GetPrivateProfileString("TOP10","TARGET","073",buff,sizeof(buff),strFile);

	strTarget.Format("%s",buff);

	m_n2018Target = 0;

	CWinApp* app = AfxGetApp();
	CString serviceID = app->GetProfileString(WORKSTATION, "serviceID");

	if((nMode == 0) || (nMode == 1 && strTarget.Find(serviceID) > -1))
	{
		m_n2018Target = 1;
	}

	if((nUse == 2 && Axis::isCustomer) || (nUse == 1 && !Axis::isCustomer) || nUse == 3)
	{
		Send2018();
	}

	//서킷브레이크 조회
	//SetTimer(TM_CB_SEARCH, 5000, NULL);

}

void CMainFrame::Send2018()
{
	CString date;
	const CTime tm = CTime::GetCurrentTime();
	
	date.Format("%04d%02d%02d",tm.GetYear(),tm.GetMonth(),tm.GetDay());
	m_slog.Format("[axis][Send2018] date =[ %s]\n", date);
	OutputDebugString(m_slog);


	std::unique_ptr <struct pibo2018_mid> pdata = std::make_unique<struct pibo2018_mid>();
	memset((char*)pdata.get(), ' ', sizeof(struct pibo2018_mid));
	memcpy(pdata->gubn, "m", 1);
	memcpy(pdata->date, date, date.GetLength());
	sendTR("pibo2018", (char*)pdata.get(), sizeof(struct pibo2018_mid), 0, 'S');
	
}

void CMainFrame::SendEnca(CString sDN,bool bSuccess)
{
	struct enca_mid mid;

	memset(&mid,' ',sizeof(mid));

	if(bSuccess)
		sprintf(mid.gubn, "S");
	else
		sprintf(mid.gubn, "E");

	CopyMemory(mid.dnxx,sDN,sDN.GetLength());
	
	sendTR("pibfenca", (char*)&mid, sizeof(mid), 0, 'H');
}	

void CMainFrame::ParseEnca(char* dat,int len)
{
	const struct enca_mod* mod = (struct enca_mod*)dat;

	if(mod->ret[0] == '1' && mod->cnt[0] != '5')
	{
		if(m_bEnca == true)
			return;
#ifdef DF_USE_CPLUS17
		CCertErrDialog dlg(m_axConnect.get());
#else
		CCertErrDialog dlg(m_axConnect);
#endif
		dlg.SetErrCount(CString(mod->cnt,1));

		if(dlg.DoModal() == IDOK)
		{
			
		}
	}
}

void CMainFrame::signOnCert()
{
#ifdef DF_SIMPLEAUTH
m_slog.Format("[QRCODE] signOnCert [%s][%s]", m_ip, m_port);;
OutputDebugString(m_slog);
	//간편인증
	if(m_bSimpleAuth)
	{
		m_sCustNumber.Empty();
		m_axConnect->ShowQRDlg();
		
		return;
	}
	//@@간편인증
#endif

OutputDebugString("GLB signOnCert");
	m_axConnect->SetGuide(_T("사용자정보 확인 중 입니다."));

	BOOL	rc = FALSE;
	CString	str;

	const int nEncSize = 0;

	char	wb[6890] = { 0, };
	const char encPass[10] = { 0, }, clkPass[16] = { 0, };
// 
// 	static struct i_pc {
// 	  char  optn[10];       /* [0] : 0:최초 2:강제
// 							   [1] : CA passwd
// 							   [2] : 시세전용
// 							   [3] : 0:직원 1:고객 */
// 	  char  cpas[30];	
// 	  char  pcip[15];
// 	  char  maca[20];
// 	  char  sdat[6776];         /* 인증 DN값 */
// 	} signPC;

	struct	i_pc {
		char	user[12]{};
		char	pass[10]{};
		char	dats[10]{};
		char	cpas[30]{};
		char	uips[15]{};
		char	madr[16]{};
		char  sdat[6776]{};         /* 인증 DN값 */
	} signPC;


	m_step = axOPENSIGN;
// 	Axis::userID = m_axConnect->GetUserID();
// 	m_pass = m_axConnect->GetPassword();
// 	m_cpass  = m_axConnect->GetCPass();

// 	CString cpass;
// 	cpass.Format("%s%c",m_cpass,'0x00');
// 	
// 	m_cpass = cpass;

// 	if(Axis::isCustomer)
// 		CopyMemory(encPass, m_pass, m_pass.GetLength());
// 	else
// 		CopyMemory(clkPass, m_pass, m_pass.GetLength());

	str = m_axConnect->GetSignInfo();
	
	m_axConnect->SetProgress(false);
	m_axConnect->SetStatus(SM_WSH);
	m_axConnect->SetChoice(false);

	//ZeroMemory(&signM, sizeof(struct _signM));
	memset(&signPC, ' ', sizeof(struct i_pc));

	CopyMemory(signPC.dats, str, str.GetLength());

	GetLocalIP();

	CString shashID{};
	shashID.Format("%08u", HashDataAXIS(Axis::userID));

	if (shashID == "1415129685")
	{
		if(!isIPInRange(m_ipAddr,"172.17.0.0") && !isIPInRange(m_ipAddr,"172.20.0.0"))
		{
			m_axConnect->SetGuide(_T("해당 계정은 사내에서만 접속이 가능합니다."));

			m_step = axNONE;
			m_axConnect->SetChoice(true);
			return;
		}
	}

	//**AfxMessageBox(m_ipAddr);
	CopyMemory(signPC.uips, m_ipAddr, m_ipAddr.GetLength());
	
	char	macaddr[20], ip[16];
	memset(macaddr, 0x00, sizeof(macaddr));
	memset(ip, 0x00, sizeof(ip));
	memcpy(ip, signPC.uips, sizeof(signPC.uips));

	CString s;
	s.Format("MAC INIT [%s]\n",macaddr);
	OutputDebugString(s);
	GetMacAddr(ip, macaddr);
	s.Format("MAC ADDR RESULT [%s]\n",macaddr);
	OutputDebugString(s);
	memcpy(signPC.madr, macaddr, strlen(macaddr));

	if (m_ip.IsEmpty())
	{
		CString glbip(get_glb_addr(macaddr, ip));

		if (!glbip.IsEmpty())
			m_ip = glbip;
	}

	//CopyMemory(signPC.cpas,m_axConnect->GetCPass(),m_axConnect->GetCPass().GetLength());

	char ca[6776];
	int ret{};

	memset(ca,' ',6776);
	//CopyMemory(ca,m_axConnect->GetCPass(),m_axConnect->GetCPass().GetLength());

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(caFULL, 1),ca);

	CString sCert(ca,sizeof(ca));
	sCert.TrimRight();
	
	CString sCSize = sCert.Mid(0,5);
	CString sDSize = sCert.Mid(5,5);
	
	const int nCSize = atoi(sCSize);
	const int nDSize = atoi(sDSize);
	
	CString sCAFull = sCert.Mid(10,nCSize);
	CString sDN = sCert.Mid(10 + nCSize,nDSize);

	s.Format("CERT DN [%d] [%d] [%s]\n",nCSize,nDSize,sDN);
	OutputDebugString(s);

	m_strDN = sDN;


	if(ret == 2501)
	{
		m_step = axNONE;
		m_axConnect->SetChoice(true);
		m_axConnect->SetGuide("인증로그인을 취소했습니다.");
		return;
	}
	else if(ret == 2417)
	{
		m_step = axNONE;
		m_axConnect->SetChoice(true);
		str = "인증 비밀번호 오류입니다.";
		m_axConnect->SetGuide(str);

		SendEnca(m_strDN,false);
		return;
	}
	else if(ret == 2500)
	{
		m_step = axNONE;
		m_axConnect->SetChoice(true);
		str = "선택 가능한 인증서가 없습니다.";
		m_axConnect->SetGuide(str);
		
		//SendEnca(m_strDN,false);
		return;
	}
	else if(ret == -1)
	{
		m_step = axNONE;
		m_axConnect->SetChoice(true);
		str = "AXCERTIFY 또는 버퍼에러입니다.";
		m_axConnect->SetGuide(str);
		return;
	}
	else if(ret > 1)
	{
		m_step = axNONE;
		m_axConnect->SetChoice(true);
		//m_axMisc->GetGuide(AE_ESECURE, str);   
		str.Format("인증오류 [%d]",ret);
		m_axConnect->SetGuide(str);
		return;
	}	

	//CopyMemory(signPC.sdat,ca,sizeof(ca));

	CopyMemory(signPC.sdat,sCAFull,nCSize);

	m_cpass = sCAFull;
	//AfxMessageBox(s);

	if(sCert == "")
	{
		m_step = axNONE;
		m_axConnect->SetChoice(true);
		m_axMisc->GetGuide(AE_ESECURE, str);   
		m_axConnect->SetGuide(str);
		return;
	}
	//AfxMessageBox(signPC.sdat);

	int nBytes = sizeof(i_pc);
	CopyMemory(wb, (char *)&signPC, nBytes);
	wb[nBytes] = '\0';
	
	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), signUSERc, (long)wb, nBytes);

	if (!rc)
	{
		m_step = axNONE;
		m_axConnect->SetChoice(true);
		m_axMisc->GetGuide(AE_ESIGNON, str);
		m_axConnect->SetGuide(str);
	}
}

void CMainFrame::signOn()
{
//m_slog.Format("[QRCODE] signOnCert [%s][%s]",m_ip,m_port);
//OutputDebugString(m_slog);
	OutputDebugString("----------------------GLB signOn");

#ifdef DF_SIMPLEAUTH
	//간편인증
	if(m_bSimpleAuth)
	{
		m_sCustNumber.Empty();
		m_axConnect->ShowQRDlg();
	
		return;
	}
	//@@간편인증
#endif
	//SetEncriptFile();
	if(m_bUseNewLogin)
	{
		OutputDebugString("GLB m_bUseNewLogin");
		m_axConnect->SetGuide(_T("사용자정보 확인 중 입니다."));

		BOOL	rc = FALSE;
		CString	str;

		const int nEncSize = 0;

		char	wb[128]{}, encPass[12]{}, clkPass[16]{};

		ZeroMemory(encPass, sizeof(encPass));
		ZeroMemory(clkPass, sizeof(clkPass));
		struct	_signM {
			char	user[12]{};
			char	pass[12]{};
			char	dats[10]{};
			char	cpas[30]{};
			char	uips[15]{};
			char	madr[16]{};
			char	encP[16]{};
		} signM;


		m_step = axOPENSIGN;
		Axis::userID = m_axConnect->GetUserID();
		m_pass = m_axConnect->GetPassword();
		m_cpass  = m_axConnect->GetCPass();

	// 	CString cpass;
	// 	cpass.Format("%s%c",m_cpass,'0x00');
	// 	
	// 	m_cpass = cpass;

		if(Axis::isCustomer)
			CopyMemory(encPass, m_pass, m_pass.GetLength());
		else
			CopyMemory(clkPass, m_pass, m_pass.GetLength());
	//#ifndef	ENCTEST
	//	if (false && m_connectBy == byTCP)
	//#endif
		{
			long	ret = 0;
			sprintf(wb, "%s\t%s", m_pass, Axis::userID);
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
						(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(encPASS, modeUID), (LPARAM) wb);
			CString ss = (char *) ret;

			if(Axis::isCustomer)
			{
				memset(encPass, ' ', sizeof(encPass));
				CopyMemory(encPass, ss, ss.GetLength());
			}
			else
			{
				memset(clkPass, ' ', sizeof(clkPass));
				CopyMemory(clkPass, ss, ss.GetLength());
			}
		}
		str = m_axConnect->GetSignInfo();

		m_axConnect->SetProgress(false);
		m_axConnect->SetStatus(SM_WSH);
		m_axConnect->SetChoice(false);

		//ZeroMemory(&signM, sizeof(struct _signM));
		memset(&signM, ' ', sizeof(struct _signM));
		CopyMemory(signM.user, Axis::userID, Axis::userID.GetLength());
		
		if(Axis::isCustomer)
		{
			CopyMemory(signM.pass, encPass, sizeof(encPass));
		}

		CopyMemory(signM.dats, str, str.GetLength());
		CopyMemory(signM.cpas, m_cpass, m_cpass.GetLength());

		//인증모듈(AXCERTIFY.OCX) 변경 후 널문자 마지막에 추가
		signM.cpas[m_cpass.GetLength()] = '\0';

		if (m_ipAddr.IsEmpty())
		{
			GetLocalIP();
		}

		CString shashID{};
		shashID.Format("%08u", HashDataAXIS(Axis::userID));
		if (shashID == "1415129685")
		{
			if(!isIPInRange(m_ipAddr,"172.17.0.0") && !isIPInRange(m_ipAddr,"172.20.0.0"))
			{
				m_axConnect->SetGuide(_T("해당 계정은 사내에서만 접속이 가능합니다."));

				m_step = axNONE;
				m_axConnect->SetChoice(true);
				return;
			}
		}

		//**AfxMessageBox(m_ipAddr);
		CopyMemory(signM.uips, m_ipAddr, m_ipAddr.GetLength());
		
		char	macaddr[20], ip[16];
		memset(macaddr, 0x00, sizeof(macaddr));
		memset(ip, 0x00, sizeof(ip));
		memcpy(ip, signM.uips, sizeof(signM.uips));

		CString s;
		s.Format("MAC INIT [%s]\n",macaddr);
		OutputDebugString(s);
		GetMacAddr(ip, macaddr);
		s.Format("MAC ADDR RESULT [%s]\n",macaddr);
		OutputDebugString(s);
		memcpy(signM.madr, macaddr, strlen(macaddr));

		if(!Axis::isCustomer)
		{
			CopyMemory(signM.encP, clkPass,sizeof(clkPass));
		}

		int nBytes = sizeof(_signM);
		CopyMemory(wb, (char *)&signM, nBytes);
		wb[nBytes] = '\0';

		CString ss;
		ss.Format("GLB signOn new logindlg m_ip = [%s]\n",m_ip);
		OutputDebugString(ss);

		if (m_ip.IsEmpty())
		{
			CString glbip(get_glb_addr(macaddr, ip));
CString slog;
slog.Format("GLB signOn ip =  [%s]\n" ,CString(ip, 16));
OutputDebugString(slog);
			ss.Format("GLB SIGNON glbip [%s]\n",glbip);
			OutputDebugString(ss);
			if (!glbip.IsEmpty())
				m_ip = glbip;
			ss.Format("GLB SIGNON m_ip [%s]\n",m_ip);
			OutputDebugString(ss);
		}

		memcpy(signM.uips, (const char*)m_ip, m_ip.GetLength());
		
		//AFX_MODULE_STATE* pModuleState = AfxGetModuleState();

	/*	char buf[256]{};
		GetCurrentDirectory(256, buf);
		CString spath;
		spath = Axis::home + "\\exe";
		CString stmp;
		stmp.Format("%s", buf);
		int ret = SetCurrentDirectory(spath);
		memset(buf, 0x00, 256);
		GetCurrentDirectory(256, buf);
		spath.Format("%s", buf); spath.TrimRight();

		if(stmp.Find(spath) < 0)
			SetCurrentDirectory(stmp);

		memset(buf, 0x00, 256);
		GetCurrentDirectory(256, buf);*/
		

		m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4 VTS_I4), signUSER, (long)wb, nBytes);

	/*	ss.Format("GLB SIGNON signUSER  rc= [%d]\n", rc);
		OutputDebugString(ss);*/

		if (!rc)
		{
			m_step = axNONE;
			m_axConnect->SetChoice(true);
			m_axMisc->GetGuide(AE_ESIGNON, str);
			m_axConnect->SetGuide(str);
		}
	}
	else
	{
OutputDebugString("GLB not m_bUseNewLogin");
		m_axConnectOld->SetGuide(_T("사용자정보 확인 중 입니다."));

		BOOL	rc = FALSE;
		CString	str;

		const int nEncSize = 0;

		char	wb[128]{}, encPass[10]{}, clkPass[16]{};

		ZeroMemory(encPass, sizeof(encPass));
		ZeroMemory(clkPass, sizeof(clkPass));
		struct	_signM {
			char	user[12]{};
			char	pass[12]{};
			char	dats[10]{};    
			char	cpas[30]{};
			char	uips[15]{};
			char	madr[16]{};
			char	encP[16]{};
		} signM;


		m_step = axOPENSIGN;
		Axis::userID = m_axConnectOld->GetUserID();
		m_pass = m_axConnectOld->GetPassword();
		m_cpass  = m_axConnectOld->GetCPass();

	// 	CString cpass;
	// 	cpass.Format("%s%c",m_cpass,'0x00');
	// 	
	// 	m_cpass = cpass;

		if(Axis::isCustomer)
			CopyMemory(encPass, m_pass, m_pass.GetLength());
		else
			CopyMemory(clkPass, m_pass, m_pass.GetLength());
	//#ifndef	ENCTEST
	//	if (false && m_connectBy == byTCP)
	//#endif
		{
			long	ret = 0;
			sprintf(wb, "%s\t%s", m_pass, Axis::userID);
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
						(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(encPASS, modeUID), (LPARAM) wb);
			CString ss = (char *) ret;

			if(Axis::isCustomer)
			{
				memset(encPass, ' ', sizeof(encPass));
				CopyMemory(encPass, ss, ss.GetLength());
			}
			else
			{
				memset(clkPass, ' ', sizeof(clkPass));
				CopyMemory(clkPass, ss, ss.GetLength());
			}
		}
		str = m_axConnectOld->GetSignInfo();

		m_axConnectOld->SetProgress(false);
		m_axConnectOld->SetStatus(SM_WSH);
		m_axConnectOld->SetChoice(false);

		//ZeroMemory(&signM, sizeof(struct _signM));
		memset(&signM, ' ', sizeof(struct _signM));
		CopyMemory(signM.user, Axis::userID, Axis::userID.GetLength());
		
		if(Axis::isCustomer)
		{
			CopyMemory(signM.pass, encPass, sizeof(encPass));
		}

		CopyMemory(signM.dats, str, str.GetLength());
		CopyMemory(signM.cpas, m_cpass, m_cpass.GetLength());

		//인증모듈(AXCERTIFY.OCX) 변경 후 널문자 마지막에 추가
		signM.cpas[m_cpass.GetLength()] = '\0';

		if (m_ipAddr.IsEmpty())
		{
			GetLocalIP();
		}

		CString shashID{};
		shashID.Format("%08u", HashDataAXIS(Axis::userID));
		if (shashID == "1415129685")
		{
			if(!isIPInRange(m_ipAddr,"172.17.0.0") && !isIPInRange(m_ipAddr,"172.20.0.0"))
			{
				m_axConnectOld->SetGuide(_T("해당 계정은 사내에서만 접속이 가능합니다."));

				m_step = axNONE;
				m_axConnectOld->SetChoice(true);
				return;
			}
		}

		//**AfxMessageBox(m_ipAddr);
		CopyMemory(signM.uips, m_ipAddr, m_ipAddr.GetLength());
		
		char	macaddr[20], ip[16];
		memset(macaddr, 0x00, sizeof(macaddr));
		memset(ip, 0x00, sizeof(ip));
		memcpy(ip, signM.uips, sizeof(signM.uips));

		CString s;
		s.Format("MAC INIT [%s]\n",macaddr);
		OutputDebugString(s);
		GetMacAddr(ip, macaddr);
		s.Format("MAC ADDR RESULT [%s]\n",macaddr);
		OutputDebugString(s);
		memcpy(signM.madr, macaddr, strlen(macaddr));

		if(!Axis::isCustomer)
		{
			CopyMemory(signM.encP, clkPass,sizeof(clkPass));
		}

		int nBytes = sizeof(_signM);
		CopyMemory(wb, (char *)&signM, nBytes);
		wb[nBytes] = '\0';

		CString ss;
		ss.Format("GLB signOn M_IP [%s]\n",m_ip);
		OutputDebugString(ss);

		if (m_ip.IsEmpty())
		{
			CString glbip(get_glb_addr(macaddr, ip));
CString slog;
slog.Format("GLB signOn ip = [%s]\n " ,CString(ip, 16));
OutputDebugString(slog);
			ss.Format("GLB SIGNON glbip [%s]\n",glbip);
			OutputDebugString(ss);
			if (!glbip.IsEmpty())
				m_ip = glbip;
			ss.Format("GLB SIGNON m_ip [%s]\n",m_ip);
			OutputDebugString(ss);
		}

		memcpy(signM.uips, (const char*)m_ip, m_ip.GetLength());
		
		m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4 VTS_I4), signUSER, (long)wb, nBytes);

		if (!rc)
		{
			m_step = axNONE;
			m_axConnectOld->SetChoice(true);
			m_axMisc->GetGuide(AE_ESIGNON, str);
			m_axConnectOld->SetGuide(str);
		}
	}
}

unsigned int CMainFrame::IPToUInt(CString ip) 
{
	int a{}, b{}, c{}, d{};
    unsigned int addr = 0;
	
    if (sscanf_s(ip, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
        return 0;
	
    addr = a << 24;
    addr |= b << 16;
    addr |= c << 8;
    addr |= d;
    return addr;
}

bool CMainFrame::isIPInRange(CString ip,CString network)
{
	const unsigned int ip_addr = IPToUInt(ip);
	const unsigned int network_addr = IPToUInt(network);
	const unsigned int mask_addr = IPToUInt("255.255.0.0");
	
	const unsigned int net_lower = (network_addr & mask_addr);
	const unsigned int net_upper = (net_lower | (~mask_addr));
	
    if (ip_addr >= net_lower &&
        ip_addr <= net_upper)
        return true;
    return false;
}

void CMainFrame::setTitle(int key, CString title)
{
	CString	mapN = _T("");
	int vsN{}; 
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
		CSChild* schild{};
		CChildFrame* child{};
		
		for (int vsN = V_SCREEN1; vsN <= V_SCREEN6 && !match; vsN++)
		{
			if (!m_arMDI[vsN].Lookup(key, child))
				continue;
			
			match = true;
			if (!mapN.IsEmpty())
			{
				child->SetMapName(axiscall, mapN);
				m_bar1->set_Maps(mapN);
#ifdef DF_USE_CPLUS17
				m_bar2->change_Info(key, mapN, m_tMenu.get());
#else
				m_bar2->change_Info(key, mapN, m_tMenu);
#endif
				
				if (!ExistMenu(mapN))
				{
					CString	string = child->m_xcaption.GetTitle();
					m_bar2->change_ButtonText(key, string, vsN);
				}				
			}
			SetSCN((CWnd*) child, key, child->m_mapN, title);
		}

		if (!match)
		{
			for (vsN = V_SCREEN1; vsN <= V_SCREEN6 && !match; vsN++)
			{
				if (!m_arSDI[vsN].Lookup(key, schild))
					continue;
				match = true;
				SetSCN((CWnd*) schild, key, schild->m_mapN, title, true);
			}
		}
	}
}

void CMainFrame::SetSCN(CWnd* wnd, int key, CString mapN, CString title, bool bSDI)
{
	if (!wnd->GetSafeHwnd() || title.IsEmpty())
		return;

	title.TrimLeft();

	CString orgTitle(title);

	CString mapTitle(m_tMenu->GetDesc(mapN));
	CString tmp;
	CString sc_gb(_T(""));

	if (!Axis::isCustomer) sc_gb = " - 직원용";
	if (Axis::devMode) tmp = "(개발용) ";
	
	if (mapTitle.Left(6) != mapN.Left(6))
		title = Format("[%s] "+tmp+"%s", m_tMenu->GetDispN(mapN), m_tMenu->GetDesc(mapN));
	title += sc_gb;
	if (WK_NORM <= key && key < WK_POPUP)
	{
		if (bSDI)
			((CSChild*)wnd)->m_xcaption.SetTitle(title);
		else	
			((CChildFrame*)wnd)->m_xcaption.SetTitle(title);
	}
	else
	{
		wnd->SetWindowText(orgTitle);
	}

	CString	mpN = m_tMenu->GetDispN(mapN);

	if (m_tMenu->IsMappinged(mapN))
	{
		long	rc = 0;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setSCN, key), (LPARAM)(const char*)mpN);
	}
}

int CMainFrame::create_Newview(int actkey, char* data)
{
	int	 key{}, position{};
	CString	 mapN;
	CPoint	 point;
	_userWH* userWH;
	
	userWH = (struct _userWH *)data;
	mapN   = CString(userWH->maps);

	if (ScreenCheck(mapN) == DF_NUSE)
		return 0;
	
	if (ExceptionProcess(mapN))	return 1;
	if (userWH->pos.x == -1)
		position = userWH->pos.y;
	else	position = -1;
	point = CPoint(userWH->pos.x, userWH->pos .y);

	switch (userWH->type)
	{
	case typeVIEW:
		key = m_mapHelper->CreateChild(mapN, userWH->group, userWH->key, position, point);
		//key = m_mapHelper->ChangeChild(mapN, userWH->group, userWH->key, position);//15.08.06 axWizard 수정으로 팝업메뉴에서 무조건 newView 발생
		if (position != -1)	
			positionWindow(actkey, key, position);
		return key;
	case typeMODAL:
	{
		CString sMapN;
		sMapN = GetMapNumByKey(m_activeKey);
		if (mapN.Find("IB999920") >= 0)
		{
			CString	sfile, stmp, smap;
			sfile.Format("%s\\tab\\axis.ini", Axis::home);
			WritePrivateProfileString("ParentMap", "IB999920", sMapN, sfile);
		}
		return m_mapHelper->CreateModal(mapN, userWH->group, userWH->key, position, actkey, point);
	}
	case typePOPUP:
		return m_mapHelper->CreatePopup(mapN, userWH->group, userWH->key, position, actkey, point);
	case typePOPUPX:
		return m_mapHelper->CreatePopup(mapN, userWH->group, userWH->key, position, actkey, point, true);
	case typePOPUPXN:
		return m_mapHelper->CreatePopup(mapN, userWH->group, userWH->key, position, actkey, point, true, true);
	}
	return 0;
}

void CMainFrame::loadMap(int key, int size)
{
	if (!m_modal)	return;
	key = m_modal->m_vwKEY;
#ifdef DF_USE_CPLUS17
	m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void*)&key,
		(BYTE*)(VTS_I4 VTS_I4 VTS_I4), (long)m_modal->m_wnd.get(), m_modal->m_vwTYPE, key);
#else
	m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)m_modal->m_wnd, m_modal->m_vwTYPE, key);
#endif
	m_modal->m_vwKEY = key;
	m_arMPOP.SetAt(key, m_modal);
 
	if (m_modal->m_wpos >= CenterPOS)
		positionWindow(m_modal->m_actW, m_modal, m_modal->m_wpos, false);

	BOOL	rc = FALSE;
	m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, m_modal->m_mapN, size, false/*true*/);
	if (!rc)	m_modal->PostMessage(WM_CLOSE, 0, 0);
}

void CMainFrame::Setfocus_Child(int key,bool bVS)
{
// 	CChildFrame*	child;
// 	m_activeKey = key;
// 	if (!m_arMDI[m_vsN].Lookup(key, child))
// 	{
// 		CSChild*	schild;
// 		if (m_arSDI[m_vsN].Lookup(key, schild))
// 		{
// 			if (schild->m_bIconic)
// 				schild->PostMessage(WM_SYSCOMMAND, SC_RESTORE);//actionSCaption(schild->m_key, IDX_RESTORE);
// 			else	
// 				schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
// 		}
// 	}
// 	else
// 	{
// 		if (child->m_bIconic)
// 			child->actionCaption(IDX_RESTORE, CPoint(0, 0));
// 		else	
// 			child->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
// 	}

	CChildFrame* child{};
	BOOL bTabChange = FALSE;

	if(bVS == FALSE)
	{
		if(m_activeKey != key)
		{
			bTabChange = TRUE;
		}
	}
	else
	{
		bTabChange = TRUE;
	}

	m_activeKey = key;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		CSChild*	schild;
		CRect wRc;
		if (m_arSDI[m_vsN].Lookup(key, schild))
		{
			if (schild->m_bIconic)
			{
				schild->PostMessage(WM_SYSCOMMAND, SC_RESTORE);//actionSCaption(schild->m_key, IDX_RESTORE);
			}
			else	
			{
				if(bTabChange != TRUE)
				{
					//최소화 상태에서 다시 원복하게 변경
					const CPoint sp(-1, -1);

					if (!schild->m_xcaption.IsMax())
					{
						schild->GetWindowRect(wRc);
						schild->m_restoreRc.CopyRect(&wRc);
					}

					schild->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
				}
				else
				{
					schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
				}
				schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			}
		}
	}
	else
	{
// 		CString s;
// 		s.Format("[MAPNAME:%s]\n",child->m_mapN);
// 		OutputDebugString(s);
		if (child->m_bIconic)
		{
			child->actionCaption(IDX_RESTORE, CPoint(0, 0));
		}
		else	
		{	
			if(bTabChange != TRUE)
			{
				//최소화 상태에서 다시 원복하게 변경
				CPoint sp(-1, -1);

				sp = getMinimizePos(child->m_key);
				child->actionCaption(IDX_MIN, sp);
				MDINext();
			}
			else
			{
				child->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			}

			CProfile p1(pkVTCode);
			
			CString map(child->m_mapN);

			//최소화 처리 dkkim 2015.10.28
			//DLL은 실제로 최소화하지 않고 화면만 숨긴다.OnSize를 보내지 않음.
			if(p1.GetString("DLL Controls", map).IsEmpty())
			{
				child->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			}
		}
	}
}

void CMainFrame::showfname()
{
	if (!Axis::devMode && !IsSuperUser())	
		return;

	CFname	dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString mapN = dlg.GetMapName();
		if (mapN.GetLength() == L_MAPN)
			m_mapHelper->ChangeChild(mapN);

		CString file;
		file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);
		WritePrivateProfileString("DEVRUN", "MAPNAME", mapN, file);
	}
}

void CMainFrame::WriteFile(char* pBytes, int nBytes)
{
#if 0
	CFile		Dfile;
	DWORD		pos;
	CString		filename, time;

	filename.Format("%s\\log1", Axis::home);
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

void CMainFrame::WriteErrFile(char* pBytes, int nBytes)
{
	CFile		Dfile;
	DWORD		pos{};
	CString		filename, time;
	
	filename.Format("%s\\log", Axis::home);
	const BOOL bExist = Dfile.Open(filename, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);
	if (!bExist)
	{
		Dfile.Open(filename, CFile::modeCreate|CFile::modeReadWrite);
		Dfile.Write(pBytes, nBytes);
	}
	else
	{
		pos = (DWORD)Dfile.SeekToEnd();
		Dfile.Seek((long) pos, CFile::begin);
		Dfile.Write(pBytes, nBytes);
	}
	Dfile.Close();
}

void CMainFrame::write_err()
{
	CString	err;
	const DWORD	dw = GetLastError();
	err.Format("[GDI_ERROR] [%u]\n", dw);
	WriteFile((char *)(const char*)err, err.GetLength());
}

#ifdef DF_MAIN_RTS
//bool CMainFrame::ReadTick(const char* code, TickSnapshot* out)  //dll에서 호출
//{
//	int idx = GetSlotIndex_FindOnly(code);
//	ReadTick(idx, *out);
//	return false;
//}
//
//bool CMainFrame::ReadTick(int idx, TickSnapshot& out)
//{
//	TickSnapshot& s = g_tickSlots[idx];
//
//	for (;;)
//	{
//		int v1 = s.seq.load(std::memory_order_acquire);
//		if (v1 & 1) continue; // writing 중
//
//		CopySnapshot(out, s);   //  구조체 직접 복사 대신 사용
//
//		int v2 = s.seq.load(std::memory_order_acquire);
//		if (v1 == v2 && !(v2 & 1))
//			return true;
//	}
//}
#endif

#ifdef DF_MAIN_RTS
void CMainFrame::update_ticker(int kind, struct _alertR* alertR)
{
		if (!alertR || alertR->size <= 0 || alertR->ptr[0] == 0)
			return;

		/*
		DWORD uiThreadId = AfxGetApp()->m_nThreadID;  // UI 스레드 ID
		DWORD curThreadId = GetCurrentThreadId();      // 현재 스레드 ID
		CString slog;
		slog.Format("[update_ticker] UI스레드=[%d] 현재스레드=[%d] %s\n",
			uiThreadId, curThreadId,
			(uiThreadId == curThreadId) ? "UI스레드" : "별도스레드");
		OutputDebugString(slog);*/

		CString symbol, sData;
		symbol = alertR->code;
		DWORD* data{};

		for (int i = 0; i < alertR->size; i++)
		{
			data = (DWORD*)alertR->ptr[i];
			sData.Format("%s", (char*)data[0]);
			if (sData == "d" || sData == "D")
				return;

			CString str;
			if (!symbol.IsEmpty() && (symbol.GetAt(0) == 'X' || symbol.GetAt(0) == 'x'))
			{
				m_slog.Format("[mnginfo]");
				output_DebugString(m_slog);
				m_slog.Format("[mnginfo][%s]<%d>  size=[%d] symbol=[%s]", __FUNCTION__, __LINE__, alertR->size, symbol);
				output_DebugString(m_slog);

				if (symbol.CompareNoCase(SYM_MNG) == 0)
				{
					CTime time;
					time = CTime::GetCurrentTime();
					CString str;
					str.Format("\r\n update_ticker[%02d:%02d:%02d] kind[%d] sym[%s] dat[%.200s]\n",
						time.GetHour(), time.GetMinute(), time.GetSecond(), kind, symbol, data);
					OutputDebugString(str);
					ShowMngInfo(data);
				}
				else
				{
					str = ReplaceExpectSymbol(symbol);
					if (!str.IsEmpty())
						symbol = str;
				}
			}

			if (kind != 0)
			{
				if (kind == 6)
				{
					const char* szNewsRTS = "S0000";
					if (m_tInfo1)
						m_tInfo1->ProcessRTS(szNewsRTS, data);
					if (m_tInfo2)
						m_tInfo2->ProcessRTS(szNewsRTS, data);
				}
				return;
			}

			if (i == 0)
			{
				if (m_tInfo1 && m_tInfo1->IsVisible())
					m_tInfo1->ProcessRTS(symbol, data);
				if (m_tInfo2 && m_tInfo2->IsVisible())
					m_tInfo2->ProcessRTS(symbol, data);
			}
		}

		if (!m_bMainRTS)
			return;
	/*
		// 덤프용 카운터
		if (m_tickCountStart == 0)
			m_tickCountStart = GetTickCount();

		const char* code = (const char*)(LPCTSTR)alertR->code;

		if (!code || !code[0]) return;

		m_tickCount[std::string(code)]++;

		// 큐 사이즈 체크 - new 하기 전에
		{
			std::lock_guard<std::mutex> lock(m_alertMutex);
			if (m_alertQueue.size() > 500)
				return;
		}

		// UI 스레드에서 포인터 역참조 + 값 복사
		if (1)
		{
			if (!code || !code[0]) return;
			m_tickCount[std::string(code)]++;
			if (m_tickCountStart == 0)
				m_tickCountStart = GetTickCount();

			int idx = EnsureSlotIndexForCode(code);
			if (idx < 0) return;

			TickSnapshot& s = g_tickSlots[idx];
			s.ts_ms = GetTickCount();

			static const int targetSymbols[] = {
				0,    // 구분
				23,   // 현재가
				24,   // 전일대비
				27,   // 거래량
				33,   // 등락률
				29,   // 시가
				30,   // 고가
				31,   // 저가
				34,   // 체결시간
				40,   // 서버시간
				111,  // 예상가
				112,  // 예상거래량
				115,  // 예상대비
				116,  // 예상등락률
			};
			constexpr int targetCount = sizeof(targetSymbols) / sizeof(targetSymbols[0]);

			for (int ii = alertR->size - 1; ii >= 0; ii--)
			{
				if (alertR->ptr[ii] == 0) continue;
				const PTR_T* data = reinterpret_cast<const PTR_T*>(alertR->ptr[ii]);

				for (int kk = 0; kk < targetCount; kk++)
				{
					int jj = targetSymbols[kk];
					if (data[jj] == 0) continue;
					const char* val = reinterpret_cast<const char*>(data[jj]);
					if (val && val[0])
					{
						CopyZ(s.values[jj], SYMBOL_STR_LEN, val);
						s.valid.set(jj);
					}
				}
			}
		}
		else
		{
			SafeAlertItem* item = new SafeAlertItem{};
			CopyZ(item->code, CODE_LEN, code);
			item->ts_ms = GetTickCount();
			memset(item->hasValue, 0, sizeof(item->hasValue));

			for (int ii = alertR->size - 1; ii >= 0; ii--)
			{
				if (alertR->ptr[ii] == 0) continue;
				const PTR_T* data = reinterpret_cast<const PTR_T*>(alertR->ptr[ii]);

				for (int jj = 0; jj < MAX_RTS_INDEX; jj++)
				{
					if (data[jj] == 0) continue;
					const char* val = reinterpret_cast<const char*>(data[jj]);
					if (val && val[0])
					{
						CopyZ(item->values[jj], SYMBOL_STR_LEN, val);
						item->hasValue[jj] = true;
					}
				}
			}

			std::lock_guard<std::mutex> lock(m_alertMutex);
			m_alertQueue.push(item);
		}
		*/
}
#else
void CMainFrame::update_ticker(int kind, struct _alertR* alertR)
{
	CString symbol;
	CString sData;
	CString stmp;

	symbol = alertR->code;

	DWORD* data{};
	int i = 0;
	for (int i = 0; i < alertR->size; i++)
	{
		data = (DWORD*)alertR->ptr[i];

		sData.Format("%s", (char*)data[0]);

		if (sData == "d" || sData == "D")
			return;

		CString	str;
		if (!symbol.IsEmpty() && (symbol.GetAt(0) == 'X' || symbol.GetAt(0) == 'x'))
		{
			m_slog.Format("[mnginfo]");
			output_DebugString(m_slog);
			m_slog.Format("[mnginfo][%s]<%d>  size=[%d] symbol=[%s]", __FUNCTION__, __LINE__,  alertR->size, symbol );
			output_DebugString(m_slog);


			// 장운영정보를 위해 추가
			if (symbol.CompareNoCase(SYM_MNG) == 0)
			{

				CTime time;
				time = CTime::GetCurrentTime();
				CString str;
				str.Format("\r\n update_ticker[%02d:%02d:%02d] kind[%d] sym[%s] dat[%.200s]\n", time.GetHour(), time.GetMinute(), time.GetSecond(), kind, symbol, data);
				OutputDebugString(str);

				ShowMngInfo(data);		//진짜사용	

				//return;
			}
			else
			{
				str = ReplaceExpectSymbol(symbol);
				if (!str.IsEmpty())
					symbol = str;
			}
		}

		if (kind != 0)
		{
			if (kind == 6)
			{
				const char* szNewsRTS = "S0000";

				if (m_tInfo1)
					m_tInfo1->ProcessRTS(szNewsRTS, data);
				if (m_tInfo2)
					m_tInfo2->ProcessRTS(szNewsRTS, data);
			}
			return;
		}

		if (i == 0)
		{
			if (m_tInfo1 && m_tInfo1->IsVisible())
				m_tInfo1->ProcessRTS(symbol, data);
			if (m_tInfo2 && m_tInfo2->IsVisible())
				m_tInfo2->ProcessRTS(symbol, data);
		}
	}
	
}
#endif
/*
void CMainFrame::update_ticker(int kind, struct _alertR* alertR)
{
	if (0)
	{
		CString symbol;
		CString sData, str;
		CString stmp;

		symbol = alertR->code;
		if (!(alertR->stat & alert_SCR))
			return;

		DWORD* data = reinterpret_cast<DWORD*>(alertR->ptr[0]);
		sData.Format("%s", (char*)data[0]);
		if (sData == "d" || sData == "D")
			return;

		//[xxx] 시세만...
		if (kind == 0)
		{


			// 장운영정보를 위해 추가
			if (symbol.CompareNoCase(SYM_MNG) == 0)
			{
				CTime time;
				time = CTime::GetCurrentTime();
				CString str;
				str.Format("\r\n update_ticker[%02d:%02d:%02d] kind[%d] sym[%s] dat[%.200s]\n", time.GetHour(), time.GetMinute(), time.GetSecond(), kind, symbol, data);
				OutputDebugString(str);
				ShowMngInfo(data);  //안쓴다

				return;
			}
			else
			{

				str = ReplaceExpectSymbol(symbol);
				if (!str.IsEmpty())
					symbol = str;
			}


			if (m_tInfo1 && m_tInfo1->IsVisible())
				m_tInfo1->ProcessRTS(symbol, data);
			if (m_tInfo2 && m_tInfo2->IsVisible())
				m_tInfo2->ProcessRTS(symbol, data);
		}
		else if (kind == 6) //[xxx] 뉴스만...
		{
			const char* szNewsRTS = "S0000";
			if (m_tInfo1)
				m_tInfo1->ProcessRTS(szNewsRTS, data);
			if (m_tInfo2)
				m_tInfo2->ProcessRTS(szNewsRTS, data);
		}
	}
	else
	{
		CString symbol;
		CString sData;
		CString stmp;

		symbol = alertR->code;

		DWORD* data{};
		int i = 0;
		for (int i = 0; i < alertR->size; i++)
		{
			data = (DWORD*)alertR->ptr[i];

			sData.Format("%s", (char*)data[0]);
			if (sData == "d" || sData == "D")
				return;

			CString	str;
			if (!symbol.IsEmpty() && (symbol.GetAt(0) == 'X' || symbol.GetAt(0) == 'x'))
			{
				// 장운영정보를 위해 추가
				if (symbol.CompareNoCase(SYM_MNG) == 0)
				{

					CTime time;
					time = CTime::GetCurrentTime();
					CString str;
					str.Format("\r\n update_ticker[%02d:%02d:%02d] kind[%d] sym[%s] dat[%.200s]\n", time.GetHour(), time.GetMinute(), time.GetSecond(), kind, symbol, data);
					OutputDebugString(str);

#ifndef DF_MNG_THREAD
					ShowMngInfo(data);		//진짜사용
#else
					if (!data[601])
						return;
					str.Format("%s", data[601]);
					_mapMngInfo.emplace(std::move("601"), std::move(str));

					if (!data[23])
						return;
					str.Format("%s", data[23]);
					_mapMngInfo.emplace(std::move("23"), std::move(str));

					if (!data[47])
						return;
					str.Format("%s", data[47]);
					_mapMngInfo.emplace(std::move("47"), std::move(str));

					if (!data[600])
						return;
					str.Format("%s", data[600]);
					_mapMngInfo.emplace(std::move("600"), std::move(str));

					SetTimer(TM_MNGINFO, 1000, nullptr);
#endif
					return;
				}
				else
				{
					str = ReplaceExpectSymbol(symbol);
					if (!str.IsEmpty())
						symbol = str;
				}
			}

			if (kind != 0)
			{
				if (kind == 6)
				{
					const char* szNewsRTS = "S0000";

					if (m_tInfo1)
						m_tInfo1->ProcessRTS(szNewsRTS, data);
					if (m_tInfo2)
						m_tInfo2->ProcessRTS(szNewsRTS, data);
				}
				return;
			}

			if (i == 0)
			{
				if (m_tInfo1 && m_tInfo1->IsVisible())
					m_tInfo1->ProcessRTS(symbol, data);
				if (m_tInfo2 && m_tInfo2->IsVisible())
					m_tInfo2->ProcessRTS(symbol, data);
			}
		}
	}
}
*/

void CMainFrame::show_Newslist(WPARAM wParam, LPARAM lParam)
{
	const int	id = LOWORD(wParam);
	const CPoint	point = CPoint(HIWORD(lParam), LOWORD(lParam));

//	m_news->SetList(id, Axis::user, point);
}

void CMainFrame::ShowNews()
{
	m_mapHelper->CreateChild("IB771000", 0);	
}

void CMainFrame::change_Skin()
{
	Axis::SetSkin(GetSkinName());

	// 리소스 리로드 
	Axis::ReloadSkin();


	SetTimer(TM_CHANGESKIN, 1000, nullptr);

	/*
	ChangeLogo();
	m_bitmapBtn->Invalidate();
	
	DrawFrame();
	change_BackGround();
	change_Resource();

	if (m_conclusion)	
		m_conclusion->ChangePalette();
	if (m_kobanotify)
		m_kobanotify->ChangePallete();
	if (m_Econclusion)	
		m_Econclusion->ChangePalette();
	if (m_mngInfo)		
		m_mngInfo->ChangePalette();

	m_bar0->change_BandInfo();
	m_bar1->Change_Skin(Axis::skinName);
	m_bar2->Change_Skin(Axis::skinName);
	m_smain->Change_Skin(Axis::skinName);
	
	long	rv = 0;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, 
				(void *)&rv, (BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setPAL, 0), 0);

	IMAXSkinSet();
	*/
}

void CMainFrame::IMAXSkinSet()
{
	int nkey{}, nSkinKind{};
	CString tmp, sSkinName;
	CSChild* schild{};
	CChildFrame* child{};

	sSkinName = Axis::skinName;

	if(sSkinName == "Gray")  //gray 회색임
		nSkinKind = 1;
	else if(sSkinName == "Blue")
		nSkinKind = 2;
	else if(sSkinName == "Green")
		nSkinKind = 2;
	else if(sSkinName == "Brown")
		nSkinKind = 4;
	else if(sSkinName == "Violet")
		nSkinKind = 2;

	tmp.Format("IMAXSKIN\t%d", nSkinKind);

	if(m_bSDI)
	{
		for(int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
		{
			for(POSITION pos = m_arSDI[vsN].GetStartPosition(); pos;)
			{
				m_arSDI[vsN].GetNextAssoc(pos, nkey, schild);
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, nkey), (LPARAM)(const char*)tmp);
			}
		}
	}
	else
	{
		for(int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
		{
			for(POSITION pos = m_arMDI[vsN].GetStartPosition(); pos;)
			{
				m_arMDI[vsN].GetNextAssoc(pos, nkey, child);
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, nkey), (LPARAM)(const char*)tmp);
			}
		}
	}

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, nkey), (LPARAM)(const char*)tmp);
}

void CMainFrame::change_VirtualScreen(int index)
{
	if (index == m_vsN)	return;

	displayChild(m_vsN, false);
	displayChild(m_vsN, false, true);

	m_vsN = index;

	m_activeKey = 0;

	m_bar2->change_VirtualScreen(index);
	m_smain->change_VirtualScreen(index);
	
	displayChild(m_vsN, true);
	displayChild(m_vsN, true, true);

	long	rc = 0;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setVID, 0), MAKELONG(0, index));

	OnNcPaint();
}

void CMainFrame::change_BackGround()
{
	m_MClient->change_Color(Axis::skinName, m_resourceHelper->GetColor(-1));
}

void CMainFrame::set_Trigger(int key, int triggerN)
{
	set_Trigger(key, triggerN, m_vsN);	
}

void CMainFrame::set_Trigger(int key, int triggerN, int vsN)
{
	if (vsN == -1)	vsN = m_vsN;
	int		ret{}, group = triggerN;
	CChildFrame*	child;

	if (!m_arMDI[vsN].Lookup(key, child))	
		return;

	child->m_xcaption.SetGroup(triggerN);

	if (triggerN == ALL_GROUP)
		group = -1;

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
			(BYTE *)(VTS_I4 VTS_I4), setGID, (long)MAKELONG(key, group));

	child->m_xcaption.SetGroup(triggerN);
	child->PostMessage(WM_NCPAINT, 1, 0);

	CString strGroup;
	strGroup.Format("%d",triggerN);
 	m_groupBymapN.SetAt(child->m_mapN,strGroup);

	IMAXGroupSet(key, triggerN);
}

void CMainFrame::IMAXGroupSet(int key, int triggerN)
{
	CString tmp;
	tmp.Format("IMAXGROUP\t%d", triggerN);
OutputDebugString(tmp);
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)tmp);
}



void CMainFrame::set_STrigger(int key, int triggerN, int vsN)
{
	if (vsN == -1)	vsN = m_vsN;
	int		ret{}, group = triggerN;

	CSChild*	child;
	if (m_arSDI[vsN].Lookup(key, child))
	{
		child->m_xcaption.SetGroup(triggerN);
		if (triggerN == ALL_GROUP)
			group = -1;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
				(BYTE *)(VTS_I4 VTS_I4), setGID, (long)MAKELONG(key, group));

		child->m_xcaption.SetGroup(triggerN);

		CString strGroup;
		strGroup.Format("%d",triggerN);
 		m_groupBymapN.SetAt(child->m_mapN,strGroup);
	}

	IMAXGroupSet(key, triggerN);
}

void CMainFrame::restore(int key)
{
	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))	
		return;

	child->SetWindowPos(NULL, 0, 0, child->GetSize().cx, child->GetSize().cy, SWP_NOMOVE);
}

bool CMainFrame::sendTR(char* datB, int datL)
{
	BOOL	rc = FALSE;
	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), (long)datB, datL);

	return (rc ? true : false);
}

bool CMainFrame::sendTR(CString trN, char* datB, int datL, BYTE stat, int key)
{
	if (datL <= 0)	return false;

	//char*	wb;

	BOOL	rc  = FALSE;
	struct	_userTH* userTH;
	std::unique_ptr <char[]> wb;
	wb = std::make_unique<char[]>(L_userTH + datL + 1);
    ZeroMemory(wb.get(), L_userTH + datL + 1);
	userTH = (struct _userTH*)wb.get();
	CopyMemory(userTH->trc, (char*)(const char*)trN, trN.GetLength());
	userTH->stat = stat;
	userTH->key = key;
	CopyMemory(&wb[L_userTH], datB, datL);
	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void*)&rc, (BYTE*)(VTS_I4 VTS_I4), (long)wb.get(), datL);
	
	/*  //vc2019 unique
	wb = new char[L_userTH+datL+1];
	ZeroMemory(wb, L_userTH+datL+1);
	userTH = (struct _userTH *)wb;
	CopyMemory(userTH->trc, (char *)(const char*)trN, trN.GetLength());
	
	userTH->stat = stat;	
	userTH->key  = key;
	CopyMemory(&wb[L_userTH], datB, datL);

	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), (long)wb, datL);
	delete[] wb;
	*/

	return (rc ? true : false);
}

void CMainFrame::positionWindow(int actkey, int poskey, int pos, bool child)
{
	CWnd* posW{};
	if (pos < LtopPOS)	return;
	if (!m_arMDI[m_vsN].Lookup(poskey, (CChildFrame *&) posW))
		return;
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
	else	m_arMDI[m_vsN].Lookup(actkey, (CChildFrame *&) actW);

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

	if (actW)
	{
		actW->GetWindowRect(&rect);
		if (child)
			m_MClient->ScreenToClient(&rect);
	}

	switch (pos / 10)
	{
	case 0:
		posW->GetWindowRect(&rect);
		if (child)	m_MClient->GetClientRect(&clientRc);
		else		m_MClient->GetWindowRect(&clientRc);

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
			posW->CenterWindow(m_MClient);
			return;
		}
		//posW->ShowWindow(SW_HIDE);
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

	if (child)	
		posW->MoveWindow(point.x, point.y, rect.Width(), rect.Height());
	else		
		posW->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
}

bool CMainFrame::Agree_Duplication(CString mapN,bool bDup)
{
	int		key{};
	POSITION 	pos{};
	WINDOWPLACEMENT	pl{};

	if (m_bSDI)	
		return !FindSDIMap(mapN);
	else
	{
		CChildFrame* pChild{};
		bool bfind2000 = false;
		if (mapN.Find("IB200000") >= 0 && mapN.Find("1301") >= 0)
		{
			for(pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
			{
				m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);
				if (pChild->m_mapN.Left(L_MAPN) == "IB2000XX" || pChild->m_mapN.Left(L_MAPN) == "IB2000YY")
				{
					bfind2000 = true;
					break;
				}
			}
		}

		for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);

			if(mapN == "IB200000")
			{
				if (getSMap(pChild->m_mapN.Left(L_MAPN)) != getSMap("IB2000XX") && getSMap(pChild->m_mapN.Left(L_MAPN)) != getSMap("IB2000YY"))
					continue;
			}
			else if (mapN.Find("IB200000") >= 0  && mapN.Find("1301") >= 0)  //돌려보기등으로 트리거 던진경우 
			{
				if (bfind2000 == false)
					continue;
			}
			else if (getSMap(pChild->m_mapN.Left(L_MAPN)) != getSMap(mapN.Left(L_MAPN)))
				continue;
			if (IsAgreeDuplicate(mapN.Left(L_MAPN)))
				continue;
			if (IsSelectMap(mapN))	loadDomino(pChild->m_key,  mapN);

			if (m_screenNew && bDup == false)
			{
				int		value{};
				const long		rc = 0;
				const CRect		sdiRc, mdiRc;
				CPoint		sp(-1, -1);
				CString		mapname;
				WINDOWPLACEMENT	pl;

				pChild->GetWindowPlacement(&pl);
				value = GetSystemMetrics(SM_CYCAPTION);
				value += GetSystemMetrics(SM_CXFRAME);
				value += GetSystemMetrics(SM_CXBORDER);
				sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
				sp.Offset(value, value);
				
// 				value = pChild->m_xcaption.GetGroup();
// 				switch (value)
// 				{
// 				case 0:/*none group*/
// 				case 99:/*all group*/	break;
// 				case 1:case 2: case 3:case 4: case 5:
// 					value++;
// 					if (value > 5)	value = 1;
// 					break;
// 				default:	
// 					value = 1;	break;
// 				}		
				CString strValue = "";
				
				if(!m_groupBymapN.Lookup(pChild->m_mapN,strValue))
				{
					strValue = _T("1");
				}
				
				if(strValue == "")
				{
					strValue = _T("1");
				}
				
				value = atoi(strValue);
				
				if(value == 10)
				{
					value = 1;
				}
				else
				{
					value += 1;
				}

				m_mapHelper->CopyScreen(pChild->m_mapN, value, pChild->m_xcaption.GetFontSize(), sp);
				return false;
			}
		
			pChild->GetWindowPlacement(&pl);
			if (pl.showCmd & SW_SHOWMINIMIZED)
			{
				pl.showCmd = SW_RESTORE;
				pChild->SetWindowPlacement(&pl);
			}
			else
			{
				if ((CString(mapN).Mid(0,6) == "IB7700") && (CString(mapN).Mid(0,8) != "IB770000"))
					return false;
				CString	data = mapN.Mid(L_MAPN);
				if (!data.IsEmpty())
				{
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

						m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
						(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)dat);
					}
				}
				pChild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			}

			//OutputDebugString("ISAGREEDUPLICATION\n");
			return false;
		}

		if (FindSDIMap(mapN))	
			return false;
	}
	//OutputDebugString("ISAGREEDUPLICATION=========>TRUE\n");
	return true;
}

// bool CMainFrame::Agree_Duplication(CString mapN)
// {
// 	CString s;
// 	s.Format("RUN MAP NAME : %s		%d\n",mapN,m_vsN);
// 	OutputDebugString(s);
// 	int		key;
// 	POSITION	pos;
// 	WINDOWPLACEMENT	pl;
// 	CPoint		sp(-1, -1);
// 	if (m_bSDI)	
// 		return !FindSDIMap(mapN);
// 	else
// 	{
// 		OutputDebugString("PROCESS====================\n");
// 		CChildFrame*	pChild;
// 		for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
// 		{
// 			OutputDebugString("PROCESS11====================\n");
// 			m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);
// 			OutputDebugString("PROCESS22====================\n");
// 			if (getSMap(pChild->m_mapN.Left(L_MAPN)) != getSMap(mapN.Left(L_MAPN)))
// 				continue;
// 			OutputDebugString("PROCESS33====================\n");
// 			if (IsAgreeDuplicate(mapN.Left(L_MAPN)))
// 				continue;
// 			OutputDebugString("PROCESS44====================\n");
// 			if (IsSelectMap(mapN))	loadDomino(pChild->m_key,  mapN);
// 
// 			s.Format("RUN MAP NAME 2: %s\n",mapN);
// 			OutputDebugString(s);
// 
// 			if (m_screenNew)
// 			{
// 				if (pl.showCmd & SW_SHOWMINIMIZED)
// 				{
// 
// 				}
// 				else
// 				{
// 					int		value;
// 					long		rc = 0;
// 					CRect		sdiRc, mdiRc;
// 					//CPoint		sp(-1, -1);
// 					CString		mapname;
// 					WINDOWPLACEMENT	pl;
// 
// 					pChild->GetWindowPlacement(&pl);
// 					value = GetSystemMetrics(SM_CYCAPTION);
// 					value += GetSystemMetrics(SM_CXFRAME);
// 					value += GetSystemMetrics(SM_CXBORDER);
// 					sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
// 					sp.Offset(value, value);
// 				}				
// 			}
// 			else
// 			{
// 				if (pl.showCmd & SW_SHOWMINIMIZED)
// 				{
// 					pl.showCmd = SW_RESTORE;
// 					pChild->SetWindowPlacement(&pl);
// 				}
// 				else
// 				{
// 					if ((CString(mapN).Mid(0,6) == "IB7700") && (CString(mapN).Mid(0,8) != "IB770000"))
// 						return false;
// 					CString	data = mapN.Mid(L_MAPN);
// 					if (!data.IsEmpty())
// 					{
// 						while (!data.IsEmpty())
// 						{
// 							CString	dat;
// 							int	po = data.Find('\n');
// 							if (po == -1)
// 							{
// 								dat = data;
// 								data.Empty();
// 							}
// 							else
// 							{
// 								dat = data.Left(po++);
// 								data = data.Mid(po);
// 							}
// 
// 							m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)dat);
// 						}
// 					}
// 					pChild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
// 				}
// 				return false;
// 			}
// 		}
// 
// 		if(m_screenNew)
// 		{
// 			CString strValue = "";
// 			int		value;
// 			
// 			if(!m_groupBymapN.Lookup(pChild->m_mapN,strValue))
// 			{
// 				strValue = 1;
// 			}
// 			
// 			if(strValue == "")
// 			{
// 				strValue = 1;
// 			}
// 			
// 			value = atoi(strValue);
// 			
// 			if(value == 5)
// 			{
// 				value = 1;
// 			}
// 			else
// 			{
// 				value += 1;
// 			}
// 			
// 			m_mapHelper->CopyScreen(pChild->m_mapN, value, pChild->m_xcaption.GetFontSize(), sp);
// 			return false;
// 		}
// 
// 		if (FindSDIMap(mapN))	
// 			return false;
// 	}
// 
// 	return true;
// }

bool CMainFrame::select_SName(CString input)
{
	CStringArray	array;
	if (!m_tMenu)		
		return false;

	if (m_tMenu->GetSelectList(input, array))
	{
		m_bar1->set_List(array);
		m_smain->set_List(array);
		return true;
	}

	CString	msg;
	msg.LoadString(ST_MSG_NOTHING_SELECTED);
	displayGuide(msg);
	return false;
}

void CMainFrame::changeSize(int key, CSize size)
{
	if (size.cx == 0 && size.cy == 0)	
		return;

	int	nCx{}, nCy{}, gapX{}, gapY{}, viewType{};
	
	gapX = GetFrameGap().cx;
	gapY = GetFrameGap().cy;
	gapY += CAPTION_HEIGHT;

	m_fit = true;
	if (key & winK_POPUP)
	{
		CMPop* Mpop{};
		CGPop* Gpop{};
		if (m_arMPOP.Lookup(key, Mpop))
		{
			if ((Mpop->m_vwTYPE & vtypeMSK) ==vtypeWND)
				Mpop->ModifyStyle(WS_CAPTION, 0, SWP_NOZORDER|SWP_DRAWFRAME);
			else	
				gapY = GetFrameGap().cy;

			nCx = size.cx + gapX;
			nCy = size.cy + gapY;
			
		
			Mpop->SetSize(size.cx, size.cy);
			if (Mpop->GetSafeHwnd())  //주문확인창등을 띄울때 캡션의 높이
				Mpop->SetWindowPos(NULL, 0, 0, nCx , nCy + 25, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
			
		}
		else if (m_arGPOP.Lookup(key, Gpop))
		{
			if ((Gpop->m_vwTYPE & vtypeMSK) == vtypeWND)
				Gpop->ModifyStyle(WS_CAPTION, 0, SWP_DRAWFRAME);
			else	
				gapY = GetFrameGap().cy;

			gapX -= 4;
			gapY -= 10;

			nCx = size.cx + gapX;
			nCy = size.cy + gapY;
			Gpop->SetSize(nCx, nCy);
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

		if (schild->IsTabView())	
			gapY +=	TABH;

		gapY -= 6;

		viewType = schild->m_vwTYPE & vtypeMSK;
		if (viewType == vtypeRSZ)
		{
			if (size.cx == 0)
			{
				
				nCy = size.cy + gapY;
				schild->SetSize(schild->GetSize().cx, nCy);
				schild->m_fixW = true;
				schild->m_fixH = false;
			}
			else if (size.cy == 0)
			{
				nCx = size.cx + gapX;
				schild->SetSize(nCx, schild->GetSize().cy);
				schild->m_fixW = false;
				schild->m_fixH = true;
			}
			else
			{
				nCx = size.cx + gapX;
				nCy = size.cy + gapY;
				schild->SetSize(nCx, nCy);
			}
		}
		else
		{
			nCx = size.cx + gapX;
			//nCy = size.cy + gapY+4; 
			nCy = size.cy + gapY + 8;
			schild->SetSize(nCx, nCy);
		}

		CView*	view = schild->GetActiveView();
		schild->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
		if (viewType == vtypeSCR)
			((CAxScrollView *)view)->SetSizeView(size.cx, size.cy);
		if (schild->IsTabView())	
			((CAxTabView *) view)->SizeWindow(size);
		return;
	}

	if (child->IsTabView())	
		gapY +=	TABH;

	viewType = child->m_vwTYPE & vtypeMSK;
	if (viewType == vtypeRSZ)
	{
		if (size.cx == 0)
		{
			nCx = child->GetSize().cx;
			nCy = size.cy + gapY;
			child->SetSize(nCx, nCy);
			child->m_fixW = true;
			child->m_fixH = false;
		}
		else if (size.cy == 0)
		{
			nCx = size.cx + gapX;
			nCy = child->GetSize().cy;
			child->SetSize(nCx, nCy);
			child->m_fixW = false;
			child->m_fixH = true;
		}
		else
		{
			nCx = size.cx + gapX;
			nCy = size.cy + gapY;
			//nCx = size.cx + 2;
			//nCy = size.cy + CAPTION_HEIGHT + 4;
			child->SetSize(nCx, nCy);
		}
	}
	else
	{
		nCx = size.cx + gapX;
		nCy = size.cy + gapY;
		
		child->SetSize(nCx, nCy);
	}

	CView*	view = child->GetActiveView();
	
	if (child->m_bIconic) 
	{
		
		child->SetWindowPos(NULL, child->m_restoreRc.left, child->m_restoreRc.top, child->m_restoreRc.Width(),
				child->m_restoreRc.Height(), SWP_SHOWWINDOW);
		child->SetRestore();
		return;
	}
	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);

	if (viewType == vtypeSCR)
		((CAxScrollView *)view)->SetSizeView(size.cx, size.cy);
	if (child->IsTabView())	
		((CAxTabView *)view)->SizeWindow(size);
}

void CMainFrame::firstTabOrder()
{
	long		rc = 0;
	const CChildFrame*	child = (CChildFrame*) MDIGetActive();
	if (!child)	return;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(homeEV, child->m_key), 0);
}

bool CMainFrame::HotKey(WPARAM wParam, LPARAM lParam)
{
	CString mapN(m_tMenu->GetMap(CString((const char*)lParam)));
	
	m_mapHelper->ChangeChild(mapN);
	return true;
}

LONG CMainFrame::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
{
	CString	info = m_bar1->screenInfo();
	CString desc = m_tMenu->GetDesc(info);

	info += "\t" + desc;
	CopyMemory((char *)lParam, info, info.GetLength());
	return 0;
}

void CMainFrame::GoToTray()
{
	ShowWindow(SW_HIDE);
	//ShowSingleWindows(FALSE);  //SDI 모드일때 각각 childframe 를 보여주거나 안보여주거 한다.

	bool	bConclusion = false, bKobaElw = false;
	if (m_conclusion->GetStyle() & WS_VISIBLE)
	{
		bConclusion = true;
		m_conclusion->ShowWindow(SW_HIDE);
	}
	if (m_kobanotify->GetStyle() & WS_VISIBLE)
	{
		bKobaElw = true;
		m_kobanotify->ShowWindow(SW_HIDE);
	}

	if (m_appMode == MODE_SDI)
		ShowSingleWindows(FALSE);

	PostMessage(WM_CANCELMODE);
	m_appMode = MODE_TRAY;
	TrayCmd(TRAY_SHOWICON);

}

void CMainFrame::BackFromTray()
{
	TrayCmd(TRAY_HIDEICON);

	//m_conclusion->ShowWindow(SW_SHOW);
	//m_kobanotify->ShowWindow(SW_SHOW);

	//if(m_appMode == MODE_SDI) 
	//우선 여기서는 모드가 MODE_TRAY 이지만 다시원복될때 SDI인지 MDI인지 알면 ShowSingleWindows 를 굳히 호출안해도 된다
	//하지만 ShowSingleWindows 함수자체가 SDI 화면이 있으면 작동되기  때문에 MDI 모드에서 호출해도 문제는 없다
	ShowSingleWindows(TRUE);  

	ShowWindow(SW_SHOW);
}

void CMainFrame::LockProg()
{
// 	if(m_bCertLogin)
// 	{
// 		Axis::MessageBox(this, "인증로그인시에는 화면잠금을 사용 할 수 없습니다.", MB_ICONINFORMATION);
// 		return;
// 	}

	const bool	visblebar = false;
	m_block = true;

	CString pass;

	if (m_bCertLogin)
	{
		CLockPass lockpassDlg(this);
		//if(lockpassDlg.DoModal() == IDCANCEL)
		//{
		//	Axis::MessageBox(this, "화면 잠금을 취소했습니다.", MB_ICONINFORMATION);
		//	return;
		//}
		ShowWindow(SW_HIDE);
		ShowSingleWindows(FALSE);

		bool	bConclusion = false, bKobaElw = false;
		if (m_conclusion->GetStyle() & WS_VISIBLE)
		{
			bConclusion = true;
			m_conclusion->ShowWindow(SW_HIDE);
		}
		if (m_kobanotify->GetStyle() & WS_VISIBLE)
		{
			bKobaElw = true;
			m_kobanotify->ShowWindow(SW_HIDE);
		}

		KillTimer(TM_IDLE);

		lockpassDlg.DoModal();

		if (bConclusion)
			m_conclusion->ShowWindow(SW_SHOW);
		if (bKobaElw)
			m_kobanotify->ShowWindow(SW_SHOW);

		m_noActive = false;
		ShowWindow(SW_SHOW);
		ShowSingleWindows(TRUE);
		m_noActive = true;
		m_block = false;

		if (m_idletime > 0)
		{
			OutputDebugString("[TM_IDLE] settimer 2");
			SetTimer(TM_IDLE, m_idletime * 60000, NULL);  //60000
		}

		return;
	}
	
	pass = m_pass;

	CLockDlg lockDlg(NULL, Axis::userID, pass);
	ShowWindow(SW_HIDE);
	ShowSingleWindows(FALSE);

	bool	bConclusion = false, bKobaElw = false;
	if (m_conclusion->GetStyle() & WS_VISIBLE)
	{
		bConclusion = true;
		m_conclusion->ShowWindow(SW_HIDE);
	}
	if (m_kobanotify->GetStyle() & WS_VISIBLE)
	{
		bKobaElw = true;
		m_kobanotify->ShowWindow(SW_HIDE);
	}

	KillTimer(TM_IDLE);
	
	if(m_bCertLogin)
#ifdef DF_USE_CPLUS17
		lockDlg.SetWizard(m_wizard.get());
#else
		lockDlg.SetWizard(m_wizard);
#endif


	lockDlg.DoModal();
	if (bConclusion)
		m_conclusion->ShowWindow(SW_SHOW);
	if (bKobaElw)
		m_kobanotify->ShowWindow(SW_SHOW);
	
	m_noActive = false;
	ShowWindow(SW_SHOW);
	ShowSingleWindows(TRUE);
	m_noActive = true;
	m_block = false;

	if (m_idletime > 0)
		SetTimer(TM_IDLE, m_idletime*60000, NULL);
}

void CMainFrame::CreateHistoryBar()
{
	CString strRet(_T(""));
	long rc;

	CString symbol = "1301";

	//double starttime = GetTickCount();
	
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(getHISTORY, 0), (LPARAM)symbol.operator LPCTSTR());

// 	double endtime = GetTickCount();
// 
// 	double spend = endtime - starttime;

	//strRet = rc ? (char *)rc : _T("");

	strRet = (char*)rc;

	if(strRet.IsEmpty())
	{
		return;
	}

	CString	str;
	int	find{};
	
	while (TRUE) {
		find = strRet.Find('\t');
		
		if (find < 1) 
		{
			//OutputDebugString("NOT FOUND\n");
			break;
		}
		
		str = strRet.Left(find);
		strRet.Delete(0, find + 1);
		
		find = str.Find(" ");
		if (find > 0) {
			//_JCode jCodeH;
			CString code = str.Left(find);
			CString name = str.Mid(find + 1);

			m_bar3->add_History(code,name);
		}
	}
}

void CMainFrame::createUserScreen(int key)
{
	char	wb[512];
	CString	file, tmps;

	tmps = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, tmps, tmps);

	tmps.Format("%02d", key);
	const DWORD dwRc = GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), file);
	if (dwRc <= 0)	return;

	tmps = wb;
	createUserScreen(tmps);
}

void CMainFrame::createUserScreen(CString mapN, bool allVS)
{
	m_iAxisState = AXIS_STATE_CREATEUSERSCREEN;
	m_slog.Format("--------------------[Axis_State][createUserScreen][cx_account]  start-------------");
	OutputDebugString(m_slog);

	int	pos{}, key{}, trigger{}, sdi{};
	int    vsN{};  //vc2019
	char	wb[512]{};
	CString	file, tmps, tmpx;
	CString	data, mapName;
	WINDOWPLACEMENT wndpl{};
	
	tmps = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, tmps, tmps);

	m_n2022Cnt = 0;
	
	//m_mapHelper->closeAll(false);
	m_mapHelper->closeAll(true);
	
	DWORD dwRc{};
	CArray	< int , int > ary;

	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6 && allVS; vsN++)
	{
		if (vsN == m_vsN)	continue;
		ary.Add(vsN);
	}
	ary.Add(m_vsN);

	CString s;
	
	WriteLog("[AXIS] CreateUserScrren========================\n");
	// 어떤 맵이 포커스를 가져야하는지에 대한 정보
	// 2013.01.02 김덕기
// 	dwRc = GetPrivateProfileString(mapN, "LASTSTATFOCUS", "", wb, sizeof(wb), file);
// 	
// 	CString activeKey;
// 	
// 	activeKey = wb;
	/*m_bLoadScreen = true;*/
	
	//int focusKey;
	//////////////////////////////////////////////////
	bool bpopCDD = false;
	bool	showChild = false;
	for (;ary.GetSize();)
	{
		vsN = ary.GetAt(0);
		ary.RemoveAt(0);
		
		if (allVS)
		{
			long	rc = 0;
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setVID, 0), MAKELONG(0, vsN));
		}
		
		if (!ary.GetSize())	showChild = true;

		for (int ii = 1; ; ii++)
		{
			tmpx.Format("%d%02d", allVS ? vsN : 0, ii);
			dwRc = GetPrivateProfileString(mapN, tmpx, "", wb, sizeof(wb), file);
			if (dwRc <= 0)	break;
			
			data  = wb;
			data.Replace(";", "\n");

			CString strTag;

			for (int jj = 0; jj < 15; jj++)
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
				case 4:	wndpl.showCmd = atoi(tmpx);			break;	// .showCmd
				case 5:	wndpl.ptMinPosition.x = atoi(tmpx);		break;	// .ptMinPosition.x
				case 6:	wndpl.ptMinPosition.y = atoi(tmpx);		break;	// .ptMinPosition.y
				case 7:	wndpl.ptMaxPosition.x = atoi(tmpx);		break;	// .ptMaxPosition.x
				case 8:	wndpl.ptMaxPosition.y = atoi(tmpx);		break;	// .ptMaxPosition.y
				case 9:	wndpl.rcNormalPosition.left = atoi(tmpx);	break;	// .rcNormalPosition.left
				case 10:wndpl.rcNormalPosition.top  = atoi(tmpx);	break;	// .rcNormalPosition.top
				case 11:wndpl.rcNormalPosition.right  = atoi(tmpx);	break;	// .rcNormalPosition.right
				case 12:wndpl.rcNormalPosition.bottom = atoi(tmpx);	break;	// .rcNormalPosition.bottom
				case 13:trigger = atoi(tmpx);				break;	// trigger Group
				case 14:							// symbol data
					if (!tmpx.IsEmpty())
					{
						int pos;
						pos = tmpx.Find(";");
						if ( pos >= 0)
						{
							mapName += tmpx.Left(pos);
						}
						
						strTag = tmpx;
						strTag.TrimRight();
						strTag.TrimLeft();
					}
					break;
				}
			}
#ifdef DF_CDDUSE
			bpopCDD = isCDDScreen(mapName);
#endif
			if (IsNoSaveLastmap(mapName)) continue;
			if (ExceptMap(mapName.Left(L_MAPN)))	continue;
			
			if (sdi || m_bSDI)
				key = m_mapHelper->CreateSDI(mapName, trigger, wndpl, showChild, vsN);
			else
				key = m_mapHelper->CreateChild(mapName, trigger, wndpl, showChild, vsN, strTag);
			
		}
	}

#ifdef DF_CDDUSE
	if (bpopCDD)  //test CDD
	{

	}
#endif
	m_bLoadScreen = true;

	m_slog.Format("--------------------[Axis_State]=[%d]-------------", m_iAxisState);
	OutputDebugString(m_slog);

	/*auto f = std::async(std::launch::async | std::launch::deferred, [this]() {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		PostMessage(WM_USER, MMSG_SETMAIN_STATE, 0);
		});*/

	/*std::thread([this]() {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		PostMessage(WM_USER, MMSG_SETMAIN_STATE, 0);
		}).detach();*/


	char buff[256]{};
	file.Format("%s\\%s\\AXISFILE.INI", Axis::home, TABDIR);
	GetPrivateProfileString("STAFFACCPOP", "time", "5000", buff, sizeof(buff), file);
	tmps.Format("%s", buff);
	tmps.TrimRight();

	SetTimer(TM_STAFF_OPENNOPOACC, atoi(tmps), nullptr);

	m_slog.Format("--------------------[Axis_State]m_iAxisState =[%d] tmps=[%s]-------------", m_iAxisState, tmps);
	OutputDebugString(m_slog);
}

void CMainFrame::saveUserScreen()
{
	char	wb[1024*4];
	CString	file, filex, path;
	CString	tmps, tmpx, str;

	tmps = Axis::user;
	//MessageBox(Axis::home);
	file.Format("%s\\%s\\%s\\%s.ini",  Axis::home, USRDIR, tmps, tmps);
	filex.Format("%s\\%s\\%s\\%s.tmp", Axis::home, USRDIR, tmps, tmps);

	CScreenDlg dlg(this, Axis::home);
	dlg.DoModal();

// last screen
	GetPrivateProfileSection("LASTSTAT", wb, sizeof(wb), file);
	WritePrivateProfileSection("LASTSTAT", wb, filex);
// first screen for all virtual screen
	GetPrivateProfileSection("STARTVSMAP", wb, sizeof(wb), file);
	WritePrivateProfileSection("STARTVSMAP", wb, filex);

	DeleteFile(filex);
	makeUserScreenMenu();
}

void CMainFrame::makeUserScreenMenu()
{
	
	char	wb[512]{};
	int	count{};
	CString	file, tmps, value;

	tmps = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, tmps, tmps); 

	const CMenuXP*	mainM = (CMenuXP*) m_tMenu->GetPopupMenu(0);
	CMenuXP*	thisM = (CMenuXP*) mainM->GetSubMenu(UMENUN);	// fix menu position
	if (!thisM)	
		return;

	count = (int)thisM->GetMenuItemCount();
	for (int ii = 0; ii < count - 2; ii++)
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
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		const DWORD dwRc = GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), file);
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
	int	pos{}, key{}, index{};
	CString	mpN, tmps, data, info;

	CProfile profile(pkUserSetup);

	if (add)
	{
		pos  = keys.Find('=');
		tmps = keys.Left(pos);
		keys = keys.Mid(pos+1);
		profile.Write("ROOT", tmps, keys);

		WINDOWPLACEMENT	wndpl;
		CArray	<int, int>	aryK;

		wndpl.length = sizeof(WINDOWPLACEMENT);
		profile.WriteSection(keys, "");
		
		CChildFrame*	child  = NULL;
		CSChild*	schild = NULL;
		for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
		{
			if (!allvs && vsN != m_vsN)	continue;

			for (int ii = m_hooklist[vsN].GetSize() - 1; 0 <= ii; ii--)
			{
				key = m_hooklist[vsN].GetAt(ii);
				aryK.Add(key);
			}
			CMapStringToString mapDRFN;
			mapDRFN.RemoveAll();

			int ii{};
			for (index = 1,  ii = 0; ii < aryK.GetSize(); ii++)
			{
				key = aryK.GetAt(ii);
				if (!m_arMDI[vsN].Lookup(key, child))	continue;
				if (ExceptMap(child->m_mapN))		continue;

				mpN.Format("%s", child->m_mapN.Left(L_MAPN));
				if (!ExistMenu(mpN))	continue;

				data.Empty();
				child->GetWindowPlacement(&wndpl);
				getScreenData(child->m_key, data);				
				// dll data append.........
				if (m_mapHelper->IsDLL(child->m_mapN))
				{
					const CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
					if (base)
					{
						if(m_mapHelper->IsDRFN(child->m_mapN))
						{
							int nCount = profile.GetInt("USERDRFN","COUNT");

							if(nCount == 0)
							{
								nCount = 1;

								profile.Write("USERDRFN","COUNT",1);
							}
							else
							{
								nCount++;

								profile.Write("USERDRFN","COUNT",nCount);
							}

							CString usrdata;
							usrdata.Format("USERSCREEN%08d",nCount);

							CString tmp;
							CString strValue;
							
							if(mapDRFN.Lookup(child->m_mapN,tmp))
							{
								const int n = atoi(tmp);
								
								tmp.Format("%d",n+1);
								mapDRFN.SetAt(child->m_mapN,tmp);
								
								tmp.Format(",%d%02d",vsN,n+1);
								
								tmp.Insert(2,",");
								tmp.Insert(4,",");
								
								strValue = usrdata + tmp;
							}
							else
							{
								tmp.Format("%d",1);
								mapDRFN.SetAt(child->m_mapN,tmp);
								
								strValue = usrdata + ",0,0,1";
							}
							
							char	wb[256];
							sprintf(wb, strValue);
							base->SendMessage(WM_DLLDATA, 0, (long) &wb);
							if (!data.IsEmpty())	data += ";";
							tmps.Format("%s\t%s", USERDEFSYM, wb);
							data = tmps;
						}
						else
						{
							char	wb[256];
							sprintf(wb, keys);
							base->SendMessage(WM_DLLDATA, 0, (long) &wb);
							if (!data.IsEmpty())	data += ";";
							tmps.Format("%s\t%s", USERDEFSYM, wb);
							data += tmps;
						}
					}
				}

				tmps.Format("%d%02d", m_saveALLVS ? vsN : 0, index++);
				if (child->IsIconic())		// 20070723
				{
					CRect	rctmp;
					rctmp = child->GetRestoreRC();
					wndpl.rcNormalPosition.left = rctmp.left;
					wndpl.rcNormalPosition.right = rctmp.right;
					wndpl.rcNormalPosition.top = rctmp.top;
					wndpl.rcNormalPosition.bottom = rctmp.bottom;
				}
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
						wndpl.rcNormalPosition.bottom,
						child->m_xcaption.GetGroup(),
						data);
				profile.Write(keys, tmps, info);
			}
			aryK.RemoveAll();

			for (ii = m_sdiZORDER[vsN].GetSize() - 1; 0 <= ii; ii--)
			{
				key = m_sdiZORDER[vsN].GetAt(ii);
				aryK.Add(key);
			}
			
			for (ii = 0; ii < aryK.GetSize(); ii++)
			{
				key = aryK.GetAt(ii);
				if (!m_arSDI[vsN].Lookup(key, schild))	continue;
				if (ExceptMap(schild->m_mapN))		continue;
				
				mpN.Format("%s", schild->m_mapN.Left(L_MAPN));
				if (!ExistMenu(mpN))	continue;

				data.Empty();
				schild->GetWindowPlacement(&wndpl);
				getScreenData(schild->m_key, data);

				// dll data append.........
				if (m_mapHelper->IsDLL(schild->m_mapN))
				{
					const CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
					if (base)
					{
						char	wb[256];
						sprintf(wb, keys);
						base->SendMessage(WM_DLLDATA, 0, (long) &wb);
						if (!data.IsEmpty())	data += ";";
						tmps.Format("%s\t%s", USERDEFSYM, wb);
						data += tmps;
					}
				}

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
						wndpl.rcNormalPosition.bottom,
						schild->m_xcaption.GetGroup(),
						data);
				profile.Write(keys, tmps, info);
			}
			aryK.RemoveAll();
		}
		return true;
	}

	CStringArray	arr;
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		if (!tmps.CompareNoCase(keys))
			continue;

		tmps = profile.GetString("ROOT", tmps);
		if (tmps.IsEmpty())
			break;
		arr.Add(tmps);
	}

	profile.WriteSection("ROOT", "");
	
	for (int ii = 0; ii < arr.GetSize(); ii++)
		profile.Write("ROOT", Format("%02d", ii + 1), arr.GetAt(ii));

	arr.RemoveAll();
	return true;
}

void CMainFrame::ClearUserIni()
{
	CString date;
	const CTime tm = CTime::GetCurrentTime() + CTimeSpan(-5,0,0,0);

	date.Format("%04d%02d%02d",tm.GetYear(),tm.GetMonth(),tm.GetDay());
	
	CString strPath = Format("%s\\user\\%s\\%s.ini.*", Axis::home, Axis::user, Axis::user);
	CString strStd = Format("%s.ini.%s", Axis::user, date);
	
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(strPath, &wfd);

	int cnt = 0;
	
	while(hFind!=INVALID_HANDLE_VALUE)
	{
		CString s;
		CString strFile = Format("%s\\user\\%s\\%s", Axis::home, Axis::user, wfd.cFileName);
		if (Format("%s.ini",Axis::user) != wfd.cFileName && strStd > wfd.cFileName)
		{
			s.Format("USER INI [%s]\n",wfd.cFileName);
			OutputDebugString(s);
			DeleteFile(strFile);
		}

		cnt++;
		
		if (!FindNextFile(hFind, &wfd)) 
		{
			FindClose(hFind);
			break;
		}
	}
}

void CMainFrame::save_laststat()
{
	int	key{}, index{};
	CString	mpN, tmps, data, info, keys = "LASTSTAT";
	CString date;

	CTime time;
	time = CTime::GetCurrentTime();

	date.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

	CString orginF = Format("%s\\user\\%s\\%s.ini", Axis::home, Axis::user, Axis::user);
	CString copyF = Format("%s\\user\\%s\\%s.ini.%s", Axis::home, Axis::user, Axis::user, date);
	CopyFile(orginF, copyF, FALSE);

	CProfile profile(pkUserSetup);
	profile.Write("ROOT", "00", keys);

	WINDOWPLACEMENT	wndpl;
	CArray	<int, int>	aryK;

	wndpl.length = sizeof(WINDOWPLACEMENT);
	profile.WriteSection(keys, "");
	profile.WriteSection("LASTSTATCOUNT", "");
	profile.WriteSection("LASTSTATSTATUS", "");

	CString strSendvsN, strTotalLast, strResult;

	m_saveALLVS = true;

	strResult = "";

	CChildFrame* child = NULL;
	CSChild* schild = NULL;
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		strSendvsN = "";

		CString vsnS, lsc;
		vsnS.Format("%d", vsN);

		CString strLog;

		strLog.Format("SAVEALL [%d] CVSN [%d] vsN [%d]", m_saveALLVS, m_vsN, vsN);
		profile.Write("LASTSTATSTATUS", vsnS, strLog);

		if (!m_saveALLVS && vsN != m_vsN)	continue;

		for (int ii = 0; ii < m_bar2->m_arButton[vsN].GetSize(); ii++)
		{
			//const _button* btn = m_bar2->m_arButton[vsN].GetAt(ii); //vc2019? test 
			auto  btn = m_bar2->m_arButton[vsN].GetAt(ii);

			key = btn->key;
			aryK.Add(key);
		}

		lsc.Format("%d", m_bar2->m_arButton[vsN].GetSize());
		profile.Write("LASTSTATCOUNT", vsnS, lsc);

		CMapStringToString mapDRFN;
		mapDRFN.RemoveAll();
		int ii{};
		for (index = 1, ii = 0; ii < aryK.GetSize(); ii++)
		{
			key = aryK.GetAt(ii);
			if (!m_arMDI[vsN].Lookup(key, child))	continue;
			strLog.Format("[%d] KEY [%d]", ii, key);
			profile.Write("LASTSTATSTATUS", vsnS, strLog);
			if (!child->GetSafeHwnd())		continue;
			strLog.Format("SAFE HANDLE [%s] [%d]", child->m_mapN, (int)child->GetSafeHwnd());
			profile.Write("LASTSTATSTATUS", vsnS, strLog);
			if (ExceptMap(child->m_mapN))		continue;
			strLog.Format("EXCEPT MAP [%s]", child->m_mapN);
			profile.Write("LASTSTATSTATUS", vsnS, strLog);

			if (IsNoSaveLastmap(child->m_mapN)) continue;

			mpN = child->m_mapN.Left(L_MAPN);

			if (mpN != "IB715100" && mpN != "IB715200")
			{
				if (!m_mapHelper->IsValidMap(mpN) || !ExistMenu(mpN))
				{
					mpN = mpN.Left(L_MAPN - 2) + "00";
					if (!m_mapHelper->IsValidMap(mpN) || !ExistMenu(mpN))
						continue;
				}
			}

			strLog.Format("VALID MAP [%s]", mpN);
			profile.Write("LASTSTATSTATUS", vsnS, strLog);

			data.Empty();
			child->GetWindowPlacement(&wndpl);
			getScreenData(child->m_key, data);

			// dll data append.........
			if (m_mapHelper->IsDLL(child->m_mapN))
			{
				//DLL 화면들은 특별히 OnSize를 처리해 주지 않으면 최소화 처리가 제대로 안됨
				//그래서 DLL 화면들은 종료시 원복 후 저장 처리.특별히 차트가 문제
				//2016.02.11 - dkkim
				if (child->m_bIconic)
				{
					child->actionCaption(IDX_RESTORE, CPoint(0, 0));
				}
				/////////////////////////////////////////////////////////////////////////////

				const CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
				if (base)
				{
					if (m_mapHelper->IsDRFN(child->m_mapN))
					{
						CString tmp;
						CString strValue;

						if (mapDRFN.Lookup(child->m_mapN, tmp))
						{
							const int n = atoi(tmp);

							tmp.Format("%d", n + 1);
							mapDRFN.SetAt(child->m_mapN, tmp);

							tmp.Format(",%d%02d", vsN, n + 1);

							tmp.Insert(2, ",");
							tmp.Insert(4, ",");

							strValue = "LASTSTAT" + tmp;
						}
						else
						{
							tmp.Format("%d", 1);
							mapDRFN.SetAt(child->m_mapN, tmp);

							strValue = "LASTSTAT,0,0,1";
						}

						char	wb[256];
						sprintf(wb, strValue);
						base->SendMessage(WM_DLLDATA, 0, (long)&wb);
						if (!data.IsEmpty())	data += ";";
						tmps.Format("%s\t%s", USERDEFSYM, wb);
						data = tmps;
					}
					else
					{
						char	wb[256];
						sprintf(wb, keys);
						base->SendMessage(WM_DLLDATA, 1, (long)&wb);
						if (!data.IsEmpty())	data += ";";
						tmps.Format("%s\t%s", USERDEFSYM, wb);
						data += tmps;
					}
				}
			}

			if (child->IsIconic())
			{
				CRect	rctmp;
				rctmp = child->GetRestoreRC();
				wndpl.rcNormalPosition.left = rctmp.left;
				wndpl.rcNormalPosition.right = rctmp.right;
				wndpl.rcNormalPosition.top = rctmp.top;
				wndpl.rcNormalPosition.bottom = rctmp.bottom;
			}

			tmps.Format("%d%02d", m_saveALLVS ? vsN : 0, index++);
			info.Format("%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|",
				child->m_mapN,
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
				wndpl.rcNormalPosition.bottom,
				child->m_xcaption.GetGroup(),
				data);

			profile.Write(keys, tmps, info);

			strSendvsN += child->m_mapN + " ";

			// 			if(key == m_activeKey)
			// 			{
			// 				profile.Write(keys,"LASTSTATFOCUS",tmps);
			// 			}
		}
		aryK.RemoveAll();

		for (POSITION spos = m_arSDI[vsN].GetStartPosition(); spos; )
		{
			m_arSDI[vsN].GetNextAssoc(spos, key, schild);
			aryK.Add(key);
		}

		for (ii = 0; ii < aryK.GetSize(); ii++)
		{
			key = aryK.GetAt(ii);
			if (!m_arSDI[vsN].Lookup(key, schild))	continue;
			if (ExceptMap(schild->m_mapN))		continue;

			mpN.Format("%s", schild->m_mapN.Left(L_MAPN));
			if (!ExistMenu(mpN))	continue;

			data.Empty();
			schild->GetWindowPlacement(&wndpl);
			getScreenData(schild->m_key, data);

			// dll data append.........
			if (m_mapHelper->IsDLL(schild->m_mapN))
			{
				const CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
				if (base)
				{
					char	wb[256];
					sprintf(wb, keys);
					base->SendMessage(WM_DLLDATA, 1, (long)&wb);
					if (!data.IsEmpty())	data += ";";
					tmps.Format("%s\t%s", USERDEFSYM, wb);
					data += tmps;
				}
			}

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
				wndpl.rcNormalPosition.bottom,
				schild->m_xcaption.GetGroup(),
				data);
			profile.Write(keys, tmps, info);

			strSendvsN += mpN + " ";

			// 			if(key == m_activeKey)
			// 			{
			// 				profile.Write(keys,"LASTSTATFOCUS",tmps);
			// 			}
		}
		aryK.RemoveAll();

		if (vsN < 6)
		{
			CString tmpVsN;
			tmpVsN.Format("%03d", vsN);

			//struct	_pidouini_mid	uini;

			//FillMemory((char*) &uini, sizeof(_pidouini_mid), ' ');
			CString sdat;

			//sval.Format("%s INSTALL PATH [%s]\n",str,Axis::home);
			//==================================================
			CString s;
			// 	s.Format("SEND CONFIG : [%s]\n",sval);
			// 	OutputDebugString(s);

			if (strSendvsN == "")
			{
				strSendvsN = "Nothing";
			}

			CTime	time;
			time = time.GetCurrentTime();
			sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
			CString fname = Axis::user + ".ini";

			CString strTmp;
			//strTmp = CString((char*)&uini,sizeof(struct _pidouini_mid));

			strTmp.Format("%c%-8s%-100s%-100s%-100s%-2000s%-8s", 'I', Axis::userID, fname, "LASTSTAT", tmpVsN, strSendvsN, sdat);

			// 			s.Format("LAST SEND [%s]\n",strTmp);
			// 			OutputDebugString(s);

			strResult += strTmp;
		}
	}

	strTotalLast.Format("A06%s", strResult);

	// 	CString s;
	// 	s.Format("LAST STAT : [%s]\n",strTotalLast);
	// 	OutputDebugString(s);

	if (strlen(strTotalLast) < (1024 * 15))
	{
		SendLastStat((char*)(const char*)strTotalLast);
	}

	// 바탕화면 공지사항 mode
	CProfile p(pkEnvironment);
	p.Write("BKNOTICE", "MODE", Format("%d", m_nBkMode));

	// scr_report() 함수로 대체함.
	// SendConfig();

	 //SendInstallPath();

	os_report();

	CString ips;

	if (m_forceIP.IsEmpty() == FALSE)
	{
		SendForceIP();
	}

	CString		filename;
	//에러로그 안올리게 차단
// 	 CFile cfile;
// 
// 	 filename.Format("%s\\log", Axis::home);
// 
// 	 if(_taccess(filename,04)) return;
// 
// 	 if (cfile.Open(filename, CFile::modeRead|CFile::typeBinary))
// 	 {
// 		 int	fileL = cfile.GetLength();
// 		 char*	sndB = new char [fileL + 1];
// 		 
// 		 ZeroMemory(sndB, fileL + 1);
// 		 cfile.Read(sndB, fileL);
// 		 cfile.Close();
// 		
// 		 CString errMsg(sndB,fileL);
// 
// 		 trouble_shooting(errMsg,"ERRORLOG");
// 	 }
// 
// 	 ::DeleteFile(filename);

// 	 filename.Format("%s\\menulog", Axis::home);
// 	 
// 	 if(_taccess(filename,04)) return;
// 	 
// 	 if (cfile.Open(filename, CFile::modeRead|CFile::typeBinary))
// 	 {
// 		 int	fileL = cfile.GetLength();
// 		 char*	sndB = new char [fileL + 1];
// 		 
// 		 ZeroMemory(sndB, fileL + 1);
// 		 cfile.Read(sndB, fileL);
// 		 cfile.Close();
// 		 
// 		 CString errMsg(sndB,fileL);
// 		 
// 		 trouble_shooting(errMsg,"MENULOG");
// 	 }
// 	 
// 	 ::DeleteFile(filename);
	 //2012.09.18 김덕기 - 2000번에서 실시간잔고 조회건수 전송
// 	 if(Axis::userID == "dundas" || Axis::userID == "yale8700")
// 	 {
// 		Send2000();
// 	 }

	CString strFile;
	strFile.Format("%s\\tab\\TOP10.ini", Axis::home);
	if (m_top10 != nullptr && m_top10->IsWindowVisible())
		WritePrivateProfileString("TOP10", "AUTOPOPUP", "1", strFile);
	else
		WritePrivateProfileString("TOP10", "AUTOPOPUP", "0", strFile);
}
#pragma warning (disable : 26400)
#pragma warning (disable : 26409)
#pragma warning (disable : 6001)
#pragma warning (disable : 6387)
#pragma warning (disable : 26401)

void CMainFrame::getScreenData(int key, CString& data, BOOL save)
{
	CString		value, tmps, symbol = "1301, 30301, 40401, ed_focod";

	CProfile profile(GetProfileFileName());
	symbol = profile.GetString("LinkEx", "codesymbols");

	CStringArray		ary;
	CMapStringToString	arSymbyData;
	
	parseAry(symbol, ary, ",");
	
	long	rc = 0;
	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		symbol = ary.GetAt(ii);
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(getFDC, key), (long)(const char*)symbol);

		value = (rc ? (char *) rc : _T(""));
		arSymbyData.SetAt(symbol, value);
	}

	if (save)
	{
		for (int ii = 0; ii < ary.GetSize(); ii++)
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
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			symbol = ary.GetAt(ii);
			arSymbyData.Lookup(symbol, value);
			value.TrimLeft();value.TrimRight();

			if (value.IsEmpty())	continue;
			arSymbylink.SetAt(symbol, value);
			tmps = profile.GetString("Link", symbol);
			if (!tmps.IsEmpty())
			{
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

		for (int ii = 0; ii < ary.GetSize(); ii++)
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

void CMainFrame::saveImg(int entire)
{
	PostMessage(WM_CANCELMODE);
	PostMessage(WM_AXIS, MAKEWPARAM(axWRITEIMG, entire));
	//writeImg(Axis::home);
}

BOOL CMainFrame::printImg(CString dat)
{	
	if(m_bInit == TRUE)
	{
		//m_bInit = FALSE;
		return TRUE;
	}

	CWnd* pChild{};
	CString	title, fontName;

	title.LoadString(ST_TEXT_OK);
	if (!m_bSDI)
	{
		CChildFrame* child = (CChildFrame*) MDIGetActive();
		if (!child)
		{
			Axis::MessageBox(this, "인쇄할 화면이 없습니다.", MB_ICONINFORMATION);
			return TRUE;
		}

		pChild = child;
	}
	else
	{
		CSChild*	schild;
		if (m_arSDI[m_vsN].Lookup(m_activeKey, schild))
		{
			pChild = schild;
		}
		else
		{
			Axis::MessageBox(this, "인쇄할 화면이 없습니다.", MB_ICONINFORMATION);
			return TRUE;
		}	
	}	

	// 2011.02.22 인쇄정보 추가 by LKM
	{
		const CTime tm = CTime::GetCurrentTime();
		
		dat.Format(
			"인쇄정보 : %s(PC기준), %s(%s)-%s\r\n"
			"본 화면내용은 참고자료로만 활용하시기 바라며,\r\n"
			"어떠한 경우에도 법적책임소재에 대한 증빙자료등으로 사용하실 수 없음을 알려드립니다.", 
			(LPCSTR)tm.Format("%Y.%m.%d %H:%M:%S"), Axis::user, Axis::userID, m_ipAddr
		);
	}
	
	CRect		rect;
	pChild->GetWindowRect(&rect);
	CRect	mRc, rc;
	
	//CString tmp; tmp.Format("width: %d, height: %d", rect.Width(), rect.Height());
	//AfxMessageBox(tmp);
	if (!m_bSDI)
	{
		rc.CopyRect(&rect);
		m_MClient->GetWindowRect(mRc);
		rect.IntersectRect (mRc, rect);
	}

	CSize	szOrg, szRect;
	bool	bResize = false;

	szOrg.cx = rect.Width();
	szOrg.cy = rect.Height();
	szRect = szOrg;
	
	if (szOrg.cx > MAX_XBMP || szOrg.cy > MAX_YBMP)
	{
		szRect.cx = MAX_XBMP;		
		szRect.cy = (int)((double)szOrg.cy * (double)szRect.cx / (double)szOrg.cx);
		if (szRect.cy > MAX_YBMP)
		{
			szRect.cy = MAX_YBMP;
			szRect.cx =  (int)((double)szOrg.cx * (double)szRect.cy / (double)szOrg.cy);
		}

		bResize = true;
	}	
	
	//CString tmp; tmp.Format("cx: %d, cy: %d", szRect.cx, szRect.cy);
	//AfxMessageBox(tmp);
	if (Axis::MessageBox(this, "출력 하시겠습니까", MB_YESNO) == IDYES)
	{
		if (bResize)
			pChild->SetWindowPos(NULL, 0, 0, szRect.cx, szRect.cy, SWP_NOMOVE|SWP_NOZORDER);

		m_pPRNChild = pChild;
		m_sizePRN = szRect;
		//m_sizePRN.cx -= 5;    //test print
		//m_sizePRN.cy -= 5;
		m_sizePRNORG = szOrg;
		m_bPRNResize = bResize;
		m_szPRNData.Format("%s", dat);

		SetTimer(TM_PRNIMG, 1000, NULL);
	}
	return TRUE;
}

BOOL CMainFrame::printOper(CWnd* pChild, CSize size, CSize sizeOrg, CString dat, bool bResize)
{
	CString	title, fontName;	
	title.LoadString(ST_TEXT_OK);

	CString		file;
	CBitmap 	bitmap;
	CWindowDC	dc(pChild);
	CDC 		memDC;

	//bitmap.CreateCompatibleBitmap(&dc, size.cx, size.cy);
	bitmap.CreateCompatibleBitmap(&dc, size.cx - 10, size.cy - 10);  //test print
	memDC.CreateCompatibleDC(&dc); 	
	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);	

	//memDC.BitBlt(0, 0, size.cx, size.cy, &dc, 0, 0, SRCCOPY); 
	 memDC.BitBlt(-5, -5, size.cx, size.cy, &dc, 0, 0, SRCCOPY); 
	// Create logical palette if device support a palette
	CPalette pal;
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		const UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
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
	if(hDIB == NULL)	
		return FALSE;

	file.Format("%s\\%d", Axis::home, (int)pChild);
	writeDIB((LPSTR)(LPCTSTR)file, hDIB);
	GlobalFree(hDIB);

	CNDib*	nDib = new CNDib();
	if (nDib->SetNDIB(file))
	{
		DeleteFile(file);
		delete nDib;
		Axis::MessageBox(this, "Error print", MB_ICONINFORMATION);
		return FALSE;
	}

	HANDLE	pDib = nDib->GetNDIB();
	BITMAP	bm{};
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
		Axis::MessageBox(this, "Error Print", MB_ICONINFORMATION);
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
		Axis::MessageBox(this, "Error Print", MB_ICONINFORMATION);
		return FALSE;
	}

	const int	paperX = GetDeviceCaps(printDC.m_hDC, HORZRES);
	const int	paperY = GetDeviceCaps(printDC.m_hDC, VERTRES);

	const int	printX = GetDeviceCaps(printDC.m_hDC, LOGPIXELSX);
	const int	printY = GetDeviceCaps(printDC.m_hDC, LOGPIXELSY);
	const int	deviceX = GetDeviceCaps(pDC->m_hDC, LOGPIXELSX);
	const int	deviceY = GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);
	
	const int	width  = (int) (bm.bmWidth*printX/deviceX);
	const int	height = (int) (bm.bmHeight*printY/deviceY);
	const int	pagexN = (int) ((width+paperX-1)/paperX);
	const int	pageyN = (int) ((height+paperY-1)/paperY);

	int	xDest{}, yDest{}, nDestW{}, nDestH{};
	int	xSrc{}, ySrc{}, nSrcW{}, nSrcH{};

	::SetStretchBltMode(printDC.m_hDC, COLORONCOLOR);

	LPBITMAPINFOHEADER lpbi{};
	LPSTR        pBuf{};
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(pDib);
	if (!lpbi)
	{
		DeleteFile(file);
		printDC.DeleteDC();
		ReleaseDC(pDC);
		delete nDib;

		Axis::MessageBox(this, "Error Print", MB_ICONINFORMATION);
		return FALSE;
	}
	pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + nDib->PaletteSize(lpbi);

	CFont	Font, * font{};
	fontName.LoadString(ST_FONTNAME_DEFAULT);
	const int	fontS = 90*printX/deviceX;
	Font.CreatePointFont(fontS, fontName);
	font = printDC.SelectObject(&Font);
	int	ret{};
	if (pagexN == 1 && pageyN == 1)
	{
		if(StartPage(printDC.m_hDC))
		{
			xDest = paperX/2 - width/2;
			yDest = paperY/2 - height/2;
			ret = StretchDIBits(printDC.m_hDC, xDest, yDest, width ,height, 0, 0, 
				bm.bmWidth, bm.bmHeight, pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);

		/*	m_slog.Format("%d %d %d %d  %d %d", xDest, yDest, width, height, bm.bmWidth, bm.bmHeight);
			AfxMessageBox(m_slog);
			return 0;*/

			if (!dat.IsEmpty())
			{
// #if 0
// 				TextOut(printDC.m_hDC, xDest, yDest+height+20, dat, dat.GetLength());
// #else
// 				CRect rc;
// 				DrawText(printDC.m_hDC, dat, dat.GetLength(), &rc, DT_TOP|DT_LEFT|DT_CALCRECT);
// 				rc.OffsetRect(xDest, yDest+height-rc.top+20);
// 				DrawText(printDC.m_hDC, dat, dat.GetLength(), &rc, DT_TOP|DT_LEFT);
// #endif
				OutputDebugString("화면인쇄 하단 안내 인쇄");
				CRect rc;
				DrawText(printDC.m_hDC, dat, dat.GetLength(), &rc, DT_TOP|DT_LEFT|DT_CALCRECT);
				//rc.OffsetRect(xDest, yDest+height-rc.top+20);

				//rc.top = 4407;
				rc.top = 5;
				rc.left = 10;

				DrawText(printDC.m_hDC, dat, dat.GetLength(), &rc, DT_TOP|DT_LEFT);

// 				CString s;
// 				s.Format("BOTTOM FOOTER [%d] [%d]\n",rc.top,rc.left);
// 				OutputDebugString(s);
			}
			EndPage(printDC.m_hDC);
		}
	} // 사실상 이코드는 필요가 없다. 이것은 푸르덴셜당시 화면을 크게 인쇄할수 있도록 하기 위해 필요했던 옵션
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

				if (!dat.IsEmpty())
				{
					// #if 0
					// 				TextOut(printDC.m_hDC, xDest, yDest+height+20, dat, dat.GetLength());
					// #else
					// 				CRect rc;
					// 				DrawText(printDC.m_hDC, dat, dat.GetLength(), &rc, DT_TOP|DT_LEFT|DT_CALCRECT);
					// 				rc.OffsetRect(xDest, yDest+height-rc.top+20);
					// 				DrawText(printDC.m_hDC, dat, dat.GetLength(), &rc, DT_TOP|DT_LEFT);
					// #endif
					OutputDebugString("화면인쇄 하단 안내 인쇄");
					CRect rc;
					DrawText(printDC.m_hDC, dat, dat.GetLength(), &rc, DT_TOP|DT_LEFT|DT_CALCRECT);
					rc.OffsetRect(xDest, yDest+height-rc.top+20);
					DrawText(printDC.m_hDC, dat, dat.GetLength(), &rc, DT_TOP|DT_LEFT);
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
	if (bResize)
	{
		pChild->SetWindowPos(NULL, 0, 0, sizeOrg.cx, sizeOrg.cy, SWP_NOMOVE|SWP_NOZORDER);
	}
	return TRUE;
}

BOOL CMainFrame::writeImg(CString dir, int entire)
{
	// 2010.07.21 화면캡쳐시 메뉴잔상이 남는 현상이 있어 Invalidate -> 메세지펌핑 후에 캡쳐한다. by LKM
	{
		MSG msg;
		Invalidate(TRUE);
		while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)==TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CWnd*	pChild = NULL;
	CString	mapN;

	if (entire)	
		pChild = this;
	else
	{
		CChildFrame* child = (CChildFrame*) MDIGetActive();
		if (!child || child->IsWindowVisible() == FALSE)	
		{
			Axis::MessageBox(this, "실행중인 화면이 없습니다.", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		mapN = child->m_mapN;
		ASSERT(mapN.GetLength() >= L_MAPN);
		pChild = child;
	}

	if (!pChild) {
		Axis::MessageBox(this, "실행중인 화면이 없습니다.", MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	for (int ii = 0; ii < 10; ii++)	
		Sleep(10);

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
		const UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = 
			GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);

		// Create the palette
		pal.CreatePalette(pLP);

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// Convert the bitmap to a DIB
	HANDLE hDIB = writeDDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)	
	{
		Axis::MessageBox("bitmap to DIB Failed.", MB_OK | MB_ICONSTOP);
		return FALSE;
	}


	file.Format("%s\\%s.bmp", dir, entire ? "Full_Screen" : m_tMenu->GetMenuName(mapN.Left(L_MAPN)));
	file.Replace("/", "");
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
}

HANDLE CMainFrame::writeDDBToDIB(const CBitmap& bitmap, DWORD dwCompression, CPalette* pPal)
{
	BITMAP			bm{};
	BITMAPINFOHEADER	bi{};
	LPBITMAPINFOHEADER 	lpbi{};
	DWORD			dwLen{};
	HANDLE			hDIB{};
	HANDLE			handle{};
	HDC 			hDC{};
	HPALETTE		hPal{};

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
	int nColors = int(pow(2, bi.biBitCount));
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
	const BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
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
	BITMAPFILEHEADER	hdr{};
	LPBITMAPINFOHEADER	lpbi{};

	if (!hDIB)
		return FALSE;

	CFile file;
	if(!file.Open(szFile, CFile::modeWrite|CFile::modeCreate))
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	int nColors = int(pow(2, lpbi->biBitCount));
	if (nColors > 256)
		nColors = 0;

	// Fill in the fields of the file header 
	hdr.bfType	= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize	= GlobalSize (hDIB) + sizeof(hdr);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits	= (DWORD) (sizeof(hdr) + lpbi->biSize +	nColors * sizeof(RGBQUAD));

	// Write the file header 
	file.Write(&hdr, sizeof(hdr));

	// Write the DIB header and the bits 
	file.Write(lpbi, GlobalSize(hDIB));
	file.Close();
	return TRUE;
}

LPCTSTR	CMainFrame::getPrintName()
{
	PRINTDLG pd{};
	LPCTSTR	name{};
	CWinApp* app = AfxGetApp();

	pd.lStructSize = (DWORD) sizeof(PRINTDLG);
	if (!app->GetPrinterDeviceDefaults(&pd))
		return NULL;
	const LPDEVMODE pDevMode = (LPDEVMODE)::GlobalLock(pd.hDevMode);
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
	PRINTDLG pd{};
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

#pragma warning (default : 26400)
#pragma warning (default : 26409)
#pragma warning (default : 6001)
#pragma warning (default : 6387)
#pragma warning (default : 26401)

void CMainFrame::setUDP()
{
	BOOL		rc{};
	char		wb[128]{};
	int		value;
	CString		tmps = _T("");

	getConnectInfo(tmps, value);
	value = 15900;
	strcpy_s(wb, 128, tmps);
	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), listenUDP, (long)wb, value);
}

void CMainFrame::beginWait(int key)
{
	int	waitkey{};
	for (int ii = 0; ii < m_waitlist.GetSize(); ii++)
	{
		waitkey = m_waitlist.GetAt(ii);
		if (waitkey == key)
			m_waitlist.RemoveAt(ii--);
	}

	if (key & 0x80)
	{
		CMPop* Mpop{};
		CGPop* Gpop{};
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
	//m_slog.Format("[WAIT][%s]<%d> key=[%d] m_waitlist size = [%d]", __FUNCTION__, __LINE__, key, m_waitlist.GetSize());
	//OutputDebugString(m_slog);
	if (key == m_activeKey)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

void CMainFrame::endWait(int key)
{
	int	endkey{};
	for (int ii = 0; ii < m_waitlist.GetSize(); ii++)
	{
		endkey = m_waitlist.GetAt(ii);
		if (endkey == key)
		{
			m_waitlist.RemoveAt(ii--);
			//m_slog.Format("[WAIT][%s]<%d> key=[%d] 1. m_waitlist size = [%d]", __FUNCTION__, __LINE__, key, m_waitlist.GetSize());
			//OutputDebugString(m_slog);
		}
	}

	if (key & 0x80)
	{
		CMPop* Mpop{};
		CGPop* Gpop{};
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

	//m_slog.Format("[WAIT][%s]<%d> key=[%d] 2. m_waitlist size = [%d]", __FUNCTION__, __LINE__, key, m_waitlist.GetSize());
	//OutputDebugString(m_slog);
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CMainFrame::removeWait(int key)
{
	int	removekey{};
	for (int ii = 0; ii < m_waitlist.GetSize(); ii++)
	{
		removekey = m_waitlist.GetAt(ii);
		if (removekey == key)
			m_waitlist.RemoveAt(ii--);
	}
}

bool CMainFrame::IsSelectMap(CString mapN)
{
	CString smapN;

	CProfile profile(GetProfileFileName());
	smapN = profile.GetString("Select", mapN.Mid(2, 4));

	return !smapN.IsEmpty();
}

CString CMainFrame::getSMap(CString mapN)
{
	if (mapN.IsEmpty())	return _T("");

	CProfile profile(GetProfileFileName());
	CString smapN(profile.GetString("Select", m_tMenu->GetDispN(mapN)));
	if (smapN.IsEmpty())
		return mapN;
	else
		return mapN.Left(2) + smapN;
}

void CMainFrame::loadDomino(int key, CString mapN, bool bSDI)
{
	if (bSDI)
	{
		CSChild*	schild;
		if (!m_arSDI[m_vsN].Lookup(key, schild))	return;

		int	size, vwTYPE;
		m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vwTYPE,
					(BYTE *)(VTS_BSTR VTS_I4), getSMap(mapN), &size);

		const CRect	rect;
		CView*	view = schild->GetActiveView();

		int	gapX{}, gapY{};
		gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
		gapX += GetSystemMetrics(SM_CXBORDER) * 2;

		gapY  = GetSystemMetrics(SM_CYFRAME) * 2;
		gapY += GetSystemMetrics(SM_CYBORDER) * 2;
		if ((vwTYPE & vtypeMSK ) != vtypeWND)
			gapY += XCAP_HEIGHT;

		switch (vwTYPE)
		{
		case 0:
		default:
			if ((vwTYPE & vtypeMSK) == vtypeSCR)
				((CAxScrollView *)view)->SetInfo(key);//m_key = key;
			else	((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;
			break;
		}

		BOOL	rc = FALSE;
		m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
		if (!rc)	return;
		//m_bar1->set_Maps(mapN.Left(L_MAPN));
	}
	else
	{
		class CChildFrame* child;
		if (!m_arMDI[m_vsN].Lookup(key, child))	return;

		int	size, vwTYPE;
		m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vwTYPE,
					(BYTE *)(VTS_BSTR VTS_I4), getSMap(mapN), &size);

		const CRect	rect;
		CView*	view = child->GetActiveView();

		int	gapX{}, gapY{};
		gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
		gapX += GetSystemMetrics(SM_CXBORDER) * 2;

		gapY  = GetSystemMetrics(SM_CYFRAME) * 2;
		gapY += GetSystemMetrics(SM_CYBORDER) * 2;
		if ((vwTYPE & vtypeMSK ) != vtypeWND)
			gapY += XCAP_HEIGHT;

		switch (vwTYPE)
		{
		case 0:
		default:
			if ((vwTYPE & vtypeMSK) == vtypeSCR)
				((CAxScrollView *)view)->SetInfo(key);//m_key = key;
			else	((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;
			break;
		}

		BOOL	rc = FALSE;
		m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
		if (!rc)	return;
		m_bar1->set_Maps(mapN.Left(L_MAPN));
	}
}

void CMainFrame::process_ConSee(int option)
{
	CString	code, mapN;
	switch (option)
	{
#ifdef DF_USE_CPLUS17
	case CON_HIDE:	ShowControlBar(m_bar0.get(), FALSE, FALSE);	break;
#else
	case CON_HIDE:	ShowControlBar(m_bar0, FALSE, FALSE);	break;
#endif

	default:	return;
	}
}

// control bar code data receive
void CMainFrame::setCodeReg(int cnt, CString dat)
{
	CFile		file;
	CString		path, path_, code, name, string;
	path.Format("%s\\%s\\%s\\regCode", Axis::home, USRDIR, Axis::user);
	path_.Format("%s\\%s\\%s\\regCode.tmp", Axis::home, USRDIR, Axis::user);

	CopyFile(path, path_, FALSE);
	DeleteFile(path);
	if (!file.Open(path, CFile::modeCreate|CFile::modeWrite))
	{
		Axis::MessageBox(this, "Error. Don't save.", MB_ICONASTERISK);
		MoveFile(path_, path);
		return;
	}

	struct _regCode	regCode {};
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
		if (code.GetLength() > 12)
			code = code.Left(12);
		if (name.GetLength() >= 32)
			name = name.Left(31);	// last byte is null terminate
		name.Replace("%", "%%");

		sprintf(regCode.code, code);
		sprintf(regCode.name, name);

		file.Write(&regCode, sizeof(regCode));
	}

	file.Close();
	DeleteFile(path_);

	m_bar0->reload_Code();

#ifdef DF_USE_CPLUS17
	if (!(m_bar0->GetStyle() & WS_VISIBLE))
		ShowControlBar(m_bar0.get(), TRUE, FALSE);
#else
	if (!(m_bar0->GetStyle() & WS_VISIBLE))
		ShowControlBar(m_bar0, TRUE, FALSE);
#endif

}

void CMainFrame::ChangeUser()
{
	m_bExit = false;
	m_bReconnect = true;

	saveExitMap();
	PostMessage(WM_CLOSE);
}

void CMainFrame::RunWebBrowser(CString HomePage, int cx, int cy)
{
	if (HomePage.Find("@") != -1 && HomePage.Find("@@dev9") == -1)	// ID에 "@@dev9가"있음
	{
		ShellExecute(NULL, _T("open"), HomePage, NULL,NULL, SW_SHOWNORMAL);
		return;
	}
	
	//AfxMessageBox(HomePage);
	int find = HomePage.Find('\t');
	CString url;
	
	if (find > 0)
	{
		url = HomePage.Left(find);
		CString str(HomePage.Right(HomePage.GetLength() - (find + 1)));
		int width{}, height{};
		width = height = 0;

		find = str.Find('\t');
		if (find > 0)
		{
			width = atoi(str.Left(find));
			str = str.Right(str.GetLength() - (find + 1));
			height = atoi(str);
		}
		if (width && height)
		{
			cx = width;
			cy = height;
		}
	}
	else
	{
		url = HomePage;
	}
	
	HomePage = url;
	VARIANT vFlags = {0},
	vTargetFrameName = {0},
	vPostData = {0},
	vHeaders = {0};

	HRESULT hr{};
	_bstr_t	bstr(HomePage);
	const UINT	nFlag = SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW;
	WINDOWPLACEMENT	pl{};

	CRect	CRC = CRect(0, 0, 1, 1);
	ClientToScreen(&CRC);
	CRC.top += 85;
	
	if (::IsWindow(m_WBWnd))
	{
		m_pWBApp->Stop();
		if (HomePage.IsEmpty())
			m_pWBApp->GoHome();
		else
			m_pWBApp->Navigate(bstr, &vFlags, &vTargetFrameName, &vPostData, &vHeaders);

		//m_pWBApp->put_Top(VARIANT_TRUE);
		m_pWBApp->put_Left(CRC.left);
		m_pWBApp->put_Top(CRC.top);
		m_pWBApp->put_Visible(VARIANT_TRUE);
		VariantClear(&vPostData);
		::GetWindowPlacement(m_WBWnd, &pl);
		if (pl.showCmd & SW_SHOWMINIMIZED)
		{
			pl.showCmd = SW_RESTORE;
			::SetWindowPlacement(m_WBWnd, &pl);
		}
		else
		{
			::SetWindowPos(m_WBWnd, HWND_TOP, 0, 0, 0, 0, nFlag);
		}
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
//	m_pWBApp->put_Left(VARIANT_TRUE);
//	m_pWBApp->put_Top(VARIANT_TRUE);
	m_pWBApp->put_Left(CRC.left);
	m_pWBApp->put_Top(CRC.top);
	
	//** 툴바와 상태바를 표시하기로 함. (IBK)
	//m_pWBApp->put_ToolBar(VARIANT_FALSE);
	//**m_pWBApp->put_StatusBar(VARIANT_FALSE);
//	m_pWBApp->put_Resizable(VARIANT_FALSE);
	CString helpurl = "http://www.ibks.com/HTSHELP/helptrnum.ids?trnum=";
	if (!(HomePage.Compare(helpurl)<0))
		m_pWBApp->put_ToolBar(VARIANT_FALSE);


	hr = m_pWBApp->Navigate(bstr, &vFlags, &vTargetFrameName, &vPostData, &vHeaders);
	if (HomePage.IsEmpty())	m_pWBApp->GoHome();
	m_pWBApp->put_Visible(VARIANT_TRUE);
	VariantClear(&vPostData);

	m_pWBApp->get_HWND((long *)&m_WBWnd);
	::GetWindowPlacement(m_WBWnd, &pl);
	if (pl.showCmd & SW_SHOWMINIMIZED)
	{
		pl.showCmd = SW_RESTORE;
		::SetWindowPlacement(m_WBWnd, &pl);
	}
	else
	{
		::SetWindowPos(m_WBWnd, HWND_TOP, 0, 0, 0, 0, nFlag);
	}

	CoUninitialize();
}

//	cx = 792, cy = 569;
void CMainFrame::PopupWeb(CString url, int cx, int cy)
{
	if (url.Find("@") != -1)
	{
		ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOWNORMAL);
		return;
	}
	
	CString HomePage = url;
	const VARIANT vFlags = {0},
	const vTargetFrameName = {0},
	const vPostData = {0},
	const vHeaders = {0};

	HRESULT hr{};
	_bstr_t	bstr(HomePage);
	const UINT	nFlag = SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW;
	//WINDOWPLACEMENT	pl;

	if (FAILED(hr = CoInitialize(NULL)))
		return;

	struct	IWebBrowserApp* pWEBApp;
//	HWND	hWEB;
	if (FAILED(hr = CoCreateInstance(CLSID_InternetExplorer,
			NULL, CLSCTX_SERVER, IID_IWebBrowserApp,
			(LPVOID*)&pWEBApp)))
	{
		return;
	}

	IWebBrowserApp* m_pIEApp{};

	if (pWEBApp)
	{
		// 			OutputDebugString("GET BROWSER");
		
		const HRESULT hr = pWEBApp->QueryInterface(IID_IWebBrowserApp,(void**)&m_pIEApp);
		
		m_pIEApp->put_MenuBar(false);
		m_pIEApp->put_ToolBar(false);
		m_pIEApp->put_StatusBar(false);
		
		// 			SysFreeString(bstrURL);
		// 			pBrowser2->Release();
	}

// 	cx += GetSystemMetrics(SM_CXFRAME) * 2;
// 	cx += GetSystemMetrics(SM_CXBORDER) * 4;
// 	cy += GetSystemMetrics(SM_CYFRAME) * 2;
// 	cy += GetSystemMetrics(SM_CYBORDER) * 4;
// 	cy += GetSystemMetrics(SM_CYCAPTION);

// 	pWEBApp->put_Width(cx);
// 	pWEBApp->put_Height(cy);
// 	pWEBApp->put_Left(VARIANT_TRUE);
// 	pWEBApp->put_Top(VARIANT_TRUE);
// 	pWEBApp->put_ToolBar(VARIANT_FALSE);
// 	pWEBApp->put_StatusBar(VARIANT_FALSE);
// 	hr = pWEBApp->Navigate(bstr, &vFlags, &vTargetFrameName, &vPostData, &vHeaders);
// 	if (HomePage.IsEmpty())	pWEBApp->GoHome();
// 	pWEBApp->put_Visible(VARIANT_TRUE);
// 	VariantClear(&vPostData);
// 
// 	pWEBApp->get_HWND((long *)&hWEB);
// 	::GetWindowPlacement(hWEB, &pl);
// 	if (pl.showCmd & SW_SHOWMINIMIZED)
// 	{
// 		pl.showCmd = SW_RESTORE;
// 		::SetWindowPlacement(hWEB, &pl);
// 	}
// 	else	
// 		::SetWindowPos(hWEB, HWND_TOPMOST, 0, 0, 0, 0, nFlag);

	VARIANT vEmpty;
	VARIANT vHeader;
	VariantInit(&vEmpty);
	
	CString strHeader;
	strHeader = "IBK투자증권";
	vHeader.vt = VT_BSTR;
	vHeader.bstrVal = strHeader.AllocSysString();
	
	BSTR bstrURL = url.AllocSysString();
	
	pWEBApp->put_Width(cx);
	pWEBApp->put_Height(cy);
	
	hr = pWEBApp->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vHeader);
	if (SUCCEEDED(hr))
	{
		m_pIEApp->put_MenuBar(false);
		m_pIEApp->put_ToolBar(false);
		m_pIEApp->put_StatusBar(false);
		
		m_pIEApp->put_Top(100);
		m_pIEApp->put_Left(200);
		
		// 			CString s;
		// 			s.Format("X [%d] Y [%d]\n",rc.top,rc.left);
		// 			OutputDebugString(s);
		
		hr = pWEBApp->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vHeader);
		
		m_pIEApp->put_Visible(true);
		
		HWND pWB;
		//m_pIEApp->get_HWND((long*)&pWB);
		
		pWEBApp->get_HWND((long*)&pWB);
		CWnd *pwnd = CWnd::FromHandle(pWB); 
		
		pwnd->BringWindowToTop();

		//::SetWindowPos(pWB, HWND_TOPMOST, 0, 0, 0, 0, nFlag);
		
		CString data;
		data.Format("$WEBHWND\t%u",(LONG)pwnd);
		
		//Variant(triggerCC,data);
	}

	CoUninitialize();
}

void CMainFrame::ExitWebBrowser()
{
	if (::IsWindow(m_WBWnd))
	{
		if(m_pWBApp)
		{
			m_pWBApp->Stop();
			m_pWBApp->Quit();
			m_pWBApp->Release();
		}
		m_WBWnd = NULL;
	}
}

void CMainFrame::funcKey(int funcID, int index)
{
	switch (funcID)
	{
	case CTRL_EDITTOOL:	EditTool();	break;
	case CTRL_REGISTER:	toolRegister();	break;
	case CTRL_SCREENPRINT:	printImg();	break;
	case CTRL_LOCK:		LockProg();	break;
	case CTRL_SAVEWORKSPACE:
		saveUserScreen();		break;
	case CTRL_WORKSPACELIST:
		ShowUScreenMenu();		break;
	case CTRL_REGISTERALL:
		toolRegisterAll(index);		break;
	case CTRL_ENVIRONMENT:
		SetTicker();//TotalSetup(); 
		break;
	case CTRL_NEWS:
		ShowNews();
		break;
	case CTRL_FUNC + 14:
		RunTopBanner(index);
		break;
	case CTRL_FUNC + 16:
		RunTOP10();
		break;
	case CTRL_FUNC + 17:
	{
		if (m_top10)
			m_top10->SetCurrent(index);
	}
		break;
// 	case CTRL_FUNC + 15:
// 		RunStaffTopBanner();
// 		break;
	default:
		if (funcID >= CTRL_FUNC + 1 && funcID <= CTRL_FUNC + 12)
			DoFunc(funcID);
	}
}

void CMainFrame::RunStaffTopBanner()
{
	CString	Path;
	Path.Format("%s\\%s\\BANNER.INI", Axis::home, "tab");
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("BANNER_STAFF","url","http://www.ibks.com",readB,sizeof(readB),Path);
	
	CString url(readB,readL);
	
	int left{}, top{};
	
	left = 0;
	top = 0;

	if (url.Find("HTS:") > -1)
	{
		CString mapname = url.Right(8);

		if(mapname == "AM000101")
			m_mapHelper->CreatePopup(mapname, 1, WK_POPUP, RtopPOS);
		else
			m_mapHelper->CreateChild(mapname,0,0,LtopPOS,CPoint(left,top));
	}
	else
	{
		ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOWNORMAL);
		return;
	}
}

void CMainFrame::RunTopBanner(int index) 
{
	CString	Path;
	Path.Format("%s\\%s\\BANNER.INI", Axis::home, "tab");

	char readB[1024]{};
	int readL{};
	
	if (index == 0)
	{
		m_iClickCROWDBanner++;
		readL = GetPrivateProfileString("BANNER","url","http://www.ibks.com",readB,sizeof(readB),Path);
	}
	else 
	{
		m_iClickEPBBanner++;
		readL = GetPrivateProfileString("BANNER","url2","http://www.ibks.com",readB,sizeof(readB),Path);
	}

	CString url(readB,readL);

	int left{}, top{};

	left = 0;
	top = 0;
	
	if (url.Find("HTS:") > -1)
	{
		CString mapname = url.Right(8);
		m_mapHelper->CreateChild(mapname,0,0,LtopPOS,CPoint(left,top));
	}
	else
	{
		ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOWNORMAL);  
		return;
	}

}

bool CMainFrame::changeMAX(CString mapN)
{
	bool	bMax = false;
	int	vtype = 0,  size = 0;
	const int vwTYPE = 0, key = 0;
	m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
				(BYTE *)(VTS_BSTR VTS_I4), mapN, &size);

	WINDOWPLACEMENT	pl{};
	for (int ii = 0; ii < 4; ii++)
	{
		int		key{};
		CChildFrame* pChild{};
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

////////////////////////////////////////////////
/////  2006. 08. 23 이인호
////////////////////////////////////////////////
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

void CMainFrame::KillUpdateAgent()
{
	HWND hOsk = ::FindWindow(NULL,"IBKS Resource Update Agent");

	if(hOsk)
	{
		const CWnd* pWnd = CWnd::FromHandle(hOsk);

		DWORD pid2;
		const DWORD pid = GetWindowThreadProcessId(hOsk,&pid2);

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid2);

		if(hProcess)
		{
			if(TerminateProcess(hProcess,0))
			{
				unsigned long nCode;
				GetExitCodeProcess(hProcess,&nCode);
			}

			CloseHandle(hProcess);
		}
	}
}

void CMainFrame::load_eninfomation(bool first)
{
	ReadManageMapInfo();
	LoadScreenInfo("AxMapInfo.ini");
#ifdef DF_MK_CAPTION
	ReadMarketFile();
#else
	FileMove();
#endif
	CString		file, key, mapN, userN;
	int		value{};
	long		rc = 0;

	CProfile profile(pkEnvironment);

	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);
	WriteLog("[AXIS] load_eninformation========================1\n");
	//처음 접속자는 디폴트로 팝업되게 변경

	// 현재 팝업 정책 버전. 정책을 또 바꿔야 할 때 이 값을 올린다.
	const int ACCPOP_CUR_VERSION = 1;

	if (Axis::isCustomer)
	{
		// 처음 접속자 초기화: 디폴트는 안 띄움
		if (!GetPrivateProfileInt("SCREEN", "POPUPACCSAVE", 0, file))
		{
			WritePrivateProfileString("SCREEN", "POPUPACCSAVE", "1", file);
			WritePrivateProfileString("SCREEN", "POPUPACC", "0", file);
		}
	}
	else
	{
		// 직원도 처음 접속자 초기화: 디폴트는 안 띄움
		if (!GetPrivateProfileInt("SCREEN", "POPUPACCSTAFF", 0, file))
		{
			WritePrivateProfileString("SCREEN", "POPUPACCSTAFF", "1", file);
			WritePrivateProfileString("SCREEN", "POPUPACC", "0", file);
		}
	}

	// 버전 마이그레이션 (고객/직원 공통):
	// 버전 키가 없거나 구버전인 사용자는 기존 "계속 띄우기" 설정을 무시하고 안 띄운다.
	int iAccPopVer = GetPrivateProfileInt("SCREEN", "POPUPACCVER", 0, file);
	if (iAccPopVer < ACCPOP_CUR_VERSION)
	{
		WritePrivateProfileString("SCREEN", "POPUPACC", "0", file);

		CString strVer;
		strVer.Format("%d", ACCPOP_CUR_VERSION);
		WritePrivateProfileString("SCREEN", "POPUPACCVER", strVer, file);
	}
	// iAccPopVer >= ACCPOP_CUR_VERSION 이면 직접 설정한 사람. POPUPACC 값을 그대로 존중.

	m_ShowSlide = GetPrivateProfileInt("CHOKNTALK", "showslide", 1, file);

	if (first && GetPrivateProfileInt("SCREEN", "POPUPACC", 1, file) && Axis::user.CollateNoCase("guest"))
		AcctPasswordConfig();
	if(first)
		Popup7805();

	//새창열기 허용
	m_screenNew = GetPrivateProfileInt("SCREEN", "SCREENNEW", 1, file);

	m_titleChange = GetPrivateProfileInt("SCREEN", "TITLECHANGE", 0, file);

	char buff[1024];
	GetPrivateProfileString("SCREEN","USERTITLE","",buff,sizeof(buff),file);
	m_userTitle = buff;

	value = profile.GetInt(szETC, "SELECT", 1);

	if (value < 0 || value > 4)
		value = 1;

	key.Format("%02d", value);

	m_maxChild = profile.GetInt(szETC, key, 16);
	if (m_maxChild > 99)	m_maxChild = 99;
	if (m_maxChild <= 0)	m_maxChild = 1;

	if((m_bSDI && !m_miniSWid) || (!m_bSDI && !m_miniWid))
	{
		const char* trust = "IB0000X8";
		m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
		
// 		if(m_bMiniMode)
// 		{
// 			CWnd* base = m_miniSWid->GetActiveView()->GetWindow(GW_CHILD);
// 			
// 			base->SendMessage(WD_ADD_CODE,(WPARAM)0,(LPARAM)"TR");
// 		}
	}
	WriteLog("[AXIS] load_eninformation========================2\n");

	m_slog.Format("[axis][CMainFrame] 사용자 화면 띄우기 전\n");
	WriteLog(m_slog);

	value = profile.GetInt(szScreen, "STARTMAP");
	if (first)
	{
		switch (value)
		{
		case 1:	createUserScreen(szLastStat, true);		break;
		case 2: createUserScreen(szStartMap, true);	break;
		default:			break;
		}
	}
	WriteLog("[AXIS] load_eninformation========================3\n");
	m_posChild = profile.GetInt(szScreen, "CHILDPOS");
	m_fontSize = profile.GetInt(szScreen, "FONTSIZE", 9);
	if (m_fontSize < 8) m_fontSize = 8;
	if (m_fontSize > 12) m_fontSize = 12;

	if(m_bMiniMode)
	{
		if (!m_arSDI[m_vsN].IsEmpty())
		{
			int key{};
			CSChild* pChild{};
			POSITION pos = m_arSDI[m_vsN].GetStartPosition();
			while(pos!=NULL)
			{	
				m_arSDI[m_vsN].GetNextAssoc(pos, key, pChild);
				pChild->ShowWindow(SW_HIDE);
			}
		}
		if (!m_arMPOP.IsEmpty())
		{
			int key{};
			CMPop* pPop{};
			POSITION pos = m_arMPOP.GetStartPosition();
			while(pos!=NULL)
			{
				m_arMPOP.GetNextAssoc(pos, key, pPop);
				pPop->ShowWindow(SW_HIDE);
			}
		}

		ShowWindow(SW_HIDE);
	}
	changeFontSize();
	WriteLog("[AXIS] load_eninformation========================4\n");
	value = profile.GetInt(szScreen, "UNFLESH") ? 0 : 1;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFCB, 0), value);

//	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
//			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setTRG, 0), MAKELONG(0, 0));	// 동일그룹 적용

	value = profile.GetInt(szScreen, "VS_REFLECT", 1) ? 1 : 0;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setTRG, 1), MAKELONG(0, value));
	
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setVID, m_vsN), MAKELONG(0, 0));
	//change_VirtualScreen(V_SCREEN1);
	SetVirtualSDIVisible(m_vsN, false);

	m_axGuide->SetPos(profile.GetInt(szScreen, "MSGPOS"));

	KillTimer(TM_IDLE);
	m_idletime = profile.GetInt(szScreen, "IDLETIME");

// 	if((m_bSDI && !m_miniSWid) || (!m_bSDI && !m_miniWid))
// 	{
// 		const char* trust = "IB0000X8";
// 		m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
// 		
// 		if(m_bMiniMode)
// 		{
// 			CWnd* base = m_miniSWid->GetActiveView()->GetWindow(GW_CHILD);
// 			
// 			base->SendMessage(WD_ADD_CODE,(WPARAM)0,(LPARAM)"TR");
// 		}
// 	}
	WriteLog("[AXIS] load_eninformation========================5\n");
	LoadHotkeySetting();
	
	m_bMustRetry = CheckMasterFile();
	WriteLog("[AXIS] load_eninformation========================6\n");
}

BOOL CMainFrame::CheckMasterFile()
{
	CString strFilePath;
	strFilePath.Format("%s\\tab\\%s",Axis::home,MSLIST_FILE);

	CString s;
// 	s.Format("MSLIST FILE : [%s]\n",strFilePath);
// 	OutputDebugString(s);

	CStdioFile fileM;

	if(!fileM.Open(strFilePath,CFile::modeRead | CFile::typeText))
	{
		return TRUE;
	}

	CString sResult;

	CString strData;

	while(fileM.ReadString(sResult))
	{
		CFileFind findFile;

		CString strFile;
		strFile.Format("%s\\tab\\%s",Axis::home,sResult);

// 		s.Format("MASTERFILE : [%s]\n",strFile);
// 		OutputDebugString(s);

		if(findFile.FindFile(strFile))
		{
			findFile.FindNextFile();

			CTime tTime;
			findFile.GetLastWriteTime(tTime);

			const int month = tTime.GetMonth();
			const int day = tTime.GetDay();

// 			s.Format("MASTERFILE : [%04d - %02d - %02d]\n",tTime.GetYear(),month,day);
//  			OutputDebugString(s);			

			SYSTEMTIME st;
			::GetLocalTime(&st);

			const CTime today(st.wYear,st.wMonth,st.wDay,0,0,0);

// 			s.Format("TODAY : [%04d - %02d - %02d]\n",today.GetYear(),today.GetMonth(),today.GetDay());
//  			OutputDebugString(s);

			if(month != today.GetMonth() || day != today.GetDay())
			{
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}

		//iFind = sResult.Find("\n");
	}

	return FALSE;
}

void CMainFrame::SetDefaultMng()
{
	m_mapAlarmList.RemoveAll();

	CString mtblFile;
	mtblFile.Format("%s\\%s\\mngsetup.ini",
		Axis::home, MTBLDIR);

	// KRX
	LoadMngFromIni(mtblFile, "Manage");

	// NXT
	LoadMngFromIni(mtblFile, "NXT");

}

void CMainFrame::ExpandDerivedSignals()
{
	CString v;

	if (m_mapAlarmList.Lookup("871", v))  //사이드카발동
	{
		m_mapAlarmList.SetAt("873", "");  //?
		m_mapAlarmList.SetAt("875", ""); //?
		m_mapAlarmList.SetAt("877", ""); //?
	}

	if (m_mapAlarmList.Lookup("872", v))
	{
		m_mapAlarmList.SetAt("874", "");  //?
		m_mapAlarmList.SetAt("876", "");  //?
		m_mapAlarmList.SetAt("878", "");  //?
	}
}

void CMainFrame::LoadMngFromIni(const CString& file, const CString& section)
{
	TCHAR buf[4096]{};
	DWORD len = GetPrivateProfileSection(section, buf, 4096, file);

	if (len <= 0) return;

	TCHAR* p = buf;

	while (*p)
	{
		CString line = p;

		int eq = line.Find('=');
		if (eq > 0)
		{
			CString key = line.Left(eq);
			CString val = line.Mid(eq + 1);

			int semi = val.Find(';');
			if (semi >= 0)
				val = val.Left(semi);

			key.TrimRight();
			val.TrimRight();

			if (atoi(val) == 1)
				m_mapAlarmList.SetAt(key, val);
			else
				m_mapAlarmList.RemoveKey(key);
		}

		p += _tcslen(p) + 1;
	}
}

void CMainFrame::MigrateMng(LPCTSTR file, int oldVersion)
{

	if (oldVersion < 5)
	{
		m_mapAlarmList.SetAt("871", "");  //사이드카 발동
		m_mapAlarmList.SetAt("872", "");  //사이드카 해제
		m_mapAlarmList.SetAt("837", "1");   //선물/옵션 서킷브레이커발동
		m_mapAlarmList.SetAt("838", "1");   //선물/옵션 서킷브레이커해제
	}

	CString v;
	v.Format("%d", 5);
	WritePrivateProfileString("MESSAGE", "INIT", v, file);
}

#ifdef DF_NEW_MNG
void CMainFrame::load_mngSetup()
{
	CString userFile, mtblFile;

	userFile.Format("%s\\%s\\%s\\mngsetup.ini",
		Axis::home, USRDIR, Axis::user);

	mtblFile.Format("%s\\%s\\mngsetup.ini",
		Axis::home, MTBLDIR);

	bool userExist =
		(GetFileAttributes(userFile) != INVALID_FILE_ATTRIBUTES);

	int mtblVer = GetPrivateProfileInt("MESSAGE", "INIT", 0, mtblFile);
	int userVer = 0;

	if (userExist)
		userVer = GetPrivateProfileInt("MESSAGE", "INIT", 0, userFile);

	// version sync
	if (!userExist || mtblVer > userVer)
		CopyFile(mtblFile, userFile, FALSE);

	CString file = userFile;

	// -------------------------
	// UI 설정
	// -------------------------
	CProfile profile(pkManageSetup);

	m_bSound = profile.GetInt("Setup", "SOUND", 0);
	m_nInfoPos = profile.GetInt("Setup", "POS", 2);
	m_bUseAlarm = profile.GetInt("Setup", "USE", 1);
	m_nBkMode = profile.GetInt(szBkNotice, "MODE");

	int val = profile.GetInt("Manage", MNG_INFO_KOBAELW, -1);
	m_bKobaElwNotify = (val == -1) ? true : (val == 1);

	// -------------------------
	// default = mtbl
	// -------------------------
	SetDefaultMng();

	// -------------------------
	// user overlay
	// -------------------------
	LoadMngFromIni(file, "Manage");
	LoadMngFromIni(file, "NXT");

	// -------------------------
	// derived
	// -------------------------
	ExpandDerivedSignals();
}
#else
void CMainFrame::load_mngSetup()
{
	CProfile profile(pkManageSetup);
	CString szTotal, szUnit;

	const int iret = profile.GetInt(szMessage, "Init");

	m_bSound = profile.GetInt("Setup", "SOUND") == 1;
	m_nInfoPos = profile.GetInt("Setup", "POS", 2);
	m_bUseAlarm = profile.GetInt("Setup", "USE", 1);
	m_nBkMode = profile.GetInt(szBkNotice, "MODE");

	// KOBA ELW 장운영 알림 설정
	const int val = profile.GetInt("Manage", MNG_INFO_KOBAELW, -1);
	if (val == -1)
	{
		m_bKobaElwNotify = true;
	}
	else
	{
		m_bKobaElwNotify = (val == 1) ? true : false;
	}

	m_mapAlarmList.RemoveAll();
	if (iret == 0)  //유저폴더에  없는 상태
	{	//디폴트 장운영을 넣어준다.
		m_mapAlarmList.SetAt("851", "");	//장전 동시호가 개시
		m_mapAlarmList.SetAt("801", "");	//장개시
		m_mapAlarmList.SetAt("21", "");		//장개시 10분전
		m_mapAlarmList.SetAt("26", "");		//장개시 5분전
		m_mapAlarmList.SetAt("30", "");		//장개시 1분전
		m_mapAlarmList.SetAt("35", "");		//장개시 10초전
		m_mapAlarmList.SetAt("852", "");	//장후 동시호개 개시
		m_mapAlarmList.SetAt("809", "");	//장마감
		m_mapAlarmList.SetAt("126", "");	//장마감 5분전
		m_mapAlarmList.SetAt("130", "");	//장마감 1분전
		m_mapAlarmList.SetAt("135", "");	//장마감 10초전
		m_mapAlarmList.SetAt("817", "1");	//서킷브레이커발동
		m_mapAlarmList.SetAt("818", "1");	//서킷브레이커해제
		m_mapAlarmList.SetAt("804", "");	//시간외종가 매매개시
		m_mapAlarmList.SetAt("853", "");	//시간외종가 매매종료, 시간외단일가 매매개시 
		m_mapAlarmList.SetAt("806", "");	//시간외단일가 매매종료
		m_mapAlarmList.SetAt("846", "");	//KOBA ELW 조기종료
		m_mapAlarmList.SetAt("871", "1");	//사이드카 발동
		m_mapAlarmList.SetAt("872", "1");	 //사이드카 해제

		 //version 4  NXT 
		m_mapAlarmList.SetAt("881", "NXT 프리마켓 개시");  /* 08:00            */
		m_mapAlarmList.SetAt("882", "NXT 프리마켓 마감");  /* 08:50            */
		m_mapAlarmList.SetAt("884", "NXT 메인마켓 개시"); /* 09:00:30         */
		m_mapAlarmList.SetAt("885", "NXT 메인마켓 마감");  /* 15:20            */
		m_mapAlarmList.SetAt("887", "NXT 단일가 호가개시"); /* 15:30~15:40      */
		m_mapAlarmList.SetAt("888", "NXT 애프터마켓 개시");  /* 15:40            */
		m_mapAlarmList.SetAt("889", "NXT 애프터마켓 마감"); /* 20:00            */
	}
	else if (iret == 3)
	{	//기존 설정  //디폴트 장운영을 넣어준다.
		profile.Write(szMessage, "OPEN", "");
		profile.Write(szMessage, "CLOSE", "");
		m_mapAlarmList.SetAt("851", "");	//장전 동시호가 개시
		m_mapAlarmList.SetAt("801", "");	//장개시
		m_mapAlarmList.SetAt("21", "");		//장개시 10분전
		m_mapAlarmList.SetAt("26", "");		//장개시 5분전
		m_mapAlarmList.SetAt("30", "");		//장개시 1분전
		m_mapAlarmList.SetAt("35", "");		//장개시 10초전
		m_mapAlarmList.SetAt("852", "");	//장후 동시호개 개시
		m_mapAlarmList.SetAt("809", "");	//장마감
		m_mapAlarmList.SetAt("126", "");	//장마감 5분전
		m_mapAlarmList.SetAt("130", "");	//장마감 1분전
		m_mapAlarmList.SetAt("135", "");	//장마감 10초전
		m_mapAlarmList.SetAt("817", "");	//서킷브레이커발동
		m_mapAlarmList.SetAt("818", "");	//서킷브레이커해제
		m_mapAlarmList.SetAt("804", "");	//시간외종가 매매개시
		m_mapAlarmList.SetAt("853", "");	//시간외종가 매매종료, 시간외단일가 매매개시 
		m_mapAlarmList.SetAt("806", "");	//시간외단일가 매매종료
		m_mapAlarmList.SetAt("846", "");	//KOBA ELW 조기종료
		m_mapAlarmList.SetAt("871", "");	//사이드카 발동
		m_mapAlarmList.SetAt("872", "");	 //사이드카 해제

		 //version 4  NXT 
		m_mapAlarmList.SetAt("881", "NXT 프리마켓 개시");  /* 08:00            */
		m_mapAlarmList.SetAt("882", "NXT 프리마켓 마감");  /* 08:50            */
		m_mapAlarmList.SetAt("884", "NXT 메인마켓 개시"); /* 09:00:30         */
		m_mapAlarmList.SetAt("885", "NXT 메인마켓 마감");  /* 15:20            */
		m_mapAlarmList.SetAt("887", "NXT 단일가 호가개시"); /* 15:30~15:40      */
		m_mapAlarmList.SetAt("888", "NXT 애프터마켓 개시");  /* 15:40            */
		m_mapAlarmList.SetAt("889", "NXT 애프터마켓 마감"); /* 20:00            */
	}
	else if (iret == 4)  //version 4  NXT 
	{//설정화면을 이미 다녀왔다. 
		//ref 장운영 추가 
		CString file;
		file.Format("%s\\%s\\%s\\mngsetup.ini", Axis::home, USRDIR, Axis::user);
		char	ssb[1024 * 4];
		DWORD ssL = GetPrivateProfileSection("Manage", ssb, sizeof(ssb), file);
		if (ssL <= 0)
			return;

		CString subitem, keys, value, string = CString(ssb, ssL);
		for (; !string.IsEmpty(); )
		{
			int idx = string.Find('\0');
			if (idx == -1)	break;

			subitem = string.Left(idx++);
			string = string.Mid(idx);

			idx = subitem.Find('=');
			if (idx == -1)	continue;

			keys = subitem.Left(idx++);
			value = subitem.Mid(idx);

			if (atoi(value) == 1)
				m_mapAlarmList.SetAt(keys, "");
		}


		ssL = GetPrivateProfileSection("NXT", ssb, sizeof(ssb), file);
		if (ssL <= 0)
			return;

		string = CString(ssb, ssL);
		for (; !string.IsEmpty(); )
		{
			int idx = string.Find('\0');
			if (idx == -1)	break;

			subitem = string.Left(idx++);
			string = string.Mid(idx);

			idx = subitem.Find('=');
			if (idx == -1)	continue;

			keys = subitem.Left(idx++);
			value = subitem.Mid(idx);

			if (atoi(value) == 1)
				m_mapAlarmList.SetAt(keys, "");
		}
	}

	CString sym, vals{};
	if (m_mapAlarmList.Lookup("871", vals)) //사이드카 발동
	{
		m_mapAlarmList.SetAt("873", "");
		m_mapAlarmList.SetAt("875", "");
		m_mapAlarmList.SetAt("877", "");
	}

	vals.Empty();
	if (m_mapAlarmList.Lookup("872", vals)) //사이드카 해제
	{
		m_mapAlarmList.SetAt("874", "");
		m_mapAlarmList.SetAt("876", "");
		m_mapAlarmList.SetAt("878", "");
	}
}
#endif

void CMainFrame::load_hkey()
{
	CProfile profile(pkHotKey);

	m_bHome = profile.GetInt(szHKey, "home", 1);
	m_bEnd  = profile.GetInt(szHKey, "end", 0);
}

void CMainFrame::maxChild()
{
	int	key{}, index{}, count = 0;
	int    vsN{}; //vc2019
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		count += m_arMDI[vsN].GetCount();
		count += m_arSDI[vsN].GetCount();
	}

	if (count > m_maxChild)
	{
		vsN = m_vsN+1;
		for (int jj = 0; jj <= V_SCREEN6; jj++, vsN++)
		{
			bool sdi = false;
			if (vsN > V_SCREEN6)	vsN = V_SCREEN1;
			if (m_arMDI[vsN].GetCount() <= 0)
			{
				if (m_arSDI[vsN].GetCount() <= 0)
					continue;
				sdi = true;
			}

			if (sdi)
			{
				CSChild*	schild = NULL;
				POSITION pos = m_arSDI[vsN].GetStartPosition();
				m_arSDI[vsN].GetNextAssoc(pos, key, schild);
				if (!schild)	continue;
				schild->PostMessage(WM_CLOSE);
			}
			else
			{
				index = m_hooklist[vsN].GetSize() - 1;
				key = m_hooklist[vsN].GetAt(index);
				
				CChildFrame*	child;
				if (!m_arMDI[vsN].Lookup(key, child))
					continue;
				child->PostMessage(WM_CLOSE);
			}
			break;
		}
	}
}

void CMainFrame::detachChild(int key)
{
	if (key < 0)	return;

	CChildFrame*	pChild = NULL;
	int	vsN;
	for (vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		if (!m_arMDI[vsN].Lookup(key, pChild))
			continue;
		//m_arMDI[vsN].RemoveKey(key);
		//RedrawVS(true);
		// dtitle
		RemoveMdi(vsN, key);
		break;
	}

	if (!pChild)	return;
	m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
			(void *)NULL, (BYTE *)(VTS_I4), (short) key);

	if (MDIGetActive() == NULL)
	{
		if (m_bar1)	m_bar1->set_Maps();
		m_hooklist[m_vsN].RemoveAll();
	}

	OutputDebugString("DETACH CHILD\n");
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
	CProfile profile(pkEnvironment);
// 	CString str(profile.GetString(szScreen, "STARTMAP"));
// 	if (atoi(str) != 1) 
// 		return;
	if(!m_bLoadScreen)
		return;

	save_laststat();

	ClearUserIni();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	//WriteLog("OnTimer Start");
	switch (nIDEvent)
	{
		/*
	case TM_CB_SEARCH:
		{
			KillTimer(TM_CB_SEARCH);
			SendCBSerachTR();
		}
		break;
		*/
	case TM_HTS_LOADENFORMATION_POP:
	case TM_HTS_LOADENFORMATION:
	{
		KillTimer(nIDEvent);
		load_eninfomation();
		//핵심 
		const char* trust = "IB820850";
		if (!IsExistMap(trust))
			load_hidescreen(trust);

		CString	PathCookie;

		PathCookie.Format("%s\\%s\\NOTICECOOKIE.INI", Axis::home, "tab");

		CString date;

		CTime time;
		time = CTime::GetCurrentTime();

		date.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

		char readB[1024];
		int readL;

		readL = GetPrivateProfileString("7700", "TODAY", "", readB, sizeof(readB), PathCookie);

		CString strDate(readB, readL);

		strDate.TrimLeft();
		strDate.TrimRight();

		if (strDate != "")
		{
			if (date == strDate)
			{
				const int update = GetPrivateProfileInt("7700", "FIRST", 1, PathCookie);
				if (update == 1)
				{
					trust = "IB770000";
					m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
				}
			}
			else
			{
				WritePrivateProfileString("7700", "TODAY", date, PathCookie);
				WritePrivateProfileString("7700", "FIRST", "1", PathCookie);

				trust = "IB770000";
				m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
			}
		}
		else
		{
			trust = "IB770000";
			m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
		}

		ReadNoticeMap();

		if (nIDEvent == TM_HTS_LOADENFORMATION_POP)
		{
			int key = m_mapHelper->CreateSDI("IB999962", 0, 0, 1, 0);
			CSChild* schild = NULL;
			if (!m_arSDI[m_vsN].Lookup(key, schild))
				return;

			schild->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE);
			schild->CenterWindow();

			schild->SetFocus();
			schild->SendMessage(WM_NCACTIVATE, TRUE, 0);
			//schild->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(5, 5));
			//schild->SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(5, 5));
		}
#ifdef  DF_MK_CAPTION
		
#endif
	}
	break;
	case TM_HISTORY:
	{
		KillTimer(TM_HISTORY);
		m_bar3->del_AllButton();

		CreateHistoryBar();
		m_bHistory = FALSE;
	}
	break;
	case TM_AUTOHIDE:
		WriteLog("[AXIS] OnTimer - TM_AUTOHIDE");
		KillTimer(nIDEvent);
		if (m_conAUTOTIME)
			m_conclusion->ShowWindow(SW_HIDE);
		break;
	case TM_AUTOMSN:
		WriteLog("[AXIS] OnTimer - TM_AUTOMSN");
		m_Econclusion->HideSlide();
		break;
	case TM_AUTOMNGINFO:
		WriteLog("[AXIS] OnTimer - TM_AUTOMNGINFO");
		m_mngInfo->HideSlide();
		KillTimer(TM_AUTOMNGINFO);
		break;
	case TM_MNGINFO_NOW:
		/*KillTimer(TM_MNGINFO_NOW);
		m_mngInfo->ShowWindow(SW_HIDE);
		m_sMngType.Empty();*/
		break;
	case TM_IDLE:
		WriteLog("[AXIS] OnTimer - TM_IDLE");
		KillTimer(nIDEvent);
		LockProg();
		break;
	case TM_PURL:
		WriteLog("OnTimer - TM_PURL");
		KillTimer(nIDEvent);
		break;
	case TM_PRNIMG:
		WriteLog("OnTimer - TM_PRNIMG");
		{
			KillTimer(TM_PRNIMG);
			printOper(m_pPRNChild, m_sizePRN, m_sizePRNORG, m_szPRNData, m_bPRNResize);
		}
		break;
	case TM_AUTODCOTOR:
		WriteLog("OnTimer - TM_AUTODCOTOR");
		if (m_pDoctor)	m_pDoctor->HideSlide();
		break;
	case TM_RTSQUEUE:
		// 		WriteLog("OnTimer - TM_RTSQUEUE");
		// 		checkRTSQueue();
		break;
	case TM_SCRLOG:
		WriteLog("[AXIS] OnTimer - TM_SCRLOG");
		writeOpenedList();
		break;
	case TM_TEST:
		WriteLog("[AXIS] OnTimer - TM_TEST");
		{
			KillTimer(TM_TEST);
		}
		break;
	case TM_INITSIZE:
		WriteLog("[AXIS] OnTimer - TM_INITSIZE");
		{
			KillTimer(TM_INITSIZE);
			CString name = "IBINITSIZE";
			CProfile pout(pkUserConfig);
			const int piout = pout.GetInt(name, "OnLoad", 0);
			if (piout == 0)
			{
				InitMapSize();
			}
		}
		break;
	case TM_NOTICE:
		WriteLog("[AXIS] OnTimer - TM_NOTICE");
		{
			KillTimer(TM_NOTICE);
			//ReadNotice();
			ReadNoticeMap();
		}
		break;
	case TM_DNINTEREST:
		WriteLog("[AXIS] OnTimer - TM_DNINTEREST");
		{
			WriteLog("[AXIS] TM_DNINTEREST - Step 1");
			KillTimer(TM_DNINTEREST);
			if (m_info_interest)
			{
				m_info_interest->SetOK();
			}

			WriteLog("[AXIS] TM_DNINTEREST - Step 2");
			SetForegroundWindow();

			WriteLog("[AXIS] TM_DNINTEREST - Step 3");
			if (m_bdnInterest)
			{
				WriteLog("[AXIS] TM_DNINTEREST - Step 4-1");
				if (!Axis::isCustomer)
					load_eninfomation();
				WriteLog("[AXIS] TM_DNINTEREST - Step 4-2");

				const char* trust = "IB820850";
				if (!IsExistMap(trust))
					load_hidescreen(trust);

				WriteLog("[AXIS] TM_DNINTEREST - Step 4-3");
				//load_start_notice();
				WriteLog("[AXIS] TM_DNINTEREST - Step 4-4");

				CString	PathCookie;

				PathCookie.Format("%s\\%s\\NOTICECOOKIE.INI", Axis::home, "tab");

				CString date;

				CTime time;
				time = CTime::GetCurrentTime();

				date.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

				char readB[1024];
				int readL;

				readL = GetPrivateProfileString("7700", "TODAY", "", readB, sizeof(readB), PathCookie);

				CString strDate(readB, readL);

				strDate.TrimLeft();
				strDate.TrimRight();

				if (strDate != "")
				{
					if (date == strDate)
					{
						const int update = GetPrivateProfileInt("7700", "FIRST", 1, PathCookie);

						if (update == 1)
						{
							trust = "IB770000";
							//if (!IsExistMap(trust))
							{
								m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
							}
						}
					}
					else
					{
						WritePrivateProfileString("7700", "TODAY", date, PathCookie);
						WritePrivateProfileString("7700", "FIRST", "1", PathCookie);

						trust = "IB770000";
						//if (!IsExistMap(trust))
						{
							m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
						}
					}
				}
				else
				{
					trust = "IB770000";
					//if (!IsExistMap(trust))
					{
						m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
					}
				}

				ReadNoticeMap();
				//긴급장애공지
				//MessageBox("시스템 점검작업으로 인하여 일부시세 정보제공이 안 되고 있습니다. \n불편을 드려 죄송합니다. 감사합니다.", "IBK투자증권");
				WriteLog("[AXIS] TM_DNINTEREST - Step 4-5");
			}
		}
		break;
	case TM_SLIDE:
		WriteLog("[AXIS] OnTimer - TM_SLIDE");
		if (m_slideMsg != "")
		{
			m_mngInfo->SetData(m_slideMsg, 3);

			const CRect		mRc;
			WINDOWPLACEMENT	pl;
			GetWindowPlacement(&pl);
			switch (pl.showCmd)
			{
			case SW_HIDE:case SW_MINIMIZE:case SW_SHOWMINIMIZED:
				m_mngInfo->ShowWindow(SW_HIDE);
				CMDIFrameWnd::OnTimer(nIDEvent);
				return;
			default:
				break;
			}
			m_mngInfo->ShowWindow(SW_HIDE);

			MngInfoPos();

			m_slideMsg = "";

			KillTimer(TM_SLIDE);
		}
		break;
	case TM_ITGY:
	{

		WriteLog("[AXIS] OnTimer - TM_ITGY 50초동안 piboitgy 응답이 안왔다?");

		m_bItgy = FALSE;

		KillTimer(TM_ITGY);

		if (m_bCertLogin)
			signOnCert();
		else
		{
			signOn();
		}
	}
	break;
	case TM_WINES_KIUP:
	{
		WriteLog("[AXIS] OnTimer - TM_WINES_KIUP");
		KillTimer(TM_WINES_KIUP);
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
			(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_nKiupKey), (LPARAM)(const char*)m_sKiupDomino);
	}
	break;
	case TM_NPROTECT_ALIVE:
	{
		WriteLog("[AXIS] OnTimer - TM_NPROTECT_ALIVE");
		const BOOL pcFirewall = AfxGetApp()->GetProfileInt(INFORMATION, "PCFirewall", 0);
		if (pcFirewall && Axis::isCustomer)
			InitFirewall();
	}
	break;
	case TM_AOS_ALIVE:
	{  //ref 보안프로그래(ASTx) 가동 확인

		int iret = Self_VerifyIntegrity();
		if (!IsASTxRunning() || iret > 0)
		{
			KillTimer(TM_AOS_ALIVE);
			BOOL		rc{};
			m_bExit = false;
			m_forceClose = true;
			m_bCLOSE_ASTx = TRUE;

			m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&rc,
				(BYTE*)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);

			PostMessage(WM_AXISCLOSE, 99, 0);
			return;
		}
	}
	break;
	case TM_POPUP_JISU:
	{
		KillTimer(TM_POPUP_JISU);

		//PopupWeb("http://www.ibks.com/worldstocks/ws_list.do?popup=Y&nPro_YN=N&keyB_YN=Y",800,715);
		const char* trust = "IB780500";
		m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
	}
	break;
	case TM_TOP10_2018:
	{
		//KillTimer(TM_TOP10_2018);
		//Send2018();
	}
	break;
	case TM_2022_CLOSE:
	{
		PostMessage(WM_CLOSE);
	}
	break;
	case TM_NOSCREEN_URL:
	{
		KillTimer(TM_NOSCREEN_URL);
		m_slog.Format("ScreenCheck Ontimer key=[%d] url=[%s]", m_iKey, m_sTriggerUrl);
		OutputDebugString(m_slog);
		if (m_iKey > 0)
		{
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY,
				(void*)NULL, (BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_iKey), (LPARAM)(const char*)m_sTriggerUrl);
			m_iKey = 0;
			m_sTriggerUrl.Empty();
		}
	}
	break;
	case TM_MNGINFO:
	{
		KillTimer(TM_MNGINFO);

		if (m_mngInfo)
		{
			CString		sym, val, str;
			CString		msgS;
			const int		pos = 0;
			int		kind = 0;
			CString		wavfile;

			auto at = _mapMngInfo.find("601");
			if (at == _mapMngInfo.end())
				return;

			val = at->second;

			if (val == MNG_KOBAELW && m_bKobaElwNotify)
			{
				auto at = _mapMngInfo.find("23");
				if (at != _mapMngInfo.end())
				{
					val = at->second;

					int x{}, y{};
					CRect mRc, wRc;

					mRc = GetWndRect();
					m_kobanotify->GetWindowRect(wRc);

					x = mRc.left;
					y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
					if (x < 0)	x = 0;
					if (y < 0)	y = 0;
					if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
						y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();

					m_kobanotify->AddNotify(val);
					m_kobanotify->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
					m_kobanotify->MoveWindow(x, y, wRc.Width(), wRc.Height());
				}
			}

			CheckMarketByMNG(val);

			if (!m_mapAlarmList.Lookup(val, str))
				return;

			at = _mapMngInfo.find("47");
			if (at != _mapMngInfo.end())
				return;

			sym = at->second;

			if (!m_bUseAlarm)
				return;

			kind = atoi(sym);
			val = _T("");

			at = _mapMngInfo.find("47");
			if (at != _mapMngInfo.end())
				msgS = at->second;
			m_mngInfo->StopSlide();
			str = _T("");

			at = _mapMngInfo.find("600");
			if (m_bSound && at != _mapMngInfo.end())
			{
				str = at->second;
				if (!str.IsEmpty())
				{
					wavfile.Format("%s\\%s\\%s.wav", Axis::home, IMAGEDIR, str);
					if (!IsFileExist(wavfile))  wavfile.Format("%s\\%s\\%s.wav", Axis::home, IMAGEDIR, "000");
					sndPlaySound(NULL, SND_NODEFAULT | SND_ASYNC);
					sndPlaySound(wavfile, SND_NODEFAULT | SND_ASYNC);
				}
			}

			if (msgS.GetAt(0) == 'X')
			{
				msgS = msgS.Mid(1);
				kind = 3;
			}
			else if (msgS.GetAt(0) == 'W')
			{
				msgS = msgS.Mid(1);
				kind = 4;
			}
			else if (msgS.GetAt(0) == 'Y')
			{
				msgS = msgS.Mid(1);
				msgS.Remove('\n');
				msgS.Remove('\r');
				kind = 5;
			}

			ShowMngInfo(msgS, kind);  //OnTimer
		}
		break;
	}
	break;
	case TM_CHANGESKIN:
	{
		KillTimer(TM_CHANGESKIN);
		ChangeLogo();
		m_bitmapBtn->Invalidate();

		DrawFrame();
		change_BackGround();
		change_Resource();

		if (m_conclusion)
			m_conclusion->ChangePalette();
		if (m_kobanotify)
			m_kobanotify->ChangePallete();
		if (m_Econclusion)
			m_Econclusion->ChangePalette();
		if (m_mngInfo)
			m_mngInfo->ChangePalette();

		m_bar0->change_BandInfo();
		m_bar1->Change_Skin(Axis::skinName);
		m_bar2->Change_Skin(Axis::skinName);
		m_smain->Change_Skin(Axis::skinName);

		long	rv = 0;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4,
			(void*)&rv, (BYTE*)(VTS_I4 VTS_I4), MAKEWPARAM(setPAL, 0), 0);

		IMAXSkinSet();
	}
	break;
	case TM_STAFF_OPENNOPOACC:
	{
		KillTimer(TM_STAFF_OPENNOPOACC);
		SendMessage(WM_USER, MMSG_SETMAIN_STATE, 0);
	}
	break;
	case TM_NOTICE_POP:
	{
		KillTimer(TM_NOTICE_POP);
		if (m_sNoticePopType == "1")
			m_mapHelper->ChangeChild(m_sNoticePopMap, 1, 0, 5);
		else if (m_sNoticePopType == "2")
			m_mapHelper->CreateModal(m_sNoticePopMap);
		else if (m_sNoticePopType == "3")
			m_mapHelper->CreatePopup(m_sNoticePopMap, 1, 0, 5);

		m_sNoticePopType = "";
		m_sNoticePopMap = "";
	}
	break;
#ifdef DF_MAIN_RTS
	case TM_MAIN_RTS_PUSH:
	{
		std::vector<DispatchJob> jobs;
		jobs.reserve(512);


	}
	break;
	case TM_MAIN_TEST_RTS:
	{
		//TestRTSData();
	}
	break;
#endif
	}
	//WriteLog("End OnTimer");
	CMDIFrameWnd::OnTimer(nIDEvent);
}

#ifdef DF_MK_CAPTION
void CMainFrame::CheckMarketByMNG(CString sval)
{
	int igubn = atoi(sval);
	
	CString slog;
	slog.Format("[mnginfo][%s][%d] sval = [%s]", __FUNCTION__, __LINE__, sval );
	//m_dtick = GetTickCount();a
	OutputDebugString(slog);


	/*
KRX
0 장마감, 1 시간외,  2 정규장, 3 단일가
	809       802,804        801          805
NXT
0 장마감, 1 프리, 2 메인, 3 애프터
	890        881        884      888
*/
	switch (igubn)
	{
		case 799:  // KRX 7:00~ 7:50   KRX프리
		{
			m_iKRXype = 5;
			m_iNXType = 5;
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 798:  // KRX 7:50~ 8:00  KRX장전
		{
			m_iKRXype = 4;
			m_iNXType = 5;
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 881:  //NXT  8:00~ NXT 프리 시작
		{
			m_iNXType = 1;  //프리
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 851:  //KRX 8:30~ 주식 장개시전 동시호가 개시
		case 802:  //KRX 8:30~KRX 장전시간외 시작    
		{
			m_iKRXype = 1;   //시간외
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 832:  //NXT 8:30~
		{
			m_iNXType = 1;   // 프리
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);   //   KRX 802가 들어오면 NXT 881 프리
		}
		break;
		case 803:  //KRX 장전     8:40~8:50
		{
			m_iKRXype = 4;   //장전
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);   //   KRX 802가 들어오면 NXT 881 프리
		}
		break;
		case 883:  //KRX 단일가 시작     8:50~
		{
			m_iKRXype = 3;   //단일가
			m_iNXType = 0;  //장마감
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);   //   KRX 802가 들어오면 NXT 881 프리
		}
		break;
		case 801:   //KRX 9:00~  KRX 정규장 시작
		{
			m_iKRXype = 2;  //정규장
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 884:   //NXT 9:00:30~  NXT 메인 시작
		{
			m_iNXType = 2;  //정규장
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 886: //NXT 15:20~ NXT 오후휴장 
		{
			m_iNXType = 0;  //장마감
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 887: //NXT 15:30~ NXT 단일가
		{
			m_iNXType = 4;  //NXT 15:30~15:40  단일가 개시
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 809: //KRX 15:30~ KRX장마감
		{
			m_iKRXype = 4;
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 888:  //NXT 15:40~ NXT 애프터 시작
		{
			m_iNXType = 3;  //애프터
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 804:  //KRX 15:40~ KRX 시간외
		{
			m_iKRXype = 1;   //시간외
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 805: //KRX 16:00~ KRX 단일가 시작
		case 853:  //KRX 16:00~ 주식시간외종가매매종료
		{
			m_iKRXype = 6;  //애프터
			m_iNXType = 3; //애프터
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);  //KRX 805가 들어오면 NXT 888 애프터
		}
		break;
		case 897:  //NXT 16:00~
		{
			m_iNXType = 3; //애프터
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);  //KRX 805가 들어오면 NXT 888 애프터
		}
		break;
		case 806: //KRX 장마감 시작 18:00~
		{
			m_iKRXype = 6;  //장마감
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		case 889: //KRX 장마감 시작 20:00~
		{
			m_iKRXype = 0;  //장마감
			m_iNXType = 0; //장마감
			m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);
		}
		break;
		default:
		{
		
		}
		break;
	}
}

void CMainFrame::CheckMarketStat()
{
	/*
KRX
0 장마감, 1 시간외,  2 정규장, 3 단일가
    809       802,804        801          805
NXT
0 장마감, 1 프리, 2 메인, 3 애프터
    890        881        884      888
*/
	if (IsCurrentTimeBetween(7 + (m_bCSAT * 1), 0, 7 + (m_bCSAT * 1), 50))  //7 00 ~ 7 50
	{
		m_iKRXype = 5;   //KRX프리
		m_iNXType = 5;  //NXT장전
	}
	else if (IsCurrentTimeBetween(7 + (m_bCSAT * 1), 50, 8 + (m_bCSAT * 1), 00))  //7 50 ~ 8 00
	{
		m_iKRXype = 4;   //KRX장전
		m_iNXType = 5;  //NXT장전
	}
	else if (IsCurrentTimeBetween(8 + (m_bCSAT * 1), 0, 8 + (m_bCSAT * 1), 30))  //8 00 ~ 8 30
	{
		m_iKRXype = 4;   //KRX장전
		m_iNXType = 1;  //NXT프리
	}
	else if (IsCurrentTimeBetween(8 + (m_bCSAT * 1), 30, 8 + (m_bCSAT * 1), 40))  //8 30 ~ 8 40
	{
		m_iKRXype = 1;   //KRX시간외
		m_iNXType = 1;  //NXT프리
	}
	else if (IsCurrentTimeBetween(8 + (m_bCSAT * 1), 40, 8 + (m_bCSAT * 1), 50))  //8 40 ~ 8 50
	{
		m_iKRXype = 4;  //KRX장전
		m_iNXType = 1;  //NXT프리
	}
	else if (IsCurrentTimeBetween(8 + (m_bCSAT * 1), 50, 9 + (m_bCSAT * 1), 00))  //8 50 ~ 9 00
	{
		m_iKRXype = 3;  //KRX단일가
		m_iNXType = 5;  //장전
	}
	else if (IsCurrentTimeBetween(9 + (m_bCSAT * 1), 0, 15 + (m_bCSAT * 1), 0))  //9 00 ~ 15 00
	{
		m_iKRXype = 2;  //정규장
		m_iNXType = 2;  //정규장
	}
	else if (IsCurrentTimeBetween(15 + (m_bCSAT * 1), 0, 15 + (m_bCSAT * 1), 20))  //15 00 ~ 15 20
	{
		m_iKRXype = 2;  //정규장
		m_iNXType = 2;  //정규장
	}
	else if (IsCurrentTimeBetween(15 + (m_bCSAT * 1), 20, 15 + (m_bCSAT * 1), 30))  //15 20 ~ 15 30
	{
		m_iKRXype = 2;  //정규장
		m_iNXType = 0;  //장전
	}
	else if (IsCurrentTimeBetween(15 + (m_bCSAT * 1), 30, 15 + (m_bCSAT * 1), 40))  //15 30 ~ 15 40
	{
		m_iKRXype = 4;  //KRX장전
		m_iNXType = 4;  //단일가
	}
	else if (IsCurrentTimeBetween(15 + (m_bCSAT * 1), 40, 16 + (m_bCSAT * 1), 0))  //15 40 ~ 16 00
	{
		m_iKRXype = 1;  //시간외
		m_iNXType = 3;  //애프터
	}
	else if (IsCurrentTimeBetween(16 + (m_bCSAT * 1), 0, 18 + (m_bCSAT * 1), 0))  //16 00 ~ 18 00
	{
		m_iKRXype = 6;  //애프터
		m_iNXType = 3;  //애프터
	}
	else if (IsCurrentTimeBetween(18 + (m_bCSAT * 1), 0, 20 + (m_bCSAT * 1), 0))  //18 00 ~ 20 00
	{
		m_iKRXype = 6;    //애프터
		m_iNXType = 3;    //애프터
	}
	else if (IsCurrentTimeBetween(20 + (m_bCSAT * 1), 0, 24 + (m_bCSAT * 1), 0))  //20 00 ~ 24 00
	{
		m_iKRXype = 0;  //장마감
		m_iNXType = 0;  //장마감
	}
	else
	{
		m_iKRXype = 0;  //장마감
		m_iNXType = 0;  //장마감
	}

#ifdef   DF_MK_CAPTION
	m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);   
#endif
}

bool CMainFrame::IsCurrentTimeBetween(int startHour, int startMin, int endHour, int endMin)
{
	CTime now = CTime::GetCurrentTime();
	int curMinutes = now.GetHour() * 60 + now.GetMinute();
	int startMinutes = startHour * 60 + startMin;
	int endMinutes = endHour * 60 + endMin;

	// 시작 시간이 종료 시간보다 앞에 있는 경우 (같은 날)
	if (startMinutes < endMinutes)
	{
		return (curMinutes >= startMinutes && curMinutes < endMinutes);
	}
	else if (startMinutes > endMinutes)
	{
		return (curMinutes >= startMinutes || curMinutes < endMinutes);
	}
	else
	{
		return false;
	}
}
#endif

CWnd* CMainFrame::getActiveView(int key)
{
	CChildFrame* child{};
	CSChild* schild{};

	if (m_arMDI[m_vsN].Lookup(key, child))
		return child->GetActiveView();
	if (m_arSDI[m_vsN].Lookup(key, schild))
		return schild->GetActiveView();
	return NULL;
}

void CMainFrame::load_history()
{
	if (!m_tMenu)	return;

	CProfile profile(pkAxisUser);

	m_savelist.RemoveAll();
	CString maps(profile.GetString(szSystem, "maps"));
	//AfxMessageBox(maps);
	if (maps.IsEmpty())
		return;

	CString tmpS;
// 	CString s;
// 	s.Format("MAPS [%s]\n",maps);
// 	OutputDebugString(s);

	while (!maps.IsEmpty())
	{
		const int pos = maps.Find(',');
		if (pos == -1)
		{
// 			s.Format("HISTORY MAP [%s]\n",maps);
// 			OutputDebugString(s);
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
// 			s.Format("HISTORY MAP [%s]\n",tmpS);
// 			OutputDebugString(s);
			add_history(tmpS);
			addTool_history(tmpS);
		}
	}
}

void CMainFrame::save_history()
{
	CProfile profile(pkAxisUser);
	CString str(""), maps;

	for (int i = 0; i < m_savelist.GetSize(); i++)
	{
		if (!i)
			maps = m_savelist.GetAt(i);
		else
			maps.Format(", %s", m_savelist.GetAt(i));

		str += maps;
	}

	profile.Write(szSystem, "maps", str);
}

void CMainFrame::add_history(CString mapN)
{
	if (mapN.GetLength() != L_MAPN)	return;

	CString	file, maps, tmpS, tail;
	for (int ii = 0; ii < m_savelist.GetSize(); ii++)
	{
		maps = m_savelist.GetAt(ii);
		tail = maps.Mid(6, 2);
		// 화면 히스토리 오류 수정 - dkkim 2016.01.08
		//if (tail.CompareNoCase("00"))	continue;
		if (!mapN.CompareNoCase(maps))	return;
	}
	m_savelist.Add(mapN);
}

void CMainFrame::addTool_history(CString mapName)
{
	CString	dat, mapN = mapName.Left(L_MAPN);
	CString	tail = mapN.Mid(6);

	//if (tail.CompareNoCase("00"))	return;

	if (!ExistMenu(mapN.Left(L_MAPN-2)+"00")) return;
	dat.Format("%s%s%s", m_tMenu->GetDispN(mapN), mapN, m_tMenu->GetDesc(mapN));

	m_screenHistory.Add(mapN);

	m_bar1->add_History(dat);
	m_smain->add_History(dat);
}

// for device............
void CMainFrame::view_Hook(int key)
{
return;
	const CView* view = (CView *) getActiveView(key);
	if (!view)	return;
	view->SendMessage(WM_VIEWHOOK, 0, 0);
}

void CMainFrame::view_UnHook()
{
	if (m_callproc && m_view)
		SetWindowLong(m_view->GetSafeHwnd(), GWL_WNDPROC, (LONG)m_callproc);

	m_view = NULL;
	m_callproc = NULL;
}

void CMainFrame::load_tabview()
{
	CProfile profile(pkAxisTab);
	CString str, key, dat;

	for (int i = 0; i < 999; i++)
	{
		key.Format("%03d", i);
		str = profile.GetString(szTabView, key);
		if (str.IsEmpty())
			continue;

		while (!str.IsEmpty()) 
		{
			const int pos = str.Find(',');
			if (pos == -1)
			{
				Trim(str);
				if (str.GetLength() == L_MAPN)
				{
					if (ExistMenu(str))
						m_tabviewlist.SetAt(str, key);
				}
				break;
			}

			dat = str.Left(pos);
			str = str.Mid(pos+1);
			Trim(dat);
			
			if (dat.GetLength() == L_MAPN)
			{
				if (ExistMenu(dat))
					m_tabviewlist.SetAt(dat, key);
			}
		}
	}
}

bool CMainFrame::IsTabView(CString mapN)
{
	CString	key;
	if (m_tabviewlist.Lookup(mapN, key))
	{
		return ExistMenu(mapN);
	}
	return false;
}

bool CMainFrame::IsTabChange(CString mapN, int &key)
{
	int	childkey{};
	CString	tKey, oKey;
	if (!m_tabviewlist.Lookup(mapN, tKey))
		return false;

	CChildFrame* pChild{};
	CSChild* schild{};

	BOOL fromSingleMap = FALSE;
	if (!m_arMDI[m_vsN].Lookup(key, pChild) && m_arSDI[m_vsN].Lookup(key, schild))
		fromSingleMap = TRUE;

	if (!fromSingleMap)
	{
		for (POSITION pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arMDI[m_vsN].GetNextAssoc(pos, childkey, pChild);
			if (!pChild->IsTabView())		
				continue;
			if (!m_tabviewlist.Lookup(pChild->m_mapN, oKey))
				continue;
			if (tKey != oKey)		
				continue;

			key = childkey;

			return true;
		}
	}
	else
	{
		for (POSITION pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arSDI[m_vsN].GetNextAssoc(pos, childkey, schild);
			if (!schild->IsTabView())		
				continue;
			if (!m_tabviewlist.Lookup(schild->m_mapN, oKey))
				continue;
			if (tKey != oKey)		
				continue;
			key = childkey;
			return true;
		}
	}
	return false;
}

CString CMainFrame::GetTabKey(CString mapN)
{
	CString	key;
	if (m_tabviewlist.Lookup(mapN, key))
		return key;
	return _T("");
}

void CMainFrame::change_Resource()
{
	CString		skinName = GetSkinName();
	m_resourceHelper->ChangeRES(skinName,true);

	int		key{};
	CSChild* schild{};
	CChildFrame* child{};
	for (int ii = V_SCREEN1; ii <= V_SCREEN6; ii++)
	{
		for (POSITION pos = m_arMDI[ii].GetStartPosition(); pos; )
		{
			m_arMDI[ii].GetNextAssoc(pos, key, child);
			child->ChangeSkin(skinName);
			if (!child->IsTabView())	
				continue;
			CView* view = child->GetActiveView();
			((CAxTabView *) view)->ChangeSkin();
		}

		for (POSITION pos = m_arSDI[ii].GetStartPosition(); pos; )
		{
			m_arSDI[ii].GetNextAssoc(pos, key, schild);
			schild->ChangeSkin(skinName);
			if (!schild->IsTabView())	
				continue;
			CView* view = schild->GetActiveView();
			((CAxTabView *) view)->ChangeSkin();
		}
	}
}

void CMainFrame::SetConclusion()
{
	CProfile profile(pkOrderSetup);

	m_conKIND = profile.GetInt(szCommon, "CK_RTF", CON_POPUP | CON_SOUND);
	m_conHISTORY = profile.GetInt(szCommon, "CK_HISTORY", 20);
	m_conAUTOTIME = profile.GetInt(szCommon, "CK_AUTOHIDE", 0);
	m_conPOS = profile.GetInt(szCommon, "CK_POS", 5);

	if(m_conHISTORY == 0)
	{
		m_conHISTORY = 300;
	}

	const char* const szSoundFiles[] = 
	{
		"SOUNDFILE_MDORDER", "SOUNDFILE_MSORDER", "SOUNDFILE_JJORDER", "SOUNDFILE_CSORDER",
		"SOUNDFILE_MDCON", "SOUNDFILE_MSCON", "SOUNDFILE_REFUSAL", NULL
	};

	const char* const szDefSoundFiles[] =
	{
		"\\image\\매도주문접수.WAV",
		"\\image\\매수주문접수.WAV",
		"\\image\\정정주문접수.WAV",
		"\\image\\취소주문접수.WAV",
		"\\image\\매도주문체결.WAV",
		"\\image\\매수주문체결.WAV",
		"\\image\\주문거부.WAV"
	};

	for (int i = 0; szSoundFiles[i]; i++)
	{
		m_waveF[i] = profile.GetString(szCommon, szSoundFiles[i]);

		// 2011.02.23 사운드파일이 정상이 아닐경우 Default로 설정해준다.
		WIN32_FIND_DATA wfd;
		memset(&wfd, 0, sizeof(wfd));
		HANDLE handle = FindFirstFile(m_waveF[i], &wfd);
		if (handle==INVALID_HANDLE_VALUE)
		{
			m_waveF[i] = Axis::home + szDefSoundFiles[i];
			profile.Write(szCommon, szSoundFiles[i], (LPCSTR)m_waveF[i]);
		}
		else
		{
			FindClose(handle);
		}
	}

	m_matchToolBar = profile.GetInt(szCommon, "MatchToolBar");
	m_matchToolCount = profile.GetInt(szCommon, "MatchToolCount", 3);

	if (m_tInfo3)
	{
		m_tInfo3->SetCols(m_matchToolCount);
#ifdef DF_USE_CPLUS17
		if (m_matchToolBar == 1)
			ShowControlBar(m_tInfo3.get(), TRUE, FALSE);
		else
			ShowControlBar(m_tInfo3.get(), FALSE, FALSE);
#else
		if (m_matchToolBar == 1)
			ShowControlBar(m_tInfo3, TRUE, FALSE);
		else
			ShowControlBar(m_tInfo3, FALSE, FALSE);
#endif
	}
	
	
	// 20070330 메세지확인창:ledger에서 msgbox 띄우라는 field check안하도록
	// wizard 0:기본메세지나오게,1:메세지안나오도록
	// file 0:안띄움, 1:띄움
	/**
	long	rc = 0;
	int value = profile.GetInt(szCommon, "ShowMsg2", 1);
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setNOMSG, 0), (value ? 0 : 1));
	**/

	CProfile p(pkEnvironment);
	long	rc = 0;
	const int value = p.GetInt(szScreen, "msgbox", 1);
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setNOMSG, 0), (value ? 0 : 1));
}

//modi 서버주문
void CMainFrame::ShowServerOrdDlg()
{
	char	buf[256]{};
	CString	file, strtmp;;
	file.Format("%s\\tab\\axisAI.ini", Axis::home);
	DWORD dw = GetPrivateProfileString("ServerOrd", "show", "0", buf, sizeof(buf), file);

	if (dw < 0)
		return;

	strtmp.Format("%s", buf);

	if (m_pServerOrd && (strtmp == "1"))
	{
		if (m_pServerOrd->GetStyle() & WS_VISIBLE)
			m_pServerOrd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
		else
		{
			int	x{}, y{};
			CRect	mRc, wRc;

			mRc = GetWndRect();
			m_pServerOrd->GetWindowRect(wRc);

			switch (m_conPOS)
			{
			case 5:
			{
				x = mRc.right - wRc.Width() - GetSystemMetrics(SM_CXFIXEDFRAME);
				y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;

				if (m_conclusion)
				{
					CRect rec;
					m_conclusion->GetWindowRect(rec);
					y -= rec.Height();
				}
				else
					y -= 185;
			}
			break;
			default:
				x = mRc.right - wRc.Width() - GetSystemMetrics(SM_CXFIXEDFRAME);
				y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;

				x -= 10;
				y += 30;
				break;
			}

			m_pServerOrd->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
			m_pServerOrd->MoveWindow(x, y, wRc.Width(), wRc.Height());
			CString slog;
			slog.Format("[conclusion] x=[%d] y=[%d] w=[%d] H=[%d]", x, y, wRc.Width(), wRc.Height());
			OutputDebugString(slog);

		}
	}
}

void CMainFrame::ShowConclusion()
{
	if (m_conclusion)
	{
		if (m_conclusion->GetStyle() & WS_VISIBLE)
			m_conclusion->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
		else
		{
			int	x{}, y{};
			CRect	mRc, wRc;

			mRc = GetWndRect();
			m_conclusion->GetWindowRect(wRc);

			switch (m_conPOS)
			{
			case 0:	// left - top
				x = mRc.left;	y = mRc.top;
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;
				break;
			case 1:	// center
				x = mRc.CenterPoint().x - wRc.Width()/2;
				y = mRc.CenterPoint().y - wRc.Height()/2;
				break;
			case 2:	// right - top
				x = mRc.right - wRc.Width() - GetSystemMetrics(SM_CXFIXEDFRAME);
				y = mRc.top;
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;
// 				if (mRc.right > GetSystemMetrics(SM_CXSCREEN))
// 					x = GetSystemMetrics(SM_CXSCREEN) - wRc.Width();
				break;
			case 3:	// left - bottom
				x = mRc.left;
				y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;
// 				if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
// 					y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();
				break;
			case 4:	// mouse pos
				{
					CPoint	point(0, 0);
					GetCursorPos(&point);
					x = point.x;	y = point.y;
					if (x < 0)	x = 0;
					if (y < 0)	y = 0;

// 					if (x > GetSystemMetrics(SM_CXSCREEN) - wRc.Width())
// 						x = GetSystemMetrics(SM_CXSCREEN) - wRc.Width();
// 					if (y > GetSystemMetrics(SM_CYSCREEN) - wRc.Height())
// 						y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();
				}
				break;
			default:
			case 5:	// right - bottom
				x = mRc.right - wRc.Width() - GetSystemMetrics(SM_CXFIXEDFRAME);
				y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;
// 				if (mRc.right > GetSystemMetrics(SM_CXSCREEN))
// 					x = GetSystemMetrics(SM_CXSCREEN) - wRc.Width();
// 				if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
// 					y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();
				break;
			}
			
			m_conclusion->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
			m_conclusion->MoveWindow(x, y, wRc.Width(), wRc.Height());
CString slog;
slog.Format("[conclusion] x=[%d] y=[%d] w=[%d] H=[%d]", x, y, wRc.Width(), wRc.Height());
OutputDebugString(slog);

		}
	}
}

void CMainFrame::ShowEConclusion(CString dat, bool bFail)
{
	
	if (m_Econclusion)
	{
		
		m_Econclusion->StopSlide();
		m_Econclusion->SetData(dat, bFail);

		CRect		mRc;
		WINDOWPLACEMENT	pl;
		GetWindowPlacement(&pl);
		switch (pl.showCmd)
		{
		case SW_HIDE:case SW_MINIMIZE:case SW_SHOWMINIMIZED:
			m_Econclusion->ShowWindow(SW_HIDE);
			return;
		default:
			break;
		}
		m_Econclusion->ShowWindow(SW_HIDE);
		mRc = GetWndRect();
		m_Econclusion->ShowSlide(mRc);

		SetTimer(TM_AUTOMSN, 3000, NULL);
	}
}

void CMainFrame::ShowMngInfo(DWORD* dat)
{
	if (m_mngInfo)
	{
		CString		sym, val, str, sMngType{};
		CString		msgS;
		const int		pos = 0;
		int		kind = 0;
		CString		wavfile;
	
		if (!dat[601])	// KEY(601)
			return;

		val = (char*)dat[601];
		sMngType = val;
// 		for(int i=0;i<1000;i++)
// 		{
// 			CString s;
// 			s.Format("MNG INFO [%d][%s]\n",i,(char*)dat[i]);
// 			OutputDebugString(s);
// 		}
		// KOBA ELW 조기종료 안내
		if (val==MNG_KOBAELW && m_bKobaElwNotify)
		{
			if (dat[23])
			{
				val = (char*)dat[23];

				int x{}, y{};
				CRect mRc, wRc;
				
				mRc = GetWndRect();
				m_kobanotify->GetWindowRect(wRc);

				x = mRc.left;
				y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;
				if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
					y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();
				
				m_kobanotify->AddNotify(val);
				m_kobanotify->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
				m_kobanotify->MoveWindow(x, y, wRc.Width(), wRc.Height());
			}
		}

#ifdef DF_MK_CAPTION
		CheckMarketByMNG(val);
		if (val == "884")  //우선 NXT 장시작은 skip
			return;
#endif

		if (!m_mapAlarmList.Lookup(val, str))
		{
			m_slog.Format("[mnginfo][%s][%d] val = [%s]  is not in mngsetting", __FUNCTION__, __LINE__, val);
			OutputDebugString(m_slog);
			return;
		}
		
// 		if (!fms.Lookup(MNG_KIND, sym))	// 0:장운영정보,1:알림
// 			return;
		if(!dat[47])
			return;
		sym = (char*)dat[47];

		if (!m_bUseAlarm)
			return;

		kind = atoi(sym);
		val = _T("");
		if(dat[23])
			msgS = (char*)dat[23];
		m_mngInfo->StopSlide();
		str = _T("");
		if (m_bSound && dat[600])
		{
			str = (char*)dat[600];
			if (!str.IsEmpty())	
			{
				wavfile.Format("%s\\%s\\%s.wav", Axis::home, IMAGEDIR, str);
				if (!IsFileExist(wavfile))  wavfile.Format("%s\\%s\\%s.wav", Axis::home, IMAGEDIR, "000");
				sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
				sndPlaySound(wavfile, SND_NODEFAULT|SND_ASYNC);
			}
		}

		if(msgS.GetAt(0) == 'X')
		{
			msgS = msgS.Mid(1);
			kind = 3;
		}
		else if(msgS.GetAt(0) == 'W')
		{
			msgS = msgS.Mid(1);
			kind = 4;
		}
		else if(msgS.GetAt(0) == 'Y')
		{
			msgS = msgS.Mid(1);
			msgS.Remove('\n');
			msgS.Remove('\r');
			kind = 5;
		}

#ifdef  DF_MNG_THREAD
	//	if (kind != 3 && kind != 4 && kind != 5)
		if (1)
		{
			_vMngInfo.emplace_back(std::move(msgS), std::move(msgS));
			ShowMngInfo(msgS, kind, "thread");  //진짜사용
		}
		else
			ShowMngInfo(msgS, kind);
#else
		ShowMngInfo(msgS, kind);
#endif
	}
}

void CMainFrame::ShowMngInfo(CString dat)
{
	// {      2, 802,  "주식 시간외개시"       },
	// {      3, 803,  "주식 시간외종료"       },
	// {      4, 804,  "주식 시간외개시"       },
	// {      5, 805,  "주식 단일개개시"       },
	// {      6, 806,  "주식 단일가종료"       },
	// SAMPLE
	// #define SYM_MNG		"XXXXX"
	// #define MNG_KIND		"047"	0:장운영정보,1:알림
	// #define MSG_MGUBN		"600"	장구분
	// #define MNG_FLAG		"601"	KEY
	// #define MNG_MSG		"023"	message
	
	// #define NO_CHECK		200
	// XXXXX	047	1	600	000	601	35	023	장개시 10초전
	// XXXXX	047	0	600	801	601	801	023	장개시 
	//AfxMessageBox(dat);



	if (m_mngInfo)
	{
		CMapStringToString	fms;
		CString		sym, val, str;
		CString		msgS;
		int		pos = 0, kind = 0;
		CString		wavfile;

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
	
		if (!fms.Lookup(MNG_FLAG, val))	// KEY(601)
			return;

		// KOBA ELW 조기종료 안내
		if (val==MNG_KOBAELW && m_bKobaElwNotify)
		{
			if (fms.Lookup("023", val))
			{
				int x{}, y{};
				CRect mRc, wRc;
				
				mRc = GetWndRect();
				m_kobanotify->GetWindowRect(wRc);

				x = mRc.left;
				y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;
				if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
					y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();
				
				m_kobanotify->AddNotify(val);
				m_kobanotify->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
				m_kobanotify->MoveWindow(x, y, wRc.Width(), wRc.Height());
			}
		}

		if (atoi(val) <= NO_CHECK)
		{
			if (!m_mapAlarmList.Lookup(val, str))
				return;
		}
		
		if (!fms.Lookup(MNG_KIND, sym))	// 0:장운영정보,1:알림
			return;
		if (!m_bUseAlarm)
			return;

		kind = atoi(sym);
		val = _T("");
		fms.Lookup(MNG_MSG, msgS);			
		m_mngInfo->StopSlide();
		str = _T("");
		if (m_bSound && fms.Lookup(MSG_MGUBN, str))
		{
			if (!str.IsEmpty())	
			{
				wavfile.Format("%s\\%s\\%s.wav", Axis::home, IMAGEDIR, str);
				if (!IsFileExist(wavfile))  wavfile.Format("%s\\%s\\%s.wav", Axis::home, IMAGEDIR, "000");
				sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
				sndPlaySound(wavfile, SND_NODEFAULT|SND_ASYNC);
			}
		}

		if(msgS.GetAt(0) == 'X')
		{
// 			OutputDebugString("============================================================\n");
// 			OutputDebugString("Bullon Notice Call\n");
// 			OutputDebugString("============================================================\n");
			msgS = msgS.Mid(1);
			kind = 3;
		}

		ShowMngInfo(msgS, kind);  //showmnginfor(CString dat)
	//	//ref 장운영  데이터를 우선 서버자동주문에 넣어봤다.. 추후게 지워야함 
	//	CTime time;
	//	time = CTime::GetCurrentTime();
	//	val.Empty();
	//	if (!fms.Lookup(MNG_FLAG, val))	// KEY(601)
	//		return;
	//	
	//	str.Format("update_ticker[%02d:%02d:%02d]\t  sym[%s] msgS[%s]", time.GetHour(), time.GetMinute(), time.GetSecond(), val, msgS);
	//	if (m_pServerOrd && Axis::devMode)
	//		m_pServerOrd->AddServerOrd(str, 100);
	}
}

//modi 서버자동
void CMainFrame::ServerOrdNotice(CString dat)
{
	if (!m_runAxis)	return;

	CString			str, value, jcode;
	CMapStringToString	ary;

	CProfile profile(pkOrderSetup);

	NoticeParse(dat, ary);
	dat.Empty();
	
	if (m_pServerOrd)
	{
		if (ary.Lookup("951", value))	  // time
			dat = value;
		else	dat.Empty();
		dat += "\t";

		if (ary.Lookup("952", value))	// contents
		{
			str.Format("%s", value);
			str.TrimRight();
			str.TrimLeft();
			dat += str;
		}
		dat += "\t";

		if (ary.Lookup("950", value))	// contents type
		{
			value.TrimRight();
			value.TrimLeft();
			dat += value;
		}
		dat += "\t";

	
		if(atoi(value) == SERVERORDER_MSG_RELOAD)  //화면에서 내역재조회
			ServerOrderMsgToMap(atoi(value));
		//else if(atoi(value) == SERVERORDER_MSG_MISORDER)  //착오주문
		//	ServerOrderMsgToMap(atoi(value));
		else
		{
			m_pServerOrd->AddServerOrd(dat, m_serverOrdHISTORY);
			ShowServerOrdDlg();
		}
	}

	ary.RemoveAll();
}

BOOL CMainFrame::ConclusionNotice(CString dat, CString& title)
{
	//if ((m_dept == "811") || (m_dept == "812")) return false;
	CString strback;
	strback = dat;

	if (m_chaser && m_chaser->GetSafeHwnd())
	{
		OnCHASER(MAKEWPARAM(dat.GetLength(), x_CONs),
			(LPARAM) (char*)(const char*)dat);
	}
	//TRACE("RD(ORIGINAL): "+dat+"\n");

//	if (m_block)	return FALSE;
	if (!m_runAxis)	return FALSE;

	CString			str, value, jcode;
	CMapStringToString	ary;

	CProfile profile(pkOrderSetup);

	NoticeParse(dat, ary);

#ifdef DF_SERVERAUTO
	if (ary.Lookup("950", str) && ary.Lookup("951", str) && ary.Lookup("952", str))
	{
		ServerOrdNotice(dat);  //modi 서버자동 noticePAN
		return FALSE;
	}
#endif

	if (!ary.Lookup("902", str))		// user ID
	{
m_slog.Format("no 902 symbol  Alldata = [%s]", strback);

//m_slog.Format("[NOTICE] 902 유저아이디 없다!!  리턴 = [%s]  ", str);
//OutputDebugString(m_slog);

		ary.RemoveAll();
		return FALSE;
	}
	// 2013.03.13 dkkim - 체결통보 변경으로 제거
	if ((m_dept == "811") || (m_dept == "812"))  //법인영업 1,2팀일때만
		if (Axis::userID != str) return FALSE;  //본인이 낸 주문이 아니라면 필터링
	//TRACE("RD: "+str + "]" + Axis::userID+"\n");
	

	CString strOdrUser;
	
// 	s.Format("ID COMPARE [%s] [%s]\n",str,Axis::userID);
// 	OutputDebugString(s);

// 	if(str == Axis::userID)
// 	{
// 		OutputDebugString("ID SAME\n");
// 	}
// 관리자 주문체결통보를 위해!! for test
#if 1
	if (str.CompareNoCase(Axis::userID))		// check userID
#else
	if (FALSE)
#endif
	{
m_slog.Format("notice ID = [%s]  login ID not same =[%s]  alldata=[%s]", str, Axis::userID, strback);
WriteUpLog("notice.ini", m_slog);
		ary.RemoveAll();
		return FALSE;
		//strOdrUser = _T("★ ");
		// 대리인 주문 관련 부분
// 		CString strAcc = Variant(loadACC);
// 
// // 		s.Format("HTS ACC [%s] [%d]\n",strAcc,Axis::isCustomer);
// // 		OutputDebugString(s);
// 		
// 		if(ary.Lookup("901", value))
// 		{
// 			if(strAcc.Find(value) < 0)
// 			{
// 				ary.RemoveAll();
// 				return FALSE;
// 			}
// 		}
	}

	if (!ary.Lookup("988", str))		// 처리구분
	{
//m_slog.Format("[NOTICE] no 988 symbol  Alldata = [%s]", strback);
//WriteUpLog("notice.ini", m_slog);
		ary.RemoveAll();
		return FALSE;
	}

	ary.Lookup("907", jcode);		// jCODE

	dat.Empty();
	bool	bCon = false;
	bool	bFail = false;
	int		priceUnit = 1;

	if (str.Find("체결") != -1)
		bCon = true;
	else if (str.Find("거부") != -1)
		bFail = true;

	if (m_Econclusion && (bFail || (m_conKIND & CON_MSN)))	// 거부는 무조건 처리하기 위해
	{
		//계좌\t종목명\t매매구분\t체결수량\t체결가격
		if (bCon)
		{
			if (ary.Lookup("901", value))	// account number
			{
				dat += EncodeAcctNo(value);
			}
			dat += "\t";

			if (ary.Lookup("908", value))	// code Name
				dat += value;
			dat += "\t";

			if (ary.Lookup("912", value))	// 처리구분(mmgb+data구분)
			{
				dat += value;
				if (ary.Lookup("988", value))
					dat += value;
			}
			else	dat += " ";
			dat += "\t";

			if (ary.Lookup("931", value))	// conclusion count
			{
				str.Format("%d", atoi(value));
				dat += str;
			}
			dat += "\t";

			if (ary.Lookup("999", value))
			{
				priceUnit = atoi(value);
				if (priceUnit == 0)
					priceUnit = 1;
			}

			if (ary.Lookup("916", value))	// conclusion price
			{
				const double price = atof(value);
				if (jcode.GetLength() == 8)
					str.Format("%.2f", price / priceUnit);
				else	
					str.Format("%d", int(price / priceUnit));
				dat += str;
			}
		}
		else
		{
			if (ary.Lookup("901", value))	// account number
			{
				dat += EncodeAcctNo(value);
			}
			dat += "\t";
			
			if (ary.Lookup("908", value))	// code Name
				dat += value;
			dat += "\t";

			if (ary.Lookup("912", value))	// 처리구분(mmgb+data구분)
			{
				dat += value;
				if (ary.Lookup("988", value))
					dat += value;
			}
			else	dat += " ";
			dat += "\t";

			if (!bFail)
			{
				if (ary.Lookup("931", value))	// amount
				{
					str.Format("%d", atoi(value));
					dat += str;
				}
				dat += "\t";
			}

			if (ary.Lookup("999", value))
			{
				priceUnit = atoi(value);
				if  (priceUnit == 0)
					priceUnit = 1;
			}

			if (ary.Lookup("916", value))	// price
			{
				const double price = atof(value);
				if (jcode.GetLength() == 8)
					str.Format("%.2f", price / priceUnit);
				else	
					str.Format("%d", int(price / priceUnit));
				dat += str;
			}
			
			dat += "\t";
			if (ary.Lookup("921", value))	// 미체결
			{
				str.Format("%d", atoi(value));
				dat += str;
			}
			
		}
		
		if (ary.Lookup("988", str))	// 처리구분
		{
			
			if (str.Find("접수") != -1)
			{
				m_Econclusion->ChangeTitle(2, "매매구분");
				m_Econclusion->ChangeTitle(3, "주문수량");
				m_Econclusion->ChangeTitle(4, "주문단가");
			}
			else if (str.Find("확인") != -1)
			{
				m_Econclusion->ChangeTitle(2, "확인구분");
				m_Econclusion->ChangeTitle(3, "확인수량");
				m_Econclusion->ChangeTitle(4, "주문단가");
			}
			else if (str.Find("체결") != -1)
			{
				m_Econclusion->ChangeTitle(2, "매매구분");
				m_Econclusion->ChangeTitle(3, "체결수량");
				m_Econclusion->ChangeTitle(4, "체결단가");
			}
			else if (str.Find("거부") != -1)
			{
				m_Econclusion->ChangeTitle(2, "매매구분");
				m_Econclusion->ChangeTitle(3, "거부단가");
			}
		}
		else	m_Econclusion->ChangeTitle();

		if (bCon || bFail)	ShowEConclusion(dat, bFail);
	}

	if (m_conclusion && (bCon || bFail))
	{
		
		// 주문접수처리... filter........
		if (!bFail && !ary.Lookup("992", str))		// conclusion count
		{
m_slog.Format("no 992 symbol and bFail failse  Alldata = [%s]", strback);
WriteUpLog("notice.ini", m_slog);
			ary.RemoveAll();
			return FALSE;
		}
		
		if (!bFail && atoi(str) <= 0)
		{
			
			ary.RemoveAll();
			return FALSE;
		}

		//체결시간\t주문번호\t원주문번호\t계좌\t고객명\t종목명\t기호(1)매매구분\t체결수량\t체결가격
		if (ary.Lookup("923", value))	// time
			dat = value;
		else	dat.Empty();
		dat += "\t";

// 		if (ary.Lookup("904", value))	// order number
// 		{
// 			str.Format("%s%d",strOdrUser, atoi(value));
// 			dat += str;
// 		}

		if (ary.Lookup("904", value))	// order number
		{
			str.Format("%d", atoi(value));
			dat += str;
		}
		dat += "\t";

		if (ary.Lookup("905", value))	// original order number
		{
			str.Format("%d", atoi(value));
			dat += str;
		}
		dat += "\t";

		if (ary.Lookup("926", value))	//장구분자
		{
			if (value == "1")
				value = "KRX";
			else if (value == "2")
				value = "NXT";
			else if (value == "6")
				value = "J";
			else if (value == "S")
				value = "SOR";

			dat += value;
		}
		dat += "\t";

		if (ary.Lookup("901", value))	// account
			dat += value;
		dat += "\t";

		if (ary.Lookup("906", value))	// customer name
			dat += value;
		dat += "\t";

		if (ary.Lookup("908", value))	// code Name
			dat += value;
		dat += "\t";

		if (ary.Lookup("912", value))	// 처리구분(mmgb+data구분)
		{
			if (bCon)
			{
				str = " ";
				if (value.Find("매도") != -1)	str = "-";
				if (value.Find("매수") != -1)	str = "+";
				dat += str + value;
			}
			else	dat += " " + value;
			if (ary.Lookup("988", value))
				dat += value;
		}
		dat += "\t";
		
		if (ary.Lookup("931", value))	// amount
		{
			str.Format("%d", atoi(value));
			dat += str;
		}
		dat += "\t";

		if (ary.Lookup("999", value))
		{
			priceUnit = atoi(value);
			if (priceUnit == 0)
				priceUnit =  1;
		}

		if (ary.Lookup("916", value))	// price
		{
			const double price = atof(value);
			if (jcode.GetLength() == 8)
			{
				//int mkgb = atoi(jcode.Mid(1, 2));
				jcode.MakeUpper();

				CString sMkgb = jcode.Mid(1,2);

// 				if (mkgb>=85 && mkgb<=99)
// 					str.Format("%.f", price / priceUnit);
// 				else if (mkgb>=10 && mkgb<=59)
// 					str.Format("%.f", price / priceUnit);
// 				else
// 					str.Format("%.2f", price / priceUnit);
				// 파생제도변경
				// 2014.08.11 by dkkim
				if (sMkgb>="85" && sMkgb<="99")
					str.Format("%.f", price / priceUnit);
				else if ((sMkgb>="10" && sMkgb<="59") || (sMkgb>="B1" && sMkgb<="ZZ"))
					str.Format("%.f", price / priceUnit);
				else
					str.Format("%.2f", price / priceUnit);
			}
			else	
			{
				str.Format("%d", int(price / priceUnit));
			}
			dat += str;
		}
		
		dat += "\t";
		if (ary.Lookup("921", value))	// 미체결
		{
			str.Format("%d", atoi(value));
			dat += str;
		}
		//대리인 체결 부분
// 		dat += "\t";
// 
// 		if(ary.Lookup("973", value))
// 		{
// 			str.Format("%s", value);
// 
// // 			CString s;
// // 			s.Format("MAIN 973 [%s]\n",str);
// // 			OutputDebugString(s);
// 
// 			if(Axis::isCustomer)
// 			{
// 				if(str == "01")
// 				{
// 					str = "직";
// 				}
// 				else if( str == "02")
// 				{
// 					str = "타";
// 				}
// 				else if( str == "04")
// 				{
// 					str = "기";
// 				}
// 				else
// 				{
// 					str = " ";
// 				}
// 			}
// 			else
// 			{
// 				if( str == "01")
// 				{
// // 					CString errMsg = "직원용에서 '직'이 보임.[" + dat + "]";
// // 
// // 					trouble_shooting(errMsg);
// 					str = "직";
// 				}
// 				else if( str == "02")
// 				{
// 					str = "타";
// 				}
// 				else if( str == "04")
// 				{
// 					str = "기";
// 				}
// 				else
// 				{
// 					str = " ";
// 				}
// 			}
// 
// 			dat += str;
// 		}
		
		if (bFail)
			m_conclusion->AddFail(dat, m_conHISTORY);
		else
			m_conclusion->AddConclusion(dat, m_conHISTORY, true);

		if (bFail || (m_conKIND & CON_POPUP))
		{
			KillTimer(TM_AUTOHIDE);
			SetTimer(TM_AUTOHIDE, m_conAUTOTIME*1000, NULL);
			ShowConclusion();
		}
	}

	if (m_conKIND & CON_SOUND)
	{
		int	mmgbK  = 0; /* 0 - none, 1 - 매도, 2 - 매수 , 3 - 정정, 4 - 취소*/
		int	sndIdx = -1;
		if (ary.Lookup("912", value))	// 매매구분
		{
			if (value.Find("매도") != -1)	mmgbK = 1;
			if (value.Find("매수") != -1)	mmgbK = 2;
		}

		if (ary.Lookup("988", str))	// 처리구분
		{
			if (str.Find("접수") != -1)
			{
				if (str.Find("정정") != -1)	mmgbK = 3;
				if (str.Find("취소") != -1)	mmgbK = 4;
				switch (mmgbK)
				{
				case 1:
					if (profile.GetInt(szCommon, "SOUNDON_MDORDER"))
						sndIdx = IDX_MDORDER;
					break;
				case 2:
					if (profile.GetInt(szCommon, "SOUNDON_MSORDER"))
						sndIdx = IDX_MSORDER;
					break;
				case 3:
					if (profile.GetInt(szCommon, "SOUNDON_JJORDER"))
						sndIdx = IDX_JJORDER;
					break;
				case 4:
					if (profile.GetInt(szCommon, "SOUNDON_CSORDER"))
						sndIdx = IDX_CSORDER;
					break;
				}
			}
			else if (str.Find("체결") != -1)
			{
				switch (mmgbK)
				{
				case 1:
					if (profile.GetInt(szCommon, "SOUNDON_MDCON"))
						sndIdx = IDX_MDCON;
					break;
				case 2:
					if (profile.GetInt(szCommon, "SOUNDON_MSCON"))
						sndIdx = IDX_MSCON;
					break;
				}
			}
			else if (str.Find("거부") != -1)
			{
				sndIdx = IDX_REFUSAL;
			}
		}

		if (sndIdx != -1)
		{
			if (m_waveF[sndIdx].IsEmpty())	MessageBeep(MB_OK);
			else
			{
				sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
				sndPlaySound(m_waveF[sndIdx], SND_NODEFAULT|SND_ASYNC);
			}
		}
	}

//#ifdef _DEBUG
	
	//if (!bCon)  //접수때도 데이터를 만들어서 툴바에 뿌림
	{
		// 주문접수처리... filter........
		/*
		if (!bFail && !ary.Lookup("992", str))		// conclusion count
		{
			ary.RemoveAll();
			return FALSE;
		}
		
		if (!bFail && atoi(str) <= 0)
		{
			
			ary.RemoveAll();
			return FALSE;
		}
		*/
		
		//체결시간\t주문번호\t원주문번호\t계좌\t고객명\t종목명\t기호(1)매매구분\t체결수량\t체결가격
		if (ary.Lookup("923", value))	// time
			dat = value;
		else	dat.Empty();
		dat += "\t";
		
		if (ary.Lookup("904", value))	// order number
		{
			str.Format("%d", atoi(value));
			dat += str;
		}
		dat += "\t";
		
		if (ary.Lookup("905", value))	// original order number
		{
			str.Format("%d", atoi(value));
			dat += str;
		}
		dat += "\t";
		
		if (ary.Lookup("901", value))	// account
			dat += value;
		dat += "\t";
		// 접수 확인 메세지 일때는 계좌명이 내려와서 툴바에 표시되므로 해당부분 제거
		/*
		if (ary.Lookup("906", value))	// customer name
			dat += value;
		*/
		dat += "\t";
		
		if (ary.Lookup("908", value))	// code Name
			dat += value;
		dat += "\t";
		
		if (ary.Lookup("912", value))	// 처리구분(mmgb+data구분)
		{
			if (bCon)
			{
				str = " ";
				if (value.Find("매도") != -1)	str = "-";
				if (value.Find("매수") != -1)	str = "+";
				dat += str + value;
			}
			else	dat += " " + value;
			if (ary.Lookup("988", value))
				dat += value;
		}
		dat += "\t";
		
		if (ary.Lookup("931", value))	// amount
		{
			str.Format("%d", atoi(value));
			dat += str;
		}
		dat += "\t";
		
		if (ary.Lookup("999", value))
		{
			priceUnit = atoi(value);
			if (priceUnit == 0)
				priceUnit =  1;
		}
		
		if (ary.Lookup("916", value))	// price
		{
			const double price = atof(value);
			if (jcode.GetLength() == 8)
				str.Format("%.2f", price / priceUnit);
			else	
				str.Format("%d", int(price / priceUnit));
			dat += str;
		}
		
		dat += "\t";
		if (ary.Lookup("921", value))	// 미체결
		{
			str.Format("%d", atoi(value));
			dat += str;
		}
	}
	
	//AfxMessageBox(dat);
	if (m_tInfo3)
	{
		if ((m_tInfo3->AddData(dat, bFail)) &&
		  !(m_tInfo3->GetStyle() & WS_VISIBLE))
		{
#ifdef DF_USE_CPLUS17
			if (m_matchToolBar == 1)
				ShowControlBar(m_tInfo3.get(), TRUE, FALSE);
			else
				ShowControlBar(m_tInfo3.get(), FALSE, FALSE);
#else
			if (m_matchToolBar == 1)
				ShowControlBar(m_tInfo3, TRUE, FALSE);
			else
				ShowControlBar(m_tInfo3, FALSE, FALSE);
#endif
		}
	}
//#endif

	ary.RemoveAll();

	return FALSE;
}

void CMainFrame::NoticeParse(CString dat, CMapStringToString& ary)
{
	int	pos{}, pos2{};
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
// 		CString s;
// 		s.Format("RD [%s] [%s]\n",symbol,value);
// 		OutputDebugString(s);
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

// 		CString s;
// 		s.Format("RD [%s] [%s]\n",symbol,value);
// 		OutputDebugString(s);
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
	if (mapN == "IB985000") return true; //직원 비밀번호 설정 화면이라면 메뉴에 있는듯 인식시켜줌
	if(mapN == "IBAI0000")  return true;
	return m_tMenu->ExistMenu(mapN);
}

int CMainFrame::ExceptionProcess(CString mapN)
{
	CString	mapname = mapN.Left(L_MAPN);
	/*
	if (!mapname.CompareNoCase("DH611900"))
	{
		if (IsRunTime())
			return 0;
		else
		{
			MessageBox("본 서비스는 영업일 09:00 ~ 17:00 에만 서비스 가능합니다.", "알림", MB_OK);
			return 1;
		}
	}
	*/
	if (!mapname.CompareNoCase("IB100400"))
	{
		if (m_axMisc->MsgBox("HTS를 종료하셔도 최대 한달간 유지되는 \r\n새로운 주식자동주문을 이용하시겠어요?", "IBK 투자증권", MB_YESNO) == IDYES)
		{
			m_mapHelper->ChangeChild("IB102000");
			return TRUE;
		}
		else
			return FALSE;
	}
	if (!mapname.CompareNoCase("IB100500"))
	{
		if (m_axMisc->MsgBox("HTS를 종료하셔도 최대 한달간 유지되는 \r\n새로운 주식자동주문을 이용하시겠어요?", "IBK 투자증권", MB_YESNO) == IDYES)
		{
			m_mapHelper->ChangeChild("IB102000");
			return TRUE;
		}
		else
			return FALSE;
	}

	if (!mapname.CompareNoCase("IBXXXX00"))
		return 1;
	if (!mapname.CompareNoCase(MAPN_REALTIMEJANGO))
		return 1;
	if (!mapname.CompareNoCase(MAPN_SISECATCH1))
		return 1;

/*
	if (!mapname.CompareNoCase("IBNEWSXX"))
	{
		int	vtype = 0, size = 0, nCx, nCy, x, y, key = KEY_NEWSVIEWER;
		BOOL	rc = FALSE;
		CRect	mRc, wRc;
		
		if (m_newsviewer)
		{
			m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
						(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);
			m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);
			if (m_newsviewer->GetStyle() & WS_VISIBLE)
			{
				m_newsviewer->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_SHOWWINDOW|SWP_NOSIZE);
				
			}
			else
			{
				mRc = GetWndRect();
				m_newsviewer->GetWindowRect(wRc);

				x = mRc.left;
				y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
				if (x < 0)	x = 0;
				if (y < 0)	y = 0;
				if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
					y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();
					
				if (m_newsviewer->GetStyle() & WS_VISIBLE)
					m_newsviewer->ShowWindow(SW_HIDE);
				else	
					m_newsviewer->SetWindowPos(&wndTopMost, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
			}
			m_newsviewer->GetWindowRect(&wRc);
			m_newsviewer->SendMessage(WM_SIZE, 0, MAKELPARAM(wRc.Width(), wRc.Height()));
			m_newsviewer->Invalidate();
		}
		else
		{
			m_newsviewer = new CNewsViewer();
			if (!m_newsviewer->Create(IDD_NEWSVIEWER))
			{
				delete m_newsviewer;
				m_newsviewer = NULL;
			}

			m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
						(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);
			m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
					(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)m_newsviewer, vtype, key);
			m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);

			int	ret = 0;
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
				(BYTE *)(VTS_I4 VTS_I4), setGID, (long)MAKELONG(key, 0));

			CSize	frameGap;
			frameGap.cx  = GetSystemMetrics(SM_CXFRAME);
			frameGap.cx += GetSystemMetrics(SM_CXBORDER) * 2;

			frameGap.cy  = GetSystemMetrics(SM_CYFRAME);
			frameGap.cy += GetSystemMetrics(SM_CYBORDER) * 2;
			frameGap.cy += GetSystemMetrics(SM_CYCAPTION);

			nCx = frameGap.cx + LOWORD(size);
			nCy = frameGap.cy + HIWORD(size);
			m_newsviewer->SetWindowPos(&wndTopMost, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_HIDEWINDOW);
			
			mRc = GetWndRect();
			m_newsviewer->GetWindowRect(wRc);

			x = mRc.left;
			y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
			if (x < 0)	x = 0;
			if (y < 0)	y = 0;
			if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
				y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();
				
			m_newsviewer->SetWindowPos(&wndTopMost, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
		}
		return 1;
	}
*/
	return 0;
}

int CMainFrame::CheckMapping(CString mapN)
{
	const int	key = -1;
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
				sprintf(mapN, "");
				return;
			}
			CopyMemory(mapN, schild->m_mapN, L_MAPN);
		}
		else	CopyMemory(mapN, child->m_mapN, L_MAPN);
	}
}

void CMainFrame::SetSearchMap(char* mapN)
{
#ifdef DF_USE_CPLUS17
	CTrSearch	dlg(m_tMenu.get());
#else
	CTrSearch	dlg(m_tMenu);
#endif
	if (dlg.DoModal() == IDOK)
		sprintf(mapN, dlg.getTR());
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

		if (!user.CompareNoCase(Axis::userID))
			return dat;
	}
	return _T("");
}

void CMainFrame::childAll(CString dat)
{
	CString	mapN = dat.Left(L_MAPN);
	dat = dat.Mid(L_MAPN);

	int		key{};
	POSITION	pos{};
	CChildFrame* pChild{};
	for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);
		if (pChild->m_mapN.Left(L_MAPN) != mapN)
			continue;

		key |= 0xff00;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)dat);
		break;
	}
}

void CMainFrame::HelpLink(int key)
{
	CChildFrame* child{};
	CSChild* schild{};
	CString url = "http://www.ibks.com/HTSHELP/helptrnum.ids?trnum=";
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		if (!m_arSDI[m_vsN].Lookup(key, schild))
		{
			
			return;
		}
		RunWebBrowser(url+CString(schild->m_mapN).Mid(2,4),780,688);

		return;
	}
	RunWebBrowser(url+CString(child->m_mapN).Mid(2,4),780,688);
	
	return;
	CString	mpN;
	
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		HelpLinkS(key);
		return;
	}
	mpN = m_tMenu->GetMapping(child->m_mapN);
	if (mpN.GetLength() != L_MAPN)	return;

	CString	file, link, dat, dispN = mpN.Mid(2, 4);

	if (mpN.CompareNoCase(MAPN_SISECATCH) == 0)
	{
		dispN = "sise";
	}

	file.Format("%s\\help\\help.dat", Axis::home);
	FILE* fp;
	fopen_s(&fp, file, "r");
	if (!fp)	return;
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

	if (mpN.CompareNoCase(MAPN_SISECATCH) == 0)
	{
		dispN = "sise";
	}

	file.Format("%s\\help\\help.dat", Axis::home);
	FILE* fp;
	fopen_s(&fp, file, "r");
	if (!fp)	return;
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

		RunWebBrowser(link,780,688);
		break;
	}
	fclose(fp);
}

void CMainFrame::SmCall(int idx)
{
	if (m_smcall)
	{
		m_smcall->Show_Environment(idx);
		
		load_eninfomation(false);
		load_mngSetup();
		m_bar1->Refresh();
	}
}

void CMainFrame::toolRegister()
{
	CString	mapN;
	const CChildFrame* child = (CChildFrame*) MDIGetActive();

	if (!child)	
		return;

	mapN.Format("%s00", child->m_mapN.Left(L_MAPN-2));

	if (!ExistMenu(mapN))	
		return;

	CProfile profile(pkUserTool);

	CString tmps(profile.GetString(szUserSN, szUserEN, "0, 0")), str;
	int idx = tmps.Find(',');

	if (idx == -1) 
		idx = 0;
	else
	{
		tmps = tmps.Left(idx);
		idx = atoi(tmps);
	}

	tmps.Format("%02d", idx);
	if (profile.GetString(szUserSN, tmps).IsEmpty())
		return;
	if (!profile.GetString(tmps, mapN).IsEmpty())
		return;

	CString ssb;
	char ssb2[1024 * 4]{};

	ssb = profile.GetSectionLF(tmps);
	str.Format("%s;%s", m_tMenu->GetDesc(mapN), m_tMenu->GetSName(mapN));

	if (ssb.IsEmpty())
		profile.Write(tmps, mapN, str);
	else
	{
		//FillMemory(ssb2, ArraySize(ssb2), ' ');
		ZeroMemory(ssb2, ArraySize(ssb2));	
		CopyMemory(&ssb2[0], ssb, ssb.GetLength());
		//CopyMemory(&ssb2[str.GetLength() + L_MAPN + 2], ssb, ssb.GetLength());
		//sprintf(&ssb2[str.GetLength() + L_MAPN + 2 + ssb.GetLength()], "%s=%s", mapN, str);
		sprintf(&ssb2[ssb.GetLength()], "%s=%s", mapN, str);
		profile.WriteSection(tmps, CString(ssb2, ssb.GetLength() + str.GetLength() + L_MAPN + 2));
	}

	m_bar1->AddItem(mapN + "="+ str);
}

void CMainFrame::toolRegisterAll(int index)
{
	switch (index)
	{
	case 0:		// 모든화면 툴바등록 --> 툴바 편집
		EditTool();
		//AllScreenRegister();
		break;
	case 1:		// 사용자화면 저장
		saveUserScreen();
		break;
	default:
		break;
	}
}

void CMainFrame::AllScreenRegister()
{
	int	key{}, idx{};
	CString ssb;
	DWORD	ssL{};
	CString	file, section, str, mpN;
	CMapStringToString	ary;

	CChildFrame* child = (CChildFrame*) MDIGetActive();
	if (!child)	return;

	CProfile profile(pkUserTool);
	section = profile.GetString(szUserSN, szUserEN, "0, 0");
	idx = section.Find(',');

	if (idx == -1)
		idx = 0;
	else
	{
		section = section.Left(idx);
		idx = atoi(section);
	}

	section.Format("%02d", idx);
	ssb = profile.GetString(szUserSN, section);
	ssL = ssb.GetLength();

	if (ssL <= 0)
		return;

	ssb = profile.GetSectionLF(section);
	ssL = ssb.GetLength();

	if (ssL > 0)
	{
		CString item, tmps, mapN, string(ssb, ssL);
		while (!string.IsEmpty())
		{
			idx = string.Find('\0');
			if (idx == -1)
				break;

			item = string.Left(idx++);
			string = string.Mid(idx);

			idx = item.Find('=');
			if (idx == -1)
				continue;
			mapN = item.Left(L_MAPN);
			if (!ary.Lookup(mapN, tmps))
				ary.SetAt(mapN, "1");
		}
	}
	for (POSITION pos = m_arMDI[m_vsN].GetStartPosition(); pos; ) 
	{
		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
		mpN.Format("%s00", child->m_mapN.Left(L_MAPN-2));
		if (ary.Lookup(mpN, str))
			continue;
		if (!ExistMenu(mpN))
			continue;

		str.Format("%s=%s;%s", mpN, m_tMenu->GetDesc(mpN), m_tMenu->GetSName(mpN));

		m_bar1->AddItem(str);
		ssb += str;
		ssL += str.GetLength() + 1;
		ary.SetAt(mpN, "1");
	}

	profile.WriteSection(section, ssb);
	ary.RemoveAll();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	if (m_bClose) return;  //vc2019 test
	//CDebug::Out("=================");
	CMDIFrameWnd::OnSize(nType, cx, cy);
#ifdef DF_USE_CPLUS17
	if (m_runAxis && m_bar0->GetSafeHwnd() && (m_bar0->GetStyle() & WS_VISIBLE))
		ShowControlBar(m_bar0.get(), TRUE, FALSE);
#else
	if (m_runAxis && m_bar0->GetSafeHwnd() && (m_bar0->GetStyle() & WS_VISIBLE))
		ShowControlBar(m_bar0, TRUE, FALSE);
#endif

	//modi CMainFrame::OnSize
#ifdef DF_SLIDEWND
	if (m_pSlideWnd && m_pSlideWnd.get()->GetSafeHwnd())
	{
		if (m_pSlideWnd.get()->GetStyle() & WS_VISIBLE)
		{
			CRect rec, recAuto;
			m_pSlideWnd.get()->GetWindowRect(&recAuto);  //SWP_NOSIZE
			rec = GetWndRect();
			m_pSlideWnd->MoveWindow(rec.left, rec.top, rec.Width() / 5, rec.Height());
			m_pSlideWnd->Invalidate();
		}
	}
#endif

	if(m_top10 != nullptr && m_top10->GetSafeHwnd())
	{
		if(m_top10->m_bShown)
		{
			CRect rc,rc10;
			GetWindowRect(rc);
			
			m_top10->GetClientRect(rc10);
			
			rc.top = rc.top + 77;
			rc.left = rc.right - rc10.Width() - 3;
			rc.right = rc.left + rc10.Width();
			rc.bottom = rc.top + rc10.Height();
			
			m_top10->MoveWindow(rc);
		}
	}

	//vc2019 test?
	CRect rc;
	GetWindowRect(rc);
	rc.OffsetRect(-rc.TopLeft());
	const int idata = GetSystemMetrics(SM_CYFRAME); 
	rc.DeflateRect(GetSystemMetrics(SM_CXFRAME),
		idata);

	CRgn rgn;
	rgn.CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
	SetWindowRgn(rgn, true);

	rgn.DeleteObject();

	CChildFrame* child;
	if (m_arMDI[m_vsN].Lookup(m_BackGroundKey, child)) child->CenterWindow();
	if (m_tInfo1 && m_tInfo1->IsWindowVisible())
	{
		m_tInfo1->RedrawWindow();
		m_tInfo1->InvalidatePane();
	}
	if (m_tInfo2 && m_tInfo2->IsWindowVisible())
	{
		m_tInfo2->RedrawWindow();
		m_tInfo2->InvalidatePane();
	}
	
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

void CMainFrame::OnMove(int x, int y) 
{
	CMDIFrameWnd::OnMove(x, y);
#ifdef DF_SLIDEWND
	//modi CMainFrame::OnMove
	if (m_pSlideWnd && m_pSlideWnd.get()->GetSafeHwnd())
	{
		if (m_pSlideWnd.get()->GetStyle() & WS_VISIBLE)
		{
			CRect rec;
			rec = GetWndRect();
			m_pSlideWnd->SetWindowPos(nullptr, rec.left, rec.top, 0, 0, SWP_NOSIZE);
		}
	}
#endif
	if (m_Nclock && m_Nclock->GetSafeHwnd())
	{
		if (m_Nclock->GetStyle() & WS_VISIBLE)
			m_Nclock->OnMainWndMoved();
	}

	if (m_logodlg && m_logodlg->GetSafeHwnd())
	{
		if (m_logodlg->GetStyle() & WS_VISIBLE)

			if( m_bar1->IsWindowVisible() )
				m_logodlg->OnMainWndMoved();			
	}
	
	if(m_top10 != nullptr && m_top10->GetSafeHwnd())
	{
		if(m_top10->m_bShown)
		{
			CRect rc,rc10;
			GetWindowRect(rc);
		
			m_top10->GetClientRect(rc10);

			rc.top = rc.top + 77;
			rc.left = rc.right - rc10.Width() - 3;
			rc.right = rc.left + rc10.Width();
			rc.bottom = rc.top + rc10.Height();

			m_top10->MoveWindow(rc);
		}
	}
}


void CMainFrame::AppendAllMap()
{
	int		key{};
	CChildFrame* child{};
	CMapStringToString	arMap;
	CProfile profile(pkScreenReg);
	CString str, buf;

	for (int i = 0; ; i++)
	{
		str.Format("%04d", i);
		buf = profile.GetString(szScreen, str);
		if (buf.IsEmpty())
			break;

		arMap.SetAt(buf, str);
	}

	for (POSITION pos = m_arMDI[m_vsN].GetStartPosition(); pos; ) 
	{
		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
		if (arMap.Lookup(child->m_mapN.Left(L_MAPN), str))
			continue;

		str.Format("%04d", arMap.GetCount());
		profile.Write(szScreen, str, child->m_mapN.Left(L_MAPN));
		arMap.SetAt(child->m_mapN.Left(L_MAPN), str);
	}
}

bool CMainFrame::axCloseV(int key)
{
// 	if (m_arMDI[vsN].Lookup(key, child))
// 	{
// 		if
// 	}
	int vsN{};  //vc2019
	CChildFrame* child{};
	removeWait(key);
	if (key & winK_POPUP)
	{
		for (int ii = 0; ii < m_arHide.GetSize(); ii++)
		{
			child = m_arHide.GetAt(ii);
			if (child && child->m_key != key)
				continue;
			m_arHide.RemoveAt(ii);
			m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
					(void *)NULL, (BYTE *)(VTS_I4), (short) key);
			return true;
		}
		
		m_arGPOP.RemoveKey(key);
		m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
				(void *)NULL, (BYTE *)(VTS_I4), (short) key);
		if (m_arGPOP.IsEmpty())
			SetFocus();
		else
		{
			CGPop* pop{};
			POSITION pos = m_arGPOP.GetStartPosition();
			m_arGPOP.GetNextAssoc(pos, key, pop);
			if (pop != NULL)	// 20070823
			{
				pop->SetFocus();
				m_activeMapN = pop->m_mapN;
			}
		}
	}
	else
	{
		bool		match = false;
		CSChild*	schild = NULL;

		for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
		{
			for (int value = 0; value < m_hooklist[vsN].GetSize(); value++)
			{
				if (key != m_hooklist[vsN].GetAt(value))
					continue;
				m_hooklist[vsN].RemoveAt(value);
			}
		}

		for (vsN = V_SCREEN1; vsN <= V_SCREEN6 && !match; vsN++)
		{
			if (m_arMDI[vsN].Lookup(key, child))
			{
				match = true;

				//m_arMDI[vsN].RemoveKey(key);
				//RedrawVS(true);
				// dtitle
				RemoveMdi(vsN, key);
				m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
					(void *)NULL, (BYTE *)(VTS_I4), (short) key);
				if (m_bar2)	m_bar2->del_Button(key, vsN);
			}
			else if (m_arSDI[vsN].Lookup(key, schild))
			{
				match = true;
				//m_arSDI[vsN].RemoveKey(key);
				//RedrawVS(true);
				// dtitle
				RemoveSdi(vsN, key);
				m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
						(void *)NULL, (BYTE *)(VTS_I4), (short) key);
				if (m_bar2)	m_bar2->del_Button(key, vsN);
			}
			
			if (match && vsN == m_vsN)
			{
				if (MDIGetActive() == NULL)
				{
					m_hooklist[m_vsN].RemoveAll();
					m_activeMapN.Empty();
				}
			}
		}
	}
	return true;
}

bool CMainFrame::axFocus(int key)
{
	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		CSChild*	schild;
		if (!m_arSDI[m_vsN].Lookup(key, schild))
			return false;

		if (m_bar2)	m_bar2->change_Button(schild->m_key);

		for (int value = 0; value < m_sdiZORDER[m_vsN].GetSize(); value++)
		{
			key = m_sdiZORDER[m_vsN].GetAt(value);
			if (key == schild->m_key)
			{
				m_sdiZORDER[m_vsN].RemoveAt(value);
				break;
			}
		}
		m_sdiZORDER[m_vsN].InsertAt(0, schild->m_key);
		m_activeMapN = schild->m_mapN;
		return true;
	}
	
	if (m_bar1)	m_bar1->set_Maps(child->m_mapN);
	if (m_bar2)	m_bar2->change_Button(child->m_key);

	for (int value = 0; value < m_hooklist[m_vsN].GetSize(); value++)
	{
		key = m_hooklist[m_vsN].GetAt(value);
		if (key == child->m_key)
		{
			m_hooklist[m_vsN].RemoveAt(value);
			break;
		}
	}
	m_hooklist[m_vsN].InsertAt(0, child->m_key);
	m_activeMapN = child->m_mapN;
	IMAXSkinSet();

	return true;
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	if(nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
	{
		if(m_slideMsg != "")
			SetTimer(TM_SLIDE,1000,NULL);
	}

	if (Axis::isVista) return;
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	switch( nState )
	{
	case WA_INACTIVE:
		OnNcActivate(FALSE);
		break;
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		OnNcActivate(TRUE);
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

#ifdef DF_USE_CPLUS17
		ShowControlBar(m_bar1.get(), FALSE, FALSE);
		ShowControlBar(m_bar2.get(), FALSE, FALSE);
		ShowControlBar(m_bar3.get(), FALSE, FALSE);
#else
		ShowControlBar(m_bar1, FALSE, FALSE);
		ShowControlBar(m_bar2, FALSE, FALSE);
		ShowControlBar(m_bar3, FALSE, FALSE);
#endif	
		
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_tInfo1.get(), FALSE, FALSE);
		ShowControlBar(m_tInfo2.get(), FALSE, FALSE);
		if (m_tInfo3)
			ShowControlBar(m_tInfo3.get(), FALSE, FALSE);
#else
		ShowControlBar(m_tInfo1, FALSE, FALSE);
		ShowControlBar(m_tInfo2, FALSE, FALSE);
		if (m_tInfo3)
			ShowControlBar(m_tInfo3, FALSE, FALSE);
#endif
	

		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}
	else
	{	
		if (m_toolStatus & TB_MAINBAR)
		{
#ifdef DF_USE_CPLUS17
			ShowControlBar(m_bar1.get(), TRUE, FALSE);
			if (m_toolStatus & TB_LISTBAR)		ShowControlBar(m_bar2.get(), TRUE, FALSE);
			if (m_toolStatus & TB_HISTBAR)		ShowControlBar(m_bar3.get(), TRUE, FALSE);
#else
			ShowControlBar(m_bar1, TRUE, FALSE);
			if (m_toolStatus & TB_LISTBAR)		ShowControlBar(m_bar2, TRUE, FALSE);
			if (m_toolStatus & TB_HISTBAR)		ShowControlBar(m_bar3, TRUE, FALSE);
#endif			
		}
		
#ifdef DF_USE_CPLUS17
		if (m_toolStatus & TB_INFOBAR1)		ShowControlBar(m_tInfo1.get(), TRUE, FALSE);
		if (m_toolStatus & TB_INFOBAR2)		ShowControlBar(m_tInfo2.get(), TRUE, FALSE);
		if (m_toolStatus & TB_INFOBAR3)
			if (m_tInfo3)
				ShowControlBar(m_tInfo3.get(), TRUE, FALSE);
#else
		if (m_toolStatus & TB_INFOBAR1)		ShowControlBar(m_tInfo1, TRUE, FALSE);
		if (m_toolStatus & TB_INFOBAR2)		ShowControlBar(m_tInfo2, TRUE, FALSE);
		if (m_toolStatus & TB_INFOBAR3)
			if (m_tInfo3)
				ShowControlBar(m_tInfo3, TRUE, FALSE);
#endif
		
		
		SetWindowPlacement(&m_pl);
	}
	ChangeLogo();
	
}

void CMainFrame::changeFontSize()
{
	int	key{};
	long	rc = 0;
	POSITION pos{};

//test 2023
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, -1), MAKELONG(0, m_fontSize));
//	global font apply

	CChildFrame* child{};
	CSChild* schild{};
	for (int ii = V_SCREEN1; ii <= V_SCREEN6; ii++)
	{
		for (pos = m_arMDI[ii].GetStartPosition(); pos; )
		{
			m_arMDI[ii].GetNextAssoc(pos, key, child);
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, m_fontSize));
			child->m_xcaption.SetFontSize(m_fontSize);
			child->Invalidate();
		}

		for (pos = m_arSDI[ii].GetStartPosition(); pos; )
		{
			m_arSDI[ii].GetNextAssoc(pos, key, schild);
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, m_fontSize));
			schild->m_xcaption.SetFontSize(m_fontSize);
			schild->Invalidate();
		}
	}

	CGPop* Gpop{};
	for (pos = m_arGPOP.GetStartPosition(); pos; )
	{
		m_arGPOP.GetNextAssoc(pos, key, Gpop);
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, m_fontSize));
		Gpop->Invalidate();
	}
}

void CMainFrame::processFMX(WPARAM wParam, LPARAM lParam)
{
	const int key = LOWORD(wParam);
	const int len = HIWORD(wParam);

	if (m_chaser && m_chaser->GetSafeHwnd())
		OnCHASER(MAKEWPARAM(len, x_RCVs), (LPARAM) (char *) lParam);
	switch (key)
	{
	case 'L':	// file list
		if (m_infofile)
			m_infofile->ProcessFMX(wParam, lParam);
		break;
	case 't':	processTICK((char *) lParam);	break;	// ticker fmx
	case 'e':	processETICK((char *) lParam);	break;	// eticker fmx
	case 'I':	processINTER((char *) lParam);	break;	// inter code sise
	case 'r':	processELOG((char *) lParam);	break;	// error log write response
	case 'm':	processRTime((char*)lParam, HIWORD(wParam));	break;
	case 'l':	processNEWS((char*)lParam, HIWORD(wParam)); break;
	case 'k':	if (m_EmpPassChangeDlg) m_EmpPassChangeDlg->processOUB((char *) lParam, HIWORD(wParam)); break;
	case 254:   m_TotalAcc->ProcessAccount((char *) lParam, HIWORD(wParam)); break;
	case 253:   m_TotalAcc->ProcessGroup((char *) lParam, HIWORD(wParam)); break;
	case 'Q':	parsingOubsDN((char *) lParam, HIWORD(wParam)); break;
	case 'P':	processSecureTool((char *) lParam, HIWORD(wParam)); break;
	case 'p':	break;	// (SecureTool 등록TR, OS Report TR) 로 처리하지 않는다.
	case 'R':   processTran((char*)lParam, len); break;
	case 255:	ParsePihoitgyList((char*)lParam, len); break;
	case 252:	ParsePihoitgy((char*)lParam, len); break;
	case 'U':	ParseScrap((char*)lParam, len); break;
	case 'S':	Parse2018((char*)lParam, len); break;
	case 'H':	ParseEnca((char*)lParam, len); break;
	case 'c':	ParseSBPGT336((char*)lParam, len); break;
	case 251:   m_bar0->ParingGroupList((char*)lParam, HIWORD(wParam)); break;     //axissm::Categorypopup
	case 250:   m_bar0->ParingGroupCode((char*)lParam, HIWORD(wParam)); break;  //axissm::Categorypopup
	case 236:  ParseSAMFQ014((char*)lParam, len); break;   //test CDD
	case 235:  ParseSACMQ101((char*)lParam, len); break;  //test CDD
	case 234:  parseMemoUpload((char*)lParam, len); break;
	case 233 : 
	{
		if (m_pUpload)
			m_pUpload->ParseOub(wParam, lParam);
	}
	break;
	case 232:
	{
		CString str;  //btnai  SetPosition()
		char* pdata = (char*)lParam;

		m_slog.Format("[AXIS] [%s] ID=[%s]  pdata=[%.10s]", __FUNCTION__,  Axis::userID, pdata);
		OutputDebugString(m_slog);
		WriteLog(m_slog);

		for (int ii = 1; ii < 5; ii++)
		{
			str.Format("%c", pdata[ii]);
			if (isdigit(pdata[ii]))
				TRACE("\r\n" + str);
			else
				return;
		}

		m_slog.Format("[AXIS] 고객아이디로 접속한 직원 [%s]", Axis::userID );
		OutputDebugString(m_slog);
		WriteLog(m_slog);

		char	buf[256]{};
		CString	file, strmap, strname;
		file.Format("%s\\tab\\axisAI.ini", Axis::home);
		DWORD dw = GetPrivateProfileString("MAP", "num", "IB999900", buf, sizeof(buf), file);
		strmap.Format("%s", buf);
		strmap.TrimRight();

		memset(buf, 0x00, 256);
		dw = GetPrivateProfileString("MAP", "name", "IBK AI", buf, sizeof(buf), file);
		strname.Format("%s", buf);
		strname.TrimRight();

		CString strmenu;
		strmenu.Format("도우미\t#%s\t%s\t%s", strname, strmap, strname);

#ifdef  DF_MK_CAPTION
			//m_bar1.get()->SetShowAIBtn(0);  //안쓴다
#endif

		m_bar1.get()->SetPosition();

		//m_tMenu->AddMenu(strmenu);
	}
	break;
	case 231:  //pibopopu  조회
	{
		CString sRes;

		struct PIBOpopu_mod //최선집행의무 pibopopu 조회
		{
			char popup[1];
			char emsg[99];
			char nrec[4];
		};

		char* pbuf = new char[1024];
		FillMemory(pbuf, 1024, 0x00);
		memcpy(pbuf, (char*)lParam, sizeof(struct PIBOpopu_mod));
		PIBOpopu_mod* pdata = (PIBOpopu_mod*)pbuf;
		m_slog.Format("[axis][CMainFrame] 최선집행 pibo조회!!  nrec= [%.4s] emsg  = [%s]  \n", (char*)pdata->nrec, pdata->emsg);
		WriteLog(m_slog);

		bool btest = false;
		for (int ii = 0; ii < 4; ii++)
		{
			m_slog.Format("%s", (char*)&pdata->nrec[ii]);
			m_slog.TrimRight();
			if (m_slog == "0" || m_slog == "1")
				btest = true;
			else
				btest = false;
		}

		m_slog.Format("pibopopu nrect=[%.4s]", (char*)pdata->nrec);
		WriteLog(m_slog);

		delete[] pbuf;
		if (!btest)
		{
			if (Axis::isCustomer)
			{
#ifndef DF_POPSDI
				CDLG_Notice dlg(this);
				dlg.m_bDev = Axis::devMode;
				dlg.m_sRoot = Axis::home;
				dlg.m_sUser = Axis::user;
				if (dlg.DoModal() == IDOK)
				{

				}
#else
				m_slog.Format("[axis][CMainFrame] 최선집행 SDI 팝업 timer start \n");
				WriteLog(m_slog);
				SetTimer(TM_HTS_LOADENFORMATION_POP, 1000, nullptr);
				return;
#endif
			}
		}

		m_slog.Format("[axis][CMainFrame] 최선집행 팝업후 \n");
		WriteLog(m_slog);

		SetTimer(TM_HTS_LOADENFORMATION, 2000, nullptr);
		//load_eninfomation();
	
	}
	break;
	case 230:
	{
		CString sRes;

		struct PIBOpopu_mod //최선집행의무 pibopopu 조회
		{
			char popup[1];
			char emsg[99];
			char nrec[4];
		};

		char* pbuf = new char[1024];
		FillMemory(pbuf, 1024, 0x00);
		memcpy(pbuf, (char*)lParam, sizeof(struct PIBOpopu_mod));
		PIBOpopu_mod* pdata = (PIBOpopu_mod*)pbuf;
		m_slog.Format("[axis][CMainFrame] 최선집행 pibo등록!!  nrec= [%.4s] emsg  = [%s]  \n", (char*)pdata->nrec, pdata->emsg);
		WriteLog(m_slog);
		m_slog.Format("%s", pdata->emsg);
		m_slog.TrimRight();
		if (m_slog.Find("이벤트") >= 0 && m_slog.Find("신청") >= 0 && m_slog.Find("완료") >= 0)
		{
			char	buf[512];
			CString	file, stmp, smap;
			file.Format("%s\\tab\\AXNXT.ini", Axis::home);
			DWORD dw = GetPrivateProfileString("PDF", "sucess_msg", "중요약관/설명서 확인이 완료되었습니다.", buf, sizeof(buf), file);

			m_slog.Format("%s", buf); m_slog.TrimRight();

			displayGuide(m_slog);
		}

		delete[] pbuf;
	}
	break;
	case 229:
	{
		_ledgerH* ledger = (_ledgerH*)lParam;;
		CString ecod(ledger->ecod, sizeof(ledger->ecod));
		CString emsg(ledger->emsg, sizeof(ledger->emsg));

		m_slog.Format("[axis][CMainFrame] 코스콤 최선집행 등록 결과 [%d]   [%s]",atoi(ecod), emsg);

		if (atoi(ecod) < 1000 )
			this->SendPIBOpopu("I", 230);

		WriteLog(m_slog);
	}
	break;
	case 228:
	{
		CString sRes;

		struct PIBOjggb_mod //장운영
		{
			char gubn[1];
		};

		char* pbuf = new char[16];
		FillMemory(pbuf, 16, 0x00);
		memcpy(pbuf, (char*)lParam, sizeof(struct PIBOjggb_mod));
		PIBOjggb_mod* pdata = (PIBOjggb_mod*)pbuf;
		m_slog.Format("[axis][CMainFrame] 장시장구분(PIBOjggb) 조회결과 !!  gubn= [%s]  \n", (char*)pdata->gubn);
		OutputDebugString(m_slog);

		sRes.Format("%s", (char*)pdata->gubn);
		sRes.TrimRight();

#ifdef DF_MK_CAPTION
		/*
KRX
0 장마감, 1 시간외,  2 정규장, 3 단일가
	809       802,804        801          805
NXT
0 장마감, 1 프리, 2 메인, 3 애프터 , 4 단일가
	890        881        884      888

	/* N NXT                                          */
	/*          0. 휴장일                             */
	/*          1. 장전              ~08:00           */
	/*          2. 프리마켓     08:00~08:50           */
	/*          3. 오전휴장     08:50~09:00           */
	/*          4. 메인마켓     09:00~15:20           */
	/*          5. 오후휴장     15:20~15:30           */
	/*          6. 단일가매매   15:30~15:40           */
	/*          7. 에프터마켓   15:40~20:00           */
	/*          8. 장마감       20:00~                
*/
		switch (atoi(sRes))   //NXT
		{
			case 0:  //휴장일                       
			{
				m_iKRXype = 0;   //KRX 장마감
				m_iNXType = 0;   //NXT 장마감  
			}
			break;
			case 1:  //1. 장전              ~08:00               
			{
				m_iNXType = 5;   //NXT 장전 
			}
			break;
			/*
			//8:30 - 8:40  1 KRX시간외, 1 NXT프리 
			//8:40 - 8:50  0 KRX장마감, 1 NXT프리
			*/
			case 2:  // 2. 프리마켓     08:00~08:50       
			{
				m_iNXType = 1;   //NXT 프리마켓
			}
			break;
			case 3:  //3. 오전휴장     08:50~09:00                
			{
				m_iNXType = 0;   //NXT 장마감
			}
			break;
			case 4:  //메인마켓     09:00~15:20                    
			{
				m_iNXType = 2;    //NXT 메인
			}
			break;
			case 5:  //오후휴장     15:20~15:30                    
			{
				m_iNXType = 0;  //NXT 장마감
			}
			break;
			case 6:  //단일가매매   15:30~15:40                 
			{
				m_iNXType = 4;  //NXT 단일가
			}
			break;
			case 7:  //에프터마켓   15:40~18:00                       
			{
				m_iNXType = 3;   //NXT 애프터
			}
			break;
			/*
		//16:00 - 18:00  3 KRX단일가, 3 NXT애프터
		//18:00 - 20:00  0 KRX장마감, 3 NXT애프터
		*/
			case 8:  //장마감       20:00~                    
			{
				m_iNXType = 0; //NXT 장마감
			}
			break;
			default:
			{
				m_iKRXype = -1;
				m_iNXType = -1;
			}
		}

		//m_bar1->SetShowAIBtn(m_iKRXype, m_iNXType);  //HTS 시작후 장상태조회  pibojggb
		m_bar1->SetShowAIBtn(-1, m_iNXType);  //HTS 시작후 장상태조회  pibojggb
		Sendpibojggb("K");
#endif
	}
	break;
	case 227:
	{
		CString sRes;

		/*
		/* K 현물                                         */
/*          0. 휴장일                             */
/*          1. 장전              ~08:10           */
/*          A. 장준비       08:10~08:30           */
/*          2. 장전시간외   08:30~08:40           */
/*          3. 장전동시호가 08:40~08:50(주문접수) */
/*          B. 장전동시호가 08:50~09:00(예상호가) */
/*          4. 정규장장중   09:00~15:20           */
/*          5. 정후동시호가 15:20~15:30           */
/*          6. 장후시간외   15:30~16:00           */
/*          7. 시간외단일가 16:00~18:00           */
/*          8. 장마감       18:00~                
		*/
		struct PIBOjggb_mod //장운영
		{
			char gubn[1];
		};

		char* pbuf = new char[16];
		FillMemory(pbuf, 16, 0x00);
		memcpy(pbuf, (char*)lParam, sizeof(struct PIBOjggb_mod));
		PIBOjggb_mod* pdata = (PIBOjggb_mod*)pbuf;
		m_slog.Format("[axis][CMainFrame] 장시장구분(PIBOjggb) 조회결과 !!  gubn= [%s]  \n", (char*)pdata->gubn);
		OutputDebugString(m_slog);

		sRes.Format("%s", (char*)pdata->gubn);
		sRes.TrimRight();

		if(sRes.SpanIncluding("0123456789") == sRes)
		{
			switch (atoi(sRes))
			{
				case 0:  //휴장일                       
				{
					m_iKRXype = 0;   //KRX 장마감
				}
				break;
				case 1:  //1. 장전              ~08:10               
				{
					m_iKRXype = 4;   //KRX 장전 ~08:10       
				}
				break;
				case 2:  // 2. 장전시간외    08:30~08:40       
				{
					m_iKRXype = 1;   //KRX 시간외
				}
				break;
				case 3:  //3. 장전동시호가     08:40~08:50    주문접수           
				{
					m_iKRXype = 4;   //KRX 장전 08:40~08:50    주문접수     
				}
				break;
				case 4:  //메인마켓     09:00~15:20                    
				{
					m_iKRXype = 2;     //KRX 정규장
				}
				break;
				case 5:  // 장후동시호가    15:20~15:30                    
				{
					m_iKRXype = 2;   //KRX 정규장
				}
				break;
				case 6:  //장마감, 시간외   15:30~16:00                 
				{
					if(m_iNXType == 4)
						m_iKRXype = 0;  //KRX 장마감
					else
						m_iKRXype = 1;   //KRX 시간외
				}
				break;
				case 7:  //단일가매매   16:00~18:00                       
				{
					m_iKRXype = 3;   //KRX 단일가
				}
				break;
				case 8:  //장마감       18:00~                    
				{
					m_iKRXype = 0;  //KRX 장마감
				}
				break;
				default:
				{
					m_iKRXype = -1;
					m_iNXType = -1;
				}
			}
		}
		else
		{
			if (sRes == "A")   //장준비    08:10~08 : 30
			{
				m_iKRXype = 4;   //KRX 장전  08:10~08 : 30
			}
			else if (sRes == "B")  //B. 장전동시호가     08:50~09:00       예상체결
			{
				m_iKRXype = 3;   //KRX 단일가
			}
		}
		m_bar1->SetShowAIBtn(m_iKRXype, -1);  //HTS 시작후 장상태조회  pibojggb
	}
	break;
//	case 249: ParingGroupList(wParam, lParam); break;												  //axiscp::
//	case 248: ParingGroupCode(wParam, lParam); break;
		/*
	case 'C':   
		{
			slog.Format("[cb] processFMX ParseRTSMessage = [%s] [%d]\n ", (char*)lParam, len);
			OutputDebugString(slog);
			ParseRTSMessage((char*)lParam, len); 
		}
		break;
		*/
	default:
		{
#ifdef DF_MAIN_RTS		
		//auto it = g_mapRoute.find(key);
		//if (it != g_mapRoute.end())
		//{
		//	TR_ROUTE_INFO info = it->second;

		//	// 화면으로 돌려줄 TR 구조체 생성
		//	//ST_SEND_TR* pTr = new ST_SEND_TR;
		//	//pTr->key = info.key;     // 화면이 원래 보낸 key
		//	//pTr->datB = data;
		//	//pTr->datL = len;
		//	//pTr->stat = 2;

		//	// 화면으로 응답 전달
		//	if (::IsWindow(info.hWnd))
		//	{
		//		m_slog.Format("[2022][%s]<%d>  메인에서 tr 받고 다시 화면[%x]으로  key[%d] ", __FUNCTION__, __LINE__, info.hWnd, info.key);
		//		output_DebugString(m_slog);   //
		//		::PostMessage(info.hWnd, WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, info.key), len), lParam);
		//	}

		//	// poolKey 반환
		//	g_mapRoute.erase(it);
		//	FreePoolKey(key);
		//	return;
		//}
#endif	
			int igubn{};
			HWND hwnd = GetHWndByKey(key, igubn);
			if (hwnd)
			{
				int len = HIWORD(wParam);
				::SendMessage(hwnd, WM_MSG_MAINTOMAP, MAKEWPARAM(igubn, len), lParam);
				m_mapHWndToKey.RemoveKey(key);
				m_mapHWndToKey.SetAt(key, "");
			}
		}
		;	break;
	}
}

void CMainFrame::SendSACMQ101() //test CDD
{
	char data[1024];
	FillMemory(data, sizeof(data), ' ');
	_ledgerH* ledger = (_ledgerH*)&data[0];

	memcpy(ledger->svcd, "SACMQ101", 8);
	memcpy(ledger->usid, Axis::userID, Axis::userID.GetLength());
	ledger->odrf[0] = '1';
	ledger->mkty[0] = '3';
	memcpy(ledger->pcip, Axis::userIP, Axis::userIP.GetLength());
	memcpy((char*)&data[L_ledgerH], "00001", 5);
	memcpy((char*)&data[L_ledgerH + 5], (LPSTR)m_sjumin.GetBuffer(0), m_sjumin.GetLength());

	bool bret = sendTR("pibopbxq", data, L_ledgerH + 25, US_KEY, 235);
	m_slog.Format("[AXIS] [%s][cdd] ret=[%d] ID=[%s] ip=[%s] jumin=[%s]", __FUNCTION__,bret, Axis::userID, Axis::userIP, m_sjumin);
	OutputDebugString(m_slog);
	WriteLog(m_slog);
}

void CMainFrame::SendSAMFQ014() //test CDD
{
	char data[1024];
	FillMemory(data, sizeof(data), ' ');
	_ledgerH* ledger = (_ledgerH*)&data[0];

	memcpy(ledger->svcd, "SAMFQ014", 8);
	//memcpy(ledger->fkey, "5   ", 4);
	memcpy(ledger->usid, Axis::userID, Axis::userID.GetLength());
	ledger->odrf[0] = '1';
	ledger->mkty[0] = '3';
	memcpy(ledger->pcip, Axis::userIP, Axis::userIP.GetLength());

	bool bret = sendTR("pibopbxq", data, L_ledgerH, US_KEY, 236);
	m_slog.Format("[AXIS] [%s][cdd] ret =[%d]  ID=[%s] ip=[%s]", __FUNCTION__, bret, Axis::userID, Axis::userIP);
	OutputDebugString(m_slog);
	WriteLog(m_slog);
}

void CMainFrame::SendSBPGT336(CString sData)
{
	char data[1024];
	FillMemory(data, sizeof(data), ' ');
	_ledgerH* ledger = (_ledgerH*)&data[0];
	SBPGT336_mid* mid = (SBPGT336_mid*)&data[L_ledgerH];

	memcpy(ledger->svcd, "SBPGT336", 8);
	memcpy(ledger->fkey, "5   ", 4);
	memcpy(ledger->usid, Axis::userID, Axis::userID.GetLength());
	ledger->odrf[0] = '1';
	ledger->mkty[0] = '3';
	memcpy(ledger->pcip, Axis::userIP, Axis::userIP.GetLength());

	memcpy(mid->In, "00001", 5);
	memcpy(mid->uID, Axis::userID, Axis::userID.GetLength());
	memcpy(mid->zTrxTp, sData, 1);

	sendTR("pibopbxq", data, L_ledgerH + sizeof(SBPGT336_mid), US_ENC, 'c');
}

void CMainFrame::SendSACMT279()
{
	struct SACMT279_mid //최선집행의무 등록
	{
		char In[5];
		char TrxTp[1];
		char CIninfTy[1];
		char CIntNo[20];
		char pwd[8];
		char DlvrDt[8];
		char DlvrMthdCode[2];
		char RtpstTp[1];
		char RtpstDt[8];
		char RtpstRsnCode[2];
	};

	char data[1024];
	FillMemory(data, sizeof(data), ' ');
	_ledgerH* ledger = (_ledgerH*)&data[0];

	memcpy(ledger->svcd, "SACMT279", 8);
	memcpy(ledger->fkey, "5   ", 4);

	SACMT279_mid* mid = (SACMT279_mid*)&data[L_ledgerH];
	memcpy(mid->In, "00001", 5);
	memcpy(mid->TrxTp, "1", 1);
	memcpy(mid->CIninfTy, "4", 1);
	memcpy(mid->CIntNo, Axis::userID, Axis::userID.GetLength());

	const CTime tm(CTime::GetCurrentTime());
	CString today;
	today.Format("%04d%02d%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay());
	memcpy(mid->DlvrDt, today, today.GetLength());

	memcpy(mid->DlvrMthdCode, "05", 2);
	memcpy(mid->RtpstTp, "0", 1);
	memcpy(mid->RtpstDt, "00000000", 8);

	sendTR("pibopbxq", data, L_ledgerH + sizeof(SACMT279_mid), US_ENC, 229);
}

void CMainFrame::SendPIBOpopu(CString sGubn, int ikey)
{
	struct PIBOpopu_mid //최선집행의무 등록
	{
		char gubn[4];
		char usid[16];
		char type[1];
		char data[4];
	};

	char data[100];
	FillMemory(data, sizeof(data), ' ');
	
	PIBOpopu_mid* mid = (PIBOpopu_mid*)&data[0];
	memcpy(mid->gubn, "NXTS", 4);   //NXTF->NXTS
	memcpy(mid->type, sGubn, 1);
	memcpy(mid->data, "0100", 4);
	memcpy(mid->usid, Axis::userID, Axis::userID.GetLength());

	sendTR("pibopopu", data, sizeof(PIBOpopu_mid), US_ENC, ikey);
}

#define DF_DEV 
void CMainFrame::ParseSACMQ101(char* dat, int len)   //test CDD
{
	struct st_out_SACMQ101
	{
		char out_in[25];
		char out_out[25];
#ifdef DF_DEV
		char out_jumin[24];
#elif defined(DEV_REAL)
		char out_jumin[20];
#endif
		char out_realnamegubn[20];
		char out_custname[40];
#ifdef DF_DEV
		char out_remain[649];
#elif defined(DEV_REAL)
		char out_remain[645];
#endif
	};

	_ledgerH* ledger = (_ledgerH*)dat;
	CString ecod(ledger->ecod, sizeof(ledger->ecod));
	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	emsg.TrimRight();

	char pdata[900]{};
	memcpy(pdata, (char*)&dat[L_ledgerH], 900);
	st_out_SACMQ101* psacmq101 = (st_out_SACMQ101*)pdata;

	CString sres(psacmq101->out_realnamegubn, sizeof(psacmq101->out_realnamegubn));
	sres.TrimRight();

	m_slog.Format("[AXIS] [%s][cdd] len=[%d] ecod=[%s] sres=[%s]", __FUNCTION__, len, ecod, sres);
	OutputDebugString(m_slog);
	WriteLog(m_slog);

	if (sres.Find("주민등록") >= 0)
	{
		CString file, axisfile, usnm = Axis::user;
		file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm);
		WritePrivateProfileString("CDD/EDD", "popIB8224", "1", file);
	}
	else
		displayGuide("CDD EDD 온라인 등록은 개인 고객만 가 능합니다.");
		//m_axMisc->MsgBox("CDD EDD 온라인 등록은 개인 고객만 가 능합니다.", "IBK 투자증권");
}

void CMainFrame::ParseSAMFQ014(char* dat, int len)  //test CDD
{
	_ledgerH* ledger = (_ledgerH*)dat;
	CString ecod(ledger->ecod, sizeof(ledger->ecod));
	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	emsg.TrimRight();

	//char* pdata = (char*)&dat[L_ledgerH]; 
	char pdata[5];
	memcpy(pdata, (char*)&dat[L_ledgerH], 5);

	m_slog.Format("[AXIS] [%s][cdd] len=[%d] ecod=[%s] pdata=[%s]", __FUNCTION__, len, ecod, pdata);
	OutputDebugString(m_slog);
	WriteLog(m_slog);

	if (atoi(pdata) == 1)
		SendSACMQ101();
	else
	{
		CString file, axisfile, usnm = Axis::user;
		file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm);
		WritePrivateProfileString("CDD/EDD", "popIB8224", "0", file);
	}
}

void CMainFrame::ParseSBPGT336(char* pdata, int len)
{
	_ledgerH* ledger = (_ledgerH*)pdata;

	CString ecod(ledger->ecod, sizeof(ledger->ecod));
	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	emsg.TrimRight();


	SBPGT336_mod* pmod = (SBPGT336_mod*)&pdata[L_ledgerH];
	CString stmp;
	stmp.Format("%.2s", pmod->psRtyCnt);
	stmp.Replace("+", "");

	const int m_iCErrCnt = abs(atoi(stmp) - 5);

	m_slog.Format("[cert] error cnt =[%d] \n", m_iCErrCnt);
	OutputDebugString(m_slog);

	if (m_iCErrCnt == 5)
	{
		stmp.Format("전자서명 비밀번호 %d회 오류로 HTS가 종료됩니다.\r\n 인증서를 재발급 받으시고 다시 시도해주세요", m_iCErrCnt);
		AfxMessageBox(stmp);
		this->SendMessage(WM_CLOSE, 0, 0);
	}
	else if (m_iCErrCnt != 0)
	{
		stmp.Format("전자서명 비밀번호 오류가 %d회 발생 하였습니다.. \r\n 비밀번호를 확인하시고 다시 시도해 주시기 바랍니다.", m_iCErrCnt);
		AfxMessageBox(stmp);
	}
}

void CMainFrame::processNEWS(char* dat, int len)
{
	WriteLog("[AXIS] ProcessNEWS - Step 1");
	CString sdata(_T(""));
	sdata = (char*) dat;
	//dat = dat+4;
	CString	Path;
	//if (sdata[0] == '1') AfxMessageBox(sdata);

	Path.Format("%s\\%s\\%s\\notice.dat", Axis::home, USRDIR, Axis::user);

	WriteLog("[AXIS] ProcessNEWS - Step 2");
	
	TRY
	{
		WriteLog("[AXIS] ProcessNEWS - Step 3");
		CFile	file;
		if (!file.Open(Path, CFile::modeCreate|CFile::modeWrite))
		{
			TRACE("Create Failed\n");
			return;
		}
		WriteLog("[AXIS] ProcessNEWS - Step 4");
		file.Write(dat, len);
		WriteLog("[AXIS] ProcessNEWS - Step 5");
		file.Close();
		WriteLog("[AXIS] ProcessNEWS - Step 6");
	}
	CATCH (CFileException, e) 
	{
		char emsg[1024];
		e->GetErrorMessage(emsg, 1024);
		WriteLog("[AXIS] ProcessNEWS Error- %s", emsg);
	}
	END_CATCH;
	
	WriteLog("[AXIS] SetTimer -> %d", SetTimer(TM_NOTICE, 1000, NULL));
}

void CMainFrame::processTICK(CString dat)
{
	//OutputDebugString("PROCESSTICK START\n");
	CString tmps, value, sym, yesym;
	int	pos{}, key{};
	//OutputDebugString(dat);
	
	m_slog.Format("\r\n   ------------[CMainFrame][processTICK] dat=[%s] \r\n---------------------", dat);
	//OutputDebugString(m_slog);

	pos = dat.GetLength();
	while (!dat.IsEmpty())
	{
		pos  = dat.Find(0x0a);
		if (pos == -1)	break;
		tmps = dat.Left(pos++);
		dat  = dat.Mid(pos);

		pos  = tmps.Find(0x1F);
		key  = atoi(tmps.Left(pos++));
		tmps = tmps.Mid(pos);

		pos  = tmps.Find(0x09);
		sym  = tmps.Left(pos++);
		tmps = tmps.Mid(pos);
		value.Format("%d", key);
		
		if (sym.IsEmpty())	continue;
		if (value.IsEmpty())	continue;
		//TRACE("\nTick: %d|%s|%s\n", key, sym, tmps);
		CString s;
		s.Format("\nTick: %d|%s|%s\n", key, sym, tmps);
		OutputDebugString(s);
		switch (atoi(value))
		{
		default:
			if (m_tInfo1)	m_tInfo1->SetFmx(true, TKIND_INDEX, key, sym, tmps);
			if (m_tInfo2)	m_tInfo2->SetFmx(true, TKIND_INDEX, key, sym, tmps);
			break;
		case UPDN_KOSPI:case UPDN_KOSDAQ:
			if (m_tInfo1)	m_tInfo1->SetFmx(true, TKIND_UPDOWN, key, sym, tmps);
			if (m_tInfo2)	m_tInfo2->SetFmx(true, TKIND_UPDOWN, key, sym, tmps);
			break;
		case NEWS_BASE:
			if (m_tInfo1)	m_tInfo1->SetFmx(true, TKIND_NEWS, key, sym, tmps);
			if (m_tInfo2)	m_tInfo2->SetFmx(true, TKIND_NEWS, key, sym, tmps);
			break;
		}
	}

	if (m_tInfo1)	m_tInfo1->SetFmx(false);
	if (m_tInfo2)	m_tInfo2->SetFmx(false);

	//OutputDebugString("PROCESSTICK END\n");
}

void CMainFrame::processETICK(CString dat)
{
	CString tmps, value, sym, yesym;
	int	pos{}, key{};

	pos = dat.GetLength();

	while (!dat.IsEmpty())
	{
		pos  = dat.Find(0x0a);
		if (pos == -1)	break;
		tmps = dat.Left(pos++);
		dat  = dat.Mid(pos);

		pos  = tmps.Find(0x1F);
		key  = atoi(tmps.Left(pos++));
		tmps = tmps.Mid(pos);

		pos  = tmps.Find(0x09);
		sym  = tmps.Left(pos++);
		tmps = tmps.Mid(pos);

		value.Format("%d", key);
		if (sym.IsEmpty())	continue;
		if (value.IsEmpty())	continue;

		switch (atoi(value))
		{
		default:
			if (m_tInfo1)	m_tInfo1->SetEFmx(true, TKIND_INDEX, key, sym, tmps);
			if (m_tInfo2)	m_tInfo2->SetEFmx(true, TKIND_INDEX, key, sym, tmps);
			break;
		case UPDN_KOSPI:case UPDN_KOSDAQ:
			if (m_tInfo1)	m_tInfo1->SetEFmx(true, TKIND_UPDOWN, key, sym, tmps);
			if (m_tInfo2)	m_tInfo2->SetEFmx(true, TKIND_UPDOWN, key, sym, tmps);
			break;
		case NEWS_BASE:
			if (m_tInfo1)	m_tInfo1->SetEFmx(true, TKIND_NEWS, key, sym, tmps);
			if (m_tInfo2)	m_tInfo2->SetEFmx(true, TKIND_NEWS, key, sym, tmps);
			break;
		}
	}

	if (m_tInfo1)	m_tInfo1->SetEFmx(false);
	if (m_tInfo2)	m_tInfo2->SetEFmx(false);
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
		if (m_tInfo1)	m_tInfo1->SetInter((char*) &interMod->grid[ii]);
		if (m_tInfo2)	m_tInfo2->SetInter((char*) &interMod->grid[ii]);
	}
}

void CMainFrame::processELOG(char* dat)
{
	if (dat[0] == '0')	// error report OK
	{
		::DeleteFile(m_elogfileName);
		PostMessage(WM_ELOG);
	}
}


void CMainFrame::sendTicInfo()
{
WriteLog("[AXIS] CMainFrame::sendTicInfo");

	CString	section, keys;
	char	buf[1024 * 2]{}, ticInfo[255]{};
	int	len{}, idx = 0, kk{};
	CMapStringToString	interAry;

	ZeroMemory(ticInfo, sizeof(ticInfo));

	CProfile profile(pkAxisTicker);

	CString dval;
	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		keys.Format("BAR%02d", ii);
		const int splitN = profile.GetInt(szGeneral, keys, SPLIT_CNT);
		for (int jj = 0; jj < splitN; jj++)
		{
			section.Format("BAR_INFO_%02d", jj + ii*10);
			switch (profile.GetInt(section, "INDEXK", TKIND_INDEX))
			{
			case TKIND_INDEX:
				{
					CString	value;
					for (kk = 0; ; kk++)
					{
						keys.Format("%03d", kk);
						dval = profile.GetString(section, keys);
						/*
						if (dval == "28")
						{
							//AfxMessageBox("27 out");
							//dval = "1";
							continue;
						}
						*/
						strcpy_s(buf,  2048, dval);
						
						if (strlen(buf) < 1)
							break;
						
						len = GetTicKey(atoi(buf));
						/*
						CString tmp;
						tmp.Format("%d",len);	
						AfxMessageBox(tmp);
						*/
						ticInfo[len] = '1';
					}
				}
				break;
			case TKIND_UPDOWN:
				kk = profile.GetInt(section, "SELECT");
				switch (kk)
				{
				case 0:	case 1:
					ticInfo[UPDN_KOSPI+kk] = '1';	// 7 - base updown key
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
						strcpy_s(buf, 2048, profile.GetString(section, keys));
						if (strlen(buf) < 1)
							break;
						//if (atoi(buf) == 27) continue;
						if (interAry.Lookup(buf, value))
							continue;
						interAry.SetAt(buf, "reg");
					}
				}
				break;
			case TKIND_INVEST:
				{
					//AfxMessageBox("TKIND_INVEST");
				}
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
	for (int ii = 1 ; ii < sizeof(ticInfo); ii++)
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
	
	CString tmp(buf,sizeof(buf));
	CString s;
	s.Format("Tick SEND [%s]\n",tmp);
	OutputDebugString(s);
	
	len = idx*sizeof(struct _rtick);
	//2012.08.10 김덕기 - PIBOTICK -> PIBFTICK
	//sendTR("PIBOTICK", buf, len, US_PASS, 't');

	m_slog.Format("\r\n ----------- TICK SEND [%s] ------\r\n", CString(buf, len));
	OutputDebugString(m_slog);
	sendTR("PIBOTICK", buf, len, US_PASS, 't');
	if (interAry.GetCount())
	{
		const	int	L_code = 12;
		const	int	codeCnt = 5;/* 2 - key, 3 - code count */
		CString	codes;
		/*
		struct _iSise
		{
			char	keyf[2];	// ticker id
			char	nrec[3];	// count jcode
			char	code[L_code];	// jcode
		};
		*/
		const int	cnt = interAry.GetCount();
		const int	sndL = codeCnt + L_code*cnt + 1;
		char*	sndB = new char [sndL];
		ZeroMemory(sndB, sndL);
		len = idx = codeCnt;
		sprintf(sndB, "%05d", cnt);
		for (POSITION pos = interAry.GetStartPosition(); pos; )
		{
			interAry.GetNextAssoc(pos, codes, keys);
			
			sprintf(&sndB[idx], "%s", (LPSTR)(LPCTSTR)codes.Mid(1));
			idx += L_code;
		}
		sendTR("PIBOSISE", sndB, idx, US_PASS, 'I');
		delete [] sndB;
	}
}

void CMainFrame::ParseSym(CString dat, CMapStringToString& ary)
{
	CString sym, value;
	int	pos{};
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
	CString s;
	s.Format("ACTIONCAPTION KEY : %d	ACTION : %d\n",key,action);
	OutputDebugString(s);
	if (key == -1)	// for main caption
	{
		switch (action)
		{
		case MIDX_HOME:
			GoHomepage();
			break;
		case MIDX_FULL:		fullScreenMode(!m_fullmode);	break;
		case MIDX_DUAL:		fitDual();	break;
		case MIDX_VS1:case MIDX_VS2:case MIDX_VS3:
		case MIDX_VS4:case MIDX_VS5:case MIDX_VS6:
			change_VirtualScreen(action - MIDX_VS1);
			break;
		case MIDX_MIN:		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);	break;
		case MIDX_RESTORE:	PostMessage(WM_SYSCOMMAND, SC_RESTORE);		break;
		case MIDX_MAX:
			{

				PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);	
				/*
				CRect	mRc;
				m_pMain->GetWindowRect(mRc);
				mRc.bottom -= 100;
				
				m_pMain->SetWindowPos(&wndTop, mRc.left, mRc.top, mRc.Width(), mRc.Height(), SWP_SHOWWINDOW);
				*/
				break;
			}
		case MIDX_CLOSE:
		case MIDX_ICON:		PostMessage(WM_SYSCOMMAND, SC_CLOSE);		break;
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

	int		value{};
	long		rc = 0;
	
	CPoint		sp(-1, -1);
	CString		mapname;
	WINDOWPLACEMENT	pl;

	GetWindowPlacement(&pl);
	switch (action)
	{
	case IDX_SINGLE:	
		convertSDI(key, m_vsN);		
		break;
#ifdef DF_MK_CAPTION
	case IDX_LOCK:
	{
		child->m_xcaption.ChangeLock();
	}
	break;
	case IDX_MARKET:
	{
		if (child->m_xcaption.m_marketN == MK_NON)
			return;

		if (child->m_xcaption.m_MkLock)
		{
			MessageBox("거래소 잠금 해제 후 선택 가능합니다", "IBK투자증권", MB_OK);
			return;
		}

		value = child->m_xcaption.ClickMK();

		if (value < 1)
			return;

		m_slog.Format("[AXIS][NXT][%s]<%d> Market =[%d]", __FUNCTION__, __LINE__, value);
		child->m_xcaption.SetMarket(value);
		child->m_xcaption.Invalidate();

		CString tmp;
		tmp.Format("edMarketTrigger\t%s", value == 1?"KRX": value == 2?"NXT":"통합");
		OutputDebugString(tmp);
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY,
			(void*)NULL, (BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, child->m_key), (LPARAM)(const char*)tmp);

	}
	break;
#endif
	case IDX_GROUP:
		value = child->m_xcaption.ChangeGroup();
		if (value >= 0)	set_Trigger(key, value);
		break;
	case IDX_COPY:
		child->GetWindowPlacement(&pl);
		value = GetSystemMetrics(SM_CYCAPTION);
		value += GetSystemMetrics(SM_CXFRAME);
		value += GetSystemMetrics(SM_CXBORDER);
		sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
		sp.Offset(value, value);

		value = child->m_xcaption.GetGroup();
		switch (value)
		{
		case 0:/*none group*/
		case 99:/*all group*/	break;
		case 1:case 2: case 3:case 4: case 5: case 6: case 7: case 8: case 9: case 10:
			value++;
			if (value > 10)	value = 1;
			break;
		default:	
			value = 1;	break;
		}		
		m_mapHelper->CopyScreen(child->m_mapN, value, child->m_xcaption.GetFontSize(), sp);
		break;
	case IDX_HELP:	HelpLink(key);	break;
	case IDX_FONTX: //기본크기로
		refleshMap(child->m_key);
		child->m_xcaption.SetFontSize(m_fontSize);
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, m_fontSize));	// cej
		child->m_xcaption.SetMax(false);
		break;
	case IDX_FONT:
		
		m_preFontSize = child->m_xcaption.GetFontSize();
		value = child->m_xcaption.ChangeFont();	
		
		if (!CheckClientWidth(child, child->m_xcaption.GetFontSize()))
		{
			child->m_xcaption.SetFontSize(m_preFontSize);
			return;
		}
		
		
		
		if (value > 0)
		{
			if (value == INITMAPSIZE)
			{
				//if (m_preFontSize == m_fontSize) return;
				refleshMap(child->m_key);
				//child->m_xcaption.SetFontSize(m_fontSize);
				
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, m_fontSize));	// cej
			}
			else
			{
				refleshMap(child->m_key);
				
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, value));
				
			}
			child->m_xcaption.SetMax(false);
		}
		break;
	case IDX_MIN:
		sp = getMinimizePos(child->m_key);
		child->actionCaption(action, sp);
		MDINext();
		break;
	case IDX_RESTORE:
	case IDX_MAX:		child->actionCaption(action, sp);	break;
	case IDX_CLOSE:
	case IDX_ICON:		
		{
			if(child->m_mapN == "IB0000X8")
			{
				child->ShowWindow(SW_HIDE);
			}
			else
			{
				m_mapHelper->closeChild(key);			
			}
		}
		break;
	default:
		break;
	}
}

void CMainFrame::actionSCaption(int key, int action)
{
	CSChild*	schild = NULL;
	if (!m_arSDI[m_vsN].Lookup(key, schild))
		return;

	int		value{};
	CRect		wRc;
	long		rc = 0;
	CPoint		sp(-1, -1);
	WINDOWPLACEMENT	pl;
	
	GetWindowPlacement(&pl);
	switch (action)
	{
	case IDX_PIN:
		if (schild->m_xcaption.changePin())
			schild->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		else	schild->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		break;
	case IDX_SINGLE:	convertMDI(key, m_vsN);		break;
#ifdef DF_MK_CAPTION
	case IDX_LOCK:
	{
		schild->m_xcaption.ChangeLock();
	}
	break;
	case IDX_MARKET:
	{
		if (schild->m_xcaption.m_marketN == MK_NON)
			return;

		if (schild->m_xcaption.m_MkLock)
			return;

		value = schild->m_xcaption.ClickMK();
		m_slog.Format("[AXIS][NXT][%s]<%d> Market =[%d]", __FUNCTION__, __LINE__, value);
		schild->m_xcaption.SetMarket(value);
		schild->m_xcaption.Invalidate();

		CString tmp;
		tmp.Format("edMarketTrigger\t%s", value == 1 ? "KRX" : value == 2 ? "NXT" : "통합");
		OutputDebugString(tmp);
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY,
			(void*)NULL, (BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, schild->m_key), (LPARAM)(const char*)tmp);
		/*
			   tmp.Format("edMarketTrigger\t%d", value);
			   OutputDebugString(tmp);
			   m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (
				   void*)NULL, (BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)tmp);*/
	}
	break;
#endif
	case IDX_GROUP:
		value = schild->m_xcaption.ChangeGroup();
		if (value >= 0)	set_STrigger(key, value, m_vsN);
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
		m_mapHelper->CopySScreen(schild->m_mapN, value, schild->m_xcaption.GetFontSize(), sp);
		break;
	case IDX_HELP:	HelpLink(key);	break;
	case IDX_FONT:
		value = schild->m_xcaption.ChangeFont();
		if (value > 0)
		{
			if (value == INITMAPSIZE)
			{
				refleshMap(schild->m_key);
				schild->m_xcaption.SetFontSize(m_fontSize);
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, m_fontSize));	// cej
			}
			else
			{
				refleshMap(schild->m_key);
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, value));
			}
			schild->m_xcaption.SetMax(false);
		}
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
		schild->SetMax();
		break;
	case IDX_CLOSE:
	case IDX_ICON:	schild->PostMessage(WM_CLOSE);	break;
	default:
		break;
	}
}

void CMainFrame::GetDispN(char* mapN)
{
	if (!m_tMenu)	return;
	CString dispN = m_tMenu->GetDispN(mapN);
	if (dispN.GetLength() >= 16)
		dispN = dispN.Left(15);	// Last byte null terminate
	sprintf(mapN, dispN);
}


int CMainFrame::InputScreenNo(CString dispN)
{
	if (dispN == "8788")
	{
		RunPhonePad();
		return 0;
	}

	if (m_tMenu)
	{
		CString mapN = m_tMenu->GetMap(dispN);
		if (ScreenCheck(mapN) == DF_NUSE)
			return 0;

		if (!ExistMenu(mapN))	
			return 0;
// 		if (IsForeignMap(mapN))
// 			RunForeignMap();
		else if (IsNewRealTick(mapN))
			RunNewRealTick();
		else if (IsPhonePad(mapN))
			RunPhonePad();
		else
			return m_mapHelper->ChangeChild(mapN);
	}
	return 0;
}

void CMainFrame::ConfigFrame()
{
	OutputDebugString("--------------------------------1--------------------\n");
	SetUserInfo();	
	CString	file, cpfile, port;

	file.Format("%s\\%s\\%s", Axis::home, USRDIR, Axis::user);
	::CreateDirectory(file, NULL);

	file.Format("%s\\%s\\%s", Axis::home, GEXDIR, Axis::user);
	::CreateDirectory(file, NULL);

	file.Format("%s\\%s\\%s\\axisensetup.ini", Axis::home, USRDIR, Axis::user);
	cpfile.Format("%s\\%s\\axisensetup.ini", Axis::home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\usertool.ini", Axis::home, USRDIR, Axis::user);
	cpfile.Format("%s\\%s\\usertool.ini", Axis::home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\axisticker.ini", Axis::home, USRDIR, Axis::user);
	cpfile.Format("%s\\%s\\axisticker.ini", Axis::home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\hkey.ini", Axis::home, USRDIR, Axis::user);
	cpfile.Format("%s\\%s\\hkey.ini", Axis::home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	file.Format("%s\\%s\\%s\\ordcfg.ini", Axis::home, USRDIR, Axis::user);
	cpfile.Format("%s\\%s\\ordcfg.ini", Axis::home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);
	OutputDebugString("--------------------------------2--------------------\n");
	CheckSoundConfig();
	CheckNewsSetting();
	OutputDebugString("--------------------------------3--------------------\n");

#ifdef DF_USE_CPLUS17
	m_conclusion = std::make_unique<CConclusion>(this);
	if (!m_conclusion->Create(IDD_CONCLUSIONLIST))
		m_conclusion = nullptr;
	else	m_conclusion->Init();

	m_pServerOrd = std::make_unique<CDlgServerOrder>(this);   //modi 서버주문 리스트
	if (!m_pServerOrd->Create(IDD_DLG_SERVERORDER))
		m_pServerOrd = nullptr;
	else	m_pServerOrd->Init();

	OutputDebugString("--------------------------------4--------------------\n");
	m_kobanotify = std::make_unique<CKobaElwNotify>(this);   
	if (!m_kobanotify->Create(IDD_KOBA_NOTIFY))
		m_kobanotify = nullptr;
	else
		m_kobanotify->Init();
	OutputDebugString("--------------------------------5--------------------\n");
	m_Econclusion = std::make_unique<CEConclusion>(this);
	if (!m_Econclusion->Create(IDD_CONCLUSION))
		m_Econclusion = nullptr;
	else	m_Econclusion->Init();
	OutputDebugString("--------------------------------6--------------------\n");

	m_mngInfo = std::make_unique<CManageInfo>(this);
	if (!m_mngInfo->Create(IDD_MNGINFO))
		m_mngInfo = nullptr;

	//modi createslidewnd
#ifdef DF_SLIDEWND
	CreateSlideWnd();
#endif

	OutputDebugString("--------------------------------7--------------------\n");
	m_top10 = std::make_unique<CTOP10Dialog>(this);
	if (!m_top10->Create(IDD_TOP10))
		m_top10 = nullptr;

	OutputDebugString("--------------------------------8--------------------\n");

	m_Nclock = std::make_unique<CNClock>();
	LPCTSTR className = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, ::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH), m_resourceHelper->GetIcon());
	if (!m_Nclock->CreateEx(WS_EX_TOOLWINDOW, className, _T(""), WS_POPUP,//|WS_BORDER, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0))
		m_Nclock = nullptr;
	else
		m_Nclock->Init();
#else
	m_conclusion = new CConclusion(this);
	if (!m_conclusion->Create(IDD_CONCLUSIONLIST))
	{
		delete m_conclusion;
		m_conclusion = NULL;
	}
	else	m_conclusion->Init();

	m_kobanotify = new CKobaElwNotify(this);
	if (!m_kobanotify->Create(IDD_KOBA_NOTIFY))
	{
		delete m_kobanotify;
		m_kobanotify = NULL;
	}
	else
	{
		m_kobanotify->Init();
		//m_kobanotify->ShowWindow(SW_SHOW);
	}

	m_Econclusion = new CEConclusion(this);
	if (!m_Econclusion->Create(IDD_CONCLUSION))
	{
		delete m_Econclusion;
		m_Econclusion = NULL;
	}
	else	m_Econclusion->Init();
	OutputDebugString("--------------------------------6--------------------\n");

	m_mngInfo = new CManageInfo(this);
	if (!m_mngInfo->Create(IDD_MNGINFO))
	{
		delete m_mngInfo;
		m_mngInfo = NULL;
	}
	OutputDebugString("--------------------------------7--------------------\n");
	m_top10 = new CTOP10Dialog(this);
	if (!m_top10->Create(IDD_TOP10))
	{
		delete m_top10;
		m_top10 = NULL;
	}
	OutputDebugString("--------------------------------8--------------------\n");

	m_Nclock = new CNClock();
	LPCTSTR className = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, ::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH), m_resourceHelper->GetIcon());
	if (!m_Nclock->CreateEx(WS_EX_TOOLWINDOW, className, _T(""), WS_POPUP,//|WS_BORDER, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0))
	{
		delete m_Nclock;
		m_Nclock = NULL;
	}
	else
		m_Nclock->Init();
#endif
	


	change_BackGround();
	CreateTB();
}

void CMainFrame::ProcessInitMap()
{
	CString initMap(m_axis->GetProfileString(WORKSTATION, INITMAP, "00000000"));
	
	if (initMap.GetLength() >= L_MAPN && initMap.Left(2) == "IB")
	{
		m_mapHelper->ChangeChild(initMap, 0, 0, CenterPOS);
	}
}

void CMainFrame::EditTool()
{
	TotalSetup(IDD_TSETUP); 
}

void CMainFrame::SetTicker()
{
	TotalSetup(IDD_SETTICKINFO);
}

BOOL CMainFrame::Change_VsKey(int index)
{
	if (0 > index || index >= 6)
		return FALSE;
	change_VirtualScreen(index);
	return TRUE;
}

void CMainFrame::ConfigTicker(bool reconfig/*=false*/)
{
	CProfile profile(pkAxisTicker);

	if (reconfig)
	{
		m_tInfo1->ReConfig(profile.GetInt(szGeneral, "Bar00", SPLIT_CNT));
		m_tInfo2->ReConfig(profile.GetInt(szGeneral, "Bar01", SPLIT_CNT));
	}
	else
	{
		m_tInfo1->Config(profile.GetInt(szGeneral, "Bar00", SPLIT_CNT));
		m_tInfo2->Config(profile.GetInt(szGeneral, "Bar01", SPLIT_CNT));
	}
}

void CMainFrame::CreateChaser()
{
	long	rc = 0;
	if (m_chaser)
	{
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setTRACE, WM_USER), m_chaser->GetSafeHwnd());
		return;
	}

#ifdef DF_USE_CPLUS17
	m_chaser = std::make_unique<CChaser>(this); 
	if (!m_chaser->Create(NULL, "axis chaser", WS_CHILD, CRect(0, 0, 0, 0), this, (int)m_chaser.get()))
	{
		m_chaser = NULL;
		return;
	}
#else
	m_chaser = new CChaser(this);
	if (!m_chaser->Create(NULL, "axis chaser", WS_CHILD, CRect(0, 0, 0, 0), this, (int)m_chaser))
	{
		delete m_chaser;
		m_chaser = NULL;
		return;
	}
#endif

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
	(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setTRACE, WM_USER), m_chaser->GetSafeHwnd());
}

void CMainFrame::DeleteChaser()
{
#ifndef _CHASERDEBUG
	long	rv = 0;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rv,
			(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setTRACE, WM_USER), NULL);
	CloseChaserAPP();
#endif
}

void CMainFrame::CloseChaserAPP()
{
	CString	winCaption;
	winCaption.Format("AxisChaser for %s", m_regkey);
	CWnd* wnd = CWnd::FindWindow(NULL, winCaption);
	 if (wnd && wnd->GetSafeHwnd())
		::PostMessage(wnd->m_hWnd, WM_CLOSE, 0, 0);
}

void CMainFrame::showChaser()
{
	if (!m_runAxis)	return;
	//	create tracer process
	CString	aps, cmds, exes;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	char	buffer[1024];
	GetClassName(m_hWnd, buffer, sizeof(buffer));
	cmds.Format(" /c %s /r \"%s\"", buffer, m_regkey);
	aps.Format("%s\\%s\\AxisChaser.exe", Axis::home, RUNDIR);

	const BOOL bRc = CreateProcess(
			aps,				// application name
			(char *)(const char*)cmds,// command line
			NULL,				// process attribute
			NULL,				// thread attribute
			FALSE,				// is inherit handle
			0,					// creation flags
			NULL,				// environment
			NULL,				// current directory
			&si,				// STARTUPINFO
			&pi);				// PROCESS_INFORMATION
}

LONG CMainFrame::OnCHASER(WPARAM wParam, LPARAM lParam)
{
// 	CString	winCaption;
// 	winCaption.Format("AxisChaser for %s", m_regkey);
// 	CWnd* wnd = CWnd::FindWindow(NULL, winCaption);
// 
// 	if (!wnd->GetSafeHwnd())
// 		return 0;

	m_sync.Lock();
	const int	len = L_cds + LOWORD(wParam);
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

	CString	winCaption, slog;
	slog.Format("%s", bufx + L_cds);
	OutputDebugString(slog);

	CString str;
	winCaption.Format("AxisChaser for %s", m_regkey);
	CWnd* wnd = CWnd::FindWindow(NULL, winCaption);
	if (wnd->GetSafeHwnd())
		::SendMessage(wnd->m_hWnd, WM_COPYDATA, 0, (LPARAM)&cs);

	m_sync.Unlock();
	return 0;
}

LONG CMainFrame::OnELOG(WPARAM wParam, LPARAM lParam)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	CFile		cfile;
	CString		allfile, filename;

	allfile.Format("%s\\%s\\%s\\crash\\elog_*", Axis::home, USRDIR, Axis::user);
	hFind = FindFirstFile(allfile, &FindFileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			filename.Format("%s\\%s\\%s\\crash\\%s", Axis::home, USRDIR, Axis::user, FindFileData.cFileName);
			if (cfile.Open(filename, CFile::modeRead|CFile::typeBinary))
			{
				const int	fileL = (int)cfile.GetLength();
				//char*	sndB = new char [fileL + 1];
				std::unique_ptr<char[]> sndB = std::make_unique<char[]>(fileL + 1);

				ZeroMemory(sndB.get(), fileL + 1);
				cfile.Read(sndB.get(), fileL);
				cfile.Close();

				m_elogfileName = filename;
				sendTR("pihoerpt", sndB.get(), fileL, US_PASS, 'r');
			//	delete [] sndB;
				return 0;
			}
		}
		if (!FindNextFile(hFind, &FindFileData))	break;
	}

	return 0;
}

LONG CMainFrame::OnApplyACC(WPARAM wp, LPARAM lp)
{
	CString	sym, value, data = (char *) lp;

	long	rc = 0;
	int	sndL{};
	const int command = LOWORD(wp);
	switch (command)
	{
	case 0:		// apply acccout and password for trigger
		{
			const int	key = m_activeKey;
			while (!data.IsEmpty())
			{
				int pos = data.Find("\n");
				if (pos == -1)	break;
				value = data.Left(pos++);
				data = data.Mid(pos);

				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)value);
			}
		}
		break;
	case 1:	// save account password for staff
		while (!data.IsEmpty())
		{
			int pos = data.Find('\n');
			if (pos == -1)	break;
			value = data.Left(pos++);
			data  = data.Mid(pos);

			rc = 0;
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(saveACC, 0), (long)(const char*)value);
		}
		break;
	case 100:
		sndL = HIWORD(wp);
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setACCB, HIWORD(wp)), lp);
		break;
	case 101:	// 주식
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setACCG, 0), (long)(const char*)data);
		break;
	case 102:	// 선물
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setACCG, 1), (long)(const char*)data);
		break;
	}

	return 0;
}

LONG CMainFrame::OnSaveACC(WPARAM wp, LPARAM lp)
{
	const int	command = (int) wp;
	long	rc = 0;
	CString	sym, value, data;

	switch (command)
	{
	case 0:	// save acc pass
		data = (char *) lp;
		//**TRACE("SAVE pass [%s]\n", data);

		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(saveACC, 0), (long)(const char*)data);
		break;
	case 1:	// attach map
		{
		int	size{}, vtype{}, key = KEY_ACCTOOL;
			CString	mapname = _T(MAPN_ACCTCONFIG);
			
			m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
						(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);
			m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
					(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)m_accSave, vtype, key);
			m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false);

			CSize	frameGap;
			frameGap.cx  = GetSystemMetrics(SM_CXFRAME);
			frameGap.cx += GetSystemMetrics(SM_CXBORDER) * 2;

			frameGap.cy  = GetSystemMetrics(SM_CYFRAME);
			frameGap.cy += GetSystemMetrics(SM_CYBORDER) * 2;
			frameGap.cy += GetSystemMetrics(SM_CYCAPTION);

			const int nCx = frameGap.cx + LOWORD(size);
			const int nCy = frameGap.cy + HIWORD(size);
			m_accSave->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_NOZORDER);
		}
		break;
	case 2:	// detach map
		break;
	case 100:	// save password for staff.
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnAxisClose(WPARAM wParam, LPARAM lParam)
{
	CString	str, msg, title, logs;
	
	switch (m_step)
	{
	case axOPENRSM:
	{
		if (m_bUseNewLogin)
			m_axConnect->SetProgress(false);
		else
			m_axConnectOld->SetProgress(false);
	}
	case axOPENWSH:
	case axOPENSIGN:
		m_step = axNONE;
		if (m_bUseNewLogin)
		{
			m_axConnect->SetStatus(SM_EXIT);
			if (m_axConnect->GetStyle() & WS_VISIBLE)
			{
				m_axConnect->EndDialog(IDCANCEL);
				m_axConnect = NULL;
			}
		}
		else
		{
			m_axConnectOld->SetStatus(SM_EXIT);
			if (m_axConnectOld->GetStyle() & WS_VISIBLE)
			{
				m_axConnectOld->EndDialog(IDCANCEL);
				m_axConnectOld = NULL;
			}
		}

		//20170210 dkkim
		//종료전 마지막화면 저장
		saveExitMap();

		msg.LoadString(ST_MSG_DISCONNECT_EXIT);
		title.LoadString(ST_TEXT_PROGRAMEXIT);
		//**::MessageBox(m_hWnd, msg, title, MB_ICONINFORMATION);
		Axis::MessageBox(this, msg, MB_ICONINFORMATION);
		break;
	case axNONE:
		title.LoadString(ST_TEXT_PROGRAMEXIT);
		m_axMisc->MsgBox(str, title);

		//20170210 dkkim
		//종료전 마지막화면 저장
		saveExitMap();

		if (m_bUseNewLogin)
		{
			if (m_axConnect && IsWindow(m_axConnect->GetSafeHwnd()) && m_axConnect->ContinueModal())
				m_axConnect->EndDialog(IDCANCEL);
		}
		else
		{
			if (m_axConnectOld && IsWindow(m_axConnectOld->GetSafeHwnd()) && m_axConnectOld->ContinueModal())
				m_axConnectOld->EndDialog(IDCANCEL);
		}
		break;
	case axDONE:
		//ref 보안프로그램(ASTx) 미가동으로 인한 프로세스 종료
		if (wParam == 99)
		{
			char	buf[512];
			CString	file, stmp, smap;
			file.Format("%s\\tab\\axis.ini", Axis::home);
			if (m_sIntegrityMSG.IsEmpty())
			{
				DWORD dw = GetPrivateProfileString("ASTx", "msg", "보안프로그램(ASTx) 미가동!!", buf, sizeof(buf), file);
				str.Format("%s", buf); stmp.TrimRight();

				m_axMisc->MsgBox(str, title);
			}
			else
				m_axMisc->MsgBox(m_sIntegrityMSG, title);

			PostMessage(WM_CLOSE);
			return 0;
		}
		//ref

		msg.LoadString(ST_MSG_DISCONNECT_RETRY);
		title.LoadString(ST_TEXT_OK);
		//2014.02.04 dkkim
		//재접속시 방화벽 해제
		FreeFirewall();

		//20170210 dkkim
		//종료전 마지막화면 저장
		saveExitMap();

		if (Axis::MessageBox(msg, MB_OKCANCEL) == IDOK)
		{
			if (!m_axMisc->RunVers(verRETRY, Axis::userID, m_pass, m_cpass))
				return 0;
		}
		else
		{
			if (Axis::MessageBox(this, "프로그램을 종료하시려면 [확인] 버튼을 눌러 주세요", MB_OKCANCEL) == IDCANCEL)
				return 0;
		}
		break;
	}

	m_step = axCLOSE;

	if (GetSafeHwnd())
	{
		PostMessage(WM_CLOSE);
	}
	return 1;
}

LRESULT CMainFrame::OnTrayEvent(WPARAM wParam, LPARAM lParam)
{
	const UINT	msg = LOWORD(lParam);
	switch (msg)
	{
	case WM_RBUTTONDOWN:	// display menu
		{
			CPoint pt;
			GetCursorPos(&pt);
			m_tMenu->ShowFullMenu(pt);
		}
		break;
	case WM_LBUTTONDBLCLK:	// restore SDI 
		//BackFromTray();
		
		m_appMode = MODE_SDI;
		TrayCmd(TRAY_HIDEICON);
		ShowWindow(SW_SHOW);
		
		break;
	}
		
	return 1;
}

void CMainFrame::ProcessInfofile(bool upload)
{
#ifdef DF_USE_CPLUS17
	m_infofile = std::make_unique<CInfofile>(m_wizard.get(), upload);
#else
	m_infofile = new CInfofile(m_wizard, upload);
#endif
	m_infofile->DoModal();

	if (!upload)
	{
		const DWORD bits = m_infofile->GetApplyBits();

		if (bits & APPLY_ENSETUP)	
		{
			load_eninfomation(false);
			load_mngSetup();
			load_hkey();
		}

		if (bits & APPLY_TOOLBAR)	
			m_bar1->Refresh();

		if (bits & APPLY_TICKER)
		{
			ConfigTicker(true);
			sendTicInfo();

			CProfile profile(pkAxisTicker);
#ifdef DF_USE_CPLUS17
			ShowControlBar(m_tInfo1.get(), profile.GetInt(szGeneral, "view1", 1), FALSE);
			ShowControlBar(m_tInfo2.get(), profile.GetInt(szGeneral, "view2", 1), FALSE);
			if (m_tInfo3)
				ShowControlBar(m_tInfo3.get(), profile.GetInt(szGeneral, "view3", 1), FALSE);
#else
			ShowControlBar(m_tInfo1, profile.GetInt(szGeneral, "view1", 1), FALSE);
			ShowControlBar(m_tInfo2, profile.GetInt(szGeneral, "view2", 1), FALSE);
			if (m_tInfo3)
				ShowControlBar(m_tInfo3, profile.GetInt(szGeneral, "view3", 1), FALSE);
#endif
		

			if (m_bSDI)
			{
				saveToolStatus();
				SetSDIChangeHeight();
			}
		}

		if (bits & APPLY_WORKSPACE)	
		{
			makeUserScreenMenu();
		}

		load_dbar2();
	}
	else
	{
		if (m_infofile->GetUploadMemoYN())
			MemoUpload();
	}

#ifdef DF_USE_CPLUS17
	if (m_infofile && m_infofile->GetSafeHwnd())
		m_infofile = nullptr;
#else
	if (m_infofile && m_infofile->GetSafeHwnd())
	{
		delete m_infofile;
		m_infofile = NULL;
	}

	if (m_infofile)
		delete m_infofile;
#endif
}

void CMainFrame::Action(WPARAM wParam, LPARAM lParam)
{
	const int	key = LOWORD(wParam);
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
	case 6:	m_mapHelper->closeChild(child->m_key);		break;
	default:
		if (actK >= GROUPBASE && actK < FONTBASE)
		{
			actK -= GROUPBASE;
			switch (actK)
			{
			case 0:case 1:case 2:case 3: case 4:case 5:case 99:
				set_Trigger(key, actK);	break;
			default:	break;
			}
		}
		else if (actK >= FONTBASE)
		{
			actK -= FONTBASE;
			long	rc = 0;
			switch (actK)
			{
			case 8:case 9:case 10:case 11:case 12:
				child->m_xcaption.SetFontSize(actK);
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, actK));
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
	const int	infoK =  LOWORD(wParam);
	const int	key = HIWORD(wParam);
	char*	buf = (char *) lParam;
	
	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		sprintf((char *) lParam, "-1");
		return;
	}

	switch(infoK)
	{
	case 0:	//	group
		sprintf(buf, "%d", child->m_xcaption.GetGroup());
		break;
	case 1:
		sprintf(buf, "%d", child->m_xcaption.GetFontSize());
		break;
	default:
		break;
	}
}

// must to call after call SetMisc (..)
void CMainFrame::HideChildButton(int key)
{
	CString		file;

	CChildFrame* child{};
	CSChild* schild{};
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		if (!m_arMDI[vsN].Lookup(key, child))
		{
			if (!m_arSDI[vsN].Lookup(key, schild))
				continue;
			
			/*
			if (m_mapHelper->IsDLL(schild->m_mapN))
				schild->m_xcaption.HideButton(HIDE_FONT);
			else */
			if (IsWebMenu(schild->m_mapN))
				schild->m_xcaption.HideButton(HIDE_FONT);

			//if (schild->m_mapN.Mid(0,4) == "IB76" || schild->m_mapN.Mid(0,8) == "IB202200")  //종목검색 쪽은 폰트 뺌
			if (IsNoFontMenu(schild->m_mapN))
			{
				schild->m_xcaption.HideButton(HIDE_FONT);
			}

			if (IsHelpMenu(schild->m_mapN))
				schild->m_xcaption.ShowButton(HIDE_HELP);
			else
				schild->m_xcaption.HideButton(HIDE_HELP);
			break;
		}
		//TRACE("["+child->m_mapN+"]\n");

		/*if (m_mapHelper->IsDLL(child->m_mapN))
			child->m_xcaption.HideButton(HIDE_FONT);
		else */
		if (IsWebMenu(child->m_mapN))
			child->m_xcaption.HideButton(HIDE_FONT);
		else
			child->m_xcaption.ShowButton(HIDE_FONT);

		if (IsHelpMenu(child->m_mapN))
			child->m_xcaption.ShowButton(HIDE_HELP);
		else
			child->m_xcaption.HideButton(HIDE_HELP);
		//if (child->m_mapN.Mid(0,4) == "IB76" || child->m_mapN.Mid(0,8) == "IB202200")  //종목검색 쪽은 폰트 뺌
		if (IsNoFontMenu(child->m_mapN))
		{
			child->m_xcaption.HideButton(HIDE_FONT);
		}
		else
			child->m_xcaption.ShowButton(HIDE_FONT);


		
		
	}
//	file.Format("%s\\%s\\caption.ini", Axis::home, TABDIR);
//	UINT value = GetPrivateProfileInt("BUTTONHIDE", child->m_mapN, 0, file);
//	if (value == SHOW_ALL)	return;
//	child->m_xcaption.HideButton(value);
}

void CMainFrame::SettingEticker(int paneID)
{
	if (m_tInfo1)	m_tInfo1->SettingEticker(paneID);
	if (m_tInfo2)	m_tInfo2->SettingEticker(paneID);

	CProfile profile(pkAxisTicker);

	CString	section, keys;
	char	buf[1024 * 2]{}, ticInfo[128]{};
	int	len{}, idx = 0, kk{};
	CMapStringToString	interAry;

	ZeroMemory(ticInfo, sizeof(ticInfo));
	
	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		keys.Format("BAR%02d", ii);
		const int splitN = profile.GetInt(szGeneral, keys, SPLIT_CNT);
		for (int jj = 0; jj < splitN; jj++)
		{
			section.Format("BAR_INFO_%02d", jj + ii*10);
			switch (profile.GetInt(section, "INDEXK", TKIND_INDEX))
			{
			case TKIND_INDEX:
				{
					CString	value;
					for (kk = 0; ; kk++)
					{
						value = profile.GetString(section, Format("%02d", kk));
						if (value.IsEmpty())
							break;
						len = atoi(value) + 1;
						if (len >= UPDN_KOSPI)	len += 2;
						if (len >= NEWS_BASE)	len += 1;
						ticInfo[len] = '1';
					}
				}
				break;
			case TKIND_UPDOWN:
				kk = profile.GetInt(section, "SELECT");
				switch (kk)
				{
				case 0:	case 1:
					ticInfo[UPDN_KOSPI+kk] = '1';	// 7 - base updown key
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
				break;
				{
					CString	data, value, codes;
					CProfile inter(Format("%s\\%s\\%s.ins", Axis::home, TABDIR, Axis::user));

					kk = profile.GetInt(section, "SELECT", -1);
					if (kk == -1)	continue;
					
					section.Format("%02d", kk);
					for (kk = 0; ; kk++)
					{
						keys.Format("%02d", kk);
						data = inter.GetString(section, keys);
						if (data.IsEmpty())
							break;
						const int	pos = data.Find(':');
						if (pos == -1)	continue;
						codes = data.Left(pos);
						if (interAry.Lookup(codes, value))
							continue;
						interAry.SetAt(codes, "reg");
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
	for (int ii = 1 ; ii < sizeof(ticInfo); ii++)
	{
		if (ticInfo[ii] == 0x00)
			continue;
		section.Format("%03d", ii);
		rtick[idx].acts[0] = '1';
		CopyMemory(rtick[idx++].code, section, sizeof(rtick->code));
	}
	
	len = idx*sizeof(struct _rtick);

	//2012.08.10 김덕기 - PIBOTICK -> PIBFTICK
	//sendTR("PIBOTICK", buf, len, US_PASS, 'e');
	sendTR("PIBOTICK", buf, len, US_PASS, 'e');

	if (interAry.GetCount() > 0)
	{
		const	int	L_code = 12;
		const	int	codeCnt = 5;/* 2 - key, 3 - code count */
		CString	codes;
		/*
		struct _iSise
		{
			char	keyf[2];	// ticker id
			char	nrec[3];	// count jcode
			char	code[L_code];	// jcode
		};
		*/
		ZeroMemory(buf, sizeof(buf));
		const int cnt = interAry.GetCount();
		len = idx = codeCnt;
		sprintf(buf, "%05d", cnt);
		for (POSITION pos = interAry.GetStartPosition(); pos; )
		{
			interAry.GetNextAssoc(pos, codes, keys);
			sprintf(&buf[idx], "%s", (LPSTR)(LPCTSTR)codes);
			idx += L_code;
		}
		sendTR("PIBOSISE", buf, idx, US_PASS, 'I');
	}
}

#pragma warning (disable : 26400)
#pragma warning (disable : 26409)

void CMainFrame::changePassword(CString newpassword)
{
	newpassword.TrimRight();
	m_pass = newpassword;

	long	rc = 0;
	CString	data = _T("");

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), loadACC, 0);
	data = (rc ? (char *)rc : _T(""));
	if (data.IsEmpty())	return;

	int		pos{};
	Caccount* acc{};
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
	
	for (int ii = 0; ii < acs.GetSize(); ii++)
	{
		rc = 0;
		acc = acs.GetAt(ii);
		if (acc->m_name.IsEmpty())
			continue;
		data.Format("%s\t%s\t%s", acc->m_account, m_pass, acc->m_name);
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(saveACC, 0), (long)(const char*)data);
	}

	for (int ii = 0; ii < acs.GetSize(); ii++)
		delete acs.GetAt(ii);
	acs.RemoveAll();
}

CPoint CMainFrame::getMinimizePos(int childKey)
{
	int		key{}, cnt{};
	CRect		iconicRc, cRc, mRc;
	CPoint		pt;
	POSITION	pos{};
	CChildFrame*	child = NULL;
	CArray < CChildFrame*, CChildFrame* > ary;

	m_arMDI[m_vsN].Lookup(childKey, child);
	if (child->m_iconpos.x != -1 || child->m_iconpos.y != -1)
		return child->m_iconpos;

	m_MClient->GetClientRect(cRc);
	const int cx = GetSystemMetrics(SM_CXMINIMIZED);
	const int cy = GetSystemMetrics(SM_CYMINIMIZED);

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

	cnt = ary.GetSize();
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
	CDocTemplate* pTemplate{};
	for (int ii = 0; ii < vwKIND; ii++)
		pTemplate = m_axis->GetNextDocTemplate(pos);

	ASSERT(pTemplate != NULL);
#pragma warning (disable : 6011)
	ASSERT_KINDOF(CDocTemplate, pTemplate);
#pragma warning (default1)
	CDocument* pDocument = pTemplate->OpenDocumentFile(NULL);
	ASSERT_VALID(pDocument);

	pos = pDocument->GetFirstViewPosition();
	return pDocument->GetNextView(pos);	
}

CView* CMainFrame::GetSDIView(int vwKIND)
{
	CString strWndClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, ::LoadCursor(NULL, IDC_ARROW),
			      (HBRUSH) ::GetStockObject(WHITE_BRUSH), m_resourceHelper->GetIcon());

	int col = m_arSDI[m_vsN].GetCount()/10;
	int row = m_arSDI[m_vsN].GetCount()%10;
	col *= XCAP_HEIGHT;	row *=	XCAP_HEIGHT;
	CSChild*	schild = new CSChild(this);
	if(!schild->Create(strWndClass, AXISKEY, WS_OVERLAPPEDWINDOW, CRect(col+row, row, col+row, row), NULL,NULL,0,NULL))
	{
		delete schild;
		return NULL;
	}

	CCreateContext	context;
	CRuntimeClass* pNewViewClass{};
	switch (vwKIND)
	{
	default:
	case NORMALVIEW:	
		//TRACE("NormalView\n");
		pNewViewClass = RUNTIME_CLASS(CAxisView);	
		break;
	case SCROLLVIEW:	
		//TRACE("ScrollView\n");
		pNewViewClass = RUNTIME_CLASS(CAxScrollView);	
		break;
	case TABLISTVIEW:	
		//TRACE("TabView\n");
		pNewViewClass = RUNTIME_CLASS(CAxTabView);	
		break;
	}

	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = GetActiveDocument();

	return schild->AttachView(context);
}
#pragma warning (default : 26400)
#pragma warning (default : 26409)

CSize CMainFrame::GetFrameGap(bool mdichild/*= true*/)
{
	const  int xx = (GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXSIZEFRAME)) * 2;  
	const int yy = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYSIZEFRAME) * 2 + 5;

	return CSize(xx, yy);
}

void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (m_tMenu)
		m_tMenu->MeasureMenuItem(nIDCtl,lpMeasureItemStruct); 
}

void CMainFrame::trouble_shooting( CString strErrMsg ,CString sKey, CString sSection)
{	
	CString sdat,stime,strLog;

	struct	_pidouini_mid mid {};

	CTime	time;
	time = time.GetCurrentTime();

	stime.Format("%04d%02d%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

	if(strErrMsg.GetLength() > 2000)
		strLog = strErrMsg.Mid(0,2000);
	else
		strLog = strErrMsg;

	memset(&mid, 0, sizeof(mid));
	mid.gubn[0] = 'I';
	memcpy(mid.item.usid, (LPCSTR)Axis::userID, Axis::userID.GetLength());
	memcpy(mid.item.innm, sKey, sKey.GetLength());
	memcpy(mid.item.senm, sSection, sSection.GetLength());
	memcpy(mid.item.skey, sKey, 8);
	sprintf(mid.item.valu, strLog);
	memcpy(mid.item.date, sdat, 8);
	
	sendTR("pidouini", (char*)&mid, sizeof(mid), 0, 'p');
}

void CMainFrame::SendInstallPath()
{	
	struct	_pidouini_mid	uini;
	
	FillMemory((char*) &uini, sizeof(_pidouini_mid), ' ');
	CString sval, sdat;
	
	CString s;
	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	CString fname = Axis::user + ".ini";
	CopyMemory(uini.gubn, "I", sizeof(uini.gubn));
	CopyMemory(uini.item.usid, Axis::userID, sizeof(uini.item.usid));
	CopyMemory(uini.item.innm, fname, sizeof(uini.item.innm));
#ifdef DF_USE_CPLUS17
	CopyMemory(uini.item.senm, "OS", strlen("OS"));
	CopyMemory(uini.item.skey, "INSTALL", strlen("INSTALL"));
#else
	CopyMemory(uini.item.senm, "OS", sizeof(uini.item.senm));
	CopyMemory(uini.item.skey, "INSTALL", sizeof(uini.item.skey));
#endif

	CopyMemory(uini.item.valu, Axis::home, sizeof(uini.item.valu));
	CopyMemory(uini.item.date, sdat, sizeof(uini.item.date));
	
	sendTR("pidouini", (char*)&uini, sz_pidouini, 0, 'N');
}

// 프로세스 리스트 정보업데이트
void CMainFrame::SendProcessList()
{		
	struct	_pidouini_mid	uini;
	
	FillMemory((char*) &uini, sizeof(_pidouini_mid), ' ');
	CString sval, sdat;
	
	//sval.Format("%s INSTALL PATH [%s]\n",str,Axis::home);
	//==================================================
	CString s;
	// 	s.Format("SEND CONFIG : [%s]\n",sval);
	// 	OutputDebugString(s);

	CString strProcess;
	strProcess = getProcessList();
	//AfxMessageBox(strProcess,MB_OK,-1);

	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	CString fname;

	int count = 0;

// 	s.Format("COUNT [%d]",strProcess.GetLength());
// 	AfxMessageBox(s,MB_OK,-1);
	if(strProcess.GetLength() > 500)
	{
		count = strProcess.GetLength() / 500;
		
		if(strProcess.GetLength() % 500 > 0)
			count++;

		for(int i=0; i< count;i++)
		{
			fname.Format("LIST%02d",i);

			CopyMemory(uini.gubn, "I", sizeof(uini.gubn));
			CopyMemory(uini.item.usid, Axis::userID, sizeof(uini.item.usid));
			CopyMemory(uini.item.innm, fname, sizeof(uini.item.innm));
#ifdef DF_USE_CPLUS17
			CopyMemory(uini.item.senm, "HTS", strlen("HTS"));
			CopyMemory(uini.item.skey, "PROCESS",strlen("HTS"));
#else
			CopyMemory(uini.item.senm, "HTS", sizeof(uini.item.senm));
			CopyMemory(uini.item.skey, "PROCESS", sizeof(uini.item.skey));
#endif

			CopyMemory(uini.item.valu, strProcess.Mid(i*500,500), sizeof(uini.item.valu));
			CopyMemory(uini.item.date, sdat, sizeof(uini.item.date));
			
			sendTR("pidouini", (char*)&uini, sz_pidouini, 0, 'N');
		}
	}
	else
	{
		fname = "LIST01";

		CopyMemory(uini.gubn, "I", sizeof(uini.gubn));
		CopyMemory(uini.item.usid, Axis::userID, sizeof(uini.item.usid));
		CopyMemory(uini.item.innm, fname, sizeof(uini.item.innm));

#ifdef DF_USE_CPLUS17
		CopyMemory(uini.item.senm, "HTS", strlen("HTS"));
		CopyMemory(uini.item.skey, "PROCESS", strlen("PROCESS"));
#else
		CopyMemory(uini.item.senm, "HTS", sizeof(uini.item.senm));
		CopyMemory(uini.item.skey, "PROCESS", sizeof(uini.item.skey));
#endif

		CopyMemory(uini.item.valu, strProcess, sizeof(uini.item.valu));
		CopyMemory(uini.item.date, sdat, sizeof(uini.item.date));
		
		sendTR("pidouini", (char*)&uini, sz_pidouini, 0, 'N');
	}
}

/*
CString strFile;	 
strFile.Format("%s\\tab\\TOP10.ini", Axis::home); 
if(m_top10->IsWindowVisible())	 
WritePrivateProfileString("TOP10","AUTOPOPUP","1",strFile);		
else
		WritePrivateProfileString("TOP10","AUTOPOPUP","0",strFile);	
*/
void writeError(CString strlog)
{
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("----------%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

	CString strDay;
	strDay.Format(_T("%d%d"), oTime.GetMonth(), oTime.GetDay());

	
	CString file;
	file.Format("%s\\%s\\%s\\Crashlog", Axis::home, USRDIR, Axis::user);
	
	CFileFind ff; 
	if(!ff.FindFile(file))
	{
		::CreateDirectory(file, NULL);
	}
	
	CString strFile;	 
	strFile.Format("%s\\user\\%s\\Crashlog\\%s%s", Axis::home, Axis::user, strDay,"log.ini"); 
	WritePrivateProfileString("log", strCurTime, strlog, strFile);
}

#pragma warning (disable : 26400)
#pragma warning (disable : 26409)
#pragma warning (disable : 26401)

void CMainFrame::ErrReport(CString eMsg)
{
	writeError(eMsg);
	if(!m_bClose)
		WriteErrFile((char *)(const char*)eMsg, eMsg.GetLength());

	//save_laststat();
	//MessageBox(eMsg, "오류감지");
	// 2010.07.21 화면캡쳐시 메뉴잔상이 남는 현상이 있어 Invalidate -> 메세지펌핑 후에 캡쳐한다. by LKM
	{
		MSG msg;
		Invalidate(TRUE);
		while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)==TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CWnd*	pChild = NULL;
	CString	mapN;

	pChild = this;

	if (!pChild) {
		Axis::MessageBox(this, "실행중인 화면이 없습니다.", MB_OK | MB_ICONSTOP);
		return;
	}

	for (int ii = 0; ii < 10; ii++)	
		Sleep(10);

	CString		file;
	CBitmap 	bitmap;
	CWindowDC	dc(pChild);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc); 
	pChild->GetWindowRect(&rect);

	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY); 

	// Create logical palette if device support a palette
	CPalette pal;
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		const UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = 
			GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);

		// Create the palette
		pal.CreatePalette(pLP);

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// Convert the bitmap to a DIB
	HANDLE hDIB = writeDDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)	
	{
		Axis::MessageBox("bitmap to DIB Failed.", MB_OK | MB_ICONSTOP);
		return;
	}


	file.Format("%s\\Err_Screen.bmp", Axis::home);
	//file.Replace("/", "");
	CFileDialog	dlg(FALSE, "bmp", file, OFN_OVERWRITEPROMPT, 
		"BMP Files(*.bmp)|*.bmp||", NULL);

	//szFile = (LPSTR)(LPCTSTR)path;
	// Write it to file
	writeDIB((LPSTR)(LPCTSTR)file, hDIB);

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree(hDIB);

	return;

// 	CWnd* wnd = CWnd::FindWindow(NULL, m_reportCaption);
// 	if (wnd->GetSafeHwnd())
// 	{
// 		int	len = L_cds + eMsg.GetLength();
// 		char	bufx[512];
// 
// 		ZeroMemory(bufx, sizeof(bufx));
// 		struct _exeCDSS* cds = (struct _exeCDSS *) bufx;
// 
// 		cds->flag = 9/*ELOG*/;
// 		cds->len  = eMsg.GetLength();
// 		CopyMemory(&bufx[L_cds], eMsg, len);
// 
// 		COPYDATASTRUCT cs;
// 		cs.cbData = len + L_cds;
// 		cs.dwData = 0;
// 		cs.lpData = bufx;
// 
// 		::SendMessage(wnd->m_hWnd, WM_COPYDATA, 0, (LPARAM) &cs);
// 	}
// 	else
// 	{
// 		CString			aps, cmds, exes;
// 		STARTUPINFO		si;
// 		PROCESS_INFORMATION	pi;
// 
// 
// 		ZeroMemory(&si, sizeof(STARTUPINFO));
// 		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
// 
// 		si.cb          = sizeof(STARTUPINFO);
// 		si.dwFlags     = STARTF_USESHOWWINDOW;
// 		si.wShowWindow = SW_SHOW;
// 
// 		cmds.Format(" /c %s /d %s /m %s /e \"%s\" /r \"%s\"", m_classN, Axis::devMode ? "s" : "x", m_activeMapN, eMsg, m_regkey);
// 		aps.Format("%s\\%s\\AXISReport.exe", Axis::home, RUNDIR);
// 		BOOL bRc = CreateProcess(
// 				aps,				// application name
// 				(char *)(const char*)cmds, // command line
// 				NULL,				// process attribute
// 				NULL,				// thread attribute
// 				FALSE,				// is inherit handle
// 				0,				// creation flags
// 				NULL,				// environment
// 				NULL,				// current directory
// 				&si,				// STARTUPINFO
// 				&pi);				// PROCESS_INFORMATION
// 		
// 		for (int ii = 0; ii < 100; ii++)
// 		{
// 			Sleep(100);
// 			wnd = CWnd::FindWindow(NULL, m_reportCaption);
// 			if (wnd->GetSafeHwnd())	break;
// 		}
// 	}
}

#pragma warning (default : 26400)
#pragma warning (default : 26409)
#pragma warning (default : 26401)

void CMainFrame::ShowUScreenMenu()
{
	if (!m_bar1)	return;

	CProfile profile(pkUserSetup);
	CMenuXP menu;

	menu.CreatePopupMenu();

	CMapStringToString ary;
	CString value, item;

	for (int i = 1; ; i++)
	{
		item = profile.GetString(szRoot, Format("%02d", i));
		if (item.IsEmpty())
			break;

		if (ary.Lookup(item, value))
			continue;

		ary.SetAt(item, "1");
		Trim(item);
		menu.AppendMenuX(MF_STRING | MF_ENABLED, ID_MENU_USER + i, item);
	}

	ary.RemoveAll();

	const CPoint	pt = m_bar1->GetPos(2);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
}

CString CMainFrame::GetSkinName()
{
	CProfile profile(pkPalette);
	CString value(profile.GetString(GENERALSN, "Palette"));

	if (!value.CompareNoCase("Green") || !value.CompareNoCase("Violet"))
	{
		value = "Blue";
	}

	Trim(value);
	return value;
}

void CMainFrame::AppSwitch()
{
	if(!m_bMiniMode)
	{
		if (m_bSDI)
		{
			m_bar3->SetPopupUse(TRUE);

			ChangeMDI();
		}
		else	
		{
			m_bar3->SetPopupUse(FALSE);

			if(m_miniWid == NULL || (m_miniWid->GetSafeHwnd() == NULL)) 
				return;

			const CWnd* base = m_miniWid->GetActiveView()->GetWindow(GW_CHILD);
			base->SendMessage(WD_HISTORYCLOSE,0,0);
			
// 			POINT pt;
// 
// 			GetCursorPos(&pt);
// 
// 			pt.y -= 100;
// 
// 			SetCursorPos(pt.x,pt.y);

			ChangeSDI();
		}
	}
}

void CMainFrame::convertSDI(int key, int vsN)
{
	m_mapHelper->TransformMDIChildToSingleWindow(key, vsN);
}

void CMainFrame::convertMDI(int key, int vsN)
{
	m_mapHelper->TransformSingleWindowToMDIChild(key, vsN);
}

void CMainFrame::addScreenList(CString mapN, int key, int vsN)
{
	add_history(mapN);
	addTool_history(mapN);
#ifdef DF_USE_CPLUS17
	m_bar2->add_Button(key, mapN, m_tMenu.get(), vsN);
#else
	m_bar2->add_Button(key, mapN, m_tMenu, vsN);
#endif
	m_activeMapN = mapN;
}

bool CMainFrame::FindSDIMap(CString mapName, int vsN)
{
	int		key{};
	POSITION	pos{};
	WINDOWPLACEMENT	pl{};
	CSChild* schild{};

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
			schild->SetWindowPlacement(&pl);
		}
		else
		{
			CString	data = mapName.Mid(L_MAPN);
			if (!data.IsEmpty())
			{
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

					m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)dat);
				}
			}
			schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		}
		return true;
	}
	return false;
}

void CMainFrame::loadToolStatus()
{
	CProfile profile(pkAxisTicker);
#ifdef DF_USE_CPLUS17
	ShowControlBar(m_tInfo1.get(), profile.GetInt(szGeneral, "view1", 1), FALSE);
	ShowControlBar(m_tInfo2.get(), profile.GetInt(szGeneral, "view2", 1), FALSE);
	if (m_tInfo3)
		ShowControlBar(m_tInfo3.get(), profile.GetInt(szGeneral, "view3", 0), FALSE);

	ShowControlBar(m_bar1.get(), profile.GetInt(szGeneral, "ttbar1", 1), FALSE);
	ShowControlBar(m_bar2.get(), profile.GetInt(szGeneral, "ttbar2", 1), FALSE);
	ShowControlBar(m_bar3.get(), profile.GetInt(szGeneral, "ttbar3", 1), FALSE);
#else
	ShowControlBar(m_tInfo1, profile.GetInt(szGeneral, "view1", 1), FALSE);
	ShowControlBar(m_tInfo2, profile.GetInt(szGeneral, "view2", 1), FALSE);
	if (m_tInfo3)
		ShowControlBar(m_tInfo3, profile.GetInt(szGeneral, "view3", 0), FALSE);

	ShowControlBar(m_bar1, profile.GetInt(szGeneral, "ttbar1", 1), FALSE);
	ShowControlBar(m_bar2, profile.GetInt(szGeneral, "ttbar2", 1), FALSE);
	ShowControlBar(m_bar3, profile.GetInt(szGeneral, "ttbar3", 1), FALSE);
#endif

	RelayoutTicker();
	ChangeLogo();
}

void CMainFrame::saveToolStatus()
{
	CProfile profile(pkAxisTicker);

	profile.Write(szGeneral, "view1", (int)(m_tInfo1->GetStyle() & WS_VISIBLE));
	profile.Write(szGeneral, "view2", (int)(m_tInfo2->GetStyle() & WS_VISIBLE));
	if (m_tInfo3)
		profile.Write(szGeneral, "view3", (int)(m_tInfo3->GetStyle() & WS_VISIBLE));
	profile.Write(szGeneral, "ttbar1", (int)(m_bar1->GetStyle() & WS_VISIBLE));
	profile.Write(szGeneral, "ttbar2", (int)(m_bar2->GetStyle() & WS_VISIBLE));
	profile.Write(szGeneral, "ttbar3", (int)(m_bar3->GetStyle() & WS_VISIBLE));

	RelayoutTicker();

	DrawFrame();
}

void CMainFrame::SetToolStatus()
{
	m_toolStatus = 0;
	if (m_bar0->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_JCONTROLBAR;
	if (m_bar1->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_MAINBAR;
	if (m_bar2->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_LISTBAR;
	if (m_bar3->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_HISTBAR;
	if (m_tInfo1->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_INFOBAR1;
	if (m_tInfo2->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_INFOBAR2;
	if (m_tInfo3 && (m_tInfo3->GetStyle() & WS_VISIBLE))	m_toolStatus |= TB_INFOBAR3;
	if (m_sdibar->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_SDIBAR;
	if (m_smain->GetStyle() & WS_VISIBLE)	m_toolStatus |= TB_SDIMAINBAR;
}

void CMainFrame::SetSDIHeight()
{
	m_SDIHEIGHT = SDIHEIGHT;

	if(Axis::isVista)
	{
		m_SDIHEIGHT += 9;
	}

	int	width{};
	if (m_switchpl.showCmd == SW_SHOWMAXIMIZED)
		width = GetSystemMetrics(SM_CXFULLSCREEN);
	else	
		width = m_switchpl.rcNormalPosition.right - m_switchpl.rcNormalPosition.left;

	if(Axis::isVista)
	{
		width -= 3;
	}

	SetWindowPos(&wndTop, 0, 0, width, m_SDIHEIGHT, SWP_NOZORDER);
}

void CMainFrame::SetSDIChangeHeight()
{
	if (!m_bSDI)	return;

	m_SDIHEIGHT = SDIHEIGHT;

	if(Axis::isVista)
	{
		m_SDIHEIGHT += 3;
	}

	WINDOWPLACEMENT pl;
	GetWindowPlacement(&pl);
	const int	width = pl.rcNormalPosition.right - pl.rcNormalPosition.left;

	SetWindowPos(&wndTop, 0, 0, width, m_SDIHEIGHT, SWP_NOMOVE|SWP_NOZORDER);
}

void CMainFrame::displayGuide(CString guide, CWnd* owner)
{
	if (guide.Find("TIMEOUT") >= 0)
		return;

	if (m_bSDI)
		m_axGuide->SetGuide(guide, GetDesktopWindow());
	else	
		m_axGuide->SetGuide(guide, this);
}

void CMainFrame::displayMsgBox(CString caption, CString guide)
{
	//**::MessageBox(m_hWnd, guide, caption, MB_OK);
	Axis::MessageBox(this, guide, MB_OK);
}

void CMainFrame::SetHome()
{
	if (!m_bHome)
		return;

	if (m_bSDI)
	{
		if (m_smain)
		{
			m_smain->SetHome();
			m_smain->SetHome();
		}
	}
	else
	{
		if (m_bar1)
		{
			m_bar1->SetHome();
			m_bar1->SetHome();
		}
	}
}

void CMainFrame::appendUserToolMenu()
{
	char	wb[512]{};
	CString	tmps, str;
	
	const CMenuXP*	mainM = (CMenuXP*) m_tMenu->GetPopupMenu(0);
	CMenuXP*	thisM = (CMenuXP*) mainM->GetSubMenu(USERTOOLMENU);	// fix menu position
	if (!thisM)	return;

	CProfile profile(pkUserProgram);

	const int count = (int)thisM->GetMenuItemCount();
	for (int ii = 0; ii < count - 2; ii++)
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


	const DWORD dw = GetPrivateProfileSection(szUserTool, wb, ArraySize(wb), profile.GetFileName());

	if (dw > 0)
	{
		for (int ii = 0, idx = 0;; ii++)
		{
			str = &wb[idx];
			if (str.IsEmpty())	break;
			idx += str.GetLength()+1;
			const int pos = str.Find('=');
			if (pos == -1)		continue;
			tmps = str.Left(pos);
			tmps.TrimLeft(); tmps.TrimRight();
			thisM->AppendMenuX(MF_STRING|MF_ENABLED, ID_MENU_USERTOOL+ii, tmps);
		}	
	}
}

void CMainFrame::runProgram(int index)
{
	char	buffer[1024 * 2]{};
	int	pos{}, idx = 0, count = 0;
	CString	file, str, tmpS, name;

	CProfile profile(pkUserProgram);
	strcpy_s(buffer, 2048, profile.GetSectionLF(szUserTool));
	const DWORD dw = strlen(buffer);

	if (dw <= 0)
	{
		Axis::MessageBox(this, "프로그램연결정보 오류입니다.", MB_ICONSTOP);
		return;
	}
	CString cstr = CString(buffer, dw);
	for (int ii = 0;; ii++)
	{
		//str = &buffer[idx];
		str = Parser(cstr,"\n");
		//str.TrimLeft(); str.TrimRight();
		
		
		if (str.IsEmpty())	break;
		idx += str.GetLength()+1;
		pos = str.Find('=');
		if (pos == -1)		continue;
		name = str.Left(pos++);
		if (count++ != index)	continue;
		tmpS = str.Mid(pos);
		/*
		int epos = tmpS.Find("\n");
		tmpS.Mid(1,epos);
		*/
		
		tmpS.TrimLeft(); tmpS.TrimRight();
		
		STARTUPINFO		si;
		PROCESS_INFORMATION	pi;
		char	apName[256];

		wsprintf(apName, "%s", tmpS);
		ZeroMemory(&si, sizeof(STARTUPINFO));
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;

		CString	dir;
		tmpS.MakeUpper();
		ii = tmpS.ReverseFind('\\');
		dir = tmpS.Left(ii);
		
		const BOOL bRc = CreateProcess(
				apName,			// application name
				NULL,			// command line
				NULL,			// process attribute
				NULL,			// thread attribute
				FALSE,			// is inherit handle
				0,			// creation flags
				NULL,			// environment
				dir,			// current directory
				&si,			// STARTUPINFO
				&pi);			// PROCESS_INFORMATION
		break;
	}
}

void CMainFrame::preload_screen()
{
#ifdef DF_NO_PRELOADHIDDEN

	CString mapname;
	mapname = "IBXXXX12";


	m_slog.Format("[axis][CMainFrame] load_hidescreen = [%s]\n", mapname);
	OutputDebugString(m_slog);

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{}, key = 128;
	m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void*)&vtype,
		(BYTE*)(VTS_BSTR VTS_I4), mapname, &size);

	if (vtype == vtypeERR)	
		return ;
	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;

	CView* view = GetNewView(vwKIND);
	CChildFrame* child = (CChildFrame*)view->GetParent();

	m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void*)&key,
		(BYTE*)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = GetFrameGap();

	if (vwTYPE != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;

	if (vwTYPE == vtypeSCR)
	{
		((CAxScrollView*)view)->SetInfo(key);
		((CAxScrollView*)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	((CAxisView*)view)->SetInfo(key);

	child->m_key = key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapname);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;
	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void*)&rc,
		(BYTE*)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false/*true*/);

	if (!rc)
		child->PostMessage(WM_CLOSE);

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;

	set_Trigger(key, 99);
	child->SetMisc(WM_AXIS, this, axiscall, m_fontSize, vwTYPE == vtypeWND ? false : true);
	//child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_HIDEWINDOW);
	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_SHOWWINDOW);
	child->ModifyStyle(WS_TABSTOP, NULL);
	m_arHide.Add(child);
	return;
#else
	
	if (m_hMNews==NULL) {
		CString path, emsg;
		path.Format("%s\\%s\\%s", Axis::home, DEVDIR, "IBNEWSXX.DLL");

// 	CString s;
// 	s.Format("[dll]NEW DLL PATH : [%s]\n",path);
// 	OutputDebugString(s);
		m_hMNews = LoadLibraryA(path); //test
		if (!m_hMNews)
		{
			m_hMNews = LoadLibraryEx(path, nullptr, LOAD_LIBRARY_SEARCH_USER_DIRS);
			if (!m_hMNews)
			{
				const DWORD dwError = GetLastError();
				emsg.Format("GetLastError(%d)", dwError);
				MessageBox(path, emsg);
			}
		
		}
	}
	
	if(m_hSHA256 == NULL)
	{
		CString path, emsg;
		path.Format("%s\\%s\\%s", Axis::home, DEVDIR, "CX_SHA256.DLL");
		
		// 		CString s;
		// 		s.Format("DRFN DLL PATH : [%s]\n",path);
		// 		OutputDebugString(s);
		m_hSHA256 = LoadLibraryA(path);
		if (!m_hSHA256)
		{
			const DWORD dwError = GetLastError();
			emsg.Format("암호화 모듈이 존재하지 않습니다.로그인시에 파일 전체 받기를 해주세요.(%d)", dwError);
			MessageBox(path, emsg);
		}
	}
//FDS 사용 예제
// 	CString path, emsg;
// 	path.Format("%s\\%s\\%s", Axis::home, DEVDIR, "CX_FDS.DLL");
// 
// 	HINSTANCE hFDS = LoadLibraryA(path);
// 
// 	if(!hFDS)
// 	{
// 		WORD dwError = GetLastError();
// 		emsg.Format("암호화 모듈이 존재하지 않읍니다.로그인시에 파일 전체 받기를 해주세요.(%d)", dwError);
// 		MessageBox(path, emsg);
// 	}
// 	char src[1024];
// 	char val[1024];
// 
// 	memset(val, ' ', sizeof(val));
// 
// 	FillMemory(src, sizeof(src), ' ');
// 	_ledgerH *ledger = (_ledgerH*)&src[0];
// 	sbpgt104_mid *mid = (sbpgt104_mid*)&src[L_ledgerH];
// 	
// 	memcpy(ledger->svcd, "SBPGT104", 8);
// 	memcpy(ledger->fkey, "5   ", 4);
// 	memcpy(ledger->usid, Axis::userID, Axis::userID.GetLength());
// 	ledger->odrf[0] = '1';
// 	ledger->mkty[0] = '3';
// 	memcpy(ledger->pcip, Axis::userIP, Axis::userIP.GetLength());
// 
// 	typedef int (WINAPI *GETFDSFUNC)(char*, char*,int, char*);
// 	
// 	GETFDSFUNC func = (GETFDSFUNC)GetProcAddress(hFDS, "axFDSValue");
// 
// 	if (func)
// 	{
// 		OutputDebugString("FDS LOAD SUCCESS\n");
// 		int bret =  (int)func(_T("axlogon"),src,sizeof(_ledgerH)+sizeof(sbpgt104_mid),val);
// 		CString s;
// 		s.Format("FDS RET [%d] VAL [%s]\n",bret,val);
// 		OutputDebugString(s);
// 	}
// 	else
// 	{
// 		OutputDebugString("FDS CALL FAIL\n");
// 	}

	CChildFrame*	child = NULL;
#if 1
	// 2011.01.26 LOGIN객체를 Push해주는 맵을 로딩한다. (주문대리인/투자일임업자여부체크)
	child = load_hidescreen(MAPN_LOGINSET);
	m_arHide.Add(child);
#endif
	child = load_hidescreen(MAPN_REALTIMEJANGO);
	m_arHide.Add(child);

	child = load_hidescreen("IBXXXX00");
	m_arHide.Add(child);


	// 20070604
//	child = load_hidescreen(MAPN_SISECATCH1);
//	m_arHide.Add(child);

// 	child = load_hidescreen(MAPN_MINIWID);
// 	m_arHide.Add(child);
// 
// 	m_miniWid = child;

	const int vtype = 0, size = 0, key = KEY_CODE;
	//**int ret, nCx, nCy;
	const BOOL	rc = FALSE;
	CString mapname;

//	종목제어바 코드등록
	/**
	m_category = m_bar0->GetCategoryWnd();
	mapname = _T("IBCBCODE");
	m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
				(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);
	m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
			(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)m_category, vtype, key);
	m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false);
	**/

//	계좌툴바

	//** temp
	//***m_accTool = FALSE;
	/**
	if (m_accTool)
	{
		key = KEY_ACCTOOL;
		mapname = _T(MAPN_ACCTTOOLBAR);
		m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
					(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);
		m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)m_accTool, vtype, key);
		m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false);

		int ret = 0;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
			(BYTE *)(VTS_I4 VTS_I4), setGID, (long)MAKELONG(key, -1));

		CSize	frameGap;
		frameGap.cx  = GetSystemMetrics(SM_CXFRAME);
		frameGap.cx += GetSystemMetrics(SM_CXBORDER) * 2;

		frameGap.cy  = GetSystemMetrics(SM_CYFRAME);
		frameGap.cy += GetSystemMetrics(SM_CYBORDER) * 2;
		frameGap.cy += GetSystemMetrics(SM_CYCAPTION);

		int nCx = frameGap.cx + LOWORD(size);
		int nCy = frameGap.cy + HIWORD(size);
		int	x = GetSystemMetrics(SM_CXSCREEN);
		m_accTool->SetWindowPos(NULL, x - nCx, 0, nCx, nCy, SWP_NOZORDER|SWP_HIDEWINDOW);
	}
	**/
#endif
}

CChildFrame* CMainFrame::load_hidescreen(CString mapname)
{
	//vc2019 test




	m_slog.Format("[axis][CMainFrame] load_hidescreen = [%s]\n", mapname);
	OutputDebugString(m_slog);

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{}, key = 128;
	m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
				(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);

	if (vtype == vtypeERR)	return NULL;
	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;
	
	CView	    *view  = GetNewView(vwKIND);
	CChildFrame *child = (CChildFrame *) view->GetParent();

 	m_slog.Format("HIDE SCREEN : [%s] [%d] [%d]\n",mapname,key,size);
 	OutputDebugString(m_slog);
	m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);
	m_slog.Format("HIDE SCREEN KEY: [%d]\n",key);
 	OutputDebugString(m_slog);

	int	nCx{}, nCy{};
	CSize	frameGap = GetFrameGap();

	if (vwTYPE != vtypeWND)	frameGap.cy += XCAP_HEIGHT;
	if (vwTYPE == vtypeSCR)
	{	
		((CAxScrollView *)view)->SetInfo(key);//m_key = key;
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	((CAxisView *)view)->SetInfo(key);

	child->m_key	= key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapname);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;
	child->SetSize(nCx, nCy);
	
	BOOL	rc = FALSE;
	m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false/*true*/);

	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		return NULL;
	}

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;

	set_Trigger(key, 99);
	child->SetMisc(WM_AXIS, this, axiscall, m_fontSize, vwTYPE == vtypeWND ? false : true);
	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_HIDEWINDOW);
	child->ModifyStyle(WS_TABSTOP, NULL);

	return child;
}

BOOL CMainFrame::TransKey(int keyboard)
{
	if (!m_runAxis)	return FALSE;
	if (m_activeKey < WK_NORM || m_activeKey > WK_NONE)
		return FALSE;

	if (m_bar1->IsInputFocus())	return FALSE;
	if (m_smain->IsInputFocus())	return FALSE;

	CChildFrame* child{};
	CSChild* schild{};
	BOOL		bFocus = FALSE;

	if (m_arMDI[m_vsN].Lookup(m_activeKey, child))
		bFocus = child->m_xcaption.IsActive();
	else if (m_arSDI[m_vsN].Lookup(m_activeKey, schild))
		bFocus = schild->m_xcaption.IsActive();
	else	return FALSE;

	if (!bFocus)	return FALSE;
	BOOL	value = 0;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_BOOL, (void *)&value,
				(BYTE *)(VTS_I4 VTS_I4), MAKELONG(keyEV, m_activeKey), keyboard);
	return value;
}

void CMainFrame::displayChild(int vsN, bool show/*=true*/, bool bSDI/*=false*/)
{
	int			key{};
	CArray	<int, int>	ary;
	CChildFrame*		child = NULL;
	CSChild*		schild = NULL;
	UINT			nFlags{};

	if (show) nFlags = SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER;
	else	nFlags = SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER;

	if (bSDI)
	{
		keyArray(ary, vsN, true);
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			key = ary.GetAt(ii);
			if (!m_arSDI[vsN].Lookup(key, schild))
				continue;
			//if(schild->m_mapN == "IB820850") continue;
			schild->SetWindowPos(&wndTop, 0, 0, 0, 0, nFlags);
		}
	}
	else
	{
		keyArray(ary, vsN);
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			key = ary.GetAt(ii);
			
			
			if (!m_arMDI[vsN].Lookup(key, child))
				continue;
			//if(child->m_mapN == "IB820850") continue;
			/*
			if(child->m_mapN == "IB820850") child->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER);
			else 
			*/	

// 			CString s;
// 			s.Format("VSN [%d] MAP [%s]\n",vsN,child->m_mapN);
// 			OutputDebugString(s);

			if(child->m_mapN == "IB0000X8")
			{
				continue;
			}

			child->SetWindowPos(&wndTop, 0, 0, 0, 0, nFlags);
		}
	}

	ary.RemoveAll();
}

void CMainFrame::keyArray(CArray <int, int> &ary, int vsN, bool bSDI/*=false*/)
{
	CArray	<int, int>* aryZ{};

	if (bSDI)
		aryZ = &m_sdiZORDER[vsN];
	else	aryZ = &m_hooklist[vsN];

	for (int ii = aryZ->GetSize() - 1; 0 <= ii; ii--)
	{
		const int key = aryZ->GetAt(ii);
		ary.Add(key);
	}
}

void CMainFrame::LinkOPEN(int dat, CString mapN)
{
	const int	key   = HIWORD(dat);
	const int	group = LOWORD(dat);
	CString	data, mapname;

	getScreenData(key, data, FALSE);
	data.Replace(";", "\n");
	mapname.Format("%s%s", mapN, data);
	m_mapHelper->ChangeChild(mapname, group);
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

CWnd* CMainFrame::refleshMap(int key)
{
	int		size{}, vwTYPE{}, nCx{}, nCy{};
	BOOL		rc = FALSE;
	CSize		frameGap;
	CChildFrame* child{};
	CSChild* schild{};
	CString		mapname;

	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		if (!m_arSDI[m_vsN].Lookup(key, schild))	return NULL;

		mapname = schild->m_mapN.Left(L_MAPN);
		if (ExceptionProcess(mapname))	return NULL;

		m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vwTYPE,
				(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);
		
		frameGap = GetFrameGap();
		if ((vwTYPE& vtypeMSK) != vtypeWND)	frameGap.cy += XCAP_HEIGHT;

		nCx = LOWORD(size) + frameGap.cx-4;
		nCy = HIWORD(size) + frameGap.cy-6;
		schild->SetSize(nCx, nCy);

		m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false);
		if (!rc)	return NULL;

		nCx = schild->GetSize().cx;
		nCy = schild->GetSize().cy;
		
		schild->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_NOZORDER);
		return schild;
	}
	
	mapname = child->m_mapN.Left(L_MAPN);
	if (ExceptionProcess(mapname))	return NULL;
	
	m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vwTYPE,
				(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);

	frameGap = GetFrameGap();
	if ((vwTYPE& vtypeMSK) != vtypeWND)	frameGap.cy += XCAP_HEIGHT;

	nCx = LOWORD(size) + frameGap.cx-4;
	nCy = HIWORD(size) + frameGap.cy-6;
	//CString tmp; tmp.Format("nCx: %d, nCy: %d\n", nCx, nCy);
	//AfxMessageBox(tmp);
	//TRACE(tmp);
	child->SetSize(nCx, nCy);

	m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false);
	if (!rc)	return NULL;

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;
	
	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_NOZORDER);
	
	return child;
}

void CMainFrame::ChangeMDI()
{
	m_bitmapBtn->ShowWindow(SW_SHOW);
	int			key{};
	CArray	<int, int>	ary;
	
	m_bSDI	  = false;
	m_appMode = MODE_MDI;

	//ModifyStyle(0, WS_CAPTION);
#ifdef DF_USE_CPLUS17
	ShowControlBar(m_tMenu.get(), TRUE, FALSE);
	ShowControlBar(m_bar1.get(), TRUE, FALSE);
	ChangeLogo();

	if (m_toolStatus & TB_LISTBAR)
		ShowControlBar(m_bar2.get(), FALSE, FALSE);
#else
	ShowControlBar(m_tMenu, TRUE, FALSE);
	ShowControlBar(m_bar1, TRUE, FALSE);
	ChangeLogo();

	if (m_toolStatus & TB_LISTBAR)
		ShowControlBar(m_bar2, FALSE, FALSE);
#endif
	

#if 0
	DWORD dwStyle = m_axis->GetProfileInt(INFORMATION, "listbar_pos", CBRS_ALIGN_BOTTOM);//m_bar2->GetBarStyle();
#else
	DWORD dwStyle = m_axis->GetProfileInt(INFORMATION, "listbar_pos", 0);
	if (dwStyle==0)
	{
		CProfile p(pkUserSetup);
		dwStyle = (DWORD)p.GetDouble(INFORMATION, "listbar_pos", CBRS_ALIGN_BOTTOM);
	}
	else
	{
		m_axis->WriteProfileInt(INFORMATION, "listbar_pos", 0);
		CProfile(pkUserSetup).Write(INFORMATION, "listbar_pos", (double)dwStyle);
	}

	DWORD dwStyle3 = m_axis->GetProfileInt(INFORMATION, "listbar3_pos", 0);
	if (dwStyle3==0)
	{
		CProfile p(pkUserSetup);
		dwStyle3 = (DWORD)p.GetDouble(INFORMATION, "listbar3_pos", CBRS_ALIGN_BOTTOM);
	}
	else
	{
		m_axis->WriteProfileInt(INFORMATION, "listbar3_pos", 0);
		CProfile(pkUserSetup).Write(INFORMATION, "listbar3_pos", (double)dwStyle3);
	}
#endif
	
	m_bar2->EnableDocking(CBRS_ALIGN_BOTTOM|CBRS_ALIGN_TOP);
	m_bar2->SetBarStyle(dwStyle);
	m_bar2->SetMDIMode(TRUE);
#ifdef DF_USE_CPLUS17
	if (m_toolStatus & TB_LISTBAR)
		ShowControlBar(m_bar2.get(), TRUE, FALSE);
#else
	if (m_toolStatus & TB_LISTBAR)
		ShowControlBar(m_bar2, TRUE, FALSE);
#endif

	m_bar3->EnableDocking(CBRS_ALIGN_BOTTOM|CBRS_ALIGN_TOP);
	m_bar3->SetBarStyle(dwStyle3);
	m_bar3->SetMDIMode(TRUE);

#ifdef DF_USE_CPLUS17
	if (m_toolStatus & TB_HISTBAR)
		ShowControlBar(m_bar3.get(), TRUE, FALSE);
	m_bar0->EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(m_bar0.get(), AFX_IDW_DOCKBAR_LEFT);
	ShowControlBar(m_sdibar.get(), FALSE, FALSE);
	ShowControlBar(m_smain.get(), FALSE, FALSE);
#else
	if (m_toolStatus & TB_HISTBAR)
		ShowControlBar(m_bar3, TRUE, FALSE);
	m_bar0->EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(m_bar0, AFX_IDW_DOCKBAR_LEFT);
	ShowControlBar(m_sdibar, FALSE, FALSE);
	ShowControlBar(m_smain, FALSE, FALSE);
#endif

	SetWindowPlacement(&m_switchpl);
	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		keyArray(ary, vsN, true);
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			key = ary.GetAt(ii);
			convertMDI(key, vsN);
		}
		ary.RemoveAll();
	}
	TrayCmd(TRAY_HIDEICON);
	m_bar1->Refresh();
	ChangeLogo();
	
}

void CMainFrame::ChangeSDI(bool hide)
{
	m_bitmapBtn->ShowWindow(SW_HIDE);
	int			key{};
	int            vsN{}; //vc2019
	CArray	<int, int>	ary;
	CSChild* schild{};
	
	m_bSDI	  = true;
	m_appMode = hide ? MODE_TRAY : MODE_SDI;

	GetWindowPlacement(&m_switchpl);

	ShowWindow(SW_HIDE);
	if (m_switchpl.showCmd == SW_SHOWMAXIMIZED)
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);
		ShowWindow(SW_HIDE);
	}

	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		keyArray(ary, vsN);
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			key = ary.GetAt(ii);

			CChildFrame* child;

			if(m_arMDI[vsN].Lookup(key,child))
			{
				if(child->m_mapN == "IB0000X8")
				{
					continue;
				}
			}

			convertSDI(key, vsN);
		}
		ary.RemoveAll();
	}

	for (vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		for (POSITION pos = m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			schild->m_xcaption.SetSDI();
		}
	}

	SetToolStatus();

	ModifyStyle(WS_CAPTION, 0);
#ifdef DF_USE_CPLUS17
	ShowControlBar(m_tMenu.get(), FALSE, FALSE);
	ShowControlBar(m_bar1.get(), FALSE, FALSE);
	ShowControlBar(m_bar0.get(), FALSE, FALSE);
	ShowControlBar(m_bar2.get(), FALSE, FALSE);
	ShowControlBar(m_bar3.get(), FALSE, FALSE);
	ShowControlBar(m_sdibar.get(), TRUE, FALSE);
	ShowControlBar(m_smain.get(), TRUE, FALSE);
#else
	ShowControlBar(m_tMenu, FALSE, FALSE);
	ShowControlBar(m_bar1, FALSE, FALSE);
	ShowControlBar(m_bar0, FALSE, FALSE);
	ShowControlBar(m_bar2, FALSE, FALSE);
	ShowControlBar(m_bar3, FALSE, FALSE);
	ShowControlBar(m_sdibar, TRUE, FALSE);
	ShowControlBar(m_smain, TRUE, FALSE);
#endif
	
	SetSDIChangeHeight();
	SetSDIHeight();
			
	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);

#ifdef DF_USE_CPLUS17
	const CPoint	sdiPt = CPoint(pl.rcNormalPosition.left, m_switchpl.rcNormalPosition.top + m_SDIHEIGHT);
	FloatControlBar(m_bar0.get(), sdiPt);
	m_bar0->EnableDocking(0);
#else
	const CPoint	sdiPt = CPoint(pl.rcNormalPosition.left, m_switchpl.rcNormalPosition.top + m_SDIHEIGHT);
	FloatControlBar(m_bar0, sdiPt);
	m_bar0->EnableDocking(0);
#endif


	

	if (!hide)	
		ShowWindow(SW_SHOW);

	TrayCmd(TRAY_HIDEICON);
	m_smain->Refresh();
	ChangeLogo();
}

void CMainFrame::ChangeTRAY()
{
	PostMessage(WM_CANCELMODE);
	switch (m_appMode)
	{
	case MODE_MDI:	ChangeSDI(true);	break;
	case MODE_SDI:	ShowWindow(SW_HIDE);	break;
	default:	break;
	}

	m_appMode = MODE_TRAY;
	TrayCmd(TRAY_SHOWICON);
}

void CMainFrame::TrayCmd(int cmd)
{
	m_tray.cbSize	= sizeof(NOTIFYICONDATA);
	m_tray.hWnd	= GetSafeHwnd();
	m_tray.uID	= IDI_CLIENT;
	m_tray.hIcon	= m_resourceHelper->GetIcon();
	m_tray.uCallbackMessage = WM_TRAYEVENT;

	switch (cmd)
	{
	case TRAY_SHOWICON:
		m_tray.uFlags	= NIF_MESSAGE | NIF_ICON | NIF_TIP;
		strcpy_s (m_tray.szTip, sizeof(m_tray.szTip), m_regkey);

		Shell_NotifyIcon(NIM_ADD, &m_tray);
		break;
	case TRAY_HIDEICON:
		m_tray.uFlags = 0;
		Shell_NotifyIcon(NIM_DELETE, &m_tray);
		break;
	default:return;
	}
}

void CMainFrame::StopLoss()
{
	UnderConstruction();
	return;

	const bool onlysise = m_axis->GetProfileInt(WORKSTATION, "OnlySise", 0) ? TRUE : FALSE;
	if (onlysise)	return;
	if (!m_stoploss)
	{
		CStopWarn	dlg;
		if (dlg.DoModal() != IDOK)	return;
#ifdef DF_USE_CPLUS17
		m_stoploss = new CStoploss(m_bar2.get());
#else
		m_stoploss = new CStoploss(m_bar2);
#endif
		if (!m_stoploss->Create(IDD_STOPLOSS))
		{
			delete m_stoploss;
			m_stoploss = NULL;
		}
		else
		{
			BOOL	rc = 0;
			int	vtype = 0, size = 0, ret{}, key = KEY_STOPLOSS;
			CString mapname = MAPN_STOPLOSS;

			m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
						(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);
			m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
					(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)m_stoploss, vtype, key);
			m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false);

			ret = 0;
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
				(BYTE *)(VTS_I4 VTS_I4), setGID, (long)MAKELONG(key, 0));

			CSize	frameGap;
			frameGap.cx  = GetSystemMetrics(SM_CXFRAME);
			frameGap.cx += GetSystemMetrics(SM_CXBORDER) * 2;

			frameGap.cy  = GetSystemMetrics(SM_CYFRAME);
			frameGap.cy += GetSystemMetrics(SM_CYBORDER) * 2;
			frameGap.cy += GetSystemMetrics(SM_CYCAPTION);

			const int nCx = frameGap.cx + LOWORD(size);
			const int nCy = frameGap.cy + HIWORD(size);

			m_stoploss->SetWindowPos(&wndTopMost, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);
		}
	}

	CRect	mRc, wRc;

	mRc = GetWndRect();
	m_stoploss->GetWindowRect(wRc);

	int x = mRc.right - wRc.Width() - GetSystemMetrics(SM_CXFIXEDFRAME);
	int y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
	if (x < 0)	x = 0;
	if (y < 0)	y = 0;
	if (mRc.right > GetSystemMetrics(SM_CXSCREEN))
		x = GetSystemMetrics(SM_CXSCREEN) - wRc.Width();
	if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
		y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();

	if (m_stoploss->GetStyle() & WS_VISIBLE)
		m_stoploss->ShowWindow(SW_HIDE);
	else	m_stoploss->SetWindowPos(&wndTopMost, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
}

void CMainFrame::drawTitle(CDC* pDC)
{
	if (!m_resourceHelper) 
		return;
	
	CDC		memDC;
	CRect		cRc, iRc, tRc;
	const  CRect clipRc;
	int		round{};

	cRc = GetCapRect();
	tRc.CopyRect(&cRc);

	const int leftCaptionShapeWidth = 100;

	round		= cRc.Height()/2;

	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(pDC, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		if (!DrawCaption(&memDC, cRc))
		{
			COLORREF	sColor{}, eColor{};
		
			CAxDrawHelper painter(&memDC);
			if (m_activeCap)
			{
				sColor = RGB(8, 24, 47);
				eColor = RGB(171, 180, 189);
				painter.DrawGradient(CRect(0, 0, cRc.Width(), cRc.Height()), sColor, eColor, true);
			}
			else
			{
				sColor = RGB(93, 102, 114);
				eColor = RGB(163, 185, 217);
				painter.DrawGradient(CRect(0, 0, cRc.Width(), cRc.Height()), sColor, eColor, true);
			}
		}
// draw icon
		iRc = m_arRc.GetAt(MIDX_ICON);
		if (iRc.IsRectEmpty())
		{
			SetRegion();
			iRc = m_arRc.GetAt(MIDX_ICON);
		}
		//**DrawBitmapByMask(&memDC, MIDX_ICON, iRc);
		//DrawIconEx (memDC.m_hDC, iRc.left, iRc.top, m_resourceHelper->m_hIcon,
		//		iRc.Width(), iRc.Height(), 0, NULL, DI_NORMAL|DI_COMPAT);

// draw system button
		iRc = m_arRc.GetAt(MIDX_CLOSE);	
		DrawBitmapByMask(&memDC, MIDX_CLOSE, iRc);

		if (IsZoomed())
		{
			iRc = m_arRc.GetAt(MIDX_RESTORE);
			DrawBitmapByMask(&memDC, MIDX_RESTORE, iRc);
		}
		else
		{
			iRc = m_arRc.GetAt(MIDX_MAX);
			DrawBitmapByMask(&memDC, MIDX_MAX, iRc);
		}
		iRc = m_arRc.GetAt(MIDX_MIN);	
		DrawBitmapByMask(&memDC, MIDX_MIN, iRc);
// draw user button

		for (int i = MIDX_HOME; i <= MIDX_VS6; i++)
		{
			DrawBitmapByMask(&memDC, i, m_arRc.GetAt(i));
		}
		iRc = m_arRc.GetAt(MIDX_HOME);

// draw text
		CFont*	font = memDC.SelectObject(m_resourceHelper->GetFont());

		if (m_activeCap)
			memDC.SetTextColor(RGB(255, 255, 255));
		else	
			memDC.SetTextColor(RGB(192, 192, 192));
	
		//GetWindowText(title);
		memDC.SetBkMode(TRANSPARENT);

		CString callCenter = (m_rndkey%2) ? CALLCENTER1 : CALLCENTER2;
		callCenter += CALLCENTER3;

		callCenter = m_strServer + "  " + callCenter;

		const CSize sz = memDC.GetTextExtent(m_bOnlySise ? "       "+callCenter : callCenter);
		iRc.OffsetRect(-sz.cx - 20, 0);
		iRc.right = iRc.left + sz.cx;
		memDC.DrawText(m_bOnlySise ? "       "+callCenter : callCenter, iRc, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	
		CString	title = m_saveTitle; //**m_titleS ;
		tRc.top	  = 0;
		tRc.left  = m_arRc.GetAt(MIDX_ICON).right + GAP;
		tRc.right = m_arRc.GetAt(MIDX_HOME).left - GAP;
		tRc.bottom= tRc.top + cRc.Height();

		/**
		if (m_dev) // paint dev info
		{
			tRc.OffsetRect(leftCaptionShapeWidth, 0);
			memDC.DrawText(m_titleS, tRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		**/

		CString caption = "IBK hot Trading";
		if (!Axis::isCustomer)	caption += "(직원용)";
		if (Axis::devMode)     caption += " - 개발용";

		if(m_titleChange)
		{
			caption = m_userTitle;

			if (!Axis::isCustomer)	caption += "(직원용)";
			if (Axis::devMode)     caption += " - 개발용";

			CString tmp;
			GetWindowText(tmp);

			if(tmp != caption)
			{
				SetWindowText(caption);
			}
		}
		else
		{
			CString tmp;
			GetWindowText(tmp);

			if (m_axMisc->m_regkey == "IBK")
			{
				if(tmp != "IBK개발")
					SetWindowText("IBK개발");
			}
			else if (m_axMisc->m_regkey == "IBK_STAFF")
			{
				if(tmp != "[직원]IBK개발")
					SetWindowText("[직원]IBK개발");
			}
			else if (m_axMisc->m_regkey == "IBKMAC_STAFF")
			{
				if(tmp != "[직원]IBK hot Trading")
					SetWindowText("[직원]IBK hot Trading");
			}
			else
			{
				if(tmp != "IBK hot Trading")
					SetWindowText("IBK hot Trading");
			}
		}

		if (!m_netTypeStr.IsEmpty())
		{
			CString pingInfo;
			pingInfo.Format(" %s %dms | %s", m_netTypeStr, m_pingMs, m_sMonitor);
			caption += pingInfo;
		}


		memDC.TextOut(100, 4, caption);

		memDC.SelectObject(font);

		pDC->BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
}

CRect CMainFrame::GetCapRect()
{
	CRect	rc{};
	GetWindowRect(&rc);
	rc.OffsetRect(-rc.TopLeft());
	rc.top += GetSystemMetrics(SM_CYFRAME);
	rc.bottom = rc.top + GetSystemMetrics(SM_CYCAPTION) - 2 ;
	return rc;
}

void CMainFrame::SetRegion()
{
	const int gapVS = -3;
	
	CRect	cRc, iRc;
	const CRect iRc2;
	CPoint	pt;
	BITMAP	bm{};
	
	cRc = GetCapRect();
	CBitmap* bmp = Axis::GetBitmap(CBTN_CLOSE);
	if (!bmp || !bmp->m_hObject)
		return;
	
	bmp->GetBitmap(&bm);

	pt = CPoint(cRc.right - bm.bmWidth/2, (cRc.Height() - bm.bmHeight)/2);
	iRc.SetRect(pt.x, pt.y, pt.x + bm.bmWidth/2, pt.y + bm.bmHeight);
	iRc.OffsetRect(-GAP*3, 0);

	for (int ii = MIDX_CLOSE; ii > MIDX_ICON; ii--)
	{
		switch (ii)
		{
		case MIDX_HOME:
			m_arRc.SetAt(MIDX_HOME, iRc);
			iRc.OffsetRect(-1, 0);		break;
		case MIDX_FULL:
			m_arRc.SetAt(MIDX_FULL, iRc);
			iRc.OffsetRect(-1, 0);		break;
		case MIDX_DUAL:
			iRc.InflateRect(1, 0, -1, 1);
			m_arRc.SetAt(MIDX_DUAL, iRc);
			iRc.OffsetRect(-1, 0);		break;
		case MIDX_VS1:
			m_arRc.SetAt(MIDX_VS1, iRc);
			iRc.OffsetRect(-GAP*2, 0);	break;
		case MIDX_VS2:
			m_arRc.SetAt(MIDX_VS2, iRc);
			iRc.OffsetRect(gapVS, 0);		break;
		case MIDX_VS3:
			m_arRc.SetAt(MIDX_VS3, iRc);
			iRc.OffsetRect(gapVS, 0);		break;
		case MIDX_VS4:
			m_arRc.SetAt(MIDX_VS4, iRc);
			iRc.OffsetRect(gapVS, 0);		break;
		case MIDX_VS5:
			m_arRc.SetAt(MIDX_VS5, iRc);
			iRc.OffsetRect(gapVS, 0);		break;
		case MIDX_VS6:
			m_arRc.SetAt(MIDX_VS6, iRc);
			iRc.OffsetRect(gapVS, 0);		break;
		case MIDX_MIN:
			m_arRc.SetAt(MIDX_MIN, iRc);
			iRc.OffsetRect(-GAP*2 + 1, 0);	break;
		case MIDX_MAX:
			m_arRc.SetAt(MIDX_RESTORE, iRc);
			m_arRc.SetAt(MIDX_MAX, iRc);
			iRc.OffsetRect(-1, 0);		
			break;
		case MIDX_CLOSE:
			m_arRc.SetAt(MIDX_CLOSE, iRc);
			iRc.OffsetRect(-1, 0);		
			break;
		default:		
			continue;
		}

		switch (ii)
		{
		case MIDX_HOME:	case MIDX_FULL:	case MIDX_DUAL:
			iRc.OffsetRect(-iRc.Width(), 0);	
			break;
		default:
			if (ii >= MIDX_VS1 && ii <= MIDX_VS6)
			{
				iRc.OffsetRect(-14, 0);
			}
			else
			{
				iRc.OffsetRect(-bm.bmWidth/2, 0);	
			}
			break;
		}
	}

	const int	cx = GetSystemMetrics(SM_CXSMICON);
	const int	cy = GetSystemMetrics(SM_CYSMICON);

	pt = CPoint((cRc.Height() - cy)/2 + GAP, (cRc.Height() - cy)/2);
	iRc.SetRect(pt.x, pt.y, pt.x + cx, pt.y + cy);
	
	m_arRc.SetAt(MIDX_ICON, iRc);
}

int CMainFrame::DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown, int maskcolor)
{
	if (bRC.IsRectEmpty())	return 0;

	BITMAP		bm{};
	CBitmap* bitmap{};
	int		bx{}, by{}, sx{};

	bx = bRC.Width();
	by = bRC.Height();
	sx = bDown ? bx: 0;

	class BitmapBranch
	{
	public:
		BitmapBranch(CBitmap** bmp, const char* skinBmpName, int index, int& sx, BOOL bDown, CRect& rc, BOOL active)
		{
			if (active)
				*bmp = Axis::GetBitmap(skinBmpName);
			else
				*bmp = Axis::GetBitmap(Format("XN_%s", skinBmpName));
			if (!*bmp)
				return;
			BITMAP bm;
			(*bmp)->GetBitmap(&bm);
			sx = bm.bmWidth / 3; 
			if (bDown)
				sx += bm.bmWidth / 3 * 2;

			rc.top += 2;
		}
	};

	switch (index)
	{
	case MIDX_CLOSE:
		if (m_activeCap)
			bitmap = Axis::GetBitmap(CBTN_CLOSE);
		else
			bitmap = Axis::GetBitmap(Format("XN_%s", CBTN_CLOSE));
		if (!bitmap)
			return 0;
		bitmap->GetBitmap(&bm);
		break;
	case MIDX_MAX:
		if (m_activeCap)
			bitmap = Axis::GetBitmap(CBTN_MAX);
		else
			bitmap = Axis::GetBitmap(Format("XN_%s", CBTN_MAX));
		if (!bitmap)
			return  0;
		bitmap->GetBitmap(&bm);
		break;
	case MIDX_MIN:
		if (m_activeCap)
			bitmap = Axis::GetBitmap(CBTN_MIN);
		else
			bitmap = Axis::GetBitmap(Format("XN_%s", CBTN_MIN));
		if (!bitmap)
			return 0;
		bitmap->GetBitmap(&bm);
		break;
	case MIDX_RESTORE:
		if (m_activeCap)
			bitmap = Axis::GetBitmap(CBTN_RESTORE);
		else
			bitmap = Axis::GetBitmap(Format("XN_%s", CBTN_RESTORE));
		if (!bitmap)
			return 0;
		bitmap->GetBitmap(&bm);
		break;
	case MIDX_VS6:case MIDX_VS5: case MIDX_VS4:
	case MIDX_VS3:case MIDX_VS2: case MIDX_VS1:
		{
			int	nIndex = index - MIDX_VS1;

			bitmap = Axis::GetBitmap(IDB_VSCREEN);
			if (!bitmap)
				return 0;
			bitmap->GetBitmap(&bm);

			const int buttonWidth = bm.bmWidth / 12;

			nIndex = (nIndex * 2) + ((nIndex == m_vsN) ? 1: 0);

			sx = (nIndex * buttonWidth);
			bx = buttonWidth;
			by = bm.bmHeight;

			//bRC.top += 1;
		}
		break;
	case MIDX_DUAL:
		BitmapBranch(&bitmap, "DUEL", index, sx, bDown, bRC, m_activeCap);
		break;
	case MIDX_FULL:
		BitmapBranch(&bitmap, "FULL", index, sx, bDown, bRC, m_activeCap);
		break;
	case MIDX_HOME:
		BitmapBranch(&bitmap, "HOME", index, sx, bDown, bRC, m_activeCap);
		break;
	case MIDX_ICON:
		bitmap = m_resourceHelper->GetSiteIcon();
		break;
	default:
		return 0;
	}

	if (!bitmap)
		return 0;

	CAxDrawHelper painter(pDC);
	
// 	if (Axis::isVista)
// 	{
// // 		bRC.left    += VistaFrame;
// // 		bRC.right   += VistaFrame;
// 		bRC.top     += VistaFrame;
// 		bRC.bottom  += VistaFrame;
// 	}
	
	painter.DrawIndexedBitmap(bRC.left, bRC.top, bitmap, maskcolor, bx, by, sx);

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
		/*
		if (Axis::isVista)
		{
			bRc.left    += VistaFrame;
			bRc.right   += VistaFrame;
			bRc.top     += VistaFrame;
			bRc.bottom  += VistaFrame;
		}
		*/
		//***bRc.OffsetRect(XGAP, YGAP);
		if (!bRc.PtInRect(pt))
			continue;

		if (ii == MIDX_MAX && IsZoomed())
			return MIDX_RESTORE;
		else	return ii;
	}
	return -1;
}

CString CMainFrame::GetConnectIP(CString dns)
{
	CString	file;
	char	glbIP[32]{};

	file.Format("%s\\exe\\EnMsgOem.dll", Axis::home);
	HMODULE hMod = LoadLibrary(file);
	if (hMod)
	{
		typedef struct hostent * (*fnGetHostByName) (const char * name);
		fnGetHostByName egethostbyname = (fnGetHostByName) GetProcAddress(hMod, "egethostbyname");
		const struct hostent *hp = egethostbyname(dns);
		if (hp)
			sprintf(glbIP, "%s", inet_ntoa(*((struct in_addr*)hp->h_addr)));
		else	sprintf(glbIP, dns);
		FreeLibrary(hMod);

		return glbIP;
	}
	return dns;
}

void CMainFrame::RunHelpCom()
{
	/**
	if (m_siteID != 'D')
	{
		char	buf[512];
		CString	file;
		file.Format("%s\\tab\\axis.ini", Axis::home);
		DWORD dw = GetPrivateProfileString("WebLinkHN", "remote", "", buf, sizeof(buf), file);
		if (dw > 0)	ShellExecute(NULL, _T("open"), buf, NULL,NULL, SW_SHOWNORMAL);
		return;
	}
	**/
	
	CHelpOK	dlg;
	if (dlg.DoModal() != IDOK)
		return;
	
	CString	aps, cmds, exes;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	//cmds.Format("%s\\exe\\HelpCom.exe type:2/aip:210.107.66.228/aport:6090/eip:210.107.66.228/eport:6080/t1:%s",
	//			Axis::home, Axis::user);
	//cmds.Format("%s\\exe\\HCUser.exe 210.107.66.228 6080 %s 123-1234 none 0000", Axis::home, Axis::user);
	//aps.Format("%s\\exe\\HCUser.exe", Axis::home);
	cmds.Format("%s\\exe\\AHCUser.exe 210.107.66.228 6080 %s 123-1234 none 0000 0 none", Axis::home, Axis::userID);
	aps.Format("%s\\exe\\AHCUser.exe", Axis::home);
	

	const BOOL bRc = CreateProcess(
			NULL/*aps*/,				// application name
			(char*)(const char*)cmds,	// command line
			NULL,						// process attribute
			NULL,						// thread attribute
			FALSE,						// is inherit handle
			0,							// creation flags
			NULL,						// environment
			NULL,						// current directory
			&si,						// STARTUPINFO
			&pi);						// PROCESS_INFORMATION
}

void CMainFrame::fitDual()
{
	const int moniterN = GetSystemMetrics(SM_CMONITORS);

	if (moniterN <= 1)
	{
		Axis::MessageBox(this, "듀얼모니터가 아닙니다.", MB_ICONINFORMATION);
		return;
	}
	

	WINDOWPLACEMENT	pl;
	

	GetWindowPlacement(&pl);
	g_monRc.SetRectEmpty();
	EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);
	
	if (m_bfitdual) m_bfitdual = false;
	else m_bfitdual = true;

	WINDOWPLACEMENT tmpl;

	GetWindowPlacement(&tmpl);

	if(m_bfitdual && tmpl.rcNormalPosition.right == g_monRc.right)
	{
		m_bfitdual = false;
	}

	RECT rcWork{};

// 	CString s;
// 	s.Format("FITDUAL : [%d]\n",m_bfitdual);
// 	OutputDebugString(s);
	
	if (m_bfitdual) 
	{
		GetWindowPlacement(&m_predual);
		pl.showCmd = SW_RESTORE;
		pl.rcNormalPosition.left   = g_monRc.left;
		
// 		s.Format("FITDUAL RIGHT: [%d]	[%d]\n",m_predual.rcNormalPosition.right,pl.rcNormalPosition.right);
// 		OutputDebugString(s);

// 		if(m_predual.rcNormalPosition.right == g_monRc.right)
// 		{
// 			//m_predual.rcNormalPosition.right = g_monRc.right / 2;
// 			SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 ) ;
// 
// 			m_predual.rcNormalPosition.right = rcWork.right;
// 		}

		pl.rcNormalPosition.top    = g_monRc.top;
		pl.rcNormalPosition.right  = g_monRc.right;
		pl.rcNormalPosition.bottom = GetSystemMetrics(SM_CYMAXIMIZED) -5;//g_monRc.bottom;
		SetWindowPlacement(&pl);
	}
	else
	{
// 		s.Format("FITDUAL FALSE: [%d]\n",m_predual.rcNormalPosition.right);
// 		OutputDebugString(s);

		if(m_predual.rcNormalPosition.right == 0)
		{
			GetWindowPlacement(&m_predual);

			SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 ) ;

// 			s.Format("FITDUAL FALSE2: [%d]\n",rcWork.right);
// 			OutputDebugString(s);

			m_predual.rcNormalPosition.top = rcWork.top;
			m_predual.rcNormalPosition.left = rcWork.left;
			m_predual.rcNormalPosition.bottom = rcWork.bottom;
			m_predual.rcNormalPosition.right = rcWork.right;
		}

		SetWindowPlacement(&m_predual);
	}
	/*
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
	*/
	
}

//기존 해외 지수
// const int indexKey[] = {
// 	1, 2, 3, 4, 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20,
// 		21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 
// };
//새로운 해외 지수
// const int indexKey[] = {
// 	1, 2, 3, 4, 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20,
// 		21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50
// };
//티커변경시 수정
const int indexKey[] = {
	1, 2, 3, 4,/*4,*/ 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 32, 33
};

const int indexKeyCount = ArraySize(indexKey);

int CMainFrame::GetTicKey(int index)
{
	if (index >= 0 && index < indexKeyCount)
		return indexKey[index];
	else
		return 0;

}

int CMainFrame::ConvertIndexKey(int index)
{
	for (int i = 0; i < indexKeyCount; i++)
	{
		if (indexKey[i] == index)
			return i;
	}

	return 0;
}

void CMainFrame::convertIndex()
{
	CString		section, keys, value;
	CStringArray	ary;

	CProfile profile(pkAxisTicker);

	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			section.Format("BAR_INFO_%d%d", ii, jj);
			const int tKIND = profile.GetInt(section, "INDEXK", TKIND_INDEX);
			if (tKIND != TKIND_INDEX)	continue;

			keys = _T("00");
			value = profile.GetString(section, keys);
			if (value.IsEmpty())
				continue;

			ary.RemoveAll();
			for (int idx = 0; ; idx++)
			{
				keys.Format("%02d", idx);
				value = profile.GetString(section, keys);
				if (value.IsEmpty())
					break;
				ary.Add(value);
			}

			profile.WriteSection(section, "");
			profile.Write(section, "INDEXK", "0");

			for (int idx = 0; idx < ary.GetSize(); idx++)
			{
				keys.Format("%03d", idx);
				value.Format("%02d", ConvertIndexKey(atoi(ary.GetAt(idx))));
				profile.Write(section, keys, value);
			}
			ary.RemoveAll();
		}
	}
}

void CMainFrame::deleteNewsfile()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	CFile		cfile;
	CString		allfile, filename;

	allfile.Format("%s\\%s\\*.html", Axis::home, USRDIR);
	hFind = FindFirstFile(allfile, &FindFileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		filename.Format("%s\\%s\\%s", Axis::home, USRDIR, FindFileData.cFileName);
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			DeleteFile(filename);
		if (!FindNextFile(hFind, &FindFileData))	break;
	}
}

void CMainFrame::OpenAnnouncement()
{
	CString		keys, value;

	CProfile profile(GetProfileFileName());

//	check popup
	for (int ii = 0; ; ii++)
	{
		keys.Format("open%02d", ii);
		value = profile.GetString(szDTAccouncement, keys);
		if (value.IsEmpty())
			break;

		if (!isOpenAnnouncement(value))
			continue;
#if 0
		m_mapHelper->CreatePopup(value, 1, WK_POPUP, CenterPOS);
#else
		m_mapHelper->CreateChild(value, 0, 0, CenterPOS);
#endif
	}

//	absolute popup
	for (int ii = 0; ; ii++)
	{
		keys.Format("absopen%02d", ii);
		value = profile.GetString(szDTAccouncement, keys);
		if (value.IsEmpty())
			break;
		m_mapHelper->CreatePopup(value, 1, WK_POPUP, CenterPOS);
	}

	for (int ii = 0; ; ii++)
	{
		keys.Format("webopen%02d", ii);
		value = profile.GetString(szDTAccouncement, keys);
		if (value.IsEmpty())
			break;

		CString	urls, tmps;
		int	cx{}, cy{}, pos{};
		
		urls = value;
		pos = urls.Find('|');
		if (pos == -1)
		{
			cx = 800;
			cy = 600;
		}
		else
		{
			cx = _ttoi(urls.Left(4));
			cy = _ttoi(urls.Mid(4, 4));
			urls = urls.Mid(pos+1);
		}

		PopupWeb(urls, cx, cy);
	}
// 배경화면 공지사항

	keys.Format("bknotice%02d", m_nBkMode);
	value = profile.GetString(szDTAccouncement, keys);
	if (!value.IsEmpty() && isOpenAnnouncement(value))
		m_mapHelper->CreateBkNotice(value, 0, 0, CenterPOS);
}

bool CMainFrame::isOpenAnnouncement(CString mapName)
{
	CProfile profile(pkUserConfig);
	CString		times, keys;
	SYSTEMTIME	tm;

	GetLocalTime(&tm);
	times.Format("%04d%02d%02d", tm.wYear, tm.wMonth, tm.wDay);

	CString value(profile.GetString(mapName, "DATE", "00000000"));
	if (!times.CompareNoCase(value))	
		return false;
	else
		return true;
}

bool CMainFrame::ExceptMap(CString mapN)
{
	CString mapname = mapN.Left(L_MAPN);
	if (!mapname.CollateNoCase("DH000099"))
		return true;
	if (!mapname.CollateNoCase(MAPN_GONGJI))
		return true;
	if (mapname.Find("IB7700")>=0)
		return true;
	if(mapname.Find("IB7805")>=0)
		return true;
	if(mapname.Find("IB700000")>=0)
	{
		return true;
	}
	if(mapname.Find("IB710000")>=0)
	{
		return true;
	}
	if(mapname.Find("IB720000")>=0)
	{
		return true;
	}
	if(mapname.Find("IB701000")>=0)
	{
		return true;
	}
	if(mapname.Find("IB702000")>=0)
	{
		return true;
	}
	if(mapname.Find("IB702100")>=0)
	{
		return true;
	}
	if(mapname.Find("IB740100")>=0)
	{
		return true;
	}
	if(mapname.Find("IB703000")>=0)
	{
		return true;
	}
	if(mapname.Find("IB704000")>=0)
	{
		return true;
	}
	if(mapname.Find("IB730000")>=0)
	{
		return true;
	}
	if(mapname.Find("IB731000")>=0)
	{
		return true;
	}

	if (ScreenCheck(mapname, 1) == DF_NUSE)
		return true;

	return false;
}

CString CMainFrame::FindKeyByTargetAndType(const CString& strTargetKey, int nType)
{
	POSITION pos = m_mapManage.GetStartPosition();
	while (pos != NULL)
	{
		CString strKey, strValue;
		m_mapManage.GetNextAssoc(pos, strKey, strValue);

		// value를 '|' 기준으로 파싱
		// 형식: 필드0|필드1|필드2|필드3|
		CString strTemp = strValue;
		CStringArray arrFields;

		int nFind = 0;
		while ((nFind = strTemp.Find('|')) != -1)
		{
			arrFields.Add(strTemp.Left(nFind));
			strTemp = strTemp.Mid(nFind + 1);
		}
		// 마지막 잔여값도 추가
		arrFields.Add(strTemp);

		// 필드 수 최소 4개 필요 (인덱스 0~3)
		if (arrFields.GetSize() < 4)
			continue;

		CString strField1 = arrFields[1]; // 타입 (예: "5")
		CString strField3 = arrFields[3]; // 매핑키 (예: "IB202700")

		// 두 조건 모두 일치하면 key 반환
		if (strField3 == strTargetKey && _ttoi(strField1) == nType)
		{
			return strKey; // "IB202200" 반환
		}
	}

	return _T(""); // 못 찾으면 빈 문자열
}

void CMainFrame::HandleScreenPopup(const CString& sPopType, const CString& sPopMap, const CString& sUrl, int itimer)
{
	if (!sPopMap.IsEmpty())
	{
		if (itimer > 0)
		{
			m_sNoticePopType = sPopType;
			m_sNoticePopMap = sPopMap;
			SetTimer(TM_NOTICE_POP, itimer, nullptr);
		}
		else
		{
			CString popType = sPopType;
			if (popType.IsEmpty())
				popType = "1";

			if (popType == "1")
				m_mapHelper->ChangeChild(sPopMap, 1, 0, 5);
			else if (popType == "2")
				m_mapHelper->CreateModal(sPopMap);
			else if (popType == "3")
				m_mapHelper->CreatePopup(sPopMap, 1, 0, 5);
		}
	}
	else if (sPopType == "0")
	{
		return;
	}
	else
	{
		CDlg_MSGBOX dlg;
		if (dlg.DoModal() == IDOK)
		{
			m_iKey = m_mapHelper->ChangeChild("IB980001");
			m_sTriggerUrl.Format("edUrlTrigger\t%s", sUrl);
			SetTimer(TM_NOSCREEN_URL, 1000, nullptr);
		}
	}
}

bool CMainFrame::IsManagerUser() const
{
	// TODO: 실제 관리자 여부 판정
	return false;
}

bool CMainFrame::IsDebugUser() const
{
	// TODO: 실제 디버그 허용 사용자 여부 판정
	return false;
}

CString CMainFrame::GetTodayYmd() const
{
	CTime now = CTime::GetCurrentTime();

	CString s;
	s.Format("%04d%02d%02d",
		now.GetYear(),
		now.GetMonth(),
		now.GetDay());

	return s;
}

bool CMainFrame::IsGuidePopupTime() const
{
	char buff[256] = {};
	CString file, tmps;

	file.Format("%s\\%s\\SWITCHING.INI", Axis::home, TABDIR);

	GetPrivateProfileString("해외", "STIM", "720", buff, sizeof(buff), file);
	tmps = buff;
	tmps.TrimRight();
	const int from = _ttoi(tmps);

	GetPrivateProfileString("해외", "ETIM", "750", buff, sizeof(buff), file);
	tmps = buff;
	tmps.TrimRight();
	const int end = _ttoi(tmps);

	CTime now = CTime::GetCurrentTime();
	const int nowTime = now.GetHour() * 100 + now.GetMinute();

	return (nowTime >= from && nowTime <= end);
}

int CMainFrame::ScreenCheck(CString mapname, int igubn)
{
	//#define DF_NUSE 0                  //#0x00 사용불가
	//#define DF_YUSE 1                  //#0x01 사용가능
	//#define DF_YUSE_NOTICEPOP 2  //#0x02 공지창도 같이 띄우기    2
	//#define DF_YUSE_AFTERDAY 4         //#0x04 특정날짜이후 사용중지
	//#define DF_YUSE_MANAGER 8          //#0x08 관리자전용
	//#define DF_YUSE_DEBUG 16           //#0x10 디버그기능 포함
	//#define DF_NUSE_HTSOPEN 32         //#HTS 시작할때는 안띄움
	//#define DF_YUSE_GUIDEPOP 64        //특정시간에는 대체팝업

	auto HasFlag = [](int value, int flag) -> bool
	{
		return (value & flag) == flag;
	};

	m_slog.Format("[ScreenCheck] mapname=%s igubn=[%d]", mapname, igubn);
	OutputDebugString(m_slog);

	CString sData;
	if (!m_mapManage.Lookup(mapname, sData))
	{
		if (mapname.Left(2) == "00")
		{
			mapname = "IB" + mapname.Mid(2);
			if (!m_mapManage.Lookup(mapname, sData))
			{
				m_slog.Format("[ScreenCheck]1 YESUSE mapname=%s igubn=[%d]", mapname, igubn);
				OutputDebugString(m_slog);
				return DF_YUSE;
			}
		}
		else
		{
			m_slog.Format("[ScreenCheck]2 YESUSE mapname=%s igubn=[%d]", mapname, igubn);
			OutputDebugString(m_slog);
			return DF_YUSE;
		}
	}

	CString sval, sDate, sPopType, sPopMap, sUrl;
	sval = Parse(sData, "|");
	sPopType = Parse(sData, "|");
	sDate = Parse(sData, "|");
	sPopMap = Parse(sData, "|");
	sUrl = Parse(sData, "|");

	const int nFlag = _ttoi(sval);

	m_slog.Format("[ScreenCheck] nFlag=%d sDate=%s sPopType=%s sPopMap=%s sUrl=%s",
		nFlag, sDate, sPopType, sPopMap, sUrl);
	OutputDebugString(m_slog);

	// 1. 완전 사용불가
	if (nFlag == DF_NUSE)
	{
		HandleScreenPopup(sPopType, sPopMap, sUrl);
		return DF_NUSE;
	}

	// 2. HTS 시작시 안띄움  32
	if (igubn == 1 && HasFlag(nFlag, DF_NUSE_HTSOPEN))
	{
		m_slog.Format("[ScreenCheck] HTS 시작시 DF_NUSE mapname=%s", mapname);
		OutputDebugString(m_slog);
		return DF_NUSE;
	}

	// 3. 특정시간 대체 팝업  64
	if (igubn != 1 && HasFlag(nFlag, DF_YUSE_GUIDEPOP))
	{
		if (IsGuidePopupTime())
		{
			m_mapHelper->CreatePopup(sPopMap, 1, WK_POPUP, 5);
			return DF_YUSE;
		}
	}

	// 4. 공지창을 하나 띄운다.
	if (HasFlag(nFlag, DF_YUSE_NOTICEPOP))  //2
	{
		m_slog.Format("[ScreenCheck] [2]DF_YUSE_NOTICEPOP  mapname=%s   sPopType[%s], sPopMap[%s], sUrl[%s]",
			mapname, sPopType, sPopMap, sUrl);
		OutputDebugString(m_slog);

		HandleScreenPopup(sPopType, sPopMap, sUrl, _ttoi(sUrl));
		return DF_YUSE;
	}

	// 5. 특정날짜 이후 사용중지
	if (HasFlag(nFlag, DF_YUSE_AFTERDAY))  //4
	{
		if (!sDate.IsEmpty())
		{
			CString today = GetTodayYmd();

			m_slog.Format("[ScreenCheck] [4]DF_YUSE_AFTERDAY mapname=%s today=%s sDate=%s 날짜이후 사용중지 sPopType[%s], sPopMap[%s], sUrl[%s]",
				mapname, today, sDate, sPopType, sPopMap, sUrl);
			OutputDebugString(m_slog);

			if (today >= sDate)
			{
				HandleScreenPopup(sPopType, sPopMap, sUrl);
				return DF_NUSE;
			}
		}
	}

	// 6. 관리자 전용
	if (HasFlag(nFlag, DF_YUSE_MANAGER))
	{
		if (!IsManagerUser())
		{
			m_slog.Format("[ScreenCheck] DF_YUSE_MANAGER blocked mapname=%s", mapname);
			OutputDebugString(m_slog);
			return DF_NUSE;
		}
	}

	// 7. 디버그 전용
	if (HasFlag(nFlag, DF_YUSE_DEBUG))
	{
#ifndef _DEBUG
		if (!IsDebugUser())
		{
			m_slog.Format("[ScreenCheck] DF_YUSE_DEBUG blocked mapname=%s", mapname);
			OutputDebugString(m_slog);
			return DF_NUSE;
		}
#endif
	}

	return DF_YUSE;
}

int CMainFrame::GetMacAddr(char* ipaddr, char* data)
{
	int ilen = 0;
	CString file;
	file.Format("%s\\exe\\CX_PCIdentity.dll", Axis::home);

	typedef char*(*GETMACADD)();
	HMODULE hModule = LoadLibrary(file);
	if (hModule)
	{
		GETMACADD func = (GETMACADD)GetProcAddress(hModule, "axGetMac");
		if (func)
		{
			char* pdata = func();
			CString smac;
			if (pdata)
			{
				smac.Format("%s", pdata);
				memcpy(data, pdata, strlen(pdata));
				ilen = smac.GetLength();
			}
		}
		m_slog.Format("mac add = %s", data);
		//WriteLog(m_slog);
		FreeLibrary(hModule);
	}
	return ilen;
}

void CMainFrame::SetEncriptFile()
{
	CProfile profile(pkAxis);
	CString value(profile.GetString(szSecure, "file"));
	if (value.IsEmpty())
	{
		value.Format("dev/dt_crypt.dll");
		profile.Write(szSecure, "file", value);
	}
}

typedef void (CALLBACK* LPFNDestory)();

void CMainFrame::OnDestroy() 
{
	m_iAxisState = AXIS_STATE_ONDESTORY;
	m_slog.Format("--------------------[Axis_State][OnDestroy start]  m_iAxisState =[%d]-------------", m_iAxisState);
	OutputDebugString(m_slog);
	Axis::sFiller = "destroying";
	CFrameWnd::OnDestroy();
	Axis::sFiller = "";
// 	if (m_hMRadar)
// 	{
// 		LPFNDestory ldes = (LPFNDestory)GetProcAddress(m_hMRadar, "AllDestory");
// 		if(ldes) 
// 			ldes();
// 		FreeLibrary(m_hMRadar);		
// 	}

	OutputDebugString("COMPLETE MAIN DESTROY\n");

 	if (m_hMNews) FreeLibrary(m_hMNews);

	m_slog.Format("--------------------[Axis_State][OnDestroy end]  m_iAxisState =[%d]-------------", m_iAxisState);
	OutputDebugString(m_slog);
	//FreeFirewall();
}

void CMainFrame::MRFrame(CChildFrame* child)
{
	// 2006.10.26 실시간지수 화면의 독립실행버튼과 복사버튼 삭제를 위하여
	if (child->m_mapN.CompareNoCase("DH150700") == 0)
	{  
		child->HideButton(HIDE_SINGLE|HIDE_GROUP|HIDE_COPY|HIDE_FONT|HIDE_HELP);
	}
	else if (child->m_mapN.CompareNoCase("DH205700") == 0)
	{
		child->HideButton(HIDE_COPY|HIDE_FONT);
	}
}

void CMainFrame::MngInfoPos()
{
	if (m_mngInfo)
	{
		
		int	x{}, y{};
		CRect	mRc;
		int	width = 0, height = 0;

		mRc = GetWndRect();
		m_mngInfo->GetSize(width, height);
		/*
		CString tmp; tmp.Format("%d", m_nInfoPos);
		AfxMessageBox(tmp);
		*/
		switch (m_nInfoPos)
		{
		case 0:	// left - top
			x = mRc.left;	y = mRc.top;
			if (x < 0)	x = 0;
			if (y < 0)	y = 0;
			break;
		case 1:	// center
			x = mRc.CenterPoint().x - width/2;
			y = mRc.CenterPoint().y - height/2;
			break;
		
		case 3:	// left - bottom
			x = mRc.left;
			y = mRc.bottom - height - GetSystemMetrics(SM_CYFIXEDFRAME);
			if (x < 0)	x = 0;
			if (y < 0)	y = 0;
			/*
			if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
				y = GetSystemMetrics(SM_CYSCREEN) - height;
			*/
			break;			
		case 4:	// right - bottom
			x = mRc.right - width - GetSystemMetrics(SM_CXFIXEDFRAME);
			y = mRc.bottom - height - GetSystemMetrics(SM_CYFIXEDFRAME);
			if (x < 0)	x = 0;
			if (y < 0)	y = 0;
			/*
			if (mRc.right > GetSystemMetrics(SM_CXSCREEN))
				x = GetSystemMetrics(SM_CXSCREEN) - width;
			if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
				y = GetSystemMetrics(SM_CYSCREEN) - height;
			*/
			break;
		case 2:	// right - top
		default:
			x = mRc.right - width - GetSystemMetrics(SM_CXFIXEDFRAME);
			y = mRc.top;
			if (x < 0)	x = 0;
			if (y < 0)	y = 0;
			
			/*
			if (mRc.right > GetSystemMetrics(SM_CXSCREEN))
				x = GetSystemMetrics(SM_CXSCREEN) - width;
			*/
			break;
		}
		
		CRect	rect;
		rect = mRc;
		rect.left = x;
		rect.top = y;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
		m_mngInfo->ShowSlide(rect);
	}
}

/////////////////////////////////////////////////////////////////////////////////
	// 개장시간에 맞추어 장애신고를 보낼수 있도록 영업을 저장 2006.12.06


void CMainFrame::sendRTime()
{
	CString	str, gdatS, inputStr = _T(""), gData;

	str.Format("1301%c005930\t", 0x7f);
	gdatS.Format("?%s%c", "2500", 0x7F);

	_dataH	dataH;

	memset(&dataH, 0x20, sizeof(dataH));
	memcpy(dataH.count, "000001", 6);
	dataH.dkey = 0x39;
	dataH.dunit = 0x01;
	dataH.dindex = 0x05;
	memcpy(dataH.lgap, "0001", 4);
	dataH.option1 = 0x21;

	gData.Format("2302%c%c", 0x0A, 0x09);

	inputStr.Format("%s%s%.*s%s", str, gdatS, sizeof(dataH), (char*)&dataH, gData);
	sendTR("GOOPPOOP", (char*)(const char*)inputStr, inputStr.GetLength(), US_OOP, 'm');
}

void CMainFrame::processRTime(char* ptr, int len)
{
	int	nSize = sizeof(struct _dataH);
	const struct _dataH*	dataH = NULL;
	if (len <= nSize)
	{
		m_szRTime = _T("");
		return;
	}

	ptr += nSize;
	CString	str;
	str.Format("%s", ptr);
	if (str.GetLength() < 8)
		return;
	m_szRTime = str.Left(8);
}

BOOL CMainFrame::IsRunTime()
{
	if (m_szRTime.IsEmpty())
		return FALSE;
	CTime	time;
	time = time.GetCurrentTime();

	CString	szDate, szTime;
	szDate.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	if (m_szRTime.CompareNoCase(szDate) != 0)
		return FALSE;

	szTime.Format("%02d%02d%02d", time.GetHour(), time.GetMinute(), time.GetSecond());
	if (90000 > _ttoi(szTime) || _ttoi(szTime) > 170000)
		return FALSE;

	return TRUE;
}

void CMainFrame::AddMdi(int vsn, int key, CChildFrame* pChild)
{
// 	CString s;
// 	s.Format("MAP NAME : %s	GROUP : %d\n",pChild->m_mapN,pChild->m_xcaption.GetGroup());
// 	OutputDebugString(s);
// 	m_groupBymapN.SetAt(pChild->m_mapN,CString(pChild->m_xcaption.GetGroup()));

	m_arMDI[vsn].SetAt(key, pChild);
	RedrawVS();
}

void CMainFrame::RemoveMdi(int vsn, int key)
{
	CChildFrame* child = NULL;
	POSITION pos{};
	int nTmpKey{};
	bool flag = FALSE;

 	if(m_arMDI[vsn].Lookup(key,child))
	{
		CChildFrame* pChild{};
		for (pos = m_arMDI[vsn].GetStartPosition(); pos; )
		{
			m_arMDI[vsn].GetNextAssoc(pos, nTmpKey, pChild);

			if(nTmpKey != key && pChild->m_mapN == child->m_mapN)
			{
				flag = TRUE;
			}
		}
		
		if(flag == FALSE)
			m_groupBymapN.RemoveKey(child->m_mapN);
	}

	m_arMDI[vsn].RemoveKey(key);	
	
	RedrawVS(true);

	if (!m_bLastClose)
		writeOpenedList();	// 20070627 kwon
}

void CMainFrame::AddSdi(int vsn, int key, CSChild* pChild)
{
	//m_groupBymapN.SetAt(pChild->m_mapN,CString(pChild->m_xcaption.GetGroup()));
	CString s;
	s.Format("MAP NAME SDI : [%s] GROUP : [%d]    key=[%d]\n", pChild->m_mapN, pChild->m_xcaption.GetGroup(), key);
	OutputDebugString(s);

	m_arSDI[vsn].SetAt(key, pChild);
	RedrawVS();
}

void CMainFrame::RemoveSdi(int vsn, int key)
{
	CSChild* child = NULL;
	POSITION pos{};
	int nTmpKey{};
	bool flag = FALSE;
	
	if(m_arSDI[vsn].Lookup(key,child))
	{
		CSChild* pChild{};
		for (pos = m_arMDI[vsn].GetStartPosition(); pos; )
		{
			m_arSDI[vsn].GetNextAssoc(pos, nTmpKey, pChild);
			
			if(nTmpKey != key && pChild->m_mapN == child->m_mapN)
			{
				flag = TRUE;
			}
		}
		
		if(flag == FALSE)
			m_groupBymapN.RemoveKey(child->m_mapN);
	}

	m_arSDI[vsn].RemoveKey(key);	
	RedrawVS(true);

	if (!m_bLastClose)
		writeOpenedList();	// 20070627 kwon
}

void CMainFrame::RemoveAllChild(int vsn, bool bSdi)
{
	if (bSdi)
		m_arSDI[vsn].RemoveAll();
	else
		m_arMDI[vsn].RemoveAll();

	m_groupBymapN.RemoveAll();
	
	RedrawVS(true);
}

CString CMainFrame::GetScreenList(int vsn)
{
	CString	ret = "";
	int	key = 0;
	CSChild*	schild = NULL;
	CChildFrame*	pchild = NULL;
	POSITION	pos = m_arSDI[vsn].GetStartPosition();
	while (pos)
	{
		ret += "\n";
		m_arSDI[vsn].GetNextAssoc(pos, key, schild);
		ret += schild->GetTitle();
	}

	pos = m_arMDI[vsn].GetStartPosition();
	while (pos)
	{
		//ret += "\n";
		m_arMDI[vsn].GetNextAssoc(pos, key, pchild);

		if(pchild->m_mapN == "IB0000X8")
		{
			continue;
		}
		ret += "\n";

		ret += pchild->GetTitle();
	}

	return ret;
}

// cej
void CMainFrame::DeleteAllScreen(bool all /*= true*/)
{
	int		key{};
	CSChild* schild{};
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		for (POSITION pos = m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			schild->SendMessage(WM_CLOSE);
		}
		RemoveAllChild(vsN, true);
	}
	m_mapHelper->closeAll(false);
}

// void CMainFrame::ShowDoctorInfo(DWORD* data)
// {
// 	//	TRACE("척척박사[%s]\n", dat);
// 	CString dat;
// 	
// 	if (!m_pDoctor)		return;
// 	if (m_bCustomer)	return;		// 고객은 popup안나옴
// 	
// 	// popup 0:보임, 1:안보임
// 	
// 	CProfile profile(pkEnvironment);
// 	
// 	if (profile.GetInt("DOCTORQNA", "CHKPOPUP", 0) != 0)	return;	// 0:보임
// 	
// 	CString symS, tmpS;
// 	
// 	int	idx = 0;
// 	
// 	
// 	m_pDoctor->StopSlide();
// 	//#define	SYM_NUM			14			// 일련번호
// 	//#define	SYM_DATE		48			// 등록일
// 	//#define	SYM_TIME		44			// 등록시간
// 	//#define	SYM_ID			13			// ID
// 	//#define	SYM_TITLE		15			// 제목
// 	//#define	SYM_RES			46			// 답변건수
// 	//#define	SYM_SEQ			47			// 나만보기
// 	
// 	CString idS, titleS, resS;
// 	if (!data[46])		return;
// 	resS = (char*)data[46];
// 	if (!data[13])		return;
// 	idS = (char*)data[13];
// 	if (!data[15])	return;
// 	titleS = (char*)data[15];
// 	if (atoi(resS) > 0)			return;			// 질문내용만 tooltip 
// 	int len = min(titleS.GetLength(), 30);
// 	dat.Format("%s:%s", idS, titleS.Left(len));
// 	
// 	m_pDoctor->SetData(dat);
// 	
// 	CRect		mRc;
// 	WINDOWPLACEMENT	pl;
// 	GetWindowPlacement(&pl);
// 	switch (pl.showCmd)
// 	{
// 	case SW_HIDE:case SW_MINIMIZE:case SW_SHOWMINIMIZED:
// 		m_pDoctor->ShowWindow(SW_HIDE);
// 		return;
// 	default:
// 		break;
// 	}
// 	m_pDoctor->ShowWindow(SW_HIDE);
// 	doctorInfoPos(true);
// 	SetTimer(TM_AUTODCOTOR, 3000, NULL);
// }

// void CMainFrame::ShowDoctorInfo(CString dat)
// {
// //	TRACE("척척박사[%s]\n", dat);
// 
// 	if (!m_pDoctor)		return;
// 	if (m_bCustomer)	return;		// 고객은 popup안나옴
// 
// 	// popup 0:보임, 1:안보임
// 
// 	CProfile profile(pkEnvironment);
// 
// 	if (profile.GetInt("DOCTORQNA", "CHKPOPUP", 0) != 0)	return;	// 0:보임
// 
// 	CString symS, tmpS;
// 	CString dataS = dat;
// 
// 	CMap <int, int, CString, LPCTSTR>	rtmStore;
// 	int	idx = 0;
// 
// 	rtmStore.RemoveAll();
// 	for (; !dataS.IsEmpty(); )
// 	{
// 		idx = dataS.Find('\t');
// 		if (idx < 0)
// 			break;
// 		symS = dataS.Left(idx);
// 		
// 		dataS = dataS.Mid(idx+1);
// 		idx = dataS.Find('\t');
// 		if (idx < 0)
// 			idx = dataS.GetLength();
// 		tmpS = dataS.Left(idx);
// 		rtmStore.SetAt(atoi(symS), tmpS);
// 
// 		if (dataS.GetLength() <= idx)
// 			break;
// 		dataS = dataS.Mid(idx+1);
// 	}
// 	
// 
// 	m_pDoctor->StopSlide();
// 	//#define	SYM_NUM			14			// 일련번호
// 	//#define	SYM_DATE		48			// 등록일
// 	//#define	SYM_TIME		44			// 등록시간
// 	//#define	SYM_ID			13			// ID
// 	//#define	SYM_TITLE		15			// 제목
// 	//#define	SYM_RES			46			// 답변건수
// 	//#define	SYM_SEQ			47			// 나만보기
// 
// 	CString idS, titleS, resS;
// 	if (!rtmStore.Lookup(46, resS))		return;
// 	if (!rtmStore.Lookup(13, idS))		return;
// 	if (!rtmStore.Lookup(15, titleS))	return;
// 	if (atoi(resS) > 0)			return;			// 질문내용만 tooltip 
// 	int len = min(titleS.GetLength(), 30);
// 	dat.Format("%s:%s", idS, titleS.Left(len));
// 
// 	m_pDoctor->SetData(dat);
// 
// 	rtmStore.RemoveAll();
// 
// 	CRect		mRc;
// 	WINDOWPLACEMENT	pl;
// 	GetWindowPlacement(&pl);
// 	switch (pl.showCmd)
// 	{
// 	case SW_HIDE:case SW_MINIMIZE:case SW_SHOWMINIMIZED:
// 		m_pDoctor->ShowWindow(SW_HIDE);
// 		return;
// 	default:
// 		break;
// 	}
// 	m_pDoctor->ShowWindow(SW_HIDE);
// 	doctorInfoPos(true);
// 	SetTimer(TM_AUTODCOTOR, 3000, NULL);
// }


// void CMainFrame::doctorInfoPos(bool bDoctor)
// {
// 	if (!m_pDoctor)	return;
// 	
// 	int	x, y;
// 	CRect	mRc;
// 	int	width = 0, height = 0;
// 
// 	mRc = GetWndRect();
// 	m_pDoctor->GetSize(width, height);
// 
// 	x = mRc.right - width - GetSystemMetrics(SM_CXFIXEDFRAME);
// 	y = mRc.top;
// 	if (x < 0)	x = 0;
// 	if (y < 0)	y = 0;
// 	if (mRc.right > GetSystemMetrics(SM_CXSCREEN))
// 		x = GetSystemMetrics(SM_CXSCREEN) - width;
// 	
// 	m_pDoctor->SetGubn(bDoctor); 
// 	
// 	CRect	rect;
// 	rect = mRc;
// 	rect.left = x;
// 	rect.top = y;
// 	rect.right = rect.left + width;
// 	rect.bottom = rect.top + height;
// 	m_pDoctor->ShowSlide(rect);
// 
// }

#pragma warning (disable : 26409)
#pragma warning (disable : 26400)

void CMainFrame::showMisuAlarm()
{
	if (!m_misuAlarm)
	{
		m_misuAlarm = new CDlgMisuAlarm(this);
		if (!m_misuAlarm->Create(IDD_DLGMISU))
		{
			delete m_misuAlarm;
			m_misuAlarm = NULL;
		}
	}
	
	if (m_misuAlarm)
	{
		m_misuAlarm->Init();
		m_misuAlarm->ShowWindow(SW_SHOW);
	}

}

#pragma warning (default : 26409)
#pragma warning (default : 26400)

void CMainFrame::RunBrowser(CString url)
{
	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOWNORMAL);
}

void CMainFrame::checkRTSQueue()
{
	if (!m_wizard || !IsWindow(m_wizard->GetSafeHwnd()))
		return;
	CString tmpS;
	long	rc = 0;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
						(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(getRTS, 0), 0);
	const int cnt = (int)rc;

	const DWORD processID = GetCurrentProcessId();
	const int cpuuse = m_cpu.GetCpuUsage(processID);//m_cpu.GetCpuUsage();

	if (m_bDelayIn && m_nDelayPeak < cnt)
		m_nDelayPeak = cnt;

	if (m_bDelayIn && m_nCpuPeak < cpuuse)
		m_nCpuPeak = cpuuse;

	if (!m_bDelayIn && cnt > 100)
	{
		m_tmDelayStart = CTime::GetCurrentTime();
		m_sDelayData.Format("%s\t%d ~ ", m_tmDelayStart.Format("%H:%M:%S"), cnt);
		m_bDelayIn = !m_bDelayIn;
		m_nDelayPeak = cnt;
		m_nCpuPeak = cpuuse;
	}

	if (m_bDelayIn && cnt < 100)
	{
		CTimeSpan timeDelta;
		CString sPath;
		m_tmDelayEnd = CTime::GetCurrentTime();
		sPath.Format("%s\\%s\\%s\\delayinfo", Axis::home, USRDIR, Axis::user);
		tmpS.Format("%s%s\t%d,P=%d,C=%d<br>", m_sDelayData, m_tmDelayEnd.Format("%H:%M:%S"), cnt, m_nDelayPeak, m_nCpuPeak);
		timeDelta = m_tmDelayEnd - m_tmDelayStart;
		if (timeDelta.GetTotalSeconds() > 2 || m_nDelayPeak > 300)
			WriteFileData(sPath, tmpS);
		m_bDelayIn = !m_bDelayIn;
	}

	CTime curtime;
	curtime = CTime::GetCurrentTime();

	CString strFile;
	strFile.Format("%s\\tab\\RSCMONITOR.ini", Axis::home); 

	const int nStart = GetPrivateProfileInt("RSCMONITOR","STIME",90000,strFile);
	const int nEnd = GetPrivateProfileInt("RSCMONITOR","ETIME",160000,strFile);

	if(atoi(curtime.Format("%H%M%S")) > nStart)
	{
		CString sRscInfo;
		sRscInfo.Format("%s\\%s\\%s\\rscinfo", Axis::home, USRDIR, Axis::user);

		if(_taccess(sRscInfo,4))
		{
			CString tmp;
			tmp.Format("ID [%s] 2022 COUNT [%d]\n",Axis::userID,m_n2022Cnt);
			WriteFileData(sRscInfo,tmp);
		}

		tmpS.Format("%s,%d,%d",curtime.Format("%H:%M:%S"),cnt,cpuuse);
		WriteFileData(sRscInfo,tmpS);
	}

	if(atoi(curtime.Format("%H%M%S")) > nEnd)
	{
		KillTimer(TM_RTSQUEUE);
	}

// 	if (Axis::devMode)
// 	{
// 		m_titleS.Format("CPU[%d%%] 지연건수[%d개]", cpuuse, cnt);
// 		DrawFrame();
// 		return;
// 	}
	
// 	if (cnt > 500 || cpuuse > 90)	// 지연건수, CPU사용율 높을때만 title에 표시
// 	{
// 		m_bRTSQueue = true;
// 		tmpS.Format("%s    CPU[%d%%] 지연건수[%d개]", m_saveTitle, cpuuse, cnt);
// 		m_titleS = tmpS;
// 		DrawFrame();
// 	}
// 	else
// 	{
// 		if (m_bRTSQueue)
// 		{
// 			m_bRTSQueue = false;
// 			m_titleS = m_saveTitle;
// 			DrawFrame();
// 		}
// 	}
}

//===================================================================================
void CMainFrame::checkOpenedList()
{
	CString	Path;
	Path.Format("%s\\%s\\%s\\dinfo", Axis::home, USRDIR, Axis::user);

	CProfile profile(pkUserSetup);
	CString value;
	value = profile.GetString(szLogCheck, "CLOSE");
	profile.Write(szLogCheck, "CLOSE", "0");

	((CAxisApp*)m_axis)->m_logPath = profile.GetFileName();

	if (_ttoi(value) != 1)
	{
		TRY
		{
			CFile file;
			if (file.Open(Path, CFile::modeRead)) 
			{
				const int len = (int)file.GetLength();
				if (len > 0 && len < 2000)	// 정상적인 로그면 2000 이하일것
				{
#ifdef DF_USE_CPLUS17
					std::unique_ptr<char[]> pBuf = std::make_unique<char[]>(len);
					file.Read(pBuf.get(), len);
					const bool bret = sendTR("pihoerpt", pBuf.get(), len, US_PASS, 'd');
#else
					char* pBuf = new char[len];
					file.Read(pBuf, len);
					const bool bret = sendTR("pihoerpt", pBuf, len, US_PASS, 'd');

					delete[] pBuf;
#endif
				
				}
				file.Close();
			}
		}
		CATCH (CFileException, e) 
		{
		}
		END_CATCH;
	}

	m_bFirstOpen = false;
	writeOpenedList(true);
	//SetTimer(TM_SCRLOG, 120000, NULL);	화면 생성될때랑 종료될때 남기기때문에 일단 주석처리
}

struct _errmid {
	char	usid[24];		// user id
	char	optn[4];		// 0 - 회원사구분( 'H' - hana, 'D' - daetoo), 1 - ( 1 - 직원, 2- 고객), 2~3 : filler 
	char	yymd[8];		// YYYYMMDD
	char	hmss[6];		// HHMMSS
	char	mapn[8];		// map name
	char	ipad[15];		// local ip-addr
	char	fill[80];		// filler
	char	info[512];		// error information
	char	cont[1];		// user situation
};
#define	sz_errmid	sizeof(_errmid)

void CMainFrame::writeOpenedList(bool bFirst /*= false*/)
{
	if (m_bFirstOpen)	return;

	CString	mpN, tmps, info;

	CArray	<int, int>	aryK;

	CString		sMap = m_sLastMapInfo;
	int		cnt = 0;
	
	sMap.Replace(MAPN_REALTIMEJANGO, "");
	sMap.Replace("IBXXXX00", "");
	sMap.Replace(MAPN_SISECATCH1, "");
	
	CString	Path, sData;
	Path.Format("%s\\%s\\%s\\dinfo", Axis::home, USRDIR, Axis::user);
	const CTime curr = CTime::GetCurrentTime();

	if (m_sWin.IsEmpty())
	{
		m_sWin = m_sysInfo->GetWindowInfo();
		m_sCpu = m_sysInfo->GetCPUInfo();	m_sCpu.TrimLeft();
		m_sMem = m_sysInfo->GetMemoryInfo();
		m_sIE  = m_sysInfo->GetIEInfo();
	}

#ifdef DF_USE_CPLUS17
	const int	dLen = sz_errmid;
	std::unique_ptr<char[]>pBuf = std::make_unique<char[]>(dLen);
	memset(pBuf.get(), ' ', dLen);
	_errmid* errmid = (_errmid*)pBuf.get();
	const int	infoL = sizeof(errmid->info);		// 512
	cnt = sMap.GetLength() / 9;

	sData.Format("%s\t%s\t%s\tIEV:%s\t[%d개:%s] %s ", m_sWin, m_sCpu, m_sMem, m_sIE, cnt, (bFirst ? "F" : "C"), sMap/*, getProcessList()*/);

	if (sData.GetLength() > infoL)
		sData = sData.Left(infoL - 1);

	sprintf(errmid->usid, Axis::userID);
	strcpy_s(errmid->optn, sizeof(errmid->optn), "110");
	sprintf(errmid->yymd, "%04d%02d%02d", curr.GetYear(), curr.GetMonth(), curr.GetDay());
	sprintf(errmid->hmss, "%02d%02d%02d", curr.GetHour(), curr.GetMinute(), curr.GetSecond());

	if (m_ipAddr.IsEmpty())
	{
		::gethostname(errmid->ipad, sizeof(errmid->ipad));
		if (lstrcmp(errmid->ipad, "") != 0)
		{
			const HOSTENT FAR* lphostent = ::gethostbyname(errmid->ipad);
			if (lphostent)
			{
				sprintf(errmid->ipad, "%s", inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
			}
		}
		m_ipAddr.Format("%s", errmid->ipad);
	}
	const int ipadL = min(sizeof(errmid->ipad), m_ipAddr.GetLength());
	sprintf(errmid->ipad, "%s", (LPSTR)(LPCTSTR)m_ipAddr.Left(ipadL));
	sprintf(errmid->info, sData);

	TRY
	{
		CFile	file;
		if (!file.Open(Path, CFile::modeCreate | CFile::modeWrite))
		{
			TRACE("Create Failed\n");
			return;
		}
		file.Write(pBuf.get(), dLen);
		file.Close();
	}
		CATCH(CFileException, e)
	{
	}
	END_CATCH;
#else
	const int	dLen = sz_errmid;
	char* pBuf = new char[dLen];
	memset(pBuf, ' ', dLen);
	_errmid* errmid = (_errmid*)pBuf;
	const int	infoL = sizeof(errmid->info);		// 512
	cnt = sMap.GetLength() / 9;

	sData.Format("%s\t%s\t%s\tIEV:%s\t[%d개:%s] %s ", m_sWin, m_sCpu, m_sMem, m_sIE, cnt, (bFirst ? "F" : "C"), sMap/*, getProcessList()*/);

	if (sData.GetLength() > infoL)
		sData = sData.Left(infoL - 1);

	sprintf(errmid->usid, Axis::userID);
	strcpy_s(errmid->optn, sizeof(errmid->optn), "110");
	sprintf(errmid->yymd, "%04d%02d%02d", curr.GetYear(), curr.GetMonth(), curr.GetDay());
	sprintf(errmid->hmss, "%02d%02d%02d", curr.GetHour(), curr.GetMinute(), curr.GetSecond());

	if (m_ipAddr.IsEmpty())
	{
		::gethostname(errmid->ipad, sizeof(errmid->ipad));
		if (lstrcmp(errmid->ipad, "") != 0)
		{
			const HOSTENT FAR* lphostent = ::gethostbyname(errmid->ipad);
			if (lphostent)
			{
				sprintf(errmid->ipad, "%s", inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
			}
		}
		m_ipAddr.Format("%s", errmid->ipad);
	}
	const int ipadL = min(sizeof(errmid->ipad), m_ipAddr.GetLength());
	sprintf(errmid->ipad, "%s", (LPSTR)(LPCTSTR)m_ipAddr.Left(ipadL));
	sprintf(errmid->info, sData);

	TRY
	{
		CFile	file;
		if (!file.Open(Path, CFile::modeCreate | CFile::modeWrite))
		{
			TRACE("Create Failed\n");
			delete[] pBuf;
			return;
		}
		file.Write(pBuf, dLen);
		file.Close();
	}
		CATCH(CFileException, e)
	{
	}
	END_CATCH;

	delete[] pBuf;
#endif
}

void CMainFrame::removeOpenedList()
{
	CProfile profile(pkUserSetup);

	profile.Write("LOGCHECK", "CLOSE", "1"); // 정상종료 1


	CString	Path;
	Path.Format("%s\\%s\\%s\\dinfo", Axis::home, USRDIR, Axis::user);
	TRY 
	{
		CFile::Remove(Path);
	}
	CATCH(CFileException, e) 
	{
	}
	END_CATCH;
}

CString CMainFrame::getProcessList()
{
	if (m_winVer == WIN9X || m_winVer == UNKNOWN)
		return _T("");

	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32	pe32 = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	if (hProcessSnap == INVALID_HANDLE_VALUE) 
		return _T(""); 

	CStringArray arProcess;
	arProcess.RemoveAll();

	int pos = 0;
	CString proS;
	pe32.dwSize = sizeof(PROCESSENTRY32); 
	if (Process32First(hProcessSnap, &pe32)) 
	{ 
		DWORD		dwPriorityClass{};
		const BOOL		bGotModule = FALSE;
		MODULEENTRY32	me32 = {0}; 
		const DWORD		Code = 0;
		const int		cnt = 0;
		do 
		{ 
			HANDLE hProcess; 
			hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); 
			dwPriorityClass = GetPriorityClass (hProcess); 

			proS = CString(pe32.szExeFile);
			proS.MakeLower();
			if ((pos = proS.Find(".exe")) != -1)
				proS = proS.Left(pos);
			arProcess.Add(proS);
			if (proS != "mfnt")
				CloseHandle(hProcess);
		} while (Process32Next(hProcessSnap, &pe32)); 
	}
	CloseHandle (hProcessSnap); 

	// 킹스 : kdrmgr
	// nProtect(잉카), np로 시작 : npkcmsvc, npmon, npkagt
	// 안철수 키보드 : mkd20tray
	//       V3 2007 : AcAis.exe,acaegmgr.exe,acaas.exe,acals.exe, msProxy
	//       V3 : AhnSd, AhnSDsv, monsvcnt, monsysnt
	//       방화벽 : mfnt
	// 이니텍 : INISafeWeb
	CString tmpS, allS = _T("");
	for (int ii = 0, idx = 0; ii < arProcess.GetSize(); ii++)
	{
		tmpS.Format("%s  ", arProcess.GetAt(ii)); 
		if (tmpS.Find("conime") != -1			// 윈도우콘솔 관장, 쓰레드를 생성삭제,16비트 가상DOS모드를 지원
			|| tmpS.Find("csrss") != -1		// 명령프롬프트상의 한글입력이나 그외 프롬프트를 사용하는 프로그램을 지원
			|| tmpS.Find("ctfmon") != -1		// 바탕 화면의 입력 도구의 실행 파일로서 문자 입력을 수행하는 텍스트 서비스
			|| tmpS.Find("explore") != -1 
			|| tmpS.Find("jusched") != -1		// 자바업데이트 스케줄러
			|| tmpS.Find("lsass") != -1		// Winlogon 서비스에 필요한 인증 프로세스를 담당
			|| tmpS.Find("mdm") != -1		// Machine Debug Manager
			|| tmpS.Find("mstask") != -1		// 작업 스케쥴러 서비스
			|| tmpS.Find("process") != -1 
			|| tmpS.Find("rundll32") != -1		// 32bit 프로그램
			|| tmpS.Find("services") != -1		// 시스템 서비스들을 시작/정지시키고, 그들간의 상호작용하는 기능
			|| tmpS.Find("smss") != -1		// 사용자 세션을 시작하는 기능
			|| tmpS.Find("spoolsv") != -1		// 프린터 및 팩스의 Spooling 기능
			|| tmpS.Find("svchost") != -1		// DLL로부터 실행되는 다른 프로세스들의 host 역할
			|| tmpS.Find("system") != -1		// 대부분의 커널모드 쓰레드들의 시작점이 되는 프로세스
			|| tmpS.Find("taskmgr") != -1		// 작업관리자
			|| tmpS.Find("usnsvc") != -1		// Messenger Sharing USN Journal Reader Service 
			|| tmpS.Find("winmgmt") != -1		// 클라이언트 관리의 핵심
			|| tmpS.Find("winlogon") != -1		// 사용자 로그인/로그오프를 담당하는 프로세스
			|| tmpS.Find("wuauclt") != -1		// 윈도우 업데이트

			// vista
			|| tmpS.Find("wininit") != -1		// 윈도우 업데이트
			|| tmpS.Find("lsm") != -1		// 로컬 세션 관리자
			|| tmpS.Find("audiodg") != -1		// 오디오장치
			|| tmpS.Find("slsvc") != -1		// Microsoft Software Licensing Service 
			|| tmpS.Find("taskeng") != -1		// 작업 스케줄러의 엔진 실행
			|| tmpS.Find("dwm") != -1		// 바탕 화면 창 관리자
			|| tmpS.Find("sidebar") != -1		// side bar
			)
			continue;
		allS += tmpS;
		idx++;
	}
	arProcess.RemoveAll();
	return allS;
}

void CMainFrame::show_Ticker(int nID)
{
	switch (nID)
	{
	case 1:		// info1
		{
#ifdef DF_USE_CPLUS17
		ShowControlBar(m_tInfo1.get(), FALSE, FALSE);
#else
			ShowControlBar(m_tInfo1, FALSE, FALSE);
#endif
		}
		saveToolStatus();
		SetSDIChangeHeight();
		break;
	case 2:
		{
#ifdef DF_USE_CPLUS17
			ShowControlBar(m_tInfo2.get(), FALSE, FALSE);
#else
			ShowControlBar(m_tInfo2, FALSE, FALSE);
#endif
		}
		saveToolStatus();
		SetSDIChangeHeight();
		break;
	case 3:		// info3
#ifdef DF_USE_CPLUS17
		if (!(m_tInfo2 && m_tInfo2->GetStyle() & WS_VISIBLE))
			ShowControlBar(m_tInfo2.get(), TRUE, FALSE);
		else if (!(m_tInfo1 && m_tInfo1->GetStyle() & WS_VISIBLE))
			ShowControlBar(m_tInfo1.get(), TRUE, FALSE);
		else
		{
			ShowControlBar(m_tInfo1.get(), FALSE, FALSE);
			ShowControlBar(m_tInfo2.get(), FALSE, FALSE);
		}
#else
		if (!(m_tInfo2 && m_tInfo2->GetStyle() & WS_VISIBLE))
			ShowControlBar(m_tInfo2, TRUE, FALSE);
		else if (!(m_tInfo1 && m_tInfo1->GetStyle() & WS_VISIBLE))
			ShowControlBar(m_tInfo1, TRUE, FALSE);
		else
		{
			ShowControlBar(m_tInfo1, FALSE, FALSE);
			ShowControlBar(m_tInfo2, FALSE, FALSE);
		}
#endif		
		saveToolStatus();
		SetSDIChangeHeight();
		break;
	default:
		break;
	}
}

void CMainFrame::hide_Ticker(int nID)
{
	switch (nID)
	{
		case 5:
		{
	#ifdef DF_USE_CPLUS17
			ShowControlBar(m_tInfo3.get(), FALSE, FALSE);
	#else
			ShowControlBar(m_tInfo3, FALSE, FALSE);
	#endif
			saveToolStatus();
		}
		break;
		default:
			break;
	}
}

CGPop*	CMainFrame::getGPOP( CString strMapName )
{
	POSITION pos = m_arGPOP.GetStartPosition();
	while( pos )
	{
		int key;
		CGPop* p{};
		m_arGPOP.GetNextAssoc( pos, key, p );		

		if( p->m_mapN == strMapName.Left(8) )
			return p;
	}

	return NULL;
}

CRect CMainFrame::GetFullRect()
{
	CRect rc;
	const int moniterN = GetSystemMetrics(SM_CMONITORS);

	if (moniterN <= 1)
	{
		rc.SetRect(0, 0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
		return rc;
	}

	g_monRc.SetRectEmpty();
	EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);
	rc = g_monRc;
	return rc;
}

BOOL CMainFrame::IsNewLoginImage(BOOL bStaff)
{
	CString	imgN;
	
	/**
	if (!bStaff)
	{
		imgN.Format("%s\\%s\\axis.bmp", Axis::home, IMAGEDIR);
	}
	else
	{
		imgN.Format("%s\\%s\\saxis.bmp", Axis::home, IMAGEDIR);
	}
	**/
	imgN.Format("%s\\%s\\LOGIN_BG_BAGIC.bmp", Axis::home, IMAGEDIR);

	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			imgN,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

	if (hBitmap)
	{
		BITMAP bm;
		GetObject(hBitmap, sizeof(BITMAP), &bm);
		::DeleteObject(hBitmap);
		if (!(bm.bmWidth == 370 && bm.bmHeight == 621))	// 신 이미지
			return TRUE;
	}
	return FALSE;
}

BOOL CMainFrame::IsNewExitImage()
{
	return TRUE;
	/**
	CString	path, path_;
	path.Format("%s\\%s\\exitdlg.bmp", Axis::home, IMAGEDIR);
	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

	if (hBitmap)
	{
		BITMAP bm;
		GetObject(hBitmap, sizeof(BITMAP), &bm);
		::DeleteObject(hBitmap);
		if (!(bm.bmWidth == 279 && bm.bmHeight == 525))	// 신 이미지
			return TRUE;
	}
	return FALSE;
	**/
}

void CMainFrame::VoidRect(CRect *pRect)
{
	if (pRect)
	{
		CRect fullRc = GetFullRect();
		CRect iRc;
		if (iRc.IntersectRect(fullRc, pRect))
		{
			CRect deltaRc;
			deltaRc.SubtractRect(iRc, *pRect);
			pRect->OffsetRect((iRc.right < pRect->right? (iRc.right - pRect->right):(iRc.left - pRect->left)),
				(iRc.bottom < pRect->bottom? (iRc.bottom - pRect->bottom):(iRc.top - pRect->top)));
		}
	}
}

void CMainFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (!m_bSDI)
	{
		
		lpncsp->rgrc[0].top    += SZ_MAINCAPTION - 2;  
		lpncsp->rgrc[0].bottom += 2 ;
		
		const int WinTheme = GetTheme();
		
		if(Axis::isVista && WinTheme == 1)
		{
			const int interFrame = GetSystemMetrics(SM_CXFRAME) - 4;
			lpncsp->rgrc[0].top +=interFrame - 2;
			lpncsp->rgrc[0].bottom +=interFrame;
			lpncsp->rgrc[0].left -=interFrame;
			lpncsp->rgrc[0].right +=interFrame;
			
		}
		else if(Axis::isVista && WinTheme == 0)
		{
			const int interFrame = GetSystemMetrics(SM_CXFRAME) - 4;
			lpncsp->rgrc[0].top -=interFrame;
			lpncsp->rgrc[0].bottom +=interFrame;
			lpncsp->rgrc[0].left -=interFrame;
			lpncsp->rgrc[0].right +=interFrame;
		}
		
	}
	else
	{
		lpncsp->rgrc[0].top -= 3;
		lpncsp->rgrc[0].bottom += 2;
	}
	if(Axis::isVista)	Invalidate(false);
	
	CMDIFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

LRESULT CMainFrame::OnNcHitTest(CPoint point) 
{
	if (m_bSDI)
		return CMDIFrameWnd::OnNcHitTest(point);
	UINT nHitTest = HTNOWHERE;

	CPoint tPnt = point;
	const CRect iconRc(4, 0, 20, 12);
	CRect captionRc;
	const CRect rightBottomRc;

	GetClientRect(&captionRc);
	captionRc.top = -(14+ 4);
	captionRc.bottom = 0;
	/*captionRc.left  -= 4;
	captionRc.right += 4;*/
	ScreenToClient(&tPnt);

	if (iconRc.PtInRect(CPoint(tPnt.x, tPnt.y + 12 + GetSystemMetrics(SM_CYFRAME))))
		nHitTest = HTSYSMENU;
	else if (captionRc.PtInRect(tPnt))
	{
		return HTCAPTION;
	}
	else
		nHitTest = CMDIFrameWnd::OnNcHitTest(point);

	return nHitTest;
	return CMDIFrameWnd::OnNcHitTest(point);
}

bool CMainFrame::IsAgreeDuplicate(CString mapN)
{
	if (mapN.IsEmpty())	return false;
	CString smapN, key = m_tMenu->GetDispN(mapN);

	CProfile profile(pkAxis);

	smapN = profile.GetString("Select"	, key);
	if (!smapN.Compare("980000")) // Web Link Map.
	{
		return !IsExistMap(mapN, m_vsN);
	}
	//AfxMessageBox(szAgreeDuplicate);
	smapN = profile.GetString(szAgreeDuplicate, key);
	return !smapN.IsEmpty();
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	long nMaxHeight = GetSystemMetrics(SM_CYMAXIMIZED);
	long nMaxWidth = GetSystemMetrics(SM_CXMAXIMIZED);
	CPoint pt;
	CRect rc;
	GetWindowRect(&rc);

	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);
	if (pl.showCmd == SW_SHOWMINIMIZED && m_minSaveRC.IsRectEmpty())
	{
		m_minSaveRC = pl.rcNormalPosition;
	}
	else
	{
		if (!m_minSaveRC.IsRectEmpty())
		{
			rc = m_minSaveRC;
			m_minSaveRC.SetRectEmpty();
		}
	}

	pt = rc.CenterPoint();
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
	if (GetMonitorInfo(hMonitor, &mi))
	{
		nMaxHeight = mi.rcWork.bottom - mi.rcWork.top;
		nMaxWidth = mi.rcWork.right - mi.rcWork.left;
	}

	lpMMI->ptMaxSize.y = nMaxHeight + GetSystemMetrics(SM_CYFRAME)-3;
	lpMMI->ptMaxSize.x = nMaxWidth + GetSystemMetrics(SM_CXFRAME);

	//20230522(BF) HTS Max size fixing 
	lpMMI->ptMaxSize.x += GetSystemMetrics(SM_CXFRAME) * 2;  
	lpMMI->ptMaxSize.y += GetSystemMetrics(SM_CYFRAME) * 2;
	CMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
}

#pragma warning( disable : 26409)
#pragma warning( disable : 26400)

void CMainFrame::FOStopLoss()
{
	const bool onlysise = m_axis->GetProfileInt(WORKSTATION, "OnlySise", 0) ? TRUE : FALSE;
	if (onlysise)	return;
	if (!m_foStoploss)
	{
		CStopWarn	dlg;
		if (dlg.DoModal() != IDOK)	return;
#ifdef DF_USE_CPLUS17
		m_foStoploss = new CStoploss(m_bar2.get());
#else
		m_foStoploss = new CStoploss(m_bar2);
#endif
		if (!m_foStoploss->Create(IDD_STOPLOSS))
		{
			delete m_foStoploss;
			m_foStoploss = NULL;
		}
		else
		{
			BOOL	rc = 0;
			int	vtype = 0, size = 0, ret{}, key = KEY_FOSTOPLOSS;
			CString mapname = MAPN_FOSTOPLOSS;

			m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
						(BYTE *)(VTS_BSTR VTS_I4), mapname, &size);
			m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
					(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)m_foStoploss, vtype, key);
			m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false);

			ret = 0;
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
				(BYTE *)(VTS_I4 VTS_I4), setGID, (long)MAKELONG(key, 0));

			CSize	frameGap;
			frameGap.cx  = GetSystemMetrics(SM_CXFRAME);
			frameGap.cx += GetSystemMetrics(SM_CXBORDER) * 2;

			frameGap.cy  = GetSystemMetrics(SM_CYFRAME);
			frameGap.cy += GetSystemMetrics(SM_CYBORDER) * 2;
			frameGap.cy += GetSystemMetrics(SM_CYCAPTION);

			const int nCx = frameGap.cx + LOWORD(size);
			const int nCy = frameGap.cy + HIWORD(size);

			m_foStoploss->SetWindowPos(&wndTopMost, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);
		}
	}

	CRect	mRc, wRc;

	mRc = GetWndRect();
	m_foStoploss->GetWindowRect(wRc);

	int x = mRc.right - wRc.Width() - GetSystemMetrics(SM_CXFIXEDFRAME);
	int y = mRc.bottom - wRc.Height() - GetSystemMetrics(SM_CYFIXEDFRAME);
	if (x < 0)	x = 0;
	if (y < 0)	y = 0;
	if (mRc.right > GetSystemMetrics(SM_CXSCREEN))
		x = GetSystemMetrics(SM_CXSCREEN) - wRc.Width();
	if (mRc.bottom > GetSystemMetrics(SM_CYSCREEN))
		y = GetSystemMetrics(SM_CYSCREEN) - wRc.Height();

	if (m_foStoploss->GetStyle() & WS_VISIBLE)
		m_foStoploss->ShowWindow(SW_HIDE);
	else	m_foStoploss->SetWindowPos(&wndTopMost, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
}

#pragma warning( default : 26409)
#pragma warning( default : 26400)

void CMainFrame::RelayoutTicker()
{
	/**
	if (m_tInfo3)
	{
		if ((m_tInfo2 && m_tInfo2->GetStyle() & WS_VISIBLE) && (m_tInfo1 && m_tInfo1->GetStyle() & WS_VISIBLE))
			m_tInfo3->ChangeMode(TICKERMODE_DOWN);
		else
			m_tInfo3->ChangeMode(TICKERMODE_UP);
	}
	**/
	if (m_tInfo3 && !(m_tInfo3->GetStyle() & WS_VISIBLE))
	{
		if (m_tInfo2 && m_tInfo2->GetStyle() & WS_VISIBLE)
		{
			m_tInfo2->SetBase(TRUE);
			m_tInfo1->SetBase(FALSE);
		}
		else
		{
			m_tInfo2->SetBase(FALSE);
			m_tInfo1->SetBase(TRUE);
		}
	}
	else
	{
		m_tInfo2->SetBase(FALSE);
		m_tInfo1->SetBase(FALSE);
	}
}


void CMainFrame::MDINextChild()
{
	int key{};
	if (m_hooklist[m_vsN].GetSize() > 1)
	{
		key = m_hooklist[m_vsN].GetAt(1);
		const int firstKey = m_hooklist[m_vsN].GetAt(0);
		m_hooklist[m_vsN].RemoveAt(0);
		m_hooklist[m_vsN].Add(firstKey);	// 처음 key를 last로
		m_hooklist[m_vsN].RemoveAt(0);
		m_hooklist[m_vsN].InsertAt(0, key);	// 두번째 key를 first로
	}

	CChildFrame*	child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		CSChild*	schild;
		if (!m_arSDI[m_vsN].Lookup(key, schild))
			return;
		MDIActivate(schild);
		return;
	}
	MDIActivate(child);
}

void CMainFrame::WriteFileData(CString sPath, CString sData, BOOL bCreate)
{
	TRY
	{
		CFile file;
		CString filename = sPath;
		CFileFind finder;
		if (!bCreate && finder.FindFile(filename))
		{
			if (!file.Open(filename, CFile::modeReadWrite|CFile::modeReadWrite|CFile::typeBinary))
			{
				TRACE("File Create Error!!");
				return;
			}
		}
		else
		{
			if (!file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
			{
				TRACE("File Create Error!!");
				return;
			}
		}
		if (!bCreate)
			finder.Close();
		file.SeekToEnd();
		file.Write((void*)(const char*)sData, sData.GetLength());
		file.SeekToEnd();
		file.Write("\r\n", 2);
		file.Close();
	}
	CATCH (CFileException, e) 
	{
	}
	END_CATCH;
}

void CMainFrame::checkDelayList()
{
#ifdef DF_USE_CPLUS17
	CFileFind finder;
	CString sPath;
	sPath.Format("%s\\%s\\%s\\delayinfo", Axis::home, USRDIR, Axis::user);
	if (finder.FindFile(sPath))
	{
		TRY
		{
			CFile file;
			CString sBuf;
			if (file.Open(sPath, CFile::modeRead))
			{
				const long len = (long)file.GetLength();
				file.Read(sBuf.GetBuffer(len + 1), len);
				sBuf.ReleaseBuffer();
				file.Close();
				::DeleteFile(sPath);
			}

			const CTime curr = CTime::GetCurrentTime();
			const int	dLen = sz_errmid;
		
			std::unique_ptr<char[]>pBuf = std::make_unique<char[]>(dLen);
			memset(pBuf.get(), ' ', dLen);
			_errmid* errmid = (_errmid*)pBuf.get();
			const int	infoL = sizeof(errmid->info);		// 512

			CString sData = "**Delay Mon**<br>";
			CString sMap = m_sLastMapInfo;
			sMap.Replace(MAPN_REALTIMEJANGO, "");
			sMap.Replace("IBXXXX00", "");
			sMap.Replace(MAPN_SISECATCH1, "");
			sMap.Replace("IB", "");
			if (m_sWin.IsEmpty())
			{
				m_sCpu = m_sysInfo->GetCPUInfo();	m_sCpu.TrimLeft();
				m_sMem = m_sysInfo->GetMemoryInfo();
			}

			sData += sMap + "<br>" + m_sCpu + "<br>" + m_sMem + "<br>";
			sData += sBuf;

			if (sData.GetLength() >= infoL)
				sData = sData.Left(infoL - 1);

			sprintf(errmid->usid, Axis::userID);
			strcpy(errmid->optn, "111");
			sprintf(errmid->yymd, "%04d%02d%02d", curr.GetYear(), curr.GetMonth(), curr.GetDay());
			sprintf(errmid->hmss, "%02d%02d%02d", curr.GetHour(), curr.GetMinute(), curr.GetSecond());

			if (m_ipAddr.IsEmpty())
			{
				::gethostname(errmid->ipad, sizeof(errmid->ipad));
				if (lstrcmp(errmid->ipad, "") != 0)
				{
					const HOSTENT FAR* lphostent = ::gethostbyname(errmid->ipad);
					if (lphostent)
					{
						sprintf(errmid->ipad, "%s", inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
					}
				}
				m_ipAddr.Format("%s", errmid->ipad);
			}
			const int ipadL = min(sizeof(errmid->ipad), m_ipAddr.GetLength());
			sprintf(errmid->ipad, "%s", m_ipAddr.Left(ipadL));
			sprintf(errmid->info, sData);

			const bool bret = sendTR("pihoerpt", pBuf.get(), dLen, US_PASS, 'd');
		}
			CATCH(CFileException, e)
		{
		}
		END_CATCH;
	}
	finder.Close();
#else
	CFileFind finder;
	CString sPath;
	sPath.Format("%s\\%s\\%s\\delayinfo", Axis::home, USRDIR, Axis::user);
	if (finder.FindFile(sPath))
	{
		TRY
		{
			CFile file;
			CString sBuf;
			if (file.Open(sPath, CFile::modeRead))
			{
				const long len = (long)file.GetLength();
				file.Read(sBuf.GetBuffer(len + 1), len);
				sBuf.ReleaseBuffer();
				file.Close();
				::DeleteFile(sPath);
			}

			const CTime curr = CTime::GetCurrentTime();
			const int	dLen = sz_errmid;
			char* pBuf = new char[dLen];
			memset(pBuf, ' ', dLen);
			_errmid* errmid = (_errmid*)pBuf;
			const int	infoL = sizeof(errmid->info);		// 512

			CString sData = "**Delay Mon**<br>";
			CString sMap = m_sLastMapInfo;
			sMap.Replace(MAPN_REALTIMEJANGO, "");
			sMap.Replace("IBXXXX00", "");
			sMap.Replace(MAPN_SISECATCH1, "");
			sMap.Replace("IB", "");
			if (m_sWin.IsEmpty())
			{
				m_sCpu = m_sysInfo->GetCPUInfo();	m_sCpu.TrimLeft();
				m_sMem = m_sysInfo->GetMemoryInfo();
			}

			sData += sMap + "<br>" + m_sCpu + "<br>" + m_sMem + "<br>";
			sData += sBuf;

			if (sData.GetLength() >= infoL)
				sData = sData.Left(infoL - 1);

			sprintf(errmid->usid, Axis::userID);
			strcpy(errmid->optn, "111");
			sprintf(errmid->yymd, "%04d%02d%02d", curr.GetYear(), curr.GetMonth(), curr.GetDay());
			sprintf(errmid->hmss, "%02d%02d%02d", curr.GetHour(), curr.GetMinute(), curr.GetSecond());

			if (m_ipAddr.IsEmpty())
			{
				::gethostname(errmid->ipad, sizeof(errmid->ipad));
				if (lstrcmp(errmid->ipad, "") != 0)
				{
					const HOSTENT FAR* lphostent = ::gethostbyname(errmid->ipad);
					if (lphostent)
					{
						sprintf(errmid->ipad, "%s", inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
					}
				}
				m_ipAddr.Format("%s", errmid->ipad);
			}
			const int ipadL = min(sizeof(errmid->ipad), m_ipAddr.GetLength());
			sprintf(errmid->ipad, "%s", m_ipAddr.Left(ipadL));
			sprintf(errmid->info, sData);

			const bool bret = sendTR("pihoerpt", pBuf, dLen, US_PASS, 'd');
			delete[] pBuf;
		}
			CATCH(CFileException, e)
		{
		}
		END_CATCH;
	}
	finder.Close();
#endif

}

void CMainFrame::SetLastMaps(CString sRemoveMap, CString sInsertMap)
{
	//if (sRemoveMap == sInsertMap) return;//sInsertMap sRemoveMap = "";
	
	if (!sRemoveMap.IsEmpty())
	{
		const int pos = m_sLastMapInfo.Find(sRemoveMap, 0);
		if (pos >= 0)
			m_sLastMapInfo.Delete(pos, sRemoveMap.GetLength() + 1);
	}
	if (!sInsertMap.IsEmpty())
	{
		m_sLastMapInfo += sInsertMap + " ";
	}
}

#define AGENT_MSG_PING      9998    // 핑 로그
#define AGENT_MSG_NETTYPE   9999    // 네트워크 타입
#define AGENT_MSG_MONITOR       9995    // CPU 로그
BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	char* pMsg = (char*)pCopyDataStruct->lpData;
	switch (pCopyDataStruct->dwData)
	{
		case AGENT_MSG_NETTYPE: // 9999
			if (strstr(pMsg, "WiFi"))
				m_netTypeStr = "[W]";
			else if (strstr(pMsg, "\xc0\xaf\xbc\xb1"))
				m_netTypeStr = "[L]";
			DrawFrame(); // 타이틀 즉시 갱신
			break;

		case AGENT_MSG_PING: // 9998
			// "[2025-03-15 09:00:01] 시간=12ms" 에서 ms 파싱
		{
			CString msg(pMsg);
			int msPos = msg.Find("ms");
			if (msPos > 0)
			{
				// "ms" 앞에서 "=" 찾기
				CString before = msg.Left(msPos);
				int eqPos = before.ReverseFind('=');
				if (eqPos >= 0)
				{
					CString msStr = before.Mid(eqPos + 1);
					msStr.TrimLeft();
					m_pingMs = atoi(msStr);
					DrawFrame();
				}
			}
		}
		break;
		case AGENT_MSG_MONITOR:
		{
			CString msg(pMsg);
			m_sMonitor = msg;
			DrawFrame();
		}
		break;
		default:
		{
			CString data((const char*)pCopyDataStruct->lpData, pCopyDataStruct->cbData);

			// for WTS Interface
			const int find = data.Find('\t');

			if (find > 0)
			{
				CString account, password;
				account = data.Left(find);
				password = data.Right(data.GetLength() - (find + 1));

				//** send trigger account and password to some order map.

			}
		}
		break;
	}


	return CMDIFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}


BOOL CMainFrame::IsForeignMap(const char* map)
{
	const char* foreignMapName = "IB780100";

	return !strcmp(foreignMapName, map);
}

BOOL CMainFrame::IsNewRealTick(const char* map)
{
	const char* foreignMapName = "IB780100";
	
	return !strcmp(foreignMapName, map);
}

void CMainFrame::RunForeignMap()
{
	CString cmdLine;

	cmdLine.Format("IBK,IBK,A,%d", (int)m_hWnd);
	//MessageBox(Axis::home);
	//SetWorkingDirectory(Axis::home);
	SetCurrentDirectory(Axis::home + "\\exe");
	ShellExecute(NULL, _T("open"), "RTV.EXE", cmdLine,NULL, SW_SHOWNORMAL);
}

void CMainFrame::RunNewRealTick()
{
	CString cmdLine;
	
	//cmdLine.Format("C,%d", m_hWnd);
	cmdLine = "GLOBALWATCHTICKER 026 IBK";
	//MessageBox(Axis::home);
	//SetWorkingDirectory(Axis::home);
	SetCurrentDirectory(Axis::home + "\\exe");
	ShellExecute(NULL, _T("open"), "GWTicker.EXE", cmdLine,NULL, SW_SHOWNORMAL);
// 	CString s;
// 	s.Format("REALTV.EXE [%s]\n",cmdLine);
// 	OutputDebugString(s);

// 	CProfile profile(pkAxis);
// 	
// 	int nRealTV;
// 	
// 	nRealTV = 0;
// 	
// 	nRealTV = profile.GetInt("REALTV","USE");
// 
// 	if(nRealTV != 1)
// 		ShellExecute(NULL, _T("open"), "http://members.edaily.co.kr/kgtg/worldpay/newrealtick/notice.html", NULL,NULL, SW_SHOWNORMAL);
}

//** macho add begin

bool CMainFrame::GetScrButtonPos(int key, LPRECT lpRect)
{
	if (!m_bar2)
		return false;

	CRect rc = m_bar2->GetScrButtonPos(key);

	m_bar2->ClientToScreen(rc);
	if (rc.left < 0)
		::SystemParametersInfo (SPI_GETWORKAREA, 0, lpRect, 0);    
	else
		memcpy(lpRect, (LPRECT)rc, sizeof(RECT));

	return true;
}

/** 
void CMainFrame::AccSetting()
{
	if (m_bCustomer)
	{
		int onlysise = m_axis->GetProfileInt(WORKSTATION, "OnlySise", 0);
		if (onlysise)	return;

#if 1
		CAccSave	dlg(this);
		m_accSave = &dlg;
		dlg.DoModal();
		m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
				(void *)NULL, (BYTE *)(VTS_I4), (short) KEY_ACCTOOL);
		m_accSave = NULL;
#else
		CPassDlg* passDlg = new CPassDlg();
		passDlg->SetData(data);
		if (passDlg->DoModal() == IDOK)
		{
			Caccount* ac;
			data.Empty();
			for (int ii = 0; ii < passDlg->m_acs.GetSize(); ii++)
			{
				rc = 0;
				ac = passDlg->m_acs.GetAt(ii);
				data.Format("%s\t%s\t%s", ac->m_account, ac->m_pass, ac->m_name);
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
							(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(saveACC, 0), (long)data.operator LPCTSTR());
			}
		}
		delete passDlg;
#endif
	}
	else	
		CreateModal("IBXXXX02", 0, 0x9f-4, 99);
}
**/

void CMainFrame::AcctGroupConfig()
{
	if (Axis::isCustomer)
	{
		const int onlysise = m_axis->GetProfileInt(WORKSTATION, "OnlySise", 0);
		if (!Axis::devMode && onlysise) 
			return;
	}
	if (Axis::isCustomer)
	{
		m_mapHelper->CreateChild("IB0000A4", 0);
	}
	else
		m_mapHelper->CreateChild("IB0000A2", 0);
	//m_mapHelper->CreateModal("IB0000A2", 0x9f-5, 97);
}

void CMainFrame::AcctPasswordConfig()
{
	const int onlysise = m_axis->GetProfileInt(WORKSTATION, "OnlySise", 0);

	if (!Axis::devMode && onlysise)
		return;
	
	m_mapHelper->CreateModal("IB0000A1", 0, 0x9f-4, 99);

	//m_mapHelper->CreatePopup("IB0000A1", 1, WK_POPUP, CenterPOS);
}

void CMainFrame::AcctAliasConfig()
{
	AcctConfig();
}

void CMainFrame::AcctConfig()
{
	CAccountConfig config;

	config.DoModal();
}

void CMainFrame::TotalSetup(UINT id)
{
	if (!id)
		id = IDD_ESETUP;

#ifdef DF_USE_CPLUS17
	CTotalSetup dlg(m_tMenu.get(), id, 0, this);
#else
	CTotalSetup dlg(m_tMenu, id, this);
#endif
	dlg.DoModal();

	if (dlg.IsApplied(IDD_ESETUP))
	{
		load_eninfomation(false);
		SetConclusion();

	//	load_start_notice(true);
	//	change_BackGround(
	}

	if (dlg.IsApplied(IDD_MNGSETUP))
		load_mngSetup();

	if (dlg.IsApplied(IDD_ORDER_SETUP))
		SetConclusion();

	if (dlg.IsApplied(IDD_IMAGESETUP))
		change_Skin();

	if (dlg.IsApplied(IDD_SETTICKINFO))
	{
		load_tickInfo();
	}

	if (dlg.IsApplied(IDD_HOTKEY))
		load_hkey();

	if (dlg.IsApplied(IDD_TSETUP))
	{
		m_bar1->Refresh();
		m_smain->Refresh();
	}

	if (dlg.IsApplied(IDD_FUNCTIONS))
		m_bar1->RefreshFunc();
}

LRESULT CMainFrame::ApplySetup(UINT id)
{
	switch (id)
	{
	case IDD_ESETUP:
		load_eninfomation(false);
		break;
	case IDD_MNGSETUP:
		load_mngSetup();
		break;
	case IDD_ORDER_SETUP:
		SetConclusion();
		break;
	case IDD_IMAGESETUP:
		change_Skin();
		break;
	case IDD_SETTICKINFO:
		load_tickInfo();
		break;
	case IDD_HOTKEY:
		load_hkey();
		break;
	case IDD_TSETUP:
		m_bar1->Refresh();
		m_smain->Refresh();
		break;
	case IDD_FUNCTIONS:
		m_bar1->RefreshFunc();
		break;
	}
	return 0;
}

void CMainFrame::load_tickInfo()
{
	ConfigTicker(true);
	sendTicInfo();
	/**
	ShowControlBar(m_tInfo1, dlg.m_bTickView[0], FALSE);
	ShowControlBar(m_tInfo2, dlg.m_bTickView[1], FALSE);
	ShowControlBar(m_bar2, dlg.m_bScreenBar, FALSE);
	**/

	CProfile profile(pkAxisTicker);
#ifdef DF_USE_CPLUS17
	ShowControlBar(m_tInfo1.get(), profile.GetInt(szGeneral, "view1", 1), FALSE);
	ShowControlBar(m_tInfo2.get(), profile.GetInt(szGeneral, "view2", 1), FALSE);
#else
	ShowControlBar(m_tInfo1, profile.GetInt(szGeneral, "view1", 1), FALSE);
	ShowControlBar(m_tInfo2, profile.GetInt(szGeneral, "view2", 1), FALSE);
#endif
	//화면목록바 적용
#ifdef DF_USE_CPLUS17
	ShowControlBar(m_bar2.get(), profile.GetInt(szGeneral, "screenbar", 1), FALSE);
	if (profile.GetInt(szGeneral, "screenbar", 1) && (m_bar0->GetStyle() & WS_VISIBLE))
		ShowControlBar(m_bar0.get(), TRUE, FALSE);
#else
	ShowControlBar(m_bar2, profile.GetInt(szGeneral, "screenbar", 1), FALSE);
	if (profile.GetInt(szGeneral, "screenbar", 1) && (m_bar0->GetStyle() & WS_VISIBLE))
		ShowControlBar(m_bar0, TRUE, FALSE);
#endif

	saveToolStatus();
	SetSDIChangeHeight();
	//화면목록바 적용 끝
	
	if (m_bSDI)
	{
		saveToolStatus();
		SetSDIChangeHeight();
	}
}

void CMainFrame::processMapVersionInfo(WPARAM wParam, LPARAM lParam)
{
	/**
	맵화면 실시간 Update
  : FEV_VERS 메시지로 전달
    lParam   : 화면명 \t 화면명 \t ...
    wParam : lParam length 
	**/

	CStringArray maps;
	CString buffer((const char*)lParam), str;
	while (TRUE) {
		str = Parse(buffer);
		if (str.IsEmpty())
			break;

		if (str.GetLength() == L_MAPN)
		{
			str = str.Mid(2, 4);
			BOOL exist = FALSE;

			for (int i = 0; i < maps.GetSize(); i++)
			{
				if (maps[i] == str)
				{
					exist = TRUE;
					break;
				}
			}
			if (!exist)
				maps.Add(str);
		}
	}

	if (maps.GetSize() > 0)
	{
		//**CDebug::Out(Format("%d maps changed. first map name is %s", maps.GetSize(), maps[0]));
		for (int i = 0; i < maps.GetSize(); i++)
		{
			if (IsExistMap(maps[i]))
			{
				ShowMngInfo(Format("[%s] %s 화면이 변경되었습니다.", maps[i], GetMapName(maps[i])), 0);
			}
		}
	}
}

void CMainFrame::ShowMngInfo(CString msg, int kind, CString sMngType)
{
	if (sMngType == "thread")
	{
		m_slog.Format("[mnginfo]m_mngInfo visible = [%d] ", m_mngInfo->IsWindowVisible());
		std::thread([this]()
			{
				for (;;)
				{
					if (m_mngInfo->IsWindowVisible())
					{
						Sleep(100);
						continue;
					}

					std::pair<std::string, CString> item;
					bool hasItem = false;

					std::lock_guard<std::mutex> lock(_vMutex);
					if (!_vMngInfo.empty())
					{
						item = _vMngInfo.front();
						_vMngInfo.erase(_vMngInfo.begin());
						hasItem = true;
						m_sMngMsg = item.second;
					}

					if (!hasItem)
						break;

					m_slog.Format("[mnginfo][%s][%d] m_sMngMsg = [%s]", __FUNCTION__, __LINE__, m_sMngMsg);
					OutputDebugString(m_slog);


					if (1)
					{
						CString msg;
						msg = m_sMngMsg;
						m_mngInfo->SetData(msg, 1);

						WINDOWPLACEMENT	pl;
						GetWindowPlacement(&pl);
						MngInfoPos();
						SetTimer(TM_AUTOMNGINFO, 3000, NULL);
					}
					else
					{
						if (hasItem)
							PostMessage(WM_USER, MMSG_MAIN_TEST + 1, 0);
						else
							break;
					}
				}
			}).detach();
	}
	else
	{
		m_mngInfo->SetData(msg, kind);

		WINDOWPLACEMENT	pl;
		GetWindowPlacement(&pl);
		switch (pl.showCmd)
		{
		case SW_HIDE:case SW_MINIMIZE:case SW_SHOWMINIMIZED:
			m_mngInfo->ShowWindow(SW_HIDE);
			if (kind == 3 || kind == 4)
			{
				m_slideMsg = msg;
			}
			return;
		default:
			break;
		}
		m_mngInfo->ShowWindow(SW_HIDE);

		MngInfoPos();

		//if (sMngType.IsEmpty())
		{
			if (kind != 3 && kind != 4 && kind != 5)
				SetTimer(TM_AUTOMNGINFO, 3000, NULL);
		}
		/*else
		{
			m_sMngType = sMngType;
			SetTimer(TM_MNGINFO_NOW, 3000, NULL);
		}*/
	}
}

BOOL CMainFrame::IsExistMap(const char* mapName)
{
	const int vsCount = 6;
	
	for (int i = 0; i < vsCount; i++)
	{
		if (IsExistMap(mapName, i))
			return TRUE;
	}
	return FALSE;
}

BOOL CMainFrame::IsExistMap(const char* mapName, int vs)
{
	int key{};
	CChildFrame* pChild{};
	CSChild* sChild{};

	POSITION pos = m_arMDI[vs].GetStartPosition();
	while (pos)
	{
		m_arMDI[vs].GetNextAssoc(pos, key, pChild);
		ASSERT(pChild->m_mapN.GetLength() == L_MAPN);
		if (pChild->m_mapN.Mid(2, 4) == mapName)
			return TRUE;
	}

	pos = m_arSDI[vs].GetStartPosition();
	while (pos)
	{
		m_arSDI[vs].GetNextAssoc(pos, key, sChild);
		ASSERT(sChild->m_mapN.GetLength() == L_MAPN);
		if (sChild->m_mapN.Mid(2, 4) == mapName)
			return TRUE;
	}
	return FALSE;
}

CString CMainFrame::GetMapName(const char* mapName)
{
	return m_tMenu->GetDesc(Format("IB%s00", mapName));
}

CString CMainFrame::get_glb_addr(char* macaddr, char* ip)
{
	WriteLog("");
	OutputDebugString("GLB -------get_glb_addr------\n");
	CString file;
	file.Format("%s\\exe\\axisglb.dll", Axis::home);

	typedef long (WINAPI* GETGLBFUNC)(char*, char*, int, char*, int, bool);
	HMODULE hModule = LoadLibrary(file);
	CString ips("");

	if (hModule)
	{
		GETGLBFUNC func = (GETGLBFUNC)GetProcAddress(hModule, "axGetGLB");
		if (func)
		{
			typedef struct {
				char user[12];
				char pass[8];
				char dats[10];
				char cpas[30];
				char uips[15];
				char madr[16];
				char fill[32];
			} axglbM;

			typedef struct {
				char result;  // R: ok  X: failed (ip는 준다) ===> X, R 이외의 플래그일 경우 추후 메시지 처리 가능.
				char ip[16];
				char ecod[1];
				char verx[32];
			} axglbRcv;

			axglbM* mid{};   mid = new axglbM;
			axglbRcv* mod{}; mod = new axglbRcv;

			if (m_bUseNewLogin)
			{
				CString str = m_axConnect->GetSignInfo();

				Axis::userID = m_axConnect->GetUserID();
				m_pass = m_axConnect->GetPassword();
				m_cpass = m_axConnect->GetCPass();

				memset(mid, ' ', sizeof(axglbM));
				memset(mod, ' ', sizeof(axglbRcv));
				//보안상의 이유로 비밀번호 제거 - 2012.10.07
				FormatCopy(mid->user, Axis::userID);
				FormatCopy(mid->pass, "");
				CopyMemory(mid->dats, str, str.GetLength());
				FormatCopy(mid->cpas, "");
				FormatCopy(mid->uips, ip);
				FormatCopy(mid->madr, macaddr);

				if (m_regkey.Find("STAFF") >= 0)
					FormatCopy(mid->fill, "HTS_HOT_STAFF_0001");   //vc2019
				else
					FormatCopy(mid->fill, "HTS_HOT_0001");

				OutputDebugString("GLB -------get_glb_addr------  HTS_HOT_0001\n");

				int len = func((char*)(const char*)(Axis::home + "\\" + TABDIR),
					(char*)mid, sizeof(axglbM), (char*)mod, sizeof(axglbRcv), (bool)!Axis::isCustomer);

				ips = CString(mod->ip, sizeof(mod->ip));
				ips.TrimRight();
			}
			else
			{
				CString str = m_axConnectOld->GetSignInfo();

				Axis::userID = m_axConnectOld->GetUserID();
				m_pass = m_axConnectOld->GetPassword();
				m_cpass = m_axConnectOld->GetCPass();

				memset(mid, ' ', sizeof(axglbM));
				memset(mod, ' ', sizeof(axglbRcv));
				//보안상의 이유로 비밀번호 제거 - 2012.10.07
				FormatCopy(mid->user, Axis::userID);
				FormatCopy(mid->pass, "");
				CopyMemory(mid->dats, str, str.GetLength());
				FormatCopy(mid->cpas, "");
				FormatCopy(mid->uips, ip);
				FormatCopy(mid->madr, macaddr);

				if (m_regkey.Find("STAFF") >= 0)
					FormatCopy(mid->fill, "HTS_HOT_STAFF_0001");   //vc2019
				else
					FormatCopy(mid->fill, "HTS_HOT_0001");

				int len = func((char*)(const char*)(Axis::home + "\\" + TABDIR),
					(char*)mid, sizeof(axglbM), (char*)mod, sizeof(axglbRcv), (bool)!Axis::isCustomer);

				ips = CString(mod->ip, sizeof(mod->ip));
				ips.TrimRight();
			}

			m_axis->WriteProfileString(INFORMATION, "Server", ips);
			CheckServer(ips);
		}
		FreeLibrary(hModule);
	}

	return ips;
}

CString CMainFrame::get_glb_addr_Index(char* macaddr, char* ip)
{
	OutputDebugString("GLB -------get_glb_addr_Index------\n");


	CString file;
	file.Format("%s\\exe\\axisglb.dll", Axis::home);

	typedef long (WINAPI* GETGLBFUNC)(char*, char*, int, char*, int, int, bool);
	HMODULE hModule = LoadLibrary(file);
	CString ips("");

	if (hModule)
	{
		GETGLBFUNC func = (GETGLBFUNC)GetProcAddress(hModule, "axGetGLB_Index");
		if (func)
		{
			typedef struct {
				char user[12];
				char pass[8];
				char dats[10];
				char cpas[30];
				char uips[15];
				char madr[16];
				char fill[32];
			} axglbM;

			typedef struct {
				char result;  // R: ok  X: failed (ip는 준다) ===> X, R 이외의 플래그일 경우 추후 메시지 처리 가능.
				char ip[16];
				char ecod[1];
				char verx[32];
			} axglbRcv;

			axglbM* mid{};  mid = new axglbM;
			axglbRcv* mod{}; mod = new  axglbRcv;

			if (m_bUseNewLogin)
			{
				CString str = m_axConnect->GetSignInfo();

				Axis::userID = m_axConnect->GetUserID();
				m_pass = m_axConnect->GetPassword();
				m_cpass = m_axConnect->GetCPass();

				memset(mid, ' ', sizeof(axglbM));
				memset(mod, ' ', sizeof(axglbRcv));
				//보안상의 이유로 비밀번호 제거 - 2012.10.07
				FormatCopy(mid->user, Axis::userID);
				FormatCopy(mid->pass, "");
				CopyMemory(mid->dats, str, str.GetLength());
				FormatCopy(mid->cpas, "");
				FormatCopy(mid->uips, ip);
				FormatCopy(mid->madr, macaddr);

				if (m_regkey.Find("STAFF") >= 0)
					FormatCopy(mid->fill, "HTS_HOT_STAFF_0001");   //vc2019
				else
					FormatCopy(mid->fill, "HTS_HOT_0001");

				int len = func((char*)(const char*)(Axis::home + "\\" + TABDIR),
					(char*)mid, sizeof(axglbM), (char*)mod, sizeof(axglbRcv), m_iGlbIndex, (bool)!Axis::isCustomer);

				ips = CString(mod->ip, sizeof(mod->ip));
				ips.TrimRight();
			}
			else
			{
				CString str = m_axConnectOld->GetSignInfo();

				Axis::userID = m_axConnectOld->GetUserID();
				m_pass = m_axConnectOld->GetPassword();
				m_cpass = m_axConnectOld->GetCPass();

				memset(mid, ' ', sizeof(axglbM));
				memset(mod, ' ', sizeof(axglbRcv));
				//보안상의 이유로 비밀번호 제거 - 2012.10.07
				FormatCopy(mid->user, Axis::userID);
				FormatCopy(mid->pass, "");
				CopyMemory(mid->dats, str, str.GetLength());
				FormatCopy(mid->cpas, "");
				FormatCopy(mid->uips, ip);
				FormatCopy(mid->madr, macaddr);

				if (m_regkey.Find("STAFF") >= 0)
					FormatCopy(mid->fill, "HTS_HOT_STAFF_0001");   //vc2019
				else
					FormatCopy(mid->fill, "HTS_HOT_0001");

				int len = func((char*)(const char*)(Axis::home + "\\" + TABDIR),
					(char*)mid, sizeof(axglbM), (char*)mod, sizeof(axglbRcv), m_iGlbIndex, (bool)!Axis::isCustomer);

				ips = CString(mod->ip, sizeof(mod->ip));
				ips.TrimRight();
			}

			m_axis->WriteProfileString(INFORMATION, "Server", ips);
			CheckServer(ips);
		}
		FreeLibrary(hModule);
	}

	return ips;
}

CString CMainFrame::GetChildWindowCaption(int key)
{
	CString caption("");

	CChildFrame* child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		CSChild*	schild;
		if (m_arSDI[m_vsN].Lookup(key, schild))
		{
			schild->GetWindowText(caption);
		}
	}
	else
	{
		child->GetWindowText(caption);
	}

	return caption;
}

LRESULT CMainFrame::SetChildMinMaxInfo(int key, MINMAXINFO* lpMinMaxInfo)
{
	CChildFrame* child;
	if (!m_arMDI[m_vsN].Lookup(key, child))
	{
		CSChild* schild;
		if (m_arSDI[m_vsN].Lookup(key, schild))
		{
			schild->SetMinMaxInfo(lpMinMaxInfo);
		}
	}
	else
	{
		child->SetMinMaxInfo(lpMinMaxInfo);
	}

	return 0L;
}

BOOL CMainFrame::DrawCaption(CDC* dc, CRect rc, int height)
{
	CBitmap* bmpHead{}, * bmpBack{}, * bmpBackEnd{};

	if (m_activeCap)
	{
		if (m_bar1)
		{
			if (m_bar1->IsWindowVisible())
			{
				bmpHead = (Axis::GetSkinBitmap("TITLE",true));
				bmpBack = (Axis::GetBitmap("FRAME_MIDDLE"));
				bmpBackEnd = (Axis::GetSkinBitmap("FRAME_SIDE"));
			}
			else
			{
				bmpHead = (Axis::GetSkinBitmap("TITLE2",true));
				bmpBack = (Axis::GetBitmap("FRAME_MIDDLE"));
				bmpBackEnd = (Axis::GetSkinBitmap("FRAME_SIDE"));
			}
		}
		else
		{
			bmpHead = (Axis::GetSkinBitmap("TITLE",true));
			bmpBack = (Axis::GetBitmap("FRAME_MIDDLE"));
			bmpBackEnd = (Axis::GetSkinBitmap("FRAME_SIDE"));
		}
	}
	else
	{
		if (m_bar1)
		{
			if (m_bar1->IsWindowVisible())
			{
				bmpHead = (Axis::GetSkinBitmap("XN_TITLE",true));
				bmpBack = (Axis::GetBitmap("XN_FRAME_MIDDLE"));
				bmpBackEnd = (Axis::GetBitmap("XN_FRAME_SIDE"));
			}
			else
			{
				bmpHead = (Axis::GetSkinBitmap("XN_TITLE2",true));
				bmpBack = (Axis::GetBitmap("XN_FRAME_MIDDLE"));
				bmpBackEnd = (Axis::GetBitmap("XN_FRAME_SIDE"));
			}
		}
		else
		{
			bmpHead = (Axis::GetSkinBitmap("XN_TITLE",true));
			bmpBack = (Axis::GetBitmap("XN_FRAME_MIDDLE"));
			bmpBackEnd = (Axis::GetBitmap("XN_FRAME_SIDE"));
		}
		
	}

	if (!bmpHead || !bmpBack)
		return FALSE;

	CBitmap* oldBmp{};
	CDC mdc;

	rc.bottom = rc.top + height;

	BITMAP bmHead, bmBack;
	bmpHead->GetBitmap(&bmHead);
	bmpBack->GetBitmap(&bmBack);

	mdc.CreateCompatibleDC(dc);

	oldBmp = mdc.SelectObject(bmpHead);
	
 	if (Axis::isVista)
		dc->BitBlt(rc.left + 4, rc.top - 3, bmHead.bmWidth, bmHead.bmHeight, &mdc, 0, 0, SRCCOPY);
// 		dc->BitBlt(rc.left, rc.top-1, bmHead.bmWidth, bmHead.bmHeight, &mdc, 0, 0, SRCCOPY);
 	else
		dc->BitBlt(rc.left + 4, rc.top - 3  , bmHead.bmWidth, bmHead.bmHeight, &mdc, 0, 0, SRCCOPY); 

	rc.left += bmHead.bmWidth;
	//if (Axis::isVista) rc.left--;

	mdc.SelectObject(bmpBack);
 	if (Axis::isVista)
		dc->StretchBlt(rc.left + 1, rc.top -3 , rc.Width(), rc.Height() + 2, &mdc, 0, 0, bmBack.bmWidth, bmBack.bmHeight , SRCCOPY);
// 		dc->StretchBlt(rc.left, rc.top-1, rc.Width(), rc.Height(), &mdc, 0, 0, bmBack.bmWidth, bmBack.bmHeight, SRCCOPY);
	else
//		dc->StretchBlt(rc.left, rc.top-1, rc.Width(), rc.Height(), &mdc, 0, 0, bmBack.bmWidth, bmBack.bmHeight, SRCCOPY);
	   dc->StretchBlt(rc.left + 1, rc.top -3 , rc.Width(), rc.Height() + 2, &mdc, 0, 0, bmBack.bmWidth, bmBack.bmHeight , SRCCOPY);  //vc2019? test

	/*if (bmpBackEnd)
	{
		mdc.SelectObject(bmpBackEnd);
		BITMAP bm;
		bmpBackEnd->GetBitmap(&bm);
		
		dc->BitBlt(rc.right - bm.bmWidth - 1, rc.top, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);
	}*/

	mdc.SelectObject(oldBmp);
//	if (m_bitmapBtn)
//		m_bitmapBtn->ShowWindow(SW_HIDE);  //vc2019? test
	return TRUE;
}

LRESULT CMainFrame::SetMenuMode(int menuMode, int height)
{
	RecalcLayout();
	return 0;
}

void CMainFrame::DrawFrame(CDC* dc)
{
	CRect rc;
	GetWindowRect(&rc);

	rc.OffsetRect(-rc.TopLeft());

	const int szFrame = 3;

	if (!m_bSDI)
	{
		CBitmap* bmpBgPattern  = Axis::GetSkinBitmap("BG_PATERN");

		CDC mdc;
		mdc.CreateCompatibleDC(dc);
		const CBitmap* oldBmp = mdc.SelectObject(bmpBgPattern);

		BITMAP bm;
		bmpBgPattern->GetBitmap(&bm);

		rc.top += CAPTION_HEIGHT;
		rc.bottom += 2;
		rc.left++;
		rc.right--;
		
		dc->StretchBlt( rc.left, rc.top, 2, rc.Height(), &mdc, 0,0, bm.bmWidth, bm.bmHeight, SRCCOPY );
		dc->StretchBlt( rc.right-2, rc.top, 2, rc.Height(), &mdc, 0,0, bm.bmWidth, bm.bmHeight, SRCCOPY );
		

		/*rc.top += CAPTION_HEIGHT;
		rc.bottom += 2;
		rc.left += 1;
		rc.right -= 2;
		for (int i = 3; i <= 4; i++)
		{
			CPen* oldPen;
			CPen pen(PS_SOLID, 1, Axis::GetColor(i));
			oldPen = dc->SelectObject(&pen);

			dc->MoveTo(rc.left, rc.top);
			dc->LineTo(rc.left, rc.bottom);

			dc->MoveTo(rc.right, rc.top);
			dc->LineTo(rc.right, rc.bottom);
			dc->SelectObject(oldPen);

			rc.left++;
			rc.right--;
		}*/
	}
	else
	{
		rc.left++;
		rc.right--;
		
		for (int i = 0; i < szFrame; i++)
		{
			CPen* oldPen;
			CPen pen(PS_SOLID, 1, Axis::GetColor(3));
			oldPen = dc->SelectObject(&pen);

			dc->MoveTo(rc.left, rc.top);
			dc->LineTo(rc.left, rc.bottom);

			dc->MoveTo(rc.right, rc.top);
			dc->LineTo(rc.right, rc.bottom);
			dc->SelectObject(oldPen);

			rc.left++;
			rc.right--;
		}
	}

}

void CMainFrame::DrawBorder(CDC* dc)
{
	CRect rc, rcLeft, rcBottom, rcRight;
	GetWindowRect(&rc);
	rc.OffsetRect(-rc.TopLeft());
	rcLeft = rc;
	rcRight = rc;
	rcBottom = rc;
	int ii = GetSystemMetrics(SM_CXFRAME);
	ii = GetSystemMetrics(SM_CXSIZEFRAME);
	//rcLeft.right = rcLeft.left + GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXSIZEFRAME);
	rcLeft.right = rcLeft.left + GetSystemMetrics(SM_CXFRAME) + 3;
	dc->FillSolidRect(rcLeft, Axis::GetColor(1));

	//rcRight.left = rcRight.right - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXSIZEFRAME);
	rcRight.left = rcRight.right - GetSystemMetrics(SM_CXFRAME) - 3;
	dc->FillSolidRect(rcRight, Axis::GetColor(1));

	rcBottom.top = rcBottom.bottom - GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CYSIZEFRAME);
	dc->FillSolidRect(rcBottom, Axis::GetColor(1));
	/*
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, Axis::GetColor(1));
	
	CPen* oldPen = dc->SelectObject(&pen);
	
	
	dc->MoveTo(rc.left, rc.top);
	dc->LineTo(rc.left, rc.bottom - 1);
	dc->LineTo(rc.right - 1, rc.bottom - 1);
	dc->LineTo(rc.right - 1, rc.top);
	dc->LineTo(rc.left, rc.top);
	
	dc->SelectObject(oldPen);
	*/
}

void CMainFrame::SetUserInfo()
{
	Axis::userID = m_axis->GetProfileString(WORKSTATION, SIGNONID);
	m_axis->WriteProfileInt(WORKSTATION, "HTS", 1);
	const int pos = Axis::userID.Find('|');
	if (pos >= 0)
	{
#if 0
		// 직원만 디렉토리 암호화 적용
		if (Axis::isCustomer)
		{
			Axis::userNM = Axis::userID.Mid(pos + 1);
			Axis::userID = Axis::userID.Left(pos);
			Axis::user   = Axis::userNM;
		}
		else
#endif
		{
			Axis::userNM = Axis::userID.Mid(pos + 1);
			Axis::userID = Axis::userID.Left(pos);

			if(Axis::userID.Mid(0,3) == "!&$")
			{
				Axis::userID = Axis::userID.Mid(3);

				CString decID = DecryptAES(Axis::userID);
				
				CString s;
				s.Format("DEC [%s] [%s]\n",Axis::userID,decID);
				OutputDebugString(s);
				Axis::userID = decID;
			}

			Axis::user.Format("%08u", HashDataAXIS((LPCSTR)Axis::userNM));
		}
		
		//m_axis->WriteProfileString(WORKSTATION, SIGNONID, Axis::userID +"|");

		if (!Axis::userID.CompareNoCase("guest") && !m_axis->GetProfileString(WORKSTATION, "BackUserID").IsEmpty())
		{
			Axis::userID = m_axis->GetProfileString(WORKSTATION, "BackUserID");

			if(Axis::userID.Mid(0,3) == "!&$")
			{
				Axis::userID = Axis::userID.Mid(3);
				
				CString decID = DecryptAES(Axis::userID);
				
				CString s;
				s.Format("DEC [%s] [%s]\n",Axis::userID,decID);
				OutputDebugString(s);
				Axis::userID = decID;
			}
		}
	}
	else
	{
		Axis::user = "Anonymous";
	}
}

void CMainFrame::DoFunc(int funcID)
{
	switch (funcID - CTRL_FUNC)
	{
	case 1: // 환경설정
		TotalSetup();
		break;
	case 2: // 인쇄
		printImg();
		break;
	case 3: // 화면잠금
		LockProg();
		break;
	case 4: // 계좌비밀번호저장
		AcctPasswordConfig();
		break;
	case 5: // 사용자 WORKSPACE
		//ShowUScreenMenu();
		saveUserScreen();
		break;
	case 6: // 증권계산기
		ExecuteCalculator();
		break;
	case 7: // 해외지수 실시간티커
		/*RunForeignMap();*/
		RunNewRealTick();
		break;
	case 8: // 이미지저장(현재화면)
		saveImg();
		break;
	case 9: // 엑셀시세연동
		m_mapHelper->ChangeChild(MAPN_LINKEXCEL);
		break;
	case 11: //EP
		OpenPBNews();
		break;
	case 12: // reserved..
		{
			char	buf[256]{};
			CString	file, strmap, strname;
			file.Format("%s\\tab\\axisAI.ini", Axis::home);
			DWORD dw = GetPrivateProfileString("MAP", "num", "IBAI0000", buf, sizeof(buf), file);
			strmap.Format("%s", buf);
			strmap.TrimRight();

			m_mapHelper->ChangeChild(strmap);
		}
		break;
	}
}

void CMainFrame::ExecuteCalculator()
{
	ShellExecute(NULL, _T("open"), "SecCalc.exe", NULL,NULL, SW_SHOWNORMAL); 
}

void CMainFrame::GoHomepage()
{
	CProfile profile(GetProfileFileName());
	CString web(profile.GetString(szWebLink, "home", IBK_HOME_URL));
	if (web.IsEmpty())
		return;
	ShellExecute(NULL, _T("open"), web, NULL, NULL, SW_SHOWNORMAL);
}

void CMainFrame::UnderConstruction()
{
	Axis::MessageBox(this, "준비중입니다.", MB_ICONINFORMATION);
}

void CMainFrame::RecalcFrame()
{
	RecalcLayout();
	DrawFrame();
}

void CMainFrame::ShowSingleWindows(BOOL bShow)
{
	CSChild* schild{};
	int key{};
	for (POSITION pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
	{
		m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
		schild->ShowWindow(bShow ? SW_SHOW: SW_HIDE);
	}
}

CString CMainFrame::GetUserPassword()
{
	m_slog.Format("[AXIS][WEB] m_bCertLogin=[%d] m_pass=[%s] ", m_bCertLogin, m_pass);
	OutputDebugString(m_slog);
	//WriteLog(m_slog);
	if (m_bCertLogin)
		return _T("");
	return m_pass;
}

CString CMainFrame::GetCertPassword()
{
	return m_cpass;
}

CRect CMainFrame::GetWndRect()
{
	CRect rc;
	if (m_bSDI)
		GetDesktopWindow()->GetWindowRect(&rc);
	else
		m_MClient->GetWindowRect(&rc);

	return rc;
}

CWnd* CMainFrame::GetClock()
{
#ifdef DF_USE_CPLUS17
	return m_Nclock.get();
#else
	return m_Nclock;
#endif
	
}

void CMainFrame::GetLocalIP()
{
	if (m_ipAddr.GetLength() > 0)
		return;

WriteLog("");
m_slog.Format("[AXIS] GetLocalIP start");
WriteLog(m_slog);
	char buf[256]{};
	CString	file, stmp, smap;
	file.Format("%s\\tab\\axis.ini", Axis::home);
	DWORD dw = GetPrivateProfileString("IP", "local", "", buf, sizeof(buf), file);
	
	if (dw > 0)
	{
		m_bINILocal = TRUE;
		m_ipAddr.Format("%s", buf);
		m_slog.Format("[AXIS] GetLocalIP By ini m_ipAddr=[%s]", m_ipAddr);
WriteLog(m_slog);
		return;
	}

	char szHostName[64] = {0};
	::gethostname(szHostName, sizeof(szHostName));
	stmp.Format("[AXIS] gethostname=[%s]", szHostName);

WriteLog(stmp);

	if(lstrcmp(szHostName, "") != 0)
	{
		const HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
		CString	tmps, serverips = m_ip;

		tmps = m_axis->GetProfileString(INFORMATION, "Port", 
			Format("%d", m_bCustomer? portProxy: portEmployee));
		const UINT	port = atoi(tmps);

		CSocket	sock;
		sock.Create();

		m_slog.Format("[AXIS] GetLocalIP serverips =[%s] [%s]  port=[%d]", serverips, szHostName, port);
		WriteLog(m_slog);

		if (serverips.GetLength() > 0 && sock.Connect(serverips, port))
		{
			IN_ADDR	in{};
			SOCKADDR_IN sockAddr{};
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
				sprintf(szHostName, "%u.%u.%u.%u",
					0xff & lphostent->h_addr_list[ii][0],
					0xff & lphostent->h_addr_list[ii][1],
					0xff & lphostent->h_addr_list[ii][2],
					0xff & lphostent->h_addr_list[ii][3]);
				m_ipAddr = szHostName;
			}
		}
	}

m_slog.Format("[AXIS] GetLocalIP end local ip = [%s]", m_ipAddr);
WriteLog(m_slog);
}

//** FIRE WALL (INCA)
void CMainFrame::InitFirewall()
{
// 	OutputDebugString("InitFirewall\n");
// 	if (m_Npnx.IsExecute()) return;
// 	m_Npnx.m_strCurrentPath =	Axis::home + "\\exe";
// 
// //	m_Npnx.m_strCustomerID =	"ibkistest";
// 	m_Npnx.m_strCustomerID =	"ibkis";					//** npn_업체명.conf
// 	
// 	m_Npnx.m_strModulePath =	Axis::home + "\\exe";
// 	m_Npnx.m_hWnd = GetSafeHwnd();
// 	//m_Npnx.Init();
// 	m_Npnx.Quick();

	if(m_bNoProtect)
		return;

	CString strCustomID,strDllPath;
	strCustomID = "ibkscs";
	strDllPath = Axis::home + "\\exe";

	m_npAppMgr.setDllPath(strDllPath.GetBuffer(0));
	strDllPath.ReleaseBuffer();
	
	DWORD dwErr = m_npAppMgr.isMainRunning();
	if( npenkAppInstall5WIN::eErr_True == dwErr )
	{
		return;
	}

	//OutputDebugString("INCA MODULE LOADING...........\n");

	const DWORD dwWaitTime = 100;
	
	dwErr = m_npAppMgr.start( strCustomID.GetBuffer( 0 ), dwWaitTime );
	strCustomID.ReleaseBuffer();
	if( npenkAppInstall5WIN::eErr_True != dwErr )
	{
		CString ssErrMsg;
		ssErrMsg.Format( _T("에러 메세지 %x\n"), dwErr );
		OutputDebugString(ssErrMsg);
	}
	else
	{
		SetTimer(TM_NPROTECT_ALIVE,30000,NULL);
	}
}

void CMainFrame::FreeFirewall()
{
//  	if (m_Npnx.m_hModule)
//  		m_Npnx.CloseNetizen();
	m_npAppMgr.killMain();

	m_npAppMgr.uninit();

	KillTimer(TM_NPROTECT_ALIVE);
}

//** macho add end

LRESULT CMainFrame::OnPhonePad(WPARAM wParam, LPARAM lParam)
{
	CString file;
	char	buf[512];
	file.Format("%s\\%s\\axisENC.ini", Axis::home, "tab");
	DWORD dw = GetPrivateProfileString("USE", "OnPhonePad", "1", buf, sizeof(buf), file);
	CString stemp{};
	stemp.Format("%s", buf);
	stemp.TrimRight();
	if (stemp == "0")
		return 0;

	//MessageBox((char*)lParam);
	m_slog.Format("[phonepad] OnPhonePad ");
	OutputDebugString(m_slog);
	SetForegroundWindow();
	//m_mapHelper->ChangeChild(m_activeMapN);
	CChildFrame* child = (CChildFrame *) MDIGetActive();

	Setfocus_Child(m_activeKey);
	//child->SetFocus();
	const int len = wParam;
	int ii = 0;
	CString ddd;
	ddd.Format("%d",lParam);
	for(ii=0;ii<(len-ddd.GetLength());ii++)
		ddd = "0"+ddd;

	//AfxMessageBox(ddd);
	if (!m_mapHelper->IsDLL(m_activeMapN))
	{
		//TRACE("MAP\n");
		CString dat = ddd;
		CString sdat;
		sdat = "zPwd\t"+dat;

		m_slog.Format("[phonepad] OnPhonePad sdat=[%s] m_activeKey=[%d]", sdat, m_activeKey);
		OutputDebugString(m_slog);

		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		sdat = "pswd\t"+dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		sdat = "ed_pswd\t"+dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		/*
		dat = "zPwd\t0423";
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)dat);
		dat = "zPwd\t0423";
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)dat);
		*/	
	}
	else
	{
		
		stopDllMessage = false;
		CString dat;
		dat = (char*) lParam;
		sendDllMessage(child->m_hWnd, (long) ddd.operator LPCTSTR());
		if (!stopDllMessage)
		{
			//MessageBox("비밀번호 입력란을 찾을 수 없습니다.");
		}
		
	}	

	return 1;
}

LRESULT CMainFrame::OnRefresh813(WPARAM wParam, LPARAM lParam)
{
	//if (m_TotalAcc) m_TotalAcc->Refresh813((int)lParam);
	return 1;
}

void CMainFrame::sendDllMessage(HWND sdm, LPARAM lparam)
{
	HWND sub_cont_hwnd=0;
	int dd;
	do
	{
		dd = 0;
		//sub_cont_hwnd = FindWindowEx(sdm, sub_cont_hwnd, NULL, NULL);  //vc2019
		CWnd* pwnd = FindWindowEx(sdm, sub_cont_hwnd, NULL, NULL);
		sub_cont_hwnd = pwnd->m_hWnd;
		if ((int)sub_cont_hwnd > 32 )
		{
			//::GetClassName(sub_cont_hwnd, test, 255);
			//MessageBox(test);
			dd = ::SendMessage(sub_cont_hwnd, WM_USER+1111, 0,lparam);
			if (dd == 1)
			{
				//MessageBox("Find it");
				stopDllMessage = true;
			}
			if (stopDllMessage) break;
			sendDllMessage(sub_cont_hwnd, lparam);
		}
	}while( (int)sub_cont_hwnd > 32 );
}

#pragma warning (disable : 26409)
#pragma warning (disable : 26400)
void CMainFrame::ReadPhonePad(int fill)
{
	CPhonePad* m_phone_dlgs;
	m_phone_dlgs = new CPhonePad(this);
	m_phone_dlgs->main_hwnd = m_hWnd;
	//m_phone_dlgs->ShowWindow(SW_SHOWNORMAL);
	m_phone_dlgs->DoModal();
	
	//m_phone_dlgs->SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	return;
			

	//==============포트 열기============
	if(PHONEPAD_PortOpen(0))
	{
		PHONEPAD_InitDevice();
		PHONEPAD_SetConfig(0,10);	
		PHONEPAD_GetPWSignalWindow(m_hWnd);
		
		
		//if (m_phone_dlgs)
		{
			m_phone_dlgs = new CPhonePad(this);
			m_phone_dlgs->ShowWindow(SW_SHOWNORMAL);
			
			m_phone_dlgs->SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
			
			//m_phone_dlgs->BringWindowToTop();
			
		}
		
		MessageBox("폰패드 연결 성공(Port Open Success)!!","IBK투자증권");
	}
	else
	{
		//char * tmp = "0423";
		//PostMessage(WM_PHONEPAD,0,(long)tmp);
		MessageBox("폰패드 연결 실패(Port Open Failure)!!","IBK투자증권");
		return;
	}
	//==============비밀번호 읽어오기 시작============
	// TODO: Add your control notification handler code here
	char * temp = NULL;
	int  loop=1;
	int  limit = 0;
	CString slog;
	
	while(loop)
	{			
		//temp = PHONEPAD_GetPassWDSTR();
		//if(temp != NULL) break;		
		Sleep(300);
		
		switch(PHONEPAD_Status())
		{
		case 0: 
			break;     // normal
			
		case 1: 
			loop = 0; 
			temp = PHONEPAD_GetPassWDSTR();
			break;     //  비밀번호 입력완료
			
		case 2: 
			MessageBox("시간을 초과하였습니다.","IBK투자증권");
			m_phone_dlgs->CloseWindow();
			//m_phone_dlgs = NULL;
			return; 
			break;       //  비밀번호 입력취소
		}
		Sleep(300);
		
		limit++;
		if (limit>=20) break;
	}
	if (limit == 20)
	{
		m_phone_dlgs->CloseWindow();
		//m_phone_dlgs = NULL;
		MessageBox("입력 시간이 초과되었습니다.","IBK투자증권");
		
	}	
	//받아온 비밀번호
	//AfxMessageBox((LPCTSTR)temp);
	CString g_pwd = temp;
	//AfxMessageBox(g_pwd);
	g_pwd.TrimLeft(); g_pwd.TrimLeft();
	//g_pwd.Delete(g_pwd.GetLength()-1);
	//AfxMessageBox("g_pwd: "+g_pwd);
	SetPwd(g_pwd);
	//PostMessage(WM_PHONEPAD,0,(long)g_pwd.operator LPCTSTR());
	//m_stpw.SetWindowText((LPCTSTR)temp);
	
	//==============비밀번호 읽어오기 끝============
	
	//==============포트 닫기============
	PHONEPAD_PortClose();

}

#pragma warning (default : 26409)
#pragma warning (default : 26400)

void CMainFrame::SetPwd(CString strpwd)
{

	//MessageBox((char*)lParam);
	
	

	SetForegroundWindow();
	//m_mapHelper->ChangeChild(m_activeMapN);
	CChildFrame* child = (CChildFrame *) MDIGetActive();

	Setfocus_Child(m_activeKey);
	//child->SetFocus();
	CString ddd;

	if (!m_mapHelper->IsDLL(m_activeMapN))
	{
		//TRACE("MAP\n");
		CString dat = strpwd;
		CString sdat;
		sdat = "zPwd\t"+dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		sdat = "pswd\t"+dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		sdat = "ed_pswd\t"+dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		/*
		dat = "zPwd\t0423";
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)dat);
		dat = "zPwd\t0423";
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)dat);
		*/	
	}else
	{
		stopDllMessage = false;
		CString dat;
		dat = strpwd;
		sendDllMessage(child->m_hWnd, (long) dat.operator LPCTSTR());
		if (!stopDllMessage)
		{
			//MessageBox("비밀번호 입력란을 찾을 수 없습니다.");
		}
	}	

//	return 1;
}

bool CMainFrame::CheckClientWidth(CChildFrame*	child, int preFontSize)
{
	CRect rt, crt;
	GetWindowRect(rt);
	child->GetWindowRect(crt);
	CString tmp;
	double rate{};
	if (((m_preFontSize == 8) || (m_preFontSize == 9)) && ((preFontSize == 8) || (preFontSize == 9))) rate = 1;
	else if (((m_preFontSize == 8) || (m_preFontSize == 9)) && ((preFontSize == 10) || ((preFontSize == 11)))) rate = 1.1647;
	else if (((m_preFontSize == 8) || (m_preFontSize == 9)) && (preFontSize == 12)) rate = 1.32915;
	else if (((m_preFontSize == 10) || (m_preFontSize == 11)) && ((preFontSize == 12))) rate = 1.1413;
	else if (((preFontSize == 8) || (preFontSize == 9)) && ((m_preFontSize == 8) || (m_preFontSize == 9))) rate = 1;
	else if (((preFontSize == 8) || (preFontSize == 9)) && ((m_preFontSize == 10) || ((m_preFontSize == 11)))) rate = 1/1.1647;
	else if (((preFontSize == 8) || (preFontSize == 9)) && (m_preFontSize == 12)) rate = 1/1.32915;
	else if (((preFontSize == 10) || (preFontSize == 11)) && ((m_preFontSize == 12))) rate = 1/1.1413;
	else return true;

	const int nCx = int(crt.Width()*rate);
	//tmp.Format("%d<=>%d(%d)\n",crt.left+nCx,rt.right,nCx);
	//TRACE(tmp);
	
	if (crt.left+nCx > rt.right)
	{
		//refleshMap(child->m_key);
		const long rc = 0;
		const int value = m_preFontSize;
		if(value>0)
		{
			const int ret = MessageBox("표시할수 있는 범위를 벗어 났습니다.\n계속진행하시겠습니까?","IBK투자증권",MB_YESNO);
			if (ret == IDNO)
				return false;
		}
	}
	return true;
}

void CMainFrame::CheckSoundConfig()
{
#if 1
	// 아래 루틴이 실제 설정파일의 Path를 깨먹는듯 하다!
	return;
#else
	CString ordcfg_path = Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "ordcfg.ini");
	if (IsFileExist(ordcfg_path))
	{
		CProfile profile(ordcfg_path);
		CString	m_waveF[MAXSOUND];
		CString section = "Common";
		m_waveF[IDX_MDORDER] = profile.GetString(section, "SOUNDFILE_MDORDER", m_waveF[IDX_MDORDER]);
		m_waveF[IDX_MSORDER] = profile.GetString(section, "SOUNDFILE_MSORDER", m_waveF[IDX_MSORDER]);
		m_waveF[IDX_JJORDER] = profile.GetString(section, "SOUNDFILE_JJORDER", m_waveF[IDX_JJORDER]);
		m_waveF[IDX_CSORDER] = profile.GetString(section, "SOUNDFILE_CSORDER", m_waveF[IDX_CSORDER]);
		m_waveF[IDX_MDCON]   = profile.GetString(section, "SOUNDFILE_MDCON",   m_waveF[IDX_MDCON]);
		m_waveF[IDX_MSCON]   = profile.GetString(section, "SOUNDFILE_MSCON",   m_waveF[IDX_MSCON]);
		m_waveF[IDX_REFUSAL] = profile.GetString(section, "SOUNDFILE_REFUSAL", m_waveF[IDX_REFUSAL]);
		
		//초기 설정값 변경
		for(int i=0; i<7;i++)
		{
			if (m_waveF[i].Find("C:\\Program Files\\IBKS\\HTS"))
			{
				if (Axis::home != "C:\\Program Files\\IBKS\\HTS")
				{
					if (!IsFileExist(m_waveF[i]))
					{
						if(m_waveF[i].Find("image\\"))
						{
							m_waveF[i].Delete(0,m_waveF[i].Find("image\\"));
							m_waveF[i] = Axis::home +"\\"+ m_waveF[i];
						}
					}
				}
			}
		}
		profile.Write(section, "SOUNDFILE_MDORDER", m_waveF[IDX_MDORDER]);
		profile.Write(section, "SOUNDFILE_MSORDER", m_waveF[IDX_MSORDER]);
		profile.Write(section, "SOUNDFILE_JJORDER", m_waveF[IDX_JJORDER]);
		profile.Write(section, "SOUNDFILE_CSORDER", m_waveF[IDX_CSORDER]);
		profile.Write(section, "SOUNDFILE_MDCON",	m_waveF[IDX_MDCON]);
		profile.Write(section, "SOUNDFILE_MSCON",	m_waveF[IDX_MSCON]);
		profile.Write(section, "SOUNDFILE_REFUSAL", m_waveF[IDX_REFUSAL]);
	}
#endif
}

void CMainFrame::CheckNewsSetting()
{
	//return;
	
	CString	file, section, keys, value;
	bool m_bHk = false;
	bool m_bMk = false;
	bool m_bAs = false; //아시아경제
	bool m_bHr = false; //헤럴드경제
	int idx = -1;
	int bx = -1; int by = -1;

	file.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, TICKSETUP);
	if(!IsFileExist(file)) return;
	//if (GetPrivateProfileInt("InitNewsVer", "VER", 0, file)>0) return;
#ifdef DF_USE_CPLUS17
	std::unique_ptr<_tickitem>item = std::make_unique<_tickitem>();
	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			section.Format("BAR_INFO_%d%d", ii, jj);
			item->kind = GetPrivateProfileInt(section, "INDEXK", 0, file);
			switch (item->kind)
			{
			case TKIND_NEWS:
			{
				bx = ii; by = jj;
				item->aux = 0;
				item->sAry.RemoveAll();
				item->selectN = 0;
				int hh = 0;
				for (int kk = 0; ; kk++)
				{
					keys.Format("%02d", kk);
					hh = GetPrivateProfileInt(section, keys, -1, file);
					idx = kk;
					if (hh == -1)	break;
					value.Format("%d", hh);
					if (hh == 12) m_bHr = true;
					if (hh == 11) m_bAs = true;
					if (hh == 10) m_bMk = true;
					if (hh == 9) m_bHk = true;
					item->sAry.Add(value);
				}
				//keys = _T("AUX");
				//item->aux = GetPrivateProfileInt(section, keys, 0, file);

				//if (!item->sAry.GetSize())	item->sAry.Add("1");
			}
			break;

			default:
				//AfxMessageBox("Tick infomation load Error!");
				continue;
			}
		}
	}
#else
	_tickitem* item = new _tickitem;
	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			section.Format("BAR_INFO_%d%d", ii, jj);
			item->kind = GetPrivateProfileInt(section, "INDEXK", 0, file);
			switch (item->kind)
			{
			case TKIND_NEWS:
			{
				bx = ii; by = jj;
				item->aux = 0;
				item->sAry.RemoveAll();
				item->selectN = 0;
				int hh = 0;
				for (int kk = 0; ; kk++)
				{
					keys.Format("%02d", kk);
					hh = GetPrivateProfileInt(section, keys, -1, file);
					idx = kk;
					if (hh == -1)	break;
					value.Format("%d", hh);
					if (hh == 12) m_bHr = true;
					if (hh == 11) m_bAs = true;
					if (hh == 10) m_bMk = true;
					if (hh == 9) m_bHk = true;
					item->sAry.Add(value);
				}
				//keys = _T("AUX");
				//item->aux = GetPrivateProfileInt(section, keys, 0, file);

				//if (!item->sAry.GetSize())	item->sAry.Add("1");
			}
			break;

			default:
				//AfxMessageBox("Tick infomation load Error!");
				continue;
			}
		}
	}

	// 2011.07.22 leak! by warship
	delete item; item = NULL;
#endif
	
	

	if ((bx==-1) ||(by==-1)||(idx==-1)) return;
	if (GetPrivateProfileInt("InitNewsVer", "VER", 0, file)<=0)
	{
		if(!m_bAs)
		{
			section.Format("BAR_INFO_%d%d", bx, by);
			keys.Format("%02d", idx);
			value = "11";//item->sAry.GetAt(kk);
			//AfxMessageBox(section +"]["+keys+"]["+ value);
			WritePrivateProfileString(section, keys, value, file);
			WritePrivateProfileString("InitNewsVer", "VER", "1", file);
			idx++;
		}
		if(!m_bHk)
		{
			section.Format("BAR_INFO_%d%d", bx, by);
			keys.Format("%02d", idx);
			value = "9";//item->sAry.GetAt(kk);
			//AfxMessageBox(section +"]["+keys+"]["+ value);
			WritePrivateProfileString(section, keys, value, file);
			WritePrivateProfileString("InitNewsVer", "VER", "1", file);
			idx++;
		}
	}
	if (GetPrivateProfileInt("InitNewsVer", "VER_ASIA", 0, file)<=0)
	{
		if(!m_bMk)
		{
			section.Format("BAR_INFO_%d%d", bx, by);
			keys.Format("%02d", idx);
			value = "10";//item->sAry.GetAt(kk);
			//AfxMessageBox(section +"]["+keys+"]["+ value);
			WritePrivateProfileString(section, keys, value, file);
			WritePrivateProfileString("InitNewsVer", "VER_ASIA", "1", file);
			idx++;
		}
	}
	
	if (GetPrivateProfileInt("InitNewsVer", "VER_HERALD", 0, file)<=0)
	{
		if(!m_bHr)
		{
			section.Format("BAR_INFO_%d%d", bx, by);
			keys.Format("%02d", idx);
			value = "12";//item->sAry.GetAt(kk);
			//AfxMessageBox(section +"]["+keys+"]["+ value);
			WritePrivateProfileString(section, keys, value, file);
			WritePrivateProfileString("InitNewsVer", "VER_HERALD", "1", file);
		}
	}

	//IR GO  , 컨슈머타임즈 뉴스 추가
	CString slog;
	if(GetPrivateProfileInt("InitNewsVer", "VER", 0, file) != 2)
	{
		for (int ii = 0; ii < TICKBAR_CNT; ii++)
		{
			for (int jj = 0; jj < SPLIT_CNT; jj++)
			{
				section.Format("BAR_INFO_%d%d", ii, jj);
				const int kind = GetPrivateProfileInt(section, "INDEXK", 0, file);
				
				slog.Format("[news] ----------------------------section=[%s] ----------- ", section);
				//OutputDebugString(slog);
				int kk = 0;
				switch (kind)
				{
				case TKIND_NEWS:
					{	
						bool bIR = false;
						bool bCON = false;
						bool bjustgong = true;
						CString str, stmp;
						int ivalue{};
						
						char		wb[512];
						memset(wb, ' ', 512);
						const int readL = GetPrivateProfileString(section, NULL, "", wb, sizeof(wb), file);
						
						str.Empty();
						for(kk = 0 ; kk < readL; kk++)
						{	
							stmp.Format("%c", wb[kk]);
							if(wb[kk] == NULL)
							{
								if(atoi(str) > 0 && atoi(str) <= 15)  //뉴스 추가되면 15를 더 크게 해줘야 함
								{
									keys.Format("%s", str);
									ivalue = GetPrivateProfileInt(section, keys, -1, file);
									if(ivalue == 13)
										bIR = true;
									if(ivalue == 14)
										bCON = true;
									if(ivalue != -1 && (ivalue >= 3))
										bjustgong = false;
									
									str.Empty();
								}
								else
									str.Empty();
							}
							else
								str += stmp;
						}
						
						//	if (bIR == false && bCON == false && !bjustgong && (GetPrivateProfileInt("InitNewsVer", "VER", 0, file) != 2))
						if (!bjustgong && (GetPrivateProfileInt("InitNewsVer", "VER", 0, file) != 2))
						{
							stmp.Format("%02d", atoi(keys) + 1);
							OutputDebugString("[news] ----------------------------------------------------");
							slog.Format("[news] !!!!!!!!!!  ver=[%d] section=[%s] keys=[%s] [%s] bIR=[%d] bCON=[%d]  ",
								GetPrivateProfileInt("InitNewsVer", "VER", 0, file), section, keys, stmp, bIR, bCON);
							OutputDebugString(slog);
							
							WritePrivateProfileString(section, stmp, "13", file);
							stmp.Format("%02d", atoi(keys) + 2);
							
							slog.Format("[news] !!!!!!!!!!  ver=[%d] section=[%s] keys=[%s] [%s] bIR=[%d] bCON=[%d]  ",
								GetPrivateProfileInt("InitNewsVer", "VER", 0, file), section, keys, stmp, bIR, bCON);
							OutputDebugString(slog);
							
							WritePrivateProfileString(section, stmp, "14", file);
							WritePrivateProfileString("InitNewsVer", "VER", "2", file);
							
							slog.Format("[news] !!!!!!!!!!  ver=[%d] section=[%s] keys=[%s] [%s] bIR=[%d] bCON=[%d]  ",
								GetPrivateProfileInt("InitNewsVer", "VER", 0, file), section, keys, stmp, bIR, bCON);
							OutputDebugString(slog);
							OutputDebugString("[news] ----------------------------------------------------");
						}	
					}
					break;
				} //for
			}
		} //for
	}//if
}

BOOL CMainFrame::IsWebMenu(const char *MapName)
{
	CProfile profile(Format("%s\\%s\\WEBINFO.INI", Axis::home, TABDIR));

	return !profile.GetString(CString(MapName).Mid(2, 4), "Type").IsEmpty();
}

BOOL CMainFrame::IsHelpMenu(const char *MapName)
{
	CProfile profile(Format("%s\\%s\\HELPLIST.INI", Axis::home, TABDIR));
	
	return !profile.GetString("SCREENLIST",CString(MapName).Mid(0, 8)).IsEmpty();
}


CString CMainFrame::Variant(int nComm, CString strData)
{
	//위저드는 현재 쓸수없는 컨트롤임 만약 쓰려면 위닉스 플랫폼 담당자와 협의 해야함
	CString strRet(_T(""));
	const char* pRet = NULL;
		
	long rc = 0;
	
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), nComm, 0);
	strRet = rc ? (char *)rc : _T("");
	
	/*
	pRet = (char*)m_wizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);
	
	if ((long)pRet > 1)
		strRet = pRet;
	*/	
	return strRet;
}

void CMainFrame::ChangeLogo()
{
	if (m_bSDI)
	{
		m_bitmapBtn->ShowWindow(SW_HIDE);
		return;
	}
	else
	{
		if (!m_bitmapBtn->IsWindowVisible())
			m_bitmapBtn->ShowWindow(SW_SHOW);
	}
	if (m_bar1->IsWindowVisible())
	{
		//m_bitmapBtn->MoveWindow(5,0, 71,55);
		if(Axis::isVista)
			m_bitmapBtn->SetWindowPos( NULL, 5,0, 71,55, SWP_NOZORDER );
		else
			m_bitmapBtn->SetWindowPos( NULL, 5,0, 71,55, SWP_NOZORDER );
		
		m_bitmapBtn->SetImgBitmap(Axis::GetSkinBitmap("LOGO", true), 1);
	}
	else
	{
		//m_bitmapBtn->MoveWindow(4,-1, 71,22);
		if(Axis::isVista)
			m_bitmapBtn->SetWindowPos( NULL, 5,0, 71,24, SWP_NOZORDER );
		else
			m_bitmapBtn->SetWindowPos( NULL, 5,0, 71,24, SWP_NOZORDER );

		m_bitmapBtn->SetImgBitmap(Axis::GetSkinBitmap("LOGO2", true), 1);
	}
}

bool CMainFrame::IsFileExist(CString filename)
{
     CFileFind   finder;

     if(!finder.FindFile(filename))
            return FALSE;

     return TRUE;
}

BOOL CMainFrame::IsNoFontMenu(const char *MapName)
{
	CProfile profile(Format("%s\\%s\\HELPLIST.INI", Axis::home, TABDIR));
	
	return !profile.GetString("NOFONTLIST",CString(MapName).Mid(0, 8)).IsEmpty();
}

void CMainFrame::InitMapSize()
{
	CString name = "IBINITSIZE";
	CProfile pout(pkUserConfig);
	pout.Write(name, "OnLoad", 1);
	int	key = 0;
	CChildFrame*	pChild = NULL;
	CSChild*	sChild = NULL;
	for ( int ii = 0 ; ii < 6 ; ii++ )
	{
		POSITION	pos = m_arMDI[ii].GetStartPosition();
		while (pos)
		{
			m_arMDI[ii].GetNextAssoc(pos, key, pChild);
			actionCaption(pChild->m_key, IDX_FONTX);
		}

		pos = m_arSDI[ii].GetStartPosition();
		while (pos)
		{
			m_arSDI[ii].GetNextAssoc(pos, key, sChild);
			actionCaption(sChild->m_key, IDX_FONTX);
		}
	}
}

bool CMainFrame::SetToolAccount(CString acnt, int wparam)
{
	const CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (m_activeMapN.Mid(0,6)=="IB7700")	return true;
	Setfocus_Child(m_activeKey,TRUE);
	//child->SetFocus();
	CString ddd;
	CStringArray sary;
	
	sary.Add("AN10"); sary.Add("AN30"); sary.Add("AN11"); sary.Add("AN12"); sary.Add("AN13");
	sary.Add("AN14"); sary.Add("AN15"); sary.Add("AN24");  sary.Add("AN50");
	sary.Add("AN99"); sary.Add("AN01"); sary.Add("AN16"); sary.Add("AN17"); 
	sary.Add("AN22"); sary.Add("AN19"); sary.Add("AN18"); sary.Add("AN31"); sary.Add("AN32");
	sary.Add("AN33"); sary.Add("AN34"); sary.Add("AN1A"); sary.Add("AN1B"); sary.Add("AN00");
	sary.Add("AN97"); sary.Add("AN98"); sary.Add("AN51");
	
	sary.Add("AN20");sary.Add("AN21");
	

	//if (!m_mapHelper->IsDLL(m_activeMapN))
	{
		//TRACE("MAP\n");
		CString dat = acnt;
		CString sdat;
		/*
		sdat = "AN20\t"+dat;
		
		int		key;
		POSITION	pos;
		CChildFrame*	pChild;
		
		for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);
			//if (m_mapHelper->IsDLL(pChild->m_mapN)) continue;
			if (key != m_activeKey)	key |= 0xff00;

			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sdat);
			break;
		}
		*/
		
		
		for (int jj = 0; jj<sary.GetSize(); jj++)
		{
			sdat = sary.GetAt(jj)+"\t"+dat;//+"\nACNM\t\"정문일";
			
			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
								(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		}
		/*
		for (int jj = 0; jj<sary.GetSize(); jj++)
		{
			sdat = sary.GetAt(jj)+"\t"+dat;//+"\nACNM\t\"정문일";

			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
								(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		}
		*/
		/*
		sdat = "pswd\t"+dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		sdat = "ed_pswd\t"+dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		/*
		dat = "zPwd\t0423";
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)dat);
		dat = "zPwd\t0423";
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
							(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)dat);
		*/	
	}
	/*
	else
	{
		//DLL인경우 좀더 심사숙고처리
		stopDllMessage = false;
		CString dat;
		dat = acnt;
		
		//sendDllMessage(child->m_hWnd, (long) dat.operator LPCTSTR());
		if (!stopDllMessage)
		{
			//MessageBox("비밀번호 입력란을 찾을 수 없습니다.");
		}
	}	
	*/
	return true;
}

void CMainFrame::PopupPosWeb(CString url, int left, int top, int cx, int cy)
{
	if (url.Find("@") != -1)
	{
		ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOWNORMAL);
		return;
	}
	
	//CString tmp;
	

	CString HomePage = url;
	VARIANT vFlags = {0},
	vTargetFrameName = {0},
	vPostData = {0},
	vHeaders = {0};

	HRESULT hr{};
	_bstr_t	bstr(HomePage);
	const UINT	nFlag = SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW;
	WINDOWPLACEMENT	pl{};

	if (FAILED(hr = CoInitialize(NULL)))
		return;

	struct	IWebBrowserApp* pWEBApp{};
	HWND	hWEB{};
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
	pWEBApp->put_Left(left);//VARIANT_TRUE);
	pWEBApp->put_Top(top);//VARIANT_TRUE);
	pWEBApp->put_ToolBar(VARIANT_FALSE);
	pWEBApp->put_StatusBar(VARIANT_FALSE);
	//pWEBApp->put_Title("IBK투자증권 공지");
	hr = pWEBApp->Navigate(bstr, &vFlags, &vTargetFrameName, &vPostData, &vHeaders);
	if (HomePage.IsEmpty())	pWEBApp->GoHome();
	pWEBApp->put_Visible(VARIANT_TRUE);
	VariantClear(&vPostData);

	pWEBApp->get_HWND((long *)&hWEB);
	::GetWindowPlacement(hWEB, &pl);
	if (pl.showCmd & SW_SHOWMINIMIZED)
	{
		pl.showCmd = SW_RESTORE;
		::SetWindowPlacement(hWEB, &pl);
	}
	else	
		::SetWindowPos(hWEB, HWND_TOPMOST, 0, 0, 0, 0, nFlag);
	
	::SetWindowLong(hWEB, GWL_EXSTYLE, ::GetWindowLong(hWEB, GWL_EXSTYLE) | WS_EX_TOPMOST);
	
	CoUninitialize();
	//SendMessage(WM_ACTIVATE);
	//SetActivateWindow(m_hWnd);
	SetForegroundWindow();

	
}

#pragma warning (disable : 26400)
#pragma warning (disable : 26409)
#pragma warning (disable : 26401)

void CMainFrame::ReadNotice()
{
	CString	Path;
	Path.Format("%s\\%s\\%s\\notice.dat", Axis::home, USRDIR, Axis::user);
	int nrec{}, grec{}; nrec = grec = 0;
	const int pos  = 0;
	CString gpid, gpnm;

	const bool find = false;
	struct	_grid {
		char	url[255];		 /* 종목코드			 */
		char    width[5];                /* 종목명                       */
		char    height[5];                 /* 현재가                       */
		char    use[1];                 /* 전일대비      (9999V99)      */
		char	cookie[20];				//쿠키명
	};

	struct  _urlData {
		char	nrec[4];		// count jcode
		struct	_grid	grid[1];	// data
	};
	
	CString		url, width, height, use, cookie;
	
	TRY
	{
		CFile file;
		if (file.Open(Path, CFile::modeRead)) 
		{
			const int len = (int)file.GetLength();
			if (len > 0)	
			{
				char	*dat = new char[len];
				file.Read(dat, len);
		
				const _urlData*	urlData = (struct _urlData*) dat;
				
				int	cnt = atoi(CString(urlData->nrec, sizeof(urlData->nrec)));
				int left{}, top{}; left = top = 0;
				DWORD dwSize = 1024;
				TCHAR szData[1024]{};
				CString value;

				bool bRet{}, find{};
				int ipos{};
				for (int ii = 0; ii < cnt; ii++)
				{
					//FillMemory(urlData->grid[ii].url, sizeof(urlData->grid[ii].url), ' ');
					url    = CString(urlData->grid[ii].url, sizeof(urlData->grid[ii].url));	url.TrimRight();
					width  = CString(urlData->grid[ii].width, sizeof(urlData->grid[ii].width));	width.TrimRight();
					height = CString(urlData->grid[ii].height, sizeof(urlData->grid[ii].height));	height.TrimRight();
					use    = CString(urlData->grid[ii].use, sizeof(urlData->grid[ii].use));	use.TrimRight();
					cookie = CString(urlData->grid[ii].cookie, sizeof(urlData->grid[ii].cookie));	cookie.TrimRight();
					width.Remove(',');
					height.Remove(',');

					bRet = false;
					find = false;
					bRet = (bool)InternetGetCookie(url,NULL, szData, &dwSize);//szData, &dwSize);
					
					if (bRet) 
					{
						value = szData;
						ipos = value.Find(cookie+"=");
						//CString tmp; tmp.Format("ipos: [%d], cookie: [%s=]\n%s",ipos, cookie, szData);
						//AfxMessageBox(tmp);
						if (ipos!=-1) find = true;
					}

					if (!find)
					PopupPosWeb(url, left, top, atoi(width), atoi(height));
					
					left += atoi(width);
					left += 10; //간격
					
				}				
				delete[] dat;
			}
			file.Close();
		}
	}
	CATCH (CFileException, e) 
	{
	}
	END_CATCH;
	
	
	//HttpQueryInfo(hURL, HTTP_QUERY_EXPIRES, &dwCertSize, &dwSize, 0);
	/*
	CInternetSession m_Session;
	CInternetFile * pInternetFile = NULL;

	char bufQuery[256];
	ZeroMemory(bufQuery, sizeof(bufQuery));
	BOOL bQuery = FALSE;
	CString strUrl;
	strUrl.Format("http://www.ibks.com/");
	DWORD dwLengthBufQuery;
	pInternetFile = (CInternetFile*)m_Session.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_BINARY, NULL, 0);
	CHttpFile *pHttpFile = (CHttpFile *)pInternetFile;
	bQuery = pHttpFile->QueryInfo(HTTP_QUERY_EXPIRES, bufQuery, &dwLengthBufQuery);
	AfxMessageBox(bufQuery);
	*/
}

#pragma warning (default : 26400)
#pragma warning (default : 26409)
#pragma warning (default : 26401)

void CMainFrame::ReadNoticeMap()
{
	CString	PathConf,PathCookie;
	PathConf.Format("%s\\%s\\NOTICECONF.INI", Axis::home, "tab");
	PathCookie.Format("%s\\%s\\NOTICECOOKIE.INI", Axis::home, "tab");

	int nrec{}, grec{}; nrec = grec = 0;
	const int pos  = 0;
	CString gpid, gpnm;
	
	const bool find = false;

// 	OutputDebugString(Path);
// 	OutputDebugString("\n");
	CString		url;
	
	TRY
	{
		nrec = GetPrivateProfileInt("BANNER","imgCount",0,PathConf);
		CString s;
		s.Format("[NREC : %d]",nrec);
		//OutputDebugString(s);

		for(int i=0;i<nrec;i++)
		{
			CString strKey;
			strKey.Format("B%d",i);
// 			s.Format("[KEY : %s]",strKey);
// 			OutputDebugString(s);
			const int nomore = GetPrivateProfileInt(strKey,"nomore",1,PathConf);
			const int showlayer = GetPrivateProfileInt(strKey,"showlayer",1,PathConf);

			const int user = GetPrivateProfileInt(strKey,"USER",0,PathConf);

			if(Axis::isCustomer && user == 1)
			{
				continue;
			}
			else if(!Axis::isCustomer && user == 2)
			{
				continue;
			}

			if(nomore == 0)
			{
				int left = GetPrivateProfileInt(strKey,"LEFT",0,PathConf);
				int top = GetPrivateProfileInt(strKey,"TOP",0,PathConf);

				CString date;

				CTime time;
				time = CTime::GetCurrentTime();

				date.Format("%04d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay());

				char readB[1024];
				int readL;

				readL = GetPrivateProfileString(strKey,"FROM",date,readB,sizeof(readB),PathConf);

				CString strFrom(readB,readL);
// 				s.Format("[FROM : %s]",strFrom);
// 				OutputDebugString(s);
				readL = GetPrivateProfileString(strKey,"TO",date,readB,sizeof(readB),PathConf);
				
				CString strTo(readB,readL);
// 				s.Format("[TO : %s]",strTo);
// 				OutputDebugString(s);

// 				s.Format("[DATE : %s]",date);
// 				OutputDebugString(s);
				if(atoi(date) < atoi(strFrom) || atoi(date) > atoi(strTo))
				{
					OutputDebugString("[Out Of Date]\n");
					continue;
				}
				
				const int update = GetPrivateProfileInt(strKey,"update",0,PathConf);

				if(update == 1)
				{
					WritePrivateProfileString(strKey,"nomore","0",PathCookie);
					WritePrivateProfileString(strKey,"update","0",PathConf);
				}

				const int type = GetPrivateProfileInt(strKey,"readtype",0,PathConf);

				if(type == 0)
				{
					const int cookie_nomore = GetPrivateProfileInt(strKey,"nomore",0,PathCookie);
					
					if(cookie_nomore != 0)
					{
// 						readL = GetPrivateProfileString(strKey,"TO",date,readB,sizeof(readB),PathCookie);
// 
// 						CString cookieTo(readB,readL);
// 
// 						if(atoi(date) <= atoi(cookieTo))
// 						{
// 							OutputDebugString("[Cookie Out Of Date]\n");
// 							continue;
// 						}
// 						else 
// 						{
// 							WritePrivateProfileString(strKey,"nomore","0",PathCookie);
// 						}
						continue;
					}
				}

				if(showlayer == 1)
				{
					if(strKey == "B0")
					{
						left = 50;
						top = 50;
					}
					else if(strKey == "B1")
					{
						left = 400;
						top = 400;

					}
					else if(strKey == "B2")
					{
						left = 200;
						top = 200;
					}
				}

// 				s.Format("[X : %d Y : %d]\n",left,top);
// 				OutputDebugString(s);
				if(strKey == "B0")
				{
					CString slog;
					slog.Format("%d %d", left, top);
					m_mapHelper->CreateChild("AM080101",0,0,LtopPOS,CPoint(left,top));
				}
				else if(strKey == "B1")
					m_mapHelper->CreateChild("AM080201",0,0,CenterPOS,CPoint(left,top));
				else if(strKey == "B2")
					m_mapHelper->CreateChild("AM080301",0,0,RbottomPOS,CPoint(left,top));
			}
		}

		if(!Axis::isCustomer)
		{
			CString strStaffKey;
			strStaffKey = "STAFF";
	// 			s.Format("[KEY : %s]",strKey);
	// 			OutputDebugString(s);
			const int nomore = GetPrivateProfileInt(strStaffKey,"nomore",1,PathConf);

			if(nomore == 0)
			{
				const int left = GetPrivateProfileInt(strStaffKey,"LEFT",0,PathConf);
				const int top = GetPrivateProfileInt(strStaffKey,"TOP",0,PathConf);

				CString date;

				CTime time;
				time = CTime::GetCurrentTime();
				
				date.Format("%04d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay());
				
				char readB[1024];
				int readL;

				readL = GetPrivateProfileString(strStaffKey,"FROM",date,readB,sizeof(readB),PathConf);

				CString strFrom(readB,readL);
	// 				s.Format("[FROM : %s]",strFrom);
	// 				OutputDebugString(s);
				readL = GetPrivateProfileString(strStaffKey,"TO",date,readB,sizeof(readB),PathConf);
				
				CString strTo(readB,readL);
	// 				s.Format("[TO : %s]",strTo);
	// 				OutputDebugString(s);

	// 				s.Format("[DATE : %s]",date);
	// 				OutputDebugString(s);
				if(atoi(date) < atoi(strFrom) || atoi(date) > atoi(strTo))
				{
					OutputDebugString("[Out Of Date]\n");
					return;
				}
				
				const int update = GetPrivateProfileInt(strStaffKey,"update",0,PathConf);

				if(update == 1)
				{
					WritePrivateProfileString(strStaffKey,"nomore","0",PathCookie);
					WritePrivateProfileString(strStaffKey,"update","0",PathConf);
				}

				const int type = GetPrivateProfileInt(strStaffKey,"readtype",0,PathConf);

				if(type == 0)
				{
					const int cookie_nomore = GetPrivateProfileInt(strStaffKey,"nomore",0,PathCookie);
					
					if(cookie_nomore != 0)
					{
						return;
					}
				}
				
				m_mapHelper->CreateChild("AM080401",0,0,RtopPOS,CPoint(6,0));
			}
		}
	}
	CATCH (CFileException, e) 
	{
	}
	END_CATCH;
}

CString CMainFrame::GetMyThemeName()
{
	/*
	WCHAR szThemeName[MAXARRAY] = { 0 };
	WCHAR szColorBuff[MAXARRAY] = { 0 };
	WCHAR szSizeBuff[MAXARRAY] = { 0 }; 
	HRESULT hr = GetCurrentThemeName(szThemeName, MAXARRAY,
	szColorBuff, MAXARRAY, 
	szSizeBuff, MAXARRAY ); 
	TCHAR tmp[MAXARRAY]; 
	TCHAR tmp2[MAXARRAY]; 
	CString stheme;
	

	if (SUCCEEDED( hr )) 
	{
		 //WidecharToMultibyte(
		wcstombs(tmp,szThemeName,MAXARRAY);
		wcstombs(tmp2,szSizeBuff,MAXARRAY);
		
		stheme = tmp;  stheme.TrimRight();
		if (!stheme.IsEmpty())
		{
			int FindPos = -1;
			int FindPos2 = -1;
			for(int dd = stheme.GetLength(); dd>1;dd--)
			{
				if(stheme[dd] == '\\')
				{
					FindPos = dd;
					break;
				}
				if (stheme[dd] == '.') FindPos2 = dd;
			}
			if (FindPos > 0) stheme = stheme.Mid(FindPos+1, FindPos2-FindPos-1); 

		}
		stheme.MakeUpper();
		return stheme;
		
	}
	
	*/
	return "";
}

void CMainFrame::RunPhonePad()
{
	CString strPhonepad;
	strPhonepad = "C:\\PhonePAD\\PhonePad.EXE";

	if(!IsFileExist(strPhonepad))
	{
		Axis::MessageBox(this,"폰패드 프로그램이 설치되지 않았습니다.\n폰패드 프로그램을 설치하셔요.",MB_ICONINFORMATION);
		return;
	}
	
	CString mapN;

	CPhonePad* m_phone_dlgs = new CPhonePad(this);
	m_phone_dlgs->main_hwnd = m_hWnd;
	int ret = m_phone_dlgs->DoModal();
	delete m_phone_dlgs;
	if (ret == IDOK)
	{
		mapN = _T("IB877700");
	}
	else if (ret == IDCANCEL)
	{
		mapN = _T("IB878800");
	}
	else
	{
		return;
	}
	//mapN = _T("IB877700");
	m_pMain->m_mapHelper->CreatePopup(mapN, TRIGGERN, winK_POPUP,
		1, 1, CPoint(-1, -1), false);
}

BOOL CMainFrame::IsPhonePad(const char *map)
{
	if (map == nullptr)
		return FALSE;

	return strcmp(map, "IB877700") == 0 ||
		strcmp(map, "IB878800") == 0;
}

void CMainFrame::dnloadAction()
{
	SetTimer(TM_DNINTEREST, 1000, NULL);
	return;
//	CString filePath, fileTemp, m_root, m_name;
//	m_root = Axis::home;
//	m_name = Axis::user;
//	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
//	
//	WritePrivateProfileString("GROUPORDER", "00", "", filePath);
//
//	char* pdata = new char[sizeof(struct _uinfo) + 1];
//	memset(pdata, 0x00, sizeof(struct _uinfo) + 1);
//	memset(pdata, ' ', sizeof(struct _uinfo));
//	struct	_uinfo* puinfo;
//	puinfo = (struct	_uinfo*)pdata;
//	CopyMemory(puinfo->gubn, "MY", 2);
//	puinfo->dirt[0] = 'D';
//	puinfo->cont[0] = 'F';
//	CopyMemory(puinfo->nblc, "00000", 5);
//	sendTR("pihomyst", (char*)puinfo, sz_uinfo, 0, 'Q');
//
//	CString stmp;
//
//	m_slog = "[axis][dnloadaction]";
//	for (int ii = 0; ii < sizeof(struct _uinfo); ii++)
//	{
//		stmp.Format("%c", pdata[ii]);
//		m_slog += stmp;
//	}
//
//	m_slog += "|";
//OutputDebugString(m_slog);
//
//	ChangeLogo();
//	m_info_interest = new CInfo_Interest(this);
//	m_info_interest->DoModal();
	
}

void CMainFrame::parsingOubsDN(char *datB, int datL)
{
	//if (datL < sz_updn)
	//{
	//	TRACE("dnload : response length error....[%d]\n", datL);
	//	SetTimer(TM_DNINTEREST, 1000, NULL);
	//	return;
	//}
	//
	//int	parseL = 0;
	//const struct _updn* updn = (struct _updn *) &datB[0];
	//parseL += sz_updn;

	//CString path;
	//path.Format("%s\\portfolio.log",Axis::home);
	//
	//if (updn->uinfo.retc[0] == 'E')
	//{
	//	CString s;
	//	s.Format("dnload : response error....[%s][%s]\n", CString(updn->ginfo.gnam, sizeof(updn->ginfo.gnam)), CString(updn->uinfo.emsg, sizeof(updn->uinfo.emsg)));
	//	WriteFileData(path,s,FALSE);

	//	TRACE("dnload : response error....[%s]\n", CString(updn->uinfo.emsg, sizeof(updn->uinfo.emsg)));
	//	//updnWaitAction(1); 
	//	//load_tabview();
	//	SetTimer(TM_DNINTEREST, 1000, NULL);
	//	return;
	//}
	//
	//CString	gnox, gnam, jcnt;
	//
	//gnox = CString(updn->ginfo.gnox, sizeof(updn->ginfo.gnox)); 
	//gnox.TrimRight();
	//
	//gnam = CString(updn->ginfo.gnam, sizeof(updn->ginfo.gnam)); 
	//gnam.TrimRight();
	//jcnt = CString(updn->ginfo.jrec, sizeof(updn->ginfo.jrec)); 
	//jcnt.TrimRight();
	//
	////TRACE("kwon [%s] [%s] [%s] [%c]\n", gnox, gnam, jcnt, updn->uinfo.cont[0]);
	////CString tmp; tmp.Format("kwon [%s] [%s] [%s] [%c] [%d] [%d]\n", gnox, gnam, jcnt, updn->uinfo.cont[0] , parseL + (atoi(jcnt) * sz_jinfo) , datL);
	//
	////WriteFileData(path,tmp,FALSE);
	//
	//if (parseL + (atoi(jcnt) * sz_jinfo) <= datL)
	//	SetDnloadData(atoi(gnox), gnam, atoi(jcnt), &datB[parseL]);

	//if (updn->uinfo.cont[0] != 'L')
	//{
	//	//vc2019
	//	char* pdata = new char[sizeof(struct _uinfo) + 1];
	//	memset(pdata, 0x00, sizeof(struct _uinfo) + 1);
	//	memset(pdata, ' ', sizeof(struct _uinfo));
	//	struct	_uinfo* puinfo;
	//	puinfo = (struct	_uinfo*)pdata;
	//	//CopyMemory((char*)&puinfo, (char*)&updn->uinfo, sz_uinfo);  //vc2019
	//	
	//	CopyMemory((char*)&puinfo->gubn, (char*)&updn->uinfo.gubn, 2);
	//	CopyMemory((char*)&puinfo->dirt, (char*)&updn->uinfo.dirt, 1);
	//	CopyMemory((char*)&puinfo->cont, (char*)&updn->uinfo.cont, 1);
	//	CopyMemory((char*)&puinfo->name, (char*)&updn->uinfo.name, 80);
	//	CopyMemory((char*)&puinfo->nblc, (char*)&updn->uinfo.nblc, 5);
	//	CopyMemory((char*)&puinfo->retc, (char*)&updn->uinfo.retc, 1);


	//	OutputDebugString("[axis] parsingOubsDN 3\n");

	//	sendTR("pihomyst", (char*)puinfo, sz_uinfo, 0, 'Q');
	//	return;
	//}

	//if (m_bdnInterest)
	//{
	//	SetTimer(TM_DNINTEREST, 1000, NULL);
	//}
}

int CMainFrame::SetDnloadData(int gno, CString gname, int count, char *datB)
{
	//int	readL = 0;
	//char	readB[512];
	//CString	string = _T("");

	//const CTime time = CTime::GetCurrentTime();

	//CString filePath, fileTemp, m_root, m_name;
	//m_root = Axis::home;
	//m_name = Axis::user;
	//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	//fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	//
	//CString	section; 
	//section.Format("%02d", gno);
	//
	//readL = GetPrivateProfileString("GROUPORDER", "00", "", readB, sizeof(readB), filePath);
	//string = CString(readB, readL); string += section; string += ";"; 
	//WritePrivateProfileString("GROUPORDER", "00", string, filePath);
	//WritePrivateProfileString("GROUPORDER", "00", string, fileTemp);
	//
	//WritePrivateProfileString("GROUPNAME", section, gname, filePath); WritePrivateProfileString(NULL, NULL, NULL, filePath);
	//WritePrivateProfileString("GROUPNAME", section, gname, fileTemp); WritePrivateProfileString(NULL, NULL, NULL, fileTemp);
	//
	//filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);
	//CString fileBook;
	//fileBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_name, gno);

	//struct  _bookmarkinfo* bInfo{};
	//bool isfile = false;
	//CFile	fileB;
	//if (IsFileExist(fileBook))
	//{
	//	fileB.Open(fileBook, CFile::modeRead);
	//			
	//	if (fileB.m_hFile != CFile::hFileNull) 
	//	{
	//		isfile = true;
	//	}
	//}
	//
	//
	//CFile	fileH(filePath, CFile::modeWrite|CFile::modeCreate);
	//if (fileH.m_hFile == CFile::hFileNull) return 0;
	//
	//int	parseL = 0;
	//CString	code(_T(""));
	//CString name(_T(""));
	//
	////	char	wB[64] = {' ', };
	//CString wB;
	//
	////	char*	xxP;
	//CString xxP;
	//
	//struct	_inters	inters {};
	//struct	_jinfo* jinfo{};
	//CString cname;
	//for (int ii = 0; ii < count; ii++)
	//{
	//	jinfo = (struct _jinfo *) &datB[parseL];
	//	parseL += sz_jinfo;
	//	
	//	memset((void*)&inters, 0, sz_inters);
	//	
	//	inters.gubn[0] = jinfo->gubn[0];
	//	CopyMemory(inters.code, jinfo->code, sizeof(jinfo->code));
	//	CopyMemory(inters.xprc, jinfo->xprc, sizeof(jinfo->xprc));
	//	CopyMemory(inters.xnum, jinfo->xnum, sizeof(jinfo->xnum));
	//	//CopyMemory(inters.name, jinfo->hnam, sizeof(jinfo->hnam));
	//	
	//	code = CString(inters.code, sizeof(inters.code)); 
	//	
	//	if(code.Left(1) != " ")
	//	{
	//		code.TrimRight();
	//	}
	//	

	//	xxP = GetCodeName(code);

	//	if(xxP.GetLength() > 32)
	//		xxP = xxP.Mid(0,32);
	//	
	//	name = xxP; 
	//	name.TrimLeft(); 
	//	name.TrimRight();

	//	if(code.Left(1) == " ")
	//	{
	//		name = "      ";
	//	}
	//	if (isfile)
	//	{
	//		
	//		bInfo = (_bookmarkinfo *) new char[sizeof(_bookmarkinfo)];
	//		ZeroMemory(bInfo, sizeof(_bookmarkinfo));

	//		readL = fileB.Read(bInfo, sizeof(_bookmarkinfo));

	//		if(readL < sizeof(_bookmarkinfo))
	//		{
	//			delete bInfo;
	//		}
	//		else
	//		{
	//			CString temp = CString((jinfo->code));
	//			
	//			temp = temp.Left(12);
	//			temp.TrimRight();
	//			CString temp2 = CString(bInfo->code, sizeof(bInfo->code));
	//			
	//			temp2.TrimRight();
	//		
	//			if(strcmp(temp, temp2) == 0)
	//			{
	//				if (code.Left(1) == 'm')
	//				{
	//					name = CString(bInfo->name, sizeof(bInfo->name));
	//				}
	//		
	//			}
	//		}
	//	}	
	//	
	//	strncpy_s(inters.name, sizeof(inters.name), (LPSTR)(LPCTSTR)name, name.GetLength());
	//	
	//	fileH.Write(&inters, sz_inters);
	//}
	//
	//fileH.Close(); 
	//return count;
return 0;
}

void CMainFrame::UploadFile()
{
	CString		file, fileN, str, zipfile;
	CFile		hFile;
	char		sndB[UPMAXSIZE]{};
	bool m_wait = false;
	zipfile = Compress("ordcfg.ini");
	if (!zipfile.IsEmpty())
	{
		m_sfilelist.Add(zipfile);
		str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
		m_removelist.Add(str);
	}
		
	for (;;)
	{
		if (m_sfilelist.GetSize() <= 0)
		{
			m_wait = false;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			str.LoadString(ST_USEINFO_TEXT_ENDWORK);
			
			removeCompressfile();
			return;
		}
		fileN = m_sfilelist.GetAt(0);
		m_sfilelist.RemoveAt(0);		
		
		if (fileN.Find(".ini") >= 0 || fileN.Find(".mmo") >= 0)
			file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, fileN);
		else
		{   //신차트 압축업로드시 수정 필요 - 김덕기
			if (fileN.Find("IB7") >= 0 || fileN.Find("ib7") >= 0)
				file.Format("%s\\gex\\%s\\%s", Axis::home, Axis::user, fileN);
// 			else if(fileN.Find("Chart.zip") >= 0)
// 				file.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, fileN);
			else if(fileN.Find("dat") >= 0 || fileN.Find("cht") >= 0)
				file.Format("%s\\user\\%s\\Chart\\%s", Axis::home, Axis::user, fileN);
		}
			//file.Format("%s\\gex\\%s\\%s", Axis::home, Axis::user, fileN);
		
		if (!hFile.Open(file, CFile::modeRead | CFile::typeBinary))
			continue;
		
		const int fileL = (int)hFile.GetLength();
		const int fSize = L_usr+fileL;
		if (fileL > UPMAXSIZE)
		{
			hFile.Close();
			str.Format("%s이 전송 사이즈를 초과 하였습니다.", fileN);
			Axis::MessageBox(this, str, MB_ICONSTOP);
			continue;
		}
		
		_userTH*	userTH = (_userTH *) sndB;
		usrinfo*	info = (usrinfo *) &sndB[L_userTH];
		
		memset(sndB, ' ', fSize);
		hFile.Read(&sndB[L_userTH + L_usr], fileL);
		hFile.Close();
		
		sprintf(userTH->trc, "PMHOUPDN");
		userTH->key = 'L';
		userTH->stat = US_PASS;
		
		sprintf(info->gubn, "UD");
		info->dirt[0] = 'U';	// pc -> host
		CopyMemory(info->name, fileN, fileN.GetLength());
		sprintf(info->nblc, "%05d", fileL);
		info->retc[0] = '0';
		
		BOOL	rc = FALSE;
		m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), (long)sndB, fSize);
	}
}

CString CMainFrame::Compress(CString target, CString path)
{
	//if (!m_upload)	return target + ".zip";
	
	CString		zipFile, srcFile;
	
	if (path.IsEmpty())
		srcFile.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, target);
	else
		srcFile.Format("%s\\%s", path, target);
	
	// does zip source file exist?
	if (_taccess(srcFile, 04))	// can read?
	{
		CString	str;
		str.Format("%s\t%s", srcFile, "파일을 찾을 수 없습니다.");
		//m_errfile.Add(str);
		return _T("");
	}
	
	// use only the file name for zip file entry
	const TCHAR * cp = _tcsrchr(srcFile, _T('\\')); 
	if (cp == NULL)
		cp = (TCHAR *)(const char *)srcFile;
	else
		cp++;
	
	zipFile.Format("%s.zip", srcFile);
	HZIP hz = CreateZip((void *)(const char *) zipFile, 0, ZIP_FILENAME);
	if (hz)
	{
		const ZRESULT zr = ZipAdd(hz, cp, (void *)(const char *)srcFile, 0, ZIP_FILENAME);
		CloseZip(hz);
		
		// did add work?
		if (zr != ZR_OK)
			zipFile.Empty();
		else	zipFile.Format("%s.zip", target);
	}
	
	return zipFile;
}

void CMainFrame::CompressChart(CString path)
{
	CString		zipfile;
	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	
	CString		allfile, filename;

	allfile.Format("%s\\*.zip", path);
	hFind = FindFirstFile(allfile, &FindFileData);
	
	while (hFind != INVALID_HANDLE_VALUE)
	{
		filename.Format("%s\\%s", path, FindFileData.cFileName);
		::DeleteFile(filename);
		if (!FindNextFile(hFind, &FindFileData))	break;
	}

	//**allfile.Format("%s\\*.c*", path);
	allfile.Format("%s\\IB7*.*", path);
	hFind = FindFirstFile(allfile, &FindFileData);
	
	while (hFind != INVALID_HANDLE_VALUE)
	{
		filename.Format("%s\\%s", path, FindFileData.cFileName);
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			TCHAR * cp =(TCHAR *) _tcsrchr(filename, _T('\\'));
			
			if (cp == NULL)
				cp = (TCHAR *)(const char*)filename;
			else
				cp++;

			zipfile = Compress(cp, path);		// 사용자 환경설정
			
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);				
				m_removelist.Add(filename + ".zip");	
			}	
		}

		if (!FindNextFile(hFind, &FindFileData))	break;
	}	
}

void CMainFrame::removeCompressfile()
{
	CString	file;
	
	for (int ii = 0; ii < m_removelist.GetSize(); ii++)
		::DeleteFile(m_removelist.GetAt(ii));
	m_removelist.RemoveAll();
}

void CMainFrame::SendConfig(int igubn)
{
	struct	_pidouini_mid	uini;	
	//FillMemory((char*) &uini, sizeof(_pidouini_mid) + 1, ' ');
	FillMemory((char*)&uini, sizeof(_pidouini_mid) , ' ');

	CString s;
	CString sval, sdat;

	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	CopyMemory(uini.gubn, "I", sizeof(uini.gubn));
	CopyMemory(uini.item.usid, Axis::userID, sizeof(uini.item.usid));   //아이디
#ifdef DF_USE_CPLUS17
	CopyMemory(uini.item.senm, "OPEN", strlen("OPEN"));		    //섹션명
#else
	CopyMemory(uini.item.senm, "OPEN", sizeof(uini.item.senm));		    //섹션명
#endif

	CopyMemory(uini.item.skey, sdat, sizeof(uini.item.skey));			//날짜
	CopyMemory(uini.item.date, sdat.Left(8), sizeof(uini.item.date));   //업데이트 일자

	if(igubn == 0)
	{
		if(m_iClickCROWDBanner == 0)
			return;

		sval.Format("%d", m_iClickCROWDBanner);
#ifdef DF_USE_CPLUS17
		CopyMemory(uini.item.innm, "CROWD", strlen("CROWD"));
#else
		CopyMemory(uini.item.innm, "CROWD", sizeof(uini.item.innm));
#endif
		CopyMemory(uini.item.valu, sval, sizeof(uini.item.valu));		//CROWD 클릭횟수

CString s;
s.Format("Sendconfig Main Sendconfig 클라우드 m_iClickCROWDBanner=[%d] \n",m_iClickCROWDBanner);
OutputDebugString(s);
	}
	else if(igubn == 1)
	{
		if(m_iClickEPBBanner == 0)
			return;

		sval.Format("%d", m_iClickEPBBanner);
#ifdef DF_USE_CPLUS17
		CopyMemory(uini.item.innm, "E-PB", strlen("E-PB"));
#else
		CopyMemory(uini.item.innm, "E-PB", sizeof(uini.item.innm));
#endif
		
		CopyMemory(uini.item.valu, sval, sizeof(uini.item.valu));		//E-PB 클릭횟수

CString s;
s.Format("Sendconfig Main Sendconfig EPB m_iClickEPBBanner=[%d] \n",m_iClickEPBBanner);
OutputDebugString(s);
	}

	sendTR("pidouini", (char*)&uini, sz_pidouini, 0, 'N'); 
}

void CMainFrame::SendForceIP()
{
	
	struct	_pidouini_mid	uini;
	
	FillMemory((char*) &uini, sizeof(_pidouini_mid), ' ');
	CString sval, sdat;
	
	//==================================================
	CString s;
	// 	s.Format("SEND CONFIG : [%s]\n",sval);
	// 	OutputDebugString(s);
	sval = m_forceIP;
	
	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	CString fname = Axis::user + ".ini";
	CopyMemory(uini.gubn, "I", sizeof(uini.gubn));
	CopyMemory(uini.item.usid, Axis::userID, sizeof(uini.item.usid));
	CopyMemory(uini.item.innm, fname, sizeof(uini.item.innm));
#ifdef DF_USE_CPLUS17
	CopyMemory(uini.item.senm, "FORCEIP", strlen("FORCEIP"));
	CopyMemory(uini.item.skey, "IP", strlen("IP"));
#else
	CopyMemory(uini.item.senm, "FORCEIP", sizeof(uini.item.senm));
	CopyMemory(uini.item.skey, "IP", sizeof(uini.item.skey));
#endif
	
	CopyMemory(uini.item.valu, sval, sval.GetLength());
	CopyMemory(uini.item.date, sdat, sizeof(uini.item.date));
	
	sendTR("pidouini", (char*)&uini, sz_pidouini, 0, 'N');
}

void CMainFrame::SendLastStat(CString vsN, CString sval)
{
	
	struct	_pidouini_mid	uini;
	
	FillMemory((char*) &uini, sizeof(_pidouini_mid), ' ');
	CString sdat;

	//sval.Format("%s INSTALL PATH [%s]\n",str,Axis::home);
	//==================================================
	CString s;
	// 	s.Format("SEND CONFIG : [%s]\n",sval);
	// 	OutputDebugString(s);

	if(sval == "")
	{
		sval = "Nothing";
	}
	
	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	CString fname = Axis::user + ".ini";
	CopyMemory(uini.gubn, "I", sizeof(uini.gubn));
	CopyMemory(uini.item.usid, Axis::userID, sizeof(uini.item.usid));
	CopyMemory(uini.item.innm, fname, sizeof(uini.item.innm));
#ifdef DF_USE_CPLUS17
	CopyMemory(uini.item.senm, "LASTSTAT", strlen("LASTSTAT"));
#else
	CopyMemory(uini.item.senm, "LASTSTAT", sizeof(uini.item.senm));
#endif
	CopyMemory(uini.item.skey, vsN, sizeof(uini.item.skey));
	CopyMemory(uini.item.valu, sval, sizeof(uini.item.valu));
	CopyMemory(uini.item.date, sdat, sizeof(uini.item.date));
	
	sendTR("pidouini", (char*)&uini, sz_pidouini, 0, 'N');
}

void CMainFrame::SendLastStat(char* strData)
{	
	sendTR("pidouini", strData, strlen(strData), 0, 'N');
}

void CMainFrame::SendCPUInfo()
{
	KillTimer(TM_RTSQUEUE);
	
	//sval.Format("%s INSTALL PATH [%s]\n",str,Axis::home);
	//==================================================
	CString		filename,fileN,sdat;
	
	CFile cfile;
	
	filename.Format("%s\\%s\\%s\\rscinfo", Axis::home, USRDIR, Axis::user);
	
	if(_taccess(filename,04)) return;
	
	if (!cfile.Open(filename, CFile::modeRead | CFile::typeBinary))
		return;
	
	const int fileL = (int)cfile.GetLength();
	const int fSize = L_usr+fileL;
	if (fileL > UPMAXSIZE)
	{
		::DeleteFile(filename);
		return;
	}

	char		sndB[UPMAXSIZE]{};
	
	_userTH*	userTH = (_userTH *) sndB;
	usrinfo*	info = (usrinfo *) &sndB[L_userTH];
	
	memset(sndB, ' ', fSize);
	cfile.Read(&sndB[L_userTH + L_usr], fileL);
	cfile.Close();
	
	sprintf(userTH->trc, "PMHOUPDN");
	userTH->key = 'L';
	userTH->stat = US_PASS;

	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	
	fileN.Format("rscinfo_%s_%s",Axis::userID,sdat);

	sprintf(info->gubn, "UD");
	info->dirt[0] = 'U';	// pc -> host
	CopyMemory(info->name, fileN, fileN.GetLength());
	sprintf(info->nblc, "%05d", fileL);
	info->retc[0] = '0';
	
	BOOL	rc = FALSE;
	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), (long)sndB, fSize);

	::DeleteFile(filename);
}

void CMainFrame::SendFile(CString file)
{
	CString		filename,fileN,sdat;
	
	CFile cfile;
	
	//filename.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user,file);
	
	if(_taccess(file,04)) return;
	
	if (!cfile.Open(file, CFile::modeRead | CFile::typeBinary))
		return;
	
	const int fileL = (int)cfile.GetLength();
	const int fSize = L_usr+fileL;
	if (fileL > UPMAXSIZE)
	{
		return;
	}
	
	char		sndB[UPMAXSIZE]{};
	
	_userTH*	userTH = (_userTH *) sndB;
	usrinfo*	info = (usrinfo *) &sndB[L_userTH];
	
	memset(sndB, ' ', fSize);
	cfile.Read(&sndB[L_userTH + L_usr], fileL);
	cfile.Close();
	
	sprintf(userTH->trc, "PMHOUPDN");
	userTH->key = 'L';
	userTH->stat = US_PASS;
	
	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d-%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(),time.GetHour(),time.GetMinute());

	const int find = file.ReverseFind('\\');
	if(find > -1)
		file = file.Right(file.GetLength() - (find+1));
	
	fileN.Format("%s_%s_%s",m_ipAddr,sdat,file);
	
	sprintf(info->gubn, "UD");
	info->dirt[0] = 'U';	// pc -> host
	CopyMemory(info->name, fileN, fileN.GetLength());
	sprintf(info->nblc, "%05d", fileL);
	info->retc[0] = '0';
	
	BOOL	rc = FALSE;
	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), (long)sndB, fSize);
}

//2012.08.22 김덕기 - 2000번에서 잔고조회를 얼마나 하는지 전송.
//박영태 대리와 함께 작업.2000번 맵 수정(임시)
void CMainFrame::Send2000()
{
	struct	_pidouini_mid	uini;
	
	FillMemory((char*) &uini, sizeof(_pidouini_mid), ' ');
	CString sval, sdat;
	CString str(""), maps;
	CString file;	
	
	//==================================================
	CString s;
	// 	s.Format("SEND CONFIG : [%s]\n",sval);
	// 	OutputDebugString(s);
	
	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	CString fname = "userconf.ini";
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, fname);

	char buff[1024];
	GetPrivateProfileString("Clicktabinfo",sdat,"",buff,sizeof(buff),file);
	str = buff;
	sval = "TAB CHANGE : ";
	sval += str;
	sval += "\t";

	GetPrivateProfileString("Clickbtninfo",sdat,"",buff,sizeof(buff),file);
	str = buff;

	sval += "BUTTON CLICK : ";
	sval += str;
// 	s.Format("SEND 2000 : [%s]\n",sval);
// 	OutputDebugString(s);

	sdat.Remove('-');

	CopyMemory(uini.gubn, "I", sizeof(uini.gubn));
	CopyMemory(uini.item.usid, Axis::userID, sizeof(uini.item.usid));
	CopyMemory(uini.item.innm, fname, sizeof(uini.item.innm));
#ifdef DF_USE_CPLUS17
	CopyMemory(uini.item.senm, "PIBOJNGO", strlen("PIBOJNGO"));
	CopyMemory(uini.item.skey, "click", strlen("click"));
#else
	CopyMemory(uini.item.senm, "PIBOJNGO", sizeof(uini.item.senm));
	CopyMemory(uini.item.skey, "click", sizeof(uini.item.skey));
#endif

	CopyMemory(uini.item.valu, sval, sizeof(uini.item.valu));
	CopyMemory(uini.item.date, sdat, sizeof(uini.item.date));
	
	sendTR("pidouini", (char*)&uini, sz_pidouini, 0, 'N');
}

void CMainFrame::RotateView(int tp)
{
	CString sdat;
	sdat.Format("ROTATEVIEW\t%d", tp);
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setNDC, 0), (LPARAM)(const char*)sdat);
	//OutputDebugString(sdat);
	/*
	if (tp==1)
	{
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setNDC, 0), (LPARAM)(const char*)sdat);

	}
	else
	{

	}
	*/
}

CString CMainFrame::GetCodeName(CString code)
{
	//code = code.Left(12);
	//code.TrimRight();
	//code.TrimLeft();
	//code.Remove(0);
	//
	//const int codeL = code.GetLength();

	//if (codeL == 6)					// kospi code
	//{
	//	_shjcode   hjcode; 
	//	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	//	{
	//		hjcode = m_hjcode.GetAt(ii);
	//		if (code != hjcode.code)
	//			continue;

	//		switch (hjcode.kosd)
	//		{
	//		case jmKOSPI:
	//		case jmKOSDAQ:
	//		case jm3RD:
	//			if (hjcode.ssgb == jmELW)
	//				return hjcode.name;
	//			break;
	//		}
	//		return hjcode.name.Mid(1);
	//	}

	//}
	//else if (codeL == 9)				// 신주인수권 추가
	//{
	//	_shjcode   hjcode; 
	//	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	//	{
	//		hjcode = m_hjcode.GetAt(ii);
	//		if (code != hjcode.code)
	//			continue;
	//		
	//		switch (hjcode.kosd)
	//		{
	//		case jmSINJU:
	//			if (hjcode.ssgb == jmSINJU)
	//				return hjcode.name;
	//			break;
	//		}
	//		return hjcode.name.Mid(1);
	//	}
	//}
	//else if (codeL <= 3)				// upjong code
	//{
	//	struct	upcode	upcode {};
	//	for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
	//	{
	//		upcode = m_upcode.GetAt(ii);
	//		
	//		CString upCodeStr;
	//		int upCodeInt;
	//		upCodeStr.Format("%d%02d", upcode.jgub, upcode.ucod);
	//		upCodeInt = atoi(upCodeStr);
	//		
	//		if (atoi(code) != upCodeInt)
	//			continue;
	//		
	//		return CString(upcode.hnam, UNameLen);
	//	}
	//}
	//else if (code[0] == '1' || code[0] == '4')	// future code
	//{
	//	_sjcode   sjcode;
	//	for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
	//	{
	//		sjcode = m_fjcode.GetAt(ii);
	//		if (code != sjcode.code)
	//			continue;
	//		
	//		return sjcode.name;
	//	}
	//	
	//}
	//else if (code[0] == '2' || code[0] == '3')
	//{
	//	if (code[1] == '0')			// future option code
	//	{
	//		struct ojcode	ojcode {};
	//		for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
	//		{
	//			ojcode = m_ojcode.GetAt(ii);
	//			OutputDebugString("LOAD OPTION CODE");
	//			for (int jj = 0; jj < 11; jj++)
	//			{
	//				if (ojcode.call[jj].yorn != '1')
	//					continue;
	//				if (code != CString(ojcode.call[jj].cod2, OCodeLen))
	//					continue;
	//				
	//				return CString(ojcode.call[jj].hnam, ONameLen);
	//			}
	//			for (int jj = 0; jj < 11; jj++)
	//			{
	//				if (ojcode.put[jj].yorn != '1')
	//					continue;
	//				if (code != CString(ojcode.put[jj].cod2, OCodeLen))
	//					continue;
	//				
	//				return CString(ojcode.put[jj].hnam, ONameLen);
	//			}
	//			OutputDebugString("COMPLETE CODE");
	//		}
	//	}
	//	else					// 
	//	{
	//		struct	pjcode pjcode {};
	//		for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
	//		{
	//			pjcode = m_pjcode.GetAt(ii);
	//			if (code != CString(pjcode.codx, PCodeLen))
	//				continue;
	//			
	//			return CString(pjcode.hnam, sizeof(pjcode.hnam));
	//		}
	//	}
	//}
	
	return _T("");
}


void CMainFrame::loadingHJcode() 
{
	//if (m_hjcode.GetSize() > 0) return;
	//
	//CFile	fileH;
	//CString	filePath;
	//struct  hjcode  hjcode {};
	//struct	_shjcode s_hjcode;
	//
	//filePath.Format("%s/%s/%s", Axis::home, TABDIR, HJCODE);
	//if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	//{
	//	//AfxMessageBox(idTOstring(IDS_EM_HJCODE));
	//	return;
	//}
	//
	//const int countC = (int)fileH.GetLength() / sizeof(struct hjcode);
	//
	//for (int ii = 0; ii < countC; ii++)
	//{
	//	fileH.Read(&hjcode, sizeof(struct hjcode));
	//	
	//	s_hjcode.name = CString(hjcode.hnam, HNameLen);
	//	const int idx = s_hjcode.name.Find('\0');
	//	if (idx != -1) s_hjcode.name = s_hjcode.name.Left(idx);
	//	s_hjcode.name.MakeUpper(); s_hjcode.name.TrimRight();
	//	//		s_hjcode.name = s_hjcode.name.Mid(1, s_hjcode.name.GetLength());
	//	
	//	s_hjcode.symb = CString(hjcode.symb, HSymbolLen);
	//	s_hjcode.symb.TrimRight();
	//	
	//	s_hjcode.ecng = hjcode.ecng;
	//	s_hjcode.size = hjcode.size;
	//	s_hjcode.ucdm = hjcode.ucdm;
	//	s_hjcode.ucds = hjcode.ucds;
	//	s_hjcode.jjug = hjcode.jjug;
	//	s_hjcode.kpgb = hjcode.kpgb;
	//	s_hjcode.ucmd = hjcode.ucmd;
	//	s_hjcode.kosd = (char)hjcode.kosd;
	//	s_hjcode.ssgb = hjcode.ssgb;
	//	s_hjcode.wsgb = hjcode.wsgb;
	//	s_hjcode.jsiz = hjcode.jsiz;
	//	s_hjcode.itgb = hjcode.itgb;
	//	s_hjcode.star = hjcode.star;
	//	s_hjcode.unio = hjcode.unio;
	//	s_hjcode.prmr = hjcode.prmr;
	//	
	//	switch (hjcode.ssgb)
	//	{
	//	case jmSINJU:
	//	case jmSINJS:
	//	case jmHYFND:
	//		s_hjcode.code = CString(hjcode.code, HCodeLen + 2);
	//		break;
	//	default:
	//		s_hjcode.code = CString(hjcode.code, HCodeLen + 2).Mid(1);
	//		break;
	//	}
	//	
	//	switch (s_hjcode.ssgb)
	//	{
	//	case jmHYFND:
	//		//			continue;
	//	default:
	//		m_hjcode.Add(s_hjcode);
	//		break;
	//	}
 //   }
	//
	////이부분 추가
	////QSortCArray( m_hjcode, CompareChar ); 
	//fileH.Close();
}


void CMainFrame::loadingFJcode()
{
	/*if (m_fjcode.GetSize() > 0) return;
	
	CFile	fileH;
	CString	filePath;
	struct  fjcode  fjcode {};
	struct	_sjcode s_jcode;
	
	filePath.Format("%s/%s/%s", Axis::home, TABDIR, FJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		return;
	}
	
	const int countC = (int)fileH.GetLength() / sizeof(struct fjcode);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&fjcode, sizeof(struct fjcode));
		
		s_jcode.code = CString(fjcode.cod2, FCodeLen);
		s_jcode.name = CString(fjcode.hnam, FNameLen);
		
		m_fjcode.Add(s_jcode);
	}
	
	fileH.Close();*/
}
void CMainFrame::loadingOJcode()
{
	//if (m_ojcode.GetSize() > 0) return;
	//
	//CFile	fileH;
	//CString	filePath;
	//
	//struct  ojcodh  ojcodH {};
	//struct  ojcode  ojcode {};
	//
	//filePath.Format("%s/%s/%s", Axis::home, TABDIR, OJCODE);
	//if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	//{
	//	//AfxMessageBox(idTOstring(IDS_EM_OPCODE));
	//	return;
	//}
	//
	//fileH.Read(&ojcodH, sizeof(struct ojcodh));
	//
	//const int countC = (int)(fileH.GetLength() - sizeof(struct ojcodh)) / sizeof(struct ojcode);
	//for (int ii = 0; ii < countC; ii++)
	//{
	//	fileH.Read(&ojcode, sizeof(struct ojcode));
	//	m_ojcode.Add(ojcode);
	//}
	//
	//fileH.Close();
}
void CMainFrame::loadingPJcode()
{
	//if (m_pjcode.GetSize() > 0) return;
	//
	//CFile	fileH;
	//CString	filePath;
	//struct  pjcode  pjcode {};
	//
	//filePath.Format("%s/%s/%s", Axis::home, TABDIR, PJCODE);
	//if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	//{
	//	//AfxMessageBox(idTOstring(IDS_EM_PJCODE));
	//	return;
	//}
	//
	//const int countC = (int)fileH.GetLength() / sizeof(struct pjcode);
	//for (int ii = 0; ii < countC; ii++)
	//{
	//	fileH.Read(&pjcode, sizeof(struct pjcode));
	//	m_pjcode.Add(pjcode);
	//}
	//
	//fileH.Close();
}
void CMainFrame::loadingUPcode()
{
	//if (m_upcode.GetSize() > 0) return;
	//
	//CFile	fileH;
	//CString	filePath;
	//struct  upcode  upcode {};
	//
	//filePath.Format("%s/%s/%s", Axis::home, TABDIR, UPCODE);
	//if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	//{
	//	//AfxMessageBox(idTOstring(IDS_EM_UPCODE));
	//	return;
	//}
	//
	//const int countC = (int)fileH.GetLength() / sizeof(struct upcode);
	//for (int ii = 0; ii < countC; ii++)
	//{
	//	fileH.Read(&upcode, sizeof(struct upcode));
	//	m_upcode.Add(upcode);
	//}
	//
	//fileH.Close();
}
void CMainFrame::loadingELWcode()
{
	///*if (m_elwbase.GetSize() > 0)
	//	return;
	//
	//int	codeN{};
	//CFile	file;
	//_sjcode	sjcode;
	//struct  elwcode  ELWCode {};
	//
	//CString path = Axis::home + "\\tab\\elwcod.dat";
	//if (!file.Open(path, CFile::modeRead|CFile::typeBinary))	return;
	//
	//codeN = (int)file.GetLength() / sizeof(struct elwcode);
	//
	//bool	bFound{};
	//for (int ii = 0; ii < codeN; ii++)
	//{
	//	file.Read(&ELWCode, sizeof(struct elwcode));
	//	
	//	for (int jj = 0; jj < 5; jj ++)
	//	{
	//		CString bcode = CString((char*)ELWCode.gcjs[jj], 12);
	//		bcode.TrimRight();
	//		if (bcode == "") break;
	//		
	//		bcode = bcode.Mid(1);
	//		bFound = false;
	//		for (int kk = 0; kk < m_elwbase.GetSize(); kk++)
	//		{
	//			if (bcode.CompareNoCase(m_elwbase.GetAt(kk).code) == 0)
	//			{
	//				bFound = true;
	//				break;
	//			}
	//		}
	//		
	//		if (!bFound)
	//		{
	//			CString	bname = GetCodeName(bcode);
	//			if (!bname.IsEmpty())	
	//				;
	//			else if (bcode.CompareNoCase("OSPI200") == 0)
	//				continue;
	//			else if (bcode.CompareNoCase("ASKET") == 0)
	//				break;
	//			
	//			sjcode.code = bcode;
	//			sjcode.name = bname;
	//			sjcode.name.TrimRight();
	//			
	//			m_elwbase.Add(sjcode);
	//		}
	//	}
	//}
	//
	//file.Close();*/
}

void CMainFrame::RunFOConfig()
{
	m_mapHelper->CreateChild("IB0000B2", 0);
}

void CMainFrame::load_secure_agree(BOOL bAOS, BOOL bFirewall, BOOL bKeysecure)
{
	struct	_pidouini_mid mid;
	memset(&mid, 0, sizeof(mid));
	mid.gubn[0] = 'Q';
	memcpy(mid.item.usid, (LPCSTR)Axis::userID, Axis::userID.GetLength());
	memcpy(mid.item.innm, "AGREEMENT", 9);
	memcpy(mid.item.senm, "SECURETOOLS", 11);
	memcpy(mid.item.skey, "DISABLE", 7);

	sendTR("pidouini", (char*)&mid, sizeof(mid), 0, 'P');
}

void CMainFrame::processSecureTool(char *data, int dlen)
{
	const struct _pidouini_item *mod = (struct _pidouini_item*)data;

	// 보안프로그램을 사용하지 않겟다고 동의하지 않았으면! (Disable에 대한 OK)
	if (memcmp(mod->valu, "OK", 2)!=0)
		PostMessage(WM_SECUREDLG, 0, 0);
}


LRESULT CMainFrame::OnSecureDlg(WPARAM wParam, LPARAM lParam)
{
	CString file;
	char	buf[512];
	file.Format("%s\\%s\\axisENC.ini", Axis::home, "tab");
	DWORD dw = GetPrivateProfileString("USE", "OnSecureDlg", "1", buf, sizeof(buf), file);
	CString stemp{};
	stemp.Format("%s", buf);
	stemp.TrimRight();
	if (stemp == "0")
		return 0;

	const BOOL pcAOS = AfxGetApp()->GetProfileInt(INFORMATION, "AOS", 1); //modi ASTx 202409 이제 아래함수는 호출될 일이 없다
	const BOOL pcFirewall = AfxGetApp()->GetProfileInt(INFORMATION,   "PCFirewall", 0);
	const BOOL pcKeyProtect = AfxGetApp()->GetProfileInt(ENVIRONMENT, "KeyProtect", 0);
	CSecureDlg dlg(this, pcAOS, pcFirewall, pcKeyProtect);
	if (dlg.DoModal()==IDOK)
	{
		if(!pcAOS)
		{
			// 			if(!isRunningSDKPd(AOSSDK_SERVICE_CODE_SB))
			// 			{
			// 				if (initAOSSDK())
			// 					initSBSDK();
			// 			}
			if(!m_bNoProtect && initASTx())
			{
				startPB();
				startFW();
				startAK();
			}
		}
		
		AfxGetApp()->WriteProfileInt(INFORMATION, "AOS", 1);    //modi ASTx 202409 이제 아래함수는 호출될 일이 없다
		AfxGetApp()->WriteProfileInt(INFORMATION, "PCFirewall", 1);
		AfxGetApp()->WriteProfileInt(ENVIRONMENT, "KeyProtect", 1);
	}
	else
	{
		if (dlg.m_bNoRetry)
		{
			const CTime time = CTime::GetCurrentTime();
			struct	_pidouini_mid mid;
			memset(&mid, 0, sizeof(mid));
			mid.gubn[0] = 'I';
			memcpy(mid.item.usid, (LPCSTR)Axis::userID, Axis::userID.GetLength());
#ifdef DF_USE_CPLUS17
			memcpy(mid.item.innm, "AGREEMENT",strlen("AGREEMENT"));
			memcpy(mid.item.senm, "SECURETOOLS", strlen("SECURETOOLS"));
			memcpy(mid.item.skey, "DISABLE", strlen("DISABLE"));
#else
			memcpy(mid.item.innm, "AGREEMENT", 9);
			memcpy(mid.item.senm, "SECURETOOLS", 11);
			memcpy(mid.item.skey, "DISABLE", 7);
#endif
			
			sprintf(mid.item.valu, "OK - %s", time.Format("[%Y/%m/%d][%H:%M:%S]"));
			sendTR("pidouini", (char*)&mid, sizeof(mid), 0, 'p');
		}
	}
	return 0;
}

void CMainFrame::os_report()
{
	//OSVERSIONINFO info; 
	OSVERSIONINFOEX info{};
	SYSTEM_INFO sysinfo{};
	MEMORYSTATUS memStatus{};

	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	//GetVersionEx(&info);
	GetVersion(&info);

	GetSystemInfo(&sysinfo);

	memset(&memStatus,sizeof(MEMORYSTATUS),0);
	memStatus.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&memStatus);

	int IMemTotal;

	IMemTotal = memStatus.dwTotalPhys / 1024;

	IMemTotal = IMemTotal / 1024;
	
	CString sdat;
	CTime	time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

	CString		skinName = GetSkinName();
	
	struct	_pidouini_mid mid;
	memset(&mid, 0, sizeof(mid));
	mid.gubn[0] = 'I';
	memcpy(mid.item.usid, (LPCSTR)Axis::userID, Axis::userID.GetLength());
#ifdef DF_USE_CPLUS17
	memcpy(mid.item.innm, "INFO", strlen("INFO"));
	memcpy(mid.item.senm, "OS", strlen("OS"));
#else
	memcpy(mid.item.innm, "INFO", 8);
	memcpy(mid.item.senm, "OS", 10);
#endif
	
	memcpy(mid.item.skey, "VERSION", 7);
// 	sprintf(mid.item.valu, "VER[%d.%d] BUILD[%d] PLATFORM[%d] OS[%s] OS BIT[%s] CPU[%d] CPU NUMBER[%d] MEMORY PHYS[%dM] SKIN [%s]", 
// 		info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber,
// 		info.dwPlatformId, m_sysInfo->GetWindowInfo(),IsWow64(),m_sysInfo->GetCPUInfo(),sysinfo.dwNumberOfProcessors,m_sysInfo->GetMemoryInfo(),skinName);
	sprintf(mid.item.valu, "VER[%s] BUILD[%d] PLATFORM[%d] OS[%s] OS BIT[%s] CPU[%s] CPU NUMBER[%d] MEMORY PHYS[%sM] SKIN [%s] network=[%s]",
		m_sysInfo->GetWindowVersion(), info.dwBuildNumber,
		info.dwPlatformId, m_sysInfo->GetWindowInfo(), IsWow64(), m_sysInfo->GetCPUInfo(), sysinfo.dwNumberOfProcessors, m_sysInfo->GetMemoryInfo(), skinName,
		GetCurrentNetType(TRUE) == 0 ? "none" : GetCurrentNetType(TRUE) == 1 ? "wifi" : "WIRED");
	memcpy(mid.item.date, sdat, 8);

// 	CString s;
// 	s.Format("OS REPORT : [%s] [%s]\n",sdat,mid.item.valu);
// 	OutputDebugString(s);

	sendTR("pidouini", (char*)&mid, sizeof(mid), 0, 'p');
}

// 2014.03.17 dkkim
// 64 bit 구분
CString CMainFrame::IsWow64()
{
	CString sResult = "UNKNOWN";
	BOOL bIsWow64 = FALSE;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if(fnIsWow64Process != NULL)
	{
		if(!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			return sResult;
		} 
	}
	
	if(bIsWow64 == FALSE)
	{
		sResult = "32 Bit";
	}
	else
	{
		sResult = "64 Bit";
	}

	return sResult;
}

void CMainFrame::checkFirewall()
{
	CRegKey reg;

	const LPCTSTR keys[]  = {
		_T("Software\\IBK_STAFF\\AXIS Workstation V04.00\\Information"),			// 개발 직원용 
		_T("Software\\IBKMAC_STAFF\\AXIS Workstation V04.00\\Information"),			// 리얼 직원용
		NULL
	};

	/* 직원용 HTS가 설치되어 있을경우 방화벽설정을 강제 해제한다 */
	for(int n=0; keys[n]!=NULL; ++n)
	{
		if (reg.Open(HKEY_CURRENT_USER, keys[n])==ERROR_SUCCESS)
		{
			AfxGetApp()->WriteProfileInt(INFORMATION, "PCFirewall", 0);
			reg.Close();
			return;
		}
	}

	/* 직원이 로그인한 경우 방화벽 설정을 강제 해제한다 */
	if (!Axis::isCustomer)
	{
		AfxGetApp()->WriteProfileInt(INFORMATION, "PCFirewall", 0);
		return;
	}
}

LONG CMainFrame::OnHotKey(WPARAM wp, LPARAM lp)
{
	if(m_bMiniMode)
		return 0;
	if  (IsWindowVisible())
	{
		if (!m_arSDI[m_vsN].IsEmpty())
		{
			int key{};
			CSChild* pChild{};
			POSITION pos = m_arSDI[m_vsN].GetStartPosition();
			while(pos!=NULL)
			{	
				m_arSDI[m_vsN].GetNextAssoc(pos, key, pChild);
				pChild->ShowWindow(SW_HIDE);
			}
		}
		if (!m_arMPOP.IsEmpty())
		{
			int key{};
			CMPop* pPop{};
			POSITION pos = m_arMPOP.GetStartPosition();
			while(pos!=NULL)
			{
				m_arMPOP.GetNextAssoc(pos, key, pPop);
				pPop->ShowWindow(SW_HIDE);
			}
		}
		ShowWindow(SW_HIDE);
	}
	else
	{
		if (!m_arSDI[m_vsN].IsEmpty())
		{
			int key{};
			CSChild* pChild{};
			POSITION pos = m_arSDI[m_vsN].GetStartPosition();
			while(pos!=NULL)
			{	
				m_arSDI[m_vsN].GetNextAssoc(pos, key, pChild);
				if(pChild->m_mapN != "IB0000X8")
					pChild->ShowWindow(SW_SHOW);
			}
		}
		if (!m_arMPOP.IsEmpty())
		{
			int key{};
			CMPop* pPop{};
			POSITION pos = m_arMPOP.GetStartPosition();
			while(pos!=NULL)
			{
				m_arMPOP.GetNextAssoc(pos, key, pPop);
				pPop->ShowWindow(SW_SHOW);
			}
		}
		ShowWindow(SW_SHOW);
	}
	return 0;
}

void CMainFrame::LoadSecureTools()
{	
#ifdef DF_GLBFILE_CNVS
	CString tmps, temps;
	tmps = m_axis->GetProfileString(INFORMATION, "Port", Format("%d", m_bCustomer ? portProxy : portEmployee));
	const UINT	port = atoi(tmps);

	CString fname;
	fname = Axis::home + "\\tab\\AXGLB.INI";
	CFileFind finder;
	if (port == portProxy)
	{
		if (finder.FindFile(fname))
		{
			WritePrivateProfileString("Server_PORT", "00", "80", fname);
			WritePrivateProfileString("Server_PORT", "01", "80", fname);
		}
	}
	else
	{
		if (finder.FindFile(fname))
		{
			WritePrivateProfileString("Server_PORT", "00", "15101", fname);
			WritePrivateProfileString("Server_PORT", "01", "15201", fname);
		}
	}

	char buf[16]{};
	DWORD dw = GetPrivateProfileString("Server_PORT", "00", "", buf, sizeof(buf), fname);
	tmps.Format("%s", buf); tmps.TrimRight();
	
	memset(buf, 0x00, 16);
	dw = GetPrivateProfileString("Server_PORT", "01", "", buf, sizeof(buf), fname);
	temps.Format("%s", buf); temps.TrimRight();

	if (tmps.GetLength() > 0 && temps.GetLength() > 0)
	{
m_slog.Format("[AXIS] LoadSecureTools   port=[%s][%s]\n ", tmps, temps);
OutputDebugString(m_slog);
WriteLog(m_slog);
	}
	else
	{
m_slog.Format("[AXIS] LoadSecureTools  file error");
OutputDebugString(m_slog);
	}

#endif

	DWORD dwError = STSDKEX_ERROR_SUCCESS;
	
	//modi ASTx 202409  무조건 실행
	//const BOOL pcAOS = AfxGetApp()->GetProfileInt(INFORMATION, "AOS", 1);
	BOOL pcAOS = TRUE;

	if(pcAOS && Axis::isCustomer)
	{
		if(m_bNoProtect)
			return;
		
		if(m_axConnect)
		{
			m_axConnect->SetGuide("메모리 보호 프로그램을 설정합니다...");
		}
		
		dwError = STSDKEX_IsInstalled();
		if(STSDKEX_ERROR_PRODUCT_NOT_INSTALLED == dwError)
		{
			installASTx();
		}

		const int AOSRESTART = AfxGetApp()->GetProfileInt(INFORMATION, "AOSRESTART", 0);
		if(AOSRESTART == 1)
			AfxGetApp()->WriteProfileInt(INFORMATION, "AOSRESTART", 0);
		
		m_axConnect->SetGuide("보안 프로그램을 설정합니다...");
		
		if(m_bSdkInitialized || initASTx())
		{
			startPB();
		}
	}
	else
	{
		if(m_bNoProtect)
			return;
		
		m_axConnect->SetGuide("보안 프로그램을 해제합니다...");
		
		if(uninitASTx())
		{
			stopPB();
			// 			stopFW();
			// 			stopAK();
		}
	}
	
	if(m_axConnect)
	{
		m_axConnect->SetGuide("방화벽 프로그램을 설정합니다...");
	}
	
	//내부직원들에게는 보안모듈 실행 안되게 함
	const BOOL pcFirewall = AfxGetApp()->GetProfileInt(INFORMATION, "PCFirewall", 0);
	(pcFirewall && Axis::isCustomer) ? startFW() : stopFW();
	
	if(m_axConnect)
	{
		m_axConnect->SetGuide("키보드 보안 프로그램을 설정합니다...");
	}
	
	const CAxisApp *app = (CAxisApp*)m_axis;
	const BOOL pcKeyProtect = AfxGetApp()->GetProfileInt(ENVIRONMENT, "KeyProtect", 0);
	(pcKeyProtect) ? startAK() : stopAK();
}

LONG CMainFrame::OnSecureChange( WPARAM wp, LPARAM lp )
{
	LoadSecureTools();

	if(m_bUseNewLogin)
	{
		if(m_axConnect)
		{
			m_axConnect->SetGuide("환경 설정을 적용하였습니다.");
		}
	}
	else
	{
		if(m_axConnectOld)
		{
			m_axConnectOld->SetGuide("환경 설정을 적용하였습니다.");
		}
	}

	return 0;
}

void CMainFrame::LoadHotkeySetting()
{
	FreeHotkeySetting();
	CString file;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);

	m_bUseHideHotkey = (BOOL)::GetPrivateProfileInt("HOTKEY", "UseHideHotkey", 0, file);
	if (m_bUseHideHotkey)
	{
		char buff[16];
		int pos = ::GetPrivateProfileString("HOTKEY", "HideHotkey", "", buff, sizeof(buff)-1, file);
		buff[pos] = NULL;
		if (buff[0]=='F')
		{
			const UINT keycode = VK_F1 + (atoi(&buff[1])-1);
			if (!RegisterHotKey(m_hWnd, 0x1212, 0, keycode))
			{
				CString msg;
				msg.Format("다른 프로그램에서 이미 사용하고 있는 키[%s]입니다.\r\n재지정 하시기 바랍니다.", buff);
				MessageBox(msg, "IBK투자증권", MB_OK|MB_ICONINFORMATION);
			}
		}
	}
}

void CMainFrame::FreeHotkeySetting()
{
	UnregisterHotKey(m_hWnd, 0x1212);
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
	return CreateClient(lpcs, NULL);
}

BOOL CMainFrame::IsSuperUser()
{
	CString s;
	//if (!Axis::isCustomer)
	//	return (Axis::userID=="071006"); 

	//else
	{
		CString strFile;
		strFile.Format("%s\\tab\\EXECSCREEN.ini", Axis::home); 

		char readB[1024]{};
		int readL{};
		
		const int count = GetPrivateProfileInt("USER","COUNT",0,strFile);

s.Format("------------------------------------------------[axis][mainframe][IsSuperUser][aes] strFile=[%s] count=[%d]\n", strFile, count);
OutputDebugString(s);

		for(int i=0;i<count;i++)
		{
			CString strKey;
			strKey.Format("%02d",i);

			readL = GetPrivateProfileString("USER",strKey,"",readB,sizeof(readB),strFile);

			CString userID(readB,readL);

			if(userID.Mid(0,2) == "!@")
			{
			
				CString encID = userID.Mid(2);
 				s.Format("[axis][mainframe][IsSuperUser][aes] ENC ID [%s]\n",encID);
 				OutputDebugString(s);
				userID = DecryptAES(encID);
 				s.Format("[axis][mainframe][IsSuperUser][aes] DEC ID [%s]\n",userID);
 				OutputDebugString(s);
			}
			else
			{
				CString encID = EncryptAES(userID);

				encID = "!@" + encID;

				WritePrivateProfileString("USER",strKey,encID,strFile);
			}

			userID.TrimLeft();
			userID.TrimRight();

			userID.MakeLower();

			if(Axis::userID == userID)
			{
				return TRUE;
			}
		}

		WritePrivateProfileString("USER","ENC","1",strFile);

		return FALSE;
	}
}

void CMainFrame::mac_report()
{
	if (m_macaddr.IsEmpty()) return;

	const CTime tm = CTime::GetCurrentTime();
	CString data;
	data.Format("%s;%s;%s", (LPCSTR)tm.Format("%Y/%m/%d %H:%M:%S"), (LPCSTR)m_macaddr, (LPCSTR)m_ipAddr);

	struct	_pidouini_mid mid;
	memset(&mid, 0, sizeof(mid));
	mid.gubn[0] = 'I';
	sprintf(mid.item.usid, (LPCSTR)Axis::userID);
	sprintf(mid.item.innm, "INFO");
	sprintf(mid.item.senm, "HTS");
	sprintf(mid.item.skey, "CONNECTION");
	sprintf(mid.item.valu, (LPCSTR)data);
	sendTR("pidouini", (char*)&mid, sizeof(mid), 0, 'p');
}

void CMainFrame::scr_counter_report()
{
	const CTime now = CTime::GetCurrentTime();
	CString yymmdd = now.Format("%Y%m%d");

	CString &cdat = m_mapHelper->GetCountData();
	if (cdat.IsEmpty()) return;

	struct	_pidouini_mid mid;
	memset(&mid, 0, sizeof(mid));
	mid.gubn[0] = 'I';
	sprintf(mid.item.usid, (LPCSTR)Axis::userID);
	sprintf(mid.item.innm, "INFO");
	sprintf(mid.item.senm, "HTS");
	sprintf(mid.item.skey, "SCREEN");
	if (cdat.GetLength() > sizeof(mid.item.valu)) 
		cdat = cdat.Mid(0, sizeof(mid.item.valu));
	sprintf(mid.item.valu, (LPCSTR)cdat);
	memcpy(mid.item.date, (LPCSTR)yymmdd, min(yymmdd.GetLength(), sizeof(mid.item.date)));

	sendTR("pidouini", (char*)&mid, sizeof(mid), 0, 'p');
}

void CMainFrame::save_scr_counter()
{
	const CTime now = CTime::GetCurrentTime();
	CString yymmdd = now.Format("%Y%m%d");
	
	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 
	WritePrivateProfileString("TRCounter", "Date", (LPCSTR)yymmdd, file);
	WritePrivateProfileString("TRCounter", "Data", (LPCSTR)m_mapHelper->GetCountData(), file);
}

void CMainFrame::load_scr_counter()
{
	const CTime now = CTime::GetCurrentTime();
	CString yymmdd = now.Format("%Y%m%d");

	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 

	char date[16]{}, data[1024 * 16]{};
	GetPrivateProfileString("TRCounter", "Date", "", date, sizeof(date), file);

	if (yymmdd.Compare(date)==0)
	{
		const int dlen = GetPrivateProfileString("TRCounter", "Data", "", data, sizeof(data), file);
		m_mapHelper->SetCountData(data, dlen);
	}
}

void CMainFrame::SplitStr( LPCSTR st, LPCSTR ed, CHAR del, std::vector<CString> *pResult )
{
	if (st==NULL || ed==NULL) return;
	if (st>=ed) return;
	
	pResult->clear();
	while(st<ed)
	{
		LPCSTR pos = std::find(st, ed, del);
		pResult->push_back(CString(st, pos-st));
		pResult->back().TrimLeft();
		pResult->back().TrimRight();
		st = pos + 1;
	}
}

void CMainFrame::SplitStr( CString *data, CHAR del, std::vector<CString> *pResult )
{
	LPCSTR st = (LPCSTR)(*data);
	LPCSTR ed = st + data->GetLength();
	SplitStr( st, ed, del, pResult );
}

void CMainFrame::ProcessRexp( struct rexp_mid *mid )
{
	char buff[32];
	sprintf(buff, "%.4s", mid->nrec);
	const int nrec = atoi(buff);
	
	for(int n=0; n<nrec; ++n)
	{
		m_rexp.push_back( mid->item[n] );
	}
	
	StartRexp();
}

void CMainFrame::StartRexp()
{
	if (m_rexp.empty()) return;
	
	CString type(m_rexp[0].type, sizeof(m_rexp[0].type)); type.TrimLeft(); type.TrimRight();
	CString path(m_rexp[0].path, sizeof(m_rexp[0].path)); path.TrimLeft(); path.TrimRight();
	CString size(m_rexp[0].size, sizeof(m_rexp[0].size)); size.TrimLeft(); size.TrimRight();
	
	struct pibotran_mid mid;
	printf(mid.seqn, "000001");
	sprintf(mid.start, "00000000");
	sprintf(mid.file, (LPCSTR)path);
	
	sendTR("pibotran", (char*)&mid, sizeof(mid), 0, 'R');
}

void CMainFrame::processTran( char *data, int dlen )
{
	if (m_rexp.empty()) return;
	
	const struct pibotran_mod *mod = (struct pibotran_mod*)data;
	
	CString dstpath;
	CString type(m_rexp[0].type, sizeof(m_rexp[0].type)); type.TrimLeft(); type.TrimRight();
	CString path(m_rexp[0].path, sizeof(m_rexp[0].path)); path.TrimLeft(); path.TrimRight();
	CString size(m_rexp[0].size, sizeof(m_rexp[0].size)); size.TrimLeft(); size.TrimRight();
	
	
	CString seqn(mod->seqn, sizeof(mod->seqn));
	CString max_seqn(mod->max_seqn, sizeof(mod->max_seqn));
	CString data_size(mod->size, sizeof(mod->size));
	
// 	CString emsg;
// 	emsg.Format("pibotran seqn(%s/%s) size(%s)\n", seqn, max_seqn, data_size);
// 	OutputDebugString(emsg);
	
	if (type=="MAP")
	{
		std::vector<CString> sp_path;
		SplitStr(&path, '/', &sp_path);
		
		dstpath.Format("%s\\map\\%.2s\\%.3s\\%s.exp", (LPCSTR)Axis::home, (LPCSTR)sp_path.back(), (LPCSTR)sp_path.back(), (LPCSTR)sp_path.back());
		
		FILE* fp;
		fopen_s(&fp, (LPCSTR)dstpath, "a+b");
		if (fp)
		{
			fwrite(mod->data, 1, atoi(data_size), fp);
			fclose(fp);
			
			if (atoi(seqn)==atoi(max_seqn))	// 파일받기 완료
			{
				CString newpath = dstpath;
				newpath.Replace(".exp", "");
				DeleteFile(newpath);
				MoveFile((LPCSTR)dstpath, (LPCSTR)newpath);
			}
			else
			{
				struct pibotran_mid mid;
				sprintf(mid.seqn, "%06d", atoi(seqn) + 1);
				memcpy(mid.start, mod->start, sizeof(mod->start));
				sprintf(mid.file, (LPCSTR)path);
				sendTR("pibotran", (char*)&mid, sizeof(mid), 0, 'R');
				return;
			}
		}
	}
	
	m_rexp.erase(m_rexp.begin());
	StartRexp();
}

const CWnd* CMainFrame::GetBkWnd()
{
	return m_MClient->m_pBkMapWnd;
}

void CMainFrame::OutputWaitList()
{
	return;
	/*
	OutputDebugString("OutputWaitList ----------------------\r\n");
	int size = m_waitlist.GetSize();
	for(int ii=0; ii<size; ++ii)
	{
		int key = m_waitlist.GetAt(ii);
		
		BOOL bFind = FALSE;
		CString dmsg, caption;
		CChildFrame *frame = NULL;
		CSChild *child = NULL;
		for(int jj=0; jj<6; ++jj)
		{
			if (m_arMDI[jj].Lookup(key, frame))
			{
				frame->GetWindowText(caption);
				dmsg.Format("MDI [%s] %s\r\n", (LPCSTR)frame->m_mapN, (LPCSTR)caption);
				OutputDebugString((LPCSTR)dmsg);
				bFind = TRUE;
				break;
			}
			else if (m_arSDI[jj].Lookup(key, child))
			{
				child->GetWindowText(caption);
				dmsg.Format("SDI [%s] %s\r\n", (LPCSTR)child->m_mapN, (LPCSTR)caption);
				OutputDebugString((LPCSTR)dmsg);
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)
		{
			dmsg.Format("Can't found key(%d) info\r\n", key);
			OutputDebugString(dmsg);
		}
	}
	OutputDebugString("-------------------------------------\r\n");
	*/
}

BOOL CMainFrame::ChangeTabView( LPCSTR mapN, int key )
{
//  	OutputWaitList();
	for(int n=0; n<m_waitlist.GetSize(); ++n)
	{
		if (m_waitlist.GetAt(n)==key) return FALSE;
	}
	m_mapHelper->ChangeTabView(mapN, 0, key);
	return TRUE;
}

BOOL CMainFrame::ClearGex()
{
	if (MessageBox(
		"모든 차트설정이 초기화 됩니다.\n"
		"(현재 열린화면은 적용되지 않습니다)\n\n"
		"초기화 하시겠습니까?", "IBK투자증권", MB_YESNO)==IDYES)
	{
		CString org_path = Axis::home + "\\gex\\" + Axis::user;
		CString new_path = Axis::home + "\\gex\\" + Axis::user + "." + CTime::GetCurrentTime().Format("%Y%m%d_%H%M%S");
		return MoveFile(org_path, new_path);
	}
	return FALSE;
}

void CMainFrame::ini_report(LPCSTR innm, LPCSTR senm, LPCSTR skey, LPCSTR valu)
{
	struct	_pidouini_mid mid;
	memset(&mid, 0, sizeof(mid));
	mid.gubn[0] = 'I';
	memcpy(mid.item.usid, (LPCSTR)Axis::userID, Axis::userID.GetLength());
	memcpy(mid.item.innm, innm, min(strlen(innm), sizeof(mid.item.innm)));
	memcpy(mid.item.senm, senm, min(strlen(senm), sizeof(mid.item.senm)));
	memcpy(mid.item.skey, skey, min(strlen(skey), sizeof(mid.item.skey)));
	memcpy(mid.item.valu, valu, min(strlen(valu), sizeof(mid.item.valu)));
	sendTR("pidouini", (char*)&mid, sizeof(mid), 0, 'p');
}

void CMainFrame::ClearRepositoryLog()
{
	const CTime tm = CTime::GetCurrentTime() + CTimeSpan(-1, 0, 0, 0);
	
	CString strPath = Axis::home + "\\log????????_??????.trc";
	CString strToday = Axis::home + "\\log" + tm.Format("%Y%m%d");

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	while(hFind!=INVALID_HANDLE_VALUE)
	{
		CString strFile = Axis::home + "\\" + wfd.cFileName;
		if (strToday>strFile)
		{
			DeleteFile(strFile);
		}

		if (!FindNextFile(hFind, &wfd)) 
		{
			FindClose(hFind);
			break;
		}
	}
}

void CMainFrame::load_dbar2()
{
	DWORD dwListBarStyle = m_axis->GetProfileInt(INFORMATION, "listbar_pos", 0);

	if (dwListBarStyle==0)
	{
		CProfile p(pkUserSetup);
		dwListBarStyle = (DWORD)p.GetDouble(INFORMATION, "listbar_pos", CBRS_ALIGN_BOTTOM);
	}
	else
	{
		m_axis->WriteProfileInt(INFORMATION, "listbar_pos", 0);
		CProfile(pkUserSetup).Write(INFORMATION, "listbar_pos", (double)dwListBarStyle);
	}	
	if (m_bar2)
	{
		if      (dwListBarStyle&CBRS_ALIGN_BOTTOM) m_bar2->MoveBar(MB_BOTTOM);
		else if (dwListBarStyle&CBRS_ALIGN_TOP)    m_bar2->MoveBar(MB_TOP);
	}
	
}

void CMainFrame::FixUserConfigDirectory()
{
	CString gexPath, usrPath, emsg;
	gexPath.Format("%s\\%s\\*.*", Axis::home, GEXDIR);

	BOOL bFind;
	CFileFind ffGex, ffUsr;
	
	CStringArray delList;
	bFind = ffGex.FindFile(gexPath);
	while(bFind)
	{
		bFind = ffGex.FindNextFile();

		usrPath = ffGex.GetFilePath();
		usrPath.Replace(GEXDIR, USRDIR);
		if  (!ffUsr.FindFile(usrPath))
		{
			delList.Add(ffGex.GetFilePath());
		}
	}

	for(int n=0; n<delList.GetSize(); ++n)
	{
		DeleteDirectory(delList[n]);
	}
}

void CMainFrame::CheckEncryptDirectory()
{
	SetUserInfo();

	CString src, dst, enc;
	//enc.Format("%08u", HashKey<LPCSTR>(Axis::userNM));
	enc.Format("%08u", HashDataAXIS((LPCSTR)Axis::userNM));
#if 0
	if (Axis::isCustomer)
	{
		gAPIHook.StopHook();
		src.Format("%s/%s/%s", Axis::home, USRDIR, enc);
		dst.Format("%s/%s/%s", Axis::home, USRDIR, Axis::userNM);
		MoveFile(src, dst);
		
		src.Format("%s/%s/%s", Axis::home, GEXDIR, enc);
		dst.Format("%s/%s/%s", Axis::home, GEXDIR, Axis::userNM);
		MoveFile(src, dst);

		src.Format("%s/%s/%s/%s.ini", Axis::home, USRDIR, Axis::userNM, enc);
		dst.Format("%s/%s/%s/%s.ini", Axis::home, USRDIR, Axis::userNM, Axis::userNM);
		MoveFile(src, dst);
		//OutputDebugString("APIHOOK Stop!!!!!!!!!!!!!!!!!!!!!\n");
	}
	else
#endif
	{	
		gAPIHook.StopHook();

		src.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, Axis::userNM, Axis::userNM);
		dst.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, Axis::userNM, enc);
		MoveFile(src, dst);
		
		src.Format("%s\\%s\\%s", Axis::home, USRDIR, Axis::userNM);
		dst.Format("%s\\%s\\%s", Axis::home, USRDIR, enc);
		MoveFile(src, dst);
		
		src.Format("%s\\%s\\%s", Axis::home, GEXDIR, Axis::userNM);
		dst.Format("%s\\%s\\%s", Axis::home, GEXDIR, enc);
		MoveFile(src, dst);
		
		src.Format("%s\\%s\\%s", Axis::home, USRDIR, Axis::userNM);
		RemoveDirectory(src);
		
		src.Format("%s\\%s\\%s", Axis::home, GEXDIR, Axis::userNM);
		RemoveDirectory(src);

		gAPIHook.SetInfo(Axis::home, Axis::userNM);
		gAPIHook.StartHook();
	}
}

#ifdef USE_AHNLAB_SECUREBROWSER

// int __stdcall SBSDK_Callback(long lCode, void* pParam, long lParamSize)
// {
// 	switch(lCode)
// 	{
// 	case SBSDK_CALLBACK_DRIVERFAILED :
// 		AfxMessageBox(_T("드라이버를 정상적으로 실행하지 못했습니다."));
// 		break;
// 	case SBSDK_CALLBACK_DETECT_DEBUG :
// 		{
// 			AfxMessageBox(_T("디버거 실행을 탐지했습니다.보안위험이 발생하여 HTS를 종료합니다.\nPC를 점검하여 주시기 바랍니다."));
// 			
// 			// 프로그램 종료 코드
// 			//theApp.quitApplication();
// 			//AfxGetMainWnd()->PostMessage(WM_CLOSE);
// 			DWORD processId = 0;
// 			GetWindowThreadProcessId(AfxGetMainWnd()->m_hWnd,&processId);
// 			TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,FALSE,processId),0);
// 		}
// 		break;
// 	case SBSDK_CALLBACK_ABNORMAL_MEMORY_ACCESS :
// 		{
// 			CString memAccess;
// 
// 			if(pParam)
// 			{
// 				PST_SBSDK_PROCESS_PATH phpf = (PST_SBSDK_PROCESS_PATH)pParam;
// 				
// 				USES_CONVERSION;
// 				TRACE(_T("[SBSDK_Callback] SBSDK_CALLBACK_ABNORMAL_MEMORY_ACCESS(#%ld,'%s')\n"),  
// 					phpf->ulPID, A2T(phpf->szProcessName));
// 
// 				memAccess.Format("[%s]",phpf->szProcessName);
// 			}
// 			
// 			AfxMessageBox(_T(memAccess + " 메모리에 허가되지 않은 접근을 시도했습니다.보안위험이 발생하여 HTS를 종료합니다.\nPC를 점검하여 주시기 바랍니다."));
// 			
// 			// 프로그램 종료 코드
// 			//theApp.quitApplication();
// 			//AfxGetMainWnd()->PostMessage(WM_CLOSE);
// 			DWORD processId = 0;
// 			GetWindowThreadProcessId(AfxGetMainWnd()->m_hWnd,&processId);
// 			TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,FALSE,processId),0);
// 		}
// 		break;
// 	case SBSDK_CALLBACK_BLACKLIST :
// 		AfxMessageBox(_T("악성코드로 의심되는 프로그램을 차단했습니다."));
// 		break;
// 	default :
// 		{
// 			//theApp.messageBoxFormat(_T("해킹 방지 기능에 문제가 발생했습니다(lCode=0x%x)"), lCode);
// 		}
// 		break;
// 	} // end of switch
// 	
// 	return 0;
// }
// 
// BOOL CMainFrame::initAOSSDK()
// {
// 	if(isVerifyTrustSDK() == FALSE)
// 	{
// 		//AfxMessageBox(_T("[SB] 메모리보호모듈 검증에 실패하였습니다."));
// 		return FALSE;
// 	}
// 
// 	// 업데이트 상태 창이 보이도록 설정
// 	Aossdk_SetModeUI(TRUE);
// 
// 	// 업데이트 동기 모드(Sync Mode) 설정: AOS Secure Browser 반드시 동기 모드로 동작 해야 함 
// 	Aossdk_SetModeAsync(FALSE);
// 
// 	DWORD dwErrorCode = 0;
// 	BOOL bResult = FALSE;
// 	// "20A06A69C9F10BAF085E02416745AEBA7BE57FEE8BCA4F29"
// 	// AOS Secure Browser 업데이트
// 	bResult = Aossdk_StartAosSDKA( 
// 					"e9",
// 					"20A06A69C9F10BAF085E02416745AEBA7BE57FEE8BCA4F29",
// 					GetSafeHwnd(),
// 					&dwErrorCode
// 				);
// 
// 	if(bResult == FALSE)
// 	{
// 		CString emsg;
// 		emsg.Format(_T("[SB] 메모리보호모듈 업데이트에 실패하였습니다. [0x%X])"), dwErrorCode);
// 		//MessageBox(emsg);
// 		return FALSE;
// 	}
// 
// 	m_pAosSB = Aossdk_GetSBObject(&dwErrorCode);
// 	if(m_pAosSB == NULL)
// 	{
// 		CString emsg;
// 		emsg.Format(_T("[SB] 메모리보호모듈 생성에 실패하였습니다. (0x%X)"), dwErrorCode);
// 		//MessageBox(emsg);
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }
// 
// void CMainFrame::uninitAOSSDK()
// {
// 	//Aossdk_TerminateSDKA("id:test_sb_sdk");
// 	BOOL bRtn = Aossdk_TerminatePdA(AOSSDK_SERVICE_CODE_SB,"id:5DABE5C601A548810878846D671C0DA7");
// }
// 
// BOOL CMainFrame::initSBSDK()
// {
// 	if(m_pAosSB == NULL)
// 	{
// 		//AfxMessageBox(_T("[aossdk] SBSDK object error"));
// 		return FALSE;
// 	}
// 	
// 	CString strPath;
// 	DWORD dwSize = GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
// 	strPath.GetBufferSetLength(dwSize);
// 	strPath.Replace("axis.exe", "axis.ndg");
// 	strPath.ReleaseBuffer();
// 	
// 	DWORD dwOption;
// 	CFileFind ff;
// 	if (ff.FindFile(strPath)) dwOption = NULL;
// 	else                      dwOption = SBSDK_ACTIVATE_ANTIDEBUG|SBSDK_ACTIVATE_SHILD|SBSDK_ACTIVATE_REMOTE;
// 	//dwOption = NULL;
// 
// 	DWORD dwResult = m_pAosSB->Initialize(dwOption, SBSDK_Callback);
// 	if(dwResult != SBSDK_ERROR_SUCCESS && dwResult != SBSDK_ERROR_COMPATIBILITY_MODE)
// 	{
// 		CString csmsg;
// 		if(dwResult == SBSDK_ERROR_DETECT_DEBUG)
// 		{
// 			csmsg = _T("[SB] 메모리보호모듈이 디버거툴을 감지하였습니다.");
// 			
// 			// 프로그램 종료 코드
// 			//theApp.quitApplication();
// 		}
// 		else
// 		{
// 			csmsg.Format(_T("[SB] 메모리보호모듈 실행에 실패하였습니다 (0x%X)"), dwResult);
// 		}
// 		
// 		AfxMessageBox(csmsg);
// 		return FALSE;
// 	}
// 	
// 	OutputDebugString("AOS 감시 타이머 시작\n");
// 	SetTimer(TM_AOS_ALIVE,30000,NULL);
// 
// 	return TRUE;
// }
// 
// void CMainFrame::uninitSBSDK()
// {
// 	if(m_pAosSB != NULL)
// 	{
// 		m_pAosSB->Uninitialize();
// 		m_pAosSB = NULL;
// 	}
// }
// 
// BOOL CMainFrame::isVerifyTrustSelf( LPCTSTR szSignedName )
// {
// 	BOOL bResult = FALSE;
// 	
// 	IAosVerify*	pAosVerify = getAosVerify();
// 	if(pAosVerify)
// 	{
// 		// AOS SDK를 호출하는 실행 파일이 Code Sign 되어 있을 경우 현재 모듈이
// 		// 변조되지 않았는지 자체 검사
// #ifdef _UNICODE
// 		bResult = pAosVerify->VerifyTrustSelfW(szSignedName);
// #else
// 		bResult = pAosVerify->VerifyTrustSelfA(szSignedName);
// #endif
// 	}
// 	
// 	return bResult;
// }
// 
// BOOL CMainFrame::isVerifyTrustSDK()
// {
// 	BOOL bResult = FALSE;
// 	
// 	IAosVerify*	pAosVerify = getAosVerify();
// 	if(pAosVerify)
// 	{
// 		// AOS SDK 모듈이 변조되지 않았는지 검사
// 		bResult = pAosVerify->VerifyTrustSDK();
// 	}
// 	
// 	return bResult;
// }
// 
// BOOL CMainFrame::isRunningSDKPd( DWORD dwServiceCode )
// {
// 	BOOL bResult = FALSE;
// 	
// 	IAosVerify*	pAosVerify = getAosVerify();
// 	if(pAosVerify)
// 	{
// 		// AOS SDK 모듈이 현재 실행 중인지 검사
// 		bResult = pAosVerify->IsRunningPd(dwServiceCode);
// 
// // 		CString s;
// // 		s.Format("RUNNING [%d]\n",bResult);
// // 		OutputDebugString(s);
// 	}
// 	
// 	return bResult;
// }
// 
// IAosVerify* CMainFrame::getAosVerify()
// {
// 	static IAosVerify *_pAosVerify = NULL;
// 	
// 	if(_pAosVerify == NULL)  
// 	{
// 		DWORD dwErrorCode = 0;
// 		
// 		// Get IAosVerify Object
// 		_pAosVerify = Aossdk_GetVerifyObject(&dwErrorCode);
// 	}
// 	
// 	return _pAosVerify;
// }

LRESULT CMainFrame::OnMiniClose( WPARAM wParam, LPARAM lParam )
{
	m_miniWid = NULL;

	//OutputDebugString("DESTORY MINI WND FROM MAIN\n");
	
	return 0;
}

LRESULT CMainFrame::OnHTSCloseFMini( WPARAM wParam, LPARAM lParam )
{
	PostMessage(WM_SYSCOMMAND, SC_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnHotTrade( WPARAM wParam, LPARAM lParam )
{
	m_bMiniMode = FALSE;

	ChangeMDI();

	if (!m_arSDI[m_vsN].IsEmpty())
	{
		int key{};
		CSChild* pChild{};
		POSITION pos = m_arSDI[m_vsN].GetStartPosition();
		while(pos!=NULL)
		{	
			m_arSDI[m_vsN].GetNextAssoc(pos, key, pChild);
			
			if(pChild->m_mapN != "IB0000X8")
				pChild->ShowWindow(SW_SHOW);
			else
				pChild->ShowWindow(SW_HIDE);
		}
	}
	if (!m_arMPOP.IsEmpty())
	{
		int key{};
		CMPop* pPop{};
		POSITION pos = m_arMPOP.GetStartPosition();
		while(pos!=NULL)
		{
			m_arMPOP.GetNextAssoc(pos, key, pPop);
			pPop->ShowWindow(SW_SHOW);
		}
	}
	ShowWindow(SW_SHOW);

	PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);

	m_miniWid->ShowWindow(SW_HIDE);

	return 0;
}

LRESULT CMainFrame::OnIsDev( WPARAM wParam, LPARAM lParam )
{
	if(Axis::devMode)	
	{
		return 2;
	}
	
	return 1;
}

LRESULT CMainFrame::OnStaff( WPARAM wParam, LPARAM lParam )
{
	if(Axis::isCustomer)
	{
		return 1;
	}

	return 2;
}

void CMainFrame::AddCodehistoryToInter(int igroup, char* pdata)
{
	CString strCode;

	if(igroup == -1)
		strCode.Format("IB202201%s%c%02d%c%s", "selectGROUP", 0x09, -1, 0x09, "");
	else
		strCode.Format("IB202201%s%c%02d%c%s", "appendITEMS", 0x09, igroup, 0x09, pdata);

	m_mapHelper->ChangeChild(strCode, 1, 0, CenterPOS);
}

LRESULT CMainFrame::OnIBKWine( WPARAM wParam, LPARAM lParam )
{
	// 와인서비스 종료에따라 주석처리 - dkkim 2016.01.11
// 	CString str;
// 	str.Format("WM_IBK 메시지를 받았습니다. ( wParam = %d, lParam = %d )", wParam, lParam);
// 	OutputDebugString(str);
// 	
// 	HWND hwnd = m_winechartHwnd = (HWND)wParam;
// 	int n = ((int)lParam % 17);
// 	
// 	::SendMessage(hwnd,  WM_IBK, n, 0);
// 	str.Format("인증 정보 전송 완료 [ 검증 숫자 : %d ]", n);	
// 	OutputDebugString(str);
// 
// 	if(m_miniWid == NULL || !m_miniWid->GetSafeHwnd())
// 	{
// 		OutputDebugString("RECREATE MINI WID\n");
// 		const char* trust = "IB0000X8";
// 		m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
// 	}
// 	
// 	if(!m_bSDI)
// 	{
// 		CWnd* base = m_miniWid->GetActiveView()->GetWindow(GW_CHILD);
// 		base->SendMessage(WD_WINEWND,(WPARAM)0,(LPARAM)(LONG)hwnd);
// 	}

	return 0;
}

LRESULT CMainFrame::OnIBKCode( WPARAM wParam, LPARAM lParam )
{
// 	CString str;
// 	str.Format("WM_IBK_CODE 메시지를 받았습니다. ( wParam = %d, lParam = %d )", wParam, lParam);
// 	OutputDebugString(str);
// 
// 	str.Format("종목설정: %06d", wParam);
// 	OutputDebugString(str);

// 	CChildFrame*	child;
// 	CSChild*	schild;
// 
// 	CString sdat;
// 	sdat.Format("1301\t%06d",wParam);
// 	
// 	int	key;
// 	long	rc = 0;
// 	POSITION pos;
// 	
// 	for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
// 	{
// 		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
// 		
// 		if(child->m_mapN.Right(2) == "00")
// 
// 			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sdat);
// 	}
// 	
// 	for (pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
// 	{
// 		m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
// 		
// 		if(schild->m_mapN.Right(2) == "00")
// 			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 					(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sdat);
// 	}

	return 0;
}

char* CMainFrame::GetSHA256(char* data, int dataL,HMODULE hModule,bool isfile)
{
	//HMODULE hModule = LoadLibrary("CX_SHA256.DLL");

	char* sha256{};
	
	if (hModule)
	{
		if(!isfile)
		{
			typedef long (WINAPI *GETSHAFUNC)(char*, int);

			GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axEncrypt");
			if (func)
			{
				sha256 =  (char*)func(data,dataL);
			}
		}
		else
		{
			typedef long (WINAPI *GETSHAFUNC)(char*);

			GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axFileEncrypt");
			if (func)
			{
				sha256 =  (char*)func(data);
			}
		}

		//FreeLibrary(hModule);
	}

	return sha256;
}

char* CMainFrame::GetFileSHA256( CString sPath, HMODULE hModule)
{
	char* sha256{};

	CFileFind finder;
	
	if (finder.FindFile(sPath))
	{
		TRY
		{
			sha256 = GetSHA256((LPSTR)(LPCTSTR)sPath,0,hModule,TRUE);
		}
		CATCH (CFileException, e) 
		{
		}
		END_CATCH;
	}
	else
	{
		return NULL;
	}

	finder.Close();

	return sha256;
}

LRESULT CMainFrame::OnGoodOrder( WPARAM wParam, LPARAM lParam )
{
	m_mapHelper->CreateChild("AM070101",0,0,CenterPOS,CPoint(400,400));

	return 0;
}

LRESULT CMainFrame::OnFocusNotice( WPARAM wParam, LPARAM lParam )
{
	if(m_bStart)
	{
		m_bStart = false;

		int key{};

		CChildFrame* pChild{};
		for (POSITION pos = m_arMDI[0].GetStartPosition(); pos; )
		{
			m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);
			
			if(pChild->m_mapN == "IB770000" || pChild->m_mapN.Mid(0,2) == "AM")
			{
				pChild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			}
		}	
	}
	return 0;
}

void CMainFrame::ScrapInformation()
{
	//struct	pihoitgy_mid	mid;
	
// 	FillMemory((char*) &mid, sizeof(pihoitgy_mid), ' ');
// 	
// 	CopyMemory(mid.gubn, "1", sizeof(mid.gubn));
// 	CopyMemory(mid.nrec, "0", sizeof(mid.nrec));
// 	CopyMemory(mid.grid2, "", sizeof(mid.grid2));
	
	sendTR("pibfstup", "Y", 1, 0, 'U');
}

void CMainFrame::QueryPihoitgyList()
{
	const int nItgy	   = m_axis->GetProfileInt(WORKSTATION, "itgy", 0);

	if(nItgy != 0 )
	{
		m_axis->WriteProfileInt(WORKSTATION, "itgy", 0);

		if(m_bCertLogin)
			signOnCert();
		else
		{
			OutputDebugString("GLB case QueryPihoitgyList");
			signOn();
		}
		return;
	}

	CString filename;
	filename.Format("%s\\%s\\%s", Axis::home, "exe", "NOITGY.TXT");
	FILE* fp;
	fopen_s(&fp, filename, "rb");
	
	if (fp)
	{
		fclose(fp);

		if(m_bCertLogin)
			signOnCert();
		else
		{
			OutputDebugString("GLB case QueryPihoitgyList");
			signOn();
		}
		return;
	}

	if(m_bUseNewLogin)
		m_axConnect->SetGuide("무결성 검증을 시작합니다.");
	else
		m_axConnectOld->SetGuide("무결성 검증을 시작합니다.");

	struct	pihoitgy_mid	mid;
	
	FillMemory((char*) &mid, sizeof(pihoitgy_mid), ' ');
	CString sdat;
	
	//sval.Format("%s INSTALL PATH [%s]\n",str,Axis::home);
	//==================================================
	CString s;
// 	s.Format("SEND CONFIG : [%s]\n",sval);
// 	OutputDebugString(s);
	OutputDebugString("pihoitgy");
	
	CopyMemory(mid.gubn, "1", sizeof(mid.gubn));
	CopyMemory(mid.nrec, "0", sizeof(mid.nrec));
	CopyMemory(mid.grid2, "", sizeof(mid.grid2));
WriteLog("[AXIS] CMainFrame::QueryPihoitgyList  pihoitgy send ");
	sendTR("pihoitgy", (char*)&mid, sizeof(pihoitgy_mid), 0, 255);

	SetTimer(TM_ITGY,5000,NULL);
}

void CMainFrame::QueryPihoitgy()
{
	
	struct	pihoitgy_mid	mid;
	
	FillMemory((char*) &mid, sizeof(pihoitgy_mid), ' ');
	CString sdat;
	
	//sval.Format("%s INSTALL PATH [%s]\n",str,Axis::home);
	//==================================================
	CString s;
	// 	s.Format("SEND CONFIG : [%s]\n",sval);
	// 	OutputDebugString(s);
	
	CString strCnt,strMsg;
	strCnt.Format("%d",m_arrayItgy.GetSize());

// 	CopyMemory(mid.gubn, "2", sizeof(mid.gubn));
// 	CopyMemory(mid.nrec, strCnt, sizeof(mid.nrec));
	//CopyMemory(mid.grid2, "", sizeof(mid.grid2));

	CString strGrid;

	strGrid = "";

	for(int i=0;i<m_arrayItgy.GetSize();i++)
	{
		strGrid += m_arrayItgy.GetAt(i);

		s.Format("[PIHOITGY] [UPLOAD] DATA [%d][%s]\n",i, m_arrayItgy.GetAt(i));
		OutputDebugString(s);
	}

	strMsg.Format("2%04d%s", m_arrayItgy.GetSize(), strGrid);

	//CopyMemory(mid.grid2, strGrid,strGrid.GetLength());

	//s.Format("[PIHOITGY] [UPLOAD] DATA [%s][%d]\n",strMsg,strMsg.GetLength());
	//OutputDebugString(s);
	
	//sendTR("pihoitgy", (LPSTR)(LPCTSTR)strMsg, strMsg.GetLength(), 0, 252);  //test comment

	OutputDebugString("Success Send ITGY\n");

	SetTimer(TM_ITGY,5000,NULL);
}

void CMainFrame::ParseScrap(char* dat, int len)
{
	TRY 
	{
		const pibfstup_mod* mod = (struct pibfstup_mod*)dat;
		
		int	cnt = atoi(CString(mod->nrec, sizeof(mod->nrec)));
		
		CString utyp,typ,fnam,sect,skey;
		
		m_arStup.RemoveAll();

		CString tmp = CString(dat,len);
		
		CString s;
		s.Format("!!!SCRAP ParseScrap[%d] --- %100s\n",cnt, dat);
		OutputDebugString(s);
		
		for(int i=0;i<cnt;i++)
		{
			m_arStup.Add((pibfstup_grid)mod->grid[i]);
		}
	}
	CATCH (CMemoryException, e)
	{
		
	}
	END_CATCH
}

CString CMainFrame::SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if(strToken == "") return sResult;
	const int iFind = strData.Find(strToken);
	if(iFind > -1 ) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}
	
	return sResult;
}

void CMainFrame::SendScrap()
{
	for(int i = 0;i < m_arStup.GetSize(); i++)
	{
		const pibfstup_grid grid = (struct pibfstup_grid)m_arStup.GetAt(i);

		CString utyp = CString(grid.utyp, sizeof(grid.utyp));	utyp.TrimRight();
		CString uid = CString(grid.uid, sizeof(grid.uid));		uid.TrimRight();
		CString typ = CString(grid.type, sizeof(grid.type));	typ.TrimRight();
		CString fnam = CString(grid.fnam, sizeof(grid.fnam));	fnam.TrimRight();
		CString sect = CString(grid.sect, sizeof(grid.sect));	sect.TrimRight();
		CString skey = CString(grid.skey, sizeof(grid.skey));	skey.TrimRight();

		CString s;
		s.Format("SCRAP [%s] [%s] [%s] [%s] [%s]\n",utyp,typ,fnam,sect,skey);
		OutputDebugString(s);

		struct	_pidouini_mid* puini = new _pidouini_mid;
		memset(puini, 0x00, sz_pidouini);
		
		CString sval, sdat;
		CString str(""), maps;
		CString file;	
		
		sval = "";
		//==================================================
		//CString s;
		// 	s.Format("SEND CONFIG : [%s]\n",sval);
		// 	OutputDebugString(s);
		if(utyp != "0")
		{
			if(utyp == "1" && Axis::isCustomer)
				return;
			else if(utyp =="2" && !Axis::isCustomer)
				return;
			else if(utyp == "3" && uid.Find(Axis::userID) < 0)
				return;
		}
		
		CTime	time;
		time = time.GetCurrentTime();
		sdat.Format("%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());

		if(fnam.Find("tab/") > -1)
		{
			const int find = fnam.ReverseFind('/');
			if (find > 0)
				fnam = fnam.Right(fnam.GetLength() - (find + 1));

			file.Format("%s\\%s\\%s", Axis::home, TABDIR, fnam);
		}
		else if(fnam.Find("/") < 0)
		{
			if(fnam == "NAME.ini")
			{
				file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, Axis::user, Axis::user);
			}
			else 
			{
				file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, fnam);
			}
		}
		
		if(typ == "0")
		{
			SendFile(file);
			continue;
		}
		else if(typ == "1")
		{
			char buff[2000];
			const int nCnt = GetPrivateProfileSection(sect,buff,sizeof(buff),file);

			for(int i=0;i<nCnt;i++)
			{
				if(buff[i] == '\0')
				{
					buff[i] =' ';
				}
			}
			//CString tmp;
			sval.Format("%s",buff);
			sval.TrimRight();

			skey = sect;
		}
		else if(typ == "2")
		{
			char buff[1024];
			GetPrivateProfileString(sect,skey,"",buff,sizeof(buff),file);
			
			sval = buff;
		}
		
		sdat.Remove('-');
		/*
		CopyMemory(uini.gubn, "I", sizeof(uini.gubn));
		CopyMemory(uini.item.usid, Axis::userID, sizeof(uini.item.usid));
		CopyMemory(uini.item.innm, fnam, sizeof(uini.item.innm));
		CopyMemory(uini.item.senm, sect, sizeof(uini.item.senm));
		CopyMemory(uini.item.skey, skey, sizeof(uini.item.skey));
		CopyMemory(uini.item.valu, sval, sizeof(uini.item.valu));
		CopyMemory(uini.item.date, sdat, sizeof(uini.item.date));
		
		sendTR("pidouini", (char*)&uini, sz_pidouini, 0, 'N');
		*/
		memcpy(puini->gubn, "I", 1);
		memcpy(puini->item.usid, Axis::userID, Axis::userID.GetLength());
		memcpy(puini->item.innm, fnam, fnam.GetLength());
		memcpy(puini->item.senm, sect, sect.GetLength());
		memcpy(puini->item.skey, skey, skey.GetLength());
		memcpy(puini->item.valu, sval, sval.GetLength());
		memcpy(puini->item.date, sdat, sdat.GetLength());

		sendTR("pidouini", (char*)puini, sz_pidouini, 0, 'N');
	}
}

void CMainFrame::ParsePihoitgyList(char* dat, int len)
{	
	CString s;
// 	s.Format("PIHOITGY LIST DATA : [%s]\n",CString(dat,len));
// 	OutputDebugString(s);
	if(!m_bItgy)
	{
		return;
	}
	else
	{
		KillTimer(TM_ITGY);
	}

	HMODULE hModule = LoadLibrary("CX_SHA256.DLL");

	CString		gubn, mnam, itgy;
	const pihoitgy_mod* mod = (struct pihoitgy_mod*) dat;
	int	cnt = atoi(CString(mod->nrec, sizeof(mod->nrec)));

	s.Format("[PIHOITGY] cnt = [%d]\n", cnt);
	OutputDebugString(s);


	m_arrayItgy.RemoveAll();
	m_arrayPlfItgy.RemoveAll();

	for (int ii = 0; ii < cnt; ii++)
	{
		gubn = CString(mod->grid[ii].gubn, sizeof(mod->grid[ii].gubn));	gubn.TrimRight();
		mnam = CString(mod->grid[ii].mnam, sizeof(mod->grid[ii].mnam));	mnam.TrimRight();

		CString filename,gridItem;

 		//s.Format("[PIHOITGY][%s][%s] [%s] [%d]\n", __FUNCTION__,gubn,mnam,mnam.Find("@"));
 		//OutputDebugString(s);

// 		if(!gubn.CompareNoCase("PLF"))
// 		{
// 			gubn = "EXE";
// 		}

		if(mnam.Find("@") > -1)
		{
			CString tmp;
			
			tmp = mnam.Mid(0,mnam.Find("@"));
// 			s.Format("@ REMOVE [%s]\n",tmp);
// 			OutputDebugString(s);
			
			mnam = tmp;
		}

		if(gubn != "MAP")
		{
			CString tmpG;
			
			if(!gubn.CompareNoCase("PLF"))
			{
				tmpG = "EXE";

#ifdef DF_SHOWITGYLOG
				m_slog.Format("[PIHOITGY] 조회해온 itgylist mnam=[%s] ", mnam);
				OutputDebugString(m_slog);
#endif

			}
			else
			{
				tmpG = gubn;
			}

			filename.Format("%s\\%s\\%s", Axis::home, tmpG, mnam);
		}
		else
		{
			filename.Format("%s\\%s\\IB\\%s\\%s", Axis::home, gubn, mnam.Mid(0,3), mnam);
		}

		//s.Format("[PIHOITGY] [%s] [%s] [%d]\n",gubn,mnam,mnam.Find("@"));
		//OutputDebugString(s);

		if(hModule)
		{
			char* str = GetFileSHA256(filename,hModule);

			gridItem.Format("%-3s%-47s%-44s",gubn,mnam,str);

#ifdef DF_SHOWITGYLOG	
			m_slog.Format("[PIHOITGY][%s]sha256  mnam=[%s] sha=[%s]  ", __FUNCTION__, mnam, str);
			OutputDebugString(m_slog);
#endif
			m_arrayItgy.Add(gridItem);
		}
	}

	if(hModule)
	{
		FreeLibrary(hModule);
	}

	if(cnt > 0)
	{
		if(m_bUseNewLogin)
			m_axConnect->SetGuide("검증 데이타를 전송합니다.");
		else
			m_axConnectOld->SetGuide("검증 데이타를 전송합니다.");

		QueryPihoitgy();
	}
	else
	{
		if(m_bUseNewLogin)
			m_axConnect->SetGuide("무결성 검증 대상이 없습니다.");
		else
			m_axConnectOld->SetGuide("무결성 검증 대상이 없습니다.");
		OutputDebugString("PIHOITGY LIST COUNT 0\n");
		if(m_bCertLogin)
			signOnCert();
		else
		{
			OutputDebugString("GLB case ParsePihoitgyList");
			signOn();
		}
	}
}

void CMainFrame::AddUniqueFromBtoA(CStringArray& arrA, const CStringArray& arrB)
{
	for (int i = 0; i < arrB.GetSize(); ++i)
	{
		BOOL bExists = FALSE;
		for (int j = 0; j < arrA.GetSize(); ++j)
		{
			if (arrA[j].Compare(arrB[i]) == 0)
			{
				bExists = TRUE;
				break;
			}
		}

		if (!bExists)
		{
			arrA.Add(arrB[i]);
		}
	}
}

int CMainFrame::Self_VerifyIntegrity()
{
	m_sIntegrityMSG.Empty();
	CString file;
	char	buf[512];
	file.Format("%s\\%s\\axisENC.ini", Axis::home, "tab");
	DWORD dw = GetPrivateProfileString("Integrity", "Except", "", buf, sizeof(buf), file);

	m_sExIntegrity.Format("%s", buf);
	m_sExIntegrity.TrimRight();

	m_slog.Format("[core] [%s]<%d> m_sExIntegrity= [%s] \n", __FUNCTION__, __LINE__, m_sExIntegrity);
	OutputDebugString(m_slog);

	CString strPath;
	char chfile[500]{};
	GetModuleFileName(nullptr, chfile, 260);
	strPath.Format("%s", chfile);

	int nPos = strPath.ReverseFind('\\');
	if (nPos != -1)
		m_sExtraced = strPath.Mid(nPos + 1);
	else
		m_sExtraced = strPath;

	m_slog.Format("[core] [%s]<%d>  [%s] [%s]\n", __FUNCTION__, __LINE__, m_sExtraced, m_sMainName);
	OutputDebugString(m_slog);

	//실행파일명을 고의로 변경했는지 비교해본다.
	if (m_sExtraced.CompareNoCase(m_sMainName) == 0)
		return 0;


	//m_arraySelfItgy.Add(m_sMainName);
	return 1;
}

void CMainFrame::ParsePihoitgy(char* dat, int len)
{	
	CString s;
	s.Format("PIHOITGY DATA [%s]\n",CString(dat,len));
	OutputDebugString(s);

	if(!m_bItgy)
	{
		return;
	}
	else
	{
		KillTimer(TM_ITGY);
	}

	BOOL	rc{};
	CString		gubn, mnam, itgy;
	const pihoitgy_mod* mod = (struct pihoitgy_mod*) dat;
	int	cnt = atoi(CString(mod->nrec, sizeof(mod->nrec)));

#ifdef DF_SHOWITGYLOG
	m_slog.Format("[core] [%s]<%d> itgy검증결과  >>>>>>>>>>    cnt =  [%d]\n", __FUNCTION__, __LINE__, cnt);
	OutputDebugString(m_slog);
#endif

	if(cnt == 0)
	{
		if(m_bUseNewLogin)
			m_axConnect->SetGuide("무결성 검증을 완료했습니다.");
		else
			m_axConnectOld->SetGuide("무결성 검증을 완료했습니다.");

		if(m_bCertLogin)
			signOnCert();
		else
		{
			OutputDebugString("GLB case ParsePihoitgy");
			signOn();
		}

		return;
	}

	m_arrayItgy.RemoveAll();
	m_arrayPlfItgy.RemoveAll();

	for (int ii = 0; ii < cnt; ii++)
	{
		gubn = CString(mod->grid[ii].gubn, sizeof(mod->grid[ii].gubn));	gubn.TrimRight();
		mnam = CString(mod->grid[ii].mnam, sizeof(mod->grid[ii].mnam));	mnam.TrimRight();

		if(gubn == "PLF")
		{

#ifdef DF_SHOWITGYLOG
			m_slog.Format("[core] 조회결과 :  변조된  platform 파일 =[%s] ", mnam);
			OutputDebugString(m_slog);
#endif

			m_arrayPlfItgy.Add(mnam);
		}
		else
		{

#ifdef DF_SHOWITGYLOG
			m_slog.Format("[core] 조회결과 :  변조된   파일 =[%s] ", mnam);
			OutputDebugString(m_slog);
#endif
			m_arrayItgy.Add(mnam);
		}
	}

	if(m_arrayItgy.GetSize() > 0)
	{
		UpdateInfo(true);
	}

	if(m_arrayPlfItgy.GetSize() > 0)
	{
		UpdateInfo(false);
	}

	if(m_bUseNewLogin)
		m_axConnect->SetGuide("파일이 변조되어 재기동 됩니다.");
	else
		m_axConnectOld->SetGuide("파일이 변조되어 재기동 됩니다.");

	m_axis->WriteProfileInt(WORKSTATION, "itgy", 1);

	m_update = true;
	//m_bExit = false;
	//m_forceClose = true;
	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc, (BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
	
	if(m_bUseNewLogin)
		m_axMisc->RunVers(verRETRY, m_axConnect->GetUserID(), m_axConnect->GetPassword(), m_axConnect->GetCPass());
	else
		m_axMisc->RunVers(verRETRY, m_axConnectOld->GetUserID(), m_axConnectOld->GetPassword(), m_axConnectOld->GetCPass());

	PostMessage(WM_CLOSE);
	//m_axConnect->EndDialog(IDCANCEL);
}

void CMainFrame::UpdateInfo(bool rsc)
{
	//char		name[32], ver[32], vkey[32], size[32], path[32], buf[256], buftmp[256];
	char		  buf[256]{};
	FILE* fp{}, * wfp{};
	CString		writefile, readfile, delRsc, tmps;
	
	if (rsc)
	{
		writefile.Format("%s\\tab\\infoRSC", Axis::home);
		readfile.Format("%s\\tab\\infoRSC_", Axis::home);
		CopyFile(writefile, readfile, TRUE);

		fopen_s(&fp, readfile, "r");
		if (fp && m_arrayItgy.GetSize())
		{
			 fopen_s(&wfp, writefile, "w+");
			while (fgets(buf, sizeof(buf), fp) != NULL)
			{
				bool	match  = false;
				//아래에서 데이타가 깨지는 현상 수정.아마도 한글포함 장문의 파일이름이 문제인듯
				//dkkim 2016.03.22
				//sscanf(buf, "%s %s %s %s %s", name, ver, vkey, size, path);
				CString line(buf);

// 				if(line.Find("movefield_sys.xml") > -1)
// 				{
// 					OutputDebugString("OK");
// 					flag = true;
// 				}
	
				for (int ii = 0; ii < m_arrayItgy.GetSize(); ii++)
				{
					delRsc = m_arrayItgy.GetAt(ii);
// 					if (!delRsc.CompareNoCase(name))
// 					{
// 						match = true;
// 						break;
// 					}
					if (line.Find(delRsc) > -1)
					{
						match = true;
						break;
					}
				}
// 				CString line;
// 				line.Format("%s %s %s %s %s", name, ver, vkey, size, path);
// 				if(flag == true)
// 				{
// 					CString s;
// 					s.Format("[DATA]\n%s\n=========\n%s\n",line,buf);
// 					OutputDebugString(s);
// 				}

				if (!match)	fprintf(wfp, buf);
			}
			
			fclose(wfp);
			fclose(fp);
			DeleteFile(readfile);
		}
	}
	else
	{
		writefile.Format("%s\\tab\\infoAXIS", Axis::home);
		readfile.Format("%s\\tab\\infoAXIS_", Axis::home);
		CopyFile(writefile, readfile, TRUE);
		
		fopen_s(&fp, readfile, "r");
		if (fp && m_arrayPlfItgy.GetSize())
		{
			 fopen_s(&wfp, writefile, "w+");
			while (fgets(buf, sizeof(buf), fp) != NULL)
			{
				bool	match = false;
				//sscanf(buf, "%s %s %s", name, ver, size);
				CString line(buf);

				for (int ii = 0; ii < m_arrayPlfItgy.GetSize(); ii++)
				{
					delRsc = m_arrayPlfItgy.GetAt(ii);
// 					CString s;
// 					s.Format("ITGY [%s]\n",delRsc);
// 					OutputDebugString(s);
// 					if (!delRsc.CompareNoCase(name))
// 					{
// 						match = true;
// 						break;
// 					}
					if (line.Find(delRsc) > -1)
					{
#ifdef DF_SHOWITGYLOG
		m_slog.Format("[core] [%s]<%d> ---- 변조되었다고 판단되어 infoAxis 파일 생성시 배제 !!!  delRsc=[%s] \n", __FUNCTION__, __LINE__, delRsc);
		OutputDebugString(m_slog);
#endif
						match = true;
						break;
					}
				}
				if (!match)	fprintf(wfp, buf);
			}
			
			fclose(wfp);
			fclose(fp);
			DeleteFile(readfile);
		}
	}
}

LRESULT CMainFrame::OnIBKWineS( WPARAM wParam, LPARAM lParam )
{
	// 와인서비스 종료에따라 주석처리 - dkkim 2016.01.11
// 	CString str;
// 	str.Format("WM_IBKWINES 메시지를 받았습니다. ( wParam = %d, lParam = %d )", wParam, lParam);
// 	OutputDebugString(str);
// 	
// 	HWND hwnd = m_wineSchartHwnd = (HWND)wParam;
// 	int n = ((int)lParam % 17);
// 	
// 	::SendMessage(hwnd,  WM_IBKWINES, n, 0);
// 	str.Format("인증 정보 전송 완료 [ 검증 숫자 : %d ]", n);	
// 	OutputDebugString(str);
// 	
// 	if(m_miniWid == NULL || !m_miniWid->GetSafeHwnd())
// 	{
// 		OutputDebugString("RECREATE MINI WID\n");
// 		const char* trust = "IB0000X8";
// 		m_mapHelper->ChangeChild(trust, 1, 0, CenterPOS);
// 	}
// 	
// 	if(!m_bSDI)
// 	{
// 		CWnd* base = m_miniWid->GetActiveView()->GetWindow(GW_CHILD);
// 		base->SendMessage(WD_WINESWND,(WPARAM)0,(LPARAM)(LONG)hwnd);
// 	}
	
	return 0;
}

LRESULT CMainFrame::OnIBKCodeS( WPARAM wParam, LPARAM lParam )
{
	// 와인서비스 종료에따라 주석처리 - dkkim 2016.01.11
// 	CChildFrame*	child;
// 	CSChild*	schild;
// 	
// 	CString sdat;
// 	sdat.Format("1301\t%06d",wParam);
// 	
// 	int	key;
// 	long	rc = 0;
// 	POSITION pos;
// 	
// 	for (pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
// 	{
// 		m_arMDI[m_vsN].GetNextAssoc(pos, key, child);
// 		
// 		if(child->m_mapN.Right(2) == "00")
// 			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sdat);
// 	}
// 	
// 	for (pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
// 	{
// 		m_arSDI[m_vsN].GetNextAssoc(pos, key, schild);
// 		
// 		if(schild->m_mapN.Right(2) == "00")
// 			m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sdat);
// 	}
	
	return 0;
}

LRESULT CMainFrame::OnIBKWineSNews( WPARAM wParam, LPARAM lParam )
{
	// 와인서비스 종료에따라 주석처리 - dkkim 2016.01.11
	//ShowNews();
	return 0;
}

LRESULT CMainFrame::OnIBKWineSKiup( WPARAM wParam, LPARAM lParam )
{
	// 와인서비스 종료에따라 주석처리 - dkkim 2016.01.11
// 	CString strCode;
// 
// 	strCode.Format("1301\t%06d",wParam);
// 
// 	CString s;
// 	s.Format("WINES-1 2023 DOMINO [%s]\n",strCode);
// 	OutputDebugString(s);
// 
// 	int key = m_mapHelper->CreateChild("IB202300",0);
// 
// 	m_nKiupKey = key;
// 	m_sKiupDomino = strCode;
// 
// 	SetTimer(TM_WINES_KIUP,500,NULL);

	return 0;
}

LRESULT CMainFrame::OnWebHwnd( WPARAM wParam, LPARAM lParam )
{
	CWnd* pwnd = (CWnd*)lParam;

	m_WBWnd = pwnd->m_hWnd;

	m_arWEB.Add(pwnd->m_hWnd);

	return 0;
}

LRESULT CMainFrame::OnCloseWeb(WPARAM wParam, LPARAM lParam)
{
	CloseWebHandle();

	return 0;
}

LRESULT CMainFrame::OnPhonePadNating(WPARAM wParam, LPARAM lParam)
{
	//Setfocus_Child(m_activeKey);

	CString dat;
	dat.Format("%s",(char*)lParam);
	dat.TrimLeft();
	dat.TrimRight();
	m_strPhone = dat;
	CString sdat;
	sdat = "zPwd\t"+dat;

	CString savl{};
	_stMap info;
	if (GetScreenInfoByMapKey(m_activeKey, info))
	{
		m_slog.Format("[phonepad] GetScreenInfoByMapKey TRUE dat=[%s] m_activeKey=[%d] ", dat, m_activeKey);
		OutputDebugString(m_slog);

		sdat = info.authId.TrimRight() + "\t" + dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
			(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
	}
	else
	{
		m_slog.Format("[phonepad] GetScreenInfoByMapKey FALSE dat=[%s] m_activeKey=[%d] ", dat, m_activeKey);
		OutputDebugString(m_slog);

		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
			(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		sdat = "pswd\t" + dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
			(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
		sdat = "ed_pswd\t" + dat;
		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
			(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, m_activeKey), (LPARAM)(const char*)sdat);
	}
	return 0;
}

LRESULT CMainFrame::OnLoadSecure( WPARAM wParam, LPARAM lParam )
{
	const bool m_auto = (bool)lParam;

	if(m_bUseNewLogin)
	{
		if(m_axConnect)
		{
			//m_axConnect->SetGuide("보안설정을 적용중입니다...");
			m_axConnect->SetStatus(SM_RSM);

			LoadSecureTools();

			m_axConnect->SetStatus(SM_NO);

			//m_axConnect->SetGuide("보안설정 적용을 완료하였습니다.");

			m_axConnect->SetGuide("로그인 준비가 완료되었습니다.");

			if(!m_auto)
				m_axConnect->SetEditFocus();
		}
	}
	else
	{
		if(m_axConnectOld)
		{
			//m_axConnect->SetGuide("보안설정을 적용중입니다...");
			m_axConnectOld->SetStatus(SM_RSM);
			
			LoadSecureTools();
			
			m_axConnectOld->SetStatus(SM_NO);
			
			//m_axConnect->SetGuide("보안설정 적용을 완료하였습니다.");
			
			m_axConnectOld->SetGuide("로그인 준비가 완료되었습니다.");
			
			if(!m_auto)
				m_axConnectOld->SetEditFocus();
		}
	}

	return 0;
}

LRESULT CMainFrame::OnResInterest(WPARAM wParam, LPARAM lParam)
{
	m_nInterest--;

	if(m_bExit == false)
	{
// 		if(m_nInterest < 1)
// 			PostMessage(WM_CLOSE);
		if(m_nInterest < 1)
			SetTimer(TM_2022_CLOSE,2000,NULL);
	}

	return 1;	
}

void CMainFrame::RunTOP10()
{
	//Send2018();

	if(m_top10 != nullptr && m_top10->GetSafeHwnd())
	{
		if(m_top10->IsData())
		{
			m_top10->SetBaseRect(m_TOP10Rect);
			if(m_top10->IsWindowVisible())
				m_top10->ShowWindow(SW_HIDE);
			else
				m_top10->ShowWindow(SW_SHOW);

		//	m_top10->ShowWindow(SW_SHOW);  //20191205 top10배너 클릭시 토글 동작
		}
	}
	else
	{
#ifdef DF_USE_CPLUS17
		m_top10 = std::make_unique<CTOP10Dialog>(this);  
		if (!m_top10->Create(IDD_TOP10))
			m_top10 = NULL;
#else
		m_top10 = new CTOP10Dialog(this);
		if (!m_top10->Create(IDD_TOP10))
		{
			delete m_top10;
			m_top10 = NULL;
		}
#endif
	
	}

}

void CMainFrame::Parse2018(char* dat, int len)
{
	const struct pibo2018_mod2* mod = (struct pibo2018_mod2*)dat;

	int	cnt = atoi(CString(mod->ncnt, sizeof(mod->ncnt)));

 	CString s;
// 	int nMod2 = (cnt-1) * sizeof(struct pibo2018_grid2) + sizeof(struct pibo2018_mod) + cnt;
// 	s.Format("TOP10 [%s] [%d]\n",(char*)dat,cnt);
// 	OutputDebugString(s);

//	dat = dat + nMod2;

//	struct pibo2018_mod2* mod2 = (struct pibo2018_mod2*)dat;
	
//	cnt = atoi(CString(mod2->ncnt, sizeof(mod2->ncnt)));
// 	s.Format("TOP10 [%s] [%d]\n",(char*)dat,cnt);
// 	OutputDebugString(s);
	
	CStringArray arr,codes,arrDlg;

	for(int i=0;i<cnt;i++)
	{
		CString name(mod->grid[i].hnam,sizeof(mod->grid[i].hnam));name.TrimRight();name.TrimLeft();
		CString code(mod->grid[i].cod2,sizeof(mod->grid[i].cod2));code.TrimRight();
		CString rate(mod->grid[i].rchg,sizeof(mod->grid[i].rchg));rate.TrimRight();
		CString isnew(mod->grid[i].isnew,sizeof(mod->grid[i].isnew));isnew.TrimRight();
		//s.Format("2018 [%s] [%s]\n",name,rate);
 		//OutputDebugString(s);
		CString str;

		str.Format("%s\t%s\t%s",name,rate,isnew);

		arr.Add(name);
		codes.Add(code);
		arrDlg.Add(str);
	}

	if(cnt > 0)
	{
		if (m_bar1 && m_bar1->IsWindowVisible())
			m_bar1->SetTOP10(arr);

		if(m_top10 != nullptr)
			m_top10->SetData(arrDlg,codes);
	}

// 	CString strFile;
// 	strFile.Format("%s\\tab\\TOP10.ini", Axis::home); 
// 	
// 	int nInterval = GetPrivateProfileInt("TOP10","SENDTIME",60000,strFile);
	
	//SetTimer(TM_TOP10_2018,nInterval,NULL);
	if(m_n2018Target == 1)
		m_b2018 = true;
}

LRESULT CMainFrame::OnTop10( WPARAM wParam, LPARAM lParam )
{
	CString code = (char*)lParam;

	if(code.GetAt(0) == 'A')
	{
		code.Remove('A');
	}

	if(wParam == 0)
	{
		char buff[1024];
		CString strFile;

		strFile.Format("%s\\tab\\TOP10.ini", Axis::home); 
		
		GetPrivateProfileString("TOP10","LINK","",buff,sizeof(buff),strFile);
		
		CString str,mapname;

		mapname = buff;

		if(mapname.GetLength() != L_MAPN)
		{
			return 1;
		}

		str.Format("%s1301\t%s",mapname,code);

		if(!IsExistMap(mapname.Mid(2,4),m_vsN))
		{
			const int key = m_mapHelper->ChangeChild(str,1, 0);
		}
		else
		{
			int key = 0;
			CChildFrame* pChild{};
			CString dat;
			dat.Format("1301\t%s",code);

			POSITION pos = m_arMDI[m_vsN].GetStartPosition();
			while (pos)
			{
				m_arMDI[m_vsN].GetNextAssoc(pos, key, pChild);
				ASSERT(pChild->m_mapN.GetLength() == L_MAPN);
				if (pChild->m_mapN.Mid(2, 4) == mapname.Mid(2,4))
				{
					m_activeKey = key;
					pChild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

					m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
						(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)dat);
				}
			}
		}
	}
	else if(wParam == 1)
	{
		SendTrigger(code);
	}
	else 
	{
		const int key = m_mapHelper->ChangeChild("IB201800",1, 0);
	}

// 	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)str);
// 	Sleep(1000);

	return 0;
}

//ASTx 관련 함수
BOOL CMainFrame::installASTx()
{
	CString strMasterURL;

//	if(IsWow64() == TRUE) //20191212 ASTX 설치 
	if(IsWow64() == "64 Bit") 
	{
		// x64
		strMasterURL.Format(_T("%s/%s/%s"), STSDKEX_MASTER_URL, STSDKEX_MASTER_PLATFORM_X64, STSDKEX_MASTER_NAME);
	}
	else
	{
		// x86
		strMasterURL.Format(_T("%s/%s/%s"), STSDKEX_MASTER_URL, STSDKEX_MASTER_PLATFORM_NT32, STSDKEX_MASTER_NAME);
	}

	const DWORD dwUIMode = STSDKEX_UI_MODE_SHOW;
// 	BOOL bHideUpdateUI = IsDlgButtonChecked(IDC_CHK_HIDE_UPDATEUI);
// 	if( bHideUpdateUI )
// 		dwUIMode = STSDKEX_UI_MODE_HIDE;
	strMasterURL.Empty();  //url 주소를 빈값으로 올리면 알아서 바꿔준다고 함
	DWORD dwError = STSDKEX_IsInstalled();
	switch( dwError )
	{
	case STSDKEX_ERROR_PRODUCT_NOT_INSTALLED:
		{
			dwError = STSDKEX_InstallA(STSDKEX_UI_MODE_SHOW, this->GetSafeHwnd(), strMasterURL);
			
			if( STSDKEX_ERROR_SUCCESS	!= dwError )
			{
				if(STSDKEX_ERROR_PRODUCT_NOT_INSTALLED == dwError)
				{
					CString path;
					path.Format("%s\\exe\\%s",Axis::home,"astx_setup_offline.exe");
					ShellExecute(NULL, _T("open"), path, "/s",NULL, SW_SHOWNORMAL);
				}
				else
				{
					CString szMsg;
					szMsg.Format(_T("업데이트중 오류가 발생했습니다.\r\n(result=0x%08x, %s)"), dwError, Err2Str(dwError));
					AfxMessageBox(szMsg);
					return FALSE;
				}
			}
		}
		break;
	}

	//astx 설치후 재시작
	const int AOSRESTART = AfxGetApp()->GetProfileInt(INFORMATION, "AOSRESTART", 0);
	if(AOSRESTART == 0 && dwError == STSDKEX_ERROR_SUCCESS)
	{
		AfxGetApp()->WriteProfileInt(INFORMATION, "AOSRESTART", 1);
		m_bExit = false;
		m_forceClose = true;
		m_axMisc->RunVers(verRETRY);
		//PostMessage(WM_CLOSE);
		SendMessage(WM_CLOSE);
	}
	else
		AfxGetApp()->WriteProfileInt(INFORMATION, "AOSRESTART", 0);

	return TRUE;
}

CString CMainFrame::Err2Str(DWORD dwErrCode)
{
	CString strErr;

	switch( dwErrCode )
	{
	case STSDKEX_ERROR_SUCCESS:
		{
			strErr = _T("성공");
		}
		break;
	case STSDKEX_ERROR_UNINITIALIZE_FAILED:
		{
			strErr = _T("UNINITIALIZE에 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_LOADLIBRARY_FAILED:
		{
			strErr = _T("SDK 모듈을 로드하는데 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_GETPROCADDRESS_FAILED:
		{
			strErr = _T("SDK 모듈에서 인터페이스를 찾을 수 없습니다.");
		}
		break;
	case STSDKEX_ERROR_FUNCTION_START_FAILED:
		{
			strErr = _T("기능을 시작시키는데 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_FUNCTION_STOP_FAILED:
		{
			strErr = _T("기능을 종료시키는데 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_ALREADY_INSTALLED:
		{
			strErr = _T("제품이 이미 설치되어 있습니다.");
		}
		break;
	case STSDKEX_ERROR_NOT_INITIALIZED:
		{
			strErr = _T("초기화 되지 않았습니다.");
		}
		break;
	case STSDKEX_ERROR_BAD_PARAMETER:
		{
			strErr = _T("파라미터가 이상합니다.");
		}
		break;
	case STSDKEX_ERROR_INITIALIZE_FAILED:
		{
			strErr = _T("초기화에 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_NOT_LOADLIBRARY:
		{
			strErr = _T("SDK 모듈이 로드되어 있지 않습니다.");
		}
		break;
	case STSDKEX_ERROR_REMAIN_SHAREDCOUNT:
		{
			strErr = _T("기능을 다른 곳에서 사용중이므로 종료할 수가 없습니다.");
		}
		break;
	case STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED:
		{
			strErr = _T("기능이 이미 종료된 상태입니다.");
		}
		break;
	case STSDKEX_ERROR_INVALID_CUSTOMER_ID:
		{
			strErr = _T("고객사 ID가 잘못 되었습니다.");
		}
		break;
	case STSDKEX_ERROR_INVALID_FUNCTION_CODE:
		{
			strErr = _T("기능 코드가 잘못 되었습니다.");
		}
		break;
	case STSDKEX_ERROR_PRODUCT_NOT_INSTALLED:
		{
			strErr = _T("제품이 설치되지 않았습니다.");
		}
		break;
	case STSDKEX_ERROR_DRIVER_RULE_NOT_VERIFYED:
		{
			strErr = _T("드라이버 룰이 유효하지 않습니다.");
		}
		break;
	case STSDKEX_ERROR_NOT_PROTECTED_HWND:
		{
			strErr = _T("보호하지 않는 Edit Control입니다.");
		}
		break;
	case STSDKEX_ERROR_NOT_PROPERTY_HWND:
		{
			strErr = _T("속성이 유효하지 않습니다.");
		}
		break;
	case STSDKEX_ERROR_INVALID_PROCESS_ID:
		{
			strErr = _T("프로세스 ID가 잘못 지정되었습니다.");
		}
		break;
	case STSDKEX_ERROR_DOWNLOAD_SETTING_FAILED:
		{
			strErr = _T("다운로드할 파일셋을 구성하는데 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_DOWNLOAD_UPDATESETUP:
		{
			strErr = _T("업데이트 프로그램 셋업을 받아올수 없습니다.");
		}
		break;
	case STSDKEX_ERROR_CHECK_UPDATEMODULE:
		{
			strErr = _T("업데이트 프로그램 모듈 검증이 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_SIGN_NOT_VERIFYED:
		{
			strErr = _T("서명이 올바르게 되어 있지 않습니다.");
		}
		break;
	case STSDKEX_ERROR_FUNCTION_FAILED:
		{
			strErr = _T("함수를 수행하는데 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_INVALID_SUB_FUNCTION_CODE:
		{
			strErr = _T("해킹보호 기능의 세부기능 코드가 잘못 되었습니다.");
		}
		break;
	case STSDKEX_ERROR_FUNCTION_NOT_RUNNING:
		{
			strErr = _T("기능이 동작중이지 않습니다.");
		}
		break;
	case STSDKEX_ERROR_FUNCTION_ALREADY_STARTED:
		{
			strErr = _T("기능이 이미 동작중인 상태입니다.");
		}
		break;
	case STSDKEX_ERROR_CUSTOMER_POLICY_DOWNLOAD_FAILED:
		{
			strErr = _T("고객사 정책파일(기능)을 다운로드 하는데 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_OPTION_POLICY_DOWNLOAD_FAILED:
		{
			strErr = _T("고객사 정책파일(옵션)을 다운로드 하는데 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_INVALID_EXCEPTION_PROCESS_LIST:
		{
			strErr = _T("예외프로세스 리스트가 잘못 지정되었습니다.");
		}
		break;
	case STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE:
		{
			strErr = _T("우선순위(충돌 프로그램 존재, Super룰)에 따라 상위 룰이 존재하여 요청을 수행할 수 없습니다.");
		}
		break;

	case STSDKEX_ERROR_CONNECT_FAILED:
		{
			strErr = _T("기능 연결에 실패하였습니다.");
		}
		break;
	case STSDKEX_ERROR_FUNCTION_CALL_FAILED:
		{
			strErr = _T("시스템 함수 호출에 실패하였습니다.");
		}
		break;

	case STSDKEX_ERROR_STSESS_EXEC_FAILED:
		{
			strErr = _T("STSESS 실행에 실패하였습니다.");
		}
		break;

	case STSDKEX_ERROR_STSESS_TIMEOUT_FAILED:
		{
			strErr = _T("STSESS 실행이 Timeout 되었습니다.");
		}
		break;

	case STSDKEX_ERROR_ASDSVC_NOT_RUNNING:
		{
			strErr = _T("AsdSvc 가 실행중이지 않습니다.");
		}
		break;

	case STSDKEX_ERROR_ASTX_UPDATING:
		{
			strErr = _T("ASTx 업데이트 중입니다.");
		}
		break;

	case STSDKEX_ERROR_NULL_CUSTOMER_ID:
		{
			strErr = _T("CUSTOMER ID가 NULL입니다.");
		}
		break;
		
	case STSDKEX_ERROR_GET_ACTIVE_CONSOLE_SESSION_ID:
		{
			strErr = _T("ActiveConsoleSessionId를 얻지 못했습니다.");
		}
		break;

	case STSDKEX_ERROR_CALL_AFTER_INIT:
		{
			strErr = _T("STSDKEX_Initialize()보다 먼저 호출되어야 합니다.");
		}
		break;

	case STSDKEX_ERROR_CALL_NOT_IMPLEMENTED:
		{
			strErr = _T("지원하지 않는 함수입니다.");
		}
		break;

	case STSDKEX_ERROR_PB_NULL_CALLBACK:
		{
			strErr = _T("콜백함수가 NULL입니다.");
		}
		break;
		
	case STSDKEX_ERROR_PB_NULL_HSBCTL:
		{
			strErr = _T("HSBCTL이 NULL입니다.");
		}
		break;
		
	case STSDKEX_ERROR_PB_COMPATIBLITY_MODE:
		{
			strErr = _T("호환성 모드가 설정되어 있습니다.");
		}
		break;

	case STSDKEX_ERROR_PB_CREATEOPTIONS_FAILED:
		{
			strErr = _T("CreateOptions이 실패하였습니다.");
		}
		break;

	case STSDKEX_ERROR_PB_COPYDRIVER_FAILED:
		{
			strErr = _T("드라이버 복사에 실패하였습니다.");
		}
		break;

	case STSDKEX_ERROR_PB_HSBDRV_INITIALIZE_FAILED:
		{
			strErr = _T("드라이버 초기화에 실패하였습니다.");
		}
		break;

	case STSDKEX_ERROR_PB_HSBDRV_START_FAILED:
		{
			strErr = _T("드라이버 구동에 실패하였습니다.");
		}
		break;

	case STSDKEX_ERROR_PB_HSBDRV_INSERTEXCEPTIONPROCESS_FAILED:
		{
			strErr = _T("드라이버 예외프로세스 설정에 실패하였습니다.");
		}
		break;

	case STSDKEX_ERROR_PB_HSBDRV_GETDRIVERVERSION_FAILED:
		{
			strErr = _T("드라이버 버전확인에 실패하였습니다.");
		}
		break;

	case STSDKEX_ERROR_PB_HSBDRV_LOADDRIVER_FAILED:
		{
			strErr = _T("드라이버 로드에 실패하였습니다.");
		}
		break;

	}

	return strErr;
}

BOOL CMainFrame::initASTx()
{
#ifdef _DEBUG
	return FALSE;
#endif
	m_slog.Format("[AStx][initASTx] start m_bSdkInitialized=[%d]\n ", m_bSdkInitialized);
	WriteLog(m_slog);

	DWORD dwError = STSDKEX_ERROR_SUCCESS;

	m_strCustomerID = "ibkstock";
	
	if(TRUE == m_bSdkInitialized)
	{
		OutputDebugString(_T("[AStx][initASTx] already initialized"));
		return FALSE;
	}
	
	dwError = STSDKEX_IsInstalled();
	if(STSDKEX_ERROR_PRODUCT_NOT_INSTALLED == dwError)
	{
		installASTx();
		return FALSE;
	}	
	
	// 비동기 모드 설정
	dwError = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_ASYNC,  "on");
	
	if( dwError != STSDKEX_ERROR_SUCCESS )
	{
		//( _T("[OnButtonSdkInitialize] Error SetEnvOption, STSDKEX_ENV_OPTION_ASYNC"), dwError ); 
		return FALSE;
	}
	
	// 기능 호출 Timeout 시간을 Milisecond 단위로 설정합니다.
	dwError = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_FUNCTIMEOUT,  "180000"); // 3분	
	
	if( dwError != STSDKEX_ERROR_SUCCESS )
	{
		//ShowErrorMessage( _T("[OnButtonSdkInitialize] Error SetEnvOption, STSDKEX_ENV_OPTION_FUNCTIMEOUT"), dwError ); 
		return FALSE;
	}
	
	// 독립실행모드 설정
	dwError = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_STANDALONE,  "on");
	
	if( dwError != STSDKEX_ERROR_SUCCESS )
	{
		//ShowErrorMessage( _T("[OnButtonSdkInitialize] Error SetEnvOption, STSDKEX_ENV_OPTION_STANDALONE"), dwError ); 
		return FALSE;
	}
	
	
	// 콜백함수를 설정합니다.
	STSDKEX_SetEventCallback(STSDKEX_EventCallback);
	
	// Initialize
	dwError = STSDKEX_Initialize();
	
	if( dwError != STSDKEX_ERROR_SUCCESS )
	{
		//ShowErrorMessage( _T("[OnButtonSdkInitialize] call STSDKEX_Initialize"), dwError ); 
		return FALSE;
	}

	m_bSdkInitialized = TRUE;

	m_slog.Format("[AStx][initASTx]  정상  m_bSdkInitialized=[%d]\n ", m_bSdkInitialized);
	WriteLog(m_slog);

	return TRUE;
}

#pragma warning (disable : 26494)
#pragma warning (disable : 26496)

BOOL CMainFrame::uninitASTx()
{
	if(FALSE == m_bSdkInitialized)
	{
		//AfxMessageBox(_T("[OnButtonSDKUninitialize]  not SDKInitialized"));
		return FALSE;
	}
	
	USES_CONVERSION;
	
	// 기능 종료
	DWORD dwResult = STSDKEX_ERROR_FUNCTION_FAILED;

	dwResult = STSDKEX_StopFuncA(T2A(LPSTR(LPCTSTR(m_strCustomerID))), STSDKEX_FUNC_CODE_PB|STSDKEX_FUNC_CODE_FW|STSDKEX_FUNC_CODE_PCSCAN);
	
	STSDKEX_Unintialize();
	
	m_bSdkInitialized = FALSE;

	m_slog.Format("[AStx][uninitASTx]   m_bSdkInitialized=[%d]\n ", m_bSdkInitialized);
	WriteLog(m_slog);

	return TRUE;
}

BOOL CMainFrame::startPB()
{
	USES_CONVERSION;
	
	if(FALSE == m_bSdkInitialized)
	{
		OutputDebugString("ASTx가 초기화 되지 않았습니다.");
		return FALSE;
	}
	
	const DWORD dwOption = STSDKEX_PB_ACTIVATE_PROTECT_MEMORY | STSDKEX_PB_ACTIVATE_BLOCK_REMOTE;
	
	STSDKEX_PBSetActivateSubFunc(dwOption);
	
	const DWORD dwResult = STSDKEX_StartFuncA(T2A(LPSTR(LPCTSTR(m_strCustomerID))), STSDKEX_FUNC_CODE_PB, STSDKEX_CUSTOM_POLICY_URL);
	
	if( STSDKEX_ERROR_SUCCESS == dwResult || STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE == dwResult || STSDKEX_ERROR_FUNCTION_ALREADY_STARTED == dwResult )
	{
		OutputDebugString("[ASTx]해킹방지 기능이 정상적으로 시작되었습니다.");
		WriteLog("[ASTx][startPB] 해킹방지 기능이 정상적으로 시작되었습니다. [메모리]"); 
	}
	else
	{
		CString szMsg;
		szMsg.Format(_T("[startPB]해킹방지 기능 시작중 문제가 발생하였습니다.\r\n(result=0x%08x, %s)"), dwResult, Err2Str(dwResult));
		AfxMessageBox(szMsg);
	}

	return TRUE;
}

BOOL CMainFrame::stopPB()
{
	USES_CONVERSION;
	
	if(FALSE == m_bSdkInitialized)
	{
		//AfxMessageBox(_T("[OnButtonStopPB] not SDKInitialized"));
		OutputDebugString("[ASTx] SDK가 초기화 되지 않았습니다.");
		return FALSE;
	}
	
	const DWORD dwOption = STSDKEX_PB_ACTIVATE_PROTECT_MEMORY | STSDKEX_PB_ACTIVATE_BLOCK_REMOTE;
	
	STSDKEX_PBSetActivateSubFunc(dwOption);
		
	const DWORD dwResult = STSDKEX_StopFuncA(T2A(LPSTR(LPCTSTR(m_strCustomerID))), STSDKEX_FUNC_CODE_PB);
	
	if( STSDKEX_ERROR_SUCCESS == dwResult )
	{
		OutputDebugString("[ASTx] 해킹방지 기능이 종료 되었습니다.");
		WriteLog("[ASTx][stopPB]해킹방지 기능이 종료 되었습니다. [메모리]");
	}
	else if( STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED == dwResult )
	{
		OutputDebugString("[ASTx] 해킹방지 기능이 동작중이지 않습니다.");
		WriteLog("[ASTx][stopPB]해킹방지 기능이 동작중이지 않습니다. [메모리]");
	}
	else
	{
		CString szMsg;
		szMsg.Format(_T("[stopPB]해킹방지 기능 종료중 문제가 발생하였습니다.\r\n(result=0x%08x, %s)"), dwResult, Err2Str(dwResult));
		AfxMessageBox(szMsg);
	}

	return TRUE;
}

BOOL CMainFrame::startFW()
{
	USES_CONVERSION;
	
	if(FALSE == m_bSdkInitialized)
	{
		//AfxMessageBox(_T("[OnButtonStartFW] not SDKInitialized"));
		return FALSE;
	}
	
	const DWORD dwResult = STSDKEX_StartFuncA(T2A(LPSTR(LPCTSTR(m_strCustomerID))), STSDKEX_FUNC_CODE_FW, STSDKEX_CUSTOM_POLICY_URL);
	
	if( STSDKEX_ERROR_SUCCESS == dwResult || STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE == dwResult || STSDKEX_ERROR_FUNCTION_ALREADY_STARTED == dwResult )
	{
		OutputDebugString("[ASTx] 방화벽 기능이 정상적으로 기동되었습니다.");
		WriteLog("[ASTx][startFW]방화벽 기능이 정상적으로 기동되었습니다. [방화벽]");
	}
	else
	{
		CString szMsg;
		szMsg.Format(_T("방화벽 기능 시작중 문제가 발생하였습니다.\r\n(result=0x%08x, %s)"), dwResult, Err2Str(dwResult));
		AfxMessageBox(szMsg);
	}

	return TRUE;
}

BOOL CMainFrame::stopFW()
{
	USES_CONVERSION;
	
	if(FALSE == m_bSdkInitialized)
	{
		//AfxMessageBox(_T("[OnButtonStopFW] not SDKInitialized"));
		return FALSE;
	}
	
	const DWORD dwResult = STSDKEX_StopFuncA(T2A(LPSTR(LPCTSTR(m_strCustomerID))), STSDKEX_FUNC_CODE_FW);
	
	if( STSDKEX_ERROR_SUCCESS == dwResult )
	{
		OutputDebugString(_T("[ASTx] 방화벽 기능이 종료 되었습니다."));
		WriteLog("[ASTx]방화벽 기능이 종료 되었습니다. [방화벽] stopFW()");
		return TRUE;
	}
	else if( STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED == dwResult )
	{
		OutputDebugString(_T("[ASTx] 방화벽 기능이 이미 동작중이지 않습니다."));
		WriteLog("[ASTx]방화벽 기능이 이미 동작중이지 않습니다. [방화벽] stopFW()");
		return TRUE;
	}
	else
	{
		CString szMsg;
		szMsg.Format(_T("방화벽 기능 종료중 문제가 발생하였습니다.\r\n(result=0x%08x, %s)"), dwResult, Err2Str(dwResult));
		AfxMessageBox(szMsg);
	}

	return FALSE;
}

#pragma warning (default : 26494)
#pragma warning (default : 26496)

// AK SDK Test
typedef IAstxMkd8A* (*_StSdk_GetMkd8ObjectA)(DWORD* pdwErrorCode);
typedef IAstxMkd8W* (*_StSdk_GetMkd8ObjectW)(DWORD* pdwErrorCode);

static IAstxMkd8* g_pIAstxAkSDK = NULL;

int __stdcall AKSDK_Callback(long lCode, LPARAM lParam)
{
	DWORD dwError = ERROR_SUCCESS;
	if(lParam != ERROR_SUCCESS)
	{
		dwError = (DWORD)lParam;
	}
	CString s;

	switch(lCode)
	{
	case STSDKEX_ERROR_BAD_PARAMETER:					// 0x32A41008L
		{
			s.Format(_T("STSDKEX_ERROR_BAD_PARAMETER"));
			OutputDebugString(s);
		}
	case STSDKEX_ERROR_UNINITIALIZE_FAILED:			// 0x32A41001L
		{
			// Uninitialization failed.
			s.Format(_T("STSDKEX_ERROR_UNINITIALIZE_FAILED"));
			OutputDebugString(s);
		}
		break;
	case STSDKEX_ERROR_LOADLIBRARY_FAILED:			// 0x32A41002L
		{
			// ASTX AK module loading failed.
			s.Format(_T("STSDKEX_ERROR_LOADLIBRARY_FAILED"));
			OutputDebugString(s);
		}
		break;
	case STSDKEX_ERROR_NOT_INITIALIZED:				// 0x32A41007L
		{
			//Initialization not carried out.
			s.Format(_T("STSDKEX_ERROR_NOT_INITIALIZED"));
			OutputDebugString(s);
		}
		break;
	case STSDKEX_ERROR_INITIALIZE_FAILED:				// 0x32A41009L
		{
			// Failed while initializing.
			s.Format(_T("STSDKEX_ERROR_INITIALIZE_FAILED"));
			OutputDebugString(s);
		}
		break;
	case STSDKEX_ERROR_NOT_LOADLIBRARY:				// 0x32A4100AL
		{
			// ASTX AK module is not loaded.
			s.Format(_T("STSDKEX_ERROR_NOT_LOADLIBRARY"));
			OutputDebugString(s);
		}
		break;
	case STSDKEX_ERROR_PRODUCT_NOT_INSTALLED:					// 0x32A4100FL
		{
			// ASTX AK is not installed.
			s.Format(_T("STSDKEX_ERROR_PRODUCT_NOT_INSTALLED"));
			OutputDebugString(s);
		}
		break;
	case STSDKEX_ERROR_DRIVER_RULE_NOT_VERIFYED:		// 0x32A41010L
		{
			// ASTX AK is not compatible with application.
			// Terminate all applications that use ASTX AK and restart.
			s.Format(_T("STSDKEX_ERROR_DRIVER_RULE_NOT_VERIFYED"));
			OutputDebugString(s);
		}
		break;
	case STSDKEX_ERROR_NOT_PROTECTED_HWND:			// 0x32A41011L
		{
			// Unprotected Edit Control.
			s.Format(_T("STSDKEX_ERROR_NOT_PROTECTED_HWND"));
			OutputDebugString(s);
		}
		break;
	case STSDKEX_ERROR_NOT_PROPERTY_HWND:				// 0x32A41012L
		{
			// Properties do not match.
			s.Format(_T("STSDKEX_ERROR_NOT_PROPERTY_HWND"));
			OutputDebugString(s);
		}
		break;
	default:
		break;
	}
	return 0;
}

#pragma warning (disable : 26494)
#pragma warning (disable : 26496)
BOOL CMainFrame::startAK()
{
	if(FALSE == m_bSdkInitialized)
	{
		OutputDebugString("[ASTx] 초기화 되지않았습니다.");
		WriteLog("[ASTx][startAK] m_bSdkInitialized=[FALSE] 초기화 되지않았습니다. [키보드보안])");
		return FALSE;
	}
	
	// StSdk_GetMkd8Object function call, get interface.
	// StSdk_GetMkd8Object 함수를 호출하여 Interface를 얻어 옵니다.
	DWORD dwErrorCode = STSDKEX_ERROR_SUCCESS;
	
	g_pIAstxAkSDK = STSDKEX_GetMkd8ObjectA( m_strCustomerID, &dwErrorCode );
	
	if( g_pIAstxAkSDK == NULL )
	{
		OutputDebugString("[ASTx] AK SDK 초기화에 실패했습니다.");
		WriteLog("[ASTx][startAK] AK SDK 초기화에 실패했습니다. [키보드보안]");
		return FALSE;
	}

	USES_CONVERSION;
	
	const DWORD dwInitMode = AstxmodeProtectInAstx;
	const DWORD dwErr = g_pIAstxAkSDK->Initialize(dwInitMode, AKSDK_Callback);
	if( dwErr != STSDKEX_ERROR_SUCCESS )
	{
		OutputDebugString("[ASTx] AK 초기화에 실패했습니다.");
		WriteLog("[ASTx][startAK] AK 초기화에 실패했습니다 [키보드보안]");
		return FALSE;
	}
	
	// Protect Edit Ctrl1
// 	HWND hwnd1 = m_axConnect->GetPassHwnd();
// 	DWORD dwErr1 = g_pIAstxAkSDK->ProtectEditControl(hwnd1);
// 	if( dwErr1 != 0 ) 
// 	{
// 		OutputDebugString("[ASTx] AK 비밀번호 에디트 초기화에 실패했습니다.");
// 		return FALSE;
// 	}
// 
// 	HWND hwnd2 = m_axConnect->GetCPassHwnd();
// 	DWORD dwErr2 = g_pIAstxAkSDK->ProtectEditControl(hwnd2);
// 	if( dwErr2 != 0 ) 
// 	{
// 		OutputDebugString("[ASTx] AK 인증번호 에디트 초기화에 실패했습니다.");
// 		return FALSE;
// 	}
// 
// 	m_axConnect->SetAK(g_pIAstxAkSDK);
	WriteLog("[ASTx][startAK] 키보드보안 기능이 정상적으로 기동되었습니다");
	return TRUE;
}

BOOL CMainFrame::stopAK()
{
	USES_CONVERSION;
	
	if(g_pIAstxAkSDK != NULL)
	{	
		const DWORD dwErr = g_pIAstxAkSDK->Uninitialize();
		if( dwErr != 0 )
		{
			return FALSE;
		}
		
		g_pIAstxAkSDK = NULL;
		WriteLog("[ASTx][stopAK]키보드 보안기능이  정상적으로 종료되었습니다. [키보드보안]");
	}

	return TRUE;
}

int CMainFrame::GetTheme()
{
	HKEY openKey = NULL;
	char szData[2] = "";
	char szTheme[256] = "";
	const DWORD Type = 0;
	DWORD dwType = REG_SZ;
	DWORD dwCount = sizeof(char) * 2;
	DWORD dwTheme = sizeof(char) * 256;
	
	::RegOpenKey(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\ThemeManager\\"),&openKey);
	::RegQueryValueEx(openKey,_T("ThemeActive"),0,&dwType,(LPBYTE)szData,&dwCount);
	::RegCloseKey(openKey);
	
	CString s;
	s.Format("MAIN THEME [%s]\n",szData);
//	OutputDebugString(s);
	
	if(strcmp(szData,"1") == 0)
	{
		::RegOpenKey(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\"),&openKey);
		::RegQueryValueEx(openKey,_T("CurrentTheme"),0,&dwType,(LPBYTE)szTheme,&dwTheme);
		::RegCloseKey(openKey);
		
		CString str(szTheme,256);
		
		s.Format("THEME [%s]\n",str);
	//	OutputDebugString(s);
		
		if(str.Find("basic.theme") > -1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
	
	return 0;
}

LRESULT CMainFrame::OnLockPass( WPARAM wParam, LPARAM lParam )
{
	if ((int)wParam == 1)
	{
		KillMySelf();
		return 0;
	}
	char ca[20];
	int ret{};

	memset(ca, ' ', 20);
	int ilen = Axis::userID.GetLength();
	memcpy(ca, Axis::userID, ilen);

	m_slog.Format("[idle][cert] OnLockPass lParam=[%s] \n", (char*)lParam);
	OutputDebugString(m_slog);

	if (m_iCErrCnt >= 5)
	{
		AfxMessageBox("전자서명 비밀번호 5회 오류로 HTS가 종료됩니다.\r\n 인증서를 재발급 받으시고 다시 시도해주세요");
		this->SendMessage(WM_CLOSE, 0, 0);
		return 0;
	}

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void*)&ret,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(0x24, ilen), ca);

	char* pdata = (char*)lParam;

	m_slog.Format("[idle][auth][cert] ret=[%d] \n", ret);
	OutputDebugString(m_slog);

	if ((ret == 0 && !m_bCloudeUse) || (ret == 1 && m_bCloudeUse))  
	{//공동인증서는 성공시 ret = 0,  클라우드 인증서는 성공시 ret = 1
		SendSBPGT336("S");
		memcpy(pdata, "success", 7);
	}
	else if (ret == 2417)
	{
		SendSBPGT336("E");
		memcpy(pdata, "fail", 4);
	}

	return 0;
}

#pragma warning (default : 26494)
#pragma warning (default : 26496)


BOOL CMainFrame::CheckBrowserVersion()
{
	LONG lResult{};
	HKEY hKey{};
	TCHAR keyValue[20]; 
	memset(keyValue, 0, sizeof(keyValue));
	DWORD dwType = REG_SZ; 
	DWORD dwSize = sizeof(keyValue);
	
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Internet Explorer"), 0, KEY_QUERY_VALUE, &hKey);
	
	if(lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}
	
	lResult = RegQueryValueEx(hKey, _T("svcVersion"), 0, &dwType, (LPBYTE)keyValue, &dwSize);
	if(lResult != ERROR_SUCCESS)
	{
		lResult = RegQueryValueEx(hKey, _T("Version"), 0, &dwType, (LPBYTE)keyValue, &dwSize);
		
		if(lResult != ERROR_SUCCESS)
		{
			return FALSE;
		}
	}
	
	CString sValue;
	sValue = keyValue;

	//AfxMessageBox(sValue);
	
	if(sValue.GetAt(0) == '7')
		return TRUE;
	
	RegCloseKey(hKey);
	
	return FALSE;
}

#pragma warning(disable : 26430)
#pragma warning(disable : 26400)
#pragma warning(disable : 26409)
#pragma warning(disable : 6011)
void CMainFrame::PBMngShow(CString strMsg)
{
	if(m_ShowSlide == 0)
		return;

	CString stmp;
	int kind;
	kind = 4;

	CDlgPB* pMngPB;
	pMngPB = new CDlgPB(this);     
	if(!pMngPB->Create(IDD_DLGPB))
	{
		delete pMngPB;
		pMngPB = NULL;
	}

	pMngPB->SetData(strMsg, kind);
	
	CRect		mRc;
	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);
	switch (pl.showCmd)
	{
		case SW_HIDE:case SW_MINIMIZE:case SW_SHOWMINIMIZED:
			m_mngInfo->ShowWindow(SW_HIDE);
			
			return;
		default:
			break;
	}
	pMngPB->ShowWindow(SW_HIDE);
	
	//int	x, y;
	int	width = 0, height = 0;
	
	pMngPB->GetSize(width, height);
	mRc = getPBArrRect(pMngPB, width, height);

	pMngPB->ShowPBSlide(mRc);
}

//void CMainFrame::HidePBArrItem(CWnd* pwnd)
//{
//	if(m_arrOrdRect.GetSize() == 0)
//		return;
//
//	CPBRect* pArrRect = NULL;
//	for(int ii = 0 ; ii < m_arrOrdRect.GetSize() ; ii++)
//	{
//		pArrRect = m_arrOrdRect.GetAt(ii);
//		if(pArrRect->pWnd == pwnd)
//			pArrRect->bShow = false;
//	}
//
//	if(!CheckArrRectShow())
//		m_arrOrdRect.RemoveAll();
//}
void CMainFrame::HidePBArrItem(CWnd* pwnd)
{
	if (m_arrOrdRect.empty())
		return;

	for (auto& item : m_arrOrdRect)
	{
		if (item->pWnd == pwnd)
			item->bShow = false;
	}

	if (!CheckArrRectShow())
		m_arrOrdRect.clear();   // 자동 delete
}

//bool CMainFrame::CheckArrRectShow()
//{
//	bool bShow = false;
//	CPBRect* pArrRect = NULL;
//	for(int ii = 0 ; ii < m_arrOrdRect.GetSize() ; ii++)
//	{
//		pArrRect = m_arrOrdRect.GetAt(ii);
//		if(pArrRect->bShow == true)
//			bShow = true;	
//	}
//
//	return bShow;
//}
bool CMainFrame::CheckArrRectShow()
{
	for (const auto& item : m_arrOrdRect)
	{
		if (item->bShow)
			return true;
	}

	return false;
}

//CRect CMainFrame::getPBArrRect(CWnd* pdlg, int iwidth, int iheight)
//{
////	int index;
//	CPBRect* pArrRect = NULL;
//
//	if(m_arrOrdRect.GetSize() == 0 ) //단독(최초)으로 뜨게 된경우
//	{
//		
//		CRect mRc;
//		mRc = GetWndRect();
//		const int x = mRc.right - iwidth - GetSystemMetrics(SM_CXFIXEDFRAME);
//		const int y = mRc.bottom - iheight - GetSystemMetrics(SM_CYFIXEDFRAME);
//			
//		CRect rect;
//		rect = mRc;
//		rect.left = x;
//		rect.top = y;
//		rect.right = rect.left + iwidth;
//		rect.bottom = rect.top + iheight;
//		
//		pArrRect = new CPBRect;
//		pArrRect->bShow = true;
//		pArrRect->rect = rect;
//		pArrRect->pWnd = pdlg;
//		pArrRect->MainRect = GetWndRect();
//		m_arrOrdRect.Add(pArrRect);
//		return rect;
//	}
//
//	//이미 PB 다이알로그가 떠있는 상태인대 또 내려온경우
//	CRect beforeRect;
//	for(int ii = 0 ; ii < m_arrOrdRect.GetSize() ; ii++)
//	{
//		pArrRect = m_arrOrdRect.GetAt(ii);
//		if(pArrRect->bShow == false) 
//		{
//			pArrRect->bShow = true;
//			pArrRect->pWnd = pdlg;
//
//			if(!CompareRect(pArrRect->MainRect, GetWndRect()))
//			{
//				
//				pArrRect->MainRect = GetWndRect();
//				pArrRect->rect = getBasePBRect(iwidth, iheight);
//				m_arrOrdRect.RemoveAll();
//				m_arrOrdRect.Add(pArrRect);
//				return getBasePBRect(iwidth, iheight);
//			}
//			else
//			{
//				m_arrOrdRect.SetAt(ii, pArrRect);
//				return m_arrOrdRect.GetAt(ii)->rect;
//			}
//		}
//		else 
//			beforeRect = m_arrOrdRect.GetAt(ii)->rect;  //가장 마지막 존재하는 PBDlg의 postion
//	}
//	
//	if((pArrRect != NULL) && !CompareRect(pArrRect->MainRect, GetWndRect()))
//	{//팝업창이 떠있는 상태에서 HTS 메인전체를 옮겼을때
//		beforeRect = getBasePBRect(iwidth, iheight);
//		m_arrOrdRect.RemoveAll();
//	}
//	else	
//		beforeRect.OffsetRect(0, iheight * -1); 
//
//	if(beforeRect.top < 0)  
//	{
//		beforeRect.left -= iwidth;
//		beforeRect.bottom = getBasePBRect(iwidth, iheight).bottom;
//		beforeRect.right = beforeRect.left + iwidth;
//		beforeRect.top = beforeRect.bottom - iheight;
//	}
//
//	pArrRect = new CPBRect;
//	pArrRect->bShow = true;	
//	pArrRect->rect = beforeRect;
//	pArrRect->MainRect = GetWndRect();
//	pArrRect->pWnd = pdlg;
//	m_arrOrdRect.Add(pArrRect);
//	return  beforeRect;  
//}
CRect CMainFrame::getPBArrRect(CWnd* pdlg, int iwidth, int iheight)
{
	if (m_arrOrdRect.empty())
	{
		CRect mRc = GetWndRect();

		const int x = mRc.right - iwidth - GetSystemMetrics(SM_CXFIXEDFRAME);
		const int y = mRc.bottom - iheight - GetSystemMetrics(SM_CYFIXEDFRAME);

		CRect rect = mRc;
		rect.left = x;
		rect.top = y;
		rect.right = rect.left + iwidth;
		rect.bottom = rect.top + iheight;

		auto newItem = std::make_unique<CPBRect>();
		newItem->bShow = true;
		newItem->rect = rect;
		newItem->pWnd = pdlg;
		newItem->MainRect = GetWndRect();

		m_arrOrdRect.push_back(std::move(newItem));

		return rect;
	}

	CRect beforeRect;

	for (auto& item : m_arrOrdRect)
	{
		if (!item->bShow)
		{
			item->bShow = true;
			item->pWnd = pdlg;

			if (!CompareRect(item->MainRect, GetWndRect()))
			{
				item->MainRect = GetWndRect();
				item->rect = getBasePBRect(iwidth, iheight);

				m_arrOrdRect.clear();
				m_arrOrdRect.push_back(std::move(item));

				return getBasePBRect(iwidth, iheight);
			}

			return item->rect;
		}
		else
		{
			beforeRect = item->rect;
		}
	}

	if (!m_arrOrdRect.empty() &&
		!CompareRect(m_arrOrdRect.back()->MainRect, GetWndRect()))
	{
		beforeRect = getBasePBRect(iwidth, iheight);
		m_arrOrdRect.clear();
	}
	else
	{
		beforeRect.OffsetRect(0, iheight * -1);
	}

	if (beforeRect.top < 0)
	{
		beforeRect.left -= iwidth;
		beforeRect.bottom = getBasePBRect(iwidth, iheight).bottom;
		beforeRect.right = beforeRect.left + iwidth;
		beforeRect.top = beforeRect.bottom - iheight;
	}

	auto newItem = std::make_unique<CPBRect>();
	newItem->bShow = true;
	newItem->rect = beforeRect;
	newItem->MainRect = GetWndRect();
	newItem->pWnd = pdlg;

	m_arrOrdRect.push_back(std::move(newItem));

	return beforeRect;
}

void CMainFrame::OpenPBNews(CString strdata)  //HOB
{
	ShellExecute(NULL, _T("open"), "HUB.exe", NULL,NULL, SW_SHOWNORMAL); 
	/*
	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\CHOKNTALK.ini", Axis::home, USRDIR, usnm); 
	WritePrivateProfileString("CHOKNTALK", "Clickpopup", "1", file);

	m_mapHelper->CreateChild("IB771000", 1);
	m_bar1->PBMsgFromMain("1");  //khs
	*/
}

CRect CMainFrame::getBasePBRect(int iwidth, int iheight)
{
	CRect mRc;
	mRc = GetWndRect();
	const int x = mRc.right - iwidth - GetSystemMetrics(SM_CXFIXEDFRAME);
	const int y = mRc.bottom - iheight - GetSystemMetrics(SM_CYFIXEDFRAME);
	
	CRect rect;
	rect = mRc;
	rect.left = x;
	rect.top = y;
	rect.right = rect.left + iwidth;
	rect.bottom = rect.top + iheight;

	return rect;
}

bool CMainFrame::CompareRect(CRect aRect, CRect bRect)
{
	bool bResult = true;
	
	if(aRect.left != bRect.left)
		bResult = false;
	if(aRect.top != bRect.top)
		bResult = false;
	if(aRect.right != bRect.right)
		bResult = false;
	if(aRect.right != bRect.right)
		bResult = false;

	return bResult;
}
#pragma warning(default : 26430)
#pragma warning(default : 26400)
#pragma warning(default : 26409)
#pragma warning(default : 6011)

void CMainFrame::ReadAxisGlb()
{
	OutputDebugString("CMainFrame::ReadAxisGlb ");
	m_arrGlb.RemoveAll();

	CString	keys, secs, slog;
	char	wb[256]{};
	DWORD	rc{};

	CString path;	 
	path.Format("%s\\tab\\AXGLB.INI", Axis::home); 


	if (Axis::isCustomer)
		secs = _T("Server_IP");
	else
		secs = _T("Staff_Server_IP");

slog.Format("GLB secs=%s\n", secs);
OutputDebugString(slog);

	for (int ii = 0; ; ii++)
	{
		keys.Format("%02d", ii);
		rc = GetPrivateProfileString(secs, keys, "", wb, sizeof(wb), path);
		if (rc <= 0)
			break;
		
		m_arrGlb.Add(wb);
	
slog.Format("GLB m_arrGlb size=%d  [%s]\n", m_arrGlb.GetSize(), wb);
OutputDebugString(slog);		

	}
}

void CMainFrame::GetASTxInstall()  //20191212 ASTx 설치체크
{
	m_DInstallASTx = STSDKEX_IsInstalled();
}

void CMainFrame::LoadNoTwoPOP_NoSaveLast()
{
	m_arNoTwoPop.RemoveAll()  ;
	m_arNoSaveLastmap.RemoveAll();

	char	buf[512];
	CString	file, stmp, smap;
	file.Format("%s\\tab\\axis.ini", Axis::home);
	DWORD dw = GetPrivateProfileString("NoTwoPop", "map", "", buf, sizeof(buf), file);

	stmp.Format("%s", buf); stmp.TrimRight();

	while (!stmp.IsEmpty())
	{
		smap = Parser(stmp, ",");
		Trim(smap);
		if (!smap.IsEmpty())
			m_arNoTwoPop.Add(smap);
	}

	stmp.Empty();
	dw = GetPrivateProfileString("NoSaveLast", "map", "", buf, sizeof(buf), file);

	stmp.Format("%s", buf); stmp.TrimRight();
	
	while (!stmp.IsEmpty())
	{
		smap = Parser(stmp, ",");
		Trim(smap);
		if (!smap.IsEmpty())
			m_arNoSaveLastmap.Add(smap);
	}
}


//두번 열리지 않는 화면인경우 
//현재 열려있는지 확인후에 안열려 있으면 열고 열려있으면 포커스를 준다
bool CMainFrame::IsNoTwopop(CString strmap)
{  
CString slog;
	bool bfind = false;

	if(m_arNoTwoPop.GetSize() == 0)
		return false;

	for(int ii = 0; ii < m_arNoTwoPop.GetSize() ; ii++)
	{
		if(strmap == m_arNoTwoPop.GetAt(ii))
			bfind = true;
	}

//slog.Format("[AXIS]_ [start IsNoTwopop] map strmap=[%s] find= [%d] m_vsN =[%d] \n", strmap, bfind, m_vsN);
//OutputDebugString(slog);

	if(bfind)	
	{
		CChildFrame*	child = NULL;
		int tkey{};
		for (POSITION pos = m_arMDI[m_vsN].GetStartPosition(); pos; )
		{
			m_arMDI[m_vsN].GetNextAssoc(pos, tkey, child);
			if (child && child->GetSafeHwnd())
			{
//slog.Format("[AXIS]_ MDI map strmap=[%s] child->m_mapN= [%s]  \n", strmap, child->m_mapN);
//OutputDebugString(slog);
				if(child->m_mapN == strmap)
				{
//slog.Format("[AXIS]_ map FIND!!!! strmap=[%s] findmap= [%s]  \n", strmap, child->m_mapN);
//OutputDebugString(slog);
					if(child->m_bIconic)   
						child->SendMessage(WM_SYSCOMMAND, SC_RESTORE);
					else
						child->BringWindowToTop();

					return true;
				}
			}
		}
		
		
		for (POSITION pos = m_arSDI[m_vsN].GetStartPosition(); pos; )
		{
			CSChild*	schild = NULL;
			m_arSDI[m_vsN].GetNextAssoc(pos, tkey, schild);
			if (schild && schild->GetSafeHwnd())
			{
//slog.Format("[AXIS]_ SDI map strmap=[%s] child->m_mapN= [%s]  \n", strmap, child->m_mapN);
//OutputDebugString(slog);
				if(schild->m_mapN == strmap)
				{
//slog.Format("[AXIS]_ map SDI FIND!!!! strmap=[%s] findmap= [%s]  \n", strmap, child->m_mapN);
//OutputDebugString(slog);
					schild->BringWindowToTop();
					return true;
				}
			}
		}
	}
	
	return false;
}

bool CMainFrame::IsNoSaveLastmap(CString strmap)
{
	CString slog;
	if(m_arNoSaveLastmap.GetSize() == 0)
		return false;

	for(int ii = 0; ii < m_arNoSaveLastmap.GetSize() ; ii++)
	{
		if(strmap == m_arNoSaveLastmap.GetAt(ii))
		{
//slog.Format("[AXIS]_ map [IsNoSaveLastmap] SDI FIND!!!! strmap=[%s]  \n", strmap);
//OutputDebugString(slog);
			return true;
		}
	}
	
	return false;
}

void   CMainFrame::signOnSimpleAuth(char* pdata)
{
m_slog.Format("[QRCODE] signOnSimpleAuth [%d][%s]", strlen(pdata), pdata);
OutputDebugString(m_slog);

	m_axConnect->SetGuide(_T("사용자정보 확인 중 입니다."));
	BOOL rc = FALSE;

	CString	str;
	
	const int nEncSize = 0;
	
	char	wb[7590]{};

	struct	i_pc {
		char    header[20]{};
		char	user[12]{};
		char	pass[10]{};
		char	dats[10]{};
		char	cpas[30]{};
		char	uips[15]{};
		char	madr[16]{};
		char    sdat[7476]{};         /* 인증 DN값 */
	} signPC;

	m_step = axOPENSIGN;
//	str = m_axConnect->GetSignInfo();
	str = "5101";

	m_axConnect->SetProgress(false);
	m_axConnect->SetStatus(SM_WSH);
	m_axConnect->SetChoice(false);

	memset(&signPC, ' ', sizeof(struct i_pc));
	
	CopyMemory(signPC.header, "HTS", 3);
	CopyMemory(signPC.dats, str, str.GetLength());
	
	GetLocalIP();

	CopyMemory(signPC.uips, m_ipAddr, m_ipAddr.GetLength());
	
	char	macaddr[20], ip[16];
	memset(macaddr, 0x00, sizeof(macaddr));
	memset(ip, 0x00, sizeof(ip));
	memcpy(ip, signPC.uips, sizeof(signPC.uips));

	CString s;
	s.Format("MAC INIT [%s]\n",macaddr);
	OutputDebugString(s);
	GetMacAddr(ip, macaddr);
	s.Format("MAC ADDR RESULT [%s]\n",macaddr);
	OutputDebugString(s);
	memcpy(signPC.madr, macaddr, strlen(macaddr));

	if (m_ip.IsEmpty())
	{
		CString glbip(get_glb_addr(macaddr, ip));
		
		if (!glbip.IsEmpty())
			m_ip = glbip;
	}


	CopyMemory(signPC.sdat, pdata, strlen(pdata));   //여기가 핵심이다. 인증서값을 넣어줘야 한다. 

	int nBytes = sizeof(i_pc);
	CopyMemory(wb, (char *)&signPC, nBytes);
	wb[nBytes] = '\0';

	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), signUSERc, (long)wb, nBytes);


	if (!rc)
	{
		m_step = axNONE;
		m_axConnect->SetChoice(true);
		m_axMisc->GetGuide(AE_ESIGNON, str);
		//m_axConnect->SetGuide(str);
		MessageBox(str, "IBK투자증권.", MB_OK);
		return;
	}

	CString stmp;
	stmp.Format("%s|%s", m_ip, m_port);
	nBytes = stmp.GetLength();
	char	buff[100];
	CopyMemory(buff, (char*)(LPSTR)(LPCTSTR)stmp, nBytes);
	buff[nBytes] = '\0';

	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), 0x08, (long)buff, nBytes);
	
}

void   CMainFrame::Check_XECUREPATH()
{
	CString stmp, filePath;
	char buff[128];
	CString iniConf = Axis::home + "\\tab\\axis.ini";
	GetPrivateProfileString("XECURE", "size", "10", buff, sizeof(buff) - 1, iniConf);
	stmp.Format("%s", buff);
	int maxSizeInKB = _ttoi(stmp);
	filePath = Axis::home + _T("\\exe\\xc.log");
	CFileStatus fileStatus;
	if (CFile::GetStatus(filePath, fileStatus))
	{
		ULONGLONG fileSize = fileStatus.m_size;

		if (fileSize >= (ULONGLONG)maxSizeInKB * 1024)
		{
			// 파일 크기가 maxSizeInKB 이상인 경우 파일을 삭제합니다.
			DeleteFile(filePath);
		}
	}

	memset(buff, 0x00, 128);

	GetPrivateProfileString("XECURE", "level", "0", buff, sizeof(buff) - 1, iniConf);
	stmp.Format("%s", buff);
	int ilevel = _ttoi(stmp);

	CString rootPath, logPath, logLevel;
	filePath = Axis::home + _T("\\exe\\xc_conf.ini");
	rootPath = "Module Directory = " + Axis::home + "\\xc32_win64\\";
	logPath = "LOG DIR = " + Axis::home + "\\exe\\";
	logLevel.Format("LOG LEVEL 	= %d", ilevel);

	CStdioFile fileB;
	CString strLine;
	CString sWriteBuf;
	BOOL bRightPath{};
	
	if (!fileB.Open(filePath, CFile::modeRead))
	{
		m_slog.Format("[axis][MainFrame][Check_XECUREPATH] XECURE File Open fiil [%d]\n", GetLastError());
		XMSG(m_slog);
		return;
	}

	bRightPath = TRUE;
	while (fileB.ReadString(strLine))
	{
		OutputDebugString(strLine + "\n");
		if (strLine.Find("Module Directory") >= 0)
		{
			if (strLine != rootPath)
			{
				bRightPath = FALSE;
				strLine = rootPath;
			}
		}
		else if (strLine.Find("LOG DIR") >= 0)
		{
			if (strLine != logPath)
			{
				bRightPath = FALSE;
				strLine = logPath;
			}
		}
		else if (strLine.Find("LOG LEVEL 	=") >= 0)
		{
			if (strLine != logLevel)
			{
				bRightPath = FALSE;
				strLine = logLevel;
			}
		}
		sWriteBuf += strLine;
		sWriteBuf += "\r\n";
	}

	fileB.Close();

	if (!bRightPath)
	{
		::DeleteFile(filePath);
		CFile cfile;
		if (cfile.Open(filePath, CFile::modeWrite | CFile::modeCreate))
		{
			cfile.Write((LPSTR)(LPCTSTR)sWriteBuf, sWriteBuf.GetLength());
			cfile.Close();
		}
	}
}

void   CMainFrame::FileMove()
{
	char buff[128];
	CString iniConf = Axis::home + "\\tab\\axis.ini";
	GetPrivateProfileString("move", "file", "", buff, sizeof(buff) - 1, iniConf);

	CString strpath, strfile;
	strpath.Format("%s", buff);
	strpath.TrimRight();

	strfile = Parser(strpath, ";");

	iniConf = Axis::home +  "\\tab\\";
	iniConf += strfile;
	CString strOripath;
	strOripath = iniConf;

	CFileFind cfFind;
	BOOL bFind = cfFind.FindFile(iniConf);

	if (bFind)
	{
		int ifind = iniConf.ReverseFind('\\');
		iniConf = iniConf.Left(ifind);
		iniConf += "\\";
		iniConf += strfile;
		iniConf.Replace("tab", strpath);
		CopyFile(strOripath, iniConf, FALSE);
		//DeleteFile(strOripath);
	}
}

bool CMainFrame::MapCheckAndSendMsg(CString sMsg)
{
	int key{};
	const long rc = 0;
	POSITION pos;
	CChildFrame* child{};
	CSChild* schild{};

	bool bfind{};
	for (int ii = 0; ii < 6; ii++)
	{
		for (pos = m_arMDI[ii].GetStartPosition(); pos;)
		{
			m_arMDI[ii].GetNextAssoc(pos, key, child);
			if (child->m_mapN == SERVER_ORDERMAP)
			{
				bfind = true;
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
					(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sMsg);
			}
		}
	}

	for (int ii = 0; ii < 6; ii++)
	{
		for (pos = m_arSDI[ii].GetStartPosition(); pos;)
		{
			m_arSDI[ii].GetNextAssoc(pos, key, schild);
			if (schild->m_mapN == SERVER_ORDERMAP)
			{
				bfind = true;
				m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void*)NULL,
					(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)sMsg);
			}
		}
	}
	return bfind;
}

void CMainFrame::ServerOrderMsgToMap(int igubn, bool bPop)
{
	bool bSend{};
	CString smsg;
	switch (igubn)
	{
		case SERVERORDER_MSG_RELOAD:
		{
			smsg.Format("%s\t%d", SERVER_ORDER_MSGEDIT, igubn);
			MapCheckAndSendMsg(smsg);
		}
		break;
		case SERVERORDER_MSG_SVCREGI: break;  //서비스 신청되었음
		case SERVERORDER_MSG_SVCTERMINATE: break; //서비스 해지
		case SERVERORDER_MSG_SELLCONOK:  //(매도) 감시조건을 충족  /주문내역보기
		case SERVERORDER_MSG_BUYCONOK:  //(매수) 감시조건을 충족  /주문내역보기 
		case SERVERORDER_MSG_SELLNEWCONOK: //(신규편입매도) 감시조건을 충족  /주문내역보기
		{
			smsg.Format("%s\t%d", SERVER_ORDER_MSGEDIT, igubn);
			bSend = MapCheckAndSendMsg(smsg);
			if (!bSend && bPop)
			{
				smsg.Format("%s%s\t%d", SERVER_ORDERMAP, SERVER_ORDER_MSGEDIT, igubn);
				m_mapHelper->CreateChild(smsg, 0);
			}
		}
		break;
		case SERVERORDER_MSG_CONEXPD: //조건만료  /감시내역보기
		{
			smsg.Format("%s\t%d", SERVER_ORDER_MSGEDIT, igubn);
			bSend = MapCheckAndSendMsg(smsg);
			if (!bSend && bPop)
			{
				smsg.Format("%s%s\t%d", SERVER_ORDERMAP, SERVER_ORDER_MSGEDIT, igubn);
				m_mapHelper->CreateChild(smsg, 0);
			}
		}
		break;
		case SERVERORDER_MSG_MISORDER: //착오주문 관련
		{
			smsg.Format("%s\t%d", SERVER_ORDER_MSGEDIT, igubn);
			bSend = MapCheckAndSendMsg(smsg);
			if (!bSend && bPop)
			{
				smsg.Format("%s%s\t%d", SERVER_ORDERMAP, SERVER_ORDER_MSGEDIT, igubn);
				m_mapHelper->CreateChild(smsg, 0);
			}
		}
		case SERVERORDER_MSG_CONSTATUE:  //감시현황
		{
		}
		break;
		default:
			
			return;
			break;
	}
}

#ifdef DF_MAIN_RTS	

#endif


#pragma warning (default : 4477)
/*
void CMainFrame::SendCBSerachTR()
{

	char* pdata;
	struct _pibfjinf{
		char gubn;
		char usid[8];
	};

	char igubn = 'S';   //메인에서 호출시 S  맵에서 호출시 Q
	struct _pibfjinf *pPibfjinf;
	pdata = new char[sizeof(struct _pibfjinf) + 1];
	memset(pdata, 0x00, sizeof(struct _pibfjinf));
	pPibfjinf = (struct _pibfjinf*)pdata;

	pPibfjinf->gubn = igubn;
	memcpy((char*)&pPibfjinf->usid[0], (LPSTR)(LPCTSTR)Axis::userID, Axis::userID.GetLength());
//	memcpy((char*)&pPibfjinf, (char*)pdata, sizeof(struct _pibfjinf));

CString slog;
slog.Format("[cb] SendCBSerachTR = [%s] [%s]\n ", (char*)pPibfjinf, Axis::userID);
OutputDebugString(slog);


	sendTR("pibfjinf", pdata, sizeof(struct _pibfjinf), 0, 'C');
}
*/
//1 코스피 발동
//2 코스닥 발동
//3 코스피, 코스닥 발동
/*
void CMainFrame::ParseRTSMessage(char* pdata, int len)
{
	//내려준 데이터를 보고 CB인지 확인한다
	int iGubn = 0;
	CString sgubn;
	sgubn.Format("%s", pdata);

	CString slog;
	slog.Format("[cb]  ParseRTSMessage = [%s] [%s]\n ", pdata, sgubn);
	OutputDebugString(slog);

	if(sgubn == "+1")		//1 코스피 발동
	{
		PBMngShow("코스피 발동", 1);
	}
	else if(sgubn == "+2")  //2 코스닥 발동
	{
		PBMngShow("코스닥 발동", 2);
	}
	else if(sgubn == "+3")  //3 코스피, 코스닥 발동
	{
		PBMngShow("코스피 발동", 1);
		PBMngShow("코스닥 발동", 2);
	}
}
*/
/*
		//20200402 다중모니터  //모니터 배열을 변경한경우 발생하는 경우가 많음
		CString slog;
		int iWidth = 0;
		int iHeight = 0;
		int iMonitorCount = GetSystemMetrics(SM_CMONITORS);   //모니터 개수

		DISPLAY_DEVICE dd;
		DEVMODE dm;

		CRect* MonitorFromRect = NULL;
		MonitorFromRect = new CRect[iMonitorCount];

		for(int ii = 0 ; ii < iMonitorCount; ii++)
		{
			memset(&dd, 0, sizeof(DISPLAY_DEVICE));
			dd.cb = sizeof(DISPLAY_DEVICE);
			
			EnumDisplayDevices(NULL, ii, &dd, 0);
			memset(&dm, 0, sizeof(DEVMODE));
			
			if(EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
			{
				MonitorFromRect[ii].left = dm.dmPosition.x;
				MonitorFromRect[ii].top = dm.dmPosition.y;
				MonitorFromRect[ii].right = dm.dmPosition.x + dm.dmPelsWidth;
				MonitorFromRect[ii].bottom = dm.dmPosition.y + dm.dmPelsHeight;

				iWidth = MonitorFromRect[ii].right;
				iHeight = MonitorFromRect[ii].bottom;

slog.Format("[MIAN_MONITOR] [%s] left=[%d] top=[%d] right=[%d] bottom=[%d] iWidth=[%d]  iHeight=[%d]\n", dd.DeviceName, MonitorFromRect[ii].left, MonitorFromRect[ii].top, 
																		 dm.dmPelsWidth, dm.dmPelsHeight, iWidth, iHeight);

OutputDebugString(slog);
			}
		}
*/

/*
LRESULT CMainFrame::OnGoodOrder( WPARAM wParam, LPARAM lParam )
{
switch((int)lParam)
{
case 0:
{
m_mapHelper->CreateChild("AM070101",0,0,CenterPOS,CPoint(400,400));
}
break;
case 1:  //코스피
{
m_mapHelper->CreatePopup("IB0000E1",1,WK_POPUP, 5);	//centerpos = 5
}
break;
case 2:  //코스닥
{
m_mapHelper->CreatePopup("IB0000E1",1,WK_POPUP, 5);	//centerpos = 5
}
break;
}

  return 0;
}
*/
/*
void CMainFrame::PBMngShow(CString strMsg, int iGubn)
{
//	if(m_ShowSlide == 0)
//		return;

  CString stmp;
  int kind;
  kind = 4;
  
	CDlgPB* pMngPB;
	pMngPB = new CDlgPB(this);     
	if(!pMngPB->Create(IDD_DLGPB))
	{
	delete pMngPB;
	pMngPB = NULL;
	return;
	}
	
	  pMngPB->m_igubn = iGubn;
	  pMngPB->SetData(strMsg, iGubn);
	  
		CRect		mRc;
		WINDOWPLACEMENT	pl;
		GetWindowPlacement(&pl);
		switch (pl.showCmd)
		{
		case SW_HIDE:case SW_MINIMIZE:case SW_SHOWMINIMIZED:
		m_mngInfo->ShowWindow(SW_HIDE);
		
		  return;
		  default:
		  break;
		  }
		  pMngPB->ShowWindow(SW_HIDE);
		  
			int	x, y;
			int	width = 0, height = 0;
			
			  pMngPB->GetSize(width, height);
			  mRc = getPBArrRect(pMngPB, width, height);
			  
				pMngPB->ShowPBSlide(mRc);
}
*/



//XCERE 사용시 필요한 모듈-> 폴더환경 구성 
#define DF_XECURE_FILE  "xc32_win64.zip"
#define DF_XECURE_FOLDER  "xc32_win64"
void CMainFrame::UnzipXecure()
{
	ZIPENTRY ze{};
	CFileFind ff;
	CString xcure_path, dir, srcfile, strguide, stmp;
	xcure_path = Axis::home + "\\exe\\XC32_WIN64_test.zip";

	if (ff.FindFile(xcure_path))
	{
		CString sCopyfile;
		sCopyfile.Format("%s\\\%s", Axis::home, DF_XECURE_FILE);
		CopyFile(xcure_path, sCopyfile, true);
		int ierr = GetLastError();
		strguide = "HTS XCURE 업데이트중";
			
		//unzip
		const int	pos = sCopyfile.ReverseFind('.');
		if (pos == -1)
			return;
		dir = sCopyfile.Left(pos);
		CString s;

		if (_taccess(xcure_path, 04))
			return;

		HZIP hz = OpenZip((char*)(const char*)sCopyfile, 0, ZIP_FILENAME);

		if (hz)
		{
			ZRESULT zr = GetZipItem(hz, -1, &ze);
			if (zr == ZR_OK)
			{
				CreateFolder(dir);
				const int itemCnt = ze.index;
				int ifind = -1;

				for (int ii = 0; ii < itemCnt; ii++)
				{

					for (int jj = 0; jj < ii % 4; jj++)
						stmp += ".";
					m_axConnect->SetGuide(strguide + stmp);
			
					GetZipItem(hz, ii, &ze);
					srcfile.Format("[%s]    %s\\%s", strguide + stmp,  dir, ze.name);
					stmp.Empty();
					OutputDebugString(srcfile + "\r\n");
					srcfile.Replace('/', '\\');
					ifind = srcfile.ReverseFind('\\');
					int ilen = srcfile.GetLength();
					if (itemCnt > 1)
					{
						if (ii == 0 || ilen - 1 == ifind)
						{
							CreateFolder(srcfile);
						}
						else
						{
							zr = UnzipItem(hz, ii, (char*)(const char*)srcfile, 0, ZIP_FILENAME);
						}
					}
					else
					{
						zr = UnzipItem(hz, ii, (char*)(const char*)srcfile, 0, ZIP_FILENAME);
					}
				}
				CloseZip(hz);
				::DeleteFile(sCopyfile);
				::DeleteFile(xcure_path);
			}
		}
	}//if

}


void CMainFrame::CreateFolder(CString strTargetPath)
{
	if (_taccess(strTargetPath, 0) != 0)
	{
		BOOL l_bExtractSucc = TRUE;

		int j = 0;

		CString strDirPath = _T("");
		CString strNowPath = _T("");

		while (l_bExtractSucc)
		{
			l_bExtractSucc = AfxExtractSubString(strNowPath, strTargetPath, j, '\\');

			strDirPath += strNowPath + _T("\\");

			if (l_bExtractSucc && _taccess(strDirPath, 0) != 0)
			{
				CreateDirectory(strDirPath, NULL);
			}

			j++;
		}
	}
}

UINT AFXAPI CMainFrame::HashDataAXIS(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}

void CMainFrame::Delete_AsisICon()
{
	CString fname;
	fname = Axis::home + "\\tab\\ICON.bat";
	CFileFind finder;
	if (finder.FindFile(fname))
	{
		m_slog.Format("[axis] Delete_AsisICon");
		OutputDebugString(m_slog);
		ShellExecute(NULL, _T("open"), fname, NULL, NULL, SW_SHOWNORMAL);
		::DeleteFile(fname);
	}
}

void CMainFrame::CheckServer(CString strip)
{
	if (strip.Find("211.255.204.70") >= 0) m_strServer = "[BP10]";
	else if (strip.Find("211.255.204.71") >= 0) m_strServer = "[BP11]";
	else if (strip.Find("211.255.204.72") >= 0) m_strServer = "[BP12]";
	else if (strip.Find("211.255.204.73") >= 0) m_strServer = "[BP13]";
	else if (strip.Find("211.255.204.74") >= 0) m_strServer = "[BP14]";
	else if (strip.Find("211.255.204.75") >= 0) m_strServer = "[BP15]";
	else if (strip.Find("211.255.204.76") >= 0) m_strServer = "[BP16]";
	else if (strip.Find("211.255.204.77") >= 0) m_strServer = "[BP17]";
	else if (strip.Find("211.255.204.78") >= 0) m_strServer = "[BP18]";
	else if (strip.Find("211.255.204.79") >= 0) m_strServer = "[BP19]";
	else if (strip.Find("211.255.204.33") >= 0) m_strServer = "[BP20]";
	else if (strip.Find("211.255.204.34") >= 0) m_strServer = "[BP21]";
	else if (strip.Find("211.255.204.35") >= 0) m_strServer = "[BP22]";
	else if (strip.Find("211.255.204.36") >= 0) m_strServer = "[BP23]";
	else if (strip.Find("211.255.204.37") >= 0) m_strServer = "[BP24]";
	else if (strip.Find("211.255.204.38") >= 0) m_strServer = "[BP25]";
	else if (strip.Find("211.255.204.39") >= 0) m_strServer = "[BP26]";
	else if (strip.Find("211.255.204.57") >= 0) m_strServer = "[BP27]";
	else if (strip.Find("211.255.204.58") >= 0) m_strServer = "[BP28]";
	else if (strip.Find("211.255.204.59") >= 0) m_strServer = "[BP29]";
	else m_strServer.Format("[%s]", strip);
}

void CMainFrame::Popup7805()
{
	CString strFile;
	//7805 팝업 
	strFile.Format("%s\\tab\\NOTICECOOKIE.ini", Axis::home);

	char buff[1024];
	GetPrivateProfileString("7805", "VISIBLE", "Y", buff, sizeof(buff), strFile);

	CString str;
	str = buff;

	if (str == "Y")
	{
		GetPrivateProfileString("7805", "TODAY", "0", buff, sizeof(buff), strFile);

		str = buff;

		const int nFirst = GetPrivateProfileInt("7805", "FIRST", 1, strFile);

		const CTime tm(CTime::GetCurrentTime());
		CString today;
		today.Format("%04d%02d%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay());

		if (str != today)
		{
			WritePrivateProfileString("7805", "FIRST", "1", strFile);
		}

		if ((str == today && nFirst == 1) || str != today)
		{
			CTime time;
			time = CTime::GetCurrentTime();

			char readB[1024];
			int readL;

			readL = GetPrivateProfileString("7805", "FROM", "", readB, sizeof(readB), strFile);

			if (readL == 0)  //파일이 없거나 데이터를 못읽으면 그냥 띄운다
			{
				SetTimer(TM_POPUP_JISU, 1000, NULL);
				return;
			}

			CString strFrom(readB, readL);
			const CTime FromTime(atoi(strFrom.Mid(0, 4)), atoi(strFrom.Mid(4, 2)), atoi(strFrom.Mid(6, 2)), atoi(strFrom.Mid(8, 2)), atoi(strFrom.Mid(10, 2)), atoi(strFrom.Mid(12, 2)));

			readL = GetPrivateProfileString("7805", "TO", "", readB, sizeof(readB), strFile);

			if (readL == 0)  //파일이 없거나 데이터를 못읽으면 그냥 띄운다
			{
				SetTimer(TM_POPUP_JISU, 1000, NULL);
				return;;
			}

			CString strTo(readB, readL);
			const CTime ToTime(atoi(strTo.Mid(0, 4)), atoi(strTo.Mid(4, 2)), atoi(strTo.Mid(6, 2)), atoi(strTo.Mid(8, 2)), atoi(strTo.Mid(10, 2)), atoi(strTo.Mid(12, 2)));

			if (time <= FromTime || time >= ToTime)
			{
				SetTimer(TM_POPUP_JISU, 1000, NULL);
			}
		}
	}

}

void CMainFrame::CloudeCertUp()
{
	int ret = -1;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void*)&ret,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(caCLOUD, 1), 0);

	if (ret == 0)
	{
		if (m_bUseNewLogin)
			m_axConnect->SetGuide(_T("클라우드로 인증서 올리기 성공"));
		else
			m_axConnectOld->SetGuide(_T("클라우드로 인증서 올리기 성공"));
	}
}

void CMainFrame::CloudeCertDown()
{
	int ret = -1;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void*)&ret,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(caCLOUD, 2), 0);

	if (ret == 0)
	{
		if (m_bUseNewLogin)
			m_axConnect->SetGuide(_T("클라우드에서 인증서 내려받기 성공"));
		else
			m_axConnectOld->SetGuide(_T("클라우드로 인증서 내려받기 성공"));
	}
}

void CMainFrame::CloudeCertPassChange()
{
	int ret = -1;
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void*)&ret,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(caCLOUD, 3), 0);

	if (ret == 0)
	{
		if (m_bUseNewLogin)
			m_axConnect->SetGuide(_T("클라우드 인증서 간편비밀번호 변경 성공"));
		else
			m_axConnectOld->SetGuide(_T("클라우드 인증서 간편비밀번호 변경 성공"));
	}
}

void CMainFrame::CludeFuncCall(int igubn)
{
	int ret{};
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void*)&ret,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(caCLOUD, igubn), 0);
}

void CMainFrame::CludeUSE(bool bUseCloude)
{
	int ret{};
	int igubn = 0;
	m_bCloudeUse = bUseCloude;

	if (bUseCloude)
		igubn = 11;
	else
		igubn = 12;

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void*)&ret,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(caCLOUD, igubn), 0);
}

BOOL CMainFrame::isCDDScreen(CString strScreen)  //test CDD
{
	m_slog.Format("[axis][%s][CDD] isCDDScreen = [%s]", __FUNCTION__, strScreen);
	OutputDebugString(m_slog);

	if (m_arrCDDScreen.GetSize() <= 0)
		return FALSE;

	char	wb[512];
	CString file, axisfile, usnm = Axis::user, stmp;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm);

	const DWORD dwRc = GetPrivateProfileString("CDD/EDD", "popIB8224", "", wb, sizeof(wb), file);

	m_slog.Format("[axis][%s][CDD] popIB8224 = [%s]", __FUNCTION__, wb);
	OutputDebugString(m_slog);

	if (dwRc <= 0)
		return FALSE;

	stmp.Format("%s", wb);
	stmp.TrimRight();
	if (stmp == "0")
		return FALSE;

	for (int ii = 0; ii < m_arrCDDScreen.GetSize(); ii++)
	{
		if (m_arrCDDScreen.GetAt(ii).Find(strScreen) >= 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int CMainFrame::CheckCDDEDD()  //test CDD
{
	m_arrCDDScreen.RemoveAll();
	CString	Path, stmp;
	Path.Format("%s\\%s\\CDDEDD.INI", Axis::home, "tab");

	char readB[1024];
	int readL;
	readL = GetPrivateProfileString("CDD/EDD", "screen", "", readB, sizeof(readB), Path);
	if (readL > 0)
	{
		stmp.Format("%s", readB);
		while (1)
		{
			if (stmp.IsEmpty())
				break;
			m_arrCDDScreen.Add(Parser(stmp, ";"));
		}
	}

	if (m_arrCDDScreen.GetSize() <= 0 )
	{
		m_slog.Format("[axis][%s][CDD] m_arrCDDScreen size = 0", __FUNCTION__);
		OutputDebugString(m_slog);
		return 0;
	}

	CString str;
	long  rc;
	char data[20]{};

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void*)&rc,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(0x26, 3), data);

	m_sjumin.Format("%s", (char*)data);
	m_sjumin.TrimRight();
	m_slog.Format("[AXIS] [%s][CDD] m_sjumin = [%s]", __FUNCTION__, m_sjumin);
	OutputDebugString(m_slog);
	WriteLog(m_slog);
	if(!m_sjumin.IsEmpty())
		SendSAMFQ014();
	return 0;
}

void CMainFrame::KillMySelf()
{
	HANDLE         hProcessSnap = NULL;
	BOOL           bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };
	CString strProcess, strTarget;
	strTarget.Format("%s", "axis.exe");
	strTarget.TrimRight();

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == (HANDLE)-1)
		return;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	CString slog;
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{
		BOOL          bCurrent = FALSE;
		MODULEENTRY32 me32 = { 0 };

		do
		{
			//	bCurrent = GetProcessModule(pe32.th32ProcessID, strProcessName, pe32.szExeFile);
			slog.Format("[MAC] [%s] \r\n", pe32.szExeFile);
			OutputDebugString(slog);
			OutputDebugString("\r\n-----------------------------------------------\r\n");
			strProcess.Format("%s", pe32.szExeFile);
			if (strProcess.Find(strTarget) >= 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				if (hProcess && pe32.th32ProcessID == GetCurrentProcessId())
				{
					TerminateProcess(hProcess, 0);
					return;
				}
			}
		} while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}
	CloseHandle(hProcessSnap);
}

HANDLE CMainFrame::ProcessFind(char* strProcessName)
{
	HANDLE         hProcessSnap = NULL;
	BOOL           bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };
	CString strProcess, strTarget;
	strTarget.Format("%s", strProcessName);
	strTarget.TrimRight();

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == (HANDLE)-1)
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	CString slog;
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{
		BOOL          bCurrent = FALSE;
		MODULEENTRY32 me32 = { 0 };

		do
		{
			//	bCurrent = GetProcessModule(pe32.th32ProcessID, strProcessName, pe32.szExeFile);
			slog.Format("[MAC] [%s] \r\n", pe32.szExeFile);
			OutputDebugString(slog);
			OutputDebugString("\r\n-----------------------------------------------\r\n");
			strProcess.Format("%s", pe32.szExeFile);
			if (strProcess.Find(strTarget) >= 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				//if (hProcess)
				//	TerminateProcess(hProcess, 0);
				return hProcess;
			}
		} while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}
	CloseHandle(hProcessSnap);

	return nullptr;
}

void  CMainFrame::MemoDownoad()
{

}


typedef struct st_mid_memo
{
	char chgubn[1]{}; //구분: S-조회 . I-등록, U-수정, D-삭제
	char chusid[16]{}; //user ID
	char chcode[16]{}; //종목코드 
	char chmemo[15 * 1024]{}; //종목메모
}mid_memo;

void  CMainFrame::MemoUpload()
{
	CString str, rDir, dat;
	rDir.Format("%s\\%s\\%s\\memo.mmo", Axis::home, USER, Axis::user);

	char key[12]{}, lBytes[4]{};

	CFileFind	finder;
	if (!finder.FindFile(rDir))
		return;

	CFile rFile(rDir, CFile::modeRead);

	m_mapMemo.RemoveAll();

	UINT nBytesRead;
	do
	{
		FillMemory(key, sizeof(key), ' ');
		FillMemory(lBytes, sizeof(lBytes), ' ');

		nBytesRead = rFile.Read(&key, sizeof(key));
		if (nBytesRead == sizeof(key))
		{
			nBytesRead = rFile.Read(&lBytes, sizeof(lBytes));
			if (nBytesRead == sizeof(lBytes))
			{
				int lSize = _ttoi(CString(lBytes, 4));
				nBytesRead = rFile.Read(dat.GetBuffer(lSize), lSize);

				if ((int)nBytesRead != lSize)
					break;

				str.Format("%s", key);
				m_mapMemo.SetAt(str, dat);
				dat.ReleaseBuffer();
			}
			else
				break;
		}
		else
			break;

	} while ((int)nBytesRead);

	if (m_mapMemo.GetCount() > 0)
		UploadEachMemo();
}

BOOL CMainFrame::UploadEachMemo()
{
	mid_memo* pmid = new mid_memo;
	pmid->chgubn[0] = 'I';

	memcpy(pmid->chusid, Axis::userID, Axis::userID.GetLength());

	CString skey, sval;
	GetCodeForMemoUpload(skey, sval);
	memcpy(pmid->chcode, (char*)skey.GetBuffer(0), skey.GetLength());
	memcpy(pmid->chmemo, (char*)sval.GetBuffer(0), sval.GetLength());
	int bret = sendTR("pidomemo", (char*)pmid, sizeof(mid_memo), 0, 234);
	delete pmid;
	return bret;
}

void CMainFrame::parseMemoUpload(char* dat, int len)
{
	if (m_mapMemo.GetCount() <= 0)
	{
		CString str, rDir, bDir;
		rDir.Format("%s\\%s\\%s\\memo.mmo", Axis::home, USER, Axis::user);
		bDir.Format("%s\\%s\\%s\\memo.mmo_back", Axis::home, USER, Axis::user);	
		CopyFile(rDir, bDir, TRUE);
		::DeleteFile(rDir);
		return;
	}
	UploadEachMemo();
}

BOOL CMainFrame::GetCodeForMemoUpload(CString& skey, CString& sval)
{
	CString key, sdata;
	POSITION pos{};
	for (pos = m_mapMemo.GetStartPosition(); pos; )
	{
		m_mapMemo.GetNextAssoc(pos, key, sdata);
		skey = key;
		sval = sdata;
		m_mapMemo.RemoveKey(key);
		break;
	}
	return FALSE;
}

void CMainFrame::DumpUpload()
{
	CString	Path;
	Path.Format("%s\\%s\\AXIS.INI", Axis::home, "tab");

	char readB[1024];
	int readL;

	readL = GetPrivateProfileString("LOGUPLOAD", "ALL", "0", readB, sizeof(readB), Path);
	CString sALL(readB, readL);
	sALL.TrimLeft(); sALL.TrimRight();

	/*readL = GetPrivateProfileString("LOGUPLOAD", "DUMP", "0", readB, sizeof(readB), Path);
	CString sDump(readB, readL);
	sDump.TrimLeft(); sDump.TrimRight();

	readL = GetPrivateProfileString("LOGUPLOAD", "NOTICE", "0", readB, sizeof(readB), Path);
	CString sNOTICE(readB, readL);
	sNOTICE.TrimLeft(); sNOTICE.TrimRight();*/

	//m_slog.Format("DumpUpload  sALL =[%s]  sDump =[%s]  sNOTICE =[%s] ", sALL, sDump, sNOTICE);
	m_slog.Format("[AXIS] DumpUpload  sALL =[%s]", sALL);
	WriteLog(m_slog);

		if (!m_pUpload)
			m_pUpload = std::make_unique<class CUploadFile>(m_wizard.get());

		if (1)
		{
			m_bUploadComplet = FALSE;
			m_slog.Format("[AXIS] DumpUpload  sALL == 1   path = [% s]  ", Axis::home + "\\user\\" + Axis::user + "\\Crashlog");
			WriteLog(m_slog);

			if (m_pUpload->uploadfolder(Axis::home + "\\user\\" + Axis::user + "\\Crashlog"))
				m_bUploadComplet = TRUE;
		}
		else
			m_bUploadComplet = TRUE;

	/*if (!m_pUpload)
	{
		m_pUpload = std::make_unique<class CUploadFile>(m_wizard.get());

		if (sALL == "1")
		{
			m_bUploadComplet = FALSE;
			m_slog.Format("[AXIS] DumpUpload  sALL == 1   path = [% s]  ", Axis::home + "\\user\\" + Axis::user + "\\Crashlog");
			WriteLog(m_slog);

			if(m_pUpload->uploadfolder(Axis::home + "\\user\\" + Axis::user + "\\Crashlog"))
				m_bUploadComplet = TRUE;
		}
		else
			m_bUploadComplet = TRUE;*/


	/*	else
		{
			if (sDump == "1")
				m_pUpload->uploadfolder(Axis::home + "\\user\\" + Axis::user + "\\Crashlog");

			if (sNOTICE == "1")
				m_pUpload->uploadfolder(Axis::home + "\\user\\" + Axis::user + "\\log");
		}*/
	//}

}

void CMainFrame::SendPiboStaf()
{  //testAI
	char data[16];
	FillMemory(data, sizeof(data), ' ');
	sendTR("pibostaf", data, 16, US_KEY, 232);
}

//modi auto   CMainFrame::CreateSlideWnd()
void CMainFrame::CreateSlideWnd()
{
	m_pSlideWnd = std::make_unique<CSlideWnd>(this);
	if (!m_pSlideWnd->Create(IDD_DLG_SLIDEWND))
		m_pSlideWnd = nullptr;

	m_pSlideWnd->ShowWindow(SW_HIDE);
	CRect rect;
	m_pSlideWnd->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.OffsetRect((-1 * rect.left) - rect.Width(), 0);
	m_pSlideWnd->MoveWindow(rect);
	//m_pAutoOrderList->SetWindowPos(&wndTop, -rect.Width(), 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

//modi auto   CMainFrame::ShowAutoOrderList()
void CMainFrame::ShowSlideWnd(BOOL bShow)
{
	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);
	switch (pl.showCmd)
	{
	case SW_HIDE:case SW_MINIMIZE:case SW_SHOWMINIMIZED:
		m_pSlideWnd->ShowWindow(bShow);
		return;
	default:
		break;
	}


	int	x{}, y{};
	CRect	mRc;
	int	width = 0, height = 0;

	mRc = GetWndRect();

	x = mRc.left;	y = mRc.top;
	if (x < 0)	x = 0;
	if (y < 0)	y = 0;

	CRect rect;
	m_pSlideWnd->GetWindowRect(rect);

	rect.left = x;
	rect.top = y;
	rect.right = rect.left + mRc.Width() / 5;
	rect.bottom = rect.top + mRc.Height();

	if (!m_pSlideWnd->IsWindowVisible())
		m_pSlideWnd->SlideOpen(rect);
	else
		m_pSlideWnd->SlideClose(rect);

	m_pSlideWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	m_slog.Format("%d %d %d %d", rect.left, rect.top, rect.right, rect.bottom);
	OutputDebugString(m_slog);
}

CString CMainFrame::GetMapNumByKey(int nkey)
{
	CString sMapnum{};
	CChildFrame* actChild{};
	CSChild* actSchild{};

	if (m_arMDI[m_vsN].Lookup(m_activeKey, actChild))
		sMapnum = actChild->m_mapN;
	else if (m_arSDI[m_vsN].Lookup(m_activeKey, actSchild))
		sMapnum = actSchild->m_mapN;

	return sMapnum;
}
//modi shared
void CMainFrame::initShared()
{
	char	buf[512];
	CString	file, stmp, smap;
	file.Format("%s\\tab\\axshared.ini", Axis::home);
	DWORD dw = GetPrivateProfileString("Mode", "noshared", "0", buf, sizeof(buf), file);
	stmp.Format("%s", buf); stmp.TrimRight();

	m_slog.Format("[AXIS] initShared noshared =[%s]", stmp);
	WriteLog(m_slog);

	if (stmp == "1")
		return;

	CString path;
	path.Format("%s\\%s\\%s", Axis::home, DEVDIR, "cx_shared.dll");
	m_hSharedLib = LoadLibrary(path);

	if (!m_hSharedLib)
	{
		WriteLog("[AXIS] cx_shared.dll LoadLibrary fail");
		return;
	}

	CWnd* (APIENTRY * axCreate)(CWnd*, void*);
	axCreate = (CWnd * (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hSharedLib, "axCreate");

	if (axCreate == nullptr)
	{
		WriteLog("[AXIS] cx_shared.dll axcreate fail");
		return;
	}

	DWORD processID = GetCurrentProcessId();
	m_sHSharedkey.Format("%s%s%d", Axis::userID, "_HsharedCtrl_", processID);   //ex  ##ibkmts_HsharedCtrl_PROCESSID
	m_sMSharedkey.Format("%s%s%d", Axis::userID, "_MsharedCtrl_", processID); //ex  ##ibkmts_MsharedCtrl_PROCESSID

	CString strkey;
	strkey = m_sHSharedkey + "|" + m_sMSharedkey;

	WriteLog(strkey);

	std::unique_ptr <char[]> pchar;
	pchar = std::make_unique<char[]>(strkey.GetLength() + 1);
	memcpy(pchar.get(), (LPSTR)(LPCTSTR)strkey, strkey.GetLength());
	m_pSharedMemory = axCreate(this, (LPVOID)pchar.get());
}

BOOL CMainFrame::IsASTxRunning(BOOL bLog)
{
	CString file;
	file.Format("%s\\exe\\CHECKAOS.TXT", Axis::home);
	bool bExistFile = IsFileExist(file);

	if(m_bNoProtect || bExistFile)  //CHECKAOS 파일이 있거나 내부망이면 ASTx 가동 안함
	{
		m_slog.Format("[AXIS] !!!!!!!!!  [ASTx] IsASTxRunning pass not workingm_bNoProtect=[%d] bExistFile=[%d] ip=[%s]", m_bNoProtect, bExistFile, m_ipAddr);
		OutputDebugString(m_slog);
		if(bLog)
			WriteLog(m_slog);
		return TRUE;
	}

	DWORD ret = STSDKEX_IsRunningFuncA("ibkstock", STSDKEX_FUNC_CODE_PB);
	if (ret == STSDKEX_ERROR_SUCCESS)
	{
		m_slog.Format("[AXIS] !!!!!!!!!!! [ASTx] IsASTxRunning ASTx on[%d] m_bNoProtect=[%d] bExistFile=[%d] ", ret, m_bNoProtect,bExistFile);
		OutputDebugString(m_slog);
		if (bLog)
			WriteLog(m_slog);
		return TRUE;
	}
	else
	{
		m_slog.Format("[AXIS] !!!!!!!!!!! IsASTxRunning ASTx not running[%lx]", ret);
		OutputDebugString(m_slog);
		WriteLog(m_slog);
		return FALSE;
	}
}

void CMainFrame::CheckEdgeInstalled()
{
	CString stmp, sProgramName, strEdgeEnv;
	sProgramName = "Microsoft Edge WebView2 런타임";
	BOOL bInstalled{};
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		char szSubKey[MAX_PATH];
		DWORD dwIndex = 0;
		DWORD dwSize = MAX_PATH;

		//해당레지스트리의 하위키를 열거 한다. 
		while (RegEnumKeyEx(hKey, dwIndex, szSubKey, &dwSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
		{
			HKEY hSubKey;
			if (RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
			{
				stmp.Format("\r\n[Edge] szSubKey=[%s], dwindex=[%d]", szSubKey, dwIndex);
				OutputDebugString(stmp);
				char szDisplayName[MAX_PATH];
				DWORD dwSize = sizeof(szDisplayName);

				//패키지명 버전 확인
				if (RegQueryValueEx(hSubKey, _T("Displayname"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayName), &dwSize) == ERROR_SUCCESS)
				{
					CString strDisplayName(szDisplayName);
					if (strDisplayName.Find(sProgramName) >= 0)
					{
						bInstalled = TRUE;
						char szDisplayVersion[MAX_PATH];
						dwSize = sizeof(szDisplayVersion);
						if (RegQueryValueEx(hSubKey, _T("DisplayVersion"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayVersion), &dwSize) == ERROR_SUCCESS)
						{
							CString strDisplayVersion(szDisplayVersion);
							strEdgeEnv += "DisplayVersion = ";
							strEdgeEnv += strDisplayVersion;
							strEdgeEnv += "|";
						}

						char szInstallDate[MAX_PATH];
						dwSize = sizeof(szInstallDate);
						if (RegQueryValueEx(hSubKey, _T("InstallDate"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szInstallDate), &dwSize) == ERROR_SUCCESS)
						{
							CString szInstallDate(szInstallDate);
							strEdgeEnv += "InstallDate = ";
							strEdgeEnv += szInstallDate;
							strEdgeEnv += "|";
						}

						stmp.Format("\r\n  !!!!!!!!!!!!!!!!!!!EDGE Installed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  \r\n[Edge] strEdgeEnv=[%s], dwindex=[%d]", strEdgeEnv);
						OutputDebugString(stmp);

						break;
					}
				}

			}
			dwSize = MAX_PATH;
			dwIndex++;
		}
	}


	char	bufA[128]{};
	char	bufB[128]{};
	CString	file;
	file.Format("%s\\%s\\%s\\userconf.ini", Axis::home, USRDIR, Axis::user);
	if (!bInstalled)  //edge 미설치시
		WritePrivateProfileString("PDF_CONFIRM", "pop", "browser", file);   //매매
	else
		WritePrivateProfileString("PDF_CONFIRM", "pop", "map", file);   //매매	
}

void CMainFrame::Sendpibojggb(CString sMarket)
{
	struct PIBOjggb_mid 
	{
		char gubn[1];
		char intype[2];
	};

	char data[100];
	FillMemory(data, sizeof(data), ' ');

	PIBOjggb_mid* mid = (PIBOjggb_mid*)&data[0];
	memcpy(mid->gubn, sMarket, 1);

	if(sMarket == "N")
		sendTR("pibojggb", data, sizeof(PIBOjggb_mid), US_ENC, 228);
	else
		sendTR("pibojggb", data, sizeof(PIBOjggb_mid), US_ENC, 227);
}

void CMainFrame::WriteMainInfo()
{
	CString sPath;
	CString strFilePath;
	CFile	file;
	CString sBuf;
	TCHAR	chFileName[128]{};

	GetModuleFileName(NULL, chFileName, MAX_PATH);

	strFilePath.Format(_T("%s"), chFileName);

	strFilePath = strFilePath.Left(strFilePath.ReverseFind('\\'));
	strFilePath.Replace("\\exe", "");

	CString sMainAdd, sfile;
	sMainAdd.Format("%d", (int)this->m_hWnd);

	CString sHandle{};
	DWORD pid = ::GetCurrentProcessId();
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	sHandle.Format("%d", (int)hProcess);
	sfile.Format("%s\\%s\\AXISENC.ini", strFilePath, "tab");
	WritePrivateProfileString("ADDRESS", "main", sMainAdd, sfile);
	WritePrivateProfileString("ADDRESS", "PROCESS_HANDLE", sHandle, sfile);
}

void CMainFrame::SetVirtualSDIVisible(int vsN, bool bshow)
{
	CSChild* schild{};
	POSITION pos;
	int key{};

	UINT			nFlags{};

	if (bshow) nFlags = SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER;
	else	nFlags = SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER;

	for (int ii = 0; ii < 6; ii++)
	{
		for (pos = m_arSDI[ii].GetStartPosition(); pos; )
		{
			m_arSDI[ii].GetNextAssoc(pos, key, schild);

			if (vsN != ii)
				schild->SetWindowPos(&wndTop, 0, 0, 0, 0, nFlags);
		}
	}
}

void CMainFrame::ReadManageMapInfo()
{
	if (m_mapManage.GetCount() > 0)
		return;

	CString sfile;
	sfile.Format("%s\\%s\\AXSCREENMANAGE.INI", Axis::home, "tab");
	CStdioFile file;
	CString line;

	if (!file.Open(sfile, CFile::modeRead | CFile::typeText))
		return;

	while (file.ReadString(line))
	{
		line.TrimRight(); // 앞뒤 공백 제거

		// 빈 줄, 주석은 건너뜀
		if (line.IsEmpty() || line[0] == _T('#') || line[0] == _T(';'))
			continue;

		int pos = line.Find(_T('='));
		if (pos > 0)
		{
			CString key = line.Left(pos);
			CString value = line.Mid(pos + 1);

			key.TrimRight();
			value.TrimRight();

			if (!key.IsEmpty())
				m_mapManage.SetAt(key, value);
		}
	}
	file.Close();
}

#ifdef DF_MK_CAPTION
void CMainFrame::ReadMarketFile()
{
	CString sfile;
	sfile.Format("%s\\%s\\AXISMARKET.INI", Axis::home, "tab");
	CStdioFile file;
	CString line;

	if (!file.Open(sfile, CFile::modeRead | CFile::typeText))
		return;

	while (file.ReadString(line))
	{
		line.TrimRight(); // 앞뒤 공백 제거

		// 빈 줄, 주석은 건너뜀
		if (line.IsEmpty() || line[0] == _T('#') || line[0] == _T(';'))
			continue;

		int pos = line.Find(_T('='));
		if (pos > 0)
		{
			CString key = line.Left(pos);
			CString value = line.Mid(pos + 1);

			key.TrimRight();
			value.TrimRight();

			if (!key.IsEmpty())
				m_mapPermissions.SetAt(key, value);
		}
	}
	file.Close();
}

int CMainFrame::GetMarketType(const CString& screenNo)
{
	CString valStr;
	if (!m_mapPermissions.Lookup(screenNo, valStr))
		return 0; // 화면번호가 없으면 권한 없음

	int perm = _ttoi(valStr); // 문자열을 정수로 변환
	return perm;
}

bool CMainFrame::GetScreenInfoByMapKey(
	int mapkey,
	_stMap& outMap
) const
{
	CString mapname;
	CChildFrame* actChild{};
	CSChild* actSchild{};
	int nActGroup{};
	if (m_arMDI[m_vsN].Lookup(m_activeKey, actChild))
	{
		nActGroup = actChild->m_xcaption.GetGroup();
	}
	else if (m_arSDI[m_vsN].Lookup(m_activeKey, actSchild))
	{
		nActGroup = actSchild->m_xcaption.GetGroup();
	}

	CChildFrame* child{};
	CSChild* schild{};

	if (!m_arMDI[m_vsN].Lookup(m_activeKey, child))
	{
		CSChild* schild;
		CRect wRc;
		if (m_arSDI[m_vsN].Lookup(m_activeKey, schild))
		{
			mapname = schild->m_mapN;
		}
	}
	else
	{
		if (child->m_bIconic)
		{
			//child->actionCaption(IDX_RESTORE, CPoint(0, 0));
		}
		else
		{
			mapname = child->m_mapN;
		}
	}

	
	auto it = m_MapScreenInfo.find(mapname);
	if (it == m_MapScreenInfo.end())
		return false;

	outMap = it->second;
	return true;
}

bool CMainFrame::LoadScreenInfo(const CString& filePath)
{
	CString sfile;
	sfile.Format("%s\\%s\\%s", Axis::home, "tab", filePath);

	CStdioFile file;
	if (!file.Open(sfile, CFile::modeRead | CFile::typeText))
		return false;

	m_MapScreenInfo.clear();

	CString line;
	while (file.ReadString(line))
	{
		line.TrimRight();
		if (line.IsEmpty() || line[0] == _T('#'))
			continue;

		int eqPos = line.Find(_T('='));
		if (eqPos < 0)
			continue;

		CString key = line.Left(eqPos);
		CString value = line.Mid(eqPos + 1);

		key.TrimRight();;
		value.TrimRight();;

		std::vector<CString> tokens;
		int pos = 0;
		CString token = value.Tokenize(_T("|"), pos);
		while (!token.IsEmpty())
		{
			tokens.push_back(token);
			token = value.Tokenize(_T("|"), pos);
		}

		if (tokens.size() < 3)
			continue;

		_stMap info;
		info.screenId = tokens[0];
		info.screenName = tokens[1];
		info.authId = tokens[2];

		// 나머지는 extra로
		for (size_t i = 3; i < tokens.size(); ++i)
			info.extra.push_back(tokens[i]);

		m_MapScreenInfo.emplace(key, info);
	}

	file.Close();

	_stMap info;
	//if (GetScreenInfoByName(_T("1002"), info))
	{
		// 성공
		// info.screenId
		// info.screenName
		// info.authId
		//AfxMessageBox(info.authId);
	}
	//else
	{
		// 실패
	}
	return true;
}


#ifdef DF_MAIN_RTS
//MMSG_GETRTS_FROM_MAIN   화면에서 실시간 데이터 요청해서 읽을때!!
int CMainFrame::GetSlotIndex_FindOnly(const char* code)
{
	std::shared_lock<std::shared_mutex > lock(g_codeMapLock);  //GetSlotIndex_FindOnly

	auto it = g_codeToIndex.find(code);
	if (it == g_codeToIndex.end())
	{
		char pcode[7]{};   // 반드시 7

		if (code)
		{
			size_t len = strlen(code);

			if (len >= 6)
				memcpy(pcode, code + (len - 6), 6);
			else
				strncpy(pcode, code, 6);

			pcode[6] = '\0';
		}

		it = g_codeToIndex.find(pcode);  // 재할당 (새 선언 X)

		if (it == g_codeToIndex.end())
			return -1;
	}
	return it->second;
}

int CMainFrame::EnsureSlotIndexForCode(const char* code)
{
	auto it = g_codeToIndex.find(code);

	if (it != g_codeToIndex.end())
		return it->second;

	int idx = g_nextSlot++;
	g_codeToIndex[code] = idx;

	CopyZ(g_tickSlots[idx].code, CODE_LEN, code);

	return idx;
	//if (!code || !code[0])
	//	return -1;

	//// 읽기로 먼저 확인
	//{
	//	std::shared_lock<std::shared_mutex> readLock(g_codeMapLock);
	//	auto it = g_codeToIndex.find(code);
	//	if (it != g_codeToIndex.end())
	//		return it->second;
	//}

	//// 없으면 쓰기락으로 새로 할당
	//std::unique_lock<std::shared_mutex> writeLock(g_codeMapLock);

	//// double-check (락 잡는 사이 다른 스레드가 먼저 넣었을 수 있음)
	//auto it = g_codeToIndex.find(code);
	//if (it != g_codeToIndex.end())
	//	return it->second;

	//if (g_nextIndex >= MAX_SLOT)
	//	return -1;

	//int idx = g_nextIndex++;
	//g_codeToIndex.emplace(code, idx);
	//CopyZ(g_tickSlots[idx].code, sizeof(g_tickSlots[idx].code), code);

	//CString slog;
	//slog.Format("[EnsureSlot] 신규등록 code=[%s] idx=[%d] 총갯수=[%d]\n",
	//	code, idx, (int)g_codeToIndex.size());
	//OutputDebugString(slog);

	//return idx;
}

AXIS_API int Axis_State()
{
	if (!g_pMainFrame) return FALSE;
	return g_pMainFrame->m_iAxisState;
}

AXIS_API int Axis_EnsureSlotIndex(const char* code)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!pFrame)
		return -1;

	return pFrame->EnsureSlotIndexForCode(code);
}

AXIS_API const TickSnapshot* Axis_GetTickSlots()
{
	return g_tickSlots;
}

//AXIS_API void Axis_PushDirtySlot(int idx)
//{
//	std::lock_guard<std::mutex> lk(g_dirtyMtx);
//	g_dirtySlots.insert(idx);  // 중복 자동 제거
//}
//
//AXIS_API int Axis_SwapDirtySlots(int* outBuf, int bufSize)
//{
//	std::unordered_set<int> dirty;
//	{
//		std::lock_guard<std::mutex> lk(g_dirtyMtx);
//		dirty.swap(g_dirtySlots);
//	}
//	int count = min((int)dirty.size(), bufSize);
//	int i = 0;
//	for (int idx : dirty)
//		outBuf[i++] = idx;
//	return count;
//}

AXIS_API  BOOL Axis_IsMainRTS()
{
	if (!g_pMainFrame) return FALSE;
	return g_pMainFrame->m_bMainRTS;
}

AXIS_API  CWnd* Axis_GetMainWnd()
{
	if (!g_pMainFrame) return FALSE;
	return g_pMainFrame;
}
#endif


#endif

void CMainFrame::TestRTSData()
{
	static int s_seq = 0;
	s_seq++;

	// 1. lock 잡고 코드목록만 복사
	std::vector<std::string> codes;
	{
		//std::lock_guard<std::mutex> lock(g_codeMapLock); 
		std::shared_lock<std::shared_mutex> lock(g_codeMapLock);   //TestRTSData
		for (const auto& pair : g_codeToIndex)
			codes.push_back(pair.first);
	}
	// 2. lock 해제 후 테스트 데이터 생성 및 update_ticker 호출
	for (const auto& scode : codes)
	{
		const char* code = scode.c_str();

		char s_gubn[4] = "1";
		char s_curr[16] = {};
		char s_diff[16] = {};
		char s_rate[16] = {};
		char s_volume[16] = {};

		int price = 10000 + (rand() % 90000);
		int diff = (rand() % 2000) - 1000;
		float rate = (float)diff / price * 100.0f;
		int volume = rand() % 9000000 + 100000;

		sprintf_s(s_curr, "%d", price);
		sprintf_s(s_diff, "%d", diff);
		sprintf_s(s_rate, "%.2f", rate);
		sprintf_s(s_volume, "%d", volume);

		DWORD data[MAX_RTS_INDEX]{};
		data[0] = (DWORD)s_gubn;
		data[23] = (DWORD)s_curr;
		data[24] = (DWORD)s_diff;
		data[33] = (DWORD)s_rate;
		data[27] = (DWORD)s_volume;

		_alertR alertR{};
		alertR.code = code;
		alertR.stat = 1;
		alertR.size = 1;
		alertR.ptr[0] = (DWORD)data;

		update_ticker(0, &alertR);
	}
}

void CMainFrame::DumpAllSlots()
{
	DWORD now = GetTickCount();
	DWORD elapsed = (m_tickCountStart > 0) ? (now - m_tickCountStart) : 1;
	float elapsedSec = elapsed / 1000.0f;
	if (elapsedSec < 0.001f) elapsedSec = 0.001f;

	std::vector<std::pair<std::string, int>> sorted(m_tickCount.begin(), m_tickCount.end());
	std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
		return a.second > b.second;
		});

	// 전체 합계 먼저 계산 (리셋 전에)
	int totalCnt = 0;
	for (const auto& p : m_tickCount) totalCnt += p.second;
	float totalPerSec = totalCnt / elapsedSec;

	CString slog;
	slog.Format("[TopTickers] 경과=[%.1f초] 총종목=[%d] 전체수신=[%d회] 전체초당=[%.1f회/초] =====\n",
		elapsedSec, (int)sorted.size(), totalCnt, totalPerSec);
	OutputDebugString(slog);

	int rank = 1;
	for (const auto& item : sorted)
	{
		float perSec = item.second / elapsedSec;

		const char* curr = "-";
		const char* volume = "-";
		const char* rate = "-";
		{
			std::shared_lock<std::shared_mutex> readLock(g_codeMapLock);
			auto it = g_codeToIndex.find(item.first);
			if (it != g_codeToIndex.end())
			{
				const TickSnapshot& slot = g_tickSlots[it->second];
				int seq = slot.seq.load(std::memory_order_acquire);
				if (!(seq & 1))
				{
					if (slot.valid[23]) curr = slot.values[23];
					if (slot.valid[27]) volume = slot.values[27];
					if (slot.valid[33]) rate = slot.values[33];
				}
			}
		}

		slog.Format("[TopTickers][%3d위] code=[%-12s] 수신=[%3d회/%.1f회/초] 현재가=[%8s] 거래량=[%10s] 등락률=[%s]\n",
			rank++, item.first.c_str(), item.second, perSec,  // perSec = item.second / elapsedSec
			curr, volume, rate);
		OutputDebugString(slog);

		//if (rank > 30) break;
	}

	slog.Format("[TopTickers] ====END====\n");
	OutputDebugString(slog);

	// 마지막에 리셋
	m_tickCount.clear();
	m_tickCountStart = GetTickCount();
}

void CMainFrame::StartWorkerThread()
{
	if (m_bMainRTS)
	{
		m_workerRunning = true;
		m_workerThread = std::thread(&CMainFrame::WorkerThreadFunc, this);
	}
}

// 워커 스레드 종료 - OnDestroy 시점에 호출
void CMainFrame::StopWorkerThread()
{
	if (m_bMainRTS)
	{
		m_workerRunning = false;
		if (m_workerThread.joinable())
			m_workerThread.join();
	}
}

// 워커 스레드 함수
void CMainFrame::WorkerThreadFunc()
{
	//while (m_workerRunning)
	//{
	//	SafeAlertItem* item = nullptr;
	//	{
	//		std::lock_guard<std::mutex> lock(m_alertMutex);
	//		if (!m_alertQueue.empty())
	//		{
	//			item = m_alertQueue.front();
	//			m_alertQueue.pop();

	//			// 같은 종목이 큐에 또 있으면 최신값으로 교체하고 이전꺼 버림
	//			while (!m_alertQueue.empty() &&
	//				strcmp(m_alertQueue.front()->code, item->code) == 0)
	//			{
	//				delete item;
	//				item = m_alertQueue.front();
	//				m_alertQueue.pop();
	//			}
	//		}
	//	}

	//	int idx = EnsureSlotIndexForCode(item->code);
	//	if (idx >= 0)
	//	{
	//		UpdateSnapshotFromAlert(g_tickSlots[idx], item);
	//		Axis_PushDirtySlot(idx);  // ← API로
	//	}
	//	delete item;
	//}
}

NetType CMainFrame::GetCurrentNetType(BOOL bUpload)
{
	DWORD bestIfIndex = 0;
	GetBestInterface(inet_addr("8.8.8.8"), &bestIfIndex);   // 실제 기본 경로 인터페이스

	ULONG size = 0;
	GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_GATEWAYS, NULL, NULL, &size);
	if (size == 0) return NET_NONE;

	IP_ADAPTER_ADDRESSES* addresses = (IP_ADAPTER_ADDRESSES*)malloc(size);
	if (!addresses) return NET_NONE;

	NetType result = NET_NONE;

	if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_GATEWAYS, NULL, addresses, &size) == NO_ERROR)
	{
		for (IP_ADAPTER_ADDRESSES* addr = addresses; addr; addr = addr->Next)
		{
			if (addr->OperStatus != IfOperStatusUp) continue;
			if (addr->FirstGatewayAddress == NULL) continue;
			if (addr->IfType == IF_TYPE_SOFTWARE_LOOPBACK) continue;
			if (addr->IfType == IF_TYPE_TUNNEL) continue;

			// 실제 기본 경로로 쓰이는 어댑터만 인정 (메트릭/우선순위 반영됨)
			if (bestIfIndex != 0 && addr->IfIndex != bestIfIndex) continue;

			result = (addr->IfType == IF_TYPE_IEEE80211) ? NET_WIFI : NET_WIRED;
			break;
		}
	}

	free(addresses);
	return result;
}
   
void CMainFrame::ForwardTickerToAgent(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 0) return;

	m_sync.Lock();  // 기존 코드와 동일 패턴

	const WORD wFlag = HIWORD(wParam);
	const WORD wBodyLen = LOWORD(wParam);
	const int  nTotal = L_cds + wBodyLen;

	// 안전 체크
	static char bufx[1024 * 128];  // 128KB - 기존과 동일
	if (nTotal > (int)sizeof(bufx)) {
		m_sync.Unlock();
		return;
	}

	ZeroMemory(bufx, nTotal);  // 필요한 만큼만 (성능)

	struct _exeCDSS* cds = (struct _exeCDSS*)bufx;
	cds->flag = wFlag;
	cds->len = wBodyLen;
	CopyMemory(&bufx[L_cds], (char*)lParam, wBodyLen);  // 본문 길이만 복사

	COPYDATASTRUCT cs;
	cs.cbData = nTotal;
	cs.dwData = 0;            // 기존 컨벤션 유지 (또는 IPC_REALTIME_TICK)
	cs.lpData = bufx;

	// 에이전트로 송신
	if (m_hAgentWnd && ::IsWindow(m_hAgentWnd))
	{
		::SendMessageTimeout(
			m_hAgentWnd, WM_COPYDATA,
			(WPARAM)GetSafeHwnd(),
			(LPARAM)&cs,
			SMTO_ABORTIFHUNG, 200, NULL);
	}

	m_sync.Unlock();
}

void CMainFrame::CreateAgentProcess(bool bforce)
{
	if (!m_bAxisAgent && !bforce)
		return;
	if (bforce)
		m_bShowAxisAgent = true;
	// 현재 실행파일 경로 기준으로 AxisAgent.exe 경로 생성
	char selfPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, selfPath, MAX_PATH);

	char folder[MAX_PATH] = { 0 };
	strcpy_s(folder, selfPath);
	char* lastSlash = strrchr(folder, '\\');
	if (lastSlash) *lastSlash = '\0';

	// AxisAgent.exe 전체 경로
	char aps[MAX_PATH] = { 0 };
	sprintf_s(aps, "%s\\AxisAgent.exe", folder);

	// Named Event 생성 (전역으로 보관)
	DWORD pid = GetCurrentProcessId();
	char eventName[64] = { 0 };
	sprintf_s(eventName, "AgentEnd_%lu", pid);
	m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, eventName); // ← 멤버변수

	// 커맨드라인
	CRect rc;
	GetWindowRect(&rc);

	CString userpath;
	userpath.Format("%s\\%s\\%s\\Crashlog", Axis::home, USRDIR, Axis::user);

	char cmds[1024] = { 0 };
	sprintf_s(cmds, "AxisAgent.exe /p %lu /h %llu /n %s /v %d /x %d /y %d /t %lu /d \"%s\"",
		pid,
		(UINT64)this->GetSafeHwnd(),
		(LPCSTR)m_regkey,
		m_bShowAxisAgent,
		rc.left,   // ← 좌측
		rc.top,
		GetCurrentThreadId(),
		(LPCSTR)userpath);      // 현재 스레드 ID);

	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi = { 0 };

	BOOL bRc = CreateProcess(
		aps,
		cmds,
		NULL,
		NULL,
		FALSE,  // 파이프 없으니 상속 불필요
		0,
		NULL,
		NULL,
		&si,
		&pi
	);

	if (!bRc)
	{
	//	DebugLog("CreatePingProcess: CreateProcess 실패 err=%d\n", GetLastError());
		return;
	}

	// 프로세스 핸들 보관 (나중에 종료 확인용)
	m_hAgentProcess = pi.hProcess; // ← 멤버변수
	CloseHandle(pi.hThread);

//	DebugLog("CreatePingProcess: AxisAgent 시작 PID=%lu\n", pi.dwProcessId);
}

void CMainFrame::CloseAgent()
{
	// 정상 종료
	if (m_hStopEvent)
	{
		SetEvent(m_hStopEvent);
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}

	// 강제 종료 (혹시 살아있으면)
	char caption[256] = { 0 };
	sprintf_s(caption, "AxisAgent_%s", (LPCSTR)m_regkey);
	const CWnd* wnd = FindWindow(nullptr, caption);
	if (wnd && wnd->GetSafeHwnd())
		::PostMessage(wnd->m_hWnd, WM_CLOSE, 0, 0);

	if (m_hAgentProcess)
	{
		CloseHandle(m_hAgentProcess);
		m_hAgentProcess = NULL;
	}
}

CString CMainFrame::DpapiEncrypt(const CString& plainText)
{
	DATA_BLOB dataIn = { (DWORD)plainText.GetLength(), (BYTE*)(LPCSTR)plainText };
	DATA_BLOB dataOut = { 0, nullptr };

	if (!CryptProtectData(&dataIn, L"AccountInfo", nullptr, nullptr, nullptr, 0, &dataOut))
		throw std::runtime_error("DPAPI encrypt failed");

	DWORD b64Len = 0;
	CryptBinaryToStringA(dataOut.pbData, dataOut.cbData,
		CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, nullptr, &b64Len);

	CString b64;
	CryptBinaryToStringA(dataOut.pbData, dataOut.cbData,
		CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
		b64.GetBuffer(b64Len), &b64Len);
	b64.ReleaseBuffer();

	LocalFree(dataOut.pbData);
	return b64;  // ENC: 없이 그냥 반환
}

// ── DPAPI 복호화 ─────────────────────────────────────────────────
CString CMainFrame::DpapiDecrypt(const CString& encText)
{
	// ENC: 분기 없이 바로 복호화
	DWORD blobLen = 0;
	CryptStringToBinaryA(encText, encText.GetLength(), CRYPT_STRING_BASE64,
		nullptr, &blobLen, nullptr, nullptr);

	std::vector<BYTE> blob(blobLen);
	CryptStringToBinaryA(encText, encText.GetLength(), CRYPT_STRING_BASE64,
		blob.data(), &blobLen, nullptr, nullptr);

	DATA_BLOB dataIn = { blobLen, blob.data() };
	DATA_BLOB dataOut = { 0, nullptr };

	if (!CryptUnprotectData(&dataIn, nullptr, nullptr, nullptr, nullptr, 0, &dataOut))
		throw std::runtime_error("DPAPI decrypt failed");

	CString result((LPCSTR)dataOut.pbData, dataOut.cbData);
	SecureZeroMemory(dataOut.pbData, dataOut.cbData);
	LocalFree(dataOut.pbData);
	return result;
}

std::map<CString, CString> CMainFrame::LoadAccountHistory(const CString& iniPath)
{
	// STATUS 확인
	CString sval{};
	char status[8] = {};
	GetPrivateProfileStringA("ENCRYPT", "STATUS", "0",
		status, _countof(status), iniPath);
	bool isEncrypted = (strcmp(status, "1") == 0);

	std::map<CString, CString> result;
	std::ifstream fin(iniPath);
	if (!fin.is_open()) return result;

	std::string line;
	bool inSection = false;

	while (std::getline(fin, line))
	{
		if (!line.empty() && line.front() == '[') {
			inSection = (line == "[AccountHistory]");
			continue;
		}
		if (!inSection) continue;

		auto eq = line.find('=');
		if (eq == std::string::npos) continue;

		CString key(line.substr(0, eq).c_str());
		CString val(line.substr(eq + 1).c_str());

		// VERSION 키는 복호화 제외
		if (key == "VERSION") {
			result[key] = val;
			continue;
		}

		// 빈 값도 복호화 시도 안 함
		if (val.IsEmpty()) {
			result[key] = val;
			continue;
		}
	
		sval =  DpapiDecrypt(val);
		result[key] = isEncrypted ? sval : val;

		m_slog.Format("[ENC][DEC] [%d] [%s]", sval.GetLength(), sval);
		OutputDebugString(m_slog);

		sval.Empty();

	
	}

	return result;
}

void CMainFrame::EncryptIniFile(const CString& iniPath, const std::vector<BYTE>& key)
{
	// Backup
	CString backupPath = iniPath + ".bak";
	DeleteFileA(backupPath);

	m_slog.Format("[AXIS][ENC][%s]<%d> iniPath=[%s]", __FUNCTION__, __LINE__, iniPath);
	//OutputDebugString(m_slog);

	if (!CopyFileA(iniPath, backupPath, FALSE))
		throw std::runtime_error("Backup failed");

	std::ifstream fin(iniPath, std::ios::binary);
	if (!fin.is_open())
		throw std::runtime_error("Cannot open ini file");

	std::ostringstream out;
	std::string line;
	bool inAccountHistory = false;

	while (std::getline(fin, line))
	{
		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		if (!line.empty() && line.front() == '[')
		{
			inAccountHistory = (line == "[AccountHistory]");
			out << line << '\n';
			continue;
		}

		if (inAccountHistory && !line.empty() && line.front() != ';')
		{
			auto eq = line.find('=');
			if (eq != std::string::npos)
			{
				std::string key_str = line.substr(0, eq);
				std::string val = line.substr(eq + 1);

				// '|' present = plain text -> encrypt
				// '|' absent  = already encrypted
				if (!val.empty())
				{
					// split by \t, encrypt each entry that contains '|'
					std::vector<std::string> parts;
					std::istringstream ss(val);
					std::string token;
					while (std::getline(ss, token, '\t'))
						parts.push_back(token);

					for (auto& part : parts)
					{
						if (!part.empty() && part.find('|') != std::string::npos)
						{
							CString encVal = AesEncrypt(CString(part.c_str()), key);
							part = (LPCSTR)encVal;

							if (encVal.GetLength() < 32)
							{
								m_slog.Format("[AXIS][ENC][%s]<%d>  lower than 32 = [%s] [%s]", __FUNCTION__, __LINE__, encVal, iniPath);
								OutputDebugString(m_slog);
							}
						}
					}

					val.clear();
					for (size_t i = 0; i < parts.size(); i++)
					{
						if (i > 0) val += '\t';
						val += parts[i];
					}
				}

				out << key_str << '=' << val << '\n';
				continue;
			}
		}

		out << line << '\n';
	}
	fin.close();

	CString tmpPath = iniPath + ".tmp";
	{
		std::ofstream fout(tmpPath, std::ios::binary);
		if (!fout.is_open())
			throw std::runtime_error("Cannot write tmp file");
		fout << out.str();
	}

	if (!MoveFileExA(tmpPath, iniPath,
		MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		DeleteFileA(tmpPath);
		throw std::runtime_error("File replace failed");
	}

	m_slog.Format("[AXIS][ENC][%s]<%d> [%s] deleted=[%d]", __FUNCTION__, __LINE__, backupPath, DeleteFileA(backupPath));
	//OutputDebugString(m_slog);
}

std::pair<int, int> CMainFrame::EncryptAllUserIni(const CString& rootPath)
	{
		int successCount = 0;
		int failCount = 0;

		CString searchPath = rootPath + "\\*";
		WIN32_FIND_DATAA fd = {};
		HANDLE hFind = FindFirstFileA(searchPath, &fd);
		if (hFind == INVALID_HANDLE_VALUE)
			throw std::runtime_error("Cannot open root folder");

		std::vector<BYTE> key = DeriveKeyFromRegkey(m_regkey);

		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
			if (strcmp(fd.cFileName, ".") == 0)  continue;
			if (strcmp(fd.cFileName, "..") == 0) continue;

			CString userFolder;
			userFolder.Format("%s\\%s", (LPCSTR)rootPath, fd.cFileName);

			// ── 사용자 폴더 내 파일 전체 순회 ──────────────
			CString innerSearch = userFolder + "\\*";
			WIN32_FIND_DATAA fd2 = {};
			HANDLE hFind2 = FindFirstFileA(innerSearch, &fd2);
			if (hFind2 == INVALID_HANDLE_VALUE)
				continue;

			do {
				if (fd2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

				CString fname(fd2.cFileName);

				// .ini 포함된 파일만, .bak/.tmp 제외
				if (fname.Find(".ini") == -1) continue;
				if (fname.Right(4) == ".bak") continue;
				if (fname.Right(4) == ".tmp") continue;

				CString iniPath;
				iniPath.Format("%s\\%s", (LPCSTR)userFolder, (LPCSTR)fname);

				try {
					if (fname == CString(fd.cFileName) + ".ini")
					{
						CString endataPath;
						endataPath.Format("%s\\endata.dll", (LPCSTR)userFolder);

						if (Axis::isCustomer)
						{
							// 고객 PC: 평문 백업을 만들지 않고, 과거 버전이 남긴 잔존 파일도 정리
							if (GetFileAttributesA(endataPath) != INVALID_FILE_ATTRIBUTES)
							{
								DeleteFileA(endataPath);
								m_slog.Format("[ENC][BACKUP] customer - removed stale [%s]", endataPath);
								OutputDebugString(m_slog);
							}
						}
						else if (HasPlainAccountHistory(iniPath))
						{
							// 직원/지원용: 아직 평문일 때만 1회 복구본 생성
							if (CopyFileA(iniPath, endataPath, FALSE))
							{
								m_slog.Format("[ENC][BACKUP] [%s] -> [%s]", iniPath, endataPath);
								OutputDebugString(m_slog);
							}
							else
							{
								m_slog.Format("[ENC][BACKUP] copy failed [%s] -> [%s]", iniPath, endataPath);
								OutputDebugString(m_slog);
							}
						}
					}
					EncryptIniFile(iniPath, key);
					successCount++;
					m_slog.Format("[ENC] iniPath=[%s] successCount=[%d]", iniPath, successCount);
					OutputDebugString(m_slog);
				}
				catch (const std::exception& e) {
					m_slog.Format("[ENC] 파일암호화 실패 = [%s]", iniPath);
					OutputDebugString(m_slog);
					OutputDebugStringA(e.what());
					failCount++;
				}

			} while (FindNextFileA(hFind2, &fd2));

			FindClose(hFind2);

		} while (FindNextFileA(hFind, &fd));

		FindClose(hFind);
		SecureZeroMemory(key.data(), key.size());
		return { successCount, failCount };
	}

void CMainFrame::DecryptIniFile(const CString& iniPath, const std::vector<BYTE>& enkey)
{
	std::ifstream fin(iniPath);
	if (!fin.is_open())
		throw std::runtime_error("Cannot open ini file");

	std::ostringstream out;
	std::string line;
	bool inAccountHistory = false;

	while (std::getline(fin, line))
	{
		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		if (!line.empty() && line.front() == '[')
		{
			inAccountHistory = (line == "[AccountHistory]");
			out << line << '\n';
			continue;
		}

		if (inAccountHistory && !line.empty() && line.front() != ';')
		{
			auto eq = line.find('=');
			if (eq != std::string::npos)
			{
				std::string key = line.substr(0, eq);
				std::string val = line.substr(eq + 1);

				// '|' absent and not VERSION = encrypted -> decrypt
				// '|' present = plain text, leave as-is
				if (!val.empty() && key != "VERSION" && val.find('|') == std::string::npos)
				{
#ifdef DF_ENC_AES
					CString decVal = AesDecrypt(CString(val.c_str()), enkey);
#else
					CString decVal = DpapiDecrypt(CString(val.c_str()));
#endif
					val = (LPCSTR)decVal;

					m_slog.Format("[ENC][DEC] [%d] [%s]", decVal.GetLength(), decVal);
					OutputDebugString(m_slog);
				}

				out << key << '=' << val << '\n';
				continue;
			}
		}
		out << line << '\n';
	}
	fin.close();

	CString tmpPath = iniPath + ".tmp";
	{
		std::ofstream fout(tmpPath);
		if (!fout.is_open())
			throw std::runtime_error("Cannot write tmp file");
		fout << out.str();
	}

	if (!MoveFileExA(tmpPath, iniPath,
		MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		DeleteFileA(tmpPath);
		throw std::runtime_error("File replace failed");
	}
}

std::pair<int, int> CMainFrame::DecryptAllUserIni(const CString& rootPath)
{
	int successCount = 0;
	int failCount = 0;

	CString searchPath = rootPath + "\\*";
	WIN32_FIND_DATAA fd = {};
	HANDLE hFind = FindFirstFileA(searchPath, &fd);

	if (hFind == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Cannot open root folder");

	std::vector<BYTE> key = DeriveKeyFromRegkey(m_regkey);
	do {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
		if (strcmp(fd.cFileName, ".") == 0) continue;
		if (strcmp(fd.cFileName, "..") == 0) continue;

		CString iniPath;
		iniPath.Format("%s\\%s\\%s.ini", (LPCSTR)rootPath, fd.cFileName, fd.cFileName);

		if (GetFileAttributesA(iniPath) == INVALID_FILE_ATTRIBUTES)
			continue;

		try {
			DecryptIniFile(iniPath, key);
			successCount++;
		}
		catch (const std::exception& e) {
			OutputDebugStringA(e.what());
			CString tmpPath = iniPath + ".tmp";  // 추가
			DeleteFileA(tmpPath);                // 추가
			failCount++;
		}

	} while (FindNextFileA(hFind, &fd));

	FindClose(hFind);
	SecureZeroMemory(key.data(), key.size());
	return { successCount, failCount };
}


CString CMainFrame::AesEncrypt(const CString& plainText, const std::vector<BYTE>& key)
{
	HCRYPTPROV hProv = 0;
	HCRYPTKEY  hKey = 0;

	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
		throw std::runtime_error("CryptAcquireContext failed");

	// 키 임포트용 구조체
	struct {
		BLOBHEADER hdr;
		DWORD      keyLen;
		BYTE       keyData[32];
	} keyBlob = {};

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_256;
	keyBlob.keyLen = 32;
	memcpy(keyBlob.keyData, key.data(), 32);

	if (!CryptImportKey(hProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey)) {
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptImportKey failed");
	}

	// CBC 모드 설정
	DWORD mode = CRYPT_MODE_CBC;
	CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);

	// IV 랜덤 생성 (16바이트)
	BYTE iv[16] = {};
	CryptGenRandom(hProv, 16, iv);
	CryptSetKeyParam(hKey, KP_IV, iv, 0);

	// CP949 바이트 기준으로 복사 (한글 2바이트 정확히 처리)
	LPCSTR pszPlain = (LPCSTR)plainText;
	DWORD  plainLen = (DWORD)strlen(pszPlain);  // 실제 바이트 수

	// 암호화 버퍼: AES 블록(16바이트) 단위 올림 + 여유
	DWORD bufLen = ((plainLen / 16) + 1) * 16;
	std::vector<BYTE> data(bufLen, 0);
	memcpy(data.data(), pszPlain, plainLen);

	DWORD dataLen = plainLen;
	if (!CryptEncrypt(hKey, 0, TRUE, 0, data.data(), &dataLen, bufLen)) {
		CryptDestroyKey(hKey);
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptEncrypt failed");
	}

	CryptDestroyKey(hKey);
	CryptReleaseContext(hProv, 0);

	// IV(16바이트) + 암호문 합쳐서 Base64 인코딩
	std::vector<BYTE> ivAndData;
	ivAndData.reserve(16 + dataLen);
	ivAndData.insert(ivAndData.end(), iv, iv + 16);
	ivAndData.insert(ivAndData.end(), data.begin(), data.begin() + dataLen);

	// 민감 데이터 정리
	SecureZeroMemory(data.data(), data.size());
	SecureZeroMemory(keyBlob.keyData, 32);

	DWORD b64Len = 0;
	CryptBinaryToStringA(ivAndData.data(), (DWORD)ivAndData.size(),
		CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, nullptr, &b64Len);
	CString b64;
	CryptBinaryToStringA(ivAndData.data(), (DWORD)ivAndData.size(),
		CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
		b64.GetBuffer(b64Len), &b64Len);
	b64.ReleaseBuffer();

	return b64;
}

CString CMainFrame::AesDecrypt(const CString& encText, const std::vector<BYTE>& key)
{
	// Base64 디코딩
	DWORD blobLen = 0;
	CryptStringToBinaryA(encText, encText.GetLength(), CRYPT_STRING_BASE64,
		nullptr, &blobLen, nullptr, nullptr);
	if (blobLen < 16)
		throw std::runtime_error("Invalid encrypted data");

	std::vector<BYTE> ivAndData(blobLen);
	CryptStringToBinaryA(encText, encText.GetLength(), CRYPT_STRING_BASE64,
		ivAndData.data(), &blobLen, nullptr, nullptr);

	// IV(앞 16바이트) / 암호문 분리
	BYTE iv[16] = {};
	memcpy(iv, ivAndData.data(), 16);
	std::vector<BYTE> data(ivAndData.begin() + 16, ivAndData.end());

	HCRYPTPROV hProv = 0;
	HCRYPTKEY  hKey = 0;

	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
		throw std::runtime_error("CryptAcquireContext failed");

	struct {
		BLOBHEADER hdr;
		DWORD      keyLen;
		BYTE       keyData[32];
	} keyBlob = {};

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_256;
	keyBlob.keyLen = 32;
	memcpy(keyBlob.keyData, key.data(), 32);

	if (!CryptImportKey(hProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey)) {
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptImportKey failed");
	}

	DWORD mode = CRYPT_MODE_CBC;
	CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);
	CryptSetKeyParam(hKey, KP_IV, iv, 0);

	DWORD dataLen = (DWORD)data.size();
	if (!CryptDecrypt(hKey, 0, TRUE, 0, data.data(), &dataLen)) {
		CryptDestroyKey(hKey);
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptDecrypt failed");
	}

	CryptDestroyKey(hKey);
	CryptReleaseContext(hProv, 0);
	SecureZeroMemory(keyBlob.keyData, 32);

	// ★ null terminator 공간 확보
	data.resize(dataLen + 1, 0);
	data[dataLen] = 0;
	CString result((LPCSTR)data.data());

	SecureZeroMemory(data.data(), data.size());
	return result;
}

// ── m_regkey → 32바이트 AES 키 파생 (SHA-256) ───────────────────
std::vector<BYTE> CMainFrame::DeriveKeyFromRegkey(const CString& regkey)
{
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
		throw std::runtime_error("CryptAcquireContext failed");

	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptCreateHash failed");
	}

	// m_regkey 문자열을 해시 입력으로
	if (!CryptHashData(hHash, (BYTE*)(LPCSTR)regkey, regkey.GetLength(), 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptHashData failed");
	}

	DWORD keyLen = 32;
	std::vector<BYTE> key(keyLen);
	if (!CryptGetHashParam(hHash, HP_HASHVAL, key.data(), &keyLen, 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptGetHashParam failed");
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	return key;  // SHA-256 = 32바이트 = AES-256 키로 딱 맞음
}

bool CMainFrame::isIPInRange24(CString ip, CString network)
{
	const unsigned int ip_addr = IPToUInt(ip);
	const unsigned int network_addr = IPToUInt(network);
	const unsigned int mask_addr = IPToUInt("255.255.255.0");   // /24

	const unsigned int net_lower = (network_addr & mask_addr);
	const unsigned int net_upper = (net_lower | (~mask_addr));

	if (ip_addr >= net_lower && ip_addr <= net_upper)
		return true;
	return false;
}

bool CMainFrame::IsHeadquartersIP(CString ip)
{
	static const char* HQ_RANGES[] = {
		"172.17.0.0",    // 1F
		"172.17.1.0",    // 4F(IT)
		"172.17.4.0",    // 2F
		"172.17.5.0",    // 7F
		"172.17.6.0",    // 3F
		"172.17.7.0",    // 6F
		"172.17.8.0",    // 5F
		"172.17.12.0",   // 9F
		"172.17.13.0",   // 10F
		"172.17.14.0",   // 11F
		"172.17.16.0",   // 8F
	};

	for (size_t i = 0; i < _countof(HQ_RANGES); ++i)
	{
		if (isIPInRange24(ip, HQ_RANGES[i]))
			return true;
	}
	return false;
}

void CMainFrame::AccEncrypt()
{
	CString filename;
	filename.Format("%s\\%s\\%s", Axis::home, "tab", "axis.ini");
	WritePrivateProfileString("AXIS", "reg", m_regkey, filename);

	filename.Format("%s\\%s\\AXISAI.ini", Axis::home, "tab");
	int dw = GetPrivateProfileInt("ENC", "acchistory", 0, filename);

	if (dw == 0)
		return;

	//if (!IsHeadquartersIP(m_ipAddr))
	//{
	//	m_slog.Format("[AXIS][AccEncrypt] Skipped - not HQ IP=[%s]", m_ipAddr);
	//	output_DebugString(m_slog);
	//	return;
	//}

	CString sPath;
	CString strFilePath;
	CFile	file;
	CString sBuf;
	TCHAR	chFileName[128]{};

	GetModuleFileName(NULL, chFileName, MAX_PATH);

	strFilePath.Format(_T("%s"), chFileName);

	strFilePath = strFilePath.Left(strFilePath.ReverseFind('\\'));
	strFilePath.Replace("\\exe", "\\user");

	auto [ok, fail] = EncryptAllUserIni(strFilePath);

	if (fail > 0) {
		// 로그 남기거나 관리자에게 알림
		// 단, 실패한 파일은 평문 그대로 → 다음 실행에 재시도됨
		//AfxMessageBox("파일 암호화 중 에러");
	}
}

//============================================================
// FileManager: 메인 시작 시 1회 실행.
//   Axis::home\exe\filemanager.ini 를 읽어 파일 제어 작업 수행.
//   - DELETE : 파일 삭제
//   - SETKEY : 비표준 "키 = 값" 설정파일의 특정 키 값만 교체(빈값 포함)
//   VERSION 으로 중복 실행을 막고, AFTER=DELETE 면 ini 자체를 삭제.
//============================================================

// 폴더키 → 실제 경로 매핑 (화이트리스트). 목록 밖이면 빈 문자열 반환 → skip.
 CString CMainFrame::FM_MapFolderKey(const CString& key)
{
	CString k = key;  k.MakeUpper();  k.TrimRight();
	if (k == "EXE")   return Axis::home + "\\exe";
	if (k == "DEV")   return Axis::home + "\\dev";
	if (k == "GEX")   return Axis::home + "\\gex";
	if (k == "TAB")   return Axis::home + "\\tab";
	if (k == "XC32")  return Axis::home + "\\xc32_win64\\conf";
	if (k == "USER")  return Axis::home + "\\user";
	return "";
}

// 비표준 "키 = 값" 설정파일에서 지정 키의 값만 newValue로 교체. 반환: 바꾼 줄 수.
// (xc_conf.ini 처럼 섹션이 없고 '=' 양옆 공백/탭이 섞인 파일용. WritePrivateProfile* 못 씀)
 int CMainFrame::FM_SetPlainConfValue(const CString& filePath,
	const CString& key,
	const CString& newValue)
{
	CFile f;
	if (!f.Open(filePath, CFile::modeRead | CFile::shareDenyWrite))
	{
		WriteLog("[FM] SETKEY open fail: %s\n", (LPCSTR)filePath);
		return 0;
	}
	ULONGLONG len = f.GetLength();
	CStringA buf;
	if (len > 0)
	{
		f.Read(buf.GetBuffer((int)len), (UINT)len);
		buf.ReleaseBuffer((int)len);
	}
	f.Close();

	CStringA keyA(key);        keyA.TrimRight();
	CStringA valA(newValue);   // 빈 문자열이면 값 제거

	CStringA out;
	int changed = 0;
	int pos = 0;
	while (pos < buf.GetLength())
	{
		int nl = buf.Find('\n', pos);
		CStringA line, eol;
		if (nl < 0) { line = buf.Mid(pos);            pos = buf.GetLength(); }
		else { line = buf.Mid(pos, nl - pos);  pos = nl + 1;  eol = "\n"; }

		if (!line.IsEmpty() && line[line.GetLength() - 1] == '\r')
		{
			line = line.Left(line.GetLength() - 1);
			eol = "\r\n";
		}

		CStringA trimmed = line;  trimmed.TrimLeft();
		int eq = line.Find('=');
		if (eq < 0 || (!trimmed.IsEmpty() && trimmed[0] == '#'))
		{
			out += line;  out += eol;   // 주석/'=' 없는 줄은 그대로
			continue;
		}

		CStringA lhs = line.Left(eq);
		CStringA lhsTrim = lhs;  lhsTrim.TrimRight();

		if (lhsTrim.CompareNoCase(keyA) == 0)
		{
			CStringA rebuilt = lhs;   // 키 + 정렬용 공백/탭 원형 보존
			rebuilt += "= ";
			rebuilt += valA;          // 빈값이면 여기서 끝
			out += rebuilt;  out += eol;
			changed++;
			WriteLog("[FM] SETKEY: %s -> [%s]\n", (LPCSTR)keyA, (LPCSTR)valA);
		}
		else
		{
			out += line;  out += eol;
		}
	}

	if (changed == 0)
	{
		WriteLog("[FM] SETKEY key not found: %s in %s\n", (LPCSTR)keyA, (LPCSTR)filePath);
		return 0;
	}

	// 임시 파일에 쓰고 원자적 교체
	CString tmp = filePath + ".tmp";
	CFile wf;
	if (!wf.Open(tmp, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive))
	{
		WriteLog("[FM] SETKEY tmp open fail: %s\n", (LPCSTR)tmp);
		return 0;
	}
	if (out.GetLength() > 0)
		wf.Write((LPCSTR)out, out.GetLength());
	wf.Close();

	::DeleteFile(filePath);
	if (!::MoveFile(tmp, filePath))
	{
		WriteLog("[FM] SETKEY replace fail(%d): %s\n", GetLastError(), (LPCSTR)filePath);
		::DeleteFile(tmp);   // 교체 실패 시 임시파일 정리
		return 0;
	}
	return changed;
}

// 메인 시작 시 호출. setupFile: 처리버전(FMDONEVER)을 기록할 셋업 ini 경로.
void CMainFrame::ProcessFileManager(const CString& setupFile)
{
	// filemanager.ini 위치 (일단 exe 폴더 기준)
	CString fmFile;
	fmFile.Format("%s\\tab\\filemanager.ini", (LPCSTR)Axis::home);

	if (::GetFileAttributes(fmFile) == INVALID_FILE_ATTRIBUTES)
		return;   // 파일 없으면 조용히 종료

	// 버전 기반 중복 실행 방지
	int fmVer = GetPrivateProfileInt("GENERAL", "FMVERSION", 0, fmFile);
	int doneVer = GetPrivateProfileInt("SCREEN", "FMDONEVER", 0, fmFile);
	bool bForce = (fmVer < 0);
	m_slog.Format("[FM]  (fmVer=%d <= doneVer=%d)\n", fmVer, doneVer);
	output_DebugString(m_slog);
	if (!bForce && fmVer <= doneVer)
	{
		m_slog.Format("[FM] skip (fmVer=%d <= doneVer=%d)\n", fmVer, doneVer);
		output_DebugString(m_slog);
		return;
	}

	int nCount = GetPrivateProfileInt("GENERAL", "COUNT", 0, fmFile);
	m_slog.Format("[FM] start: ver=%d, count=%d\n", fmVer, nCount);
	output_DebugString(m_slog);

	for (int i = 1; i <= nCount; i++)
	{
		CString sec;  sec.Format("ITEM%d", i);

		char szAct[64] = { 0 }, szFolder[64] = { 0 }, szFile[260] = { 0 };
		GetPrivateProfileString(sec, "ACTION", "", szAct, sizeof(szAct), fmFile);
		GetPrivateProfileString(sec, "FOLDER", "", szFolder, sizeof(szFolder), fmFile);
		GetPrivateProfileString(sec, "FILE", "", szFile, sizeof(szFile), fmFile);

		CString act = szAct;       act.MakeUpper();  act.TrimLeft();
		CString dir = FM_MapFolderKey(szFolder);
		CString fileName = szFile; fileName.TrimLeft();

		// 안전 가드: 폴더 매핑 실패 / 빈 파일명 / 경로 탈출 문자 차단
		if (dir.IsEmpty() || fileName.IsEmpty())
		{
			m_slog.Format("[FM] ITEM%d skip (folder/file invalid)\n", i);
			output_DebugString(m_slog);
			continue;
		}
		if (fileName.Find("..") >= 0 || fileName.Find("\\") >= 0
			|| fileName.Find("/") >= 0 || fileName.Find(":") >= 0)
		{
			m_slog.Format("[FM] ITEM%d skip (path escape blocked): %s\n", i, (LPCSTR)fileName);
			output_DebugString(m_slog);
			continue;
		}

		CString full = dir + "\\" + fileName;

		if (act == "DELETE")
		{
			if (::GetFileAttributes(full) == INVALID_FILE_ATTRIBUTES)
				WriteLog("[FM] DELETE skip (not exist): %s\n", full);
			else if (::DeleteFile(full))
				WriteLog("[FM] DELETE ok: %s\n", (full));
			else
				WriteLog("[FM] DELETE fail(%d): %s\n", full);
		}
		else if (act == "SETKEY")
		{
			char szKey[128] = { 0 }, szVal[256] = { 0 };
			GetPrivateProfileString(sec, "KEY", "", szKey, sizeof(szKey), fmFile);
			GetPrivateProfileString(sec, "VALUE", "", szVal, sizeof(szVal), fmFile);

			CString k = szKey;  k.TrimLeft();
			if (k.IsEmpty())
			{
				WriteLog("[FM] ITEM%d SETKEY skip (empty key)\n", i);
				continue;
			}
			if (::GetFileAttributes(full) == INVALID_FILE_ATTRIBUTES)
			{
				WriteLog("[FM] SETKEY skip (not exist): %s\n", (LPCSTR)full);
				continue;
			}
			FM_SetPlainConfValue(full, k, CString(szVal));
		}
		else if (act == "DELUSERFILE")
		{
			CString searchPath = dir + "\\*";
			WIN32_FIND_DATAA fd = {};
			HANDLE hFind = FindFirstFileA(searchPath, &fd);
			if (hFind != INVALID_HANDLE_VALUE)
			{
				do {
					if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
					if (strcmp(fd.cFileName, ".") == 0) continue;
					if (strcmp(fd.cFileName, "..") == 0) continue;

					CString target;
					target.Format("%s\\%s\\%s", (LPCSTR)dir, fd.cFileName, (LPCSTR)fileName);
					if (::GetFileAttributes(target) == INVALID_FILE_ATTRIBUTES)
						WriteLog("[FM] DELUSERFILE skip (not exist): %s\n", (LPCSTR)target);
					else if (::DeleteFile(target))
						WriteLog("[FM] DELUSERFILE ok: %s\n", (LPCSTR)target);
					else
						WriteLog("[FM] DELUSERFILE fail: %s\n", (LPCSTR)target);
				} while (FindNextFileA(hFind, &fd));
				FindClose(hFind);
			}
		}
		else
		{
			WriteLog("[FM] ITEM%d unknown action: %s\n", i, (LPCSTR)act);
		}
	}

	// 처리 버전 기록
	if (fmVer >= 0)
	{
		CString v;  v.Format("%d", fmVer);
		WritePrivateProfileString("SCREEN", "FMDONEVER", v, fmFile);
	}

	// AFTER 처리
	char szAfter[16] = { 0 };
	GetPrivateProfileString("GENERAL", "AFTER", "KEEP", szAfter, sizeof(szAfter), fmFile);
	if (_stricmp(szAfter, "DELETE") == 0)
	{
		if (::DeleteFile(fmFile))
			WriteLog("[FM] ini deleted (AFTER=DELETE)\n");
	}

	WriteLog("[FM] done.\n");
}

bool CMainFrame::HasPlainAccountHistory(const CString& iniPath)
{
	std::vector<char> buf(4096);
	DWORD len = GetPrivateProfileSection("AccountHistory", buf.data(), (DWORD)buf.size(), iniPath);

	// 섹션이 버퍼보다 크면(반환값 == 버퍼크기-2) 버퍼를 키워서 재시도
	while (len == buf.size() - 2)
	{
		buf.resize(buf.size() * 4);
		len = GetPrivateProfileSection("AccountHistory", buf.data(), (DWORD)buf.size(), iniPath);
	}

	for (const char* p = buf.data(); *p; p += strlen(p) + 1)
	{
		const char* eq = strchr(p, '=');
		if (!eq) continue;

		std::string key(p, eq - p);
		std::string val(eq + 1);

		if (key != "VERSION" && !val.empty() && val.find('|') != std::string::npos)
			return true;
	}
	return false;
}
#endif

//int CMainFrame::ScreenCheck(CString mapname, int  igubn)
//{
//	//#define DF_NUSE 0					    //#0x00 사용불가      0  
//	//#define DF_YUSE 1					     //#0x01 사용가능      1
//	//#define DF_NUSE_AFTERDAY 2    //#0x02 특정날짜이후 사용가능    2
//	//#define DF_YUSE_AFTERDAY 4  //#0x02 특정날짜이후 사용중지    4
//	//#define DF_YUSE_MANAGER 8  //#0x08 관리자전용  8
//	//#define DF_YUSE_DEBUG   16   //#0x10 디버그기능 포함   16
//	//#define DF_NUSE_HTSOPEN   32   //#HTS 시작할때는 안띄움
//	//#define DF_YUSE_GUIDEPOP   64   //특정시간에는 대체팝업
//	m_slog.Format("[ScreenCheck] mapname=%s igubn=[%d]", mapname, igubn);
//	OutputDebugString(m_slog);
//
//	CString sData{};
//	if (!m_mapManage.Lookup(mapname, sData))
//	{
//		if (mapname.Left(2) == "00")
//		{
//			mapname = "IB" + mapname.Mid(2);
//			if (!m_mapManage.Lookup(mapname, sData))
//			{
//				m_slog.Format("[ScreenCheck]1 YESUSE mapname=%s igubn=[%d]", mapname, igubn);
//				OutputDebugString(m_slog);
//				return DF_YUSE;
//			}
//		}
//		else
//		{
//			m_slog.Format("[ScreenCheck]2 YESUSE mapname=%s igubn=[%d]", mapname, igubn);
//			OutputDebugString(m_slog);
//			return DF_YUSE;
//		}
//	}
//
//	CString sval, sDate, sPopType, sPopMap, sUrl;
//	sval = Parse(sData, "|");
//	sPopType = Parse(sData, "|");
//	sDate = Parse(sData, "|");
//	sPopMap = Parse(sData, "|");
//	sUrl = Parse(sData, "|");
//
//	if (igubn == 1 && sval == "32")
//	{  //HTS 시작할때는 #IB586000=32|0|||    이런식이면 안띄운다 
//		m_slog.Format("[ScreenCheck] HTS 시작시 DF_NUSE mapname=%s", mapname);
//		OutputDebugString(m_slog);
//		return DF_NUSE;
//	}
//
//	if (sval == "64" && igubn != 1)  //HTS 시작하거나 끌때는 안한다
//	{
//		char buff[256]{};
//		CString file, tmps;
//		file.Format("%s\\%s\\SWITCHING.INI", Axis::home, TABDIR);
//		GetPrivateProfileString("해외", "STIM", "720", buff, sizeof(buff), file);
//		tmps.Format("%s", buff);
//		tmps.TrimRight();
//
//		int from = _ttoi(tmps);
//
//		GetPrivateProfileString("해외", "ETIM", "750", buff, sizeof(buff), file);
//		tmps.Format("%s", buff);
//		tmps.TrimRight();
//
//		int end = _ttoi(tmps);
//
//		CTime now = CTime::GetCurrentTime();
//
//		int nowTime = now.GetHour() * 100 + now.GetMinute();
//
//		bool bIn = (nowTime >= from && nowTime <= end);
//
//		if (bIn)
//		{
//			m_mapHelper->CreatePopup(sPopMap, 1, WK_POPUP, 5);
//			return DF_YUSE;
//		}
//	}
//	else if (sval == "0")     //사용안하는 화면에 대한 처리
//	{
//		if (!sPopMap.IsEmpty())   //맵으로 띄우는 안내팝업에 대한 내용이 있을경우
//		{
//			if (sPopType.IsEmpty())
//				sPopType = "1";
//
//			if (sPopType == "1")  //일반화면
//				m_mapHelper->ChangeChild(sPopMap, 1, 0, 5);
//			else if (sPopType == "2")
//				m_mapHelper->CreateModal(sPopMap);
//			else if (sPopType == "2")
//				m_mapHelper->CreatePopup(sPopMap);
//		}
//		else	if (sPopType == "0")  //그냥 안띄우고 넘어감
//			return DF_NUSE;
//		else //url 팝업을 띄운다
//		{
//			CDlg_MSGBOX dlg;
//			if (dlg.DoModal() == IDOK)
//			{
//				m_iKey = m_mapHelper->ChangeChild("IB980001");
//
//				m_sTriggerUrl.Format("edUrlTrigger\t%s", sUrl);
//
//				SetTimer(TM_NOSCREEN_URL, 1000, nullptr);
//			}
//		}
//		return DF_NUSE;
//	}
//
//	return DF_YUSE;
//}