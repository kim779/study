// DrawingRegionCalculator.cpp: implementation of the CDrawingRegionCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawingRegionCalculator.h"

#include <assert.h>
#include <locale.h>					// for setlocale()
#include <math.h>					// for pow()

#include "../Include_Chart/Dll_Load/PacketList.h"				// PacketList.dll
#include "../Include_Chart/Dll_Load/PacketBase.h"				// PacketList.dll
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"			// IndicatorCondition.dll
#include "../Include_Chart/Dll_Load/GraphDrawer.h"				// GraphDraw.dll
#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/BaseData.h"
#include "../Include_Chart/ScaleCalculator.h"
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
// class CDrawingRegionCalculator

// public =================================================================================
// 그리는 영역(graph, scale)을 구한다.
CDrawingRegion CDrawingRegionCalculator::GetDrawingRegion(CDC* pDC,	const CDrawingRegionCalData& calData, BOOL p_bReverse) const
{
	CGraphRegionCalculator graphRegionCalculator;
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	CGraphRegions graphRegions = graphRegionCalculator.GetGraphRegions(pDC, calData, p_bReverse);

	CDrawingRegion drawingRegion;
	drawingRegion.SetGraphRegions(graphRegions);

	CRect blockRegion = calData.GetBlockRegion();
	CRect graphFrameRegion = graphRegions.GetFrameRegion();
	CRect graphDrawingRegion = drawingRegion.GetGraphRegions().GetDrawingRegion();
	drawingRegion.SetHorzScaleRegion(
		GetTopScaleRegion(		blockRegion, graphDrawingRegion, graphFrameRegion), 
		GetBottomScaleRegion(	blockRegion, graphDrawingRegion, graphFrameRegion));
	drawingRegion.SetVertScaleRegion(GetLeftScaleRegion(blockRegion, graphFrameRegion, graphDrawingRegion, calData.GetVertScaleRegionWidth().GetLeftVertScaleRegionWidth()), 
		GetRightScaleRegion(blockRegion, graphFrameRegion, graphDrawingRegion, calData.GetVertScaleRegionWidth().GetRightVertScaleRegionWidth()));

	return drawingRegion;
}

// private ================================================================================
// scale 영역
// 왼쪽
CRect CDrawingRegionCalculator::GetLeftScaleRegion(const CRect& blockRegion, const CRect& graphFrameRegion, const CRect& graphRegion, const int /*nWidth*/) const
{
	return CRect(blockRegion.left, graphRegion.top, graphFrameRegion.left, graphRegion.bottom);
}

// 오른쪽
CRect CDrawingRegionCalculator::GetRightScaleRegion(const CRect& blockRegion, const CRect& graphFrameRegion, const CRect& graphRegion, const int nWidth) const
{
	return CRect(graphFrameRegion.right, graphRegion.top, blockRegion.right, graphRegion.bottom);
}

// 위쪽
CRect CDrawingRegionCalculator::GetTopScaleRegion(		const CRect& blockRegion, const CRect &graphDrawRegion, const CRect& graphFrameRegion) const
{
	return CRect( blockRegion.left, blockRegion.top, blockRegion.right, graphFrameRegion.top - (blockRegion.top == graphFrameRegion.top ? 0 : 1));
}

// 아래쪽
CRect CDrawingRegionCalculator::GetBottomScaleRegion(	const CRect& blockRegion, const CRect &graphDrawRegion, const CRect& graphFrameRegion) const
{
	return CRect(blockRegion.left, graphFrameRegion.bottom + (blockRegion.bottom == graphFrameRegion.bottom ? 0 : 1), blockRegion.right, blockRegion.bottom);
}


//////////////////////////////////////////////////////////////////////////////////////////
// class CGraphRegionCalculator

// public ================================================================================
// graph 영역들.
CGraphRegions CGraphRegionCalculator::GetGraphRegions(CDC* pDC, const CDrawingRegionCalData& calData, BOOL p_bReverse) const
{
	CGraphRegions graphRegions;
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	graphRegions.SetFrameRegion( GetGraphFrameRegion(pDC, calData), calData.GetGraphRegionFrameThickness(), p_bReverse);
	graphRegions.SetVertScale(calData.IsVertScale());
	graphRegions.SetTitle(calData.IsGraphTitleDraw());
	graphRegions.SetText(IsGraphTextDraw(graphRegions.GetNoFrameRegion(), calData));
	graphRegions.SetBlockMoveDeleteFlag( calData.GetBlockMoveDeleteFlag());

	CSize textSize = pDC->GetTextExtent("A");
	graphRegions.SetFontSize(textSize.cy + MARGIN);

	return graphRegions;
}


// private ===============================================================================
// graph frame이 포함된 영역을 구한다.
CRect CGraphRegionCalculator::GetGraphFrameRegion(CDC* pDC, const CDrawingRegionCalData& calData) const
{
	// frame 굵기 뺀 graph 영역
	CRect maxGraphDrawingRegion = GetMaxGraphFrameRegion(calData.GetBlockRegion(), calData.GetHorzScaleRegionHeight(), calData.GetVertScaleRegionWidth());
	return maxGraphDrawingRegion;

// (2006/10/10 - Seung-Won, Bae) why? comment? why direct return?
/*
	maxGraphDrawingRegion.DeflateRect(calData.GetGraphRegionFrameThickness(), calData.GetGraphRegionFrameThickness());

	// 화면에 보일 data 수와 관련하여 계산한 실제 graph drawing region
	CRect graphFrameRegion = GetRealGraphRegion(calData, maxGraphDrawingRegion);
	graphFrameRegion.InflateRect(calData.GetGraphRegionFrameThickness(), calData.GetGraphRegionFrameThickness());
	return graphFrameRegion;
*/
}

