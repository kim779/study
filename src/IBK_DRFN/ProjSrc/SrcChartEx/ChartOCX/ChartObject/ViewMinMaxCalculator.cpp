// ViewMinMaxCalculator.cpp: implementation of the CViewMinMaxCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewMinMaxCalculator.h"

#include <assert.h>
#include "../Include_Chart/Dll_Load/PacketList.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/PacketBase.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/DataMath.h"
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/PacketPointerList.h"
#include "CalculatorData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CViewMinMaxCalculator

// public ===============================================================================
// view min, max 값 가져오기
CDisplayDataMinMax CViewMinMaxCalculator::GetViewHorzMinMax(const CHorzDisplayDataMinMaxInputData& inputData) const
{
	// "백분율" 일 경우
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	if( !strncmp( _MTEXT( C6_PERCENT), inputData.GetPacketName(), 6))
		return CDisplayDataMinMax(0.0, 100.0);
	// "MarketProfile" 일 경우
	else if(inputData.GetPacketName() == _MTEXT( C3_MARKET_PROFILE))
		return GetViewHorzMinMax_MarketProfile(inputData);

	// 일반형 scale을 구한다.
	CDisplayDataMinMax displayDataMinMax;
	GetPacketMinMax(inputData.GetPacket(), inputData.GetDisplayAttributes(), false, displayDataMinMax, FALSE);
	return displayDataMinMax;
}

// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
bool CViewMinMaxCalculator::GetViewVertMinMax(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine)
{
	if(!IsViewVertMinMax(inputData.GetSubGraphPacketList()))
		return false;

	// "백분율" 일 경우
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	if( !strncmp( _MTEXT( C6_PERCENT), inputData.GetScaleData(), 6))
	{
		displayDataMinMax = CDisplayDataMinMax(0.0, 100.0);
		return true;
	}

	// "지표 Data" 일 경우
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	if( !strncmp( _MTEXT( C6_INDICATOR_DATA), inputData.GetScaleData(), 9))
		return GetViewVertMinMax_Indicate(inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);

	// "packet" 인 경우
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	return GetViewVertMinMax_Etc(inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);
}


// private ==============================================================================
// 세로 scale의 min/max를 구할지의 여부
bool CViewMinMaxCalculator::IsViewVertMinMax(CSubGraphPacketList* pSubGraphPacketList) const
{
	return (pSubGraphPacketList != NULL && pSubGraphPacketList->GetCount() > 0);
}

// --------------------------------------------------------------------------------------
// "MarketProfile" 일 경우
CDisplayDataMinMax CViewMinMaxCalculator::GetViewHorzMinMax_MarketProfile(const CHorzDisplayDataMinMaxInputData& inputData) const
{
/*
	if(inputData.GetPacket() == NULL)
		return CDisplayDataMinMax(0.0, 0.0);

	CMPPacket* pMPPacket = (CMPPacket*) inputData.GetPacket();
	CList<CMProfile*, CMProfile*>* pMProfileList = pMPPacket->GetMarketProfileList();
	if(pMProfileList == NULL || pMProfileList->GetCount() <= 0)
		return CDisplayDataMinMax(0.0, 0.0);
*/
	double dMin = 0.0;
	double dMax = 0.0;
/*
	CMProfile* pMProfile = pMProfileList->GetHead();
	if( pMProfile != NULL) dMin = pMProfile->GetDate();
	pMProfile = pMProfileList->GetTail();
	if( pMProfile != NULL) dMax = pMProfile->GetDate();
*/
	return 	CDisplayDataMinMax( dMin, dMax);
}

