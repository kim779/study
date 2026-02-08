// BongDraw.cpp: implementation of the CBongDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BongDraw.h"

#include "../Include_Chart/DLL_Load/PacketList.h"				// for CPacketList
#include "../Include_Chart/Dll_Load/PacketListManager.h"		// for CPacketListManager
#include "DataMath.h"
#include "Conversion.h"

#include "Draw.h"
#include "BaseData.h"
#include "PacketPointerList.h"

#include "PacketBase.h"
#include "IndicatorInfo.h"
#include "IndicatorList.h"

#include "XScaleManager.h"
#include "GraphDrawer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBongDraw::CBongDraw()
{
// (2006.11.03 안승환) Flow차트 추가
	nPtCount = 0;
	pFlowPt = NULL;
// (2006.11.03 안승환) Flow차트 추가
	m_eGraphType = CGraphBaseData::Bong_Type;
}

CBongDraw::~CBongDraw()
{
	if( pFlowPt) delete pFlowPt;
}

//public ====================================================================================
bool CBongDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList<CPacket*, CPacket*>& packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);
	const CGraphColor& graphColor = subGraphData.GetColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();

	// (2006/12/6 - Seung-Won, Bae) Get BongType directly.
	int nBohapType = 3;
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	if( pIndicatorInfo) nBohapType = GetBohapType( pIndicatorInfo);
	m_graphDrawStyle = subGraphData.GetDrawStyle();

	// (2006/12/7 - Seung-Won, Bae) Change Bong Draw Style to Lin in 1 Tick Chart.
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
/*
	CPacketList *pPacketList = drawingGraphData.GetPacketList();
	if( CGraphBaseData::Line_Draw != m_graphDrawStyle.GetDrawStyle() && pPacketList)
		if( 1 == pPacketList->GetTimeInterval() && CScaleBaseData::HORZ_TICK == pPacketList->GetDateUnitType())
			m_graphDrawStyle.SetDrawStyle( CGraphBaseData::Line_Draw);
*/

	//그래프 타입에 따른 그리는 function을 불러온다.
	switch( m_graphDrawStyle.GetDrawStyle())
	{
	case CGraphBaseData::Candlestick_Draw:
	case CGraphBaseData::CandleVolume_Draw:
		return DrawCandlestickBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText());

// (2006.10.30 안승환) 바차트(고'저'종)추가
	//	case CGraphBaseData::HighLowClose_Draw:
	case CGraphBaseData::OpenHighLowClose_Draw:
		return DrawOpenHighLowCloseBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText());
// (2006.10.30 안승환) 바차트(고'저'종)추가
	case CGraphBaseData::NewHighLowClose_Draw:
		return DrawNewHighLowCloseBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText());
// (2006.10.30 안승환) 바차트(고'저'종)추가
	case CGraphBaseData::Line_Draw:
		return DrawSolidBong(pDC, realRegion, graphColor, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText(), drawingGraphData.GetWithBaseText(),
			drawingGraphData.GetDrawBaseLine());	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	case CGraphBaseData::Antenna_Draw:
		return DrawAntennaBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText());
// (2006.11.03 안승환) Flow차트 추가
	case CGraphBaseData::Flow_Draw:
		return DrawFlowBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText());
// (2006.11.03 안승환) Flow차트 추가
	}

	return false;
}

bool CBongDraw::DrawGraph(CDC* pDC,
		const CRect& realRegion, 
		const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData,
		const CSubGraphData& subGraphData,
		const CString& strRQ,
		CXScaleManager* pxScaleManager)
{
	const CList<CPacket*, CPacket*>& packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);
	const CGraphColor& graphColor = subGraphData.GetColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();

	// (2006/12/6 - Seung-Won, Bae) Get BongType directly.
	int nBohapType = 3;
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	if( pIndicatorInfo) nBohapType = GetBohapType( pIndicatorInfo);

	// (2006/12/7 - Seung-Won, Bae) Change Bong Draw Style to Lin in 1 Tick Chart.
	CGraphBaseData::BONGDRAWSTYLE eBongDrawStyle = subGraphData.GetDrawStyle().GetGraphBongDrawStyle();
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
/*
	CPacketList *pPacketList = drawingGraphData.GetPacketList();
	if( CGraphBaseData::Line_Draw != eBongDrawStyle && pPacketList)
		if( 1 == pPacketList->GetTimeInterval() && CScaleBaseData::HORZ_TICK == pPacketList->GetDateUnitType())
			eBongDrawStyle = CGraphBaseData::Line_Draw;
*/

	// KTB 체결잔고표시 - ojtaso (20091019)
	const CObArray& arrInfo = (CObArray&)subGraphData.GetHorizontalLine();

	//그래프 타입에 따른 그리는 function을 불러온다.
	switch( eBongDrawStyle)
	{
	case CGraphBaseData::Candlestick_Draw:
	case CGraphBaseData::CandleVolume_Draw:
		return DrawCandlestickBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText(), strRQ, pxScaleManager, drawingGraphData.GetColumnIndex(), arrInfo);
	case CGraphBaseData::Line_Draw:
		return DrawSolidBong(pDC, realRegion, graphColor, dRealWidth, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText(), drawingGraphData.GetWithBaseText(), drawingGraphData.GetDrawBaseLine(), // 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
			strRQ, pxScaleManager, const_cast<CSubGraphData&>(subGraphData), drawingGraphData.GetColumnIndex(), arrInfo); //2009.01.04 차트 성능 개선 그래프 계산결과 값 보존위해 SubGraphData인자 추가
	case CGraphBaseData::OpenHighLowClose_Draw:
		return DrawOpenHighLowCloseBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText(),strRQ, pxScaleManager, drawingGraphData.GetColumnIndex(), arrInfo);
	case CGraphBaseData::NewHighLowClose_Draw:
		return DrawNewHighLowCloseBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText(),strRQ, pxScaleManager, drawingGraphData.GetColumnIndex(), arrInfo);
	case CGraphBaseData::Antenna_Draw:
		return DrawAntennaBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText(),strRQ, pxScaleManager, drawingGraphData.GetColumnIndex(), arrInfo);
	case CGraphBaseData::Flow_Draw:
		return DrawFlowBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText(),strRQ, pxScaleManager, drawingGraphData.GetColumnIndex(), arrInfo);
	case CGraphBaseData::EquiVolume_Draw:
		return DrawEquiVolumeBong(pDC, realRegion, graphColor, dRealWidth, nRealStart, nRealEnd, 
			nBohapType, dViewMax, dViewMin, packetList, subGraphData.GetPenThickness(), 
			drawingGraphData.DoesVertScaleLogType(), drawingGraphData.DoesVertScaleInvertType(), 
			drawingGraphData.GetGraphRegions().IsText(), strRQ, pxScaleManager, drawingGraphData.GetColumnIndex(), arrInfo);
	}

	return false;
}

/*
	graphRegion	: 그래프 영역
	graphColor	: 그리는데 사용할 색정보
	dRealWidth	: 한개 아이템이 차지하는 Width
	nRealStart	: 그릴 데이터의 시작위치
	nRealEnd	: 그릴 데이터의 종료위치
	nBohapType	: 봉종류
	dViewMax	: 보기최대
	dViewMin	: 보기최소
	packetList	: 봉을 그리기 위한 packet목록
	penThickness : 얇은 팬
	bLog		: Log취할지
	bInvert		: 뒤집기
	bGraphTextDraw : 텍스트 그리기
*/
bool CBongDraw::DrawCandlestickBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
									const double dRealWidth, int nRealStart, int nRealEnd, const int nBohapType, 
									const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
									const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw)
{
	//data수에 따른 for문을 돌리는 루틴 (CandlestickBong Graph를 그린다)
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL)		return false;

	if(packetList.GetCount() < 4)	return false;
	CPacket* pOpenPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pHighPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pLowPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pClosePacket = packetList.GetNext(headPos);

	if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

	POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nRealStart);

	if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL)
		return false;

	CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
	CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList = pClosePacket->GetnumericDataList();
	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)
		return false;

	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

	double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
	pHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	pLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	bool bHighText = true, bLowText = true;

	///김찬중...Comparison 그리는 부분
	POSITION preClosePos = closePos;	pCloseList->GetPrev(preClosePos);	
	double dPreClose = 0.0;
	if(preClosePos != NULL) dPreClose = CDraw::CalLogValue(bLog, pCloseList->GetAt(preClosePos));

	double dHigh = 0.0, dOpen = 0.0, dClose = 0.0, dLow = 0.0;
	double dLogHigh = 0.0, dLogOpen = 0.0, dLogClose = 0.0, dLogLow = 0.0;

	double dViewBottom = (double)graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
	double dRealWidth_Half = dRealWidth / 2.0;
	double dRealHeight = dRealMax - dRealMin;
	double dViewY_High, dViewY_Open, dViewY_Close, dViewY_Low;

	int nViewX_Start, nViewX_Center;
	int nViewY_High, nViewY_Open, nViewY_Close, nViewY_Low;
	int nStartPeriod = pOpenPacket->GetStartPeriod();

	double dViewX_Center = graphRegion.left + dRealWidth_Half;

	int nOldMode = pDC->SetBkMode(TRANSPARENT);

	if(pHighList->GetCount() <= nRealEnd) nRealEnd = pHighList->GetCount() - 1;
	if(pOpenList->GetCount() <= nRealEnd) nRealEnd = pOpenList->GetCount() - 1;
	if(pCloseList->GetCount() <= nRealEnd) nRealEnd = pCloseList->GetCount() - 1;
	if(pLowList->GetCount() <= nRealEnd) nRealEnd = pLowList->GetCount() - 1;

	// 봉영역이 1 이상이면 영역이 2이 될수도 있기 때문에 완전한 봉을 그리도록 한다. 프로그램 하고 일부 1.0 ~ 1.5 까지는 아마도 빈번하게 선이 그려질것이다
	// 그렇다고 그 부분 성능향상을 하기 위해서 1.5이상에서 그려질경우까지 성능 저하를 하는것은 적절하지 않다. 하여 그 특정한 영역은 그냥 그래로 처리한다.
	if( 3.0 <= dRealWidth)
	{
		// (2006/12/29 - Seung-Won, Bae) Support Fixed Width. 
		// (2007/1/6 - Seung-Won, Bae) Support symmetry candle ( -2 is Gap Pixel and Center Line)
		int nHalfWith = int( dRealWidth - 2) / 2;
		if( nHalfWith < 1) nHalfWith = 1;

		for(int i = nRealStart; i <= nRealEnd; i++)
		{
			dLogHigh	= dHigh		= pHighList->GetNext(highPos);
			dLogOpen	= dOpen		= pOpenList->GetNext(openPos);
			dLogClose	= dClose	= pCloseList->GetNext(closePos);
			dLogLow		= dLow		= pLowList->GetNext(lowPos);

			// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( dHigh < dClose || dHigh < dOpen || dClose < dLow || dOpen < dLow)
				dLogHigh = dLogOpen = dLogClose = dLogLow = dHigh = dOpen = dLow = dClose;

			if(bLog)
			{
				dLogHigh	= CMath::Log(dHigh);
				dLogOpen	= CMath::Log(dOpen);
				dLogClose	= CMath::Log(dClose);
				dLogLow		= CMath::Log(dLow);
			}

			SelectGdiObject(pDC, nBohapType, dOpen, dClose, dPreClose, graphColor, brushArray, penArray);

			dPreClose = dClose;

			dViewY_High		= dViewBottom - (dViewHeight * ((dLogHigh	- dRealMin) / dRealHeight));
			dViewY_Open		= dViewBottom - (dViewHeight * ((dLogOpen	- dRealMin) / dRealHeight));
			dViewY_Close	= dViewBottom - (dViewHeight * ((dLogClose	- dRealMin) / dRealHeight));
			dViewY_Low		= dViewBottom - (dViewHeight * ((dLogLow	- dRealMin) / dRealHeight));

			// 그리기 위해서 최종적으로 double을 int로 변환한다.
			// 모든 값이 양수라고 보고 작업해도 상관이 없다.(메모리 DC를 사용할것이고, 그렇지 않더라도 음수인 영역은 보여지지 않는 영역이고
			// 보여지지 않는 영역을 포함한 차트라도 처음 한픽셀이 문제가 될수 있지만 그렇게 비중이 있는 량은 아니다.)
			// 그리고 당연히 메모리DC를 사용하기 때문에 고려해볼 가치도 없다.
			nViewX_Center	= ROUND( dViewX_Center);

			// 현재 이렇게 구현되어 있다.
			nViewX_Start	= nViewX_Center - nHalfWith;
			// nViewX_Center = ROUND(dViewY_Center);

			nViewY_High		= ROUND(dViewY_High);
			nViewY_Open		= ROUND(dViewY_Open);
			nViewY_Close	= ROUND(dViewY_Close);
			nViewY_Low		= ROUND(dViewY_Low);

			// (2007/1/23 - Seung-Won, Bae) Swap the open, close for High-Low Line.
			if( nViewY_Close < nViewY_Open)
			{
				int nClose = nViewY_Close;
				nViewY_Close = nViewY_Open;
				nViewY_Open = nClose;
			}

			// (2007/1/2 - Seung-Won, Bae) One line Steady Candle and Remove Low Line Gap.
			if(bInvert)
			{
				nViewY_High = nViewY_half2 - nViewY_High;
				nViewY_Open = nViewY_half2 - nViewY_Open;
				nViewY_Close = nViewY_half2 - nViewY_Close;
				nViewY_Low = nViewY_half2 - nViewY_Low;
			
				if(i >= nStartPeriod)
				{
					pDC->MoveTo(nViewX_Center, nViewY_High);
					pDC->LineTo(nViewX_Center, nViewY_Open - 1);
					pDC->MoveTo(nViewX_Center, nViewY_Close);
					pDC->LineTo(nViewX_Center, nViewY_Low - 1);

					pDC->Rectangle( nViewX_Center - nHalfWith, nViewY_Close, nViewX_Center + nHalfWith + 1, nViewY_Open + 1);
				}
			}
			else
			{
				if(i >= nStartPeriod)
				{
					pDC->MoveTo(nViewX_Center, nViewY_High);
					pDC->LineTo(nViewX_Center, nViewY_Open + 1);
					pDC->MoveTo(nViewX_Center, nViewY_Close);
					pDC->LineTo(nViewX_Center, nViewY_Low + 1);

					pDC->Rectangle( nViewX_Center - nHalfWith, nViewY_Open, nViewX_Center + nHalfWith + 1, nViewY_Close + 1);
				}
			}

			// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
			if( bGraphTextDraw && ( ( bHighText	&& dHigh == dHighMax) || ( bLowText	&& dLow == dLowMin)))
			{
				CString strTime;
				GetPriceMinMaxTime( strTime, pHighPacket, i);
				if( bHighText	&& dHigh	== dHighMax)
				{
					bHighText = false;
					DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, nViewX_Center, nViewY_High, bInvert);
				}
				if( bLowText	&& dLow		== dLowMin)
				{
					bLowText = false;
					DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, nViewX_Center, nViewY_Low, bInvert);
				}
			}

			dViewX_Center	+= dRealWidth;
		}
	}
	// 봉영역이 1픽셀 이하이면 굳이 사각형을 그릴필요도 없고 시가, 종가를 그릴필요가 없다.
	else
	{
		for(int i = nRealStart; i <= nRealEnd; i++)
		{
			dLogHigh	= dHigh		= pHighList->GetNext(highPos);
			dLogLow		= dLow		= pLowList->GetNext(lowPos);
						  dClose	= pCloseList->GetNext(closePos);

			// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( dHigh < dClose || dClose < dLow) dLogHigh = dLogLow = dHigh = dLow = dClose;

			if(bLog)
			{
				dLogHigh	= CMath::Log(dHigh);
				dLogLow		= CMath::Log(dLow);
			}

			dOpen	= pOpenList->GetNext(openPos);

			SelectGdiObject(pDC, nBohapType, dOpen, dClose, dPreClose, graphColor, brushArray, penArray);

			dPreClose = dClose;

			dViewY_High	= dViewBottom - (dViewHeight * ((dLogHigh	- dRealMin) / dRealHeight));
			dViewY_Low	= dViewBottom - (dViewHeight * ((dLogLow	- dRealMin) / dRealHeight));

			// 그리기 위해서 최종적으로 double을 int로 변환한다.
			// 모든 값이 양수라고 보고 작업해도 상관이 없다.(메모리 DC를 사용할것이고, 그렇지 않더라도 음수인 영역은 보여지지 않는 영역이고
			// 보여지지 않는 영역을 포함한 차트라도 처음 한픽셀이 문제가 될수 있지만 그렇게 비중이 있는 량은 아니다.)
			// 그리고 당연히 메모리DC를 사용하기 때문에 고려해볼 가치도 없다.

			nViewX_Center	= ROUND(dViewX_Center);
			nViewY_High	= ROUND(dViewY_High);
			nViewY_Low	= ROUND(dViewY_Low);

			if(bInvert)
			{
				nViewY_High = nViewY_half2 - nViewY_High;
				nViewY_Low = nViewY_half2 - nViewY_Low;

				if(i >= nStartPeriod)
				{
					pDC->MoveTo(nViewX_Center, nViewY_High);
					pDC->LineTo(nViewX_Center, nViewY_Low - 1);
				}
			}
			else
			{
				if(i >= nStartPeriod)
				{
					pDC->MoveTo(nViewX_Center, nViewY_High);
					pDC->LineTo(nViewX_Center, nViewY_Low + 1);
				}			
			}

			// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
			if( bGraphTextDraw && ( ( bHighText	&& dHigh == dHighMax) || ( bLowText	&& dLow == dLowMin)))
			{
				CString strTime;
				GetPriceMinMaxTime( strTime, pHighPacket, i);
				if( bHighText	&& dHigh	== dHighMax)
				{
					bHighText = false;
					DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, nViewX_Center, nViewY_High, bInvert);
				}
				if( bLowText	&& dLow		== dLowMin)
				{
					bLowText = false;
					DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, nViewX_Center, nViewY_Low, bInvert);
				}
			}

			dViewX_Center	+= dRealWidth;
		}
	}
	pDC->SetBkMode(nOldMode);
	DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);

	return true;
}

