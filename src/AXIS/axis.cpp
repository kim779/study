// axis.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "axis.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "axisDoc.h"
#include "axisView.h"
#include "RSCinfo.h"

#include "axInstance.h"

//#include <..\src\occimpl.h> //vc2019
#include <afxocc.h>  //vc2019 occimpl.h -> afxocc.h


#include "custsite.h"
#include "axmsg.hxx"

#include "cpuUse.h"

#include <TlHelp32.h>

#define	SERVERIP	"Server_IP"
#define	SERVERURL	"Server_URL"
#define	YELLOWPAGE	"YellowPage.ini"

#define _LICENCE_ "ovs-dpme-iph"

#include "FirstJob.h"
#include "whdump.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CHKID	100			// CPU, RTS check ID
#define	CHKTIME	2000			// CPU, RTS check time

/////////////////////////////////////////////////////////////////////////////
// CAxisApp

BEGIN_MESSAGE_MAP(CAxisApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisApp construction

CAxisApp::CAxisApp()
{
	m_instance = NULL;
	m_flag     = 0;
	Axis::user     = _T("");
	m_regkey   = _T("");
	m_progK	   = MD_DEV;//PK_DEV;
	m_pDispOM  = NULL;
	m_hSCSK	   = NULL;
	
	m_logPath  = _T("");

	m_resourceHelper = NULL;

	m_forceIP.Empty();
	m_forcePort = portEmployee;
}

CAxisApp::~CAxisApp()
{
	if (m_pDispOM)	delete m_pDispOM;
	if (m_resourceHelper) delete m_resourceHelper;
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisApp object

CAxisApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxisApp initialization
// int GetProcssNameByPID(DWORD dwPID, char *buff, int size)
// {
//     int nBinNameLen = 0;
//     HANDLE hProc = NULL;
// 	
//     if ( (hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID)) != NULL)
//     {
//         nBinNameLen = ::GetModuleBaseName(hProc, NULL, buff, size);
//         CloseHandle (hProc);
//     }
// 	
//     return nBinNameLen;
// }

ULONG CAxisApp::KillPID(CString processname,ULONG processId)
{	
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	PROCESSENTRY32 pe32 = { 0, };
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	
	if ( !Process32First(hProcessSnap, &pe32) )
	{
		OutputDebugString(_T("Error checking process"));
		CloseHandle(hProcessSnap);
		return false;
	}
	
	do {
		CString progS(pe32.szExeFile);

		if (progS.Find(processname) > -1)
		{
			if(pe32.th32ProcessID == processId)
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,pe32.th32ProcessID);

				if(hProcess)
				{
					TerminateProcess(hProcess,-1);
					CloseHandle(hProcess);

					return pe32.th32ProcessID;
				}
			}
		}
	} while ( Process32Next(hProcessSnap, &pe32) );
	
	
	CloseHandle(hProcessSnap);
	
	return false;
}

