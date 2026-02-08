// GapRevisionAddInImp.cpp: implementation of the CChartDataListViewerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "GapRevisionAddInImp.h"

#include "../../Include_Chart/Conversion.h"							// for CDataConversion
#include "../../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../../Include_Chart/IPropertyMap.h"						// for IPropertyMap

#include "../Include_AddIn/_resource.h"								// for ID_COA_...
#include "../Include_AddIn/AddInCommand.h"							// for EAI_DRDS_PACKET_END

#include "../Include_AddIn/I000000/_IChartManager.h"				// for IChartManager
#include "../Include_AddIn/I000000/_IPacket.h"						// for IPacket
#include "../Include_AddIn/I000000/_IPacketManager.h"				// for IPacketManager
#include "../Include_AddIn/I000000/_IPacketListManager.h"			// for IPacketListManager
#include "../Include_AddIn/I000000/_IString.h"						// for ILPCSTR
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"				// for ILPCSTR
#include "../Include_AddIn/I000000/_IChartOCX.h"					// for IChartOCX
#include "../Include_AddIn/I000000/_IStringList.h"					// for IStringList

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define HELP_MSG_MARKETTYPE		"MARKETTYPE"
#define HELP_MSG_MARKETTIME		"MARKETTIME"

// ST_GAP_INFO
ST_GAP_INFO::ST_GAP_INFO()
{
	Init();
}

ST_GAP_INFO::~ST_GAP_INFO()
{
	Init();
}

void ST_GAP_INFO::Init()
{
	m_arGapInfo.RemoveAll();
	m_nCurRealDate = 0;
	m_nPacketCount = 0;
}

void ST_GAP_INFO::SetGapInfo(double dGapValue, BOOL bReal/* = TRUE*/)
{
	// HEAD : 현재, TAIL : 과거
	if(bReal)
		m_arGapInfo.AddHead(dGapValue);
	else
		m_arGapInfo.AddTail(dGapValue);
}

double ST_GAP_INFO::GetGapInfo(int nIndx)
{
	int nInfoCount = m_arGapInfo.GetCount();
//	ASSERT(nIndx >= 0 && nIndx < nInfoCount);
	if(nIndx == nInfoCount)
		return 0.0;

	double dGapInfo = 0.0;
	int nCount = nIndx + 1;
	int nInfoIndx = 0;
	while(nCount--)
	{
		dGapInfo += m_arGapInfo.GetAt(m_arGapInfo.FindIndex(nInfoIndx));
		nInfoIndx++;
	}

	return dGapInfo;
}

void ST_GAP_INFO::SetRealDate(int nCurRealDate)
{
	m_nCurRealDate = nCurRealDate;
}

int ST_GAP_INFO::GetRealDate()
{
	return m_nCurRealDate;
}

void ST_GAP_INFO::SetPacketCount(int nPacketCount)
{
	m_nPacketCount = nPacketCount;
}

int ST_GAP_INFO::GetPacketCount()
{
	return m_nPacketCount;
}

int ST_GAP_INFO::GetAllGapInfo(double& dAllGapValue)
{
	dAllGapValue = 0.0;
	int nInfoCount = m_arGapInfo.GetCount();
	for(int nIndx = 0; nIndx < nInfoCount; ++nIndx)
	{
		dAllGapValue += m_arGapInfo.GetAt(m_arGapInfo.FindIndex(nIndx));
	}

	return nInfoCount;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGapRevisionAddInImp::CGapRevisionAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_bUseGapRevision = FALSE;

	m_bUseMonthGapRevision = FALSE;
	m_bShowMonthGapRevision = FALSE;
	// (2009/9/19 - Seung-Won, Bae) Manage OnPacketDataMultiItem Flag of On-GapRevision.
	m_bOnPacketDataMultiItem = FALSE;
}

CGapRevisionAddInImp::~CGapRevisionAddInImp()
{
	CString strRQ;
	ST_GAP_INFO* pSTGapInfo = NULL;
	POSITION pos = m_mapRQGapInfo.GetStartPosition();
	while( pos)
	{
		m_mapRQGapInfo.GetNextAssoc( pos, strRQ, pSTGapInfo);
		delete pSTGapInfo;
		pSTGapInfo = NULL;
	}
	m_mapRQGapInfo.RemoveAll();
}

