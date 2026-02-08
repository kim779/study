// RealModuleCenter.h: interface for the CRealModuleCenter class.
//
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (2009/10/19 - Seung-Won, Bae) 실시간 기본 원칙 재정리
//	1. 해외선물(WorldOn) 일자/시간 Data의 종류 : FX와 HTS는 모두 서버의 일자와 시간으로 통일됨.
//		조회 (일/주/월)							: 종목 영업일
//		조회 (분/틱)							: 거래소 일자/시간
//		실시간									: 종목 영업일 / 거래소 시간
//		장시작/종료 시간 (MARKETTIME)			: 거래소 시간 (일자없음)
//		현재시각 (DATATIME)						: 거래소 일자/시간
//		영업일자 (MARKETDAY)					: 종목 영업일
//		24시간 시세의 N분봉 구성기준 (BASETIME)	: 월드온에서는 사용안됨 (장시작시간이 기본이 될 듯)
//		허수봉 CLOCK 시간						: 서버(시카고) 일자/시간
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//	1. 실시간 처리 기본 원칙
//		: 실시간의 Update와 Add 여부는 조회된(또는 실시간 수신된) 마지막 시간값으로 산정한 Interval을 기준으로 처리한다.
//		1) 일/주/월봉은 마지막 봉과 비교하여 마지막 봉보다 과거 데이타인 경우 마지막 봉에 Update한다
//		2) 분봉의 경우 마지막 봉과 비교하여 마지막 봉보다 과거 데이타인 경우 마지막 봉에 Update한다.
//			만약 조회데이타에 YYYY 가 없는 경우는 월을 판단하여 12월->1월 인 경우는 미래로 간주한다
//		
//		=> 만약 이 원칙을 따르지 않고 날자가 바뀌면 과거,미래 관계없이 Append하고 싶은 경우는
//			_REAL_UPDATE_LOGIC_1  를 Define 하여 사용한다.
//		
//	2. 허수봉 처리
//		: 허수봉 시계가 발생시키는 시간은, 조회시점의 서버시간(없으면 수신된 자료끝시간)을 기준으로, 가상의 시간 Data를 발생시킨다.
//		1) 실제 시세가 발생하는 경우는 가상 시간 데이타의 시점을 실제 시세의 시간 데이타로 Update한다.
//			=> 실제 시세 시간과 PC에서 Tick으로 Count 하는 시간과의 오차가 있을 수 있으므로 시세 시간으로 맞춘다.
//		2) 허수봉 사용하는 봉 타입 : CScaleBaseData::HORZ_TIME_ONE, CScaleBaseData::HORZ_TIME_TWO, CScaleBaseData::HORZ_TIME_SECOND 
//		3) 장시작,장마감 시간과 현재시간이 서버에서 온 경우 Tick 시간이 장중이 아니면 허수봉 발생시키지 않는다.
//		4) Tick 시간이 봉을 만들어야할 시간보다 30초 지나면 허봉데이타를 실시간으로 보낸다.
//		5) 서버에서 현재시간 안주고 시세도 아직 안 온 경우는 허수봉 처리 안한다.
//			=> 허수봉을 생성한 근거 시간이 없으므로 발생시킬 수가 없다.
//			=> TR 데이타의 마지막 데이타를 기준으로 허수봉을 발생시키면, 
//				영업중이 아닐 때 조회를 한 경우는 과거 데이타로 허수봉 발생할 수 있으므로 안된다
//
//	3. 영업일 처리
//		1) 영업시간이 자정을 넘겨 이틀에 걸쳐있는 경우 전일일자를 영업일자로 할 것인지, 후일일자를 영업일자로 할 것인지 판단하여 처리
//			=> 판단방법 : 서버에서 내려주는 영업일자와 현재시간을 비교하여 계산한다
//				- 현재 시간이 장마감시간 이후인 경우 ( 이하 오후장 )
//						영업일 == 현재일 : 전일일자 사용(CPacketType::DAY_DOWN)
//						영업일 > 현재일 : 후일일자 사용(CPacketType::DAY_UP)	=> CME
//				- 현재 시간이 장마감시간 이전인 경우 ( 이하 오전장 )
//						영업일 == 현재일 : 후일일자 사용(CPacketType::DAY_UP)	=> CME
//						영업일 < 현재일 : 전일일자 사용(CPacketType::DAY_DOWN)
//		2) 실제 시세에 오는 날자는 거래소시간일자가 아니라 영업일자가 온다
//				- 일/주/월봉의 경우 이 일자를 그대로 사용하여 봉 구성
//				- 분봉의 경우 오후장이면서 영업일이 다음날자 사용인 경우 시세의 날자에서 -1 을 하여 처리하고,
//							  오전장이면서 영업일이 전일날자 사용인 경우 시세의 날자에서 +1 을 하여 처리한다.
//		3) 허수봉 생성시에서 거래소 시간이 아니라 영업일자로 데이타를 생성해야 한다
//				- 오후장이면서 영업일이 다음날자 사용인 경우 Tick으로 계산된 현재날자에서 +1 을 하여 실시간으로 보내고,
//				  오전장이면서 영업일이 전일날자 사용인 경우 Tick으로 계산된 현재날자에서 -1 을 하여 실시간으로 보낸다.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALMODULECENTER_H__36233F7E_0FC6_4D5A_9F9F_698C7F318819__INCLUDED_)
#define AFX_REALMODULECENTER_H__36233F7E_0FC6_4D5A_9F9F_698C7F318819__INCLUDED_

