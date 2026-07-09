// MultiSelTree.cpp : implementation file
//

#include "stdafx.h"
#include "..\axiswork.h"
#include "MultiSelTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSelTree

CMultiSelTree::CMultiSelTree()
{
	m_firstItem = NULL;
}

CMultiSelTree::~CMultiSelTree()
{
}


BEGIN_MESSAGE_MAP(CMultiSelTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CMultiSelTree)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSelTree message handlers

void CMultiSelTree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT	flag;
	HTREEITEM hitem = HitTest(point, &flag);

	if (nFlags & MK_CONTROL)
	{
		if (hitem)
		{
			UINT newSel = GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED ? 0 : TVIS_SELECTED;

			HTREEITEM olditem = GetSelectedItem();
			UINT oldSel = olditem ? GetItemState(olditem, TVIS_SELECTED) : 0;

			if (GetSelectedItem() == hitem)
				SelectItem(NULL);
		
			CTreeCtrl::OnLButtonDown(nFlags, point);

			SetItemState(hitem, newSel, TVIS_SELECTED);

			if (olditem && olditem != hitem)
				SetItemState(olditem, oldSel, TVIS_SELECTED);

			m_firstItem = NULL;
		}
	}
	else if (nFlags & MK_SHIFT)
	{
		if (!m_firstItem)
			m_firstItem = GetSelectedItem();

		if (GetSelectedItem() == hitem)
			SelectItem(NULL);

		CTreeCtrl::OnLButtonDown(nFlags, point);

		if (m_firstItem)
			selectItems(m_firstItem, hitem);
	}
	else
	{
		m_firstItem = NULL;
		ClearSelection();
		SelectItem(hitem);
		CTreeCtrl::OnLButtonDown(nFlags, point);
	}
	GetOwner()->SendMessage(WM_USER + 10, 0, (LPARAM)hitem);
	Invalidate();
}

void CMultiSelTree::OnLButtonUp(UINT nFlags, CPoint point) 
{

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CMultiSelTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_UP || nChar == VK_DOWN) && GetKeyState(VK_SHIFT) & 0x8000)
	{
		if (!m_firstItem)
		{
			m_firstItem = GetSelectedItem();
			ClearSelection();
		}

		HTREEITEM prevItem = GetSelectedItem();
		HTREEITEM nextItem;
		if (nChar == VK_UP)
			nextItem = GetPrevVisibleItem(prevItem);
		else
			nextItem = GetNextVisibleItem(prevItem);

		if (nextItem)
		{
			BOOL reselected = !(GetItemState(nextItem, TVIS_SELECTED) & TVIS_SELECTED);

			SelectItem(nextItem);

			if (reselected)
				SetItemState(prevItem, TVIS_SELECTED, TVIS_SELECTED);
			GetOwner()->SendMessage(WM_USER + 10, 0, (LPARAM)nextItem);
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

void CMultiSelTree::ClearSelection()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			SetItemState(hitem, 0, TVIS_SELECTED);
	}
	SelectItem(NULL);
}

bool CMultiSelTree::selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO)
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

	if (hitem == hitemTO)
	{
		hitemTO   = hitemFROM;
		hitemFROM = hitem;
	}

	BOOL	select = TRUE;
	while (hitem)
	{
		SetItemState(hitem, (select ? TVIS_SELECTED : 0), TVIS_SELECTED);

		if (hitem == hitemTO)
			select = FALSE;

		hitem = GetNextVisibleItem(hitem);
	}

	return true;
}

int CMultiSelTree::GetSelectedItemCount()
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

HTREEITEM CMultiSelTree::getFirstSelectedItem()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

HTREEITEM CMultiSelTree::getNextSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetNextVisibleItem(hitem); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

void CMultiSelTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UINT	flag;
	HTREEITEM hitem = HitTest(point, &flag);
	if (hitem)
	{
		if (!(GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED))
		{
			m_firstItem = NULL;
			ClearSelection();
			SelectItem(hitem);
		}

		GetOwner()->SendMessage(WM_USER + 10, 0, (LPARAM)hitem);

		GetCursorPos(&point);
		if (GetSelectedItemCount() > 1)
		{
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, 1, "Delete Controls");
			switch(menu.TrackPopupMenu(TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_LEFTALIGN,
				point.x, point.y, this))
			{
			case 1:
				GetOwner()->SendMessage(WM_USER + 10, 1, 1);	// Delete Forms
				break;
			}
		}
		else
		{
			CMenu menu;
			menu.CreatePopupMenu();
			if (GetParentItem(hitem))
				menu.AppendMenu(MF_STRING, 1, "Delete Control");
			else
				menu.AppendMenu(MF_STRING, 1, "Delete Map");
			switch(menu.TrackPopupMenu(TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_LEFTALIGN,
				point.x, point.y, this))
			{
			case 1:
				if (GetParentItem(hitem))
					GetOwner()->SendMessage(WM_USER + 10, 1, 1);	// Delete Forms
				else
					AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_APP_MAPCLOSE);
				break;
			}
		}
	}
	return;
	
	CTreeCtrl::OnRButtonDown(nFlags, point);
}
