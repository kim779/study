// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SystemTrading.h"
#include "SystemTradingDoc.h"
#include "SystemTradingView.h"
#include "ChildFrm.h"
#include "LoadHelper.h"
#include "ClientSocket.h"
#include "DlgFindScript.h"
#include "Editreg.h"
#include "DlgLoadScript.h"
#include "DlgConfig.h"
#include "DlgPassword.h"

#include "MainFrm.h"
#include "DynamicHelpView.h"
#include "STPropertyGridItemColor.h"
#include "STPropertyGridItemLineFormat.h"
#include "STPropertyGridItemLineWidth.h"

#include "../Include_ST/structdefine.h"
#include "../Include_ST/MessageDefine.h"

#include "../include_ST/DeclareDefine.h"
#include "../Common_ST/STConfigDef.h"
#include "../Common_ST/SystemTradingSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_MASK	RGB(0x00, 0x80, 0x80)

void FreeString(LPCSTR p_szVariable)
{
	::free((LPVOID)p_szVariable);
}

UINT ConvertScriptFiles(LPVOID lpHelper)
{
	CLoadSystemTradingEncoder* pLoadEncoder = (CLoadSystemTradingEncoder*)lpHelper;

	//[ 사용자 경로관련 수정. 07.01.12
	KEY_GETENVVARIABLE();

	char szScriptFiles[MAX_PATH] = {NULL, };
	if(xRead==0)
		::wsprintf(szScriptFiles, _T("%s%s"), theApp.GetRootPath(), PATH_SCRIPTFILES);
	else
		::wsprintf(szScriptFiles, _T("%s%s"), xUserEncDir, PATH_SCRIPTFILES2);

//@유진삭제
//	::wsprintf(szScriptFiles, _T("%s%s"), theApp.GetRootPath(), PATH_SCRIPTFILES);
//@유진삭제
//@유진추가
	::wsprintf(szScriptFiles, _T("%s%s"), theApp.GetUserPath(), PATH_SCRIPTFILES2);
//@유진추가

	char szTemp[MAX_PATH] = {NULL, };
	::wsprintf(szTemp, _T("%sTemp"), szScriptFiles);
	::CreateDirectory(szTemp, NULL);
	
	::wsprintf(szTemp, _T("%s*.*"), szScriptFiles);

	CFileFind ff;
	BOOL bFind = ff.FindFile(szTemp);
	while(bFind)
	{
		bFind = ff.FindNextFile();
		::wsprintf(szTemp, _T("%sTemp\\%s"), szScriptFiles, ff.GetFileName());

		pLoadEncoder->EncryptFile(ff.GetFilePath(), szTemp, NULL, NULL, NULL);
		::CopyFile(szTemp, ff.GetFilePath(), FALSE);
		::DeleteFile(szTemp);
	}

	::wsprintf(szTemp, _T("%sTemp"), szScriptFiles);
	::RemoveDirectory(szTemp);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_NEW_STRATEGY, OnFileNewStrategy)
	ON_COMMAND(ID_FILE_NEW_FUNCTION, OnFileNewFunction)
	ON_COMMAND(ID_FILE_OPEN_PROJECT, OnFileOpenStrategy)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpenFunction)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSaveAll)
	ON_WM_COPYDATA()
	ON_COMMAND(IDM_CENTER, OnCenter)
	ON_COMMAND(ID_FINDANDREPLACE_FINDINFILES, OnFindandreplaceFindinfiles)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(IDM_CONNECTOSERVER, OnUpdateConnectoserver)
	ON_COMMAND(IDM_CONNECTOSERVER, OnConnectoserver)
	ON_COMMAND(IDM_TRADESTATION_VIEW, OnTradestationView)
	ON_COMMAND(IDM_YESTRADER_VIEW, OnYestraderView)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_SOLUTIONEXPLORER, ID_HELP_DYNAMICHELP, OnShowView)
	
	ON_COMMAND_RANGE(ID_FILE_NEW_PROJECT + 100, 200, OnEmptyCommandRange)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_XTP_CREATECONTROL()

	ON_XTP_EXECUTE(ID_EDIT_STATE, OnEditState)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STATE, OnUpdateEditState)
	ON_REGISTERED_MESSAGE(RMSG_DBCLICK_WORKSPACE, OnDblClickWorkspace)
	ON_REGISTERED_MESSAGE(RMSG_OPENFILE, OnDblClickTree)
	ON_REGISTERED_MESSAGE(RMSG_DELFILE, OnDelScript)
	ON_REGISTERED_MESSAGE(RMSG_COMPILEDLL, OnCompileDLL)
	ON_REGISTERED_MESSAGE(RMSG_SINGLIECOMPILEDLLMAIN, OnSingleCompileDLLMain)
	ON_REGISTERED_MESSAGE(RMSG_SINGLIECOMPILEDLLFUNCTION, OnSingleCompileDLLFunction)
	ON_REGISTERED_MESSAGE(RMSG_SINGLIECOMPILEDLLMAIN_YES, OnSingleCompileDLLMain_YES)
	ON_REGISTERED_MESSAGE(RMSG_SINGLIECOMPILEDLLFUNCTION_YES, OnSingleCompileDLLFunction_YES)
	ON_REGISTERED_MESSAGE(RMSG_VIEWPROPERTY, OnViewProperty)
	ON_REGISTERED_MESSAGE(RMSG_STCONFIGURENAME, OnCloseConfigDialog)
	ON_REGISTERED_MESSAGE(RMSG_DBLCLICKOUTPUT, OnDblClickOutput)
	ON_REGISTERED_MESSAGE(RMSG_DBLCLICKFIND, OnDblClickFind)
	ON_REGISTERED_MESSAGE(RMSG_SYSTEMTRADING, OnAnyTrader)
	ON_REGISTERED_MESSAGE(RMSG_VERIFYSCRIPTNAME, OnVerifyScriptName)
	ON_REGISTERED_MESSAGE(RMSG_STARTDRAG, OnStartDrag)
	ON_REGISTERED_MESSAGE(RMSG_SINGLIECOMPILEDLLMAIN_TREE, OnSingleCompileDLLMainTree)
	ON_REGISTERED_MESSAGE(RMSG_SINGLIECOMPILEDLLFUNCTION_TREE, OnSingleCompileDLLFunctionTree)
	ON_REGISTERED_MESSAGE(RMSG_MULTICOMPILEDLLMAIN_TREE, OnMultiCompileDLLMainTree)
	ON_REGISTERED_MESSAGE(RMSG_MULTICOMPILEDLLFUNCTION_TREE, OnMultiCompileDLLFunctionTree)
	ON_REGISTERED_MESSAGE(RMSG_GETCOMPILESTATE_TREE, OnGetCompileStateTree)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_nState = 1;
	m_nPropertyState = 0;
	m_pPreviewLayout = NULL;
	m_pLoadWorkspace = NULL;
	m_pLoadPaser = NULL;
	m_pLoadExtLib = NULL;
	m_pLoadEncoder = NULL;
	m_pLoadConfig = NULL;

	m_pPrptyScriptName = NULL;

	m_pMainClient = NULL;
	m_pFileDll = NULL;
	m_nStrategyType = 0;

	m_nCompileProgressCount = 0;
	m_bCompiling = FALSE;
	
	m_pViewHelp = NULL;
	m_bMultiCompile = FALSE;

	m_nScriptNameCount = 0;

	m_hPctrWnd = NULL;
}

CMainFrame::~CMainFrame()
{
	if(m_pFileDll)
	{
		m_pFileDll->Close();
		delete m_pFileDll;
		m_pFileDll = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
		if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

 	if(!InitCommandBars())
		return -1;

	CreateSubStatusWindow();

	LoadEncoder();
//	if(!theApp.GetProfileInt(REG_CONV_SUBKEY, REG_CONV_FLAG, 0))
//	{
//		AfxBeginThread(ConvertScriptFiles, m_pLoadEncoder);
//		theApp.WriteProfileInt(REG_CONV_SUBKEY, REG_CONV_FLAG, 1);
//	}

	// 백업파일 검사
	CheckupBackupFiles();

	// 전략 리스트
	InitStrategyList();

	// 사용자함수 리스트
	InitUserFuntionList();

	char szPath[MAX_PATH] = {NULL, };
#ifdef _DEBUG
	//@Solomon ::wsprintf(szPath, _T("%s\\Chart\\STBasicalFunc.dll"), theApp.GetRootPath());
	::wsprintf(szPath, _T("%s\\dev\\STBasicalFunc.dll"), theApp.GetRootPath());
#else
	//@Solomon ::wsprintf(szPath, _T("%s\\Chart\\STBasicalFunc.dll"), theApp.GetRootPath());
	::wsprintf(szPath, _T("%s\\dev\\STBasicalFunc.dll"), theApp.GetRootPath());
#endif
	InitInnerFunctionList(szPath);		// 내부함수 리스트

#ifdef _DEBUG
	//@Solomon ::wsprintf(szPath, _T("%s\\Chart\\TechnicalIndexFunc.dll"), theApp.GetRootPath());
	::wsprintf(szPath, _T("%s\\dev\\TechnicalIndexFunc.dll"), theApp.GetRootPath());
#else
	//@Solomon ::wsprintf(szPath, _T("%s\\Chart\\TechnicalIndexFunc.dll"), theApp.GetRootPath());
	::wsprintf(szPath, _T("%s\\dev\\TechnicalIndexFunc.dll"), theApp.GetRootPath());
#endif
	InitInnerFunctionList(szPath);		// 내부함수 리스트

	m_pLoadWorkspace = new CLoadSystemTradingWorkspaceHelper;
	if(!m_pLoadWorkspace->CreateWorkspace(this, theApp.GetRootPath(), &m_vtStrategyInfo, &m_vtUserFuncInfo))
	{
		delete m_pLoadWorkspace;
		m_pLoadWorkspace = NULL;
	}

	LoadExtLib();

	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();
	
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_GUI_VITYPE); 
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);
	
	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	
	CXTPToolBar* pBuildBar = (CXTPToolBar*)pCommandBars->Add(_T("Build"), xtpBarTop);
	if (!pBuildBar ||
		!pBuildBar->LoadToolBar(IDR_TOOLBAR_BUILD))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	XTPImageManager()->SetMaskColor(RGB(0, 255, 0));

/*
	CXTPToolBar* pWebBar = (CXTPToolBar*)pCommandBars->Add(_T("Web"), xtpBarTop);
	if(!pWebBar || !pWebBar->LoadToolBar(IDR_TOOLBAR_WEB))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
*/

/*
	if (XTPImageManager()->IsAlphaIconsSupported())
	{
		XTPImageManager()->SetIcons(IDR_MAINFRAME, IDB_BITMAP1, xtpImageNormal);
		XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
		CXTPImageManager::m_dDisabledBrightnessFactor = 2.0;
		CXTPImageManager::m_dDisabledAlphaFactor = 4.0;
	}
*/

	XTPImageManager()->SetMaskColor(RGB(0xC0, 0xC0, 0xC0));
	XTPImageManager()->SetIcons(IDR_TOOLBAR_EXT);

	//@Solomon LoadCommandBars(_T("KB"));
	LoadCommandBars(_T("IBK"));

//////////////////////////////////////////////////////////////////////////
// Docking Pane

	GetDockingPaneManager()->InstallDockingPanes(this);
	GetDockingPaneManager()->SetTheme(xtpPaneThemeNativeWinXP);

	CXTPDockingPaneLayout layoutNormal(GetDockingPaneManager());

	if(layoutNormal.Load(_T("IBK"))) 
	{
		GetDockingPaneManager()->SetLayout(&layoutNormal);	
	} 
	else
	{		
//		CXTPDockingPane* paneToolBox = GetDockingPaneManager()->CreatePane(
//			ID_VIEW_TOOLBOX, CRect(0, 0, 150, 120), dockLeftOf);
//		paneToolBox->Hide();
		
		// 스크립트내 함수
		CXTPDockingPane* paneFuncInScriptView = GetDockingPaneManager()->CreatePane(
			ID_VIEW_CLASSVIEW, CRect(0, 0, 210, 140), xtpPaneDockLeft);

		// 출력창
		GetDockingPaneManager()->CreatePane(
			ID_WINDOWS_OUTPUT, CRect(0, 0, 150, 120), xtpPaneDockBottom);

		// 전략 리스트
		CXTPDockingPane* paneStrategy = GetDockingPaneManager()->CreatePane(
			ID_VIEW_SOLUTIONEXPLORER, CRect(0, 0, 210, 140), xtpPaneDockRight);

		// 함수 리스트
		CXTPDockingPane* paneFunction = GetDockingPaneManager()->CreatePane(
			ID_VIEW_RESOURCEVIEW, CRect(0, 0, 210, 140), xtpPaneDockRight);

		GetDockingPaneManager()->AttachPane(paneFunction, paneStrategy);
		
		paneStrategy->Select();

		CXTPDockingPane* paneProperties = GetDockingPaneManager()->CreatePane(
			ID_VIEW_PROPERTIESWINDOW, CRect(0, 0, 180, 140), xtpPaneDockBottom, paneStrategy);

		CXTPDockingPane* paneHelpView = GetDockingPaneManager()->CreatePane(
			ID_HELP_DYNAMICHELP, CRect(0, 0, 210, 140), xtpPaneDockRight, paneStrategy);
		
		GetDockingPaneManager()->AttachPane(paneHelpView, paneProperties);

		paneProperties->Select();
	}

	//20110726 이문수 >>	
	{
		AFX_SIZEPARENTPARAMS layout;

		layout.bStretch = TRUE;
		layout.sizeTotal.cx = layout.sizeTotal.cy = 0;
		GetClientRect(&layout.rect);    // starting rect comes from client rect

		layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess	
		UINT nChangeID = 0xffff - 1;

		HWND hWndChild = ::GetTopWindow(m_hWnd);
		for (; hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
		{
			UINT_PTR nIDC = ((UINT)::GetDlgCtrlID(hWndChild));
			CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
			if (nIDC != AFX_IDW_PANE_FIRST && nIDC > 0xffff && pWnd != NULL)
			{
				int nResult = pWnd->SetDlgCtrlID(nChangeID);
				nChangeID--;
			}
		}
	}
	//20110727 이문수 

	int nIDIcons[] = {
		ID_VIEW_PROPERTIESWINDOW, ID_VIEW_SOLUTIONEXPLORER, ID_WINDOWS_OUTPUT,
		ID_VIEW_CLASSVIEW, 0, 0, 0, 0, 0, 0, 0, 0, 0, ID_VIEW_RESOURCEVIEW, ID_HELP_DYNAMICHELP	
	};

	GetDockingPaneManager()->SetIcons(IDB_BITMAP_PANEICONS, nIDIcons,
		sizeof(nIDIcons)/sizeof(nIDIcons[0]), RGB(0, 255, 0));

	VERIFY(m_MTIClientWnd.Attach(this));
	
	CreateCompileStatusWindow();

	InitIndexFunctionStrategy();
	CollectCompileMessage();

	for(int i = 0; i < m_starBackupFile.GetSize(); i++)
		SetServerMessage(m_starBackupFile.GetAt(i), SVRMSG_NORMAL);

	m_starBackupFile.RemoveAll();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnEmptyCommand()
{

}
void CMainFrame::OnEmptyCommandRange(UINT)
{

}

CWnd* CMainFrame::CreatePropertyView()
{
	if (m_wndPropertyGrid.GetSafeHwnd() == 0)
	{
		m_wndPropertyGrid.Create( CRect(0, 0, 0, 0), this, 0 );
//		m_wndPropertyGrid.ShowToolBar(TRUE);
		m_wndPropertyGrid.SetOwner(this);
		m_wndPropertyGrid.SetTheme(xtpGridThemeNativeWinXP);

		CXTPPropertyGridItem* pSettings = m_wndPropertyGrid.AddCategory(_T("일반"));
		pSettings->SetDescription(_T("일반적인 스크립트의 속성을 나타냅니다."));

		m_pPrptyScriptName = pSettings->AddChildItem(new CXTPPropertyGridItem(_T("전략이름"), _T("")));
		m_pPrptyScriptName->SetDescription(_T("사용자가 작성한 스크립트의 이름입니다."));
		m_pPrptyScriptName->SetReadOnly(TRUE);

		m_pPrptyScriptComment = pSettings->AddChildItem(new CXTPPropertyGridItem(_T("설명"), _T("")));
		m_pPrptyScriptComment->SetDescription(_T("사용자가 작성한 스크립트의 간단한 설명입니다."));
		m_pPrptyScriptComment->SetReadOnly(TRUE);

		m_pPrptyScriptLanguage = pSettings->AddChildItem(new CXTPPropertyGridItem(_T("사용언어"), _T("")));
		m_pPrptyScriptLanguage->SetDescription(_T("스크립트에 사용된 언어입니다. (PASCAL/Yes Language)"));
		m_pPrptyScriptLanguage->SetReadOnly(TRUE);

		m_pPrptyScriptCheck = pSettings->AddChildItem(new CXTPPropertyGridItem(_T("검증여부"), _T("")));
		m_pPrptyScriptCheck->SetDescription(_T("스크립트의 문법 검사를 마쳤는지를 나타냅니다."));
		m_pPrptyScriptCheck->SetReadOnly(TRUE);

		pSettings->Expand();

		m_pPrptyVariables = m_wndPropertyGrid.AddCategory(_T("변수"));
		m_pPrptyVariables->SetDescription(_T("전략 스크립트의 변수 초기화를 표시합니다."));

		CXTPPropertyGridItem* pSignal = m_wndPropertyGrid.AddCategory(_T("신호표시"));
		pSignal->SetDescription(_T("스크립트에 사용된 신호명령에 대한 속성입니다."));

		m_pPrptyBuy = pSignal->AddChildItem(new CSTPropertyGridItemColor(_T("매수신호"), 0x000000));
		m_pPrptyBuy->SetValue(_T("없음"));
		m_pPrptyBuy->SetDescription(_T("매수신호에 대한 색상과 표시도형입니다."));
		m_pPrptyBuy->SetReadOnly(TRUE);
		m_pPrptyExitlong = pSignal->AddChildItem(new CSTPropertyGridItemColor(_T("매수청산"), 0x000000));
		m_pPrptyExitlong->SetValue(_T("없음"));
		m_pPrptyExitlong->SetDescription(_T("매수청산에 대한 색상과 표시도형입니다."));
		m_pPrptyExitlong->SetReadOnly(TRUE);
		m_pPrptySell = pSignal->AddChildItem(new CSTPropertyGridItemColor(_T("매도신호"), 0x000000));
		m_pPrptySell->SetValue(_T("없음"));
		m_pPrptySell->SetDescription(_T("매도신호에 대한 색상과 표시도형입니다."));
		m_pPrptySell->SetReadOnly(TRUE);
		m_pPrptyExitshort = pSignal->AddChildItem(new CSTPropertyGridItemColor(_T("매도청산"), 0x000000));
		m_pPrptyExitshort->SetValue(_T("없음"));
		m_pPrptyExitshort->SetDescription(_T("매도청산에 대한 색상과 표시도형입니다."));
		m_pPrptyExitshort->SetReadOnly(TRUE);
		m_pPrptyShowSignalName = pSignal->AddChildItem(new CXTPPropertyGridItemBool(_T("신호이름표시"), FALSE));
		m_pPrptyShowSignalName->SetReadOnly(TRUE);
		m_pPrptyShowSignalName->SetDescription(_T("신호의 이름을 표현할지 결정합니다."));
		m_pPrptyShowSignalQty = pSignal->AddChildItem(new CXTPPropertyGridItemBool(_T("매수수량표시"), FALSE));
		m_pPrptyShowSignalQty->SetReadOnly(TRUE);
		m_pPrptyShowSignalQty->SetDescription(_T("매수 수량을 표현할지 결정합니다."));
		m_pPrptyPosOfSignal = pSignal->AddChildItem(new CXTPPropertyGridItem(_T("신호완성"), _T("봉완성시")));
		m_pPrptyPosOfSignal->SetReadOnly(TRUE);
		m_pPrptyPosOfSignal->SetDescription(_T("선택된 신호완성 기준을 표시합니다."));

		m_pPrptyIndex = m_wndPropertyGrid.AddCategory(_T("지표표시"));
		m_pPrptyIndex->SetDescription(_T("스크립트에 사용된 지표명령 속성을 표시합니다."));

		CXTPPropertyGridItem* pBullBear = m_wndPropertyGrid.AddCategory(_T("강세/약세"));
		pBullBear->SetDescription(_T("스크립트에 사용된 강세/약세 명령 속성을 표시합니다."));

		m_pPrptyBull = pBullBear->AddChildItem(new CSTPropertyGridItemColor(_T("강세"), 0));
		m_pPrptyBull->SetDescription(_T("강세 색상을 표시합니다."));
		m_pPrptyBull->SetReadOnly(TRUE);

		m_pPrptyBear = pBullBear->AddChildItem(new CSTPropertyGridItemColor(_T("약세"), 0));
		m_pPrptyBear->SetDescription(_T("약세 색상을 표시합니다."));
		m_pPrptyBear->SetReadOnly(TRUE);

		CXTPPropertyGridItem* pSecurity = m_wndPropertyGrid.AddCategory(_T("보안"));
		pSecurity->SetDescription(_T("스크립트의 제한사항, 비밀번호 유무를 표시합니다."));

		m_pPrptyPassword = pSecurity->AddChildItem(new CXTPPropertyGridItem(_T("비밀번호"), _T("")));
		m_pPrptyPassword->SetDescription(_T("스크립트를 편집/열람할 수 있는 비밀번호 지정여부를 표시합니다."));
		m_pPrptyPassword->SetReadOnly(TRUE);

		m_pPrptyUserID = pSecurity->AddChildItem(new CXTPPropertyGridItem(_T("사용자ID"), _T("")));
		m_pPrptyUserID->SetDescription(_T("전략을 사용할 수 있는 사용자의 ID를 표시합니다. ID가 없으면 모두 사용할 수 있는 전략입니다."));
		m_pPrptyUserID->SetReadOnly(TRUE);

		m_pPrptyExpires = pSecurity->AddChildItem(new CXTPPropertyGridItem(_T("만료일"), _T("")));
		m_pPrptyExpires->SetDescription(_T("전략을 사용할 수 있는 만료일을 표시합니다. 만료일이 없으면 계속 사용할 수 있는 전략입니다."));
		m_pPrptyExpires->SetReadOnly(TRUE);
/*
		CFont font;
		font.CreatePointFont(60, _T("Marlett"));

		LOGFONT lf;
		font.GetLogFont(&lf);

		// create document settings category.
        CXTPPropertyGridItem* pSettings        = m_wndPropertyGrid.AddCategory(_T("Document Settings"));
        
		// add child items to category.
		CXTPPropertyGridItem* pItemSaveOnClose = pSettings->AddChildItem(new CXTPPropertyGridItemBool(_T("SaveOnClose"), TRUE));
        pSettings->AddChildItem(new CXTPPropertyGridItemFont(_T("WindowFont"), lf));
        pSettings->AddChildItem(new CXTPPropertyGridItemSize(_T("WindowSize"), CSize(100, 100)));

		pSettings->Expand();
		pItemSaveOnClose->Select();

		// create global settings category.
		CXTPPropertyGridItem* pGlobals      = m_wndPropertyGrid.AddCategory(_T("Global Settings"));

		// add child items to category.
		CXTPPropertyGridItem* pItemGreeting = pGlobals->AddChildItem(new CXTPPropertyGridItem(_T("Greeting Text"), _T("Welcome to your application!")));
		pGlobals->AddChildItem(new CXTPPropertyGridItemNumber(_T("ItemsInMRUList"), 4));
		CXTPPropertyGridItem* pItemRate     = pGlobals->AddChildItem(new CXTPPropertyGridItemNumber(_T("MaxRepeatRate"), 10));
		pGlobals->AddChildItem(new CXTPPropertyGridItemColor(_T("ToolbarColor"), RGB(255, 192,128)));

		pItemGreeting->SetReadOnly(TRUE);
		pItemRate->SetDescription(_T("The rate in milliseconds that the text will repeat."));

		// create version category.
		CXTPPropertyGridItem* pVersion      = m_wndPropertyGrid.AddCategory(_T("Version"));

		// add child items to category.
		CXTPPropertyGridItem* pItemVersion  = pVersion->AddChildItem(new CXTPPropertyGridItem(_T("AppVerion"), _T("1.0")));
		CXTPPropertyGridItem* pItemLanguage = pVersion->AddChildItem(new CXTPPropertyGridItem(_T("Language"), _T("English (United States)")));

		pItemVersion->SetReadOnly(TRUE);
		pVersion->Expand();

		CXTPItemConstraints* pList = pItemLanguage->GetConstraints();

		pList->AddConstraint(_T("Neutral"));
		pList->AddConstraint(_T("Arabic"));
		pList->AddConstraint(_T("German"));
		pList->AddConstraint(_T("Chinese(Taiwan)"));
		pList->AddConstraint(_T("English (United Kingdom)"));
		pList->AddConstraint(_T("English (United States)"));
		pList->AddConstraint(_T("France"));
		pList->AddConstraint(_T("Russian"));

		pItemLanguage->SetFlags(pgiHasComboButton | pgiHasEdit);

		// create custom items category.
		CXTPPropertyGridItem* pCustom   = m_wndPropertyGrid.AddCategory(_T("Custom Items"));


		// add multi level tree node.
		CXTPPropertyGridItem* pItemOne    = pCustom->AddChildItem(new CXTPPropertyGridItem(_T("First Level"), _T("")));
		CXTPPropertyGridItem* pItemTwo    = pItemOne->AddChildItem(new CXTPPropertyGridItem(_T("Second Level"), _T("")));
		CXTPPropertyGridItem* pItemThird	 = pItemTwo->AddChildItem(new CXTPPropertyGridItem(_T("Third Level"), _T("")));
		pItemThird->AddChildItem(new CXTPPropertyGridItem(_T("Fourth Level 1"), _T("")));
		pItemThird->AddChildItem(new CXTPPropertyGridItem(_T("Fourth Level 2"), _T("")));
*/
	}
	
	return &m_wndPropertyGrid;
}
LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
				case ID_VIEW_PROPERTIESWINDOW:	pPane->Attach(CreatePropertyView()); break;
				case ID_VIEW_CLASSVIEW:	
					pPane->Attach(m_pLoadWorkspace->GetFunctionInStrategyWnd()); 
					ScanFunctionInStrategy();
					break;				
				case ID_VIEW_SOLUTIONEXPLORER:	pPane->Attach(m_pLoadWorkspace->GetStrategyWnd()); break;				
				case ID_VIEW_RESOURCEVIEW:	pPane->Attach(m_pLoadWorkspace->GetFunctionWnd()); break;				
				case ID_WINDOWS_OUTPUT:	pPane->Attach(m_pLoadWorkspace->GetOutputWnd()); break;
				case ID_HELP_DYNAMICHELP:
					{
						// attach the dynamic help view to the pane.
						m_pViewHelp = (CDynamicHelpView*)pPane->AttachView(this, RUNTIME_CLASS(CDynamicHelpView));

						// send a WM_INITIALUPDATE message to the view.
						if (m_pViewHelp && ::IsWindow(m_pViewHelp->m_hWnd)) {
							m_pViewHelp->SendMessage(WM_INITIALUPDATE);
						}
						break;
					}
			}
		}
		return 1;
	}
	return 0;
}

