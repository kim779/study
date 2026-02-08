// RealTimeTransducerImp.cpp: implementation of the CRealTimeTransducerImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "RealTimeTransducerImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for ML_SET_LANGUAGE_RES()
#include "../../Include_Chart/Conversion.h"					// for	CDataConversion

#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IHelpMessageMap.h"		// for IHelpMessageMap
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRealTimeTransducerImp::CRealTimeTransducerImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase),
	m_dlgTimeTransducer( m_nRealTime, m_nAddTime, m_nFakeTime, m_tcTR, m_tcReal)
{
	// (2009/10/23 - Seung-Won, Bae) Manage Time Value
	m_nRealTime	= 0;
	m_nAddTime	= 0;
	m_nFakeTime	= 0;
}

CRealTimeTransducerImp::~CRealTimeTransducerImp()
{
	if( m_dlgTimeTransducer.GetSafeHwnd())
		m_dlgTimeTransducer.DestroyWindow();
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CRealTimeTransducerImp::m_strAddInItemName = "REAL_TIME_TRANSDUCER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CRealTimeTransducerImp)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnRealPacketEnd)
	ONADDINEVENT( OnPreRealData)
	ONADDINEVENT( OnPreRealPacketEnd)
	ONADDINEVENT( OnPreNMTRClock)
	ONADDINEVENT( OnLButtonDblClk)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CRealTimeTransducerImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( strcmp( lpszRQ, "DEFAULT")) return;

	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return;

	ILPCSTR szPacketType = pIPacket->GetType();
	m_tcTR.SetPacketType( szPacketType);

	int nDataCount = pIPacket->GetDataCount();
	m_nRealTime = 0;
	if( 0 < nDataCount)
	{
		double dTime = 0;
		if( pIPacket->GetData( nDataCount - 1, dTime))
			m_nRealTime = m_tcTR.GetFullTypeTime( ( __int64)dTime);
	}


	m_tcReal.SetPacketType( "HHMMSS");
	IHelpMessageMap *pIHelpMessageMap = m_pIDefaultPacketManager->GetHelpMessageMap();
	if( pIHelpMessageMap)
	{
		if( pIHelpMessageMap->Lookup( "REALTIMET", szPacketType))
			m_tcReal.SetPacketType( szPacketType);
		pIHelpMessageMap->Release();
	}


	// Clear Adding Factor for new TR.
	m_nAddTime = 0;
	m_dlgTimeTransducer.UpdateTime( m_pIDefaultPacketManager);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CRealTimeTransducerImp::OnRealPacketEnd( const char *p_szRQ)
{
	if( strcmp( p_szRQ, "DEFAULT") && strcmp( p_szRQ, "CLOCK")) return;
	if( m_dlgTimeTransducer.GetSafeHwnd()) m_dlgTimeTransducer.UpdateReal();
}


// (2008/10/22 - Seung-Won, Bae) for Real Filtering
//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
//	You can change the Real Data Value. it will be set to chart.
BOOL CRealTimeTransducerImp::OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ, BOOL p_bRealData)
{
	if(	m_dlgTimeTransducer.IsClockTimeOnly() && p_bRealData) return FALSE;

	if( strcmp( p_szPacketName, _MTEXT( C0_DATE_TIME))
		|| ( strcmp( p_szRQ, "DEFAULT") && strcmp( p_szRQ, "CLOCK"))) return TRUE;

	double dDataBackup = p_dData;

	m_nRealTime = m_tcReal.GetFullTypeTime( ( __int64)p_dData);

	if( !p_bRealData) m_nRealTime = CDataConversion::CalcTimeSum( m_nRealTime, -m_nAddTime);

	m_dlgTimeTransducer.UpdateTime();

	ASSERT( p_bRealData || dDataBackup == m_nFakeTime);

	p_dData = ( double)m_tcReal.GetPacketTypeTime( m_nFakeTime);

	return TRUE;
}
BOOL CRealTimeTransducerImp::OnPreRealPacketEnd( const char *p_szRQ, BOOL p_bRealData)
{
	if(	m_dlgTimeTransducer.IsClockTimeOnly() && p_bRealData) return FALSE;
	return TRUE;
}

BOOL CRealTimeTransducerImp::OnPreNMTRClock( __int64 &p_nRealClockSecs)
{
	if(	m_dlgTimeTransducer.IsNoClockTime( p_nRealClockSecs)) return FALSE;
	m_dlgTimeTransducer.UpdateClockTime( p_nRealClockSecs);
	return TRUE;
}

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
BOOL CRealTimeTransducerImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !( nFlags & MK_CONTROL) || !( nFlags & MK_SHIFT)) return FALSE;

	if( m_dlgTimeTransducer.GetSafeHwnd()) return FALSE;

	// (2006/2/10 - Seung-Won, Bae) Manage State for CWnd::FromHandle()
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	ML_SET_LANGUAGE_RES();
	m_dlgTimeTransducer.Create( IDD_REAL_TIME_TRANSDUCER, m_pChartCWnd);

	// (2009/10/23 - Seung-Won, Bae) Get final version interface.
	char szMChartMode[ 256];
	ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
	GetPrivateProfileString( "Chart Version", "MChartMode", "", szMChartMode, 255, CString( szDataPath) + "ChartMetaTable.dat");
	WritePrivateProfileString( "Chart Version", "MChartMode", "", CString( szDataPath) + "ChartMetaTable.dat");
	IRealReceiver *pIRealReceiver = ( IRealReceiver *)m_pIAddInToolMgr->GetAddInCustomInterface( "ChartCommonAddIn.dll:DIRECT_REAL");
	WritePrivateProfileString( "Chart Version", "MChartMode", szMChartMode, CString( szDataPath) + "ChartMetaTable.dat");

	m_dlgTimeTransducer.UpdateTime( m_pIDefaultPacketManager, pIRealReceiver);
	if( pIRealReceiver) pIRealReceiver->Release();


	return TRUE;
}
