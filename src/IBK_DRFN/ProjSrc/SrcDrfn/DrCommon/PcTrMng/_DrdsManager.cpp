#include "stdafx.h"
#include "PcTrMng.h"
#include <math.h>

#include "../../inc/RealUpdate.h"

//@신영제거#include "../../../../Common/CommTranInfo.h"
//@신영제거#include "../../../../Common/CommDef.h"

long __declspec(dllexport) CommGetRealTranMapPtr();

/////////////////////////////////////////////////////////////////////////////
// CAUDrdsManager
CAUDrdsManager::CAUDrdsManager()
{
}

CAUDrdsManager::~CAUDrdsManager()
{
}

IDrdsLib* CAUDrdsManager::AdviseDrdsLib()
{
	CDrdsLib* pDrds = new CDrdsLib;
	
	return pDrds;
}

void CAUDrdsManager::UnadviseDrdsLib(IDrdsLib* pDrds)
{
	if(pDrds) 
	{
		CDrdsLib* pTDrds = (CDrdsLib*)pDrds;
		delete pTDrds;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDrdsLib
CDrdsLib::CDrdsLib()
{
	m_hHandle = NULL;
	m_pDrds = NULL;

//@Solomon090819	m_pCommApiHelper = new CCommApiHelper;
//@Solomon090819	m_pCommApiHelper->LoadCommHelper();
//@Solomon090819	m_nConnectID = m_pCommApiHelper->CommGetConnectionID(CONNECT_ID_RQ);
}

CDrdsLib::~CDrdsLib()
{
}

void* CDrdsLib::DrdsInit(HWND hWnd)
{
	m_hHandle = hWnd;
	return NULL;
}

BOOL CDrdsLib::DrdsTerm()
{
	return TRUE;
}

BOOL CDrdsLib::DrdsConnect(LPSTR szDataName)
{
	return TRUE;
}

BOOL CDrdsLib::DrdsClose(LPSTR szDataName)
{
	//return m_DrdsDLL.DrdsClose(m_pDrds, m_hHandle, szDataName);
	return TRUE;
}

BOOL CDrdsLib::DrdsAdvise(UINT id, LPSTR szDataName, LPSTR FieldName, LPSTR KeyName)
{
//@신영제거
//	BYTE pPBID[2] = { 0x32, 0 };
//	pPBID[0] = id;
//	
//	CommSetBrod( m_hHandle, pPBID, 1, 'U',  (BYTE *) KeyName );
//@Solomon090819	m_pCommApiHelper->CommSB(m_hHandle, szDataName, KeyName, strlen(KeyName), 1, m_nConnectID);

	{
		int nSize = sizeof(STIDM2Info_RA) + strlen(KeyName)-sizeof(char);
		LPBYTE pInfoBuf = new BYTE[nSize+1];
		STIDM2Info_RA* pRA = (STIDM2Info_RA*)pInfoBuf;
		ZeroMemory(pRA, nSize);
		
		pRA->nInfoSize = nSize;
		sprintf(pRA->aKey, "%08X", this);
		memcpy(pRA->aDataName, szDataName, strlen(szDataName));
		memcpy(pRA->aFieldName, FieldName, strlen(FieldName));
		pRA->nKeyNameSize = strlen(KeyName);
		memcpy(pRA->aKeyName, KeyName, pRA->nKeyNameSize);
		
		//theApp.m_DebugMng.TraceEx(IDM2_TAG_RA, nSize, pInfoBuf);
		theApp.m_DebugMng.TraceEx(IDM2_TAG_RA, '*', szDataName, nSize, pInfoBuf);
		
		delete[] pInfoBuf;
	}

	return TRUE;
}

BOOL CDrdsLib::DrdsUnadvise(UINT id, LPSTR szDataName, LPSTR FieldName, LPSTR KeyName)
{
//@신영제거
//	BYTE pPBID[2] = { 0x32, 0 };
//	pPBID[0] = id;
//
//	CommRemoveBrod( m_hHandle, pPBID, 1, 'U',  (BYTE *) KeyName );
//@Solomon090819	m_pCommApiHelper->CommSBC(m_hHandle, szDataName, KeyName, strlen(KeyName), 1, m_nConnectID);

	{
		int nSize = sizeof(STIDM2Info_RA) + strlen(KeyName)-sizeof(char);
		LPBYTE pInfoBuf = new BYTE[nSize+1];
		STIDM2Info_RA* pRA = (STIDM2Info_RA*)pInfoBuf;
		ZeroMemory(pRA, nSize);
		
		pRA->nInfoSize = nSize;
		sprintf(pRA->aKey, "%08X", this);
		memcpy(pRA->aDataName, szDataName, strlen(szDataName));
		memcpy(pRA->aFieldName, FieldName, strlen(FieldName));
		pRA->nKeyNameSize = strlen(KeyName);
		memcpy(pRA->aKeyName, KeyName, pRA->nKeyNameSize);
		
		//theApp.m_DebugMng.TraceEx(IDM2_TAG_RU, nSize, pInfoBuf);
		theApp.m_DebugMng.TraceEx(IDM2_TAG_RU, '*', szDataName, nSize, pInfoBuf);
		
		delete[] pInfoBuf;
	}

	return TRUE;
}

BOOL CDrdsLib::DrdsSetSocketHandle(HWND hWnd)
{
	return 1;
}

void CDrdsLib::DrdsSendCode()
{
}

BOOL CDrdsLib::AdviseRealCodeData(BOOL bIsAdd, int nCount, LPSTR sPacketName, LPSTR sCodeList)
{
	return 1;
}

int CDrdsLib::SetNewRealData(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType) 
{
	return 1;
}

BOOL CDrdsLib::DrdsAdviseEx(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName)
{
	return 1;
}

BOOL CDrdsLib::DrdsUnadviseEx(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName)
{
	return 1;
}

BOOL CDrdsLib::DrdsRequest(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
{
	return 1;
}

void CDrdsLib::DrdsPropertySet()
{
}

void CDrdsLib::DrdsTraceOn()
{
}

void CDrdsLib::DrdsTraceOff()
{
}

void CDrdsLib::DrdsSetFlag(bool bTrace)
{
}

void CDrdsLib::UDPCreate()
{
}

void CDrdsLib::UDPCreate2(LPSTR strIP, LPSTR strPort)
{
}

void CDrdsLib::UDPDestroy()
{
}

void CDrdsLib::UdpCreate3(LPSTR sCfgFullPathName)
{
}

int  CDrdsLib::SetUserDefData(LPSTR sKeyName, LPSTR sData)
{
	return 1;
}

BOOL CDrdsLib::Drds_OpenSignalMng(LPSTR sCfgFullPathName,long lMainHwnd)
{
	return 1;
}

int  CDrdsLib::Drds_SignalMng_SetNewRealData(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType)
{
	return 1;
}

long CDrdsLib::Drds_GetInterface(UINT nID)
{
	return 1;
}

BOOL CDrdsLib::Drds_SetInterface(UINT nID, long dwData)
{
	return 1;
}

int CDrdsLib::ConvertToRealStruct(WPARAM wParam, LPARAM lParam, char* pStruct, int nRealType)
{
	return 0;
}

// 처리루틴은 Controls\RunChart\ChartCtl.cpp 에서 CChartCtl::GetRealData(..)참고.
BOOL CDrdsLib::GetRealStructData(WPARAM wParam, LPARAM lParam, char*& prValue, int &nRealType)
{
//@신영수정
//	LPREALDATA lpRealData = (LPREALDATA)lParam;
//	LPSTR lpRpData  = lpRealData->pDataBuf;						// 실제데이터 
//	prValue = lpRealData->pDataBuf;
//	nRealType = (int)lpRealData->chRealType;
	return TRUE;
}