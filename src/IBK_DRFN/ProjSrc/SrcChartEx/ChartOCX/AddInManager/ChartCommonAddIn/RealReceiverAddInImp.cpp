// RealReceiverAddInImp.cpp: implementation of the CRealReceiverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "RealReceiverAddInImp.h"

#include <Mmsystem.h>										// for timeGetTime()

#include "../../Include/DumpLogger.h"						// for CDumpLogger
#include "../../Include_Chart/Conversion.h"					// for CDataConversion
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_AddIn/I000000/_IStringList.h"			// for IStringList
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IPacketListManager.h"	// for IPacketListManager
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket

#define _DRAW_SKIP_FACTOR		( 1 + 2)	// Draw and 2 Sleep
#define _REAL_DELAY_SKIP		1	// 0 or 1

double			CRealReceiverAddInImp::m_dDrawSkipFactor	= 1 + 2;	// Draw and 2 Sleep
int				CRealReceiverAddInImp::m_nCpuCount			= 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// (2007/3/13 - Seung-Won, Bae) Synchronized Draw
CWinThread *	CRealReceiverAddInImp::m_pWinThread			= NULL;
HANDLE			CRealReceiverAddInImp::m_hRealReceiveEvent	= NULL;
int				CRealReceiverAddInImp::m_nNomalizedMinuteTimerID = -1;

CTypedPtrArray < CPtrArray, CRealReceiverAddInImp *>	CRealReceiverAddInImp::m_aRealReceivers;
CCriticalSection										CRealReceiverAddInImp::m_csRealReceivers;

CRealReceiverAddInImp::CRealReceiverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2009/11/1 - Seung-Won, Bae) for Normalized candle clock.
	m_nRealClockSecs = 0;

	// (2006/7/26 - Seung-Won, Bae) Manage Removed Packet Data Count for Reset Scroll in OnDrdsPacketEnd().
	m_nRemovePacketDataCount = 0;

	// (2007/3/2 - Seung-Won, Bae) Manage Real Receiveing Check to skip the Empty PacketEnd Event.
	m_nRealReceivedType = 0;		// 0:No Real, 1:Time Real, 2:Indexed Real

	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	// Skip same '자료일자'
	m_eScaleDrawType		= CScaleBaseData::HORZ_NOTYPE;
	m_dPrevServerTime			= 0;
	// Status Data
	m_lRealReceiveCount		= 0;
	m_lServerTime			= 0;
	m_lDrawSpeed			= 0;
	// Analysis Real-Delay
	m_bSecondTime				= FALSE;
	m_lServerBaseTime		= 0;
	m_lClientBaseTime1000	= 0;
	m_lRealLoadTime			= 0;
	m_lRealLoadTimeMax		= 0;
	// Real Process Speed
	m_lPrevClientTime1000				= 0;
	m_d5RealReceiveSpeed				= 0;
	m_dRealReceiveSpeedAverage			= 0;
	m_dRealReceiveSpeedMax				= DBL_MAX;
	m_lLastRealCountOnDraw				= 0;
	m_l5RealProcessCountPerDraw			= 0;
	m_l5RealProcessCountPerDrawAverage	= 0;
	m_l5RealProcessCountPerDrawMax		= 0;
	// Multi-Thread
	// (2007/3/13 - Seung-Won, Bae) Synchronized Draw
	m_bDoInvalidate						= FALSE;
	m_csRealReceivers.Lock();
	int i = 0;
	if( !m_hRealReceiveEvent && !m_pWinThread)
	{
		m_aRealReceivers.SetSize( 10, 10);
		for(  i = 0; i <= m_aRealReceivers.GetUpperBound(); i++) m_aRealReceivers[ i] = NULL;
		if( !m_hRealReceiveEvent)
		{
			CString strEventID;
			strEventID.Format( "CRealReceiverAddInImp%08X", this);
			m_hRealReceiveEvent = CreateEvent( NULL, FALSE, FALSE, strEventID);
		}
		if( !m_pWinThread) m_pWinThread = AfxBeginThread( RealUpdateWithClutch, m_hRealReceiveEvent);
	}
	int nUBound = m_aRealReceivers.GetUpperBound();
	for(  i = 0; i <= nUBound; i++) if( NULL == m_aRealReceivers[ i]) break;
	if( i <= nUBound) m_aRealReceivers[ i] = this;
	else m_aRealReceivers.SetAtGrow( nUBound + 1, this);
	m_csRealReceivers.Unlock();

	// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
	ILPCSTR szValue = p_pIChartOCX->GetTestEnvValue( "Real Load Indicator");
	m_bRealLoadIndicator	= atoi( szValue);
	
	szValue = p_pIChartOCX->GetTestEnvValue( "Real Speed Log");
	m_bRealSpeedLog			= atoi( szValue);

	szValue = p_pIChartOCX->GetTestEnvValue( "Real Data Log");
	m_bRealDataLog			= atoi( szValue);

	szValue = p_pIChartOCX->GetTestEnvValue( "Real Not Receive");
	m_bRealNoData			= atoi( szValue);

	// (2007/4/6 - Seung-Won, Bae) Real Time Speed Logging
	m_pDumpLogger = NULL;
	if( m_bRealSpeedLog || m_bRealDataLog) m_pDumpLogger = new CDumpLogger( "DirectRealTimeLog", TRUE);

	// (2008/12/26 - Seung-Won, Bae) Make cache.
	m_bIsRuntimeMode = m_pIChartOCX->IsRuntimeMode();
	m_bInUseRealData = m_bIsRuntimeMode && m_pIChartOCX->IsInUseRealData() && m_pIDefaultPacketManager->IsInUseRealData();

	// 2008.08.25 김진순 허수봉 처리	<<
	/*
	if( !m_pWinTickThread )
	{
		m_pWinTickThread = AfxBeginThread(RealUpdateTickTimer, this);
		TRACE("\n※※※※※※※※ [RealUpdateTickTimer Start] ※※※※※※※※\n");
	}
	*/
	if( m_nNomalizedMinuteTimerID < 0 )
	{
		m_nNomalizedMinuteTimerID = 90001;
		m_nNomalizedMinuteTimerID = SetTimer(NULL, m_nNomalizedMinuteTimerID, 1000, (TIMERPROC)OnNomalizedMinuteTimerProc);		// 1초 주기 Timer 생성
	}
	//>>
}

