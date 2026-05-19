// treeListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "gridListCtrl.h"


// CGridListCtrl

IMPLEMENT_DYNAMIC(CGridListCtrl, CListCtrl)

CGridListCtrl::CGridListCtrl()
{
	m_focusCol = -1;
	m_focusRow = -1;
	m_skipCol  = -1;
	m_focus    = true;
}

CGridListCtrl::~CGridListCtrl()
{
}


BEGIN_MESSAGE_MAP(CGridListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()


// CGridListCtrl 메시지 처리기입니다.

int CGridListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CGridListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hitinfo = {0};
	hitinfo.flags = nFlags;
	hitinfo.pt = point;
	SubItemHitTest(&hitinfo);

	m_focusCol = hitinfo.iSubItem;
	m_focusRow = hitinfo.iItem;
	CListCtrl::OnLButtonDown(nFlags, point);
	UpdateFocusCell(hitinfo.iSubItem);
}

void CGridListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_focus)
		return;

	if (m_focusCol < m_skipCol)
		return;

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)(pNMHDR);
	int	nRow = (int)pLVCD->nmcd.dwItemSpec;
	int	RowItemData = (int)pLVCD->nmcd.lItemlParam;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult |= CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		if (pLVCD->nmcd.uItemState & CDIS_FOCUS)
		{
			if (GetNextItem(-1, LVNI_FOCUSED)==nRow)
			{
				if (m_focusCol >= 0)
				{
					pLVCD->nmcd.uItemState &= ~CDIS_FOCUS;
					*pResult |= CDRF_NOTIFYPOSTPAINT;
				}
			}
		}

		if (pLVCD->nmcd.uItemState & CDIS_SELECTED)
		{
			if (m_focusCol!=-1)
				*pResult |= CDRF_NOTIFYSUBITEMDRAW;
		}
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			int	nCol = pLVCD->iSubItem;

			if (pLVCD->nmcd.uItemState & CDIS_SELECTED
				&& m_focusCol==nCol
				&& GetNextItem(-1, LVNI_FOCUSED)==nRow)
			{
				pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;
			}
		} 
		break;

	case CDDS_ITEMPOSTPAINT:
		if (GetNextItem(-1, LVNI_FOCUSED)!=nRow)
			break;
		if (m_focusCol >= 0)
		{
			CRect rcHighlight;
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			VERIFY(GetCellRect(nRow, m_focusCol, rcHighlight));
			pDC->DrawFocusRect(rcHighlight);
		}
		break;
	}
}

void CGridListCtrl::AddItem(int item, int subItem, CString str)
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

void CGridListCtrl::UpdateFocusCell(int nCol)
{
	m_focusCol = nCol;
	int nFocusRow = GetNextItem(-1, LVNI_FOCUSED);

	if (nFocusRow >= 0)
	{
		CRect itemRect;
		VERIFY(GetItemRect(nFocusRow, itemRect, LVIR_BOUNDS));
		InvalidateRect(itemRect);
		UpdateWindow();
	}
}

BOOL CGridListCtrl::GetCellRect(int nRow, int nCol, CRect& rect)
{
	CRect rowRect;

	if (!GetItemRect(nRow, rowRect, LVIR_BOUNDS))
		return FALSE;

	CRect colRect;
	if (!GetHeaderCtrl()->GetItemRect(nCol, colRect))
		return FALSE;

	colRect.left -= GetScrollPos(SB_HORZ);
	colRect.right -= GetScrollPos(SB_HORZ);

	rect.left = colRect.left;
	rect.top = rowRect.top;
	rect.right = colRect.right;
	rect.bottom = rowRect.bottom;
	return TRUE;
}