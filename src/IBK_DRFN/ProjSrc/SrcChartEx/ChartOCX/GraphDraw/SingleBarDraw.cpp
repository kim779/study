// SingleBarDraw.cpp : implementation file
//

#include "stdafx.h"
#include "SingleBarDraw.h"

#include "../Include_Chart/DLL_Load/IndicatorList.h"			// for CIndicatorList
#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_Chart/Dll_Load/MainBlock.h"				// for CMainBlock
#include "BongDraw.h"
#include "XScaleManager.h"

#include "GraphDrawer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSingleBarDraw
CSingleBarDraw::CSingleBarDraw()
{
	m_eGraphType = CGraphBaseData::Bar_Type;
}

CSingleBarDraw::~CSingleBarDraw()
{
}

bool CSingleBarDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	const CPacket& pPacket = *drawingGraphData.GetSubGraphPacketList()->GetTailPacket();
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);

	int nBarWidth = CMath::Round(dRealWidth);
	if(nBarWidth <= 1) 
		nBarWidth = 1;

	const CGraphDrawStyle& drawStyle = subGraphData.GetDrawStyle();
	const CGraphColor& graphColor = subGraphData.GetColor();
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	const CGraphPaintStyle& paintStyle = subGraphData.GetPaintStyle();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	if(nRealStart > nRealEnd)
		return false;
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	bool bLog = drawingGraphData.DoesVertScaleLogType();
	bool bInvert = drawingGraphData.DoesVertScaleInvertType();

	// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
	//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
	//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 한다.
	CIndicatorList *pIndicatorList = drawingGraphData.GetIndicatorList();

	switch(subGraphData.GetStyle().GetGraphBarStyle())
	{
	case CGraphBaseData::VerticalBar:// 수직식
		return DrawVerticalBar(pDC, realRegion, drawingGraphData.GetGraphName(), 
			drawStyle, graphColor, drawingGraphData.GetPriceChartColor(), 
			nBarWidth, dRealWidth, nRealStart, nRealEnd, 
			dViewMax, dViewMin, packetList, pIndicatorInfo, penThickness, bLog, bInvert, pIndicatorList);
	case CGraphBaseData::HorizontalBar:// 수평식
		return DrawHorizontalBar(pDC, realRegion, drawStyle, graphColor, 
			nBarWidth, dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, 
			packetList, penThickness, bLog, bInvert);
	case CGraphBaseData::UpDownBar:// 상하식
	// 고정비율상하식 추가 - ojtaso (20080603)
	case CGraphBaseData::FixedRateUpDownBar:// 고정비율상하식
		// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
		return DrawUpDownBar(pDC, realRegion, drawStyle, graphColor, paintStyle, 
			nBarWidth, dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, 
			packetList, pIndicatorInfo, penThickness, bLog, bInvert);
	case CGraphBaseData::CombBar:// 빗형식
		return DrawCombBar(pDC, realRegion, drawStyle, graphColor, 
			nBarWidth, dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, 
			packetList, penThickness, bLog, bInvert);
	case CGraphBaseData::BoxBar:// 박스식
		return DrawBoxBar(pDC, realRegion, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, pIndicatorInfo, penThickness, bLog, bInvert);
	case CGraphBaseData::DotBar:// 도트식
		return DrawDotBar(pDC, realRegion, drawStyle, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, bLog, bInvert);
	case CGraphBaseData::LeftRightBar:// 좌우식
		return DrawLeftRightBar(pDC, realRegion, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, packetList, penThickness);
	case CGraphBaseData::LeftRightCombBar:// 좌우빗형식
		return DrawLeftRightCombBar(pDC, realRegion, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, packetList, penThickness);
	case CGraphBaseData::GroupBar:// 그룹바식
		return DrawGroupBar(pDC, realRegion, drawStyle, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, pIndicatorInfo, penThickness, bLog, bInvert);	
	case CGraphBaseData::RatioBar:// 비율식 (ex- 매수/매도 거래량 표현시)
		return DrawRatioBar(pDC, realRegion, drawStyle, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, penThickness, bLog, bInvert);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
//	case CGraphBaseData::TextHorizontalBar:// Text가 있는 수평식
//		return DrawTextHorizontalBar(pDC, realRegion, subGraphData.GetName(), 
//			drawStyle, graphColor, nRealStart, nRealEnd, dViewMax, dViewMin, 
//			packetList, pIndicatorInfo, bLog, bInvert);
	case CGraphBaseData::TextHorizontalBar:// Text가 있는 수평식
		return DrawTextHorizontalBar(pDC, realRegion, subGraphData.GetName(), 
			drawStyle, graphColor, nRealStart, nRealEnd, dViewMax, dViewMin, 
			packetList, pIndicatorInfo, bLog, bInvert,pPacket);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	case CGraphBaseData::TextGroupBar:// Text가 있는 그룹 바식
		return DrawTextGroupBar(pDC, realRegion, drawStyle, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, penThickness, bLog, bInvert);	
	case CGraphBaseData::UpDownHighLowBar:// 상하등락식
		return DrawUpDownHighLowBar(pDC, realRegion, drawStyle, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, pIndicatorInfo, penThickness, bLog, bInvert);
	case CGraphBaseData::FixedRateLeftRightBar:// 고정비율좌우식
		return DrawFixedRateLeftRightBar(pDC, realRegion, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, packetList, penThickness);
	case CGraphBaseData::FixedRateCombBar:	// 고정비율빗형식
		return DrawFixedRateCombBar(pDC, realRegion, drawStyle, graphColor, 
			nBarWidth, dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, 
			packetList, penThickness, bLog, bInvert);
	}
	return false;
}

bool CSingleBarDraw::DrawGraph(CDC* pDC, const CRect& realRegion, const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData,
		const CString& strRQ, CXScaleManager* pxScaleManager)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	const CPacket& pPacket = *drawingGraphData.GetSubGraphPacketList()->GetTailPacket();
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);

	int nBarWidth = CMath::Round(dRealWidth);
	if(nBarWidth <= 1) 
		nBarWidth = 1;

	const CGraphDrawStyle& drawStyle = subGraphData.GetDrawStyle();
	const CGraphColor& graphColor = subGraphData.GetColor();
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	const CGraphPaintStyle& paintStyle = subGraphData.GetPaintStyle();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	if(nRealStart > nRealEnd)
		return false;
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	bool bLog = drawingGraphData.DoesVertScaleLogType();
	bool bInvert = drawingGraphData.DoesVertScaleInvertType();
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	const CGraphBaseLineStyle& baseLineStyle = subGraphData.GetBaseLineDrawStyle();

	// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
	//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
	//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 한다.
	CIndicatorList *pIndicatorList = drawingGraphData.GetIndicatorList();

	switch(subGraphData.GetStyle().GetGraphBarStyle())
	{
	case CGraphBaseData::VerticalBar:// 수직식
		return DrawVerticalBar(pDC, realRegion, drawingGraphData.GetGraphName(), 
			drawStyle, graphColor, drawingGraphData.GetPriceChartColor(), 
			nBarWidth, dRealWidth, nRealStart, nRealEnd, 
			dViewMax, dViewMin, packetList, pIndicatorInfo, penThickness, bLog, bInvert, pIndicatorList,
			strRQ, pxScaleManager, subGraphData, drawingGraphData.GetColumnIndex(), drawingGraphData.GetMainBlock());
	case CGraphBaseData::UpDownBar:// 상하식
	// 고정비율상하식 추가 - ojtaso (20080603)
	case CGraphBaseData::FixedRateUpDownBar:// 고정비율상하식
		// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
		// 기준선 두께, 모양 설정 - ojtaso (20071213)
		return DrawUpDownBar(pDC, realRegion, drawStyle, graphColor, paintStyle, 
			nBarWidth, dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, 
			packetList, pIndicatorInfo, ( CGraphBaseData::BARDRAWSTYLE)CBarDraw::GetDrawType(pIndicatorInfo, true), penThickness, baseLineStyle, bLog, bInvert,
			strRQ, pxScaleManager, const_cast<CSubGraphData&>(subGraphData), drawingGraphData.GetColumnIndex());
	case CGraphBaseData::BoxBar:// 박스식
		return DrawBoxBar(pDC, realRegion, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, pIndicatorInfo, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
	case CGraphBaseData::DotBar:// 도트식
		return DrawDotBar(pDC, realRegion, drawStyle, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
	}
	return false;
}

// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 한다.
bool CSingleBarDraw::DrawVerticalBar(CDC* pDC, const CRect& graphRegion, const CString& GraphName, 
		const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, CGraphColor* pPriceChartColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, CIndicatorInfo* pIndicatorInfo, 
		const CPenThickness& penThickness, bool bLog, bool bInvert, CIndicatorList *pIndicatorList)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;

	CPacket* pVolumePacket = packetList.GetNext(pos);
	if(!pVolumePacket)	return false;

	int nType = CBarDraw::GetDrawType(pIndicatorInfo, true);
	CPacket* pOpenPacket = NULL;
	CPacket* pClosePacket = NULL;
	if(nType > 0)
	{
		pOpenPacket = pVolumePacket->GetPacketList()->GetBaseData( _MTEXT( C0_OPEN));
		pClosePacket = pVolumePacket->GetPacketList()->GetBaseData( _MTEXT( C0_CLOSE));
	}

	if(DrawVerticalBar_VolumeChart(pDC, pVolumePacket, pOpenPacket, pClosePacket, pIndicatorInfo, 
		nType, drawStyle.GetGraphBarDrawStyle(), graphRegion, graphColor, pPriceChartColor, 
		nBarWidth, dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, penThickness, bLog, bInvert, pIndicatorList))
		return true;

	return DrawVerticalBar_General(pDC, pVolumePacket, drawStyle.GetGraphBarDrawStyle(),
			graphRegion, graphColor, nBarWidth, dRealWidth, nRealStart, nRealEnd,
			dViewMax, dViewMin, penThickness, bLog, bInvert);
}

// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 한다.
bool CSingleBarDraw::DrawVerticalBar(CDC* pDC, const CRect& graphRegion, const CString& GraphName, 
		const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, CGraphColor* pPriceChartColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, CIndicatorInfo* pIndicatorInfo, 
		const CPenThickness& penThickness, bool bLog, bool bInvert, CIndicatorList *pIndicatorList,
		const CString& strRQ, CXScaleManager* pxScaleManager, const CSubGraphData& subGraphData, int p_nColumn, CMainBlock *pMainBlock)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;

	CPacket* pVolumePacket = packetList.GetNext(pos);
	if(!pVolumePacket)	return false;

	int nType = CBarDraw::GetDrawType(pIndicatorInfo, true);
	CPacket* pOpenPacket = NULL;
	CPacket* pClosePacket = NULL;
	if(nType > 0)
	{
		pOpenPacket = pVolumePacket->GetPacketList()->GetBaseData( _MTEXT( C0_OPEN));
		pClosePacket = pVolumePacket->GetPacketList()->GetBaseData( _MTEXT( C0_CLOSE));

		// (2009/5/8 - Seung-Won, Bae) Check not candle.
		if( !pOpenPacket || !pClosePacket) nType = 0;
	}

	if(DrawVerticalBar_VolumeChart(pDC, pVolumePacket, pOpenPacket, pClosePacket, pIndicatorInfo, 
		nType, drawStyle.GetGraphBarDrawStyle(), graphRegion, graphColor, pPriceChartColor, 
		nBarWidth, dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, penThickness, bLog, bInvert, pIndicatorList, strRQ, pxScaleManager, subGraphData, p_nColumn, pMainBlock))
		return true;

	return DrawVerticalBar_General(pDC, pVolumePacket, drawStyle.GetGraphBarDrawStyle(),
			graphRegion, graphColor, nBarWidth, dRealWidth, nRealStart, nRealEnd,
			dViewMax, dViewMin, penThickness, bLog, bInvert,
 			strRQ, pxScaleManager, const_cast<CSubGraphData&>(subGraphData), p_nColumn, pIndicatorInfo);
}

// ----------------------------------------------------------------------------
// 일반적인 수직바
bool CSingleBarDraw::DrawVerticalBar_General(CDC* pDC, CPacket* pPacket, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle,
		const CRect& graphRegion, const CGraphColor& graphColor, const int nBarWidth, const double dRealWidth, 
		const int nStart, const int nEnd, const double& dViewMax, const double& dViewMin, 
		const CPenThickness& penThickness, const bool bLog, const bool bInvert)
{
	return DrawVerticalBar_General(pDC, pPacket, eBarDrawStyle, graphRegion, nBarWidth, dRealWidth, nStart, nEnd, 
		dViewMax, dViewMin, bLog, bInvert, penThickness, graphColor.GetColor1(), graphColor.GetColor2(), graphColor.GetUpGraphFill());
}

bool CSingleBarDraw::DrawVerticalBar_General(CDC* pDC, CPacket* pPacket, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle,
		const CRect& graphRegion, const CGraphColor& graphColor, const int nBarWidth, const double dRealWidth, 
		const int nStart, const int nEnd, const double& dViewMax, const double& dViewMin, 
		const CPenThickness& penThickness, const bool bLog, const bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn, CIndicatorInfo* pIndicatorInfo)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		//		Alloc max memory.
		int nDataCount = pxScaleManager->GetColumnWidth( p_nColumn)  * 5;
		if( eBarDrawStyle != CGraphBaseData::Bar_Draw) nDataCount = (nEnd - nStart + 1) * 5;

		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( nDataCount < 1) return true;

		// the change of Graph Style, Y Min/Max is xScaleMode 2 (Don't ask X Scale Manager !)
		int xScaleMode = 2;
		if(	subGraphData.m_pGraphCache)
		{	
			//Check YValue
			bool bYValue = subGraphData.m_pGraphCache->CheckYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);
			if(bYValue)
			{					
				if(pxScaleManager) xScaleMode = pxScaleManager->GetCalculationMode();
				if(xScaleMode == 0)	return true;
			}
		}	

		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		
		if( xScaleMode != 1)
		{
			DrawVerticalBar_ReallocCache( pxScaleManager, subGraphData.m_pGraphCache, eBarDrawStyle, nDataCount, nDTI_Start, nDTI_End, p_nColumn, strRQ);

			// (2010/4/18 - Seung-Won, Bae) Use Steady Color for Volume Bar Chart.
			subGraphData.m_pGraphCache->nCount_Bohap2 = 0;
			if( pIndicatorInfo) if( pIndicatorInfo->GetIndicatorName() == _MTEXT( C2_VOLUME_CHART))
				subGraphData.m_pGraphCache->nCount_Bohap2 = 2;
		}
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		pCalcData->SetYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);

		// (2009/7/27 - Seung-Won, Bae) Do not use nDTI_Start on xScaleMode 1.
		//		Use nDTI_Start_LastPixel
		if( xScaleMode != 1) pCalcData->nDTI_Start_LastPixel = nDTI_Start;
		if( pCalcData->nDTI_Start_LastPixel == -1) return true;

		CList<double, double>* pVolumeList = NULL;
		CList<double, double>* pOpenList = NULL;
		CList<double, double>* pCloseList = NULL;
		POSITION volumePos(NULL), openPos(NULL), closePos(NULL);
		int nDrawingIndex = 0;
		if( !GetDatasInPacket(pPacket, pCalcData->nDTI_Start_LastPixel, nDrawingIndex,
			pVolumeList, volumePos))
			return false;

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
		// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
		CRect rect;
		if(bInvert) rect.bottom = graphRegion.top;
		else		rect.bottom = graphRegion.bottom;
		// (2009/2/3 - Seung-Won, Bae) Show non-zero volume.
		rect.bottom -= 1;

		double dViewBottom = graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dRealHeight = dRealMax - dRealMin;
		int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();

		double dCurVolume = 0.0, dPreVolume = 0.0, dViewY = 0.0, dCurOpen = 0.0, dCurClose = 0.0;
		GetPrevData( pVolumeList, volumePos, 1, dPreVolume);

		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( pCalcData->nDTI_Start_LastPixel, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		int nIndex = 0;
		int nIndex_High  = 0;	// 상승 좌표 총개수
		int nIndex_Low   = 0;	// 하락 좌표 총개수
		int nIndex_Bohap = 0;	// 보합 좌표 총개수
		int nMaxTop = INT_MAX;	// 병합시의 최대값.
		int	nPreLeft = 0.0;		// 이전 좌표
		int nLastInsert = 0;    //0:상승 1:하락   마지막 Insert된 Data
		bool IsOverlap = false;

		if( xScaleMode == 1)
		{
			//이전 데이터의 마지막 픽셀 시작되는 DTI 정보 
			openPos			= NULL;
			closePos		= NULL;
			nIndex_High		= pCalcData->nLastPixel_high;
			nIndex_Low		= pCalcData->nLastPixel_Low;
			nIndex_Bohap	= pCalcData->nLastPixel_Bohap;
			pCalcData->nCount_High		= pCalcData->nLastPolyCnt_high;
			pCalcData->nCount_Low		= pCalcData->nLastPolyCnt_Low;
			pCalcData->nCount_Bohap	= pCalcData->nLastPolyCnt_Bohap;
		}

		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		double *daCurOpen = pCalcData->Get_dCurOpen();
		double *daCurClose = pCalcData->Get_dCurClose();
		double *daCurVolume = pCalcData->Get_dCurVolume();
		CRect *rtGraph = pCalcData->Get_rtGraph();

		for( nDTI = pCalcData->nDTI_Start_LastPixel; nDTI <= nDTI_End; nDTI++)
		{
			if( volumePos == NULL) break;
			parea = pblaAreafDTI->GetNext( dblCursor);
			
			if(parea == NULL) break;
			
			dCurVolume	= CDraw::CalLogValue(bLog, pVolumeList->GetNext( volumePos));
			dCurOpen	= 0.0;
			dCurClose	= 0.0;

			dViewY = dViewBottom - (dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
			rect.top = ROUND(dViewY);
			
			rect.left = parea->left;
			rect.right = parea->right;
			
			// Check Max Value on same pixel.
			if( nPreLeft == rect.left && nMaxTop < rect.top) 
			{					
				IsOverlap = true;
				continue;
			}

			//새로운 픽셀이 시작되는 시점의 정보 
			pCalcData->nDTI_Start_LastPixel = nDTI;
			pCalcData->nLastPixel_high		= nIndex_High;
			pCalcData->nLastPixel_Low		= nIndex_Low;
			pCalcData->nLastPixel_Bohap		= nIndex_Bohap;
			pCalcData->nLastPolyCnt_high	= pCalcData->nCount_High;
			pCalcData->nLastPolyCnt_Low		= pCalcData->nCount_Low;
			pCalcData->nLastPolyCnt_Bohap	= pCalcData->nCount_Bohap;

			nPreLeft = rect.left;
			nMaxTop = rect.top;

			// (2009/2/3 - Seung-Won, Bae) Check no volume.
			if( 0 == dCurVolume)
			{
				if( nDTI == nDTI_End) nLastInsert = -1;
				dPreVolume = dCurVolume;
				continue;
			}
			
			if(bInvert)
			{
				rect.top = graphRegion.top + ROUND(dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
				rect.top -= 1;
			}	
			
			//그래프 계산 결과 저장		
			if(eBarDrawStyle == CGraphBaseData::Bar_Draw)
			{
				//기존에 Rectangle로 그렸기 때문에 맞춰주기 위해서 right를 한 픽셀 줄인다.
				if( rect.left < rect.right) rect.right -= 1;
				
				if(rect.top < 0) rect.top = ROUND(dViewBottom - dViewHeight);
				
				// (2010/4/18 - Seung-Won, Bae) Use Steady Color for Volume Bar Chart.
				nLastInsert = subGraphData.m_pGraphCache->nCount_Bohap2;

				if( !DrawVerticalBar_AddBar( nLastInsert, pCalcData, nIndex_High, nIndex_Low, nIndex_Bohap, rect)) break;
			}
			else
			{
				if(nIndex == 0) pCalcData->dPreVolume	= dPreVolume;			
				// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
				daCurOpen[ nIndex]						= 0;
				daCurClose[ nIndex]						= 0;
				daCurVolume[ nIndex]					= dCurVolume;
				rtGraph[ nIndex]						= rect;
				nIndex++;
			}
			dPreVolume = dCurVolume;
		}	
		//<<20100521_JS.Kim  Bohap을 안넣으면 실시간 들어올 때 계산이 잘못되어 봉들이 앞으로 하나씩 지워진다.
		//DrawVerticalBar_NotOverlapDataSetting( pCalcData, nLastInsert, nDTI, IsOverlap, nIndex_High, 0, 0, graphRegion.right - graphRegion.left, nDTI_End - nDTI_Start);
		DrawVerticalBar_NotOverlapDataSetting( pCalcData, nLastInsert, nDTI, IsOverlap, nIndex_High, nIndex_Low, nIndex_Bohap, graphRegion.right - graphRegion.left, nDTI_End - nDTI_Start);
		//>>
		pCalcData->nDTI_Start  = nDTI_Start; //차트 성능 개선 관련 - Data 저장
		pCalcData->nDTI_End    = nDTI_End;
		pCalcData->nMaxTop		= 0;
		pCalcData->nPreLeft	= 0;
	}
	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{
		if( !subGraphData.m_pGraphCache) return true;
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;

		CBrush brushArray[BRUSH_COUNT];
		CBrush* pOldBrush = pDC->GetCurrentBrush();
		CPen penArray[PEN_COUNT];
		CPen* pOldPen = pDC->GetCurrentPen();
		CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);
//		// <가격차트> color 이 있다면 가격차트의 색을 가지고 와서 pen과 brush를 만든다.
//		if(pPriceChartColor != NULL)
//			CreateBrushsAndPens(brushArray, penArray, *pPriceChartColor, dRealWidth, penThickness, false);
//		else
//			CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);
//
//		// (2006/12/6 - Seung-Won, Bae) Get Candle Steady Type directly.
//		int nBohapInBong = 1;
//		if( pIndicatorList)
//		{			
//			CIndicatorInfo *pPriceIndiInfo = pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART));
//			if( pPriceIndiInfo) pPriceIndiInfo = pIndicatorList->GetDefaultIndicatorInfo( EILT_USER_DEFAULT, _MTEXT( C2_PRICE_CHART), EIIGT_WITH_DEFAULT_GET);
//			if( pPriceIndiInfo) nBohapInBong = CBongDraw::GetBohapType( pPriceIndiInfo);
//		}	

		if(eBarDrawStyle == CGraphBaseData::Bar_Draw)
			DrawVerticalBar_DrawPolygon( pCalcData, pDC, graphColor, brushArray, penArray);
		else
		{			
			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			double *daCurVolume = pCalcData->Get_dCurVolume();
			CRect *rtGraph = pCalcData->Get_rtGraph();

			int nIndex = 0;
			for(int nDTI = pCalcData->nDTI_Start; nDTI <= pCalcData->nDTI_End; nDTI++)
			{
				SelectGdiObject_HighLow(pDC, graphColor.GetUpGraphFill(), UP_BRUSH, UP_PEN, brushArray, penArray);
				// cj 2003.5.14 NonZero좌표점형 처리
				if( !(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && daCurVolume[nIndex] == 0) )
					DrawVerticalBar(pDC, eBarDrawStyle, rtGraph[nIndex], nBarWidth);
				
				nIndex++;
			}
		}
		DeleteBrushsAndPens(pDC, brushArray, penArray, pOldBrush, pOldPen);
	}
	return true;
}

bool CSingleBarDraw::DrawVerticalBar_General(CDC* pDC, CPacket* pPacket, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle,
		const CRect& graphRegion, const int nBarWidth, const double dRealWidth, int nStart, int nEnd, 
		double dRealMax, double dRealMin, const bool bLog, const bool bInvert, 
		const CPenThickness& penThickness, const COLORREF& penColor, const COLORREF& brushColor, const bool bIsFill)
{
	if(pPacket == NULL || pPacket->GetnumericDataList() == NULL)
		return false;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL)	return false;
	POSITION curPos = pDataList->FindIndex(nStart);
	if(curPos == NULL) 
		return false;

	CBrush upBrush(brushColor);
	CBrush* pOldBrush = NULL;
	if(bIsFill)
		pOldBrush = pDC->SelectObject(&upBrush);
	else
		pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

	CPen upPen;
	int nLineThick = CDraw::CalUpLineThickness(penThickness, nBarWidth);
	upPen.CreatePen(PS_SOLID, GetThickness(nLineThick, dRealWidth), penColor);
	CPen* pOldPen = pDC->SelectObject(&upPen);

	double dPreData = 0.0;
	POSITION prePos = curPos;
	pDataList->GetPrev(prePos);
	if(prePos != NULL)
		dPreData = pDataList->GetAt(prePos);

	dRealMin = CDraw::CalLogValue(bLog, dRealMin);
	dRealMax = CDraw::CalLogValue(bLog, dRealMax);

	double dViewBottom = graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	double dRealHeight = dRealMax - dRealMin;
	int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();

	double dViewX_Center = graphRegion.left + dRealWidth / 2;
	double dY, dViewY;
	CRect rect;
	rect.bottom = graphRegion.bottom;

	if(pPacket->GetStartPeriod() > nStart)	nStart = pPacket->GetStartPeriod();
	if(pDataList->GetCount() <= nEnd)		nEnd = pDataList->GetCount() - 1;

	if(bInvert) rect.bottom = graphRegion.top;
	else		rect.bottom = graphRegion.bottom;

	// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
	// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
	int nHalfWith = int( dRealWidth - 2) / 2;
	if( 3.0 <= dRealWidth && nHalfWith < 1) nHalfWith = 1;
	int nViewX_Center = 0;

	for(int i = nStart; i <= nEnd; i++)
	{
		nViewX_Center = ROUND( dViewX_Center);
		
		dY = pDataList->GetNext(curPos);
		if( bLog) dY = CMath::Log( dY);
		dViewY = dViewBottom - (dViewHeight * ((dY - dRealMin) / dRealHeight));
		rect.top = ROUND(dViewY);
		rect.left = nViewX_Center - nHalfWith;
		rect.right = nViewX_Center + nHalfWith + 1;

		if(bInvert) rect.top = nViewY_half2 - rect.top;

		// cj 2003.5.14 NonZero좌표점형 처리
		if(!(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && dY == 0))
			DrawVerticalBar(pDC, eBarDrawStyle, rect, nBarWidth);

		dViewX_Center += dRealWidth;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	upBrush.DeleteObject();
	upPen.DeleteObject();
	return true;
}