CString CGapRevisionAddInImp::m_strAddInItemName = "GAP_REVISION";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CGapRevisionAddInImp)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnRealPacketEnd)
END_ADDIN_EVENT_MAP()

BOOL CGapRevisionAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( EAI_ENVSETUP_GAPREVISION_CHANGED != p_eCommandID )		return FALSE;

	ML_SET_LANGUAGE_RES();

	SetUseGapRevision( p_lParam1, p_lParam2);

	return TRUE;
}

//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
BOOL CGapRevisionAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1)
{
	if( EAI_ENVSETUP_MONTHGAPREVISION_CHANGED != p_eCommandID &&
		EAI_ENVSETUP_SHOWMONTHGAPREVISION_CHANGED != p_eCommandID )
		return FALSE;

	ML_SET_LANGUAGE_RES();

	if (EAI_ENVSETUP_MONTHGAPREVISION_CHANGED == p_eCommandID)
		m_bUseMonthGapRevision = p_lParam1;
	else if (EAI_ENVSETUP_SHOWMONTHGAPREVISION_CHANGED == p_eCommandID)
		m_bShowMonthGapRevision = p_lParam1;

	return TRUE;
}

// AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
BOOL CGapRevisionAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	switch (p_nCommandType)
	{
	case 0:		OnAddInToolCommand(EAI_ENVSETUP_SHOWMONTHGAPREVISION_CHANGED, p_lData);	break;
	case 1:		return m_bUseMonthGapRevision;
	default:	break;
	}

	return TRUE;
}
//>> (Issue No : 0006572)

BOOL CGapRevisionAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID)
{
	if( EAI_CUSTOM_GET_GAPREVISION != p_eCommandID &&
		EAI_CUSTOM_GET_MONTHGAPREVISION != p_eCommandID &&
		EAI_ENVSETUP_SHOWMONTHGAPREVISION_CHANGED != p_eCommandID)
		return FALSE;

	ML_SET_LANGUAGE_RES();

	if (EAI_CUSTOM_GET_GAPREVISION == p_eCommandID)
		p_lResult = m_bUseGapRevision;
//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
	else if (EAI_CUSTOM_GET_MONTHGAPREVISION == p_eCommandID)
		p_lResult = m_bUseMonthGapRevision;
	else if (EAI_ENVSETUP_SHOWMONTHGAPREVISION_CHANGED == p_eCommandID)
		p_lResult = m_bShowMonthGapRevision;
//>> (Issue No : 0006572)

	return TRUE;

}

void CGapRevisionAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( !p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;

	CString strEnvValue = szEnvValue;

	if( strEnvValue.GetLength() <= 0 )
		return;

	CString strUseGapRevision = strEnvValue.Mid( 0, 1 );
	SetUseGapRevision( ( atoi( strUseGapRevision) == 1), FALSE);

//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
	int nIdx = strEnvValue.Find(":");
	if( nIdx < 0 )	return;
	strEnvValue = strEnvValue.Mid(nIdx+1);
	m_bUseMonthGapRevision = atoi(strEnvValue);
//>> (Issue No : 0006572)
}

void CGapRevisionAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	CString strEnvValue;
	// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
	strEnvValue.Format( "%d:%d", m_bUseGapRevision ? 1 : 0, m_bUseMonthGapRevision ? 1 : 0);
	p_pIAddInEnvMap->SetAt( m_strAddInItemName, strEnvValue);
}

