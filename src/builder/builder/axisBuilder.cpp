
// axisBuilder.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "axisBuilder.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ChildView.h"
#include "splashWnd.h"

#include "h/mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CaxisBuilderApp

BEGIN_MESSAGE_MAP(CaxisBuilderApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CaxisBuilderApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_HELP_CONTENTS, &CaxisBuilderApp::OnHelpContents)
	ON_COMMAND(ID_HELP_AXHELP, &CaxisBuilderApp::OnHelpAxhelp)
	ON_COMMAND(ID_HELP_VBHELP, &CaxisBuilderApp::OnHelpVbhelp)
END_MESSAGE_MAP()


// CaxisBuilderApp 생성

CaxisBuilderApp::CaxisBuilderApp()
{
	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Winix.AxisBuilder.KTB64.0001"));

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

// 유일한 CaxisBuilderApp 개체입니다.

CaxisBuilderApp theApp;
// 이 식별자는 응용 프로그램에서 통계적으로 고유한 값을 가지도록 생성되었습니다.
// 특정 식별자를 선호할 경우 변경할 수 있습니다.

// {F2F82956-356E-4E4F-8E7E-27D4A40EB65A}
static const CLSID clsid =
{ 0xF2F82956, 0x356E, 0x4E4F, { 0x8E, 0x7E, 0x27, 0xD4, 0xA4, 0xE, 0xB6, 0x5A } };

const GUID CDECL _tlid = { 0xC16DAB37, 0x2322, 0x4C2E, { 0x99, 0x80, 0xF0, 0x77, 0x8B, 0x55, 0x4E, 0x77 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CaxisBuilderApp 초기화

BOOL CaxisBuilderApp::InitInstance()
{
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

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	CSplashWnd::EnableSplashScreen(true);

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("AXIS"));

	InitContextMenuManager();

	InitKeyboardManager();
	InitShellManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 모든 OLE 서버 팩터리를 실행 중으로 등록합니다. 이렇게 하면
	//  OLE 라이브러리가 다른 응용 프로그램에서 개체를 만들 수 있습니다.
	COleTemplateServer::RegisterAll();
		// 참고: MDI 응용 프로그램은 명령줄의 /Embedding 또는 /Automation에
		//  상관 없이 모든 서버 개체를 등록합니다.

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	m_pMainFrame = pMainFrame;

	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


/*	// 응용 프로그램이 /Embedding 또는 /Automation 스위치로 시작되었습니다.
	// 응용 프로그램을 자동화 서버로 실행합니다.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// 주 창을 표시하지 않습니다.
		return TRUE;
	}
	// 응용 프로그램이 /Unregserver 또는 /Unregister 스위치로 시작되었습니다. 
	// typelibrary를 등록 취소합니다. 다른 등록 취소는 ProcessShellCommand()에서 발생합니다.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT, NULL, NULL, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// 응용 프로그램이 독립 실행형으로 시작되었거나 다른 스위치로 시작되었습니다(예: /Register
	// 또는 /Regserver). typelibrary를 포함하여 레지스트리 항목을 업데이트합니다.
	else
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}
*/
	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(SW_HIDE);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CaxisBuilderApp::ExitInstance()
{
	CloseHandle(m_hSemaphore);

	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

// CaxisBuilderApp 사용자 지정 로드/저장 메서드

void CaxisBuilderApp::PreLoadState()
{
	BOOL	bNameValid;
	CString	strName;

	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CaxisBuilderApp::LoadCustomState()
{
	__super::LoadCustomState();

	// 레지스트리에서 로드한 저장된 단축키를 지우고, 리소스의 것을 사용하도록 한다.
	GetKeyboardManager()->ResetAll();
}

void CaxisBuilderApp::SaveCustomState()
{
}

// CaxisBuilderApp 메시지 처리기

void CaxisBuilderApp::OnHelpContents()
{
	CString path = (CString)(char *)m_pMainFrame->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	LPCTSTR	pWord = (LPCTSTR)m_pMainFrame->SendMessage(WM_USER, ID_USR_CURRENTWORD, 0);
	if (pWord)
	{
		HH_AKLINK link;
		link.cbStruct =     sizeof(HH_AKLINK) ;
		link.fReserved =    FALSE ;
		link.pszKeywords =  pWord; 
		link.pszUrl =       NULL ; 
		link.pszMsgText =   NULL ; 
		link.pszMsgTitle =  NULL ; 
		link.pszWindow =    NULL ;
		link.fIndexOnFail = TRUE ;

		
		if (IsKeyword(pWord) >= 0|| IsConstant(pWord) >= 0)
		{
			path += _T("\\help\\VBS55.chm");
			HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
						path,
						HH_KEYWORD_LOOKUP,
						(DWORD_PTR)&link);
		}
		else
		{
			path += _T("\\help\\axisBuilder.chm");
			HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
						path,
						HH_KEYWORD_LOOKUP,
						(DWORD_PTR)&link);
		}
	}
	else
	{
		path += _T("\\help\\axisBuilder.chm");
		HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
					path,
					HH_DISPLAY_TOPIC,
					NULL);
	}
}


void CaxisBuilderApp::OnHelpAxhelp()
{
	CString path = (CString)(char *)m_pMainFrame->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	LPCTSTR	pWord = (LPCTSTR)m_pMainFrame->SendMessage(WM_USER, ID_USR_CURRENTWORD, 0);
	path += _T("\\help\\axisBuilder.chm");
	if (pWord)
	{
		HH_AKLINK link;
		link.cbStruct =     sizeof(HH_AKLINK) ;
		link.fReserved =    FALSE ;
		link.pszKeywords =  pWord ; 
		link.pszUrl =       NULL ; 
		link.pszMsgText =   NULL ; 
		link.pszMsgTitle =  NULL ; 
		link.pszWindow =    NULL ;
		link.fIndexOnFail = TRUE ;
		
		HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
					path,
					HH_KEYWORD_LOOKUP,
					(DWORD_PTR)&link);
	}
	else
	{
		HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
				path,
				HH_DISPLAY_TOPIC,
				NULL);
	}	
}