bool CSingleBarDraw::DrawVerticalBar_General(CDC* pDC, CPacket* pPacket, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle,
		const CRect& graphRegion, const int nBarWidth, const double dRealWidth, int nStart, int nEnd, 
		double dRealMax, double dRealMin, const bool bLog, const bool bInvert, 
		const CPenThickness& penThickness, const COLORREF& penColor, const COLORREF& brushColor, const bool bIsFill,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{
		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		if(pPacket == NULL || pPacket->GetnumericDataList() == NULL)
			return false;

		CList<double, double>* pDataList = pPacket->GetnumericDataList();
		if(pDataList == NULL)	return false;
		POSITION curPos = pDataList->FindIndex(nDTI_Start);
		if(curPos == NULL) 
			return false;

		CBrush upBrush(brushColor);
		CBrush* pOldBrush = NULL;
		if(bIsFill)
			pOldBrush = pDC->SelectObject(&upBrush);
		else
			pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

		CPen upPen;
		int nLineThick = CDraw::CalUpLineThickness(penThickness, nBarWidth);
		upPen.CreatePen(PS_SOLID, GetThickness(nLineThick, dRealWidth), penColor);
		CPen* pOldPen = pDC->SelectObject(&upPen);

		double dPreData = 0.0;
		POSITION prePos = curPos;
		pDataList->GetPrev(prePos);
		if(prePos != NULL)
			dPreData = pDataList->GetAt(prePos);
		
	//	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dPreData), false);

		if(bLog)
		{
			dRealMin = CMath::Log(dRealMin);
			dRealMax = CMath::Log(dRealMax);
		}

		double dViewBottom = graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dRealHeight = dRealMax - dRealMin;
		int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();
		double dY, dViewY;

		CRect rect;
		rect.bottom = graphRegion.bottom;

		if(pPacket->GetStartPeriod() > nStart)	nStart = pPacket->GetStartPeriod();
		if(pDataList->GetCount() <= nEnd)		nEnd = pDataList->GetCount() - 1;

		if(bInvert) rect.bottom = graphRegion.top;
		else		rect.bottom = graphRegion.bottom;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		if(dRealWidth >= 1.0 || eBarDrawStyle != CGraphBaseData::Bar_Draw)
		{
			for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
			{
		//		pxScaleManager->GetAt_DrawArea(nDTI, &parea);

				// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
				//	pxScaleManager->NextDrawArea(&parea);
				parea = pblaAreafDTI->GetNext( dblCursor);
						
				if(parea == NULL) break;
				if(curPos == NULL) break;

				dY = pDataList->GetNext(curPos);
				
				if(bLog) dY = CMath::Log(dY);

				dViewY = dViewBottom - (dViewHeight * ((dY - dRealMin) / dRealHeight));

				rect.top = ROUND(dViewY);
				rect.left = parea->left;
				rect.right = parea->right;
				
				if(bInvert) rect.top = nViewY_half2 - rect.top;

				// cj 2003.5.14 NonZero좌표점형 처리
				if(!(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && dY == 0))
				{
					DrawVerticalBar(pDC, eBarDrawStyle, rect, nBarWidth);
				}
			}
		}
		else
		{
			int nIdxLine = 0;
			POINT* lpptLine = new POINT[(nDTI_End + 1) * 2];
			DWORD* lpdwcntPoly = new DWORD[(nDTI_End + 1)];

			for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
			{
	//			pxScaleManager->GetAt_DrawArea(nDTI, &parea);

				// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
				//	pxScaleManager->NextDrawArea(&parea);
				parea = pblaAreafDTI->GetNext( dblCursor);
		
				if(parea == NULL) break;
				if(curPos == NULL) break;
				
				dY = pDataList->GetNext(curPos);
				
				if(bLog) dY = CMath::Log(dY);
				
				dViewY = dViewBottom - (dViewHeight * ((dY - dRealMin) / dRealHeight));
				
				rect.top = ROUND(dViewY);
				rect.left = parea->left;
				rect.right = parea->right;
				
				if(bInvert) rect.top = nViewY_half2 - rect.top;
				
				lpdwcntPoly[nIdxLine / 2] = 2;
				
				if(rect.top == rect.bottom)
				{
					if(bInvert) rect.top++;
					else		rect.top--;
				}

				lpptLine[nIdxLine].x = rect.right;
				lpptLine[nIdxLine].y = rect.top;
				nIdxLine++;

				lpptLine[nIdxLine].x = rect.right;
				lpptLine[nIdxLine].y = rect.bottom;
				nIdxLine++;

	/*
				// cj 2003.5.14 NonZero좌표점형 처리
				if(!(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && dY == 0))
				{
					DrawVerticalBar(pDC, eBarDrawStyle, rect, nBarWidth);
				}
	*/
			}

			if(nIdxLine != 0) pDC->PolyPolyline( lpptLine, lpdwcntPoly, nIdxLine / 2);

			delete [] lpptLine;
			delete [] lpdwcntPoly;
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		upBrush.DeleteObject();
		upPen.DeleteObject();
	}
// 	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	return true;
}

// "거래량차트", "거래대금" 인 경우
// -> nType : 1 ~ 3 , nType == 3 이면 "시가", "종가"가 있어야함
// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 한다.
bool CSingleBarDraw::DrawVerticalBar_VolumeChart(CDC* pDC, CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const int nType, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, 
		const CRect& graphRegion, const CGraphColor& graphColor, CGraphColor* pPriceChartColor, 
		const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, const double& dViewMax, 
		const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList)
{
	if(!CBarDraw::DoesVolumeDrawingType(pIndicatorInfo))
		return false;

	switch(nType)
	{
	case 0:
	case 1:	// 일반
		return DrawVerticalBar_General(pDC, pVolumePacket, eBarDrawStyle, graphRegion, 
			nBarWidth, dRealWidth, nStart, nEnd, dViewMax, dViewMin, bLog, bInvert, 
			penThickness, graphColor.GetColor5(), graphColor.GetColor6(), graphColor.GetUpGraphFill());
	case 2:	// 상승하락(거래량)
		return DrawVerticalBar_VolumeChart_HighLow(pDC, pVolumePacket, eBarDrawStyle, 
			graphRegion, graphColor, nBarWidth, dRealWidth, nStart, nEnd, 
			dViewMax, dViewMin, penThickness, bLog, bInvert);
	case 3:	// 가격차트처럼
		return DrawVerticalBar_VolumeChart_Price(pDC, pVolumePacket, pOpenPacket, pClosePacket, 
			pIndicatorInfo, eBarDrawStyle, graphRegion, graphColor, pPriceChartColor, 
			nBarWidth, dRealWidth, nStart, nEnd, dViewMax, dViewMin, penThickness, bLog, bInvert, pIndicatorList);
	case 4: // 전일종가대비
		return DrawVerticalBar_VolumeChart_PrevPrice(pDC, pVolumePacket, pClosePacket, 
			pIndicatorInfo, eBarDrawStyle, graphRegion, graphColor, pPriceChartColor, 
			nBarWidth, dRealWidth, nStart, nEnd, dViewMax, dViewMin, penThickness, bLog, bInvert, pIndicatorList);
	}

	return false;
}

bool CSingleBarDraw::DrawVerticalBar_VolumeChart(CDC* pDC, CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const int nType, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, 
		const CRect& graphRegion, const CGraphColor& graphColor, CGraphColor* pPriceChartColor, 
		const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, const double& dViewMax, 
		const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList,
		const CString& strRQ, CXScaleManager* pxScaleManager, const CSubGraphData& subGraphData, int p_nColumn, CMainBlock *pMainBlock)
{
	if(!CBarDraw::DoesVolumeDrawingType(pIndicatorInfo))
		return false;

	switch(nType)
	{
	case 0:
	case 1:	// 상승하락(거래량)
		return DrawVerticalBar_General(pDC, pVolumePacket, eBarDrawStyle, 
			graphRegion, graphColor, nBarWidth, dRealWidth, nStart, nEnd, 
			dViewMax, dViewMin, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, const_cast<CSubGraphData&>(subGraphData), p_nColumn, pIndicatorInfo);
// 	case 1:	// 일반
// 		return DrawVerticalBar_General(pDC, pVolumePacket, eBarDrawStyle, graphRegion, 
// 			nBarWidth, dRealWidth, nStart, nEnd, dViewMax, dViewMin, bLog, bInvert, 
// 			penThickness, graphColor.GetColor5(), graphColor.GetColor6(), graphColor.GetUpGraphFill(), strRQ, pxScaleManager,
// 			const_cast<CSubGraphData&>(subGraphData));
	case 2:	// 상승하락(거래량)
		return DrawVerticalBar_VolumeChart_HighLow(pDC, pVolumePacket, eBarDrawStyle, 
			graphRegion, graphColor, nBarWidth, dRealWidth, nStart, nEnd, 
			dViewMax, dViewMin, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, const_cast<CSubGraphData&>(subGraphData), p_nColumn);
	case 3:	// 가격차트처럼
		return DrawVerticalBar_VolumeChart_Price(pDC, pVolumePacket, pOpenPacket, pClosePacket, 
			pIndicatorInfo, eBarDrawStyle, graphRegion, graphColor, pPriceChartColor, 
			nBarWidth, dRealWidth, nStart, nEnd, dViewMax, dViewMin, penThickness, bLog, bInvert, pIndicatorList,
			strRQ, pxScaleManager, const_cast<CSubGraphData&>(subGraphData), p_nColumn, pMainBlock);
	case 4: // 전일종가대비
		return DrawVerticalBar_VolumeChart_PrevPrice(pDC, pVolumePacket, pClosePacket, 
			pIndicatorInfo, eBarDrawStyle, graphRegion, graphColor, pPriceChartColor, 
			nBarWidth, dRealWidth, nStart, nEnd, dViewMax, dViewMin, penThickness, bLog, bInvert, pIndicatorList,
			strRQ, pxScaleManager, const_cast<CSubGraphData&>(subGraphData), p_nColumn);
	}

	return false;
}
	 
// 상승하락 거래량
bool CSingleBarDraw::DrawVerticalBar_VolumeChart_HighLow(CDC* pDC, CPacket* pVolumePacket, 
		const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, const double& dViewMax, const double& dViewMin, 
		const CPenThickness& penThickness, const bool bLog, const bool bInvert)
{
	CList<double, double>* pVolumeList = NULL;
	POSITION volumePos = NULL;
	int nDrawingIndex = 0;
	if(!GetDatasInPacket(pVolumePacket, nStart, nDrawingIndex, pVolumeList, volumePos))
		return false;

	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
	// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
	CRect rect;
	if(bInvert) rect.bottom = graphRegion.top;
	else		rect.bottom = graphRegion.bottom;
	double dViewBottom = graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	double dRealHeight = dRealMax - dRealMin;
	int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();
	double dViewX_Center = graphRegion.left + dRealWidth / 2;
	int nHalfWith = int( dRealWidth - 2) / 2;
	if( 3.0 <= dRealWidth && nHalfWith < 1) nHalfWith = 1;

	int nViewX_Center = 0;
	double dCurVolume = 0.0, dPreVolume = 0.0, dViewY = 0.0;
	GetPrevData(pVolumeList, volumePos, 1, dPreVolume);
	while(volumePos != NULL && nDrawingIndex <= (nEnd - nStart))
	{
		nViewX_Center = ROUND( dViewX_Center);

		dCurVolume = pVolumeList->GetNext( volumePos);
		if( bLog) dCurVolume = CMath::Log( dCurVolume);

		dViewY = dViewBottom - (dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
		rect.top = ROUND(dViewY);
		rect.left = nViewX_Center - nHalfWith;
		rect.right = nViewX_Center + nHalfWith + 1;

		if(bInvert) rect.top = nViewY_half2 - rect.top;

		SelectGdiObject_VolumeChart(pDC, dCurVolume, dPreVolume, graphColor, brushArray, penArray);

		// cj 2003.5.14 NonZero좌표점형 처리
		if( !(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && dCurVolume == 0) )
			DrawVerticalBar(pDC, eBarDrawStyle, rect, nBarWidth);

		dPreVolume = dCurVolume;
		dViewX_Center += dRealWidth;
		nDrawingIndex++;
	}

	DeleteBrushsAndPens(pDC, brushArray, penArray, pOldBrush, pOldPen);
	return true;
}

bool CSingleBarDraw::DrawVerticalBar_VolumeChart_HighLow(CDC* pDC, CPacket* pVolumePacket, 
		const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, const double& dViewMax, const double& dViewMin, 
		const CPenThickness& penThickness, const bool bLog, const bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		//		Alloc max memory.
		int nDataCount = pxScaleManager->GetColumnWidth( p_nColumn)  * 5;
		if( eBarDrawStyle != CGraphBaseData::Bar_Draw) nDataCount = (nEnd - nStart + 1) * 5;

		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( nDataCount < 1) return true;

		// the change of Graph Style, Y Min/Max is xScaleMode 2 (Don't ask X Scale Manager !)
		int xScaleMode = 2;
		if(	subGraphData.m_pGraphCache)
		{	
			//Check YValue
			bool bYValue = subGraphData.m_pGraphCache->CheckYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);
			if(bYValue)
			{					
				if(pxScaleManager) xScaleMode = pxScaleManager->GetCalculationMode();
				if(xScaleMode == 0)	return true;
			}
		}	

		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		
		if( xScaleMode != 1) DrawVerticalBar_ReallocCache( pxScaleManager, subGraphData.m_pGraphCache, eBarDrawStyle, nDataCount, nDTI_Start, nDTI_End, p_nColumn, strRQ);
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		pCalcData->SetYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);

		// (2009/7/27 - Seung-Won, Bae) Do not use nDTI_Start on xScaleMode 1.
		//		Use nDTI_Start_LastPixel
		if( xScaleMode != 1) pCalcData->nDTI_Start_LastPixel = nDTI_Start;
		if( pCalcData->nDTI_Start_LastPixel == -1) return true;
		
		CList<double, double>* pVolumeList = NULL;
		CList<double, double>* pOpenList = NULL;
		CList<double, double>* pCloseList = NULL;
		POSITION volumePos(NULL), openPos(NULL), closePos(NULL);
		int nDrawingIndex = 0;
		if( !GetDatasInPacket( pVolumePacket, pCalcData->nDTI_Start_LastPixel, nDrawingIndex,
			pVolumeList, volumePos))
			return false;

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
		// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
		CRect rect;
		if(bInvert) rect.bottom = graphRegion.top;
		else		rect.bottom = graphRegion.bottom;
		// (2009/2/3 - Seung-Won, Bae) Show non-zero volume.
		rect.bottom -= 1;

		double dViewBottom = graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dRealHeight = dRealMax - dRealMin;
		int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();

		double dCurVolume = 0.0, dPreVolume = 0.0, dViewY = 0.0, dCurOpen = 0.0, dCurClose = 0.0;
		GetPrevData( pVolumeList, volumePos, 1, dPreVolume);

		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( pCalcData->nDTI_Start_LastPixel, nZeroDTI, dblCursor);
		
		CXScaleManager::PAREA parea;

		int nIndex = 0;
		int nIndex_High  = 0;	// 상승 좌표 총개수
		int nIndex_Low   = 0;	// 하락 좌표 총개수
		int nIndex_Bohap = 0;	// 보합 좌표 총개수
		int nMaxTop = INT_MAX;	// 병합시의 최대값.
		int	nPreLeft = 0.0;		// 이전 좌표
		int nLastInsert = 0;    //0:상승, 1:하락   마지막 Insert된 Data
		bool IsOverlap = false;

		if( xScaleMode == 1)
		{
			//이전 데이터의 마지막 픽셀 시작되는 DTI 정보 
			openPos			= NULL;
			closePos		= NULL;
			nIndex_High		= pCalcData->nLastPixel_high;
			nIndex_Low		= pCalcData->nLastPixel_Low;
			nIndex_Bohap	= pCalcData->nLastPixel_Bohap;
			pCalcData->nCount_High		= pCalcData->nLastPolyCnt_high;
			pCalcData->nCount_Low		= pCalcData->nLastPolyCnt_Low;
			pCalcData->nCount_Bohap	= pCalcData->nLastPolyCnt_Bohap;
		}

		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		double *daCurOpen = pCalcData->Get_dCurOpen();
		double *daCurClose = pCalcData->Get_dCurClose();
		double *daCurVolume = pCalcData->Get_dCurVolume();
		CRect *rtGraph = pCalcData->Get_rtGraph();

		for( nDTI = pCalcData->nDTI_Start_LastPixel; nDTI <= nDTI_End; nDTI++)
		{
			if( volumePos == NULL) break;
			parea = pblaAreafDTI->GetNext( dblCursor);
			
			if(parea == NULL) break;
			
			dCurVolume	= CDraw::CalLogValue(bLog, pVolumeList->GetNext( volumePos));
			dCurOpen	= 0.0;
			dCurClose	= 0.0;
			
			dViewY = dViewBottom - (dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
			rect.top = ROUND(dViewY);
			
			rect.left = parea->left;
			rect.right = parea->right;
			
			// Check Max Value on same pixel.
			if( nPreLeft == rect.left && nMaxTop < rect.top) 
			{						
				IsOverlap = true;
				continue;
			}

			//새로운 픽셀이 시작되는 시점의 정보 
			pCalcData->nDTI_Start_LastPixel = nDTI;
			pCalcData->nLastPixel_high		= nIndex_High;
			pCalcData->nLastPixel_Low		= nIndex_Low;
			pCalcData->nLastPixel_Bohap		= nIndex_Bohap;
			pCalcData->nLastPolyCnt_high	= pCalcData->nCount_High;
			pCalcData->nLastPolyCnt_Low		= pCalcData->nCount_Low;
			pCalcData->nLastPolyCnt_Bohap	= pCalcData->nCount_Bohap;

			nPreLeft = rect.left;
			nMaxTop = rect.top;

			// (2009/2/3 - Seung-Won, Bae) Check no volume.
			if( 0 == dCurVolume)
			{
				if( nDTI == nDTI_End) nLastInsert = -1;
				dPreVolume = dCurVolume;
				continue;
			}
			
			if(bInvert)
			{
				rect.top = graphRegion.top + ROUND(dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
				rect.top -= 1;
			}	
			
			//그래프 계산 결과 저장		
			if(eBarDrawStyle == CGraphBaseData::Bar_Draw)
			{
				//기존에 Rectangle로 그렸기 때문에 맞춰주기 위해서 right를 한 픽셀 줄인다.
				if( rect.left < rect.right) rect.right -= 1;
				
				if(rect.top < 0) rect.top = ROUND(dViewBottom - dViewHeight);
				
				if(dCurVolume > dPreVolume) //상승
					nLastInsert = 0;
				else
					nLastInsert = 1;

				if( !DrawVerticalBar_AddBar( nLastInsert, pCalcData, nIndex_High, nIndex_Low, nIndex_Bohap, rect)) break;
			}
			else
			{
				if(nIndex == 0) pCalcData->dPreVolume	= dPreVolume;
				// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
				daCurOpen[ nIndex]						= 0;
				daCurClose[ nIndex]						= 0;
				daCurVolume[nIndex]						= dCurVolume;
				rtGraph[nIndex]							= rect;
				nIndex++;
			}
			dPreVolume = dCurVolume;
		}
		//<<20100521_JS.Kim  Bohap을 안넣으면 실시간 들어올 때 계산이 잘못되어 봉들이 앞으로 하나씩 지워진다.
		//DrawVerticalBar_NotOverlapDataSetting(pCalcData, nLastInsert, nDTI, IsOverlap, nIndex_High, nIndex_Low, 0, graphRegion.right - graphRegion.left, nDTI_End - nDTI_Start);
		DrawVerticalBar_NotOverlapDataSetting(pCalcData, nLastInsert, nDTI, IsOverlap, nIndex_High, nIndex_Low, nIndex_Bohap, graphRegion.right - graphRegion.left, nDTI_End - nDTI_Start);
		//>>
		pCalcData->nDTI_Start	= nDTI_Start;
		pCalcData->nDTI_End	= nDTI_End;
		pCalcData->nMaxTop		= nMaxTop;
		pCalcData->nPreLeft	= nPreLeft;
	}
	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{
		if( !subGraphData.m_pGraphCache) return true;
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;

		CBrush brushArray[BRUSH_COUNT];
		CBrush* pOldBrush = pDC->GetCurrentBrush();
		CPen penArray[PEN_COUNT];
		CPen* pOldPen = pDC->GetCurrentPen();
		CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);
//		// <가격차트> color 이 있다면 가격차트의 색을 가지고 와서 pen과 brush를 만든다.
//		if(pPriceChartColor != NULL)
//			CreateBrushsAndPens(brushArray, penArray, *pPriceChartColor, dRealWidth, penThickness, false);
//		else
//			CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);
//
//		// (2006/12/6 - Seung-Won, Bae) Get Candle Steady Type directly.
//		int nBohapInBong = 1;
//		if( pIndicatorList)
//		{			
//			CIndicatorInfo *pPriceIndiInfo = pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART));
//			if( pPriceIndiInfo) pPriceIndiInfo = pIndicatorList->GetDefaultIndicatorInfo( EILT_USER_DEFAULT, _MTEXT( C2_PRICE_CHART), EIIGT_WITH_DEFAULT_GET);
//			if( pPriceIndiInfo) nBohapInBong = CBongDraw::GetBohapType( pPriceIndiInfo);
//		}	

		if(eBarDrawStyle == CGraphBaseData::Bar_Draw)
			DrawVerticalBar_DrawPolygon( pCalcData, pDC, graphColor, brushArray, penArray);
		else
		{	
			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			double *daCurVolume = pCalcData->Get_dCurVolume();
			CRect *rtGraph = pCalcData->Get_rtGraph();

			int dPreVolume = pCalcData->dPreVolume;
			int nIndex = 0;
			for(int nDTI = pCalcData->nDTI_Start; nDTI <= pCalcData->nDTI_End; nDTI++)
			{
				SelectGdiObject_VolumeChart(pDC, daCurVolume[nIndex], dPreVolume, graphColor, brushArray, penArray);
				// cj 2003.5.14 NonZero좌표점형 처리
				if( !(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && daCurVolume[nIndex] == 0) )
					DrawVerticalBar(pDC, eBarDrawStyle, rtGraph[nIndex], nBarWidth);
				
				dPreVolume = daCurVolume[ nIndex];
				nIndex++;
			}
		}
		DeleteBrushsAndPens(pDC, brushArray, penArray, pOldBrush, pOldPen);
	}
	return true;
}

// 가격적용 거래량
bool CSingleBarDraw::DrawVerticalBar_VolumeChart_Price(CDC* pDC, CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, 
		const CGraphColor& graphColor, CGraphColor* pPriceChartColor, const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, 
		const double& dViewMax, const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList)
{
	CList<double, double>* pVolumeList = NULL;
	CList<double, double>* pOpenList = NULL;
	CList<double, double>* pCloseList = NULL;
	POSITION volumePos(NULL), openPos(NULL), closePos(NULL);
	int nDrawingIndex = 0;
	if(!GetDatasInPacket(pVolumePacket, pOpenPacket, pClosePacket, nStart, nDrawingIndex, 
		pVolumeList, volumePos, pOpenList, openPos, pCloseList, closePos))
		return false;

	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	// <가격차트> color 이 있다면 가격차트의 색을 가지고 와서 pen과 brush를 만든다.
	if(pPriceChartColor != NULL)
		CreateBrushsAndPens(brushArray, penArray, *pPriceChartColor, dRealWidth, penThickness, false);
	else
		CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	// (2006/12/6 - Seung-Won, Bae) Get Candle Steady Type directly.
	int nBohapInBong = 1;
	if( pIndicatorList)
	{
		CIndicatorInfo *pPriceIndiInfo = pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART));
		if( pPriceIndiInfo) pPriceIndiInfo = pIndicatorList->GetDefaultIndicatorInfo( EILT_USER_DEFAULT, _MTEXT( C2_PRICE_CHART), EIIGT_WITH_DEFAULT_GET);
		if( pPriceIndiInfo) nBohapInBong = CBongDraw::GetBohapType( pPriceIndiInfo);
	}

	// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
	// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
	CRect rect;
	if(bInvert) rect.bottom = graphRegion.top;
	else		rect.bottom = graphRegion.bottom;
	double dViewBottom = graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	double dRealHeight = dRealMax - dRealMin;
	int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();
	double dViewX_Center = graphRegion.left + dRealWidth / 2;
	int nHalfWith = int( dRealWidth - 2) / 2;
	if( 3.0 <= dRealWidth && nHalfWith < 1) nHalfWith = 1;

	int nViewX_Center = 0;
	double dPreClose = 0.0, dViewY = 0.0;
	GetPrevData(pCloseList, closePos, 1, dPreClose);
	while( nDrawingIndex <= (nEnd - nStart))
	{
		if(volumePos == NULL || openPos == NULL || closePos == NULL) break;

		nViewX_Center = ROUND( dViewX_Center);

		double dCurVolume = CDraw::CalLogValue(bLog, pVolumeList->GetNext(volumePos));
		double dCurOpen = CDraw::CalLogValue(bLog, pOpenList->GetNext(openPos));
		double dCurClose = CDraw::CalLogValue(bLog, pCloseList->GetNext(closePos));

		if( bLog) dCurVolume = CMath::Log( dCurVolume);

		dViewY = dViewBottom - (dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
		rect.top = ROUND(dViewY);
		rect.left = nViewX_Center - nHalfWith;
		rect.right = nViewX_Center + nHalfWith + 1;

		if(bInvert) rect.top = nViewY_half2 - rect.top;

		SelectGdiObject_PriceChart(pDC, nBohapInBong, dCurOpen, dCurClose, dPreClose, graphColor, brushArray, penArray);

		// cj 2003.5.14 NonZero좌표점형 처리
		if( !(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && dCurVolume == 0) )
			DrawVerticalBar(pDC, eBarDrawStyle, rect, nBarWidth);

		dPreClose = dCurClose;
		dViewX_Center += dRealWidth;
		nDrawingIndex++;
	}

	DeleteBrushsAndPens(pDC, brushArray, penArray, pOldBrush, pOldPen);
	return true;
}

