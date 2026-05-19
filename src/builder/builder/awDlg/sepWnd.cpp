// sepWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "sepWnd.h"

// CSepWnd

IMPLEMENT_DYNAMIC(CSepWnd, CWnd)

CSepWnd::CSepWnd()
{

}

CSepWnd::~CSepWnd()
{
}


BEGIN_MESSAGE_MAP(CSepWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSepWnd 메시지 처리기입니다.


int CSepWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	setTP();	
	return 0;
}


void CSepWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	rt;

	GetClientRect(&rt);
	if (rt.Height() > rt.Width())
	{
		dc.FillSolidRect(2, 0, 2, rt.Height(), RGB(0, 0, 0));
		dc.FillSolidRect(0, 0, 6, 1, RGB(0, 0, 0));
		dc.FillSolidRect(1, 1, 4, 1, RGB(0, 0, 0));
		dc.FillSolidRect(0, rt.Height() - 1, 6, 1, RGB(0, 0, 0));
		dc.FillSolidRect(1, rt.Height() - 2, 4, 1, RGB(0, 0, 0));
	}
	else
	{
		dc.FillSolidRect(2, 2, rt.Width(), 2, RGB(0, 0, 0));
		dc.FillSolidRect(0, 0, 1, 6, RGB(0, 0, 0));
		dc.FillSolidRect(1, 1, 1, 4, RGB(0, 0, 0));
	}
}

void CSepWnd::MoveSWWnd(int x, int y)
{
	CRect	rt;

	GetClientRect(&rt);
	MoveWindow(x, y, rt.Width(), 6, TRUE);
	Invalidate();
}

void CSepWnd::MoveWnd(int x, int y)
{
	CRect	rt;

	GetClientRect(&rt);
	MoveWindow(x, y, 6, rt.Height(), TRUE);
	Invalidate();
}

void CSepWnd::setTP()
{
	CRgn	tmpRgn;
	CRect	rt;
	CDC*	pDC = GetDC();

	GetClientRect(rt);
	m_wndRgn.DeleteObject();
	m_wndRgn.CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);

	tmpRgn.CreateRectRgn(0, 2, 2, rt.Height() - 2);
	m_wndRgn.CombineRgn(&m_wndRgn, &tmpRgn, RGN_XOR);
	tmpRgn.DeleteObject();
	tmpRgn.CreateRectRgn(4, 2, 6, rt.Height() - 2);
	m_wndRgn.CombineRgn(&m_wndRgn, &tmpRgn, RGN_XOR);
	tmpRgn.DeleteObject();

	SetWindowRgn(m_wndRgn, TRUE);
	ReleaseDC(pDC);
}
