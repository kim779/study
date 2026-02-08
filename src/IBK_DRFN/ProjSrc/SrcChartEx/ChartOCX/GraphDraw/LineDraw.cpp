// LineDraw.cpp: implementation of the CLineDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineDraw.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "DataMath.h"
#include "Conversion.h"
#include "BaseData.h"
#include "PacketPointerList.h"
#include "PacketBase.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "XScaleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineDraw::CLineDraw()
{
	m_eGraphType = CGraphBaseData::Line_Type;
}

CLineDraw::~CLineDraw()
{
}

bool CLineDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);
	const CGraphDrawStyle& drawStyle = subGraphData.GetDrawStyle();
	const CGraphColor& graphColor = subGraphData.GetColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	bool bLog = drawingGraphData.DoesVertScaleLogType();
	bool bInvert = drawingGraphData.DoesVertScaleInvertType();
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	const CGraphBaseLineStyle& baseLineStyle = subGraphData.GetBaseLineDrawStyle();

	bool bResult = false;
	switch(subGraphData.GetStyle().GetGraphLineStyle())
	{
	case CGraphBaseData::SimpleLine:
		// 기준선 두께, 모양 설정 - ojtaso (20071213)
		bResult = DrawSimpleLine(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, baseLineStyle, bLog, bInvert);
		break;
	case CGraphBaseData::FillLine:
		bResult = DrawFillLine(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::CloudLine:
		bResult = DrawCloudLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::HorizontalLine:
		bResult = DrawHorizontalLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::StairLine:
		bResult = DrawStairLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::NonZeroLine:
		bResult = DrawNonZeroLine(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::ComparisonLine:
		bResult = DrawComparisonLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert, drawingGraphData.GetGraphRegions().IsText(), drawingGraphData.GetWithBaseText(),
			drawingGraphData.GetDrawBaseLine());	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		break;
	case CGraphBaseData::RainbowLine:
		bResult = DrawRainbowLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, bLog, bInvert);
		break;
	case CGraphBaseData::ZigZagLine:
		bResult = DrawZigzagLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::LineCloudLine:
		bResult = DrawLineCloudLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::DataDotSimpleLine:
		bResult = DrawDataDotSimple(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::VerticalLine:
		bResult = DrawVerticalLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMax(), drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMin(), 
			dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	case CGraphBaseData::NonCloudLine:
		bResult = DrawNonCloudLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
		break;
	}

	pDC->SetBkMode(nOldMode);
	return bResult;
}

bool CLineDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData,
			const CString& strRQ,
			CXScaleManager* pxScaleManager)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);
	const CGraphDrawStyle& drawStyle = subGraphData.GetDrawStyle();
	const CGraphColor& graphColor = subGraphData.GetColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	bool bLog = drawingGraphData.DoesVertScaleLogType();
	bool bInvert = drawingGraphData.DoesVertScaleInvertType();
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	const CGraphBaseLineStyle& baseLineStyle = subGraphData.GetBaseLineDrawStyle();
	
	bool bResult = false;
	switch(subGraphData.GetStyle().GetGraphLineStyle())
	{
	case CGraphBaseData::SimpleLine:
		// 기준선 두께, 모양 설정 - ojtaso (20071213)
		bResult = DrawSimpleLine(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, baseLineStyle, bLog, bInvert,
			strRQ, pxScaleManager, subGraphData, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::FillLine:
		bResult = DrawFillLine(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, const_cast<CSubGraphData&>(subGraphData), drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::CloudLine:
		bResult = DrawCloudLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::HorizontalLine:
		bResult = DrawHorizontalLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::StairLine:
		bResult = DrawStairLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::NonZeroLine:
		bResult = DrawNonZeroLine(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::ComparisonLine:
		bResult = DrawComparisonLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert, drawingGraphData.GetGraphRegions().IsText(), drawingGraphData.GetWithBaseText(), drawingGraphData.GetDrawBaseLine(), // 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::RainbowLine:
		bResult = DrawRainbowLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::ZigZagLine:
		bResult = DrawZigzagLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::LineCloudLine:
		bResult = DrawLineCloudLine(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::DataDotSimpleLine:
		bResult = DrawDataDotSimple(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::VerticalLine:
		bResult = DrawVerticalLine(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMax(), drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMin(), 
			dRealWidth, packetList, penThickness, bLog, bInvert,strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	case CGraphBaseData::NonCloudLine:
		bResult = DrawNonCloudLine(pDC, realRegion, drawStyle, graphColor, pIndicatorInfo, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert,
			strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());
		break;
	}

	pDC->SetBkMode(nOldMode);
	return bResult;
}

bool CLineDraw::DrawSimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, const CGraphBaseLineStyle& baseLineStyle, bool bLog, bool bInvert)
{
	// 기준선을 그린다.
	DrawBaseLine_SimpleLine(pDC, graphRegion, drawStyle, graphColor, pIndicatorInfo, dViewMax, dViewMin, dRealWidth, bInvert, penThickness, baseLineStyle);

	// data를 그린다.
	return DrawDataLine_SimpleLine(pDC, graphRegion, drawStyle, graphColor, pIndicatorInfo, 
		nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert);
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
bool CLineDraw::DrawSimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, const CGraphBaseLineStyle& baseLineStyle, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, const CSubGraphData& subGraphData, int p_nColumn)
{
	// 기준선을 그린다.
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	DrawBaseLine_SimpleLine(pDC, graphRegion, drawStyle, graphColor, pIndicatorInfo, dViewMax, dViewMin, dRealWidth, bInvert, penThickness, baseLineStyle);

	// data를 그린다.
	return DrawDataLine_SimpleLine(pDC, graphRegion, drawStyle, graphColor, pIndicatorInfo, 
		nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, penThickness, bLog, bInvert, strRQ, pxScaleManager, 
		const_cast<CSubGraphData&>(subGraphData), p_nColumn);
}

// 기준선 그리기
// 기준선 두께, 모양 설정 - ojtaso (20071213)
bool CLineDraw::DrawBaseLine_SimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& /*drawStyle*/, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, bool bInvert,
		const CPenThickness& penThickness, const CGraphBaseLineStyle& baseLineStyle)
{
	if(pIndicatorInfo == NULL)
		return false;

	CList<double, double>* pBaseLineList = pIndicatorInfo->GetBaseLineList();
	if(pBaseLineList == NULL || pBaseLineList->GetCount() <= 0)
		return false;

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	CPen standardPen;
	CreatePen(standardPen, baseLineStyle, penThickness.GetBaseLineThickness(), graphColor.GetColor6());
//	standardPen.CreatePen(PS_DASH, 1, graphColor.GetColor6());
	//CreatePen(standardPen, drawStyle, 1, graphColor.GetColor6());
	CPen* pOldPen = pDC->SelectObject(&standardPen);

	POSITION pos = pBaseLineList->GetHeadPosition();
	while(pos != NULL){
		double dStandard = pBaseLineList->GetNext(pos);
		CPoint standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
		if(bInvert == true)	
			standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

		if(dStandard >= dViewMin && dStandard <= dViewMax)
			CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
	}

	pDC->SelectObject(pOldPen);
	standardPen.DeleteObject();	
	return true;
}

bool CLineDraw::DrawDataLine_SimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	if( packetList.IsEmpty()) return false;
	CPacket *pPacket = packetList.GetHead();
	if( !pPacket) return false;

	CList<double, double>* pDataList = NULL;
	POSITION pos = NULL;
	int nDrawStart = nRealStart, nDrawEnd = nRealEnd, nArrowIndex = -1;
	double dRealMin = dViewMin, dRealMax = dViewMax;
	CPoint prePt(0, 0);
	if(!GetEnvironment_SimpleLine(pIndicatorInfo, pPacket, graphRegion, 
		dRealWidth, nRealStart, dViewMin, dViewMax, bLog, bInvert, 
		pDataList, pos, nDrawStart, nDrawEnd, nArrowIndex, dRealMin, dRealMax, prePt))
		return false;

	CPen linePen;
	CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&linePen);

	double dViewBottom = graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;

	// 중간 지점을 구하는 방식을 기존에 않좋은 방법을 사용해서 구하고 있다
	// 해서 정확한 기존 위치와 일치시키기 위해서 다음을 포기한다.
	double dViewX_Start = graphRegion.left + (dRealWidth * (nDrawStart - nRealStart));
	double dViewX_End	= dViewX_Start + dRealWidth;
	int nViewX_Start, nViewX_End;

	double dRealHeight = dRealMax - dRealMin;

	double dY, dViewY;
	int nViewX, nViewY;

	// 이 루틴도 필요없을거 같지만 for문 않에서 체크하는 코드를 없애기 위해서 넣어줍니다.
	if(pDataList->GetCount() <= nDrawEnd) nDrawEnd = pDataList->GetCount() - 1;

	// (2007/2/22 - Seung-Won, Bae) Support Exception Value for Continue(Skip) Code
	double dContinueCode = pPacket->GetContinueCode();

	//sy 2003.8.24. 그리는 data 가 1개일 경우 수평선으로 표현
	// 주의: nRealEnd 로 비교해선 안된다.(margin 이 있기 때문에..)
	if(nDrawStart == nDrawEnd)
	{
		dY = pDataList->GetNext(pos);
		if( dContinueCode != dY)
		{
			if(bLog) dY = CMath::Log(dY);

			dViewY = dViewBottom - (dViewHeight * ((dY - dRealMin) / dRealHeight));

			nViewY = ROUND(dViewY);

			if(bInvert) nViewY = nViewY_half2 - nViewY;

			// (2007/5/8 - Seung-Won, Bae) Draw in Only One-Candle Width.
			double dViewX = (double)graphRegion.left + (dRealWidth / 2.0) + (dRealWidth * (nDrawStart - nRealStart));
			pDC->MoveTo( ROUND(dViewX) - (dRealWidth / 2.0), nViewY);
			pDC->LineTo( ROUND(dViewX) + (dRealWidth / 2.0), nViewY);
		}
	}
	else
	{
		if( nDrawStart > pPacket->GetStartPeriod()) 
		{ 
			nDrawStart--; 
			dViewX_Start -= dRealWidth;
			dViewX_End -= dRealWidth;
		}

		// (2007/2/22 - Seung-Won, Bae) Support Exception Value for Continue(Skip) Code
		double dOldValue = dContinueCode;
		pos = pDataList->FindIndex(nDrawStart);
		POSITION posBack = pos;
		if( posBack)
		{
			pDataList->GetPrev( posBack);
			while( posBack && dOldValue == dContinueCode)
				dOldValue = pDataList->GetPrev( posBack);
		}

		if(nDrawStart <= nDrawEnd)
		{
			POINT* pptLine = new POINT[nDrawEnd - nDrawStart + 1];

			for(int i = nDrawStart, nIndex = 0; i <= nDrawEnd; i++)
			{		
				dY = pDataList->GetNext(pos);

				// (2007/2/22 - Seung-Won, Bae) Support Exception Value for Continue(Skip) Code
				if( dContinueCode == dY)	dY = dOldValue;
				else						dOldValue = dY;
				
				if(bLog) dY = CMath::Log(dY);

				dViewY = dViewBottom - (dViewHeight * ((dY - dRealMin) / dRealHeight));
				
				nViewX_Start = ROUND(dViewX_Start);
				nViewX_End = ROUND(dViewX_End);
				
				nViewX = nViewX_Start + (nViewX_End - nViewX_Start) / 2;
				nViewY = ROUND(dViewY);

				if(bInvert) nViewY = nViewY_half2 - nViewY;

				pptLine[nIndex].x = nViewX;
				pptLine[nIndex].y = nViewY;

				if(i == nArrowIndex) DrawArrow(pDC, CPoint(nViewX, nViewY), graphColor.GetColor1());

				dViewX_Start += dRealWidth;
				dViewX_End += dRealWidth;

				nIndex++;
			}

			DrawPolyLine(pDC, pptLine, nDrawEnd - nDrawStart + 1, penThickness.GetDrawingThickness1());

			delete [] pptLine;
		}
	}

	pDC->SelectObject(pOldPen);
	linePen.DeleteObject();

	return true;
}

bool CLineDraw::DrawDataLine_SimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn)
{
	//2009.01.06 차트 성능 개선. 김성조
	//구분자가 THREAD_CALCULATION일경우는 계산 후 결과값만 산출.
// 	class CSimpleLine_Data
// 	{
// 	public:
// 		int	   nFlag;
// 		int	   nDTI_Start, nDTI_End; 
// 		int	   nArrowIndex;
// 		POINT* pptLine;
// 	};
	if(pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		int nDTI, nDTI_Start, nDTI_End;
		
		BOOL bOEBT;
		if(pIndicatorInfo == NULL) bOEBT = FALSE;
		else					   bOEBT = (pIndicatorInfo->GetIndicatorName() == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART));

		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		//		Alloc max memory.
		int nPoints = pxScaleManager->GetColumnWidth( p_nColumn)  * 4;
		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( nPoints < 1) return true;

		// the change of Graph Style, Y Min/Max is xScaleMode 2 (Don't ask X Scale Manager !)
		int xScaleMode = 2;
		if(	subGraphData.m_pGraphCache)
		{	
			//Check YValue
			bool bYValue = subGraphData.m_pGraphCache->CheckYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nPoints);
			if(bYValue)
			{					
				if(pxScaleManager) xScaleMode = pxScaleManager->GetCalculationMode();
				if(xScaleMode == 0)	return true;
			}
		}			
		if( xScaleMode == 2)
		{	
			if( subGraphData.m_pGraphCache) subGraphData.m_pGraphCache->ResetValue();
			else							subGraphData.m_pGraphCache = new CCalcDataForDraw;
			subGraphData.m_pGraphCache->SetYValue(dViewMax, dViewMin, bLog, bInvert, graphRegion, nPoints);
			subGraphData.m_pGraphCache->nArrowIndex = -1;
		}

		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		//YValue Set
		
		pxScaleManager->GetDrawIndex(strRQ, FALSE, bOEBT, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		CList<double, double>* pDataList = NULL;
		POSITION pos = NULL;
		int nDrawStart = nDTI_Start, nDrawEnd = nDTI_End, nArrowIndex = -1;
		double dRealMin = dViewMin, dRealMax = dViewMax;		

		CPoint prePt(0, 0);
		if(!GetEnvironment_SimpleLine(pIndicatorInfo, packetList.GetHead(), graphRegion, 
			dRealWidth, nRealStart, dViewMin, dViewMax, bLog, bInvert, 
			pDataList, pos, nDrawStart, nDrawEnd, nArrowIndex, dRealMin, dRealMax, prePt))
			return false;

		// "가격 이동평균", "거래량 이동평균" 인 경우 화살표를 그린다.
		// 130612 이상신 : 이평선 화살표 안나타나는 현상 수정(ISSUE NO - 6817).
		nArrowIndex = GetArrowIndex(pIndicatorInfo, subGraphData.GetName(), nDrawEnd);

		double dViewBottom = graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;

		double dRealHeight = dRealMax - dRealMin;

		double dY, dViewY;
		int nViewY;

		// 이 루틴도 필요없을거 같지만 for문 않에서 체크하는 코드를 없애기 위해서 넣어줍니다.
		if(pDataList->GetCount() <= nDrawEnd) nDrawEnd = pDataList->GetCount() - 1;

		//sy 2003.8.24. 그리는 data 가 1개일 경우 수평선으로 표현
		// 주의: nRealEnd 로 비교해선 안된다.(margin 이 있기 때문에..)

		if(nDTI_Start < packetList.GetHead()->GetStartPeriod()) nDTI_Start = packetList.GetHead()->GetStartPeriod();

		if(nDTI_Start == nDTI_End)
		{
			pCalcData->nFlag = 1;
			
			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			pCalcData->Alloc_pptLine( 2);

			CXScaleManager::PAREA parea;

			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
			int nZeroDTI = 0;
			CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
			if( !pblaAreafDTI) return false;
			CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
			pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);
			
			if(parea == NULL) 
			{
				pCalcData->nFlag = 0;
				return true;
			}

			dY = pDataList->GetNext(pos);

			if(bLog) dY = CMath::Log(dY);

			dViewY = dViewBottom - (dViewHeight * ((dY - dRealMin) / dRealHeight));


			nViewY = ROUND(dViewY);

			if(bInvert) nViewY = nViewY_half2 - nViewY;

			double dViewX = (double)parea->left + (dRealWidth / 2.0) + (dRealWidth * (nDrawStart - nRealStart));

			//차트 성능 개선 - 데이터 저장
			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			POINT *pptLine = pCalcData->Get_pptLine();
			pptLine[0].x = ROUND(dViewX) - (dRealWidth / 2.0);
			pptLine[0].y = nViewY;
			pptLine[1].x = ROUND(dViewX) + (dRealWidth / 2.0);
			pptLine[1].y = nViewY;

			// (2009/5/4 - Seung-Won, Bae) Show single data.
			pCalcData->nPtCnt = 2;  //Point 개수
		}
		else if(nDTI_Start < nDTI_End)
		{
			pCalcData->nFlag = 2;		

			// (2009/1/28 - Seung-Won, Bae) for mode 1.
			int nIndex = -4;
			if( xScaleMode == 2 )
			{
				// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
				pCalcData->Alloc_pptLine( pCalcData->m_nPoints);
			}
			else
			{
				nDTI_Start = pCalcData->nDTI_Start_LastPixel;
				nIndex = pCalcData->nIndex_LastPixel;
			}

			pos = pDataList->FindIndex( nDTI_Start);

			int nZeroDTI = 0;
			CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
			if( !pblaAreafDTI) return false;
			CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
			pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			POINT *pptLine = pCalcData->Get_pptLine();

			// (2009/4/22 - Seung-Won, Bae) Check NULL
			if( !pptLine) return false;

			int nPreX = INT_MIN;
			CXScaleManager::PAREA parea;
			for( nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
			{
				parea = pblaAreafDTI->GetNext( dblCursor);

				//>> CCalcDataForDraw Init - vntsorl(20091208)
//				if(parea == NULL) break;
//				if(pos == NULL) break;
				if(parea == NULL || pos == NULL)
				{
					if(nDTI == nDTI_Start)
					{
						// (2009/1/28 - Seung-Won, Bae) for mode 1.
						pCalcData->nDTI_Start_LastPixel = nDTI;
						pCalcData->nIndex_LastPixel		= nIndex;
					}
					break;
				}
				//<< CCalcDataForDraw Init - vntsorl(20091208)

				dY = pDataList->GetNext(pos);
				
				if(bLog) dY = CMath::Log(dY);

				dViewY = dViewBottom - (dViewHeight * ((dY - dRealMin) / dRealHeight));				
				nViewY = ROUND(dViewY);

				if(bInvert) nViewY = nViewY_half2 - nViewY;

				if( nPreX == parea->center)
				{
					if( pptLine[ nIndex + 1].y < nViewY) pptLine[ nIndex + 1].y = nViewY;
					if( nViewY < pptLine[ nIndex + 2].y) pptLine[ nIndex + 2].y = nViewY;
					pptLine[ nIndex + 3].y = nViewY;
				}
				else
				{
					// (2009/1/28 - Seung-Won, Bae) for mode 1.
					pCalcData->nDTI_Start_LastPixel = nDTI;
					pCalcData->nIndex_LastPixel		= nIndex;

					// (2009/2/10 - Seung-Won, Bae) Check Overflow exception.
					if( pCalcData->GetSize_pptLine() < nIndex + 4) break;

					nIndex += 4;
					for( int i = 0; i < 4; i++)
					{
						pptLine[ nIndex + i].x = parea->center;
						pptLine[ nIndex + i].y = nViewY;
					}
					nPreX = parea->center;
				}

				if( nArrowIndex == nDTI)
					pCalcData->nArrowIndex = nIndex + 3;
			}
			// (2009/1/9 - Seung-Won, Bae) Do not draw for none data DTI (Future).
			if( nDTI <= nDTI_End) nDTI_End = nDTI - 1;

			pCalcData->nPtCnt = nIndex + 4;  //Point 개수

			// TRACE( "\r\nPoint %d on %d with X Scale Mode %d\r\n", pCalcData->nPtCnt, pCalcData->m_nPoints, xScaleMode);
		}
		pCalcData->nDTI_Start  = nDTI_Start; //차트 성능 개선 관련 - Data 저장
		pCalcData->nDTI_End    = nDTI_End;
	}
	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{
		if(!subGraphData.m_pGraphCache)
			return TRUE;
		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( subGraphData.m_pGraphCache->nPtCnt < 1) return true;

		CPen linePen;
 		CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
 		CPen* pOldPen = pDC->SelectObject(&linePen); 

		CCalcDataForDraw *pCalcData = subGraphData.m_pGraphCache;
		int nDTI_Start  = pCalcData->nDTI_Start;
		int nDTI_End    = pCalcData->nDTI_End;
		int nArrowIndex = pCalcData->nArrowIndex;
		int nDTI		= 0;
		int nFlag		= pCalcData->nFlag;
		int nViewX, nViewY;
		int nPtCnt		= pCalcData->nPtCnt;

		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		POINT *pptLine = pCalcData->Get_pptLine();

		if( pptLine)
		{
			if(nFlag == 1)  //nDTI_Start == nDTI_End
			{
				pDC->MoveTo( pptLine[0].x, pptLine[0].y);
				pDC->LineTo( pptLine[1].x, pptLine[1].y);
			}
			else if(nFlag == 2) //(nDTI_Start <= nDTI_End) 
			{
				// (2009/1/7 - Seung-Won, Bae) Do not search with loop.
				if( 0 <= nArrowIndex && nArrowIndex < nPtCnt)
				{
					nViewX = pptLine[ nArrowIndex].x;
					nViewY = pptLine[ nArrowIndex].y;
					DrawArrow( pDC, CPoint( nViewX, nViewY), graphColor.GetColor1());
				}
				DrawPolyLine(pDC, pptLine, nPtCnt, penThickness.GetDrawingThickness1());
			}
		}

		pDC->SelectObject(pOldPen);
		linePen.DeleteObject();
	}
	return true;
}