// block region에서 scale region을 뺀 영역 --> 최대 graph region(frame 포함)
CRect CGraphRegionCalculator::GetMaxGraphFrameRegion(const CRect& blockRegion, const CHorzScaleRegionHeight& horzScaleRegionHeight, const CVertScaleRegionWidth& vertScaleRegionWidth) const
{
	return CRect(blockRegion.left + vertScaleRegionWidth.GetLeftVertScaleRegionWidth(), blockRegion.top + horzScaleRegionHeight.GetTopHorzScaleRegionHeight(),
		blockRegion.right - vertScaleRegionWidth.GetRightVertScaleRegionWidth(), blockRegion.bottom - horzScaleRegionHeight.GetBottomHorzScaleRegionHeight());
}

// 화면에 보일 data 수와 관련하여 계산한 실제적인 graph drawing region
CRect CGraphRegionCalculator::GetRealGraphRegion(const CDrawingRegionCalData& calData, const CRect& maxGraphDrawingRegion) const
{
	const CVertScaleRegionWidth& vertScaleRegionWidth = calData.GetVertScaleRegionWidth();

	CScaleBaseData::VERTSCALEPOSITION vertScalePosition = GetVertScalePosition(vertScaleRegionWidth);
	int remainedSpaceWidth = GetRemainedSpaceWidth(maxGraphDrawingRegion.Width(), calData.GetDisplayAttributes().GetDataCountInBlock());

	CRect adjustedGraphDrawingRegion = maxGraphDrawingRegion;
	adjustedGraphDrawingRegion.left = GetLeft(vertScalePosition, maxGraphDrawingRegion.left,
			calData.GetMaxVertScaleRegionWidth(), vertScaleRegionWidth.GetLeftVertScaleRegionWidth(), remainedSpaceWidth);
	adjustedGraphDrawingRegion.right = GetRight(vertScalePosition, maxGraphDrawingRegion.right,
			calData.GetMaxVertScaleRegionWidth(), vertScaleRegionWidth.GetRightVertScaleRegionWidth(), remainedSpaceWidth);

	return adjustedGraphDrawingRegion;
}

// ---------------------------------------------------------------------------------------
// drawing graph region을 data 수에 따라 적용시킬때 세로 scale 의 타입을 가져온다.
CScaleBaseData::VERTSCALEPOSITION CGraphRegionCalculator::GetVertScalePosition(const CVertScaleRegionWidth& vertScaleRegionWidth) const
{
	// scale이 양쪽에 있을 경우
	if(vertScaleRegionWidth.GetLeftVertScaleRegionWidth() > 1 && vertScaleRegionWidth.GetRightVertScaleRegionWidth() >= 1)
		return CScaleBaseData::VERT_BOTH;
	// scale이 왼쪽에만 있을 경우
	else if(vertScaleRegionWidth.GetLeftVertScaleRegionWidth() >= 1)
		return CScaleBaseData::VERT_LEFT;
	// scale이 오른쪽에만 있을 경우
	else if(vertScaleRegionWidth.GetRightVertScaleRegionWidth() >= 1)
		return CScaleBaseData::VERT_RIGHT;

	return CScaleBaseData::VERT_NONE;
}

// graph 영역에서 세로 scale 영역으로 넘길 폭(x축의 길이)
int CGraphRegionCalculator::GetRemainedSpaceWidth(const int graphDrawingRegionWidth, const int viewDataCount) const
{
	if(viewDataCount <= 0 || (graphDrawingRegionWidth / viewDataCount) <= 1)
		return 0;

	return (graphDrawingRegionWidth - viewDataCount * (graphDrawingRegionWidth / viewDataCount));
}

// ---------------------------------------------------------------------------------------
// graph drawing region의 왼쪽 좌표를 구한다. ==> space 적용
int CGraphRegionCalculator::GetLeft(const CScaleBaseData::VERTSCALEPOSITION vertScalePosition, const int orgLeft, 
		const int maxVertScaleRegionWidth, const int leftVertScaleRegionWidth, const int remainedSpaceWidth) const
{
	if(vertScalePosition == CScaleBaseData::VERT_LEFT)
		return GetData(orgLeft, maxVertScaleRegionWidth, leftVertScaleRegionWidth, remainedSpaceWidth, 1);
	// 양쪽에 scale이 있을 경우엔 절반 값을 준다. (절사)
	else if(vertScalePosition == CScaleBaseData::VERT_BOTH)
		return GetData(orgLeft, maxVertScaleRegionWidth, leftVertScaleRegionWidth, remainedSpaceWidth/2, 1);

	return orgLeft;
}

