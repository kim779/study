// TriDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CX_POP.h"
#include "TriDlg.h"
#include "afxdialogex.h"


// CTriDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTriDlg, CDialogEx)

CTriDlg::CTriDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTriDlg::IDD, pParent)
{

}

CTriDlg::~CTriDlg()
{
}

void CTriDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTriDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTriDlg 메시지 처리기입니다.


void CTriDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(249,249,249));
	
	CPen	pen;
	pen.CreatePen(PS_SOLID, 1, RGB(183,187,209));
	CPen	*pOldPen = dc.SelectObject(&pen);

	dc.MoveTo(rc.left, rc.bottom);
	dc.LineTo(rc.right/2, rc.top);

	dc.MoveTo(rc.right/2, rc.top);
	dc.LineTo(rc.right, rc.bottom);

	dc.SelectObject(pOldPen);
}


BOOL CTriDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	/*
	LONG style = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
	style |= WS_EX_LAYERED;
	int iresult = SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, style);
	::SetLayeredWindowAttributes(GetSafeHwnd(), 0,128, LWA_ALPHA);
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
