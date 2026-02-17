// SockPropPage.cpp : CSockPropPage 속성 페이지 클래스의 구현입니다.

#include "pch.h"
#include "framework.h"
#include "Sock.h"
#include "SockPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CSockPropPage, COlePropertyPage)

// 메시지 맵입니다.

BEGIN_MESSAGE_MAP(CSockPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 클래스 팩터리와 GUID를 초기화합니다.

IMPLEMENT_OLECREATE_EX(CSockPropPage, "AxisSock.SockPropPage.IBK2019",
	0x4984abf1,0xa7fe,0x42e1,0x97,0x6c,0x18,0xc8,0xe5,0x1e,0x02,0x41)

// CSockPropPage::CSockPropPageFactory::UpdateRegistry -
// CSockPropPage에 대한 시스템 레지스트리 항목을 추가하거나 제거합니다.

BOOL CSockPropPage::CSockPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SOCK_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, nullptr);
}

// CSockPropPage::CSockPropPage - 생성자

CSockPropPage::CSockPropPage() :
	COlePropertyPage(IDD, IDS_SOCK_PPG_CAPTION)
{
}

// CSockPropPage::DoDataExchange - 페이지와 속성 간 데이터를 이동합니다.

void CSockPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CSockPropPage 메시지 처리기
