// TestDLL.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "TestDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// CTestDLLApp

BEGIN_MESSAGE_MAP(CTestDLLApp, CWinApp)
END_MESSAGE_MAP()


// CTestDLLApp 생성

CTestDLLApp::CTestDLLApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTestDLLApp 개체입니다.

CTestDLLApp theApp;


// CTestDLLApp 초기화

BOOL CTestDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#include "CMainWnd.h"

#include "CSampleWnd.h"
__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	OutputDebugString("[IB714100]  axcreate");

	if (1)
	{
		CSampleWnd* pMapWnd = new CSampleWnd(parent);

		if (!pMapWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
			CRect(0, 0, 750, 660), parent, 0, NULL))
		{
			return nullptr;
		}
		return pMapWnd;
	}
	else
	{
		CMainWnd* pMapWnd = new CMainWnd(parent);

		if (!pMapWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
			CRect(0, 0, 750, 660), parent, 0, NULL))
		{
			return nullptr;
		}
		return pMapWnd;
	}
}