CRealReceiverAddInImp::~CRealReceiverAddInImp()
{
	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	// (2007/3/13 - Seung-Won, Bae) Synchronized Draw
	BOOL bNone = TRUE;
	m_csRealReceivers.Lock();
	int nUBound = m_aRealReceivers.GetUpperBound();
	for( int i = 0; i <= nUBound; i++)	if( this == m_aRealReceivers[ i]) m_aRealReceivers[ i] = NULL;
	else								if( NULL != m_aRealReceivers[ i]) bNone = FALSE;
	if( bNone)
	{
		if( m_pWinThread)
		{
			TerminateThread( m_pWinThread->m_hThread, 0);
			delete m_pWinThread;
			m_pWinThread = NULL;
		}
		if( m_hRealReceiveEvent)
		{
			CloseHandle( m_hRealReceiveEvent);
			m_hRealReceiveEvent = NULL;
		}
		/*
		if( m_pWinTickThread)// 2008.08.25 김진순 허수봉 처리
		{
			TerminateThread( m_pWinTickThread->m_hThread, 0);
			delete m_pWinTickThread;
			m_pWinTickThread = NULL;
			TRACE("\n※※※※※※※※ [RealUpdateTickTimer Terminate] ※※※※※※※※\n");
		}
		*/
		if( m_nNomalizedMinuteTimerID >= 0 )
		{
			KillTimer( NULL, m_nNomalizedMinuteTimerID );
			m_nNomalizedMinuteTimerID = -1;
		}
	}
	m_csRealReceivers.Unlock();

	// (2007/4/6 - Seung-Won, Bae) Real Time Speed Logging
	if( m_pDumpLogger)
	{
		delete m_pDumpLogger;
		m_pDumpLogger = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
//////////////////////////////////////////////////////////////////////
IUnknown *CRealReceiverAddInImp::GetAddInCustomInterface( void)
{
	IUnknown *pIUnknown = ( IUnknown *)( IRealReceiver *)this;

	// (2009/9/5 - Seung-Won, Bae) Support Old MChart Mode
	char szMChartMode[ 256];
	ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
	DWORD dwError = GetPrivateProfileString( "Chart Version", "MChartMode", "", szMChartMode, 255, CString( szDataPath) + "ChartMetaTable.dat");
	CString strChartMode( szMChartMode);
	if(			strChartMode == "HTS")	pIUnknown = ( IUnknown *)( IOldHTSRealReceiver *)this;
	else if(	strChartMode == "FX")	pIUnknown = ( IUnknown *)( IOldFXRealReceiver *)this;

	pIUnknown->AddRef();
	return pIUnknown;
}


// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CRealReceiverAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	if( !m_bRealLoadIndicator) return;
	if( !p_hDC) return;
	CString strRealLoadMsg;
	int nTime = (int)m_lServerTime;
	strRealLoadMsg.Format(	"%dth Real Received in %02d:%02d:%02d.\r\n"
							"Draw Speed %dms.\r\n"
							"Real Delay %dsec (max %dsec).\r\n"
							"Real Speed %0.2f count/sec (max %0.2f count/sec).\r\n"
							"Draw Skip Count %d (max %d).",						m_lRealReceiveCount, nTime / 10000, ( nTime / 100) % 100, nTime % 100,
																				m_lDrawSpeed,
																				m_lRealLoadTime, m_lRealLoadTimeMax,
																				( 0 != m_dRealReceiveSpeedAverage ? 1000 / m_dRealReceiveSpeedAverage : 0), ( DBL_MAX != m_dRealReceiveSpeedMax ? 1000 / m_dRealReceiveSpeedMax : 0),
																				m_l5RealProcessCountPerDrawAverage, m_l5RealProcessCountPerDrawMax);
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	pDC->DrawText( strRealLoadMsg, CRect( 0, 0, 100, 100), DT_LEFT | DT_NOCLIP | DT_TOP);
}


// [04/10/13] TR의 수신을 AddIn DLL들에게 알려 대응토록 한다.
void CRealReceiverAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// Skip same '자료일자'
	m_eScaleDrawType = m_pIDefaultPacketManager->GetBaseTimeDataType();
	// Status Data
	m_lRealReceiveCount = 0;
	m_lServerTime		= 0;
	// Analysis Real-Delay
	m_bSecondTime				= FALSE;
	m_lServerBaseTime		= 0;
	m_lClientBaseTime1000	= 0;
	m_lRealLoadTime			= 0;
	m_lRealLoadTimeMax		= 0;
	// Real Process Speed
	m_lPrevClientTime1000				= 0;
	m_d5RealReceiveSpeed				= 0;
	m_dRealReceiveSpeedAverage			= 0;
	m_dRealReceiveSpeedMax				= DBL_MAX;
	m_lLastRealCountOnDraw				= 0;
	m_l5RealProcessCountPerDraw			= 0;
	m_l5RealProcessCountPerDrawAverage	= 0;
	m_l5RealProcessCountPerDrawMax		= 0;
}


