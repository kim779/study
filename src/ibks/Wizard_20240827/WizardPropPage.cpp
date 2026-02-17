// WizardPropPage.cpp : CWizardPropPage 속성 페이지 클래스의 구현입니다.

#include "pch.h"
#include "framework.h"
#include "Wizard.h"
#include "WizardPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CWizardPropPage, COlePropertyPage)

// 메시지 맵입니다.

BEGIN_MESSAGE_MAP(CWizardPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 클래스 팩터리와 GUID를 초기화합니다.

IMPLEMENT_OLECREATE_EX(CWizardPropPage, "AxisWizard.WizardPropPage.IBK2019",
	0xbbc048ca,0xf7f5,0x458b,0xa7,0x40,0x32,0xfd,0xa0,0x3d,0xfd,0xa0)

// CWizardPropPage::CWizardPropPageFactory::UpdateRegistry -
// CWizardPropPage에 대한 시스템 레지스트리 항목을 추가하거나 제거합니다.

BOOL CWizardPropPage::CWizardPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_WIZARD_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, nullptr);
}

// CWizardPropPage::CWizardPropPage - 생성자

CWizardPropPage::CWizardPropPage() :
	COlePropertyPage(IDD, IDS_WIZARD_PPG_CAPTION)
{
}

// CWizardPropPage::DoDataExchange - 페이지와 속성 간 데이터를 이동합니다.

void CWizardPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CWizardPropPage 메시지 처리기
