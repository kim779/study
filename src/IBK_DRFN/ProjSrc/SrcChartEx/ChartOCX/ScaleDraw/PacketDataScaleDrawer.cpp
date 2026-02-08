// PacketDataScaleDrawer.cpp: implementation of the CPacketDataScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketDataScaleDrawer.h"

#include "ScaleRegion.h"
#include "BaseData.h"
#include "Drawer.h"
#include "Conversion.h"
#include "DataMath.h"

// packet.dll
#include "PacketBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CPacketDataScaleDrawer

CPacketDataScaleDrawer::CPacketDataScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CVertScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid를 그린다.
void CPacketDataScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;
	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;
	//sy 2002.8.28. --> gap에 따른 packetDataType 수정
	CString dataType = CDrawer::GetPacketDataType( pPacket);

	if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, scaleRegion.GetScaleTextRegion(), 
		drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;

	// 절사 할 자리수 구하기 - 정확한 타입 다시 쓰기 12.24.2001
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans(drawingVertScaleData.GetDisplayDataMinMax(), dataType, bAutoScaleType);
	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());

	// type 쓰기
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleRegion.GetScaleTextRegion(), strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale 쓰기
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		typeRegionY, start, end, dGap, bround, drawingVertScaleData, vertScale, 
		scaleRegion.GetScaleTextRegion(), CRect(0, 0, 0, 0), dataType, strFormatTrans, IsSingDraw());
}

// 양쪽
void CPacketDataScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;

	CRect scaleTextRegion_left = vertScaleRegion.GetLeftScaleTextRegion();
	CRect scaleTextRegion_right = vertScaleRegion.GetRightScaleTextRegion();

	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;
	//sy 2002.8.28. --> gap에 따른 packetDataType 수정
	CString dataType = CDrawer::GetPacketDataType( pPacket);

	if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, scaleTextRegion_left, 
		drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
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
		scaleTextRegion_left, scaleTextRegion_right, dataType, strFormatTrans, IsSingDraw());
}


// protected =========================================================
bool CPacketDataScaleDrawer::IsSingDraw() const
{
	return true;
}


//////////////////////////////////////////////////////////////////////
// class CPercentFullScaleDrawer

CPercentFullScaleDrawer::CPercentFullScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CVertScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid를 그린다.
void CPercentFullScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	CString dataType = "%";
	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;
	if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, scaleRegion.GetScaleTextRegion(), drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;

	// type 쓰기
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleRegion.GetScaleTextRegion(), dataType, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale 쓰기
	// 0 단위로 자르기 위해서...
	double dOrgY = CMath::Round(CMath::Round_Double(drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin()), 1);

	CVertScaleDrawer::DrawTextOrGrid(pDC, dOrgY, 
		typeRegionY, start, end, dGap, bround, drawingVertScaleData, vertScale, 
		scaleRegion.GetScaleTextRegion(), CRect(0, 0, 0, 0), dataType, dataType, true);
}

// 양쪽
void CPercentFullScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	CRect scaleTextRegion_left = vertScaleRegion.GetLeftScaleTextRegion();
	CRect scaleTextRegion_right = vertScaleRegion.GetRightScaleTextRegion();

	CString dataType = "%";
	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;
	if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, scaleTextRegion_left, drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;

	// type 쓰기
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleTextRegion_left, dataType, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());
	typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleTextRegion_right, dataType, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale 쓰기
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		typeRegionY, start, end, dGap, bround, drawingVertScaleData, vertScale, 
		scaleTextRegion_left, scaleTextRegion_right, dataType, dataType, true);
}

//////////////////////////////////////////////////////////////////////
// class CTopButtomPlusScaleDrawer

CTopButtomPlusScaleDrawer::CTopButtomPlusScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CPacketDataScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// protected =========================================================
bool CTopButtomPlusScaleDrawer::IsSingDraw() const
{
	return false;
}
