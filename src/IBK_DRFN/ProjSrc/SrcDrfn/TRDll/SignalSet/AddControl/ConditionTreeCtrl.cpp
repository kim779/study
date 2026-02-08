// ConditionTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "ConditionTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int m_nTreeIconIndex0St = 0;
const int m_nTreeIconIndex1St = 2;
const int m_nTreeIconIndex2St = 4;
const int m_nTreeIconIndexNormal = 6;		//#<= Normal
const int m_nTreeIconIndexNonAlert = 7;	//#<= Alert



/////////////////////////////////////////////////////////////////////////////
// CConditionTreeCtrl

CConditionTreeCtrl::CConditionTreeCtrl()
{
}

CConditionTreeCtrl::~CConditionTreeCtrl()
{ 
  m_imglistTree.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CConditionTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CConditionTreeCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConditionTreeCtrl message handlers

int CConditionTreeCtrl::Create(CWnd* pParentWnd, UINT nID) 
{
	int nResult = -1;

	nResult = CTreeCtrl::Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | TVS_DISABLEDRAGDROP | 
															TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_NOTOOLTIPS,// | 
															//TVS_CHECKBOXES,
												CRect(0, 0, 0, 0),
												pParentWnd,	
												nID);
	CTreeCtrl::ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	SetIconLoad();

	return nResult;
}


void CConditionTreeCtrl::SetIconLoad()
{
	m_imglistTree.DeleteImageList();
	m_imglistTree.Create(16, 16, ILC_COLOR8 | ILC_MASK, 3, 3);

	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE00));	//# 0 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE01));
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE10));	//# 1 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE11));	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE10));	//# 2 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE11));	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NORMAL));		//# 일반
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NONALERT));	//# Non Alert
		
	SetImageList(&m_imglistTree, TVSIL_NORMAL);
}


int CConditionTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}


BOOL CConditionTreeCtrl::SetInit(CList <ConditionDataGroup, ConditionDataGroup&> *plistGroup)
{
	BOOL bResult = TRUE;

	m_nConditionCnt = 0;

	HTREEITEM htiRoot, htiRoot_Sub, htiRoot_Item;
	htiRoot = GetRootItem();

	POSITION posGroup, posItem;
	ConditionDataGroup stConditionDataGroup;

	int nIndex, nIndex_Sub;
// 	try {
// 		for (nIndex = 0; nIndex < plistGroup->GetCount(); nIndex++)
// 		{	
// 			posGroup = plistGroup->FindIndex(nIndex);
// 			stConditionDataGroup = plistGroup->GetAt(posGroup);
// 
// 			if(stConditionDataGroup.strGroup == "순간체결량")
// 				continue;
// 			htiRoot_Sub = SetTreeItem(htiRoot, stConditionDataGroup.strGroup, 
// 																m_nTreeIconIndex0St, m_nTreeIconIndex0St + 1, (long)posGroup);
// 			
// 			m_nConditionCnt += stConditionDataGroup.plistData->GetCount();
//       
// 			//# Add Tree Child!
// 			ConditionData		stConditionData;
// 			for (nIndex_Sub = 0; nIndex_Sub < stConditionDataGroup.plistData->GetCount(); nIndex_Sub++)
// 			{
// 				posItem = stConditionDataGroup.plistData->FindIndex(nIndex_Sub);
// 				stConditionData = stConditionDataGroup.plistData->GetAt(posItem);
// 
// 				htiRoot_Item = SetTreeItem(htiRoot_Sub, stConditionData.strTitle, m_nTreeIconIndexNormal, m_nTreeIconIndexNormal, stConditionData.nIndex);
// 			}
// 		}
// 	} catch( CException e )
// 	{
// 		bResult = FALSE;
// 	}

	for (nIndex = 0; nIndex < plistGroup->GetCount(); nIndex++)
	{	
		posGroup = plistGroup->FindIndex(nIndex);
		if(!posGroup)
		{
			return FALSE;
		}
			
		stConditionDataGroup = plistGroup->GetAt(posGroup);

		if(stConditionDataGroup.strGroup == "순간체결량")
			continue;
		htiRoot_Sub = SetTreeItem(htiRoot, stConditionDataGroup.strGroup, 
			m_nTreeIconIndex0St, m_nTreeIconIndex0St + 1, (long)posGroup);

		m_nConditionCnt += stConditionDataGroup.plistData->GetCount();

		//# Add Tree Child!
		ConditionData		stConditionData;
		for (nIndex_Sub = 0; nIndex_Sub < stConditionDataGroup.plistData->GetCount(); nIndex_Sub++)
		{
			posItem = stConditionDataGroup.plistData->FindIndex(nIndex_Sub);
			if(!posItem)
			{
				return FALSE;
			}

			stConditionData = stConditionDataGroup.plistData->GetAt(posItem);

			htiRoot_Item = SetTreeItem(htiRoot_Sub, stConditionData.strTitle, m_nTreeIconIndexNormal, m_nTreeIconIndexNormal, stConditionData.nIndex);
		}
	}

	m_plistGroup = plistGroup;

	return bResult;
}


