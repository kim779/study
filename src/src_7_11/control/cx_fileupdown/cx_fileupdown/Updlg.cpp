// Updlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "cx_fileupdown.h"
#include "Updlg.h"
#include "afxdialogex.h"


// CUpdlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUpdlg, CDialogEx)

CUpdlg::CUpdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdlg::IDD, pParent)
{

}

CUpdlg::~CUpdlg()
{
}

void CUpdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UP, m_uplist);
}


BEGIN_MESSAGE_MAP(CUpdlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_FILE, &CUpdlg::OnBnClickedBtnFile)
END_MESSAGE_MAP()


// CUpdlg 메시지 처리기입니다.


void CUpdlg::OnBnClickedBtnFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char name_filter[] = "All Files (*.*)|*.*|";
	CFileDialog ins_dlg(TRUE, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);
	// 파일 형식 콤보박스에 나열된 필터들 중에서 2번째 항목(*.cpp)을 선택한다.
	ins_dlg.m_ofn.nFilterIndex = 2;

	if(ins_dlg.DoModal() == IDOK)
	{
		CString str(ins_dlg.GetPathName());
		
	}
}
