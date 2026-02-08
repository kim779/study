#ifndef __LOAD_FOR_CONDITIONMANAGER__BY_JUNOK_LEE__HEADER__
#define __LOAD_FOR_CONDITIONMANAGER__BY_JUNOK_LEE__HEADER__

// #include "LoadConditionMng.h"

//#include "../../inc/Condition_MngDef.h"
#include "../../inc/ITreeListMng.h"
#include "../../inc/IDataUtilMng.h"
#include "../../inc/IAUTrCommMng.h"

//#include "../../../TRDll/Dr_Include/ConditionSearchOne_Convert.h"
//#include "../../../TRDll/ConditionSearch_One/DefineFiles/EUTranHelper_D1022.h"

#include "./DefineFiles/Condition_MngDef.h"
//#include "../../../TRDll/Dr_Include/Condition_MngDef.h"
#include "./DefineFiles/client_typedef.h"
#include "./DefineFiles/server_typedef.h"
#include "./DefineFiles/TranHelperd1022.h"

#include "../../ForSite/FS_Env.h"	//Solomon-090829

#include "../../Chart/COMMONTR/TRDefine.h"

//#include "Winsock2.h"
//#include "DefineFiles/Signal_MngDef.h"
//#include "DefineFiles/Signal_PacketDef.h"
//#include "DefineFiles/server_typedef.h"

class CTreeResItem
{
public:
	ITrComm2*		m_pTrComm;
	LIST_CBaseLongData*		m_pItemList;
	HTREEITEM		m_hGwansim;
	HTREEITEM		m_hImsi;
	ITrCommSite*	m_pTrCommSite;

public:
	CTreeResItem()
	{
		m_pTrComm = NULL;
		m_pItemList = NULL;
		m_hGwansim = NULL;
		m_hImsi = NULL;
		m_pTrCommSite = NULL;
	}
};


class CTreeResItemEx : public CTreeResItem
{
public:
	CMapStringToPtr *m_pmapCondition;

	long	m_dwReserved1;	//현재는 사용안함. 추후사용을 위한 메모리공간 확보.
	long	m_dwReserved2;	//현재는 사용안함. 추후사용을 위한 메모리공간 확보.
	long	m_dwReserved3;	//현재는 사용안함. 추후사용을 위한 메모리공간 확보.

	CTreeResItemEx()
	{
		m_pmapCondition = NULL;
		m_dwReserved1 = NULL;
		m_dwReserved2 = NULL;
		m_dwReserved3 = NULL;
	}
};

typedef CList<CTreeResItem*, CTreeResItem*> LIST_CTreeResItem;
typedef CList<CTreeResItemEx*, CTreeResItemEx*> LIST_CTreeResItemEx;


// 2006.03.10 By JoonOkLee
// 기존과는 다른 형태로 LoadLib를 구현해봄..

#define DEFINE_FUNC_PROC(funcname)	Func##funcname		Lib##funcname
#define DEFINE_FUNC_NULL(funcname)	Lib##funcname = NULL;