/*
	graphRegion	: 그래프 영역
	graphColor	: 그리는데 사용할 색정보
	dRealWidth	: 한개 아이템이 차지하는 Width
	nRealStart	: 그릴 데이터의 시작위치
	nRealEnd	: 그릴 데이터의 종료위치
	nBohapType	: 봉종류
	dViewMax	: 보기최대
	dViewMin	: 보기최소
	packetList	: 봉을 그리기 위한 packet목록
	penThickness : 얇은 팬
	bLog		: Log취할지
	bInvert		: 뒤집기
	bGraphTextDraw : 텍스트 그리기
*/
// KTB 체결잔고표시 - ojtaso (20091019)
bool CBongDraw::DrawCandlestickBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
									const double dRealWidth, int nRealStart, int nRealEnd, const int nBohapType, 
									const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
									const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
									const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
									const CObArray& arrInfo)
{
	if (pxScaleManager->GetThreadFlag() != THREAD_DRAWING)
		return true;

	//data수에 따른 for문을 돌리는 루틴 (CandlestickBong Graph를 그린다)
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL)		return false;

	if(packetList.GetCount() < 4)	return false;
	CPacket* pOpenPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pHighPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pLowPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pClosePacket = packetList.GetNext(headPos);

	if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

	int nDTI, nDTI_Start, nDTI_End;
	pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nDTI_Start);

	if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL)
		return false;

	CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
	CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList = pClosePacket->GetnumericDataList();
	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)
		return false;

	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

	double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
	pHighPacket->GetMinMax(nDTI_Start, nDTI_End, dTempMinMax, dHighMax, TRUE);
	pLowPacket->GetMinMax(nDTI_Start, nDTI_End, dLowMin, dTempMinMax, TRUE);

	dHighMax = CDraw::CalLogValue(bLog, dHighMax);
	dLowMin = CDraw::CalLogValue(bLog, dLowMin);
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	int nArrowLength = 11;
	bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;
	SIZE size;

	///김찬중...Comparison 그리는 부분
	POSITION preClosePos = closePos;	pCloseList->GetPrev(preClosePos);	
	double dPreClose = 0.0;
	if(preClosePos != NULL) dPreClose = CDraw::CalLogValue(bLog, pCloseList->GetAt(preClosePos));

	double dHigh = 0.0, dOpen = 0.0, dClose = 0.0, dLow = 0.0;

	double dViewBottom = (double)graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
	double dRealWidth_Half = dRealWidth / 2.0;
	double dRealHeight = dRealMax - dRealMin;

	double dViewY_High, dViewY_Open, dViewY_Close, dViewY_Low;
	int nViewY_High, nViewY_Open, nViewY_Close, nViewY_Low;

	int nStartPeriod = pOpenPacket->GetStartPeriod();

	int nOldMode = pDC->SetBkMode(TRANSPARENT);

//	CDC dcAlpha;
//	CBitmap bmpAlpha;
//	CBitmap* bmpAlpha_Back;
//	CFont* fontAlpha_Back;

//	dcAlpha.CreateCompatibleDC(pDC);
//	bmpAlpha.CreateCompatibleBitmap(pDC, pxScaleManager->GetGraphXArea_Right(), graphRegion.bottom + 50);
//	bmpAlpha_Back = dcAlpha.SelectObject(&bmpAlpha);
//	fontAlpha_Back = dcAlpha.SelectObject(pDC->GetCurrentFont());

//	dcAlpha.FillSolidRect(0, 0, pxScaleManager->GetGraphXArea_Right(), graphRegion.bottom + 50, RGB(255, 255, 255));

	if(pHighList->GetCount() <= nRealEnd) nRealEnd = pHighList->GetCount() - 1;
	if(pOpenList->GetCount() <= nRealEnd) nRealEnd = pOpenList->GetCount() - 1;
	if(pCloseList->GetCount() <= nRealEnd) nRealEnd = pCloseList->GetCount() - 1;
	if(pLowList->GetCount() <= nRealEnd) nRealEnd = pLowList->GetCount() - 1;

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

	CXScaleManager::PAREA parea;

	double dMinMaxRatioPrice = pxScaleManager->GetPriceMinMaxShowType() == 0 ? pClosePacket->GetnumericDataList()->GetTail() : pClosePacket->GetStandardValue();

	for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
	{
//		pxScaleManager->GetAt_DrawArea(nDTI, &parea);

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->NextDrawArea(&parea);
		parea = pblaAreafDTI->GetNext( dblCursor);

		if(parea == NULL) break;
		if(highPos == NULL) break;
		if(openPos == NULL) break;
		if(closePos == NULL) break;
		if(lowPos == NULL) break;
		
		dHigh = pHighList->GetNext(highPos);
		dOpen = pOpenList->GetNext(openPos);
		dClose = pCloseList->GetNext(closePos);
		dLow = pLowList->GetNext(lowPos);

		// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( dHigh < dClose || dHigh < dOpen || dClose < dLow || dOpen < dLow)
			dHigh = dOpen = dLow = dClose;

		if(bLog)
		{
			dHigh = CMath::Log(dHigh);
			dOpen = CMath::Log(dOpen);
			dClose = CMath::Log(dClose);
			dLow = CMath::Log(dLow);
		}

		SelectGdiObject(pDC, nBohapType, dOpen, dClose, dPreClose, graphColor, brushArray, penArray);

		dPreClose = dClose;

		if (dClose > dOpen)			// 상승
		{
			// 데이터 순서 정리
			double dTemp = dClose;
			dClose = dOpen;
			dOpen = dTemp;
		}

		// 2px 이상
		if(parea->right - parea->left > 1)
		{
			dViewY_High = dViewBottom - (dViewHeight * ((dHigh - dRealMin) / dRealHeight));
			dViewY_Open = dViewBottom - (dViewHeight * ((dOpen - dRealMin) / dRealHeight));
			dViewY_Close = dViewBottom - (dViewHeight * ((dClose - dRealMin) / dRealHeight));
			dViewY_Low = dViewBottom - (dViewHeight * ((dLow - dRealMin) / dRealHeight));

			nViewY_High		= ROUND(dViewY_High);
			nViewY_Open		= ROUND(dViewY_Open);
			nViewY_Close	= ROUND(dViewY_Close);
			nViewY_Low		= ROUND(dViewY_Low);

			if(bInvert)
			{
				nViewY_High = nViewY_half2 - nViewY_High;
				nViewY_Open = nViewY_half2 - nViewY_Open;
				nViewY_Close = nViewY_half2 - nViewY_Close;
				nViewY_Low = nViewY_half2 - nViewY_Low;
			
				if(nDTI >= nStartPeriod)
				{
					pDC->MoveTo(parea->center, nViewY_High);
					pDC->LineTo(parea->center, nViewY_Open - 1);
					pDC->MoveTo(parea->center, nViewY_Close);
					pDC->LineTo(parea->center, nViewY_Low - 1);

					pDC->Rectangle(parea->left, nViewY_Close, parea->right, nViewY_Open + 1);
				}
			
			} else
			{
				if(nDTI >= nStartPeriod)
				{
					pDC->MoveTo(parea->center, nViewY_High);
					pDC->LineTo(parea->center, nViewY_Open + 1);
					pDC->MoveTo(parea->center, nViewY_Close);
					pDC->LineTo(parea->center, nViewY_Low + 1);

					pDC->Rectangle(parea->left, nViewY_Open, parea->right, nViewY_Close + 1);
				}

			}
		}
		// 1px 이하
		else
		{
			dViewY_High = dViewBottom - (dViewHeight * ((dHigh - dRealMin) / dRealHeight));
			dViewY_Low = dViewBottom - (dViewHeight * ((dLow - dRealMin) / dRealHeight));

			nViewY_High		= ROUND(dViewY_High);
			nViewY_Low		= ROUND(dViewY_Low);

			if(bInvert)
			{
				nViewY_High = nViewY_half2 - nViewY_High;
				nViewY_Low = nViewY_half2 - nViewY_Low;
			
				if(nDTI >= nStartPeriod)
				{
					pDC->MoveTo(parea->center, nViewY_High);
					pDC->LineTo(parea->center, nViewY_Low - 1);
				}
			} else
			{
				if(nDTI >= nStartPeriod)
				{
					pDC->MoveTo(parea->center, nViewY_High);
					pDC->LineTo(parea->center, nViewY_Low + 1);
				}
			}
		}

		// 화살표를 그려주는 부분 ================================
		if(bGraphTextDraw){
			if(bHighText){
				if(dHigh == dHighMax){
					int nXCenter;

//					if(parea->left < graphRegion.left)
//					{
//						nXCenter = (graphRegion.left + parea->right) / 2;
//					}
//					else
					{
						nXCenter = parea->center;
					}

					bUpDown = true;
					bLeftRight = false;
					COLORREF textClr = pDC->SetTextColor(graphColor.GetColor1());
					CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
					if(!pHighList)	break;
					POSITION pos = pHighList->FindIndex(nDTI);
					if(!pos)	break;
					double dData = pHighList->GetNext(pos);
					CString str = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dData), pHighPacket->GetType());

					CPacket * pDatePacket = pHighPacket->GetDatePacket();
					if(pDatePacket){
						POSITION datePos = pDatePacket->GetnumericDataList()->FindIndex(nDTI);
						if(!datePos)	break;

						CList<double, double>* pDateList = pDatePacket->GetnumericDataList();
						if(!pDateList || datePos == NULL)	break;
						double date = pDateList->GetNext(datePos);
						CString datePacketType = pDatePacket->GetType();
						
						// 2011.02.08 by onlyojt >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
						// 추세선 공유 기능 수정 - onlyojt
						CDataConversion dataConversion( m_hOcxWnd, m_eChartMode);
						CString strRatio(_T(""));
						if (datePacketType == _T("YYYYMMDDHHMMSS"))
						{
							int nDataUnit = pDatePacket->GetDateUnitType();
							CString dateStr = dataConversion.GetStringFormatTrans2( CDataConversion::DoubleToString( date), nDataUnit);

							if(dMinMaxRatioPrice != -HUGE_VAL)
							{
								double dPercentData = (dData * 100.0) / dMinMaxRatioPrice;
								strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100., 2));
							}

							str = str + " (" + strRatio + dateStr + ")";
						}
						else
						// 2011.02.08 by onlyojt <<
						{
							if(pDatePacket->GetDateUnitType() == CScaleBaseData::HORZ_MONTHLY){
								if(CDataConversion::DoubleToString(date).GetLength() >= 8.0)
									date = date/100.0;
								datePacketType = datePacketType.Left(6);
							}
			
							CString dateStr = dataConversion.GetStringFormatTrans(CDataConversion::DoubleToString(date), datePacketType);
							if(pDatePacket->GetDateUnitType() == CScaleBaseData::HORZ_TICK)
							{
								CString strTemp;
								int nFind = dateStr.Find('/');
								if(nFind > -1)
								{
									strTemp = dateStr.Mid(nFind + 1);
									dateStr = strTemp;
								}
							}

							if(dMinMaxRatioPrice != -HUGE_VAL)
							{
								double dPercentData = (dData * 100.0) / dMinMaxRatioPrice;
								strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
							}
							
							str = str + " (" + strRatio + dateStr + ")";
						}
					}

					size = pDC->GetTextExtent(str);

					if(nXCenter + nArrowLength + 2 + size.cx >= graphRegion.right){
						bLeftRight = true;
						if(bInvert)
							pDC->TextOut(nXCenter - nArrowLength - 2 - size.cx, nViewY_High +5, str);
						else
							pDC->TextOut(nXCenter - nArrowLength - 2 - size.cx, nViewY_High - size.cy -3, str);
					}
					else{
						if(bInvert)
							pDC->TextOut(nXCenter + nArrowLength + 2, nViewY_High +5, str);
						else
							pDC->TextOut(nXCenter + nArrowLength + 2, nViewY_High - size.cy -3, str);
					}

					if(bInvert)
						bUpDown = false;

					//dcAlpha.SelectObject(&brushArray[UP_BRUSH]);
					//dcAlpha.SelectObject(&penArray[UP_PEN]);
					DrawArrow(pDC, CPoint(nXCenter, nViewY_High), bLeftRight, bUpDown, size, graphColor.GetColor1());

					bHighText = false;					

					pDC->SetTextColor(textClr);
				}
			}
			if(bLowText){
				if(dLow == dLowMin){
					int nXCenter;

//					if(parea->left < graphRegion.left)
//					{
//						nXCenter = (graphRegion.left + parea->right) / 2;
//					}
//					else
					{
						nXCenter = parea->center;
					}

					bUpDown = false;
					bLeftRight = true;
					COLORREF textClr = pDC->SetTextColor(graphColor.GetColor3());

					CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
					if(!pLowList)	break;
					POSITION Pos = pLowList->FindIndex(nDTI);
					if(!Pos)	break;
					double dData = pLowList->GetNext(Pos);
					CString str = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dData), pLowPacket->GetType());
					CPacket * pDatePacket = pLowPacket->GetDatePacket();
					if(pDatePacket){
						CList<double, double>* pDateList = pDatePacket->GetnumericDataList();
						POSITION datePos = pDateList->FindIndex(nDTI);
						if(!datePos || datePos == NULL)	break;
						double date = pDateList->GetNext(datePos);
						CString datePacketType = pDatePacket->GetType();

						// 2011.02.08 by onlyojt >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
						// 추세선 공유 기능 수정 - onlyojt
						CDataConversion dataConversion( m_hOcxWnd, m_eChartMode);
						CString strRatio(_T(""));
						if (datePacketType == _T("YYYYMMDDHHMMSS"))
						{
							int nDataUnit = pDatePacket->GetDateUnitType();
							CString dateStr = dataConversion.GetStringFormatTrans2( CDataConversion::DoubleToString( date), nDataUnit);
							if(dMinMaxRatioPrice != -HUGE_VAL)
							{
								double dPercentData = (dData * 100.0) / dMinMaxRatioPrice;
								strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
							}

							str = str + " (" + strRatio + dateStr + ")";
						}
						else
						// 2011.02.08 by onlyojt <<
						{
							if(pDatePacket->GetDateUnitType() == CScaleBaseData::HORZ_MONTHLY){
								if(CDataConversion::DoubleToString(date).GetLength() >= 8.0)
									date = date/100.0;
								datePacketType = datePacketType.Left(6);
							}								

							CString dateStr = dataConversion.GetStringFormatTrans(CDataConversion::DoubleToString(date), datePacketType);
							if(pDatePacket->GetDateUnitType() == CScaleBaseData::HORZ_TICK)
							{
								CString strTemp;
								int nFind = dateStr.Find('/');
								if(nFind > -1)
								{
									strTemp = dateStr.Mid(nFind + 1);
									dateStr = strTemp;
								}
							}

							if(dMinMaxRatioPrice != -HUGE_VAL)
							{
								double dPercentData = (dData * 100.0) / dMinMaxRatioPrice;
								strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
							}
							
							str = str + " (" + strRatio + dateStr + ")";
						}
					}

					size = pDC->GetTextExtent(str);

					if(nXCenter - nArrowLength - 2 - size.cx <= graphRegion.left){
						bLeftRight = false;
						if(bInvert)
							pDC->TextOut(nXCenter + nArrowLength + 2, nViewY_Low - size.cy -3, str);
						else
							pDC->TextOut(nXCenter + nArrowLength + 2, nViewY_Low +5, str);
					}
					else{
						if(bInvert)
							pDC->TextOut(nXCenter - nArrowLength - 2 - size.cx, nViewY_Low - size.cy -2, str);
						else
							pDC->TextOut(nXCenter - nArrowLength - 2 - size.cx, nViewY_Low +2, str);
					}

					if(bInvert)
						bUpDown = true;

					//dcAlpha.SelectObject(brushArray[DOWN_BRUSH]);
					//dcAlpha.SelectObject(penArray[DOWN_PEN]);
					DrawArrow(pDC, CPoint(nXCenter, nViewY_Low), bLeftRight, bUpDown, size, graphColor.GetColor3());

					bLowText = false;

					pDC->SetTextColor(textClr);
				}
			}
		}
	}

	// KTB 체결잔고표시 - ojtaso (20091019)
	int nCheJanInfo = arrInfo.GetSize();
	if(nCheJanInfo)
	{
		int nStart = pxScaleManager->GetGraphXArea_Left(p_nColumn);
		int nEnd = pxScaleManager->GetGraphXArea_Right(p_nColumn);

		CPen CheJanPen;
		for(int i = 0; i < nCheJanInfo; i++)
		{
			ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
			
			double dYAvgPrice = dViewBottom - (dViewHeight * ((pInfo->m_dPrice - dRealMin) / dRealHeight));

			CheJanPen.CreatePen(PS_SOLID, pInfo->m_nLine, pInfo->m_clrLine);
			pDC->SelectObject(&CheJanPen);

			pDC->MoveTo(nStart, dYAvgPrice);
			pDC->LineTo(nEnd, dYAvgPrice);
			
			size = pDC->GetTextExtent(pInfo->m_strTitle);
			pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);

			CheJanPen.DeleteObject();
		}
	}

