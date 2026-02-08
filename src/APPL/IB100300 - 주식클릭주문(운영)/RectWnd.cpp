// RectWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RectWnd.h"
#include "HogaWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRectWnd

CRectWnd::CRectWnd()
{
}

CRectWnd::CRectWnd(CWnd* pParent)
{
	m_pParent = pParent;
}

CRectWnd::~CRectWnd()
{
}

BEGIN_MESSAGE_MAP(CRectWnd, CWnd)
	//{{AFX_MSG_MAP(CRectWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRectWnd message handlers

void CRectWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect clientRt;
	GetClientRect(&clientRt);
	if (m_color == 1)
		dc.FillSolidRect(clientRt, RGB(0, 0, 255));
	else if (m_color == 2)
		dc.FillSolidRect(clientRt, RGB(255, 0, 0));
	else if (m_color == 3)
		dc.FillSolidRect(clientRt, RGB(200, 200, 200));
}
	
int CRectWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRgn	tmpRgn, wndRgn;
	CRect clientRt;
	GetClientRect(&clientRt);

	wndRgn.CreateRectRgn(clientRt.left, clientRt.top, clientRt.right, clientRt.bottom);
	tmpRgn.CreateRectRgn(clientRt.left + 2, clientRt.top + 2, clientRt.right - 2, clientRt.bottom - 2);
	wndRgn.CombineRgn(&wndRgn, &tmpRgn, RGN_XOR);
	SetWindowRgn(wndRgn, TRUE);		// 윈도우 영역을 wndRgn 으로 set
	tmpRgn.DeleteObject();
	wndRgn.DeleteObject();
	return 0;
}

void CRectWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	ClientToScreen(&pt);

	pt = ((CHogaWnd *)m_pParent)->GetRect2HogaPoint(pt);
	m_pParent->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));

	CWnd::OnLButtonDown(nFlags, point);
}

void CRectWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	ClientToScreen(&pt);

	pt = ((CHogaWnd *)m_pParent)->GetRect2HogaPoint(pt);
	m_pParent->SendMessage(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));

	CWnd::OnLButtonUp(nFlags, point);
}

void CRectWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	ClientToScreen(&pt);

	pt = ((CHogaWnd *)m_pParent)->GetRect2HogaPoint(pt);
	m_pParent->SendMessage(WM_LBUTTONDBLCLK, nFlags, MAKELPARAM(point.x, point.y));

	CWnd::OnLButtonDblClk(nFlags, point);
}
void CRectWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRgn	tmpRgn, wndRgn;
	CRect clientRt;
	GetClientRect(&clientRt);

	wndRgn.CreateRectRgn(clientRt.left, clientRt.top, clientRt.right, clientRt.bottom);
	//tmpRgn.CreateRectRgn(clientRt.left + 1, clientRt.top + 1, clientRt.right - 1, clientRt.bottom - 1);
	tmpRgn.CreateRectRgn(clientRt.left + 2, clientRt.top + 2, clientRt.right - 2, clientRt.bottom - 2);
	wndRgn.CombineRgn(&wndRgn, &tmpRgn, RGN_XOR);
	SetWindowRgn(wndRgn, TRUE);		// 윈도우 영역을 wndRgn 으로 set
	tmpRgn.DeleteObject();
	wndRgn.DeleteObject();
}