BOOL CRealReceiverAddInImp::UpdatePacketData( const char *p_szPacketName, const int p_nIndex, const double p_dData)
{
	if( !p_szPacketName) return FALSE;
	if( !*p_szPacketName) return FALSE;

	if( m_bRealNoData) return FALSE;

	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	double dData = p_dData;
	if( !m_pIAddInToolMgr->OnPreIndexedRealData( p_szPacketName, p_nIndex, dData)) return FALSE;

	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( p_szPacketName);
	if( pIPacket)
	{
		BOOL bResult = FALSE;
		if( 0 <= p_nIndex && p_nIndex < pIPacket->GetDataCount())
		{
			bResult = pIPacket->SetData( p_nIndex, p_dData);

			// (2007/3/2 - Seung-Won, Bae) Manage Real Receiveing Check to skip the Empty PacketEnd Event.
			m_nRealReceivedType = 2;		// 0:No Real, 1:Time Real, 2:Indexed Real

			// (2007/2/8 - Seung-Won, Bae) Notify to AddIn
			m_pIAddInToolMgr->OnIndexedRealData( p_szPacketName, p_nIndex, p_dData);
		}
		pIPacket->Release();
		return bResult;
	}

	return FALSE;
}


// 복수종목에 리얼처리 : 복수종목 - ojtaso (20070208)
// 차트의 원, %, $의 구분관련 : 복수종목 - ojtaso (20070411)
// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
BOOL CRealReceiverAddInImp::SetRealDataRQ( LPCTSTR lpszRQ, const char *p_szPacketName, double p_dData, BOOL p_bRealData)
{
	if( m_bRealNoData || !strcmp( lpszRQ, "CLOCK")) return FALSE;

	if(g_pDRDebuger && g_pDRDebuger->IsLoaded())
	{
		CString szTmpReal;
		szTmpReal.Format("%s(%f)", p_szPacketName, p_dData);
		m_szDebugRealData += szTmpReal;
	}
	
	// (2007/4/6 - Seung-Won, Bae) Real Time Speed Logging
	if( m_pDumpLogger && m_bRealDataLog)
	{
		CString strLogData;
		strLogData.Format( "[%s:%s] : [%f]\r\n", lpszRQ, p_szPacketName, p_dData);
		m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
	}

	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	BOOL bDateTimePacket = !strcmp( p_szPacketName, _MTEXT( C0_DATE_TIME));
	if( bDateTimePacket)
	{
		//<<20100525_JS.Kim 솔로몬의 경우 이 로직이 필요없으므로 막는다.
		// 이 코드 풀면 무한루프 발생하는 경우 있음.
		//// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
		//if( CChartInfo::WORLD_ON == m_eChartMode)
		//	m_pIPacketListManager->GetRealData_RealDateTime( lpszRQ, p_dData);
		//else
		//>>
		// (2009/12/2 - Seung-Won, Bae) for 31 Hour.
		if( CChartInfo::KRX_CME == m_eChartMode && 240000 <= p_dData)
			p_dData -= 240000;

		// Status Data
		// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
		//m_lServerTime = ( long)p_dData;
		m_lServerTime = ( __int64)p_dData;
		// 2011.01.06 by SYS <<
		m_bInUseRealData = m_bIsRuntimeMode && m_pIChartOCX->IsInUseRealData() && m_pIDefaultPacketManager->IsInUseRealData();
	}
	
	if( !m_bInUseRealData) return FALSE;

	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	if( !m_pIAddInToolMgr->OnPreRealData( p_szPacketName, p_dData, lpszRQ, p_bRealData)) return FALSE;

	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	m_pIChartOCX->LockWindowUpdate();

	//sy 2004.07.16. packet 이 삭제된 만큼 scroll 변경
	int nRemovePacketDataCount = 0;

	BOOL bResult = m_pIPacketListManager->SetRealData( lpszRQ, p_szPacketName, p_dData, nRemovePacketDataCount);

	//sy 2004.07.26 packet 이 삭제된 만큼 scroll 변경
	if( nRemovePacketDataCount) m_nRemovePacketDataCount = nRemovePacketDataCount;

	// (2007/3/2 - Seung-Won, Bae) Manage Real Receiveing Check to skip the Empty PacketEnd Event.
	m_nRealReceivedType = 1;		// 0:No Real, 1:Time Real, 2:Indexed Real

	// (2007/2/8 - Seung-Won, Bae) Notify to AddIn
	m_pIAddInToolMgr->OnRealData( p_szPacketName, p_dData, lpszRQ);

	return bResult;
}


