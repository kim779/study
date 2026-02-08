// PacketList.cpp: implementation of the CPacketList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketList.h"

#include <math.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "Conversion.h"
#include "PacketBase.h"
#include "PacketListImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXCOUNT 1024//2048
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPacketList::CPacketList(const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl)
{
	// (2008/1/20 - Seung-Won, Bae) for MultiLanguage.
	g_iMetaTable.LoadMetaTable();

	m_pPacketListImple = new CPacketListImplementation(this, eRealType, p_pIChartCtrl);
}

CPacketList::CPacketList(CString strPacketOption, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl)
{
	// (2008/1/20 - Seung-Won, Bae) for MultiLanguage.
	g_iMetaTable.LoadMetaTable();

	m_pPacketListImple = new CPacketListImplementation(this, strPacketOption, eRealType, p_pIChartCtrl);
}

CPacketList::~CPacketList()
{
	if(m_pPacketListImple)
		delete m_pPacketListImple;
}

int CPacketList::SetBaseData(long pData, int nCount, int nType)
{
	return m_pPacketListImple->SetBaseData(pData, nCount, nType);
}

int CPacketList::SetBaseData(CString szData, int nCount, int nType)
{
	return m_pPacketListImple->SetBaseData(szData, nCount, nType);
}

/*
CPacket * CPacketList::SetBaseData(const CString& szName,
								   const CString& szType, 
								   const CString& szExpressionType, 
								   CList<double, double>& lsData, 
								   const int nStartPeriod, CPacketDataInfo::DATAINSERTTYPE nInsertType, 
								   CPoint ptUpdateRange)
{
	return m_pPacketListImple->SetBaseData(szName, szType, szExpressionType, lsData,
										   nStartPeriod, nInsertType, ptUpdateRange);

}

// 03.22.2002  문자형 데이타를 받는다
// 공식에서 호출하는 함수.
CPacket * CPacketList::SetBaseData(const CString& szName,
								   const CString& szType,
								   const CString& szExpressionType,
								   CList<CString, CString>& lsData, 
								   const int nStartPeriod,
								   CPacketDataInfo::DATAINSERTTYPE nInsertType, 
								   CPoint ptUpdateRange)
{
	return m_pPacketListImple->SetBaseData(szName, szType, szExpressionType, lsData,
										   nStartPeriod, nInsertType, ptUpdateRange);
}
*/

CPacket * CPacketList::GetBaseData(CString strFunction, const bool bCheckCurrentPacket)
{
	return m_pPacketListImple->GetBaseData(strFunction, bCheckCurrentPacket);
}

bool CPacketList::GetPacketList(const CString& strPacketNames, const CString& strCompart, CList<CPacket*, CPacket*>& packetList) const
{
	return m_pPacketListImple->GetPacketList(strPacketNames, strCompart, packetList);
}

/*
//	생성해야 할 기본 데이타 Packet항목을 받아서 Packet List를 생성한다
void CPacketList::SetOption(CString strPacketOption)
{
	m_pPacketListImple->SetOption(strPacketOption);
}
*/

//	여러 Packet에서 Minimun & Maximun Value를 Join한다
//	nStart : 시작데이타 위치
//	nEnd : 끝데이타 위치
//	strPacketNameList : Join할 Packet의 List(packet1,packet2,Packet3,...)
//	dMin : Minimum
//	dMax : Maximun
int CPacketList::GetMinMax(int nStart, int nEnd, CString strPacketNameList, double& nMin, double& nMax)
{
	return 	m_pPacketListImple->GetMinMax(nStart, nEnd, strPacketNameList, nMin, nMax);
}

bool CPacketList::GetAllPacketsMinMax(const int nStart, const int nEnd, double& dMin, double& dMax)
{
	return 	m_pPacketListImple->GetAllPacketsMinMax(nStart, nEnd, dMin, dMax);
}

int CPacketList::ClearDataAll(const bool bClearAllData)
{
	return m_pPacketListImple->ClearDataAll(bClearAllData);
}

int CPacketList::ClearData(const CString& strPacketNames, const CString& strCompartName, const bool bClearIncludingTR)
{
	return m_pPacketListImple->ClearData(strPacketNames, strCompartName, bClearIncludingTR);
}

