// ContractScroll.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FODepth.h"
#include "ContractScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContractScroll

CContractScroll::CContractScroll()
{
}

CContractScroll::~CContractScroll()
{
}


BEGIN_MESSAGE_MAP(CContractScroll, CScrollBar)
	//{{AFX_MSG_MAP(CContractScroll)
	ON_WM_VSCROLL_REFLECT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CContractScroll::Initialize(SCROLLINFO sInfo)
{
	SetScrollInfo(&sInfo);
	return;
	if ((int)sInfo.nPage >= sInfo.nMax)
		EnableScrollBar(ESB_DISABLE_BOTH);
	else
		EnableScrollBar(ESB_ENABLE_BOTH);
}

void CContractScroll::VScroll(UINT nSBCode, UINT nPos) 
{
	SCROLLINFO sInfo{};

	GetScrollInfo(&sInfo, SIF_ALL);  
	const	int	pos = GetScrollPos();

	switch (nSBCode)
	{
	case SB_LINEUP:
	case SB_LINEDOWN:
		GetOwner()->SendMessage(WM_SCRL, MAKEWPARAM(SCRL_VCHANGE, nSBCode), 0);
		break;
	case SB_PAGEUP:
		if ((nPos = getPos(SB_PAGEUP)) >= 0)
			SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(WM_SCRL, MAKEWPARAM(SCRL_VCHANGE, nSBCode), GetScrollPos());
		break;
	case SB_PAGEDOWN:
//		if ((nPos = getPos(SB_PAGEDOWN, sInfo.nPage)) >= 0)
//			SetScrollPos(nPos);
//		if (pos != GetScrollPos())
			GetOwner()->SendMessage(WM_SCRL, MAKEWPARAM(SCRL_VCHANGE, nSBCode), GetScrollPos());
		break;
	case SB_THUMBTRACK:
		SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(WM_SCRL, MAKEWPARAM(SCRL_VCHANGE, nSBCode), GetScrollPos());
		break;
	default:
		break;
	}
}

int CContractScroll::getPos(int type, int gap)
{
	CPoint	pt;
	CRect	scrollRC;
	int	nPos = -1;
	const	int	cx = GetSystemMetrics(SM_CXHSCROLL);

	GetCursorPos(&pt);
	GetOwner()->ScreenToClient(&pt);
	GetClientRect(scrollRC);
	ClientToScreen(scrollRC);
	GetOwner()->ScreenToClient(scrollRC);
	scrollRC.left  += cx;
	scrollRC.right -= cx;
	if (scrollRC.PtInRect(pt))
	{
		const	int	xpos = pt.x - scrollRC.left;
		nPos = (int)(100 * xpos / (float)scrollRC.Width());
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

BOOL CContractScroll::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