bool CSingleBarDraw::DrawVerticalBar_VolumeChart_Price(CDC* pDC, CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, 
		const CGraphColor& graphColor, CGraphColor* pPriceChartColor, const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, 
		const double& dViewMax, const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn, CMainBlock *pMainBlock)
{	
	if(pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		//		Alloc max memory.
		int nDataCount = pxScaleManager->GetColumnWidth( p_nColumn)  * 5;
		if( eBarDrawStyle != CGraphBaseData::Bar_Draw) nDataCount = (nEnd - nStart + 1) * 5;

		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( nDataCount < 1) return true;

		// the change of Graph Style, Y Min/Max is xScaleMode 2 (Don't ask X Scale Manager !)
		int xScaleMode = 2;
		if(	subGraphData.m_pGraphCache)
		{	
			//Check YValue
			bool bYValue = subGraphData.m_pGraphCache->CheckYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);
			if(bYValue)
			{					
				if(pxScaleManager) xScaleMode = pxScaleManager->GetCalculationMode();
				if(xScaleMode == 0) return true;
			}
		}			
		
		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);

		if( xScaleMode != 1)
		{
			DrawVerticalBar_ReallocCache( pxScaleManager, subGraphData.m_pGraphCache, eBarDrawStyle, nDataCount, nDTI_Start, nDTI_End, p_nColumn, strRQ);

			// (2010/4/18 - Seung-Won, Bae) for Proce Steady Color of Volume Chart.
			subGraphData.m_pGraphCache->nCount_Bohap2 = 1;
			if( pIndicatorList && pMainBlock)
			{
				CString strGraphName;
				int nR = -1, nC = -1, nV = -1, nG = -1;
				pMainBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, strGraphName, strRQ);
				if( !strGraphName.IsEmpty())
				{
					CIndicatorInfo *pPriceIndiInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, _MTEXT( C2_PRICE_CHART), EIIGT_WITHOUT_DEFAULT);
					if( pPriceIndiInfo) subGraphData.m_pGraphCache->nCount_Bohap2 = CBongDraw::GetBohapType( pPriceIndiInfo);
				}
			}	
		}
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		pCalcData->SetYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);

		// (2009/7/27 - Seung-Won, Bae) Do not use nDTI_Start on xScaleMode 1.
		//		Use nDTI_Start_LastPixel
		if( xScaleMode != 1) pCalcData->nDTI_Start_LastPixel = nDTI_Start;
		if( pCalcData->nDTI_Start_LastPixel == -1) return true;

		CList<double, double>* pVolumeList = NULL;
		CList<double, double>* pOpenList = NULL;
		CList<double, double>* pCloseList = NULL;
		POSITION volumePos(NULL), openPos(NULL), closePos(NULL);
		int nDrawingIndex = 0;
		if( !GetDatasInPacket(pVolumePacket, pOpenPacket, pClosePacket, pCalcData->nDTI_Start_LastPixel, nDrawingIndex, 
			pVolumeList, volumePos, pOpenList, openPos, pCloseList, closePos))
			return false;

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
		// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
		CRect rect;
		if(bInvert) rect.bottom = graphRegion.top;
		else		rect.bottom = graphRegion.bottom;
		// (2009/2/3 - Seung-Won, Bae) Show non-zero volume.
		rect.bottom -= 1;

		double dViewBottom = graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dRealHeight = dRealMax - dRealMin;
		int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();

		double dCurVolume = 0.0, dPreClose = 0.0, dViewY = 0.0, dCurOpen = 0.0, dCurClose = 0.0;
		GetPrevData( pCloseList, closePos, 1, dPreClose);

		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( pCalcData->nDTI_Start_LastPixel, nZeroDTI, dblCursor);

 		CXScaleManager::PAREA parea;

		int nIndex = 0;
		int nIndex_High  = 0;	// 상승 좌표 총개수
		int nIndex_Low   = 0;	// 하락 좌표 총개수
		int nIndex_Bohap = 0;	// 보합 좌표 총개수
		int nMaxTop = INT_MAX;	// 병합시의 최대값.
		int	nPreLeft = 0.0;		// 이전 좌표
		int nLastInsert = 0;    //0:상승, 1:하락 2:보합
		bool IsOverlap = false;
		
		if( xScaleMode == 1)
		{
			//이전 데이터의 마지막 픽셀 시작되는 DTI 정보 
			nIndex_High		= pCalcData->nLastPixel_high;
			nIndex_Low		= pCalcData->nLastPixel_Low;
			nIndex_Bohap	= pCalcData->nLastPixel_Bohap;
			pCalcData->nCount_High		= pCalcData->nLastPolyCnt_high;
			pCalcData->nCount_Low		= pCalcData->nLastPolyCnt_Low;
			pCalcData->nCount_Bohap	= pCalcData->nLastPolyCnt_Bohap;
		}

		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		double *daCurOpen = pCalcData->Get_dCurOpen();
		double *daCurClose = pCalcData->Get_dCurClose();
		double *daCurVolume = pCalcData->Get_dCurVolume();
		CRect *rtGraph = pCalcData->Get_rtGraph();

		for( nDTI = pCalcData->nDTI_Start_LastPixel; nDTI <= nDTI_End; nDTI++)
		{
			if( volumePos == NULL || openPos == NULL || closePos == NULL) break;
			parea = pblaAreafDTI->GetNext( dblCursor);
				
			if(parea == NULL) break;

			dCurVolume	= CDraw::CalLogValue(bLog, pVolumeList->GetNext(volumePos));
			dCurOpen	= CDraw::CalLogValue(bLog, pOpenList->GetNext(openPos));
			dCurClose	= CDraw::CalLogValue(bLog, pCloseList->GetNext(closePos));

			dViewY = dViewBottom - (dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
			rect.top = ROUND(dViewY);

			rect.left = parea->left;
			rect.right = parea->right;

			// Check Max Value on same pixel.
			if( nPreLeft == rect.left && nMaxTop < rect.top) 
			{					
				IsOverlap = true;
				continue;
			}

			//새로운 픽셀이 시작되는 시점의 정보 
			pCalcData->nDTI_Start_LastPixel = nDTI;
			pCalcData->nLastPixel_high		= nIndex_High;
			pCalcData->nLastPixel_Low		= nIndex_Low;
			pCalcData->nLastPixel_Bohap		= nIndex_Bohap;
			pCalcData->nLastPolyCnt_high	= pCalcData->nCount_High;
			pCalcData->nLastPolyCnt_Low		= pCalcData->nCount_Low;
			pCalcData->nLastPolyCnt_Bohap	= pCalcData->nCount_Bohap;

			nPreLeft = rect.left;
			nMaxTop = rect.top;

			// (2009/2/3 - Seung-Won, Bae) Check no volume.
			if( 0 == dCurVolume)
			{
				if( nDTI == nDTI_End) nLastInsert = -1;
				continue;
			}

			if(bInvert) 
			{
				rect.top = graphRegion.top + ROUND(dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
				rect.top -= 1;
			}
	
			//그래프 계산 결과 저장		
			if(eBarDrawStyle == CGraphBaseData::Bar_Draw)
			{					
				//기존에 Rectangle로 그렸기 때문에 맞춰주기 위해서 right를 한 픽셀 줄인다.
				if( rect.left < rect.right) rect.right -= 1;

				if(rect.top < 0) rect.top = ROUND(dViewBottom - dViewHeight);

				if(dCurOpen < dCurClose) //상승
					nLastInsert = 0;
				else if(dCurOpen > dCurClose) //하락
					nLastInsert = 1;
				// (2010/4/18 - Seung-Won, Bae) for Proce Steady Color of Volume Chart.
				else if( 2 == pCalcData->nCount_Bohap2) //보합
				{
					if( dPreClose < dCurClose) //상승
							nLastInsert = 0;
					else	nLastInsert = 1;
				}
				else nLastInsert = 2;

				if( !DrawVerticalBar_AddBar( nLastInsert, pCalcData, nIndex_High, nIndex_Low, nIndex_Bohap, rect)) break;
			}
			else
			{			
				if(nIndex == 0) pCalcData->dPreClose	= dPreClose;
				// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
				daCurOpen[ nIndex]						= dCurOpen;
				daCurClose[nIndex]						= dCurClose;
	 			daCurVolume[nIndex]						= dCurVolume;
	 			rtGraph[nIndex]							= rect;
				nIndex++;
			}
			dPreClose = dCurClose;
		}
		DrawVerticalBar_NotOverlapDataSetting(pCalcData, nLastInsert, nDTI, IsOverlap,  nIndex_High, nIndex_Low, nIndex_Bohap, graphRegion.right - graphRegion.left, nDTI_End - nDTI_Start);	
		pCalcData->nDTI_Start	= nDTI_Start;
		pCalcData->nDTI_End	= nDTI_End;
		pCalcData->nMaxTop		= nMaxTop;
		pCalcData->nPreLeft	= nPreLeft;
	}
	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{
		if( !subGraphData.m_pGraphCache) return true;
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;

		CBrush brushArray[BRUSH_COUNT];
		CBrush* pOldBrush = pDC->GetCurrentBrush(); 
		CPen penArray[PEN_COUNT];
		CPen* pOldPen = pDC->GetCurrentPen();
//		CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);
		// <가격차트> color 이 있다면 가격차트의 색을 가지고 와서 pen과 brush를 만든다.
		if(pPriceChartColor != NULL)
			CreateBrushsAndPens(brushArray, penArray, *pPriceChartColor, dRealWidth, penThickness, false);
		else
			CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

		// (2006/12/6 - Seung-Won, Bae) Get Candle Steady Type directly.
		int nBohapInBong = 1;
		if( pIndicatorList)
		{			
			CIndicatorInfo *pPriceIndiInfo = pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART));
			if( pPriceIndiInfo) pPriceIndiInfo = pIndicatorList->GetDefaultIndicatorInfo( EILT_USER_DEFAULT, _MTEXT( C2_PRICE_CHART), EIIGT_WITH_DEFAULT_GET);
			if( pPriceIndiInfo) nBohapInBong = CBongDraw::GetBohapType( pPriceIndiInfo);
		}	

		if(eBarDrawStyle == CGraphBaseData::Bar_Draw)
			DrawVerticalBar_DrawPolygon( pCalcData, pDC, graphColor, brushArray, penArray);
		else
		{	
			int dPreClose = pCalcData->dPreClose;
			int nIndex = 0;

			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			double *daCurOpen = pCalcData->Get_dCurOpen();
			double *daCurClose = pCalcData->Get_dCurClose();
			double *daCurVolume = pCalcData->Get_dCurVolume();
			CRect *rtGraph = pCalcData->Get_rtGraph();

			for(int nDTI = pCalcData->nDTI_Start; nDTI <= pCalcData->nDTI_End; nDTI++)
			{
				SelectGdiObject_PriceChart(pDC, nBohapInBong, daCurOpen[ nIndex], daCurClose[nIndex], dPreClose,	graphColor, brushArray, penArray);
				// cj 2003.5.14 NonZero좌표점형 처리
				if( !(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && daCurVolume[nIndex] == 0) )
					DrawVerticalBar(pDC, eBarDrawStyle, rtGraph[nIndex], nBarWidth);

				dPreClose = daCurClose[ nIndex];
				nIndex++;
			}
		}
		DeleteBrushsAndPens(pDC, brushArray, penArray, pOldBrush, pOldPen);	
	}	
	return true;
}

//sy 2005.11.02.
// 전일종가대비
bool CSingleBarDraw::DrawVerticalBar_VolumeChart_PrevPrice(CDC* pDC, CPacket* pVolumePacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, 
		const CGraphColor& graphColor, CGraphColor* pPriceChartColor, const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, 
		const double& dViewMax, const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList)
{
	CList<double, double>* pVolumeList = NULL;
	CList<double, double>* pCloseList = NULL;
	POSITION volumePos(NULL), closePos(NULL);
	int nDrawingIndex = 0;
	if(!GetDatasInPacket(pVolumePacket, pClosePacket, nStart, nDrawingIndex, 
		pVolumeList, volumePos, pCloseList, closePos))
		return false;

	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
	// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
	CRect rect;
	if(bInvert) rect.bottom = graphRegion.top;
	else		rect.bottom = graphRegion.bottom;
	double dViewBottom = graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	double dRealHeight = dRealMax - dRealMin;
	int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();
	double dViewX_Center = graphRegion.left + dRealWidth / 2;
	int nHalfWith = int( dRealWidth - 2) / 2;
	if( 3.0 <= dRealWidth && nHalfWith < 1) nHalfWith = 1;

	int nViewX_Center = 0;
	double dPreClose = 0.0, dViewY = 0.0;
	GetPrevData(pCloseList, closePos, 1, dPreClose);
	while(nDrawingIndex <= (nEnd - nStart))
	{
		if(volumePos == NULL || closePos == NULL) break;

		nViewX_Center = ROUND( dViewX_Center);

		double dCurVolume = CDraw::CalLogValue(bLog, pVolumeList->GetNext(volumePos));
		double dCurClose = CDraw::CalLogValue(bLog, pCloseList->GetNext(closePos));

		if( bLog) dCurVolume = CMath::Log( dCurVolume);

		dViewY = dViewBottom - (dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
		rect.top = ROUND(dViewY);
		rect.left = nViewX_Center - nHalfWith;
		rect.right = nViewX_Center + nHalfWith + 1;

		if(bInvert) rect.top = nViewY_half2 - rect.top;

		SelectGdiObject_VolumeChart(pDC, dCurClose, dPreClose, graphColor, brushArray, penArray);

		// cj 2003.5.14 NonZero좌표점형 처리
		if( !(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && dCurVolume == 0) )
			DrawVerticalBar(pDC, eBarDrawStyle, rect, nBarWidth);

		dPreClose = dCurClose;
		dViewX_Center += dRealWidth;
		nDrawingIndex++;
	}

	DeleteBrushsAndPens(pDC, brushArray, penArray, pOldBrush, pOldPen);
	return true;
}

bool CSingleBarDraw::DrawVerticalBar_VolumeChart_PrevPrice(CDC* pDC, CPacket* pVolumePacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, 
		const CGraphColor& graphColor, CGraphColor* pPriceChartColor, const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, 
		const double& dViewMax, const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		//		Alloc max memory.
		int nDataCount = pxScaleManager->GetColumnWidth( p_nColumn)  * 5;
		if( eBarDrawStyle != CGraphBaseData::Bar_Draw) nDataCount = (nEnd - nStart + 1) * 5;

		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( nDataCount < 1) return true;

		// the change of Graph Style, Y Min/Max is xScaleMode 2 (Don't ask X Scale Manager !)
		int xScaleMode = 2;
		if(	subGraphData.m_pGraphCache)
		{	
			//Check YValue
			bool bYValue = subGraphData.m_pGraphCache->CheckYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);
			if(bYValue)
			{					
				if(pxScaleManager) xScaleMode = pxScaleManager->GetCalculationMode();
				if(xScaleMode == 0)	return true;
			}
		}			

		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		
		if( xScaleMode != 1) DrawVerticalBar_ReallocCache( pxScaleManager, subGraphData.m_pGraphCache, eBarDrawStyle, nDataCount, nDTI_Start, nDTI_End, p_nColumn, strRQ);
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		pCalcData->SetYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);

		// (2009/7/27 - Seung-Won, Bae) Do not use nDTI_Start on xScaleMode 1.
		//		Use nDTI_Start_LastPixel
		if( xScaleMode != 1) pCalcData->nDTI_Start_LastPixel = nDTI_Start;
		if( pCalcData->nDTI_Start_LastPixel == -1) return true;
		
		CList<double, double>* pVolumeList = NULL;
		CList<double, double>* pOpenList = NULL;
		CList<double, double>* pCloseList = NULL;
		POSITION volumePos(NULL), openPos(NULL), closePos(NULL);
		int nDrawingIndex = 0;
		if( !GetDatasInPacket(pVolumePacket, pClosePacket, pCalcData->nDTI_Start_LastPixel, nDrawingIndex, 
			pVolumeList, volumePos, pCloseList, closePos))
			return false;

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
		// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
		CRect rect;
		if(bInvert) rect.bottom = graphRegion.top;
		else		rect.bottom = graphRegion.bottom;
		// (2009/2/3 - Seung-Won, Bae) Show non-zero volume.
		rect.bottom -= 1;

		double dViewBottom = graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dRealHeight = dRealMax - dRealMin;
		int nViewY_half2 = graphRegion.top + graphRegion.top + graphRegion.Height();
	
		double dCurVolume = 0.0, dPreClose = 0.0, dViewY = 0.0, dCurOpen = 0.0, dCurClose = 0.0;
		GetPrevData( pCloseList, closePos, 1, dPreClose);

		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( pCalcData->nDTI_Start_LastPixel, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		int nIndex = 0;
		int nIndex_High  = 0;	// 상승 좌표 총개수
		int nIndex_Low   = 0;	// 하락 좌표 총개수
		int nIndex_Bohap = 0;	// 보합 좌표 총개수
		int nMaxTop = INT_MAX;	// 병합시의 최대값.
		int	nPreLeft = 0.0;		// 이전 좌표
		int nLastInsert = 0;    //0:상승 1:하락   마지막 Insert된 Data
		bool IsOverlap = false;
		
		if( xScaleMode == 1)
		{
			//이전 데이터의 마지막 픽셀 시작되는 DTI 정보 
			openPos			= NULL;
			nIndex_High		= pCalcData->nLastPixel_high;
			nIndex_Low		= pCalcData->nLastPixel_Low;
			nIndex_Bohap	= pCalcData->nLastPixel_Bohap;
			pCalcData->nCount_High		= pCalcData->nLastPolyCnt_high;
			pCalcData->nCount_Low		= pCalcData->nLastPolyCnt_Low;
			pCalcData->nCount_Bohap	= pCalcData->nLastPolyCnt_Bohap;
		}

		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		double *daCurOpen = pCalcData->Get_dCurOpen();
		double *daCurClose = pCalcData->Get_dCurClose();
		double *daCurVolume = pCalcData->Get_dCurVolume();
		CRect *rtGraph = pCalcData->Get_rtGraph();

		for( nDTI = pCalcData->nDTI_Start_LastPixel; nDTI <= nDTI_End; nDTI++)
		{
			if( volumePos == NULL || closePos == NULL) break;
			parea = pblaAreafDTI->GetNext( dblCursor);

			if(parea == NULL) break;

			dCurVolume	= CDraw::CalLogValue(bLog, pVolumeList->GetNext(volumePos));
			dCurOpen	= 0.0;
			dCurClose	= CDraw::CalLogValue(bLog, pCloseList->GetNext(closePos));

			dViewY = dViewBottom - (dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
			rect.top = ROUND(dViewY);
	
			rect.left = parea->left;
			rect.right = parea->right;

			// Check Max Value on same pixel.
			if( nPreLeft == rect.left && nMaxTop < rect.top) 
			{					
				IsOverlap = true;
				continue;
			}

			//새로운 픽셀이 시작되는 시점의 정보 
			pCalcData->nDTI_Start_LastPixel = nDTI;
			pCalcData->nLastPixel_high		= nIndex_High;
			pCalcData->nLastPixel_Low		= nIndex_Low;
			pCalcData->nLastPixel_Bohap		= nIndex_Bohap;
			pCalcData->nLastPolyCnt_high	= pCalcData->nCount_High;
			pCalcData->nLastPolyCnt_Low		= pCalcData->nCount_Low;
			pCalcData->nLastPolyCnt_Bohap	= pCalcData->nCount_Bohap;

			nPreLeft = rect.left;
			nMaxTop = rect.top;

			// (2009/2/3 - Seung-Won, Bae) Check no volume.
			if( 0 == dCurVolume)
			{
				if( nDTI == nDTI_End) nLastInsert = -1;
				continue;
			}

			if(bInvert)
			{
				rect.top = graphRegion.top + ROUND(dViewHeight * ((dCurVolume - dRealMin) / dRealHeight));
				rect.top -= 1;
			}

			//그래프 계산 결과 저장
			if(eBarDrawStyle == CGraphBaseData::Bar_Draw)
			{		
				//기존에 Rectangle로 그렸기 때문에 맞춰주기 위해서 right를 한 픽셀 줄인다.
				if( rect.left < rect.right) rect.right -= 1;
				
				if(rect.top < 0) rect.top = ROUND(dViewBottom - dViewHeight);
				
				if( dCurClose > dPreClose) //상승
					nLastInsert = 0;
				else
					nLastInsert = 1;

				if( !DrawVerticalBar_AddBar( nLastInsert, pCalcData, nIndex_High, nIndex_Low, nIndex_Bohap, rect)) break;
			}
			else
			{
				if(nIndex == 0) pCalcData->dPreClose	= dPreClose;			
				// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
				daCurOpen[ nIndex]						= 0;
				daCurClose[nIndex]						= dCurClose;
				daCurVolume[nIndex]						= dCurVolume;
				rtGraph[nIndex]							= rect;
				nIndex++;
			}
			dPreClose = dCurClose;
		}
		//<<20100521_JS.Kim  Bohap을 안넣으면 실시간 들어올 때 계산이 잘못되어 봉들이 앞으로 하나씩 지워진다.
		//DrawVerticalBar_NotOverlapDataSetting(pCalcData, nLastInsert, nDTI, IsOverlap, nIndex_High, nIndex_Low, 0, graphRegion.right - graphRegion.left, nDTI_End - nDTI_Start);
		DrawVerticalBar_NotOverlapDataSetting(pCalcData, nLastInsert, nDTI, IsOverlap, nIndex_High, nIndex_Low, nIndex_Bohap, graphRegion.right - graphRegion.left, nDTI_End - nDTI_Start);
		//>>
		pCalcData->nDTI_Start	= nDTI_Start;
		pCalcData->nDTI_End	= nDTI_End;
		pCalcData->nMaxTop		= nMaxTop;
		pCalcData->nPreLeft	= nPreLeft;
	}
	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{
		if( !subGraphData.m_pGraphCache) return true;
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;

		CBrush brushArray[BRUSH_COUNT];
		CBrush* pOldBrush = pDC->GetCurrentBrush();
		CPen penArray[PEN_COUNT];
		CPen* pOldPen = pDC->GetCurrentPen();
		CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);
//		// <가격차트> color 이 있다면 가격차트의 색을 가지고 와서 pen과 brush를 만든다.
//		if(pPriceChartColor != NULL)
//			CreateBrushsAndPens(brushArray, penArray, *pPriceChartColor, dRealWidth, penThickness, false);
//		else
//			CreateBrushsAndPens(brushArray, penArray, graphColor, dRealWidth, penThickness, false);
//
//		// (2006/12/6 - Seung-Won, Bae) Get Candle Steady Type directly.
//		int nBohapInBong = 1;
//		if( pIndicatorList)
//		{			
//			CIndicatorInfo *pPriceIndiInfo = pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART));
//			if( pPriceIndiInfo) pPriceIndiInfo = pIndicatorList->GetDefaultIndicatorInfo( EILT_USER_DEFAULT, _MTEXT( C2_PRICE_CHART), EIIGT_WITH_DEFAULT_GET);
//			if( pPriceIndiInfo) nBohapInBong = CBongDraw::GetBohapType( pPriceIndiInfo);
//		}	

		if(eBarDrawStyle == CGraphBaseData::Bar_Draw)
			DrawVerticalBar_DrawPolygon( pCalcData, pDC, graphColor, brushArray, penArray);
		else
		{	
			int dPreClose = pCalcData->dPreClose;
			int nIndex = 0;

			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			double *daCurClose = pCalcData->Get_dCurClose();
			double *daCurVolume = pCalcData->Get_dCurVolume();
			CRect *rtGraph = pCalcData->Get_rtGraph();

			for(int nDTI = pCalcData->nDTI_Start; nDTI <= pCalcData->nDTI_End; nDTI++)
			{
				SelectGdiObject_VolumeChart( pDC, daCurClose[ nIndex], dPreClose, graphColor, brushArray, penArray);
				// cj 2003.5.14 NonZero좌표점형 처리
				if( !(eBarDrawStyle == CGraphBaseData::NonZero_DotBar_Draw && daCurVolume[nIndex] == 0) )
 					DrawVerticalBar(pDC, eBarDrawStyle, rtGraph[nIndex], nBarWidth);
				
				dPreClose = daCurClose[ nIndex];
				nIndex++;
			}
		}
		DeleteBrushsAndPens(pDC, brushArray, penArray, pOldBrush, pOldPen);	
 	}
	return true;
}

void CSingleBarDraw::DrawVerticalBar(CDC* pDC, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& barRegion, const int nBarWidth)
{
	switch(eBarDrawStyle)
	{
	case CGraphBaseData::Bar_Draw: // 바형
		DrawVerticalBar_BarStyle(pDC, barRegion);
		break;
	case CGraphBaseData::DotBar_Draw: // 좌표점형
	case CGraphBaseData::NonZero_DotBar_Draw: // NonZero좌표점형
		DrawVerticalBar_DotBarStyle(pDC, barRegion, nBarWidth);
		break;
	case CGraphBaseData::TextBar_Draw: // Text형
		break;
	case CGraphBaseData::HorzLineBar_Draw: // 수평선바형
		DrawVerticalBar_HorzLineBarStyle(pDC, barRegion);
		break;
	}
}

//-> 바형(막대형)
void CSingleBarDraw::DrawVerticalBar_BarStyle(CDC* pDC, const CRect& barRegion)
{
	if(barRegion.Width() <= 1)
	{
		CDraw::DrawLine(pDC, CPoint(barRegion.left, barRegion.top), CPoint(barRegion.left, barRegion.bottom));
	}
	else
	{
		if(barRegion.top == barRegion.bottom)
			CDraw::DrawLine(pDC, CPoint(barRegion.left, barRegion.bottom), CPoint(barRegion.right, barRegion.bottom));
		else
			CDraw::DrawRectangle(pDC, barRegion);
	}
}

//-> 좌표점형, NonZero좌표점형
void CSingleBarDraw::DrawVerticalBar_DotBarStyle(CDC* pDC, const CRect& barRegion, const int nBarWidth)
{
	// 주의! nRadius(반지름) 값을 CRect값으로 안하는 이유는 
	// 각 bar 마다 rect가 틀리므로 반지름값이 각자 틀리다.
	// 그러므로 전체 바의 기본 폭을 가지고 반지름을 구하는것이 올바르다.
	int nRadius = nBarWidth / 2;
	if(nRadius <= 1)
		nRadius = 1;
	else if(nRadius > 5 && nRadius <= 15)
		nRadius = 5;
	else if(nRadius > 15)
		nRadius = 15;

	DrawDotBar(pDC, CPoint(barRegion.CenterPoint().x , barRegion.top), nRadius);
}

//-> 수평선바형
void CSingleBarDraw::DrawVerticalBar_HorzLineBarStyle(CDC* pDC, const CRect& barRegion)
{
	// rect의 wihth를 체크할 필요가 없다. 
	// -> 상위 함수에서 체크함.
	// size 조절하기.
	int nGap = 0;
	if(barRegion.Width() > 4)
		nGap = 1;
	CDraw::DrawLine(pDC, CPoint(barRegion.left + nGap, barRegion.top), CPoint(barRegion.right - nGap, barRegion.top));
}
//sy end

// ----------------------------------------------------------------------------
//수평바를 그리는 함수 
//sy 2002.10.29. -> 대기매물 관련 내용
bool CSingleBarDraw::DrawHorizontalBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	return false;

	CPacket* pVolumePacket = packetList.GetNext(pos);
	if(!pVolumePacket)	return false;

	CList<double, double>* pDataList = pVolumePacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)	return false;

	POSITION volumePos = pDataList->GetTailPosition();
	if(volumePos == NULL) return false;
	
	//가져올 데이터의 시작 위치를 얻어오는 부분======= 
	int indexCurrent = -1, indexHighest = -1;
	double dVolume;
	CString dataName = pVolumePacket->GetName();
	int dataCount = pVolumePacket->GetCount();
	// modified by rich 02/09/17
	// -> "대기매물"일 경우
// (2006.12.06) Sunghwan Ahn
	if( g_iMetaTable.IsVolumeForSale(dataName) && dataCount > 2)
// (2006.12.06) Sunghwan Ahn
//	if(dataName == _MTEXT( C2_OVERHANGING_SUPPLY) && dataCount > 2)
	{
		dataCount -= 2;
		// 최대
		dVolume = pDataList->GetPrev(volumePos);
		indexHighest = dataCount - (int)dVolume - 1;
		// 현재
		dVolume = pDataList->GetPrev(volumePos);
		indexCurrent = dataCount - (int)dVolume - 1;
	}
	volumePos = pDataList->FindIndex(0);

	double viewMin = 0.0;
	double viewMax = 0.0;
	pVolumePacket->GetMinMax(0, dataCount, viewMin, viewMax);	
	
	viewMin = 0;

	double Width = (double)graphRegion.Height()/(double)dataCount;
	nBarWidth = int(Width*0.8);
	nBarWidth = nBarWidth / 2;

	int orgMODE = pDC->SetBkMode(TRANSPARENT);
	COLORREF orgClr = pDC->SetTextColor(graphColor.GetColor1());
	//============================================
	for(int i = 0; i < dataCount; i++)	{
		if(volumePos == NULL)
			break;

		//sy 2002.9.24. -> brush 의 리소스 세는 문제 해결
		CPen barPen;
		CPen* pOldPen = NULL;
		CBrush barBrush;
		CBrush* pOldBrush = NULL;
		if(i == indexCurrent){
			if(graphColor.GetDownGraphFill()){
				barBrush.CreateSolidBrush(graphColor.GetColor6());
				pOldBrush = pDC->SelectObject(&barBrush);
			}
			else
				pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

			int lineThickness = CDraw::CalUpLineThickness(penThickness, nBarWidth);
			if(nBarWidth <= lineThickness * 3)
				barPen.CreatePen(PS_SOLID, 1, graphColor.GetColor5());
			else
				barPen.CreatePen(PS_SOLID, lineThickness, graphColor.GetColor5());
		}
		else if(i == indexHighest){
			if(graphColor.GetDownGraphFill()){
				barBrush.CreateSolidBrush(graphColor.GetColor4());
				pOldBrush = pDC->SelectObject(&barBrush);
			}
			else
				pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

			int lineThickness = CDraw::CalUpLineThickness(penThickness, nBarWidth);
			if(nBarWidth <= lineThickness * 3)
				barPen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
			else
				barPen.CreatePen(PS_SOLID, lineThickness, graphColor.GetColor3());
		}
		else{
			if(graphColor.GetUpGraphFill()){
				barBrush.CreateSolidBrush(graphColor.GetColor2());
				pOldBrush = pDC->SelectObject(&barBrush);
			}
			else
				pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

			int lineThickness = CDraw::CalUpLineThickness(penThickness, nBarWidth);
			if(nBarWidth <= lineThickness * 3)
				barPen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
			else
				barPen.CreatePen(PS_SOLID, lineThickness, graphColor.GetColor1());
		}
		pOldPen = pDC->SelectObject(&barPen);
	
		//dVolume에 해당하는 그래프 영역의 포인트를 가지고 온다.
		double dVolume = pDataList->GetNext(volumePos);
		
		CPoint dataPt = CDataConversion::GetHorizontalPoint(graphRegion, i, Width, CDraw::CalLogValue(bLog, viewMin), CDraw::CalLogValue(bLog, viewMax), CDraw::CalLogValue(bLog, dVolume));

		CRect rect(graphRegion.left, dataPt.y - nBarWidth, dataPt.x, dataPt.y + nBarWidth);
		if(bInvert == true){
			InvertBar(graphRegion, rect);
		}

		// 수평바 그리는 부분
		switch(drawStyle.GetGraphBarDrawStyle())
		{
			case CGraphBaseData::Bar_Draw:
			{
				if(nBarWidth < 1)
					CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.right, rect.top));
				else
					if(rect.left == rect.right)
						CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom));
					else
						CDraw::DrawRectangle(pDC, rect);
				break;
			}
			case CGraphBaseData::NonZero_DotBar_Draw:
			{
				if(dVolume == 0) // dVolumn이 0이 아니면 다음 case를 탄다.
					break;	
			}
			case CGraphBaseData::DotBar_Draw:
			{
				if(nBarWidth/2 < 1)
					DrawDotBar(pDC, CPoint(dataPt.x, dataPt.y), 1);
				else
					DrawDotBar(pDC, CPoint(dataPt.x, dataPt.y), nBarWidth/2);
				break;
			}
		
		}
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		barBrush.DeleteObject();
		barPen.DeleteObject();
	}

	//sy 2002.9.24. -> brush 를 제대로 안돌림
	pDC->SetBkMode(orgMODE);
	pDC->SetTextColor(orgClr);

	return true;
}