// graph drawing region의 오른쪽 좌표를 구한다. ==> space 적용
int CGraphRegionCalculator::GetRight(const CScaleBaseData::VERTSCALEPOSITION vertScalePosition, const int orgRight, 
		const int maxVertScaleRegionWidth, const int rightVertScaleRegionWidth, const int remainedSpaceWidth) const
{
	if(vertScalePosition == CScaleBaseData::VERT_RIGHT)
		return GetData(orgRight, maxVertScaleRegionWidth, rightVertScaleRegionWidth, remainedSpaceWidth, -1);
	// 양쪽에 scale이 있을 경우엔 절반 값을 준다. (절사)
	else if(vertScalePosition == CScaleBaseData::VERT_BOTH)
		return GetData(orgRight, maxVertScaleRegionWidth, rightVertScaleRegionWidth, remainedSpaceWidth/2, -1);

	return orgRight;
}

// - max scale left region : max	- min scale left region : min
// - 원래의 위치 : org				- remained space : r
// scale position : left 일 경우  => if((org+r) >= max) org+(max-min)
//									 else org+r
// scale position : right 일 경우 => if((org+r) >= max) org-(max-min)
//									 else org-r
// scalePosition 은 +1(scale position : left), -1(scale position : right) 이다!
int CGraphRegionCalculator::GetData(const int orgData, const int maxVertScaleRegionWidth, const int vertScaleRegionWidth, const int remainedSpaceWidth, const int scalePosition) const
{
	if((vertScaleRegionWidth + remainedSpaceWidth) >= maxVertScaleRegionWidth)
		return (orgData + (maxVertScaleRegionWidth - vertScaleRegionWidth)*scalePosition);

	return (orgData + remainedSpaceWidth*scalePosition);
}

// ---------------------------------------------------------------------------------------
// graph text를 쓸 영역 유무
bool CGraphRegionCalculator::IsGraphTextDraw(const CRect& graphRegion, const CDrawingRegionCalData& calData) const
{
	if(!calData.IsGraphTextDrawCheck())
		return calData.IsGraphTextDraw();

	if(!calData.IsGraphTextDraw())
		return false;

	if(graphRegion.Width() < MARGIN * 30 || graphRegion.Height() < MARGIN * 15)
		return false;

	return (calData.GetDisplayAttributes().GetDataCountInBlock() > 2);
}


//////////////////////////////////////////////////////////////////////////////////////////
// class  CTextSizeCalculator

// public =================================================================================
// scale로 영역구하기 위해 scale로 표현할 data의 최장 string의 size.
CSize CTextSizeCalculator::GetMaxHorzScaleTextSize(CDC* pDC, const CHorzScaleMaxWidthCalculatorData& calData) const
{
	if(calData.GetScaleCompart() == NULL || !IsTextSizeCalculator(calData.GetPacket()))
		return CSize(0, 0);

	// <자료일자> 일 경우
	if(calData.GetPacketName() == _MTEXT( C0_DATE_TIME))
		return GetMaxScaleTextSize_DataDate(pDC, calData.GetPacket(), *calData.GetScaleCompart());

	// <순수 packet name> 일 경우
	return GetMaxScaleTextSize_Packet(pDC, calData);
}

// 세로 scale의 최대 text size
CSize CTextSizeCalculator::GetMaxVertScaleTextSize(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	if(!IsTextSizeCalculator(calData.GetSubGraphPacketList()))
		return CSize(0, 0);

	// packet의 data 인 경우
	CSize dataSize = GetMaxScaleText_Data(pDC, calData);
	CSize rateSize = GetMaxScaleText_Rate(pDC, calData);
	if(dataSize.cx > rateSize.cx)
		return dataSize;
	return rateSize;
}


// private ================================================================================
// text size를 구할지의 유무
bool CTextSizeCalculator::IsTextSizeCalculator(CPacket* pPacket) const
{
	return (pPacket != NULL && pPacket->GetCount() > 0);
}

bool CTextSizeCalculator::IsTextSizeCalculator(CSubGraphPacketList* pSubGraphPacketList) const
{
	if(pSubGraphPacketList == NULL || pSubGraphPacketList->GetCount() <= 0)
		return false;

	return IsTextSizeCalculator(pSubGraphPacketList->GetHeadPacket());
}

// <기준선> 이 있는지의 여부
bool CTextSizeCalculator::IsBaseLineInIndicateData(CIndicatorInfo* pIndicatorInfo) const
{
	if(pIndicatorInfo == NULL || pIndicatorInfo->GetBaseLineList() == NULL)
		return false;

	return (pIndicatorInfo->GetBaseLineList()->GetCount() > 0);
}

// <기준선> 이 그리는 영역인지의 여부
// y축 위치 : "오른쪽" 또는 "왼쪽" 만 있을 경우 -> <Data> scale
//			  "양쪽" 일 경우 -> "왼쪽" : <기준선>, "오른쪽" <Data> scale
bool CTextSizeCalculator::IsBaseLineInRegion(const CVertScaleMaxWidthCalculatorData& calData) const
{
	CScaleBaseData::VERTSCALEPOSITION eVertScalePos = calData.GetVerticalScale().GetVertScalePosition();
	if(eVertScalePos == CScaleBaseData::VERT_BOTH)
		return !calData.DoesRight();

	return false;
	/*if(!calData.DoesRight())
		return true;

	return (calData.DoesRight() && calData.GetVerticalScale().GetVertScalePosition() == CScaleBaseData::VERT_RIGHT);*/
}


