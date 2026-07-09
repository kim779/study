// tlTable.cpp: implementation of the CtlTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlTable.h"
#include "picture.h"

#include "../childview.h"
#include "../mapform.h"
#include "../resource.h"
#include "../awCommon/CellItem.h"
#include "../mainvar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	sortMARK	"□"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtlTable::CtlTable(CChildView* view, UINT id)
	: CtlRect(view)
{
	m_pic = (Cpicture *) 0;
	switch (id)
	{
	case ID_TOOL_GRID:
		m_type = tbGRID;
		m_name = _T("Grid");
		break;
	case ID_TOOL_TABLE:
		m_type = tbTABLE;
		m_name = _T("Table");
		break;
	default:
		m_type = tbNONE;
		break;
	}
	m_pArrowWnd = NULL;
}

CtlTable::~CtlTable()
{
	if (m_pic)
		delete m_pic;
}

void CtlTable::drawBegin(CPoint point)
{
	if (!m_view->m_move && !m_view->m_bSelect)
	{
		CtlRect::drawBegin(point);
		return;
	}
	
	m_fN = m_view->m_selF;
	m_column = Movable(point, m_rect);
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

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	m_selected = Selectable(point, m_rect, m_toPt);
	m_view->InvalidateRect(getRect());
}

void CtlTable::drawTo(CPoint point)
{
	if (!m_view->m_move && !m_view->m_bSelect)
	{
		CtlRect::drawTo(point);
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
		struct	_repR*	repR = (struct _repR *)m_pFormItem->getFormPoint(m_view->m_selF)->auxR;
		CCellItem cellItem(repR);

		switch (m_type)
		{
		case tbGRID:
			if (nDelta >= 0)	// right
				nColWidth = cellItem.getCellPoint(m_column)->width + nDelta;
			else			// left
			{
				if (abs(nDelta) >= cellItem.getCellPoint(m_column)->width)
					nColWidth = 5;
				else
					nColWidth = cellItem.getCellPoint(m_column)->width + nDelta;
			}
			break;
			
		case tbTABLE:
			if (nDelta >= 0)	// right
				nColWidth = cellItem.getCellPoint(0)->width + nDelta;
			else			// left
			{
				if (abs(nDelta) >= cellItem.getCellPoint(0)->width)
					nColWidth = 5;
				else
					nColWidth = cellItem.getCellPoint(0)->width + nDelta;
			}
			break;
		}
		CString sTip;
		sTip.Format("%d", nColWidth);
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 1), (LPARAM)sTip.operator LPCTSTR());
		//TRACE("Width = %d\n", nColWidth);
	}
	else
	{
		Selectable(point, m_rect, m_toPt, false);
		if (m_pArrowWnd)
		{
			if (m_type == tbGRID)
				m_rect.InflateRect(0, 0, 100, 0);
			else
				m_rect.InflateRect(0, 0, 0, 100);

			if (m_rect.PtInRect(point))
			{
				if (m_type == tbGRID)
					m_pArrowWnd->SetWindowPos(NULL, m_toPt.x - 5, m_toPt.y - 6, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
				else
					m_pArrowWnd->SetWindowPos(NULL, m_toPt.x - 6, m_toPt.y - 5, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
				SetCursor(AfxGetApp()->LoadCursor(IDR_TREEFILECTRL_DROPCOPY));
			}
			else
			{
				m_pArrowWnd->ShowWindow(SW_HIDE);
				SetCursor(AfxGetApp()->LoadCursor(IDR_TREEFILECTRL_NO_DROPCOPY));
			}
		}
	}
}

void CtlTable::drawEnd()
{
	if (m_pArrowWnd)
	{
		m_pArrowWnd->DestroyWindow();
		delete m_pArrowWnd;
		m_pArrowWnd = NULL;
	}

	if (!m_view->m_move && !m_view->m_bSelect)
	{
		CtlRect::drawEnd();

		if (m_rect.Width() < 10 || m_rect.Height() < 10)
		{
			CString sCtrlName = "GRID";
			if (m_type == tbTABLE)
				sCtrlName = "TABLE";
			m_rect.right  = m_rect.left + getDefaultWidth(sCtrlName, 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight(sCtrlName, 120);
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
		case tbGRID:
			{
				m_pFormItem->getFormPoint(m_fN)->kind = FM_GRID;
				m_pFormItem->getFormPoint(m_fN)->auxR = (char *)0;
			}
			break;
		
		case tbTABLE:
			m_pFormItem->getFormPoint(m_fN)->kind = FM_TABLE;
			m_pFormItem->getFormPoint(m_fN)->iok  = EIO_OUTPUT;
			m_pFormItem->getFormPoint(m_fN)->bRGB = getDefaultBRGB("TABLE", 232);
			m_pFormItem->getFormPoint(m_fN)->keys = 20;
			m_pFormItem->getFormPoint(m_fN)->attr2 |= GO_AUTOSIZE;
			m_pFormItem->getFormPoint(m_fN)->auxR = (char *)0;
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
		struct	_repR*	repR = (struct _repR *)m_pFormItem->getFormPoint(m_fN)->auxR;
		CCellItem cellItem(repR);
		
		switch (m_type)
		{
		case tbGRID:
			if (m_delta >= 0)	// right
				cellItem.getCellPoint(m_column)->width += m_delta;
			else			// left
			{
				if (abs(m_delta) >= cellItem.getCellPoint(m_column)->width)
					cellItem.getCellPoint(m_column)->width = 5;
				else
					cellItem.getCellPoint(m_column)->width += m_delta;
			}
			break;
			
		case tbTABLE:
			if (m_delta >= 0)	// right
				cellItem.getCellPoint(0)->width += m_delta;
			else			// left
			{
				if (abs(m_delta) >= cellItem.getCellPoint(0)->width)
					cellItem.getCellPoint(0)->width = 5;
				else
					cellItem.getCellPoint(0)->width += m_delta;
			}
			for (ii = 1; ii < repR->rows; ii++)
				cellItem.getCellPoint(ii)->width = cellItem.getCellPoint(0)->width;
			break;
		}
	}
	else if (m_view->m_bSelect)
	{
		CRect tRc;
		int sel = Selectable(m_endpt, tRc, m_toPt);
		
		if (m_type == tbGRID)
			tRc.InflateRect(0, 0, 100, 0);
		else
			tRc.InflateRect(0, 0, 0, 100);

		if (tRc.PtInRect(m_endpt))
		{
			if (m_selected >= 0 && (m_selected > sel || sel - 1 != m_selected))
			{
				struct	_repR* repR = (struct _repR *)m_pFormItem->getFormPoint(m_fN)->auxR;
				CCellItem cellItem(repR);
				if (sel == -1)
				{
					sel = cellItem.getCellCount();
					if (m_selected == sel - 1)
						sel = m_selected + 1;
				}
				if (m_selected > sel || m_selected != sel - 1)
				{
					cellItem.moveCell(m_selected, sel);
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

void CtlTable::Draw(CDC* dc, int fN)
{
	int	ndc = dc->SaveDC();

	CTOOL::Draw(dc, fN);
	switch (m_type)
	{
	case tbGRID:
		drawGRID(dc);		break;
	case tbTABLE:
		drawTABLE(dc);		break;
	default:
		break;
	}
	dc->RestoreDC(ndc);
}

void CtlTable::drawGRID(CDC* dc)
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
	bool	even = true;

	selRect.SetRectEmpty();
	tRc = fRc = getRect();
	setFont(dc);
	setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB));

	if (tRc.Height() == 0 || tRc.Width() == 0)
		return;

	if (m_pFormItem->getFormPoint(m_fN)->auxR == (char *)0)
		return;

	if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);

	repR = (struct _repR *)m_pFormItem->getFormPoint(m_fN)->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	
	drawRows = repR->rows;
	if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE)
		drawRows = HIWORD(m_pFormItem->getFormPoint(m_fN)->keys);

	if (repR->cols != 0/* && drawRows != 0*/)
	{
		line = 1;
		multi = 0;
		for (int ii = 0; ii < repR->cols; ii++)
		{
			CString tHead = cellItem->getCellPoint(ii)->head;
			if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE))
				line = max(line, tHead.Replace("~", "~") + 1);//(int)cellItem->getCellPoint(ii)->headN);
			if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MULTI)
				multi = max(multi, (int)cellItem->getCellPoint(ii)->lineN);
			else
				cellItem->getCellPoint(ii)->lineN = 0;
		}
		multi++;

		dy = tRc.top;

		if (LOWORD(m_pFormItem->getFormPoint(m_fN)->keys) <= 0)
			m_pFormItem->getFormPoint(m_fN)->keys = MAKELONG(m_fheight + 4, HIWORD(m_pFormItem->getFormPoint(m_fN)->keys));
		cy = LOWORD(m_pFormItem->getFormPoint(m_fN)->keys);

		linecnt = 0;
		colidx  = 0;
		if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHEAD)
			line = 0;

		if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHLINE))
			nSlit = 1;

		m_nMaxrow = 1;
		colcnt = 0;

		for (kk = colidx; kk < repR->cols; kk++)
		{
			CString tmp = cellItem->getCellPoint(kk)->head;
			if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE)
				m_nMaxrow = max(m_nMaxrow, tmp.Replace("|", "|") + 1);
		}
		if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE)
			drawRows += m_nMaxrow - 1;

		for (int ii = 0; ii < (drawRows+line)*multi; ii++)
		{
			dx = 0;			
			
			if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE)
			{
				int nTop = m_pFormItem->getFormPoint(m_fN)->top,
					nBottom = m_pFormItem->getFormPoint(m_fN)->bottom;
				cRect.top = nTop + (nBottom - nTop) * ii / ((drawRows+line)*multi);
				dy = nTop + (nBottom - nTop) * (ii + 1) / ((drawRows+line)*multi) - 1;
				if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE)
				{
					if (!(ii < multi && !(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHEAD)) && m_nMaxrow > 1)
					{
						cRect.top += nHeadDy + /*nSlit **/ m_nMaxrow;
						dy += nHeadDy;
					}
					else
						nHeadDy = (dy - cRect.top + 1) * (m_nMaxrow - 1);
				}
				else if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D))
				{
					dy += (ii >= multi?4:1);	// 3d
					cRect.top += (ii >= multi?4:1);
				}

				dy += nSlit * m_nMaxrow;
			}
			else
			{
				cRect.top = dy;
				dy += cy + nSlit;
			}

			if (dy > tRc.bottom)
			{
				dy = tRc.bottom;
				ii = (drawRows+line)*multi + multi;
			}

			colcnt = 0;
			for (kk = colidx; kk < repR->cols; kk++)
			{
				if (cellItem->getCellPoint(kk)->lineN == linecnt)
					colcnt++;
			}
						
			for (jj = colidx; jj < colidx+colcnt; jj++)
			{
				struct _cellR *cellR = cellItem->getCellPoint(jj);
				if (!(cellR->properties & PR_VISIBLE))
					continue;
				if (jj < m_pFormItem->getFormPoint(m_fN)->scrollPos)
					continue;

				if (cellR->width == 0)
				{
					cellR->width = (WORD)(cellR->size * m_fwidth);
				}
				cRect.left = tRc.left + dx;
				dx = dx + cellR->width;

				if (dx <= tRc.Width())
					cRect.right = tRc.left + dx;
				else
					cRect.right = tRc.right;

				if (((m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE) && ii >= multi &&	// ii < multi is head
					!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE)) ||
					(!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE) && ii >= multi))
					cRect.bottom = dy - nSlit;
				else
					cRect.bottom = dy;

				if (ii == (drawRows+line)*multi - 1) // last row
				{
					
				}

				if (ii < multi && !(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHEAD))	// ishead?
				{
					CRect tmpRc;
					CString sUHead = cellR->head;
					int nMaxRow = 1;

					if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE)
					{
						nMaxRow = m_nMaxrow;//sUHead.Replace("|", "|") + 1;
						for (int kk = 0; kk < nMaxRow - sUHead.Replace("|", "|") - 1; kk++)
							sUHead += "|";
					}

					if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE)
						cRect.bottom = m_pFormItem->getFormPoint(m_fN)->top + (m_pFormItem->getFormPoint(m_fN)->bottom - m_pFormItem->getFormPoint(m_fN)->top) * (ii + 1) / ((drawRows+line)*multi) * line * m_nMaxrow + m_nMaxrow;
					else
						cRect.bottom = cRect.top + cy * line * m_nMaxrow + nSlit * nMaxRow;
					
					if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D) &&
						!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE))
						cRect.bottom += 4;	// 3d
					else if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE))
						cRect.bottom += nMaxRow;
					dy = cRect.bottom;

					for (int nRow = 0; nRow < nMaxRow; nRow++)
					{
						CString tCap = sUHead;
						bool bTop = true;
						if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE)
							tCap = Parser(sUHead, "|");
						tmpRc = cRect;
						dc->IntersectClipRect(tmpRc);

						if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MERGE)
							cRect = getHeadRect(jj, nRow, cRect, tCap);

						if ((m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D) &&cRect.top != tmpRc.top)
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
						//if (repR->selected == jj)
						//	setTextColor(dc, GetSysColor(COLOR_HIGHLIGHTTEXT));
						dc->SetBkMode(TRANSPARENT);
						
						hRect = cRect;
						hRect.bottom = cRect.top;
						if (!tCap.IsEmpty())
						{
							str = tCap;//cellR->head;
							/*ptr = str.Find('|');
							if (ptr != -1)
								str = str.Left(ptr);*/
							
							for (kk = 0; kk < tCap.Replace("~", "~") + 1/*cellR->headN*/; kk++)
							{
								hRect.left += 2;
								hRect.right -= 2;
								hRect.top = hRect.bottom;
								hRect.bottom = cRect.top+ cRect.Height()*(kk+1)/(tCap.Replace("~", "~") + 1);
								
								ptr = str.Find('~');
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
						
						if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D)
						{
							setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB));
							dc->SelectStockObject(NULL_BRUSH);
							if (jj != colidx)
								cRect.left--;
							/*if (ii != 0)
								cRect.top--;*/
							dc->Rectangle(cRect);
							if (ii != 0)
								cRect.top++;
						}
						else
							drawEdge(dc, cRect, EDGE_RAISED);
						
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
					if (repR->selected == jj)
						selRect.UnionRect(selRect, cRect);
					dc->MoveTo(cRect.left, cRect.bottom);
					if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_ROW)
						brush = setBrush(dc, getRGB(even ? m_pFormItem->getFormPoint(m_fN)->spRGB : m_pFormItem->getFormPoint(m_fN)->stRGB));
					else
						brush = setBrush(dc, getRGB(cellR->pRGB));

					cRect.bottom++;
					dc->FillRect(cRect, brush);
					cRect.bottom--;

					setTextColor(dc, getRGB(cellR->tRGB));
					dc->SetBkMode(TRANSPARENT);

					if (cellR->vals > 0)
						cellR->attr |= FA_RIGHT;

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
					if (cellR->attr & FA_RIGHT)
						style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;

					hRect = cRect;
					hRect.left += 3;
					hRect.right -= (int)m_fwidth/2;
					text = _T("");
					if (cellR->size != 0)
					{
						len = (int)(cellR->size - cellR->vals);
						if (cellR->isEdit == IE_EDIT)
							text = CString(cellR->editS);
						else
						{
							if (cellR->attr & FA_PASSWD)
								text = CString('*', cellR->size);
							else if (cellR->type == IO_NUM
								&& cellR->attr & FA_COMMA)
							{
								editComma(text, len);
								if (len != cellR->size)
								{
									CString	tmps = _T("");
									text += '.';
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
									text += '.';
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
						drawCHECK(dc, cRect, cellR->str2, cellR->alignC);
						break;
					case CK_BUTTON:
						brush = setBrush(dc, getRGB(cellR->pRGB));
						dc->FillRect(cRect, brush);
						if (cellR->properties & PR_IMAGE || cellR->properties & PR_IMAGETEXT)
						{
							cRect.DeflateRect(1, 1);
							if (strlen(cellR->str3) > 0)
							{
								if (m_pic)
								{
									delete m_pic;
									m_pic = (Cpicture *) 0;
								}
								CString	path = cellR->str3;
								getPath(path);
								m_pic = new Cpicture(path);
								m_pic->DrawPicture(dc, cRect, cellR->alignI);
							}
							else
								drawBMP(dc, cRect, ((cellR->properties & PR_IMAGETEXT)?"":"IMAGE"));
							cRect.InflateRect(1, 1);
						}
						if (!(cellR->properties & PR_IMAGE))
						{
							if (cellR->properties & PR_IMAGETEXT)
								style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;
							cRect.DeflateRect(1, 0);
							dc->SetBkMode(TRANSPARENT);
							dc->SetTextColor(getRGB(cellR->tRGB));
							dc->DrawText(cellR->txt, cRect, style);
							cRect.InflateRect(1, 0);
						}
						drawEdge(dc, cRect, EDGE_RAISED);
						break;
					default:
						hRect.DeflateRect(1, 0);
						dc->DrawText(text, hRect, style);
						hRect.InflateRect(1, 0);
						break;
					}

					setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB));
					if (dx <= tRc.Width())
					{
						cRect.right--;
						if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHLINE))
						{
							dc->MoveTo(cRect.left, cRect.bottom);
							dc->LineTo(cRect.right, cRect.bottom);
						}
						if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XVLINE))
						{
							dc->MoveTo(cRect.right, cRect.top);
							dc->LineTo(cRect.right, cRect.bottom+1);
						}
					}
					else
					{
						if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHLINE))
							dc->LineTo(cRect.right, cRect.bottom);
						dx = -1;
						jj = colidx+colcnt;
					}
				}
			}	// jj loop
			
			linecnt++;
			colidx += colcnt;

			if (linecnt == multi)
				linecnt = 0;
			if (colidx == repR->cols)
			{
				colidx = 0;
				if (ii >= multi || (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHEAD))
					even = !even;
			}
		}	// ii loop
	}
	else
	{
		setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB));	// outline
		if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D)
		{
			dc->SelectStockObject(NULL_BRUSH);
			dc->Rectangle(fRc);
		}
		else
			drawEdge(dc, fRc, EDGE_SUNKEN);
		delete cellItem;
		return;
	}
	// selected draw
	if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHEAD)
		DrawFocusRect(dc, selRect);

	m_pFormItem->getFormPoint(m_fN)->attr2 &= ~GO_VSCR;
	m_pFormItem->getFormPoint(m_fN)->attr2 &= ~GO_HSCR;
	// draw auto-hscroll, auto-vscroll
	if ((dx != -1) && !(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MULTI))
	{
		dx = 0;
		for (int ii = 0; ii < repR->cols; ii++)
		{
			if (cellItem->getCellPoint(ii)->properties & PR_VISIBLE)
				dx += cellItem->getCellPoint(ii)->width;
		}
		if (((cy*(drawRows+line)*multi) > tRc.Height()) && (dx > (tRc.Width() - szSCROLL)))
			dx = -1;
		else if (dx > tRc.Width())
			dx = -1;
	}

	if (dx == -1 && !(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MULTI))
	{
		m_pFormItem->getFormPoint(m_fN)->attr2 |= GO_HSCR;
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
	if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);

	if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_HSCR)
		tRc.bottom -= szSCROLL;

	if ((!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE) && (cy*(drawRows+line)*multi) > tRc.Height()) ||
		((m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE) && HIWORD(m_pFormItem->getFormPoint(m_fN)->keys) < repR->rows))
	{
		float	bH;
		CRect	pRc1, pRc2, pRc3;

		m_pFormItem->getFormPoint(m_fN)->attr2 |= GO_VSCR;
		//m_pFormItem->getFormPoint(m_fN)->attr2 &= ~GO_PAGE;
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
		if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_PAGE)
		{
			drawPage(dc, pRc1, 1);	// page up
			drawPage(dc, pRc2, 0);	// home
			drawPage(dc, pRc3, 2);	// page down
		}
	}
	else if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_PAGE)
	{
		float	bH;
		CRect	pRc1, pRc2, pRc3;

		if (tRc.Width() > szSCROLL)
			tRc.left  = tRc.right  - szSCROLL;

		pRc1 = pRc2 = pRc3 = tRc;
		bH = (float)(tRc.Height() / 7);
		pRc1.bottom = tRc.top + (int)(bH*3);
		pRc2.top    = pRc1.bottom;
		pRc2.bottom = pRc2.top + (int)bH;
		pRc3.top    = pRc2.bottom;

		drawPage(dc, pRc1, 1);	// page up
		drawPage(dc, pRc2, 0);	// home
		drawPage(dc, pRc3, 2);	// page down
	}

	setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB));	// outline
	if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D)
	{
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(fRc);
	}
	else
		drawEdge(dc, fRc, EDGE_SUNKEN);
	delete cellItem;
}

