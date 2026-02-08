// RealModuleCenter.cpp: implementation of the CRealModuleCenter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealModuleCenter.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl

#include "PacketBase.h"
#include "Conversion.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CRealModuleCenter

CRealModuleCenter::CRealModuleCenter( const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl, CTypedPtrList<CObList, CPacket*>* pPacketList) :
	m_realDataModule( p_pIChartCtrl->GetSafeHwnd())
{
	// (2009/11/4 - Seung-Won, Bae) Backup to receive NMTR Data.
	m_pIChartCtrl = p_pIChartCtrl;

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	m_nChartModeSetting = p_pIChartCtrl->GetChartMode();

	// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
	m_pOpenPacket	= NULL;
	m_pHighPacket	= NULL;
	m_pLowPacket	= NULL;
	m_pClosePacket	= NULL;
	m_pVolumePacket	= NULL;
	m_bOnClockedData = FALSE;

	// (2009/10/19 - Seung-Won, Bae) comment.
	//	Base Packet의 설정처리 - PacketList 최초 구성시 또는 AddPacket에 의하여 CPacketListImplementation::m_PacketList에
	//		Packet이 추가되는 경우 C0_DATE_TIME Packet (자료일자)를 구하여 등록한다.
	//	그외에는 별도의 CPacketList::SetBasePacketName()에 의해 별도 지정받기도 한다. (안쓰일듯.)
	m_pRealBasePacket = NULL;

	// (2009/10/20 - Seung-Won, Bae) Comment. 분봉 처리 Logic Class를 관리한다.
	m_pTimeRealDataModule = NULL;

	// (2009/10/21 - Seung-Won, Bae) Comment. 실시간 Data의 Packet Type을 관리한다.
	m_strRealTimeType = "HHMMSS";

	// (2009/10/22 - Seung-Won, Bae) Comment. X Scale Manager에서 시간 Merge에 참고하도록 분봉 올림 여부를 제공한다.
	m_eRealType = eRealType;

	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
	m_nOrgBaseTimeDiff = 0;
	m_nBaseTimeDiff = 0;

	// (2009/11/1 - Seung-Won, Bae) Support Last Real Time.
	m_nClock = -1;
	m_nLastClockTime = -1;
	m_nLastRealTime = -1;

	InitializeHelpMessage();
}

CRealModuleCenter::~CRealModuleCenter()
{
	delete m_pTimeRealDataModule;
}


// (2009/10/19 - Seung-Won, Bae) comment.
//	Base Packet의 설정처리 - PacketList 최초 구성시 또는 AddPacket에 의하여 CPacketListImplementation::m_PacketList에
//		Packet이 추가되는 경우 C0_DATE_TIME Packet (자료일자)를 구하여 등록한다.
//	그외에는 별도의 CPacketList::SetBasePacketName()에 의해 별도 지정받기도 한다. (안쓰일듯.)
void CRealModuleCenter::SetRealBasePacket( CPacket* pPacket)
{
	m_pRealBasePacket = pPacket;
}


// (2009/10/19 - Seung-Won, Bae) Comment. 마지막 실시간 갱신된 DTI.
//		전체 Data보기에서 Update할 행 확인시 이용한다.
//		자료일자가 아닌 기준패킷에 의해 실시간이 들어오는 경우만 이용된다.
void CRealModuleCenter::SetUpdateOrAppendIndex( const int nUpdateOrAppendIndex)
{
	m_nUpdateOrAppendIndex = nUpdateOrAppendIndex;
}
int CRealModuleCenter::GetUpdateOrAppendIndex( void) const
{
	return m_nUpdateOrAppendIndex;
}


// (2009/10/19 - Seung-Won, Bae) Comment. 마지막 Real이 반영된 DTI Range?
//		지표 계산시 CMainChartFormulateImplementation::GetDataPoint()에서 조회한다.
//			(실제로 range로 이용되지 않으며 start로만 이용되는 것으로 판단된다.)
//		UM=B로 이전이 아닌 이후 추가 Data를 수신하는 경우에 추가분 지표 재계산을 위하여 설정한다.
void CRealModuleCenter::SetUpdateRangeOffset(const int nStart, const int nEnd)
{
	m_ptUpdateRange.Offset(nStart, nEnd);
}
// 실시간 수신에 의해 새로 계산해야될 data 영역을 지정한다.
void CRealModuleCenter::SetPacketMinMaxAndUpdateRage(CPacket* pPacket)
{
	assert(pPacket != NULL);

	int nDataCount = pPacket->GetCount();
	// data가 없을 경우
	if(nDataCount <= 0)
		m_ptUpdateRange = CPoint(-1, -1);
	// data 가 1개
	else if(nDataCount == 1)
		m_ptUpdateRange = CPoint(0, 0);
	// data 가 2개 이상
	else
		m_ptUpdateRange = CPoint(nDataCount - 2, nDataCount - 1);
}
CPoint CRealModuleCenter::GetUpdateRange( void) const
{
	return m_ptUpdateRange;
}


