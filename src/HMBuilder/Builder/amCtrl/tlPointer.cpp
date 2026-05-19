// tlPointer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlPointer.h"
#include "tracker.h"
#include "pointwnd.h"

#include "../h/mainvar.h"
#include "../amCom/formItem.h"
#include "../amCom/cellItem.h"

// CTlPointer

CTlPointer::CTlPointer(CChildView* view, CAmDraw* draw, CFormItem* formItem)
	: CTlRect(view, draw, formItem)
{
	m_name  = _T("Pointer");
	m_ptWnd = NULL;
	m_block = view->m_block;
	m_firstCurPos = CPoint(0, 0);
	m_gap  = (int)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETGAP, 0);
	m_bMoveLine = false;
	m_type = none;
}

CTlPointer::~CTlPointer()
{
	if (m_ptWnd)
		delete m_ptWnd;
}

void CTlPointer::drawBegin(CPoint point)
{
	CRect	tRc;
	int	ii, nFormCnt = m_formItem->GetCount();

	if (m_view->m_nTabOdr || m_view->m_nIoOdr) // taborder 
	{
		m_view->m_selF = -1;
		for (ii = 0; ii < nFormCnt; ii++) 
		{
			if ((m_view->m_nTabOdr && !m_formItem->GetForm(ii)->index) ||
				(m_view->m_nIoOdr && !m_formItem->GetForm(ii)->iorder))
				continue;
			tRc = getFormRect(ii);
			
			if (tRc.PtInRect(point))
				m_view->m_selF = ii;
		}
		return;
	}

	m_pRc.SetRectEmpty();
	if (m_view->m_selF >= 0 && m_block->count <= 1)
	{
		tRc = getFormRect(m_view->m_selF);
		tRc.InflateRect(5, 5);
		m_view->RedrawWindow();
	}

	if ((::GetKeyState(VK_SHIFT) & 0x8000) && m_view->m_selF >= 0 && m_block->count == 0)
	{
		m_block->count++;
		m_view->m_stdF = m_view->m_selF;
		m_formItem->GetForm(m_view->m_selF)->block = true;
	}

	if (!(m_view->m_selF >= 0 && isHit(m_view->m_selF, point)))
	{
		m_view->m_selF = -1;
		for (ii = 0; ii < nFormCnt; ii++)
		{
			if (m_formItem->GetForm(ii)->group && m_formItem->GetIdxOfPtr(m_formItem->GetForm(ii)->group) != ii)
				continue;
			
			tRc = getFormRect(ii);
			if (tRc.PtInRect(point) || (!tRc.IsRectEmpty() && isHit(ii, point)))
				m_view->m_selF = ii;
		}
	}

	if (m_view->m_selF > nFormCnt)
		m_view->m_selF = -1;

	int	form = -1;
	for (;;)
	{
		if (m_view->m_selF == form)
			break;

		if (m_formItem->GetForm(m_view->m_selF)->group)
			break;

		struct	_formR*	formR = m_formItem->GetForm(m_view->m_selF);
		if (formR->kind == FM_GROUP || formR->kind == FM_IMAGEVIEW || formR->kind == FM_BOX)
		{
			form = m_view->m_selF;
			CRect	sRc, fRc;
			sRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);

			for (ii = 0; ii < form; ii++)
			{
				if (m_formItem->GetFormStr(ii, _T("name")).IsEmpty())
					continue;

				if (m_formItem->GetForm(ii)->group && 	m_formItem->GetIdxOfPtr(m_formItem->GetForm(ii)->group) != ii)
					continue;

				fRc = getFormRect(ii);
				if (fRc.PtInRect(point))
				{
					tRc.IntersectRect(fRc, sRc);
					if (fRc.EqualRect(tRc))
						m_view->m_selF = ii;
				}
			}
		}
		else
			break;
	}

	if (m_block->count > 1 && !(::GetKeyState(VK_SHIFT) & 0x8000) && m_view->m_selF >= 0 && !m_formItem->GetForm(m_view->m_selF)->block)
	{
		ReleaseBlock();
		m_view->RedrawWindow();
	}

	if (m_view->m_selF >= 0)
	{
		if (::GetKeyState(VK_SHIFT) & 0x8000)
		{
			m_formItem->GetForm(m_view->m_selF)->block = !m_formItem->GetForm(m_view->m_selF)->block;
			if (m_formItem->GetForm(m_view->m_selF)->block)
			{
				m_block->count++;
				if (m_block->count == 1 && m_formItem->GetForm(m_view->m_selF)->block)
					m_view->m_stdF = m_view->m_selF;
			}
			else
			{
				int selF = -1;
				m_block->count = 0;
				for (int ii = 0; ii < nFormCnt; ii++)
				{
					if (m_formItem->GetForm(ii)->block)
					{
						m_block->count++;
						selF = ii;
					}
				}
				m_view->m_stdF = selF;
				m_view->m_selF = selF;
				m_view->Invalidate();
			}
		}
		else
		{
			if (!m_formItem->GetForm(m_view->m_selF)->block)
				ReleaseBlock();
			else
				m_view->m_stdF = m_view->m_selF;
		}
	}

	m_dc->SelectStockObject(NULL_BRUSH);
	m_dc->SetROP2(R2_XORPEN);

	if (::GetKeyState(VK_CONTROL) & 0x8000)
		m_view->m_selF = -1;

	if (m_view->m_bLine)
	{
		ReleaseBlock();
		m_block->bRc.left = m_block->bRc.right = point.x;
		m_block->bRc.top = m_block->bRc.bottom = point.y;
		m_view->RedrawWindow();
	}
	else if (m_view->m_selF == -1)
	{
		ReleaseBlock();
		m_block->bRc.left = m_block->bRc.right  = point.x;
		m_block->bRc.top  = m_block->bRc.bottom = point.y;
		m_dc->Rectangle(m_block->bRc);
		m_view->RedrawWindow();
	}
	else
	{
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
		if (!m_formItem->GetForm(m_view->m_selF)->block)
			formBegin(point);
		else
			blockBegin(point);

		switch (m_where)
		{
		case CTracker::hitTopLeft:
		case CTracker::hitBottomRight:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
			break;
		case CTracker::hitTopRight:
		case CTracker::hitBottomLeft:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
			break;
		case CTracker::hitTop:
		case CTracker::hitBottom:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			break;
		case CTracker::hitRight:
		case CTracker::hitLeft:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			break;
		case CTracker::hitMiddle:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			break;
		case CTracker::hitNothing:
		default:
			break;
		}
	}
}