// 복수종목에 리얼처리 : 복수종목 - ojtaso (20070208)
// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
BOOL CRealReceiverAddInImp::SetRealPacketEndRQ( LPCTSTR lpszRQ, BOOL p_bRealData)
{
	//>>[alzioyes:지우지말것,DRFN Platform 로그기능추가]
	if(!g_pDRDebuger) gfnGetDRDebuger();
	if(g_pDRDebuger && g_pDRDebuger->IsLoaded())
	{
		CString szDebug;
//		szDebug.Format("[DRCS] CRealReceiverAddInImp::SetRealPacketEndRQ [%s][%s]\n", lpszRQ, m_szDebugRealData);
//		OutputDebugString(szDebug);
//
		CString szRealKey;szRealKey.Format("C:%s:%08X", lpszRQ, this);
		//g_pDRDebuger->DRLog_RealData(szRealKey, (LPBYTE)(LPCTSTR)m_szDebugRealData, m_szDebugRealData.GetLength());
		m_szDebugRealData.Empty();
	}
	//<<

	// (2007/4/6 - Seung-Won, Bae) Real Time Speed Logging
	if( m_pDumpLogger && m_bRealDataLog)
	{
		LPCTSTR szItemCode = NULL, szItemName = NULL;
		m_pIPacketListManager->GetItemCode( lpszRQ, szItemCode, szItemName);
		CString strLogData;
		strLogData.Format( "RealPacketEnd : [%s:%s]\r\n", lpszRQ, szItemCode);
		m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
	}

	if( m_bRealNoData || !strcmp( lpszRQ, "CLOCK")) return FALSE;
#ifdef _DEBUG
//	m_bRealNoData = TRUE;
#endif

	if( !m_bInUseRealData || 0 == m_nRealReceivedType) return FALSE;

	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	if( !m_pIAddInToolMgr->OnPreRealPacketEnd( lpszRQ, p_bRealData)) return FALSE;

	// (2006/7/26 - Seung-Won, Bae) Manage Removed Packet Data Count for Reset Scroll in OnDrdsPacketEnd().
	if( m_nRemovePacketDataCount)
	{
		// (2006/6/5 - Seung-Won, Bae) Notify to ST AddIn for Packet Data Cutting.
		m_pIAddInToolMgr->OnPacketTruncated( m_nRemovePacketDataCount);
		// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
		m_pIChartManager->SetFlagForCalcDRIandScrollByReal( TRUE);
		m_nRemovePacketDataCount = 0;

		//sy 2003.3.7 - 속도 개선 - 실시간 들어올때 실시간 data 만 계산
		m_pIChartManager->CalculateAllGraphs( CCalculateBaseData::TRDATA_CAL);
	}
	else
	{
		//sy 2003.3.7 - 속도 개선 - 실시간 들어올때 실시간 data 만 계산
		m_pIChartManager->CalculateAllGraphs( CCalculateBaseData::REALDATA_CAL);
	}

	// (2004.12.07, 배승원) Real의 반영에 따른 지표 계산을 싯점을 AddIn Module에게 알린다.
	m_pIAddInToolMgr->OnRealPacketEnd( lpszRQ);

	m_pIChartManager->RealDataReceived( lpszRQ, 2 == m_nRealReceivedType);
	// (2007/3/2 - Seung-Won, Bae) Manage Real Receiveing Check to skip the Empty PacketEnd Event.
	m_nRealReceivedType = 0;

	GetRealMonitorDataInPacketEnd();		

	if( m_hRealReceiveEvent && m_pWinThread)
	{
		m_bDoInvalidate = TRUE;

		// (2009/3/11 - Seung-Won, Bae) for UI re-action on high speed real situation.
		m_pIChartOCX->UnlockWindowUpdate();

		SetEvent( m_hRealReceiveEvent);
	}
	else InvalidateControl();

	return 0L;
}