// --------------------------------------------------------------------------------------
// "지표 Data" 인 경우
// -> 지표명으로 Min/Max를 구한다.
// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
bool CViewMinMaxCalculator::GetViewVertMinMax_Indicate(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine)
{
	// 마지막의 packet만 min/max 인 경우 - "Zig Zag", "꺽은선차트"
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	if( g_iMetaTable.IsViewVertMinMax_EndPacket(inputData.GetIndicatorName()))
		return GetViewVertMinMax_EndPacket(inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);
	// "역시계곡선" 인 경우
	else if( g_iMetaTable.IsScaleGroup_InverseLine(inputData.GetIndicatorName()))
		return GetViewVertMinMax_HeadPacket(inputData, displayDataMinMax, p_bWithFullRange);
	// (2004.02.06, 배승원) 선물 전용 P&F의 경우 별도로 확인한다.
	//		1. 특히 일반 P&F Group에 해당되므로 일반 P&F보다 우선 확인, 처리한다.
	//		2. 선물 전용 P&F Chart의 Packet은 'FUPF_ML시가', 'FUPF_ML종가', 'FUPF_새일자가'로 구성되어 있다.
	//		3. 이들 모든 Packet이 가격(Price)로 Y Scale에 해당되지만,
	//			'FUPF_새일자가'는 새로운 일자로 변경되는 가격만을 명시는 것으로,
	//			일자가 변경되지 않는 경우 0을 갖아 Minimum Scale에 영향을 줄 수 있다.
	//		4. MinMax에 '0'을 감안하지 않도록 조정한다.
	else if(inputData.GetIndicatorName() == _MTEXT( C2_FUTURE_OPTION_P_AND_F))
		return GetViewVertMinMax_FuPAndF( inputData, displayDataMinMax) ? true : false;
	// "P&F" 인 경우
	else if( g_iMetaTable.IsDrawGroup_PAndF(inputData.GetIndicatorName()))
		return GetViewVertMinMax_PAndF(inputData, displayDataMinMax, p_bWithFullRange);
	// "대기매물" 인 경우
// (2006.12.06) Seunghwan Ahn
	else if( g_iMetaTable.IsVolumeForSale(inputData.GetIndicatorName()))
// (2006.12.06) Seunghwan Ahn
//	else if(inputData.GetIndicatorName() == _MTEXT( C2_OVERHANGING_SUPPLY))
		return GetViewVertMinMax_Sale(inputData, displayDataMinMax, p_bWithFullRange);
	// "MarketProfile" 인 경우
	else if(inputData.GetIndicatorName() == _MTEXT( C3_MARKET_PROFILE))
		return GetViewVertMinMax_MarketProfile(inputData, displayDataMinMax);
	// ( + ) percent 형태인 경우 - "심리지표", "RSI", "Stochastics Fast", "Stochastics Slow"
	else if( g_iMetaTable.IsScaleGroup_PlusPercent(inputData.GetIndicatorName()))
	{
		displayDataMinMax = CDisplayDataMinMax(0.0, 100.0);
		return true;
	}
	// (-) percent 형태인 경우 - "William's %R"
	else if( g_iMetaTable.IsScaleGroup_MinusPercent(inputData.GetIndicatorName()))
	{
		displayDataMinMax = CDisplayDataMinMax(-100.0, 0.0);
		return true;
	}
	// (2006.11.09 안승환) 분산차트 추가
	else if(inputData.GetIndicatorName() == _MTEXT( C2_DISPERSION))
		return GetViewVertMinMax_DisperSion(inputData, displayDataMinMax, p_bWithFullRange);
	// (2007/1/15 - Seung-Won, Bae) Line Chart of Price Chart
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	if( ( inputData.GetSubGraphType() == CGraphBaseData::Bong_Type
			&& inputData.GetSubGraphStyle().GetGraphBongStyle() == CGraphBaseData::LineBong)
		|| ( inputData.GetSubGraphType() == CGraphBaseData::Line_Type
			&& inputData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::ComparisonLine))
		return GetViewVertMinMax_EndPacket( inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);

	return GetViewVertMinMax_Indicate_General_Gap(inputData, displayDataMinMax, p_bWithFullRange);
}

bool CViewMinMaxCalculator::GetViewVertMinMax_HeadPacket(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CPacket* pPacket = inputData.GetSubGraphPacketList()->GetHeadPacket();
	return GetPacketMinMax(pPacket, inputData.GetDisplayAttributes(), false, displayDataMinMax, p_bWithFullRange);
}

// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
bool CViewMinMaxCalculator::GetViewVertMinMax_EndPacket(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine)
{
	CPacket* pPacket = inputData.GetSubGraphPacketList()->GetTailPacket();
	if( !GetPacketMinMax(pPacket, inputData.GetDisplayAttributes(), false, displayDataMinMax, p_bWithFullRange)) return false;
	// (2007/1/14 - Seung-Won, Bae) Include Base Line Value
	if( pPacket->IsUseStandardValue())
	{
		double dMin = displayDataMinMax.GetDisplayMin();
		double dMax = displayDataMinMax.GetDisplayMax();
		// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		if(bDrawBaseLine)
		{
			double dBase = pPacket->GetStandardValue();
			double dGap = (dMax - dMin) * (20. / 100.);
			if( dBase < dMin && dBase > dMin - dGap) dMin = dBase;
			if( dMax < dBase && dMax + dGap > dBase) dMax = dBase;
		}
		displayDataMinMax.SetDisplayData( dMin, dMax);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.02.06
//
//	목적	:	선물 전용 P&F Chart
//
//	내용	:	1. 선물 전용 P&F Chart의 Packet은 'FUPF_ML시가', 'FUPF_ML종가', 'FUPF_새일자가'로 구성되어 있다.
//				2. 이들 모든 Packet이 가격(Price)로 Y Scale에 해당되지만,
//					'FUPF_새일자가'는 새로운 일자로 변경되는 가격만을 명시는 것으로,
//					일자가 변경되지 않는 경우 0을 갖아 Minimum Scale에 영향을 줄 수 있다.
//				3. MinMax에 '0'을 감안하지 않도록 조정한다.
//
//	참고	:	1. DoesViewMinMaxWithoutZero()를 이용하여 GetViewVertMinMax_Indicate_General()에서
//					처리하려고 하였으나, DrawStyle이 0인 MP Chart과 구분되지 않아 별도 Routine으로 제공한다.
//
//	수정	:	(2004.02.22, 배승원) 위/아래로 1호가(0.05)씩을 추가하여 여백이 보이도록 한다.
//				(2004.02.24, 배승원) 실시간 Tick P&F는 Scale의 급변을 제한하기 위하여,
//					첫 두 Packet의 첫 Data로 각각 Min/Max 값을 제공한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CViewMinMaxCalculator::GetViewVertMinMax_FuPAndF( const CVertDisplayDataMinMaxInputData &inputData, CDisplayDataMinMax &displayDataMinMax)
{
	// 1. Sub Graph Packet List와 Display Attribute을 구한다.
	CSubGraphPacketList *pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if( !pSubGraphPacketList) return false;
	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();

	// 2. 각 Packet을 모두 확인하여 Scale을 확보한다.
	// (2004.02.24, 배승원) 실시간 Tick P&F는 Scale의 급변을 제한하기 위하여,
	//		첫 두 Packet의 첫 Data로 각각 Min/Max 값을 제공한다.
	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	if( !pos) FALSE;
	CPacket* pPacket = pSubGraphPacketList->GetNext( pos);
	int nMinX100 = int( pPacket->GetData( 0) * 100);
	if( !pos) FALSE;
	pPacket = pSubGraphPacketList->GetNext( pos);
	int nMaxX100 = int( pPacket->GetData( 0) * 100);

	// (2004.02.22, 배승원) 위/아래로 1호가(0.05)씩을 추가하여 여백이 보이도록 한다.
	displayDataMinMax.SetDisplayData( ( nMinX100 - 5) / 100., ( nMaxX100 + 5) / 100.);

	return TRUE;
}

bool CViewMinMaxCalculator::GetViewVertMinMax_PAndF(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CSubGraphPacketList* pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return false;

	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();
	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = pSubGraphPacketList->GetNext(pos);
		assert(pPacket != NULL);
		if(pPacket->GetName().Find( _MTEXT( C0_OPEN)) < 0)
			GetViewVertMinMax_Packet(pPacket, displayAttributes, false, displayDataMinMax, p_bWithFullRange);
	}

	return displayDataMinMax.IsInitialized() ? true : false;
}

bool CViewMinMaxCalculator::GetViewVertMinMax_Sale(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	//2004.05.12. -> "대기매물" 이 첨부형일 때의 구현 방식.
	//				 단, vertScaleGroup가 독립적일때 사용된다.
	// "대기매물" -> packet : <대기매물>, <대기매물_가격대>
	// -> Min/Max : <대기매물_가격대> 로만 구한다.
	// (2006/12/11 - Seung-Won, Bae) "대기매물_가격대" is second packet.
	CSubGraphPacketList *pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if( !pSubGraphPacketList) return false;
	const CList < CPacket *, CPacket *> &packetList = pSubGraphPacketList->GetSubGraphPacketList();
	if( packetList.GetCount() < 2) return false;
	POSITION psnPacket = packetList.FindIndex( 1);
	if( !psnPacket) return false;
	CPacket *pPacket = packetList.GetAt( psnPacket);
	if( !pPacket) return false;

	// 시작과 끝은 Packet Data 갯수로 한다.
	// -> 대기매물 현재가/최대값 index를 나타내는 값은 빼고 실질적인 data 갯수를 구한다.
	CDisplayAttributes displayAttributes(0, pPacket->GetCount() -1);
	return GetViewVertMinMax_Packet(pPacket, displayAttributes, false, displayDataMinMax, p_bWithFullRange);
}

bool CViewMinMaxCalculator::GetViewVertMinMax_MarketProfile(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax)
{
	CMPPacket* pMPPacket = (CMPPacket*) inputData.GetSubGraphPacketList()->GetHeadPacket();
	if(pMPPacket == NULL)
		return false;

	// (2004.01.20, 배승원) 각 가격구간의 대표값 범주는 전체 가격범주보다 단위 가격폭이 적다.
	//		전체 가격범주를 구하는 Interface로 변경한다.
	//		또한, Double이 아닌 IntX100를 이용한다.
	int nMinX100, nMaxX100;
	if( !pMPPacket->GetPriceScaleX100( nMinX100, nMaxX100)) return false;

	displayDataMinMax.SetDisplayData( ( nMinX100 - 15) / 100., ( nMaxX100 + 15) / 100.);
	return true;
}

// Min/Max 에 gap을 준다.
bool CViewMinMaxCalculator::GetViewVertMinMax_Indicate_General_Gap(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	if(!GetViewVertMinMax_Indicate_General(inputData, displayDataMinMax, p_bWithFullRange))
		return false;

	// Min/Max 에 gap 을 준다.
	if(!DoesDisplayDataMinMaxInGap(inputData))
		return true;

	double dGap = displayDataMinMax.GetCount() * 0.05;

	displayDataMinMax.Inflate(dGap, dGap);

	return true;
}

bool CViewMinMaxCalculator::GetViewVertMinMax_Indicate_General(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CSubGraphPacketList* pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return false;

	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();
	bool bWithoutZero = DoesViewMinMaxWithoutZero(inputData.GetSubGraphData());
	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = pSubGraphPacketList->GetNext(pos);
		assert(pPacket != NULL);
		GetViewVertMinMax_Packet(pPacket, displayAttributes, bWithoutZero, displayDataMinMax, p_bWithFullRange);
	}
	if( displayDataMinMax.IsInitialized()) ChangeViewVertMinMax_Indicator(inputData, displayDataMinMax);

	return displayDataMinMax.IsInitialized() ? true : false;
}