bool CPacketList::ClearAllIndicatorData(const bool bOnlyData)
{
	return m_pPacketListImple->ClearAllIndicatorData(bOnlyData);
}

bool CPacketList::DeleteOrClearPacket(const CString& strPacketName, const bool bIsDelete, const bool bIsOnlyNoTRData)
{
	return m_pPacketListImple->DeleteOrClearPacket(strPacketName, bIsDelete, bIsOnlyNoTRData);
}

bool CPacketList::DeleteOrClearPacket(CPacket* pPacket, const bool bIsDelete, const bool bIsOnlyNoTRData)
{
	return m_pPacketListImple->DeleteOrClearPacket(pPacket, bIsDelete, bIsOnlyNoTRData);
}

bool CPacketList::ClearIndicatorData(const CString &strName)
{
	return m_pPacketListImple->ClearIndicatorData(strName);
}

CList<double, double> * CPacketList::GetnumericDataList(CPacket *pPacket)
{
	return m_pPacketListImple->GetnumericDataList(pPacket);
}

CList<CString, CString> * CPacketList::GetstrDataList(CPacket *pPacket)
{
	return m_pPacketListImple->GetstrDataList(pPacket);
}

// (2004.06.25, 배승원) Packet 명의 구분자를 전달받게 한다. (기본값은 이전에 고정되었던 ',')
int CPacketList::GetMaxCount(CString strPacketNameList, const CString& strCompartName)
{
	return m_pPacketListImple->GetMaxCount(strPacketNameList, strCompartName);
}

int CPacketList::GetPacketDataLength(int nIndex)
{
	return m_pPacketListImple->GetPacketDataLength(nIndex);
}

CString CPacketList::GetPacketType(CPacket *pPacket, const bool bIsFullType)
{
	return m_pPacketListImple->GetPacketType(pPacket, bIsFullType);
}

void CPacketList::SetDataInsertType(CPacketDataInfo::DATAINSERTTYPE nDataInsertType)
{
	m_pPacketListImple->SetDataInsertType(nDataInsertType);
}

CPacketDataInfo::DATAINSERTTYPE CPacketList::GetDataInsertType()
{
	return m_pPacketListImple->GetDataInsertType();
}

int CPacketList::GetMaxCount()
{
	return m_pPacketListImple->GetMaxCount();
}

int CPacketList::GetTimeInterval()
{
	return m_pPacketListImple->GetTimeInterval();
}
__int64 CPacketList::GetTimeIntervalWithSecond( void)
{
	return m_pPacketListImple->GetTimeIntervalWithSecond();
}

// (2006/7/11 - Seung-Won, Bae) Retrived the Next First Time of Data.
// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가 <<
//int CPacketList::GetStartTimeInTimeRangeOfLastData( void)
__int64 CPacketList::GetStartTimeInTimeRangeOfLastData( void)
{
	return m_pPacketListImple->GetStartTimeInTimeRangeOfLastData();
}

int CPacketList::GetTotalTRIndex() const
{
	return m_pPacketListImple->GetTotalTRIndex();
}

int CPacketList::GetTotalTRCount() const
{
	return m_pPacketListImple->GetTotalTRCount();
}

int CPacketList::GetTRCount(const int nTRIndex) const
{
	return m_pPacketListImple->GetTRCount(nTRIndex);
}

int CPacketList::GetRequestTRCount(const int nCurTRCount) const
{
	return m_pPacketListImple->GetRequestTRCount(nCurTRCount);
}

CScaleBaseData::HORZSCALEDRAWERTYPE CPacketList::GetDateUnitType()
{
	return m_pPacketListImple->GetDateUnitType();
}

CPoint CPacketList::GetUpdateRange()
{
	return m_pPacketListImple->GetUpdateRange();
}

int CPacketList::GetStartDate()
{
	return m_pPacketListImple->GetStartDate();
}

int CPacketList::GetEndDate(CPacket* pDatePacket)
{
	return m_pPacketListImple->GetEndDate(pDatePacket);
}

