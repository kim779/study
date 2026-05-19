
#include "stdafx.h"
#include "ViewTree.h"
#include "resource.h"
#include "splashWnd.h"
#include "h/mainvar.h"
#include "h/axisvar.h"
#include "amCc/LibBuild.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	R_NONE		-1
#define IDR_POPUP_MENU	10001
////////////////////////////////////////////////////////////////////////////
// CViewTree

static int CALLBACK mapSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the tree control.
	// The lParam of an item is just its handle.
	CViewTree* pTree = (CViewTree*) lParamSort;
	//CString    strItem1 = pTree->m_treeArr.GetAt(lParam1).mapn;
	//CString    strItem2 = pTree->m_treeArr.GetAt(lParam2).mapn;
	CString    strItem1 = pTree->m_treeArr.GetAt(lParam1)->m_fname;
	CString    strItem2 = pTree->m_treeArr.GetAt(lParam2)->m_fname;
	
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

	m_pilDrag   = NULL;
	m_firstItem = NULL;
	m_dragItem  = NULL;
	m_dropItem  = NULL;

	m_treeArr.RemoveAll();
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

	CMapTree* tree;
	for (int ii = 0; ii < m_treeArr.GetCount(); ii++)
	{
		tree = m_treeArr.GetAt(ii);
		delete tree;
		tree = NULL;
	}

	m_maptreekind = ID_MAPTREE;
}

CViewTree::~CViewTree()
{
	for (int ii = 0;  ii < m_treeArr.GetSize(); ii++)
		delete (m_treeArr.GetAt(ii));

	m_treeArr.RemoveAll();
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
	ON_WM_RBUTTONDOWN()
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
	int count = GetSelectedItemCount();

	if (count != 0)
	{
		OpenMap();
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
		if (m_maptreekind == ID_MAPTREE)
			m_name = m_name.Left(m_name.Find(" "));
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
		m_name.ReleaseBuffer();
		if (m_name.GetLength() > L_MAPN)
			m_name = m_name.Left(m_name.Find(" "));

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

		if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISDROPAREA, MAKELPARAM(pt.x, pt.y)))
			OpenMap();
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

		if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISDROPAREA, MAKELPARAM(pt.x, pt.y)))
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


void CViewTree::OnTimer(UINT nIDEvent)
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