void CGapRevisionAddInImp::SetUseGapRevision( const BOOL bUseGapRevision, const BOOL bWithNewData)
{
	if( m_bUseGapRevision != bUseGapRevision)
	{
		m_bUseGapRevision = bUseGapRevision;

		//--------------------------------------------------------
		//>> 갭보정 사용시 처리
		IStringList *pIStringList = m_pIPacketListManager->GetAllRQ();
		pIStringList->AddTail( "DEFAULT");

		IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
		if( !pIAddInToolMgr)
		{
			pIStringList->Release();
			return;
		}
		
		if( m_bUseGapRevision && !bWithNewData)
		{
			POSITION psn = pIStringList->GetHeadPosition();
			ILPCSTR szData;
			while( psn)
			{
				pIStringList->GetNext( psn, szData);
				ST_GAP_INFO* pSTGapInfo = NULL;
				if( !m_mapRQGapInfo.Lookup( szData, pSTGapInfo))
					m_mapRQGapInfo.SetAt( CString( szData), new ST_GAP_INFO);

				CalcGapRevision( szData);

				if( pIAddInToolMgr)
				{
					IPacketManager *pPacketList = m_pIPacketListManager->GetPacketManager( szData);
					if( pPacketList)
					{
						CString strTRPacketNames = pPacketList->GetTRPacketNames();
						// (2009/9/19 - Seung-Won, Bae) Manage OnPacketDataMultiItem Flag of On-GapRevision.
						m_bOnPacketDataMultiItem = TRUE;
						pIAddInToolMgr->OnPacketDataMultiItem( szData, strTRPacketNames, TRUE);
						m_bOnPacketDataMultiItem = FALSE;
						pPacketList->Release();
					}
				}
			}
			if( m_pIChartManager != NULL)
				m_pIChartManager->CalculateAllGraphs( CCalculateBaseData::TRDATA_CAL);
		}
		//<< 갭보정 사용시 처리
		else if( !m_bUseGapRevision)
		{
			POSITION psn = pIStringList->GetHeadPosition();
			ILPCSTR szData;
			while( psn)
			{
				pIStringList->GetNext( psn, szData);

				ST_GAP_INFO* pSTGapInfo = NULL;
				if( !m_mapRQGapInfo.Lookup( szData, pSTGapInfo))
					m_mapRQGapInfo.SetAt( CString( szData), new ST_GAP_INFO);

				RestoreGapRevision( szData);

				if( pIAddInToolMgr)
				{
					IPacketManager *pPacketList = m_pIPacketListManager->GetPacketManager( szData);
					if( pPacketList)
					{
						CString strTRPacketNames = pPacketList->GetTRPacketNames();
						// (2009/9/19 - Seung-Won, Bae) Manage OnPacketDataMultiItem Flag of On-GapRevision.
						m_bOnPacketDataMultiItem = TRUE;
						pIAddInToolMgr->OnPacketDataMultiItem( szData, strTRPacketNames, TRUE);
						m_bOnPacketDataMultiItem = FALSE;
						pPacketList->Release();
					}
				}
			}
			if( m_pIChartManager != NULL)
				m_pIChartManager->CalculateAllGraphs( CCalculateBaseData::TRDATA_CAL);
		}

		pIAddInToolMgr->Release();
	}
}