//	BLENDFUNCTION bf;
//	bf.BlendOp = AC_SRC_OVER;
//	bf.BlendFlags = 0;
//	bf.AlphaFormat = 0;
//	bf.SourceConstantAlpha = pxScaleManager->GetScaleUnitAlphaWeight(strRQ);
//	AlphaBlend(pDC->GetSafeHdc(), 0, 0, pxScaleManager->GetGraphXArea_Right(), graphRegion.bottom + 50, dcAlpha.GetSafeHdc(), 0, 0, pxScaleManager->GetGraphXArea_Right(), graphRegion.bottom + 50, bf);

//	dcAlpha.SelectObject(fontAlpha_Back);
//	dcAlpha.SelectObject(bmpAlpha_Back);
//	dcAlpha.DeleteDC();
//	bmpAlpha.DeleteObject();

	pDC->SetBkMode(nOldMode);
	DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);

	return true;
}

/*
	graphRegion	: 그래프 영역
	graphColor	: 그리는데 사용할 색정보
	dRealWidth	: 한개 아이템이 차지하는 Width
	nRealStart	: 그릴 데이터의 시작위치
	nRealEnd	: 그릴 데이터의 종료위치
	nBohapType	: 봉종류
	dViewMax	: 보기최대
	dViewMin	: 보기최소
	packetList	: 봉을 그리기 위한 packet목록
	penThickness : 얇은 팬
	bLog		: Log취할지
	bInvert		: 뒤집기
	bGraphTextDraw : 텍스트 그리기
*/
// KTB 체결잔고표시 - ojtaso (20091019)
bool CBongDraw::DrawEquiVolumeBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
									const double dRealWidth, int nRealStart, int nRealEnd, const int nBohapType, 
									const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
									const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
									const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
									const CObArray& arrInfo)
{
	if (pxScaleManager->GetThreadFlag() != THREAD_DRAWING)
		return true;

	//data수에 따른 for문을 돌리는 루틴 (CandlestickBong Graph를 그린다)
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL)		return false;

	if(packetList.GetCount() < 4)	return false;
	CPacket* pOpenPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pHighPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pLowPacket = packetList.GetNext(headPos);
	if(headPos == NULL)
		return false;
	CPacket* pClosePacket = packetList.GetNext(headPos);

	if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

	int nDTI, nDTI_Start, nDTI_End;
	pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nDTI_Start);

	if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL)
		return false;

	CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
	CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList = pClosePacket->GetnumericDataList();
	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)
		return false;

	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

	double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
	pHighPacket->GetMinMax(nDTI_Start, nDTI_End, dTempMinMax, dHighMax);
	pLowPacket->GetMinMax(nDTI_Start, nDTI_End, dLowMin, dTempMinMax);

	dHighMax = CDraw::CalLogValue(bLog, dHighMax);
	dLowMin = CDraw::CalLogValue(bLog, dLowMin);
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	int nArrowLength = 11;
	bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;
	SIZE size;

	///김찬중...Comparison 그리는 부분
	POSITION preClosePos = closePos;	pCloseList->GetPrev(preClosePos);	
	double dPreClose = 0.0;
	if(preClosePos != NULL) dPreClose = CDraw::CalLogValue(bLog, pCloseList->GetAt(preClosePos));

	double dHigh = 0.0, dOpen = 0.0, dClose = 0.0, dLow = 0.0;

	double dViewBottom = (double)graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
	double dRealWidth_Half = dRealWidth / 2.0;
	double dRealHeight = dRealMax - dRealMin;

	double dViewY_High, dViewY_Open, dViewY_Close, dViewY_Low;
	int nViewY_High, nViewY_Open, nViewY_Close, nViewY_Low;

	int nStartPeriod = pOpenPacket->GetStartPeriod();

	int nOldMode = pDC->SetBkMode(TRANSPARENT);

//	CDC dcAlpha;
//	CBitmap bmpAlpha;
//	CBitmap* bmpAlpha_Back;
//	CFont* fontAlpha_Back;
//
//	dcAlpha.CreateCompatibleDC(pDC);
//	bmpAlpha.CreateCompatibleBitmap(pDC, pxScaleManager->GetGraphXArea_Right(), graphRegion.bottom + 50);
//	bmpAlpha_Back = dcAlpha.SelectObject(&bmpAlpha);
//	fontAlpha_Back = dcAlpha.SelectObject(pDC->GetCurrentFont());
//
//	dcAlpha.FillSolidRect(0, 0, pxScaleManager->GetGraphXArea_Right(), graphRegion.bottom + 50, RGB(255, 255, 255));

	if(pHighList->GetCount() <= nRealEnd) nRealEnd = pHighList->GetCount() - 1;
	if(pOpenList->GetCount() <= nRealEnd) nRealEnd = pOpenList->GetCount() - 1;
	if(pCloseList->GetCount() <= nRealEnd) nRealEnd = pCloseList->GetCount() - 1;
	if(pLowList->GetCount() <= nRealEnd) nRealEnd = pLowList->GetCount() - 1;

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

	CXScaleManager::PAREA parea;

	// KTB 체결잔고표시 - ojtaso (20091019)
	int nCheJanInfo = arrInfo.GetSize();
	if(nCheJanInfo)
	{
/*
		int nStart = 0;
		parea = pblaAreafDTI->GetAt(nDTI_Start);
		if(parea)
			nStart = parea->left;
		
		int nEnd = 0;
		parea = pblaAreafDTI->GetAt(nDTI_End);
		if(parea)
			nEnd = parea->right;

		for(int i = 0; i < nCheJanInfo; i++)
		{
			ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
			
			double dYAvgPrice = dViewBottom - (dViewHeight * ((pInfo->m_dPrice - dRealMin) / dRealHeight));

			pDC->MoveTo(nStart, dYAvgPrice);
			pDC->LineTo(nEnd, dYAvgPrice);
			
			size = pDC->GetTextExtent(pInfo->m_strTitle);
			pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);
		}
*/
		int nStart = pxScaleManager->GetGraphXArea_Left(p_nColumn);
		int nEnd = pxScaleManager->GetGraphXArea_Right(p_nColumn);

		CPen CheJanPen;
		for(int i = 0; i < nCheJanInfo; i++)
		{
			ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
			
			double dYAvgPrice = dViewBottom - (dViewHeight * ((pInfo->m_dPrice - dRealMin) / dRealHeight));

			CheJanPen.CreatePen(PS_SOLID, pInfo->m_nLine, pInfo->m_clrLine);
			pDC->SelectObject(&CheJanPen);

			pDC->MoveTo(nStart, dYAvgPrice);
			pDC->LineTo(nEnd, dYAvgPrice);
			
			size = pDC->GetTextExtent(pInfo->m_strTitle);
			pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);

			CheJanPen.DeleteObject();
		}
	}

	double dMinMaxRatioPrice = pxScaleManager->GetPriceMinMaxShowType() == 0 ? pClosePacket->GetnumericDataList()->GetTail() : pClosePacket->GetStandardValue();

	for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
	{
//		pxScaleManager->GetAt_DrawArea(nDTI, &parea);

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->NextDrawArea(&parea);
		parea = pblaAreafDTI->GetNext( dblCursor);

		if(parea == NULL) break;
		if(highPos == NULL) break;
		if(openPos == NULL) break;
		if(closePos == NULL) break;
		if(lowPos == NULL) break;
		
		dHigh = pHighList->GetNext(highPos);
		dOpen = pOpenList->GetNext(openPos);
		dClose = pCloseList->GetNext(closePos);
		dLow = pLowList->GetNext(lowPos);

		// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( dHigh < dClose || dHigh < dOpen || dClose < dLow || dOpen < dLow)
			dHigh = dOpen = dLow = dClose;

		if(bLog)
		{
			dHigh = CMath::Log(dHigh);
			dOpen = CMath::Log(dOpen);
			dClose = CMath::Log(dClose);
			dLow = CMath::Log(dLow);
		}

		SelectGdiObject(pDC, nBohapType, dOpen, dClose, dPreClose, graphColor, brushArray, penArray);

		dPreClose = dClose;

		if (dClose > dOpen)			// 상승
		{
			// 데이터 순서 정리
			double dTemp = dClose;
			dClose = dOpen;
			dOpen = dTemp;
		}

		// 2px 이상
		if(parea->right - parea->left > 1)
		{
			dViewY_High = dViewBottom - (dViewHeight * ((dHigh - dRealMin) / dRealHeight));
			dViewY_Open = dViewBottom - (dViewHeight * ((dOpen - dRealMin) / dRealHeight));
			dViewY_Close = dViewBottom - (dViewHeight * ((dClose - dRealMin) / dRealHeight));
			dViewY_Low = dViewBottom - (dViewHeight * ((dLow - dRealMin) / dRealHeight));

			nViewY_High		= ROUND(dViewY_High);
			nViewY_Open		= ROUND(dViewY_Open);
			nViewY_Close	= ROUND(dViewY_Close);
			nViewY_Low		= ROUND(dViewY_Low);

			if(bInvert)
			{
				nViewY_High = nViewY_half2 - nViewY_High;
				nViewY_Open = nViewY_half2 - nViewY_Open;
				nViewY_Close = nViewY_half2 - nViewY_Close;
				nViewY_Low = nViewY_half2 - nViewY_Low;
			
				if(nDTI >= nStartPeriod)
				{
//					dcAlpha.MoveTo(parea->center, nViewY_High);
//					dcAlpha.LineTo(parea->center, nViewY_Open - 1);
//					dcAlpha.MoveTo(parea->center, nViewY_Close);
//					dcAlpha.LineTo(parea->center, nViewY_Low - 1);

//					dcAlpha.Rectangle(parea->left, nViewY_Close, parea->right, nViewY_Open + 1);
					pDC->Rectangle(parea->left, nViewY_High, parea->right, nViewY_Low + 1);
				}
			
			} else
			{
				if(nDTI >= nStartPeriod)
				{
//					dcAlpha.MoveTo(parea->center, nViewY_High);
//					dcAlpha.LineTo(parea->center, nViewY_Open + 1);
//					dcAlpha.MoveTo(parea->center, nViewY_Close);
//					dcAlpha.LineTo(parea->center, nViewY_Low + 1);

//					dcAlpha.Rectangle(parea->left, nViewY_Open, parea->right, nViewY_Close + 1);
					pDC->Rectangle(parea->left, nViewY_Low, parea->right, nViewY_High + 1);
				}

			}
		}
		// 1px 이하
		else
		{
			dViewY_High = dViewBottom - (dViewHeight * ((dHigh - dRealMin) / dRealHeight));
			dViewY_Low = dViewBottom - (dViewHeight * ((dLow - dRealMin) / dRealHeight));

			nViewY_High		= ROUND(dViewY_High);
			nViewY_Low		= ROUND(dViewY_Low);

			if(bInvert)
			{
				nViewY_High = nViewY_half2 - nViewY_High;
				nViewY_Low = nViewY_half2 - nViewY_Low;
			
				if(nDTI >= nStartPeriod)
				{
					pDC->MoveTo(parea->center, nViewY_High);
					pDC->LineTo(parea->center, nViewY_Low - 1);
				}
			} else
			{
				if(nDTI >= nStartPeriod)
				{
					pDC->MoveTo(parea->center, nViewY_High);
					pDC->LineTo(parea->center, nViewY_Low + 1);
				}
			}
		}

		// 화살표를 그려주는 부분 ================================
		if(bGraphTextDraw){
			if(bHighText){
				if(dHigh == dHighMax){
					int nXCenter;

//					if(parea->left < graphRegion.left)
//					{
//						nXCenter = (graphRegion.left + parea->right) / 2;
//					}
//					else
					{
						nXCenter = parea->center;
					}

					bUpDown = true;
					bLeftRight = false;
					COLORREF textClr = pDC->SetTextColor(graphColor.GetColor1());
					CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
					if(!pHighList)	break;
					POSITION pos = pHighList->FindIndex(nDTI);
					if(!pos)	break;
					double dData = pHighList->GetNext(pos);
					CString str = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dData), pHighPacket->GetType());

					CPacket * pDatePacket = pHighPacket->GetDatePacket();
					if(pDatePacket){
						POSITION datePos = pDatePacket->GetnumericDataList()->FindIndex(nDTI);
						if(!datePos)	break;

						CList<double, double>* pDateList = pDatePacket->GetnumericDataList();
						if(!pDateList || datePos == NULL)	break;
						double date = pDateList->GetNext(datePos);
						CString datePacketType = pDatePacket->GetType();

						// 2011.02.08 by onlyojt >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
						// 추세선 공유 기능 수정 - onlyojt
						CDataConversion dataConversion( m_hOcxWnd, m_eChartMode);
						CString strRatio(_T(""));
						if (datePacketType == _T("YYYYMMDDHHMMSS"))
						{
							int nDataUnit = pDatePacket->GetDateUnitType();
							CString dateStr = dataConversion.GetStringFormatTrans2( CDataConversion::DoubleToString( date), nDataUnit);
							if(dMinMaxRatioPrice != -HUGE_VAL)
							{
								double dPercentData = (dData * 100.0) / dMinMaxRatioPrice;
								strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
							}
							
							str = str + " (" + strRatio + dateStr + ")";
						}
						else
						// 2011.02.08 by onlyojt <<
						{
							if(pDatePacket->GetDateUnitType() == CScaleBaseData::HORZ_MONTHLY){
								if(CDataConversion::DoubleToString(date).GetLength() >= 8.0)
									date = date/100.0;
								datePacketType = datePacketType.Left(6);
							}
							CString dateStr = dataConversion.GetStringFormatTrans(CDataConversion::DoubleToString(date), datePacketType);
							if(dMinMaxRatioPrice != -HUGE_VAL)
							{
								double dPercentData = (dData * 100.0) / dMinMaxRatioPrice;
								strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
							}
							
							str = str + " (" + strRatio + dateStr + ")";
						}
					}

					size = pDC->GetTextExtent(str);

					if(nXCenter + nArrowLength + 2 + size.cx >= graphRegion.right){
						bLeftRight = true;
						if(bInvert)
							pDC->TextOut(nXCenter - nArrowLength - 2 - size.cx, nViewY_High +5, str);
						else
							pDC->TextOut(nXCenter - nArrowLength - 2 - size.cx, nViewY_High - size.cy -3, str);
					}
					else{
						if(bInvert)
							pDC->TextOut(nXCenter + nArrowLength + 2, nViewY_High +5, str);
						else
							pDC->TextOut(nXCenter + nArrowLength + 2, nViewY_High - size.cy -3, str);
					}

					if(bInvert)
						bUpDown = false;

					//dcAlpha.SelectObject(&brushArray[UP_BRUSH]);
					//dcAlpha.SelectObject(&penArray[UP_PEN]);
					DrawArrow(pDC, CPoint(nXCenter, nViewY_High), bLeftRight, bUpDown, size, graphColor.GetColor1());

					bHighText = false;					

					pDC->SetTextColor(textClr);
				}
			}
			if(bLowText){
				if(dLow == dLowMin){
					int nXCenter;

//					if(parea->left < graphRegion.left)
//					{
//						nXCenter = (graphRegion.left + parea->right) / 2;
//					}
//					else
					{
						nXCenter = parea->center;
					}

					bUpDown = false;
					bLeftRight = true;
					COLORREF textClr = pDC->SetTextColor(graphColor.GetColor3());

					CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
					if(!pLowList)	break;
					POSITION Pos = pLowList->FindIndex(nDTI);
					if(!Pos)	break;
					double dData = pLowList->GetNext(Pos);
					CString str = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dData), pLowPacket->GetType());
					CPacket * pDatePacket = pLowPacket->GetDatePacket();
					if(pDatePacket){
						CList<double, double>* pDateList = pDatePacket->GetnumericDataList();
						POSITION datePos = pDateList->FindIndex(nDTI);
						if(!datePos || datePos == NULL)	break;
						double date = pDateList->GetNext(datePos);
						CString datePacketType = pDatePacket->GetType();

						// 2011.02.08 by onlyojt >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
						// 추세선 공유 기능 수정 - onlyojt
						CDataConversion dataConversion( m_hOcxWnd, m_eChartMode);
						CString strRatio(_T(""));
						if (datePacketType == _T("YYYYMMDDHHMMSS"))
						{
							int nDataUnit = pDatePacket->GetDateUnitType();
							CString dateStr = dataConversion.GetStringFormatTrans2( CDataConversion::DoubleToString( date), nDataUnit);
							if(dMinMaxRatioPrice != -HUGE_VAL)
							{
								double dPercentData = (dData * 100.0) / dMinMaxRatioPrice;
								strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
							}
							
							str = str + " (" + strRatio + dateStr + ")";
						}
						else
						// 2011.02.08 by onlyojt <<
						{
							if(pDatePacket->GetDateUnitType() == CScaleBaseData::HORZ_MONTHLY){
								if(CDataConversion::DoubleToString(date).GetLength() >= 8.0)
									date = date/100.0;
								datePacketType = datePacketType.Left(6);
							}
							CString dateStr = dataConversion.GetStringFormatTrans(CDataConversion::DoubleToString(date), datePacketType);
							if(dMinMaxRatioPrice != -HUGE_VAL)
							{
								double dPercentData = (dData * 100.0) / dMinMaxRatioPrice;
								strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
							}
							
							str = str + " (" + strRatio + dateStr + ")";
						}
					}

					size = pDC->GetTextExtent(str);

					if(nXCenter - nArrowLength - 2 - size.cx <= graphRegion.left){
						bLeftRight = false;
						if(bInvert)
							pDC->TextOut(nXCenter + nArrowLength + 2, nViewY_Low - size.cy -3, str);
						else
							pDC->TextOut(nXCenter + nArrowLength + 2, nViewY_Low +5, str);
					}
					else{
						if(bInvert)
							pDC->TextOut(nXCenter - nArrowLength - 2 - size.cx, nViewY_Low - size.cy -2, str);
						else
							pDC->TextOut(nXCenter - nArrowLength - 2 - size.cx, nViewY_Low +2, str);
					}

					if(bInvert)
						bUpDown = true;

					//dcAlpha.SelectObject(brushArray[DOWN_BRUSH]);
					//dcAlpha.SelectObject(penArray[DOWN_PEN]);
					DrawArrow(pDC, CPoint(nXCenter, nViewY_Low), bLeftRight, bUpDown, size, graphColor.GetColor3());

					bLowText = false;

					pDC->SetTextColor(textClr);
				}
			}
		}
	}

