// TimeRealDataModule.cpp: implementation of the CTimeRealDataModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeRealDataModule.h"

#include "../Include_Chart/DLL_Load/PacketBase.h"			// for CPacket
#include "../Include_Chart/DLL_Load/IMetaTable.h"			// for g_iMetaTable

#include "Conversion.h"
#include "PacketBaseData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CTimeRealDataModuleBase

// (2009/10/28 - Seung-Won, Bae) Real 시각 처리 Logic Class의 최상위 Base
// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
CTimeRealDataModuleBase::CTimeRealDataModuleBase( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
	: m_marketTimeData( p_marketTimeData)
{
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_eDateUnitType = eDateUnitType;

	// (2009/10/21 - Seung-Won, Bae) Comment. 자료일자 Packet의 Data Type을 관리한다. (ex : "YYYYMMDDHHMMSS")
	m_ttcTrPacket.SetPacketType( strPacketType);
	
	// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
	m_nClockedDataCount = 0;

	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
	m_nRealTimeDiff = 0;
	m_nNewRangeStartTime = 0;

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	m_nChartModeSetting = CChartInfo::HTS;

	// (2009/11/4 - Seung-Won, Bae) Empty Candle Closing Time Type.
	m_bCloseEmptyCandleWhenEnd = TRUE;
}
CTimeRealDataModuleBase::~CTimeRealDataModuleBase()
{
}

void CTimeRealDataModuleBase::OnInitTR( const __int64 &p_nRealTimeDiff, const int &p_nNewRangeStartTime, const CString &p_szMarketDayInfo)
{
	m_nRealTimeDiff = p_nRealTimeDiff;
	m_nNewRangeStartTime = p_nNewRangeStartTime;

	// (2009/11/16 - Seung-Won, Bae) Check for Market Days
	m_mapAddedMarketDays.RemoveAll();
	m_mapRemovedMarketDays.RemoveAll();
	if( !p_szMarketDayInfo.IsEmpty())
	{
		CString strBuffer;
		char *szBuffer = strBuffer.GetBufferSetLength( p_szMarketDayInfo.GetLength());
		strcpy( szBuffer, p_szMarketDayInfo);
		char *szNext = szBuffer;

		do
		{
			szNext = strchr( szNext, ',');
			if( szNext) *szNext = '\0';
			long lDay = atol( szBuffer);
			if( lDay < 0) m_mapRemovedMarketDays.SetAt( ( void *)( -lDay), NULL);
			if( 0 < lDay) m_mapAddedMarketDays.SetAt( ( void *)lDay, NULL);
			if( szNext)
			{
				szNext++;
				szBuffer = szNext;
			}
		} while( szNext);
	}
}

// (2009/11/4 - Seung-Won, Bae) Calculate Market Date.
void CTimeRealDataModuleBase::GetMarketDate_DateTime( __int64 &p_nData) const
{
	if( !m_marketTimeData.IsReverseTime()) return;

	// X ScaleManager의 다른 시간대의 Scale 처리를 위하여 장시작/마감시간을 조회한다.
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime)) return;

	//<<20100511_JS.Kim 솔로몬. 솔로몬은 데이타 시간이 거래소 시간 기준이 아닌 한국기준이다
	// 원래 코드대로 하면 일봉에서 실시간 들어오면 내일 날자로 봉이 하나 생긴다...
	//if( CChartInfo::WORLD_ON == m_nChartModeSetting && nMarketStartTime <= p_nData % 1000000)
	if( CChartInfo::WORLD_ON == m_nChartModeSetting && nMarketStartTime > p_nData % 1000000)
		p_nData = CDataConversion::CalcTimeSum( p_nData, -1000000);
	else if( ( CChartInfo::FX == m_nChartModeSetting || CChartInfo::KRX_CME == m_nChartModeSetting) && p_nData % 1000000 < nMarketStartTime)
		p_nData = CDataConversion::CalcTimeSum( p_nData, -1000000);
}

// (2009/11/16 - Seung-Won, Bae) Check for Market Days
BOOL CTimeRealDataModuleBase::IsMarketDay( int p_nDate)
{
	tm tmMarket = { 0, 0, 0, 0, 0, 0, 0, 0, -1};	//[A00000554]alzioyes:20110530 시간셋팅 초기값변경.
	tmMarket.tm_mday = int( p_nDate % 100);
	int nMonth = int( p_nDate / 100);
	tmMarket.tm_mon = nMonth % 100 - 1;
	tmMarket.tm_year = nMonth / 100 - 1900;
	mktime( &tmMarket);
	void *pTemp = NULL;
	if( 0 == tmMarket.tm_wday || 6 == tmMarket.tm_wday)
	{
		if( !m_mapAddedMarketDays.Lookup( ( void *)p_nDate, pTemp)) return FALSE;
	}
	else
	{
		if( m_mapRemovedMarketDays.Lookup( ( void *)p_nDate, pTemp)) return FALSE;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// class CTickRealDataModule

// (2009/10/28 - Seung-Won, Bae) Tick 처리 Logic Class
// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
CTickRealDataModule::CTickRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const int nAddTimeInterval, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
	: CTimeRealDataModuleBase( p_marketTimeData, strPacketType, eDateUnitType)
{
	// (2009/10/21 - Seung-Won, Bae) Comment. N틱 봉의 단위 틱을 관리한다. (UT값)
	m_nTimeInterval = nTimeInterval;	// 분간격
	if( m_nTimeInterval < 1) m_nTimeInterval = 1;

	// (2009/10/20 - Seung-Won, Bae) N틱을 위한 마지막봉에 들어간 Tick의 개수 (보조메시지 UTEC에 의해 지정됨)
	m_nAddTimeInterval = nAddTimeInterval;
}
CTickRealDataModule::~CTickRealDataModule()
{
}

// (2009/10/22 - Seung-Won, Bae) Comment. 수신된 실시간의 올림/버림 값을 구하여 각 Logic Class에 따라 Packet에 반영 처리한다.
int CTickRealDataModule::SetRealData( CPacket* pPacket, const __int64 &nRealTime)
{
	// 3.3.3. N틱을 위한 Counter를 증가시킨다.
	//	마지막 봉에 들어간 틱의 개수로 m_nTimeInterval이 차면 다음은 새로운 봉을 구성토록 한다.
	m_nAddTimeInterval++;
	if( m_nTimeInterval < m_nAddTimeInterval)
		m_nAddTimeInterval = 1;

	// (2009/11/3 - Seung-Won, Bae) User Time.
	__int64 nUserTime = nRealTime;
	if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

	if( m_nTimeInterval <= 1 || m_nAddTimeInterval <= 1)
			pPacket->AppendData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));
	else	pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));

	return 0;
}


//////////////////////////////////////////////////////////////////////
// class CDateRealDataModule

// (2009/10/28 - Seung-Won, Bae) Date(일/월) 처리 Logic Class
// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
CDateRealDataModule::CDateRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType, BOOL p_bMonthChart)
	: CTimeRealDataModuleBase( p_marketTimeData, strPacketType, eDateUnitType)
{
	m_bMonthChart = p_bMonthChart;
}
CDateRealDataModule::~CDateRealDataModule()
{
}

