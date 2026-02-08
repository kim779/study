// SiteCustomAddin.cpp: implementation of the CSiteCustomAddin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./SiteCustomAddin.h"

#include "../Include_Chart/BlockBaseEnum.h"				// for CToolOptionInfo
#include "./Include_AddIn/I139092/_IPacketList.h"		// for CIPacketList
#include "./Include_AddIn/I139092/_IPacket.h"			// for IPacket
#include "./Include_AddIn/I139092/_IPacketManager.h"	// for IPacketManager

//#include "./Include_AddIn/I429091/_IPacketManager.h"	// for IPacketManager
//#include "./Include_AddIn/I429091/_IPacket.h"			// for IPacket
#include "../Common_ST/IDataItem.h"
#include "../Common_ST/STConfigdef.h"
#include "./WndAssistIndexManager.h"

#include "../../inc/IMainExInfoMng.h"				// for IMainExInfoManager2
#include "../Include_ST/STOrderDef.h"
#include "../Include_ST/SiteDefine.h"					// for g_strChartProgID

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString Parser(LPCTSTR lpInputData, int *pNShifter );
extern HINSTANCE g_hInstance;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiteCustomAddin::CSiteCustomAddin( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
	if( g_strChartProgID.IsEmpty())
	{
		ILPCSTR szProgID = p_pIChartOCX->GetProgID();
		g_strChartProgID = szProgID;
	}

	// (2006/2/22 - Seung-Won, Bae) Create Manager Directly in constructor.
	//	m_assistIndexManager.Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
	//	m_assistIndexManager.SetSiteCUstomAddin((LONG)this);

	m_lpszRQ = _T("DEFAULT");	// 현재 RQ : 복수종목 - ojtaso (20070305)

	// RQ별 관리 : 복수종목 - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager = new CWndAssistIndexManager;
	pAssistIndexManager->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
	pAssistIndexManager->SetSiteCUstomAddin((LONG)this);

	m_mapWndAssistIndexManager.insert(MAP_WNDASSISTINDEXMANAGER::value_type(m_lpszRQ, pAssistIndexManager));
}

CSiteCustomAddin::~CSiteCustomAddin()
{
	//	m_assistIndexManager.DestroyWindow();

	// RQ별 관리 : 복수종목 - ojtaso (20070305)
	if(!m_mapWndAssistIndexManager.empty())
	{
		MAP_WNDASSISTINDEXMANAGER::iterator it = m_mapWndAssistIndexManager.begin();
		while(it != m_mapWndAssistIndexManager.end())
		{
			(*it).second->DestroyWindow();
			delete (*it++).second;
		}
	}
}


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CSiteCustomAddin)
	ONADDINEVENT( OnDrawBeforeGraph)
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// 	ONADDINEVENT( OnPacketData)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnUpdatePacketData)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_char)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_CObject)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_long)
	ONADDINEVENT( OnLButtonDblClk)
	// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char)
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	ONADDINEVENT( OnUpdatePacketDataMultiItem)
	ONADDINEVENT( OnPacketDataMultiItem)
	// 복수종목 추가/삭제 : 복수종목 - ojtaso (20070302)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char)
END_ADDIN_EVENT_MAP()


// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
IUnknown *CSiteCustomAddin::GetAddInCustomInterface( void)
{
// (2006/1/23 - Seung-Won, Bae) Do not use it. Use AddIn Command!
/*
	IUnknown *pIUnknown = ( IUnknown *)( IChartAssistIndexAddIn *)this;
	pIUnknown->AddRef();
	return pIUnknown;
*/
	return NULL;
}

// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
void CSiteCustomAddin::GetAssistIndexManager(CWndAssistIndexManager*& pAssistIndexManager, LPCTSTR lpszRQ/* = NULL*/)
{
	BOOL bRetunNull = TRUE;

	if(!lpszRQ)
	{
		lpszRQ = m_lpszRQ;
		bRetunNull = FALSE;
	}

	MAP_WNDASSISTINDEXMANAGER::iterator it = m_mapWndAssistIndexManager.find(lpszRQ);
	if(it == m_mapWndAssistIndexManager.end())
	{
		// RQ를 지정했을경우 기본 RQ의 값을 리턴하지 않는다
		if(bRetunNull)
		{
			pAssistIndexManager = NULL;
			return;
		}
		else
		{
			it = m_mapWndAssistIndexManager.find(_T("DEFAULT"));
			// (2008/10/16 - Seung-Won, Bae) Check NULL.
			if(it == m_mapWndAssistIndexManager.end())
			{
				pAssistIndexManager = NULL;
				return;
			}
		}
	}

	pAssistIndexManager = (*it).second;
}