ULONG ProcIDFromWnd(HWND hwnd) // 윈도우 핸들로 프로세스 아이디 얻기   
{   
	ULONG idProc;   
	::GetWindowThreadProcessId( hwnd, &idProc );   
	return idProc;   
}
#pragma warning(disable : 26409)
#pragma warning(disable : 26400)
#define DF_DUPULICATION   //중복실행
BOOL CAxisApp::InitInstance()
{
	// Parse command line for standard shell commands, DDE, file open
	// Interface 포인터 넘겨주기

	Axis::Initialize();

#ifdef _DEBUG
	const char* defaultRegKey = "IBK_TEST";				// 개발		IBK_STAFF 개발/직원 
#else  // _RELEASE
	const char* defaultRegKey = "IBK투자증권MAC";	// REAL		IBKMAC_STAFF REAL/직원
#endif // _DEBUG
	
	SetOtherCore();

	m_regkey = defaultRegKey;

#ifndef _DEBUG
	TCHAR chFileName[500];
	
	GetModuleFileName(NULL,chFileName,MAX_PATH);
	
	CString strFilePath;
	strFilePath.Format(_T("%s"),chFileName);
	
	if(strFilePath.Find("STAFF") > -1)
	{
		m_regkey = "IBKMAC_STAFF";  //IBK_STAFF_XECURE  //IBKMAC_STAFF
	}
#endif

	CCommParam cmdInfo;
	ParseCommandLine(cmdInfo);
	const MiniDumper mDump(_T("MiniDump"));
	if (!AfxSocketInit())
	{
		Axis::MessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	if (m_regkey.IsEmpty())
		m_regkey = AXISKEY;

	if (m_instance == NULL)
		m_instance = new CAxInstance(m_regkey);

	#ifndef _DEBUG

#ifdef DF_DUPULICATION
	if (!m_instance->IsFirstInstance())
	{
		//		// 이미 실행된 것이 있으면 해당 프로그램을 활성화 시킴
		CWnd* wnd = NULL;
		if (m_regkey == "IBK투자증권MAC")
			wnd = CWnd::FindWindow(NULL, "IBK hot Trading");
		else if (m_regkey == "IBKMAC_STAFF")
			wnd = CWnd::FindWindow(NULL, "[직원]IBK hot Trading");
		else if (m_regkey == "IBK")
			wnd = CWnd::FindWindow(NULL, "IBK개발");
		else if (m_regkey == "IBK_STAFF")
			wnd = CWnd::FindWindow(NULL, "[직원]IBK개발");
		else if (m_regkey == "IBK_XECURE")
			wnd = CWnd::FindWindow(NULL, "IBK hot Trading");

		CString slog;
		slog.Format("[axis] m_regkey=[%s] wnd=[%x] \r\n ", m_regkey, wnd);
		OutputDebugString(slog);
		
		if (wnd && wnd->GetSafeHwnd())
		{
			const int nResult = Axis::MessageBox("HTS가 실행중입니다.  중복 실행하시겠습니까?",MB_YESNO);
			if(nResult == IDNO)
			{
				if (wnd && wnd->GetSafeHwnd())
				{
					if(wnd->IsWindowVisible())
					{
						wnd->ShowWindow(SW_RESTORE);
						wnd->SetForegroundWindow();
					}
				}
				return FALSE;
			}
		}
	}
#else
	if (!m_instance->IsFirstInstance())
	{

		// 이미 실행된 것이 있으면 해당 프로그램을 활성화 시킴
		CWnd* wnd = NULL;
		if (m_regkey == "IBK투자증권MAC")
		{
			wnd = CWnd::FindWindow(NULL, "IBK hot Trading");
		}
		else if (m_regkey == "IBKMAC_STAFF")
		{
			wnd = CWnd::FindWindow(NULL, "[직원]IBK hot Trading");
		}
		else if (m_regkey == "IBK")
		{
			wnd = CWnd::FindWindow(NULL, "IBK개발");
		}
		else if (m_regkey == "IBK_STAFF")
		{
			wnd = CWnd::FindWindow(NULL, "[직원]IBK개발");
		}
		else if (m_regkey == "IBK_HOT_TRADING")
		{
			wnd = CWnd::FindWindow(NULL, "IBK hot Trading");
		}

		if (wnd && wnd->GetSafeHwnd())
		{
			int nResult = Axis::MessageBox("HTS는 단일 PC에서 한 개만 실행할 수 있습니다. \r\n이전 HTS를 종료하고 다시 실행하시겠습니까?", MB_YESNO);

			if (nResult == IDYES)
			{
				ULONG processId = ProcIDFromWnd(wnd->m_hWnd);

				if (!KillPID("axis.exe", processId))
				{
					Axis::MessageBox("HTS 강제종료에 실패했습니다.수동 종료 후 다시 시작해 주세요.");
					return FALSE;
				}
			}
			else
			{
				if (wnd && wnd->GetSafeHwnd())
				{
					if (wnd->IsWindowVisible())
					{
						wnd->ShowWindow(SW_RESTORE);
						wnd->SetForegroundWindow();
					}
					//wnd->ShowWindow(SW_SHOW);
				}
				return FALSE;
			}
		}
	}
#endif
	
#endif



	AfxOleInit();
	// Create a custom control manager class so we can overide the site
	CCustomOccManager *pMgr = new CCustomOccManager;
	// Create an IDispatch class for extending the Dynamic HTML Object Model 
	m_pDispOM = new CImpIDispatch;
	// Set our control containment up but using our control container 
	// management class instead of MFC's default
	CString m_slog;
	m_slog.Format("[axis][initinstance]  m_regkey=[%s] \r\n", m_regkey);
	OutputDebugString(m_slog);

	SetRegistryKey(m_regkey);
	AfxEnableControlContainer(pMgr);

	if (!m_instance->RegisterClass(AfxGetInstanceHandle(), IDR_MAINFRAME))
	{
		if (m_pDispOM)
		{
			delete m_pDispOM;
			m_pDispOM = NULL;
		}
		return FALSE;
	}

	CoInitialize(NULL);
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
//	SetRegistryKey(m_regkey);
	//LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	// 현철과장님 요청으로 초기에 한번만 서버타임으로 PC시간 초기화로 지정
	CWinApp* app  = AfxGetApp();
	const int m_bTime = app->GetProfileInt(WORKSTATION, "OneTimeSync", 0);
	if (!m_bTime)
	{
		app->WriteProfileInt(WORKSTATION, "ServerTime", 1);	
		app->WriteProfileInt(WORKSTATION, "OneTimeSync", 1);	
	}
	//app->WriteProfileInt(WORKSTATION, "ServerTime", 0);  //vc2019? test 테스트용이니 나중에 지우기 현재 서버시간 가져와서 처리하다가 죽는다
	// PC시간 초기화 지정
	m_instance->AddDocTemplate();
	Axis::home = GetProfileString(ENVIRONMENT, ROOTDIR);
	
	m_resourceHelper = new CResourceHelper;
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	OutputDebugString("[axis]Initinstance 1");
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		CString slog;
		slog.Format("[axis] LoadFrame fail err=[%d]", GetLastError());
		OutputDebugString(slog);
		
		return FALSE;
	}
	OutputDebugString("[axis]Initinstance 2");
	m_pMainWnd = pMainFrame;

	//((CMainFrame*)m_pMainWnd)->UserFileCrypt(FALSE);

	if (!pMainFrame->Start(Axis::userID))
		return FALSE;

	pMainFrame->ShowWindow(SW_HIDE);
	pMainFrame->UpdateWindow();
	pMainFrame->PostMessage(WM_USER+500, MAKEWPARAM(3/*axAXIS*/, 0), 0);

	
	return TRUE;
}

