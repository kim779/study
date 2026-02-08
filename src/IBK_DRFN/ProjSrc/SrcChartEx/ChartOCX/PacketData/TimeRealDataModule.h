// TimeRealDataModule.h: interface for the CTimeRealDataModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEREALDATAMODULE_H__AA27934F_EA58_40D3_960A_440C19829896__INCLUDED_)
#define AFX_TIMEREALDATAMODULE_H__AA27934F_EA58_40D3_960A_440C19829896__INCLUDED_

#include <afxtempl.h>			// for CTypedPtrMap

#include "../Include_Chart/BlockBaseEnum.h"
#include "../Include_Chart/TimeTypeConvert.h"		// for CTimeTypeConvert

#include "PacketBaseData.h"							// for CMarketTimeData

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDateData;
class CPacket;

class CBaseData
{
public:
	enum RESULT
	{
		Error = 0,
		TodayFirstReal,
		Add,
		Update,
		AddNMTime,		// 2008.08.25 JS.Kim 허수봉 처리
		UpdateNMTime,
		UpdateCloseTime
	};

	enum MARKET_STATUS
	{
		Close		= 0,
		Trading		= 1,
		NoDefine	= 2
	};
};


// (2009/10/28 - Seung-Won, Bae) Real 시각 처리 Logic Class의 최상위 Base
class CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CTimeRealDataModuleBase( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	virtual ~CTimeRealDataModuleBase();


// (2009/10/21 - Seung-Won, Bae) Comment. 자료일자 Packet의 Data Type을 관리한다. (ex : "YYYYMMDDHHMMSS")
protected:
	CTimeTypeConvert	m_ttcTrPacket;


// (2009/11/19 - Seung-Won, Bae) 조회된 마지막 Data로 실시간 수신 대기할 구간값을 구한다.
//	이 구간값을 기준으로 실시간 수신시 마지막 Data에 Update할 것인지 Append할 것인지 결정된다.
//		특히 Append시에는 이전 구간을 마감시키는 Update이후 처리된다.
//		따라서 마지막 봉에 Update하기 위한 실시간 데이터의 시간범위를 구하는 것이다.
//	이 구간은 마지막 Data이전의 구간 시작기준시로 기본 구간을 구성하고, 마지막 Data가 실시간 Data인 것처럼 포함되는 구간으로 검색한다.
//	마지막 Data 자체가 없는 경우, 구간을 구성하지 않고, 첫 실시간이 들어올 때, 같은 위와 같은 방식으로 구한다.
//		물론 이렇게 구간이 구성되지 않은 상황에서는 허봉을 진행할 수 없으며,
//		첫 실봉 수신시는 무조건 Append로 처리후 구간을 구성한다.
public:
	virtual BOOL	SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime)
	{
		// (2009/11/13 - Seung-Won, Bae) 추가된 허봉을 RollBack 시키기.
		//	추가된 허봉의 갯수 만큼 확인하면서, 봉수신 대기 구간도 후진시키면서 삭제해야할 Data 갯수를 구한다.
		//  후 마감방식(m_bCloseEmptyCandleWhenEnd:TRUE)의 첫음에 실봉을 마감시키므로,
		//		허봉 Data 갯수를 -1부터 시작되도록 한다.
		m_nClockedDataCount = 0;
		return TRUE;
	}


// (2009/10/22 - Seung-Won, Bae) Comment. 수신된 실시간의 올림/버림 값을 구하여 각 Logic Class에 따라 Packet에 반영 처리한다.
public:
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime)					{	return -1;	}


// (2009/10/21 - Seung-Won, Bae) Comment. 현재 실시간을 수신하는 봉의 구간 시작/끝 시간값을 관리한다. (Packet Type값이다.)
public:
	virtual __int64		GetTimeRangeStartWithPacketType( void) const							{	return 0;		}


// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
protected:
	int				m_nClockedDataCount;
	int				m_nClockedDataCountBackup;
