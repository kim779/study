// AgentCodeList.cpp : implementation file
//

#include "stdafx.h"
#include "AgentTreeList.h"
#include "./AgentTreeListDefine.h"
//#include "../CommonTR/Tr88803.h"
#include "../../chart/CommonTR/Tr88803.h"
#include "../../inc/HelperTreeCtrl.h"
#include "TreeListMng.h"
#include "../../inc/RealUpdate.h"
#include "DefineFiles/TranHelperd1022.h"
//#include "../../inc/IMainExInfoMng.h"
//#include "../../../Shared/inc/CommonCtrl.h"		// \\shared\inc\commonctrl.h
//#include "../../../Shared/src/SharedInfoDll/SharedInfo.h"

#include "../../../SrcSite/include/axisfire.h"

#include "../../../SrcSite/include/ledger.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

inline void g_ConvertBigEndian ( long& d )
{
	long s=d;
	char *ps = (char*)&s;
	char *pd = (char*)&d;

	pd[0] = ps[3];
	pd[1] = ps[2];
	pd[2] = ps[1];
	pd[3] = ps[0];
}

// ----------------------------------------------------------------------
//           인터페이스 구현
// ----------------------------------------------------------------------
void CTreeListManager::SetKeyData(LPCSTR szKey, LPCSTR szData)
{	
}

void CTreeListManager::SetKeyData(LPCSTR szKey, long   dwData)
{
	if(strcmp(szKey, "WinixWnd")==0)
	{
		m_hWinixWnd = (HWND)dwData;
		if(!m_hWinixWnd || !IsWindow(m_hWinixWnd))
			m_hWinixWnd = NULL;
	}
	else if(strcmp(szKey, "WrapperWnd")==0)
	{
		m_hWrapper = (HWND)dwData;
		if(!m_hWrapper || !IsWindow(m_hWrapper))
			m_hWrapper = NULL;
// 		else
// 		{
// 			m_pGwanMng = (IWinixGwanManager*)GetParent()->SendMessage(RMSG_GETINTERFACE, 0, 0);
// 			::SendMessage
// 		}
	}
}

LONG CTreeListManager::LoadTreeList(CInputCodeListEx *pInData, LPCSTR szSection)
{
	m_pTreeCtrl = pInData->m_pTreeCtrl;
	m_pListCtrl = pInData->m_pListCtrl;
	CTreeCtrl* pTreeCtrl = pInData->m_pTreeCtrl;
	CListCtrl* pListCtrl = pInData->m_pListCtrl;

	CString szFile(pInData->m_szFile);
	szFile.TrimRight();
	if(szFile.GetLength()==0)
		szFile = m_szConfigFile;

	CTreeResItemEx*  pNewList = new CTreeResItemEx;
	pNewList->m_pmapCondition = NULL;

	pNewList->m_pItemList = new LIST_CBaseLongData;
	pNewList->m_pTrComm = (ITrComm2*)m_pTrCommMng->AdviseTrComm();
	pNewList->m_pTrCommSite = &m_xTrCommSite;

	TRACE1("CTreeListManager::LoadTreeList ITrComm[%08X\n", pNewList->m_pTrComm);

	POSITION pos = m_AllList.AddTail(pNewList);

	HTREEITEM hTreeParent=TVI_ROOT;
	LoadTreeConfig(pNewList, pTreeCtrl, hTreeParent, szSection, szFile, pListCtrl);

//	LoadConditonMng(pNewList, pTreeCtrl);
//	LoadConditonMngUser(pNewList, pTreeCtrl);
	return (LONG)pos;
}

void CTreeListManager::UnLoadTreeList(LONG dwKey)
{

	POSITION pos = (POSITION)dwKey;
	if(CheckAllListt(pos)==FALSE)
	{
		CString szMsg;
		szMsg.Format("CTreeListManager::UnLoadTreeList(..)에서 파라미터 Error(%d)", dwKey);
		// AfxMessageBox();
		TRACE1("%s\n", szMsg);
		return;
	}

	CTreeResItemEx* pList = (CTreeResItemEx*)m_AllList.GetAt(pos);
	if(!pList) return;
	
	if(pList->m_pTrComm) {
		m_pTrCommMng->UnAdviseTrComm(pList->m_pTrComm);
		// pList->m_pTrComm = NULL;
	}
	
	// 원클릭과 관련된 정보를 내린다.
	m_LibCondNng.SetClear_Condition(pList);
	//m_LibCondNng.UnLoadLib();

	ClearList(pList->m_pItemList);
	if(pList->m_pItemList) delete pList->m_pItemList; pList->m_pItemList=NULL;
	if(pList) delete pList; pList=NULL;
	m_AllList.RemoveAt(pos);
	
	//if(m_pMDMng)
	//{
	//	delete m_pMDMng;
	//	m_pMDMng = NULL;
	//}
}

BOOL CTreeListManager::CheckAllListt(POSITION comPos)
{
	if(!comPos) return FALSE;

	for(POSITION pos=m_AllList.GetHeadPosition(); pos;)
	{
		if(comPos==pos) return TRUE;
		m_AllList.GetNext(pos);
	}
	return FALSE;
}

//BOOL CTreeListManager::LoadFile(CInputCodeListEx *pInData, LPCSTR szSection)
//{
//	CTreeCtrl* pTree = pInData->m_pTreeCtrl;
//	
//	CString szFile(pInData->m_szFile);
//	szFile.TrimRight();
//	if(szFile.GetLength()==0)	// && strlen(pInData->m_szFile)>0)
//		szFile = m_szConfigFile;
//
//	HTREEITEM hTreeParent=TVI_ROOT;
//	LoadConfig(pTree, hTreeParent, szSection, szFile);
//
//	return FALSE;
//}


/*
cDataType  'A': 마스터파일일 경우에는 PC직접처리한다.
           'B': 시세등의 데이타가 필요하기 때문에 무조건 서버로 요청한다.
*/
void CTreeListManager::RequestData_HistoryTR(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
{
	LIST_CCodeTypeA ListTypeA;
	CallHistory(&ListTypeA);
	if(ListTypeA.GetCount()==0) return;

	LIST_TR_CODEINFO CodeList;
	CCodeTypeA codeT;
	for(POSITION pos=ListTypeA.GetHeadPosition(); pos; )
	{
		codeT = ListTypeA.GetNext(pos);

		TR_CODEINFOEX oneCode;
		ZeroMemory(&oneCode, sizeof(TR_CODEINFOEX));
		oneCode.gb = codeT.m_cGb;
		memcpy(&oneCode.code[0], (LPCSTR)codeT.m_szcode, 6); 
		CodeList.AddTail(oneCode);
	}
	if(CodeList.GetCount()>0)
		RequestCodeData((void*)pILData->m_pITrComm, &CodeList, pCallback, cDataType, cSort, dwReserved);
	else
		pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);	// 데이타가 없을 때


/*
	CPacketInfo newInfo;
	newInfo.m_aDataType = cDataType;
	newInfo.m_pCallback = pCallback;
	newInfo.m_dwReserved = dwReserved;
	POSITION pos=m_PacketList.AddTail(newInfo);
	
	char aReserved[9]; 
	sprintf(aReserved, "%08X", pos);

	char aCount[5];
	sprintf(aCount, "%04d", ListTypeA.GetCount());

	ITrComm* pTrComm = (ITrComm*)pILData->m_pITrComm;
	if(pTrComm)
	{
		int nLen = TR_88803_I_SIZE + (TR_CODEINFO_SIZE * ListTypeA.GetCount());
		char* pReadData = (char*)malloc(nLen);
		memset(pReadData, 0x20, nLen);
		TR_88803_I* pInData = (TR_88803_I*)pReadData ;
		memcpy(pInData->reserved, aReserved, 8);
		memcpy(pInData->mainkey, pILData->m_szInputData, 10);
		pInData->sort[0] = cSort;
		pInData->datatype[0]= cDataType;
		memcpy(pInData->count, aCount, 4);
		TR_CODEINFO trCode;
		CCodeTypeA codeT;
		int nOffset = 0;
		for(POSITION pos=ListTypeA.GetHeadPosition(); pos; )
		{
			codeT = ListTypeA.GetNext(pos);
			trCode.gb[0] = codeT.m_cGb;
			memcpy(&trCode.code[0], (LPCSTR)codeT.m_szcode, 6); 
			memcpy(&pInData->data[nOffset], &trCode, TR_CODEINFO_SIZE);
			nOffset += TR_CODEINFO_SIZE;
		}
//		inData.data

		pTrComm->SetTR(pILData->m_szTR);
		pTrComm->ChangeCallback(&m_xTrCommSite);
		pTrComm->Send2Server(pReadData, nLen);

	//	TRACE("CTreeListManager::RequestData:SendPacket[TR[%s]%*.*s]\n", pILData->m_szTR, nLen, nLen, szData);

		free(pReadData);
	}
*/
}

//*****************************************************************************
// 루    틴    명 :  GetGwanGroupCode
// 기          능 :  그루병에 해당하는 관심그룹 리스트를 얻어오는 함수.
// 입          력 :  CString szGroupName 그룹명
//                   LIST_TR_CODEINFO* pCodeList 종목리스트
// 출          력 :  void
// 작    성    자 :  2006년 2월 13일   이 준옥.
// 수  정  사  항 :  
//*****************************************************************************
void CTreeListManager::GetGwanGroupCode(CString szGroupKey, LIST_TR_CODEINFO* pCodeList)
{
	pCodeList->RemoveAll();
	CStringArray arrCode, arrItem;
	theApp.GetGwansimItem(m_pWinixGwanInterface, szGroupKey, arrCode, arrItem);

	int nLen, nSize = arrItem.GetSize();	
	CString strData, strCode, strType;
	TR_CODEINFOEX oneCode;
	int nIndex=0;
	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	{
		ZeroMemory(&oneCode, sizeof(TR_CODEINFOEX));
		strData = arrItem.GetAt(nIndx);
		strCode = arrCode.GetAt(nIndx);
//		if(TRUE)
		{
			strType = m_pMDMng->GetCodeInfo("", 2, strCode);
			//기존에 동작하던 타입으로 변경
			//if(strType=="1") strType = "J";
			//else if(strType=="2") strType = "Q";
			//if(strType == "J" || strType == "Q")

			// 2012.2.7 박성경 >>
			nLen = strData.GetLength();

			if (nLen > 0)
			{
				memcpy(oneCode.code, strCode, min(strCode.GetLength(), sizeof(oneCode.code)));
				memcpy(&oneCode.gb, strType, 1);
				memcpy(oneCode.codeName, strData.GetBuffer(0), strData.GetLength());
				pCodeList->AddTail(oneCode);
			}
			// 2012.2.7 박성경 <<
			
			// 2012.2.7 박성경: 관심 솔로몬 >>
			/*
			if( strType == "1" ||		// 상장종목
				strType == "2" ||		// 코스닥종목
				strType == "3" ||		// 프리보드
				strType == "N" )		// 주식워런트 ELW
			{
				memcpy(&oneCode.code[0], strCode, 6); 
				memcpy(&oneCode.gb, strType, 1);
				memcpy(&oneCode.codeName, strData.GetBuffer(0), strData.GetLength());
				pCodeList->AddTail(oneCode);
			}
			else if( strType == "B" ||		// KOSPI 200 업종, KOSPI 100 업종, 장내업종, KRX, KRX업종 
					 strType == "C" )			// 코스닥 업종, 코스닥 50
			{
				memcpy(&oneCode.code[0], strCode, 3); 
				memcpy(&oneCode.gb, strType, 1);
				memcpy(&oneCode.codeName, strData.GetBuffer(0), strData.GetLength());
				pCodeList->AddTail(oneCode);
			}
			else if( strType == "4" ||		// 선물, 스프레드 
					 strType == "5" ||		// 옵션
					 strType == "M" )		// 스타선물
			{
				memcpy(&oneCode.code[0], strCode, 8); 
				memcpy(&oneCode.gb, strType, 1);
				memcpy(&oneCode.codeName, strData.GetBuffer(0), strData.GetLength());
				pCodeList->AddTail(oneCode);
			}
			*/
			// 2012.2.7 박성경: 관심 솔로몬 <<
		}
//		else
//		{
//			memcpy(&oneCode.code[0], "      ", 6); 
//			memcpy(&oneCode.gb, "J", 1);
//			pCodeList->AddTail(oneCode);
//		}
	}
	//pCodeList->RemoveAll();
	//INT_TOTAL* pData = (INT_TOTAL*)theApp.GetGwansimDataAll();
	//if(!pData) return;

	//CPtrArray* pArGroupAll = (CPtrArray*)pData->Idata;
	//if(pArGroupAll) {
	//	int nSize = pArGroupAll->GetSize();
	//	char szNo[3] = {0, };
	//	INT_GROUP_ST* pIntGroup = NULL;
	//	CINT_GROUP_ST itemGwan;

	//	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	//	{
	//		pIntGroup = (INT_GROUP_ST*)pArGroupAll->GetAt(nIndx);
	//		itemGwan.Convert(pIntGroup);
	//		if(szGroupName.Compare(itemGwan.m_szItitle)==0) // 그룹이름이 같을 때의 종목 리스트 검색..
	//		{
	//			CPtrArray* pGroupItem = (CPtrArray*)itemGwan.m_pIdata;
	//			int nSize = pGroupItem->GetSize();

	//			INT_ITEM_ST* pItem = NULL;
	//			CINT_ITEM_ST  OneItem;
	//			for(int nIndx = 0; nIndx < nSize; ++nIndx)
	//			{
	//				if(pItem = (INT_ITEM_ST*)pGroupItem->GetAt(nIndx)) {
	//					OneItem.Convert(pItem);
	//					// OneItem.LocalTRACE("CGwanWnd::FindGroup");

	//					TR_CODEINFO oneCode;
	//					oneCode.gb = OneItem.GetMyGb();
	//					if(oneCode.gb=='E') continue;  // Blank등 에러이면 뺀다.

	//					memcpy(&oneCode.code[0], OneItem.m_szIcode, 6); 
	//					pCodeList->AddTail(oneCode);
	//				}
	//			}
	//		}
	//	}
	//}
}


//*****************************************************************************
// 루    틴    명 :  GetImsiGroupCode
// 기          능 :  임시그룹에 속하는 리스트를 얻어오는 함수.
// 입          력 :  CString szGroupName 임시그룹명
//                   LIST_TR_CODEINFO* pCodeList 종목리스트
// 출          력 :  void
// 작    성    자 :  2006년 2월 13일   이 준옥.
// 수  정  사  항 :  
//*****************************************************************************
LIST_TR_CODEINFO* CTreeListManager::GetImsiGroupCode(CString szGroupName)
{
	//IMainExInfoManager* pMng = (IMainExInfoManager*)theApp.m_pIGateMng->GetInterface(22);
	//COleVariant* pVar = pMng->GetData(IMainExInfoManager_ImsiGroup);
	//if(pVar) return (LIST_TR_CODEINFO*)pVar->lVal;

	return NULL;
}

void CTreeListManager::RequestDataOfOneClick(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
{
	CItemOneClickData* pItemData = (CItemOneClickData*)pILData;
	TRACE("%s\n", pItemData->m_szOCPath);

// 	CPacketInfo newInfo;
// 	newInfo.m_aDataType = cDataType;
// 	newInfo.m_pCallback = pCallback;
// 	newInfo.m_dwReserved = dwReserved;
// 	//POSITION pos=m_PacketList.AddTail(newInfo);
	m_CurPackInfo.m_aDataType = cDataType;
	m_CurPackInfo.m_pCallback = pCallback;
	m_CurPackInfo.m_dwReserved = dwReserved;

	//화면번호를 세팅한다.
	CString strScreenNo;
	GetBaseData("SCREENCODE", strScreenNo);
	m_LibCondNng.m_hSenComm = m_hWinixWnd;
	m_LibCondNng.m_hWrapper = m_hWrapper;
	POSITION pos=(POSITION)100;
	if(m_LibCondNng.SetTrSend_Result(pos, pItemData, (LPCSTR)strScreenNo)==FALSE)
	{
		//m_PacketList.RemoveAt(pos);
	}
}

void CTreeListManager::RequestData(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
{
	if(!pILData) return;

	if(pILData->m_nType==BaseLongData_OneClick)
	{
		// CItemOneClickData* pItemData = (CItemOneClickData*)pILData;
		RequestDataOfOneClick(pILData, pCallback, cDataType, cSort, dwReserved);
		return;
	}

	if(!pILData->m_szMethod.CompareNoCase("MASTER"))
	{
//		if(cDataType=='A')
		{	
			RequestData_A_MASTER(pILData, pCallback, cDataType, dwReserved);
		}
//-->Edit 유진080331
//		else
//		{
//		}
//<--Edit 유진080331
	}
	else if(!pILData->m_szMethod.CompareNoCase("GROUPMASTER"))
	{
		RequestData_GroupMASTER(pILData, pCallback, cDataType, dwReserved, cSort);
	}
	else if(!pILData->m_szMethod.CompareNoCase("USERTR"))
	{
		// 히스토리 구하기
		if(!pILData->m_szSection.CompareNoCase("USERCODE"))
		{
			//if(!pILData->m_szInputData.CompareNoCase(SZKEY_CODEREQUEST))
			//{
			//	cSort = '0';	//히스토리는 소트하지 않고 그대로..
			//	RequestData_HistoryTR(pILData, pCallback, cDataType, cSort, dwReserved);
			//}
			cSort = '0';	//히스토리는 소트하지 않고 그대로..

			CString szInData = pILData->m_szInputData;
			// 관심그룹의 리스트를 얻어온다음 TR조회를 요청한다.
			LIST_TR_CODEINFO CodeList;

			CStringArray arrCode, arrItem;
			theApp.GetGwansimItem(m_pWinixGwanInterface, "최근조회종목_CHART#", arrCode, arrItem);

			int nSize = arrItem.GetSize();	
			CString strData, strCode, strType;
			TR_CODEINFOEX oneCode;
			ZeroMemory(&oneCode, sizeof(TR_CODEINFOEX));
			int nIndex=0;
			for(int nIndx = 0; nIndx < nSize; ++nIndx)
			{
				strCode = arrItem.GetAt(nIndx);
				strType = m_pMDMng->GetCodeInfo("", 2, strCode);
				if(strType == "J" || strType == "Q")
				{
					memcpy(&oneCode.code[0], strCode, 6); 
					memcpy(&oneCode.gb, strType, 1);
					CodeList.AddTail(oneCode);
				}
			}
		
			if(CodeList.GetCount()>0)
				RequestCodeData((void*)pILData->m_pITrComm, &CodeList, pCallback, cDataType, cSort, dwReserved);
			else
				pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);	// 데이타가 없을 때
		}
		else if(!pILData->m_szSection.CompareNoCase("GWANGROUP"))
		{
			cSort = '0';	//관심은 소트하지 않고 그대로..

			CString szInData = pILData->m_szInputData;
			CString szGroup = szInData.Mid(strlen(SZKEY_CODEREQUEST)+1); // ":"으로 구분할 수도 있음.

			// 관심그룹의 리스트를 얻어온다음 TR조회를 요청한다.
			LIST_TR_CODEINFO CodeList;
			GetGwanGroupCode(szGroup, &CodeList);
		
			//@유진변경if(CodeList.GetCount()>0)
			//@유진변경	RequestCodeData((void*)pILData->m_pITrComm, &CodeList, pCallback, cDataType, cSort, dwReserved);
			//@유진변경else
			//@유진변경	pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);	// 데이타가 없을 때

			if(CodeList.GetCount()>0)
			{
				LIST_CCodeTypeA ListTypeA; 
				for(POSITION pos=CodeList.GetHeadPosition(); pos;)
				{
					TR_CODEINFOEX codeInfo = CodeList.GetNext(pos);
					CCodeTypeA typeA;
					typeA.m_cGb = codeInfo.gb;
					typeA.m_szcode = codeInfo.code;
					typeA.m_szhname = codeInfo.codeName;
					
					ListTypeA.AddTail(typeA);
				}
				pCallback->ReceiveData(0, 'A', dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
			}
			else
				pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);	// 데이타가 없을 때

		}
		else if(!pILData->m_szSection.CompareNoCase("USERIMSI"))
		{
			// 임시그룹 리스트 구하기.
			LPCSTR szGroup = "";   // 나중에 임시그룹도 이름이 필요할 지 몰라서.. 지금은 그냥 아무거나..
			LIST_TR_CODEINFO *pCodeList = GetImsiGroupCode(szGroup);
		
			// 구한 종목리스트로 조회하기..
			if(pCodeList && pCodeList->GetCount()>0)
				RequestCodeData((void*)pILData->m_pITrComm, pCodeList, pCallback, cDataType, cSort, dwReserved);
			else
				pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);	// 데이타가 없을 때
		}
		else if(!pILData->m_szSection.CompareNoCase("GROUP"))
		{
			RequestData_TR_Group(pILData, pCallback, cDataType, cSort, dwReserved);
		}
		// 2012.5.8 박성경: 보유종목 로드 >>
		else if(!pILData->m_szSection.CompareNoCase("ACCOUNT"))
		{
			cSort = '0';	// 계좌종목은 소트하지 않고 그대로..
			
			CString szInData = pILData->m_szInputData;
			CString szGroup = szInData.Mid(strlen("NONE")+1); // ":"으로 구분할 수도 있음.
			
			
			SendTr_AccountInfo(szGroup);

			m_CurPackInfo.m_aDataType = cDataType;
			m_CurPackInfo.m_pCallback = pCallback;
			m_CurPackInfo.m_dwReserved = dwReserved;

			return ;
		}
		// 2012.5.8 박성경: 보유종목 로드 <<

	}
	else if(!pILData->m_szMethod.CompareNoCase("CFG"))
	{
	}
	else		// Method가 TR일경우..
		RequestData_TR(pILData, pCallback, cDataType, cSort, dwReserved);
}