// (2009/10/22 - Seung-Won, Bae) Comment. 수신된 실시간의 올림/버림 값을 구하여 각 Logic Class에 따라 Packet에 반영 처리한다.
int CDateRealDataModule::SetRealData( CPacket* pPacket, const __int64 &nRealTime)
{
	if( !pPacket) return -1;
	const CList< double, double> *pDateList = pPacket->GetnumericDataList();
	if( !pDateList) return -1;

	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//__int64 nRealDate = nRealTime;
	__int64 nRealDate = 0;
	if (m_ttcTrPacket.GetPacketType() == _T("YYYYMMDDHHMMSS"))
	{
		if (m_bMonthChart)
			nRealDate = nRealTime / 100000000;	// 월간인 경우
		else
			nRealDate = nRealTime / 1000000;	// 일간인 경우.
	}
	else
		nRealDate = nRealTime;
	// 2011.01.21 by SYS <<

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	if( CChartInfo::WORLD_ON == m_nChartModeSetting || CChartInfo::FX == m_nChartModeSetting)
		GetMarketDate_DateTime( nRealDate);

	nRealDate = m_ttcTrPacket.GetPacketTypeTime( nRealDate);

	// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
	int nDeleteCount = 0;
	int nEndDate = -1;
	POSITION psnDate = pDateList->GetTailPosition();
	while( 0 < m_nClockedDataCount - nDeleteCount && psnDate)
	{
		nEndDate = ( int)pDateList->GetPrev( psnDate);
		if( nEndDate < nRealDate) break;
		nDeleteCount++;
	}
	if( 0 < nDeleteCount) 
		pPacket->RemoveBaseData( nDeleteCount);
	m_nClockedDataCount = 0;

	// 3.5. 일간 처리
	//		m_dateData에 관리중인 일자와 조회된 마지막 일자가 다른 경우만 Append 처리 (Update는 무의미)
	//		N일의 경우도 내림 분봉의 사이트도 올림으로 처리하여 모두 당일 일자를 표시한다.
	//			다만 24시간 연동될 경우만 문제된다.
	nEndDate = -1;
	if( 0 < pPacket->GetCount()) 
		nEndDate = ( int)pPacket->GetnumericDataList()->GetTail();

	// 장전에 TR을 받고 실시간이 처음 들어온 경우 (TR data에 오늘 날짜가 없는 경우)
	if( nEndDate < nRealDate) 
		pPacket->AppendData( ( double)nRealDate);

	return nDeleteCount;
}

// (2009/11/22 - Seung-Won, Bae) 일/월봉은 허봉에 의한 Update 기능이 필요없다. (조회 마감이 필요없다.)
//	따라서, 선/후 마감방식에 따라 마지막 일/월 봉, 또는 다음 일/월 봉값과 비교하여 마감 데이터를 발생시킨다.
//	이때 일봉의 경우 영업일을 확인하여야 한다.
bool CDateRealDataModule::GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime)
{
	// 1. Get the Last Data
	if( pPacket->GetCount() <= 0) 
		return false;

	__int64 nEndDate = ( __int64)pPacket->GetnumericDataList()->GetTail();

	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//nEndDate = m_ttcTrPacket.GetFullTypeTime( nEndDate);
	if (m_ttcTrPacket.GetPacketType() == "YYYYMMDDHHMMSS")
	{
		if (m_bMonthChart)
			nEndDate = nEndDate * 100000000;	// 월간인 경우
		else
			nEndDate = nEndDate * 1000000;		// 일간인 경우.
	}
	else
		nEndDate = m_ttcTrPacket.GetFullTypeTime(nEndDate);
	// 2011.01.21 by SYS <<

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	__int64 nRealDateTime = p_nRealDataTime;
	GetMarketDate_DateTime( nRealDateTime);

	__int64 nRealDate = nRealDateTime / 1000000;
	if( m_bMonthChart)
	{
		if( m_bCloseEmptyCandleWhenEnd)
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, 100000000);
		if( nRealDate / 100 <= nEndDate / 100000000) 
			return false;
	}
	else
	{
		do
		{
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, 1000000);
		}
		while( !IsMarketDay( int( nEndDate / 1000000)));

		if( !m_bCloseEmptyCandleWhenEnd)
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, -1000000);

		if( nRealDate <= nEndDate / 1000000) 
			return false;
	}

	if( CChartInfo::WORLD_ON == m_nChartModeSetting) 
		p_nRealDataTime = nRealDateTime;
//<<솔로몬... 해외선물 일봉 조회시 죽어서 수정 
	//==> 하였다가 원복함. 이 로직 없으면 허봉 마감이 안된다고 함. 죽었던 원인은 데이타 오류로 추정됨(배승원부장)
	if( m_bCloseEmptyCandleWhenEnd)
//	else if( m_bCloseEmptyCandleWhenEnd)
//>>
		p_nRealDataTime = CDataConversion::CalcTimeSum( p_nRealDataTime, -1000000);

	m_nClockedDataCount++;

	return true;
}

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
__int64 CDateRealDataModule::GetTimeIntervalWithSecond( void)
{
	if( m_bMonthChart) return __int64( 60. * 60. * 24. * 30.);
	return __int64( 60. * 60. * 24.);
}

//////////////////////////////////////////////////////////////////////
// class CWeekRealDataModule

// (2009/10/28 - Seung-Won, Bae) Week(주) 처리 Logic Class
// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
CWeekRealDataModule::CWeekRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
	: CTimeRealDataModuleBase( p_marketTimeData, strPacketType, eDateUnitType)
{
}
CWeekRealDataModule::~CWeekRealDataModule()
{
}


// (2009/11/19 - Seung-Won, Bae) 조회된 마지막 Data로 실시간 수신 대기할 구간값을 구한다.
//	이 구간값을 기준으로 실시간 수신시 마지막 Data에 Update할 것인지 Append할 것인지 결정된다.
//		특히 Append시에는 이전 구간을 마감시키는 Update이후 처리된다.
//		따라서 마지막 봉에 Update하기 위한 실시간 데이터의 시간범위를 구하는 것이다.
//	이 구간은 마지막 Data이전의 구간 시작기준시로 기본 구간을 구성하고, 마지막 Data가 실시간 Data인 것처럼 포함되는 구간으로 검색한다.
//	마지막 Data 자체가 없는 경우, 구간을 구성하지 않고, 첫 실시간이 들어올 때, 같은 위와 같은 방식으로 구한다.
//		물론 이렇게 구간이 구성되지 않은 상황에서는 허봉을 진행할 수 없으며,
//		첫 실봉 수신시는 무조건 Append로 처리후 구간을 구성한다.
BOOL CWeekRealDataModule::SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime)
{
	// (2009/11/13 - Seung-Won, Bae) 추가된 허봉을 RollBack 시키기.
	//	추가된 허봉의 갯수 만큼 확인하면서, 봉수신 대기 구간도 후진시키면서 삭제해야할 Data 갯수를 구한다.
	//  후 마감방식(m_bCloseEmptyCandleWhenEnd:TRUE)의 첫음에 실봉을 마감시키므로,
	//		허봉 Data 갯수를 -1부터 시작되도록 한다.
	if( m_bCloseEmptyCandleWhenEnd && 0 <= nTailTime)	m_nClockedDataCount = -1;
	else												m_nClockedDataCount = 0;
	return TRUE;
}