void CTlPointer::drawTo(CPoint point)
{
	CPen pen, *open;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(255, 255, 255);
	pen.CreatePen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, 1, &logBrush);
	open = m_dc->SelectObject(&pen);


	if (::GetKeyState(VK_SHIFT) & 0x8000)
		return;

	if (m_view->m_nTabOdr || m_view->m_nIoOdr)
		return;

	if (!m_view->m_bFirstDrawing)
		m_view->m_bRedraw = false;
	
	if (m_view->m_bLine)
	{
		CRect rc = m_block->bRc;

		if (rc.Width() > rc.Height())
		{
			m_dc->MoveTo(rc.TopLeft());
			m_dc->LineTo(CPoint(rc.right, rc.top));

			m_dc->MoveTo(rc.TopLeft());
			m_dc->LineTo(CPoint(point.x, rc.top));
		}
		else
		{
			m_dc->MoveTo(rc.TopLeft());
			m_dc->LineTo(CPoint(rc.left, rc.bottom));

			m_dc->MoveTo(rc.TopLeft());
			m_dc->LineTo(CPoint(rc.left, point.y));

		}
		m_block->bRc.right = point.x;
		m_block->bRc.bottom = point.y;

	}
	else if (m_view->m_selF == -1)
	{
		m_dc->Rectangle(m_block->bRc);
		m_block->bRc.right  = point.x;
		m_block->bRc.bottom = point.y;
		m_dc->Rectangle(m_block->bRc);
	}
	else
	{
		DrawPositionLine();
		if (!m_formItem->GetForm(m_view->m_selF)->block)
			formTo(point);
		else
			blockTo(point);
	}

	m_dc->SelectObject(open);
	pen.DeleteObject();
}

