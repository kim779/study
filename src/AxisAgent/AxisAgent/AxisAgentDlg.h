
// AxisAgentDlg.h: 헤더 파일
//

#pragma once
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#define WM_PING_LOG (WM_USER + 100)

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

	// 메서드
	void WriteLog(const char* msg);
	void SendToParent(const char* msg, int nKind);
	void StartPingThread();
	void ParseCommandLine();

	static DWORD WINAPI PingThreadProc(LPVOID pParam);
	void PingLoop();
	bool ShouldStop();

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
};