bool CPacketList::GetUseReal()
{
	return m_pPacketListImple->GetUseReal();
}

//bIsFullType =true 면 데이타형
//false면 표현형
CString CPacketList::GetPacketType(CString strPacketName, const bool bIsFullType)
{
	return m_pPacketListImple->GetPacketType(strPacketName, bIsFullType);
}

// 03.22.2002  특정 패킷에 있는 특정 index의 데이타를 return한다
double CPacketList::GetDataFromIndex(CString strFunction, int nIndex)
{
	return m_pPacketListImple->GetDataFromIndex(strFunction, nIndex);
}

// 03.22.2002  특정 패킷에 있는 특정 index의 데이타를 return한다
CString CPacketList::GetStrDataFromIndex(CString strFunction, int nIndex)
{
	return m_pPacketListImple->GetStrDataFromIndex(strFunction, nIndex);
}

bool CPacketList::FindKey(CString strKey)
{
	return m_pPacketListImple->FindKey(strKey);
}

int CPacketList::GetCount(const CString strPacketName)
{
	return m_pPacketListImple->GetCount(strPacketName);
}

CTypedPtrList<CObList, CPacket*>& CPacketList::GetTRPacketList()
{
	return m_pPacketListImple->GetTRPacketList();
}

/*
CTypedPtrList<CObList, CPacket*>& CPacketList::GetIndicatorPacketList()
{
	return m_pPacketListImple->GetIndicatorPacketList();
}
*/

CString CPacketList::GetTRPacketNames(const CString& strCompart) const
{
	return m_pPacketListImple->GetTRPacketNames(strCompart);
}

double CPacketList::GetCurrentDate()
{
	return m_pPacketListImple->GetCurrentDate();
}

bool CPacketList::RemoveFictionData()
{
	return m_pPacketListImple->RemoveFictionData();
}

void CPacketList::SetHoldingPacketSize(const int nSize)
{
	m_pPacketListImple->SetHoldingPacketSize(nSize);
}

void CPacketList::SetBasePacketName(const CString &szPacketName)
{
	m_pPacketListImple->SetBasePacketName(szPacketName);
}

bool CPacketList::UseRealData()
{
	return m_pPacketListImple->UseRealData();
}

CList<CString, CString>* CPacketList::GetRealKeyName()
{
	return m_pPacketListImple->GetRealKeyName();
}

CPacketType::PRICETYPE CPacketList::GetPriceType()
{
	return m_pPacketListImple->GetPriceType();
}

void CPacketList::SetUseRealData(bool bUseRealData)
{
	m_pPacketListImple->SetUseRealData(bUseRealData);
}

void CPacketList::SetItemData(const CString& strCode, const CString& strName)
{
	m_pPacketListImple->SetItemData(strCode, strName);
}

void CPacketList::GetItemData(CString& strCode, CString& strName) const
{
	m_pPacketListImple->GetItemData(strCode, strName);
}

CString CPacketList::GetItemCode() const
{
	return m_pPacketListImple->GetItemCode();
}

CString CPacketList::GetItemName() const
{
	return m_pPacketListImple->GetItemName();
}

CPacketType::COMMODITY_BOND_NAME CPacketList::GetCommodityBondNameInBase() const
{
	return m_pPacketListImple->GetCommodityBondNameInBase();
}

int CPacketList::GetLastestRealIndex()
{
	return m_pPacketListImple->GetLastestRealIndex();
}

void CPacketList::AddPacket(const CString szPacketOption)
{
	m_pPacketListImple->AddPacket(szPacketOption);
}

bool CPacketList::RemovePacket(const CString szPacketName)
{
	return m_pPacketListImple->RemovePacket(szPacketName);
}

// (2004.06.18, 배승원) Data Byte수를 Long으로 조정한다.
void CPacketList::SetPacketData(const CString& szPacketName, long pData, long lCount)
{
	m_pPacketListImple->SetPacketData(szPacketName, pData, lCount);
}

bool CPacketList::SetPacketData(const CString& strPacketName, const CString& strPacketType, 
		const char* pPacketData, const int nCount)
{
	return m_pPacketListImple->SetPacketData(strPacketName, strPacketType, pPacketData, nCount);
}