HTREEITEM CConditionTreeCtrl::SetTreeItem(HTREEITEM htiParent, CString strItem, 
																		int nImgIndex, int nImgSelIndex, int lParam) 
{
	TV_INSERTSTRUCT tvstruct;
	
	tvstruct.hParent = htiParent;
	tvstruct.hInsertAfter = TVI_LAST;
		
	tvstruct.item.pszText = (LPTSTR)(LPCSTR)strItem;	
		
	tvstruct.item.lParam = (long)lParam;
	tvstruct.item.iImage = nImgIndex;
	tvstruct.item.iSelectedImage = nImgSelIndex;
	tvstruct.item.mask = TVIF_TEXT | TVIF_PARAM |TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	HTREEITEM hItem = InsertItem(&tvstruct);
	Expand(hItem, TVE_COLLAPSE);
	UpdateWindow();
		
	return hItem; 
}

void CConditionTreeCtrl::SetItemAllView(BOOL bAllItemView)
{
	HTREEITEM	htiRoot;
	htiRoot = GetRootItem();
	
	if (bAllItemView == m_bAllItemView)
		return;

	while (htiRoot != NULL)
	{
		if (bAllItemView == TRUE)
			Expand(htiRoot, TVE_EXPAND);
		else
			Expand(htiRoot, TVE_COLLAPSE);

		SetItemChildView(htiRoot, bAllItemView);
		htiRoot = GetNextSiblingItem(htiRoot);
	}

	m_bAllItemView = bAllItemView;
}

void CConditionTreeCtrl::SetItemChildView(HTREEITEM hItem, BOOL bAllItemView)
{
	HTREEITEM hChild = GetChildItem(hItem);
	while (hChild != NULL)
	{
		if (bAllItemView == TRUE)
			Expand(hChild, TVE_EXPAND);
		else
			Expand(hChild, TVE_COLLAPSE);

		if (ItemHasChildren(hChild) == TRUE)
			SetItemChildView(hChild, bAllItemView);

		hChild = GetNextItem(hChild, TVGN_NEXT);
	}
}

BOOL CConditionTreeCtrl::GetData_SelItem(ConditionData	&stConditionData)
{
	stConditionData = m_stSelectData;
	return m_bSelState;
}

BOOL CConditionTreeCtrl::GetData_Item(ConditionData	&stConditionData)
{
	BOOL bResult = FALSE;

	TVITEM item;
  TCHAR szText[1024];
  item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
  item.pszText = szText;
  item.cchTextMax = 1024;

	item.hItem = m_hSelItem;

	int nSelect_Index;
	if (GetItem(&item) == TRUE)
		nSelect_Index = (int)item.lParam;
	else
		return FALSE;

	POSITION posGroup, posItem;
	ConditionDataGroup stConditionDataGroup;

	int nIndex, nIndex_Sub;
	for (nIndex = 0; nIndex < m_plistGroup->GetCount(); nIndex++)
	{	
		posGroup = m_plistGroup->FindIndex(nIndex);
		stConditionDataGroup = m_plistGroup->GetAt(posGroup);
		
		for (nIndex_Sub = 0; nIndex_Sub < stConditionDataGroup.plistData->GetCount(); nIndex_Sub++)
		{
			posItem = stConditionDataGroup.plistData->FindIndex(nIndex_Sub);
			stConditionData = stConditionDataGroup.plistData->GetAt(posItem);

			if (nSelect_Index == stConditionData.nIndex)
			{
				bResult = TRUE;
				break;
			}
		}

		if (bResult == TRUE)
			break;
	}

	return bResult;
}


void CConditionTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	/*
	ConditionData	stConditionData;
	if (GetData_Item(stConditionData) == TRUE)
		GetParent()->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_ITEM_DBCLK, (LPARAM)&stConditionData);
	*/

	if (m_bSelState == TRUE)
		GetParent()->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_ITEM_DBCLK, (LPARAM)&m_stSelectData);
	
	*pResult = 0;
}

void CConditionTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	HTREEITEM hSelItem = GetSelectedItem();

	if ((hSelItem != NULL) && (m_hSelItem != hSelItem))
	{
		SelectItem(hSelItem);

		m_hSelItem = hSelItem;
		m_bSelState= GetData_Item(m_stSelectData);

		if( !m_bSelState)
		{
			m_stSelectData.nIndex = 0;    
			m_stSelectData.strTitle = ""; 
			m_stSelectData.nBasePoint = 0;
			m_stSelectData.strCode = "";  
			m_stSelectData.strPath = "";  
		}

		GetParent()->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_ITEM_CLK, (LPARAM)&m_stSelectData);
	}

	*pResult = 0;
}

void CConditionTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);

	if ((hSelItem != NULL) && (m_hSelItem != hSelItem))
	{
		if (TVHT_ONITEM & uFlags)
		{
			SelectItem(hSelItem);

			m_hSelItem = hSelItem;
			m_bSelState= GetData_Item(m_stSelectData);

			if( !m_bSelState)
			{
				m_stSelectData.nIndex = 0;    
				m_stSelectData.strTitle = ""; 
				m_stSelectData.nBasePoint = 0;
				m_stSelectData.strCode = "";  
				m_stSelectData.strPath = "";  
			}

			GetParent()->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_ITEM_CLK, (LPARAM)&m_stSelectData);
		}
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}


void CConditionTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
		
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

/*
void CConditionTreeCtrl::GetTreeList(CList <ProfferInfo, ProfferInfo&> *plistProfferInfo)
{
	BOOL bCheck;
	HTREEITEM hParent, hItem;

	TVITEM item;
  TCHAR szText[1024];
  item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
  item.pszText = szText;
  item.cchTextMax = 1024;
	
	BOOL bWorked;
	POSITION pos;
	ProfferInfo	stProfferInfo;
	int nIndex;

	hParent = GetRootItem();
	while (hParent != NULL)
	{
		item.hItem = hParent;
		bWorked = GetItem(&item);
		if (bWorked == TRUE)
		{
			pos = (POSITION)item.lParam;
			stProfferInfo = plistProfferInfo->GetAt(pos);
		}
		else
			continue;

		bCheck = GetCheck(hParent);
		if (bCheck == TRUE)
		{
			//# All Check!
			stProfferInfo.nAllCheck = 1;

			if (stProfferInfo.bHaveChild == TRUE)
			{
				for (nIndex = 0; nIndex < stProfferInfo.piaType_User->GetSize(); nIndex++)
					stProfferInfo.piaType_User->SetAt(nIndex, 1);
			}
		}
		else
		{
			if (stProfferInfo.nChildView == 0)
			{
				stProfferInfo.nAllCheck = 0;
				if (stProfferInfo.bHaveChild == TRUE)
				{
					for (nIndex = 0; nIndex < stProfferInfo.piaType_User->GetSize(); nIndex++)
						stProfferInfo.piaType_User->SetAt(nIndex, 0);
				}
			}
			else
			{
				stProfferInfo.nAllCheck = 0;

				hItem = GetChildItem(hParent);
				while (hItem != NULL)
				{
					bCheck = GetCheck(hItem);

					//# Check Apply!
					item.hItem = hItem;
					bWorked = GetItem(&item);
					int nTypeID;
					if (bWorked == TRUE)
					{
						nTypeID = (int)item.lParam;
						for (nIndex = 0; nIndex < stProfferInfo.psaType_ID->GetSize(); nIndex++)
						{
							if (atoi(stProfferInfo.psaType_ID->GetAt(nIndex)) == (int)nTypeID)
							{
								stProfferInfo.piaType_User->SetAt(nIndex, bCheck);
								break;
							}
						}
					}
					else
						continue;
					
					hItem = GetNextSiblingItem(hItem);
				}
			}
		}

		plistProfferInfo->SetAt(pos, stProfferInfo);
		hParent = GetNextSiblingItem(hParent);
	}
}
*/

void CConditionTreeCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CTreeCtrl::OnShowWindow(bShow, nStatus);
	
	UpdateWindow();
}

