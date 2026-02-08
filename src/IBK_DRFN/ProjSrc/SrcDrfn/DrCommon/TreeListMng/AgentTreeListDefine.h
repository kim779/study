#ifndef  __TREELIST_DEFINE_HEADER__BY_JUNOK_LEE_20060107__
#define  __TREELIST_DEFINE_HEADER__BY_JUNOK_LEE_20060107__

// #include "./TreeListDefine.h"

#define FN_Callback_Master(xxClass, xxMasterName, xxC)	void CTreeListManager::Callback_##xxClass(LIST_CCodeTypeA* pListTypeA)				\
	{																														\
		if(m_pMDMng)																										\
		{																													\
			CList<xxClass, xxClass>* list = (CList<xxClass, xxClass>*)m_pMDMng->GetCodePointer(xxMasterName);	\
			if(!list)																										\
			{																												\
				return;																										\
			}																												\
			for(POSITION pos=list->GetHeadPosition(); pos;)																	\
			{																												\
				xxClass data = list->GetNext(pos);																			\
				CCodeTypeA typeA;																							\
				typeA.m_cGb = xxC;																							\
				typeA.m_szcode = data.m_szCode;																				\
				typeA.m_szhname = data.m_szName;																			\
				pListTypeA->AddTail(typeA);																					\
			}																												\
		}																													\
	}

#define FN_LoadTree_SubMaster(xxClass,xxMasterName,xxMid)	int CTreeListManager::LoadTree_##xxClass(CTreeResItem* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1)	\
	{																														\
		if(m_pMDMng)																										\
		{																													\
			char aKeyA[5+1]={0,}; memcpy(aKeyA, _szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;				\
			CList<xxClass, xxClass>* list = (CList<xxClass, xxClass>*)m_pMDMng->GetCodePointer(xxMasterName); 				\
			if(!list) return 0;																								\
			for(POSITION pos=list->GetHeadPosition(); pos;)																	\
			{																												\
				xxClass data = list->GetNext(pos);																			\
				HTREEITEM hTree = pTree->InsertItem(data.m_szName, hParentTree);											\
				sprintf(aKeyB, "B%04.4s", data.m_szCode.Mid(xxMid));														\
				szInputData.Format("%s%s", aKeyA, aKeyB);																	\
				CItemLongData* pNew = new CItemLongData(data.m_szName, "SUB_NO", "TR", "NONE", szInputData, szTr, dwKey1, 0);	\
				pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);						\
				pList->m_pItemList->AddTail(pNew);																			\
			}																												\
			return list->GetCount();																						\
		}																													\
		return 0;																											\
	}



/*
void CTreeListManager::Callback_ETFMaster(LIST_CCodeTypeA* pListTypeA)				
{																														
	if(m_pMDMng)																										
	{																													
		CString szMster("ETFMASTER");
		CList<CETFMaster, CETFMaster>* list = (CList<CETFMaster, CETFMaster>*)m_pMDMng->GetCodePointer(szMster);	
		if(!list)																										
		{																												
			return;																										
		}																												
		for(POSITION pos=list->GetHeadPosition(); pos;)																	
		{																												
			CETFMaster data = list->GetNext(pos);																		
			CCodeTypeA typeA;																							
			typeA.m_szcode = data.m_szCode;																				
			typeA.m_szhname = data.m_szName;																			
			pListTypeA->AddTail(typeA);																					
		}																												
	}																													
}

int CTreeListManager::LoadUMASTER(CTreeCtrl* pTree, HTREEITEM hParentTree)
{
	if(m_pMDMng)
	{
		CList<CUMaster, CUMaster>* list = 
			(CList<CUMaster, CUMaster>*)m_pMDMng->GetCodePointer("UMASTER"); 

		if(!list) return 0;
		for(POSITION pos=list->GetHeadPosition(); pos;)
		{
			CUMaster data = list->GetNext(pos);
			TRACE("%s[%s]\n", data.m_szCode, data.m_szName);
			pTree->InsertItem(data.m_szName, hParentTree);
		}
	}

	return 0;	
}

int CTreeListManager::LoadKUMASTER(CTreeCtrl* pTree, HTREEITEM hParentTree)
{
	if(m_pMDMng)
	{
		CList<CKUMaster, CKUMaster>* list = 
			(CList<CKUMaster, CKUMaster>*)m_pMDMng->GetCodePointer("KUMASTER"); 

		if(!list) return 0;
		for(POSITION pos=list->GetHeadPosition(); pos;)
		{
			CKUMaster data = list->GetNext(pos);
			TRACE("%s[%s]\n", data.m_szCode, data.m_szName);
			pTree->InsertItem(data.m_szName, hParentTree);
		}
	}

	return 0;	
}

*/

#endif // __TREELIST_DEFINE_HEADER__BY_JUNOK_LEE_20060107__
