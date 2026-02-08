
// DlgProxy.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Regester.h"
#include "DlgProxy.h"
#include "RegesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRegesterDlgAutoProxy

IMPLEMENT_DYNCREATE(CRegesterDlgAutoProxy, CCmdTarget)

CRegesterDlgAutoProxy::CRegesterDlgAutoProxy()
{
	EnableAutomation();

	// 자동화 개체가 활성화되어 있는 동안 계속 애플리케이션을 실행하기 위해
	//	생성자에서 AfxOleLockApp를 호출합니다.
	AfxOleLockApp();

	// 애플리케이션의 주 창 포인터를 통해 대화 상자에 대한
	//  액세스를 가져옵니다.  프록시의 내부 포인터를 설정하여
	//  대화 상자를 가리키고 대화 상자의 후방 포인터를 이 프록시로
	//  설정합니다.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CRegesterDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CRegesterDlg)))
		{
			m_pDialog = reinterpret_cast<CRegesterDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CRegesterDlgAutoProxy::~CRegesterDlgAutoProxy()
{
	// 모든 개체가 OLE 자동화로 만들어졌을 때 애플리케이션을 종료하기 위해
	// 	소멸자가 AfxOleUnlockApp를 호출합니다.
	//  이러한 호출로 주 대화 상자가 삭제될 수 있습니다.
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CRegesterDlgAutoProxy::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하세요.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CRegesterDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRegesterDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 참고: IID_IRegester에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다.
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {b94b75ef-16d9-4cb7-b837-a111d278db64}
static const IID IID_IRegester =
{0xb94b75ef,0x16d9,0x4cb7,{0xb8,0x37,0xa1,0x11,0xd2,0x78,0xdb,0x64}};

BEGIN_INTERFACE_MAP(CRegesterDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CRegesterDlgAutoProxy, IID_IRegester, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 매크로가 이 프로젝트의 pch.h에 정의됩니다.
// {3644d613-c69d-432b-a4fa-1d2b8da8ebb8}
IMPLEMENT_OLECREATE2(CRegesterDlgAutoProxy, "Regester.Application", 0x3644d613,0xc69d,0x432b,0xa4,0xfa,0x1d,0x2b,0x8d,0xa8,0xeb,0xb8)


// CRegesterDlgAutoProxy 메시지 처리기