#define INIT_FUNC_PROC(funcname);\
	Lib##funcname = (Func##funcname)GetProcAddress(m_hLib,#funcname);\
	if(Lib##funcname == NULL)\
	{\
		AfxMessageBox("funcname 함수를 찾을수 없습니다.");\
	}

typedef BOOL(*Func_DLL_GetOther_ConditionPath) (CString strFilePath, CString strWorkFolder, BOOL bGetAlert, 
                                          CStringArray &saGroupName, CStringArray &saPath);
typedef BOOL(*Func_DLL_GetFind_ConditionPath_Array) (int nType, BOOL bGetAlert, 
											CStringArray &saName, CStringArray &saPath);
typedef  int (*Func_DLL_GetFind_Packet) (CString strPath, int nTrIndex, BOOL bGetIndustry, BOOL bDefultResultField,
                                  BOOL bOrdering_Apply, LPSTR &lpPacket, int &nOrderCnt);
typedef  void (*Func_DLL_GetFind_Packet_ByteOrdering) (User_Finder *pstUserFinder);
typedef  int (*Func_DLL_GetFilterToPacket) (CString strFullPath, long *plPacket);



  //#        CString nType => 0 : 추천, 1 : 사용자, 2 : 빠른조건검색
  //#        BOOL bIndustry => TRUE : 업종, FALSE : 종목
  //#        BOOL bGetAlert => TRUE : Alert 사용 조건만, FALSE : 전체 조건
  //#        BOOL bJustLockGroup => TRUE : Only Lock Group, FALSE : All Group
  //#
  //#        CStringArray &saGroupName => 조건 그룹을 받을 주소
  //#        CStringArray &saCondiData => 조건 데이터를 받을 주소
  //#						[형식] : Group View Name | File View Name | Condition File Path | Alert State | 설명 |
typedef  BOOL (*Func_DLL_GetConditionData)(int nType, BOOL bIndustry, BOOL bJustLockGroup, 
                                                            CStringArray &saGroupName, CStringArray &saCondiData);

typedef  int (*Func_DLL_Open)(CStringArray *psaMSTCode);

  //# [설명] : Lib 소멸
typedef  void (*Func_DLL_Close)();

class CConditionMngLoader
{
public:
	HINSTANCE m_hLib;
	CString m_szLibName;
	int		m_LoadCnt;

	Func_DLL_GetOther_ConditionPath			Lib_DLL_GetOther_ConditionPath;
	Func_DLL_GetFind_ConditionPath_Array	Lib_DLL_GetFind_ConditionPath_Array;
	Func_DLL_GetFind_Packet					Lib_DLL_GetFind_Packet;
	Func_DLL_GetFind_Packet_ByteOrdering	Lib_DLL_GetFind_Packet_ByteOrdering;
	Func_DLL_GetFilterToPacket				Lib_DLL_GetFilterToPacket;
	Func_DLL_GetConditionData				Lib_DLL_GetConditionData;
	Func_DLL_Open							Lib_DLL_Open;
	Func_DLL_Close							Lib_DLL_Close;

//	Constructor..Destructor..
public:
	CConditionMngLoader()
	{
		m_LoadCnt = 0;
		m_hLib = NULL;

		//-->Solomon-090829
		//#include "../../ForSite/FS_Env.h"	//Solomon-090829
		//m_szLibName = "Lib_ConditionMng.dll";

		CString szDir;
		GetDir_DrfnBin(szDir);
		m_szLibName.Format("%s\\%s", szDir, "Lib_ConditionMng.dll");
		//<--Solomon-090829

		DEFINE_FUNC_NULL(_DLL_GetOther_ConditionPath);
		DEFINE_FUNC_NULL(_DLL_GetFind_ConditionPath_Array);
		DEFINE_FUNC_NULL(_DLL_GetFind_Packet);
		DEFINE_FUNC_NULL(_DLL_GetFind_Packet_ByteOrdering);
		DEFINE_FUNC_NULL(_DLL_GetFilterToPacket);
		DEFINE_FUNC_NULL(_DLL_GetConditionData);
		DEFINE_FUNC_NULL(_DLL_Open);
		DEFINE_FUNC_NULL(_DLL_Close);
	}

	~CConditionMngLoader()
	{
	}


	HINSTANCE LoadLib()
	{
		if(m_hLib)
		{
			m_LoadCnt++;
			return m_hLib;
		}

		HINSTANCE hLib = NULL;
		hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( hLib == NULL)
		{
			CString szMsg;
			szMsg.Format("%s을 Load할 수 없습니다.", m_szLibName);
			TRACE("%s\n", szMsg);
			// AfxMessageBox(szMsg);
			return NULL;
		}
		m_hLib = hLib;
		m_LoadCnt++;

		LoadProcess();

		return hLib;
	}

	BOOL UnLoadLib()
	{
		m_LoadCnt--;
		if(m_LoadCnt>0) return TRUE;

		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;

		return bResult;
	}


	void LoadProcess()
	{
		if(!m_hLib)	return ;

		INIT_FUNC_PROC(_DLL_GetOther_ConditionPath);
		INIT_FUNC_PROC(_DLL_GetFind_ConditionPath_Array);
		INIT_FUNC_PROC(_DLL_GetFind_Packet);
		INIT_FUNC_PROC(_DLL_GetFind_Packet_ByteOrdering);
		INIT_FUNC_PROC(_DLL_GetFilterToPacket);
		INIT_FUNC_PROC(_DLL_GetConditionData);
		INIT_FUNC_PROC(_DLL_Open);
		INIT_FUNC_PROC(_DLL_Close);
	}

	BOOL GetOther_ConditionPath(CString strFilePath, CString strWorkFolder, BOOL bGetAlert, 
                                          CStringArray &saGroupName, CStringArray &saPath)
	{
		if(!m_hLib ||!Lib_DLL_GetOther_ConditionPath)	return FALSE;

		return Lib_DLL_GetOther_ConditionPath(strFilePath, strWorkFolder, bGetAlert, saGroupName, saPath);
	}

	BOOL GetFind_ConditionPath_Array(int nType, BOOL bGetAlert, 
											CStringArray &saName, CStringArray &saPath)
	{
		if(!m_hLib ||!Lib_DLL_GetFind_ConditionPath_Array)  return FALSE;
		return Lib_DLL_GetFind_ConditionPath_Array(nType, bGetAlert, saName, saPath);
	}

	int GetFind_Packet(CString strPath, int nTrIndex, BOOL bGetIndustry, BOOL bDefultResultField,
                                  BOOL bOrdering_Apply, LPSTR &lpPacket, int &nOrderCnt)
	{
		if(!m_hLib ||!Lib_DLL_GetFind_Packet)	return 0;

		return Lib_DLL_GetFind_Packet(strPath, nTrIndex, bGetIndustry, bDefultResultField, bOrdering_Apply, lpPacket, nOrderCnt);

	}

	void GetFind_Packet_ByteOrdering(User_Finder *pstUserFinder)
	{
		if(!m_hLib ||!Lib_DLL_GetFind_Packet_ByteOrdering) return;

		Lib_DLL_GetFind_Packet_ByteOrdering(pstUserFinder);
	}

	int GetFilterToPacket(CString strFullPath, long *plPacket)
	{
		if(!m_hLib ||!Lib_DLL_GetFilterToPacket)	return 0;

		return Lib_DLL_GetFilterToPacket(strFullPath, plPacket);
	}

	int GetConditionData(int nType, BOOL bIndustry, BOOL bJustLockGroup, 
                          CStringArray &saGroupName, CStringArray &saCondiData)
	{
		if(!m_hLib ||!Lib_DLL_GetConditionData)	return 0;

		return Lib_DLL_GetConditionData(nType, bIndustry, bJustLockGroup, saGroupName, saCondiData);
	}

	int Open(CStringArray *psaMSTCode)
	{
		if(!m_hLib ||!Lib_DLL_Open)	return 0;

		return Lib_DLL_Open(psaMSTCode);
	}

	void Close()
	{
		if(!m_hLib ||!Lib_DLL_Close)	return;

		Lib_DLL_Close();
	}
};


#define		MAX_CCondHelper_Cnt		30
class CCondHelper : public CConditionMngLoader
{
public:
	int			m_nTrIndex, m_nCurTrIndex;
	POSITION    m_TrPos[MAX_CCondHelper_Cnt];
//	BOOL		m_b0622Ing;
	HWND		m_hWrapper, m_hSenComm;

	CCondHelper()
	{
		m_hSenComm = NULL;
		m_nTrIndex = m_nCurTrIndex = 0;
		for(int i=0; i<MAX_CCondHelper_Cnt; i++) m_TrPos[i]=0;

//		m_b0622Ing = FALSE;
	}


	virtual void Init_New(CTreeResItemEx* pList, CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem, int nType)
	{
		if(pList->m_pmapCondition==NULL)
			pList->m_pmapCondition = new CMapStringToPtr();
		
		CStringArray saGroupName, saCondiData;
        if (Lib_DLL_GetConditionData(nType, FALSE, FALSE, saGroupName, saCondiData) == TRUE)
			SubInit_New(pList, pTreeCtrl, hTreeItem, &saGroupName, &saCondiData);

		saGroupName.RemoveAll();
		saCondiData.RemoveAll();
	}


	void SubInit_New(CTreeResItemEx* pList, CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem,
		CStringArray *psaGroupName, CStringArray *psaCondiData)
	{
		CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;

		HTREEITEM	htiChild;
		int nIndex, nIndex_Group, nIndex_Item;

		int nFindPos;
		CString strGroupName, strCondiData, strFileName;
		CString strInfo[5];
		for (nIndex_Group = 0; nIndex_Group < psaGroupName->GetSize(); nIndex_Group++)
		{
			strGroupName = psaGroupName->GetAt(nIndex_Group);

			tcItem.pszText = (LPTSTR)(LPCTSTR)psaGroupName->GetAt(nIndex_Group);
			htiChild = pTreeCtrl->InsertItem(tcItem.pszText, hTreeItem);

			plistConditionInfo = (CList <ConditionInfo, ConditionInfo&>*)Find_Condition(strGroupName, pList);

			if(plistConditionInfo==NULL)
				plistConditionInfo = new CList <ConditionInfo, ConditionInfo&>;

			for (nIndex_Item = 0; nIndex_Item < psaCondiData->GetSize(); nIndex_Item++)
			{
				strCondiData = psaCondiData->GetAt(nIndex_Item);
				
				nIndex = 0;
				//#	[형식] : Group View Name | File View Name | Condition File Path | Alert State | 설명 |
				while (strCondiData.IsEmpty() == FALSE)
				{
					nFindPos = strCondiData.Find("|", 0);
					if (nFindPos > -1)
					{
						strInfo[nIndex] = strCondiData.Left(nFindPos);
						strCondiData = strCondiData.Mid(nFindPos + 1, strCondiData.GetLength() - nFindPos);
						
						nIndex++;
					}
					else
						strCondiData = "";
				}
				
				if (strGroupName == strInfo[0])
				{
					ConditionInfo	stConditionInfo;
					
					stConditionInfo.nGroupID = nIndex_Group + 1;
					stConditionInfo.strGroupName = strGroupName;
					
					strInfo[1].Replace("_", " ");
					stConditionInfo.strName = strInfo[1];
					stConditionInfo.strPath = strInfo[2];
					if(stConditionInfo.strName.GetLength())
					{
						HTREEITEM hItem = pTreeCtrl->InsertItem(stConditionInfo.strName, htiChild);
						CItemOneClickData* pDataItem = new CItemOneClickData;
						pDataItem->m_szOCPath = stConditionInfo.strPath;
						pDataItem->m_pITrComm = (long)pList->m_pTrComm;
						pDataItem->m_pITrcommSite = (long)pList->m_pTrCommSite;
						pTreeCtrl->SetItemData(hItem, (DWORD)pDataItem);
						pList->m_pItemList->AddTail(pDataItem);
					}

					stConditionInfo.bLoad = FALSE;
					stConditionInfo.nOrderCnt = 0;
					memset(&stConditionInfo.stUser_Finder, 0x00, SZ_User_Finder);
					plistConditionInfo->AddTail(stConditionInfo);
				}
			}
			pList->m_pmapCondition->SetAt(strGroupName, plistConditionInfo);
		}
	}


	virtual void Init(CTreeResItemEx* pList, CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem)
	{
		pList->m_pmapCondition = new CMapStringToPtr();

		HTREEITEM hSubItem;
		CStringArray saPath;
		// m_saGroupName.RemoveAll();
		CStringArray saGroupName;
		CString strFilePath("OneClkList.xml");

		CString strWorkFolder=CONDITIONLIST_FOLDER;
		if (GetOther_ConditionPath(strFilePath, strWorkFolder,
			FALSE, saGroupName, saPath) == TRUE)
		{
			TCITEM tcItem;
			tcItem.mask = TCIF_TEXT;
			for (int nIndex_Group = 0; nIndex_Group < saGroupName.GetSize(); nIndex_Group++)
			{
				tcItem.pszText = (LPTSTR)(LPCTSTR)saGroupName.GetAt(nIndex_Group);
				// 테마종목은 별도 트리가 있으므로 원클릭에서는 뺀다.
				if(tcItem.pszText && strcmp(tcItem.pszText, "테마")==0) continue;
				// 사용자검색 원클릭에서는 뺀다.
				else if(tcItem.pszText && strcmp(tcItem.pszText, "사용자검색")==0) continue;

				hSubItem = pTreeCtrl->InsertItem(tcItem.pszText, hTreeItem);
				SubInit(pList, tcItem.pszText, saGroupName, saPath, nIndex_Group, pTreeCtrl, hSubItem);
			}

			TRACE("TreeListMng::Init Count[%d]\n", pList->m_pmapCondition->GetCount());

		}
		else
		{
			saGroupName.RemoveAll();
			// SetSendMainMsg(strFilePath + " 파일 Load Error!");
			TRACE("%[s] 파일 Load Error!", strFilePath);
		}


//		BOOL  bSetIndustry = FALSE;
//		if (bSetIndustry == FALSE)
//		{
//			//# 사용자 
//			strGroupName = "사용자검색";
//			CStringArray saItemName;
//			if (GetFind_ConditionPath_Array(1, FALSE, saItemName, saPath) == TRUE)
//			{
//				plistConditionInfo = new CList <ConditionInfo, ConditionInfo&>;
//				for (int nIndex_Item = 0; nIndex_Item < saItemName.GetSize(); nIndex_Item++)
//				{
//					ConditionInfo	stConditionInfo;
//					
//					//stConditionInfo.nGroupID = IDC_BTN_GROUP + (nIndex_Group + 1);
//					stConditionInfo.nGroupID = nIndex_Group + 1;
//					stConditionInfo.strGroupName = strGroupName;
//					
//					stConditionInfo.strName = saItemName.GetAt(nIndex_Item);
//					stConditionInfo.strPath = saPath.GetAt(nIndex_Item);
//					
//					stConditionInfo.bLoad = FALSE;
//					memset(&stConditionInfo.stUser_Finder, 0x00, SZ_User_Finder);
//					plistConditionInfo->AddTail(stConditionInfo);
//				}
//				m_mapCondition.SetAt(strGroupName, plistConditionInfo);
//			}
//		}
	}

	void SubInit(CTreeResItemEx* pList, CString strGroupName, CStringArray &saGroupName, CStringArray &saPath, int nIndex_Group, CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem)
	{
		CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;

		CString  strPath, strInfo[5];//, sSavePath;
		int nFindPos, nIndex;
//		for (int nIndex_Group = 0; nIndex_Group < saGroupName.GetSize(); nIndex_Group++)
//		{
//			strGroupName = saGroupName.GetAt(nIndex_Group);
			
			plistConditionInfo = new CList <ConditionInfo, ConditionInfo&>;
			for (int nIndex_Item = 0; nIndex_Item < saPath.GetSize(); nIndex_Item++)
			{
				strPath = saPath.GetAt(nIndex_Item);
				//sSavePath = strPath;
				
				nIndex = 0;
				while (strPath.IsEmpty() == FALSE)
				{
					nFindPos = strPath.Find("|", 0);
					if (nFindPos > -1)
					{
						strInfo[nIndex] = strPath.Left(nFindPos);
						strPath = strPath.Mid(nFindPos + 1, strPath.GetLength() - nFindPos);
						nIndex++;
					}
					else
						strPath = "";
				}
				
				//if(strInfo[0].GetLength()) pTreeCtrl->InsertItem(strInfo[0], hTreeItem);

				if (strGroupName == strInfo[0])
				{
					ConditionInfo	stConditionInfo;
					
					//stConditionInfo.nGroupID = IDC_BTN_GROUP + (nIndex_Group + 1);
					stConditionInfo.nGroupID = nIndex_Group + 1;
					stConditionInfo.strGroupName = strGroupName;
					
					strInfo[1].Replace("_", " ");
					stConditionInfo.strName = strInfo[1];
					stConditionInfo.strPath = strInfo[2];
					if(stConditionInfo.strName.GetLength())
					{
						HTREEITEM hItem = pTreeCtrl->InsertItem(stConditionInfo.strName, hTreeItem);
						CItemOneClickData* pDataItem = new CItemOneClickData;
						pDataItem->m_szOCPath = stConditionInfo.strPath;
						pDataItem->m_pITrComm = (long)pList->m_pTrComm;
						pDataItem->m_pITrcommSite = (long)pList->m_pTrCommSite;
						pTreeCtrl->SetItemData(hItem, (DWORD)pDataItem);
						pList->m_pItemList->AddTail(pDataItem);
					}

					stConditionInfo.bLoad = FALSE;
					stConditionInfo.nOrderCnt = 0;
					memset(&stConditionInfo.stUser_Finder, 0x00, SZ_User_Finder);
					plistConditionInfo->AddTail(stConditionInfo);
				}
			}
			
			pList->m_pmapCondition->SetAt(strGroupName, plistConditionInfo);
//		}

	}

//	#define TRNO_CONDIONECLK_RESULT "50004"

	BOOL SetTrSend_Result(POSITION pos, CItemOneClickData* pDataItem, LPCSTR szScreenNo);

	int GetMakePacket_Condition(CItemOneClickData* pDataItem, User_Finder *pstUser_Finder)
	{
		int nResult = 0;

		char *pcPacket;
		int nOrderCnt = 0;
		if (GetFind_Packet(pDataItem->m_szOCPath, 0, FALSE, TRUE, TRUE, 
			pcPacket, nOrderCnt) > 0)
		{
			memcpy(pstUser_Finder, pcPacket, SZ_User_Finder);
			 delete pcPacket;
			nResult++;
			return nResult;
		}
		return nResult;
	}

	virtual void SetClear_Condition(CTreeResItemEx* pList)
	{
		CMapStringToPtr *pmapCondition = pList->m_pmapCondition;
		if(!pmapCondition) return;

		TRACE("TreeListMng::SetClear_Condition Count[%d]\n", pmapCondition->GetCount());

		POSITION pos;
		CString strGroupName;
		CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;
		for (pos = pmapCondition->GetStartPosition(); pos != NULL;)
		{
			pmapCondition->GetNextAssoc(pos, strGroupName, (void*&)plistConditionInfo);

//			for(POSITION spos=plistConditionInfo->GetHeadPosition(); spos; )
//			{
//			}
			plistConditionInfo->RemoveAll();
			delete plistConditionInfo;
			plistConditionInfo = NULL;
			
			pmapCondition->RemoveKey(strGroupName);
		}
		pmapCondition->RemoveAll();
		delete pmapCondition;
		pList->m_pmapCondition = NULL;
	}
	
	virtual LONG Find_Condition(LPCSTR szFindGroupName, CTreeResItemEx* pList)
	{
		CMapStringToPtr *pmapCondition = pList->m_pmapCondition;
		if(!pmapCondition) return NULL;

		POSITION pos;
		CString strGroupName;
		CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;
		for (pos = pmapCondition->GetStartPosition(); pos != NULL;)
		{
			pmapCondition->GetNextAssoc(pos, strGroupName, (void*&)plistConditionInfo);
			if(strGroupName.CompareNoCase(szFindGroupName)==0)
			{
				return (LONG)plistConditionInfo;
			}
		}
		return NULL;
	}

	int Modual_SendTr(CString szTrNo, int nTrLen, BYTE *TrData, CItemOneClickData* pDataItem, LPCSTR szScreenNo);

	void InMakeFid_D1022(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);

	int D1022OutRec1_GetDataCount(KB_d1022_OutRec1* pOutRec);

	void D1022OutRec1_SetData(KB_d1022_OutRec1* pOutRec, TR_RESULT_HEADER* pTrResult);

	void D1022OutRec2_SetData(KB_d1022_OutRec2* pOutRec2, char* pDataBuf, int nCnt);
};

#endif //__LOAD_FOR_CONDITIONMANAGER__BY_JUNOK_LEE__HEADER__

