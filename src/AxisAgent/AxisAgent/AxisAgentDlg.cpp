
// AxisAgentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "AxisAgent.h"
#include "AxisAgentDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "CL_ENC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const int MAX_DUMP_COUNT = 3;
static const DWORD DUMP_INTERVAL_MS = 1000;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
#include <string>



namespace
{
	static bool IsWaitLikeFunction(const char* name)
	{
		if (!name || !name[0]) return false;

		return
			strstr(name, "Wait") != NULL ||
			strstr(name, "SleepConditionVariable") != NULL ||
			strstr(name, "SRW") != NULL ||
			strstr(name, "CriticalSection") != NULL ||
			strstr(name, "NtWait") != NULL ||
			strstr(name, "ZwWait") != NULL ||
			strstr(name, "MsgWait") != NULL;
	}

	static bool IsUiLikeFunction(const char* name)
	{
		if (!name || !name[0]) return false;

		return
			strstr(name, "CUGCell") != NULL ||
			strstr(name, "axCreate") != NULL ||
			strstr(name, "GetSign") != NULL ||
			strstr(name, "OnPaint") != NULL ||
			strstr(name, "Draw") != NULL ||
			strstr(name, "user32") != NULL;
	}

	static void WriteMainSummary(
		CAxisAgentDlg* pThis,
		DWORD threadId,
		const char* f0,
		const char* f1,
		const char* f2)
	{
		if (!pThis) return;

		const char* s0 = (f0 && f0[0]) ? f0 : "?";
		const char* s1 = (f1 && f1[0]) ? f1 : "?";
		const char* s2 = (f2 && f2[0]) ? f2 : "?";

		char buf[1024] = { 0 };
		sprintf_s(buf,
			"[MAIN_SUMMARY] TID=%lu TOP=%s | CALLER1=%s | CALLER2=%s",
			threadId, s0, s1, s2);
		pThis->WriteMonitorLog(buf);

		if (IsWaitLikeFunction(s0) || IsWaitLikeFunction(s1) || IsWaitLikeFunction(s2))
		{
			sprintf_s(buf,
				"[MAIN_WAIT] TID=%lu wait-pattern detected: %s / %s / %s",
				threadId, s0, s1, s2);
			pThis->WriteMonitorLog(buf);
		}

		if (IsUiLikeFunction(s0) || IsUiLikeFunction(s1) || IsUiLikeFunction(s2))
		{
			sprintf_s(buf,
				"[MAIN_UI] TID=%lu ui-pattern detected: %s / %s / %s",
				threadId, s0, s1, s2);
			pThis->WriteMonitorLog(buf);
		}
	}

	struct StackHotState
	{
		std::string lastTopFunc;
		DWORD64 lastTopAddr;
		int repeatCount;

		StackHotState()
			: lastTopAddr(0), repeatCount(0)
		{
		}
	};

	static StackHotState g_hotState;

	static void WriteTaggedLog(CAxisAgentDlg* pThis, const char* tag, const char* msg)
	{
		if (!pThis || !tag || !msg)
			return;

		char buf[1024] = { 0 };
		sprintf_s(buf, "[%s] %s", tag, msg);
		pThis->WriteMonitorLog(buf);
	}

	static void UpdateHotStateAndLog(
		CAxisAgentDlg* pThis,
		BOOL bMain,
		int frameNo,
		DWORD64 addr,
		const char* funcName,
		DWORD64 disp64)
	{
		if (!pThis || !funcName || !funcName[0])
			return;

		char msg[1024] = { 0 };

		// 1. TOP 프레임은 무조건 강조
		if (frameNo == 0)
		{
			sprintf_s(msg,
				"%s %s + 0x%I64X",
				bMain ? "[MAIN][TOP]" : "[TOP]",
				funcName,
				disp64);
			WriteTaggedLog(pThis, "HOT", msg);

			// 2. 같은 TOP 함수/주소 반복되면 STUCK 강조
			if (g_hotState.lastTopFunc == funcName && g_hotState.lastTopAddr == addr)
			{
				g_hotState.repeatCount++;
			}
			else
			{
				g_hotState.lastTopFunc = funcName;
				g_hotState.lastTopAddr = addr;
				g_hotState.repeatCount = 0;
			}

			if (g_hotState.repeatCount >= 2)
			{
				sprintf_s(msg,
					"%s 같은 TOP 반복 func=%s addr=0x%I64X repeat=%d",
					bMain ? "[MAIN][STUCK]" : "[STUCK]",
					funcName,
					addr,
					g_hotState.repeatCount);
				WriteTaggedLog(pThis, "HOT", msg);
			}
		}
	}

	struct DumpWalkContext
	{
		BYTE* pView;
		MINIDUMP_MEMORY_LIST* pMemList;
		MINIDUMP_MEMORY64_LIST* pMem64List;

		DumpWalkContext()
			: pView(NULL), pMemList(NULL), pMem64List(NULL)
		{
		}
	};

	static DumpWalkContext* g_pDumpWalkCtx = NULL;

	static const MINIDUMP_MEMORY_DESCRIPTOR* FindMemoryDesc32(
		MINIDUMP_MEMORY_LIST* pList, ULONG64 addr)
	{
		if (!pList)
			return NULL;

		for (ULONG i = 0; i < pList->NumberOfMemoryRanges; ++i)
		{
			const MINIDUMP_MEMORY_DESCRIPTOR& md = pList->MemoryRanges[i];
			const ULONG64 start = md.StartOfMemoryRange;
			const ULONG64 end = start + md.Memory.DataSize;
			if (addr >= start && addr < end)
				return &md;
		}
		return NULL;
	}

	static BOOL CALLBACK DumpReadMemoryRoutine(
		HANDLE /*hProcess*/,
		DWORD64 qwBaseAddress,
		PVOID lpBuffer,
		DWORD nSize,
		LPDWORD lpNumberOfBytesRead)
	{
		if (lpNumberOfBytesRead)
			*lpNumberOfBytesRead = 0;

		if (!lpBuffer || !g_pDumpWalkCtx)
			return FALSE;

		DumpWalkContext* ctx = g_pDumpWalkCtx;

		// MemoryListStream 우선
		if (ctx->pMemList)
		{
			const MINIDUMP_MEMORY_DESCRIPTOR* md =
				FindMemoryDesc32(ctx->pMemList, qwBaseAddress);
			if (!md)
				return FALSE;

			ULONG64 offset = qwBaseAddress - md->StartOfMemoryRange;
			ULONG64 remain = md->Memory.DataSize - offset;
			DWORD toCopy = (DWORD)((remain < (ULONG64)nSize) ? remain : (ULONG64)nSize);

			BYTE* src = ctx->pView + md->Memory.Rva + (SIZE_T)offset;
			memcpy(lpBuffer, src, toCopy);

			if (lpNumberOfBytesRead)
				*lpNumberOfBytesRead = toCopy;

			return TRUE;
		}

		// Memory64ListStream
		if (ctx->pMem64List)
		{
			ULONG64 curRva = ctx->pMem64List->BaseRva;

			for (ULONG64 i = 0; i < ctx->pMem64List->NumberOfMemoryRanges; ++i)
			{
				const MINIDUMP_MEMORY_DESCRIPTOR64& md = ctx->pMem64List->MemoryRanges[i];
					const ULONG64 start = md.StartOfMemoryRange;
						const ULONG64 end = start + md.DataSize;

				if (qwBaseAddress >= start && qwBaseAddress < end)
				{
					ULONG64 offset = qwBaseAddress - start;
					ULONG64 remain = md.DataSize - offset;
					DWORD toCopy = (DWORD)((remain < (ULONG64)nSize) ? remain : (ULONG64)nSize);

					BYTE* src = ctx->pView + (SIZE_T)(curRva + offset);
					memcpy(lpBuffer, src, toCopy);

					if (lpNumberOfBytesRead)
						*lpNumberOfBytesRead = toCopy;

					return TRUE;
				}

				curRva += md.DataSize;
			}
		}

		return FALSE;
	}

	static std::string DumpWideStringToAnsi(BYTE* pView, ULONG32 rva)
	{
		if (!pView || rva == 0)
			return std::string();

		MINIDUMP_STRING* pStr =
			reinterpret_cast<MINIDUMP_STRING*>(pView + rva);

		if (!pStr || pStr->Length == 0)
			return std::string();

		int wcharCount = (int)(pStr->Length / sizeof(WCHAR));
		int need = WideCharToMultiByte(
			CP_ACP, 0, pStr->Buffer, wcharCount,
			NULL, 0, NULL, NULL);

		if (need <= 0)
			return std::string();

		std::string out;
		out.resize(need);

		WideCharToMultiByte(
			CP_ACP, 0, pStr->Buffer, wcharCount,
			&out[0], need, NULL, NULL);

		return out;
	}
}

void dumptest()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	char selfPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, selfPath, MAX_PATH);
	char* p = strrchr(selfPath, '\\');
	if (p) *p = '\0';

	// DbgHelp.dll - exe 옆에 있는거 우선 로드 (없으면 시스템꺼)
	char dbgHelpPath[MAX_PATH] = { 0 };
	sprintf_s(dbgHelpPath, "%s\\DbgHelp.dll", selfPath);

	HMODULE hDbgHelp = LoadLibraryA(dbgHelpPath);
	if (!hDbgHelp)
	{
		//DebugLog("TestDump: exe 옆 DbgHelp.dll 없음 - 시스템꺼 로드\n");
		hDbgHelp = LoadLibraryA("DbgHelp.dll");
	}

	if (!hDbgHelp)
	{
		//AddLog("DUMP", "DbgHelp.dll 로드 실패");
		return;
	}

	// 버전 로그
	char loadedPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(hDbgHelp, loadedPath, MAX_PATH);
	//DebugLog("TestDump: DbgHelp.dll 로드 [%s]\n", loadedPath);

	typedef BOOL(WINAPI* PFN_MiniDumpWriteDump)(
		HANDLE, DWORD, HANDLE, MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION);

	PFN_MiniDumpWriteDump pfnDump =
		(PFN_MiniDumpWriteDump)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (!pfnDump)
	{
	//	AddLog("DUMP", "MiniDumpWriteDump GetProcAddress 실패");
		FreeLibrary(hDbgHelp);
		return;
	}

	// 덤프 파일 생성
	char dumpPath[MAX_PATH] = { 0 };
	sprintf_s(dumpPath, "%s\\ping\\test_dump_%04d%02d%02d_%02d%02d%02d.dmp",
		selfPath,
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	HANDLE hFile = CreateFileA(
		dumpPath,
		GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "TestDump: CreateFile 실패 err=%lu", GetLastError());
	//	AddLog("DUMP", buf);
		FreeLibrary(hDbgHelp);
		return;
	}

	MINIDUMP_TYPE dumpType = (MINIDUMP_TYPE)(
		MiniDumpWithDataSegs |
		MiniDumpWithHandleData |
		MiniDumpWithThreadInfo |
		MiniDumpWithUnloadedModules);

	BOOL bOk = pfnDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		dumpType,
		NULL, NULL, NULL);

	DWORD err = GetLastError();
	CloseHandle(hFile);
	FreeLibrary(hDbgHelp);

	if (bOk)
	{
		HANDLE hCheck = CreateFileA(dumpPath, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD size = 0;
		if (hCheck != INVALID_HANDLE_VALUE)
		{
			size = GetFileSize(hCheck, NULL);
			CloseHandle(hCheck);
		}

		char buf[256] = { 0 };
		sprintf_s(buf, "TestDump: 성공 size=%lu bytes", size);
	
	//	DebugLog("TestDump: [%s] %s\n", dumpPath, buf);
	}
	else
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "TestDump: 실패 err=%lu (0x%08X)", err, err);
	
	//	DebugLog("%s\n", buf);
	}

}