void CLineDraw::DrawArrow(CDC* pDC, const CPoint& point, const COLORREF& color)
{
	//pen을 새로 만든 이유 : 화살표의 테두리두께(pen 굵기)는 항상 "1"이여야 한다.
	CBrush arrowBrush(color);
	CBrush* pOldBrush = pDC->SelectObject(&arrowBrush);
	CPen arrowPen(PS_SOLID, 1, color);
	CPen* pOldPen = pDC->SelectObject(&arrowPen);
	CDraw::DrawArrow(pDC, CPoint(point.x, point.y +2), true);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	arrowPen.DeleteObject();
	arrowBrush.DeleteObject();
}

bool CLineDraw::DrawNonZeroLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)	return false;
	CPacket* pPacket = packetList.GetNext(packetPos);
	
	POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(pos == NULL)	return false;
	
	//팬 선택한다.
	CPen linePen, standardPen;
	CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CreatePen(standardPen, drawStyle, 1, graphColor.GetColor6());
	CPen* pOldPen = pDC->SelectObject(&standardPen);

	// 기준가를 가지고 오는부분. 그려줌.===================================================================================
	///이부분은 데이터가 1개일 때를 고려할 필요가 없다.
	CPoint standardPt(0, 0);
	if(pIndicatorInfo != NULL){
		CList<double, double> * pBaseLineList = pIndicatorInfo->GetBaseLineList();
		if(pBaseLineList != NULL){
			POSITION basePos = pBaseLineList->GetHeadPosition();
			if(basePos != NULL){
				double dStandard = pBaseLineList->GetNext(basePos);
				standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
				if(bInvert == true)	
					standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

				if(dStandard >= dViewMin && dStandard <= dViewMax)
					CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
				for(int base = 2; base <= pBaseLineList->GetCount(); base++){
					dStandard = pBaseLineList->GetNext(basePos);
					standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
					if(bInvert == true)	
						standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
					if(dStandard >= dViewMin && dStandard <= dViewMax)
						CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
				}
			}
		}
	}
	pDC->SelectObject(&linePen);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	CPoint prePt(0, 0), curPt(0, 0);
	double dPreValue = 0.0, dCurValue = 0.0;
	dPreValue = pPacket->GetnumericDataList()->GetAt(pos);

	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dPreValue);
	if(bInvert == true)
		prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos == NULL)
			break;
		dCurValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));

		curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dCurValue);
		if(bInvert == true)
			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

		if(i >= pPacket->GetStartPeriod()){
			if(dPreValue != 0 && dCurValue != 0){
				if(nRealStart == nRealEnd)	///김찬중...데이터가 한개일 때
					CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
				else if(i >= pPacket->GetStartPeriod())
					CDraw::DrawLine(pDC, prePt, curPt);
			}
			/// 한투의 요구에 의해 삽입 : 마지막 앞의 데이터는 모두 0이고 마지막 데이터만 0이 아닐 경우 마지막 포인트만 수평선을 긋는다.
			else if(i == nRealEnd && dPreValue == 0 && dCurValue != 0)	
				CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
		}
		
		dPreValue = dCurValue;
		prePt = curPt;
	}
	
	pDC->SelectObject(pOldPen);
	linePen.DeleteObject();
	
	return true;
}
bool CLineDraw::DrawNonZeroLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,	const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		POSITION packetPos = packetList.GetHeadPosition();
		if(packetPos == NULL)	return false;
		CPacket* pPacket = packetList.GetNext(packetPos);
			
		int nDTI, nDTI_Start, nDTI_End;
		
		BOOL bOEBT;
		if(pIndicatorInfo == NULL) bOEBT = FALSE;
		else					   bOEBT = (pIndicatorInfo->GetIndicatorName() == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART));

		pxScaleManager->GetDrawIndex(strRQ, FALSE, bOEBT, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

	//	POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);
		POSITION pos = pPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		if(pos == NULL)	return false;
		
		//팬 선택한다.
		CPen linePen, standardPen;
		CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		CreatePen(standardPen, drawStyle, 1, graphColor.GetColor6());
		CPen* pOldPen = pDC->SelectObject(&standardPen);

		// 기준가를 가지고 오는부분. 그려줌.===================================================================================
		///이부분은 데이터가 1개일 때를 고려할 필요가 없다.
		CPoint standardPt(0, 0);
		if(pIndicatorInfo != NULL)
		{
			CList<double, double> * pBaseLineList = pIndicatorInfo->GetBaseLineList();
			if(pBaseLineList != NULL)
			{
				POSITION basePos = pBaseLineList->GetHeadPosition();
				if(basePos != NULL)
				{
					double dStandard = pBaseLineList->GetNext(basePos);
					standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
					if(bInvert == true)	
						standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

					if(dStandard >= dViewMin && dStandard <= dViewMax)
						CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
					for(int base = 2; base <= pBaseLineList->GetCount(); base++)
					{
						dStandard = pBaseLineList->GetNext(basePos);
						standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
						if(bInvert == true)	
							standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
						if(dStandard >= dViewMin && dStandard <= dViewMax)
							CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
					}
				}
			}
		}
		pDC->SelectObject(&linePen);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dViewY1;//, dViewY2;
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
		double dValue1 = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));
		double dRealHeight = dRealMax - dRealMin;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->GetAt_DrawArea(nDTI_Start,&parea);
		parea = pblaAreafDTI->GetAt( dblCursor);

		if(parea == NULL) return false;

		
		CPoint prePt(0, 0), curPt(0, 0);
	//	double dPreValue = 0.0, dCurValue = 0.0;
	//	dPreValue = pPacket->GetnumericDataList()->GetAt(pos);
		dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
		dViewY1		= ROUND(dViewY1);
	//	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dPreValue);

		if(bInvert) 
			dViewY1 = nViewY_half2 - dViewY1;
	//	if(bInvert == true)
	//		prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

		prePt.x = parea->center,prePt.y = dViewY1;

	//	for(int i = nRealStart; i <= nRealEnd; i++)
		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);

			if(pos == NULL || parea == NULL ) break;		
	//		if(pos == NULL)
	//			break;
	//		dCurValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));
			dValue1 = pPacket->GetnumericDataList()->GetNext(pos);
			if(bLog)
				dValue1 = CMath::Log(dValue1);

	//		curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dCurValue);
	//		if(bInvert) {
	//			dViewY1 = nViewY_half2 - dViewY1;
	//			dViewY2 = nViewY_half2 - dViewY2;
	//		}
			dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
			dViewY1		= ROUND(dViewY1);

	//		if(bInvert == true)
	//			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);
			if(bInvert)
				dViewY1 = nViewY_half2 - dViewY1;

			curPt.x = parea->center,curPt.y = dViewY1;

			if(nDTI >= pPacket->GetStartPeriod())
			{
				if(dValue1 != 0)
				{
					if(nDTI_Start == nDTI_End)	///김찬중...데이터가 한개일 때
						CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
					else if(nDTI >= pPacket->GetStartPeriod())
						CDraw::DrawLine(pDC, prePt, curPt);
				}
				/// 한투의 요구에 의해 삽입 : 마지막 앞의 데이터는 모두 0이고 마지막 데이터만 0이 아닐 경우 마지막 포인트만 수평선을 긋는다.
	//			else if(i == nRealEnd && dPreValue == 0 && dValue1 != 0)	
				else if(nDTI == nDTI_End && dValue1 != 0)	
					CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
			}
			
	//		dPreValue = dCurValue;
			prePt = curPt;
		}
		
		pDC->SelectObject(pOldPen);
		linePen.DeleteObject();
		standardPen.DeleteObject();
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	
	return true;
}