// (2009/10/19 - Seung-Won, Bae) Comment. Real Data를 가공하는 Logic Class이다.
//		현재 RDC로만 이용되고 있음. ER도 있으나 RDC = 100 / ER 의 값으로 제공되는 경우일 뿐이므로 RDC로 통일이 바람직하다.
// 보조메시지로 RDC 비율값을 지정하는 Interface
bool CRealModuleCenter::SetRealDataModuleAndData( const bool bIsRemove, const CString& strModuleHelpMsg, const bool bIsExchangeRate_Percent)
{
	// 기본 비율 정보에 추가
	if( !bIsRemove) return m_realDataModule.ChangeRealDataModuleAndData(strModuleHelpMsg);

	m_realDataModule.SetRealDataModuleType( CRealDataModule::RATE_TYPE, strModuleHelpMsg);
	// 환율 비율 정보.
	if( bIsExchangeRate_Percent) m_realDataModule.ChangeExchangeRateData_Percent();
	return true;
}


// (2009/10/20 - Seung-Won, Bae) N틱을 위한 마지막봉에 들어간 Tick의 개수 (보조메시지 UTEC에 의해 지정됨)
void CRealModuleCenter::SetAddTimeInterval(const int nData)
{
	//sy 2005.07.28. n틱일 경우 마지막 봉의 틱수.
	if( nData < 0)	m_nAddTimeInterval = 0;
	else			m_nAddTimeInterval = nData;
}