void CtlTable::drawTABLE(CDC* dc)
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

	tRc = fRc = getRect();
	setFont(dc);
	setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB), m_pFormItem->getFormPoint(m_fN)->keys);
	if (tRc.Height() == 0 || tRc.Width() == 0)
		return;

	if (m_pFormItem->getFormPoint(m_fN)->auxR == (char *) 0)
		return;

	linecnt = 0;
	repR = (struct _repR *)m_pFormItem->getFormPoint(m_fN)->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	for (int ii = 0; ii < repR->rows; ii++)
	{
		if (cellItem->getCellPoint(ii)->properties & PR_VISIBLE)
			linecnt++;
	}

	multi = 0;	// BOTH
	if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MULTI))
		multi = (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHEAD) ? 1 : 2;

	dy = 1;
	slit = m_pFormItem->getFormPoint(m_fN)->keys;
	/*if (slit > 1)
	{
		float	fy = (float)slit / (float)2.0;
		dy = (int) fy;
		if (fy - (float)dy >= 0.5)
			dy++;
	}*/
	
	if (repR->rows != 0)
	{
		if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE) && m_pFormItem->getFormPoint(m_fN)->bottom != m_pFormItem->getFormPoint(m_fN)->top + (slit + 1) * repR->rows)
		{
			m_pFormItem->getFormPoint(m_fN)->bottom = m_pFormItem->getFormPoint(m_fN)->top + (slit + 1) * repR->rows;
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
			m_view->Invalidate();
		}

		sdy = (float)tRc.top + 1;//slit;
		if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE))
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
			struct _cellR *cellR = cellItem->getCellPoint(ii);
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
			cRect.right = tRc.right - /*slit*/1;

			if (cRect.left == cRect.right || multi == 2)
			{
				if (ii != repR->rows - 1 && !(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHLINE))
				{
					setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB), /*slit*/1);
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

			if (cellR->vals > 0)
				cellR->attr |= FA_RIGHT;
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
			if (cellR->attr & FA_RIGHT)
				style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;

			text = _T("");
			if (cellR->size != 0)
			{
				len = (int)(cellR->size - cellR->vals);
				if (cellR->isEdit == IE_EDIT)
					text = CString(cellR->editS);
				else
				{
					if (cellR->attr & FA_PASSWD)
						text = CString('*', cellR->size);
					else if (cellR->type == IO_NUM
						&& cellR->attr & FA_COMMA)
					{
						editComma(text, len);
						if (len != cellR->size)
						{
							CString	tmps = _T("");
							text += '.';
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
							text += '.';
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
			if (ii != repR->rows-1 && !(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHLINE))
			{
				setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB), /*slit*/1);
				dc->MoveTo(tRc.left+dy, (int)sdy-dy);
				dc->LineTo(cRect.right, (int)sdy-dy);
			}
		}

		if (dx != tRc.right && dx != 0 && !(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XVLINE))
		{
			setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB), /*slit*/1);
			dc->MoveTo(dx, tRc.top+/*slit*/1);
			dc->LineTo(dx, tRc.bottom-/*slit*/1);
		}
	}

	setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB), /*slit*/1);
	dc->SelectStockObject(NULL_BRUSH);
	dy = 0;
	/*if (slit > 1)
		dy = slit / 2;*/
	dc->Rectangle(tRc.left+dy, tRc.top+dy, tRc.right-dy, tRc.bottom-dy);
	delete cellItem;
}