void CaxisBuilderApp::OnHelpVbhelp()
{
	CString	path = (CString)(char *)m_pMainFrame->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	LPCTSTR	pWord = (LPCTSTR)m_pMainFrame->SendMessage(WM_USER, ID_USR_CURRENTWORD, 0);
	path += _T("\\help\\VBS55.chm");
	if (pWord)
	{
		HH_AKLINK link;
		link.cbStruct =     sizeof(HH_AKLINK) ;
		link.fReserved =    FALSE ;
		link.pszKeywords =  pWord ; 
		link.pszUrl =       NULL ; 
		link.pszMsgText =   NULL ; 
		link.pszMsgTitle =  NULL ; 
		link.pszWindow =    NULL ;
		link.fIndexOnFail = TRUE ;
		
		HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
					path,
					HH_KEYWORD_LOOKUP,
					(DWORD_PTR)&link);
	}
	else
	{
		HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
				path,
				HH_DISPLAY_TOPIC,
				NULL);
	}
}

int CaxisBuilderApp::IsKeyword(LPCTSTR lpszSymbol)
{
	CString strSymbol;

	strSymbol.Format(_T(" %s "), lpszSymbol);
	strSymbol.MakeLower();
	return m_strKeywordsLower.Find(strSymbol);
}

int CaxisBuilderApp::IsConstant(LPCTSTR lpszSymbol)
{
	CString strSymbol;
	
	strSymbol.Format(_T(" %s "), lpszSymbol);
	strSymbol.MakeLower();
	return m_strConstantsLower.Find(strSymbol);
}

void CaxisBuilderApp::SetKey(CString sKey)
{
	if (m_pszRegistryKey)
		free((void*)m_pszRegistryKey);

	m_pszRegistryKey = NULL;
	SetRegistryKey(sKey);
}


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CaxisBuilderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