//상하바를 그리는 함수 기준가를 가지고 오는 부분이 있어야 하고 기준가와 실데이터를 비교해서 upbar인지 downbar인지를 알아내야 한다.
// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
bool CSingleBarDraw::DrawUpDownBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, const CGraphPaintStyle& paintStyle, 
		 const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		 const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		 CIndicatorInfo* pIndicatorInfo, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	double dStandard = 0.0;   // 임이의 기준가~~~
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* pVolumePacket = packetList.GetNext(pos);
	if(!pVolumePacket)	
		return false;
	POSITION volumePos = pVolumePacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION tempPos = volumePos;
	if(volumePos == NULL) 
		return false;

	CList<double, double>* pVolumeList = pVolumePacket->GetnumericDataList();
	if(pVolumeList == NULL)	
		return false;

	if(pIndicatorInfo != NULL){
		CList<double, double> * pBaseLineList = pIndicatorInfo->GetBaseLineList();
		if(pBaseLineList != NULL && pBaseLineList->GetCount() > 0)
			dStandard = pBaseLineList->GetHead();
	}

	CBrush upBrush(graphColor.GetColor2()), downBrush(graphColor.GetColor4()), bohapBrush(graphColor.GetColor5());
	CPen upPen, downPen, bohapPen;

	int nUpLineThick    = CDraw::CalUpLineThickness(penThickness, nBarWidth);
	int nDownLineThick  = CDraw::CalDownLineThickness(penThickness, nBarWidth);
	int nBohapLineThick = CDraw::CalStationaryLineThickness(penThickness, nBarWidth);

	if(nBarWidth <= nUpLineThick * 3 || nBarWidth <= nDownLineThick * 3){
		upPen.CreatePen	(PS_SOLID, 1, graphColor.GetColor1());
		downPen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
	}
	else{
		upPen.CreatePen	(PS_SOLID, nUpLineThick,	graphColor.GetColor1());
		downPen.CreatePen(PS_SOLID, nDownLineThick,	graphColor.GetColor3());
	}
	bohapPen.CreatePen(PS_SOLID, nBohapLineThick, graphColor.GetColor6());
	
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&upBrush);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&bohapPen);	

	// 기준선을 그려주는 부분 
	CPoint standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dStandard));
	if(bInvert == true)	
		standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
	if(dStandard >= dViewMin && dStandard <= dViewMax)
		CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
	
	//============================================
	double dPreVolume = 0.0;
	pVolumeList->GetPrev(tempPos);
	if(tempPos != NULL)
		dPreVolume = pVolumeList->GetAt(tempPos);
	
	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dPreVolume), false);

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(volumePos == NULL) 
			break;

		double dVolume = pVolumeList->GetNext(volumePos);
		CPoint curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dVolume), false);

		if(curPt.y < 0)
			continue;

		if(dVolume > dStandard){
			if(graphColor.GetUpGraphFill() == true)
				pDC->SelectObject(&upBrush);
			else
				pDC->SelectObject(GetStockObject(NULL_BRUSH));
			pDC->SelectObject(&upPen);
		}
		else if(dVolume < dStandard){
			if(graphColor.GetDownGraphFill() == true)
				pDC->SelectObject(&downBrush);
			else
				pDC->SelectObject(GetStockObject(NULL_BRUSH));
			pDC->SelectObject(&downPen);
		}
		else{
			pDC->SelectObject(&bohapBrush);
			pDC->SelectObject(&bohapPen);
		}

		CRect rect(prePt.x, curPt.y, curPt.x, standardPt.y);

		if(curPt.x - prePt.x > nUpLineThick)
			rect.left += nUpLineThick;
		
		if(rect.top < graphRegion.top)
			rect.top = graphRegion.top;
		if(rect.bottom < graphRegion.top)
			rect.bottom = graphRegion.top;
		if(rect.bottom > graphRegion.bottom )
			rect.bottom = graphRegion.bottom;
		if(rect.top > graphRegion.bottom )
			rect.top = graphRegion.bottom;

		if(bInvert == true)			
			rect.top = CDataConversion::InvertGraphDataPt(graphRegion, rect.top);
		// 바를 그려주는 부분 
		if(i >= pVolumePacket->GetStartPeriod()){
			// cj 2003.5.14 NonZero좌표점형 처리
			switch(drawStyle.GetGraphBarDrawStyle())
			{
				case CGraphBaseData::Bar_Draw:
				{
					if(nBarWidth < 1)
						CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom));
					else{
						if(rect.top == rect.bottom)
							CDraw::DrawLine(pDC, CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom));
						else
							CDraw::DrawRectangle(pDC, rect);
					}
					break;
				}
				case CGraphBaseData::NonZero_DotBar_Draw:
				{
					if(dVolume == 0) // dVolume이 0이 아니면 다음 case를 탄다.
						break;	
				}
				case CGraphBaseData::DotBar_Draw:
				{
					CPoint centerPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dVolume), true);
					if(nBarWidth/2 < 1)
						DrawDotBar(pDC, CPoint(centerPt.x, rect.top), 1);
					else
						DrawDotBar(pDC, CPoint(centerPt.x, rect.top), nBarWidth/2);
					break;
				}
			
			}
		}
		prePt = curPt;
	}	
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upBrush.DeleteObject();
	downBrush.DeleteObject();
	bohapBrush.DeleteObject();

	upPen.DeleteObject();
	downPen.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
double CSingleBarDraw::GetUpDownBarPrevValue(CPacket*& pPacket, POSITION& pos, CList<double, double>*& pList, const int& nStart)
{
	pos = pPacket->GetnumericDataList()->FindIndex(nStart);
	POSITION tempPos = pos;
	
	pList = pPacket->GetnumericDataList();
	pList->GetPrev(tempPos);
	if(tempPos != NULL)
		return pList->GetAt(tempPos);

	return 0.;
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
void CSingleBarDraw::DrawUpDownBarGraphFill(CDC* pDC, const double& dLeftValue, const double& dRightValue, const CGraphColor& graphColor,
											CBrush& upBrush, CPen& upPen, CBrush& downBrush, CPen& downPen, CBrush& bohapBrush, CPen& bohapPen)
{
	if(dLeftValue >= dRightValue)
	{
		if(graphColor.GetUpGraphFill() == true)
			pDC->SelectObject(&upBrush);
		else
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->SelectObject(&upPen);
	}
	else// if(dLeftValue < dRightValue)
	{
		if(graphColor.GetDownGraphFill() == true)
			pDC->SelectObject(&downBrush);
		else
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->SelectObject(&downPen);
	}
/*
	else{
		pDC->SelectObject(&bohapBrush);
		pDC->SelectObject(&bohapPen);
	}
*/
}

//{{XScale 적용
//상하바를 그리는 함수 기준가를 가지고 오는 부분이 있어야 하고 기준가와 실데이터를 비교해서 upbar인지 downbar인지를 알아내야 한다.
// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
// 기준선 두께, 모양 설정 - ojtaso (20071213)
bool CSingleBarDraw::DrawUpDownBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, const CGraphPaintStyle& paintStyle, 
		 const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		 const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		 CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CPenThickness& penThickness, const CGraphBaseLineStyle& baseLineStyle, bool bLog, bool bInvert,
		 const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn)
{
	if( !pxScaleManager) return false;
	if( pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		// 0. Check Index
		int nDTI_Start = -1, nDTI_End = -1;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if( nDTI_Start == -1) return true;

		if( packetList.IsEmpty()) return false;
		CPacket* pVolumePacket = packetList.GetHead();
		if( !pVolumePacket)	return false;

		CList<double, double>* pVolumeList = pVolumePacket->GetnumericDataList();
		if( !pVolumeList) return false;

		// 0.5 Check X Scale Mode.
		int nDataCount = pxScaleManager->GetColumnWidth( p_nColumn)  * 5;
		if( eBarDrawStyle != CGraphBaseData::Bar_Draw) nDataCount = ( nDTI_End - nDTI_Start + 1);
		if( nDataCount < 1) return true;
		// the change of Graph Style, Y Min/Max is xScaleMode 2 (Don't ask X Scale Manager !)
		int xScaleMode = 2;
		if(	subGraphData.m_pGraphCache)
		{	
			//Check YValue
			bool bYValue = subGraphData.m_pGraphCache->CheckYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);
			if(bYValue)
			{					
				if(pxScaleManager) xScaleMode = pxScaleManager->GetCalculationMode();
				if(xScaleMode == 0)	return true;
			}
		}	
		if( xScaleMode != 1)
		{
			if( subGraphData.m_pGraphCache)	subGraphData.m_pGraphCache->ResetValue();
			else							subGraphData.m_pGraphCache = new CCalcDataForDraw;
			subGraphData.m_pGraphCache->Alloc_lpPoint_High(	nDataCount);
			subGraphData.m_pGraphCache->Alloc_lpPoint_Low(	nDataCount);
			subGraphData.m_pGraphCache->Alloc_lpPoint_Bohap(	nDataCount);

			int nPixelCount = nDataCount;
			if( eBarDrawStyle == CGraphBaseData::Bar_Draw) nPixelCount = nDataCount / 5;
			subGraphData.m_pGraphCache->Alloc_lpPolyCount_High(	nPixelCount);
			subGraphData.m_pGraphCache->Alloc_lpPolyCount_Low(	nPixelCount);
			subGraphData.m_pGraphCache->Alloc_lpPolyCount_Bohap(	nPixelCount);
			subGraphData.m_pGraphCache->Alloc_lpdwcntPoly_High(	nPixelCount);
			subGraphData.m_pGraphCache->Alloc_lpdwcntPoly_Low(	nPixelCount);
			subGraphData.m_pGraphCache->Alloc_lpdwcntPoly_Bohap(	nPixelCount);

			subGraphData.m_pGraphCache->nCount_High  = 0;
			subGraphData.m_pGraphCache->nCount_Low   = 0;
			subGraphData.m_pGraphCache->nCount_Bohap = 0;

			// for Base Line
			double dStandard = 0.0;   // 임이의 기준가~~~
			if( pIndicatorInfo)
			{
				CList<double, double> * pBaseLineList = pIndicatorInfo->GetBaseLineList();
				if( pBaseLineList)
					if( 0 < pBaseLineList->GetCount())
						dStandard = pBaseLineList->GetHead();
			}
			// 기준선을 그려주는 부분 
			CPoint standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dStandard));
			if( bInvert) standardPt.y = CDataConversion::InvertGraphDataPt( graphRegion, standardPt.y);
			subGraphData.m_pGraphCache->dPreVolume = dStandard;
			subGraphData.m_pGraphCache->nPreLeft = standardPt.y;		
		}
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		pCalcData->SetYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);

		int *	pnCount_High		= &pCalcData->nCount_High;
		LPPOINT	lpPoint_High		= pCalcData->Get_lpPoint_High();
		int *	lpPolyCount_High	= pCalcData->Get_lpPolyCount_High();
		DWORD *	lpdwcntPoly_High	= pCalcData->Get_lpdwcntPoly_High();
		int *	pnIdxPoint_High		= &pCalcData->nLastPolyCnt_high;
		int *	pnCount_Low			= &pCalcData->nCount_Low;
		LPPOINT	lpPoint_Low			= pCalcData->Get_lpPoint_Low();
		int *	lpPolyCount_Low		= pCalcData->Get_lpPolyCount_Low();
		DWORD *	lpdwcntPoly_Low		= pCalcData->Get_lpdwcntPoly_Low();
		int *	pnIdxPoint_Low		= &pCalcData->nLastPolyCnt_Low;
		int *	pnCount_Bohap		= &pCalcData->nCount_Bohap;
		LPPOINT	lpPoint_Bohap		= pCalcData->Get_lpPoint_Bohap();
		int *	lpPolyCount_Bohap	= pCalcData->Get_lpPolyCount_Bohap();
		DWORD *	lpdwcntPoly_Bohap	= pCalcData->Get_lpdwcntPoly_Bohap();
		int *	pnIdxPoint_Bohap	= &pCalcData->nLastPolyCnt_Bohap;

		// 3. Set Iterator
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;

		// for X Scale Mode 1
		if( xScaleMode == 1)
		{
			nDTI_Start		= pCalcData->nDTI_Start;
		}
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);
		POSITION volumePos = pVolumeList->FindIndex( nDTI_Start);
		if( !volumePos) return false;

		BOOL bLineBar = ( nBarWidth <= 1 && !pxScaleManager->GetWeightedVolume());
		BOOL bXReverse = pxScaleManager->GetReverse();

		int nX = -1;
		CXScaleManager::PAREA parea;
		double dLeftValue = 0., dRightValue = 0.;
		for( int nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			parea = pblaAreafDTI->GetNext( dblCursor);
			if( !parea) break;
			if( !volumePos) break;

			// (2009/2/27 - Seung-Won, Bae) Backup for XScaleMode 1
			if( nX != parea->center)
			{
				nX = parea->center;
				pCalcData->nDTI_Start = nDTI;
			}

			double dVolume = pVolumeList->GetNext( volumePos);
			CPoint curPt = CDataConversion::GetVerticalPoint(graphRegion, nDTI-nRealStart+1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dVolume), false);
			CRect rect(parea->left, curPt.y, parea->right - 1, pCalcData->nPreLeft);

			// (2009/5/4 - Seung-Won, Bae) Make width not 0.
			if( rect.left > rect.right) rect.right = rect.left;
			
			if( rect.top	< graphRegion.top)		rect.top	= graphRegion.top;
			if( rect.bottom	< graphRegion.top)		rect.bottom	= graphRegion.top;
			if(	rect.bottom	> graphRegion.bottom)	rect.bottom	= graphRegion.bottom;
			if( rect.top	> graphRegion.bottom)	rect.top	= graphRegion.bottom;

			if( bInvert) rect.top = CDataConversion::InvertGraphDataPt(graphRegion, rect.top);
			if( nDTI < pVolumePacket->GetStartPeriod()) continue;

			int *	pnCount		= pnCount_High;
			LPPOINT	lpPoint		= lpPoint_High;
			int *	lpPolyCount	= lpPolyCount_High;
			int *	pnIdxPoint	= pnIdxPoint_High;
			DWORD *	lpdwcntPoly = lpdwcntPoly_High;
			int nFlag = 1;
			if( dVolume < pCalcData->dPreVolume)
			{
				pnCount		= pnCount_Low;
				lpPoint		= lpPoint_Low;
				lpPolyCount	= lpPolyCount_Low;
				pnIdxPoint	= pnIdxPoint_Low;
				lpdwcntPoly = lpdwcntPoly_Low;
				nFlag = -1;
			}
			else if( dVolume == pCalcData->dPreVolume)
			{
				pnCount		= pnCount_Bohap;
				lpPoint		= lpPoint_Bohap;
				lpPolyCount	= lpPolyCount_Bohap;
				pnIdxPoint	= pnIdxPoint_Bohap;
				lpdwcntPoly = lpdwcntPoly_Bohap;
				nFlag = 0;
			}

			int *	pre_pnCount		= pnCount_High;
			LPPOINT	pre_lpPoint		= lpPoint_High;
			int *	pre_lpPolyCount	= lpPolyCount_High;
			int *	pre_pnIdxPoint	= pnIdxPoint_High;
			DWORD *	pre_lpdwcntPoly = lpdwcntPoly_High;
			if( -1 == pCalcData->nFlag)
			{
				pre_pnCount		= pnCount_Low;
				pre_lpPoint		= lpPoint_Low;
				pre_lpPolyCount	= lpPolyCount_Low;
				pre_pnIdxPoint	= pnIdxPoint_Low;
				pre_lpdwcntPoly = lpdwcntPoly_Low;
			}
			else if( 0 == pCalcData->nFlag)
			{
				pre_pnCount		= pnCount_Bohap;
				pre_lpPoint		= lpPoint_Bohap;
				pre_lpPolyCount	= lpPolyCount_Bohap;
				pre_pnIdxPoint	= pnIdxPoint_Bohap;
				pre_lpdwcntPoly = lpdwcntPoly_Bohap;
			}
			pCalcData->nFlag = nFlag;

			// cj 2003.5.14 NonZero좌표점형 처리
			if( drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::Bar_Draw)
			{
				if( bLineBar)
				{
					BOOL bNewLine = ( *pnCount == 0);
					if( !bNewLine) bNewLine = ( lpPoint[ *pnIdxPoint - 1].x != rect.left);
					if( bNewLine)
					{
						lpPoint[ *pnIdxPoint].x = rect.left;
						lpPoint[ *pnIdxPoint].y = rect.top;
						( *pnIdxPoint)++;
						lpPoint[ *pnIdxPoint].x = rect.left;
						lpPoint[ *pnIdxPoint].y = rect.bottom;
						( *pnIdxPoint)++;
						lpdwcntPoly[ *pnCount] = 2;
						( *pnCount)++;
					}
					else if( ( rect.top < rect.bottom && rect.top < lpPoint[ *pnIdxPoint - 2].y)
							|| ( rect.top > rect.bottom && rect.top > lpPoint[ *pnIdxPoint - 2].y))
						lpPoint[ *pnIdxPoint - 2].y = rect.top;
				}
				else
				{
					BOOL bNewBar = ( *pre_pnIdxPoint == 0);
					if( !bNewBar) bNewBar = ( ( !bXReverse && pre_lpPoint[ *pre_pnIdxPoint - 1].x < rect.left)
												|| ( bXReverse && rect.right < pre_lpPoint[ *pre_pnIdxPoint - 3].x));
					if( !bNewBar)
					{
						( *pre_pnIdxPoint) -= 5;
						( *pre_pnCount)--;
					}

					lpPoint[ *pnIdxPoint].x = rect.right;
					lpPoint[ *pnIdxPoint].y = rect.bottom;
					( *pnIdxPoint)++;
					lpPoint[ *pnIdxPoint].x = rect.left;
					lpPoint[ *pnIdxPoint].y = rect.bottom;
					( *pnIdxPoint)++;
					lpPoint[ *pnIdxPoint].x = rect.left;
					lpPoint[ *pnIdxPoint].y = rect.top;
					( *pnIdxPoint)++;
					lpPoint[ *pnIdxPoint].x = rect.right;
					lpPoint[ *pnIdxPoint].y = rect.top;
					( *pnIdxPoint)++;
					lpPoint[ *pnIdxPoint].x = rect.right;
					lpPoint[ *pnIdxPoint].y = rect.bottom;
					( *pnIdxPoint)++;
					lpPolyCount[ *pnCount] = 5;
					( *pnCount)++;
				}
			}
			else
			{
				lpPoint[ *pnIdxPoint].x = parea->center;
				lpPoint[ *pnIdxPoint].y = rect.top;
				( *pnIdxPoint)++;
				lpPolyCount[ *pnCount] = parea->right - parea->left;
				( *pnCount)++;
			}
		}
	}
	else
	{
		if( !subGraphData.m_pGraphCache)
			return true;

		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( pCalcData->nCount_High < 1 && pCalcData->nCount_Low < 1 && pCalcData->nCount_Bohap < 1) return true;

		// 1. Draw Base Line.
		if( dViewMin <= pCalcData->dPreVolume && pCalcData->dPreVolume <= dViewMax)
		{
			// 기준선 두께, 모양 설정 - ojtaso (20071213)
			CPen standardPen;
			CreatePen( standardPen, baseLineStyle, penThickness.GetBaseLineThickness(), graphColor.GetColor6());
			pDC->SelectObject(&standardPen);
			CDraw::DrawLine(pDC, CPoint( graphRegion.left, pCalcData->nPreLeft), CPoint(graphRegion.right, pCalcData->nPreLeft));
			standardPen.DeleteObject();	
		}

		// 2. Create Brush and pen
		CBrush upBrush( graphColor.GetColor2()), downBrush(graphColor.GetColor4()), bohapBrush(graphColor.GetColor5());
		CPen upPen, downPen, bohapPen;
		int nUpLineThick    = CDraw::CalUpLineThickness(			penThickness, nBarWidth);
		int nDownLineThick  = CDraw::CalDownLineThickness(			penThickness, nBarWidth);
		int nBohapLineThick = CDraw::CalStationaryLineThickness(	penThickness, nBarWidth);
		if( nBarWidth <= nUpLineThick * 3 || nBarWidth <= nDownLineThick * 3)
		{
			upPen.CreatePen	(PS_SOLID, 1, graphColor.GetColor1());
			downPen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
		}
		else
		{
			upPen.CreatePen	(PS_SOLID, nUpLineThick,	graphColor.GetColor1());
			downPen.CreatePen(PS_SOLID, nDownLineThick,	graphColor.GetColor3());
		}
		bohapPen.CreatePen(PS_SOLID, nBohapLineThick, graphColor.GetColor6());

		
		CBrush* pOldBrush = ( CBrush*)pDC->SelectObject( &upBrush);
		CPen* pOldPen = ( CPen*)pDC->SelectObject( &bohapPen);	

		if( drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::Bar_Draw)
		{
			if( nBarWidth <= 1 && !pxScaleManager->GetWeightedVolume())
			{
				if( pCalcData->nCount_High > 0)
				{
					pDC->SelectObject( upBrush);
					pDC->SelectObject( upPen);
					if( 1 < pCalcData->nCount_High)
							pDC->PolyPolyline( pCalcData->Get_lpPoint_High(), pCalcData->Get_lpdwcntPoly_High(), pCalcData->nCount_High);
					else	pDC->Polyline( pCalcData->Get_lpPoint_High(), 2);
				}
				if( pCalcData->nCount_Low > 0)
				{
					pDC->SelectObject( downBrush);
					pDC->SelectObject( downPen);
					if( 1 < pCalcData->nCount_Low)
							pDC->PolyPolyline( pCalcData->Get_lpPoint_Low(), pCalcData->Get_lpdwcntPoly_Low(), pCalcData->nCount_Low);
					else	pDC->Polyline( pCalcData->Get_lpPoint_Low(), 2);
				}
			}
			else
			{
				if( pCalcData->nCount_High > 0)
				{
					pDC->SelectObject( upBrush);
					pDC->SelectObject( upPen);
					if( 1 < pCalcData->nCount_High)
							pDC->PolyPolygon( pCalcData->Get_lpPoint_High(), pCalcData->Get_lpPolyCount_High(), pCalcData->nCount_High);
					else	pDC->Polygon( pCalcData->Get_lpPoint_High(), 5);
				}
				if( pCalcData->nCount_Low > 0)
				{
					pDC->SelectObject( downBrush);
					pDC->SelectObject( downPen);
					if( 1 < pCalcData->nCount_Low)
							pDC->PolyPolygon( pCalcData->Get_lpPoint_Low(), pCalcData->Get_lpPolyCount_Low(), pCalcData->nCount_Low);
					else	pDC->Polygon( pCalcData->Get_lpPoint_Low(), 5);
				}
			}
		}
		else
		{
			int *	pnCount_High		= &pCalcData->nCount_High;
			LPPOINT	lpPoint_High		= pCalcData->Get_lpPoint_High();
			int *	lpPolyCount_High	= pCalcData->Get_lpPolyCount_High();
			DWORD *	lpdwcntPoly_High	= pCalcData->Get_lpdwcntPoly_High();
			int *	pnIdxPoint_High		= &pCalcData->nLastPolyCnt_high;
			int *	pnCount_Low			= &pCalcData->nCount_Low;
			LPPOINT	lpPoint_Low			= pCalcData->Get_lpPoint_Low();
			int *	lpPolyCount_Low		= pCalcData->Get_lpPolyCount_Low();
			DWORD *	lpdwcntPoly_Low		= pCalcData->Get_lpdwcntPoly_Low();
			int *	pnIdxPoint_Low		= &pCalcData->nLastPolyCnt_Low;
			int *	pnCount_Bohap		= &pCalcData->nCount_Bohap;
			LPPOINT	lpPoint_Bohap		= pCalcData->Get_lpPoint_Bohap();
			int *	lpPolyCount_Bohap	= pCalcData->Get_lpPolyCount_Bohap();
			DWORD *	lpdwcntPoly_Bohap	= pCalcData->Get_lpdwcntPoly_Bohap();
			int *	pnIdxPoint_Bohap	= &pCalcData->nLastPolyCnt_Bohap;

			int *	pnCount		= pnCount_High;
			LPPOINT	lpPoint		= lpPoint_High;
			int *	lpPolyCount	= lpPolyCount_High;
			int *	pnIdxPoint	= pnIdxPoint_High;
			DWORD *	lpdwcntPoly = lpdwcntPoly_High;
			if( *pnCount > 0)
			{
				pDC->SelectObject( upBrush);
				pDC->SelectObject( upPen);
				for( int i = 0; i < *pnCount; i++)
				{
					if( lpPolyCount[ i] / 2 < 1)
							DrawDotBar( pDC, lpPoint[ i], 1);
					else	DrawDotBar( pDC, lpPoint[ i], lpPolyCount[ i] / 2);
				}
			}
			pnCount		= pnCount_Low;
			lpPoint		= lpPoint_Low;
			lpPolyCount	= lpPolyCount_Low;
			pnIdxPoint	= pnIdxPoint_Low;
			lpdwcntPoly = lpdwcntPoly_Low;
			if( *pnCount > 0)
			{
				pDC->SelectObject( downBrush);
				pDC->SelectObject( downPen);
				for( int i = 0; i < *pnCount; i++)
				{
					if( lpPolyCount[ i] / 2 < 1)
							DrawDotBar( pDC, lpPoint[ i], 1);
					else	DrawDotBar( pDC, lpPoint[ i], lpPolyCount[ i] / 2);
				}
			}
			pnCount		= pnCount_Bohap;
			lpPoint		= lpPoint_Bohap;
			lpPolyCount	= lpPolyCount_Bohap;
			pnIdxPoint	= pnIdxPoint_Bohap;
			lpdwcntPoly = lpdwcntPoly_Bohap;
			if( *pnCount > 0 && drawStyle.GetGraphBarDrawStyle() != CGraphBaseData::NonZero_DotBar_Draw)
			{
				pDC->SelectObject( bohapBrush);
				pDC->SelectObject( bohapPen);
				for( int i = 0; i < *pnCount; i++)
				{
					if( lpPolyCount[ i] / 2 < 1)
							DrawDotBar( pDC, lpPoint[ i], 1);
					else	DrawDotBar( pDC, lpPoint[ i], lpPolyCount[ i] / 2);
				}
			}
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		upBrush.DeleteObject();
		downBrush.DeleteObject();
		bohapBrush.DeleteObject();

		upPen.DeleteObject();
		downPen.DeleteObject();
		bohapPen.DeleteObject();
	}
	return true;
}
//}}

