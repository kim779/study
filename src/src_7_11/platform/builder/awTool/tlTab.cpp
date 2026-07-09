// tlTab.cpp: implementation of the CtlTab class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlTab.h"

#include "../childview.h"
#include "../mapform.h"
#include "../resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	TAB_EXTRA	14
#define	TAB_EXTRA2	25

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtlTab::CtlTab(CChildView* view, UINT id)
	: CtlRect(view)
{
	switch (id)
	{
	case ID_TOOL_TAB:
		m_type = taTAB;
		m_name = _T("Tab");
		break;
	case ID_TOOL_USERTAB:
		m_type = taUTAB;
		m_name = _T("UserTab");
		break;
	default:
		m_type = taNONE;
		break;
	}
	m_pArrowWnd = NULL;
	m_selected = -1;
}

CtlTab::~CtlTab()
{
}

void CtlTab::drawBegin(CPoint point)
{
	if (!m_view->m_bSelect)
	{
		CtlRect::drawBegin(point);
		return;
	}
	m_fN = m_view->m_selF;
	if (m_type == taUTAB)
		m_selected = SelectableUTab(point, m_rect, m_toPt);
	else
		m_selected = Selectable(point, m_rect, m_toPt);
	m_view->InvalidateRect(getRect());
}

void CtlTab::drawTo(CPoint point)
{
	if (!m_view->m_bSelect)
	{
		CtlRect::drawTo(point);
		return;
	}

	m_fN = m_view->m_selF;
	if (m_type == taUTAB)
		SelectableUTab(point, m_rect, m_toPt, false);
	else
		Selectable(point, m_rect, m_toPt, false);
	m_rect.InflateRect(0, 0, 100, 0);

	if (m_type == taTAB)
		m_rect.bottom = m_rect.top + m_pFormItem->getFormPoint(m_fN)->size;
	
	if (m_rect.PtInRect(point))
	{
		if (m_pArrowWnd && m_selected >= 0)
		{
			m_pArrowWnd->SetWindowPos(NULL, m_toPt.x - 5, m_toPt.y - 6, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
			SetCursor(AfxGetApp()->LoadCursor(IDR_TREEFILECTRL_DROPCOPY));
		}
	}
	else
	{
		if (m_pArrowWnd && m_selected >= 0)
		{
			m_pArrowWnd->ShowWindow(SW_HIDE);
			SetCursor(AfxGetApp()->LoadCursor(IDR_TREEFILECTRL_NO_DROPCOPY));
		}
	}
}

void CtlTab::drawEnd()
{
	if (m_pArrowWnd)
	{
		m_pArrowWnd->DestroyWindow();
		delete m_pArrowWnd;
		m_pArrowWnd = NULL;
	}
	if (!m_view->m_bSelect)
	{
		struct _formR *formR = m_pFormItem->getFormPoint(m_fN);
		CtlRect::drawEnd();
		
		switch (m_type)
		{
		case taTAB:
			formR->size = m_fheight + 12;
			if (m_rect.Height() < formR->size)
				m_rect.bottom = m_rect.top + formR->size;
			if (m_rect.Width() < 10 || m_rect.Height() < 50)
			{
				m_rect.right  = m_rect.left + getDefaultWidth("TAB", 300);
				m_rect.bottom = m_rect.top + getDefaultHeight("TAB", 150 + formR->size);
			}
			break;
			
		case taUTAB:
			m_rect.bottom = m_rect.top + getDefaultHeight("UTAB", m_fheight + 12);
			if (m_rect.Width() < 10 || m_rect.Height() < 10)
				m_rect.right  = m_rect.left + getDefaultWidth("UTAB", 100);
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
		
		switch (m_type)
		{
		case taTAB:
			formR->kind = FM_TAB;
			formR->type = TAB_BORDER;
			formR->keys = 1;
			m_pFormItem->setFormStr(m_fN, "dat", "Tab0\t");

			formR->pRGB = getDefaultPRGB("TAB", basPRGB);
			formR->tRGB = getDefaultTRGB("TAB", 232);
			break;
			
		case taUTAB:
			formR->kind = FM_UTAB;
			formR->type = TAB_BORDER;
			formR->alignment = AL_TOP;
			formR->keys = 1;
			m_pFormItem->setFormStr(m_fN, "dat", "Tab0\t");
			m_pFormItem->setFormStr(m_fN, "str", "1:0\t");
			
			formR->pRGB = getDefaultPRGB("UTAB", basPRGB);
			formR->tRGB = getDefaultTRGB("UTAB", 232);
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
			struct _formR *formR = m_pFormItem->getFormPoint(m_fN);
			int sel = (m_type == taUTAB?SelectableUTab(m_endpt, m_rect, m_toPt, false):Selectable(m_endpt, m_rect, m_toPt, false));
			
			m_rect.InflateRect(0, 0, 100, 0);
			if (m_type == taTAB)
				m_rect.bottom = m_rect.top + m_pFormItem->getFormPoint(m_fN)->size;
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

void CtlTab::Draw(CDC* dc, int fN)
{
	int	ndc = dc->SaveDC();

	CTOOL::Draw(dc, fN);
	if (m_pFormItem->getFormPoint(m_fN)->spRGB == 0 && m_pFormItem->getFormPoint(m_fN)->stRGB == 0)
	{
		m_pFormItem->getFormPoint(m_fN)->spRGB = (m_pFormItem->getFormPoint(m_fN)->type == TAB_NONE) ? 193 : m_pFormItem->getFormPoint(m_fN)->pRGB;
		m_pFormItem->getFormPoint(m_fN)->stRGB   = (m_pFormItem->getFormPoint(m_fN)->type == TAB_NONE) ? 203 : m_pFormItem->getFormPoint(m_fN)->tRGB;
	}

	switch (m_type)
	{
	case taTAB:
		drawTAB(dc);		break;
	case taUTAB:
		drawUserTAB(dc);	break;
	default:
		break;
	}

	dc->RestoreDC(ndc);
}

void CtlTab::drawTAB(CDC* dc)
{
	CPoint	pts[20];
	CString	cells, item;
	UINT	style;
	CBrush*	brush;
	CString	data, tmpS;
	bool	scroll = false;
	CRect	fRc,  pRc,  tRc,  tRc1,  rc,  cell;
	int	last,  dx,  jj,  cx = 2;

	pRc = fRc = getRect();
	pRc.bottom = pRc.top + m_pFormItem->getFormPoint(m_fN)->size;
	fRc.top    = pRc.bottom-1;

	setFont(dc);

	data = m_pFormItem->getFormStr(m_fN, "dat");

	scroll = isScrolled(dc, (int)m_pFormItem->getFormPoint(m_fN)->keys, data, last, dx);
	if (scroll)
	{
		pRc.right -= (int)(float)(m_pFormItem->getFormPoint(m_fN)->size * 2.0 / 3.0) * 2;
		last = min((int)m_pFormItem->getFormPoint(m_fN)->keys, last + m_pFormItem->getFormPoint(m_fN)->scrollPos);
	}
	else
	{
		pRc.right = pRc.left + dx;
		last = max((int)m_pFormItem->getFormPoint(m_fN)->keys, 1);
		m_pFormItem->getFormPoint(m_fN)->scrollPos = 0;
	}

	dx = pRc.left;
	cell.SetRectEmpty();
	
	for (int ii = 0; ii < last; ii++)
	{
		if (!m_pFormItem->getFormStr(m_fN, "dat").IsEmpty())
			item = Parser(data, "\t");
		if (m_pFormItem->getFormPoint(m_fN)->scrollPos > ii)
			continue;
		item = (m_pFormItem->getFormPoint(m_fN)->keys == 0) ? m_name : item;

		tRc = pRc;
		tRc.left = dx;
		if (!scroll || ii < last-1)
		{
			dx += getSize(dc, item);
			tRc.right = dx;
		}

		if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
		{
			setTextColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->stRGB));
			setBkColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
		}
		else
		{
			setBkColor(dc);
			setTextColor(dc);
		}
		style = DT_SINGLELINE | DT_VCENTER;
		switch (m_pFormItem->getFormPoint(m_fN)->type)
		{
		case TAB_NONE:
			tRc1 = tRc;
			tRc1.left+=2;	tRc1.top+=2;
			pts[0].x = tRc1.left;		pts[0].y = tRc1.bottom-1;
			pts[1].x = tRc1.left;		pts[1].y = tRc1.top+cx;
			pts[2].x = tRc1.left+cx;	pts[2].y = tRc1.top;
			pts[3].x = tRc1.right-cx;	pts[3].y = tRc1.top;
			pts[4].x = tRc1.right;		pts[4].y = pts[1].y;
			pts[5].x = tRc1.right;		pts[5].y = tRc1.bottom-1;

			setPen(dc, clEDGE);
			brush = setBrush(dc, clEDGE);
			dc->SetPolyFillMode(ALTERNATE);
			dc->Polygon(pts, 6);

			for (jj = 0; jj < 6; jj++)
			{
				pts[jj].x--;
				pts[jj].y--;
			}
			pts[0].y = tRc.bottom;
			pts[5].y = pts[0].y;
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
			{
				setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
			}
			else
			{
				setPen(dc);
				brush = setBrush(dc);
			}
			dc->SetPolyFillMode(ALTERNATE);
			dc->Polygon(pts, 6);

			rc = tRc;
			rc.top = rc.bottom-1;

			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
				brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
			else
				brush = setBrush(dc, clWHITE);

			dc->FillRect(&rc, brush);	// bottom

			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
				brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
			else
				brush = setBrush(dc);
			tRc1.bottom--;
			break;
			
		case TAB_OBLIQUE:
			{
				bool bChecked = ii == m_pFormItem->getFormPoint(m_fN)->checked;
				tRc1 = tRc;
				dc->SetPolyFillMode(ALTERNATE);
				pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
				pts[1].x = tRc.left;	pts[1].y = tRc.top+3;
				pts[2].x = tRc.left+3;	pts[2].y = tRc.top;
				pts[3].x = tRc.right-4;	pts[3].y = tRc.top;
				pts[4].x = tRc.right-1;	pts[4].y = tRc.top+3;
				pts[5].x = tRc.right-1;	pts[5].y = tRc.bottom;
				pts[6].x = pts[5].x-1;	pts[6].y = tRc.bottom;
				pts[7].x = pts[6].x;	pts[7].y = tRc.top+3;
				pts[8].x = pts[3].x;	pts[8].y = pts[3].y+1;
				pts[9].x = pts[2].x;	pts[9].y = pts[8].y;
				pts[10].x = pts[1].x+1;	pts[10].y = pts[1].y;
				pts[11].x = pts[0].x+1;	pts[11].y = pts[0].y;
				setPen(dc, getRGB(clBLACK));
				setBrush(dc, getRGB(clBLACK));
				dc->Polygon(pts, 12);
				
				if (scroll && ii == m_pFormItem->getFormCount() - 1)
				{
					style |= DT_LEFT;
					tmpS = _T(" ") + tmpS;
				}
				else
					style |= DT_CENTER;
				
				if (!bChecked)
				{
					pts[0].x++;
					pts[1].x++;
					pts[2].y++;
					pts[3].y++;
					pts[4].x = pts[8].x;	pts[4].y = pts[8].y+1;
					pts[5].x = pts[9].x;	pts[5].y = pts[9].y+1;
					pts[6].x = pts[10].x+1;	pts[6].y = pts[10].y;
					pts[7].x = pts[11].x+1;	pts[7].y = pts[11].y;
					setPen(dc, getRGB(clYWHITE));
					setBrush(dc, getRGB(clYWHITE));
					dc->Polygon(pts, 8);
					
					pts[0] = pts[7];
					pts[1] = pts[6];
					pts[2] = pts[5];
					pts[3] = pts[4];
					pts[4].x = tRc.right-2;	pts[4].y = tRc.top+3;
					pts[5].x = pts[4].x;	pts[5].y = tRc.bottom;
					setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
					setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
					dc->Polygon(pts, 6);
					
					pts[0].x = pts[3].x+1;	pts[0].y = tRc.top+2;
					pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y+1;
					setPen(dc, getRGB(clEDGE));
					setBrush(dc, getRGB(clEDGE));
					dc->Polygon(pts, 2);
					
					pts[0].x = pts[1].x;	pts[0].y++;
					pts[1].x = pts[0].x;	pts[1].y = tRc.bottom;
					dc->Polygon(pts, 2);
					
					pts[0].x = tRc.left;	pts[0].y = tRc.bottom-2;
					pts[1].x = tRc.right-1;	pts[1].y = tRc.bottom-2;
					pts[2].x = tRc.right-1;	pts[2].y = tRc.bottom;
					pts[3].x = tRc.left;	pts[3].y = tRc.bottom;
					setPen(dc, getRGB(clYWHITE));
					setBrush(dc, getRGB(clYWHITE));
					dc->Polygon(pts, 4);
					
					pts[2].y-=2;
					pts[3].y-=2;
					setPen(dc, getRGB(clBOTTOM));
					setBrush(dc, getRGB(clBOTTOM));
					dc->Polygon(pts, 4);
					
					tRc1.DeflateRect(2, 2, 2, 2);
					break;
				}
				
				pts[0].x++;
				pts[1].x++;		pts[1].y++;
				pts[2].x += 1;		pts[2].y++;
				pts[3].x--;		pts[3].y++;
				pts[4].x--;		pts[4].y++;
				pts[5].x = pts[4].x;	pts[5].y = pts[4].y+1;
				pts[6].x = pts[3].x;	pts[6].y = pts[3].y+1;
				pts[7].x = pts[2].x;	pts[7].y = pts[2].y+1;
				pts[8].x = pts[1].x+1;	pts[8].y = pts[1].y;
				pts[9].x = pts[8].x;	pts[9].y = pts[0].y;
				setPen(dc, getRGB(clYWHITE));
				setBrush(dc, getRGB(clYWHITE));
				dc->Polygon(pts, 10);
				
				pts[0] = pts[9];
				pts[1] = pts[8];
				pts[2] = pts[7];
				pts[3].x = pts[6].x;	pts[3].y = pts[6].y;
				pts[4].x = pts[3].x+2;	pts[4].y = pts[3].y+2;
				pts[5].x = tRc.right-3;	pts[5].y = pts[0].y;
				setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				dc->Polygon(pts, 6);
				
				pts[0].x--;		pts[0].y = pts[1].y-1;
				pts[1].x = tRc.left+3;	pts[1].y = pts[2].y-1;
				dc->Polygon(pts, 2);
				
				pts[0].x = pts[3].x+1;	pts[0].y = pts[1].y;
				pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y+1;
				dc->Polygon(pts, 2);
				
				pts[0].x = pts[1].x+1;	pts[0].y = pts[1].y+1;
				pts[1].x = pts[0].x;	pts[1].y = tRc.bottom;
				setPen(dc, getRGB(clEDGE));
				setBrush(dc, getRGB(clEDGE));
				dc->Polygon(pts, 2);
				
				pts[0].x = tRc.left;	pts[0].y = tRc.bottom-1;
				pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y;
				setPen(dc, getRGB(clYWHITE));
				setBrush(dc, getRGB(clYWHITE));
				dc->Polygon(pts, 2);
				
				pts[0].x = tRc.right-2;
				pts[1].x = pts[0].x+2;
				dc->Polygon(pts, 2);
				
				pts[0].x++;		pts[0].y--;
				pts[1].x = tRc.right;	pts[1].y = pts[0].y+1;
				setPen(dc, getRGB(clBOTTOM));
				setBrush(dc, getRGB(clBOTTOM));
				dc->Polygon(pts, 2);
				
				tRc1.DeflateRect(2, 2, 1, 0);
			}
			break;

		case TAB_RECT:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
			{
				setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
			}
			else
			{
				setPen(dc);
				brush = setBrush(dc);
			}
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x = tRc.left;			pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;			pts[1].y = tRc.top;
			if (scroll && ii == last-1)
			{
				pts[2].x = tRc.right;		pts[2].y = tRc.top;
				pts[3].x = tRc.right;		pts[3].y = tRc.bottom;
				dc->Polygon(pts, 4);
			}
			else
			{
				pts[2].x = tRc.right-(tRc.Height()/2);	pts[2].y = tRc.top;
				pts[3].x = tRc.right;			pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4].x = tRc.right;			pts[4].y = tRc.bottom;
				dc->Polygon(pts, 5);
			}

			setPen(dc, clBLACK);
			brush = setBrush(dc, clBLACK);
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x = tRc.left;			pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;			pts[1].y = tRc.top;
			if (scroll && ii == last-1)
			{
				pts[2] = tRc.right;		pts[2].y = tRc.top;
				pts[3] = pts[1];
				dc->Polygon(pts, 4);
			}
			else
			{
				pts[2].x = tRc.right-(tRc.Height()/2);	pts[2].y = tRc.top;
				pts[3].x = tRc.right;			pts[3].y = tRc.top + (tRc.Height()/2);
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
			if (scroll && ii == last-1)
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

			setPen(dc, clBLACK);
			brush = setBrush(dc, clBLACK);
			dc->SetPolyFillMode(ALTERNATE);
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
			{
				pts[0].x = tRc.right;	pts[0].y = tRc.bottom;
				pts[1].x = tRc.right;	pts[1].y = tRc.top + (tRc.Height()/2);
				dc->Polygon(pts, 2);
			}
			else
			{
				pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
				pts[1].x = tRc.right;	pts[1].y = tRc.bottom;
				if (scroll && ii == last-1)
					dc->Polygon(pts, 2);
				else
				{
					pts[2].x = tRc.right;	pts[2].y = tRc.top + (tRc.Height()/2);
					pts[3] = pts[1];
					pts[4] = pts[0];
					dc->Polygon(pts, 5);
				}
			}

			setPen(dc, clEDGE);
			brush = setBrush(dc, clEDGE);
			dc->SetPolyFillMode(ALTERNATE);
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
			{
				pts[0].x--;
				pts[1].x--;		pts[1].y++;
				dc->Polygon(pts, 2);
			}
			else
			{
				pts[0].x++;		pts[0].y--;
				pts[1].x--;		pts[1].y--;
				if (scroll && ii == last-1)
					dc->Polygon(pts, 2);
				else
				{
					pts[2].x--;		pts[2].y++;
					pts[3] = pts[1];
					pts[4] = pts[0];
					dc->Polygon(pts, 5);
				}
			}
			tRc1 = tRc;
			tRc1.top+=2;
			style |= DT_LEFT;
			item = _T(" ") + item;
			break;

		case TAB_BORDER:
		default:
			{
				bool bChecked = ii == m_pFormItem->getFormPoint(m_fN)->checked;
				tRc1 = tRc;
				
				if (scroll && ii == last-1)
				{
					style |= DT_LEFT;
					item = _T(" ") + item;
				}
				else
				{
					style |= DT_CENTER;
					/*tRc1 = tRc;
					tRc1.top++;
					tRc1.left = tRc1.right - 1;
					brush = setBrush(dc, clBOTTOM);
					dc->FillRect(&tRc1, brush);*/	// right
				}
				if (bChecked)
					draw3dRoundRect(dc, tRc1, AL_TOP, 3, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
				else
				{
					tRc1.top += 2;
					draw3dRoundRect(dc, tRc1, AL_TOP, 5, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
					tRc1 = tRc;
					tRc1.top = tRc1.bottom - 2;
					dc->FillSolidRect(&tRc1, clBOTTOM);	
				}
				
				tRc1 = tRc;
				tRc1.top = tRc1.bottom - 1;
				
				if (bChecked)
					brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				else
					brush = setBrush(dc, clWHITE);
				
				dc->FillRect(&tRc1, brush);		// bottom
				
				tRc1 = tRc;
				tRc1.DeflateRect(2, (bChecked?2:4), 1, 1);
				if (bChecked)
				{
					brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
					dc->FillRect(&tRc1, brush);		// text rect
				}
				/*else
					brush = setBrush(dc);*/
				
			}
			break;
		}

		tRc1.DeflateRect(3, 0);
		editAmpersand(item);
		if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
			setBoldFont(dc, true);
		else
			setBoldFont(dc, false);
		dc->SetBkMode(TRANSPARENT);
       	 	dc->DrawText(item, tRc1, style);
	}

	if (!cell.IsRectEmpty())
	{
		setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
		brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
		dc->SetPolyFillMode(ALTERNATE);
		pts[0].x = cell.left;			pts[0].y = cell.bottom;
		pts[1].x = cell.left;			pts[1].y = cell.top;
		if (scroll && m_pFormItem->getFormPoint(m_fN)->checked == last-1)
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
		if (scroll && m_pFormItem->getFormPoint(m_fN)->checked == last-1)
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
		if (scroll && m_pFormItem->getFormPoint(m_fN)->checked == last-1)
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

		setTextColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->stRGB));
		setBkColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
		cells = _T(" ") + cells;
		cell.DeflateRect(3, 0);
		cell.top+=2;
		editAmpersand(cells);
       	 	dc->DrawText(cells, cell, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	}

	if (scroll)
	{
		tRc = pRc;
		tRc.bottom--;
		tRc.left  = pRc.right;
		tRc.right = fRc.right;
		drawScroll(dc, tRc);
	}
	drawPage(dc, pRc, fRc, (m_pFormItem->getFormPoint(m_fN)->type != TAB_NONE) ? true : false);
}

void CtlTab::drawUserTAB(CDC* dc)
{
	CPoint	pts[20];
	UINT	style;
	CBrush*	brush;
	CString	str, item, cells, tmpS;
	CString	data, info;
	bool	vertical, scroll;
	CRect	fRc, pRc, tRc, tRc1, rc, cell;
	int	count,  vcount,  cx = 2;
	int	last, len,  jj,  dx;

	fRc = pRc = getRect();
	setFont(dc);

	count = max((int)m_pFormItem->getFormPoint(m_fN)->keys, 1);
	len = vcount = 0;
	if (!m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
	{
		info = m_pFormItem->getFormStr(m_fN, "str");
		for (int ii = 0; ii < (int)m_pFormItem->getFormPoint(m_fN)->keys; ii++)
		{
			item = Parser(info, "\t");
			if (!item.IsEmpty() && item.GetAt(0) == '1')
				vcount++;
		}
	}

	vertical = scroll = false;
	if (m_pFormItem->getFormPoint(m_fN)->alignment == AL_LEFT || m_pFormItem->getFormPoint(m_fN)->alignment == AL_RIGHT)
		vertical = true;

	if (!m_pFormItem->getFormStr(m_fN, "dat").IsEmpty() && !m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
	{
		data = m_pFormItem->getFormStr(m_fN, "dat");
		info = m_pFormItem->getFormStr(m_fN, "str");
	}

	scroll = isScrolled(dc, (int)m_pFormItem->getFormPoint(m_fN)->keys, data, info, last, dx, vertical);
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
			last = min((int)m_pFormItem->getFormPoint(m_fN)->keys, last + m_pFormItem->getFormPoint(m_fN)->scrollPos);
		}
		else
			pRc.right = pRc.left + dx;

		dx = pRc.left;
	}

	cell.SetRectEmpty();
	for (int ii = 0; ii < last; ii++)
	{
		if (!m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
		{
			item = Parser(info, "\t");

			if ((!item.IsEmpty() && item.GetAt(0) != '1') && (m_pFormItem->getFormPoint(m_fN)->keys != 0))
			{
				if (!m_pFormItem->getFormStr(m_fN, "dat").IsEmpty())
					str = Parser(data, "\t");
				continue;
			}
		}

		if (!m_pFormItem->getFormStr(m_fN, "dat").IsEmpty())
			str = Parser(data, "\t");

		if (m_pFormItem->getFormPoint(m_fN)->scrollPos > ii)
			continue;

		str = (m_pFormItem->getFormPoint(m_fN)->keys == 0) ? m_name : str;

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

		if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
		{
			setTextColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->stRGB));
			setBkColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
		}
		else
		{
			setBkColor(dc);
			setTextColor(dc);
		}
		bool bChecked = ii == m_pFormItem->getFormPoint(m_fN)->checked;

		switch (m_pFormItem->getFormPoint(m_fN)->type)
		{
		case TAB_NONE:
			tRc.right--;
			tRc.bottom--;
			switch (m_pFormItem->getFormPoint(m_fN)->alignment)
			{
			case AL_LEFT:
				style = DT_CENTER|DT_TOP;
				tRc1 = tRc;
				tRc1.left++;	tRc1.top++;
				pts[0].x = tRc1.right;		pts[0].y = tRc1.top;
				pts[1].x = tRc1.left+cx;	pts[1].y = tRc1.top;
				pts[2].x = tRc1.left;		pts[2].y = tRc1.top+cx;
				pts[3].x = tRc1.left;		pts[3].y = tRc1.bottom-cx;
				pts[4].x = tRc1.left+cx;	pts[4].y = tRc1.bottom;
				pts[5].x = tRc1.right;		pts[5].y = tRc1.bottom;

				setPen(dc, clEDGE);
				brush = setBrush(dc, clEDGE);
				dc->SetPolyFillMode(ALTERNATE);
				dc->Polygon(pts, 6);

				for (jj = 0; jj < 6; jj++)
				{
					pts[jj].x--;
					pts[jj].y--;
				}
				pts[0].x = tRc.right;
				pts[5].x = pts[0].x;
				if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
				{
					setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
					brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				}
				else
				{
					setPen(dc);
					brush = setBrush(dc);
				}
				dc->SetPolyFillMode(ALTERNATE);
				dc->Polygon(pts, 6);

				rc = tRc;
				rc.left = rc.right-1;
				brush = ii != m_pFormItem->getFormPoint(m_fN)->checked? setBrush(dc, clYWHITE) : setBrush(dc);
				dc->FillRect(&rc, brush);	// right
				brush = setBrush(dc);
				tRc1.right--;
				break;

			case AL_RIGHT:
				style = DT_CENTER|DT_TOP;
				tRc1 = tRc;
				tRc1.left++;	tRc1.top++;
				pts[0].x = tRc1.left;		pts[0].y = tRc1.top;
				pts[1].x = tRc1.left;		pts[1].y = tRc1.bottom;
				pts[2].x = tRc1.right-cx;	pts[2].y = tRc1.bottom;
				pts[3].x = tRc1.right;		pts[3].y = tRc1.bottom-cx;
				pts[4].x = tRc1.right;		pts[4].y = tRc1.top+cx;
				pts[5].x = tRc1.right-cx;	pts[5].y = tRc1.top;

				setPen(dc, clEDGE);
				brush = setBrush(dc, clEDGE);
				dc->SetPolyFillMode(ALTERNATE);
				dc->Polygon(pts, 6);

				for (jj = 0; jj < 6; jj++)
				{
					pts[jj].x--;
					pts[jj].y--;
				}
				pts[0].x = tRc.left;
				pts[5].x = pts[0].x;
				if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
				{
					setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
					brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				}
				else
				{
					setPen(dc);
					brush = setBrush(dc);
				}
				dc->SetPolyFillMode(ALTERNATE);
				dc->Polygon(pts, 6);

				rc = tRc;
				rc.right = rc.left+1;
				brush = ii != m_pFormItem->getFormPoint(m_fN)->checked? setBrush(dc, clYWHITE) : setBrush(dc);
				dc->FillRect(&rc, brush);	// left
				brush = setBrush(dc);
				tRc1.left++;
				break;

			case AL_BOTTOM:
				style = DT_SINGLELINE | DT_VCENTER;
				tRc1 = tRc;
				tRc1.left++;	tRc1.top++;
				pts[0].x = tRc1.left;		pts[0].y = tRc1.top;
				pts[1].x = tRc1.left;		pts[1].y = tRc1.bottom-cx;
				pts[2].x = tRc1.left+cx;	pts[2].y = tRc1.bottom;
				pts[3].x = tRc1.right-cx;	pts[3].y = tRc1.bottom;
				pts[4].x = tRc1.right;		pts[4].y = pts[1].y;
				pts[5].x = tRc1.right;		pts[5].y = tRc1.top;
				setPen(dc, clBLACK);
				brush = setBrush(dc, clBLACK);
				dc->SetPolyFillMode(ALTERNATE);
				dc->Polygon(pts, 6);

				for (jj = 0; jj < 6; jj++)
				{
					pts[jj].x--;
					pts[jj].y--;
				}
				pts[0].y = tRc.top;
				pts[5].y = pts[0].y;
				if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
				{
					setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
					brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				}
				else
				{
					setPen(dc);
					brush = setBrush(dc);
				}
				dc->SetPolyFillMode(ALTERNATE);
				dc->Polygon(pts, 6);

				rc = tRc;
				rc.bottom = rc.top+1;
				brush = ii != m_pFormItem->getFormPoint(m_fN)->checked? setBrush(dc, clYWHITE) : setBrush(dc);
				dc->FillRect(&rc, brush);	// top
				brush = setBrush(dc);
				tRc1.top++;
				break;

			case AL_TOP:
			default:
				style = DT_SINGLELINE | DT_VCENTER;
				tRc1 = tRc;
				tRc1.left++;	tRc1.top++;
				pts[0].x = tRc1.left;		pts[0].y = tRc1.bottom;
				pts[1].x = tRc1.left;		pts[1].y = tRc1.top+cx;
				pts[2].x = tRc1.left+cx;	pts[2].y = tRc1.top;
				pts[3].x = tRc1.right-cx;	pts[3].y = tRc1.top;
				pts[4].x = tRc1.right;		pts[4].y = pts[1].y;
				pts[5].x = tRc1.right;		pts[5].y = tRc1.bottom;

				setPen(dc, clEDGE);
				brush = setBrush(dc, clEDGE);
				dc->SetPolyFillMode(ALTERNATE);
				dc->Polygon(pts, 6);

				for (jj = 0; jj < 6; jj++)
				{
					pts[jj].x--;
					pts[jj].y--;
				}
				pts[0].y = tRc.bottom;
				pts[5].y = pts[0].y;
				
				if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
				{
					setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
					brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				}
				else
				{
					setPen(dc);
					brush = setBrush(dc);
				}
				dc->SetPolyFillMode(ALTERNATE);
				dc->Polygon(pts, 6);

				rc = tRc;
				rc.top = rc.bottom-1;
				brush = ii != m_pFormItem->getFormPoint(m_fN)->checked? setBrush(dc, clYWHITE) : setBrush(dc);
				dc->FillRect(&rc, brush);	// bottom
				brush = setBrush(dc);
				tRc1.bottom--;
				break;
			}
			tRc1.left  += 3;
			tRc1.right -= 3;
			break;
			
		case TAB_OBLIQUE:
			tRc1 = tRc;
			style = DT_SINGLELINE | DT_VCENTER;
			tRc.bottom--;
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;	pts[1].y = tRc.top+3;
			pts[2].x = tRc.left+3;	pts[2].y = tRc.top;
			pts[3].x = tRc.right-4;	pts[3].y = tRc.top;
			pts[4].x = tRc.right-1;	pts[4].y = tRc.top+3;
			pts[5].x = tRc.right-1;	pts[5].y = tRc.bottom;
			pts[6].x = pts[5].x-1;	pts[6].y = tRc.bottom;
			pts[7].x = pts[6].x;	pts[7].y = tRc.top+3;
			pts[8].x = pts[3].x;	pts[8].y = pts[3].y+1;
			pts[9].x = pts[2].x;	pts[9].y = pts[8].y;
			pts[10].x = pts[1].x+1;	pts[10].y = pts[1].y;
			pts[11].x = pts[0].x+1;	pts[11].y = pts[0].y;
			setPen(dc, getRGB(clBLACK));
			setBrush(dc, getRGB(clBLACK));
			dc->Polygon(pts, 12);
			
			if (scroll && ii == last -1)
			{
				style |= DT_LEFT;
				tmpS = _T(" ") + tmpS;
			}
			else
				style |= DT_CENTER;
			
			if (!bChecked)
			{
				pts[0].x++;
				pts[1].x++;
				pts[2].y++;
				pts[3].y++;
				pts[4].x = pts[8].x;	pts[4].y = pts[8].y+1;
				pts[5].x = pts[9].x;	pts[5].y = pts[9].y+1;
				pts[6].x = pts[10].x+1;	pts[6].y = pts[10].y;
				pts[7].x = pts[11].x+1;	pts[7].y = pts[11].y;
				setPen(dc, getRGB(clYWHITE));
				setBrush(dc, getRGB(clYWHITE));
				dc->Polygon(pts, 8);
				
				pts[0] = pts[7];
				pts[1] = pts[6];
				pts[2] = pts[5];
				pts[3] = pts[4];
				pts[4].x = tRc.right-2;	pts[4].y = tRc.top+3;
				pts[5].x = pts[4].x;	pts[5].y = tRc.bottom;
				setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
				setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
				dc->Polygon(pts, 6);
				
				pts[0].x = pts[3].x+1;	pts[0].y = tRc.top+2;
				pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y+1;
				setPen(dc, getRGB(clEDGE));
				setBrush(dc, getRGB(clEDGE));
				dc->Polygon(pts, 2);
				
				pts[0].x = pts[1].x;	pts[0].y++;
				pts[1].x = pts[0].x;	pts[1].y = tRc.bottom;
				dc->Polygon(pts, 2);
				
				pts[0].x = tRc.left;	pts[0].y = tRc.bottom-1;
				pts[1].x = tRc.right-1;	pts[1].y = tRc.bottom-1;
				pts[2].x = tRc.right-1;	pts[2].y = tRc.bottom;
				pts[3].x = tRc.left;	pts[3].y = tRc.bottom;
				setPen(dc, getRGB(clYWHITE));
				setBrush(dc, getRGB(clYWHITE));
				dc->Polygon(pts, 4);
				
				pts[2].y--;
				pts[3].y--;
				setPen(dc, getRGB(clBOTTOM));
				setBrush(dc, getRGB(clBOTTOM));
				dc->Polygon(pts, 4);
				
				//setTextColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->tRGB));
				//dc->SetBkColor(getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
				
				tRc1.DeflateRect(2, 5, 2, 0);
				break;
			}
			
			pts[0].x++;
			pts[1].x++;		pts[1].y++;
			pts[2].x += 1;		pts[2].y++;
			pts[3].x--;		pts[3].y++;
			pts[4].x--;		pts[4].y++;
			pts[5].x = pts[4].x;	pts[5].y = pts[4].y+1;
			pts[6].x = pts[3].x;	pts[6].y = pts[3].y+1;
			pts[7].x = pts[2].x;	pts[7].y = pts[2].y+1;
			pts[8].x = pts[1].x+1;	pts[8].y = pts[1].y;
			pts[9].x = pts[8].x;	pts[9].y = pts[0].y;
			setPen(dc, getRGB(clYWHITE));
			setBrush(dc, getRGB(clYWHITE));
			dc->Polygon(pts, 10);
			
			pts[0] = pts[9];
			pts[1] = pts[8];
			pts[2] = pts[7];
			pts[3].x = pts[6].x;	pts[3].y = pts[6].y;
			pts[4].x = pts[3].x+2;	pts[4].y = pts[3].y+2;
			pts[5].x = tRc.right-3;	pts[5].y = pts[0].y;
			setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
			setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
			dc->Polygon(pts, 6);
			
			pts[0].x--;		pts[0].y = pts[1].y-1;
			pts[1].x = tRc.left+3;	pts[1].y = pts[2].y-1;
			dc->Polygon(pts, 2);
			
			pts[0].x = pts[3].x+1;	pts[0].y = pts[1].y;
			pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y+1;
			dc->Polygon(pts, 2);
			
			pts[0].x = pts[1].x+1;	pts[0].y = pts[1].y+1;
			pts[1].x = pts[0].x;	pts[1].y = tRc.bottom;
			setPen(dc, getRGB(clEDGE));
			setBrush(dc, getRGB(clEDGE));
			dc->Polygon(pts, 2);
			
			pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
			pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y;
			setPen(dc, getRGB(clYWHITE));
			setBrush(dc, getRGB(clYWHITE));
			dc->Polygon(pts, 2);
			
			pts[0].x = tRc.right-2;
			pts[1].x = pts[0].x+2;
			dc->Polygon(pts, 2);
			
			pts[0].x++;		pts[0].y--;
			pts[1].x = tRc.right;	pts[1].y = pts[0].y+1;
			setPen(dc, getRGB(clBOTTOM));
			setBrush(dc, getRGB(clBOTTOM));
			dc->Polygon(pts, 2);
			
			//setTextColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->stRGB));
			//dc->SetBkColor(getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
			
			tRc1.DeflateRect(2, 5, 1, 0);
			break;

		case TAB_RECT:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
			{
				setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
				brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
			}
			else
			{
				setPen(dc);
				brush = setBrush(dc);
			}
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x = tRc.left;			pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;			pts[1].y = tRc.top;
			if (scroll && ii == last-1)
			{
				pts[2].x = tRc.right;		pts[2].y = tRc.top;
				pts[3].x = tRc.right;		pts[3].y = tRc.bottom;
				dc->Polygon(pts, 4);
			}
			else
			{
				pts[2].x = tRc.right-(tRc.Height()/2);	pts[2].y = tRc.top;
				pts[3].x = tRc.right;			pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4].x = tRc.right;			pts[4].y = tRc.bottom;
				dc->Polygon(pts, 5);
			}

			setPen(dc, clBLACK);
			brush = setBrush(dc, clBLACK);
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x = tRc.left;			pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;			pts[1].y = tRc.top;
			if (scroll && ii == last-1)
			{
				pts[2] = tRc.right;		pts[2].y = tRc.top;
				pts[3] = pts[1];
				dc->Polygon(pts, 4);
			}
			else
			{
				pts[2].x = tRc.right-(tRc.Height()/2);	pts[2].y = tRc.top;
				pts[3].x = tRc.right;			pts[3].y = tRc.top + (tRc.Height()/2);
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
			if (scroll && ii == last-1)
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

			setPen(dc, clBLACK);
			brush = setBrush(dc, clBLACK);
			dc->SetPolyFillMode(ALTERNATE);
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
			{
				pts[0].x = tRc.right;	pts[0].y = tRc.bottom;
				pts[1].x = tRc.right;	pts[1].y = tRc.top + (tRc.Height()/2);
				dc->Polygon(pts, 2);
			}
			else
			{
				pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
				pts[1].x = tRc.right;	pts[1].y = tRc.bottom;
				if (scroll && ii == last-1)
					dc->Polygon(pts, 2);
				else
				{
					pts[2].x = tRc.right;	pts[2].y = tRc.top + (tRc.Height()/2);
					pts[3] = pts[1];
					pts[4] = pts[0];
					dc->Polygon(pts, 5);
				}
			}

			setPen(dc, clEDGE);
			brush = setBrush(dc, clEDGE);
			dc->SetPolyFillMode(ALTERNATE);
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
			{
				pts[0].x--;
				pts[1].x--;		pts[1].y++;
				dc->Polygon(pts, 2);
			}
			else
			{
				pts[0].x++;		pts[0].y--;
				pts[1].x--;		pts[1].y--;
				if (scroll && ii == last-1)
					dc->Polygon(pts, 2);
				else
				{
					pts[2].x--;		pts[2].y++;
					pts[3] = pts[1];
					pts[4] = pts[0];
					dc->Polygon(pts, 5);
				}
			}
			tRc1 = tRc;
			tRc1.top+=2;
			style |= DT_LEFT;
			str = _T(" ") + str;
			break;
			
		case TAB_BORDER:
		default:
			{
				switch (m_pFormItem->getFormPoint(m_fN)->alignment)
				{
				case AL_LEFT:
					style = DT_CENTER|DT_TOP;
					tRc1 = tRc;

					if (bChecked)
						draw3dRoundRect(dc, tRc1, AL_LEFT, 3, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
					else
					{
						tRc1.left += 2;
						draw3dRoundRect(dc, tRc1, AL_LEFT, 5, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
						tRc1 = tRc;
						tRc1.left = tRc1.right - 1;
						dc->FillSolidRect(&tRc1, clBOTTOM);
					}
					
					if (!(scroll && ii == last-1))
					{
						/*tRc1 = tRc;
						tRc1.left++;
						tRc1.top = tRc1.bottom - 1;
						brush = setBrush(dc, clBOTTOM);
						dc->FillRect(&tRc1, brush);	// bottom*/
					}
					
					tRc1 = tRc;
					tRc1.DeflateRect(1, 1);
					break;
					
				case AL_RIGHT:
					style = DT_CENTER|DT_TOP;
					tRc1 = tRc;
			
					if (bChecked)
						draw3dRoundRect(dc, tRc1, AL_RIGHT, 3, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
					else
					{
						tRc1.right -= 2;
						draw3dRoundRect(dc, tRc1, AL_RIGHT, 5, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
						tRc1 = tRc;
						tRc1.right = tRc1.left + 1;
						dc->FillSolidRect(&tRc1, clBOTTOM);
					}
					tRc1 = tRc;
					tRc1.DeflateRect(1, 1);
					
					break;
					
				case AL_BOTTOM:
					style = DT_SINGLELINE|DT_VCENTER;
					tRc1 = tRc;
					
					if (bChecked)
						draw3dRoundRect(dc, tRc1, AL_BOTTOM, 3, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
					else
					{
						tRc1.bottom -= 2;
						draw3dRoundRect(dc, tRc1, AL_BOTTOM, 5, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
						tRc1 = tRc;
						tRc1.bottom = tRc1.top + 1;
						dc->FillSolidRect(&tRc1, clBOTTOM);
					}

					if (scroll && ii == last-1)
					{
						style |= DT_LEFT;
						str = _T(" ") + str;
					}
					else
						style |= DT_CENTER;
					
					tRc1 = tRc;
					tRc1.DeflateRect(1, 1);
					break;
					
				case AL_TOP:
				default:
					style = DT_SINGLELINE|DT_VCENTER;
					tRc1 = tRc;
					
					if (bChecked)
						draw3dRoundRect(dc, tRc1, AL_TOP, 3, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
					else
					{
						tRc1.top += 2;
						draw3dRoundRect(dc, tRc1, AL_TOP, 5, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
						tRc1 = tRc;
						tRc1.top = tRc1.bottom - 1;
						dc->FillSolidRect(&tRc1, clBOTTOM);
					}

					if (scroll && ii == last-1)
					{
						style |= DT_LEFT;
						str = _T(" ") + str;
					}
					else
					{
						style |= DT_CENTER;
					}
										
					tRc1 = tRc;
					tRc1.DeflateRect(1, 1);
					break;
				}
			}
			break;
		}

		if (vertical)
		{
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
				setBoldFont(dc, true);
			else
				setBoldFont(dc, false);
			dc->SetBkMode(TRANSPARENT);
			drawVText(dc, str, tRc1, style);
		}
		else
		{
			tRc1.DeflateRect(3, 0);
			editAmpersand(str);
			if (ii == m_pFormItem->getFormPoint(m_fN)->checked)
				setBoldFont(dc, true);
			else
				setBoldFont(dc, false);
			dc->SetBkMode(TRANSPARENT);
       	 		dc->DrawText(str, tRc1, style);
		}
	}

	if (!cell.IsRectEmpty())
	{
		setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
		brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
		dc->SetPolyFillMode(ALTERNATE);
		pts[0].x = cell.left;			pts[0].y = cell.bottom;
		pts[1].x = cell.left;			pts[1].y = cell.top;
		if (scroll && m_pFormItem->getFormPoint(m_fN)->checked == last-1)
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
		if (scroll && m_pFormItem->getFormPoint(m_fN)->checked == last-1)
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
		if (scroll && m_pFormItem->getFormPoint(m_fN)->checked == last-1)
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

		setTextColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->stRGB));
		setBkColor(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
		cells = _T(" ") + cells;
		cell.DeflateRect(3, 0);
		cell.top+=2;
		editAmpersand(cells);
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

void CtlTab::drawVText(CDC* dc, CString tabs, CRect rect, UINT style)
{
	CRect	rc;
	CSize	size;
	float	dy;
	CStringArray	arrStr;

	getString(tabs, arrStr);
	size = dc->GetOutputTextExtent("A");

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

void CtlTab::drawPage(CDC* dc, CRect tabRc, CRect rect, bool left)
{
	CRect	rc;
	CPoint	pts[6];
	CBrush*	brush;
	int	gap = 1;

	rc = rect;
	rc.top++;
	if (/*m_pFormItem->getFormPoint(m_fN)->type == TAB_OBLIQUE ||*/ m_pFormItem->getFormPoint(m_fN)->type == TAB_RECT)
	{
		rc.top++; rect.top++;
	}
	brush = setBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->spRGB));
	dc->FillRect(&rc, brush);

	if (/*m_pFormItem->getFormPoint(m_fN)->type == TAB_OBLIQUE ||*/ m_pFormItem->getFormPoint(m_fN)->type == TAB_RECT)
	{
		setPen(dc, clBLACK);
		brush = setBrush(dc, clBLACK);
	}
	else
	{
		setPen(dc, clYWHITE);
		brush = setBrush(dc, clYWHITE);
	}
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

void CtlTab::drawScroll(CDC* dc, CRect scrRc)
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

bool CtlTab::isScrolled(CDC* dc, int tabN, CString data, int& index, int& width)
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
			tabs = Parser(data, "\t");

		width += getSize(dc, tabs);
		//if (m_pFormItem->getFormPoint(m_fN)->type == TAB_OBLIQUE && ii == count-1)
		//	width += (m_pFormItem->getFormPoint(m_fN)->size/2);
		if (!index && (width > rect.Width() - (int)(float)(m_pFormItem->getFormPoint(m_fN)->size * 2.0 / 3.0) * 2))
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

bool CtlTab::isScrolled(CDC* dc, int tabN, CString data, CString info, int& index, int& width, bool vertical)
{
	CString	tabs, infos;
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
		{
			infos = Parser(info, "\t");
			tabs  = Parser(data, "\t");

			/*if (infos.GetAt(0) != '1')
				continue;*/
		}

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
			//if (m_pFormItem->getFormPoint(m_fN)->type == TAB_OBLIQUE && ii == getLastVisibleIndex())
			//	width += (rect.Height()/2);
			if (!index && (width > rect.Width() - (int)(float)(rect.Height() * 2.0 / 3.0) * 2))
				index = ii+1;
			if (width > rect.Width())
			{
				scroll = true;
				break;
			}
		}
	}

	if (!scroll)
		index = getLastVisibleIndex()+1;
	index = (index == 0) ? count : index;
	return scroll;
}

int CtlTab::getSize(CDC* dc, CString tabs, bool vertical)
{
	CSize	size;

	if (vertical)
	{
		CStringArray arrStr;
		size = dc->GetOutputTextExtent("A");
		getString(tabs, arrStr);
		return (size.cy * arrStr.GetSize() + (int)(size.cy * 0.5 * (arrStr.GetSize()-1)) + TAB_EXTRA);
	}

	size = dc->GetOutputTextExtent(tabs);
	if (/*m_pFormItem->getFormPoint(m_fN)->type == TAB_OBLIQUE || */m_pFormItem->getFormPoint(m_fN)->type == TAB_RECT)
		return (size.cx + TAB_EXTRA2);

	return (size.cx + TAB_EXTRA);
}

void CtlTab::getString(CString string, CStringArray& arrStr)
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

int CtlTab::getLastVisibleIndex()
{
	CString	info;
	CString	item;
	int	index = 0;

	if (!m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
	{
		info = m_pFormItem->getFormStr(m_fN, "str");
		for (int ii = 0; ii < (int)m_pFormItem->getFormPoint(m_fN)->keys; ii++)
		{
			item = Parser(info, "\t");
			if (!item.IsEmpty() && item.GetAt(0) == '1')
				index = ii;
		}
	}

	return index;
}

CString CtlTab::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}



int CtlTab::Selectable(CPoint point, CRect &rc, CPoint &pt, bool bSet)
{
	CPoint	pts[6];
	CString	cells, item;
	UINT	style;
	CString	data;
	bool	scroll = false;
	CRect	fRc,  pRc,  tRc,  tRc1, cell;
	CRgn	tRgn;
	int	last,  dx,  cx = 2, nPnt = 0;
	CDC *dc = m_dc;

	pRc = fRc = getRect();
	pRc.bottom = pRc.top + m_pFormItem->getFormPoint(m_fN)->size;
	fRc.top    = pRc.bottom-1;
	
	setFont(dc);
	data = m_pFormItem->getFormStr(m_fN, "dat");

	scroll = isScrolled(dc, (int)m_pFormItem->getFormPoint(m_fN)->keys, data, last, dx);
	if (scroll)
	{
		tRc = pRc;
		pRc.right -= (int)(float)(m_pFormItem->getFormPoint(m_fN)->size * 2.0 / 3.0) * 2;
		tRc.SubtractRect(tRc, pRc);
		if (tRc.PtInRect(point))
		{
			if (point.x < tRc.left + tRc.Width() / 2) // left shift
			{
				m_pFormItem->getFormPoint(m_fN)->scrollPos--;
				if (m_pFormItem->getFormPoint(m_fN)->scrollPos < 0)
					m_pFormItem->getFormPoint(m_fN)->scrollPos = 0;
			}
			else
			{
				m_pFormItem->getFormPoint(m_fN)->scrollPos++;
				dx = pRc.left;
				for (int ii = 0; ii < (int)m_pFormItem->getFormPoint(m_fN)->keys; ii++)
				{
					item = Parser(data, "\t");
					if (m_pFormItem->getFormPoint(m_fN)->scrollPos > ii)
						continue;
					dx += getSize(dc, item);
				}

				if (pRc.right > dx)
					m_pFormItem->getFormPoint(m_fN)->scrollPos--;
				/*if (m_pFormItem->getFormPoint(m_fN)->scrollPos >= (int)m_pFormItem->getFormPoint(m_fN)->keys)
					m_pFormItem->getFormPoint(m_fN)->scrollPos--;*/
			}
			return -1;
		}
		last = min((int)m_pFormItem->getFormPoint(m_fN)->keys, last + m_pFormItem->getFormPoint(m_fN)->scrollPos);
	}
	else
	{
		pRc.right = pRc.left + dx;
		last = max((int)m_pFormItem->getFormPoint(m_fN)->keys, 1);
	}

	dx = pRc.left;
	cell.SetRectEmpty();
	for (int ii = 0; ii < last; ii++)
	{
		if (!m_pFormItem->getFormStr(m_fN, "dat").IsEmpty())
			item = Parser(data, "\t");

		if (m_pFormItem->getFormPoint(m_fN)->scrollPos > ii)
			continue;

		item = (m_pFormItem->getFormPoint(m_fN)->keys == 0) ? m_name : item;

		tRc = pRc;
		tRc.left = dx;
		if (!scroll || ii < last-1)
		{
			dx += getSize(dc, item);
			tRc.right = dx;
		}

		style = DT_SINGLELINE | DT_VCENTER;
		switch (m_pFormItem->getFormPoint(m_fN)->type)
		{
		case TAB_NONE:
			tRc1 = tRc;
			tRc1.left+=2;	tRc1.top+=2;
			pts[0].x = tRc1.left;		pts[0].y = tRc1.bottom-1;
			pts[1].x = tRc1.left;		pts[1].y = tRc1.top+cx;
			pts[2].x = tRc1.left+cx;	pts[2].y = tRc1.top;
			pts[3].x = tRc1.right-cx;	pts[3].y = tRc1.top;
			pts[4].x = tRc1.right;		pts[4].y = pts[1].y;
			pts[5].x = tRc1.right;		pts[5].y = tRc1.bottom-1;
			tRgn.CreatePolygonRgn(pts, 6, ALTERNATE);
			if (tRgn.PtInRegion(point))
			{
				if (bSet)
					m_pFormItem->getFormPoint(m_fN)->checked = ii;
				pt = CPoint(tRc1.left, tRc1.top);
				tRgn.DeleteObject();
				return ii;
			}
			pt = CPoint(tRc1.right, tRc1.top);
			tRgn.DeleteObject();
			break;

		case TAB_OBLIQUE:
			if (tRc.PtInRect(point))
			{
				if (bSet)
					m_pFormItem->getFormPoint(m_fN)->checked = ii;
				pt = CPoint(tRc.left, tRc.top);
				return ii;
			}
			pt = CPoint(tRc.right, tRc.top);
			break;
		case TAB_RECT:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
	
			pts[0].x = tRc.left;			pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;			pts[1].y = tRc.top;
			if (scroll && ii == last-1)
			{
				pts[2].x = tRc.right;		pts[2].y = tRc.top;
				pts[3].x = tRc.right;		pts[3].y = tRc.bottom;
				nPnt = 4;
			}
			else
			{
				pts[2].x = tRc.right-(tRc.Height()/2);	pts[2].y = tRc.top;
				pts[3].x = tRc.right;			pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4].x = tRc.right;			pts[4].y = tRc.bottom;
				nPnt = 5;
			}

			tRgn.CreatePolygonRgn(pts, nPnt, ALTERNATE);
			if (tRgn.PtInRegion(point))
			{
				if (bSet)
					m_pFormItem->getFormPoint(m_fN)->checked = ii;
				pt = CPoint(tRc.left, tRc.top);
				tRgn.DeleteObject();
				return ii;
			}
			pt = CPoint(tRc.right, tRc.top);
			tRgn.DeleteObject();
			break;

		case TAB_BORDER:
		default:
			if (tRc.PtInRect(point))
			{
				if (bSet)
					m_pFormItem->getFormPoint(m_fN)->checked = ii;
				pt = CPoint(tRc.left, tRc.top);
				return ii;
			}
			pt = CPoint(tRc.right, tRc.top);
			break;
		}
	}
	return -1;
}


int CtlTab::SelectableUTab(CPoint point, CRect &rc, CPoint &pt, bool bSet)
{
	CPoint	pts[6];
	UINT	style;
	CString	str, item, cells;
	CString	data, info;
	bool	vertical, scroll;
	CRect	fRc, pRc, tRc, tRc1, cell;
	int	count,  vcount,  cx = 2;
	int	last, len, dx, nPnt;
	CDC *dc = m_dc;
	CRgn	tRgn;

	fRc = pRc = getRect();
	setFont(dc);

	count = max((int)m_pFormItem->getFormPoint(m_fN)->keys, 1);
	len = vcount = 0;
	if (!m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
	{
		info = m_pFormItem->getFormStr(m_fN, "str");
		for (int ii = 0; ii < (int)m_pFormItem->getFormPoint(m_fN)->keys; ii++)
		{
			item = Parser(info, "\t");
			if (item.GetLength() && item.GetAt(0) == '1')
				vcount++;
		}
	}

	vertical = scroll = false;
	if (m_pFormItem->getFormPoint(m_fN)->alignment == AL_LEFT || m_pFormItem->getFormPoint(m_fN)->alignment == AL_RIGHT)
		vertical = true;

	if (!m_pFormItem->getFormStr(m_fN, "dat").IsEmpty() && !m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
	{
		data = m_pFormItem->getFormStr(m_fN, "dat");
		info = m_pFormItem->getFormStr(m_fN, "str");
	}

	scroll = isScrolled(dc, (int)m_pFormItem->getFormPoint(m_fN)->keys, data, info, last, dx, vertical);
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
				if (point.x < tRc.left + tRc.Width() / 2) // left shift
				{
					m_pFormItem->getFormPoint(m_fN)->scrollPos--;
					if (m_pFormItem->getFormPoint(m_fN)->scrollPos < 0)
						m_pFormItem->getFormPoint(m_fN)->scrollPos = 0;
				}
				else
				{
					m_pFormItem->getFormPoint(m_fN)->scrollPos++;
					dx = pRc.left;
					for (int ii = 0; ii < (int)m_pFormItem->getFormPoint(m_fN)->keys; ii++)
					{
						item = Parser(data, "\t");
						if (m_pFormItem->getFormPoint(m_fN)->scrollPos > ii)
							continue;
						dx += getSize(dc, item);
					}
					
					if (pRc.right > dx)
						m_pFormItem->getFormPoint(m_fN)->scrollPos--;
						/*if (m_pFormItem->getFormPoint(m_fN)->scrollPos >= (int)m_pFormItem->getFormPoint(m_fN)->keys)
					m_pFormItem->getFormPoint(m_fN)->scrollPos--;*/
				}
				return -1;
			}

		}
		else
			pRc.right = pRc.left + dx;

		last = min((int)m_pFormItem->getFormPoint(m_fN)->keys, last + m_pFormItem->getFormPoint(m_fN)->scrollPos);
		dx = pRc.left;
	}

	cell.SetRectEmpty();
	for (int ii = 0; ii < last; ii++)
	{
		if (!m_pFormItem->getFormStr(m_fN, "str").IsEmpty())
		{
			item = Parser(info, "\t");

			if ((item.GetLength() && item.GetAt(0) != '1') && (m_pFormItem->getFormPoint(m_fN)->keys != 0))
			{
				if (!m_pFormItem->getFormStr(m_fN, "dat").IsEmpty())
					str = Parser(data, "\t");
				continue;
			}
		}

		if (!m_pFormItem->getFormStr(m_fN, "dat").IsEmpty())
			str = Parser(data, "\t");

		if (m_pFormItem->getFormPoint(m_fN)->scrollPos > ii)
			continue;

		str = (m_pFormItem->getFormPoint(m_fN)->keys == 0) ? m_name : str;

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

		switch (m_pFormItem->getFormPoint(m_fN)->type)
		{
		case TAB_NONE:
			tRc.right--;
			tRc.bottom--;
			switch (m_pFormItem->getFormPoint(m_fN)->alignment)
			{
			case AL_LEFT:
				style = DT_CENTER|DT_TOP;
				tRc1 = tRc;
				tRc1.left++;	tRc1.top++;
				pts[0].x = tRc1.right;		pts[0].y = tRc1.top;
				pts[1].x = tRc1.left+cx;	pts[1].y = tRc1.top;
				pts[2].x = tRc1.left;		pts[2].y = tRc1.top+cx;
				pts[3].x = tRc1.left;		pts[3].y = tRc1.bottom-cx;
				pts[4].x = tRc1.left+cx;	pts[4].y = tRc1.bottom;
				pts[5].x = tRc1.right;		pts[5].y = tRc1.bottom;
				nPnt = 6;

				tRgn.CreatePolygonRgn(pts, nPnt, ALTERNATE);
				if (tRgn.PtInRegion(point))
				{
					if (bSet)
						m_pFormItem->getFormPoint(m_fN)->checked = ii;
					pt = CPoint(tRc1.left, tRc1.top);
					tRgn.DeleteObject();
					return ii;
				}
				pt = CPoint(tRc1.left, tRc1.bottom);
				tRgn.DeleteObject();
				break;

			case AL_RIGHT:
				style = DT_CENTER|DT_TOP;
				tRc1 = tRc;
				tRc1.left++;	tRc1.top++;
				pts[0].x = tRc1.left;		pts[0].y = tRc1.top;
				pts[1].x = tRc1.left;		pts[1].y = tRc1.bottom;
				pts[2].x = tRc1.right-cx;	pts[2].y = tRc1.bottom;
				pts[3].x = tRc1.right;		pts[3].y = tRc1.bottom-cx;
				pts[4].x = tRc1.right;		pts[4].y = tRc1.top+cx;
				pts[5].x = tRc1.right-cx;	pts[5].y = tRc1.top;

				nPnt = 6;
				tRgn.CreatePolygonRgn(pts, nPnt, ALTERNATE);
				if (tRgn.PtInRegion(point))
				{
					if (bSet)
						m_pFormItem->getFormPoint(m_fN)->checked = ii;
					pt = CPoint(tRc1.right, tRc1.top);
					tRgn.DeleteObject();
					return ii;
				}
				pt = CPoint(tRc1.right, tRc1.bottom);
				tRgn.DeleteObject();
				break;

			case AL_BOTTOM:
				style = DT_SINGLELINE | DT_VCENTER;
				tRc1 = tRc;
				tRc1.left++;	tRc1.top++;
				pts[0].x = tRc1.left;		pts[0].y = tRc1.top;
				pts[1].x = tRc1.left;		pts[1].y = tRc1.bottom-cx;
				pts[2].x = tRc1.left+cx;	pts[2].y = tRc1.bottom;
				pts[3].x = tRc1.right-cx;	pts[3].y = tRc1.bottom;
				pts[4].x = tRc1.right;		pts[4].y = pts[1].y;
				pts[5].x = tRc1.right;		pts[5].y = tRc1.top;
				nPnt = 6;
				tRgn.CreatePolygonRgn(pts, nPnt, ALTERNATE);
				if (tRgn.PtInRegion(point))
				{
					if (bSet)
						m_pFormItem->getFormPoint(m_fN)->checked = ii;
					pt = CPoint(tRc1.left, tRc1.bottom);
					tRgn.DeleteObject();
					return ii;
				}
				pt = CPoint(tRc1.right, tRc1.bottom);
				tRgn.DeleteObject();
				break;

			case AL_TOP:
			default:
				style = DT_SINGLELINE | DT_VCENTER;
				tRc1 = tRc;
				tRc1.left++;	tRc1.top++;
				pts[0].x = tRc1.left;		pts[0].y = tRc1.bottom;
				pts[1].x = tRc1.left;		pts[1].y = tRc1.top+cx;
				pts[2].x = tRc1.left+cx;	pts[2].y = tRc1.top;
				pts[3].x = tRc1.right-cx;	pts[3].y = tRc1.top;
				pts[4].x = tRc1.right;		pts[4].y = pts[1].y;
				pts[5].x = tRc1.right;		pts[5].y = tRc1.bottom;

				nPnt = 6;
				tRgn.CreatePolygonRgn(pts, nPnt, ALTERNATE);
				if (tRgn.PtInRegion(point))
				{
					if (bSet)
						m_pFormItem->getFormPoint(m_fN)->checked = ii;
					pt = CPoint(tRc1.left, tRc1.top);
					tRgn.DeleteObject();
					return ii;
				}
				pt = CPoint(tRc1.right, tRc1.top);
				tRgn.DeleteObject();
				break;
			}
			tRc1.left  += 3;
			tRc1.right -= 3;
			break;

		case TAB_RECT:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
			
			pts[0].x = tRc.left;			pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;			pts[1].y = tRc.top;
			if (scroll && ii == last-1)
			{
				pts[2].x = tRc.right;		pts[2].y = tRc.top;
				pts[3].x = tRc.right;		pts[3].y = tRc.bottom;
				nPnt = 4;
			}
			else
			{
				pts[2].x = tRc.right-(tRc.Height()/2);	pts[2].y = tRc.top;
				pts[3].x = tRc.right;			pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4].x = tRc.right;			pts[4].y = tRc.bottom;
				nPnt = 5;
			}

			tRgn.CreatePolygonRgn(pts, nPnt, ALTERNATE);
			if (tRgn.PtInRegion(point))
			{
				if (bSet)
					m_pFormItem->getFormPoint(m_fN)->checked = ii;
				pt = CPoint(tRc.left, tRc.top);
				tRgn.DeleteObject();
				return ii;
			}
			pt = CPoint(tRc.right, tRc.top);
			tRgn.DeleteObject();
			break;

		case TAB_OBLIQUE:
		case TAB_BORDER:
		default:
			switch (m_pFormItem->getFormPoint(m_fN)->alignment)
			{
			case AL_LEFT:
				if (tRc.PtInRect(point))
				{
					if (bSet)
						m_pFormItem->getFormPoint(m_fN)->checked = ii;
					pt = CPoint(tRc.left, tRc.top);
					return ii;
				}
				pt = CPoint(tRc.left, tRc.bottom);
				break;

			case AL_RIGHT:
				if (tRc.PtInRect(point))
				{
					if (bSet)
						m_pFormItem->getFormPoint(m_fN)->checked = ii;
					pt = CPoint(tRc.right, tRc.top);
					return ii;
				}
				pt = CPoint(tRc.right, tRc.bottom);
				break;

			case AL_BOTTOM:
				if (tRc.PtInRect(point))
				{
					if (bSet)
						m_pFormItem->getFormPoint(m_fN)->checked = ii;
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
						m_pFormItem->getFormPoint(m_fN)->checked = ii;
					pt = CPoint(tRc.left, tRc.top);
					return ii;
				}
				pt = CPoint(tRc.right, tRc.top);
				break;
			}
			break;
		}
	}
	return -1;
}

void CtlTab::MouseDrag()
{
	if (m_view->m_bSelect && !m_pArrowWnd && m_selected >= 0)
	{
		m_pArrowWnd = new CArrowWnd();
		m_pArrowWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 8, 5), m_view, 1);
		m_pArrowWnd->SetTP(4);
	}
}

void CtlTab::deletePage(int nPage)
{
	struct _formR *formR = m_pFormItem->getFormPoint(m_fN);
	if (nPage >= 0)
	{
		CString temp = m_pFormItem->getFormStr(m_fN, "dat"), tmp = "";
		CString temp2 = m_pFormItem->getFormStr(m_fN, "str"), tmp2 = "";
		for (int jj = 0; jj < (int)formR->keys; jj++)
		{
			if (nPage == jj)
			{
				m_dat = Parser(temp, "\t");
				m_str = Parser(temp2, "\t");
				continue;
			}
			tmp += Parser(temp, "\t");
			tmp += '\t';
			if (formR->kind == FM_UTAB)
			{
				tmp2 += Parser(temp2, "\t");
				tmp2 += '\t';
			}
		}
		m_pFormItem->setFormStr(m_fN, "dat", tmp);
		if (formR->kind == FM_UTAB)
			m_pFormItem->setFormStr(m_fN, "str", tmp2);
		if (formR->keys > 0)
			formR->keys--;
		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
		{
			if (m_pFormItem->getFormPoint(ii)->tabN == formR)
			{
				if (m_pFormItem->getFormPoint(ii)->pageN > nPage)
					m_pFormItem->getFormPoint(ii)->pageN--;
				else if (m_pFormItem->getFormPoint(ii)->pageN == nPage)
					m_pFormItem->getFormPoint(ii)->pageN = 30000;
			}
		}
		if (nPage >= (int)formR->keys)
			formR->checked = 0;
	}
}

void CtlTab::addPage(int nPage)
{
	struct _formR *formR = m_pFormItem->getFormPoint(m_fN);
	if (nPage >= 0)
	{
		CString temp = m_pFormItem->getFormStr(m_fN, "dat"), tmp = "";
		CString temp2 = m_pFormItem->getFormStr(m_fN, "str"), tmp2 = "";
		formR->keys++;
		for (int jj = 0; jj < (int)formR->keys; jj++)
		{
			if (nPage == jj)
			{
				tmp += m_dat;
				tmp += '\t';
				if (formR->kind == FM_UTAB)
				{
					tmp2 += m_str;
					tmp2 += '\t';
				}
				continue;
			}
			tmp += Parser(temp, "\t");
			tmp += '\t';
			if (formR->kind == FM_UTAB)
			{
				tmp2 += Parser(temp2, "\t");
				tmp2 += '\t';
			}
		}
		m_pFormItem->setFormStr(m_fN, "dat", tmp);
		if (formR->kind == FM_UTAB)
			m_pFormItem->setFormStr(m_fN, "str", tmp2);
		
		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
		{
			if (m_pFormItem->getFormPoint(ii)->tabN == formR)
			{
				if (m_pFormItem->getFormPoint(ii)->pageN == 30000)
					m_pFormItem->getFormPoint(ii)->pageN = nPage;
				else if (m_pFormItem->getFormPoint(ii)->pageN >= nPage)
					m_pFormItem->getFormPoint(ii)->pageN++;
			}
		}
		if (nPage >= (int)formR->keys)
			formR->checked = 0;
		formR->checked = nPage;
	}
}

void CtlTab::draw3dRoundRect(CDC *dc, CRect rc, int align, int depth, COLORREF borderColor)
{
	CRect tRc = rc;
	setBrush(dc, clWHITE);
	setPen(dc, clBOTTOM);
	dc->RoundRect(rc, CPoint(depth, depth));
	
	switch(align)
	{
	case AL_TOP:
		{
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
		}
		break;
	case AL_RIGHT:
		{
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
		}
		break;
	case AL_BOTTOM:
		{
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
		}
		break;
	case AL_LEFT:
		{
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
		}
		break;
	}
}

void CtlTab::draw2Rect(CDC *dc, CRect rc, COLORREF color1, COLORREF color2)
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