void CTlPointer::DrawPositionLine()
{
	CRect tRc, rc, irc;
	rc.CopyRect(m_rect);

	CPen pen, *open;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(20, 190, 200);
	pen.CreatePen(PS_DOT /*| PS_GEOMETRIC /*| PS_ENDCAP_ROUND*/, 1, &logBrush);
	open = m_dc->SelectObject(&pen);


	if (m_bMoveLine)
	{
		switch (m_type)
		{
		case CTlPointer::left:
			rc.bottom = rc.top + 10;
			rc.right = rc.left + 2;
			break;
		case CTlPointer::top:
			rc.right = rc.left + 10;
			rc.bottom = rc.top + 2;
			break;
		case CTlPointer::right:
			rc.bottom = rc.top + 10;
			rc.left = rc.right - 2;
			break;
		case CTlPointer::bottom:
			rc.top = rc.top + 10;
			rc.top = rc.bottom - 2;
			break;
		default:
			break;
		}

		irc.IntersectRect(m_RcLine, rc);
		if (irc.IsRectEmpty())
		{
			m_view->Invalidate();
			m_RcLine.SetRect(0, 0, 0, 0);
			m_bMoveLine = false;
		}	
		else
		{
			switch (m_type)
			{
			case CTlPointer::left:
				m_rect.left = m_RcLine.left;
				break;
			case CTlPointer::top:
				m_rect.top = m_RcLine.top;
				break;
			case CTlPointer::right:
				m_rect.right = m_RcLine.right;
				break;
			case CTlPointer::bottom:
				m_rect.bottom = m_RcLine.bottom;
				break;
			default:
				break;
			}
		}
	}

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		tRc = getRect(ii);
		if (m_view->m_selF == ii)
			continue;
				
		int minval, maxval;

		if (rc.left == tRc.left || rc.left == tRc.right)
		{
			minval = min(rc.top, tRc.top);
			maxval = max(rc.bottom, tRc.bottom);
			m_dc->MoveTo(rc.left, minval);
			m_dc->LineTo(rc.left, maxval);


			m_RcLine.SetRect(rc.left, minval, rc.left, maxval);
			m_RcLine.InflateRect(1, 0);
			m_bMoveLine = true;
			m_type = left;
	
		}
		else if (rc.right == tRc.left || rc.right == tRc.right)
		{
			minval = min(rc.top, tRc.top);
			maxval = max(rc.bottom, tRc.bottom);
			m_dc->MoveTo(rc.right, minval);
			m_dc->LineTo(rc.right, maxval);


			m_RcLine.SetRect(rc.right, minval, rc.right, maxval);
			m_RcLine.InflateRect(1, 0);
			m_bMoveLine = true;
			m_type = right;

		
		}
		else if (rc.top == tRc.top || rc.top == tRc.bottom)
		{
			minval = min(rc.left, tRc.left);
			maxval = max(rc.right, tRc.right);
			m_dc->MoveTo(minval, rc.top);
			m_dc->LineTo(maxval, rc.top);


			m_RcLine.SetRect(minval, rc.top, maxval, rc.top);
			m_RcLine.InflateRect(0, 1);
			m_bMoveLine = true;
			m_type = top;

			
		}
		else if (rc.bottom == tRc.top || rc.bottom == tRc.bottom)
		{
			minval = min(rc.left, tRc.left);
			maxval = max(rc.right, tRc.right);
			m_dc->MoveTo(minval, rc.bottom);
			m_dc->LineTo(maxval, rc.bottom);

			m_RcLine.SetRect(minval, rc.bottom, maxval, rc.bottom);
			m_RcLine.InflateRect(0, 1);
			m_bMoveLine = true;
			m_type = bottom;
			
		}
	}
	m_dc->SelectObject(open);
	pen.DeleteObject();
}

