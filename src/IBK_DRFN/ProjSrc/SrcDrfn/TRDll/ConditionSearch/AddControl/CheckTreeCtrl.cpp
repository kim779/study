// CheckTreeCtrl.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Winsock2.h"
#include "CheckTreeCtrl.h"

#include "../MainFrmWnd.h"
#include "../ConditionResultWnd.h"


#define		WMU_JANGO_CTRLDATADISP		(WM_USER+1010)

#define TREE_MARKET				0
#define TREE_PORTFOLIO			1
#define TREE_RECOMMENDATION		2

LPCSTR ROOT_TITLE_MARKET[] = {_T("전체"), _T("거래소"), _T("코스닥"), _T("KOSPI200"), 
/*_T("KOSTAR"),*/ _T("KRX100")};
LPCSTR TYPE_DATA_MARKET[] = {"I", "M", "M", "M", /*"M",*/ "M"};

LPCSTR ROOT_TITLE_PORT[] = {_T("관심종목"), _T("보유종목")};
LPCSTR TYPE_DATA_PORT[] = {"P", "H"};

LPCSTR ROOT_TITLE_RECOMMENDATION[] = {_T("테마그룹")};
LPCSTR TYPE_DATA_RECOMMENDATION[] = {"T"};

const UINT OFFSET_DATA[] = {100, 200, 300, 400, 500, 600};

/*--------------------------------
	거래소		000
	코스닥		200
	KOSPI200	2001
	KOSTAR		3001
	KRX100		4001
---------------------------------*/
const LPCSTR TYPE_MARKETCODE[] = {_T("1000"), _T("000"), _T("200"), _T("100"),/* _T("3001"),*/ _T("900")};

const LPCSTR TYPE_CODE[] = {_T("")};

// CCheckTreeCtrl

IMPLEMENT_DYNAMIC(CCheckTreeCtrl, CTreeCtrl)
CCheckTreeCtrl::CCheckTreeCtrl()
{
//	m_pSaSelectedItem = NULL;
	m_pReWnd = NULL;
}

CCheckTreeCtrl::~CCheckTreeCtrl()
{
	POSITION pos;
	TreeItem stTreeItem;
	int nKey;
	for(pos = m_mapTreeItem.GetStartPosition(); pos != NULL;)
	{
		m_mapTreeItem.GetNextAssoc(pos, nKey, stTreeItem);
		m_mapTreeItem.RemoveKey(nKey);
	}
	m_mapTreeItem.RemoveAll();
}


BEGIN_MESSAGE_MAP(CCheckTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCheckTreeCtrl 메시지 처리기입니다.

void CCheckTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	UINT uFlags;
	GetCursorPos(&point);
	ScreenToClient(&point);

	HTREEITEM hItem = HitTest(point, &uFlags);

	if((hItem != NULL) && (uFlags & TVHT_ONITEMSTATEICON))
	{	
		if(ItemHasChildren(hItem))
		{
			BOOL bCheck;
			bCheck = GetCheck(hItem);

			SetCheckChildren(hItem, !bCheck);

			UpdateParentState(hItem);
		}
		else
		{
			UpdateParentState(hItem);			
		}
		SelectItem(hItem);		
	}
	else if((hItem != NULL) && (uFlags & TVHT_ONITEMLABEL))
	{
		SetSelectItem(hItem);
	}
	else
	{
		TRACE("INVALID\n");
	}

	if(GetItemText(hItem) == "전체")
	{
		HTREEITEM hParent = NULL;

		if(!GetCheck(hItem)) // 비선택 -> 선택
		{
			for(int i = 1; m_htiMarket[i] != NULL; i++)
			{
				hParent = m_htiMarket[i];
				CString strTest;
				strTest = GetItemText(hParent);
				HTREEITEM hChilde = GetChildItem(hParent);
				do{
					SetCheck(hChilde, TRUE);
					hChilde = GetNextSiblingItem(hChilde);
				}while(hChilde);

				SetCheck(hParent, TRUE);
			}
		}
		else // 선택 -> 비선택
		{
			for(int i = 1; m_htiMarket[i] != NULL; i++)
			{
				HTREEITEM hParent = m_htiMarket[i];
				
				if(GetCheck(hParent))
					SetCheck(hParent, FALSE);
				if(IsCheckAnyChild(hParent))
				{
					HTREEITEM hChilde = GetChildItem(hParent);
					while(hChilde)
					{
						if(GetCheck(hChilde))
							SetCheck(hChilde, FALSE);
						hChilde = GetNextSiblingItem(hChilde);
					}
				}
			}
		}
	}
	else if(GetItemText(m_htiMarket[0]) == "전체") 
	{
		if(GetCheck(m_htiMarket[0]))	// 전체가 클릭 되어 있는 상태에서 다른 노드 선택 -> 전체 해제
			SetCheck(m_htiMarket[0], FALSE);
	}

// 	if(GetItemText(m_htiMarket[0]) == "전체") // 모든 항목 사용자가 선택했을 시 -> 선택 해제 후 전체만 선택
// 	{
// 		BOOL bAllCheck = TRUE;
// 		for(int i = 1; m_htiMarket[i] != NULL; i++)
// 		{
// 			CString strTest = GetItemText(m_htiMarket[i]);
// 			if(!GetCheck(m_htiMarket[i]))
// 			{	
// 				bAllCheck = FALSE;
// 				break;
// 			}	
// 		}
// 
// 		if(bAllCheck)
// 		{
// 			for(int i = 1; m_htiMarket[i] != NULL; i++)
// 				SetCheck(m_htiMarket[i], FALSE);
// 			SetCheck(m_htiMarket[0], TRUE);
// 		}
// 	}

	*pResult = 0;
}

