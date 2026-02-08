

//////////////// Includes ////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "TreeOptionsCtrl.h"
#include "shareMsg.h"

//////////////// Macros / Locals /////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////// Implementation //////////////////////////////////////
IMPLEMENT_DYNAMIC(CTreeOptionsCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CTreeOptionsCtrl, CTreeCtrl)
//{{AFX_MSG_MAP(CTreeOptionsCtrl)
ON_WM_DESTROY()
ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CTreeOptionsCtrl::CTreeOptionsCtrl()
{
	m_nilID = IDB_BITMAP1;//IDB_TREE_CTRL_OPTIONS;
	m_hPrev = NULL;
	m_bFirst = true;
	m_selItem.RemoveAll();
}


HTREEITEM CTreeOptionsCtrl::InsertGroup(LPCTSTR lpszItem, int nImage, int nSelImage, HTREEITEM hParent)
{
	ASSERT(nImage > 1); //You must specify an image index greater than three as the 
	//first 8 images in the image list are reserved for the 
	//checked and unchecked check box and radio buttons images
	
	HTREEITEM hItem = InsertItem(lpszItem, nImage, nSelImage, hParent);
	SetItemData(hItem, 0); //Item data must be 0
	return hItem;
}

HTREEITEM CTreeOptionsCtrl::InsertChild(LPCTSTR lpszItem, int nImage, int nSelImage, HTREEITEM hParent)
{
	HTREEITEM hItem = InsertItem(lpszItem, nImage, nSelImage, hParent);
	SetItemData(hItem, 0);    //Item data must be 0
	
	return hItem;
}

void CTreeOptionsCtrl::setImage(CBitmap* pBitmap)
{
	m_ilTree.Create(64, 16, ILC_COLORDDB, 4, 1 );
	m_ilTree.Add(pBitmap, 0xFF00FF);
}

void CTreeOptionsCtrl::PreSubclassWindow() 
{
	//Let the parent class do its thing	
	CTreeCtrl::PreSubclassWindow();
	
	if (m_ilTree.GetSafeHandle() != NULL)
		return;

	CBitmap bm[8];
    bm[0].LoadBitmap(IDB_BITMAP1);   ;//< = 32 * 32, 256 비트맵
    bm[1].LoadBitmap(IDB_BITMAP2);
	bm[3].LoadBitmap(IDB_BITMAP4);
	bm[4].LoadBitmap(IDB_BITMAP3);
	bm[5].LoadBitmap(IDB_BITMAP5);
	bm[2].LoadBitmap(IDB_BITMAP6);
	bm[6].LoadBitmap(IDB_BITMAP9);
	bm[7].LoadBitmap(IDB_BITMAP10);

  //  CImageList img;
    m_ilTree.Create(16, 16, ILC_COLOR8 | ILC_MASK, 1, 8);
    m_ilTree.Add( &bm[0], 0xFF00FF );  ; //< = 비트맵에서 투명처리할꺼
    m_ilTree.Add( &bm[1], 0xFF00FF );
	m_ilTree.Add( &bm[3], 0xFF00FF );
	m_ilTree.Add( &bm[4], 0xFF00FF );
	m_ilTree.Add( &bm[5], 0xFF00FF );
	m_ilTree.Add( &bm[2], 0xFF00FF );
	m_ilTree.Add( &bm[6], 0xFF00FF );
	m_ilTree.Add( &bm[7], 0xFF00FF );
	SetImageList(&m_ilTree, TVSIL_NORMAL);
    m_ilTree.Detach();


//	VERIFY(m_ilTree.Create(m_nilID, 16, 1,0xFF00FF|ILC_COLOR8   ));
	
}

void CTreeOptionsCtrl::OnDestroy() 
{
	//Let the parent class do its thing
	m_ilTree.DeleteImageList(); 
	CTreeCtrl::OnDestroy();
}

void CTreeOptionsCtrl::OnClick(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	//If the mouse was over the label or icon and the item is a combo box
	//or edit box and editing is currently not active then create the 
	//new control
	UINT uFlags=0;
	CPoint point = GetCurrentMessage()->pt;
	ScreenToClient(&point);
	
	HTREEITEM hItem = HitTest(point, &uFlags);

	HTREEITEM hParentItem = GetParentItem(hItem);
	
	if (uFlags & TVHT_ONITEMLABEL || uFlags & TVHT_ONITEMICON)
	{
		HTREEITEM hRoot = GetParentItem( hItem );
		if (ItemHasChildren(hItem) != 0 && hRoot != NULL )
		{
			// Expand current item
			/*HTREEITEM hSide = GetRootItem();
			while (hSide)
			{
				if (hSide != hItem)
				{
					Expand(hSide, TVE_COLLAPSE);
				}		
				hSide = GetNextSiblingItem(hSide);
			}*/
			
			Expand(hItem, TVE_TOGGLE);
		}
		
		GetParent()->SendMessage(WM_MANAGE, (WPARAM)MSG_TREE_CLICK, (LPARAM)hItem);
	}
	else
	{
		if(GetItemText(hItem) == "공시" || GetItemText(hItem) == "언론사별")
		{
			SetChildCheck(hItem);
		}
		else if(GetItemText(hParentItem) == "공시" || GetItemText(hParentItem) == "언론사별")
		{
			AppendChecked(hItem);
		}

		GetParent()->SendMessage(WM_MANAGE, (WPARAM)MSG_TREECHK_CLICK, (LPARAM)hItem);
	}

	*pResult = 0;
}

void CTreeOptionsCtrl::AppendChecked(HTREEITEM hItem)
{
	if(GetCheck(hItem) == FALSE)
	{
		m_selItem.Add(hItem);
	}
	else
	{
		for(int i=0;i<m_selItem.GetSize();i++)
		{
			HTREEITEM hTmp = (HTREEITEM)m_selItem.GetAt(i);

			if(GetItemText(hTmp) == GetItemText(hItem))
			{
				m_selItem.RemoveAt(i);
			}
		}
	}
}

void CTreeOptionsCtrl::SetChildCheck(HTREEITEM parentItem, BOOL bParentCheck, BOOL bCheck)
{
	const BOOL bFlag = GetCheck(parentItem);

	if (ItemHasChildren(parentItem))
	{
	  HTREEITEM hNextItem{};
	  HTREEITEM hChildItem = GetChildItem(parentItem);

	  while (hChildItem != NULL)
	  {
		if((GetCheck(hChildItem) == FALSE && bFlag == FALSE) || (bFlag == TRUE && GetCheck(hChildItem) == TRUE))
		{
			AppendChecked(hChildItem);
		}

		SetCheck(hChildItem,!bFlag);

		hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		hChildItem = hNextItem;
	  }
	}
}

BOOL CTreeOptionsCtrl::IsGroup(HTREEITEM hItem)
{
	int nImage = 0, nSelImage;
	
	GetItemImage(hItem, nImage, nSelImage);

	return (nImage > 1);
}

int CTreeOptionsCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CTreeOptionsCtrl::UnselectCurrentItem()
{
	HTREEITEM item = GetFirstVisibleItem();
	SetItemState( item, 0, TVIS_SELECTED);
}