// (2009/11/23 - Seung-Won, Bae) 주봉의 실시간 수신은 수신된 실시간 데이터와 추가된 마지막 데이터가 속한
//		주초 월요일이 같은 날자인지 여부를 확인하여 Append/Update 여부를 결정한다.
//	이때, Append의 경우 추가된 마지막 데이터가 속한 마지막 영업일자로 Update를 선행시킨다.
int CWeekRealDataModule::SetRealData( CPacket* pPacket, const __int64 &nRealTime)
{
	if( !pPacket) return -1;
	const CList< double, double> *pDateList = pPacket->GetnumericDataList();
	if( !pDateList) return -1;

	// for Date Check.
	// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//__int64 nDateOfReal = nRealTime;
	__int64 nDateOfReal = 0;
	if (m_ttcTrPacket.GetPacketType() == _T("YYYYMMDDHHMMSS"))
		nDateOfReal = nRealTime / 1000000;
	else
		nDateOfReal = nRealTime;
	// 2011.01.24 by SYS <<

	GetMarketDate_DateTime( nDateOfReal);
	__int64 nLastMondayDateOfReal = GetLastMondayDate( nDateOfReal);

	// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
	int nDeleteCount = 0;
	__int64 nEndDate = -1;
	POSITION psnDate = pDateList->GetTailPosition();
	while( 0 < m_nClockedDataCount - nDeleteCount && psnDate)
	{
		nEndDate = ( __int64)pDateList->GetPrev( psnDate);
		if( GetLastMondayDate( m_ttcTrPacket.GetFullTypeTime( nEndDate)) < nLastMondayDateOfReal) 
			break;
		nDeleteCount++;
	}
	if( 0 < nDeleteCount) pPacket->RemoveBaseData( nDeleteCount);
	m_nClockedDataCount = ( m_bCloseEmptyCandleWhenEnd ? -1 : 0);

	if( pPacket->GetCount() <= 0)
		pPacket->AppendData( ( double)m_ttcTrPacket.GetPacketTypeTime( nDateOfReal));
	else
	{
		nEndDate = ( __int64)pPacket->GetnumericDataList()->GetTail();
		nEndDate = GetLastMondayDate( m_ttcTrPacket.GetFullTypeTime( nEndDate));
		if( nLastMondayDateOfReal <= nEndDate)
			pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nDateOfReal));
		else
		{
			nEndDate *= 1000000;
			__int64 nLastMarketDay = -1;

			// (2009/11/23 - Seung-Won, Bae) WorldOn close with first monday.
			if( CChartInfo::WORLD_ON == m_nChartModeSetting)
				nLastMarketDay = nEndDate;
			else
			{
				nEndDate = CDataConversion::CalcTimeSum( nEndDate, 6000000);
				for( int i = 0; i < 7; i++)
				{
					if( IsMarketDay( int( nEndDate / 1000000)))
					{
						nLastMarketDay = nEndDate;
						break;
					}
					nEndDate = CDataConversion::CalcTimeSum( nEndDate, -1000000);
				}
			}


			if( 0 < nLastMarketDay)
				pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nLastMarketDay));
			pPacket->AppendData( ( double)m_ttcTrPacket.GetPacketTypeTime( nDateOfReal));
		}
	}

	return nDeleteCount;
}

// (2009/10/29 - Seung-Won, Bae) Get the last monday date.
int CWeekRealDataModule::GetLastMondayDate( __int64 p_nTime)
{
	tm tmSum;
	tmSum.tm_isdst = -1;			//[A00000554]alzioyes:20110530 시간셋팅 초기값변경.

	tmSum.tm_sec = 0;
	tmSum.tm_min = 0;
	tmSum.tm_hour = 0;
	p_nTime /= 1000000;

	tmSum.tm_mday = int( p_nTime % 100);
	p_nTime /= 100;
	tmSum.tm_mon = int( p_nTime % 100) - 1;
	p_nTime /= 100;
	tmSum.tm_year = int( p_nTime) - 1900;

	tmSum.tm_wday = 0;
	tmSum.tm_yday = 0;
	//tmSum.tm_isdst = 0;			//[A00000554]alzioyes:20110530 시간셋팅 초기값변경.
	mktime( &tmSum);

	if( 1 != tmSum.tm_wday)
	{
		if( tmSum.tm_wday < 1) tmSum.tm_wday += 7;
		tmSum.tm_mday += 1 - tmSum.tm_wday;
		mktime( &tmSum);
	}

	int nDate = tmSum.tm_year + 1900;
	nDate *= 100;
	nDate += tmSum.tm_mon + 1;
	nDate *= 100;
	nDate += tmSum.tm_mday;
	return nDate;
}

// (2009/11/22 - Seung-Won, Bae) 주봉은 허봉에 의한 Update 기능이 필요하다.
//		그러나, 정확하게 주가 지난뒤에 Update하지 않으면 주중에 계속 Update하는 오동작을 만들게 된다.
//			(비정상적으로 장외로 실시간이 들어오는 경우 등)
//	선 마감방식의 경우, 마지막 데이터 주의 다음주의 첫 영업일 전날을 넘어섰는지로 확인하며,
//		들어온 시간을 선마감 기준시로 전달한다. (이 시간으로 마지막 데이터의 주가 바뀌어 반복 작업이 발생하지 않는다.)
//	후 마감방식의 경우, 마지막 데이터 주의 다음-다음주의 월요일 전날을 넘어섰는지로 확인하며,
//		주말 마지막 영업일자를 후 마감 기준시로 전달한다. (명확히 주가 지난뒤의 작업으로 반복 작업이 발생하지 않는다.)
//		특히 첫허봉의 경우 (허봉수가 -1), 다음-다음주가 아닌 마지막 데이터주의 다음주 월요일 전달을 넘어섰는지로 확인한다.
bool CWeekRealDataModule::GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime)
{
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime)) return false;

	if( pPacket->GetCount() <= 0) 
		return false;
	
	__int64 nEndDate = ( __int64)pPacket->GetnumericDataList()->GetTail();

	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//	__int64 nTempDate = GetLastMondayDate( m_ttcTrPacket.GetFullTypeTime( nEndDate));
	//	nEndDate = nTempDate * 1000000 + nMarketStartTime;
	if (m_ttcTrPacket.GetPacketType() == "YYYYMMDDHHMMSS")
		nEndDate = nEndDate * 1000000;
	else
		nEndDate = m_ttcTrPacket.GetFullTypeTime(nEndDate);
	nEndDate = nEndDate + nMarketStartTime;
	// 2011.01.21 by SYS <<

	//<<20100511_JS.Kim 솔로몬. 솔로몬은 데이타 시간이 거래소 시간 기준이 아닌 한국기준이다
	//			장시간이 역전인 경우 왜 무조건 하루를 빼주는지????
	//if( CChartInfo::WORLD_ON == m_nChartModeSetting && m_marketTimeData.IsReverseTime())
	//	nEndDate = CDataConversion::CalcTimeSum( nEndDate, -1000000);
	//>>

	if( nEndDate < 0) return false;

	if( m_bCloseEmptyCandleWhenEnd)
	{
		if( 0 <= m_nClockedDataCount)
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, 7000000);
		nEndDate = CDataConversion::CalcTimeSum( nEndDate, 7000000);

		if( p_nRealDataTime <= nEndDate) 
			return false;

		__int64 nLastMarketDay = -1;
		if( CChartInfo::WORLD_ON == m_nChartModeSetting)
		{
			nLastMarketDay = CDataConversion::CalcTimeSum( nEndDate, -7000000);
			GetMarketDate_DateTime( nLastMarketDay);
		}
		else
		{
			for( int i = 0; i < 7; i++)
			{
				nEndDate = CDataConversion::CalcTimeSum( nEndDate, -1000000);
				if( IsMarketDay( int( nEndDate / 1000000)))
				{
					nLastMarketDay = nEndDate;
					break;
				}
			}
		}
		if( nLastMarketDay < 0) return false;
		p_nRealDataTime = nLastMarketDay;
	}
	else
	{
		nEndDate = CDataConversion::CalcTimeSum( nEndDate, 7000000);
		__int64 nLastMarketDay = -1;
		for( int i = 1; i < 7; i++)
		{
			if( IsMarketDay( int( nEndDate / 1000000)))
			{
				nLastMarketDay = nEndDate;
				break;
			}
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, 1000000);
		}
		if( nLastMarketDay < 0) return false;
		nLastMarketDay = CDataConversion::CalcTimeSum( nLastMarketDay, -1000000);

		if( p_nRealDataTime / 1000000 <= nLastMarketDay / 1000000) return false;
	}

	m_nClockedDataCount++;
	return true;
}

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
__int64 CWeekRealDataModule::GetTimeIntervalWithSecond( void)
{
	return __int64( 60. * 60. * 24. * 7.);
}