public:
	virtual bool	GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime)			{	return false;	}
	void			PushNMTRState( void)	{	m_nClockedDataCountBackup = m_nClockedDataCount;	m_nClockedDataCount = 0;	}
	void			PopNMTRState( void)		{	m_nClockedDataCount = m_nClockedDataCountBackup;	}


// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
protected:
	__int64		m_nRealTimeDiff;
	int			m_nNewRangeStartTime;
public:
	void		OnInitTR( const __int64 &p_nRealTimeDiff, const int &p_nNewRangeStartTime, const CString &p_szMarketDayInfo);
	void		SetRealTimeDiff( const __int64 &p_nRealTimeDiff)	{	m_nRealTimeDiff = p_nRealTimeDiff;	}


// (2009/11/4 - Seung-Won, Bae) Calculate Market Date.
protected:
	CMarketTimeData &	m_marketTimeData; // 장시작시간,장마감시간
public:
	void				GetMarketDate_DateTime( __int64 &p_nData) const;


// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
protected:
	CChartInfo::CHART_MODE	m_nChartModeSetting;
public:
	void					SetChartMode( CChartInfo::CHART_MODE p_nChartModeSetting)			{	m_nChartModeSetting = p_nChartModeSetting;		}


// (2009/11/4 - Seung-Won, Bae) Empty Candle Closing Time Type.
protected:
	BOOL	m_bCloseEmptyCandleWhenEnd;

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
public:
	virtual __int64		GetTimeIntervalWithSecond( void)		{	return 0;	}

// (2009/11/16 - Seung-Won, Bae) Check for Market Days
protected:
	CMapPtrToPtr		m_mapAddedMarketDays;
	CMapPtrToPtr		m_mapRemovedMarketDays;
public:
	BOOL				IsMarketDay( int p_nDate);

	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CScaleBaseData::HORZSCALEDRAWERTYPE		m_eDateUnitType; // "UD" : 일,주,월,분,틱... 형태
	// 2011.01.21 by SYS <<
};


// (2009/10/28 - Seung-Won, Bae) Tick 처리 Logic Class
class CTickRealDataModule : public CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CTickRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const int nAddTimeInterval, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	virtual ~CTickRealDataModule();

// (2009/10/21 - Seung-Won, Bae) Comment. N틱 봉의 단위 틱을 관리한다. (UT값)
protected:
	__int64		m_nTimeInterval;	// 분간격

// (2009/10/20 - Seung-Won, Bae) N틱을 위한 마지막봉에 들어간 Tick의 개수 (보조메시지 UTEC에 의해 지정됨)
protected:
	int		m_nAddTimeInterval; // Tick에서만 사용

// (2009/10/22 - Seung-Won, Bae) Comment. 수신된 실시간의 올림/버림 값을 구하여 각 Logic Class에 따라 Packet에 반영 처리한다.
protected:
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime);

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
protected:
	virtual __int64		GetTimeIntervalWithSecond( void)		{	return 1;	}
};


// (2009/10/28 - Seung-Won, Bae) Date(일/월) 처리 Logic Class
class CDateRealDataModule : public CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CDateRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType, BOOL p_bMonthChart = FALSE);
	virtual ~CDateRealDataModule();

// (2009/10/22 - Seung-Won, Bae) Comment. 수신된 실시간의 올림/버림 값을 구하여 각 Logic Class에 따라 Packet에 반영 처리한다.
protected:
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime);

// (2009/11/22 - Seung-Won, Bae) 일/월봉은 허봉에 의한 Update 기능이 필요없다. (조회 마감이 필요없다.)
//	따라서, 선/후 마감방식에 따라 마지막 일/월 봉, 또는 다음 일/월 봉값과 비교하여 마감 데이터를 발생시킨다.
//	이때 일봉의 경우 영업일을 확인하여야 한다.
protected:
	virtual bool	GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime);

protected:
	BOOL	m_bMonthChart;

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
protected:
	virtual __int64		GetTimeIntervalWithSecond( void);
};


// (2009/10/28 - Seung-Won, Bae) Week(주) 처리 Logic Class
class CWeekRealDataModule : public CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CWeekRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	virtual ~CWeekRealDataModule();

