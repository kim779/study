// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

	// 관심그룹 띄우기
	//	nShowOpt  : 0-전체, 1-관심그룹, 2-손익설정, 3-필드설정, 4-필드속성설정, 5-화면설정
	// nType  : nShowOpt(필드설정)일때 필드그룹 index이다.
//@신영제거extern int Dll_ShowPortfolioDlg(CString strCodeList, void* pConfigMngr, void* pFieldGroupMngr, CString strCurrKey, CString &strLastKey, int nShowOpt, int nType, CWnd* pWnd = NULL);
//@삼성것 extern int Dll_ShowPortfolioDlg(CString strCodeList, CString strName, CString& strTargetName, int nShowOpt, int nType);

	// 관심그룹 리스트 얻기
//@신영제거extern void Dll_GetRegKwansimMasterGroupKey( CStringArray &arrKey, CStringArray &arrName);
//@삼성것 extern void Dll_GetRegKwansimMasterGroupName( CStringArray &arrName );

	//관심종목 리스트 얻기
//@신영제거extern void Dll_GetKwGroupItem(CString strKey, CStringArray& arrCode, CStringArray& arrName);
//@삼성것 extern void Dll_GetRegKwansimMasterGroupItem( CString strName, CStringArray &arrItem );

//extern void Dll_SetRegKwansimMasterGroupItem( CString strName, CStringArray &arrItem );
//extern void Dll_GetRegKwansimMasterGroupItemForSort( CString strName, CStringArray &arrItem, int nSort );
//@신영제거extern BOOL Dll_AddRegKwansimMasterGroupItem( CString strKey, CStringArray &arrItem, int nIndex );

	//그룹변경 시 메세지 처리
	//	등록 : extern int Dll_SetRegKwCombo( CWnd* pComboWnd, int nType); ////[EDIT] 관심종목 콤보를 관리
	//	변경시 : 윈도우에 메시지
	//	#define UM_CHANGE_REGKW    (WM_USER + 605)  // 관심종목 그룹변경 처리용

// 관심그룹 변경정보 등록하고 받기.
//@신영제거extern int Dll_SetRegKwCombo( CWnd* pComboWnd, int nType);	////[EDIT] 관심종목 콤보를 관리
	//	nType:1이면 등록, 0이면 해제
	//	WM_USER + 525 LParam이 변경된 그룹코드

	// 최근 조회종목 얻어오기
//@신영제거extern	CStringArray* KSLink_GetRecentCodeList(CString strMarket);

	// 관심 임시그룹으로 띄우기
//@신영제거extern int Dll_ShowSearchGroup(CStringArray& arrCode, CWnd* pWnd);