void CtlTable::drawCOMBO(CDC* dc, CRect rc, CString text)
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

void CtlTable::drawCHECK(CDC* dc, CRect rc, CString text, BYTE alignC)
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
	tRc.OffsetRect(max(index, 0), 0);
	dc->DrawFrameControl(tRc, DFC_BUTTON, DFCS_BUTTONCHECK);

	if (rc.Height() < rc.Width())
	{
		tRc2.left = tRc.right + 3;
		dc->DrawText(text, tRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
	}
}

int CtlTable::Movable(CPoint point, CRect& rc)
{
	CRect	tRc;
	int	ii, width;
	struct	_repR*	repR;

	tRc.SetRect(m_pFormItem->getFormPoint(m_fN)->left, m_pFormItem->getFormPoint(m_fN)->top, m_pFormItem->getFormPoint(m_fN)->right, m_pFormItem->getFormPoint(m_fN)->bottom);
	if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);

	rc    = tRc;
	width = tRc.left;
	repR  = (struct _repR *)m_pFormItem->getFormPoint(m_fN)->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	switch (m_pFormItem->getFormPoint(m_fN)->kind)
	{
	case FM_GRID:
		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR *cellR = cellItem->getCellPoint(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;
			if (ii < m_pFormItem->getFormPoint(m_fN)->scrollPos)
				continue;

			width += cellR->width;
			if (width >= tRc.right)
			{
				delete cellItem;
				return (-1);
			}

			if (point.x >= width-2 && point.x <= width+2)
			{
				if ((m_pFormItem->getFormPoint(m_fN)->attr2 & GO_HSCR) &&
					point.y >= m_pFormItem->getFormPoint(m_fN)->bottom - szSCROLL)
				{
					delete cellItem;
					return (-1);
				}

				rc.left  = width-1;
				rc.right = width+1;
				delete cellItem;
				return (ii);
			}

			if (cellR->attr & FA_WRAP)
				width = tRc.left;
		}
		break;

	case FM_TABLE:
		if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MULTI) || !repR->rows)
		{
			delete cellItem;
			return (-1);
		}

		rc.left += cellItem->getCellPoint(0)->width - 2;
		rc.right = rc.left + 2;
		tRc = rc;
		tRc.InflateRect(2, 0);
		if (tRc.PtInRect(point))
		{
			delete cellItem;
			return (0);
		}
		break;
	}

	delete cellItem;
	return (-1);
}