void CSiteCustomAddin::OnResetChart( void)
{
	//// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	//CWndAssistIndexManager* pAssistIndexManager;
	//GetAssistIndexManager(pAssistIndexManager);

	//pAssistIndexManager->ClearData();
	//m_pIChartOCX->InvalidateControl();
	//pAssistIndexManager->m_hSysTabEx = NULL;
}

// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CSiteCustomAddin::OnDrawBeforeGraph( HDC p_hDC) //  CDC *p_pDC)
{
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());
	HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	//	GetAssistIndexManager(pAssistIndexManager);
	MAP_WNDASSISTINDEXMANAGER::iterator it = m_mapWndAssistIndexManager.begin();
	while(it != m_mapWndAssistIndexManager.end())
	{
		pAssistIndexManager = (*it).second;

		if(pAssistIndexManager->GetSafeHwnd())
		{
			CDC* pDC = CDC::FromHandle(p_hDC);
			pAssistIndexManager->Draw(pDC);// to draw system treading signal
			//pDC->ReleaseOutputDC();
		}

		it++;
	}
	AfxSetResourceHandle(hInstResourceClient);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CalculateIndicator
 - Created at  :  2005-07-11   09:56
 - Author      :  최종찬
 - Parameters  :  pcszIndicatorName - 지표명
 - Return Value:  BOOL - TRUE: 성공, FALSE: 실표
 - Description :  지표를 추가 및 계산한다. (지표추가시, 지표설정값 변경시)
 -----------------------------------------------------------------------------------------*/
// (2006/1/9 - Seung-Won, Bae) CObject * <= const CList<CPacket*, CPacket*> *
/*
BOOL CSiteCustomAddin::CalculateIndicator(const char* pcszIndicatorName, CObject *p_plCPacket)
{
	// (2006/1/9 - Seung-Won, Bae) Use IPacket for suitable Version Number
	// IPacket으로 변환하여 AddIn에 보내준다. AddIn에서는 CPacket은 모르나
	// IPacket인터페이스는 알기 때문이다.
	BOOL bResult = FALSE;
	CList <IPacket *, IPacket *> lIPacketList;
	if( m_pIPacketManager->GetIPacketListFromCPacketList( p_plCPacket, &lIPacketList))
	{
		if(m_assistIndexManager.GetSafeHwnd())
		{
			m_assistIndexManager.AddGraph( pcszIndicatorName, &lIPacketList);
			bResult = TRUE;
		}
		IPacket *pIPacket = NULL;
		while( !lIPacketList.IsEmpty())
		{
			pIPacket = lIPacketList.RemoveHead();
			if( pIPacket) pIPacket->Release();
		}
	}
	return bResult;
}
*/


// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
BOOL CSiteCustomAddin::CalculateIndicator(LPCTSTR lpszRQ, LPCTSTR lpszParamCombine, CObject *p_plCPacket)
{
	// (2006/1/9 - Seung-Won, Bae) Use IPacket for suitable Version Number
	// IPacket으로 변환하여 AddIn에 보내준다. AddIn에서는 CPacket은 모르나
	// IPacket인터페이스는 알기 때문이다.
	//KHD : 변형 
	BOOL bResult = FALSE;
	CIPacketList lPacketList;

	if( m_pIPacketManager->GetIPacketListFromCPacketList(p_plCPacket, &lPacketList) )
	{
		CWndAssistIndexManager* pAssistIndexManager;
		GetAssistIndexManager(pAssistIndexManager, lpszRQ);

		if(pAssistIndexManager->GetSafeHwnd())
		{
			pAssistIndexManager->AddGraph( lpszParamCombine, &lPacketList);
			bResult = TRUE;
		}
		IPacket *pIPacket = NULL;
		while( !lPacketList.IsEmpty())
		{
			pIPacket = lPacketList.RemoveHead();
			if( pIPacket) pIPacket->Release();
		}
	}
	return bResult;
	//END
}

/*-----------------------------------------------------------------------------------------
 - Function    :  DeleteIndicator
 - Created at  :  2005-07-11   09:56
 - Author      :  최종찬
 - Parameters  :  pcszIndicatorName - 지표명
 - Return Value:  BOOL - TRUE: 성공, FALSE: 실패
 - Description :  지표를 삭제한다.
 -----------------------------------------------------------------------------------------*/
// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
BOOL CSiteCustomAddin::DeleteIndicator(LPCTSTR lpszRQ, LPCTSTR lpszParamCombine)
{
	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	GetAssistIndexManager(pAssistIndexManager, lpszRQ);

	if(pAssistIndexManager->GetSafeHwnd())
	{
		pAssistIndexManager->RemoveIndexItem(lpszParamCombine);		
		return TRUE;
	}	
	return FALSE;	
}

BOOL CSiteCustomAddin::CalculateAll(const int nDataOption)
{
	return TRUE;
}

// [04/10/13] TR의 수신을 AddIn DLL들에게 알려 대응토록 한다.
void CSiteCustomAddin::OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2006/2/22 - Seung-Won, Bae) Init I/O Packet.
	// (2006/2/24 - Seung-Won, Bae) Do not SetRawData() in CalculateIndicator.
//	if( m_assistIndexManager.GetSafeHwnd()) m_assistIndexManager.SetRawData();
}

// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
void CSiteCustomAddin::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2006/2/22 - Seung-Won, Bae) Init I/O Packet.
	// (2006/2/24 - Seung-Won, Bae) Do not SetRawData() in CalculateIndicator.
	//	if( m_assistIndexManager.GetSafeHwnd()) m_assistIndexManager.SetRawData(lpszRQ);

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	GetAssistIndexManager(pAssistIndexManager, lpszRQ);

	if(!pAssistIndexManager)
		return;

	if( pAssistIndexManager->GetSafeHwnd()) pAssistIndexManager->SetRawData();
}

// [04/12/07] TR이나 Real의 수신에 의해 Packet Data가 Update되었음을 AddIn DLL들에 알려 지표등을 처리토록 한다.
void CSiteCustomAddin::OnUpdatePacketData( BOOL p_bTrUpdate)
{
//	if(p_bTrUpdate==0&&m_assistIndexManager.GetSafeHwnd())
//	{
//		m_assistIndexManager.UpdateRawData();
//	}
}

// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
void CSiteCustomAddin::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	GetAssistIndexManager(pAssistIndexManager, lpszRQ);

	if(!pAssistIndexManager)
		return;

	if(p_bTrUpdate==0&&pAssistIndexManager->GetSafeHwnd())
	{
		//		m_assistIndexManager.UpdateRawData(lpszRQ);
		pAssistIndexManager->UpdateRawData();
	}
}

