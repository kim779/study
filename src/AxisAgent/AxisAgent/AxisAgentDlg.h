
// AxisAgentDlg.h: 헤더 파일
//

#pragma once
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#include <DbgHelp.h>
#include <TlHelp32.h>
#pragma comment(lib, "DbgHelp.lib")

#include <mutex>
#include <deque>
#define WM_AGENT_LOG (WM_APP + 0x120)

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
	CButton m_chkHts;
	CButton m_chkDebug;
	CButton m_chkFilter;
	CEdit   m_editFilter;
	CString m_sFilter;		// cached keyword (read from worker threads, no GetWindowText there)
	bool    m_bFilter = false;
	CListCtrl m_listLog;
	CRichEditCtrl m_editDetail;	// shows the double-clicked log row

	// resize layout
	enum { LAY_MOVE_X = 1, LAY_MOVE_Y = 2, LAY_SIZE_X = 4, LAY_SIZE_Y = 8 };
	struct LayoutItem { UINT id; CRect rc; int flags; };
	std::vector<LayoutItem> m_layout;
	CSize m_szInitClient;
	CSize m_szMinWindow;
	void InitLayout();
	void CopyLogsToClipboard(bool bAll);
	CEdit   m_editSymPath;
	CString m_sSymPath;

	void RegisterToMain();
	bool ShouldShowLogType(const char* type);
	bool PassFilter(const CString& msg);

	int        m_dumpSeq = 0;
	int        m_dumpCount = 0;
	int        m_dumpTotal = 0;		// lifetime dump count (not reset)
	bool       m_bDumpLimitLogged = false;
	int        m_dumpFailCount = 0;
	ULONGLONG  m_lastDumpTick = 0;

	// 멤버변수
	DWORD   m_dwMainThreadId = 0;       // 메인 스레드 ID
	HANDLE  m_hMonitorThread = NULL;
	static DWORD WINAPI MonitorThreadProc(LPVOID pParam);
	void MonitorLoop();

	//락
	std::mutex	m_logQueueLock;
	std::deque<AGENT_LOG_ROW>   m_logQueue;
	bool m_bLogPosted = false;

	// latest open-screen list from HTS "[MAP] ... | now=[...]" log
	std::mutex m_mapsLock;
	CStringA   m_lastMaps;
	DWORD      m_firstMapTick = 0;	// guarded by m_mapsLock; tick of first [MAP] (screen restore start)

	// CpuSpike dump policy
	bool IsCpuDumpEnabled() const;	// tab\AXISAI.ini [Axisagent] cpudump (default 1)


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

	void CreateDump(const char* reason, const char* trigger = nullptr);
	void AnalyzeDump(const char* dumpPath);
	void WriteMonitorLog(const char* msg, const char* type = "MONITOR");
	void CleanupPingFolder(int keepDays);
	CString GetFileNameOnly(const char* fullPath) const;
	CString GetDumpAnalysisIniPath() const;

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
	void SaveSymPath();

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

	CAxisAgentDlgAutoProxy* m_pAutoProxy{};
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
	afx_msg LRESULT OnAgentLog(WPARAM, LPARAM);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCds); // HTS → Agent
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedChkPing();
	afx_msg void OnBnClickedChkMonitor();
	afx_msg void OnBnClickedChkHts();
	afx_msg void OnBnClickedChkDebug();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNMRClickListLog(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListLog(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedChkFilter();
	afx_msg void OnEnChangeEditFilter();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedListClear();
	afx_msg void OnBnClickedBtnHide();
	afx_msg void OnBnClickedBtnDumpOpen();
	afx_msg void OnBnClickedBtnSympath();

	afx_msg void OnBnClickedChkStop();
};
