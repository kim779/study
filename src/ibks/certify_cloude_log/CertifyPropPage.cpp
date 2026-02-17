// CertifyPropPage.cpp : CCertifyPropPage 속성 페이지 클래스의 구현입니다.

#include "pch.h"
#include "framework.h"
#include "Certify.h"
#include "CertifyPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCertifyPropPage, COlePropertyPage)

// 메시지 맵입니다.

BEGIN_MESSAGE_MAP(CCertifyPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 클래스 팩터리와 GUID를 초기화합니다.

IMPLEMENT_OLECREATE_EX(CCertifyPropPage, "AxisCertify.CertifyPropPage.IBK2019",
	0x24526ecf,0x5a7b,0x49c2,0x9a,0x02,0xc7,0xfc,0x57,0x5a,0x39,0xb5)

// CCertifyPropPage::CCertifyPropPageFactory::UpdateRegistry -
// CCertifyPropPage에 대한 시스템 레지스트리 항목을 추가하거나 제거합니다.

BOOL CCertifyPropPage::CCertifyPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CERTIFY_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, nullptr);
}

// CCertifyPropPage::CCertifyPropPage - 생성자

CCertifyPropPage::CCertifyPropPage() :
	COlePropertyPage(IDD, IDS_CERTIFY_PPG_CAPTION)
{
}

// CCertifyPropPage::DoDataExchange - 페이지와 속성 간 데이터를 이동합니다.

void CCertifyPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CCertifyPropPage 메시지 처리기
