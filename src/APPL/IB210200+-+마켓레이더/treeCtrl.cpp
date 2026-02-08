// treeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB210200.h"
#include "treeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CtreeCtrl

CtreeCtrl::CtreeCtrl()
{
}

CtreeCtrl::~CtreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CtreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CtreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CtreeCtrl message handlers

void CtreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_CONTROL) 
	{
		UINT	flag{};
		HTREEITEM hItem = HitTest(point, &flag);
		if (hItem)
		{
			UINT	n_state{}, o_state{};
			HTREEITEM o_item{};

			n_state = GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED ? 0 : TVIS_SELECTED;
            		o_item  = GetSelectedItem();
			o_state = o_item ? GetItemState(o_item, TVIS_SELECTED) : 0;
            
			
			if (GetSelectedItem() == hItem)
				SelectItem(NULL);
			CTreeCtrl::OnLButtonDown(nFlags, point);

			SetItemState(hItem, n_state,  TVIS_SELECTED);
			if (o_item && o_item != hItem)
				SetItemState(o_item, o_state, TVIS_SELECTED);
			m_item = NULL;
			return;
		}
	}
	else if (nFlags & MK_SHIFT)
	{
		UINT	flag{};
		HTREEITEM hItem = HitTest(point, &flag);

		if (!m_item)
			m_item = GetSelectedItem();

		if (GetSelectedItem() == hItem)
			SelectItem(NULL);
		CTreeCtrl::OnLButtonDown(nFlags, point);

		if (m_item)
		{
			selectItems(m_item, hItem);
			return;
		}
	}
	else
	{
		clearSelection();
		m_item = NULL;
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CtreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_UP || nChar == VK_DOWN) && (GetKeyState(VK_SHIFT) & 0x8000))
	{
		if (!m_item)
		{
			m_item = GetSelectedItem();
			clearSelection();
		}

		HTREEITEM n_item{};
		HTREEITEM p_item = GetSelectedItem();

		if (nChar == VK_UP)
			n_item = GetPrevVisibleItem(p_item);
		else
			n_item = GetNextVisibleItem(p_item);

		if (n_item)
		{
			const BOOL reselect = !(GetItemState(n_item, TVIS_SELECTED) & TVIS_SELECTED);
			SelectItem(n_item);
			if (reselect)
				SetItemState(p_item, TVIS_SELECTED, TVIS_SELECTED);
		}
		return;
	}
	else if (nChar >= VK_SPACE)
	{
		m_item = NULL;
		clearSelection();
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CtreeCtrl::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
}

HTREEITEM CtreeCtrl::GetPrevItem(HTREEITEM hItem)
{
	HTREEITEM item{};

	if (item = GetPrevSiblingItem(hItem))
		return item;		
	else
		return GetParentItem(hItem);
}

HTREEITEM CtreeCtrl::GetPrevSelectedItem(HTREEITEM hItem)
{
	for (hItem = GetPrevVisibleItem(hItem); hItem!=NULL; hItem = GetPrevVisibleItem(hItem))
		if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
			return hItem;
	return NULL;
}

HTREEITEM CtreeCtrl::GetNextSelectedItem(HTREEITEM hItem)
{
	for (hItem = GetNextVisibleItem(hItem); hItem!=NULL; hItem = GetNextVisibleItem(hItem))
		if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
			return hItem;
	return NULL;
}

HTREEITEM CtreeCtrl::GetFirstSelectedItem()
{
	for (HTREEITEM hItem = GetRootItem(); hItem!=NULL; hItem = GetNextVisibleItem(hItem))
	{
		if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
			return hItem;
	}
	return NULL;
}

void CtreeCtrl::clearSelection()
{
	for (HTREEITEM hItem = GetRootItem(); hItem != NULL; hItem = GetNextVisibleItem(hItem))
	{
		if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
			SetItemState(hItem, 0, TVIS_SELECTED);
	}
}

BOOL CtreeCtrl::selectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo)
{
	HTREEITEM hItem = GetRootItem();

	while (hItem && hItem != hItemFrom && hItem != hItemTo)
	{
		hItem = GetNextVisibleItem(hItem);
		SetItemState(hItem, 0, TVIS_SELECTED);
	}

	if (!hItem)
		return FALSE;

	SelectItem(hItemTo);
	if (hItem == hItemTo)
	{
		hItemTo = hItemFrom;
		hItemFrom = hItem;
	}

	BOOL select = TRUE;
	while (hItem)
	{
		SetItemState(hItem, select ? TVIS_SELECTED : 0, TVIS_SELECTED);
		if (hItem == hItemTo)
			select = FALSE;
		hItem = GetNextVisibleItem(hItem);
	}

	return TRUE;
}