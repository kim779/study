// ServerTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../mapvar.h"
#include "../mainvar.h"
#include "../axiswork.h"
#include "ServerTreeCtrl.h"
#include "../awWcc/libwcc.h"
#include "../../h/axisrsm.h"
#include "../awSock/wSock.h"
#include "../Components/Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int CALLBACK mapSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the tree control.
	// The lParam of an item is just its handle.
	CServerTreeCtrl* pmyTreeCtrl = (CServerTreeCtrl*) lParamSort;
	CString    strItem1 = pmyTreeCtrl->m_treeArr.GetAt(lParam1).mapn;
	CString    strItem2 = pmyTreeCtrl->m_treeArr.GetAt(lParam2).mapn;
	
	if (strItem1.GetLength() >= 8 && strItem2.GetLength() >= 8)
	{
		strItem1 = strItem1.Mid(0, 8);
		strItem2 = strItem2.Mid(0, 8);
		return strcmp(strItem1.Mid(2), strItem2.Mid(2));
	}
	
	return strcmp(strItem2, strItem1);
}



/////////////////////////////////////////////////////////////////////////////
// CServerTreeCtrl

CServerTreeCtrl::CServerTreeCtrl()
{
	m_bOpen = false;
	m_pilDrag = NULL;
	m_arItem.RemoveAll();
	m_dropPos = CPoint(0, 0);

	m_pchTip = NULL;
	m_pwchTip = NULL;

	m_nTwoDepth = 3;

	m_NoDropCopyCursor = AfxGetApp()->LoadCursor(IDR_TREEFILECTRL_NO_DROPCOPY);
	VERIFY(m_NoDropCopyCursor);
	m_DropCopyCursor = AfxGetApp()->LoadCursor(IDR_TREEFILECTRL_DROPCOPY);
	VERIFY(m_DropCopyCursor);
	m_NoDropMoveCursor = AfxGetApp()->LoadCursor(IDR_TREEFILECTRL_NO_DROPMOVE);
	VERIFY(m_NoDropMoveCursor);
	m_DropMoveCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	VERIFY(m_DropMoveCursor);
}

CServerTreeCtrl::~CServerTreeCtrl()
{
	if(m_pwchTip != NULL)
		delete m_pwchTip;
	
	if(m_pchTip != NULL)
		delete m_pchTip;

	m_treeArr.RemoveAll();
	m_files.RemoveAll();
	m_types.RemoveAll();
}


BEGIN_MESSAGE_MAP(CServerTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CServerTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_ACTIVATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_REFLECT_EX(TVN_BEGINDRAG, OnBeginDrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerTreeCtrl message handlers

void CServerTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_files.RemoveAll();
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
			ClearSelection();
			SetTimer(1, 300, NULL);
		}
		m_firstItem = NULL;
	}

	for (int ii = 0; ii < GetSelectedItemCount(); ii++)
	{
		m_type = GetItemData(m_arItem.GetAt(ii));
		m_name = GetItemText(m_arItem.GetAt(ii));
		if (m_name.GetLength() > 8)
			m_name = m_name.Mid(0, 8);
		AddFiles(m_name);
	}
	if (!(nFlags & MK_CONTROL) && !(nFlags & MK_SHIFT))
		CTreeCtrl::OnLButtonDown(nFlags, point);
	//GetParent()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
}

void CServerTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

int CServerTreeCtrl::GetSelectedItemCount()
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

void CServerTreeCtrl::ClearSelection()
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

bool CServerTreeCtrl::selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO)
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

HTREEITEM CServerTreeCtrl::getFirstSelectedItem()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

HTREEITEM CServerTreeCtrl::getNextSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetNextVisibleItem(hitem); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

HTREEITEM CServerTreeCtrl::getPrevSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetPrevVisibleItem(hitem); hitem != NULL; hitem = GetPrevVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