void CGapRevisionAddInImp::CalcGapRevision( const char *p_szRQ, const BOOL bIsReset/* = TRUE*/)
{
	if( !m_bUseGapRevision || !p_szRQ || !*p_szRQ) return;

	IPacketManager *pPacketList = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pPacketList) return;

	//--------------------------------------------------------
	//>> CME(1),COBT(2) 거래소 종목은 지원하지 않음
	CString szMarketType;
	ILPCSTR szIStr = NULL;
	pPacketList->GetHelpMessageItem( HELP_MSG_MARKETTYPE, szIStr);
	szMarketType = szIStr;
	int nMarketType = atoi(szMarketType);

	if( nMarketType == 1 || nMarketType == 2)
	{
		pPacketList->Release();
		return;
	}
	//<< CME(1),COBT(2) 거래소 종목은 지원하지 않음

	// Market BaseTime 얻음
	CString szMarketTime;
	pPacketList->GetHelpMessageItem( HELP_MSG_MARKETTIME, szIStr);
	szMarketTime = szIStr;

	int nBaseTime = 0;
	if(szMarketTime.GetLength() >= 5 && szMarketTime.Find("|") > 0)
	{
		CString strStartTime = szMarketTime.Mid(0, szMarketTime.Find("|"));
		nBaseTime = atoi(strStartTime);
	}
	
	// RQ별 패킷 얻음
	IPacket *pTimePacket = pPacketList->GetPacket(_MTEXT( C0_DATE_TIME));
	if( !pTimePacket)
	{
		pPacketList->Release();
		return;
	}

	// 패킷타입 Check
	CString strType;
	szIStr = pTimePacket->GetType();
	strType = (LPCTSTR)szIStr;
	if( strType != "DDHHMMSS" && strType != "MMDDHHMM")
	{
		pTimePacket->Release();
		pPacketList->Release();
		return;
	}

	ST_GAP_INFO* pSTGapInfo = NULL;
	if( !m_mapRQGapInfo.Lookup( p_szRQ, pSTGapInfo))
	{
		pTimePacket->Release();
		pPacketList->Release();
		return;
	}

	IPacket *pOpenPacket = pPacketList->GetPacket(_MTEXT( C0_OPEN));
	IPacket *pHighPacket = pPacketList->GetPacket(_MTEXT( C0_HIGH));
	IPacket *pLowPacket = pPacketList->GetPacket(_MTEXT( C0_LOW));
	IPacket *pClosePacket = pPacketList->GetPacket(_MTEXT( C0_CLOSE));
	pPacketList->Release();
	
	//--------------------------------------------------------
	//>> 갭보정 처리
	double dTime, dOpen, dHigh, dLow, dClose;
	double dGap=0, dGapData;
	BOOL bNeedCalcuate = FALSE;

	int nPacketCount = pClosePacket->GetDataCount();
	int nCurIndx = 0;
	int nNextIndx = -1;
	int nCurDay = 0, nNextDay = 0, nCurTime=0, nNextTime=0;
	BOOL bInitialized = FALSE;

	// 데이터 추가부르기 처리
	if(bIsReset)
	{
		pSTGapInfo->Init();
		nCurIndx = nPacketCount - 1;
		pSTGapInfo->SetPacketCount(nPacketCount);
	}
	else
	{
		nCurIndx = (nPacketCount - pSTGapInfo->GetPacketCount()) - 1;
		nNextIndx = nCurIndx + 1;
		pSTGapInfo->SetPacketCount(nPacketCount);
		if(pSTGapInfo->GetAllGapInfo(dGap) > 0)
			bNeedCalcuate = TRUE;
		bInitialized = TRUE;

		pTimePacket->GetData(nNextIndx, dTime);
		if(strType == "MMDDHHMM")
		{
			nNextDay = (int)dTime/10000;
			nNextTime = (int)dTime%10000 * 100;
		}
		else if(strType == "DDHHMMSS")
		{
			nNextDay = (int)dTime/1000000;
			nNextTime = (int)dTime%1000000;
		}
	}

	for(; nCurIndx >= 0; --nCurIndx)
	{
		pTimePacket->GetData(nCurIndx, dTime);
		if(strType == "MMDDHHMM")
		{
			nCurDay = (int)dTime/10000;
			nCurTime = (int)dTime%10000 * 100;
		}
		else if(strType == "DDHHMMSS")
		{
			nCurDay = (int)dTime/1000000;
			nCurTime = (int)dTime%1000000;
		}

		if(bInitialized == FALSE)
		{
			bInitialized = TRUE;
			pSTGapInfo->SetRealDate(nCurDay);
		}

		if(nCurDay != nNextDay && nCurTime > nBaseTime)
		{
			if(nCurIndx != (pClosePacket->GetDataCount()-1))
			{
				double dOpenData, dCloseData;
				pOpenPacket->GetData(nNextIndx, dOpenData);
				pClosePacket->GetData(nCurIndx, dCloseData);
				dGapData = (dOpenData - dGap) - dCloseData;
				dGap += dGapData;
				bNeedCalcuate = TRUE;

				pSTGapInfo->SetGapInfo(dGapData, FALSE);
			}
		}

		if(bNeedCalcuate)
		{
			pOpenPacket->GetData(nCurIndx, dOpen);
			dOpen += dGap;
			pHighPacket->GetData(nCurIndx, dHigh);
			dHigh += dGap;
			pLowPacket->GetData(nCurIndx, dLow);
			dLow += dGap;
			pClosePacket->GetData(nCurIndx, dClose);
			dClose += dGap;

			pOpenPacket->SetData(nCurIndx, dOpen);
			pHighPacket->SetData(nCurIndx, dHigh);
			pLowPacket->SetData(nCurIndx, dLow);
			pClosePacket->SetData(nCurIndx, dClose);
		}

		nNextIndx = nCurIndx;
		nNextDay = nCurDay;
		nNextTime = nCurTime;
	}
	//<< 갭보정 처리

	pTimePacket->Release();
	pOpenPacket->Release();
	pHighPacket->Release();
	pLowPacket->Release();
	pClosePacket->Release();
}