void CAxisAgentDlg::DebugLog(const char* fmt, ...)
{

	char msg[1024] = { 0 };
	va_list args;
	va_start(args, fmt);
	vsprintf_s(msg, sizeof(msg), fmt, args);
	va_end(args);

	char buf[1024] = { 0 };
	sprintf_s(buf, sizeof(buf), "[AxisAgent] %s", msg);
	OutputDebugStringA(buf);

	AddLog("DEBUG", msg);
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAxisAgentDlg 대화 상자


IMPLEMENT_DYNAMIC(CAxisAgentDlg, CDialogEx);

CAxisAgentDlg::CAxisAgentDlg(CWnd* pParent)
	: CDialogEx(IDD_AXISAGENT_DIALOG, pParent)
	, m_hParent(NULL)
	, m_hStopEvent(NULL)
	, m_hParentWnd(NULL)
	, m_parentPid(0)
	, m_hPingThread(NULL)
	, m_hMonitorThread(NULL)
	, m_hPingProcess(NULL)
	, m_hPingPipeRead(NULL)
	, m_hPingPipeWrite(NULL)
	, m_dumpSeq(0)
	, m_dumpCount(0)
	, m_lastDumpTick(0)
{
	memset(m_logFile, 0, sizeof(m_logFile));
	memset(m_regkey, 0, sizeof(m_regkey));
}

CAxisAgentDlg::~CAxisAgentDlg()
{
	// 이 대화 상자에 대한 자동화 프록시가 있을 경우 이 대화 상자에 대한
	//  후방 포인터를 null로 설정하여
	//  대화 상자가 삭제되었음을 알 수 있게 합니다.
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void CAxisAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG,      m_listLog);
	DDX_Control(pDX, IDC_CHK_PING,     m_chkPing);
	DDX_Control(pDX, IDC_CHK_MONITOR,  m_chkMonitor);
	DDX_Control(pDX, IDC_CHK_STOP,     m_chkStop);
	DDX_Control(pDX, IDC_EDIT_SYMPATH, m_editSymPath);
}

BEGIN_MESSAGE_MAP(CAxisAgentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_TEST, &CAxisAgentDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_CHK_PING, &CAxisAgentDlg::OnBnClickedChkPing)
	ON_BN_CLICKED(IDC_CHK_MONITOR, &CAxisAgentDlg::OnBnClickedChkMonitor)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_STOP, &CAxisAgentDlg::OnBnClickedChkStop)
	ON_BN_CLICKED(IDC_LIST_CLEAR, &CAxisAgentDlg::OnBnClickedListClear)
	ON_BN_CLICKED(IDC_BTN_DUMPANS, &CAxisAgentDlg::OnBnClickedBtnDumpans)
	ON_COMMAND(ID_TRAY_SHOW, OnTrayShow)
	ON_COMMAND(ID_TRAY_HIDE, OnTrayHide)
	ON_COMMAND(ID_TRAY_EXIT, OnTrayExit)
	ON_MESSAGE(WM_TRAYICON, OnTrayIcon)
	ON_BN_CLICKED(IDC_BTN_HIDE, &CAxisAgentDlg::OnBnClickedBtnHide)
	ON_BN_CLICKED(IDC_BTN_DECACC, &CAxisAgentDlg::OnBnClickedBtnDecacc)
	ON_BN_CLICKED(IDC_BTN_DUMPOPEN, &CAxisAgentDlg::OnBnClickedBtnDumpOpen)
	ON_BN_CLICKED(IDC_BTN_SYMPATH,  &CAxisAgentDlg::OnBnClickedBtnSympath)
END_MESSAGE_MAP()


// CAxisAgentDlg 메시지 처리기
struct AGENT_REG_INFO
{
	HWND   hAgentWnd;       // 에이전트 메인 다이얼로그 HWND
	DWORD  dwAgentPid;      // 에이전트 PID (검증용)
	DWORD  dwVersion;       // 프로토콜 버전 (확장 대비)
};

void CAxisAgentDlg::RegisterToMain()
{
	AGENT_REG_INFO info = { 0 };
	info.hAgentWnd = GetSafeHwnd();
	info.dwAgentPid = ::GetCurrentProcessId();
	info.dwVersion = 1;

	COPYDATASTRUCT cds = { 0 };
	cds.dwData = AGENT_MSG_AGENT_REGISTER;
	cds.cbData = sizeof(info);
	cds.lpData = &info;

	::SendMessageTimeout(
		m_hParentWnd, WM_COPYDATA,
		(WPARAM)GetSafeHwnd(),
		(LPARAM)&cds,
		SMTO_ABORTIFHUNG, 2000, NULL);
}


bool CAxisAgentDlg::ShouldShowLogType(const char* type)
{
	CString sType = type ? type : "";

	if (sType.CompareNoCase("PING") == 0)
		return m_chkPing.GetCheck() == BST_CHECKED;

	if (sType.CompareNoCase("MONITOR") == 0)
		return m_chkMonitor.GetCheck() == BST_CHECKED;

	// INIT, DEBUG 같은 건 일단 항상 보이게 할지,
	// 아니면 MONITOR 쪽으로 묶을지 선택
	return true;
}

BOOL CAxisAgentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	SetIcon(hIcon, TRUE);   // 큰 아이콘 (Alt+Tab, 작업표시줄)
	SetIcon(hIcon, FALSE);  // 작은 아이콘 (타이틀바)

	((CWnd*)GetDlgItem(IDOK))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDCANCEL))->ShowWindow(SW_HIDE);

	m_chkPing.SetCheck(BST_CHECKED);
	m_chkMonitor.SetCheck(BST_CHECKED);

	m_listLog.SetExtendedStyle(
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_DOUBLEBUFFER);

	m_listLog.InsertColumn(0, "Time", LVCFMT_LEFT, 90);
	m_listLog.InsertColumn(1, "Type", LVCFMT_LEFT, 80);
	m_listLog.InsertColumn(2, "Message", LVCFMT_LEFT, 500);
	AddLog("INIT", "리스트 테스트");

	// 커맨드라인 파싱
#ifdef _DEBUG   //디버그는 다보이기
	if (m_parentPid == 0)
	{
		m_parentPid = GetCurrentProcessId();  // 자기 자신
		m_hParentWnd = this->GetSafeHwnd();
		strcpy_s(m_regkey, "TEST");
		m_bShow = TRUE;
		m_startX = 100;
		m_startY = 100;
		DebugLog("OnInitDialog: DEBUG 모드 - 더미값으로 진행\n");
	}
#endif
	ParseCommandLine();

	// 저장된 심볼경로 로드
	{
		char exeDir[MAX_PATH] = {};
		GetModuleFileNameA(NULL, exeDir, MAX_PATH);
		char* sl = strrchr(exeDir, '\\'); if (sl) *sl = '\0';
		char iniPath[MAX_PATH] = {};
		sprintf_s(iniPath, "%s\\AxisAgent.ini", exeDir);
		char buf[MAX_PATH] = {};
		GetPrivateProfileStringA("Symbol", "Path", "", buf, MAX_PATH, iniPath);
		m_sSymPath = buf;
		m_editSymPath.SetWindowText(m_sSymPath);
	}

	LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	style |= WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION;
	SetWindowLong(m_hWnd, GWL_STYLE, style);

	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	// 유효성 체크
	if (m_parentPid == 0 || m_hParentWnd == NULL || strlen(m_regkey) == 0)
	{
		DebugLog("OnInitDialog: 커맨드라인 파싱 실패 - 종료\n");
		PostMessage(WM_CLOSE);
		return FALSE;
	}

	// 부모 프로세스 핸들
	m_hParent = OpenProcess(SYNCHRONIZE, FALSE, m_parentPid);
	if (!m_hParent)
	{
		DebugLog("OnInitDialog: 부모 프로세스 없음 - 종료\n");
		PostMessage(WM_CLOSE);
		return FALSE;
	}





	// Named Event 열기
	char eventName[64] = { 0 };
	sprintf_s(eventName, "PingStop_%lu", m_parentPid);
	m_hStopEvent = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, eventName);
	if (m_hStopEvent)
		DebugLog("OnInitDialog: StopEvent 열기 성공 [%s]\n", eventName);
	else
		DebugLog("OnInitDialog: StopEvent 없음\n");

	// 윈도우 타이틀 → "agent_MyRegKey" 로 설정 (FindWindow 용)
	char caption[256] = { 0 };
	sprintf_s(caption, "AxisAgent_%s", m_regkey);
	SetWindowText(caption);
	DebugLog("OnInitDialog: SetWindowText [%s]\n", caption);

	// 로그 파일 경로 설정
	SYSTEMTIME st;
	GetLocalTime(&st);

	char selfPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, selfPath, MAX_PATH);
	char folder[MAX_PATH] = { 0 };
	strcpy_s(folder, selfPath);
	char* lastSlash = strrchr(folder, '\\');
	if (lastSlash) *lastSlash = '\0';

	char pingFolder[MAX_PATH] = { 0 };
	sprintf_s(pingFolder, "%s\\ping", folder);
	if (GetFileAttributesA(pingFolder) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(pingFolder, NULL);
		DebugLog("OnInitDialog: ping 폴더 생성 [%s]\n", pingFolder);
	}

	sprintf_s(m_logFile, MAX_PATH, "%s\\pinglog_%04d%02d%02d.txt",
		pingFolder, st.wYear, st.wMonth, st.wDay);
	DebugLog("OnInitDialog: 로그 파일 [%s]\n", m_logFile);

	char exePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, exePath, MAX_PATH);

	char* p = strrchr(exePath, '\\');
	if (p) *p = '\0';

	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	BOOL bOk = SymInitialize(GetCurrentProcess(), NULL, TRUE);

	// 네트워크 타입 로그
	char netLog[256] = { 0 };
	switch (GetCurrentNetType())
	{
	case NET_WIFI:  sprintf_s(netLog, "[시작] 네트워크: WiFi\n");  break;
	case NET_WIRED: sprintf_s(netLog, "[시작] 네트워크: 유선\n");  break;
	case NET_NONE:  sprintf_s(netLog, "[시작] 네트워크: 없음\n");  break;
	}
	WriteLog(netLog);
	//SendToParent(netLog, AGENT_MSG_NETTYPE);

	m_bLastPingSuccess = true;
	m_bPingStateInit = false;
	m_lastPingSummaryTick = GetTickCount();
	m_pingOkCount = 0;
	m_pingFailCount = 0;
	// ping 스레드 시작
	StartPingThread();

	// 실제 모니터 스레드 시작

	m_lastMonitorSummaryTick = GetTickCount();
	m_bLastHung = false;
	m_lastNoResponseCount = 0;
	m_hMonitorThread = CreateThread(
		NULL, 0, MonitorThreadProc, (LPVOID)this, 0, NULL);

	if (m_hMonitorThread)
		DebugLog("OnInitDialog: MonitorThread 시작\n");
	else
		DebugLog("OnInitDialog: MonitorThread CreateThread 실패 err=%lu\n", GetLastError());

	RegisterTrayIcon();
	RegisterToMain();
	return TRUE;
}

void CAxisAgentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_MINIMIZE)
		{
			ShowWindow(SW_HIDE);
			return;
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CAxisAgentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAxisAgentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 컨트롤러에서 해당 개체 중 하나를 계속 사용하고 있을 경우
//  사용자가 UI를 닫을 때 자동화 서버를 종료하면 안 됩니다.  이들
//  메시지 처리기는 프록시가 아직 사용 중인 경우 UI는 숨기지만,
//  UI가 표시되지 않아도 대화 상자는
//  남겨 둡니다.

void CAxisAgentDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CAxisAgentDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CAxisAgentDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CAxisAgentDlg::CanExit()
{
	// 프록시 개체가 계속 남아 있으면 자동화 컨트롤러에서는
	//  이 애플리케이션을 계속 사용합니다.  대화 상자는 남겨 두지만
	//  해당 UI는 숨깁니다.
	//if (m_pAutoProxy != nullptr)
	//{
	//	ShowWindow(SW_HIDE);
	//	return FALSE;
	//}
	this->RemoveTrayIcon();
	return TRUE;
}

void CAxisAgentDlg::ParseCommandLine()
{
	int argc = 0;
	LPWSTR* argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (!argvW) return;

	char output[2048] = { 0 };

	CString slog;
	for (int i = 1; i < argc; i++)
	{
		if (wcscmp(argvW[i], L"/p") == 0 && i + 1 < argc)
		{
			m_parentPid = (DWORD)_wtoi(argvW[++i]);
			memset(output, sizeof(output), 0x00);
			sprintf_s(output, sizeof(output),
				"[m_parentPid = %d]\n",
				m_parentPid);
			AddLog("MONITOR", output);
		}

		else if (wcscmp(argvW[i], L"/h") == 0 && i + 1 < argc)
			m_hParentWnd = (HWND)(UINT64)_wtoi64(argvW[++i]);

		else if (wcscmp(argvW[i], L"/n") == 0 && i + 1 < argc)
		{
			WideCharToMultiByte(CP_ACP, 0, argvW[++i], -1,
				m_regkey, sizeof(m_regkey), NULL, NULL);
			memset(output, sizeof(output), 0x00);
			sprintf_s(output, sizeof(output),
				"[m_regkey = %s]\n",
				m_regkey);
			AddLog("MONITOR", output);
		}

		else if (wcscmp(argvW[i], L"/v") == 0 && i + 1 < argc) // ← 수정
		{
			int bShow = _wtoi(argvW[++i]); // ← ++i 추가
			if (bShow)
				m_bShow = TRUE;
			else
				m_bShow = FALSE;
		}

		else if (wcscmp(argvW[i], L"/x") == 0 && i + 1 < argc)
			m_startX = _wtoi(argvW[++i]);

		else if (wcscmp(argvW[i], L"/y") == 0 && i + 1 < argc)
			m_startY = _wtoi(argvW[++i]);

		else if (wcscmp(argvW[i], L"/t") == 0 && i + 1 < argc)
		{
			m_dwMainThreadId = (DWORD)_wtoi(argvW[++i]);
			memset(output, sizeof(output), 0x00);
			sprintf_s(output, sizeof(output),
				"[m_dwMainThreadId = %d]\n",
				m_dwMainThreadId);
			AddLog("MONITOR", output);
		}

		else if (wcscmp(argvW[i], L"/d") == 0 && i + 1 < argc)
		{
			WideCharToMultiByte(CP_ACP, 0, argvW[++i], -1,
				m_sUserpath, sizeof(m_sUserpath), NULL, NULL);
			memset(output, sizeof(output), 0x00);
			sprintf_s(output, sizeof(output),
				"[m_sUserpath = %s]\n",
				m_sUserpath);
			AddLog("MONITOR", output);
		}
	}

	LocalFree(argvW);
	slog.Format("ParseCommandLine: PID=%lu HWND=%llu TID=%lu KEY=%s show=%d\n",
		m_parentPid, (UINT64)m_hParentWnd, m_dwMainThreadId, m_regkey, m_bShow);
	WriteMonitorLog(slog);

	DebugLog("OnInitDialog: MonitorThread 시작\n");

#ifndef _DEBUG
	SetTimer(9898, 100, nullptr);
#endif
}

void CAxisAgentDlg::WriteLog(const char* msg)
{
	FILE* fp = nullptr;
	fopen_s(&fp, m_logFile, "a");
	if (fp)
	{
		fputs(msg, fp);
		fclose(fp);
	}
}

void CAxisAgentDlg::SendToParent(const char* msg, int nKind)
{
	if (!m_hParentWnd)
	{
		WriteMonitorLog("[S2] m_hParentWnd NULL");
		return;
	}

	if (!::IsWindow(m_hParentWnd))
	{
		WriteMonitorLog("[S3] 부모 윈도우 invalid");
		return;
	}

	COPYDATASTRUCT cds = {};
	cds.dwData = (ULONG_PTR)nKind;
	cds.cbData = (DWORD)strlen(msg) + 1;
	cds.lpData = (PVOID)msg;



	DWORD_PTR result = 0;
	LRESULT lr = SendMessageTimeout(
		m_hParentWnd,
		WM_COPYDATA,
		(WPARAM)m_hWnd,
		(LPARAM)&cds,
		SMTO_ABORTIFHUNG | SMTO_BLOCK,
		3000,
		&result);

	if (lr == 0)
	{
		char buf[128] = { 0 };
		sprintf_s(buf, "[S5] SendToParent 실패 err=%lu", GetLastError());
		WriteMonitorLog(buf);
	}
	else
	{

	}
}

BOOL CAxisAgentDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCds)
{
	if (!pCds) return FALSE;

	switch (pCds->dwData)
	{
		case 1: // ping 관련 명령
		{
			char* pMsg = (char*)pCds->lpData;
			DebugLog("OnCopyData: 수신 [%s]\n", pMsg);
			// 추후 명령 처리
		}
		break;
	}
	return TRUE;
}

void CAxisAgentDlg::OnDestroy()
{
	DebugLog("OnDestroy: 종료 처리\n");

	// StopEvent 먼저 세팅해서 각 루프가 종료 조건을 보게 함
	if (m_hStopEvent)
	{
		SetEvent(m_hStopEvent);
	}

	// ping 프로세스 먼저 종료
	if (m_hPingProcess)
	{
		TerminateProcess(m_hPingProcess, 0);
	}

	// pipe 닫아서 ReadFile 블로킹을 깨움
	if (m_hPingPipeRead)
	{
		CloseHandle(m_hPingPipeRead);
		m_hPingPipeRead = NULL;
	}

	if (m_hPingPipeWrite)
	{
		CloseHandle(m_hPingPipeWrite);
		m_hPingPipeWrite = NULL;
	}

	// 모니터 스레드 종료 대기
	if (m_hMonitorThread)
	{
		WaitForSingleObject(m_hMonitorThread, 3000);
		CloseHandle(m_hMonitorThread);
		m_hMonitorThread = NULL;
	}

	// ping 스레드 종료 대기
	if (m_hPingThread)
	{
		WaitForSingleObject(m_hPingThread, 3000);
		CloseHandle(m_hPingThread);
		m_hPingThread = NULL;
	}

	// ping 프로세스 핸들 정리
	if (m_hPingProcess)
	{
		CloseHandle(m_hPingProcess);
		m_hPingProcess = NULL;
	}

	// StopEvent 핸들 정리
	if (m_hStopEvent)
	{
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}

	// 부모 프로세스 핸들 정리
	if (m_hParent)
	{
		CloseHandle(m_hParent);
		m_hParent = NULL;
	}

	CDialogEx::OnDestroy();
}
CAxisAgentDlg::NetType CAxisAgentDlg::GetCurrentNetType()
{
	ULONG size = 0;
	GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_GATEWAYS, NULL, NULL, &size);
	if (size == 0)
	{
		DebugLog("GetCurrentNetType: 어댑터 없음\n");
		return NET_NONE;
	}

	IP_ADAPTER_ADDRESSES* addresses = (IP_ADAPTER_ADDRESSES*)malloc(size);
	if (!addresses)
	{
		DebugLog("GetCurrentNetType: malloc 실패\n");
		return NET_NONE;
	}

	NetType result = NET_NONE;
	if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_GATEWAYS,
		NULL, addresses, &size) == NO_ERROR)
	{
		for (IP_ADAPTER_ADDRESSES* addr = addresses; addr; addr = addr->Next)
		{
			if (addr->OperStatus != IfOperStatusUp) continue;
			if (addr->FirstGatewayAddress == NULL)  continue;
			if (addr->IfType == IF_TYPE_SOFTWARE_LOOPBACK) continue;
			if (addr->IfType == IF_TYPE_TUNNEL) continue;

			if (addr->IfType == IF_TYPE_IEEE80211)
			{
				DebugLog("GetCurrentNetType: WiFi 감지\n");
				result = NET_WIFI;
				break;
			}
			else
			{
				DebugLog("GetCurrentNetType: 유선 감지\n");
				result = NET_WIRED;
			}
		}
	}
	free(addresses);
	return result;
}

bool CAxisAgentDlg::ShouldStop()
{
	if (m_bShow)
		return false;
	// 부모 프로세스 죽었는지
	if (m_hParent)
	{
		if (WaitForSingleObject(m_hParent, 0) == WAIT_OBJECT_0)
		{
			DebugLog("ShouldStop: 부모 프로세스 종료 감지\n");
			return true;
		}
	}

	// StopEvent 신호 왔는지
	if (m_hStopEvent)
	{
		if (WaitForSingleObject(m_hStopEvent, 0) == WAIT_OBJECT_0)
		{
			DebugLog("ShouldStop: StopEvent 신호 감지\n");
			return true;
		}
	}

	return false;
}

// 스레드 진입점 (static)
DWORD WINAPI CAxisAgentDlg::PingThreadProc(LPVOID pParam)
{
	CAxisAgentDlg* pDlg = (CAxisAgentDlg*)pParam;
	pDlg->PingLoop();
	return 0;
}

bool CAxisAgentDlg::IsPingSuccessLine(const char* line)
{
	if (!line || !line[0])
		return false;

	// 한글/영문 ping 출력 둘 다 대충 커버
	if (strstr(line, "TTL=") != NULL) return true;
	if (strstr(line, "ttl=") != NULL) return true;
	if (strstr(line, "시간=") != NULL) return true;
	if (strstr(line, "bytes=") != NULL) return true;

	return false;
}

