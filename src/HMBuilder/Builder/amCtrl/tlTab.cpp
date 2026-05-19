// tlTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlTab.h"
#include "markWnd.h"

#include "../resource.h"
#include "../amCom/formItem.h"

#define	TAB_EXTRA	14
#define	TAB_EXTRA2	25

// CTlTab

CTlTab::CTlTab(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type)
	: CTlRect(view, draw, formItem)
{
	switch (type)
	{
	case ID_TOOL_TAB:
		m_type = taTAB;
		m_name = _T("Tab");
		break;
	default:
		m_type = taNONE;
		break;
	}
	m_markWnd = NULL;
	m_selected = -1;
	m_str = m_dat = _T("");
}

CTlTab::~CTlTab()
{
}

void CTlTab::Draw(CDC* dc, int fN)
{
	int	ndc = dc->SaveDC();
	struct	_formR*	formR = m_formItem->GetForm(fN);

	CTlBase::Draw(dc, fN);
	if (formR->spRGB == 0 && formR->stRGB == 0)
	{
		formR->spRGB = formR->pRGB;
		formR->stRGB = formR->tRGB;
	}

	drawTAB(dc);
	dc->RestoreDC(ndc);
}

void CTlTab::MouseDrag()
{
	if (m_view->m_bSelect && m_markWnd == NULL && m_selected >= 0)
	{
		m_markWnd = new CMarkWnd();
		m_markWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 8, 5), m_view, 1);
		m_markWnd->SetTP(4);
	}
}