void CGapRevisionAddInImp::RestoreGapRevision( const char *p_szRQ)
{
	if( !p_szRQ || !*p_szRQ)			return;
	int nGapInfoCount = m_mapRQGapInfo.GetCount();
	if( nGapInfoCount <= 0) 	return;

	IPacketManager *pPacketList = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pPacketList) return;

	//--------------------------------------------------------
	//>> CME(1),COBT(2) 거래소 종목은 지원하지 않음
	CString szMarketType;
	ILPCSTR szIStr = NULL;
	pPacketList->GetHelpMessageItem( HELP_MSG_MARKETTYPE, szIStr);
	szMarketType = szIStr;
	int nMarketType = atoi(szMarketType);

	if( nMarketType == 1 || nMarketType == 2)
	{
		pPacketList->Release();
		return;
	}
	//<< CME(1),COBT(2) 거래소 종목은 지원하지 않음
	
	// Market BaseTime 얻음
	CString szMarketTime;
	pPacketList->GetHelpMessageItem( HELP_MSG_MARKETTIME, szIStr);
	szMarketTime = szIStr;

	int nBaseTime = 240000;
	if(szMarketTime.GetLength() >= 5 && szMarketTime.Find("|") > 0)
	{
		CString strStartTime = szMarketTime.Mid(0, szMarketTime.Find("|"));
		nBaseTime = atoi(strStartTime);
	}

	// RQ별 패킷 얻음
	IPacket *pTimePacket = pPacketList->GetPacket(_MTEXT( C0_DATE_TIME));
	if( !pTimePacket) 
	{
		pPacketList->Release();
		return;
	}

	// 패킷타입 Check
	CString strType;
	szIStr = pTimePacket->GetType();
	strType = (LPCTSTR)szIStr;
	if( strType != "DDHHMMSS" && strType != "MMDDHHMM")
	{
		pTimePacket->Release();
		pPacketList->Release();
		return;
	}

	ST_GAP_INFO* pSTGapInfo = NULL;
	if( !m_mapRQGapInfo.Lookup( p_szRQ, pSTGapInfo))
	{
		pTimePacket->Release();
		pPacketList->Release();
		return;
	}

	IPacket *pOpenPacket = pPacketList->GetPacket(_MTEXT( C0_OPEN));
	IPacket *pHighPacket = pPacketList->GetPacket(_MTEXT( C0_HIGH));
	IPacket *pLowPacket = pPacketList->GetPacket(_MTEXT( C0_LOW));
	IPacket *pClosePacket = pPacketList->GetPacket(_MTEXT( C0_CLOSE));
	pPacketList->Release();
	
	//--------------------------------------------------------
	//>> 갭보정 처리
	double dTime, dOpen, dHigh, dLow, dClose;
	double dGap=0;
	BOOL bNeedCalcuate = FALSE;

	int nCurIndx = pClosePacket->GetDataCount() - 1;
	int nGapInfoIndx = 0;
	int nCurDay = 0, nNextDay = 0, nCurTime=0, nNextTime=0;
	for(; nCurIndx >= 0; --nCurIndx)
	{
		// 현재에서 과거시간 방향으로 일자/시간 값 얻음
		pTimePacket->GetData(nCurIndx, dTime);
		if(strType == "MMDDHHMM")
		{
			nCurDay = (int)dTime/10000;
			nCurTime = (int)dTime%10000 * 100;
		}
		else if(strType == "DDHHMMSS")
		{
			nCurDay = (int)dTime/1000000;
			nCurTime = (int)dTime%1000000;
		}

		if(nCurDay != nNextDay && nCurTime > nBaseTime)
		{
			if(nCurIndx != (pClosePacket->GetDataCount()-1))
			{
				dGap = pSTGapInfo->GetGapInfo(nGapInfoIndx++);
				bNeedCalcuate = TRUE;
			}
		}

		if(bNeedCalcuate)
		{
			pOpenPacket->GetData(nCurIndx, dOpen);
			dOpen -= dGap;
			pHighPacket->GetData(nCurIndx, dHigh);
			dHigh -= dGap;
			pLowPacket->GetData(nCurIndx, dLow);
			dLow -= dGap;
			pClosePacket->GetData(nCurIndx, dClose);
			dClose -= dGap;

			pOpenPacket->SetData(nCurIndx, dOpen);
			pHighPacket->SetData(nCurIndx, dHigh);
			pLowPacket->SetData(nCurIndx, dLow);
			pClosePacket->SetData(nCurIndx, dClose);
		}

		nNextDay = nCurDay;
		nNextTime = nCurTime;
	}
	//<< 갭보정 처리

	pTimePacket->Release();
	pOpenPacket->Release();
	pHighPacket->Release();
	pLowPacket->Release();
	pClosePacket->Release();
	
	if(pSTGapInfo)
	{
		delete pSTGapInfo;
		pSTGapInfo = NULL;
	}

	m_mapRQGapInfo.RemoveKey(p_szRQ);
	if(m_mapRQGapInfo.GetCount() == 0)
		m_mapRQGapInfo.RemoveAll();
}