//	BLENDFUNCTION bf;
//	bf.BlendOp = AC_SRC_OVER;
//	bf.BlendFlags = 0;
//	bf.AlphaFormat = 0;
//	bf.SourceConstantAlpha = pxScaleManager->GetScaleUnitAlphaWeight(strRQ);
//	AlphaBlend(pDC->GetSafeHdc(), 0, 0, pxScaleManager->GetGraphXArea_Right(), graphRegion.bottom + 50, dcAlpha.GetSafeHdc(), 0, 0, pxScaleManager->GetGraphXArea_Right(), graphRegion.bottom + 50, bf);
//
//	dcAlpha.SelectObject(fontAlpha_Back);
//	dcAlpha.SelectObject(bmpAlpha_Back);
//	dcAlpha.DeleteDC();
//	bmpAlpha.DeleteObject();

	pDC->SetBkMode(nOldMode);
	DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);

	return true;
}

// (2006.10.30 안승환) 바차트(고'저'종)추가
//   오른쪽:종가   위:고가   아래:저가
bool CBongDraw::DrawNewHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw)
{
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL)		return false;
	if(packetList.GetCount() < 4)	return false;
	
	CPacket* pOpenPacket = packetList.GetNext(headPos);
	CPacket* pHighPacket = packetList.GetNext(headPos);
	CPacket* pLowPacket = packetList.GetNext(headPos);
	CPacket* pClosePacket = packetList.GetNext(headPos);

	if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

	POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nRealStart);

	if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL)
		return false;

	CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
	CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList = pClosePacket->GetnumericDataList();

	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)	return false;

	double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
	pHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	pLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);

	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();

	InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	int nArrowLength = 11;
	bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;
	COLORREF clr = pDC->GetTextColor();
	
	POSITION preOpenPos = openPos, preClosePos = closePos;
	pOpenList->GetPrev(preOpenPos);
	pCloseList->GetPrev(preClosePos);

	double dPreOpen = 0.0, dPreClose = 0.0;
	if(preOpenPos != NULL || preClosePos != NULL)	
	{
		dPreOpen = CDraw::CalLogValue(bLog, pOpenList->GetAt(preOpenPos));
		dPreClose = CDraw::CalLogValue(bLog, pCloseList->GetAt(preClosePos));
	}
		
	CPoint preClosePt(graphRegion.left, graphRegion.top);

	for(int i = nRealStart; i <= nRealEnd; i++)
	{
		if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL) break;
		double dOpen = pOpenList->GetNext(openPos);
		double dHigh = pHighList->GetNext(highPos);
		double dClose = pCloseList->GetNext(closePos);
		double dLow = pLowList->GetNext(lowPos);

		// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( dHigh < dClose || dHigh < dOpen || dClose < dLow || dOpen < dLow)
			dHigh = dOpen = dLow = dClose;
		
		const int nIndex = i - nRealStart;
		CPoint highPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dHigh), true);
		CPoint closePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex +1, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dClose), false);
		CPoint lowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dLow), true);

		if(bInvert)
		{
			closePt.y = CDataConversion::InvertGraphDataPt(graphRegion, closePt.y);
			highPt.y = CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
			lowPt.y = CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
		}

		SelectGdiObject(pDC, nBohapType, dOpen, dClose, dPreClose, graphColor, brushArray, penArray);
		
		//line 을 그린다.==========================
		CDraw::DrawLine(pDC, highPt, lowPt);
		//bong을 그린다 ==========================
		if(preClosePt.x +1 < closePt.x)
		{
			//종가
			CDraw::DrawLine(pDC, CPoint(highPt.x, closePt.y), closePt);
		}

		// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
		if( bGraphTextDraw && ( ( bHighText	&& dHigh == dHighMax) || ( bLowText	&& dLow == dLowMin)))
		{
			CString strTime;
			GetPriceMinMaxTime( strTime, pHighPacket, i);
			if( bHighText	&& dHigh	== dHighMax)
			{
				bHighText = false;
				DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, highPt.x, highPt.y, bInvert);
			}
			if( bLowText	&& dLow		== dLowMin)
			{
				bLowText = false;
				DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, lowPt.x, lowPt.y, bInvert);
			}
		}

		preClosePt = closePt;
		dPreOpen = dOpen;
		dPreClose = dClose;
	}

	pDC->SetTextColor(clr);
	DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);
	return true;
}
// (2006.10.30 안승환) 바차트(고'저'종)추가

// KTB 체결잔고표시 - ojtaso (20091019)
bool CBongDraw::DrawNewHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
			const CObArray& arrInfo)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		POSITION headPos = packetList.GetHeadPosition();
		if(headPos == NULL)		return false;
		if(packetList.GetCount() < 4)	return false;
		
		CPacket* pOpenPacket = packetList.GetNext(headPos);
		CPacket* pHighPacket = packetList.GetNext(headPos);
		CPacket* pLowPacket = packetList.GetNext(headPos);
		CPacket* pClosePacket = packetList.GetNext(headPos);

		if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nDTI_Start);

		if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL)
			return false;

		CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
		CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
		CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
		CList<double, double>* pCloseList = pClosePacket->GetnumericDataList();

		if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)	return false;

		double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
		pHighPacket->GetMinMax(nDTI_Start, nDTI_End, dTempMinMax, dHighMax);
		pLowPacket->GetMinMax(nDTI_Start, nDTI_End, dLowMin, dTempMinMax);
	//	pHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	//	pLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);

		CPen penArray[PEN_COUNT];
		CPen* pOldPen = pDC->GetCurrentPen();
		CBrush brushArray[BRUSH_COUNT];
		CBrush* pOldBrush = pDC->GetCurrentBrush();

		InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

		dHighMax = CDraw::CalLogValue(bLog, dHighMax);
		dLowMin = CDraw::CalLogValue(bLog, dLowMin);
		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dRealHeight = dRealMax - dRealMin;

		int nArrowLength = 11;
		bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;
		COLORREF clr = pDC->GetTextColor();
		
		POSITION preOpenPos = openPos, preClosePos = closePos;
		pOpenList->GetPrev(preOpenPos);
		pCloseList->GetPrev(preClosePos);

		double dPreOpen = 0.0, dPreClose = 0.0;
		if(preOpenPos != NULL || preClosePos != NULL)	
		{
			dPreOpen = CDraw::CalLogValue(bLog, pOpenList->GetAt(preOpenPos));
			dPreClose = CDraw::CalLogValue(bLog, pCloseList->GetAt(preClosePos));
		}
			
	//	CPoint preClosePt(graphRegion.left, graphRegion.top);
		CPoint preClosePt( pxScaleManager->GetGraphXArea_Start( p_nColumn), graphRegion.top);

		double dViewY_High, dViewY_Close, dViewY_Low;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		// KTB 체결잔고표시 - ojtaso (20091019)
		int nCheJanInfo = arrInfo.GetSize();
		if(nCheJanInfo)
		{
			int nStart = pxScaleManager->GetGraphXArea_Left(p_nColumn);
			int nEnd = pxScaleManager->GetGraphXArea_Right(p_nColumn);

			CPen CheJanPen;
			for(int i = 0; i < nCheJanInfo; i++)
			{
				ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
				
				double dYAvgPrice = dViewBottom - (dViewHeight * ((pInfo->m_dPrice - dRealMin) / dRealHeight));

				CheJanPen.CreatePen(PS_SOLID, pInfo->m_nLine, pInfo->m_clrLine);
				pDC->SelectObject(&CheJanPen);

				pDC->MoveTo(nStart, dYAvgPrice);
				pDC->LineTo(nEnd, dYAvgPrice);
				
				SIZE size = pDC->GetTextExtent(pInfo->m_strTitle);
				pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);

				CheJanPen.DeleteObject();
			}
		}

		CPoint highPt ,closePt,lowPt;

		double dMinMaxRatioPrice = pxScaleManager->GetPriceMinMaxShowType() == 0 ? pClosePacket->GetnumericDataList()->GetTail() : pClosePacket->GetStandardValue();
		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);
		
			if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL || parea == NULL) break;
			double dOpen = pOpenList->GetNext(openPos);
			double dHigh = pHighList->GetNext(highPos);
			double dClose = pCloseList->GetNext(closePos);
			double dLow = pLowList->GetNext(lowPos);

			// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( dHigh < dClose || dHigh < dOpen || dClose < dLow || dOpen < dLow)
				dHigh = dOpen = dLow = dClose;
			
			const int nIndex = nDTI - nDTI_Start;
	//		CPoint highPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dHigh), true);
	//		CPoint closePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex +1, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dClose), false);
	//		CPoint lowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dLow), true);
	//		if(bInvert)
	//		{
	//			closePt.y = CDataConversion::InvertGraphDataPt(graphRegion, closePt.y);
	//			highPt.y = CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
	//			lowPt.y = CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
	//		}
			dViewY_High  = ROUND(dViewBottom - ( dViewHeight * ((dHigh - dRealMin) / dRealHeight)));
			dViewY_Close = ROUND(dViewBottom - ( dViewHeight * ((dClose - dRealMin) / dRealHeight)));
			dViewY_Low   = ROUND(dViewBottom - ( dViewHeight * ((dLow - dRealMin) / dRealHeight)));
			if(bInvert)	{
				dViewY_High = nViewY_half2 - dViewY_High;
				dViewY_Close = nViewY_half2 - dViewY_Close;
				dViewY_Low = nViewY_half2 - dViewY_Low;
			}

			SelectGdiObject(pDC, nBohapType, dOpen, dClose, dPreClose, graphColor, brushArray, penArray);
			
			highPt.x = parea->center,highPt.y = dViewY_High;
			closePt.x = parea->right,closePt.y = dViewY_Close;
			lowPt.x = parea->center,lowPt.y = dViewY_Low;

			//line 을 그린다.==========================
			CDraw::DrawLine(pDC, highPt, lowPt);
			//bong을 그린다 ==========================
			if(preClosePt.x +1 < closePt.x)
			{
				//종가
				CDraw::DrawLine(pDC, CPoint(highPt.x, closePt.y), closePt);
			}

			// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
			if( bGraphTextDraw && ( ( bHighText	&& dHigh == dHighMax) || ( bLowText	&& dLow == dLowMin)))
			{
				CString strRatio(_T(""));

				CString strTime;
				GetPriceMinMaxTime( strTime, pHighPacket, nDTI);
				if( bHighText	&& dHigh	== dHighMax)
				{
					if(dMinMaxRatioPrice != -HUGE_VAL)
					{
						double dPercentData = (dHigh * 100.0) / dMinMaxRatioPrice;
						strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
						strTime.Insert(strTime.Find("(") + 1, strRatio);
					}

					bHighText = false;
					DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, highPt.x, highPt.y, bInvert);
				}
				if( bLowText	&& dLow		== dLowMin)
				{
					if(dMinMaxRatioPrice != -HUGE_VAL)
					{
						double dPercentData = (dHigh * 100.0) / dMinMaxRatioPrice;
						strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
						strTime.Insert(strTime.Find("(") + 1, strRatio);
					}

					bLowText = false;
					DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, lowPt.x, lowPt.y, bInvert);
				}
			}

	//		preClosePt = closePt;
			dPreOpen = dOpen;
			dPreClose = dClose;
		}

		pDC->SetTextColor(clr);
		DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);
	}
// 	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	return true;
}