// (2009/11/19 - Seung-Won, Bae) 조회된 마지막 Data로 실시간 수신 대기할 구간값을 구한다.
//	이 구간값을 기준으로 실시간 수신시 마지막 Data에 Update할 것인지 Append할 것인지 결정된다.
//		특히 Append시에는 이전 구간을 마감시키는 Update이후 처리된다.
//		따라서 마지막 봉에 Update하기 위한 실시간 데이터의 시간범위를 구하는 것이다.
//	이 구간은 마지막 Data이전의 구간 시작기준시로 기본 구간을 구성하고, 마지막 Data가 실시간 Data인 것처럼 포함되는 구간으로 검색한다.
//	마지막 Data 자체가 없는 경우, 구간을 구성하지 않고, 첫 실시간이 들어올 때, 같은 위와 같은 방식으로 구한다.
//		물론 이렇게 구간이 구성되지 않은 상황에서는 허봉을 진행할 수 없으며,
//		첫 실봉 수신시는 무조건 Append로 처리후 구간을 구성한다.
protected:
	virtual BOOL	SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime);

// (2009/10/22 - Seung-Won, Bae) Comment. 수신된 실시간의 올림/버림 값을 구하여 각 Logic Class에 따라 Packet에 반영 처리한다.
protected:
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime);

// (2009/10/29 - Seung-Won, Bae) Get the last monday date.
protected:
	int		GetLastMondayDate( __int64 p_nTime);

// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
protected:
	virtual bool	GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime);

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
protected:
	virtual __int64		GetTimeIntervalWithSecond( void);
};


// (2009/10/28 - Seung-Won, Bae) Comment. 시간개념 Logic Class의 Base
class CTimeRealDataModule : public CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	virtual ~CTimeRealDataModule();


// (2009/10/22 - Seung-Won, Bae) Comment. 분봉 올림 여부를 제공한다.
protected:
	virtual CPacketType::REALTYPE	GetRealType( void) const = 0;


// (2009/10/21 - Seung-Won, Bae) Comment. N 시/분/초 봉의 단위 폭을 관리한다. (UT값)
protected:
	__int64		m_nTimeInterval;
// (2009/10/21 - Seung-Won, Bae) Comment. 장시작 시간을 HHMMSS로 관리한다. (실시간 시작시간 확인용)
protected:
	int			m_nMarketStartTime;
	CString		m_strMarketStartTime;	// 장시작시간(HHMMSS 타입으로 저장한다.)


// (2009/10/21 - Seung-Won, Bae) Comment. 현재 실시간을 수신하는 봉의 구간 시작/끝 시간값을 관리한다. (Packet Type값이다.)
protected:
	__int64				m_nTimeRangeStart;		// update 할 범위(시작)	~ Floor Type 봉마감시 마감값으로만 쓰인다.
	__int64				m_nTimeRangeEnd;		// update 할 범위(끝)	~ GetNextIntervalWithoutOffTime()에 의해 구해져, Update/Appen 상황인지 식별하는 기준 및 Ceil Type 봉마감값으로 쓰인다.
protected:
	virtual __int64		GetTimeRangeStartWithPacketType( void) const;


// (2009/11/19 - Seung-Won, Bae) 조회된 마지막 Data로 실시간 수신 대기할 구간값을 구한다.
//	이 구간값을 기준으로 실시간 수신시 마지막 Data에 Update할 것인지 Append할 것인지 결정된다.
//		특히 Append시에는 이전 구간을 마감시키는 Update이후 처리된다.
//		따라서 마지막 봉에 Update하기 위한 실시간 데이터의 시간범위를 구하는 것이다.
//	이 구간은 마지막 Data이전의 구간 시작기준시로 기본 구간을 구성하고, 마지막 Data가 실시간 Data인 것처럼 포함되는 구간으로 검색한다.
//	마지막 Data 자체가 없는 경우, 구간을 구성하지 않고, 첫 실시간이 들어올 때, 같은 위와 같은 방식으로 구한다.
//		물론 이렇게 구간이 구성되지 않은 상황에서는 허봉을 진행할 수 없으며,
//		첫 실봉 수신시는 무조건 Append로 처리후 구간을 구성한다.
protected:
	virtual BOOL	SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime);
					// (2009/10/22 - Seung-Won, Bae) 실시간에 봉을 구성하는 시간범위 끝값이 nEndTime이고, 실시간 값이 nCurTime일 경우, Append 상황인지 확인하는 Logic.
					//		각 처리 Logic Class 별로 다르게 처리된다.
	virtual bool	IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const = 0;


