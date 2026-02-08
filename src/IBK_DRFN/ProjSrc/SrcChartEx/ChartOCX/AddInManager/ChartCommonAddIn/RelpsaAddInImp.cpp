// RelpsaAddInImp.cpp: implementation of the CRelpsaAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "RelpsaAddInImp.h"

#include "LoadDrds.h"									// for DrdsData
#include "BlockBaseEnum.h"								// for ERRORVALUE
#include "../../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTEXT()
#include "../Include_AddIn/AddInCommand.h"				// for EAI_DRDS_PACKET_END
#include "../Include_AddIn/I000000/_IPacket.h"			// for IPacket
#include "../Include_Addin/I000000/_IChartManager.h"	// for IChartManager
#include "../Include_Addin/I000000/_IPacketManager.h"	// for IPacketManager
#include "../Include_AddIn/I000000/_IString.h"			// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRelpsaAddInImp::CRelpsaAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.10.15, 배승원) Real을 수신하여 Reset할 시간인지를 확인/처리한다.
	//		OnDrdsPacketEnd()에서는 원하던 Real이었는지 식별할 수 없어 OnDrdsData에서 모든 상황 판단을 끝내 값을 관리한다.
	m_bOnResetReal = FALSE;
}

CRelpsaAddInImp::~CRelpsaAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CRelpsaAddInImp::m_strAddInItemName = "RELPSA";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CRelpsaAddInImp)
	ONADDINEVENT( OnRealData)
	ONADDINEVENT( OnRealPacketEnd)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()

// (2004.10.14, 배승원) 초기화 기준 시각(장시작 시각)과 종목을 관리한다.
void CRelpsaAddInImp::ResetRELPSA( const char *p_szResetMsg)
{
	// 0. 초기화인 경우를 처리한다.
	CString strResetMsg( p_szResetMsg);
	if( strResetMsg.IsEmpty())
	{
		// (2004.10.15, 배승원) 가격 Data 초기화를 위한 Real 수신 환경을 Clear하는 Routine을 제공한다.
		ClearRELPSA();
		return;
	}

	// 1. Format을 확인한다. (HHMMSS:00..00)
	// 1.0 ':'의 위치를 확인한다.
	if( strResetMsg.Find( ':') != 6) return;
	// 1.1 종목 Code의 유무를 확인한다.
	if( strResetMsg.GetLength() <= 7) return;

	// 2. 기준시각과 종목 코드를 관리한다.
	m_dResetTime = atof( strResetMsg.Left( 6));
	m_strResetItemCode = strResetMsg.Mid( 7);

	// (2004.10.15, 배승원) Real을 수신하여 Reset할 시간인지를 확인/처리한다.
	//		OnDrdsPacketEnd()에서는 원하던 Real이었는지 식별할 수 없어 OnDrdsData에서 모든 상황 판단을 끝내 값을 관리한다.
	m_bOnResetReal = FALSE;
}

// (2004.10.15, 배승원) 가격 Data 초기화를 위한 Real 수신 환경을 Clear하는 Routine을 제공한다.
void CRelpsaAddInImp::ClearRELPSA( void)
{
	m_dResetTime = 0;
	m_strResetItemCode.Empty();
	m_bOnResetReal = FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CRelpsaAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2004.10.13, 배승원) 우선 상하한가 보조 Message를 수신할 시점인지 확인한다.
	// (2004.10.22, 배승원) 상한/하한가 표시 경우와 가격 끝일봉 초기화 기능은
	//			모두 가격 차트가 있는 종목의 초기화 TR의 수신시점에 해당한다.
	//		이 기능들은 동양 지존차트의 종목/업종의 가격/비율형 봉차트를 지원하면서,
	//			상/하한가를 표시하거나 전일 종가 표시를 Reset 시키는 기능을 처리하는 것이다.
	//		특히 이 기능들은 차트내 1개의 Graph에만 적용할 수 있는 기능으로
	//			(기능의 Data 구조가 AddIn 당 1개로 국한된 기술적인 문제이다.)
	//			종가 Packet을 이용하는 가격차트에만 제공되는 기능으로 국한한다.
	//		*. 보다 자세한 내용은 각 Class Header 참고
	if( !( BOOL)strstr( p_szPacketNames, CString( _MTEXT( C0_CLOSE)) + ";") || !p_bWithReset) return;

	// (2004.10.14, 배승원) 가격 끝일봉 초기화 시각을 확인한다.
	ILPCSTR szHelpMsg;
	if( m_pIDefaultPacketManager) m_pIDefaultPacketManager->GetHelpMessageItem( "RELPSA", szHelpMsg);
	ResetRELPSA( szHelpMsg);
}

// (2007/2/8 - Seung-Won, Bae) On Real Data
void CRelpsaAddInImp::OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)
{
	// 1. 처리할 조건인지를 확인한다.
	// 1.1 RELPSA의 기준시각이 있는지 확인한다.
	if( m_dResetTime == 0 || m_strResetItemCode.IsEmpty()) return;

	// (2006/11/20 - Seung-Won, Bae) Do not release packet interface.
	//		It is released in OffDrdsAdvise() Time
	//	pIPacket->Release();
	if( strcmp( p_szPacketName, _MTEXT( C0_DATE_TIME))) return;

	// 2. Real의 시각이 Reset할 시간을 경과했는지 확인한다.
	m_bOnResetReal = ( m_dResetTime <= p_dData);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CRelpsaAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	// 0. 먼저 가격 Data를 Reset 시켜야 할 상황인지 확인한다.
	if( !m_bOnResetReal) return;

	// 1. 시/고/저 Data를 종가 Data로 설정한다.
	// 1.0 먼저 종가를 확인한다.
	if( !m_pIDefaultPacketManager) return;
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( !pIPacket) return;
	int nDataIndex = pIPacket->GetDataCount() - 1;	// 0 Base
	if( nDataIndex < 0)
	{
		pIPacket->Release();
		return;
	}
	double dPrice;
	if( !pIPacket->GetData( nDataIndex, dPrice)) return;
	pIPacket->Release();
	// 1.1 시가를 설정한다.
	pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_OPEN));
	if( pIPacket)
	{
		pIPacket->SetData( nDataIndex, dPrice);
		pIPacket->Release();
	}
	// 1.2 고가를 설정한다.
	pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_HIGH));
	if( pIPacket)
	{
		pIPacket->SetData( nDataIndex, dPrice);
		pIPacket->Release();
	}
	// 1.1 저가를 설정한다.
	pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_LOW));
	if( pIPacket)
	{
		pIPacket->SetData( nDataIndex, dPrice);
		pIPacket->Release();
	}

	// 2. 처리가 끝나면, 더 이상의 반복작업이 없도록 RELPSA 기준 Data를 Clear시킨다.
	ClearRELPSA();
}