//////////////////////////////////////////////////////////////////////
// class CTimeRealDataModule

// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
CTimeRealDataModule::CTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
	:	CTimeRealDataModuleBase( p_marketTimeData, strPacketType, eDateUnitType),
		m_dateData(dateData),
		m_nTimeRangeStart( 0),
		m_nTimeRangeEnd( 0)
{
	// (2009/10/21 - Seung-Won, Bae) Comment. N 시/분/초 봉의 단위 폭을 관리한다. (UT값)
	if( nTimeInterval < 1) 
		nTimeInterval = 1;

	// 2011.01.18 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//m_nTimeInterval = m_ttcTrPacket.GetFullTypeTime( nTimeInterval);
	if (m_ttcTrPacket.GetPacketType() == "YYYYMMDDHHMMSS")
		m_nTimeInterval = nTimeInterval * 100;
	else
		m_nTimeInterval = m_ttcTrPacket.GetFullTypeTime( nTimeInterval);
	// 2011.01.18 by SYS <<


	// (2009/10/21 - Seung-Won, Bae) Comment. 장시작 시간을 HHMMSS로 관리한다. (실시간 시작시간 확인용)
	m_strMarketStartTime = strMarketStartTime;
	m_nMarketStartTime = atoi( m_strMarketStartTime);

	// (2009/10/27 - Seung-Won, Bae) the Flag for real after special code like market end.
	m_bMarketOn = TRUE;
}

CTimeRealDataModule::~CTimeRealDataModule()
{
}


// (2009/10/21 - Seung-Won, Bae) Comment. 현재 실시간을 수신하는 봉의 구간 시작/끝 시간값을 관리한다. (Packet Type값이다.)
__int64 CTimeRealDataModule::GetTimeRangeStartWithPacketType( void) const
{
	return m_ttcTrPacket.GetPacketTypeTime( m_nTimeRangeStart);
}


// (2009/11/19 - Seung-Won, Bae) 조회된 마지막 Data로 실시간 수신 대기할 구간값을 구한다.
//	이 구간값을 기준으로 실시간 수신시 마지막 Data에 Update할 것인지 Append할 것인지 결정된다.
//		특히 Append시에는 이전 구간을 마감시키는 Update이후 처리된다.
//		따라서 마지막 봉에 Update하기 위한 실시간 데이터의 시간범위를 구하는 것이다.
//	이 구간은 마지막 Data이전의 구간 시작기준시로 기본 구간을 구성하고, 마지막 Data가 실시간 Data인 것처럼 포함되는 구간으로 검색한다.
//	마지막 Data 자체가 없는 경우, 구간을 구성하지 않고, 첫 실시간이 들어올 때, 같은 위와 같은 방식으로 구한다.
//		물론 이렇게 구간이 구성되지 않은 상황에서는 허봉을 진행할 수 없으며,
//		첫 실봉 수신시는 무조건 Append로 처리후 구간을 구성한다.
BOOL CTimeRealDataModule::SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime)
{
	if( nTailTime < 0) return FALSE;

	// (2009/11/13 - Seung-Won, Bae) 추가된 허봉을 RollBack 시키기.
	//	추가된 허봉의 갯수 만큼 확인하면서, 봉수신 대기 구간도 후진시키면서 삭제해야할 Data 갯수를 구한다.
	//  후 마감방식(m_bCloseEmptyCandleWhenEnd:TRUE)의 첫음에 실봉을 마감시키므로,
	//		허봉 Data 갯수를 -1부터 시작되도록 한다.
	m_nClockedDataCount = ( m_bCloseEmptyCandleWhenEnd ? -1 : 0);

	// 1. Make nTailTime to FullTime with Server Time.

	// 2011.01.11 by SYS >> 분차트 YYYY(년) 추가
	// 분봉에도 연도가 들어가도록 처리가 되어 수정
	//
// 	__int64 nFullTailTime = nTailTime;
// 	if( !m_ttcTrPacket.IsYearInPacketType())
// 	{
// 		// 1.1.1 Make User Locale Time of Server Time.
// 		__int64 nUserTimeOfServer = p_nServerTime;
// 		if( 0 != m_nRealTimeDiff) nUserTimeOfServer = CDataConversion::CalcTimeSum( nUserTimeOfServer, m_nRealTimeDiff);
// 		// 1.1.2 Make nTailTime to FullTime. (FullTime = FullTyped_DataTime + ServerTime - PacketTyped_ServerTime)
// 		__int64 nRotationTime = m_ttcTrPacket.GetRotationFullTime();
// 		nFullTailTime = m_ttcTrPacket.GetFullTypeTime( nFullTailTime);
// 		nFullTailTime += p_nServerTime - p_nServerTime % nRotationTime;
// 		// 1.1.3 Make Standard Time.
// 		if( 0 != m_nRealTimeDiff) nFullTailTime = CDataConversion::CalcTimeSum( nFullTailTime, -m_nRealTimeDiff);
// 		// 1.3 Tail Time can not be future time. (but can be future with time interval)
// 		__int64 nServerTime = CDataConversion::CalcTimeSum( p_nServerTime, m_nTimeInterval);
// 		while( nServerTime < nFullTailTime)
// 			nFullTailTime = CDataConversion::CalcTimeSum( nFullTailTime, -nRotationTime);
// 	}

	__int64 nFullTailTime = m_ttcTrPacket.GetFullTypeTime( nTailTime);

//	{
//		CString szDebug;
//		szDebug.Format("A p_nServerTime(%I64d) nFullTailTime(%I64d) nTailTime(%I64d) m_nRealTimeDiff(%I64d) m_nNewRangeStartTime(%I64d)", 
//				p_nServerTime, nFullTailTime, nTailTime, m_nRealTimeDiff, m_nNewRangeStartTime);
//		//OutputDebugString(szDebug);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}
	if( !m_ttcTrPacket.IsYearInPacketType())
	{
//		g_DRDebuger.DRLog_Message("B IsYearInPacketType is FALSE.");
		// 1.1.1 Make User Locale Time of Server Time.
		__int64 nUserTimeOfServer = p_nServerTime;
		if( 0 != m_nRealTimeDiff) nUserTimeOfServer = CDataConversion::CalcTimeSum( nUserTimeOfServer, m_nRealTimeDiff);
		// 1.1.2 Make nTailTime to FullTime. (FullTime = FullTyped_DataTime + ServerTime - PacketTyped_ServerTime)
		__int64 nRotationTime = m_ttcTrPacket.GetRotationFullTime();
		nFullTailTime += p_nServerTime - p_nServerTime % nRotationTime;
		// 1.1.3 Make Standard Time.
		if( 0 != m_nRealTimeDiff) nFullTailTime = CDataConversion::CalcTimeSum( nFullTailTime, -m_nRealTimeDiff);
		// 1.3 Tail Time can not be future time. (but can be future with time interval)
		__int64 nServerTime = CDataConversion::CalcTimeSum( p_nServerTime, m_nTimeInterval);
		while( nServerTime < nFullTailTime)
			nFullTailTime = CDataConversion::CalcTimeSum( nFullTailTime, -nRotationTime);
	}
	// 2011.01.11 by SYS <<


	// 2. Get the first Range with Range Start Time.
	// 2.1 Get FullTime of Range Start Time.
	if(p_nServerTime == 0)
		return FALSE;

	__int64 nRangeStartTime = p_nServerTime - ( p_nServerTime % 1000000) + m_nNewRangeStartTime;
	// 2.2 Range Start Time must be after tail time.
	while( nFullTailTime <= nRangeStartTime)
		nRangeStartTime = CDataConversion::CalcTimeSum( nRangeStartTime, -1000000);

	// 3. Get the Range for Tail Time.
	m_nTimeRangeStart = nRangeStartTime;
	m_nTimeRangeEnd = CDataConversion::CalcTimeSum( m_nTimeRangeStart, m_nTimeInterval);
//	{
//		CString szDebug;
//		szDebug.Format("C before: m_nTimeRangeEnd(%I64d) m_nTimeRangeStart(%I64d) m_nTimeInterval(%I64d) nFullTailTime(%I64d)\n", 
//					m_nTimeRangeEnd, m_nTimeRangeStart, m_nTimeInterval, nFullTailTime);
//		//OutputDebugString(szDebug);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}

	while( IsTimeAdd( nFullTailTime, m_nTimeRangeEnd))
		if( !GetNextIntervalWithoutOffTime( m_nTimeRangeStart, m_nTimeRangeEnd, FALSE))
			break;
//	{
//		CString szDebug;
//		szDebug.Format("D after : m_nTimeRangeEnd(%I64d) m_nTimeRangeStart(%I64d) m_nTimeInterval(%I64d)\n", m_nTimeRangeEnd, m_nTimeRangeStart, m_nTimeInterval);
//		//OutputDebugString(szDebug);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}

	// 4. for CEIL2 and FLOOR2 (Bound data is not included in range)
	if( GetRealType() == CPacketType::CEILING2 && m_nTimeRangeStart == nFullTailTime)
		GetPrevIntervalWithoutOffTime( m_nTimeRangeStart, m_nTimeRangeEnd, TRUE);

//	{
//		CString szDebug;
//		szDebug.Format("E m_nTimeRangeStart(%I64d) m_nTimeRangeEnd(%I64d)\n", m_nTimeRangeStart, m_nTimeRangeEnd);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}

	return TRUE;
}