void CTreeListManager::RequestData_TR(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
{
//	if(cDataType=='T') {
//		RequestData_Thema_Group(pILData, pCallback, cDataType, cSort, dwReserved);
//		return;
//	}
//	if(cDataType=='t') {
//		RequestData_Thema_Item(pILData, pCallback, cDataType, cSort, dwReserved);
//		return;
//	}

// 	CPacketInfo newInfo;
// 	newInfo.m_aDataType = cDataType;
// 	newInfo.m_pCallback = pCallback;
// 	newInfo.m_dwReserved = dwReserved;
// 	//POSITION pos=m_PacketList.AddTail(newInfo);
	m_CurPackInfo.m_aDataType = cDataType;
	m_CurPackInfo.m_pCallback = pCallback;
	m_CurPackInfo.m_dwReserved = dwReserved;
	
	POSITION pos=(POSITION)100;
	char aReserved[9]; 
	sprintf(aReserved, "%08X", pos);

	ITrComm2* pTrComm = (ITrComm2*)pILData->m_pITrComm;
	if(pTrComm)
	{
		TR_88803_I inData;
		MakeSpaceTR_88803_I(inData);
		memcpy(inData.reserved, aReserved, 8);
		memcpy(inData.mainkey, pILData->m_szInputData, 10);
		inData.sort[0] = cSort;
		inData.datatype[0]= cDataType;

		int nLen = TR_88803_I_SIZE;
		char* szData = (char*)&inData;
		pTrComm->SetTR(pILData->m_szTR);
		pTrComm->ChangeCallback(&m_xTrCommSite);

		m_strReserved.Format("%8.8s", inData.reserved);
		CString strCode;
		strCode.Format("%5.5s", inData.subkey);

//@solomon TrComm에서 함수 삭제로 컴파일 안되서 막음 by 정용호  20091008
// 		CString strQuery="";
// 		CString strKeyList = "";
// 		CString strSaveBuf = "";
// 		CStringArray arFidList;
// 			
// 		pTrComm->FidQueryKeyPartMake ( strKeyList, "3", (LPSTR)(LPCSTR)strCode.Mid(2));   // 종목코드FID
// 		pTrComm->FidQueryKeyPartMake ( strKeyList, "9104", "U");		// 시장구분FID
// 		pTrComm->FidQueryKeyPartMake ( strKeyList, "9103", "4");		// 시장구분FID
// 		if(strCode.GetLength()>2)
// 		{
// 			if( strCode.GetAt(2) == '0')
// 			{
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "242", "2");		// 거래소
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "9210", "0534");		// 시장구분FID
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "GID", "3053");   // 체결GID
// 			}
// 			else if( strCode.GetAt(2) == '1')
// 			{
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "242", "3");		// KOSPI200
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "9221", "1");	// 시장구분FID
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "9210", "0535");	// 시장구분FID
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "GID", "1007");   // 체결GID
// 			}
// 			else if( strCode.GetAt(2) == '2')
// 			{
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "242", "4");		// 코스닥
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "9210", "0534");		// 시장구분FID
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "GID", "3053");   // 체결GID
// 			}
// 			else if( strCode.GetAt(2) == '3')
// 			{
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "242", "5");		// 코스닥50
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "9221", "1");	// 시장구분FID
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "9210", "0535");	// 시장구분FID
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "GID", "1007");   // 체결GID
// 			}
// 			else if( strCode.GetAt(2) == '4')
// 			{
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "242", "10");		// KRX
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "9210", "0534");		// 시장구분FID
// 				pTrComm->FidQueryKeyPartMake ( strKeyList, "GID", "3053");   // 체결GID
// 			}
//		}
//
//
// 		arFidList.Add ( "902");							// 순위번호
// 		arFidList.Add ( "3");							// 시간 
// 		arFidList.Add ( "1");							// 종목코드 
// 		arFidList.Add ( "4");							// 현재가(색정보*)FID번호  
// 		arFidList.Add ( "5");							// 대비부호  
// 		arFidList.Add ( "6");							// 전일대비  
// 		arFidList.Add ( "7");							// 등락률  
// 
// 		int nQueryCnt = MAX_MASTERQUERY_CNT;
// 		nLen = pTrComm->FidArrayQueryMake ( strQuery, strKeyList,  strSaveBuf , arFidList, 0, nQueryCnt);
// 
// 		//화면번호를 세팅한다.
// 		CString strScreenNo;
// 		GetBaseData("SCREENCODE", strScreenNo);
// 		pTrComm->SetBaseData("SCREENCODE", strScreenNo);
// 		pTrComm->Send2Server(szData, nLen);
// 
// 		TRACE("CTreeListManager::RequestData:SendPacket[TR[%s]%*.*s]\n", pILData->m_szTR, nLen, nLen, szData);
	}

return;
//	if(cDataType=='A')
//	{
//		if(!pILData->m_szMethod.CompareNoCase("MASTER"))
//			RequestData_A_MASTER(pILData, pCallback, cDataType, dwReserved);
//		else if(!pILData->m_szMethod.CompareNoCase("TR"))
//				RequestData_A_Tr(pILData, pCallback, cDataType, dwReserved);
//		else if(!pILData->m_szMethod.CompareNoCase("SUBMASTER"))
//			RequestData_A_SUBMASTER(pILData, pCallback, cDataType, dwReserved);
//		else
//			AfxMessageBox("데이터포맷이상(00001)");
//	}
//	else if(cDataType=='B')
//	{
//		if(!pILData->m_szMethod.CompareNoCase("MASTER"))
//			RequestData_B_MASTER(pILData, pCallback, cDataType, dwReserved);
//		else if(!pILData->m_szMethod.CompareNoCase("TR"))
//				RequestData_B_Tr(pILData, pCallback, cDataType, dwReserved);
//		else if(!pILData->m_szMethod.CompareNoCase("SUBMASTER"))
//			RequestData_B_SUBMASTER(pILData, pCallback, cDataType, dwReserved);
////		if(!pILData->m_szMethod.CompareNoCase("TR"))
////			RequestData_B_Tr(pILData, pCallback, cDataType, dwReserved);
//		else
//			AfxMessageBox("현재지원되지 않습니다.060107_01");
//	}
//	else
//	{
//		AfxMessageBox("데이터포맷이상(00003)");
//	}
}