// (2009/10/22 - Seung-Won, Bae) Comment. 수신된 실시간의 올림/버림 값을 구하여 각 Logic Class에 따라 Packet에 반영 처리한다.
protected:
					// (2009/11/20 - Seung-Won, Bae) 주의! 조회데이터가 없는 경우 m_nTimeRangeStart, m_nTimeRangeEnd 값이 -1이다.
					//		사용전 확인이 필요하다.
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime);
protected:
						// (2009/10/22 - Seung-Won, Bae) 실시간 시간의 올림/버림값으로 실 Packet값을 갱신하고 신규봉 구성시 구간값을 다시 산정한다.
						//		Logic Class에 따라 Update/Append 확인용 시각, Update용 시각, 봉마감용 시각을 달리하여,
						//		SetRealData_AppendOrUpdate()를 통해 처리된다.
	virtual int			SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime) = 0;
						// (2009/10/22 - Seung-Won, Bae) nCheckTime을 기준으로 Update/Append 여부를 결정하고,
						//		실 Data로는 nUpdateTime을 반영한다. 봉마감시에는 nCloseTime으로 설정한다.
	int					SetRealData_AppendOrUpdate2( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nCheckTime, const __int64 &nUpdateTime);
						// (2009/10/27 - Seung-Won, Bae) 분봉 마감시 갱신시각을 조회한다.
	virtual __int64		GetCloseTime( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd) const	= 0;


protected:
	const CDateData& m_dateData; // 오늘 날짜

// (2009/10/27 - Seung-Won, Bae) the Flag for real after special code like market end.
protected:
	BOOL	m_bMarketOn;

// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
protected:
	virtual bool	GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime);
	virtual bool	GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime) = 0;
	bool			GetRealData_CloseTime_AppendOrUpdate2( CPacket* pPacket, const __int64 &nCheckTime, const __int64 &nUpdateTime, __int64 &p_nRealDataTime);
	BOOL			GetNextIntervalWithoutOffTime( __int64 &p_nTimeRangeStart, __int64 &p_nTimeRangeEnd, BOOL p_bWithoutOffTime = TRUE);
	void			GetPrevIntervalWithoutOffTime( __int64 &p_nTimeRangeStart, __int64 &p_nTimeRangeEnd, BOOL p_bWithoutOffTime = TRUE);
					// (2009/11/21 - Seung-Won, Bae) 지정된 구간이 장중인지 여부를 확인한다.
	BOOL			IsOnMarketTimeRange( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd, const int &nStartTime, const int &nEndTime, const int &nMarketStartTime, const int &nMarketEndTime);
	BOOL			IsOnMarketDay( __int64 p_nDateCheckTime);

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
protected:
	virtual __int64		GetTimeIntervalWithSecond( void);
};


// 09:01:00 ~ 09:01:59 -> 09:01 : 버림
class CFloorTimeRealDataModule: public CTimeRealDataModule  
{
public:
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CFloorTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);


// (2009/10/22 - Seung-Won, Bae) Comment. 분봉 올림 여부를 제공한다.
protected:
									//버림. 09:01:00 ~ 09:01:59 -> 09:01
	virtual CPacketType::REALTYPE	GetRealType( void) const	{	return CPacketType::FLOOR;	}


protected:
					// (2009/10/22 - Seung-Won, Bae) 실시간에 봉을 구성하는 시간범위 끝값이 nEndTime이고, 실시간 값이 nCurTime일 경우, Append 상황인지 확인하는 Logic.
					//		각 처리 Logic Class 별로 다르게 처리된다.
	virtual bool	IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const;


