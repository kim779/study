// PacketListImplementation.h: interface for the CPacketListImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLISTIMPLEMENTATION_H__356283A1_90B9_409D_992E_3A81BB4F1E5C__INCLUDED_)
#define AFX_PACKETLISTIMPLEMENTATION_H__356283A1_90B9_409D_992E_3A81BB4F1E5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"
#include "PacketBase.h"
#include "RealModuleCenter.h"
#include "TimeRealDataModule.h"
#include "PacketListModule.h"
#include "PacketBaseData.h"

interface IChartCtrl;
class CPacket;
class CPacketList;
class CPacketListImplementation  
{
public:	
	CPacketListImplementation(CPacketList* pPacketList, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl);
	CPacketListImplementation(CPacketList* pPacketList, CString strPacketOption, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl);
	virtual ~CPacketListImplementation();

	void SetVersion(double dVersion);
	void SetPacketLengthCount(int nLength);
	bool SetData(const CString szPacketName, int nIndex, double dValue, bool IsByReal);

	CPacketType::REALTYPE GetRealType() const;

	// (2004.06.18, 배승원) Data Byte수를 Long으로 조정한다.
	void SetPacketData(const CString& szPacketNames, long pData, long lCount);

	bool SetPacketData(const CString& strPacketName, const CString& strPacketType, 
		const char* pPacketData, const int nCount);
	CString AddPacket(const CString szPacketOption);
	bool RemovePacket(const CString szPacketName);
	int GetLastestRealIndex();

	//	시간 간격 리턴
	int GetTimeInterval();
	__int64 GetTimeIntervalWithSecond( void);
	// (2006/7/11 - Seung-Won, Bae) Retrived the Next First Time of Data.
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가 <<
	//int GetStartTimeInTimeRangeOfLastData( void);
	__int64	GetStartTimeInTimeRangeOfLastData( void);
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
	int GetMaxCount(CString strPacketNameList, const CString& strCompartName);
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
	CList<CString, CString>* GetRealKeyName()	{return &m_lstRealKeyName;}
	CPacketType::PRICETYPE GetPriceType()	{ return m_nPriceType;}

	bool UseRealData()	{return m_bUseRealData;}
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
	CPacket * GetBaseData(const CString& strPacketName, const bool bCheckCurrentPacket = true);
	bool GetPacketList(const CString& strPacketNames, const CString& strCompart, CList<CPacket*, CPacket*>& packetList) const;
	bool GetPacketList(const CString& strPacketNames, const CString& strCompart, const bool bIsOnlyBaseTRData, CTypedPtrList<CObList, CPacket*>& packetList) const;
	CTypedPtrList<CObList, CPacket*>& GetTRPacketList();
//	CTypedPtrList<CObList, CPacket*>& GetIndicatorPacketList();
	CString GetTRPacketNames(const CString& strCompart) const;

	//sy 2004.07.26.
	void SetUseRealData(bool bUseRealData)	{m_bUseRealData = bUseRealData;}
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
	//	새로운 data list로 새 Packet을 만든다. Packet의 이름과 data list가 함께 주어질때 사용
	CPacket * SetBaseData(const CString& szName,
						  const CString& szType,
						  const CString& szExpressionType,
						  CList<double, double>& lsData,
						  const int nStartPeriod = 0, 
						  CPacketDataInfo::DATAINSERTTYPE nInsertType = CPacketDataInfo::NEW,
						  CPoint ptUpdateRange = CPoint(-1,-1));
	CPacket * SetBaseData(const CString& szName,
					      const CString& szType,
						  const CString& szExpressionType,
						  CList<CString, CString>& lsData,
						  const int nStartPeriod = 0,
						  CPacketDataInfo::DATAINSERTTYPE nInsertType = CPacketDataInfo::NEW,
						  CPoint ptUpdateRange = CPoint(-1,-1));
*/

	//	생성해야 할 기본 데이타 Packet항목을 받아서 Packet List를 생성한다
//	void SetOption(CString strPacketOption);

	bool RemoveFictionData();
	bool ClearIndicatorData(const CString& strName);
	//  Packet List에서 가지고 있는 모든 Packet의 data list를 clear한다
	void Initialize_Packet(const bool bClearIncludingTR = true);
	int ClearDataAll(const bool bClearIncludingTR = true);
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
	int SetHelpMessage( const char *p_szHelpMessage, const bool bIsAllRemove);
	bool SetHelpMessageRealDataRate(const CString& strHelpMessage, const bool bIsRemove);

 private:
	void DeleteData(CTypedPtrList<CObList, CPacket*>* lstPacket);

