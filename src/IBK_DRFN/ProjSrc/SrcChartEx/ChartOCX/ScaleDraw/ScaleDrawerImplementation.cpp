// ScaleDrawerImplementation.cpp: implementation of the CScaleDrawerImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleDrawerImplementation.h"

#include "ScaleRegion.h"
#include "BaseData.h"
#include "HorzScaleDrawer.h"
#include "ScaleDrawerSingleton.h"

#include "Conversion.h"
#include "DataMath.h"

#include "PacketBase.h"		// PacketList.dll
#include "XScaleManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CScaleDrawerImplementation

CScaleDrawerImplementation::~CScaleDrawerImplementation()
{
}

// public ===========================================================================
// 가로 scale 그린다.
void CScaleDrawerImplementation::DrawHorzTextOrGrid(CDC* pDC, 
				const CDrawingHorzScaleData& drawingHorzScaleData,
				const CHorzScaleRegion& horzScaleRegion, // 가로 scale의 영역
				const CHorizontalScale& horzScale,		 // 가로 scale에 대한 값
				CXScaleManager* pxScaleManager,
				const char *p_szRQ)
{
	if(drawingHorzScaleData.GetPacket() == NULL)
		return;

	// 위쪽
	if(horzScale.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP){
		DrawHorzTextOrGrid(pDC, drawingHorzScaleData, horzScaleRegion.GetTopScaleRegion(), horzScale, pxScaleManager, p_szRQ);
	}
	// 아래쪽
	else if(horzScale.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTTOM){
		DrawHorzTextOrGrid(pDC, drawingHorzScaleData, horzScaleRegion.GetBottomScaleRegion(), horzScale, pxScaleManager, p_szRQ);
	}
	// 양쪽
	else if(horzScale.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH){
		DrawHorzTextOrGrid_Both(pDC, drawingHorzScaleData, horzScaleRegion, horzScale, pxScaleManager, p_szRQ);
	}
	// text 숨김
	else if(horzScale.GetHorzScalePosition() == CScaleBaseData::HORZ_HIDE){
		DrawHorzGrid(pDC, drawingHorzScaleData, horzScaleRegion, horzScale, pxScaleManager, p_szRQ);
	}
}

// 세로 scale 그린다.
void CScaleDrawerImplementation::DrawVertTextOrGrid(CDC* pDC, 
				const CDrawingVertScaleData& drawingVertScaleData,
				const CVertScaleRegion& vertScaleRegion, // 세로 scale의 영역
				const CVerticalScale& vertScale)		 // 세로 scale에 대한 값
{
	CScaleBaseData::VERTSCALEPOSITION position = vertScale.GetVertScalePosition();
	// (2007/1/1 - Seung-Won, Bae) Manage with Activate
	int nActiveFlag = vertScale.GetActiveFlag();

	// 양쪽
	if( ( nActiveFlag & VSGA_LEFTSCALE) && ( nActiveFlag & VSGA_RIGHTSCALE))
		if(position == CScaleBaseData::VERT_BOTH)
	{
		DrawVertTextOrGrid_Both( pDC, drawingVertScaleData, vertScaleRegion, vertScale);
		return;
	}

	// 왼쪽
	if( nActiveFlag & VSGA_LEFTSCALE)
		if( position == CScaleBaseData::VERT_LEFT || position == CScaleBaseData::VERT_BOTH)
	{
		CScaleDrawingRegion scaleDrawingRegion(vertScaleRegion.GetLeftScaleTextRegion(), vertScaleRegion.GetLeftScaleGridRegion());
		DrawVertTextOrGrid(pDC, drawingVertScaleData, scaleDrawingRegion, vertScale);
		return;
	}

	// 오른쪽
	if( nActiveFlag & VSGA_RIGHTSCALE)
		if(position == CScaleBaseData::VERT_RIGHT || position == CScaleBaseData::VERT_BOTH)
	{
		CScaleDrawingRegion scaleDrawingRegion(vertScaleRegion.GetRightScaleTextRegion(), vertScaleRegion.GetRightScaleGridRegion());
		DrawVertTextOrGrid(pDC, drawingVertScaleData, scaleDrawingRegion, vertScale);
		return;
	}
}

