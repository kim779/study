// autoToolTip.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "autoToolTip.h"
#include "afxdialogex.h"


// CAutoToolTip 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoToolTip, CDialogEx)

CAutoToolTip::CAutoToolTip(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoToolTip::IDD, pParent)
{
	m_parent = pParent;
}

CAutoToolTip::~CAutoToolTip()
{
}

void CAutoToolTip::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoToolTip, CDialogEx)
	ON_WM_MOUSEACTIVATE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_USER, &CAutoToolTip::OnMessage)
END_MESSAGE_MAP()


// CAutoToolTip 메시지 처리기입니다.


int CAutoToolTip::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_NOACTIVATE;
//	return CDialogEx::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CAutoToolTip::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CString sText;
	CRect	rc;

	GetClientRect(&rc);
	GetWindowText(sText);
	dc.FillSolidRect(rc, RGB(0, 0, 0));
	rc.DeflateRect(1, 1, 1, 1);
	dc.FillSolidRect(rc, RGB(249, 254, 210));
	dc.DrawText(sText, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

int CAutoToolTip::GetWidth()
{
	CString	sText;
	long	nWidth;
	CDC*	pDC = GetDC();

	GetWindowText(sText);
	nWidth = pDC->GetTextExtent(sText).cx;
	ReleaseDC(pDC);
	return nWidth;

}


afx_msg LRESULT CAutoToolTip::OnMessage(WPARAM wParam, LPARAM lParam)
{
	m_parent->PostMessage(WM_KEYDOWN, wParam, 0);
	return 0;
}
