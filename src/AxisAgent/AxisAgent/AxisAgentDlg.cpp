
// AxisAgentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "AxisAgent.h"
#include "AxisAgentDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

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
				const MINIDUMP_MEMORY_DESCRIPTOR64& md =
					ctx->pMem64List->MemoryRanges[i];

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

void DebugLog(const char* fmt, ...)
{
	char msg[1024] = { 0 };
	va_list args;
	va_start(args, fmt);
	vsprintf_s(msg, sizeof(msg), fmt, args);
	va_end(args);

	char buf[1024] = { 0 };
	sprintf_s(buf, sizeof(buf), "[AxisAgent] %s", msg);
	OutputDebugStringA(buf);
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
	, m_dumpSeq(0)
	, m_dumpCount(0)
	, m_lastDumpTick(0)
{
	memset(m_logFile, 0, sizeof(m_logFile));
	memset(m_regkey, 0, sizeof(m_regkey));
}

//CAxisAgentDlg::CAxisAgentDlg(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_AXISAGENT_DIALOG, pParent)
//{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//	m_pAutoProxy = nullptr;
//}

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
}

BEGIN_MESSAGE_MAP(CAxisAgentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_TEST, &CAxisAgentDlg::OnBnClickedBtnTest)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAxisAgentDlg 메시지 처리기

