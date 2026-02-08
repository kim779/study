// LogScaleDrawer.cpp: implementation of the CLogScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogScaleDrawer.h"

#include "ScaleRegion.h"
#include "BaseData.h"
#include "Conversion.h"
#include "DataMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CLogScaleDrawer

CLogScaleDrawer::CLogScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CVertScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid를 그린다.
void CLogScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid(pDC, drawingVertScaleData, scaleRegion.GetScaleTextRegion(), CRect(0, 0, 0, 0), vertScale);
}

// 양쪽
void CLogScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid(pDC, drawingVertScaleData, vertScaleRegion.GetLeftScaleTextRegion(), vertScaleRegion.GetRightScaleTextRegion(), vertScale);
}

// private ==============================================================================
void CLogScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CRect& textRegion1, const CRect& textRegion2, const CVerticalScale& vertScale)
{
	double dGap = 0.0;
	int start = 0, end = 0;
	bool bround = false;
	CString dataType = CDrawer::GetPacketDataType(drawingVertScaleData.GetPacket());
	if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, textRegion1, drawingVertScaleData.GetDisplayDataMinMax(), 
		dataType, start, end, dGap, bround))
		return;

	int nRealStart = start, nRealEnd = end;
	if(start * end < 0)	{
		nRealStart = 0;
		nRealEnd = abs(start - end);
	}

	// type 쓰기
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, textRegion1, dataType, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());
	typeRegionY = CVertScaleDrawer::DrawType(pDC, textRegion2, dataType, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// invert-scale인지 확인한다
	bool bInvert = drawingVertScaleData.GetVertScaleInvertType();

	// 절사 할 자리수 구하기
	int nroundlength = 0;
	if(!dataType.IsEmpty() && dataType.Find(".") == -1){
		nroundlength = CDataConversion::GetStringDataCount(dataType, "0");
	}
	int nRoundIndex = CVertScaleDrawer::GetRoundIndexDecimalDown(dataType);

	CVertScaleDrawer::DrawScale(pDC, textRegion1, dGap, 
		drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		nRealStart, nRealEnd, drawingVertScaleData);
	CVertScaleDrawer::DrawScale(pDC, textRegion2, dGap, 
		drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		nRealStart, nRealEnd, drawingVertScaleData);

	// scale 쓰기
	double y = drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin();
	bool bIsLog = true;
	if(CDrawer::GetPacketDataCount(drawingVertScaleData.GetPacket()) <= 0)
		bIsLog = false;

	CString strOrgData;
	for(int i = nRealStart ; i < nRealEnd; i++){
		double value = CMath::Round((y + dGap * (double)i), nRoundIndex);
		int nowPt_y = 0;
		if(!CVertScaleDrawer::GetVertScaleAbsolutePosition_y(textRegion1, value, 
			drawingVertScaleData.GetDisplayDataMinMax(), bIsLog, nowPt_y))
			continue;

		CString strData = CVertScaleDrawer::GetStrData(value, dataType, bround, nroundlength);
		if(bround && strData.Find(".") > 0)
			strData = strData.Left(strData.Find("."));
		
		// sy 2003.01.09.
		if(strData.IsEmpty() || strData == strOrgData)
			continue;

		strOrgData = strData;
		CSize textSize = pDC->GetTextExtent(strData);
		int pt_y = nowPt_y;
		// invert이면 뒤집는다
		if(bInvert)
			pt_y = CDataConversion::InvertGraphDataPt(textRegion1, nowPt_y);

		CVertScaleDrawer::DrawTextOut(pDC, 
							CVertScaleDrawer::GetTextPosition(textRegion1.right, pt_y, textSize), 
							strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
		if(textRegion2.IsRectEmpty() == FALSE){
			CVertScaleDrawer::DrawTextOut(pDC, 
								CVertScaleDrawer::GetTextPosition(textRegion2.right, pt_y, textSize), 
								strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
		}
		CVertScaleDrawer::DrawGrid(pDC, drawingVertScaleData, vertScale.GetGridType(), pt_y, vertScale.GetActiveFlag());
	}
}