void CTlPointer::drawEnd()
{
	m_view->Invalidate();

	m_view->m_bRedraw = true;
	if (m_view->m_nTabOdr || m_view->m_nIoOdr)
	{
		if (m_view->m_selF == -1)
		{
			if (m_view->m_nTabOdr)
				m_view->m_nTabOdr = 0;
			else
				m_view->m_nIoOdr = 0;
		}
		m_view->Invalidate();
		return;
	}

	int	count;
	if (::GetKeyState(VK_SHIFT) & 0x8000)
	{
		count = m_formItem->GetCount();
		m_pRc.SetRectEmpty();
		for (int ii = 0; ii < count; ii++)
			if (m_formItem->GetForm(ii)->block)
				m_pRc.UnionRect(m_pRc, getFormRect(ii));

		m_pRc.NormalizeRect();
		if (!m_pRc.IsRectEmpty())
		{
			m_pRc.InflateRect(5, 5);
			m_view->InvalidateRect(&m_pRc);
		}
		return;
	}

	CRect	tRc;
	CString	sLayoutName;
	struct	_formR*	formR;
	struct	_Layout* layout;


	if (m_view->m_bLine)
	{
		CArray<CRect, CRect> *pLine = (CArray<CRect, CRect> *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STANDARDLINE);
		if (pLine && ((m_block->bRc.Width() > 150) || (m_block->bRc.Height() > 150)))
		{
			CRect rcline = m_block->bRc;
			if (rcline.Width() > rcline.Height())
			{
				rcline.bottom = rcline.top + 1;
				rcline.left = 0;
				rcline.right = 3000;
			}
			else
			{
				rcline.right = rcline.left + 1;
				rcline.top = 0;
				rcline.bottom = 3000;
			}
			pLine->Add(rcline);
			m_view->Invalidate();
		}
	}
	else if (m_view->m_selF == -1)
	{
		CRgn	pRgn;

		m_block->bRc.NormalizeRect();
		m_dc->Rectangle(m_block	->bRc);
		m_pRc = m_block->bRc;
		
		pRgn.CreateRectRgnIndirect(&m_block->bRc);
		m_block->count = 0;

		count = m_formItem->GetCount();
		for (int ii = 0; ii < count; ii++)
		{
			if (m_formItem->GetFormStr(ii, _T("name")).IsEmpty())
				continue;
			
			formR = m_formItem->GetForm(ii);
			if (formR->group && m_formItem->GetIdxOfPtr(formR->group) != ii)
				continue;
			
			tRc = getFormRect(ii);
			if (pRgn.PtInRegion(CPoint(tRc.left, tRc.top)) && pRgn.PtInRegion(CPoint(tRc.right, tRc.bottom)))
			{
				m_pRc.left   = __min(m_pRc.left, tRc.left);
				m_pRc.top    = __min(m_pRc.top, tRc.top);
				m_pRc.right  = __max(m_pRc.right, tRc.right);
				m_pRc.bottom = __max(m_pRc.bottom, tRc.bottom);
				formR->block = true;
				m_block->count++;
				
				if (m_block->count == 1)
					m_view->m_stdF = m_view->m_selF = ii;
			}
		}
		
		if (m_block->count == 1)
			m_formItem->GetForm(m_view->m_selF)->block = false;
		
		pRgn.DeleteObject();
	}
	else
	{
		CPoint cursorPos;

		GetCursorPos(&cursorPos);
		if (m_firstCurPos != cursorPos)
		{
			formR = m_formItem->GetForm(m_view->m_selF);

			sLayoutName = m_formItem->m_mapH->LayoutName;
			if (!sLayoutName.IsEmpty() && sLayoutName.CompareNoCase("Original"))
				formR->layout->Lookup(sLayoutName, (void*&)layout);

			if (!formR->block)
			{
				m_rect.NormalizeRect();				
				if (formR->group)
				{
					CRect tRc = GroupRect((int)formR->group);
					setBlockGroup(m_view->m_selF, m_rect.left - tRc.left, m_rect.top - tRc.top);
				}
				else
				{
					formR->left   = m_rect.left;
					formR->top    = m_rect.top;
					formR->right  = m_rect.right;
					formR->bottom = m_rect.bottom;
					// layout
					if (!sLayoutName.IsEmpty() && sLayoutName.CompareNoCase("Original"))
					{
						layout->left  = m_rect.left;
						layout->top    = m_rect.top;
						layout->right  = m_rect.right;
						layout->bottom = m_rect.bottom;
					}
				}
			}
			else
			{
				count = m_formItem->GetCount();
				for (int ii = 0; ii < count; ii++)
				{
					formR = m_formItem->GetForm(ii);
					if (formR->block)
					{
						if (formR->group)
							setBlockGroup(ii, m_x, m_y);
						else
						{
							formR->left   += m_x;
							formR->top    += m_y;
							formR->right  += m_x;
							formR->bottom += m_y;
						}
						
						tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
						if (formR->group)
							tRc = GroupRect((int)formR->group);
					}
				}
				tRc = m_pRc;
				m_pRc.OffsetRect(m_x, m_y);
				m_pRc.UnionRect(tRc, m_pRc);
			}
		}
		else
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, FALSE);
	}
	
	if (m_ptWnd)
	{
		m_ptWnd->DestroyWindow();
		delete m_ptWnd;
		m_ptWnd = NULL;
	}
	m_pRc.NormalizeRect();
	if (!m_pRc.IsRectEmpty())
	{
		m_pRc.InflateRect(5, 5);
		m_view->InvalidateRect(&m_pRc);
	}
}