BOOL CAxisAgentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CWnd*)GetDlgItem(IDOK))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDCANCEL))->ShowWindow(SW_HIDE);

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	// 커맨드라인 파싱
	ParseCommandLine();

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

	// 네트워크 타입 로그
	char netLog[256] = { 0 };
	switch (GetCurrentNetType())
	{
	case NET_WIFI:  sprintf_s(netLog, "[시작] 네트워크: WiFi\n");  break;
	case NET_WIRED: sprintf_s(netLog, "[시작] 네트워크: 유선\n");  break;
	case NET_NONE:  sprintf_s(netLog, "[시작] 네트워크: 없음\n");  break;
	}
	WriteLog(netLog);
	SendToParent(netLog, AGENT_MSG_NETTYPE);

	// ping 스레드 시작
	StartPingThread();

	// 실제 모니터 스레드 시작
	m_hMonitorThread = CreateThread(
		NULL, 0, MonitorThreadProc, (LPVOID)this, 0, NULL);

	if (m_hMonitorThread)
		DebugLog("OnInitDialog: MonitorThread 시작\n");
	else
		DebugLog("OnInitDialog: MonitorThread CreateThread 실패 err=%lu\n", GetLastError());

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
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CAxisAgentDlg::ParseCommandLine()
{
	int argc = 0;
	LPWSTR* argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (!argvW) return;

	CString slog;
	for (int i = 1; i < argc; i++)
	{
		if (wcscmp(argvW[i], L"/p") == 0 && i + 1 < argc)
			m_parentPid = (DWORD)_wtoi(argvW[++i]);

		else if (wcscmp(argvW[i], L"/h") == 0 && i + 1 < argc)
			m_hParentWnd = (HWND)(UINT64)_wtoi64(argvW[++i]);

		else if (wcscmp(argvW[i], L"/n") == 0 && i + 1 < argc)
			WideCharToMultiByte(CP_ACP, 0, argvW[++i], -1,
				m_regkey, sizeof(m_regkey), NULL, NULL);

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
			m_dwMainThreadId = (DWORD)_wtoi(argvW[++i]);
	}



	SetWindowPos(
		NULL,
		m_startX,   // 메인 left
		m_startY,   // 메인 top
		400, 200,   // 크기 (원하는 크기로)
		SWP_NOZORDER | SWP_NOACTIVATE | (m_bShow ? SWP_SHOWWINDOW : SWP_HIDEWINDOW)
	);

	LocalFree(argvW);
	slog.Format("ParseCommandLine: PID=%lu HWND=%llu TID=%lu KEY=%s show=%d\n",
		m_parentPid, (UINT64)m_hParentWnd, m_dwMainThreadId, m_regkey, m_bShow);
	WriteMonitorLog(slog);

	//m_hMonitorThread = CreateThread(
	//	NULL, 0, MonitorThreadProc, (LPVOID)this, 0, NULL);
	//DebugLog("OnInitDialog: MonitorThread 시작\n");

	SetTimer(9898, 100, nullptr);
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
	return;
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

	if (m_hStopEvent)
	{
		SetEvent(m_hStopEvent);
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}

	if (m_hMonitorThread)
	{
		WaitForSingleObject(m_hMonitorThread, 3000);
		CloseHandle(m_hMonitorThread);
		m_hMonitorThread = NULL;
	}

	if (m_hPingThread)
	{
		WaitForSingleObject(m_hPingThread, 3000);
		CloseHandle(m_hPingThread);
		m_hPingThread = NULL;
	}

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

void CAxisAgentDlg::PingLoop()
{
	DebugLog("PingLoop 시작\n");

	SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
	HANDLE hReadPipe, hWritePipe;
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		DebugLog("PingLoop: CreatePipe 실패 err=%d\n", GetLastError());
		return;
	}
	SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

	STARTUPINFOA si = { sizeof(si) };
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = hWritePipe;
	si.hStdError = hWritePipe;

	PROCESS_INFORMATION pi = { 0 };
	char cmd[] = "ping 8.8.8.8 -t";

	if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE,
		CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		DebugLog("PingLoop: ping CreateProcess 실패 err=%d\n", GetLastError());
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		return;
	}

	DebugLog("PingLoop: ping 프로세스 시작 PID=%lu\n", pi.dwProcessId);
	CloseHandle(hWritePipe);

	char buf[1024] = { 0 };
	char line[1024] = { 0 };
	int  linePos = 0;
	DWORD bytesRead;

	while (ReadFile(hReadPipe, buf, sizeof(buf) - 1, &bytesRead, NULL)
		&& bytesRead > 0)
	{
		buf[bytesRead] = '\0';

		for (DWORD i = 0; i < bytesRead; i++)
		{
			char c = buf[i];
			if (c == '\r') continue;

			if (c == '\n')
			{
				line[linePos] = '\0';
				linePos = 0;
				if (strlen(line) == 0) continue;

				SYSTEMTIME st;
				GetLocalTime(&st);

				char output[2048] = { 0 };
				sprintf_s(output, sizeof(output),
					"[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
					st.wYear, st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond,
					line);

				WriteLog(output);
				DebugLog("%s", output);
				SendToParent(output, AGENT_MSG_PING);

				// 종료 체크
				if (ShouldStop())
				{
					WriteLog("[종료] 종료 신호 감지 - 자체 종료\n");
					DebugLog("PingLoop: 종료 신호 - ping 종료\n");
					TerminateProcess(pi.hProcess, 0);
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
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(hReadPipe);
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

	//OutputDebugStringA(output);
}

// 덤프 생성
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

	char logMsg[1024] = { 0 };
	sprintf_s(logMsg,
		"[D1] dump start seq=%d reason=%s path=%s pid=%lu",
		seq,
		reason ? reason : "(null)",
		dumpPath,
		m_parentPid);
	WriteMonitorLog(logMsg);

	DWORD access =
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ |
		PROCESS_DUP_HANDLE |
		SYNCHRONIZE;

	HANDLE hProcess = OpenProcess(access, FALSE, m_parentPid);
	if (!hProcess)
	{
		sprintf_s(logMsg, "[D2] seq=%d OpenProcess fail err=%lu", seq, GetLastError());
		WriteMonitorLog(logMsg);
		return;
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

	MINIDUMP_EXCEPTION_INFORMATION* pExInfo = NULL;

	SetLastError(0);

	BOOL bOk = MiniDumpWriteDump(
		hProcess,
		m_parentPid,
		hFile,
		MiniDumpNormal,
		pExInfo,
		NULL,
		NULL);

	DWORD dwErr = GetLastError();

	FlushFileBuffers(hFile);

	LARGE_INTEGER fileSize = {};
	GetFileSizeEx(hFile, &fileSize);

	CloseHandle(hFile);
	CloseHandle(hProcess);

	sprintf_s(logMsg,
		"[D5] seq=%d MiniDumpWriteDump ok=%d err=%lu size=%lld",
		seq, (int)bOk, dwErr, fileSize.QuadPart);
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


		char logMsg[512] = { 0 };
		sprintf_s(logMsg,
			"[monitor]프로세스CPU=%.1f%% 메인스레드CPU=%.1f%% 스레드수=%d 응답실패=%d Hung=%d",
			fProcCpu, fMainCpu, nThreadCount, noResponseCount, bHung ? 1 : 0);
		WriteMonitorLog(logMsg);

		char sendMsg[256] = { 0 };
		sprintf_s(sendMsg, "CPU=%.1f MAIN=%.1f THR=%d",
			fProcCpu, fMainCpu, nThreadCount);
		SendToParent(sendMsg, AGENT_MSG_MONITOR);

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

void CAxisAgentDlg::OnBnClickedBtnTest()
{

}


void CAxisAgentDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case 9898:
		{
			KillTimer(nIDEvent);
			ShowWindow(m_bShow);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CAxisAgentDlg::AnalyzeDump(const char* dumpPath)
{
	if (!dumpPath || !dumpPath[0])
		return;

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

	if (!SymInitialize(hSymProcess, NULL, FALSE))
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

		sprintf_s(logMsg,
			"[AN1] ExceptionThread=%lu Code=0x%08lX Addr=0x%08lX",
			pExc->ThreadId,
			pExc->ExceptionRecord.ExceptionCode,
			(DWORD)pExc->ExceptionRecord.ExceptionAddress);
		WriteMonitorLog(logMsg);
	}
	else
	{
		WriteMonitorLog("[AN1] ExceptionStream 없음");
	}

	DumpWalkContext walkCtx;
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

	g_pDumpWalkCtx = &walkCtx;

	bool bFoundMain = false;

	for (ULONG i = 0; i < pThreadList->NumberOfThreads; ++i)
	{
		MINIDUMP_THREAD& th = pThreadList->Threads[i];

		bool bMain = (th.ThreadId == m_dwMainThreadId);
		bool bException = (exceptionThreadId != 0 && th.ThreadId == exceptionThreadId);

		if (!bMain)
			continue;

		bFoundMain = true;

		sprintf_s(logMsg,
			"=== ThreadID=%lu %s%s ===",
			th.ThreadId,
			bMain ? "(메인스레드)" : "",
			bException ? "(예외스레드)" : "");
		WriteMonitorLog(logMsg);

		CONTEXT* pCtx = (CONTEXT*)(pView + th.ThreadContext.Rva);
		if (!pCtx)
		{
			WriteMonitorLog("  컨텍스트 없음");
			break;
		}

#ifdef _M_IX86
		sprintf_s(logMsg,
			"  EIP=0x%08lX EBP=0x%08lX ESP=0x%08lX",
			pCtx->Eip, pCtx->Ebp, pCtx->Esp);
		WriteMonitorLog(logMsg);

		STACKFRAME64 sf;
		ZeroMemory(&sf, sizeof(sf));

		sf.AddrPC.Offset = pCtx->Eip;
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrFrame.Offset = pCtx->Ebp;
		sf.AddrFrame.Mode = AddrModeFlat;
		sf.AddrStack.Offset = pCtx->Esp;
		sf.AddrStack.Mode = AddrModeFlat;

		CONTEXT ctxCopy = *pCtx;

		char mainF0[256] = { 0 };
		char mainF1[256] = { 0 };
		char mainF2[256] = { 0 };

		for (int frameNo = 0; frameNo < 10; ++frameNo)
		{
			BOOL sw = StackWalk64(
				IMAGE_FILE_MACHINE_I386,
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

			char symBuf[sizeof(SYMBOL_INFO) + 256] = { 0 };
			SYMBOL_INFO* pSym = (SYMBOL_INFO*)symBuf;
			pSym->SizeOfStruct = sizeof(SYMBOL_INFO);
			pSym->MaxNameLen = 255;

			DWORD64 disp64 = 0;
			IMAGEHLP_LINE64 line;
			ZeroMemory(&line, sizeof(line));
			line.SizeOfStruct = sizeof(line);
			DWORD lineDisp = 0;

			char funcName[256] = { 0 };

			if (SymFromAddr(hSymProcess, sf.AddrPC.Offset, &disp64, pSym))
			{
				strcpy_s(funcName, pSym->Name);

				if (frameNo == 0) strcpy_s(mainF0, funcName);
				else if (frameNo == 1) strcpy_s(mainF1, funcName);
				else if (frameNo == 2) strcpy_s(mainF2, funcName);

				if (frameNo == 0)
				{
					sprintf_s(logMsg,
						"[MAIN][TOP] %s + 0x%I64X",
						pSym->Name,
						disp64);
					WriteMonitorLog(logMsg);
				}

				if (SymGetLineFromAddr64(hSymProcess, sf.AddrPC.Offset, &lineDisp, &line))
				{
					sprintf_s(logMsg,
						"  [%02d] %s + 0x%I64X (%s:%lu)",
						frameNo,
						pSym->Name,
						disp64,
						line.FileName ? line.FileName : "?",
						line.LineNumber);
				}
				else
				{
					sprintf_s(logMsg,
						"  [%02d] %s + 0x%I64X",
						frameNo,
						pSym->Name,
						disp64);
				}
			}
			else
			{
				if (frameNo == 0)
				{
					sprintf_s(logMsg,
						"[MAIN][TOP] 0x%08I64X",
						sf.AddrPC.Offset);
					WriteMonitorLog(logMsg);
				}

				if (frameNo == 0) sprintf_s(mainF0, "0x%08I64X", sf.AddrPC.Offset);
				else if (frameNo == 1) sprintf_s(mainF1, "0x%08I64X", sf.AddrPC.Offset);
				else if (frameNo == 2) sprintf_s(mainF2, "0x%08I64X", sf.AddrPC.Offset);

				sprintf_s(logMsg,
					"  [%02d] 0x%08I64X",
					frameNo,
					sf.AddrPC.Offset);
			}

			WriteMonitorLog(logMsg);
		}

		WriteMainSummary(this, th.ThreadId, mainF0, mainF1, mainF2);
#else
		WriteMonitorLog("  현재 AnalyzeDump는 x86 기준으로 작성됨");
#endif
		break;
	}

	if (!bFoundMain)
	{
		char buf[256] = { 0 };
		sprintf_s(buf,
			"[AN_MAIN] 메인스레드 미발견 m_dwMainThreadId=%lu",
			m_dwMainThreadId);
		WriteMonitorLog(buf);
	}

	g_pDumpWalkCtx = NULL;

	SymCleanup(hSymProcess);
	UnmapViewOfFile(pView);
	CloseHandle(hMapping);
	CloseHandle(hFile);

	WriteMonitorLog("덤프 분석 완료");
}