void CServerTreeCtrl::Initialize(CString dir,  CString path, int child, bool request, bool bTemplate)
{
	m_count   = 0;
	m_index   = 0;
	m_dir     = dir;
	m_child   = child;
	m_state   = stIDLE;
	m_enable  = true;
	m_finish  = false;
	m_request = request;
	m_bTemplate = bTemplate;

	m_files.RemoveAll();
	m_types.RemoveAll();

	m_path = _T("");
	m_type = R_NONE;
	m_item = (HTREEITEM) NULL;

	CImageList	imglist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_SERVERTREE);
	imglist.Create(16, 16, ILC_COLOR24|ILC_MASK, 5, 1);
	imglist.Add(&bitmap, RGB(0, 255, 255));
	imglist.SetBkColor(CLR_NONE);
	SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();
}

void CServerTreeCtrl::removeAll()
{
	m_treeArr.RemoveAll();
	DeleteAllItems();
	m_files.RemoveAll();
	m_types.RemoveAll();
}

HTREEITEM CServerTreeCtrl::getSelectedItem(int index)
{
	if (m_arItem.GetSize() <= 0 || index > m_arItem.GetUpperBound())
		return (HTREEITEM) NULL;

	HTREEITEM hitem = m_arItem.GetAt(index);
	int	type = GetItemData(hitem);
	if (m_arItem.GetUpperBound() > 1 && type == R_NONE)
		return (HTREEITEM) NULL;

	return hitem;
}

void CServerTreeCtrl::insert(CString mapName, CString mapDesc, int idx)
{
	HTREEITEM	hitem1 = NULL, hitem2 = NULL;
	void *pVoid;
	if (!m_itemArr1.Lookup(mapName.Mid(0, 2), pVoid))
	{
		hitem1 = InsertItem(mapName.Mid(0, 2), 0, 0);
		SetItemData(hitem1, R_NONE);
		m_itemArr1.SetAt(mapName.Mid(0, 2), hitem1);
	}
	else
		hitem1 = (HTREEITEM)pVoid;

	if (!m_itemArr2.Lookup(mapName.Mid(0, m_nTwoDepth), pVoid))
	{
		hitem2 = InsertItem(mapName.Mid(0, m_nTwoDepth), 0, 0, hitem1);
		SetItemData(hitem2, R_NONE);
		m_itemArr2.SetAt(mapName.Mid(0, m_nTwoDepth), hitem2);
	}
	else
		hitem2 = (HTREEITEM)pVoid;
	hitem1 = InsertItem(mapName + " " + mapDesc, 2, 2, hitem2);
	SetItemData(hitem1, idx);
}

void CServerTreeCtrl::loadFile()
{
	CString	dir;
	
	if (m_bTemplate)
		dir = m_dir + "\\tmplres.ini";
	else
		dir = m_dir + "\\" + DN_FILE;

	struct _mapTree	mapTree;
	m_treeArr.RemoveAll();
	
	TRY
	{
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&mapTree, sizeof(mapTree));
			if (nBytesRead == sizeof(mapTree))
				m_treeArr.Add(mapTree);
		}while((int)nBytesRead);
		file.Close();
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	InitTree();
}

bool CServerTreeCtrl::openList(CComboBox *CBO)
{
	m_pCombo = CBO;
	removeAll();

	CString tmps;
	
	m_section.Lock();

	m_pDownLoad = new CDownLoad();
	if (m_bTemplate)
		wccSetTempPath(tmps, "tmplres.ini");
	else
		wccSetTempPath(tmps, DN_FILE);
	if (!m_pDownLoad->downLoad("", tmps, (m_bTemplate?3:2)))
	{
		delete m_pDownLoad;
		m_section.Unlock();
		return false;
	}
	delete m_pDownLoad;

	m_section.Unlock();

	loadFile();
	return true;
}

void CServerTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (pNMTreeView->itemNew.hItem == (HTREEITEM) NULL)
		return;

	m_type = GetItemData(pNMTreeView->itemNew.hItem);

	if (m_type != R_NONE)
	{
		m_name = GetItemText(pNMTreeView->itemNew.hItem);
		if (m_name.GetLength() > 8)
			m_name = m_name.Mid(0, 8);
	}

	if (GetSelectedItemCount() == 1 && !m_name.IsEmpty())
	{
		m_files.RemoveAll();
		AddFiles(m_name);
	}
	
	if (GetSelectedItemCount() >= 1 && !m_name.IsEmpty())
		//GetParent()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
		SetTimer(1, 300, NULL);
	*pResult = 0;
}

CString CServerTreeCtrl::setMapPath(CString mapN)
{
	if (mapN.GetLength() != 8)
		return "";
	CString tmp;
	if (m_bTemplate)
		tmp.Format("template/%s/%s/%s", mapN.Left(2), mapN.Left(3), mapN);
	else
		tmp.Format("map/%s/%s/%s", mapN.Left(2), mapN.Left(3), mapN);
	return tmp;
}

HTREEITEM CServerTreeCtrl::GetItemByName(HTREEITEM hItem, LPCTSTR szItemName)
{
	// If hItem is NULL, start search from root item.
	if (hItem == NULL)
		hItem = (HTREEITEM)SendMessage(TVM_GETNEXTITEM,
		TVGN_ROOT, 0);
	while (hItem != NULL)
	{
		char szBuffer[8+1];
		TV_ITEM item;
		
		item.hItem = hItem;
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = szBuffer;
		item.cchTextMax = 8;
		SendMessage(TVM_GETITEM, 0, (LPARAM)&item);
		
		// Did we find it?
		if (lstrcmp(szBuffer, szItemName) == 0)
			return hItem;
		
		// Check whether we have child items.
		if (item.cChildren)
		{
			// Recursively traverse child items.
			HTREEITEM hItemFound, hItemChild;
			
			hItemChild = (HTREEITEM)SendMessage(TVM_GETNEXTITEM,
				TVGN_CHILD, (LPARAM)hItem);
			hItemFound = GetItemByName(hItemChild, szItemName);
			
			// Did we find it?
			if (hItemFound != NULL)
				return hItemFound;
		}
		
		// Go to next sibling item.
		hItem = (HTREEITEM)SendMessage(TVM_GETNEXTITEM,
			TVGN_NEXT, (LPARAM)hItem);
	}
	
	// Not found.
	return NULL;
} 

BOOL CServerTreeCtrl::OnBeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	m_pilDrag = CreateDragImage(pNMTreeView->itemNew.hItem);
	if (!m_pilDrag)
		return TRUE; //Allow the message to be reflected again
	
	m_hItemDrag = pNMTreeView->itemNew.hItem;
	m_hItemDrop = NULL;

	// Calculate the offset to the hotspot
	CPoint offsetPt(8,8);   // Initialize a default offset
	
	CPoint dragPt = pNMTreeView->ptDrag;    // Get the Drag point
	UINT nHitFlags = 0;
	HTREEITEM htiHit = HitTest(dragPt, &nHitFlags);
	if (htiHit != NULL)
	{
		// The drag point has Hit an item in the tree
		CRect itemRect;
		if (GetItemRect(htiHit, &itemRect, FALSE))
		{
			// Count indent levels
			HTREEITEM htiParent = htiHit;
			int nIndentCnt = 0;
			while (htiParent != NULL)
			{
				htiParent = GetParentItem(htiParent);
				nIndentCnt++;
			}
			
			if (!(GetStyle() & TVS_LINESATROOT)) 
				nIndentCnt--;
			
			// Calculate the new offset
			offsetPt.y = dragPt.y - itemRect.top;
			offsetPt.x = dragPt.x - (nIndentCnt * GetIndent()) + GetScrollPos(SB_HORZ);
		}
	}
	
	//Begin the dragging  
	m_pilDrag->BeginDrag(0, offsetPt);
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	m_pilDrag->DragEnter(NULL, pt);
	SetCapture();
	SelectItem(m_hItemDrag);

	return TRUE; //Allow the message to be reflected again
}

void CServerTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_pilDrag != NULL)
	{
		CRect clientRect;
		GetClientRect(&clientRect);
		
		//Draw the drag
		POINT pt = point;
		ClientToScreen(&pt);
		CImageList::DragMove(pt);

		SelectDropTarget(m_hItemDrop);
		HTREEITEM hItem = m_hItemDrop;

		if (m_bTemplate || AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISDROPAREA, MAKELPARAM(pt.x, pt.y)))
			SetCursor(m_DropCopyCursor);
		else
		{
			//Change the cursor to give feedback
			if (hItem)
			{
				if ((GetKeyState(VK_CONTROL) & 0x8000))
					SetCursor(m_DropCopyCursor);
				else
					SetCursor(m_DropMoveCursor);	
			}
			else
			{
				if ((GetKeyState(VK_CONTROL) & 0x8000))
					SetCursor(m_NoDropCopyCursor);
				else
					SetCursor(m_NoDropMoveCursor);
			}
		}
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CServerTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_pilDrag != NULL)
	{
		POINT pt = point;
		ClientToScreen(&pt);

		CImageList::DragLeave(this);
		CImageList::EndDrag();

		ReleaseCapture();

		//Delete the drag image list
		m_pilDrag->DeleteImageList();
		delete m_pilDrag;
		m_pilDrag = NULL;

		if (m_bTemplate)
		{
			m_dropPos = point;
			loadTemplate(pt);
		}
		else if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISDROPAREA, MAKELPARAM(pt.x, pt.y)))
		{
			m_count = GetSelectedItemCount();
			//m_fnames.GetWindowText(m_name);
			//sendData(false);
			OpenMap();
		}
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CServerTreeCtrl::fillCombo(CComboBox *CBO)
{
	if (!CBO || !IsWindow(CBO->m_hWnd))
		return;
	CBO->ResetContent();
	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
	{
		CString sGroupName = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETGROUP, atoi(m_treeArr.GetAt(ii).mgub));
		if (CBO->FindString(0, sGroupName) < 0)
			CBO->SetItemData(CBO->AddString(sGroupName), atoi(m_treeArr.GetAt(ii).mgub));
	}
	if (CBO->GetCount())
		CBO->SetCurSel(0);

	if (m_bTemplate)
		GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_WRK_GROUPCBOTMP, CBN_SELENDOK), (long)this->GetSafeHwnd());
	else
		GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_WRK_GROUPCBOSVR, CBN_SELENDOK), (long)this->GetSafeHwnd());
}

void CServerTreeCtrl::fillList(CToolTipListCtrl *LC, int idx)
{
	LC->DeleteAllItems();

	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
	{
		if (idx == -1 || atoi(m_treeArr.GetAt(ii).mgub) == idx)
		{
			addItem(LC, ii, 0, m_treeArr.GetAt(ii).mapd, ii, m_treeArr.GetAt(ii).mapn);
		}
	}
}

BOOL CServerTreeCtrl::addItem(CToolTipListCtrl *LC, int item, int subItem, CString str, int data, CString tipText)
{
	BOOL ret;
	LV_ITEM lvi;

        lvi.mask     = LVIF_TEXT | LVIF_PARAM;
        lvi.iItem    = item;
        lvi.iSubItem = subItem;
	lvi.lParam   = data;
        lvi.pszText  = (LPTSTR) str.operator LPCTSTR();

        if (subItem == 0)
                ret = LC->InsertItem(&lvi);
	else
		ret = LC->SetItem(&lvi);
	LC->SetItemToolTipText(item, subItem, tipText);
	return ret;
}

void CServerTreeCtrl::setFileName(int idx)
{
	m_name = m_treeArr.GetAt(idx).mapn;
	AddFiles(m_name);
}

