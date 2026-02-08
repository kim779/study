// PnWndCtrl.cpp: implementation of the CPnWndCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnWndCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////
// SliderCtrl
////////////////////////////////////////////////////////////

CPnSlider::CPnSlider()
{
}

CPnSlider::~CPnSlider()
{
}

BEGIN_MESSAGE_MAP(CPnSlider, CSliderCtrl)
	//{{AFX_MSG_MAP(CPnSlider)
	ON_WM_MOUSEWHEEL()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPnSlider::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta > 0)
	{
		GetParent()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, ctrlWheelLeft), NULL);
	}
	else
	{
		GetParent()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, ctrlWheelRight), NULL);
	}
	return TRUE;
	
	return CSliderCtrl::OnMouseWheel(nFlags, zDelta, pt);
}


/////////////////////////////////////////////////////////////
// ScrollBar
/////////////////////////////////////////////////////////////

CPnScrollBar::CPnScrollBar()
{
}

CPnScrollBar::~CPnScrollBar()
{
}


BEGIN_MESSAGE_MAP(CPnScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CPnScrollBar)
	ON_WM_HSCROLL_REFLECT()
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

void CPnScrollBar::Initialize(SCROLLINFO sInfo)
{
	SetScrollInfo(&sInfo);
//	if ((int)sInfo.nPage >= sInfo.nMax)
//		EnableScrollBar(ESB_DISABLE_BOTH);
//	else
		EnableScrollBar(ESB_ENABLE_BOTH);
}

void CPnScrollBar::HScroll(UINT nSBCode, UINT nPos) 
{
	SCROLLINFO sInfo;
	GetScrollInfo(&sInfo, SIF_ALL);  
	int pos = GetScrollPos();

	switch (nSBCode)
	{
	case SB_LINEDOWN:
	case SB_LINEUP:
		GetOwner()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, nSBCode), 0);
		break;
	case SB_PAGEDOWN:
		if ((nPos = getPos(SB_PAGEDOWN, sInfo.nPage)) >= 0)
			SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, nSBCode), GetScrollPos());
		break;
	case SB_PAGEUP:
		if ((nPos = getPos(SB_PAGEUP)) >= 0)
			SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, nSBCode), GetScrollPos());
		break;
	case SB_THUMBTRACK:
		SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, nSBCode), GetScrollPos());
		break;
	default:
		break;
	}

}

int CPnScrollBar::getPos(int type, int gap)
{
	int nPos = -1;
	
	CPoint	pt;
	GetCursorPos(&pt);
	GetOwner()->ScreenToClient(&pt);
	CRect scrollRC;
	GetClientRect(scrollRC);
	ClientToScreen(scrollRC);

	GetOwner()->ScreenToClient(scrollRC);
	int cx = GetSystemMetrics(SM_CXHSCROLL);
	scrollRC.left += cx;
	scrollRC.right -= cx;
	if (scrollRC.PtInRect(pt))
	{
		int xpos = pt.x - scrollRC.left;
		nPos = int(100 * xpos / (float)scrollRC.Width());
		switch (type)
		{
		case SB_PAGEDOWN:
			nPos = nPos - gap + 2;
			break;
		case SB_PAGEUP:
			break;
		}
	}
	return nPos;
	
}