//왼쪽:시가   오른쪽:종가   위:고가   아래:저가
bool CBongDraw::DrawOpenHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw)
{
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL)		return false;
	if(packetList.GetCount() < 4)	return false;
	
	CPacket* pOpenPacket = packetList.GetNext(headPos);
	CPacket* pHighPacket = packetList.GetNext(headPos);
	CPacket* pLowPacket = packetList.GetNext(headPos);
	CPacket* pClosePacket = packetList.GetNext(headPos);

	if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

	POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nRealStart);

	if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL)
		return false;

	CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
	CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
	CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList = pClosePacket->GetnumericDataList();
	if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)	return false;

	double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
	pHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	pLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);

	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();

	InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	int nArrowLength = 11;
	bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;
	COLORREF clr = pDC->GetTextColor();
	
	//if(pClosePacket->IsUseStandardValue())
	//	DrawComparisonLine(pDC, &graphRegion, dRealWidth, dRealMin, dRealMax, dViewMin, dViewMax, pClosePacket);

	POSITION preOpenPos = openPos, preClosePos = closePos;
	pOpenList->GetPrev(preOpenPos);
	pCloseList->GetPrev(preClosePos);
	double dPreOpen = 0.0, dPreClose = 0.0;
	if(preOpenPos != NULL || preClosePos != NULL){
		dPreOpen = CDraw::CalLogValue(bLog, pOpenList->GetAt(preOpenPos));
		dPreClose = CDraw::CalLogValue(bLog, pCloseList->GetAt(preClosePos));
	}
		
	CPoint preClosePt(graphRegion.left, graphRegion.top);

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL) break;
		double dHigh = pHighList->GetNext(highPos);
		double dOpen = pOpenList->GetNext(openPos);
		double dClose = pCloseList->GetNext(closePos);
		double dLow = pLowList->GetNext(lowPos);

		// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( dHigh < dClose || dHigh < dOpen || dClose < dLow || dOpen < dLow)
			dHigh = dOpen = dLow = dClose;
		
		const int nIndex = i - nRealStart;
		CPoint highPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dHigh), true);
		CPoint openPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dOpen), false);
		CPoint closePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex +1, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dClose), false);
		CPoint lowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dLow), true);

		if(bInvert){
			openPt.y = CDataConversion::InvertGraphDataPt(graphRegion, openPt.y);
			closePt.y = CDataConversion::InvertGraphDataPt(graphRegion, closePt.y);
			highPt.y = CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
			lowPt.y = CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
		}

		//==========================================================
		SelectGdiObject(pDC, nBohapType, dOpen, dClose, dPreClose, graphColor, brushArray, penArray);
		
		//line 을 그린다.==========================
		CDraw::DrawLine(pDC, highPt, lowPt);
		//bong을 그린다 ==========================
		if(i >= pOpenPacket->GetStartPeriod()){
			if(preClosePt.x +1 < closePt.x){
				//시가
				CDraw::DrawLine(pDC, CPoint(preClosePt.x+1, openPt.y), CPoint(highPt.x +1, openPt.y));	
				//종가
				CDraw::DrawLine(pDC, CPoint(highPt.x, closePt.y), closePt);
			}
		}

		// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
		if( bGraphTextDraw && ( ( bHighText	&& dHigh == dHighMax) || ( bLowText	&& dLow == dLowMin)))
		{
			CString strTime;
			GetPriceMinMaxTime( strTime, pHighPacket, i);
			if( bHighText	&& dHigh	== dHighMax)
			{
				bHighText = false;
				DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, highPt.x, highPt.y, bInvert);
			}
			if( bLowText	&& dLow		== dLowMin)
			{
				bLowText = false;
				DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, lowPt.x, lowPt.y, bInvert);
			}
		}

		preClosePt = closePt;
		dPreOpen = dOpen;
		dPreClose = dClose;
	}

	pDC->SetTextColor(clr);
	DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);
	return true;
}
// KTB 체결잔고표시 - ojtaso (20091019)
bool CBongDraw::DrawOpenHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
			const CObArray& arrInfo)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		POSITION headPos = packetList.GetHeadPosition();
		if(headPos == NULL)		return false;
		if(packetList.GetCount() < 4)	return false;
		
		CPacket* pOpenPacket = packetList.GetNext(headPos);
		CPacket* pHighPacket = packetList.GetNext(headPos);
		CPacket* pLowPacket = packetList.GetNext(headPos);
		CPacket* pClosePacket = packetList.GetNext(headPos);

		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

		POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nDTI_Start);

		if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL)
			return false;

		CList<double, double>* pOpenList = pOpenPacket->GetnumericDataList();
		CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
		CList<double, double>* pLowList = pLowPacket->GetnumericDataList();
		CList<double, double>* pCloseList = pClosePacket->GetnumericDataList();
		if(pOpenList == NULL || pHighList == NULL || pLowList == NULL || pCloseList == NULL)	return false;

		double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
	//	pHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	//	pLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);
		pHighPacket->GetMinMax(nDTI_Start, nDTI_End, dTempMinMax, dHighMax);
		pLowPacket->GetMinMax(nDTI_Start, nDTI_End, dLowMin, dTempMinMax);

		CPen penArray[PEN_COUNT];
		CPen* pOldPen = pDC->GetCurrentPen();
		CBrush brushArray[BRUSH_COUNT];
		CBrush* pOldBrush = pDC->GetCurrentBrush();

		InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, false);

		dHighMax = CDraw::CalLogValue(bLog, dHighMax);
		dLowMin = CDraw::CalLogValue(bLog, dLowMin);
		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dRealHeight = dRealMax - dRealMin;

		int nArrowLength = 11;
		bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;
		COLORREF clr = pDC->GetTextColor();
		
		//if(pClosePacket->IsUseStandardValue())
		//	DrawComparisonLine(pDC, &graphRegion, dRealWidth, dRealMin, dRealMax, dViewMin, dViewMax, pClosePacket);

		POSITION preOpenPos = openPos, preClosePos = closePos;
		pOpenList->GetPrev(preOpenPos);
		pCloseList->GetPrev(preClosePos);
		double dPreOpen = 0.0, dPreClose = 0.0;
		if(preOpenPos != NULL || preClosePos != NULL)
		{
			dPreOpen = CDraw::CalLogValue(bLog, pOpenList->GetAt(preOpenPos));
			dPreClose = CDraw::CalLogValue(bLog, pCloseList->GetAt(preClosePos));
		}
			
		double dViewY_High, dViewY_Open, dViewY_Close, dViewY_Low;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		// KTB 체결잔고표시 - ojtaso (20091019)
		int nCheJanInfo = arrInfo.GetSize();
		if(nCheJanInfo)
		{
			int nStart = pxScaleManager->GetGraphXArea_Left(p_nColumn);
			int nEnd = pxScaleManager->GetGraphXArea_Right(p_nColumn);

			CPen CheJanPen;
			for(int i = 0; i < nCheJanInfo; i++)
			{
				ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
				
				double dYAvgPrice = dViewBottom - (dViewHeight * ((pInfo->m_dPrice - dRealMin) / dRealHeight));

				CheJanPen.CreatePen(PS_SOLID, pInfo->m_nLine, pInfo->m_clrLine);
				pDC->SelectObject(&CheJanPen);

				pDC->MoveTo(nStart, dYAvgPrice);
				pDC->LineTo(nEnd, dYAvgPrice);
				
				SIZE size = pDC->GetTextExtent(pInfo->m_strTitle);
				pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);

				CheJanPen.DeleteObject();
			}
		}

		CPoint highPt ,openPt,closePt,lowPt;

	//	CPoint preClosePt1(graphRegion.left, graphRegion.top);
		CPoint preClosePt( pxScaleManager->GetGraphXArea_Start( p_nColumn), graphRegion.top);

	//	for(int i = nRealStart; i <= nRealEnd; i++)
		double dMinMaxRatioPrice = pxScaleManager->GetPriceMinMaxShowType() == 0 ? pClosePacket->GetnumericDataList()->GetTail() : pClosePacket->GetStandardValue();

		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);

			if(highPos == NULL || openPos == NULL || closePos == NULL || lowPos == NULL ||parea == NULL ) break;
			double dHigh = pHighList->GetNext(highPos);
			double dOpen = pOpenList->GetNext(openPos);
			double dClose = pCloseList->GetNext(closePos);
			double dLow = pLowList->GetNext(lowPos);

			// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( dHigh < dClose || dHigh < dOpen || dClose < dLow || dOpen < dLow)
				dHigh = dOpen = dLow = dClose;
			
			const int nIndex = nDTI - nDTI_Start;

			dViewY_High  = ROUND(dViewBottom - ( dViewHeight * ((dHigh - dRealMin) / dRealHeight)));
			dViewY_Open  = ROUND(dViewBottom - ( dViewHeight * ((dOpen - dRealMin) / dRealHeight)));
			dViewY_Close = ROUND(dViewBottom - ( dViewHeight * ((dClose - dRealMin) / dRealHeight)));
			dViewY_Low   = ROUND(dViewBottom - ( dViewHeight * ((dLow - dRealMin) / dRealHeight)));
			if(bInvert)	{
				dViewY_High = nViewY_half2 - dViewY_High;
				dViewY_Open = nViewY_half2 - dViewY_Open;
				dViewY_Close = nViewY_half2 - dViewY_Close;
				dViewY_Low = nViewY_half2 - dViewY_Low;
			}
	//		CPoint highPt1 = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dHigh), true);
	//		CPoint openPt1 = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dOpen), false);
	//		CPoint closePt1 = CDataConversion::GetVerticalPoint(graphRegion, nIndex +1, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dClose), false);
	//		CPoint lowPt1 = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dLow), true);
	//		if(bInvert){
	//			openPt.y = CDataConversion::InvertGraphDataPt(graphRegion, openPt.y);
	//			closePt.y = CDataConversion::InvertGraphDataPt(graphRegion, closePt.y);
	//			highPt.y = CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
	//			lowPt.y = CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
	//		}
			
			//==========================================================
			SelectGdiObject(pDC, nBohapType, dOpen, dClose, dPreClose, graphColor, brushArray, penArray);
			
			highPt.x = parea->center,highPt.y = dViewY_High;
			openPt.x = parea->left,openPt.y = dViewY_Open;
			closePt.x = parea->right,closePt.y = dViewY_Close;
			lowPt.x = parea->center,lowPt.y = dViewY_Low;
			
			//line 을 그린다.==========================
			CDraw::DrawLine(pDC, highPt, lowPt);
			//bong을 그린다 ==========================
			if(nDTI >= pOpenPacket->GetStartPeriod())
			{
	//			if(preClosePt.x +1 < closePt.x)
				{
					//시가
					CDraw::DrawLine(pDC, CPoint(openPt.x, openPt.y), CPoint(highPt.x, openPt.y));	
					//종가
					CDraw::DrawLine(pDC, CPoint(highPt.x, closePt.y), closePt);
				}
			}

			// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
			if( bGraphTextDraw && ( ( bHighText	&& dHigh == dHighMax) || ( bLowText	&& dLow == dLowMin)))
			{
				CString strRatio(_T(""));

				CString strTime;
				GetPriceMinMaxTime( strTime, pHighPacket, nDTI);
				if( bHighText	&& dHigh	== dHighMax)
				{
					if(dMinMaxRatioPrice != -HUGE_VAL)
					{
						double dPercentData = (dHigh * 100.0) / dMinMaxRatioPrice;
						strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
						strTime.Insert(strTime.Find("(") + 1, strRatio);
					}

					bHighText = false;
					DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, highPt.x, highPt.y, bInvert);
				}
				if( bLowText	&& dLow		== dLowMin)
				{
					if(dMinMaxRatioPrice != -HUGE_VAL)
					{
						double dPercentData = (dLow * 100.0) / dMinMaxRatioPrice;
						strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
						strTime.Insert(strTime.Find("(") + 1, strRatio);
					}

					bLowText = false;
					DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, lowPt.x, lowPt.y, bInvert);
				}
			}

	//		preClosePt = closePt;
			dPreOpen = dOpen;
			dPreClose = dClose;
		}

		pDC->SetTextColor(clr);
		DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);
	}
// 	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	return true;
}
bool CBongDraw::DrawSolidBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw, BOOL p_bWithBaseText,
			bool bDrawBaseLine)	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
{
	CPacket * pPacket = NULL;
	POSITION Pos = packetList.GetHeadPosition();
	if(Pos == NULL)		return false;
	//packet이 4개 이상 일때는 4번째 packet으로 그리고 그렇지 않을때는 첫번째 packet으로 그린다.
	if(packetList.GetCount() >= 4){
		POSITION packetPos = packetList.FindIndex(3);
		if(packetPos)
			pPacket = packetList.GetNext(packetPos);
	}
	else
		pPacket = packetList.GetNext(Pos);
	if(!pPacket)	return false;

	CList<double, double>* pCloseList = pPacket->GetnumericDataList();
	if(pCloseList == NULL)	return false;

	POSITION closePos = pCloseList->FindIndex(nRealStart);
	if(closePos == NULL)	return false;

	CBrush brushArray[BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, true);

	//============================================================================================
	double dHighMax = 0.0, dLowMin = 0.0;
	pPacket->GetMinMax( nRealStart, nRealEnd, dLowMin, dHighMax);
	
	CPoint prePt(0, 0), curPt(0, 0);
	double dCurClose = 0.0;
	
	int nArrowLength = 11;
	bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;

	///로그값으로 바꿔준다
	double dLoggedViewMin = CDraw::CalLogValue(bLog, dViewMin);
	double dLoggedViewMax = CDraw::CalLogValue(bLog, dViewMax);

	// (2007/1/14 - Seung-Won, Bae) Draw BaseLine and Texts
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	int nBase = CDraw::DrawBaseLineAndTexts( pDC, graphRegion, graphColor, pPacket, dViewMax, dViewMin, dLoggedViewMax, dLoggedViewMin, bLog, bInvert, p_bWithBaseText, bDrawBaseLine);

	COLORREF clr = pDC->GetTextColor();
/// 이 타입에서는 화면에서 보여지는 시작 점보다 이전점의 역할이 없다
	dCurClose = pCloseList->GetAt(closePos);
	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dLoggedViewMin, dLoggedViewMax, CDraw::CalLogValue(bLog, dCurClose), true);
	if(bInvert == true)
		prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

	int nStartPeriod = pPacket->GetStartPeriod();
	for(int i = nRealStart; i <= nRealEnd; i++)
	{
		if(closePos == NULL)	break;
		dCurClose = pCloseList->GetNext( closePos);
		const int nIndex = i - nRealStart;
		curPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dLoggedViewMin, dLoggedViewMax, CDraw::CalLogValue(bLog, dCurClose), true);

		//line 을 그린다.==========================
		if(bInvert == true)
			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

		if(i >= nStartPeriod)
		{
			if(nRealStart == nRealEnd)
				Draw_SolidBong(pDC, curPt, nBase, dRealWidth, penArray);
			else if(i >= nStartPeriod)
				Draw_SolidBong(pDC, prePt, curPt, nBase, penArray);

			prePt = curPt;
		}
		
		// 선차트에 최고/최저가 표시 - ojtaso (20071115)
// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
// (2007/1/20 - Seung-Won, Bae) Do not Show in Line Chart.
/*
		if( bGraphTextDraw && ( ( bHighText	&& dCurClose == dHighMax) || ( bLowText	&& dCurClose == dLowMin)))
		{
			CString strTime;
			GetPriceMinMaxTime( strTime, pPacket, i);
			if( bHighText	&& dCurClose	== dHighMax)
			{
				bHighText = false;
				DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion::GetStringFormatTrans( CDataConversion::DoubleToString( dCurClose),	pPacket->GetType()) + strTime, curPt.x, curPt.y, bInvert);
			}
			if( bLowText	&& dCurClose	== dLowMin)
			{
				bLowText = false;
				DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion::GetStringFormatTrans( CDataConversion::DoubleToString( dCurClose),	pPacket->GetType())	+ strTime, curPt.x, curPt.y, bInvert);
			}
		}
*/

		prePt = curPt;
	}

	pDC->SetTextColor(clr);
	DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);
	return true;
}

