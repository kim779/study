// tlBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlBox.h"

#include "../amCom/formItem.h"

// CTlBox

CTlBox::CTlBox(CChildView* view, CAmDraw* draw, CFormItem* formItem)
	: CTlRect(view, draw, formItem)
{
	m_name = _T("Box");
}

CTlBox::~CTlBox()
{
}


void CTlBox::Draw(CDC* dc, int fN)
{
	int	dx, ndc = dc->SaveDC();
	CRect	fRc;
	struct	_formR*	formR = m_formItem->GetForm(fN);

	CTlBase::Draw(dc, fN);

	fRc = getRect();

	formR->size = __max(formR->size, 1);
	dx = formR->size / 2;
	fRc.DeflateRect(dx, dx);
	setPen(dc, formR->size);
	dc->SelectStockObject(NULL_BRUSH);

	if (fRc.Width() == 0)
	{
		dc->MoveTo(fRc.left, fRc.top);
		dc->LineTo(fRc.left, fRc.bottom);
	}
	else if (fRc.Height() == 0)
	{
		dc->MoveTo(fRc.left, fRc.top);
		dc->LineTo(fRc.right, fRc.top);
	}
	else
	{
		if (fRc.left == fRc.right)
			fRc.right = fRc.left + 1;
		if (fRc.top == fRc.bottom)
			fRc.bottom = fRc.top + 1;

		dc->Rectangle(fRc);
	}
	dc->RestoreDC(ndc);
}

void CTlBox::drawEnd()
{
	CTlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		m_rect.right  = m_rect.left + getDefaultWidth(_T("BOX"), 120);
		m_rect.bottom = m_rect.top  + getDefaultHeight(_T("BOX"), 50);
	}

	CRect	wRc;
	struct	_formR*	formR = m_formItem->GetForm(m_fN);

	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	formR->left   = m_rect.left;
	formR->top    = m_rect.top;
	formR->right  = m_rect.right;
	formR->bottom = m_rect.bottom;

	formR->kind = FM_BOX;
	formR->pRGB = getDefaultPRGB(_T("BOX"), 232);
	formR->tRGB = getDefaultTRGB(_T("BOX"), 232);
	formR->alpha = 100;			// alpha default setting
	formR->size = 1;

	m_dc->Rectangle(m_rect);
}

void CTlBox::draw3DBox(CDC* dc, int fN)
{
	CPoint	pts[6];
	CBrush*	brush;
	CRect	tRc = getRect();
	int	ww  = m_formItem->GetForm(fN)->size;
	int	gap = 1;
	
	pts[0].x = tRc.right-gap;	pts[0].y = tRc.top;
	pts[1].x = tRc.left;		pts[1].y = tRc.top;
	pts[2].x = tRc.left;		pts[2].y = tRc.bottom-gap;
	pts[3].x = tRc.left+gap;	pts[3].y = pts[2].y;
	pts[4].x = pts[3].x;		pts[4].y = tRc.top+gap;
	pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
	setPen(dc, clYWHITE);
	brush = setBrush(dc, clYWHITE);
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0] = pts[5];
	pts[1] = pts[4];
	pts[2] = pts[3];
	pts[3].x = tRc.left+ww-gap;	pts[3].y = pts[2].y;
	pts[4].x = pts[3].x;		pts[4].y = tRc.top+ww-gap;
	pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
	setPen(dc);
	brush = setBrush(dc);
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = tRc.right-ww+gap;	pts[0].y = pts[5].y;
	pts[1].x = pts[0].x;		pts[1].y = tRc.bottom-ww+gap;
	pts[2].x = tRc.left+ww-gap;	pts[2].y = pts[1].y;
	pts[3].x = pts[2].x;		pts[3].y = tRc.bottom-gap;
	pts[4].x = tRc.right-gap;	pts[4].y = pts[3].y;
	pts[5].x = pts[4].x;		pts[5].y = pts[0].y;
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = tRc.right;		pts[0].y = tRc.top;
	pts[1].x = pts[0].x;		pts[1].y = tRc.bottom;
	pts[2].x = tRc.left;		pts[2].y = pts[1].y;
	pts[3].x = tRc.left;		pts[3].y = tRc.bottom;
	pts[4].x = tRc.right;		pts[4].y = tRc.bottom;
	pts[5].x = tRc.right;		pts[5].y = tRc.top;
	setPen(dc, clBOTTOM);
	brush = setBrush(dc, clBOTTOM);
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = tRc.right-ww+gap;	pts[0].y = tRc.top+ww-gap;
	pts[1].x = tRc.left+ww-gap;	pts[1].y = pts[0].y;
	pts[2].x = pts[1].x;		pts[2].y = tRc.bottom-ww+gap;
	pts[3].x = tRc.left+ww-gap;	pts[3].y = pts[2].y;
	pts[4].x = pts[3].x;		pts[4].y = tRc.top+ww-gap;
	pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = tRc.right-ww+gap;
	pts[1].x = pts[0].x;		pts[1].y = tRc.bottom-ww+gap;
	pts[2].x = tRc.left+ww-gap;	pts[2].y = pts[1].y;
	pts[3].x = pts[2].x;		pts[3].y = tRc.bottom-ww+gap;
	pts[4].x = tRc.right-ww+gap;	pts[4].y = pts[3].y;
	pts[5].x = pts[4].x;		pts[5].y = pts[0].y;
	setPen(dc, clYWHITE);
	brush = setBrush(dc, clYWHITE);
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);
}

void CTlBox::draw2CBox(CDC *dc, int fN)
{
	CRect	tRc, rc = getRect();
	struct	_formR* formR = m_formItem->GetForm(fN);
	int	ww  = formR->size;
	
	// top
	tRc.SetRect(rc.left, rc.top, rc.right, rc.top + ww);
	dc->FillSolidRect(tRc, getRGB(formR->pRGB));

	// bottom
	tRc.SetRect(rc.left, rc.bottom, rc.right, rc.bottom - ww);
	dc->FillSolidRect(tRc, getRGB(formR->pRGB));

	// left
	tRc.SetRect(rc.left, rc.top + ww, rc.left + ww, rc.bottom - ww);
	dc->FillSolidRect(tRc, getRGB(formR->tRGB));

	// right
	tRc.SetRect(rc.right, rc.top + ww, rc.right - ww, rc.bottom - ww);
	dc->FillSolidRect(tRc, getRGB(formR->tRGB));
}