void CMainFrame::OnShowView(UINT nID)
{
	GetDockingPaneManager()->ShowPane(nID);
}


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

/*
		if (lpCreateControl->nID == ID_VIEW_CLASSVIEW && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
*/
		if (lpCreateControl->nID == ID_FILE_NEW_STRATEGY && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		if (lpCreateControl->nID == ID_FILE_OPEN_PROJECT && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
/*
		if (lpCreateControl->nID == ID_PROJECT_ADDNEWITEM && pToolBar->GetBarID() == IDR_MAINFRAME)		 
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
*/

		if (lpCreateControl->nID == ID_EDIT_FIND_EX && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			CXTPControlComboBox* pComboFind = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
			pComboFind->SetDropDownListStyle();
			
			pComboFind->SetWidth(150);
			pComboFind->SetCaption(_T("Find"));
			pComboFind->SetStyle(xtpComboLabel);

			lpCreateControl->pControl = pComboFind;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_EDIT_STATE && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
			pComboState->AddString(_T("Debug"));
			pComboState->AddString(_T("Release"));
			pComboState->AddString(_T("Configuration Manager"));
			pComboState->SetDropDownWidth(150);
			lpCreateControl->pControl = pComboState;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_GOTO_URL && pToolBar->GetBarID() == IDR_TOOLBAR_WEB)
		{
			CXTPControlComboBox* pComboUrl = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
			pComboUrl->AddString(_T("http://www.dooriic.co.kr"));
//			pComboUrl->AddString(_T("ms-help://MS.VSCC/vsccCommon/html/vsccCommon_Start_Page.htm"));
			pComboUrl->SetWidth(200);
			pComboUrl->SetDropDownWidth(300);
			pComboUrl->SetDropDownListStyle();
			pComboUrl->SetFlags(xtpFlagManualUpdate);
			pComboUrl->SetEditText(_T("http://www.dooriic.co.kr"));

			lpCreateControl->pControl = pComboUrl;
			return TRUE;
		}


	}

	return FALSE;
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());
	
	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);
	
	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_GUI_VITYPE, TRUE);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_GUI_VITYPE);
	pCommands->InsertNewMenuCategory();
	
	cs.DoModal();
}

void CMainFrame::OnEditState(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
		
	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		int nState = pControl->GetCurSel();
		if (nState != 2)
		{
			m_nState = nState;
		} 
		else
		{
			MessageBox(_T("Configuration Manager"));
			pControl->SetCurSel(m_nState);
		}	

		*pResult = TRUE; // Handled
	}	
}

void CMainFrame::OnUpdateEditState(CCmdUI* pCmdUI)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if (pToolBar && !XTPMouseManager()->IsTrackedLock())
	{	
		CXTPControlComboBox* pStateCombo = (CXTPControlComboBox*)pToolBar->GetControls()->GetAt(pCmdUI->m_nIndex);

		if (pStateCombo->GetType() == xtpControlComboBox)
		{
			pStateCombo->SetCurSel(m_nState);
		}
	}

	pCmdUI->Enable(TRUE);
}

BOOL CMainFrame::ShowWindowEx(int nCmdShow)
{
#ifdef _XTP_INCLUDE_CONTROLS
	// Restore frame window size and position.
	CXTWindowPos wndPosition;
	wndPosition.LoadWindowPos(this);
	nCmdShow = wndPosition.showCmd;
#endif

	return ShowWindow(nCmdShow);
}

void CMainFrame::OnClose() 
{
	SaveCommandBars(_T("IBK"));

	CXTPDockingPaneLayout layoutNormal(GetDockingPaneManager());
	GetDockingPaneManager()->GetLayout(&layoutNormal);	
	layoutNormal.Save(_T("IBK"));

#ifdef _XTP_INCLUDE_CONTROLS
	CXTWindowPos wndPosition;
	wndPosition.SaveWindowPos(this);
#endif
	
	CMDIFrameWnd::OnClose();
}

void NotifyExtLib(CPtrList* pList, long lResv)
{
	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		ST_SCRIPTINFO* pScriptInfo = (ST_SCRIPTINFO*)pList->GetNext(pos);
	}
}

void CMainFrame::OnDestroy() 
{
	CMDIFrameWnd::OnDestroy();
	
	m_MTIClientWnd.Detach();	

	if(m_pLoadWorkspace)
	{
		delete m_pLoadWorkspace;
		m_pLoadWorkspace = NULL;
	}

	if(m_pLoadExtLib)
	{
//		m_pLoadExtLib->ExitExtLib(NotifyExtLib);
		
		delete m_pLoadExtLib;
		m_pLoadExtLib = NULL;
	}

	if(m_pLoadEncoder)
	{
		delete m_pLoadEncoder;
		m_pLoadEncoder = NULL;
	}

	if(m_pLoadConfig)
	{
		delete m_pLoadConfig;
		m_pLoadConfig = NULL;
	}

	DeleteSystemTradingParser();

	if(m_pMainClient)
	{
		m_pMainClient->Close();
		
		delete m_pMainClient;
		m_pMainClient = NULL;
	}

	std::for_each(m_vtStrategyInfo.begin(), m_vtStrategyInfo.end(), DeleteScriptInfo);
	m_vtStrategyInfo.clear();

	std::for_each(m_vtUserFuncInfo.begin(), m_vtUserFuncInfo.end(), DeleteScriptInfo);
	m_vtUserFuncInfo.clear();

	std::for_each(m_vtInnerFuncInfo.begin(), m_vtInnerFuncInfo.end(), DeleteInnerFuncInfo);
	m_vtInnerFuncInfo.clear();

	std::for_each(m_vtInnerFuncInfo_YES.begin(), m_vtInnerFuncInfo_YES.end(), DeleteInnerFuncInfo);
	m_vtInnerFuncInfo_YES.clear();

	for_each(m_vtIndexFunction.begin(), m_vtIndexFunction.end(), FreeString);
	m_vtIndexFunction.clear();
}

void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState) 
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

// Toggle CommandBars
	int nCount = pCommandBars->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		
		CXTPToolBar* pBar = pCommandBars->GetAt(i);
		pBar->OnSetPreviewMode(bPreview);	
	}
// Toggle Docking Panes.
	if (bPreview)
	{
		ASSERT(m_pPreviewLayout == NULL);
		m_pPreviewLayout   = m_paneManager.CreateLayout();
		m_paneManager.GetLayout(m_pPreviewLayout);
		m_paneManager.CloseAll();
	} else
	{
		ASSERT(m_pPreviewLayout != NULL);		
		m_paneManager.SetLayout(m_pPreviewLayout);
		delete m_pPreviewLayout;
		m_pPreviewLayout = 0;
		
	}

	
	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}

void CMainFrame::InitUserFuntionList()
{
	char szPath[MAX_PATH] = {NULL, };

//@유진삭제::wsprintf(szPath, _T("%s%s*.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES);
	::wsprintf(szPath, _T("%s%s*.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2);
	
	UINT nID = TREEINDEX_DEFUALTITEM;

	CFileFind ff;
	BOOL bFind = ff.FindFile(szPath);

	CString strScriptName;
	char szConfigPath[MAX_PATH];
	UINT nCompiled = IMG_FUNC;
	while(bFind)
	{
		bFind = ff.FindNextFile();

		if(ff.IsHidden())
		{
			CString strFilePath = ff.GetFilePath();

			::SetFileAttributes(strFilePath, FILE_ATTRIBUTE_NORMAL);

			strScriptName.Format(_T("%s.bak"), strFilePath);
			CFileFind ff2;
			if(!ff2.FindFile(strScriptName))
				EncodingFile(strFilePath, strScriptName);

			if(::CopyFile(strScriptName, strFilePath, FALSE))
				::DeleteFile(strScriptName);

			::wsprintf(szPath, _T("%s 백업파일을 복구했습니다."), ff.GetFileName().Left(ff.GetFileName().GetLength() - 4)); 
			m_starBackupFile.Add(szPath);
		}

		strScriptName = ff.GetFileTitle();

		if(::GetSTFile(szConfigPath, strScriptName, theApp.GetRootPath()))
			nCompiled = ::GetPrivateProfileInt(_T("Basic"), _T("Compiled"), nCompiled, szConfigPath);
		else
			::WritePrivateProfileString(_T("Basic"), _T("Compiled"), _T("7"), szConfigPath);

		CScriptInfomation* pUserFunc = new CScriptInfomation(strScriptName, nID++, FT_USERFUNCTION, nCompiled);
		m_vtUserFuncInfo.push_back(pUserFunc);
	}
}

void CMainFrame::InitStrategyList()
{
	char szPath[MAX_PATH] = {NULL, };

//@유진삭제::wsprintf(szPath, _T("%s%s*.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES);
	::wsprintf(szPath, _T("%s%s*.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2);
	
	UINT nID = TREEINDEX_DEFUALTITEM;

	CFileFind ff;
	BOOL bFind = ff.FindFile(szPath);
	CString strScriptName;
	char szConfigPath[MAX_PATH];
	UINT nCompiled = IMG_STS;
	while(bFind)
	{
		bFind = ff.FindNextFile();

		if(ff.IsHidden())
		{
			CString strFilePath = ff.GetFilePath();

			::SetFileAttributes(strFilePath, FILE_ATTRIBUTE_NORMAL);

			strScriptName.Format(_T("%s.bak"), strFilePath);
			CFileFind ff2;
			if(!ff2.FindFile(strScriptName))
				EncodingFile(strFilePath, strScriptName);

			if(::CopyFile(strScriptName, strFilePath, FALSE))
				::DeleteFile(strScriptName);

			::wsprintf(szPath, _T("%s 백업파일을 복구했습니다."), ff.GetFileName().Left(ff.GetFileName().GetLength() - 4)); 
			m_starBackupFile.Add(szPath);
		}

		strScriptName = ff.GetFileTitle();
		::GetSTFile(szConfigPath, strScriptName, theApp.GetRootPath());
		nCompiled = ::GetPrivateProfileInt(_T("Basic"), _T("Compiled"), nCompiled, szConfigPath);

		CScriptInfomation* pStrategy = new CScriptInfomation(strScriptName, nID++, FT_STRATEGY, nCompiled);
		m_vtStrategyInfo.push_back(pStrategy);
	}
}


typedef BOOL (WINAPI* FpGetInputs)(long, long, CString&, CString&);
void CMainFrame::InitInnerFunctionList(LPSTR lpszFileName)
{
	HINSTANCE hInnerFunction = ::LoadLibrary(lpszFileName);

	if(!hInnerFunction)
		return;

	FpGetInputs	pGetInputs;
	pGetInputs = (FpGetInputs)::GetProcAddress(hInnerFunction, _T("fnGetInputs"));
	
	if(!pGetInputs)
	{
		::FreeLibrary(hInnerFunction);
		return;
	}

	int i = 0;
	CString strFunctioName(_T(""));
	CString strInputs(_T(""));

	BOOL bTraderStation = TRUE;
	BOOL bYesTrader = TRUE;
	
//	CFile ff("d:\\inner.dat", CFile::modeCreate|CFile::modeWrite);

	while(bTraderStation || bYesTrader)
	{
		if(bTraderStation)
		{
			bTraderStation = pGetInputs(ST_TRADESTATION, i, strFunctioName, strInputs);
			if(bTraderStation)
				AddInnerFunction(strFunctioName, strInputs, i, ST_TRADESTATION);
		}

		if(bYesTrader)
		{
			bYesTrader = pGetInputs(ST_YESTRADER, i, strFunctioName, strInputs);
			if(bYesTrader)
				AddInnerFunction(strFunctioName, strInputs, i, ST_YESTRADER);
		}

		i++;
	}

//	ff.Close();

	::FreeLibrary(hInnerFunction);
}

void CMainFrame::AddInnerFunction(CString strFunctionName, CString strInputs, int nIndex, UINT nScriptType)
{
	strFunctionName.Replace(_T(" "), _T(""));
	strInputs.Replace(_T(" "), _T(""));

	if(!strFunctionName.GetLength())
		return;

	CString strProperty(_T(""));
	CString strSection(_T(""));
	CString strKey(_T(""));
	CString strRefIndex(_T(""));

	int nLength = 0;

	if(strInputs.GetLength() && strInputs[0] == '[')
	{
		strInputs.Delete(0, 1);
		if(AfxExtractSubString(strProperty, strInputs, 0, ']'))
		{
			nLength = strProperty.GetLength();
			AfxExtractSubString(strSection, strProperty, 0, ',');
			AfxExtractSubString(strKey, strProperty, 1, ',');
			AfxExtractSubString(strRefIndex, strProperty, 2, ',');
		}

		strInputs.Delete(0, nLength + 1);
	}
	
	CInnerFunctionInfo* pInnerFunction = new CInnerFunctionInfo(strFunctionName, strInputs, nIndex, strSection, strKey, atoi(strRefIndex));

	if(nScriptType == ST_TRADESTATION)
		m_vtInnerFuncInfo.push_back(pInnerFunction);
	else if(nScriptType == ST_YESTRADER)
		m_vtInnerFuncInfo_YES.push_back(pInnerFunction);
}

LRESULT CMainFrame::OnDblClickWorkspace(WPARAM p_wParam, LPARAM p_lParam)
{
	CScriptFind findScript((UINT)p_wParam);

	if(p_wParam >= 1000 && p_wParam < 2000)
	{
		VT_SCRIPTINFO::iterator it = std::find_if(m_vtUserFuncInfo.begin(), m_vtUserFuncInfo.end(), findScript);
		if(it != m_vtUserFuncInfo.end())
		{
			char szPath[MAX_PATH] = {NULL, };

//@유진삭제	::wsprintf(szPath, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, (*it)->GetFunctionName());
			::wsprintf(szPath, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, (*it)->GetFunctionName());

			theApp.OpenDocumentFile(szPath);
		}
	}

	return 0L;
}

int CMainFrame::IsUserFunctionKeyword(LPSTR p_lpszChars, int p_nLength)
{
	CScriptFind findScript(p_lpszChars, p_nLength);
	VT_SCRIPTINFO::iterator it = std::find_if(m_vtUserFuncInfo.begin(), m_vtUserFuncInfo.end(), findScript);

	if(it != m_vtUserFuncInfo.end())
	{
		UINT nCompiled = (*it)->GetCompiledFlag();
		if(nCompiled == IMG_FUNC || nCompiled == IMG_FUNC_LOCK)
			return -1;
		else
			return 1; 
	}
	else
		return 0;
}

BOOL CMainFrame::IsInnerFunctionKeyword(LPSTR p_lpszChars, int p_nLength, UINT nScriptType)
{
	CFindInnerFunction findFunc(p_lpszChars, p_nLength);

	if(nScriptType == ST_TRADESTATION)
	{
		VT_INNERFUNCINFO::iterator it = std::find_if(m_vtInnerFuncInfo.begin(), m_vtInnerFuncInfo.end(), findFunc);
		findFunc.DeleteString();
		
		if(it != m_vtInnerFuncInfo.end())
			return TRUE;
		else
			return FALSE;
	}
	else if(nScriptType == ST_YESTRADER)
	{
		VT_INNERFUNCINFO::iterator it = std::find_if(m_vtInnerFuncInfo_YES.begin(), m_vtInnerFuncInfo_YES.end(), findFunc);
		findFunc.DeleteString();
		
		if(it != m_vtInnerFuncInfo_YES.end())
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

void CMainFrame::OnFileNewStrategy() 
{
	LoadConfigDlg();

	CString strDataPath;
	strDataPath.Format(_T("%s%sNewScript.ST"), theApp.GetRootPath(), PATH_STDATA);

	// 읽기 전용 해제
	DWORD dwAttributes = GetFileAttributes(strDataPath);
	if(dwAttributes & FILE_ATTRIBUTE_READONLY)
	{
		dwAttributes ^= FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strDataPath, dwAttributes);
	}

//@유진삭제m_strNewFileName.Format(_T("%s%sNewStrategy@0.ST"), theApp.GetRootPath(), PATH_STFILES);
	m_strNewFileName.Format(_T("%s%sNewStrategy%c0.ST"), theApp.GetUserPath(), PATH_STFILES2, gSTGubunKey);

	CFileFind ff;
	int nInc = 1;
	while(ff.FindFile(m_strNewFileName))
//@유진삭제m_strNewFileName.Format(_T("%s%sNewStrategy(%d)@0.ST"), theApp.GetRootPath(), PATH_STFILES, nInc++);
		m_strNewFileName.Format(_T("%s%sNewStrategy[%d]%c0.ST"), theApp.GetUserPath(), PATH_STFILES2, nInc++, gSTGubunKey);

	m_nPropertyState = 1;		// New Strategy
	
	::CopyFile(strDataPath, m_strNewFileName, TRUE);


	m_pLoadConfig->ShowConfigureDlg(m_hWnd, NULL, (LPCTSTR)m_strNewFileName, m_nPropertyState);
}

void CMainFrame::OnFileNewFunction() 
{
	LoadConfigDlg();

	CString strDataPath;
	strDataPath.Format(_T("%s%sNewScript.ST"), theApp.GetRootPath(), PATH_STDATA);

	// 읽기 전용 해제
	DWORD dwAttributes = GetFileAttributes(strDataPath);
	if(dwAttributes & FILE_ATTRIBUTE_READONLY)
	{
		dwAttributes ^= FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strDataPath, dwAttributes);
	}

//@유진삭제m_strNewFileName.Format(_T("%s%sNewFunction@0.ST"), theApp.GetRootPath(), PATH_STFILES);
	m_strNewFileName.Format(_T("%s%sNewFunction%c0.ST"), theApp.GetUserPath(), PATH_STFILES2, gSTGubunKey);

	CFileFind ff;
	int nInc = 1;
	while(ff.FindFile(m_strNewFileName))
//@유진삭제m_strNewFileName.Format(_T("%s%sNewFunction(%d)@0.ST"), theApp.GetRootPath(), PATH_STFILES, nInc++);
		m_strNewFileName.Format(_T("%s%sNewFunction(%d)%c0.ST"), theApp.GetUserPath(), PATH_STFILES2, nInc++, gSTGubunKey);

	m_nPropertyState = 2;		// New Function
	
	::CopyFile(strDataPath, m_strNewFileName, TRUE);
	m_pLoadConfig->ShowConfigureDlg(m_hWnd, NULL, (LPCTSTR)m_strNewFileName, m_nPropertyState);
}

void CMainFrame::OnFileSaveAll() 
{
	POSITION pos = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(pos)
	{
		CDocument* pDoc = theApp.m_pDocTemplate->GetNextDoc(pos);
		pDoc->DoFileSave();
	}
}

void CMainFrame::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	POSITION pos = theApp.m_pDocTemplate->GetFirstDocPosition();
	pCmdUI->Enable(!pos ? FALSE : TRUE);
}

LRESULT CMainFrame::OnDblClickTree(WPARAM p_wParam, LPARAM p_lParam)
{
	char szFile[MAX_PATH] = {NULL, };
	
	if(p_wParam == FT_STRATEGY)	// Open the strategy file.
//@유진삭제::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, (LPCSTR)p_lParam);
		::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, (LPCSTR)p_lParam);

	else if(p_wParam == FT_USERFUNCTION)	// Open the user function file.
//@유진삭제::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, (LPCSTR)p_lParam);
		::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, (LPCSTR)p_lParam);

	else if(p_wParam == FT_ALLFILE)	// Open the user function file.
//@유진삭제::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, (LPCSTR)p_lParam);
		::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, (LPCSTR)p_lParam);

	theApp.OpenDocumentFile(szFile);

	return 0L;
}

