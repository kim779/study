
// AxisAgentDlg.h: 헤더 파일
//

#pragma once
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#include <DbgHelp.h>
#include <TlHelp32.h>
#pragma comment(lib, "DbgHelp.lib")

#define WM_PING_LOG (WM_USER + 100)



// 감지 임계값
#define THRESHOLD_CPU       80.0f   // CPU 80% 이상
#define THRESHOLD_HANG_MS   3000    // 3초 응답없음
#define MONITOR_INTERVAL    1000    // 1초 주기

struct AGENT_LOG_ROW
{
	CString time;
	CString type;
	CString msg;
};

struct ThreadCpuInfo
{
	DWORD       dwThreadId;
	FILETIME    ftPrevKernel;
	FILETIME    ftPrevUser;
	float       fCpuUsage;
};

class CAxisAgentDlgAutoProxy;


// CAxisAgentDlg 대화 상자
class CAxisAgentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAxisAgentDlg);
	friend class CAxisAgentDlgAutoProxy;

// 생성입니다.
public:
	CAxisAgentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CAxisAgentDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AXISAGENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	// 전역 데이터
	int m_startX{}, m_startY{};
	BOOL m_bShow{};
	char   m_logFile[MAX_PATH];
	HANDLE m_hParent;
	HANDLE m_hStopEvent;
	HWND   m_hParentWnd;
	char   m_regkey[256];
	DWORD  m_parentPid;
	HANDLE m_hPingThread;

	std::vector<AGENT_LOG_ROW> m_allLogs;

	CButton m_chkStop;
	CButton m_chkPing;
	CButton m_chkMonitor;
	CListCtrl m_listLog;

	bool ShouldShowLogType(const char* type);

	int        m_dumpSeq = 0;
	int        m_dumpCount = 0;
	ULONGLONG  m_lastDumpTick = 0;

	// 멤버변수
	DWORD   m_dwMainThreadId = 0;       // 메인 스레드 ID
	HANDLE  m_hMonitorThread = NULL;
	bool    m_bDumpCreated = false;   // 중복 덤프 방지
	static DWORD WINAPI MonitorThreadProc(LPVOID pParam);
	void MonitorLoop();

	float CalcProcessCpuUsage(
		FILETIME& prevKernel, FILETIME& prevUser,
		FILETIME  curKernel, FILETIME  curUser,
		ULONGLONG& prevTickMs,
		ULONGLONG  curTickMs);

	float CalcThreadCpuUsage(
		FILETIME& prevKernel, FILETIME& prevUser,
		FILETIME  curKernel, FILETIME  curUser,
		ULONGLONG& prevTickMs,
		ULONGLONG  curTickMs);

	void CreateDump(const char* reason);
	void AnalyzeDump(const char* dumpPath);
	void WriteMonitorLog(const char* msg);

	static ULONGLONG FileTimeToULL(const FILETIME& ft)
	{
		return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
	}




	// 메서드
	void WriteLog(const char* msg);
	void SendToParent(const char* msg, int nKind);
	void StartPingThread();
	void ParseCommandLine();

	static DWORD WINAPI PingThreadProc(LPVOID pParam);
	void PingLoop();
	bool ShouldStop();

	void DebugLog(const char* fmt, ...);
	void AddLog(const char* type, const char* msg);
	void AppendLogToList(const CString& sTime, const CString& sType, const CString& sMsg);
	void RefreshLogList();

	// 네트워크
	enum NetType { NET_NONE, NET_WIFI, NET_WIRED };
	NetType GetCurrentNetType();
// 구현입니다.
protected:
	CAxisAgentDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCds); // HTS → Agent
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedChkPing();
	afx_msg void OnBnClickedChkStop();
	afx_msg void OnBnClickedChkMonitor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedListClear();
};