// (2009/10/19 - Seung-Won, Bae) Comment. UD 보조메시지값 (주기값)을 관리한다.
void CRealModuleCenter::SetDateUnitType( const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
{
	m_eDateUnitType = eDateUnitType;
}
CScaleBaseData::HORZSCALEDRAWERTYPE CRealModuleCenter::GetDateUnitType( void) const
{
	return m_eDateUnitType;
}


// (2009/10/20 - Seung-Won, Bae) UT 값으로 1봉을 구성하기 위한 UD의 수량이다.
void CRealModuleCenter::SetTimeInterval(const int nTimeInterval)
{
	//>> 20091214_JS.Kim  동부.   버그수정.  보조메세지의 UT가 0인 경우 TimeRange 계산시 무한루프에 빠지는 문제 있음
	if( nTimeInterval <= 0 )
		m_nTimeInterval = 1;
	else
	//<<
		m_nTimeInterval = nTimeInterval;
}
int CRealModuleCenter::GetTimeInterval( void) const
{
	return m_nTimeInterval;
}
__int64 CRealModuleCenter::GetTimeIntervalWithSecond( void)
{
	return m_pTimeRealDataModule->GetTimeIntervalWithSecond();
}


// 실시간 Data를 추가 또는 Update 처리한다.
// 특히 일중 거래량, 거래대금을 받는 경우, 추가시 이전 일중 값을 구하여 편차가 추가되도록 한다.
bool CRealModuleCenter::SetAppendOrUpdateData_DataInDate(CPacket* pPacket, const int nBaseCount, const double& dData, double& dPrevData)
{
	if( !pPacket) return false;

	// Base count > Data count -> Data append
	if( 0 < nBaseCount && pPacket->GetCount() < nBaseCount)
	{
		dPrevData = dData;
		// real data 를 변경하는 경우 -> "기본거래량", "거래대금"
		if( pPacket->IsChangeRealData())
		{
			dPrevData = 0.0;
			pPacket->GetRealDataGap( dPrevData);
		}
		else
		{
			CList<double, double>* pDataList = pPacket->GetnumericDataList();
			if( pDataList != NULL && pDataList->GetTailPosition() != NULL)
				dPrevData = pDataList->GetTail();
		}

		if( SetAppendData(pPacket, nBaseCount, dData, dPrevData)) return true;
	}

	// Base count <= Data count -> Data update
	if( m_nUpdateOrAppendIndex < -1 || pPacket->GetCount() < nBaseCount)
		return false;

	pPacket->UpdateData( dData, m_nUpdateOrAppendIndex);
	SetPacketMinMaxAndUpdateRage(pPacket);
	return true;
}
// 실시간 Data 추가시, Base Packet의 Data Count에 맞춰 부족한 부분을 이전 값으로 Append한 뒤에 Append 처리를 한다.
bool CRealModuleCenter::SetAppendData( CPacket* pPacket, const int nBaseCount, const double& dData, const double& dPrevData)
{
	if( !pPacket) return false;

	int nCount = pPacket->GetCount();
	if( nBaseCount <= 0 || nBaseCount <= nCount) return false;

	// Base(자료일자 or 종가)의 갯수와 맞춰 연장. Base마지막이전값=이전data
	for( int nIndex = ( nCount < 0 ? 0 : nCount); nIndex < ( nBaseCount - 1); nIndex++)
		pPacket->AppendData( dPrevData);

	pPacket->AppendData(dData);
	SetPacketMinMaxAndUpdateRage(pPacket);
	return true;
}
// 고가와 저가의 Update를 처리한다. (현재 값보다 높거나 낮은 경우만 처리한다.)
bool CRealModuleCenter::SetUpdateData_Compare(CPacket* pPacket, const int nBaseCount, const double& dData, const bool bLowCheck)
{
	if( pPacket->GetCount() < nBaseCount) return false;

	double dTailData = pPacket->GetnumericDataList()->GetTail();
	// 저가(bLowCheck : true), 고가(bLowCheck : false) 값 비교
	if( bLowCheck && (dTailData <= dData) || !bLowCheck && (dTailData >= dData))
		return false;

	pPacket->UpdateData(dData);
	SetPacketMinMaxAndUpdateRage(pPacket);
	return true;
}
// 실시간 Data를 처리하는 Main Routine. (종가/자료일자/기준패킷/기타 로 구분되어 처리된다.)
bool CRealModuleCenter::SetRealData(CPacket* pPacket, const double dData, const int nOption, int &p_nRemovePacketDataCount)
{
	if(pPacket == NULL) return false;

	// 0. nOption에 의해 강제 Update(1), 또는 Append(2) 처리한다.
	if( 0 != nOption)
	{
		//update
		if(			nOption == 1)	pPacket->UpdateData(dData);
		//append
		else if(	nOption == 2)	pPacket->AppendData(dData);
		//min, max setting 하기
		SetPacketMinMaxAndUpdateRage(pPacket);
		return true;
	}

	// 1. 실 적용할 값을 구한다. (RDC에 의한 가공값을 구한다.)
	CString strPacketName = pPacket->GetName();
	double dRealData = m_realDataModule.GetRealData( strPacketName, dData);

	// 2. 종가 Packet의 처리 ~ <종가(현재가)> 일 경우는 종가에 따라 시,고,저 가 바뀌게 된다.
	//<< 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
	//if( pPacket->GetName() == _MTEXT( C0_CLOSE))
	if( strPacketName == _MTEXT( C0_CLOSE) || strPacketName == "_BuyClose_" || strPacketName == "_SellClose_")
	//>>
	{
		// 시/고/저 허봉처리
		double dPrevData = dRealData;
		if( m_pRealBasePacket) SetAppendOrUpdateData_DataInDate( pPacket, m_pRealBasePacket->GetCount(), dRealData, dPrevData);
		// 시/고/저의 봉이 부족하여 연장시에는 이전 종가를 사용하여 SetAppendOrUpdateData_DataInDate()를 사용하지 않는다.
		int nCloseCount = pPacket->GetCount();
		if( 0 < nCloseCount)
		{
			if( m_pOpenPacket) if( m_pOpenPacket->GetnumericDataList())
				SetAppendData( m_pOpenPacket, nCloseCount, dRealData, dPrevData);

			if( m_pHighPacket)
				if( m_pHighPacket->GetnumericDataList())
					if( !SetAppendData( m_pHighPacket, nCloseCount, dRealData, dPrevData))
						SetUpdateData_Compare( m_pHighPacket, nCloseCount, dRealData, false);

			if( m_pLowPacket)
				if( m_pLowPacket->GetnumericDataList())
					if( !SetAppendData( m_pLowPacket, nCloseCount, dRealData, dPrevData))
						SetUpdateData_Compare( m_pLowPacket, nCloseCount, dRealData, true);
		}
		
		return true;
	}

	// 3. 자료일자의 처리
	// "자료일자"가 포함된 packet : "자료일자", "MP_자료일자"
	if( 0 <= strPacketName.Find( _MTEXT( C0_DATE_TIME)))
	{
		__int64 nRealData = ( __int64)dRealData;

		// (2009/12/11 - Seung-Won, Bae) Check Special Time
		BOOL bSpecialTime = g_iMetaTable.IsSpecialTime( int( nRealData % 1000000));

		// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
		if( nRealData < 1000000)
		{
			if( nRealData + 10000 < m_nLastRealTime % 1000000 && !bSpecialTime)
				m_nLastRealTime = CDataConversion::CalcTimeSum( m_nLastRealTime, 1000000);
			nRealData += m_nLastRealTime - ( m_nLastRealTime % 1000000);
		}

		if( !m_bOnClockedData && !bSpecialTime) SetLastRealTime( nRealData);

		TRACE( "\r\nRealTime : %I64d", nRealData);

		//>>20100520_JS.Kim.		원래대로 복구하면서 원래 있던 오류만 수정
		////@Solomon:100114-JunokLee
		//int nDeleteCount = -1;
		//if(m_pTimeRealDataModule) m_pTimeRealDataModule->SetRealData( pPacket, nRealData);
		//if( nDeleteCount < 0) false;
		if(!m_pTimeRealDataModule) return false;

		int nDeleteCount = m_pTimeRealDataModule->SetRealData( pPacket, nRealData);
		if( nDeleteCount < 0) return false;
		//<<

		SetPacketMinMaxAndUpdateRage( pPacket);

		if( 0 < nDeleteCount)
		{
			if( m_pOpenPacket)		m_pOpenPacket->RemoveBaseData(		nDeleteCount);
			if( m_pHighPacket)		m_pHighPacket->RemoveBaseData(		nDeleteCount);
			if( m_pLowPacket)		m_pLowPacket->RemoveBaseData(		nDeleteCount);
			if( m_pClosePacket)		m_pClosePacket->RemoveBaseData(		nDeleteCount);
			if( m_pVolumePacket)	m_pVolumePacket->RemoveBaseData(	nDeleteCount);

			p_nRemovePacketDataCount += nDeleteCount;
		}
		return true;
	}

	// 4. 기준 Packet의 처리
	//		정렬된 수치를 검색하여 Update 또는 Insert할 Index를 검색하고(m_nUpdateOrAppendIndex) 처리한다.
	if( pPacket == m_pRealBasePacket)
	{
		m_nUpdateOrAppendIndex = -1;
		int nIndex = 0;
		CList<double, double>* pDataList = pPacket->GetnumericDataList();
		if( !pDataList) return false;

		POSITION pos = pDataList->GetHeadPosition();
		while(pos != NULL)
		{
			double dOrgData = pDataList->GetNext(pos);
			if( dRealData == dOrgData) //Update
			{
				m_nUpdateOrAppendIndex = nIndex;
				return true;
			}
			else if( dRealData < dOrgData) //Insert
			{
				m_nUpdateOrAppendIndex = nIndex;
				pPacket->AppendData( dRealData, m_nUpdateOrAppendIndex);
				return true;
			}
			nIndex++;
		}

		//AddTail
		pPacket->AppendData(dRealData);
		return false;
	}

	// 5. 일반 Packet의 처리
	if( m_pRealBasePacket)
	{
		double dPrevData = 0;
		SetAppendOrUpdateData_DataInDate( pPacket, m_pRealBasePacket->GetCount(), dRealData, dPrevData);
	}
	else
	{
		if( m_nUpdateOrAppendIndex >= -1)
		{
			// (2009/10/20 - Seung-Won, Bae) m_nUpdateOrAppendIndex는 기준 Packet이 있는 경우에만 이용되는 것으로 분석된다.
			//	기준 패킷이 없어서 이용되지 않는 경우 -1값으로 고정일텐데, -1보다 큰경우는 무엇일까?
			ASSERT( m_nUpdateOrAppendIndex == -1);

			pPacket->AppendData( dRealData, m_nUpdateOrAppendIndex);
		}
		else
		{
			// (2009/10/20 - Seung-Won, Bae) m_nUpdateOrAppendIndex는 기준 Packet이 있는 경우에만 이용되는 것으로 분석된다.
			//	기준 패킷이 없어서 이용되지 않는 경우 -1값으로 고정일텐데, -1보다 작은 경우는 뭘까?
			ASSERT( FALSE);

			pPacket->UpdateData( dRealData, m_nUpdateOrAppendIndex);
		}
		SetPacketMinMaxAndUpdateRage(pPacket);
	}
	return true;
}
// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
bool CRealModuleCenter::GetRealData_CloseTime( CPacket* pPacket, const __int64 &p_nClock, __int64 &p_nRealDataTime)
{
	m_nClock = p_nClock;
	if( m_nLastClockTime < 0) m_nLastClockTime = m_nClock;
	if( m_nLastRealTime < 0) return false;
	if( m_nClock == m_nLastRealTime) return false;
//	20121017 이상신 허봉그리기전(5초의 시간간격 사이에)에 실시간 데이터를 먼저 받는 오류를 막기위해 ---->>>>
//	if( m_nClock % 5) return false;
//	---------<<<<<<

	//2012-1012 by alzioyes.
	if( !m_pIChartCtrl->GetNormalizedMinuteTimeRule()) return false;

	m_nLastRealTime = CDataConversion::CalcTimeSum( m_nLastRealTime, m_nClock - m_nLastClockTime, TRUE);
	m_nLastClockTime = m_nClock;

	// Caution! the Clock must be faster than real time not to omit the empty candle.
	//	So the empty candle will be closed before 5 sec of real time.
	p_nRealDataTime = CDataConversion::CalcTimeSum( m_nLastRealTime, 5 - ( m_nLastRealTime % 5));

	TRACE( "CLOCK:%I64d\r\n", p_nRealDataTime);

	if( !m_pTimeRealDataModule->GetRealData_CloseTime( pPacket, p_nRealDataTime)) return false;

	if( CChartInfo::HTS == m_nChartModeSetting || CChartInfo::KRX_CME == m_nChartModeSetting)
		p_nRealDataTime %= 1000000;
	// (2009/12/2 - Seung-Won, Bae) for 31 Hour.
	if( CChartInfo::KRX_CME == m_nChartModeSetting && p_nRealDataTime < 70000)
		p_nRealDataTime += 240000;
		
	return true;
}


// (2009/10/20 - Seung-Won, Bae) Comment. 보조 메시지로 지정된 일자값을 관리한다.
// 지정된 설정값으로 일자로 초기화한다.
//		보조메시지 DATE 값.
//		보조메시지 DATATIME 값.
//		보조메시지 DATATIME 값으로 받은 거래소 일자를 기준으로 재산정한 영업일.
//				영업일은 지역시간 0시를 기준으로 바뀌며, 거래소 시간으로 일중에 변경된다.
//				지역시간을 기준으로한 영업일은 일/주/월봉의 산출 기준이 된다.
//"DATE=YYYYMMDD@" 에서 "YYYYMMDD"만 들어온다.  -> 현재 오늘 날짜를 받는다.
void CRealModuleCenter::SetCurrentDate(const CString& strData)
{
//	CString strOldDate = m_dateData.GetDate();
	
	if( strData.IsEmpty())	m_dateData.SetDate();	// PC 날짜로 셋팅.
	else					m_dateData.SetDate(strData);

// (2009/10/20 - Seung-Won, Bae) It will be done on CRealModuleCenter::InitializeTRPart()
// 날짜가 바뀔 경우 -> real 구간을 다시 정한다.
//	if( strOldDate != m_dateData.GetDate())
//		SetRealTimeModuleData();
}


// (2009/10/20 - Seung-Won, Bae) MARKETTIME 보조메시지로 설정된 장시작, 마감시간을 관리한다.
//sy 2005.01.25. 장시작시간과 마감시간 받기
//"MARKETTIME=장시작시간|장마감시간@" 에서 "장시작시간|장마감시간" 이 들어온다.
void CRealModuleCenter::SetMarketTime(const CString& strData)
{
	CString strEndTime = strData;
	if(strEndTime.Find("|") < 0)
		strEndTime += "|";

	CString strStartTime = CDataConversion::GetStringData(strEndTime, "|");
	if(!strStartTime.IsEmpty())
	{
		int nTime = atoi(strStartTime);
		if(nTime > 0)
			m_marketTimeData.SetMarketStartTime(nTime, true);
	}

	if(!strEndTime.IsEmpty())
	{
		int nTime = atoi(strEndTime);
		if(nTime > 0)
			m_marketTimeData.SetMarketEndTime(nTime, true);
	}
}
// X ScaleManager의 다른 시간대의 Scale 처리를 위하여 장시작/마감시간을 조회한다.
bool CRealModuleCenter::GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const
{
	return m_marketTimeData.GetMarketTime(nMarketStartTime, nMarketEndTime);
}


// (2009/10/20 - Seung-Won, Bae) Comment. 분봉 처리 Logic Class를 관리한다.
// 분봉 처리 Logic Type을 구한다.
CPacketType::REALTYPE CRealModuleCenter::GetRealType( void) const
{
	return m_eRealType;
}
// 분봉의 미래/과거 영역을 추론하기 위하여 마지막봉 구간 시작시간을 조회한다.
__int64	CRealModuleCenter::GetStartTimeInTimeRangeOfLastData( void)
{
	if( !m_pTimeRealDataModule) return -1;
	return m_pTimeRealDataModule->GetTimeRangeStartWithPacketType();
}


// (2009/10/21 - Seung-Won, Bae) Comment. 서로 다른 시간대를 비교하기 위한 기준으로,
//		현재 거래소의 일자/시각을 관리한다. (조회시점인가?)
// 현재 일자+시간 ( "YYYYMMDDHHMMSS" )
//  이 시간을 기준으로 시세 데이타의 일자 변경을 처리한다.
void CRealModuleCenter::SetCurrDateTime(const CString& strData)
{
	if( strData.IsEmpty() )	return;

	if( strData.GetLength() > 8 )
		m_nBasisTime = atoi(strData.Mid(8));

//	{
//		CString szDebug;
//		szDebug.Format("CRealModuleCenter::SetCurrDateTime(%s)", strData);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}

	SetCurrentDate(strData.Left(8));

	//>>20100222_JS.Kim		==>복기차트의 경우 CRealModuleCenter::InitializeTRPart()가 Call이 안되므로 여기서 해준다.
	m_nLastRealTime = -1;
	if( 0 <= m_nBasisTime) SetLastRealTime( (__int64)m_dateData.GetFullDate() * 1000000 + (__int64)m_nBasisTime);
	else
	{
		SYSTEMTIME st;
		GetLocalTime( &st);
		__int64 nLocalTime = (__int64)m_dateData.GetFullDate() * 1000000 + st.wHour * 10000 + st.wMinute * 100 + st.wSecond;
		if( CChartInfo::WORLD_ON == m_nChartModeSetting)
			nLocalTime = CDataConversion::CalcTimeSum( nLocalTime, ( m_nOrgBaseTimeDiff - 9) * 10000);
		SetLastRealTime( nLocalTime);
	}
	//<<
// 	if( strData.GetLength() > 8 )
// 	{
// 		// vntsorl:[0000684] 해외선물 차트에서 200분봉으로 조회시 19시 이후에 오류봉이 생성되는 현상
// 		m_nBasisTime = atoi(strData.Mid(8));
// 		
// 		if((m_eRealType == CPacketType::CEILING || m_eRealType == CPacketType::CEILING2) && 
// 			m_nTimeInterval > 1)
// 		{
// 			int nTimeDiff = (m_nTimeInterval / 100) * 10000 + (m_nTimeInterval % 100) * 100 - 1;
// 			m_nBasisTime -= nTimeDiff;
// 			if( m_nBasisTime < 0)
// 				m_nBasisTime = 0;
// 		}
// 	}
// 	
// 	SetCurrentDate(strData.Left(8));

}
// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
__int64 CRealModuleCenter::GetCurrDateTime() const
{
	if( m_nBasisTime < 0) return -1;

	return (__int64)m_dateData.GetFullDate() * 1000000 + (__int64)m_nBasisTime;
}


// (2009/10/21 - Seung-Won, Bae) Comment. 1일장이 장이 0시를 넘어가면서 진행하는지를 나타내는 Type을 관리한다.
CPacketType::MARKETDAYTYPE CRealModuleCenter::GetMarketDayType()
{
	return m_nMarketDayType;
}


// (2009/10/21 - Seung-Won, Bae) Comment. 실시간 Data의 Packet Type을 관리한다.
// 실시간 Packet 의 시간에 대한 type ( "HHMMSS", "YYYYMMDDHHMMSS" 등 )
//"REALTIMET=패킷타입@" 에서 "패킷타입" 이 들어온다.
void CRealModuleCenter::SetRealTimeType(const CString& strData)
{
	m_strRealTimeType = strData;
}


// (2009/10/21 - Seung-Won, Bae) Comment. 실시간 처리를 위한 환경값을 초기화한다.
// 보조메시지 설정 전 초기화
void CRealModuleCenter::InitializeHelpMessage()
{
	// (2009/11/16 - Seung-Won, Bae) Base Time of N-Minite
	m_nNewRangeStartTime = 0;

	//0000603: FX의 240 분봉으로 놓은 상태에서 리얼 시세를 받으면 아래와 같이 봉이 하나 더 생기지 않고 기존봉에 리얼데이타가 추가 되는 현상
	if(CChartInfo::FX == m_nChartModeSetting)
		m_nNewRangeStartTime = 50000;	//5시로 셋팅.

	// (2009/10/19 - Seung-Won, Bae) Comment. 마지막 실시간 갱신된 DTI
	m_nUpdateOrAppendIndex = -1;

	// (2009/10/19 - Seung-Won, Bae) Comment. 마지막 Real이 반영된 DTI Range?
	//		지표 계산시 CMainChartFormulateImplementation::GetDataPoint()에서 조회한다.
	//			(실제로 range로 이용되지 않으며 start로만 이용되는 것으로 판단된다.)
	m_ptUpdateRange = CPoint(-1, -1);

	// (2009/10/19 - Seung-Won, Bae) Comment. Real Data를 가공하는 Logic Class이다.
	//		현재 RDC로만 이용되고 있음. ER도 있으나 RDC = 100 / ER 의 값으로 제공되는 경우일 뿐이므로 RDC로 통일이 바람직하다.
	m_realDataModule.SetRealDataModuleType( CRealDataModule::BASE_TYPE);

	// (2009/10/20 - Seung-Won, Bae) N틱을 위한 마지막봉에 들어간 Tick의 개수 (보조메시지 UTEC에 의해 지정됨)
	m_nAddTimeInterval = 0;

	// (2009/10/19 - Seung-Won, Bae) Comment. UD 보조메시지값 (주기값)을 관리한다.
	// 가격 타입을 일단은 주가로 세팅한다
	m_eDateUnitType = CScaleBaseData::HORZ_DAILY;

	// (2009/10/20 - Seung-Won, Bae) UT 값으로 1봉을 구성하기 위한 UD의 수량이다.
	m_nTimeInterval = 1;

	// (2009/10/20 - Seung-Won, Bae) Comment. 보조 메시지로 지정된 일자값을 관리한다.
	//		우선 PC 일자으로 초기화한다.
	m_dateData.SetDate();

	// (2009/10/21 - Seung-Won, Bae) Comment. 서로 다른 시간대를 비교하기 위한 기준으로,
	//		현재 거래소의 일자/시각을 관리한다. (조회시점인가?)
	m_nBasisTime = -1;

	// (2009/10/21 - Seung-Won, Bae) Comment. 1일장이 장이 0시를 넘어가면서 진행하는지를 나타내는 Type을 관리한다.
	m_nMarketDayType = CPacketType::TODAY_ONLY;	//20081029 JS.Kim

	// (2009/11/16 - Seung-Won, Bae) Check for Market Days
	m_strMarketDayInfo.Empty();
}
// 보조메시지 및 TR Data 설정후 초기화
void CRealModuleCenter::InitializeTRPart( CPacket *p_pOpenPacket, CPacket *p_pHighPacket, CPacket *p_pLowPacket, CPacket *p_pClosePacket, CPacket *p_pVolumePacket)
{
	// (2009/11/1 - Seung-Won, Bae) Support Last Real Time.
	m_nLastRealTime = -1;
	if( 0 <= m_nBasisTime) SetLastRealTime( (__int64)m_dateData.GetFullDate() * 1000000 + (__int64)m_nBasisTime);
	else
	{
		SYSTEMTIME st;
		GetLocalTime( &st);
		//>>20100221_JS.Kim	PC일자로 처리하지 말고 보조메세지로 들어온 DATE를 사용하자.
		//__int64 nLocalTime = st.wYear * 10000000000 + st.wMonth * 100000000 + st.wDay * 1000000 + st.wHour * 10000 + st.wMinute * 100 + st.wSecond;
		__int64 nLocalTime = (__int64)m_dateData.GetFullDate() * 1000000 + st.wHour * 10000 + st.wMinute * 100 + st.wSecond;
		//<<
		if( CChartInfo::WORLD_ON == m_nChartModeSetting)
			nLocalTime = CDataConversion::CalcTimeSum( nLocalTime, ( m_nOrgBaseTimeDiff - 9) * 10000);
		SetLastRealTime( nLocalTime);
	}

	// (2009/10/21 - Seung-Won, Bae) Comment. 1일장이 장이 0시를 넘어가면서 진행하는지를 나타내는 Type을 관리한다.
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime))
		if( nMarketEndTime <= nMarketStartTime)
			m_nMarketDayType = CPacketType::WITH_NEXT_DAY;

	// (2009/11/23 - Seung-Won, Bae) for HTS default value.
	if( CChartInfo::HTS == m_nChartModeSetting && 0 == m_nNewRangeStartTime)
		m_nNewRangeStartTime = nMarketStartTime;

	// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
	m_pOpenPacket	= p_pOpenPacket;
	m_pHighPacket	= p_pHighPacket;
	m_pLowPacket	= p_pLowPacket;
	m_pClosePacket	= p_pClosePacket;
	m_pVolumePacket	= p_pVolumePacket;

	if( !m_pRealBasePacket) return;

	// (2009/10/21 - Seung-Won, Bae) Comment. 자료일자 PacketType을 지정받아 해당 Format에 맞는 Date를 별도로 관리한다.
	m_dateData.SetPacketType( m_pRealBasePacket->GetType());

	if( m_pRealBasePacket->GetName().Find( _MTEXT( C0_DATE_TIME)) < 0) return;

	//sy 2005.01.28. -> 장시작시간을 보조메시지를 통해 받는다.
	CString strMarketStartTime;
	if( !m_marketTimeData.GetMarketStartTime( strMarketStartTime)) strMarketStartTime.Empty();

	if( m_pTimeRealDataModule) delete m_pTimeRealDataModule;

	// 2011.02.18 by SYS >> 분차트 YYYY(년) 수정 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	switch( m_eDateUnitType)
