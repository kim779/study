// AxisChaser.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AxisChaser.h"
#include "chaser.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisChaserApp

BEGIN_MESSAGE_MAP(CAxisChaserApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisChaserApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisChaserApp construction

CAxisChaserApp::CAxisChaserApp()
{
	m_flag = ' ';
	m_class = _T("");
	m_TargetKey = _T("AXIS");
	m_regkey = _T("[AXIS Chaser] AXIS");
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisChaserApp object

CAxisChaserApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxisChaserApp initialization

BOOL CAxisChaserApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCommParam cmdInfo;
	ParseCommandLine(cmdInfo);
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	if (!IsFirstInstance())	return FALSE;
#ifdef NDEBUG
	if (m_class.IsEmpty())	return FALSE;
#endif

	SetRegistryKey(m_TargetKey);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

	// The one and only window has been initialized, so show and update it.
	pFrame->UpdateWindow();
	pFrame->ShowWindow(SW_HIDE);
	pFrame->PostMessage(CHASERDISPLAY);
	return TRUE;
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
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAxisChaserApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAxisChaserApp message handlers

bool CAxisChaserApp::IsFirstInstance()
{
	m_hMutex = ::OpenMutex( MUTEX_ALL_ACCESS, TRUE, m_regkey );
	if ( m_hMutex != NULL )
	{
		::CloseHandle( m_hMutex );
		m_hMutex = NULL;
		return false;
	}

	int nCount = 0;
	while ( m_hMutex == NULL && nCount < 5 ) 
	{
		m_hMutex = ::CreateMutex(NULL, FALSE, m_regkey );
		nCount++;
	}
	return true;
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
	CAxisChaserApp *app = (CAxisChaserApp *) AfxGetApp();

	if (bFlag)
	{
		CString	tmps = lpszParam;

		if (!tmps.CompareNoCase("c"))
			app->m_flag = 'c';
		if (!tmps.CompareNoCase("r"))
			app->m_flag = 'r';
		return;
	}

	CString	str;
	switch (app->m_flag)
	{
	case 'c':
		app->m_class = lpszParam;
		break;
	case 'r':
		app->m_TargetKey = lpszParam;
		app->m_regkey.Format("[AXIS Chaser] %s", lpszParam);
		break;
	}
}