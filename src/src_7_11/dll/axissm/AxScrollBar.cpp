// AxScrollBar.cpp : implementation file
//

#include "stdafx.h"
#include "AxScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxScrollBar

CAxScrollBar::CAxScrollBar()
{
}

CAxScrollBar::~CAxScrollBar()
{
}


BEGIN_MESSAGE_MAP(CAxScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CAxScrollBar)
	ON_WM_VSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxScrollBar message handlers

void CAxScrollBar::VScroll(UINT nSBCode, UINT nPos) 
{
//	GetParent()->SendMessage(WM_VSCROLL, nSBCode, nPos);
	switch (nSBCode)
	{
	case	SB_LINEUP:		// move 1
		LineUp();
		break;
	case	SB_LINEDOWN:		// move 1
		LineDown();
		break;
	case	SB_PAGEUP:
		PageUp();		// move nPage
		break;
	case	SB_PAGEDOWN:		// move nPage
		PageDown();
		break;
	case	SB_THUMBTRACK:
		ThumbTrack(nPos);
		break;
	default:
		break;
	}
	GetParent()->Invalidate();
}

BOOL CAxScrollBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	SCROLLINFO	info;
	int		deltaPOS = zDelta/120;

	GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	if (deltaPOS)
	{
		info.nPos -= deltaPOS*3;
		if (info.nMin > info.nPos)
			info.nPos = info.nMin;
		SetScrollInfo(&info);
		SetScrollPos(info.nPos, true);
	}
	else
	{
		info.nPos += deltaPOS*3;
		if (info.nMax > info.nPos)
			info.nPos = info.nMax;
		SetScrollInfo(&info);
		SetScrollPos(info.nPos, true);
	}
	GetParent()->Invalidate();
	return true;
}

void CAxScrollBar::LineUp()
{
	SCROLLINFO	info;
	GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	info.nPos--;
	if (info.nMin > info.nPos)
		info.nPos = info.nMin;
	SetScrollInfo(&info);
	SetScrollPos(info.nPos, true);
}

void CAxScrollBar::LineDown()
{
	SCROLLINFO	info;
	GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	info.nPos++;
	if (info.nMax + 1< info.nPos + (int)info.nPage)
		info.nPos = info.nMax - info.nPage + 1;
	SetScrollInfo(&info);
	SetScrollPos(info.nPos, true);
}

void CAxScrollBar::PageUp()
{
	SCROLLINFO	info;
	GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	info.nPos -= info.nPage;
	if (info.nMin > info.nPos)
		info.nPos = info.nMin;
	SetScrollInfo(&info);
	SetScrollPos(info.nPos, true);
}

void CAxScrollBar::PageDown()
{
	SCROLLINFO	info;
	GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	
	info.nPos += info.nPage;
	if (info.nMax + 1< info.nPos + (int)info.nPage)
		info.nPos = info.nMax - info.nPage + 1;
	SetScrollInfo(&info);
	SetScrollPos(info.nPos, true);
}

void CAxScrollBar::ThumbTrack(UINT nPos)
{
	SCROLLINFO	info;
	GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	info.nPos = nPos;
	if (info.nPos + (int)info.nPage > info.nMax + 1)
		info.nPos = info.nMax - info.nPage + 1;
	SetScrollInfo(&info);
	SetScrollPos(info.nPos, false);
}