bool CPacketList::SetData(const CString szPacketName, int nIndex, double dValue, bool IsByReal)
{
	return m_pPacketListImple->SetData(szPacketName, nIndex, dValue, IsByReal);
}

void CPacketList::SetPacketLengthCount(int nLength)
{
	m_pPacketListImple->SetPacketLengthCount(nLength);
}

void CPacketList::SetVersion(double dVersion)
{
	m_pPacketListImple->SetVersion(dVersion);
}

CPacketType::REALTYPE CPacketList::GetRealType() const
{
	return m_pPacketListImple->GetRealType();
}

//sy 2004.07.16.
int CPacketList::GetRemovePacketCountInRealData() const
{
	return m_pPacketListImple->GetRemovePacketCountInRealData();
}

// (2004.09.14, 배승원) 보조 Message Item을 범용적으로 관리하여 조회 이용할 수 있도록한다.
BOOL CPacketList::GetHelpMessageItem( const char *p_szMsgKey, CString &p_strMsgValue)
{
	return m_pPacketListImple->GetHelpMessageItem( p_szMsgKey, p_strMsgValue);
}
// (2005/9/29 - Seung-Won, Bae) Get Help Message List
const CMapStringToString *CPacketList::GetHelpMessageMap( void)
{
	return m_pPacketListImple->GetHelpMessageMap();
}

//sy 2005.08.23
//보조메시지 (서버에서 받는 항목으로 각 항목별로 받을 수 있다)
bool CPacketList::SetHelpMessage(const CString& strHelpMessage, const bool bIsAllRemove)
{
	return (m_pPacketListImple->SetHelpMessage(strHelpMessage, bIsAllRemove) > 0);
}

bool CPacketList::SetHelpMessageRealDataRate(const CString& strHelpMessage, const bool bIsRemove)
{
	return m_pPacketListImple->SetHelpMessageRealDataRate(strHelpMessage, bIsRemove);
}
//sy end

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
BOOL CPacketList::SetBinTrHelpMsg( const char *p_szHelpMessage)
{
	return m_pPacketListImple->SetBinTrHelpMsg( p_szHelpMessage);
}
BOOL CPacketList::SetBinTrDoubleData( const char *p_szPacketName, long p_nCount, double FAR* p_pdData)
{
	return m_pPacketListImple->SetBinTrDoubleData( p_szPacketName, p_nCount, p_pdData);
}
BOOL CPacketList::SetBinTrStringData( const char *p_szPacketName, long p_nCount, const char **p_pszData)
{
	return m_pPacketListImple->SetBinTrStringData( p_szPacketName, p_nCount, p_pszData);
}
BOOL CPacketList::SetBinTrEnd( void)
{
	return m_pPacketListImple->SetBinTrEnd();
}

// 자신의 RQ : 복수종목 - ojtaso (20070117)
void CPacketList::SetRQ(LPCTSTR lpszRQ)
{
	m_pPacketListImple->SetRQ(lpszRQ);
}

// 자신의 RQ : 복수종목 - ojtaso (20070117)
LPCTSTR CPacketList::GetRQ()
{
	return m_pPacketListImple->GetRQ();
}

// (2008/2/4 - Seung-Won, Bae) for Other Meta of MultiLanguage
CString CPacketList::GetPacketOption( void)
{
	return m_pPacketListImple->GetPacketOption();
}

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
BOOL CPacketList::ShiftPacket( LPCTSTR p_szPacketNames, long p_szShiftCount)
{
	return m_pPacketListImple->ShiftPacket( p_szPacketNames, p_szShiftCount);
}

// 현재가 등락률 - ojtaso (20080602)
void CPacketList::OffsetPacketData(double dData, CString strPacketName/* = _T("")*/)
{
	m_pPacketListImple->OffsetPacketData(dData, strPacketName);
}


// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리 <<
CString CPacketList::GetBaseTimeName()
{
	return m_pPacketListImple->GetBaseTimeName();
}

//	void		SetBaseTimeName(CString strBaseTimeName);

int CPacketList::GetBaseTimeDifference()
{
	return m_pPacketListImple->GetBaseTimeDifference();
}