int CtlTable::Selectable(CPoint point, CRect& rc, CPoint &pt, bool bSet)
{
	CRect	tRc;
	int	ii, width, line, drawRows, headHeight, multi;
	struct	_repR*	repR;

	tRc.SetRect(m_pFormItem->getFormPoint(m_fN)->left, m_pFormItem->getFormPoint(m_fN)->top, m_pFormItem->getFormPoint(m_fN)->right, m_pFormItem->getFormPoint(m_fN)->bottom);
	if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);

	rc    = tRc;
	width = tRc.left;
	repR  = (struct _repR *)m_pFormItem->getFormPoint(m_fN)->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	switch (m_pFormItem->getFormPoint(m_fN)->kind)
	{
	case FM_GRID:
		line = 0;
		
		multi = 0;
		for (ii = 0; ii < repR->cols; ii++)
		{
			if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MULTI)
				multi = max(multi, (int)cellItem->getCellPoint(ii)->lineN);
		}
		multi++;

		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR *cellR = cellItem->getCellPoint(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;
			if (ii < m_pFormItem->getFormPoint(m_fN)->scrollPos)
					continue;
			if (line != cellR->lineN)
			{
				line++;
				width = tRc.left;
			}

			drawRows = repR->rows;
			headHeight = LOWORD(m_pFormItem->getFormPoint(m_fN)->keys);
			if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_AUTOSIZE)
			{
				drawRows = HIWORD(m_pFormItem->getFormPoint(m_fN)->keys);
				headHeight = (m_pFormItem->getFormPoint(m_fN)->bottom - m_pFormItem->getFormPoint(m_fN)->top) / (drawRows * multi);
			}
			
			int height = tRc.top + line * headHeight;
			rc.SetRect(width, height,
				width + cellR->width,
				headHeight + height);

			if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_XHEAD)
				rc.bottom = headHeight * drawRows + height;
			if (rc.bottom > m_pFormItem->getFormPoint(m_fN)->bottom)
				rc.bottom = m_pFormItem->getFormPoint(m_fN)->bottom;
			if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_HSCR)
				if (rc.bottom + szSCROLL > m_pFormItem->getFormPoint(m_fN)->bottom)
					rc.bottom -= szSCROLL;

			if (rc.PtInRect(point))
			{
				pt = CPoint(rc.left, rc.top);
				delete cellItem;
				if (bSet)
					repR->selected = ii;
				return (ii);
			}
						
			width += cellR->width;
			if (width >= tRc.right)
				width  = tRc.right;
		}
		pt = CPoint(rc.right, rc.top);
		if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_HSCR)
		{
			CRect tRc1, tRc2;
			if  (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_VSCR)
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
				m_pFormItem->getFormPoint(m_fN)->scrollPos--;
				if (m_pFormItem->getFormPoint(m_fN)->scrollPos < 0)
					m_pFormItem->getFormPoint(m_fN)->scrollPos = 0;
				delete cellItem;
				return -1;
			}
			else if (tRc2.PtInRect(point) && !bSet)
			{
				m_pFormItem->getFormPoint(m_fN)->scrollPos++;
				for (ii = m_pFormItem->getFormPoint(m_fN)->scrollPos; ii < repR->cols; ii++)
				{
					struct _cellR *cellR = cellItem->getCellPoint(ii);
					if (!(cellR->properties & PR_VISIBLE))
						continue;
					tRc.right -= cellR->width;
				}
				if (tRc.Width() > 0)
					m_pFormItem->getFormPoint(m_fN)->scrollPos--;
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
			return (-1);
		}
		struct _cellR	*cellR = cellItem->getCellPoint(0);
		if (m_pFormItem->getFormPoint(m_fN)->attr2 & GO_MULTI)
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
	return (-1);
}

