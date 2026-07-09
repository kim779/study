// tlImage.cpp: implementation of the CtlImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlImage.h"

#include "../childview.h"
#include "../mapform.h"
#include "../resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtlImage::CtlImage(CChildView* view, UINT id)
	: CtlRect(view)
{
	switch (id)
	{
	case ID_TOOL_AVI:
		m_type = imAVI;
		m_name = _T("Avi");
		break;
	default:
		m_type = imNONE;
		break;
	}
}

CtlImage::~CtlImage()
{
}

void CtlImage::drawEnd()
{
	CtlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		m_rect.right  = m_rect.left + getDefaultWidth("AVI", 105);
		m_rect.bottom = m_rect.top  + getDefaultHeight("AVI", 105);
	}	

	CRect	wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	m_pFormItem->getFormPoint(m_fN)->left   = m_rect.left;
	m_pFormItem->getFormPoint(m_fN)->top    = m_rect.top;
	m_pFormItem->getFormPoint(m_fN)->right  = m_rect.right;
	m_pFormItem->getFormPoint(m_fN)->bottom = m_rect.bottom;

	switch (m_type) 
	{
	case imAVI:
		m_pFormItem->getFormPoint(m_fN)->kind = FM_AVI;
		break;
	
	default: 
		break;
	}

	m_dc->Rectangle(m_rect);
}

void CtlImage::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	fRc;
	int	ndc = dc->SaveDC();

	CTOOL::Draw(dc, fN);
	fRc = getRect();
	switch (m_type)
	{
	case imAVI:
		brush = setBrush(dc, clDGRAY);
		dc->FillRect(fRc, brush);
		dc->SetBkMode(TRANSPARENT);
		setTextColor(dc, clWHITE);
		dc->DrawText("A V I", fRc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		break;

	default:
		break;
	}

	dc->RestoreDC(ndc);
}