// UserSaveDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axis.h"
#include "UserSaveDlg.h"
#include "afxdialogex.h"


// CUserSaveDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserSaveDlg, CDialogEx)

CUserSaveDlg::CUserSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserSaveDlg::IDD, pParent)
{

}

CUserSaveDlg::~CUserSaveDlg()
{
}

void CUserSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserSaveDlg, CDialogEx)
END_MESSAGE_MAP()


// CUserSaveDlg 메시지 처리기입니다.
