#include "stdafx.h"
#include "ExTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////
TreeData::TreeData(int depth, int nData)
{
	m_nDepth = depth;
	m_nData = nData;
}

TreeData::TreeData(int depth, CString sData)
{
	m_nDepth = depth;
	m_sData = sData;
}

TreeData::TreeData(CString data, DWORD dw)
{
	m_sData = data;
	m_sPoint = dw;
}

TreeData::TreeData(int depth, int nData, DWORD dw)
{
	m_nDepth = depth;
	m_nData = nData;
	m_sPoint = dw;
}

TreeData::~TreeData()
{
}
//////////////////////////////////////////////////////////////

CExTreeCtrl::CExTreeCtrl()
{
	m_ImageList = NULL;
	m_Ptr.RemoveAll();
	m_bDrag = false;
	m_bDragging = false;
	m_hItemDrag = NULL;
	m_hItemDrop = NULL;

	m_bMultiSel = false;
}

CExTreeCtrl::~CExTreeCtrl()
{
	if (m_ImageList)
	{
		m_ImageList->DeleteImageList();
		delete m_ImageList;
	}
	FreePtr();
}


BEGIN_MESSAGE_MAP(CExTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CExTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	/*
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	*/
END_MESSAGE_MAP()

void CExTreeCtrl::InitTreeImage(CBitmap& bm)
{
	CBitmap& bitmap = bm;
	
	m_ImageList = new CImageList();
	m_ImageList->Create(16, 16, ILC_COLORDDB | ILC_MASK, 4, 1); 
	m_ImageList->Add(&bitmap, RGB(255, 0, 255)); 
	SetImageList(m_ImageList, TVSIL_NORMAL); 
}

void CExTreeCtrl::SetEditState(bool bEdit, HTREEITEM item /* = NULL */)
{
	long style;

	if (bEdit)
	{
		style = GetWindowLong(m_hWnd, GWL_STYLE);
		style |= TVS_EDITLABELS;
		SetWindowLong(m_hWnd, GWL_STYLE, style);
		if (item)
		{
			EditLabel(item);
			CEdit* pEdit = GetEditControl();
			pEdit->LimitText(80);
		}

	}
	else
	{
		style = GetWindowLong(m_hWnd, GWL_STYLE);
		style &= ~(TVS_EDITLABELS);
		SetWindowLong(m_hWnd, GWL_STYLE, style);
	}
}

void CExTreeCtrl::SetItemDataEx(HTREEITEM hItem, int depth, int ndata, DWORD dw)
{
	TreeData* pData = new TreeData(depth, ndata, dw);
	m_Ptr.Add(pData);
	SetItemData(hItem, (DWORD)m_Ptr[m_Ptr.GetSize()-1]);
}

void CExTreeCtrl::GetItemDataEx(HTREEITEM hItem, int& depth, int& ndata, DWORD& dw)
{
	TreeData* pData = (TreeData *)GetItemData(hItem);
	depth = pData->m_nDepth;
	ndata = pData->m_nData;
	dw = pData->m_sPoint;
}

void CExTreeCtrl::SetItemDataEx(HTREEITEM hItem, int depth, CString str)
{
	TreeData* pData = new TreeData(depth, str);
	m_Ptr.Add(pData);
	SetItemData(hItem, (DWORD)m_Ptr[m_Ptr.GetSize()-1]);
}

void CExTreeCtrl::GetItemDataEx(HTREEITEM hItem, int& depth, CString& str)
{
	TreeData* pData = (TreeData *)GetItemData(hItem);
	depth = pData->m_nDepth;
	str = pData->m_sData;
}

void CExTreeCtrl::SetItemDataEx(HTREEITEM hItem, int depth, int ndata)
{
	TreeData* pData = new TreeData(depth, ndata);
	m_Ptr.Add(pData);
	SetItemData(hItem, (DWORD)m_Ptr[m_Ptr.GetSize()-1]);
}

void CExTreeCtrl::GetItemDataEx(HTREEITEM hItem, int& depth, int& ndata)
{
	TreeData* pData = (TreeData *)GetItemData(hItem);
	depth = pData->m_nDepth;
	ndata = pData->m_nData;
}

void CExTreeCtrl::SetItemDataEx(HTREEITEM hItem, CString data, DWORD dw)
{
	TreeData* pData = new TreeData(data, dw);
	m_Ptr.Add(pData);
	SetItemData(hItem, (DWORD)m_Ptr[m_Ptr.GetSize()-1]);
}

void CExTreeCtrl::GetItemDataEx(HTREEITEM hItem, CString& data, DWORD &dw)
{
	TreeData* pData = (TreeData *)GetItemData(hItem);
	if (!pData)
	{
		data.Empty();
		dw = -1;
		return;
	}
	data = pData->m_sData;
	dw = pData->m_sPoint;
}

void CExTreeCtrl::SetDragEnabled(bool bDrag /* = true */)
{
	m_bDrag = bDrag;
}
/*
void CExTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (!m_bDrag)		return;
	
	m_hItemDrag = pNMTreeView->itemNew.hItem;
	m_hItemDrop = NULL;
	m_bDragging = true;

	*pResult = 0;
}


void CExTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bDrag)		return;

	if (m_bDragging)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		HTREEITEM hItem = HitTest(point);
		SelectItem(hItem);
	}
	else
		SetCursor(LoadCursor(NULL,IDC_ARROW));
	
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CExTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonUp(nFlags, point);
	if (!m_bDrag)		return;
	if (!m_bDragging)	return;
	
	int tot = 0;
	m_bDragging = false;
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	m_hItemDrop = HitTest(point);
	if (m_hItemDrag == m_hItemDrop || m_hItemDrop == NULL)
		return;

	int nDrag = getDepth(m_hItemDrag);
	int nDrop = getDepth(m_hItemDrop);

	HTREEITEM hItemNew = NULL;
	if (nDrag == nDrop)
		hItemNew = copyBranch(m_hItemDrag, GetParentItem(m_hItemDrop), m_hItemDrop);
	else if ((nDrag - nDrop) == 1)
		hItemNew = copyBranch(m_hItemDrag, m_hItemDrop, m_hItemDrop);
	else
		return;

	DeleteItem(m_hItemDrag);
	SelectItem(hItemNew);
	Expand(hItemNew, TVE_EXPAND);
}
*/
int CExTreeCtrl::getDepth(HTREEITEM item)
{
	if (GetParentItem(item) == NULL)
		return 0;
	else 
	{
		if (GetParentItem(GetParentItem(item)) == NULL)
			return 1;
		else
			return 2;
	}
	return -1;
}

HTREEITEM CExTreeCtrl::copyBranch(HTREEITEM hitemDrag, HTREEITEM hitemDrop, 
					HTREEITEM htiAfter /*= TVI_LAST*/ )
{
        HTREEITEM hChild;

        HTREEITEM hNewItem = copyItem(hitemDrag, hitemDrop, htiAfter);
        hChild = GetChildItem(hitemDrag);
        while (hChild != NULL)
        {
                copyBranch(hChild, hNewItem);  
                hChild = GetNextSiblingItem(hChild);
        }
        return hNewItem;
}

HTREEITEM CExTreeCtrl::copyItem(HTREEITEM hItem, HTREEITEM htiNewParent, 
					HTREEITEM htiAfter /*= TVI_LAST*/ )
{
        TV_INSERTSTRUCT         tvstruct;
        HTREEITEM               hNewItem;
        CString                 sText;
        
        tvstruct.item.hItem = hItem;
        tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | 
                                TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        GetItem(&tvstruct.item);  
        sText = GetItemText(hItem);
        
        tvstruct.item.cchTextMax = sText.GetLength();
        tvstruct.item.pszText = sText.LockBuffer();

        tvstruct.hParent = htiNewParent;
        tvstruct.hInsertAfter = htiAfter;
        tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
        hNewItem = InsertItem(&tvstruct);
        sText.ReleaseBuffer();

        SetItemData(hNewItem, GetItemData(hItem));
        SetItemState(hNewItem, GetItemState(hItem, TVIS_STATEIMAGEMASK), 
                                                        TVIS_STATEIMAGEMASK);

        return hNewItem;
}

BOOL CExTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN 
		&& pMsg->wParam == VK_ESCAPE 
		&& m_bDragging
		&& m_bDrag)
	{
		m_bDragging = false;
		ReleaseCapture();
		SelectDropTarget(NULL);
		return TRUE;
	}
	
	return CTreeCtrl::PreTranslateMessage(pMsg);
}