void CServerTreeCtrl::OpenMap(int idx)
{
	AfxGetApp()->DoWaitCursor(1);
	m_bOpen = true;
	if (idx >= 0)
	{
		m_name = m_treeArr.GetAt(idx).mapn;
		m_files.RemoveAll();
	}
	AddFiles(m_name);

	CString tmps, fullName;
	for (int ii = 0; ii < m_files.GetSize(); ii++)
	{
		m_section.Lock();

		m_pDownLoad = new CDownLoad();
		wccSetTempPath(tmps, (LPSTR)m_files.GetAt(ii).operator LPCTSTR());
		fullName = setMapPath(m_files.GetAt(ii));
		
		if (!m_pDownLoad->downLoad(fullName, tmps, (m_bTemplate?1:0)))
		{
			delete m_pDownLoad;
			m_section.Unlock();
			m_bOpen = false;
			AfxGetApp()->DoWaitCursor(0);
			return;
		}
		delete m_pDownLoad;
		m_section.Unlock();
	}
	if (m_files.GetSize())
	{
		if (m_bTemplate)
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, 1);
		else
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, 0);
	}
	m_bOpen = false;
	AfxGetApp()->DoWaitCursor(0);
}

void CServerTreeCtrl::OpenMap(CString mapName)
{
	AfxGetApp()->DoWaitCursor(1);
	m_bOpen = true;
	
	CString tmps, fullName;
	m_section.Lock();
	if (mapName.GetLength() > 8)
		mapName = mapName.Mid(0, 8);
	
	m_pDownLoad = new CDownLoad();
	wccSetTempPath(tmps, (LPSTR)mapName.operator LPCTSTR());
	fullName = setMapPath(mapName);
	m_files.RemoveAll();
	AddFiles(mapName);
	
	m_pDownLoad->downLoad(fullName, tmps, (m_bTemplate?1:0));
	delete m_pDownLoad;
	
	m_section.Unlock();
	if (mapName.GetLength() == 8)
	{
		if (m_bTemplate)
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, 1);
		else
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, 0);
	}
	m_bOpen = false;
	AfxGetApp()->DoWaitCursor(-1);
}

void CServerTreeCtrl::AddFiles(CString name)
{
	if (name.IsEmpty())
		return;

	for (int ii = 0; ii < m_files.GetSize(); ii++)
		if (!m_files.GetAt(ii).Compare(name))
			return;
	m_files.Add(name);
}

void CServerTreeCtrl::loadTemplate(CPoint pnt)
{
	CString tmps;
	m_MousePoint = pnt;
	
	m_section.Lock();
	
	m_pDownLoad = new CDownLoad();
	wccSetTempPath(tmps, "template");

	m_pDownLoad->downLoad(setMapPath(m_name), tmps, 1);
	delete m_pDownLoad;
	
	m_section.Unlock();

	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_LOADTEMPLATE, MAKELPARAM(m_MousePoint.x, m_MousePoint.y));
}

CString CServerTreeCtrl::getNameOfIdx(int idx)
{
	return setMapPath(m_treeArr.GetAt(idx).mapn);
}

void CServerTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
	EnableToolTips(TRUE);
}

int CServerTreeCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	RECT rect;
	
	UINT nFlags;
	HTREEITEM hitem = HitTest( point, &nFlags );
	if( nFlags & TVHT_ONITEM)
	{
		GetItemRect( hitem, &rect, TRUE );
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT)hitem;
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rect;
		return pTI->uId;
	}
	
	return -1;
}


BOOL CServerTreeCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText = "";
	UINT nID = pNMHDR->idFrom;
	
	// Do not process the message from built in tooltip 
	if( nID == (UINT)m_hWnd &&
		(( pNMHDR->code == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND ) ||
		( pNMHDR->code == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND ) ) )
		return FALSE;
	
	// Get the mouse position
	const MSG* pMessage;
	CPoint pt;
	pMessage = GetCurrentMessage();
	ASSERT ( pMessage );
	pt = pMessage->pt;
	ScreenToClient( &pt );
	
	UINT nFlags;
	HTREEITEM hitem = HitTest( pt, &nFlags );
	if( nFlags & TVHT_ONITEM)
	{
		int idx = GetItemData((HTREEITEM)nID);
		if (idx != R_NONE)
			strTipText = m_treeArr.GetAt(idx).mapd;
	}
	