// (2009/10/22 - Seung-Won, Bae) Comment. 수신된 실시간의 올림/버림 값을 구하여 각 Logic Class에 따라 Packet에 반영 처리한다.
// (2009/11/20 - Seung-Won, Bae) 주의! 조회데이터가 없는 경우 m_nTimeRangeStart, m_nTimeRangeEnd 값이 -1이다.
//		사용전 확인이 필요하다.
int CTimeRealDataModule::SetRealData( CPacket* pPacket, const __int64 &nRealTime)
{
	if( m_ttcTrPacket.GetPacketType().IsEmpty()) return -1;

	if( !m_bMarketOn) return 0;
	// (2009/10/27 - Seung-Won, Bae) the Flag for real after special code like market end.
	int nRealHMS = int( nRealTime % 1000000);
	if( g_iMetaTable.IsSpecialTime( nRealHMS))
	{
		if( 0 < m_nTimeRangeEnd)
		{
			// (2009/11/3 - Seung-Won, Bae) User Time.
			__int64 nUserTime = GetCloseTime( m_nTimeRangeStart, m_nTimeRangeEnd);
			if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

			m_bMarketOn = FALSE;
			pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));
		}
		return 0;
	}



	// 1. 수신된 실시간값의 분단위 올림/버림값을 굿한다. (시단위-시차트는 제공하지 않는다.)

	// 2011.01.18 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//__int64 nMod = nRealTime % m_ttcTrPacket.GetMinimumUnitTime();

	__int64	nMinimumUnitTime = m_ttcTrPacket.GetMinimumUnitTime();
	if (m_ttcTrPacket.GetPacketType() == _T("YYYYMMDDHHMMSS"))
		nMinimumUnitTime = 100;	
	__int64 nMod = nRealTime % nMinimumUnitTime;
	// 2011.01.18 by SYS <<
	
	__int64 nFloorTime = nRealTime - nMod;
	__int64 nCeilTime = nRealTime;
	// 01초 이상인 경우 분으로 올림.
	// 09:00:00 인경우 서버에서 백분의1초를 버림으로 줘서 09:00:00인 경우만 1초 올림(삼성전용)
	if( nRealHMS == m_nMarketStartTime
		|| ( 0 == nRealHMS % 100 && GetRealType() == CPacketType::CEILING2)
		|| 0 != nMod)
	{
		// 2011.01.18 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//nCeilTime = CDataConversion::CalcTimeSum( nCeilTime, m_ttcTrPacket.GetMinimumUnitTime());
		nCeilTime = CDataConversion::CalcTimeSum( nCeilTime, nMinimumUnitTime);
		// 2011.01.18 by SYS <<
	}
	nCeilTime = nCeilTime - nMod;

	// 2. 수신된 실시간의 올림/버림 값을 가지고 각 Logic Class에 맞게 Packet에 반영 처리한다.
	return SetRealData_AppendOrUpdate( pPacket, nRealTime, nFloorTime, nCeilTime);
}
// (2009/10/22 - Seung-Won, Bae) nCheckTime을 기준으로 Update/Append 여부를 결정하고,
//		실 Data로는 nUpdateTime을 반영한다. 봉마감시에는 nCloseTime으로 설정한다.
int CTimeRealDataModule::SetRealData_AppendOrUpdate2( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nCheckTime, const __int64 &nUpdateTime)
{
	// (2009/11/13 - Seung-Won, Bae) 추가된 허봉을 RollBack 시키기.
	//	추가된 허봉의 갯수 만큼 확인하면서, 봉수신 대기 구간도 후진시키면서 삭제해야할 Data 갯수를 구한다.
	//  후 마감방식(m_bCloseEmptyCandleWhenEnd:TRUE)의 첫음에 실봉을 마감시키므로,
	//		허봉 Data 갯수를 -1부터 시작되도록 한다.
	int nDeleteCount = 0;
	if( 0 < m_nTimeRangeEnd) while( 0 < m_nClockedDataCount - nDeleteCount && !IsTimeAdd( nCheckTime, m_nTimeRangeEnd))
	{
		GetPrevIntervalWithoutOffTime( m_nTimeRangeStart, m_nTimeRangeEnd);
		nDeleteCount++;
	}
	if( 0 < nDeleteCount) pPacket->RemoveBaseData( nDeleteCount);
	m_nClockedDataCount = ( m_bCloseEmptyCandleWhenEnd ? -1 : 0);

	CBaseData::RESULT eResult = CBaseData::Add;
	if( 0 < m_nTimeRangeEnd)
		if( !IsTimeAdd( nCheckTime, m_nTimeRangeEnd)) eResult = CBaseData::Update;

//	{
//		CString szDebug;
//		szDebug.Format("[SMalzio] realchk : IsTimeAdd (%d) nCheckTime(%I64d) m_nTimeRangeEnd(%I64d)\n", (nCheckTime > m_nTimeRangeEnd), nCheckTime, m_nTimeRangeEnd);
//		OutputDebugString(szDebug);
//	}

	if(eResult == CBaseData::Update)
	{
		// (2009/11/3 - Seung-Won, Bae) User Time.
		__int64 nUserTime = nUpdateTime;
		if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

		pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));
		return nDeleteCount;
	}

	if( 0 < m_nTimeRangeEnd && eResult == CBaseData::Add)
	{
		// (2009/11/3 - Seung-Won, Bae) User Time.
		__int64 nUserTime = GetCloseTime( m_nTimeRangeStart, m_nTimeRangeEnd);
		if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

		pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));
	}

	// (2009/11/3 - Seung-Won, Bae) User Time.
	__int64 nUserTime = nUpdateTime;
	if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

	// with CBaseData::TodayFirstReal
	__int64 nAppendTime = m_ttcTrPacket.GetPacketTypeTime( nUserTime);
	pPacket->AppendData( ( double)nAppendTime);

	// (2009/11/19 - Seung-Won, Bae) 조회된 마지막 Data로 실시간 수신 대기할 구간값을 구한다.
	//	이 구간값을 기준으로 실시간 수신시 마지막 Data에 Update할 것인지 Append할 것인지 결정된다.
	//		특히 Append시에는 이전 구간을 마감시키는 Update이후 처리된다.
	//		따라서 마지막 봉에 Update하기 위한 실시간 데이터의 시간범위를 구하는 것이다.
	//	이 구간은 마지막 Data이전의 구간 시작기준시로 기본 구간을 구성하고, 마지막 Data가 실시간 Data인 것처럼 포함되는 구간으로 검색한다.
	//	마지막 Data 자체가 없는 경우, 구간을 구성하지 않고, 첫 실시간이 들어올 때, 같은 위와 같은 방식으로 구한다.
	//		물론 이렇게 구간이 구성되지 않은 상황에서는 허봉을 진행할 수 없으며,
	//		첫 실봉 수신시는 무조건 Append로 처리후 구간을 구성한다.
	if( m_nTimeRangeEnd <= 0)
		SetTimeRange_TRPart( nRealTime, nAppendTime);

