// tlGrid.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlGrid.h"
#include "tlImage.h"
#include "markWnd.h"

#include "../resource.h"
#include "../h/mainvar.h"
#include "../amCom/formItem.h"
#include "../amCom/cellItem.h"

#define	sortMARK	"□"

// CTlGrid

CTlGrid::CTlGrid(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type)
	: CTlRect(view, draw, formItem)
{
	switch (type)
	{
	case ID_TOOL_GRID:
		m_type = tbGRID;
		m_name = _T("Grid");
		break;
	case ID_TOOL_GRIDEX:
		m_type = tbGRIDEX;
		m_name = _T("GridEX");
		break;
	case ID_TOOL_TABLE:
		m_type = tbTABLE;
		m_name = _T("Table");
		break;
	default:
		m_type = tbNONE;
		break;
	}

	m_column = m_selected = -1;
	m_markWnd = NULL;
	m_image = NULL;
	m_maxRow = 0;
}

CTlGrid::~CTlGrid()
{
	if (m_markWnd)
		delete m_markWnd;
	if (m_image)
		delete m_image;
}

void CTlGrid::Draw(CDC* dc, int fN)
{
	int	ndc = dc->SaveDC();

	CTlBase::Draw(dc, fN);
	switch (m_type)
	{
	case tbGRID:
	case tbGRIDEX:
		drawGRID(dc, fN);
		break;
	case tbTABLE:
		drawTABLE(dc, fN);
		break;
	default:
		break;
	}
	dc->RestoreDC(ndc);
}

void CTlGrid::MouseDrag()
{
	if (m_view->m_bSelect && !m_markWnd)
	{
		m_markWnd = new CMarkWnd();
		if (m_type == tbGRID)
		{
			m_markWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 8, 5), m_view, 1);
			m_markWnd->SetTP(4);
		}
		else 
		{
			m_markWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 5, 8), m_view, 1);
			m_markWnd->SetTP(3);
		}
	}
}

bool CTlGrid::OptimumSize(CPoint point)
{
	CRect	tRc, rect;
	int	ii;
	struct	_repR*	repR;
	struct	_formR*	formR = m_formItem->GetForm(m_fN);

	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	ii = (formR->keys <= 0) ? m_fheight+4 : formR->keys;
	tRc.bottom = tRc.top + ii;
	tRc.right  = tRc.left;
	repR = (struct _repR *)formR->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	switch (formR->kind)
	{
	case FM_GRID:
	case FM_GRIDEX:
		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR *cellR = cellItem->GetCell(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;

			tRc.right += cellR->width;
			if (tRc.PtInRect(point))
			{
				cellR->width = getColumnWidth(repR, ii);
				break;
			}
			tRc.left = tRc.right;
		}

		if (ii >= repR->cols)
		{
			delete cellItem;
			return false;
		}
		break;

	default:
		delete cellItem;
		return false;
	}

	delete cellItem;
	m_rect = getRect();
	m_view->InvalidateRect(&m_rect);
	return true;
}

//
//		1
//	      4 0 2
//		3
//
CRect CTlGrid::getHeadRect(int col, int row, CRect rc, CString txt, int pos, CRect unionRc)
{
	CRect tRc, cRc;
	CString	sHead, sUnitHead;
	int	nMaxRow = m_maxRow;
	struct _repR *repR = (struct _repR *)m_formItem->GetForm(m_fN)->auxR;
	CCellItem cellItem(repR);

	sHead = cellItem.GetCell(col)->head;
	nMaxRow = m_maxRow;
	for (int kk = 0; kk < nMaxRow - sHead.Replace(_T(","), _T(",")) - 1; kk++)
		sHead += ",";
	
	tRc = cRc = rc;
	if (row < 0 || col < 0 || col >= cellItem.GetCount() || row >= nMaxRow)
		return CRect(0, 0, 0, 0);

	tRc.bottom = tRc.top + tRc.Height() / nMaxRow;
	tRc.OffsetRect(0, tRc.Height() * row);

	for (int ii = 0; ii <= row; ii++)
		sUnitHead = parse(sHead, _T(","));

	if (txt.IsEmpty())
		return tRc;

	if (sUnitHead.Compare(txt))
		return CRect(0, 0, 0, 0);

	unionRc.UnionRect(unionRc, CRect(tRc.left, tRc.top, tRc.right, tRc.bottom + 1));

	CRect tmpRect;
	if (col >= 1 && pos != 2)
	{
		cRc.left   = cRc.left - cellItem.GetCell(col - 1)->width;
		cRc.right  = cRc.left + cellItem.GetCell(col - 1)->width;
		cRc.top    = cRc.top + (cRc.Height() / nMaxRow) * row;
		cRc.bottom = cRc.top + (cRc.Height() / nMaxRow) * (row + 1);
		tmpRect.UnionRect(unionRc, cRc);
		if (!tmpRect.EqualRect(unionRc))
			tRc.UnionRect(tRc, getHeadRect(col - 1, row, cRc, sUnitHead, 4, unionRc));
	}

	unionRc.UnionRect(unionRc, tRc);

	if (col + 1 < cellItem.GetCount() && pos != 4)
	{
		cRc = rc;
		cRc.right  = cRc.right + cellItem.GetCell(col + 1)->width;
		cRc.left   = cRc.right - cellItem.GetCell(col + 1)->width;
		cRc.top    = cRc.top + (cRc.Height() / nMaxRow) * row;
		cRc.bottom = cRc.top + (cRc.Height() / nMaxRow) * (row + 1);
		tmpRect.UnionRect(unionRc, cRc);
		if (!tmpRect.EqualRect(unionRc))
			tRc.UnionRect(tRc, getHeadRect(col + 1, row, cRc, sUnitHead, 2, unionRc));
	}

	unionRc.UnionRect(unionRc, tRc);
	if (pos != 3)
	{
		tmpRect.UnionRect(unionRc, rc);
		if (!tmpRect.EqualRect(unionRc))
			tRc.UnionRect(tRc, getHeadRect(col, row + 1, rc, sUnitHead, 1, unionRc));
	}

	unionRc.UnionRect(unionRc, tRc);
	if (pos != 1)
	{
		tmpRect.UnionRect(unionRc, rc);
		if (!tmpRect.EqualRect(unionRc))
			tRc.UnionRect(tRc, getHeadRect(col, row - 1, rc, sUnitHead, 3, unionRc));
	}
	return tRc;
}

void CTlGrid::drawFocusRect(CDC *pDC, CRect rc)
{
	int	size = 2;

	CPen	Pen, *pOldPen;

	Pen.CreatePen(PS_SOLID, size, GetSysColor(COLOR_HIGHLIGHT));
	pOldPen = (CPen*)pDC->SelectObject(&Pen);
	
	pDC->SelectObject(&Pen);
	
	pDC->MoveTo(rc.right, rc.top);
	pDC->LineTo(rc.left, rc.top);
	pDC->LineTo(rc.left, rc.bottom);
	
	pDC->MoveTo(rc.left + size, rc.bottom - size);
	pDC->LineTo(rc.right - size, rc.bottom - size);
	pDC->LineTo(rc.right - size, rc.top + size);
	
	pDC->MoveTo(rc.right - size, rc.top + size);
	pDC->LineTo(rc.left + size, rc.top + size);
	pDC->LineTo(rc.left + size, rc.bottom - size);
	
	pDC->MoveTo(rc.left, rc.bottom);
	pDC->LineTo(rc.right , rc.bottom);
	pDC->LineTo(rc.right , rc.top);
	
	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();
}