void CMainFrame::SetOutputText(int nMessage, LPCSTR lpszText, CErrorInfomation* pErrorInfo/* = NULL*/)
{
	if(m_pLoadWorkspace)
	{
		CString strMessage(_T(""));
		if(m_mapCompileMessage.Lookup(nMessage, strMessage))
		{
			char szMessage[256] = {NULL, };
			if(pErrorInfo)
			{
				LPCSTR lpszError = pErrorInfo->GetErrorString();
				if(::lstrlen(lpszError))
					::wsprintf(szMessage, strMessage, lpszText, pErrorInfo->GetErrorString(), nMessage);
				else
					::wsprintf(szMessage, strMessage, lpszText, nMessage);
			}
			else
				::wsprintf(szMessage, strMessage, lpszText, nMessage);

			m_pLoadWorkspace->SetOutputText(nMessage, szMessage, pErrorInfo);
		}
	}
}

void CMainFrame::ChangeCompiledOption(LPCSTR lpszName, UINT nFileType, UINT nCompiled)
{
	char szPath[MAX_PATH] = {NULL, };
	::GetSTFile(szPath, lpszName, theApp.GetRootPath());

	int nPrevImgIndex = ::GetPrivateProfileInt(_T("Basic"), _T("Compiled"), IMG_STS, szPath);
	if((nPrevImgIndex == IMG_STS_LOCK || nPrevImgIndex == IMG_STS_LOCK_CERT || nPrevImgIndex == IMG_STS_LOCK_DLL) &&
		(nCompiled == IMG_TREE || nCompiled == IMG_STS_CERT || nCompiled == IMG_STS_DLL || nCompiled == IMG_FUNC_CERT || nCompiled == IMG_FUNC_DLL))
		nCompiled += 1;
	else if((nPrevImgIndex == IMG_FUNC_LOCK || nPrevImgIndex == IMG_FUNC_LOCK_CERT || nPrevImgIndex == IMG_FUNC_LOCK_DLL) &&
		(nCompiled == IMG_TREE || nCompiled == IMG_FUNC_CERT || nCompiled == IMG_FUNC_DLL))
		nCompiled += 1;

	CString strCompiled;
	strCompiled.Format(_T("%d"), nCompiled);
	
	::WritePrivateProfileString(_T("Basic"), _T("Compiled"), strCompiled, szPath);

	if(m_pLoadWorkspace)
		m_pLoadWorkspace->ChangeCompiledOption(nFileType, lpszName, nCompiled);

	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		POSITION posView = pDoc->GetFirstViewPosition();
		while(posView)
		{
			CSystemTradingView* pView = (CSystemTradingView*)pDoc->GetNextView(posView);
			pView->Invalidate();
		}
	}
}