// ----------------------------------------------------------------------------------------
// scale 에 쓰일 data의 최대 길이
CSize CTextSizeCalculator::GetMaxScaleText_Data(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// <백분율> 인 경우
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	if( !strncmp( _MTEXT( C6_PERCENT), calData.GetVerticalScale().GetScaleData(), 6))
		return pDC->GetTextExtent("100.00%");
	// <지표 Data> 인 경우
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	else if( !strncmp( _MTEXT( C6_INDICATOR_DATA), calData.GetVerticalScale().GetScaleData(), 9))
		return GetMaxScaleTextSize_IndicateData(pDC, calData);

	// <packet> 인 경우
	CPacket* pPacket = GetVertScalePacket(calData.GetSubGraphPacketList(), calData.GetVertScalePacketName());
	return GetMaxScaleTextSize_Packet(pDC, pPacket, calData);
}

// 오른쪽 scale 에 쓰일 data rate text size.
CSize CTextSizeCalculator::GetMaxScaleText_Rate(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	if(!calData.DoesRight() || calData.GetCalcRiseFallRate_CurData() <= 0)
		return CSize(0, 0);

	CTextInRightScaleRegionDrawer textData(calData.GetGraphName(), calData.GetSubGraphPacketList(), 
		CRect(0, 0, 100, 100), calData.GetCalcRiseFallRate_CurData(), calData.GetDisplayAttributes().GetDataEndInBlock());

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	CString strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate;
	if(!CGraphDrawer().GetTextInRightScaleRegionDrawer(textData, strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate))
		return CSize(0, 0);

// 	if(strCurPrice.GetLength() > strRiseFallRate.GetLength())
// 		return pDC->GetTextExtent(strCurPrice);
// 	return pDC->GetTextExtent(strRiseFallRate);

	CString strMaxLenValue(strCurPrice);
	if( strMaxLenValue.GetLength() < strDiffPrice.GetLength())
		strMaxLenValue = strDiffPrice;
	if( strMaxLenValue.GetLength() < strPipPrice.GetLength())
		strMaxLenValue = strPipPrice;
	if( strMaxLenValue.GetLength() < strRiseFallRate.GetLength())
		strMaxLenValue = strRiseFallRate;

	return pDC->GetTextExtent(strMaxLenValue);
}

/*CSize CTextSizeCalculator::GetMaxScaleText_Rate(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	if(!calData.DoesRight() || calData.GetCalcRiseFallRate_CurData() <= 0)
		return CSize(0, 0);

	double dPrevPrice = 0.0, dCurPrice = 0.0;
	if(!GetPrices(calData, dPrevPrice, dCurPrice))
		return CSize(0, 0);

	CString strCurPrice = GetCurrentPriceText(dCurPrice);
	CString strRiseFallRate = GetRateText(dPrevPrice, dCurPrice);
	if(strCurPrice.GetLength() > strRiseFallRate.GetLength())
		return pDC->GetTextExtent(strCurPrice);
	return pDC->GetTextExtent(strRiseFallRate);
}*/

// ----------------------------------------------------------------------------------------
// <자료일자>인 경우.
CSize CTextSizeCalculator::GetMaxScaleTextSize_DataDate(CDC* pDC, CPacket* pPacket, const CScaleCompart& scaleCompart) const
{
	if(pPacket == NULL)
		return CSize(0, 0);

	CString headPacketData, dataType;
	if(GetDatePacketHeadDataAndType_DataDate(pPacket, headPacketData, dataType) == false)
		return CSize(0, 0);

	CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType = (CScaleBaseData::HORZSCALEDRAWERTYPE) pPacket->GetDateUnitType();

	CString strData = CScaleCalculator( m_hOcxWnd, m_eChartMode).GetCompartScaleData(scaleDrawerType, headPacketData, dataType, scaleCompart);
	if(strData.IsEmpty())
		return CSize(0, 0);

	return GetMaxScaleTextSize(CScaleCalculator::GetTextSize(pDC, strData), strData.GetLength(), scaleCompart.GetScaleTextType());
}

// ----------------------------------------------------------------------------
// <지표 data>인 경우.
CSize CTextSizeCalculator::GetMaxScaleTextSize_IndicateData(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	if( calData.GetGraphName() == _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY))
		return GetMaxScaleTextSize_Sale2(pDC, calData);
	// <대기매물> 일 경우
	if( g_iMetaTable.IsVolumeForSale(calData.GetGraphName()))
		return GetMaxScaleTextSize_Sale(pDC, calData);
	// <상대비교차트> 일 경우
	if( calData.GetGraphName() == _MTEXT( C2_COMPARISON_CHART))
		return pDC->GetTextExtent("100%");
	// <기준선이 있는 지표 Data> 일 경우
	if(IsBaseLineInIndicateData(calData.GetIndicatorInfo()))
		return GetMaxScaleTextSize_BaseLine(pDC, calData);

	// <packet> 일 경우
	return GetMaxScaleTextSize_Packet(pDC, calData);
}

// -> <대기매물>인 경우.
CSize CTextSizeCalculator::GetMaxScaleTextSize_Sale(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// <대기매물>이 특수형일때는 "왼쪽:%, 오른쪽:가격대" 였으나
	// 첨부형으로 수정뒤에는 "무조건 가격대"로 수정되었다,
	// 단, 가격차트가 있을 경우는 이 함수를 호출하지 않는다.

	/*// <대기매물> - 왼쪽(최대 99.99%)
	if(!calData.DoesRight())
		return GetMaxScaleTextSize(pDC, 6);*/

	// <체결가> - 오른쪽(가격)
	if(calData.GetSubGraphPacketList() == NULL)
		return CSize(0, 0);

	CPacket* pPacket = calData.GetSubGraphPacketList()->GetPacket(1);
	if(pPacket == NULL)
		return CSize(0, 0);

	return GetMaxScaleTextSize_Packet(pDC, pPacket, calData);
}

