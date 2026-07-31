// CMDLSbox.cpp: 구현 파일
//

#include "stdafx.h"
#include "IMAXContainer.h"
#include "CMDLSbox.h"
#include "afxdialogex.h"


// CMDLSbox 대화 상자

IMPLEMENT_DYNAMIC(CMDLSbox, CDialog)

CMDLSbox::CMDLSbox(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_MDLS, pParent)
{
	m_pParent = pParent;
}

CMDLSbox::~CMDLSbox()
{
}

void CMDLSbox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMDLSbox, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CMDLSbox::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMDLSbox::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMDLSbox 메시지 처리기


void CMDLSbox::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}


BOOL CMDLSbox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetDlgItemText(IDC_STATIC, m_text);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMDLSbox::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
		m_pParent->PostMessage(WM_USER + 999, 1, 0);
	CDialog::OnOK();
}


void CMDLSbox::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
		m_pParent->PostMessage(WM_USER + 999, 0, 0);
	CDialog::OnCancel();
}


BOOL CMDLSbox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