void CMainFrame::MakeFileUserFunction(LPCSTR lpszFunctionName)
{
	int nCommand = SOCK_CMD_START_UF;

	if(theApp.GetDebugMode())
	{
		UCHAR cBuf[256] = {NULL, };
		::GetKeyboardState(cBuf);
		
		if(cBuf[VK_LSHIFT] & 128)
			nCommand = SOCK_CMD_START_UF_DEBUG;
	}

	if(m_pMainClient)
	{
		m_pMainClient->SendToServer(nCommand, (LPVOID)lpszFunctionName, ::lstrlen(lpszFunctionName));
		SetOutputText(CM_COMPILE_COMPLETE_FUNCTION, lpszFunctionName);
	}
	else
	{
		if(MessageBox(_T("서버와의 통신이 끊겼습니다.\n재접속 하시겠습니까?"), _T("서버연결"), MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			SetConnectionStatusTest(_T("재접속중"));
			
			ConnectToCompileServer();

			if(m_pMainClient)
			{
				SetServerMessage(_T("서버와의 연결에 성공하였습니다."), SVRMSG_NORMAL);

				m_pMainClient->SendToServer(nCommand, (LPVOID)lpszFunctionName, ::lstrlen(lpszFunctionName));
				SetOutputText(CM_COMPILE_COMPLETE_FUNCTION, lpszFunctionName);
			}
			else
			{
				m_bCompiling = FALSE;
				m_ctrlCompileProgress.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			m_bCompiling = FALSE;
			m_ctrlCompileProgress.ShowWindow(SW_HIDE);
		}
	}
}

void CMainFrame::MakeFileMainStrategy(LPCSTR lpszStrategyName, UINT nStrategyType)
{
	int nCommand = SOCK_CMD_START;

	if(theApp.GetDebugMode())
	{
		UCHAR cBuf[256] = {NULL, };
		::GetKeyboardState(cBuf);
		
		if(cBuf[VK_LSHIFT] & 128)
			nCommand = SOCK_CMD_START_DEBUG;
	}

	if(m_pMainClient)
	{
		m_pMainClient->SendToServer(nCommand, (LPVOID)lpszStrategyName, ::lstrlen(lpszStrategyName), nStrategyType);
		SetOutputText(CM_COMPILE_COMPLETE_STRATEGY, lpszStrategyName);
	}
	else
	{
		if(MessageBox(_T("서버와의 통신이 끊겼습니다.\n재접속 하시겠습니까?"), _T("서버연결"), MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			SetConnectionStatusTest(_T("재접속중"));
			
			ConnectToCompileServer();

			if(m_pMainClient)
			{
				SetServerMessage(_T("서버와의 연결에 성공하였습니다."), SVRMSG_NORMAL);

				m_pMainClient->SendToServer(nCommand, (LPVOID)lpszStrategyName, ::lstrlen(lpszStrategyName));
				SetOutputText(CM_COMPILE_COMPLETE_STRATEGY, lpszStrategyName);
			}
			else
			{
				m_bCompiling = FALSE;
				m_ctrlCompileProgress.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			m_bCompiling = FALSE;
			m_ctrlCompileProgress.ShowWindow(SW_HIDE);
		}
	}
}

void CMainFrame::MakeFileAllScirpt(LPCSTR lpszNames)
{
	if(!m_pMainClient)
	{
		if(MessageBox(_T("서버와의 통신이 끊겼습니다.\n재접속 하시겠습니까?"), _T("서버연결"), MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			SetConnectionStatusTest(_T("재접속중"));
			
			ConnectToCompileServer();

			if(m_pMainClient)
			{
				AfxMessageBox(_T("서버와의 연결에 성공하였습니다."));
			}
			else
			{
				m_bCompiling = FALSE;
				m_ctrlCompileProgress.ShowWindow(SW_HIDE);
				return;
			}
		}
		else
		{
			m_bCompiling = FALSE;
			m_ctrlCompileProgress.ShowWindow(SW_HIDE);
			return;
		}
	}

	char szCppFiles[MAX_PATH] = {NULL, };
	
	::wsprintf(szCppFiles, _T("%s%sOutput\\*.cpp"), theApp.GetRootPath(), PATH_STDATA);
	
	CFileFind ff;
	BOOL bFind = ff.FindFile(szCppFiles);

	char szZip[MAX_PATH] = {NULL, };

//@유진삭제::wsprintf(szZip, _T("%s%s%s.Zip"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszNames);
	::wsprintf(szZip, _T("%s%s%s.Zip"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszNames);

	CLoadSystemTradingZipHelper Helper;
	CStringArray arFiles;

	while(bFind)
	{
		bFind = ff.FindNextFile();
		arFiles.Add(ff.GetFilePath());
	}
	
	if(Helper.CompressFiles(szZip, arFiles) && m_pMainClient)
	{
		m_pMainClient->SendToServer(SOCK_CMD_START_ALL, (LPVOID)lpszNames, ::lstrlen(lpszNames));
		SetOutputText(CM_COMPILE_COMPLETE_ALL, lpszNames);
	}

}

void CMainFrame::CollectCompileMessage()
{
	char szFile[MAX_PATH] = {NULL, };
	::wsprintf(szFile, _T("%s%scompile_msg.dat"), theApp.GetRootPath(), PATH_STDATA);

	CStdioFile file(szFile, CFile::modeRead);
	CString strLine;

	CString strValue(_T(""));
	int	nValue = 0;
	while(file.ReadString(strLine))
	{
		AfxExtractSubString(strValue, strLine, 0, ':');
		nValue = atoi(strValue);

		AfxExtractSubString(strValue, strLine, 1, ':');

		m_mapCompileMessage.SetAt(nValue, strValue);
	}

	file.Close();
}

LRESULT CMainFrame::OnVerifyScriptName(WPARAM wParam, LPARAM lParam)
{
	// Do not check for duplicate name when it is modifiy property.
	if(wParam == 3 || wParam == 4)
		return 1L;

	LPCSTR lpszScriptName = (LPCSTR)lParam;

	// Check for the script name.
	// If the name is exist, then require an user to another script name
	if(!VerifyScriptNameBySavedName(lpszScriptName))
		return 0L;

	if(!VerifyScriptNameByFileName(lpszScriptName))
		return 0L;

	// 내부함수 검사
	if(!VerifyScriptNameByInnerFunction(lpszScriptName, wParam))
		return 0L;

	// 예약어 검사
	if(!VerifyScriptNameByReservedWord(lpszScriptName, wParam))
		return 0L;

	return 1L;
}

BOOL CMainFrame::VerifyScriptNameByFileName(LPCSTR lpszScriptName)
{
	//@Solomon ^ 추가.
	char cChar[] = {'\"',':', '/', '*', '?', '<', '>', '|', '\\', '@', '^'};
	int nLen = ::lstrlen(lpszScriptName);

	for(int i = 0; i < nLen; i++)
		for(int j = 0; j < sizeof(cChar); j++)
			if(lpszScriptName[i] == cChar[j])
			{
				CString strMessage;
				strMessage.Format(_T("%s 파일명에 사용할 수 없는 문자가 있습니다. - %c"), lpszScriptName, cChar[j]);

				MessageBox(strMessage);
				return FALSE;
			}

	return TRUE;
}

BOOL CMainFrame::VerifyScriptNameBySavedName(LPCSTR lpszScriptName)
{
	CScriptFind findScript(lpszScriptName);
	
	VT_SCRIPTINFO::iterator it = std::find_if(m_vtStrategyInfo.begin(), m_vtStrategyInfo.end(), findScript);
	if(it != m_vtStrategyInfo.end())
	{
		CString strMessage;
		strMessage.Format(_T("%s 이름은 전략 스크립트에서 사용중입니다."), lpszScriptName);

		MessageBox(strMessage);
		return FALSE;
	}

	it = std::find_if(m_vtUserFuncInfo.begin(), m_vtUserFuncInfo.end(), findScript);
	if(it != m_vtUserFuncInfo.end())
	{
		CString strMessage;
		strMessage.Format(_T("%s 이름은 함수 스크립트에서 사용중입니다."), lpszScriptName);

		MessageBox(strMessage);
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::VerifyScriptNameByInnerFunction(LPCSTR lpszScriptName, WPARAM wParam)
{
	CFindInnerFunction findFunc(lpszScriptName);

	VT_INNERFUNCINFO::iterator itInner = std::find_if(m_vtInnerFuncInfo.begin(), m_vtInnerFuncInfo.end(), findFunc);
	if(itInner != m_vtInnerFuncInfo.end())
	{
		findFunc.DeleteString();
		CString strMessage;
		strMessage.Format(_T("%s 이름은 내부함수(Trade Station)에서 사용중입니다."), lpszScriptName);

		MessageBox(strMessage);
		return FALSE;
	}

	itInner = std::find_if(m_vtInnerFuncInfo_YES.begin(), m_vtInnerFuncInfo_YES.end(), findFunc);
	if(itInner != m_vtInnerFuncInfo_YES.end())
	{
		findFunc.DeleteString();
		CString strMessage;
		strMessage.Format(_T("%s 이름은 내부함수(Yes Trader)에서 사용중입니다."), lpszScriptName);

		MessageBox(strMessage);
		return FALSE;
	}

	findFunc.DeleteString();

	return TRUE;
}

BOOL CMainFrame::VerifyScriptNameByReservedWord(LPCSTR lpszScriptName, WPARAM wParam)
{
	LPSTR szUpperChar = ::strdup(lpszScriptName);
	::strupr(szUpperChar);
	int L = 0;
	for ( L = 0; s_apszStsKeywordList[L] != NULL; L ++)
	{
		if(strcmp(s_apszStsKeywordList[L], szUpperChar) == 0)
		{
			::free(szUpperChar);

			CString strMessage;
			strMessage.Format(_T("%s 이름은 예약어(Trade Station)로 사용중입니다."), lpszScriptName);

			MessageBox(strMessage);
			return FALSE;
		}
	}

	::free(szUpperChar);
	
	szUpperChar = ::strdup(lpszScriptName);
	::strupr(szUpperChar);
	
	for (L = 0; s_apszStsKeywordList_YES[L] != NULL; L ++)
	{
		if (strcmp(s_apszStsKeywordList_YES[L], szUpperChar) == 0)
		{
			::free(szUpperChar);
			CString strMessage;
			strMessage.Format(_T("%s 이름은 예약어(Yes Trader)로 사용중입니다."), lpszScriptName);

			MessageBox(strMessage);
			return FALSE;
		}
	}

	::free(szUpperChar);

	return TRUE;
}

LRESULT CMainFrame::OnCloseConfigDialog(WPARAM wParam, LPARAM lParam)
{
	// lParam : 1 -> OK
	// lParam : 0 -> Cancel	
	switch(m_nPropertyState)
	{
	case 1:
	case 2:
		if(!lParam)
		{
			::DeleteFile(m_strNewFileName);
			m_nPropertyState = 0;
		}
		else
		{
			OpenNewFile(wParam, lParam);
			m_nPropertyState += 2;
			ChangedProperty(wParam, lParam);
		}
		break;
	case 3:
	case 4:
		if(!lParam)
			m_nPropertyState = 0;
		else
			ChangedProperty(wParam, lParam);
	}

	return 0L;
}

void CMainFrame::OpenNewFile(WPARAM wParam, LPARAM lParam)
{
	LPCSTR szNameTemp = ::strrchr((LPCSTR)wParam, '\\') + 1;
	//LPSTR szAt = ::strchr(szNameTemp, '@');
	LPCSTR szAt = ::strchr(szNameTemp, gSTGubunKey);

	char szFileName[MAX_PATH] = {NULL, };
	::strncpy(szFileName, szNameTemp, szAt - szNameTemp);

	char szPath[MAX_PATH] = {NULL, };
	::lstrcpy(szPath, (LPCSTR)wParam);

	if(m_strNewFileName.CompareNoCase(szPath))
	{
		if(m_nPropertyState == 1)
		{
			CFile file1, file2;
			file1.Open(szPath, CFile::modeWrite);
			file2.Open(m_strNewFileName, CFile::modeRead);

			file1.SeekToEnd();
			ULONGLONG lCount = file2.GetLength();
			LPSTR lpszBuffer = new char[lCount];

			file2.Read(lpszBuffer, lCount);
			file1.Write(lpszBuffer, lCount);

			delete [] lpszBuffer;

			file1.Close();
			file2.Close();
		}

		::DeleteFile(m_strNewFileName);
	}

	CString strFileName(_T(""));
	UINT nFileType = FT_STRATEGY;

	if(m_nPropertyState == 1)
	{
//		UINT nCompiled = (BOOL)::GetPrivateProfileInt(_T("Basic"), _T("Compiled"), IMG_STS, szPath);

		UINT nID = TREEINDEX_DEFUALTITEM;
		if(!m_vtStrategyInfo.empty())
		{
			VT_SCRIPTINFO::reverse_iterator rit = m_vtStrategyInfo.rbegin();
			nID = (*rit)->GetUniqueID() + 1;
		}

		nFileType = FT_STRATEGY;

		CScriptInfomation* pStrategy = new CScriptInfomation(szFileName, nID, nFileType, IMG_STS);
		m_vtStrategyInfo.push_back(pStrategy);

		if(m_pLoadWorkspace)
			m_pLoadWorkspace->AddListStrategy(pStrategy);


//@유진삭제strFileName.Format(_T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, szFileName);
		strFileName.Format(_T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, szFileName);
	}
	else if(m_nPropertyState == 2)
	{
//		BOOL bCompiled = (BOOL)::GetPrivateProfileInt(_T("Basic"), _T("Compiled"), 0, szPath);

		UINT nID = TREEINDEX_DEFUALTITEM;
		if(!m_vtUserFuncInfo.empty())
		{
			VT_SCRIPTINFO::reverse_iterator rit = m_vtUserFuncInfo.rbegin();
			nID = (*rit)->GetUniqueID() + 1;
		}

		nFileType =  FT_USERFUNCTION;

		CScriptInfomation* pUserFunc = new CScriptInfomation(szFileName, nID++, nFileType, IMG_FUNC);
		m_vtUserFuncInfo.push_back(pUserFunc);

		if(m_pLoadWorkspace)
			m_pLoadWorkspace->AddListFunction(pUserFunc);


//@유진삭제strFileName.Format(_T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, szFileName);
		strFileName.Format(_T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, szFileName);
	}
	
	CStdioFile fileProject(strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	int nInputCount = ::GetPrivateProfileInt(_T("Variables"), _T("InputCnt"), 0, szPath);
	if(nInputCount)
	{
		char szKeyName[20] = {NULL, };
		char szKeyName2[20] = {NULL, };
		
		char szInputStatement[256] = {NULL, };
		char szOneInput[100] = {NULL, };
		::lstrcpy(szInputStatement, _T("Inputs: "));

		char szDefault[10] = {NULL, };

		char szValue[128] = {NULL, };
		char szInit[32] = {NULL, };
		for(int i = 0; i < nInputCount; i++)
		{
			::wsprintf(szKeyName, _T("InputName%d"), i);
			::wsprintf(szKeyName2, _T("Input%d"), i);
			::wsprintf(szDefault, _T("Value%d"), i);

			if(i)
				::lstrcat(szInputStatement, _T(", "));

			::GetPrivateProfileString(_T("Variables"), szKeyName, szDefault, szValue, sizeof(szValue), szPath);
			::GetPrivateProfileString(_T("Variables"), szKeyName2, _T("0"), szInit, sizeof(szInit), szPath);
			::wsprintf(szOneInput, _T("%s(%s)"), szValue, szInit);

			::lstrcat(szInputStatement, szOneInput);
		}

		::lstrcat(szInputStatement, _T(";"));

		fileProject.WriteString(szInputStatement);
		
		char szNewLine[2] = {0x0a, 0x0d};
		fileProject.Write(szNewLine, 1);
	}

	fileProject.Close();

	::wsprintf(szPath, _T("%s.bak"), strFileName);
	::CopyFile(strFileName, szPath, FALSE);

	EncodingFile(szPath, strFileName);
	::DeleteFile(szPath);
	
	CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.OpenDocumentFile(strFileName);

//	ChangeProperty(FALSE, szFileName, nFileType);
//	ChangeProperty(TRUE, szFileName, nFileType);
}

void CMainFrame::LoadConfigDlg()
{
	if(m_pLoadConfig)
		delete m_pLoadConfig;

	m_pLoadConfig = new CLoadSystemTradingConfigDll;
}

void CMainFrame::OpenPropertyDlg(LPCSTR szFile, int nFileType)
{
	LoadConfigDlg();

	m_nPropertyState = nFileType;		// Property
	m_pLoadConfig->ShowConfigureDlg(m_hWnd, NULL, szFile, nFileType);
}

void CMainFrame::ChangedProperty(WPARAM wParam, LPARAM lParam)
{
	LPCSTR szNameTemp = ::strrchr((LPCSTR)wParam, '\\') + 1;
	//LPSTR szAt = ::strchr(szNameTemp, '@');
	LPCSTR szAt = ::strchr(szNameTemp, gSTGubunKey);

	char szFileName[MAX_PATH] = {NULL, };
	::strncpy(szFileName, szNameTemp, szAt - szNameTemp);

	ChangeProperty(FALSE, szFileName, m_nPropertyState);
	ChangeProperty(TRUE, szFileName, m_nPropertyState);

	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		if(::lstrcmp(pDoc->GetScriptName(), szFileName) == 0)
		{
			BOOL bPassword = m_pLoadConfig->GetUsePassword();
			
			BOOL bModified = pDoc->SetScriptProperty(TRUE,
			bPassword, m_pLoadConfig->GetPassword(),
			m_pLoadConfig->GetUseUserID(), m_pLoadConfig->GetUserID(),
			m_pLoadConfig->GetUseExpiredDate(), m_pLoadConfig->GetExpiredDate());

			int nOption = 0;
			if(bModified)
			{
				if(m_nPropertyState == FT_STRATEGY)
					nOption = bPassword ? IMG_STS_LOCK : IMG_STS;
				else if(m_nPropertyState == FT_USERFUNCTION)
					nOption = bPassword ? IMG_FUNC_LOCK : IMG_FUNC;

				ChangeCompiledOption(szFileName, m_nPropertyState, nOption);
			}

			return;
		}
	}

	// 열려있는 스크립트가 없을때
	char szFile[MAX_PATH] = {NULL, };
	if(m_nPropertyState == FT_STRATEGY)
//@유진삭제::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, szFileName);
		::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, szFileName);		

	else if(m_nPropertyState == FT_USERFUNCTION)

//@유진삭제::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, szFileName);
		::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, szFileName);		
	
	CString strExt;
	strExt.Format(_T("%s.bak"), szFile);
	::CopyFile(szFile, strExt, FALSE);
	
	char szLine[MAX_PATH + 1] = {NULL, };
	DecodingFile(strExt, szFile, szLine);
	
	CScriptSecurity* pInfo = new CScriptSecurity(m_nPropertyState, szFileName, szLine);
	
	BOOL bUsePassword = m_pLoadConfig->GetUsePassword();
	CString strPassword = m_pLoadConfig->GetPassword();

	BOOL bUseUserID = m_pLoadConfig->GetUseUserID();
	CString strUserID = m_pLoadConfig->GetUserID();

	BOOL bUseExpires = m_pLoadConfig->GetUseExpiredDate();
	CString strExpires = m_pLoadConfig->GetExpiredDate();

	BOOL bModified = FALSE;
	bModified = pInfo->GetUsePassword() != bUsePassword;
	if(!bModified)
		bModified = pInfo->GetPassword().Compare(strPassword);

	if(!bModified)
		bModified = pInfo->GetUseUserID() != bUseUserID;
	if(!bModified)
		bModified = pInfo->GetUserID().Compare(strUserID);

	if(!bModified)
		bModified = pInfo->GetUseExpires() != bUseExpires;
	if(!bModified)
		bModified = pInfo->GetExpires().Compare(strExpires);

	delete pInfo;

	if(bModified)
	{
		// 1,password,1,userid,0,expires
		::wsprintf(szLine, _T("%d,%s,%d,%s,%d,%s"), 
			bUsePassword, strPassword, 
			bUseUserID, strUserID, 
			bUseExpires, strExpires);
		
		EncodingFile(szFile, (LPCSTR)strExt, szLine);
		
		int nOption = 0;
		if(bModified)
		{
			if(m_nPropertyState == FT_STRATEGY)
				nOption = bUsePassword ? IMG_STS_LOCK : IMG_STS;
			else if(m_nPropertyState == FT_USERFUNCTION)
				nOption = bUsePassword ? IMG_FUNC_LOCK : IMG_FUNC;

			ChangeCompiledOption(szFileName, m_nPropertyState, nOption);
		}
	}

	::CopyFile(strExt, szFile, FALSE);
	::DeleteFile(strExt);
}

LRESULT CMainFrame::OnDelScript(WPARAM wParam, LPARAM lParam)
{
	if(wParam == FT_STRATEGY)	// Delete the strategy file.
		DeleteStrategyScript((UINT)lParam);
	else if(wParam == FT_USERFUNCTION)	// Delete the user function file.
		DeleteFunctionScript((UINT)lParam);

	return 0L;
}

void CMainFrame::DeleteStrategyScript(UINT nID)
{
	CScriptFind findScript(nID);
	
	VT_SCRIPTINFO::iterator it = std::find_if(m_vtStrategyInfo.begin(), m_vtStrategyInfo.end(), findScript);
	if(it == m_vtStrategyInfo.end())
		return;

	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		if(::lstrcmp(pDoc->GetScriptName(), (*it)->GetFunctionName()) == 0)
		{
			pDoc->SetModifiedFlag(FALSE);
			POSITION posView = pDoc->GetFirstViewPosition();
			CSystemTradingView* pView = (CSystemTradingView*)pDoc->GetNextView(posView);
			pView->GetParentFrame()->SendMessage(WM_CLOSE);
			break;
		}
	}

	char szFile[MAX_PATH] = {NULL, };
	::GetSTFile(szFile, (*it)->GetFunctionName(), theApp.GetRootPath());

	UINT nCompiled = IMG_STS;
	nCompiled = ::GetPrivateProfileInt(_T("Basic"), _T("Compiled"), nCompiled, szFile);

	//>>0002010: [언어편집기] - 언어편집기에서 컴파일하여 파일생성 후 스크립트 삭제를 하여도 실제 폴더에서는 파일이 사라지지 않음
 	::DeleteFile(szFile);

 	char szDll[MAX_PATH] = {NULL, };

	::wsprintf(szDll, _T("%s%s%s.sd"), theApp.GetUserPath(), PATH_DLLFILES2, (*it)->GetFunctionName());
	::DeleteFile(szDll);

	::wsprintf(szDll, _T("%s%s%s.sfg"), theApp.GetUserPath(), PATH_DLLFILES2, (*it)->GetFunctionName());
	::DeleteFile(szDll);
	//<<0002010

//@유진삭제::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, (*it)->GetFunctionName());
	::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, (*it)->GetFunctionName());		
	::SetFileAttributes(szFile, FILE_ATTRIBUTE_NORMAL);
	::DeleteFile(szFile);


//@유진삭제::wsprintf(szFile, _T("%s%s%s.sts.bak"), theApp.GetRootPath(), PATH_SCRIPTFILES, (*it)->GetFunctionName());
	::wsprintf(szFile, _T("%s%s%s.sts.bak"), theApp.GetUserPath(), PATH_SCRIPTFILES2, (*it)->GetFunctionName());		
	::DeleteFile(szFile);

	DeleteScriptInfo((*it));
	m_vtStrategyInfo.erase(it);

	ReloadScriptInfoToExtLib();
}

void CMainFrame::DeleteFunctionScript(UINT nID)
{
	CScriptFind findScript(nID);
	
	VT_SCRIPTINFO::iterator it = std::find_if(m_vtUserFuncInfo.begin(), m_vtUserFuncInfo.end(), findScript);
	if(it == m_vtUserFuncInfo.end())
		return;

	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		if(::lstrcmp(pDoc->GetScriptName(), (*it)->GetFunctionName()) == 0)
		{
			pDoc->SetModifiedFlag(FALSE);
			POSITION posView = pDoc->GetFirstViewPosition();
			CSystemTradingView* pView = (CSystemTradingView*)pDoc->GetNextView(posView);
			pView->GetParentFrame()->SendMessage(WM_CLOSE);
			break;
		}
	}

	char szFile[MAX_PATH] = {NULL, };
	::GetSTFile(szFile, (*it)->GetFunctionName(), theApp.GetRootPath());
	::DeleteFile(szFile);
	

//@유진삭제::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, (*it)->GetFunctionName());
	::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, (*it)->GetFunctionName());		
	::DeleteFile(szFile);

	DeleteScriptInfo((*it));
	m_vtUserFuncInfo.erase(it);
}

void CMainFrame::SingleComileDllMain(BOOL bMakeDll, LPSTR szName, UINT nScriptType, CScriptSecurity* pInfo/* = NULL*/)
{
	char szSaveFile[MAX_PATH] = {NULL, };

	if(m_pLoadWorkspace && !m_bMultiCompile)
		m_pLoadWorkspace->ClearAllOutputText();

	if(!m_pLoadPaser)
		LoadSystemTradingParser(nScriptType);


//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, szName);
	::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, szName);

	BOOL bIndexFunction = FALSE;
	CErrorInfomation* pErrorInfo = new CErrorInfomation(FT_STRATEGY, nScriptType);

	CSystemTradingDoc* pDoc;
	if(pInfo == NULL)
	{
		POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
		while(posDoc)
		{
			pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
			if(::lstrcmp(pDoc->GetScriptName(), szName) == 0)
			{
				pInfo = pDoc->GetScriptSecurity();
				break;
			}
		}
	}

	int nError = m_pLoadPaser->ParseMainToCPP(szSaveFile, pErrorInfo, bIndexFunction, pInfo);
	if(nError == 0)
	{		
		if(bMakeDll)
		{
			m_bCompiling = TRUE;
			m_ctrlCompileProgress.ShowWindow(SW_SHOW);
			m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_CERT);
		}

		AddIndexFunctionList(szName, bIndexFunction);

		SetOutputText(CM_PARSE_COMPLETE_STRATEGY, szName);
		
		ChangeCompiledOption(szName, FT_STRATEGY, IMG_STS_CERT);
		
		if(bMakeDll)
		{
			m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_READYDLL);
			MakeFileMainStrategy(szName, pErrorInfo->GetStrategyType());
		}
		else
		{
//			::wsprintf(szSaveFile, _T("%s%sOutput\\STSample.cpp"), theApp.GetRootPath(), PATH_STDATA);
//			::DeleteFile(szSaveFile);

			m_nStrategyType = pErrorInfo->GetStrategyType();
			if(m_nStrategyType != STRATEGY_FIND)
				m_nStrategyType = 0;

			MakeSTFile(szName, NULL);
			m_nStrategyType = 0;
		}

		//pDoc->SaveFile();	//0003170: [오류][상] 언어편집기에서 컴파일시 식에 문제가 없는데 컴파일 되지 않는 현상.
		ReloadScriptInfoToExtLib();

		if(IsWindow(m_hPctrWnd))
		{
			::SendMessage(m_hPctrWnd, UMSG_LANGUAGE_RELOAD, 0, 0);
		}
	}
	else if(nError > 0)
	{
		m_bCompiling = FALSE;
		m_ctrlCompileProgress.ShowWindow(SW_HIDE);
		
		if(nError == 1)
		{
			nError = pErrorInfo->GetErrorIndex();
			if(nError == 0)
				nError = CM_UNKNOWN_ERROR_STRATEGY;
		}

		pErrorInfo->SetErrorFileName(szSaveFile);
		SetOutputText(nError, szName, pErrorInfo);

//		m_bMultiCompile = FALSE;
		m_nScriptNameCount = 0;

		WriteEndDllFile();
	}

	delete pErrorInfo;
	DeleteSystemTradingParser();

	if(ScanScriptFile(szName, FT_STRATEGY, nScriptType))
	{
		ChangeProperty(FALSE, szName, FT_STRATEGY);
		ChangeProperty(TRUE, szName, FT_STRATEGY);
	}
}

LRESULT CMainFrame::OnMultiCompileDLLMainTree(WPARAM wParam, LPARAM lParam)
{
	if(m_pLoadWorkspace && !m_bMultiCompile)
		m_pLoadWorkspace->ClearAllOutputText();

	m_bMultiCompile = TRUE;
	m_nScriptNameCount = 0;

	CStringArray* pStarScriptName = (CStringArray*)lParam;
	int nCount = pStarScriptName->GetSize();
	
	// 컴파일서버에 컴파일 요청일 경우
	if((BOOL)wParam)
	{
		float fTime = (float)nCount * 3.8;		// 기본 3.8초로 간주
		CString strMessage;
		strMessage.Format(_T("%d개의 전략 스크립트 컴파일을 요청합니다.\n총 소요시간 %.1f초가 예상됩니다."), nCount, fTime);

		if(AfxMessageBox(strMessage, MB_OKCANCEL) == IDOK)
		{
			m_starScriptName.RemoveAll();
			m_starScriptName.Copy(*pStarScriptName);
			OnSingleCompileDLLMainTree(wParam, (LPARAM)(LPSTR)(LPCTSTR)m_starScriptName.GetAt(m_nScriptNameCount++));
		}
		return 0L;
	}

	for(int i = 0; i < nCount; i++)
	{
		OnSingleCompileDLLMainTree(wParam, (LPARAM)(LPSTR)(LPCTSTR)pStarScriptName->GetAt(i));
		m_pLoadWorkspace->GetOutputWnd()->UpdateWindow();
	}

	m_bMultiCompile = FALSE;

	return 0L;
}

LRESULT CMainFrame::OnMultiCompileDLLFunctionTree(WPARAM wParam, LPARAM lParam)
{
	if(m_pLoadWorkspace && !m_bMultiCompile)
		m_pLoadWorkspace->ClearAllOutputText();

	m_bMultiCompile = TRUE;
	m_nScriptNameCount = 0;

	CStringArray* pStarScriptName = (CStringArray*)lParam;
	int nCount = pStarScriptName->GetSize();

	// 컴파일서버에 컴파일 요청일 경우
	if((BOOL)wParam)
	{
		float fTime = (float)nCount * 3.8;		// 기본 3.8초로 간주
		CString strMessage;
		strMessage.Format(_T("%d개의 함수 스크립트 컴파일을 요청합니다.\n총 소요시간 %.1f초가 예상됩니다."), nCount, fTime);

		if(AfxMessageBox(strMessage, MB_OKCANCEL) == IDOK)
		{
			m_starScriptName.RemoveAll();
			m_starScriptName.Copy(*pStarScriptName);
			OnSingleCompileDLLFunctionTree(wParam, (LPARAM)(LPSTR)(LPCTSTR)m_starScriptName.GetAt(m_nScriptNameCount++));
		}
		return 0L;
	}

	for(int i = 0; i < nCount; i++)
	{
		OnSingleCompileDLLFunctionTree(wParam, (LPARAM)(LPSTR)(LPCTSTR)pStarScriptName->GetAt(i));
		m_pLoadWorkspace->GetOutputWnd()->UpdateWindow();
	}

	m_bMultiCompile = FALSE;

	return 0L;
}

LRESULT CMainFrame::OnSingleCompileDLLMainTree(WPARAM wParam, LPARAM lParam)
{
	BOOL bMakeDll = (BOOL)wParam;
	LPSTR lpszName = (LPSTR)lParam;

	char szPath[MAX_PATH] = {NULL, };
	::GetSTFile(szPath, lpszName, theApp.GetRootPath());

	int nScriptType = ::GetPrivateProfileInt(_T("Basic"), _T("LangType"), 0, szPath);

	BOOL bOpenedScript = FALSE;
	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		if(::lstrcmp(pDoc->GetScriptName(), lpszName) == 0)
		{
			bOpenedScript = TRUE; 
			break;
		}
	}
	
	CScriptSecurity* pInfo = NULL;

	if(!bOpenedScript)
	{
		char szFile[MAX_PATH] = {NULL, };

//@유진삭제::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszName);
		::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszName);
		
		CString strExt;
		strExt.Format(_T("%s.bak"), szFile);
		::CopyFile(szFile, strExt, FALSE);
		
		char szLine[MAX_PATH + 1] = {NULL, };
		DecodingFile(strExt, szFile, szLine);
		
		pInfo = new CScriptSecurity(nScriptType, lpszName, szLine);

		if(pInfo->GetUsePassword())
		{
			CDlgPassword dlg;
			if(dlg.DoModal() == IDOK)
			{
				if(dlg.m_strPassword.Compare(pInfo->GetPassword()))
				{
					AfxMessageBox(_T("올바른 비밀번호가 아닙니다."));
					
					delete pInfo;
					EncodingFile(szFile, strExt, szLine);
					
					::CopyFile(strExt, szFile, FALSE);
					::DeleteFile(strExt);

					return 0L;
				}
			}
			else
			{
				delete pInfo;
				EncodingFile(szFile, strExt, szLine);
				
				::CopyFile(strExt, szFile, FALSE);
				::DeleteFile(strExt);

				return 0L;
			}			
		}

		SingleComileDllMain(bMakeDll, lpszName, nScriptType, pInfo);

		EncodingFile(szFile, strExt, szLine);
		
		::CopyFile(strExt, szFile, FALSE);
		::DeleteFile(strExt);
	}
	else
		SingleComileDllMain(bMakeDll, lpszName, nScriptType, pInfo);

	if(pInfo)
		delete pInfo;

	return 0L;
}

LRESULT CMainFrame::OnSingleCompileDLLFunctionTree(WPARAM wParam, LPARAM lParam)
{
	BOOL bMakeDll = (BOOL)wParam;
	LPSTR lpszName = (LPSTR)lParam;

	char szPath[MAX_PATH] = {NULL, };
	::GetSTFile(szPath, lpszName, theApp.GetRootPath());

	int nScriptType = ::GetPrivateProfileInt(_T("Basic"), _T("LangType"), 0, szPath);

	BOOL bOpenedScript = FALSE;
	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		if(::lstrcmp(pDoc->GetScriptName(), lpszName) == 0)
		{
			bOpenedScript = TRUE; 
			break;
		}
	}

	CScriptSecurity* pInfo = NULL;
	
	if(!bOpenedScript)
	{
		char szFile[MAX_PATH] = {NULL, };

//@유진삭제::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszName);
		::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszName);
		
		CString strExt;
		strExt.Format(_T("%s.bak"), szFile);
		::CopyFile(szFile, strExt, FALSE);
		
		char szLine[MAX_PATH + 1] = {NULL, };
		DecodingFile(strExt, szFile, szLine);
		
		pInfo = new CScriptSecurity(nScriptType, lpszName, szLine);

		if(pInfo->GetUsePassword())
		{
			CDlgPassword dlg;
			if(dlg.DoModal() == IDOK)
			{
				if(dlg.m_strPassword.Compare(pInfo->GetPassword()))
				{
					AfxMessageBox(_T("올바른 비밀번호가 아닙니다."));
					
					delete pInfo;
					EncodingFile(szFile, strExt, szLine);
					
					::CopyFile(strExt, szFile, FALSE);
					::DeleteFile(strExt);

					return 0L;
				}
			}
			else
			{
				delete pInfo;
				EncodingFile(szFile, strExt, szLine);
				
				::CopyFile(strExt, szFile, FALSE);
				::DeleteFile(strExt);

				return 0L;
			}			
		}
		SingleComileDllFunction(bMakeDll, lpszName, nScriptType, pInfo);

		EncodingFile(szFile, strExt, szLine);
		
		::CopyFile(strExt, szFile, FALSE);
		::DeleteFile(strExt);
	}
	else
		SingleComileDllFunction(bMakeDll, lpszName, nScriptType, pInfo);

	if(pInfo)
		delete pInfo;

	return 0L;
}

