// tlPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlPanel.h"
#include "tlImage.h"

#include "../resource.h"
#include "../amCom/formItem.h"

// CTlPanel

CTlPanel::CTlPanel(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type)
	: CTlRect(view, draw, formItem)
{
	m_image = (CTlImage *) NULL;
	switch (type)
	{
	case ID_TOOL_IMAGEVIEW:
		m_type = pnIMAGE;
		m_name = _T("Imageview");
		break;
	case ID_TOOL_GROUP:
		m_type = pnGROUP;
		m_name = _T("Group");
		break;
	default:
		m_type = pnNONE;
		break;
	}
}

CTlPanel::~CTlPanel()
{
	if (m_image)
		delete m_image;
}


// CTlPanel 멤버 함수

void CTlPanel::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	tRc,  fRc;
	int	ndc = dc->SaveDC();
	UINT	style;
	CSize	size;
	CString	str;
	struct	_formR*	formR = m_formItem->GetForm(fN);

	CTlBase::Draw(dc, fN);
	fRc = getRect();
	switch (m_type)
	{
	case pnIMAGE:
		setPen(dc);
		brush = setBrush(dc);
		dc->FillRect(fRc, brush);

		/*if (formR->properties & (PR_IMAGE|PR_IMAGETEXT))
		{
			tRc = fRc;
			tRc.DeflateRect(1, 1);
			str = m_formItem->GetFormStr(fN, _T("str2"));
			if (!str.IsEmpty())
			{
				if (m_image)
					delete m_image;
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

			tRc = fRc;
			tRc.DeflateRect(3, 0);
			str = m_formItem->GetFormStr(fN, _T("str"));
			editAmpersand(str);

			if (!m_formItem->GetFormStr(fN, _T("str2")).IsEmpty())
			{
				drawPNG(dc, tRc, m_formItem->GetFormStr(fN, _T("str2")));
				dc->SetBkMode(TRANSPARENT);
			}

			dc->DrawText(str, tRc, style);
		//}
		break;

	case pnGROUP:
		tRc = fRc;
		str = m_formItem->GetFormStr(fN, _T("str"));
		setFont(dc);
		dc->SetBkMode(TRANSPARENT);
		setTextColor(dc);
		tRc.top += m_fheight / 2;	
		dc->DrawEdge(tRc, EDGE_ETCHED, BF_BOTTOMLEFT);
		dc->DrawEdge(tRc, EDGE_ETCHED, BF_RIGHT);
		tRc.right = tRc.left + 5;
		dc->DrawEdge(tRc, EDGE_ETCHED, BF_TOP);
		dc->TextOut(tRc.right+2, formR->top, str);
		size = dc->GetTextExtent(str);
		if (str.IsEmpty())
			size.cx = 0;
		tRc.left  = tRc.right + size.cx + 4;
		tRc.right = fRc.right;
		dc->DrawEdge(tRc, EDGE_ETCHED, BF_TOP);
		break;
	
	default:
		break;
	}

	dc->RestoreDC(ndc);
}

void CTlPanel::drawEnd()
{
	CTlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		switch (m_type)
		{
		case pnIMAGE:
			m_rect.right  = m_rect.left + getDefaultWidth(_T("PANEL"), 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("PANEL"), 50);
			break;
		case pnGROUP:
			m_rect.right  = m_rect.left + getDefaultWidth(_T("PANEL"), 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("PANEL"), 50);
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
	formR->alpha  = 100;			// alpha default setting

	switch (m_type)
	{
	case pnIMAGE:
		formR->kind = FM_IMAGEVIEW;
		formR->pRGB = getDefaultPRGB(_T("PANEL"), basPRGB);
		break;

	case pnGROUP:
		formR->kind = FM_GROUP;
		formR->tRGB = getDefaultTRGB(_T("PANEL"), 232);
		m_formItem->SetFormStr(m_fN, _T("str"), GROUPFACE);
		break;
	
	default:
		break;
	}

	m_dc->Rectangle(m_rect);
}