bool CSingleBarDraw::DrawCombBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* pPacket1 = packetList.GetNext(pos);
	if(!pPacket1)	return false;
	if(pos == NULL)
		return false;
	CPacket* pPacket2 = packetList.GetNext(pos);
	if(!pPacket2)	return false;

	POSITION Data1Pos = pPacket1->GetnumericDataList()->FindIndex(nRealStart);
	POSITION Data2Pos = pPacket2->GetnumericDataList()->FindIndex(nRealStart);

	if(Data1Pos == NULL || Data2Pos == NULL)
		return false;

	int realDrawStart = 0;
	int tempStart = realDrawStart;
	realDrawStart = pPacket1->GetStartPeriod();
	tempStart = pPacket2->GetStartPeriod();
	if(realDrawStart < tempStart)
		realDrawStart = tempStart;
	
	CBrush upBrush	(graphColor.GetColor2());
	CBrush downBrush(graphColor.GetColor4());
	
	CPen upPen;
	CPen downPen;
	CPen bohapPen;

	int uplineThickness = CDraw::CalUpLineThickness(penThickness, nBarWidth);
	int downlineThickness = CDraw::CalDownLineThickness(penThickness, nBarWidth);
	int stationarylineThickness = CDraw::CalStationaryLineThickness(penThickness, nBarWidth);

	if(nBarWidth <= uplineThickness * 3 || nBarWidth <= downlineThickness * 3){
		upPen.CreatePen	(PS_SOLID,	1, graphColor.GetColor1());
		downPen.CreatePen(PS_SOLID,	1, graphColor.GetColor3());
	}
	else{
		upPen.CreatePen	(PS_SOLID,	uplineThickness,	graphColor.GetColor1());
		downPen.CreatePen(PS_SOLID,	downlineThickness,	graphColor.GetColor3());
	}
	bohapPen.CreatePen(PS_SOLID, stationarylineThickness, graphColor.GetColor6());
	
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&upBrush);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&bohapPen);

	//min 과 max를 새로 구한다.=====================================================
	double datamax = 0.0, datamin = 0.0, tempmax = 0.0, tempmin = 0.0;
	pPacket1->GetMinMax(nRealStart, nRealEnd, datamin, datamax); 
	datamin = fabs(datamin);
	datamax = fabs(datamax);
	if(datamin > datamax)
		datamax = datamin;

	pPacket2->GetMinMax(nRealStart, nRealEnd, tempmax, tempmin); 
	tempmin = fabs(tempmin);
	tempmax = fabs(tempmax);
	if(tempmin > tempmax)
		tempmax = tempmin;
	if(tempmax > datamax)
		datamax = tempmax;
	datamin = datamax * -1;
	//==============================================================================
	// 기준선을 그려주는 부분 -> 기준선 = 0.0
	CPoint standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, CDraw::CalLogValue(bLog, datamin), CDraw::CalLogValue(bLog, datamax), CDraw::CalLogValue(bLog, 0.0));
	if(bInvert == true)	
		standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

	CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));

	if(nBarWidth <= 2)
		nBarWidth = 1;
	else
		nBarWidth = nBarWidth / 2;
	
	double dPreVolume = 0.0;
	POSITION preVolumePos = Data1Pos;
	pPacket1->GetnumericDataList()->GetPrev(preVolumePos);
	if(preVolumePos != NULL)
		dPreVolume = pPacket1->GetnumericDataList()->GetAt(preVolumePos);
	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dPreVolume), false);
	
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(Data1Pos == NULL || Data2Pos == NULL)
			break;

		double Data1 = pPacket1->GetnumericDataList()->GetNext(Data1Pos);
		if(Data1 < 0.0)
			Data1 = Data1 * -1.0;
		CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, CDraw::CalLogValue(bLog, datamin), CDraw::CalLogValue(bLog, datamax), CDraw::CalLogValue(bLog, Data1), false);
		if(dataPt.y < 0)
			continue;

		if(graphColor.GetUpGraphFill() == true)
			pDC->SelectObject(&upBrush);
		else
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->SelectObject(&upPen);

		CRect rect1(prePt.x, dataPt.y, dataPt.x, standardPt.y);
		if(dataPt.x - prePt.x > uplineThickness)
			rect1.left += uplineThickness;

		if(bInvert == true)			
			InvertBar(graphRegion, rect1);
		// 첫번재 바를 그려주는 부분 
		if(nRealStart >= realDrawStart){
			switch(drawStyle.GetGraphBarDrawStyle())
			{
				case CGraphBaseData::Bar_Draw:
				{
					if(nBarWidth < 1)
						CDraw::DrawLine(pDC, CPoint(rect1.left, rect1.top), CPoint(rect1.left, rect1.bottom));
					else if(rect1.top == rect1.bottom)
						CDraw::DrawLine(pDC, CPoint(rect1.left, rect1.bottom), CPoint(rect1.right, rect1.bottom));
					else
						CDraw::DrawRectangle(pDC, rect1);
					break;
				}
				case CGraphBaseData::NonZero_DotBar_Draw:
				{
					if(Data1 == 0) // dVolumn이 0이 아니면 다음 case를 탄다.
						break;	
				}
				case CGraphBaseData::DotBar_Draw:
				{
					CPoint centerPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, CDraw::CalLogValue(bLog, datamin), CDraw::CalLogValue(bLog, datamax), CDraw::CalLogValue(bLog, Data1), true);
					if(nBarWidth < 1)
						DrawDotBar(pDC, CPoint(centerPt.x, rect1.top), 1);
					else
						DrawDotBar(pDC, CPoint(centerPt.x, rect1.top), nBarWidth/2);
					break;
				}
			
			}
		}

		double Data2 = pPacket2->GetnumericDataList()->GetNext(Data2Pos);
		if(Data2 > 0.0)
			Data2 = Data2 * -1.0;
		dataPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, CDraw::CalLogValue(bLog, datamin), CDraw::CalLogValue(bLog, datamax), CDraw::CalLogValue(bLog, Data2), false);
		if(dataPt.y < 0)
			continue;
		
		if(graphColor.GetDownGraphFill() == true)
			pDC->SelectObject(&downBrush);
		else
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->SelectObject(&downPen);

		CRect rect2(prePt.x, dataPt.y, dataPt.x, standardPt.y);
		if(dataPt.x - prePt.x > uplineThickness)
			rect2.left += uplineThickness;

		if(bInvert == true)			
			InvertBar(graphRegion, rect2);
		// 두번째 바를 그려주는 부분 
		if(nRealStart >= realDrawStart){
			switch(drawStyle.GetGraphBarDrawStyle())
			{
				case CGraphBaseData::Bar_Draw:
				{
					if(nBarWidth < 1)
						CDraw::DrawLine(pDC, CPoint(rect2.left, rect2.top), CPoint(rect2.left, rect2.bottom));
					else if(rect2.top == rect2.bottom)
						CDraw::DrawLine(pDC, CPoint(rect2.left, rect2.bottom), CPoint(rect2.right, rect2.bottom));
					else
						CDraw::DrawRectangle(pDC, rect2);
					break;
				}
				case CGraphBaseData::NonZero_DotBar_Draw:
				{
					if(Data2 == 0) // dVolumn이 0이 아니면 다음 case를 탄다.
						break;	
				}
				case CGraphBaseData::DotBar_Draw:
				{
//					CPoint centerPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, CDraw::CalLogValue(bLog, datamin), CDraw::CalLogValue(bLog, datamax), CDraw::CalLogValue(bLog, Data1), true);
					CPoint centerPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, CDraw::CalLogValue(bLog, datamin), CDraw::CalLogValue(bLog, datamax), CDraw::CalLogValue(bLog, Data2), true);
					if(nBarWidth/2 < 1)
						DrawDotBar(pDC, CPoint(centerPt.x, rect2.top), 1);
					else
						DrawDotBar(pDC, CPoint(centerPt.x, rect2.top), nBarWidth/2);
					break;
				}
			
			}
		}
		prePt = dataPt;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upBrush.DeleteObject();
	downBrush.DeleteObject();

	upPen.DeleteObject();
	downPen.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}

bool CSingleBarDraw::DrawFixedRateCombBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	if(packetList.GetCount() < 2)
		return false;

	POSITION pos = packetList.GetHeadPosition();
	CPacket *pPacket1 = packetList.GetNext(pos);
	CPacket *pPacket2 = packetList.GetNext(pos);

	if(pPacket1 == NULL || pPacket2 == NULL)
		return false;

	// 기준선을 그리는 부분 -> 기준선 = 0.0
	CPoint standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, 0.0));
	if(bInvert == true)	
		standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
	CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), 
		CPoint(graphRegion.right, standardPt.y), graphColor.GetColor5(), penThickness.GetThickness3());

	// bar를 그리는 부분
	nBarWidth = nBarWidth <= 2 ? 1: nBarWidth / 2;

	DrawFixedRateCombBar_Bar(pDC, pPacket1, graphRegion, drawStyle.GetGraphBarDrawStyle(),
		nRealStart, nRealEnd, dRealWidth, nBarWidth, dViewMin, dViewMax, 
		bLog, bInvert, standardPt, graphColor.GetColor1(), graphColor.GetColor2(), 
		CDraw::CalUpLineThickness(penThickness, nBarWidth), graphColor.GetUpGraphFill(), true);
	DrawFixedRateCombBar_Bar(pDC, pPacket2, graphRegion, drawStyle.GetGraphBarDrawStyle(), 
		nRealStart, nRealEnd, dRealWidth, nBarWidth, dViewMin, dViewMax, 
		bLog, bInvert, standardPt, graphColor.GetColor3(), graphColor.GetColor4(), 
		CDraw::CalDownLineThickness(penThickness, nBarWidth), graphColor.GetDownGraphFill(), false);

	return true;
}

bool CSingleBarDraw::DrawFixedRateCombBar_Bar(CDC* pDC, CPacket* pPacket, const CRect& graphRegion, 
		const CGraphBaseData::BARDRAWSTYLE eDrawStyle, const int nRealStart, const int nRealEnd, const double& dRealWidth, const int nBarWidth, 
		const double& dViewMin, const double& dViewMax, const bool bLog, const bool bInvert, const CPoint& standardPt, 
		const COLORREF& plusColor, const COLORREF& minusColor, const int nLineThickness, const bool bIsFill, const bool bIsTop)
{
	if(pPacket == NULL)
		return false;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;

	POSITION dataPos = pDataList->FindIndex(nRealStart);
	if(dataPos == NULL)
		return false;

	CPen plusPen(PS_SOLID, nLineThickness, plusColor);
	CPen minusPen(PS_SOLID, nLineThickness, minusColor);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&plusPen);

	CBrush plusBrush(plusColor);
	CBrush minusBrush(minusColor);
	CBrush *pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

	// text 관련 mode setting
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = pDC->SetTextColor(plusColor);

	int nRealDrawStart = pPacket->GetStartPeriod();
	int nSign = bIsTop ? 1: -1;
	int nPrePt_x = graphRegion.left;
	double dViewMin_Log = CDraw::CalLogValue(bLog, dViewMin);
	double dViewMax_Log = CDraw::CalLogValue(bLog, dViewMax);
	CSize textSize(0, 0);
	if(DoesTextDrawing(eDrawStyle))
		textSize = pDC->GetTextExtent("A");
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(dataPos == NULL)
			break;

		double dData = pDataList->GetNext(dataPos);

		pDC->SelectObject(dData < 0.0 ? &minusPen : &plusPen);
		if(bIsFill)
			pDC->SelectObject(dData < 0.0 ? &minusBrush : &plusBrush);

		// 그리는 위치에 따라 값을 조절
		dData = fabs(dData) * nSign;
		dData = CDraw::CalLogValue(bLog, dData);

		CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, 
			dViewMin_Log, dViewMax_Log, dData, false);

		if(dData == 0){
			nPrePt_x = dataPt.x;
			continue;
		}

		CRect barRect(nPrePt_x, dataPt.y, dataPt.x, standardPt.y);
		// 중앙을 맞추기 위해서..
		if(dataPt.x - nPrePt_x > nLineThickness)
			barRect.left += nLineThickness;

		if(dataPt.y == standardPt.y){
			// data가 "0"은 아니지만 standardPt.y 와 같은 좌표일 경우 크기를 1로 한다.
			barRect.OffsetRect(0, -1 * nSign);
		}
		else{
			// 기준선("0") 을 구분해주기 위해 중심에서 1만큼 띈다.
			barRect.DeflateRect(0, 0, 0, 1 * nSign);
		}

		// 좌우 여백을 준다. -> 폭을 생각해서 offset 값을 정한다.
		if(barRect.Width() > 5){
			int nOffset = 1;
			if(barRect.Width() > 20)
				nOffset = 3;

			// 폭을 생각해서 offset 적용한다.
			if(barRect.Width() > nOffset*3)
				barRect.DeflateRect(nOffset, 0, nOffset, 0);
		}

		barRect.NormalizeRect();
		if(bInvert)			
			InvertBar(graphRegion, barRect);

		// 바를 그려주는 부분 
		if(nRealStart >= nRealDrawStart){
			if(nBarWidth < 1)
				CDraw::DrawLine(pDC, CPoint(barRect.left, barRect.top), CPoint(barRect.left, barRect.bottom));
			else if(barRect.top == barRect.bottom)
				CDraw::DrawLine(pDC, CPoint(barRect.left, barRect.bottom), CPoint(barRect.right, barRect.bottom));
			else
				CDraw::DrawRectangle(pDC, barRect);
		}
		// text를 그리는 부분 : 항상 양수!
		if(DoesTextDrawing(eDrawStyle))
		{
			pDC->SetTextColor(dData < 0.0 ? minusColor : plusColor);
			DataTextDraw(pDC, (dData * nSign), barRect, textSize, bIsTop);
		}

		nPrePt_x = dataPt.x;
	}

	pDC->SetBkMode(nOldBkMode);
	pDC->SetTextColor(oldTextColor);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	plusPen.DeleteObject();
	minusPen.DeleteObject();
	plusBrush.DeleteObject();
	minusBrush.DeleteObject();

	return true;
}
//상하바를 그리는 함수 기준가를 가지고 오는 부분이 있어야 하고 기준가와 실데이터를 비교해서 upbar인지 downbar인지를 알아내야 한다.
bool CSingleBarDraw::DrawUpDownHighLowBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		CIndicatorInfo* pIndicatorInfo, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	double dStandard = 0.0;   // 임이의 기준가~~~
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;

	CPacket* pVolumePacket = packetList.GetNext(pos);
	if(!pVolumePacket)	return false;

	CList<double, double>* pDataList = pVolumePacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;

	POSITION volumePos = pDataList->FindIndex(nRealStart);
	if(volumePos == NULL) 
		return false;

	// 기준가를 가지고 오는부분.
	if(pIndicatorInfo != NULL){
		CList<double, double>* pBaseLineList = pIndicatorInfo->GetBaseLineList();		
		if(pBaseLineList != NULL && pBaseLineList->GetCount() > 0)
			dStandard = pBaseLineList->GetHead();
	}

	CBrush upBrush_high(graphColor.GetColor1()), upBrush_low(graphColor.GetColor2());
	CBrush downBrush_high(graphColor.GetColor3()), downBrush_low(graphColor.GetColor4());
	CBrush bohapBrush(graphColor.GetColor5());
	
	CPen upPen_high, upPen_low, downPen_high, downPen_low, bohapPen;

	int nUpLineThick	= CDraw::CalUpLineThickness(penThickness, nBarWidth);
	int nDownLineThick	= CDraw::CalDownLineThickness(penThickness, nBarWidth);
	int nBohapLineThick = CDraw::CalStationaryLineThickness(penThickness, nBarWidth);

	if(nBarWidth <= nUpLineThick * 3 || nBarWidth <= nDownLineThick * 3){
		upPen_high.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
		upPen_low.CreatePen(PS_SOLID, 1, graphColor.GetColor2());
		downPen_high.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
		downPen_low.CreatePen(PS_SOLID, 1, graphColor.GetColor4());
	}
	else{
		upPen_high.CreatePen(PS_SOLID, nUpLineThick, graphColor.GetColor1());
		upPen_low.CreatePen(PS_SOLID, nUpLineThick, graphColor.GetColor2());
		downPen_high.CreatePen(PS_SOLID, nDownLineThick, graphColor.GetColor3());
		downPen_low.CreatePen(PS_SOLID, nDownLineThick, graphColor.GetColor4());
	}
	bohapPen.CreatePen(PS_SOLID, nBohapLineThick, graphColor.GetColor5());
	
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&upBrush_high);
	/// 이부분이 교보와 다름
	CPen* pOldPen = (CPen*)pDC->SelectObject(&bohapPen);

	// 기준선을 그려주는 부분 
	CPoint standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dStandard));
	if(bInvert == true)	
		standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
	if(dStandard >= dViewMin && dStandard <= dViewMax)
		CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));

	double dPreVolume = 0.0;
	POSITION preVolumePos = volumePos;
	pDataList->GetPrev(preVolumePos);
	if(preVolumePos != NULL)
		dPreVolume = pDataList->GetAt(preVolumePos);
	
	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dPreVolume), false);

	//============================================
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(volumePos == NULL)	
			break;
		double dVolume = pDataList->GetNext(volumePos);
		CPoint curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dVolume), false);

		if(curPt.y < 0)
			continue;

		// 위
		if(dVolume > dStandard){
			if(graphColor.GetUpGraphFill()){
				// 상승(high)
				if(dPreVolume < dVolume)
					pDC->SelectObject(&upBrush_high);
				// 하락(low)
				else if(dPreVolume > dVolume)
					pDC->SelectObject(&upBrush_low);
				// 보합
				else
					pDC->SelectObject(&bohapBrush);
			}
			else
				pDC->SelectObject(GetStockObject(NULL_BRUSH));

			// 상승(high)
			if(dPreVolume < dVolume)
				pDC->SelectObject(&upPen_high);
			// 하락(low)
			else if(dPreVolume > dVolume)
				pDC->SelectObject(&upPen_low);
			// 보합
			else
				pDC->SelectObject(&bohapPen);
		}
		// 아래
		else if(dVolume < dStandard){
			if(graphColor.GetDownGraphFill()){
				// 상승(high)
				if(dPreVolume < dVolume)
					pDC->SelectObject(&downBrush_high);
				// 하락(low)
				else if(dPreVolume > dVolume)
					pDC->SelectObject(&downBrush_low);
				// 보합
				else
					pDC->SelectObject(&bohapBrush);
			}
			else
				pDC->SelectObject(GetStockObject(NULL_BRUSH));

			// 상승(high)
			if(dPreVolume < dVolume)
				pDC->SelectObject(&downPen_high);
			// 하락(low)
			else if(dPreVolume > dVolume)
				pDC->SelectObject(&downPen_low);
			// 보합
			else
				pDC->SelectObject(&bohapPen);
		}
		else{
			pDC->SelectObject(&bohapBrush);
			pDC->SelectObject(&bohapPen);
		}


		CRect rect(prePt.x, curPt.y, curPt.x, standardPt.y);
///		if(nBarWidth <= 1)
		if(curPt.x - prePt.x > nUpLineThick)
			rect.left += nUpLineThick;
		
		if(rect.top < graphRegion.top)
			rect.top = graphRegion.top;
		if(rect.bottom < graphRegion.top)
			rect.bottom = graphRegion.top;
		if(rect.bottom > graphRegion.bottom )
			rect.bottom = graphRegion.bottom;
		if(rect.top > graphRegion.bottom )
			rect.top = graphRegion.bottom;

		if(bInvert == true)			
			rect.top = CDataConversion::InvertGraphDataPt(graphRegion, rect.top);
		// 바를 그려주는 부분 
///		checkDataStart++;
		if(i >= pVolumePacket->GetStartPeriod()){
			// cj 2003.5.14 NonZero좌표점형 처리
			switch(drawStyle.GetGraphBarDrawStyle())
			{
				case CGraphBaseData::Bar_Draw:
				{
					if(rect.Width() < 1)
						CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom));
					else{
						if(rect.top == rect.bottom)
							CDraw::DrawLine(pDC, CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom));
						else
							CDraw::DrawRectangle(pDC, rect);
					}
					break;
				}
				case CGraphBaseData::NonZero_DotBar_Draw:
				{
					if(dVolume == 0) // dVolumn이 0이 아니면 다음 case를 탄다.
						break;	
				}
				case CGraphBaseData::DotBar_Draw:
				{
					CPoint centerPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dVolume), true);
					if(nBarWidth/2 < 1)
						DrawDotBar(pDC, CPoint(curPt.x, rect.top), 1);
					else
						DrawDotBar(pDC, CPoint(curPt.x, rect.top), nBarWidth/2);
					break;
				}
			
			}
		}
		dPreVolume = dVolume;
		prePt = curPt;
	}	
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upBrush_high.DeleteObject();
	upBrush_low.DeleteObject();
	downBrush_high.DeleteObject();
	downBrush_low.DeleteObject();
	bohapBrush.DeleteObject();

	upPen_high.DeleteObject();
	upPen_low.DeleteObject();
	downPen_high.DeleteObject();
	downPen_low.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}
void CSingleBarDraw::DrawDotBar(CDC* pDC, POINT startPt, int nBarWidth)
{
	if(nBarWidth < 3)
		nBarWidth = 3;
	else if(nBarWidth > 10)
		nBarWidth = 10;
	pDC->Ellipse(startPt.x - nBarWidth, startPt.y - nBarWidth, startPt.x + nBarWidth, startPt.y + nBarWidth);
}
void CSingleBarDraw::DataTextDraw(CDC* pDC, const double& dData, const CRect& barRegion, const CSize& textSize, const bool bIsTop)
{
	CString strText = CDataConversion::GetStringThousandFormat(CDataConversion::DoubleToString(dData));
	if(strText.IsEmpty())
		return;

	CRect textRegion = barRegion;
	if(bIsTop)
	{
		textRegion.top = barRegion.top - (textSize.cy + MARGIN);
		textRegion.bottom = barRegion.top;
	}
	else
	{
		textRegion.top = barRegion.bottom;
		textRegion.bottom = barRegion.bottom + (textSize.cy + MARGIN);
	}

	CDraw::TextOut(pDC, textRegion, strText, true);
}
bool CSingleBarDraw::DrawDotBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert)
{
	if( packetList.IsEmpty()) return false;
	CPacket* dataPacket = packetList.GetHead();
	
	CPacketList *pPacketList = dataPacket->GetPacketList();
	if( !pPacketList) return false;
	
	CPacket* highPacket = pPacketList->GetBaseData( _MTEXT( C0_HIGH));
	CPacket* lowPacket	= pPacketList->GetBaseData( _MTEXT( C0_LOW));
	if( !highPacket || !lowPacket) return false;
	
	//CList<double, double>* pDataList = dataPacket->GetnumericDataList();
	POSITION startPos = dataPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION highPos = highPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION lowPos = lowPacket->GetnumericDataList()->FindIndex(nRealStart);

	if(startPos == NULL || highPos == NULL || lowPos == NULL)
		return false;

	CBrush upBrush(graphColor.GetColor2());
	CBrush downBrush(graphColor.GetColor4());

	//팬 선택한다.
	CPen uplinePen, downlinePen;
	uplinePen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	downlinePen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());

	CPen* pOldPen = pDC->SelectObject(&uplinePen);
	CBrush* pOldBrush = pDC->SelectObject(&upBrush);

	double realMin = CDraw::CalLogValue(bLog, (double)dViewMin);
	double realMax = CDraw::CalLogValue(bLog, (double)dViewMax);

	CPoint startDataPt;

	nBarWidth = CMath::Round(double(nBarWidth)*1/3);
	if(nBarWidth < 1)
		nBarWidth = 1;
	double startData, highData, lowData;
	//============================================
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(startPos == NULL || highPos == NULL || lowPos == NULL) break;
		startData = CDraw::CalLogValue(bLog, dataPacket->GetnumericDataList()->GetNext(startPos));
		highData = CDraw::CalLogValue(bLog, highPacket->GetnumericDataList()->GetNext(highPos));
		lowData = CDraw::CalLogValue(bLog, lowPacket->GetnumericDataList()->GetNext(lowPos));

		// cj 2003.5.14 NonZero좌표점형 처리
		if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::NonZero_DotBar_Draw && startData == 0)
			continue;

		startDataPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, realMin, realMax, startData);
		if(startDataPt.y < 0)
			continue;
		if(bInvert == true)
			startDataPt.y = CDataConversion::InvertGraphDataPt(graphRegion, startDataPt.y);

		if(startData <= lowData){
			pDC->SelectObject(&uplinePen);
			pDC->SelectObject(&upBrush);
		}
		else{
			pDC->SelectObject(&downlinePen);
			pDC->SelectObject(&downBrush);
		}

		if(i >= dataPacket->GetStartPeriod()){
			int dotWidth = nBarWidth;
			if(nBarWidth > 4)
				dotWidth = 4;
			pDC->Ellipse(startDataPt.x - dotWidth, startDataPt.y - dotWidth, startDataPt.x + dotWidth, startDataPt.y + dotWidth);
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	uplinePen.DeleteObject();
	downlinePen.DeleteObject();
	upBrush.DeleteObject();
	downBrush.DeleteObject();
	
	return true;
}
//{{XScale 적용
bool CSingleBarDraw::DrawDotBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager && (pxScaleManager->GetThreadFlag() == THREAD_CALCULATION))
		return true;

	int nDTI, nDTI_Start, nDTI_End;
	pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	if( packetList.IsEmpty()) return false;
	CPacket* dataPacket = packetList.GetHead();
	
	CPacketList *pPacketList = dataPacket->GetPacketList();
	if( !pPacketList) return false;
	
	CPacket* highPacket = pPacketList->GetBaseData( _MTEXT( C0_HIGH));
	CPacket* lowPacket	= pPacketList->GetBaseData( _MTEXT( C0_LOW));
	if( !highPacket || !lowPacket) return false;
	
	//CList<double, double>* pDataList = dataPacket->GetnumericDataList();
	POSITION startPos = dataPacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION highPos = highPacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION lowPos = lowPacket->GetnumericDataList()->FindIndex(nDTI_Start);

	if(startPos == NULL || highPos == NULL || lowPos == NULL)
		return false;

	CBrush upBrush(graphColor.GetColor2());
	CBrush downBrush(graphColor.GetColor4());

	//팬 선택한다.
	CPen uplinePen, downlinePen;
	uplinePen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	downlinePen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());

	CPen* pOldPen = pDC->SelectObject(&uplinePen);
	CBrush* pOldBrush = pDC->SelectObject(&upBrush);

	double realMin = CDraw::CalLogValue(bLog, (double)dViewMin);
	double realMax = CDraw::CalLogValue(bLog, (double)dViewMax);

	CPoint startDataPt;

	nBarWidth = CMath::Round(double(nBarWidth)*1/3);
	if(nBarWidth < 1)
		nBarWidth = 1;
	double startData, highData, lowData;
	//============================================
	//for(int i = nRealStart; i <= nRealEnd; i++){

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

	CXScaleManager::PAREA parea;

	for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
	{
		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->NextDrawArea(&parea);
		parea = pblaAreafDTI->GetNext( dblCursor);

		if(parea == NULL) break;
		if(startPos == NULL || highPos == NULL || lowPos == NULL) break;
		startData = CDraw::CalLogValue(bLog, dataPacket->GetnumericDataList()->GetNext(startPos));
		highData = CDraw::CalLogValue(bLog, highPacket->GetnumericDataList()->GetNext(highPos));
		lowData = CDraw::CalLogValue(bLog, lowPacket->GetnumericDataList()->GetNext(lowPos));

		// cj 2003.5.14 NonZero좌표점형 처리
		if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::NonZero_DotBar_Draw && startData == 0)
			continue;

		startDataPt = CDataConversion::GetVerticalPoint(graphRegion, nDTI-nRealStart, dRealWidth, realMin, realMax, startData);
		if(startDataPt.y < 0)
			continue;
		if(bInvert == true)
			startDataPt.y = CDataConversion::InvertGraphDataPt(graphRegion, startDataPt.y);

		startDataPt.x = parea->center;

		if(startData <= lowData){
			pDC->SelectObject(&uplinePen);
			pDC->SelectObject(&upBrush);
		}
		else{
			pDC->SelectObject(&downlinePen);
			pDC->SelectObject(&downBrush);
		}

		if(nDTI >= dataPacket->GetStartPeriod()){
			int dotWidth = nBarWidth;
			if(nBarWidth > 4)
				dotWidth = 4;
			pDC->Ellipse(startDataPt.x - dotWidth, startDataPt.y - dotWidth, startDataPt.x + dotWidth, startDataPt.y + dotWidth);
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	uplinePen.DeleteObject();
	downlinePen.DeleteObject();
	upBrush.DeleteObject();
	downBrush.DeleteObject();
	
	return true;
}
//}}
void CSingleBarDraw::InvertBar(const CRect& graphRegion, CRect& rect)
{
	rect.top = CDataConversion::InvertGraphDataPt(graphRegion, rect.top);
	rect.bottom = CDataConversion::InvertGraphDataPt(graphRegion, rect.bottom);
	rect.NormalizeRect();
}