// KTB 체결잔고표시 - ojtaso (20091019)
bool CBongDraw::DrawSolidBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw, BOOL p_bWithBaseText, bool bDrawBaseLine, // 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
			const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn,
			const CObArray& arrInfo)
{
	//2009.01.04 차트 성능 개선. 김성조
	//구분자가 THREAD_CALCULATION일경우는 계산 후 결과값만 산출.

// 	class CGraphBong
// 	{
// 	public:
// 		int		nIdxPoly_High, nIdxPoly_Low;
// 		POINT* lpptLine_High;
// 		DWORD* lpdwcntPoly_High;
// 		POINT* lpptLine_Low; 
// 		DWORD* lpdwcntPoly_Low; 
// 	};//봉그래프 데이터 저장 클래스


	CPacket * pPacket = NULL;
	POSITION Pos = packetList.GetHeadPosition();
	if(Pos == NULL)		return false;
	//packet이 4개 이상 일때는 4번째 packet으로 그리고 그렇지 않을때는 첫번째 packet으로 그린다.
	if(packetList.GetCount() >= 4){
		POSITION packetPos = packetList.FindIndex(3);
		if(packetPos)
			pPacket = packetList.GetNext(packetPos);
	}
	else
			pPacket = packetList.GetNext(Pos);
	if( !pPacket)	return false;

	if(pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		// 1. Get the cache size.
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		//		Alloc max memory.
		int nDataCount = pxScaleManager->GetColumnWidth( p_nColumn) * 4;
		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( nDataCount < 1) return true;

		// 2. Check X Scale Mode.
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
		if(xScaleMode != 1)
		{
			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			if( subGraphData.m_pGraphCache) subGraphData.m_pGraphCache->ResetValue();
			else							subGraphData.m_pGraphCache = new CCalcDataForDraw;
			subGraphData.m_pGraphCache->Alloc_lpPoint_High( nDataCount);
			subGraphData.m_pGraphCache->Alloc_lpdwcntPoly_High( nDataCount);
			subGraphData.m_pGraphCache->Alloc_lpPoint_Low( nDataCount);
			subGraphData.m_pGraphCache->Alloc_lpdwcntPoly_Low( nDataCount);

TRACE( "\r\n2009.02.27 Memory is created with [%d]\r\n", nDataCount);
		}
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		//YValue Set
		pCalcData->SetYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);

		// 3. Check start/end Data Index.
		int nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		// 4. for each Data.
		// 4.1 for AREA
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);
		CXScaleManager::PAREA parea;
		// 4.2 for Data value
		CList<double, double>* pCloseList = pPacket->GetnumericDataList();
		if(pCloseList == NULL)	return false;
		POSITION closePos = pCloseList->FindIndex(nDTI_Start);
		if( closePos == NULL)	return false;
		// 4.3 for Env Values
		double dViewBottom = graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dRealMin = CDraw::CalLogValue(bLog, dViewMin); 
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
		double dRealHeight = dRealMax - dRealMin;
		// 4.4 for Prev Info
		CPoint prePt( 0, 0);
		parea = pblaAreafDTI->GetAt( dblCursor);
		if( parea == NULL) return false;
		double dClose = CDraw::CalLogValue( bLog, pCloseList->GetAt( closePos));
		dClose = dViewBottom - ( dViewHeight * ( ( dClose - dRealMin) / dRealHeight));
		dClose	= ROUND( dClose);
		if( bInvert) dClose = nViewY_half2 - dClose;
		prePt.x = parea->center;
		prePt.y = dClose;
		// 4.5 for current PolyLine.
		int nIdxPoint_High = -4;
		int nIdxPoint_Low = -4;
		if( xScaleMode != 1)
		{
			pCalcData->nFlag = CDraw::DrawBaseLineAndTexts( graphRegion, pPacket, dViewMax, dViewMin, dRealMax, dRealMin, bLog, bInvert);
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			CPacketListManager *pPacketListManager = pxScaleManager->GetPacketListManager();
			if( pPacketListManager)
			{
				const char *szItemCode = NULL, *szItemName = NULL;
				pPacketListManager->GetItemCode( strRQ, szItemCode, szItemName);
				if( szItemCode)
					if( *szItemCode == '|') pCalcData->nFlag = INT_MAX;
			}
		}
		int *	pnCount_High		= &pCalcData->nCount_High;
		LPPOINT	lpPoint_High		= pCalcData->Get_lpPoint_High();
		DWORD *	lpdwcntPoly_High	= pCalcData->Get_lpdwcntPoly_High();
		int *	pnIdxPoint_High		= &nIdxPoint_High;
		int *	pnCount_Low			= &pCalcData->nCount_Low;
		LPPOINT	lpPoint_Low			= pCalcData->Get_lpPoint_Low();
		DWORD *	lpdwcntPoly_Low		= pCalcData->Get_lpdwcntPoly_Low();
		int *	pnIdxPoint_Low		= &nIdxPoint_Low;
		if( bInvert)
		{
			pnCount_High		= &pCalcData->nCount_Low;
			lpPoint_High		= pCalcData->Get_lpPoint_Low();
			lpdwcntPoly_High	= pCalcData->Get_lpdwcntPoly_Low();
			pnIdxPoint_High		= &nIdxPoint_Low;
			pnCount_Low			= &pCalcData->nCount_High;
			lpPoint_Low			= pCalcData->Get_lpPoint_High();
			lpdwcntPoly_Low		= pCalcData->Get_lpdwcntPoly_High();
			pnIdxPoint_Low		= &nIdxPoint_High;
		}
		int *	pnCount		= pnCount_High;
		LPPOINT	lpPoint		= lpPoint_High;
		DWORD *	lpdwcntPoly	= lpdwcntPoly_High;
		int *	pnIdxPoint	= pnIdxPoint_High;
		if( pCalcData->nFlag < prePt.y)
		{
			pnCount		= pnCount_Low;
			lpPoint		= lpPoint_Low;
			lpdwcntPoly	= lpdwcntPoly_Low;
			pnIdxPoint	= pnIdxPoint_Low;
		}

		// for X Scale Mode 1
		if( xScaleMode == 1)
		{
			nDTI_Start		= pCalcData->nDTI_Start;
			prePt			= pCalcData->m_Prept;
			nIdxPoint_High	= pCalcData->nLastPixel_high;
			nIdxPoint_Low	= pCalcData->nLastPixel_Low;
			pCalcData->nCount_High	= pCalcData->nCount_High2;
			pCalcData->nCount_Low	= pCalcData->nCount_Low2;
			if( 0 < pCalcData->nCount_High) *( pCalcData->Get_lpdwcntPoly_High() +  pCalcData->nCount_High - 1) = pCalcData->nLastPolyCnt_high;
			if( 0 < pCalcData->nCount_Low)	*( pCalcData->Get_lpdwcntPoly_Low() + pCalcData->nCount_Low - 1) = pCalcData->nLastPolyCnt_Low;

			if( pCalcData->m_pTemp == lpPoint_High)
			{
				pnCount		= pnCount_High;
				lpPoint		= lpPoint_High;
				lpdwcntPoly	= lpdwcntPoly_High;
				pnIdxPoint	= pnIdxPoint_High;
			}
			else
			{
				pnCount		= pnCount_Low;
				lpPoint		= lpPoint_Low;
				lpdwcntPoly	= lpdwcntPoly_Low;
				pnIdxPoint	= pnIdxPoint_Low;
			}

			pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);
			closePos = pCloseList->FindIndex( nDTI_Start);
			if( closePos == NULL) return false;
		}

		// KTB 체결잔고표시 - ojtaso (20091019)
		int nCheJanInfo = arrInfo.GetSize();
		if(nCheJanInfo)
		{
			int nStart = pxScaleManager->GetGraphXArea_Left(p_nColumn);
			int nEnd = pxScaleManager->GetGraphXArea_Right(p_nColumn);

			CPen CheJanPen;
			for(int i = 0; i < nCheJanInfo; i++)
			{
				ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
				
				double dYAvgPrice = dViewBottom - (dViewHeight * ((pInfo->m_dPrice - dRealMin) / dRealHeight));

				CheJanPen.CreatePen(PS_SOLID, pInfo->m_nLine, pInfo->m_clrLine);
				pDC->SelectObject(&CheJanPen);

				pDC->MoveTo(nStart, dYAvgPrice);
				pDC->LineTo(nEnd, dYAvgPrice);
				
				SIZE size = pDC->GetTextExtent(pInfo->m_strTitle);
				pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);

				CheJanPen.DeleteObject();
			}
		}

		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);
		CPoint curPt( 0, 0);
		for( int i = nDTI_Start; i <= nDTI_End; i++)
		{
			parea = pblaAreafDTI->GetNext( dblCursor);
			if( closePos == NULL || parea == NULL) break;

			dClose = CDraw::CalLogValue(bLog, pCloseList->GetNext( closePos));
			dClose = dViewBottom - ( dViewHeight * ( ( dClose - dRealMin) / dRealHeight));
			dClose = ROUND( dClose);
			if( bInvert) dClose = nViewY_half2 - dClose;

			curPt.x = parea->center;
			curPt.y = dClose;

			// (2009/2/27 - Seung-Won, Bae) Backup for XScaleMode 1
			if( curPt.x != prePt.x)
			{
				pCalcData->nDTI_Start			= i;
				pCalcData->m_Prept				= prePt;
				pCalcData->m_pTemp				= lpPoint;
				pCalcData->nLastPixel_high		= nIdxPoint_High;
				pCalcData->nLastPixel_Low		= nIdxPoint_Low;
				pCalcData->nCount_High2			= pCalcData->nCount_High;
				pCalcData->nCount_Low2			= pCalcData->nCount_Low;
				if( 0 < pCalcData->nCount_High2) pCalcData->nLastPolyCnt_high	= *( pCalcData->Get_lpdwcntPoly_High() + pCalcData->nCount_High2 - 1);
				if( 0 < pCalcData->nCount_Low2)	pCalcData->nLastPolyCnt_Low		= *( pCalcData->Get_lpdwcntPoly_Low() + pCalcData->nCount_Low2 - 1);
			}

			// 1. Check change polyline.
			if( !( pCalcData->nFlag < curPt.y && lpPoint == lpPoint_High) && !( curPt.y < pCalcData->nFlag && lpPoint == lpPoint_Low))
			{
				if( 0 == ( *pnCount))
				{
					DrawSolidBong_Append( curPt.x, curPt.y, pnCount, lpdwcntPoly, pnIdxPoint, lpPoint);

					// for only one data.
					lpPoint[ ( *pnIdxPoint)].x = parea->left;
					lpPoint[ ( *pnIdxPoint) + 3].x = parea->right;
				}
				else  DrawSolidBong_Append( curPt.x, curPt.y, pnCount, lpdwcntPoly, pnIdxPoint, lpPoint);
			}
			else
			{
				int X = ROUND( double( pCalcData->nFlag - prePt.y) / double( curPt.y - prePt.y) * double( curPt.x - prePt.x)) + prePt.x;
				DrawSolidBong_Append( X, pCalcData->nFlag, pnCount, lpdwcntPoly, pnIdxPoint, lpPoint);

				if( pCalcData->nFlag < curPt.y && lpPoint == lpPoint_High)
				{
					pnCount		= pnCount_Low;
					lpPoint		= lpPoint_Low;
					lpdwcntPoly	= lpdwcntPoly_Low;
					pnIdxPoint	= pnIdxPoint_Low;
				}
				else if( curPt.y < pCalcData->nFlag && lpPoint == lpPoint_Low)
				{
					pnCount		= pnCount_High;
					lpPoint		= lpPoint_High;
					lpdwcntPoly	= lpdwcntPoly_High;
					pnIdxPoint	= pnIdxPoint_High;
				}

				DrawSolidBong_Append( X, pCalcData->nFlag, pnCount, lpdwcntPoly, pnIdxPoint, lpPoint, TRUE);
				DrawSolidBong_Append( curPt.x, curPt.y, pnCount, lpdwcntPoly, pnIdxPoint, lpPoint);
			}

			prePt = curPt;
		}
ASSERT( nIdxPoint_High + 4 <= pCalcData->m_nPoints && nIdxPoint_Low + 4 <= pCalcData->m_nPoints);
TRACE( "\r\n2009.02.27 Memory is filled with [%d,%d]\r\n", nIdxPoint_High + 4, nIdxPoint_Low + 4);
	}
	//2009.01.04 차트 성능 개선. THREAD_DRAWING 넘어온 결과 값으로 그래프 그린다.
	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{	
		if( !pPacket)	return false;
		if( !subGraphData.m_pGraphCache) return true;

		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( pCalcData->nCount_High < 1 && pCalcData->nCount_Low < 1) return true;

		CBrush brushArray[BRUSH_COUNT];
		CBrush* pOldBrush = pDC->GetCurrentBrush();
		CPen penArray[PEN_COUNT];
		CPen* pOldPen = pDC->GetCurrentPen();
		InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, true);

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin); 
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		// KTB 체결잔고표시 - ojtaso (20091019)
		int nCheJanInfo = arrInfo.GetSize();
		if(nCheJanInfo)
		{
			int nStart = pxScaleManager->GetGraphXArea_Left(p_nColumn);
			int nEnd = pxScaleManager->GetGraphXArea_Right(p_nColumn);

			CPen CheJanPen;
			for(int i = 0; i < nCheJanInfo; i++)
			{
				ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
				
				double dYAvgPrice = graphRegion.bottom - (graphRegion.Height() * ((pInfo->m_dPrice - dRealMin) / graphRegion.Height()));

				CheJanPen.CreatePen(PS_SOLID, pInfo->m_nLine, pInfo->m_clrLine);
				pDC->SelectObject(&CheJanPen);

				pDC->MoveTo(nStart, dYAvgPrice);
				pDC->LineTo(nEnd, dYAvgPrice);
				
				SIZE size = pDC->GetTextExtent(pInfo->m_strTitle);
				pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);

				CheJanPen.DeleteObject();
			}
		}

		// (2010/1/26 - Seung-Won, Bae) for Base Line.
		if( pCalcData->nFlag != INT_MAX)
			CDraw::DrawBaseLineAndTexts( pDC, graphRegion, graphColor, pPacket, dViewMax, dViewMin, dRealMax, dRealMin,
				bLog, bInvert, p_bWithBaseText, bDrawBaseLine);	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)

		COLORREF clr = pDC->GetTextColor();

		if( pCalcData->nCount_High > 0)
		{
			pDC->SelectObject( penArray[UP_PEN]);
			if( 1 < pCalcData->nCount_High)
					pDC->PolyPolyline( pCalcData->Get_lpPoint_High(), pCalcData->Get_lpdwcntPoly_High(), pCalcData->nCount_High);
			else	pDC->Polyline( pCalcData->Get_lpPoint_High(), *pCalcData->Get_lpdwcntPoly_High());
		}
		if( pCalcData->nCount_Low > 0)
		{
			pDC->SelectObject( penArray[DOWN_PEN]);
			if( 1 < pCalcData->nCount_Low)
					pDC->PolyPolyline( pCalcData->Get_lpPoint_Low(), pCalcData->Get_lpdwcntPoly_Low(), pCalcData->nCount_Low);
			else	pDC->Polyline( pCalcData->Get_lpPoint_Low(), *pCalcData->Get_lpdwcntPoly_Low());
		}
		pDC->SetTextColor( clr);
		DestroyGdiObject( pDC, brushArray, penArray, pOldBrush, pOldPen);
	}	

	return true;
}

void CBongDraw::DrawSolidBong_Append( int curX, int curY, int *pnCount, DWORD *lpdwcntPoly, int *pnIdxPoint, LPPOINT lpPoint, BOOL p_bWithNew)
{
	if( 0 == ( *pnCount))
	{
		( *pnCount)++;
		lpdwcntPoly[ ( *pnCount) - 1] = 4;
		( *pnIdxPoint) += 4;
		lpPoint[ ( *pnIdxPoint)] = lpPoint[ ( *pnIdxPoint) + 1] = lpPoint[ ( *pnIdxPoint) + 2] = lpPoint[ ( *pnIdxPoint) + 3] = CPoint( curX, curY);
	}
	else if( lpPoint[ ( *pnIdxPoint) + 3].x != curX)
	{
		// for not only one data.
		lpPoint[ ( *pnIdxPoint)].x = lpPoint[ ( *pnIdxPoint) + 3].x = lpPoint[ ( *pnIdxPoint) + 2].x;

		if( p_bWithNew)
		{
			( *pnCount)++;
			lpdwcntPoly[ ( *pnCount) - 1] = 4;
		}
		else lpdwcntPoly[ ( *pnCount) - 1] += 4;
		( *pnIdxPoint) += 4;
		lpPoint[ ( *pnIdxPoint)] = lpPoint[ ( *pnIdxPoint) + 1] = lpPoint[ ( *pnIdxPoint) + 2] = lpPoint[ ( *pnIdxPoint) + 3] = CPoint( curX, curY);
	}
	else
	{
		if( lpPoint[ ( *pnIdxPoint) + 1].y < curY) lpPoint[ ( *pnIdxPoint) + 1].y = curY;
		if( lpPoint[ ( *pnIdxPoint) + 2].y > curY) lpPoint[ ( *pnIdxPoint) + 2].y = curY;
		lpPoint[ ( *pnIdxPoint) + 3].y = curY;
	}
}