// 	{
// 		// 일간/월간
// 		case CScaleBaseData::HORZ_MONTHLY:
// 		case CScaleBaseData::HORZ_DAILY:	m_pTimeRealDataModule = new CDateRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), CScaleBaseData::HORZ_MONTHLY == m_eDateUnitType);
// 											break;
// 		// 주간
// 		case CScaleBaseData::HORZ_WEEKLEY:	m_pTimeRealDataModule = new CWeekRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType());
// 											break;
// 		// 시간(분)
// 		case CScaleBaseData::HORZ_TIME_TWO:
// 		case CScaleBaseData::HORZ_TIME_ONE:	
// 			switch( m_eRealType)
// 			{
// 				// 버림. 09:01:00 ~ 09:01:59 -> 09:01
// 				case CPacketType::FLOOR:	m_pTimeRealDataModule = new CFloorTimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime);
// 											break;
// 				// 올림. 09:00:01 ~ 09:01:00 -> 메리츠
// 				default:
// 				case CPacketType::CEILING:	m_pTimeRealDataModule = new CCeilingTimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime);
// 											break;
// 				// 올림. 09:00:00 ~ 09:00:59 -> 09:01 (KOSCOM)	// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
// 				case CPacketType::CEILING2:	m_pTimeRealDataModule = new CCeiling2TimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime);
// 											break;
// 			}
// 			break;
// 		// 틱
// 		default:
// 		case CScaleBaseData::HORZ_TICK:		m_pTimeRealDataModule = new CTickRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, m_nAddTimeInterval);
// 											break;
// 	}
	
	switch( m_eDateUnitType)
	{
		// 일간/월간
		case CScaleBaseData::HORZ_MONTHLY:
		case CScaleBaseData::HORZ_DAILY:	m_pTimeRealDataModule = new CDateRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), m_eDateUnitType, CScaleBaseData::HORZ_MONTHLY == m_eDateUnitType);
											break;
		// 주간
		case CScaleBaseData::HORZ_WEEKLEY:	m_pTimeRealDataModule = new CWeekRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), m_eDateUnitType);
											break;
		// 시간(분)
		case CScaleBaseData::HORZ_TIME_TWO:
		case CScaleBaseData::HORZ_TIME_ONE:	
			switch( m_eRealType)
			{
				// 버림. 09:01:00 ~ 09:01:59 -> 09:01
				case CPacketType::FLOOR:	m_pTimeRealDataModule = new CFloorTimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime, m_eDateUnitType);
											break;
				// 올림. 09:00:01 ~ 09:01:00 -> 메리츠
				default:
				case CPacketType::CEILING:	m_pTimeRealDataModule = new CCeilingTimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime, m_eDateUnitType);
											break;
				// 올림. 09:00:00 ~ 09:00:59 -> 09:01 (KOSCOM)	// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
				case CPacketType::CEILING2:	m_pTimeRealDataModule = new CCeiling2TimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime, m_eDateUnitType);
											break;
			}
			break;
		// 틱
		default:
		case CScaleBaseData::HORZ_TICK:		m_pTimeRealDataModule = new CTickRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, m_nAddTimeInterval, m_eDateUnitType);
											break;
	}
	// 2011.02.18 by SYS <<

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	m_pTimeRealDataModule->SetChartMode( m_nChartModeSetting);

	m_pTimeRealDataModule->OnInitTR( ( m_nBaseTimeDiff - m_nOrgBaseTimeDiff) * 10000, m_nNewRangeStartTime, m_strMarketDayInfo);

	double dTailData = -1;
	CList< double, double>* pDataList = m_pRealBasePacket->GetnumericDataList();
	if( pDataList) if( 0 < pDataList->GetCount())
		m_pTimeRealDataModule->SetTimeRange_TRPart( m_nLastRealTime, ( __int64)pDataList->GetTail());
}


