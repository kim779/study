// CDlg_book.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_book.h"
#include "afxdialogex.h"

#include "CSRWSR.h"

// CDlg_book 대화 상자

IMPLEMENT_DYNAMIC(CDlg_book, CDialogEx)

CDlg_book::CDlg_book(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CHOLBOOK, pParent)
{

}

CDlg_book::~CDlg_book()
{
}

void CDlg_book::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_book, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_book::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg_book::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlg_book 메시지 처리기


void cs_92p1::TestFunc(int a, int b , int c )
{

}
void cs_92p1::TestFunc(int a, int b )
{

}

void CDlg_book::OnBnClickedButton1()
{
	cs_92p1 cs1;
	//cs1.TestFunc(1, 2);
}



void CDlg_book::OnBnClickedButton2()
{
	//cs_RW cs;
	//BOOL bret = cs.IsDateBigAandB(20230201, GetCurrentTime() );
}