void CAxisAgentDlg::PingLoop()
{
	DebugLog("PingLoop 시작\n");

	SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };

	// 혹시 이전 값이 남아있으면 초기화
	m_hPingPipeRead = NULL;
	m_hPingPipeWrite = NULL;
	m_hPingProcess = NULL;

	if (!CreatePipe(&m_hPingPipeRead, &m_hPingPipeWrite, &sa, 0))
	{
		DebugLog("PingLoop: CreatePipe 실패 err=%lu\n", GetLastError());
		return;
	}

	SetHandleInformation(m_hPingPipeRead, HANDLE_FLAG_INHERIT, 0);

	STARTUPINFOA si = { sizeof(si) };
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = m_hPingPipeWrite;
	si.hStdError = m_hPingPipeWrite;

	PROCESS_INFORMATION pi = { 0 };
	char cmd[] = "ping 8.8.8.8 -t";

	if (!CreateProcessA(
		NULL,
		cmd,
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi))
	{
		DebugLog("PingLoop: ping CreateProcess 실패 err=%lu\n", GetLastError());

		if (m_hPingPipeRead)
		{
			CloseHandle(m_hPingPipeRead);
			m_hPingPipeRead = NULL;
		}

		if (m_hPingPipeWrite)
		{
			CloseHandle(m_hPingPipeWrite);
			m_hPingPipeWrite = NULL;
		}

		return;
	}

	m_hPingProcess = pi.hProcess;

	DebugLog("PingLoop: ping 프로세스 시작 PID=%lu\n", pi.dwProcessId);

	// 부모(Agent) 쪽에서는 write pipe 더 이상 안 씀
	if (m_hPingPipeWrite)
	{
		CloseHandle(m_hPingPipeWrite);
		m_hPingPipeWrite = NULL;
	}

	char buf[1024] = { 0 };
	char line[1024] = { 0 };
	int linePos = 0;
	DWORD bytesRead = 0;

	while (true)
	{
		// 종료 요청이 먼저 들어왔는지 확인
		if (ShouldStop())
		{
			WriteLog("[종료] 종료 신호 감지 - 자체 종료\n");
			DebugLog("PingLoop: while 시작 전 종료 감지\n");

			if (m_hPingProcess)
				TerminateProcess(m_hPingProcess, 0);

			goto CLEANUP;
		}

		BOOL bRead = ReadFile(m_hPingPipeRead, buf, sizeof(buf) - 1, &bytesRead, NULL);
		if (!bRead || bytesRead == 0)
			break;

		buf[bytesRead] = '\0';

		for (DWORD i = 0; i < bytesRead; i++)
		{
			char c = buf[i];
			if (c == '\r')
				continue;

			if (c == '\n')
			{
				line[linePos] = '\0';
				linePos = 0;

				if (strlen(line) == 0)
					continue;

				bool bPingOk = IsPingSuccessLine(line);

				if (bPingOk)
					m_pingOkCount++;
				else
					m_pingFailCount++;

				DWORD nowTick = GetTickCount();

				if (!m_bPingStateInit)
				{
					m_bLastPingSuccess = bPingOk;
					m_bPingStateInit = true;

					SYSTEMTIME st;
					GetLocalTime(&st);

					char output[2048] = { 0 };
					sprintf_s(output, sizeof(output),
						"[%04d-%02d-%02d %02d:%02d:%02d] ping 초기상태: %s | %s\n",
						st.wYear, st.wMonth, st.wDay,
						st.wHour, st.wMinute, st.wSecond,
						bPingOk ? "정상" : "실패",
						line);

					WriteLog(output);
					AddLog("PING", output);
					//SendToParent(output, AGENT_MSG_PING);
				}
				else
				{
					if (bPingOk != m_bLastPingSuccess)
					{
						SYSTEMTIME st;
						GetLocalTime(&st);

						char output[2048] = { 0 };
						sprintf_s(output, sizeof(output),
							"[%04d-%02d-%02d %02d:%02d:%02d] ping 상태 변경: %s -> %s | %s\n",
							st.wYear, st.wMonth, st.wDay,
							st.wHour, st.wMinute, st.wSecond,
							m_bLastPingSuccess ? "정상" : "실패",
							bPingOk ? "정상" : "실패",
							line);

						WriteLog(output);
						AddLog("PING", output);
						//SendToParent(output, AGENT_MSG_PING);

						m_bLastPingSuccess = bPingOk;
					}

					if (nowTick - m_lastPingSummaryTick >= 60000)
					{
						SYSTEMTIME st;
						GetLocalTime(&st);

						char output[2048] = { 0 };
						sprintf_s(output, sizeof(output),
							"[%04d-%02d-%02d %02d:%02d:%02d] ping 요약(1분): OK=%d FAIL=%d 현재=%s\n",
							st.wYear, st.wMonth, st.wDay,
							st.wHour, st.wMinute, st.wSecond,
							m_pingOkCount,
							m_pingFailCount,
							bPingOk ? "정상" : "실패");

						WriteLog(output);
						AddLog("PING", output);
						//SendToParent(output, AGENT_MSG_PING);

						m_pingOkCount = 0;
						m_pingFailCount = 0;
						m_lastPingSummaryTick = nowTick;
					}
				}

				if (ShouldStop())
				{
					WriteLog("[종료] 종료 신호 감지 - 자체 종료\n");
					DebugLog("PingLoop: 종료 신호 - ping 종료\n");

					if (m_hPingProcess)
						TerminateProcess(m_hPingProcess, 0);

					goto CLEANUP;
				}
			}
			else
			{
				if (linePos < (int)sizeof(line) - 1)
					line[linePos++] = c;
			}
		}
	}

	DebugLog("PingLoop: ReadFile 종료\n");

CLEANUP:
	if (pi.hThread)
	{
		CloseHandle(pi.hThread);
		pi.hThread = NULL;
	}

	if (m_hPingPipeRead)
	{
		CloseHandle(m_hPingPipeRead);
		m_hPingPipeRead = NULL;
	}

	if (m_hPingPipeWrite)
	{
		CloseHandle(m_hPingPipeWrite);
		m_hPingPipeWrite = NULL;
	}

	if (m_hPingProcess)
	{
		CloseHandle(m_hPingProcess);
		m_hPingProcess = NULL;
	}

	DebugLog("PingLoop 종료\n");

	// 스레드 종료 후 다이얼로그 종료
	PostMessage(WM_CLOSE);
}

void CAxisAgentDlg::StartPingThread()
{
	DebugLog("StartPingThread\n");
	m_hPingThread = CreateThread(
		NULL, 0,
		PingThreadProc,
		(LPVOID)this, // this 포인터 전달
		0, NULL);

	if (!m_hPingThread)
		DebugLog("StartPingThread: CreateThread 실패 err=%d\n", GetLastError());
}

// CPU 사용률 계산
float CAxisAgentDlg::CalcProcessCpuUsage(
	FILETIME& prevKernel, FILETIME& prevUser,
	FILETIME  curKernel, FILETIME  curUser,
	ULONGLONG& prevTickMs,
	ULONGLONG  curTickMs)
{
	const ULONGLONG prevTotal =
		FileTimeToULL(prevKernel) + FileTimeToULL(prevUser);

	const ULONGLONG curTotal =
		FileTimeToULL(curKernel) + FileTimeToULL(curUser);

	if (curTotal < prevTotal || curTickMs <= prevTickMs)
	{
		prevKernel = curKernel;
		prevUser = curUser;
		prevTickMs = curTickMs;
		return 0.0f;
	}

	const ULONGLONG cpuTime100ns = curTotal - prevTotal;
	const ULONGLONG elapsedMs = curTickMs - prevTickMs;
	const double elapsed100ns = static_cast<double>(elapsedMs) * 10000.0;

	SYSTEM_INFO si = {};
	GetSystemInfo(&si);
	const DWORD cpuCount = (si.dwNumberOfProcessors > 0) ? si.dwNumberOfProcessors : 1;

	float fCpu = 0.0f;
	if (elapsed100ns > 0.0)
	{
		fCpu = static_cast<float>(
			(static_cast<double>(cpuTime100ns) / elapsed100ns)
			* 100.0
			/ static_cast<double>(cpuCount));
	}

	prevKernel = curKernel;
	prevUser = curUser;
	prevTickMs = curTickMs;

	if (fCpu < 0.0f) fCpu = 0.0f;
	return fCpu;
}

float CAxisAgentDlg::CalcThreadCpuUsage(
	FILETIME& prevKernel, FILETIME& prevUser,
	FILETIME  curKernel, FILETIME  curUser,
	ULONGLONG& prevTickMs,
	ULONGLONG  curTickMs)
{
	const ULONGLONG prevTotal =
		FileTimeToULL(prevKernel) + FileTimeToULL(prevUser);

	const ULONGLONG curTotal =
		FileTimeToULL(curKernel) + FileTimeToULL(curUser);

	if (curTotal < prevTotal || curTickMs <= prevTickMs)
	{
		prevKernel = curKernel;
		prevUser = curUser;
		prevTickMs = curTickMs;
		return 0.0f;
	}

	const ULONGLONG cpuTime100ns = curTotal - prevTotal;
	const ULONGLONG elapsedMs = curTickMs - prevTickMs;
	const double elapsed100ns = static_cast<double>(elapsedMs) * 10000.0;

	float fCpu = 0.0f;
	if (elapsed100ns > 0.0)
	{
		fCpu = static_cast<float>(
			(static_cast<double>(cpuTime100ns) / elapsed100ns) * 100.0);
	}

	prevKernel = curKernel;
	prevUser = curUser;
	prevTickMs = curTickMs;

	if (fCpu < 0.0f) fCpu = 0.0f;
	if (fCpu > 100.0f) fCpu = 100.0f;
	return fCpu;
}

// 모니터링 로그 저장
void CAxisAgentDlg::WriteMonitorLog(const char* msg)
{
#ifdef AGENT_LOG_MONITOR
		SYSTEMTIME st;
		GetLocalTime(&st);

		char logPath[MAX_PATH] = { 0 };
		char selfPath[MAX_PATH] = { 0 };
		GetModuleFileNameA(NULL, selfPath, MAX_PATH);
		char* lastSlash = strrchr(selfPath, '\\');
		if (lastSlash) *lastSlash = '\0';

		sprintf_s(logPath, "%s\\ping\\monitorlog_%04d%02d%02d.txt",
			selfPath, st.wYear, st.wMonth, st.wDay);

		char output[2048] = { 0 };
		sprintf_s(output, "[AxisAgent][%04d-%02d-%02d %02d:%02d:%02d][AxisAgent] %s\n",
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond,
			msg);
		OutputDebugStringA(output);
		FILE* fp = nullptr;
		fopen_s(&fp, logPath, "a");
		if (fp)
		{
			fputs(output, fp);
			fclose(fp);
		}
#endif

	AddLog("MONITOR", msg);   //testcmt
}

