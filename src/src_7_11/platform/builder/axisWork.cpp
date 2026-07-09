// axisWork.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "axisWork.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "../h/axisvar.h"
#include "mainvar.h"
#include "Components/Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWorkApp

BEGIN_MESSAGE_MAP(CWorkApp, CWinApp)
	//{{AFX_MSG_MAP(CWorkApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_HELP, OnAppHelp)
	ON_UPDATE_COMMAND_UI(ID_APP_HELP, OnUpdateAppHelp)
	ON_COMMAND(ID_APP_AXHELP, OnAppAxhelp)
	ON_COMMAND(ID_APP_VBHELP, OnAppVbhelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkApp construction

CWorkApp::CWorkApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_strKeywordsLower = " Call Const Dim Do Loop Erase Exit For To Step Next "
		"Each Function If Then Else ElseIf On Error Err Resume Option Explicit Private Public Randomize "
		"ReDim Rem Select Case Set Sub End While Wend "
		"And Eqv Imp Is Mod Not Or Xor "
		"MsgBox InputBox With LoadPicture "
		"Abs Int Fix Round Asc AscB AscW Chr ChrB ChrW CBool CByte CCur CDate CDbl CInt CLng CSng CStr DateSerial DateValue Hex Oct Fix Int Sgn TimeSerial TimeValue "
		"Date Time  DateAdd DateDiff DatePart DateSerial DateValue Day Month MonthName Weekday WeekdayName Year Hour Minute Second Now TimeSerial TimeValue "
		"Class Eval Excute Regplace Test FormatCurrency FormatDateTime FormatNumber FormatPercent "
		"Atn Cos Sin Tan Exp Log Sqr Randomize Rnd "
		"Filter InStr InStrB InStrRev Join Len LenB LCase UCase Left LeftB Mid MidB Right RightB Replace Space Split StrComp String StrReverse LTrim RTrim Trim "
		"IsArray IsDate IsEmpty IsNull IsNumeric IsObject TypeName VarType ";
	
	m_strConstantsLower = " Empty False True Nothing Null "
		"vbBlack vbRed vbGreen vbYellow vbBlue vbMagenta vbCyan vbWhite "
		"vbBinaryCompare vbTextCompare vbDatabaseCompare "
		"vbGeneralDate vbLongDate vbShortDate vbLongTime vbShortTime "
		"vbSunday vbMonday vbTuesday vbWednesday vbThursday vbFriday vbSaturday "
		"vbFirstJan1 vbFirstFourDays vbFirstFullWeek vbUseSystem vbUseSystemDayOfWeek "
		"vbObjectError vbOKOnly vbOKCancel vbAbortRetryIgnore vbYesNoCancel vbYesNo "
		"vbRetryCancel vbCritical vbQuestion vbExclamation vbInformation "
		"vbDefaultButton1 vbDefaultButton2 vbDefaultButton3 vbDefaultButton4 "
		"vbApplicationModal vbSystemModal "
		"vbCr vbCrLf vbFormFeed vbLf vbNewLine vbNullChar vbNullString vbTab vbVerticalTab "
		"vbEmpty vbNull vbInteger vbLong vbSingle vbDouble vbCurrency vbDate vbString "
		"vbObject vbError vbBoolean vbVariant vbDataObject vbDecimal vbByte vbArray ";

	m_strKeywordsLower.MakeLower();
	m_strConstantsLower.MakeLower();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWorkApp object

CWorkApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWorkApp initialization

BOOL CWorkApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox("Ole initialize Error!");
		return FALSE;
	}
	CoInitialize(NULL);



	long	ReleaseCount;
	CString	apps;

	apps  = m_pszAppName;
	apps += _T("_XXBuilder");
	m_hSemaphore = CreateSemaphore(NULL, 0, 1, apps);
	switch (GetLastError())
	{
	case 0:
	case ERROR_ALREADY_EXISTS:
		if (!ReleaseSemaphore(m_hSemaphore, 1, &ReleaseCount))
		{
			CloseHandle(m_hSemaphore);
			m_hSemaphore = NULL;
			return FALSE;
		}
		break;
	default:
		break;
	}

	AfxEnableControlContainer();
	EnableTaskbarInteraction();
	AfxInitRichEdit2();

#ifdef _DEBUG
	SetCurrentDirectory("E:/AxisBuilder/exe");
#endif

	CSplashWnd::EnableSplashScreen(true);



	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

