// xTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "xTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTreeCtrl

CXTreeCtrl::CXTreeCtrl()
{
	m_arItem.RemoveAll();
}

CXTreeCtrl::~CXTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CXTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CXTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTreeCtrl message handlers

BOOL CXTreeCtrl::Attach(const UINT nID, CWnd* parent)
{
	if (!SubclassDlgItem(nID, parent))
		return FALSE;

	return TRUE;
}

void CXTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
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
			return;
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
			return;
		}
	}
	else
	{
		// normal - remove all selection ans let default handler do the rest
		ClearSelection();
		m_firstItem = NULL;
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CXTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

int CXTreeCtrl::GetSelectedItemCount()
{
	m_arItem.RemoveAll();
	HTREEITEM hitem = getFirstSelectedItem();
	if (hitem == (HTREEITEM) NULL)
		return 0;

	if (GetItemData(hitem) != (int)R_NONE)
		m_arItem.Add(hitem);
	for (hitem = getNextSelectedItem(hitem); hitem != NULL; hitem = getNextSelectedItem(hitem))
	{
		if (hitem == (HTREEITEM) NULL)
			break;

		if (GetItemData(hitem) == (int)R_NONE)
			continue;

		m_arItem.Add(hitem);
	}

	return m_arItem.GetSize();
}

void CXTreeCtrl::ClearSelection()
{
	// This can be time consuming for very large trees
	// and is called every time the user does a normal selection
	// If performance is an issue, it may be better to maintain
	// a list of selected items
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			SetItemState(hitem, 0, TVIS_SELECTED);
	}
	SelectItem(NULL);
}

bool CXTreeCtrl::selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO)
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

HTREEITEM CXTreeCtrl::getFirstSelectedItem()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

HTREEITEM CXTreeCtrl::getNextSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetNextVisibleItem(hitem); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

HTREEITEM CXTreeCtrl::getPrevSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetPrevVisibleItem(hitem); hitem != NULL; hitem = GetPrevVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}
