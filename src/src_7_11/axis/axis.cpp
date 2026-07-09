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
#include "../h/axisvar.h"

#define	SERVERIP	"Server_IP"
#define	SERVERURL	"Server_URL"
#define	YELLOWPAGE	"YellowPage.ini"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	_AFX_NO_DEBUG_CRT	1

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
	m_flag = 0;
	m_user = _T("");
	m_regkey = _T("");
	m_progK = MD_DEV;
}

CAxisApp::~CAxisApp()
{

}
/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisApp object

CAxisApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxisApp initialization

BOOL CAxisApp::InitInstance()
{
	// Parse command line for standard shell commands, DDE, file open
	CCommParam cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

#ifndef _DEBUG
	char temp_path[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), temp_path, sizeof(temp_path));

	if(CString(temp_path, MAX_PATH).TrimRight().Find("Work") >= 0)
		m_regkey = "KBond_Work";
	else
	{
		if(m_regkey.IsEmpty())
			m_regkey = AXISKEY;
	}
#endif

	//if (m_regkey.IsEmpty())
	//	m_regkey = AXISKEY;

	if (m_instance == NULL)
		m_instance = new CAxInstance(m_regkey);

 	if (!m_instance->IsFirstInstance())
 		return FALSE;

	if (!m_instance->RegisterClass(AfxGetInstanceHandle(), IDR_MAINFRAME))
		return FALSE;
	
	AfxOleInit();
	AfxEnableControlContainer();

	CoInitialize(NULL);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(m_regkey);  
	//LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_instance->AddDocTemplate();
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	if (!pMainFrame->Start(m_user))
		return FALSE;

	COLORREF	clrDialog = 0, clrText = 0;

	pMainFrame->ShowWindow(SW_HIDE);
	pMainFrame->UpdateWindow();
	pMainFrame->PostMessage(WM_USER+500, MAKEWPARAM(3/*axAXIS*/, 0), 0);

	return TRUE;
}

void CAxisApp::ExceptionInformation(CString strInfo)		// elog
{
	((CMainFrame*)m_pMainWnd)->ErrReport(strInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
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
	afx_msg void OnRscinfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
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
	DDX_Text(pDX, IDC_SERVERIP, m_serverIP);
	DDX_Text(pDX, IDC_SERVERNAME, m_serverName);
	DDX_Text(pDX, IDC_USERID, m_userID);
	DDX_Text(pDX, IDC_USERIP, m_userIP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_RSCINFO, OnRscinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CString Parser(CString &srcstr, CString substr)
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
	return _T("");
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString	home, file, tmpS, serviceID;
	CString	tmps, userip;
	CWinApp* app = AfxGetApp();
	home = app->GetProfileString(ENVIRONMENT, ROOTDIR);
	tmpS = app->GetProfileString(WORKSTATION, SIGNONID);
	serviceID = app->GetProfileString(WORKSTATION, "serviceID");
	file.Format("%s\\%s\\yellowpage.ini", home, TABDIR);

	int pos = tmpS.Find('|');
	if (pos == -1)	
		m_userID = tmpS;
	else	m_userID = tmpS.Left(pos);

	char szHostName[64] = {0};
	::gethostname(szHostName, sizeof(szHostName));
	if(lstrcmp(szHostName, "") != 0)
	{
#if 1
		UINT	port = atoi(app->GetProfileString(INFORMATION, "Port", "80"));
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
			HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
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
	
	//m_serverName.Format("#%s", serviceID);
	m_serverName.Format("%s", Parser(tmps, "|"));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CAxisApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAxisApp message handlers


int CAxisApp::ExitInstance() 
{
	m_instance->UnsetClass(AfxGetInstanceHandle());
	if (m_instance)	delete m_instance;

	CoUninitialize();
	return CWinApp::ExitInstance();
}

//*****************************************************************************
// CNoCrashApp::ExceptionInformation()
// Purpose : [ 에러가 발생하였을 때 호출되는 가상 함수 입니다. ]
// Parameters :	
// [strInfo] :
// Returns :	none
//*****************************************************************************
//void CAxisApp::ExceptionInformation(CString strInfo)
//{
//	((CMainFrame*)m_pMainWnd)->ErrReport(strInfo);
//}

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
		if (!tmps.CompareNoCase("i"))
			app->m_flag = 'i';
		else if (!tmps.CompareNoCase("k"))
			app->m_flag = 'k';
		else if (!tmps.CompareNoCase("n"))
			app->m_flag = 'n';
		else if (!tmps.CompareNoCase("p"))
			app->m_flag = 'p';
		else if (!tmps.CompareNoCase("s"))
			app->m_flag = 's';
		else if (!tmps.CompareNoCase("f"))
			app->m_flag = 'f';
		else if (!tmps.CompareNoCase("g"))
			app->m_flag = 'g';
		return;
	}

	CString	str, tmps;
	switch (app->m_flag)
	{
	case 'i':
		app->m_user += lpszParam;
		app->m_user += '\t';
		break;
	case 'k':
	default:
		if (app->m_regkey.IsEmpty())
			app->m_regkey = lpszParam;
		else
		{
			app->m_regkey += " ";
			app->m_regkey += lpszParam;
		}
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
	}

}

void CAboutDlg::OnRscinfo() 
{
	CWinApp* app = AfxGetApp();
	CString	home = app->GetProfileString(ENVIRONMENT, ROOTDIR);

	CRSCinfo	dlg(home);
	dlg.DoModal();
}

