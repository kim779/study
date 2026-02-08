// SizeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RECT_SIZE	22
#define RECT_FILL	20

const char* _tableCtrlClassName = "__axTableCtrlClass";

/////////////////////////////////////////////////////////////////////////////
// CAxMTableCtrl

CAxMTableCtrl::CAxMTableCtrl(int maxx, int maxy, int nX, int nY)
{
	n_nMessageId = AXM_TABLE_NOTIFY;
	m_nMaxX = maxx;
	m_nMaxY = maxy;
	m_nNumX = nX;
	m_nNumY = nY;
	m_nSelX = m_nSelY = 0;
	m_bInitOver = FALSE;
	m_bCaptured = FALSE;

	XRegisterWindowClass(_tableCtrlClassName, ::CreateSolidBrush(COLOR_BTNFACE));
}

CAxMTableCtrl::~CAxMTableCtrl()
{

}


BEGIN_MESSAGE_MAP(CAxMTableCtrl, CWnd)
	//{{AFX_MSG_MAP(CAxMTableCtrl)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAxMTableCtrl::Create(CWnd *pParent, int x, int y)
{
	ASSERT(!IsWindow(m_hWnd));

	LOGFONT lFont;
	::ZeroMemory(&lFont, sizeof(lFont));
	CDC *pDC = pParent->GetDC();
	if (pDC)
	{
		lFont.lfHeight = -MulDiv(9, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
		pParent->ReleaseDC(pDC);
	}
	else
		lFont.lfHeight = 14;
	
	strcpy(lFont.lfFaceName, "굴림");
	m_Font.CreateFontIndirect(&lFont);

	CSize cx;
	cx = CalcSize(m_nNumX, m_nNumY);
	m_pParent = pParent;
	m_nSelX = m_nSelY = 0;
	m_bCaptured = FALSE;
	m_bInitOver = FALSE;
	if (CWnd::CreateEx(0, _tableCtrlClassName, NULL, WS_VISIBLE | WS_POPUP, x, y, cx.cx, cx.cy, pParent->m_hWnd, NULL))
	{
		CRect cr;
		cr = CRect(0, 0, 0, 0);
		return m_WndStatic.Create("취 소", WS_CHILD | WS_VISIBLE | SS_CENTER, cr, this, 500);
	}
	else 
		return FALSE;
}

CSize CAxMTableCtrl::CalcSize(int nX, int nY)
{
	CSize cx;
	cx.cx = RECT_SIZE * nX + 4;	
	cx.cy = RECT_SIZE * nY + 4 + 20;	
	return cx;
}

/////////////////////////////////////////////////////////////////////////////
// CAxMTableCtrl message handlers

void CAxMTableCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE)
		CloseWindow();
}

int CAxMTableCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}


void CAxMTableCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	CloseWindow();
}


void CAxMTableCtrl::OnPaint() 
{
	static int oldsx=-1, oldsy=-1, oldnx=-1, oldny=-1;

	if (!m_bInitOver) 
	{
		oldsx = oldsy = oldnx = oldny=-1;
		m_WndStatic.SetFont(&m_Font);
		MoveStatic();
	}

	if (oldsx == m_nSelX && oldsy == m_nSelY && oldnx == m_nNumX && oldny == m_nNumY)
		return;

	CPaintDC dc(this); 
	CRect cr;
	GetClientRect(&cr);
	dc.Draw3dRect(&cr,::GetSysColor(COLOR_BTNSHADOW), RGB(0, 0, 0));
	cr.InflateRect(-1, - 1);

	dc.Draw3dRect(&cr,::GetSysColor(COLOR_BTNHILIGHT),::GetSysColor(COLOR_BTNSHADOW));

	int x, y;
	y = 2;

	for (int i = 0; i < m_nNumY; i++, y += RECT_SIZE)
	{
		x = 2;
		for (int j = 0; j < m_nNumX; j++, x += RECT_SIZE)
		{
			COLORREF color;
			cr = CRect(x, y, x + RECT_SIZE, y + RECT_SIZE);
			cr.InflateRect(-1, - 1);
			if (i < m_nSelY && j < m_nSelX) 
			{
				color = RGB(0, 0, 128);
			}
			else 
			{
				color = RGB(255, 255, 255);
			}
			dc.FillSolidRect(&cr, color);
			dc.Draw3dRect(&cr,::GetSysColor(COLOR_BTNSHADOW),::GetSysColor(COLOR_BTNSHADOW));
		}
	}

	oldsx = m_nSelX;
	oldsy = m_nSelY;
	oldnx = m_nNumX;
	oldny = m_nNumY;

	if (::IsWindow(m_WndStatic.m_hWnd))
	{
		m_WndStatic.GetWindowRect(&cr);
		ScreenToClient(&cr);
		cr.InflateRect(1, 1);
		dc.Draw3dRect(&cr,::GetSysColor(COLOR_BTNSHADOW),::GetSysColor(COLOR_BTNHILIGHT));
	}
}