void CPacketList::SetBaseTimeDifference(int nBaseTimeDiff)
{
	m_pPacketListImple->SetBaseTimeDifference(nBaseTimeDiff);
}

int CPacketList::GetCalcDataTimeDifference()
{
	return m_pPacketListImple->GetCalcDataTimeDifference();
}

void CPacketList::SetBaseTime(CString strBaseTimeName, int nBaseTimeDiff)
{
	m_pPacketListImple->SetBaseTimeName(strBaseTimeName);
	m_pPacketListImple->CalcPacketTimeDiff(nBaseTimeDiff);
	m_pPacketListImple->SetBaseTimeDifference(nBaseTimeDiff);
}

int	CPacketList::GetOrgBaseTimeDifference()
{
	return m_pPacketListImple->GetOrgBaseTimeDifference();
}

void CPacketList::SetOrgBaseTime()
{
	m_pPacketListImple->SetBaseTimeName("");
	m_pPacketListImple->CalcPacketTimeDiff(m_pPacketListImple->GetOrgBaseTimeDifference());
	m_pPacketListImple->SetBaseTimeDifference(m_pPacketListImple->GetOrgBaseTimeDifference());
}

__int64 CPacketList::GetCurrDateTime() const
{
	return m_pPacketListImple->GetCurrDateTime();
}

bool CPacketList::GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const
{
	return m_pPacketListImple->GetMarketTime(nMarketStartTime, nMarketEndTime);
}

//	void		CalcPacketTimeDiff(int nBaseTimeDiff);

//	CPacket*	GetDatePacket();

	// 20081024 JS.Kim	허수봉을 ocx 및 AddIn 으로 주기위해 허수봉의 시간을 실봉시간으로 환산한다.
//	bool		CalcNMTimeToRealTime(CPacket *pPacket, const double& dTime, double& dRealTime, double& dCloseData);

// 20081029 JS.Kim	영업일 처리 기준을 구한다.
CPacketType::MARKETDAYTYPE CPacketList::GetMarketDayType()
{
	return m_pPacketListImple->GetMarketDayType();
}
//>>

// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
bool CPacketList::SetRealData( const char *p_szPacketName, const double dData, int &p_nRemovePacketDataCount, const int nOption)
{
	return m_pPacketListImple->SetRealData( p_szPacketName, dData, p_nRemovePacketDataCount, nOption);
}
bool CPacketList::GetRealData_CloseTime( const __int64 &p_nClock, __int64 &p_nRealDataTime)
{
	return m_pPacketListImple->GetRealData_CloseTime( p_nClock, p_nRealDataTime);
}
void CPacketList::PushNMTRState( const __int64 &p_nRealDataTime)
{
	m_pPacketListImple->PushNMTRState( p_nRealDataTime);
}
void CPacketList::PopNMTRState( void)
{
	m_pPacketListImple->PopNMTRState();
}
// (2009/11/12 - Seung-Won, Bae) Get NMTR Info
BOOL CPacketList::GetLastClockAndRealTime( __int64 &p_nLastClockTime, __int64 &p_nLastRealTime)
{
	return m_pPacketListImple->GetLastClockAndRealTime( p_nLastClockTime, p_nLastRealTime);
}

// (2009/10/21 - Seung-Won, Bae) Comment. 실시간 Data의 Packet Type을 관리한다.
CString CPacketList::GetRealTimeType( void)
{
	return m_pPacketListImple->GetRealTimeType();
}

// (2009/11/1 - Seung-Won, Bae) for Normalized Candle data.
double CPacketList::GetOrgDataBeforeRate( const char *p_szPacketName, const double p_dRatedData)
{
	return m_pPacketListImple->GetOrgDataBeforeRate( p_szPacketName, p_dRatedData);
}


// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
void CPacketList::GetRealData_RealDateTime( double &p_dData)
{
	m_pPacketListImple->GetRealData_RealDateTime( p_dData);
}


// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
void CPacketList::ChangeChartMode( int p_nChartMode)
{
	m_pPacketListImple->ChangeChartMode( p_nChartMode);
}