void CRealReceiverAddInImp::GetRealMonitorDataInPacketEnd( void)
{
	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	// Status Data
	m_lRealReceiveCount++;
	// Analysis Real-Delay
	if( !m_bSecondTime) if( m_lServerTime % 10) m_bSecondTime = TRUE;		// (2007/3/5 - Seung-Won, Bae) 10 Sec. Indicator Base Time Update.
	long lClientTime1000 = timeGetTime();
	if( 0 == m_lServerBaseTime || 0 == m_lClientBaseTime1000)
	{
		m_lServerBaseTime		= m_lServerTime;
		m_lClientBaseTime1000	= lClientTime1000;
	}
	long lServerInterval	= m_lServerTime		- m_lServerBaseTime;
	long lClientInterval	= ( lClientTime1000	- m_lClientBaseTime1000) / 1000;
	if( lServerInterval < lClientInterval)
	{
		m_lRealLoadTime = lClientInterval - lServerInterval;

		// (2007/3/5 - Seung-Won, Bae) 10 Sec. Indicator Base Time Update.
		if( !m_bSecondTime)
		{
			if( 20 < m_lRealLoadTime) m_lRealLoadTime -= 20;
			else m_lRealLoadTime = 0;
		}
		else
		{
			if( 2 < m_lRealLoadTime) m_lRealLoadTime -= 2;
			else m_lRealLoadTime = 0;
		}
	}
	else if( m_bSecondTime) m_lClientBaseTime1000 -= ( lServerInterval - lClientInterval) * 1000;
	if( m_lRealLoadTimeMax < m_lRealLoadTime) m_lRealLoadTimeMax = m_lRealLoadTime;
	// Real Process Speed
	m_d5RealReceiveSpeed -= m_dRealReceiveSpeedAverage;
	if( 0 != m_lPrevClientTime1000) m_d5RealReceiveSpeed += lClientTime1000 - m_lPrevClientTime1000;
	m_dRealReceiveSpeedAverage = m_d5RealReceiveSpeed / 5;
	if( 5 < m_lRealReceiveCount && m_dRealReceiveSpeedAverage < m_dRealReceiveSpeedMax) m_dRealReceiveSpeedMax = m_dRealReceiveSpeedAverage;
	m_l5RealProcessCountPerDraw -= m_l5RealProcessCountPerDrawAverage;
	if( 0 != m_lLastRealCountOnDraw) m_l5RealProcessCountPerDraw +=  m_lRealReceiveCount - m_lLastRealCountOnDraw;
	m_l5RealProcessCountPerDrawAverage = m_l5RealProcessCountPerDraw / 5;
	if( m_l5RealProcessCountPerDrawMax < m_l5RealProcessCountPerDrawAverage) m_l5RealProcessCountPerDrawMax = m_l5RealProcessCountPerDrawAverage;
	m_lPrevClientTime1000 = lClientTime1000;

	// (2007/4/6 - Seung-Won, Bae) Real Time Speed Logging
	if( m_pDumpLogger && m_bRealSpeedLog)
	{
		CString strLogData;
		strLogData.Format(	"Base Time : S[%d], C[%d]\r\n\t"			// m_lServerBaseTime, m_lClientBaseTime1000
							"Current Time : S[%d], C[%d]\r\n\t"			// m_lServerTime, lClientTime1000
							"Interval : S[%d], C[%d]\r\n\t"				// lServerInterval, lClientInterval * 1000
							"Delay Time : [%d]\r\n",					// m_lRealLoadTime * 1000
							m_lServerBaseTime, m_lClientBaseTime1000,
							m_lServerTime, lClientTime1000,
							lServerInterval, lClientInterval * 1000,
							m_lRealLoadTime * 1000);
		m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
	}
}


