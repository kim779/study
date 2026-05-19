// symbolTree.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "symbolTree.h"

#include "h/mainvar.h"

// CSymbolTree

IMPLEMENT_DYNAMIC(CSymbolTree, CTreeCtrl)

CSymbolTree::CSymbolTree()
{
	m_itemArr.RemoveAll();
	m_firstItem  = NULL;
	m_singleItem = NULL;;
//	m_dragRect.SetRectEmpty();

	m_dragImg = NULL;
}

CSymbolTree::~CSymbolTree()
{
	m_itemArr.RemoveAll();
}


BEGIN_MESSAGE_MAP(CSymbolTree, CTreeCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CSymbolTree::OnTvnBegindrag)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CSymbolTree::OnTvnItemexpanded)
END_MESSAGE_MAP()


// CSymbolTree 메시지 처리기입니다.


int CSymbolTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
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


void CSymbolTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT	flag = 0;
	HTREEITEM hitem;

	m_singleItem = NULL;
	if (nFlags & MK_CONTROL)
	{
		hitem = HitTest(point, &flag);
		if (hitem)
		{
			// toggle selection state
			UINT newSel = GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED ? 0 : TVIS_SELECTED;

			// get old selected item & state
			HTREEITEM olditem = CTreeCtrl::GetSelectedItem();
			UINT oldSel = olditem ? GetItemState(olditem, TVIS_SELECTED) : 0;

			// select new item
			if (CTreeCtrl::GetSelectedItem() == hitem)
				SelectItem(NULL);
		
			CTreeCtrl::OnLButtonDown(nFlags, point);

			// set proper selection state for new item
			SetItemState(hitem, newSel, TVIS_SELECTED);

			// restore state of old selected item
			if (olditem && olditem != hitem)
				SetItemState(olditem, oldSel, TVIS_SELECTED);

			m_firstItem = NULL;
		}
	}
	else if (nFlags & MK_SHIFT)
	{
		// shift key is down
		hitem = HitTest(point, &flag);

		// initialize the reference item if this is the first shift selection
		if (!m_firstItem)
			m_firstItem = CTreeCtrl::GetSelectedItem();

		// select new item
		if (CTreeCtrl::GetSelectedItem() == hitem)
			SelectItem(NULL);

		CTreeCtrl::OnLButtonDown(nFlags, point);

		if (m_firstItem)
			selectItems(m_firstItem, hitem);
	}
	else
	{
		// normal - remove all selection ans let default handler do the rest
		if (GetSelectedItemCount() > 1)
		{
			hitem = HitTest(point, &flag);
			if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			{
				m_singleItem = hitem;
				m_dragRect = CRect(point.x, point.y, point.x, point.y);
				m_dragRect.DeflateRect(-1, -1, 1, 1);
				return;
			}
		}
		clearSelection();
		hitem = HitTest(point, &flag);
		SelectItem(hitem);
		m_firstItem = NULL;
		Invalidate();

		CTreeCtrl::OnLButtonDown(nFlags, point);
	}
}


void CSymbolTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == VK_UP || nChar == VK_DOWN) && GetKeyState(VK_SHIFT) & 0x8000)
	{
		// initialize the reference item if this is the first shift selection
		if (!m_firstItem)
		{
			m_firstItem = CTreeCtrl::GetSelectedItem();
			clearSelection();
		}

		// find which item is currently selected
		HTREEITEM prevItem = CTreeCtrl::GetSelectedItem();
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
		clearSelection();
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSymbolTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_dragImg != NULL)
	{
		m_dragImg->DragLeave(this);
		m_dragImg->EndDrag();
		::ReleaseCapture();

		m_dragImg->DeleteImageList();
		delete m_dragImg;
		m_dragImg = NULL;

		short x = (WORD)point.x;
		short y = (WORD)point.y;
		
		if (x < 0)
			point.x = 0;
		else
			x = 0;
		
		if (y < 0)
			point.y = 0;
		else
			y = 0;
		
		ClientToScreen(&point);
		point.x += x;
		point.y += y;

		AfxGetMainWnd()->PostMessage(WM_USER, MAKEWPARAM(ID_USR_DROP, 0), MAKELPARAM(point.x, point.y));
	}
	if (m_singleItem)
	{
		clearSelection();
		SelectItem(m_singleItem);
	}

	CTreeCtrl::OnLButtonUp(nFlags, point);
}