bool CLineDraw::DrawFillLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)	return false;

	CPacket* pPacket = packetList.GetNext(packetPos);
	if(pPacket == NULL)	return false;
	
	POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(pos == NULL)	return false;

	//팬과 브러시를 선택한다.
	CBrush upBrush	(graphColor.GetColor2());
	CBrush downBrush(graphColor.GetColor4());
	CBrush bohapBrush(graphColor.GetColor6());
	
	CPen upPen, downPen, upFillPen, downFillPen, bohapPen, standardPen; // 기준선 그리는 pen

	CreatePen(upPen,		drawStyle, penThickness.GetDrawingThickness1(),	graphColor.GetColor1());
	CreatePen(upFillPen,	drawStyle, penThickness.GetDrawingThickness1(),	graphColor.GetColor2());
	CreatePen(downPen,		drawStyle, penThickness.GetDrawingThickness2(),	graphColor.GetColor3());
	CreatePen(downFillPen,	drawStyle, penThickness.GetDrawingThickness2(),	graphColor.GetColor4());
	CreatePen(bohapPen,		drawStyle, penThickness.GetDrawingThickness3(),	graphColor.GetColor5());
	CreatePen(standardPen,	drawStyle, 1,									graphColor.GetColor6());
	
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&bohapBrush);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&standardPen);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	// 기준가를 가지고 오는 부분.
	double dStandard = 0.0;   // 임이의 기준가~~~
	double dStandardMin = 0.0;
	double dStandardMax = 0.0;
	CPoint standardPt(0, 0);
	if(pIndicatorInfo == NULL){
		dStandard = dStandardMin = dStandardMax = pPacket->GetStandardValue();
		// 기준선이 min 보다 더 작을 경우 -> min 값으로 바꾼다.
		// 기준선이 max 보다 더 클 경우 -> max 값으로 바꾼다.
		if(dStandardMin < dRealMin)
			dStandard = dStandardMin = dStandardMax = dRealMin;
		else if(dStandardMax > dRealMax)
			dStandard = dStandardMin = dStandardMax = dRealMax;

		standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandard);
		if(bInvert == true)
			standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

		if(dStandard >= dRealMin && dStandard <= dRealMax)
			CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
	}
	else
		GetStandardValue(pDC, pPacket, pIndicatorInfo, dRealMin, dRealMax, dViewMin, dViewMax, dRealWidth, bInvert, graphRegion, dStandardMin, dStandardMax);

	CPoint standardMinPt, standardMinPt_Draw, standardMaxPt, standardMaxPt_Draw, prePt, prePt_Draw;
	standardMinPt_Draw = standardMinPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandardMin);
	standardMaxPt_Draw = standardMaxPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandardMax);

	double dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));
	prePt_Draw = prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue);
	if(bInvert == true)
	{
		prePt_Draw.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt_Draw.y);
		standardMinPt_Draw.y = CDataConversion::InvertGraphDataPt(graphRegion, standardMinPt_Draw.y);
		standardMaxPt_Draw.y = CDataConversion::InvertGraphDataPt(graphRegion, standardMaxPt_Draw.y);
	}

	int nStartPeriod = pPacket->GetStartPeriod();
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos == NULL)	break;

		dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));
		CPoint curPt, curPt_Draw;
		curPt_Draw = curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue);
		if(bInvert)
		{
			curPt_Draw.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt_Draw.y);
		}

		CPoint calDataPtEnd(0, 0);
		if(i >= nStartPeriod)
		{
			pDC->SelectObject(&bohapPen);
			//if(nRealStart==nRealEnd)	///김찬중...데이터가 한개일 때도 그린다
			//	CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
	
			if(i >= nStartPeriod + 1)
				CDraw::DrawLine(pDC, prePt_Draw, curPt_Draw);
		}
				
		if(prePt.y <= standardMaxPt.y && curPt.y <= standardMaxPt.y)
		{
			CPoint fourPoint[4];	
			InsertFourPointToArray(fourPoint, prePt_Draw, curPt_Draw, CPoint(curPt_Draw.x, standardMaxPt_Draw.y), CPoint(prePt_Draw.x, standardMaxPt_Draw.y));
			pDC->SelectObject(&upBrush);
			pDC->SelectObject(&upFillPen);
			if(graphColor.GetUpGraphFill() && i >= nStartPeriod)
				pDC->Polygon(fourPoint,4);

			if(i >= nStartPeriod)
			{
				//pDC->SelectObject(&upPen);
				pDC->SelectObject(&bohapPen);
				///여기가 테두리 그리는 부분
				CDraw::DrawLine(pDC, prePt_Draw, curPt_Draw);
			}
		}
		else if(prePt.y >= standardMinPt.y && curPt.y >= standardMinPt.y)
		{
			CPoint fourPoint[4];
			InsertFourPointToArray(fourPoint, CPoint(prePt_Draw.x, standardMinPt_Draw.y), CPoint(curPt_Draw.x, standardMinPt_Draw.y), curPt_Draw, prePt_Draw);
			pDC->SelectObject(&downBrush);
			pDC->SelectObject(&downFillPen);
			if(graphColor.GetDownGraphFill() && i >= nStartPeriod)
				pDC->Polygon(fourPoint,4);	///기준선 이하 칠하는 부분

			if(i >= nStartPeriod)
			{
				//pDC->SelectObject(&downPen);
				pDC->SelectObject(&bohapPen);
				CDraw::DrawLine(pDC, prePt_Draw, curPt_Draw);
			}
		}
		if(prePt.y < standardMaxPt.y && curPt.y > standardMaxPt.y && curPt.y < standardMinPt.y)
		{
			CPoint startPt = CPoint(prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMaxPt.y), CPoint(curPt.x, standardMaxPt.y)), standardMaxPt.y);				
			if(bInvert)
				startPt.y = CDataConversion::InvertGraphDataPt(graphRegion, startPt.y);
			CPoint threePoint[3];
			InsertThreePointToArray(threePoint, prePt_Draw, CPoint(startPt.x, standardMaxPt_Draw.y), CPoint(prePt_Draw.x, standardMaxPt_Draw.y));
			pDC->SelectObject(&upBrush);
			pDC->SelectObject(&upFillPen);
			if(graphColor.GetUpGraphFill() && i >= nStartPeriod)
				pDC->Polygon(threePoint,3);

			if(i >= nStartPeriod)
			{	
				//pDC->SelectObject(&upPen);
				pDC->SelectObject(&bohapPen);
				CDraw::DrawLine(pDC, prePt_Draw, startPt);
			}
		}
		if(prePt.y > standardMaxPt.y && curPt.y < standardMaxPt.y && prePt.y < standardMinPt.y)
		{

			CPoint startPt = CPoint(prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMaxPt.y), CPoint(curPt.x, standardMaxPt.y)), standardMaxPt.y);
			if(bInvert)
				startPt.y = CDataConversion::InvertGraphDataPt(graphRegion, startPt.y);
			CPoint threePoint[3];
			InsertThreePointToArray(threePoint, CPoint(startPt.x, standardMaxPt_Draw.y), curPt_Draw, CPoint(curPt_Draw.x, standardMaxPt_Draw.y));
			pDC->SelectObject(&upBrush);
			pDC->SelectObject(&upFillPen);
			if(graphColor.GetUpGraphFill() && i >= nStartPeriod)
				pDC->Polygon(threePoint,3);
			
			if(i >= nStartPeriod)
			{
				//pDC->SelectObject(&upPen);
				pDC->SelectObject(&bohapPen);
				CDraw::DrawLine(pDC, startPt, curPt_Draw);
			}
		}

		if(prePt.y < standardMinPt.y && curPt.y > standardMinPt.y && prePt.y > standardMaxPt.y)
		{
			CPoint startPt = CPoint(prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMinPt.y), CPoint(curPt.x, standardMinPt.y)), standardMinPt.y);
			if(bInvert)
				startPt.y = CDataConversion::InvertGraphDataPt(graphRegion, startPt.y);
			CPoint threePoint[3];
			InsertThreePointToArray(threePoint, CPoint(startPt.x, standardMinPt_Draw.y), curPt_Draw, CPoint(curPt_Draw.x, standardMinPt_Draw.y));
			pDC->SelectObject(&downBrush);
			pDC->SelectObject(&downFillPen);
			if(graphColor.GetUpGraphFill() && i >= nStartPeriod)
				pDC->Polygon(threePoint,3);
			
			if(i >= nStartPeriod)
			{	
				//pDC->SelectObject(&downPen);
				pDC->SelectObject(&bohapPen);
				CDraw::DrawLine(pDC, startPt, curPt_Draw);
			}
		}
		if(prePt.y > standardMinPt.y && curPt.y < standardMinPt.y && curPt.y > standardMaxPt.y)
		{

			CPoint startPt = CPoint(prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMinPt.y), CPoint(curPt.x, standardMinPt.y)), standardMinPt.y);
			if(bInvert)
				startPt.y = CDataConversion::InvertGraphDataPt(graphRegion, startPt.y);
			CPoint threePoint[3];
			InsertThreePointToArray(threePoint, prePt_Draw, CPoint(startPt.x, standardMinPt_Draw.y), CPoint(prePt_Draw.x, standardMinPt_Draw.y));
			pDC->SelectObject(&downBrush);
			pDC->SelectObject(&downFillPen);
			if(graphColor.GetUpGraphFill() && i >= nStartPeriod)
				pDC->Polygon(threePoint,3);
			
			if(i >= nStartPeriod)
			{	
				//pDC->SelectObject(&downPen);
				pDC->SelectObject(&bohapPen);
				CDraw::DrawLine(pDC, prePt_Draw, startPt);
			}
		}
		if(prePt.y < standardMaxPt.y && curPt.y > standardMinPt.y)
		{
			CPoint startPt = CPoint(prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMaxPt.y), CPoint(curPt.x, standardMaxPt.y)));
			CPoint calDataPtEnd = CPoint(startPt.x + CalculateDeclination(CPoint(startPt.x, standardMaxPt.y), curPt, CPoint(startPt.x, standardMinPt.y), CPoint(curPt.x, standardMinPt.y)));
			if(bInvert)
			{
				startPt.y = CDataConversion::InvertGraphDataPt(graphRegion, startPt.y);
				calDataPtEnd.y = CDataConversion::InvertGraphDataPt(graphRegion, calDataPtEnd.y);
			}
			CPoint threePoint[3];
			InsertThreePointToArray(threePoint, prePt_Draw, CPoint(startPt.x, standardMaxPt_Draw.y), CPoint(prePt_Draw.x, standardMaxPt_Draw.y));
			pDC->SelectObject(&upBrush);
			pDC->SelectObject(&upPen);
			if(graphColor.GetUpGraphFill() && i >= nStartPeriod)
				pDC->Polygon(threePoint, 3);

			InsertThreePointToArray(threePoint, CPoint(calDataPtEnd.x, standardMinPt_Draw.y), curPt_Draw, CPoint(curPt_Draw.x, standardMinPt_Draw.y));
			pDC->SelectObject(&downBrush);
			pDC->SelectObject(&downPen);
			if(graphColor.GetDownGraphFill() && i >= nStartPeriod)
				pDC->Polygon(threePoint, 3);
		}
		if(prePt.y > standardMinPt.y && curPt.y < standardMaxPt.y)
		{
			CPoint startPt = CPoint(prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMinPt.y), CPoint(curPt.x, standardMinPt.y)));
			CPoint calDataPtEnd = CPoint(startPt.x + CalculateDeclination(CPoint(startPt.x, standardMinPt.y), curPt, CPoint(startPt.x, standardMaxPt.y), CPoint(curPt.x, standardMaxPt.y)));
			if(bInvert)
			{
				startPt.y = CDataConversion::InvertGraphDataPt(graphRegion, startPt.y);
				calDataPtEnd.y = CDataConversion::InvertGraphDataPt(graphRegion, calDataPtEnd.y);
			}
			CPoint fourPoint[4], threePoint[3];
			InsertThreePointToArray(threePoint, prePt_Draw, CPoint(startPt.x, standardMinPt_Draw.y), CPoint(prePt_Draw.x, standardMinPt_Draw.y));

			pDC->SelectObject(&downBrush);
			pDC->SelectObject(&downPen);
			if(graphColor.GetUpGraphFill() && i >= nStartPeriod)
				pDC->Polygon(threePoint, 3);

			InsertThreePointToArray(threePoint, CPoint(calDataPtEnd.x, standardMaxPt_Draw.y), curPt_Draw, CPoint(curPt_Draw.x, standardMaxPt_Draw.y));
			pDC->SelectObject(&upBrush);
			pDC->SelectObject(&upPen);
			if(graphColor.GetDownGraphFill() && i >= nStartPeriod)
				pDC->Polygon(threePoint, 3);
		}

		prePt = curPt;
		prePt_Draw = curPt_Draw;
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	
	upBrush.DeleteObject();
	downBrush.DeleteObject();
	bohapBrush.DeleteObject();

	bohapPen.DeleteObject();
	upPen.DeleteObject();
	upFillPen.DeleteObject();
	downFillPen.DeleteObject();
	downPen.DeleteObject();
	standardPen.DeleteObject();
	return true;

}

bool CLineDraw::DrawFillLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,const CString& strRQ, CXScaleManager* pxScaleManager,
		CSubGraphData& subGraphData, int p_nColumn)
{
	if( pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		// 1. Get the cache size.
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		//		Alloc max memory.
		int nDataCount = pxScaleManager->GetColumnWidth( p_nColumn);
		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( nDataCount < 1) return true;

		// 2. Check X Scale Mode.
		// the change of Graph Style, Y Min/Max is xScaleMode 2 (Don't ask X Scale Manager !)
		int xScaleMode = 2;
		if(	subGraphData.m_pGraphCache)
		{	
			//Check YValue
			bool bYValue = subGraphData.m_pGraphCache->CheckYValue( dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);
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
			subGraphData.m_pGraphCache->Alloc_pptLine( nDataCount * 4);				// Open, High, Low, Close for every pixel.
			subGraphData.m_pGraphCache->Alloc_lpPoint_High( nDataCount * 2.5);			// Up and Down bound for every pixel.
			subGraphData.m_pGraphCache->Alloc_lpPolyCount_High( nDataCount);	// One polycount for every pixel.
			subGraphData.m_pGraphCache->Alloc_lpPoint_Low( nDataCount * 2.5);				// Up and Down bound for every pixel.
			subGraphData.m_pGraphCache->Alloc_lpPolyCount_Low( nDataCount);		// One polycount for every pixel.
			subGraphData.m_pGraphCache->Alloc_lpPolyCount_Bohap( 20);				// for guide line. max 20.

TRACE( "\r\n2009.02.27 Memory is created with [%d]\r\n", nDataCount);
		}
		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		//YValue Set
		pCalcData->SetYValue( dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);

		double dRealMin = CDraw::CalLogValue( bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue( bLog, dViewMax);

		double dStandardMin = DBL_MAX;
		double dStandardMax = DBL_MIN;
		pCalcData->nCount_Bohap = 0;	// Base line count.
		CList<double, double> * pBaseLineList = NULL;
		if( pIndicatorInfo) pBaseLineList = pIndicatorInfo->GetBaseLineList();
		POSITION basePos = NULL;
		if( pBaseLineList) basePos = pBaseLineList->GetHeadPosition();
		int *lpPolyCount_Bohap = pCalcData->Get_lpPolyCount_Bohap();
		if( !basePos)
		{
			if( !packetList.IsEmpty())
			{
				CPacket* pPacket = packetList.GetHead();
				if( pPacket)
				{
					double dStandard = pPacket->GetStandardValue();
					if( -HUGE_VAL != dStandard)
					{
						dStandardMin = dStandardMax = dStandard;
						if( dRealMin <= dStandard && dStandard <= dRealMax)
						{
							CPoint standardPt = CDataConversion::GetVerticalPoint( graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandard);
							if( bInvert) standardPt.y = CDataConversion::InvertGraphDataPt( graphRegion, standardPt.y);
							lpPolyCount_Bohap[ 0] = standardPt.y;
							pCalcData->nCount_Bohap = 1;
						}
					}
				}
			}
		}
		else
		{
			double dStandard = 0;
			while( basePos)
			{
				dStandard = pBaseLineList->GetNext(basePos);
				if( dStandard >= dViewMin && dStandard <= dViewMax)
				{
					CPoint standardPt = CDataConversion::GetVerticalPoint( graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandard);
					if( bInvert) standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
					lpPolyCount_Bohap[ pCalcData->nCount_Bohap] = standardPt.y;
					pCalcData->nCount_Bohap++;
				}
				if( dStandard < dStandardMin) dStandardMin = dStandard;
				if( dStandardMax < dStandard) dStandardMax = dStandard;
			}
		}
		CPoint standardMinPt = CDataConversion::GetVerticalPoint( graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandardMin);
		CPoint standardMaxPt = CDataConversion::GetVerticalPoint( graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandardMax);
		if( bInvert)
		{
			int nTempMax = CDataConversion::InvertGraphDataPt( graphRegion, standardMinPt.y);
			standardMinPt.y = CDataConversion::InvertGraphDataPt( graphRegion, standardMaxPt.y);
			standardMaxPt.y = nTempMax;
		}

		int nDTI_Start = -1, nDTI_End = -1;
		pxScaleManager->GetDrawIndex( strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if( nDTI_Start == -1) return true;

		BOOL bLineBar = ( graphRegion.Width() < nDTI_End - nDTI_Start && !pxScaleManager->GetWeightedVolume());

		if( packetList.IsEmpty()) return false;
		CPacket* pPacket = packetList.GetHead();
		if( !pPacket) return false;
		CList<double, double>* pValueList = pPacket->GetnumericDataList();
		if( !pValueList) return true;

		LPPOINT	pptLine				= pCalcData->Get_pptLine();
		int *	pnPtCnt				= &pCalcData->nPtCnt;
		int *	pnCount_High		= &pCalcData->nCount_High;
		LPPOINT	lpPoint_High		= pCalcData->Get_lpPoint_High();
		int *	lpPolyCount_High	= pCalcData->Get_lpPolyCount_High();
		int *	pnIdxPoint_High		= &pCalcData->nLastPolyCnt_high;
		int *	pnCount_Low			= &pCalcData->nCount_Low;
		LPPOINT	lpPoint_Low			= pCalcData->Get_lpPoint_Low();
		int *	lpPolyCount_Low		= pCalcData->Get_lpPolyCount_Low();
		int *	pnIdxPoint_Low		= &pCalcData->nLastPolyCnt_Low;
		if( bInvert)
		{
			pnCount_High		= &pCalcData->nCount_Low;
			lpPoint_High		= pCalcData->Get_lpPoint_Low();
			lpPolyCount_High	= pCalcData->Get_lpPolyCount_Low();
			pnIdxPoint_High		= &pCalcData->nLastPolyCnt_Low;
			pnCount_Low			= &pCalcData->nCount_High;
			lpPoint_Low			= pCalcData->Get_lpPoint_High();
			lpPolyCount_Low		= pCalcData->Get_lpPolyCount_High();
			pnIdxPoint_Low		= &pCalcData->nLastPolyCnt_high;	
		}

		// for X Scale Mode 1
		if( xScaleMode == 1)
		{
			nDTI_Start	= pCalcData->nDTI_Start;
		}

		POSITION posValue = pValueList->FindIndex( nDTI_Start);
		if( !posValue) return false;
		double dValue = CDraw::CalLogValue( bLog, pValueList->GetAt( posValue));
		CPoint prePt = CDataConversion::GetVerticalPoint( graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue);
		if( bInvert) prePt.y = CDataConversion::InvertGraphDataPt( graphRegion, prePt.y);

		// for X Scale Mode 1
		if( xScaleMode == 1)
		{
			nDTI_Start	= pCalcData->nDTI_Start;
			prePt		= pCalcData->m_Prept;
		}

		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);
		CXScaleManager::PAREA parea;

		BOOL bXReverse = pxScaleManager->GetReverse();

		int nX = -1;
		int nStartPeriod = pPacket->GetStartPeriod();
		for( int i = nDTI_Start; i <= nDTI_End; i++)
		{
			parea = pblaAreafDTI->GetNext( dblCursor);
			if( !parea || !posValue)	break;

			// (2009/2/27 - Seung-Won, Bae) Backup for XScaleMode 1
			if( nX != parea->center)
			{
				nX = parea->center;
				pCalcData->nDTI_Start = i;
				pCalcData->m_Prept = prePt;
			}

			dValue = CDraw::CalLogValue( bLog, pValueList->GetNext( posValue));
			if( i < nStartPeriod) continue;
			if( xScaleMode != 1 && ( i == nDTI_Start || i == nStartPeriod))
				prePt.x = parea->center;

			CPoint curPt = CDataConversion::GetVerticalPoint( graphRegion, i - nRealStart, dRealWidth, dRealMin, dRealMax, dValue);
			if( bInvert) curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);
			curPt.x = parea->center;

			// 1. Add Boder Line.
			BOOL bNewPixel = ( *pnPtCnt == 0);
			if( !bNewPixel) bNewPixel = ( pptLine[ *pnPtCnt - 1].x != curPt.x);
			if( bNewPixel)
			{
				pptLine[ *pnPtCnt] = pptLine[ *pnPtCnt + 1] = pptLine[ *pnPtCnt + 2] = pptLine[ *pnPtCnt + 3] = curPt;
				( *pnPtCnt) += 4;
			}
			else
			{
				if( curPt.y < pptLine[ *pnPtCnt - 3].y) pptLine[ *pnPtCnt - 3].y = curPt.y;
				if( pptLine[ *pnPtCnt - 2].y < curPt.y) pptLine[ *pnPtCnt - 2].y = curPt.y;
				pptLine[ *pnPtCnt - 1].y = curPt.y;
				
				if( !bLineBar)
				{
					pptLine[ *pnPtCnt - 4].y = pptLine[ *pnPtCnt - 3].y = pptLine[ *pnPtCnt - 2].y = curPt.y;

					// (2009/10/9 - Seung-Won, Bae) Remove Prev Area.
					if( 0 < pCalcData->m_nS_High)
					{
						POINT ptEnd = lpPoint_High[ *pnIdxPoint_High - 1];
						if( 1 < pCalcData->m_nS_High)
						{
							( *pnIdxPoint_High) -= lpPolyCount_High[ *pnCount_High - 1];
							( *pnCount_High)--;
						}
						else
						{
							( *pnIdxPoint_High) -= 3;
							lpPolyCount_High[ *pnCount_High - 1] -= 3;
							lpPoint_High[ *pnIdxPoint_High].x = lpPoint_High[ *pnIdxPoint_High - 1].x;
							lpPoint_High[ *pnIdxPoint_High].y = ptEnd.y;
							lpPoint_High[ *pnIdxPoint_High + 1] = ptEnd;
							( *pnIdxPoint_High) += 2;
							lpPolyCount_High[ *pnCount_High - 1] += 2;
						}
					}
					if( 0 < pCalcData->m_nS_Low)
					{
						POINT ptEnd = lpPoint_Low[ *pnIdxPoint_Low - 1];
						if( 1 < pCalcData->m_nS_Low)
						{
							( *pnIdxPoint_Low) -= lpPolyCount_Low[ *pnCount_Low - 1];
							( *pnCount_Low)--;
						}
						else
						{
							( *pnIdxPoint_Low) -= 3;
							lpPolyCount_Low[ *pnCount_Low - 1] -= 3;
							lpPoint_Low[ *pnIdxPoint_Low].x = lpPoint_Low[ *pnIdxPoint_Low - 1].x;
							lpPoint_Low[ *pnIdxPoint_Low].y = ptEnd.y;
							lpPoint_Low[ *pnIdxPoint_Low + 1] = ptEnd;
							( *pnIdxPoint_Low) += 2;
							lpPolyCount_Low[ *pnCount_Low - 1] += 2;
						}
					}
				}
			}

			pCalcData->m_nS_High = 0;
			pCalcData->m_nS_Low = 0;

			// 2. Check no Up/Down Area.
			if( ( standardMaxPt.y < prePt.y && prePt.y < standardMinPt.y)
				&& ( standardMaxPt.y < curPt.y && curPt.y < standardMinPt.y))
			{
				prePt = curPt;
				continue;
			}

			// 3. Check Up Area
			if( graphColor.GetUpGraphFill() && ( prePt.y < standardMaxPt.y || curPt.y < standardMaxPt.y))
			{
				// 3.1 Get the Point.
				CPoint ptFrom( prePt), ptTo( curPt);
				if( prePt.y > standardMaxPt.y) ptFrom	= CPoint( prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMaxPt.y), CPoint(curPt.x, standardMaxPt.y)), standardMaxPt.y);
				if( curPt.y > standardMaxPt.y) ptTo		= CPoint( prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMaxPt.y), CPoint(curPt.x, standardMaxPt.y)), standardMaxPt.y);

				// 3.2. Add Up Area
				bNewPixel = ( *pnIdxPoint_High == 0);
				if( !bNewPixel) bNewPixel = ( ( !bXReverse && lpPoint_High[ *pnIdxPoint_High - 2].x + 1 < ptFrom.x)
												|| ( bXReverse && ptFrom.x + 1 < lpPoint_High[ *pnIdxPoint_High - 2].x));
				if( bNewPixel)
				{
					lpPoint_High[ *pnIdxPoint_High].x = lpPoint_High[ *pnIdxPoint_High + 1].x = lpPoint_High[ *pnIdxPoint_High + 4].x = ptFrom.x;
					lpPoint_High[ *pnIdxPoint_High + 2].x = lpPoint_High[ *pnIdxPoint_High + 3].x = ptTo.x;
					lpPoint_High[ *pnIdxPoint_High].y = lpPoint_High[ *pnIdxPoint_High + 3].y = lpPoint_High[ *pnIdxPoint_High + 4].y = standardMaxPt.y;
					lpPoint_High[ *pnIdxPoint_High + 1].y = ptFrom.y;
					lpPoint_High[ *pnIdxPoint_High + 2].y = ptTo.y;
					( *pnIdxPoint_High) += 5;
					lpPolyCount_High[ *pnCount_High] = 5;
					( *pnCount_High)++;

					pCalcData->m_nS_High = 2;
				}
				else
				{
					POINT ptEnd = lpPoint_High[ *pnIdxPoint_High - 1];
					( *pnIdxPoint_High) -= 2;
					lpPolyCount_High[ *pnCount_High - 1] -= 2;
					if( lpPoint_High[ *pnIdxPoint_High - 1].x == ptFrom.x)
					{
						if( ptFrom.y < lpPoint_High[ *pnIdxPoint_High - 1].y) lpPoint_High[ *pnIdxPoint_High - 1].y = ptFrom.y;
					}
					else
					{
						lpPoint_High[ *pnIdxPoint_High] = ptFrom;
						( *pnIdxPoint_High)++;
						lpPolyCount_High[ *pnCount_High - 1]++;
					}
					if( lpPoint_High[ *pnIdxPoint_High - 1].x == ptTo.x)
					{
						if( ptTo.y < lpPoint_High[ *pnIdxPoint_High - 1].y) lpPoint_High[ *pnIdxPoint_High - 1].y = ptTo.y;
					}
					else
					{
						lpPoint_High[ *pnIdxPoint_High] = ptTo;
						( *pnIdxPoint_High)++;
						lpPolyCount_High[ *pnCount_High - 1]++;
					}
					lpPoint_High[ *pnIdxPoint_High].x = ptTo.x;
					lpPoint_High[ *pnIdxPoint_High].y = ptEnd.y;
					lpPoint_High[ *pnIdxPoint_High + 1] = ptEnd;
					( *pnIdxPoint_High) += 2;
					lpPolyCount_High[ *pnCount_High - 1] += 2;

					pCalcData->m_nS_High = 1;
				}
			}

			// 4. Check Down Area
			if( graphColor.GetDownGraphFill() && ( prePt.y > standardMinPt.y || curPt.y > standardMinPt.y))
			{
				// 4.1 Get the Point.
				CPoint ptFrom( prePt), ptTo( curPt);
				if( prePt.y < standardMinPt.y) ptFrom	= CPoint(prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMinPt.y), CPoint(curPt.x, standardMinPt.y)), standardMinPt.y);
				if( curPt.y < standardMinPt.y) ptTo		= CPoint(prePt.x + CalculateDeclination(prePt, curPt, CPoint(prePt.x, standardMinPt.y), CPoint(curPt.x, standardMinPt.y)), standardMinPt.y);

				// 4.2. Add Up Area
				bNewPixel = ( *pnIdxPoint_Low == 0);
				if( !bNewPixel) bNewPixel = ( ( !bXReverse && lpPoint_Low[ *pnIdxPoint_Low - 2].x + 1 < ptFrom.x)
												|| ( bXReverse && ptFrom.x + 1 < lpPoint_Low[ *pnIdxPoint_Low - 2].x));
				if( bNewPixel)
				{
					lpPoint_Low[ *pnIdxPoint_Low].x = lpPoint_Low[ *pnIdxPoint_Low + 1].x = lpPoint_Low[ *pnIdxPoint_Low + 4].x = ptFrom.x;
					lpPoint_Low[ *pnIdxPoint_Low + 2].x = lpPoint_Low[ *pnIdxPoint_Low + 3].x = ptTo.x;
					lpPoint_Low[ *pnIdxPoint_Low].y = lpPoint_Low[ *pnIdxPoint_Low + 3].y = lpPoint_Low[ *pnIdxPoint_Low + 4].y = standardMinPt.y;
					lpPoint_Low[ *pnIdxPoint_Low + 1].y = ptFrom.y;
					lpPoint_Low[ *pnIdxPoint_Low + 2].y = ptTo.y;
					( *pnIdxPoint_Low) += 5;
					lpPolyCount_Low[ *pnCount_Low] = 5;
					( *pnCount_Low)++;

					pCalcData->m_nS_Low = 2;
				}
				else
				{
					POINT ptEnd = lpPoint_Low[ *pnIdxPoint_Low - 1];
					( *pnIdxPoint_Low) -= 2;
					lpPolyCount_Low[ *pnCount_Low - 1] -= 2;
					if( lpPoint_Low[ *pnIdxPoint_Low - 1].x == ptFrom.x)
					{
						if( lpPoint_Low[ *pnIdxPoint_Low - 1].y < ptFrom.y) lpPoint_Low[ *pnIdxPoint_Low - 1].y = ptFrom.y;
					}
					else
					{
						lpPoint_Low[ *pnIdxPoint_Low] = ptFrom;
						( *pnIdxPoint_Low)++;
						lpPolyCount_Low[ *pnCount_Low - 1]++;
					}
					if( lpPoint_Low[ *pnIdxPoint_Low - 1].x == ptTo.x)
					{
						if( lpPoint_Low[ *pnIdxPoint_Low - 1].y < ptTo.y) lpPoint_Low[ *pnIdxPoint_Low - 1].y = ptTo.y;
					}
					else
					{
						lpPoint_Low[ *pnIdxPoint_Low] = ptTo;
						( *pnIdxPoint_Low)++;
						lpPolyCount_Low[ *pnCount_Low - 1]++;
					}
					lpPoint_Low[ *pnIdxPoint_Low].x = ptTo.x;
					lpPoint_Low[ *pnIdxPoint_Low].y = ptEnd.y;
					lpPoint_Low[ *pnIdxPoint_Low + 1] = ptEnd;
					( *pnIdxPoint_Low) += 2;
					lpPolyCount_Low[ *pnCount_Low - 1] += 2;

					pCalcData->m_nS_Low = 1;
				}
			}

			prePt = curPt;
		}
	}
	else
	{
		if( !subGraphData.m_pGraphCache) return true;

		CCalcDataForDraw* pCalcData = subGraphData.m_pGraphCache;
		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( pCalcData->nPtCnt < 1) return true;

		CBrush upBrush(		graphColor.GetColor2());
		CBrush downBrush(	graphColor.GetColor4());
		CPen upFillPen, downFillPen, bohapPen; // 기준선 그리는 pen
		CreatePen(upFillPen,	drawStyle, penThickness.GetDrawingThickness1(),	graphColor.GetColor2());
		CreatePen(downFillPen,	drawStyle, penThickness.GetDrawingThickness2(),	graphColor.GetColor4());
		CreatePen(bohapPen,		drawStyle, penThickness.GetDrawingThickness3(),	graphColor.GetColor5());
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject( &upBrush);
		CPen* pOldPen = (CPen*)pDC->SelectObject( &upFillPen);

		// 1. Draw Up Area
		if( graphColor.GetUpGraphFill() && pCalcData->nCount_High > 0)
		{
			pDC->SelectObject( &upBrush);
			pDC->SelectObject( &upFillPen);
			if( 1 < pCalcData->nCount_High)
					pDC->PolyPolygon( pCalcData->Get_lpPoint_High(), pCalcData->Get_lpPolyCount_High(), pCalcData->nCount_High);
			else	pDC->Polygon( pCalcData->Get_lpPoint_High(), *pCalcData->Get_lpPolyCount_High());
		}

		// 2. Draw Down Area
		if( graphColor.GetDownGraphFill() && pCalcData->nCount_Low > 0)
		{
			pDC->SelectObject( &downBrush);
			pDC->SelectObject( &downFillPen);
			if( 1 < pCalcData->nCount_Low)
					pDC->PolyPolygon( pCalcData->Get_lpPoint_Low(), pCalcData->Get_lpPolyCount_Low(), pCalcData->nCount_Low);
			else	pDC->Polygon( pCalcData->Get_lpPoint_Low(), *pCalcData->Get_lpPolyCount_Low());
		}

		// 3. Draw Base Line
		if( 0 < pCalcData->nCount_Bohap)
		{
			CPen standardPen;
			CreatePen( standardPen,	subGraphData.GetBaseLineDrawStyle(), penThickness.GetBaseLineThickness(), graphColor.GetColor6());
			pDC->SelectObject( &standardPen);
			int *lpPolyCount_Bohap = pCalcData->Get_lpPolyCount_Bohap();
			for( int i = 0; i < pCalcData->nCount_Bohap; i++)
				CDraw::DrawLine( pDC, CPoint( graphRegion.left, lpPolyCount_Bohap[ i]), CPoint(graphRegion.right, lpPolyCount_Bohap[ i]));
			standardPen.DeleteObject();
		}
		
		// 4. Draw Graph
		if( 1 < pCalcData->nPtCnt)
		{
			pDC->SelectObject( &bohapPen);
			pDC->Polyline( pCalcData->Get_pptLine(), pCalcData->nPtCnt);
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		
		upBrush.DeleteObject();
		downBrush.DeleteObject();

		upFillPen.DeleteObject();
		downFillPen.DeleteObject();
		bohapPen.DeleteObject();
	}
	return true;
}