// (2009/11/1 - Seung-Won, Bae) for Normalized Candle data.
double CRealModuleCenter::GetOrgDataBeforeRate( const char *p_szPacketName, const double p_dRatedData)
{
	return m_realDataModule.GetOrgData( p_szPacketName, p_dRatedData);
}


// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
void CRealModuleCenter::GetRealData_RealDateTime( double &p_dData) const
{
	// (2009/12/11 - Seung-Won, Bae) for No Date Real Time.
	if( p_dData < 1000000) return;

	if( !m_marketTimeData.IsReverseTime()) return;

	// X ScaleManager의 다른 시간대의 Scale 처리를 위하여 장시작/마감시간을 조회한다.
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime)) return;

	if( __int64( p_dData) % 1000000 < nMarketStartTime) return;
	p_dData = ( double)CDataConversion::CalcTimeSum( ( __int64)p_dData, -1000000);
}


// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
void CRealModuleCenter::SetTimeDiffInfo( const int &p_nOrgBaseTimeDiff, const int &p_nBaseTimeDiff)
{
	m_nOrgBaseTimeDiff = p_nOrgBaseTimeDiff;
	m_nBaseTimeDiff	= p_nBaseTimeDiff;
	if( m_pTimeRealDataModule) m_pTimeRealDataModule->SetRealTimeDiff( ( m_nBaseTimeDiff - m_nOrgBaseTimeDiff) * 10000);
}