// -> <기준선>인 경우.
CSize CTextSizeCalculator::GetMaxScaleTextSize_BaseLine(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// <기준선> - 양쪽일 경우 왼쪽영역
	if(IsBaseLineInRegion(calData)){
		return GetMaxScaleTextSize(pDC, GetLongLength(calData.GetIndicatorInfo()));
	}

	// <값 (Data)> - 오른쪽
	// ( + ) percent 형태인 경우 - "심리지표", "RSI", "Stochastics"
	if( g_iMetaTable.IsScaleGroup_PlusPercent(calData.GetGraphName()))
		return GetMaxScaleTextSize(pDC, 4);
	// (-) percent 형태인 경우 - "William's %R"
	else if( g_iMetaTable.IsScaleGroup_MinusPercent(calData.GetGraphName()))
		return GetMaxScaleTextSize(pDC, 5);

	return GetMaxScaleTextSize_Packet(pDC, calData);
}

// ----------------------------------------------------------------------------
// <packet>인 경우.
CSize CTextSizeCalculator::GetMaxScaleTextSize_Packet(CDC* pDC, const CHorzScaleMaxWidthCalculatorData& calData) const
{
	int nLength = GetDrawingDataLength( calData.GetPacket(), calData.GetDisplayAttributes(), CScaleBaseData::VERT_NUM);
	return GetMaxScaleTextSize(pDC, nLength, calData.GetScaleCompart()->GetScaleTextType());
}

CSize CTextSizeCalculator::GetMaxScaleTextSize_Packet(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	int nMaxLength = GetMaxPacketDataLength(calData.GetGraphName(), calData.GetSubGraphPacketList(), 
		calData.GetDisplayAttributes(), calData.GetVerticalScale().GetVertScaleUnitType(),
		calData.GetSubGraphDisplayMinMax());

	return GetMaxScaleTextSize(pDC, nMaxLength);
}

CSize CTextSizeCalculator::GetMaxScaleTextSize_Packet(CDC* pDC, CPacket* pPacket, const CVertScaleMaxWidthCalculatorData& calData) const
{
	int nMaxLength = GetDrawingDataLength(pPacket, calData.GetDisplayAttributes(), calData.GetVerticalScale().GetVertScaleUnitType());
	return GetMaxScaleTextSize(pDC, nMaxLength);
}

// ----------------------------------------------------------------------------
// packet에서 min/max 가져오기.
bool CTextSizeCalculator::GetPacketMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, double& dMin, double& dMax) const
{
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return false;

	int nDataEndInBlock = pPacket->GetnumericDataList()->GetCount() -1;
	if(nDataEndInBlock > displayAttributes.GetDataEndInBlock())
		nDataEndInBlock = displayAttributes.GetDataEndInBlock();

	if(pPacket->GetMinMax(displayAttributes.GetDataStartInBlock(), nDataEndInBlock, dMin, dMax) < 0)
		return false;
	return true;
}

// ----------------------------------------------------------------------------
// 가장 긴 길이 가져오기
int CTextSizeCalculator::GetLongLength(CIndicatorInfo* pIndicatorInfo) const
{
	assert(pIndicatorInfo != NULL);
	if(pIndicatorInfo->GetBaseLineList() == NULL)
		return 0;

	int nLength = 0;
	POSITION pos = pIndicatorInfo->GetBaseLineList()->GetHeadPosition();
	while(pos != NULL){
		double dData = pIndicatorInfo->GetBaseLineList()->GetNext(pos);
		int temp = CDataConversion::DoubleToString(dData).GetLength();
		if(nLength < temp)
			nLength = temp;
	}

	return nLength;
}

// ----------------------------------------------------------------------------
int CTextSizeCalculator::GetMaxPacketDataLength( const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList,
												const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
												const CDisplayDataMinMax &p_displaySubGraphMinMax) const
{
	if(pSubGraphPacketList == NULL)
		return 0;

	int nMaxLength = 0;
	POSITION pos = GetDrawingDataLengthStartPosition(strGraphName, pSubGraphPacketList);
	while(pos != NULL)
	{
		CPacket* pPacket = pSubGraphPacketList->GetNext(pos);
		// packe name 에 "자료일자" 가 포함되어 있으면 구하지 않는다.
		if(pPacket == NULL || pPacket->GetName().Find( _MTEXT( C0_DATE_TIME)) != -1)
			continue;

		int nLength = GetDrawingDataLength_WithMinMax(pPacket, displayAttributes, eVertScaleUnitType, p_displaySubGraphMinMax);
		if(nMaxLength < nLength)
			nMaxLength = nLength;
	}

	return nMaxLength;
}

POSITION CTextSizeCalculator::GetDrawingDataLengthStartPosition(const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList) const
{
	if(pSubGraphPacketList == NULL)
		return NULL;

	if( g_iMetaTable.IsViewVertMinMax_EndPacket(strGraphName))
		return pSubGraphPacketList->GetTailPosition();
	return pSubGraphPacketList->GetHeadPosition();
}

