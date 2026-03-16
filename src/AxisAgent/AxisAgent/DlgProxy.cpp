
// DlgProxy.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "AxisAgent.h"
#include "DlgProxy.h"
#include "AxisAgentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAxisAgentDlgAutoProxy

IMPLEMENT_DYNCREATE(CAxisAgentDlgAutoProxy, CCmdTarget)

CAxisAgentDlgAutoProxy::CAxisAgentDlgAutoProxy()
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
		ASSERT_KINDOF(CAxisAgentDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CAxisAgentDlg)))
		{
			m_pDialog = reinterpret_cast<CAxisAgentDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CAxisAgentDlgAutoProxy::~CAxisAgentDlgAutoProxy()
{
	// 모든 개체가 OLE 자동화로 만들어졌을 때 애플리케이션을 종료하기 위해
	// 	소멸자가 AfxOleUnlockApp를 호출합니다.
	//  이러한 호출로 주 대화 상자가 삭제될 수 있습니다.
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CAxisAgentDlgAutoProxy::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하세요.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CAxisAgentDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAxisAgentDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 참고: IID_IAxisAgent에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다.
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {dfd75f25-bcd8-4c1f-b686-b305d73fba67}
static const IID IID_IAxisAgent =
{0xdfd75f25,0xbcd8,0x4c1f,{0xb6,0x86,0xb3,0x05,0xd7,0x3f,0xba,0x67}};

BEGIN_INTERFACE_MAP(CAxisAgentDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CAxisAgentDlgAutoProxy, IID_IAxisAgent, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 매크로가 이 프로젝트의 pch.h에 정의됩니다.
// {f67dc37b-5139-450a-9a14-d0bad2e406bf}
IMPLEMENT_OLECREATE2(CAxisAgentDlgAutoProxy, "AxisAgent.Application", 0xf67dc37b,0x5139,0x450a,0x9a,0x14,0xd0,0xba,0xd2,0xe4,0x06,0xbf)


// CAxisAgentDlgAutoProxy 메시지 처리기
