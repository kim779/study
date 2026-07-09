// testdlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axis.h"
#include "testdlg.h"
#include "afxdialogex.h"


// testdlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(testdlg, CDialogEx)

testdlg::testdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(testdlg::IDD, pParent)
{

}

testdlg::~testdlg()
{
}

void testdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(testdlg, CDialogEx)
END_MESSAGE_MAP()


// testdlg 메시지 처리기입니다.
