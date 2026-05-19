// tlRegion.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlRegion.h"

#include "../resource.h"
#include "../amCom/formItem.h"

// CTlRegion

CTlRegion::CTlRegion(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type)
	: CTlRect(view, draw, formItem)
{
	switch (type)
	{
	case ID_TOOL_OBJECT:
		m_type = rnOBJECT;
		m_name = _T("Object");
		break;

	case ID_TOOL_BROWSER:
		m_type = rnBROWSER;
		m_name = _T("Browser");
		break;
		
	default:
		m_type = rnNONE;
		break;
	}
}

CTlRegion::~CTlRegion()
{
}


void CTlRegion::Draw(CDC* dc, int fN)
{
	CBrush*	brush = NULL;
	CRect	fRc;
	int	ndc = dc->SaveDC();
	CString	str;

	CTlBase::Draw(dc, fN);
	fRc = getRect();
	switch (m_type)
	{
	case rnOBJECT:
		setTextColor(dc, clBLACK);
		brush = setBrush(dc, clGREEN);
		str = m_formItem->GetFormStr(fN, _T("str"));
		if (str.IsEmpty())
			str = _T("Form");
		break;

	case rnBROWSER:
		setTextColor(dc, clBLACK);
		brush = setBrush(dc, clWHITE);
		str = _T("Browser");
		break;
	}
	dc->FillRect(fRc, brush);
	dc->SetBkMode(TRANSPARENT);
	dc->DrawText(str, fRc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	dc->RestoreDC(ndc);
}

void CTlRegion::drawEnd()
{
	CTlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		m_rect.right  = m_rect.left + 105;
		m_rect.bottom = m_rect.top  + 105;
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

	switch (m_type)
	{
	case rnOBJECT:
		formR->kind = FM_OBJECT;
		break;

	case rnBROWSER:
		formR->kind = FM_BROWSER;
		formR->properties |= PR_CHECKED;
		break;

	default:
		break;
	}

	m_dc->Rectangle(m_rect);
}