void CCheckTreeCtrl::SetSelectItem(HTREEITEM hItem)
{
	HTREEITEM hParent = GetParentItem(hItem);
	
	CDlgEditSubject* pParent = (CDlgEditSubject*)GetParent();
	CString strTemp;
	
	CString strText = GetItemText(hItem);
	CString strPText = GetItemText(hParent);
	if (strPText == "테마그룹")
	{
		CString strTemme;
		strTemp.Format("%s", GetItemText(hItem));
		strTemme = strTemp.Mid(2, 3);

		pParent->GetItemCodeFromThemeGroup(strTemme);	
	}
	else if (strPText == "보유종목")
	{
		strTemp.Format("%s", GetItemMaketCode(hItem));

		pParent->RequestHaveItem(strTemp, GetItemText(hItem));
	}
	else
	{
		if (strText == "관심종목" || strText == "보유종목" || strText == "테마그룹")
			pParent->AllClear();
		else
		{
			strTemp.Format("%s", GetItemMaketCode(hItem));

			pParent->GetStockMasterData(strTemp, GetItemText(hItem), GetItemText(hParent));
		}
	}
	m_strParentItemText = GetItemText(hParent);
	m_strSelectItemText = GetItemText(hItem);

	SelectItem(hItem);
}

void CCheckTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
// 	UINT uFlags;
// 
// 	HTREEITEM hItem = HitTest(point, &uFlags);
// 
// 	HTREEITEM hParent = GetParentItem(hItem);
// 
// 	if((hItem != NULL) && (uFlags & TVHT_ONITEMLABEL))
// 	{
// 		CDlgEditSubject* pParent = (CDlgEditSubject*)GetParent();
// 		
// 		CString strTemp;
// 
// 		if(GetItemText(hParent) == "테마그룹")
// 		{
// 			CString strTemme;
// 			strTemp.Format("%s", GetItemText(hItem));
// 			strTemp.Mid(1, 4);
// 			strTemme = strTemp;
// 		}
// 		
// 		strTemp.Format("%s", GetItemMaketCode(hItem));
// 		pParent->GetStockMasterData(strTemp);
//	}
}

void CCheckTreeCtrl::SetCheckChildren(HTREEITEM hItem, BOOL bCheck)
{
    hItem = GetChildItem(hItem);
    while(hItem != NULL)
    {
		CString szText = GetItemText(hItem);

        SetCheckChildren(hItem, bCheck);

		SetCheck(hItem, bCheck);

        hItem = GetNextSiblingItem(hItem);
    }
}

BOOL CCheckTreeCtrl::IsCheckAllChild(HTREEITEM hItem)
{
	BOOL bChecked = FALSE;

	if(!ItemHasChildren(hItem))
	{
		bChecked = GetCheck(hItem);
	}
	else
	{
		bChecked = TRUE;
		hItem = GetChildItem(hItem);
		
		while(hItem != NULL && bChecked)
		{
			bChecked = IsCheckAllChild(hItem);
			if(bChecked)
			{
				bChecked = GetCheck(hItem);
				hItem = GetNextSiblingItem(hItem);
			}
			else
			{		
				break;
			}
		}
	}

	return bChecked;
}

BOOL CCheckTreeCtrl::IsCheckAnyChild(HTREEITEM hItem)
{
	BOOL bChecked = FALSE;

	if(!ItemHasChildren(hItem))
	{
		bChecked = GetCheck(hItem);
	}
	else
	{
		bChecked = FALSE;
		hItem = GetChildItem(hItem);
		
		while(hItem != NULL && !bChecked)
		{
			bChecked = IsCheckAnyChild(hItem);
			if(bChecked)
			{
				break;
			}
			else
			{
				bChecked = GetCheck(hItem);
				hItem = GetNextSiblingItem(hItem);
			}			
		}
	}

	return bChecked;
}

