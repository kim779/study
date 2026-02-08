
// NaonStarterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "NaonStarter.h"
#include "NaonStarterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
void WriteLog(LPCSTR log, ...)
{
	CString slog;
	
	CString strPath;
 	DWORD dwSize = GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
 	strPath.GetBufferSetLength(dwSize);
	strPath.Replace("NaonStarter.exe", "log.ini");
 	//strPath.Replace("axis.exe", "axis.ndg");
 	//strPath.ReleaseBuffer();
	
	FILE* fp;
	fopen_s(&fp, strPath , "a+");
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


// CNaonStarterDlg 대화 상자



CNaonStarterDlg::CNaonStarterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAONSTARTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNaonStarterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNaonStarterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CNaonStarterDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNaonStarterDlg 메시지 처리기
#define TM_CHECK_NETWORK 9999
#define TM_CHECK_GNAGENT 9997
#define TM_CLOSE  9998

BOOL CNaonStarterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);

	CString strPath;
	DWORD dwSize = GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.GetBufferSetLength(dwSize);
	strPath.Replace("NaonStarter.exe", "log.ini");
	CFileFind find;
	if (find.FindFile(strPath))
		DeleteFile(strPath);

	DWORD processID = GetCurrentProcessId();
	ProcessFind("NaonStarter.exe", processID);

	strPath.Replace("log.ini", "NaonStarter.INI");
	if (find.FindFile(strPath))
	{
		CString stmp;
		char		wb[512];
		memset(wb, ' ', 512);
		int readL = GetPrivateProfileString("NaonBrower", "serverip", "", wb, sizeof(wb), strPath);
		if (readL){
			m_serverip.Format("%s", wb); m_serverip.Trim();
		}

		readL = GetPrivateProfileString("NaonBrower", "port", "", wb, sizeof(wb), strPath);
		if (readL) {
			stmp.Format("%s", wb); stmp.Trim();
			m_port = atoi(stmp);
		}

		readL = GetPrivateProfileString("NaonBrower", "path", "C:\\Naonsoft\\ToolBox\\NaonBrowser.exe", wb, sizeof(wb), strPath);
		if (readL) {
			m_strPath.Format("%s", wb); m_strPath.Trim();
		}

		readL = GetPrivateProfileString("NaonBrower", "try", "", wb, sizeof(wb), strPath);
		if (readL) {
			stmp.Format("%s", wb); stmp.Trim();
			m_itry = atoi(stmp);
		}
	}
	else
	{
		m_strPath = "C:\\Naonsoft\\ToolBox\\NaonBrowser.exe";
		m_serverip = "172.16.202.191";
		m_port = 9090;
		m_itry = 100;
	}

	SetTimer(TM_CHECK_GNAGENT, 3000, nullptr);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CNaonStarterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNaonStarterDlg::OnPaint()
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
HCURSOR CNaonStarterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNaonStarterDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
#include <TlHelp32.h>
BOOL CNaonStarterDlg::ProcessFind(char* strProcessName, DWORD processID)
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
			strProcess.Format("%s", pe32.szExeFile);
			if (strProcess.Find(strTarget) >= 0)
			{
				if (processID)
				{
					if (pe32.th32ProcessID != processID)
					{
						m_slog.Format("이미 naonstarter 실행중[%d] 신규 프로세스[%d] 꺼짐 [작업관리자 확인!!]", 
							pe32.th32ProcessID, processID );
						WriteLog(m_slog);
						PostMessage(WM_CLOSE, 0, 0);
						return 0;
					}

				}
				else
				{
					return TRUE;
				}
			}
		} while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}
	CloseHandle(hProcessSnap);

	return FALSE;
}

void CNaonStarterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString slog;
	switch (nIDEvent)
	{
		case TM_CHECK_GNAGENT:
		{
			if (ProcessFind("GnStart.exe"))
			{
				m_slog.Format(" GnStart.exe  FIND!!!!!");
				WriteLog(m_slog);
				KillTimer(nIDEvent);
				SetTimer(TM_CHECK_NETWORK, 1000, nullptr);
			}
		}
		break;
		case TM_CHECK_NETWORK:
		{
			KillTimer(nIDEvent);
			CSocket	sock;
			sock.Create();
		
			if (m_serverip.GetLength() > 0 && sock.Connect(m_serverip, m_port))
			{
				HINSTANCE hins =  ShellExecute(NULL, _T("open"), m_strPath, NULL, NULL, SW_SHOWNORMAL);
				m_slog.Format("[%s] 실행 HINSTANCE=[%x]", m_strPath, hins);
				WriteLog(m_slog);
				KillTimer(nIDEvent);
			}
			else
			{
				m_icnt++;
				SetTimer(TM_CHECK_NETWORK, 1000, nullptr);
				int ierror = GetLastError();
				slog.Format("err=[%d] cnt=[%d]", ierror, m_icnt);
				WriteLog(slog);
				if (m_icnt == m_itry)
					SendMessage(WM_CLOSE, 0, 0);
			}
		}
		break;
		case TM_CLOSE:
		{
			KillTimer(TM_CLOSE);
			PostMessage(WM_CLOSE, 0, 0);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
