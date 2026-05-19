
#include "stdafx.h"
#include "ViewTree.h"
#include "resource.h"
#include "splashWnd.h"

#include "h/mainvar.h"
#include "../h/axisvar.h"
#include "awWcc/libWcc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	R_NONE		-1

////////////////////////////////////////////////////////////////////////////
// CViewTree

static int CALLBACK mapSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the tree control.
	// The lParam of an item is just its handle.
	CString    strItem1;
	CString    strItem2;
	CViewTree* pTree = (CViewTree*) lParamSort;
	if (pTree->m_kind == viewMAP)
	{
		strItem1 = pTree->m_treeArr.GetAt(lParam1).mapn;
		strItem2 = pTree->m_treeArr.GetAt(lParam2).mapn;
	}
	else
	{
		strItem1 = pTree->m_trxArr.GetAt(lParam1).name;
		strItem2 = pTree->m_trxArr.GetAt(lParam2).name;
	}
	
	if (strItem1.GetLength() >= L_MAPN && strItem2.GetLength() >= L_MAPN)
	{
		strItem1 = strItem1.Left(L_MAPN);
		strItem2 = strItem2.Left(L_MAPN);
		return strcmp(strItem1.Mid(L_SGID), strItem2.Mid(L_SGID));
	}
	return strcmp(strItem2, strItem1);
}

CViewTree::CViewTree()
{
	m_depth    = L_TGID;	// 3
	m_dir      = _T("");
	m_name     = _T("");
	m_open     = false;
	m_template = false;

	m_pilDrag   = NULL;
	m_firstItem = NULL;
	m_dragItem  = NULL;
	m_dropItem  = NULL;

	m_treeArr.RemoveAll();
	m_trxArr.RemoveAll();
	m_itemArr.RemoveAll();
	m_files.RemoveAll();
	m_itemMap1.RemoveAll();
	m_itemMap2.RemoveAll();

	m_cNoDropCopy = AfxGetApp()->LoadCursor(IDC_NODROPCOPY);
	VERIFY(m_cNoDropCopy);
	m_cDropCopy = AfxGetApp()->LoadCursor(IDC_DROPCOPY);
	VERIFY(m_cDropCopy);
	m_cNoDropMove = AfxGetApp()->LoadCursor(IDC_NODROPMOVE);
	VERIFY(m_cNoDropMove);
	m_cDropMove = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	VERIFY(m_cDropMove);

	m_kind = viewMAP;
}

CViewTree::~CViewTree()
{
	m_treeArr.RemoveAll();
	m_trxArr.RemoveAll();
	m_itemArr.RemoveAll();
	m_files.RemoveAll();
	m_itemMap1.RemoveAll();
	m_itemMap2.RemoveAll();
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CViewTree::OnTvnItemexpanded)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CViewTree::OnTvnBegindrag)
ON_NOTIFY_REFLECT(NM_RCLICK, &CViewTree::OnNMRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 메시지 처리기

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


BOOL CViewTree::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CTreeCtrl::Create(dwStyle, rect, pParentWnd, nID);
}


void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	int	count = GetSelectedItemCount();

	if (count != 0)
	{
		OpenMap(-1, (int)'R');
		m_files.RemoveAll();
	}
	*pResult = 0;
}


void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	if (pNMTreeView->itemNew.hItem == (HTREEITEM) NULL)
		return;

	int	type;

	type = (int)GetItemData(pNMTreeView->itemNew.hItem);
	if (type != R_NONE)
	{
		m_name = GetItemText(pNMTreeView->itemNew.hItem);
		if (m_name.GetLength() > 8)
			m_name = m_name.Mid(0, 8);
	}

	if (GetSelectedItemCount() == 1 && !m_name.IsEmpty())
	{
		m_files.RemoveAll();
		addFiles(m_name);
	}
	
	if (GetSelectedItemCount() >= 1 && !m_name.IsEmpty())
		SetTimer(1, 300, NULL);

	*pResult = 0;
}


void CViewTree::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	SetItemImage(pNMTreeView->itemNew.hItem, (int)(pNMTreeView->action - 1), (int)(pNMTreeView->action - 1));
	*pResult = 0;
}


void CViewTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == VK_UP || nChar == VK_DOWN) && GetKeyState(VK_SHIFT) & 0x8000)
	{
		if (!m_firstItem)	// initialize the reference item if this is the first shift selection
		{
			m_firstItem = GetSelectedItem();
			clearSelection();
		}

		// find which item is currently selected
		HTREEITEM nextItem, prevItem = GetSelectedItem();
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


void CViewTree::OnLButtonDown(UINT nFlags, CPoint point)
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
			UINT	oldSel = olditem ? GetItemState(olditem, TVIS_SELECTED) : 0;

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
		}
	}
	else if (nFlags & MK_SHIFT)
	{
		// shift key is down
		UINT	flag;
		HTREEITEM hitem = HitTest(point, &flag);
		
		// initialize the reference item if this is the first shift selection
		if (!m_firstItem)
			m_firstItem = GetSelectedItem();

		// select new item
		if (GetSelectedItem() == hitem)
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
			clearSelection();
			SetTimer(1, 300, NULL);
		}
		m_firstItem = NULL;
	}

	for (int ii = 0; ii < GetSelectedItemCount(); ii++)
	{
		m_name = GetItemText(m_itemArr.GetAt(ii));
		if (m_name.GetLength() > L_MAPN)
			m_name = m_name.Left(L_MAPN);
		addFiles(m_name);
	}

	if (!(nFlags & MK_CONTROL) && !(nFlags & MK_SHIFT))
		CTreeCtrl::OnLButtonDown(nFlags, point);
}


void CViewTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pilDrag != NULL)
	{
		CPoint	pt = point;

		ClientToScreen(&pt);
		CImageList::DragLeave(this);
		CImageList::EndDrag();

		ReleaseCapture();

		//Delete the drag image list
		m_pilDrag->DeleteImageList();
		delete m_pilDrag;
		m_pilDrag = NULL;

		if (m_kind == viewMAP)
		{
			if (m_template)
				loadTemplate(pt);
			else if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISDROPAREA, MAKELPARAM(pt.x, pt.y)))
				OpenMap();
		}
		else
		{
			if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISDROPAREA, MAKELPARAM(pt.x, pt.y)))
			{
				DownloadFile();
				//AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TRLAY_DROP, MAKELPARAM(pt.x, pt.y));
			}
		}
	}

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CViewTree::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pilDrag != NULL)
	{
		CRect	rc;
		CPoint	pt = point;

		GetClientRect(&rc);
		
		//Draw the drag
		ClientToScreen(&pt);
		CImageList::DragMove(pt);

		SelectDropTarget(m_dropItem);
		HTREEITEM hItem = m_dropItem;

		if (m_template || AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISDROPAREA, MAKELPARAM(pt.x, pt.y)))
			SetCursor(m_cDropCopy);
		else
		{
			//Change the cursor to give feedback
			if (hItem)
			{
				if ((GetKeyState(VK_CONTROL) & 0x8000))
					SetCursor(m_cDropCopy);
				else
					SetCursor(m_cDropMove);
			}
			else
			{
				if ((GetKeyState(VK_CONTROL) & 0x8000))
					SetCursor(m_cNoDropCopy);
				else
					SetCursor(m_cNoDropMove);
			}
		}
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}


void CViewTree::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);
		GetParent()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	}

	CTreeCtrl::OnTimer(nIDEvent);
}


void CViewTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_pilDrag = CreateDragImage(pNMTreeView->itemNew.hItem);
	if (!m_pilDrag)
		return;
	
	m_dragItem = pNMTreeView->itemNew.hItem;
	m_dropItem = NULL;

	// Calculate the offset to the hotspot

	CPoint	offsetPt(8, 8);			// Initialize a default offset
	CPoint	pt = pNMTreeView->ptDrag;	// Get the Drag point
	UINT	nHitFlags = 0;
	HTREEITEM htiHit = HitTest(pt, &nHitFlags);

	if (htiHit != NULL)
	{
		// The drag point has Hit an item in the tree
		CRect	rc;

		if (GetItemRect(htiHit, &rc, FALSE))
		{
			// Count indent levels
			HTREEITEM htiParent = htiHit;
			int	nIndentCnt = 0;
			while (htiParent != NULL)
			{
				htiParent = GetParentItem(htiParent);
				nIndentCnt++;
			}
			
			if (!(GetStyle() & TVS_LINESATROOT)) 
				nIndentCnt--;
			
			// Calculate the new offset
			offsetPt.y = pt.y - rc.top;
			offsetPt.x = pt.x - (nIndentCnt * GetIndent()) + GetScrollPos(SB_HORZ);
		}
	}
	
	//Begin the dragging  
	m_pilDrag->BeginDrag(0, offsetPt);
	pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	m_pilDrag->DragEnter(NULL, pt);
	SetCapture();
	SelectItem(m_dragItem);

	*pResult = 0;
}