void CCheckTreeCtrl::UpdateParentState(HTREEITEM hItem)
{
	HTREEITEM hParent;
	HTREEITEM hCurrent = hItem;

	BOOL bCheck = GetCheck(hCurrent);
	SetCheck(hCurrent, !bCheck);
	
	while(hParent = GetParentItem(hCurrent))
	{		
		if(IsCheckAllChild(hParent))
		{			
			SetCheck(hParent, TRUE);
		}
		else
		{		
			SetCheck(hParent, FALSE);
		}		
			
		hCurrent = hParent;
	}

	SetCheck(hItem, bCheck);
}

HTREEITEM CCheckTreeCtrl::GetFirstCheckedItem()
{
	for ( HTREEITEM hItem = GetRootItem(); hItem!=NULL;  )
	{
		if(GetCheck(hItem))
		{
			return hItem;
		}

		if(!IsCheckAnyChild(hItem))
		{
			HTREEITEM hti = GetNextItem(hItem, TVGN_NEXT);
			if(hti == NULL)
				hItem = GetNextItem(hItem, TVGN_CHILD);
			else
				hItem = hti;
		}
		else
		{
			hItem = GetNextItem(hItem, TVGN_CHILD);
		}
	}

	return NULL;
}

HTREEITEM CCheckTreeCtrl::GetNextCheckedItem(HTREEITEM hItem)
{
	if(ItemHasChildren(hItem))
	{
		hItem = GetNextItem(hItem, TVGN_CHILD);
	}
	else
	{
		HTREEITEM hti = GetNextItem(hItem, TVGN_NEXT);

		if(hti == NULL)
		{
			hItem = GetNextItem(hItem, TVGN_PARENT);			
			hItem = GetNextItem(hItem, TVGN_NEXT);			
		}
		else
		{
			hItem = hti;
		}
	}

	while(hItem != NULL)
	{
		if(GetCheck(hItem))
		{
			return hItem;
		}

		if(!IsCheckAnyChild(hItem))
		{		
			HTREEITEM hti = GetNextItem(hItem, TVGN_NEXT);
			if(hti == NULL)
			{
				hItem = GetNextItem(hItem, TVGN_PARENT);		
				hItem = GetNextItem(hItem, TVGN_NEXT);				
			}
			else
				hItem = hti;
		}
		else
		{
			hItem = GetNextItem(hItem, TVGN_CHILD);
		}
	}

	return NULL;
}

