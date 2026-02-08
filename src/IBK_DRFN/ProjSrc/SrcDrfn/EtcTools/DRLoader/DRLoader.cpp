// DRLoader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DRLoader.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDRLoaderApp

BEGIN_MESSAGE_MAP(CDRLoaderApp, CWinApp)
	//{{AFX_MSG_MAP(CDRLoaderApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_BUTTON32772, OnButton32772)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDRLoaderApp construction

CDRLoaderApp::CDRLoaderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDRLoaderApp object

CDRLoaderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDRLoaderApp initialization

BOOL CDRLoaderApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

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

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("DRFN"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMDIFrameWnd* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_SOLMAITYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_SOLMAITYPE));

	// The main window has been initialized, so show and update it.
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	//
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if(cmdInfo.m_strFileName.IsEmpty() == FALSE && cmdInfo.m_strFileName.Right(4) == ".dll")
	{
		CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

		// create a new MDI child window
		CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_SOLMAITYPE, m_hMDIMenu, m_hMDIAccel);

		pChild->m_wndView._DoOpenDllFile(cmdInfo.m_strFileName);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDRLoaderApp message handlers

int CDRLoaderApp::ExitInstance() 
{
	//TODO: handle additional resources you may have added
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

void CDRLoaderApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_SOLMAITYPE, m_hMDIMenu, m_hMDIAccel);
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
void CDRLoaderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDRLoaderApp message handlers
void CDRLoaderApp::OnButton32772() 
{
	char* DllName[] = {"SI035000.dll","SI035100.dll","SI035200.dll","SI035300.dll","SI035400.dll",
						"SI035500.dll","SI035600.dll","SI035700.dll",};
	CPoint pt;
	::GetCursorPos(&pt);

	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, 1000, "조건검색 - SI035000");
	menu.AppendMenu(MF_STRING, 1001, "조건검색실시간 - SI035100");
	menu.AppendMenu(MF_STRING, 1002, "조건검색성과분석 - SI035200");
	menu.AppendMenu(MF_STRING, 1003, "빠른종목검색 - SI035300");
	menu.AppendMenu(MF_STRING, 1004, "빠른업종검색 - SI035400");
	menu.AppendMenu(MF_STRING, 1005, "패턴검색 - SI035500");
	menu.AppendMenu(MF_STRING, 1006, "차트구간검색 - SI035600");
	menu.AppendMenu(MF_STRING, 1007, "업종테마급변종목포착 - SI035700");

	int nSel = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, AfxGetMainWnd());

	if(nSel >= 1000)
	{
		int nDll = nSel - 1000;

		CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

		// create a new MDI child window
		CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_SOLMAITYPE, m_hMDIMenu, m_hMDIAccel);

		pChild->m_wndView._DoOpenDllFile(DllName[nDll]);
	}

	menu.DestroyMenu();
}