void CViewTree::Initialize(CString dir)
{
	CImageList	imglist;
	CBitmap		bitmap;

	m_dir = dir;
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
	int iFind = 0;

	DeleteAllItems();	
	m_itemMap1.RemoveAll();
	m_itemMap2.RemoveAll();
	is_splash = CSplashWnd::IsCreated();
	if (!is_splash)
		CSplashWnd::ShowSplashScreen(AfxGetMainWnd());
	
	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
	{
		if (m_treeArr.GetSize() == 1)
			CSplashWnd::NextScreen(100);
		else
			CSplashWnd::NextScreen((ii * 100) / ((int)m_treeArr.GetSize() - 1));
		//insert(m_treeArr.GetAt(ii).mapn, m_treeArr.GetAt(ii).mapd, ii);

		if (m_maptreekind == ID_MAPTREE)
		{
			iFind = m_treeArr.GetAt(ii)->m_fname.Find(_T("."));
			insert(m_treeArr.GetAt(ii)->m_fname.Left(iFind), m_treeArr.GetAt(ii)->m_desc, ii);
		}
		else
			insert(m_treeArr.GetAt(ii)->m_fname, m_treeArr.GetAt(ii)->m_desc, ii);
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
//	struct _mapTree	mapTree;
//
//	m_treeArr.RemoveAll();
// 	TRY
// 	{
// 		CFile file(path, CFile::modeRead);
// 		UINT nBytesRead;
// 		do
// 		{
// 			nBytesRead = file.Read(&mapTree, sizeof(mapTree));
// 			if (nBytesRead == sizeof(mapTree))
// 				m_treeArr.Add(mapTree);
// 		}while((int)nBytesRead);
// 		file.Close();
// 	}CATCH(CFileException, e )
// 	{
// #ifdef _DEBUG
// 		afxDump << "File could not be opened " << e->m_cause << "\n";
// #endif
// 	}
// 	END_CATCH
	
	//struct _mapTree	mapTree;

	m_treeArr.RemoveAll();

	class CMapTree *mapTree;

	CFile	file;
	int	Size = 0;
	char*	sbuff;
	CString treedata, tmp, data;

	if (file.Open(path, CFile::modeRead))
	{
		Size = (UINT)file.GetLength();

		sbuff = new char[Size + 1];
		ZeroMemory(sbuff, Size + 1);
		UINT rc = file.Read(sbuff, Size);

		if (rc != Size)
		{
			delete[] sbuff;
			file.Close();
		}
		
		treedata = CString(sbuff, Size);
		delete [] sbuff;

		while (!treedata.IsEmpty())
		{
			tmp = Parser(treedata, _T("\n"));

			if (tmp.IsEmpty())
				break;
			int ii = 0;
			mapTree = new CMapTree();
			while (!tmp.IsEmpty())
			{
				data = Parser(tmp, _T("\t"));
				switch (ii)
				{
				case 0:
					mapTree->m_mgub = data;
					break;
				case 1:
					mapTree->m_desc = data;
					break;
				case 2:
					mapTree->m_fname = data;
					break;
				case 3:
					mapTree->m_ckgb = data;
					break;
				case 4:
					mapTree->m_ckid = data;
					break;
				}
				ii++;
			}
			m_treeArr.Add(mapTree);
		}
	}

	InitTree();
}

CString CViewTree::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return _T("");
}

void CViewTree::OpenMap(int idx)
{
	CString tmps, maps, fullN;
	CMapStringToString	files;
	CString sRoot;

	CMainFrame* pFrame = NULL;
	while (pFrame == NULL)
		pFrame = (CMainFrame*)AfxGetMainWnd();


	//AfxGetApp()->DoWaitCursor(1);
	m_open = true;
	if (idx >= 0)
	{
//		m_name = m_treeArr.GetAt(idx).mapn;
		m_name = m_treeArr.GetAt(idx)->m_fname;
		m_files.RemoveAll();
	}
	addFiles(m_name);

	files.RemoveAll();
	for (int ii = 0; ii < m_files.GetSize(); ii++)
	{
		maps = m_files.GetAt(ii);
//		if (maps.GetLength() != L_MAPN)
//			continue;

		wccSetTempPath(tmps, (char *)maps.operator LPCSTR());	

		switch (m_maptreekind)
		{
		case ID_MAPTREE:
			if (maps.Find("XML") == -1)
			{
				maps.Remove('.');
				maps += ".XML";
				pFrame->DownMapDlg(maps);
			}
			break;
		case ID_TRLATOUT:
			pFrame->DownTRlayOut(maps, 0);

			sRoot.Format("%s\\trlayout\\%s", pFrame->m_root, maps);
			pFrame->SendMessage(WM_USER, ID_USR_TESTTRLAYOUT, (LPARAM)sRoot.operator LPCSTR());
			AfxGetApp()->DoWaitCursor(0);
			return;
			break;
		}

		fullN = setMapPath(maps);
		files.SetAt(fullN, tmps);
	}
	//GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(1, 0), (LPARAM)&files);
	//AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, 0);
	pFrame->OpenServerMap();

	m_open = false;
	//AfxGetApp()->DoWaitCursor(0);
}

