
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


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

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
END_MESSAGE_MAP()


// CAxisAgentDlg 메시지 처리기

BOOL CAxisAgentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CWnd*)GetDlgItem(IDOK))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDCANCEL))->ShowWindow(SW_HIDE);

	// 창 숨기기
	ShowWindow(SW_HIDE);

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
	m_hStopEvent = OpenEvent(SYNCHRONIZE, FALSE, eventName);
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
	DebugLog("ParseCommandLine: PID=%lu HWND=%llu KEY=%s\n",
		m_parentPid, (UINT64)m_hParentWnd, m_regkey);


	// MonitorThread 시작
	m_hMonitorThread = CreateThread(
		NULL, 0, MonitorThreadProc, (LPVOID)this, 0, NULL);
	DebugLog("OnInitDialog: MonitorThread 시작\n");
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
	if (!m_hParentWnd) return;

	COPYDATASTRUCT cds = { 0 };
	cds.dwData = (ULONG_PTR)nKind;
	cds.cbData = (DWORD)strlen(msg) + 1;
	cds.lpData = (void*)msg;

	DWORD_PTR result = 0;
	LRESULT lr = SendMessageTimeout(
		m_hParentWnd,
		WM_COPYDATA,
		(WPARAM)m_hWnd,   // 보내는 쪽 HWND
		(LPARAM)&cds,
		SMTO_ABORTIFHUNG,
		3000,
		&result);

	if (lr == 0)
		DebugLog("SendToParent: 전송 실패 or 타임아웃 err=%d\n", GetLastError());
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

	// StopEvent 시그널
	if (m_hStopEvent)
	{
		SetEvent(m_hStopEvent);
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}

	// ping 스레드 종료 대기
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
float CAxisAgentDlg::CalcCpuUsage(
	FILETIME& prevKernel, FILETIME& prevUser,
	FILETIME  curKernel, FILETIME  curUser)
{
	ULONGLONG prevTotal = FileTimeToULL(prevKernel) + FileTimeToULL(prevUser);
	ULONGLONG curTotal = FileTimeToULL(curKernel) + FileTimeToULL(curUser);
	ULONGLONG diff = curTotal - prevTotal;

	// 1초 = 10000000 (100나노초 단위)
	float fCpu = (float)diff / 10000000.0f * 100.0f;

	prevKernel = curKernel;
	prevUser = curUser;

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
	sprintf_s(output, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond,
		msg);

	FILE* fp = nullptr;
	fopen_s(&fp, logPath, "a");
	if (fp)
	{
		fputs(output, fp);
		fclose(fp);
	}

	OutputDebugStringA(output);
}

// 덤프 생성
void CAxisAgentDlg::CreateDump(const char* reason)
{
	if (m_bDumpCreated) return; // 중복 방지
	m_bDumpCreated = true;

	SYSTEMTIME st;
	GetLocalTime(&st);

	char selfPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, selfPath, MAX_PATH);
	char* lastSlash = strrchr(selfPath, '\\');
	if (lastSlash) *lastSlash = '\0';

	char dumpPath[MAX_PATH] = { 0 };
	sprintf_s(dumpPath, "%s\\ping\\dump_%04d%02d%02d_%02d%02d%02d.dmp",
		selfPath,
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	char logMsg[512] = { 0 };
	sprintf_s(logMsg, "덤프 생성 시작 reason=%s path=%s", reason, dumpPath);
	WriteMonitorLog(logMsg);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_parentPid);
	if (!hProcess)
	{
		WriteMonitorLog("덤프 생성 실패 OpenProcess 에러");
		return;
	}

	HANDLE hFile = CreateFileA(dumpPath,
		GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		WriteMonitorLog("덤프 생성 실패 파일 생성 에러");
		CloseHandle(hProcess);
		return;
	}

	BOOL bOk = MiniDumpWriteDump(
		hProcess,
		m_parentPid,
		hFile,
		(MINIDUMP_TYPE)(MiniDumpWithFullMemory |
			MiniDumpWithThreadInfo |
			MiniDumpWithProcessThreadData),
		NULL, NULL, NULL);

	CloseHandle(hFile);
	CloseHandle(hProcess);

	if (bOk)
	{
		WriteMonitorLog("덤프 생성 완료");
		AnalyzeDump(dumpPath); // 바로 분석
	}
	else
	{
		WriteMonitorLog("덤프 생성 실패 MiniDumpWriteDump 에러");
	}
}