void CViewTree::Initialize(CString dir, bool bTemplate)
{
	CImageList	imglist;
	CBitmap		bitmap;

	m_dir = dir;
	m_template = bTemplate;
	m_files.RemoveAll();

	bitmap.LoadBitmap(IDB_SERVERTREE);
	imglist.Create(16, 16, ILC_COLOR24|ILC_MASK, 5, 1);
	imglist.Add(&bitmap, RGB(0, 255, 255));
	imglist.SetBkColor(CLR_NONE);
	SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();
}

void CViewTree::InitTree()
{
	POSITION pos;
	CString sKey;
	bool	is_splash;
	void*	ptr;

	DeleteAllItems();	
	m_itemMap1.RemoveAll();
	m_itemMap2.RemoveAll();
	is_splash = CSplashWnd::IsCreated();
	if (!is_splash)
		CSplashWnd::ShowSplashScreen(AfxGetMainWnd());
	
	if (m_kind == viewMAP)
	{
		for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
		{
			if (m_treeArr.GetSize() == 1)
				CSplashWnd::NextScreen(100);
			else
				CSplashWnd::NextScreen((ii * 100) / ((int)m_treeArr.GetSize() - 1));
			insert(m_treeArr.GetAt(ii).mapn, m_treeArr.GetAt(ii).mapd, ii);
		}
	}
	else
	{
		for (int ii = 0; ii < m_trxArr.GetSize(); ii++)
		{
			if (m_trxArr.GetSize() == 1)
				CSplashWnd::NextScreen(100);
			else
				CSplashWnd::NextScreen((ii * 100) / ((int)m_trxArr.GetSize() - 1));

			insertTrx(ii);
		}
	}

	SortChildren(NULL);
	for (pos = m_itemMap1.GetStartPosition(); pos != NULL; )
	{
		m_itemMap1.GetNextAssoc(pos, sKey, ptr);
		SortChildren((HTREEITEM)ptr);
	}

	
	TVSORTCB tvs;
	for(pos = m_itemMap2.GetStartPosition(); pos != NULL; )
	{
		m_itemMap2.GetNextAssoc(pos, sKey, ptr);
		tvs.hParent = (HTREEITEM)ptr;
		tvs.lpfnCompare = mapSortProc;
		tvs.lParam = (LPARAM) this;
		SortChildrenCB(&tvs);
	}
	
	CSplashWnd::NextScreen(100);
	CSplashWnd::NextScreen(-1);
	if (!is_splash)
		CSplashWnd::HideSplashScreen();
	m_itemMap1.RemoveAll();
	m_itemMap2.RemoveAll();
}

void CViewTree::LoadFile(CString path)
{
	if (m_kind == viewMAP)
	{
		struct _mapTree	mapTree;

		m_treeArr.RemoveAll();
		TRY
		{
			CFile file(path, CFile::modeRead);
			UINT nBytesRead;
			do
			{
				nBytesRead = file.Read(&mapTree, sizeof(mapTree));
				if (nBytesRead == sizeof(mapTree))
					m_treeArr.Add(mapTree);
			} while ((int)nBytesRead);
			file.Close();
		}CATCH(CFileException, e)
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		}
		END_CATCH
	}
	else
	{
		struct _trxlist	trxlist;

		m_trxArr.RemoveAll();
		TRY
		{
			CFile file(path, CFile::modeRead);
			UINT nBytesRead;
			do
			{
				nBytesRead = file.Read(&trxlist, sizeof(trxlist));
				if (nBytesRead == sizeof(trxlist))
					m_trxArr.Add(trxlist);
			} while ((int)nBytesRead);
			file.Close();
		}CATCH(CFileException, e)
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		}
		END_CATCH
	}
	InitTree();
}