void CAxisApp::SetOtherCore()
{
	DWORD_PTR procMask, sysMask;
	GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

	// sysMask에서 마지막(최상위) 코어 비트 찾기
	DWORD_PTR lastCoreMask = 0;
	for (DWORD_PTR bit = (DWORD_PTR)1; bit; bit <<= 1)
		if (sysMask & bit) lastCoreMask = bit;

	DWORD_PTR mainMask = sysMask & ~lastCoreMask;   // last core exclude
	if (mainMask == 0) mainMask = sysMask;          // single core fallback

	DWORD_PTR prev = SetThreadAffinityMask(GetCurrentThread(), mainMask);
	if (prev == 0)
		OutputDebugString("[wizard] SetThreadAffinityMask failed\n");

	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
}

#pragma warning(default : 26409)
#pragma warning(default : 26400)

CResourceHelper* ResourceHelper()
{
	return theApp.GetResourceHelper();
}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CString GetServerName(CString tmp);
	CAboutDlg(CWnd* pMain);

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_rts;
	CStatic	m_cpuUse;
	CString	m_serverIP;
	CString	m_serverName;
	CString	m_userID;
	CString	m_userIP;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();
	afx_msg void OnRscinfo();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CWnd		*m_pMain;
	CCpuUse		m_cpu;
	bool		m_bCpuUse;

#ifdef DF_USE_CPLUS17
	std::unique_ptr< CControlChanger> m_changer;
#else
	CControlChanger* m_changer;
#endif
protected:
	void	displayRTS();
	void	displayCPU();
};

CAboutDlg::CAboutDlg(CWnd *pMain) : CDialog(CAboutDlg::IDD)
{
	m_pMain = pMain;
	//{{AFX_DATA_INIT(CAboutDlg)
	m_serverIP = _T("");
	m_serverName = _T("");
	m_userID = _T("");
	m_userIP = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_RTS, m_rts);
	DDX_Control(pDX, IDC_CPUUSE, m_cpuUse);
	DDX_Text(pDX, IDC_SERVERIP, m_serverIP);
	DDX_Text(pDX, IDC_SERVERNAME, m_serverName);
	DDX_Text(pDX, IDC_USERID, m_userID);
	DDX_Text(pDX, IDC_USERIP, m_userIP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_RSCINFO, OnRscinfo)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef DF_USE_CPLUS17
	m_changer = std::make_unique<CControlChanger>(this);
#else
	m_changer = new CControlChanger(this);