void CSymbolTree::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_dragImg != NULL)
	{
		CPoint	pt = point;

		ClientToScreen(&pt);
		m_dragImg->DragMove(pt);
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}


void CSymbolTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	if (GetSelectedItemCount() <= 0)
		return;

	CPoint	pt = pNMTreeView->ptDrag;
	m_dragImg = createDragImage(pt);
	if (m_dragImg == (CImageList *)NULL)
		return;

	CPoint	startpt = pNMTreeView->ptDrag;
	startpt -= pt;

	m_dragImg->BeginDrag(0, CPoint(0, 0));
	m_dragImg->DragEnter(GetDesktopWindow(), startpt);
	SetCapture();
}


void CSymbolTree::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	SetItemImage((HTREEITEM)pNMTreeView->itemNew.hItem, (int)(pNMTreeView->action - 1), (int)(pNMTreeView->action - 1));	
	*pResult = 0;
}

HTREEITEM CSymbolTree::GetSelectedItem(int idx)
{
	if (idx < 0 || idx >= m_itemArr.GetCount())
		return NULL;

	return m_itemArr.GetAt(idx);
}

int CSymbolTree::GetSelectedItemCount()
{
	HTREEITEM hitem;

	m_itemArr.RemoveAll();
	hitem = getFirstSelectedItem();
	if (hitem == (HTREEITEM) NULL)
		return 0;

	if (!ItemHasChildren(hitem))
		m_itemArr.Add(hitem);
	for (hitem = getNextSelectedItem(hitem); hitem != NULL; hitem = getNextSelectedItem(hitem))
	{
		if (hitem == (HTREEITEM) NULL)
			break;

		if (ItemHasChildren(hitem))
			continue;

		m_itemArr.Add(hitem);
	}

	return (int)m_itemArr.GetSize();
}

void CSymbolTree::clearSelection()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			SetItemState(hitem, 0, TVIS_SELECTED);
	}
	SelectItem(NULL);
}

bool CSymbolTree::selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO)
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

HTREEITEM CSymbolTree::getFirstSelectedItem()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

HTREEITEM CSymbolTree::getNextSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetNextVisibleItem(hitem); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

CImageList* CSymbolTree::createDragImage(CPoint& point)
{
	CRect	singleRc, completeRc = CRect(0, 0, 0, 0);
	
	HTREEITEM hitem = getFirstSelectedItem();
	
	while (hitem)
	{
		if (ItemHasChildren(hitem))
		{
			hitem = getNextSelectedItem(hitem);
			continue;
		}

		GetItemRect(hitem, &singleRc, TRUE);
		singleRc.InflateRect(0, 0, 25, 0);
		if (completeRc.IsRectEmpty())
			completeRc = singleRc;
		else
		{
			completeRc.bottom += singleRc.Height();
			completeRc.right = max(singleRc.right, completeRc.right);
		}

		hitem = getNextSelectedItem(hitem);
	}

	CClientDC dcClient(this);
	CDC	memDC;
	CBitmap bitmap;
	int	nCnt = 0;

	if (!memDC.CreateCompatibleDC(&dcClient))
		return NULL;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, completeRc.Width(), completeRc.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
 	memDC.FillSolidRect(0, 0, completeRc.Width(), completeRc.Height(), RGB(255, 255,255));

	hitem = getFirstSelectedItem();

	while (hitem)
	{
		if (ItemHasChildren(hitem))
		{
			hitem = getNextSelectedItem(hitem);
			continue;
		}

		CImageList* pSingleImageList = CreateDragImage(hitem);
		if (pSingleImageList)
		{
			GetItemRect(hitem, &singleRc, TRUE);
			pSingleImageList->Draw(&memDC, 0, CPoint(0, singleRc.Height() * nCnt), ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
			nCnt++;
		}
		hitem = getNextSelectedItem(hitem);
	}

	memDC.SelectObject(pOldMemDCBitmap);
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(completeRc.Width(), completeRc.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&bitmap, RGB(0, 255, 0)); 
	bitmap.DeleteObject();

	point.x = completeRc.left;
	point.y = completeRc.top;

	return pCompleteImageList;
}