//sy test -> 수정해도 될까??
bool CLineDraw::DrawHorizontalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int /*nRealStart*/, const int /*nRealEnd*/, const double& dViewMax, const double& dViewMin, double /*dRealWidth*/, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	CPacket* pPacket = packetList.GetHead();
	if(pPacket == NULL)
		return false;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;
	
	//팬 선택한다.
	CPen linePen;
	CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&linePen);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	POSITION pos = pDataList->GetHeadPosition();
	while(pos != NULL){
		double dValue = CDraw::CalLogValue(bLog, pDataList->GetNext(pos));
		if(dValue < dRealMin || dValue > dRealMax)
			continue;

		int y = CDataConversion::RelativeToAbsolute_Y(graphRegion, dValue, dRealMin, dRealMax);
		if(bInvert == true)
			y = CDataConversion::InvertGraphDataPt(graphRegion, y);

		CDraw::DrawLine(pDC, CPoint(graphRegion.left, y), CPoint(graphRegion.right, y));
	}
	pDC->SelectObject(pOldPen);
	linePen.DeleteObject();
	
	return true;
}

bool CLineDraw::DrawHorizontalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int /*nRealStart*/, const int /*nRealEnd*/, const double& dViewMax, const double& dViewMin, double /*dRealWidth*/, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		CPacket* pPacket = packetList.GetHead();
		if(pPacket == NULL)
			return false;

		int nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		CList<double, double>* pDataList = pPacket->GetnumericDataList();
		if(pDataList == NULL || pDataList->GetCount() <= 0)
			return false;
		
		//팬 선택한다.
		CPen linePen;
		CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		CPen* pOldPen = pDC->SelectObject(&linePen);

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		POSITION pos = pDataList->GetHeadPosition();
		while(pos != NULL)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);

			if(parea == NULL) break;		
			double dValue = CDraw::CalLogValue(bLog, pDataList->GetNext(pos));
			if(dValue < dRealMin || dValue > dRealMax)
				continue;

			int y = CDataConversion::RelativeToAbsolute_Y(graphRegion, dValue, dRealMin, dRealMax);
			if(bInvert == true)
				y = CDataConversion::InvertGraphDataPt(graphRegion, y);

	//		CDraw::DrawLine(pDC, CPoint(graphRegion.left, y), CPoint(graphRegion.right, y));
			CDraw::DrawLine(pDC, CPoint(parea->left, y), CPoint(parea->right, y));
		}
		pDC->SelectObject(pOldPen);
		linePen.DeleteObject();
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	
	return true;
}
/*bool CLineDraw::DrawHorizontalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)	return false;
	CPacket* pPacket = packetList.GetNext(packetPos);
	if(pPacket == NULL)	return false;

	CList<double, double>* pNumList = pPacket->GetnumericDataList();
	if(pNumList == NULL)	return false;
		
	POSITION pos = pNumList->FindIndex(nRealStart);
	
	if(pos == NULL)	return false;
	
	//팬 선택한다.
	CPen linePen;
	CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&linePen);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	CPoint prePt(0, 0), curPt(0, 0);
	double dValue = CDraw::CalLogValue(bLog, pNumList->GetAt(pos));
	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue, false);
	if(bInvert == true)
		prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos == NULL)	break;
		dValue = CDraw::CalLogValue(bLog, pNumList->GetNext(pos));

		curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, dRealMin, dRealMax, dValue, false);
		//line 을 그린다.==========================
		if(bInvert == true)
			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

		///데이터가 1일때 라인을 그리기 위함
		if(i >= pPacket->GetStartPeriod()){
			if(nRealStart == nRealEnd)
				CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
			else if(i >= pPacket->GetStartPeriod())
				CDraw::DrawLine(pDC, CPoint(prePt.x + 1, curPt.y), CPoint(curPt.x, curPt.y));
		}
		prePt = curPt;
	}
	pDC->SelectObject(pOldPen);
	linePen.DeleteObject();
	
	return true;
}*/

bool CLineDraw::DrawVerticalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int /*nRealStart*/, const int /*nRealEnd*/, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool /*bLog*/, bool /*bInvert*/)
{
	CPacket* pPacket = packetList.GetHead();
	if(pPacket == NULL)
		return false;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;
	
	//팬 선택한다.
	CPen linePen;
	CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&linePen);

	// data 를 표현하는 경우 graph 를 draw 할때는 중심부터 그린다.
	// 그러므로 양쪽을 Width/2.0 만큼 줄여줘야 한다.!!
	CRect realGraphRegion = graphRegion;
	realGraphRegion.DeflateRect(CMath::Round(dRealWidth/2.0), 0);
	
	POSITION pos = pDataList->GetHeadPosition();
	while(pos != NULL){
		double dValue = pDataList->GetNext(pos);
		if(dValue < dViewMin || dValue > dViewMax)
			continue;

		int x = CDataConversion::GetHorizontalPoint_X(realGraphRegion, dViewMin, dViewMax, dValue);
		CDraw::DrawLine(pDC, CPoint(x, realGraphRegion.top), CPoint(x, realGraphRegion.bottom));
	}
	pDC->SelectObject(pOldPen);
	linePen.DeleteObject();
	
	return true;
}
//sy end
bool CLineDraw::DrawVerticalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int /*nRealStart*/, const int /*nRealEnd*/, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool /*bLog*/, bool /*bInvert*/,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		CPacket* pPacket = packetList.GetHead();
		if(pPacket == NULL)
			return false;

		CList<double, double>* pDataList = pPacket->GetnumericDataList();
		if(pDataList == NULL || pDataList->GetCount() <= 0)
			return false;
		int nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;
		
		//팬 선택한다.
		CPen linePen;
		CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		CPen* pOldPen = pDC->SelectObject(&linePen);

		// data 를 표현하는 경우 graph 를 draw 할때는 중심부터 그린다.
		// 그러므로 양쪽을 Width/2.0 만큼 줄여줘야 한다.!!
		CRect realGraphRegion = graphRegion;
		realGraphRegion.DeflateRect(CMath::Round(dRealWidth/2.0), 0);

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);
		
		CXScaleManager::PAREA parea;
		POSITION pos = pDataList->GetHeadPosition();
		while(pos != NULL)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);

			if(parea == NULL)	break;
			double dValue = pDataList->GetNext(pos);
			if(dValue < dViewMin || dValue > dViewMax)
				continue;

			//X값을 구하네...X매니저에서 받아서 처리하면 될듯...
			CDraw::DrawLine(pDC, CPoint(parea->center, realGraphRegion.top), CPoint(parea->center, realGraphRegion.bottom));		
			//int x = CDataConversion::GetHorizontalPoint_X(realGraphRegion, dViewMin, dViewMax, dValue);
			//CDraw::DrawLine(pDC, CPoint(x, realGraphRegion.top), CPoint(x, realGraphRegion.bottom));
		}
		pDC->SelectObject(pOldPen);
		linePen.DeleteObject();
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	
	return true;
}
bool CLineDraw::DrawStairLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)	return false;
	CPacket* pPacket = packetList.GetNext(packetPos);
	if(pPacket == NULL)	return false;
	POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(pos == NULL)	return false;
	
	CPen linePen;
	CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&linePen);
	
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	CPoint prePt(0, 0), curPt(0, 0);
	double dPreData = 0.0, dCurData = 0.0;

	dPreData = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));
	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dPreData);

	if(bInvert == true)
			prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos == NULL)	break;
		dCurData = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));
		curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dCurData, false);

		if(bInvert == true)
			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

		if((nRealStart==nRealEnd) && (nRealStart >= pPacket->GetStartPeriod()))
			CDraw::DrawLine(pDC, prePt, curPt);

		if(i >= pPacket->GetStartPeriod()){
			///김찬중...데이터가 한개일 때도 그린다 : nRealStart == nRealEnd
			/// 데이터의 가장 마지막 점에 이르렀을 때는 수평선을 마무리 해줘야 한다.
			if(i==nRealEnd){
///				CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth / 2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth / 2), curPt.y) );
				CPoint endPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, dRealMin, dRealMax, dCurData, false);
				CDraw::DrawLine(pDC, CPoint(curPt.x, curPt.y), CPoint(endPt.x, curPt.y));
			}
			if(i >= pPacket->GetStartPeriod() + 1){
				CDraw::DrawLine(pDC, CPoint(prePt.x, prePt.y), CPoint(curPt.x, prePt.y));
				CDraw::DrawLine(pDC, CPoint(curPt.x, prePt.y), CPoint(curPt.x, curPt.y));
			}
		}
		prePt = curPt;
	}
	pDC->SelectObject(pOldPen);
	linePen.DeleteObject();
	
	return true;
}

