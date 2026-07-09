// tlSplit.cpp: implementation of the CtlSplit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlSplit.h"

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

CtlSplit::CtlSplit(CChildView* view)
	: CtlRect(view)
{
	m_name = _T("Split");
}

CtlSplit::~CtlSplit()
{
}

void CtlSplit::Draw(CDC* dc, int fN)
{
	CRect	fRc;
	CPoint	pts[6];
	CBrush*	brush;
	int	gap = 1;
	int	ndc = dc->SaveDC();

	CTOOL::Draw(dc, fN);
	fRc = getRect();

	pts[0].x = fRc.right;		pts[0].y = fRc.top;
	pts[1].x = fRc.left;		pts[1].y = fRc.top;
	pts[2].x = fRc.left;		pts[2].y = fRc.bottom;
	pts[3].x = fRc.left+gap;	pts[3].y = fRc.bottom-gap;
	pts[4].x = pts[3].x;		pts[4].y = fRc.top+gap;
	pts[5].x = fRc.right-gap;	pts[5].y = pts[4].y;
	setPen(dc, clWHITE);
	brush = setBrush(dc, clWHITE);
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0] = pts[5];
	pts[1] = pts[4];
	pts[2] = pts[3];
	pts[3].x = fRc.right-gap;	pts[3].y = pts[2].y;
	setPen(dc, clSPLIT);
	brush = setBrush(dc, clSPLIT);
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 4);

	pts[0].x = fRc.right;		pts[0].y = fRc.top;
	pts[1].x = fRc.right-gap;	pts[1].y = fRc.top+gap;
	pts[2].x = pts[1].x;		pts[2].y = fRc.bottom-gap;
	pts[3].x = fRc.left+gap;	pts[3].y = pts[2].y;
	pts[4].x = fRc.left;		pts[4].y = fRc.bottom;
	pts[5].x = fRc.right;		pts[5].y = fRc.bottom;
	setPen(dc, clBOTTOM);
	brush = setBrush(dc, clBOTTOM);
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	dc->RestoreDC(ndc);
}

void CtlSplit::drawBegin(CPoint point)
{
	m_fN    = m_view->m_selF;
	m_rect  = getRect();

	if (m_view->m_formR[m_view->m_selF].iok)	// vertical
		m_delta = m_rect.left - point.x;
	else						// horizontal
		m_delta = m_rect.top - point.y;

	m_dc->SelectStockObject(NULL_PEN);
	m_dc->SelectStockObject(DKGRAY_BRUSH);
	m_dc->SetROP2(R2_NOT);
	m_dc->Rectangle(m_rect);
}

void CtlSplit::drawTo(CPoint point)
{
	int	gap = 0;

	m_dc->Rectangle(m_rect);
	if (m_view->m_formR[m_view->m_selF].iok)	// vertical
	{
		gap = m_rect.Width();
		m_rect.left  = point.x + m_delta;
		m_rect.right = m_rect.left + gap;
	}
	else						// horizontal
	{
		gap = m_rect.Height();
		m_rect.top    = point.y + m_delta;
		m_rect.bottom = m_rect.top + gap;
	}

	m_dc->Rectangle(m_rect);
}

void CtlSplit::drawEnd()
{
	m_dc->Rectangle(m_rect);
	if (m_view->m_formR[m_view->m_selF].iok)	// vertical
		m_delta = m_endpt.x - m_lastpt.x;
	else						// horizontal
		m_delta = m_endpt.y - m_lastpt.y;

	m_formR[m_fN].left   = m_rect.left;
	m_formR[m_fN].top    = m_rect.top;
	m_formR[m_fN].right  = m_rect.right;
	m_formR[m_fN].bottom = m_rect.bottom;
}