//////////////////////////////////////////////////////////////////////
// Temp Interface III	: 사장되는 Option 기능의 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////
// JSJ_ADD_50119__차트에서 InvokeAddIn함수 호출함
BOOL CSiteCustomAddin::InvokeAddIn( int p_nCommandType, long p_lData)
{
	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;

	// (2004.10.19, 배승원) Resource의 접근이 잘되도록 Module State를 적용한다.
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());
	BOOL bRetValue = FALSE;
	switch(p_nCommandType)
	{
	case 1:// add
		{
			GetAssistIndexManager(pAssistIndexManager, m_lpszRQ);
			if(!pAssistIndexManager)
			{
				CWndAssistIndexManager* pAssistIndexManager = new CWndAssistIndexManager;
				pAssistIndexManager->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
				pAssistIndexManager->SetSiteCUstomAddin((LONG)this);//?

				m_mapWndAssistIndexManager.insert(MAP_WNDASSISTINDEXMANAGER::value_type(m_lpszRQ, pAssistIndexManager));
			}
			CStringArray *pStrArrayName = (CStringArray *)p_lData;
			bRetValue = pAssistIndexManager->AddST(pStrArrayName->GetAt(0),pStrArrayName->GetAt(1));
			m_pIChartOCX->InvalidateControl();
		}
		break;


	//멀티차트에서 불릴경우 CChartMng::OnDestroy()에서 콜됨.
	//pChartItem->InvokeAddIn("ChartSTIndexAddIn.dll",2,0);
	case 2://Clear
		{
			GetAssistIndexManager(pAssistIndexManager);
			// (2008/10/16 - Seung-Won, Bae) Check NULL.
			if( pAssistIndexManager) pAssistIndexManager->DeleteSTInfo();
			bRetValue = TRUE;
		}
		break;

	case 3: // 자기자신 셋팅
		{
			TRACE("point is %ld\n", p_lData);
			IMainExInfoManager2* pMng = (IMainExInfoManager2*)AfxGetPctrInterface(UUID_IMainExInfoManager2);
			if(pMng)
			{
				LONG pValue = (LONG)&m_xSTAddinData;
				pMng->SetUserExData(p_lData, pValue);
				TRACE("p_lData=%ld, pValue=%ld\n", p_lData, pValue);
			}
		}
		break;
		//KHD by : 2006.08.29
	case 4:// It is For Strategy Analysis View
		{
			//m_assistIndexManager.SetpChartModule(m_pIChartManager, m_pIChartOCX, m_pIPacketManager);
			GetAssistIndexManager(pAssistIndexManager);

			pAssistIndexManager->SetOnlyDealSignal(p_lData);
			m_pIChartOCX->InvalidateControl();
		}
		break;
	case 5:// It is For Strategy Analysis View
		{
			GetAssistIndexManager(pAssistIndexManager);

			pAssistIndexManager->SetpChartModule(m_pIChartManager, m_pIChartOCX, m_pIPacketManager);
			pAssistIndexManager->SetPeriod(p_lData);
			//m_pIChartOCX->InvalidateControl();
		}	
		break;
	case 6:// It is For Strategy Analysis View
		{
			GetAssistIndexManager(pAssistIndexManager);

			pAssistIndexManager->m_hSysTabEx = (HWND)p_lData;	 //KHD 2006.11.22 HWND
			//m_assistIndexManager.SetpChartModule(m_pIChartManager, m_pIChartOCX, m_pIPacketManager);
			//m_assistIndexManager.SetHandleUpdatePeriod(p_lData);
			//m_pIChartOCX->InvalidateControl();
		}	
		break;
	case 7://현재 챠트의 전략매매표시 및 수익률그래프를 없앤다. : KHD 
		{
			GetAssistIndexManager(pAssistIndexManager);

			pAssistIndexManager->DestroyStrategyItem();
			pAssistIndexManager->ClearData();
			m_pIChartOCX->InvalidateControl();

		}
		break;
	case 8:// It is For Strategy Analysis View
		{
			GetAssistIndexManager(pAssistIndexManager);

			CString* pstrSTPath = (CString*)p_lData;
			(*pstrSTPath) = pAssistIndexManager->m_szMSTName;
		}
		break;
	case 10:	// 전략(ST)을 실행한다.
		{
			GetAssistIndexManager(pAssistIndexManager, m_lpszRQ);
			if(!pAssistIndexManager)
			{
				pAssistIndexManager = new CWndAssistIndexManager;
				pAssistIndexManager->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
				pAssistIndexManager->SetSiteCUstomAddin((LONG)this);

				m_mapWndAssistIndexManager.insert(MAP_WNDASSISTINDEXMANAGER::value_type(m_lpszRQ, pAssistIndexManager));
			}			
			stAddinSTRun* pData = (stAddinSTRun*)p_lData;	
			
			{
				//분할상태 
				pAssistIndexManager->SetChartDivideInfo((int)m_pIPacketManager->GetBaseTimeDataType(), m_pIPacketManager->GetBaseTimeUnit());
			}

			if(pData->nCount <= 0)
			{
				pAssistIndexManager->ClearData();
				m_pIChartOCX->InvalidateControl();
				pAssistIndexManager->m_hSysTabEx = NULL;			//@@@@@06.11.20 HWND
				return bRetValue;
			}

			int nRet = pAssistIndexManager->LoadST((LPCSTR)pData->szSTFile,
				pData->pChartInterface,
				(LPCSTR)pData->szCode,
				pData->nCount,
				(LPCSTR)pData->szEndData,
				pData->stArrayPath,
				pData->szSTName);

			pAssistIndexManager->m_StDataIndex.RemoveAll();//성과분석에서 잔재한 데이터 Clear : KHD : 2006.11.06
			pAssistIndexManager->m_hSysTabEx = pData->wndInfo.hSystemTabEx;		//@@@@@06.11.20 HWND

			pAssistIndexManager->m_STConfigInfo.m_szSTFileName = pData->szSTFile;		// ST파일명
			pAssistIndexManager->m_STConfigInfo.m_szSTName = pData->szSTName;			// 전략명
			pAssistIndexManager->m_STConfigInfo.m_szCode = pData->szCode;	// 종목명
			pAssistIndexManager->m_STConfigInfo.m_szCodeName = pData->szCodeName;
			pAssistIndexManager->m_STConfigInfo.m_nCodeType = pData->nCodeType;
			if(nRet==0)
			{	// case 7일때의 동작
				// 0인경우는 실행중인 전략의 삭제일때임,
				// 마이너스값은 에러
				// 플러스는 성공
				pAssistIndexManager->ClearData();
				m_pIChartOCX->InvalidateControl();
				pAssistIndexManager->m_hSysTabEx = NULL;			//@@@@@06.11.20 HWND
			}
		}
		break;
	case 11:	// 현재 조합전략의 실행상태를 리턴한다.
		{
			GetAssistIndexManager(pAssistIndexManager);
			// (2008/10/16 - Seung-Won, Bae) Check NULL.
			if( !pAssistIndexManager) return FALSE;

			bRetValue = TRUE;
			stAddinSTRunInfo* pInfo = (stAddinSTRunInfo*)p_lData;
			pInfo->bIsRun = FALSE;
			pInfo->szRunSTFile = pAssistIndexManager->m_szMSTName;
			if(pAssistIndexManager->GetSafeHwnd())
			{
				if(pInfo->nSTType == 4)		// 강세약세
				{
					CString strSTName(pInfo->szSTFile);
					int nPosExt = strSTName.ReverseFind('.');
					int nPosFileTitle = strSTName.ReverseFind(('\\'));
					strSTName = strSTName.Left(nPosExt);
					strSTName = strSTName.Mid(nPosFileTitle+1);
					if(strSTName.Left(2) == "P_")
						strSTName = strSTName.Mid(2);

					if(strSTName.CompareNoCase(pAssistIndexManager->m_szSTName) == 0)
					{
						// 실행중일 때만 TRUE;
						pInfo->bIsRun = TRUE;
					}
				}
				else if(pAssistIndexManager->m_szMSTName.CompareNoCase(pInfo->szSTFile)==0) 
				{
					// 실행중일 때만 TRUE;
					pInfo->bIsRun = TRUE;
				}
			}
		}
		break;
	// 복수종목 추가로 CSiteCustomAddin을 사용하지 않는다 : 복수종목 - ojtaso (20070306)
/*	
	case 20:	// 주문신호
		{
			STORDER_INDATA* lpSTOrderData = (STORDER_INDATA*)p_lData;
			m_assistIndexManager.OrderSign(lpSTOrderData);
		}
		break;
*/
	case 200:	//전체초기화
		{	
			// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
			CWndAssistIndexManager* pAssistIndexManager = NULL;
			GetAssistIndexManager(pAssistIndexManager);

			// (2008/10/16 - Seung-Won, Bae) Check NULL.
			if( !pAssistIndexManager) return FALSE;

			pAssistIndexManager->ClearData();
			m_pIChartOCX->InvalidateControl();
			pAssistIndexManager->m_hSysTabEx = NULL;
			bRetValue = TRUE;
		}
		break;
	}

	return bRetValue;
}


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