//////////////////////////////////////////////////////////////////////
// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
//////////////////////////////////////////////////////////////////////
// 	UINT CRealReceiverAddInImp::RealUpdateWithClutch( LPVOID pParam)
// 	{
// 		long lLoadTime = 0;
// 		do
// 		{
// 			WaitForSingleObject( ( HANDLE)pParam, INFINITE);
// 			lLoadTime = CRealReceiverAddInImp::GetMaxLoadTime();
// 			CRealReceiverAddInImp::InvalidateControls();
// 	#ifdef _DEBUG	// use min for debugging.
// 			if( 1000 < lLoadTime) lLoadTime = 1000;
// 	#endif
// 	// (2009/1/29 - Seung-Won, Bae) for 92000 TEST
// 			if( 0 < lLoadTime) Sleep( lLoadTime);
// 		} while( TRUE);
// 
// 		return 0;
// 	}
UINT CRealReceiverAddInImp::RealUpdateWithClutch( LPVOID pParam)
{
	long lLoadTime = 0;
	do
	{
		WaitForSingleObject( ( HANDLE)pParam, INFINITE);
		lLoadTime = CRealReceiverAddInImp::InvalidateControls();
		if( lLoadTime <= 49) lLoadTime = 50;
		// use min for debugging.
		if( 5000 < lLoadTime) lLoadTime = 5000;
		if( 0 < lLoadTime) Sleep( lLoadTime);
	} while( TRUE);

	return 0;
}