bool CLineDraw::DrawStairLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		POSITION packetPos = packetList.GetHeadPosition();
		if(packetPos == NULL)	return false;
		CPacket* pPacket = packetList.GetNext(packetPos);
		if(pPacket == NULL)	return false;

		int nDTI,nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		POSITION pos = pPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		if(pos == NULL)	return false;
		
		CPen linePen;
		CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		CPen* pOldPen = pDC->SelectObject(&linePen);
		
		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dRealHeight = dRealMax - dRealMin;

		CPoint prePt(0, 0), curPt(0, 0);
		double dPreData = 0.0, dCurData = 0.0;
		double dPreData_Y= 0.0,dCurData_Y= 0.0;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->GetAt_DrawArea(nDTI_Start,&parea);
		parea = pblaAreafDTI->GetAt( dblCursor);
		
		if(parea == NULL) return false;

		dPreData = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));
		dPreData_Y  = ROUND(dViewBottom - ( dViewHeight * ((dPreData - dRealMin) / dRealHeight)));
		if(bInvert)
			dPreData_Y = nViewY_half2 - dPreData_Y;
		prePt.x = parea->center,prePt.y = dPreData_Y;
	//	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dPreData);
	//	if(bInvert == true)
	//			prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);

	
			if(pos == NULL||parea == NULL)	break;

			dCurData = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));
			dCurData_Y  = ROUND(dViewBottom - ( dViewHeight * ((dCurData - dRealMin) / dRealHeight)));
			if(bInvert)
				dCurData_Y = nViewY_half2 - dCurData_Y;
	//		curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dCurData, false);
	//		if(bInvert == true)
	//			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

			curPt.x = parea->center,curPt.y = dCurData_Y;
			if((nDTI_Start==nDTI_End) && (nDTI_Start >= pPacket->GetStartPeriod()))
				CDraw::DrawLine(pDC, prePt, curPt);

			if(nDTI >= pPacket->GetStartPeriod()){
				///김찬중...데이터가 한개일 때도 그린다 : nRealStart == nRealEnd
				/// 데이터의 가장 마지막 점에 이르렀을 때는 수평선을 마무리 해줘야 한다.
				if(nDTI==nDTI_End)
				{
	//				CPoint endPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart+1, dRealWidth, dRealMin, dRealMax, dCurData, false);
					CDraw::DrawLine(pDC, CPoint(curPt.x, dCurData_Y), CPoint(parea->center, dCurData_Y));
				}
				if(nDTI >= pPacket->GetStartPeriod() + 1)
				{
					CDraw::DrawLine(pDC, CPoint(prePt.x, prePt.y), CPoint(curPt.x, prePt.y));
					CDraw::DrawLine(pDC, CPoint(curPt.x, prePt.y), CPoint(curPt.x, curPt.y));
				}
			}
			prePt = curPt;
		}
		pDC->SelectObject(pOldPen);
		linePen.DeleteObject();
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	
	return true;
}

bool CLineDraw::DrawCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	//가져올 데이터의 시작 위치를 얻어오는 부분======= 
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)		return false;
	CPacket* pPacket1 = packetList.GetNext(packetPos);
	if(packetPos == NULL)		return false;
	CPacket* pPacket2 = packetList.GetNext(packetPos);
	if(pPacket1 == NULL || pPacket2 == NULL)	return false;
	
	POSITION pos1 = pPacket1->GetnumericDataList()->FindIndex(nRealStart);
	POSITION pos2 = pPacket2->GetnumericDataList()->FindIndex(nRealStart);
	if(pos1 == NULL || pos2 == NULL)	return false;
	
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	CPoint prePt1(0, 0), curPt1(0, 0), prePt2(0, 0), curPt2(0, 0), CrossPt(0, 0);
	double dValue1 = 0.0, dValue2 = 0.0;
	CPen upPen, downPen, *pOldPen = NULL;
	CreatePen(upPen,	drawStyle, 1, graphColor.GetColor1());
	CreatePen(downPen,	drawStyle, 1, graphColor.GetColor3());
	pOldPen = pDC->SelectObject(&upPen);
	
	COLORREF rgb = RGB(0, 0, 0);

	dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetAt(pos1));
	dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetAt(pos2));

	prePt1 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue1);
	prePt2 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue2);
	if(bInvert == true){
		prePt1.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt1.y);
		prePt2.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt2.y);
	}

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos1 == NULL || pos2 == NULL) break;		

		dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetNext(pos1));
		dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetNext(pos2));

		curPt1 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue1);
		curPt2 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue2);

		if(bInvert == true){
			curPt1.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt1.y);
			curPt2.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt2.y);
		}

/////////////////////////////////////////////////////////////////////////////////////////
//경고 : 이부분은 상당히 짜증나는 부분이므로 잘모르면 건드리지 말 것
//몰라도 찬중씨한테 물어보지 마세요 바쁘니깐 ^^
//제목 : 교점의 좌표 구하기//////////////////////////////////////////////////////////////
			
//   사다리꼴 (점0~점3과 점2~점1은 Y축에 평행) : x0과 x3는 같고 x2와 x1은 같다
//		점0----------점2        
//      |            |
//      |            |
//      점3----------점1
//
//   대각선의 교점을 찾는 것이 문제이므로
//	 점(x0, y0)를 원점이라고 가정하면
//   점0~점1간의 직선의 방정식
//      x1-x0
// Y = -------- * X
//      y1-y0
//   점2~점3간의 직선의 방정식
//      x2-x3
// Y = -------- * X + y3-y0
//      y2-y3
//이렇게 나온다. 두식을 연립방정식으로 풀면 
//        y3-y0
// Y = -----------
//          y2-y3   
//     (1- -------)
//          y1-y0
//
// X점은 위의 식을 이용하기 보다는 비례식으로 쉽게 나온다
/////////////////////////////////////////////////////////////////////////////////////////
///		int nHalfWidth = CMath::Round(dRealWidth/2);
		///수직선
		if(i >= pPacket2->GetStartPeriod() && i >= pPacket1->GetStartPeriod()){
			CPoint ptArr[4];
			ptArr[0] = prePt1;
			ptArr[1] = curPt1;
			ptArr[2] = curPt2;
			ptArr[3] = prePt2;

			double x=0, y=0;
			double	x0 = prePt1.x,	y0 = prePt1.y, 
					x1 = curPt1.x,	y1 = curPt1.y, 
					x2 = curPt2.x,	y2 = curPt2.y, 
					x3 = prePt2.x,	y3 = prePt2.y;

			if((prePt1.y >= prePt2.y && curPt1.y <= curPt2.y)
				|| (prePt1.y <= prePt2.y && curPt1.y >= curPt2.y))
			{

				if(y1==y0 || (y2-y3)/(y1-y0)==1)	///에러 처리
					y=0;
				else
					y = (y3-y0)/(1-(y2-y3)/(y1-y0));
				
				if((y3-y0+y1-y2)==0)				///에러처리
					x=0;
				else
					x = (x1-x0)*(y3-y0)/(y3-y0+y1-y2);

				x += x0;
				y += y0;
				ptArr[1].x = ptArr[2].x = CMath::Round(x);
				ptArr[1].y = ptArr[2].y = CMath::Round(y);

				if(prePt1.y > prePt2.y)
					DrawSemiTransparentBitmap(pDC, ptArr, rgb = graphColor.GetColor4());
				else if(prePt1.y < prePt2.y)
					DrawSemiTransparentBitmap(pDC, ptArr, rgb = graphColor.GetColor2());

				ptArr[0].x = ptArr[3].x = CMath::Round(x);
				ptArr[0].y = ptArr[3].y = CMath::Round(y);
				ptArr[1] = curPt1;
				ptArr[2] = curPt2;

				if(curPt1.y > curPt2.y)
					DrawSemiTransparentBitmap(pDC, ptArr, rgb = graphColor.GetColor4());
				else if(curPt1.y < curPt2.y)
					DrawSemiTransparentBitmap(pDC, ptArr, rgb = graphColor.GetColor2());
			}
			else 
			{
				if(curPt1.y >= curPt2.y)
					DrawSemiTransparentBitmap(pDC, ptArr, graphColor.GetColor4());
				else
					DrawSemiTransparentBitmap(pDC, ptArr, graphColor.GetColor2());
			}
		}

		if(graphColor.GetUpGraphFill() == false)	///의미상으로는 맞지 않지만 이 옵션은 라인을 그릴건지 말건지 결정
			pDC->SelectObject(GetStockObject(NULL_PEN));
		else{
			///윗선
			if(i >= pPacket2->GetStartPeriod()){
				pDC->SelectObject(&downPen);
				if(nRealStart == nRealEnd)///김찬중...데이터가 한개일 때도 그린다
					CDraw::DrawLine(pDC, prePt2, curPt2);
				else if(i >= pPacket2->GetStartPeriod())
					CDraw::DrawLine(pDC, prePt2, curPt2);
			}

			///아랫선
			if(i >= pPacket1->GetStartPeriod()){
				pDC->SelectObject(&upPen);
				if(nRealStart == nRealEnd)///김찬중...데이터가 한개일 때도 그린다nEnd
					CDraw::DrawLine(pDC, prePt1, curPt1);
				else if(i >= pPacket1->GetStartPeriod())
					CDraw::DrawLine(pDC, prePt1, curPt1);
			}
		}
		prePt1 = curPt1;
		prePt2 = curPt2;
	}
	pDC->SelectObject(pOldPen);
	upPen.DeleteObject();
	downPen.DeleteObject();
	return true;
}
bool CLineDraw::DrawCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		//가져올 데이터의 시작 위치를 얻어오는 부분======= 
		POSITION packetPos = packetList.GetHeadPosition();
		if(packetPos == NULL)		return false;
		CPacket* pPacket1 = packetList.GetNext(packetPos);
		if(packetPos == NULL)		return false;
		CPacket* pPacket2 = packetList.GetNext(packetPos);
		if(pPacket1 == NULL || pPacket2 == NULL)	return false;
		
		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		POSITION pos1 = pPacket1->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION pos2 = pPacket2->GetnumericDataList()->FindIndex(nDTI_Start);
		if(pos1 == NULL || pos2 == NULL)	return false;
		
		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		CPoint prePt1(0, 0), curPt1(0, 0), prePt2(0, 0), curPt2(0, 0), CrossPt(0, 0);
		double dValue1 = 0.0, dValue2 = 0.0;
		CPen upPen, downPen, *pOldPen = NULL;
		CreatePen(upPen,	drawStyle, 1, graphColor.GetColor1());
		CreatePen(downPen,	drawStyle, 1, graphColor.GetColor3());
		pOldPen = pDC->SelectObject(&upPen);
		
		COLORREF rgb = RGB(0, 0, 0);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dRealHeight = dRealMax - dRealMin;
		double dViewY1, dViewY2;
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dPreData_Y1= 0.0, dPreData_Y2= 0.0;

		dValue1 = pPacket1->GetnumericDataList()->GetAt(pos1);
		dValue2 = pPacket2->GetnumericDataList()->GetAt(pos2);

		dPreData_Y1  = ROUND(dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight)));
		dPreData_Y2  = ROUND(dViewBottom - ( dViewHeight * ((dValue2 - dRealMin) / dRealHeight)));
		if(bInvert == true)
		{
			dPreData_Y1 = nViewY_half2 - dPreData_Y1;
			dPreData_Y2 = nViewY_half2 - dPreData_Y2;
		}

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->GetAt_DrawArea(nDTI_Start,&parea);
		parea = pblaAreafDTI->GetAt( dblCursor);

		curPt1.x = parea->center, curPt1.y = dPreData_Y1;
		curPt2.x = parea->center, curPt2.y = dPreData_Y2;

		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);
	
			if(pos1 == NULL || pos2 == NULL ||parea == NULL) break;		

			dValue1 = pPacket1->GetnumericDataList()->GetNext(pos1);
			dValue2 = pPacket2->GetnumericDataList()->GetNext(pos2);
	//		dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetNext(pos1));
	//		dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetNext(pos2));
			if(bLog)
			{
				dValue1 = CMath::Log(dValue1);
				dValue2 = CMath::Log(dValue2);
			}

			dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
			dViewY2 = dViewBottom - ( dViewHeight * ((dValue2 - dRealMin) / dRealHeight));

			dPreData_Y1  = ROUND(dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight)));
			dPreData_Y2  = ROUND(dViewBottom - ( dViewHeight * ((dValue2 - dRealMin) / dRealHeight)));
			
			dViewY1		= ROUND(dViewY1);
			dViewY2		= ROUND(dViewY2);

			if(bInvert == true)
			{
				dViewY1 = nViewY_half2 - dViewY1;
				dViewY2 = nViewY_half2 - dViewY2;

				dPreData_Y1 = nViewY_half2 - dPreData_Y1;
				dPreData_Y2 = nViewY_half2 - dPreData_Y2;
			}

			curPt1.x = parea->center, curPt1.y = dPreData_Y1;
			curPt2.x = parea->center, curPt2.y = dPreData_Y2;

			///수직선
			if(nDTI >= pPacket2->GetStartPeriod() && nDTI >= pPacket1->GetStartPeriod())
			{
				CPoint ptArr[4];
				ptArr[0] = prePt1;
				ptArr[1] = curPt1;
				ptArr[2] = curPt2;
				ptArr[3] = prePt2;

				double x=0, y=0;
				double	x0 = prePt1.x,	y0 = prePt1.y, 
						x1 = curPt1.x,	y1 = curPt1.y, 
						x2 = curPt2.x,	y2 = curPt2.y, 
						x3 = prePt2.x,	y3 = prePt2.y;

				if((prePt1.y >= prePt2.y && curPt1.y <= curPt2.y)
					|| (prePt1.y <= prePt2.y && curPt1.y >= curPt2.y))
				{

					if(y1==y0 || (y2-y3)/(y1-y0)==1)	///에러 처리
						y=0;
					else
						y = (y3-y0)/(1-(y2-y3)/(y1-y0));
					
					if((y3-y0+y1-y2)==0)				///에러처리
						x=0;
					else
						x = (x1-x0)*(y3-y0)/(y3-y0+y1-y2);

					x += x0;
					y += y0;
					ptArr[1].x = ptArr[2].x = CMath::Round(x);
					ptArr[1].y = ptArr[2].y = CMath::Round(y);

					if(prePt1.y > prePt2.y)
						DrawSemiTransparentBitmap(pDC, ptArr, rgb = graphColor.GetColor4());
					else if(prePt1.y < prePt2.y)
						DrawSemiTransparentBitmap(pDC, ptArr, rgb = graphColor.GetColor2());

					ptArr[0].x = ptArr[3].x = CMath::Round(x);
					ptArr[0].y = ptArr[3].y = CMath::Round(y);
					ptArr[1] = curPt1;
					ptArr[2] = curPt2;

					if(curPt1.y > curPt2.y)
						DrawSemiTransparentBitmap(pDC, ptArr, rgb = graphColor.GetColor4());
					else if(curPt1.y < curPt2.y)
						DrawSemiTransparentBitmap(pDC, ptArr, rgb = graphColor.GetColor2());
				}
				else 
				{
					if(curPt1.y >= curPt2.y)
						DrawSemiTransparentBitmap(pDC, ptArr, graphColor.GetColor4());
					else
						DrawSemiTransparentBitmap(pDC, ptArr, graphColor.GetColor2());
				}
			}

			if(graphColor.GetUpGraphFill() == false)	///의미상으로는 맞지 않지만 이 옵션은 라인을 그릴건지 말건지 결정
				pDC->SelectObject(GetStockObject(NULL_PEN));
			else
			{
				///윗선
				if(nDTI >= pPacket2->GetStartPeriod()){
					pDC->SelectObject(&downPen);
					if(nRealStart == nRealEnd)///김찬중...데이터가 한개일 때도 그린다
						CDraw::DrawLine(pDC, prePt2, curPt2);
					else if(nDTI >= pPacket2->GetStartPeriod())
						CDraw::DrawLine(pDC, prePt2, curPt2);
				}

				///아랫선
				if(nDTI >= pPacket1->GetStartPeriod()){
					pDC->SelectObject(&upPen);
					if(nRealStart == nRealEnd)///김찬중...데이터가 한개일 때도 그린다nEnd
						CDraw::DrawLine(pDC, prePt1, curPt1);
					else if(nDTI >= pPacket1->GetStartPeriod())
						CDraw::DrawLine(pDC, prePt1, curPt1);
				}
			}
			prePt1 = curPt1;
			prePt2 = curPt2;
		}
		pDC->SelectObject(pOldPen);
		upPen.DeleteObject();
		downPen.DeleteObject();
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	return true;
}

