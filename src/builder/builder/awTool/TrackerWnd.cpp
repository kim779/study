
#include "stdafx.h"
#include "../ChildView.h"
#include "TrackerWnd.h"

CTrackerWnd::CTrackerWnd()
{
	m_view = NULL;
	m_bFst = false;
}

CTrackerWnd::CTrackerWnd(CChildView* view)
{
	m_view = view;
	m_bFst = false;
}

CTrackerWnd::~CTrackerWnd()
{

}

BEGIN_MESSAGE_MAP(CTrackerWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

int CTrackerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CTrackerWnd::SetFst(bool bFst)
{
	m_bFst = bFst;
}

void CTrackerWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CPen pen;
	if (m_bFst) pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 254));
	else pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 100));
	CPen* oldPen = dc.SelectObject(&pen);

	CBrush brush;
	if (m_bFst) brush.CreateSolidBrush(RGB(0, 0, 100));
	else brush.CreateSolidBrush(RGB(255, 255, 254));
	CBrush* oldBrush = dc.SelectObject(&brush);

	CRect rc;
	GetClientRect(rc);
	dc.Rectangle(rc);

	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);
}