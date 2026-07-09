// tlPanel.cpp: implementation of the CtlPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlPanel.h"
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

CtlPanel::CtlPanel(CChildView* view, UINT id)
	: CtlRect(view)
{
	m_pic = (Cpicture *) 0;
	switch (id)
	{
	case ID_TOOL_PANEL:
		m_type = pnPANEL;
		m_name = _T("Panel");
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

CtlPanel::~CtlPanel()
{
	if (m_pic)
		delete m_pic;	
}

void CtlPanel::drawEnd()
{
	CtlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		switch (m_type)
		{
		case pnPANEL:
			m_rect.right  = m_rect.left + getDefaultWidth("PANEL", 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight("PANEL", 50);
			break;
		case pnGROUP:
			m_rect.right  = m_rect.left + getDefaultWidth("PANEL", 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight("PANEL", 50);
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

	switch (m_type)
	{
	case pnPANEL:
		m_pFormItem->getFormPoint(m_fN)->kind = FM_PANEL;
		m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("PANEL", basPRGB);
		m_pFormItem->getFormPoint(m_fN)->type = PN_BORDER;
		m_pFormItem->getFormPoint(m_fN)->borders = BD_RAISED;
		break;

	case pnGROUP:
		m_pFormItem->getFormPoint(m_fN)->kind = FM_GROUP;
		m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("PANEL", 232);
		m_pFormItem->setFormStr(m_fN, "str", GROUPFACE);
		break;
	
	default:
		break;
	}

	m_dc->Rectangle(m_rect);
}

void CtlPanel::Draw(CDC* dc, int fN)
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
	case pnPANEL:
		setPen(dc);
		if (!(m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT))
		{
			brush = setBrush(dc);
			dc->FillRect(fRc, brush);
		}

		if ((m_pFormItem->getFormPoint(m_fN)->properties & PR_IMAGE) ||
			(m_pFormItem->getFormPoint(m_fN)->properties & PR_IMAGETEXT))
		{
			tRc = fRc;
			tRc.DeflateRect(1, 1);
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
			tRc.DeflateRect(3, 0);
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

		switch (m_pFormItem->getFormPoint(m_fN)->type)
		{
		case PN_BORDER:
			switch (m_pFormItem->getFormPoint(m_fN)->borders) 
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
			default:
				break;
			}
			break;

		case PN_LINE:
			setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB));
			dc->SelectStockObject(NULL_BRUSH);
			dc->Rectangle(fRc);
			break;

		case PN_NONE:
		default:
			break;
		}
		break;

	case pnGROUP:
		tRc = fRc;
		str = m_pFormItem->getFormStr(m_fN, "str");
		setFont(dc);
		dc->SetBkMode(TRANSPARENT);
		setTextColor(dc);
		tRc.top += m_fheight / 2;	
		dc->DrawEdge(tRc, EDGE_ETCHED, BF_BOTTOMLEFT);
		dc->DrawEdge(tRc, EDGE_ETCHED, BF_RIGHT);
		tRc.right = tRc.left + 5;
		dc->DrawEdge(tRc, EDGE_ETCHED, BF_TOP);
		dc->TextOut(tRc.right+2, m_pFormItem->getFormPoint(m_fN)->top, str);
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
