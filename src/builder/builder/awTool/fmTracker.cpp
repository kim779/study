
#include "stdafx.h"
#include "../ChildView.h"
#include "fmTracker.h"
#include "TrackerWnd.h"

CfmTracker::CfmTracker(CChildView* view, CObjectLoad* ObjLoad)
{
	m_view = view;
	m_x = 0;
	m_y = 0;
	m_bFst = false;
	m_ObjLoad = ObjLoad;
	m_nHit = 0;

	for (size_t ii = 0; ii < 8; ii++)
	{
		m_TrkWnd[ii] = NULL;
	}
}

CfmTracker::~CfmTracker()
{
	for (size_t ii = 0; ii < 8; ii++)
	{
		if (m_TrkWnd[ii])
		{
			m_TrkWnd[ii]->DestroyWindow();
			delete m_TrkWnd[ii];
		}
	}
}

void CfmTracker::SetPostion(int x, int y, int w, int h)
{
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;

	SetPostion();
}

void CfmTracker::SetPostion()
{
	int gapX, gapY, marginX, marginY;
	gapX = m_w / 2 - 2;
	gapY = m_h / 2 - 2;
	marginX = m_w % 2 - 1;
	marginY = m_h % 2 - 1;

	m_rc[0].SetRect(m_x, m_y, m_x + 5, m_y + 5);
	m_rc[1].CopyRect(m_rc[0]);
	m_rc[1].OffsetRect(gapX, 0);
	m_rc[2].CopyRect(m_rc[1]);
	m_rc[2].OffsetRect(gapX + marginX, 0);
	m_rc[3].CopyRect(m_rc[2]);
	m_rc[3].OffsetRect(0, gapY);
	m_rc[4].CopyRect(m_rc[3]);
	m_rc[4].OffsetRect(0, gapY + marginY);
	m_rc[5].CopyRect(m_rc[4]);
	m_rc[5].OffsetRect(-gapX - marginX, 0);
	m_rc[6].CopyRect(m_rc[5]);
	m_rc[6].OffsetRect(-gapX, 0);
	m_rc[7].CopyRect(m_rc[6]);
	m_rc[7].OffsetRect(0, -gapY - marginY);
}

void CfmTracker::MovePostion(CPoint point)
{
	m_x += point.x;
	m_y += point.y;

	m_rc[0].OffsetRect(point);
	m_rc[1].OffsetRect(point);
	m_rc[2].OffsetRect(point);
	m_rc[3].OffsetRect(point);
	m_rc[4].OffsetRect(point);
	m_rc[5].OffsetRect(point);
	m_rc[6].OffsetRect(point);
	m_rc[7].OffsetRect(point);
}

void CfmTracker::ResizePostion(int left, int top, int right, int bottom)
{
	MovePostion(CPoint(left,top));
	
	m_w += right;
	m_h += bottom;
	SetPostion();
}

void CfmTracker::SetFst(bool bFst)
{
	m_bFst = bFst;
	for (size_t ii = 0; ii < 8; ii++)
	{
		if(m_TrkWnd[ii]) m_TrkWnd[ii]->SetFst(m_bFst);
	}
}

void CfmTracker::MoveTracker()
{
	for (size_t ii = 0; ii < 8; ii++)
	{
		m_TrkWnd[ii]->MoveWindow(m_rc[ii]);
	}
}

void CfmTracker::Hide()
{
	for (size_t ii = 0; ii < 8; ii++)
	{
		m_TrkWnd[ii]->ShowWindow(SW_HIDE);
	}
}

void CfmTracker::SetHit(int nHit)
{
	m_nHit = nHit;
}

void CfmTracker::Redraw()
{
	for (size_t ii = 0; ii < 8; ii++)
	{
		m_TrkWnd[ii]->Invalidate();
		m_TrkWnd[ii]->ShowWindow(SW_SHOW);
	}
}

void CfmTracker::Init()
{
	for (size_t ii = 0; ii < 8; ii++)
	{
		m_TrkWnd[ii] = new CTrackerWnd(m_view);
		m_TrkWnd[ii]->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_rc[ii], m_view->GetParent(), -1);
		m_TrkWnd[ii]->SetFst(m_bFst);
	}
}