/*
	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
*/
	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
//	CMultiDocTemplate* pDocTemplate;
//	pDocTemplate = new CMultiDocTemplate(IDR_AxBuilderTYPE,
//		RUNTIME_CLASS(CAxBuilderDoc),
//		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
//		RUNTIME_CLASS(CAxBuilderView));
//	if (!pDocTemplate)
//		return FALSE;
//	AddDocTemplate(pDocTemplate);


	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame || !pFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pFrame;
		return FALSE;
	}
	m_pMainWnd = pFrame;


	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 

	//pFrame->m_hMenuDefault=((CMainFrame*)pFrame)->NewDefaultMenu();
	CMenu* pMenu = m_pMainWnd->GetMenu();
	HMENU hMenu = ((CMainFrame*) m_pMainWnd)->NewDefaultMenu();

	if (pMenu)
		pMenu->DestroyMenu();

	pMenu = CMenu::FromHandle(hMenu);
	m_pMainWnd->SetMenu(pMenu);
	((CMainFrame*)m_pMainWnd)->m_hMenuDefault = hMenu;
	

	// The main window has been initialized, so show and update it.
	pFrame->SetWindowText(_T("AXIS Builder"));
	pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();
	m_nGap = 5;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkApp message handlers

int CWorkApp::ExitInstance() 
{
	m_wndMDIClient.Detach();
	CloseHandle(m_hSemaphore);
	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
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
void CWorkApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWorkApp message handlers



bool CWorkApp::RegisterCtrl(CString name)
{
	HINSTANCE	handle;
	CString		path;
	path.Format("./%s", name);

	handle = LoadLibrary(path);
	if (handle)
	{
		FARPROC	lpDllEntryPoint;
		(FARPROC&)lpDllEntryPoint = GetProcAddress(handle,
			_T("DllRegisterServer"));
		if (lpDllEntryPoint != NULL)
			(*lpDllEntryPoint)();
		
		FreeLibrary(handle);
		return true;
	}
	return false;
}

bool CWorkApp::UnRegisterCtrl(CString name)
{
	HINSTANCE	handle;
	CString		path;
	path.Format("./%s", name);
	handle = LoadLibrary(path);

	if (handle)
	{
		FARPROC	lpDllEntryPoint;
		(FARPROC&)lpDllEntryPoint = GetProcAddress(handle,
			_T("DllUnregisterServer"));
		if (lpDllEntryPoint != NULL)
			(*lpDllEntryPoint)();
		
		FreeLibrary(handle);	
		return true;
	}
	else
		AfxMessageBox("Failed to unregister " + name + "!!");
	return false;
}

int CWorkApp::setLock(CWnd *pWnd)
{
	CWnd *ret;
	if (m_LockMap.GetCount())
	{
		int idx = 0;
		for (idx = 0; m_LockMap.Lookup(idx, ret); idx++);
			m_LockMap.SetAt(idx, pWnd);
		return idx;
	}
	return -1;
}

void CWorkApp::releaseLock()
{
	POSITION pos = m_LockMap.GetStartPosition();
	if (pos)
	{
		CWnd *value;
		int key;
		m_LockMap.GetNextAssoc(pos, key, value);
		value->PostMessage(WM_USER);
		m_LockMap.RemoveKey(key);
	}
}

void CWorkApp::OnAppHelp() 
{
	CString rootPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, 16/*ID_USR_GETROOT*/, 0);
	CString sWord = (char*)AfxGetMainWnd()->SendMessage(WM_USER, 35/*ID_USR_CURRENTWORD*/, 0);
	if (!sWord.IsEmpty())
	{
		HH_AKLINK link;
		link.cbStruct =     sizeof(HH_AKLINK) ;
		link.fReserved =    FALSE ;
		link.pszKeywords =  sWord.operator LPCTSTR(); 
		link.pszUrl =       NULL ; 
		link.pszMsgText =   NULL ; 
		link.pszMsgTitle =  NULL ; 
		link.pszWindow =    NULL ;
		link.fIndexOnFail = TRUE ;
		
		if (IsKeyword(sWord) >= 0|| IsConstant(sWord) >= 0)
		{
			HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
				rootPath + "\\help\\VBS55.chm",
				HH_KEYWORD_LOOKUP,
				(DWORD)&link);
		}
		else
		{
			HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
				rootPath + "\\help\\axisBuilder.chm",
				HH_KEYWORD_LOOKUP,
				(DWORD)&link);
		}
	}
	else
	{
		HWND hwnd = ::HtmlHelp(
			GetDesktopWindow(),
			rootPath + "\\help\\axisBuilder.chm",
			HH_DISPLAY_TOPIC,
			NULL);
	}
}