LRESULT CMainFrame::OnSingleCompileDLLMain(WPARAM wParam, LPARAM lParam)
{
	BOOL bMakeDll = (BOOL)wParam;
	LPSTR szName = (LPSTR)lParam;

	SingleComileDllMain(bMakeDll, szName, ST_TRADESTATION);
	
	return 0L;
}

LRESULT CMainFrame::OnSingleCompileDLLMain_YES(WPARAM wParam, LPARAM lParam)
{
	BOOL bMakeDll = (BOOL)wParam;
	LPSTR szName = (LPSTR)lParam;

	SingleComileDllMain(bMakeDll, szName, ST_YESTRADER);
	
	return 0L;
}

void CMainFrame::SingleComileDllFunction(BOOL bMakeDll, LPSTR szName, UINT nScriptType, CScriptSecurity* pInfo/* = NULL*/)
{
	char szSaveFile[MAX_PATH] = {NULL, };

	if(m_pLoadWorkspace && !m_bMultiCompile)
		m_pLoadWorkspace->ClearAllOutputText();

	if(!m_pLoadPaser)
		LoadSystemTradingParser(nScriptType);


//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, szName);
	::wsprintf(szSaveFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, szName);

	CErrorInfomation* pErrorInfo = new CErrorInfomation(FT_USERFUNCTION, nScriptType);

	if(pInfo == NULL)
	{
		POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
		while(posDoc)
		{
			CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
			if(::lstrcmp(pDoc->GetScriptName(), szName) == 0)
			{
				pInfo = pDoc->GetScriptSecurity();
				break;
			}
		}
	}

	int nError = m_pLoadPaser->ParseUserFunctionToCPP(szSaveFile, szName, pErrorInfo, pInfo);
	if(nError == 0)
	{
		if(bMakeDll)
		{
			m_bCompiling = TRUE;
			m_ctrlCompileProgress.ShowWindow(SW_SHOW);
			m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_CERT);
		}

		SetOutputText(CM_PARSE_COMPLETE_FUNCTION, szName);
		
		ChangeCompiledOption(szName, FT_USERFUNCTION, IMG_FUNC_CERT);
		
		if(bMakeDll)
		{
			MakeFileUserFunction(szName);
			m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_READYDLL);
		}
//		else
//		{
//			::wsprintf(szSaveFile, _T("%s%sOutput\\%s.STSample.cpp"), theApp.GetRootPath(), PATH_STDATA, szName);
//			::DeleteFile(szSaveFile);
//		}
	}
	else if(nError > 0)
	{
		m_bCompiling = FALSE;
		m_ctrlCompileProgress.ShowWindow(SW_HIDE);

		if(nError == 1)
		{
			nError = pErrorInfo->GetErrorIndex();
			if(nError == 0)
				nError = CM_UNKNOWN_ERROR_FUNCTION;
		}

		pErrorInfo->SetErrorFileName(szSaveFile);
		SetOutputText(nError, szName, pErrorInfo);

//		m_bMultiCompile = FALSE;
		m_nScriptNameCount = 0;

		WriteEndDllFileFunc();
	}

	delete pErrorInfo;
	DeleteSystemTradingParser();

	if(ScanScriptFile(szName, FT_USERFUNCTION, nScriptType))
	{
		ChangeProperty(FALSE, szName, FT_USERFUNCTION);
		ChangeProperty(TRUE, szName, FT_USERFUNCTION);
	}
}

LRESULT CMainFrame::OnSingleCompileDLLFunction(WPARAM wParam, LPARAM lParam)
{	
	BOOL bMakeDll = (BOOL)wParam;
	LPSTR szName = (LPSTR)lParam;
	
	SingleComileDllFunction(bMakeDll, szName, ST_TRADESTATION);

	return 0L;
}

LRESULT CMainFrame::OnSingleCompileDLLFunction_YES(WPARAM wParam, LPARAM lParam)
{	
	BOOL bMakeDll = (BOOL)wParam;
	LPSTR szName = (LPSTR)lParam;
	
	SingleComileDllFunction(bMakeDll, szName, ST_YESTRADER);

	return 0L;
}

LRESULT CMainFrame::OnCompileDLL(WPARAM wParam, LPARAM lParam)
{
	BOOL bMakeDll = (BOOL)wParam;

	LPSTR szName = (LPSTR)lParam;
	char szSaveFile[MAX_PATH] = {NULL, };

	if(m_pLoadWorkspace)
		m_pLoadWorkspace->ClearAllOutputText();

	char szPath[MAX_PATH] = {NULL, };
	::GetSTFile(szPath, szName, theApp.GetRootPath());
	
	int nValue = ::GetPrivateProfileInt(BASIC, LANGTYPE, 0, szPath);

	if(!m_pLoadPaser)
		LoadSystemTradingParser(nValue);
	
	::wsprintf(szSaveFile, _T("%s%sOutput\\*.cpp"), theApp.GetRootPath(), PATH_STDATA);
	
	CFileFind ff;
	BOOL bFind = ff.FindFile(szSaveFile);
	while(bFind)
	{
		bFind = ff.FindNextFile();
		::DeleteFile(ff.GetFilePath());
	}

//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, szName);
	::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, szName);

	CErrorInfomation* pErrorInfo = new CErrorInfomation(FT_STRATEGY, nValue);

	CString strExt;
	strExt.Format(_T("%s.bak"), szSaveFile);
	::CopyFile(szSaveFile, strExt, FALSE);
	
	char szLine[MAX_PATH + 1] = {NULL, };
	DecodingFile(strExt, szSaveFile, szLine);
	
	CScriptSecurity* pInfo = new CScriptSecurity(nValue, szName, szLine);
	
	EncodingFile(szSaveFile, strExt, szLine);
	
	::CopyFile(strExt, szSaveFile, FALSE);
	::DeleteFile(strExt);

	BOOL bIndexFunction = FALSE;
	int nError = m_pLoadPaser->ParseFileToCPP(szSaveFile, pErrorInfo, bIndexFunction, pInfo);
	delete pInfo;

	if(nError == 0)
	{
		if(bMakeDll)
		{
			m_bCompiling = TRUE;
			m_ctrlCompileProgress.ShowWindow(SW_SHOW);
			m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_CERT);
		}

		AddIndexFunctionList(szName, bIndexFunction);
		SetOutputText(CM_PARSE_COMPLETE_ALL, szName);
		
		if(bMakeDll)
		{
			m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_READYDLL_ZIP);
			MakeFileAllScirpt(szName);
		}
	}
	else if(nError > 0)
	{
		m_bCompiling = FALSE;
		m_ctrlCompileProgress.ShowWindow(SW_HIDE);
//		pErrorInfo->SetErrorFileName(szSaveFile);

		LPCSTR lpszFile = pErrorInfo->GetFileName();

		LPCSTR lpszName = ::strrchr(lpszFile, '\\') + 1;
		int nEnd = ::lstrlen(lpszName) - ::lstrlen(::strrchr(lpszName, '.'));

		::strset(szSaveFile, 0x00);
		::strncpy(szSaveFile, lpszName, nEnd);
		
		if(nError == 1)
		{
			nError = pErrorInfo->GetErrorIndex();

			if(nError == 0)
				nError = pErrorInfo->GetFileType() == FT_STRATEGY ? CM_UNKNOWN_ERROR_STRATEGY : CM_UNKNOWN_ERROR_FUNCTION;
		}

		SetOutputText(nError, szSaveFile, pErrorInfo);

//		m_bMultiCompile = FALSE;
		m_nScriptNameCount = 0;

		WriteEndDllFile();
	}
	
	delete pErrorInfo;
	DeleteSystemTradingParser();
	
	if(ScanScriptFile(szName, FT_STRATEGY, ST_TRADESTATION))
	{
		ChangeProperty(FALSE, szName, FT_STRATEGY);
		ChangeProperty(TRUE, szName, FT_STRATEGY);
	}
	
	return 0L;
}

LRESULT CMainFrame::OnViewProperty(WPARAM wParam, LPARAM lParam)
{
	int nFileType = 0;
	char szPath[MAX_PATH] = {NULL, };
	CScriptSecurity* pInfo = NULL;

	if(wParam == 0)
	{
		pInfo = (CScriptSecurity*)lParam;
		
		CString strName(_T(""));
		
		pInfo->GetScriptInfo(nFileType, strName);
		
		::GetSTFile(szPath, strName, theApp.GetRootPath());
//		::wsprintf(szPath, _T("\\%s."), strName);		// ConfigDlg의 스크립트명 파싱에 맞춘것

		LoadConfigDlg();

		m_nPropertyState = nFileType;		// Property
		m_pLoadConfig->SetUsePassword(pInfo->GetUsePassword());
		m_pLoadConfig->SetPassword(pInfo->GetPassword());

		m_pLoadConfig->SetUseUserID(pInfo->GetUseUserID());
		m_pLoadConfig->SetUserID(pInfo->GetUserID());

		m_pLoadConfig->SetUseExpiredDate(pInfo->GetUseExpires());
		m_pLoadConfig->SetExpiredDate(pInfo->GetExpires());
	}
	else
	{
		LPSTR lpszName = (LPSTR)lParam;
		::GetSTFile(szPath, lpszName, theApp.GetRootPath());

		BOOL bOpenedScript = FALSE;
		POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
		while(posDoc)
		{
			CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
			if(::lstrcmp(pDoc->GetScriptName(), lpszName) == 0)
			{
				bOpenedScript = TRUE; 
				pInfo = pDoc->GetScriptSecurity();
				break;
			}
		}
		
		if(!bOpenedScript || !pInfo)
		{
			char szFile[MAX_PATH] = {NULL, };
			if(wParam == FT_STRATEGY)

//@유진삭제		::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszName);
				::wsprintf(szFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszName);

			else if(wParam == FT_USERFUNCTION)

//@유진삭제		::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszName);
				::wsprintf(szFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszName);
			
			CString strExt;
			strExt.Format(_T("%s.bak"), szFile);
			::CopyFile(szFile, strExt, FALSE);
			
			char szLine[MAX_PATH + 1] = {NULL, };
			DecodingFile(strExt, szFile, szLine);
			
			EncodingFile(szFile, strExt, szLine);
			
			::CopyFile(strExt, szFile, FALSE);
			::DeleteFile(strExt);
			
			pInfo = new CScriptSecurity(wParam, lpszName, szLine);
		}
	
		if(pInfo->GetUsePassword())
		{
			CDlgPassword dlg;
			if(dlg.DoModal() == IDOK)
			{
				if(dlg.m_strPassword.Compare(pInfo->GetPassword()))
				{
					AfxMessageBox(_T("올바른 비밀번호가 아닙니다."));
					
					if(!bOpenedScript)
						delete pInfo;

					return 0L;
				}
			}
			else
			{
				if(!bOpenedScript)
					delete pInfo;

				return 0L;
			}			
		}

		m_nPropertyState = wParam;		// Property
		nFileType = m_nPropertyState;

		LoadConfigDlg();

		m_pLoadConfig->SetUsePassword(pInfo->GetUsePassword());
		m_pLoadConfig->SetPassword(pInfo->GetPassword());

		m_pLoadConfig->SetUseUserID(pInfo->GetUseUserID());
		m_pLoadConfig->SetUserID(pInfo->GetUserID());

		m_pLoadConfig->SetUseExpiredDate(pInfo->GetUseExpires());
		m_pLoadConfig->SetExpiredDate(pInfo->GetExpires());

		if(!bOpenedScript)
			delete pInfo;
	}

	m_pLoadConfig->ShowConfigureDlg(m_hWnd, NULL, szPath, nFileType);

	return 0L;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if(pCopyDataStruct->dwData == 200)
	{
		ST_MAKEFILEDLL* prData = (ST_MAKEFILEDLL*)pCopyDataStruct->lpData;
		CompileSucceed(prData);
	}
	else if(pCopyDataStruct->dwData == 300)
	{
		ST_MAKEFILEDLL* prData = (ST_MAKEFILEDLL*)pCopyDataStruct->lpData;
		if(prData->nFileType == FT_STRATEGY)
			SetOutputText(CM_MAKE_FAILED_STRATEGY, prData->szName);
		else if(prData->nFileType == FT_USERFUNCTION)
			SetOutputText(CM_MAKE_FAILED_FUNCTION, prData->szName);
	}
	else if(pCopyDataStruct->dwData == 1000)
	{
		LPCTSTR lpszFileName = (LPCTSTR)pCopyDataStruct->lpData;
	
		char szFile[MAX_PATH] = {NULL, };

//@유진삭제	::wsprintf(szFile, _T("%s%s%s"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszFileName);
		::wsprintf(szFile, _T("%s%s%s"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszFileName);

		theApp.OpenDocumentFile(szFile);
	}
	else if(pCopyDataStruct->dwData ==2000)//KHD : Pctr 핸들 갖구 오기..
	{
		LPCSTR stHandle = (LPCSTR)	pCopyDataStruct->lpData;
		sscanf(stHandle,"%X", &m_hPctrWnd);
	}
	
	return CMDIFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::CompileSucceed(LPVOID prData)
{
	ST_MAKEFILEDLL* pMakeFile = (ST_MAKEFILEDLL*)prData;
	
	char szMakeFile[MAX_PATH] = {NULL, };
	char szDll[MAX_PATH] = {NULL, };

	if(pMakeFile->nFileType == FT_STRATEGY)
	{
		ChangeCompiledOption(pMakeFile->szName, FT_STRATEGY, TRUE);
//@유진삭제::wsprintf(szDll, _T("%s%s%s.sd"), theApp.GetRootPath(), PATH_DLLFILES, pMakeFile->szName);
		::wsprintf(szDll, _T("%s%s%s.sd"), theApp.GetUserPath(), PATH_DLLFILES2, pMakeFile->szName);

		SetOutputText(CM_MAKE_COMPLETE_STRATEGY, pMakeFile->szName);
		//AfxMessageBox("컴파일완료3");
		CFileFind ff;
		if(ff.FindFile(szDll))
		{
			if(!::DeleteFile(szDll))
			{
				SetOutputText(CM_COPY_FAILED_STRATEGY, pMakeFile->szName);
					//AfxMessageBox("컴파일완료2");
				return;
			}
		}

		::wsprintf(szMakeFile, _T("%s\\Server\\Release\\STSample\\%s.sd"), theApp.GetRootPath(), pMakeFile->szName);
	}
	else if(pMakeFile->nFileType == FT_USERFUNCTION)
	{
		ChangeCompiledOption(pMakeFile->szName, FT_USERFUNCTION, TRUE);
//@유진삭제::wsprintf(szDll, _T("%s%s%s.sdf"), theApp.GetRootPath(), PATH_DLLFILES, pMakeFile->szName);
		::wsprintf(szDll, _T("%s%s%s.sdf"), theApp.GetUserPath(), PATH_DLLFILES2, pMakeFile->szName);

		SetOutputText(CM_MAKE_COMPLETE_FUNCTION, pMakeFile->szName);
		AfxMessageBox("컴파일완료4");
		CFileFind ff;
		if(ff.FindFile(szDll))
		{
			if(!::DeleteFile(szDll))
			{
				SetOutputText(CM_COPY_FAILED_FUNCTION, pMakeFile->szName);
					AfxMessageBox("1컴파일완료");
				return;
			}

		}
		::wsprintf(szMakeFile, _T("%s\\Server\\Release\\STSample\\%s.sdf"), theApp.GetRootPath(), pMakeFile->szName);
	}

	::MoveFile(szMakeFile, szDll);
}

void CMainFrame::ChangeProperty(BOOL bActive, LPCSTR lpszName/* = NULL*/, UINT nID/* = 0*/)
{
	if(!m_pPrptyScriptName)
		return;

	CSystemTradingDoc* pDoc = NULL;
	BOOL bExistScreen = FALSE;
	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		if(::lstrcmp(pDoc->GetScriptName(), lpszName) == 0)
		{
			bExistScreen = TRUE;
			break;
		}
	}

	if(!bExistScreen || !pDoc)
		return;
	
	if(bActive)
	{
		if(nID == FT_STRATEGY)
		{
			m_pPrptyScriptName->SetCaption(_T("전략이름"));
			if(m_pLoadWorkspace)
				m_pLoadWorkspace->EnsureVisibleFunctionInStrategy(lpszName);
		}
		else if(nID == FT_USERFUNCTION)
			m_pPrptyScriptName->SetCaption(_T("함수이름"));

		m_pPrptyScriptName->SetValue(lpszName);

		char szFile[MAX_PATH] = {NULL, };
		::GetSTFile(szFile, lpszName, theApp.GetRootPath());

		char szValue[512 + 1] = {NULL, };
		::GetPrivateProfileString(BASIC, EXPLANATION, _T(""), szValue, sizeof(szValue), szFile);

		m_pPrptyScriptComment->SetValue(szValue);

		int nValue = ::GetPrivateProfileInt(BASIC, LANGTYPE, 0, szFile);
		if(nValue)
			m_pPrptyScriptLanguage->SetValue(_T("BASIC"));
		else
			m_pPrptyScriptLanguage->SetValue(_T("PASCAL"));
		
		nValue = ::GetPrivateProfileInt(BASIC, _T("Compiled"), 0, szFile);
		if(nValue == IMG_STS || nValue == IMG_STS_LOCK || nValue == IMG_FUNC || nValue == IMG_FUNC_LOCK)
			m_pPrptyScriptCheck->SetValue(_T("검증미완료"));
		else
			m_pPrptyScriptCheck->SetValue(_T("검증완료"));

		nValue = ::GetPrivateProfileInt(VARIABLES, INPUTCNT, 0, szFile);
		
		char szKeyName[20] = {NULL, };
		char szKeyName2[20] = {NULL, };
		char szDefault[10] = {NULL, };
		char szInit[32] = {NULL, };
		int i = 0;
		for( i = 0; i < nValue; i++)
		{
			::wsprintf(szKeyName, _T("InputName%d"), i);
			::wsprintf(szKeyName2, _T("Input%d"), i);
			::wsprintf(szDefault, _T("Value%d"), i);

			::GetPrivateProfileString(VARIABLES, szKeyName, szDefault, szValue, sizeof(szValue), szFile);
			::GetPrivateProfileString(VARIABLES, szKeyName2, _T("0"), szInit, sizeof(szInit), szFile);

			CXTPPropertyGridItem* pVariables = m_pPrptyVariables->AddChildItem(new CXTPPropertyGridItem(szValue, szInit));
			pVariables->SetReadOnly(TRUE);
		}
		
		nValue = ::GetPrivateProfileInt(SIGNAL, USEBUY, 0, szFile);
		if(nValue)
		{
			nValue = ::GetPrivateProfileInt(SIGNAL, COLORBUY, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptyBuy)->SetColor((COLORREF)nValue);
			nValue = ::GetPrivateProfileInt(SIGNAL, SHAPEBUY, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptyBuy)->SetValue(GetShapeString(nValue));
		}

		nValue = ::GetPrivateProfileInt(SIGNAL, USESELL, 0, szFile);
		if(nValue)
		{
			nValue = ::GetPrivateProfileInt(SIGNAL, COLORSELL, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptySell)->SetColor((COLORREF)nValue);
			nValue = ::GetPrivateProfileInt(SIGNAL, SHAPESELL, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptySell)->SetValue(GetShapeString(nValue));
		}
	
		nValue = ::GetPrivateProfileInt(SIGNAL, USEEXITLONG, 0, szFile);
		if(nValue)
		{
			nValue = ::GetPrivateProfileInt(SIGNAL, COLOREXITLONG, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptyExitlong)->SetColor((COLORREF)nValue);
			nValue = ::GetPrivateProfileInt(SIGNAL, SHAPEEXITLONG, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptyExitlong)->SetValue(GetShapeString(nValue));
		}
	
		nValue = ::GetPrivateProfileInt(SIGNAL, USEEXITSHORT, 0, szFile);
		if(nValue)
		{
			nValue = ::GetPrivateProfileInt(SIGNAL, COLOREXITSHORT, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptyExitshort)->SetColor((COLORREF)nValue);
			nValue = ::GetPrivateProfileInt(SIGNAL, SHAPEEXITSHORT, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptyExitshort)->SetValue(GetShapeString(nValue));
		}

		nValue = ::GetPrivateProfileInt(SIGNAL, SHOWSIGNALNAME, 0, szFile);
		((CXTPPropertyGridItemBool*)m_pPrptyShowSignalName)->SetBool(nValue);

		nValue = ::GetPrivateProfileInt(SIGNAL, SHOWSIGNALQTY, 0, szFile);
		((CXTPPropertyGridItemBool*)m_pPrptyShowSignalQty)->SetBool(nValue);

		nValue = ::GetPrivateProfileInt(SIGNAL, POSOFSIGNAL, 0, szFile);

		if(nValue == 0)
			m_pPrptyPosOfSignal->SetValue(_T("봉완성시"));
		else if(nValue == 1)
			m_pPrptyPosOfSignal->SetValue(_T("신호발생시"));

		for(i = 0; i < 10; i++)
		{
			::wsprintf(szKeyName, _T("%s%d"), INDEXNAME, i);

			::GetPrivateProfileString(INDEX, szKeyName, _T(""), szValue, sizeof(szValue), szFile);
			if(!::lstrlen(szValue))
				break;

			CXTPPropertyGridItem* pIndex = m_pPrptyIndex->AddChildItem(new CXTPPropertyGridItem(szValue, _T("")));
			pIndex->SetReadOnly(TRUE);

			::wsprintf(szKeyName2, _T("%s%d"), INDEXTYPE, i);
			nValue = ::GetPrivateProfileInt(INDEX, szKeyName2, 0, szFile);

			CXTPPropertyGridItem* pIndexLine = pIndex->AddChildItem(new CXTPPropertyGridItem(_T("종류"), GetLineType(nValue)));
			pIndexLine->SetReadOnly(TRUE);

			::wsprintf(szKeyName2, _T("%s%d"), INDEXCOLOR, i);
			nValue = ::GetPrivateProfileInt(INDEX, szKeyName2, 0, szFile);

			pIndexLine = pIndex->AddChildItem(new CXTPPropertyGridItemColor(_T("색상"), (COLORREF)nValue));
			pIndexLine->SetReadOnly(TRUE);

			::wsprintf(szKeyName2, _T("%s%d"), INDEXSHAPE, i);
			nValue = ::GetPrivateProfileInt(INDEX, szKeyName2, 0, szFile);

			pIndexLine = pIndex->AddChildItem(new CSTPropertyGridItemLineFormat(_T("형태"), nValue));
			pIndexLine->SetReadOnly(TRUE);

			::wsprintf(szKeyName2, _T("%s%d"), INDEXTHICK, i);
			nValue = ::GetPrivateProfileInt(INDEX, szKeyName2, 0, szFile);

			pIndexLine = pIndex->AddChildItem(new CSTPropertyGridItemLineWidth(_T("굵기"), nValue + 1));
			pIndexLine->SetReadOnly(TRUE);
		}

		nValue = ::GetPrivateProfileInt(INDEX, USEBULL, 0, szFile);
		if(nValue)
		{
			nValue = ::GetPrivateProfileInt(INDEX, COLORBULL, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptyBull)->SetColor((COLORREF)nValue);
		}

		nValue = ::GetPrivateProfileInt(INDEX, USEBEAR, 0, szFile);
		if(nValue)
		{
			nValue = ::GetPrivateProfileInt(INDEX, COLORBEAR, 0, szFile);
			((CSTPropertyGridItemColor*)m_pPrptyBear)->SetColor((COLORREF)nValue);
		}

		CScriptSecurity* pInfo = pDoc->GetScriptSecurity();
		if(pInfo->GetUsePassword())
			m_pPrptyPassword->SetValue(_T("비밀번호 지정"));
		else
			m_pPrptyPassword->SetValue(_T("비밀번호 없음"));

		if(pInfo->GetUseUserID())
			m_pPrptyUserID->SetValue(pInfo->GetUserID());
		else
			m_pPrptyUserID->SetValue(_T(""));

		if(pInfo->GetUseExpires())
		{
			CString strDate(_T(""));
			CString strExpires = pInfo->GetExpires();

			strDate.Format(_T("%4s년 %2s월 %2s일"), strExpires.Left(4), strExpires.Mid(4, 2), strExpires.Right(2));
			m_pPrptyExpires->SetValue(strDate);
		}
		else
			m_pPrptyExpires->SetValue(_T(""));
	}
	else
	{
		m_pPrptyScriptName->SetValue(_T(""));
		m_pPrptyScriptComment->SetValue(_T(""));
		m_pPrptyScriptLanguage->SetValue(_T(""));
		m_pPrptyScriptCheck->SetValue(_T(""));

		CXTPPropertyGridItems* pVariables = m_pPrptyVariables->GetChilds();
		pVariables->Clear();

		((CSTPropertyGridItemColor*)m_pPrptyBuy)->SetColor(NULL);
		((CSTPropertyGridItemColor*)m_pPrptyBuy)->SetValue(_T("없음"));
		((CSTPropertyGridItemColor*)m_pPrptyExitlong)->SetColor(NULL);
		((CSTPropertyGridItemColor*)m_pPrptyExitlong)->SetValue(_T("없음"));
		((CSTPropertyGridItemColor*)m_pPrptySell)->SetColor(NULL);
		((CSTPropertyGridItemColor*)m_pPrptySell)->SetValue(_T("없음"));
		((CSTPropertyGridItemColor*)m_pPrptyExitshort)->SetColor(NULL);
		((CSTPropertyGridItemColor*)m_pPrptyExitshort)->SetValue(_T("없음"));

		((CXTPPropertyGridItemBool*)m_pPrptyShowSignalName)->SetBool(FALSE);
		((CXTPPropertyGridItemBool*)m_pPrptyShowSignalQty)->SetBool(FALSE);

		m_pPrptyPosOfSignal->SetValue(_T("봉완성시"));

		CXTPPropertyGridItems* pIndex = m_pPrptyIndex->GetChilds();
		pIndex->Clear();

		((CSTPropertyGridItemColor*)m_pPrptyBull)->SetColor(0);
		((CSTPropertyGridItemColor*)m_pPrptyBear)->SetColor(0);

		m_pPrptyPassword->SetValue(_T(""));
		m_pPrptyUserID->SetValue(_T(""));
		m_pPrptyExpires->SetValue(_T(""));
	}
}

CString CMainFrame::GetShapeString(int nIndex)
{
	switch(nIndex)
	{
	case 0:
		return _T("화살표");
	case 1:
		return _T("삼각형");
	case 2:
		return _T("원");
	case 3:
		return _T("사각형");
	}

	return _T("없음");
}

CString CMainFrame::GetLineType(int nIndex)
{
	switch(nIndex)
	{
	case 0:
		return _T("선그래프");
	case 1:
		return _T("점그래프");
	case 2:
		return _T("바그래프");
	}

	return _T("없음");
}

BOOL CMainFrame::ScanScriptFile(LPSTR lpszName, UINT nFileType, UINT nScriptType)
{
	char szPath[MAX_PATH] = {NULL, };
	::GetSTFile(szPath, lpszName, theApp.GetRootPath());

	//KHD : 삼성 요청으로 주석처리
	//::WritePrivateProfileString(_T("Signal"), _T("UseBuy"), NULL, szPath);
	//::WritePrivateProfileString(_T("Signal"), _T("UseExitlong"), NULL, szPath);
	//::WritePrivateProfileString(_T("Signal"), _T("UseSell"), NULL, szPath);
	//::WritePrivateProfileString(_T("Signal"), _T("UseExitshort"), NULL, szPath);
	
	char szNameKey[10] = {NULL, };
	for(int i = 0; i < 10; i++)
	{
		::wsprintf(szNameKey, _T("Name%d"), i);
		::WritePrivateProfileString(_T("Index"), szNameKey, NULL, szPath);
	}

	char szSaveFile[MAX_PATH] = {NULL, };
	if(nFileType == FT_STRATEGY)

//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszName);
		::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszName);

	else if(nFileType == FT_USERFUNCTION)
//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszName);
		::wsprintf(szSaveFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszName);

	if(!m_pLoadPaser)
		LoadSystemTradingParser(nScriptType);

	m_pLoadPaser->CheckScriptStatement(szSaveFile, szPath);
	DeleteSystemTradingParser();

	BOOL bExistScreen = FALSE;
	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		if(::lstrcmp(pDoc->GetScriptName(), lpszName) == 0)
		{
			bExistScreen = TRUE;
			break;
		}
	}

	if(bExistScreen)
		ShowFunctionInStrategy(lpszName, nFileType, nScriptType);

	return bExistScreen;
}