//{{JS.Kim_20100908 데이타건수 초과시 삭제할때 지표 반영안되는 문제 수정
BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_ST_REMOVED_DATA_COUNT == p_eCommandID )
	{
		CWndAssistIndexManager* pAssistIndexManager = NULL;
		GetAssistIndexManager(pAssistIndexManager);
		if(pAssistIndexManager && pAssistIndexManager->GetSafeHwnd())
		{
			pAssistIndexManager->UpdateRawData();
		}
		return TRUE;
	}
	return FALSE;
}
//}}

BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)
{
	/* BAE, 2개, 이동평균, #3333, =>  InedicatorName, GraphName */
	//	if( EAI_ALL_ON_DELETE_INDICATOR == p_eCommandID)
	//	{
	//		CString szParamCombine = p_szParam1;
	//		szParamCombine += ";";
	//		szParamCombine += p_szParam2;
	//		//		DeleteIndicator( p_szParam);
	//		DeleteIndicator( szParamCombine);
	//	}
	return TRUE;
}

BOOL CSiteCustomAddin::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam)
{
	/* BAE, IndicatorName, GraphName, CObjectg!!*/
	//	if( EAI_ST_CALCULATE_INDICATOR == p_eCommandID)
	//	{
	//		CString szParamCombine = p_szParam1;
	//		szParamCombine += ";";
	//		szParamCombine += p_szParam2;
	//		//		p_lResult = CalculateIndicator( p_szParam1, p_pParam);
	//		p_lResult = CalculateIndicator( szParamCombine, p_pParam);
	//		return TRUE;
	//	}
	return FALSE;
}