//	CString strTemp;
//	strTemp.Format("\n DRFN => nCheckTime = %I64d, m_nTimeRangeStart = %I64d, m_nTimeRangeEnd = %I64d", nCheckTime, m_nTimeRangeStart, m_nTimeRangeEnd);
//	OutputDebugString(strTemp);

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	if(m_ttcTrPacket.GetPacketType() == "YYYYMMDDHHMMSS"  && m_nTimeRangeEnd<20000000000000)
	{
//		{
//			CString szDebug;
//			szDebug.Format("[SMalzio]m_nTimeRangeEnd(%I64d) nCheckTime(%I64d)\n", m_nTimeRangeEnd, nCheckTime);
//			OutputDebugString(szDebug);
//		}

		m_nTimeRangeEnd = nCheckTime;
	}
	// 2011.01.06 by SYS <<

	while( IsTimeAdd( nCheckTime, m_nTimeRangeEnd))
	{
		GetNextIntervalWithoutOffTime( m_nTimeRangeStart, m_nTimeRangeEnd, FALSE);
	}

	return nDeleteCount;
}


// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
bool CTimeRealDataModule::GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime)
{
	if( m_ttcTrPacket.GetPacketType().IsEmpty() || m_nTimeRangeEnd <= 0 || !m_bMarketOn) 
		return false;

	// 1. 수신된 실시간값의 분단위 올림/버림값을 굿한다. (시단위-시차트는 제공하지 않는다.)


	// 2011.01.18 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//__int64 nMod = p_nRealDataTime % m_ttcTrPacket.GetMinimumUnitTime();
	__int64	nMinimumUnitTime = m_ttcTrPacket.GetMinimumUnitTime();
	if (m_ttcTrPacket.GetPacketType() == _T("YYYYMMDDHHMMSS"))
		nMinimumUnitTime = 100;	
	__int64 nMod = p_nRealDataTime % nMinimumUnitTime;
	// 2011.01.18 by SYS <<

	__int64 nFloorTime = p_nRealDataTime - nMod;
	__int64 nCeilTime = p_nRealDataTime;
	// 01초 이상인 경우 분으로 올림.
	// 09:00:00 인경우 서버에서 백분의1초를 버림으로 줘서 09:00:00인 경우만 1초 올림(삼성전용)
	if( p_nRealDataTime % 1000000 == m_nMarketStartTime
		|| ( 0 == p_nRealDataTime % 100 && GetRealType() == CPacketType::CEILING2)
		|| 0 != nMod)
	{
		// 2011.01.18 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//nCeilTime = CDataConversion::CalcTimeSum( nCeilTime, m_ttcTrPacket.GetMinimumUnitTime());
		nCeilTime = CDataConversion::CalcTimeSum( nCeilTime, nMinimumUnitTime);
		// 2011.01.18 by SYS <<
	}
	nCeilTime = nCeilTime - nMod;

	// 2. 수신된 실시간의 올림/버림 값을 가지고 각 Logic Class에 맞게 Packet에 반영 처리한다.
	if( !GetRealData_CloseTime_AppendOrUpdate( pPacket, nFloorTime, nCeilTime, p_nRealDataTime))
		return false;

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	if( CChartInfo::WORLD_ON == m_nChartModeSetting)
		GetMarketDate_DateTime( p_nRealDataTime);

	m_nClockedDataCount++;
	return true;
}
bool CTimeRealDataModule::GetRealData_CloseTime_AppendOrUpdate2( CPacket* pPacket, const __int64 &nCheckTime, const __int64 &nUpdateTime, __int64 &p_nRealDataTime)
{
	if( nCheckTime <= 0 || nUpdateTime <= 0) return  false;

	// (2009/11/13 - Seung-Won, Bae) 허봉 처리시, CLOCK의 주 기능은 시간이 되었을때,
	//		적절한 봉을 마감시켜 보이는 것이다.
	//	선 마감방식(m_bCloseEmptyCandleWhenEnd:FALSE)의 경우 현재 대기중인 구간을 벗어나는지 비교하고,
	//														(정확히는 다음 대기 구간을 도달했는지)
	//		벗어날 경우, 다음 구간을 마감시켜 보일 시간 Data로 구하여 전달하면,
	//		나머지는 실봉 처리 Logic에 의해 이전 구간이 자동으로 마감되고,
	//		신규 구간이 새로 구성되면서 마감표시도 이뤄진다.
	//  후 마감방식(m_bCloseEmptyCandleWhenEnd:TRUE)의 경우 현재 대기중인 구간의 다음 구간이 벗어나는지 비교하고,
	//			(첫 마감은 실봉의 마감이다. 그래서 허봉 갯수가 -1인 경우에는 다음 구간이 아닌 현 구간을 비교한다.)
	//		벗어날 경우, 그 현재 대기중인 구간의 다음 구간을 마감시켜 보일 시간 Data로 구하여 전달하면,
	//		나머지는 실봉 처리 Logic에 의해 이전 구간이 자동으로 마감되고,
	//		신규 구간이 새로 구성되면서 마감표시도 이뤄진다.
	// (2009/11/18 - Seung-Won, Bae) 비영업시간의 예외
	//		1. 장마감시간
	//		2. 비영업일
	//	둘다 GetNextIntervalWithoutOffTime()와 GetPrevIntervalWithoutOffTime()에서 자동으로 건너 뛰어야 한다.
	//	그래야 허봉의 RollBack 처리시 정확하게 처리된다.
	//	주의! 선 마감방식의 경우 비영업시간일 경우(다음 봉구성 구간에 시간이 되지 않은 경우) 선마감처리하지 않도록 한다.
	__int64 nNewTimeRangeStart = m_nTimeRangeStart;
	__int64 nNewTimeRangeEnd = m_nTimeRangeEnd;

	// 선/후 마감 방식에 따라 마감시킬 구간을 구하고, 마감시킬 시점인지 확인한다.
	if( m_bCloseEmptyCandleWhenEnd)
	{
		// 후마감 방식은 CLOCK 시간이 다음 봉구성 구간을 넘었는지 비교한다. 단 첫 허봉은 현 봉구성을 마감시켜야 한다.
		if( 0 <= m_nClockedDataCount) GetNextIntervalWithoutOffTime( nNewTimeRangeStart, nNewTimeRangeEnd);
		// 대상 봉구성 구간을 넘어섰는지 확인한다. 넘어야만 마감을 하던가 한다.
		if( !IsTimeAdd( nCheckTime, nNewTimeRangeEnd))	return false;
	}
	else
	{
		// 선 마감방식은 다음 봉구성 구간에 도달하였는지를 비교하여 선마감시킨다.
		GetNextIntervalWithoutOffTime( nNewTimeRangeStart, nNewTimeRangeEnd);
		if( !IsTimeAdd( nCheckTime, nNewTimeRangeStart)) return false;
	}

	// 마감시키기 위한 시간값을 구한다.
	__int64 nUpdateSec = CDataConversion::GetSecond( nUpdateTime, TRUE);
	__int64 nCloseSec = CDataConversion::GetSecond( GetCloseTime( nNewTimeRangeStart, nNewTimeRangeEnd), TRUE);
	if( nUpdateSec < nCloseSec && !m_ttcTrPacket.IsYearInPacketType())
		nUpdateSec += CDataConversion::GetSecond( m_ttcTrPacket.GetRotationFullTime());
	__int64 nDiff = nCloseSec - nUpdateSec;

	if( nDiff < -60 * 60 * 24) return false;

	p_nRealDataTime = CDataConversion::CalcTimeSum( p_nRealDataTime, nDiff, TRUE);
	return true;
}