int CTextSizeCalculator::GetDrawingDataLength(CPacket* pPacket, const CDisplayAttributes& displayAttributes,
											  const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const
{
	if(pPacket == NULL)
		return 0;

	// <문자> 일 경우
	if( pPacket->GetType() == _MTEXT( C6_CHARACTER))
//////////////////////////////////////////////////////////////////////////
// 2006.12.20 세로형일 때 아래쪽 공백 trim
// 세로형일 때 아래쪽 공백 trim을 위해 아래와 같이 변경
// 		return pPacket->GetLength();-->
		return GetDrawingDataLength_Text(pPacket);
//////////////////////////////////////////////////////////////////////////

	// <숫자> 일 경우
	return GetDrawingDataLength_Number(pPacket, displayAttributes, eVertScaleUnitType);
}

int CTextSizeCalculator::GetDrawingDataLength_WithMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes,
											  const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
												const CDisplayDataMinMax &p_displaySubGraphMinMax) const
{
	if(pPacket == NULL)
		return 0;

	// <문자> 일 경우
	if( pPacket->GetType() == _MTEXT( C6_CHARACTER))
//////////////////////////////////////////////////////////////////////////
// 2006.12.20 세로형일 때 아래쪽 공백 trim
// 세로형일 때 아래쪽 공백 trim을 위해 아래와 같이 변경
// 		return pPacket->GetLength();-->
		return GetDrawingDataLength_Text(pPacket);
//////////////////////////////////////////////////////////////////////////

	// <숫자> 일 경우
	return GetDrawingDataLength_Number_WithMinMax(pPacket, displayAttributes, eVertScaleUnitType, p_displaySubGraphMinMax);
}

int CTextSizeCalculator::GetDrawingDataLength_Text(CPacket* pPacket) const
{
	int nMaxLength = 0;

	POSITION pos = pPacket->GetstrDataList()->GetHeadPosition();
	while(pos != NULL){
		CString strData = pPacket->GetstrDataList()->GetNext(pos);
		strData.TrimRight(" ");
		strData.TrimLeft(" ");
		
// 		if(strData.GetLength() > nMaxLength)
// 			nMaxLength = strData.GetLength();
		//-------------------------------------------------------->
		// 2007.1.8 문자 아래 잘리는 것 해결
		// 문자 수 계산  
		setlocale( LC_CTYPE, "Korean" );	// 지역을 한국으로 설정
		if(_mbstrlen(strData) > nMaxLength)	// _mbstrlen의 반환값은 한글의 경우 2byte를 1문자로 계산
			nMaxLength = _mbstrlen(strData);
		//<--------------------------------------------------------
	}

	return nMaxLength;
}

int CTextSizeCalculator::GetDrawingDataLength_Number( CPacket* pPacket, const CDisplayAttributes& displayAttributes,
													const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const
{
	CString strData, strFormatTrans;
	if(!GetDrawingData_Number(pPacket, displayAttributes, strData, strFormatTrans))
		return 0;

	// 소숫점 이하일 경우에 scale에서 절사여부
	int nNumLength = GetDataLength(strData);
	int nFormatLength = GetFormatLength(strFormatTrans, eVertScaleUnitType);

	return ((nNumLength > nFormatLength) ? nNumLength: nFormatLength);
}

int CTextSizeCalculator::GetDrawingDataLength_Number_WithMinMax( CPacket* pPacket, const CDisplayAttributes& displayAttributes,
													const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
													const CDisplayDataMinMax &p_displaySubGraphMinMax) const
{
	CString strData, strFormatTrans;
	if(!GetDrawingData_Number_WithMinMax(pPacket, displayAttributes, strData, strFormatTrans, p_displaySubGraphMinMax))
		return 0;

	// 소숫점 이하일 경우에 scale에서 절사여부
	int nNumLength = GetDataLength(strData);
	int nFormatLength = GetFormatLength(strFormatTrans, eVertScaleUnitType);

	return ((nNumLength > nFormatLength) ? nNumLength: nFormatLength);
}

bool CTextSizeCalculator::GetDrawingData_Number( CPacket* pPacket, const CDisplayAttributes& displayAttributes, CString& strData,
												CString& strFormatTrans) const
{
	double dMin = 0, dMax = 0;
	if(!GetPacketMinMax(pPacket, displayAttributes, dMin, dMax))
		return 0;
	
	CString dataType = pPacket->GetType(false);
	double dMaxLengthData = CDataConversion::GetLongLengthData(dMin, dMax, dataType);
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(dMaxLengthData, dataType, strData, strFormatTrans, bAutoScaleType);
	if( strData.IsEmpty()) return 0;
	strFormatTrans.Replace(" ", "");

	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());
	return (!strData.IsEmpty() || !strFormatTrans.IsEmpty());
}

bool CTextSizeCalculator::GetDrawingData_Number_WithMinMax( CPacket* pPacket, const CDisplayAttributes& displayAttributes, CString& strData,
												CString& strFormatTrans, const CDisplayDataMinMax &p_displaySubGraphMinMax) const
{
	double dMin = 0, dMax = 0;
	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		It is initialized in CSubGraph::GetDisplayDataMinMax() for CMainBlockImp::CalculateDataForDrawing().
	//	if(!GetPacketMinMax(pPacket, displayAttributes, dMin, dMax))
	//		return 0;
	dMax = p_displaySubGraphMinMax.GetDisplayMax();
	dMin = p_displaySubGraphMinMax.GetDisplayMin();

	CString dataType = pPacket->GetType(false);
	double dMaxLengthData = CDataConversion::GetLongLengthData(dMin, dMax, dataType);
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(dMaxLengthData, dataType, strData, strFormatTrans, bAutoScaleType);
	if( strData.IsEmpty()) return 0;
	strFormatTrans.Replace(" ", "");

	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());
	return (!strData.IsEmpty() || !strFormatTrans.IsEmpty());
}