//void CCheckTreeCtrl::SetInit(int nTreeSelect, CStringArray* pSaResult, CList<ST_HAVEITEM, ST_HAVEITEM&> *listHaveItem, CWnd* pReWnd)
void CCheckTreeCtrl::SetInit(int nTreeSelect, CList<ST_HAVEITEM, ST_HAVEITEM&> *listHaveItem, CWnd* pReWnd)
{	
	m_pReWnd = pReWnd;
	CConditionResultWnd* pResultWnd = (CConditionResultWnd*)m_pReWnd;

	m_strParentItemText = pResultWnd->m_strEditSubjectSelectParentItemText;
	m_strSelectItemText = pResultWnd->m_strEditSubjectSelectItemText;
	
	CStringArray saRootTitle;
	CStringArray saTypeData;
	int i = 0;
	for( i = 0; i < 30; i++)
		m_htiMarket[i] = NULL;

//	switch (nTreeSelect)
//	{
//		case TREE_MARKET :
			for(i = 0; i < _countof(ROOT_TITLE_MARKET); i++)
				saRootTitle.Add(ROOT_TITLE_MARKET[i]);

			for(i = 0; i < _countof(TYPE_DATA_MARKET); i++)
				saTypeData.Add(TYPE_DATA_MARKET[i]);			
//			break;

//		case TREE_PORTFOLIO :
			for(i = 0; i < _countof(ROOT_TITLE_PORT); i++)
				saRootTitle.Add(ROOT_TITLE_PORT[i]);

			for(i = 0; i < _countof(TYPE_DATA_PORT); i++)
				saTypeData.Add(TYPE_DATA_PORT[i]);
//			break;

//		case TREE_RECOMMENDATION :
			for(i = 0; i < _countof(ROOT_TITLE_RECOMMENDATION); i++)
				saRootTitle.Add(ROOT_TITLE_RECOMMENDATION[i]);

			for(i = 0; i < _countof(TYPE_DATA_RECOMMENDATION); i++)
				saTypeData.Add(TYPE_DATA_RECOMMENDATION[i]);
//			break;
//	}
	
//	m_pSaSelectedItem = pSaResult;

	HTREEITEM htiRoot;
	htiRoot = GetRootItem();
	int nIndex_Market = 0;

	if(SetLoad_ConditionMngLib())
	{
		//KHD: 필요없는 호출 
		//SetSearch_MSTCode();
		//END 
		//# Load Tree Parent Item!
		
		for (int nIndex = 0; nIndex < saRootTitle.GetSize(); nIndex++)
		{
			if (saTypeData.GetAt(nIndex) == "I")
			{
				m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, saRootTitle.GetAt(nIndex), OFFSET_DATA[nIndex]);

				CString szTitle = saRootTitle.GetAt(nIndex);
				CString szMarketCode = TYPE_MARKETCODE[nIndex];

//				if(m_pSaSelectedItem->GetSize() != 0 && m_pSaSelectedItem->GetAt(0) == "0000")
				{
// 					this->ModifyStyle(TVS_CHECKBOXES, 0);
// 					this->ModifyStyle(0, TVS_CHECKBOXES);
// 					
// 					SetCheck(m_htiMarket[nIndex_Market]);
				}

				TreeItem stTreeItem;
				stTreeItem.hTreeItem = m_htiMarket[nIndex_Market];
				stTreeItem.szItemCode = szMarketCode;
				stTreeItem.szItemName = szTitle;
				stTreeItem.nIndex = OFFSET_DATA[nIndex];
				m_mapTreeItem.SetAt(stTreeItem.nIndex, stTreeItem);

				nIndex_Market++;
			}
			else if (saTypeData.GetAt(nIndex) == "M")
			{
				CStringArray saCode;
				CStringArray saName;
				if (GetMarketInfo(_ttoi(TYPE_MARKETCODE[nIndex]), &saCode, &saName) == TRUE)
				{
					m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, saRootTitle.GetAt(nIndex), -1);
					
					CString strTest = GetItemText(m_htiMarket[nIndex_Market]);
					
					SetInitTree_Item(m_htiMarket[nIndex_Market], nIndex, &saName, &saCode);
					nIndex_Market++;
				}
			}
			else if (saTypeData.GetAt(nIndex) == "T")
			{
				CStringArray saCode;
				CStringArray saName;
				if(GetThemeInfo(&saCode, &saName))
				{
					m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, saRootTitle.GetAt(nIndex), -1);
					SetInitTree_Item(m_htiMarket[nIndex_Market], nIndex, &saName, &saCode);
					nIndex_Market++;		
				}
			}
			else if (saTypeData.GetAt(nIndex) == "F")
			{
// 				CStringArray saGroupName, saCondiData;
// 				if (Lib_GetConditionData(atoi(TYPE_MARKETCODE[nIndex]), FALSE, 0, saGroupName, saCondiData) == TRUE)
// 				{
// 					m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, ROOTTITLE_MARKET[nIndex], 
// 						m_nTreeIconIndex0St_File, m_nTreeIconIndex0St_File + 1, -1);
// 
// 					SetInitTree_File(m_htiMarket[nIndex_Market], nIndex, &saGroupName, &saCondiData);
// 
// 					nIndex_Market++;
// 				}
			}
			else if (saTypeData.GetAt(nIndex) == "P")
			{
				m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, saRootTitle.GetAt(nIndex), OFFSET_DATA[nIndex]);
				SetInitTree_Port(m_htiMarket[nIndex_Market], nIndex);

				/*m_nIndex_Port = nIndex;*/
				m_htiPort = m_htiMarket[nIndex_Market];
				nIndex_Market++;
			}

			else if (saTypeData.GetAt(nIndex) == "H")
			{
 				CStringArray saCode;
 				CStringArray saName;
// 				POSITION pos;
				
				m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, saRootTitle.GetAt(nIndex), -1);

				CStringArray saAccNo;
				pResultWnd->GetAccNo(&saAccNo);

				for(int nIndex = 1; nIndex < saAccNo.GetSize(); nIndex += 2)
				{
					saCode.Add(saAccNo.GetAt(nIndex));
					saName.Add(saAccNo.GetAt(nIndex + 1));
				}

				SetInitTree_HaveItem(m_htiMarket[nIndex_Market], nIndex, &saName, &saCode, "ACCOUNT");

