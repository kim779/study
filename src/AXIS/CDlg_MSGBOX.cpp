// CDlg_MSGBOX.cpp: 구현 파일
//

#include "stdafx.h"
#include "axis.h"
#include "CDlg_MSGBOX.h"
//#include "afxdialogex.h"


// CDlg_MSGBOX 대화 상자

IMPLEMENT_DYNAMIC(CDlg_MSGBOX, CDialog)

CDlg_MSGBOX::CDlg_MSGBOX(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_MSGBOX, pParent)
{

}

CDlg_MSGBOX::~CDlg_MSGBOX()
{
}

void CDlg_MSGBOX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_MSGBOX, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_MSGBOX::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlg_MSGBOX::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlg_MSGBOX 메시지 처리기


void CDlg_MSGBOX::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*
	/*
	 더 편리한 HTS 환경을 제공해 드리기 위해 이용빈도가 낮은 일부화면을 정리하게 되었습니다.

                                 고객님의 양해 부탁드리며, 더 나은 서비스로 다가갈 수 있도록 최선의 노력을 다하겠습니다.

                                 (공지사항 바로가기)

            //                     (바로가기 클릭 시 https://www.ibks.com/notice/notice_view.do?seq=5780 링크)
	*/
	CDialog::OnOK();
}


void CDlg_MSGBOX::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}
