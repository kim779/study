// IndicatorSingleton.cpp: implementation of the CIndicatorFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorFactory.h"

#include <Math.h>											// for floor()
#include <Assert.h>											// for assert()
#include "../Include_Chart/DLL_Load/IndicatorInfo.h"		// for CIndicatorInfo
#include "../Include_Chart/DLL_Load/PacketList.h"			// for CPacketList
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/ChartFormulateData.h"			// for CChartFormulateData
#include "../Include_Chart/PacketPointerList.h"				// for CGraphCalculateData
#include "../Include_Chart/Conversion.h"					// for CDataConversion
#include "../Include_Chart/DataMath.h"						// for CMath
#include "GetIndicatorInfo.h"								// for CGetIndicatorInfo
#include "resource.h"										// for IDS_EX_RIGHTS
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
#include "../Include_Chart/GraphData.h"					// for CSubGraphData
#include "Signal.h"											// for CSignal

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CIndicator* CIndicatorSingleton::Create( IMetaTable::EDDC_INDICATOR_ID p_eID, HWND p_hOcxHwnd)
{
	switch( p_eID)
	{
		case IMetaTable::EDDC_OCX_ACCOUNT_ITEMSIZE:	return new CAccount;
		case IMetaTable::EDDC_OCX_BIG_NEWS:			return new CBigNews;
		case IMetaTable::EDDC_OCX_BOXCHARTCHART:		return new CBoxChart;
		case IMetaTable::EDDC_OCX_FOURIER_TRANSFORM:	return new CFourierTransform;
		case IMetaTable::EDDC_OCX_FUOPPANDFCHART:		return new CFuOpPF;
		case IMetaTable::EDDC_OCX_GLANCEBALANCECHART:	return new CGlanceBalance;
		case IMetaTable::EDDC_OCX_VOLUMEFORSALE:		return new CHorizontalBar;
		case IMetaTable::EDDC_OCX_INVERSLINECHART:		return new CInverseLine;
		case IMetaTable::EDDC_OCX_KAGICHART:			return new CKagiChart;
		case IMetaTable::EDDC_OCX_LOCKCHART:			return new CLock( p_hOcxHwnd);
		case IMetaTable::EDDC_OCX_MARKETPROFILECHART:	return new CMarketProfileChart;
		case IMetaTable::EDDC_OCX_PANDFCHART:			return new CPF;
		case IMetaTable::EDDC_OCX_RAINBOWCHART:		return new CRainbow;
		case IMetaTable::EDDC_OCX_COMPARATIVECHART:	return new CRelative_Strength;
		case IMetaTable::EDDC_OCX_RENKOCHART:			return new CRenKoChart;
		case IMetaTable::EDDC_OCX_SWINGCHART:			return new CSwingChart;
		case IMetaTable::EDDC_OCX_THREELINEBRACKCHART:	return new CThreeLineBreak;
		case IMetaTable::EDDC_OCX_ZIGZAGCHART:			return new CZigzag;
		case IMetaTable::EDDC_OCX_CLCCHART:				return new CCLC;
		case IMetaTable::EDDC_OCX_VOLUMESELLBUYCHART:	return new CSellBuyHorizontalBar;
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
		// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
		case IMetaTable::EDDC_OCX_PRICEMACHART:			return new CMA( IMetaTable::EDDC_OCX_PRICEMACHART);
		case IMetaTable::EDDC_OCX_VOLUMEMACHART:		return new CMA( IMetaTable::EDDC_OCX_VOLUMEMACHART);

		default:										return new CNoCalculate( p_eID);
	}
	return NULL;
}

// (2006/9/6 - Seung-Won, Bae) Check ST Index Indicator
CIndicator *CIndicatorSingleton::CreateSTIndexCalculator( IMetaTable::EDDC_INDICATOR_ID p_eID)
{
	return new CSystemTradingCalculate( p_eID);
}


//*****************************************************************************************************
// SystemTrading에서 계산을 하는 지표
// 종찬(05/08/03) 지금 시간이 없기때문에 완전 날코딩함! 나중에 꼭! 반드시! 고칠것!!!!!!!
//******************************************************************************************************
void CSystemTradingCalculate::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	//bool bUse = signalData.IsUse();
	// signal 사용할 경우만 packet 연결
	// subGraph에 해당 packet 연결하기
	SetGraphPacketList(chartFormulateData, pGraphCalculateData, pIndicatorInfo, calculateData);
	// graph에 signal 연결하기
	SetSingalPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

// ----------------------------------------------------------------------------
//sy 2006.04.13.
// subGraph에 해당 packet 연결하기
void CSystemTradingCalculate::SetGraphPacketList(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo, CCalculateData& calculateData)
{
	//sy 2005.10.11 -> real 계산할때는 packet을 다시 셋팅할 필요가 없다!!
	if(pIndicatorInfo == NULL || chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL)
		return;

	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	CString strIndicatorName = pIndicatorInfo->GetIndicatorName();

// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
// 	//sy 2005.10.11 Parabolic은 패킷이 세개필요하다 parabolic데이타와 고가, 저가패킷.(특별한경우임)
// 	switch( g_iMetaTable.GetIndicatorID( strIndicatorName))
// 	{
// // (2007/1/25 - Seung-Won, Bae) Use High/Low Packet directly on Draw Graph.
// //		case IMetaTable::EDDC_ST_PARABOLICCHART:	SetPacketList_Parabolic( pPacketList, pGraphCalculateData, pIndicatorInfo);
// //													break;
// 		case IMetaTable::EDDC_ST_PRICEMACHART:
// 		case IMetaTable::EDDC_ST_VOLUMEMACHART:	SetMAPacketList( pPacketList, pGraphCalculateData, pIndicatorInfo, calculateData);
// 												break;
// 		default:	SetGraphPacketList( pPacketList, pGraphCalculateData, pIndicatorInfo);
// 					break;
// 	}
	SetGraphPacketList( pPacketList, pGraphCalculateData, pIndicatorInfo);
}

// graph에 signal 연결하기
void CSystemTradingCalculate::SetSingalPacket(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	if(pGraphCalculateData == NULL || pIndicatorInfo == NULL)
		return;

	// signal 사용할 경우만 packet 연결
	if(!pIndicatorInfo->IsSignalUse())
		return;

	if(pGraphCalculateData->GetSignalPacket() != NULL)
		return;

	CPacket* pSignalPacket = NULL;
	// (2006/12/11 - Seung-Won, Bae) Support multi-same Indicator.
	if( !CIndicator::GetSignalPacket( chartFormulateData.GetPacketList(), chartFormulateData.GetGraphName(), pSignalPacket))
		return;

	pGraphCalculateData->SetSignalPacket( pSignalPacket);
}
//sy end

// ----------------------------------------------------------------------------
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// void CSystemTradingCalculate::SetMAPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo, CCalculateData& calculateData)
// {
// 	if(!pPacketList || !pGraphCalculateData || !pIndicatorInfo)
// 		return;
// 
// 	int nSubGraphIndex = CGetIndicatorInfo::GetSubGraphIndex(pIndicatorInfo, calculateData.GetPacketNamesForSave());
// 	if(nSubGraphIndex < 0)
// 		return;
// 
// 	CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetAt(nSubGraphIndex);
// 	if(pSubGraphList == NULL)
// 		return;
// 
// 	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
// 	CString strSubGraphPacketNameHeader;
// 	if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";
// 
// 	CPacket* pPacket = pSubGraphList->GetHeadPacket();
// 	if(pPacket == NULL) 
// 	{
// 		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
// 		CPacket* pMAPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + calculateData.GetPacketNamesForSave(), false);
// 		if(pMAPacket == NULL)
// 			return;
// 		pSubGraphList->AddTail2( pMAPacket);
// 	}
// 
// }

// (2007/1/25 - Seung-Won, Bae) Use High/Low Packet directly on Draw Graph.
/*
//sy 2005.10.11
void CSystemTradingCalculate::SetPacketList_Parabolic( CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	if(pPacketList == NULL || pGraphCalculateData == NULL || pIndicatorInfo == NULL)
		return;

	CPacket* pHighPricePacket = pPacketList->GetBaseData( _MTEXT( C0_HIGH));
	CPacket* pLowPricePacket = pPacketList->GetBaseData( _MTEXT( C0_LOW));
	if(pHighPricePacket == NULL || pLowPricePacket == NULL) return;

	CPacket* pParabolicPacket = NULL;
	CParabolic_SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo, pParabolicPacket, pHighPricePacket, pLowPricePacket);
}
//sy end
bool CSystemTradingCalculate::CParabolic_SetGraphPacketList(CPacketList* pPacketList,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo*& pIndicatorInfo,
									CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	pPacket = NULL;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) return CParabolic_InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pPacket, pPacket1, pPacket2, pSubGraphList);
		else
		{
			if(pPacket == NULL)
				pPacket = pIndiPacket;
		}
	}
	return (pPacket != NULL);
}
bool CSystemTradingCalculate::CParabolic_InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					                   CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2,
									   CSubGraphPacketList* pSubGraphList)
{
	if(pIndicatorInfo == NULL)
		return false;

	CString strSubGraphName = pIndicatorInfo->GetSubGraphName(0);
	if(strSubGraphName.IsEmpty())
		return false;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	CString strSubGraphPacketNameHeader;
	if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";
	pPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + strSubGraphName, false);

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( pPacket);
	pSubGraphList->AddTail2( pPacket1);
	pSubGraphList->AddTail2( pPacket2);
	return (pPacket != NULL);
}
*/

void CSystemTradingCalculate::SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	if(!pPacketList || !pGraphCalculateData || !pIndicatorInfo)
		return;

	CList<CString, CString> subGraphNameList;
	if(!pIndicatorInfo->GetSubGraphNameList(subGraphNameList))
		return;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	CString strSubGraphPacketNameHeader;
	if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

	POSITION pos_subGraphName = subGraphNameList.GetHeadPosition();
	POSITION pos_graphCalData = pGraphCalculateData->GetHeadPosition();
	while(pos_subGraphName && pos_graphCalData)
	{
		CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetNext(pos_graphCalData);
		if(pSubGraphPacketList == NULL)
			return;

		CPacket* pPacket = pSubGraphPacketList->GetHeadPacket();
		if(pPacket == NULL)
		{
			CPacket* pIndiPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + subGraphNameList.GetNext(pos_subGraphName), false);
			if(pIndiPacket == NULL)
				return;
			pSubGraphPacketList->AddTail2(pIndiPacket);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//거래내역
///////////////////////////////////////////////////////////////////////////////////////////////////////
CAccount::CAccount() : 
	m_pDateList(NULL),
	m_pHighPriceList(NULL),
	m_pLowPriceList(NULL),
	m_pAccountList(NULL),
	m_pAccountDateList(NULL),
	m_pDatePacket(NULL),
	m_pAccountDatePacket(NULL),
	CIndicator( IMetaTable::EDDC_OCX_ACCOUNT_ITEMSIZE)
{
}

void CAccount::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDateList = m_pDatePacket->GetnumericDataList();
		m_pHighPriceList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pLowPriceList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pAccountList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pAccountDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pAccountDateList = GetnumericDataList(m_pAccountDatePacket);
	}
	if(!IsValidPacket(m_pDateList) || !IsValidPacket(m_pHighPriceList) || !IsValidPacket(m_pLowPriceList) ||
	   !IsValidPacket(m_pAccountList) || !IsValidPacket(m_pAccountDateList))
		return;

	CPacket* pBuy = NULL;
	CPacket* pSell = NULL;
	if(!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo, pBuy, pSell))
		return;
	
	if(IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pBuy))
		return;

	//자료일자, 거래일자의 date 타입얻기, 두개의 일자를 비교할 수 있는 date 타입 얻기
	CString strgraphdate, strsellbuydate, strcomparedate;
	GetPacketDateType(m_pDatePacket, m_pAccountDatePacket, strgraphdate, strsellbuydate, strcomparedate);	

	CList<double, double> drealgraphdate, drealsellbuydate;
	GetRealDate(strgraphdate, strcomparedate, *m_pDateList, drealgraphdate); //자료일자
	GetRealDate(strsellbuydate, strcomparedate, *m_pAccountDateList, drealsellbuydate); //계좌 자료일자
	//매도, 매수
	GetSellandBuyList(calculateData.GetPoint(), drealgraphdate, drealsellbuydate,
				      m_pAccountList, pBuy, pSell);

	SetPacketTypes(pBuy, TYPE_TWODECIMAL, TYPE_INTERGER, 0);
	SetPacketTypes(pSell, TYPE_TWODECIMAL, TYPE_INTERGER, 0); 
}


void CAccount::GetPacketDateType(CPacket* pDate, CPacket* pAccountDate,
								 CString& strgraphdate,
								 CString& strsellbuydate,
								 CString& strcomparedate)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pDate || !pAccountDate) return;

	strgraphdate = pDate->GetType(); 
	strsellbuydate = pAccountDate->GetType();

	strcomparedate = GetCompareDateType(strgraphdate, strsellbuydate);
}

//원래 내려왔던 data 에서 비교해야하는 data만을 추출..
//예) 20020314, 14102109 일때, 각각 뒤의 2자리와 앞의 2자리를 추출한 data를 얻는다. 비교를 위해서...
void CAccount::GetRealDate(const CString& stroriginaldatetype, const CString& strdatetype,
						   const CList<double, double>& dDateList,
						   CList<double, double>& dRealDateList)
{
	TCHAR first = strdatetype.GetAt(0);
	int nfind = stroriginaldatetype.Find(first, 0);

	//string으로 바꿔서 해당 인덱스부터 인덱스까지 가져와서 다시 double형으로 바꿔서 넣는다.
	POSITION pos = dDateList.GetHeadPosition();
	while(pos) {
		CString temp = CDataConversion::DoubleToString(dDateList.GetNext(pos), stroriginaldatetype.GetLength());
		dRealDateList.AddTail(atoi(temp.Mid(nfind, strdatetype.GetLength())));
	}
}

//dSellBuyDateList = HHMMSSNN, 다른게 될수도 있따..
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//예) graph = YYYYMMDD, sellbuy = DDHHMMSS 일때는 dd로만 비교 가능
CString CAccount::GetCompareDateType(const CString& strgraphdate, const CString& strsellbuydate)
{
	if(strsellbuydate.CompareNoCase("YYYYMMDD") == 0)
		return WhenYYYYMMDD(strgraphdate);
	else if(strsellbuydate.CompareNoCase("YYMMDD") == 0)
		return WhenYYMMDD(strgraphdate);
	else if(strsellbuydate.CompareNoCase("YYYYMM") == 0)
		return WhenYYYYMM(strgraphdate);
	else if(strsellbuydate.CompareNoCase("YYMM") == 0)
		return WhenYYMM(strgraphdate);
	else if(strsellbuydate.CompareNoCase("MMDD") == 0)
		return WhenMMDD(strgraphdate);
	else if(strsellbuydate.CompareNoCase("DDHH") == 0)
		return WhenDDHH(strgraphdate);
	else if(strsellbuydate.CompareNoCase("DDHHMM") == 0)
		return WhenDDHHMM(strgraphdate);
	else if(strsellbuydate.CompareNoCase("DDHHMMSS") == 0)
		return WhenDDHHMMSS(strgraphdate);
	else if(strsellbuydate.CompareNoCase("HHMMSS") == 0)
		return WhenHHMMSS(strgraphdate);
	else if(strsellbuydate.CompareNoCase("HHMMSSNN") == 0)
		return WhenHHMMSSNN(strgraphdate);
	return "";
}

//dSellBuyDateList = YYYYMMDD
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. yyyy
//2. YYMM - 분과 헷갈릴수 있으니까..
//3. MMDD비교 - 분과 헷갈릴수 있으니까..
//4. DD비교
//1,2,3 => YYYYMMDD
//2,3 => YYMMDD
//1,2=> YYYYMM
//2=> YYMM
//3 => MMDD
//4=> DD
CString CAccount::WhenYYYYMMDD(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYYY") != -1 && strGraphDateType.Find("YYMM") != -1 && strGraphDateType.Find("MMDD") != -1)
		return "YYYYMMDD";
	else if(strGraphDateType.Find("YYMM") != -1 && strGraphDateType.Find("MMDD") != -1)
		return "YYMMDD";
	else if(strGraphDateType.Find("YYYY") != -1 && strGraphDateType.Find("YYMM") != -1)
		return "YYYYMM";
	else if(strGraphDateType.Find("YYMM") != -1)
		return "YYMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MMDD";
	else if(strGraphDateType.Find("DD") != -1)
		return "DD";
	return "";
}
//dSellBuyDateList = YYMMDD
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. YYMM
//2. MMDD
//3. DD
//1,2 => YYMMDD
//1 => YYMM
//2 => MMDD
//3=> DD
CString CAccount::WhenYYMMDD(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYMM") != -1 && strGraphDateType.Find("MMDD") != -1)
		return "YYMMDD";
	else if(strGraphDateType.Find("YYMM") != -1)
		return "YYMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MMDD";
	else if(strGraphDateType.Find("DD") != -1)
		return "DD";
	return "";
}
//dSellBuyDateList = YYYYMM
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. YYYY
//2. YYMM
//3. MMDD - 분과 헷갈릴수 있으니까..비교해서 DD는 없으니까. MM만 해당
//1,2 => YYYYMM
//2 => YYMM
//3 => MM
CString CAccount::WhenYYYYMM(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYYY") != -1 && strGraphDateType.Find("YYMM") != -1)
		return "YYYYMM";
	else if(strGraphDateType.Find("YYMM") != -1)
		return "YYMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MM";
	return "";
}
//dSellBuyDateList = YYMM
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. YYMM
//2. MMDD - 분과 헷갈릴수 있으니까..비교해서 DD는 없으니까. MM만 해당
//1 => YYMM
//2 => MM
CString CAccount::WhenYYMM(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYMM") != -1)
		return "YYMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MM";
	return "";
}
//dSellBuyDateList = MMDD
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. YYMM - 분과 헷갈릴수 있으니까..비교해서 YY는 없으니까. MM만 해당
//2. MMDD - 분과 헷갈릴수 있으니까..
//3. DD
//1 => MM
//2 => MMDD
//3 => DD
CString CAccount::WhenMMDD(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYMM") != -1)
		return "MM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MMDD";
	else if(strGraphDateType.Find("DD") != -1)
		return "DD";
	return "";
}
//dSellBuyDateList = DDHH
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. MMDD - 분과 헷갈릴수 있으니까..비교해서 MM는 없으니까. DD만 해당
//2. DDHH
//3. HH
//1 => DD
//2 => DDHH
//3 => HH
CString CAccount::WhenDDHH(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("MMDD") != -1)
		return "DD";
	else if(strGraphDateType.Find("DDHH") != -1)
		return "DDHH";
	else if(strGraphDateType.Find("HH") != -1)
		return "HH";
	return "";
}
//dSellBuyDateList = DDHHMM
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. MMDD - 분과 헷갈릴수 있으니까..비교해서 MM는 없으니까. DD만 해당
//2. DDHH
//3. HHMM - 분과 헷갈릴수 있으니까..
//1 => DD
//2 => DDHH
//3 => HHMM
//2,3 => DDHHMM
CString CAccount::WhenDDHHMM(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("DDHH") != -1 && strGraphDateType.Find("HHMM") != -1)
		return "DDHHMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "DD";
	else if(strGraphDateType.Find("DDHH") != -1)
		return "DDHH";
	else if(strGraphDateType.Find("HHMM") != -1)
		return "HHMM";
	return "";
}

//dSellBuyDateList = DDHHMMSS
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. MMDD - 분과 헷갈릴수 있으니까..비교해서 MM는 없으니까. DD만 해당
//2. DDHH
//3. HHMM - 분과 헷갈릴수 있으니까..
//3. SS
//1 => DD
//2 => DDHH
//3 => HHMM
//2,3 => DDHHMM
//2,3,4 => DDHHMMSS
CString CAccount::WhenDDHHMMSS(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("DDHH") != -1 && strGraphDateType.Find("HHMM") != -1 && strGraphDateType.Find("SS") != -1)
		return "DDHHMMSS";
	else if(strGraphDateType.Find("DDHH") != -1 && strGraphDateType.Find("HHMM") != -1)
		return "DDHHMM";
	if(strGraphDateType.Find("MMDD") != -1)
		return "DD";
	else if(strGraphDateType.Find("DDHH") != -1)
		return "DDHH";
	else if(strGraphDateType.Find("HHMM") != -1)
		return "HHMM";
	return "";
}

//dSellBuyDateList = HHMMSS
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. HH
//2. HHMM 
//3. HHMMSS
//1 => HH
//2 => HHMM
//3 => HHMMSS
CString CAccount::WhenHHMMSS(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("HH") != -1)
		return "HH";
	else if(strGraphDateType.Find("HHMM") != -1)
		return "HHMM";
	else if(strGraphDateType.Find("HHMMSS") != -1)
		return "HHMMSS";
	return "";
}
//dSellBuyDateList = HHMMSSNN
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph에
//1. HH
//2. HHMM 
//3. HHMMSS
//4. HHMMSSNN
//1 => HH
//2 => HHMM
//3 => HHMMSS
//4 => HHMMSSNN
CString CAccount::WhenHHMMSSNN(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("HH") != -1)
		return "HH";
	else if(strGraphDateType.Find("HHMM") != -1)
		return "HHMM";
	else if(strGraphDateType.Find("HHMMSS") != -1)
		return "HHMMSS";
	else if(strGraphDateType.Find("HHMMSSNN") != -1)
		return "HHMMSSNN";
	return "";
}

//
void CAccount::GetSellandBuyList(const CPoint& point,
								 const CList<double, double>& dRealGraphDateList,
								 const CList<double, double>& dRealSellBuyDateList,
								 CList<double, double>* pSellBuyList,
								 CPacket* pBuy, CPacket* pSell)
{
//실시간.....
	double nsell = 0; //data
	double nbuy = 0;
	int ngraph, nsellbuy;
	int date[2];  

	double* nSell = new double[dRealGraphDateList.GetCount()];
	double* nBuy = new double[dRealGraphDateList.GetCount()];
	for(int count = 0; count< dRealGraphDateList.GetCount(); count++) {
		nSell[count] = 0;
		nBuy[count] = 0;
	}

	//실시간..
	//맨 마지막 거래내역만 비교한다.
	if(point.x != 0) {
		ngraph = point.y;
		nsellbuy = (dRealSellBuyDateList.GetCount() - 1);
	}
	else { //New, Insert
		ngraph = (dRealGraphDateList.GetCount() - 1);
		nsellbuy = 0;
	}

	date[0] = (int)(dRealGraphDateList.GetAt(FindPosition(dRealGraphDateList.GetCount() - 1, dRealGraphDateList)));
	POSITION pos = FindPosition(ngraph, dRealGraphDateList);
	if(pos == NULL)
	{
		delete[] nSell;
		delete[] nBuy;
		return ;
	}
	for(ngraph; ngraph >= point.x ; ngraph--) {
		if(nsellbuy == dRealSellBuyDateList.GetCount())
			break;

		date[1] = date[0];
//		date[0] = (int)(dRealGraphDateList.GetAt(dRealGraphDateList.FindIndex(ngraph)));
		date[0] = (int)(dRealGraphDateList.GetPrev(pos));
		POSITION pos1 = FindPosition(nsellbuy, dRealSellBuyDateList); 
		if(pos1 == NULL)
		{
			delete[] nSell;
			delete[] nBuy;
			return ;
		}
		for(nsellbuy; nsellbuy < dRealSellBuyDateList.GetCount() ; nsellbuy++) {
//			int sellbuydate = (int)(dRealSellBuyDateList.GetAt(dRealSellBuyDateList.FindIndex(nsellbuy)));
			int sellbuydate = (int)(dRealSellBuyDateList.GetNext(pos1));

			//nsellbuy는 바뀌는게 불규칙
			if(date[0] == sellbuydate) {

				// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
				if( !pSellBuyList){
					delete[] nSell;
					delete[] nBuy;
					return;
				}

				double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
				if(value == 1)
					nbuy += 1;
				else if(value == 2)
					nsell += 1;
			}
			else if(date[1] > sellbuydate && date[0] < sellbuydate) {
				CString strPredate = CDataConversion::IntToString(date[1]);
				CString strCurrentdate = CDataConversion::IntToString(date[0]);
				CString strsellbuydate = CDataConversion::IntToString(sellbuydate);

				int nCount = (strPredate.GetLength() / 2);
				if(nCount > 3)
					nCount = 3;
				bool blast = false;
				for(int nIndex = 1 ; nIndex < nCount; nIndex++) {
					strCurrentdate.Delete(strCurrentdate.GetLength() - 2, 2);
					strPredate.Delete(strPredate.GetLength() - 2, 2);
					strsellbuydate.Delete(strsellbuydate.GetLength() - 2, 2);

					if((nIndex +1) == nCount)
						blast = true;

					if(CompareDate(blast, strPredate, strCurrentdate, strsellbuydate,
								  ngraph, nsellbuy, nsell, nbuy, nSell, nBuy, pSellBuyList))
						break;
				}
			}
			else  //현재번째의 data는 다시 비교해야 하니까..
				break;
		}
		nSell[ngraph] = nsell;
		nBuy[ngraph] = nbuy;
		nsell = 0;
		nbuy = 0;
//		TRACE("index=> %d, date=>%d, sell => %d, buy => %d\r\n", ngraph, date[0], nSell[ngraph], nBuy[ngraph]);
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSell || !pBuy){
		delete[] nSell;
		delete[] nBuy;
		return;
	}

	for(int i = point.x ; i < dRealGraphDateList.GetCount() ; i++) {
		pSell->AppendData(nSell[i]);
		pBuy->AppendData(nBuy[i]);
	}
	delete[] nSell;
	delete[] nBuy;
}

//1, 0,
//sellbuydate = 13090023
bool CAccount::CompareDate(const bool bLast,
						   const CString strPreDate,
						   const CString strCurrentDate,
						   const CString strSellBuyDate,
						   const int ngraph, const int nsellbuy, double& nsell, double& nbuy,
						   double* nSell, double* nBuy, CList<double, double>* pSellBuyList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSellBuyList) return FALSE;

	//13090024, 13090015
	if(strCurrentDate.CompareNoCase(strSellBuyDate) == 0 && strPreDate.CompareNoCase(strSellBuyDate) == 0) {
		double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
		if(value == 1)
			nbuy += 1;
		else if(value == 2)
			nsell += 1;
		return true;
	}
	//130902 , 130800 //2번째 들어왔을때.
	else if(strCurrentDate.CompareNoCase(strSellBuyDate) != 0 && strPreDate.CompareNoCase(strSellBuyDate) == 0) {
		double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
		if(value ==1)
			nSell[ngraph + 1] = (nSell[ngraph+1] + 1);
		else if(value == 2)
			nBuy[ngraph + 1] = (nBuy[ngraph+1] + 1);
		return true;
	}
	//13090200, 13090000
	else if(strCurrentDate.CompareNoCase(strSellBuyDate) == 0 && strPreDate.CompareNoCase(strSellBuyDate) != 0) {
		double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
		if(value == 1)
			nbuy += 1;
		else if(value == 2)
			nsell += 1;
		return true;
	}
	//131002 , 130800 하다가 말았음.
	else if(strPreDate.CompareNoCase(strSellBuyDate) != 0 && strPreDate.CompareNoCase(strSellBuyDate) != 0) {
		if(bLast == true) {
			double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
			if(value == 1)
				nbuy += 1;
			else if(value == 2)
				nsell += 1;
			return true;
		}
		return false;
	}
	return false;
}

bool CAccount::IsCurrentDateCloserthanPreDate(const int nPredate, const int nCurrentdate, const int nSellbuyDate)
{
	return abs(nCurrentdate - nSellbuyDate) < abs(nPredate - nSellbuyDate) ? true : false;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//BigNews
//늘 다시 계산.
///////////////////////////////////////////////////////////////////////////////////////////////////////
CBigNews::CBigNews() : 
	m_pDateList(NULL),
	m_pBignewsDataList(NULL),
	m_pNewsDateList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_BIG_NEWS)
{
}
void CBigNews::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pDateList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pBignewsDataList = pPacketList->GetstrDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pNewsDateList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
	}
//	if(!IsValidPacket(m_pDateList) || !IsValidPacket(m_pNewsDateList) || m_pBignewsDataList == NULL || m_pBignewsDataList->GetCount() <= 0)
//		return;

	CPacket* pBigNews = NULL;
	if(!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo, pBigNews))
		return;
	
	if(IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pBigNews))
		return;

	//sy 2004.04.13.
	// 문자일 경우에는 미리 type를 넣어 줘야 한다.
	// -> 기본으로 "X 1" 로 setting 하므로 add되지 않는다.
	SetPacketTypes(pBigNews, _MTEXT( C6_CHARACTER), _MTEXT( C6_CHARACTER), 0);
	GetBigNews(calculateData.GetPoint(), m_pDateList, m_pBignewsDataList, m_pNewsDateList, pBigNews);
}

void CBigNews::GetBigNews(const CPoint& point, 
						  CList<double, double>* pGraphDateList,
						  CList<CString, CString>* pBigNewsList,
						  CList<double, double>* pBigNewsDateList,
						  CPacket* pBigNews)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pBigNewsDateList || !pGraphDateList || !pBigNews) return;

	if(pBigNewsDateList->GetCount() <= 0)
		return;

	POSITION posNewsDate = FindPosition(0, *pBigNewsDateList);
	POSITION posDate = FindPosition(0, *pGraphDateList);

	if(!IsValidBigNews(pGraphDateList, pBigNewsDateList, pBigNews))
		return;

	int nCount = pGraphDateList->GetCount();
	int ngraph = 0;
	int nNewsDate = 0;
	for(int nbignews = 0; nbignews < pBigNewsDateList->GetCount() ; nbignews++)
	{
		if(posNewsDate == NULL || posDate == NULL)
			return ;
		//자료일자에 빅뉴스가 일어난 날짜가 일부 없는 경우(자료일자가 더 이전인경우)
		//이후 빅뉴스에 대해 표현할수 없다.
		if(ngraph == nCount - 1)
			break;

		nNewsDate = (int)pBigNewsDateList->GetNext(posNewsDate);
		for(ngraph; ngraph < nCount ; ngraph++)
		{
			POSITION pos = posDate;
			int nGraphDate = (int)pGraphDateList->GetNext(posDate);
			if(nGraphDate < nNewsDate)
				pBigNews->AppendData("");
			else if(nGraphDate == nNewsDate)
			{
				pBigNews->AppendData(pBigNewsList->GetAt(FindPosition(nbignews, *pBigNewsList)));
				ngraph++;
				break;
			}
			else if(nGraphDate > nNewsDate)
			{
				posDate = pos;
				break;
			}
		}
	}

	//자료일자에 빅뉴스가 일어난 날짜가 일부 없는 경우(자료일자가 마지막 빅뉴스 이후에 존재하는경우)
	//마지막 빅뉴스를 비교하고 위 for문을 벗어난다.
	if(ngraph != nCount - 1)
	{
		for(ngraph ; ngraph < nCount ; ngraph++)
			pBigNews->AppendData("");
	}

}

bool CBigNews::IsValidBigNews(CList<double, double>* pGraphDateList,
							  CList<double, double>* pBigNewsDateList,
							  CPacket* pBigNews)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphDateList || !pBigNewsDateList || !pBigNews) return FALSE;

	int nCount = pGraphDateList->GetCount();
	//자료일자에 빅뉴스가 일어난 날짜가 전부 없는 경우(자료일자가 더 이전인경우)
	if(pGraphDateList->GetTail() < pBigNewsDateList->GetHead())
	{
		for(int nGraph = 0 ; nGraph < nCount ; nGraph++)
			pBigNews->AppendData("");
		return false;
	}

	//자료일자에 빅뉴스가 일어난 날짜가 전부 없는 경우(자료일자가 마지막 빅뉴스 이후에 존재하는경우)
	if(pGraphDateList->GetHead() > pBigNewsDateList->GetTail())
	{
		for(int nGraph = 0 ; nGraph < nCount ; nGraph++)
			pBigNews->AppendData("");
		return false;
	}
	return true;
}


//******************************************************************************************************************
//	CBoxChart
//	  
// (2004.10.20, 배승원) Logic 분석 및 오류 수정
// (2004.10.20, 배승원) Box Chart Packet을 시고저종 4개로 수정한다.
// [ Box Chart의 Output Packet에 대하여 ]
//		1. Output Packet 4개로 구성되며 아래와 Packet 별 설명과 같이 구성된다.
//		2. Output Packet의 Data는 가격 Packet (시/고/저/종)과 그 Data 수가 같으며,
//				Box 구간네 값을 일정하게 유지되도록 산출한다.
//******************************************************************************************************************
CBoxChart::CBoxChart() : 
	m_pBoxStartPacket(NULL),	// Box Chart에서의 시가 Packet
	m_pBoxHighPacket(NULL),		// Box Chart에서의 고가 Packet
	m_pBoxLowPacket(NULL),		// Box Chart에서의 저가 Packet
	m_pBoxEndPacket(NULL),		// Box Chart에서의 종가 Packet
	m_pStartPriceList(NULL),	// 가격차트의 시/고/저/종 4 Packet
	m_pHighPriceList(NULL),
	m_pLowPriceList(NULL),
	m_pEndPriceList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_BOXCHARTCHART)
{
}

void CBoxChart::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 0. Packet List를 확보한다.
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList || !pIndicatorInfo) return;

	// 1. 조건 List를 확보한다. (조건값은 Box로 묽을 봉수 하나 뿐이다.)
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();

	// 2. 필요한 기본 Packet을 확보한다.
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		CPacket *pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pStartPriceList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pHighPriceList  = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pLowPriceList   = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pEndPriceList   = pPacket->GetnumericDataList();
	}
	if(!IsValidPacket(m_pStartPriceList) || !IsValidPacket(m_pHighPriceList) || 
		!IsValidPacket(m_pLowPriceList) || !IsValidPacket(m_pEndPriceList))
		return;

	// 3. 산출해야할 Packet을 확보한다. (왜 6개씩이나?)
	if(!SetGraphPacketList(chartFormulateData.GetCalculateType(), pPacketList, pGraphCalculateData, 
		                   pIndicatorInfo))
		return;

	// 4. Box Chart Data를 산출한다.
	CPoint point = calculateData.GetPoint();
	CScaleBaseData::HORZSCALEDRAWERTYPE nType = pPacketList->GetDateUnitType();
	// 4.1 먼저 Box차트를 구성하는 값을 구한다.
	// 4.1.1 먼저 일/주/월 Chart의 Box Chart 산출한다.
	//		Real이 와도 Boxing 대상 봉이 늘지 않는다. 실제로 일봉만 Real이 수신된다.
	//		따라서, 최종 봉에서 Box가 완전히 마무리되도록 마지막 Data에서부터 Box를 구성하여,
	//			첫 봉에서 Box가 미완으로 끝날 수 있도록 한다.
	if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY)
		GetBackwardBoxChart(point, GetCondition(0, pConditionList), m_pHighPriceList, m_pLowPriceList, m_pStartPriceList, m_pEndPriceList);
	// 4.1.2 분/틱 Chart의 Box Chart Data를 산출한다. (Real 수신시 Boxing 대상 봉이 늘어날 수 있다.)
	//		따라서, 특정 중간의 봉이 조회시점에 따라 Box상의 위치가 변하지 않도록 (일/주/월 방식으로 하면 변하게 된다.)
	//			첫봉에서 Box가 완전하도록 시작하여, 끝봉에서 Box가 미완으로 끝날 수 있도록 한다.
	//		그렇게 하여 Real을 수신하면서 Box를 마무리지어 갈수 있도록 한다.
	else GetForwardBoxChart(point, GetCondition(0, pConditionList), m_pHighPriceList, m_pLowPriceList, m_pStartPriceList, m_pEndPriceList);

	// 5. 산출된 Packet을 설정한다.
	CString strPacketType = GetPacketType(pPacketList);
	SetPacketTypes( m_pBoxStartPacket,	strPacketType, strPacketType, 0);
	SetPacketTypes( m_pBoxHighPacket,	strPacketType, strPacketType, 0);
	SetPacketTypes( m_pBoxLowPacket,	strPacketType, strPacketType, 0);
	SetPacketTypes( m_pBoxEndPacket,	strPacketType, strPacketType, 0);
}

void CBoxChart::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData( m_pBoxStartPacket);
	ClearPacketData( m_pBoxHighPacket);
	ClearPacketData( m_pBoxLowPacket);
	ClearPacketData( m_pBoxEndPacket);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

// (2004.10.20, 배승원, 분석, 오류 수정) 일/주/월 Chart의 Box Chart 산출한다.
//		Real이 와도 Boxing 대상 봉이 늘지 않는다. 실제로 일봉만 Real이 수신된다.
//		따라서, 최종 봉에서 Box가 완전히 마무리되도록 마지막 Data에서부터 Box를 구성하여,
//			첫 봉에서 Box가 미완으로 끝날 수 있도록 한다.
void CBoxChart::GetBackwardBoxChart(const CPoint& point, const double dCondition,
									CList<double, double>* pTopPriceList,
									CList<double, double>* pBottomPriceList,
									CList<double, double>* pStartPriceList,
									CList<double, double>* pEndPriceList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pBoxStartPacket || !m_pBoxHighPacket || !m_pBoxLowPacket || !m_pBoxEndPacket) return;

	// 0. 고/저 Data Packet을 구하고, 중간 산출 변수를 Clear한다.
	CList<double, double>* pStartList	= m_pBoxStartPacket->GetnumericDataList();
	CList<double, double>* pTopList		= m_pBoxHighPacket->GetnumericDataList();
	CList<double, double>* pBottomList	= m_pBoxLowPacket->GetnumericDataList();
	CList<double, double>* pEndList		= m_pBoxEndPacket->GetnumericDataList();

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pTopPriceList || !pBottomPriceList || !pStartPriceList || !pEndPriceList
		|| !pTopList || !pBottomList || !pStartList || !pEndList) return;

	// 1. 기존에 산출한 Box Chart Data를 Clear시킨다.
	//		Clear 예외의 경우가, Data가 하나도 없거나 TR 또는 첫Real(NoTR Real)의 경우이므로,
	//		실질적으로 Clear시키지 않는 경우가 없다고 봐도 무방하다.
	// (2004.10.20, 배승원) 역으로 Box를 구성해 가는 상황에서 Real이 들어온다고,
	//		맨 첫 Box (역으로 구성하는 입장에서)만 재구성하는 Logic이 아니므로,
	//		항상 Clear시키도록 하여야 한다.
	pTopList->RemoveAll();
	pBottomList->RemoveAll();
	pStartList->RemoveAll();
	pEndList->RemoveAll();

	// 2. Box Chart의 시고 List를 구성한다.
	// 2.0 먼저 고가와 종가의 Data수가 같지 않으면 진행하지 않는다.
	int nTopPriceCount = pTopPriceList->GetCount();
	if( nTopPriceCount != pBottomPriceList->GetCount()
		&& nTopPriceCount != pStartPriceList->GetCount()
		&& nTopPriceCount != pEndPriceList->GetCount()) return;
	// 2.1 가격차트 고가와 종가의 끝부터 값을 추출하면서 진행한다.
	int			nDataCount = 0;
	int			nBoxWidth = (int)dCondition;
	double		dTop, dBottom, dMaxTop, dMaxBottom, dStart, dEnd;
	POSITION	posTop		= pTopPriceList->GetTailPosition();
	POSITION	posBottom	= pBottomPriceList->GetTailPosition();
	POSITION	posStart	= pStartPriceList->GetTailPosition();
	POSITION	posEnd		= pEndPriceList->GetTailPosition();
	while( posTop && posBottom && posStart && posEnd)
	{
		// 2.1.1 이번에 구성시킬 Box Chart의 고가와 저가를 산출한다.
		// 2.1.1.1 첫 Box Data를 확인한다.
		dMaxTop		= pTopPriceList->GetPrev(		posTop);
		dMaxBottom	= pBottomPriceList->GetPrev(	posBottom);
		dStart		= pStartPriceList->GetPrev(		posStart);
		dEnd		= pEndPriceList->GetPrev(		posEnd);

		// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( dMaxTop < dEnd || dMaxTop < dStart || dEnd < dMaxBottom || dStart < dMaxBottom)
			dMaxTop = dStart = dMaxBottom = dEnd;

		nDataCount++;
		// 2.1.1.2 이후 Box Data를 확인한다.
		while( posTop && posBottom && posStart && posEnd && nDataCount < nBoxWidth)
		{
			dTop	= pTopPriceList->GetPrev(		posTop);
			dBottom	= pBottomPriceList->GetPrev(	posBottom);
			dStart	= pStartPriceList->GetPrev(		posStart);
					  pEndPriceList->GetPrev(		posEnd);		// 종가는 이미 결정 났다. 반영하지 않는다.
			if( dMaxTop < dTop)			dMaxTop = dTop;
			if( dBottom < dMaxBottom)	dMaxBottom = dBottom;
			nDataCount++;
		}

		// 2.1.2 확인된 가격 Data 만큼 Box Data를 구성시킨다.
		for( ; 0 < nDataCount; nDataCount--)
		{
			pTopList->AddHead(		dMaxTop);
			pBottomList->AddHead(	dMaxBottom);
			pStartList->AddHead(	dStart);
			pEndList->AddHead(		dEnd);
		}
	}
}

// (2004.10.20, 배승원, 분석, 오류 수정) 분/틱 Chart의 Box Chart Data를 산출한다.
//		Real 수신시 Boxing 대상 봉이 늘어날 수 있다.
//		따라서, 특정 중간의 봉이 조회시점에 따라 Box상의 위치가 변하지 않도록 (일/주/월 방식으로 하면 변하게 된다.)
//			첫봉에서 Box가 완전하도록 시작하여, 끝봉에서 Box가 미완으로 끝날 수 있도록 한다.
//		그렇게 하여 Real을 수신하면서 Box를 마무리지어 갈수 있도록 한다.
void CBoxChart::GetForwardBoxChart(const CPoint& point, const double dCondition,
									CList<double, double>* pTopPriceList,
									CList<double, double>* pBottomPriceList,
									CList<double, double>* pStartPriceList,
									CList<double, double>* pEndPriceList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pBoxStartPacket || !m_pBoxHighPacket || !m_pBoxLowPacket || !m_pBoxEndPacket) return;

	// 0. 고/저 Data Packet을 구하고, 중간 산출 변수를 Clear한다.
	CList<double, double>* pStartList	= m_pBoxStartPacket->GetnumericDataList();
	CList<double, double>* pTopList		= m_pBoxHighPacket->GetnumericDataList();
	CList<double, double>* pBottomList	= m_pBoxLowPacket->GetnumericDataList();
	CList<double, double>* pEndList		= m_pBoxEndPacket->GetnumericDataList();

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pTopPriceList || !pBottomPriceList || !pStartPriceList || !pEndPriceList
		|| !pTopList || !pBottomList || !pStartList || !pEndList) return;

	// 1. 기존에 산출한 Box Chart Data를 Clear시킨다.
	//		Clear 예외의 경우가, 첨부터 다시 계산하는 경우 즉 TR이므로 역시 Clear되는 경우이다.
	//		즉, 항상 Clear되게 된다.
	// (2004.10.20, 배승원) Box를 구성해 가는 상황에서 Real이 들어온다고,
	//		맨 마지막 Box (구성하는 입장에서)만 재구성하는 Logic이 아니므로,
	//		항상 Clear시키도록 하여야 한다.
	pTopList->RemoveAll();
	pBottomList->RemoveAll();
	pStartList->RemoveAll();
	pEndList->RemoveAll();

	// 2. Box Chart의 시고 List를 구성한다.
	// 2.0 먼저 고가와 종가의 Data수가 같지 않으면 진행하지 않는다.
	int nTopPriceCount = pTopPriceList->GetCount();
	if( nTopPriceCount != pBottomPriceList->GetCount()
		&& nTopPriceCount != pStartPriceList->GetCount()
		&& nTopPriceCount != pEndPriceList->GetCount()) return;
	// 2.1 가격차트 고가와 종가의 첨부터 값을 추출하면서 진행한다.
	int			nDataCount = 0;
	int			nBoxWidth = (int)dCondition;
	double		dTop, dBottom, dMaxTop, dMaxBottom, dStart, dEnd;
	POSITION	posTop		= pTopPriceList->GetHeadPosition();
	POSITION	posBottom	= pBottomPriceList->GetHeadPosition();
	POSITION	posStart	= pStartPriceList->GetHeadPosition();
	POSITION	posEnd		= pEndPriceList->GetHeadPosition();
	while( posTop && posBottom && posStart && posEnd)
	{
		// 2.1.1 이번에 구성시킬 Box Chart의 고가와 저가를 산출한다.
		// 2.1.1.1 첫 Box Data를 확인한다.
		dMaxTop		= pTopPriceList->GetNext(		posTop);
		dMaxBottom	= pBottomPriceList->GetNext(	posBottom);
		dStart		= pStartPriceList->GetNext(		posStart);
		dEnd		= pEndPriceList->GetNext(		posEnd);
		nDataCount++;
		// 2.1.1.2 이후 Box Data를 확인한다.
		while( posTop && posBottom && posStart && posEnd && nDataCount < nBoxWidth)
		{
			dTop	= pTopPriceList->GetNext(		posTop);
			dBottom	= pBottomPriceList->GetNext(	posBottom);
					  pStartPriceList->GetNext(		posStart);		// 시가는 이미 결정 났다. 반영하지 않는다.
			dEnd	= pEndPriceList->GetNext(		posEnd);
			if( dMaxTop < dTop)			dMaxTop = dTop;
			if( dBottom < dMaxBottom)	dMaxBottom = dBottom;
			nDataCount++;
		}

		// 2.1.2 확인된 가격 Data 만큼 Box Data를 구성시킨다.
		for( ; 0 < nDataCount; nDataCount--)
		{
			pTopList->AddTail(		dMaxTop);
			pBottomList->AddTail(	dMaxBottom);
			pStartList->AddTail(	dStart);
			pEndList->AddTail(		dEnd);
		}
	}
}

bool CBoxChart::ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList,
							CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL)
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		m_pBoxStartPacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BOX_CHART) + _MTEXT( C0_OPEN), false);
		m_pBoxHighPacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BOX_CHART) + _MTEXT( C0_HIGH), false);
		m_pBoxLowPacket		= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BOX_CHART) + _MTEXT( C0_LOW), false);
		m_pBoxEndPacket		= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BOX_CHART) + _MTEXT( C0_CLOSE), false);
		
		ClearPacketData( m_pBoxStartPacket);
		ClearPacketData( m_pBoxHighPacket);
		ClearPacketData( m_pBoxLowPacket);
		ClearPacketData( m_pBoxEndPacket);
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSubGraphPacketList) return FALSE;

	pSubGraphPacketList->AddTail2( m_pBoxStartPacket);
	pSubGraphPacketList->AddTail2( m_pBoxHighPacket);
	pSubGraphPacketList->AddTail2( m_pBoxLowPacket);
	pSubGraphPacketList->AddTail2( m_pBoxEndPacket);

	return (m_pBoxStartPacket != NULL && m_pBoxHighPacket != NULL && m_pBoxLowPacket != NULL
		&& m_pBoxEndPacket != NULL);
}
bool CBoxChart::SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
								   CPacketList* pPacketList,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
		{
			return (ClearPacket(calculatetype, pPacketList, pSubGraphList, pIndicatorInfo));
		}
		if(calculatetype == CCalculateBaseData::TRDATA_CAL)
		{
			ClearPacketData( m_pBoxStartPacket);
			ClearPacketData( m_pBoxHighPacket);
			ClearPacketData( m_pBoxLowPacket);
			ClearPacketData( m_pBoxEndPacket);
		}
	}
	return ( m_pBoxStartPacket != NULL && m_pBoxHighPacket != NULL && m_pBoxLowPacket != NULL
		&& m_pBoxEndPacket != NULL);
}

//******************************************************************************************************************
//	CFourierTransform
//	  
//******************************************************************************************************************
CFourierTransform::CFourierTransform() :
	CIndicator( IMetaTable::EDDC_OCX_FOURIER_TRANSFORM)
{
}
void CFourierTransform::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.02.05
//
//	분석	:	선물 전용 P&F Chart (옵션 아님. 선물전용 호가 단위 기준으로 Hard Codeing 되어 있음.)
//				1. 기본사항
//					1. 선물 전용 P&F는 t분봉 종가 혹은 Tick의 Price를 기준으로 n호가 단위의 전환도이다.
//						(삼선 전환도와 유사)
//					2. 상승은 'X', 하락은 'O'표를 하여 MarkLine을 구성한다.
//					3. 등락의 방향이 이전 고/저 가격의 n호가 단위 반전되어야만 방향 전환을 인정하고,
//						전환 깃점 다음부터의 Marking을 처리하게 된다.
//					4. Mark는 항상 호가단위 (0.05)마다 표시한다.
//					5. 일자가 변경되는 경우, 해당 Mark부터 색상을 변경한다. (적색과 청색을 반복하며 전환한다.)
//				2. 개발 정보
//					1. TR에 의한 조회시에는 Server에서 이미 P&F Chart를 그리기 위한 Data만을 집계하여 내려준다.
//						(각 Packet은 하나의 등락 MarkLine이다.)
//							자료일자	: 해당 MarkLine에 영향을 준(추세를 확장시킨) 마지막 Data의 시각이다.
//											(추세를 확장시키지 않고, 비교만된 Data의 시각은 반영하지 않는다.
//											 새일자가의 판단 기준이 되기 위함이다.)
//										  P&F Chart에는 출력되지 않지만, 이후 Real의 수신시에 일자 변경의 기준이 된다.
//							시가		: MarkLine의 시작 가격이다.
//							종가		: MarkLine의 끝 가격이다.
//										  MarkLine의 시종 가격에 의한 등락여부로 상승의 경우 'X', 하락의 경우 'O'표시한다.
//							새일자가	: 하나의 MarkLine내에서 일자가 변경되는 경우에 색상을 변경할 Mark의 가격이다.
//											새일자가가 없는 경우는 0으로 설정된다.
//											(주의, 새일자가는 그날의 첫 봉의 종가 Data가 아니다.
//												새일자가는 그날의 첫 확장 및 반전에 의해 생성되는 첫 Mark가격이다.
//												따라서 특정일이 전일 추세를 확장하거나 반전하지 않는 경우,
//												새일자가가 존재하지 않을 수 있다.)
//											(반대로, 하나의 추세에 일자가 두번 바뀌면서 추세가 형성되면,
//													새일자가는 두 개의 값이어야 한다.
//												그러나 1개 추세를 두 개의 Packet으로 나눠구현할 수 있는 구조를 감안하지 않아,
//													최종의 새일자가만 기억토록한다.
//												특히 이런 경우는 반전호가단위를 일일평균 등락폭보다 크게 잡았을 때나타나는 현상으로,
//													일단 반전호가단위의 Maximum을 작게하여 해결하는 방향으로 조절한다. - 문준철 차장 승인 처리 2004.02.26)
//					2. Server에서 내려주는 Packet에는 특수한 경우를 처리하기 위한 보조 Packet이 내려온다.
//						1. 제1 기준 Packet (기준종가 Packet)	: 모든 Packet Data에 앞서 항상 들어오는 첫번째 기본 Packet이다.
//								자료일자	: 금일(조회당일)의 장시작 시간이다.
//												금일이 영업일이 아닌 경우, (Real이 없겠지?) 0으로 들어온다.
//												이 값은 조회당일, 장시작전, 분봉으로, 당일 Data만을 조회하는 경우에,
//													첫 봉을 구성하기 위한 시각으로 1분봉의 경우 1분정각의 Data '이전'까지 포함하여 봉이 형성된다.
//								시가		: 자료조회 시작일의 '전일종가'
//												Server에서 형성된 (정/소)추세가 없는 경우, 화면에 출력할 기준가이다.
//												소추세 형성시 초기 시/종가로 이용된다.
//								종가		: 이용않음
//								새일자가	: 이용않음
//						2. 제 2기준 Packet (끝봉 Packet)	: 모든 Packet Data에 앞서 항상 들어오는 두번째 기본 Packet이다.
//												끝봉은 이후의 Real 수신에 의하여 완성되는 미완성 봉으로 간주하여 별도의 Packet으로 받는다.
//												즉 아래의 소추세나 추세 Packet에 가산되지 않고 별도로 수신하여 가추세 Packet으로 구성하게 된다.
//												Tick인 경우에도 마지막 Tick을 끝봉으로 받는다.
//								자료일자	: 끝봉의 시작시간이다.
//								시가		: 이용않음
//								종가		: 끝봉의 종가이다.
//								새일자가	: 이후 Packet Type (0:정추세 or NoPacket, 1:소추세)
//						2. 소추세 Packet Data	: Server에 조회된 Data가 있으나,
//											  Server에서 구성된 추세가 하나도 없는 경우 내려오는, 추세 추적값 Packet이다.
//								[소추세]	: 보통 추세는 이전 추세의 마지막 추세치를 기준으로 반전을 확인하게되는데,
//													첫 추세는 이전 추세가 없는 관계로 상승/하락의 추세를 동시에 추적하면서,
//													그 폭이 지정 호가를 넘어서는 시점에 추세로 인정하게 된다.
//													이때, 소추세의 시가를 +/- 0.05로하여 이전 시가 기준의 반전으로 인정토록한다.
//												추세 추적중의 고/저가를 갱신시에는 마지막으로 갱신된 것이 종가로 나타나도록 한다.
//												특히 소추세는 Drawing Packet에는 등록되지 않는다. 단지 가추세에 반영되어 보인다.
//												그것은 소추세와 정추세의 다음 추세 추적 방식이 틀리기 때문에 굳이 통일할 필요성이 없고,
//													또한 소추세를 출력시에는 이후 가추세 출력의 통합방식이 또한 정추세와 틀리기에,
//													소추세는 출력용 Packet으로 등록하지 않는다.
//												소추세라 하더라도, 그 고/저 폭이 반전가격폭 이상이 되는 경우가 있다.
//													따라서, 소추세임을 제 2기준 Packet의 새일자가를 확인하여 이용한다.
//								자료일자	: 추세 추적을 확장시킨 마지막 Data의 시각이다.
//								시가		: 추세 추적 시가로, 소추세를 구성하는 동안 발생한 최저가 혹은 최고가이다.
//								종가		: 추세 추적 종가로, 소추세를 구성하는 동안 발생한 최저가 혹은 최고가이다.
//								새일자가	: 추세 추적중에 발생한 새일자가이다.
//						3. 추세 Packet Data	: Server에 구성된 추세 Packet이다.
//								자료일자	: 추세를 확장시킨 마지막 Data의 시각이다.
//								시가		: 추세의 시작 가격이다. 이전 추세의 종가보다 0.05가 크거나 (상승의 경우) 작다 (하락의 경우)
//								종가		: 추세의 끝 가격이다. 시가와 종가로 상승추세인지 하락추세인지 식별한다.
//								새일자가	: 추세 추적중에 발생한 새일자가이다.
//					3. 출력용 보조 Packet
//						1. Scale Packet		: Drawing 처리시 Y Scale이 가추세에 의해 급변하는 경우를 제한하기 위하여,
//												Drawing된 값의 Min/Max Data를 관리하여, 지속적인 축소만이 가능하도록 지원하는 Packet이다.
//											  Drawing 처리시의 일관적으로 처리가 가능하도록 항상 맨 처음 추가한다.
//								자료일자	: 자료구조 없음
//								시가		: 전체 출력 Data의 최소값
//								종가		: 전체 출력 Data의 최대값
//								새일자가	: 이용 없음
//						2. 가추세 Packet Data	: Server에서 내려주는 Packet이 아닌, Real의 분봉 처리시 Tick에 의해 형성되는 추세 Packet이다.
//								[가추세]	: 가추세는 분봉의 최종 종가가 확정되지 않는 상황에서의 임시 추세를 나타내는 Packet으로,
//													Server가 아닌, 지표 산출시 제 2기준 Packet을 바탕으로 생성되며,
//													Tick에 의한 추세 반전이나 확장을 나타내는 Packet으로 항상 등록된다.
//											  가추세는 지표 출력 Routine의 일관성을 위하여, Real 뿐만 아니라 TR의 경우에도 추가한다.
//											  또한 가추세는 출력시 그 추세폭을 확인하여 반전폭이 넘어서는 경우에만 반전으로,
//													그리고, 추세확장인 경우에만 확장으로 출력처리한다.
//											  이때, 이전 추세를 확장하여도 별도 Packet으로 두어, 상승이후 상승의 가추세가 가능하다.
//													Drawing시에 취합하여 처리한다.
//											  특히 가추세는 소추세를 포함하여 보여준다. 이경우 끝봉에 의한 소추세 추적이 확정될 때,
//													이전 가추세를 소추세 추적 변수에 반영하여, 새로운 소추세를 형성토록한다.
//								자료일자	: 자료구조 없음
//								시가		: Tick에 의해 임시로 생성된 추세의 시가이다.
//								종가		: Tick에 의해 임시로 생성된 추세의 종가이다.
//								새일자가	: 가추세를 형성하는 끝봉이 이전추세에 확장/반전을 발생시키면서,
//												날짜가 다른 경우, 확장/반전되는 첫 Mark의 가격이다. (가추세의 시가이다.)
//												(주의, 추세의 종가가 아니다.)
//					4. Real 처리
//						1. 소추세 형성	:
//								1. Server에서 추세 Packet Data가 없이, 기준 Packet만, 혹은 소추세 Packet과 함께
//										내려오는 경우이다.
//								2. 소추세는 지표처리시 Packet으로 관리하지 않고, 최종 상황 변수로만 관리한다.
//								3. 각 분봉의 종가만을 취하여 소추세를 확장하여 나간다. (추세가 확정될때까지)
//								4. 기준 Packet의 시가(조회시작일 전일 종가)를 시종으로 설정한 가추세 Packet만을 등록한다.
//								5. 추세가 형성될 경우, 새로운 추세 Packet으로 등록시킨다.
//						2. 추세 진행	:
//								1. Server에서 추세 Packet Data를 내려주거나, Real에 의해 추세가 확정된 경우이다.
//								2. 각 Real Data를 적용하여 추세의 확장 또는 반전의 가추세 Packet을 등록한다.
//						3. Scale Packet 관리	:
//								1. 소추세/정추세/가추세를 포함한 모든 추세 Data들의 Drawing을 위하여
//										추세 Data들의 Min/Max를 Packet으로 관리한다.
//								2. 첫 지표 Packet의 시/종 가로 계속 Update한다.
//						4. 가추세 Packet 관리	:
//								1. 분봉 추세추적시 Real에의한 마지막 미완성 분봉의 추세를 관리하는 Packet으로,
//										지표 Packet의 마지막에 항상 등록시킨다.
///////////////////////////////////////////////////////////////////////////////
// (2004.02.05, 배승원, 분석) 선물 전용 P&F Chart 지표 Contructor
CFuOpPF::CFuOpPF() : CIndicator( IMetaTable::EDDC_OCX_FUOPPANDFCHART)
{
	// (2004.02.05, 배승원, 분석) Input Packet을 관리한다.
	m_pIMLDatePacket			= NULL;	// 자료일자 Packet
	m_pIMLStartPricePacket		= NULL;	// MarkLine 시가 Packet
	m_pIMLEndPricePacket		= NULL;	// MarkLine 종가 Packet (Real 채결가 Packet)
	m_pIMLNewDatePricePacket	= NULL;	// 새일자가 Packet

	// (2004.02.05, 배승원, 분석) Output Packet을 관리한다.
	//		(출력시에는 Date Packet을 필요로 하지 않는다.
	//		 각 Mark마다 시간을 명시하지 않고, MarkLine 단위의 시간은 의미가 없다.)
	m_pOMLStartPricePacket		= NULL;	// MarkLine 시가 Packet
	m_pOMLEndPricePacket		= NULL;	// MarkLine 종가 Packet
	m_pOMLNewDatePricePacket	= NULL;	// 새일자가 Packet

	// (2004.02.24, 배승원) P&F 지표를 산출하기 위한 조건값을 구하여 관리한다.
	m_nStickTimeWidth				= 0;	// Real 취합 시간 (분단위, 0은 Tick)
	m_nNewTrendPriceX100			= 0;	// 추세반전 호가단위

	// (2004.02.24, 배승원) Real을 처리하기 위한 최종 상태를 관리한다.
	m_nLastStickTime				= 0;	// 끝봉의 시작시간 (이것을 기준으로 Real을 봉으로 취합한다.)

	// (2004.02.24, 배승원) 소추세 추적 변수를 관리한다.
	m_nMiniTrendTime				= -3;	// 소추세의 마지막 시간이다. (기본 오류의 상태로 초기화한다.)
	m_nMiniTrendStartX100			= 0;	// 소추세의 시가이다.
	m_nMiniTrendEndX100				= 0;	// 소추세의 종가이다.
	m_nMiniTrendNewDatePriceX100	= 0;	// 소추세의 새일자가이다.
	m_nFirstBasePriceX100			= 0;	// 첫 소추세를 Real로 형성시에 참조할 조회 전일 종가를 관리한다.

	// (2004.03.01, 배승원) 마지막 정추세의 시각을 관리한다. (가추세의 새일자가 산출시 참조한다.
	m_nFixedTrendTime				= 0;

	// (2004.03.05, 배승원) 끝봉 시각을 추적하면서 Real을 취합하는 것은 당일 끝봉인 경우뿐이다.
	//		따라서, 장시작 시각을 별도로 관리하여, 첫 Real 수신시 새일자가를 산출하도록 한다.
	m_nMarketStartTime				= 0;
}

// (2004.02.05, 배승원, 분석) 선물 P&F Chart의 지표를 구성하는 기본 Routine이다.
void CFuOpPF::GetData( const CChartFormulateData &chartFormulateData,
		CGraphCalculateData *pGraphCalculateData, CIndicatorInfo * &pIndicatorInfo,
		CCalculateData &calculateData)
{
	// 1. 먼저 TR 수신시의 I/O Packet을 초기화한다.
	//	(기본 지표 Data를 구성한다.)
	CPacketList *pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		// 1.1 Input Packet을 확보하여 관리한다. (자료일자, MarkLine 시가, MarkLine 종가, 새일자가 Pakcet으로 구성된다.)
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pIMLDatePacket			= pPacketList->GetBaseData( CDataConversion::GetStringData( strPacketNames, ","));
		m_pIMLStartPricePacket		= pPacketList->GetBaseData( CDataConversion::GetStringData( strPacketNames, ","));
		m_pIMLEndPricePacket		= pPacketList->GetBaseData( CDataConversion::GetStringData( strPacketNames, ","));
		m_pIMLNewDatePricePacket	= pPacketList->GetBaseData( CDataConversion::GetStringData( strPacketNames, ","));
		if( !m_pIMLDatePacket || !m_pIMLStartPricePacket || !m_pIMLEndPricePacket || !m_pIMLNewDatePricePacket) return;
		// 1.2 Output Packet을 확보하여 초기화/관리한다. (CGraphCalculateData에도 등록시켜 준다.)
		// 1.2.1 Packet List로부터 Packet을 검색한다.
		m_pOMLStartPricePacket		= pPacketList->GetBaseData( CString( "FUPF_ML") + _MTEXT( C0_OPEN),		FALSE);
		m_pOMLEndPricePacket		= pPacketList->GetBaseData( CString( "FUPF_ML") + _MTEXT( C0_OPEN),		FALSE);
		m_pOMLNewDatePricePacket	= pPacketList->GetBaseData( CString( "FUPF") + "_" + _MTEXT( C0_NEW_DATE_PRICE),	FALSE);
		// 1.2.2 검색 실패시 처리를 종료한다.
		if( !m_pOMLStartPricePacket || !m_pOMLEndPricePacket || !m_pOMLNewDatePricePacket) return;
		// 1.2.3 검색된 Packet을 초기화한다.
		ClearPacketData( m_pOMLStartPricePacket		);
		ClearPacketData( m_pOMLEndPricePacket		);
		ClearPacketData( m_pOMLNewDatePricePacket	);
		// 1.2.4 CGraphCalculateData에 Output Packet을 등록시킨다.
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pGraphCalculateData) return;
		if( pGraphCalculateData->GetCount() < 1) return;
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pSubGraphList) return;
		if( pSubGraphList->GetCount() < 1)	// (처음 한번만 등록시켜준다.)
		{
			pSubGraphList->AddTail2( m_pOMLStartPricePacket		);		// 등록 순서에 주의한다.
			pSubGraphList->AddTail2( m_pOMLEndPricePacket		);
			pSubGraphList->AddTail2( m_pOMLNewDatePricePacket	);
		}

		// (2004.02.24, 배승원) 실시간 Tick P&F Chart의 반전호가 단위와 분봉 취합시간 조건을 구하여 관리한다.
		//		(이후 Real이나 TR 처리시 참조한다.)
		CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
		if( !pConditionList) return;
		m_nNewTrendPriceX100 = int( GetCondition( 0, pConditionList) * 100);
		m_nStickTimeWidth = int( GetCondition( 1, pConditionList));

		// 1.3 TR Data를 기반으로 지표 Data를 구성한다.
		GetTRFuOpPF();
	}
	// 2. Real의 지표 Data 구성을 처리한다.
	else if( chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL)
		GetRealFuOpPF( calculateData.GetPoint());

	// 3. 다음 Real의 처리를 위하여 Real 수신 Packet Data를 Clear한다.
	if( m_pIMLDatePacket->GetCount() > 0 && m_pIMLEndPricePacket->GetCount() > 0)
	{
		m_pIMLDatePacket->ClearData(		TRUE);
		m_pIMLEndPricePacket->ClearData(	TRUE);
	}

	// 4. 구성된 Output Packet을 등록시킨다.
	CString strPriceType = m_pIMLEndPricePacket->GetType( FALSE);
	SetPacketTypes( m_pOMLStartPricePacket,		TYPE_TWODECIMAL, strPriceType, 0);
	SetPacketTypes( m_pOMLEndPricePacket,		TYPE_TWODECIMAL, strPriceType, 0);
	SetPacketTypes( m_pOMLNewDatePricePacket,	TYPE_TWODECIMAL, strPriceType, 0);
}

// (2004.02.05, 배승원, 분석) 선물 P&F Chart의 TR을 처리하는 Routine을 제공한다.
// (2004.02.24, 배승원) Server에서 내려주는 기준 Packet, 소추세 Packet, 추세 Packet을 처리하고,
//		가추세 Packet을 추가하여 출력토록한다.
void CFuOpPF::GetTRFuOpPF( void)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pIMLDatePacket || !m_pIMLStartPricePacket || !m_pIMLEndPricePacket || !m_pIMLNewDatePricePacket) return;

	// 1. Input Packet의 DataList들을 구한다.
	CList< double, double> *pMLDateList			= m_pIMLDatePacket->GetnumericDataList();
	CList< double, double> *pMLStartPriceList	= m_pIMLStartPricePacket->GetnumericDataList();
	CList< double, double> *pMLEndPriceList		= m_pIMLEndPricePacket->GetnumericDataList();
	CList< double, double> *pMLNewDatePriceList	= m_pIMLNewDatePricePacket->GetnumericDataList();
	if( !pMLDateList || !pMLStartPriceList || !pMLEndPriceList || !pMLNewDatePriceList) return;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pOMLStartPricePacket || !m_pOMLEndPricePacket || !m_pOMLNewDatePricePacket) return;

	// (2004.02.24, 배승원) Scale Packet을 무조건 추가하여, Drawing시에 참조토록 한다.
	// (2004.03.05, 배승원) Min 값이 0인 경우는 기본 제2 Packet의 시/종가가 0인 끝봉없는 경우로
	//		이는 Scale에 감안시키지 않는다.
	double dI, dMin = DBL_MAX, dMax = DBL_MIN;
	POSITION posMLStartPrice	= pMLStartPriceList->GetHeadPosition();
	while( posMLStartPrice)
	{
		dI = pMLStartPriceList->GetNext( posMLStartPrice);
		if( 0 < dI)
		{
			if( dI < dMin) dMin = dI;
			else if( dMax < dI) dMax = dI;
		}
	}
	POSITION posMLEndPrice	= pMLEndPriceList->GetHeadPosition();
	while( posMLEndPrice)
	{
		dI = pMLStartPriceList->GetNext( posMLEndPrice);
		if( 0 < dI)
		{
			if( dI < dMin) dMin = dI;
			else if( dMax < dI) dMax = dI;
		}
	}

	// (2004.04.09, 배승원) 장 시작시, 전일종가의 표시가 지나치가 커지지 않도록,
	//		기본 폭을 확보한다.
	const double dWidth = 0.5;
	if( dMax - dMin < dWidth)
	{
		int nPaddingX100 = ( ( ( int( dWidth * 100) - int( dMax - dMin) * 100) / 2) / 5) * 5;
		dMin = ( int( dMin * 100) - nPaddingX100) / 100.;
		dMax = ( int( dMax * 100) + nPaddingX100) / 100.;
	}

	m_pOMLStartPricePacket->AppendData(		dMin);
	m_pOMLEndPricePacket->AppendData(		dMax);
	m_pOMLNewDatePricePacket->AppendData(	0);

	// (2004.02.26, 배승원) 소추세 추적 변수를 초기화한다.
	//		특히, Server의 Packet에 문제가 있는 경우를 감안하여
	//			Server Packet을 확인하기에 앞서, 소추세 추적 변수를 초기화한다.
	//		Server Packet의 오류시에는 정추세가 이미 있는 것처럼 설정하여,
	//			Real에 의한 소추세 추적이 진행되지 않도록 한다.
	m_nMiniTrendTime				= -3;
	m_nMiniTrendStartX100			= 0;
	m_nMiniTrendEndX100				= 0;
	m_nMiniTrendNewDatePriceX100	= 0;

	// 2. MarkLine 시가/종가, 새일자가 Packet을 Output Packet으로 복사처리한다.
	POSITION	posMLDate		= pMLDateList->GetHeadPosition();
				posMLStartPrice	= pMLStartPriceList->GetHeadPosition();
				posMLEndPrice	= pMLEndPriceList->GetHeadPosition();
	POSITION	posNewDatePrice	= pMLNewDatePriceList->GetHeadPosition();
	
	// (2004.02.24, 배승원) Server에서 내려주는 기준 Packet, 소추세 Packet, 추세 Packet을 처리하고,
	//		가추세 Packet을 추가하여 출력토록한다.
	// 2.1 먼저 제1 기준 Packet을 추출하여 Real에 의한 소추세 추적 변수를 초기화한다. (기본 Packet은 항상 수신된다.)
	//		(제1 기준 Packet도 없는 경우에는 Real이나 가추세 Packet 구성하지 않는다.)
	if( !posMLDate || !posMLStartPrice || !posMLEndPrice || !posNewDatePrice) return;
	// 2.1.1 금일 장시작 시간을 수신한다.
	//	(Real에 의한 소추세 추적의 마지막 봉 시작 시간에 이용된다.
	//	 특히 봉이 하나도 없는 경우에만 이며, 봉이 있으면 그 봉의 시작시간으로 Update된다.)
	// (2004.03.05, 배승원) 끝봉 시각을 추적하면서 Real을 취합하는 것은 당일 끝봉인 경우뿐이다.
	//		따라서, 장시작 시각을 별도로 관리하여, 첫 Real 수신시 새일자가를 산출하도록 한다.
	m_nMarketStartTime = int( pMLDateList->GetNext( posMLDate));
	// 2.1.2 조회일 이전 종가를 추출한다.
	//	(소추세가 없는 경우의 가추세 시/종가에 이용된다.
	//	 또한 이후 Real 수신시에는 첫 소추세 형성시 시가에 참조된다.)
	m_nFirstBasePriceX100	= int( pMLStartPriceList->GetNext( posMLStartPrice) * 100);
	// 2.1.3 그외 값은 무시한다.
	pMLEndPriceList->GetNext(		posMLEndPrice	);
	pMLNewDatePriceList->GetNext(	posNewDatePrice	);
	// 2.1.4 제2 기준 Packet이 없는 경우도 역시 오류로 Real이나 가추세 Packet을 구성하지 않는다.
	if( !posMLDate || !posMLStartPrice || !posMLEndPrice || !posNewDatePrice) return;
	
	// 2.2 제2 기준 Packet을 추출하여 끝봉의 정보를 관리한다. (Real 수신시 이용한다.)
	// 2.2.0 먼저 제 2기준 Packet에서 이용치 않는 정보를 Skip한다.
	pMLStartPriceList->GetNext(		posMLStartPrice	);
	// (2004.03.01, 배승원) 제2 기본 Packet의 새일자가를 확인하여 이후 Packet이 소/정 추세임을 확인한다.
	//		(0이면 정추세 or No Packet, 1이면 소추세)
	int nPacketType = ( int)pMLNewDatePriceList->GetNext(	posNewDatePrice);
	// 2.2.1 끝봉의 시작시간을 확인한다.
	//	(이 값이 0이면 Server에서 조회된 봉이 하나도 없는 경우이다.
	//	 Real 수신을 통해 봉을 형성시, 봉마감시각을 판단하기 위한 기준시각으로 이용된다.)
	m_nLastStickTime = int( pMLDateList->GetNext( posMLDate));
	// 2.2.2 끝봉의 종가를 확인한다.
	//	(Real에 의해 구성된 끝봉의 종가로 인정하여 가추세 Packet을 형성하는데 이용한다.)
	int nLastStickPriceX100 = int( pMLEndPriceList->GetNext( posMLEndPrice) * 100);
	// 2.2.3 조회된 끝봉의 시작시간에 따라, Real 처리용 끝봉 시작시간이나 끝봉 종가를 수정한다.
	//	(봉이 없는 경우나, 전일의 끝봉인 경우 장시작시간을 기준으로 처리하기 위하여 Update하지 않는다. - 분봉의 경우
	//		또한 분봉이 있다해도 그것이 금일의 마지막 봉이어야만 한다.
	//	 특히 TR로 수신한 끝봉 종가는 Real로 수신한 것처럼 소추세나 마지막 정추세를 연장하는,
	//		가추세 생성에 이용하지만,
	//	 Real의 처리를 위한 끝봉 관리시간과 TR에서 제공하는 끝봉시각의 개념이 같지 않음을 주의한다.
	//		따라서, Real의 끝봉 시각을 장시작시각이 아닌 새로운 시각으로 Update하지 않는다고 하여,
	//		끝봉 가격을 Clear시키는 오류를 범하지 않도록 주의한다.)
	//   추세 Packet이 하나도 (소추세/정추세 둘다) 없는 경우는, 조회전일종가를 이용한 가추세 Packet을 추가한다.
	if( !posMLDate || !posMLStartPrice || !posMLEndPrice || !posNewDatePrice)
	{
		int nStartPriceX100	= m_nFirstBasePriceX100;
		int nEndPriceX100	= m_nFirstBasePriceX100;

		// (2004.02.26, 배승원) 소추세의 추적이 아직 시작하지 않은 경우를 처리한다.
		//		이때 끝봉이 Data가 없는 경우 -2, 있는 경우 -1로 설정한다.
		if( m_nLastStickTime)
		{
			m_nMiniTrendTime = -1;

			// 2.2.3.1 끝봉 종가로 가추세의 종가를 설정한다.
			nEndPriceX100	= nLastStickPriceX100;

			// 2.2.3.2 가추세의 시가는 가추세의 추세방향에 따라 설정한다.
			if( m_nFirstBasePriceX100 < nLastStickPriceX100)		nStartPriceX100 = m_nFirstBasePriceX100 + 5;
			else if( nLastStickPriceX100 < m_nFirstBasePriceX100)	nStartPriceX100 = m_nFirstBasePriceX100 - 5;
			else													nStartPriceX100	= m_nFirstBasePriceX100;
		}
		else m_nMiniTrendTime = -2;

		m_pOMLStartPricePacket->AppendData(		nStartPriceX100 / 100.);
		m_pOMLEndPricePacket->AppendData(		nEndPriceX100 / 100.);
		m_pOMLNewDatePricePacket->AppendData(	0);

		return;
	}
	
	// 2.3 소추세 Packet이 오는지 확인하여 처리한다.
	//	(우선 추출하고, 소추세의 고/저가 폭이 반전 호가 단위에 미치면 첫 추세로 등록시킨다.
	//	 그건 Server에서 소추세로 제공한 것이 아니기 때문이다.)
	// 2.3.1 소추세 추정값을 구한다.
	double dMLDate			= pMLDateList->GetNext(			posMLDate		);
	double dMLStartPrice	= pMLStartPriceList->GetNext(	posMLStartPrice	);
	double dMLEndPrice		= pMLEndPriceList->GetNext(		posMLEndPrice	);
	double dNewDatePrice	= pMLNewDatePriceList->GetNext(	posNewDatePrice	);
	// 2.3.2 소추세로 확인되는 경우를 처리한다.
	//	(실제로 소추세로 확인되는 경우, 소추세 추적 변수를 초기화하고,
	//	 끝봉 Data를 감안하여 가추세를 생성한다.)
	// (2004.03.01, 배승원) Server로 부터 내려오는 소추세 Packet이 정추세와 식별되지 않으므로,
	//		별도의 Flag (제2 기본 Packet의 새일자가)를 확인하여 처리한다.
	if( nPacketType == 1)
	{
		// 2.3.2.1 먼저 소추세의 마지막 봉시각을 받는다.
		//	(이후 Real 처리에서 정추세로 확정될때 등록될 시간이다.)
		m_nMiniTrendTime = int( dMLDate);
		// 2.3.2.2 소추세의 저가를 확인한다.
		//	(이후 Real 처리에서 추세 확정시, 시가로 이용된다.)
		m_nMiniTrendStartX100 = int( dMLStartPrice * 100);
		// 2.3.2.3 소추세의 고가를 확인한다.
		//	(이후 Real 처리에서 추세 확정시, 종가로 이용된다.)
		m_nMiniTrendEndX100 = int( dMLEndPrice * 100);
		// 2.3.2.4 소추세의 새일자가를 확인한다.
		//  (이후 Real 처리에서 추세 확정시의, 새일자가로 이용된다.)
		m_nMiniTrendNewDatePriceX100 = int( dNewDatePrice * 100);
		// 2.3.2.5 설정된 소추세에 끝봉 Data를 감안하여 가추세를 등록시킨다.
		// 2.3.2.5.1 끝봉의 종가가 있는 경우, 소추세를 감안하여 가추세를 등록한다.
		//	(소추세는 Drawing Packet으로 등록되지 않는다. 가추세에 반영되어 등록된다.)
		if( m_nLastStickTime)
		{
			// 2.3.2.5.1.2 끝봉이 소추세를 확장시키는 경우를 처리한다. (확장 가추세이면 종가만을 수정한다.)
			//	- 확장 가추세 : 소추세를 더 상승시키거나, 하락 추세를 더 하락시키는 경우이다.
			//					확장의 경우, 같은 추세에 있는가만 확인한다. 아래의 IF문으로 이미 확장하고 있음을 확인할 수 있다.
			//					얼마나 확장시키는 것은 중요하지 않다. (1호가(0.05)만 확장해도 인정된다.)
			//	(주의, 소추세 추적 변수를 수정하지 않는다.
			//	 왜냐하면 다음 Real에서도 소추세 추적변수를 기준으로 이용하여야 하니까.)
			if( 0 < ( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) * ( nLastStickPriceX100 - m_nMiniTrendEndX100))
			{
				// 2.3.2.5.1.2.1 소추세를 반영하여 등록하는 가추세에, 끝봉에 의한 새일자가를 Update한다.
				//	(이미 소추세에는 소추세내의 새일자가가가 등록되어 있으므로 새일자인 경우만 새일자가를 반영하고,
				//		그렇지 않으면 원래값을 유지한다.
				//	 특히 그값은 소추세 확장 방향에 따라, 고/저가의 +/- 0.05이다.)
				if( m_nMiniTrendTime / 10000 != m_nLastStickTime / 10000)
				{
					// 2.3.2.5.1.2.2 하락의 경우, 종가보다 0.05적게 설정한다.
					if( m_nMiniTrendEndX100 < m_nMiniTrendStartX100)		dNewDatePrice = ( m_nMiniTrendEndX100 - 5) / 100.;
					// 2.3.2.5.1.2.3 상승의 경우, 종가보다 0.05많게 설정한다.
					else if( m_nMiniTrendStartX100 < m_nMiniTrendEndX100)	dNewDatePrice = ( m_nMiniTrendEndX100 + 5) / 100.;
				}
			
				// 2.3.2.5.1.2.4 가추세에 끝봉종가를 종가로 등록한다.
				dMLEndPrice = nLastStickPriceX100 / 100.;
			}
			// 2.3.2.5.1.3 끝봉이 소추세를 역으로 확장시키는 경우를 처리한다.
			//	(추세 반전을 확인하지 않고, 소추세를 역으로 확장시키는 경우만 확인한다.
			//	 소추세의 경우, 정추세가 아닌 관계로 반전 추세는 새로운 가추세로 흡수하게된다.)
			//  - 역확장 가추세 : 소추세를 역행하면서 시가보다 더 진행하는 경우이다.
			//					  이경우 소추세의 기본 종가를 시가로 이용하고, 끝봉의 가격을 종가로 이용하는
			//						새로운 소추세의 형태로 가추세를 등록하여야 한다.
			//					  (물론 추적중이므로 소추세 추적 변수는 갱신하지 않는다.)
			//	(추세 확인은 소추세의 방향을 역으로 놓고 확인한다.)
			if( 0 < ( m_nMiniTrendStartX100 - m_nMiniTrendEndX100) * ( nLastStickPriceX100 - m_nMiniTrendStartX100))
			{
				// 2.3.2.5.1.3.1 소추세를 반영하는 등록하는 가추세에, 끝봉에 의한 새일자가를 Update한다.
				//	(이미 소추세에는 소추세내의 새일자가가가 등록되어 있으므로 새일자인 경우만 새일자가를 반영하고,
				//		그렇지 않으면 원래값을 유지한다.
				//	 특히 그값은 소추세 확장 방향에 따라, 고/저가의 +/- 0.05이다.)
				if( m_nMiniTrendTime / 10000 != m_nLastStickTime / 10000)
				{
					// 2.3.2.5.1.3.2 하락의 경우, 시가보다 0.05많게 설정한다. (역추세 확장에 주의)
					if( m_nMiniTrendEndX100 < m_nMiniTrendStartX100)		dNewDatePrice = ( m_nMiniTrendStartX100 + 5) / 100.;
					// 2.3.2.5.1.3.3 상승의 경우, 시가보다 0.05적게 설정한다. (역추세 확장에 주의)
					else if( m_nMiniTrendStartX100 < m_nMiniTrendEndX100)	dNewDatePrice = ( m_nMiniTrendStartX100 - 5) / 100.;
				}

				// 2.3.2.5.1.3.4 소추세의 시가를 소추세 기본 종가로 반영한다.
				dMLStartPrice = dMLEndPrice;
				// 2.3.2.5.1.3.5 소추세의 종가를 끝봉의 종가로 반영한다.
				dMLEndPrice = nLastStickPriceX100 / 100.;
			}

			// 2.3.2.5.1.4 조정된 소추세의 폭이 반전 호가단위보다 큰 경우,
			//		소추세와 끝봉 Data를 취합하여 구성한 가추세를 정추세 형식으로 조정하여 보여준다.
			//		(소추세가 정추세로 인정될때, 시가를 +/- 0.05 처리하여야 한다.
			//		 시가기준의 반전으로 인정되는 정추세이기 때문이다.)
			if( m_nNewTrendPriceX100 <= abs( int( dMLEndPrice * 100) - int( dMLStartPrice * 100)))
			{
				if( dMLStartPrice < dMLEndPrice)	dMLStartPrice = ( int( dMLStartPrice * 100) + 5) / 100.;
				else								dMLStartPrice = ( int( dMLStartPrice * 100) - 5) / 100.;
			}

			// 2.3.2.5.1.5 끝봉이 소추세에 영향을 주지 않는 경우에는 소추세를 그대로 반영한다.
			//	(소추세의 고가나 시가를 갱신하지 않는 경우 모두를 포함한다. 추세 반전도 포함된다.)
			// 2.3.2.5.1.6 끝봉에 의한 가추세 값은 dMLStartPrice, dMLEndPrice, dNewDatePrice에 반영되었으므로
			//	아래의 소추세 등록 Routine에서 처리도록 한다.
		}
		// 2.3.2.5.2 끝봉에 의한 가추세가 추가되지 않은 경우, 소추세를 그대로 가추세에 반영한다.
		m_pOMLStartPricePacket->AppendData(		dMLStartPrice);
		m_pOMLEndPricePacket->AppendData(		dMLEndPrice	);
		m_pOMLNewDatePricePacket->AppendData(	dNewDatePrice);

		// 2.3.2.6 TR 처리를 종료한다.
		return;
	}
	// 2.3.3 정추세로 확인되는 경우, 추세로 등록시킨다.
	m_pOMLStartPricePacket->AppendData(		dMLStartPrice	);
	m_pOMLEndPricePacket->AppendData(		dMLEndPrice		);
	m_pOMLNewDatePricePacket->AppendData(	dNewDatePrice	);

	// 2.4 더 추가할 정추세가 있는지 확인하여 등록시킨다.
	//	(이 시점에서의 정추세는 이미 하나 등록된 상황이다.)
	while( posMLDate && posMLStartPrice && posMLEndPrice && posNewDatePrice)
	{
		dMLDate			= pMLDateList->GetNext(			posMLDate		);
		dMLStartPrice	= pMLStartPriceList->GetNext(	posMLStartPrice	);
		dMLEndPrice		= pMLEndPriceList->GetNext(		posMLEndPrice	);
		dNewDatePrice	= pMLNewDatePriceList->GetNext(	posNewDatePrice	);

		m_pOMLStartPricePacket->AppendData(		dMLStartPrice	);
		m_pOMLEndPricePacket->AppendData(		dMLEndPrice		);
		m_pOMLNewDatePricePacket->AppendData(	dNewDatePrice	);
	}

	// (2004.03.01, 배승원) 마지막 정추세의 시각을 관리한다. (가추세의 새일자가 산출시 참조한다.)
	//		특히, 이때의 소추세 추적시각을 0으로 설정하여 정추세 추적중임을 설정한다.
	m_nMiniTrendTime	= 0;
	m_nFixedTrendTime	= int( dMLDate);

	// 2.5 마지막으로 가추세를 추가한다.
	// 2.5.1 끝봉의 종가가 있는 경우, 확장/반전 가추세를 확인하여 추가시킨다.
	if( m_nLastStickTime)
	{
		//	(정추세 이후의 가추세는 마지막으로 등록된 추세의 종가를 기준으로,
		//	 새로운 추세를 만드는지 확인하며 추가한다.)
		// 2.5.1.0 이전 정추세의 가격 범위와 새로운 가추세의 새일자가를 미리 구하여 놓는다.
		int nMLStartPriceX100	= int( dMLStartPrice	* 100);
		int nMLEndPriceX100		= int( dMLEndPrice		* 100);
		// 2.5.1.1 먼저 확장 또는 반전 가추세의 경우를 처리한다.
		//	- 확장 가추세 : 이전 상승 추세를 더 상승시키거나, 하락 추세를 더 하락시키는 경우이다.
		//					확장의 경우 같은 추세에 있는가만 확인한다. 아래의 IF문으로 이미 확장하고 있음을 확인할 수 있다.
		//	- 반전 가추세 : 이전 상승 추세에 반해 하락하거나, 하락 추세에 반해 상승하는 경우이다.
		//					반전의 경우에는 추세 역전 방향만이 아니라, 가추세 폭의 크기가 반전 단위 호가에 해당하는지도 확인한다.
			// (확장 가추세이거나)
		if( 0 < ( nMLEndPriceX100 - nMLStartPriceX100) * ( nLastStickPriceX100 - nMLEndPriceX100)
			// (반전 가추세인 경우)
			|| ( ( nMLEndPriceX100 - nMLStartPriceX100) * ( nLastStickPriceX100 - nMLEndPriceX100) < 0
				&& m_nNewTrendPriceX100 <= abs( nLastStickPriceX100 - nMLEndPriceX100)))
		{
			// (2004.02.26, 배승원) 새일자가는 확장/반전되는 가추세의 시가이다.
			// 2.5.1.1.1 먼저 확장/반전되는 가추세의 시가를 구한다.
			int nVirtualStartPriceX100 = nMLEndPriceX100 + ( nMLEndPriceX100 < nLastStickPriceX100 ? 5 : -5);
			// 2.5.1.1.2 새일자인 경우를 확인하여 새일자가를 Update한다. (주의. 기존것을 감안하여 Clear시키지 않는다.)
			if( m_nFixedTrendTime / 10000 != m_nLastStickTime / 10000) dNewDatePrice =  nVirtualStartPriceX100 / 100.;
			// (2004.02.27, 배승원) 정추세 이후의 가추세에 의한 새일자가가 발생하지 않는 경우에는 0으로 Clear시킨다.
			else dNewDatePrice = 0;
			// 2.5.1.1.3 가추세를 등록한다.
			m_pOMLStartPricePacket->AppendData(		nVirtualStartPriceX100 / 100.);
			m_pOMLEndPricePacket->AppendData(		nLastStickPriceX100 / 100.);
			m_pOMLNewDatePricePacket->AppendData(	dNewDatePrice);
			return;
		}
		// 2.5.1.2 확장/반전의 가추세가 형성되지 않은 경우, 이전 정추세의 종가로 설정한다.
		// (2004.02.25, 배승원) 가추세가 확장/반전의 추세를 형성하지 않는 경우,
		//		새일자가가는 존재하지 않는다.
	}
	// 2.5.2 끝봉에 의한 가추세가 추가되지 않은 경우, 형식적인 가추세를 추가한다.
	//	(가추세는 Drawing시에 일정하게 감안토록 추가하여야 한다. 마지막 종가로 한번 더 그리도록 한다.)
	m_pOMLStartPricePacket->AppendData(		dMLEndPrice	);
	m_pOMLEndPricePacket->AppendData(		dMLEndPrice	);
	m_pOMLNewDatePricePacket->AppendData(	0			);
}

//PF 리얼계산
// (2004.02.26, 배승원, 분석) Real의 처리는 TR시의 끝봉 Data의 처리와 같다.
//		단 끝봉의 처리시, 이전 끝봉과 다른 새로운 봉으로 형성될 경우,
//		이전 끝봉을 소/정추세에 추가한다.
void CFuOpPF::GetRealFuOpPF( const CPoint &point)
{
	// 0. 먼저 Real을 수신하는 상황인지 확인한다.
	//	(장시작의 시각이 없다는 것은 장중이 아니면서, 또한 금일이 영업일이 아닌 경우이다.)
	if( !m_nMarketStartTime) return;
	CList< double, double> *pOMLStartPriceList		= m_pOMLStartPricePacket->GetnumericDataList();
	CList< double, double> *pOMLEndPriceList		= m_pOMLEndPricePacket->GetnumericDataList();
	CList< double, double> *pOMLNewDatePriceList	= m_pOMLNewDatePricePacket->GetnumericDataList();
	if( !pOMLStartPriceList || !pOMLEndPriceList || !pOMLNewDatePriceList) return;

	// 1. 먼저 새로운 Real의 시각과 종가를 구한다.
	//		(시각과 종가 Packet의 이전 Data들은 이미 TR 처리후 Clear되었다.
	//		 따라서 등록되어 있는 첫 Data만을 처리한다. => Real이 몰아 들어오면?)
	// 1.1 Real이 수신되었는지 확인한다.
	if( !m_pIMLDatePacket->GetCount() || !m_pIMLEndPricePacket->GetCount()) return;
	// 1.2 Real 시각을 구한다. (TR로 추적하는 시각과 동일하게 하기 위하여 초단위를 절사한다.)
	// (2004.03.05, 배승원) 끝봉 시각을 추적하면서 Real을 취합하는 것은 당일 끝봉인 경우뿐이다.
	//		따라서, 장시작 시각을 별도로 관리하여, 첫 Real 수신시 새일자가를 산출하도록 한다.
	//		Tick Time에 장시작 시각을 이용한 일자를 추가한다.
	// (2006/5/25 - Seung-Won, Bae) PacketData.dll process Date Setting already.
	//	int	nTickTime		= ( m_nMarketStartTime / 10000) * 10000 + int( m_pIMLDatePacket->GetData( 0)) / 100;
	int	nTickTime		= int( m_pIMLDatePacket->GetData( 0)) / 100;
	// 1.3 Real의 가격을 구한다.
	int nTickPriceX100	= int( m_pIMLEndPricePacket->GetData( 0) * 100);
	// 1.4 수신된 Real의 가격을 확인한다.
	if( !nTickTime || !nTickPriceX100) return;

	// 2. 끝봉의 시각보다 Real의 시각이 더 이른 경우(장 종료후 Test Real)에도
	//		정상적으로 처리될 수 있도록 끝봉시각을 Real시각 이전으로 조정한다.
	// (2004.03.05, 배승원) 끝봉 시각을 추적하면서 Real을 취합하는 것은 당일 끝봉인 경우뿐이다.
	//		따라서, 장시작 시각을 별도로 관리하여, 첫 Real 수신시 새일자가를 산출하도록 한다.
	//		끝봉의 시각 이후, 이전의 Real의 수신되는 장종료후 Test Real의 처리시,
	//		끝봉을 전일의 것으로 조정한다.
	// (2004.03.09, 배승원) Tick의 시각에 금일 일자가 추가되므로 이를 이용하여 비교한다.
	if( nTickTime < m_nLastStickTime) m_nLastStickTime = ( ( m_nMarketStartTime / 10000 - 1) * 10000) + m_nLastStickTime % 10000;

	// 2.5 먼저 가추세의 Index를 구한다.
	//		가추세가 없다면 TR을 제대로 수신하지 못한 경우이다.
	POSITION posVirtualTrendStart	= pOMLStartPriceList->GetTailPosition();
	POSITION posVirtualTrendEnd		= pOMLEndPriceList->GetTailPosition();
	POSITION posVirtualTrendNewDate = pOMLNewDatePriceList->GetTailPosition();
	if( !posVirtualTrendStart || !posVirtualTrendEnd || !posVirtualTrendNewDate) return;

	// (2004.03.05, 배승원) Real 처리중에서도 Scale이 확장되도록,
	//		Scale Packet을 Update한다.
	int nScalePriceX100	= int( pOMLStartPriceList->GetHead() * 100);
	if( nTickPriceX100	< nScalePriceX100)	pOMLStartPriceList->SetAt(	pOMLStartPriceList->GetHeadPosition(),	nTickPriceX100 / 100.);
	nScalePriceX100		= int( pOMLEndPriceList->GetHead() * 100);
	if( nScalePriceX100	< nTickPriceX100)	pOMLEndPriceList->SetAt(	pOMLEndPriceList->GetHeadPosition(),	nTickPriceX100 / 100.);

	// 3. 이전 끝봉과 이번 Real의 값을 확인하여, 이전 끝봉을 소/정추세에 적용할지 결정한다.
	//		특히 Tick P&F의 경우에는 무조건 이전 끝봉(Tick)을 소/정추세에 적용시킨다.
	//		또한, 끝봉시각은 끝봉의 시작시간으로 그 차이가 분봉 폭보다 큰 경우로 판단한다.
	// (2004.03.05, 배승원) 시각의 대소 비교시에는 전체를 분으로 환산하는 방법으로 하여야 한다.
	// (2004.03.05, 배승원) 끝봉 시각을 추적하면서 Real을 취합하는 것은 당일 끝봉인 경우뿐이다.
	//		따라서, 장시작 시각을 별도로 관리하여, 첫 Real 수신시 새일자가를 산출하도록 한다.
	//		따라서 끝봉시각이 0인 경우(끝봉없음)을 감안한다.
	// (2004.03.05, 배승원) 끝봉이 전일의 것인 경우도 처리되도록 설정한다.
	if( m_nLastStickTime
		&& ( !m_nStickTimeWidth
			|| m_nLastStickTime / 10000 < nTickTime / 10000
			|| ( ( m_nLastStickTime % 10000) / 100) * 60 + m_nLastStickTime % 100 + m_nStickTimeWidth
				<= ( ( nTickTime % 10000) / 100) * 60 + nTickTime % 100))
	{
		// 3.1 아직 가추세가 형성되지 않은 상태에서, 첫 Data을 받는 경우를 처리한다.
		//		(이전에 끝봉에 의한 가추세가 형성되지 않은 상태이다.
		//		 이경우, 끝봉 시각이 장시작시간이라 Update할 소추세 추적 Data가 없다.)
		if( m_nMiniTrendTime == -2);
		// 3.2 소추세의 추적을 처리한다.
		else if( m_nMiniTrendTime != 0)
		{
			// 3.2.1 아직 소추세가 형성되지 않고, 새로운 소추세 추적을 시작하는 경우를 처리한다.
			// (2004.03.03, 배승원) 새로운 소추세의 등록시의 가추세의 수용에서도,
			//		가추세가 이미 정추세의 형태로 조정하여 등록된 것을 감안하여 수용한다.
			//		(가추세의 종가만을 수용한다. 시가는 이미 출력용으로 조정되어 있다.)
			if( m_nMiniTrendTime == -1)
			{
				// 3.2.1.1 새로운 소추세 추적을 의한 변수를 설정한다.
				m_nMiniTrendTime				= m_nLastStickTime;
				// 3.2.1.2 새로이 형성되는 소추세의 시가는, 조회전일 종가이다.
				//	(조회전일 종가를 기준으로 하며, 가추세의 종가를 포함하도록 조정한다.)
				m_nMiniTrendStartX100			= m_nFirstBasePriceX100;
				// 3.2.1.3 새로이 형성되는 소추세의 종가는, 가추세의 종가이다.
				m_nMiniTrendEndX100				= int( pOMLEndPriceList->GetAt( posVirtualTrendEnd) * 100);
				// 3.2.1.4 새로이 형성되는 소추세의 새일자가는 없다.
				m_nMiniTrendNewDatePriceX100	= 0;
			}
			// 3.2.2 소추세 추적인 경우를 처리한다.
			//		소추세의 추적인 경우, 마지막 끝봉이 인정되므로,
			//		 마지막 끝봉을 감안하여 출력한 가추세가 새로운 소추세로 인정된다.
			// (2004.03.03, 배승원) 소추세의 추적시 가추세에서 수용할 것은 종가 뿐이다.
			//		가추세가 이미 이전 소추세와의 Tick을 취합된 형태로 구성된 것이므로,
			//		종가만 참조하여 새로이 취합한다. 그래야 이후의 정추세 판단이 가능하다.
			//		(별도로 판단없이 그냥 정추세형 가추세를 바로 정추세로 인정하는 Logic을 구성하기 위해서는,
			//		 별도의 Flag 개념이 필요하므로, 그렇게 처리하지않고, 여기서 소추세를 다시 취합한다.)
			// (2004.03.04, 배승원) 추세의 시각은 추세를 확장시킨 경우에만 Update한다.
			else if( 0 < m_nMiniTrendTime)
			{
				// 3.2.2.2 새로이 형성되는 소추세의 새일자가는, 가추세의 새일자가이다.
				m_nMiniTrendNewDatePriceX100	= int( pOMLNewDatePriceList->GetAt(	posVirtualTrendNewDate) * 100);
				// 3.2.2.3 새로이 형성되는 소추세의 시가나 종가를 갱신한다.
				//		(둘다 갱신하지 않는다. 만약 정추세형 가추세인 경우, 가추세에는 반전 기준이 이미 제외되어 있다.)
				int nVirtualTrendEnd			= int( pOMLEndPriceList->GetAt(		posVirtualTrendEnd)		* 100);
				// 3.2.2.3.1 종가를 갱신하는 경우를 처리한다. (소추세를 연장하는 경우)
				if( 0 < ( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) * ( nVirtualTrendEnd - m_nMiniTrendEndX100)
					&& abs( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) < abs( nVirtualTrendEnd - m_nMiniTrendStartX100))
				{
					// 3.2.2.3.1.1 새로운 소추세 추적을 의한 변수를 설정한다.
					m_nMiniTrendTime	= m_nLastStickTime;
					// 3.2.2.3.1.2 추세의 방향으로 확장한다.
					m_nMiniTrendEndX100	= nVirtualTrendEnd;
				}
				// 3.2.2.3.2 시가를 갱신하는 경우를 처리한다. (소추세를 반대 확장하는 경우)
				else if( ( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) * ( nVirtualTrendEnd - m_nMiniTrendEndX100) < 0
					&& abs( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) < abs( nVirtualTrendEnd - m_nMiniTrendEndX100))
				{
					// 3.2.2.3.2.1 새로운 소추세 추적을 의한 변수를 설정한다.
					m_nMiniTrendTime		= m_nLastStickTime;
					// 3.2.2.3.2.2 추세의 방향을 전환한다.
					m_nMiniTrendStartX100	= m_nMiniTrendEndX100;
					m_nMiniTrendEndX100		= nVirtualTrendEnd;
				}
			}
			// 3.2.3 소추세 추적 상태가 오류인 경우를 처리한다.
			else return;

			// 3.2.4 수정된 소추세 추적이 정추세로 인정되는 경우를 처리한다.
			if( m_nNewTrendPriceX100 <= abs( m_nMiniTrendEndX100 - m_nMiniTrendStartX100))
			{
				// (2004.03.01, 배승원) 마지막 정추세의 시각을 관리한다. (가추세의 새일자가 산출시 참조한다.)
				m_nFixedTrendTime = m_nMiniTrendTime;
				// 3.2.4 먼저 추적중인 소추세를 정추세로 추가한다. (가추세보다 맨앞에 추가한다.)
				// 3.2.4.1 소추세 추적 시각을 처리한다. (정추세가 있는 것으로 설정한다.)
				m_nMiniTrendTime = 0;
				// 3.2.4.2 소추세 시가를 처리한다.
				// (2004.03.01, 배승원) 소추세가 정추세로 인정되는 시점에서는 시가가 이전 추세의 종가로 평가되므로,
				//		새로운 추세의 방향에 따라 +/- 0.05를 처리하여야 한다.
				// (2004.03.09, 배승원) 첫 정추세라고해도 첫 Packet은 Scale Packet이다.
				//		따라서 가추세 이전으로 추가시켜야 한다.
				pOMLStartPriceList->InsertBefore(	posVirtualTrendStart,	( m_nMiniTrendStartX100 + ( m_nMiniTrendStartX100 < m_nMiniTrendEndX100 ? 5 : -5))  / 100.);
				// 3.2.4.3 소추세 종가를 처리한다.
				pOMLEndPriceList->InsertBefore(		posVirtualTrendEnd,		m_nMiniTrendEndX100 / 100.);
				// 3.2.4.4 소추세 새일자가를 처리한다.
				pOMLNewDatePriceList->InsertBefore(	posVirtualTrendNewDate,	m_nMiniTrendNewDatePriceX100	/ 100.);
			}
		}
		// 3.3 정추세 추적인 경우를 처리한다.
		//		정추세의 추적인 경우, 가추세가 확장/반전하는지 확인하여 처리한다.
		else if( !m_nMiniTrendTime)
		{
			// 3.3.1 먼저 가추세의 시/종/새일자가를 확보한다.
			//		(이때 정추세의 Position을 구할 수 있는 방법으로 처리한다.)
			POSITION posLastFixedStart		= posVirtualTrendStart;
			POSITION posLastFixedEnd		= posVirtualTrendEnd;		
			POSITION posLastFixedNewDate	= posVirtualTrendNewDate;
			int nVirtualStartPriceX100		= int( pOMLStartPriceList->GetPrev(		posLastFixedStart	) * 100);
			int nVirtualEndPriceX100		= int( pOMLEndPriceList->GetPrev(		posLastFixedEnd		) * 100);
			int nVirtualNewDatePriceX100	= int( pOMLNewDatePriceList->GetPrev(	posLastFixedNewDate	) * 100);
			// 3.3.2 최종 정추세를 확보한다.
			int nLastFixedStartPriceX100		= int( pOMLStartPriceList->GetAt(	posLastFixedStart)		* 100);
			int nLastFixedEndPriceX100			= int( pOMLEndPriceList->GetAt(		posLastFixedEnd)		* 100);
			// 3.3.3 최종 정추세를 확장하는 경우를 처리한다.
			// (2004.03.01, 배승원) 반전폭의 범위는 이전 정추세의 종가에서 이번 가추세의 종가까지의 차로 산출한다.
			// (2004.03.04, 배승원) 추세의 확장은 정추세의 시가 -> 종가 -> 가추세 종가로 이어지는 상황을 말한다.
			if( 0 < ( nLastFixedEndPriceX100 - nLastFixedStartPriceX100) * ( nVirtualEndPriceX100 - nLastFixedEndPriceX100))
			{
				// 3.3.3.1 정추세의 종가를 갱신한다.
				pOMLEndPriceList->SetAt( posLastFixedEnd, nVirtualEndPriceX100 / 100.);
				// 3.3.3.2 가추세의 새일자가가 0이 아니면 정추세에 Update한다.
				//		이경우 기존 정추세에 새일자가가 있었다면, 그것은 단일 추세안에 일자가 두번째 바뀌는 것으로,
				//		최근의 것만을 인정한다. ~ 현 Server Packet구조로 1개 추세안의 두 새일자가를 수용못함 (문준철 차장 동의, 2004.02.26)
				if( nVirtualNewDatePriceX100) pOMLNewDatePriceList->SetAt( posLastFixedNewDate, nVirtualNewDatePriceX100 / 100.);
			}
			// 3.3.4 최종 정추세를 반전하는 경우를 갱신한다.
			//		최종 정추세를 반전하는 가추세의 등록은 그냥 가추세를 정추세로 인정하는 것으로 처리된다.
			//		즉 새로운 형식 가추세를 등록하여 가추세로 정의한다.
			// (2004.03.01, 배승원) 반전폭의 범위는 이전 정추세의 종가에서 이번 가추세의 종가까지의 차로 산출한다.
			else if( ( nLastFixedEndPriceX100 - nLastFixedStartPriceX100) * ( nVirtualEndPriceX100 - nLastFixedEndPriceX100) < 0
				&& m_nNewTrendPriceX100 <= abs( nLastFixedEndPriceX100 - nVirtualEndPriceX100))
			{
				posVirtualTrendStart	= pOMLStartPriceList->AddTail(		nVirtualEndPriceX100 / 100.);
				posVirtualTrendEnd		= pOMLEndPriceList->AddTail(		nVirtualEndPriceX100 / 100.);
				posVirtualTrendNewDate	= pOMLNewDatePriceList->AddTail(	0.);
			}
			// 3.3.4 최종 정추세에 포함되는 경우를 갱신한다.
			//	(특별한 작업을 진행하지 않는다. 가추세를 그대로 둔다.)
			else;
			// 3.3.5 (2004.03.01, 배승원) 마지막 정추세의 시각을 관리한다. (가추세의 새일자가 산출시 참조한다.)
			m_nFixedTrendTime = m_nLastStickTime;
		}
	
		// 3.4 끝봉을 새로이 마감하였으므로, 새로운 끝봉시각을 설정하여 추적토록한다.
		//		아래 공식의 정확한 이해가 필요하다.
		//	(주의, 끝봉시각이 항상 봉 취합시간으로 나눠 떨어진다는 보장이 없다. ex:14분봉)
		//		아래의 공식이 무엇을 의미하는지 정확히 이해할 것~!
		//		(장시작시간 nStartT, nBarW 분봉 시간폭, 마지막 분봉 시작시각은 nLastBarT, 이라고 할때,
		//		 nStartT = nVirtualBarC * nBarW + nStartTMod	(nVirtualBarC는 0시부터 장시작 시각까지의 봉수이다.)
		//		 nLastBarT = ( nVirtualBarC + nRealBarC) * nBarW + nStartTMod 라고 표현할 수 있다. (nRealBarC는 장시작후 완성된 봉수이다. 하나는 미완성)
		//		따라서, 현 Tick 시간인 nTickT를 포함하는 마지막 분봉을 생각한다면,
		//		 nTickT = ( nVirtualBarC + nRealBarC) * nBarW + nStartTMod + nBarTMod 이다. (nBarTMod는 nTickT를 포함하는 분봉시작 이후의 시각이다.)
		//		그러므로, nTickT = ( nVirtualBarC + nRealBarC) * nBarW + nStartTMod + nBarTMod 
		//				nTickT = ( nVirtualBarC * nBarW + nStartTMod) + ( nRealBarC * nBarW + nBarTMod)
		//				nBarTMod = ( nTickT - ( nVirtualBarC * nBarW + nStartTMod)) % nBarW
		//						 = ( nTickT + nA * nBarW - ( nVirtualBarC * nBarW + nStartTMod)) % nBarW
		// (2004.03.05, 배승원) 끝봉 시각을 추적하면서 Real을 취합하는 것은 당일 끝봉인 경우뿐이다.
		//		따라서, 장시작 시각을 별도로 관리하여, 첫 Real 수신시 새일자가를 산출하도록 한다.
		//		이경우 금일의 끝봉이 아니면 장시작봉을 형성시킨다.
		if( m_nLastStickTime < m_nMarketStartTime) m_nLastStickTime = m_nMarketStartTime;
		else if( m_nStickTimeWidth)
		{
			// 3.4.1 먼저 Tick시각을 정규화시킨다. (전체 분수로 조정한다.)
			int nNTickTime = nTickTime % 10000;
			nNTickTime = ( nNTickTime / 100) * 60 + nNTickTime % 100;
			// 3.4.2 마지막 봉시각을 정규화시킨다. (전체 분수로 조정한다.)
			int nNLastStickTime = m_nLastStickTime % 10000;
			nNLastStickTime = ( nNLastStickTime / 100) * 60 + nNLastStickTime % 100;
			// 3.4.3 Tick을 포함하는 봉의 시작시각을 구한다.
			int nBarTMod = ( nNTickTime - nNLastStickTime) % m_nStickTimeWidth;
			nNTickTime -= nBarTMod;
			// 3.4.4 새로운 Tick을 포함하는 봉의 시작시각을 산출한다.
			m_nLastStickTime = ( ( nTickTime / 10000) * 10000)
				+ ( ( nNTickTime / 60) * 100) + ( nNTickTime % 60); // 일자는 유지한 상태에서 시분만 조정한 60진법 표기로 수정한다.
		}
		else m_nLastStickTime = nTickTime;
	}
	// (2004.03.05, 배승원) 끝봉 시각을 추적하면서 Real을 취합하는 것은 당일 끝봉인 경우뿐이다.
	//		따라서, 장시작 시각을 별도로 관리하여, 첫 Real 수신시 새일자가를 산출하도록 한다.
	//		따라서 끝봉시각이 0인(끝봉없음) 경우, 다음 끝봉의 시각을 장시작시각으로 설정한다.
	if( !m_nLastStickTime) m_nLastStickTime = m_nMarketStartTime;

	// 4. 수신된 Real Data를 이용한 가추세를 형성시킨다.
	//	(주의, 소추세 추적변수나 정추세를 조작하지 않는다. Tick은 바로 다음 Tick에 의해 RollBack될 수 있기 때문이다.)
	// 4.1 먼저 소추세가 형성되지 않은 상태에서, 첫 Data을 받는 경우를 처리한다.
	//		(전일 종가를 기준으로 첫 가추세를 구성한다.
	//		 이경우 이미 가추세가 형성된 경우(m_nMiniTrendTime == -1)도 함께 처리한다.
	//		 어차피 전일조회종가를 기준으로 처리하기 때문이다.)
	if( m_nMiniTrendTime == -2 || m_nMiniTrendTime == -1)
	{
		// 4.1.1 먼저 가추세 종가를 설정한다.
		pOMLEndPriceList->SetAt(		posVirtualTrendEnd,		nTickPriceX100	/ 100.	);
		// 4.1.2 조회 전일 종가를 기준으로 가추세 시가를 설정한다.
		// (2004.03.03, 배승원) 초기 소추세 없는 가추세의 출력시 시가는 전일 종가 +/-0.05이다.
		if( m_nFirstBasePriceX100 < nTickPriceX100)			nTickPriceX100 = m_nFirstBasePriceX100 + 5;
		else if( nTickPriceX100 < m_nFirstBasePriceX100)	nTickPriceX100 = m_nFirstBasePriceX100 - 5;
		pOMLStartPriceList->SetAt(		posVirtualTrendStart,	nTickPriceX100	/ 100.	);
		// 4.1.3 소추세가 없는 상황에서의 첫 가추세에는 새일자가를 적용하지 않는다.
		pOMLNewDatePriceList->SetAt(	posVirtualTrendNewDate,	0						);
		// (2004.03.03, 배승원) 끝봉에 의해 가추세가 형성되었으므로, m_nMiniTrendTime을 -1로 전환한다.
		m_nMiniTrendTime = -1;
	}
	// 4.2 소추세의 추적 진행을 중인 경우를 처리한다.
	//		(소추세를 감안하여 가추세를 형성한다.)
	else if( 0 < m_nMiniTrendTime)
	{
		// 4.2.0 소추세에 Tick을 반영하여 가추세를 형성하기 위해, 기본적으로 소추세의 값을 구하여 놓는다.
		double dStartPrice		= m_nMiniTrendStartX100			/ 100.;
		double dEndPrice		= m_nMiniTrendEndX100			/ 100.;
		double dNewDatePrice	= m_nMiniTrendNewDatePriceX100	/ 100.;

		// 4.2.1 Tick이 소추세를 확장시키는 경우를 처리한다. (확장 가추세이면 종가만을 수정한다.)
		//	- 확장 가추세 : 소추세를 더 상승시키거나, 하락 추세를 더 하락시키는 경우이다.
		//					확장의 경우, 같은 추세에 있는가만 확인한다. 아래의 IF문으로 이미 확장하고 있음을 확인할 수 있다.
		//					얼마나 확장시키는 것은 중요하지 않다. (1호가(0.05)만 확장해도 인정된다.)
		//	(주의, 소추세 추적 변수를 수정하지 않는다.
		//	 왜냐하면 다음 Real에서도 소추세 추적변수를 기준으로 이용하여야 하니까.)
		if( 0 < ( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) * ( nTickPriceX100 - m_nMiniTrendEndX100))
		{
			// 4.2.1.1 소추세를 반영하여 등록하는 가추세에, Tick에 의한 새일자가를 Update한다.
			//	(이미 소추세에는 소추세내의 새일자가가가 등록되어 있으므로 새일자인 경우만 새일자가를 반영하고,
			//		그렇지 않으면 원래값을 유지한다.
			//	 특히 그값은 소추세 확장 방향에 따라, 고/저가의 +/- 0.05이다.)
			if( m_nMiniTrendTime / 10000 != m_nLastStickTime / 10000)
			{
				// 4.2.1.1.1 하락의 경우, 종가보다 0.05적게 설정한다.
				if( m_nMiniTrendEndX100 < m_nMiniTrendStartX100)		dNewDatePrice = ( m_nMiniTrendEndX100 - 5) / 100.;
				// 4.2.1.1.2 상승의 경우, 종가보다 0.05많게 설정한다.
				else if( m_nMiniTrendStartX100 < m_nMiniTrendEndX100)	dNewDatePrice = ( m_nMiniTrendEndX100 + 5) / 100.;
			}
			// 4.2.1.2 가추세에 종가로 Tick 가격을 적용시킨다.
			dEndPrice = nTickPriceX100 / 100.;
		}
		// 4.2.2 Tick이 소추세를 역으로 확장시키는 경우를 처리한다.
		//	(추세 반전을 확인하지 않고, 소추세를 역으로 확장시키는 경우만 확인한다.
		//	 소추세의 경우, 정추세가 아닌 관계로 반전 추세는 새로운 가추세로 흡수하게된다.)
		//  - 역확장 가추세 : 소추세를 역행하면서 시가보다 더 진행하는 경우이다.
		//					  이경우 소추세의 기본 종가를 시가로 이용하고, Tick의 가격을 종가로 이용하는
		//						새로운 소추세의 형태로 가추세를 등록하여야 한다.
		//					  (물론 추적중이므로 소추세 추적 변수는 갱신하지 않는다.)
		//	(추세 확인은 소추세의 방향을 역으로 놓고 확인한다.)
		if( 0 < ( m_nMiniTrendStartX100 - m_nMiniTrendEndX100) * ( nTickPriceX100 - m_nMiniTrendStartX100))
		{
			// 4.2.2.1 소추세를 반영하며 등록하는 가추세에, Tick에 의한 새일자가를 Update한다.
			//	(이미 소추세에는 소추세내의 새일자가가가 등록되어 있으므로 새일자인 경우만 새일자가를 반영하고,
			//		그렇지 않으면 원래값을 유지한다.
			//	 특히 그값은 소추세 확장 방향에 따라, 고/저가의 +/- 0.05이다.)
			if( m_nMiniTrendTime / 10000 != m_nLastStickTime / 10000)
			{
				// 4.2.2.1.1 하락의 경우, 시가보다 0.05많게 설정한다. (역추세 확장에 주의)
				if( m_nMiniTrendEndX100 < m_nMiniTrendStartX100)		dNewDatePrice = ( m_nMiniTrendStartX100 + 5) / 100.;
				// 4.2.2.1.2 상승의 경우, 시가보다 0.05적게 설정한다. (역추세 확장에 주의)
				else if( m_nMiniTrendStartX100 < m_nMiniTrendEndX100)	dNewDatePrice = ( m_nMiniTrendStartX100 - 5) / 100.;
			}

			// 4.2.2.2 소추세의 시가를 소추세 기본 종가로 설정한다.
			dStartPrice = dEndPrice;
			// 4.2.2.3 소추세의 종가를 Tick의 가격으로 반영한다.
			dEndPrice = nTickPriceX100 / 100.;
		}

		// 4.2.3 조정된 소추세의 폭이 반전 호가단위보다 큰 경우,
		//		소추세와 끝봉 Data를 취합하여 구성한 가추세를 정추세 형식으로 조정하여 보여준다.
		//		(소추세가 정추세로 인정될때, 시가를 +/- 0.05 처리하여야 한다.
		//		 시가기준의 반전으로 인정되는 정추세이기 때문이다.)
		if( m_nNewTrendPriceX100 <= abs( int( dEndPrice * 100) - int( dStartPrice * 100)))
		{
			if( dStartPrice < dEndPrice)	dStartPrice = ( int( dStartPrice * 100) + 5) / 100.;
			else							dStartPrice = ( int( dStartPrice * 100) - 5) / 100.;
		}

		// 4.2.4 Tick이 소추세에 영향을 주지 않는 경우에는 소추세를 그대로 반영한다.
		//	(소추세의 고가나 시가를 갱신하지 않는 경우 모두를 포함한다. 추세 반전도 포함된다.)
		pOMLStartPriceList->SetAt(			posVirtualTrendStart,	dStartPrice		);
		pOMLEndPriceList->SetAt(			posVirtualTrendEnd,		dEndPrice		);
		pOMLNewDatePriceList->SetAt(		posVirtualTrendNewDate,	dNewDatePrice	);
	}
	// 3.3 정추세 추적인 경우를 처리한다.
	//		정추세의 추적인 경우, Tick이 추세를 확장/반전하는지 확인하여 가추세를 설정한다.
	//		(정추세 이후의 가추세는 마지막으로 등록된 추세의 종가를 기준으로,
	//		 새로운 추세를 만드는지 확인하며 추가한다.)
	else
	{
		// 3.3.1 최종 정추세의 가격을 확보한다.
		// 3.3.1.1 마지막 가추세의 Position을 구한다.
		POSITION posLastFixedStart		= posVirtualTrendStart;
		POSITION posLastFixedEnd		= posVirtualTrendEnd;		
		POSITION posLastFixedNewDate	= posVirtualTrendNewDate;
		pOMLStartPriceList->GetPrev(	posLastFixedStart	);
		pOMLEndPriceList->GetPrev(		posLastFixedEnd		);
		pOMLNewDatePriceList->GetPrev(	posLastFixedNewDate	);
		// (2004.04.19, 배승원) UI상에서 색상 조건을 변경하여 재계산시, Output Packet이 Clear되는 경우를 감안하여,
		//		NULL Position을 제외한다.
		if( !posLastFixedStart || !posLastFixedEnd || !posLastFixedNewDate) return;
		// 3.3.1.2 최종 정추세를 확보한다.
		int nLastFixedStartPriceX100		= int( pOMLStartPriceList->GetAt(	posLastFixedStart)		* 100);
		int nLastFixedEndPriceX100			= int( pOMLEndPriceList->GetAt(		posLastFixedEnd)		* 100);
		// 3.3.1.3 먼저 확장 또는 반전 가추세의 경우를 처리한다.
		//	- 확장 가추세 : 이전 상승 추세를 더 상승시키거나, 하락 추세를 더 하락시키는 경우이다.
		//					확장의 경우 같은 추세에 있는가만 확인한다. 아래의 IF문으로 이미 확장하고 있음을 확인할 수 있다.
		//	- 반전 가추세 : 이전 상승 추세에 반해 하락하거나, 하락 추세에 반해 상승하는 경우이다.
		//					반전의 경우에는 추세 역전 방향만이 아니라, 가추세 폭의 크기가 반전 단위 호가에 해당하는지도 확인한다.
			// (확장 가추세이거나)
		if( 0 < ( nLastFixedEndPriceX100 - nLastFixedStartPriceX100) * ( nTickPriceX100 - nLastFixedEndPriceX100)
			// (반전 가추세인 경우)
			|| ( ( nLastFixedEndPriceX100 - nLastFixedStartPriceX100) * ( nTickPriceX100 - nLastFixedEndPriceX100) < 0
				&& m_nNewTrendPriceX100 <= abs( nTickPriceX100 - nLastFixedEndPriceX100)))
		{
			// (2004.02.26, 배승원) 새일자가는 확장/반전되는 가추세의 시가이다.
			// 3.3.1.3.1 먼저 확장/반전되는 가추세의 시가를 구한다.
			int nVirtualStartPriceX100 = nLastFixedEndPriceX100 + ( nLastFixedEndPriceX100 < nTickPriceX100 ? 5 : -5);
			// 3.3.1.3.2 새일자인 경우를 확인하여 새일자가를 Update한다. (주의. 기존것을 감안하여 Clear시키지 않는다.)
			// (2004.02.27, 배승원) 정추세 이후의 가추세에 의한 새일자가가 발생하지 않는 경우에는 0으로 Clear시킨다.
			double dNewDatePrice = 0;
			if( m_nFixedTrendTime / 10000 != m_nLastStickTime / 10000) dNewDatePrice =  nVirtualStartPriceX100 / 100.;
			// 3.3.1.3.3 가추세를 설정한다.
			pOMLStartPriceList->SetAt(			posVirtualTrendStart,	nVirtualStartPriceX100	/ 100.	);
			pOMLEndPriceList->SetAt(			posVirtualTrendEnd,		nTickPriceX100			/ 100.	);
			pOMLNewDatePriceList->SetAt(		posVirtualTrendNewDate,	dNewDatePrice					);
			return;
		}
		// 3.3.1.4 확장/반전의 가추세가 형성되지 않은 경우, 이전 정추세의 종가로 설정한다.
		//		가추세가 확장/반전의 추세를 형성하지 않는 경우, 새일자가가는 존재하지 않는다.
		//		즉, Tick에 의한 가추세가 추가되지 않은 경우, 형식적인 가추세로 설정한다.
		//			가추세는 Drawing시에 일정하게 감안토록 추가하여야 한다. 마지막 종가로 한번 더 그리도록 한다.
		pOMLStartPriceList->SetAt(			posVirtualTrendStart,	nLastFixedEndPriceX100	/ 100.	);
		pOMLEndPriceList->SetAt(			posVirtualTrendEnd,		nLastFixedEndPriceX100	/ 100.	);
		pOMLNewDatePriceList->SetAt(		posVirtualTrendNewDate,	0								);
	}
}



//////////////////////////////////////////////////////////////////////////////////////////
// 지표 name : 일목균형표
// 주기 : 9, 26 (dcondition : 2개임)
// 전환선, 기준선, 후행스팬, 선행스팬1, 2, 구름층
// 
//////////////////////////////////////////////////////////////////////////////////////////
CGlanceBalance::CGlanceBalance() : 
	m_pLeadSpan1(NULL), 
	m_pLeadSpan2(NULL),
	m_pHighPriceList(NULL),
	m_pLowPriceList(NULL),
	m_pEndPriceList(NULL),
	m_pHighPacket(NULL),
	CIndicator( IMetaTable::EDDC_OCX_GLANCEBALANCECHART)
{
}

void CGlanceBalance::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pHighPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pHighPriceList = GetnumericDataList(m_pHighPacket);
		m_pLowPriceList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pEndPriceList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
	}
	if(!IsValidPacket(m_pHighPriceList) || !IsValidPacket(m_pLowPriceList) || !IsValidPacket(m_pEndPriceList))
		return;

	CPacket* pTuringPoint = NULL;
	CPacket* pStandardLine = NULL;
	CPacket* pLagSpan = NULL;
	if(!SetGraphPacketList(chartFormulateData.GetCalculateType(), pPacketList, pGraphCalculateData, pIndicatorInfo, 
						  pTuringPoint, pStandardLine, pLagSpan))
		return;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pIndicatorInfo) return;
	
	//sy 2005.11.03. -> 조건값 위치 변경
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dStandardCond = GetCondition(0, pConditionList);
	double dTuringCond = GetCondition(1, pConditionList);
	// 일목균형표 선행, 후행 별도 설정 - ojtaso (20071226)
	double dLeadSpanCond1 = GetCondition(2, pConditionList);
	double dLeadSpanCond2 = GetCondition(3, pConditionList);
	double dSpanCond = GetCondition(4, pConditionList);
	//sy end

	// 일목균형표 선행, 후행 별도 설정 - ojtaso (20071226)
	BOOL bEachSpan = !( dLeadSpanCond2 == 0 && dSpanCond == 0);
	if( !bEachSpan)
	{
		dSpanCond = dLeadSpanCond1;

		dLeadSpanCond1 = dStandardCond;
		dLeadSpanCond2 = dSpanCond;
		dSpanCond = dStandardCond;

		POSITION pos = pConditionList->GetHeadPosition();	// array Index[0]
		pConditionList->GetNext(pos);						// array Index[1]
		pConditionList->GetNext(pos);						// array Index[2]
		pConditionList->SetAt(pos, dLeadSpanCond1);			// dLeadSpanCond1 array Index[2]
		pConditionList->AddTail(dLeadSpanCond2);			// array Index[3]
		pConditionList->AddTail(dSpanCond);					// array Index[4]
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pTuringPoint || !pStandardLine || !pLagSpan || !m_pLeadSpan1 || !m_pLeadSpan2) return;

	CString strType;
	if(m_pHighPacket == NULL)
		strType = TYPE_INTERGER;
	else
		strType = m_pHighPacket->GetType(false);
	
	// (2009/9/19 - Seung-Won, Bae) for FX/WorldOn Index Digit.
	CString strValueType( TYPE_TWODECIMAL);
	if( CChartInfo::HTS != chartFormulateData.GetChartMode())
		strValueType = strType;

	SetPacketTypes( pTuringPoint,	strValueType, strType, (int)dTuringCond - 1);
	SetPacketTypes( pStandardLine,	strValueType, strType, (int)dStandardCond - 1);
	// 일목균형표 선행, 후행 별도 설정 - ojtaso (20071226)
	SetPacketTypes( pLagSpan,		strValueType, strType, int( dStandardCond - dSpanCond));
	SetPacketTypes( m_pLeadSpan1,	strValueType, strType, (int)(dStandardCond + dLeadSpanCond1) - 1);
	SetPacketTypes( m_pLeadSpan2,	strValueType, strType, (int)(dLeadSpanCond1 + dLeadSpanCond2) - 1);

	CPoint point = calculateData.GetPoint();
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pTuringPoint))
		GetTurningPointLine(point, dTuringCond, m_pHighPriceList, m_pLowPriceList, *pTuringPoint->GetnumericDataList());
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pStandardLine))
		GetStandardLine(point, dStandardCond, m_pHighPriceList, m_pLowPriceList, *pStandardLine->GetnumericDataList());
	// 일목균형표 선행, 후행 별도 설정 - ojtaso (20071226)
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pLagSpan))
		GetLagSpan(point, dSpanCond, m_pEndPriceList, *pLagSpan->GetnumericDataList());
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), m_pLeadSpan1))
	{
		GetLeadSpan1(point, m_pHighPriceList->GetCount(), dLeadSpanCond1,
						pTuringPoint->GetnumericDataList(), pStandardLine->GetnumericDataList(), *m_pLeadSpan1->GetnumericDataList());
		GetLeadSpan2(point, dLeadSpanCond1, dLeadSpanCond2, m_pHighPriceList, m_pLowPriceList, *m_pLeadSpan2->GetnumericDataList());
	}

	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pLagSpan))
		GetLagSpan(point, dStandardCond, m_pEndPriceList, *pLagSpan->GetnumericDataList());	
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), m_pLeadSpan1)){
		GetLeadSpan1(point, m_pHighPriceList->GetCount(), dStandardCond, 
				pTuringPoint->GetnumericDataList(), pStandardLine->GetnumericDataList(), 
				*m_pLeadSpan1->GetnumericDataList());
		GetLeadSpan2(point, dStandardCond, dSpanCond, m_pHighPriceList, m_pLowPriceList, *m_pLeadSpan2->GetnumericDataList());
	}
}

bool CGlanceBalance::SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
										CPacketList* pPacketList,
										CGraphCalculateData* pGraphCalculateData,
										CIndicatorInfo*& pIndicatorInfo,
										CPacket*& pTuringPoint, CPacket*& pStandardLine,
										CPacket*& pLagSpan)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	pTuringPoint = pStandardLine = pLagSpan = NULL;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL) 
			return InCaseNoPacketPointer(calculatetype, pPacketList, pIndicatorInfo, pGraphCalculateData, pTuringPoint, pStandardLine, pLagSpan);
		else
		{
/*			if(pTuringPoint == NULL)
				pTuringPoint = pPacket;
			else if(pTuringPoint != NULL && pStandardLine == NULL)	
				pStandardLine = pPacket;
			else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan == NULL)	
				pLagSpan = pPacket;
			else break;
*/
			// 일목균형표 선행, 후행 별도 설정 - ojtaso (20071226)
			if(pStandardLine == NULL)
				pStandardLine = pPacket;
			else if(pStandardLine != NULL && pTuringPoint == NULL)
				pTuringPoint = pPacket;
			else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan == NULL)
				pLagSpan = pPacket;
			else break;
		}
	}
	return (pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL && m_pLeadSpan1 != NULL && m_pLeadSpan2 != NULL);
}

bool CGlanceBalance::InCaseNoPacketPointer(const CCalculateBaseData::CALCULATETYPE calculatetype,
										   CPacketList* pPacketList,
										   CIndicatorInfo*& pIndicatorInfo,
										   CGraphCalculateData* pGraphCalculateData,
										   CPacket*& pTuringPoint, CPacket*& pStandardLine,
										   CPacket*& pLagSpan)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;

	CList<CString, CString> subGraphNameList;
	if(!pIndicatorInfo->GetSubGraphNameList(subGraphNameList))
		return false;

	POSITION pos_subGraphName = subGraphNameList.GetHeadPosition();

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	CString strSubGraphPacketNameHeader;
	if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

	POSITION pos_graphCalData = pGraphCalculateData->GetHeadPosition();
	while(pos_subGraphName && pos_graphCalData)
	{
		CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetNext(pos_graphCalData);
		if(pSubGraphPacketList == NULL)
			return false;

		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CPacket* pPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + subGraphNameList.GetNext(pos_subGraphName), false);
/*		if(pTuringPoint == NULL)
			pTuringPoint = pPacket;
		else if(pTuringPoint != NULL && pStandardLine == NULL)	
			pStandardLine = pPacket;
		else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan == NULL)	
			pLagSpan = pPacket;
		else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL)//선행스팬
			return ( GetSubGraphPacket( calculatetype, pPacketList, pSubGraphPacketList, pIndicatorInfo) &&
					pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL);
*/
		// 일목균형표 선행, 후행 별도 설정 - ojtaso (20071226)
		if( pStandardLine == NULL)
			pStandardLine = pPacket;
		else if(pStandardLine != NULL && pTuringPoint == NULL)
			pTuringPoint = pPacket;
		else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan == NULL)
			pLagSpan = pPacket;
		else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL)//선행스팬
			return ( GetSubGraphPacket( calculatetype, pPacketList, pSubGraphPacketList, pIndicatorInfo) &&
					pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL);

		// (2007/6/18 - Seung-Won, Bae) Clear Packet for Recalculation.
		ClearPacketData( pPacket);

		pSubGraphPacketList->AddTail2( pPacket);
	}
	return (pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL && m_pLeadSpan1 != NULL && m_pLeadSpan2 != NULL);

}

bool CGlanceBalance::GetSubGraphPacket( const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList, CSubGraphPacketList* pSubGraphList,
									   CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	bool bClearResult = false;
	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		m_pLeadSpan1 = pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C3_PRE_SPAN_1), false);
		m_pLeadSpan2 = pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C3_PRE_SPAN_2), false);

		if(ClearPacketData(m_pLeadSpan1))
			bClearResult = true;
		if(ClearPacketData(m_pLeadSpan2))
			bClearResult = true;
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( m_pLeadSpan1);
	pSubGraphList->AddTail2( m_pLeadSpan2);

	return (bClearResult || (m_pLeadSpan1 != NULL && m_pLeadSpan2 != NULL));
}

void CGlanceBalance::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
										CGraphCalculateData* pGraphCalculateData,
										CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pLeadSpan1);
	ClearPacketData(m_pLeadSpan2);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

void CGlanceBalance::GetTurningPointLine(const CPoint& point, double dCondition,
										 CList<double, double>* pTopPriceList,
										 CList<double, double>* pBottomPriceList,
										 CList<double, double>& dTurningPointLineList)
{ //전환선 계산
	GetMA(point, dCondition, pTopPriceList, pBottomPriceList, dTurningPointLineList);
}

void CGlanceBalance::GetStandardLine(const CPoint& point, double dCondition,
									 CList<double, double>* pTopPriceList,
									 CList<double, double>* pBottomPriceList,
									 CList<double, double>& dStandardLineList)
{	//	기준선 계산
	GetMA(point, dCondition, pTopPriceList, pBottomPriceList, dStandardLineList);
}

void CGlanceBalance::GetMA(const CPoint& point, double dCondition,
						   CList<double, double>* pTopPriceList,
						   CList<double, double>* pBottomPriceList,
						   CList<double, double>& dResultList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pTopPriceList || !m_pEndPriceList) return;

	// dCondition간 주가의 최고값과 최저값의 중간평균치를 이동평균선처럼 그린다.
	// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
	//		if Error, Set All with Close for Base or Market Close Price.
	for(int count = point.x ; count < pTopPriceList->GetCount() ; count++)
	{
		if(count >= (dCondition - 1))
		{
			int start = (int)floor(count - dCondition +1);

			POSITION posTop = FindPosition(start, *pTopPriceList);
			POSITION posBottom = FindPosition(start, *pBottomPriceList);
			POSITION posClose = FindPosition(start, *m_pEndPriceList);
			if(posTop == NULL || posBottom == NULL || posClose == NULL) return;

			double dTop = GetAt(pTopPriceList, posTop);
			double dBottom = GetAt(pBottomPriceList, posBottom);
			double dClose = GetAt( m_pEndPriceList, posClose);

			// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( 0 == dTop || 0 == dBottom) dTop = dBottom = dClose;

			for(int j = start ; j <= count ; j++) 
			{
				double dTopPrice = GetNext(pTopPriceList, posTop);
				double dBottomPrice = GetNext(pBottomPriceList, posBottom);
				double dClosePrice = GetNext( m_pEndPriceList, posClose);

				// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
				//		if Error, Set All with Close for Base or Market Close Price.
				if( 0 == dTopPrice || 0 == dBottomPrice) dTopPrice = dBottomPrice = dClosePrice;

				dTop = dTop > dTopPrice ? dTop : dTopPrice;
				dBottom = dBottom < dBottomPrice ? dBottom : dBottomPrice;
			}
			SetData(point, count, (dTop + dBottom) / 2.0, &dResultList);
		}
		else
			SetData(point, count, 0.00, &dResultList);
	}
}

//end의 5번쨰 데이터가 후행스팬의 첫번쨰 데이터가 된다.
void CGlanceBalance::GetLagSpan(const CPoint& point, double dCondition, CList<double, double>* pEndPriceList, CList<double, double>& dLagSpanList)
{	//	후행스팬 계산
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPriceList) return;

	if(pEndPriceList->GetCount() < dCondition)
		return;

	int startpoint = point.x;
	//계산을 이 시점부터 시작해야 한다. 만약. 100번째 data가 real로 변경되었다면..
	//후행스팬의 경우는 74번쨰 Data가 마지막이고 그 Data가 변경되어야 한다.
	if(startpoint != 0) //append, realappend
		startpoint -= ((int)dCondition - 1);

	int nDataCount = pEndPriceList->GetCount();
	POSITION pos = FindPosition((int)(dCondition) + startpoint - 1, *pEndPriceList);
	for(int count = (int)(dCondition) + startpoint - 1; count < nDataCount; count++)
	{
		if(pos == NULL)
			return ;
		double dData = GetNext(pEndPriceList, pos);
		//종가데이터count - (dCondition - 1) == lagspan데이터count 면 update mode
		//lagspan이 종가데이터보다 dCondition-1보다 작으니까...
		SetData(point, count - int(dCondition - 1), dData, &dLagSpanList);
	}
}

/////
//(기준선 + 전환선) /2 를 26일 선행시킨선.
//	선행스팬1 계산
///////
//맨 마지막 데이타 포함해서 26일
//만약 26개의 데이타와 조건이 26일이라면..51개의 데이타가 리스트에 저장되어야 한다.
//첫번째 데이타는 26번째에 들어가야 한다. 
//지금까지는 모두 index가 아닌 실제 count이다.
void CGlanceBalance::GetLeadSpan1(const CPoint& point,
								  const int nDataCount,
								  double dCondition,
								  CList<double, double>* pTurningPointLine,
								  CList<double, double>* pStandardLine,
								  CList<double, double>& dLeadSpanList)
{
	if(nDataCount < dCondition)
			return;

	POSITION posTurning = FindPosition(point.x, *pTurningPointLine);
	POSITION posStandard = FindPosition(point.x, *pStandardLine);

	//conditiondl 26이면 앞 리스트 25개의 데이터가 0이다
	if(point.x == 0) //new, insert
	{ 
		for(int count = 0 ; count < dCondition - 1 ; count++)	
			dLeadSpanList.AddTail(0.00);
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pTurningPointLine) return;

	for(int count = point.x ; count < pTurningPointLine->GetCount() ; count++)
	{
		if(posTurning == NULL || posStandard == NULL)
			return ;
		double dData = (GetNext(pTurningPointLine, posTurning) + GetNext(pStandardLine, posStandard)) / 2.0;
		//기본데이터보다 count + dCondition - 1만큼 데이터가 더 많음.
		SetData(point, count + int(dCondition - 1), dData, &dLeadSpanList);
	}
}
/////
//(과거 52일간의 고가 + 저가) /2 를 26일 선행시킨선.
///////
//맨 마지막 데이타 포함해서 26일
//만약 26개의 데이타와 조건이 26일이라면..51개의 데이타가 리스트에 저장되어야 한다.
//첫번째 데이타는 26번째에 들어가야 한다. 
//지금까지는 모두 index가 아닌 실제 count이다.
// 일목균형표 선행, 후행 별도 설정 - ojtaso (20071226)
void CGlanceBalance::GetLeadSpan2(const CPoint& point, const double& dSpanCond1, const double& dSpanCond2, 
								  CList<double, double>* pTopPriceList,
								  CList<double, double>* pBottomPriceList,
								  CList<double, double>& dLeadSpanList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pTopPriceList || !pBottomPriceList || !m_pEndPriceList) return;

	if(pTopPriceList->GetCount() < dSpanCond2)
			return;

	if(point.x == 0) // insert, new의 경우. dSpanCond1이전까지 0으로 채움.
	{ 
		for(int i = 0 ; i < dSpanCond1 - 1 ; i++)
			dLeadSpanList.AddTail(0.00);
	}

	// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
	//		if Error, Set All with Close for Base or Market Close Price.
	for(int i = point.x ; i < pTopPriceList->GetCount() ; i++)
	{
		if(i >= (dSpanCond2 - 1)) 
		{
			double high = GetAt(pTopPriceList, i);
			double low = GetAt(pBottomPriceList, i);
			double close = GetAt( m_pEndPriceList, i);

			// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( 0 == high || 0 == low) high = low = close;

			int start = (int)floor(i - dSpanCond2 +1);

			POSITION posTop = FindPosition(start, *pTopPriceList);
			POSITION posBottom = FindPosition(start, *pBottomPriceList);
			POSITION posClose = FindPosition(start, *m_pEndPriceList);
			for(int j = start ; j <= i ; j++)
			{
				if(posTop == NULL || posBottom == NULL || posClose == NULL) return;
				double dTop = GetNext(pTopPriceList, posTop);
				double dBottom = GetNext(pBottomPriceList, posBottom);
				double dClose = GetNext( m_pEndPriceList, posClose);

				// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
				//		if Error, Set All with Close for Base or Market Close Price.
				if( 0 == dTop || 0 == dBottom) dTop = dBottom = dClose;

				high = high > dTop ? high : dTop;
				low = low < dBottom ? low : dBottom;
			}
			SetData(point, i + int(dSpanCond1 - 1), (high + low) / 2.0, &dLeadSpanList);
		}
		else
			SetData(point, i + int(dSpanCond1 - 1), 0.00, &dLeadSpanList);
	}
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.02.03
//
//	분석	:	대기 매물 Chart
//					1. 종가와 거래량 Packet을 Input으로하여, 대기매물 가격대 Packet과
//						대기매물 거래량누계 Packet을 산출한다.
//					2. 대기매물 가격대 Packet과 거래량누계 Packet은 고가에서 저가의 가격대 순으로 등록되어 있다.
//					3. 대기매물 가격대 Packet에 등록되는 값은 각 가격대 구간의 하한값이다.
//					4. 대기매물 거래량누계 Packet을 실제 조건치보다 2개 더 초기화한다.
//						그것은 향후 Drawing 시점에서 참조할 Data로 최종 종가의 가격대 Index와
//						산출 구간내의 가격대별 거래량누계중 최대인 가격대의 Index를 관리한다.
//						Drawing시에 최종 가격대와 최대 가격대의 High Light에 참조된다.
//
//	수정	:	(2004.02.05, 배승원) Real 처리시에는 수신된 Real에 대한 대기매물 지표의 보정작업으로
//					구현되어 있었으나, 실제로 Real이 들어와도 그것이 구간내인 경우에만 보정해야 하므로 Logic이 맞지 않는다.
//					아마 전에는 항상 전체 보기를 기준으로 되어 있었던 것같다.
//					따라서 Real 처리도 TR처럼 항상 산출 구간을 중심으로 재계산하는 것으로 수정한다.
///////////////////////////////////////////////////////////////////////////////
CHorizontalBar::CHorizontalBar() : CIndicator( IMetaTable::EDDC_OCX_VOLUMEFORSALE)
{
	// (2004.02.03, 배승원, 분석) 대기매물 지표의 산출에 필요한 Packet을 관리한다.
	m_pHorZonPacket			= NULL;
	m_pHorZonPricePacket	= NULL;
	m_pEndPriceList			= NULL;
	m_pVolumeList			= NULL;

	// (2004.02.03, 배승원, 분석) 대기매물 지표의 산출 구간의 기본값을 관리한다.
	m_StartPoint			= 0;
	m_EndPoint				= 0;
	m_Max					= 0.0;
	m_Min					= 0.0;
}

// (2004.02.03, 배승원, 분석) 대기매물 지표값을 산출한다. (산출을 위한 기본 정보 구성)
void CHorizontalBar::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 1. Packet List를 구하여 관리한다.
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	
	// 2. 먼저 TR의 처리시를 처리한다. (초기화한다.)
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		// 2.1 Input Packet을 구한다.
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		CPacket *pPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));// "종가"
		if( !pPacket) return;
		m_pEndPriceList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData( _MTEXT( C0_VOLUME));
		// (2006/1/12 - Seung-Won, Bae) Check NULL Pointer
		if( !pPacket) return;
		m_pVolumeList = pPacket->GetnumericDataList();
		// 2.2 Output Packet Pointer를 Clear한다.
		//		(Object의 생성은 어디서 하는지?)
		m_pHorZonPacket = NULL;
		m_pHorZonPricePacket = NULL;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
		CString csHoga;
		(pPacketList->GetHelpMessageMap())->Lookup("HBPRICE", csHoga);
		m_dHoga = atof(csHoga);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	}

	// 3. Input Packet이 유효한지 확인한다.
	if( !IsValidPacket( m_pEndPriceList) || !IsValidPacket( m_pVolumeList)) return;

	// 4. 대기매물 Output Packet을 확보한다.
	// 4.1 이미 확보되어 있는 경우는 제외한다. (TR의 경우에만 NULL로 초기화되어 있다.)
	if( !m_pHorZonPacket || !m_pHorZonPricePacket)
	{
		// 4.2 CGraphCalculateData에서 Output Packet을 확보한다.
		// 4.2.1 지표 Output Packet 정보가 있는 경우만 처리한다.
		//		SubGraphList의 갯수는 2개 (대기매물은 항상 가격차트를 수반한다.)
		//		첫번째 리스트에는 대기매물 거래량누계, 대기매물가격대 패킷 주소
		//		두번째 리스트에는 시,고,저,종 패킷 주소
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pGraphCalculateData) return;
		if( pGraphCalculateData->GetCount() > 0)
		{
			// 4.2.2 CGraphCalculateData에서 Output Packet의 Sub Graph List를 확보한다.
			CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
			if( !pSubGraphList) return;

			// 4.2.3 Output Packet을 확보한다.
			m_pHorZonPacket			= pSubGraphList->GetPacket( 0);
			m_pHorZonPricePacket	= pSubGraphList->GetPacket( 1);
		}

		// 4.3 CGraphCalculateData에서 Output Packet의 확보에 실패한 경우를 처리한다.
		//		Packet List에서 직접 구한다.
		if( !m_pHorZonPacket || !m_pHorZonPricePacket)
		{
			// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
			CString strSubGraphPacketNameHeader;
			if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

			// 4.3.1 Packet List에서 고정 이름의 대기매물 Output Packet을 구한다.
			m_pHorZonPacket			= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C3_OVERHANGING_SUPPLY),							FALSE);
			m_pHorZonPricePacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C3_OVERHANGING_SUPPLY) + _MTEXT( C6_PRICE_BAND),	FALSE);

			// 4.3.2 Packet의 확보에 실패한 경우를 처리한다.
			if( !m_pHorZonPacket || !m_pHorZonPricePacket) return;

			// 4.3.3 구해진 Packet의 Data를 초기화한다.
			// (2004.02.05, 배승원) Real 처리시에는 수신된 Real에 대한 대기매물 지표의 보정작업으로
			//		구현되어 있었으나, 실제로 Real이 들어와도 그것이 구간내인 경우에만 보정해야 하므로 Logic이 맞지 않는다.
			//		아마 전에는 항상 전체 보기를 기준으로 되어 있었던 것같다.
			//		따라서 Real 처리도 TR처럼 항상 산출 구간을 중심으로 재계산하는 것으로 수정한다.
			//		따라서 if 구간밖에서 항상 Packet을 Clear시키고 재계산한다.

			// 4.3.4 구해진 Packet를 대기매물 I/O Packet 정보에 등록시킨다.
			CSubGraphPacketList *pSubGraphList = NULL;
			POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
			if( posSubGraph)
			{
				pSubGraphList = pGraphCalculateData->GetNext( posSubGraph);
				if( pSubGraphList)
				{
					pSubGraphList->AddTail2( m_pHorZonPacket);
					pSubGraphList->AddTail2( m_pHorZonPricePacket);
				}
			}
		}
	}

	// (2004.06.25, 배승원) 종가가 하나도 없는 경우에서는 대기매물을 계산하지 않는다.
	if( m_pEndPriceList->GetCount() <= 0) return;

	// 6. 대기매물 산출 구간의 정보를 구하여 관리한다.
	//		이후 GetHorizontalBar()에서 이용한다.
	m_StartPoint = chartFormulateData.GetDisplayDataStartIndex();
	m_EndPoint = chartFormulateData.GetDisplayDataEndIndex();
	
	// (2006/8/22 - Seung-Won, Bae) Real Calculate is in the case of Last Data in View.
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL
		&& m_EndPoint < m_pEndPriceList->GetCount() - 1)
			return;

	// 5. Scroll에 의한 대기매물 재계산을 위하여 Ouput Packet을 Clear시킨다.
	// (2004.02.05, 배승원) Real 처리시에는 수신된 Real에 대한 대기매물 지표의 보정작업으로
	//		구현되어 있었으나, 실제로 Real이 들어와도 그것이 구간내인 경우에만 보정해야 하므로 Logic이 맞지 않는다.
	//		아마 전에는 항상 전체 보기를 기준으로 되어 있었던 것같다.
	//		따라서 Real 처리도 TR처럼 항상 산출 구간을 중심으로 재계산하는 것으로 수정한다.
	//		따라서 항상 Packet을 Clear시키고 재계산한다.
	ClearPacketData( m_pHorZonPacket);
	ClearPacketData( m_pHorZonPricePacket);

	// (2004.06.25, 배승원) 시종 Index가 정상적인 값이 아니면 대기매물을 계산하지 않는다.
	if( m_EndPoint < 0 || m_StartPoint < 0) return;
	CPacket *pEndPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));//"종가"
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPacket) return;
	pEndPacket->GetMinMax( m_StartPoint, m_EndPoint, m_Min, m_Max);
	// (2004.06.25, 배승원) 종가 범주의 폭이 0이면, 최저가격치 이하로 (선물/옵션 감안, 0.001) 폭을 넓힌다.
	if( m_Min == m_Max)
	{
		m_Min -= 0.001;
		m_Max += 0.001;
	}
	
	// 7. 대기매물 지표를 산출한다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pIndicatorInfo) return;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
//	GetHorizontalBar( chartFormulateData, calculateData.GetPoint(),
//		GetCondition( 0, pIndicatorInfo->GetCommonCalConditionList()), m_pEndPriceList, m_pVolumeList);
	GetHorizontalBar( chartFormulateData, calculateData.GetPoint(),
		GetCondition( 0, pIndicatorInfo->GetCommonCalConditionList()), m_pEndPriceList, m_pVolumeList,
		GetCondition( 1, pIndicatorInfo->GetCommonCalConditionList()), m_dHoga);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가

	// 8. 대기매물 Output Packet을 등록한다.
	SetPacketTypes(m_pHorZonPacket,		pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
	SetPacketTypes(m_pHorZonPricePacket,	pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
}

// (2004.02.03, 배승원, 분석) 대기매물 지표를 산출한다. (가격대 구간 구성 및 거래량 누계 처리)
void CHorizontalBar::GetHorizontalBar(const CChartFormulateData& chartFormulateData, 
				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
				CList<double, double>* pVolumeList,double dConditionType, double dHoga)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPriceList || !pVolumeList) return;

	// 1. 종가와 체결량 Packet의 Data를 확인한다.
	if( pEndPriceList->GetCount() < 1 || pVolumeList->GetCount() < 1) return;

	// 2. 산출 범위의 끝 Index를 확인하여, 실 Data를 벗어나는 경우를 조정한다.
	//		(이것은 Chart의 우측 여백이 있는 경우로 판단된다.)
	if( m_EndPoint > pEndPriceList->GetCount() - 1) m_EndPoint = pEndPriceList->GetCount() - 1;

	// 3. Real 수신시의 경우를 우선 처리하여 제외한다.
	// (2004.02.05, 배승원) Real 처리시에는 수신된 Real에 대한 대기매물 지표의 보정작업으로
	//		구현되어 있었으나, 실제로 Real이 들어와도 그것이 구간내인 경우에만 보정해야 하므로 Logic이 맞지 않는다.
	//		아마 전에는 항상 전체 보기를 기준으로 되어 있었던 것같다.
	//		따라서 Real 처리도 TR처럼 항상 산출 구간을 중심으로 재계산하는 것으로 수정한다.

	// 4. TR이나 Scroll에 의한 재계산의 경우를 처리한다.
	// 4.1 이전에 산출한 대기매물 Data들이 있는 경우 Clear 시킨다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pHorZonPacket || !m_pHorZonPricePacket) return;
	if( m_pHorZonPacket->GetCount() > 0)
	{
		m_pHorZonPacket->ClearData();
		m_pHorZonPricePacket->ClearData();
	}

	// 4.2 먼저 대기매물 거래량누계 Packet을 구간별로 Clear시킨다.
	//		(실제 조건치보다 2개 더 초기화한다. 그것은 향후 Drawing 시점에서 참조할 Data로 
	//		 그것은 향후 Drawing 시점에서 참조할 Data로 최종 종가의 가격대 Index와
	//		 산출 구간내의 가격대별 거래량누계중 최대인 가격대의 Index를 관리한다.
	//		 Drawing시에 최종 가격대와 최대 가격대의 High Light에 참조된다.)
	CList<double, double>* pHorizontalBarList = m_pHorZonPacket->GetnumericDataList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pHorizontalBarList) return;

	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	double nInterval;
	if( dCondition < 1) dCondition = 1;
	if(dConditionType && 0 < dHoga)
	{
		nInterval = dHoga * dCondition;
		dCondition = int( ( ( m_Max - m_Min) / nInterval) * ( 1 + DBL_EPSILON)) + 1;
	}
	else
	{
		dHoga = 0;
		nInterval = ( m_Max - m_Min) / dCondition;
	}
	int i = 0;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	for( i = 0; i < dCondition + 2; i++) pHorizontalBarList->AddTail( 0.0);

	// 4.3 종가를 dCondition 개수로 분할하여 대기매물 가격대 Packet 값을 설정한다.
	CList<double, double>* pPriceZonList = m_pHorZonPricePacket->GetnumericDataList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPriceZonList) return;
// (2006.10.23 안승환) 대기매물 호가별 기능 추가	double nInterval = ( m_Max - m_Min) / dCondition;
// (2006.10.23 안승환) 대기매물 호가별 기능 추가	for( i = 0; i < dCondition; i++) pPriceZonList->AddHead( m_Min + nInterval * i);
	double	dHogaPrice = 0.0;
	for( i = 0; i <= dCondition; i++)
	{
		if( i == 0) dHogaPrice = m_Min;
		else if( i == dCondition && dHoga <= 0) dHogaPrice = m_Max;
		else dHogaPrice = m_Min + nInterval  * i;

		pPriceZonList->AddHead( dHogaPrice);
	}
// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	// 4.5 대기매물 산출구간의 거래량을 누계한다.
	//	(이때 최종 종가의 가격대 Index를 별도 등록 관리한다.)
	// 4.5.0 대기매물 산출 구간의 끝 종가와 거래량의 Position을 구하여 관리한다.
	//		이후 산출 끝범위를 확인하는데 이용한다.
	POSITION posEndPrice	= FindPosition( m_EndPoint, *pEndPriceList);
	POSITION posEndVolume	= FindPosition( m_EndPoint, *pVolumeList);
	if( posEndPrice == NULL || posEndVolume == NULL) return;
	// 4.5.1 대기매물 산출구간의 첫 종가와 거래량의 Position을 구한다.
	POSITION posOldPrice	= NULL, posPrice	= FindPosition( m_StartPoint, *pEndPriceList);
	POSITION posOldVolume	= NULL, posVolume	= FindPosition( m_StartPoint, *pVolumeList);
	if( posPrice == NULL || posVolume == NULL) return;
	// 4.5.2 산출 구간의 각 종가를 바탕으로, 해당 대기매물 가격대의 거래량에 누계처리한다.
	//		(구간 끝까지만 처리하도록 한다.)
	int nIndex;
	while( posPrice && posVolume && posEndPrice	!= posOldPrice && posEndVolume != posOldVolume)
	{
		// 4.5.2.0 먼저 Position을 Backup한다.
		posOldPrice		= posPrice;
		posOldVolume	= posOldVolume;

		// 4.5.2.1 가격대 Index를 산출한다.
		// (2008/4/23 - Seung-Won, Bae) Repaire of the loss in double calculation.
		double dValue = ( pEndPriceList->GetNext( posPrice) - m_Min) / nInterval;
		dValue *= ( 1 + DBL_EPSILON * 1000);
		nIndex = int( dCondition - int( dValue)) - 1;

		// 4.5.2.2 Zero Base로 전환한다.
		//		이때 종가가 산출구간내의 최고가인 경우에만, nIndex가 -1로 조정되는데,
		//		이는 첫 가격대 구간인 최고가 구간은 대표값인 구간 하한값을 포함할 뿐만 아니라
		//			산출 전체구간의 최고가도 포함한다.
		if( nIndex < 0) nIndex = 0;

		// 4.5.2.3 구해진 Index의 가격대에 거래량을 누계한다.
		// (2007/3/15 - Seung-Won, Bae) Check NULL Position
		POSITION posVolumeBar = FindPosition( nIndex, *pHorizontalBarList);
		if( posVolumeBar) pHorizontalBarList->SetAt( posVolumeBar, GetAt( pHorizontalBarList, posVolumeBar)
			+ pVolumeList->GetNext(posVolume));

		// 4.5.2.4 마지막으로 누계된 최종 종가의 가격대 Index를 등록하여 관리한다.
		posVolumeBar = FindPosition( ( int)dCondition, *pHorizontalBarList);
		if( posVolumeBar) pHorizontalBarList->SetAt( posVolumeBar, nIndex);
	}

	// 4.6 대기매물 산출구간의 가격대별 거래량 누계중 최고치인 가격대의 Index를 별도 관리한다.
	POSITION pos = pHorizontalBarList->GetHeadPosition();
	if(pos == NULL) return;
	double dVolume, dMaxVolume = 0;
	for(i = 0; i < dCondition; i++)
	{
		dVolume = pHorizontalBarList->GetNext(pos);
		if( dVolume >= dMaxVolume)
		{
			dMaxVolume = dVolume;
			pHorizontalBarList->SetAt( FindPosition( ( int)dCondition + 1, *pHorizontalBarList), i);
		}
	}
}

				
//******************************************************************************************************************
//역시계 곡선 
//:종축(30일 주가 이동평균치)과 횡축(30일 거래량 이동평균치)의 교차점 연결
//******************************************************************************************************************

CInverseLine::CInverseLine() : 
	m_pPriceMA(NULL), 
	m_pVolumeMA(NULL),
	m_pEndPricePacket(NULL),
	m_pVolumePacket(NULL),
	m_pEndPriceList(NULL),
	m_pVolumeList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_INVERSLINECHART)
{
}

void CInverseLine::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pEndPricePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pEndPriceList = GetnumericDataList(m_pEndPricePacket);
		m_pVolumePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pVolumeList = GetnumericDataList(m_pVolumePacket);
	}
	if(!IsValidPacket(m_pEndPriceList) || !IsValidPacket(m_pVolumeList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pIndicatorInfo) return;

	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dCondition0 = GetCondition(0, pConditionList);

	CPoint point = calculateData.GetPoint();

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pEndPricePacket || !m_pVolumePacket) return;

	if(m_pEndPricePacket == NULL)
		SetPacketTypes(m_pPriceMA, TYPE_TWODECIMAL, TYPE_INTERGER, (int)dCondition0 - 1);
	else
	{
		// (2008/10/17 - Seung-Won, Bae) Use packet type of price.
		CString strPacketType = m_pEndPricePacket->GetType(false);
		strPacketType.Replace( "×", "");
		strPacketType.TrimLeft( "xX ");
		if( strPacketType.IsEmpty())				strPacketType = TYPE_TWODECIMAL;
		else if( strPacketType.GetAt( 0) == '1')	strPacketType = TYPE_TWODECIMAL;
		SetPacketTypes(m_pPriceMA, strPacketType, m_pEndPricePacket->GetType(false), (int)dCondition0 - 1);
	}

	SetPacketTypes(m_pVolumeMA, TYPE_TWODECIMAL, TYPE_INTERGER, (int)dCondition0 - 1);

	GetSimpleMA(point, dCondition0, m_pEndPriceList, m_pPriceMA);
	GetSimpleMA(point, dCondition0, m_pVolumeList, m_pVolumeMA);
}

//특수형
bool CInverseLine::SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);

	}
	return (m_pPriceMA != NULL && m_pVolumeMA != NULL);
}

//특수형
bool CInverseLine::InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;

	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;

	m_pPriceMA	= pPacketList->GetBaseData( _MTEXT( C0_INVERSETIMELINE_PRICE),	false);
	m_pVolumeMA	= pPacketList->GetBaseData( _MTEXT( C0_INVERSETIMELINE_VOLUME),	false);

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( m_pPriceMA);
	pSubGraphList->AddTail2( m_pVolumeMA);

	// (2007/4/30 - Seung-Won, Bae) Add Time Data
	CPacket *pDatePacket = pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( pDatePacket) pSubGraphList->AddTail2( pDatePacket);

	return (m_pPriceMA != NULL || m_pVolumeMA != NULL);
}

bool CInverseLine::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		m_pPriceMA = pPacketList->GetBaseData( _MTEXT( C0_INVERSETIMELINE_PRICE), false);
		m_pVolumeMA = pPacketList->GetBaseData( _MTEXT( C0_INVERSETIMELINE_VOLUME), false);

		ClearPacketData(m_pPriceMA);
		ClearPacketData(m_pVolumeMA);
	}
	return (m_pPriceMA != NULL  && m_pVolumeMA != NULL );
}

void CInverseLine::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pPriceMA);
	ClearPacketData(m_pVolumeMA);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}



//////////////////////////////////////////////////////////////////////////
// Swing Chart
// 종찬(05/06/14)
//
//////////////////////////////////////////////////////////////////////////
CSwingChart::CSwingChart() : CIndicator( IMetaTable::EDDC_OCX_SWINGCHART)
{
	m_pEndPricePacket = NULL;
	m_pDatePacket = NULL;
	m_pSwingPrice = NULL;
	m_pSwingDate = NULL;
}


void CSwingChart::GetData(const CChartFormulateData& chartFormulateData, 
						  CGraphCalculateData* pGraphCalculateData, 
						  CIndicatorInfo*& pIndicatorInfo, 
						  CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(!pPacketList || !pIndicatorInfo)
		return;
	
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();	// 설정값
	CList<double, double>* pEndPriceList = NULL;	// 종가
	CList<double, double>* pDateList = NULL;		// 자료일자

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		
		//자료일자와 종가패킷가져오기 (IndiCond.cfg를 참조)
		m_pEndPricePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		
	}
	if(!m_pEndPricePacket || !m_pDatePacket)
		return;

	pEndPriceList = this->GetnumericDataList(m_pEndPricePacket);
	pDateList = GetnumericDataList(m_pDatePacket);

	// 데이타값이 유효한지를 검사한다.
	if(!IsValidPacket(pEndPriceList) || !IsValidPacket(pDateList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	if(!pIndicatorInfo)
		return;

	// 칸전환값 구하기
	double dInverseUnit = GetCondition(0, pConditionList);

	CString strPriceType = m_pEndPricePacket->GetType(false);
	CString strDateType = m_pDatePacket->GetType(false);
	SetPacketTypes(m_pSwingPrice, strPriceType, strPriceType, 0);
	SetPacketTypes(m_pSwingDate, strDateType, strDateType, 0);


	// 한칸값 구하기 = 최고가 최저가 가격폭의 2%.
	double dMin = 0.00;
	double dMax = 0.00;
	m_pEndPricePacket->GetMinMax(0, m_pEndPricePacket->GetCount() - 1, dMin, dMax);
	double dUnitValue = (dMax - dMin) / 50.0;

	GetSwingChart(calculateData.GetPoint(), dUnitValue, dInverseUnit, pEndPriceList, pDateList, m_pSwingPrice, m_pSwingDate);	
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetSwingChart
 - Created at  :  2005-06-16   10:11
 - Author      :  최종찬
 - Parameters  :  point         - 
 -                dUnitValue    - 한칸값
 -                dInverseUnit  - 칸전환값
 -                pEndPriceList - 종가 데이타
 -                pDateList     - 자료일자 데이타
 -				  pSwingPrice   - [out] Swing Data
 -				  pSwingDate    - [out] Swing 차트의 자료일자.
 - Description :  SwingChart의 데이타를 계산해낸다. (P&F 계산법과 동일)
 -----------------------------------------------------------------------------------------*/
void CSwingChart::GetSwingChart(const CPoint& point, 
								double dUnitValue,
								double dInverseUnit,
								CList<double, double>* pEndPriceList,
								CList<double, double>* pDateList,
								CPacket* pSwingPrice,
								CPacket* pSwingDate)
{
	if(pEndPriceList == NULL || pDateList == NULL)
		return;
	
	double dBaseData = 0.00; // SwingChart를 계산할때 필요한 BaseData.
	double dBaseDate = 0.00;	// BaseData의 자료일자.
	UINT nIndex = 0;		// SwingChart 패킷에 Setting할 순서 인덱스.
	short nSignal = 0;	// 현재의 Swing Data 추세를 설정한다. -1: 하락, 0: 보합(처음시작시), 1: 상승
	double dInverseUnitValue = dUnitValue * dInverseUnit;	// 최소반전폭 = 한칸값 * 칸전환값

	POSITION posEndPrice = pEndPriceList->GetHeadPosition();
	POSITION posDate = pDateList->GetHeadPosition();

	// BaseData를 설정한다.(첫번째 종가)
	dBaseData = pEndPriceList->GetNext(posEndPrice);
	dBaseDate = pDateList->GetNext(posDate);
	SetData(point, nIndex, dBaseData, pSwingPrice);
	SetData(point, nIndex, dBaseDate, pSwingDate);
	nIndex++;

	while(posEndPrice)
	{
		double dEndPrice = pEndPriceList->GetNext(posEndPrice);
		double dDate = pDateList->GetNext(posDate);

		// 1. 처음 추세 결정용.
		if(0 == nSignal)	
		{
			// 1.0. Data는 앞서 while문 앞에서 Insert를 했다.
			//     추세만 결정하면 된다.
			
			// 1.1. 현재종가가 기준가+최소반전폭 이상일때.
			if(dBaseData+dInverseUnitValue <= dEndPrice)
			{
				// 1.1.1. 새로운 BaseData설정
				dBaseData = dEndPrice;
				dBaseDate = dDate;

				// 1.1.2. 상승추세
				nSignal = 1;
			}
			// 1.2. 현재종가가 기준가+최소반전폭 이하일때.
			else if(dBaseData-dInverseUnitValue >= dEndPrice)
			{
				// 1.2.1. 새로운 BaseData설정
				dBaseData = dEndPrice;
				dBaseDate = dDate;

				// 1.2.2. 하락추세
				nSignal = -1;
			}
		}
		// 2. 상승추세일때...
		else if(1 == nSignal)
		{
			// 2.1. 기준가-최소반전폭 이하로 종가가 내려가면
			if(dBaseData-dInverseUnitValue >= dEndPrice)
			{	
				// 2.1.1. 추세반전
				nSignal = -1;
				// 2.1.2. Insert
				SetData(point, nIndex, dBaseData, pSwingPrice);
				SetData(point, nIndex, dBaseDate, pSwingDate);
				nIndex++;
				// 2.1.3. 새로운 BaseData썰정
				dBaseData = dEndPrice;
				dBaseDate = dDate;
			}
			// 2.2. 기준가+한칸값이상 상승이면 새로운 BaseData설정.
			else if(dBaseData+dUnitValue < dEndPrice)
			{
				dBaseData = dEndPrice;
				dBaseDate = dDate;
			}
			// 2.3. 기준가+한칸값미만 상승이거나 기준가-최소반전폭 미만의 하락
			// 2.3.1 아무것도 하지 않는다. ^^;
		}
		// 3. 하락추세일때...
		else if(-1 == nSignal)
		{
			// 3.1. 기준가+최소반전폭 이상으로 종가가 올라가면
			if(dBaseData+dInverseUnitValue <= dEndPrice)
			{
				// 3.1.1. 추세반전
				nSignal = 1;
				// 3.1.2. Insert
				SetData(point, nIndex, dBaseData, pSwingPrice);
				SetData(point, nIndex, dBaseDate, pSwingDate);
				nIndex++;
				// 3.1.3. 새로운 BaseData설정
				dBaseData = dEndPrice;
				dBaseDate = dDate;
			}
			// 3.2. 기준가-한칸값이하로  하락이면 새로운 BaseData설정.
			else if(dBaseData-dUnitValue > dEndPrice)
			{
				dBaseData = dEndPrice;
				dBaseDate = dDate;
			}
			// 3.3. 기준가+한칸값미만 상승이거나 기준가-최소반전폭미만의 하락
			// 3.3.1 아무것도 하지 않는다.
		}
	}

	// 최종 BaseData를 Insert한다.
	SetData(point, nIndex, dBaseData, pSwingPrice);
	SetData(point, nIndex, dBaseDate, pSwingDate);

	return;
}

bool CSwingChart::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculateType, CPacketList* pPacketList)
{
	if(!pPacketList)
		return false;

//	if(calculateType == CCalculateBaseData::TRDATA_CAL)
//	{
		m_pSwingPrice = pPacketList->GetBaseData( _MTEXT( C0_SWING_CHART_PRICE), false);
		m_pSwingDate = pPacketList->GetBaseData( _MTEXT( C0_SWING_CHART_DATE_TIME), false);

		ClearPacketData(m_pSwingPrice);
		ClearPacketData(m_pSwingDate);
//	}
	return (m_pSwingPrice != NULL && m_pSwingDate != NULL);
}

bool CSwingChart::SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo)
{
	if(!pGraphCalculateData)
		return false;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	return (m_pSwingPrice != NULL && m_pSwingDate != NULL);
}

bool CSwingChart::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo, CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;
	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;
	if(!pSubGraphList)
		return false;

	m_pSwingPrice	= pPacketList->GetBaseData( _MTEXT( C0_SWING_CHART_PRICE),	false);
	m_pSwingDate	= pPacketList->GetBaseData( _MTEXT( C0_SWING_CHART_DATE_TIME),		false);

	pSubGraphList->AddTail2( m_pSwingPrice);
	pSubGraphList->AddTail2( m_pSwingDate);
	return (m_pSwingPrice != NULL || m_pSwingDate != NULL);
}

void CSwingChart::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pSwingPrice);
	ClearPacketData(m_pSwingDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

//////////////////////////////////////////////////////////////////////////
// Kagi 차트
// 종찬(05/06/20)
//////////////////////////////////////////////////////////////////////////
CKagiChart::CKagiChart() : CIndicator( IMetaTable::EDDC_OCX_KAGICHART)
{
	m_pEndPricePacket = NULL;
	m_pDatePacket = NULL;
	m_pKagiPrice = NULL;
	m_pKagiDate = NULL;
}

void CKagiChart::GetData(const CChartFormulateData& chartFormulateData, 
					CGraphCalculateData* pGraphCalculateData, 
					CIndicatorInfo*& pIndicatorInfo, 
					CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(!pPacketList || !pIndicatorInfo)
		return;
	
	CList<double, double>* pEndPriceList = NULL;	// 종가
	CList<double, double>* pDateList = NULL;		// 자료일자

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		
		//자료일자와 종가패킷가져오기 (IndiCond.cfg를 참조)
		m_pEndPricePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
	}

	if(!m_pEndPricePacket || !m_pDatePacket)
		return;

	pEndPriceList = this->GetnumericDataList(m_pEndPricePacket);
	pDateList = GetnumericDataList(m_pDatePacket);

	// 데이타값이 유효한지를 검사한다.
	if(!IsValidPacket(pEndPriceList) || !IsValidPacket(pDateList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	if(!pIndicatorInfo)
		return;

	CString strPriceType = m_pEndPricePacket->GetType(false);
	CString strDateType = m_pDatePacket->GetType(false);
	SetPacketTypes(m_pKagiPrice, strPriceType, strPriceType, 0);
	SetPacketTypes(m_pKagiDate, strDateType, strDateType, 0);


	// 한칸값 구하기 = 최고가 최저가 가격폭의 3%.
	double dMin = 0.00;
	double dMax = 0.00;
	m_pEndPricePacket->GetMinMax(0, m_pEndPricePacket->GetCount() - 1, dMin, dMax);
	double dUnitValue = (dMax - dMin) * 0.03;


	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dInverseUnit = GetCondition(0, pConditionList);

	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	GetKagiChart(calculateData.GetPoint(), dUnitValue, dInverseUnit, pEndPriceList, pDateList, m_pKagiPrice, /*m_pCenterLine,*/ m_pKagiDate);
}

// 조건값 사용자 지정 가능 - ojtaso (20070913)
void CKagiChart::GetKagiChart(const CPoint& point, 
							double dUnitValue, 
							double dInverseUnit,
							CList<double, double>* pEndPriceList, 
							CList<double, double>* pDateList,
							CPacket* pKagiPrice,
							CPacket* pKagiDate)
{
	if(pEndPriceList == NULL || pDateList == NULL)
		return;

	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	m_swingChart.GetSwingChart(point, dUnitValue, dInverseUnit, pEndPriceList, pDateList, pKagiPrice, pKagiDate);
}


bool CKagiChart::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculateType, CPacketList* pPacketList)
{
	if(!pPacketList)
		return false;

//	if(calculateType == CCalculateBaseData::TRDATA_CAL)
//	{
		m_pKagiPrice = pPacketList->GetBaseData( _MTEXT( C0_KAGI_PRICE), false);
		m_pKagiDate = pPacketList->GetBaseData( _MTEXT( C0_KAGI_DATE_TIME), false);

		ClearPacketData(m_pKagiPrice);
		ClearPacketData(m_pKagiDate);
//	}
	return (m_pKagiPrice != NULL && m_pKagiDate != NULL);
}


bool CKagiChart::SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo)
{
	if(!pGraphCalculateData)
		return false;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	return (m_pKagiPrice != NULL && m_pKagiDate != NULL);
}


bool CKagiChart::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo, CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;
	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;
	if(!pSubGraphList)
		return false;

	m_pKagiPrice	= pPacketList->GetBaseData( _MTEXT( C0_KAGI_PRICE),	false);
	m_pKagiDate		= pPacketList->GetBaseData( _MTEXT( C0_KAGI_DATE_TIME),	false);

	pSubGraphList->AddTail2( m_pKagiPrice);
	pSubGraphList->AddTail2( m_pKagiDate);
	return (m_pKagiPrice != NULL && m_pKagiDate != NULL);
}


void CKagiChart::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pKagiPrice);
	ClearPacketData(m_pKagiDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//Lock
///////////////////////////////////////////////////////////////////////////////////////////////////////
CLock::CLock( HWND p_hOcxWnd) : 
	m_pLockYesOrNotList(NULL),
	m_pLockRatioTextList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_LOCKCHART)
{
	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
}
void CLock::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		CPacket *pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pLockYesOrNotList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( pPacket) m_pLockRatioTextList = pPacket->GetnumericDataList();
	}
	if( !IsValidPacket(m_pLockYesOrNotList)) return;

	CPacket* pLock = NULL;
	if(!SetGraphPacketList(pPacketList, pGraphCalculateData, 
		chartFormulateData.GetPacketNames(), pIndicatorInfo, pLock))
		return;
	
	if(IsIndicatorCalculated( chartFormulateData.GetCalculateType(), pLock))
		return;

	//sy 2004.04.13.
	// "문자"일 경우에는 미리 type를 넣어 줘야 한다.
	// -> 기본으로 "X 1" 로 setting 하므로 add되지 않는다.
	SetPacketTypes(pLock, _MTEXT( C6_CHARACTER), _MTEXT( C6_CHARACTER), 0);
	GetLock(calculateData.GetPoint(), m_pLockYesOrNotList, m_pLockRatioTextList, pLock);
}

//sy 2004.04.13.
bool CLock::SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							const CString& strPacketNames, 
							CIndicatorInfo*& pIndicatorInfo,
							CPacket*& pLock)
{
	if(pPacketList == NULL || pGraphCalculateData == NULL)
		return false;

	// 첫번째 packet : Data Packet (계산한 Packet)
	if(!CIndicator::SetGraphPacketList(pPacketList, 
		pGraphCalculateData, pIndicatorInfo, pLock))
		return false;

	// 두번째 packet : 그릴 위치를 알려주는 packet (TR로 받은 Packet)
	CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
	if(pSubGraphList == NULL)
		return false;

	if(pSubGraphList->GetCount() >= 2)
		return true;

	CString strPacketName = strPacketNames;
	CDataConversion::GetStringData(strPacketName, ",");
	strPacketName.TrimRight(",");
	if(strPacketName.IsEmpty())
		return false;

	CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return false;

	pSubGraphList->AddTail2( pPacket);
	return true;
}

void CLock::GetLock(const CPoint& point, CList<double, double>* pLockList, CList<double, double>* pLockRatioTextList, CPacket* pLock)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pLockList) return;

	if( pLockList->GetCount() <= 0) return;

	POSITION pos = FindPosition(point.x, *pLockList);
	POSITION RatioPos = NULL;
	if( pLockRatioTextList) RatioPos = FindPosition(point.x, *pLockRatioTextList);
	CString strRatio,strLock;
	for(int count = point.x ; count < pLockList->GetCount() ; count++) 
	{
		if(pos == NULL) return;
		strLock = GetData((int)pLockList->GetNext(pos));

		if( pLockRatioTextList &&  RatioPos != NULL) strRatio = GetRatioTextData( pLockRatioTextList->GetNext(RatioPos));
		SetData( point, count, strLock, pLock);
	}
}

CString CLock::GetRatioTextData(const double nRatioData) const
{
	CString strText;
//	if(nRatioData<=0.0)
//		return "";
//	else
	
		strText.Format("(%.2f%%)",nRatioData);
		return strText;
//	}
}

CString CLock::GetData(const int nData) const
{
	if(nData==0) return "";

	ML_SET_LANGUAGE_RES();

	// data 가 3자리인 경우 "1":거래소, "2":코스닥
	CString strLock;
	switch(nData)
	{
	// 거래소
	case 1:
	case 110:
	case 210:	strLock.LoadString( IDS_EX_RIGHTS);
				break;
	case 2:
	case 120:
	case 220:	strLock.LoadString( IDS_EX_DIVIDEND);
				break;
	case 3:
	case 130:
	case 230:	strLock.LoadString( IDS_EX_RIGHTS_AND_DIVIDEND);
				break;
	case 6:		strLock.LoadString( IDS_EX_DOUBLE_DIVIDEND);
				break;
	case 7:		strLock.LoadString( IDS_EX_RIGHTS_AND_DOUBLE_DIVIDEND);
				break;
	case 140:
	case 240:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN);
				break;
	case 141:
	case 241:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_EX_RIGHTS);
				break;
	case 142:
	case 242:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_EX_DIVIDEND);
				break;
	case 143:
	case 243:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_EX_RIGHTS_AND_DIVIDEND);
				break;
	case 146:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_INTERIM_DIVIDEND);
				break;
	case 147:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_EX_RIGHTS_AND_INTERIM_DIVIDEND);
				break;
	case 150:
	case 250:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION);
				break;
	case 151:		
	case 251:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_EX_RIGHTS);
				break;
	case 152:
	case 252:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_EX_DIVIDEND);
				break;
	case 153:
	case 253:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_EX_RIGHTS_AND_DIVIDEND);
				break;
	case 156:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_INTERIM_DIVIDEND);
				break;
	case 157:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_EX_RIGHTS_AND_INTERIM_DIVIDEND);
				break;
	case 160:	strLock.LoadString( IDS_INTERIM_DIVIDEND);
				break;
	case 170:	strLock.LoadString( IDS_EX_RIGHTS_AND_INTERIM_DIVIDEND);
				break;

	// 코스닥
	case 246:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_REDUCTION_OF_CAPITAL);
				break;
	case 247:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_STOCKS_ANNEXATION);
				break;
	case 256:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_REDUCTION_OF_CAPITAL);
				break;
	case 257:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_STOCKS_ANNEXATION);
				break;
	case 260:	strLock.LoadString( IDS_REDUCTION_OF_CAPITAL);
				break;
	case 270:	strLock.LoadString( IDS_STOCKS_ANNEXATION);
				break;
	case 280:	strLock.LoadString( IDS_ENTERPRISE_PARTITION);
				break;
	case 290:	strLock.LoadString( IDS_MUTUAL_FUND);
				break;
	}

	return strLock;
}
//sy end


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.22
//
//	분석	:	Market Profile (시간대별 가격 차트)
//					1. Server에서는 지정되는 가격폭과 시간폭을 기준으로 누계된 거래량 Data를 산출한다.
//						1. 일시		: MMDDHHMM
//						2. 가격		: 누계된 대표 가격이다.
//						3. CODE		: 시간의 흐름을 보여주는 Alphabet Code
//						4. Index	: 산출 순서를 나타는 Index이다.
//					2. Client에서는 통계된 Data를 출력하기 좋은 구조의 Data Structure를 형성시킨다.
//						1. CMPPacket		: MP Chart의 출력을 위한 Data Structure Object Class이다.
//						2. CMProfile		: CMPPacket을 구성하는 일별 Chart의 Data Structure Object Class인
//												MP Unit Chart Data Structure Object Class이다.
//						3. CMPItem			: CMProfile를 구성하는 가격대별 Graph의 Data Strucre Object Class이다.
//						4. CBracket			: CMPItem을 구성하는 가격대/시간대별의 Data Object이다.
//												바로 Server에서 내려준 1개 Packet Data을 나타낸다.
//					3. m_CurrentIndex, m_CurrentTime, m_CurrentChar, m_CurrentPriceX100
//						1. Server Data를 CMPPacket Data Object에 등록시키기 위한, 최종 등록 상태를 나타낸다.
//						2. TR의 경우 GetTotalMarketProfile()에서 첫 Data로 초기화되고, (첫 Data 없으면 -1쯤?)
//								GetTotalMarketProfile()을 호출하기 전에 IsValidPacket()으로 각 자료 Data Packet에 초기화할 Data가 있는지 검증된다.
//								GetDayMarketProfile()에서도 마지막 일자를 기준으로 GetTotalMarketProfile()이 호출되므로 당연 초기화할 Data가 있게 된다.
//						3. Real의 경우 TR에 의해 초기화가 실패되면, 또한 일별 새로운 CMProfile로 분리해야 하며...
//								Real의 시간은 DDHHMMSS로 나타난다.
//
//	주의	:	1. Local 변수나 Member 변수로 Double을 정의하여 이용하지 않는다.
//				2. double은 그 자체로 정확하지 않으므로 (근사치를 갖는다고 생각해야 한다.)
//					항상 Int나 Long 변수로 변환하여 이용한다.
//					(MSDN, Knowledge Base의 Miscellaneous, [Q125056] INFO: Precision and Accuracy in Floating-Point Calculations 참조)
//
//	수정	:	(2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
//					이를 위하여 CMProfile Chart의 Member로 Last Price를 기억하여 출력시 참조토록한다.
//					Server에서는 일별/통합 구분없이 TR Packet 끝의 추가 Packet Data로 Alphabet이 '*'인 Data를 받아,
//						그 Packet Data의 Price로 CMProfile Chart의 Last Price를 지정한다.
//						이 보조 Packet은 복수일 조회라 하더라도 마지막에 1개만 추가된다.
//					일별의 경우에는 마지막 Bracket이 종가이므로 이를 이용하여 설정한다.
//				(2004.02.10, 배승원) 일시 Packet에 '월'도 받아 X Scale에 출력토록 한다.
//				(2005/8/31 - Seung-Won, Bae) '*' is last price indicator. Because Last Braket can be not last price.
//					and first some Aphabet times have no Tick Data. then The First Braket's Alphabet Character must not be 'A'.
//						so. I need the time of Market Open for correcting this.
//					then. I need '*' Packet every time even if there is no tick in Market Open Time.
//						and that packet('*')'s Time must be Market Open Time for the Time of Last 'A'
//				(2005/9/7 - Seung-Won, Bae) In Hanwha, The Real Time 09:05 means 09:04:00.01 ~ 09:05:00.00
//						m_CurrentTime	- 3. Get from TR	: It is the Last Bracket's Time for Last Group Alphabet.
//										  2. Set to TR		: Set to m_LastAMinute with Last Character.
//																CMProfile::SetDate()
//																Reference for Creating New Propfile Chart on Real.
//										- 1. Get from Real	: It is the Last Real Time in Minute, not Current.
//										  2. Set to Real	: Set To Date of Real Time (It is from TR).
//										=> It is used for Only m_LastAMinute in TR.
//										=> It is used in Real for Only Remember Date.
//										=> Then, Is m_LastAMinute the Start Time of A or the End Time of A ?
//						m_LastAMinute	- Get from TR	: It is from Last Time (m_CurrentTime) & Alphabet of TR.
//										- Set to Real	: It is used to make a Alphabet of Real.
//										=> m_LastAMinute is used only for the logic to get Alphabet.
//										=> So. It is good to be the Start Time of Alphabet for the Logic.
//											Because. In the case of No data in TR. Server will send the Constant Market Open Time.
//										=> So. In Hanwha, It must be the Start Time (Minute) + 1 in Minute. ( The Start Time of B is 09:06 in Minute.)
//						Bracket's Time	- It is used to check if New Bracket in same alphabet.
//					So. The Time Field in TR must be the Start Time of Alphabet.
//						And m_CurrentTime is Last Bracket's Start Time. (before Market Open. It's 0900)
//						And It is not matter for Bracket's Time to be the Start Time or +1.
//						So in Hanwha. m_LastAMinute must have +1 for Upper Round.
///////////////////////////////////////////////////////////////////////////////
// (2004.01.16, 배승원, 분석) MP Chart 지표를 초기화 처리한다.
CMarketProfileChart::CMarketProfileChart() : CIndicator( IMetaTable::EDDC_OCX_MARKETPROFILECHART)
{
	m_pEndPriceList = NULL;
	m_pVolumeList = NULL;
	m_pDateList = NULL;

	// (2004.01.19, 배승원) 최종적으로 등록한 Data의 값들 관리한다.
	//		이후의 Data 추가시 참조하기 위함이다.
	m_CurrentIndex			= -1;	// 마지막으로 등록된 CBracket이 있는 CMPItem의 Index 번호이다.
	m_CurrentTime			= -1;	// 마지막으로 등록된 CBracket의 Data Time이다.
	m_CurrentPriceX100		= -1;	// 미자막으로 등록된 CBracket이 있는 CMPItem의 가격이다.
	m_CurrentBracketIndex	= -1;	// 마지막으로 등록된 CBracket의 Index값이다.
	m_CurrentChar			= ' ';	// 마지막으로 등록된 CBracket의 Character Item이다.

	m_strDateType = ' ';
	m_pPacketList = NULL;
	m_pAlpabetList = NULL;
	m_pIndexList = NULL;

	// (2004.01.19, 배승원) CMPItem에서 CBracket의 신규 추가를 식별하기 위하여 조건 시간과 가격을 관리한다.
	m_nTimeUnit		= 0;
	m_nPriceUnitX100	= 0;

	// (2004.01.26, 배승원) 마지막 'A'의 시간을 관리하여, Real을 처리토록한다.
	m_LastAMinute	= -1;

	// (2004.01.26, 배승원) Real 처리시를 위해 일별/통합 여부도 관리한다.
	m_nTotalMPChart	= 0;	// 1:통합, 0:일별
}
///////

// (2004.01.16, 배승원, 분석) MP Chart 지표 Data를 구성한다.
void CMarketProfileChart::GetData( const CChartFormulateData &chartFormulateData,
									CGraphCalculateData *pGraphCalculateData,
									CIndicatorInfo *&pIndicatorInfo,
									CCalculateData &calculateData)
{
	// 1. Packet List를 구한다.
	m_pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pPacketList) return;

	// 2. 지표 계산 Type을 구한다. (이후 Sub Routine에서 참조할 것이다.)
	m_CalType = chartFormulateData.GetCalculateType();

	// 3. TR 계산시의 초기 설정을 처리한다. (TR이면 기본 Data Packet등의 Pointer를 초기화한다.)
	if(m_CalType == CCalculateBaseData::TRDATA_CAL)
	{
		// 3.2 계산에 필요한 Packet Data들을 구한다.
		CString strPacketNames = chartFormulateData.GetPacketNames();
		// 3.2.1 날짜 Packet을 구한다.
		CPacket *pPacket = m_pPacketList->GetBaseData( CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pDateList = GetnumericDataList( pPacket);
		// 3.2.1.1 날짜 Packet의 Type을 구한다.
		m_strDateType = pPacket->GetType();
		// 3.2.2 종가 Packet을 구한다.
		pPacket = m_pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pEndPriceList = GetnumericDataList( pPacket);
		// 3.2.3 ALPHABET Packet을 구한다.
		pPacket = m_pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pAlpabetList = GetstrDataList( pPacket);
		// 3.2.4 Index Packet을 구한다.
		pPacket = m_pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pIndexList = GetnumericDataList( pPacket);
		// 3.2.5 거래량 Packet을 구한다.
		pPacket = m_pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pVolumeList = GetnumericDataList( pPacket);
	}

	// 4. 구해진 Data Packet들이 유효한지를 확인한다.
	//		(2004.02.18, 배승원) Real의 경우 Index List에는 Data가 하나도 없는 경우가 있으므로
	//			확인하지 않는다.
	if( !IsValidPacket( m_pEndPriceList) || !IsValidPacket( m_pVolumeList) || !IsValidPacket( m_pDateList))
		return;

	// 5. MarketProfile Packet Object Pointer를 구하고, 재계산 필요성을 확인한다.
	CPacket *pMarketPacket = NULL;
	if( !SetGraphPacketList( m_pPacketList, pGraphCalculateData, pIndicatorInfo, pMarketPacket)) return;
	if( IsIndicatorCalculated( m_CalType, pMarketPacket)) return;

	// 6. MarketProfile의 처리에 필요한 Condition Data를 확인한다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pIndicatorInfo) return;
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	if(pConditionList == NULL || pConditionList->GetCount() < 3) return ;
	// 6.1 이후 한 CMPItem에서 CBracket의 신규 추가를 식별하기 위하여 조건 시간을 관리한다.
	// (2005.02.26, 배승원) Int로 환산시 유효숫자의 끝이 X.XXXX99999로 끝나는 경우에 대비하여
	//		Epsilon을 더한뒤 Int로 환산한다.
	m_nPriceUnitX100	= int( ( GetCondition(0, pConditionList) * ( 1 + DBL_EPSILON)) * 100); //가격단위
	m_nTimeUnit			= ( int)GetCondition(1, pConditionList);		 //시간단위
	// 6.2 Real 처리시를 위해 일별/통합 여부도 관리한다.
	m_nTotalMPChart		= ( int)GetCondition(2, pConditionList);		 // 1:통합, 0:일별

	// 7. MarketProfile Packet을 구하는 경우를 구분하여 처리한다.
	CPoint point								= calculateData.GetPoint();
	CMPPacket* pPacket							= (CMPPacket* )pMarketPacket;
	CScaleBaseData::HORZSCALEDRAWERTYPE nType	= m_pPacketList->GetDateUnitType();
	// 7.0 Draw시에 참조가 가능하도록 MP Chart Packet에 가격폭 단위를 설정한다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacket) return;
	pPacket->SetPriceUnitX100( m_nPriceUnitX100);
	// 7.1 TR의 경우를 처리한다.
	if( m_CalType == CCalculateBaseData::TRDATA_CAL)
	{
		// 7.1.1 통합의 경우를 처리한다. (두번째 조건이 1)
		if( GetCondition( 2, pConditionList))	// 1:통합, 0:일별
			GetTotalMarketProfile(point, pConditionList, pPacket, 0, m_pEndPriceList->GetCount(), nType);

		// 7.1.2 일별의 경우를 처리한다. (두번째 조건이 0)
		else GetDayMarketProfile(point, pConditionList, pPacket, nType);

		// (2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
		//		최종 종가를 알려주는 보조 Packet을 감안하여 GetTotalMarketProfile()에서 최종 상태를 Backup 관리한다.
		//	m_CurrentChar	= *( const char *)m_pAlpabetList->GetTail();
		// 7.1.3.1 (2004.01.26, 배승원) 마지막 'A'의 시간을 관리하여, Real을 처리토록한다.
		// 7.1.3.1.1 먼저 최종 시간을 구한다. (시분 4자리만 구한다.)
		m_LastAMinute	= m_CurrentTime % 10000;
		m_LastAMinute	= m_LastAMinute / 100 * 60 + m_LastAMinute % 100;	// 시간 진법을 해제한다.
		// 7.1.3.1.2 최종 시간대 시작시간을 구한다.
		m_LastAMinute	-= m_LastAMinute % m_nTimeUnit;
		// 7.1.3.1.3 마지막 'A'의 시작시간을 구한다.
		m_LastAMinute	-= ( m_CurrentChar - 'A') * m_nTimeUnit;
		// (2005/9/7 - Seung-Won, Bae) Update Last A Minute for Ceiling Mode
		//		in floor,	A is 0900, 0901, 0902, 0903, 0904
		//		in Ceiling, A is 0901, 0902, 0903, 0904, 0905
		// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
		if( CPacketType::FLOOR != m_pPacketList->GetRealType()) m_LastAMinute++;
	}
	// 7.2 Real의 경우를 처리한다. (Real 처리시의 다음 Real 준비 설정은 GetRealMarketProfile()내에서 처리한다.)
	else GetRealMarketProfile( point, pConditionList, pPacket, nType);

	// 8. 구해진 Market Profile 지표 Packet에 기본 정보를 등록처리한다.
	CPacket* pClosePacket = m_pPacketList->GetBaseData( CString( "MP_") + _MTEXT( C0_CLOSE));
	assert(pClosePacket != NULL);
	SetPacketTypes( pPacket, pClosePacket->GetType(true), pClosePacket->GetType(false), 0);
}

// (2004.01.19, 배승원, 분석) 1개 MarketProfile 단위 Chart를 구성한다.
//		일별 Chart의 경우 일수만큼의 MarketProfile Chart를 생성토록 호출된다.
//		(원래는 CMPPacket에 등록하는 기능으로 CMPPacket의 Member Routine에 해당하는 기능이나,
//		Data Packet과 그 Handling Routine인 Indicator Member Routine을 이용하기 위하여 여기서 처리한다.)
void CMarketProfileChart::GetTotalMarketProfile(const CPoint& point, CList<double, double>* pConditionList,
  CMPPacket* pMarketPacket, int Start, int End, CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	// 1. MP Unit Chart Object를 생성하고, 일자를 설정한다.
	// 1.1 현재 MP Unit Chart에 등록된 Data의 MP Unit Chart내의 Index를 초기화한다.
	// (2004.01.19, 배승원) 최종적으로 등록한 Data의 값들 관리한다.
	//		이후의 Data 추가시 참조하기 위함이다.
	m_CurrentIndex		= 0;														// 마지막으로 등록된 CBracket이 있는 CMPItem의 Index 번호이다.
	m_CurrentTime		= ( int)GetAt( m_pDateList, ( int)Start);							// 마지막으로 등록된 CBracket의 Data Time이다.
	m_CurrentChar		= *( const char *)GetAt( m_pAlpabetList, ( int)Start);		// 마지막으로 등록된 CBracket의 Character Item이다.
	// (2005.02.26, 배승원) Int로 환산시 유효숫자의 끝이 X.XXXX99999로 끝나는 경우에 대비하여
	//		Epsilon을 더한뒤 Int로 환산한다.
	m_CurrentPriceX100	= int( ( GetAt( m_pEndPriceList, (int)Start) * ( 1 + DBL_EPSILON)) * 100);	// 미자막으로 등록된 CBracket이 있는 CMPItem의 가격이다.
	// (2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
	//		Server로부터 받는 최종 종가 Packet이 첨부터 있는 경우에는 처리를 하지 않는다.
	if( m_CurrentChar == '*')
	{
		m_CurrentChar = 'A';	// (2005/8/31 - Seung-Won, Bae) for Market Open Time.
		return;
	}
	// 1.2 MP Chart Object를 생성한다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pMarketPacket) return;
	CMProfile* pMarketProfile = pMarketPacket->AddTail();
	if( !pMarketProfile) return;
	// 1.2 첫 Time의 일자를 CMPPacket(단위 MP Chart Object)에 등록시킨다.
	pMarketProfile->SetDate( m_CurrentTime);

	// 2. 먼저 첫 Server Packet Data에 해당하는 CBracket Object를 생성 등록하여,
	//		이후 Packet Data 등록의 기준으로 삶는다.
	// 2.1 먼저 CMPItem Object를 생성/등록한다.
	CMPItem* pMPItem = pMarketProfile->AddTail();
	if( !pMPItem) return;
	// 2.2 생성된 CMPItem Object에 대표 가격과 거래량을 설정한다.
	//		(이때 가격을 Backup하여 이후 Packet Data 등록시 참조토록 한다.)
	pMPItem->SetPrice( m_CurrentPriceX100);
	pMPItem->AddVolume( ( DWORD)GetAt( m_pVolumeList, Start));
	// (2004.01.27, 배승원) Real 수신시를 위하여 최종 Index를 관리한다.
	m_CurrentBracketIndex = ( int)GetAt( m_pIndexList, Start);
	// 2.3 생성된 CMPItem Object에 Server Packet Data에 맞는 CBracket Object를 생성/등록한다.
	pMPItem->AddBracket( CString( m_CurrentChar), m_CurrentBracketIndex, m_CurrentTime);

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pIndexList || !m_pVolumeList || !m_pAlpabetList || !m_pEndPriceList || !m_pDateList) return;

	// 3. Server의 Packet Data를 Iterating하면서 각 Data를 등록처리한다.
	int			nPriceX100, nIndex, nDate;
	double		dVolume;		// 거래량은 수치가 크므로 Double로 관리한다.
	CString		strAlphabet;
	POSITION	posIndex	= FindPosition( ( int)Start + 1, *m_pIndexList);
	POSITION	posVolume	= FindPosition( ( int)Start + 1, *m_pVolumeList);
	POSITION	posAlphabet	= FindPosition( ( int)Start + 1, *m_pAlpabetList);
	POSITION	posEndPrice	= FindPosition( ( int)Start + 1, *m_pEndPriceList);
	POSITION	posDate		= FindPosition( ( int)Start + 1, *m_pDateList);
	for( int count = Start + 1; count <= End; count++)
	{
		// 3.1 먼저 지정된 Server Packet에 Data가 모자란 경우를 처리한다.
		if( !posEndPrice || !posVolume || !posAlphabet || !posIndex) return;

		// 3.2 Server의 Packet Data를 구한다.
		// (2005.02.26, 배승원) Int로 환산시 유효숫자의 끝이 X.XXXX99999로 끝나는 경우에 대비하여
		//		Epsilon을 더한뒤 Int로 환산한다.
		nPriceX100	= int( ( GetNext( m_pEndPriceList, posEndPrice) * ( 1 + DBL_EPSILON)) * 100);
		dVolume		= GetNext( m_pVolumeList,	posVolume);
		strAlphabet	= GetNext( m_pAlpabetList,	posAlphabet);
		nIndex		= ( int)GetNext( m_pIndexList,	posIndex);
		nDate		= ( int)GetNext( m_pDateList,		posDate);

		// (2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
		//		최종 종가를 알려주는 보조 Packet의 경우에는 MP Chart에 종가를 설정하고 처리를 종료한다.
		// 3.3.1 '*'의 경우를 확인한다.
		if( *( const char *)strAlphabet == '*')
		{
			// (2005/8/31 - Seung-Won, Bae) Set Current Time with Last Bracket Time.
			m_CurrentTime		= nDate;

			pMarketProfile->SetLastPriceX100( nPriceX100);
			return;
		}
		// 3.3.2 '*'의 경우를 감안하여 TR처리를 완전히 끝낸 GetData()가 아닌 이곳에서 매번 Backup 받아 놓는다.
		m_CurrentChar	= *( const char *)strAlphabet;

		// 3.4 이전 가격과 비교하여 Data를 위, 또는 아래로 확장하여 등록시킨다.
		if(			nPriceX100 > m_CurrentPriceX100) InsertUp(		pMarketProfile, nPriceX100, dVolume, strAlphabet, nIndex, nDate);
		else if(	nPriceX100 < m_CurrentPriceX100) InsertDown(	pMarketProfile, nPriceX100, dVolume, strAlphabet, nIndex, nDate);

		// 3.5 이전 가격과 같은 경우, 마지막 등록한 CMPItem Object에 Data를 추가한다.
		else
		{
			CMPItem *pMPItem = pMarketProfile->GetMPItem( m_CurrentIndex);
			if( !pMPItem) return;
			pMPItem->AddVolume( ( unsigned long)dVolume);

			// 3.6 이전 가격과 같아도 당연 Server에서 다른 Bracket으로 구성했을 테니 무조건 추가한다.
			pMPItem->AddBracket( strAlphabet, nIndex, nDate);

			// (2004.01.27, 배승원) Real 수신시를 위하여 최종 Index를 관리한다. (Insert Up/Down에서도 처리한다.)
			m_CurrentBracketIndex = nIndex;
		}
	}

	// (2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
	//		일별로 조회하면서 마지막 MP Chart가 아닌 경우의 마무리로, (마지막은 '*'를 확인하지만)
	//		마지막 등록한 Bracket의 가격으로 MP Chart의 마지막 가격을 설정한다.
	pMarketProfile->SetLastPriceX100( nPriceX100);
}

// (2004.01.19, 배승원, 분석) 이전에 등록된 CMPItem의 Index인 m_CurrentIndex의 윗쪽으로 확장하면서,
//		Data를 등록시킨다.
void CMarketProfileChart::InsertUp( CMProfile* pMarketProfile, int nPriceX100, double dVolume, CString& strAlpa, int nIndex, int nDate)
{
	// 1. (2004.01.27, 배승원) Real 수신시를 위하여 최종 Index를 관리한다.
	m_CurrentBracketIndex = nIndex;

	// 2. CMPItem의 List를 위로 Iterating하면서 추가할 Data 가격에 맞는 CMPItem을 찾아 등록시킨다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pMarketProfile) return;
	CList<CMPItem*, CMPItem*>* pMPItemList = pMarketProfile->GetMPItemList();
	if( !pMPItemList) return;
	POSITION posMPItem = pMPItemList->FindIndex( m_CurrentIndex + 1);
	while( posMPItem)
	{
		// 2.1 먼저 다음 위의 CMPItem Object를 구한다.
		CMPItem *pMPItem = pMPItemList->GetNext( posMPItem);

		// 2.2 CMPItem이 등록된 Position만 있고, Object Pointer가 NULL이면 오류로 바로 Return한다.
		if( !pMPItem) return;

		// 2.2.5 마지막 가격과 CMPItem의 Index를 관리하여 확장시에 참조한다.
		//	(원래는 등록대상인 CMPItem의 가격을 설정받아야 하나, 확장의 경우 참조할 수 있도록 미리 받아 놓는다.)
		m_CurrentPriceX100 = pMPItem->GetPriceX100();
		m_CurrentIndex++;

		// 2.3 등록할 Data와 가격이 같은 CMPItem Object를 찾은 경우, 해당 Object에 Data를 등록시키고 Return한다.
		if( m_CurrentPriceX100 == nPriceX100)
		{
			// 2.3.1 거래량을 누계시킨다.
			pMPItem->AddVolume( ( unsigned long)dVolume);

			// 2.3.2 CBracket Object를 추가시킬 필요가 있는지 확인하여 추가한다.
			//	( CBracket Object를 추가하지 않는 경우는, 마지막으로 등록된 CBracket Object가 
			//	  이번 Data와 시간적으로 하나의 구간에 속한 경우이다. (Real)
			//	  따라서, 우선적으로 시간의 간격이 구분 단위보다 큰 경우로 확인하고,
			//			그렇지 않은 경우 (시간 간격이 구분 단위보다 작더라도) Aphabet이 변경된 경우도 등록 처리한다.)
			//	( *. 주의, CMPItem이 있다고 해서 항상 CBracket Object를 포함하지 않을 수 있다. JUMP 가능)
			// (2004.01.27, 배승원) TR 수신시에는 Bracket 구성을 Server에서 책임지고 처리하므로 무조건 등록한다.
			CList< CBracket *, CBracket *> *pBracketList = pMPItem->GetBracketList();
			// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
			if( !pBracketList) return;
			if( pBracketList->IsEmpty() || m_CalType == CCalculateBaseData::TRDATA_CAL) pMPItem->AddBracket( strAlpa, nIndex, nDate);
			else
			{
				CBracket *pBracket = pBracketList->GetTail();
				// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
				if( !pBracket) return;
				if( nDate - pBracket->GetDate() > m_nTimeUnit	// 또한 초를 포함하므로 기준시도 시간진법의 분으로 환산한다.
					|| *( const char *)pBracket->GetItem() != *( const char *)strAlpa)
						pMPItem->AddBracket( strAlpa, nIndex, nDate);
				// (2004.01.27, 배승원) Real 수신시를 위하여 최종 Index를 관리한다.
				//		(InsertUp 시작시 바로 Backup하였으니, Real에 의해 등록시에 새로운 Bracket이 생성되지 않으면 다시 감한다.)
				else m_CurrentBracketIndex--;
			}

			// 2.3.4 Data의 추가가 끝났으므로 Return한다.
			return;
		}
	}

	// 3. 이미 등록된 CMPItem의 List에 알맞은 가격대가 없다면, 새로 확장하고 등록시킨다.
	// 3.1 마지막으로 등록되어 있는 가격을 기준으로 필요한 CMPItem Object를 확장한다.
	// 3.1.1 먼저 등록해야 하는 CMPItem Object의 수를 확인한다.
	// (2005/8/12 - Seung-Won, Bae) Check Devide by Zero
	if( !m_nPriceUnitX100) return;
	int nCount = ( nPriceX100 - m_CurrentPriceX100) / m_nPriceUnitX100;
	// 3.1.2 CMPItem Object를 개수만큼 생성한다.
	CMPItem *pMPItem = NULL;
	for( int i = 0; i < nCount; i++)
	{
		m_CurrentPriceX100 += m_nPriceUnitX100;
		m_CurrentIndex++;

		pMPItem = pMarketProfile->AddTail();
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pMPItem) return;
		pMPItem->SetPrice( m_CurrentPriceX100);
	}
	// 3.2 마지막으로 등록한 CMPItem Object에 CBracket Object를 생성하며 Data를 등록시킨다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pMPItem) return;
	pMPItem->AddVolume( ( unsigned long)dVolume);
	pMPItem->AddBracket( strAlpa, nIndex, nDate);
}

// (2004.01.19, 배승원, 분석) 이전에 등록된 CMPItem의 Index인 m_CurrentIndex의 아래쪽으로 확장하면서,
//		Data를 등록시킨다.
void CMarketProfileChart::InsertDown( CMProfile* pMarketProfile, int nPriceX100, double dVolume, CString& strAlpa, int nIndex, int nDate)
{
	// 1. (2004.01.27, 배승원) Real 수신시를 위하여 최종 Index를 관리한다.
	m_CurrentBracketIndex = nIndex;

	// 2. CMPItem의 List를 위로 Iterating하면서 추가할 Data 가격에 맞는 CMPItem을 찾아 등록시킨다.
	//	(물론 먼저 m_CurrentIndex값을 이용하여 찾을 대상이 있는지 확인한다.)
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pMarketProfile) return;
	CList<CMPItem*, CMPItem*>* pMPItemList = pMarketProfile->GetMPItemList();
	if( !pMPItemList) return;
	POSITION posMPItem = NULL;
	if( 0 < m_CurrentIndex) posMPItem = pMPItemList->FindIndex( m_CurrentIndex - 1);
	while( posMPItem && 0 < m_CurrentIndex)
	{
		// 2.1 먼저 다음 아래의 CMPItem Object를 구한다.
		CMPItem* pMPItem = pMPItemList->GetPrev(posMPItem);

		// 2.2 CMPItem이 등록된 Position만 있고, Object Pointer가 NULL이면 오류로 바로 Return한다.
		if( !pMPItem) return;

		// 2.2.5 마지막 가격과 CMPItem의 Index를 관리하여 확장시에 참조한다.
		//	(원래는 등록대상인 CMPItem의 가격을 설정받아야 하나, 확장의 경우 참조할 수 있도록 미리 받아 놓는다.)
		m_CurrentPriceX100 = pMPItem->GetPriceX100();
		m_CurrentIndex--;

		// 2.3 등록할 Data와 가격이 같은 CMPItem Object를 찾은 경우, 해당 Object에 Data를 등록시키고 Return한다.
		if( m_CurrentPriceX100 == nPriceX100)
		{
			// 2.3.1 거래량을 누계시킨다.
			pMPItem->AddVolume( ( unsigned long)dVolume);

			// 2.3.2 CBracket Object를 추가시킬 필요가 있는지 확인하여 추가한다.
			//	( CBracket Object를 추가하지 않는 경우는, 마지막으로 등록된 CBracket Object가 
			//	  이번 Data와 시간적으로 하나의 구간에 속한 경우이다. (Real)
			//	  따라서, 우선적으로 시간의 간격이 구분 단위보다 큰 경우로 확인하고,
			//			그렇지 않은 경우 (시간 간격이 구분 단위보다 작더라도) Aphabet이 변경된 경우도 등록 처리한다.)
			//	( *. 주의, CMPItem이 있다고 해서 항상 CBracket Object를 포함하지 않을 수 있다. JUMP 가능)
			// (2004.01.27, 배승원) TR 수신시에는 Bracket 구성을 Server에서 책임지고 처리하므로 무조건 등록한다.
			CList< CBracket *, CBracket *> *pBracketList = pMPItem->GetBracketList();
			// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
			if( !pBracketList) return;
			if( pBracketList->IsEmpty() || m_CalType == CCalculateBaseData::TRDATA_CAL) pMPItem->AddBracket( strAlpa, nIndex, nDate);
			else
			{
				CBracket *pBracket = pBracketList->GetTail();
				// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
				if( !pBracket) return;
				if( nDate - pBracket->GetDate() > m_nTimeUnit	// 또한 초를 포함하므로 기준시도 시간진법의 분으로 환산한다.
					|| *( const char *)pBracket->GetItem() != *( const char *)strAlpa)
						pMPItem->AddBracket( strAlpa, nIndex, nDate);
				// (2004.01.27, 배승원) Real 수신시를 위하여 최종 Index를 관리한다.
				//		(InsertDown 시작시 바로 Backup하였으니, Real에 의해 등록시에 새로운 Bracket이 생성되지 않으면 다시 감한다.)
				else m_CurrentBracketIndex--;
			}

			// 2.3.4 Data의 추가가 끝났으므로 Return한다.
			return;
		}
	}

	// 3. 이미 등록된 CMPItem의 List에 알맞은 가격대가 없다면, 새로 확장하고 등록시킨다.
	// 3.1 마지막으로 등록되어 있는 가격을 기준으로 필요한 CMPItem Object를 확장한다.
	// 3.1.1 먼저 등록해야 하는 CMPItem Object의 수를 확인한다.
	int nCount = ( m_CurrentPriceX100 - nPriceX100) / m_nPriceUnitX100;
	// 3.1.2 CMPItem Object를 개수만큼 생성한다.
	CMPItem *pMPItem = NULL;
	for( int i = 0; i < nCount; i++)
	{
		m_CurrentPriceX100 -= m_nPriceUnitX100;
		m_CurrentIndex = 0;

		pMPItem = pMarketProfile->AddHead();
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pMPItem) return;
		pMPItem->SetPrice( m_CurrentPriceX100);
	}
	// 3.2 마지막으로 등록한 CMPItem Object에 CBracket Object를 생성하며 Data를 등록시킨다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pMPItem) return;
	pMPItem->AddVolume( ( unsigned long)dVolume);
	pMPItem->AddBracket( strAlpa, nIndex, nDate);
}

// (2004.01.19, 배승원, 분석) 일별 MP Chart를 구성하는 Routine을 제공한다.
void CMarketProfileChart::GetDayMarketProfile(const CPoint& point, CList<double, double>* pConditionList,
			CMPPacket* pMarketPacket, CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	// 1. CMProfile Object를 구성한 구간의 기준값을 초기화한다.
	//	(dLastIndex는 nNextDate의 Index값이고, dStartIndex는 nPrevDate의 Index이다.
	//	 단, 처음 시작을 위하여 nPrevDate를 -1로 초기화하였다.)
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pDateList) return;
	int		nPrevDate = -1;
	int		nNextDate = GetMDay( ( int)m_pDateList->GetHead());
	int 	nStartIndex = 0;
	// (2004.01.26, 배승원) nLastIndex는 Loop내에서 바로 증가하므로 미리 1 감소시킨다. (원래 ZeroBase)
	int		nLastIndex = -1;

	// 2. 구간을 검색하여 CMProfile Object를 생성한다.
	POSITION posDate = m_pDateList->GetHeadPosition();
	while( posDate)
	{
		// 2.1 이전 일자와 다른 경우 새로운 일자의 Data 끝을 확인한다.
		//	(nPrevDate를 처음에 -1로 설정한 이유도 여기 있다.)
		nPrevDate = nNextDate;
		while( posDate && nPrevDate == nNextDate)
		{
			nNextDate = GetMDay( ( int)GetNext( m_pDateList, posDate));
			nLastIndex++;
		}

		// 2.2 GetTotalMarketProfile()로 구간내 Data로 CMProfile Object를 생성한다.
		//	(물론 dLastIndex Index의 nNextDate값은 이미 구간 밖에 해당한다.)
		// (2004.02.05, 배승원) 마지막 구간의 경우, 일자가 바뀌어 구간이 끝나지 않고 Data가 없어 구간이 끝난다.
		//		따라서, 그경우 nLastIndex는 구간 안을 가르킨다. 이를 식별하여 구간을 전달하도록 한다.
		GetTotalMarketProfile( point, pConditionList, pMarketPacket, nStartIndex, nLastIndex - ( posDate ? 1 : 0), nType);

		// 2.3 다음 구간을 위하여 Start값을 설정한다.
		//	(nPrevDate는 2.1의 Loop에서 처리된다.)
		nStartIndex = nLastIndex;
	}
}

// (2004.01.19, 배승원, 분석) 일별 계산을 위하여 들어온 데이타(예 DDHHMMSS 행태의 DATE에서 DD만을 뽑아 낸다..
int CMarketProfileChart::GetMDay( int nDate)
{
	// 1. 먼저 Date Data Format을 확인한다.
	int nDataTypeLen = m_strDateType.GetLength();
	if( nDataTypeLen < 2) return 0;

	// 2. Date Data Format에서 "DD"를 자리순을 확인한다.
	int nDigit = m_strDateType.Find( "DD");
	if( nDigit < 0) return 0;

	// 3. "DD"이후의 자릿수를 확인한다.
	nDigit = nDataTypeLen - nDigit - 2;

	// 4. "DD" 이후의 자릿수만큼 10^n으로 나눠 "DD"이후를 절사시킨다.
	nDate = int( nDate / pow( 10, nDigit));

	// 5. 100의 나머지로 "DD" 자리의 값을 구한다.
	return nDate % 100;
}

// (2004.01.19, 배승원, 분석) Real이 수신되는 경우의 MP Chart를 구성한다.
void CMarketProfileChart::GetRealMarketProfile(const CPoint& point,CList<double, double>* pConditionList, 
			CMPPacket* pMarketPacket, CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	// 1. 먼저 거래량 Data수와 일자 Data수가 맞지 않는 경우를 제외한다.
	//	(아직 Packet 단위 Real이 수신되지 않는 경우에 대한 Code이다.)
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pDateList || !m_pVolumeList) return;
	if( m_pVolumeList->GetCount() != m_pDateList->GetCount()) return;
	
	// 2. 또 CMProfile List를 구하지 못하는 경우를 처리한다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pMarketPacket) return;
	CList<CMProfile*, CMProfile*>* pMProfileList = pMarketPacket->GetMarketProfileList();
	if( !pMProfileList) return;

	// 3. Real 등록할 CMProfile Chart Object를 검색한다.
	//	(오늘 일자의 CMProfile Object가 등록되지 않은 경우도 처리한다. 바로 당일 첫 Real의 경우이다.)
	BOOL bAddNewMProfile = FALSE;
	CMProfile *pMarketProfile = NULL;
	// 3.1 새로운 CMProfile Object가 필요한 지를 판단한다.
	// 3.1.0 Real의 시간(Time)을 미리 확보한다. (추가 기준이 되며, 일자는 없다.)
	// (2004.01.27, 배승원) 기준 Packet을 'MP_시간'에서 '자료일자'로 변경하여 DDHHMMSS로 Data가 오나,
	//		실제 TR Packet은 DDHHMM으로 처리된다. 따라서 초를 없앤다.
	// (2004.02.10, 배승원) 일시 Packet에 '월'도 받아 X Scale에 출력토록 한다.
	//		즉 TR Packet이 MMDDHHMM으로 수신된다.
	//		따라서, Real로 수신된 일시에서 시간만 추출하여, 마지막 수신된 TR Data의 월일을 추가한다.
	// (2005/8/31 - Seung-Won, Bae) Real이 HHMMSS로 수신되어도 자료일자 Packet에서 자동으로 MMDDHHMM으로 변경한다.
	//		따라서 HHMM만 추출하여 이용한다.
	// (2006/5/25 - Seung-Won, Bae) PacketData.dll process Date Setting already.
	//	int nDate = int( m_pDateList->GetTail()) % 10000 + ( ( m_CurrentTime / 10000) * 10000);
	int nDate = ( int)m_pDateList->GetTail();
	// 3.1.1 CMProfile Chart가 하나도 없거나
	if( pMProfileList->IsEmpty()) bAddNewMProfile = TRUE;
	else
	{
		// 3.1.2 미리 마지막 CMProfile Chart를 검색하여 이후에 새로이 생성되지 않는 경우에도 대비한다.
		pMarketProfile = pMProfileList->GetTail();
		// 3.1.3 통합 Mode가 아니면서, 마지막 등록된 CMProfile Chart의 마지막 시각이 이번 Real 시각보다 늦는 경우 (시간 역전)
		//		(이때 마지막 CMProfile은 마지막 Bracket의 시간을 갖지 않고, m_CurrentTime이 마지막 Bracket의 시간을 갖는 것에 주의하자.)
		//		(Real 처리 오류로 가끔 시간이 들어 있는 Packet이 조회되기도 한다. 아마 아직 시간 Packet은 반영되지 않은듯..)
		bAddNewMProfile = ( !m_nTotalMPChart && ( nDate < m_CurrentTime));
		// (2004.02.10, 배승원) 일시 Packet에 '월'도 받아 X Scale에 출력토록 한다.
		//		Real의 경우 시간이 역전되는 경우는 없다. 즉 TR의 마지막 시간보다 이른 Real이 올 수 없다.
		//		굳이 있다면 장전에 Open한 Map이 Real을 수신하는 경우이다.
		//		이러한 경우를 감안하여 일자를 보정하는 Code를 추가시킨다.
		//		특히 전체보기의 경우에는 역전된 일자를 바로 잡아줘야만 Alphabet이 정상적으로 나타난다.
		if( m_nTotalMPChart && ( nDate < m_CurrentTime)) nDate += 10000;	// 일자를 하나 올려준다.
	}
	// 3.2 새로이 생성한다. (생성되지 않는 경우, 마지막 CMProfile Chart가 검색되어 있다.)
	if( bAddNewMProfile)
	{
		// (2004.02.10, 배승원) 일시 Packet에 '월'도 받아 X Scale에 출력토록 한다.
		//		Real의 경우 시간이 역전되는 경우는 없다. 즉 TR의 마지막 시간보다 이른 Real이 올 수 없다.
		//		굳이 있다면 장전에 Open한 Map이 Real을 수신하는 경우이다.
		//		이러한 경우를 감안하여 일자를 보정하는 Code를 추가시킨다.
		SYSTEMTIME stDate;
		GetLocalTime( &stDate);
		nDate = nDate % 10000 + stDate.wMonth * 1000000 + stDate.wDay * 10000;

		pMarketProfile = pMarketPacket->AddTail();
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pMarketProfile) return;
		pMarketProfile->SetDate( nDate);
	}

	// 4. 먼저 Real로 수신된 Packet의 Data를 확보한다.
	// 4.1 시간(Time) 확보한다. (추가 기준이 되며, 일자는 없다.)
	//		(앞의 CMProfile 검색에 참조하기 위하여 미리 확보하였다.)
	// 4.2 수신된 종가를 가격 단위에 맞춰 계산한다. (단위 가격폭으로 나눠 무조건 절사한다.)
	// (2005.02.26, 배승원) Int로 환산시 유효숫자의 끝이 X.XXXX99999로 끝나는 경우에 대비하여
	//		Epsilon을 더한뒤 Int로 환산한다.
	int nPriceX100 = ( int( ( m_pEndPriceList->GetTail() * ( 1 + DBL_EPSILON)) * 100) / m_nPriceUnitX100) * m_nPriceUnitX100;
	// 4.3 등록할 거래량 Data를 조회한다. (Real에서는 매도/매수에 따라 부호가 설정되어 있다.)
	double dVolume = fabs( m_pVolumeList->GetTail());
	// 4.4 Alphabet은 계산하여 산출한다.
	// 4.4.1 새로운 CMProfile Chat의 상황에서의 Alphabet 산출을 위하여 'A' 기준시를 지정한다.
	// (2005/8/31 - Seung-Won, Bae) Server에서 최소한 1개의 Packet Set('*')을 내려주면서
	//		체결이 없는 경우 장시작시간을 내려주어 반드시 m_LastAMinute가 초기화되도록 하여 추가 초기화 Logic은 삭제한다.
	// 4.4.2 Real의 Alphabet을 설정한다.
	// 4.4.2.1 먼저 Real 시간을 분수로 산출한다. (기준시와 비교하기 위함이다.)
	int nRealMinute = nDate % 10000;
	nRealMinute	= nRealMinute / 100 * 60 + nRealMinute % 100;	// 시간 진법을 해제한다.
	// 4.4.2.2 기준시로부터 흐른 시간 산출한다.
	nRealMinute -= m_LastAMinute;
	// 4.4.2.3 분수에 맞는 Aphabet을 산출한다.
	CString strAlpha;
	char *szAlpha = strAlpha.GetBufferSetLength( 2);
	szAlpha[ 0] = 'A' + int( nRealMinute / m_nTimeUnit) % 26;

	// 5. TR이 수신되지 않은 상황에서의 Real 추가를 위한, 최종 상태 변수들을 초기화하여 가상의 이전 환경을 구성한다.
	//		(실제로는 새로운 CMProfile이 생성되는 경우마다 초기화하고 새로이 시작토록한다.)
	if( pMarketProfile->IsEmpty())
	{
		// 5.1 기본 변수들을 초기화한다.
		m_CurrentIndex			= 0;			// 마지막으로 등록된 CBracket이 있는 CMPItem의 Index 번호이다.
		m_CurrentChar			= 'A';			// 마지막으로 등록된 CBracket의 Character Item이다.
		m_CurrentPriceX100		= nPriceX100;	// 마자막으로 등록된 CBracket이 있는 CMPItem의 가격이다.
		m_CurrentTime			= nDate;		// 마지막으로 등록된 CBracket의 등록시간이다.
		m_CurrentBracketIndex	= 0;			// 마지막으로 등록된 CBracket의 Index이다.

		// 5.2 CMProfile Data Object의 첫 Data 등록인 경우를 처리한다.
		CMPItem *pMPItem = pMarketProfile->AddHead();
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pMPItem) return;
		pMPItem->SetPrice( nPriceX100);
	}

	// 6. 이전에 등록한 Data를 기준으로 Insert 처리한다.
	if( nPriceX100 > m_CurrentPriceX100)		InsertUp(	pMarketProfile, nPriceX100, dVolume, strAlpha, m_CurrentBracketIndex + 1, nDate); 
	else if( nPriceX100 < m_CurrentPriceX100)	InsertDown(	pMarketProfile, nPriceX100, dVolume, strAlpha, m_CurrentBracketIndex + 1, nDate);
	// 6.1 이전 가격과 같은 경우, 마지막 등록한 CMPItem Object에 Data를 추가한다.
	else
	{
		CMPItem *pMPItem = pMarketProfile->GetMPItem( m_CurrentIndex);
		if( !pMPItem) return;
		pMPItem->AddVolume( ( unsigned long)dVolume);

		//	( *. 주의, CMPItem이 있다고 해서 항상 CBracket Object를 포함하지 않을 수 있다. JUMP 가능)
		CList< CBracket *, CBracket *> *pBracketList = pMPItem->GetBracketList();
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pBracketList) return;
		if( pBracketList->IsEmpty()) pMPItem->AddBracket( strAlpha, ++m_CurrentBracketIndex, nDate);		// 최종 Index도 1증가시킨다.
		else
		{
			CBracket *pBracket = pBracketList->GetTail();
			// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
			if( !pBracket) return;
			if( nDate - pBracket->GetDate() > m_nTimeUnit		// 또한 초를 포함하므로 기준시도 시간진법의 분으로 환산한다.
				|| *( const char *)pBracket->GetItem() != szAlpha[ 0])
					pMPItem->AddBracket( strAlpha, ++m_CurrentBracketIndex, nDate);	// 최종 Index도 1증가시킨다.
		}
	}

	// 7. 다음 Real 처리시에 참조할 수 있도록 기준 Data를 설정한다.
	// m_CurrentIndex는 InsertUp과 Down에서 설정된다.
	m_CurrentPriceX100	= nPriceX100;	// 미자막으로 등록된 CBracket이 있는 CMPItem의 가격이다.
	m_CurrentTime		= nDate;		// 마지막으로 등록된 CBracket의 Data Time이다.
	m_CurrentChar		= szAlpha[ 0];	// 마지막으로 등록된 CBracket의 Character Item이다.
	// m_CurrentBracketIndex는 InsertUp과 Down에서 설정된다.

	// (2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
	//		Real의 경우, Real 처리후 MP Chart에 최종 종가로서 등록한다.
	pMarketProfile->SetLastPriceX100( nPriceX100);
}


//******************************************************************************************************************
//	CPF
//	  
//******************************************************************************************************************

CPF::CPF() : 
	m_pTLBTop(NULL), 
	m_pTLBBottom(NULL),
	m_pTLBDate(NULL),
	m_pEndPricePacket(NULL),
	m_pDatePacket(NULL),
	m_pEndPriceList(NULL),
	m_pDatePriceList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_PANDFCHART)
{
}

void CPF::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList || !pIndicatorInfo) return;
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();

//	(2006.08.14, 안승환) 한칸의 값및 칸 전환수를 수정할수 있도록 함 
	CList<double, double>* pDrawConditionList = pIndicatorInfo->GetDrawConditionList();
	BOOL	bReal = FALSE;
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL)
		bReal = TRUE;
//	(2006.08.14, 안승환) 한칸의 값및 칸 전환수를 수정할수 있도록 함 
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pEndPricePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pEndPriceList = GetnumericDataList(m_pEndPricePacket);
		m_pDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDatePriceList = GetnumericDataList(m_pDatePacket);
	}
	if(!IsValidPacket(m_pEndPriceList) || !IsValidPacket(m_pDatePriceList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	double nMin = 0;
	double nMax = 0;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pEndPricePacket || !m_pDatePacket) return;

	CString strPriceType = m_pEndPricePacket->GetType(false);
	CString strDateType = m_pDatePacket->GetType(false);
	SetPacketTypes(m_pTLBTop, strPriceType, strPriceType, 0);
	SetPacketTypes(m_pTLBBottom, strPriceType, strPriceType, 0);
	SetPacketTypes(m_pTLBDate, strDateType, strDateType, 0);

	m_pEndPricePacket->GetMinMax(0, m_pEndPricePacket->GetCount() - 1, nMin, nMax);
	GetPF(calculateData.GetPoint(), nMin, nMax, pConditionList, m_pEndPriceList, m_pDatePriceList, 
				m_pTLBTop, m_pTLBBottom, m_pTLBDate,pDrawConditionList,bReal, 
				pPacketList->GetDataInsertType() == CPacketDataInfo::NEW, pPacketList);
}

//특수형
bool CPF::SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	return (m_pTLBTop != NULL && m_pTLBBottom != NULL && m_pTLBDate != NULL);
}

//특수형
bool CPF::InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;

	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;

	m_pTLBTop		= pPacketList->GetBaseData( _MTEXT( C0_PF_HIGH),		false);//"PF_고가"
	m_pTLBBottom	= pPacketList->GetBaseData( _MTEXT( C0_PF_LOW),			false);//"PF_저가"
	m_pTLBDate		= pPacketList->GetBaseData( _MTEXT( C0_PF_DATE_TIME),	false);//"PF_자료일자"

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( m_pTLBTop);
	pSubGraphList->AddTail2( m_pTLBBottom);
	pSubGraphList->AddTail2( m_pTLBDate);
	return (m_pTLBTop != NULL || m_pTLBBottom != NULL || m_pTLBDate != NULL);
}

//나중에 실시간 처리 확인
//실시간 들어올때도 첨부터 다시 계산해야 되니까...data를 전부 지워준다.
bool CPF::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL)
	{
		m_pTLBTop = pPacketList->GetBaseData(		_MTEXT( C0_PF_HIGH),		false);//"PF_고가"
		m_pTLBBottom = pPacketList->GetBaseData(	_MTEXT( C0_PF_LOW),			false);//"PF_저가"
		m_pTLBDate = pPacketList->GetBaseData(		_MTEXT( C0_PF_DATE_TIME),	false);//"PF_자료일자"

		ClearPacketData(m_pTLBTop);
		ClearPacketData(m_pTLBBottom);
		ClearPacketData(m_pTLBDate);
	}
	else if(calculatetype == CCalculateBaseData::REALDATA_CAL)
	{
		ClearPacketData(m_pTLBTop);
		ClearPacketData(m_pTLBBottom);
		ClearPacketData(m_pTLBDate);
	}
	return (m_pTLBTop != NULL  && m_pTLBBottom != NULL  && m_pTLBDate != NULL );
}

void CPF::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pTLBTop);
	ClearPacketData(m_pTLBBottom);
	ClearPacketData(m_pTLBDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}


void CPF::GetPF(const CPoint &point, const double nMin, const double nMax, CList<double, double>* pConditionList,
				CList<double, double>* pClosePriceList, CList<double, double>* pDateList,
				CPacket* pTOPPacket, CPacket* pBOTTOMPacket, CPacket* pDATEPacket,
				CList<double, double>* pDrawConditionList,BOOL bReal,BOOL bDataType, CPacketList* pPacketList)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return ;

//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
	double dUnitSize;
	POSITION pos = pDrawConditionList->GetHeadPosition();
	if( !pos) return;
	double nIndex = pDrawConditionList->GetAt(pos);
	// (2007/6/19 - Seung-Won, Bae) Support Only Draw Condition of P&F for User Setting.
	if( nIndex) //  || bReal ||!bDataType
		dUnitSize = GetCondition(1, pConditionList);
	else
	{
		// (2008/11/2 - Seung-Won, Bae) Use HBPRICE.
		dUnitSize = (nMax - nMin) / 50;
		CString strTickValue;
		if( pPacketList->GetHelpMessageItem( "HBPRICE", strTickValue))
		{
			double dTickValue = atof( strTickValue);
			if( dUnitSize < dTickValue) dUnitSize = dTickValue;
		}

		pConditionList->SetAt(pConditionList->FindIndex(1),dUnitSize);
	}
//	if(dUnitSize <= 0)
//		dUnitSize = (nMax - nMin) / 50;
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 

	if(dUnitSize == 0.00)
		return;

	double dBaseData = GetAt(pClosePriceList, 0);

	int startpoint;
	double SecondData;

	bool UPCheck = true;
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
//	int nRet = 3;
	int nRet = (int)GetCondition(0, pConditionList);
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
	double conversionValue = 0;

	GetSecondDataandStartPoint(pClosePriceList, dUnitSize, startpoint, SecondData);

	if(SecondData >= dBaseData + dUnitSize)
		UPCheck = true;
	else if(SecondData <= dBaseData - dUnitSize)
		UPCheck = false;

	pTOPPacket->AppendData(dBaseData);

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pClosePriceList) return;

	const int nCnt = pClosePriceList->GetCount();

	POSITION posClose = FindPosition(0, *pClosePriceList);
	POSITION posdate = FindPosition(0, *pDateList);

	for(int count = 0 ; count< nCnt; count++) 
	{
		double dClose = GetNext(pClosePriceList, posClose);
		double dDate = GetNext(pDateList, posdate);

		if(dClose >= dBaseData + dUnitSize && UPCheck == TRUE) //상승 중 상승
			WhenUpduringUp(dClose, dDate, dUnitSize, dBaseData, pTOPPacket, pBOTTOMPacket, pDATEPacket);
		else if(dClose >= dBaseData + dUnitSize && UPCheck == FALSE)  //하락중 상승
			WhenUpduringDown(nRet, dClose, dDate, dUnitSize, UPCheck, dBaseData, conversionValue, pTOPPacket, pBOTTOMPacket, pDATEPacket);
		else if(dClose <= dBaseData - dUnitSize && UPCheck == FALSE)  //하락중 하락
			WhenDownduringDown(dClose, dDate, dUnitSize, dBaseData, pTOPPacket, pBOTTOMPacket, pDATEPacket);
		else if(dClose <= dBaseData - dUnitSize && UPCheck == TRUE)  //상승중 하락
			WhenDownduringUp(nRet, dClose, dDate, dUnitSize, UPCheck, dBaseData, conversionValue, pTOPPacket, pBOTTOMPacket, pDATEPacket);
		else
			conversionValue = 0;
	}
}
double CPF::GetUnitValue(const double nMin, const double nMax, CList<double, double>* pConditionList)
{
	if(pConditionList == NULL || pConditionList->GetCount() < 2)
		return (nMax - nMin) / 50.0;

	// 첫번째 조건값 -> value
	// 두번째 조건값 -> 계산 방법(%, 수치)
	double dCondition0 = GetCondition(0, pConditionList);
	double dCondition1 = GetCondition(1, pConditionList);
	if(dCondition1 == 1)	//퍼센트
		return ((nMax - nMin) * (dCondition0 / 100));
	else if(dCondition1 == 2)	//단순 수치
		return dCondition0;
	
	return -1;
}
void CPF::GetSecondDataandStartPoint(CList<double, double>* pClosePriceList,
									 const double dUnitSize,
									 int& startpoint,
									 double& seconddata)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pClosePriceList) return;

	startpoint = 1;

	POSITION pos = FindPosition(0, *pClosePriceList);
	double first = GetNext(pClosePriceList, pos);

	for(int i = 1 ; i < pClosePriceList->GetCount() ; i++) {
		double second = GetNext(pClosePriceList, pos);
		if(first == second || !((second > first + dUnitSize) || (second < first - dUnitSize)) ) 
			startpoint++;
		else 
		{
			seconddata = second;
			break;
		}
	}
}


void CPF::WhenUpduringUp(const double& dClose, const double& dDate, const double dUnitSize,
						 double& dBaseData, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						 CPacket* pDATEPacket)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return;

	// 20080916 김진순 pTOPList에 데이타가 없는 상태인 경우 처리하지 않는다 => 비정상종료하는 문제 처리
	if(pTOPList->GetCount() == 0 )
		return;

	int nUnitSize = (int)(dUnitSize * 10000);
	// 20080916 김진순	0으로 나누거나 나머지를 구하면 비정상 종료한다...
	//if(nUnitSize == 0)
	//	return ;
	if( nUnitSize == 0 )		nUnitSize = 1;

	if(pBOTTOMPacket->GetCount() > 0 )  {//계속 상승중일때는 종가가 계속 변경되고  마지막 종가로 저장.
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->UpdateData(dClose, pBOTTOMList->GetCount() - 1, false);
		else {
			if(data < (nUnitSize / 2 * 0.0001)) 
				pBOTTOMPacket->UpdateData(dClose - data, pBOTTOMList->GetCount() - 1, false);
			else 
				pBOTTOMPacket->UpdateData(dClose + (dUnitSize - data), pBOTTOMList->GetCount() - 1, false);
		}
		pDATEPacket->UpdateData(dDate, pBOTTOMList->GetCount() - 1, false);
	}
	else {
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize)* 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->AppendData(dClose);
		else {
			if(data < (nUnitSize / 2 * 0.0001))
				pBOTTOMPacket->AppendData(dClose - data);
			else
				pBOTTOMPacket->AppendData(dClose + (dUnitSize - data));
		}
		pDATEPacket->AppendData(dDate);
	}
	dBaseData = dClose;
}

void CPF::WhenUpduringDown(const int& nRet, const double& dClose, const double& dDate,
						   const double dUnitSize, bool& UPCheck, double& dBaseData,
						   double& conversionValue, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						   CPacket* pDATEPacket)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return ;
	// 20080916 김진순 pTOPList에 데이타가 없는 상태인 경우 처리하지 않는다 => 비정상종료하는 문제 처리
	if(pTOPList->GetCount() == 0 )
		return;

	int nUnitSize = (int)(dUnitSize * 10000);
	// 20080916 김진순	0으로 나누거나 나머지를 구하면 비정상 종료한다...
	if( nUnitSize == 0 )		nUnitSize = 1;

	conversionValue = (dClose - dBaseData) / dUnitSize;
	// 20080916 김진순 pBOTTOMList에 데이타가 없는 상태인 경우 처리하지 않는다 => 비정상종료하는 문제 처리
//	if(conversionValue >= nRet) //3칸 이상 상승이나 하락이 되었을때.
	if(conversionValue >= nRet && pBOTTOMPacket->GetCount() > 0) //3칸 이상 상승이나 하락이 되었을때.
	{ 
		pTOPPacket->AppendData(pBOTTOMList->GetTail() + dUnitSize);

		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->AppendData(dClose);
		else {
			if(data < (nUnitSize / 2 * 0.0001))
				pBOTTOMPacket->AppendData(dClose - data);
			else 
				pBOTTOMPacket->AppendData(dClose + (dUnitSize - data));	
		}
		pDATEPacket->AppendData(dDate);
		UPCheck = TRUE;
		dBaseData = dClose;
	}
	else
		conversionValue = 0;

}

//만약 unitsize가 5,840 이고. top이 224260 close가 218000이라면
//bottom에는 206740이 들어가야 한다. 
//한칸값에 부족한 만큼의 데이타를 빼야 한다.
//이렇게 맞춰줘야 3개의 O를 그릴수 있다.
//단위만큼 데이타가 늘고 줄어야 한다.
void CPF::WhenDownduringDown(const double& dClose, const double& dDate, const double dUnitSize, 
							 double& dBaseData, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
							 CPacket* pDATEPacket)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return ;

	// 20080916 김진순 pTOPList에 데이타가 없는 상태인 경우 처리하지 않는다 => 비정상종료하는 문제 처리
	if(pTOPList->GetCount() == 0 )
		return;

	int nUnitSize = (int)(dUnitSize * 10000);
	// 20080916 김진순	0으로 나누거나 나머지를 구하면 비정상 종료한다...
	if( nUnitSize == 0 )		nUnitSize = 1;
	if( nUnitSize <= 0) return;
	if(pBOTTOMList->GetCount() > 0 ) { //계속 상승중일때는 종가가 계속 변경되고  마지막 종가로 저장.
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->UpdateData(dClose, pBOTTOMList->GetCount() - 1, false);
		else {
			if(data < (nUnitSize / 2 * 0.0001)) 
				pBOTTOMPacket->UpdateData(dClose + data, pBOTTOMList->GetCount() - 1, false);
			else 
				pBOTTOMPacket->UpdateData(dClose - (dUnitSize - data), pBOTTOMList->GetCount() - 1, false);
		}
		pDATEPacket->UpdateData(dDate, pDATEList->GetCount() - 1, false);
	}
	else {
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->AppendData(dClose);
		else {
			if(data < (nUnitSize / 2 * 0.0001))
				pBOTTOMPacket->AppendData(dClose + data);
			else
				pBOTTOMPacket->AppendData(dClose - (dUnitSize - data));
		}
		pDATEPacket->AppendData(dDate);
	}

	dBaseData = dClose;

}


void CPF::WhenDownduringUp(const int& nRet, const double& dClose, const double& dDate,
						   const double dUnitSize, bool& UPCheck, double& dBaseData,
						   double& conversionValue, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						   CPacket* pDATEPacket)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return;

	// 20080916 김진순 pTOPList에 데이타가 없는 상태인 경우 처리하지 않는다 => 비정상종료하는 문제 처리
	if(pTOPList->GetCount() == 0 )
		return;

	// (2009/9/8 - Seung-Won, Bae) Check Empty Data.
	if( pBOTTOMList->IsEmpty()) return;

	int nUnitSize = (int)(dUnitSize * 10000);
	// 20080916 김진순	0으로 나누거나 나머지를 구하면 비정상 종료한다...
	if( nUnitSize == 0 )		nUnitSize = 1;
	conversionValue = (dBaseData - dClose) / dUnitSize;
	// 20080916 김진순 pBOTTOMList에 데이타가 없는 상태인 경우 처리하지 않는다 => 비정상종료하는 문제 처리
//	if(conversionValue >= nRet) //3칸 이상 상승이나 하락이 되었을때.
	if(conversionValue >= nRet && pBOTTOMPacket->GetCount() > 0) //3칸 이상 상승이나 하락이 되었을때.
	{
		pTOPPacket->AppendData(pBOTTOMList->GetTail() - dUnitSize);
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->AppendData(dClose);
		else {
			if(data < (nUnitSize / 2 * 0.0001))
				pBOTTOMPacket->AppendData(dClose + data);
			else
				pBOTTOMPacket->AppendData(dClose - (dUnitSize - data));
		}
		pDATEPacket->AppendData(dDate);
		UPCheck = FALSE;
		dBaseData = dClose;
	}
	else
		conversionValue = 0;

}


//******************************************************************************************************************
//	Rainbow 
//	  
//******************************************************************************************************************
CRainbow::CRainbow() :
	m_pEndPriceList(NULL),
	m_pEndPricePacket(NULL),
	CIndicator( IMetaTable::EDDC_OCX_RAINBOWCHART)
{
}
CRainbow::~CRainbow()
{
	// (2006/12/12 - Seung-Won, Bae) Release List Chain.
	m_RainbowList.RemoveAll();
}
void CRainbow::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		m_pEndPricePacket = GetPacket(calculateData.GetPoint(), chartFormulateData);
		m_pEndPriceList = GetnumericDataList(m_pEndPricePacket);
	}
	if(!IsValidPacket(m_pEndPriceList))
		return;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pIndicatorInfo) return;

	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dFirstCondition  = GetCondition(0, pConditionList);
	double dIntervalofMV  = GetCondition(1, pConditionList);
	double dPacketCount = GetCondition(2, pConditionList);

	if( !GetSubGraphPacket(chartFormulateData.GetCalculateType(), dPacketCount, pPacketList, pIndicatorInfo)) return;
	if( !SetGraphPacketList(chartFormulateData, pGraphCalculateData)) return;
	
	if(IsIndicatorCalculated(chartFormulateData.GetCalculateType()))
		return;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pEndPricePacket) return;

	GetRainbow(calculateData.GetPoint(), dFirstCondition, dIntervalofMV, dPacketCount, 
			   m_pEndPricePacket->GetType(false), m_pEndPriceList, pPacketList, chartFormulateData.GetChartMode());
}

//이미 계산이 되어 있음.
//object에서 build를 다시 했을때.가지고 있던 packet의 주소를 모두 잃어버리니까..
//다시 세팅해 줌. 그리고, 계산은 다시 할 필요 없다.
bool CRainbow::IsIndicatorCalculated(const CCalculateBaseData::CALCULATETYPE calculatetype)
{
	if(calculatetype == CCalculateBaseData::TRDATA_CAL)
	{
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( m_RainbowList.IsEmpty()) return FALSE;
		CPacket *pPacket = m_RainbowList.GetHead();
		if( !pPacket) return FALSE;

		if( pPacket->GetCount() > 0)
			return true;
	}
	return false;
}

bool CRainbow::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, const double dPacketCount, CPacketList* pPacketList,
								 CIndicatorInfo* pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		//20개의 그물이 있다가 5개로 줄어들수도 있으니까 리스트에 있는 목록 모두 삭제
		m_RainbowList.RemoveAll();
		for(int count = 0 ; count < dPacketCount ; count++) 
		{
			CString strSubgraphName;
			strSubgraphName.Format("%s%s%d", strSubGraphPacketNameHeader, _MTEXT( C2_RAINBOW_CHART), count + 1);//"그물차트"
			
			CPacket* pRainbow = pPacketList->GetBaseData( strSubgraphName, false);
			ClearPacketData(pRainbow);
			m_RainbowList.AddTail(pRainbow);
		}
	}
	return (m_RainbowList.GetCount() > 0);
}

//실제 subgraphname은 안쓴다. 
bool CRainbow::SetGraphPacketList(const CChartFormulateData& chartFormulateData,
								  CGraphCalculateData* pGraphCalculateData)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL) 
		{
			POSITION pos = m_RainbowList.GetHeadPosition();
			while(pos)
				pSubGraphList->AddTail2( m_RainbowList.GetNext(pos));
		}
		return (pSubGraphList->GetCount() > 0);
	}
	return false;
}

void CRainbow::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									    CGraphCalculateData* pGraphCalculateData,
										CIndicatorInfo* pIndicatorInfo)
{
	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

void CRainbow::GetRainbow(const CPoint& point,
						  const double dFirstCondition, 
						  const double dIntervalofMV, 
						  const double dPacketCount, 
						  const CString& strType,
						  CList<double, double>* pEndPriceList, 
						  CPacketList* pPacketList,
						  CChartInfo::CHART_MODE p_eChartMode)
{
	POSITION pos = m_RainbowList.GetHeadPosition();
	for(int count = 0 ; count < dPacketCount ; count++) 
	{
		if(pos == NULL)
			return ;
		double dCondition = dFirstCondition + (count * dIntervalofMV);

		CPacket* pRainbowPacket = m_RainbowList.GetNext(pos);

		// (2009/9/19 - Seung-Won, Bae) for FX/WorldOn Index Digit.
		CString strValueType( TYPE_TWODECIMAL);
		if( CChartInfo::HTS != p_eChartMode)
			strValueType = strType;

		SetPacketTypes( pRainbowPacket, strValueType, strType, (int)dCondition - 1);
		GetSimpleMA(point, dCondition, pEndPriceList, pRainbowPacket, TRUE);
	}
}


//******************************************************************************************************************
//	CRelative_Strength
//	예) 1999.10.10 ~ 2000.01.01
// 10.10 의 데이타가 90이였다.
// 그 데이타를 100으로 놓고 모든 데이타에 0.9로 나눈다.  
//////
// 금일종가 / (첫번째 data / dCondition).
//******************************************************************************************************************

CRelative_Strength::CRelative_Strength() : 
	m_pRS(NULL),
	m_pEndPriceList(NULL),
	m_pEndPricePacket(NULL),
	CIndicator( IMetaTable::EDDC_OCX_COMPARATIVECHART)
{
}

void CRelative_Strength::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;

	CString strPacketNames = CDataConversion::GetStringData(chartFormulateData.GetPacketNames(), ",");
	if(strPacketNames.IsEmpty())
		strPacketNames = chartFormulateData.GetPacketNames();

	// (2006/12/11 - Seung-Won, Bae) With Multiple-Same Indciator.
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		m_pEndPricePacket = pPacketList->GetBaseData(strPacketNames);
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !m_pEndPricePacket) return;
		m_pEndPriceList = m_pEndPricePacket->GetnumericDataList();

		CString strSubGraphName = CDataConversion::GetStringData(chartFormulateData.GetSubGraphNames(), ",");
		if( !GetSubGraphPacket(chartFormulateData.GetCalculateType(), strPacketNames + strSubGraphName, pPacketList, pIndicatorInfo)) return;
		if( !SetGraphPacketList(strSubGraphName, pPacketList, pGraphCalculateData, pIndicatorInfo))return;
	}
	if( !IsValidPacket(m_pEndPriceList)) return;

	if(IsIndicatorCalculated(chartFormulateData.GetCalculateType(), m_pRS))
		return;

	//sy test
	double dCond = 0.0;
	if(!GetCondition(pIndicatorInfo, dCond))
		return;

	SetPacketTypes(dCond);
	GetRSC( calculateData.GetPoint(), dCond, m_pEndPricePacket, m_pRS);

	//double dMin = 0, dMax = 0;
	//m_pEndPricePacket->GetMinMax(0, m_pEndPricePacket->GetCount() - 1, dMin, dMax);
	//GetRSC(calculateData.GetPoint(), dCond, nMin, nMax, m_pEndPriceList, m_pRS);	
}

void CRelative_Strength::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
											CGraphCalculateData* pGraphCalculateData,
											CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pRS);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

bool CRelative_Strength::SetGraphPacketList(const CString& strSubGraphName,
											CPacketList* pPacketList,
											CGraphCalculateData* pGraphCalculateData,
											CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	if( !posSubGraph) return false;

	CSubGraphPacketList *pSubGraphList = pGraphCalculateData->GetNext( posSubGraph);
	if( pSubGraphList == NULL) return false;

	CPacket *pPacket = pSubGraphList->GetHeadPacket();
	if( !pPacket && m_pRS) pSubGraphList->AddTail2( m_pRS);
	return true;
}

bool CRelative_Strength::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, const CString& strSubName, CPacketList* pPacketList,
										   CIndicatorInfo* pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		m_pRS = pPacketList->GetBaseData( strSubGraphPacketNameHeader + strSubName, false);
		ClearPacketData( m_pRS);
	}
// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
//	else if(calculatetype == CCalculateBaseData::REALDATA_CAL)
//	{
//		m_pRS = pPacketList->GetBaseData(strSubName, true);
//	}
	return ( m_pRS != NULL);
}

void CRelative_Strength::GetRSC(const CPoint &point, const double dCondition,
								const double nMin, const double nMax,
								CList<double, double>* pClosePriceList,
								CPacket* pRSC)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pClosePriceList || !pRSC) return;

	int nCnt = pClosePriceList->GetCount();
	if(nMin == 0 && nMax == 0)
	{ //data가 모두 0일때 모든 data를 100%로.
		for(int count = point.x;  count < nCnt; count++) 
		{
			//sy 2006.06.01.
			//pRSC->AppendData(100);
			pRSC->AppendData(dCondition);
		}
		return;
	}

	int startpoint = point.x;
	if(point.x == 0) 
	{
		startpoint += 1;
		//sy 2006.06.01.
		//pRSC->AppendData(100);
		pRSC->AppendData(dCondition);
	}
	//nami 7.30
	double dFirstData = GetAt(pClosePriceList, 0);

	POSITION posStart = FindPosition(startpoint, *pClosePriceList);
	for(int count = startpoint;  count < nCnt; count++) {
		if(posStart == NULL) return ;
		if(dFirstData == 0)
			dFirstData = 1;

		SetData(point, count, (GetNext(pClosePriceList, posStart) / dFirstData ) * dCondition, pRSC);
	}
}

//sy test
bool CRelative_Strength::GetCondition(const CIndicatorInfo* pIndicatorInfo, double& dCond) const
{
	if(pIndicatorInfo == NULL)
		return false;

	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	if(pConditionList == NULL)
		return false;

	dCond = pConditionList->GetHead();
	return true;
}

void CRelative_Strength::GetRSC(const CPoint& point, const double& dCond, const CPacket* pClosePrice, CPacket* pRSC)
{
	if(pClosePrice == NULL || pRSC == NULL)
		return;

	CList<double, double>* pClosePriceDataList = NULL;
	int nCount = 0;
	double dMin = 0.0, dMax = 0.0, dFirstData;
	if(!GetEnvironmentDataForCalRSC(pClosePrice, pClosePriceDataList, nCount, dMin, dMax, dFirstData))
		return;

	if(InitializeRSC(pRSC, point.x, nCount, dCond, dMin, dMax))
		return;

	// 상대비교계산
	POSITION pos = CIndicator::FindPosition(point.x, *pClosePriceDataList);
	for(int i = point.x; i < nCount; i++)
	{
		if(pos == NULL)
			break;

		double dRSCData = GetRSCData(dFirstData, pClosePriceDataList->GetNext(pos), dCond);
		CIndicator::SetData(point, i, dRSCData, pRSC);
	}
}

double CRelative_Strength::GetRSCData(const double& dFirstData, const double& dPriceData, const double& dCond) const
{
	double dRSCData = (dPriceData - dFirstData)/dFirstData;
	// dCond : 기준 시작 값
	dRSCData = dCond + (dRSCData * 100);
	return dRSCData;
}

bool CRelative_Strength::GetEnvironmentDataForCalRSC(const CPacket* pClosePrice, 
		CList<double, double>*& pClosePriceDataList, int& nCount, double& dMin, double& dMax, double& dFirstData) const
{
	if(pClosePrice == NULL)
		return false;

	pClosePriceDataList = const_cast<CPacket*>(pClosePrice)->GetnumericDataList();
	if(pClosePriceDataList == NULL)
		return false;

	nCount = pClosePriceDataList->GetCount();
	if(nCount <= 0)
		return false;

	const_cast<CPacket*>(pClosePrice)->GetMinMax(0, (nCount - 1), dMin, dMax);
	dFirstData = pClosePriceDataList->GetHead();
	if(dFirstData == 0.0)
		dFirstData = 1.0;

	return true;
}

void CRelative_Strength::SetPacketTypes(const double& dCond)
{
	if( dCond == 100.0)
	{
		CIndicator::SetPacketTypes( m_pRS, TYPE_TWODECIMAL, TYPE_INTERGER_PERCENT, 0);
	}
	else
	{
		CIndicator::SetPacketTypes( m_pRS, TYPE_TWODECIMAL, TYPE_TWODECIMAL, 0);
	}
}

bool CRelative_Strength::InitializeRSC(CPacket* pRSC, const int nStartIndex, const int nCount, const double& dCond, const double& dMin, const double& dMax)
{
	if(pRSC == NULL || dMin != 0.0 || dMax != 0.0)
		return false;

	bool bResult = false;
	//data가 모두 0일때 : 모든 data를 조건값으로 넣는다 (0 or 100%)
	for(int i = nStartIndex; i < nCount; i++)
	{
		pRSC->AppendData(dCond);
		bResult = true;
	}
	
	return bResult;
}
//sy end

/******************************************************************************************************************
* Chart Name : CRenKoChart 
* Date       : 2005.06.23. 	
* Made By    : Yoo. Je Sun
* 지표설명   : 일정량을 초과하는 가격변화(최소전환수준, 벽돌크기)의 발생시 벽돌(Bricks)을 그림
*              벽돌의 크기는 동일함
*******************************************************************************************************************/
CRenKoChart::CRenKoChart() : 
	m_pClosePrice(NULL), 
	m_pDate(NULL), 
	m_pRenKoHighPrice(NULL),
	m_pRenKoLowPrice(NULL),
	m_pRenKoDate(NULL),
	CIndicator( IMetaTable::EDDC_OCX_RENKOCHART)
{
}

void CRenKoChart::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(!pPacketList || !pIndicatorInfo)
		return;
	
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		//TR 계산일때만 기존 한칸값을 초기화 한다.
		// 조건값 사용자 지정 가능 - ojtaso (20070913)
		//	(2006.08.14, 안승환) 한칸의 값및 칸 전환수를 수정할수 있도록 함 
		CList<double, double>* pDrawConditionList = pIndicatorInfo->GetDrawConditionList();
		if( pDrawConditionList)
		{
			//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
			POSITION pos = pDrawConditionList->GetHeadPosition();
			if( !pos) return;
			double nIndex = pDrawConditionList->GetAt(pos);
			// (2007/6/19 - Seung-Won, Bae) Support Only Draw Condition of P&F for User Setting.
			if( 0 == nIndex) pIndicatorInfo->SetCommonCalConditionData(0, 0);
		}
		//자료일자와 종가패킷가져오기 (IndiCond.cfg를 참조)
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pClosePrice = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDate = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
	}

	// 데이타값이 유효한지를 검사한다.
	if(!IsValidPacket(m_pClosePrice) || !IsValidPacket(m_pDate))
		return;

	CList<double, double>* pClosePriceList = CIndicator::GetnumericDataList(m_pClosePrice); // 종가 리스트
	CList<double, double>* pDateList = CIndicator::GetnumericDataList(m_pDate); // 자료일자 리스트
	// 데이타값이 유효한지를 검사한다.
	if(!IsValidPacket(pClosePriceList) || !IsValidPacket(pDateList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	double dBlockCondition = GetBrickSize( pIndicatorInfo, m_pClosePrice, pPacketList, chartFormulateData.GetChartMode());
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		//TR 계산일때만 info에 한칸값을 변경한다.
		pIndicatorInfo->SetCommonCalConditionData(0, dBlockCondition);
		
		// packet type 셋팅.
		CString strPriceType = m_pClosePrice->GetType(false);
		CString strDateType = m_pDate->GetType(false);
		SetPacketTypes(m_pRenKoHighPrice, strPriceType, strPriceType, 0);
		SetPacketTypes(m_pRenKoLowPrice, strPriceType, strPriceType, 0);
		SetPacketTypes(m_pRenKoDate, strDateType, strDateType, 0);
	}

	GetRenKoChart(calculateData.GetPoint(), dBlockCondition, pClosePriceList, pDateList);
}

bool CRenKoChart::SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo)
{
	// NULL Pointer Exception을 확인하여, 제외한다.
	if(!pGraphCalculateData)
		return false;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	
	// 멤버 변수를 Clear
	ClearPacketData(m_pRenKoHighPrice);
	ClearPacketData(m_pRenKoLowPrice);
	ClearPacketData(m_pRenKoDate);

	return (m_pRenKoHighPrice != NULL && m_pRenKoLowPrice != NULL && m_pRenKoDate != NULL);
}

bool CRenKoChart::InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;
	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;
	if(!pSubGraphList)
		return false;

	m_pRenKoHighPrice	= pPacketList->GetBaseData( _MTEXT( C0_RENKO_CHART_OPEN),		false);
	m_pRenKoLowPrice	= pPacketList->GetBaseData( _MTEXT( C0_RENKO_CHART_CLOSE),		false);
	m_pRenKoDate		= pPacketList->GetBaseData( _MTEXT( C0_RENKO_CHART_DATETIME),	false);

	pSubGraphList->AddTail2( m_pRenKoHighPrice);
	pSubGraphList->AddTail2( m_pRenKoLowPrice);
	pSubGraphList->AddTail2( m_pRenKoDate);
	return (m_pRenKoHighPrice != NULL || m_pRenKoLowPrice != NULL || m_pRenKoDate != NULL);
}
					
//나중에 실시간 처리 확인
//실시간 들어올때도 첨부터 다시 계산해야 되니까...data를 전부 지워준다.
bool CRenKoChart::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList)
{
	if(!pPacketList)
		return false;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL)
	{
		m_pRenKoHighPrice = pPacketList->GetBaseData(	_MTEXT( C0_RENKO_CHART_OPEN),		false);
		m_pRenKoLowPrice = pPacketList->GetBaseData(	_MTEXT( C0_RENKO_CHART_CLOSE),		false);
		m_pRenKoDate = pPacketList->GetBaseData(		_MTEXT( C0_RENKO_CHART_DATETIME),	false);

		ClearPacketData(m_pRenKoHighPrice);
		ClearPacketData(m_pRenKoLowPrice);
		ClearPacketData(m_pRenKoDate);
	}
	else if(calculatetype == CCalculateBaseData::REALDATA_CAL)
	{
		ClearPacketData(m_pRenKoLowPrice);
		ClearPacketData(m_pRenKoDate);
	}
	return (m_pRenKoHighPrice != NULL && m_pRenKoLowPrice != NULL && m_pRenKoDate != NULL);
}

void CRenKoChart::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
										 CGraphCalculateData* pGraphCalculateData,
										 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pRenKoHighPrice);
	ClearPacketData(m_pRenKoLowPrice);
	ClearPacketData(m_pRenKoDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

void CRenKoChart::GetRenKoChart(const CPoint &point, double dBrickCondition,
								CList<double, double>* pClosePriceList,
								CList<double, double>* pDateList)
{
	// 종가 패킷이 없는경우 리턴한다
	if( !pClosePriceList )
		return;

	UINT nIndex = 0;	// RenKoPacket에 사용되는 Index.
	POSITION posDate		= pDateList->GetHeadPosition();
	POSITION posClosePrice	= pClosePriceList->GetHeadPosition();
	
	int		i=0, nListCount=0;				// nListCount-종가 리스트 갯수
	double	dBrickType = 0;					// BrickType (0: Down, 1: Up)
	double	dClosePrice = 0;				// 종가 데이터
	double	dHighPrice = 0, dLowPrice = 0;	// RenKoPacket의 High/Low Price
	double	dDate = 0;						// 종가 자료일자
	double	dBrickHigh = 0, dBrickLow = 0;	// 벽돌의 High/Low Price

	nListCount = pClosePriceList->GetCount();	// 종가 데이터 갯수
	
	if(nListCount <= 1)	// 종가 데이터가 1이하인 경우 계산 필요없음
		return;

	dClosePrice = pClosePriceList->GetNext(posClosePrice);	// 첫 종가
	dDate = pDateList->GetNext(posDate);				  // 첫 일자 데이터
	dHighPrice = dLowPrice = 0;				// 변수값 세팅
	dBrickHigh = dBrickLow = dClosePrice;

	CList<double, double>* pBrickTypeList = NULL;
	pBrickTypeList = m_pRenKoHighPrice->GetTempList();

	for(i=0; i < nListCount -1; i++)
	{
		while( dClosePrice >= dBrickHigh + dBrickCondition )
		{
			/* Up Type Brick 생성 */
			dBrickType = 1;
			dHighPrice = dBrickHigh + dBrickCondition;
			dLowPrice = dBrickHigh;

			// A HighPrice and a BrickType add to m_pRenKoHighPrice
			SetData(point, nIndex, dHighPrice, m_pRenKoHighPrice);
			m_pRenKoHighPrice->SetTempListData(nIndex, dBrickType, true);

			// LowPrice add to m_pRenKoLowPrice
			SetData(point, nIndex, dLowPrice, m_pRenKoLowPrice);

			// dDate add to m_pRenKoDate
			SetData(point, nIndex, dDate, m_pRenKoDate);
			nIndex++;
			
			dBrickHigh = dHighPrice; /* ReSetting BrickData */
			dBrickLow = dLowPrice;
		}
		while( dClosePrice <= dBrickLow - dBrickCondition )
		{
			/* Down Type Brick 생성 */
			dBrickType = 0;
			dHighPrice = dBrickLow;
			dLowPrice = dBrickLow - dBrickCondition;

			// HighPrice add to m_pRenKoHighPrice 
			SetData(point, nIndex, dHighPrice, m_pRenKoHighPrice);
			m_pRenKoHighPrice->SetTempListData(nIndex, dBrickType, true);

			// LowPrice add to m_pRenKoLowPrice
			SetData(point, nIndex, dLowPrice, m_pRenKoLowPrice);

			// dDate add to m_pRenKoDate
			SetData(point, nIndex, dDate, m_pRenKoDate);
			nIndex++;
			// ReSetting BrickData
			dBrickHigh = dHighPrice;
			dBrickLow = dLowPrice;
		}
		dClosePrice = pClosePriceList->GetNext(posClosePrice);	// Next 종가
		dDate = pDateList->GetNext(posDate);				  // Next 자료일자
	}
}

// 벽돌의 사이즈를 종목별로 가져오는 함수
// -> 조건 설정이 0인 경우 강제로 50으로 세팅. (수정 필요)
//	  기본 벽돌의 크기
//	  + 주식 : 호가단위 * 10
//	  + 업종/선물/옵션 : 5-Point
double CRenKoChart::GetBrickSize( CIndicatorInfo* pIndicatorInfo, CPacket* pClosePacket, CPacketList* pPacketList, CChartInfo::CHART_MODE p_eChartMode) const
{
	// 조건설정
	double dBrickSize = GetBrickSize(pIndicatorInfo);
	if(dBrickSize > 0)
		return dBrickSize;

	// (2008/10/27 - Seung-Won, Bae) Do not use asked price in world-on.
	if( CChartInfo::WORLD_ON != p_eChartMode)
	{
		CString strHoga;
		if (pPacketList->GetHelpMessageItem("HOGAUNIT", strHoga))
		{
			double dMin = 0.00, dMax = 0.00;
			pClosePacket->GetMinMax(0, pClosePacket->GetCount() - 1, dMin, dMax);
			dBrickSize = (dMax - dMin) * 0.02;

			double dTickValue = atoi(strHoga);
			if (dTickValue != 1)
				return CalculateBrickSizeUsingTickValue(dBrickSize, dTickValue);
		}

		// 호가설정
		dBrickSize = GetBrickSize(pClosePacket);
		if(dBrickSize > 0)
			return dBrickSize;
	}

	// 최고가 최저가 가격폭의 2%.
	dBrickSize = GetBrickSize_Price(pClosePacket, pPacketList, p_eChartMode);

	// (2008/10/27 - Seung-Won, Bae) Do not use asked price in world-on.
	if( CChartInfo::WORLD_ON == p_eChartMode) return dBrickSize;

	return (dBrickSize > 0 ? dBrickSize: 1);
}

double CRenKoChart::GetBrickSize(CIndicatorInfo* pIndicatorInfo) const
{
	// 조건설정
	if(pIndicatorInfo == NULL)
		return 0;

	return GetCondition(0, pIndicatorInfo->GetCommonCalConditionList());
}

double CRenKoChart::GetBrickSize(CPacket* pClosePacket) const
{
	// 호가설정
	if(pClosePacket == NULL)
		return 0;

	switch(pClosePacket->GetCommodityBondName()) 
	{
	case CPacketType::NONAME:
	case CPacketType::STOCK:	//주식
		return GetBrickSize_Stock(pClosePacket);
	case CPacketType::INDUSTRY:	//업종
	case CPacketType::FUTURES:	//선물
	case CPacketType::OPTION:	//옵션
		return 5;
	}

	return 0;
}

double CRenKoChart::GetBrickSize_Stock(CPacket* pClosePacket) const
{
	// 호가설정 -> 주식
	if(pClosePacket == NULL)
		return 0;

	CList<double, double>* pCloseDataList = pClosePacket->GetnumericDataList();
	// 조건설정이 없는 경우 -> "마지막 종가"로 임의 지정
	if(pCloseDataList == NULL || pCloseDataList->GetTailPosition() == NULL)
		return 0;

	double dBrickSize = 0;
	// 마지막 종가 호가 반영에 대한 설정
	double dClosePrice = pCloseDataList->GetTail();
	if(dClosePrice >= 500000)		// 50만원 이상
		dBrickSize = 1000;
	else if(dClosePrice >= 200000)	// 20만원 이상
		dBrickSize = 500;
	else if(dClosePrice >= 50000)	// 5만원 이상
		dBrickSize = 100;
	else if(dClosePrice >= 20000)	// 2만원 이상
		dBrickSize = 50;
	else if(dClosePrice >= 5000)	// 5천원 이상
		dBrickSize = 10;
	else							// 5천원 미만
		dBrickSize = 5;
	
	return (dBrickSize * 10);
}

double CRenKoChart::GetBrickSize_Price(CPacket* pClosePacket, CPacketList* pPacketList, CChartInfo::CHART_MODE p_eChartMode) const
{
	if( !pClosePacket || !pPacketList) return 0;

	// 최고가 최저가 가격폭의 2%.
	double dMin = 0.00, dMax = 0.00;
	pClosePacket->GetMinMax(0, pClosePacket->GetCount() - 1, dMin, dMax);
	double dBrickSize = (dMax - dMin) * 0.02;

	// (2008/11/2 - Seung-Won, Bae) Use HBPRICE.
	CString strTickValue;
	if( pPacketList->GetHelpMessageItem( "HBPRICE", strTickValue))
	{
		double dTickValue = atof( strTickValue);
		if( dTickValue < dBrickSize) dTickValue = dBrickSize;
		return dTickValue;
	}

	// (2008/10/27 - Seung-Won, Bae) Do not use asked price in world-on.
	if( CChartInfo::WORLD_ON != p_eChartMode)
	{
		if(dBrickSize > 100000) // 10만원 단위
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 5);
		else if(dBrickSize > 10000) // 1만원 단위
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 4);
		else if(dBrickSize > 1000) // 1천원 단위
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 3);
		else if(dBrickSize > 100) // 100원 단위
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 2);
		else if(dBrickSize > 10) // 10원 단위
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 1);
		else if(dBrickSize > 5) // 5원 단위
			dBrickSize = 5;
		return (dBrickSize > 1 ? dBrickSize : 1);
	}
	return dBrickSize;
}

double CRenKoChart::CalculateBrickSizeUsingTickValue(double brickSize, double tickValue) const
{
	if (tickValue > 0) {
		brickSize = int(brickSize / tickValue + 0.99) * tickValue;
		if (brickSize < tickValue) {
			brickSize = tickValue;
		}
	}
	return brickSize;
}

///******************************************************************************************************************
//	CThreeLineBreak
//	  
///******************************************************************************************************************
CThreeLineBreak::CThreeLineBreak() : 
	m_pTLBTop(NULL), 
	m_pTLBBottom(NULL), 
	m_pTLBDate(NULL), 
	m_pEndPriceList(NULL),
	m_pDateList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_THREELINEBRACKCHART)
{
}

void CThreeLineBreak::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;

//	(2006.09.07,안승환) 삼선전환도 칸 전환수를 수정할수 있도록 함 
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
//	(2006.09.07,안승환) 삼선전환도 칸 전환수를 수정할수 있도록 함 
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		CPacket *pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pEndPriceList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pDateList = pPacket->GetnumericDataList();
	}
	if(!IsValidPacket(m_pEndPriceList) || !IsValidPacket(m_pDateList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

//	(2006.09.07,안승환) 삼선전환도 칸 전환수를 수정할수 있도록 함 
//	GetThreeLineBreak(calculateData.GetPoint(), m_pEndPriceList, m_pDateList, 
//					  m_pTLBTop, m_pTLBBottom, m_pTLBDate);	
	GetThreeLineBreak(calculateData.GetPoint(), m_pEndPriceList, m_pDateList, 
					  m_pTLBTop, m_pTLBBottom, m_pTLBDate,pConditionList);	
//	(2006.09.07,안승환) 삼선전환도 칸 전환수를 수정할수 있도록 함 

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	CPacket *pPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));//"종가"
	if( !pPacket) return;
	CString strEndPacketType = pPacket->GetType(false);
	pPacket = pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return;
	CString strDatePacketType = pPacket->GetType(false);

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pTLBTop || !m_pTLBBottom || !m_pTLBDate) return;

	SetPacketTypes(m_pTLBTop, strEndPacketType, strEndPacketType, 0);
	SetPacketTypes(m_pTLBBottom, strEndPacketType, strEndPacketType, 0);
	SetPacketTypes(m_pTLBDate, strDatePacketType, strDatePacketType, 0);
}
	
//특수형
bool CThreeLineBreak::SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	return (m_pTLBTop != NULL && m_pTLBBottom != NULL && m_pTLBDate != NULL);
}

//특수형
bool CThreeLineBreak::InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;

	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;

	m_pTLBTop		= pPacketList->GetBaseData( _MTEXT( C0_THREE_LINE_BREAK_OPEN),		false);
	m_pTLBBottom	= pPacketList->GetBaseData( _MTEXT( C0_THREE_LINE_BREAK_CLOSE),		false);
	m_pTLBDate		= pPacketList->GetBaseData( _MTEXT( C0_THREE_LINE_BREAK_DATE_TIME),	false);

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( m_pTLBTop);
	pSubGraphList->AddTail2( m_pTLBBottom);
	pSubGraphList->AddTail2( m_pTLBDate);
	return (m_pTLBTop != NULL || m_pTLBBottom != NULL || m_pTLBDate != NULL);
}
					
//나중에 실시간 처리 확인
//실시간 들어올때도 첨부터 다시 계산해야 되니까...data를 전부 지워준다.
bool CThreeLineBreak::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		m_pTLBTop = pPacketList->GetBaseData(		_MTEXT( C0_THREE_LINE_BREAK_OPEN),		false);
		m_pTLBBottom = pPacketList->GetBaseData(	_MTEXT( C0_THREE_LINE_BREAK_CLOSE),		false);
		m_pTLBDate = pPacketList->GetBaseData(		_MTEXT( C0_THREE_LINE_BREAK_DATE_TIME),	false);

		ClearPacketData(m_pTLBTop);
		ClearPacketData(m_pTLBBottom);
		ClearPacketData(m_pTLBDate);
	}
	else if(calculatetype == CCalculateBaseData::REALDATA_CAL)
	{
		ClearPacketData(m_pTLBTop);
		ClearPacketData(m_pTLBBottom);
		ClearPacketData(m_pTLBDate);
	}
	return (m_pTLBTop != NULL && m_pTLBBottom != NULL  && m_pTLBDate != NULL );
}

void CThreeLineBreak::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
										 CGraphCalculateData* pGraphCalculateData,
										 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pTLBTop);
	ClearPacketData(m_pTLBBottom);
	ClearPacketData(m_pTLBDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

// (2004.04.06, 배승원, 분석) 삼선 전환 Data Packet의 의미를 이해할 수 있도록 변수명을 조정한다.
//		(TopPacket은 삼선전환선 시가 Packet, BottomPacket은 종가 Packet)
// (2004.04.07, 배승원, 분석) 삼선전환도
//		1. 추세의 방향인 경우, 지속 해서 봉을 형성시키고,
//			추세의 반전인 경우, 이전 3개 봉을 넘어서는 경우에만 반전봉으로 인정한다.
//		2. 따라서 항상 이전 4개의 삼선봉 종가를 관리하여 반전 목표값을 확인한다.
//		3. 특히 반전이 발생하는 경우, 관리중인 이전 4개 삼선봉 종가를 반전 이전 추세종가로 모두 설정하여,
//			재 반전시의 목표치로 한다.
//		4. 또한 반전시의 4개 삼선봉 종가 초기화후, 반전의 종가를 설정하기에 앞서,
//			반전 이전의 '이전' 종가를 두어, 이번 반전의 시가를 추가한다.
//			이것은 향후, 2회 추세 확장 진행시 다음 반전 목표치가 되기 때문이다.
void CThreeLineBreak::GetThreeLineBreak(const CPoint &point,
										CList<double, double>* pClosePriceList,
										CList<double, double>* pDateList,
										CPacket* pStartPacket,
										CPacket* pEndPacket,
										CPacket* pDATEPacket,
										CList<double, double>* pConditionList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pClosePriceList) return;

//	(2006.09.07,안승환) 삼선전환도 칸 전환수를 수정할수 있도록 함 
	CList<double, double> m_DataList;
	if( pConditionList->IsEmpty()) return;
	int nCount = (int)pConditionList->GetHead();
	double dData;
	if(m_DataList.GetCount() > 0)
		m_DataList.RemoveAll();
	for(int i =0; i < nCount+1; i++)
	{
		dData = GetAt(pClosePriceList, 0);
		m_DataList.AddHead(dData);
	}
	
	POSITION posDate		= pDateList->GetHeadPosition();
	POSITION posClosePrice	= pClosePriceList->GetHeadPosition();
	while( posDate && posClosePrice)
	{
		double dClose	= pClosePriceList->GetNext( posClosePrice);
		double dDate	= pDateList->GetNext( posDate);

		if( GetAt(&m_DataList,0) == dClose) continue;

		if( ( dClose - GetAt(&m_DataList,0)) * ( GetAt(&m_DataList,0) - GetAt(&m_DataList,1)) >= 0) ;

		else if( 0 < ( GetAt(&m_DataList,nCount) - dClose) * ( GetAt(&m_DataList,0) - GetAt(&m_DataList,nCount)))
		{
			
			for(int j = nCount; j>=2; j--)
			{
				m_DataList.SetAt(FindPosition(j,m_DataList),GetAt(&m_DataList,0));
			}
			m_DataList.SetAt(FindPosition(0,m_DataList),GetAt(&m_DataList,1));			
			m_DataList.SetAt(FindPosition(1,m_DataList),GetAt(&m_DataList,2));
		}
		else continue;

		pStartPacket->AppendData( GetAt(&m_DataList,0));
		pEndPacket->AppendData(	dClose);
		pDATEPacket->AppendData(dDate);

		// 6. 이전 삼선봉 갯수를 순연시킨다.
		for(int k = nCount; k>=1; k--)
		{
			m_DataList.SetAt(FindPosition(k,m_DataList),GetAt(&m_DataList,k-1));
		}
		m_DataList.SetAt(FindPosition(0,m_DataList),dClose);
		
	}
//	(2006.09.07,안승환) 삼선전환도 칸 전환수를 수정할수 있도록 함 
	
	// 1. 첫 삼선전환의 시작시 4개 이전 종가를 첫 가격으로 초기화한다.
/*	double data[ 4];
	data[ 0] = data[ 1] = data[ 2] = data[ 3] = GetAt(pClosePriceList, 0);

	// (2004.12.15, 배승원) 처리 속도를 빠르게 하기 위하여 Index 방식에서 List Link 방식을 따른다.
	POSITION posDate		= pDateList->GetHeadPosition();
	POSITION posClosePrice	= pClosePriceList->GetHeadPosition();
	while( posDate && posClosePrice)
	{
		double dClose	= pClosePriceList->GetNext( posClosePrice);
		double dDate	= pDateList->GetNext( posDate);

		// 2. 먼저 추세에 영향을 주지 않는 경우를 제외한다.
		if( data[ 0] == dClose) continue;

		// 3. 추세의 확장이나 반전이 아닌 경우를 제외한다.
		// 3.1 추세의 확장의 경우를 확인한다.
		//		(처음 추세 형성시에는 'data[ 0] - data[ 1]'가 0이고, 무조건 추세의 확장으로 감안한다.)
		if( ( dClose - data[ 0]) * ( data[ 0] - data[ 1]) >= 0);
		// 3.2 추세의 반전의 경우를 확인한다.
		//		(추세의 반전은 이전 4번째 삼선봉 종가를 갱신하는 경우여야 한다. 이전 삼선 돌파 !
		//		 즉 이전 4번째 삼선봉 종가가, 마지막 삼선봉 종가와 이번 가격 사이에 놓여야 한다.)
		else if( 0 < ( data[ 3] - dClose) * ( data[ 0] - data[ 3]))
		{
			// 3.3 반전시에는 관리중인 이전 삼선봉 종가를 새로운 추세의 방향으로 재정리 한다.
			//		관리중인 이전 삼선봉을 마지막 삼선봉 종가로 초기화하고,
			//		마지막 삼선봉 종가로는 그 '이전' 종가가 오도록한다.
			//		(다음 반전 목표는 바로전 삼선봉 종가이며,
			//			향후 추세가 반전되지 않고 계속 2회 확장시, 그 '이전' 종가가 다음 반전 목표가 되기 때문이다.)
			data[ 3] = data[ 2] = data[ 0];
			data[ 0] = data[ 1];
			data[ 1] = data[ 2];
		}
		else continue;

		// 5. 삼선봉을 등록시킨다.
		pStartPacket->AppendData(	data[0]);
		pEndPacket->AppendData(		dClose);
		pDATEPacket->AppendData(	dDate);

		// 6. 이전 삼선봉 4개를 순연시킨다.
		data[3] = data[2];
		data[2] = data[1];
		data[1] = data[0];
		data[0] = dClose;
	}
*/
}


//******************************************************************************************************************
// 지표 name : Zig Zag
// -전일과 오늘을 비교하여 전일의 수치보다 X만큼 가격이 높(낮)거나 X%만큼 높(낮)으면 
//  지그재그 포인트가 될 수 있다.
// -첫 데이터와 끝 데이터는 임시로 지그재그 포인트가 된다. 
// -상승 포인트 다음에 하락포인트가 나왔을 경우 상승 포인트는 지그재그 포인트가 되고 
//  하락 포인트는 기다린다.
// -상승이 여러개 나왔다가 하락이 나왔을 경우 하락 바로 앞에 있는 상승만 남기고 모두 
//  제거한다.반대로 하락이 여러개 나왔다가 상승이 나왔을 경우 상승 바로 앞에 있는 
//  하락만 남기고 모두 제거
// -상승이 반복되다가 하락을 만나지 못하고 끝 데이터에 이르렀을 경우 잠정적으로 
//  마지막에 있는 상승을 지그재그 포인트로 정했다가 뒤에 데이터가 더 추가될 경우 바로 
//  뒤에 상승이 또 나타나는지 고려해 주어야 한다.
//  반대의 경우도 마찬가지
// ex)     0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  17  18  19  20  21
//       시작 0  상 0  0  상 0  0  0  상  0  하   0  하  하   0   0   상  0  상   0  끝
// 위와 같은 구조로 되어 있을 경우 시작, 9, 14, 17, 19, 끝이 지그재그 포인트가 된다.
// 애구 글자로 표현하기가 복잡하네... 
//******************************************************************************************************************
CZigzag::CZigzag() : 
	m_MoveAverge(0.0), 
	m_Startpoint(0.0), 
	m_lastDirection(NONE),
	m_PrevHighValue(0.0),
	m_PrevLowValue(0.0),
	m_ChangeValue(0.0),
	m_Min(0.0),
	m_Max(0.0),
	m_pZigDataPacket(NULL), 
	m_pZigIndexPacket(NULL),
	m_pEndPacket(NULL),
	m_pEndPriceList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_ZIGZAGCHART)
{
}

void CZigzag::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList || !pIndicatorInfo) return;
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		m_pEndPacket = GetPacket(calculateData.GetPoint(), chartFormulateData);
		m_pEndPriceList = GetnumericDataList(m_pEndPacket);
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pEndPacket) return;
	m_pEndPacket->GetMinMax(0, m_pEndPacket->GetCount(), m_Min, m_Max);

	if(!IsValidPacket(m_pEndPriceList))
		return;

	if(SetGraphPacketList(chartFormulateData.GetCalculateType(), pPacketList, pGraphCalculateData, 
		                   pIndicatorInfo))
		return;

	CPoint point = calculateData.GetPoint();

	CScaleBaseData::HORZSCALEDRAWERTYPE nType = pPacketList->GetDateUnitType();

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pZigDataPacket) return;

	CList<double, double>* pZigDataList = m_pZigDataPacket->GetnumericDataList();
	CList<double, double>* pZigIndexList = m_pZigIndexPacket->GetnumericDataList();
	///조건값을 2개를 받아서 첫번째 값은 원하는 수치이고 두번째 값은 가격인지 %인지 결정
	GetZigzag(point, GetCondition(0, pConditionList), GetCondition(1, pConditionList),
			  m_pEndPriceList, pZigDataList, pZigIndexList, false, nType);

	CString strType = m_pEndPacket->GetType(false);
	SetPacketTypes(m_pZigDataPacket, TYPE_TWODECIMAL, strType, 0);
	SetPacketTypes(m_pZigIndexPacket, TYPE_TWODECIMAL, strType, 0);
}

void CZigzag::GetZigzag(const CPoint& point, double dCondition1, double dCondition2,
				   CList<double, double>* pEndPriceList, CList<double, double>* pZigDataList,
				   CList<double, double>* pZigIndexList, bool CLCflage,
				   CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	DIRECTION nDirection = NONE, nPreDirection = NONE;
	double HighLimit=0.0, LowLimit=0.0, ChangeValue=0.0, PrevValue=0.0, CurValue=0.0, CompareValue =0.0;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPriceList || !pZigDataList || !pZigIndexList) return;

	int nCount = pEndPriceList->GetCount();
	int nIndex=0, nSignal=0;
	BOOL HighFlage = true, LowFlage = true;
	int nPoint = point.x;

	if(point.x)
	{
		if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY )
			return ;
		else if(nType == CScaleBaseData::HORZ_TIME_ONE)
		{
			if(point.x != pEndPriceList->GetCount()-1)
			{
				nPoint = 0;
				PrevValue = GetAt(pEndPriceList, nPoint);
				pZigDataList->RemoveAll();
				pZigIndexList->RemoveAll();
			}
			else
				return ;
		}
		else if(nType == CScaleBaseData::HORZ_TICK)
		{
			PrevValue = GetRealDataIndex(pEndPriceList, pZigDataList, pZigIndexList, nPoint);
			if(PrevValue < 0)
				return ;
			if(pZigIndexList->IsEmpty())
			{
				nPreDirection = m_lastDirection = NONE;
			}
			else
			{
				nPreDirection = m_lastDirection;
			}
			nIndex = nPoint;
		}
	}
	else
	{
		double Max = 0.00, Min = pEndPriceList->GetHead();
		PrevValue = GetAt(pEndPriceList, nPoint);
		
		if(dCondition2==2)	//단순 수치
			m_ChangeValue = dCondition1;
		//else if(dCondition2==1)	///퍼센트
		else	///퍼센트
		{
			if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY )
				m_ChangeValue = PrevValue * (dCondition1 / 100);
			else
				m_ChangeValue = (m_Max - m_Min)*(dCondition1/100);
		}
	}

	double dPrevValue = 0;
	HighLimit = PrevValue + m_ChangeValue;
	LowLimit = PrevValue - m_ChangeValue;
	int i = 0;
	for( i = nPoint; i < nCount; i++)
	{
		CurValue = GetAt(pEndPriceList, i);

		// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( 0 == CurValue)	CurValue = dPrevValue;
		else				dPrevValue = CurValue;

		pZigIndexList->AddTail(0.00);

		if(CurValue > HighLimit){  //n%상승..
			nDirection = HIGH;
			if(nDirection != nPreDirection){
				m_lastDirection = nDirection;
				pZigIndexList->SetAt(FindPosition(nIndex, *pZigIndexList), nIndex);
			}
			nIndex = i, nPreDirection = nDirection;
			LowLimit = MakeChangLowValue(CurValue, dCondition2, dCondition1, nType);
			HighLimit = CurValue;
		}
		else if(CurValue < LowLimit){
			nDirection = LOW;
			if(nDirection != nPreDirection){
				m_lastDirection = nDirection;
				pZigIndexList->SetAt(FindPosition(nIndex, *pZigIndexList), nIndex);
			}
			nIndex = i, nPreDirection = nDirection;
			HighLimit = MakeChangHighValue(CurValue, dCondition2, dCondition1, nType);
			LowLimit = CurValue;
			//nIndex는 n%상승, 하락한 봉의 위치를 저장하고 있다
		}
	}
	pZigIndexList->SetAt(FindPosition(i-1, *pZigIndexList), i-1);       

	int nPrevIndex=0, nCurIndex=0;
	double step=0,  Tostep = 0, TPrevValue=0;
	POSITION pointpos;
	if(point.x){
		if(nPoint)
		{
			pointpos = FindPosition(nPoint-1, *pEndPriceList); 
			PrevValue = GetNext(pEndPriceList, pointpos);
		}
		else
		{
			pointpos = FindPosition(nPoint, *pEndPriceList); 
			PrevValue = GetNext(pEndPriceList, pointpos);
		}

		nPrevIndex = nPoint-1;
		pZigIndexList->SetAt(pZigIndexList->GetHeadPosition(), m_Startpoint);
		m_PrevHighValue = PrevValue;
		m_PrevLowValue  = PrevValue;
	}
	else{
		pointpos = FindPosition(nPoint, *pEndPriceList); 
		PrevValue = GetNext(pEndPriceList, pointpos);
		pZigDataList->AddTail(PrevValue);
		m_PrevHighValue = PrevValue;
		m_PrevLowValue  = PrevValue;
		//첫데이타는를 high,low 값으로 잡는다..
	}
	int IndexCount = 0, StepCount = 0;
	double TotalStep = 0.00;
	const int indexCnt = pZigIndexList->GetCount();
	if(!CLCflage)
	{
		for(int n=nPrevIndex+1; n<indexCnt; n++)
		{
			nCurIndex = (int)(pZigIndexList->GetAt(FindPosition(n, *pZigIndexList)));
			if(nCurIndex && (abs(nCurIndex) != 9999))
			{
				CurValue = GetAt(pEndPriceList, nCurIndex);

				// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
				//		if Error, Set All with Close for Base or Market Close Price.
				if( 0 == CurValue && 0 < nCurIndex)	CurValue = GetAt(pEndPriceList, nCurIndex - 1);;

				step = (CurValue-PrevValue)/(nCurIndex-nPrevIndex);
				CompareValue = PrevValue; // 직전의 고,저 값을 알기 위한 플레그 역활
				//꺽인선을 위해서 계산하는 것임.. 지그재그에서는 전혀 필여 없는데 말이여..
				TotalStep = TotalStep + fabs(step);
				StepCount++;
				///////////////////////////////////////////////////////////////////////////
				for(int l=nPrevIndex+1; l<=nCurIndex; l++)
				{
					pZigDataList->AddTail(PrevValue+=step);
					if(PrevValue > m_PrevHighValue && HighFlage)
					{
						if(!l)
							pZigIndexList->SetAt(FindPosition(l, *pZigIndexList), 9999);
						else
							pZigIndexList->SetAt(FindPosition(l-1, *pZigIndexList), 9999);
						HighFlage = false; LowFlage = true;
					}
					if(PrevValue < m_PrevLowValue && LowFlage)
					{
						if(!l)
							pZigIndexList->SetAt(FindPosition(l, *pZigIndexList), -9999);
						else
							pZigIndexList->SetAt(FindPosition(l-1, *pZigIndexList), -9999);
						LowFlage = false; HighFlage = true;
					}
				}
				if(StepCount == 1)
					HighFlage = true; LowFlage = true;
				nPrevIndex = nCurIndex;
				
				PrevValue  = CurValue;
				//직전의 고,저값을 가져오기 위해서 indexlist에서 항시 전 index의 값을 가지고 있어야 한다..
				if(CurValue > CompareValue)
					m_PrevHighValue = CurValue;  //직전 고점을 바꾼다..
				else if(CurValue < CompareValue)
					m_PrevLowValue  = CurValue;  //직전 저점을 바꾼다..
			}
		}
		if (StepCount > 0 && TotalStep > 0)
		{
			m_MoveAverge = TotalStep / StepCount;
			//첫데이타를 따로 저장하고 그곳에 색깔 시작값을 넣어 준다..
			POSITION pos = pEndPriceList->GetHeadPosition();
			if(pEndPriceList->GetNext(pos) < pEndPriceList->GetNext(pos))
			{
				m_Startpoint = pZigIndexList->GetHead();
				pZigIndexList->SetAt(FindPosition(0, *pZigIndexList), -9999);
			}
			else
			{
				m_Startpoint = pZigIndexList->GetHead();
				pZigIndexList->SetAt(FindPosition(0, *pZigIndexList), 9999);
			}
		}
	}
}
double CZigzag::GetRealDataIndex(CList<double, double>* pEndPriceList,CList<double, double>* pZigDataList, 
					             CList<double, double>* pZigIndexList, int& nPoint)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPriceList || !pZigDataList || !pZigIndexList) return 0;

	double PrevValue = 0.0;
	if(nPoint){
		if(pZigDataList->GetCount()==0 || pZigIndexList->GetCount() == 0)
			return -1;

		///뒤에서부터 시작하여 두번째 지그재그 포인트까지 지운다. 그 이유는 가장 처음 발견되는 지그재그 포인트는
		///진정한 지그재그 포인트가 아닐수가 있기 때문이다.
		pZigDataList->RemoveTail();
		pZigIndexList->RemoveTail();
		//끝에서 부터 첫 인덱스 포인터가 나올때 까지 데아타를 지운다...
		while(!pZigIndexList->GetTail() || fabs(pZigIndexList->GetTail()) == 9999){
			pZigDataList->RemoveTail();
			pZigIndexList->RemoveTail();
			if(pZigIndexList->IsEmpty())
				break;
		}
		if(pZigIndexList->GetCount()){
			pZigDataList->RemoveTail();
			pZigIndexList->RemoveTail();
		}
		////끝에서 부터 두번째 인덱스 포인터가 나올때 까지 데아타를 지운다...
		if(pZigIndexList->GetCount())
			while(!pZigIndexList->GetTail() || fabs(pZigIndexList->GetTail()) == 9999){
				pZigDataList->RemoveTail();
				pZigIndexList->RemoveTail();
				if(pZigIndexList->IsEmpty())
					break;
			}
		if(pZigIndexList->IsEmpty()){
			nPoint=0;	///제일 마지막 지그재그 포인트의 인덱스0이거나 지그재그 포인트가 없을 경우
			POSITION endpos = FindPosition(nPoint, *pEndPriceList);	
			PrevValue = GetNext(pEndPriceList, endpos);//리얼이때 인덱스카운터가 0일때..
		}
		else{
			const int nEndIndex = pZigIndexList->GetCount()-1;
			POSITION indexpos = FindPosition(nEndIndex, *pZigIndexList);
			nPoint = (int)(GetNext(pZigIndexList, indexpos));
			POSITION endpos = FindPosition(nPoint, *pEndPriceList);
			PrevValue = GetNext(pEndPriceList, endpos);
			nPoint++;	//현재 포인트까지는 계산되어 있으므로 다음 포인트부터 계산해야 한다.
		}
	}
	return PrevValue;
}


bool CZigzag::ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList,
						  CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		m_pZigDataPacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + "Zig_Data",	false);//"Zig_Data"
		m_pZigIndexPacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + "Zig_Index",	false);//"Zig_Index"

		ClearPacketData(m_pZigDataPacket);
		ClearPacketData(m_pZigIndexPacket);
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSubGraphPacketList) return FALSE;

	pSubGraphPacketList->AddTail2( m_pZigIndexPacket);
	pSubGraphPacketList->AddTail2( m_pZigDataPacket);

	return (m_pZigDataPacket != NULL && m_pZigIndexPacket != NULL);
}

void CZigzag::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pZigDataPacket);
	ClearPacketData(m_pZigIndexPacket);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

double CZigzag::MakeChangHighValue(double PrevValue, double dCondition2, double dCondition1,
								   CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	if(dCondition2==2)	//단순 수치
		m_ChangeValue = dCondition1;
	else
	{///퍼센트
		if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY )
			m_ChangeValue = PrevValue * (dCondition1 / 100);
		else
			m_ChangeValue = (m_Max - m_Min) * dCondition1 / 100; 
	}
	double HighLimit		= PrevValue + m_ChangeValue;
	return HighLimit;
}

double CZigzag::MakeChangLowValue(double PrevValue, double dCondition2, double dCondition1,
								  CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	if(dCondition2==2)	//단순 수치
		m_ChangeValue = dCondition1;
	else	///퍼센트
	{
		if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY )
			m_ChangeValue = PrevValue * (dCondition1 / 100);
		else
			m_ChangeValue = (m_Max - m_Min) * dCondition1 / 100; 
	}
	double LowLimit		= PrevValue - m_ChangeValue;
	return LowLimit;
}

bool CZigzag::SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
								   CPacketList* pPacketList,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
		{
			return ( !ClearPacket(calculatetype, pPacketList, pSubGraphList, pIndicatorInfo));
		}
	}
	return (m_pZigDataPacket == NULL && m_pZigIndexPacket == NULL);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//꺽은선 차트
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
CCLC::CCLC() : 
	m_step(0.0),
	m_nType(CScaleBaseData::HORZ_NOTYPE),
	m_lastDirection(NONE),
	m_PrevHighValue(0.0),
	m_PrevLowValue(0.0),
	m_pCLCDataPacket(NULL),
	m_pCLCIndexPacket(NULL),
	m_pEndPacket(NULL),
	m_pEndPriceList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_CLCCHART)
{
}

void CCLC::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList || !pIndicatorInfo) return;
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		m_pEndPacket = GetPacket(calculateData.GetPoint(), chartFormulateData);
		m_pEndPriceList = GetnumericDataList(m_pEndPacket);
	}
	if(!IsValidPacket(m_pEndPriceList))
		return ;

	if(SetGraphPacketList(chartFormulateData.GetCalculateType(), pPacketList, pGraphCalculateData, 
		                   pIndicatorInfo))
		return ;

	CPoint point = calculateData.GetPoint();
	if(point.x == m_pEndPriceList->GetCount()-1)
		return ;

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !m_pCLCDataPacket) return;

	else if(point.x == 0 && m_pCLCDataPacket->GetCount() > 0)
		return ;
	else if(point.x > 0)
	{
		ClearPacketData(m_pCLCDataPacket);
		ClearPacketData(m_pCLCIndexPacket);
		point.x = 0;
	}
	
	///조건값을 2개를 받아서 첫번째 값은 원하는 수치이고 두번째 값은 가격인지 %인지 결정
	GetCLC(point, GetCondition(0, pConditionList), GetCondition(1, pConditionList),
		   m_pEndPriceList, m_pCLCDataPacket, m_pCLCIndexPacket);

	CString strType = TYPE_INTERGER;
	if(m_pEndPacket != NULL)
		strType = m_pEndPacket->GetType(false);

	SetPacketTypes(m_pCLCDataPacket, TYPE_TWODECIMAL, strType, 0);
	SetPacketTypes(m_pCLCIndexPacket, TYPE_TWODECIMAL, strType, 0);
}

void CCLC::GetCLC(const CPoint& point, double dCondition1, double dCondition2,
			  	  CList<double, double>* pEndPriceList, CPacket* pCLCDataPacket,
				  CPacket* pCLCIndexPacket)
{
	CZigzag zig;
	DIRECTION nDirection = NONE, nPreDirection = NONE;
	double startdata =0.00;
	double Tostep = 0,   TPrevValue=0, CompareValue = 0.00;
	int nIndex=0, ToValue = 0, nPoint = 0;
	int nPrevIndex=0, nCurIndex=0;
	int StepIndex = 0, StepCount = 0, IndexCount = 0, count = 0; 

	BOOL HighFlage = true, LowFlage = true;
	
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pCLCDataPacket || !pCLCIndexPacket) return;

	CList<double, double>* pCLCDataList = pCLCDataPacket->GetnumericDataList();
	CList<double, double>* pCLCIndexList = pCLCIndexPacket->GetnumericDataList();
	CList<double, double> dZizagDataList;
	CList<double, double> dZizagIndexList;
	
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPriceList) return;

	int nCount = pEndPriceList->GetCount();
	startdata = pEndPriceList->GetHead();

	CPacketList* pPacketList = pCLCIndexPacket->GetPacketList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return;

	CScaleBaseData::HORZSCALEDRAWERTYPE nType = pPacketList->GetDateUnitType();

	zig.GetZigzag(point, dCondition1, dCondition2, pEndPriceList, &dZizagDataList,
	              &dZizagIndexList, true, nType);

	CalculateRatio(point, dZizagIndexList, pEndPriceList);
	
	int Color =0;
	if(GetAt(pEndPriceList, 0) > GetAt(pEndPriceList, 1))
	{
		nDirection = LOW;
		Color = 1;
	}

	bool flage = SetCLCIndexList(nPrevIndex, pEndPriceList, dZizagIndexList, pCLCIndexList, pCLCDataList);

	if(pCLCIndexList->GetCount() < dZizagIndexList.GetCount())
	{
		int nCount = dZizagIndexList.GetCount() - pCLCIndexList->GetCount();
		double prevalue = pCLCDataList->GetTail(); 
		for(int i = 0; i < nCount; i++)
		{
			pCLCIndexList->AddTail(0.0);
			if(flage)
				pCLCDataList->AddTail(prevalue+=m_step);
			else
				pCLCDataList->AddTail(prevalue-=m_step);
		}
	}
	nPrevIndex = 0;
	double PrevValue = GetAt(pCLCDataList, nPoint+1);
	m_PrevHighValue = PrevValue;
	m_PrevLowValue  = PrevValue;

	if(GetAt(pCLCDataList, 0) > GetAt(pCLCDataList, 1))
		Color = 1;
	for(int n = nPrevIndex; n < pCLCIndexList->GetCount(); n++){
		nCurIndex = (int)(pCLCIndexList->GetAt(FindPosition(n, *pCLCIndexList)));
		if(nCurIndex){
			CompareValue = PrevValue; // 직전의 고,저 값을 알기 위한 플레그 역활
			double CurValue = pCLCDataList->GetAt(FindPosition(nCurIndex, *pCLCDataList));
			if(!nPrevIndex)
			{
				if(PrevValue >= CurValue)
					HighFlage = false;
				else
					LowFlage = false;
			}
			if(int p = Color%2)
				m_PrevHighValue = PrevValue;
			else
				m_PrevLowValue = PrevValue;
			
			for(int l = nPrevIndex+1; l<=nCurIndex; l++)
			{
				PrevValue = GetAt(pCLCDataList , l);
				if(PrevValue >= m_PrevHighValue && HighFlage)
				{
					if(nPrevIndex)
						pCLCIndexList->SetAt(FindPosition(l, *pCLCIndexList), 9999);
					else
						pCLCIndexList->SetAt(FindPosition(l-1, *pCLCIndexList), 9999);
					HighFlage = false; LowFlage = true;
					break;
				}
				if(PrevValue <= m_PrevLowValue && LowFlage)
				{
					if(nPrevIndex)
						pCLCIndexList->SetAt(FindPosition(l, *pCLCIndexList),-9999);
					else
						pCLCIndexList->SetAt(FindPosition(l-1, *pCLCIndexList), -9999);
					LowFlage = false; HighFlage = true;
					break;
				} 
			}
			nPrevIndex = nCurIndex;
			PrevValue = CurValue;
			Color++;
		}
	}
	return ;
}
bool CCLC::SetCLCIndexList(double nPrevIndex, CList<double, double>* pEndPriceList, 
							 CList<double, double>& dZigzagIndexList, 
							 CList<double, double>* pCLCIndexList, CList<double, double>* pCLCDataList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPriceList || !pCLCDataList || !pCLCIndexList) return FALSE;

	double remain = 0.0;
	double CompareValue = 0.0;
	DIRECTION nDirection = NONE, nPreDirection = NONE;
	bool flage = true;
	int count = 0;
	double PrevValue = GetAt(pEndPriceList, 0);
	for(int n = (int)nPrevIndex; n < dZigzagIndexList.GetCount(); n++){
		int nCurIndex = (int)(dZigzagIndexList.GetAt(FindPosition(n, dZigzagIndexList)));
		if(nCurIndex ){
			CompareValue = PrevValue; // 직전의 고,저 값을 알기 위한 플레그 역활
			double CurValue = pEndPriceList->GetAt(FindPosition(nCurIndex, *pEndPriceList));
			
			if(CurValue < PrevValue)
			{
				if(nDirection == LOW)
					dZigzagIndexList.SetAt(FindPosition((int)nPrevIndex, dZigzagIndexList), 0);
				flage = false;
				nDirection = LOW;
			}
			else if(CurValue > PrevValue)
			{
				if(nDirection == HIGH)
					dZigzagIndexList.SetAt(FindPosition((int)nPrevIndex, dZigzagIndexList), 0);
				nDirection = HIGH;
			}
			if(flage)
			{
				CurValue = CurValue - remain;
				while(PrevValue <= CurValue)
				{
					pCLCDataList->AddTail(PrevValue+=m_step);
					pCLCIndexList->AddTail(0.0);
					count++;
					if(pCLCDataList->GetCount() >= dZigzagIndexList.GetCount())
						return flage;
				}
				pCLCIndexList->SetAt(FindPosition(count-1, *pCLCIndexList), count-1);
			}
			else
			{
				CurValue = CurValue + remain;
				while(PrevValue >= CurValue)
				{
					pCLCDataList->AddTail(PrevValue-=m_step);
					pCLCIndexList->AddTail(0.0);
					count++;
					if(pCLCDataList->GetCount() >= dZigzagIndexList.GetCount())
						return flage;
				}
				pCLCIndexList->SetAt(FindPosition(count-1, *pCLCIndexList), count-1);
			}
			flage = true;
			nPrevIndex = nCurIndex;
			remain = fabs(CurValue - PrevValue);
		}
	}
	return flage;
}
double CCLC::MakeChangHighValue(double PrevValue, double dCondition2, double dCondition1,
								CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	double ChangeValue;
	if(dCondition2==2)	///단순 수치


		 ChangeValue = dCondition1;
	else	///퍼센트
		 ChangeValue = PrevValue * dCondition1 / 100;
	double HighLimit		= PrevValue + ChangeValue;
	return HighLimit;
}

double CCLC::MakeChangLowValue(double PrevValue, double dCondition2, double dCondition1,
							   CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	double ChangeValue;
	if(dCondition2==2)	///단순 수치
		 ChangeValue = dCondition1;
	else	///퍼센트
		 ChangeValue = PrevValue * dCondition1 / 100;


	double LowLimit		= PrevValue - ChangeValue;
	return LowLimit;
}
void CCLC::CalculateRatio(const CPoint& point, CList<double, double>& dZigzagList, 
						  CList<double, double>* pEndPriceList)
{
	int preindex = 0, count = 0, nPoint = 0, nCurIndex = 0;
	double PrevValue = 0.0, ratio = 0.0, curdata = 0.0, predata = 0.0;
	
	PrevValue = GetAt(pEndPriceList, nPoint);
	const int indexCnt = dZigzagList.GetCount();
	POSITION pos = dZigzagList.GetHeadPosition();

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPriceList) return;

	POSITION pos1 = pEndPriceList->GetHeadPosition();

	for(int z = point.x; z < indexCnt; z++){
		curdata = pEndPriceList->GetNext(pos1);
		if(z == 0)
		{
			predata = curdata;
			preindex = z;
		}
		nCurIndex = (int)(dZigzagList.GetNext(pos));
		if(nCurIndex)
		{
			ratio += fabs(predata - curdata);
			predata = curdata;
			preindex = z;
			count++;
		}
	}
	m_step = ratio/(pEndPriceList->GetCount()-1);
	m_PrevHighValue = PrevValue;
	m_PrevLowValue  = PrevValue;
}
bool CCLC::SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
								   CPacketList* pPacketList,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
		{
			return (!ClearPacket(calculatetype, pPacketList, pSubGraphList));
		}
	}
	return (m_pCLCDataPacket == NULL && m_pCLCIndexPacket == NULL);

}

bool CCLC::ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, 
						  CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList)
{
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		m_pCLCDataPacket = pPacketList->GetBaseData("CLC_Data", false);
		m_pCLCIndexPacket = pPacketList->GetBaseData("CLC_Index", false);

		ClearPacketData(m_pCLCDataPacket);
		ClearPacketData(m_pCLCIndexPacket);
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pSubGraphPacketList) return FALSE;

	pSubGraphPacketList->AddTail2(m_pCLCIndexPacket);
	pSubGraphPacketList->AddTail2(m_pCLCDataPacket);

	return (m_pCLCDataPacket != NULL && m_pCLCIndexPacket != NULL);
}

void CCLC::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pCLCDataPacket);
	ClearPacketData(m_pCLCIndexPacket);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}


CSellBuyHorizontalBar::CSellBuyHorizontalBar() : CIndicator( IMetaTable::EDDC_OCX_VOLUMESELLBUYCHART)
{
	// (2004.02.03, 배승원, 분석) 대기매물 지표의 산출에 필요한 Packet을 관리한다.
	m_pHorZonPacket			= NULL;
	m_pHorZonPricePacket	= NULL;
	m_pEndPriceList			= NULL;
	m_pVolumeList			= NULL;

	// (2004.02.03, 배승원, 분석) 대기매물 지표의 산출 구간의 기본값을 관리한다.
	m_StartPoint			= 0;
	m_EndPoint				= 0;
	m_Max					= 0.0;
	m_Min					= 0.0;
}

// (2004.02.03, 배승원, 분석) 대기매물 지표값을 산출한다. (산출을 위한 기본 정보 구성)
void CSellBuyHorizontalBar::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 1. Packet List를 구하여 관리한다.
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	
	// 2. 먼저 TR의 처리시를 처리한다. (초기화한다.)
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		// (2007/3/27 - Seung-Won, Bae) Use Map Setting of Packet
		CString strPacketNames = chartFormulateData.GetPacketNames();
		CPacket *pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));	// 종가
		if( !pPacket) return;
		m_pEndPriceList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));			// 기본거래량
		if( !pPacket) return;
		m_pVolumeList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));			// 매수거래량
		if( !pPacket) return;
		m_pBuyVolumeList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));			// 매도거래량
		if( !pPacket) return;
		m_pSellVolumeList = pPacket->GetnumericDataList();

		// 2.2 Output Packet Pointer를 Clear한다.
		//		(Object의 생성은 어디서 하는지?)
		m_pHorZonPacket = NULL;
		m_pHorZonPricePacket = NULL;
		m_pHorZonBuyPacket = NULL;
		m_pHorZonSellPacket = NULL;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
		CString csHoga;
		(pPacketList->GetHelpMessageMap())->Lookup("HBPRICE", csHoga);
		m_dHoga = atof(csHoga);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	}

	// 3. Input Packet이 유효한지 확인한다.
	if( !IsValidPacket( m_pEndPriceList) || !IsValidPacket( m_pVolumeList) ||
		!IsValidPacket( m_pSellVolumeList) || !IsValidPacket( m_pBuyVolumeList)) return;

	// 4. 대기매물 Output Packet을 확보한다.
	// 4.1 이미 확보되어 있는 경우는 제외한다. (TR의 경우에만 NULL로 초기화되어 있다.)
	if( !m_pHorZonPacket || !m_pHorZonPricePacket || !m_pHorZonBuyPacket || !m_pHorZonSellPacket)
	{
		// 4.2 CGraphCalculateData에서 Output Packet을 확보한다.
		// 4.2.1 지표 Output Packet 정보가 있는 경우만 처리한다.
		//		SubGraphList의 갯수는 2개 (대기매물은 항상 가격차트를 수반한다.)
		//		첫번째 리스트에는 대기매물 거래량누계, 대기매물가격대 패킷 주소
		//		두번째 리스트에는 시,고,저,종 패킷 주소
		// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
		if( !pGraphCalculateData) return;
		if( pGraphCalculateData->GetCount() > 0)
		{
			// 4.2.2 CGraphCalculateData에서 Output Packet의 Sub Graph List를 확보한다.
			CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
			if( !pSubGraphList) return;

			// 4.2.3 Output Packet을 확보한다.
			m_pHorZonPacket			= pSubGraphList->GetPacket( 0);
			m_pHorZonPricePacket	= pSubGraphList->GetPacket( 1);
			m_pHorZonBuyPacket		= pSubGraphList->GetPacket( 2);
			m_pHorZonSellPacket		= pSubGraphList->GetPacket( 3);
		}

		// 4.3 CGraphCalculateData에서 Output Packet의 확보에 실패한 경우를 처리한다.
		//		Packet List에서 직접 구한다.
		if( !m_pHorZonPacket || !m_pHorZonPricePacket)
		{
			// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
			CString strSubGraphPacketNameHeader;
			if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

			// 4.3.1 Packet List에서 고정 이름의 대기매물 Output Packet을 구한다.
			m_pHorZonPacket			= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY),				FALSE);
			m_pHorZonPricePacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C0_BUY_SELL_OVERHANGING_SUPPLY_PRICE_BAND),	FALSE);
			m_pHorZonBuyPacket		= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C0_BUY_OVERHANGING_SUPPLY),					FALSE);
			m_pHorZonSellPacket		= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C0_SELL_OVERHANGING_SUPPLY),					FALSE);

			// 4.3.2 Packet의 확보에 실패한 경우를 처리한다.
			if( !m_pHorZonPacket || !m_pHorZonPricePacket) return;

			// 4.3.3 구해진 Packet의 Data를 초기화한다.
			// (2004.02.05, 배승원) Real 처리시에는 수신된 Real에 대한 대기매물 지표의 보정작업으로
			//		구현되어 있었으나, 실제로 Real이 들어와도 그것이 구간내인 경우에만 보정해야 하므로 Logic이 맞지 않는다.
			//		아마 전에는 항상 전체 보기를 기준으로 되어 있었던 것같다.
			//		따라서 Real 처리도 TR처럼 항상 산출 구간을 중심으로 재계산하는 것으로 수정한다.
			//		따라서 if 구간밖에서 항상 Packet을 Clear시키고 재계산한다.

			// 4.3.4 구해진 Packet를 대기매물 I/O Packet 정보에 등록시킨다.
			CSubGraphPacketList *pSubGraphList = NULL;
			POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
			if( posSubGraph)
			{
				pSubGraphList = pGraphCalculateData->GetNext( posSubGraph);
				if( pSubGraphList)
				{
					pSubGraphList->AddTail2( m_pHorZonPacket);
					pSubGraphList->AddTail2( m_pHorZonPricePacket);
					pSubGraphList->AddTail2( m_pHorZonBuyPacket);
					pSubGraphList->AddTail2( m_pHorZonSellPacket);
				}
			}
		}
	}

	// (2004.06.25, 배승원) 종가가 하나도 없는 경우에서는 대기매물을 계산하지 않는다.
	if( m_pEndPriceList->GetCount() <= 0) return;

	// 6. 대기매물 산출 구간의 정보를 구하여 관리한다.
	//		이후 GetHorizontalBar()에서 이용한다.
	m_StartPoint = chartFormulateData.GetDisplayDataStartIndex();
	m_EndPoint = chartFormulateData.GetDisplayDataEndIndex();
	
	// (2006/8/22 - Seung-Won, Bae) Real Calculate is in the case of Last Data in View.
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL
		&& m_EndPoint < m_pEndPriceList->GetCount() - 1)
			return;

	// 5. Scroll에 의한 대기매물 재계산을 위하여 Ouput Packet을 Clear시킨다.
	// (2004.02.05, 배승원) Real 처리시에는 수신된 Real에 대한 대기매물 지표의 보정작업으로
	//		구현되어 있었으나, 실제로 Real이 들어와도 그것이 구간내인 경우에만 보정해야 하므로 Logic이 맞지 않는다.
	//		아마 전에는 항상 전체 보기를 기준으로 되어 있었던 것같다.
	//		따라서 Real 처리도 TR처럼 항상 산출 구간을 중심으로 재계산하는 것으로 수정한다.
	//		따라서 항상 Packet을 Clear시키고 재계산한다.
	ClearPacketData( m_pHorZonPacket);
	ClearPacketData( m_pHorZonPricePacket);
	ClearPacketData( m_pHorZonBuyPacket);
	ClearPacketData( m_pHorZonSellPacket);

	// (2004.06.25, 배승원) 시종 Index가 정상적인 값이 아니면 대기매물을 계산하지 않는다.
	if( m_EndPoint < 0 || m_StartPoint < 0) return;
	CPacket *pEndPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));//"종가"
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPacket) return;
	pEndPacket->GetMinMax( m_StartPoint, m_EndPoint, m_Min, m_Max);
	// (2004.06.25, 배승원) 종가 범주의 폭이 0이면, 최저가격치 이하로 (선물/옵션 감안, 0.001) 폭을 넓힌다.
	if( m_Min == m_Max)
	{
		m_Min -= 0.001;
		m_Max += 0.001;
	}
	
	// 7. 대기매물 지표를 산출한다.
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pIndicatorInfo) return;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	GetSellBuyHorizontalBar( chartFormulateData, calculateData.GetPoint(),
		GetCondition( 0, pIndicatorInfo->GetCommonCalConditionList()), m_pEndPriceList, m_pVolumeList,
		m_pSellVolumeList,m_pBuyVolumeList,GetCondition( 1, pIndicatorInfo->GetCommonCalConditionList()),
		m_dHoga);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가

	// 8. 대기매물 Output Packet을 등록한다.
	SetPacketTypes( m_pHorZonPacket,		pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
	SetPacketTypes( m_pHorZonPricePacket,	pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
	SetPacketTypes( m_pHorZonBuyPacket,		pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
	SetPacketTypes( m_pHorZonSellPacket,	pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
}

// 대기매물 지표를 산출한다. (가격대 구간 구성 및 거래량 누계 처리)
void CSellBuyHorizontalBar::GetSellBuyHorizontalBar(const CChartFormulateData& chartFormulateData, 
				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
				CList<double, double>* pVolumeList,CList<double, double>* pSellVolumeList,
				CList<double, double>* pBuyVolumeList,double dConditionType,double dHoga)
{
	// NULL Pointer Exception을 확인하여, 제외한다.
	if( !pEndPriceList || !pVolumeList || !pBuyVolumeList || !pSellVolumeList) return;

	// 1. 종가와 체결량 Packet의 Data를 확인한다.
	if( pEndPriceList->GetCount() < 1 || pVolumeList->GetCount() < 1 || 
		pSellVolumeList->GetCount() < 1|| pBuyVolumeList->GetCount() < 1)	return;

	// 2. 산출 범위의 끝 Index를 확인하여, 실 Data를 벗어나는 경우를 조정한다.
	//		(이것은 Chart의 우측 여백이 있는 경우로 판단된다.)
	if( m_EndPoint > pEndPriceList->GetCount() - 1) m_EndPoint = pEndPriceList->GetCount() - 1;

	// 3. Real 수신시의 경우를 우선 처리하여 제외한다.
	// Real 처리시에는 수신된 Real에 대한 대기매물 지표의 보정작업으로
	//		구현되어 있었으나, 실제로 Real이 들어와도 그것이 구간내인 경우에만 보정해야 하므로 Logic이 맞지 않는다.
	//		아마 전에는 항상 전체 보기를 기준으로 되어 있었던 것같다.
	//		따라서 Real 처리도 TR처럼 항상 산출 구간을 중심으로 재계산하는 것으로 수정한다.

	// 4. TR이나 Scroll에 의한 재계산의 경우를 처리한다.
	// 4.1 이전에 산출한 대기매물 Data들이 있는 경우 Clear 시킨다.

	if( !m_pHorZonPacket || !m_pHorZonPricePacket) return;
	if( m_pHorZonPacket->GetCount() > 0)
	{
		m_pHorZonPacket->ClearData();
		m_pHorZonPricePacket->ClearData();
		m_pHorZonSellPacket->ClearData();
		m_pHorZonBuyPacket->ClearData();
	}

	// 4.2 먼저 대기매물 거래량누계 Packet을 구간별로 Clear시킨다.
	//		(실제 조건치보다 2개 더 초기화한다. 그것은 향후 Drawing 시점에서 참조할 Data로 
	//		 그것은 향후 Drawing 시점에서 참조할 Data로 최종 종가의 가격대 Index와
	//		 산출 구간내의 가격대별 거래량누계중 최대인 가격대의 Index를 관리한다.
	//		 Drawing시에 최종 가격대와 최대 가격대의 High Light에 참조된다.)
	CList<double, double>* pHorizontalBarList = m_pHorZonPacket->GetnumericDataList();
	CList<double, double>* pSellHorizontalBarList = m_pHorZonSellPacket->GetnumericDataList();
	CList<double, double>* pBuyHorizontalBarList = m_pHorZonBuyPacket->GetnumericDataList();

	if( !pHorizontalBarList || !pSellHorizontalBarList || !pBuyHorizontalBarList) return;

	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	double nInterval;
	if( dCondition < 1) dCondition = 1;
	if(dConditionType && 0 < dHoga)
	{
		nInterval = dHoga * dCondition;
		dCondition = int( ( ( m_Max - m_Min) / nInterval) * ( 1 + DBL_EPSILON)) + 1;
	}
	else
	{
		dHoga = 0;
		nInterval = ( m_Max - m_Min) / dCondition;
	}
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	int i = 0;
	for( i = 0; i < dCondition + 2; i++) 
	{
		pHorizontalBarList->AddTail( 0.0);
		pSellHorizontalBarList->AddTail( 0.0);
		pBuyHorizontalBarList->AddTail( 0.0);
	}
	// 4.3 종가를 dCondition 개수로 분할하여 대기매물 가격대 Packet 값을 설정한다.
	CList<double, double>* pPriceZonList = m_pHorZonPricePacket->GetnumericDataList();

	if( !pPriceZonList) return;
	double	dHogaPrice = 0.0;
	for( i = 0; i <= dCondition; i++)
	{
		if( i == 0) dHogaPrice = m_Min;
		else if( i == dCondition && dHoga <= 0) dHogaPrice = m_Max;
		else dHogaPrice = m_Min + nInterval  * i;
		
		pPriceZonList->AddHead( dHogaPrice);
	}
// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	// 4.5 대기매물 산출구간의 거래량을 누계한다.
	//	(이때 최종 종가의 가격대 Index를 별도 등록 관리한다.)
	// 4.5.0 대기매물 산출 구간의 끝 종가와 거래량의 Position을 구하여 관리한다.
	//		이후 산출 끝범위를 확인하는데 이용한다.
	POSITION posEndPrice	= FindPosition( m_EndPoint, *pEndPriceList);
	POSITION posEndVolume	= FindPosition( m_EndPoint, *pVolumeList);
	POSITION posEndSellVolume	= FindPosition( m_EndPoint, *pSellVolumeList);
	POSITION posEndBuyVolume	= FindPosition( m_EndPoint, *pBuyVolumeList);
	if( posEndPrice == NULL || posEndVolume == NULL|| posEndSellVolume == NULL|| posEndBuyVolume == NULL) return;
	// 4.5.1 대기매물 산출구간의 첫 종가와 거래량의 Position을 구한다.
	POSITION posOldPrice	= NULL, posPrice	= FindPosition( m_StartPoint, *pEndPriceList);
	POSITION posOldVolume	= NULL, posVolume	= FindPosition( m_StartPoint, *pVolumeList);
	POSITION posOldSellVolume	= NULL, posSellVolume	= FindPosition( m_StartPoint, *pSellVolumeList);
	POSITION posOldBuyVolume	= NULL, posBuyVolume	= FindPosition( m_StartPoint, *pBuyVolumeList);
	if( posPrice == NULL || posVolume == NULL) return;
	// 4.5.2 산출 구간의 각 종가를 바탕으로, 해당 대기매물 가격대의 거래량에 누계처리한다.
	//		(구간 끝까지만 처리하도록 한다.)
	int nIndex;
	while( posPrice && posVolume && posSellVolume && posBuyVolume&& posEndPrice	!= posOldPrice && 
		posEndVolume != posOldVolume && posEndSellVolume != posOldSellVolume && posEndBuyVolume != posOldBuyVolume)
	{
		// 4.5.2.0 먼저 Position을 Backup한다.
		posOldPrice		= posPrice;
		posOldVolume	= posOldVolume;

		// 4.5.2.1 가격대 Index를 산출한다.
		// (2008/4/23 - Seung-Won, Bae) Repaire of the loss in double calculation.
		double dValue = ( pEndPriceList->GetNext( posPrice) - m_Min) / nInterval;
		dValue *= ( 1 + DBL_EPSILON * 10000);
		nIndex = int( dCondition - int( dValue)) - 1;

		// 4.5.2.2 Zero Base로 전환한다.
		//		이때 종가가 산출구간내의 최고가인 경우에만, nIndex가 -1로 조정되는데,
		//		이는 첫 가격대 구간인 최고가 구간은 대표값인 구간 하한값을 포함할 뿐만 아니라
		//			산출 전체구간의 최고가도 포함한다.
		if( nIndex < 0) nIndex = 0;

		// 4.5.2.3 구해진 Index의 가격대에 거래량을 누계한다.
		POSITION posVolumeBar = FindPosition( nIndex, *pHorizontalBarList);
		pHorizontalBarList->SetAt( posVolumeBar, GetAt( pHorizontalBarList, posVolumeBar)
			+ pVolumeList->GetNext(posVolume));
	
		POSITION posSellVolumeBar = FindPosition( nIndex, *pSellHorizontalBarList);
		pSellHorizontalBarList->SetAt( posSellVolumeBar, GetAt( pSellHorizontalBarList, posSellVolumeBar)
			+ pSellVolumeList->GetNext(posSellVolume));
	
		POSITION posBuyVolumeBar = FindPosition( nIndex, *pBuyHorizontalBarList);
		pBuyHorizontalBarList->SetAt( posBuyVolumeBar, GetAt( pBuyHorizontalBarList, posBuyVolumeBar)
			+ pBuyVolumeList->GetNext(posBuyVolume));

		// 4.5.2.4 마지막으로 누계된 최종 종가의 가격대 Index를 등록하여 관리한다.
		pHorizontalBarList->SetAt( FindPosition( ( int)dCondition, *pHorizontalBarList), nIndex);
		pSellHorizontalBarList->SetAt( FindPosition( ( int)dCondition, *pSellHorizontalBarList), nIndex);
		pBuyHorizontalBarList->SetAt( FindPosition( ( int)dCondition, *pBuyHorizontalBarList), nIndex);
	}

	// 4.6 대기매물 산출구간의 가격대별 거래량 누계중 최고치인 가격대의 Index를 별도 관리한다.
	POSITION pos = pHorizontalBarList->GetHeadPosition();
	POSITION posBuy = pBuyHorizontalBarList->GetHeadPosition();
	POSITION posSell = pSellHorizontalBarList->GetHeadPosition();
	if(pos == NULL) return;
	double dVolume, dMaxVolume = 0;
	double dSellVolume, dMaxSellVolume = 0;
	double dBuyVolume, dMaxBuyVolume = 0;
	for(i = 0; i < dCondition; i++)
	{
		dVolume = pHorizontalBarList->GetNext(pos);
		dSellVolume = pBuyHorizontalBarList->GetNext(posBuy);
		dBuyVolume = pBuyHorizontalBarList->GetNext(posSell);
		if( dVolume >= dMaxVolume)
		{
			dMaxVolume = dVolume;
			pHorizontalBarList->SetAt( FindPosition( ( int)dCondition + 1, *pHorizontalBarList), i);
		}
		if( dSellVolume >= dMaxSellVolume)
		{
			dMaxSellVolume = dSellVolume;
			pHorizontalBarList->SetAt( FindPosition( ( int)dCondition + 1, *pSellHorizontalBarList), i);
		}
		if( dBuyVolume >= dMaxBuyVolume)
		{
			dMaxBuyVolume = dBuyVolume;
			pHorizontalBarList->SetAt( FindPosition( ( int)dCondition + 1, *pBuyHorizontalBarList), i);
		}
	}
}

//*****************************************************************************************************
//계산이 필요없는 지표
//가격차트, 거래량차트 외
//******************************************************************************************************
void CNoCalculate::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 미니지수차트를 위해서 만든 함수
	CalculateBalancePacket(chartFormulateData, calculateData);
	// subGraph에 해당 packet 연결하기
	SetGraphPacketList(chartFormulateData, pGraphCalculateData);
}

//미니지수
void CNoCalculate::CalculateBalancePacket(const CChartFormulateData& chartFormulateData, CCalculateData& calculateData)
{
	CString strOrgPacketName = chartFormulateData.GetPacketNames();
	if(strOrgPacketName.Find("##") < 0)
		return;

	CString strPacketName = CDataConversion::GetStringData(strOrgPacketName, ",");
	if(strPacketName.IsEmpty() && !strOrgPacketName.IsEmpty())
		strPacketName = strOrgPacketName;

	if(strPacketName.Find( _MTEXT( C0_PROGRAM_TRADE)) != -1 || strPacketName.Find( _MTEXT( C0_PER_INVESTOR)) != -1)
	{
		CalExCeptionPacket(chartFormulateData, calculateData);
		return;
	}

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	CPacketList *pPacketList = chartFormulateData.GetPacketList();
	if( !pPacketList) return;

	CPacket* pPacket1 = pPacketList->GetBaseData(strPacketName);
	if(pPacket1 == NULL) return;
	CList<double, double>* pList1 = pPacket1->GetnumericDataList();
	
	CString strPacketName1 = CDataConversion::GetStringData(strPacketName, "_");
	CPacket* pPacket = pPacketList->GetBaseData(strPacketName1, false);
	CString temp = strPacketName1 + "_" + _MTEXT( C6_PREVIOUS_DAY_COMPARISON);
	CPacket* pk = pPacketList->GetBaseData(temp, true);

	if(pPacket == NULL || pk == NULL)
		return ;
	CPoint point = calculateData.GetPoint();
	CList<double, double>* pList = pPacket->GetnumericDataList();
	if(pList == NULL || pList->GetCount() <= 0)
		return ;
	POSITION pos = NULL;
	int start = 0;

	CString tempStr = strPacketName;
	CDataConversion::GetStringData(tempStr, "##");
	int nMarity = atoi(tempStr);
	CPacket* pPreRatioPacket = NULL;
	BOOL bStart = FALSE;
	CString strName = strPacketName1 + "_" + _MTEXT( C6_CHANGE_RATE);
	pPreRatioPacket = pPacketList->GetBaseData(strName, true);
	if(!pPreRatioPacket) return;

	// (2004.03.10, 배승원, 임시 수정) 비교차트의 한 Block내에 두 개의 선물종목을 추가한 경우,
	//		Real이 드문 Packet을 보충해주는 Logic에 의해 다른 Packet의 지표산출에 오류가 있는 것으로 추정된다.
	//		따라서 임시 방편으로 Real 수신시 전 Packet을 다시 계산하는 것으로 수정한다.
	point.x = 0;

	if(point.x == 0)
	{
		pPacket1->ClearData(true);
		pPreRatioPacket->ClearData(TRUE);
		start = 0;
		//pPacket1->AppendData(0.0);
		pos = pList->FindIndex(start);
		bStart = TRUE;
	}
	else
		pos = pList->GetTailPosition();


	if(pos == NULL)
		return ;

	if(nMarity < 1)
		nMarity = 1;

	if(pList->GetCount() < 1)
		return ;
	double dStartPrice = 0.0;
	BOOL bRecalc = TRUE;
	
	dStartPrice = pList->GetHead();
	//전일종가 : Yesterday, 현재시가 : CurPrice , R : 비율로 그릴 경우 	
	if(strPacketName.Find("YesterPriceR##")!=-1)
		dStartPrice = pPacket->GetStandardValue(); 
	else if(strPacketName.Find("YesterPrice##")!=-1)
	{
		dStartPrice = pPacket->GetStandardValue();
		bRecalc = FALSE;
	}
	else if(strPacketName.Find("CurPrice##")!=-1)
		bRecalc = FALSE;


	start = point.x;
	double dDiv = dStartPrice / 100;
	double dData;
	for(int i = start; i < pList->GetCount() ; i++)
	{
		if(pos == NULL)
			return ;

		double realPrice = pList->GetNext(pos);
		dData = CMath::Round((realPrice - dStartPrice) / dDiv, 3);
		if(bRecalc)
			SetData(point, i, dData*nMarity, pPacket1); 
		else
			SetData(point, i, realPrice*nMarity, pPacket1);
		
		int pCnt=0,rCnt = 0;
		if(pk) pCnt = pk->GetCount();
		if(pPreRatioPacket) rCnt = pPreRatioPacket->GetCount();

		if(!bStart && pk && pPreRatioPacket && (pCnt > rCnt+1))
		{
			double tData = pPreRatioPacket->GetData(rCnt-1);
			for(int i=0;i<pCnt-rCnt-1;i++)
				pPreRatioPacket->AppendData(dData);
			rCnt = pPreRatioPacket->GetCount();
		}

		if(bStart || point.x > 0 && pk->GetCount() != pPreRatioPacket->GetCount())
			pPreRatioPacket->AppendData(dData);
	}
}

//미니지수
void CNoCalculate::CalExCeptionPacket(const CChartFormulateData& chartFormulateData, CCalculateData& calculateData)
{
	CString strPacketName = chartFormulateData.GetPacketNames();
	strPacketName = CDataConversion::GetStringData(strPacketName, ",");

	CString tempStr = strPacketName;
	CDataConversion::GetStringData(tempStr, "##");
	int nMarity = atoi(tempStr);
	if(nMarity<1) nMarity = 1;
	CPoint point = calculateData.GetPoint();

	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	CPacketList *pPacketList = chartFormulateData.GetPacketList();
	if( !pPacketList) return;

	CPacket* pPacket1 = pPacketList->GetBaseData(strPacketName);
	if(pPacket1 == NULL)
		return ;
	CList<double, double>* pList1 = pPacket1->GetnumericDataList();
	if(point.x == 0)
		pPacket1->ClearData(true);
	CString strPacketName1 = CDataConversion::GetStringData(strPacketName, "_");
	CPacket* pPacket = pPacketList->GetBaseData(strPacketName1, false);
	
	if(pPacket == NULL)
		return ;

	CList<double, double>* pList = pPacket->GetnumericDataList();
	// (2004.02.07, 배승원) NULL Pointer Exception을 확인하여, 제외한다.
	if( !pList) return;

	POSITION pos = FindPosition(point.x, *pList);
	if(pos == NULL || pList->GetCount() <= 0)
		return ;
	double dmin, dmax;
	double dDiv;
	pPacket->GetMinMax(0, pPacket->GetCount(), dmin, dmax);
	if(fabs(dmin) >= fabs(dmax))
		dDiv = 1 / fabs(dmin);
	else
		dDiv = 1 / fabs(dmax);
	for(int i = point.x; i < pList->GetCount(); i++)
	{
		if(pos == NULL)
			return ;
		double dData = CMath::Round((pList->GetNext(pos) * dDiv), 3);
		SetData(point.x, i, dData*nMarity, pPacket1);
	}
}

//sy 2005.10.25.
void CNoCalculate::SetGraphPacketList(const CChartFormulateData& chartFormulateData,
							  CGraphCalculateData* pGraphCalculateData)
{
	if(!pGraphCalculateData || pGraphCalculateData->GetCount() <= 0)
		return;

	int nSubGraphCount = pGraphCalculateData->GetCount();
	int nSubGraphIndex = 0;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL) 
			InCaseNoPacketPointer(chartFormulateData, nSubGraphCount, nSubGraphIndex, pSubGraphList);

		nSubGraphIndex++;
	}
}

bool CNoCalculate::InCaseNoPacketPointer(const CChartFormulateData& chartFormulateData, 
		const int nSubGraphCount, const int nSubGraphIndex, CSubGraphPacketList* pSubGraphList)
{
	if(pSubGraphList == NULL || nSubGraphIndex < 0)
		return false;

	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(pPacketList == NULL)
		return FALSE;

	CString strPacketNames = GetPacketNames(chartFormulateData.GetPacketNames(), nSubGraphCount, nSubGraphIndex);
	if(strPacketNames.IsEmpty())
		return false;

	int nCount = CDataConversion::GetStringDataCount(strPacketNames, ",");
	for(int i = 0; i < nCount; i++)
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ",");
		if(strPacketName.IsEmpty())
			continue;

		CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
		if(pPacket == NULL)
		{
			pSubGraphList->RemoveAll();
			break;
		}
		pSubGraphList->AddTail2( pPacket);
	}

	return (pSubGraphList->GetCount() > 0 ? true: false);
}

CString CNoCalculate::GetPacketNames(const CString& strOrgPacketNames, const int nSubGraphCount, const int nSubGraphIndex) const
{
	if(strOrgPacketNames.IsEmpty())
		return "";

	CString strPacketNames = strOrgPacketNames;
	if(strPacketNames.Right(1) != ',')
		strPacketNames += ',';

	// subGraph 갯수가 1개일 경우
	if(nSubGraphCount <= 1 || nSubGraphIndex < 0)
		return strPacketNames;

	// subGraph 갯수가 2개 이상일 경우
	for(int i = 0; i < nSubGraphCount; i++)
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ",", false);
		if(i == nSubGraphIndex)
			return strPacketName;
	}

	return "";
}
//sy end

// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
CMA::CMA( IMetaTable::EDDC_INDICATOR_ID p_eID): CIndicator( p_eID)
{
	m_pEndList	= NULL;
	m_pHighList	= NULL;
	m_pLowList	= NULL;
	m_saSubGraphName.SetSize( _MA_COUNT);

	m_pPacketList = NULL;

	// (2010/4/19 - Seung-Won, Bae) for Signal Sub Graph Index.
	m_nSignalMA1 = -1;
	m_nSignalMA2 = -1;

	// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
	m_nFirstVisibleSubGraph = 0;
}

CMA::~CMA()
{
	for( int i = 0; i < _MA_COUNT; i++)
	{
		m_dlDoubleList1[ i].RemoveAll();
		m_dlDoubleList2[ i].RemoveAll();
	}
}

void CMA::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 0. Check NULL.
	m_pPacketList = chartFormulateData.GetPacketList();
	if( !pIndicatorInfo || !pGraphCalculateData || !m_pPacketList) return;

	int nCount = pIndicatorInfo->GetSubGraphCount();
	if( nCount <= 0) return;
	m_eCalcType = chartFormulateData.GetCalculateType();

	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
	{
		CPacket *pSignalPacket = NULL;
		// (2010/5/30 - Seung-Won, Bae) Do not use Indicator Name for Multiple Same Indicator.
		CString strIndicatorName( chartFormulateData.GetGraphName());
		if( !GetSignalPacket( m_pPacketList, strIndicatorName, pSignalPacket))
			return;
		pGraphCalculateData->SetSignalPacket( pSignalPacket);
	}

	// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
		m_nFirstVisibleSubGraph = _MA_COUNT;

	CString strSubGraphNames;
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType) strSubGraphNames = chartFormulateData.GetSubGraphNames();
	for( m_nSubGraphIndex = 0 ; m_nSubGraphIndex < nCount && m_nSubGraphIndex < _MA_COUNT; m_nSubGraphIndex++)
	{
		if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
			m_saSubGraphName[ m_nSubGraphIndex] = CDataConversion::GetStringData(strSubGraphNames, ",");
		if( m_saSubGraphName[ m_nSubGraphIndex].IsEmpty()) return;

		GetMA( chartFormulateData, pGraphCalculateData, pIndicatorInfo, calculateData);
	}

	// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
		if( _MA_COUNT == m_nFirstVisibleSubGraph)
			m_nFirstVisibleSubGraph = 0;

	// (2010/4/19 - Seung-Won, Bae) for Signal Sub Graph Index.
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
	{
		m_nSignalMA1 = -1;
		m_nSignalMA2 = -1;
		if( pIndicatorInfo->GetSignalData().IsShow())
		{
			CList< double, double> *pConditionList = pIndicatorInfo->GetCommonCalConditionList();
			if( pConditionList) if( 2 == pConditionList->GetCount())
			{
				m_nSignalMA1 = pConditionList->GetHead();
				m_nSignalMA2 = pConditionList->GetTail();
			}
			if( m_nSignalMA1 == m_nSignalMA2)
			{
				m_nSignalMA1 = -1;
				m_nSignalMA2 = -1;
			}
			if( nCount <= m_nSignalMA1) m_nSignalMA1 = -1;
			if( nCount <= m_nSignalMA2) m_nSignalMA2 = -1;
			if( 0 <= m_nSignalMA1) if( !m_bShow[ m_nSignalMA1]) m_nSignalMA1 = -1;
			if( 0 <= m_nSignalMA2) if( !m_bShow[ m_nSignalMA2]) m_nSignalMA2 = -1;
		}
		pGraphCalculateData->GetSignalPacket()->ClearData();
	}

	if( 0 <= m_nSignalMA1 && 0 <= m_nSignalMA2)
	{
		CSignal signal;
		signal.GetSignalTypesbyUsingGraphDatas( calculateData.GetPoint(),
												m_ppaMAPacket[ m_nSignalMA1]->GetnumericDataList(),
												m_ppaMAPacket[ m_nSignalMA2]->GetnumericDataList(),
												pGraphCalculateData->GetSignalPacket());
	}
}

inline void CMA::GetMA(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 2. Get the Environment
	CPoint point = calculateData.GetPoint();
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
	{
		// 2.1 Bind Packet to Graph.
		m_ppaMAPacket[ m_nSubGraphIndex] = NULL;
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetAt( m_nSubGraphIndex);
		if( !pSubGraphList) return;
		m_ppaMAPacket[ m_nSubGraphIndex] = pSubGraphList->GetHeadPacket();
		if( !m_ppaMAPacket[ m_nSubGraphIndex])
		{
			// (2010/5/30 - Seung-Won, Bae) Use Graph Name for Multiple Same Indicator.
			// 130612 이상신 : 이평선 화살표 안나타나는 현상 수정(ISSUE NO - 6817).
			// 130814 이상신 : 차트가 시세가 이상하게 들어오는 현상(ISSUE NO - 7435). 위 이슈때문에 생긴 사항, 6817 원상복귀.
			m_ppaMAPacket[ m_nSubGraphIndex] = m_pPacketList->GetBaseData( chartFormulateData.GetGraphName() + "_" + m_saSubGraphName[ m_nSubGraphIndex], false);
//			m_ppaMAPacket[ m_nSubGraphIndex] = m_pPacketList->GetBaseData( m_saSubGraphName[ m_nSubGraphIndex], false);

			m_ppaMAPacket[ m_nSubGraphIndex]->ClearData();
			pSubGraphList->AddTail2( m_ppaMAPacket[ m_nSubGraphIndex]);
		}

		// 2.2 Get Input Data List
		CPacket *pSourcePacket = NULL;
		CString strPacketNames = pIndicatorInfo->GetPacketNames( m_nSubGraphIndex);
		if( strPacketNames.IsEmpty() || IMetaTable::EDDC_OCX_PRICEMACHART != m_eID)
			strPacketNames = pIndicatorInfo->GetCommonPacketNames();
		m_naInputPacketType[ m_nSubGraphIndex] = 1;
		// (2010/12/2 - Seung-Won, Bae) for Pseudo Packet.
		CString strPseudoName, strHighPacket( _MTEXT( C0_HIGH)), strLowPacket( _MTEXT( C0_LOW)), strClosePacket( _MTEXT( C0_CLOSE));
		CString strHLCPacket( _MTEXT( C0_COMBINE_HIGH_LOW_CLOSE)), strHLPacket( _MTEXT( C0_COMBINE_HIGH_LOW));
// 		if( 0 <= strPacketNames.Find( _MTEXT( C0_PSEUDO_CLOSE))) strPseudoName = _MTEXT( C0_PSEUDO_CLOSE);
// 		if( strPseudoName.IsEmpty())
// 			if( 0 <= strPacketNames.Find( _MTEXT( C0_PSEUDO_HIGH))) strPseudoName = _MTEXT( C0_PSEUDO_HIGH);
// 		if( strPseudoName.IsEmpty())
// 			if( 0 <= strPacketNames.Find( _MTEXT( C0_PSEUDO_LOW))) strPseudoName = _MTEXT( C0_PSEUDO_LOW);
// 		if( strPseudoName.IsEmpty())
// 			if( 0 <= strPacketNames.Find( _MTEXT( C0_PSEUDO_OPEN))) strPseudoName = _MTEXT( C0_PSEUDO_OPEN);
// 		if( !strPseudoName.IsEmpty())
// 		{
// 			int nLeft = strPacketNames.Find( strPseudoName);
// 			int nRight = nLeft + strPseudoName.GetLength();
// 			CString strPseudoLeft = strPacketNames.Left( nLeft);
// 			CString strPseudoRight = strPacketNames.Mid( nRight);
// 			nLeft = strPseudoLeft.ReverseFind( '_');
// 			nRight = strPseudoRight.Find( '_');
// 			if( 0 <= nLeft && 0 <= nRight)
// 			{
// 				strPseudoLeft = strPseudoLeft.Mid( nLeft);
// 				strPseudoRight = strPseudoRight.Left( nRight + 1);
// 				
// 				strHighPacket	= strPseudoLeft + _MTEXT( C0_PSEUDO_HIGH) + strPseudoRight;
// 				strLowPacket	= strPseudoLeft + _MTEXT( C0_PSEUDO_LOW) + strPseudoRight;
// 				strClosePacket	= strPseudoLeft + _MTEXT( C0_PSEUDO_CLOSE) + strPseudoRight;
// 				strHLCPacket	= strPseudoLeft + _MTEXT( C0_PSEUDO_HIGH) + "/" + _MTEXT( C0_PSEUDO_LOW) + "/" + _MTEXT( C0_PSEUDO_CLOSE) + strPseudoRight;
// 				strHLPacket		= strPseudoLeft + _MTEXT( C0_PSEUDO_HIGH) + "/" + _MTEXT( C0_PSEUDO_LOW) + strPseudoRight;
// 			}
// 		}
		if( 0 <= strPacketNames.Find( strHighPacket) && 0 <= strPacketNames.Find( strLowPacket))
		{
			if( 0 <= strPacketNames.Find( strClosePacket))
			{
				m_naInputPacketType[ m_nSubGraphIndex] = 3;
				pSourcePacket = m_pPacketList->GetBaseData( strHLCPacket, false);
			}
			else
			{
				m_naInputPacketType[ m_nSubGraphIndex] = 2;
				pSourcePacket = m_pPacketList->GetBaseData( strHLPacket, false);
			}

			m_pHighList = GetnumericDataList( m_pPacketList->GetBaseData( strHighPacket));
			m_pLowList = GetnumericDataList( m_pPacketList->GetBaseData( strLowPacket));
			m_pEndList = GetnumericDataList( m_pPacketList->GetBaseData( strClosePacket));
		}
		else
		{
			if( 0 <= strPacketNames.Find( ',')) strPacketNames = CDataConversion::GetStringData(strPacketNames, ",");
			pSourcePacket = m_pPacketList->GetBaseData( strPacketNames, true);
		}
		m_pdlaSourceList[ m_nSubGraphIndex] = CIndicator::GetnumericDataList( pSourcePacket);

		// 2.3 Get Calculate Condition.
		m_daMAPeriod[ m_nSubGraphIndex] = 99999;
		m_naMAType[ m_nSubGraphIndex] = 0;
		CList< double, double> *pConditionList = pIndicatorInfo->GetCalConditionList( m_nSubGraphIndex);
		if( pConditionList) if( 2 <= pConditionList->GetCount())
		{
			m_daMAPeriod[ m_nSubGraphIndex] = pConditionList->GetHead();
			m_naMAType[ m_nSubGraphIndex] = pConditionList->GetTail();
		}

		// Get StartIndex
		int nStartIndex = m_daMAPeriod[ m_nSubGraphIndex] - 1;
		switch( m_naMAType[ m_nSubGraphIndex])
		{
			case 2:	// EMA
					nStartIndex = 0;
					break;
					// AMA
			case 7:	nStartIndex = m_daMAPeriod[ m_nSubGraphIndex];
					break;
		}

		// 2.4 Set Packet Type
		// (2009/9/19 - Seung-Won, Bae) for FX/WorldOn Index Digit.
		CString strDataType = TYPE_TWODECIMAL, strScaleType = TYPE_TWODECIMAL;
		if( pSourcePacket)
		{
			strScaleType = pSourcePacket->GetType( false);
			nStartIndex += pSourcePacket->GetStartPeriod();
		}
		if( CChartInfo::HTS != chartFormulateData.GetChartMode()) strDataType = strScaleType;
		SetPacketTypes( m_ppaMAPacket[ m_nSubGraphIndex], strDataType, strScaleType, nStartIndex);

		m_bShow[ m_nSubGraphIndex] = !pIndicatorInfo->GetSubGraphHiding( m_nSubGraphIndex);
		if( !m_bShow[ m_nSubGraphIndex]) m_ppaMAPacket[ m_nSubGraphIndex]->ClearData();
		// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
		else if( m_nSubGraphIndex < m_nFirstVisibleSubGraph) m_nFirstVisibleSubGraph = m_nSubGraphIndex;

		if( IsIndicatorCalculated( chartFormulateData.GetCalculateType(), m_ppaMAPacket[ m_nSubGraphIndex])) return;
	}
	else if( CCalculateBaseData::REALDATA_CAL != m_eCalcType) return;
	if( !m_pdlaSourceList[ m_nSubGraphIndex]) return;
	if( !m_ppaMAPacket[ m_nSubGraphIndex]) return;
	if( !m_bShow[ m_nSubGraphIndex]) return;
	if( m_daMAPeriod[ m_nSubGraphIndex] < 1) return;

	// for Combinated Input Packet.
	if( 1 < m_naInputPacketType[ m_nSubGraphIndex] && m_pEndList && m_pHighList && m_pLowList)
	{
		int nStartIndex = point.x;
		int nSourceLastIndex = m_pdlaSourceList[ m_nSubGraphIndex]->GetCount() - 1;
		if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
		{
			if( nSourceLastIndex + 1 != m_pPacketList->GetMaxCount())	// It can be bigger or smaller.
			{
				nStartIndex = 0;
				nSourceLastIndex = -1;
				m_pdlaSourceList[ m_nSubGraphIndex]->RemoveAll();
			}
			else nStartIndex = nSourceLastIndex + 1;
		}
		// (2010/4/15 - Seung-Won, Bae) Caution! It can be multiple calculated on Real Time.
		else if( nStartIndex <= nSourceLastIndex) for( ; nStartIndex <= nSourceLastIndex; nSourceLastIndex--)
				m_pdlaSourceList[ m_nSubGraphIndex]->RemoveTail();
		if( nSourceLastIndex < nStartIndex - 1) nStartIndex = nSourceLastIndex + 1;

		double end = 0.0, high = 0.0, low = 0.0;
		POSITION endpos		= CIndicator::FindPosition( nStartIndex, *m_pEndList);
		POSITION highpos	= CIndicator::FindPosition( nStartIndex, *m_pHighList);
		POSITION lowpos		= CIndicator::FindPosition( nStartIndex, *m_pLowList);
		while( endpos && highpos && lowpos)
		{
			end		= m_pEndList->GetNext( endpos);
			high	= m_pHighList->GetNext( highpos);
			low		= m_pLowList->GetNext( lowpos);

			// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( high < end) high = low = end;

			if( 3 == m_naInputPacketType[ m_nSubGraphIndex])
					m_pdlaSourceList[ m_nSubGraphIndex]->AddTail( ( end + high + low) / 3);
			else	m_pdlaSourceList[ m_nSubGraphIndex]->AddTail( ( high + low) / 2);
		}
	}
	int nSourceCount = m_pdlaSourceList[ m_nSubGraphIndex]->GetCount();
	if( nSourceCount <= 0 || nSourceCount <= point.x) return;
	if (m_saSubGraphName[ m_nSubGraphIndex].CompareNoCase(calculateData.GetPacketNamesForSave()) != 0)
		return;

	switch( m_naMAType[ m_nSubGraphIndex])
	{
		case 0:
		case 1:	GetSimpleMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 2:	GetExponentialMA(	point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 3:	GetWeightedMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 4:	GetGeometricMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 5:	GetHarmonicMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 6:	GetTriangularMA(	point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 7:	GetAdaptiveMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
	}
}

inline void	CMA::SetMAValue( const int &p_nStartIndex, const int &nUpdateIndex, const double &dMA, CPacket *pMAPacket)
{
	if( 0 == p_nStartIndex)
	{
		if( nUpdateIndex >= pMAPacket->GetCount())
			pMAPacket->AppendData( dMA);
		else
			pMAPacket->UpdateData( dMA, nUpdateIndex, false);
	}
	else
		pMAPacket->UpdateData( dMA, nUpdateIndex, false);
}

void CMA::GetSimpleMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	double dSource = 0.0;
	double dSum = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA = pMAPacket->GetData( nIndex - 1);
		dSum = dMA * dPeriod;
	}

	// nIndex : [0 ~ dPeriod - 1]
	if( nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dSum += dSource;
		dMA = dSum / dPeriod;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	double dPreSource = 0.0;
	for( ; psnSource && psnPreSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dPreSource = pdlSourceList->GetNext( psnPreSource);
		dSum += dSource - dPreSource;
		dMA = dSum / dPeriod;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}
}

void CMA::GetExponentialMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	double dSource = 0.0;
	double dSmoothConst = 2 / ( dPeriod + 1);
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA = pMAPacket->GetData( nIndex - 1);
	}

	// nIndex : [0 ~ 0]
	if( 0 == nIndex)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dMA = dSource;
		SetMAValue( p_nStartIndex, nIndex++, dMA, pMAPacket);
	}

	// nIndex : [1 ~]
	for( ; psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dMA = dMA * ( 1 - dSmoothConst) + dSource * dSmoothConst;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}
}

void CMA::GetWeightedMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	CList< double, double> &dlSum = m_dlDoubleList1[ m_nSubGraphIndex];
	double dSource = 0.0;
	double dSum = 0.0;
	double dWeightedSum = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA = pMAPacket->GetData( nIndex - 1);
		dWeightedSum = dMA / 2 * ( dPeriod * ( dPeriod + 1));

		// dSum
		POSITION psnSum = CIndicator::FindPosition( nIndex - 1, dlSum);
		dSum = dlSum.GetAt( psnSum);
		while( nIndex < dlSum.GetCount())
			dlSum.RemoveTail();
	}
	else
	{
		dlSum.RemoveAll();
	}

	// nIndex : [0 ~ dPeriod - 1]
	if( nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dWeightedSum = dWeightedSum + dSource * ( nIndex + 1);
		dMA = dWeightedSum * 2 / ( dPeriod * ( dPeriod + 1));
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dSum = dSum + dSource;
		dlSum.AddTail( dSum);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	double dPreSource = 0.0;
	for( ; psnSource && psnPreSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dPreSource = pdlSourceList->GetNext( psnPreSource);
		dWeightedSum = dWeightedSum - dSum + dSource * dPeriod;
		dMA = dWeightedSum * 2 / ( dPeriod * ( dPeriod + 1));
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dSum = dSum + dSource - dPreSource;
		dlSum.AddTail( dSum);
	}
}

void CMA::GetGeometricMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	double dSource = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
			dMA = pMAPacket->GetData( nIndex - 1);
	else	dMA = 1.0;

	// nIndex : [0 ~ dPeriod - 1]
	if( nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dMA *= pow( dSource, 1/dPeriod);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	double dPreSource = 0.0;
	for( ; psnSource && psnPreSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dPreSource = pdlSourceList->GetNext( psnPreSource);
		dMA *= pow( dSource / dPreSource, 1/dPeriod);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}
}

void CMA::GetHarmonicMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	double dSource = 0.0;
	double dInverseSum = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA = pMAPacket->GetData( nIndex - 1);
		dInverseSum = 1 / dMA * dPeriod;
	}

	// nIndex : [0 ~ dPeriod - 1]
	if( nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dInverseSum += 1 / dSource;
		dMA = 1 / ( dInverseSum / dPeriod);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	double dPreSource = 0.0;
	for( ; psnSource && psnPreSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dPreSource = pdlSourceList->GetNext( psnPreSource);
		dInverseSum += 1 / dSource - 1 / dPreSource;
		dMA = 1 / ( dInverseSum / dPeriod);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}
}

void CMA::GetTriangularMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	const double dPeriod1 = int( dPeriod / 2 + 0.5);
	const double dPeriod2 = dPeriod1 + ( dPeriod1 * 2 == dPeriod ? 1 : 0);

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	CList< double, double> &dlMA1 = m_dlDoubleList1[ m_nSubGraphIndex];
	double dSource = 0.0;
	double dSum1 = 0.0, dSum2 = 0.0;
	double dMA1 = 0.0, dMA2 = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA2 = pMAPacket->GetData( nIndex - 1);
		dSum2 = dMA2 * dPeriod2;

		// dMA1
		POSITION psnMA1 = CIndicator::FindPosition( nIndex - 1, dlMA1);
		dMA1 = dlMA1.GetAt( psnMA1);
		while( nIndex < dlMA1.GetCount())
			dlMA1.RemoveTail();
		dSum1 = dMA1 * dPeriod1;
	}
	else
	{
		dlMA1.RemoveAll();
	}

	// nIndex : [0 ~ dPeriod1 - 2]
	if( nIndex < dPeriod1 - 1) for( ; nIndex < dPeriod1 - 1 && psnSource; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);

		dSum1 += dSource;
		dMA1 = dSum1 / dPeriod1;
		dSum2 = 0;
		dMA2 = 0;
		SetMAValue( p_nStartIndex, nIndex, dMA2, pMAPacket);

		dlMA1.AddTail( dMA1);
	}

	// nIndex : [dPeriod1 - 1]
	if( dPeriod1 - 1 == nIndex) for( ; dPeriod1 - 1 == nIndex && psnSource; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);

		dSum1 += dSource;
		dMA1 = dSum1 / dPeriod1;
		dSum2 += dMA1;
		dMA2 = dSum2 / dPeriod2;
		SetMAValue( p_nStartIndex, nIndex, dMA2, pMAPacket);

		dlMA1.AddTail( dMA1);
	}

	// nIndex : [dPeriod1 ~ dPeriod - 1]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod1, *pdlSourceList);
	double dPreSource = 0.0;
	if( dPeriod1 <= nIndex && nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource && psnPreSource; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);
		dPreSource	= pdlSourceList->GetNext( psnPreSource);

		dSum1 += dSource - dPreSource;
		dMA1 = dSum1 / dPeriod1;
		dSum2 += dMA1;
		dMA2 = dSum2 / dPeriod2;
		SetMAValue( p_nStartIndex, nIndex, dMA2, pMAPacket);

		dlMA1.AddTail( dMA1);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreMA1 = CIndicator::FindPosition( nIndex - dPeriod2, dlMA1);
	POSITION psnLastMA1 = NULL;
	double dPreMA1 = 0.0;
	for( ; psnSource && psnPreSource && psnPreMA1; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);
		dPreSource	= pdlSourceList->GetNext( psnPreSource);
		dPreMA1		= dlMA1.GetNext( psnPreMA1);

		dSum1 += dSource - dPreSource;
		dMA1 = dSum1 / dPeriod1;
		dSum2 += dMA1 - dPreMA1;
		dMA2 = dSum2 / dPeriod2;
		SetMAValue( p_nStartIndex, nIndex, dMA2, pMAPacket);

		psnLastMA1 = dlMA1.AddTail( dMA1);
		if( 1 == dPeriod2) psnPreMA1 = psnLastMA1;
	}
}

void CMA::GetAdaptiveMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = NULL;
	CList< double, double> &dlDiff		= m_dlDoubleList1[ m_nSubGraphIndex];
	CList< double, double> &dlDiffSum	= m_dlDoubleList2[ m_nSubGraphIndex];
	double dSource = 0.0;
	double dLastSource = 0.0;
	double dDiffSum = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		psnSource	= CIndicator::FindPosition( nIndex - 1, *pdlSourceList);
		dLastSource	= pdlSourceList->GetNext( psnSource);

		while( nIndex < dlDiff.GetCount())
			dlDiff.RemoveTail();

		POSITION psnDiffSum = CIndicator::FindPosition( nIndex - 1, dlDiffSum);
		dDiffSum = dlDiffSum.GetAt( psnDiffSum);
		while( nIndex < dlDiffSum.GetCount())
			dlDiffSum.RemoveTail();

		dMA = pMAPacket->GetData( nIndex - 1);
	}
	// nIndex : [0]
	else
	{
		dlDiff.RemoveAll();
		dlDiffSum.RemoveAll();

		psnSource	= pdlSourceList->GetHeadPosition();
		dSource		= pdlSourceList->GetNext( psnSource);
		dMA = dSource;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dLastSource = dSource;
		dlDiff.AddTail( 0.0);
		dlDiffSum.AddTail( 0.0);

		nIndex++;
	}

	// nIndex : [1 ~ dPeriod]
	double dDiff = 0.0;
	if( nIndex <= dPeriod) for( ; nIndex <= dPeriod && psnSource; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);

		dDiff = fabs( dSource - dLastSource);
		dDiffSum += dDiff;
		dMA = dSource;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dLastSource = dSource;
		dlDiff.AddTail( dDiff);
		dlDiffSum.AddTail( dDiffSum);
	}

	// nIndex : [dPeriod + 1~]
	POSITION psnPreSource	= CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	POSITION psnDiff		= CIndicator::FindPosition( nIndex - dPeriod, dlDiff);
	POSITION psnLastDiff	= NULL;
	double dPreSource = 0.0, dSmoothFactor = 0.0, dEffRatio = 0.0, dPreDiff = 0.0;
	for( ; psnSource && psnPreSource && psnDiff; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);
		dPreSource	= pdlSourceList->GetNext( psnPreSource);
		dPreDiff	= dlDiff.GetNext( psnDiff);

		dDiff = fabs( dSource - dLastSource);
		dDiffSum += dDiff - dPreDiff;
		dEffRatio = fabs( dSource - dPreSource) / ( 0 == dDiffSum ? 1 : dDiffSum);
		dSmoothFactor = dEffRatio * ( ( 2. / 3.) - ( 2. / 31.)) + 2. / 31.;
		dSmoothFactor *= dSmoothFactor;
		dMA += dSmoothFactor * ( dSource - dMA);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dLastSource = dSource;
		psnLastDiff = dlDiff.AddTail( dDiff);
		dlDiffSum.AddTail( dDiffSum);

		if( 1 == dPeriod) psnDiff = psnLastDiff;
	}
}