BOOL CSiteCustomAddin::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_ST_CALCULATE_ALL == p_eCommandID)
	{
		p_lResult = CalculateAll( p_lParam);
		return TRUE;
	}
	return FALSE;
}

// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
BOOL CSiteCustomAddin::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam)
{
	if( EAI_ST_CALCULATE_INDICATOR == p_eCommandID)
	{
		if( EAI_ST_CALCULATE_INDICATOR == p_eCommandID)
		{
			CString szParamCombine = p_szParam2;	// Indicator Name
			szParamCombine += ";";
			szParamCombine += p_szParam3;			// Graph Name (Unique)
			//		p_lResult = CalculateIndicator( p_szParam1, p_pParam);
			// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
			p_lResult = CalculateIndicator(p_szParam1, szParamCombine, p_pParam);
			return TRUE;
		}
		return TRUE;
	}

	return FALSE;
}

// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
{
	/* BAE, 2개, 이동평균, #3333, =>  InedicatorName, GraphName */
	if( EAI_ALL_ON_DELETE_INDICATOR == p_eCommandID)
	{
		CString szParamCombine = p_szParam3;		// Indicator Name
		szParamCombine += ";";
		szParamCombine += p_szParam2;				// Graph Name (Unique)
		//		DeleteIndicator( p_szParam);
		// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
		DeleteIndicator(p_szParam1, szParamCombine);
	}
	return TRUE;
}

// BOOL CSiteCustomAddin::OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
BOOL CSiteCustomAddin::OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	GetAssistIndexManager(pAssistIndexManager);

	CPoint pt(point);
	if(pAssistIndexManager->GetSafeHwnd()) return pAssistIndexManager->OnLButtonDblClk(nFlags,pt);
	return FALSE;
}

// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
int CSiteCustomAddin::CSTAddinData::GetChartData(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,LONG& dwDataVolume, LONG& dwDataExt)
{
	MYMETHOD_PROLOGUE(CSiteCustomAddin, STAddinData)

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	pThis->GetAssistIndexManager(pAssistIndexManager);

	dwDataTime	= (LONG)&pAssistIndexManager->m_SDataItemDate;
	dwDataOpen	= (LONG)&pAssistIndexManager->m_DataItemOpen;
	dwDataHigh	= (LONG)&pAssistIndexManager->m_DataItemHigh;
	dwDataLow	= (LONG)&pAssistIndexManager->m_DataItemLow;
	dwDataClose	= (LONG)&pAssistIndexManager->m_DataItemClose;
	dwDataVolume= (LONG)&pAssistIndexManager->m_DataItemVolume;
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	dwDataExt	= (LONG)&pAssistIndexManager->m_DataItemExt;

	return pAssistIndexManager->m_DataItemOpen.GetSize();
}

// 복수종목 추가/삭제 : 복수종목 - ojtaso (20070302)
BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
{
	if(p_eCommandID == EAI_PACKETLISTMANAGER_NEW_RQ)
	{
		CWndAssistIndexManager* pAssistIndexManager;
		GetAssistIndexManager(pAssistIndexManager, p_szParam);
		// 같은 RQ에 대해선 추가하지 않음
		if(!pAssistIndexManager)
		{
			CWndAssistIndexManager* pAssistIndexManager = new CWndAssistIndexManager;
			pAssistIndexManager->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
			pAssistIndexManager->SetSiteCUstomAddin((LONG)this);
			pAssistIndexManager->SetRQ(p_szParam);

			m_mapWndAssistIndexManager.insert(MAP_WNDASSISTINDEXMANAGER::value_type(p_szParam, pAssistIndexManager));
			m_lpszRQ = p_szParam;
		}
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_CHANGE_RQ)
	{
		m_lpszRQ = p_szParam;
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_DELETE_RQ)
	{
		CWndAssistIndexManager* pAssistIndexManager;
		GetAssistIndexManager(pAssistIndexManager, p_szParam);

		if(pAssistIndexManager)
		{
			pAssistIndexManager->DestroyWindow();
			delete pAssistIndexManager;
		}

		m_mapWndAssistIndexManager.erase(p_szParam);
	}

	return FALSE;
}