/*
void CBongDraw::SetValueFromStartToEnd(CCalcDataForDraw* pCalcData, int nS, int nH, int nL, int nE, int nCurX, int nMerCnt, int nHighLow,
									   int nIdxLine, int nIdxPoly, CPoint ptPre, int nDTI, int nPolyCnt)
{
	if(pCalcData)
	{
		if(nHighLow == 1)
		{
			pCalcData->m_nS_High   = nS;
			pCalcData->m_nH_High   = nH;
			pCalcData->m_nL_High   = nL;
			pCalcData->m_nE_High   = nE;
		    pCalcData->m_nCurXHigh = nCurX;
			pCalcData->m_nMerCnt_High = nMerCnt;
			pCalcData->nLastPixel_high = nIdxLine - 4;
			if(nIdxPoly > 0) pCalcData->nCount_High     = nIdxPoly;
			pCalcData->nLastPolyCnt_high = nPolyCnt;
	
		}
		else if(nHighLow == 2) 
		{
			pCalcData->m_nS_Low = nS;
			pCalcData->m_nH_Low	= nH;
			pCalcData->m_nL_Low	= nL;
			pCalcData->m_nE_Low = nE;
			pCalcData->m_nCurXLow  = nCurX;
			pCalcData->m_nMerCnt_Low = nMerCnt;
			pCalcData->nLastPixel_Low = nIdxLine - 4;
			if(nIdxPoly > 0) pCalcData->nCount_Low     = nIdxPoly;
			pCalcData->nLastPolyCnt_Low = nPolyCnt;
		}
		pCalcData->m_Prept = ptPre;
		pCalcData->nDTI_Start_LastPixel = nDTI;
		pCalcData->m_nHighLow = nHighLow;
	}
}

BOOL CBongDraw::DrawSolidBong_New(int& nIdxLine, int& nIdxPoly, POINT* ptLine, DWORD* dwcntPoly, int& nCurX, int& nMergeCnt, int& nS, int& nH, int& nL, int& nE, POINT& ptOne, CCalcDataForDraw* pCalcData)
{	
	if( nCurX == ptOne.x)
	{
		if(ptOne.y > nH) nH = ptOne.y;
		if(ptOne.y < nL) nL = ptOne.y;

		nE = ptOne.y;	
		nMergeCnt++;		
	}
	else
	{
		if( nMergeCnt != 0)
		{
			// (2009/2/10 - Seung-Won, Bae) Check Overflow exception.
			if( pCalcData->m_nPoints - 3 <= nIdxLine) return FALSE;

			int nE_i = nE;
			if((nL == nE_i)) nE_i++;

			ptLine[nIdxLine].x = nCurX;
			ptLine[nIdxLine].y = nS;
			nIdxLine++;

			ptLine[nIdxLine].x = nCurX;
			ptLine[nIdxLine].y = nH;
			nIdxLine++;

			ptLine[nIdxLine].x = nCurX;
			ptLine[nIdxLine].y = nL;
			nIdxLine++;

			ptLine[nIdxLine].x = nCurX;
			ptLine[nIdxLine].y = nE_i;
			nIdxLine++;

			dwcntPoly[nIdxPoly] += 4;
			nMergeCnt = 0;
		}

		if( nCurX + 1 < ptOne.x || ptOne.x + 1 < nCurX)
		{
			nIdxPoly++;
			dwcntPoly[nIdxPoly] = 0;
		}

		nCurX = ptOne.x;
		nS = nH = nL = nE = ptOne.y;
		nMergeCnt = 1;
	}

	return TRUE;
}


BOOL CBongDraw::DrawSolidBong_End(int& nIdxLine, int& nIdxPoly, POINT* ptLine, DWORD* dwcntPoly, int& nCurX, int& nMergeCnt, int& nS, int& nH, int& nL, int& nE,
								  CCalcDataForDraw* pCalcData, int nHighLow, int nDTI, CPoint pt)
{
	if(nMergeCnt != 0)
	{
		// (2009/2/10 - Seung-Won, Bae) Check Overflow exception.
		if( pCalcData->m_nPoints - 3 <= nIdxLine) return FALSE;

		int nE_i = nE;
		if((nL == nE_i)) nE_i++;

		ptLine[nIdxLine].x = nCurX;
		ptLine[nIdxLine].y = nS;
		nIdxLine++;

		ptLine[nIdxLine].x = nCurX;
		ptLine[nIdxLine].y = nH;
		nIdxLine++;

		ptLine[nIdxLine].x = nCurX;
		ptLine[nIdxLine].y = nL;
		nIdxLine++;

		ptLine[nIdxLine].x = nCurX;
		ptLine[nIdxLine].y = nE_i;
		nIdxLine++;

		dwcntPoly[nIdxPoly] += 4;
		nMergeCnt = 0;
	}
	return TRUE;
}

void CBongDraw::DrawSolidBong_Item(int& nIdxLine, int& nIdxPoly, POINT* ptLine, DWORD* dwcntPoly, int& nCurX, int& nMergeCnt, int& nS, int& nH, int& nL, int& nE, POINT& ptOne, POINT& ptTwo)
{
	ptLine[nIdxLine++] = ptOne;
	ptLine[nIdxLine++] = ptTwo;

	nIdxPoly++;
	dwcntPoly[nIdxPoly] = 2;
	nMergeCnt = 2;
}
*/

// packet 순서가 중요!
// 1: 시가, 매수(color1)  2: 고가(color3)  3: 저가(color3)  4: 종가, 매도(color2)
bool CBongDraw::DrawAntennaBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw)
{
	CList<double, double>* pPoleHighList = NULL, *pPoleLowList = NULL, *pData1List = NULL, *pData2List = NULL;
	POSITION poleHighPos = NULL, poleLowPos = NULL, data1Pos = NULL, data2Pos = NULL;
	if(!GetPacketData_Antenna(packetList, nRealStart, pPoleHighList, pPoleLowList, 
		pData1List, pData2List, poleHighPos, poleLowPos, data1Pos, data2Pos))
		return false;

	CPen data1Pen(PS_SOLID, 1, graphColor.GetColor1());
	CPen data2Pen(PS_SOLID, 1, graphColor.GetColor3());
	CPen polePen(PS_SOLID, 1, graphColor.GetColor5());

	for(int nIndex = 0; nIndex <= (nRealEnd - nRealStart); nIndex++)
	{
		if(poleHighPos == NULL || poleLowPos == NULL || data1Pos == NULL || data2Pos == NULL)
			break;
		
		double dPoleHigh = CDraw::CalLogValue(bLog, pPoleHighList->GetNext(poleHighPos));
		double dPoleLow = CDraw::CalLogValue(bLog, pPoleLowList->GetNext(poleLowPos));
		double dData1 = CDraw::CalLogValue(bLog, pData1List->GetNext(data1Pos));
		double dData2 = CDraw::CalLogValue(bLog, pData2List->GetNext(data2Pos));

		// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( dPoleHigh < dData1 || dPoleHigh < dData2 || dData1 < dPoleLow || dData2 < dPoleLow)
			dPoleHigh = dData1 = dPoleLow = dData2;

		/// 봉의 중간 지점을 얻기 위해 마지막 인자를 true로 줌
		CPoint poleHighPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dViewMin, dViewMax, dPoleHigh, true);
		CPoint poleLowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dViewMin, dViewMax, dPoleLow, true);

		/// 봉Rect의 X축 끝점 및 Y축 시가/종가
		CPoint data1Pt = GetVerticalPoint_Antenna(poleHighPt.x, 1, graphRegion, nIndex, dRealWidth, dViewMin, dViewMax, dData1);
		CPoint data2Pt = GetVerticalPoint_Antenna(poleHighPt.x, -1, graphRegion, nIndex + 1, dRealWidth, dViewMin, dViewMax, dData2);

		if(bInvert){
			poleHighPt.y = CDataConversion::InvertGraphDataPt(graphRegion, poleHighPt.y);
			poleLowPt.y = CDataConversion::InvertGraphDataPt(graphRegion, poleLowPt.y);
			data1Pt.y = CDataConversion::InvertGraphDataPt(graphRegion, data1Pt.y);
			data2Pt.y = CDataConversion::InvertGraphDataPt(graphRegion, data2Pt.y);
		}

		// 기둥 그리기
		CDraw::DrawLine(pDC, &polePen, poleHighPt, poleLowPt);
		// data 그리기
		CDraw::DrawLine(pDC, &data1Pen, data1Pt, CPoint(data2Pt.x, data1Pt.y));
		CDraw::DrawLine(pDC, &data2Pen, CPoint(data1Pt.x, data2Pt.y), data2Pt);
	}

	data1Pen.DeleteObject();
	data2Pen.DeleteObject();
	polePen.DeleteObject();
	return true;
}

// KTB 체결잔고표시 - ojtaso (20091019)
bool CBongDraw::DrawAntennaBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
			const CObArray& arrInfo)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		CList<double, double>* pPoleHighList = NULL, *pPoleLowList = NULL, *pData1List = NULL, *pData2List = NULL;
		POSITION poleHighPos = NULL, poleLowPos = NULL, data1Pos = NULL, data2Pos = NULL;

		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		if(!GetPacketData_Antenna(packetList, nDTI_Start, pPoleHighList, pPoleLowList, 
			pData1List, pData2List, poleHighPos, poleLowPos, data1Pos, data2Pos))
			return false;

		CPen data1Pen(PS_SOLID, 1, graphColor.GetColor1());
		CPen data2Pen(PS_SOLID, 1, graphColor.GetColor3());
		CPen polePen(PS_SOLID, 1, graphColor.GetColor5());

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dRealHeight = dViewMax - dViewMin;
		double dViewY_High,dViewY_Low,dViewY_Data1,dViewY_Data2;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		// KTB 체결잔고표시 - ojtaso (20091019)
		int nCheJanInfo = arrInfo.GetSize();
		if(nCheJanInfo)
		{
			int nStart = pxScaleManager->GetGraphXArea_Left(p_nColumn);
			int nEnd = pxScaleManager->GetGraphXArea_Right(p_nColumn);

			CPen CheJanPen;
			for(int i = 0; i < nCheJanInfo; i++)
			{
				ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
				
				double dYAvgPrice = dViewBottom - (dViewHeight * ((pInfo->m_dPrice - dRealMin) / dRealHeight));

				CheJanPen.CreatePen(PS_SOLID, pInfo->m_nLine, pInfo->m_clrLine);
				pDC->SelectObject(&CheJanPen);

				pDC->MoveTo(nStart, dYAvgPrice);
				pDC->LineTo(nEnd, dYAvgPrice);
				
				SIZE size = pDC->GetTextExtent(pInfo->m_strTitle);
				pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);

				CheJanPen.DeleteObject();
			}
		}

		//	for(int nIndex = 0; nIndex <= (nRealEnd - nRealStart); nIndex++)
		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);

			if(parea == NULL)
				break;
			if(poleHighPos == NULL || poleLowPos == NULL || data1Pos == NULL || data2Pos == NULL)
				break;
			
			double dPoleHigh = CDraw::CalLogValue(bLog, pPoleHighList->GetNext(poleHighPos));
			double dPoleLow = CDraw::CalLogValue(bLog, pPoleLowList->GetNext(poleLowPos));
			double dData1 = CDraw::CalLogValue(bLog, pData1List->GetNext(data1Pos));
			double dData2 = CDraw::CalLogValue(bLog, pData2List->GetNext(data2Pos));

			// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( dPoleHigh < dData1 || dPoleHigh < dData2 || dData1 < dPoleLow || dData2 < dPoleLow)
				dPoleHigh = dData1 = dPoleLow = dData2;

			/// 봉의 중간 지점을 얻기 위해 마지막 인자를 true로 줌
			dViewY_High  = ROUND(dViewBottom - ( dViewHeight * ((dPoleHigh - dViewMin) / dRealHeight)));
			dViewY_Low   = ROUND(dViewBottom - ( dViewHeight * ((dPoleLow - dViewMin) / dRealHeight)));
	//		CPoint poleHighPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dViewMin, dViewMax, dPoleHigh, true);
	//		CPoint poleLowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dViewMin, dViewMax, dPoleLow, true);

			/// 봉Rect의 X축 끝점 및 Y축 시가/종가
			dViewY_Data1  = ROUND(dViewBottom - ( dViewHeight * ((dData1 - dViewMin) / dRealHeight)));
			dViewY_Data2  = ROUND(dViewBottom - ( dViewHeight * ((dData2 - dViewMin) / dRealHeight)));
	//		CPoint data1Pt = GetVerticalPoint_Antenna(poleHighPt.x, 1, graphRegion, nIndex, dRealWidth, dViewMin, dViewMax, dData1);
	//		CPoint data2Pt = GetVerticalPoint_Antenna(poleHighPt.x, -1, graphRegion, nIndex + 1, dRealWidth, dViewMin, dViewMax, dData2);

			if(bInvert)	{
				dViewY_High = nViewY_half2 - dViewY_High;
				dViewY_Low = nViewY_half2 - dViewY_Low;
				dViewY_Data1 = nViewY_half2 - dViewY_Data1;
				dViewY_Data2 = nViewY_half2 - dViewY_Data2;
			}	
	//		if(bInvert){
	//			poleHighPt.y = CDataConversion::InvertGraphDataPt(graphRegion, poleHighPt.y);
	//			poleLowPt.y = CDataConversion::InvertGraphDataPt(graphRegion, poleLowPt.y);
	//			data1Pt.y = CDataConversion::InvertGraphDataPt(graphRegion, data1Pt.y);
	//			data2Pt.y = CDataConversion::InvertGraphDataPt(graphRegion, data2Pt.y);
	//		}

			// 기둥 그리기
			CDraw::DrawLine(pDC, &polePen, CPoint(parea->center,dViewY_High), CPoint(parea->center,dViewY_Low));
	//		CDraw::DrawLine(pDC, &polePen, poleHighPt, poleLowPt);
			// data 그리기
			CDraw::DrawLine(pDC, &data1Pen, CPoint(parea->center,dViewY_Data1), CPoint(parea->center, dViewY_Data1));
			CDraw::DrawLine(pDC, &data2Pen, CPoint(parea->center, dViewY_Data2), CPoint(parea->center,dViewY_Data2));
	//		CDraw::DrawLine(pDC, &data1Pen, data1Pt, CPoint(data2Pt.x, data1Pt.y));
	//		CDraw::DrawLine(pDC, &data2Pen, CPoint(data1Pt.x, data2Pt.y), data2Pt);
		}

		data1Pen.DeleteObject();
		data2Pen.DeleteObject();
		polePen.DeleteObject();
	}
// 	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	return true;
}
/*
//대비선
void CBongDraw::DrawComparisonLine(CDC *pDC, const CRect *graphRegion, const double dRealWidth, const double dRealMin, const double dRealMax, const double& dViewMin, const double& dViewMax, CPacket* pClosePacket)
{
	if(pClosePacket == NULL)
		return;

	double nStandard = pClosePacket->GetStandardValue();
	///count는 x축을 계산하기 위한 것이므로 여기서는 고려 대상이 아니다. 포인트에서 x값만 뽑아쓰면 된다.
///	CPoint ComparisonDataPt = CDataConversion::GetVerticalPoint(graphRegion, count,	dRealWidth, dRealMin, dRealMax, nStandardValue);
	CPoint ComparisonDataPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dRealMin, dRealMax, nStandard);

	if(nStandard >= dViewMin && nStandard <= dViewMax)
		CDraw::DrawLine(pDC, CPoint(graphRegion->left, ComparisonDataPt.y), CPoint(graphRegion->right, ComparisonDataPt.y));
}
*/

void CBongDraw::SelectGdiObject(CDC* pDC, const int nBohapType, const double& dOpen, const double& dClose, 
		const double& dPreClose, const CGraphColor& graphColor, CBrush* pBrush, CPen* pPen)
{
	//상승
	if(dOpen < dClose){
		SelectGdiObject_HighLow(pDC, graphColor.GetUpGraphFill(), UP_BRUSH, UP_PEN, pBrush, pPen);
	}
	//하락
	else if(dOpen > dClose){
		SelectGdiObject_HighLow(pDC, graphColor.GetDownGraphFill(), DOWN_BRUSH, DOWN_PEN, pBrush, pPen);
	}
	//보합
	else{
		SelectGdiObject_Bohap(pDC, nBohapType, dClose, dPreClose, pPen);
	}
}

// 상승/하락
void CBongDraw::SelectGdiObject_HighLow(CDC* pDC, const bool bFill, const int nBrushIndex, 
		const int nPenIndex, CBrush* pBrush, CPen* pPen)
{
	if(bFill){
		if(nBrushIndex >= 0 && nBrushIndex < BRUSH_COUNT)
			pDC->SelectObject(pBrush[nBrushIndex]);
	}
	else
		pDC->SelectObject(GetStockObject(NULL_BRUSH));

	if(nPenIndex >= 0 && nPenIndex < PEN_COUNT)
		pDC->SelectObject(pPen[nPenIndex]);
}

// 보합
void CBongDraw::SelectGdiObject_Bohap(CDC* pDC, const int nBohapType, const double& dClose, 
		const double& dPreClose, CPen* pPen)
{
	pDC->SelectObject(GetStockObject(NULL_BRUSH));

	if (nBohapType == 1)					//일반보합
		pDC->SelectObject(pPen[BOHAP_PEN]);
	else if (nBohapType == 2)				//전일종가 보합
	{
		if (dClose > dPreClose)
			pDC->SelectObject(pPen[UP_PEN]);
		else
			pDC->SelectObject(pPen[DOWN_PEN]);
	}
	else {}									//전일 색상 따라감.
}

//sy 2005.09.14.
void CBongDraw::Draw_SolidBong(CDC* pDC, const CPoint& curPt, const int &nBase, const double& dRealWidth, CPen* pPen)
{
	if( nBase < curPt.y)
		pDC->SelectObject(pPen[DOWN_PEN]);
	else
		pDC->SelectObject(pPen[UP_PEN]);

	CPoint pt1 = CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y);
	CPoint pt2 = CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y);
	if(pt1 == pt2)
		pt2.x += 1;

	CDraw::DrawLine(pDC, pt1, pt2);
}
//sy end

void CBongDraw::Draw_SolidBong(CDC* pDC, const CPoint& prePt, const CPoint& curPt, const int &nBase, CPen* pPen)
{
	if(prePt.y >= nBase && curPt.y >= nBase){
		pDC->SelectObject(pPen[DOWN_PEN]);
		CDraw::DrawLine(pDC, prePt, curPt);
	}
	else if(prePt.y <= nBase && curPt.y <= nBase){
		pDC->SelectObject(pPen[UP_PEN]);
		CDraw::DrawLine(pDC, prePt, curPt);
	}		
	else{
		int nCrossLen = CalcMidPoint(nBase, prePt, curPt);
		if(prePt.y >= nBase){
			pDC->SelectObject(pPen[DOWN_PEN]);
			CDraw::DrawLine(pDC, CPoint(prePt.x, prePt.y), CPoint( nCrossLen, nBase));
			pDC->SelectObject(pPen[UP_PEN]);
			CDraw::DrawLine(pDC, CPoint( nCrossLen, nBase), CPoint(curPt.x, curPt.y));
		}
		else{
			pDC->SelectObject(pPen[UP_PEN]);
			CDraw::DrawLine(pDC, CPoint(prePt.x, prePt.y), CPoint( nCrossLen, nBase));
			pDC->SelectObject(pPen[DOWN_PEN]);
			CDraw::DrawLine(pDC, CPoint( nCrossLen, nBase), CPoint(curPt.x, curPt.y));
		}
	}	
}

