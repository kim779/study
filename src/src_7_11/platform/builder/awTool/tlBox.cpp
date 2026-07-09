// tlBox.cpp: implementation of the CtlBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlBox.h"

#include "../childview.h"
#include "../mapform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtlBox::CtlBox(CChildView* view)
	: CtlRect(view)
{
	m_name = _T("Box");
}

CtlBox::~CtlBox()
{
}

void CtlBox::drawEnd()
{
	CtlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		m_rect.right  = m_rect.left + getDefaultWidth("BOX", 120);
		m_rect.bottom = m_rect.top  + getDefaultHeight("BOX", 50);
	}

	CRect	wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	m_pFormItem->getFormPoint(m_fN)->left   = m_rect.left;
	m_pFormItem->getFormPoint(m_fN)->top    = m_rect.top;
	m_pFormItem->getFormPoint(m_fN)->right  = m_rect.right;
	m_pFormItem->getFormPoint(m_fN)->bottom = m_rect.bottom;

	m_pFormItem->getFormPoint(m_fN)->kind = FM_BOX;
	m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("BOX", 232);
	m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("BOX", 232);
	m_pFormItem->getFormPoint(m_fN)->size = 1;

	m_dc->Rectangle(m_rect);
}

void CtlBox::Draw(CDC* dc, int fN)
{
	int	ndc = dc->SaveDC();
	CRect	fRc;

	CTOOL::Draw(dc, fN);

	fRc = getRect();
	if (m_pFormItem->getFormPoint(fN)->type == BX_BORDER)
	{
		if (!(m_pFormItem->getFormPoint(fN)->size < 3 || fRc.Width() < 6 || fRc.Height() < 6))
		{
			draw3DBox(dc);
			dc->RestoreDC(ndc);
			return;
		}
	}
	if (m_pFormItem->getFormPoint(fN)->type == BX_2CLR)
	{
		draw2CBox(dc);
		dc->RestoreDC(ndc);
		return;
	}

	m_pFormItem->getFormPoint(fN)->size = __max(m_pFormItem->getFormPoint(fN)->size, 1);
	int	dx = m_pFormItem->getFormPoint(m_fN)->size / 2;
	setPen(dc, m_pFormItem->getFormPoint(m_fN)->size);
	dc->SelectStockObject(NULL_BRUSH);
	fRc.left   += dx;
	fRc.top    += dx;
	fRc.right  -= dx;
	fRc.bottom -= dx;
	dc->Rectangle(fRc);

	dc->RestoreDC(ndc);
}

void CtlBox::draw3DBox(CDC* dc)
{
	CPoint	pts[6];
	CBrush*	brush;
	CRect	tRc = getRect();
	int	ww  = m_pFormItem->getFormPoint(m_fN)->size;
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

void CtlBox::draw2CBox(CDC *dc)
{
	CRect	tRc, rc = getRect();
	int	ww  = m_pFormItem->getFormPoint(m_fN)->size;
	
	// top
	tRc.SetRect(rc.left, rc.top, rc.right, rc.top + ww);
	dc->FillSolidRect(tRc, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));

	// bottom
	tRc.SetRect(rc.left, rc.bottom, rc.right, rc.bottom - ww);
	dc->FillSolidRect(tRc, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));

	// left
	tRc.SetRect(rc.left, rc.top + ww, rc.left + ww, rc.bottom - ww);
	dc->FillSolidRect(tRc, getRGB(m_pFormItem->getFormPoint(m_fN)->tRGB));

	// right
	tRc.SetRect(rc.right, rc.top + ww, rc.right - ww, rc.bottom - ww);
	dc->FillSolidRect(tRc, getRGB(m_pFormItem->getFormPoint(m_fN)->tRGB));
}