// ----------------------------------------------------------------------------------------------
// 기타 나머지 모두..
// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
bool CViewMinMaxCalculator::GetViewVertMinMax_Etc(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine)
{
	// "펀드" 인 경우
	if(inputData.GetSubGraphType() == CGraphBaseData::Fund_Type)
		return GetViewVertMinMax_Fund(inputData, displayDataMinMax, p_bWithFullRange);

	// GraphName 이 있는 경우
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	if(!inputData.GetIndicatorName().IsEmpty())
		return GetViewVertMinMax_Indicate(inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);

	// "packet"들 인 경우 - subGraph 에 있는 packet 인 경우
	if(GetViewVertMinMax_InSubGraphPacketList(inputData, displayDataMinMax, p_bWithFullRange))
		return true;

	// "packet"들 인 경우 - subGraph 에 있는 packet 이 아닌 경우
	return GetViewVertMinMax_OutsideSubGraphPacketList(inputData, displayDataMinMax, p_bWithFullRange);
}

// -> "펀드" 인 경우
bool CViewMinMaxCalculator::GetViewVertMinMax_Fund(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	switch(inputData.GetSubGraphStyle().GetGraphFundStyle()){
	case CGraphBaseData::ComponentRatio_Line:	// 구성비(선형)
	case CGraphBaseData::ComponentRatio_Bar:	// 구성비(바형)
		displayDataMinMax = CDisplayDataMinMax(0.0, 100.0);
		return true;
	case CGraphBaseData::ComponentRatio_Pie:	// 구성비(파이형)
	case CGraphBaseData::ReturnRiskGrade:		// 수익&위험등급
		displayDataMinMax = CDisplayDataMinMax(0.0, 0.0);
		return true;
	case CGraphBaseData::StyleBox:				// 스타일박스
		displayDataMinMax = CDisplayDataMinMax(0.0, 15.0);
		return true;
	}

	// "펀드&시장분산" 외 기타
	return GetPacketMinMax(GetPacket(inputData.GetSubGraphPacketList(), inputData.GetScaleData()), 
		inputData.GetDisplayAttributes(), false, displayDataMinMax, p_bWithFullRange);
}