int CTlGrid::movable(CPoint point, CRect& rc)
{
	CRect	tRc;
	int	ii, width;
	struct	_repR*	repR;
	struct	_formR*	formR = m_formItem->GetForm(m_fN);

	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	rc    = tRc;
	width = tRc.left;
	repR  = (struct _repR *)formR->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	switch (formR->kind)
	{
	case FM_GRID:
	case FM_GRIDEX:
		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR *cellR = cellItem->GetCell(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;

			if (ii < formR->scrollPos)
				continue;

			width += cellR->width;
			if (width >= tRc.right)
			{
				delete cellItem;
				return -1;
			}

			if (point.x >= width-2 && point.x <= width+2)
			{
				if ((formR->attr2 & GO_HSCR) &&
					point.y >= formR->bottom - szSCROLL)
				{
					delete cellItem;
					return -1;
				}

				rc.left  = width-1;
				rc.right = width+1;
				delete cellItem;
				return ii;
			}

			if (cellR->attr & FA_WRAP)
				width = tRc.left;
		}
		break;

	case FM_TABLE:
		if (!(formR->attr2 & GO_MULTI) || !repR->rows)
		{
			delete cellItem;
			return -1;
		}

		rc.left += cellItem->GetCell(0)->width - 2;
		rc.right = rc.left + 2;
		tRc = rc;
		tRc.InflateRect(2, 0);
		if (tRc.PtInRect(point))
		{
			delete cellItem;
			return 0;
		}
		break;
	}

	delete cellItem;
	return -1;
}

int CTlGrid::selectable(CPoint point, CRect& rc, CPoint &pt, bool bSet)
{
	CRect	tRc;
	int	ii, width, line, drawRows, headHeight, multi;
	struct	_formR*	formR = m_formItem->GetForm(m_fN);
	struct	_repR*	repR = (struct _repR *)formR->auxR;

	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	rc    = tRc;
	width = tRc.left;
	CCellItem *cellItem = new CCellItem(repR);
	switch (formR->kind)
	{
	case FM_GRID:
	case FM_GRIDEX:
		line = multi = 0;
		for (ii = 0; ii < repR->cols; ii++)
		{
			if (formR->attr2 & GO_MULTI)
				multi = max(multi, (int)cellItem->GetCell(ii)->lineN);
		}
		multi++;

		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR *cellR = cellItem->GetCell(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;

			if (ii < formR->scrollPos)
				continue;

			if (line != cellR->lineN)
			{
				line++;
				width = tRc.left;
			}

			drawRows = repR->rows;
			headHeight = LOWORD(formR->keys);
			if (formR->attr2 & GO_AUTOSIZE)
			{
				drawRows = HIWORD(formR->keys);
				headHeight = (formR->bottom - formR->top) / (drawRows * multi);
			}
			
			int height = tRc.top + line * headHeight;
			rc.SetRect(width, height, width + cellR->width, headHeight + height);

			if (formR->attr2 & GO_XHEAD)
				rc.bottom = headHeight * drawRows + height;
			if (rc.bottom > formR->bottom)
				rc.bottom = formR->bottom;
			if (formR->attr2 & GO_HSCR)
				if (rc.bottom + szSCROLL > formR->bottom)
					rc.bottom -= szSCROLL;

			if (rc.PtInRect(point))
			{
				pt = CPoint(rc.left, rc.top);
				delete cellItem;
				if (bSet)
					repR->selected = ii;
				return ii;
			}
						
			width += cellR->width;
			if (width >= tRc.right)
				width  = tRc.right;
		}
		pt = CPoint(rc.right, rc.top);
		if (formR->attr2 & GO_HSCR)
		{
			CRect tRc1, tRc2;
			if  (formR->attr2 & GO_VSCR)
				tRc.right -= szSCROLL;
			
			if (tRc.Height() > szSCROLL)
				tRc.top = tRc.bottom - szSCROLL;
			
			tRc1 = tRc2 = tRc;
			if (tRc.Width() < szSCROLL*2)
			{
				tRc1.right = tRc.left + tRc.Width()/2;
				tRc2.left  = tRc.left + tRc.Width()/2+1;
			}
			else
			{
				tRc1.right = tRc.left + szSCROLL;
				tRc2.left  = tRc.right -szSCROLL;
			}

			if (tRc1.PtInRect(point) && !bSet)
			{
				formR->scrollPos--;
				if (formR->scrollPos < 0)
					formR->scrollPos = 0;
				delete cellItem;
				return -1;
			}
			else if (tRc2.PtInRect(point) && !bSet)
			{
				formR->scrollPos++;
				for (ii = formR->scrollPos; ii < repR->cols; ii++)
				{
					struct _cellR *cellR = cellItem->GetCell(ii);
					if (!(cellR->properties & PR_VISIBLE))
						continue;
					tRc.right -= cellR->width;
				}
				if (tRc.Width() > 0)
					formR->scrollPos--;
				delete cellItem;
				return -1;
			}
		}
		repR->selected = -1;
		break;

	case FM_TABLE:
		if (!repR->rows)
		{
			delete cellItem;
			return -1;
		}
		struct _cellR	*cellR = cellItem->GetCell(0);
		if (formR->attr2 & GO_MULTI)
			rc.right = rc.left + cellR->width;
		tRc = rc;
		{
			for (int ii = 0; ii < repR->rows; ii++)
			{
				tRc.top = rc.top + ii * (rc.Height() / repR->rows);
				tRc.bottom = rc.top + (ii + 1) * (rc.Height() / repR->rows);
				if (tRc.PtInRect(point))
				{
					pt = CPoint(tRc.left, tRc.top);
					if (bSet)
						repR->selected = ii;
					delete cellItem;
					return ii;
				}
			}
		}
		pt = CPoint(tRc.left, tRc.bottom);
		break;
	}

	delete cellItem;
	return -1;
}

void CTlGrid::drawBegin(CPoint point)
{
	if (!m_view->m_move && !m_view->m_bSelect)
	{
		CTlRect::drawBegin(point);
		return;
	}
	
	m_fN = m_view->m_selF;
	m_column = movable(point, m_rect);
	if (m_column >= 0)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
		m_delta = m_rect.left - point.x;
		m_dc->SelectStockObject(NULL_PEN);
		m_dc->SelectStockObject(DKGRAY_BRUSH);
		m_dc->SetROP2(R2_NOT);
		m_dc->Rectangle(m_rect);
		return;
	}

	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	m_selected = selectable(point, m_rect, m_toPt);
	m_view->InvalidateRect(getRect());
}