	bool GetPacketTypeInInfo(const CString& strInfo, CString& strName, CString& strType, int& nLength) const;
	int GetPacketDataLength(int nIndex, CTypedPtrList<CObList, CPacket*>* lstPacket);
	int GetPacketDataLength(const CString &szPacketName);
	CPacket* GetRealPacket(const CString& strRealFieldName, const CString& strRealKeyName);
	CPacket* GetBaseDataInCurrentInCompart(const CString& strPacketName, const CString& strCompart) const;
	CPacket* GetBaseDataInCurrent(const CString& strPacketName, const bool bIsOnlyTRData = false) const;
	CPacket* GetPacketInTRData(const CString& strPacketName, const bool bIsOnlyBaseTRData) const;
	CPacket* GetPacket(const CString& strPacketName, const CTypedPtrList<CObList, CPacket*>& packetList) const;
	CPacket* GetBaseDataInNew(const CString& strPacketName);

	//code로 장구분.
	CPacketType::COMMODITY_BOND_NAME GetCommodityBondNameInCode() const;

	int SetBaseDataNewPacket(long pData, int nCount);
	int SetBaseData(long pData, int nCount, CTypedPtrList<CObList, CPacket*>* lstPacket);
	void SetPacketData(long pData, long lCount, const int nDataCount, const bool bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList);
	int SetBaseData(long pData, const int nbozolen, const int nRowCount, const bool bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList);
	bool SetErrorMessage(const CString& strMessage);
	void SetBoundLimit(const CString& strData);
	void SetBoundLimit(double nUpperBound, double nLowerBound, const CString &strPacketName);

	void SetDrawStartIndex(const CString& strInfo);
	void SetItemData(const CString &strData);
	void SetCurrentDate();
	void SetLength(CPacket * pPacket);
	void SetPrevDatas(const CString& strData);
	void SetPrevData(CString& strData);

	// (2007/2/7 - Seung-Won, Bae) for Exception Value
	void	SetExceptionValues( const CString &strData);
	// (2007/2/22 - Seung-Won, Bae) for Continue Code
	void SetContinueCodes( const CString &strData);

	void SetCommodityBondName(const CString& strData);

	int ResetTimeType(CString strTimeType);
	//	보조메세지 내용 setting
	void SetHelpMessageInitialize( const char *p_szHelpMessage);
	int SetHelpMessage(const CString& strInfo);
	int SetDataInsertTypeInfo(const CString& strData/*, const bool bIsBasePacket*/);
	void Initialize_PacketData();
	void Initialize_DataInsertType();
	void Initialize_HelpMessage();
	void SetDateUnitType(const CString& strData);
	// 02.19.2002  패킷 길이를 재 세팅한다
	bool ResetPacketInfo(const CString& strPacketInfo);
	bool SetPacketInfo(const CString& strPacketInfo);
	// 03.11.2002  새 패킷을 생성한다(보조메세지에 의해서)
	bool SetNewPacket(const CString& strPacketInfo);

//	CPacket * AddIndicatorData(CString strFuntion, CString strCondition);
	
	// (2006/6/5 - Seung-Won, Bae) return Deleted Data Count
	int	RemovePacketList(int nStart, int nEnd, int nType);

	bool DeletePacketList(CTypedPtrList<CObList, CPacket*>* lstPacket);
	// 02.19.2002  일부 패킷만 clear한다 (데이타 길이가 0이 아닌 패킷)
	bool PacketClearPart();

	bool GetAllPacketsMinMax(CTypedPtrList<CObList, CPacket*>& packetList, const int nStart, const int nEnd, double& dMin, double& dMax);
	bool FindPosition(CPacket* pPacket, CTypedPtrList<CObList, CPacket*>*& pPacketList, POSITION& pos);

	bool SetPacketData(CPacket* pPacket, const char* pPacketData, const int nCount);

	void AppendAllDataInPacket(CPacket* pPacket, char* szData, const bool bClearData, 
			const int nAllPacketLength, const int nRowCount, int& nDataTotalIndex);
	void InsertAllDataInPacket(CPacket* pPacket, char* szData, const int nAllPacketLength, 
			const int nRowCount, int& nDataTotalIndex);

	void Initialize_BaseDataBefore(const bool bIsBasePacket);
	void Initialize_BaseDataAfter(const bool bIsBasePacket, const int nRowCount, CTypedPtrList<CObList, CPacket*>* pPacketList);
	void SetPecketBaseData(CTypedPtrList<CObList, CPacket*>* pPacketList);

private:
	CPacketList* m_pPacketList;
	double m_dVersion;
	int m_nHoldingPacketSize;
	int	m_nPacketLengthCount;
	int m_nMaxDataCount;
	double m_dCurrentDate;