void CTlPointer::formBegin(CPoint point)
{
	int	selF = m_view->m_selF;

	m_rect = getFormRect(selF);
	m_pRc = m_rect;

	CTracker tracker(m_rect, CTracker::resizeOutside);
	int	hit = tracker.HitTest(point);
	m_where = CTracker::TrackerHit(hit);

  	switch (m_where) 
	{
	case CTracker::hitTopLeft:
      		m_rect.left   = point.x;
      		m_rect.top    = point.y;
      		break;
	case CTracker::hitTop:
      		m_rect.top    = point.y;
      		break;
	case CTracker::hitTopRight:
      		m_rect.top    = point.y;
      		m_rect.right  = point.x;
      		break;
	case CTracker::hitLeft:
      		m_rect.left   = point.x;
      		break;
	case CTracker::hitRight:
      		m_rect.right  = point.x;
      		break;
	case CTracker::hitBottomLeft:
      		m_rect.left   = point.x;
      		m_rect.bottom = point.y; 	 
      		break;
	case CTracker::hitBottom:
      		m_rect.bottom = point.y; 	 
      		break;
	case CTracker::hitBottomRight:
      		m_rect.right  = point.x;
      		m_rect.bottom = point.y; 	 
      		break;
  	default:
      		break;
  	}

	if (!m_ptWnd)
	{
		m_ptWnd = new CPointWnd();
		m_ptWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_rect, m_view, 1);
		m_ptWnd->AddRect(m_rect);
		m_ptWnd->SetTP();		
		m_ptWnd->SetRGB(m_dc->GetPixel(CPoint(1, 1)));
	}
	GetCursorPos(&m_firstCurPos);
}

void CTlPointer::blockBegin(CPoint point)
{
	CRect	tRc;
	m_x = m_y = 0;

	m_view->GetClientRect(&tRc);
	m_pRc.SetRect(0, 0, 0, 0);

	tRc = getFormRect(m_view->m_selF);

	CTracker tracker(tRc, CTracker::resizeOutside);
	int	hit = tracker.HitTest(point);
	m_where = CTracker::TrackerHit(hit);
	if (!m_ptWnd && !(::GetKeyState(VK_SHIFT) & 0x8000))
	{
		m_ptWnd = new CPointWnd();
		m_ptWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, tRc, m_view, 1);
		m_ptWnd->ModifyStyleEx(NULL, WS_EX_TOPMOST);
	}
	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		if (m_formItem->GetForm(ii)->block)
		{
			CRect rc = getFormRect(ii);
			
			m_pRc.UnionRect(m_pRc, rc);
			if (!(::GetKeyState(VK_SHIFT) & 0x8000))
				m_ptWnd->AddRect(rc);
		}
	}
	if (m_ptWnd)
		m_ptWnd->SetTP();

	m_pRc.InflateRect(5, 5);
}

