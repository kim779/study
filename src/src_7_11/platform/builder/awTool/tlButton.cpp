// tlButton.cpp: implementation of the CtlButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlButton.h"
#include "picture.h"

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

CtlButton::CtlButton(CChildView* view, UINT id)
	: CtlRect(view)
{
	m_pic = (Cpicture *) 0;
	switch (id)
	{
	case ID_TOOL_RADIO:
		m_type = bnRADIO;
		m_name = _T("Radio");
		break;
	case ID_TOOL_CHECK:
		m_type = bnCHECK;
		m_name = _T("CheckBox");
		break;
	case ID_TOOL_PUSH:
		m_type = bnPUSH;
		m_name = _T("PushButton");
		break;
	default:
		m_type = bnNONE;
		break;
	}
}

CtlButton::~CtlButton()
{
	if (m_pic)
		delete m_pic;
}

void CtlButton::drawEnd()
{
	CtlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		switch (m_type) 
		{
		case bnRADIO:		// Radio
			m_rect.right  = m_rect.left + getDefaultWidth("RADIO", 100);
			m_rect.bottom = m_rect.top  + getDefaultHeight("RADIO", m_fheight);
			break;
		case bnCHECK:		// CheckBox
			m_rect.right  = m_rect.left + getDefaultWidth("CHECK", 100);
			m_rect.bottom = m_rect.top  + getDefaultHeight("CHECK", m_fheight);
			break;

		case bnPUSH:		// Push Button
			m_rect.right  = m_rect.left + getDefaultWidth("BUTTON", 80);
			m_rect.bottom = m_rect.top  + getDefaultHeight("BUTTON", 40);
			break;

		default:
			break;
		}
	}

	CRect	wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	m_pFormItem->getFormPoint(m_fN)->left   = m_rect.left;
	m_pFormItem->getFormPoint(m_fN)->top    = m_rect.top;
	m_pFormItem->getFormPoint(m_fN)->right  = m_rect.right;
	m_pFormItem->getFormPoint(m_fN)->bottom = m_rect.bottom;

	int	ii;
	CRect	tRc;
	CRgn	pRgn;
	switch (m_type)
	{
	case bnRADIO:
		m_pFormItem->getFormPoint(m_fN)->kind  = FM_BUTTON;
		m_pFormItem->getFormPoint(m_fN)->type  = BTN_RADIO;
		m_pFormItem->getFormPoint(m_fN)->tRGB  = getDefaultTRGB("RADIO", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB  = getDefaultPRGB("RADIO", basPRGB);
		m_pFormItem->setFormStr(m_fN, "str", RADIOFACE);
		for (ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
		{
			tRc.SetRect(m_pFormItem->getFormPoint(ii)->left, m_pFormItem->getFormPoint(ii)->top,
					m_pFormItem->getFormPoint(ii)->right, m_pFormItem->getFormPoint(ii)->bottom);
			pRgn.CreateRectRgnIndirect(&tRc);
			if (pRgn.RectInRegion(&m_rect) && m_pFormItem->getFormPoint(ii)->kind == FM_GROUP)
			{
				pRgn.DeleteObject();
				break;
			}
			pRgn.DeleteObject();
		}

		if (ii >= 0 && ii < m_view->m_pFormItem->getFormCount())
			m_pFormItem->setFormStr(m_fN, "auxS", m_pFormItem->getFormStr(ii, "name"));
		break;

	case bnCHECK:
		m_pFormItem->getFormPoint(m_fN)->kind  = FM_BUTTON;
		m_pFormItem->getFormPoint(m_fN)->type  = BTN_CHECK;
		m_pFormItem->getFormPoint(m_fN)->tRGB  = getDefaultTRGB("CHECK", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB  = getDefaultPRGB("CHECK", basPRGB);
		m_pFormItem->setFormStr(m_fN, "str", CHECKFACE);
		break;

	case bnPUSH:
		m_pFormItem->getFormPoint(m_fN)->kind = FM_BUTTON;
		m_pFormItem->getFormPoint(m_fN)->type = BTN_PUSH;
		m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("BUTTON", basPRGB);
		m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("BUTTON", 232);
		break;

	default:
		break;
	}

	m_dc->Rectangle(m_rect);
}

void CtlButton::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CString	str;
	UINT	style;
	int	dx;
	int	ndc = dc->SaveDC();
	CRect	tRc, tRc1, tRc2, fRc;

	CTOOL::Draw(dc, fN);

	fRc = getRect();
	setPen(dc);
	switch (m_type) 
	{
	case bnPUSH:
		if (!(m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT))
		{
			brush = setBrush(dc);
			dc->FillRect(fRc, brush);
		}

		if ((m_pFormItem->getFormPoint(m_fN)->properties & PR_IMAGE) ||
			(m_pFormItem->getFormPoint(m_fN)->properties & PR_IMAGETEXT))

		{
			tRc = fRc;
			//tRc.DeflateRect(1, 1);
			if (!m_pFormItem->getFormStr(m_fN, "str2").IsEmpty())
			{
				if (m_pic)
				{
					delete m_pic;
					m_pic = (Cpicture *) 0;
				}
				CString	path = m_pFormItem->getFormStr(m_fN, "str2");
				getPath(path);
				m_pic = new Cpicture(path);
				m_pic->DrawPicture(dc, tRc, m_pFormItem->getFormPoint(m_fN)->alignImage);
			}
			else
				drawBMP(dc, tRc, ((m_pFormItem->getFormPoint(m_fN)->properties & PR_IMAGETEXT)?"":"IMAGE"));
		}
		if (!(m_pFormItem->getFormPoint(m_fN)->properties & PR_IMAGE))
		{
			setFont(dc);
			setTextColor(dc);
			dc->SetBkMode(TRANSPARENT);
			switch (m_pFormItem->getFormPoint(m_fN)->alignment)
			{
			case AL_RIGHT:
				style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;
				break;
			case AL_LEFT:
				style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
				break;
			case AL_CENTER:
			default:
				style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;
				break;
			}

			tRc = fRc;
			tRc.left += 3;
			tRc.right -= 3;
			str = m_pFormItem->getFormStr(m_fN, "str");
			editAmpersand(str);
			if (m_pFormItem->getFormPoint(m_fN)->properties & PR_VERT)
			{
				int nLength = str.GetLength(), nGap;
				str = convertVString(str);
				style &= ~DT_SINGLELINE;
				nGap = (tRc.Height() - dc->GetTextExtent(str).cy * (str.GetLength() - nLength)) / 2;
				if (nGap >= 0)
					tRc.OffsetRect(0, nGap);
			}
			dc->DrawText(str, tRc, style);
		}
		if (!(m_pFormItem->getFormPoint(m_fN)->properties & PR_IMAGE) &&
			!(m_pFormItem->getFormPoint(m_fN)->properties & PR_IMAGETEXT))
			drawEdge(dc, fRc, EDGE_RAISED);
		break;

	case bnRADIO:
		tRc = tRc1 = tRc2 = fRc;
		setFont(dc);
		setTextColor(dc);
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
			dc->SetBkMode(TRANSPARENT);
		else
		{
			brush = setBrush(dc);
			setBkColor(dc);
			dc->FillRect(tRc, brush);
		}

		tRc1.right  = tRc.left + m_fheight;
		tRc1.bottom = tRc.top + m_fheight;
		if (tRc.Height() < tRc.Width())
		{
			tRc2.left = tRc1.right + 3;
			str = m_pFormItem->getFormStr(m_fN, "str");
			editAmpersand(str);
			dc->DrawText(str, tRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		}

		if (tRc.Height() > tRc1.Height())
		{
			dx = (tRc.Height() - tRc1.Height())/2;
			tRc1.top += dx;
			tRc1.bottom = tRc1.top + m_fheight;
		}

		style = DFCS_BUTTONRADIO;
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_CHECKED)
			style |= DFCS_CHECKED;

		dc->DrawFrameControl(tRc1, DFC_BUTTON, style);
		break;

	case bnCHECK:
		tRc = tRc1 = tRc2 = fRc;
		setFont(dc);
		setTextColor(dc);
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
			dc->SetBkMode(TRANSPARENT);
		else
		{
			brush = setBrush(dc);
			setBkColor(dc);
			dc->FillRect(tRc, brush);
		}

		tRc1.right  = tRc.left + m_fheight;
		tRc1.bottom = tRc.top + m_fheight;
		if (tRc.Height() < tRc.Width())
		{
			tRc2.left = tRc1.right + 3;
			str = m_pFormItem->getFormStr(m_fN, "str");
			editAmpersand(str);
			dc->DrawText(str, tRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		}	

		if (tRc.Height() > tRc1.Height())
		{
			dx = (tRc.Height() - tRc1.Height())/2;
			tRc1.top += dx;
			tRc1.bottom = tRc1.top + m_fheight;
		}

		style = DFCS_BUTTONCHECK;
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_CHECKED)
			style |= DFCS_CHECKED;

		dc->DrawFrameControl(tRc1, DFC_BUTTON, style);
		break;
	}

	dc->RestoreDC(ndc);
}