protected:
						// (2009/10/22 - Seung-Won, Bae) 실시간 시간의 올림/버림값으로 실 Packet값을 갱신하고 신규봉 구성시 구간값을 다시 산정한다.
						//		Logic Class에 따라 Update/Append 확인용 시각, Update용 시각, 봉마감용 시각을 달리하여,
						//		SetRealData_AppendOrUpdate()를 통해 처리된다.
	virtual int			SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime);
						// (2009/10/27 - Seung-Won, Bae) 분봉 마감시 갱신시각을 조회한다.
	virtual __int64		GetCloseTime( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd) const	{	return	p_nTimeRangeStart;	}


// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
protected:
	virtual bool	GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime);
};


// 09:00:01 ~ 09:01:00 -> 09:01 : 올림. 메리츠
class CCeilingTimeRealDataModule: public CTimeRealDataModule    
{
public:
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CCeilingTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);


// (2009/10/22 - Seung-Won, Bae) Comment. 분봉 올림 여부를 제공한다.
protected:
									// 올림. 09:00:01 ~ 09:01:00 -> 09:01
	virtual CPacketType::REALTYPE	GetRealType( void) const	{	return CPacketType::CEILING;	}


protected:
					// (2009/10/22 - Seung-Won, Bae) 실시간에 봉을 구성하는 시간범위 끝값이 nEndTime이고, 실시간 값이 nCurTime일 경우, Append 상황인지 확인하는 Logic.
					//		각 처리 Logic Class 별로 다르게 처리된다.
	virtual bool	IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const;


protected:
						// (2009/10/22 - Seung-Won, Bae) 실시간 시간의 올림/버림값으로 실 Packet값을 갱신하고 신규봉 구성시 구간값을 다시 산정한다.
						//		Logic Class에 따라 Update/Append 확인용 시각, Update용 시각, 봉마감용 시각을 달리하여,
						//		SetRealData_AppendOrUpdate()를 통해 처리된다.
	virtual int			SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime);
						// (2009/10/27 - Seung-Won, Bae) 분봉 마감시 갱신시각을 조회한다.
	virtual __int64		GetCloseTime( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd) const	{	return	p_nTimeRangeEnd;	}


// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
protected:
	virtual bool	GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime);
};

// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
// 09:00:00 ~ 09:00:59 -> 09:01 : 올림 (KOSCOM)
class CCeiling2TimeRealDataModule: public CCeilingTimeRealDataModule    
{
public:
	// 2011.01.21 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CCeiling2TimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);


// (2009/10/22 - Seung-Won, Bae) Comment. 분봉 올림 여부를 제공한다.
protected:
									// 올림. 09:00:00 ~ 09:00:59 -> 09:01
	virtual CPacketType::REALTYPE	GetRealType( void) const	{	return CPacketType::CEILING2;	}


protected:
					// (2009/10/22 - Seung-Won, Bae) 실시간에 봉을 구성하는 시간범위 끝값이 nEndTime이고, 실시간 값이 nCurTime일 경우, Append 상황인지 확인하는 Logic.
					//		각 처리 Logic Class 별로 다르게 처리된다.
	virtual bool	IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const;


protected:
						// (2009/10/22 - Seung-Won, Bae) 실시간 시간의 올림/버림값으로 실 Packet값을 갱신하고 신규봉 구성시 구간값을 다시 산정한다.
						//		Logic Class에 따라 Update/Append 확인용 시각, Update용 시각, 봉마감용 시각을 달리하여,
						//		SetRealData_AppendOrUpdate()를 통해 처리된다.
	virtual int			SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime);
						// (2009/10/27 - Seung-Won, Bae) 분봉 마감시 갱신시각을 조회한다.
	virtual __int64		GetCloseTime( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd) const	{	return	p_nTimeRangeEnd;	}


// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
protected:
	virtual bool	GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime);
};

#endif // !defined(AFX_TIMEREALDATAMODULE_H__AA27934F_EA58_40D3_960A_440C19829896__INCLUDED_)
