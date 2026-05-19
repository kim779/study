
// axisMBuilder.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "axisMBuilder.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "splashWnd.h"

#include "h/mainvar.h"

#include <GdiPlus.h>
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CaxisMBuilderApp

BEGIN_MESSAGE_MAP(CaxisMBuilderApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CaxisMBuilderApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_HELP_AXHELP, &CaxisMBuilderApp::OnHelpAxhelp)
	ON_COMMAND(ID_HELP_VBHELP, &CaxisMBuilderApp::OnHelpVbhelp)
END_MESSAGE_MAP()


// CaxisMBuilderApp 생성

CaxisMBuilderApp::CaxisMBuilderApp()
{
	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Winix.AxisMBuilder.0001"));

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

// 유일한 CaxisMBuilderApp 개체입니다.

CaxisMBuilderApp theApp;


// CaxisMBuilderApp 초기화

BOOL CaxisMBuilderApp::InitInstance()
{
	long	ReleaseCount;
	CString	apps;

	apps  = m_pszAppName;
	apps += _T("_XMBuilder");
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
	// SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	// LoadStdProfileSettings(0);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	InitContextMenuManager();
	InitKeyboardManager();
	InitShellManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	SetRegistryKey(_T("amBuilder"));
	//CWinAppEx::CleanState();

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

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->UpdateWindow();

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	return TRUE;
}

int CaxisMBuilderApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);
	GdiplusShutdown(m_gdiplusToken);
	return CWinAppEx::ExitInstance();
}


// CaxisMBuilderApp 메시지 처리기

void CaxisMBuilderApp::OnHelpContents()
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
						(DWORD)&link);
		}
		else
		{
			path += _T("\\help\\axisBuilder.chm");
			HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
						path,
						HH_KEYWORD_LOOKUP,
						(DWORD)&link);
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


void CaxisMBuilderApp::OnHelpAxhelp()
{
	CString path = (CString)(char *)m_pMainFrame->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	//LPCTSTR	pWord = (LPCTSTR)m_pMainFrame->SendMessage(WM_USER, ID_USR_CURRENTWORD, 0);
	path += _T("\\help\\M_builder_Help.chm");
	
	/*if (pWord)
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
					(DWORD)&link);
	}
	else
	{*/

	ShellExecute(NULL, "open", "explorer", path, NULL, SW_SHOW); 
	
	//	HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
	//			path,
	//			HH_DISPLAY_TOPIC,
	//			NULL);
		
	//}	
}

void CaxisMBuilderApp::OnHelpVbhelp()
{
	CString	path = (CString)(char *)m_pMainFrame->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	LPCTSTR	pWord = (LPCTSTR)m_pMainFrame->SendMessage(WM_USER, ID_USR_CURRENTWORD, 0);
	path += _T("\\help\\M_Builder_Lua_Help.chm");
	/*
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
					(DWORD)&link);
	}
	else
	{
		HWND hwnd = ::HtmlHelp(GetDesktopWindow(),
				path,
				HH_DISPLAY_TOPIC,
				NULL);
	//}*/
	ShellExecute(NULL, "open", "explorer", path, NULL, SW_SHOW); 
}

int CaxisMBuilderApp::IsKeyword(LPCTSTR lpszSymbol)
{
	CString strSymbol;

	strSymbol.Format(_T(" %s "), lpszSymbol);
	strSymbol.MakeLower();
	return m_strKeywordsLower.Find(strSymbol);
}

int CaxisMBuilderApp::IsConstant(LPCTSTR lpszSymbol)
{
	CString strSymbol;
	
	strSymbol.Format(_T(" %s "), lpszSymbol);
	strSymbol.MakeLower();
	return m_strConstantsLower.Find(strSymbol);
}

// CaxisMBuilderApp 사용자 지정 로드/저장 메서드

void CaxisMBuilderApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CaxisMBuilderApp::LoadCustomState()
{
}

void CaxisMBuilderApp::SaveCustomState()
{
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
void CaxisMBuilderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
