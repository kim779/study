// VolumeScaleDrawer.cpp: implementation of the CVolumeScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VolumeScaleDrawer.h"

#include "../Include_Chart/DLL_Load/PacketBase.h"		// for CPacket
#include "ScaleRegion.h"
#include "BaseData.h"
#include "Conversion.h"
#include "DataMath.h"

/////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////////////
// class CVolumeScaleDrawer
// protected ========================================================================


/////////////////////////////////////////////////////////////////////////////////////
// class CVolumeVertScaleDrawer

CVolumeVertScaleDrawer::CVolumeVertScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CVertScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid를 그린다.
void CVolumeVertScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;

	CString dataType = CDrawer::GetPacketDataType(pPacket);
	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;
	if(!GetDrawingEnvironment(pDC, scaleRegion.GetScaleTextRegion(), drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;

	// 절사 할 자리수 구하기 - 정확한 타입 다시 쓰기 12.24.2001
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans(drawingVertScaleData.GetDisplayDataMinMax(), dataType, bAutoScaleType);
	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());

	// type 쓰기
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleRegion.GetScaleTextRegion(), strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale 쓰기
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), typeRegionY, 
		start, end, dGap, bround, drawingVertScaleData, vertScale,
		scaleRegion.GetScaleTextRegion(), CRect(0, 0, 0, 0), dataType, strFormatTrans, true);
}

// 양쪽
void CVolumeVertScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;

	CRect scaleTextRegion_left = vertScaleRegion.GetLeftScaleTextRegion();
	CRect scaleTextRegion_right = vertScaleRegion.GetRightScaleTextRegion();

	CString dataType = CDrawer::GetPacketDataType( pPacket);
	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;
	if(!GetDrawingEnvironment(pDC, scaleTextRegion_left, drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;

	// 절사 할 자리수 구하기 - 정확한 타입 다시 쓰기 12.24.2001
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans(drawingVertScaleData.GetDisplayDataMinMax(), dataType, bAutoScaleType);
	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());

	// type 쓰기
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleTextRegion_left, strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());
	typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleTextRegion_right, strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale 쓰기
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		typeRegionY, start, end, dGap, bround, drawingVertScaleData, vertScale,
		scaleTextRegion_left, scaleTextRegion_right, dataType, strFormatTrans, true);
}

// private ==========================================================================