void CViewTree::DownloadFile()
{
	CString tmps, maps, fullN;
	CMapStringToString	files;

	AfxGetApp()->DoWaitCursor(1);
	addFiles(m_name);

	files.RemoveAll();
	for (int ii = 0; ii < m_files.GetSize(); ii++)
	{
		maps = m_files.GetAt(ii);
		if (maps.GetLength() != L_MAPN)
			continue;

		wccSetTempPath(tmps, (char *)maps.operator LPCSTR());
		//tmps += _T(".xml");
		if (m_kind == viewMAP)
			fullN = setMapPath(maps);
		else
			fullN = setTrxPath(maps);
		files.SetAt(fullN, tmps);
	}
	GetParent()->SendMessage(WM_USER + 100, MAKEWPARAM(8, 0), (LPARAM)&files);
}

void CViewTree::OpenMap(int idx, int mode)
{
	CString tmps, maps, fullN;
	CMapStringToString	files;

	AfxGetApp()->DoWaitCursor(1);
	m_open = true;
	if (idx >= 0)
	{
		m_name = m_treeArr.GetAt(idx).mapn;
		m_files.RemoveAll();
	}
	addFiles(m_name);
	
	files.RemoveAll();
	for (int ii = 0; ii < m_files.GetSize(); ii++)
	{
		maps = m_files.GetAt(ii);
		if (maps.GetLength() != L_MAPN)
			continue;

		wccSetTempPath(tmps, (char *)maps.operator LPCSTR());
		if (m_kind == viewMAP)
		{
			tmps += _T(".xml");
			fullN = setMapPath(maps);
		}
		else
			fullN = setTrxPath(maps);
		files.SetAt(fullN, tmps);
	}
	//GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(1, m_template ? TRUE : FALSE), (LPARAM)&files);
	if (m_kind == viewMAP)
		GetParent()->SendMessage(WM_USER + 100, MAKEWPARAM(1, mode), (LPARAM)&files);
	else
		GetParent()->SendMessage(WM_USER + 100, MAKEWPARAM(7, 0), (LPARAM)&files);
	m_open = false;
	AfxGetApp()->DoWaitCursor(0);
}

CString CViewTree::LoadForm(CString mapName)
{
	CString tmps, fullN;
	CMapStringToString	files;

	AfxGetApp()->DoWaitCursor(1);
	m_open = true;

	files.RemoveAll();
	wccSetTempPath(tmps, (char *)mapName.operator LPCSTR());
	fullN = setMapPath(mapName);
	files.SetAt(fullN, tmps);
	GetParent()->SendMessage(WM_USER + 100, 2, (LPARAM)&files);
	m_open = false;
	AfxGetApp()->DoWaitCursor(0);
	return tmps;
}

void CViewTree::OpenMap(CString mapName, int mode)
{
	CString tmps, fullN;
	CMapStringToString files;

	AfxGetApp()->DoWaitCursor(1);
	m_open = true;
	
	if (mapName.GetLength() > L_MAPN)
		mapName = mapName.Left(L_MAPN);
	
	m_files.RemoveAll();
	files.RemoveAll();
	wccSetTempPath(tmps, (LPSTR)mapName.operator LPCTSTR());
	fullN = setMapPath(mapName);
	addFiles(mapName);

	files.SetAt(fullN, tmps);
	//GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(mapName.GetLength() == 8 ? 1 : 2, m_template ? TRUE : FALSE), (LPARAM)&files);
	GetParent()->SendMessage(WM_USER + 100, MAKEWPARAM(mapName.GetLength() == 8 ? 1 : 2, mode), (LPARAM)&files);

	m_open = false;
	AfxGetApp()->DoWaitCursor(-1);
}

void CViewTree::SetDepth(int depth, bool init)
{
	m_depth = depth;
	if (init)
		InitTree();
}

int CViewTree::GetSelectedItemCount()
{
	HTREEITEM hitem;

	m_itemArr.RemoveAll();
	hitem = getFirstSelectedItem();
	if (hitem == (HTREEITEM) NULL)
		return 0;

	if (GetItemData(hitem) != (int)R_NONE)
		m_itemArr.Add(hitem);
	for (hitem = getNextSelectedItem(hitem); hitem != NULL; hitem = getNextSelectedItem(hitem))
	{
		if (hitem == (HTREEITEM) NULL)
			break;

		if (GetItemData(hitem) == (int)R_NONE)
			continue;

		m_itemArr.Add(hitem);
	}
	return (int)m_itemArr.GetSize();
}