void CGapRevisionAddInImp::CalcGapRevisionReal( const char *p_szRQ)
{
	if( !m_bUseGapRevision || !p_szRQ || !*p_szRQ) return;
	
	IPacketManager *pPacketList = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pPacketList) return;
	
	//--------------------------------------------------------
	//>> CME(1),COBT(2) 거래소 종목은 지원하지 않음
	CString szMarketType;
	ILPCSTR szIStr = NULL;
	pPacketList->GetHelpMessageItem( HELP_MSG_MARKETTYPE, szIStr);
	szMarketType = szIStr;
	int nMarketType = atoi(szMarketType);
	
	if( nMarketType == 1 || nMarketType == 2)
	{
		pPacketList->Release();
		return;
	}
	//<< CME(1),COBT(2) 거래소 종목은 지원하지 않음
	
	// Market BaseTime 얻음
	CString szMarketTime;
	pPacketList->GetHelpMessageItem( HELP_MSG_MARKETTIME, szIStr);
	szMarketTime = szIStr;
	
	int nBaseTime = 240000;
	if(szMarketTime.GetLength() >= 5 && szMarketTime.Find("|") > 0)
	{
		CString strStartTime = szMarketTime.Mid(0, szMarketTime.Find("|"));
		nBaseTime = atoi(strStartTime);
	}
	
	// RQ별 패킷 얻음
	IPacket *pTimePacket = pPacketList->GetPacket(_MTEXT( C0_DATE_TIME));
	if( !pTimePacket)
	{
		pPacketList->Release();
		return;
	}
	
	// 패킷타입 Check
	CString strType;
	szIStr = pTimePacket->GetType();
	strType = (LPCTSTR)szIStr;
	if( strType != "DDHHMMSS" && strType != "MMDDHHMM")
	{
		pTimePacket->Release();
		pPacketList->Release();
		return;
	}
	
	ST_GAP_INFO* pSTGapInfo = NULL;
	if( !m_mapRQGapInfo.Lookup( p_szRQ, pSTGapInfo))
	{
		pTimePacket->Release();
		pPacketList->Release();
		return;
	}
	
	IPacket *pOpenPacket = pPacketList->GetPacket(_MTEXT( C0_OPEN));
	IPacket *pHighPacket = pPacketList->GetPacket(_MTEXT( C0_HIGH));
	IPacket *pLowPacket = pPacketList->GetPacket(_MTEXT( C0_LOW));
	IPacket *pClosePacket = pPacketList->GetPacket(_MTEXT( C0_CLOSE));
	pPacketList->Release();

	//--------------------------------------------------------
	//>> 실시간 갭보정 처리
	double dOpen, dHigh, dLow, dClose;
	double dGap=0;
	BOOL bNeedCalcuate = FALSE;
	
	BOOL bCalcReal = FALSE;
	if(pClosePacket->GetDataCount() < 2)	// (2008/12/5 - Seung-Won, Bae) Do not use TimePacket for data count. (there can be NMTR candle.)
		return;
	int nCurIndx = pClosePacket->GetDataCount() - 1;
	int nPrevIndx = nCurIndx - 1;
	int nCurDay, nPrevDay, nCurTime, nPrevTime;
	double dCurTime, dPrevTime;
	pTimePacket->GetData(nCurIndx, dCurTime);
	pTimePacket->GetData(nPrevIndx, dPrevTime);
	
	if(strType == "MMDDHHMM")
	{
		nCurDay = (int)dCurTime/10000;
		nCurTime = (int)dCurTime%10000 * 100;
		nPrevDay = (int)dPrevTime/10000;
		nPrevTime = (int)dPrevTime%10000 * 100;
	}
	else if(strType == "DDHHMMSS")
	{
		nCurDay = (int)dCurTime/1000000;
		nCurTime = (int)dCurTime%1000000;
		nPrevDay = (int)dPrevTime/1000000;
		nPrevTime = (int)dPrevTime%1000000;
	}
	
	if( pSTGapInfo->GetRealDate() == nCurDay)
	{
		pTimePacket->Release();
		pOpenPacket->Release();
		pHighPacket->Release();
		pLowPacket->Release();
		pClosePacket->Release();
		return;
	}

	if(nCurDay == nPrevDay)
	{
		if(nPrevTime < nBaseTime && nCurTime >= nBaseTime)
		{
			double dOpenData, dCloseData;
			pOpenPacket->GetData(nCurIndx, dOpenData);
			pClosePacket->GetData(nPrevIndx, dCloseData);
			dGap = (dOpenData - dCloseData);
			bNeedCalcuate = TRUE;
			
			pSTGapInfo->SetGapInfo(dGap);
			pSTGapInfo->SetRealDate(nCurDay);
		}
	}
	else
	{
		if(nCurTime >= nBaseTime)
		{
			double dOpenData, dCloseData;
			pOpenPacket->GetData(nCurIndx, dOpenData);
			pClosePacket->GetData(nPrevIndx, dCloseData);
			dGap = (dOpenData - dCloseData);
			bNeedCalcuate = TRUE;
			
			pSTGapInfo->SetGapInfo(dGap);
			pSTGapInfo->SetRealDate(nCurDay);
		}
	}
		
	if(bNeedCalcuate)
	{
		for(; nPrevIndx >= 0; --nPrevIndx)
		{
			pOpenPacket->GetData(nPrevIndx, dOpen);
			dOpen += dGap;
			pHighPacket->GetData(nPrevIndx, dHigh);
			dHigh += dGap;
			pLowPacket->GetData(nPrevIndx, dLow);
			dLow += dGap;
			pClosePacket->GetData(nPrevIndx, dClose);
			dClose += dGap;
			
			pOpenPacket->SetData(nPrevIndx, dOpen);
			pHighPacket->SetData(nPrevIndx, dHigh);
			pLowPacket->SetData(nPrevIndx, dLow);
			pClosePacket->SetData(nPrevIndx, dClose);
		}
	}
	//<< 실시간 갭보정 처리

	pTimePacket->Release();
	pOpenPacket->Release();
	pHighPacket->Release();
	pLowPacket->Release();
	pClosePacket->Release();

	if(bNeedCalcuate)
		m_pIChartManager->ChangeAllGraphDataFromGraphInfo(TRUE);
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CGapRevisionAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2009/9/19 - Seung-Won, Bae) Manage OnPacketDataMultiItem Flag of On-GapRevision.
	if( !m_bOnPacketDataMultiItem) CalcGapRevision( lpszRQ, p_bWithReset);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CGapRevisionAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	CalcGapRevisionReal( p_szRQ);
}