void CAxisAgentDlg::CreateDump(const char* reason)
{
	if (m_dumpCount >= MAX_DUMP_COUNT)
	{
		WriteMonitorLog("[D0] dump skipped: max dump count reached");
		return;
	}
	const ULONGLONG nowTick = GetTickCount64();
	if (m_lastDumpTick != 0 && (nowTick - m_lastDumpTick) < DUMP_INTERVAL_MS)
	{
		char skipMsg[256] = { 0 };
		sprintf_s(skipMsg,
			"[D0] dump skipped: interval not reached diff=%llu",
			(nowTick - m_lastDumpTick));
		WriteMonitorLog(skipMsg);
		return;
	}
	const int seq = ++m_dumpSeq;
	SYSTEMTIME st;
	GetLocalTime(&st);
	char selfPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, selfPath, MAX_PATH);
	char* lastSlash = strrchr(selfPath, '\\');
	if (lastSlash) *lastSlash = '\0';
	char dumpDir[MAX_PATH] = { 0 };
	sprintf_s(dumpDir, "%s\\ping", selfPath);
	CreateDirectoryA(dumpDir, NULL);
	char dumpPath[MAX_PATH] = { 0 };
	sprintf_s(dumpPath,
		"%s\\dump_%04d%02d%02d_%02d%02d%02d_%03d_pid%lu_seq%02d.dmp",
		dumpDir,
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		m_parentPid,
		seq);

	// =============================================
	//  추가: logMsg 를 여기서 미리 선언 (이후 전체에서 재사용)
	// =============================================
	char logMsg[1024] = { 0 };

	sprintf_s(logMsg,
		"[D1] dump start seq=%d reason=%s path=%s pid=%lu",
		seq,
		reason ? reason : "(null)",
		dumpPath,
		m_parentPid);
	WriteMonitorLog(logMsg);

	// =============================================
	//  추가: SeDebugPrivilege 활성화
	//    다른 PC에서 권한이 없어 덤프 크기 0 되는 주요 원인
	// =============================================
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		TOKEN_PRIVILEGES tp = {};
		if (LookupPrivilegeValueA(NULL, "SeDebugPrivilege", &tp.Privileges[0].Luid))
		{
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
			sprintf_s(logMsg, "[D1-P] SeDebugPrivilege adjust err=%lu", GetLastError());
			WriteMonitorLog(logMsg);
		}
		else
		{
			sprintf_s(logMsg, "[D1-P] LookupPrivilegeValue fail err=%lu", GetLastError());
			WriteMonitorLog(logMsg);
		}
		CloseHandle(hToken);
	}
	else
	{
		sprintf_s(logMsg, "[D1-P] OpenProcessToken fail err=%lu", GetLastError());
		WriteMonitorLog(logMsg);
	}

	DWORD access =
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ |
		PROCESS_DUP_HANDLE |
		SYNCHRONIZE;

	HANDLE hProcess = OpenProcess(access, FALSE, m_parentPid);
	if (!hProcess)
	{
		// =============================================
		// 추가: 실패 시 PROCESS_ALL_ACCESS 로 재시도
		// =============================================
		sprintf_s(logMsg, "[D2] seq=%d OpenProcess(limited) fail err=%lu, retrying ALL_ACCESS", seq, GetLastError());
		WriteMonitorLog(logMsg);

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_parentPid);
		if (!hProcess)
		{
			sprintf_s(logMsg, "[D2] seq=%d OpenProcess(ALL_ACCESS) fail err=%lu", seq, GetLastError());
			WriteMonitorLog(logMsg);
			return;
		}
		sprintf_s(logMsg, "[D2] seq=%d OpenProcess(ALL_ACCESS) success", seq);
		WriteMonitorLog(logMsg);
	}

	if (WaitForSingleObject(hProcess, 0) == WAIT_OBJECT_0)
	{
		sprintf_s(logMsg, "[D3] seq=%d target already exited", seq);
		WriteMonitorLog(logMsg);
		CloseHandle(hProcess);
		return;
	}

	HANDLE hFile = CreateFileA(
		dumpPath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		sprintf_s(logMsg, "[D4] seq=%d CreateFile fail err=%lu", seq, GetLastError());
		WriteMonitorLog(logMsg);
		CloseHandle(hProcess);
		return;
	}

	// =============================================
	//  변경: MiniDumpNormal → 추가 플래그 포함
	//    WithHandleData, WithThreadInfo 추가
	// =============================================
	MINIDUMP_TYPE dumpType = (MINIDUMP_TYPE)(
		MiniDumpNormal |
		MiniDumpWithHandleData |
		MiniDumpWithThreadInfo |
		MiniDumpWithIndirectlyReferencedMemory |
		MiniDumpWithDataSegs |
		MiniDumpWithProcessThreadData |
		MiniDumpWithCodeSegs          // x64 스택 언와인드 테이블 포함 (CALLER 복원에 필수)
		);

	SetLastError(0);
	BOOL bOk = MiniDumpWriteDump(
		hProcess,
		m_parentPid,
		hFile,
		dumpType,		// ← 기존: MiniDumpNormal
		NULL,
		NULL,
		NULL);

	DWORD dwErr = GetLastError();

	// =============================================
	//  추가: 실패 시 에러코드 hex 로 즉시 로그
	// =============================================
	if (!bOk)
	{
		sprintf_s(logMsg, "[D5-FAIL] seq=%d MiniDumpWriteDump failed err=%lu(0x%08lX)", seq, dwErr, dwErr);
		WriteMonitorLog(logMsg);
	}

	FlushFileBuffers(hFile);

	LARGE_INTEGER fileSize = {};
	GetFileSizeEx(hFile, &fileSize);
	CloseHandle(hFile);
	CloseHandle(hProcess);

	// =============================================
	//  변경: err 를 hex 병기로 출력 (에러 원인 파악 용이)
	// =============================================
	sprintf_s(logMsg,
		"[D5] seq=%d MiniDumpWriteDump ok=%d err=%lu(0x%08lX) size=%lld",
		seq, (int)bOk, dwErr, dwErr, fileSize.QuadPart);
	WriteMonitorLog(logMsg);

	if (bOk && fileSize.QuadPart > 0)
	{
		m_dumpCount++;
		m_lastDumpTick = nowTick;
		sprintf_s(logMsg,
			"[D6] dump success seq=%d count=%d",
			seq, m_dumpCount);
		WriteMonitorLog(logMsg);
		AnalyzeDump(dumpPath);
	}
	else
	{
		sprintf_s(logMsg, "[D7] dump failed seq=%d", seq);
		WriteMonitorLog(logMsg);
	}
}

// 스레드 진입점
DWORD WINAPI CAxisAgentDlg::MonitorThreadProc(LPVOID pParam)
{
	CAxisAgentDlg* pDlg = (CAxisAgentDlg*)pParam;
	pDlg->MonitorLoop();
	return 0;
}

void CAxisAgentDlg::MonitorLoop()
{
	WriteMonitorLog("MonitorLoop 시작");

	HANDLE hProcess = OpenProcess(
		PROCESS_QUERY_INFORMATION | SYNCHRONIZE,
		FALSE, m_parentPid);

	if (!hProcess)
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "OpenProcess 실패 err=%lu pid=%lu", GetLastError(), m_parentPid);
		WriteMonitorLog(buf);
		return;
	}

	HANDLE hMainThread = OpenThread(
		THREAD_QUERY_INFORMATION,
		FALSE, m_dwMainThreadId);

	if (!hMainThread)
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "OpenThread 실패 err=%lu tid=%lu", GetLastError(), m_dwMainThreadId);
		WriteMonitorLog(buf);
	}

	FILETIME ftPrevProcKernel = {}, ftPrevProcUser = {};
	FILETIME ftPrevMainKernel = {}, ftPrevMainUser = {};
	FILETIME ftDummy = {};

	ULONGLONG prevProcTickMs = GetTickCount64();
	ULONGLONG prevMainTickMs = prevProcTickMs;

	if (!GetProcessTimes(hProcess, &ftDummy, &ftDummy, &ftPrevProcKernel, &ftPrevProcUser))
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "초기 GetProcessTimes 실패 err=%lu", GetLastError());
		WriteMonitorLog(buf);

		if (hMainThread)
			CloseHandle(hMainThread);
		CloseHandle(hProcess);
		return;
	}

	if (hMainThread)
	{
		if (!GetThreadTimes(hMainThread, &ftDummy, &ftDummy, &ftPrevMainKernel, &ftPrevMainUser))
		{
			char buf[256] = { 0 };
			sprintf_s(buf, "초기 GetThreadTimes 실패 err=%lu", GetLastError());
			WriteMonitorLog(buf);
		}
	}

	int noResponseCount = 0;
	int cpuSpikeCount = 0;

	//bool bNoResponseDumpTried = false;
	//bool bCpuDumpTried = false;

	while (!ShouldStop())
	{
		Sleep(MONITOR_INTERVAL);


		if (WaitForSingleObject(hProcess, 0) == WAIT_OBJECT_0)
		{
			WriteMonitorLog("부모 프로세스 종료");
			break;
		}

		if (!::IsWindow(m_hParentWnd))
		{
			WriteMonitorLog("부모 윈도우 핸들 무효");
			break;
		}

		bool bHung = IsHungAppWindow(m_hParentWnd) ? true : false;

		DWORD_PTR result = 0;
		LRESULT lr = SendMessageTimeout(
			m_hParentWnd,
			WM_NULL,
			0,
			0,
			SMTO_ABORTIFHUNG | SMTO_BLOCK,
			THRESHOLD_HANG_MS,
			&result);

		if (lr == 0)
			++noResponseCount;
		else
			noResponseCount = 0;

		float fProcCpu = 0.0f;
		FILETIME ftCurProcKernel = {}, ftCurProcUser = {};
		if (GetProcessTimes(hProcess, &ftDummy, &ftDummy, &ftCurProcKernel, &ftCurProcUser))
		{
			ULONGLONG curProcTickMs = GetTickCount64();
			fProcCpu = CalcProcessCpuUsage(
				ftPrevProcKernel, ftPrevProcUser,
				ftCurProcKernel, ftCurProcUser,
				prevProcTickMs, curProcTickMs);
		}
		else
		{
			char buf[256] = { 0 };
			sprintf_s(buf, "GetProcessTimes 실패 err=%lu", GetLastError());
			WriteMonitorLog(buf);
		}

		float fMainCpu = 0.0f;
		if (hMainThread)
		{
			FILETIME ftCurMainKernel = {}, ftCurMainUser = {};
			if (GetThreadTimes(hMainThread, &ftDummy, &ftDummy, &ftCurMainKernel, &ftCurMainUser))
			{
				ULONGLONG curMainTickMs = GetTickCount64();
				fMainCpu = CalcThreadCpuUsage(
					ftPrevMainKernel, ftPrevMainUser,
					ftCurMainKernel, ftCurMainUser,
					prevMainTickMs, curMainTickMs);
			}
			else
			{
				char buf[256] = { 0 };
				sprintf_s(buf, "GetThreadTimes 실패 err=%lu", GetLastError());
				WriteMonitorLog(buf);
			}
		}

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		int nThreadCount = 0;
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			THREADENTRY32 te = {};
			te.dwSize = sizeof(te);

			if (Thread32First(hSnap, &te))
			{
				do
				{
					if (te.th32OwnerProcessID == m_parentPid)
						++nThreadCount;
				} while (Thread32Next(hSnap, &te));
			}
			CloseHandle(hSnap);
		}
		else
		{
			char buf[256] = { 0 };
			sprintf_s(buf, "CreateToolhelp32Snapshot 실패 err=%lu", GetLastError());
			WriteMonitorLog(buf);
		}


		DWORD nowTick = GetTickCount();

		// 상태 변화 시 즉시 로그
		if (bHung != m_bLastHung ||
			noResponseCount != m_lastNoResponseCount)
		{
			char logMsg[512] = { 0 };
			sprintf_s(logMsg,
				"[monitor][상태변화] CPU=%.1f MAIN=%.1f THR=%d HUNG=%d NORESP=%d",
				fProcCpu, fMainCpu, nThreadCount,
				bHung ? 1 : 0,
				noResponseCount);

			WriteMonitorLog(logMsg);

			m_bLastHung = bHung;
			m_lastNoResponseCount = noResponseCount;
		}

		// 1분 heartbeat 요약 로그
		if (nowTick - m_lastMonitorSummaryTick >= 60000)
		{
			char logMsg[512] = { 0 };
			sprintf_s(logMsg,
				"[monitor][1분요약] CPU=%.1f MAIN=%.1f THR=%d HUNG=%d NORESP=%d",
				fProcCpu, fMainCpu, nThreadCount,
				bHung ? 1 : 0,
				noResponseCount);

			WriteMonitorLog(logMsg);

			m_lastMonitorSummaryTick = nowTick;
		}

		char sendMsg[256] = { 0 };
		sprintf_s(sendMsg,
			"CPU=%.1f MAIN=%.1f THR=%d HUNG=%d NORESP=%d",
			fProcCpu, fMainCpu, nThreadCount, bHung ? 1 : 0, noResponseCount);
		//SendToParent(sendMsg, AGENT_MSG_MONITOR);

		if ((bHung && noResponseCount >= 2) || noResponseCount >= 3)
		{
			WriteMonitorLog("[M4] 응답없음 판정 진입");
		}

		if (fMainCpu > THRESHOLD_CPU)
		{
			WriteMonitorLog("[M5] CPU 임계치 초과");
			++cpuSpikeCount;
		}
		else
		{
			cpuSpikeCount = 0;
		}

		if (noResponseCount >= 2)
		{
			char buf[256] = { 0 };
			sprintf_s(buf,
				"[M8] noresponse trigger noResp=%d hung=%d mainCpu=%.1f procCpu=%.1f",
				noResponseCount, bHung ? 1 : 0, fMainCpu, fProcCpu);
			WriteMonitorLog(buf);

			CreateDump("NoResponse");
		}

		if (cpuSpikeCount >= 3)
		{
			char buf[256] = { 0 };
			sprintf_s(buf,
				"[M9] cpuspike trigger cpuSpikeCount=%d hung=%d mainCpu=%.1f procCpu=%.1f",
				cpuSpikeCount, bHung ? 1 : 0, fMainCpu, fProcCpu);
			WriteMonitorLog(buf);

			CreateDump("CpuSpike");
		}

	}

	if (hMainThread)
		CloseHandle(hMainThread);

	CloseHandle(hProcess);
}

