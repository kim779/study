// tlPointer.cpp: implementation of the CtlPointer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlPointer.h"
#include "../childview.h"
#include "../controlTracker.h"
#include "../mapvar.h"
#include "../mainvar.h"
#include "../mapform.h"
#include "../../dll/lib/axislib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtlPointer::CtlPointer(CChildView* view)
	: CTOOL(view)
{
	m_name  = _T("Pointer");
	m_block = m_view->m_block;
	m_nGap = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETGAP, 0);
	m_pSelFrame = NULL;
}

CtlPointer::~CtlPointer()
{
}

void CtlPointer::drawBegin(CPoint point)
{
	CRect	tRc;
	int nFormCnt = m_view->m_pFormItem->getFormCount(), ii;
	if (m_view->m_nTabOdr || m_view->m_nIoOdr) // taborder 
	{
		m_view->m_selF = -1;
		for (ii = 0; ii < nFormCnt; ii++) 
		{
			if ((m_view->m_nTabOdr && !m_view->m_pFormItem->getFormPoint(ii)->index) ||
				(m_view->m_nIoOdr && !m_view->m_pFormItem->getFormPoint(ii)->iorder))
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
		m_pFormItem->getFormPoint(m_view->m_selF)->block = true;
	}

	if (!(m_view->m_selF >= 0 && IsHit(m_view->m_selF, point)))
	{
		m_view->m_selF = -1;
		for (ii = 0; ii < nFormCnt; ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->group && 
				m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->group) != ii)
				continue;
			
			if (m_pFormItem->getFormPoint(ii)->tabN)
				tRc = getFormRect(ii, 1);
			else
				tRc = getFormRect(ii);
			
			if (tRc.PtInRect(point) || (!tRc.IsRectEmpty() && IsHit(ii, point)))
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

		if (m_pFormItem->getFormPoint(m_view->m_selF)->group)
			break;

		if (m_pFormItem->getFormPoint(m_view->m_selF)->kind == FM_GROUP || 
			m_pFormItem->getFormPoint(m_view->m_selF)->kind == FM_PANEL ||
			m_pFormItem->getFormPoint(m_view->m_selF)->kind == FM_BOX)
		{
			form = m_view->m_selF;
			CRect	sRc, fRc;
			sRc.SetRect(m_pFormItem->getFormPoint(form)->left, m_pFormItem->getFormPoint(form)->top, m_pFormItem->getFormPoint(form)->right, m_pFormItem->getFormPoint(form)->bottom);

			for (ii = 0; ii < form; ii++)
			{
				if (m_pFormItem->getFormStr(ii, "name").IsEmpty())
					continue;

				if (m_pFormItem->getFormPoint(ii)->group && 
					m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->group) != ii)
					continue;

				if (m_pFormItem->getFormPoint(ii)->tabN)
					fRc = getFormRect(ii, 1);
				else
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

	if (m_block->count > 1 && !(::GetKeyState(VK_SHIFT) & 0x8000) && m_view->m_selF >= 0 && !m_pFormItem->getFormPoint(m_view->m_selF)->block)
	{
		ReleaseBlock();
		m_view->RedrawWindow();
	}

	if (m_view->m_selF >= 0)
	{
		if (::GetKeyState(VK_SHIFT) & 0x8000)
		{
			m_pFormItem->getFormPoint(m_view->m_selF)->block = !m_pFormItem->getFormPoint(m_view->m_selF)->block;

			if (m_pFormItem->getFormPoint(m_view->m_selF)->block)
			{
				m_block->count++;
				if (m_block->count > 1)
				{
					for (int ii = m_pFormItem->getFormCount() - 1; ii >=0; ii--)
						if (m_pFormItem->getFormPoint(ii)->block && m_pFormItem->getFormPoint(ii)->tabN)
						{
							if (m_pFormItem->getFormPoint(ii)->tabN->block)
							{
								m_pFormItem->getFormPoint(ii)->tabN->block = false;
								m_block->count--;
							}
							break;
						}

				}
				if (m_block->count == 1 && m_pFormItem->getFormPoint(m_view->m_selF)->block)
					m_view->m_stdF = m_view->m_selF;
			}
			else
			{
				int selF = -1;
				m_block->count = 0;
				for (int ii = 0; ii < nFormCnt; ii++)
				{
					if (m_pFormItem->getFormPoint(ii)->block)
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
			if (!m_pFormItem->getFormPoint(m_view->m_selF)->block)
				ReleaseBlock();
			else
				m_view->m_stdF = m_view->m_selF;
		}
	}

	//m_dc->SelectStockObject(BLACK_PEN);
	m_draw->SetPen(m_dc, RGB(0, 0, 0), 1, PS_DOT);
	m_dc->SelectStockObject(NULL_BRUSH);
	m_dc->SetROP2(R2_XORPEN);

	if (::GetKeyState(VK_CONTROL) & 0x8000)
		m_view->m_selF = -1;

	if (m_view->m_selF == -1)
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
		if (!m_pFormItem->getFormPoint(m_view->m_selF)->block)
			formBegin(point);
		else
			blockBegin(point);

		switch (m_where)
		{
		case CControlTracker::hitTopLeft:
		case CControlTracker::hitBottomRight:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
			break;
		case CControlTracker::hitTopRight:
		case CControlTracker::hitBottomLeft:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
			break;
		case CControlTracker::hitTop:
		case CControlTracker::hitBottom:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			break;
		case CControlTracker::hitRight:
		case CControlTracker::hitLeft:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			break;
		case CControlTracker::hitMiddle:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			break;
		case CControlTracker::hitNothing:
		default:
			break;
		}
	}
}

void CtlPointer::drawTo(CPoint point)
{
	if (::GetKeyState(VK_SHIFT) & 0x8000)
		return;
	if (m_view->m_nTabOdr || m_view->m_nIoOdr)
		return;

	if (!m_view->m_bFirstDrawing)
		m_view->m_bRedraw = false;
	if (m_view->m_selF == -1)
	{
		m_dc->Rectangle(m_block->bRc);
		m_block->bRc.right  = point.x;
		m_block->bRc.bottom = point.y;
		m_dc->Rectangle(m_block->bRc);
	}
	else
	{
		if (!m_pFormItem->getFormPoint(m_view->m_selF)->block)
			formTo(point);
		else
			blockTo(point);
	}
}

void CtlPointer::drawEnd()
{
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
	if (::GetKeyState(VK_SHIFT) & 0x8000)
	{
		m_pRc.SetRectEmpty();
		for (int ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
			if (m_pFormItem->getFormPoint(ii)->block)
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
	if (m_view->m_selF == -1)
	{
		m_block->bRc.NormalizeRect();
		m_dc->Rectangle(m_block	->bRc);
		m_pRc = m_block->bRc;
		CRgn	pRgn;
		pRgn.CreateRectRgnIndirect(&m_block->bRc);
		m_block->count = 0;
		
		for (int ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormStr(ii, "name").IsEmpty())
				continue;
			
			if (m_pFormItem->getFormPoint(ii)->group && 
				m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->group) != ii)
				continue;
			
			if (m_pFormItem->getFormPoint(ii)->tabN)
			{
				tRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->tabN));
				if (pRgn.PtInRegion(CPoint(tRc.left, tRc.top)) && 
					pRgn.PtInRegion(CPoint(tRc.right, tRc.bottom)))
					continue;
			}
			
			/*tRc.SetRect(m_pFormItem->getFormPoint(ii)->left, m_pFormItem->getFormPoint(ii)->top,
				m_pFormItem->getFormPoint(ii)->right, m_pFormItem->getFormPoint(ii)->bottom);
			if (m_pFormItem->getFormPoint(ii)->group)
				tRc = GroupRect((int)m_pFormItem->getFormPoint(ii)->group);*/
			if (m_pFormItem->getFormPoint(ii)->tabN)
				tRc = getFormRect(ii, 1);
			else
				tRc = getFormRect(ii);
			
			if (pRgn.PtInRegion(CPoint(tRc.left, tRc.top)) && 
				pRgn.PtInRegion(CPoint(tRc.right, tRc.bottom)))
			{
				m_pRc.left   = __min(m_pRc.left, tRc.left);
				m_pRc.top    = __min(m_pRc.top, tRc.top);
				m_pRc.right  = __max(m_pRc.right, tRc.right);
				m_pRc.bottom = __max(m_pRc.bottom, tRc.bottom);
				m_pFormItem->getFormPoint(ii)->block = true;
				m_block->count++;
				
				if (m_block->count == 1)
					m_view->m_stdF = m_view->m_selF = ii;
			}
		}
		
		if (m_block->count == 1)
			m_pFormItem->getFormPoint(m_view->m_selF)->block = false;
		/*else
			m_view->m_block->count = m_block->count;*/
		
		pRgn.DeleteObject();
	}
	else
	{
		CPoint cursorPos;
		GetCursorPos(&cursorPos);
		if (m_firstCurPos != cursorPos)
		{
			if (!m_pFormItem->getFormPoint(m_view->m_selF)->block)
			{
				m_rect.NormalizeRect();
				//m_dc->DrawFocusRect(m_rect);
				
				if (m_pFormItem->getFormPoint(m_view->m_selF)->group)
				{
					CRect tRc = GroupRect((int)m_pFormItem->getFormPoint(m_view->m_selF)->group);
					setBlockGroup(m_view->m_selF, 
						m_rect.left - tRc.left, 
						m_rect.top - tRc.top);
				}
				else
				{
					if (m_pFormItem->getFormPoint(m_view->m_selF)->kind == FM_TAB)
					{
						CRect tRc = getFormRect(m_view->m_selF, NULL);
						setBlockTab(m_pFormItem->getFormPoint(m_view->m_selF), 
							m_rect.left - tRc.left, 
							m_rect.top - tRc.top);
					}
					m_pFormItem->getFormPoint(m_view->m_selF)->left   = m_rect.left;
					m_pFormItem->getFormPoint(m_view->m_selF)->top    = m_rect.top;
					m_pFormItem->getFormPoint(m_view->m_selF)->right  = m_rect.right;
					m_pFormItem->getFormPoint(m_view->m_selF)->bottom = m_rect.bottom;
				}
			}
			else
			{
				for (int ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
				{
					if (m_pFormItem->getFormPoint(ii)->block)
					{
						if (m_pFormItem->getFormPoint(ii)->group)
							setBlockGroup(ii, m_x, m_y);
						else
						{
							if (m_pFormItem->getFormPoint(ii)->kind == FM_TAB)
								setBlockTab(m_pFormItem->getFormPoint(ii), m_x, m_y);
							m_pFormItem->getFormPoint(ii)->left   += m_x;
							m_pFormItem->getFormPoint(ii)->top    += m_y;
							m_pFormItem->getFormPoint(ii)->right  += m_x;
							m_pFormItem->getFormPoint(ii)->bottom += m_y;
						}
						
						tRc.SetRect(m_pFormItem->getFormPoint(ii)->left, m_pFormItem->getFormPoint(ii)->top,
							m_pFormItem->getFormPoint(ii)->right, m_pFormItem->getFormPoint(ii)->bottom);
						if (m_pFormItem->getFormPoint(ii)->group)
							tRc = GroupRect((int)m_pFormItem->getFormPoint(ii)->group);
						//m_dc->DrawFocusRect(tRc);
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
	
	if (m_pSelFrame)
	{
		m_pSelFrame->DestroyWindow();
		delete m_pSelFrame;
		m_pSelFrame = NULL;
	}
	m_pRc.NormalizeRect();
	if (!m_pRc.IsRectEmpty())
	{
		m_pRc.InflateRect(5, 5);
		m_view->InvalidateRect(&m_pRc);
	}
}

void CtlPointer::formBegin(CPoint point)
{
	int	selF = m_view->m_selF;

	m_rect = getFormRect(selF);
	m_pRc = m_rect;

	CControlTracker	tracker(m_rect, CControlTracker::resizeOutside);
	int	hit = tracker.HitTest(point);
	m_where = CControlTracker::TrackerHit(hit);

  	switch (m_where) 
	{
	case CControlTracker::hitTopLeft:
      		m_rect.left   = point.x;
      		m_rect.top    = point.y;
      		break;
	case CControlTracker::hitTop:
      		m_rect.top    = point.y;
      		break;
	case CControlTracker::hitTopRight:
      		m_rect.top    = point.y;
      		m_rect.right  = point.x;
      		break;
	case CControlTracker::hitLeft:
      		m_rect.left   = point.x;
      		break;
	case CControlTracker::hitRight:
      		m_rect.right  = point.x;
      		break;
	case CControlTracker::hitBottomLeft:
      		m_rect.left   = point.x;
      		m_rect.bottom = point.y; 	 
      		break;
	case CControlTracker::hitBottom:
      		m_rect.bottom = point.y; 	 
      		break;
	case CControlTracker::hitBottomRight:
      		m_rect.right  = point.x;
      		m_rect.bottom = point.y; 	 
      		break;
  	default:
      		break;
  	}

	if (!m_pSelFrame)
	{
		m_pSelFrame = new CSelFrameWnd();
		m_pSelFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_rect, m_view, 1);
		m_pSelFrame->AddRect(m_rect);
		m_pSelFrame->SetTP();
	}
  	//m_dc->DrawFocusRect(m_rect);
	GetCursorPos(&m_firstCurPos);
}

void CtlPointer::blockBegin(CPoint point)
{
	CRect	tRc;
	m_x = m_y = 0;

	m_view->GetClientRect(&tRc);
	m_pRc.SetRect(0, 0, 0, 0);

	tRc = getFormRect(m_view->m_selF);

	CControlTracker	tracker(tRc, CControlTracker::resizeOutside);
	int	hit = tracker.HitTest(point);
	m_where = CControlTracker::TrackerHit(hit);
	if (!m_pSelFrame && !(::GetKeyState(VK_SHIFT) & 0x8000))
	{
		m_pSelFrame = new CSelFrameWnd();
		m_pSelFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, tRc, m_view, 1);
		m_pSelFrame->ModifyStyleEx(NULL, WS_EX_TOPMOST);
	}
	for (int ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
	{
		if (m_pFormItem->getFormPoint(ii)->block)
		{
			CRect rc = getFormRect(ii);
			
			m_pRc.UnionRect(m_pRc, rc);
			if (!(::GetKeyState(VK_SHIFT) & 0x8000))
				m_pSelFrame->AddRect(rc);
		}
	}
	if (m_pSelFrame)
		m_pSelFrame->SetTP();

	m_pRc.InflateRect(5, 5);
}

void CtlPointer::formTo(CPoint point)
{
	if (!(::GetKeyState(VK_CONTROL) & 0x8000) && m_where != CControlTracker::hitMiddle && m_lastpt != point)
	{
		point.x = ((int)(point.x / m_nGap)) * m_nGap;
		point.y = ((int)(point.y / m_nGap)) * m_nGap;
	}

	int	selF = m_view->m_selF;

	m_pSelFrame->SetWindowPos(NULL, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), SWP_NOZORDER);

	if (m_where != CControlTracker::hitMiddle && m_pFormItem->getFormPoint(selF)->group)
		return;
  	switch (m_where) 
	{
	case CControlTracker::hitTopLeft:
      		m_rect.left = point.x;
      		m_rect.top  = point.y;
      		break;
	case CControlTracker::hitTop:
      		m_rect.top = point.y;
      		break;
	case CControlTracker::hitTopRight:
      		m_rect.top   = point.y;
      		m_rect.right = point.x;
      		break;
	case CControlTracker::hitLeft:
      		m_rect.left = point.x;
      		break;
	case CControlTracker::hitMiddle:
      		m_x = point.x - m_lastpt.x;
      		m_y = point.y - m_lastpt.y;
		m_rect = getFormRect(selF);
		m_rect.OffsetRect(m_x, m_y);
		if (!(::GetKeyState(VK_CONTROL) & 0x8000) && m_lastpt != point)
		{
			m_rect.OffsetRect(((int)(m_rect.left / m_nGap)) * m_nGap - m_rect.left,
				((int)(m_rect.top / m_nGap)) * m_nGap - m_rect.top);
		}
      		break;
	case CControlTracker::hitRight:
      		m_rect.right = point.x;
      		break;
	case CControlTracker::hitBottomLeft:
      		m_rect.left   = point.x;
      		m_rect.bottom = point.y;
      		break;
	case CControlTracker::hitBottom:
      		m_rect.bottom = point.y;
      		break;
	case CControlTracker::hitBottomRight:
      		m_rect.right  = point.x;
      		m_rect.bottom = point.y;
      		break;
  	default: 
		break;
  	}

	if (m_where == CControlTracker::hitMiddle)
	{
		CString sTip;
		sTip.Format("(%d, %d, %d, %d)", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 1), (LPARAM)sTip.operator LPCTSTR());
	}
	else
	{
		CString sTip;
		if (!(::GetKeyState(VK_CONTROL) & 0x8000))
		{
			point.x = ((int)(point.x / m_nGap)) * m_nGap;
			point.y = ((int)(point.y / m_nGap)) * m_nGap;
		}
		sTip.Format("(%d, %d)", point.x, point.y);
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 1), (LPARAM)sTip.operator LPCTSTR());
	}

	m_pRc.UnionRect(m_pRc, m_rect);
}

void CtlPointer::blockTo(CPoint point)
{
	int Cnt = 0, xx, yy;

	xx = point.x - m_lastpt.x;
	yy = point.y - m_lastpt.y;
	m_x += xx;
	m_y += yy;

	m_pSelFrame->SetWindowPos(NULL, m_pRc.left + m_x + 5, m_pRc.top + m_y + 5, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

	m_lastpt = point;
}

void CtlPointer::setBlockGroup(int nGroup, int mx, int my)
{
	for (int ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
		if (m_pFormItem->getFormPoint(ii)->group == m_pFormItem->getFormPoint(nGroup)->group)
		{
			if (m_pFormItem->getFormPoint(ii)->kind == FM_TAB)
				setBlockTab(m_pFormItem->getFormPoint(ii), mx, my);
			m_pFormItem->getFormPoint(ii)->left   += mx;
			m_pFormItem->getFormPoint(ii)->top    += my;
			m_pFormItem->getFormPoint(ii)->right  += mx;
			m_pFormItem->getFormPoint(ii)->bottom += my;
		}
}

void CtlPointer::setBlockTab(struct _formR *tabN, int mx, int my)
{
	for (int ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
	{
		if (m_pFormItem->getFormPoint(ii)->tabN == tabN)
		{
			if (m_pFormItem->getFormPoint(ii)->kind == FM_TAB)
				setBlockTab(m_pFormItem->getFormPoint(ii), mx, my);
			m_pFormItem->getFormPoint(ii)->left   += mx;
			m_pFormItem->getFormPoint(ii)->top    += my;
			m_pFormItem->getFormPoint(ii)->right  += mx;
			m_pFormItem->getFormPoint(ii)->bottom += my;
		}
	}
}

bool CtlPointer::IsHit(int idx, CPoint point)
{
	CRect tRc;
	tRc.SetRect(m_pFormItem->getFormPoint(idx)->left, m_pFormItem->getFormPoint(idx)->top, m_pFormItem->getFormPoint(idx)->right, m_pFormItem->getFormPoint(idx)->bottom);
	if (m_pFormItem->getFormPoint(idx)->group)
		return false;
	CControlTracker	tracker(tRc, CControlTracker::resizeOutside);
	int	hit = tracker.HitTest(point);
	if (CControlTracker::TrackerHit(hit) == CControlTracker::hitNothing ||
		CControlTracker::TrackerHit(hit) == CControlTracker::hitMiddle)
		return false;
	return true;
}