CString CViewTree::GetNameOfIdx(int idx)
{
	return setMapPath(m_treeArr.GetAt(idx).mapn);
}

void CViewTree::RemoveAll()
{
	DeleteAllItems();
	m_treeArr.RemoveAll();
	m_trxArr.RemoveAll();
	m_itemArr.RemoveAll();
	m_files.RemoveAll();
}

int CViewTree::GetFiles(CStringArray& files)
{
	files.Copy(m_files);
	return (int)files.GetSize();
}

void CViewTree::clearSelection()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			SetItemState(hitem, 0, TVIS_SELECTED);
	}
	SelectItem(NULL);
}

bool CViewTree::selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO)
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

HTREEITEM CViewTree::getFirstSelectedItem()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return NULL;
}

HTREEITEM CViewTree::getNextSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetNextVisibleItem(hitem); hitem != NULL; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return NULL;
}

void CViewTree::loadTemplate(CPoint pt)
{
	CString	tmps;
	CString	fullN;

	wccSetTempPath(tmps, _T("template"));
	fullN = setMapPath(m_name) + _T(";") + tmps;
	GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(3, 0), (LPARAM)fullN.operator LPCSTR());

	AfxGetMainWnd()->PostMessage(WM_USER, MAKEWPARAM(ID_USR_LOADTEMPLATE, 1), MAKELPARAM(pt.x, pt.y));
}

void CViewTree::addFiles(CString name)
{
	if (name.IsEmpty())
		return;

	for (int ii = 0; ii < m_files.GetSize(); ii++)
	{
		if (!m_files.GetAt(ii).Compare(name))
			return;
	}
	m_files.Add(name);
}

void CViewTree::insert(CString maps, CString desc, int idx)
{
	void*	ptr;
	CString	tmps;
	HTREEITEM hitem1 = NULL, hitem2 = NULL;

	tmps = maps.Left(L_SGID);
	if (!m_itemMap1.Lookup(tmps, ptr))
	{
		hitem1 = InsertItem(tmps, 0, 0);
		SetItemData(hitem1, R_NONE);
		m_itemMap1.SetAt(tmps, hitem1);
	}
	else
		hitem1 = (HTREEITEM)ptr;

	tmps = maps.Left(m_depth);
	if (!m_itemMap2.Lookup(tmps, ptr))
	{
		hitem2 = InsertItem(tmps, 0, 0, hitem1);
		SetItemData(hitem2, R_NONE);
		m_itemMap2.SetAt(tmps, hitem2);
	}
	else
		hitem2 = (HTREEITEM)ptr;

	int imgIdx = 0;
	_mapTree  info = m_treeArr.GetAt(idx);
	CString sID = CString(info.ckid);
	TRIM(sID);
	CMainFrame* pFrame = NULL;
	while (pFrame == NULL)
		pFrame = (CMainFrame*)AfxGetMainWnd();

	if (sID == "")
		imgIdx = 2;
	else if (sID == pFrame->m_userID)
		imgIdx = 3;
	else
		imgIdx = 4;

	hitem1 = InsertItem(maps + _T(" ") + desc, imgIdx, imgIdx, hitem2);
	SetItemData(hitem1, idx);
}

void CViewTree::insertTrx(int idx)
{
	HTREEITEM hitem1 = NULL;
	CString strTmp("");
	_trxlist trxList = m_trxArr.GetAt(idx);

	void*	ptr;
	CString	tmps;

	tmps.Format("%c%c", trxList.trxc[0], trxList.trxc[1]);
	if (!m_itemMap1.Lookup(tmps, ptr))
	{
		hitem1 = InsertItem(tmps, 0, 0);
		SetItemData(hitem1, R_NONE);
		m_itemMap1.SetAt(tmps, hitem1);
	}
	else
		hitem1 = (HTREEITEM)ptr;

	int imgIdx = 5;

	strTmp.Format("%s(%s) %s", CString(trxList.name, sizeof(trxList.name)), 
		CString(trxList.trxc, sizeof(trxList.trxc)),
		CString(trxList.trxd, sizeof(trxList.trxd)));
	hitem1 = InsertItem(strTmp, imgIdx, imgIdx, hitem1);
	SetItemData(hitem1, idx);
}