void CTreeListManager::RequestData_TR_Group(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
{
// 	CPacketInfo newInfo;
// 	newInfo.m_aDataType = cDataType;
// 	newInfo.m_pCallback = pCallback;
// 	newInfo.m_dwReserved = dwReserved;
// 	//POSITION pos=m_PacketList.AddTail(newInfo);
	m_CurPackInfo.m_aDataType = cDataType;
	m_CurPackInfo.m_pCallback = pCallback;
	m_CurPackInfo.m_dwReserved = dwReserved;

	POSITION pos=(POSITION)100;
	char aReserved[9]; 
	sprintf(aReserved, "%08X", pos);

	ITrComm2* pTrComm = (ITrComm2*)pILData->m_pITrComm;
	if(pTrComm)
	{
		TR_88803_I inData;
		MakeSpaceTR_88803_I(inData);
		memcpy(inData.reserved, aReserved, 8);
		memcpy(inData.mainkey, pILData->m_szInputData, 10);
		inData.sort[0] = cSort;
		inData.datatype[0]= cDataType;

		int nLen = TR_88803_I_SIZE;
		char* szData = (char*)&inData;
		pTrComm->SetTR(pILData->m_szTR);
		pTrComm->ChangeCallback(&m_xTrCommSite);
		m_strReserved.Format("%8.8s", inData.reserved);
		CString strCode;
		strCode.Format("%5.5s", inData.subkey);

//@solomon TrComm에서 함수 삭제로 컴파일 안되서 막음 by 정용호  20091008
// 		CString strQuery="";
// 		CString strKeyList = "";
// 		CString strSaveBuf = "";
// 		CStringArray arFidList;
// 
// 		pTrComm->FidQueryKeyPartMake ( strKeyList, "9100", (LPSTR)(LPCSTR)strCode.Mid(2));  // 그룹코드FID
// 		pTrComm->FidQueryKeyPartMake ( strKeyList, "9210", "0514");  // 실행유형코드FID
// 		pTrComm->FidQueryKeyPartMake ( strKeyList, "GID", "1007");   // GID
// 
// 		arFidList.Add ( "902");							// 순위번호
// 		arFidList.Add ( "3");							// 종목명
// 		arFidList.Add ( "1");							// 종목코드 
// 		arFidList.Add ( "4");							// 현재가(색정보*)FID번호  
// 		arFidList.Add ( "5");							// 대비부호  
// 		arFidList.Add ( "6");							// 전일대비  
// 		arFidList.Add ( "7");							// 등락률  
// 
// 		nLen = pTrComm->FidArrayQueryMake ( strQuery, strKeyList,  strSaveBuf , arFidList, 0, 1000);
// 
// 		//화면번호를 세팅한다.
// 		CString strScreenNo;
// 		GetBaseData("SCREENCODE", strScreenNo);
// 		pTrComm->SetBaseData("SCREENCODE", strScreenNo);
// 		pTrComm->Send2Server(szData, nLen);
// 
// 		TRACE("CTreeListManager::RequestData:SendPacket[TR[%s]%*.*s]\n", pILData->m_szTR, nLen, nLen, szData);
	}

	return;
}

void CTreeListManager::ChangeConfigFile(LPCSTR szFileName)
{
	m_szConfigFile = szFileName;
}

void* CTreeListManager::GetITrComm(LONG dwKey)
{
	if(dwKey)
	{
		CTreeResItemEx* pList = (CTreeResItemEx*)m_AllList.GetAt(POSITION(dwKey));
		if(pList) return pList->m_pTrComm;
	}
	return NULL;
}

void CTreeListManager::RequestCodeData(void* pITrComm, LIST_TR_CODEINFO* pCodeList, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
{
	if(pCodeList->GetCount()==0) return;

// 	CPacketInfo newInfo;
// 	newInfo.m_aDataType = cDataType;
// 	newInfo.m_pCallback = pCallback;
// 	newInfo.m_dwReserved = dwReserved;
// 	//POSITION pos=m_PacketList.AddTail(newInfo);
	m_CurPackInfo.m_aDataType = cDataType;
	m_CurPackInfo.m_pCallback = pCallback;
	m_CurPackInfo.m_dwReserved = dwReserved;

	POSITION pos=(POSITION)100;
	char aReserved[9]; 
	sprintf(aReserved, "%08X", pos);
	char aCount[5];
	sprintf(aCount, "%04d", pCodeList->GetCount());
	ITrComm2* pTrComm = (ITrComm2*)pITrComm;
	CStringArray arFidList, arKeyList;
	if(pTrComm)
	{
		m_strReserved.Format("%8.8s", aReserved);
		int nLen = TR_88803_I_SIZE + (TR_CODEINFO_SIZE * pCodeList->GetCount());
		char* pReadData = (char*)malloc(nLen);
		memset(pReadData, 0x20, nLen);
		TR_88803_I* pInData = (TR_88803_I*)pReadData ;
		memcpy(pInData->reserved, aReserved, 8);
		memcpy(pInData->mainkey, SZKEY_CODEREQUEST, 10);
		pInData->sort[0] = cSort;
		pInData->datatype[0]= cDataType;
		memcpy(pInData->count, aCount, 4);
		TR_CODEINFO oneCode;
		int nOffset = 0;
		char cCode[7+1]={0,}; 
		for(POSITION pos=pCodeList->GetHeadPosition(); pos; )
		{
			oneCode = pCodeList->GetNext(pos);
			memcpy(cCode, &oneCode, 7);
			//memcpy(&pInData->data[nOffset], &oneCode, TR_CODEINFO_SIZE);
			//nOffset += TR_CODEINFO_SIZE;
			arKeyList.Add(cCode);						// 시장구분+코드  
		}
		arFidList.Add ( "902");							// 순위번호
		arFidList.Add ( "3");							// 시간 
		arFidList.Add ( "1");							// 종목코드 
		arFidList.Add ( "4");							// 현재가(색정보*)FID번호  
		arFidList.Add ( "5");							// 대비부호  
		arFidList.Add ( "6");							// 전일대비  
		arFidList.Add ( "7");							// 등락률  

//@solomon TrComm에서 함수 삭제로 컴파일 안되서 막음 by 정용호  20091008
// 		pTrComm->SetTR(SZKEY_TRNO);
// 		pTrComm->ChangeCallback(&m_xTrCommSite);
// 		int nType = 0;
// 		int nGidNum = 1108;							// 관심종목GID 
// 		CString strQuery;
// 		nLen = pTrComm->FidPortPolioQueryMake ( strQuery, nType, nGidNum, arKeyList, arFidList, 0 );
// 
// 		//화면번호를 세팅한다.
// 		CString strScreenNo;
// 		GetBaseData("SCREENCODE", strScreenNo);
// 		pTrComm->SetBaseData("SCREENCODE", strScreenNo);
// 		pTrComm->Send2Server(pReadData, nLen);
		free(pReadData);
	}
	//CPacketInfo newInfo;
	//newInfo.m_aDataType = cDataType;
	//newInfo.m_pCallback = pCallback;
	//newInfo.m_dwReserved = dwReserved;
	//POSITION pos=m_PacketList.AddTail(newInfo);
	//
	//char aReserved[9]; 
	//sprintf(aReserved, "%08X", pos);

	//char aCount[5];
	//sprintf(aCount, "%04d", pCodeList->GetCount());

	//ITrComm* pTrComm = (ITrComm*)pITrComm;
	//if(pTrComm)
	//{
	//	int nLen = TR_88803_I_SIZE + (TR_CODEINFO_SIZE * pCodeList->GetCount());
	//	char* pReadData = (char*)malloc(nLen);
	//	memset(pReadData, 0x20, nLen);
	//	TR_88803_I* pInData = (TR_88803_I*)pReadData ;
	//	memcpy(pInData->reserved, aReserved, 8);
	//	memcpy(pInData->mainkey, SZKEY_CODEREQUEST, 10);
	//	pInData->sort[0] = cSort;
	//	pInData->datatype[0]= cDataType;
	//	memcpy(pInData->count, aCount, 4);

	//	TR_CODEINFO oneCode;
	//	int nOffset = 0;
	//	for(POSITION pos=pCodeList->GetHeadPosition(); pos; )
	//	{
	//		oneCode = pCodeList->GetNext(pos);
	//		memcpy(&pInData->data[nOffset], &oneCode, TR_CODEINFO_SIZE);
	//		nOffset += TR_CODEINFO_SIZE;
	//	}

	//	pTrComm->SetTR(SZKEY_TRNO);
	//	pTrComm->ChangeCallback(&m_xTrCommSite);
	//	pTrComm->Send2Server(pReadData, nLen);

	//	free(pReadData);
	//}
}

//*****************************************************************************
// 루    틴    명 :  ReloadGwansimGroup
// 기          능 :  관심의 그룹을 다시 연다.
// 입          력 :  long dwKey
//                   CTreeCtrl* pTreeCtrl
// 출          력 :  void
// 작    성    자 :  2006년 2월 13일   이 준옥.
// 설          명 :  관심그룹의 변경시에는 기존의 TREE를 재구성한다.
//                   1. 관심TREE의 하위정보를 Clear
//                   2. 새로운 정보로 다시 로딩한다.
// 수  정  사  항 :  
//*****************************************************************************
void CTreeListManager::ReloadGwansimGroup(LONG dwKey, CTreeCtrl* pTreeCtrl)
{
	//if(dwKey==0) return;

	//HTREEITEM hGwan = GetGwansimItem(pTreeCtrl);
	//if(!hGwan) return;

	//DeleteTreeChildInfo(pTreeCtrl, hGwan);

	//CTreeResItemEx* pList = (CTreeResItemEx*)m_AllList.GetAt((POSITION)dwKey);

	//CString szTr = SZKEY_TRNO;
	//CString szInputData = SZKEY_CODEREQUEST;
	//LoadGwansimGroup(pList, pTreeCtrl, hGwan, szTr, szInputData);
}

HTREEITEM CTreeListManager::GetGwansimItem(CTreeCtrl* pTreeCtrl)
{
//	return CHelperTreeCtrl::FindTreeItem(pTreeCtrl, GWAMSIM_DEF_NAME);
	return NULL;
}

HTREEITEM CTreeListManager::GetTreeItem(CTreeCtrl* pTreeCtrl, LPCSTR szFolder, HTREEITEM hTreeRoot)
{
	return CHelperTreeCtrl::FindTreeItem(pTreeCtrl, szFolder);
}

void CTreeListManager::GwansimRegist(long dwData)
{
	//IMainExInfoManager* pMng = (IMainExInfoManager*)m_pGeteMng->GetInterface(22);
	//COleVariant varCode(dwData);
	//varCode.vt = VT_I4;
	//pMng->SetData(IMainExInfoManager_Gwansim, &varCode);
}

int CTreeListManager::ImsiRegist(LPCSTR szSubKey, long dwData)
{
//	ImsiDelete(szSubKey);

	//IMainExInfoManager* pMng = (IMainExInfoManager*)m_pGeteMng->GetInterface(22);
	//COleVariant varCode((long)dwData);
	//varCode.vt = VT_I4;
	//pMng->SetData(IMainExInfoManager_ImsiGroup, &varCode);

	//LIST_TR_CODEINFO* pList = (LIST_TR_CODEINFO*)dwData;
	//if(pList) {
	//	int nCount = pList->GetCount();
	//	return nCount;
	//}
	return 0;
}


/*
int CTreeListManager::ImsiAppend(LPCSTR szSubKey, long dwData)
{
	return 0;
	// 현재 
	// 아래 루틴은 검증이 되지 않은 루틴이므로 무조건 풀지 마세요^^^

	if(!dwData) return 0;

	IMainExInfoManager* pMng = (IMainExInfoManager*)m_pGeteMng->GetInterface(22);
	COleVariant* pVar = pMng->GetData(IMainExInfoManager_ImsiGroup);
	if(pVar) {
		LIST_TR_CODEINFO* pList = (LIST_TR_CODEINFO*)pVar->lVal;
		LIST_TR_CODEINFO* pArg = (LIST_TR_CODEINFO*)dwData;

		if(pList)
		{
			for(POSITION pos=pArg->GetHeadPosition(); pos; )
			{
				pList->AddTail(pArg->GetNext(pos));
			}
			COleVariant varCode((long)pList);
			varCode.vt = VT_I4;
			pMng->SetData(IMainExInfoManager_ImsiGroup, &varCode);
			return pList->GetCount();
		}
		else
		{
			COleVariant varCode((long)pArg);
			varCode.vt = VT_I4;
			pMng->SetData(IMainExInfoManager_ImsiGroup, &varCode);
			return pArg->GetCount();
		}
	}
	return 0;
}
*/


int CTreeListManager::ImsiDelete(LPCSTR szSubKey)
{
	//IMainExInfoManager* pMng = (IMainExInfoManager*)m_pGeteMng->GetInterface(22);
	//COleVariant* pVar = pMng->GetData(IMainExInfoManager_ImsiGroup);
	//if(pVar) 
	//{
	//	LIST_TR_CODEINFO* pList = (LIST_TR_CODEINFO*)pVar->lVal;
	//	if(pList)
	//	{
	//		pList->RemoveAll();
	//		delete pList;
	//	}
	//}

	//LIST_TR_CODEINFO* pList = NULL;
	//COleVariant varCode((long)pList);
	//varCode.vt = VT_I4;
	//pMng->SetData(IMainExInfoManager_ImsiGroup, &varCode);

	return 0;

}

void CTreeListManager::SetBaseData(LPCSTR szKey, LPCSTR  szValue)
{
	m_DataTable.SetAt(szKey, szValue);
}

void CTreeListManager::GetBaseData(LPCSTR szKey, CString &rValue)
{
	rValue.Empty();
	m_DataTable.Lookup(szKey, rValue);
}

void CTreeListManager::SetSendKey(HWND hWinix, HWND hDrfnWrap)
{
	m_hWinixWnd = hWinix;
	m_hWrapper = hDrfnWrap;
}

void CTreeListManager::SetGwanInterface(long pInterface)
{
	m_pWinixGwanInterface = pInterface;
}

BOOL CTreeListManager::LoadOnlnyGwansimGroup(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData)
{
//	CString szSubKey;
//	int nMaxCount=ITreeListManager_TREEMAXCOUNT;
//	char szLine[MAX_PATH]={0,}, szTR[10]={0,};
//	CItemLongData clItem;
//	HTREEITEM hTree = NULL;
//
//	::GetPrivateProfileString(szSection, "TR", "88803", szTR, 10, szFile);
//	for(int i=0; i<nMaxCount; i++)
//	{
//		szSubKey.Format("%02d", i+1);
//		::GetPrivateProfileString(szSection, szSubKey, "", szLine, MAX_PATH, szFile);
//		if(strlen(szLine)>0)
//		{
//			clItem.m_szTR = szTR;
//			clItem.Convert(szLine);
//			hTree = pTree->InsertItem(clItem.m_szTitle, hTreeItem);
//			if(clItem.m_szSub=="SUB_YES")
//			{
//				// 관심그룹 구하기..
//				if(clItem.m_szSection.CompareNoCase("GWANSIM")==0)
//				{
//					hGwanTree = hTree;
//					LPCSTR szInputData = SZKEY_CODEREQUEST;	//clItem.m_szInputData
//					LoadGwansimGroup(pList, pTree, hTree, clItem.m_szTR, szInputData);
//					return TRUE;
//				}
//			}
//		}
//	}
	return FALSE;
}


// ----------------------------------------------------------------------
// CTreeListManager
CTreeListManager::CTreeListManager()
{
	m_pGeteMng = NULL;
	m_pMainInfoMng = NULL;
	m_pTrCommMng = NULL;

	m_pTreeCtrl = NULL;
	m_pListCtrl = NULL;
	m_xTrCommSite.m_pTLMParent = this;

	hGwanTree = NULL;	// 관심그룹의 HTREEITEM값을 저장한다.
	m_bLoadCondMng = FALSE;

	m_hWinixWnd = NULL;
	m_hWrapper = NULL;
	m_pWinixGwanInterface = 0;
}

CTreeListManager::~CTreeListManager()
{
	if(m_bLoadCondMng)
		m_LibCondNng.UnLoadLib();
	ClearAll();
}

void CTreeListManager::ClearAll()
{
//	if(m_pTrComm) {
//		m_pTrCommMng->UnAdviseTrComm(m_pTrComm);
//		m_pTrComm = NULL;
//	}
//	m_PacketList.RemoveAll();
	for(POSITION pos=m_AllList.GetHeadPosition(); pos; )
	{
		CTreeResItemEx* pList = (CTreeResItemEx*)m_AllList.GetNext(pos);
		ClearList(pList->m_pItemList);
		delete pList->m_pItemList;
		delete pList;
	}
	m_AllList.RemoveAll();
}

//void CTreeListManager::ClearList(LIST_CItemLongData* pList)
//{
//	//if(pList)	LIST_DELETEALL2(pList,CItemLongData);
//	if(!pList || pList->GetCount()==0) return;
//
//	for(POSITION pos=pList->GetHeadPosition(); pos;)
//	{
//		CItemLongData* pItem = pList->GetNext(pos);		
//		if(pItem) delete pItem;						
//	}
//	pList->RemoveAll();
//}
void CTreeListManager::ClearList(LIST_CBaseLongData* pList)
{
	//if(pList)	LIST_DELETEALL2(pList,CItemLongData);
	if(!pList || pList->GetCount()==0) return;
	CBaseLongData* pBaseItem;
	for(POSITION pos=pList->GetHeadPosition(); pos;)
	{
		pBaseItem = pList->GetNext(pos);
		if(!pBaseItem) continue;

		if(pBaseItem->m_nType==BaseLongData_Item)
		{
			CItemLongData* pItem = (CItemLongData*)pBaseItem;		
			if(pItem) delete pItem;						
		}
		else if(pBaseItem->m_nType==BaseLongData_OneClick)
		{
			CItemOneClickData* pItem = (CItemOneClickData*)pBaseItem;	
			if(pItem) delete pItem;	
		}
	}
	pList->RemoveAll();
}

void CTreeListManager::SetIGateManager(long dwInterface)
{
	m_pGeteMng = (IGateManager*)dwInterface;
	m_pMainInfoMng = (IMainInfoManager*)m_pGeteMng->GetInterface(UUID_IMainInfoManager);
	m_pMDMng = (IMasterDataManagerLast*)m_pGeteMng->GetInterface(UUID_IMasterDataManager);
	m_pTrCommMng = (IAUTrCommManager*)m_pGeteMng->GetInterface(UUID_IAUTrCommManager);
//	m_pTrComm = m_pTrCommMng->AdviseTrComm();

	m_szConfigFile.Format("%s%s", m_pMainInfoMng->GetDataDir() , "\\CodeItem.cfg");
}


// ------------------------------------------------------------------------------------------------------
// FunctionName : CTreeListManager::RequestData_A_MASTER
//
// ------------------------------------------------------------------------------------------------------
void CTreeListManager::RequestData_A_MASTER(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, long dwReserved)
{
	CString szMasterKey = pILData->m_szSection;
	CString szInputData = pILData->m_szInputData;
	LIST_CCodeTypeA ListTypeA; 

//-->유진수정
	Callback_KSMaster(szMasterKey, &ListTypeA, szInputData);
//	if(!szMasterKey.CompareNoCase("Master_J_K")) 
//	{
//		Callback_CJMaster(&ListTypeA);
//		Callback_CKMaster(&ListTypeA);
//	}
//	else if(!szMasterKey.CompareNoCase("JMaster"))		Callback_CJMaster(&ListTypeA);
//	else if(!szMasterKey.CompareNoCase("KMaster"))		Callback_CKMaster(&ListTypeA);
//	else if(!szMasterKey.CompareNoCase("UMaster"))		Callback_CUMaster(&ListTypeA, szInputData);
//	else if(!szMasterKey.CompareNoCase("KUMaster"))		Callback_CKUMaster(&ListTypeA);
//	else if(!szMasterKey.CompareNoCase("TMaster"))		Callback_CTMaster(&ListTypeA);
//	else if(!szMasterKey.CompareNoCase("ETFMaster"))	Callback_CETFMaster(&ListTypeA);
//	else if(!szMasterKey.CompareNoCase("ELWMaster"))	Callback_CELWMaster(&ListTypeA);
//	else if(!szMasterKey.CompareNoCase("NONE"))			Callback_NONE(&ListTypeA);
//	else
//	{
//		AfxMessageBox("지원하지 않는 데이터(0004)");
//		pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);
//		return;
//	}
	//pCallback->ReceiveData(0, cDataType, dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
	pCallback->ReceiveData(0, 'A', dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
//<--유진수정
}


// ------------------------------------------------------------------------------------------------------
// FunctionName : CTreeListManager::Callback_JMaster
// FunctionName : CTreeListManager::Callback_KMaster
// FunctionName : CTreeListManager::Callback_UMaster
// FunctionName : CTreeListManager::Callback_KUMaster
// FunctionName : CTreeListManager::Callback_ETFMaster
// FunctionName : CTreeListManager::Callback_ELWMaster
// ------------------------------------------------------------------------------------------------------


//FN_Callback_Master(CJMaster, "JMASTER"		,'1');
//FN_Callback_Master(CKMaster, "KMASTER"		,'2');
//FN_Callback_Master(CUMaster, "UMASTER"		,'1');
//FN_Callback_Master(CKUMaster, "UKMASTER"	,'2'); // Code.ocx에서 외부노출 클래스명을 CKUMaster으로 관리하면서, 마스터키는 UKMASTER로 관리하기때문에 이렇게 처리함.
//FN_Callback_Master(CTMaster, "TMASTER"		,'0');
//FN_Callback_Master(CETFMaster, "ETFMASTER"	,'0');
//FN_Callback_Master(CELWMaster, "ELWMASTER"	,'0');

void CTreeListManager::Callback_CELWMaster(LIST_CCodeTypeA* pListTypeA){}
void CTreeListManager::Callback_CETFMaster(LIST_CCodeTypeA* pListTypeA){}
void CTreeListManager::Callback_CTMaster(LIST_CCodeTypeA* pListTypeA){}
void CTreeListManager::Callback_CKUMaster(LIST_CCodeTypeA* pListTypeA){}
void CTreeListManager::Callback_CUMaster(LIST_CCodeTypeA* pListTypeA){}
void CTreeListManager::Callback_CKMaster(LIST_CCodeTypeA* pListTypeA){}
void CTreeListManager::Callback_CJMaster(LIST_CCodeTypeA* pListTypeA){}
//int CTreeListManager::LoadTree_CGMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1){return 0;}
//int CTreeListManager::LoadTree_CTHMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1){return 0;}

void CTreeListManager::Callback_KSMaster(LPCSTR szMasterKey, LIST_CCodeTypeA* pListTypeA, CString &szInputData)
{
	if(m_pMDMng)
	{
		CString szType = szMasterKey;
		CStringArray arrCode;
		CStringArray arrBuf;
		CString strUp = szInputData;

		m_pMDMng->GetMasterDataInUpDetails(szType, arrCode, arrBuf, strUp);

		for(int i=0; i<arrCode.GetSize(); i++)
		{
			CCodeTypeA typeA;
			typeA.m_cGb = '1';
			typeA.m_szcode = arrCode.GetAt(i);
			typeA.m_szhname = arrBuf.GetAt(i);

			pListTypeA->AddTail(typeA);
		}
	}
}

/*
void CTreeListManager::Callback_CJMaster(LIST_CCodeTypeA* pListTypeA)
{
	if(m_pMDMng)
	{
		CList<CJMaster, CJMaster>* list = (CList<CJMaster, CJMaster>*)m_pMDMng->GetCodePointer("JMASTER");
		if(!list)
		{
			return;
		}
		for(POSITION pos=list->GetHeadPosition(); pos;)
		{
			CJMaster data = list->GetNext(pos);
			CCodeTypeA typeA;
			typeA.m_cGb = '1';
			typeA.m_szcode = data.m_szCode;

			typeA.m_szhname = data.m_szName;
			pListTypeA->AddTail(typeA);
		}
	}
}
*/

/*
void CTreeListManager::Callback_CJMaster(LIST_CCodeTypeA* pListTypeA)
{																											
	if(m_pMDMng)																							
	{																										
		CList<CJMaster, CJMaster>* list = (CList<CJMaster, CJMaster>*)m_pMDMng->GetCodePointer("JMASTER");	
		if(!list)																							
		{																									
			return;																							
		}																									
		for(POSITION pos=list->GetHeadPosition(); pos;)														
		{																									
			CJMaster data = list->GetNext(pos);																
			CCodeTypeA typeA;																				
			typeA.m_szcode = data.m_szCode;																	
			typeA.m_szhname = data.m_szName;																
			pListTypeA->AddTail(typeA);																		
		}																									
	}																										
}
*/

/*
void CTreeListManager::Callback_CETFMaster(LIST_CCodeTypeA* pListTypeA)
{																											
	if(m_pMDMng)																							
	{																										
		CList<CETFMaster, CETFMaster>* list = (CList<CETFMaster, CETFMaster>*)m_pMDMng->GetCodePointer("ETFMASTER");	
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
}*/


//void CTreeListManager::Callback_Master_J_K(LIST_CCodeTypeA* pListTypeA)
//{
//	if(m_pMDMng)
//	{
//		int nTotal = m_pMDMng->LoadMASTER_J_K();
//		CMaster_J_K* pData = m_pMDMng->J_K_GetMaster();
//		for(int i=0; i<nTotal; i++)
//		{
//			CCodeTypeA typeA;
//			typeA.m_szcode = pData[i].m_szCode;
//			typeA.m_szhname = pData[i].m_szName;
//			pListTypeA->AddTail(typeA);
//		}
//		m_pMDMng->UnLoadMASTER_J_K();
//	}
//}

void CTreeListManager::Callback_NONE(LIST_CCodeTypeA* pListTypeA)
{
}

long CTreeListManager::GetBitFromCodeInfo(CString& szWonjoCodeChoice)
{
	long nBit = 0;
	int n = 0;
	
	for(int i = 0 ; i < szWonjoCodeChoice.GetLength(); i++)
	{
		if(szWonjoCodeChoice[i] == '1') 
			n = 1;
		else 
			n = 0;
		
		nBit |= ( n << (szWonjoCodeChoice.GetLength() - i-1));
	}
	return nBit;
}

// 변수 : "code0=name0;code1=name1" 
// 리턴값 : code0=name0, code1=name1 의 CStringList
// CodeInput에서 소스 가져옴.
int CTreeListManager::FP_strlistHistory(CString strHistory, CStringList &strList)
{
	CString szFormer = strHistory, szLatter="";
	int nIndex = strHistory.Find(';');
	while(nIndex>0)
	{
		szLatter = szFormer.Mid(nIndex+1);
		szFormer = szFormer.Left(nIndex);
		strList.AddTail(szFormer);
		szFormer = szLatter;
		nIndex = szFormer.Find(';');
	}
	
	return strList.GetCount();
}

void CTreeListManager::CallHistory(LIST_CCodeTypeA* pListTypeA)
{
	//short nMaxCount=100;
	//CString szWonjoCodeChoice="11000000000000000000000000000"; // CodeInput 또는 Codeocx참조.
	//long  nType=GetBitFromCodeInfo(szWonjoCodeChoice);
	//CString strHistory = m_pMDMng->GetHistoryList(nMaxCount, nType);

	//CStringList strList;
	//FP_strlistHistory(strHistory, strList);

	////CString szCode, szName, szGubun;
	//char aCode[7]={0,}, aName[31]={0,}, aGubun[2]={0,};
	//CString szList;
	////int nIndex = 0, nItem = 0;
	//int nCount = strList.GetCount() -1;
	//for(;nCount>=0;nCount--)
	//{
	//	szList = strList.GetAt(strList.FindIndex(nCount));
	//	sscanf(szList, "%[^=]=%[^=]=%s", aCode, aName, aGubun);

	//	CCodeTypeA typeA;
	//	typeA.m_cGb = aGubun[0];
	//	typeA.m_szhname = aName; // szCode.Mid(nIndex+1);
	//	typeA.m_szcode  = aCode; // szCode.Left(nIndex);
	//	pListTypeA->AddTail(typeA);
	//}

}


void CTreeListManager::RequestData_B_MASTER(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, long dwReserved)
{
/*	
	CString szMasterKey = pILData->m_szInputData;
	LIST_CCodeTypeA ListTypeA; 

	if(!szMasterKey.CompareNoCase("Master_J_K")) 
	{
		Callback_CJMaster(&ListTypeA);
		Callback_CKMaster(&ListTypeA);
	}
	else if(!szMasterKey.CompareNoCase("JMaster"))		Callback_CJMaster(&ListTypeA);
	else if(!szMasterKey.CompareNoCase("KMaster"))		Callback_CKMaster(&ListTypeA);
	else if(!szMasterKey.CompareNoCase("UMaster"))		Callback_CUMaster(&ListTypeA);
	else if(!szMasterKey.CompareNoCase("KUMaster"))		Callback_CKUMaster(&ListTypeA);
	else if(!szMasterKey.CompareNoCase("ETFMaster"))	Callback_CETFMaster(&ListTypeA);
	else if(!szMasterKey.CompareNoCase("NONE"))			Callback_NONE(&ListTypeA);
	else
	{
		AfxMessageBox("지원하지 않는 데이터(0004)");
		pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);
		return;
	}

// 아래는 작업중.........	

	TRACE("CTreeListManager::RequestData_B\n");
	pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);
*/
}

void CTreeListManager::RequestData_A_Tr(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, long dwReserved)
{
	TRACE("CTreeListManager::RequestData_A_Tr\n");
	pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);
}

void CTreeListManager::RequestData_B_Tr(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, long dwReserved)
{
	TRACE("CTreeListManager::RequestData_B_Tr\n");
	pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);
}


BOOL CTreeListManager::LoadPCMASTER(LIST_CBaseLongData *pList, HTREEITEM hTree, LPCSTR szKey)
{
	return TRUE;
}

/*
	TV_INSERTSTRUCT treeitem;

	treeitem.hParent = _hParent;
	treeitem.hInsertAfter = TVI_LAST;
	treeitem.item.mask = TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	treeitem.item.pszText = _pText;
	treeitem.item.iImage = _nBmpNumber;
	treeitem.item.iSelectedImage = _nBmpNumber+1;
	treeitem.item.lParam = NULL;
	
	HTREEITEM hItem = InsertItem(&treeitem);
*/

#define		ITreeListManager_TREEMAXCOUNT		50
BOOL CTreeListManager::LoadTreeConfig(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hTreeItem, LPCSTR szSection, LPCSTR szFile, CListCtrl* pListCtrl)
{
//	CString szSubKey("COUNT");
//	int nCount = ::GetPrivateProfileInt(szSection, szSubKey,0, szFile);

	CString szSubKey;
	int nMaxCount=ITreeListManager_TREEMAXCOUNT;
	char szLine[MAX_PATH]={0,}, szTR[10]={0,};
	CItemLongData clItem;
	HTREEITEM hTree = NULL;

	// 2012.5.8 박성경: 보유종목 로드 >>
	int nPos;
	int i = 0;
	CString strScreenNo, strTitle, strTemp, strSection(szSection);
	GetBaseData("SCREENCODE", strScreenNo);
	// 2012.5.8 박성경: 보유종목 로드 <<

	::GetPrivateProfileString(szSection, "TR", "88803", szTR, 10, szFile);
	for( i=0; i<nMaxCount; i++)
	{
		szSubKey.Format("%02d", i+1);
		::GetPrivateProfileString(szSection, szSubKey, "", szLine, MAX_PATH, szFile);

		// 2012.5.8 박성경: 보유종목 로드 >>
		strTemp.Format("%s", szLine);
		nPos = strTemp.Find(",");
		strTitle.Format("%s", strTemp.Left(nPos));

 		if (!strTitle.CompareNoCase("보유종목") && !strSection.CompareNoCase("선물/옵션") && (strScreenNo == "7111" || strScreenNo == "7126"))
 			continue;
		// 2012.5.8 박성경: 보유종목 로드 <<

		if(strlen(szLine)>0)
		{
			clItem.m_szTR = szTR;
			clItem.Convert(szLine);
			if(clItem.m_szSub=="SUB_NO_LIST")
			{
				if(clItem.m_szSection.CompareNoCase("GWANSIM")==0)
				{
					hGwanTree = hTree;
					LPCSTR szInputData = SZKEY_CODEREQUEST;	//clItem.m_szInputData
					if(pTree)LoadGwansimGroup(pList, pTree, hTreeItem, clItem.m_szTR, szInputData);
					else LoadGwansimGroup(pList, pListCtrl, clItem.m_szTR, szInputData);
				}
				else if(clItem.m_szSection.CompareNoCase("ONECLICK")==0)
				{
					LoadConditonMng(pList, pTree, hTreeItem);
				}
				else if(clItem.m_szSection.CompareNoCase("USERCOND")==0 )
				{
					LoadConditonMng2(pList, pTree, hTreeItem);
				}
				else if(clItem.m_szSection.CompareNoCase("USERIMSI")==0 )		
				{
					LoadConditonMngUser(pList, pTree, hTreeItem);
				}
//-->유진추가 080331수JunokLee
				else if(clItem.m_szMethod=="MASTER")
				{
					LoadTreeConfig_Master(pList, pTree, hTreeItem, clItem.m_szSection, clItem.m_szTR, clItem.m_szInputData);
				}
//<<--유진추가 080331수JunokLee
				continue;
			}
			hTree = pTree->InsertItem(clItem.m_szTitle, hTreeItem);
			if(clItem.m_szSub=="SUB_NO")
			{
				CItemLongData* pNew = new CItemLongData;
				pNew->SetCopy(&clItem);
				pNew->m_pITrComm = (long)pList->m_pTrComm;
				pTree->SetItemData(hTree, (DWORD)pNew);
				pList->m_pItemList->AddTail(pNew);
			}

			if(clItem.m_szSub=="SUB_YES")
			{
//				CItemLongData* pNew = new CItemLongData;
//				pNew->SetCopy(&clItem);

				//06.12.21 테마조회[[
				// 테마종목은 조회를 해야하기 때문에 TRComm관련 세팅을 해준다.
//				if(clItem.m_szTitle.Compare("테마종목")==0)		pNew->m_pITrComm = (long)pList->m_pTrComm;
				//06.12.21 테마조회]]

//				pTree->SetItemData(hTree, (DWORD)pNew);
//				pList->m_pItemList->AddTail(pNew);

				if(clItem.m_szMethod=="CFG")
					LoadTreeConfig(pList, pTree, hTree, clItem.m_szSection, szFile);
				else if(clItem.m_szMethod=="MASTER")
					LoadTreeConfig_Master(pList, pTree, hTree, clItem.m_szSection, clItem.m_szTR, clItem.m_szInputData);
				// 관심그룹 구하기..
				else if(clItem.m_szSection.CompareNoCase("GWANSIM")==0)
				{
					hGwanTree = hTree;
					LPCSTR szInputData = SZKEY_CODEREQUEST;	//clItem.m_szInputData
					if(!pListCtrl)
						LoadGwansimGroup(pList, pTree, hTree, clItem.m_szTR, szInputData);
					else
						LoadGwansimGroup(pList, pListCtrl, clItem.m_szTR, szInputData);
				}
				else if(clItem.m_szSection.CompareNoCase("ONECLICK")==0)
				{
					LoadConditonMng(pList, pTree, hTree);
				}
				else if(clItem.m_szSection.CompareNoCase("USERIMSI")==0)
				{
					LoadConditonMngUser(pList, pTree, hTree);
				}
				// 2012.5.8 박성경: 보유종목 로드 >>
				else if(clItem.m_szSection.CompareNoCase("ACCOUNT")==0)
				{
                                        m_clItem.SetCopy(&clItem);//tour2k
					LoadAccountGroup(pList, pTree, hTree, clItem.m_szTR, clItem.m_szInputData);
				}
				// 2012.5.8 박성경: 보유종목 로드 <<
			}
		}
		else break;
	}

	int nCount = i;

	return TRUE;
}


//BOOL CTreeListManager::LoadConfig(CTreeCtrl* pTree, HTREEITEM hTreeItem, LPCSTR szSection, LPCSTR szFile)
//{
//	CString szSubKey("COUNT");
//	int nCount = ::GetPrivateProfileInt(szSection, szSubKey,0, szFile);
//
//	char szLine[50+1]={0,};
//	CItemLongData clItem;
//	HTREEITEM hTree = NULL;
//	for(int i=0; i<nCount; i++)
//	{
//		szSubKey.Format("%02d", i+1);
//		::GetPrivateProfileString(szSection, szSubKey, "", szLine, 50, szFile);
//		if(strlen(szLine)>0)
//		{
//			clItem.Convert(szLine);
//			hTree = pTree->InsertItem(clItem.m_szTitle, hTreeItem);
//			if(clItem.m_szSub=="SUB_NO")
//			{
//				CItemLongData* pNew = new CItemLongData;
//				pNew->SetCopy(&clItem);
//				pTree->SetItemData(hTree, (DWORD)pNew);
//			}
//
//			if(clItem.m_szSub=="SUB_YES")
//			{
//				if(clItem.m_szMethod=="CFG")
//					LoadConfig(pTree, hTree, clItem.m_szSection, szFile);
//				else if(clItem.m_szMethod=="MASTER")
//					LoadMasterFile(pTree, hTree, clItem.m_szInputData);
//			}
//		}
//	}
//
//	return TRUE;
//}

BOOL CTreeListManager::GetFileName(LPCSTR szKey, CString& szFile)
{
	return TRUE;
}

int CompareFileName(const void* p1, const void* p2)
{
    TCHAR *sz1 = ((CString*)(p1))->GetBuffer(0);
    TCHAR *sz2 = ((CString*)(p2))->GetBuffer(0);
    int nPos1 = -1;
    int nPos2 = -1;
    int nEndPos1;
    int nEndPos2;
    int nResult;
    while (1)
    {
        nPos1++;
        nPos2++;
        // Make sure we haven't hit the end of either of the strings
        if (sz1[nPos1] == 0 && sz2[nPos2] == 0)
            return 0;
        if (sz1[nPos1] == 0)
            return -1;
        if (sz2[nPos2] == 0)
            return 1;
        // See if this part of both strings is a number
        if (sz1[nPos1] >= _T('0') && sz1[nPos1] <= _T('9') &&
            sz2[nPos2] >= _T('0') && sz2[nPos2] <= _T('9'))
        {
            // Find the end of each number
            nEndPos1 = nPos1;
            do
            {
                nEndPos1++;
            } while (sz1[nEndPos1] >= _T('0') && sz1[nEndPos1] <= _T('9'));
            nEndPos2 = nPos2;
            do
            {
                nEndPos2++;
            } while (sz2[nEndPos2] >= _T('0') && sz2[nEndPos2] <= _T('9'));
            while (true)
            {
                if (nEndPos1 - nPos1 == nEndPos2 - nPos2)
                {
                    // Both numbers are the same length, just compare them
                    nResult = _tcsnicmp(sz1 + nPos1, sz2 + nPos2, nEndPos1 - nPos1);
                    if (nResult == 0)
                    {
                        nPos1 = nEndPos1 - 1;
                        nPos2 = nEndPos2 - 1;
                        break;
                    }
                    else
                    {
                        return nResult;
                    }
                }
                else if (nEndPos1 - nPos1 > nEndPos2 - nPos2)
                {
                    // First number is longer, so if it's not zero padded, it's bigger
                    if (sz1[nPos1] == _T('0'))
                        nPos1 ++;
                    else
                        return 1;
                }
                else
                {
                    // Second number is longer, so if it's not zero padded, it's bigger
                    if (sz2[nPos2] == _T('0'))
                        nPos2 ++;
                    else
                        return -1;
                }
            }
        }
        else
        {
            // One or both characters is not a number, so just compare them as a string
            nResult = _tcsnicmp(sz1 + nPos1, sz2 + nPos2, 1);
            if (nResult != 0)
            {
                return nResult;
            }
        }
    }
}

void SortFileName(CStringArray& arrStr)
{
    qsort(arrStr.GetData(), arrStr.GetSize(), sizeof(CString*), CompareFileName);
} 

//Test Routine For Solomon by JunokLee 090930
int CTreeListManager::RequestData_GroupMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved, char cSort)
{
	long dwKey1=0;
	
	CStringArray* arCode;
	CStringArray* arName;
	CStringArray* arEngName;
	arCode = new CStringArray;
	arName = new CStringArray;
	arEngName = new CStringArray;
	CString szInputData;

	if(m_pMDMng)
	{
		CString strSection = pILData->m_szSection;
		szInputData = pILData->m_szInputData;
		//if(m_hWinixWnd) m_pMDMng->SetKeyData("WinixWnd", (long)m_hWinixWnd);

		if(!strSection.CompareNoCase("0191"))		// 전종목
		{
			m_pMDMng->GetAllStock(*arCode, *arName);
		}
		else if(!strSection.CompareNoCase("UMASTER"))		// KOSPI 업종
		{
			m_pMDMng->GetKSPUpMst(arCode, arName, arEngName, 1);
		}
		else if(!strSection.CompareNoCase("KUMASTER"))	// KOSDAQ 업종
		{
			m_pMDMng->GetKSQUpMst(arCode, arName, arEngName, 1);
		}
		else if(!strSection.CompareNoCase("KNXUMASTER"))	// KONEX 업종
		{
			m_pMDMng->GetKNXUpMst(arCode, arName, arEngName, 1);
		}
		else if(!strSection.CompareNoCase("KRXUMASTER"))	// KRX 업종
		{
			m_pMDMng->GetKRXUpMst(arCode, arName, arEngName, 1);
		}
		else if(!strSection.CompareNoCase("KSP200UMASTER"))	// KOSPI200 업종
		{
			m_pMDMng->GetK200UpMst(arCode, arName, arEngName);
		}
		else if(!strSection.CompareNoCase("KSP100UMASTER"))	// KOSPI100 업종
		{
			m_pMDMng->GetK100UpMst(arCode, arName, arEngName);
		}
		else if(!strSection.CompareNoCase("FREEUMASTER"))	// 프리보드 업종
		{
			m_pMDMng->GetFREEUpMst(arCode, arName, arEngName, 1);
		}
		else if(!strSection.CompareNoCase("KSTRUMASTER"))	// KOSTAR 업종
		{
			m_pMDMng->GetKSTRUpMst(arCode, arName, arEngName);
		}
		else if(!strSection.CompareNoCase("FMASTER"))		// 선물
		{
			if(!pILData->m_szTR.CompareNoCase("MINI"))
				m_pMDMng->GetMiniFutureJongMst(arCode, arName, arEngName);
			else
				m_pMDMng->GetFutureJongMst(arCode, arName, arEngName);
		}
		else if(!strSection.CompareNoCase("PMASTER"))		// 옵션
		{
			CStringArray* pastrJongEngName;	// 옵션 영문 종목 명
			CStringArray* pastrEventPrice;	// 행사 가격
			CStringArray* pastrFinalYM;		// 만기 년월
			CStringArray* pastrValueGubun;	// 가격 구분
			pastrJongEngName = new CStringArray;
			pastrEventPrice = new CStringArray;
			pastrFinalYM = new CStringArray;
			pastrValueGubun = new CStringArray;

			if(!pILData->m_szTR.CompareNoCase("MINI"))
				m_pMDMng->GetMiniOptionJongMst(arCode, arName);
			else if (!pILData->m_szTR.CompareNoCase("WEEKLY"))
				m_pMDMng->GetWeeklyOptionJongMst(arCode, arName);
			else
				m_pMDMng->GetOptionJongMst(arCode, arName, pastrJongEngName, pastrEventPrice, pastrFinalYM, pastrValueGubun);

			CStringArray arrCode;
			CStringArray arrName;

			if (!pILData->m_szTR.CompareNoCase("WEEKLY"))
			{
				CStringArray arrNameTemp;
				arrNameTemp.Copy(*arName);

				SortFileName(arrNameTemp);


				BOOL bMondayWeelky = FALSE;
				if ((pILData->m_szTitle).Find("M") >= 0)
					bMondayWeelky = TRUE;

				CString strName, strTemp, strMonth, strWeek;
				strMonth = (pILData->m_szTitle).Mid(2, 2);
				strWeek = bMondayWeelky == TRUE ? (pILData->m_szTitle).Mid(6, 2) : (pILData->m_szTitle).Mid(5, 2);

				int nTemp = 2;
				for (int i = 0; i < arrNameTemp.GetSize(); i++)
				{
					for (int j = 0; j < arName->GetSize(); j++)
					{
						strTemp = arrNameTemp.GetAt(i);
						strName = arName->GetAt(j);
						if (arrNameTemp.GetAt(i) == strName && ((bMondayWeelky && strName.Find("M") >= 0) || (!bMondayWeelky && strName.Find("M") < 0))
							&& strName.Find(strMonth) >= 0 && strName.Find(strWeek) >= 0)
						{
							if (bMondayWeelky)
							{
								if (strncmp(strName, "M ", nTemp) == 0)			// 'M'이 추가되어 'M'을 자르는 처리
									strName = strName.Mid(nTemp);
							}

							arrCode.Add(arCode->GetAt(j));
							arrName.Add(strName);
							break;
						}
					}
				}

				arCode->Copy(arrCode);
				arName->Copy(arrName);
			}
			else
			{
				arrName.Copy(*arName);

				SortFileName(arrName);

				for (int i = 0; i < arrName.GetSize(); i++)
				{
					for (int j = 0; j < arName->GetSize(); j++)
					{
						if (arrName.GetAt(i) == arName->GetAt(j))
						{
							arrCode.Add(arCode->GetAt(j));
							break;
						}
					}
				}

				arCode->RemoveAll();
				arName->RemoveAll();

				if (szInputData.GetLength() == 6)
				{
					CString strName;
					for (int i = 0; i < arrName.GetSize(); i++)
					{
						strName = arrName.GetAt(i);
						if (strName.Find(szInputData) >= 0)
						{
							arCode->Add(arrCode.GetAt(i));
							arName->Add(strName);
						}
					}
				}
				else
				{
					arCode->Copy(arrCode);
					arName->Copy(arrName);
				}
			}			

			delete pastrJongEngName;
			delete pastrEventPrice;
			delete pastrFinalYM;
			delete pastrValueGubun;
		}
		else if(!strSection.CompareNoCase("JFMASTER"))	// 주식선물
		{
			m_pMDMng->GetJFMaster(*arCode, *arName);
			if (szInputData.GetLength() == 6)
			{
				CStringArray arrName, arrCode, arrBaseCode;
				arrBaseCode.Copy(*arCode);

				arCode->RemoveAll();
				arName->RemoveAll();

				CString strName;
				for (int i=0; i<arrBaseCode.GetSize(); i++)
				{
					m_pMDMng->GetMasterDataInUpDetails(_T("JFMASTER"), arrCode, arrName, arrBaseCode.GetAt(i));

					for (int j=0; j<arrCode.GetSize(); j++)
					{
						strName = arrName.GetAt(j);
						if (strName.Find(szInputData) >= 0)
						{
							arCode->Add(arrCode.GetAt(j));
							arName->Add(strName);
						}
					}
				}
			}
		}
		else if(!strSection.CompareNoCase("JPMASTER"))	// 주식옵션
		{
			m_pMDMng->GetJPMaster(*arCode, *arName);
		}
		else if(!strSection.CompareNoCase("HISTORY"))		// 히스토리	// 주식만
		{
			int nType = CMasterTypeInfo::STOCK_CHART;
			if (szInputData.CompareNoCase(_T("JFMASTER")) == 0)
				nType = CMasterTypeInfo::STOCKFUTOPT_CHART;

			m_pMDMng->GetRecentCodeNameArray(nType, *arCode, *arName);
		}
		else if(!strSection.CompareNoCase("ETNMASTER"))	// ETN
		{
			m_pMDMng->GetETNMst(arCode, arName, arEngName);
		}
		else if(!strSection.CompareNoCase("VFMASTER"))		// 변동성 선물
		{
			m_pMDMng->GetVFutureMst(arCode, arName, arEngName);
		}
		else if(!strSection.CompareNoCase("SFMASTER"))		// 섹터 선물
		{
			m_pMDMng->GetSFutureMst(arCode, arName, arEngName);
		}
		else
		{
			if (!strSection.CompareNoCase("달러옵션"))		aDataType = 'A';

			m_pMDMng->GetMasterDataInUpDetails(strSection, *arCode, *arName, pILData->m_szInputData);
		}

		//Test Routine For Solomon by JunokLee 090930
		// Log쓰기
// 		{
// 			CString szLogFile;
// 			if(m_pMainInfoMng)
// 			{
// 				szLogFile.Format("%s\\master\\%s.dat", m_pMainInfoMng->GetDataDir(), pILData->m_szSection);
// 			}
// 			else
// 			{
// 				szLogFile.Format("c:\\Logs\\%s.dat", pILData->m_szSection);
// 			}
// 			CFile file;
// 			//if (file.Open(szLogFile, CFile::typeBinary|CFile::modeRead))
// 			if (file.Open(szLogFile, CFile::typeBinary|CFile::modeCreate|CFile::modeWrite))
// 			{
// 				//CArchive ar(&file, CArchive::load);
// 				CArchive ar(&file, CArchive::store);
// 				_DoAlzioSerialize(ar, arCode, arName, arEngName);
// 				ar.Close();
// 				file.Close();
// 			}
// 		}

		if(aDataType=='A')
		{
			LIST_CCodeTypeA ListTypeA; 
			for(int nIndex=0; nIndex<arCode->GetSize(); nIndex++)
			{
				CCodeTypeA codeData;
				codeData.m_szcode = arCode->GetAt(nIndex);
				codeData.m_szhname = arName->GetAt(nIndex);
				ListTypeA.AddTail(codeData);
			}
			pCallback->ReceiveData(0, aDataType, dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
		}

		int ret = arCode->GetSize();
		delete arCode;
		delete arName;
		delete arEngName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTreeConfig_Master(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, CString szSection  ,LPCSTR szTr,LPCSTR szInputData)
{
	long dwKey1=0;
	if(!szSection.CompareNoCase("UMASTER"))			return LoadTree_CUMaster (pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("KUMASTER")){dwKey1=100;	return LoadTree_CKUMaster(pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1); }
	else if(!szSection.CompareNoCase("KRXUMASTER")) return LoadTree_CKRXUMaster(pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("KNXUMASTER")) return LoadTree_CKNXUMaster(pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("TMMASTER"))	return LoadTree_CTHMaster(pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("GMaster"))	return LoadTree_CGMaster (pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("ELWINDEXMASTER"))	return LoadTree_CELWIndexMaster (pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("ELWISSUEMASTER"))	return LoadTree_CELWIssueMaster (pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("UserMaster"))	return LoadTree_CUserMaster (pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("JFMASTER"))	return LoadTree_JFMaster (pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("JPMASTER"))	return LoadTree_JPMaster (pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("PMASTER"))	return LoadTree_PMaster (pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	else if(!szSection.CompareNoCase("WPMASTER"))	return LoadTree_WPMaster(pList, pTree, hParentTree, szTr, szInputData, dwKey1);
	//>> 0027177: [제도개선] 30년 국채선물 상장 관련 수정 요청
	else if(!szSection.CompareNoCase("금리선물") ||
			!szSection.CompareNoCase("통화선물"))
		return LoadTree_KofexListMaster(CDKIND_KOFEXLIST, szSection, pList, pTree, hParentTree, szTr, szInputData, dwKey1);
	//<< 0027177: [제도개선] 30년 국채선물 상장 관련 수정 요청
	else
	{
		int nKind = 0;

		if (!szSection.CompareNoCase("FOFLMASTER"))		nKind = CDKIND_FOINDEX_FO_MONTH;
		else if (!szSection.CompareNoCase("해외_지수"))	nKind = CDKIND_FOREIGN_INDEX;
		else if (!szSection.CompareNoCase("해외_종목"))	nKind = CDKIND_FOREIGN_ITEM;
		else if (!szSection.CompareNoCase("해외_선물"))	nKind = CDKIND_FOREIGN_FUTURE;
		else if (!szSection.CompareNoCase("해외_금리"))	nKind = CDKIND_FOREIGN_RATE;
		else if (!szSection.CompareNoCase("해외_국채"))	nKind = CDKIND_FOREIGN_DEBT;
		else if (!szSection.CompareNoCase("해외_환율"))	nKind = CDKIND_FOREIGN_EXCHANGE;
		else if (!szSection.CompareNoCase("상품선물"))	nKind = CDKIND_KOFEXLIST;
		else if (!szSection.CompareNoCase("달러옵션"))	nKind = CDKIND_KOFEX_OPTION;
		else				return 0;

		return LoadTree_FolderMaster(nKind, szSection, pList ,pTree ,hParentTree ,szTr ,szInputData, dwKey1);
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------
// FunctionName : CTreeListManager::Load_CUMaster
// FunctionName : CTreeListManager::Load_CKUMaster
// ------------------------------------------------------------------------------------------------------

// FN_LoadTree_SubMaster(CUMaster	, "UMASTER"	 ,0);
// FN_LoadTree_SubMaster(CKUMaster , "UKMASTER" ,0);
//FN_LoadTree_SubMaster(CTHMaster , "TMMASTER" ,0);
//FN_LoadTree_SubMaster(CGMaster  , "GMASTER"	 ,0);

int CTreeListManager::LoadTree_CUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1)
{
	if(m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, _szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		//CList<CUMaster, CUMaster>* list = (CList<CUMaster, CUMaster>*)m_pMDMng->GetCodePointer("UMASTER");
		//if(!list) return 0;
		CStringArray* arCode;
		CStringArray* arName;
		CStringArray* arEngName;
		arCode = new CStringArray;
		arName = new CStringArray;
		arEngName = new CStringArray;
		int nIndex = 0;

		m_pMDMng->GetKSPUpMst(arCode, arName, arEngName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
//			sprintf(aKeyB, "B%04.4s", arCode->GetAt(nIndex));// data.m_szCode.Mid(0));
//			if(strcmp(aKeyB, "B1001")==0) continue;

			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

//			szInputData.Format("%s%s", aKeyA, aKeyB);
			//szInputData.Format("%c%s", '1', arCode->GetAt(nIndex));
			szInputData.Format("%s", arCode->GetAt(nIndex));
//-->Edit 유진080331
			//CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "TR", "NONE", szInputData, szTr, dwKey1, 0);
			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "UMASTER", szInputData, "", dwKey1, 0);
//<--Edit 유진080331
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;
		delete arEngName;

		return ret;
		//for(POSITION pos=list->GetHeadPosition(); pos;)
		//{
		//	CUMaster data = list->GetNext(pos);
		//	sprintf(aKeyB, "B%04.4s", data.m_szCode);// data.m_szCode.Mid(0));
		//	if(strcmp(aKeyB, "B1001")==0) continue;

		//	HTREEITEM hTree = pTree->InsertItem(data.m_szName, hParentTree);

		//	szInputData.Format("%s%s", aKeyA, aKeyB);
		//	CItemLongData* pNew = new CItemLongData(data.m_szName, "SUB_NO", "TR", "NONE", szInputData, szTr, dwKey1, 0);
		//	pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
		//	pList->m_pItemList->AddTail(pNew);
		//}
		//return list->GetCount();
	}
	return 0;
}


int CTreeListManager::LoadTree_CKUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1)
{
	if(m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, _szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		CStringArray* arEngName;
		arCode = new CStringArray;
		arName = new CStringArray;
		arEngName = new CStringArray;
		int nIndex = 0;

		m_pMDMng->GetKSQUpMst(arCode, arName, arEngName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
//			sprintf(aKeyB, "B%04.4s", arCode->GetAt(nIndex));// data.m_szCode.Mid(0));
//			if(strcmp(aKeyB, "B2001")==0) continue;
			
			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

//			szInputData.Format("%s%s", aKeyA, aKeyB);
			szInputData.Format("%s", arCode->GetAt(nIndex));
//-->Edit 유진080331
			//CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "TR", "NONE", szInputData, szTr, dwKey1, 0);
			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "KUMASTER", szInputData, szTr, dwKey1, 0);
//<--Edit 유진080331
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;
		delete arEngName;
		return ret;
		//CList<CKUMaster, CKUMaster>* list = (CList<CKUMaster, CKUMaster>*)m_pMDMng->GetCodePointer("UKMASTER");
		//if(!list) return 0;
		//for(POSITION pos=list->GetHeadPosition(); pos;)
		//{
		//	CKUMaster data = list->GetNext(pos);
		//	sprintf(aKeyB, "B%04.4s", data.m_szCode);// data.m_szCode.Mid(0));
		//	if(strcmp(aKeyB, "B2001")==0) continue;
		//	
		//	HTREEITEM hTree = pTree->InsertItem(data.m_szName, hParentTree);

		//	szInputData.Format("%s%s", aKeyA, aKeyB);
		//	CItemLongData* pNew = new CItemLongData(data.m_szName, "SUB_NO", "TR", "NONE", szInputData, szTr, dwKey1, 0);
		//	pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
		//	pList->m_pItemList->AddTail(pNew);
		//}
		//return list->GetCount();
	}
	return 0;
}

int CTreeListManager::LoadTree_CKRXUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1)
{
	if(m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, _szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		CStringArray* arEngName;
		arCode = new CStringArray;
		arName = new CStringArray;
		arEngName = new CStringArray;
		int nIndex = 0;

		m_pMDMng->GetKRXUpMst(arCode, arName, arEngName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

			szInputData.Format("%s", arCode->GetAt(nIndex));
			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "KRXUMASTER", szInputData, "", dwKey1, 0);
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;
		delete arEngName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTree_CKNXUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1)
{
	if(m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, _szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		CStringArray* arEngName;
		arCode = new CStringArray;
		arName = new CStringArray;
		arEngName = new CStringArray;
		int nIndex = 0;

		m_pMDMng->GetKNXUpMst(arCode, arName, arEngName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

			szInputData.Format("%s", arCode->GetAt(nIndex));
			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "KNXUMASTER", szInputData, "", dwKey1, 0);
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;
		delete arEngName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTree_CUserMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1)
{

	CString strList[] = {"거래소종목", "코스닥종목", "KOSPI200", "KOSDAQ50", "KRX100"};
	CString strKeyA[] = {"A1100", "A1200", "A1100", "A1200", "A1100"};
	CString strKeyB[] = {"B0001", "B0201", "B0101", "B0301", "B0401"};

	char aKeyA[5+1]={0,}; memcpy(aKeyA, _szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
	for(int i=0; i<sizeof(strList)/sizeof(LPCTSTR); i++)
	{
		HTREEITEM hTree = pTree->InsertItem(strList[i], hParentTree);

		szInputData.Format("%s%s", strKeyA[i], strKeyB[i]);
//-->Edit 유진080331
		//CItemLongData* pNew = new CItemLongData(strList[i], "SUB_NO", "TR", "NONE", szInputData, szTr, dwKey1, 0);
		CItemLongData* pNew = new CItemLongData(strList[i], "SUB_NO", "MASTER", "NONE", szInputData, szTr, dwKey1, 0);
//<--Edit 유진080331
		pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
		pList->m_pItemList->AddTail(pNew);
	}
	return sizeof(strList)/sizeof(LPCTSTR);
}

int CTreeListManager::LoadTree_CGMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1)
{
	if(m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		arCode = new CStringArray;
		arName = new CStringArray;
		int nIndex = 0;

		m_pMDMng->GetGroupMst(arCode, arName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
//			sprintf(aKeyB, "B%04.4s", arCode->GetAt(nIndex));// data.m_szCode.Mid(0));
//			if(strcmp(aKeyB, "B1001")==0) continue;

			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

//			szInputData.Format("%s%s", aKeyA, aKeyB);
			szInputData.Format("%s", arCode->GetAt(nIndex));

			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "GRPMASTER", szInputData, "", dwKey1, 0);
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTree_CELWIndexMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1)
{
	if(m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		arCode = new CStringArray;
		arName = new CStringArray;
		int nIndex = 0;

		m_pMDMng->GetELWIndexMaster(*arCode, *arName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

			szInputData.Format("%s", arCode->GetAt(nIndex));
			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "ELWINDEXMASTER", szInputData, "", dwKey1, 0);
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTree_CELWIssueMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1)
{
	if(m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		arCode = new CStringArray;
		arName = new CStringArray;
		int nIndex = 0;

		//m_pMDMng->GetMasterData("0306", *arCode, *arName);
		m_pMDMng->GetELWIssueMaster(*arCode, *arName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

			szInputData.Format("%s", arCode->GetAt(nIndex));
			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "ELWISSUEMASTER", szInputData, "", dwKey1, 0);
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTree_JFMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1)
{
	if(m_pMDMng)
	{
		CString strInputData = szInputData;
		CString strInput, szName, szOldName;
		CStringArray* arCode;
		CStringArray* arName;
		arCode = new CStringArray;
		arName = new CStringArray;
		int nIndex = 0;
		int ret = 0;

		m_pMDMng->GetJFMaster(*arCode, *arName);

		if (strInputData.CompareNoCase(_T("MONTH")) == 0)
		{
			if (arCode->GetSize() > 0)
			{
				szName = arCode->GetAt(0);
				m_pMDMng->GetMasterDataInUpDetails(_T("JFMASTER"), *arCode, *arName, szName);

				for (nIndex=0; nIndex<arCode->GetSize(); nIndex++)
				{
					szName = arName->GetAt(nIndex);
					int nFind = szName.Find(_T("F"));
					if (nFind < 0)		// 선물에서만 검사한다.
						continue;

					if (szName.GetLength() <= nFind + 7)
						continue;

					szName = szName.Mid(nFind+2, 6);		// "삼성전자 F 201209 (1)" 글자중 년월(201209)만 뽑아낸다.

					if (szName.Compare(szOldName))
					{
						HTREEITEM hTree = pTree->InsertItem(szName, hParentTree);

						CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "GROUPMASTER", "JFMASTER", szName, "", dwKey1, 0);
						pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
						pList->m_pItemList->AddTail(pNew);
						ret++;
					}

					szOldName = szName;
				}
			}
		}
		else
		{
			for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
			{
				HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

				strInput.Format("%s", arCode->GetAt(nIndex));
				CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "JFMASTER", strInput, "", dwKey1, 0);
				pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
				pList->m_pItemList->AddTail(pNew);
			}
			ret = arCode->GetSize();
		}

		delete arCode;
		delete arName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTree_FolderMaster(int nKind, CString strKind, CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr, LPCSTR szInputData, long dwKey1)
{
	if (m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		CStringArray* arEngName;
		arCode = new CStringArray;
		arName = new CStringArray;
		arEngName = new CStringArray;
		int nIndex = 0;
		
		m_pMDMng->GetForeignMst(nKind, "", arCode, arName, arEngName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

			szInputData.Format("%s", arCode->GetAt(nIndex));
			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "GROUPMASTER", strKind, szInputData, "", dwKey1, 0);
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;
		delete arEngName;
		
		return ret;
	}
	return 0;
}

//>> 0027177: [제도개선] 30년 국채선물 상장 관련 수정 요청
int CTreeListManager::LoadTree_KofexListMaster(int nKind, CString strKind, CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr, LPCSTR szInputData, long dwKey1)
{
	if (m_pMDMng)
	{
		char aKeyA[5 + 1] = { 0, }; memcpy(aKeyA, szInputData, 5); char aKeyB[5 + 1] = { 0, };	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		CStringArray* arEngName;
		arCode = new CStringArray;
		arName = new CStringArray;
		arEngName = new CStringArray;
		int nIndex = 0;

		BOOL bInterestRate = FALSE;
		CString strCmp = "국채", strName;
		if (!strKind.CompareNoCase("금리선물"))
			bInterestRate = TRUE;


		m_pMDMng->GetForeignMst(nKind, "", arCode, arName, arEngName);
		for (nIndex = 0; nIndex < arCode->GetSize(); nIndex++)
		{
			strName = arName->GetAt(nIndex);
			if (strName.Find("금") == 0)
				continue;

			if (strName.Find(strCmp) >= 0 ^ bInterestRate)
			{
				HTREEITEM hTree = pTree->InsertItem(strName, hParentTree);

				szInputData.Format("%s", arCode->GetAt(nIndex));
				CItemLongData* pNew = new CItemLongData(strName, "SUB_NO", "GROUPMASTER", strKind, szInputData, "", dwKey1, 0);
				pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
				pList->m_pItemList->AddTail(pNew);
			}
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;
		delete arEngName;

		return ret;
	}
	return 0;
}
//<< 0027177: [제도개선] 30년 국채선물 상장 관련 수정 요청

int CTreeListManager::LoadTree_PMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1)
{
	if (m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, szInputData, 5); char aKeyB[5+1]={0,};
		CString szInputData, szName, szOldName=_T("");
		CStringArray* arCode;
		CStringArray* arName;
		arCode = new CStringArray;
		arName = new CStringArray;
		int nIndex = 0;

		CStringArray* pastrJongEngName = new CStringArray;
		CStringArray* pastrEventPrice = new CStringArray;
		CStringArray* pastrFinalYM = new CStringArray;
		CStringArray* pastrValueGubun = new CStringArray;

		m_pMDMng->GetOptionJongMst(arCode, arName, pastrJongEngName, pastrEventPrice, pastrFinalYM, pastrValueGubun);

		delete pastrJongEngName;
		delete pastrEventPrice;
		delete pastrFinalYM;
		delete pastrValueGubun;

		CStringArray arrName;
		arrName.Copy(*arName);
		SortFileName(arrName);

		arCode->RemoveAll();
		arName->RemoveAll();

		for (nIndex=0; nIndex<arrName.GetSize(); nIndex++)
		{
			szName = arrName.GetAt(nIndex);
			if (szName.GetAt(0) != 'C')		// 콜옵션에서만 검사한다.
				continue;

			if (szName.GetLength() < 8)
				continue;

			szName = szName.Mid(2, 6);		// "C 201209 220" 글자중 년월(201209)만 뽑아낸다.

			if (szName.Compare(szOldName))
			{
				arName->Add(szName);
				arCode->Add(szName);

				HTREEITEM hTree = pTree->InsertItem(szName, hParentTree);

				CItemLongData* pNew = new CItemLongData(szName, "SUB_NO", "GROUPMASTER", "PMASTER", szName, "", dwKey1, 0);
				pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
				pList->m_pItemList->AddTail(pNew);
			}

			szOldName = szName;
		}

		int ret = arCode->GetSize();
		delete arCode;
		delete arName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTree_JPMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr, LPCSTR szInputData, long dwKey1)
{
	if (m_pMDMng)
	{
		CString strInputData = szInputData;
		CString strInput, szName, szOldName;
		CStringArray* arCode;
		CStringArray* arName;
		arCode = new CStringArray;
		arName = new CStringArray;
		int nIndex = 0;
		int ret = 0;

		m_pMDMng->GetJPMaster(*arCode, *arName);

		for (nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);

			strInput.Format("%s", arCode->GetAt(nIndex));
			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "JPMASTER", strInput, "", dwKey1, 0);
			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		ret = arCode->GetSize();

		delete arCode;
		delete arName;

		return ret;
	}
	return 0;
}

int CTreeListManager::LoadTree_CTHMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData ,long dwKey1)
{
	if(m_pMDMng)
	{
		char aKeyA[5+1]={0,}; memcpy(aKeyA, _szInputData, 5); char aKeyB[5+1]={0,};	CString szInputData;
		CStringArray* arCode;
		CStringArray* arName;
		arCode = new CStringArray;
		arName = new CStringArray;
		int nIndex = 0;
		
		m_pMDMng->GetThemeGroup(*arCode, *arName);
		for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
		{
			HTREEITEM hTree = pTree->InsertItem(arName->GetAt(nIndex), hParentTree);
			
			szInputData.Format("%s", arCode->GetAt(nIndex));

			CItemLongData* pNew = new CItemLongData(arName->GetAt(nIndex), "SUB_NO", "MASTER", "TMMASTER", szInputData, szTr, dwKey1, 0);

			pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
			pList->m_pItemList->AddTail(pNew);
		}
		int ret = arCode->GetSize();
		delete arCode;
		delete arName;
		return ret;
	}
	return 0;
}

int	 CTreeListManager::LoadGwansimGroup(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData)
{
	CStringArray arrKey, arrName;
	theApp.GetGwansimGroupName(m_pWinixGwanInterface, arrKey, arrName);

	// 박성경
	//return 0L;

	CINT_ITEM_ST  OneItem;

	int nSize = arrName.GetSize();
	char szNo[3] = {0, };
	INT_GROUP_ST* pIntGroup = NULL;
	CINT_GROUP_ST itemGwan;

	LPCSTR szGroupName, szGroupKey;
	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	{
		szGroupName = (LPCSTR)arrName.GetAt(nIndx);	//관심그룹이름.
		szGroupKey  = (LPCSTR)arrKey.GetAt(nIndx);
		HTREEITEM hTree = pTree->InsertItem(szGroupName, hParentTree);
		CString szInData;
		//szInData.Format("%s%s%s", szInputData , ":" , szGroupName);
		szInData.Format("%s%s%s", szInputData , ":" , szGroupKey);		
		CItemLongData* pNew = new CItemLongData(szGroupName, "SUB_NO", "USERTR", "GWANGROUP", szInData, szTr, 0, 0);
		pNew->m_pITrComm = (long)pList->m_pTrComm;
		pTree->SetItemData(hTree, (DWORD)pNew);
		pList->m_pItemList->AddTail(pNew);
	}
	return nSize;
}

int	 CTreeListManager::LoadGwansimGroup(CTreeResItemEx* pList, CListCtrl* pListCtrl, LPCSTR szTr ,LPCSTR szInputData)
{
	CStringArray arrKey, arrName;
	theApp.GetGwansimGroupName(m_pWinixGwanInterface, arrKey, arrName);
	// 박성경
	//return 0L;

	CINT_ITEM_ST  OneItem;

	int nSize = arrName.GetSize();
	char szNo[3] = {0, };
	INT_GROUP_ST* pIntGroup = NULL;
	CINT_GROUP_ST itemGwan;

	LPCSTR szGroupName;
	int nLenth=0;
	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	{
		szGroupName = (LPCSTR)arrName.GetAt(nIndx);	//관심그룹이름.
		//HTREEITEM hTree = pTree->InsertItem(szGroupName, hParentTree);
		pListCtrl->InsertItem(nIndx, "");
		pListCtrl->SetItemText(nIndx, 0, szGroupName);
		nLenth = max(nLenth, strlen(szGroupName));
		CString szInData;
		szInData.Format("%s%s%s", szInputData , ":" , szGroupName);
		CItemLongData* pNew = new CItemLongData(szGroupName, "SUB_NO", "USERTR", "GWANGROUP", szInData, szTr, 0, 0);
		pNew->m_pITrComm = (long)pList->m_pTrComm;
		//pTree->SetItemData(hTree, (DWORD)pNew);
		pListCtrl->SetItemData(nIndx, (DWORD)pNew);
		pList->m_pItemList->AddTail(pNew);
	}
	if(nSize>0)
	{
		int nWidth = (nLenth*8)>150 ? nLenth*8 : 150;
		pListCtrl->SetColumnWidth(0, nWidth);
		
	}

	return nSize;
}

void CTreeListManager::RequestData_A_SUBMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, long dwReserved)
{
	//CString szMasterKey = pILData->m_szInputData;
	//CString szUpcode = pILData->m_szSection;
	//LIST_CCodeTypeA ListTypeA; 

	//// 종목마스터에서 업종마스터 일치종목을 가져와야 하는 경우
	//if(!szMasterKey.CompareNoCase("JMASTER_UMASTER")) 
	//{
	//	// 스펙과 안맞음...?????
	//	szUpcode = szUpcode.Mid(1);
	//	CList<CJMaster, CJMaster> list;
	//	m_pMDMng->GetJMaster_Upcode(szUpcode, (long)&list);

	//	for(POSITION pos=list.GetHeadPosition(); pos;)
	//	{
	//		CJMaster data = list.GetNext(pos);
	//		CCodeTypeA typeA;
	//		typeA.m_cGb = '1';
	//		typeA.m_szcode = data.m_szCode;
	//		typeA.m_szhname = data.m_szName;
	//		ListTypeA.AddTail(typeA);
	//	}
	//}
	//// 코스닥마스터에서 업종마스터 일치종목을 가져와야 하는 경우
	//else if(!szMasterKey.CompareNoCase("KMASTER_KUMASTER"))
	//{
	//	szUpcode = szUpcode.Mid(1);
	//	CList<CKMaster, CKMaster> list;
	//	m_pMDMng->GetKMaster_Upcode(szUpcode, (long)&list);

	//	for(POSITION pos=list.GetHeadPosition(); pos;)
	//	{
	//		CKMaster data = list.GetNext(pos);
	//		CCodeTypeA typeA;
	//		typeA.m_cGb = '2';
	//		typeA.m_szcode = data.m_szCode;
	//		typeA.m_szhname = data.m_szName;
	//		ListTypeA.AddTail(typeA);
	//	}
	//}
	//else
	//{
	//	AfxMessageBox("지원하지 않는 데이터(060107_02)");
	//	pCallback->ReceiveData(0, cDataType, dwReserved, 0, NULL);
	//	return;
	//}
	//pCallback->ReceiveData(0, cDataType, dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
}

void CTreeListManager::RequestData_B_SUBMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, long dwReserved)
{
}


#define DR_MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

HRESULT CTreeListManager::CTrCommSite::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
// 	if(dwKey==TRKEY_GWANSIMGROUP)
// 	{
// 		OutputDebugString("관심 OK\n");
// 		return S_OK;
// 	}
// 	else if(dwKey==TRKEY_GWANSIMITEM)
// 	{
// 		OutputDebugString("관심 OK\n");
// 		return S_OK;
// 	}

	DR_MYMETHOD_PROLOGUE(CTreeListManager, TrCommSite)
//	CTreeListManager* pThis = ((CTreeListManager*((BYTE*)this - offsetof(CTreeListManager, m_xTrCommSite)));

	if(strcmp(szTR,TRNO_CONDIONECLK_RESULT)==0)
	{
		// TRACE("CTreeListManager::CTrCommSite::ReceiveData(szTR[%s]\n", szTR);
		return pThis->ReceiveDataOneClick_Change(dwKey, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
	}
	// 2012.5.8 박성경: 보유종목 로드 >>
	else if(dwKey == TRKEY_ACCOUNTINFO)
	{
		pThis->RevData_AccountInfo((char*)aTRData, dwTRDateLen);
		return TRUE;
	}
	// 2012.5.8 박성경: 보유종목 로드 <<
//테마는 순간체결량 참고할 것.
//	else if(strcmp(szTR,"HFIBOA076220")==0)
//	{
//		return pThis->ReceiveDataThemaGroup(dwKey, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
//	}
//	else if(strcmp(szTR,"HFIBOA076210")==0)
//	{
//		return pThis->ReceiveDataThemaItem(dwKey, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
//	}

	if(dwTRDateLen==0)
		return 0;

	CTreeResItemEx* pList = (CTreeResItemEx*)pThis->m_AllList.GetHead();
	ITrComm2* pTrComm = pList->m_pTrComm;
	//Koscom 추가
	CString strTR = (LPCTSTR)szTR;
	CString strOut, strData, strCode, strCount, strType, strOut2="", strBojoMsg="";
	if(strTR == "13600" || strTR == "88803")
	{
		CHARVECTOR2* pData = (CHARVECTOR2*)aTRData;
		int nIdx = 11+ 2*1 + 2*6; // + 6 + 7;


		//메인키
//		strCode = pTrComm->ApplyByteOrder(C_DT_SHORT, (char *)aTRData+nIdx, 6, &nIdx);
		//nIdx += 2;
		//strCode = pTrComm->ApplyByteOrder(C_DT_SHORT, (char *)aTRData+nIdx, 8, &nIdx);
		//nIdx += 8;

		//옵션키
//		strCount = pTrComm->ApplyByteOrder(C_DT_SHORT, (char *)aTRData+nIdx, 4, &nIdx);
		//strType = m_TRComm->ApplyByteOrder(C_DT_LONG, (char *)aTRData+nIdx, 1, &nIdx);
		strCount.Format("%d", dwTRDateLen);
		strOut.Format("%8.8s%04d", pThis->m_strReserved, dwTRDateLen);
		//데이터
		int nLen[] = {0, 6, 20, 10, 1, 10, 6};
		for(int i=0; i<dwTRDateLen; i++)
		{
			strOut += "1";
			for(int j=1; j<(*pData)[i].size(); j++)
			{
				strData.Format("%-*.*s", nLen[j], nLen[j], (*pData)[i][j]);
				strOut += strData;
			}
			//종목코드
			//종목명
			//현재가
			//등락부호
			//대비
			//등락률
		}
	}
	//end

	//TR_88803_O* pOutData = (TR_88803_O*)aTRData;
	TR_88803_O* pOutData = (TR_88803_O*)(LPCTSTR)strOut;
	if(!pOutData) return S_OK;

    char  reserved  [ 8+1]={0,};       /* 결과리턴시 되돌려주는 값 */
	char  count     [ 4+1]={0,};       /* 데이터 카운트            */

	memcpy(reserved	, pOutData->reserved	, 8);
	memcpy(count	, strCount	, 4);

	POSITION pos=NULL;
	sscanf(reserved, "%08X", &pos);
	TRACE("CTreeListManager::CTrCommSite::ReceiveData:pos[%08X]\n", pos);

	if(!pos)	 return S_OK;

//	CPacketInfo info = m_pTLMParent->m_PacketList.GetAt(pos);
	//m_CurPackInfo

//	if(info.m_pTrComm)	pThis->m_pTrCommMng->UnAdviseTrComm(info.m_pTrComm);

	int nCount = atoi(count);

	int nPos = 0;
	if(pThis->m_CurPackInfo.m_aDataType=='A')
	{
		LIST_CCodeTypeA ListTypeA; 
		for(int i=0; i<nCount; i++)
		{
			TR_88803_A* pTrData = (TR_88803_A*)&pOutData->data[nPos];
			nPos += TR_88803_A_SIZE;

			CCodeTypeA codeData;
			codeData.Convert((char*)pTrData);
			ListTypeA.AddTail(codeData);
		}
		pThis->m_CurPackInfo.m_pCallback->ReceiveData(0, pThis->m_CurPackInfo.m_aDataType, pThis->m_CurPackInfo.m_dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
	}
	else if(pThis->m_CurPackInfo.m_aDataType=='B')
	{
		LIST_CCodeTypeB ListTypeB; 
		for(int i=0; i<nCount; i++)
		{
			TR_88803_B* pTrData = (TR_88803_B*)&pOutData->data[nPos];
			nPos += TR_88803_B_SIZE;

			CCodeTypeB codeData;
			codeData.Convert((char*)pTrData);
			ListTypeB.AddTail(codeData);
		}
		pThis->m_CurPackInfo.m_pCallback->ReceiveData(0, pThis->m_CurPackInfo.m_aDataType, pThis->m_CurPackInfo.m_dwReserved, ListTypeB.GetCount(), (void*)&ListTypeB);	
	}
	else if(pThis->m_CurPackInfo.m_aDataType=='C')
	{
		LIST_CCodeTypeC ListTypeC; 
		for(int i=0; i<nCount; i++)
		{
			TR_88803_C* pTrData = (TR_88803_C*)&pOutData->data[nPos];
			nPos += TR_88803_C_SIZE;

			CCodeTypeC codeData;
			codeData.Convert((char*)pTrData);
			ListTypeC.AddTail(codeData);
		}
		pThis->m_CurPackInfo.m_pCallback->ReceiveData(0, pThis->m_CurPackInfo.m_aDataType, pThis->m_CurPackInfo.m_dwReserved, ListTypeC.GetCount(), (void*)&ListTypeC);	
	}
	// 삼성의 테마
	else if(pThis->m_CurPackInfo.m_aDataType=='T')
	{
		TRACE("삼성 Thema Received.\n");
	}

//	m_pTLMParent->m_PacketList.RemoveAt(pos);
	
//	METHOD_PROLOGUE(CTreeListManager, TrCommSite)
//	return pThis->ReceiveData(dwKey, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
	return  S_OK;
}

HRESULT CTreeListManager::CTrCommSite::StatusChange(int nState)
{
//	MYMETHOD_PROLOGUE(CTreeListManager, TrCommSite)
//	return pThis->StatusChange(nState);
	//m_pTLMParent
	return  S_OK;
}

HRESULT CTreeListManager::CTrCommSite::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
{
	return  S_OK;
}

HRESULT CTreeListManager::CTrCommSite::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return 1;
}

// 테마조회
//void CTreeListManager::Send076220Rq()
//{
//}
//*****************************************************************************
// 함  수  명  : int CTabCodeSearch::GetThemaGroup()
// 내      용  : 테마그룹은 처음으로 아웃룩바의 폴더가 열릴 때 조회한다.
//				 플래그 m_bInitThemaGroup
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-13 오후 2:55:07
//*****************************************************************************
//void CTreeListManager::RequestData_Thema_Group(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
//{
//	TRACE("%s\n", "CTreeListManager::RequestData_Thema_Group");
//
//	CPacketInfo newInfo;
//	newInfo.m_aDataType = cDataType;
//	newInfo.m_pCallback = pCallback;
//	newInfo.m_dwReserved = 0;
//	POSITION pos=m_PacketList.AddTail(newInfo);
//	m_ThemaTrIndex["HFIBOA076220"] = pos;
//
//	char aReserved[9]; 
//	sprintf(aReserved, "%08X", pos);
//
//	ITrComm2* pTrComm = (ITrComm2*)pILData->m_pITrComm;
//	if(pTrComm)
//	{
//		BYTE*	lpInputData;
//		lpInputData = (BYTE*)calloc(sizeof(fn_HFIBOA076220_InRec1), sizeof(BYTE));
//		memset(lpInputData, ' ', sizeof(fn_HFIBOA076220_InRec1));
//
//		fn_HFIBOA076220_InRec1	lpInput;
//		memset(&lpInput, ' ', sizeof(fn_HFIBOA076220_InRec1));
//
//		CString strTemp;
//		strTemp = "1";
//		memcpy(lpInput.Sect2,				(LPCSTR)strTemp,					strTemp.GetLength() );
//		strTemp = "W";
//		memcpy( lpInput.WMonthSect,			(LPCSTR)strTemp,					strTemp.GetLength() );
//		
////		memcpy(lpInputData, &lpInput, sizeof(fn_HFIBOA076220_InRec1));
//
////		stSend2Server st;
////		st.m_pWnd = this;
////		st.m_pBuffer = lpInputData;
////		st.m_lLength= sizeof(fn_HFIBOA076220_InRec1);
////		st.m_strTRCode = QUERY_STRTHEME_GROUP;
//		
//		//m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);
//		pTrComm->SetDestination('X');
//		pTrComm->SetTR("HFIBOA076220");
//		pTrComm->ChangeCallback(&m_xTrCommSite);
//		//pTrComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);	
//
//		//화면번호를 세팅한다.
//		CString strScreenNo;
//		GetBaseData("SCREENCODE", strScreenNo);
//		pTrComm->SetBaseData("SCREENCODE", strScreenNo);
//		pTrComm->Send2Server(&lpInput, sizeof(fn_HFIBOA076220_InRec1), FALSE);
//	}
//}


//*****************************************************************************
// 함  수  명  : void CTreeListManager::RequestData_Thema_Item(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
// 내      용  : 테마아이템 조회
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-21 오전 11:09:50
//*****************************************************************************
//void CTreeListManager::RequestData_Thema_Item(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved)
//{
//	TRACE("%s\n", "CTreeListManager::RequestData_Thema_Item");
//
//	CPacketInfo newInfo;
//	newInfo.m_aDataType = cDataType;
//	newInfo.m_pCallback = pCallback;
//	newInfo.m_dwReserved = 0;
//	POSITION pos=m_PacketList.AddTail(newInfo);
//	m_ThemaTrIndex["HFIBOA076210"] = pos;
//
//	char aReserved[9]; 
//	sprintf(aReserved, "%08X", pos);
//
//	ITrComm2* pTrComm = (ITrComm2*)pILData->m_pITrComm;
//	if(pTrComm)
//	{
//		BYTE*	lpInputData;
//		lpInputData = (BYTE*)calloc(sizeof(fn_HFIBOA076210_InRec1), sizeof(BYTE));
//		memset(lpInputData, ' ', sizeof(fn_HFIBOA076210_InRec1));
//
//		fn_HFIBOA076210_InRec1	lpInput;
//		memset(&lpInput, ' ', sizeof(fn_HFIBOA076210_InRec1));
//
//		CString strTemp;
//		strTemp = "30";
//		memcpy(lpInput.InItem1,				(LPCSTR)strTemp,					strTemp.GetLength() );
//		strTemp = pILData->m_szInputData;
//		memcpy( lpInput.ThemaCd,			(LPCSTR)strTemp,					strTemp.GetLength() );
//
//		pTrComm->SetDestination('X');
//		pTrComm->SetTR("HFIBOA076210");
//		pTrComm->ChangeCallback(&m_xTrCommSite);
//
//		//화면번호를 세팅한다.
//		CString strScreenNo;
//		GetBaseData("SCREENCODE", strScreenNo);
//		pTrComm->SetBaseData("SCREENCODE", strScreenNo);
//		pTrComm->Send2Server(&lpInput, sizeof(fn_HFIBOA076210_InRec1), FALSE);
//	}
//}

//HRESULT CTreeListManager::ReceiveDataThemaGroup(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
//{
//	POSITION  pos;
//	m_ThemaTrIndex.Lookup("HFIBOA076220", (void*&)pos);
//	if(!(pos))
//	{
//		return S_FALSE;
//	}
//
//	CPacketInfo info = m_PacketList.GetAt(pos);
//
//	if(dwTRDateLen>0)
//	{
//		fn_HFIBOA076220_OutRec1* pOutRec1 = (fn_HFIBOA076220_OutRec1*)aTRData;
//		//pOutRec1 = (fn_HFIBOA076220_OutRec1 *) rpParser.GetData();
//		CString szTmp;szTmp.Format("%4.4s", pOutRec1->PrntCnt3);
//		int nDataCnt = atoi(szTmp);
//
//		fn_HFIBOA076220_OutRec2* pOutRec2 = (fn_HFIBOA076220_OutRec2*)((char*)aTRData+sizeof(fn_HFIBOA076220_OutRec1));
//		CString	strCode, strName;
//		LIST_CCodeTypeA ListTypeA;
//		for(int i=0; i<nDataCnt; i++)
//		{
//			fn_HFIBOA076220_OutRec2 *pTmpRec = (fn_HFIBOA076220_OutRec2*)(pOutRec2 + i);
//			CCodeTypeA codeItem;
//			codeItem.m_szcode.Format("%5.5s", pTmpRec->ThemaGrpCd);
//			codeItem.m_szhname.Format("%50.50s", pTmpRec->ThemaGrpNm);
//			codeItem.m_szhname.TrimRight();
//
//			ListTypeA.AddTail(codeItem);
//		}
//
//		info.m_pCallback->ReceiveData(0, info.m_aDataType, info.m_dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
//	}
//	else
//	{
//		LIST_CCodeTypeA ListTypeA;
//		info.m_pCallback->ReceiveData(0, info.m_aDataType, info.m_dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
//	}
//
//	m_PacketList.RemoveAt(pos);
//
//	return S_OK;
//}

//HRESULT CTreeListManager::ReceiveDataThemaItem(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
//{
//	POSITION  pos;
//	m_ThemaTrIndex.Lookup("HFIBOA076210", (void*&)pos);
//	if(!(pos))
//	{
//		return S_FALSE;
//	}
//
//	CPacketInfo info = m_PacketList.GetAt(pos);
//	if(dwTRDateLen>0)
//	{
//		fn_HFIBOA076210_OutRec1* pOutRec1 = (fn_HFIBOA076210_OutRec1*)aTRData;
//
//		CString strCnt;
//		strCnt.Format("%04.04s", pOutRec1->PrntCnt3);	/* 출력건수 */
//		int nDataCnt = atoi(strCnt);
//
//		fn_HFIBOA076210_OutRec2* pOutRec2 = (fn_HFIBOA076210_OutRec2 *)((char*)aTRData+sizeof(fn_HFIBOA076210_OutRec1));
//		CString	strCode, strName;
//		LIST_CCodeTypeB ListTypeB; 
//		long UpNDnRt3;
//		for(int i=0; i<nDataCnt; i++)
//		{
//			//fn_HFIBOA076220_OutRec2 *pTmpRec = (fn_HFIBOA076220_OutRec2*)(pOutRec2 + i);
//			fn_HFIBOA076210_OutRec2* pTmpRec = (fn_HFIBOA076210_OutRec2*)(pOutRec2 + i);
//
//			CCodeTypeB codeItem;
//			codeItem.m_szcode.Format("%6.6s", pTmpRec->ItemCd13);
//			codeItem.m_szhname.Format("%20.20s", pTmpRec->ItemNm1);
//			codeItem.m_szhname.TrimRight();
//
//			//codeData.m_sztime;									//시간(HH:MM:SS)
//			codeItem.m_szsign = pTmpRec->Symbl[0];					//등락부호
//
//			g_ConvertBigEndian(pTmpRec->UpNDn1);
//			codeItem.m_szchange.Format("%d", pTmpRec->UpNDn1);		//등락폭
//
//			g_ConvertBigEndian(pTmpRec->CurPrc);
//			codeItem.m_szprice.Format("%d", pTmpRec->CurPrc);		//현재가
//
//			g_ConvertBigEndian(pTmpRec->UpNDnRt3);
//			UpNDnRt3 = pTmpRec->UpNDnRt3;
//			if(UpNDnRt3>0)
//				codeItem.m_szchrate.Format("%d.%02d", pTmpRec->UpNDnRt3/100, pTmpRec->UpNDnRt3%100);	//등락률
//			else
//				codeItem.m_szchrate.Format("-%d.%02d", abs(UpNDnRt3/100), abs(UpNDnRt3%100));	//등락률
//
//			g_ConvertBigEndian(pTmpRec->TrdVol1);
//			codeItem.m_szvolume.Format("%d", pTmpRec->TrdVol1);	//누적거래량
//
//			ListTypeB.AddTail(codeItem);
//		}
//
//		//info.m_pCallback->ReceiveData(0, info.m_aDataType, info.m_dwReserved, ListTypeB.GetCount(), (void*)&ListTypeB);
//		info.m_pCallback->ReceiveData(0, 'B', info.m_dwReserved, ListTypeB.GetCount(), (void*)&ListTypeB);
//	}
//	else
//	{
//		LIST_CCodeTypeB ListTypeB; 
//		info.m_pCallback->ReceiveData(0, 'B', info.m_dwReserved, ListTypeB.GetCount(), (void*)&ListTypeB);
//	}
//
//	m_PacketList.RemoveAt(pos);
//
//	return S_OK;
//}

/*
if(strTRCode==QUERY_STRTHEME_GROUP || strTRCode==QUERY_STRMARKET)
	{
		TRACE("%s\n", strTRCode);

		fn_HFIBOA076220_OutRec1* pOutRec1 = (fn_HFIBOA076220_OutRec1*)aTRData;
		//pOutRec1 = (fn_HFIBOA076220_OutRec1 *) rpParser.GetData();
		CString szTmp;szTmp.Format("%4.4s", pOutRec1->PrntCnt3);
		int nDataCnt = atoi(szTmp);

		fn_HFIBOA076220_OutRec2* pOutRec2 = (fn_HFIBOA076220_OutRec2*)((char*)aTRData+sizeof(fn_HFIBOA076220_OutRec1));
		CString	strCode, strName;
		for(int i=0; i<nDataCnt; i++)
		{
//			fn_HFIBOA076220_OutRec2 *pTmpRec = (fn_HFIBOA076220_OutRec2*)(pOutRec2 + i);
//			strCode.Format("%5.5s", pTmpRec->ThemaGrpCd);
//			strName.Format("%50.50s", pTmpRec->ThemaGrpNm);
//			strName.TrimRight();
//			TRACE("%s[%s]\n", strCode, strName);
		}
		pWndRequested = NULL;
	}

*/
// 테마종목조회
//void CTreeListManager::Send076210Rq(CString strKey)
//{
//}

//{{ 원클릭관련 작업 시작.
#define		SZONECLICK			"빠른종목검색"
#define		SZCONDI				"종목검색"
#define		SZUSER				"사용자조건검색"
void CTreeListManager::LoadConditonMng(CTreeResItemEx* pList, CTreeCtrl* pTreeCtrl, HTREEITEM hTreeParent)
{
	HTREEITEM hTreeItem = CHelperTreeCtrl::FindTreeItem(pTreeCtrl, SZONECLICK);
//	HTREEITEM hTreeItemUser = CHelperTreeCtrl::FindTreeItem(pTreeCtrl, SZUSER);
	if(!hTreeItem && !hTreeParent)
		return;
	if(!m_bLoadCondMng)
	{
		m_LibCondNng.LoadLib();
		m_bLoadCondMng = TRUE;
	}
	
	if(m_LibCondNng.m_hLib)
	{
		if(hTreeParent) m_LibCondNng.Init_New(pList, pTreeCtrl, hTreeParent, 2);
		else if(hTreeItem) m_LibCondNng.Init_New(pList, pTreeCtrl, hTreeItem, 2);
	}
}

void CTreeListManager::LoadConditonMng2(CTreeResItemEx* pList, CTreeCtrl* pTreeCtrl, HTREEITEM hTreeParent)
{
	HTREEITEM hTreeItem = CHelperTreeCtrl::FindTreeItem(pTreeCtrl, SZCONDI);
	//	HTREEITEM hTreeItemUser = CHelperTreeCtrl::FindTreeItem(pTreeCtrl, SZUSER);
	if(!hTreeItem && !hTreeParent)
		return;
	if(!m_bLoadCondMng)
	{
		m_LibCondNng.LoadLib();
		m_bLoadCondMng = TRUE;
	}
	
	if(m_LibCondNng.m_hLib)
	{
		if(hTreeItem) m_LibCondNng.Init_New(pList, pTreeCtrl, hTreeItem, 2);
		else if(hTreeParent) m_LibCondNng.Init_New(pList, pTreeCtrl, hTreeParent, 2);
	}
}

void CTreeListManager::LoadConditonMngUser(CTreeResItemEx* pList, CTreeCtrl* pTreeCtrl, HTREEITEM hTreeParent)
{
	HTREEITEM hTreeItem = CHelperTreeCtrl::FindTreeItem(pTreeCtrl, SZUSER);
//	HTREEITEM hTreeItemUser = CHelperTreeCtrl::FindTreeItem(pTreeCtrl, SZUSER);
	if(!hTreeItem && !hTreeParent)
		return;
	if(!m_bLoadCondMng)
	{
		m_LibCondNng.LoadLib();
		m_bLoadCondMng = TRUE;
	}
	
	if(m_LibCondNng.m_hLib)
	{
		if(hTreeParent) m_LibCondNng.Init_New(pList, pTreeCtrl, hTreeParent, 1);
		else if(hTreeItem) m_LibCondNng.Init_New(pList, pTreeCtrl, hTreeItem, 1);
	}
}

HRESULT CTreeListManager::ReceiveDataOneClick_Change(long dwKey, LPCSTR szTrNo, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	if(dwTRDateLen==0)
	{
		//if(strcmp(TRNO_CONDIONECLK_RESULT,szTrNo)==0) pThis->m_b0622Ing = FALSE;
		CString szTmp = szMessage;
		szTmp.TrimLeft();
		szTmp.TrimRight();
		return S_OK;
	}

	// dwTRDateLen
	STDR_FID *pstDrData = (STDR_FID*)aTRData;

	BYTE *pUseData=NULL;
	int nUseDataLen=0;

	if(strcmp(TRNO_CONDIONECLK_RESULT,szTrNo)==0)	//"조건 결과 조회 요청";
	{
//		pThis->m_b0622Ing = FALSE;

		//AfxMessageBox("TRNO_CONDIONECLK_RESULT");
		KB_d1022_OutRec1* pOutRec = (KB_d1022_OutRec1*)aTRData;
		KB_d1022_OutRec2* pOutRec2 = (KB_d1022_OutRec2*)((char*)aTRData + sizeof(KB_d1022_OutRec1));

		char* pDataBuf = NULL;
		int nTotalSize=0;
		CCondHelper TrHelper;
		int nFindDataCnt = TrHelper.D1022OutRec1_GetDataCount(pOutRec);

		nTotalSize = sizeof(TR_RESULT_HEADER) + (sizeof(TR_RESULT_LIST) * nFindDataCnt);
		pDataBuf = new char[nTotalSize+1];
		pDataBuf[nTotalSize]=0;

		int nPos = 0;
		// inRec1
		TR_RESULT_HEADER* pOutData = (TR_RESULT_HEADER*)&pDataBuf[nPos];
		TrHelper.D1022OutRec1_SetData(pOutRec, pOutData);
		nPos += sizeof(TR_RESULT_HEADER);

		// inRec2
		//TR_RESULT_LIST** pOutList = &pDataBuf[sizeof(TR_RESULT_HEADER)];
		if(nFindDataCnt>0)
		{
			TrHelper.D1022OutRec2_SetData(pOutRec2,
					&pDataBuf[nPos], nFindDataCnt);
		}
		else
		{
		}
		ReceiveDataOneClick(dwKey, szTrNo, szMsgCode, szMessage, (BYTE*)pDataBuf, nTotalSize);
		delete [] (pDataBuf);
	}

	return S_OK;
}

HRESULT CTreeListManager::ReceiveDataOneClick(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	HRESULT hr=S_OK;
	if(dwTRDateLen>0)
	{
		//char *pData = new char[dwTRDateLen + 1];
		//::memset(pData, 0x00, sizeof(pData));
		//::memcpy(pData, aTRData, dwTRDateLen);
		//hr = SetReceiveData_Result(pData, dwTRDateLen);
		//delete [] pData;
		char *pData = new char[dwTRDateLen + 1];
		::memset(pData, 0x00, sizeof(pData));
		::memcpy(pData, aTRData, dwTRDateLen);
		if(dwTRDateLen)
			hr = SetReceiveData_Result(pData, dwTRDateLen);
		else
			SetReceiveData_NULL();
		delete [] pData;
	}
	else
	{
		SetReceiveData_NULL();
	}

	m_LibCondNng.m_nCurTrIndex = -1;

	return hr;
}

HRESULT CTreeListManager::SetReceiveData_NULL()
{
	if(m_LibCondNng.m_nCurTrIndex<0) return S_FALSE;

	POSITION pos = m_LibCondNng.m_TrPos[m_LibCondNng.m_nCurTrIndex];

	if(!pos)
	{
		return S_FALSE;
	}
	//CPacketInfo info = m_PacketList.GetAt(pos);
	//m_CurPackInfo

	LIST_CCodeTypeA ListTypeA; 
	LIST_CCodeTypeB ListTypeB; 
	LIST_CCodeTypeC ListTypeC; 

	switch(m_CurPackInfo.m_aDataType)
	{
	case 'A':
		m_CurPackInfo.m_pCallback->ReceiveData(0, m_CurPackInfo.m_aDataType, m_CurPackInfo.m_dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
		break;
	case 'B':
		m_CurPackInfo.m_pCallback->ReceiveData(0, m_CurPackInfo.m_aDataType, m_CurPackInfo.m_dwReserved, ListTypeB.GetCount(), (void*)&ListTypeB);
		break;
	case 'C':
		m_CurPackInfo.m_pCallback->ReceiveData(0, m_CurPackInfo.m_aDataType, m_CurPackInfo.m_dwReserved, ListTypeC.GetCount(), (void*)&ListTypeC);
		break;
	}
//	m_PacketList.RemoveAt(pos);

	return S_OK;
}

HRESULT CTreeListManager::SetReceiveData_Result(LPTSTR pData, DWORD dwDataLen)
{
	TR_RESULT_HEADER		stTR_RESULT_HEADER;
	memset(&stTR_RESULT_HEADER, 0x00, SZ_TR_RESULT_HEADER);
	memcpy(&stTR_RESULT_HEADER, pData, SZ_TR_RESULT_HEADER);

	// stTR_RESULT_HEADER.nTrIndex = stTR_RESULT_HEADER.nTrIndex;// ntohl(stTR_RESULT_HEADER.nTrIndex);
	//stTR_RESULT_HEADER.nTrIndex = ntohl(stTR_RESULT_HEADER.nTrIndex);
	// 인덱스를 가지고 콜백함수정보등을 찾는다.
	//if (stTR_RESULT_HEADER.nTrIndex != m_nTrIndex_Result)
	//	return S_FAIL;
	POSITION pos = m_LibCondNng.m_TrPos[stTR_RESULT_HEADER.giCallIndex];
	if(!pos)
	{
		return S_FALSE;
	}

	//CPacketInfo info = m_PacketList.GetAt(pos);

	LIST_CCodeTypeA ListTypeA; 
	LIST_CCodeTypeB ListTypeB; 
	LIST_CCodeTypeC ListTypeC; 

	if(dwDataLen>0)
	{
		int nTotCnt;
		char *pBuff;
		pBuff = new char[sizeof(stTR_RESULT_HEADER.sJongCnt) + 1];
		memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sJongCnt) + 1);
		memcpy(pBuff, &stTR_RESULT_HEADER.sJongCnt, sizeof(stTR_RESULT_HEADER.sJongCnt));
		nTotCnt = atoi(pBuff);
		delete[] pBuff;

		pData = &pData[SZ_TR_RESULT_HEADER];
		dwDataLen = dwDataLen - SZ_TR_RESULT_HEADER;


		TR_RESULT_LIST		stTR_RESULT_LIST;
		int nsizeName = sizeof(stTR_RESULT_LIST.sName);
		//		pBuff = new char[nsizeName + 1];
		for (int nIndex = 0; nIndex < nTotCnt; nIndex++)
		{
			memset(&stTR_RESULT_LIST, 0x00, SZ_TR_RESULT_LIST);
			memcpy(&stTR_RESULT_LIST, pData, SZ_TR_RESULT_LIST);

			//			CString strData;
			//			memset(pBuff, 0x00, nsizeName + 1);
			//			memcpy(pBuff, &stTR_RESULT_LIST.sName, nsizeName);
			//			strData = (CString)pBuff;

			switch(m_CurPackInfo.m_aDataType)
			{
				// A/B만 지원함.
			case 'A':
				{
					CCodeTypeA codeItem;
					codeItem.ConvertOneClickData(stTR_RESULT_LIST.sCode);
					ListTypeA.AddTail(codeItem);
				}
				break;
			case 'B':
				{
					CCodeTypeB codeItem;
					codeItem.ConvertOneClickData(stTR_RESULT_LIST.sCode);
					ListTypeB.AddTail(codeItem);
				}
				break;
			case 'C':
				{
					//					CCodeTypeC codeItem;
					//					codeItem.ConvertOneClickData(stTR_RESULT_LIST.sCode);
					//					ListTypeC.AddTail(codeItem);
				}
				break;
			}

			//		strData = strData.Mid(0, 2);
			pData = &pData[SZ_TR_RESULT_LIST];
			dwDataLen = dwDataLen - SZ_TR_RESULT_LIST;
			if(dwDataLen<=0) break;
		}
		//		delete[] pBuff;

	}

	switch(m_CurPackInfo.m_aDataType)
	{
	case 'A':
		m_CurPackInfo.m_pCallback->ReceiveData(0, m_CurPackInfo.m_aDataType, m_CurPackInfo.m_dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
		break;
	case 'B':
		m_CurPackInfo.m_pCallback->ReceiveData(0, m_CurPackInfo.m_aDataType, m_CurPackInfo.m_dwReserved, ListTypeB.GetCount(), (void*)&ListTypeB);
		break;
	case 'C':
		m_CurPackInfo.m_pCallback->ReceiveData(0, m_CurPackInfo.m_aDataType, m_CurPackInfo.m_dwReserved, ListTypeC.GetCount(), (void*)&ListTypeC);
		break;
	}

	//m_PacketList.RemoveAt(pos);

	return S_OK;
}


void CTreeListManager::DeleteTreeChildInfo(CTreeCtrl* pTree, HTREEITEM hParentTree)
{
	// Delete all of the children of hmyItem.
	if (pTree->ItemHasChildren(hParentTree))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = pTree->GetChildItem(hParentTree);
		CItemLongData* pData=NULL;
		while (hChildItem != NULL)
		{
			hNextItem = pTree->GetNextItem(hChildItem, TVGN_NEXT);
			pData = (CItemLongData*)pTree->GetItemData(hChildItem);

			pTree->DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}

//}} 원클릭관련 작업 끝.

//@For MasterData. By JunokLee 09.09.30Wed
void CTreeListManager::_DoAlzioSerialize(CArchive& ar,
										 CStringArray* parCode, CStringArray* parName, CStringArray* parEngName)
{
	int nVer = 1;
	if (ar.IsStoring())
	{	// storing code
		ar << nVer;
		parCode->Serialize(ar);
		parName->Serialize(ar);
		parEngName->Serialize(ar);
	}
	else
	{	// loading code
		ar >> nVer;
		parCode->Serialize(ar);
		parName->Serialize(ar);
		parEngName->Serialize(ar);
	}
}

void CTreeListManager::fnWinixCommCallbackProc(int nKey, long pThisPoint, long dwUserKey, int nLen, LPBYTE pData)
{
	CTreeListManager* pThis = (CTreeListManager*)pThisPoint;
}

//tour2k 관심종목 조회후 account 조회
int CTreeListManager::LoadAccountAfter( CTreeCtrl* pTree, HTREEITEM hTree )
{
	if (m_clItem.m_szTitle.GetLength() > 0 )
	{
		HTREEITEM hSubTree = pTree->InsertItem(m_clItem.m_szTitle, hTree);
		if (m_clItem.m_szSub == "SUB_YES") LoadAccountGroup(NULL, pTree, hSubTree, "NONE", "NONE");
	}

	return 0;
}
// 2012.5.8 박성경: 보유종목 로드 >>
int	 CTreeListManager::LoadAccountGroup(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData)
{
	CStringArray arrKey, arrName;
	CString strAccInfo, strData;
	
	strAccInfo = (char*)::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(variantDLL, accountCC), (LPARAM)(LPCSTR)strData);
	m_accn.load_acc(strAccInfo);
	
	CStringArray* parrCode = m_accn.get_account();
	
	CString strAccn, strText, strT, strScreenNo;
	GetBaseData("SCREENCODE", strScreenNo);

	for(int i=0 ; i<parrCode->GetSize() ; i++)
	{
		strAccn = parrCode->GetAt(i);
		
		strText = strAccn.Left(3) + "-" + strAccn.Mid(3,2) + "-" + strAccn.Right(6);
		strT = strAccn.Mid(3,1);
		
		if( strT != "5" && strT != "0" )
		{
			if(strScreenNo == "7112" && strT == "2")
				continue;
			else if(strScreenNo == "7114" && strT != "2")
				continue;
			else if(strScreenNo == "7138" && strT != "2")
				continue;

			arrKey.Add(strAccn);
			arrName.Add(strText);
		}
	}
	
	CINT_ITEM_ST  OneItem;
	
	int nSize = arrName.GetSize();
	char szNo[3] = {0, };
	INT_GROUP_ST* pIntGroup = NULL;
	CINT_GROUP_ST itemGwan;
	
	LPCSTR szGroupName, szGroupKey;
	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	{
		szGroupName = (LPCSTR)arrName.GetAt(nIndx);
		szGroupKey  = (LPCSTR)arrKey.GetAt(nIndx);
		HTREEITEM hTree = pTree->InsertItem(szGroupName, hParentTree);
		CString szInData;
		//szInData.Format("%s%s%s", szInputData , ":" , szGroupName);
		szInData.Format("%s%s%s", szInputData , ":" , szGroupKey);		
		CItemLongData* pNew = new CItemLongData(szGroupName, "SUB_NO", "USERTR", "ACCOUNT", szInData, szTr, 0, 0);
		if (pList) pNew->m_pITrComm = (long)pList->m_pTrComm;
		pTree->SetItemData(hTree, (DWORD)pNew);
		if (pList) pList->m_pItemList->AddTail(pNew);
	}

	return nSize;
}

BOOL IsNumber(CString str)
{
    BOOL bRet = TRUE;
    int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = FALSE;
            break;
        }
    }
    return bRet;
}

void CTreeListManager::SendTr_AccountInfo(CString strGroupKey)
{
	if(strGroupKey.GetLength()==0) return;
	
	CString pswd, acctdata, strMember, strData;
	
	acctdata = (char*)::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(variantDLL, accountCC), (LPARAM)(LPCSTR)strData);
	strMember = (char*)::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(variantDLL, userCC), (LPARAM)(LPCSTR)strData);
	
	//직원 고객 구분
	if(IsNumber(strMember))
	{
		pswd = m_accn.loadPass(acctdata,strGroupKey);
		
		if(pswd == "")
		{
			MessageBox(m_hWinixWnd,"계좌비밀번호 저장 후 사용하십시오.","IBK투자증권",MB_OKCANCEL | MB_ICONQUESTION);
			return;
		}
	}
	else
	{
		pswd = "9999";
	}
	
	if(strGroupKey.Mid(3, 1) == "2")	// 선물옵션
	{
		struct _ledgerH ledger;
		
		FillMemory(&ledger, L_ledgerH, ' ');
		
		::SendMessage(m_hWinixWnd, WM_USER, ledgerDLL, (LPARAM)&ledger);
		
		CopyMemory(&ledger.svcd, "SONBQ021", sizeof(ledger.svcd));
		CopyMemory(&ledger.usid, (LPCTSTR)strMember, strMember.GetLength());
		
		CString strBrno = (char*)::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(variantDLL, deptCC), 0L);
		CopyMemory(&ledger.brno, strBrno.Left(3), 3);
		CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
		
		ledger.fkey[0] = 'C';
		ledger.mkty[0] = '2';
		ledger.odrf[0] = '1';
		
		CString strLedger = CString((char*)&ledger, L_ledgerH);
		
		struct f_mid mid;
		
		FillMemory(&mid, L_fmid, ' ');
		CopyMemory(&mid.accn, strGroupKey, sizeof(mid.accn));
		CopyMemory(&mid.pswd, pswd, pswd.GetLength());
		CopyMemory(&mid.date, "", sizeof(mid.date));
		
		
		CString strSendData(_T(""));
		CString strMid = CString((char*)&mid, L_fmid);
		strSendData = strLedger + strMid;
		
		CString strTRCode;
		strTRCode.Format("%s", TRNO_FO_ACCOUNTINFO);
		
		char* szSendData = (LPSTR)(LPCTSTR)strSendData;
		
		int nDataLen = strSendData.GetLength();
		int nKey = TRKEY_ACCOUNTINFO;
		char cStat = US_KEY;
		
		
		char* szSendBuf = new char[L_userTH + nDataLen + 1];
		memset(szSendBuf, NULL, sizeof(szSendBuf));
		
		memcpy(szSendBuf, szSendData, nDataLen);
		
		CString strTRData = "";
		strTRData.Format("%s\t%08X\t%d\t%d\t%s"	
			, strTRCode
			, &m_xTrCommSite
			, TRKEY_ACCOUNTINFO
			, nDataLen			//Size
			, szSendBuf);		//Data
		
		if (m_hWrapper)
			::SendMessage(m_hWrapper, RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
		
		delete szSendBuf;
	}
	else	// 현물
	{
		struct _ledgerH ledger;
		
		FillMemory(&ledger, L_ledgerH, ' ');
		
		::SendMessage(m_hWinixWnd, WM_USER, ledgerDLL, (LPARAM)&ledger);
		
		CopyMemory(&ledger.svcd, "SONAQ052", sizeof(ledger.svcd));
		CopyMemory(&ledger.usid, (LPCTSTR)strMember, strMember.GetLength());
		
		CString strBrno = (char*)::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(variantDLL, deptCC), 0L);
		CopyMemory(&ledger.brno, strBrno.Left(3), 3);
		CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
		
		ledger.fkey[0] = 'C';
		ledger.mkty[0] = '1';
		ledger.odrf[0] = '1';
		
		CString strLedger = CString((char*)&ledger, L_ledgerH);
		
		struct s_mid mid;
		
		FillMemory(&mid, L_smid, ' ');
		CopyMemory(&mid.accn, strGroupKey, sizeof(mid.accn));
		CopyMemory(&mid.pswd, pswd, pswd.GetLength());
		mid.allf[0] = '1';
		
		CString strSendData(_T(""));
		CString strMid = CString((char*)&mid, L_smid);
		strSendData = strLedger + strMid;
		
		CString strTRCode;
		strTRCode.Format("%s", TRNO_ACCOUNTINFO);
		
		char* szSendData = (LPSTR)(LPCTSTR)strSendData;
		
		int nDataLen = strSendData.GetLength();
		int nKey = TRKEY_ACCOUNTINFO;
		char cStat = US_KEY;
		
		
		char* szSendBuf = new char[L_userTH + nDataLen + 1];
		memset(szSendBuf, NULL, sizeof(szSendBuf));
		
		memcpy(szSendBuf, szSendData, nDataLen);
		
		CString strTRData = "";
		strTRData.Format("%s\t%08X\t%d\t%d\t%s"	
			, strTRCode
			, &m_xTrCommSite
			, TRKEY_ACCOUNTINFO
			, nDataLen			//Size
			, szSendBuf);		//Data
		
		if (m_hWrapper)
			::SendMessage(m_hWrapper, RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
		
		delete szSendBuf;
	}
	
	return;
}