void CAxMTableCtrl::DrawText(int x, int y)
{
	CString cs;	
	cs.Format("  %d x %d ", x, y);
	m_WndStatic.SetWindowText(cs);
}

void CAxMTableCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
}

void CAxMTableCtrl::PostNcDestroy() 
{
}

BOOL CAxMTableCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CRect cr;
	GetClientRect(&cr);
	pDC->FillSolidRect(&cr,::GetSysColor(COLOR_BTNFACE));
	return TRUE;
}

void CAxMTableCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	MoveStatic();
}

void CAxMTableCtrl::MoveStatic()
{
	CRect cr;
	if (!::IsWindow(m_WndStatic.m_hWnd))
		return;
	m_bInitOver = TRUE;
	GetClientRect(&cr);	
	m_Rect = cr;
	m_Rect.InflateRect(-2, - 2);
	cr.bottom -= 2;
	cr.top = cr.bottom - 16;
	cr.left +=2;
	cr.right -=2;
	m_WndStatic.SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CAxMTableCtrl::CloseWindow()
{
	//	if(GetCapture() == this) ReleaseCapture();
	PostMessage(WM_CLOSE);	
}

void CAxMTableCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_Rect.PtInRect(point))
		CloseWindow();
	else
	{
		SetCapture();
		m_bCaptured = TRUE;		
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CAxMTableCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CloseWindow();
	if (m_bCaptured)
	{
		ReleaseCapture();
		if (m_nSelX > 0 && m_nSelY > 0)
		{
			m_pParent->PostMessage(n_nMessageId, MAKEWPARAM(m_nSelX, m_nSelY));
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CAxMTableCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bCaptured)
	{
		if (!m_Rect.PtInRect(point)) 
		{
			m_nSelX = m_nSelY = 0;
			InvalidateRect(&m_Rect, FALSE);
			m_WndStatic.SetWindowText("취 소");
		}
		else
		{
			point.x -= m_Rect.left;
			point.y -= m_Rect.top;
			point.x /= RECT_SIZE;
			point.y /= RECT_SIZE;
			m_nSelX = point.x + 1;
			m_nSelY = point.y + 1;
			if (m_nSelX > m_nNumX)
				m_nSelX = m_nNumX;
			if (m_nSelY > m_nNumY)
				m_nSelY = m_nNumY;
			InvalidateRect(&m_Rect, FALSE);
			DrawText(m_nSelX, m_nSelY);
		}
	}
	else
	{
		if (!m_Rect.PtInRect(point)) 
		{
			if (point.x < m_Rect.left || point.y < m_Rect.top)
			{
				m_nSelX = m_nSelY = 0;
				InvalidateRect(&m_Rect, FALSE);
				m_WndStatic.SetWindowText("취 소");
			}
			else
			{
				int nx, ny;
				nx = m_nNumX;
				ny = m_nNumY;
				if (point.x > m_Rect.right && m_nNumX <= m_nMaxX - 1)
					nx++;
				if (point.y > m_Rect.bottom && m_nNumY <= m_nMaxY - 1)
					ny++;
				if (nx != m_nNumX || ny != m_nNumY)
				{
					m_nNumX = nx;
					m_nNumY = ny;
					CSize cx;
					cx = CalcSize(m_nNumX, m_nNumY);
					if (point.x > m_Rect.right)
						m_nSelX++;
					if (point.y > m_Rect.bottom)
						m_nSelY++;
					SetWindowPos(NULL, 0, 0, cx.cx, cx.cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
					if (m_nSelX > m_nNumX)
						m_nSelX = m_nNumX;
					if (m_nSelY > m_nNumY)
						m_nSelY = m_nNumY;
					InvalidateRect(&m_Rect, FALSE);
					UpdateWindow();
					DrawText(m_nSelX, m_nSelY);
				}
			}
		}
		else
		{
			point.x -= m_Rect.left;
			point.y -= m_Rect.top;
			point.x /= RECT_SIZE;
			point.y /= RECT_SIZE;
			m_nSelX = point.x + 1;
			m_nSelY = point.y + 1;
			if (m_nSelX > m_nNumX)
				m_nSelX = m_nNumX;
			if (m_nSelY > m_nNumY)
				m_nSelY = m_nNumY;
			InvalidateRect(&m_Rect, FALSE);
			DrawText(m_nSelX, m_nSelY);
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CAxMTableCtrl::OnDestroy()
{
	m_Font.DeleteObject();
}