void CTlGrid::drawTo(CPoint point)
{
	if (!m_view->m_move && !m_view->m_bSelect)
	{
		CTlRect::drawTo(point);
		return;
	}

	m_fN = m_view->m_selF;
	if (m_column >= 0)
	{
		m_dc->Rectangle(m_rect);
		int	gap = m_rect.Width();
		m_rect.left  = point.x + m_delta;
		m_rect.right = m_rect.left + gap;
		m_dc->Rectangle(m_rect);

		int nColWidth = 5, nDelta = m_endpt.x - m_lastpt.x;
		struct	_repR*	repR = (struct _repR *)m_formItem->GetForm(m_view->m_selF)->auxR;
		CCellItem cellItem(repR);

		switch (m_type)
		{
		case tbGRID:
			if (nDelta >= 0)	// right
				nColWidth = cellItem.GetCell(m_column)->width + nDelta;
			else			// left
			{
				if (abs(nDelta) >= cellItem.GetCell(m_column)->width)
					nColWidth = 5;
				else
					nColWidth = cellItem.GetCell(m_column)->width + nDelta;
			}
			break;
			
		case tbTABLE:
			if (nDelta >= 0)	// right
				nColWidth = cellItem.GetCell(0)->width + nDelta;
			else			// left
			{
				if (abs(nDelta) >= cellItem.GetCell(0)->width)
					nColWidth = 5;
				else
					nColWidth = cellItem.GetCell(0)->width + nDelta;
			}
			break;
		}
		CString sTip;
		sTip.Format(_T("%d"), nColWidth);
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 1), (LPARAM)sTip.operator LPCTSTR());
	}
	else
	{
		selectable(point, m_rect, m_toPt, false);
		if (m_markWnd)
		{
			if (m_type == tbGRID)
				m_rect.InflateRect(0, 0, 100, 0);
			else
				m_rect.InflateRect(0, 0, 0, 100);

			if (m_rect.PtInRect(point))
			{
				if (m_type == tbGRID)
					m_markWnd->SetWindowPos(NULL, m_toPt.x - 5, m_toPt.y - 6, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
				else
					m_markWnd->SetWindowPos(NULL, m_toPt.x - 6, m_toPt.y - 5, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
				SetCursor(AfxGetApp()->LoadCursor(IDC_DROPCOPY));
			}
			else
			{
				m_markWnd->ShowWindow(SW_HIDE);
				SetCursor(AfxGetApp()->LoadCursor(IDC_NODROPCOPY));
			}
		}
	}
}

void CTlGrid::drawEnd()
{
	struct	_formR*	formR = m_formItem->GetForm(m_fN);

	if (m_markWnd)
	{
		m_markWnd->DestroyWindow();
		delete m_markWnd;
		m_markWnd = NULL;
	}

	if (!m_view->m_move && !m_view->m_bSelect)
	{
		CTlRect::drawEnd();

		if (m_rect.Width() < 10 || m_rect.Height() < 10)
		{
			CString sCtrlName = (m_type == tbTABLE) ? _T("TABLE") : _T("GRID");
			m_rect.right  = m_rect.left + getDefaultWidth(sCtrlName, 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight(sCtrlName, 120);
		}

		CRect	wRc;
		m_view->GetClientRect(&wRc);
		m_rect.right  = __min(m_rect.right,  wRc.right);
		m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

		formR->left   = m_rect.left;
		formR->top    = m_rect.top;
		formR->right  = m_rect.right;
		formR->bottom = m_rect.bottom;
		formR->alpha = 100;			// alpha default setting

		switch (m_type)
		{

		case tbGRID:
			formR->kind = FM_GRID;
			formR->auxR = NULL;
			formR->bRGB = 216;
			break;

		case tbGRIDEX:
			formR->kind = FM_GRIDEX;
			formR->auxR = NULL;
			formR->bRGB = 216;
			break;
		
		case tbTABLE:
			formR->kind = FM_TABLE;
			formR->iok  = EIO_OUTPUT;
			formR->bRGB = getDefaultBRGB(_T("TABLE"), 232);
			formR->keys = 20;
			formR->attr2 |= GO_AUTOSIZE;
			formR->auxR = NULL;
			formR->bRGB = 216;
			break;
		
		default:
			break;
		}

		m_dc->Rectangle(m_rect);
		return;
	}

	m_fN = m_view->m_selF;
	if (m_column >= 0)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 0), NULL);
		m_dc->Rectangle(m_rect);
		m_delta = m_endpt.x - m_lastpt.x;
		
		int	ii;
		struct	_repR*	repR = (struct _repR *)formR->auxR;
		CCellItem cellItem(repR);
		
		switch (m_type)
		{
		case tbGRID:
			if (m_delta >= 0)	// right
				cellItem.GetCell(m_column)->width += m_delta;
			else			// left
			{
				if (abs(m_delta) >= cellItem.GetCell(m_column)->width)
					cellItem.GetCell(m_column)->width = 5;
				else
					cellItem.GetCell(m_column)->width += m_delta;
			}
			break;
			
		case tbTABLE:
			if (m_delta >= 0)	// right
				cellItem.GetCell(0)->width += m_delta;
			else			// left
			{
				if (abs(m_delta) >= cellItem.GetCell(0)->width)
					cellItem.GetCell(0)->width = 5;
				else
					cellItem.GetCell(0)->width += m_delta;
			}
			for (ii = 1; ii < repR->rows; ii++)
				cellItem.GetCell(ii)->width = cellItem.GetCell(0)->width;
			break;
		}
	}
	else if (m_view->m_bSelect)
	{
		CRect tRc;
		int sel = selectable(m_endpt, tRc, m_toPt);
		
		if (m_type == tbGRID)
			tRc.InflateRect(0, 0, 100, 0);
		else
			tRc.InflateRect(0, 0, 0, 100);

		if (tRc.PtInRect(m_endpt))
		{
			if (m_selected >= 0 && (m_selected > sel || sel - 1 != m_selected))
			{
				struct	_repR* repR = (struct _repR *)formR->auxR;
				CCellItem cellItem(repR);

				if (sel == -1)
				{
					sel = cellItem.GetCount();
					if (m_selected == sel - 1)
						sel = m_selected + 1;
				}
				if (m_selected > sel || m_selected != sel - 1)
				{
					cellItem.MoveCell(m_selected, sel);
					if (m_selected < sel)
						repR->selected = sel - 1;
				}
			}
			else
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, FALSE);
		}
	}

	m_rect = getRect();
	m_view->InvalidateRect(&m_rect);
}