void CTreeListManager::GetAccountGroupCode(char* buf)
{
	struct s_mod	mod;	
	
	int sizeJango = 0;
	int nRec = 0;
	int sBuf = strlen(buf);
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod.nrec, sizeof(mod.nrec)));
	
	LIST_CCodeTypeA ListTypeA;
	CString strType;
	for (int i = 0 ; i<nRec ; i++)
	{
		struct s_rec rec = mod.rec[i];
		CString strCode(rec.cod2, sizeof(rec.cod2));
		CString strName(rec.hnam, sizeof(rec.hnam));
		strCode.TrimRight();
		strName.TrimRight();
		
		if (!strName.IsEmpty())
		{
			if (strCode.GetAt(0) == _T('A') || strCode.GetAt(0) == _T('J'))
			{
				if(strlen(strCode) == 7)
					strCode.Delete(0);
			}
			strType = m_pMDMng->GetCodeInfo("", 2, strCode);
 			
			CCodeTypeA codeItem;
			memcpy(&codeItem.m_cGb, strType, 1);
			codeItem.m_szcode = strCode;
			codeItem.m_szhname = strName;
 			ListTypeA.AddTail(codeItem);
		}
	}
	m_CurPackInfo.m_pCallback->ReceiveData(0, m_CurPackInfo.m_aDataType, m_CurPackInfo.m_dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
}

