// tlText.cpp: implementation of the CtlText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlText.h"

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

CtlText::CtlText(CChildView* view, UINT id)
	: CtlRect(view)
{
	switch (id)
	{
	case ID_TOOL_SYS:
		m_type = txSYS;
		m_name = _T("SystemM");
		break;
	case ID_TOOL_LABEL:
		m_type = txLABEL;
		m_name = _T("Label");
		break;
	default:
		m_type = txNONE;
		break;
	}
}

CtlText::~CtlText()
{
}

void CtlText::drawEnd()
{
	CtlRect::drawEnd();

	CString	tmps = _T("");
	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		switch (m_type)
		{
		case txSYS:	// System Message
			switch (m_pFormItem->getFormPoint(m_fN)->type)
			{
			case SYS_TIME:
				tmps = _T("HH:MM:SS");		break;
			case SYS_TERM:
				tmps = _T("TERM");		break;
			case SYS_DATE:
			default:
				tmps = _T("YYYY/MM/DD");	break;
			}
			m_rect.right  = m_rect.left + getDefaultWidth("SYSTEM", m_fwidth * tmps.GetLength());
			m_rect.bottom = m_rect.top + getDefaultHeight("SYSTEM", m_fheight);
			break;
		case txLABEL:	// Label
			m_rect.right  = m_rect.left + getDefaultWidth("LABEL", m_fwidth * strlen(LABELFACE));
			m_rect.bottom = m_rect.top + getDefaultHeight("LABEL", m_fheight);
			break;
		default:
			break;
		}
	}

	switch (m_type)
	{
	case txSYS:	// System Message
		if (m_pFormItem->getFormPoint(m_fN)->type == NULL)
			m_pFormItem->getFormPoint(m_fN)->type = SYS_DATE;
		m_pFormItem->getFormPoint(m_fN)->kind   = FM_SYS;
		m_pFormItem->getFormPoint(m_fN)->tRGB   = getDefaultTRGB("SYSTEM", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB   = getDefaultPRGB("SYSTEM", basPRGB);
		break;
	case txLABEL:	// Label
		m_pFormItem->getFormPoint(m_fN)->kind = FM_LABEL;
		m_pFormItem->setFormStr(m_fN, "str", LABELFACE);
		m_pFormItem->getFormPoint(m_fN)->tRGB   = getDefaultTRGB("LABEL", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB   = getDefaultPRGB("LABEL", basPRGB);
		break;
	default:
		break;
	}

	CRect	wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	m_pFormItem->getFormPoint(m_fN)->left   = m_rect.left;
	m_pFormItem->getFormPoint(m_fN)->top    = m_rect.top;
	m_pFormItem->getFormPoint(m_fN)->right  = m_rect.right;
	m_pFormItem->getFormPoint(m_fN)->bottom = m_rect.bottom;

	m_pFormItem->getFormPoint(m_fN)->alignment = AL_LEFT;

	m_dc->Rectangle(m_rect);
}

void CtlText::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	tRc,  fRc;
	int	ndc = dc->SaveDC();
	UINT	style;
	CSize	size;
	CString	str = _T("");

	CTOOL::Draw(dc, fN);
	fRc = getRect();
	switch (m_type)
	{
	case txSYS:
		setFont(dc);
		setTextColor(dc);
		if (m_pFormItem->getFormPoint(fN)->properties & PR_TRANSPARENT)
			dc->SetBkMode(TRANSPARENT);
		else
		{
			brush = setBrush(dc);
			dc->FillRect(fRc, brush);
			setBkColor(dc);
		}

		switch (m_pFormItem->getFormPoint(fN)->alignment)
		{
		case AL_RIGHT:
			style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;	break;
		case AL_CENTER:
			style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;	break;
		case AL_LEFT:
		default:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;	break;
		}

		switch (m_pFormItem->getFormPoint(fN)->type)
		{
		case SYS_TIME:
			str = _T("HH:MM:SS");	break;
		case SYS_TERM:
			str = _T("TERM");	break;
		case SYS_DATE:
		default:
			str = _T("YYYY/MM/DD");	break;
		}
		dc->DrawText(str, fRc, style);
		break;

	case txLABEL:
		setFont(dc);
		setTextColor(dc);
		if (m_pFormItem->getFormPoint(fN)->properties & PR_TRANSPARENT)
			dc->SetBkMode(TRANSPARENT);
		else
		{
			brush = setBrush(dc);
			setBkColor(dc);
			dc->FillRect(fRc, brush);
		}

		switch (m_pFormItem->getFormPoint(fN)->alignment)
		{
		case AL_RIGHT:
			style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;	break;
		case AL_CENTER:
			style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;	break;
		case AL_LEFT:
		default:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;	break;
		}

		str = m_pFormItem->getFormStr(fN, "str");
		editAmpersand(str);
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_VERT)
		{
			int nLength = str.GetLength(), nGap;
			str = convertVString(str);
			style &= ~DT_SINGLELINE;
			nGap = (fRc.Height() - dc->GetTextExtent(str).cy * (str.GetLength() - nLength)) / 2;
			if (nGap >= 0)
				fRc.OffsetRect(0, nGap);
		}
		else if (str.Replace("\\n", "\n"))
		{
			style &= ~DT_SINGLELINE;
			style |= DT_EXTERNALLEADING;
		}

		dc->DrawText(str, fRc, style);
		break;

	default:
		break;
	}
	dc->RestoreDC(ndc);
}