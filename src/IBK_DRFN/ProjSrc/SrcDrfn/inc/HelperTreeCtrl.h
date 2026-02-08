#if !defined(AFX_HELPER_OF_TREECTRL_HEADER__EE99FC62AAF__INCLUDED_)
#define AFX_HELPER_OF_TREECTRL_HEADER__EE99FC62AAF__INCLUDED_

//#include "../../_Include/HelperTreeCtrl.h"

class CHelperTreeCtrl
{
public:
	static void DeleteTreeChildInfo(CTreeCtrl* pTree, HTREEITEM hParentTree);
	static HTREEITEM FindTreeItem(CTreeCtrl* pTree, LPCSTR szItemName, HTREEITEM hStartItem=NULL);
	static HTREEITEM FindTreeItemEx(CTreeCtrl* pTree, LPCSTR szItemName, HTREEITEM hStartItem, BOOL bHaveChild);
};

inline void CHelperTreeCtrl::DeleteTreeChildInfo(CTreeCtrl* pTree, HTREEITEM hParentTree)
{
	// Delete all of the children of hmyItem.
	if (pTree->ItemHasChildren(hParentTree))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = pTree->GetChildItem(hParentTree);
		CItemLongData* pData=NULL;
		while (hChildItem != NULL)
		{
			pData = (CItemLongData*)pTree->GetItemData(hChildItem);
			
			pTree->DeleteItem(hChildItem);
			hNextItem = pTree->GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
	}
}

inline HTREEITEM CHelperTreeCtrl::FindTreeItem(CTreeCtrl* pTree, LPCSTR szItemName, HTREEITEM hStartItem)
{
	if(!pTree ||!pTree->IsKindOf(RUNTIME_CLASS(CTreeCtrl)) || !pTree->m_hWnd) return NULL;

	HTREEITEM hRoot = pTree->GetRootItem();;
	if(hStartItem!=NULL)
	{
		hRoot = hStartItem;
		if (pTree->ItemHasChildren(hStartItem))
		{
			HTREEITEM hChildItem = pTree->GetChildItem(hStartItem);
			hRoot = hChildItem;
		}
		else
		{
			return hStartItem;
		}
		if(!hRoot) return NULL;
	}

//	if (pTree->ItemHasChildren(hRoot))
	{
		HTREEITEM hNextItem;
//		HTREEITEM hChildItem = pTree->GetChildItem(hRoot);
		HTREEITEM hChildItem = hRoot;
		CString szItemText;
//		szItemText = pTree->GetItemText(hChildItem);
//		if(!szItemText.Compare(szItemName))
//		{
//			return hChildItem;
//		}
		while (hChildItem != NULL)
		{
			szItemText = pTree->GetItemText(hChildItem);
			if(!szItemText.Compare(szItemName))
			{
				return hChildItem;
			}
			hNextItem = pTree->GetNextItem(hChildItem, TVGN_NEXT);						
			hChildItem = hNextItem;
		}
	}
	return NULL;
}

inline HTREEITEM CHelperTreeCtrl::FindTreeItemEx(CTreeCtrl* pTree, LPCSTR szItemName, HTREEITEM hStartItem, BOOL bHaveChild)
{
	if(!pTree ||!pTree->IsKindOf(RUNTIME_CLASS(CTreeCtrl)) || !pTree->m_hWnd) return NULL;

	HTREEITEM hRoot = pTree->GetRootItem();;
	if(hStartItem!=NULL)
	{
		hRoot = hStartItem;
		if (pTree->ItemHasChildren(hStartItem))
		{
			HTREEITEM hChildItem = pTree->GetChildItem(hStartItem);
			hRoot = hChildItem;
		}
		else
		{
			return hStartItem;
		}
		if(!hRoot) return NULL;
	}

//	if (pTree->ItemHasChildren(hRoot))
	{
		HTREEITEM hNextItem;
//		HTREEITEM hChildItem = pTree->GetChildItem(hRoot);
		HTREEITEM hChildItem = hRoot;
		CString szItemText;
//		szItemText = pTree->GetItemText(hChildItem);
//		if(!szItemText.Compare(szItemName))
//		{
//			return hChildItem;
//		}
		while (hChildItem != NULL)
		{
			szItemText = pTree->GetItemText(hChildItem);
			if(!szItemText.Compare(szItemName))
			{
				if(bHaveChild)
				{
					if(pTree->ItemHasChildren(hChildItem))
					{
						return hChildItem;
					}
				}
				else
					return hChildItem;
			}
			hNextItem = pTree->GetNextItem(hChildItem, TVGN_NEXT);						
			hChildItem = hNextItem;
		}
	}
	return NULL;
}

#endif //AFX_HELPER_OF_TREECTRL_HEADER__EE99FC62AAF__INCLUDED_