bool CLineDraw::DrawComparisonLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin,double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw, BOOL p_bWithBaseText,
		bool bDrawBaseLine)	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
{

	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)	return false;
	CPacket* pPacket = packetList.GetNext(packetPos);
	if(!pPacket)	return false;
	POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(pos == NULL)	return false;

	CList<double, double>* pCloseList = pPacket->GetnumericDataList();
	if(pCloseList == NULL)	return false;

	POSITION closePos = pCloseList->FindIndex(nRealStart);
	if(closePos == NULL)	return false;

	CBrush brushArray[ BRUSH_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[PEN_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	InitGdiObject(brushArray, penArray, graphColor, dRealWidth, penThickness, true, drawStyle);

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

		if(i > nStartPeriod)
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

bool CLineDraw::DrawComparisonLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin,double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw, BOOL p_bWithBaseText, bool bDrawBaseLine,  // 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		POSITION packetPos = packetList.GetHeadPosition();
		if(packetPos == NULL)	return false;
		CPacket* pPacket = packetList.GetNext(packetPos);
		if(!pPacket)	return false;

		int nDTI,nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		POSITION pos = pPacket->GetnumericDataList()->FindIndex(nDTI_Start);
		if(pos == NULL)	return false;

		CList<double, double>* pCloseList = pPacket->GetnumericDataList();
		if(pCloseList == NULL)	return false;

		POSITION closePos = pCloseList->FindIndex(nDTI_Start);
		if(closePos == NULL)	return false;

		CBrush brushArray[ BRUSH_COUNT];
		CBrush* pOldBrush = pDC->GetCurrentBrush();
		CPen penArray[PEN_COUNT];
		CPen* pOldPen = pDC->GetCurrentPen();
		InitGdiObject( brushArray, penArray, graphColor, dRealWidth, penThickness, true, drawStyle);

		//============================================================================================
		double dHighMax = 0.0, dLowMin = 0.0;
		pPacket->GetMinMax( nDTI_Start, nDTI_End, dLowMin, dHighMax);
		
		CPoint prePt(0, 0), curPt(0, 0);
		double dCurClose = 0.0;
		
		int nArrowLength = 11;
		bool bLeftRight = true, bUpDown = true, bHighText = true, bLowText = true;

		///로그값으로 바꿔준다
		double dLoggedViewMin = CDraw::CalLogValue(bLog, dViewMin);
		double dLoggedViewMax = CDraw::CalLogValue(bLog, dViewMax);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dRealHeight = dLoggedViewMax - dLoggedViewMin;
		double	dCurClose_Y;
		// (2007/1/14 - Seung-Won, Bae) Draw BaseLine and Texts
		// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		int nBase = CDraw::DrawBaseLineAndTexts( pDC, graphRegion, graphColor, pPacket, dViewMax, dViewMin, dLoggedViewMax, dLoggedViewMin, bLog, bInvert, p_bWithBaseText, bDrawBaseLine);

		COLORREF clr = pDC->GetTextColor();

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->GetAt_DrawArea(nDTI_Start,&parea);
		parea = pblaAreafDTI->GetAt( dblCursor);
		
		if(parea == NULL) return false;

	/// 이 타입에서는 화면에서 보여지는 시작 점보다 이전점의 역할이 없다
		dCurClose = CDraw::CalLogValue( bLog, pCloseList->GetAt(closePos));
		dCurClose_Y  = ROUND(dViewBottom - ( dViewHeight * ((dCurClose - dLoggedViewMin) / dRealHeight)));
		if(bInvert)
			dCurClose_Y = nViewY_half2 - dCurClose_Y;
		prePt.x = parea->center,prePt.y = dCurClose_Y;
	//	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dLoggedViewMin, dLoggedViewMax, CDraw::CalLogValue(bLog, dCurClose), true);
	//	if(bInvert == true)
	//		prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

		int nStartPeriod = pPacket->GetStartPeriod();
		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);
	
			if(closePos == NULL||parea == NULL)	break;

			dCurClose = CDraw::CalLogValue( bLog, pCloseList->GetNext( closePos));
			const int nIndex = nDTI - nDTI_Start;
			dCurClose_Y  = ROUND(dViewBottom - ( dViewHeight * ((dCurClose - dLoggedViewMin) / dRealHeight)));
			if(bInvert)
				dCurClose_Y = nViewY_half2 - dCurClose_Y;
			curPt.x = parea->center,curPt.y = dCurClose_Y;
	//		curPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, dLoggedViewMin, dLoggedViewMax, CDraw::CalLogValue(bLog, dCurClose), true);
			//line 을 그린다.==========================
	//		if(bInvert == true)
	//			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

			if(nDTI > nStartPeriod)
			{
				if(nDTI_Start == nDTI_End)
					Draw_SolidBong(pDC, curPt, nBase, dRealWidth, penArray);
				else if(nDTI >= nStartPeriod)
					Draw_SolidBong(pDC, prePt, curPt, nBase, penArray);

				prePt = curPt;
			}
			
			// 선차트에 최고/최저가 표시 - ojtaso (20071115)
	/*
			if( bGraphTextDraw && ( ( bHighText	&& dCurClose == dHighMax) || ( bLowText	&& dCurClose == dLowMin)))
			{
				CString strTime;
				GetPriceMinMaxTime( strTime, pPacket, nDTI);
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
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	return true;
}
//sy 2005.09.14.
void CLineDraw::Draw_SolidBong(CDC* pDC, const CPoint& curPt, const int &nBase, const double& dRealWidth, CPen* pPen)
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

void CLineDraw::Draw_SolidBong(CDC* pDC, const CPoint& prePt, const CPoint& curPt, const int &nBase, CPen* pPen)
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

void CLineDraw::InitGdiObject(CBrush* pBrush, CPen* pPen, const CGraphColor& graphColor, 
		const double& dWidth, const CPenThickness& penThickness, const bool bIsDrawingThickness, const CGraphDrawStyle& drawStyle)
{
	int nPenStyle = PS_SOLID;
	if( drawStyle.GetGraphLineDrawStyle() == CGraphBaseData::Dot_Draw)			  // 좌표점형
		nPenStyle = PS_DOT;
	else if(drawStyle.GetGraphLineDrawStyle() == CGraphBaseData::DottedLine_Draw) // 선형 점선
		nPenStyle = PS_DASH;
	else if(drawStyle.GetGraphLineDrawStyle() == CGraphBaseData::DashDot_Draw)	  // 선점형
		nPenStyle = PS_DASHDOT;
	else if(drawStyle.GetGraphLineDrawStyle() == CGraphBaseData::DashDotDot_Draw) // 선점점형
		nPenStyle = PS_DASHDOTDOT;

	pBrush[UP_BRUSH].CreateSolidBrush(graphColor.GetColor2());
	pBrush[DOWN_BRUSH].CreateSolidBrush(graphColor.GetColor4());

	if(bIsDrawingThickness)
	{
		pPen[UP_PEN].CreatePen(			nPenStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		pPen[DOWN_PEN].CreatePen(		nPenStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
		pPen[BOHAP_PEN].CreatePen(		nPenStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor5());
		pPen[STANDARD_PEN].CreatePen(	nPenStyle, 1, graphColor.GetColor6());
	}
	else{
		pPen[UP_PEN].CreatePen(			nPenStyle, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor1());
		pPen[DOWN_PEN].CreatePen(		nPenStyle, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor3());
		pPen[BOHAP_PEN].CreatePen(		nPenStyle, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor5());
		pPen[STANDARD_PEN].CreatePen(	nPenStyle, 1, graphColor.GetColor6());
	}
}

void CLineDraw::DestroyGdiObject(CDC* pDC, CBrush* pBrush, CPen* pPen, CBrush* pOldBrush, CPen* pOldPen)
{
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	for(int i = 0; i < PEN_COUNT; i++)
		pPen[i].DeleteObject();

	for(int i = 0; i < BRUSH_COUNT; i++)
		pBrush[i].DeleteObject();
}

// rainBow 차트의 펜선택하는 부분과 color가지고 오는 부분을 추가 해야 됨.
bool CLineDraw::DrawRainbowLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)		return false;

	///무지개의 선이 몇개여야 하는지는 패킷에서 가져와야 함...
	COLORREF color1 = 0, color2 = 0;
	RGBElement element1, element2, delta;
	CString str;
	int nPacketCount = packetList.GetCount();
	CArray<RGBElement, RGBElement> RGBArray;
	color1 = graphColor.GetColor1();
	color2 = graphColor.GetColor2();
	
	if(nPacketCount > 1){
		element1.R = GetRValue(color1);
		element1.G = GetGValue(color1);
		element1.B = GetBValue(color1);

		element2.R = GetRValue(color2);
		element2.G = GetGValue(color2);
		element2.B = GetBValue(color2);

		delta.R = (element1.R - element2.R)/(nPacketCount-1);
		delta.G = (element1.G - element2.G)/(nPacketCount-1);
		delta.B = (element1.B - element2.B)/(nPacketCount-1);
		RGBArray.SetSize(nPacketCount);
	}

	//============================================
	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	double dValue = 0.0;
///	CPen *pOldPen = pDC->GetCurrentPen();	
	for(int i=0 ; i < nPacketCount; i++){
		if(packetPos == NULL)	break;
		CPacket* pPacket = packetList.GetNext(packetPos);
		if(pPacket == NULL)	break;
		
		POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);
		if(pos == NULL)	break;
		
		CPoint prePt(0, 0), curPt(0, 0);

		CPen pen;
		if(i-nRealStart == 0)
			CreatePen(pen, drawStyle, 1, color1);
		else if(nPacketCount>1){
			///김찬중... RGB성분별로 분리한 것에 델타 값을 더해서 라인별 RGB값을 만든다 
			RGBArray[i].R = element1.R - delta.R * i;
			RGBArray[i].G = element1.G - delta.G * i;
			RGBArray[i].B = element1.B - delta.B * i;

			CreatePen(pen, drawStyle, 1, RGB(CMath::Round(RGBArray[i].R), CMath::Round(RGBArray[i].G), CMath::Round(RGBArray[i].B)));
		}
		CPen *pOldPen = pDC->SelectObject(&pen);

		dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));
		prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue);
		if(bInvert == true)
			prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

		for(int j = nRealStart; j <= nRealEnd; j++){
			if(pos == NULL)	break;
			dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));			
			curPt = CDataConversion::GetVerticalPoint(graphRegion, j-nRealStart, dRealWidth, dRealMin, dRealMax, dValue);

			if(bInvert == true)
				curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

			if(j >= pPacket->GetStartPeriod()){
				if(dRealMin == dRealMax)
					CDraw::DrawLine(pDC, CPoint(prePt.x, graphRegion.top + graphRegion.Height()/2), CPoint(curPt.x, graphRegion.top + graphRegion.Height()/2));
				else if(nRealStart==nRealEnd)///김찬중...데이터가 한개일 때도 그린다
					CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
				else if(j >= pPacket->GetStartPeriod()+1)
					CDraw::DrawLine(pDC, prePt, curPt);
			}
			prePt = curPt;
		}
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
///	pDC->SelectObject(pOldPen);		
	return true;
}
bool CLineDraw::DrawRainbowLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{
		POSITION packetPos = packetList.GetHeadPosition();
		if(packetPos == NULL)		return false;

		int nDTI,nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		///무지개의 선이 몇개여야 하는지는 패킷에서 가져와야 함...
		COLORREF color1 = 0, color2 = 0;
		RGBElement element1, element2, delta;
		CString str;
		int nPacketCount = packetList.GetCount();
		CArray<RGBElement, RGBElement> RGBArray;
		color1 = graphColor.GetColor1();
		color2 = graphColor.GetColor2();
		
		if(nPacketCount > 1){
			element1.R = GetRValue(color1);
			element1.G = GetGValue(color1);
			element1.B = GetBValue(color1);

			element2.R = GetRValue(color2);
			element2.G = GetGValue(color2);
			element2.B = GetBValue(color2);

			delta.R = (element1.R - element2.R)/(nPacketCount-1);
			delta.G = (element1.G - element2.G)/(nPacketCount-1);
			delta.B = (element1.B - element2.B)/(nPacketCount-1);
			RGBArray.SetSize(nPacketCount);
		}

		//============================================
		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		double dRealHeight = dRealMax - dRealMin;

		double dValue = 0.0;
		double dPreData_Y = 0.0;
	///	CPen *pOldPen = pDC->GetCurrentPen();	

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;
		for(int i=0 ; i < nPacketCount; i++)
		{
			if(packetPos == NULL)	break;
			CPacket* pPacket = packetList.GetNext(packetPos);
			if(pPacket == NULL)	break;
			
			POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);
			if(pos == NULL)	break;
			
			CPoint prePt(0, 0), curPt(0, 0);

			CPen pen;
			if(i-nRealStart == 0)
				CreatePen(pen, drawStyle, 1, color1);
			else if(nPacketCount>1){
				///김찬중... RGB성분별로 분리한 것에 델타 값을 더해서 라인별 RGB값을 만든다 
				RGBArray[i].R = element1.R - delta.R * i;
				RGBArray[i].G = element1.G - delta.G * i;
				RGBArray[i].B = element1.B - delta.B * i;

				CreatePen(pen, drawStyle, 1, RGB(CMath::Round(RGBArray[i].R), CMath::Round(RGBArray[i].G), CMath::Round(RGBArray[i].B)));
			}
			CPen *pOldPen = pDC->SelectObject(&pen);

			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->GetAt_DrawArea(nDTI_Start,&parea);
			parea = pblaAreafDTI->GetAt( dblCursor);

			if(parea == NULL) break;

			dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));
			dPreData_Y  = ROUND(dViewBottom - ( dViewHeight * ((dValue - dRealMin) / dRealHeight)));
			if(bInvert)
				dPreData_Y = nViewY_half2 - dPreData_Y;
			prePt.x = parea->center;
			prePt.y = dPreData_Y;

	//		prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue);
	//		if(bInvert == true)
	//			prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
			int nZeroDTI = 0;
			CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
			if( !pblaAreafDTI) return false;
			CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
			pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

			for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
			{
				// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
				//	pxScaleManager->NextDrawArea(&parea);
				parea = pblaAreafDTI->GetNext( dblCursor);
		
				if(pos == NULL||parea == NULL)	break;
				dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));			
				dPreData_Y  = ROUND(dViewBottom - ( dViewHeight * ((dValue - dRealMin) / dRealHeight)));
				if(bInvert)
					dPreData_Y = nViewY_half2 - dPreData_Y;
				curPt.x = parea->center,curPt.y = dPreData_Y;
	//			curPt = CDataConversion::GetVerticalPoint(graphRegion, j-nRealStart, dRealWidth, dRealMin, dRealMax, dValue);
	//			if(bInvert == true)
	//				curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

				if(nDTI >= pPacket->GetStartPeriod()){
					if(dRealMin == dRealMax)
						CDraw::DrawLine(pDC, CPoint(prePt.x, graphRegion.top + graphRegion.Height()/2), CPoint(curPt.x, graphRegion.top + graphRegion.Height()/2));
					else if(nDTI_Start==nDTI_End)///김찬중...데이터가 한개일 때도 그린다
						CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
					else if(nDTI >= pPacket->GetStartPeriod()+1)
						CDraw::DrawLine(pDC, prePt, curPt);
				}
				prePt = curPt;
			}
			pDC->SelectObject(pOldPen);
			pen.DeleteObject();
		}
	///	pDC->SelectObject(pOldPen);		
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	return true;
}
bool CLineDraw::DrawZigzagLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	// modified by rich 02/10/04 인덱스 패킷처리
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	return false;
	CPacket* pIndexPacket = packetList.GetNext(pos);	// 색깔인덱스 패킷
	if(pIndexPacket == NULL || pos == NULL)	return false;
	CPacket* pPacket = packetList.GetNext(pos);		// 데이터 패킷
	if(pPacket == NULL)	return false;
	
	CList<double, double>* pIndexList = pIndexPacket->GetnumericDataList();
	CList<double, double>* pValueList = pPacket->GetnumericDataList();
	if(pIndexList == NULL || pValueList == NULL)	return false;
	if(pIndexList->GetCount() <= 0 || pIndexList->GetCount() != pValueList->GetCount())	return false;

	POSITION indexPos = pIndexList->FindIndex(nRealStart);
	POSITION prePos = pValueList->FindIndex(nRealStart);
	if(prePos == NULL || indexPos == NULL)	return false;

	POSITION curPos = prePos;
	POSITION prevPos = indexPos;	// 색깔인덱스를 역으로 추적하기 위함

	CPen linePen1, linePen2; // , linePen3;
	CreatePen(linePen1, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CreatePen(linePen2, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
	CPen *pOldPen = pDC->SelectObject(&linePen1);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	CPoint prePt(0, 0), curPt(0, 0);
	double dPreData = 0.0, dCurData = 0.0, dIndex = 0.0;
	// modified by rich 02/10/04 색변경을 위한 플래그
	BOOL bChangeFlag = FALSE;
	double dFirstIndex = 0.0, dPrevIndex = 0.0;

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(prePos == NULL || indexPos == NULL || curPos == NULL)	break;
		dPreData = CDraw::CalLogValue(bLog, pValueList->GetNext(prePos));
		dIndex = pIndexList->GetNext(indexPos);

		if(dIndex == 9999 || dIndex == -9999)
			dPrevIndex = dIndex;

		if(i - nRealStart == 0){
			prePt = CDataConversion::GetVerticalPoint(graphRegion, i - nRealStart, dRealWidth, dRealMin, dRealMax, dPreData);
			curPt = prePt;
			prePos = curPos;
			// 최초의 색을 알기위해 역으로 추적
			while(prevPos != NULL){
				dFirstIndex = pIndexList->GetPrev(prevPos);
				if(dFirstIndex == 9999){
					pDC->SelectObject(&linePen1);
					break;
				}
				else if(dFirstIndex == -9999){
					pDC->SelectObject(&linePen2);
					break;
				}
			}
		}
		else{
			prePt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart-1, dRealWidth, dRealMin, dRealMax, dPreData);
			curPos = prePos;
			dCurData = CDraw::CalLogValue(bLog, pValueList->GetNext(curPos));
			curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dCurData);
		}
		//line 을 그린다.==========================
		if(bInvert == true){
			prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);
			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);
		}

		if(i >= pPacket->GetStartPeriod()){
			if(nRealStart == nRealEnd)	// 데이터가 한개일 때도 그린다
				CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth / 2)), CPoint(curPt.x + CMath::Round(dRealWidth / 2)));
			else if(i >= pPacket->GetStartPeriod())
				CDraw::DrawLine(pDC, prePt, curPt);
		}

		if(bChangeFlag == TRUE && dPrevIndex == 9999){
			pDC->SelectObject(&linePen1);
			bChangeFlag = FALSE;
		}

		if(dIndex == 9999)	// 전환점
			bChangeFlag = TRUE;
		else if(dIndex == -9999)
			pDC->SelectObject(&linePen2);
	}

	pDC->SelectObject(pOldPen);
	linePen1.DeleteObject();
	linePen2.DeleteObject();
	
	return true;
}
bool CLineDraw::DrawZigzagLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		// modified by rich 02/10/04 인덱스 패킷처리
		POSITION pos = packetList.GetHeadPosition();
		if(pos == NULL)	return false;
		CPacket* pIndexPacket = packetList.GetNext(pos);	// 색깔인덱스 패킷
		if(pIndexPacket == NULL || pos == NULL)	return false;
		CPacket* pPacket = packetList.GetNext(pos);		// 데이터 패킷
		if(pPacket == NULL)	return false;
		
		CList<double, double>* pIndexList = pIndexPacket->GetnumericDataList();
		CList<double, double>* pValueList = pPacket->GetnumericDataList();
		if(pIndexList == NULL || pValueList == NULL)	return false;
		if(pIndexList->GetCount() <= 0 || pIndexList->GetCount() != pValueList->GetCount())	return false;

		int nDTI,nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		POSITION indexPos = pIndexList->FindIndex(nDTI_Start);
		POSITION prePos = pValueList->FindIndex(nDTI_Start);
		if(prePos == NULL || indexPos == NULL)	return false;

		POSITION curPos = prePos;
		POSITION prevPos = indexPos;	// 색깔인덱스를 역으로 추적하기 위함

		CPen linePen1, linePen2; // , linePen3;
		CreatePen(linePen1, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		CreatePen(linePen2, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
		CPen *pOldPen = pDC->SelectObject(&linePen1);

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

		//double dViewBottom = (double)graphRegion.bottom;
		//double dViewHeight = (double)graphRegion.Height();
		//int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		//double dRealHeight = dRealMax - dRealMin;
		//double dPreData_Y= 0.0,dCurData_Y= 0.0;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;

		CPoint prePt(0, 0), curPt(0, 0);
		double dPreData = 0.0, dCurData = 0.0, dIndex = 0.0;
		// modified by rich 02/10/04 색변경을 위한 플래그
		BOOL bChangeFlag = FALSE;
		double dFirstIndex = 0.0, dPrevIndex = 0.0;

	//	for(int i = nRealStart; i <= nRealEnd; i++)
		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);
		
			if(prePos == NULL || indexPos == NULL || curPos == NULL || parea == NULL)	break;

			dPreData = CDraw::CalLogValue(bLog, pValueList->GetNext(prePos));
			dIndex = pIndexList->GetNext(indexPos);

			if(dIndex == 9999 || dIndex == -9999)
				dPrevIndex = dIndex;

			if(nDTI - nDTI_Start == 0)
			{
				//dPreData_Y  = ROUND(dViewBottom - ( dViewHeight * ((dPreData - dRealMin) / dRealHeight)));
				prePt = CDataConversion::GetVerticalPoint(graphRegion, nDTI - nRealStart, dRealWidth, dRealMin, dRealMax, dPreData);
				prePt.x = parea->center;
				curPt = prePt;
				prePos = curPos;
				// 최초의 색을 알기위해 역으로 추적
				while(prevPos != NULL){
					dFirstIndex = pIndexList->GetPrev(prevPos);
					if(dFirstIndex == 9999){
						pDC->SelectObject(&linePen1);
						break;
					}
					else if(dFirstIndex == -9999){
						pDC->SelectObject(&linePen2);
						break;
					}
				}
			}
			else{
				//prePt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart-1, dRealWidth, dRealMin, dRealMax, dPreData);
				prePt = curPt;
				curPos = prePos;
				dCurData = CDraw::CalLogValue(bLog, pValueList->GetNext(curPos));
				curPt = CDataConversion::GetVerticalPoint(graphRegion, nDTI-nRealStart, dRealWidth, dRealMin, dRealMax, dCurData);
				curPt.x = parea->center;
			}
			//line 을 그린다.==========================
			if(bInvert == true){
				prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);
				curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);
			}

			if(nDTI >= pPacket->GetStartPeriod()){
				if(nRealStart == nRealEnd)	// 데이터가 한개일 때도 그린다
					CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth / 2)), CPoint(curPt.x + CMath::Round(dRealWidth / 2)));
				else if(nDTI >= pPacket->GetStartPeriod())
					CDraw::DrawLine(pDC, prePt, curPt);
			}

			if(bChangeFlag == TRUE && dPrevIndex == 9999){
				pDC->SelectObject(&linePen1);
				bChangeFlag = FALSE;
			}

			if(dIndex == 9999)	// 전환점
				bChangeFlag = TRUE;
			else if(dIndex == -9999)
				pDC->SelectObject(&linePen2);
		}

		pDC->SelectObject(pOldPen);
		linePen1.DeleteObject();
		linePen2.DeleteObject();
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	
	return true;
}
bool CLineDraw::DrawLineCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)	return false;
	CPacket* pPacket1 = packetList.GetNext(packetPos);
	if(packetPos == NULL)	return false;
	CPacket* pPacket2 = packetList.GetNext(packetPos);
	if(pPacket1 == NULL || pPacket2 == NULL)	return false;
	POSITION pos1 = pPacket1->GetnumericDataList()->FindIndex(nRealStart);
	POSITION pos2 = pPacket2->GetnumericDataList()->FindIndex(nRealStart);
	if(pos1 == NULL || pos2 == NULL)	return false;
	
	//팬 선택한다.
	CPen upPen, downPen;
	CreatePen(upPen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CreatePen(downPen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
	CPen* pOldPen = (CPen*)pDC->SelectObject(&upPen);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin); 
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	double dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetAt(pos1));
	double dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetAt(pos2));

	//처음 시작점은 한칸의 왼쪽부터 시작
	CPoint prePt1 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue1, false);
	CPoint prePt2 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue2, false);

	if(bInvert == true){
		prePt1.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt1.y);
		prePt2.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt2.y);
	}

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos1 == NULL || pos2 == NULL) break;

		dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetNext(pos1));
		dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetNext(pos2));

		CPoint curPt1 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue1);
		CPoint curPt2 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue2);
		
// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
/*
		// 봉의 수직선과 겹치는것 방지
		if(curPt1.x - prePt1.x > 3){
			curPt1.Offset(2, 0);
			curPt2.Offset(2, 0);
		}
		else if(curPt1.x - prePt1.x > 0){
			curPt1.Offset(1, 0);
			curPt2.Offset(1, 0);
		}
*/
		if(bInvert == true){
			curPt1.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt1.y);
			curPt2.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt2.y);
		}

		if(i >= pPacket1->GetStartPeriod()){
			pDC->SelectObject(&upPen);
			CDraw::DrawLine(pDC, prePt1, curPt1);
		}
		if(i >= pPacket2->GetStartPeriod()){
			pDC->SelectObject(&downPen);
			CDraw::DrawLine(pDC, prePt2, curPt2);
		}
		if(i >= pPacket2->GetStartPeriod() && i >= pPacket1->GetStartPeriod()){
			if(curPt1.y <= curPt2.y)
				pDC->SelectObject(&upPen);
			else
				pDC->SelectObject(&downPen);

			CDraw::DrawLine(pDC, curPt1, curPt2);
		}
		prePt1 = curPt1;
		prePt2 = curPt2;
	}
	pDC->SelectObject(pOldPen);
	upPen.DeleteObject();
	downPen.DeleteObject();
	return true;
}
bool CLineDraw::DrawLineCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		POSITION packetPos = packetList.GetHeadPosition();
		if(packetPos == NULL)	return false;
		CPacket* pPacket1 = packetList.GetNext(packetPos);
		if(packetPos == NULL)	return false;
		CPacket* pPacket2 = packetList.GetNext(packetPos);
		if(pPacket1 == NULL || pPacket2 == NULL)	return false;

		int nDTI, nDTI_Start, nDTI_End;
		
		BOOL bOEBT;
		if(pIndicatorInfo == NULL) bOEBT = FALSE;
		else					   bOEBT = (pIndicatorInfo->GetIndicatorName() == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART));
		
		pxScaleManager->GetDrawIndex(strRQ, FALSE, bOEBT, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		POSITION pos1 = pPacket1->GetnumericDataList()->FindIndex(nDTI_Start);
		POSITION pos2 = pPacket2->GetnumericDataList()->FindIndex(nDTI_Start);
		if(pos1 == NULL || pos2 == NULL)	return false;
		
		//팬 선택한다.
		CPen upPen, downPen;
		CreatePen(upPen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		CreatePen(downPen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
		CPen* pOldPen = (CPen*)pDC->SelectObject(&upPen);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dViewY1, dViewY2;
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin); 
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
		double dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetAt(pos1));
		double dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetAt(pos2));
		double dRealHeight = dRealMax - dRealMin;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;
		
		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->GetAt_DrawArea(nDTI_Start,&parea);
		parea = pblaAreafDTI->GetAt( dblCursor);

		if(parea == NULL) return false;

		//처음 시작점은 한칸의 왼쪽부터 시작
	//	CPoint prePt1 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue1, false);
	//	CPoint prePt2 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue2, false);
		CPoint prePt1,prePt2,curPt1,curPt2;
		dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
		dViewY2 = dViewBottom - ( dViewHeight * ((dValue2 - dRealMin) / dRealHeight));
		dViewY1		= ROUND(dViewY1);
		dViewY2		= ROUND(dViewY2);

		if(bInvert) {
			dViewY1 = nViewY_half2 - dViewY1;
			dViewY2 = nViewY_half2 - dViewY2;
		}
	//	if(bInvert == true){
	//		prePt1.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt1.y);
	//		prePt2.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt2.y);
	//	}
		prePt1.x = parea->center,prePt1.y = dViewY1;
		prePt2.x = parea->center,prePt2.y = dViewY2;

	//	for(int i = nRealStart; i <= nRealEnd; i++)
		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
	//		if(pos1 == NULL || pos2 == NULL) break;

			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);
		
			if(pos1 == NULL || pos2 == NULL || parea == NULL) break;		

	//		dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetNext(pos1));
	//		dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetNext(pos2));
			dValue1 = pPacket1->GetnumericDataList()->GetNext(pos1);
			dValue2 = pPacket2->GetnumericDataList()->GetNext(pos2);
			if(bLog){
				dValue1 = CMath::Log(dValue1);
				dValue2 = CMath::Log(dValue2);
			}
			
	//		CPoint curPt1 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue1);
	//		CPoint curPt2 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue2);
			dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
			dViewY2 = dViewBottom - ( dViewHeight * ((dValue2 - dRealMin) / dRealHeight));
			dViewY1		= ROUND(dViewY1);
			dViewY2		= ROUND(dViewY2);

			if(bInvert) {
				dViewY1 = nViewY_half2 - dViewY1;
				dViewY2 = nViewY_half2 - dViewY2;
			}
	//		if(bInvert == true)
	//		{
	//			curPt1.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt1.y);
	//			curPt2.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt2.y);
	//		}
			curPt1.x = parea->center,curPt1.y = dViewY1;
			curPt2.x = parea->center,curPt2.y = dViewY2;

			if(nDTI >= pPacket1->GetStartPeriod())
			{
				pDC->SelectObject(&upPen);
				CDraw::DrawLine(pDC, prePt1, curPt1);
			}
			if(nDTI >= pPacket2->GetStartPeriod()){
				pDC->SelectObject(&downPen);
				CDraw::DrawLine(pDC, prePt2, curPt2);
			}
			if(nDTI >= pPacket2->GetStartPeriod() && nDTI >= pPacket1->GetStartPeriod()){
				if(curPt1.y <= curPt2.y)
					pDC->SelectObject(&upPen);
				else
					pDC->SelectObject(&downPen);

				CDraw::DrawLine(pDC, curPt1, curPt2);
			}
			prePt1 = curPt1;
			prePt2 = curPt2;
		}
		pDC->SelectObject(pOldPen);
		upPen.DeleteObject();
		downPen.DeleteObject();
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	return true;
}