void CTlPointer::formTo(CPoint point)
{
	if (!(::GetKeyState(VK_CONTROL) & 0x8000) && m_where != CTracker::hitMiddle && m_lastpt != point)
	{
		point.x = ((int)(point.x / m_gap)) * m_gap;
		point.y = ((int)(point.y / m_gap)) * m_gap;
	}

	m_ptWnd->SetWindowPos(NULL, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), SWP_NOZORDER);

	if (m_where != CTracker::hitMiddle && m_formItem->GetForm(m_view->m_selF)->group)
		return;
  	switch (m_where) 
	{
	case CTracker::hitTopLeft:
      		m_rect.left = point.x;
      		m_rect.top  = point.y;
      		break;
	case CTracker::hitTop:
      		m_rect.top = point.y;
      		break;
	case CTracker::hitTopRight:
      		m_rect.top   = point.y;
      		m_rect.right = point.x;
      		break;
	case CTracker::hitLeft:
      		m_rect.left = point.x;
      		break;
	case CTracker::hitMiddle:
      		m_x = point.x - m_lastpt.x;
      		m_y = point.y - m_lastpt.y;
		m_rect = getFormRect(m_view->m_selF);
		m_rect.OffsetRect(m_x, m_y);
		if (!(::GetKeyState(VK_CONTROL) & 0x8000) && m_lastpt != point)
		{
			m_rect.OffsetRect(((int)(m_rect.left / m_gap)) * m_gap - m_rect.left,
				((int)(m_rect.top / m_gap)) * m_gap - m_rect.top);
		}
      		break;
	case CTracker::hitRight:
      		m_rect.right = point.x;
      		break;
	case CTracker::hitBottomLeft:
      		m_rect.left   = point.x;
      		m_rect.bottom = point.y;
      		break;
	case CTracker::hitBottom:
      		m_rect.bottom = point.y;
      		break;
	case CTracker::hitBottomRight:
      		m_rect.right  = point.x;
      		m_rect.bottom = point.y;
      		break;
  	default: 
		break;
  	}

	if (m_where == CTracker::hitMiddle)
	{
		CString sTip;
		sTip.Format(_T("(%d, %d, %d, %d)"), m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 1), (LPARAM)sTip.operator LPCTSTR());
	}
	else
	{
		CString sTip;
		if (!(::GetKeyState(VK_CONTROL) & 0x8000))
		{
			point.x = ((int)(point.x / m_gap)) * m_gap;
			point.y = ((int)(point.y / m_gap)) * m_gap;
		}

		sTip.Format(_T("(%d, %d)"), point.x, point.y);

		if ((point.x < m_rect.left || point.y < m_rect.top) || (point.x > m_rect.right || point.y > m_rect.bottom))
		{
			AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 0), (LPARAM)sTip.operator LPCTSTR());
		}
		else
			AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 1), (LPARAM)sTip.operator LPCTSTR());
	}

	m_pRc.UnionRect(m_pRc, m_rect);
}

void CTlPointer::blockTo(CPoint point)
{
	m_x += (point.x - m_lastpt.x);
	m_y += (point.y - m_lastpt.y);
	m_ptWnd->SetWindowPos(NULL, m_pRc.left + m_x + 5, m_pRc.top + m_y + 5, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	m_lastpt = point;
}

bool CTlPointer::isHit(int idx, CPoint point)
{
	CRect	tRc;
	struct	_formR*	formR = m_formItem->GetForm(idx);

	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	if (formR->group)
		return false;

	CTracker	tracker(tRc, CTracker::resizeOutside);

	int	hit = tracker.HitTest(point);
	if (CTracker::TrackerHit(hit) == CTracker::hitNothing || CTracker::TrackerHit(hit) == CTracker::hitMiddle)
		return false;
	return true;
}

void CTlPointer::setBlockGroup(int nGroup, int mx, int my)
{
	struct	_formR*	formR;
	struct	_formR*	groupR = m_formItem->GetForm(nGroup)->group;

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		formR = m_formItem->GetForm(ii);
		if (formR->group == groupR)
		{
			formR->left   += mx;
			formR->top    += my;
			formR->right  += mx;
			formR->bottom += my;
		}
	}
}