/*double CTextSizeCalculator::GetLongLengthData(const double& dData1, const double& dData2, const CString& dataType) const
{
	int nDecimalDownCount = CDataConversion::GetDecimalDownCount(dataType);
	CString strMin = CDataConversion::DoubleToString(dData1, nDecimalDownCount);
	CString strMax = CDataConversion::DoubleToString(dData2, nDecimalDownCount);

	if(strMin.GetLength() > strMax.GetLength())
		return dData1;

	return dData2;
}*/

int CTextSizeCalculator::GetDataLength(const CString& strData) const
{
	// 값이 소숫점 이면 그대로 소숫점을 표현한다.
	return strData.GetLength();

	// 000.00 이하일 경우엔 소숫점 .00 을 표현
	// 0,000.00 일때는 0,000 으로 표현  -> CScaleDrawer
	if(strData.Find(".") == -1)
		return strData.GetLength();

	int nNumLength = strData.GetLength();
	CString strTemp = strData;
	strTemp.TrimLeft('-');
	if(strTemp.GetLength() > 7){
		strTemp = strData.Left(strData.Find("."));
		nNumLength = strTemp.GetLength();
	}
	return nNumLength;
}

/*int CTextSizeCalculator::GetDataLength(const CString& strData) const
{
	if(strData.Find(".") == -1)
		return strData.GetLength();

	CString strTemp = strData;
	strTemp.TrimLeft('-');
	int nNumLength = strTemp.GetLength();
	if(nNumLength > 7){
		strTemp = strData.Left(strData.Find("."));
		nNumLength = strTemp.GetLength();
	}
	return nNumLength;
}*/

int CTextSizeCalculator::GetFormatLength(const CString& strFormat, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const
{
	CString strTemp = strFormat;
	if(eVertScaleUnitType == CScaleBaseData::VERT_NUM_CHAR)
		strTemp = CDataConversion( m_hOcxWnd, m_eChartMode).GetChangeFormat_NumCharUnit(strFormat);

	int nFormatLength = strTemp.GetLength();
	if(strTemp.Find(".") != -1)	
		nFormatLength = 0;

	return nFormatLength;
}

// ---------------------------------------------------------------------------------------
// packet 을 가져온다.
CPacket* CTextSizeCalculator::GetVertScalePacket(CSubGraphPacketList* pSubGraphPacketList, const CString& strPacketName) const
{
	if(pSubGraphPacketList == NULL || strPacketName.IsEmpty())
		return NULL;

	CPacket* pHeadPacket = pSubGraphPacketList->GetHeadPacket();
	if(pHeadPacket == NULL)
		return NULL;

	if(pHeadPacket->GetName() == strPacketName)
		return pHeadPacket;

	return GetVertScalePacket(pHeadPacket->GetPacketList(), strPacketName);
}

CPacket* CTextSizeCalculator::GetVertScalePacket(CPacketList* pPacketList, const CString& strPacketNames) const
{
	if(pPacketList == NULL || strPacketNames.IsEmpty())
		return NULL;

	CString strTemp = strPacketNames;
	if(strTemp.Right(1) != ',')
		strTemp += ',';

	while(!strTemp.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strTemp, ',');
		if(strPacketName.IsEmpty())
			continue;
		
		CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
		if(pPacket != NULL && pPacket->GetCount() > 0)
			return pPacket;
	}

	return NULL;
}

// packet의 처음 data와 type를 가져온다.
bool CTextSizeCalculator::GetDatePacketHeadDataAndType_DataDate(CPacket* pPacket, CString& headPacketData, CString& dataType) const
{
	if(pPacket == NULL)
		return false;

	CList<double, double>* dataList = pPacket->GetnumericDataList();
	if(dataList->GetHeadPosition() == NULL || dataList->GetCount() <= 0)
		return false;

	dataType = pPacket->GetType(false);
	headPacketData = CDataConversion::DoubleToString(dataList->GetHead(), 0);
	int nZeroCount = dataType.GetLength() - headPacketData.GetLength();
	for(int nCount = 0; nCount < nZeroCount; nCount++){
		headPacketData = "0" + headPacketData;
	}

	return true;
}

// ---------------------------------------------------------------------------------------
// text의 유형에 따라 Size를 변경한다.
CSize CTextSizeCalculator::GetMaxScaleTextSize(CDC* pDC, const int nLength, const CScaleBaseData::TEXTCOMPARTTYPE textCompart) const
{
	if(nLength <= 0)
		return CSize(0, 0);

	return GetMaxScaleTextSize(pDC->GetTextExtent("0"), nLength, textCompart);
}