BOOL CTimeRealDataModule::GetNextIntervalWithoutOffTime( __int64 &p_nTimeRangeStart, __int64 &p_nTimeRangeEnd, BOOL p_bWithoutOffTime)
{
	// X ScaleManager의 다른 시간대의 Scale 처리를 위하여 장시작/마감시간을 조회한다.
	BOOL bReturn = TRUE;
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime))
		bReturn = FALSE;

	int nStartTime = -1, nEndTime = -1;
	__int64 nIntervalSec = GetTimeIntervalWithSecond();
	__int64 nEnd = ( 24 * 60 * 60 * 7) / nIntervalSec;
	for( __int64 i = 0; i <= nEnd; i++)
	{
		p_nTimeRangeStart = p_nTimeRangeEnd;
		p_nTimeRangeEnd = CDataConversion::CalcTimeSum( p_nTimeRangeEnd, m_nTimeInterval);

		// (2009/11/16 - Seung-Won, Bae) 해당 실시간 구간을 m_nNewRangeStartTime를 기준으로 조정한다.
		nStartTime = int( p_nTimeRangeStart % 1000000);
		nEndTime = int( p_nTimeRangeEnd % 1000000);
		if( nEndTime < nStartTime)
		{
			if( m_nNewRangeStartTime < nEndTime || nStartTime < m_nNewRangeStartTime)
			{
				p_nTimeRangeEnd -= nEndTime;
				nEndTime = m_nNewRangeStartTime;
				p_nTimeRangeEnd += nEndTime;
			}
		}
		else
		{
			if( nStartTime < m_nNewRangeStartTime && m_nNewRangeStartTime < nEndTime)
			{
				p_nTimeRangeEnd -= nEndTime;
				nEndTime = m_nNewRangeStartTime;
				p_nTimeRangeEnd += nEndTime;
			}
		}

		if( !p_bWithoutOffTime) return TRUE;

		if( IsOnMarketTimeRange( p_nTimeRangeStart, p_nTimeRangeEnd, nStartTime, nEndTime, nMarketStartTime, nMarketEndTime)) return TRUE;
	}
	return bReturn; 
}

void CTimeRealDataModule::GetPrevIntervalWithoutOffTime( __int64 &p_nTimeRangeStart, __int64 &p_nTimeRangeEnd, BOOL p_bWithoutOffTime)
{
	// X ScaleManager의 다른 시간대의 Scale 처리를 위하여 장시작/마감시간을 조회한다.
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime)) return;

	int nStartTime = -1, nEndTime = -1;
	__int64 nIntervalSec = GetTimeIntervalWithSecond();
	for( int i = 0; i <= ( 24 * 60 * 60 * 7) / nIntervalSec; i++)
	{
		p_nTimeRangeEnd = p_nTimeRangeStart;
		p_nTimeRangeStart = CDataConversion::CalcTimeSum( p_nTimeRangeStart, -m_nTimeInterval);

		// (2009/11/16 - Seung-Won, Bae) 해당 실시간 구간을 m_nNewRangeStartTime를 기준으로 조정한다.
		nStartTime = int( p_nTimeRangeStart % 1000000);
		nEndTime = int( p_nTimeRangeEnd % 1000000);
		if( nEndTime == m_nNewRangeStartTime)
		{
			__int64 nMod = ( 24 * 60 * 60) % nIntervalSec;
			if( 0 != nMod)
			{
				p_nTimeRangeStart = CDataConversion::CalcTimeSum( p_nTimeRangeStart, nIntervalSec - nMod, TRUE);
				nStartTime = int( p_nTimeRangeStart % 1000000);
			}
		}

		if( p_bWithoutOffTime) return;

		if( IsOnMarketTimeRange( p_nTimeRangeStart, p_nTimeRangeEnd, nStartTime, nEndTime, nMarketStartTime, nMarketEndTime)) return;
	}
}