// -> "packet"들 인 경우 - subGraph 에 있는 packet 인 경우
bool CViewMinMaxCalculator::GetViewVertMinMax_InSubGraphPacketList(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CSubGraphPacketList* pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return false;

	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();
	bool bWithoutZero = DoesViewMinMaxWithoutZero(inputData.GetSubGraphData());

	CString strPacketNames = inputData.GetScaleData();
	if(strPacketNames.Right(1) != ',')
		strPacketNames += ',';

	while(!strPacketNames.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ',');
		CPacket* pPacket = GetPacket(pSubGraphPacketList, strPacketName);
		if(pPacket == NULL)
		{
			displayDataMinMax.SetDisplayData(1.0, 1.0);
			return false;
		}
		
		GetViewVertMinMax_Packet(pPacket, displayAttributes, bWithoutZero, displayDataMinMax, p_bWithFullRange);
	}
	if( displayDataMinMax.IsInitialized()) ChangeViewVertMinMax_GraphTypeOrStyle(inputData.GetSubGraphData(), displayDataMinMax);

	return displayDataMinMax.IsInitialized() ? true : false;
}

// -> "packet"들 인 경우 - subGraph 에 있는 packet 이 아닌 경우
bool CViewMinMaxCalculator::GetViewVertMinMax_OutsideSubGraphPacketList(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CPacketList* pPacketList = GetPacketList(inputData.GetSubGraphPacketList());
	if(pPacketList == NULL)
		return false;

	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();
	bool bWithoutZero = DoesViewMinMaxWithoutZero(inputData.GetSubGraphData());

	CString strPacketNames = inputData.GetScaleData();
	if(strPacketNames.Right(1) != ',')
		strPacketNames += ',';

	while(!strPacketNames.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ',');
		CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
		if(pPacket == NULL)
			continue;
		
		GetViewVertMinMax_Packet(pPacket, displayAttributes, bWithoutZero, displayDataMinMax, p_bWithFullRange);
	}
	
	return displayDataMinMax.IsInitialized() ? true : false;
}

// ----------------------------------------------------------------------------------------------
// graph에 따른 Min/Max 변경
bool CViewMinMaxCalculator::ChangeViewVertMinMax_Indicator(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax)
{
	return ChangeViewVertMinMax_GraphTypeOrStyle(inputData.GetSubGraphData(), displayDataMinMax);
}

// graph type / graph style 로 Min/Max 변경
bool CViewMinMaxCalculator::ChangeViewVertMinMax_GraphTypeOrStyle(CSubGraphData* pSubGraphData, CDisplayDataMinMax& displayDataMinMax)
{
	if(pSubGraphData == NULL)
		return false;

	// <바형>의 <고정비율빗형식> 인 경우
	return ChangeViewVertMinMax_BarType(pSubGraphData, displayDataMinMax);
}

// -> <바형>인 경우
bool CViewMinMaxCalculator::ChangeViewVertMinMax_BarType(CSubGraphData* pSubGraphData, CDisplayDataMinMax& displayDataMinMax)
{
	if(pSubGraphData->GetType() != CGraphBaseData::Bar_Type)
		return false;

	switch( pSubGraphData->GetStyle().GetGraphBarStyle())
	{
		// <고정비율빗형식> 인 경우 - 절대값중 가장 큰값이 Max, Min = -Max
		case CGraphBaseData::FixedRateCombBar:
		// 고정비율상하식 추가 - ojtaso (20080603)
		case CGraphBaseData::FixedRateUpDownBar:
		{
			double dMin = fabs(displayDataMinMax.GetDisplayMin());
			double dMax = fabs(displayDataMinMax.GetDisplayMax());
			if(dMax > dMin)
				displayDataMinMax.SetDisplayData(-dMax, dMax);
			else
				displayDataMinMax.SetDisplayData(-dMin, dMin);
			break;
		}

		// "거래량차트" 일 경우엔 항상 min = 0
		case CGraphBaseData::VerticalBar:
		{
			// (2007/1/5 - Seung-Won, Bae) Support Default Max of Bar Chart. (for avoiding the Center 0.0 of VScale.)
			double dMax = displayDataMinMax.GetDisplayMax();
			if( dMax == 0.0)	displayDataMinMax.SetDisplayData( 0.0, 1.0);
			else				displayDataMinMax.SetDisplayData( 0.0, dMax);
			break;
		}

		default:	return false;
	}
	return true;
}

