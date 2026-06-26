// CDlg_player.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_player.h"
#include "afxdialogex.h"


// CDlg_player 대화 상자

IMPLEMENT_DYNAMIC(CDlg_player, CDialog)

CDlg_player::CDlg_player(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_PLAYER, pParent)
{

}

CDlg_player::~CDlg_player()
{
}

void CDlg_player::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_player, CDialog)
END_MESSAGE_MAP()


// CDlg_player 메시지 처리기
