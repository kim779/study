// listCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB210200.h"
#include "listCtrl.h"
#include "item.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	clrTEXT		RGB(  0,   0,   0)
#define	clrBACK		RGB(245, 245, 245)
#define	clrSEL		RGB(199, 203, 221)

/////////////////////////////////////////////////////////////////////////////
// ClistCtrl

ClistCtrl::ClistCtrl()
{
	m_bMouseDown = false;
}

ClistCtrl::~ClistCtrl()
{
}


BEGIN_MESSAGE_MAP(ClistCtrl, CListCtrl)
	//{{AFX_MSG_MAP(ClistCtrl)
	ON_WM_NCCALCSIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDrawList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ClistCtrl message handlers

void ClistCtrl::PreSubclassWindow() 
{
	SetExtendedStyle(GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CListCtrl::PreSubclassWindow();
}

void ClistCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	UpdateWindow();
	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void ClistCtrl::OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW *)pNMHDR;

	*pResult = CDRF_DODEFAULT;
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
		{
			CDC* dc{};
			Citem* item{};
			int	row=0, subi=0;
			CString	text;
			CRect	rect;

			dc   = CDC::FromHandle(pLVCD->nmcd.hdc);
			row  = (int)pLVCD->nmcd.dwItemSpec;
			subi = (int)pLVCD->iSubItem;
			text = GetItemText(row, subi);
			item = (Citem *)GetItemData(row);
			if (!item)
			{
				*pResult = CDRF_DODEFAULT|CDRF_NOTIFYPOSTPAINT;
				break;
			}
			CListCtrl::GetSubItemRect(row, subi, LVIR_BOUNDS, rect);
			switch (subi)
			{
			case 0:	case 2:
				if (CListCtrl::GetItemState(row, LVIS_SELECTED))
					dc->FillSolidRect(rect, clrSEL);
				else
					dc->FillSolidRect(rect, clrBACK);
				dc->SetBkMode(TRANSPARENT);
				dc->SetTextColor(!subi ? item->m_clr : clrTEXT);
				dc->DrawText(text, rect, !subi ? DT_SINGLELINE|DT_VCENTER|DT_LEFT : DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				break;			
			case 1:
				dc->FillSolidRect(rect, item->m_clr);
				break;
			}
		}
		*pResult = CDRF_SKIPDEFAULT;	//CDRF_DODEFAULT|CDRF_NOTIFYPOSTPAINT;
		break;

	default:// it wasn't a notification that was interesting to us.
		*pResult = CDRF_DODEFAULT;
		break;
	}
}

void ClistCtrl::Selected(int nItem, BOOL scroll)
{
	CRect	rc;
	CSize	size;

	SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	if (scroll)
	{
		GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - GetTopIndex());
		Scroll(size);
	}
}

void ClistCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bMouseDown = true;
	CListCtrl::OnLButtonDown(nFlags, point);
}

void ClistCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate();	// 스크롤일때 라인 이상하게 칠함
}

void ClistCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	MouseUpEvent();
	CListCtrl::OnLButtonUp(nFlags, point);
}

void ClistCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bMouseDown)
		CListCtrl::OnMouseMove(nFlags, point);
}

void ClistCtrl::MouseUpEvent()
{
	m_bMouseDown = false;
}