// ----------------------------------------------------------------------------------------------
// packet에서 Min/Max 값을 구한다.
bool CViewMinMaxCalculator::GetViewVertMinMax_Packet(CPacket* pPacket, const CDisplayAttributes& displayAttributes, 
		const bool bWithoutZero, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	if(pPacket == NULL)
		return false;

	// packetName = <xxx자료일자> 일 경우에는 min/max를 구하지 않는다!!
	if(pPacket->GetName().Find( _MTEXT( C0_DATE_TIME)) >= 0)
		return false;
	
	CDisplayDataMinMax temp;
	if(!GetPacketMinMax(pPacket, displayAttributes, bWithoutZero, temp, p_bWithFullRange))
		return false;

	// Min/Max 구하는 packet이 처음이 아닐 경우에만 기존의 Min/Max와 비교한다.
	// (2006/11/19 - Seung-Won, Bae) Support Initializing State
	if( displayDataMinMax.IsInitialized())
	{
		double dMin = displayDataMinMax.GetDisplayMin();
		double dMax = displayDataMinMax.GetDisplayMax();
		if(temp.GetDisplayMin() < dMin) dMin = temp.GetDisplayMin();
		if(temp.GetDisplayMax() > dMax) dMax = temp.GetDisplayMax();
		displayDataMinMax.SetDisplayData( dMin, dMax);
	}
	else displayDataMinMax = temp;

	return true;
}

// bWithoutZero = true : Min/Max 에 "0" 은 포함하지 않는다.
bool CViewMinMaxCalculator::GetPacketMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, 
		const bool bWithoutZero, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const
{	
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return false;

	int nStart = -1, nEnd = -1;
	
	// (2006/11/11 - Seung-Won, Bae) Support Full Range Min/Max Vert Scale.
	if( !p_bWithFullRange)
	{
		if(!GetDataStartAndEndInBlock(pPacket, displayAttributes, nStart, nEnd))
			return false;
	}
	else
	{
		nStart	= 0;
		nEnd	= INT_MAX;
	}

	bool bResult = false;
	//20100515 이문수 >>
	//double dMin = -HUGE_VAL, dMax = -HUGE_VAL;
	double dMin = HUGE_VAL, dMax = -HUGE_VAL;
	//20100515 이문수 >>

	// (2007/2/7 - Seung-Won, Bae) Calculate Min/Max without Zero
	if( pPacket->GetMinMax( nStart, nEnd, dMin, dMax, bWithoutZero) < 0) return false;

	displayDataMinMax = CDisplayDataMinMax(dMin, dMax);
	return true;
}

// ----------------------------------------------------------------------------------------------
// 해당 packet 가져오기
CPacketList* CViewMinMaxCalculator::GetPacketList(CSubGraphPacketList* pSubGraphPacketList) const
{
	if(pSubGraphPacketList == NULL)
		return NULL;

	CPacket* pPacket = pSubGraphPacketList->GetHeadPacket();
	if(pPacket == NULL)
		return NULL;

	return pPacket->GetPacketList();
}