// ******************** Caution : On Multi-Thread ********************
// 	void CRealReceiverAddInImp::InvalidateControl( void)
// 	{
// 		if( m_pIChartOCX && !m_bOcxDestroyed && m_bDoInvalidate)
// 		{
// 			m_pIChartOCX->UnlockWindowUpdate();
// 			// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
// 			m_pIChartManager->SetFlagForCalcDRIandScrollByReal();
// 			m_pIChartOCX->InvalidateControl(); // m_pChartCWnd->RedrawWindow( NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE | RDW_NOFRAME | RDW_VALIDATE | RDW_NOCHILDREN);
// 			// Real Process Speed
// 			m_lLastRealCountOnDraw = m_lRealReceiveCount;
// 			m_bDoInvalidate = FALSE;
// 		}
// 	}
long CRealReceiverAddInImp::InvalidateControl( void)
{
	long lDrawSpeed = 0;
	if( m_pIChartOCX && m_bDoInvalidate)
	{
		m_pIChartOCX->UnlockWindowUpdate();
		// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
		m_pIChartManager->SetFlagForCalcDRIandScrollByReal();
		m_lDrawSpeed = m_pIChartOCX->InvalidateControl(); // m_pChartCWnd->RedrawWindow( NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE | RDW_NOFRAME | RDW_VALIDATE | RDW_NOCHILDREN);
		// Real Process Speed
		m_lLastRealCountOnDraw = m_lRealReceiveCount;
		m_bDoInvalidate = FALSE;
	}
	return long( m_lDrawSpeed * m_dDrawSkipFactor);
}
// ******************** Caution : On Multi-Thread ********************
// (2007/4/6 - Seung-Won, Bae) Use Min Load Time for Other Map Effect.
// 	BOOL CRealReceiverAddInImp::GetLoadTime( long &p_lLoadTime, long &p_lDelayTime)
// 	{
// 		if( !m_pIChartOCX || m_bOcxDestroyed || !m_bDoInvalidate) return FALSE;
// 		// Status Data
// 		m_lDrawSpeed = m_pIChartOCX->GetDrawingLoadTime();
// 		p_lLoadTime = m_lDrawSpeed * _DRAW_SKIP_FACTOR;
// 		p_lDelayTime = m_lRealLoadTime * 1000 * _REAL_DELAY_SKIP;
// 		return TRUE;
// 	}
// (2007/3/13 - Seung-Won, Bae) Synchronized Draw
// ******************** Caution : On Multi-Thread ********************
// 	void CRealReceiverAddInImp::InvalidateControls( void)
// 	{
// 		m_csRealReceivers.Lock();
// 		int nUBound = m_aRealReceivers.GetUpperBound();
// 		for( int i = 0; i <= nUBound; i++) if( m_aRealReceivers[ i]) m_aRealReceivers[ i]->InvalidateControl();
// 		m_csRealReceivers.Unlock();
// 	}
long CRealReceiverAddInImp::InvalidateControls( void)
{
	long lFullLoadTime = 0;
	CRealReceiverAddInImp *pRealReceiverAddInImp = NULL;
	m_csRealReceivers.Lock();
	int nUBound = m_aRealReceivers.GetUpperBound();
	for( int i = 0; i <= nUBound; i++)
	{
		pRealReceiverAddInImp = m_aRealReceivers[ i];
		if( !pRealReceiverAddInImp) continue;
		lFullLoadTime += pRealReceiverAddInImp->InvalidateControl();
	}
	m_csRealReceivers.Unlock();
	return lFullLoadTime;
}
// ******************** Caution : On Multi-Thread ********************
// 	long CRealReceiverAddInImp::GetMaxLoadTime( void)
// 	{
// 		m_csRealReceivers.Lock();
// 		// (2007/4/6 - Seung-Won, Bae) Use Min Delay Time for Other Map Effect.
// 		// (2008/12/23 - Seung-Won, Bae) Add load to keep the Skip Time for Max Load.
// 		long lLoadTime = 0, lDelayTime = 0, lMaxLoadTime = 0, lMinDelayTime = LONG_MAX, lFullLoadTime = 0;
// 		int nUBound = m_aRealReceivers.GetUpperBound();
// 		for( int i = 0; i <= nUBound; i++) if( m_aRealReceivers[ i])
// 		{
// 			if( !m_aRealReceivers[ i]->GetLoadTime( lLoadTime, lDelayTime)) continue;
// 			if( lMaxLoadTime < lLoadTime) lMaxLoadTime = lLoadTime;
// 			lFullLoadTime += lLoadTime;
// 			if( lDelayTime < lMinDelayTime) lMinDelayTime = lDelayTime;
// 		}
// 		m_csRealReceivers.Unlock();
// 		// (2007/4/9 - Seung-Won, Bae) Do not Support Delay Time for Not Correctly Joined Time.
// 		return lFullLoadTime; //  + ( lMinDelayTime == LONG_MAX ? 0 : lMinDelayTime);
// 	}

void CALLBACK CRealReceiverAddInImp::OnNomalizedMinuteTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	m_csRealReceivers.Lock();

	int nUBound = m_aRealReceivers.GetUpperBound();
	for( int i = 0; i <= nUBound; i++) if( m_aRealReceivers[ i])
		m_aRealReceivers[ i]->SetUpdateTick();

	m_csRealReceivers.Unlock();
}

void CRealReceiverAddInImp::SetUpdateTick( void)
{
	m_nRealClockSecs++;
	if( !m_pIAddInToolMgr->OnPreNMTRClock( m_nRealClockSecs))	return;
	m_pIPacketListManager->OnNMTRClock( m_nRealClockSecs, ( IRealReceiver *)this);
}