int CTlTab::Selectable(CPoint point, CRect &rc, CPoint &pt, bool bSet)
{
	CPoint	pts[6];
	CString	str, item, cells;
	CString	data, info;
	bool	vertical, scroll;
	CRect	fRc, pRc, tRc, tRc1, cell;
	int	count,  vcount,  cx = 2;
	int	last, len, dx;
	CDC *dc = m_dc;
	CRgn	tRgn;

	fRc = pRc = getRect();
	setFont(dc);

	count = max((int)m_formItem->GetForm(m_fN)->keys, 1);
	len = vcount = 0;
	info = m_formItem->GetFormStr(m_fN, _T("str"));
	if (!info.IsEmpty())
	{
		for (int ii = 0; ii < (int)m_formItem->GetForm(m_fN)->keys; ii++)
		{
			item = parse(info, _T("\t"));
			if (item.GetLength() && item.GetAt(0) == _T('1'))
				vcount++;
		}
	}

	vertical = scroll = false;
	if (m_formItem->GetForm(m_fN)->alignment == AL_LEFT || m_formItem->GetForm(m_fN)->alignment == AL_RIGHT)
		vertical = true;

	data = m_formItem->GetFormStr(m_fN, _T("dat"));
	info = m_formItem->GetFormStr(m_fN, _T("str"));

	scroll = isScrolled(dc, (int)m_formItem->GetForm(m_fN)->keys, data, info, last, dx, vertical);

	vcount = max(vcount, 1);
	if (vertical)
	{
		if (scroll)
			pRc.bottom -= (pRc.Width()/2);
		else
			pRc.bottom = pRc.top + dx;

		dx = pRc.top;
	}
	else
	{
		if (scroll)
		{
			tRc = pRc;
			pRc.right -= (int)(float)(pRc.Height() * 2.0 / 3.0) * 2;
			
			tRc.SubtractRect(tRc, pRc);
			if (tRc.PtInRect(point))
			{
				if (point.x < tRc.left + tRc.Width() / 2)
				{
					m_formItem->GetForm(m_fN)->scrollPos--;
					if (m_formItem->GetForm(m_fN)->scrollPos < 0)
						m_formItem->GetForm(m_fN)->scrollPos = 0;
				}
				else
				{
					m_formItem->GetForm(m_fN)->scrollPos++;
					dx = pRc.left;
					for (int ii = 0; ii < (int)m_formItem->GetForm(m_fN)->keys; ii++)
					{
						item = parse(data, _T("\t"));
						if (m_formItem->GetForm(m_fN)->scrollPos > ii)
							continue;
						dx += getSize(dc, item);
					}
					
					if (pRc.right > dx)
						m_formItem->GetForm(m_fN)->scrollPos--;
				}
				return -1;
			}

		}
		else
			pRc.right = pRc.left + dx;

		last = min((int)m_formItem->GetForm(m_fN)->keys, last + m_formItem->GetForm(m_fN)->scrollPos);
		dx = pRc.left;
	}

	cell.SetRectEmpty();
	for (int ii = 0; ii < last; ii++)
	{
		if (!m_formItem->GetFormStr(m_fN, _T("str")).IsEmpty())
		{
			item = parse(info, _T("\t"));
			if ((item.GetLength() && item.GetAt(0) != _T('1')) && (m_formItem->GetForm(m_fN)->keys != 0))
			{
				if (!m_formItem->GetFormStr(m_fN, _T("dat")).IsEmpty())
					str = parse(data, _T("\t"));
				continue;
			}
		}

		if (!m_formItem->GetFormStr(m_fN, _T("dat")).IsEmpty())
			str = parse(data, _T("\t"));

		if (m_formItem->GetForm(m_fN)->scrollPos > ii)
			continue;

		str = (m_formItem->GetForm(m_fN)->keys == 0) ? m_name : str;

		tRc = pRc;
		if (vertical)
			tRc.top = dx;
		else
			tRc.left = dx;

		if (!scroll || ii < last-1)
		{
			dx += getSize(dc, str, vertical);
			if (vertical)
				tRc.bottom = dx;
			else
				tRc.right = dx;
		}

		switch (m_formItem->GetForm(m_fN)->alignment)
		{
		case AL_LEFT:
			if (tRc.PtInRect(point))
			{
				if (bSet)
					m_formItem->GetForm(m_fN)->checked = ii;
				pt = CPoint(tRc.left, tRc.top);
				return ii;
			}
			pt = CPoint(tRc.left, tRc.bottom);
			break;

		case AL_RIGHT:
			if (tRc.PtInRect(point))
			{
				if (bSet)
					m_formItem->GetForm(m_fN)->checked = ii;
				pt = CPoint(tRc.right, tRc.top);
				return ii;
			}
			pt = CPoint(tRc.right, tRc.bottom);
			break;

		case AL_BOTTOM:
			if (tRc.PtInRect(point))
			{
				if (bSet)
					m_formItem->GetForm(m_fN)->checked = ii;
				pt = CPoint(tRc.left, tRc.bottom);
				return ii;
			}
			pt = CPoint(tRc.right, tRc.bottom);
			break;

		case AL_TOP:
		default:
			if (tRc.PtInRect(point))
			{
				if (bSet)
					m_formItem->GetForm(m_fN)->checked = ii;
				pt = CPoint(tRc.left, tRc.top);
				return ii;
			}
			pt = CPoint(tRc.right, tRc.top);
			break;
		}
	}
	return -1;
}

void CTlTab::drawBegin(CPoint point)
{
	if (!m_view->m_bSelect)
	{
		CTlRect::drawBegin(point);
		return;
	}
	m_fN = m_view->m_selF;
	m_selected = Selectable(point, m_rect, m_toPt);
	m_view->InvalidateRect(getRect());
}