bool CtlTable::OptimumSize(CPoint point)
{
	CRect	tRc, rect;
	int	ii;
	struct	_repR*	repR;

	tRc.SetRect(m_pFormItem->getFormPoint(m_fN)->left, m_pFormItem->getFormPoint(m_fN)->top, m_pFormItem->getFormPoint(m_fN)->right, m_pFormItem->getFormPoint(m_fN)->bottom);
	if (!(m_pFormItem->getFormPoint(m_fN)->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);

	ii = (m_pFormItem->getFormPoint(m_fN)->keys <= 0) ? m_fheight+4 : m_pFormItem->getFormPoint(m_fN)->keys;
	tRc.bottom = tRc.top + ii;
	tRc.right  = tRc.left;
	repR = (struct _repR *)m_pFormItem->getFormPoint(m_fN)->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	switch (m_pFormItem->getFormPoint(m_fN)->kind)
	{
	case FM_GRID:
		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR *cellR = cellItem->getCellPoint(ii);
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

void CtlTable::drawHead(CDC* dc, int col, CString data, CRect rc, UINT format)
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
	ssize = dc->GetOutputTextExtent(sortMARK);
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

void CtlTable::drawPage(CDC* dc, CRect rc, int type)
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

int CtlTable::getColumnWidth(struct _repR* repR, int index)
{
	CString	datas = _T("");
	CSize	dataSz, headSz;
	CDC*	dc  = m_view->GetDC();
	int	nDC = dc->SaveDC();
	CCellItem *cellItem = new CCellItem(repR);
	struct _cellR *cellR = cellItem->getCellPoint(index);
	int	offs,  len = (int)(cellR->size - cellR->vals);

	setFont(dc);
	if (cellR->isEdit == IE_EDIT)
		datas = CString(cellR->editS);
	else
	{
		if (cellR->attr & FA_PASSWD)
			datas = CString('*', cellR->size);
		else if (cellR->type == IO_NUM && cellR->attr & FA_COMMA)
		{
			editComma(datas, len);
			if (len != cellR->size)
			{
				CString	tmps = _T("");
				datas += '.';
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
				datas += '.';
				setInOutFace(tmps, cellR->vals, IO_NUM);
				datas += tmps;
			}
		}
	}
	dataSz = dc->GetOutputTextExtent(datas);
	headSz = dc->GetOutputTextExtent(cellR->head);

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

void CtlTable::MouseDrag()
{
	if (m_view->m_bSelect && !m_pArrowWnd)
	{
		m_pArrowWnd = new CArrowWnd();
		if (m_type == tbGRID)
		{
			m_pArrowWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 8, 5), m_view, 1);
			m_pArrowWnd->SetTP(4);
		}
		else 
		{
			m_pArrowWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 5, 8), m_view, 1);
			m_pArrowWnd->SetTP(3);
		}
	}
}