#include "PacketListModule.h"
#include "TimeRealDataModule.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartCtrl;
class CPacket;
class CRealModuleCenter
{
// 0. Constructor / Destructor
public:
	CRealModuleCenter( const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl, CTypedPtrList< CObList, CPacket*> * pPacketList = NULL);
	virtual ~CRealModuleCenter();


// (2009/10/19 - Seung-Won, Bae) comment.
//	Base Packet의 설정처리 - PacketList 최초 구성시 또는 AddPacket에 의하여 CPacketListImplementation::m_PacketList에
//		Packet이 추가되는 경우 C0_DATE_TIME Packet (자료일자)를 구하여 등록한다.
//	그외에는 별도의 CPacketList::SetBasePacketName()에 의해 별도 지정받기도 한다. (안쓰일듯.)
protected:
	CPacket *	m_pRealBasePacket; // real 기준 packet
public:
	void		SetRealBasePacket( CPacket* pPacket);


// (2009/10/19 - Seung-Won, Bae) Comment. 마지막 실시간 갱신된 DTI.
//		전체 Data보기에서 Update할 행 확인시 이용한다.
//		자료일자가 아닌 기준패킷에 의해 실시간이 들어오는 경우만 이용된다. (그것도 수치로된)
//		그외의 경우는 -1값으로 마지막 끝에 Update용으로 이용된다.
protected:
	int		m_nUpdateOrAppendIndex;		
public:
	void	SetUpdateOrAppendIndex( const int nUpdateOrAppendIndex);
	int		GetUpdateOrAppendIndex( void) const;


// (2009/10/19 - Seung-Won, Bae) Comment. 마지막 Real이 반영된 DTI Range?
//		지표 계산시 CMainChartFormulateImplementation::GetDataPoint()에서 조회한다.
//			(실제로 range로 이용되지 않으며 start로만 이용되는 것으로 판단된다.)
protected:
	CPoint	m_ptUpdateRange; // real 이 반영된 영역 (x:start, y:end)
public:
	void	SetUpdateRangeOffset( const int nStart, const int nEnd);
	void	SetPacketMinMaxAndUpdateRage(CPacket* pPacket);
	CPoint	GetUpdateRange( void) const;


// (2009/10/19 - Seung-Won, Bae) Comment. Real Data를 가공하는 Logic Class이다.
//		현재 RDC로만 이용되고 있음. ER도 있으나 RDC = 100 / ER 의 값으로 제공되는 경우일 뿐이므로 RDC로 통일이 바람직하다.
protected:
	CRealDataModule		m_realDataModule;
public:
						// 보조메시지로 RDC 비율값을 지정하는 Interface
	bool				SetRealDataModuleAndData( const bool bIsRemove, const CString& strModuleHelpMsg, const bool bIsExchangeRate_Percent);


// (2009/10/20 - Seung-Won, Bae) N틱을 위한 마지막봉에 들어간 Tick의 개수 (보조메시지 UTEC에 의해 지정됨)
protected:
	int		m_nAddTimeInterval; // Tick에서만 사용
public:
	void	SetAddTimeInterval( const int nData);


// (2009/10/19 - Seung-Won, Bae) Comment. UD 보조메시지값 (주기값)을 관리한다.
protected:
	CScaleBaseData::HORZSCALEDRAWERTYPE		m_eDateUnitType; // "UD" : 일,주,월,분,틱... 형태
public:
	void									SetDateUnitType( const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	CScaleBaseData::HORZSCALEDRAWERTYPE		GetDateUnitType( void) const;


// (2009/10/20 - Seung-Won, Bae) UT 값으로 1봉을 구성하기 위한 UD의 수량이다.
protected:
	int		m_nTimeInterval;	// "UT" : 봉하나의 기준
public:
	void	SetTimeInterval( const int nTimeInterval);
	int		GetTimeInterval( void) const;
	__int64 GetTimeIntervalWithSecond( void);


// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
public:
	CPacket *	m_pOpenPacket;
	CPacket *	m_pHighPacket;
	CPacket *	m_pLowPacket;
	CPacket *	m_pClosePacket;
	CPacket *	m_pVolumePacket;
public:
			// 실시간 Data를 추가 또는 Update 처리한다.
			// 특히 일중 거래량, 거래대금을 받는 경우, 추가시 이전 일중 값을 구하여 편차가 추가되도록 한다.
	bool	SetAppendOrUpdateData_DataInDate( CPacket* pPacket, const int nBaseCount, const double& dData, double& dPrevData);
			// 실시간 Data 추가시, Base Packet의 Data Count에 맞춰 부족한 부분을 이전 값으로 Append한 뒤에 Append 처리를 한다.
	bool	SetAppendData( CPacket* pPacket, const int nBaseCount, const double& dData, const double& dPrevData);
			// 고가와 저가의 Update를 처리한다. (현재 값보다 높거나 낮은 경우만 처리한다.)
	bool	SetUpdateData_Compare( CPacket* pPacket, const int nBaseCount, const double& dData, const bool bLowCheck);
			// 실시간 Data를 처리하는 Main Routine. (종가/자료일자/기준패킷/기타 로 구분되어 처리된다.)
	bool	SetRealData( CPacket* pPacket, const double dData, const int nOption, int &p_nRemovePacketDataCount);
			// 허봉을 위한 시간흐름을 전달한다. (허봉시각에 의해 봉이 추가되는지 여부를 Return한다.)
	bool	GetRealData_CloseTime( CPacket* pPacket, const __int64 &p_nClock, __int64 &p_nRealDataTime);


// (2009/10/20 - Seung-Won, Bae) Comment. 보조 메시지로 지정된 일자값을 관리한다.
//		실시간 수신시 실시간 Data의 일자값을 보정하기위하여 사용된다.
//		일간 주기의 영업일 기준일 경우의 처리는 어떻게 되지?
protected:
	CDateData	m_dateData;	// 오늘 날짜 (리얼처리시 PC날짜가 아닌 서버 날짜로 판단함)
public:
				// 지정된 설정값으로 일자로 초기화한다.
				//		보조메시지 DATE 값.
				//		보조메시지 DATATIME 값.
				//		보조메시지 DATATIME 값으로 받은 거래소 일자를 기준으로 재산정한 영업일.
				//				영업일은 지역시간 0시를 기준으로 바뀌며, 거래소 시간으로 일중에 변경된다.
				//				지역시간을 기준으로한 영업일은 일/주/월봉의 산출 기준이 된다.
	void		SetCurrentDate(const CString& strData);


// (2009/10/20 - Seung-Won, Bae) MARKETTIME 보조메시지로 설정된 장시작, 마감시간을 관리한다.
protected:
	CMarketTimeData		m_marketTimeData; // 장시작시간,장마감시간
public:
						// MARKETTIME 보조메시지로 수신된 장시작/마감을 설정한다.
	void				SetMarketTime( const CString& strData);
						// X ScaleManager의 다른 시간대의 Scale 처리를 위하여 장시작/마감시간을 조회한다.
	bool				GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const;
						// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
	void				GetRealData_RealDateTime( double &p_dData) const;


// (2009/10/20 - Seung-Won, Bae) Comment. 분봉 처리 Logic Class를 관리한다.
protected:
	CTimeRealDataModuleBase *	m_pTimeRealDataModule;
public:
							// 분봉 처리 Logic Type을 구한다.
	CPacketType::REALTYPE	GetRealType( void) const;
							// 분봉의 미래/과거 영역을 추론하기 위하여 마지막봉 구간 시작시간을 조회한다.
	__int64					GetStartTimeInTimeRangeOfLastData( void);


// (2009/10/21 - Seung-Won, Bae) Comment. 서로 다른 시간대를 비교하기 위한 기준으로,
//		현재 거래소의 일자/시각을 관리한다. (조회시점인가?)
protected:
	int			m_nBasisTime;
public:
	void		SetCurrDateTime(const CString& strData);	// TR 조회 일자+시간 ( "YYYYMMDDHHMMSS" )
	__int64		GetCurrDateTime() const;					// TR 조회 일자+시간 ( "YYYYMMDDHHMMSS" )


// (2009/10/21 - Seung-Won, Bae) Comment. 1일장이 장이 0시를 넘어가면서 진행하는지를 나타내는 Type을 관리한다.
protected:
	CPacketType::MARKETDAYTYPE	m_nMarketDayType;
public:
	CPacketType::MARKETDAYTYPE	GetMarketDayType();


// (2009/10/21 - Seung-Won, Bae) Comment. 실시간 Data의 Packet Type을 관리한다.
protected:
	CString		m_strRealTimeType;	//실시간데이타에 날자+시간의 Data Type(default는 HHMMSS)
public:
	void		SetRealTimeType(const CString& strData);	//실시간데이타 패킷의 날자 데이터 포맷
	CString		GetRealTimeType( void)						{	return m_strRealTimeType;	}


// (2009/10/21 - Seung-Won, Bae) Comment. 실시간 처리를 위한 환경값을 초기화한다.
public:
			// 보조메시지 설정 전 초기화
	void	InitializeHelpMessage( void);
			// 보조메시지 및 TR Data 설정후 초기화
	void	InitializeTRPart( CPacket *p_pOpenPacket, CPacket *p_pHighPacket, CPacket *p_pLowPacket, CPacket *p_pClosePacket, CPacket *p_pVolumePacket);


// (2009/10/22 - Seung-Won, Bae) Comment. X Scale Manager에서 시간 Merge에 참고하도록 분봉 올림 여부를 제공한다.
protected:
	CPacketType::REALTYPE	m_eRealType;

// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
protected:
	CChartInfo::CHART_MODE	m_nChartModeSetting;
public:
	CChartInfo::CHART_MODE	GetChartMode( void) const		{	return m_nChartModeSetting;		}
							// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
	void					ChangeChartMode( int p_nChartMode)	{	m_nChartModeSetting = ( CChartInfo::CHART_MODE)p_nChartMode;	}


// (2009/11/1 - Seung-Won, Bae) for Normalized Candle data.
public:
	double		GetOrgDataBeforeRate( const char *p_szPacketName, const double p_dRatedData);


// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
protected:
	int			m_nOrgBaseTimeDiff;
	int			m_nBaseTimeDiff;
public:
	void		SetTimeDiffInfo( const int &p_nOrgBaseTimeDiff, const int &p_nBaseTimeDiff);


// (2009/11/1 - Seung-Won, Bae) Support Last Real Time.
protected:
	__int64		m_nClock;
	__int64		m_nLastClockTime;
	__int64		m_nLastRealTime;
protected:
	void		SetLastRealTime( const __int64 &p_nRealTime);

// (2009/11/4 - Seung-Won, Bae) Backup to receive NMTR Data.
protected:
	IChartCtrl *	m_pIChartCtrl;
protected:
	BOOL		m_bOnClockedData;
	__int64		m_nLastRealTimeBackup;
public:
	void		PushNMTRState( __int64 p_nRealDataTime);
	void		PopNMTRState( void);
				// (2009/11/12 - Seung-Won, Bae) Get NMTR Info
	BOOL		GetLastClockAndRealTime( __int64 &p_nLastClockTime, __int64 &p_nLastRealTime);

// (2009/11/16 - Seung-Won, Bae) Base Time of N-Minite
protected:
	int			m_nNewRangeStartTime;
public:
	void		SetNewRangeStartTime( const CString &p_strTime);

// (2009/11/16 - Seung-Won, Bae) Manage Market Days
protected:
	CString		m_strMarketDayInfo;
public:
	void		SetMarketDayInfo( const CString &p_strMarketDayInfo)	{	m_strMarketDayInfo = p_strMarketDayInfo;	}
};

#endif // !defined(AFX_REALMODULECENTER_H__36233F7E_0FC6_4D5A_9F9F_698C7F318819__INCLUDED_)