void CExTreeCtrl::SetMultiSel(bool bMultiSel/* = true*/)
{
	m_bMultiSel = bMultiSel;
}

void CExTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_bMultiSel)
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);
		return;
	}
/*
	if (nFlags & MK_CONTROL) 
	{
		UINT flag;
		HTREEITEM hItem = HitTest(point, &flag);
		if (hItem)
		{
			UINT uNewSelState = 
				GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED ? 
							0 : TVIS_SELECTED;
            
			HTREEITEM hItemOld = GetSelectedItem();
			UINT uOldSelState = hItemOld ? 
					GetItemState(hItemOld, TVIS_SELECTED) : 0;
            
			
			if( GetSelectedItem() == hItem )
				SelectItem(NULL);

			CTreeCtrl::OnLButtonDown(nFlags, point);

			SetItemState(hItem, uNewSelState,  TVIS_SELECTED);

			if (hItemOld && hItemOld != hItem)
				SetItemState(hItemOld, uOldSelState, TVIS_SELECTED);

			m_hItemFirstSel = NULL;

			return;
		}
	}
	else if(nFlags & MK_SHIFT)
	{
		UINT flag;
		HTREEITEM hItem = HitTest(point, &flag);

		if (!m_hItemFirstSel)
			m_hItemFirstSel = GetSelectedItem();

		if( GetSelectedItem() == hItem )
			SelectItem(NULL);

		CTreeCtrl::OnLButtonDown(nFlags, point);

		if (m_hItemFirstSel)
		{
			selectItems(m_hItemFirstSel, hItem);
			return;
		}
	}
	else
	{
		clearSelection();
		m_hItemFirstSel = NULL;
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
*/
}

void CExTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bMultiSel)
	{
		CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
/*
	if ( (nChar==VK_UP || nChar==VK_DOWN) && GetKeyState( VK_SHIFT )&0x8000)
	{
		if (!m_hItemFirstSel)
		{
			m_hItemFirstSel = GetSelectedItem();
			clearSelection();
		}

		HTREEITEM hItemPrevSel = GetSelectedItem();

		HTREEITEM hItemNext;
		if ( nChar==VK_UP )
			hItemNext = GetPrevVisibleItem(hItemPrevSel);
		else
			hItemNext = GetNextVisibleItem(hItemPrevSel);

		if (hItemNext)
		{
			BOOL bReselect = 
				!(GetItemState(hItemNext, TVIS_SELECTED) & TVIS_SELECTED);

			SelectItem(hItemNext);

			if (bReselect)
				SetItemState(hItemPrevSel, TVIS_SELECTED, TVIS_SELECTED);
		}
		return;
	}
	else if (nChar >= VK_SPACE)
	{
		m_hItemFirstSel = NULL;
		clearSelection();
	}
	
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
*/
}



BOOL CExTreeCtrl::selectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo)
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

	BOOL bSelect = TRUE;
	while (hItem)
	{
		SetItemState(hItem, bSelect ? TVIS_SELECTED : 0, TVIS_SELECTED);

		if( hItem == hItemTo ) 
			bSelect = FALSE;

		hItem = GetNextVisibleItem( hItem );
	}

	return TRUE;
}

void CExTreeCtrl::clearSelection()
{
	for (HTREEITEM hItem = GetRootItem(); hItem != NULL; hItem = GetNextItem(hItem, TVGN_NEXT))
	{
		if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
			SetItemState(hItem, 0, TVIS_SELECTED);
	}
}


HTREEITEM CExTreeCtrl::GetFirstSelectedItem()
{
	for ( HTREEITEM hItem = GetRootItem(); hItem!=NULL; hItem = GetNextItem( hItem, TVGN_NEXT ) )
		if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			return hItem;

	return NULL;
}

HTREEITEM CExTreeCtrl::GetNextSelectedItem( HTREEITEM hItem )
{
	for ( hItem = GetNextItem( hItem , TVGN_CARET); hItem!=NULL; hItem = GetNextItem( hItem, TVGN_NEXT ) )
		if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			return hItem;

	return NULL;
}

HTREEITEM CExTreeCtrl::GetPrevSelectedItem(HTREEITEM hItem)
{
	for (hItem = GetPrevItem(hItem); hItem!=NULL; hItem = GetPrevItem( hItem) )
		if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			return hItem;

	return NULL;
}

HTREEITEM CExTreeCtrl::GetPrevItem(HTREEITEM hItem)
{
	HTREEITEM item;

	if (item = GetPrevSiblingItem(hItem))
		return item;		
	else
		return GetParentItem(hItem);
}

void CExTreeCtrl::FreePtr()
{
	for (int ii = 0; ii < m_Ptr.GetSize(); ii++)
	{
		TreeData* pData = (TreeData *)m_Ptr.GetAt(ii);
		delete pData;
	}

	m_Ptr.RemoveAll();
}