// 덤프 분석
void CAxisAgentDlg::AnalyzeDump(const char* dumpPath)
{
	WriteMonitorLog("덤프 분석 시작");

	HANDLE hFile = CreateFileA(dumpPath,
		GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return;

	HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (!hMapping) { CloseHandle(hFile); return; }

	void* pView = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (!pView) { CloseHandle(hMapping); CloseHandle(hFile); return; }

	// 스레드 목록 가져오기
	PVOID pStream = nullptr;
	ULONG streamSize = 0;
	MiniDumpReadDumpStream(pView, ThreadListStream,
		nullptr, &pStream, &streamSize);

	if (!pStream)
	{
		WriteMonitorLog("덤프 분석 실패 스레드 목록 없음");
		UnmapViewOfFile(pView);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}

	// PDB 경로 (exe 와 같은 폴더)
	char selfPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, selfPath, MAX_PATH);
	char* lastSlash = strrchr(selfPath, '\\');
	if (lastSlash) *lastSlash = '\0';

	// 심볼 초기화
	HANDLE hProcess = GetCurrentProcess();
	SymInitialize(hProcess, selfPath, FALSE);
	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);

	// 모듈 로드
	PVOID pModStream = nullptr;
	ULONG modStreamSize = 0;
	MiniDumpReadDumpStream(pView, ModuleListStream,
		nullptr, &pModStream, &modStreamSize);

	if (pModStream)
	{
		MINIDUMP_MODULE_LIST* pModList = (MINIDUMP_MODULE_LIST*)pModStream;
		for (ULONG i = 0; i < pModList->NumberOfModules; i++)
		{
			MINIDUMP_MODULE& mod = pModList->Modules[i];
			SymLoadModuleEx(hProcess, NULL,
				(char*)((BYTE*)pView + mod.ModuleNameRva + 4),
				NULL,
				mod.BaseOfImage, mod.SizeOfImage,
				NULL, 0);
		}
	}

	// 스레드 콜스택 분석
	MINIDUMP_THREAD_LIST* pThreadList = (MINIDUMP_THREAD_LIST*)pStream;

	char logMsg[1024] = { 0 };
	sprintf_s(logMsg, "덤프 분석 스레드수=%lu", pThreadList->NumberOfThreads);
	WriteMonitorLog(logMsg);

	for (ULONG i = 0; i < pThreadList->NumberOfThreads; i++)
	{
		MINIDUMP_THREAD& thread = pThreadList->Threads[i];

		sprintf_s(logMsg, "=== ThreadID=%lu %s ===",
			thread.ThreadId,
			thread.ThreadId == m_dwMainThreadId ? "(메인스레드)" : "");
		WriteMonitorLog(logMsg);

		// 컨텍스트
		CONTEXT* pCtx = (CONTEXT*)((BYTE*)pView +
			thread.ThreadContext.Rva);

		STACKFRAME64 sf = { 0 };

		// x86 (32비트)
		sf.AddrPC.Offset = pCtx->Eip;  // Rip → Eip
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrFrame.Offset = pCtx->Ebp;  // Rbp → Ebp
		sf.AddrFrame.Mode = AddrModeFlat;
		sf.AddrStack.Offset = pCtx->Esp;  // Rsp → Esp
		sf.AddrStack.Mode = AddrModeFlat;

		int nFrame = 0;
		while (StackWalk64(
			IMAGE_FILE_MACHINE_I386,  // AMD64 → I386
			hProcess, NULL,
			&sf, pCtx,
			NULL,
			SymFunctionTableAccess64,
			SymGetModuleBase64,
			NULL) && nFrame < 20)
		{
			char symBuf[sizeof(SYMBOL_INFO) + 256] = { 0 };
			SYMBOL_INFO* pSym = (SYMBOL_INFO*)symBuf;
			pSym->SizeOfStruct = sizeof(SYMBOL_INFO);
			pSym->MaxNameLen = 255;

			DWORD64 displacement = 0;
			if (SymFromAddr(hProcess,
				sf.AddrPC.Offset, &displacement, pSym))
			{
				IMAGEHLP_LINE64 line = { sizeof(line) };
				DWORD lineDisp = 0;
				SymGetLineFromAddr64(hProcess,
					sf.AddrPC.Offset, &lineDisp, &line);

				sprintf_s(logMsg, "  [%02d] %s() line=%lu",
					nFrame, pSym->Name, line.LineNumber);
			}
			else
			{
				// PDB 없으면 주소값
				sprintf_s(logMsg, "  [%02d] 0x%016llX (PDB없음)",
					nFrame, sf.AddrPC.Offset);
			}

			WriteMonitorLog(logMsg);
			nFrame++;
		}
	}

	SymCleanup(hProcess);
	UnmapViewOfFile(pView);
	CloseHandle(hMapping);
	CloseHandle(hFile);

	WriteMonitorLog("덤프 분석 완료");
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

	// 프로세스 핸들
	HANDLE hProcess = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_ALL_ACCESS,
		FALSE, m_parentPid);

	if (!hProcess)
	{
		WriteMonitorLog("MonitorLoop OpenProcess 실패");
		return;
	}

	// 메인 스레드 핸들
	HANDLE hMainThread = OpenThread(
		THREAD_QUERY_INFORMATION,
		FALSE, m_dwMainThreadId);

	// 이전 CPU 시간 초기화
	FILETIME ftPrevProcKernel = { 0 }, ftPrevProcUser = { 0 };
	FILETIME ftPrevMainKernel = { 0 }, ftPrevMainUser = { 0 };
	FILETIME ftDummy = { 0 };

	// 첫 측정값 가져오기
	GetProcessTimes(hProcess,
		&ftDummy, &ftDummy,
		&ftPrevProcKernel, &ftPrevProcUser);

	if (hMainThread)
		GetThreadTimes(hMainThread,
			&ftDummy, &ftDummy,
			&ftPrevMainKernel, &ftPrevMainUser);

	while (true)
	{
		Sleep(MONITOR_INTERVAL);

		if (ShouldStop()) break;

		// 1. 행 감지 - IsHungAppWindow
		if (m_hParentWnd && IsHungAppWindow(m_hParentWnd))
		{
			WriteMonitorLog("행 감지 IsHungAppWindow");
			CreateDump("HungApp");
			break;
		}

		// 2. 응답없음 감지 - SendMessageTimeout
		DWORD_PTR result = 0;
		LRESULT lr = SendMessageTimeout(
			m_hParentWnd, WM_NULL, 0, 0,
			SMTO_ABORTIFHUNG,
			THRESHOLD_HANG_MS, &result);

		if (lr == 0)
		{
			WriteMonitorLog("행 감지 SendMessageTimeout 응답없음");
			CreateDump("NoResponse");
			break;
		}

		// 3. 프로세스 CPU 측정
		FILETIME ftCurProcKernel = { 0 }, ftCurProcUser = { 0 };
		GetProcessTimes(hProcess,
			&ftDummy, &ftDummy,
			&ftCurProcKernel, &ftCurProcUser);

		float fProcCpu = CalcCpuUsage(
			ftPrevProcKernel, ftPrevProcUser,
			ftCurProcKernel, ftCurProcUser);

		// 4. 메인 스레드 CPU 측정
		float fMainCpu = 0.0f;
		if (hMainThread)
		{
			FILETIME ftCurMainKernel = { 0 }, ftCurMainUser = { 0 };
			GetThreadTimes(hMainThread,
				&ftDummy, &ftDummy,
				&ftCurMainKernel, &ftCurMainUser);

			fMainCpu = CalcCpuUsage(
				ftPrevMainKernel, ftPrevMainUser,
				ftCurMainKernel, ftCurMainUser);
		}

		// 5. 스레드 개수
		HANDLE hSnap = CreateToolhelp32Snapshot(
			TH32CS_SNAPTHREAD, m_parentPid);
		int nThreadCount = 0;
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			THREADENTRY32 te = { sizeof(te) };
			if (Thread32First(hSnap, &te))
			{
				do {
					if (te.th32OwnerProcessID == m_parentPid)
						nThreadCount++;
				} while (Thread32Next(hSnap, &te));
			}
			CloseHandle(hSnap);
		}

		// 6. 로그
		char logMsg[512] = { 0 };
		sprintf_s(logMsg,
			"프로세스CPU=%.1f%% 메인스레드CPU=%.1f%% 스레드수=%d",
			fProcCpu, fMainCpu, nThreadCount);
		WriteMonitorLog(logMsg);

		// 7. WM_COPYDATA 로 HTS 에 전송
		char sendMsg[256] = { 0 };
		sprintf_s(sendMsg, "CPU=%.1f MAIN=%.1f THR=%d",
			fProcCpu, fMainCpu, nThreadCount);
		SendToParent(sendMsg, AGENT_MSG_MONITOR);

		// 8. CPU 급등 감지
		if (fMainCpu > THRESHOLD_CPU)
		{
			sprintf_s(logMsg,
				"메인스레드 CPU 급등 %.1f%% 덤프 생성",
				fMainCpu);
			WriteMonitorLog(logMsg);
			CreateDump("CpuSpike");
			break;
		}
	}

	if (hMainThread) CloseHandle(hMainThread);
	CloseHandle(hProcess);

	WriteMonitorLog("MonitorLoop 종료");
}