#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip != NULL)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()+1];
		lstrcpyn(m_pchTip, strTipText, strTipText.GetLength());
		m_pchTip[strTipText.GetLength()] = 0;
		pTTTW->lpszText = (WCHAR*)m_pchTip;
	}
	else
	{
		if(m_pwchTip != NULL)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()+1];
		_mbstowcsz(m_pwchTip, strTipText, strTipText.GetLength());
		m_pwchTip[strTipText.GetLength()] = 0; // end of text
		pTTTW->lpszText = (WCHAR*)m_pwchTip;
	}
#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip != NULL)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()+1];
		_wcstombsz(m_pchTip, strTipText, strTipText.GetLength());
		m_pchTip[strTipText.GetLength()] = 0; // end of text
		pTTTA->lpszText = (LPTSTR)m_pchTip;
	}
	else
	{
		if(m_pwchTip != NULL)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()+1];
		lstrcpyn(m_pwchTip, strTipText, strTipText.GetLength());
		m_pwchTip[strTipText.GetLength()] = 0;
		pTTTA->lpszText = (LPTSTR) m_pwchTip;
	}
#endif
	
	*pResult = 0;
	
	return TRUE;    // message was handled
}

void CServerTreeCtrl::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case 1:
		KillTimer(nIDEvent);
		GetParent()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
		break;
	}
	CTreeCtrl::OnTimer(nIDEvent);
}

void CServerTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	*pResult = 0;
}

void CServerTreeCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CTreeCtrl::OnActivate(nState, pWndOther, bMinimized);
	
	
}


void CServerTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	SetItemImage((HTREEITEM)pNMTreeView->itemNew.hItem, (int)(pNMTreeView->action - 1), (int)(pNMTreeView->action - 1));
	*pResult = 0;
}



void CServerTreeCtrl::InitTree()
{

	DeleteAllItems();	
	m_itemArr1.RemoveAll();
	m_itemArr2.RemoveAll();
	bool bSplash = CSplashWnd::IsCreated();
	if (!bSplash)
		CSplashWnd::ShowSplashScreen(AfxGetMainWnd());
	
	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
	{
		if (m_treeArr.GetSize() == 1)
			CSplashWnd::NextScreen(100);
		else
			CSplashWnd::NextScreen((ii * 100) / (m_treeArr.GetSize() - 1));
		insert(m_treeArr.GetAt(ii).mapn, m_treeArr.GetAt(ii).mapd, ii);
	}

	POSITION pos;
	CString sKey;
	SortChildren(NULL);
	
	for(pos = m_itemArr1.GetStartPosition(); pos != NULL; )
	{
		void* pVoid;
		m_itemArr1.GetNextAssoc(pos, sKey, pVoid);
		SortChildren((HTREEITEM)pVoid);
	}

	
	for(pos = m_itemArr2.GetStartPosition(); pos != NULL; )
	{
		void* pVoid;
		m_itemArr2.GetNextAssoc(pos, sKey, pVoid);
		TVSORTCB tvs;
		tvs.hParent = (HTREEITEM)pVoid;
		tvs.lpfnCompare = mapSortProc;
		tvs.lParam = (LPARAM) this;
		SortChildrenCB(&tvs);
	}
	
	CSplashWnd::NextScreen(100);
	CSplashWnd::NextScreen(-1);
	if (!bSplash)
		CSplashWnd::HideSplashScreen();
	m_itemArr1.RemoveAll();
	m_itemArr2.RemoveAll();
	
	fillCombo(m_pCombo);
}