void CViewTree::OpenMap(CString mapName)
{
	CString tmps, fullN;
	CMapStringToString files;

	AfxGetApp()->DoWaitCursor(1);
	m_open = true;
	
	if (mapName.GetLength() > L_MAPN)
		m_name.Left(m_name.Find(" "));
	
	m_files.RemoveAll();
	files.RemoveAll();

	wccSetTempPath(tmps, (LPSTR)mapName.operator LPCTSTR());
	fullN = setMapPath(mapName);
	addFiles(mapName);

	files.SetAt(fullN, tmps);
	GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(/*mapName.GetLength() == 8 ? 1 : 2, 0*/1, 0), (LPARAM)&files);

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
	return setMapPath(m_treeArr.GetAt(idx)->m_fname);
	//return _T("") setMapPath(m_treeArr.GetAt(idx).mapn);
}

void CViewTree::RemoveAll()
{
	DeleteAllItems();
	m_treeArr.RemoveAll();
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

	if (m_maptreekind == ID_MAPTREE)
	{
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

		class CMapTree *mapTree = m_treeArr.GetAt(idx);	
		if (atoi(mapTree->m_ckgb) == 0)		// check_out
			hitem1 = InsertItem(maps + _T(" ") + desc, 2, 2, hitem2);
		else if (atoi(mapTree->m_ckgb) == 1)	// check_in
			hitem1 = InsertItem(maps + _T(" ") + desc, 3, 3, hitem2);
		else					// check_in(write)
			hitem1 = InsertItem(maps + _T(" ") + desc, 4, 4, hitem2);
	}
	else
	{
		tmps = maps.Left(4);
		if (!m_itemMap1.Lookup(tmps, ptr))
		{
			hitem1 = InsertItem(tmps, 0, 0);
			SetItemData(hitem1, R_NONE);
			m_itemMap1.SetAt(tmps, hitem1);
		}
		else
			hitem1 = (HTREEITEM)ptr;

		hitem1 = InsertItem(maps, 2, 2, hitem1);
	}

	SetItemData(hitem1, idx);
}

CString CViewTree::setMapPath(CString maps)
{
	CString	fullN;

	//fullN.Format(_T("map\\%s\\%s\\%s"), maps.Left(L_SGID), maps.Left(L_TGID), maps);
	//return fullN;
	return maps;
}


void CViewTree::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_maptreekind == ID_TRLATOUT)
		return;

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

	CMapTree  *pinfo = m_treeArr.GetAt(nSelect);
	// check in id
	CString sID = CString(pinfo->m_ckid);
	CString sFormName = CString(pinfo->m_fname);
	CString sUser = pFrame->m_userID;

	TRIM(sUser);
	TRIM(sID);
	TRIM(sFormName);


	CString sMenu;
	sMenu.Format("Authority User(%s)", sID);
	menu.CreatePopupMenu();
	GetCursorPos(&point);


	enum { tNONE, tREAD, tWRITE, tTEMP, tOUT, tHISTORY };

	menu.AppendMenu(MF_STRING, tREAD, _T("Read Only"));
	menu.AppendMenu(MF_STRING, tWRITE, _T("Write Only"));
	menu.AppendMenu(MF_STRING, tTEMP, _T("Write Only(Temporary)"));
	menu.AppendMenuA(MF_SEPARATOR, 0, "");
	
	menu.AppendMenu(MF_STRING, 10001, sMenu);
	menu.AppendMenu(MF_STRING, tOUT, _T("Check-Out"));
	menu.AppendMenuA(MF_SEPARATOR, 10000, "");	
	menu.AppendMenu(MF_STRING, tHISTORY, _T("History"));

	cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);

	switch (cmd)
	{
	case tREAD:
	case tWRITE:
	case tTEMP:
		pFrame->DownMapDlg(sFormName, cmd - 1);
		OpenMap(sFormName);
		break;
	case tOUT:
		pFrame->CheckOutDlg("0", sFormName);
		break;
	case tHISTORY:
		pFrame->GetHistoryDlg(sFormName);
		break;
	default:
		break;
	}

	CTreeCtrl::OnRButtonDown(nFlags, point);
}