// 				for(int i = 0; i < listHaveItem->GetCount(); i++)
// 				{
// 					pos = listHaveItem->FindIndex(i);
// 					ST_HAVEITEM stHaveItem = listHaveItem->GetAt(pos);
// 					CString strTemp = stHaveItem.strAccount;
// 					strTemp.Insert(8, '-');
// 					saCode.Add(stHaveItem.strAccount);
// 					saName.Add(strTemp);
// 				}
// 				SetInitTree_HaveItem(m_htiMarket[nIndex_Market], nIndex, &saName, &saCode, "ACCOUNT");
// 
// 				for(i = 0; i < listHaveItem->GetCount(); i++)
// 				{
// 					saCode.RemoveAll();
// 					saName.RemoveAll();
// 
// 					pos = listHaveItem->FindIndex(i);
// 					ST_HAVEITEM stHaveItem = listHaveItem->GetAt(pos);
// 
// 					for(int j = 0; j < (int)stHaveItem.vecItemCode.size(); j++)
// 					{
// 						int nStrLenth = 15;
// 						saCode.Add(stHaveItem.vecItemCode[j]);
// 						CString strTemp;
// 						CString _strTemp;
// 						
// 						nStrLenth -= stHaveItem.vecItemName[j].GetLength();
// 						for(int i = 0; i < nStrLenth; i++)
// 							_strTemp += " ";
// 
// 						strTemp.Format("%s%s[%s]", stHaveItem.vecItemName[j], _strTemp, stHaveItem.vecItemCode[j]);
// 						saName.Add(strTemp);
// 					}
// 
// 					TreeItem stTreeItem;
// 					if(m_mapTreeItem.Lookup(1000 * (i + 1), stTreeItem))
// 					{
// 						SetInitTree_HaveItem(stTreeItem.hTreeItem, 1000 * (i + 1), &saName, &saCode, "ITEM");
// 					}
// 					nIndex_Market++;
//				}
			}
		}
	}

	if(m_strSelectItemText == "")
	{
		SetSelectItem(m_htiMarket[0]);
	}
	else
	{
		if(m_strParentItemText != "")
		{
			int nIndex = 0;
			HTREEITEM hParent = m_htiMarket[nIndex];
			
			while(hParent)
			{
				if(GetItemText(hParent) == m_strParentItemText)
					break;
				
				nIndex++;
				hParent = m_htiMarket[nIndex];
			}

			if(hParent == NULL)
			{
				SetSelectItem(m_htiMarket[0]);
			}
			else
			{
				HTREEITEM hItem = GetChildItem(hParent);
				while (hItem)
				{
					if(GetItemText(hItem) == m_strSelectItemText)
					{
						SetSelectItem(hItem);
						break;
					}

					hItem = GetNextSiblingItem(hItem);
				}
			}
		}
		else
		{
			int nIndex = 0;
			HTREEITEM hItem = m_htiMarket[nIndex];
			while(hItem)
			{
				if(GetItemText(hItem) == m_strSelectItemText)
				{
					SetSelectItem(hItem);
					break;
				}

				nIndex++;
				hItem = m_htiMarket[nIndex];
			}

			if(hItem == NULL)
			{
				SetSelectItem(m_htiMarket[0]);
			}
				
		}
	}
// 	if(m_pSaSelectedItem->GetSize() != 0 && m_pSaSelectedItem->GetAt(0) != "0000")// 찾아서 체크 
// 	{
// 		SetCheckedTree(nIndex_Market);
// 	}
// 	else if(nTreeSelect == TREE_MARKET && m_pSaSelectedItem->GetSize() != 0 && m_pSaSelectedItem->GetAt(0) == "0000")//KHD: 만약 전체이면 전체체크를 한다.
// 	{
// 		for(int i = 1; m_htiMarket[i] != NULL; i++)
// 		{
// 			SetCheck(m_htiMarket[i], TRUE);
// 			HTREEITEM hItem = GetChildItem(m_htiMarket[i]);
// 			while(hItem)
// 			{
// 				SetCheck(hItem, TRUE);
// 				hItem = GetNextSiblingItem(hItem);
// 			}
// 		}
// 	}
}

void CCheckTreeCtrl::SetTreeData(LPCSTR ROOT_TITLE[], LPCTSTR TYPE_DATA[], 
								 CStringArray* saRootTitle, CStringArray* saTypeData)
{
	for(int i = 0; i < _countof(ROOT_TITLE); i++)
	{
		saRootTitle->Add(ROOT_TITLE[i]);
	}

	for(int i = 0; i < _countof(TYPE_DATA); i++)
	{
		saTypeData->Add(TYPE_DATA[i]);
	}
}

BOOL CCheckTreeCtrl::SetLoad_ConditionMngLib()
{
	return TRUE;

	if (m_hConditionMng != NULL)
		return TRUE;
	
	GETPATH_APP(strRoot);
	strRoot.Mid(0, strRoot.GetLength() - 4);
	CString strPath = strRoot + "\\dev\\Lib_ConditionMng.dll";

	m_hConditionMng = ::LoadLibrary(strPath);
	if (m_hConditionMng)
	{
		Lib_Open = (_DLL_Open)::GetProcAddress(m_hConditionMng, "_DLL_Open");
		Lib_Close = (_DLL_Close)::GetProcAddress(m_hConditionMng, "_DLL_Close");

		Lib_GetConditionData = (_DLL_GetConditionData)::GetProcAddress(m_hConditionMng, "_DLL_GetConditionData");
		Lib_GetFind_Packet = (_DLL_GetFind_Packet)::GetProcAddress(m_hConditionMng, "_DLL_GetFind_Packet");
		Lib_GetFind_Result = (_DLL_GetFind_Result)::GetProcAddress(m_hConditionMng, "_DLL_GetFind_Result");
	}
	else
	{
		TRACE("\n# Lib_ConditionMng.dll이 없습니다.");
		return FALSE;
	}

	return TRUE;
}