void CMainFrame::ShowFunctionInStrategy(LPCSTR lpszName, UINT nFileType, UINT nScriptType)
{
	VT_DECLDATA vtUsedUserFunction;
	vtUsedUserFunction.clear();
	
	char szSaveFile[MAX_PATH] = {NULL, };
	if(nFileType == FT_STRATEGY)
//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszName);
		::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszName);

	else if(nFileType == FT_USERFUNCTION)
//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszName);
		::wsprintf(szSaveFile, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszName);

	if(!m_pLoadPaser)
		LoadSystemTradingParser(nScriptType);

	m_pLoadPaser->GetUsedUserFunction(&vtUsedUserFunction, szSaveFile);
	DeleteSystemTradingParser();

	if(m_pLoadWorkspace)
	{
		if(nFileType == FT_STRATEGY)
			m_pLoadWorkspace->AddListFunctionInStrategy(lpszName, &vtUsedUserFunction);
		else if(nFileType == FT_USERFUNCTION)
			m_pLoadWorkspace->AddListFunctionInFunction(lpszName, &vtUsedUserFunction);
	}

	for_each(vtUsedUserFunction.begin(), vtUsedUserFunction.end(), DeleteVariable);
}

void CMainFrame::DeleteFunctionInStrategy(LPCSTR lpszName)
{
	if(m_pLoadWorkspace)
		m_pLoadWorkspace->DeleteFunctionInStrategy(lpszName);
}

void CMainFrame::ScanFunctionInStrategy()
{
	POSITION pos = theApp.m_pDocTemplate->GetFirstDocPosition();
	CDocument* pDoc = NULL;
	while(pos)
	{
		pDoc = theApp.m_pDocTemplate->GetNextDoc(pos);
		if(pDoc->IsKindOf(RUNTIME_CLASS(CSystemTradingDoc)))
		{
			CSystemTradingDoc* pSysDoc = (CSystemTradingDoc*)pDoc;
//			if(pSysDoc->GetParseFileType() == FT_STRATEGY)
				ShowFunctionInStrategy(pSysDoc->GetScriptName(), pSysDoc->GetParseFileType(), pSysDoc->GetScriptType());
		}
	}
}

void CMainFrame::LoadSystemTradingParser(UINT nScriptType)
{
	if(nScriptType == ST_TRADESTATION)
	{
		m_pLoadPaser = new CLoadSystemTradingParserHelper;
		m_pLoadPaser->InitSystemTradingParser(theApp.GetRootPath(), GetUserFuncInfo(), GetInnerFuncInfo());
	}
	else if(nScriptType == ST_YESTRADER)
	{
		m_pLoadPaser = new CLoadSystemTradingParserHelper(_T("SystemTradingParser_YES.dll"));
		m_pLoadPaser->InitSystemTradingParser(theApp.GetRootPath(), GetUserFuncInfo(), GetInnerFuncInfo_YES());
	}
}

void CMainFrame::DeleteSystemTradingParser()
{
	if(m_pLoadPaser)
	{
		delete m_pLoadPaser;
		m_pLoadPaser = NULL;
	}
}

LRESULT CMainFrame::OnDblClickOutput(WPARAM wParam, LPARAM lParam)
{
	CErrorInfomation* pErroInfo = (CErrorInfomation*)lParam;
	if(!lParam)
		return 0L;

	CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.OpenDocumentFile(pErroInfo->GetFileName());
	pDoc->SelectLine(pErroInfo->GetLineNumber());

	return 0L;
}

LRESULT CMainFrame::OnDblClickFind(WPARAM wParam, LPARAM lParam)
{
	CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.OpenDocumentFile((LPSTR)lParam);
	pDoc->SelectLine(LOWORD(wParam) - 1);
//	pDoc->SelectLine(LOWORD(wParam), HIWORD(wParam));

	return 0L;
}

void CMainFrame::OnCenter() 
{
/*
		CFontDialog FontDlg(NULL,CF_SCREENFONTS|CF_ANSIONLY|CF_FIXEDPITCHONLY|CF_NOSCRIPTSEL|CF_TTONLY|CF_NOVERTFONTS,NULL,this);
		
	 FontDlg.DoModal();
	CLoadSystemTradingZipHelper Helper;

	char szZip[MAX_PATH] = {NULL, };
	char szFile[MAX_PATH] = {NULL, };

	::wsprintf(szZip, _T("%s%sZzz.Zip"), theApp.GetRootPath(), PATH_SCRIPTFILES);
//	::wsprintf(szFile, _T("%s%sDMIPLUS.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES);
	::wsprintf(szFile, _T("%s%s"), theApp.GetRootPath(), PATH_SCRIPTFILES);

//	Helper.CompressFile(szZip, szFile);
	Helper.ExtractFile(szZip, szFile);
*/

/*
	char szHandleFile[MAX_PATH] = {NULL, };
	::wsprintf(szHandleFile, _T("%s%sHandleFile.dat"), theApp.GetRootPath(), PATH_STDATA);

	HWND hAnyTrader = (HWND)::GetPrivateProfileInt(_T("HANDLE"), _T("AnyTrader"), 0, szHandleFile);

	if(hAnyTrader)
	{
		::SendMessage(hAnyTrader, RMSG_SYSTEMTRADING, 0, 0);
		ShowWindow(SW_MINIMIZE);
	}
*/
}

LRESULT CMainFrame::OnAnyTrader(WPARAM wParam, LPARAM lParam)
{
	SetForegroundWindow();
	return 0L;
}

void CMainFrame::ConnectToCompileServer()
{
	m_pMainClient = new CClientSocket;
	m_pMainClient->Create();
	
	char szConfig[MAX_PATH] = {NULL, };
	::wsprintf(szConfig, _T("%s%sSTConfig.cfg"), theApp.GetRootPath(), PATH_STDATA);

	char szIP[16] = {NULL, };
	::GetPrivateProfileString(_T("CONNECT"), _T("IP"), _T("211.240.78.171"), szIP, sizeof(szIP), szConfig);

//#ifdef _DEBUG
//	int nPort = ::GetPrivateProfileInt(_T("CONNECT"), _T("TEST"), 40100, szConfig);
//#else
	int nPort = ::GetPrivateProfileInt(_T("CONNECT"), _T("PORT"), 40100, szConfig);
//#endif

	while(!m_pMainClient->Connect(szIP, nPort))
	{
		if(::WSAGetLastError() == WSAEWOULDBLOCK)  
			continue;
		else
		{
			delete m_pMainClient;
			m_pMainClient = NULL;

			CString szMsg;
			//szMsg.Format(_T("서버(%s:%d)와의 연결에 실패하였습니다."), szIP, nPort);
			szMsg.Format(_T("서버(%s)와의 연결에 실패하였습니다."), szIP);
			AfxMessageBox(szMsg);

			SetConnectionStatusTest(_T("오프라인"), RGB(255, 0, 0));

			SetServerMessage(szMsg, SVRMSG_ALERT);
			break;
		}
	}
}

void CMainFrame::UploadCPPFile(UINT nStrategyType)
{
	char szSaveFile[MAX_PATH];
	::wsprintf(szSaveFile, _T("%s%sOutput\\STSample.cpp"), theApp.GetRootPath(), PATH_STDATA);

	CFile fileCpp(szSaveFile, CFile::modeRead);

	int nFileSize = fileCpp.GetLength();
	LPSTR szFileData = new char[nFileSize];

	DWORD dwRead = 0;
	m_CriticalSection.Lock();
	do
	{
		dwRead += fileCpp.Read(szFileData, nFileSize);
	} while (dwRead < nFileSize);
	m_CriticalSection.Unlock();
	
	fileCpp.Close();
	m_pMainClient->SendToServer(SOCK_CMD_UPLOAD_FILE, (LPVOID)szFileData, dwRead, nStrategyType);

	delete [] szFileData;
}

void CMainFrame::UploadCPPFile_Debug(UINT nStrategyType)
{
	char szSaveFile[MAX_PATH];
	::wsprintf(szSaveFile, _T("%s%sOutput\\STSample.cpp"), theApp.GetRootPath(), PATH_STDATA);

	CFile fileCpp(szSaveFile, CFile::modeRead);

	int nFileSize = fileCpp.GetLength();
	LPSTR szFileData = new char[nFileSize];

	DWORD dwRead = 0;
	m_CriticalSection.Lock();
	do
	{
		dwRead += fileCpp.Read(szFileData, nFileSize);
	} while (dwRead < nFileSize);
	m_CriticalSection.Unlock();
	
	fileCpp.Close();
	m_pMainClient->SendToServer(SOCK_CMD_UPLOAD_FILE_DEBUG, (LPVOID)szFileData, dwRead, nStrategyType);

	delete [] szFileData;
}

void CMainFrame::UploadUserFunctionCPPFile(LPSTR lpszScriptName)
{
	char szSaveFile[MAX_PATH];
	::wsprintf(szSaveFile, _T("%s%sOutput\\%s.STSample.cpp"), theApp.GetRootPath(), PATH_STDATA, lpszScriptName);

	CFile fileCpp(szSaveFile, CFile::modeRead);

	int nFileSize = fileCpp.GetLength();
	LPSTR szFileData = new char[nFileSize];

	DWORD dwRead = 0;
	m_CriticalSection.Lock();
	do
	{
		dwRead += fileCpp.Read(szFileData, nFileSize);
	} while (dwRead < nFileSize);
	m_CriticalSection.Unlock();
	
	fileCpp.Close();
	m_pMainClient->SendToServer(SOCK_CMD_UPLOAD_FILE_UF, (LPVOID)szFileData, dwRead);

	delete [] szFileData;
}

void CMainFrame::UploadUserFunctionCPPFile_Debug(LPSTR lpszScriptName)
{
	char szSaveFile[MAX_PATH];
	::wsprintf(szSaveFile, _T("%s%sOutput\\%s.STSample.cpp"), theApp.GetRootPath(), PATH_STDATA, lpszScriptName);

	CFile fileCpp(szSaveFile, CFile::modeRead);

	int nFileSize = fileCpp.GetLength();
	LPSTR szFileData = new char[nFileSize];

	DWORD dwRead = 0;
	m_CriticalSection.Lock();
	do
	{
		dwRead += fileCpp.Read(szFileData, nFileSize);
	} while (dwRead < nFileSize);
	m_CriticalSection.Unlock();
	
	fileCpp.Close();
	m_pMainClient->SendToServer(SOCK_CMD_UPLOAD_FILE_UF_DEBUG, (LPVOID)szFileData, dwRead);

	delete [] szFileData;
}

void CMainFrame::UploadZipFile(LPSTR lpszZipName)
{
	char szSaveFile[MAX_PATH];

//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.zip"), theApp.GetRootPath(), PATH_SCRIPTFILES, lpszZipName);
	::wsprintf(szSaveFile, _T("%s%s%s.zip"), theApp.GetUserPath(), PATH_SCRIPTFILES2, lpszZipName);

	CFile fileCpp(szSaveFile, CFile::modeRead);

	int nFileSize = fileCpp.GetLength();
	LPSTR szFileData = new char[nFileSize];

	DWORD dwRead = 0;
	m_CriticalSection.Lock();
	do
	{
		dwRead += fileCpp.Read(szFileData, nFileSize);
	} while (dwRead < nFileSize);
	m_CriticalSection.Unlock();
	
	fileCpp.Close();
	m_pMainClient->SendToServer(SOCK_CMD_UPLOAD_FILE_ALL, (LPVOID)szFileData, dwRead);

	delete [] szFileData;
}

void CMainFrame::CreateNewDll(LPSTR lpszFilePath, UINT nStrategyType)
{
	char szFilePath[MAX_PATH];
//@유진삭제::wsprintf(szFilePath, _T("%s%s%s.sd"), theApp.GetRootPath(), PATH_DLLFILES, lpszFilePath);
	::wsprintf(szFilePath, _T("%s%s%s.sd"), theApp.GetUserPath(), PATH_DLLFILES2, lpszFilePath);

	if(m_pFileDll)
	{
		m_pFileDll->Close();
		m_pFileDll->Open(szFilePath, CFile::modeCreate|CFile::modeWrite);
	}
	else
		m_pFileDll = new CFile(szFilePath, CFile::modeCreate|CFile::modeWrite);

	m_nStrategyType = nStrategyType;
}

void CMainFrame::CreateNewDll_Debug(LPSTR lpszFilePath, UINT nStrategyType)
{
	char szFilePath[MAX_PATH];
//@유진삭제::wsprintf(szFilePath, _T("%s%s%s.sd"), theApp.GetRootPath(), PATH_DLLFILES_D, lpszFilePath);
	::wsprintf(szFilePath, _T("%s%s%s.sd"), theApp.GetUserPath(), PATH_DLLFILES_D2, lpszFilePath);

	if(m_pFileDll)
	{
		m_pFileDll->Close();
		m_pFileDll->Open(szFilePath, CFile::modeCreate|CFile::modeWrite);
	}
	else
		m_pFileDll = new CFile(szFilePath, CFile::modeCreate|CFile::modeWrite);

	m_nStrategyType = nStrategyType;
}