void CTreeListManager::GetFOAccountGroupCode(char* buf)
{
	struct f_mod mod;	
	
	int sizeJango = 0;
	int nRec = 0;
	int sBuf = strlen(buf);
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod.nrec, sizeof(mod.nrec)));
	
	LIST_CCodeTypeA ListTypeA;
	CString strType;
	for (int i = 0 ; i<nRec ; i++)
	{
		struct f_rec rec = mod.rec[i];
		CString strCode(rec.cod2, sizeof(rec.cod2));
		CString strName(rec.hnam, sizeof(rec.hnam));
		strCode.TrimRight();
		strName.TrimRight();
		
		if (!strName.IsEmpty())
		{
			strType = m_pMDMng->GetCodeInfo("", 2, strCode);
			
			CCodeTypeA codeItem;
			memcpy(&codeItem.m_cGb, strType, 1);
			codeItem.m_szcode = strCode;
			codeItem.m_szhname = strName;
 			ListTypeA.AddTail(codeItem);
		}
	}
	m_CurPackInfo.m_pCallback->ReceiveData(0, m_CurPackInfo.m_aDataType, m_CurPackInfo.m_dwReserved, ListTypeA.GetCount(), (void*)&ListTypeA);
}

void CTreeListManager::RevData_AccountInfo(char* buf, long len)
{
	struct _ledgerH ledger;
	CopyMemory(&ledger, (void*)buf, L_ledgerH);
	CString strLedger = CString((char*)&ledger, L_ledgerH);
	CString strErrCode = CString((char*)&ledger.emsg, 4);
	CString strErrText = CString((char*)&ledger.emsg, 98);
	CString strServiceName = CString((char*)&ledger.svcd, 8);
	
	CString strMsg = "ERR\t" + strErrText;
	
	if (strErrCode.GetAt(0) != _T('0'))	// 오류
	{
		strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));
		AfxMessageBox(strMsg);
	}
	else
	{
		buf += L_ledgerH;
		
		if(strServiceName == "SONBQ021")		//선물잔고
		{
			GetFOAccountGroupCode(buf);	
		}
		else			//현물잔고
		{
			GetAccountGroupCode(buf);
		}
	}
}
// 2012.5.8 박성경: 보유종목 로드 <<

