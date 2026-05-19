// tlButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlButton.h"
#include "tlImage.h"

#include "../resource.h"
#include "../amCom/formItem.h"

// CTlButton

CTlButton::CTlButton(class CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type)
	: CTlRect(view, draw, formItem)
{
	m_image = (CTlImage *) NULL;
	switch (type)
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

CTlButton::~CTlButton()
{
	if (m_image)
		delete m_image;
}


void CTlButton::Draw(CDC*dc, int fN)
{
	CBrush*	brush;
	CString	str;
	UINT	style;
	int	dx;
	int	ndc = dc->SaveDC();
	CRect	tRc, tRc1, tRc2, fRc;
	struct	_formR*	formR = m_formItem->GetForm(fN);

	CTlBase::Draw(dc, fN);

	fRc = getRect();
	setPen(dc);
	switch (m_type) 
	{
	case bnPUSH:
		brush = setBrush(dc);
		dc->FillRect(fRc, brush);

		/*if (formR->properties & (PR_IMAGE|PR_IMAGETEXT))
		{
			tRc = fRc;
			//tRc.DeflateRect(1, 1);
			str = m_formItem->GetFormStr(fN, _T("str2"));
			if (!str.IsEmpty())
			{
				if (m_image)
				{
					delete m_image;
					m_image = NULL;
				}
				getPath(str);
				m_image = new CTlImage(m_draw, str);
				m_image->DrawPicture(dc, tRc, formR->alignImage);
			}
			else
				drawBMP(dc, tRc, ((formR->properties & PR_IMAGETEXT) ? _T("") : _T("IMAGE")));
		}*/

		//if (!(formR->properties & PR_IMAGE))
		//{
			setFont(dc);
			setTextColor(dc);
			dc->SetBkMode(TRANSPARENT);
			switch (formR->alignment)
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
			tRc.left  += 3;
			tRc.right -= 3;
			
			str = m_formItem->GetFormStr(fN, _T("str"));
			editAmpersand(str);
	
			if (!m_formItem->GetFormStr(fN, _T("str2")).IsEmpty())
			{
				drawPNG(dc, tRc, m_formItem->GetFormStr(fN, _T("str2")));
				dc->SetBkMode(TRANSPARENT);
			}

			dc->DrawText(str, tRc, style);
		//}
		//if (!(formR->properties & PR_IMAGE) && !(formR->properties & PR_IMAGETEXT))
		//	drawEdge(dc, fRc, EDGE_RAISED);
		break;

	case bnRADIO:
		tRc = tRc1 = tRc2 = fRc;
		setFont(dc);
		setTextColor(dc);
		brush = setBrush(dc);
		setBkColor(dc);
		dc->FillRect(tRc, brush);

		tRc1.right  = tRc.left + m_fheight;
		tRc1.bottom = tRc.top + m_fheight;
		if (tRc.Height() < tRc.Width())
		{
			tRc2.left = tRc1.right + 3;
			str = m_formItem->GetFormStr(fN, _T("str"));
			editAmpersand(str);

			if (!m_formItem->GetFormStr(fN, _T("str2")).IsEmpty())
			{
				drawPNG(dc, tRc2, m_formItem->GetFormStr(fN, _T("str2")));
				dc->SetBkMode(TRANSPARENT);
			}
			dc->DrawText(str, tRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		}

		if (tRc.Height() > tRc1.Height())
		{
			dx = (tRc.Height() - tRc1.Height())/2;
			tRc1.top += dx;
			tRc1.bottom = tRc1.top + m_fheight;
		}

		style = DFCS_BUTTONRADIO;
		if (formR->properties & PR_CHECKED)
			style |= DFCS_CHECKED;

		dc->DrawFrameControl(tRc1, DFC_BUTTON, style);
		break;

	case bnCHECK:
		tRc = tRc1 = tRc2 = fRc;
		setFont(dc);
		setTextColor(dc);
		brush = setBrush(dc);
		setBkColor(dc);
		dc->FillRect(tRc, brush);

		tRc1.right  = tRc.left + m_fheight;
		tRc1.bottom = tRc.top + m_fheight;
		if (tRc.Height() < tRc.Width())
		{
			// 토글버튼 영역 조정
			if (!(formR->properties & PR_OVERLAYIMG))
				tRc2.left = tRc1.right + 3;

			str = m_formItem->GetFormStr(fN, _T("str"));
			editAmpersand(str);
			
			if (!m_formItem->GetFormStr(fN, _T("str2")).IsEmpty())
			{
				drawPNG(dc, tRc2, m_formItem->GetFormStr(fN, _T("str2")));
				dc->SetBkMode(TRANSPARENT);
			}
			
			dc->DrawText(str, tRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		}	

		if (tRc.Height() > tRc1.Height())
		{
			dx = (tRc.Height() - tRc1.Height())/2;
			tRc1.top += dx;
			tRc1.bottom = tRc1.top + m_fheight;
		}

		// 토글버튼 추가
		if (!(formR->properties & PR_OVERLAYIMG))
		{
			style = DFCS_BUTTONCHECK;
			if (formR->properties & PR_CHECKED)
				style |= DFCS_CHECKED;

			dc->DrawFrameControl(tRc1, DFC_BUTTON, style);
		}
		break;
	}

	if (bnPUSH == m_type)
	{
		setPen(dc, getRGB(244));	// outline
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(fRc);
	}
	dc->RestoreDC(ndc);
}

void CTlButton::drawEnd()
{
	CTlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		switch (m_type) 
		{
		case bnRADIO:		// Radio
			m_rect.right  = m_rect.left + getDefaultWidth(_T("RADIO"), 100);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("RADIO"), m_fheight);
			break;
		case bnCHECK:		// CheckBox
			m_rect.right  = m_rect.left + getDefaultWidth(_T("CHECK"), 100);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("CHECK"), m_fheight);
			break;

		case bnPUSH:		// Push Button
			m_rect.right  = m_rect.left + getDefaultWidth(_T("BUTTON"), 80);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("BUTTON"), 40);
			break;

		default:
			break;
		}
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

	int	ii;
	CRect	tRc;
	CRgn	pRgn;
	switch (m_type)
	{
	case bnRADIO:
		formR->kind = FM_RADIO;
		formR->tRGB = getDefaultTRGB(_T("RADIO"), 232);
		formR->pRGB = getDefaultPRGB(_T("RADIO"), 0);
		formR->alpha = 100;			// alpha default setting
		m_formItem->SetFormStr(m_fN, _T("str"), RADIOFACE);
		for (ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			tRc.SetRect(m_formItem->GetForm(ii)->left, m_formItem->GetForm(ii)->top,
					m_formItem->GetForm(ii)->right, m_formItem->GetForm(ii)->bottom);
			pRgn.CreateRectRgnIndirect(&tRc);
			if (pRgn.RectInRegion(&m_rect) && m_formItem->GetForm(ii)->kind == FM_GROUP)
			{
				m_formItem->SetFormStr(m_fN, _T("auxS"), m_formItem->GetFormStr(ii, _T("name")));
				pRgn.DeleteObject();
				break;
			}
			pRgn.DeleteObject();
		}
		break;

	case bnCHECK:
		formR->kind = FM_CHECK;
		formR->tRGB = getDefaultTRGB(_T("CHECK"), 232);
		formR->pRGB = getDefaultPRGB(_T("CHECK"), 0);
		formR->alpha = 100;			// alpha default setting
		m_formItem->SetFormStr(m_fN, _T("str"), CHECKFACE);
		break;

	case bnPUSH:
		formR->kind = FM_BUTTON;
		formR->pRGB = getDefaultPRGB(_T("BUTTON"), basPRGB);
		formR->tRGB = getDefaultTRGB(_T("BUTTON"), 232);
		formR->alpha = 100;			// alpha default setting
		formR->spRGB = getDefaultPRGB(_T("BUTTON"), basPRGB);
		m_formItem->SetFormStr(m_fN, _T("str"), _T("Button"));
		break;

	default:
		break;
	}

	m_dc->Rectangle(m_rect);
}
