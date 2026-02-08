// TabCodeSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TabCodeSearch.h"
//#include "StdDialog.h"

//@Solomon 위닉스에서 보내는 메시지임.
long CTabCodeSearch::OnWinixNotifyLoad(WPARAM wParam, LPARAM lParam)
{
//	OutputDebugString("CTabCodeSearch::OnWinixNotifyLoad\n");
	return 0L;
}

long CTabCodeSearch::OnWinixNotifySave(WPARAM wParam, LPARAM lParam)
{
	if (!m_pGwanTree || !m_pGwanMng) return 0L;

	//PctrWinix.dll의 CWinixGwanItem::CTrCommSite::ReceiveData에서 보내주는 메시지임.
	//관심그룹의 리스트가 변경됨.
	//m_pGwanMng->RequestGwansimGroup();
	CStringArray arrKey, arrName;
#if(0)//tour2k
	m_pGwanMng->GetGwansimGroupName(arrKey, arrName);
#else
        m_pGwanMng->ParseGroup( lParam, arrKey, arrName);
#endif
// 	CString rString;
// 	for(int i=0; i<arrName.GetSize(); i++)
// 	{
// 		rString = arrName.GetAt(i);
// 	}
// 	return 0L;

//	m_pGwanTree->DeleteAllItems();
	CString szRemain(_T("히스토리"));
	DoDeleteTreeItem(m_pGwanTree, szRemain);
	DoClearItemLongDataList();

	CINT_ITEM_ST  OneItem;
	
	int nSize = arrName.GetSize();
	char szNo[3] = {0, };
	INT_GROUP_ST* pIntGroup = NULL;
	CINT_GROUP_ST itemGwan;
	HTREEITEM hTreeParent=TVI_ROOT;

	LPCSTR szGroupName, szGroupKey;
	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	{
		szGroupName = (LPCSTR)arrName.GetAt(nIndx);	//관심그룹이름.
		szGroupKey  = (LPCSTR)arrKey.GetAt(nIndx);
		HTREEITEM hTree = m_pGwanTree->InsertItem(szGroupName, hTreeParent);
		CString szInData;

		szInData.Format("%s%s%s", "DRFNGWAN" , ":" , szGroupKey);		
		CItemLongData* pNew = new CItemLongData(szGroupName, "SUB_NO", "USERTR", "GWANGROUP", szInData, "", 0, 0);
		pNew->m_pITrComm = NULL;
		m_pGwanTree->SetItemData(hTree, (DWORD)pNew);

		m_ItemLongDataList.AddTail(pNew);
	}
	//TOUR2K
	m_pTreeListMng->LoadAccountAfter(m_pGwanTree, hTreeParent);

	return 0L;
}

void CTabCodeSearch::DoDeleteTreeItem(CTreeCtrl* pTreeCtrl, CString& szRemain)
{
	HTREEITEM hTreeRoot= pTreeCtrl->GetRootItem();
	if(!hTreeRoot) return;

	HTREEITEM hNextItem;
	HTREEITEM hChildItem = hTreeRoot;//pTreeCtrl->GetChildItem(hTreeRoot);
	CString szChildText;

	while (hChildItem != NULL)
	{
		hNextItem = pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);

		szChildText = pTreeCtrl->GetItemText(hChildItem);
		if(szChildText.CompareNoCase(szRemain) !=0 )
			pTreeCtrl->DeleteItem(hChildItem);

		hChildItem = hNextItem;
	}
}

void CTabCodeSearch::DoClearItemLongDataList()
{
	for(POSITION pos=m_ItemLongDataList.GetHeadPosition(); pos; )
		delete m_ItemLongDataList.GetNext(pos);
	m_ItemLongDataList.RemoveAll();
}

long CTabCodeSearch::OnWinixNotifyGroup(WPARAM wParam, LPARAM lParam)
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
long CTabCodeSearch::OnWinixNotifyGroupList(WPARAM wParam, LPARAM lParam)
{
//	OutputDebugString("CTabCodeSearch::OnWinixNotifyGroupList\n");
	return 0L;
}
long CTabCodeSearch::OnWinixNotifySaveCode(WPARAM wParam, LPARAM lParam)
{
//	OutputDebugString("CTabCodeSearch::OnWinixNotifySaveCode\n");
	return 0L;
}