bool CSingleBarDraw::DrawBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		CIndicatorInfo* pIndicatorInfo, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION headPos = packetList.GetHeadPosition();
	if(pIndicatorInfo == NULL || headPos == NULL)
		return false;
	int nCnd = GetDrawType(pIndicatorInfo, false);
	if(nCnd <= 0)
		return false;
	
	// (2004.10.20, 배승원) Box Chart Packet을 시고저종 4개로 수정한다.
	if(packetList.GetCount() < 4) return false;
	
	CPacket* pForHighPacket = packetList.GetNext(headPos);	
	if(!pForHighPacket)	return false;

	if(CheckBoxBarType(pForHighPacket)){
		return DrawForwardBoxBar(pDC, graphRegion, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, nCnd, packetList, penThickness, bLog, bInvert);
	}

	return DrawBackwardBoxBar(pDC, graphRegion, graphColor, nBarWidth, dRealWidth, 
		nRealStart, nRealEnd, dViewMax, dViewMin, nCnd, packetList, penThickness, bLog, bInvert);
}
bool CSingleBarDraw::DrawForwardBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const int nCnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	int nIndex = 0;
	double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;

	// (2004.10.20, 배승원) Box Chart Packet을 시고저종 4개로 수정한다.
	CPacket* pOpenPacket	= packetList.GetAt( packetList.FindIndex( 0));
	CPacket* pForHighPacket	= packetList.GetAt( packetList.FindIndex( 1));	
	CPacket* pForLowPacket	= packetList.GetAt( packetList.FindIndex( 2));
	CPacket* pClosePacket	= packetList.GetAt( packetList.FindIndex( 3));

	if(!pForHighPacket || !pForLowPacket || !pOpenPacket || !pClosePacket)	return false;

	CList<double, double>* pOpenList  = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList  = pForHighPacket->GetnumericDataList();
	CList<double, double>* pLowList   = pForLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList = pClosePacket->GetnumericDataList();

	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)
		return false;

	POSITION openPos	= pOpenList->GetHeadPosition();
	POSITION highPos	= pHighList->GetHeadPosition();
	POSITION lowPos		= pLowList->GetHeadPosition();
	POSITION closePos	= pCloseList->GetHeadPosition();

	if(openPos == NULL || highPos == NULL || lowPos == NULL || closePos == NULL)
		return false;

	if(nCnd > pOpenList->GetCount())	return false;

	pForHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	pForLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);

	CPen upPen, downPen, bohapPen;
	double dBoxWidth = dRealWidth * (nCnd-1) + nBarWidth;
	int nUpLineThick	= CDraw::CalUpLineThickness(penThickness, (int)dBoxWidth);
	int nDownLineThick	= CDraw::CalDownLineThickness(penThickness, (int)dBoxWidth);
	int nBohapLineThick = CDraw::CalStationaryLineThickness(penThickness, (int)dBoxWidth);
	int nLineThick = nUpLineThick;

	if(dBoxWidth <= penThickness.GetThickness1() * 3){
		upPen.CreatePen		(PS_SOLID, 1, graphColor.GetColor1());
		downPen.CreatePen	(PS_SOLID, 1, graphColor.GetColor3());
		bohapPen.CreatePen	(PS_SOLID, 1, graphColor.GetColor5());
		nLineThick = 1;
	}
	else{
		upPen.CreatePen		(PS_SOLID, nUpLineThick, graphColor.GetColor1());
		downPen.CreatePen	(PS_SOLID, nUpLineThick, graphColor.GetColor3());
		bohapPen.CreatePen	(PS_SOLID, nUpLineThick, graphColor.GetColor5());
	}

	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
	CPen* pOldPen = (CPen *)pDC->SelectObject(&bohapPen);

	//log일 경우 ==================================
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	dHighMax = CDraw::CalLogValue(bLog, dHighMax);
	dLowMin = CDraw::CalLogValue(bLog, dLowMin);

	const int nCnt = pOpenList->GetCount();
	int nRest = nCnt % nCnd;
	int nStep=0;

	nIndex -= nRealStart;

	double dHigh = 0.0, dOpen = 0.0, dClose = 0.0, dLow = 0.0;
	dOpen = pOpenList->GetNext(openPos);
	dHigh  = pHighList->GetNext(highPos);
	dLow   = pLowList->GetNext(lowPos);

	for(int i=0; i<nCnt; i+=nStep){
		if(openPos == NULL || highPos == NULL || lowPos == NULL)
			break;

		for(int n = 0; n < nStep; n++){
			if(openPos == NULL || highPos == NULL || lowPos == NULL)
				break;

			dOpen = CDraw::CalLogValue(bLog, pOpenList->GetNext(openPos));
			dHigh = CDraw::CalLogValue(bLog, pHighList->GetNext(highPos));
			dLow  = CDraw::CalLogValue(bLog, pLowList->GetNext(lowPos));
		}
		int nLimit = 0;
		if(i == nCnt - nRest)
			nLimit = nRest;
		else
			nLimit = nCnd;
		for(int n = 0; n < nLimit; n++){
			if(closePos == NULL)
				break;
			dClose  = CDraw::CalLogValue(bLog, pCloseList->GetNext(closePos));
		}

		bool bOdd = false;
		if(nCnd % 2)
			bOdd = true;

		CPoint openPt = CDataConversion::GetVerticalPointBox(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, dOpen, false);
		CPoint highPt = CDataConversion::GetVerticalPointBox(graphRegion, nIndex+CMath::Round(nCnd/2), dRealWidth, dRealMin, dRealMax, dHigh, bOdd);
		CPoint lowPt  = CDataConversion::GetVerticalPointBox(graphRegion, nIndex+CMath::Round(nCnd/2), dRealWidth, dRealMin, dRealMax, dLow, bOdd);

		CPoint closePt(0, 0);
		if(i == nCnt - nRest)
			closePt = CDataConversion::GetVerticalPointBox(graphRegion, nIndex+nRest, dRealWidth, dRealMin, dRealMax, dClose, false);
		else
			closePt = CDataConversion::GetVerticalPointBox(graphRegion, nIndex+nCnd, dRealWidth, dRealMin, dRealMax, dClose, false);

		if(dClose >dOpen)
			pDC->SelectObject(&upPen);
		else if(dClose < dOpen)
			pDC->SelectObject(&downPen);
		else
			pDC->SelectObject(&bohapPen);

		CRect rect(openPt.x + 1, openPt.y, closePt.x, closePt.y);

		if(rect.top < graphRegion.top)
			rect.top = graphRegion.top;
		if(rect.bottom > graphRegion.bottom)
			rect.bottom = graphRegion.bottom;
		
		if(rect.bottom < graphRegion.top)
			rect.bottom = graphRegion.top;

		if(bInvert){
			rect.top	= CDataConversion::InvertGraphDataPt(graphRegion, rect.top);
			rect.bottom = CDataConversion::InvertGraphDataPt(graphRegion, rect.bottom);
			highPt.y	= CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
			lowPt.y		= CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
		}
		if(i >= pOpenPacket->GetStartPeriod()){
			if(dRealMin == dRealMax)
				CDraw::DrawLine(pDC, CPoint(rect.left, graphRegion.top + graphRegion.Height()/2), CPoint(rect.right, graphRegion.top + graphRegion.Height()/2));
			else{
				//bong을 그린다 ==========================
				if(dBoxWidth < 1)
					CDraw::DrawLine(pDC, highPt, lowPt.x, lowPt.y);
				else{
					//line 을 그린다.==========================
					if(bInvert){
						if(highPt.y >= rect.top){
							if(highPt.y > graphRegion.bottom)
								highPt.y = graphRegion.bottom;
							if(rect.top >= rect.bottom)
								CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.top));
							else
								CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.bottom));
						}
						if(lowPt.y <= rect.bottom){
							if(rect.bottom <= rect.top)
								CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.bottom));
							else
								CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.top));
						}
					}
					else{
						if(highPt.y <= rect.top){
							if(highPt.y < graphRegion.top)
								highPt.y = graphRegion.top;
							if(rect.top <= rect.bottom)
								CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.top));
							else
								CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.bottom));
						}
						if(lowPt.y >= rect.bottom){
							if(rect.bottom >= rect.top)
								CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.bottom));
							else
								CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.top));
						}
					}

					if(dClose > dOpen){
						if(graphColor.GetUpGraphFill() == true)
							pDC->SelectObject(&upPen);
					}
					else if(dClose < dOpen){
						if(graphColor.GetDownGraphFill() == true)
							pDC->SelectObject(&downPen);
					}
					else if(dClose == dOpen){
						pDC->SelectObject(&bohapPen);
						CDraw::DrawLine(pDC, CPoint(rect.left+1, rect.top), CPoint(rect.right-1, rect.top));///rect.bottom
					}

					if(i == nCnt - nRest){
						pDC->MoveTo(rect.right, rect.top);
						pDC->LineTo(rect.left, rect.top);
						pDC->LineTo(rect.left, rect.bottom);
						pDC->LineTo(rect.right, rect.bottom);
					}
   				    else
						CDraw::DrawRectangle(pDC, rect);
				}
			}
		}
		if(i == nCnt - nRest){
			nIndex += nRest;
			nStep = nRest;
		}
		else{
			nIndex += nCnd;
			nStep = nCnd;
		}
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upPen.DeleteObject();
	downPen.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}
bool CSingleBarDraw::DrawBackwardBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth,	const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const int nCnd, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	int nIndex = 0;
	double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;

	// (2004.10.20, 배승원) Box Chart Packet을 시고저종 4개로 수정한다.
	CPacket* pOpenPacket		= packetList.GetAt( packetList.FindIndex( 0));
	CPacket* pBackHighPacket	= packetList.GetAt( packetList.FindIndex( 1));
	CPacket* pBackLowPacket		= packetList.GetAt( packetList.FindIndex( 2));
	CPacket* pClosePacket		= packetList.GetAt( packetList.FindIndex( 3));

	if(!pBackHighPacket || !pBackLowPacket || !pOpenPacket || !pClosePacket)	return false;

	CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList = pBackHighPacket->GetnumericDataList();
	CList<double, double>* pLowList  = pBackLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList= pClosePacket->GetnumericDataList();

	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)
		return false;

	if(nCnd > pOpenList->GetCount())	return false;

	POSITION openPos = pOpenList->GetHeadPosition();
	POSITION highPos = pHighList->GetHeadPosition();
	POSITION lowPos  = pLowList->GetHeadPosition();
	POSITION closePos= pCloseList->GetHeadPosition();

	if(openPos == NULL || highPos == NULL || lowPos == NULL || closePos == NULL)
		return false;

	pBackHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	pBackLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);

	CPen upPen, downPen, bohapPen;

	double dBoxWidth = dRealWidth * (nCnd-1) + nBarWidth;
	int nUpLineThick	= CDraw::CalUpLineThickness(penThickness, (int)dBoxWidth);
	int nDownLineThick	= CDraw::CalDownLineThickness(penThickness, (int)dBoxWidth);
	int nBohapLineThick	= CDraw::CalStationaryLineThickness(penThickness, (int)dBoxWidth);

	int nLineThick = nUpLineThick;

	if(dBoxWidth <= penThickness.GetThickness1() * 3){
		upPen.CreatePen		(PS_SOLID, 1, graphColor.GetColor1());
		downPen.CreatePen	(PS_SOLID, 1, graphColor.GetColor3());
		bohapPen.CreatePen	(PS_SOLID, 1, graphColor.GetColor5());
		nLineThick = 1;
	}
	else{
		upPen.CreatePen		(PS_SOLID, nLineThick,	graphColor.GetColor1());
		downPen.CreatePen	(PS_SOLID, nLineThick,	graphColor.GetColor3());
		bohapPen.CreatePen	(PS_SOLID, nLineThick, graphColor.GetColor5());
	}

	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
	CPen* pOldPen = (CPen *)pDC->SelectObject(&bohapPen);
	pDC->SelectObject(GetStockObject(NULL_BRUSH));

	//log일 경우 ==================================
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	dHighMax = CDraw::CalLogValue(bLog, dHighMax);
	dLowMin  = CDraw::CalLogValue(bLog, dLowMin);

	bool bHighText = true, bLowText = true;

	const int nCnt = pOpenList->GetCount();
	int nRest = nCnt % nCnd;
	int nStep=0;

	nIndex -= nRealStart;

	double dOpen = pOpenList->GetNext(openPos);
	double dHigh = pHighList->GetNext(highPos);
	double dLow  = pLowList->GetNext(lowPos);
	double dClose= 0.0;

	for(int i=0; i<nCnt; i+=nStep){
		if(i==0){
			if(nRest){
				for(int n=0; n<nRest; n++){
					if(closePos == NULL)
						break;
					dClose = pCloseList->GetNext(closePos);
				}
			}
			else{
				for(int n=0; n<nCnd; n++){
					if(closePos == NULL)
						break;
					dClose = pCloseList->GetNext(closePos);
				}
			}
		}
		else if(nRest && i==nRest){
			for(int n=0; n<nRest; n++){
				if(openPos == NULL || highPos == NULL || lowPos == NULL)
					break;
				dOpen = pOpenList->GetNext(openPos);
				dHigh = pHighList->GetNext(highPos);
				dLow  = pLowList->GetNext(lowPos);
			}
			for(int n=0; n<nCnd; n++){
				if(closePos == NULL)
					break;
				dClose = pCloseList->GetNext(closePos);
			}
		}
		else{
			for(int n=0; n<nStep; n++){
				if(openPos == NULL || highPos == NULL || lowPos == NULL || closePos == NULL)
					break;
				dOpen	= pOpenList->GetNext(openPos);
				dHigh	= pHighList->GetNext(highPos);
				dLow	= pLowList->GetNext(lowPos);
				dClose  = pCloseList->GetNext(closePos);
			}
		}

		if(bLog){
			dOpen	= CDraw::CalLogValue(bLog, dOpen);
			dHigh	= CDraw::CalLogValue(bLog, dHigh);
			dLow	= CDraw::CalLogValue(bLog, dLow);
			dClose  = CDraw::CalLogValue(bLog, dClose);
		}

		CPoint openPt = CDataConversion::GetVerticalPointBox(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, dOpen, false);
		CPoint highPt(0, 0), lowPt(0, 0), closePt(0, 0);
		int n = 0;
		
		if(nRest && i == 0)
			n = nIndex + nRest - CMath::Round(nCnd/2.0);
		else
			n = nIndex + int(nCnd/2.0);	/// Round 하면 안된다.

		bool bOdd = false;
		if(nCnd % 2)
			bOdd = true;
		if(nRest && i==0)
		{
			highPt = CDataConversion::GetVerticalPointBox(graphRegion, n, dRealWidth, dRealMin, dRealMax, dHigh, bOdd);
			lowPt  = CDataConversion::GetVerticalPointBox(graphRegion, n, dRealWidth, dRealMin, dRealMax, dLow, bOdd);
			closePt = CDataConversion::GetVerticalPointBox(graphRegion, nIndex+nRest, dRealWidth, dRealMin, dRealMax, dClose, false);
		}
			
		else
		{
			highPt = CDataConversion::GetVerticalPointBox(graphRegion, n, dRealWidth, dRealMin, dRealMax, dHigh, bOdd);
			lowPt  = CDataConversion::GetVerticalPointBox(graphRegion, n, dRealWidth, dRealMin, dRealMax, dLow, bOdd);
			closePt = CDataConversion::GetVerticalPointBox(graphRegion, nIndex+nCnd, dRealWidth, dRealMin, dRealMax, dClose, false);
		}

		if(dClose > dOpen)
			pDC->SelectObject(&upPen);
		else if(dClose < dOpen)
			pDC->SelectObject(&downPen);
		else
			pDC->SelectObject(&bohapPen);

		CRect rect(openPt.x + 1, openPt.y, closePt.x, closePt.y);

		if(rect.top < graphRegion.top)
			rect.top = graphRegion.top;
		if(rect.bottom > graphRegion.bottom)
			rect.bottom = graphRegion.bottom;
		
		if(rect.bottom < graphRegion.top)
			rect.bottom = graphRegion.top;

		if(bInvert){
			rect.top	= CDataConversion::InvertGraphDataPt(graphRegion, rect.top);
			rect.bottom = CDataConversion::InvertGraphDataPt(graphRegion, rect.bottom);
			highPt.y	= CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
			lowPt.y		= CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
		}

		if(i >= pOpenPacket->GetStartPeriod()){
			if(dRealMin == dRealMax)
				CDraw::DrawLine(pDC, CPoint(rect.left, graphRegion.top + graphRegion.Height()/2), CPoint(rect.right, graphRegion.top + graphRegion.Height()/2));
			else{
				//bong을 그린다 ==========================
				if(dBoxWidth < 1)
					CDraw::DrawLine(pDC, highPt, lowPt.x, lowPt.y);
				else{
					//line 을 그린다.==========================
					if(bInvert){
						if(highPt.y >= rect.top){
							if(highPt.y > graphRegion.bottom)
								highPt.y = graphRegion.bottom;
							if(rect.top >= rect.bottom)
								CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.top));
							else
								CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.bottom));
						}
						if(lowPt.y <= rect.bottom){
							if(rect.bottom <= rect.top)
								CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.bottom));
							else
								CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.top));
						}
					}
					else{
						if(highPt.y <= rect.top){
							if(highPt.y < graphRegion.top)
								highPt.y = graphRegion.top;
							if(rect.top <= rect.bottom)
								CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.top));
							else
								CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.bottom));
						}
						if(lowPt.y >= rect.bottom){
							if(rect.bottom >= rect.top)
								CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.bottom));
							else
								CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.top));
						}
					}

					if(dClose > dOpen){
						if(graphColor.GetUpGraphFill() == true)
							pDC->SelectObject(&upPen);
					}
					else if(dClose < dOpen){
						if(graphColor.GetDownGraphFill() == true)
							pDC->SelectObject(&downPen);
					}
					else if(dClose == dOpen){
						pDC->SelectObject(&bohapPen);
						CDraw::DrawLine(pDC, CPoint(rect.left+1, rect.top), CPoint(rect.right-1, rect.top));///rect.bottom
					}
					if(nRest && i==0){
						pDC->MoveTo(rect.left+1, rect.top);
						pDC->LineTo(rect.right-1, rect.top);
						pDC->LineTo(rect.right-1, rect.bottom);
						pDC->LineTo(rect.left+1, rect.bottom);
					}
					else
						CDraw::DrawRectangle(pDC, rect);
				}
			}
		}
		if(nRest && i==0){
			nIndex += nRest;
			nStep = nRest;
		}
		else{
			nIndex += nCnd;
			nStep = nCnd;
		}
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upPen.DeleteObject();
	downPen.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}
//{{XScale 적용
bool CSingleBarDraw::DrawBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		CIndicatorInfo* pIndicatorInfo, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager && (pxScaleManager->GetThreadFlag() == THREAD_CALCULATION))
		return true;

	POSITION headPos = packetList.GetHeadPosition();
	if(pIndicatorInfo == NULL || headPos == NULL)
		return false;
	int nCnd = GetDrawType(pIndicatorInfo, false);
	if(nCnd <= 0)
		return false;
	
	// (2004.10.20, 배승원) Box Chart Packet을 시고저종 4개로 수정한다.
	if(packetList.GetCount() < 4) return false;
	
	CPacket* pForHighPacket = packetList.GetNext(headPos);	
	if(!pForHighPacket)	return false;

	if(CheckBoxBarType(pForHighPacket)){
		return DrawForwardBoxBar(pDC, graphRegion, graphColor, nBarWidth, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, nCnd, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, p_nColumn);
	}

	return DrawBackwardBoxBar(pDC, graphRegion, graphColor, nBarWidth, dRealWidth, 
		nRealStart, nRealEnd, dViewMax, dViewMin, nCnd, packetList, penThickness, bLog, bInvert,
		strRQ, pxScaleManager, p_nColumn);
}
bool CSingleBarDraw::DrawForwardBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const int nCnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	// (2004.10.20, 배승원) Box Chart Packet을 시고저종 4개로 수정한다.
	CPacket* pOpenPacket		= packetList.GetAt( packetList.FindIndex( 0));
	CPacket* pBackHighPacket	= packetList.GetAt( packetList.FindIndex( 1));
	CPacket* pBackLowPacket		= packetList.GetAt( packetList.FindIndex( 2));
	CPacket* pClosePacket		= packetList.GetAt( packetList.FindIndex( 3));
	if(!pBackHighPacket || !pBackLowPacket || !pOpenPacket || !pClosePacket)	return false;
	
	CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList = pBackHighPacket->GetnumericDataList();
	CList<double, double>* pLowList  = pBackLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList= pClosePacket->GetnumericDataList();
	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL) return false;
	
	const int nCnt = pOpenList->GetCount();
	if( nCnt < nCnd)	return false;

	int nDTI_Start = -1, nDTI_End = -1;
	pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if( nDTI_Start == -1 || nDTI_End == -1) return true;
	int nRest = ( nDTI_Start + 1) % nCnd;
	if( 1 < nRest)			nDTI_Start += 1 - nRest;
	else if( 0 == nRest)	nDTI_Start += 1 - nCnd;
	
	POSITION openPos = pOpenList->FindIndex( nDTI_Start);
	POSITION highPos = pHighList->FindIndex( nDTI_Start);
	POSITION lowPos  = pLowList->FindIndex( nDTI_Start);
	POSITION closePos= pCloseList->FindIndex( nDTI_Start);
	if( openPos == NULL || highPos == NULL || lowPos == NULL || closePos == NULL) return false;

	int nLineThick = 1;
	double dBoxWidth = dRealWidth * (nCnd-1) + nBarWidth;
	if( penThickness.GetThickness1() * 3 < dBoxWidth) nLineThick = CDraw::CalUpLineThickness( penThickness, (int)dBoxWidth);
	CPen upPen, downPen, bohapPen;
	upPen.CreatePen(	PS_SOLID, nLineThick, graphColor.GetColor1());
	downPen.CreatePen(	PS_SOLID, nLineThick, graphColor.GetColor3());
	bohapPen.CreatePen(	PS_SOLID, nLineThick, graphColor.GetColor5());

	CPen* pOldPen = (CPen *)pDC->SelectObject(&bohapPen);
	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(GetStockObject(NULL_BRUSH));

	//log일 경우 ==================================
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);

	bool bOdd = false;
	if( nCnd % 2) bOdd = true;
	for( int i = nDTI_Start; i <= nDTI_End && openPos && highPos && lowPos && closePos; i += nCnd)
	{
		double dOpen = pOpenList->GetNext(openPos);
		double dHigh = pHighList->GetNext(highPos);
		double dLow  = pLowList->GetNext(lowPos);
		double dClose= pCloseList->GetNext(closePos);
		for( int j = 1; j < nCnd && openPos && highPos && lowPos && closePos; j++)
		{
			pOpenList->GetNext(openPos);
			double dHigh2 = pHighList->GetNext(highPos);
			if( dHigh < dHigh2) dHigh = dHigh2;
			double dLow2 = pLowList->GetNext(lowPos);
			if( dLow2 < dLow) dLow = dLow2;
			dClose = pCloseList->GetNext( closePos);
		}

		if(bLog)
		{
			dOpen	= CDraw::CalLogValue(bLog, dOpen);
			dHigh	= CDraw::CalLogValue(bLog, dHigh);
			dLow	= CDraw::CalLogValue(bLog, dLow);
			dClose  = CDraw::CalLogValue(bLog, dClose);
		}

		CPoint openPt = CDataConversion::GetVerticalPointBox( graphRegion, i, dRealWidth, dRealMin, dRealMax, dOpen, false);
		CPoint highPt = CDataConversion::GetVerticalPointBox( graphRegion, i, dRealWidth, dRealMin, dRealMax, dHigh, false);
		CPoint lowPt  = CDataConversion::GetVerticalPointBox( graphRegion, i, dRealWidth, dRealMin, dRealMax, dLow, false);
		CPoint closePt = CDataConversion::GetVerticalPointBox( graphRegion, i, dRealWidth, dRealMin, dRealMax, dClose, false);

		int n = i + int( nCnd/2.0);	/// Round 하면 안된다.
		//{{XScale 적용
		CXScaleManager::AREA area;
		if( pxScaleManager->GetReverse())	//XScale이 Reverse인 경우
		{
			pxScaleManager->GetAREAfRQnDTI( strRQ, i, p_nColumn, &area);
			openPt.x = area.right;
			pxScaleManager->GetAREAfRQnDTI( strRQ, n, p_nColumn, &area);
			highPt.x = lowPt.x = area.center;
			pxScaleManager->GetAREAfRQnDTI( strRQ, i + nCnd - 1, p_nColumn, &area);
			closePt.x = area.left;
		}
		else
		{
			pxScaleManager->GetAREAfRQnDTI( strRQ, i, p_nColumn, &area);
			openPt.x = area.left;
			pxScaleManager->GetAREAfRQnDTI( strRQ, n, p_nColumn, &area);
			highPt.x = lowPt.x = area.center;
			pxScaleManager->GetAREAfRQnDTI( strRQ, i + nCnd - 1, p_nColumn, &area);
			closePt.x = area.right;
		}
		//}}

		if( dClose > dOpen)		pDC->SelectObject(&upPen);
		else if(dClose < dOpen)	pDC->SelectObject(&downPen);
		else					pDC->SelectObject(&bohapPen);

		CRect rect( openPt.x + 1, openPt.y, closePt.x, closePt.y);
		if(bInvert)
		{
			rect.top	= CDataConversion::InvertGraphDataPt(graphRegion, rect.top);
			rect.bottom = CDataConversion::InvertGraphDataPt(graphRegion, rect.bottom);
			highPt.y	= CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
			lowPt.y		= CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
		}

		if( dRealMin == dRealMax)
			CDraw::DrawLine(pDC, CPoint(rect.left, graphRegion.top + graphRegion.Height()/2), CPoint(rect.right, graphRegion.top + graphRegion.Height()/2));
		else{
			//bong을 그린다 ==========================
			if(dBoxWidth < 1)
				CDraw::DrawLine(pDC, highPt, lowPt.x, lowPt.y);
			else{
				//line 을 그린다.==========================
				if(bInvert){
					if(highPt.y >= rect.top){
						if(highPt.y > graphRegion.bottom)
							highPt.y = graphRegion.bottom;
						if(rect.top >= rect.bottom)
							CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.top));
						else
							CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.bottom));
					}
					if(lowPt.y <= rect.bottom){
						if(rect.bottom <= rect.top)
							CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.bottom));
						else
							CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.top));
					}
				}
				else{
					if(highPt.y <= rect.top){
						if(highPt.y < graphRegion.top)
							highPt.y = graphRegion.top;
						if(rect.top <= rect.bottom)
							CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.top));
						else
							CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.bottom));
					}
					if(lowPt.y >= rect.bottom){
						if(rect.bottom >= rect.top)
							CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.bottom));
						else
							CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.top));
					}
				}

				if(dClose > dOpen){
					if(graphColor.GetUpGraphFill() == true)
						pDC->SelectObject(&upPen);
				}
				else if(dClose < dOpen){
					if(graphColor.GetDownGraphFill() == true)
						pDC->SelectObject(&downPen);
				}
				else if(dClose == dOpen){
					pDC->SelectObject(&bohapPen);
					CDraw::DrawLine(pDC, CPoint(rect.left+1, rect.top), CPoint(rect.right-1, rect.top));///rect.bottom
				}

				if(i == nCnt - nRest){
					pDC->MoveTo(rect.right, rect.top);
					pDC->LineTo(rect.left, rect.top);
					pDC->LineTo(rect.left, rect.bottom);
					pDC->LineTo(rect.right, rect.bottom);
				}
   				else
					CDraw::DrawRectangle(pDC, rect);
			}
		}
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upPen.DeleteObject();
	downPen.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}
