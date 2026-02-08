// SignalDraw.cpp: implementation of the CSignalDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SignalDraw.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "DataMath.h"
#include "Conversion.h"
#include "Draw.h"
#include "BaseData.h"
#include "PacketList.h"
#include "PacketBase.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "XScaleManager.h"										// xScaleManager/복수종목 적용 - xScaleManager - ojtaso (20070404)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// xScaleManager/복수종목 적용 - xScaleManager - ojtaso (20070404)
bool CSignalDraw::DrawSignal(CDC* pDC, 
			const CDrawingGraphData& drawingGraphData,
			const CDisplayAttributes& displayAttributes, 
			CPacket* pPacket, 
			const CList<double, double>* pSignalData,
			CSignalData &p_signalData, 
			const CString& strRQ,
			CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
	{
		// 1. Get the cache size.
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		//		Alloc max memory.
		int nDataCount = pxScaleManager->GetColumnWidth( p_nColumn);
		// (2009/2/26 - Seung-Won, Bae) Check no data.
		if( nDataCount < 1) return true;

		// 2. Check the Region
		CRect graphRegion = drawingGraphData.GetGraphRegions().GetDrawingRegion();
		if( graphRegion.bottom <= graphRegion.top) return false;
		double dViewBottom = (double)graphRegion.bottom;
		double dViewHeight = (double)graphRegion.Height();
		int nViewY_half2 = graphRegion.top + graphRegion.top + dViewHeight;
		CDisplayDataMinMax displayDataMinMax = drawingGraphData.GetDisplayDataVertMinMax();
		double dViewMax = displayDataMinMax.GetDisplayMin();
		double dViewMin = displayDataMinMax.GetDisplayMax();
		bool bLog = drawingGraphData.DoesVertScaleLogType();
		bool bInvert = drawingGraphData.DoesVertScaleInvertType();

		// 1. Check the Range
		int nDTI_Start = -1, nDTI_End = -1;
		// (2010/5/4 - Seung-Won, Bae) Fixed Area of Obvious Glance Balance Chart on X Scale Manager
		BOOL bOEBT = FALSE;
		CIndicatorInfo *pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
		if( pIndicatorInfo) bOEBT = ( pIndicatorInfo->GetIndicatorName() == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART));
		pxScaleManager->GetDrawIndex(strRQ, FALSE, bOEBT, nDTI_Start, nDTI_End);
		if( nDTI_Start == -1) return true;
		if( nDTI_Start < pPacket->GetStartPeriod()) nDTI_Start = pPacket->GetStartPeriod();
		int nDRICount = nDTI_End - nDTI_Start + 1;

		// 2. Check X Scale Mode.
		// the change of Graph Style, Y Min/Max is xScaleMode 2 (Don't ask X Scale Manager !)
		int xScaleMode = 2;
		if(	p_signalData.m_pSignalCache)
		{	
			//Check YValue
			bool bYValue = p_signalData.m_pSignalCache->CheckYValue( dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);
			if(bYValue)
			{					
				if(pxScaleManager) xScaleMode = pxScaleManager->GetCalculationMode();
				if(xScaleMode == 0)	return true;
			}
		}		
		
		if( 2 == xScaleMode)
		{
			// ReAlloc Memory
			// const int nOneLineMode = pxScaleManager->IsAllOnePixel( nColumn, strRQ);
			int nMaxElementCount = nDRICount;
			if( graphRegion.right != graphRegion.left) nMaxElementCount += int( nDRICount / double( graphRegion.right - graphRegion.left) + 1.99);

			// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
			if( p_signalData.m_pSignalCache) p_signalData.m_pSignalCache->ResetValue();
			else							p_signalData.m_pSignalCache = new CCalcDataForDraw;
			p_signalData.m_pSignalCache->Alloc_lpPoint_High( nMaxElementCount * 7);
			p_signalData.m_pSignalCache->Alloc_lpPolyCount_High( nMaxElementCount);
			p_signalData.m_pSignalCache->Alloc_lpPoint_Low( nMaxElementCount* 7);
			p_signalData.m_pSignalCache->Alloc_lpPolyCount_Low( nMaxElementCount);
			p_signalData.m_pSignalCache->Alloc_lpPointBuffer( 16);
		}
		CCalcDataForDraw* pCalcData = p_signalData.m_pSignalCache;
		//YValue Set
		pCalcData->SetYValue( dViewMax, dViewMin, bLog, bInvert, graphRegion, nDataCount);

		if( 2 == xScaleMode)
		{
			//////////////////////////////////////////////////////////////////////
			// [for Dot Shape]
			// pCalcData->lpPointBuffer			: lpPointBuffer[0]		the Center Point of Template.
			//									  lpPointBuffer[1:]		Array of Point for the Element Template.
			//////////////////////////////////////////////////////////////////////
			if( pCalcData->GetSize_lpPointBuffer() < 16) return true;
			POINT &ptUpArrowCenter			= pCalcData->Get_lpPointBuffer()[ 0];
			POINT *ptUpArrow				= pCalcData->Get_lpPointBuffer() + 1;
			POINT &ptDownArrowCenter		= pCalcData->Get_lpPointBuffer()[ 8];
			POINT *ptDownArrow				= pCalcData->Get_lpPointBuffer() + 9;

			ptUpArrowCenter.x = 0;
			ptUpArrowCenter.y = 0;
			ptUpArrow[0].x = ptUpArrowCenter.x;
			ptUpArrow[0].y = ptUpArrowCenter.y + 2;
			ptUpArrow[1].y = ptUpArrow[2].y = ptUpArrow[5].y = ptUpArrow[6].y = ptUpArrow[0].y + 3;
			ptUpArrow[3].y = ptUpArrow[4].y = ptUpArrow[0].y + 8;
			ptUpArrow[1].x = ptUpArrow[0].x - 3;
			ptUpArrow[2].x = ptUpArrow[3].x = ptUpArrow[0].x - 1;
			ptUpArrow[4].x = ptUpArrow[5].x = ptUpArrow[0].x + 1;
			ptUpArrow[6].x = ptUpArrow[0].x + 3;

			ptDownArrowCenter.x = 0;
			ptDownArrowCenter.y = 0;
			ptDownArrow[0].x = ptDownArrowCenter.x;
			ptDownArrow[0].y = ptDownArrowCenter.y - 2;
			ptDownArrow[5].y = ptDownArrow[2].y = ptDownArrow[6].y = ptDownArrow[1].y = ptDownArrow[0].y - 3;
			ptDownArrow[4].y = ptDownArrow[3].y = ptDownArrow[0].y - 8;
			ptDownArrow[1].x = ptDownArrow[0].x - 3;
			ptDownArrow[2].x = ptDownArrow[3].x = ptDownArrow[0].x - 1;
			ptDownArrow[4].x = ptDownArrow[5].x = ptDownArrow[0].x + 1;
			ptDownArrow[6].x = ptDownArrow[0].x + 3;
		}

		// (2009/10/7 - Seung-Won, Bae) draw all Arrow (without near one with one pixel.)
		nDataCount = nDTI_End - nDTI_Start;
		if( nDataCount < 1) return true;

		// 0. Check Data
		if( pPacket == NULL || displayAttributes.GetDataStartInBlock() < 0 || displayAttributes.GetDataCountInBlock() <= 0)
			return false;

		if( !pPacket->GetnumericDataList()) return false;
		if( pSignalData == NULL || pSignalData->GetCount() <= 0)
			return false;

		if( bLog)
		{
			dViewMin = CDraw::CalLogValue(true, dViewMin);
			dViewMax = CDraw::CalLogValue(true, dViewMax);
		}
		double dRealHeight = dViewMin - dViewMax;

		// 3. Loop
		int nZeroDTI = 0;
		CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
		if( !pblaAreafDTI) return false;

		//////////////////////////////////////////////////////////////////////
		// [for Dot Shape]
		// pCalcData->lpPointBuffer			: lpPointBuffer[0]		the Center Point of Template.
		//									  lpPointBuffer[1:]		Array of Point for the Element Template.
		//////////////////////////////////////////////////////////////////////
		if( pCalcData->GetSize_lpPointBuffer() < 16) return true;
		POINT *ptUpArrowCenter			= pCalcData->Get_lpPointBuffer();
		POINT *ptUpArrow				= pCalcData->Get_lpPointBuffer() + 1;
		POINT *ptDownArrowCenter		= pCalcData->Get_lpPointBuffer();
		POINT *ptDownArrow				= pCalcData->Get_lpPointBuffer() + 9;
		if( bInvert)
		{
			POINT *ptArrowCenter		= ptUpArrowCenter;
			POINT *ptArrow				= ptUpArrow;
			ptUpArrowCenter				= ptDownArrowCenter;
			ptUpArrow					= ptDownArrow;
			ptDownArrowCenter			= ptArrowCenter;
			ptDownArrow					= ptArrow;
		}

		int		&nLastDTI				= pCalcData->nDTI_Start;
		int		&nCount_High			= pCalcData->nPolyCount_High;
		int		&nLastPolyCount_High	= pCalcData->nLastPolyCount_High;
		if( pCalcData->GetSize_lpPoint_High() < 1) return true;
		LPPOINT	lpPoint_High			= pCalcData->Get_lpPoint_High();
		int *	lpPolyCount_High		= pCalcData->Get_lpPolyCount_High();
		int		&nCount_Low				= pCalcData->nPolyCount_Low;
		int		&nLastPolyCount_Low		= pCalcData->nLastPolyCount_Low;
		LPPOINT	lpPoint_Low				= pCalcData->Get_lpPoint_Low();
		int *	lpPolyCount_Low			= pCalcData->Get_lpPolyCount_Low();
		int nFullPointCount_High		= 0;
		int nFullPointCount_Low			= 0;

		// for X Scale Mode 1
		if( xScaleMode == 1)
		{
			nDTI_Start				= nLastDTI;
			nCount_High				= nLastPolyCount_High;
			nCount_Low				= nLastPolyCount_Low;
			nFullPointCount_High	= nCount_High * 7;
			nFullPointCount_Low		= nCount_Low * 7;
		}

		POSITION posValue = pPacket->GetnumericDataList()->FindIndex( nDTI_Start);
		if( posValue == NULL)	return false;
		POSITION posSignal = pSignalData->FindIndex( nDTI_Start);
		if( posSignal == NULL) return false;
		CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
		pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

		int nX = -1;
		CXScaleManager::PAREA parea = NULL;
		for( int nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
		{
			parea = pblaAreafDTI->GetNext( dblCursor);
			if( posValue == NULL || parea == NULL || posSignal == NULL) break;

			// (2009/2/27 - Seung-Won, Bae) Backup for XScaleMode 1
			if( nX != parea->center)
			{
				nX = parea->center;
				nLastDTI			= nDTI;
				nLastPolyCount_High	= nCount_High;
				nLastPolyCount_Low	= nCount_Low;
			}

			// 3.1 Check the Signal value
			double dValue = pPacket->GetnumericDataList()->GetNext( posValue);
			// (2010/4/18 - Seung-Won, Bae) Show Signal Arrow with masking on Only Both Sub Graph are shown.
			double bSignal = pSignalData->GetNext(posSignal);		
			if( bSignal == CGraphBaseData::NONE) continue;

			// 3.2 Get the position.
			if( bLog) dValue = CMath::Log( dValue);
			dValue = dViewBottom - ( dViewHeight * (( dValue - dViewMax) / dRealHeight));
			dValue = ROUND( dValue);
			if( bInvert) dValue = nViewY_half2 - dValue;

			if( bSignal == CGraphBaseData::BUY)
				DrawElement_AddShapePolygon( lpPoint_High, lpPolyCount_High, nCount_High, nFullPointCount_High,
									nX, dValue, ptUpArrow, *ptUpArrowCenter, 7);
			else
				DrawElement_AddShapePolygon( lpPoint_Low, lpPolyCount_Low, nCount_Low, nFullPointCount_Low,
									nX, dValue, ptDownArrow, *ptDownArrowCenter, 7);
		}
	}
	else if(pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
	{
		if( !p_signalData.m_pSignalCache) return true;

		CCalcDataForDraw* pCalcData = p_signalData.m_pSignalCache;

		int &nCountOfPoly_High		= pCalcData->nPolyCount_High;
		POINT *ptPolyPoints_High	= pCalcData->Get_lpPoint_High();
		int *nPointCounts_High		= pCalcData->Get_lpPolyCount_High();
		int &nCountOfPoly_Low		= pCalcData->nPolyCount_Low;
		POINT *ptPolyPoints_Low		= pCalcData->Get_lpPoint_Low();
		int *nPointCounts_Low		= pCalcData->Get_lpPolyCount_Low();

		if( 0 < nCountOfPoly_High)
			DrawElement_DrawPolygon( pDC, 0, nCountOfPoly_High,		ptPolyPoints_High,	NULL,	nPointCounts_High,	1, RGB( 255, 0, 0), TRUE);
		if( 0 < nCountOfPoly_Low)
			DrawElement_DrawPolygon( pDC, 0, nCountOfPoly_Low,		ptPolyPoints_Low,	NULL,	nPointCounts_Low,	1, RGB( 0, 0, 255), TRUE);
	}
	return true;
}
