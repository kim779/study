// symboltreectrl.cpp : implementation file
//

#include "stdafx.h"
#include "..\axiswork.h"
#include "symboltreectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolTreeCtrl

CSymbolTreeCtrl::CSymbolTreeCtrl()
{
	m_firstItem = NULL;
}

CSymbolTreeCtrl::~CSymbolTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CSymbolTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CSymbolTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolTreeCtrl message handlers

int CSymbolTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CImageList	imglist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_SERVERTREE);
	imglist.Create(16, 16, ILC_COLOR24|ILC_MASK, 5, 1);
	imglist.Add(&bitmap, RGB(0, 255, 255));
	imglist.SetBkColor(CLR_NONE);
	SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();
	
	return 0;
}

void CSymbolTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	SetItemImage((HTREEITEM)pNMTreeView->itemNew.hItem, (int)(pNMTreeView->action - 1), (int)(pNMTreeView->action - 1));	
	*pResult = 0;
}

int CSymbolTreeCtrl::GetSelectedItemCount()
{
	m_arItem.RemoveAll();
	HTREEITEM hitem = getFirstSelectedItem();
	if (hitem == (HTREEITEM) NULL)
		return 0;

	if (!ItemHasChildren(hitem))
		m_arItem.Add(hitem);
	for (hitem = getNextSelectedItem(hitem); hitem != NULL; hitem = getNextSelectedItem(hitem))
	{
		if (hitem == (HTREEITEM) NULL)
			break;

		if (ItemHasChildren(hitem))
			continue;

		m_arItem.Add(hitem);
	}

	return m_arItem.GetSize();
}

HTREEITEM CSymbolTreeCtrl::getFirstSelectedItem()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

HTREEITEM CSymbolTreeCtrl::getNextSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetNextVisibleItem(hitem); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

void CSymbolTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_singleItem = NULL;
	if (nFlags & MK_CONTROL)
	{
		UINT	flag;
		HTREEITEM hitem = HitTest(point, &flag);
		if (hitem)
		{
			// toggle selection state
			UINT newSel = GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED ? 0 : TVIS_SELECTED;

			// get old selected item & state
			HTREEITEM olditem = GetSelectedItem();
			UINT oldSel = olditem ? GetItemState(olditem, TVIS_SELECTED) : 0;

			// select new item
			if (GetSelectedItem() == hitem)
				SelectItem(NULL);
		
			CTreeCtrl::OnLButtonDown(nFlags, point);

			// set proper selection state for new item
			SetItemState(hitem, newSel, TVIS_SELECTED);

			// restore state of old selected item
			if (olditem && olditem != hitem)
				SetItemState(olditem, oldSel, TVIS_SELECTED);

			m_firstItem = NULL;
			//return;
		}
	}
	else if (nFlags & MK_SHIFT)
	{
		// shift key is down
		UINT flag;
		HTREEITEM hitem = HitTest(point, &flag);

		// initialize the reference item if this is the first shift selection
		if (!m_firstItem)
			m_firstItem = GetSelectedItem();

		// select new item
		if (GetSelectedItem() == hitem)
			SelectItem(NULL);

		CTreeCtrl::OnLButtonDown(nFlags, point);

		if (m_firstItem)
		{
			selectItems(m_firstItem, hitem);
			//return;
		}
	}
	else
	{
		// normal - remove all selection ans let default handler do the rest
		if (GetSelectedItemCount() > 1)
		{
			UINT flag;
			HTREEITEM hitem = HitTest(point, &flag);
			if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			{
				m_singleItem = hitem;
				m_dragRect = CRect(point.x, point.y, point.x, point.y);
				m_dragRect.DeflateRect(-1, -1, 1, 1);
				return;
			}
		}
		ClearSelection();
		UINT flag;
		HTREEITEM hitem = HitTest(point, &flag);
		SelectItem(hitem);
		m_firstItem = NULL;
		Invalidate();
		CTreeCtrl::OnLButtonDown(nFlags, point);
	}
}

void CSymbolTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_singleItem)
	{
		ClearSelection();
		SelectItem(m_singleItem);
		//Invalidate();
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CSymbolTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_UP || nChar == VK_DOWN) && GetKeyState(VK_SHIFT) & 0x8000)
	{
		// initialize the reference item if this is the first shift selection
		if (!m_firstItem)
		{
			m_firstItem = GetSelectedItem();
			ClearSelection();
		}

		// find which item is currently selected
		HTREEITEM prevItem = GetSelectedItem();
		HTREEITEM nextItem;
		if (nChar == VK_UP)
			nextItem = GetPrevVisibleItem(prevItem);
		else
			nextItem = GetNextVisibleItem(prevItem);

		if (nextItem)
		{
			// determine if we need to reselect previous selected item
			BOOL reselected = !(GetItemState(nextItem, TVIS_SELECTED) & TVIS_SELECTED);

			// select the next item - this will also deselect the previous item
			SelectItem(nextItem);

			// reselect the previously selected item
			if (reselected)
				SetItemState(prevItem, TVIS_SELECTED, TVIS_SELECTED);
		}
		return;
	}
	else if (nChar >= VK_SPACE)
	{
		m_firstItem = NULL;
		ClearSelection();
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSymbolTreeCtrl::ClearSelection()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			SetItemState(hitem, 0, TVIS_SELECTED);
	}
	SelectItem(NULL);
}

bool CSymbolTreeCtrl::selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO)
{
	HTREEITEM hitem = GetRootItem();

	while (hitem && hitem != hitemFROM && hitem != hitemTO)
	{
		hitem = GetNextVisibleItem(hitem);
		SetItemState(hitem, 0, TVIS_SELECTED);
	}

	if (!hitem)
		return false;		// item is not visible

	SelectItem(hitemTO);

	// rearrange hitemFROM and hitemTO so that m_firstItem is at top
	if (hitem == hitemTO)
	{
		hitemTO   = hitemFROM;
		hitemFROM = hitem;
	}

	// go through remaining visible  items
	BOOL	select = TRUE;
	while (hitem)
	{
		// select or remove selection depending on whether item
		// is still wuthun the range
		SetItemState(hitem, select ? TVIS_SELECTED : 0, TVIS_SELECTED);

		// do we need start removing items from selection
		if (hitem == hitemTO)
			select = FALSE;

		hitem = GetNextVisibleItem(hitem);
	}

	return true;
}


void CSymbolTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ((nFlags & MK_LBUTTON) && !m_dragRect.PtInRect(point))
	{
		NM_TREEVIEW nmhdr;
		nmhdr.ptDrag       = point;
		nmhdr.hdr.hwndFrom = m_hWnd;
		nmhdr.hdr.idFrom   = GetDlgCtrlID();
		nmhdr.hdr.code     = TVN_BEGINDRAG;
		CWnd *pOwner = GetOwner();
		if (pOwner && IsWindow(pOwner->m_hWnd))
			pOwner->SendMessage(WM_NOTIFY, nmhdr.hdr.idFrom, (LPARAM)&nmhdr);
		m_dragRect = CRect(0, 0, 0, 0);
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}