void CCheckTreeCtrl::SetSearch_MSTCode()
{
	int nType = 2;

	char* lpData = new char[sizeof(int) + 1];
	nType = ntohl(nType);
	::memset(lpData, 0x00, sizeof(int) + 1);
	memcpy(lpData, &nType, sizeof(int) + 1);

	int nState = Module_SendTr((CString)TRNO_MSTCODE, sizeof(int), (BYTE*)lpData);

	if (nState < 0)
		TRACE("조회실패...");
	else
		TRACE("조회중...");

	if (lpData)
		delete[] lpData;
}

int CCheckTreeCtrl::Module_SendTr(CString strTrNo, int nTrLen, BYTE *TrData)
{		
	return 1;
}

HTREEITEM CCheckTreeCtrl::SetTreeItem(HTREEITEM htiParent, CString strItem, int lParam) 
{
	TV_INSERTSTRUCT tvstruct;

	tvstruct.hParent = htiParent;
	tvstruct.hInsertAfter = TVI_LAST;

	tvstruct.item.pszText = (LPTSTR)(LPCSTR)strItem;	

	tvstruct.item.lParam = (long)lParam;
	tvstruct.item.mask = TVIF_TEXT | TVIF_PARAM |TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	HTREEITEM hItem = InsertItem(&tvstruct);
	
	Expand(hItem, TVE_COLLAPSE);
	UpdateWindow();

	return hItem; 
}

BOOL CCheckTreeCtrl::GetMarketInfo(int nMarketType, CStringArray *psaCode, CStringArray *psaName)
{
	BOOL bResult = TRUE;

	
// 	IMasterDataManager080902* pManager = (IMasterDataManager080902*)AfxGetPctrInterface(UUID_IMasterDataManager, 1);
// 	if (pManager == NULL)
// 		return bResult;
	

	psaCode->RemoveAll();
	psaName->RemoveAll();

	CDlgEditSubject* pParent = (CDlgEditSubject*)GetParent();
	pParent->GetUpMasterData(psaCode, psaName, nMarketType);

	/*--------------------------------
	거래소		0001
	코스닥		1001
	KOSPI200	2001
	KOSTAR		3001
	KRX100		4001
	---------------------------------*/
	
	
// 	if("0001" == strType)			// 거래소업종
// 	{
// 		bResult = pManager->GetKSPUpMst((CStringArray *&)psaCode, (CStringArray *&)psaName, (CStringArray *&)parrEngName);
// 	}
// 	else if("1001" == strType)		// 코스닥업종
// 	{
// 		bResult = pManager->GetKSQUpMst((CStringArray *&)psaCode, (CStringArray *&)psaName, (CStringArray *&)parrEngName);
// 	}
// 	else if("2001" == strType)		// KOSPI200
// 	{
// 		bResult = pManager->GetK200UpMst((CStringArray *&)psaCode, (CStringArray *&)psaName, (CStringArray *&)parrEngName);
// 	}
// 	else if("3001" == strType)		// KOSTAR
// 	{
// 		bResult = pManager->GetKSTRUpMst((CStringArray *&)psaCode, (CStringArray *&)psaName, (CStringArray *&)parrEngName);
// 	}
// 	else if("4001" == strType)		// KRX
// 	{
// 		bResult = pManager->GetKRXUpMst((CStringArray *&)psaCode, (CStringArray *&)psaName, (CStringArray *&)parrEngName);
// 	}
	

  return bResult;
}

