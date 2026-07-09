// tlRegion.cpp: implementation of the CtlRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlRegion.h"

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

CtlRegion::CtlRegion(CChildView* view, UINT id)
	: CtlRect(view)
{
	switch (id)
	{
	case ID_TOOL_GRAPH:
		m_type = rnGRAPH;
		m_name = _T("Graph");
		break;

	case ID_TOOL_OBJECT:
		m_type = rnOBJECT;
		m_name = _T("Object");
		break;

	case ID_TOOL_BROWSER:
		m_type = rnBROWSER;
		m_name = _T("Browser");
		break;

	case ID_TOOL_CONTROL:
		m_type = rnCONTROL;
		m_name = _T("Control");
		break;

	default:
		m_type = rnNONE;
		break;
	}
}

CtlRegion::~CtlRegion()
{
}

void CtlRegion::drawEnd()
{
	CtlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		m_rect.right  = m_rect.left + 105;
		m_rect.bottom = m_rect.top  + 105;
	}

	CRect	wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	struct _formR *formR = m_pFormItem->getFormPoint(m_fN);

	formR->left   = m_rect.left;
	formR->top    = m_rect.top;
	formR->right  = m_rect.right;
	formR->bottom = m_rect.bottom;

	switch (m_type)
	{
	case rnGRAPH:
		formR->kind = FM_GRAPH;
		formR->pRGB = 232;
		formR->tRGB = basINPUT;
		break;

	case rnOBJECT:
		formR->kind = FM_OBJECT;
		break;

	case rnBROWSER:
		formR->kind = FM_BROWSER;
		formR->properties |= PR_CHECKED;
		break;

	case rnCONTROL:
		formR->kind = FM_CONTROL;
		break;

	default:
		break;
	}

	m_dc->Rectangle(m_rect);
}

void CtlRegion::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	fRc;
	int	ndc = dc->SaveDC();
	CString	str = _T("");

	CTOOL::Draw(dc, fN);
	fRc = getRect();
	switch (m_type)
	{
	case rnGRAPH:
		setFont(dc);
		setTextColor(dc);
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
			dc->SetBkMode(TRANSPARENT);
		else
		{
			brush = setBrush(dc);
			setBkColor(dc);
			dc->FillRect(fRc, brush);
		}
		str = _T("GRAPH");
		dc->DrawText(str, fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		break;

	case rnOBJECT:
	case rnBROWSER:
		switch (m_type)
		{
		case rnOBJECT:
			setTextColor(dc, clBLACK);
			brush = setBrush(dc, clGREEN);
			if (!m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
				str = m_pFormItem->getFormStr(m_fN, "str");
			else
				str = _T("OBJECT");
			break;

		case rnBROWSER:
			setTextColor(dc, clBLACK);
			brush = setBrush(dc, clWHITE);
			str = _T("BROWSER");
			break;
		}
		dc->FillRect(fRc, brush);
		dc->SetBkMode(TRANSPARENT);
		dc->DrawText(str, fRc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		break;
	case rnCONTROL:
		setTextColor(dc, clBLACK);
		brush = setBrush(dc, clWHITE);

		if (!m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
			str = m_pFormItem->getFormStr(m_fN, "str");
		else
			str = _T("CONTROL");

		dc->FillRect(fRc, brush);
		dc->SetBkMode(TRANSPARENT);
		dc->DrawText(str, fRc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		break;
	default:
		break;
	}

	dc->RestoreDC(ndc);
}