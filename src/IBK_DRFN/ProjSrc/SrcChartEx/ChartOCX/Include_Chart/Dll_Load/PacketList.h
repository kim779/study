// PacketList.h: interface for the CPacketList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../BlockBaseData.h"

#include "PacketBase.h"

interface IChartCtrl;
class CPacketListImplementation;
class AFX_EXT_CLASS CPacketList  
{
public:	
	CPacketList( const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl);
	CPacketList( CString strPacketOption, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl);
	virtual ~CPacketList();

	void SetVersion(double dVersion);
	void SetPacketLengthCount(int nLength);
	bool SetData(const CString szPacketName, int nIndex, double dValue, bool IsByReal);

	CPacketType::REALTYPE GetRealType() const;

	// (2004.06.18, 배승원) Data Byte수를 Long으로 조정한다.
	void SetPacketData(const CString& szPacketName, long pData, long lCount);

	bool SetPacketData(const CString& strPacketName, const CString& strPacketType, 
		const char* pPacketData, const int nCount);
	void AddPacket(const CString szPacketOption);
	bool RemovePacket(const CString szPacketName);

	// (2009/10/19 - Seung-Won, Bae) Comment. 마지막 실시간 갱신된 DTI.
	//		전체 Data보기에서 Update할 행 확인시 이용한다.
	int		GetLastestRealIndex();

	//	시간 간격 리턴
	int GetTimeInterval();
	__int64 GetTimeIntervalWithSecond( void);
	// (2006/7/11 - Seung-Won, Bae) Retrived the Next First Time of Data.
	__int64	GetStartTimeInTimeRangeOfLastData( void);
	int	GetEndTimeInTimeRangeOfLastData( void);
	//  TR 받은 갯수
	int GetTotalTRIndex() const;
	//  전체 TR data count
	int GetTotalTRCount() const;
	//	부분 TR data count
	int GetTRCount(const int nTRIndex) const;
	//  TR 발생할 count
	int GetRequestTRCount(const int nCurTRCount) const;
	//	데이타중 가장 긴 데이타의 갯수를 리턴
	int GetMaxCount();
	//해당 packet의 count
	int GetCount(const CString strPacketName);
	//	Packet이 가지고 있는 데이타갯수중 가장 많은 데이타 갯수를 리턴한다
	//	strPacketList : Packet의 list
	// (2004.06.25, 배승원) Packet 명의 구분자를 전달받게 한다. (기본값은 이전에 고정되었던 ',')
	int GetMaxCount(CString strPacketNameList, const CString& strCompartName = ',');
	//	여러 Packet에서 Minimun & Maximun Value를 Join한다
	//	nStart : 시작데이타 위치
	//	nEnd : 끝데이타 위치
	//	strPacketNameList : Join할 Packet의 List(packet1,packet2,Packet3,...)
	//	dMin : Minimum
	//	dMax : Maximun
	int GetMinMax(int nStart, int nEnd, CString strPacketNameList, double& nMin, double& nMax);
	bool GetAllPacketsMinMax(const int nStart, const int nEnd, double& dMin, double& dMax);

	CList<double, double> * GetnumericDataList(CPacket * pPacket);
	CList<CString, CString> * GetstrDataList(CPacket * pPacket);

	//nami 8.29
	CString GetPacketType(CString strPacketName, const bool bIsFullType = true);
	CString GetPacketType(CPacket * pPacket, const bool bIsFullType = true);

	CString GetStrDataFromIndex(CString strFunction, int nIndex);
	double GetDataFromIndex(CString strFunction, int nIndex);

	double GetCurrentDate();
	//	시간(날짜) 타입 리턴
	CScaleBaseData::HORZSCALEDRAWERTYPE GetDateUnitType();
	//	data update타입 리턴 (NEW, INSERT, APPEND)
	CPacketDataInfo::DATAINSERTTYPE GetDataInsertType();
	// 04.16.2002  -> 종목명 저장용
	CList<CString, CString>* GetRealKeyName();
	CPacketType::PRICETYPE GetPriceType();	