BOOL CCheckTreeCtrl::GetThemeInfo(CStringArray* psaCode, CStringArray* psaName)
{
	BOOL bResult = TRUE;

	psaCode->RemoveAll();
	psaName->RemoveAll();

	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;

	CString sFile;
	sFile.Format("%s\\%s\\%s", pReWnd->Variant(homeCC), TABDIR, FILE_THEME);	
	
	for (int i = 0; ; i++)
	{
		char szBuf[BUFSIZ] = { 0x00, };
		CString sKey;
		
		if (i < 100)
			sKey.Format("%02d", i);
		else
			sKey.Format("%03d", i);
		
		GetPrivateProfileString("FOLDER", sKey, "", szBuf, sizeof(szBuf), sFile);
		
		if (strlen(szBuf) <= 0)
			break;
		
		CString sData = szBuf;
		
		if (sData.GetLength() < 33)
			continue;
		
		CString sName = sData.Left(30);
		CString sTheCode = sData.Right(3);
		
		sName.TrimLeft(); sName.TrimRight();
		sTheCode.TrimLeft(); sTheCode.TrimRight();
		
		char szCList[BUFSIZ] = { 0x00, };
		GetPrivateProfileString("TMCODE", sTheCode, "", szCList, sizeof(szCList), sFile);
		
		CString sCList = szCList;
		while (1)
		{
			CString sCode = CUtils::Parser(sCList, ";");
			sCode.TrimLeft(); sCode.TrimRight();
			
			if (sCode.GetLength() <= 0)
				break;
			
			psaCode->Add(sCode);
		}
		
 		if (psaCode->GetSize() <= 0)
 		{
 			return FALSE;
 		}
		
		CString strName;
		strName.Format("[%04d] %s", atol(sTheCode), sName);
		psaName->Add(strName);
	}

	return bResult;
}

void CCheckTreeCtrl::SetInitTree_Item(HTREEITEM htiParent, int nItemIndex, CStringArray *psaName, CStringArray *psaCode)
{
	if (htiParent == NULL)
		return;

	int nOffset;
	nOffset = OFFSET_DATA[nItemIndex];

	DrTreeItem stDrTreeItem;
	CString szName, szMarketCode;
	for (int nIndex = 0; nIndex < psaName->GetSize(); nIndex++)
	{
		szName = psaName->GetAt(nIndex);
		szMarketCode = psaCode->GetAt(nIndex);
		
		HTREEITEM hItem = SetTreeItem(htiParent, szName, nOffset);	
	
		TreeItem stTreeItem;
		stTreeItem.hTreeItem = hItem;
		stTreeItem.szItemCode = szMarketCode;
		stTreeItem.szItemName = szName;
		stTreeItem.nIndex = nOffset;
		
		m_mapTreeItem.SetAt(stTreeItem.nIndex, stTreeItem);
		nOffset++;

		if(szName == "KOSPI200")
			break;
	}
}

CString CCheckTreeCtrl::GetItemMaketCode(HTREEITEM hSelItem)
{
	TreeItem szTreeItem;
	
	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hSelItem;
	item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	item.pszText = szText;
	item.cchTextMax = 1024;
	GetItem(&item);
	if(m_mapTreeItem.Lookup((int)item.lParam, szTreeItem))
		return szTreeItem.szItemCode;

	return _T("");
}

void CCheckTreeCtrl::SetInitTree_Port(HTREEITEM htiParent, int nItemIndex)
{
	if (htiParent == NULL)
		return;

	int nOffset;
	nOffset = OFFSET_DATA[nItemIndex];

	DrTreeItem stDrTreeItem;
	CStringArray saPortName;
	CStringArray saPortKey;
	GetKwansimGroupName(saPortKey, saPortName);
	for (int nIndex = 0; nIndex < saPortName.GetSize(); nIndex++)
	{		
		CString szName = saPortName.GetAt(nIndex);
		szName.Trim();//tour2k
		CString szMarketCode = saPortKey.GetAt(nIndex);

		HTREEITEM hItem = SetTreeItem(htiParent, szName, nOffset);	

		TreeItem stTreeItem;
		stTreeItem.hTreeItem = hItem;
		stTreeItem.szItemCode = szMarketCode;
		stTreeItem.szItemName = szName;
		stTreeItem.nIndex = nOffset;
		m_mapTreeItem.SetAt(stTreeItem.nIndex, stTreeItem);
		nOffset++;
	}
}

void CCheckTreeCtrl::GetKwansimGroupName(CStringArray& arrKey, CStringArray& arrGroupName)
{
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	pReWnd->GetData_PortGroup(arrKey, arrGroupName);
}
/*
void CCheckTreeCtrl::SetCheckedTree(int nIndex)
{
	HTREEITEM hParent = NULL;
	HTREEITEM hItem = NULL;
	HTREEITEM hTemp = NULL;

	// 체크박스가 들어가있는 TreeCtrl을 init하는 순간에 SetCheck 하기 위함 >>
// 	this->ModifyStyle(TVS_CHECKBOXES, 0);
// 	this->ModifyStyle(0, TVS_CHECKBOXES);
	// <<

	TVITEM item;
	TCHAR szText[1024];
	item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	item.pszText = szText;
	item.cchTextMax = 1024;

	TreeItem stTreeItem;
	
	for(int i = 0; i < nIndex; i++)
	{
		hItem = hParent = m_htiMarket[i];
		
		while(GetChildItem(hItem))
		{
			hTemp = hItem;
			hItem = GetChildItem(hItem);
		}
			
		if(hTemp)
			hParent = hTemp;

		while(hParent)
		{
			hItem = GetChildItem(hParent);
			
			while(hItem)
			{
				item.hItem = hItem;
				GetItem(&item);

				if(m_mapTreeItem.Lookup((int)item.lParam, stTreeItem))
				{
					for(int i = 0; i < m_pSaSelectedItem->GetSize(); i++)
					{
						CString szSelectedCode = m_pSaSelectedItem->GetAt(i);
						if(stTreeItem.szItemCode == m_pSaSelectedItem->GetAt(i))
						{
							SetCheck(hItem);	
							Expand(hParent, TVE_EXPAND);
						}
					}
				}
				
				hItem = GetNextSiblingItem(hItem);
			}
			
			SetCheck(hParent, IsCheckAllChild(hParent));
			hParent = GetNextSiblingItem(hParent);
			
		}

		if(IsCheckAnyChild(m_htiMarket[i]))
			Expand(m_htiMarket[i], TVE_EXPAND);

	}

	SetScrollPos(SB_VERT, 0, TRUE);

	UpdateWindow();
}
*/