#include <random>
BOOL WriteRandomEncryptedFile(const CString& filePath)
{
	CStdioFile file;

	if (!file.Open(filePath,
		CFile::modeCreate |
		CFile::modeWrite |
		CFile::typeText))
	{
		return FALSE;
	}

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> digitDist(4, 6);

	const int count = 20;

	for (int i = 0; i < count; ++i)
	{
		int digits = digitDist(gen);

		int minValue = 1;
		int maxValue = 9;

		for (int j = 1; j < digits; ++j)
		{
			minValue *= 10;
			maxValue = maxValue * 10 + 9;
		}

		std::uniform_int_distribution<> numDist(minValue, maxValue);
		int value = numDist(gen);

		CStringA plainA;
		plainA.Format("%d", value);

		CString enc = Aes256EncryptBase64(plainA);

		CString line;
		line.Format(_T("%S = %s\r\n"), plainA.GetString(), enc.GetString());

		file.WriteString(line);
	}

	file.Close();

	return TRUE;
}

void CAxisAgentDlg::OnBnClickedBtnTest()
{
	CString enc = Aes256EncryptBase64("0423");
	// 결과: po6r90ZkUMxeEJzecHCFBg==
	AfxMessageBox(enc);
	CString dec = Aes256DecryptBase64(enc);
	//WriteRandomEncryptedFile(_T("C:\\Temp\\aes_random.txt"));
	AfxMessageBox(dec);
}

void CAxisAgentDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case 9898:
		{
			KillTimer(nIDEvent);
			if (m_bShow)
			{
				SetWindowPos(&wndTopMost,
					m_startX, m_startY,
					500, 300,
					SWP_FRAMECHANGED | SWP_SHOWWINDOW);
			}
			else
				ShowWindow(SW_HIDE);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CAxisAgentDlg::AnalyzeDump(const char* dumpPath)
{
	if (!dumpPath || !dumpPath[0])
		return;

	WriteMonitorLog("--------------------------------");
	WriteMonitorLog("덤프 분석 시작");

	HANDLE hFile = CreateFileA(
		dumpPath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "덤프 분석 실패 CreateFile err=%lu", GetLastError());
		WriteMonitorLog(buf);
		return;
	}

	HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (!hMapping)
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "덤프 분석 실패 CreateFileMapping err=%lu", GetLastError());
		WriteMonitorLog(buf);
		CloseHandle(hFile);
		return;
	}

	BYTE* pView = (BYTE*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (!pView)
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "덤프 분석 실패 MapViewOfFile err=%lu", GetLastError());
		WriteMonitorLog(buf);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}

	HANDLE hSymProcess = GetCurrentProcess();

	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);

	// 심볼 경로: 사용자 지정 + AxisAgent.exe 위치 + 덤프 파일 위치
	char symPath[MAX_PATH * 4] = {};
	{
		char exeDir[MAX_PATH] = {};
		GetModuleFileNameA(NULL, exeDir, MAX_PATH);
		char* sl = strrchr(exeDir, '\\'); if (sl) *sl = '\0';

		char dumpDir[MAX_PATH] = {};
		strcpy_s(dumpDir, dumpPath);
		char* sl2 = strrchr(dumpDir, '\\'); if (sl2) *sl2 = '\0';

		CStringA userSym(m_sSymPath);
		if (!userSym.IsEmpty())
			sprintf_s(symPath, "%s;%s;%s", (LPCSTR)userSym, exeDir, dumpDir);
		else
			sprintf_s(symPath, "%s;%s", exeDir, dumpDir);
	}

	if (!SymInitialize(hSymProcess, symPath, FALSE))
	{
		char buf[256] = { 0 };
		sprintf_s(buf, "SymInitialize 실패 err=%lu", GetLastError());
		WriteMonitorLog(buf);

		UnmapViewOfFile(pView);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}

	char logMsg[1024] = { 0 };

	PMINIDUMP_DIRECTORY pDir = NULL;
	PVOID pExcStream = NULL;
	ULONG excSize = 0;

	DWORD exceptionThreadId = 0;

	if (MiniDumpReadDumpStream(
		pView, ExceptionStream, &pDir, &pExcStream, &excSize) && pExcStream)
	{
		MINIDUMP_EXCEPTION_STREAM* pExc =
			(MINIDUMP_EXCEPTION_STREAM*)pExcStream;

		exceptionThreadId = pExc->ThreadId;

		DWORD excCode = pExc->ExceptionRecord.ExceptionCode;
		const char* excName = "UNKNOWN";
		switch (excCode)
		{
		case 0xC0000005: excName = "ACCESS_VIOLATION";        break;
		case 0xC0000094: excName = "INTEGER_DIVIDE_BY_ZERO";  break;
		case 0xC0000095: excName = "INTEGER_OVERFLOW";        break;
		case 0xC00000FD: excName = "STACK_OVERFLOW";          break;
		case 0xC000001D: excName = "ILLEGAL_INSTRUCTION";     break;
		case 0xC0000025: excName = "NONCONTINUABLE_EXCEPTION";break;
		case 0xC0000374: excName = "HEAP_CORRUPTION";         break;
		case 0xE06D7363: excName = "C++_EXCEPTION (throw)";  break;
		case 0x80000003: excName = "BREAKPOINT";              break;
		}

		sprintf_s(logMsg,
			"[AN1] ExceptionThread=%lu Code=0x%08lX (%s) Addr=0x%016I64X",
			pExc->ThreadId,
			excCode,
			excName,
			(DWORD64)pExc->ExceptionRecord.ExceptionAddress);
		WriteMonitorLog(logMsg);

		// ACCESS_VIOLATION: 읽기/쓰기 주소 추가 표시
		if (excCode == 0xC0000005 && pExc->ExceptionRecord.NumberParameters >= 2)
		{
			const char* rwStr = (pExc->ExceptionRecord.ExceptionInformation[0] == 1) ? "WRITE" : "READ";
			sprintf_s(logMsg, "[AN1] AV %s at 0x%016I64X",
				rwStr, (DWORD64)pExc->ExceptionRecord.ExceptionInformation[1]);
			WriteMonitorLog(logMsg);
		}
	}
	else
	{
		WriteMonitorLog("[AN1] ExceptionStream 없음");
	}

	DumpWalkContext walkCtx;
	ZeroMemory(&walkCtx, sizeof(walkCtx));
	walkCtx.pView = pView;

	PVOID pMemStream = NULL;
	ULONG memSize = 0;
	if (MiniDumpReadDumpStream(
		pView, MemoryListStream, &pDir, &pMemStream, &memSize) && pMemStream)
	{
		walkCtx.pMemList = (MINIDUMP_MEMORY_LIST*)pMemStream;
		WriteMonitorLog("[AN2] MemoryListStream 사용");
	}
	else
	{
		PVOID pMem64Stream = NULL;
		ULONG mem64Size = 0;
		if (MiniDumpReadDumpStream(
			pView, Memory64ListStream, &pDir, &pMem64Stream, &mem64Size) && pMem64Stream)
		{
			walkCtx.pMem64List = (MINIDUMP_MEMORY64_LIST*)pMem64Stream;
			WriteMonitorLog("[AN2] Memory64ListStream 사용");
		}
		else
		{
			WriteMonitorLog("[AN2] Memory stream 없음");
		}
	}

	PVOID pModStream = NULL;
	ULONG modSize = 0;
	if (MiniDumpReadDumpStream(
		pView, ModuleListStream, &pDir, &pModStream, &modSize) && pModStream)
	{
		MINIDUMP_MODULE_LIST* pModList =
			(MINIDUMP_MODULE_LIST*)pModStream;

		sprintf_s(logMsg, "[AN3] 모듈수=%lu", pModList->NumberOfModules);
		WriteMonitorLog(logMsg);

		for (ULONG i = 0; i < pModList->NumberOfModules; ++i)
		{
			MINIDUMP_MODULE& mod = pModList->Modules[i];
			std::string modName = DumpWideStringToAnsi(pView, mod.ModuleNameRva);

			SymLoadModuleEx(
				hSymProcess,
				NULL,
				modName.empty() ? NULL : modName.c_str(),
				NULL,
				mod.BaseOfImage,
				mod.SizeOfImage,
				NULL,
				0);
		}
	}
	else
	{
		WriteMonitorLog("[AN3] ModuleListStream 없음");
	}

	PVOID pThreadStream = NULL;
	ULONG threadSize = 0;
	if (!MiniDumpReadDumpStream(
		pView, ThreadListStream, &pDir, &pThreadStream, &threadSize) || !pThreadStream)
	{
		WriteMonitorLog("덤프 분석 실패 ThreadListStream 없음");
		SymCleanup(hSymProcess);
		UnmapViewOfFile(pView);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}

	MINIDUMP_THREAD_LIST* pThreadList =
		(MINIDUMP_THREAD_LIST*)pThreadStream;

	sprintf_s(logMsg, "[AN4] 스레드수=%lu", pThreadList->NumberOfThreads);
	WriteMonitorLog(logMsg);

	// 분석 대상 스레드 결정
	DWORD targetThreadId = 0;

	if (exceptionThreadId != 0)
	{
		targetThreadId = exceptionThreadId;
		sprintf_s(logMsg, "[AN5] 대상스레드=예외스레드 %lu", targetThreadId);
		WriteMonitorLog(logMsg);
	}
	else if (m_dwMainThreadId != 0)
	{
		targetThreadId = m_dwMainThreadId;
		sprintf_s(logMsg, "[AN5] 대상스레드=메인스레드 %lu", targetThreadId);
		WriteMonitorLog(logMsg);
	}
	else if (pThreadList->NumberOfThreads > 0)
	{
		targetThreadId = pThreadList->Threads[0].ThreadId;
		sprintf_s(logMsg, "[AN5] 대상스레드=첫번째스레드 %lu", targetThreadId);
		WriteMonitorLog(logMsg);
	}

	g_pDumpWalkCtx = &walkCtx;

	bool bFoundTarget = false;

	for (ULONG i = 0; i < pThreadList->NumberOfThreads; ++i)
	{
		MINIDUMP_THREAD& th = pThreadList->Threads[i];

		bool bTarget = (targetThreadId != 0 && th.ThreadId == targetThreadId);
		bool bException = (exceptionThreadId != 0 && th.ThreadId == exceptionThreadId);
		bool bMain = (m_dwMainThreadId != 0 && th.ThreadId == m_dwMainThreadId);

		if (!bTarget)
			continue;

		bFoundTarget = true;

		sprintf_s(logMsg,
			"=== ThreadID=%lu %s%s%s ===",
			th.ThreadId,
			bTarget ? "(분석대상)" : "",
			bMain ? "(메인스레드)" : "",
			bException ? "(예외스레드)" : "");
		WriteMonitorLog(logMsg);

		CONTEXT* pCtx = (CONTEXT*)(pView + th.ThreadContext.Rva);
		if (!pCtx)
		{
			WriteMonitorLog("  컨텍스트 없음");
			break;
		}

		STACKFRAME64 sf = {};
		CONTEXT ctxCopy = *pCtx;
		DWORD machineType = 0;

#if defined(_M_X64)
		machineType          = IMAGE_FILE_MACHINE_AMD64;
		sf.AddrPC.Offset     = pCtx->Rip;
		sf.AddrFrame.Offset  = pCtx->Rbp;
		sf.AddrStack.Offset  = pCtx->Rsp;
		sprintf_s(logMsg, "  RIP=0x%016I64X RSP=0x%016I64X", pCtx->Rip, pCtx->Rsp);
#elif defined(_M_IX86)
		machineType          = IMAGE_FILE_MACHINE_I386;
		sf.AddrPC.Offset     = pCtx->Eip;
		sf.AddrFrame.Offset  = pCtx->Ebp;
		sf.AddrStack.Offset  = pCtx->Esp;
		sprintf_s(logMsg, "  EIP=0x%08lX EBP=0x%08lX ESP=0x%08lX", pCtx->Eip, pCtx->Ebp, pCtx->Esp);
#else
		WriteMonitorLog("  지원하지 않는 아키텍처");
		break;
#endif
		sf.AddrPC.Mode    = AddrModeFlat;
		sf.AddrFrame.Mode = AddrModeFlat;
		sf.AddrStack.Mode = AddrModeFlat;
		WriteMonitorLog(logMsg);

		char mainF0[256] = {};
		char mainF1[256] = {};
		char mainF2[256] = {};
		char mainF3[256] = {};
		char mainF4[256] = {};

		const int MAX_MAIN_FRAMES = 30;

		for (int frameNo = 0; frameNo < MAX_MAIN_FRAMES; ++frameNo)
		{
			BOOL sw = StackWalk64(
				machineType,
				hSymProcess,
				NULL,
				&sf,
				&ctxCopy,
				DumpReadMemoryRoutine,
				SymFunctionTableAccess64,
				SymGetModuleBase64,
				NULL);

			if (!sw || sf.AddrPC.Offset == 0)
				break;

			char symBuf[sizeof(SYMBOL_INFO) + 256] = {};
			SYMBOL_INFO* pSym = (SYMBOL_INFO*)symBuf;
			pSym->SizeOfStruct = sizeof(SYMBOL_INFO);
			pSym->MaxNameLen   = 255;

			DWORD64 disp64 = 0;
			IMAGEHLP_LINE64 line = {};
			line.SizeOfStruct    = sizeof(line);
			DWORD lineDisp       = 0;

			const char* tag = (frameNo == 0) ? "TOP" : "   ";

			if (SymFromAddr(hSymProcess, sf.AddrPC.Offset, &disp64, pSym))
			{
				if (frameNo == 0) strcpy_s(mainF0, pSym->Name);
				else if (frameNo == 1) strcpy_s(mainF1, pSym->Name);
				else if (frameNo == 2) strcpy_s(mainF2, pSym->Name);
				else if (frameNo == 3) strcpy_s(mainF3, pSym->Name);
				else if (frameNo == 4) strcpy_s(mainF4, pSym->Name);

				if (SymGetLineFromAddr64(hSymProcess, sf.AddrPC.Offset, &lineDisp, &line))
					sprintf_s(logMsg, "[%s][%02d] %s + 0x%I64X (%s:%lu)",
						tag, frameNo, pSym->Name, disp64,
						line.FileName ? line.FileName : "?", line.LineNumber);
				else
					sprintf_s(logMsg, "[%s][%02d] %s + 0x%I64X",
						tag, frameNo, pSym->Name, disp64);
			}
			else
			{
				// PDB 없어도 모듈명 + 오프셋 표시
				char frameStr[256] = {};
				IMAGEHLP_MODULE64 modInfo = {};
				modInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
				if (SymGetModuleInfo64(hSymProcess, sf.AddrPC.Offset, &modInfo))
				{
					DWORD64 offset = sf.AddrPC.Offset - modInfo.BaseOfImage;
					sprintf_s(frameStr, "%s + 0x%I64X", modInfo.ModuleName, offset);
				}
				else
				{
					sprintf_s(frameStr, "0x%016I64X", sf.AddrPC.Offset);
				}

				if (frameNo == 0) strcpy_s(mainF0, frameStr);
				else if (frameNo == 1) strcpy_s(mainF1, frameStr);
				else if (frameNo == 2) strcpy_s(mainF2, frameStr);
				else if (frameNo == 3) strcpy_s(mainF3, frameStr);
				else if (frameNo == 4) strcpy_s(mainF4, frameStr);

				sprintf_s(logMsg, "[%s][%02d] %s", tag, frameNo, frameStr);
			}

			WriteMonitorLog(logMsg);
		}

		WriteMainSummary(this, th.ThreadId, mainF0, mainF1, mainF2);
		WriteMonitorLog("INI 저장 직전");

		AppendBlankLineToIni();
		WriteDumpAnalysisIniByDumpName(
			dumpPath,
			CString(mainF0),
			CString(mainF1),
			CString(mainF2),
			CString(mainF3),
			CString(mainF4));
		break;
}

	if (!bFoundTarget)
	{
		char buf[256] = { 0 };
		sprintf_s(buf,
			"[AN_MAIN] 대상스레드 미발견 target=%lu exception=%lu main=%lu",
			targetThreadId, exceptionThreadId, m_dwMainThreadId);
		WriteMonitorLog(buf);

		// fallback: 그래도 첫 번째 스레드 로그는 남기기
		if (pThreadList->NumberOfThreads > 0)
		{
			sprintf_s(buf,
				"[AN_MAIN] fallback first thread=%lu",
				pThreadList->Threads[0].ThreadId);
			WriteMonitorLog(buf);
		}
	}

	g_pDumpWalkCtx = NULL;

	SymCleanup(hSymProcess);
	UnmapViewOfFile(pView);
	CloseHandle(hMapping);
	CloseHandle(hFile);

	WriteMonitorLog("덤프 분석 완료");
}

void CAxisAgentDlg::AppendLogToList(const CString& sTime, const CString& sType, const CString& sMsg)
{
	if (!::IsWindow(m_listLog.GetSafeHwnd()))
		return;

	int nIndex = m_listLog.GetItemCount();
	m_listLog.InsertItem(nIndex, sTime);
	m_listLog.SetItemText(nIndex, 1, sType);
	m_listLog.SetItemText(nIndex, 2, sMsg);
	m_listLog.EnsureVisible(nIndex, FALSE);
}

void CAxisAgentDlg::AddLog(const char* type, const char* msg)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString sTime;
	sTime.Format("%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

	CString sType = type ? type : "";
	CString sMsg = msg ? msg : "";

	AGENT_LOG_ROW row;
	row.time = sTime;
	row.type = sType;
	row.msg = sMsg;

	m_allLogs.push_back(row);


	if (m_chkStop.GetCheck())
		return;

	if (ShouldShowLogType(type))
		AppendLogToList(sTime, sType, sMsg);
}

void CAxisAgentDlg::RefreshLogList()
{
	if (!::IsWindow(m_listLog.GetSafeHwnd()))
		return;

	m_listLog.DeleteAllItems();

	for (size_t i = 0; i < m_allLogs.size(); ++i)
	{
		const AGENT_LOG_ROW& row = m_allLogs[i];

		if (ShouldShowLogType(CT2A(row.type)))
			AppendLogToList(row.time, row.type, row.msg);
	}
}

void CAxisAgentDlg::OnBnClickedChkPing()
{
	RefreshLogList();
}

void CAxisAgentDlg::OnBnClickedChkMonitor()
{
	RefreshLogList();
}

void CAxisAgentDlg::OnBnClickedChkStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAxisAgentDlg::OnBnClickedListClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_listLog.DeleteAllItems();
}