void CMainFrame::CreateNewDllFunc(LPSTR lpszFilePath)
{
	char szFilePath[MAX_PATH];
//@유진삭제::wsprintf(szFilePath, _T("%s%s%s.sdf"), theApp.GetRootPath(), PATH_DLLFILES, lpszFilePath);
	::wsprintf(szFilePath, _T("%s%s%s.sdf"), theApp.GetUserPath(), PATH_DLLFILES2, lpszFilePath);

	if(m_pFileDll)
	{
		m_pFileDll->Close();
		m_pFileDll->Open(szFilePath, CFile::modeCreate|CFile::modeWrite);
	}
	else
		m_pFileDll = new CFile(szFilePath, CFile::modeCreate|CFile::modeWrite);
}

void CMainFrame::CreateNewDllFunc_Debug(LPSTR lpszFilePath)
{
	char szFilePath[MAX_PATH];
//@유진삭제::wsprintf(szFilePath, _T("%s%s%s.sdf"), theApp.GetRootPath(), PATH_DLLFILES_D, lpszFilePath);
	::wsprintf(szFilePath, _T("%s%s%s.sdf"), theApp.GetUserPath(), PATH_DLLFILES_D2, lpszFilePath);

	if(m_pFileDll)
	{
		m_pFileDll->Close();
		m_pFileDll->Open(szFilePath, CFile::modeCreate|CFile::modeWrite);
	}
	else
		m_pFileDll = new CFile(szFilePath, CFile::modeCreate|CFile::modeWrite);
}

void CMainFrame::CreateNewZip(LPSTR lpszFilePath)
{
	char szFilePath[MAX_PATH];
//@유진삭제::wsprintf(szFilePath, _T("%s%s%s.zip"), theApp.GetRootPath(), PATH_DLLFILES, lpszFilePath);
	::wsprintf(szFilePath, _T("%s%s%s.zip"), theApp.GetUserPath(), PATH_DLLFILES2, lpszFilePath);

	if(m_pFileDll)
	{
		m_pFileDll->Close();
		m_pFileDll->Open(szFilePath, CFile::modeCreate|CFile::modeWrite);
	}
	else
		m_pFileDll = new CFile(szFilePath, CFile::modeCreate|CFile::modeWrite);
}

void CMainFrame::WriteDllFile(LPSTR lpszData, UINT nFileSize)
{
	if(!m_pFileDll)
		return;

	m_CriticalSection.Lock();
	
	m_pFileDll->Write(lpszData, nFileSize);

	m_CriticalSection.Unlock();

	CString str = m_pFileDll->GetFileName();
	str.Delete(str.Find(".", 0), 3);
	SetOutputText(CM_MAKE_COMPLETE_STRATEGY, str);
	CString szFileName = m_pFileDll->GetFilePath();//KHD 삼성요청
	m_pFileDll->Close();
	delete m_pFileDll;
	m_pFileDll = NULL;

	ChangeCompiledOption(str, FT_STRATEGY, IMG_STS_DLL);

	MakeSTFile(str, (LPCSTR)szFileName);
	m_nStrategyType = 0;

	if(m_bMultiCompile)
	{
		if(m_nScriptNameCount < m_starScriptName.GetSize())
		{
			OnSingleCompileDLLMainTree(1, (LPARAM)(LPSTR)(LPCTSTR)m_starScriptName.GetAt(m_nScriptNameCount++));
			m_pLoadWorkspace->GetOutputWnd()->UpdateWindow();
		}
		else	// 멀티 컴파일 종료
		{
			m_bMultiCompile = FALSE;
			m_nScriptNameCount = 0;
			m_starScriptName.RemoveAll();

			WriteEndDllFile();
		}
	}
	else
		WriteEndDllFile();
}

void CMainFrame::WriteEndDllFile()
{
	m_bCompiling = FALSE;

	m_ctrlCompileProgress.ShowWindow(SW_HIDE);

	if(!m_pMainClient)
		SetConnectionStatusTest(_T("오프라인"), RGB(255, 0, 0));
	else
		SetConnectionStatusTest(_T("온라인"));
}

void CMainFrame::WriteDllFileFunc(LPSTR lpszData, UINT nFileSize)
{
	if(!m_pFileDll)
		return;

	m_CriticalSection.Lock();
	
	m_pFileDll->Write(lpszData, nFileSize);

	m_CriticalSection.Unlock();

	CString str = m_pFileDll->GetFileName();
	str.Delete(str.Find(".", 0), 4);
	SetOutputText(CM_MAKE_COMPLETE_FUNCTION, str);

	m_pFileDll->Close();
	delete m_pFileDll;
	m_pFileDll = NULL;

	m_nStrategyType = 0;

	ChangeCompiledOption(str, FT_USERFUNCTION, IMG_FUNC_DLL);

	if(m_bMultiCompile)
	{
		if(m_nScriptNameCount < m_starScriptName.GetSize())
		{
			OnSingleCompileDLLFunctionTree(1, (LPARAM)(LPSTR)(LPCTSTR)m_starScriptName.GetAt(m_nScriptNameCount++));
			m_pLoadWorkspace->GetOutputWnd()->UpdateWindow();
		}
		else	// 멀티 컴파일 종료
		{
			m_bMultiCompile = FALSE;
			m_nScriptNameCount = 0;
			m_starScriptName.RemoveAll();

			WriteEndDllFileFunc();
		}
	}
	else
		WriteEndDllFileFunc();

	// Function일때는 ST파일을 삭제한다.: KHD 삼성요청
	{
		char szFileName[MAX_PATH] = {NULL, };
		::GetSTFile(szFileName, str, theApp.GetRootPath());

		::DeleteFile(szFileName);
	}
}

void CMainFrame::WriteEndDllFileFunc()
{
	m_bCompiling = FALSE;

	m_ctrlCompileProgress.ShowWindow(SW_HIDE);

	if(!m_pMainClient)
		SetConnectionStatusTest(_T("오프라인"), RGB(255, 0, 0));
	else
		SetConnectionStatusTest(_T("온라인"));
}

void CMainFrame::WriteZipFile(LPSTR lpszData, UINT nFileSize)
{
	if(!m_pFileDll)
		return;

	m_CriticalSection.Lock();
	
	m_pFileDll->Write(lpszData, nFileSize);

	m_CriticalSection.Unlock();

	CString str = m_pFileDll->GetFileName();
	str.Delete(str.Find(".", 0), 4);
	SetOutputText(CM_MAKE_COMPLETE_ALL, str);

	str = m_pFileDll->GetFilePath();

	m_pFileDll->Close();
	delete m_pFileDll;
	m_pFileDll = NULL;

	m_nStrategyType = 0;

	ExtractFile(str);
	::DeleteFile(str);
}

void CMainFrame::NotifyExtract(BOOL bStrategy, LPCSTR lpszName)
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;

	if(bStrategy)
	{
		pMainFrame->SetOutputText(CM_MAKE_COMPLETE_STRATEGY, lpszName);
		//AfxMessageBox("4");
		pMainFrame->ChangeCompiledOption(lpszName, FT_STRATEGY, IMG_STS_DLL);
	}
	else
	{
		pMainFrame->SetOutputText(CM_MAKE_COMPLETE_FUNCTION, lpszName);
		//AfxMessageBox("5");
		pMainFrame->ChangeCompiledOption(lpszName, FT_USERFUNCTION, IMG_FUNC_DLL);
	}
}

BOOL CMainFrame::ExtractFile(LPCSTR lpszFile)
{
	char szFolder[MAX_PATH] = {NULL, };
//@유진삭제::wsprintf(szFolder, _T("%s%s"), theApp.GetRootPath(), PATH_DLLFILES);
	::wsprintf(szFolder, _T("%s%s"), theApp.GetUserPath(), PATH_DLLFILES2);

	CLoadSystemTradingZipHelper Helper;
	Helper.ExtractFileNotify(lpszFile, szFolder, NotifyExtract);

	return TRUE;
}

BOOL CMainFrame::CreateSubStatusWindow()
{
	DWORD dwStatic  = WS_CHILD|WS_VISIBLE|SS_CENTER;

	if (!m_wndStatusConnect.Create(_T("오프라인"), dwStatic, CRect(0,0,0,0), &m_wndStatusBar, 2))
	{
		TRACE0("Failed to create edit control.\n");
		return FALSE;
	}

	m_wndStatusConnect.SetTextColor(RGB(255,0,0));	
	m_wndStatusConnect.SetFontName("굴림체", DEFAULT_CHARSET);
	m_wndStatusConnect.SetAlign(CLabel::_CENTER);
	m_wndStatusConnect.SetFontSize(9);

	int nIndex = m_wndStatusBar.GetPaneCount();
	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_CONNECT, nIndex);
	
	// Initialize the pane info and add the control.
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_CONNECT);
	ASSERT (nIndex != -1);
	
	m_wndStatusBar.SetPaneWidth(nIndex, 50);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex));
	m_wndStatusBar.AddControl(&m_wndStatusConnect, ID_INDICATOR_CONNECT, FALSE);

	return TRUE;
}

BOOL CMainFrame::CreateCompileStatusWindow()
{
	DWORD dwStatic  = WS_CHILD|WS_VISIBLE|SS_CENTER;

	if (!m_ctrlCompileProgress.Create(dwStatic, CRect(0,0,0,0), &m_wndStatusBar, 3))
	{
		TRACE0("Failed to create edit control.\n");
		return FALSE;
	}

	m_ctrlCompileProgress.SetRange(0, 100);

	int nIndex = 1;//m_wndStatusBar.GetPaneCount();
	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_COMPILESTATUS, nIndex);
	
	// Initialize the pane info and add the control.
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_COMPILESTATUS);
	ASSERT (nIndex != -1);
	
	m_wndStatusBar.SetPaneWidth(nIndex, 150);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex));
	m_wndStatusBar.AddControl(&m_ctrlCompileProgress, ID_INDICATOR_COMPILESTATUS, FALSE);

	m_ctrlCompileProgress.ShowWindow(SW_HIDE);

	return TRUE;
}

void CMainFrame::SetConnectionStatusTest(LPCSTR lpszText, COLORREF clrText/* = RGB(0, 0, 255)*/)
{
	m_wndStatusConnect.SetTextColor(clrText);
	m_wndStatusConnect.SetText(lpszText);
}

void CMainFrame::AddIndexFunctionList(LPCSTR lpszStrategyName, BOOL bUsed)
{
	BOOL bFind = FALSE;
	vector<LPCSTR>::iterator it = m_vtIndexFunction.begin();
	while(it != m_vtIndexFunction.end())
	{
		if(::lstrcmp((*it), lpszStrategyName) == 0)
		{
			bFind = TRUE;
			break;
		}

		it++;
	}

	if(bFind && !bUsed)
	{
		::free((LPVOID)(*it));
		m_vtIndexFunction.erase(it);
	}

	if(!bFind && bUsed)
		m_vtIndexFunction.push_back(::strdup(lpszStrategyName));

	char szIndexFunction[MAX_PATH] = {NULL, };
	::wsprintf(szIndexFunction, _T("%s%sIndexFuncStrategy.dat"), theApp.GetRootPath(), PATH_STDATA);

	CFile file(szIndexFunction, CFile::modeCreate|CFile::modeWrite);

	it = m_vtIndexFunction.begin();
	while(it != m_vtIndexFunction.end())
	{
		file.Write((*it), ::lstrlen((*it)));
		file.Write(_T(";"), 1);
		it++;
	}

	file.Close();
}

void CMainFrame::InitIndexFunctionStrategy()
{
	char szIndexFunction[MAX_PATH] = {NULL, };
	::wsprintf(szIndexFunction, _T("%s%sIndexFuncStrategy.dat"), theApp.GetRootPath(), PATH_STDATA);

	CFile file;
	if(!file.Open(szIndexFunction, CFile::modeRead))
		return;

	DWORD dwSize = file.GetLength();
	LPSTR lpszData = new char[dwSize + 1];
	::memset(lpszData, 0x00, dwSize + 1);

	file.Read(lpszData, dwSize);
	file.Close();

	CString strStrategy;
	int i = 0;
	while(AfxExtractSubString(strStrategy, lpszData, i++, ';'))
	{
		if(strStrategy.GetLength())
			m_vtIndexFunction.push_back(::strdup(strStrategy));
	}

	delete [] lpszData;
}

void CMainFrame::OnFindandreplaceFindinfiles() 
{
	CDlgFindScript dlg;
	dlg.m_bMatchCase = theApp.GetProfileInt(REG_FINDFILE_SUBKEY, REG_MATCH_CASE, FALSE);
	dlg.m_bWholeWord = theApp.GetProfileInt(REG_FINDFILE_SUBKEY, REG_WHOLE_WORD, FALSE);
	dlg.m_bFindFunction = theApp.GetProfileInt(REG_FINDFILE_SUBKEY, REG_FINDFILE_UFT, TRUE);
	dlg.m_bFindStrategy = theApp.GetProfileInt(REG_FINDFILE_SUBKEY, REG_FINDFILE_STS, TRUE);
	dlg.m_strText = theApp.GetProfileString(REG_FINDFILE_SUBKEY, REG_FIND_WHAT, _T(""));

	if(dlg.DoModal() == IDOK)
	{
		if(m_pLoadWorkspace)
			m_pLoadWorkspace->ClearAllFindText();

		FindScript(dlg.m_strText, dlg.m_bMatchCase, dlg.m_bWholeWord, dlg.m_bFindStrategy, dlg.m_bFindFunction);

		theApp.WriteProfileInt(REG_FINDFILE_SUBKEY, REG_MATCH_CASE, dlg.m_bMatchCase);
		theApp.WriteProfileInt(REG_FINDFILE_SUBKEY, REG_WHOLE_WORD, dlg.m_bWholeWord);
		theApp.WriteProfileInt(REG_FINDFILE_SUBKEY, REG_FINDFILE_UFT, dlg.m_bFindFunction);
		theApp.WriteProfileInt(REG_FINDFILE_SUBKEY, REG_FINDFILE_STS, dlg.m_bFindStrategy);
		theApp.WriteProfileString(REG_FINDFILE_SUBKEY, REG_FIND_WHAT, dlg.m_strText);
	}
}

void CMainFrame::FindScript(CString strText, BOOL bMatchCase, BOOL bWholeWord, BOOL bStrategy, BOOL bFunction)
{
	char szFindFile[MAX_PATH] = {NULL, };
	if(bStrategy && bFunction)
//@유진삭제::wsprintf(szFindFile, _T("%s%s*.*"), theApp.GetRootPath(), PATH_SCRIPTFILES);
		::wsprintf(szFindFile, _T("%s%s*.*"), theApp.GetUserPath(), PATH_SCRIPTFILES2);

	else if(bStrategy)
//@유진삭제::wsprintf(szFindFile, _T("%s%s*.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES);
		::wsprintf(szFindFile, _T("%s%s*.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2);

	else if(bFunction)
//@유진삭제::wsprintf(szFindFile, _T("%s%s*.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES);
		::wsprintf(szFindFile, _T("%s%s*.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2);
	else
		return;

	CFileFind ff;

	BOOL bFind = ff.FindFile(szFindFile);
	int nFinded = 0;
	char szBackup[MAX_PATH] = {NULL, };
	CString strFilePath(_T(""));

	while(bFind)
	{
		bFind = ff.FindNextFile();
		strFilePath = ff.GetFilePath();
		
		::wsprintf(szBackup, _T("%s.bak"), strFilePath);
		::CopyFile(strFilePath, szBackup, FALSE);
		
		DecodingFile(szBackup, strFilePath);
		if(FindTextInFile(strText, bMatchCase, bWholeWord, strFilePath))
			nFinded++;

		EncodingFile(strFilePath, szBackup);
		::CopyFile(szBackup, strFilePath, FALSE);
		::DeleteFile(szBackup);
	}

	if(!nFinded)
	{
		CString strMessage(_T(""));
		strMessage.Format(_T("%s 문자열이 포함된 스크립트를 찾지 못했습니다."), strText);
		m_pLoadWorkspace->SetFindText(strMessage, 0);
	}
}

BOOL CMainFrame::FindTextInFile(CString strText, BOOL bMatchCase, BOOL bWholeWord, CString strFile)
{
	CStdioFile file;
	if(!file.Open(strFile, CFile::shareDenyRead))
		return FALSE;

	if(!bMatchCase)
		strText.MakeUpper();

	CString strLine;
	int nLine = 0;
	BOOL bReturn = FALSE;

	while(file.ReadString(strLine))
	{
		if(!bMatchCase)
			strLine.MakeUpper();

		nLine++;
		int nPos = FindStringHelper(strLine, strText, bWholeWord);
		if(m_pLoadWorkspace && nPos > 0)
		{
			CString str = strFile.Right(3);
			UINT nFileType = FT_STRATEGY;

			if(str.CompareNoCase(_T("sts")) == 0)
				nFileType = FT_STRATEGY;
			else if(str.CompareNoCase(_T("uft")) == 0)
				nFileType = FT_USERFUNCTION;

			str = file.GetFileName();
			str.Delete(str.Find(".", 0), 4);

			CString strMessage(_T(""));
			if(nFileType == FT_STRATEGY)
				strMessage.Format(_T("[%s] 전략 스크립트 [%d(%d) Line] 에서 %s를 찾았습니다."), str, nLine, nPos, strText);
			else if(nFileType == FT_USERFUNCTION)
				strMessage.Format(_T("[%s] 함수 스크립트 [%d(%d) Line] 에서 %s를 찾았습니다."), str, nLine, nPos, strText);

			m_pLoadWorkspace->SetFindText(strMessage, nFileType);

			bReturn = TRUE;
		}
	}
	
	file.Close();
	return bReturn;
}

int CMainFrame::FindStringHelper(LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, BOOL bWholeWord)
{
	ASSERT(pszFindWhere != NULL);
	ASSERT(pszFindWhat != NULL);
	int nCur = 0;
	int nLength = lstrlen(pszFindWhat);
	for(;;)
	{
#ifdef _UNICODE
		LPCTSTR pszPos = wcsstr(pszFindWhere, pszFindWhat);
#else
		LPCTSTR pszPos = strstr(pszFindWhere, pszFindWhat);
#endif
		if(pszPos == NULL) return -1;
		if(! bWholeWord) return nCur + (pszPos - pszFindWhere);
		if(pszPos > pszFindWhere && (isalnum(pszPos[-1]) || pszPos[-1] == _T('_')))
		{
			nCur += (pszPos - pszFindWhere);
			pszFindWhere = pszPos + 1;
			continue;
		}
		if(isalnum(pszPos[nLength]) || pszPos[nLength] == _T('_'))
		{
			nCur += (pszPos - pszFindWhere + 1);
			pszFindWhere = pszPos + 1;
			continue;
		}
		return nCur + (pszPos - pszFindWhere);
	}
	ASSERT(FALSE);		// Unreachable
	return -1;
}


void CMainFrame::LoadExtLib()
{
	if(!m_pLoadExtLib)
		m_pLoadExtLib = new CLoadSystemTradingExtLibHelper;

//	m_pLoadExtLib->InitExtLib(NotifyExtLib, theApp.GetRootPath());

//	CPtrList* pList = m_pLoadExtLib->GetScriptList();
//	POSITION pos = pList->GetHeadPosition();
//	while(pos)
//	{
//		ST_SCRIPTINFO* pScriptInfo = (ST_SCRIPTINFO*)pList->GetNext(pos);
//	}

//	CString strScript;
//	BOOL bReturn = m_pLoadExtLib->GetScriptInfo(_T("이격도"), strScript);

//	m_pLoadExtLib->ReloadScriptInfo(theApp.GetRootPath());
}

void CMainFrame::LoadEncoder()
{
//@유진삭제
//	if(!m_pLoadEncoder)
//		m_pLoadEncoder = new CLoadSystemTradingEncoder;
//@유진삭제
	// 유진의 경우 디렉토리의 체계때문에 조건에서는 DLL을 로딩하지 못하므로 경로를 지정해줘야함.
	CString szDllName;
	//@Solomon szDllName.Format("%s\\%s\\%s", theApp.GetRootPath(), "chart", "SystemTradingEncoder.dll");
	szDllName.Format("%s%s\\%s", theApp.GetRootPath(), "\\dev", "SystemTradingEncoder.dll");
	if(!m_pLoadEncoder)
		m_pLoadEncoder = new CLoadSystemTradingEncoder((LPSTR)(LPCSTR)szDllName);
}

BOOL CMainFrame::EncodingFile(LPCSTR lpszSource, LPCSTR lpszDest, LPSTR lpszLine1/* = NULL*/, LPSTR lpszLine2/* = NULL*/, LPSTR lpszLine3/* = NULL*/)
{
	if(!m_pLoadEncoder)
		return FALSE;

	return m_pLoadEncoder->EncryptFile(lpszSource, lpszDest, lpszLine1, lpszLine2, lpszLine3);
}

BOOL CMainFrame::DecodingFile(LPCSTR lpszSource, LPCSTR lpszDest, LPSTR lpszLine1/* = NULL*/, LPSTR lpszLine2/* = NULL*/, LPSTR lpszLine3/* = NULL*/)
{
	if(!m_pLoadEncoder)
		return FALSE;

	return m_pLoadEncoder->DecryptFile(lpszSource, lpszDest, lpszLine1, lpszLine2, lpszLine3);
}

void CMainFrame::ReloadScriptInfoToExtLib()
{
	if(m_pLoadExtLib)
		m_pLoadExtLib->ReloadScriptInfo(theApp.GetRootPath());
}

void CMainFrame::DisplayZipError(LPSTR lpszNames)
{
	SetOutputText(CM_ZIP_FAILED_ALL, lpszNames);
	SetOutputText(CM_QUESTIONTOMANAGER, lpszNames);
}

void CMainFrame::StartCompileProgress()
{
	m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_UPFILE);
	SetTimer(IDT_STARTCOMPILE, 500, NULL);
}

void CMainFrame::StartCompileProgressZip()
{
	m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_UPFILE_ZIP);
	SetTimer(IDT_STARTCOMPILE_ZIP, 500, NULL);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == IDT_STARTCOMPILE)
	{
		if(m_nCompileProgressCount >= 10)
		{
			m_nCompileProgressCount = 0;
			KillTimer(IDT_STARTCOMPILE);
		}
		else
		{
			m_nCompileProgressCount++;
			m_ctrlCompileProgress.SetPos(m_ctrlCompileProgress.GetPos() + 3);
		}
	}
	else if(nIDEvent == IDT_STARTCOMPILE_ZIP)
	{
		if(m_nCompileProgressCount >= 20)
		{
			m_nCompileProgressCount = 0;
			KillTimer(IDT_STARTCOMPILE_ZIP);
		}
		else
		{
			m_nCompileProgressCount++;
			m_ctrlCompileProgress.SetPos(m_ctrlCompileProgress.GetPos() + 2);
		}
	}
	
	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::StopCompileProgress()
{
//	m_bCompiling = FALSE;

	m_nCompileProgressCount = 0;
	KillTimer(IDT_STARTCOMPILE);

	m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_DNFILE);
}

void CMainFrame::StopCompileProgressZip()
{
//	m_bCompiling = FALSE;

	m_nCompileProgressCount = 0;
	KillTimer(IDT_STARTCOMPILE_ZIP);

	m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_DNFILE);
}