void CBongDraw::InitGdiObject(CBrush* pBrush, CPen* pPen, const CGraphColor& graphColor, 
		const double& dWidth, const CPenThickness& penThickness, const bool bIsDrawingThickness)
{
	pBrush[UP_BRUSH].CreateSolidBrush(graphColor.GetColor2());
	pBrush[DOWN_BRUSH].CreateSolidBrush(graphColor.GetColor4());

	if(bIsDrawingThickness)
	{
		pPen[UP_PEN].CreatePen(PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		pPen[DOWN_PEN].CreatePen(PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
		pPen[BOHAP_PEN].CreatePen(PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor5());
		pPen[STANDARD_PEN].CreatePen(PS_SOLID, 1, graphColor.GetColor6());
	}
	else{
		pPen[UP_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor1());
		pPen[DOWN_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor3());
		pPen[BOHAP_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor5());
		pPen[STANDARD_PEN].CreatePen(PS_SOLID, 1, graphColor.GetColor6());
	}
}

void CBongDraw::DestroyGdiObject(CDC* pDC, CBrush* pBrush, CPen* pPen, CBrush* pOldBrush, CPen* pOldPen)
{
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	for(int i = 0; i < PEN_COUNT; i++)
		pPen[i].DeleteObject();

	for(int i = 0; i < BRUSH_COUNT; i++)
		pBrush[i].DeleteObject();
}

/*
int CBongDraw::GetBohapTypeInPriceChart(CIndicatorInfo* pIndicatorInfo, CIndicatorList *pIndicatorList) const
{
	if(pIndicatorInfo == NULL)
		return 1;

	CIndicatorInfo* pPriceIndiInfo = pIndicatorInfo;
	if(pIndicatorInfo->GetIndicatorName() != _MTEXT( C2_PRICE_CHART))
	{
		if(pIndicatorList == NULL)
			return 1;
		// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
		//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
		//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 한다.
		pPriceIndiInfo = pIndicatorList->GetIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART), EIIGT_WITH_DEFAULT_GET);
	}

	return GetBohapType(pPriceIndiInfo);
}
*/

int CBongDraw::GetBohapType(CIndicatorInfo* pIndicatorInfo)
{
	if(pIndicatorInfo == NULL)
		return 1;

	CList<double, double>* pDrawConditionList = pIndicatorInfo->GetDrawConditionList();
	if(pDrawConditionList == NULL || pDrawConditionList->GetCount() <= 0)
		return 1;

	int nType = (int)pDrawConditionList->GetHead();
	if(nType < 1)
		nType = 1;
	return nType;
}

// ----------------------------------------------------------------------------
bool CBongDraw::GetPacketData_Antenna(const CList<CPacket*, CPacket*>& packetList, const int nDataIndex, 
		CList<double, double>*& pPoleHighList, CList<double, double>*& pPoleLowList, 
		CList<double, double>*& pData1List, CList<double, double>*& pData2List, 
		POSITION& poleHighPos, POSITION& poleLowPos, POSITION& data1Pos, POSITION& data2Pos) const
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL || packetList.GetCount() < 4)
		return false;

	CPacket* pPacket = packetList.GetNext(pos);
	if(!GetPacketData(pPacket, nDataIndex, pData1List, data1Pos))
		return false;

	if(pos == NULL)
		return false;
	pPacket = packetList.GetNext(pos);
	if(!GetPacketData(pPacket, nDataIndex, pPoleHighList, poleHighPos))
		return false;

	if(pos == NULL)
		return false;
	pPacket = packetList.GetNext(pos);
	if(!GetPacketData(pPacket, nDataIndex, pPoleLowList, poleLowPos))
		return false;

	if(pos == NULL)
		return false;
	pPacket = packetList.GetNext(pos);
	return GetPacketData(pPacket, nDataIndex, pData2List, data2Pos);
}

bool CBongDraw::GetPacketData(CPacket* pPacket, const int nDataIndex, CList<double, double>*& pDataList, POSITION& dataPos) const
{
	if(pPacket == NULL || nDataIndex < 0)
		return false;

	pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL)
		return false;

	dataPos = pDataList->FindIndex(nDataIndex);
	return (dataPos != NULL);
}

CPoint CBongDraw::GetVerticalPoint_Antenna(const int nMaxPos_x, const int nGap, const CRect& region, const int nIndex, 
		const double& dWidth, const double& dViewMin, const double& dViewMax, const double& dData) const
{
	CPoint dataPt = CDataConversion::GetVerticalPoint(region, nIndex, dWidth, 
		dViewMin, dViewMax, dData, false);

	int nMaxGap = abs(nMaxPos_x - dataPt.x);
	if(nMaxGap > 10)
		dataPt.x += (3 * nGap);
	else if(nMaxGap > 3)
		dataPt.x += (1 * nGap);

	return dataPt;
}
  
// (2006.11.03 안승환) Flow차트 추가
bool CBongDraw::DrawFlowBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw)
{

	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL)		return false;
	if(packetList.GetCount() < 4)	return false;
	
	CPacket* pOpenPacket = packetList.GetNext(headPos);
	CPacket* pHighPacket = packetList.GetNext(headPos);
	CPacket* pLowPacket = packetList.GetNext(headPos);
	CPacket* pClosePacket = packetList.GetNext(headPos);

	if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

	POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nRealStart);

	if(highPos == NULL || lowPos == NULL || closePos == NULL)		return false;

	CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
	CList<double, double>* pLowList =  pLowPacket->GetnumericDataList();
	CList<double, double>* pCloseList =  pLowPacket->GetnumericDataList();

	if(pHighList == NULL || pLowList == NULL || pCloseList == NULL)	return false;

	double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
	pHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	pLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);

	CPen newPen, *pOldPen;
	CBrush bkBrush(graphColor.GetColor5()),*pOldBrush;
	newPen.CreatePen(PS_SOLID, GetThickness(penThickness.GetThickness1(), dRealWidth), graphColor.GetColor5());

	pOldPen = pDC->SelectObject(&newPen);
	pOldBrush = (CBrush*) pDC->SelectObject(&bkBrush);	

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	int nArrowLength = 11;
	bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;
	COLORREF clr = pDC->GetTextColor();
	
	// (2007/1/15 - Seung-Won, Bae) Skip Reallock
	if( nPtCount != ( nRealEnd - nRealStart) * 2 + 1)
	{
		if( pFlowPt) delete pFlowPt;
		nPtCount = ( nRealEnd - nRealStart) * 2 + 1;
		pFlowPt = new CPoint[ nPtCount + 1];
	}

	int j = 0;
	for(int i = nRealStart; i <= nRealEnd; i++,j++)
	{

		// (2007/1/15 - Seung-Won, Bae) Do not break. It makes garbages.
		if(highPos == NULL || lowPos == NULL)
		{
			pFlowPt[ j]				= pFlowPt[ j - 1];
			pFlowPt[ nPtCount - j]	= pFlowPt[ nPtCount - j + 1];
			continue;
		}

		double dHigh = pHighList->GetNext(highPos);
		double dLow = pLowList->GetNext(lowPos);
		double dClose	= pCloseList->GetNext(closePos);

		// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( dHigh < dClose || dClose < dLow) dHigh = dLow = dClose;
		
		const int nIndex = i - nRealStart;
		CPoint highPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dHigh), true);
		CPoint lowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dLow), true);

		if(bInvert)
		{
			highPt.y = CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
			lowPt.y = CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
		}

		// 화살표를 그려주는 부분 ================================
		int MODE = pDC->SetBkMode(TRANSPARENT);

		// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
		if( bGraphTextDraw && ( ( bHighText	&& dHigh == dHighMax) || ( bLowText	&& dLow == dLowMin)))
		{
			CString strTime;
			GetPriceMinMaxTime( strTime, pHighPacket, i);
			if( bHighText	&& dHigh	== dHighMax)
			{
				bHighText = false;
				DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, highPt.x, highPt.y, bInvert);
			}
			if( bLowText	&& dLow		== dLowMin)
			{
				bLowText = false;
				DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, lowPt.x, lowPt.y, bInvert);
			}
		}

		pDC->SetBkMode(MODE);

		pFlowPt[j] = highPt;
		pFlowPt[nPtCount - j] = lowPt;
	}

    pDC->Polygon(pFlowPt,nPtCount+1);

	pDC->SetTextColor(clr);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	newPen.DeleteObject();

	return true;
}
// (2006.11.03 안승환) Flow차트 추가
// KTB 체결잔고표시 - ojtaso (20091019)
bool CBongDraw::DrawFlowBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_Column,
			const CObArray& arrInfo)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		POSITION headPos = packetList.GetHeadPosition();
		if(headPos == NULL)		return false;
		if(packetList.GetCount() < 4)	return false;
		
		CPacket* pOpenPacket = packetList.GetNext(headPos);
		CPacket* pHighPacket = packetList.GetNext(headPos);
		CPacket* pLowPacket = packetList.GetNext(headPos);
		CPacket* pClosePacket = packetList.GetNext(headPos);

		if(!pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket)	return false;

		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nDTI_Start);

		if(highPos == NULL || lowPos == NULL || closePos == NULL)		return false;

		CList<double, double>* pHighList = pHighPacket->GetnumericDataList();
		CList<double, double>* pLowList =  pLowPacket->GetnumericDataList();
		CList<double, double>* pCloseList =  pLowPacket->GetnumericDataList();

		if(pHighList == NULL || pLowList == NULL || pCloseList == NULL)	return false;

		double dHighMax = 0.0, dLowMin = 0.0, dTempMinMax = 0.0;
		pHighPacket->GetMinMax(nDTI_Start, nDTI_End, dTempMinMax, dHighMax);
		pLowPacket->GetMinMax(nDTI_Start, nDTI_End, dLowMin, dTempMinMax);
	//	pHighPacket->GetMinMax(nRealStart, nRealEnd, dTempMinMax, dHighMax);
	//	pLowPacket->GetMinMax(nRealStart, nRealEnd, dLowMin, dTempMinMax);


		CPen newPen, *pOldPen;
		CBrush bkBrush(graphColor.GetColor5()),*pOldBrush;
		newPen.CreatePen(PS_SOLID, GetThickness(penThickness.GetThickness1(), dRealWidth), graphColor.GetColor5());

		pOldPen = pDC->SelectObject(&newPen);
		pOldBrush = (CBrush*) pDC->SelectObject(&bkBrush);	

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dRealHeight = dRealMax - dRealMin;

		int nArrowLength = 11;
		bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;
		COLORREF clr = pDC->GetTextColor();
		
		// (2007/1/15 - Seung-Won, Bae) Skip Reallock
		if( nPtCount != ( nDTI_End - nDTI_Start) * 2 + 1)
		{
			if( pFlowPt) delete pFlowPt;
			nPtCount = ( nDTI_End - nDTI_Start) * 2 + 1;
			pFlowPt = new CPoint[ nPtCount + 1];
		}

		double dViewY_High, dViewY_Low;
		int j = 0;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_Column, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		// KTB 체결잔고표시 - ojtaso (20091019)
		int nCheJanInfo = arrInfo.GetSize();
		if(nCheJanInfo)
		{
			int nStart = pxScaleManager->GetGraphXArea_Left(p_Column);
			int nEnd = pxScaleManager->GetGraphXArea_Right(p_Column);

			CPen CheJanPen;
			for(int i = 0; i < nCheJanInfo; i++)
			{
				ST_HORZLINE* pInfo = (ST_HORZLINE*)arrInfo.GetAt(i);
				
				double dYAvgPrice = dViewBottom - (dViewHeight * ((pInfo->m_dPrice - dRealMin) / dRealHeight));

				CheJanPen.CreatePen(PS_SOLID, pInfo->m_nLine, pInfo->m_clrLine);
				pDC->SelectObject(&CheJanPen);

				pDC->MoveTo(nStart, dYAvgPrice);
				pDC->LineTo(nEnd, dYAvgPrice);
				
				SIZE size = pDC->GetTextExtent(pInfo->m_strTitle);
				pDC->TextOut(nEnd - size.cx, dYAvgPrice - size.cy, pInfo->m_strTitle);

				CheJanPen.DeleteObject();
			}
		}

		double dMinMaxRatioPrice = pxScaleManager->GetPriceMinMaxShowType() == 0 ? pClosePacket->GetnumericDataList()->GetTail() : pClosePacket->GetStandardValue();

		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++,j++)
	//	for(int i = nRealStart; i <= nRealEnd; i++,j++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);

			if(parea == NULL)
				break;
			// (2007/1/15 - Seung-Won, Bae) Do not break. It makes garbages.
			if(highPos == NULL || lowPos == NULL)
			{
				pFlowPt[ j]				= pFlowPt[ j - 1];
				pFlowPt[ nPtCount - j]	= pFlowPt[ nPtCount - j + 1];
				continue;
			}

			double dHigh = pHighList->GetNext(highPos);
			double dLow = pLowList->GetNext(lowPos);
			double dClose	= pCloseList->GetNext(closePos);

			// (2007/2/6 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( dHigh < dClose || dClose < dLow) dHigh = dLow = dClose;
			
			const int nIndex = nDTI - nDTI_Start;

	//		CPoint highPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dHigh), true);
	//		CPoint lowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dRealMin, dRealMax, CDraw::CalLogValue(bLog, dLow), true);
	//		if(bInvert)		{
	//			highPt.y = CDataConversion::InvertGraphDataPt(graphRegion, highPt.y);
	//			lowPt.y = CDataConversion::InvertGraphDataPt(graphRegion, lowPt.y);
	//		}
			dViewY_High  = ROUND(dViewBottom - ( dViewHeight * ((dHigh - dRealMin) / dRealHeight)));
			dViewY_Low   = ROUND(dViewBottom - ( dViewHeight * ((dLow - dRealMin) / dRealHeight)));
			if(bInvert)	{
				dViewY_High = nViewY_half2 - dViewY_High;
				dViewY_Low = nViewY_half2 - dViewY_Low;
			}

			// 화살표를 그려주는 부분 ================================
			int MODE = pDC->SetBkMode(TRANSPARENT);

			// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
			if( bGraphTextDraw && ( ( bHighText	&& dHigh == dHighMax) || ( bLowText	&& dLow == dLowMin)))
			{
				CString strRatio(_T(""));
				CString strTime;
				GetPriceMinMaxTime( strTime, pHighPacket, nDTI);
				if( bHighText	&& dHigh	== dHighMax)
				{
					if(dMinMaxRatioPrice != -HUGE_VAL)
					{
						double dPercentData = (dHigh * 100.0) / dMinMaxRatioPrice;
						strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
						strTime.Insert(strTime.Find("(") + 1, strRatio);
					}

					bHighText = false;
					DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, parea->center, dViewY_High, bInvert);
					//DrawPriceMaxText( pDC, graphRegion, graphColor,	CDataConversion::GetStringFormatTrans( CDataConversion::DoubleToString( dHigh),	pHighPacket->GetType()) + strTime, highPt.x, highPt.y, bInvert);
				}
				if( bLowText	&& dLow		== dLowMin)
				{
					if(dMinMaxRatioPrice != -HUGE_VAL)
					{
						double dPercentData = (dLow * 100.0) / dMinMaxRatioPrice;
						strRatio.Format("%s%% ", CDataConversion::DoubleToString(dPercentData - 100, 2));
						strTime.Insert(strTime.Find("(") + 1, strRatio);
					}

					bLowText = false;
					DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, parea->center, dViewY_Low, bInvert);
					//DrawPriceMinText( pDC, graphRegion, graphColor,	CDataConversion::GetStringFormatTrans( CDataConversion::DoubleToString( dLow),	pLowPacket->GetType())	+ strTime, lowPt.x, lowPt.y, bInvert);
				}
			}

			pDC->SetBkMode(MODE);

	//		pFlowPt[j] = highPt;
	//		pFlowPt[nPtCount - j] = lowPt;
			pFlowPt[j].x = parea->center,pFlowPt[j].y = dViewY_High;
			pFlowPt[nPtCount - j].x = parea->center,pFlowPt[nPtCount - j].y = dViewY_Low;
		}

		pDC->Polygon(pFlowPt,nPtCount+1);

		pDC->SetTextColor(clr);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		newPen.DeleteObject();
	}
// 	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}

	return true;
}