CString CViewTree::setMapPath(CString maps)
{
	CString	fullN;

	if (m_template)
		fullN.Format(_T("template/%s/%s/%s"), maps.Left(L_SGID), maps.Left(L_TGID), maps);
	else
		fullN.Format(_T("map/%s/%s/%s"), maps.Left(L_SGID), maps.Left(L_TGID), maps);
	return fullN;
}

CString CViewTree::setTrxPath(CString maps)
{
	CString	fullN;

	fullN.Format(_T("trlayout/%s"), maps);
	return fullN;
}

bool CViewTree::SelPosItem()
{
	TV_HITTESTINFO hit_info;
	bool bRet = false;

	// 화면상에서 마우스의 위치를 얻는다.
	::GetCursorPos(&hit_info.pt);

	// 얻은 마우스 좌표를 트리컨트롤 기준의 좌표로 변경한다.
	::ScreenToClient(this->m_hWnd, &hit_info.pt);

	// 현재 마우스 좌표가 위치한 항목 정보를 얻는다.
	HTREEITEM current_item = this->HitTest(&hit_info);
	if (current_item != NULL){
		// 마우스가 위치한 항목을 찾았다면 해당 항목을 선택한다.
		bRet = true;
		this->Select(current_item, TVGN_CARET);
	}
	else
	{
		//SelectDropTarget(NULL);
		this->Select(NULL, TVGN_CARET);
		//return;
	}
	return bRet;
}

void CViewTree::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_kind == viewTRLAYOUT)
		return;
	*pResult = 0;
	if (!SelPosItem()) return;

	CMenu	menu;
	int	cmd;
	CMainFrame* pFrame = NULL;

	while (pFrame == NULL)
		pFrame = (CMainFrame*)AfxGetMainWnd();

	HTREEITEM hitem = GetSelectedItem();
	if (hitem == NULL)
		return;

	int nSelect = GetItemData(hitem);
	if (nSelect < 0)
		return;

	_mapTree  info = m_treeArr.GetAt(nSelect);
	// check in id
	CString sID = CString(info.ckid);
	CString sFormName = CString(info.mapn);

	TRIM(sID);
	TRIM(sFormName);

	CString sMenu;
	//sMenu.Format("Authority User(%s)", sID);
	sMenu.Format("Authority User(%s)", sID);
	menu.CreatePopupMenu();

	CPoint point;
	GetCursorPos(&point);

	enum { tNONE, tREAD, tWRITE, tTEMP, tOUT, tHISTORY };

	menu.AppendMenu(MF_STRING, tREAD, _T("Read Only"));
	menu.AppendMenu(MF_STRING, tWRITE, _T("Write Only"));
	menu.AppendMenu(MF_STRING, tTEMP, _T("Write Only(Temporary)"));
	menu.AppendMenuA(MF_SEPARATOR, 0, "");

	menu.AppendMenu(MF_STRING, 10001, sMenu);
	menu.AppendMenu(MF_STRING, tOUT, _T("Check-Out"));
	menu.AppendMenuA(MF_SEPARATOR, 10000, "");
	menu.AppendMenu(MF_STRING, tHISTORY, _T("Resource History"));

	cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
	char gb;
	switch (cmd)
	{
	case tREAD:
		gb = 'R';
		OpenMap(nSelect, (int)gb);
		break;
	case tWRITE:
		gb = 'W';
		OpenMap(nSelect, (int)gb);
		break;
	case tTEMP:
		gb = 'T';
		OpenMap(nSelect, (int)gb);
		break;
	case tOUT:
		pFrame->CheckOutDlg(sFormName);
		break;
	case tHISTORY:
		pFrame->GetHistoryDlg(sFormName);
		break;
	default:
		break;
	}
}

BOOL CViewTree::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//  the CREATESTRUCT cs
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS;
	cs.style |= TVS_SHOWSELALWAYS;
	return CTreeCtrl::PreCreateWindow(cs);
}