void CTlGrid::drawGRID(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	cRect, hRect, selRect;
	CRect	fRc, tRc, tRc1, tRc2;
	int	ptr;
	int	line, multi, jj, kk, drawRows, nSlit = 0;
	int	dx, cy, dy, nHeadDy = 0;	// nHeadDy : AUTOSIZE일때 늘어난 headSize
	int	len, linecnt, colidx, colcnt;
	CString	str, text;
	UINT	style;
	struct	_repR*	repR;
	struct	_formR*	formR = m_formItem->GetForm(fN);
	bool	even = true;


	// dx 초기화
	dx = 0;

	selRect.SetRectEmpty();
	tRc = fRc = getRect();
	setFont(dc);
	setPen(dc, getRGB(formR->bRGB));

	if (tRc.Height() == 0 || tRc.Width() == 0)
		return;

	if (formR->auxR == (char *)0)
		return;

	repR = (struct _repR *)formR->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	
	drawRows = repR->rows;
	
	if (formR->attr2 & GO_AUTOSIZE)
		drawRows = HIWORD(formR->keys);

	if (repR->cols != 0/* && drawRows != 0*/)
	{
		line = 1;
		multi = 0;
		for (int ii = 0; ii < repR->cols; ii++)
		{
			CString tHead = (LPCTSTR)cellItem->GetCell(ii)->head;
			if (!(formR->attr2 & GO_MERGE))
				line = max(line, tHead.Replace(_T("~"), _T("~")) + 1);//(int)cellItem->getCellPoint(ii)->headN);
			if (formR->attr2 & GO_MULTI)
				multi = max(multi, (int)cellItem->GetCell(ii)->lineN);
			else
				cellItem->GetCell(ii)->lineN = 0;
		}
		multi++;

		cy = LOWORD(formR->keys);

		dy = tRc.top;
		if (LOWORD(formR->keys) <= 0)
			formR->keys = MAKELONG(m_fheight + 4, HIWORD(formR->keys));
				
		linecnt = 0;
		colidx  = 0;
		if (formR->attr2 & GO_XHEAD)
			line = 0;

		if (!(formR->attr2 & GO_XHLINE))
			nSlit = 1;

		m_maxRow = 1;
		colcnt = 0;

		for (kk = colidx; kk < repR->cols; kk++)
		{
			CString tmp = (LPCTSTR)cellItem->GetCell(kk)->head;
			if (formR->attr2 & GO_MERGE)
				m_maxRow = max(m_maxRow, tmp.Replace(_T(","), _T(",")) + 1);
		}
		if (formR->attr2 & GO_AUTOSIZE)
			drawRows += m_maxRow - 1;

		for (int ii = 0; ii < (drawRows+line)*multi; ii++)
		{
			dx = 0;			
			
			if (formR->attr2 & GO_AUTOSIZE)
			{
				int	nTop, nBottom;
				
				nTop = formR->top;
				nBottom = formR->bottom;
				cRect.top = nTop + (nBottom - nTop) * ii / ((drawRows+line)*multi);
				dy = nTop + (nBottom - nTop) * (ii + 1) / ((drawRows+line)*multi) - 1;
				if (formR->attr2 & GO_MERGE)
				{
					if (!(ii < multi && !(formR->attr2 & GO_XHEAD)) && m_maxRow > 1)
					{
						cRect.top += nHeadDy + /*nSlit **/ m_maxRow;
						dy += nHeadDy;
					}
					else
						nHeadDy = (dy - cRect.top + 1) * (m_maxRow - 1);
				}
				dy += nSlit * m_maxRow;
			}
			else
			{
				cRect.top = dy;
				// Grid Header의 크기 지정 시
				if (multi > ii && formR->headH > 0 && !(formR->attr2 & GO_XHEAD))
				{
					cy = formR->headH;
					dy += cy + nSlit;
				}
				else
				{
					cy = LOWORD(formR->keys);
					dy += cy + nSlit;
				}					
			}

			if (dy > tRc.bottom)
			{
				dy = tRc.bottom;
				ii = (drawRows+line)*multi + multi;
			}

			colcnt = 0;
			for (kk = colidx; kk < repR->cols; kk++)
			{
				if (cellItem->GetCell(kk)->lineN == linecnt)
					colcnt++;
			}
						
			for (jj = colidx; jj < colidx+colcnt; jj++)
			{
				struct _cellR *cellR = cellItem->GetCell(jj);
				int	tmpH = 0;
				int	headH = 0;

				if (!(cellR->properties & PR_VISIBLE))
					continue;
				if (jj < formR->scrollPos)
					continue;

				if (cellR->width == 0)
					cellR->width = (WORD)(cellR->size * m_fwidth);

				cRect.left = tRc.left + dx;
				dx = dx + cellR->width;

				// Font Size에 따른 Auto Size
				/*CSize size = dc->GetOutputTextExtent(_T("column"));	// 글자 크기
				if (dx < size.cx)
					dx = size.cx * 1.15;*/
	
				if (dx <= tRc.Width())
					cRect.right = tRc.left + dx;
				else
					cRect.right = tRc.right;

				if (((formR->attr2 & GO_MERGE) && ii >= multi &&	// ii < multi is head
					!(formR->attr2 & GO_AUTOSIZE)) || (!(formR->attr2 & GO_MERGE) && ii >= multi))
					cRect.bottom = dy - nSlit;
				else
					cRect.bottom = dy;
				
				//tmpH = cRect.Height();

				//if (formR->headH > 0)
				//	headH = formR->headH - tmpH;

				if (ii == (drawRows+line)*multi - 1) // last row
				{
					
				}

				if (ii < multi && !(formR->attr2 & GO_XHEAD))	// ishead?
				{
					CRect	tmpRc;
					CString	sUHead = (LPCTSTR)cellR->head;
					int	nMaxRow = 1;

					if (formR->attr2 & GO_MERGE)
					{
						nMaxRow = m_maxRow;//sUHead.Replace("|", "|") + 1;
						for (int kk = 0; kk < nMaxRow - sUHead.Replace(_T(","), _T(",")) - 1; kk++)
							sUHead += _T(",");
					}

					if (formR->attr2 & GO_AUTOSIZE)
						cRect.bottom = formR->top + (formR->bottom - formR->top) * (ii + 1) / ((drawRows+line)*multi) * line * m_maxRow + m_maxRow + headH;	// head height
					else
						cRect.bottom = cRect.top + cy * line * m_maxRow + nSlit * nMaxRow;	// head height
					
					if (!(formR->attr2 & GO_AUTOSIZE))
						cRect.bottom += nMaxRow;
					dy = cRect.bottom;

					for (int nRow = 0; nRow < nMaxRow; nRow++)
					{
						CString tCap = sUHead;
						bool bTop = true;
						if (formR->attr2 & GO_MERGE)
							tCap = parse(sUHead, _T(","));
						tmpRc = cRect;
						dc->IntersectClipRect(tmpRc);

						if (formR->attr2 & GO_MERGE)
							cRect = getHeadRect(jj, nRow, cRect, tCap);

						if (cRect.top != tmpRc.top)
							cRect.top--;

						brush = setBrush(dc, getRGB(cellR->hpRGB));
						if (repR->selected == jj)
						{
							brush = setBrush(dc, GetSysColor(COLOR_HIGHLIGHT));
							dc->FillRect(cRect, brush);
							cRect.DeflateRect(4, 4, 3, 3);
							brush = setBrush(dc, getRGB(cellR->hpRGB));
							dc->FillRect(cRect, brush);
							cRect.InflateRect(4, 4, 3, 3);
						}
						else
							dc->FillRect(cRect, brush);
						setTextColor(dc, getRGB(cellR->htRGB));
						dc->SetBkMode(TRANSPARENT);
						
						hRect = cRect;
						hRect.bottom = cRect.top;
						if (!tCap.IsEmpty())
						{
							str = tCap;
							for (kk = 0; kk < tCap.Replace(_T("~"), _T("~")) + 1; kk++)
							{
								hRect.left += 2;
								hRect.right -= 2;
								hRect.top = hRect.bottom;
								hRect.bottom = cRect.top+ cRect.Height()*(kk+1)/(tCap.Replace(_T("~"), _T("~")) + 1);
								
								ptr = str.Find(_T('~'));
								switch (cellR->alignH)
								{
								case AL_LEFT:
									style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;	break;
								case AL_RIGHT:
									style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;	break;
								case AL_CENTER:
								default:
									style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;	break;
								}
								if (ptr != -1)
								{
									text = str.Left(ptr);
									str = str.Mid(ptr+1);
								}
								else
									text = str;
								
								editAmpersand(text);
								if (!kk && cellR->properties & PR_SORT)
									drawHead(dc, jj, text, hRect, style);
								else
									dc->DrawText(text, hRect, style);
							}
						}
						if (!nRow)
							dx--;
						
						setPen(dc, getRGB(formR->bRGB));
						dc->SelectStockObject(NULL_BRUSH);
						if (jj != colidx)
							cRect.left--;
						dc->Rectangle(cRect);

						// Inner Line
						if ((formR->attr2 & GO_XINLINE) && colidx)
						{
							setPen(dc, getRGB(formR->pRGB));
							dc->SelectStockObject(NULL_BRUSH);
							dc->MoveTo(cRect.left, cRect.top);
							dc->LineTo(cRect.right - 1, cRect.top);
						}

							//g.DrawLine(setgPen(GRGB(pRGB)), tRc.left, tRc.bottom, tRc.right, tRc.bottom);

						if (ii != 0)
							cRect.top++;

						dc->SelectClipRgn(NULL);
						cRect = tmpRc;
					}
					if (dx <= tRc.Width())
						dx++;
					else
					{
						dx = -1;
						jj = colidx+colcnt;
					}
				}
				else	// data
				{
					//
					setCellFont(dc, cellR);

					if (repR->selected == jj)
						selRect.UnionRect(selRect, cRect);
					dc->MoveTo(cRect.left, cRect.bottom);
					if (formR->attr2 & GO_ROW)
						brush = setBrush(dc, getRGB(even ? formR->spRGB : formR->stRGB));
					else
						brush = setBrush(dc, getRGB(cellR->pRGB));

					if (ii != 0)
						cRect.bottom++;
					dc->FillRect(cRect, brush);
					cRect.bottom--;

					setTextColor(dc, getRGB(cellR->tRGB));
					dc->SetBkMode(TRANSPARENT);

					switch (cellR->alignC)
					{
					case AL_CENTER:
						style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;
						break;
					case AL_RIGHT:
						style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;
						break;
					case AL_LEFT:
					default :
						style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
						break;
					}

					hRect = cRect;
					hRect.left += 3;
					hRect.right -= (int)m_fwidth/2;
					text = _T("");
					if (cellR->size != 0)
					{
						len = (int)(cellR->size - cellR->vals);
						//if (cellR->isEdit == IE_EDIT)
						if (!CString(cellR->editS).IsEmpty())
							text = CString(cellR->editS);
						else
						{
							if (cellR->attr & FA_PASSWD)
								text = CString(_T('*'), cellR->size);
							else if (cellR->type == IO_NUM
								&& cellR->attr & FA_COMMA)
							{
								editComma(text, len);
								if (len != cellR->size)
								{
									CString	tmps = _T("");
									text += _T('.');
									setInOutFace(tmps, cellR->vals, IO_NUM);
									text += tmps;
								}
							}
							else
							{
								setInOutFace(text, len, cellR->type);
								if (len != cellR->size)
								{
									CString	tmps = _T("");
									text += _T('.');
									setInOutFace(tmps, cellR->vals, IO_NUM);
									text += tmps;
								}
							}
						}
					}
					else
						text = COLUMNFACE;
										
					switch (cellR->kind)
					{
					case CK_COMBO:
						drawCOMBO(dc, cRect, text);
						break;
					case CK_CHECK:
						{
							CString	path = _T("");
							CString text = (LPCTSTR)cellR->str2;
							if (cellR->properties & PR_IMAGE || cellR->properties & PR_IMAGETEXT)
							{							
								if (strlen(cellR->str3) > 0)
								{
									path = cellR->str3;
									getPath(path);
								}
								if (cellR->properties & PR_IMAGE)
									text = _T("");
							}

							drawCHECK(dc, cRect, text, path, cellR->alignC);
						}
						break;
					case CK_BUTTON:
						brush = setBrush(dc, getRGB(cellR->pRGB));
						dc->FillRect(cRect, brush);
						if (cellR->properties & PR_IMAGE || cellR->properties & PR_IMAGETEXT)
						{
							cRect.DeflateRect(1, 1);
							if (strlen(cellR->str3) > 0)
							{
								if (m_image)
								{
									delete m_image;
									m_image = NULL;
								}
								CString	path = (LPCTSTR)cellR->str3;
								getPath(path);
								if (!path.IsEmpty())
								{
									drawPNG(dc, cRect, path);
									dc->SetBkMode(TRANSPARENT);
								}
							}
							else
								drawPNG(dc, cRect, ((cellR->properties & PR_IMAGETEXT) ? _T("") : _T("IMAGE")));
							cRect.InflateRect(1, 1);
						}
						if (!(cellR->properties & PR_IMAGE))
						{
							if (cellR->properties & PR_IMAGETEXT)
								style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;
							cRect.DeflateRect(1, 0);
							dc->SetBkMode(TRANSPARENT);
							dc->SetTextColor(getRGB(cellR->tRGB));
							dc->DrawText((LPCTSTR)cellR->txt, cRect, style);
							cRect.InflateRect(1, 0);
						}
						drawEdge(dc, cRect, EDGE_RAISED);
						break;
					case CK_CUSTOM:
						text = CString(cellR->dat);
					default:
						hRect.DeflateRect(1, 0);
						dc->DrawText(text, hRect, style);
						hRect.InflateRect(1, 0);
						break;
					}

					setPen(dc, getRGB(formR->bRGB));
					if (dx <= tRc.Width())
					{
						cRect.right--;
						if (!(formR->attr2 & GO_XHLINE))
						{
							if ((formR->attr2 & GO_XINLINE) && (ii % multi) != (multi-1))
							{
								if (formR->attr2 & GO_ROW)
									setPen(dc, getRGB(even ? formR->spRGB : formR->stRGB));
								else
									setPen(dc, getRGB(cellR->pRGB));
							}
							
							dc->MoveTo(cRect.left, cRect.bottom);
							dc->LineTo(cRect.right, cRect.bottom);

							setPen(dc, getRGB(formR->bRGB));
						}
						if (!(formR->attr2 & GO_XVLINE))
						{
							dc->MoveTo(cRect.right, cRect.top);
							dc->LineTo(cRect.right, cRect.bottom+1);
						}
					}
					else
					{
						if (!(formR->attr2 & GO_XHLINE))
							dc->LineTo(cRect.right, cRect.bottom);
						dx = -1;
						jj = colidx+colcnt;
					}
				}
				setFont(dc);
			}	// jj loop
			
			linecnt++;
			colidx += colcnt;

			if (linecnt == multi)
				linecnt = 0;
			if (colidx == repR->cols)
			{
				colidx = 0;
				if (ii >= multi || (formR->attr2 & GO_XHEAD))
					even = !even;
			}
		}	// ii loop
	}
	else
	{
		setPen(dc, getRGB(formR->bRGB));	// outline
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(fRc);
		delete cellItem;
		return;
	}
	
	// selected draw
	if (formR->attr2 & GO_XHEAD)
		drawFocusRect(dc, selRect);

	formR->attr2 &= ~GO_VSCR;
	formR->attr2 &= ~GO_HSCR;
	// draw auto-hscroll, auto-vscroll
	if ((dx != -1) && !(formR->attr2 & GO_MULTI))
	{
		dx = 0;
		for (int ii = 0; ii < repR->cols; ii++)
		{
			if (cellItem->GetCell(ii)->properties & PR_VISIBLE)
				dx += cellItem->GetCell(ii)->width;
		}
		if (((cy*(drawRows+line)*multi) > tRc.Height()) && (dx > (tRc.Width() - szSCROLL)))
			dx = -1;
		else if (dx > tRc.Width())
			dx = -1;
	}

	if (dx == -1 && !(formR->attr2 & GO_MULTI))
	{
		formR->attr2 |= GO_HSCR;
		if  ((cy*(drawRows+line)*multi) > tRc.Height()-szSCROLL)
			tRc.right -= szSCROLL;

		if (tRc.Height() > szSCROLL)
			tRc.top = tRc.bottom - szSCROLL;

		tRc1 = tRc2 = tRc;
		if (tRc.Width() < szSCROLL*2)
		{
			tRc1.right = tRc.left + tRc.Width()/2;
			tRc2.left  = tRc.left + tRc.Width()/2+1;
		}
		else
		{
			tRc1.right = tRc.left + szSCROLL;
			tRc2.left  = tRc.right -szSCROLL;
		}
		tRc.right = fRc.right;
		brush = setBrush(dc, clLGRAY);
		dc->FillRect(tRc, brush);
		dc->DrawFrameControl(tRc1, DFC_SCROLL, DFCS_SCROLLLEFT);
		dc->DrawFrameControl(tRc2, DFC_SCROLL, DFCS_SCROLLRIGHT);
	}

	tRc = fRc;
	if (formR->attr2 & GO_HSCR)
		tRc.bottom -= szSCROLL;

	if ((!(formR->attr2 & GO_AUTOSIZE) && (cy*(drawRows+line)*multi) > tRc.Height()) ||
		((formR->attr2 & GO_AUTOSIZE) && HIWORD(formR->keys) < repR->rows))
	{
		float	bH;
		CRect	pRc1, pRc2, pRc3;

		formR->attr2 |= GO_VSCR;
		//formR->attr2 &= ~GO_PAGE;
		if (tRc.Width() > szSCROLL)
			tRc.left  = tRc.right  - szSCROLL;

		tRc1 = tRc2 = tRc;
		pRc1 = pRc2 = pRc3 = tRc;
		if (tRc.Height() < 5 * szSCROLL)
         	{
			bH = (float)(tRc.Height() / 5);
          		tRc1.bottom = tRc.top + (int)bH;
			pRc1.top    = tRc1.bottom;
			pRc1.bottom = pRc1.top + (int)bH;
			pRc2.top    = pRc1.bottom;
			pRc2.bottom = pRc2.top + (int)bH;
			pRc3.top    = pRc2.bottom;
			pRc3.bottom = pRc3.top + (int)bH;
           		tRc2.top    = pRc3.bottom;
           	}
            	else 
             	{
              		tRc1.bottom = tRc.top + szSCROLL;
			pRc1.top    = tRc1.bottom;
			pRc1.bottom = pRc1.top + szSCROLL;
			pRc2.top    = pRc1.bottom;
			pRc2.bottom = pRc2.top + szSCROLL;
			pRc3.top    = pRc2.bottom;
			pRc3.bottom = pRc3.top + szSCROLL;
                 	tRc2.top    = tRc.bottom - szSCROLL;
             	}

		brush = setBrush(dc, clLGRAY);
		dc->FillRect(tRc, brush);
		dc->DrawFrameControl(tRc1, DFC_SCROLL, DFCS_SCROLLUP);
		dc->DrawFrameControl(tRc2, DFC_SCROLL, DFCS_SCROLLDOWN);
	}

	setPen(dc, getRGB(formR->bRGB));	// outline
	dc->SelectStockObject(NULL_BRUSH);
	dc->Rectangle(fRc);
	delete cellItem;
}

