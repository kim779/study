// tlSheet.cpp: implementation of the CtlSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlSheet.h"

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

CtlSheet::CtlSheet(CChildView* view)
	: CtlRect(view)
{
	m_name = _T("Sheet");
}

CtlSheet::~CtlSheet()
{
}

void CtlSheet::drawEnd()
{
	CtlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
		m_rect.right  = m_rect.left + getDefaultWidth("SHEET", 100);

	m_rect.bottom = m_rect.top + getDefaultHeight("SHEET", m_fheight + 10);

	CRect   wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	m_pFormItem->getFormPoint(m_fN)->left   = m_rect.left;
	m_pFormItem->getFormPoint(m_fN)->top    = m_rect.top;
	m_pFormItem->getFormPoint(m_fN)->right  = m_rect.right;
	m_pFormItem->getFormPoint(m_fN)->bottom = m_rect.bottom;

	m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("SHEET", basPRGB);
	m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("SHEET", 232);

	m_pFormItem->getFormPoint(m_fN)->kind  = FM_SHEET;
	m_pFormItem->getFormPoint(m_fN)->attr |= FA_SEND;

	m_dc->Rectangle(m_rect);
}

void CtlSheet::Draw(CDC* dc, int fN)
{
	CBrush* brush;
	CRect   fRc;
	int     ndc = dc->SaveDC();

	CTOOL::Draw(dc, fN);
	fRc = getRect();
	setFont(dc);
	setTextColor(dc);
	brush = setBrush(dc);
	setBkColor(dc);
	dc->FillRect(fRc, brush);
	dc->DrawText(m_name, fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

	setPen(dc, clBLACK);
	dc->SelectStockObject(NULL_BRUSH);
	dc->Rectangle(fRc);

	dc->RestoreDC(ndc);
}