/////////////////////////////////////////////////////////////////////////////
// CAUGwanManager
// 	CAUGwanManager::CAUGwanManager()
// 	{
// 		m_pWinixGwanMng = NULL;
// 	}
// 
// 	CAUGwanManager::~CAUGwanManager()
// 	{
// 	}
// 
// 	// BOOL CAUGwanManager::CreateGwanWnd()
// 	// {
// 	// 	// return m_GwanWnd.Create(AfxGetMainWnd());
// 	// 	return m_GwanWnd.OpenWindow();
// 	// }
// 	// 
// 
// 	long CAUGwanManager::_GetInnerInterface()
// 	{
// 		return NULL;
// 
// 	// 		if(!m_pWinixGwanMng) return (long)m_pWinixGwanMng;
// 	// 
// 	// 		m_pPctrWinixLoader
// 	// 		theApp.m_pPtMngLoader->PTMSetPcTrInterface(UUID_IGateManager, (long)&theApp.m_GateMng);
// 	// 
// 	// 	//	//UUID_IChartBozoMsgManager 51
// 	// 	//	m_pWinixGwanMng = (IAUGwanManager*)theApp.m_uaInterface[51].pInterface = theApp.m_pPtMngLoader->PTMGetInferface(1, NULL);
// 	// 		return (long)m_pWinixGwanMng;
// 	}
// 
// 	long CAUGwanManager::Advise(LPCSTR szKey, long pThisPoint, long dwKey, DataMonitorCallbackProc pCallFunc)
// 	{
// 		_GetInnerInterface();
// 	// 	if(m_GwanWnd.GetSafeHwnd()==NULL)
// 	// 		CreateGwanWnd();
// 	// 
// 	// 	m_GwanWnd.GwanAdvise();
// 	// 	return theApp.m_pDataMonMng->Advise(szKey, pThisPoint, dwKey, pCallFunc);
// 		return NULL;
// 	}
// 
// 	void CAUGwanManager::UnAdvise(long dwRet)
// 	{
// 	// 	m_GwanWnd.GwanUnAdvise();
// 	// 	theApp.m_pDataMonMng->UnAdvise(dwRet);
// 	}
// 
// 	void CAUGwanManager::NotifyData(LPCSTR szKey, long dwSubKey, long dwNotifyData)
// 	{
// 	}
// 
// 	long CAUGwanManager::GetGwansimDataAll(int nType, LPCSTR szGroup)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) return m_pWinixGwanMng->GetGwansimDataAll(nType, szGroup);
// 
// 	//	COleVariant* pVar = theApp.m_MainExinfoMng.GetData(IMainExInfoManager_Gwansim);
// 	//	return pVar->lVal;
// 		return NULL;
// 	}
// 
// 	// void CAUGwanManager::PreUnLoad()
// 	// {
// 	// 	m_GwanWnd.DestroyWindow();
// 	// }
// 
// 	void CAUGwanManager::GetGwansimGroupName(CStringArray &arrKey, CStringArray &arrName)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) m_pWinixGwanMng->GetGwansimGroupName(arrKey, arrName);
// 
// 	//@Solomon090819	m_intrMngr.GetIntrKeyIndex(arrKey);
// 	//@Solomon090819	m_intrMngr.GetIntrGroup(arrName);
// 	}
// 
// 	void CAUGwanManager::GetGwansimItem(CString strKey, CStringArray& arrCode, CStringArray& arrName)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) m_pWinixGwanMng->GetGwansimItem(strKey, arrCode, arrName);
// 		return;
// 
// 	//@신영제거
// 	//	if(strKey == "최근조회종목_CHART#")
// 	//	{
// 	//		//최근종목
// 	//		CString	strCode, strName, strTemp;
// 	//			
// 	//		short nHistoryIndex, nHistoryCount ;
// 	//		CStringArray* pArr;
// 	//		pArr = 	::KSLink_GetRecentCodeList("01");
// 	//		if(!pArr) return;
// 	//
// 	//		nHistoryCount = pArr->GetSize();
// 	//	
// 	//		for( nHistoryIndex = 0; nHistoryIndex < nHistoryCount; nHistoryIndex++)
// 	//		{
// 	//			strTemp = pArr->GetAt(nHistoryIndex);
// 	//
// 	//			if(strTemp.GetLength() >= 6)
// 	//			{
// 	//				strCode =  strTemp.Mid(5, 6);
// 	//				arrCode.Add(strCode);
// 	//			}
// 	//		}
// 	//		return;
// 	//	}
// 	//	//Dll_GetRegKwansimMasterGroupItem( strName, arrItem );
// 	//	::Dll_GetKwGroupItem(strKey, arrCode, arrName);
// 
// 		arrCode.RemoveAll();
// 		arrName.RemoveAll();
// 
// 		int nCount=0;	
// 		int nGrpIndex = atoi(strKey);
// 	//@Solomon090819	m_intrMngr.GetItemCount(nCount, nGrpIndex);
// 
// 		CString strCode, strName;
// 		CStringArray strData;
// 		for(int nIndex = 0; nIndex < nCount; ++nIndex)
// 		{
// 	//@Solomon090819		if(!m_intrMngr.GetItemInfo(nGrpIndex, nIndex, strData))
// 	//@Solomon090819			continue;
// 	//@Solomon090819
// 	//@Solomon090819		arrCode.Add(strData.GetAt(eIntrSymCode));
// 	//@Solomon090819		arrName.Add(strData.GetAt(eIntrSymName));
// 		}
// 	}
// 
// 	int CAUGwanManager::ShowGwansimDlg(CWnd* pCaller, CString strCodeList /*=""*/)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) return m_pWinixGwanMng->ShowGwansimDlg(pCaller, strCodeList);
// 
// 	//@신영제거
// 	//	CString strTarget;
// 	//	CString strLastKey;
// 	//	int nShowOpt = 1,
// 	//		nType = 0;
// 	//	return ::Dll_ShowPortfolioDlg(strCodeList, NULL, NULL, "", strLastKey, nShowOpt, nType, pCaller);
// 
// 	//@Solomon090819	m_intrMngr.OpenIntrSetDlg(CIntrApiHelper::INTR_ALL, 0, strCodeList); 
// 		return 0;
// 	}
// 
// 	BOOL CAUGwanManager::ADDGwasimItem(CString strKey, CString strItem, BOOL bFirst)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) return m_pWinixGwanMng->ADDGwasimItem(strKey, strItem, bFirst);
// 
// 
// 	//@신영제거
// 	//	if(bFirst)
// 	//		return ::Dll_AddRegKwansimMasterGroupItem(strKey, arrItem, 0);
// 	//	else
// 	//		return ::Dll_AddRegKwansimMasterGroupItem(strKey, arrItem, -1);
// 
// 	//	m_intrMngr.InsertIntrItems(arrItem, atoi(strKey));
// 	//@Solomon090819	m_intrMngr.InsertIntrItem(strItem, atoi(strKey));
// 		return FALSE;
// 	}
// 
// 	// 	BOOL CAUGwanManager::ADDGwasimItems(CString strKey, CStringArray& arrItem)
// 	// 	{
// 	// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 	// 		if(m_pWinixGwanMng) return m_pWinixGwanMng->ADDGwasimItems(strKey, arrItem);
// 	// 
// 	// 	//@신영제거
// 	// 	//	if(bFirst)
// 	// 	//		return ::Dll_AddRegKwansimMasterGroupItem(strKey, arrItem, 0);
// 	// 	//	else
// 	// 	//		return ::Dll_AddRegKwansimMasterGroupItem(strKey, arrItem, -1);
// 	// 
// 	// 	//@Solomon090819	m_intrMngr.InsertIntrItems(arrItem, atoi(strKey));
// 	// 		return FALSE;
// 	// 	}
// 
// 	int CAUGwanManager::SetRegGwansim(CWnd* pComboWnd, int nType)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) return m_pWinixGwanMng->SetRegGwansim(pComboWnd, nType);
// 
// 
// 	//@신영제거	return ::Dll_SetRegKwCombo(pComboWnd, nType);
// 		return 0;
// 	}
// 
// 	int CAUGwanManager::RegiestToImsiGroup(CStringArray& arrCode, CWnd* pWnd)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) return m_pWinixGwanMng->RegiestToImsiGroup(arrCode, pWnd);
// 
// 
// 	//@신영제거	return ::Dll_ShowSearchGroup(arrCode, pWnd);
// 		return 0;
// 	}
// 
// 	LPCSTR CAUGwanManager::GetGwanKeyFromName(LPCSTR szGwanGroupName)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) return m_pWinixGwanMng->GetGwanKeyFromName(szGwanGroupName);
// 		return "";
// 
// 	//	CStringArray arrKey, arrName;
// 	//	m_intrMngr.GetIntrKeyIndex(arrKey);
// 		int nIndex = -1;
// 	//@Solomon090819	m_intrMngr.GetGroupIndex(nIndex, szGwanGroupName);
// 		
// 		if(nIndex >= 0)
// 			theApp.m_szTemp.Format("%d", nIndex);	//arrKey.GetAt(i);
// 		else 
// 			return "";
// 
// 		return theApp.m_szTemp;
// 	}
// 
// 	int CAUGwanManager::ShowGwansimDlgWithList(CWnd* pCaller, CStringArray& arrItem)
// 	{
// 		if(!m_pWinixGwanMng) _GetInnerInterface();
// 		if(m_pWinixGwanMng) return m_pWinixGwanMng->ShowGwansimDlgWithList(pCaller, arrItem);
// 
// 	//@Solomon090819	m_intrMngr.OpenIntrSetDlgWithList(arrItem); 
// 		return 0;
// 	}
