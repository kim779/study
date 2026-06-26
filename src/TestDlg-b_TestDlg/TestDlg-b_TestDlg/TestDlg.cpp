
// TestDlg.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "TestDlg.h"
#include "TestDlgDlg.h"
#include "whdump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDlgApp

BEGIN_MESSAGE_MAP(CTestDlgApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestDlgApp 생성

CTestDlgApp::CTestDlgApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTestDlgApp 개체입니다.

CTestDlgApp theApp;


// CTestDlgApp 초기화

BOOL CTestDlgApp::InitInstance()
{
// TODO: richedit2 라이브러리를 초기화하려면 AfxInitRichEdit2()을(를) 호출합니다.\n"	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	//HANDLE hRealHandle = 0;
	//DuplicateHandle(GetCurrentProcess(), // Source Process Handle.
	//	GetCurrentThread(),  // Source Handle to dup.
	//	GetCurrentProcess(), // Target Process Handle.
	//	&hRealHandle,        // Target Handle pointer.
	//	0,                   // Options flag.
	//	TRUE,                // Inheritable flag
	//	DUPLICATE_SAME_ACCESS);// Options

	char chdata[256];
	GetModuleFileName(nullptr, chdata, 256);

	CString spath, sData;
	spath.Format("%s", chdata);

	int ifind = spath.ReverseFind('\\');
	spath = spath.Left(ifind);
	spath += "\\Affinity.ini";

	memset(chdata, 0x00, 256);
	GetPrivateProfileString("PROCESS", "CORE", "", chdata, 256, spath);
	CString sRes;
	sRes.Format("%s", chdata);
	sRes.TrimRight();
	DWORD_PTR dwProcessAffinityMas{};
	switch (atoi(sRes))
	{
		case 1:
			dwProcessAffinityMas = 0x01;  //0x01 -> 0001   -> 1번 코어만
			break;
		case 2:
			dwProcessAffinityMas = 0x02;   //0x02 -> 0010   -> 2번 코어만
			break;
		case 3:
			dwProcessAffinityMas = 0x03;  //0x03 -> 0011   -> 3번 코어만
			break;
		case 4:
			dwProcessAffinityMas = 0x04;  //0x04 -> 0100   -> 4번 코어만
			break;
		case 5:
			dwProcessAffinityMas = 0x05;   //0x05 -> 0101   -> 5번 코어만
			break;
		case 6:
			dwProcessAffinityMas = 0x06;   //0x06 -> 0110   -> 6번 코어만
			break;
		case 7:
			dwProcessAffinityMas = 0x07;  //0x07 -> 0111   -> 7번 코어만
			break;
		case 8:
			dwProcessAffinityMas = 0x08;  //0x08 -> 1000   -> 8번 코어만
			break;
	}
	//DWORD_PTR dwProcessAffinityMask = 0x01| 0x02| 0x03| 0x04| 0x05| 0x06| 0x07| 0x08| 0x09| 0x0a;
	//if (SetProcessAffinityMask(GetCurrentProcess(), dwProcessAffinityMas) == 0) {
	//	// Handle the error
	//	int ierror = GetLastError();
	//	CString slog;
	//	slog.Format(" 프로세스 친화도 실패[%d]", ierror);
	//	AfxMessageBox(slog);
	//	return FALSE;
	//}

	CWinApp::InitInstance();
	const MiniDumper mDump(_T("MiniDump"));
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));

	CTestDlgDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.




	return FALSE;
}



BOOL CTestDlgApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinApp::PreTranslateMessage(pMsg);
}