	bool UseRealData();
	bool GetUseReal();
	// 04.17.2002  -> 기저장된 종목명인지 체크
	bool FindKey(CString strKey);

	int GetEndDate(CPacket* pDatePacket = NULL);
	int GetStartDate();
	//	data update할 영역을 리턴
	CPoint GetUpdateRange();
	//	서버에서 받아야 할 패킷에서 한 레코드의 길이를 리턴한다
	int GetPacketDataLength(int nIndex);
	//	Pakcet Name or Packet Field Name or Packet list Name 으로 들어온 내용으로 Packet을 찾아서 리턴한다
	CPacket * GetBaseData(CString strFunction, const bool bCheckCurrentPacket = true);	
	bool GetPacketList(const CString& strPacketNames, const CString& strCompart, CList<CPacket*, CPacket*>& packetList) const;
	CTypedPtrList<CObList, CPacket*>& GetTRPacketList();
//	CTypedPtrList<CObList, CPacket*>& GetIndicatorPacketList();
	CString GetTRPacketNames(const CString& strCompart = ";") const;

	//sy 2004.07.26.
	void SetUseRealData(bool bUseRealData);
	//	data update가 끝났음을 setting
	void SetHoldingPacketSize(const int nSize);
	void SetBasePacketName(const CString &szPacketName);
	//	data update타입 setting  (NEW, INSERT, APPEND)
	void SetDataInsertType(CPacketDataInfo::DATAINSERTTYPE nDataInsertType);

	//	Server에서 내려온 data를 받아서 저장한다. 저장 데이타의 주소를 받아오는 형식
	int SetBaseData(long pData, int nCount, int nType);	
	//	Server에서 내려온 data를 받아서 저장한다. 저장 데이타를 string으로 받아오는 형식
	int SetBaseData(CString szData, int nCount, int nType);
/*
	CPacket* SetBaseData(const CString& szName, const CString& szType, const CString& szExpressionType, 
					     CList<double, double>& lsData, 
						 const int nStartPeriod, CPacketDataInfo::DATAINSERTTYPE nInsertType, 
						 CPoint ptUpdateRange);

	CPacket* SetBaseData(const CString& szName,
						 const CString& szType,
						 const CString& szExpressionType,
						 CList<CString, CString>& lsData, 
						 const int nStartPeriod,
						 CPacketDataInfo::DATAINSERTTYPE nInsertType, 
						 CPoint ptUpdateRange);
*/

	//	생성해야 할 기본 데이타 Packet항목을 받아서 Packet List를 생성한다
//	void SetOption(CString strPacketOption);

	bool RemoveFictionData();
	bool ClearIndicatorData(const CString& strName);
	//  Packet List에서 가지고 있는 모든 Packet의 data list를 clear한다
	//  bClear가 false면 지표데이터만 삭제
	//  true면 기본데이터하고 지표데이터 모두 삭제
	int ClearDataAll(const bool bClearAllData = true);
	//  Packet Name에 있는 Packet의 data list를 clear한다
	int ClearData(const CString& strPacketNames, const CString& strCompartName = ',', const bool bClearIncludingTR = true);
	bool ClearAllIndicatorData(const bool bOnlyData);
	bool DeleteOrClearPacket(const CString& strPacketName, const bool bIsDelete = true, const bool bIsOnlyNoTRData = true);
	bool DeleteOrClearPacket(CPacket* pPacket, const bool bIsDelete = true, const bool bIsOnlyNoTRData = true);

	//sy 2003.3.19.
	void SetItemData(const CString& strCode, const CString& strName);
	void GetItemData(CString& strCode, CString& strName) const;
	CString GetItemCode() const;
	CString GetItemName() const;

	//sy 2004.07.16.
	int GetRemovePacketCountInRealData() const;

	//종가 packet의 장구분.
	CPacketType::COMMODITY_BOND_NAME GetCommodityBondNameInBase() const;

