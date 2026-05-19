// TrListctrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "TrListctrl.h"


// CTrListctrl

IMPLEMENT_DYNAMIC(CTrListctrl, CListCtrl)

CTrListctrl::CTrListctrl()
{
	m_FocusCol = -1;
	m_FocusRow = -1;
	m_skipcol = -1;
	m_bfocus = true;
}

CTrListctrl::~CTrListctrl()
{
}


BEGIN_MESSAGE_MAP(CTrListctrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

int CTrListctrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CTrListctrl::AddItem(int item, int subItem, CString str)
{
	LV_ITEM	lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subItem;
	lvi.pszText  = (LPTSTR) str.operator LPCTSTR();

	if (subItem == 0)
		InsertItem(&lvi);
	else
		SetItem(&lvi);
}

void CTrListctrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hitinfo = {0};
	hitinfo.flags = nFlags;
	hitinfo.pt = point;
	SubItemHitTest(&hitinfo);

	m_FocusCol = hitinfo.iSubItem;
	m_FocusRow = hitinfo.iItem;
	//TRACE("%d\t%d\n", m_FocusCol, m_FocusRow);
	CListCtrl::OnLButtonDown(nFlags, point);
	UpdateFocusCell(hitinfo.iSubItem);
}

void CTrListctrl::UpdateFocusCell(int nCol)
{
	m_FocusCol = nCol;
	int nFocusRow = GetNextItem(-1, LVNI_FOCUSED);

	if (nFocusRow >= 0)
	{
		CRect itemRect;
		VERIFY(GetItemRect(nFocusRow, itemRect, LVIR_BOUNDS));
		InvalidateRect(itemRect);
		UpdateWindow();
	}
}

void CTrListctrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_bfocus)
		return;

	if (m_FocusCol < m_skipcol)
		return;

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)(pNMHDR);
	int nRow = (int)pLVCD->nmcd.dwItemSpec;

	int nRowItemData = (int)pLVCD->nmcd.lItemlParam;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult |= CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		{
			if (pLVCD->nmcd.uItemState & CDIS_FOCUS)
			{
				if (GetNextItem(-1, LVNI_FOCUSED)==nRow)
				{
					if (m_FocusCol >= 0)
					{
						pLVCD->nmcd.uItemState &= ~CDIS_FOCUS;
						*pResult |= CDRF_NOTIFYPOSTPAINT;
					}
				}
			}

			if (pLVCD->nmcd.uItemState & CDIS_SELECTED)
			{
				if (m_FocusCol!=-1)
					*pResult |= CDRF_NOTIFYSUBITEMDRAW;
			}
		} 
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			int nCol = pLVCD->iSubItem;

			if (pLVCD->nmcd.uItemState & CDIS_SELECTED
				&& m_FocusCol==nCol
				&& GetNextItem(-1, LVNI_FOCUSED)==nRow)
			{
				pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;
			}
		} 
		break;
	case CDDS_ITEMPOSTPAINT:
		{
			if (GetNextItem(-1, LVNI_FOCUSED)!=nRow)
				break;
			if (m_FocusCol >= 0)
			{
				CRect rcHighlight;
				CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
				VERIFY(GetCellRect(nRow, m_FocusCol, rcHighlight));
				pDC->DrawFocusRect(rcHighlight);
			}
		}
		break;
	}
}

BOOL CTrListctrl::GetCellRect(int nRow, int nCol, CRect& rect)
{
	CRect rowRect;
	if (GetItemRect(nRow, rowRect, LVIR_BOUNDS)==FALSE)
		return FALSE;

	CRect colRect;
	if (GetHeaderCtrl()->GetItemRect(nCol, colRect)==FALSE)
		return FALSE;

	colRect.left -= GetScrollPos(SB_HORZ);
	colRect.right -= GetScrollPos(SB_HORZ);

	rect.left = colRect.left;
	rect.top = rowRect.top;
	rect.right = colRect.right;
	rect.bottom = rowRect.bottom;
	return TRUE;
}