void CMainFrame::CloseClientSocket()
{
	SetConnectionStatusTest(_T("오프라인"), RGB(255, 0, 0));

	if(m_pMainClient)
	{
		delete m_pMainClient;
		m_pMainClient = NULL;
	}

	SetServerMessage(_T("서버와의 연결이 종료되었습니다."), SVRMSG_WARNING);
}

void CMainFrame::OnUpdateConnectoserver(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((BOOL)!m_pMainClient);
}

void CMainFrame::OnConnectoserver() 
{
	if(!m_pMainClient)
	{
		SetConnectionStatusTest(_T("접속시도"));
		
		ConnectToCompileServer();

		if(m_pMainClient)
			SetServerMessage(_T("서버와의 연결에 성공하였습니다."), SVRMSG_NORMAL);
	}
}

void CMainFrame::StopCompiling(LPCSTR lpszName)
{
	if(m_pMainClient)
	{
		m_pMainClient->StopCompiling();
		for(int i = 0; i < 3; i++)		// 한번만 보내면 서버에서 못받는 경우가 있어서.. 여러번..
			m_pMainClient->SendToServer(SOCK_CMD_STOPCOMPILING, (LPVOID)lpszName, ::lstrlen(lpszName));
	}

	StopCompileProgress();
	StopCompileProgressZip();

	m_bCompiling = FALSE;

	SetConnectionStatusTest(_T("중지요청"));
	SetOutputText(CM_STOPCOMPILING, lpszName);

	if(m_pFileDll)
	{
		m_pFileDll->Close();
		delete m_pFileDll;
		m_pFileDll = NULL;
	}

	m_nStrategyType = 0;
	m_ctrlCompileProgress.ShowWindow(SW_HIDE);
}

void CMainFrame::OnFileOpenStrategy()
{
	char szFilter[] = "컴파일된 전략 파일|*.sd|컴파일된 함수 파일|*.sdf||";

	char szPath[MAX_PATH] = {NULL, };
//@유진삭제::wsprintf(szPath, _T("%s%s*.sd"), theApp.GetRootPath(), PATH_DLLFILES);
	::wsprintf(szPath, _T("%s%s*.sd"), theApp.GetUserPath(), PATH_DLLFILES2);

	DWORD dwFlag = OFN_HIDEREADONLY|OFN_EXTENSIONDIFFERENT;

	CFileDialog dlg(TRUE, "*.sd", szPath, dwFlag, szFilter, this);
	if(dlg.DoModal() == IDOK)
	{
		CString strExt(dlg.GetFileExt());

		if(strExt.CompareNoCase(_T("sd")) == 0)
			OpenScriptFileFromDll(dlg.GetPathName(), dlg.GetFileTitle(), FT_STRATEGY, szPath);
		else if(strExt.CompareNoCase(_T("sdf")) == 0)
			OpenScriptFileFromDll(dlg.GetPathName(), dlg.GetFileTitle(), FT_USERFUNCTION, szPath);
		else
			AfxMessageBox(_T("시스템 트레이딩 파일이 아닙니다."));
	}
}

void CMainFrame::OnFileOpenFunction()
{
	char szFilter[] = "컴파일된 전략 파일|*.sd|컴파일된 함수 파일|*.sdf||";

	char szPath[MAX_PATH] = {NULL, };
//@유진삭제::wsprintf(szPath, _T("%s%s*.sdf"), theApp.GetRootPath(), PATH_DLLFILES);
	::wsprintf(szPath, _T("%s%s*.sdf"), theApp.GetUserPath(), PATH_DLLFILES2);

	CFileDialog dlg(TRUE, "*.sdf", szPath, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, szFilter, this);
	if(dlg.DoModal() == IDOK)
	{
		CString strExt(dlg.GetFileExt());

		if(strExt.CompareNoCase(_T("sd")) == 0)
			OpenScriptFileFromDll(dlg.GetPathName(), dlg.GetFileTitle(), FT_STRATEGY, szPath);
		else if(strExt.CompareNoCase(_T("sdf")) == 0)
			OpenScriptFileFromDll(dlg.GetPathName(), dlg.GetFileTitle(), FT_USERFUNCTION, szPath);
		else
			AfxMessageBox(_T("시스템 트레이딩 파일이 아닙니다."));
	}
}

typedef CString (WINAPI* FpfnGetSrc)(long, LPCTSTR, LPCTSTR, long);
typedef CString (WINAPI* FpfnGetInputs)(long);
typedef LPCTSTR (WINAPI* FpfnGetStrInfo)(void);
typedef long (WINAPI* FpfnGetLongInfo)(void);
void CMainFrame::OpenScriptFileFromDll(LPCSTR lpszDllFile, LPCSTR lpszName, UINT nFileType, LPSTR lpszTemp)
{
	HINSTANCE hDll = ::LoadLibrary(lpszDllFile);
	if(!hDll)
		return;

	FpfnGetSrc	pfnGetSrc;
	pfnGetSrc = (FpfnGetSrc)::GetProcAddress(hDll, _T("fnGetSrc"));

	if(!pfnGetSrc)
	{
		AfxMessageBox(_T("시스템 트레이딩 파일이 아닙니다."));
		::FreeLibrary(hDll);
		return;
	}

	CDlgLoadScript dlg(lpszName);
	if(dlg.DoModal() != IDOK)
	{
		::FreeLibrary(hDll);
		return;
	}

//@유진삭제::wsprintf(lpszTemp, _T("%s%s%s.%s"), theApp.GetRootPath(), PATH_SCRIPTFILES, dlg.m_strName, nFileType == FT_STRATEGY ? _T("sts") : _T("uft"));
	::wsprintf(lpszTemp, _T("%s%s%s.%s"), theApp.GetUserPath(), PATH_SCRIPTFILES2, dlg.m_strName, nFileType == FT_STRATEGY ? _T("sts") : _T("uft"));

	char szConfig[MAX_PATH] = {NULL, };
	::GetSTFile(szConfig, lpszName, theApp.GetRootPath());

	CFileFind ff;
	BOOL bConfigFile = ff.FindFile(szConfig);

	::GetSTFile(szConfig, dlg.m_strName, theApp.GetRootPath());

	if(bConfigFile)
	{
		ff.FindNextFile();
		::CopyFile(ff.GetFilePath(), szConfig, FALSE);
	}

	ff.Close();

	CStdioFile fileScript(lpszTemp, CFile::modeCreate|CFile::modeWrite);

	CString strLine(_T(""));
	long nLine = 0;
	while(TRUE)
	{
		strLine = pfnGetSrc(nLine++, _T(""), _T(""), 0);
		if(strLine.Compare(_T("#")) == 0)
			break;

		fileScript.WriteString(strLine);
		fileScript.Write(_T("\n"), 1);
	}

	fileScript.Close();
	
	FpfnGetStrInfo	pfnGetStrInfo;

	pfnGetStrInfo = (FpfnGetStrInfo)::GetProcAddress(hDll, _T("fnGetUserID"));
	CString strUserID(_T(""));
	if(pfnGetStrInfo)
		strUserID = pfnGetStrInfo();

	pfnGetStrInfo = (FpfnGetStrInfo)::GetProcAddress(hDll, _T("fnGetPassword"));
	CString strPassword(_T(""));
	if(pfnGetStrInfo)
		strPassword = pfnGetStrInfo();

	FpfnGetLongInfo pfnGetLongInfo = (FpfnGetLongInfo)::GetProcAddress(hDll, _T("fnGetExpiredDate"));
	long nExpires = 0;
	if(pfnGetLongInfo)
		nExpires = pfnGetLongInfo();

	pfnGetLongInfo = (FpfnGetLongInfo)::GetProcAddress(hDll, _T("fnGetUseUserID"));
	BOOL bUseUserID = FALSE;
	if(pfnGetLongInfo)
		bUseUserID = (BOOL)pfnGetLongInfo();

	pfnGetLongInfo = (FpfnGetLongInfo)::GetProcAddress(hDll, _T("fnGetUsePassword"));
	BOOL bUsePassword = FALSE;
	if(pfnGetLongInfo)
		bUsePassword = (BOOL)pfnGetLongInfo();

	pfnGetLongInfo = (FpfnGetLongInfo)::GetProcAddress(hDll, _T("fnGetUseExpriedDate"));
	BOOL bUseExpires = FALSE;
	if(pfnGetLongInfo)
		bUseExpires = (BOOL)pfnGetLongInfo();

	char szLine[MAX_PATH + 1] = {NULL, };
	::wsprintf(szLine, _T("%d,%s,%d,%s,%d,%8d"), 
		bUsePassword, strPassword, 
		bUseUserID, strUserID, 
		bUseExpires, nExpires);
	
	char szBackupFile[MAX_PATH] = {NULL, };
	::wsprintf(szBackupFile, _T("%s.bak"), lpszTemp);

	EncodingFile(lpszTemp, szBackupFile, szLine);
	::CopyFile(szBackupFile, lpszTemp, FALSE);
	::DeleteFile(szBackupFile);

	if(!bConfigFile)
	{
		LPCSTR lpszSection = _T("Basic");
		::WritePrivateProfileString(lpszSection, _T("Explanation"), _T("여기에 설명을 입력하십시오"), szConfig);

		char szSourceType[2] = {0, NULL};
		pfnGetLongInfo = (FpfnGetLongInfo)::GetProcAddress(hDll, _T("fnGetSourceType"));
		if(pfnGetLongInfo)
			::wsprintf(szSourceType, _T("%d"), pfnGetLongInfo());

		::WritePrivateProfileString(lpszSection, _T("LangType"), szSourceType, szConfig);
		
		if(nFileType == FT_STRATEGY)
			::WritePrivateProfileString(lpszSection, _T("Compiled"), _T("5"), szConfig);
		else if(nFileType == FT_USERFUNCTION)
			::WritePrivateProfileString(lpszSection, _T("Compiled"), _T("11"), szConfig);

		FpfnGetInputs	pfnGetInputs;
		pfnGetInputs = (FpfnGetInputs)::GetProcAddress(hDll, _T("fnGetInputs"));
		if(pfnGetInputs)
		{
			nLine = 0;

			lpszSection = _T("Variables");
			char szKeyName[12] = {NULL, };
			char szKeyVal[8] = {NULL, };

			int nIndex = 0;
			while(TRUE)
			{
				strLine = pfnGetInputs(nLine);
				if(strLine.Compare(_T("#")) == 0)
				{
					::wsprintf(szKeyName, _T("%d"), nLine);
					::WritePrivateProfileString(lpszSection, _T("InputCnt"), szKeyName, szConfig);
					break;
				}

				::wsprintf(szKeyName, _T("InputName%d"), nLine);
				::wsprintf(szKeyVal, _T("Input%d"), nLine);
				
				nIndex = strLine.Find('(', 0);
				::WritePrivateProfileString(lpszSection, szKeyName, strLine.Left(nIndex), szConfig);

				strLine.Delete(0, nIndex + 1);

				nIndex = strLine.Find(')', 0);
				::WritePrivateProfileString(lpszSection, szKeyVal, strLine.Left(nIndex), szConfig);

				nLine++;
			}
		}
	}

	::FreeLibrary(hDll);

	UINT nID = TREEINDEX_DEFUALTITEM;
	if(nFileType == FT_STRATEGY)
	{
		if(!m_vtStrategyInfo.empty())
		{
			VT_SCRIPTINFO::reverse_iterator rit = m_vtStrategyInfo.rbegin();
			nID = (*rit)->GetUniqueID() + 1;
		}
		
		CScriptInfomation* pStrategy = new CScriptInfomation(dlg.m_strName, nID, nFileType, IMG_STS);
		m_vtStrategyInfo.push_back(pStrategy);
		
		if(m_pLoadWorkspace)
			m_pLoadWorkspace->AddListStrategy(pStrategy);
	}
	else if(nFileType == FT_USERFUNCTION)
	{
		if(!m_vtUserFuncInfo.empty())
		{
			VT_SCRIPTINFO::reverse_iterator rit = m_vtUserFuncInfo.rbegin();
			nID = (*rit)->GetUniqueID() + 1;
		}
		
		CScriptInfomation* pUserFunc = new CScriptInfomation(dlg.m_strName, nID++, nFileType, IMG_FUNC);
		m_vtUserFuncInfo.push_back(pUserFunc);
		
		if(m_pLoadWorkspace)
			m_pLoadWorkspace->AddListFunction(pUserFunc);
	}

	theApp.OpenDocumentFile(lpszTemp);
}

void CMainFrame::OnTradestationView() 
{
	CDlgConfig dlg(ST_TRADESTATION);
	if(dlg.DoModal() != IDOK)
		return;

	ApplyView();
}

void CMainFrame::OnYestraderView() 
{
	CDlgConfig dlg(ST_YESTRADER);
	if(dlg.DoModal() != IDOK)
		return;

	ApplyView();
}

void CMainFrame::ApplyView()
{
	POSITION posDoc = theApp.m_pDocTemplate->GetFirstDocPosition();
	while(posDoc)
	{
		CSystemTradingDoc* pDoc = (CSystemTradingDoc*)theApp.m_pDocTemplate->GetNextDoc(posDoc);
		pDoc->ApplyClientConfig();
	}
}

void CMainFrame::SetServerMessage(LPCSTR lpszMessage, UINT nLevel)
{
	if(m_pLoadWorkspace)
		m_pLoadWorkspace->SetServerMessage(lpszMessage, nLevel);
}

void CMainFrame::ShowDynamicHelp(LPCSTR lpszText)
{
	if(!m_pViewHelp || !m_pViewHelp->IsWindowVisible())
		return;

	m_pViewHelp->ShowHelp(lpszText);
}

LRESULT CMainFrame::OnStartDrag(WPARAM wParam, LPARAM lParam)
{
	CStartDrag* pStartDrag = (CStartDrag*)lParam;

	SetActiveWindow();
	CRect rcClient;
	GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	CPoint point;
	GetCursorPos(&point);

	DROPEFFECT RETURNA = m_DropSource.StartDragging((DWORD)this, &rcClient, &point);

	delete pStartDrag;
	
	return RETURNA;
}

void CMainFrame::CheckupBackupFiles()
{
	char szPath[MAX_PATH] = {NULL, };
//@유진삭제::wsprintf(szPath, _T("%s%s*.bak"), theApp.GetRootPath(), PATH_SCRIPTFILES);
	::wsprintf(szPath, _T("%s%s*.bak"), theApp.GetUserPath(), PATH_SCRIPTFILES2);

	CFileFind ff;
	BOOL bFind = ff.FindFile(szPath);

	CString strPath(_T(""));
	while(bFind)
	{
		bFind = ff.FindNextFile();
		strPath = ff.GetFilePath();

		::lstrcpy(szPath, strPath.Left(strPath.GetLength() - 4));

		CFileFind ff2;
		BOOL bFind2 = ff2.FindFile(szPath);
		if(bFind2)
		{
			ff2.FindNextFile();
			::SetFileAttributes(szPath, FILE_ATTRIBUTE_NORMAL);
		}
		
		if(!bFind2 || ff2.IsHidden())
		{
			::CopyFile(strPath, szPath, FALSE);
			
			::wsprintf(szPath, _T("%s 백업파일을 복구했습니다."), ff2.GetFileName().Left(ff2.GetFileName().GetLength() - 4)); 
			m_starBackupFile.Add(szPath);
		}

		::DeleteFile(strPath);
	}
}

void CMainFrame::MakeSTFile(CString strSTName, LPCSTR szSDFileName)
{
	char szPath[MAX_PATH] = {NULL, };
	char szNewPath[MAX_PATH] = {NULL, };

	::GetSTFile(szPath, strSTName, theApp.GetRootPath());
//@유진삭제::wsprintf(szNewPath, _T("%s%s%s@%d.ST"), theApp.GetRootPath(), PATH_STFILES, strSTName, m_nStrategyType);

	//파일이름에 @대신 ^으로 변경.
	//::wsprintf(szNewPath, _T("%s%s%s@%d.ST"), theApp.GetUserPath(), PATH_STFILES2, strSTName, m_nStrategyType);
	::wsprintf(szNewPath, _T("%s%s%s%c%d.ST"), theApp.GetUserPath(), PATH_STFILES2, strSTName, gSTGubunKey, m_nStrategyType);

	//KHD : 파일 핸들 얻어온 PCTR 핸들에 파일명과 함께 던져줌.
	if(IsWindow(m_hPctrWnd))
	{
		if(m_nStrategyType!=0)
		{
			CString FileName;
			//FileName.Format("%s@%d",strSTName,m_nStrategyType);
			FileName.Format("%s%c%d",strSTName,gSTGubunKey, m_nStrategyType);

			char aData[120]={0,};
			sprintf(aData, "%s", FileName);

			COPYDATASTRUCT copyData;
			copyData.dwData = 100;
			copyData.cbData = ::lstrlen(aData) + sizeof(DWORD);
			copyData.lpData = (void*)(LPCSTR)aData;

			::SendMessage(m_hPctrWnd, WM_COPYDATA , copyData.dwData, (LPARAM)&copyData);
		}
	}else m_hPctrWnd = NULL;
	////////////////
	if(::lstrlen(szPath))
	{
		::MoveFile(szPath, szNewPath);
	}
	else
	{
		::wsprintf(szPath, _T("%s%sNewScript.ST"), theApp.GetRootPath(), PATH_STDATA);
		::CopyFile(szPath, szNewPath, FALSE);
	}

	::WritePrivateProfileString(_T("StrategyList"), _T("PosName0"), strSTName, szNewPath);
	MakeSTFileConfig(szNewPath, szSDFileName, strSTName);
}

LRESULT CMainFrame::OnGetCompileStateTree(WPARAM wParam, LPARAM lParam)
{
	return m_bCompiling || m_bMultiCompile;
}

//[[ 추가루틴 06.12.07 Junok Lee
// 목적 : ST파일에 PosText0, 추가.
//	      파라미터를 갖는 경우 sfg생성
void CMainFrame::MakeSTFileConfig(LPCSTR szSTFileName, LPCSTR szSDFileName, LPCSTR szSTName)
{
	const int nSize = 20480;
	char aBuffer[nSize];
	if(szSDFileName!=NULL && ::GetPrivateProfileSection("Variables", aBuffer, nSize, szSTFileName)>0)
	{
		CString szSfg(szSDFileName);
		int nFind = szSfg.ReverseFind('.');
		if(nFind>0)
		{
			szSfg.Format("%s%s", szSfg.Left(nFind) ,  ".sfg");
			::WritePrivateProfileSection("Variables", aBuffer, szSfg);
		}
	}

	int nCount = ::GetPrivateProfileInt("Variables", "InputCnt", 0, szSTFileName);
	if(nCount>0)
	{
		CString szData=(szSTName), szKey;
		szData += ("(");
		for(int i=0; i<nCount; i++)
		{
			szKey.Format("Input%d", i);
			::GetPrivateProfileString("Variables", szKey, "", aBuffer, nSize, szSTFileName);
			szData += aBuffer;
			if(nCount!=i+1)
				szData += ",";
		}
		szData += (")");
		::WritePrivateProfileString(_T("StrategyList"), _T("PosText0"), szData, szSTFileName);
	}
	else
	{
		::WritePrivateProfileString(_T("StrategyList"), _T("PosText0"), szSTName, szSTFileName);
	}
}
//]]