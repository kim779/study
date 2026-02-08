// DirectRealReceiverAddInImp.cpp: implementation of the CDirectRealReceiverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "DirectRealReceiverAddInImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_AddIn/I000000/_IPacketListManager.h"	// for IPacketListManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirectRealReceiverAddInImp::CDirectRealReceiverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CRealReceiverAddInImp( p_pIChartOCX, p_pIAddInDllBase)
{
}

CDirectRealReceiverAddInImp::~CDirectRealReceiverAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString			CDirectRealReceiverAddInImp::m_strAddInItemName		= "DIRECT_REAL";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CDirectRealReceiverAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()


BOOL CDirectRealReceiverAddInImp::UpdatePacketData( const char *p_szPacketName, const int p_nIndex, const double p_dData)
{
	return CRealReceiverAddInImp::UpdatePacketData( p_szPacketName, p_nIndex, p_dData);
}

// 복수종목에 리얼처리 : 복수종목 - ojtaso (20070208)
// 차트의 원, %, $의 구분관련 : 복수종목 - ojtaso (20070411)
// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
BOOL CDirectRealReceiverAddInImp::SetRealDataRQ(LPCTSTR lpszRQ, const char *p_szPacketName, double p_dData, BOOL p_bRealData)
{
	return CRealReceiverAddInImp::SetRealDataRQ( lpszRQ, p_szPacketName, p_dData, p_bRealData);
}
BOOL CDirectRealReceiverAddInImp::SetRealPacketEndRQ(LPCTSTR lpszRQ, BOOL p_bRealData)
{
	return CRealReceiverAddInImp::SetRealPacketEndRQ( lpszRQ, p_bRealData);
}


// 현재가 등락률 - ojtaso (20080602)
void CDirectRealReceiverAddInImp::OffsetPacketData(LPCTSTR lpszRQ, double dData)
{
	if(dData == 0.)
		return;
	
//	m_pIPacketListManager->OffsetPacketData(lpszRQ, dData);

	IPacket *pIPacket = m_pIPacketListManager->GetPacket( lpszRQ, _MTEXT( C0_CLOSE));
	if(pIPacket) 
		pIPacket->OffsetPacketData(dData);
}