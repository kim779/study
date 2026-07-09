// SepWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SepWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSepWnd

CSepWnd::CSepWnd()
{
}

CSepWnd::~CSepWnd()
{
}


BEGIN_MESSAGE_MAP(CSepWnd, CWnd)
	//{{AFX_MSG_MAP(CSepWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSepWnd message handlers

void CSepWnd::SetTP()
{
	CRgn	tmpRgn;
	CRect	rt;
	CDC *pDC = GetDC();

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

void CSepWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rt;
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
		//dc.FillSolidRect(rt.Width() - 1, 0, 1, 6, RGB(0, 0, 0));
		//dc.FillSolidRect(rt.Width() - 2, 1, 1, 4, RGB(0, 0, 0));
	}
	// Do not call CWnd::OnPaint() for painting messages
}

void CSepWnd::MoveWnd(int xpos, int ypos)
{
	CRect rt;
	GetClientRect(&rt);
	MoveWindow(xpos, ypos, 6, rt.Height(), TRUE);
	Invalidate();
}

void CSepWnd::MoveSWWnd(int xpos, int ypos)
{
	CRect rt;
	GetClientRect(&rt);
	MoveWindow(xpos, ypos, rt.Width(), 6, TRUE);
	Invalidate();
}

int CSepWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTP();	
	return 0;
}

void CSepWnd::SetSWTP()
{
	CRgn	tmpRgn;
	CRect	rt;
	CDC	*pDC = GetDC();

	GetClientRect(rt);
	
	m_wndRgn.DeleteObject();
	m_wndRgn.CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);

	tmpRgn.CreateRectRgn(2, 0, rt.Width(), 2);
	m_wndRgn.CombineRgn(&m_wndRgn, &tmpRgn, RGN_XOR);
	tmpRgn.DeleteObject();
	tmpRgn.CreateRectRgn(2, 4, rt.Width(), 6);
	m_wndRgn.CombineRgn(&m_wndRgn, &tmpRgn, RGN_XOR);
	tmpRgn.DeleteObject();
	tmpRgn.CreateRectRgn(0, 2, 2, 2);
	m_wndRgn.CombineRgn(&m_wndRgn, &tmpRgn, RGN_XOR);
	tmpRgn.DeleteObject();

	SetWindowRgn(m_wndRgn, TRUE);
	ReleaseDC(pDC);
}

