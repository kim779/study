// CheckOutDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "CheckOutDlg.h"
#include "afxdialogex.h"


// CCheckOutDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCheckOutDlg, CDialogEx)

CCheckOutDlg::CCheckOutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckOutDlg::IDD, pParent)
	, m_sMapN(_T(""))
{

}

CCheckOutDlg::~CCheckOutDlg()
{
}

void CCheckOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAP_NAME, m_sMapN);
}


BEGIN_MESSAGE_MAP(CCheckOutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCheckOutDlg 메시지 처리기입니다.


//void CCheckOutDlg::PreInitDialog()
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	CDialogEx::PreInitDialog();
//}


BOOL CCheckOutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
