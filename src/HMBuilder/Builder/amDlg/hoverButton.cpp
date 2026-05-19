// hoverButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "hoverButton.h"


// CHoverButton

IMPLEMENT_DYNAMIC(CHoverButton, CBitmapButton)

CHoverButton::CHoverButton()
{
	m_tracking = FALSE;
	m_hover    = FALSE;
}

CHoverButton::~CHoverButton()
{
}


BEGIN_MESSAGE_MAP(CHoverButton, CBitmapButton)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CHoverButton 메시지 처리기입니다.


void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS != NULL);
	ASSERT(m_bitmap.m_hObject != NULL);     // required

	CBitmap* pBitmap = &m_bitmap;
	if ((lpDIS->itemState & ODS_SELECTED) && m_bitmapSel.m_hObject != NULL)
		pBitmap = &m_bitmapSel;
	else if ((m_hover) && m_bitmapFocus.m_hObject != NULL)
		pBitmap = &m_bitmapFocus;   // third image for focused
	else if ((lpDIS->itemState & ODS_DISABLED) && m_bitmapDisabled.m_hObject != NULL)
		pBitmap = &m_bitmapDisabled;   // last image for disabled

	CDC	memDC, *pDC = CDC::FromHandle(lpDIS->hDC);
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOld = memDC.SelectObject(pBitmap);
	if (pOld == NULL)
		return;     // destructors will clean up

	CRect	rect;

	rect.CopyRect(&lpDIS->rcItem);
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOld);
}


void CHoverButton::OnMouseHover(UINT nFlags, CPoint point)
{
	m_hover=TRUE;
	Invalidate();
	CBitmapButton::OnMouseHover(nFlags, point);
}


void CHoverButton::OnMouseLeave()
{
	m_tracking = FALSE;
	m_hover    = FALSE;
	Invalidate();
	CBitmapButton::OnMouseLeave();
}


void CHoverButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_tracking)
	{
		TRACKMOUSEEVENT	tme;

		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_tracking = _TrackMouseEvent(&tme);
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}