// private ==========================================================================
// 가로 scale text, grid을 그린다.
void CScaleDrawerImplementation::DrawHorzTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CRect& scaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager,
		const char *p_szRQ)
{
	CHorzScaleDrawerSingleton horzScaleDrawerSingleton( m_hOcxWnd, m_eChartMode);
	CHorzScaleDrawer* pHorzScaleDrawer = horzScaleDrawerSingleton.GetHorzScaleDrawer(drawingHorzScaleData.GetPacket(), drawingHorzScaleData.GetScaleDrawerType());
	//CHorzScaleDrawer* pHorzScaleDrawer = horzScaleDrawerSingleton.GetHorzScaleDrawer(pxScaleManager->GetScaleUnit_Max());
	 
	if(pHorzScaleDrawer == NULL)
		return;

	bool bResult = pHorzScaleDrawer->DrawTextOrGrid(pDC, drawingHorzScaleData, scaleRegion, horzScale, pxScaleManager, p_szRQ);
	if(bResult == false)
	{
		pHorzScaleDrawer->DrawTextOrGrid(pDC, drawingHorzScaleData, scaleRegion, horzScale);
	}
}

// 양쪽
void CScaleDrawerImplementation::DrawHorzTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
{
	CHorzScaleDrawerSingleton horzScaleDrawerSingleton( m_hOcxWnd, m_eChartMode);
	CHorzScaleDrawer* pHorzScaleDrawer = horzScaleDrawerSingleton.GetHorzScaleDrawer(drawingHorzScaleData.GetPacket(), drawingHorzScaleData.GetScaleDrawerType());
//	CHorzScaleDrawer* pHorzScaleDrawer = horzScaleDrawerSingleton.GetHorzScaleDrawer(pxScaleManager->GetScaleUnit_Max());
	if(pHorzScaleDrawer == NULL)
		return;

	bool bResult = pHorzScaleDrawer->DrawTextOrGrid_Both(pDC, drawingHorzScaleData, horzScaleRegion, horzScale, pxScaleManager, p_szRQ);
	if(bResult == false)
	{
		pHorzScaleDrawer->DrawTextOrGrid_Both(pDC, drawingHorzScaleData, horzScaleRegion, horzScale);
	}
}

// text 숨김
void CScaleDrawerImplementation::DrawHorzGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion,
											  const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
{
	CHorzScaleDrawerSingleton horzScaleDrawerSingleton( m_hOcxWnd, m_eChartMode);
	CHorzScaleDrawer* pHorzScaleDrawer = horzScaleDrawerSingleton.GetHorzScaleDrawer(drawingHorzScaleData.GetPacket(), drawingHorzScaleData.GetScaleDrawerType());
//	CHorzScaleDrawer* pHorzScaleDrawer = horzScaleDrawerSingleton.GetHorzScaleDrawer(pxScaleManager->GetScaleUnit_Max());
	if(pHorzScaleDrawer == NULL)
		return;

	bool bResult = pHorzScaleDrawer->DrawGrid(pDC, drawingHorzScaleData, horzScaleRegion, horzScale, pxScaleManager, p_szRQ);
	if(bResult == false)
	{
		pHorzScaleDrawer->DrawGrid(pDC, drawingHorzScaleData, horzScale);
	}
}

// ----------------------------------------------------------------------------------
// 세로 scale text, grid을 그린다.
void CScaleDrawerImplementation::DrawVertTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, 
		const CScaleDrawingRegion& scaleDrawingRegion, const CVerticalScale& vertScale)
{
	CVertScaleDrawerSingleton vertScaleDrawerSingleton( m_hOcxWnd, m_eChartMode);
	CVertScaleDrawer* pVertScaleDrawer = vertScaleDrawerSingleton.GetVertScaleDrawer(drawingVertScaleData.GetScaleDrawerType());
	if(pVertScaleDrawer == NULL)
		return;

	pVertScaleDrawer->DrawTextOrGrid(pDC, drawingVertScaleData, scaleDrawingRegion, vertScale);
}

// 양쪽
void CScaleDrawerImplementation::DrawVertTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, 
		const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	CVertScaleDrawerSingleton vertScaleDrawerSingleton( m_hOcxWnd, m_eChartMode);
	CVertScaleDrawer* pVertScaleDrawer = vertScaleDrawerSingleton.GetVertScaleDrawer(drawingVertScaleData.GetScaleDrawerType());
	if(pVertScaleDrawer == NULL)
		return;

	pVertScaleDrawer->DrawTextOrGrid_Both(pDC, drawingVertScaleData, vertScaleRegion, vertScale);
}