void CTlTab::drawTo(CPoint point)
{
	if (!m_view->m_bSelect)
	{
		CTlRect::drawTo(point);
		return;
	}

	m_fN = m_view->m_selF;
	Selectable(point, m_rect, m_toPt, false);
	m_rect.InflateRect(0, 0, 100, 0);

	if (m_type == taTAB)
		m_rect.bottom = m_rect.top + m_formItem->GetForm(m_fN)->size;
	
	if (m_rect.PtInRect(point))
	{
		if (m_markWnd && m_selected >= 0)
		{
			m_markWnd->SetWindowPos(NULL, m_toPt.x - 5, m_toPt.y - 6, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
			SetCursor(AfxGetApp()->LoadCursor(IDC_DROPCOPY));
		}
	}
	else
	{
		if (m_markWnd && m_selected >= 0)
		{
			m_markWnd->ShowWindow(SW_HIDE);
			SetCursor(AfxGetApp()->LoadCursor(IDC_NODROPCOPY));
		}
	}
}

void CTlTab::drawEnd()
{
	if (m_markWnd)
	{
		m_markWnd->DestroyWindow();
		delete m_markWnd;
		m_markWnd = NULL;
	}

	if (!m_view->m_bSelect)
	{
		struct _formR *formR = m_formItem->GetForm(m_fN);
		CTlRect::drawEnd();
		
		switch (m_type)
		{
		case taTAB:
			formR->size = m_fheight + 12;
			if (m_rect.Height() < formR->size)
				m_rect.bottom = m_rect.top + formR->size;
			if (m_rect.Width() < 10 || m_rect.Height() < 30)
			{
				m_rect.right  = m_rect.left + getDefaultWidth(_T("TAB"), 300);
				m_rect.bottom = m_rect.top + getDefaultHeight(_T("TAB"), formR->size);
			}
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
		formR->alpha  = 100;			// alpha default setting
		
		switch (m_type)
		{
		case taTAB:
			formR->kind = FM_TAB;
			formR->alignment = AL_TOP;
			formR->type = 1;
			formR->keys = 1;
			m_formItem->SetFormStr(m_fN, _T("dat"), _T("Tab0\t"));
			m_formItem->SetFormStr(m_fN, _T("str"), _T("1:0:formname\t"));
			
			formR->pRGB = getDefaultPRGB(_T("UTAB"), basPRGB);
			formR->tRGB = getDefaultTRGB(_T("UTAB"), 232);
			formR->spRGB = getDefaultPRGB(_T("UTAB"), basPRGB);
			formR->stRGB = getDefaultTRGB(_T("UTAB"), 232);
			break;
			
		default:
			break;
		}
		
		m_dc->Rectangle(m_rect);
		return;
	}
	else
	{
		if (m_selected >= 0)
		{
			m_fN = m_view->m_selF;
			struct _formR *formR = m_formItem->GetForm(m_fN);
			int	sel = Selectable(m_endpt, m_rect, m_toPt, false);
			
			m_rect.InflateRect(0, 0, 100, 0);
			if (m_type == taTAB)
				m_rect.bottom = m_rect.top + m_formItem->GetForm(m_fN)->size;
			if (m_rect.PtInRect(m_endpt))
			{
				if (sel == -1)
				{
					deletePage(formR->checked);
					addPage(formR->keys);
				}
				else if (formR->checked < sel)
				{
					deletePage(formR->checked);
					addPage(sel - 1);
				}
				else if (formR->checked > sel)
				{
					deletePage(formR->checked);
					addPage(sel);
				}
			}
		}
	}
	m_rect = getRect();
	m_rect.InflateRect(3, 3);
	m_view->InvalidateRect(&m_rect);
}

void CTlTab::drawTAB(CDC* dc)
{
	CPoint	pts[20];
	UINT	style;
	CBrush*	brush;
	CString	str, str2, item, cells, tmpS;
	CString	data, info;
	bool	vertical, scroll;
	CRect	fRc, pRc, tRc, tRc1, rc, cell;
	int	visible, count,  vcount,  cx = 2;
	int	last, len, dx, twidth;
	struct	_formR*	formR = m_formItem->GetForm(m_fN);

	fRc = pRc = getRect();
	setFont(dc);

	str2 = m_formItem->GetFormStr(m_fN, _T("str2"));

	count = max((int)formR->keys, 1);
	len = vcount = 0;

	info = m_formItem->GetFormStr(m_fN, _T("str"));
	if (!info.IsEmpty())
	{
		for (int ii = 0; ii < (int)formR->keys; ii++)
		{
			item = parse(info, _T("\t"));
			if (!item.IsEmpty() && item.GetAt(0) == _T('1'))
				vcount++;
		}
	}

	vertical = scroll = false;
	if (formR->alignment == AL_LEFT || formR->alignment == AL_RIGHT)
		vertical = true;


	data = m_formItem->GetFormStr(m_fN, _T("dat"));
	info = m_formItem->GetFormStr(m_fN, _T("str"));

	scroll = isScrolled(dc, (int)formR->keys, data, info, last, dx, vertical);
	vcount = max(vcount, 1);
	if (vertical)
	{
		if (scroll)
			pRc.bottom -= (pRc.Width()/2);
		else
			pRc.bottom = pRc.top + dx;
		dx = pRc.top;
	}
	else
	{
		if (scroll)
		{
			pRc.right -= (int)(float)(pRc.Height() * 2.0 / 3.0) * 2;
			last = min((int)formR->keys, last + formR->scrollPos);
		}
		else
			pRc.right = pRc.left + dx;
		dx = pRc.left;
	}

	visible = 0;

	cell.SetRectEmpty();
	for (int ii = 0; ii < last; ii++)
	{
		if (!m_formItem->GetFormStr(m_fN, _T("str")).IsEmpty())
		{
			item = parse(info, _T("\t"));

			if ((!item.IsEmpty() && item.GetAt(0) != _T('1')) && (formR->keys != 0))
			{
				if (!m_formItem->GetFormStr(m_fN, _T("dat")).IsEmpty())
					str = parse(data, _T("\t"));
				continue;
			}
		}

		if (!m_formItem->GetFormStr(m_fN, _T("dat")).IsEmpty())
			str = parse(data, _T("\t"));

		if (formR->scrollPos > ii)
			continue;

		str = (formR->keys == 0) ? m_name : str;

		tRc = pRc;
		if (vertical)
			tRc.top = dx;
		else
			tRc.left = dx;

		if (!scroll || ii < last-1)
		{
			dx += getSize(dc, str, vertical);
			if (vertical)
				tRc.bottom = dx;
			else
				tRc.right = dx;
		}

		if (ii == formR->checked)
		{
			setTextColor(dc, getRGB(formR->stRGB));
			setBkColor(dc, getRGB(formR->spRGB));
		}
		else
		{
			setBkColor(dc);
			setTextColor(dc);
		}
		bool	bChecked = ii == formR->checked;
		style = DT_SINGLELINE|DT_VCENTER;

		tRc1 = tRc;
		if (scroll && ii == last - 2)
			twidth = tRc1.Width();
		if (scroll && ii == last - 1)
			tRc1.right += (int)(float)(pRc.Height() * 2.0 / 3.0) * 2;
		 	
		if (m_formItem->GetForm(m_fN)->properties & PR_SORT && visible > (int)formR->type - 1)
			break;
		
		if (!str2.IsEmpty())
		{
			drawPNG(dc, tRc1, str2);
			dc->SetBkMode(TRANSPARENT);
		}
		else
		{
			if (bChecked)
				draw3dRoundRect(dc, tRc1, AL_TOP, 3, getRGB(formR->spRGB));
			else
			{
				tRc1.top += 2;
				draw3dRoundRect(dc, tRc1, AL_TOP, 5, getRGB(formR->pRGB));
				tRc1 = tRc;
				tRc1.top = tRc1.bottom - 1;
				dc->FillSolidRect(&tRc1, clBOTTOM);
			}
		}				

		if (scroll && ii == last-1)
		{
			style |= DT_CENTER;
		}
		else
		{
			style |= DT_CENTER;
		}
										
		tRc1 = tRc;
		tRc1.DeflateRect(4, 1);
		editAmpersand(str);

		if (scroll && ii == last - 1)
			tRc1.right += (int)(float)(pRc.Height() * 2.0 / 3.0) * 2;

		if (ii == formR->checked)
			setBoldFont(dc, true);
		else
			setBoldFont(dc, false);
		dc->SetBkMode(TRANSPARENT);

       	 	dc->DrawText(str, tRc1, style);

		visible++;
	}

	if (!cell.IsRectEmpty())
	{
		setPen(dc, getRGB(formR->spRGB));
		brush = setBrush(dc, getRGB(formR->spRGB));
		dc->SetPolyFillMode(ALTERNATE);
		pts[0].x = cell.left;			pts[0].y = cell.bottom;
		pts[1].x = cell.left;			pts[1].y = cell.top;
		if (scroll && formR->checked == last-1)
		{
			pts[2].x = cell.right;		pts[2].y = cell.top;
			pts[3].x = cell.right;		pts[3].y = cell.bottom;
		}
		else
		{
			pts[2].x = cell.right-(cell.Height()/2);pts[2].y = cell.top;
			pts[3].x = pts[2].x + cell.Height();	pts[3].y = cell.bottom;
		}
		dc->Polygon(pts, 4);

		setPen(dc, clBLACK);
		brush = setBrush(dc, clBLACK);
		dc->SetPolyFillMode(ALTERNATE);
		if (scroll && formR->checked == last-1)
		{
			pts[3] = pts[1];
			dc->Polygon(pts, 4);
		}
		else
		{
			pts[4] = pts[2];
			pts[5] = pts[1];
			dc->Polygon(pts, 6);
		}

		setPen(dc, clYWHITE);
		brush = setBrush(dc, clYWHITE);
		dc->SetPolyFillMode(ALTERNATE);
		pts[0].x++;		pts[0].y--;
		pts[1].x++;		pts[1].y++;
		pts[2].x--;		pts[2].y++;
		if (scroll && formR->checked == last-1)
		{
			pts[3] = pts[1];
			dc->Polygon(pts, 4);
		}
		else
		{
			pts[3].x--;		pts[3].y++;
			pts[4] = pts[2];
			pts[5] = pts[1];
			dc->Polygon(pts, 6);
		}

		setTextColor(dc, getRGB(formR->stRGB));
		setBkColor(dc, getRGB(formR->spRGB));
		cells = _T(" ") + cells;
		cell.DeflateRect(3, 0);
		cell.top+=2;
		editAmpersand(cells);

		if (!str2.IsEmpty())
		{
			drawPNG(dc, cell, str2);
			dc->SetBkMode(TRANSPARENT);
		}

       	 	dc->DrawText(cells, cell, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	}

	if (scroll)
	{
		tRc = pRc;
		if (vertical)
		{
			tRc.top    = pRc.bottom;
			tRc.bottom = fRc.bottom;
		}
		else
		{
			tRc.bottom--;
			tRc.left  = pRc.right;
			tRc.right = fRc.right;
		}
		drawScroll(dc, tRc);
	}
}

void CTlTab::draw2Rect(CDC *dc, CRect rc, COLORREF color1, COLORREF color2)
{
	if (rc.Width() == 2)
	{
		CRect tRc = rc;
		dc->FillSolidRect(tRc, color1);
		tRc.left++;
		dc->FillSolidRect(tRc, color2);
	}
	else
	{
		CRect tRc = rc;
		dc->FillSolidRect(tRc, color1);
		tRc.top++;
		dc->FillSolidRect(tRc, color2);
	}
}

void CTlTab::draw3dRoundRect(CDC *dc, CRect rc, int align, int depth, COLORREF borderColor)
{
	CRect tRc = rc;
	setBrush(dc, clWHITE);
	setPen(dc, clBOTTOM);
	dc->RoundRect(rc, CPoint(depth, depth));
	
	switch (align)
	{
	case AL_TOP:
		tRc.DeflateRect(2, 2, 1, 1);
		setBrush(dc, borderColor);
		setPen(dc, borderColor);
		dc->RoundRect(tRc, CPoint(depth - 2, depth - 2));

		tRc = rc;
		tRc.right = tRc.left + 2;
		tRc.top = tRc.bottom - depth;
		draw2Rect(dc, tRc, clBOTTOM, clWHITE);

		tRc = rc;
		tRc.left = tRc.right - 2;
		tRc.top = tRc.bottom - depth;
		draw2Rect(dc, tRc, clWHITE, clBOTTOM);

		tRc = rc;
		tRc.top = tRc.bottom - depth;
		tRc.DeflateRect(2, 0, 1, 0);
		dc->FillSolidRect(tRc, borderColor);
		break;

	case AL_RIGHT:
		tRc.DeflateRect(1, 2, 2, 1);
		setBrush(dc, borderColor);
		setPen(dc, borderColor);
		dc->RoundRect(tRc, CPoint(depth - 2, depth - 2));

		tRc = rc;
		tRc.bottom = tRc.top + 2;
		tRc.right = tRc.left + depth;
		draw2Rect(dc, tRc, clBOTTOM, clWHITE);

		tRc = rc;
		tRc.top = tRc.bottom - 2;
		tRc.right = tRc.left + depth;
		draw2Rect(dc, tRc, clWHITE, clBOTTOM);

		tRc = rc;
		tRc.right = tRc.left + depth;
		tRc.DeflateRect(0, 2, 0, 1);
		dc->FillSolidRect(tRc, borderColor);
		break;

	case AL_BOTTOM:
		tRc.DeflateRect(1, 1, 2, 2);
		setBrush(dc, borderColor);
		setPen(dc, borderColor);
		dc->RoundRect(tRc, CPoint(depth - 2, depth - 2));

		tRc = rc;
		tRc.right = tRc.left + 2;
		tRc.bottom = tRc.top + depth;
		draw2Rect(dc, tRc, clBOTTOM, clWHITE);

		tRc = rc;
		tRc.left = tRc.right - 2;
		tRc.bottom = tRc.top + depth;
		draw2Rect(dc, tRc, clWHITE, clBOTTOM);

		tRc = rc;
		tRc.bottom = tRc.top + depth;
		tRc.DeflateRect(1, 0, 2, 0);
		dc->FillSolidRect(tRc, borderColor);
		break;

	case AL_LEFT:
		tRc.DeflateRect(2, 1, 1, 2);
		setBrush(dc, borderColor);
		setPen(dc, borderColor);
		dc->RoundRect(tRc, CPoint(depth - 2, depth - 2));

		tRc = rc;
		tRc.bottom = tRc.top + 2;
		tRc.left = tRc.right - depth;
		draw2Rect(dc, tRc, clBOTTOM, clWHITE);

		tRc = rc;
		tRc.top = tRc.bottom - 2;
		tRc.left = tRc.right - depth;
		draw2Rect(dc, tRc, clWHITE, clBOTTOM);

		tRc = rc;
		tRc.left = tRc.right - depth;
		tRc.DeflateRect(0, 1, 0, 2);
		dc->FillSolidRect(tRc, borderColor);
		break;
	}
}

void CTlTab::drawVText(CDC* dc, CString tabs, CRect rect, UINT style)
{
	CRect	rc;
	CSize	size;
	float	dy;
	CStringArray	arrStr;

	getString(tabs, arrStr);
	size = dc->GetOutputTextExtent(_T("A"));

	rc = rect;
	dy = (float)(rect.top + TAB_EXTRA/2);
	for (int ii = 0; ii < arrStr.GetSize(); ii++)
	{
		if (dy >= rect.bottom)
			break;

		rc.top = (int)dy;
		dy += (float)size.cy;
		rc.bottom = (int)dy;
		dc->DrawText(arrStr.GetAt(ii), &rc, style);
		dy += (float)(size.cy * 0.5);
	}
}

void CTlTab::drawPage(CDC* dc, CRect tabRc, CRect rect, bool left)
{
	CRect	rc;
	CPoint	pts[6];
	CBrush*	brush;
	int	gap = 1;

	rc = rect;
	rc.top++;

	brush = setBrush(dc, getRGB(m_formItem->GetForm(m_fN)->spRGB));
	dc->FillRect(&rc, brush);

	setPen(dc, clYWHITE);
	brush = setBrush(dc, clYWHITE);
	dc->SetPolyFillMode(ALTERNATE);
	if (left)
	{
		pts[0].x = rect.left;		pts[0].y = rect.top;	// left
		pts[1].x = rect.left;		pts[1].y = rect.top;
		pts[2].x = rect.left;		pts[2].y = rect.bottom-gap;
		pts[3].x = rect.left;		pts[3].y = pts[2].y;
		dc->Polygon(pts, 4);
	}

	pts[0].x = rect.right-gap;	pts[0].y = rect.top;		// top
	pts[1].x = tabRc.right;		pts[1].y = rect.top;
	dc->Polygon(pts, 2);

	pts[0].x = rect.right-gap;	pts[0].y = rect.top;		// bottom_right
	pts[1].x = pts[0].x;		pts[1].y = rect.bottom-gap;
	pts[2].x = rect.left;		pts[2].y = pts[1].y;
	pts[3].x = rect.left;		pts[3].y = rect.bottom-gap;
	pts[4].x = rect.right-gap;	pts[4].y = rect.bottom-gap;
	pts[5].x = rect.right-gap;	pts[5].y = rect.top;
	setPen(dc, clEDGE);
	brush = setBrush(dc, clEDGE);
	dc->Polygon(pts, 6);
}

void CTlTab::drawScroll(CDC* dc, CRect scrRc)
{
	CRect	rc  = scrRc;
	int	gap = rc.Width() / 2;
	CBrush*	brush = setBrush(dc, clLGRAY);

	rc.top   = rc.bottom - gap;
	rc.right = rc.left + gap+1;
        dc->FillRect(rc, brush);
      	dc->DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLLEFT);

	rc.left  = rc.right;
	rc.right = rc.left + gap;
	dc->FillRect(rc, brush);
	dc->DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLRIGHT);
}

void CTlTab::addPage(int nPage)
{
	struct	_formR	*formR = m_formItem->GetForm(m_fN);
	if (nPage >= 0)
	{
		CString temp = m_formItem->GetFormStr(m_fN, _T("dat")), tmp = _T("");
		CString temp2 = m_formItem->GetFormStr(m_fN, _T("str")), tmp2 = _T("");
		formR->keys++;
		for (int jj = 0; jj < (int)formR->keys; jj++)
		{
			if (nPage == jj)
			{
				tmp += m_dat;
				tmp += _T('\t');

				tmp2 += m_str;
				tmp2 += _T('\t');
				continue;
			}
			tmp += parse(temp, _T("\t"));
			tmp += _T('\t');

			tmp2 += parse(temp2, _T("\t"));
			tmp2 += _T('\t');
		}
		m_formItem->SetFormStr(m_fN, _T("dat"), tmp);
		m_formItem->SetFormStr(m_fN, _T("str"), tmp2);
		
		if (nPage >= (int)formR->keys)
			formR->checked = 0;
		formR->checked = nPage;
	}
}

void CTlTab::deletePage(int nPage)
{
	struct _formR *formR = m_formItem->GetForm(m_fN);
	if (nPage >= 0)
	{
		CString temp = m_formItem->GetFormStr(m_fN, _T("dat")), tmp = _T("");
		CString temp2 = m_formItem->GetFormStr(m_fN, _T("str")), tmp2 = _T("");
		for (int jj = 0; jj < (int)formR->keys; jj++)
		{
			if (nPage == jj)
			{
				m_dat = parse(temp, _T("\t"));
				m_str = parse(temp2, _T("\t"));
				continue;
			}
			tmp += parse(temp, _T("\t"));
			tmp += _T('\t');

			tmp2 += parse(temp2, _T("\t"));
			tmp2 += _T('\t');
		}
		m_formItem->SetFormStr(m_fN, _T("dat"), tmp);
		m_formItem->SetFormStr(m_fN, _T("str"), tmp2);
		if (formR->keys > 0)
			formR->keys--;

		if (nPage >= (int)formR->keys)
			formR->checked = 0;
	}
}

bool CTlTab::isScrolled(CDC* dc, int tabN, CString data, int& index, int& width)
{
	CString	tabs;
	CRect	rect;
	int	count;
	bool	scroll = false;

	index = width = 0;
	rect  = getRect();
	count = max(tabN, 1);
	for (int ii = 0; ii < count; ii++)
	{
		if (!tabN)
			tabs = m_name;
		else
			tabs = parse(data, _T("\t"));

		width += getSize(dc, tabs);

		if (!index && (width > rect.Width() - (int)(float)(m_formItem->GetForm(m_fN)->size * 2.0 / 3.0) * 2))
			index = ii+1;
		if (width > rect.Width())
		{
			scroll = true;
			break;
		}
	}

	index = (index == 0) ? count : index;
	return scroll;
}

bool CTlTab::isScrolled(CDC* dc, int tabN, CString data, CString info, int& index, int& width, bool vertical)
{
	CString	tabs, infos;
	CRect	rect;
	int	count;
	bool	scroll = false;
	CString str, item;

	index = width = 0;
	rect  = getRect();
	count = max(tabN, 1);
	for (int ii = 0; ii < count; ii++)
	{
		if (!tabN)
			tabs = m_name;
		else
		{
			infos = parse(info, _T("\t"));
			tabs  = parse(data, _T("\t"));
		}

		if (infos.GetAt(0) == '1')
			width += getSize(dc, tabs, vertical);

		if (vertical)
		{
			if (!index && (width > rect.Height() - (int)(rect.Width() * 0.5)))
				index = ii+1;
			if (width > rect.Height())
			{
				scroll = true;
				break;
			}
		}
		else
		{
			if (!index && (width > rect.Width() - (int)(float)(rect.Height() * 2.0 / 3.0) * 2))
				index = ii+1;

			if (m_formItem->GetForm(m_fN)->properties & PR_SORT)
			{
				if (getLastVisibleIndex(false) > (int)m_formItem->GetForm(m_fN)->type && width > rect.Width())
				{
					scroll = true;
					break;
				}
			}
			else
			{
				if (width > rect.Width())
				{
					scroll = true;
					break;
				}
			}
		}
	}

	if (!scroll)
		index = getLastVisibleIndex(true)+1;
	index = (index == 0) ? count : index;
	return scroll;
}

int CTlTab::getSize(CDC* dc, CString tabs, bool vertical)
{
	CSize	size;
	CRect	rc = getRect();
	int	width, cnt;

	if (vertical)
	{
		CStringArray arrStr;
		size = dc->GetOutputTextExtent(_T("A"));
		getString(tabs, arrStr);

		if (m_formItem->GetForm(m_fN)->properties & PR_SORT)
			return rc.Width() / (int)m_formItem->GetForm(m_fN)->type;
		else
			return (size.cy * (int)arrStr.GetSize() + (int)(size.cy * 0.5 * ((int)arrStr.GetSize()-1)) + TAB_EXTRA);
	}
	
	cnt = (int)m_formItem->GetForm(m_fN)->type;
	if (m_formItem->GetForm(m_fN)->properties & PR_SORT && cnt > 0)
		width = rc.Width() / cnt;
	else
	{
		size = dc->GetOutputTextExtent(tabs);
		width = (size.cx + TAB_EXTRA);
	}
	return width;
}

void CTlTab::getString(CString string, CStringArray& arrStr)
{
	arrStr.RemoveAll();
	for (int ii = 0; ii < string.GetLength(); ii++)
	{
		if (string[ii] & 0x80)
		{
			arrStr.Add(string.Mid(ii, 2));
			ii++;
		}
		else
			arrStr.Add(string.Mid(ii, 1));
	}
}

int CTlTab::getLastVisibleIndex(bool last)
{
	CString	info;
	CString	item;
	int	index = 0;

	info = m_formItem->GetFormStr(m_fN, _T("str"));
	if (!info.IsEmpty())
	{
		for (int ii = 0; ii < (int)m_formItem->GetForm(m_fN)->keys; ii++)
		{
			item = parse(info, _T("\t"));
			if (!item.IsEmpty() && item.GetAt(0) == '1')
			{
				if (last)
					index = ii;
				else
					index++;
			}
		}
	}

	return index;
}
