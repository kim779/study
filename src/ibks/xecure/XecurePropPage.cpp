// XecurePropPage.cpp : CXecurePropPage 속성 페이지 클래스의 구현입니다.

#include "pch.h"
#include "framework.h"
#include "Xecure.h"
#include "XecurePropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CXecurePropPage, COlePropertyPage)

// 메시지 맵입니다.

BEGIN_MESSAGE_MAP(CXecurePropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 클래스 팩터리와 GUID를 초기화합니다.

IMPLEMENT_OLECREATE_EX(CXecurePropPage, "AxisXecure.XecurePropPage.IBK2019",
	0x94f8c7ca,0x87c0,0x48fc,0x9c,0xb7,0xf2,0x0a,0x42,0x98,0x0c,0xb8)

// CXecurePropPage::CXecurePropPageFactory::UpdateRegistry -
// CXecurePropPage에 대한 시스템 레지스트리 항목을 추가하거나 제거합니다.

BOOL CXecurePropPage::CXecurePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_XECURE_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, nullptr);
}

// CXecurePropPage::CXecurePropPage - 생성자

CXecurePropPage::CXecurePropPage() :
	COlePropertyPage(IDD, IDS_XECURE_PPG_CAPTION)
{
}

// CXecurePropPage::DoDataExchange - 페이지와 속성 간 데이터를 이동합니다.

void CXecurePropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CXecurePropPage 메시지 처리기
