
#include "stdafx.h"
#include "../ChildView.h"
#include "PointerWnd.h"

CPointerWnd::CPointerWnd()
{
}

CPointerWnd::CPointerWnd(CChildView* view)
{
	m_view = view;
}

CPointerWnd::~CPointerWnd()
{
}

BEGIN_MESSAGE_MAP(CPointerWnd, CWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CPointerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//m_backBrush.CreateSolidBrush(RGB(171, 171, 171));
	return 0;
}

void CPointerWnd::Init(int PenWidth, int PenStyle)
{
	m_PenWidth = PenWidth;
	m_PenStyle = PenStyle;
}

void CPointerWnd::SaveRect(CRect rc)
{
	m_rc.CopyRect(rc);
}

CRect CPointerWnd::LoadRect()
{
	return m_rc;
}

void CPointerWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	//IntersectRect
	bool bAreaOverX = false;
	bool bAreaOverY = false;
	bool bAreaUnderX = false;
	bool bAreaUnderY = false;
	CRect rc, viewRc;
	GetClientRect(rc);

	m_view->GetClientRect(viewRc);
	ClientToScreen(rc);
	m_view->ScreenToClient(rc);

	if(viewRc.right < rc.right)				//x좌표 view 영역 벗어났을때 처리
	{
		bAreaOverX = true;
		rc.right = viewRc.right;
	}
	else if(rc.left < 0)
	{
		bAreaUnderX = true;
		rc.left = 0;
	}
	if(viewRc.bottom < rc.bottom) 			//y좌표 view 영역 벗어났을때 처리
	{
		bAreaOverY = true;
		rc.bottom = viewRc.bottom;
	}
	else if(rc.top < 0)
	{
		bAreaUnderY = true;
		rc.top = 0;
	}
	if (rc.IntersectRect(rc, viewRc) == 0)
	{
		rc.bottom = m_rc.Height();
		rc.right = m_rc.Width();
		dc.FillSolidRect(rc, RGB(255, 255, 255));  //배경색(투명색) 칠함
		return;
	}
	else
	{
		m_view->ClientToScreen(rc);
		ScreenToClient(rc);

		if (!bAreaUnderX)						//펜 두께가 있을 경우 펜의 두께가 영역을 벗어나기 때문에 보정
			rc.left += m_PenWidth / 2;
		if (!bAreaUnderY)
			rc.top += m_PenWidth / 2;
		rc.InflateRect(m_rc.Width(), m_rc.Height());
		dc.FillSolidRect(rc, RGB(255, 255, 255));  //배경색(투명색) 칠함
		rc.DeflateRect(m_rc.Width(), m_rc.Height());
		rc.right--;								//좌표 보정
		rc.bottom--;

		CPen pen;
		if (m_PenStyle == PS_USERSTYLE)
		{
			DWORD style[] = { 1, 1 };
			LOGBRUSH lb;
			lb.lbStyle = BS_SOLID;
			lb.lbColor = RGB(0, 0, 0);
			pen.CreatePen(PS_GEOMETRIC | PS_USERSTYLE | PS_ENDCAP_FLAT, m_PenWidth, &lb, 2, style);
		}
		else
			pen.CreatePen(m_PenStyle, m_PenWidth, RGB(0, 0, 0));

		CPen* oldPen = dc.SelectObject(&pen);
		if (!bAreaUnderY)
		{
			dc.MoveTo(rc.left, rc.top);
			dc.LineTo(rc.right, rc.top);
			//dc.MoveTo(rc.right, rc.top);		//펜의 두께가 있을 경우 라인의 시작부분 재대로 안그려저서 반대로 한번 더 그림
			//dc.LineTo(rc.left, rc.top);
		}
		if (!bAreaOverX)
		{
			dc.MoveTo(rc.right, rc.top);
			dc.LineTo(rc.right, rc.bottom);
			//dc.MoveTo(rc.right, rc.bottom);
			//dc.LineTo(rc.right, rc.top);
		}

		if (!bAreaOverY)
		{
			dc.MoveTo(rc.right, rc.bottom);
			dc.LineTo(rc.left, rc.bottom);
			//dc.MoveTo(rc.left, rc.bottom);
			//dc.LineTo(rc.right, rc.bottom);
		}

		if (!bAreaUnderX)
		{
			dc.MoveTo(rc.left, rc.bottom);
			dc.LineTo(rc.left, rc.top - 1);
			//dc.MoveTo(rc.left, rc.top - 1);
			//dc.LineTo(rc.left, rc.bottom);
		}

		dc.SelectObject(oldPen);

		pen.DeleteObject();
	}
}

void CPointerWnd::Redraw()
{
	Invalidate(FALSE);
}

BOOL CPointerWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}
