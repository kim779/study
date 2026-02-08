// SystemTrading.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SystemTrading.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SystemTradingDoc.h"
#include "SystemTradingView.h"

#include "BrowserView.h"

#include "../include_ST/DeclareDefine.h"

#ifdef _XTP_INCLUDE_CONTROLS
#include "AboutDlg.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingApp

BEGIN_MESSAGE_MAP(CSystemTradingApp, CWinApp)
	//{{AFX_MSG_MAP(CSystemTradingApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_XTP_EXECUTE(ID_GOTO_URL, OnGotoURL)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingApp construction

CSystemTradingApp::CSystemTradingApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bDebugCompile = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSystemTradingApp object

CSystemTradingApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingApp initialization

BOOL CSystemTradingApp::InitInstance()
{
	if(FindSystemTrading())
	{

		return FALSE;
	}
	if (!AfxSocketInit())
	{
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
	SetRegistryKey(_T("Solomon SystemTrading"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	SetRootPath();

	VerifySystemTradingFolder();
	CopyBaseScriptFile();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pDocTemplate = new CMultiDocTemplate(
		IDR_GUI_VITYPE,
		RUNTIME_CLASS(CSystemTradingDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSystemTradingView));
	AddDocTemplate(m_pDocTemplate);

	m_pDocTemplateBrowserPage = new CMultiDocTemplate(
		IDR_GUI_VITYPE,
		RUNTIME_CLASS(CSystemTradingDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CBrowserView));
	AddDocTemplate(m_pDocTemplateBrowserPage);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
			
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	
//@유진삭제
//	///KHD 
//	CString FileName,Handle;
//	if(!cmdInfo.m_strFileName.IsEmpty())
//	{
//		int index = cmdInfo.m_strFileName.Find('|');
//		
//		FileName = cmdInfo.m_strFileName.Left(index);
//		cmdInfo.m_strFileName.Delete(0,index+1);
//		Handle = cmdInfo.m_strFileName;
//		sscanf(Handle,"%X", &pMainFrame->m_hPctrWnd);
//		cmdInfo.m_strFileName = FileName;
//
//	}
//	
//	if(cmdInfo.m_strFileName.IsEmpty())
//		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
//	else
//	{
//		CString strFileName(cmdInfo.m_strFileName);
////@유진삭제
////		cmdInfo.m_strFileName.Format(_T("%s%s%s"), m_szRootPath, PATH_SCRIPTFILES, strFileName);
////@유진삭제
////@유진추가
//		cmdInfo.m_strFileName.Format(_T("%s%s%s"), m_szUserPath, PATH_SCRIPTFILES2, strFileName);
////@유진추가
//		cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
//	}
//@유진삭제
//@유진추가
	CString FileName,Handle;
	CString strCmdLine = m_lpCmdLine;
	if(!strCmdLine.IsEmpty())
	{
		int index = strCmdLine.Find('|');
		strCmdLine.Delete(0,index+1);
		Handle = strCmdLine;
		sscanf(Handle,"%X", &pMainFrame->m_hPctrWnd);
	}

	if(m_szScriptFile.GetLength()==0)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	else
	{
		//CString strFileName(cmdInfo.m_strFileName);
		//cmdInfo.m_strFileName.Format(_T("%s%s%s"), m_szRootPath, PATH_SCRIPTFILES, strFileName);
		//cmdInfo.m_strFileName.Format(_T("%s%s%s"), m_szUserPath, PATH_SCRIPTFILES2, strFileName);
		cmdInfo.m_strFileName.Format(_T("%s%s%s"), m_szUserPath, PATH_SCRIPTFILES2, m_szScriptFile);
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;

		{
			CFileFind file;
			if(file.FindFile(cmdInfo.m_strFileName)==FALSE)
			{
				cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
			}
		}
	}
//@유진추가

//	cmdInfo.m_strFileName.Format(_T("%s%sSummation.uft"), m_szRootPath, PATH_SCRIPTFILES);
//	cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	m_nCmdShow = SW_SHOWMAXIMIZED;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	pMainFrame->SetConnectionStatusTest(_T("접속시도"));
	pMainFrame->ConnectToCompileServer();

	return TRUE;
}

// App command to run the dialog
void CSystemTradingApp::OnAppAbout()
{
#ifdef _XTP_INCLUDE_CONTROLS
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingApp message handlers
void CSystemTradingApp::OnGotoURL(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
		
	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)m_pDocTemplateBrowserPage->OpenDocumentFile(NULL);
		POSITION pos = pDoc->GetFirstViewPosition();
		CBrowserView* pView = (CBrowserView*)pDoc->GetNextView(pos);

		CString strURL = pControl->GetEditText();
		
	#if _MSC_VER >= 1200 // MFC 5.0
		pView->Navigate(strURL);
	#endif
		pDoc->SetTitle(strURL);
	}
	*pResult = 1;
}

void CSystemTradingApp::SetRootPath()
{
	::GetModuleFileName(AfxGetInstanceHandle(), m_szRootPath, sizeof(m_szRootPath));
	int i = 0;
	for(int j = 0; j < 2; j++)
	{
		for( i = ::strlen(m_szRootPath)-1; i >= 0 && m_szRootPath[i] != '\\'; i--);
		m_szRootPath[i] = 0x00;
	}		
}

void CSystemTradingApp::VerifySystemTradingFolder()
{
	CString szCmdLine = m_lpCmdLine;
	CString szUserDir, szScriptFile;
	if(szCmdLine.GetLength()>0)
	{
		int nFind = szCmdLine.Find("/U");
		int nSubFind;
		if(nFind>=0)
		{
			szUserDir = szCmdLine.Mid(nFind+3);
			nSubFind = szUserDir.Find("\"");
			szUserDir = szUserDir.Left(nSubFind);
			szUserDir.Remove('\"');

			sprintf(m_szUserPath, "%s", szUserDir);
		}

		nFind = szCmdLine.Find("/S");
		if(nFind>=0)
		{
			szScriptFile = szCmdLine.Mid(nFind+3);
			nSubFind = szScriptFile.Find("\"");
			szScriptFile = szScriptFile.Left(nSubFind);
			szScriptFile.Remove('\"');
			m_szScriptFile = szScriptFile;
		}
	}
	else
	{
		// Default 디렉토리를 User로.
		sprintf(m_szUserPath, "%s%s", m_szRootPath, "\\User");
	}
	::SetEnvironmentVariable(KEY_USERDIRECTORY, szUserDir);

	char szFolder[MAX_PATH] = {NULL, };

	::wsprintf(szFolder, _T("%s%s"), m_szUserPath, PATH_USER2);
	::CreateDirectory(szFolder, NULL);

	::lstrcat(szFolder, _T("STConfig.cfg"));
	m_bDebugCompile = (BOOL)::GetPrivateProfileInt(_T("DEVELOPER"), _T("DEBUG"), m_bDebugCompile, szFolder);

//@유진삭제::wsprintf(szFolder, _T("%s%s"), m_szRootPath, PATH_DLLFILES);
	::wsprintf(szFolder, _T("%s%s"), m_szUserPath, PATH_DLLFILES2);
	::CreateDirectory(szFolder, NULL);

	if(m_bDebugCompile)
	{
		::lstrcat(szFolder, _T("D"));
		::CreateDirectory(szFolder, NULL);
	}


//@유진삭제::wsprintf(szFolder, _T("%s%s"), m_szRootPath, PATH_SCRIPTFILES);
	::wsprintf(szFolder, _T("%s%s"), m_szUserPath, PATH_SCRIPTFILES2);
	::CreateDirectory(szFolder, NULL);

	::wsprintf(szFolder, _T("%s%s"), m_szUserPath, PATH_STFILEFOLDER);
	::CreateDirectory(szFolder, NULL);

	::wsprintf(szFolder, _T("%s%s"), m_szUserPath, PATH_STFILES2);
	::CreateDirectory(szFolder, NULL);

	::wsprintf(szFolder, _T("%s%sOutput"), m_szRootPath, PATH_STDATA);
	::CreateDirectory(szFolder, NULL);
}

BOOL CSystemTradingApp::FindSystemTrading()
{
	HWND hSystemTrading = ::FindWindow(NULL, _T("IBK SystemTrading"));
	if(hSystemTrading)
	{
		if(::IsIconic(hSystemTrading))
			::ShowWindow(hSystemTrading, SW_RESTORE);
		
		::SetForegroundWindow(hSystemTrading);
		return TRUE;
	}

	return FALSE;
}

short CSystemTradingApp::CopyBaseScriptFile()
{
	char szConfigFile[MAX_PATH] = {NULL, };
	::wsprintf(szConfigFile, _T("%s%sSTConfig.cfg"), GetRootPath(), PATH_STDATA);

	LPCSTR szSecKey = "LIST";

	int nCopyNO = ::GetPrivateProfileInt(szSecKey, "CopyNo", 0, szConfigFile);
	if(nCopyNO==100) return 0;

	int nCnt = ::GetPrivateProfileInt(szSecKey, "Count", 0, szConfigFile);

	char aFileFrom[MAX_PATH]={0,},  aFileTo[MAX_PATH]={0,};
	char aKeyFileName[MAX_PATH]={0,};
	char aKey[5];
	for(int i=0; i<nCnt; i++)
	{
		sprintf(aKey, "%d", i+1);
		if(::GetPrivateProfileString(szSecKey, aKey, "", aKeyFileName, MAX_PATH, szConfigFile)>0)
		{
			//@Solomon sprintf(aFileFrom,	"%s\\Data%s%s\\%s", m_szRootPath, PATH_SCRIPTFILES2, "high", aKeyFileName);
			//sprintf(aFileFrom,	"%s\\Hanaro%s%s\\%s", m_szRootPath, PATH_SCRIPTFILES2, "high", aKeyFileName);	//SOLOMON
			sprintf(aFileFrom,	"%s\\Icss%s%s\\%s", m_szRootPath, PATH_SCRIPTFILES2, "high", aKeyFileName);	//IBK
			sprintf(aFileTo,	"%s%s%s", m_szUserPath, PATH_SCRIPTFILES2, aKeyFileName);
			CopyFile(aFileFrom, aFileTo, TRUE);
		}
	}
	return nCnt;
}