void CAxisAgentDlg::WriteDumpAnalysisIniByDumpName(
	const char* dumpPath,
	const CString& top,
	const CString& c1,
	const CString& c2,
	const CString& c3,
	const CString& c4)
{
	if (dumpPath == NULL || dumpPath[0] == 0)
		return;

	CString iniPath = GetDumpAnalysisIniPath();
	CString section = GetFileNameOnly(dumpPath);

	SYSTEMTIME st;
	::GetLocalTime(&st);

	CString timeStr;
	timeStr.Format("%04d-%02d-%02d %02d:%02d:%02d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	::WritePrivateProfileStringA((LPCSTR)section, "TIME", (LPCSTR)timeStr, (LPCSTR)iniPath);
	::WritePrivateProfileStringA((LPCSTR)section, "TOP", (LPCSTR)top, (LPCSTR)iniPath);
	::WritePrivateProfileStringA((LPCSTR)section, "CALLER1", (LPCSTR)c1, (LPCSTR)iniPath);
	::WritePrivateProfileStringA((LPCSTR)section, "CALLER2", (LPCSTR)c2, (LPCSTR)iniPath);
	::WritePrivateProfileStringA((LPCSTR)section, "CALLER3", (LPCSTR)c3, (LPCSTR)iniPath);
	::WritePrivateProfileStringA((LPCSTR)section, "CALLER4", (LPCSTR)c4, (LPCSTR)iniPath);
}












CString CAxisAgentDlg::GetDumpAnalysisIniPath() const
{
	char selfPath[MAX_PATH] = { 0 };
	::GetModuleFileNameA(NULL, selfPath, MAX_PATH);

	char* lastSlash = strrchr(selfPath, '\\');
	if (lastSlash)
		*lastSlash = '\0';

	CString iniPath;
	iniPath.Format("%s\\ping\\dump_analysis.ini", selfPath);

	return iniPath;
}

CString CAxisAgentDlg::GetFileNameOnly(const char* fullPath) const
{
	CString sPath = fullPath ? fullPath : "";

	int pos = sPath.ReverseFind('\\');
	if (pos >= 0)
		return sPath.Mid(pos + 1);

	return sPath;
}

CString CAxisAgentDlg::GetMainCrashDumpDir() const
{
	// 메인 whdump.cpp 에서 dump 남기는 위치와 동일하게 맞춰야 함
	CString dumpDir;
	dumpDir.Format("%s",m_sUserpath);
	return dumpDir;
}

BOOL CAxisAgentDlg::IsDumpAlreadyAnalyzed(const CString& dumpPath) const
{
	CString iniPath = GetDumpAnalysisIniPath();
	CString section = GetFileNameOnly(dumpPath);

	char buf[64] = { 0 };
	::GetPrivateProfileStringA(
		(LPCSTR)section,
		"TIME",
		"",
		buf,
		sizeof(buf),
		(LPCSTR)iniPath);

	return (buf[0] != 0);
}

void CAxisAgentDlg::ScanAndAnalyzeMainCrashDumps()
{
	CString slog;
	CString dumpDir = GetMainCrashDumpDir();

	if (dumpDir.IsEmpty())
	{
		AddLog("MONITOR", "ScanAndAnalyzeMainCrashDumps: dumpDir is empty");
		return;
	}

	dumpDir.TrimRight("\\/");

	CString spec;
	spec.Format("%s\\*.dmp", (LPCSTR)dumpDir);

	slog.Format("dumpDir=[%s]", (LPCSTR)dumpDir);
	AddLog("MONITOR", slog);

	slog.Format("spec=[%s]", (LPCSTR)spec);
	AddLog("MONITOR", slog);

	DWORD attr = GetFileAttributes((LPCSTR)dumpDir);
	slog.Format("dir attr=0x%08X", attr);
	AddLog("MONITOR", slog);

	if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		AddLog("MONITOR", "ScanAndAnalyzeMainCrashDumps: dumpDir not found");
		return;
	}

	WIN32_FIND_DATA fd = { 0 };
	HANDLE hFind = FindFirstFile((LPCSTR)spec, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		slog.Format("FindFirstFile fail err=%lu", GetLastError());
		AddLog("MONITOR", slog);
		return;
	}

	int foundCount = 0;
	int analyzedCount = 0;
	std::vector<CString> dumpFiles;

	do
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		CString fileName = fd.cFileName;
		CString fullPath;
		fullPath.Format("%s\\%s", (LPCSTR)dumpDir, (LPCSTR)fileName);

		dumpFiles.push_back(fullPath);
		foundCount++;

		slog.Format("found dump: [%s]", (LPCSTR)fullPath);
		AddLog("MONITOR", slog);

	} while (FindNextFile(hFind, &fd));

	FindClose(hFind);

	std::sort(dumpFiles.begin(), dumpFiles.end(),
		[](const CString& a, const CString& b)
		{
			return a.CompareNoCase(b) < 0;
		});

	for (size_t i = 0; i < dumpFiles.size(); ++i)
	{
		const CString& dumpPath = dumpFiles[i];

		if (IsDumpAlreadyAnalyzed((LPCSTR)dumpPath))
			continue;

		CString msg;
		msg.Format("미분석 dump 발견: %s",
			(LPCSTR)GetFileNameOnly((LPCSTR)dumpPath));
		AddLog("DUMP", msg);

		AnalyzeDump((LPCSTR)dumpPath);

		CString okMsg;
		okMsg.Format("dump 분석 완료: %s",
			(LPCSTR)GetFileNameOnly((LPCSTR)dumpPath));
		AddLog("DUMP", okMsg);

		analyzedCount++;
	}

	CString sumMsg;
	sumMsg.Format("Crashlog scan 완료: found=%d analyzed=%d",
		foundCount, analyzedCount);
	AddLog("DUMP", sumMsg);
}


void CAxisAgentDlg::OnBnClickedBtnDumpans()
{
	ScanAndAnalyzeMainCrashDumps();
}


void CAxisAgentDlg::AppendBlankLineToIni()
{
	CString iniPath = GetDumpAnalysisIniPath();

	CFile file;
	if (file.Open(iniPath,
		CFile::modeWrite |
		CFile::modeNoTruncate |
		CFile::modeCreate))
	{
		file.SeekToEnd();
		file.Write("\r\n", 2);
		file.Close();
	}
}

void CAxisAgentDlg::RegisterTrayIcon()
{
	memset(&m_nid, 0, sizeof(m_nid));
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->GetSafeHwnd();
	m_nid.uID = 1;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_TRAYICON;

	// 아이콘: exe 내장 아이콘 사용 (IDR_MAINFRAME)
	m_nid.hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	// 툴팁 문자열
	strcpy_s(m_nid.szTip, "AxisAgent");

	BOOL bRet = Shell_NotifyIcon(NIM_ADD, &m_nid);

	// 결과 확인
	DebugLog("RegisterTrayIcon: result=%d hWnd=%p hIcon=%p err=%lu\n",
		bRet,
		m_nid.hWnd,
		m_nid.hIcon,
		GetLastError());

	if (!bRet)
	{
		// 원인별 체크
		if (!m_nid.hWnd)
			DebugLog("RegisterTrayIcon: hWnd 가 NULL\n");

		if (!m_nid.hIcon)
			DebugLog("RegisterTrayIcon: hIcon 로드 실패 - IDR_MAINFRAME 없을 수 있음\n");
	}
	else
	{
		m_bTrayRegistered = TRUE;
		DebugLog("RegisterTrayIcon: 트레이 등록 성공\n");
	}

}

void CAxisAgentDlg::RemoveTrayIcon()
{
	if (m_bTrayRegistered)
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
		m_bTrayRegistered = FALSE;
	}
}

void CAxisAgentDlg::OnTrayShow()
{
	// 스타일 확인 후 최소화 버튼 보장
	LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	if (!(style & WS_MINIMIZEBOX))
	{
		style |= WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION;
		SetWindowLong(m_hWnd, GWL_STYLE, style);
	}

	SetWindowPos(&wndTopMost,
		m_startX, m_startY,
		500, 400,
		SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	SetForegroundWindow();
}

void CAxisAgentDlg::OnTrayHide()
{
	ShowWindow(SW_HIDE);
}

void CAxisAgentDlg::OnTrayExit()
{
	PostMessage(WM_CLOSE);
}

LRESULT CAxisAgentDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_LBUTTONDBLCLK:   // 더블클릭 → 보이기/숨기기 토글
		if (IsWindowVisible())
			ShowWindow(SW_HIDE);
		else
		{
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
		}
		break;

	case WM_RBUTTONUP:        // 우클릭 → 컨텍스트 메뉴
	{
		CPoint pt;
		GetCursorPos(&pt);

		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_TRAY_SHOW, "보이기");
		menu.AppendMenu(MF_STRING, ID_TRAY_HIDE, "숨기기");
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_TRAY_EXIT, "종료");

		// 트레이 메뉴 제대로 닫히게 하는 필수 코드
		SetForegroundWindow();
		menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_BOTTOMALIGN,
			pt.x, pt.y, this);
		PostMessage(WM_NULL);
	}
	break;
	}
	return 0;
}

BOOL CAxisAgentDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::OnCommand(wParam, lParam);
}


void CAxisAgentDlg::OnBnClickedBtnHide()
{
	OnTrayHide();
}


void CAxisAgentDlg::OnBnClickedBtnDecacc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAxisAgentDlg::OnBnClickedBtnDumpOpen()
{
	CFileDialog dlg(TRUE, _T("dmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Dump Files (*.dmp)|*.dmp|All Files (*.*)|*.*||"),
		this);

	if (dlg.DoModal() != IDOK)
		return;

	m_editSymPath.GetWindowText(m_sSymPath);

	CStringA szPath(dlg.GetPathName());
	AddLog("DUMP", "--------------------------------");
	AddLog("DUMP", (LPCSTR)CStringA("[DUMP] 파일 분석 시작: ") + szPath);
	AnalyzeDump(szPath);
}

void CAxisAgentDlg::OnBnClickedBtnSympath()
{
	BROWSEINFOA bi = {};
	bi.hwndOwner = m_hWnd;
	bi.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpszTitle = "PDB 심볼 파일 폴더 선택";

	LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
	if (!pidl) return;

	char folderPath[MAX_PATH] = {};
	if (SHGetPathFromIDListA(pidl, folderPath))
	{
		m_sSymPath = folderPath;
		m_editSymPath.SetWindowText(m_sSymPath);

		// INI 저장
		char exeDir[MAX_PATH] = {};
		GetModuleFileNameA(NULL, exeDir, MAX_PATH);
		char* sl = strrchr(exeDir, '\\'); if (sl) *sl = '\0';
		char iniPath[MAX_PATH] = {};
		sprintf_s(iniPath, "%s\\AxisAgent.ini", exeDir);
		WritePrivateProfileStringA("Symbol", "Path", folderPath, iniPath);
	}
	CoTaskMemFree(pidl);
}


void CAxisAgentDlg::OnBnClickedBtnDumpopen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