	//sy 2005.11.14. -> TR 조회한 data 갯수
	CTRDataCountList m_trDataCountList;
	

	// 04.17.2002  -> 종목명 저장용(관심에 등록을 위한 종목명)
	CList<CString, CString> m_lstRealKeyName;
	CTypedPtrList<CObList, CPacket*> m_PacketList;
	CTypedPtrList<CObList, CPacket*> m_IndicatorDataList;
	CTypedPtrList<CObList, CPacket*> m_AdditionList;

	//sy 2005.09.30.
	CRealModuleCenter m_realCenter;

private:
	// 보조메시지나 메소드로 들어오는 data
	CPacketDataInfo::DATAINSERTTYPE m_nDataInsertType;
	CPacketType::PRICETYPE m_nPriceType;
	bool m_bUseRealData;
	//sy 2003.3.19. -> 종목코드와 종목명
	CItem m_itemData;

	// 자신의 RQ : 복수종목 - ojtaso (20070117)
	LPCTSTR	m_lpszRQ;
	
private:
	bool SetPacketData_HelpMsg(const CString& szPacketName, long pData, long lCount, 
			int& nDataCount, bool& bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList);
	bool SetPacketData(CTypedPtrList<CObList, CPacket*>& packetList, char* szData, const int nRowCount);
	int GetRowCount(long pData) const;
	void GetHelpMessageData(long pData, int& nDataCount, CString& strHelpMsgData) const;

	void SetMaxDataCount();
	void AddTRDataCount(const int nCount);


// (2004.09.14, 배승원) 보조 Message Item을 범용적으로 관리하여 조회 이용할 수 있도록한다.
protected:
	CMapStringToString 		m_mapHelpMessage;
public:
	BOOL GetHelpMessageItem( const char *p_szMsgKey, CString &p_strMsgValue);
	// (2005/9/29 - Seung-Won, Bae) Get Help Message List
	const CMapStringToString *	GetHelpMessageMap( void)	{	return &m_mapHelpMessage;	}

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
protected:
	long									m_lBinTrRowCount;
	BOOL									m_bBinTrIsBasePacket;
	CTypedPtrList< CObList, CPacket *> *	m_pBinTrPacketList;
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

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;

// (2008/2/4 - Seung-Won, Bae) for Other Meta of MultiLanguage
protected:
	CString		m_strPacketOption;
public:
	CString		GetPacketOption( void)		{	return m_strPacketOption;	}

// (2009/1/29 - Seung-Won, Bae) for 92000 TEST
//protected:
//	int			m_nI;
//	long		m_lTime;

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
public:
	BOOL	ShiftPacket( LPCTSTR p_szPacketNames, long p_szShiftCount);

// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
protected:
	int			m_nOrgBaseTimeDiff;

	CString		m_strBaseTimeName;
	int			m_nBaseTimeDiff;
public:
	void		SetOrgBaseTimeDifference(const CString& strData);
	int			GetOrgBaseTimeDifference();

	void		SetBaseTimeName(CString strBaseTimeName);
	CString		GetBaseTimeName();

	void		SetBaseTimeDifference(const int nTimeDiff);
	void		SetBaseTimeDifference(const CString& strData);
	int			GetBaseTimeDifference();			// 사용자가 선택한 현재 시간 구하기
	int			GetCalcDataTimeDifference();		// Packet에 Data 넣을 때 사용할 사용자시간-원래시간 계산한 값 구하기

	__int64		GetCurrDateTime() const;
	bool		GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const;
	
	void		CalcPacketTimeDiff(int nBaseTimeDiff);

	CPacket*	GetDatePacket();
	void		CalcMarketTime();

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

// 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
public:
	void		SetRedefinePacketName(const CString& strData);
	CPacket*	GetRedefinedPacket(const CString& strPacketName, const CTypedPtrList<CObList, CPacket*>& packetList) const;

	// (2011/12/09 - Junok, Lee) add function to get default chart bozo message value.
	BOOL GetDefaultBozoValue(int p_nChartMode, LPCSTR p_szKey, CString &p_rValue);

	CDRDebugerLoader *m_pDRDebuger;
	BOOL _GetDRDebuger();	
};

#endif // !defined(AFX_PACKETLISTIMPLEMENTATION_H__356283A1_90B9_409D_992E_3A81BB4F1E5C__INCLUDED_)