	//보조메시지 (서버에서 받는 항목으로 각 항목별로 받을 수 있다)
	bool SetHelpMessage(const CString& strHelpMessage, const bool bIsAllRemove);
	bool SetHelpMessageRealDataRate(const CString& strHelpMessage, const bool bIsRemove);

private:
	CPacketListImplementation* m_pPacketListImple;

// (2004.09.14, 배승원) 보조 Message Item을 조회하는 Interface를 관리한다.
public:
	BOOL						GetHelpMessageItem( const char *p_szMsgKey, CString &p_strMsgValue);
	// (2005/9/29 - Seung-Won, Bae) Get Help Message List
	const CMapStringToString *	GetHelpMessageMap( void);

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
public:
	BOOL	SetBinTrHelpMsg( const char *p_szHelpMessage);
	BOOL	SetBinTrDoubleData( const char *p_szPacketName, long p_nCount, double FAR* p_pdData);
	BOOL	SetBinTrStringData( const char *p_szPacketName, long p_nCount, const char **p_pszData);
	BOOL	SetBinTrEnd( void); 

	// 자신의 RQ : 복수종목 - ojtaso (20070117)
	void	SetRQ(LPCTSTR lpszRQ);
	LPCTSTR GetRQ();

	// 현재가 등락률 - ojtaso (20080602)
	void	OffsetPacketData(double dData, CString strPacketName = _T(""));

// (2008/2/4 - Seung-Won, Bae) for Other Meta of MultiLanguage
public:
	CString		GetPacketOption( void);

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
public:
	BOOL	ShiftPacket( LPCTSTR p_szPacketNames, long p_szShiftCount);


// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
public:
	CString		GetBaseTimeName();
	void		SetBaseTimeName(CString strBaseTimeName);
	int			GetBaseTimeDifference();
	void		SetBaseTimeDifference(int nBaseTimeDiff);
	int			GetCalcDataTimeDifference();
	void		SetBaseTime(CString strBaseTimeName, int nBaseTimeDiff);

	int			GetOrgBaseTimeDifference();
	void		SetOrgBaseTime();

	__int64		GetCurrDateTime() const;
	bool		GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const;

	void		CalcPacketTimeDiff(int nBaseTimeDiff);

//	CPacket*	GetDatePacket();

	// 20081024 JS.Kim	허수봉을 ocx 및 AddIn 으로 주기위해 허수봉의 시간을 실봉시간으로 환산한다.
	bool		CalcNMTimeToRealTime(CPacket *pPacket, const double& dTime, double& dRealTime, double& dCloseData);
	// 20081029 JS.Kim	영업일 처리 기준을 구한다.
	CPacketType::MARKETDAYTYPE GetMarketDayType();

// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
public:
	bool	SetRealData( const char *p_szPacketName, const double dData, int &p_nRemovePacketDataCount, const int nOption = 0);
	bool	GetRealData_CloseTime( const __int64 &p_nClock, __int64 &p_nRealDataTime);
	void	PushNMTRState( const __int64 &p_nRealDataTime);
	void	PopNMTRState( void);
			// (2009/11/12 - Seung-Won, Bae) Get NMTR Info
	BOOL	GetLastClockAndRealTime( __int64 &p_nLastClockTime, __int64 &p_nLastRealTime);

// (2009/10/21 - Seung-Won, Bae) Comment. 실시간 Data의 Packet Type을 관리한다.
public:
	CString		GetRealTimeType( void);						//실시간데이타 패킷의 날자 데이터 포맷

// (2009/11/1 - Seung-Won, Bae) for Normalized Candle data.
public:
	double		GetOrgDataBeforeRate( const char *p_szPacketName, const double p_dRatedData);

// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
public:
	void		GetRealData_RealDateTime( double &p_dData);

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
public:
	void		ChangeChartMode( int p_nChartMode);
};

#endif // !defined(AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_)