// (2009/11/21 - Seung-Won, Bae) 지정된 구간이 장중인지 여부를 확인한다.
//	1. 장중 여부는 기본적으로 장시작 시간과 장마감 시간을 기준으로한다.
//		실시간 시간 구간의 일부가 장시작 시간과 장마감 시간의 안쪽에 있으면 '장중'으로 인정한다.
//	2. 장중 시간대는 영업일 여부에 따라 장중 여부가 결정된다.
//	3. 장중이 아닌 시간대는 기본적으로 '장중'이 아니나, FX의 경우 당일과 익일이 영업일이면 계속 '장중'으로 인정한다.
BOOL CTimeRealDataModule::IsOnMarketTimeRange( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd, const int &nStartTime, const int &nEndTime, const int &nMarketStartTime, const int &nMarketEndTime)
{
	//	1. 장중 여부는 기본적으로 장시작 시간과 장마감 시간을 기준으로한다.
	//		실시간 시간 구간의 일부가 장시작 시간과 장마감 시간의 안쪽에 있으면 '장중'으로 인정한다.
	//	2. 장중 시간대는 영업일 여부에 따라 장중 여부가 결정된다.
	__int64 nDateCheckTime = -1;
	if( m_marketTimeData.IsReverseTime())
	{
		if( nStartTime < nMarketEndTime || nMarketStartTime <= nStartTime)
		{
			if( IsOnMarketDay( p_nTimeRangeStart)) return TRUE;
		}
		if( nEndTime <= nMarketEndTime || nMarketStartTime < nEndTime) 
		{
			if( IsOnMarketDay( p_nTimeRangeEnd)) return TRUE;
		}
		return FALSE;
	}
	else
	{
		if( nMarketStartTime <= nStartTime && nStartTime < nMarketEndTime)
		{
			if( IsOnMarketDay( p_nTimeRangeStart)) return TRUE;
		}
		if( nMarketStartTime < nEndTime && nEndTime <= nMarketEndTime)
		{
			if( IsOnMarketDay( p_nTimeRangeEnd)) return TRUE;
		}
		return FALSE;
	}

	//	3. 그리고 장중이 아닌 경우라 하여도, FX의 경우 당일과 익일이 영업일이면 연속장으로 진행된다.
	if( CChartInfo::FX == m_nChartModeSetting)
	{
		__int64 nMarketDate = p_nTimeRangeStart;
		GetMarketDate_DateTime( nMarketDate);
		if( !IsMarketDay( int( nMarketDate / 1000000))) return FALSE;
		if( !IsMarketDay( int( CDataConversion::CalcTimeSum( nMarketDate, 1000000) / 1000000))) return FALSE;
		return TRUE;
	}

	return FALSE;
}
BOOL CTimeRealDataModule::IsOnMarketDay( __int64 p_nDateCheckTime)
{
	GetMarketDate_DateTime( p_nDateCheckTime);
	return IsMarketDay( int( p_nDateCheckTime/ 1000000));
}


// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
__int64	CTimeRealDataModule::GetTimeIntervalWithSecond( void)
{
	return CDataConversion::GetSecond( m_nTimeInterval);
}


///////////////////////////////////////////////////////////////////////////////
// class CFloorTimeRealDataModule : 09:01:00 ~ 09:01:59 -> 09:01

// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
CFloorTimeRealDataModule::CFloorTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType):
	CTimeRealDataModule( dateData, p_marketTimeData, strPacketType, nTimeInterval, strMarketStartTime, eDateUnitType)
{
}

// (2009/10/22 - Seung-Won, Bae) 실시간에 봉을 구성하는 시간범위 끝값이 nEndTime이고, 실시간 값이 nCurTime일 경우, Append 상황인지 확인하는 Logic.
//		각 처리 Logic Class 별로 다르게 처리된다.
// 마지막 time에 update 할지의 여부
bool CFloorTimeRealDataModule::IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const
{
	if(nEndTime<0 || nCurTime<0)
	{
		return false;
	}

	return (nCurTime >= nEndTime);
}


// (2009/10/22 - Seung-Won, Bae) 실시간 시간의 올림/버림값으로 실 Packet값을 갱신하고 신규봉 구성시 구간값을 다시 산정한다.
//		Logic Class에 따라 Update/Append 확인용 시각, Update용 시각, 봉마감용 시각을 달리하여,
//		SetRealData_AppendOrUpdate()를 통해 처리된다.
int CFloorTimeRealDataModule::SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime)
{
	return CTimeRealDataModule::SetRealData_AppendOrUpdate2( pPacket, nRealTime, nFloorTime, nFloorTime);
}


// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
bool CFloorTimeRealDataModule::GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime)
{
	return GetRealData_CloseTime_AppendOrUpdate2( pPacket, nFloorTime, nFloorTime, p_nRealDataTime);
}


///////////////////////////////////////////////////////////////////////////////
// class CCeilingTimeRealDataModule : 09:00:01 ~ 09:01:00 -> 09:01 (메리츠)

// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
CCeilingTimeRealDataModule::CCeilingTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType):
	CTimeRealDataModule( dateData, p_marketTimeData, strPacketType, nTimeInterval, strMarketStartTime, eDateUnitType)
{
}


// (2009/10/22 - Seung-Won, Bae) 실시간에 봉을 구성하는 시간범위 끝값이 nEndTime이고, 실시간 값이 nCurTime일 경우, Append 상황인지 확인하는 Logic.
//		각 처리 Logic Class 별로 다르게 처리된다.
// 마지막 time에 update 할지의 여부
bool CCeilingTimeRealDataModule::IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const
{
	//100618-alzioyes : 무한루프 빠지는 것에대한 예외처리루틴 추가.
	if(nEndTime<0 || nCurTime<0)
	{
		return false;
	}

	return nCurTime > nEndTime;
}


// (2009/10/22 - Seung-Won, Bae) 실시간 시간의 올림/버림값으로 실 Packet값을 갱신하고 신규봉 구성시 구간값을 다시 산정한다.
//		Logic Class에 따라 Update/Append 확인용 시각, Update용 시각, 봉마감용 시각을 달리하여,
//		SetRealData_AppendOrUpdate()를 통해 처리된다.
int CCeilingTimeRealDataModule::SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime)
{
	return CTimeRealDataModule::SetRealData_AppendOrUpdate2( pPacket, nRealTime, nCeilTime, nCeilTime);
}


// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
bool CCeilingTimeRealDataModule::GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime)
{
	return GetRealData_CloseTime_AppendOrUpdate2( pPacket, nCeilTime, nCeilTime, p_nRealDataTime);
}


// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
///////////////////////////////////////////////////////////////////////////////
// class CCeiling2TimeRealDataModule : 09:00:00 ~ 09:00:59 -> 09:01 : 올림 (KOSCOM)

// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
CCeiling2TimeRealDataModule::CCeiling2TimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType):
	CCeilingTimeRealDataModule( dateData, p_marketTimeData, strPacketType, nTimeInterval, strMarketStartTime, eDateUnitType)
{
}

// (2009/10/22 - Seung-Won, Bae) 실시간에 봉을 구성하는 시간범위 끝값이 nEndTime이고, 실시간 값이 nCurTime일 경우, Append 상황인지 확인하는 Logic.
//		각 처리 Logic Class 별로 다르게 처리된다.
// 마지막 time에 update 할지의 여부
bool CCeiling2TimeRealDataModule::IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const
{
	//100618-alzioyes : 무한루프 빠지는 것에대한 예외처리루틴 추가.
	if(nEndTime<0 || nCurTime<0)
	{
		return false;
	}

	return nCurTime >= nEndTime;
}


// (2009/10/22 - Seung-Won, Bae) 실시간 시간의 올림/버림값으로 실 Packet값을 갱신하고 신규봉 구성시 구간값을 다시 산정한다.
//		Logic Class에 따라 Update/Append 확인용 시각, Update용 시각, 봉마감용 시각을 달리하여,
//		SetRealData_AppendOrUpdate()를 통해 처리된다.
int CCeiling2TimeRealDataModule::SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime)
{
	return CTimeRealDataModule::SetRealData_AppendOrUpdate2( pPacket, nRealTime, nFloorTime, nCeilTime);
}


// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
bool CCeiling2TimeRealDataModule::GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime)
{
	return GetRealData_CloseTime_AppendOrUpdate2( pPacket, nFloorTime, nCeilTime, p_nRealDataTime);
}