// (2009/11/1 - Seung-Won, Bae) Support Last Real Time.
void CRealModuleCenter::SetLastRealTime( const __int64 &p_nRealTime)
{
	m_nLastClockTime = m_nClock;
	m_nLastRealTime = p_nRealTime;
}

// (2009/11/4 - Seung-Won, Bae) Backup to receive NMTR Data.
void CRealModuleCenter::PushNMTRState( __int64 p_nRealDataTime)
{
	m_bOnClockedData = TRUE;
	m_nLastRealTimeBackup = m_nLastRealTime;
	if( CChartInfo::HTS == m_nChartModeSetting || CChartInfo::KRX_CME == m_nChartModeSetting)
	{
		// (2009/12/2 - Seung-Won, Bae) for 31 Hour.
		if( CChartInfo::KRX_CME == m_nChartModeSetting && 240000 <= p_nRealDataTime)
			p_nRealDataTime -= 240000;

		if( m_nLastRealTime % 1000000 < p_nRealDataTime)
			m_nLastRealTime = CDataConversion::CalcTimeSum( m_nLastRealTime, -1000000);
		m_nLastRealTime += p_nRealDataTime - m_nLastRealTime % 1000000;
	}
	else m_nLastRealTime = p_nRealDataTime;
	m_pTimeRealDataModule->PushNMTRState();
}
void CRealModuleCenter::PopNMTRState( void)
{
	m_pTimeRealDataModule->PopNMTRState();
	m_nLastRealTime = m_nLastRealTimeBackup;
	m_bOnClockedData = FALSE;
}

// (2009/11/12 - Seung-Won, Bae) Get NMTR Info
BOOL CRealModuleCenter::GetLastClockAndRealTime( __int64 &p_nLastClockTime, __int64 &p_nLastRealTime)
{
	p_nLastClockTime = m_nLastClockTime;
	p_nLastRealTime = m_nLastRealTime;
	return TRUE;
}

// (2009/11/16 - Seung-Won, Bae) Base Time of N-Minite
void CRealModuleCenter::SetNewRangeStartTime( const CString &p_strTime)
{
	m_nNewRangeStartTime = atoi( p_strTime);
}
