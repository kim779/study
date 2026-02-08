// CMngTabSetup.cpp: 구현 파일
//

#include "stdafx.h"
#include "CMngTabSetup.h"


// CMngTabSetup 대화 상자

CMngTabSetup::CMngTabSetup(CWnd* pParent /*=nullptr*/)
	: CSetupDialog(IDD_MNGTABSETUP, pParent)
{
	m_caption = "장운영정보 설정";
}

CMngTabSetup::~CMngTabSetup()
{
}

void CMngTabSetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMngTabSetup, CSetupDialog)
END_MESSAGE_MAP()


// CMngTabSetup 메시지 처리기
