// TabCodeSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TabStockDlg.h"
//#include "StdDialog.h"

#define _LocalClass		CTabStockDlg
//@Solomon 위닉스에서 보내는 메시지임.
long _LocalClass::OnWinixNotifyLoad(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString("_LocalClass::OnWinixNotifyLoad\n");
	return 0L;
}

long _LocalClass::OnWinixNotifySave(WPARAM wParam, LPARAM lParam)
{
	//if(!m_pGwanTree || !m_pGwanMng) return 0L;
	if(!m_pGwanMng) return 0L;
	LPCSTR szGwanSim = "관심그룹";
	HTREEITEM hItem = FindTreeItem(szGwanSim);
	if(!hItem) return 0L;

	//PctrWinix.dll의 CWinixGwanItem::CTrCommSite::ReceiveData에서 보내주는 메시지임.
	//관심그룹의 리스트가 변경됨.
	//m_pGwanMng->RequestGwansimGroup();
	CStringArray arrKey, arrName;
	m_pGwanMng->GetGwansimGroupName(arrKey, arrName);

// 	CString rString;
// 	for(int i=0; i<arrName.GetSize(); i++)
// 	{
// 		rString = arrName.GetAt(i);
// 	}
// 	return 0L;

//	m_pGwanTree->DeleteAllItems();

	CTreeCtrl* pTreeCtrl = &m_treeSearch;
	DoDeleteTreeItem(&m_treeSearch, hItem);
	DoClearItemLongDataList();

	CINT_ITEM_ST  OneItem;
	
	int nSize = arrName.GetSize();
	char szNo[3] = {0, };
	INT_GROUP_ST* pIntGroup = NULL;
	CINT_GROUP_ST itemGwan;
	HTREEITEM hTreeParent=hItem;//TVI_ROOT;

	LPCSTR szGroupName, szGroupKey;
	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	{
		szGroupName = (LPCSTR)arrName.GetAt(nIndx);	//관심그룹이름.
		szGroupKey  = (LPCSTR)arrKey.GetAt(nIndx);
		HTREEITEM hTree = pTreeCtrl->InsertItem(szGroupName, hTreeParent);
		CString szInData;

		szInData.Format("%s%s%s", "DRFNGWAN" , ":" , szGroupKey);		
		CItemLongData* pNew = new CItemLongData(szGroupName, "SUB_NO", "USERTR", "GWANGROUP", szInData, "", 0, 0);
		pNew->m_pITrComm = NULL;
		pTreeCtrl->SetItemData(hTree, (DWORD)pNew);

		m_ItemLongDataList.AddTail(pNew);
	}

	return 0L;
}

HTREEITEM _LocalClass::FindTreeItem(LPCSTR szFindStr)
{
	CTreeCtrl* pTreeCtrl = &m_treeSearch;

	HTREEITEM hTreeRoot= pTreeCtrl->GetRootItem();
	if(!hTreeRoot) return NULL;
	
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = hTreeRoot;
	CString szChildText;
	szChildText = pTreeCtrl->GetItemText(hChildItem);
	if(szChildText.CompareNoCase(szFindStr) ==0 )
		return (hChildItem);

	while (hChildItem != NULL)
	{
		hNextItem = pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
		
		szChildText = pTreeCtrl->GetItemText(hChildItem);
		if(szChildText.CompareNoCase(szFindStr) ==0 )
			 return (hChildItem);
		
		hChildItem = hNextItem;
	}
	return NULL;
}

void _LocalClass::DoDeleteTreeItem(CTreeCtrl* pTreeCtrl, HTREEITEM hRoot)
{
	if(!hRoot) return;

	HTREEITEM hNextItem;
	HTREEITEM hChildItem = pTreeCtrl->GetChildItem(hRoot);
	CString szChildText;

	while (hChildItem != NULL)
	{
		hNextItem = pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);

		szChildText = pTreeCtrl->GetItemText(hChildItem);
		pTreeCtrl->DeleteItem(hChildItem);

		hChildItem = hNextItem;
	}
}

void _LocalClass::DoClearItemLongDataList()
{
	for(POSITION pos=m_ItemLongDataList.GetHeadPosition(); pos; )
		delete m_ItemLongDataList.GetNext(pos);
	m_ItemLongDataList.RemoveAll();
}

long _LocalClass::OnWinixNotifyGroup(WPARAM wParam, LPARAM lParam)
{
// 	if(wParam==999 && lParam==999)
// 	{
// 		SendMessage(UM_CHANGE_REGKW, 0, 0);
// 		//CStringArray arrKey, arrName;
// 		//m_pGwanMng->GetGwansimGroupName(arrKey, arrName);
// 		return 0L;
// 	}

	CString szGroup;
	CStringList szItemList;
	m_pGwanMng->ParseGwanList(wParam, lParam, szGroup, szItemList);

	if(szItemList.GetCount()>0)
	{
		IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);

		CString szItem;
		LIST_CCodeTypeA ListTypeA; 
		for(POSITION pos=szItemList.GetHeadPosition(); pos;)
		{
			szItem = szItemList.GetNext(pos);
			CCodeTypeA typeA;
			//typeA.m_cGb = codeInfo.gb;
			typeA.m_szcode = szItem;
			typeA.m_szhname = pMasterDataManager->GetCodeName(szItem);
			if(typeA.m_szhname.IsEmpty()) continue;	//종목명 없는것은 추가하지 않는다.

			ListTypeA.AddTail(typeA);
		}
		m_xTLMCallback.ReceiveData(0, 'A', 0, ListTypeA.GetCount(), (void*)&ListTypeA);
	}

	return 0L;
}
long _LocalClass::OnWinixNotifyGroupList(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString("_LocalClass::OnWinixNotifyGroupList\n");
	return 0L;
}
long _LocalClass::OnWinixNotifySaveCode(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString("_LocalClass::OnWinixNotifySaveCode\n");
	return 0L;
}