void CTlGrid::drawTABLE(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	cRect, hRect;
	CRect	fRc, tRc, tRc1;
	int	slit, multi, dx, dy;
	int	len, linecnt;
	float	scy, sdy;
	CString	str, text;
	UINT	style;
	struct	_repR*	repR;
	struct	_formR*	formR = m_formItem->GetForm(m_fN);

	tRc = fRc = getRect();
	setFont(dc);
	setPen(dc, getRGB(formR->bRGB), formR->keys);
	if (tRc.Height() == 0 || tRc.Width() == 0)
		return;

	if (formR->auxR == (char *) 0)
		return;

	linecnt = 0;
	repR = (struct _repR *)formR->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	for (int ii = 0; ii < repR->rows; ii++)
	{
		if (cellItem->GetCell(ii)->properties & PR_VISIBLE)
			linecnt++;
	}

	multi = 0;	// BOTH
	if (!(formR->attr2 & GO_MULTI))
		multi = (formR->attr2 & GO_XHEAD) ? 1 : 2;

	dy = 1;
	slit = formR->keys;
	/*if (slit > 1)
	{
		float	fy = (float)slit / (float)2.0;
		dy = (int) fy;
		if (fy - (float)dy >= 0.5)
			dy++;
	}*/
	
	if (repR->rows != 0)
	{
		if (!(formR->attr2 & GO_AUTOSIZE) && formR->bottom != formR->top + (slit + 1) * repR->rows)
		{
			formR->bottom = formR->top + (slit + 1) * repR->rows;
			AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
			m_view->Invalidate();
		}

		sdy = (float)tRc.top + 1;//slit;
		if (!(formR->attr2 & GO_AUTOSIZE))
			scy = (float)slit;
		else
		{
			scy = (float)((float)(tRc.Height() - /*slit**/(linecnt+1)) / (float)linecnt);
			if ((int)(scy + 0.5) == (int)scy)
				scy = (float)(int)scy;
			else
				scy = (float)(int)scy + 1;
		}
		for (int ii = 0; ii < repR->rows; ii++)
		{
			struct _cellR *cellR = cellItem->GetCell(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;

			dx = 0;
			cRect.top = (int)sdy;
			sdy += scy;

			if (sdy > tRc.bottom-/*slit*/1 || ii == repR->rows-1)
				sdy = (float)tRc.bottom - /*slit*/1;

			cRect.left   = tRc.left;
			cRect.bottom = (int)sdy;
			sdy += /*slit*/1;
			if (multi == 0 || multi == 2)
			{
				if (multi == 0)
				{
					cRect.right = tRc.left + cellR->width;
					cRect.right = min(cRect.right, tRc.right-/*slit*/1);
				}
				else
					cRect.right = tRc.right-/*slit*/1;

				dx = cRect.right;
				brush = setBrush(dc, getRGB(cellR->hpRGB));
				if (repR->selected == ii)
				{
					brush = setBrush(dc, GetSysColor(COLOR_HIGHLIGHT));
					cRect.bottom++;
					dc->FillRect(cRect, brush);
					cRect.DeflateRect(3, 2, 2, 3);
					brush = setBrush(dc, getRGB(cellR->hpRGB));
					dc->FillRect(cRect, brush);
					cRect.InflateRect(3, 2, 2, 3);
					cRect.bottom--;
				}
				else
				{
					cRect.bottom++;
					dc->FillRect(cRect, brush);
					cRect.bottom--;
				}
				setTextColor(dc, getRGB(cellR->htRGB));
				//if (repR->selected == ii)
				//	setTextColor(dc, GetSysColor(COLOR_HIGHLIGHTTEXT));
				
				dc->SetBkMode(TRANSPARENT);

				if (cellR->head[0] != NULL)
				{
					switch (cellR->alignH)
					{
					case AL_LEFT:
						style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;	break;
					case AL_RIGHT:
						style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;	break;
					case AL_CENTER:
					default:
						style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;	break;
					}
					str = cellR->head;
					editAmpersand(str);
					tRc1 = cRect;
					tRc1.DeflateRect(3, 0);
					dc->DrawText(str, tRc1, style);
				}
				cRect.left  = dx;
			}
			cRect.right = tRc.right - 1;

			if (cRect.left == cRect.right || multi == 2)
			{
				if (ii != repR->rows - 1 && !(formR->attr2 & GO_XHLINE))
				{
					setPen(dc, getRGB(formR->bRGB), /*slit*/1);
					dc->MoveTo(tRc.left+dy, (int)sdy-dy);
					dc->LineTo(cRect.right, (int)sdy-dy);
				}
				continue;
			}

			brush = setBrush(dc, getRGB(cellR->pRGB));
			if (multi == 1 && repR->selected == ii)
			{
				brush = setBrush(dc, GetSysColor(COLOR_HIGHLIGHT));
				cRect.bottom++;
				dc->FillRect(cRect, brush);
				cRect.DeflateRect(3, 2, 2, 3);
				brush = setBrush(dc, getRGB(cellR->pRGB));
				dc->FillRect(cRect, brush);
				cRect.InflateRect(3, 2, 2, 3);
				cRect.bottom--;
			}
			else
			{
				cRect.bottom++;
				dc->FillRect(cRect, brush);
				cRect.bottom--;
			}
			setTextColor(dc, getRGB(cellR->tRGB));
			dc->SetBkMode(TRANSPARENT);

			switch (cellR->alignC)
			{
			case AL_CENTER:
				style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;
				break;
			case AL_RIGHT:
				style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;
				break;
			case AL_LEFT:
			default:
				style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
				break;
			}

			text = _T("");
			if (cellR->size != 0)
			{
				len = (int)(cellR->size - cellR->vals);
				//if (cellR->isEdit == IE_EDIT)
				if (!CString(cellR->editS).IsEmpty())
					text = CString(cellR->editS);
				else
				{
					if (cellR->attr & FA_PASSWD)
						text = CString(_T('*'), cellR->size);
					else if (cellR->type == IO_NUM && cellR->attr & FA_COMMA)
					{
						editComma(text, len);
						if (len != cellR->size)
						{
							CString	tmps = _T("");
							text += _T('.');
							setInOutFace(tmps, cellR->vals, IO_NUM);
							text += tmps;
						}
					}
					else
					{
						setInOutFace(text, len, cellR->type);
						if (len != cellR->size)
						{
							CString	tmps = _T("");
							text += _T('.');
							setInOutFace(tmps, cellR->vals, IO_NUM);
							text += tmps;
						}
					}
				}
			}
			tRc1 = cRect;
			tRc1.left  += 3;
			tRc1.right -= (int)(m_fwidth/2);
			dc->DrawText(text, tRc1, style);
			cRect.right--;
			if (ii != repR->rows-1 && !(formR->attr2 & GO_XHLINE))
			{
				setPen(dc, getRGB(formR->bRGB), /*slit*/1);
				dc->MoveTo(tRc.left+dy, (int)sdy-dy);
				dc->LineTo(cRect.right, (int)sdy-dy);
			}
		}

		if (dx != tRc.right && dx != 0 && !(formR->attr2 & GO_XVLINE))
		{
			setPen(dc, getRGB(formR->bRGB), /*slit*/1);
			dc->MoveTo(dx, tRc.top+/*slit*/1);
			dc->LineTo(dx, tRc.bottom-/*slit*/1);
		}
	}

	setPen(dc, getRGB(formR->bRGB), /*slit*/1);
	dc->SelectStockObject(NULL_BRUSH);
	dy = 0;
	/*if (slit > 1)
		dy = slit / 2;*/
	dc->Rectangle(tRc.left+dy, tRc.top+dy, tRc.right-dy, tRc.bottom-dy);
	delete cellItem;
}

void CTlGrid::drawCOMBO(CDC* dc, CRect rc, CString text)
{
	CRect	tRc = rc;
	drawEdge(dc, rc, EDGE_SUNKEN);
	tRc.left = tRc.left + 2;
	dc->DrawText(text, tRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

	tRc.right  = tRc.right - 2;
	tRc.left   = tRc.right - szSCROLL;
	tRc.top    = tRc.top + 2;
	tRc.bottom = tRc.bottom - 2;
	dc->DrawFrameControl(tRc, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
}

void CTlGrid::drawCHECK(CDC* dc, CRect rc, CString text, CString imgPath, BYTE alignC)
{
	CRect	tRc, tRc1, tRc2;
	CString tmps;
	CSize size;
	int index;

	tRc = tRc1 = tRc2 = rc;

	tmps = text;
	tmps.TrimLeft(); tmps.TrimRight();
	if (tmps.IsEmpty())
		tmps = "가나다ABW";
	size = dc->GetOutputTextExtent(tmps);
	// add 2013. 12. 16.
	if (size.cy > rc.Height())
		size.cy = rc.Height();

	tRc.bottom = rc.top  + size.cy;
	if (rc.Height() > tRc.Height())
	{
		int	dx = max(0, (rc.Height() - tRc.Height())/2 - 1);
		tRc.top += dx;
		tRc.bottom = min(rc.bottom, tRc.top+size.cy+2);
	}
	tRc.right  = rc.left + tRc.Height();
	size = dc->GetOutputTextExtent(text);
	if (!text.IsEmpty())
		size.cx += 3;
	switch (alignC)
	{
	case AL_CENTER:
		index = rc.Width() - (tRc.Width() + size.cx);
		index /= 2;
		break;
	case AL_RIGHT:
		index = rc.Width() - (tRc.Width() + size.cx + 2);
		break;
	case AL_LEFT:
	default:
		index = 3;
		break;
	}
	//tRc.OffsetRect(max(index, 0), 0);
	dc->DrawFrameControl(tRc, DFC_BUTTON, DFCS_BUTTONCHECK);

	if (!imgPath.IsEmpty())
	{
		tRc1.left = tRc.right;
		drawPNG(dc, tRc1, imgPath);
		dc->SetBkMode(TRANSPARENT);
	}
	
	if (rc.Height() < rc.Width())
	{
		tRc2.left = tRc.right + 3;
		tRc2.OffsetRect(max(index, 0), 0);
		dc->DrawText(text, tRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);		
	}
}

void CTlGrid::drawHead(CDC* dc, int col, CString data, CRect rc, UINT format)
{
	CRect	sRc;
	CSize	size, ssize;
	CBrush*	br;
	CString tmps;
	CPoint	pt[6];

	tmps  = sortMARK;
	tmps += _T(" ");
	tmps += data;

	sRc.SetRectEmpty();
	size  = dc->GetOutputTextExtent(tmps);
	ssize = dc->GetOutputTextExtent((LPCTSTR)sortMARK);
	if (rc.Width() > size.cx)
	{
		int	gap;

		sRc   = rc;
		if (format & DT_LEFT)
			sRc.right = sRc.left + ssize.cx;
		else if (format & DT_RIGHT)
		{
			sRc.left  = rc.right - size.cx;
			sRc.right = sRc.left + ssize.cx;
		}
		else
		{
			gap = rc.Width() - size.cx;
			gap /= 2;
			sRc.left += gap;
			sRc.right = sRc.left + ssize.cx;
		}
		rc.left = sRc.right;

		sRc.top = (sRc.top + sRc.Height()/2) - (int)(m_fheight/2);
		sRc.bottom = (int)(sRc.top + m_fheight)-2;

		int	ww = sRc.Width();
		if (ww >= 4)
			ww = (ww/2*2)-1;
		sRc.right = sRc.left + ww;

		br = setBrush(dc, clBASIC);
		dc->SetPolyFillMode(ALTERNATE);

		dc->FillRect(&sRc, br);
		pt[0].x = sRc.right;	pt[0].y = sRc.top;
		pt[1].x = sRc.left;	pt[1].y = sRc.top;
		pt[2].x = sRc.left;	pt[2].y = sRc.bottom;
		pt[3].x = pt[2].x;
		pt[3].y = sRc.bottom;
		pt[4].x = pt[3].x;	pt[4].y = sRc.top;
		pt[5].x = sRc.right;	pt[5].y = pt[4].y;
		setPen(dc, clSORTD);
		setBrush(dc, clSORTD);
		dc->Polygon(pt, 6);

		pt[1].x = sRc.right;
		pt[2].x = pt[1].x;	pt[2].y = sRc.bottom;
		pt[3].x = sRc.left;	pt[3].y = pt[2].y;
		pt[4].x = sRc.left;	pt[4].y = sRc.bottom;
		pt[5].x = sRc.right;	pt[5].y = sRc.bottom;
		setPen(dc, clSORTL);
		setBrush(dc, clSORTL);
		dc->Polygon(pt, 6);

		tmps  = _T(" ");
		tmps += data;
		dc->DrawText(tmps, rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	}
	else
		dc->DrawText(data, rc, format);
}

void CTlGrid::drawPage(CDC* dc, CRect rc, int type)
{
	CPoint	pts[3];
	CBrush*	br;
	CRect	rect, tRc;
	int	gap = (rc.Width() > rc.Height()) ? rc.Height() : rc.Width();
	
	rect = tRc = rc;
	gap /= 5;
	tRc.left   += ((tRc.Width() - (gap*2))/2);
	tRc.right   = tRc.left + gap*2;
	tRc.top    += ((tRc.Height() - (gap*2))/2);
	tRc.bottom  = tRc.top + gap*2;

	br = setBrush(dc, clBASIC);
	dc->FillRect(&rect, br);
	switch (type)
	{
	case 0:		// home : ⊙
		dc->SelectStockObject(BLACK_PEN);
		dc->SelectStockObject(LTGRAY_BRUSH);
		dc->Ellipse(&tRc);
		break;

	case 1:		// page up
		dc->SelectStockObject(BLACK_PEN);
		dc->SelectStockObject(BLACK_BRUSH);

		rc  = tRc;
		tRc.bottom = tRc.top + rc.Height()/2-1;
		tRc.right = tRc.left + (tRc.Width()/2)*2-1;
		pts[0].y = tRc.top;
		pts[1].y = tRc.bottom;
		pts[2].y = tRc.bottom;
		pts[0].x = tRc.left + (tRc.Width()/2);
		pts[1].x = tRc.left;
		pts[2].x = tRc.right;
		dc->Polygon(pts, 3);

		tRc.top = tRc.bottom+1;
		tRc.bottom = tRc.top + rc.Height()/2;
		pts[0].y = tRc.top;
		pts[1].y = tRc.bottom;
		pts[2].y = tRc.bottom;
		pts[0].x = tRc.left + (tRc.Width()/2);
		pts[1].x = tRc.left;
		pts[2].x = tRc.right;
		dc->Polygon(pts, 3);
		break;

	case 2:		// page down
		dc->SelectStockObject(BLACK_PEN);
		dc->SelectStockObject(BLACK_BRUSH);

		rc = tRc;
		tRc.bottom = tRc.top + rc.Height()/2-1;
		tRc.right = tRc.left + (tRc.Width()/2)*2-1;
		pts[0].y = tRc.bottom;
		pts[1].y = tRc.top;
		pts[2].y = tRc.top;
		pts[0].x = tRc.left + (tRc.Width()/2);
		pts[1].x = tRc.right;
		pts[2].x = tRc.left;
		dc->Polygon(pts, 3);

		tRc.top = tRc.bottom+1;
		tRc.bottom = tRc.top + rc.Height()/2-1;
		pts[0].y = tRc.bottom;
		pts[1].y = tRc.top;
		pts[2].y = tRc.top;
		pts[0].x = tRc.left + (tRc.Width()/2);
		pts[1].x = tRc.right;
		pts[2].x = tRc.left;
		dc->Polygon(pts, 3);
		break;

	default:
		break;
	}
	dc->DrawEdge(rect, EDGE_RAISED, BF_RECT);
}

int CTlGrid::getColumnWidth(struct _repR* repR, int index)
{
	CString	datas = _T("");
	CSize	dataSz, headSz;
	CDC*	dc  = m_view->GetDC();
	int	nDC = dc->SaveDC();
	CCellItem *cellItem = new CCellItem(repR);
	struct _cellR *cellR = cellItem->GetCell(index);
	int	offs,  len = (int)(cellR->size - cellR->vals);

	setFont(dc);
	//if (cellR->isEdit == IE_EDIT)
	if (!CString(cellR->editS).IsEmpty())
		datas = CString(cellR->editS);
	else
	{
		if (cellR->attr & FA_PASSWD)
			datas = CString(_T('*'), cellR->size);
		else if (cellR->type == IO_NUM && cellR->attr & FA_COMMA)
		{
			editComma(datas, len);
			if (len != cellR->size)
			{
				CString	tmps = _T("");
				datas += _T('.');
				setInOutFace(tmps, cellR->vals, IO_NUM);
				datas += tmps;
			}
		}
		else
		{
			setInOutFace(datas, len, cellR->type);
			if (len != cellR->size)
			{
				CString	tmps = _T("");
				datas += _T('.');
				setInOutFace(tmps, cellR->vals, IO_NUM);
				datas += tmps;
			}
		}
	}
	dataSz = dc->GetOutputTextExtent(datas);
	headSz = dc->GetOutputTextExtent((LPCTSTR)cellR->head);

	if (!dataSz.cx && !headSz.cx)
	{
		delete cellItem;
		return 10;
	}

	offs = (dataSz.cx > headSz.cx) ? dataSz.cx : headSz.cx;
	offs += 10;

	dc->RestoreDC(nDC);
	m_view->ReleaseDC(dc);

	delete cellItem;
	return offs;
}
