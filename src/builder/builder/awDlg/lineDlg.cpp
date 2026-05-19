// lineDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "lineDlg.h"
#include "afxdialogex.h"


// CLineDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLineDlg, CDialogEx)

CLineDlg::CLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLineDlg::IDD, pParent)
{
	m_lineNum = 0;
}

CLineDlg::~CLineDlg()
{
}

void CLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITLINE, m_lineNum);
	DDV_MinMaxInt(pDX, m_lineNum, 0, 10000000);
}


BEGIN_MESSAGE_MAP(CLineDlg, CDialogEx)
END_MESSAGE_MAP()


// CLineDlg 메시지 처리기입니다.