bool CLineDraw::DrawDataDotSimple(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	double dStandard = 0.0;   // 임이의 기준가~~~
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)		return false;
	CPacket* pPacket = packetList.GetNext(packetPos);
	if(!pPacket)	return false;
	
	POSITION pos = pPacket->GetnumericDataList()->FindIndex(nRealStart);

	if(pos == NULL)	return false;
	//팬 선택한다.
	CPen linePen, standardPen;
	CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CreatePen(standardPen, drawStyle, 1, graphColor.GetColor6());
	CPen* pOldPen = pDC->SelectObject(&standardPen);

	// 기준가를 가지고 오는부분. 그려줌.===================================================================================
	///이부분은 데이터가 1개일 때를 고려할 필요가 없다.
	CPoint standardPt(0, 0);
	if(pIndicatorInfo != NULL){
		CList<double, double> * pBaseLineList = NULL;
		pBaseLineList = pIndicatorInfo->GetBaseLineList();
		if(pBaseLineList != NULL){
			POSITION basePos = pBaseLineList->GetHeadPosition();
			if(basePos != NULL){
				dStandard = pBaseLineList->GetNext(basePos);
				standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
				if(bInvert == true)	
					standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

				if(dStandard >= dViewMin && dStandard <= dViewMax)
					CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
				for(int base = 2; base <= pBaseLineList->GetCount(); base++){
					if(basePos == NULL)
						break;
					dStandard = pBaseLineList->GetNext(basePos);
					standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
					if(bInvert == true)	
						standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
					if(dStandard >= dViewMin && dStandard <= dViewMax)
						CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
				}
			}
		}
	}
	pDC->SelectObject(&linePen);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	CPoint prePt(0, 0), curPt(0, 0);
	double dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));
	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue);
	if(bInvert == true)
		prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos == NULL)	break;
		dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));
		curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue);

		if(bInvert == true)
			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

		if(i >= pPacket->GetStartPeriod()){
			if(nRealStart == nRealEnd)///김찬중...데이터가 한개일 때도 그린다
			{
///				CDraw::DrawLine(pDC, prePt, curPt);
				CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
				CDraw::DrawTriangle(pDC, prePt, graphColor.GetColor1());
				CDraw::DrawTriangle(pDC, curPt, graphColor.GetColor1());
			}
			else if(i >= pPacket->GetStartPeriod())
			{
				CDraw::DrawLine(pDC, prePt, curPt);
///				CDraw::DrawTriangle(pDC, prePt, graphColor.GetColor1());
				CDraw::DrawTriangle(pDC, curPt, graphColor.GetColor1());
			}
		}
		prePt = curPt;
	}
	pDC->SelectObject(pOldPen);
	linePen.DeleteObject();
	standardPen.DeleteObject();
	
	return true;
}
bool CLineDraw::DrawDataDotSimple(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{

		double dStandard = 0.0;   // 임이의 기준가~~~
		POSITION packetPos = packetList.GetHeadPosition();
		if(packetPos == NULL)		return false;
		CPacket* pPacket = packetList.GetNext(packetPos);
		if(!pPacket)	return false;
		
		int nDTI, nDTI_Start, nDTI_End;
		pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
		if(nDTI_Start == -1) return true;

		POSITION pos = pPacket->GetnumericDataList()->FindIndex(nDTI_Start);

		if(pos == NULL)	return false;
		//팬 선택한다.
		CPen linePen, standardPen;
		CreatePen(linePen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
		CreatePen(standardPen, drawStyle, 1, graphColor.GetColor6());
		CPen* pOldPen = pDC->SelectObject(&standardPen);

		// 기준가를 가지고 오는부분. 그려줌.===================================================================================
		///이부분은 데이터가 1개일 때를 고려할 필요가 없다.
		CPoint standardPt(0, 0);
		if(pIndicatorInfo != NULL){
			CList<double, double> * pBaseLineList = NULL;
			pBaseLineList = pIndicatorInfo->GetBaseLineList();
			if(pBaseLineList != NULL){
				POSITION basePos = pBaseLineList->GetHeadPosition();
				if(basePos != NULL){
					dStandard = pBaseLineList->GetNext(basePos);
					standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
					if(bInvert == true)	
						standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

					if(dStandard >= dViewMin && dStandard <= dViewMax)
						CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
					for(int base = 2; base <= pBaseLineList->GetCount(); base++){
						if(basePos == NULL)
							break;
						dStandard = pBaseLineList->GetNext(basePos);
						standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dViewMin, dViewMax, dStandard);
						if(bInvert == true)	
							standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
						if(dStandard >= dViewMin && dStandard <= dViewMax)
							CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
					}
				}
			}
		}
		pDC->SelectObject(&linePen);

		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		double dRealHeight = dViewMax - dViewMin;
		double dViewY1;//, dViewY2;
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;

		double dRealMin = CDraw::CalLogValue(bLog, dViewMin);
		double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
		double dValue1 = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		CXScaleManager::PAREA parea;
		
		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->GetAt_DrawArea(nDTI_Start,&parea);
		parea = pblaAreafDTI->GetAt( dblCursor);

		if(parea == NULL) return false;

		CPoint prePt(0, 0), curPt(0, 0);
	//	double dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetAt(pos));
		dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
		dViewY1		= ROUND(dViewY1);

	//	prePt = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue);
		if(bInvert) 
			dViewY1 = nViewY_half2 - dViewY1;
	//	if(bInvert == true)
	//		prePt.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt.y);

		prePt.x = parea->center,prePt.y = dViewY1;

	//	for(int i = nRealStart; i <= nRealEnd; i++){
		for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
			//	pxScaleManager->NextDrawArea(&parea);
			parea = pblaAreafDTI->GetNext( dblCursor);
	
			if(pos == NULL || parea == NULL ) break;		

	//		if(pos == NULL)	break;
	//		dValue = CDraw::CalLogValue(bLog, pPacket->GetnumericDataList()->GetNext(pos));
			dValue1 = pPacket->GetnumericDataList()->GetNext(pos);
			if(bLog)
				dValue1 = CMath::Log(dValue1);

	//		curPt = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue);
			dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
			dViewY1		= ROUND(dViewY1);

	//		if(bInvert == true)
	//			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

			if(bInvert)
				dViewY1 = nViewY_half2 - dViewY1;

			curPt.x = parea->center,curPt.y = dViewY1;

	//		if(i >= pPacket->GetStartPeriod()){
			if(nDTI >= pPacket->GetStartPeriod())
			{
	//			if(nRealStart == nRealEnd)///김찬중...데이터가 한개일 때도 그린다
				if(nDTI_Start == nDTI_End)///김찬중...데이터가 한개일 때도 그린다
				{
	///				CDraw::DrawLine(pDC, prePt, curPt);
					CDraw::DrawLine(pDC, CPoint(curPt.x - CMath::Round(dRealWidth/2), curPt.y), CPoint(curPt.x + CMath::Round(dRealWidth/2), curPt.y));
					CDraw::DrawTriangle(pDC, prePt, graphColor.GetColor1());
					CDraw::DrawTriangle(pDC, curPt, graphColor.GetColor1());
				}
	//			else if(i >= pPacket->GetStartPeriod())
				else
				{
					CDraw::DrawLine(pDC, prePt, curPt);
	///				CDraw::DrawTriangle(pDC, prePt, graphColor.GetColor1());
					CDraw::DrawTriangle(pDC, curPt, graphColor.GetColor1());
				}
			}
			prePt = curPt;
		}
		pDC->SelectObject(pOldPen);
		linePen.DeleteObject();
		standardPen.DeleteObject();
	}
