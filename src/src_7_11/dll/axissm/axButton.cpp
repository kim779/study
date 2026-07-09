// axButton.cpp : implementation file
//

#include "stdafx.h"
#include "axButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxButton

CAxButton::CAxButton(int buttonID, bool repeatLBDN)
{
	m_mouseDN = false;
	m_flate = true;
	m_hover = true;

	m_buttonID   = buttonID;
	m_repeatLBDN = repeatLBDN;

	m_bkGround = ::GetSysColor(COLOR_BTNFACE);
}

CAxButton::~CAxButton()
{
}


BEGIN_MESSAGE_MAP(CAxButton, CButton)
	//{{AFX_MSG_MAP(CAxButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxButton message handlers

void CAxButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_hover)
		SetTimer (1, 10, NULL);

	CButton::OnMouseMove(nFlags, point);
}

void CAxButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_mouseDN = true;
	CButton::OnLButtonDown(nFlags, point);

	if (m_repeatLBDN)
	{
		HWND hWnd = GetSafeHwnd();
		HWND hWndParent = GetParent()->GetSafeHwnd();

		MSG msg;
		while (!PeekMessage(&msg, hWnd, WM_LBUTTONUP, WM_LBUTTONUP, PM_REMOVE))
		{
			Sleep(100); ::SendMessage(hWndParent, WM_USER, MAKEWPARAM(m_buttonID, 0), 0);
		}

		SendMessage(WM_LBUTTONUP);
	}
}

void CAxButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_mouseDN = false;
	CButton::OnLButtonUp(nFlags, point);
}

void CAxButton::OnTimer(UINT_PTR nIDEvent) 
{
	CRect	btnRC;
	GetWindowRect(&btnRC);

	CPoint	curPT;
	GetCursorPos(&curPT);

	static bool drawing = false;

	if (m_mouseDN || !btnRC.PtInRect(curPT))
	{
		KillTimer (1);
		if (drawing) InvalidateRect (NULL);
		drawing = false;
		return;
	}

	if (m_flate)
	{
		CDC *pDC = GetDC();
		if (!drawing)
		{
			GetClientRect(&btnRC);
			pDC->FillSolidRect(btnRC, m_bkGround);

			if (GetIcon())
			{
				setIconRect();
//				DrawIconEx(pDC->GetSafeHdc(), m_rcIcon.left,
//					m_rcIcon.top, GetIcon(),
//					m_rcIcon.Width(), m_rcIcon.Height(),
//					NULL, (HBRUSH)NULL, DI_NORMAL); 
				DrawIconEx (pDC->GetSafeHdc(), m_rcIcon.left,
						m_rcIcon.top, GetIcon(), m_szIcon.cx,
						m_szIcon.cy, NULL, (HBRUSH)NULL, DI_NORMAL);

			}
			
			// draw the button rect.
			pDC->Draw3dRect(btnRC, GetSysColor(COLOR_BTNHIGHLIGHT), GetSysColor(COLOR_BTNSHADOW));
			drawing = true;
		}

		ReleaseDC (pDC);
	}

	CButton::OnTimer(nIDEvent);
}

void CAxButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);

	CDC* pDC = GetDC();
	UINT itemS = lpDrawItemStruct->itemState;

	// copy the rect, and fill the background.
	m_rcButton.CopyRect(&lpDrawItemStruct->rcItem);	
	pDC->FillSolidRect(m_rcButton, m_bkGround);

	// define rect to be used for left button down.
	setIconRect();

	if (m_flate)
	{
		if (itemS & ODS_SELECTED)
			pDC->Draw3dRect(m_rcButton, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
		else
			pDC->Draw3dRect(m_rcButton, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	}
	else
	{
		CRect rc(m_rcButton);
		rc.DeflateRect(1,1);
		
		if (itemS & ODS_SELECTED)
		{	// Draw button pressed.
			pDC->Draw3dRect(m_rcButton, GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
			pDC->Draw3dRect(rc, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNFACE));
		}
		else
		{	// Draw button raised.
			pDC->Draw3dRect(m_rcButton, GetSysColor(COLOR_BTNHIGHLIGHT), GetSysColor(COLOR_3DDKSHADOW));
			pDC->Draw3dRect(rc, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNSHADOW));
		}
	}

	// Save the item state, set background to transparent.
	pDC->SetBkMode(TRANSPARENT);

	// if an icon is associated with button, draw it.
	if(GetIcon())
	{
//		DrawIconEx (pDC->GetSafeHdc(), m_rcIcon.left,
//				m_rcIcon.top, GetIcon(), m_rcIcon.Width(),
//				m_rcIcon.Height(), NULL, (HBRUSH)NULL, DI_NORMAL);
		DrawIconEx (pDC->GetSafeHdc(), m_rcIcon.left,
				m_rcIcon.top, GetIcon(), m_szIcon.cx,
				m_szIcon.cy, NULL, (HBRUSH)NULL, DI_NORMAL);
	}

	ReleaseDC (pDC);
}

void CAxButton::SetIcon(HICON hIcon, CSize cSize)
{
	CButton::SetIcon(hIcon);

	m_szIcon.cx = cSize.cx;
	m_szIcon.cy = cSize.cy;
}

void CAxButton::setIconRect()
{
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);

	CPoint cPos;
	GetCursorPos(&cPos);

	CPoint ptCenter = rcClient.CenterPoint();

	int x1 = (rcClient.Width() - ptCenter.x) / 2 - 1;
	int x2 = x1 + rcClient.Width() - 1;
	int y1 = (rcClient.Height()-ptCenter.y) / 2 - 1;
	int y2 = y1 + rcClient.Height() - 1;

	m_rcIcon.CopyRect(rcClient);
	if (m_mouseDN)
	{
		if (rcWindow.PtInRect(cPos))
			m_rcIcon.OffsetRect(1,1);
	}
}