CSize CTextSizeCalculator::GetMaxScaleTextSize(const CSize& textSize, const int nLength, const CScaleBaseData::TEXTCOMPARTTYPE textCompart) const
{
	if(nLength <= 0) // 값이 존재하지 않을 경우
		return CSize(0, 0);

	// text 구분 type가 세로형일 경우
	if(textCompart == CScaleBaseData::VERT_TEXT_COMPART)
		return CSize(textSize.cx, textSize.cy*nLength);	// 2007.1.8 아래 짤리는 현상 해결을 위해 (nLength+2)/2 --> nLength

	return CSize(textSize.cx*nLength, textSize.cy);
}

// ---------------------------------------------------------------------------------------
// "현재가" 를 화면상의 데이타!!
/*bool CTextSizeCalculator::GetPrices(const CVertScaleMaxWidthCalculatorData& calData, double& dPrevPrice, double& dCurPrice) const
{
	CPacket* pPacket = calData.GetSubGraphPacketList()->GetHeadPacket();
	if(pPacket == NULL || pPacket->GetnumericDataList() == NULL || pPacket->GetnumericDataList()->GetCount() <= 0)
		return false;

	POSITION pos = pPacket->GetnumericDataList()->FindIndex(calData.GetDisplayAttributes().GetDataEndInBlock() -1);
	if(pos == NULL)
		return false;

	dPrevPrice = pPacket->GetnumericDataList()->GetNext(pos);
	if(pos == NULL)
		return false;
	dCurPrice = pPacket->GetnumericDataList()->GetNext(pos);
	return true;
}

// 현재가
CString CTextSizeCalculator::GetCurrentPriceText(const double& dCurPrice) const
{
	CString strCurPrice = CDataConversion::DoubleToString(dCurPrice, 2);
	strCurPrice.TrimRight("0");
	strCurPrice.TrimRight(".");

	return CDataConversion::GetStringThousandFormat(strCurPrice);
}

// 전데이터 관련 비율
CString CTextSizeCalculator::GetRateText(const double& dPrevPrice, const double& dCurPrice) const
{
	double dDataRate = 0.0;
	if(dPrevPrice != 0.0)
		dDataRate = (dCurPrice - dPrevPrice) / dPrevPrice * 100.0;

	return CDataConversion::DoubleToString(dDataRate, 2) + "%";
}*/


// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
CSize CTextSizeCalculator::GetMaxScaleTextSize_Sale2(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// 1. Support Price Band Scale.
	if( calData.IsDualScale() && calData.DoesRight())
		return GetMaxScaleTextSize(pDC, 6);				// <대기매물> - 왼쪽(최대 99.99%)

	// <체결가> - 오른쪽(가격)
	if(calData.GetSubGraphPacketList() == NULL) return CSize(0, 0);
	CPacket *pPacket = calData.GetSubGraphPacketList()->GetPacket( 1);
	if( pPacket == NULL) return CSize(0, 0);
	int nDataCount = pPacket->GetCount();
	if( nDataCount <= 0) return CSize(0, 0);

//	int nMaxLength = GetDrawingDataLength(pPacket, calData.GetDisplayAttributes(), calData.GetVerticalScale().GetVertScaleUnitType());
//	int nMaxLength = GetDrawingDataLength_Number(pPacket, displayAttributes, eVertScaleUnitType);
	int nMaxLength = 0;
	CString strData, strFormatTrans;
//	if( GetDrawingData_Number( pPacket, calData.GetDisplayAttributes(), strData, strFormatTrans))
	double dMin = 0, dMax = 0;
//	if( GetPacketMinMax(pPacket, calData.GetDisplayAttributes(), dMin, dMax))
	if( 0 < pPacket->GetMinMax( 0, nDataCount - 1, dMin, dMax))
	{
		CString dataType = pPacket->GetType(false);
		if( dataType.Find( '.') < 0) dataType = "x 1";
		double dMaxLengthData = CDataConversion::GetLongLengthData(dMin, dMax, dataType);

		int decimalDownCount = CDataConversion::GetDecimalDownCount( dataType);
		long lDifference = long( ( dMax * pow( 10, decimalDownCount)) - long( dMin * pow( 10, decimalDownCount)) );
		CIndicatorInfo *pIndicatorInfo = calData.GetIndicatorInfo();
		if( pIndicatorInfo)
		{
			CList < double, double> *pConditionList = pIndicatorInfo->GetCommonCalConditionList();
			int nBarCount = int( pConditionList->GetHead());
			if( nBarCount < 1) nBarCount = 1;
			long lMod = lDifference % nBarCount;
			if( pConditionList->GetTail() == 0 && ( lMod != 0 || lDifference == 0))
			{
				if( dataType.Find( '.') < 0)	dataType = "x 0.1";
				else							dataType.Replace( "0.", "0.0");
			}
		}

		CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(dMaxLengthData, dataType, strData, strFormatTrans, FALSE);
		if( !strData.IsEmpty())
		{
			strFormatTrans.Replace(" ", "");

			pPacket->SetScaleType( dataType);
			if( !strData.IsEmpty() || !strFormatTrans.IsEmpty())
			{
				// 소숫점 이하일 경우에 scale에서 절사여부
				int nNumLength = GetDataLength(strData);
				int nFormatLength = GetFormatLength(strFormatTrans, calData.GetVerticalScale().GetVertScaleUnitType());
				nMaxLength = ((nNumLength > nFormatLength) ? nNumLength: nFormatLength);
			}
		}
	}

	return GetMaxScaleTextSize(pDC, nMaxLength);
}