bool CSingleBarDraw::DrawBackwardBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth,	const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const int nCnd, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	// (2004.10.20, 배승원) Box Chart Packet을 시고저종 4개로 수정한다.
	CPacket* pOpenPacket		= packetList.GetAt( packetList.FindIndex( 0));
	CPacket* pBackHighPacket	= packetList.GetAt( packetList.FindIndex( 1));
	CPacket* pBackLowPacket		= packetList.GetAt( packetList.FindIndex( 2));
	CPacket* pClosePacket		= packetList.GetAt( packetList.FindIndex( 3));
	if(!pBackHighPacket || !pBackLowPacket || !pOpenPacket || !pClosePacket)	return false;
	
	CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList = pBackHighPacket->GetnumericDataList();
	CList<double, double>* pLowList  = pBackLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList= pClosePacket->GetnumericDataList();
	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL) return false;
	
	const int nCnt = pOpenList->GetCount();
	if( nCnt < nCnd)	return false;

	int nRest = nCnt % nCnd;
	int nDTI_Start = -1, nDTI_End = -1;
	pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if( nDTI_Start == -1 || nDTI_End == -1) return true;
	if( nRest < nDTI_End + 1)
	{
		int nRest2 = ( nDTI_End + 1 - nRest) % nCnd;
		if( 0 < nRest2) nDTI_End += nCnd - nRest2;
	}
	
	POSITION openPos = pOpenList->FindIndex( nDTI_End);
	POSITION highPos = pHighList->FindIndex( nDTI_End);
	POSITION lowPos  = pLowList->FindIndex( nDTI_End);
	POSITION closePos= pCloseList->FindIndex( nDTI_End);
	if( openPos == NULL || highPos == NULL || lowPos == NULL || closePos == NULL) return false;

	int nLineThick = 1;
	double dBoxWidth = dRealWidth * (nCnd-1) + nBarWidth;
	if( penThickness.GetThickness1() * 3 < dBoxWidth) nLineThick = CDraw::CalUpLineThickness( penThickness, (int)dBoxWidth);
	CPen upPen, downPen, bohapPen;
	upPen.CreatePen(	PS_SOLID, nLineThick, graphColor.GetColor1());
	downPen.CreatePen(	PS_SOLID, nLineThick, graphColor.GetColor3());
	bohapPen.CreatePen(	PS_SOLID, nLineThick, graphColor.GetColor5());

	CPen* pOldPen = (CPen *)pDC->SelectObject(&bohapPen);
	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(GetStockObject(NULL_BRUSH));

	//log일 경우 ==================================
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);

	bool bOdd = false;
	if( nCnd % 2) bOdd = true;
	for( int i = nDTI_End; nDTI_Start <= i; i -= nCnd)
	{
		double dOpen = pOpenList->GetPrev(openPos);
		double dHigh = pHighList->GetPrev(highPos);
		double dLow  = pLowList->GetPrev(lowPos);
		double dClose= pCloseList->GetPrev(closePos);
		for( int j = 1; j < nCnd && openPos && highPos && lowPos && closePos; j++)
		{
			dOpen = pOpenList->GetPrev(openPos);
			double dHigh2 = pHighList->GetPrev(highPos);
			if( dHigh < dHigh2) dHigh = dHigh2;
			double dLow2 = pLowList->GetPrev(lowPos);
			if( dLow2 < dLow) dLow = dLow2;
			pCloseList->GetPrev(closePos);
		}

		if(bLog)
		{
			dOpen	= CDraw::CalLogValue(bLog, dOpen);
			dHigh	= CDraw::CalLogValue(bLog, dHigh);
			dLow	= CDraw::CalLogValue(bLog, dLow);
			dClose  = CDraw::CalLogValue(bLog, dClose);
		}

		CPoint openPt = CDataConversion::GetVerticalPointBox( graphRegion, i, dRealWidth, dRealMin, dRealMax, dOpen, false);
		CPoint highPt = CDataConversion::GetVerticalPointBox( graphRegion, i, dRealWidth, dRealMin, dRealMax, dHigh, false);
		CPoint lowPt  = CDataConversion::GetVerticalPointBox( graphRegion, i, dRealWidth, dRealMin, dRealMax, dLow, false);
		CPoint closePt = CDataConversion::GetVerticalPointBox( graphRegion, i, dRealWidth, dRealMin, dRealMax, dClose, false);

		int n = i - int( nCnd/2.0);	/// Round 하면 안된다.
		//{{XScale 적용
		CXScaleManager::AREA area;
		if( pxScaleManager->GetReverse())	//XScale이 Reverse인 경우
		{
			pxScaleManager->GetAREAfRQnDTI( strRQ, i, p_nColumn, &area);
			closePt.x = area.left;
			pxScaleManager->GetAREAfRQnDTI( strRQ, n, p_nColumn, &area);
			highPt.x = lowPt.x = area.center;
			pxScaleManager->GetAREAfRQnDTI( strRQ, i - nCnd + 1, p_nColumn, &area);
			openPt.x = area.right;
		}
		else
		{
			pxScaleManager->GetAREAfRQnDTI( strRQ, i, p_nColumn, &area);
			closePt.x = area.right;
			pxScaleManager->GetAREAfRQnDTI( strRQ, n, p_nColumn, &area);
			highPt.x = lowPt.x = area.center;
			pxScaleManager->GetAREAfRQnDTI( strRQ, i - nCnd + 1, p_nColumn, &area);
			openPt.x = area.left;
		}
		//}}

		if( dClose > dOpen)		pDC->SelectObject(&upPen);
		else if(dClose < dOpen)	pDC->SelectObject(&downPen);
		else					pDC->SelectObject(&bohapPen);

		CRect rect( openPt.x + 1, openPt.y, closePt.x, closePt.y);
		if(bInvert)
		{
			rect.top	= CDataConversion::InvertGraphDataPt(graphRegion, rect.top);
			rect.bottom = CDataConversion::InvertGraphDataPt(graphRegion, rect.bottom);
			highPt.y	= CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
			lowPt.y		= CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
		}

		if( dRealMin == dRealMax)
			CDraw::DrawLine(pDC, CPoint(rect.left, graphRegion.top + graphRegion.Height()/2), CPoint(rect.right, graphRegion.top + graphRegion.Height()/2));
		else
		{
			//bong을 그린다 ==========================
			if( dBoxWidth < 1)
				CDraw::DrawLine(pDC, highPt, lowPt.x, lowPt.y);
			else
			{
				//line 을 그린다.==========================
				if( bInvert)
				{
					if( highPt.y >= rect.top){
						if(highPt.y > graphRegion.bottom)
							highPt.y = graphRegion.bottom;
						if(rect.top >= rect.bottom)
							CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.top));
						else
							CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.bottom));
					}
					if(lowPt.y <= rect.bottom){
						if(rect.bottom <= rect.top)
							CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.bottom));
						else
							CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.top));
					}
				}
				else{
					if(highPt.y <= rect.top){
						if(highPt.y < graphRegion.top)
							highPt.y = graphRegion.top;
						if(rect.top <= rect.bottom)
							CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.top));
						else
							CDraw::DrawLine(pDC, highPt, CPoint(highPt.x, rect.bottom));
					}
					if(lowPt.y >= rect.bottom){
						if(rect.bottom >= rect.top)
							CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.bottom));
						else
							CDraw::DrawLine(pDC, lowPt, CPoint(lowPt.x, rect.top));
					}
				}

				if(dClose > dOpen){
					if(graphColor.GetUpGraphFill() == true)
						pDC->SelectObject(&upPen);
				}
				else if(dClose < dOpen){
					if(graphColor.GetDownGraphFill() == true)
						pDC->SelectObject(&downPen);
				}
				else if(dClose == dOpen){
					pDC->SelectObject(&bohapPen);
					CDraw::DrawLine(pDC, CPoint(rect.left+1, rect.top), CPoint(rect.right-1, rect.top));///rect.bottom
				}
				if(nRest && i==0){
					pDC->MoveTo(rect.left+1, rect.top);
					pDC->LineTo(rect.right-1, rect.top);
					pDC->LineTo(rect.right-1, rect.bottom);
					pDC->LineTo(rect.left+1, rect.bottom);
				}
				else
					CDraw::DrawRectangle(pDC, rect);
			}
		}
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upPen.DeleteObject();
	downPen.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}
//}}
bool CSingleBarDraw::DrawLeftRightBar_Base(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, const bool bIsFixedRate)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)
		return false;
	CPacket* pPacket = packetList.GetNext(packetPos);
	if(pPacket == NULL)
		return false;

	POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);
	
	if(pos == NULL) 
		return false;
	
	CBrush upBrush(graphColor.GetColor2());
	CBrush downBrush(graphColor.GetColor4());
	CBrush* pOldBrush = pDC->SelectObject(&upBrush);

	if(graphColor.GetUpGraphFill() == false)
		pDC->SelectObject(GetStockObject(NULL_BRUSH));

	CPen upPen;
	CPen downPen;
	int lineThickness = CDraw::CalUpLineThickness(penThickness, nBarWidth);
	if(nBarWidth <= lineThickness * 3){
		upPen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
		downPen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
	}
	else{
		upPen.CreatePen(PS_SOLID, lineThickness, graphColor.GetColor1());
		downPen.CreatePen(PS_SOLID, lineThickness, graphColor.GetColor3());
	}
	CPen* pOldPen = pDC->SelectObject(&upPen);

	int nCount = pPacket->GetCount();
	double dWidth = (double)graphRegion.Height()/(double)nCount;

	nBarWidth = int(dWidth*0.8);
	nBarWidth = nBarWidth / 2;

	// 기준가/min/max를 가지고 오는부분.
	double dStandard = 0.0, dRealMin = 0.0, dRealMax = 0.0;
	GetMinMaxAndStandard_LeftRightBar(pPacket, bIsFixedRate, dStandard, dRealMin, dRealMax);

	CPoint standardPt = CDataConversion::GetHorizontalPoint(graphRegion, 1, dWidth, dRealMin, dRealMax, dStandard);
	if(dRealMin <= dStandard && dRealMax >= dStandard)
		CDraw::DrawLine(pDC, CPoint(standardPt.x, graphRegion.top), CPoint(standardPt.x, graphRegion.bottom));

	//============================================
	for(int i = 0; i < nCount; i++){
		if(pos == NULL)	
			break;
		double dData = pPacket->GetnumericDataList()->GetNext(pos);
		// data가 0이고 기준선도 0 일 경우에 바형으로 표현하게 되면 
		// 데이터가 있는 것처럼 느껴지므로 화면에 표현하지 않기 위해 처리함
		if(dData == 0 && dStandard == 0) 
			continue;
		
		CPoint dataPt = CDataConversion::GetHorizontalPoint(graphRegion, i, dWidth, dRealMin, dRealMax, dData);
		CRect rect(standardPt.x, dataPt.y - nBarWidth, dataPt.x, dataPt.y + nBarWidth);
		
		if(standardPt.x < dataPt.x){
			pDC->SelectObject(&upPen);
			pDC->SelectObject(&upBrush);
		}
		else if(standardPt.x == dataPt.x){
			if(dData >= dStandard){
				pDC->SelectObject(&upPen);
				pDC->SelectObject(&upBrush);
			}
			else{
				pDC->SelectObject(&downPen);
				pDC->SelectObject(&downBrush);
			}
		}
		else{
			pDC->SelectObject(&downPen);
			pDC->SelectObject(&downBrush);
		}

		if(nBarWidth < 1)
			CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.right, rect.top));
		else{
			if(rect.left == rect.right)
				CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom));
			else
				CDraw::DrawRectangle(pDC, rect);
		}
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	upBrush.DeleteObject();
	upPen.DeleteObject();
	downBrush.DeleteObject();
	downPen.DeleteObject();
	return true;
}

bool CSingleBarDraw::DrawLeftRightBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness)
{
	return DrawLeftRightBar_Base(pDC, graphRegion, graphColor, nBarWidth, dRealWidth, 
		nRealStart, nRealEnd, packetList, penThickness, false);
}
bool CSingleBarDraw::DrawFixedRateLeftRightBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness)
{
	return DrawLeftRightBar_Base(pDC, graphRegion, graphColor, nBarWidth, dRealWidth, 
		nRealStart, nRealEnd, packetList, penThickness, true);
}

bool CSingleBarDraw::DrawLeftRightCombBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness)
{
	int count = 0;
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	return false;
	CPacket* m_volumnPacket1 = packetList.GetNext(pos);
	if(pos == NULL) return false;
	CPacket* m_volumnPacket2 = packetList.GetNext(pos);

	POSITION volumePos1 = m_volumnPacket1->GetnumericDataList()->FindIndex(nRealStart-1);
	POSITION volumePos2 = m_volumnPacket2->GetnumericDataList()->FindIndex(nRealStart-1);
	
	if(volumePos1 == NULL || volumePos2 == NULL) 
		return false;
	
	int dataCount = nRealEnd - nRealStart + 1;

	//min 과 max를 새로 구한다.=====================================================
	double dMax = 0.0, dMin = 0.0, tempmax = 0.0, tempmin = 0.0;
	m_volumnPacket1->GetMinMax(nRealStart -1, nRealEnd, dMin, dMax); 
	dMin = fabs(dMin);
	dMax = fabs(dMax);
	if(dMin > dMax)
		dMax = dMin;
	m_volumnPacket2->GetMinMax(nRealStart -1, nRealEnd, tempmax, tempmin); 
	tempmin = fabs(tempmin);
	tempmax = fabs(tempmax);
	if(tempmin > tempmax)
		tempmax = tempmin;
	if(tempmax > dMax)
		dMax = tempmax;
	dMin = dMax * -1;
	//==============================================================================
	double viewMin = 0.0, viewMax = 0.0;
	GetMarginMinMax(dMin, dMax, viewMin, viewMax);

	CBrush upBrush(graphColor.GetColor2());
	CBrush downBrush(graphColor.GetColor4());

	CPen upPen;
	CPen downPen;
	CPen bohapPen;

	int lineThickness = CDraw::CalUpLineThickness(penThickness, nBarWidth);
	if(nBarWidth <= lineThickness * 3){
		upPen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
		downPen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
	}
	else{
		upPen.CreatePen(PS_SOLID, lineThickness, graphColor.GetColor1());
		downPen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
	}
	
	bohapPen.CreatePen(PS_SOLID, lineThickness, graphColor.GetColor5());

	CBrush* pOldBrush = pDC->SelectObject(&upBrush);
	CPen* pOldPen = pDC->SelectObject(&bohapPen);
	
	double Width = (double)graphRegion.Height()/(double)dataCount;

	nBarWidth = int(Width*0.8);
	nBarWidth = nBarWidth / 2;
	// 기준선 = 0.0
	CPoint standardPt = CDataConversion::GetHorizontalPoint(graphRegion, 1, Width, viewMin, viewMax, 0.0);
	if(viewMin <= 0.0 && viewMax >= 0.0)
		CDraw::DrawLine(pDC, CPoint(standardPt.x, graphRegion.top), CPoint(standardPt.x, graphRegion.bottom));

	//============================================
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(volumePos1 == NULL || volumePos2 == NULL)
			break;
		count++;
		//dVolume에 해당하는 그래프 영역의 포인트를 가지고 온다.
		double Data1 = m_volumnPacket1->GetnumericDataList()->GetNext(volumePos1);
		if(Data1 < 0.0)
			Data1 = Data1 * -1.0;

		double Data2 = m_volumnPacket2->GetnumericDataList()->GetNext(volumePos2);
		if(Data2 > 0.0)
			Data2 = Data2 * -1.0;

		CPoint dataPt1 = CDataConversion::GetHorizontalPoint(graphRegion, count, Width, viewMin, viewMax, Data1);
		CPoint dataPt2 = CDataConversion::GetHorizontalPoint(graphRegion, count, Width, viewMin, viewMax, Data2);
		CRect rect1(standardPt.x, dataPt1.y - nBarWidth, dataPt1.x, dataPt1.y + nBarWidth);
		CRect rect2(standardPt.x, dataPt2.y - nBarWidth, dataPt2.x, dataPt2.y + nBarWidth);

		if(graphColor.GetUpGraphFill() == false)
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		else
			pDC->SelectObject(&upBrush);
		pDC->SelectObject(&upPen);
		if(nBarWidth < 1)
			CDraw::DrawLine(pDC, CPoint(rect1.left, rect1.top), CPoint(rect1.right, rect1.top));
		else{
			if(rect1.left == rect1.right)
				CDraw::DrawLine(pDC, CPoint(rect1.left, rect1.top), CPoint(rect1.left, rect1.bottom));
			else
				CDraw::DrawRectangle(pDC, rect1);
		}
		
		if(graphColor.GetDownGraphFill() == false)
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		else
			pDC->SelectObject(&downBrush);
		pDC->SelectObject(&downPen);
		if(nBarWidth < 1)
			CDraw::DrawLine(pDC, CPoint(rect2.left, rect2.top), CPoint(rect2.right, rect2.top));
		else{
			if(rect2.left == rect2.right)
				CDraw::DrawLine(pDC, CPoint(rect2.left, rect2.top), CPoint(rect2.left, rect2.bottom));
			else
				CDraw::DrawRectangle(pDC, rect2);
		}

	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	upBrush.DeleteObject();
	upPen.DeleteObject();
	downBrush.DeleteObject();
	downPen.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}
bool CSingleBarDraw::DrawGroupBar(CDC* pDC, const const CRect& graphRegion, const	CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, const int nBarWidth, const double dRealWidth, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, CIndicatorInfo* pIndicatorInfo,
		const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	return false;
	CPacket* pPacket1 = packetList.GetNext(pos);
	if(pos == NULL)	return false;
	CPacket* pPacket2 = packetList.GetNext(pos);

	if(pPacket1 == NULL || pPacket2 == NULL)	return false;

	POSITION pos1 = pPacket1->GetnumericDataList()->FindIndex(nRealStart);
	POSITION pos2 = pPacket2->GetnumericDataList()->FindIndex(nRealStart);

	if(pos1 == NULL || pos2 == NULL) return false;

	int realDrawStart = pPacket1->GetStartPeriod();
	int tempStart = pPacket2->GetStartPeriod();
	if(realDrawStart < tempStart)
		realDrawStart = tempStart;

	CBrush upBrush1(graphColor.GetColor1());
	CBrush upBrush2(graphColor.GetColor3());
	CBrush downBrush1(graphColor.GetColor2());
	CBrush downBrush2(graphColor.GetColor4());
	
	CPen upPen1, upPen2, downPen1, downPen2, standardPen;
	upPen1.CreatePen(PS_SOLID,	1,	graphColor.GetColor1());
	upPen2.CreatePen(PS_SOLID,	1,	graphColor.GetColor3());
	downPen1.CreatePen(PS_SOLID,	1,	graphColor.GetColor2());
	downPen2.CreatePen(PS_SOLID,	1,	graphColor.GetColor4());
	standardPen.CreatePen(PS_SOLID, 1, graphColor.GetColor6());

	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&upBrush1);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&standardPen);

	// 기준가를 가지고 오는부분.
	double dStandard = pPacket1->GetStandardValue();
//	if(dStandard < 0)
//		dStandard = 0.0;
	
	// 기준선을 그려주는 부분 
	CPoint standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dStandard));
	if(bInvert == true)	
		standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
	if(dStandard >= dViewMin && dStandard <= dViewMax)
		CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));

	double dPacketType1 = pPacket1->GetTypeFormatDouble();
	double dPacketType2 = pPacket2->GetTypeFormatDouble();

//	int nMargin = CMath::Round(nBarWidth / 8);

	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, 0), false);
	pDC->SelectObject(&upPen1);

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos1 == NULL || pos2 == NULL)
			break;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// 첫번째 바
		double dData1 = pPacket1->GetnumericDataList()->GetNext(pos1);
		CPoint curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dData1), false);
		CPoint centerPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dData1), true);

		if(dData1 >= dStandard){
			if(graphColor.GetUpGraphFill() == true)
				pDC->SelectObject(&upBrush1);
			else
				pDC->SelectObject(GetStockObject(NULL_BRUSH));
			pDC->SelectObject(&upPen1);
		}
		else if(dData1 < dStandard){
			if(graphColor.GetUpGraphFill() == true)
				pDC->SelectObject(&downBrush1);
			else
				pDC->SelectObject(GetStockObject(NULL_BRUSH));
			pDC->SelectObject(&downPen1);
		}

		CRect rect1(prePt.x, curPt.y, curPt.x, standardPt.y);

		if(rect1.Width() > 1)
			rect1.left += 1;
		
		/// 이거 안하면 하단 스케일 잡아먹는 경우가 생긴다.
		if(rect1.top < graphRegion.top)
			rect1.top = graphRegion.top;
		if(rect1.bottom < graphRegion.top)
			rect1.bottom = graphRegion.top;
		if(rect1.bottom > graphRegion.bottom )
			rect1.bottom = graphRegion.bottom;
		if(rect1.top > graphRegion.bottom )
			rect1.top = graphRegion.bottom;

		if(bInvert == true)
			InvertBar(graphRegion, rect1);
		// 첫번재 바를 그려주는 부분 
		if(i >= realDrawStart && dData1 != 0.0){
			// cj 2003.5.14 NonZero좌표점형 처리
			if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::DotBar_Draw ||
				drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::NonZero_DotBar_Draw){
				if(nBarWidth < 1)
					DrawDotBar(pDC, CPoint(rect1.left + rect1.Width() / 2, rect1.top), 1);
				else
					DrawDotBar(pDC, CPoint(rect1.left + rect1.Width() / 2, rect1.top), rect1.Width() / 2);

			}
			else{
				if(rect1.Width() < 1)
					CDraw::DrawLine(pDC, CPoint(rect1.left, rect1.top), CPoint(rect1.left, rect1.bottom));
				else{
					if(rect1.top == rect1.bottom)
						CDraw::DrawLine(pDC, CPoint(rect1.left, rect1.bottom), CPoint(rect1.right, rect1.bottom));
					else
						CDraw::DrawRectangle(pDC, rect1);
				}
			}
		}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// 두번째 바
		double dData2 = pPacket2->GetnumericDataList()->GetNext(pos2);
		curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, dData2), false);

		if(dData2 >= dStandard){
			if(graphColor.GetDownGraphFill() == true)
				pDC->SelectObject(&upBrush2);
			else
				pDC->SelectObject(GetStockObject(NULL_BRUSH));
			pDC->SelectObject(&upPen2);
		}
		else if(dData2 < dStandard){
			if(graphColor.GetDownGraphFill() == true)
				pDC->SelectObject(&downBrush2);
			else
				pDC->SelectObject(GetStockObject(NULL_BRUSH));
			pDC->SelectObject(&downPen2);
		}

		CRect rect2(prePt.x, curPt.y, curPt.x, standardPt.y);

		if(rect2.Width() > 1)
			rect2.left += 1;

		/// 이거 안하면 하단 스케일 잡아먹는 경우가 생긴다.
		if(rect2.top < graphRegion.top)
			rect2.top = graphRegion.top;
		if(rect2.bottom < graphRegion.top)
			rect2.bottom = graphRegion.top;
		if(rect2.bottom > graphRegion.bottom )
			rect2.bottom = graphRegion.bottom;
		if(rect2.top > graphRegion.bottom )
			rect2.top = graphRegion.bottom;

		if(bInvert == true)			
			InvertBar(graphRegion, rect2);
		// 두번재 바를 그려주는 부분 
		if(i >= realDrawStart && dData2 != 0.0){
			// cj 2003.5.14 NonZero좌표점형 처리
			if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::DotBar_Draw ||
				drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::NonZero_DotBar_Draw){
				if(nBarWidth/2 <= 1)
					DrawDotBar(pDC, CPoint(curPt.x, rect2.top), 1);
				else
					DrawDotBar(pDC, CPoint(rect2.right - rect2.Width() / 2, rect2.top), rect2.Width() / 2);

			}
			else{
				if(rect1.Width() <= 1)
					CDraw::DrawLine(pDC, CPoint(rect2.left, rect2.top), CPoint(rect2.left, rect2.bottom));
				else{
					if(rect2.top == rect2.bottom)
						CDraw::DrawLine(pDC, CPoint(rect2.left, rect2.bottom), CPoint(rect2.right, rect2.bottom));
					else
						CDraw::DrawRectangle(pDC, rect2);
				}
			}
		}
		prePt = curPt;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upBrush1.DeleteObject();
	upBrush2.DeleteObject();
	downBrush1.DeleteObject();
	downBrush2.DeleteObject();

	upPen1.DeleteObject();
	upPen2.DeleteObject();
	downPen1.DeleteObject();
	downPen2.DeleteObject();
	
	standardPen.DeleteObject();
	return true;
}
bool CSingleBarDraw::DrawRatioBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)		return false;
	CPacket* pVolumePacket = packetList.GetNext(pos);
	if(!pVolumePacket)	return false;
	
	POSITION volumePos = pVolumePacket->GetnumericDataList()->FindIndex(nRealStart);

	if(volumePos == NULL)	return false;

	CBrush upBrush(graphColor.GetColor2()), dnBarBrush(graphColor.GetColor4()), stBarBrush(graphColor.GetColor6());
	CBrush* pOldBrush = pDC->SelectObject(&upBrush);

	int uplineThickness =			CDraw::CalUpLineThickness			(penThickness, nBarWidth);
	int downlineThickness =			CDraw::CalDownLineThickness			(penThickness, nBarWidth);
	int stationarylineThickness =	CDraw::CalStationaryLineThickness	(penThickness, nBarWidth);

	CPen upPen, dnBarPen, stBarPen;
	int lineThickness = CDraw::CalUpLineThickness(penThickness, nBarWidth);
	if(nBarWidth <= lineThickness * 3){
		upPen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
		dnBarPen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
		stBarPen.CreatePen(PS_SOLID, 1, graphColor.GetColor5());
		lineThickness = 1;
	}
	else{
		upPen.CreatePen(PS_SOLID, uplineThickness,			graphColor.GetColor1());
		dnBarPen.CreatePen(PS_SOLID, downlineThickness,			graphColor.GetColor3());
		stBarPen.CreatePen(PS_SOLID, stationarylineThickness,	graphColor.GetColor5());
	}
	CPen* pOldPen = pDC->SelectObject(&upPen);
	//============================================