#endif

	CString	home, file, tmpS, serviceID, siteID;
	
	CWinApp* app = AfxGetApp();
	siteID = app->GetProfileString(WORKSTATION, "SiteID");
	home = app->GetProfileString(ENVIRONMENT, ROOTDIR);
	tmpS = app->GetProfileString(WORKSTATION, SIGNONID);
	serviceID = app->GetProfileString(WORKSTATION, "serviceID");

	file.Format("%s\\%s\\yellowpage.ini", home, TABDIR);

	/*
	int pos = tmpS.Find('|');
	if (pos == -1)	
		m_userID = tmpS;
	else	
		m_userID = tmpS.Left(pos);
	*/
	m_userID = Axis::userNM;

	char szHostName[64] = {0};
	::gethostname(szHostName, sizeof(szHostName));

	if(lstrcmp(szHostName, "") != 0)
	{
#if 1
		CString	tmps, userip;
		UINT	port = atoi(app->GetProfileString(INFORMATION, "Port", Format("%d", portProxy)));
		tmps = app->GetProfileString(INFORMATION, "Server");

		CSocket	sock;
		sock.Create();
		if (sock.Connect(tmps, port))
		{
			sock.GetSockName(m_userIP, port);
			sock.Close();
		}
		else
		{
			const HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
			for (int ii = 0; lphostent; ii++)
			{
				if (!lphostent->h_addr_list[ii])
					break;
				m_userIP.Format("%u.%u.%u.%u",
					0xff & lphostent->h_addr_list[ii][0],
					0xff & lphostent->h_addr_list[ii][1],
					0xff & lphostent->h_addr_list[ii][2],
					0xff & lphostent->h_addr_list[ii][3]);
			}
		}
#else
		if(lphostent)
		{
			sprintf(szHostName, "%s", inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
			m_userIP = szHostName;
		}
#endif
	}
	
	m_serverName.Format("%s", GetServerName(serviceID));
	UpdateData(FALSE);

	displayRTS();

	m_bCpuUse = false;
	switch (m_cpu.GetPlatform())
	{
	case WIN2K_XP:	case WINNT:
		m_bCpuUse = true;	
		break;
	}
	displayCPU();
	SetTimer(CHKID, CHKTIME, NULL);

	file.Format("%s\\%s\\axis.ini", home, TABDIR);

	const UINT OnOff = GetPrivateProfileInt("RSCINFO", "show", 0, file);
	if (OnOff)
		((CWnd*)GetDlgItem(IDC_RSCINFO))->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnDestroy()
{
	CDialog::OnDestroy();

#ifndef DF_USE_CPLUS17
	delete m_changer;
#endif
}

BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


// App command to run the dialog
void CAxisApp::OnAppAbout()
{
	CAboutDlg aboutDlg(m_pMainWnd);
	aboutDlg.DoModal();
}

void CAboutDlg::OnRscinfo() 
{
	CRSCinfo	dlg;
	dlg.DoModal();
}

void CAboutDlg::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case CHKID:
		displayCPU();
		displayRTS();
		
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CAboutDlg::displayRTS()
{
	if (!m_pMain)	
		return;

	const int rts = m_pMain->SendMessage(WM_AXIS, MAKEWPARAM(axRTSQueue, 0));
	m_rts.SetWindowText(Format("%d", rts));
}

void CAboutDlg::displayCPU()
{
	if (!m_bCpuUse)	
		return;
	const DWORD processID = GetCurrentProcessId();
	m_cpuUse.SetWindowText(Format("%d %%", m_cpu.GetCpuUsage(processID)));
	
}
/////////////////////////////////////////////////////////////////////////////
// CAxisApp message handlers


int CAxisApp::ExitInstance() 
{
	// 비정상 종료 (윈도우 강제 종료시)
	if (!m_logPath.IsEmpty())
		WritePrivateProfileString("LOGCHECK",  "CLOSE", "1", m_logPath);
	//((CMainFrame*)m_pMainWnd)->UserFileCrypt(TRUE);
	m_instance->UnsetClass(AfxGetInstanceHandle());
	if (m_instance)	delete m_instance;

	CString home = GetProfileString(ENVIRONMENT, ROOTDIR);
	protectKey(home, false);

	CoUninitialize();

	Axis::Release();
	return CWinApp::ExitInstance();
}

void CAxisApp::InitKings()
{

}

void CAxisApp::FreeKings()
{

}

void CAxisApp::protectKey(CString home, bool on)
{
	if (m_sFireKey.IsEmpty())	return;
	
	CString	file;
	file.Format("%s\\%s\\axisSecure.ini", home, TABDIR);

	const UINT OnOff = GetPrivateProfileInt("DT_KEYBOARD", "protect", 1, file);
	if (!OnOff)	return;
	if (on)
	{
		CString		fileName;
		fileName.Format("%s\\exe\\SCCORE_B.DLL", home);
		m_hSCSK = LoadLibrary(fileName);
		if (!m_hSCSK)	return;
		typedef void (*pNoIcon)();
		typedef UINT (*pfLoad_B)(char* key);
		typedef UINT (*pfSetWndClassNames)(char* key);

		pfLoad_B lpLoad_B = (pfLoad_B)GetProcAddress(m_hSCSK, "Load_B");   
		pfSetWndClassNames lpSetWndClassNames = NULL;
		lpSetWndClassNames = (pfSetWndClassNames)GetProcAddress(m_hSCSK, "SetWndClassNames");  

		if (lpLoad_B != NULL)
			lpLoad_B( _LICENCE_ );
	}
	else
	{
		if(m_hSCSK)
			FreeLibrary(m_hSCSK);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	CCommParam
//

CCommParam::CCommParam()
{
}

CCommParam::~CCommParam()
{
}

void CCommParam::ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CAxisApp *app = (CAxisApp *) AfxGetApp();

	if (bFlag)
	{
		CString	tmps = lpszParam;

		if (tmps.GetLength() == 1)
		{
			switch (tmps[0])
			{
			case 'i': case 'k': case 'n': case 'p': case 's': case 'f': case 'g':
			case 'a': case 't': case 'z': case 'y':
				app->m_flag = tmps[0];
				break;
			}
		}
		return;
	}

	CString	str, tmps;
	switch (app->m_flag)
	{
	case 'i':
		Axis::userID += lpszParam;
		Axis::userID += '\t';
		break;
	case 'y':
	case 'k':
	default:
		
		app->m_regkey = lpszParam;
		break;
	case 'n':
		app->m_exeName = lpszParam;
		break;
	case 'p':
		str = lpszParam;
		if (!str.IsEmpty())
			app->m_progK = str.GetAt(0);
		break;
	case 's':
		str = lpszParam;
		if (!str.IsEmpty())
			app->m_mode = str.GetAt(0);
		break;
	case 'f':
		app->m_sFireKey = lpszParam;
		break;
	case 'g':
		app->m_eFireKey = lpszParam;
		break;
	case 'a': // force ip address for op.
	case 'z':
		app->m_forceIP = lpszParam;
		break;
	case 't': // force port number for op.
		app->m_forcePort = atoi(lpszParam);
		break;
	}
}


CString CAboutDlg::GetServerName(CString tmp)
{
	tmp.TrimLeft();
	tmp.TrimRight();
	tmp.Remove(0);

	CString	Path;
	Path.Format("%s\\%s\\SERVER.INI", Axis::home, "tab");
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("NAME",tmp," ",readB,sizeof(readB),Path);
	
	CString tName(readB,readL);
	tName.TrimLeft();tName.TrimRight();

	tmp = tName;
	
	return tmp;
}


//#ifndef _DEBUG
//	if (!m_instance->IsFirstInstance())
//	{
//
//		// 이미 실행된 것이 있으면 해당 프로그램을 활성화 시킴
//		CWnd* wnd = NULL;
//		if (m_regkey == "IBK투자증권MAC")
//		{
//			wnd = CWnd::FindWindow(NULL, "IBK hot Trading");
//		}
//		else if (m_regkey == "IBKMAC_STAFF")
//		{
//			wnd = CWnd::FindWindow(NULL, "[직원]IBK hot Trading");
//		}
//		else if (m_regkey == "IBK")
//		{
//			wnd = CWnd::FindWindow(NULL, "IBK개발");
//		}
//		else if (m_regkey == "IBK_STAFF")
//		{
//			wnd = CWnd::FindWindow(NULL, "[직원]IBK개발");
//		}
//
//		if (wnd && wnd->GetSafeHwnd())
//		{
//			const int nResult = Axis::MessageBox("HTS가 실행중입니다.이전 HTS를 종료하고 다시 실행하시겠습니까?",MB_YESNO);
//
//			if(nResult == IDYES)
//			{
//				const ULONG processId = ProcIDFromWnd(wnd->m_hWnd);
//
//				if(!KillPID("axis.exe",processId))
//				{
//					Axis::MessageBox("HTS 강제종료에 실패했습니다.수동 종료 후 다시 시작해 주세요.");
//					return FALSE;
//				}
//			}
//			else
//			{
//				if (wnd && wnd->GetSafeHwnd())
//				{
//					if(wnd->IsWindowVisible())
//					{
//						wnd->ShowWindow(SW_RESTORE);
//						wnd->SetForegroundWindow();
//					}
//				}
//				return FALSE;
//			}
//		}
//	}
//#endif