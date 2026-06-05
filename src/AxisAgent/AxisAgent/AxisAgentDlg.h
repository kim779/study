
// AxisAgentDlg.h: 헤더 파일
//

#pragma once
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#include <DbgHelp.h>
#include <TlHelp32.h>
#pragma comment(lib, "DbgHelp.lib")

#define WM_PING_LOG (WM_USER + 100)

// 트레이 관련
#define WM_TRAYICON  (WM_USER + 100)
// 트레이 메뉴 ID
#define ID_TRAY_SHOW    2001
#define ID_TRAY_HIDE    2002
#define ID_TRAY_EXIT    2003

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
	char m_sUserpath[256]{};

	//ping 관련
	HANDLE m_hPingProcess{};
	HANDLE m_hPingPipeRead{};
	HANDLE m_hPingPipeWrite{};
	bool  m_bLastPingSuccess{};
	bool  m_bPingStateInit{};
	DWORD m_lastPingSummaryTick{};
	int   m_pingOkCount{};
	int   m_pingFailCount{};
	bool IsPingSuccessLine(const char* line);

	std::vector<AGENT_LOG_ROW> m_allLogs;

	CButton m_chkStop;
	CButton m_chkPing;
	CButton m_chkMonitor;
	CListCtrl m_listLog;

	void RegisterToMain();
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

	DWORD m_lastMonitorSummaryTick;
	bool  m_bLastHung;
	int   m_lastNoResponseCount;

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
	CString GetFileNameOnly(const char* fullPath) const;
	CString GetDumpAnalysisIniPath() const;
	CString GetMainCrashDumpDir() const;
	//BOOL IsDumpAlreadyAnalyzed(const char* dumpPath) const;
	BOOL IsDumpAlreadyAnalyzed(const CString& dumpPath) const;

	void ScanAndAnalyzeMainCrashDumps();

	void AppendBlankLineToIni();
	void WriteDumpAnalysisIniByDumpName(
		const char* dumpPath,
		const CString& top,
		const CString& c1,
		const CString& c2,
		const CString& c3,
		const CString& c4);

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

	//트레이
	NOTIFYICONDATA  m_nid;
	BOOL            m_bTrayRegistered = FALSE;
	void RegisterTrayIcon();
	void RemoveTrayIcon();
	void OnTrayShow();
	void OnTrayHide();
	void OnTrayExit();

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
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
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
	afx_msg void OnBnClickedBtnDumpans();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnHide();
	afx_msg void OnBnClickedBtnDecacc();
};