///	nBarWidth = nBarWidth - lineThickness;
	nBarWidth = nBarWidth / 2;

	double curValue=0;
	double preValue = pVolumePacket->GetnumericDataList()->GetAt(volumePos);
	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, preValue), false);

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(volumePos == NULL) break;
		curValue = pVolumePacket->GetnumericDataList()->GetNext(volumePos);
 		CPoint curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, curValue), false);
		/// DotBar를 위하여 미리 구함.
		CPoint centerPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, curValue), true);
 	 		
		CRect rect(prePt.x, curPt.y, curPt.x, graphRegion.bottom);
		if(curPt.x - prePt.x > 1)
			rect.left += 1;
		if(bInvert == true)
			InvertBar(graphRegion, rect);

		if(graphColor.GetUpGraphFill() == false)
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		else
			pDC->SelectObject(&upBrush);
		pDC->SelectObject(&upPen);
	
		///여기부터 type
		if(i >= pVolumePacket->GetStartPeriod()){
			if(curValue<preValue){
				if(graphColor.GetDownGraphFill() == false)
					pDC->SelectObject(GetStockObject(NULL_BRUSH));
				else
					pDC->SelectObject(&dnBarBrush);
				pDC->SelectObject(&dnBarPen);
			}

			switch(drawStyle.GetGraphBarDrawStyle())
			{
				case CGraphBaseData::Bar_Draw:
				{
					if(nBarWidth < 1)
					CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom));
					else if(rect.top == rect.bottom)
						CDraw::DrawLine(pDC, CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom));
					else
						CDraw::DrawRectangle(pDC, rect);
					
					break;
				}
				case CGraphBaseData::NonZero_DotBar_Draw:
				{
					if(curValue == 0) // dVolumn이 0이 아니면 다음 case를 탄다.
						break;	
				}
				case CGraphBaseData::DotBar_Draw:
				{
					if(nBarWidth/2 < 1)
						DrawDotBar(pDC, centerPt, 1);
					else
						DrawDotBar(pDC, centerPt, nBarWidth/2);
					break;
				}
			
			}
		}
		preValue = curValue;
		prePt = curPt;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	upBrush.DeleteObject();
	upPen.DeleteObject();
	dnBarBrush.DeleteObject();
	dnBarPen.DeleteObject();
	stBarBrush.DeleteObject();
	stBarPen.DeleteObject();
	return true;

}
bool CSingleBarDraw::DrawTextHorizontalBar(CDC* pDC, const CRect& graphRegion, 
		const CString& strSubGraphName, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nStartIndex, const int nEndIndex, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CIndicatorInfo* pIndicatorInfo, const bool bLog, 
		const bool bInvert,const CPacket& pPacket)
{
	CList<double, double>* pDataList = NULL;
	int nDataCount = 0, nCurrentIndex = -1, nHighestIndex = -1;
	double dHorzViewMin = 0.0, dHorzViewMax = 0.0;
	if(!GetDrawingEnvironment_TextHorizontalBar(packetList, pDataList, nDataCount, 
		dHorzViewMin, dHorzViewMax, nCurrentIndex, nHighestIndex))
		return false;

	CRect drawingRegion = GetDrawingRegion(packetList.GetHead(), graphRegion, 
		strSubGraphName, nStartIndex, nEndIndex, dViewMin, dViewMax);
	int nBarWidth = 1;
	double dBarMaxWidth = 0.0;
	GetBarWidth(drawingRegion, nDataCount, dBarMaxWidth, nBarWidth);

	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	const CList<double, double>* pHogaDataList = NULL;
	pHogaDataList = pPacket.GetnumericDataList();

	if(pHogaDataList == NULL || pHogaDataList->GetCount() <= 0)
		return false;
	POSITION pos = pHogaDataList->GetHeadPosition();
	
	double	dMax,dMin,dHogaData;
	dMax = CDraw::CalLogValue(bLog,dViewMax);
	dMin = CDraw::CalLogValue(bLog,dViewMin);

	CPoint oldDataPt,dataPt;
	double dData,doldData;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	double dTotalData = GetTotalData(pDataList, nDataCount);
	//sy 2005.01.26. -> 대기매물의 text type.
	int nTextType = CBarDraw::GetTextType(pIndicatorInfo);

	// 그리기 위해 필요한 값들
	int nOrgMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF nOrgTextColor = pDC->GetTextColor();
	CPen* pOrgPen = pDC->GetCurrentPen();
	CBrush* pOrgBrush = pDC->GetCurrentBrush();
	CPen pen1, pen2, pen3;
	CBrush brush1, brush2, brush3;
	CreatePenAndBrush(graphColor, pen1, pen2, pen3, brush1, brush2, brush3);


CPoint old2DataPt,dataPt2;

	POSITION dataPos = pDataList->GetHeadPosition();
	for(int nIndex = 0; nIndex <= nDataCount; nIndex++)
	{
		if(dataPos == NULL)
			break;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
		if(pos)
			dHogaData = pHogaDataList->GetNext(pos);
		dData = pDataList->GetNext(dataPos);
//		CPoint dataPt1 = CDataConversion::GetHorizontalPoint(drawingRegion, nIndex, dBarMaxWidth, 
//			CDraw::CalLogValue(bLog, dHorzViewMin), CDraw::CalLogValue(bLog, dHorzViewMax), CDraw::CalLogValue(bLog, dData));
		dataPt.x = CDataConversion::GetHorizontalPoint_X(drawingRegion,	dHorzViewMin,dHorzViewMax,dData);
		// (2008/4/23 - Seung-Won, Bae) Move 3 pixel down to include Bottom Limmit.
		dataPt.y = CMath::Round((dMax - CDraw::CalLogValue(bLog, dHogaData))/(dMax - dMin) * graphRegion.Height() + graphRegion.top) + 3;

	
//2006.11.09		SelectPenAndBrushObject(pDC, &pen1, &pen2, &pen3, &brush1, &brush2, &brush3, 
//2006.11.09			graphColor, nIndex, nCurrentIndex, nHighestIndex);

		if(nIndex > 0)
		{
			//위치 조정을 위해서 -2 를 해줌(bottom = nBarWidth )
			//top에는 +1을 해준다.(DrawBar_TextHorizontalBar() 여기에서)
//2006.11.09
			SelectPenAndBrushObject(pDC, &pen1, &pen2, &pen3, &brush1, &brush2, &brush3, 
				graphColor, nIndex-1, nCurrentIndex, nHighestIndex);
//2006.11.09	
			
			nBarWidth = dataPt.y - oldDataPt.y - 2;
			// (2008/4/23 - Seung-Won, Bae) Set minimum width for disapearing.
			if( nBarWidth < 3) nBarWidth = 3;

			DrawBar_TextHorizontalBar(pDC, drawStyle, drawingRegion, nBarWidth, bInvert, oldDataPt);
			CString strData = GetDrawingText(nTextType, dTotalData, doldData);
			DrawText_TextHorizontalBar(pDC, drawingRegion, oldDataPt, strData,nBarWidth);
			oldDataPt = dataPt;
			doldData = dData;
		}
		else
		{
			oldDataPt = dataPt;
			doldData = dData;
			// (2008/4/23 - Seung-Won, Bae) Move 3 pixel down to include Bottom Limmit.
			oldDataPt.y = oldDataPt.y - 4;
		}
		// 수평바 그리는 부분.
//		DrawBar_TextHorizontalBar(pDC, drawStyle, drawingRegion, nBarWidth, bInvert, dataPt);
		// Text 그리는 부분.
		//sy 2005.01.26.
//		CString strData = GetDrawingText(nTextType, dTotalData, dData);
//		DrawText_TextHorizontalBar(pDC, drawingRegion, dataPt, strData);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	}

	pDC->SetBkMode(nOrgMode);
	pDC->SetTextColor(nOrgTextColor);
	pDC->SelectObject(pOrgBrush);
	pDC->SelectObject(pOrgPen);

	pen1.DeleteObject();
	pen2.DeleteObject();
	pen3.DeleteObject();
	brush1.DeleteObject();
	brush2.DeleteObject();
	brush3.DeleteObject();
	return true;
}
void CSingleBarDraw::DrawBar_TextHorizontalBar(CDC* pDC, const CGraphDrawStyle& drawStyle, 
		const CRect& graphRegion, const int nBarWidth, const bool bInvert, CPoint& dataPt)
{
// (2006.10.23 안승환) 대기매물 호가별 기능 추가	CRect barRect(graphRegion.left, (dataPt.y - nBarWidth), dataPt.x, (dataPt.y + nBarWidth));
	CRect barRect(graphRegion.left, dataPt.y + 1  , dataPt.x, (dataPt.y + nBarWidth));
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	if(bInvert)
	{
		InvertBar(graphRegion, barRect);
		dataPt = CPoint(barRect.right, barRect.CenterPoint().y);
	}

	// 수평바 그리는 부분
	// -> 바형
	if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::Bar_Draw)
	{
		if(barRect.Height() <= 1)
			CDraw::DrawLine(pDC, CPoint(barRect.left, barRect.top), CPoint(barRect.right, barRect.top));
		else
			CDraw::DrawRectangle(pDC, barRect);
	}
	// -> 도트형
	else if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::DotBar_Draw)
	{
		if(barRect.Height() / 2 < 1)
			DrawDotBar(pDC, CPoint(dataPt.x, dataPt.y), 1);
		else
			DrawDotBar(pDC, CPoint(dataPt.x, dataPt.y), nBarWidth);
	}
}
void CSingleBarDraw::DrawText_TextHorizontalBar(CDC* pDC, const CRect& graphRegion, const CPoint& dataPt, 
										  const CString& strData, short nWidth)
{
	if(strData.IsEmpty())
		return;

	CSize titleSize = pDC->GetTextExtent(strData);
	CPoint textPt;
	// 바의 오른쪽이 Text 끝 위치
	int nTextPt_x = dataPt.x - (titleSize.cx + MARGIN);
	if(nTextPt_x < graphRegion.left)
		nTextPt_x = graphRegion.left;
	// 바의 오른쪽이 Text 시작 위치
	/*int nTextPt_x = dataPt.x + MARGIN;
	if((nTextPt_x + titleSize.cx) > graphRegion.right)
		nTextPt_x = dataPt.x - (titleSize.cx + MARGIN);*/

	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
//	int nTextPt_y = dataPt.y - ((titleSize.cy / 2) - 1) ;
	int nTextPt_y = dataPt.y - ((titleSize.cy / 2) - 1) + nWidth/2 ;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	pDC->TextOut(nTextPt_x, nTextPt_y, strData);
}
bool CSingleBarDraw::GetDrawingEnvironment_TextHorizontalBar(const CList<CPacket*, CPacket*>& packetList, 
		CList<double, double>*& pDataList, int& nDataCount, 
		double& dHorzViewMin, double& dHorzViewMax, int& nCurrentIndex, int& nHighestIndex) const
{
	if(packetList.GetCount() <= 0)
		return false;

	CPacket* pPacket = packetList.GetHead();
	if(pPacket == NULL)
		return false;

	pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;

	nDataCount = pPacket->GetCount();
	nHighestIndex = nHighestIndex = -1;
	// "대기매물" 일 경우
	if( pPacket->GetName().Find( _MTEXT( C3_OVERHANGING_SUPPLY)) >= 0 && nDataCount > 2)
	{
		nDataCount -= 2;
		POSITION pos = pDataList->GetTailPosition();
		// 현재, 최대 index
		nHighestIndex = (int)pDataList->GetPrev(pos);
		nCurrentIndex = (int)pDataList->GetPrev(pos);
	}

	pPacket->GetMinMax(0, nDataCount, dHorzViewMin, dHorzViewMax);	
	dHorzViewMin = 0;

	return true;
}
bool CSingleBarDraw::DrawTextGroupBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	int count = 0;
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* pPacket1 = packetList.GetNext(pos);
	if(pos == NULL)
		return false;
	CPacket* pPacket2 = packetList.GetNext(pos);

	POSITION Data1Pos = pPacket1->GetnumericDataList()->FindIndex(nRealStart-1);
	POSITION Data2Pos = pPacket2->GetnumericDataList()->FindIndex(nRealStart-1);

	if(Data1Pos == NULL || Data2Pos == NULL) 
		return false;

	int realDrawStart = pPacket1->GetStartPeriod() + 1;
	int tempStart = pPacket2->GetStartPeriod() + 1;
	if(realDrawStart < tempStart)
		realDrawStart = tempStart;

	CBrush barBrush1(graphColor.GetColor2());
	CBrush barBrush2(graphColor.GetColor4());
	
	CPen barPen1, barPen2, bohapPen;

	int uplineThickness = CDraw::CalUpLineThickness(penThickness, nBarWidth);
	int downlineThickness = CDraw::CalDownLineThickness(penThickness, nBarWidth);
	int stationarylineThickness = CDraw::CalStationaryLineThickness(penThickness, nBarWidth);

	if(nBarWidth <= uplineThickness * 3 || nBarWidth <= downlineThickness * 3){
		barPen1.CreatePen(PS_SOLID,	1, graphColor.GetColor1());
		barPen2.CreatePen(PS_SOLID,	1, graphColor.GetColor3());
	}
	else{
		barPen1.CreatePen(PS_SOLID,	uplineThickness,	graphColor.GetColor1());
		barPen2.CreatePen(PS_SOLID,	downlineThickness,	graphColor.GetColor3());
	}
	bohapPen.CreatePen(PS_SOLID, stationarylineThickness, graphColor.GetColor6());
	
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&barBrush1);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&bohapPen);

	//==============================================================================
	bool bRect = false;
	if(nBarWidth > 4)
		bRect = true;
	CRect beforeRect1;
	CRect beforeRect2;

	if(nBarWidth <= 2)
		nBarWidth = 1;
	else{
		nBarWidth = nBarWidth / 2;
		if(nBarWidth >= 2)
			nBarWidth -= 1;
	}
	//============================================
	int checkDataStart = nRealStart - 1;
	CFont            m_fontVertCaption;
	CFont			 font;
 	if (font.CreateStockObject (DEFAULT_GUI_FONT))
    {
       	LOGFONT lf;
       	memset (&lf, 0, sizeof (LOGFONT));
       	if (font.GetLogFont (&lf) != 0)
       	{
       		lf.lfOrientation = 900;
       		lf.lfEscapement = 900;
       		lf.lfHeight = 12;
       		lf.lfWidth = 0;
       		lf.lfWeight = 400;
       		lf.lfItalic = TRUE;
       		_tcscpy (lf.lfFaceName, _T("Arial"));
       		 m_fontVertCaption.CreateFontIndirect (&lf);
       	}
   	}
	CFont* pOldFont = pDC->SelectObject (pDC->GetCurrentFont());
	pDC->SelectObject(m_fontVertCaption);
	int MODE = pDC->SetBkMode(TRANSPARENT);
	COLORREF clr = pDC->GetTextColor();
	
	double dPacketType1 = pPacket1->GetTypeFormatDouble();
	double dPacketType2 = pPacket2->GetTypeFormatDouble();
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(Data1Pos == NULL || Data2Pos == NULL)
			break;

		count++;
		checkDataStart++;
		double Data1 = pPacket1->GetnumericDataList()->GetNext(Data1Pos);
		CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, count, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, Data1));
		if(graphColor.GetUpGraphFill() == true)
			pDC->SelectObject(&barBrush1);
		else
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->SelectObject(&barPen1);

		//sy
		CRect rect1(dataPt.x - nBarWidth, dataPt.y, dataPt.x, graphRegion.bottom);

		if(count >1 && bRect == true){
			if(rect1.left <= beforeRect1.right)
				rect1.left = beforeRect1.right +1;
		}
		beforeRect1 = rect1;
		if(bInvert == true)			
			InvertBar(graphRegion, rect1);
		// 첫번재 바를 그려주는 부분 
		if(checkDataStart >= realDrawStart && Data1 != 0.0){
			// cj 2003.5.14 NonZero좌표점형 처리
			if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::DotBar_Draw ||
				drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::NonZero_DotBar_Draw){
				if(nBarWidth < 1)
					DrawDotBar(pDC, CPoint(dataPt.x, rect1.top), 1);
				else
					DrawDotBar(pDC, CPoint(dataPt.x, rect1.top), nBarWidth/2);
			}
			else{
				if(nBarWidth < 1)
					CDraw::DrawLine(pDC, CPoint(rect1.left, rect1.top), CPoint(rect1.left, rect1.bottom));
				else{
					if(rect1.top == rect1.bottom)
						CDraw::DrawLine(pDC, CPoint(rect1.left, rect1.bottom), CPoint(rect1.right, rect1.bottom));
					else
						CDraw::DrawRectangle(pDC, rect1);
				}
			}
				
			//text out
			CString sTitle = CDataConversion::GetStringThousandFormat(Data1, dPacketType1); // 출력할 스트링
			CRect rectLogo(rect1.left,graphRegion.top, rect1.right, rect1.top); // 출력영역
			CPoint ptOrg = CPoint(rectLogo.left, rectLogo.bottom - 2);
			pDC->ExtTextOut(ptOrg.x, ptOrg.y, NULL/*ETO_CLIPPED*/, rectLogo, sTitle, NULL);
		}

		double Data2 = pPacket2->GetnumericDataList()->GetNext(Data2Pos);
		dataPt = CDataConversion::GetVerticalPoint(graphRegion, count, dRealWidth, CDraw::CalLogValue(bLog, dViewMin), CDraw::CalLogValue(bLog, dViewMax), CDraw::CalLogValue(bLog, Data2));
		if(graphColor.GetDownGraphFill() == true)
			pDC->SelectObject(&barBrush2);
		else
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->SelectObject(&barPen2);

		CRect rect2(dataPt.x, dataPt.y, dataPt.x + nBarWidth, graphRegion.bottom);

		if(count > 1 && bRect == true){
			if(rect2.left <= beforeRect2.right)
				rect2.left = beforeRect2.right +1;
		}
		beforeRect2 = rect2;

		if(bInvert == true)			
			InvertBar(graphRegion, rect2);
		// 첫번재 바를 그려주는 부분 
		if(checkDataStart >= realDrawStart && Data2 != 0.0){
			// cj 2003.5.14 NonZero좌표점형 처리
			if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::DotBar_Draw ||
				drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::NonZero_DotBar_Draw){
				if(nBarWidth/2 < 1)
					DrawDotBar(pDC, CPoint(dataPt.x, rect2.top), 1);
				else
					DrawDotBar(pDC, CPoint(dataPt.x, rect2.top), nBarWidth/2);
			}
			else{
				if(nBarWidth < 1)
					CDraw::DrawLine(pDC, CPoint(rect2.left, rect2.top), CPoint(rect2.left, rect2.bottom));
				else{
					if(rect2.top == rect2.bottom)
						CDraw::DrawLine(pDC, CPoint(rect2.left, rect2.bottom), CPoint(rect2.right, rect2.bottom));
					else
						CDraw::DrawRectangle(pDC, rect2);
				}
			}
			//text out
			CString sTitle = CDataConversion::GetStringThousandFormat(Data2, dPacketType2); // 출력할 스트링
			CRect rectLogo(rect2.left,graphRegion.top, rect2.right, rect2.top); // 출력영역
			CPoint ptOrg = CPoint(rectLogo.left, rectLogo.bottom - 2);
 			pDC->ExtTextOut (ptOrg.x, ptOrg.y, NULL/*ETO_CLIPPED*/, rectLogo, sTitle, NULL);
		}
	}

	m_fontVertCaption.DeleteObject();
	font.DeleteObject();
	pDC->SetBkMode(MODE);
	pDC->SetTextColor(clr);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

	barBrush1.DeleteObject();
	barBrush2.DeleteObject();

	barPen1.DeleteObject();
	barPen2.DeleteObject();
	bohapPen.DeleteObject();
	return true;
}
void CSingleBarDraw::GetMinMax_FixedRateLeftRightBar(const double& dStandard, double& dMin, double& dMax)
{
	double dMaxGap = fabs(dMax - dStandard);
	double dMinGap = fabs(dStandard - dMin);
	if(dMaxGap >= dMinGap)
		dMin = dStandard - dMaxGap;	
	else
		dMax = dStandard + dMinGap;
}
void CSingleBarDraw::GetMinMax_LeftRightBar(const double& dStandard, double& dMin, double& dMax)
{
	if(dMin > dStandard)
		dMin = dStandard;
	else if(dMax < dStandard)
		dMax = dStandard;
}
void CSingleBarDraw::GetMinMaxAndStandard_LeftRightBar(CPacket* pPacket, const bool bIsFixedRate, double& dStandard, double& dMin, double& dMax)
{
	if(pPacket == NULL)
		return;

	dStandard = pPacket->GetStandardValue();
	if(dStandard < 0)
		dStandard = 0;

	pPacket->GetMinMax(0, pPacket->GetCount()-1, dMin, dMax);

	if(bIsFixedRate)
		GetMinMax_FixedRateLeftRightBar(dStandard, dMin, dMax);
	else
		GetMinMax_LeftRightBar(dStandard, dMin, dMax);

	GetMarginMinMax(dMin, dMax, dMin, dMax);
}
void CSingleBarDraw::GetMarginMinMax(const double dMin, const double dMax, double& dRealMin, double& dRealMax)
{
	CDisplayDataMinMax displayDataMinMax(dMin, dMax);
	dRealMin = displayDataMinMax.GetMarginDisplayMin();
	dRealMax = displayDataMinMax.GetMarginDisplayMax();
}

// (2009/1/9 - Seung-Won, Bae) for Vertical Bar Draw
void CSingleBarDraw::DrawVerticalBar_ReallocCache( CXScaleManager* pxScaleManager, CCalcDataForDraw *&pCalcData, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, int nDataCount, int nDTI_Start, int nDTI_End, int p_nColumn, const char *p_szRQ)
{
	if( pCalcData)	pCalcData->ResetValue();
	else			pCalcData = new CCalcDataForDraw;
	if( eBarDrawStyle == CGraphBaseData::Bar_Draw)
	{		
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		pCalcData->Alloc_lpPoint_High( nDataCount);
		pCalcData->Alloc_lpPoint_Low( nDataCount);
		pCalcData->Alloc_lpPoint_Bohap( nDataCount);
		pCalcData->Alloc_lpPolyCount_High( nDataCount/2);
		pCalcData->Alloc_lpPolyCount_Low( nDataCount/2);
		pCalcData->Alloc_lpPolyCount_Bohap( nDataCount/2);
		pCalcData->Alloc_lpdwcntPoly_High( nDataCount/2);
		pCalcData->Alloc_lpdwcntPoly_Low( nDataCount/2);
		pCalcData->Alloc_lpdwcntPoly_Bohap( nDataCount/2);
		pCalcData->nCount_High  = 0;
		pCalcData->nCount_Low   = 0;
		pCalcData->nCount_Bohap = 0;

		pCalcData->nFlag = pxScaleManager->IsAllOnePixel( p_nColumn, p_szRQ);
	}
	else
	{
		pCalcData->nDTI_Start	= nDTI_Start;
		pCalcData->nDTI_End	= nDTI_End;
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		pCalcData->Alloc_dCurOpen( nDataCount);
		pCalcData->Alloc_dCurClose( nDataCount);
		pCalcData->Alloc_dCurVolume( nDataCount);
		pCalcData->Alloc_rtGraph( nDataCount);
	}
}
BOOL CSingleBarDraw::DrawVerticalBar_AddBar( int nLastInsert, CCalcDataForDraw* pCalcData, int &nIndex_High, int &nIndex_Low, int &nIndex_Bohap, const CRect &rect)
{
	DrawVerticalBar_RemoveOnSamePixel(	pCalcData->Get_lpPolyCount_High(),
										pCalcData->nCount_High,
										pCalcData->Get_lpPoint_High(),
										nIndex_High,
										rect);
	DrawVerticalBar_RemoveOnSamePixel(	pCalcData->Get_lpPolyCount_Low(),
										pCalcData->nCount_Low,
										pCalcData->Get_lpPoint_Low(),
										nIndex_Low,
										rect);
	DrawVerticalBar_RemoveOnSamePixel(	pCalcData->Get_lpPolyCount_Bohap(),
										pCalcData->nCount_Bohap,
										pCalcData->Get_lpPoint_Bohap(),
										nIndex_Bohap,
										rect);

	// (2009/2/10 - Seung-Won, Bae) Check Overflow exception.
	if( pCalcData->m_nPoints - 4 <= nIndex_High
		|| pCalcData->m_nPoints - 4 <= nIndex_Low
		|| pCalcData->m_nPoints - 4 <= nIndex_Bohap) return FALSE;

	switch( nLastInsert)
	{
	case 0:	DrawVerticalBar_AddBarOnPixel( pCalcData->nFlag,
											pCalcData->Get_lpPolyCount_High(),
											pCalcData->Get_lpdwcntPoly_High(),
											pCalcData->nCount_High,
											pCalcData->Get_lpPoint_High(),
											nIndex_High,
											rect);
			break;
	case 1:	DrawVerticalBar_AddBarOnPixel( pCalcData->nFlag,
											pCalcData->Get_lpPolyCount_Low(),
											pCalcData->Get_lpdwcntPoly_Low(),
											pCalcData->nCount_Low,
											pCalcData->Get_lpPoint_Low(),
											nIndex_Low,
											rect);
			break;
	case 2:	DrawVerticalBar_AddBarOnPixel( pCalcData->nFlag,
											pCalcData->Get_lpPolyCount_Bohap(),
											pCalcData->Get_lpdwcntPoly_Bohap(),
											pCalcData->nCount_Bohap,
											pCalcData->Get_lpPoint_Bohap(),
											nIndex_Bohap,
											rect);
			break;
	}

	return TRUE;
}
void CSingleBarDraw::DrawVerticalBar_RemoveOnSamePixel(	LPINT lpPolyCount, int &nCount, LPPOINT lpPoint, int &nIndex, const CRect &rect)
{
	if( nIndex < 1 || nCount < 1) return;
	if( lpPoint[ nIndex - lpPolyCount[ nCount - 1]].x != rect.left) return;
	nCount--;
	nIndex -= lpPolyCount[ nCount];
}
void CSingleBarDraw::DrawVerticalBar_AddBarOnPixel( BOOL bLine, LPINT lpPolyCount, DWORD *lpdwcntPoly, int &nCount, LPPOINT lpPoint, int &nIndex, const CRect &rect)
{
	//>>20100521_JS.Kim 비정상 종료 수정
	if( nIndex < 0 || nCount < 0 )		return;
	//<<
	if( bLine)
	{
		lpPoint[nIndex].x = rect.left;
		lpPoint[nIndex].y = rect.bottom;
		nIndex++;

		lpPoint[nIndex].x = rect.left;
		lpPoint[nIndex].y = rect.top;
		nIndex++;

		// Do always.
		lpPolyCount[nCount] = 2;
		lpdwcntPoly[nCount] = 2;
	}
	else
	{
		lpPoint[nIndex].x = rect.left;
		lpPoint[nIndex].y = rect.bottom;
		nIndex++;

		lpPoint[nIndex].x = rect.left;
		lpPoint[nIndex].y = rect.top;
		nIndex++;

		lpPoint[nIndex].x = rect.right;
		lpPoint[nIndex].y = rect.top;
		nIndex++;
			
		lpPoint[nIndex].x = rect.right;
		lpPoint[nIndex].y = rect.bottom;
		nIndex++;

		lpPoint[nIndex].x = rect.left;
		lpPoint[nIndex].y = rect.bottom;
		nIndex++;

		// Do always.
		lpPolyCount[nCount] = 5;
		lpdwcntPoly[nCount] = 5;
	}
	nCount++; //Bar 갯수
}
void CSingleBarDraw::DrawVerticalBar_NotOverlapDataSetting( CCalcDataForDraw* pCalcData, int nLastInsert, int nDTI, bool &IsOverlap, int nIndex_High, int nIndex_Low,
										   int nIndex_Bohap, int nGraphWidth, int nTotalDTI)
{
	//XscaleMode == 1일 경우 마지막 픽셀이 시작되는 시점의 데이터 정보를 사용하는데
	//화면사이즈대비 데이터량이 1:1을 넘지 않을경우 (1픽셀당 1DTI경우 데이터가 겹치지 않으므로 별도의 처리를 한다.)
	if(!IsOverlap)  
	{
		if(nGraphWidth >= nTotalDTI)
		{
			pCalcData->nDTI_Start_LastPixel = nDTI - 1; //마지막 DTI
			
			pCalcData->nLastPolyCnt_high	= pCalcData->nCount_High;		
			pCalcData->nLastPolyCnt_Low		= pCalcData->nCount_Low;			 
			pCalcData->nLastPolyCnt_Bohap	= pCalcData->nCount_Bohap;
			//nLastInsert로 마지막으로 쌓인 데이터의 형태를 구분(0:상승 1:하락 2:보합)
			//마지막으로 쌓인 데이터의 인덱스값 보존한다.
			if(pCalcData->nCount_High > 0 && nLastInsert == 0)  
			{
				pCalcData->nLastPixel_high	    = nIndex_High  - *( pCalcData->Get_lpPolyCount_High() + pCalcData->nCount_High - 1);
				pCalcData->nLastPolyCnt_high	= pCalcData->nCount_High - 1;
			}
			if(pCalcData->nCount_Low > 0 && nLastInsert == 1)
			{
				pCalcData->nLastPixel_Low	    = nIndex_Low   - *( pCalcData->Get_lpPolyCount_Low() + pCalcData->nCount_Low - 1);
				pCalcData->nLastPolyCnt_Low		= pCalcData->nCount_Low - 1;
			}
			if(pCalcData->nCount_Bohap > 0 && nLastInsert == 2)
			{
				pCalcData->nLastPixel_Bohap     = nIndex_Bohap - *( pCalcData->Get_lpPolyCount_Bohap() + pCalcData->nCount_Bohap - 1);
				pCalcData->nLastPolyCnt_Bohap	= pCalcData->nCount_Bohap - 1;
			}
		}
		IsOverlap = false;
	}

}
void CSingleBarDraw::DrawVerticalBar_DrawPolygon( CCalcDataForDraw* pCalcData, CDC *pDC, const CGraphColor& graphColor, CBrush *brushArray, CPen *penArray)
{
	if( 0 < pCalcData->nCount_High)
	{	
		SelectGdiObject_HighLow(pDC, graphColor.GetUpGraphFill(), UP_BRUSH, UP_PEN, brushArray, penArray);
		DrawVerticalBar_DrawPolygon( pDC, pCalcData->nFlag, pCalcData->nCount_High, pCalcData->Get_lpPoint_High(), pCalcData->Get_lpPolyCount_High(), pCalcData->Get_lpdwcntPoly_High());
	}
	if( 0 < pCalcData->nCount_Low)
	{
		SelectGdiObject_HighLow(pDC, graphColor.GetDownGraphFill(), DOWN_BRUSH, DOWN_PEN, brushArray, penArray);
		DrawVerticalBar_DrawPolygon( pDC, pCalcData->nFlag, pCalcData->nCount_Low, pCalcData->Get_lpPoint_Low(), pCalcData->Get_lpPolyCount_Low(), pCalcData->Get_lpdwcntPoly_Low());
	}
	if( 0 < pCalcData->nCount_Bohap)
	{
		SelectGdiObject_HighLow(pDC, graphColor.GetDownGraphFill(), BOHAP_BRUSH, BOHAP_PEN, brushArray, penArray);
		DrawVerticalBar_DrawPolygon( pDC, pCalcData->nFlag, pCalcData->nCount_Bohap, pCalcData->Get_lpPoint_Bohap(), pCalcData->Get_lpPolyCount_Bohap(), pCalcData->Get_lpdwcntPoly_Bohap());
	}
}
void CSingleBarDraw::DrawVerticalBar_DrawPolygon( CDC *pDC, int nFlag, int nCount, LPPOINT lpPoint, LPINT lpPolyCount, DWORD *lpdwcntPoly)
{
	if( 1 < nCount)
	{
		if( nFlag)
				pDC->PolyPolyline( lpPoint, lpdwcntPoly, nCount);
		else	pDC->PolyPolygon( lpPoint, lpPolyCount, nCount);
	}
	else
	{
		if( nFlag)	pDC->Polyline( lpPoint, 2);
		else		pDC->Polygon( lpPoint, 4);
	}
}