CPacket* CViewMinMaxCalculator::GetPacket(CSubGraphPacketList* pSubGraphPacketList, const CString& strPacketName) const
{
	if(pSubGraphPacketList == NULL || strPacketName.IsEmpty())
		return NULL;

	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = pSubGraphPacketList->GetNext(pos);
		assert(pPacket != NULL);
		if(pPacket->GetName() == strPacketName)
			return pPacket;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------------------
bool CViewMinMaxCalculator::GetDataStartAndEndInBlock(CPacket* pPacket, const CDisplayAttributes& displayAttributes, int& nStart, int& nEnd) const
{
	nStart = GetDataStartInBlock(pPacket, displayAttributes.GetDataStartInBlock());
	nEnd = displayAttributes.GetDataEndInBlock();

	return (nStart >= 0 && nEnd >= 0 && nStart <= nEnd);
}

// 화면에 보이는 data 시작 index
int CViewMinMaxCalculator::GetDataStartInBlock(CPacket* pPacket, const int nDataStartInBlock) const
{
	if(pPacket == NULL || nDataStartInBlock > pPacket->GetStartPeriod())
		return nDataStartInBlock;

	return pPacket->GetStartPeriod();
}

// Min/Max에 "0" 포함 여부
// true : "0" 포함 안함. -> <바형>의 <NonZero좌표점형> 일 경우만 해당됨.
bool CViewMinMaxCalculator::DoesViewMinMaxWithoutZero(CSubGraphData* pSubGraphData) const
{
	if(pSubGraphData == NULL)
		return false;

	// (2006/12/20 - Seung-Won, Bae) Support with Not-Continued Line in 0
	switch( pSubGraphData->GetType())
	{
		case CGraphBaseData::Bar_Type:
			if (pSubGraphData->GetDrawStyle().GetGraphBarDrawStyle() == CGraphBaseData::NonZero_DotBar_Draw)
				return true;
			else break;

		case CGraphBaseData::Line_Type:
			if (pSubGraphData->GetStyle().GetGraphLineStyle() == CGraphBaseData::NonZeroLine)
				return true;
			else break;

		// (2007/2/6 - Seung-Won, Bae) Check Candle Stick Chart for avoiding Zero Open/High/Low in Base or Market Close Price.
		//		Caution! It is Koscom Only. Use EXCEPTIONVALUE Help Message in Packet.
		case CGraphBaseData::Bong_Type:
			// (2009/1/17 - Seung-Won, Bae) Do not check 'KOSCOM' name. (on sometimes, CWinApp can not be found.)
			return ( pSubGraphData->GetStyle().GetGraphBongStyle() != CGraphBaseData::LineBong);
	}

	return false;
}

// Min/Max에 gap을 줄지의 여부
bool CViewMinMaxCalculator::DoesDisplayDataMinMaxInGap(const CVertDisplayDataMinMaxInputData& inputData) const
{
	// <미니지수차트> 인 경우
	if(inputData.GetIndicatorName() == _MTEXT( C2_MINI_INDEX_CHART))
		return true;

	if(!inputData.GetIndicatorName().IsEmpty())
		return false;

	// subGraph type 로 결정.
	return DoesDisplayDataMinMaxInGap_Type(inputData.GetSubGraphData());
}

// Min/Max의 gap을 두는 graph인지의 여부.
// <바형>의 <상하식>, <상하등락식>, <고정비율빗형식> 인 경우
// -> 단, drawingStyle가 <Text형>인 경우는 gap을 주지 않는다.
bool CViewMinMaxCalculator::DoesDisplayDataMinMaxInGap_Type(const CSubGraphData* pSubGraphData) const
{
	if(pSubGraphData == NULL)
		return false;
	
	if(pSubGraphData->GetType() != CGraphBaseData::Bar_Type)
		return false;

	// drawingStyle가 <Text형>인 경우는 Text가 표현되기 때문에 gap을 줄필요가 없다!
	if(pSubGraphData->GetDrawStyle().GetGraphBarDrawStyle() == CGraphBaseData::TextBar_Draw)
		return false;

	CGraphBaseData::BARSTYLE eStyle = pSubGraphData->GetStyle().GetGraphBarStyle();
	switch(eStyle){
		case CGraphBaseData::UpDownBar: // 상하식
		case CGraphBaseData::UpDownHighLowBar: // 상하등락식
		case CGraphBaseData::FixedRateCombBar: // 고정비율빗형식
		// 고정비율상하식 추가 - ojtaso (20080603)
		case CGraphBaseData::FixedRateUpDownBar: // 고정비율상하식
			return true;
	}

	return false;
}

// (2006.11.09 안승환) 분산차트 추가
bool CViewMinMaxCalculator::GetViewVertMinMax_DisperSion(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	// 1. Sub Graph Packet List를 구한다.
	CSubGraphPacketList *pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if( !pSubGraphPacketList) return false;

	// 2. 종가 Packet을 확보한다.
	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	if( !pos) FALSE;
	CPacket* pPacket = pSubGraphPacketList->GetNext( pos);	//시가Packet
	if( !pos) FALSE;
	pPacket = pSubGraphPacketList->GetNext( pos);			//종가Packet

	return GetPacketMinMax(pPacket, inputData.GetDisplayAttributes(), false, displayDataMinMax, p_bWithFullRange);
}
// (2006.11.09 안승환) 분산차트 추가