void CWorkApp::OnUpdateAppHelp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

int CWorkApp::IsKeyword(LPCTSTR lpszSymbol)
{
	CString strSymbol; strSymbol.Format(" %s ", lpszSymbol);
	strSymbol.MakeLower();
				
	return m_strKeywordsLower.Find(strSymbol);
}

int CWorkApp::IsConstant(LPCTSTR lpszSymbol)
{
	CString strSymbol; strSymbol.Format(" %s ", lpszSymbol);
	strSymbol.MakeLower();
				
	return m_strConstantsLower.Find(strSymbol);
}

void CWorkApp::setRegistryKey(CString sKey)
{
	if (m_pszRegistryKey)
		free((void*)m_pszRegistryKey);
	m_pszRegistryKey = NULL;
	SetRegistryKey(sKey);
}

void CWorkApp::OnAppAxhelp() 
{
	CString rootPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, 16/*ID_USR_GETROOT*/, 0);
	char *pWord = (char*)AfxGetMainWnd()->SendMessage(WM_USER, 35/*ID_USR_CURRENTWORD*/, 0);
	if (pWord)
	{
		HH_AKLINK link;
		link.cbStruct =     sizeof(HH_AKLINK) ;
		link.fReserved =    FALSE ;
		link.pszKeywords =  (const char*)pWord ; 
		link.pszUrl =       NULL ; 
		link.pszMsgText =   NULL ; 
		link.pszMsgTitle =  NULL ; 
		link.pszWindow =    NULL ;
		link.fIndexOnFail = TRUE ;
		
		HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
			rootPath + "\\help\\axisBuilder.chm",
			HH_KEYWORD_LOOKUP,
			(DWORD)&link);
	}
	else
	{
		HWND hwnd = ::HtmlHelp(
			GetDesktopWindow(),
			rootPath + "\\help\\axisBuilder.chm",
			HH_DISPLAY_TOPIC,
			NULL);
	}	
}

void CWorkApp::OnAppVbhelp() 
{
	CString rootPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, 16/*ID_USR_GETROOT*/, 0);
	char *pWord = (char*)AfxGetMainWnd()->SendMessage(WM_USER, 35/*ID_USR_CURRENTWORD*/, 0);
	if (pWord)
	{
		HH_AKLINK link;
		link.cbStruct =     sizeof(HH_AKLINK) ;
		link.fReserved =    FALSE ;
		link.pszKeywords =  (const char*)pWord ; 
		link.pszUrl =       NULL ; 
		link.pszMsgText =   NULL ; 
		link.pszMsgTitle =  NULL ; 
		link.pszWindow =    NULL ;
		link.fIndexOnFail = TRUE ;
		
		HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
			rootPath + "\\help\\VBS55.chm",
			HH_KEYWORD_LOOKUP,
			(DWORD)&link);
	}
	else
	{
		HWND hwnd = ::HtmlHelp(
			GetDesktopWindow(),
			rootPath + "\\help\\VBS55.chm",
			HH_DISPLAY_TOPIC,
			NULL);
	}
}

CMDIChildWnd* CWorkApp::GetNextMDIChildWnd()
{
	if (!::IsWindow(m_wndMDIClient.m_hWnd))
		return NULL;
	if (!m_pWndCurrentChild)
		m_pWndCurrentChild = m_wndMDIClient.GetWindow(GW_CHILD);
	else
		m_pWndCurrentChild=
			(CMDIChildWnd*)m_pWndCurrentChild->GetWindow(GW_HWNDNEXT);
	
	if (!m_pWndCurrentChild)
		return NULL;
	
	// Check the kind of window
	if (!m_pWndCurrentChild->GetWindow(GW_OWNER))
	{
		if (m_pWndCurrentChild->
			IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
			return (CMDIChildWnd*)m_pWndCurrentChild;
		else
			return GetNextMDIChildWnd();
	}
	else
		return GetNextMDIChildWnd();
}


void CWorkApp::PreLoadState()
{
	/*
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
	*/
}

void CWorkApp::LoadCustomState()
{
}

void CWorkApp::SaveCustomState()
{
}
