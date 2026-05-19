
#include "stdafx.h"
#include "../ChildView.h"
#include "SmartGLine.h"
#include "../h/color.H"

CSmartGLine::CSmartGLine()
{
}

CSmartGLine::CSmartGLine(CChildView* view)
{
	m_view = view;
}

CSmartGLine::~CSmartGLine()
{
}

BEGIN_MESSAGE_MAP(CSmartGLine, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CSmartGLine::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//m_backBrush.CreateSolidBrush(RGB(171, 171, 171));
	return 0;
}

void CSmartGLine::Init(int PenWidth, int PenStyle, int lineXY)
{
	m_PenWidth = PenWidth;
	m_PenStyle = PenStyle;
	m_lineXY = lineXY;
}

void CSmartGLine::SaveRect(CRect rc)
{
	m_rc.CopyRect(rc);
}

CRect CSmartGLine::LoadRect()
{
	return m_rc;
}

void CSmartGLine::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	//IntersectRect
	CRect rc;
	GetClientRect(rc);
	CPen pen;
	if (m_PenStyle == PS_USERSTYLE)
	{
		DWORD style[] = { 1, 1 };
		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID;
		lb.lbColor = LINE_COLOR;
		pen.CreatePen(PS_GEOMETRIC | PS_USERSTYLE | PS_ENDCAP_FLAT, m_PenWidth, &lb, 2, style);
	}
	else
		pen.CreatePen(m_PenStyle, m_PenWidth, RGB(0, 0, 0));

	//rc.InflateRect(m_rc.Width(), m_rc.Height());
	dc.FillSolidRect(rc, RGB(255, 255, 255));  //배경색(투명색) 칠함
	//rc.DeflateRect(m_rc.Width(), m_rc.Height());

	CPen* oldPen = dc.SelectObject(&pen);
	if (m_lineXY == LINE_X)
	{
		dc.MoveTo(rc.left, rc.top);
		dc.LineTo(rc.left, rc.bottom);
	}
	else
	{
		dc.MoveTo(rc.left, rc.top);
		dc.LineTo(rc.right, rc.top);
	}
	dc.SelectObject(oldPen);
	pen.DeleteObject();
}

void CSmartGLine::Redraw()
{
	Invalidate(FALSE);
}

BOOL CSmartGLine::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}