void CCheckTreeCtrl::GetSelectMText(CStringArray* psaMText)
{
	psaMText->RemoveAll();
	
	HTREEITEM hParent = NULL;

	for(int i = 0; m_htiMarket[i] != NULL; i++)
	{
		hParent = m_htiMarket[i];
		
		if(IsCheckAnyChild(hParent))	
			psaMText->Add(GetItemText(hParent));
		
	}
}

void CCheckTreeCtrl::OnDestroy()
{
	if(!m_hConditionMng)
		::FreeLibrary(m_hConditionMng);
	
	CTreeCtrl::OnDestroy();
}

void CCheckTreeCtrl::SetInitTree_HaveItem(HTREEITEM htiParent, int nItemIndex, CStringArray *psaName, 
										  CStringArray *psaCode, CString strDivision)
{
	if (htiParent == NULL)
		return;

	int nOffset;
	if(strDivision == "ACCOUNT")
		nOffset = 1000; // 계좌 offet 1000, 2000, 3000, ...
	else
		nOffset = nItemIndex; // 계좌별 보유종목 offset 1000(계좌) : 1001, 1002, ... 2000(계좌) : 2001, 2002, ...

	DrTreeItem stDrTreeItem;
	CString szName, szMarketCode;
	for (int nIndex = 0; nIndex < psaName->GetSize(); nIndex++)
	{
		szName = psaName->GetAt(nIndex);
		szMarketCode = psaCode->GetAt(nIndex);

		HTREEITEM hItem = NULL;
		if(strDivision == "ACCOUNT")
			hItem = SetTreeItem(htiParent, szName, nOffset * (nIndex + 1));
		else
			hItem = SetTreeItem(htiParent, szName, nOffset + nIndex + 1);

		TreeItem stTreeItem;
		stTreeItem.hTreeItem = hItem;
		stTreeItem.szItemCode = szMarketCode;
		stTreeItem.szItemName = szName;
		stTreeItem.nIndex = nOffset;

		if(strDivision == "ACCOUNT")
			stTreeItem.nIndex = nOffset * (nIndex + 1);
		else
			stTreeItem.nIndex = nOffset + nIndex + 1;

		m_mapTreeItem.SetAt(stTreeItem.nIndex, stTreeItem);
	}
}

BOOL CCheckTreeCtrl::GetItemCodeFromThemeGroup(CString strThemeGroupCode, CStringArray* pSaCode, CStringArray* pSaName)
{
	BOOL bResult = TRUE;

	
// 	IMasterDataManager080902* pManager = (IMasterDataManager080902*)AfxGetPctrInterface(UUID_IMasterDataManager, 1);
// 	if (pManager == NULL)
// 		return bResult;
// 
// 	bResult = pManager->GetThemeItemList(strThemeGroupCode, *pSaCode, *pSaName);
	

	return bResult;
}

BOOL CCheckTreeCtrl::GetItemCodeFromGwanGroup(CString strKey, CStringArray* arrCode, CStringArray* arrName)
{
	BOOL bResult = TRUE;

	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	bResult = pReWnd->GetData_PortItem(*arrCode, *arrName, _ttoi(strKey));

	return bResult;
}

BOOL CCheckTreeCtrl::IsGwanGroup(HTREEITEM hItem)
{
	HTREEITEM hChildItem = GetChildItem(m_htiMarket[0]);

	while(hChildItem)
	{
		if(hChildItem == hItem)
			return TRUE;

		hChildItem = GetNextSiblingItem(hChildItem);
	}

	return FALSE;
}

BOOL CCheckTreeCtrl::IsCheckedAnyItem()
{
	for(int i = 0; m_htiMarket[i] != NULL; i++)
	{
		if(IsCheckAnyChild(m_htiMarket[i]))
			return TRUE;
	}

	return FALSE;
}