// 	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
// 	{
// 	}
	
	return true;
}

bool CLineDraw::DrawNonCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)	return false;
	CPacket* pPacket1 = packetList.GetNext(packetPos);
	if(packetPos == NULL)	return false;
	CPacket* pPacket2 = packetList.GetNext(packetPos);
	if(pPacket1 == NULL || pPacket2 == NULL)	return false;
	POSITION pos1 = pPacket1->GetnumericDataList()->FindIndex(nRealStart);
	POSITION pos2 = pPacket2->GetnumericDataList()->FindIndex(nRealStart);
	if(pos1 == NULL || pos2 == NULL)	return false;
	
	//팬 선택한다.
	CPen upPen, downPen;
	CreatePen(upPen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CreatePen(downPen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
	CPen* pOldPen = (CPen*)pDC->SelectObject(&upPen);

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin); 
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);

	double dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetAt(pos1));
	double dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetAt(pos2));

	//처음 시작점은 한칸의 왼쪽부터 시작
	CPoint prePt1 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue1, false);
	CPoint prePt2 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue2, false);

	if(bInvert == true){
		prePt1.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt1.y);
		prePt2.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt2.y);
	}

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(pos1 == NULL || pos2 == NULL) break;

		dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetNext(pos1));
		dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetNext(pos2));

		CPoint curPt1 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue1);
		CPoint curPt2 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue2);
		
// (2006/12/29 - Seung-Won, Bae) Support Fixed Width.
/*
		// 봉의 수직선과 겹치는것 방지
		if(curPt1.x - prePt1.x > 3){
			curPt1.Offset(2, 0);
			curPt2.Offset(2, 0);
		}
		else if(curPt1.x - prePt1.x > 0){
			curPt1.Offset(1, 0);
			curPt2.Offset(1, 0);
		}
*/
		if(bInvert == true){
			curPt1.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt1.y);
			curPt2.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt2.y);
		}

		if(i >= pPacket1->GetStartPeriod()){
			pDC->SelectObject(&upPen);
			CDraw::DrawLine(pDC, prePt1, curPt1);
		}
		if(i >= pPacket2->GetStartPeriod()){
			pDC->SelectObject(&downPen);
			CDraw::DrawLine(pDC, prePt2, curPt2);
		}
//		if(i >= pPacket2->GetStartPeriod() && i >= pPacket1->GetStartPeriod()){
//			if(curPt1.y <= curPt2.y)
//				pDC->SelectObject(&upPen);
//			else
//				pDC->SelectObject(&downPen);
//
//			CDraw::DrawLine(pDC, curPt1, curPt2);
//		}
		prePt1 = curPt1;
		prePt2 = curPt2;
	}
	pDC->SelectObject(pOldPen);
	upPen.DeleteObject();
	downPen.DeleteObject();
	return true;
}
bool CLineDraw::DrawNonCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	POSITION packetPos = packetList.GetHeadPosition();
	if(packetPos == NULL)	return false;
	CPacket* pPacket1 = packetList.GetNext(packetPos);
	if(packetPos == NULL)	return false;
	CPacket* pPacket2 = packetList.GetNext(packetPos);
	if(pPacket1 == NULL || pPacket2 == NULL)	return false;

	int nDTI, nDTI_Start, nDTI_End;
	
	BOOL bOEBT;
	if(pIndicatorInfo == NULL) bOEBT = FALSE;
	else					   bOEBT = (pIndicatorInfo->GetIndicatorName() == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART));
	
	pxScaleManager->GetDrawIndex(strRQ, FALSE, bOEBT, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	POSITION pos1 = pPacket1->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION pos2 = pPacket2->GetnumericDataList()->FindIndex(nDTI_Start);
	if(pos1 == NULL || pos2 == NULL)	return false;
	
	//팬 선택한다.
	CPen upPen, downPen;
	CreatePen(upPen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	CreatePen(downPen, drawStyle, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
	CPen* pOldPen = (CPen*)pDC->SelectObject(&upPen);

	double dViewBottom = (double)graphRegion.bottom;
	double dViewHeight = (double)graphRegion.Height();
	double dRealHeight = dViewMax - dViewMin;
	double dViewY1, dViewY2;
	int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;

	double dRealMin = CDraw::CalLogValue(bLog, dViewMin); 
	double dRealMax = CDraw::CalLogValue(bLog, dViewMax);
	double dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetAt(pos1));
	double dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetAt(pos2));

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

	CXScaleManager::PAREA parea;
	
	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->GetAt_DrawArea(nDTI_Start,&parea);
	parea = pblaAreafDTI->GetAt( dblCursor);

	if(parea == NULL) return false;

	//처음 시작점은 한칸의 왼쪽부터 시작
//	CPoint prePt1 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue1, false);
//	CPoint prePt2 = CDataConversion::GetVerticalPoint(graphRegion, 0, dRealWidth, dRealMin, dRealMax, dValue2, false);
	CPoint prePt1,prePt2,curPt1,curPt2;
	dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
	dViewY2 = dViewBottom - ( dViewHeight * ((dValue2 - dRealMin) / dRealHeight));
	dViewY1		= ROUND(dViewY1);
	dViewY2		= ROUND(dViewY2);

	if(bInvert) {
		dViewY1 = nViewY_half2 - dViewY1;
		dViewY2 = nViewY_half2 - dViewY2;
	}
//	if(bInvert == true){
//		prePt1.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt1.y);
//		prePt2.y = CDataConversion::InvertGraphDataPt(graphRegion, prePt2.y);
//	}
	prePt1.x = parea->center,prePt1.y = dViewY1;
	prePt2.x = parea->center,prePt2.y = dViewY2;

//	for(int i = nRealStart; i <= nRealEnd; i++)
	for(nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
	{
		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->NextDrawArea(&parea);
		parea = pblaAreafDTI->GetNext( dblCursor);

//		if(pos1 == NULL || pos2 == NULL) break;
//		if(nDTI == 0 && pxScaleManager->IsNoTimeValue())
//			continue;
		if(pos1 == NULL || pos2 == NULL || parea == NULL) break;		

//		dValue1 = CDraw::CalLogValue(bLog, pPacket1->GetnumericDataList()->GetNext(pos1));
//		dValue2 = CDraw::CalLogValue(bLog, pPacket2->GetnumericDataList()->GetNext(pos2));
		dValue1 = pPacket1->GetnumericDataList()->GetNext(pos1);
		dValue2 = pPacket2->GetnumericDataList()->GetNext(pos2);
		if(bLog){
			dValue1 = CMath::Log(dValue1);
			dValue2 = CMath::Log(dValue2);
		}
		
//		CPoint curPt1 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue1);
//		CPoint curPt2 = CDataConversion::GetVerticalPoint(graphRegion, i-nRealStart, dRealWidth, dRealMin, dRealMax, dValue2);
		dViewY1 = dViewBottom - ( dViewHeight * ((dValue1 - dRealMin) / dRealHeight));
		dViewY2 = dViewBottom - ( dViewHeight * ((dValue2 - dRealMin) / dRealHeight));
		dViewY1		= ROUND(dViewY1);
		dViewY2		= ROUND(dViewY2);

		if(bInvert) {
			dViewY1 = nViewY_half2 - dViewY1;
			dViewY2 = nViewY_half2 - dViewY2;
		}
//		if(bInvert == true)
//		{
//			curPt1.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt1.y);
//			curPt2.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt2.y);
//		}
		curPt1.x = parea->center,curPt1.y = dViewY1;
		curPt2.x = parea->center,curPt2.y = dViewY2;

		if(nDTI >= pPacket1->GetStartPeriod())
		{
			pDC->SelectObject(&upPen);
			CDraw::DrawLine(pDC, prePt1, curPt1);
		}
		if(nDTI >= pPacket2->GetStartPeriod()){
			pDC->SelectObject(&downPen);
			CDraw::DrawLine(pDC, prePt2, curPt2);
		}
//		if(nDTI >= pPacket2->GetStartPeriod() && nDTI >= pPacket1->GetStartPeriod()){
//			if(curPt1.y <= curPt2.y)
//				pDC->SelectObject(&upPen);
//			else
//				pDC->SelectObject(&downPen);
//
//			CDraw::DrawLine(pDC, curPt1, curPt2);
//		}
		prePt1 = curPt1;
		prePt2 = curPt2;
	}
	pDC->SelectObject(pOldPen);
	upPen.DeleteObject();
	downPen.DeleteObject();
	return true;
}

// ----------------------------------------------------------------------------
int CLineDraw::CalculateDeclination(const CPoint startPt, const CPoint endPt, const CPoint standardStartPt, const CPoint standardEndPt)
{
	if(startPt.y > standardStartPt.y && endPt.y < standardEndPt.y)
		return ((endPt.x - startPt.x) * (startPt.y - standardStartPt.y)) / (startPt.y - endPt.y);
	else if(startPt.y < standardStartPt.y && endPt.y > standardEndPt.y)
		return ((endPt.x - startPt.x) * (standardStartPt.y - startPt.y)) / (endPt.y - startPt.y);
	
	return 0;
}

void CLineDraw::GetStandardValue(CDC* pDC, CPacket *pPacket, CIndicatorInfo *pIndicatorInfo, 
		double dRealMin, double dRealMax, double dViewMin, double dViewMax, double dRealWidth, 
		bool bInvert, const CRect& graphRegion, double& dStandardMin, double& dStandardMax)
{
	if(pIndicatorInfo == NULL){
		dStandardMin = 0.0;
		dStandardMax = 0.0;
		return;
	}
	
	double dStandard = 0.0;
	CPoint standardPt(0, 0);
	CList<double, double> * pBaseLineList = pIndicatorInfo->GetBaseLineList();

	if(pBaseLineList == NULL){
		dStandardMin = 0.0;
		dStandardMax = 0.0;
	}
	else{
		POSITION basePos = pBaseLineList->GetHeadPosition();
		if(basePos == NULL){
			dStandardMin = 0.0;
			dStandardMax = 0.0;
		}
		else
			dStandard = pBaseLineList->GetNext(basePos);
		standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandard);
		if(dStandard >= dViewMin && dStandard <= dViewMax){
			if(bInvert == true)
				standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
			if( pDC) CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
			if(bInvert == true)		///복구
				standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);
		}
		dStandardMin = dStandard;
		dStandardMax = dStandardMin;

		for(int base = 2; base <= pBaseLineList->GetCount(); base++){
			if(basePos == NULL)	break;
			dStandard = pBaseLineList->GetNext(basePos);
			standardPt = CDataConversion::GetVerticalPoint(graphRegion, 1, dRealWidth, dRealMin, dRealMax, dStandard);
			if(bInvert == true)
				standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

			if(dStandard >= dRealMin && dStandard <= dRealMax && pDC)
				CDraw::DrawLine(pDC, CPoint(graphRegion.left, standardPt.y), CPoint(graphRegion.right, standardPt.y));
			if(bInvert == true) ///복구
				standardPt.y = CDataConversion::InvertGraphDataPt(graphRegion, standardPt.y);

			if(dStandardMin > dStandard)
				dStandardMin = dStandard;
			if(dStandardMax < dStandard)
				dStandardMax = dStandard;
		}
	}
}

// ----------------------------------------------------------------------------
// "일반식" 의 조건값을 가지고 온다.
bool CLineDraw::GetEnvironment_SimpleLine(CIndicatorInfo* pIndicatorInfo, CPacket* pPacket, const CRect& region, 
		const double& dRealWidth, const int nRealStart, const double& dViewMin, const double& dViewMax, const bool bLog, const bool bInvert, 
		CList<double, double>*& pDataList, POSITION& pos, int& nDrawStart, int& nDrawEnd, int& nArrowIndex, double& dRealMin, double& dRealMax, CPoint& prePt)
{
	if(pPacket == NULL)
		return false;
	pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)	
		return false;

//{{ 2007.05.29 by LYH xscalemanager에서 받은 start index를 바꾸지 않기 위해 막음
	// packet 에 실질적인 그리는 시작점과 비교한다.
//	nDrawStart = pPacket->GetStartPeriod();
//	if(nDrawStart < nRealStart)
//		nDrawStart = nRealStart;
//}}
	// packet 의 data 갯수와 끝점을 비교한다.
	if(nDrawEnd >= pDataList->GetCount())
		nDrawEnd = pDataList->GetCount() -1;

	pos = pDataList->FindIndex(nDrawStart);
	if(pos == NULL)	
		return false;

	dRealMin = CDraw::CalLogValue(bLog, dViewMin);
	dRealMax = CDraw::CalLogValue(bLog, dViewMax);
	prePt = GetPoint(pDataList, region, dRealWidth, dRealMin, dRealMax, pDataList->GetAt(pos), 
		(nDrawStart > nRealStart ? nDrawStart - nRealStart: 0), bLog, bInvert);

	// "가격 이동평균", "거래량 이동평균" 인 경우 화살표를 그린다.
	nArrowIndex = GetArrowIndex(pIndicatorInfo, pPacket->GetName(), nDrawEnd);
	return true;
}

// "가격 이동평균", "거래량 이동평균" 인 경우 화살표를 그린다.
int CLineDraw::GetArrowIndex(CIndicatorInfo* pIndicatorInfo, const CString &p_strSubGraphName, const int nDrawEnd)
{
//	if(pPacket == NULL || !IsDrawArrow(pIndicatorInfo))	
	if(p_strSubGraphName.IsEmpty() || !IsDrawArrow(pIndicatorInfo))	
		return -1;

//	int nArrowIndex = GetConditionDataInName(pIndicatorInfo, pPacket->GetName());
	int nArrowIndex = GetConditionDataInName(pIndicatorInfo, p_strSubGraphName);
	if(nArrowIndex < 0)
		return -1;

	return (nDrawEnd - nArrowIndex +1);
}

bool CLineDraw::IsDrawArrow(CIndicatorInfo* pIndicatorInfo)
{
	if(pIndicatorInfo == NULL)
		return false;

	CList<double, double>* pDrawCondList = pIndicatorInfo->GetDrawConditionList();
	if(pDrawCondList == NULL || pDrawCondList->GetCount() <= 0)
		return false;

	return (pDrawCondList->GetHead() == 1.0);
}

//sy 2005.1.7.
int CLineDraw::GetConditionDataInName(CIndicatorInfo* pIndicatorInfo, const CString& strName)
{
	if(pIndicatorInfo == NULL || strName.IsEmpty())
		return -1;

	// subGraph 부분의 조건값에서 가져오기
	int nIndex = GetPartConditionDataInName(pIndicatorInfo, strName);
	if(nIndex >= 0)
		return nIndex;

	// 공통부분의 조건값에서 가져오기
	return GetCommonConditionDataInName(pIndicatorInfo, strName);
}

int CLineDraw::GetPartConditionDataInName(CIndicatorInfo* pIndicatorInfo, const CString& strName)
{
	if(pIndicatorInfo == NULL || strName.IsEmpty())
		return -1;

	CList<double, double>* pCalCondList = pIndicatorInfo->GetCalConditionList(strName);
	if(pCalCondList == NULL || pCalCondList->GetCount() <= 0)
		return -1;

	return (int)(pCalCondList->GetHead());
}

int CLineDraw::GetCommonConditionDataInName(CIndicatorInfo* pIndicatorInfo, const CString& strName)
{
	if(pIndicatorInfo == NULL || strName.IsEmpty())
		return -1;

	int nSubGraphIndex = pIndicatorInfo->GetSubGraphIndex(strName);
	if(nSubGraphIndex < 0)
		return -1;

	CList<double, double>* pCalCondList = pIndicatorInfo->GetCommonCalConditionList();
	if(pCalCondList == NULL)
		return -1;

	POSITION condPos = pCalCondList->FindIndex(nSubGraphIndex);
	if(condPos == NULL)
		return -1;

	return (int)(pCalCondList->GetAt(condPos));
}

CPoint CLineDraw::GetPoint(CList<double, double>* pDataList, const CRect& region, 
		const double& dRealWidth, const double& dRealMin, const double& dRealMax, 
		const double& dData, const int nIndex, const bool bLog, const bool bInvert)
{
	if(pDataList == NULL)
		return CPoint(0, 0);

	double dValue = CDraw::CalLogValue(bLog, dData);
	CPoint point = CDataConversion::GetVerticalPoint(region, nIndex, dRealWidth, dRealMin, dRealMax, dValue, true);
	if(bInvert)
		point.y = CDataConversion::InvertGraphDataPt(region, point.y);

	return point;
}