void CtlTable::DrawFocusRect(CDC *pDC, CRect rc)
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
//
//		1
//	       402
//		3
//
CRect CtlTable::getHeadRect(int col, int row, CRect rc, CString txt, int pos /*= 0*/, CRect unionRc /*= CRect(0, 0, 0, 0)*/)
{
	TRACE("%s, pos = %d, col = %d, row = %d, unionRect = (%d, %d, %d, %d)\n", txt, pos, col, row, unionRc.left, unionRc.top, unionRc.right, unionRc.bottom);
	struct _repR *repR = (struct _repR *)m_pFormItem->getFormPoint(m_fN)->auxR;
	CCellItem cellItem(repR);
	CString sHead = cellItem.getCellPoint(col)->head;
	int nMaxRow = m_nMaxrow;// = sHead.Replace("|", "|") + 1;
	//nMaxRow = m_nMaxrow;
	for (int kk = 0; kk < nMaxRow - sHead.Replace("|", "|") - 1; kk++)
		sHead += "|";
	CRect tRc = rc, cRc = rc;

	if (row < 0 || col < 0 || col >= cellItem.getCellCount() || row >= nMaxRow)
		return CRect(0, 0, 0, 0);

	CString sUnitHead = "";
	tRc.bottom = tRc.top + tRc.Height() / nMaxRow;
	tRc.OffsetRect(0, tRc.Height() * row);

	for (int ii = 0; ii <= row; ii++)
		sUnitHead = Parser(sHead, "|");

	if (txt.IsEmpty())
		return tRc;

	if (sUnitHead.Compare(txt))
		return CRect(0, 0, 0, 0);

	unionRc.UnionRect(unionRc, CRect(tRc.left, tRc.top, tRc.right, tRc.bottom + 1));

	CRect tmpRect;
	if (col >= 1 && pos != 2)
	{
		cRc.left   = cRc.left - cellItem.getCellPoint(col - 1)->width;
		cRc.right  = cRc.left + cellItem.getCellPoint(col - 1)->width;
		cRc.top    = cRc.top + (cRc.Height() / nMaxRow) * row;
		cRc.bottom = cRc.top + (cRc.Height() / nMaxRow) * (row + 1);
		tmpRect.UnionRect(unionRc, cRc);
		if (!tmpRect.EqualRect(unionRc))
			tRc.UnionRect(tRc, getHeadRect(col - 1, row, cRc, sUnitHead, 4, unionRc));
	}

	unionRc.UnionRect(unionRc, tRc);

	if (col + 1 < cellItem.getCellCount() && pos != 4)
	{
		cRc = rc;
		cRc.right  = cRc.right + cellItem.getCellPoint(col + 1)->width;
		cRc.left   = cRc.right - cellItem.getCellPoint(col + 1)->width;
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
