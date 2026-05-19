// tlText.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlText.h"

#include "../resource.h"
#include "../amCom/formItem.h"

// CTlText

CTlText::CTlText(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type)
	: CTlRect(view, draw, formItem)
{
	switch (type)
	{
	case ID_TOOL_LABEL:
		m_type = txLABEL;
		m_name = _T("Label");
		break;
	default:
		m_type = txNONE;
		break;
	}
}

CTlText::~CTlText()
{
}

void CTlText::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	tRc,  fRc;
	int	ndc = dc->SaveDC();
	UINT	style;
	CSize	size;
	CString	str = _T("");
	struct	_formR*	formR = m_formItem->GetForm(fN);

	CTlBase::Draw(dc, fN);
	fRc = getRect();
	switch (m_type)
	{
	case txLABEL:
		setFont(dc);
		setTextColor(dc);
		brush = setBrush(dc);
		setBkColor(dc);
		dc->FillRect(fRc, brush);

		switch (formR->alignment)
		{
		case AL_RIGHT:
			style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;	break;
		case AL_CENTER:
			style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;	break;
		case AL_LEFT:
		default:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;	break;
		}

		switch (formR->borders)
		{
		case BD_RAISED:
			drawEdge(dc, fRc, EDGE_RAISED);
			break;
		case BD_SUNKEN:
			drawEdge(dc, fRc, EDGE_SUNKEN);
			break;
		case BD_BUMP:
			drawEdge(dc, fRc, EDGE_BUMP);
			break;
		case BD_ETCHED:
			drawEdge(dc, fRc, EDGE_ETCHED);
			break;
		case BD_LINE:
			setPen(dc, getRGB(formR->bRGB));
			dc->SelectStockObject(NULL_BRUSH);
			dc->Rectangle(fRc);
			break;
		default:
			break;
		}

		str = m_formItem->GetFormStr(fN, _T("str"));
		editAmpersand(str);
		if (str.Replace(_T("\\n"), _T("\n")))
		{
			style &= ~DT_SINGLELINE;
			style |= DT_EXTERNALLEADING;
		}

		if (!m_formItem->GetFormStr(fN, _T("str2")).IsEmpty())
		{
			drawPNG(dc, fRc, m_formItem->GetFormStr(fN, _T("str2")));\
			dc->SetBkMode(TRANSPARENT);
		}

		dc->DrawText(str, fRc, style);
		break;

	default:
		break;
	}
	dc->RestoreDC(ndc);
}

void CTlText::drawEnd()
{
	CTlRect::drawEnd();

	CString	tmps = _T("");
	struct	_formR*	formR = m_formItem->GetForm(m_fN);

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		switch (m_type)
		{
		case txLABEL:	// Label
			m_rect.right  = m_rect.left + getDefaultWidth(_T("LABEL"), m_fwidth * (int)strlen((LPCSTR)LABELFACE));
			m_rect.bottom = m_rect.top + getDefaultHeight(_T("LABEL"), m_fheight);
			break;
		default:
			break;
		}
	}

	switch (m_type)
	{
	case txLABEL:	// Label
		formR->kind = FM_LABEL;
		m_formItem->SetFormStr(m_fN, _T("str"), LABELFACE);
		formR->tRGB = getDefaultTRGB(_T("LABEL"), 232);
		formR->pRGB = getDefaultPRGB(_T("LABEL"), 0);
		formR->alpha = 100;			// alpha default setting
		break;
	default:
		break;
	}

	CRect	wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	formR->left   = m_rect.left;
	formR->top    = m_rect.top;
	formR->right  = m_rect.right;
	formR->bottom = m_rect.bottom;
	formR->alignment = AL_CENTER;

	m_dc->Rectangle(m_rect);
}