int CTreeListManager::LoadTree_WPMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr, LPCSTR szInputData, long dwKey1)
{
	if (m_pMDMng)
	{
		CString strInputData = szInputData;
		CString strInput, strName, strCode;
		CStringArray arWeekly;
		CStringArray* arCode;
		CStringArray* arName;
		arCode = new CStringArray;
		arName = new CStringArray;
		int nIndex = 0;
		int ret = 0;

		m_pMDMng->GetWPMaster(*arCode, *arName);

		for (nIndex = 0; nIndex < arCode->GetSize(); nIndex++)
		{
			strName = arName->GetAt(nIndex);
			strCode = arCode->GetAt(nIndex);
	
			/*C 2306W3 302.5, M C 2307W4 182.5,	209BH302*/
			if (strName.Find("M") == 0)
				strName.Format("%s.M%s", strName.Mid(4, 4), strName.Mid(8, 2));
			else
				strName.Format("%s.%s", strName.Mid(2, 4), strName.Mid(6, 2));

			BOOL bFind = FALSE;
			for (int i = 0; i < arWeekly.GetCount(); i++)
			{
				if (arWeekly[i].Compare(strName) == 0)
				{
					bFind = TRUE;
					break;
				}
			}

			if (bFind == FALSE)
			{
				HTREEITEM hTree = pTree->InsertItem(strName, hParentTree);

				strInput.Format("%s", arCode->GetAt(nIndex));
				CItemLongData* pNew = new CItemLongData(strName, "SUB_NO", "GROUPMASTER", "PMASTER", strInput, "WEEKLY", dwKey1, 0);
				pNew->m_pITrComm = (long)pList->m_pTrComm;	pTree->SetItemData(hTree, (DWORD)pNew);
				pList->m_pItemList->AddTail(pNew);
				arWeekly.Add(strName);
			}
		}
		ret = arCode->GetSize();

		delete arCode;
		delete arName;

		return ret;
	}
	return 0;
}