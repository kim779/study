// ScaleDrawer.cpp: implementation of the CScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleDrawer.h"

#include "ScaleDrawerImplementation.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CScaleDrawer

CScaleDrawer::CScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode) :
	m_pScaleDrawerImpl(NULL)
{
	m_pScaleDrawerImpl = new CScaleDrawerImplementation( p_hOcxWnd, p_eChartMode);
}

CScaleDrawer::~CScaleDrawer()
{
	delete m_pScaleDrawerImpl;
}

// public ===========================================================================
// scale 그린다.
void CScaleDrawer::DrawHorzTextOrGrid(CDC* pDC, 
				const CDrawingHorzScaleData& drawingHorzScaleData,
				const CHorzScaleRegion& horzScaleRegion, // 가로 scale의 영역
				const CHorizontalScale& horzScale,		 // 가로 scale에 대한 값
				CXScaleManager* pxScaleManager,
				const char *p_szRQ)
{
	m_pScaleDrawerImpl->DrawHorzTextOrGrid( pDC, drawingHorzScaleData, horzScaleRegion, horzScale, pxScaleManager, p_szRQ);
}

void CScaleDrawer::DrawVertTextOrGrid(CDC* pDC, 
				const CDrawingVertScaleData& drawingVertScaleData,
				const CVertScaleRegion& vertScaleRegion, // 세로 scale의 영역
				const CVerticalScale& vertScale)		 // 세로 scale에 대한 값
{
	m_pScaleDrawerImpl->DrawVertTextOrGrid(pDC, drawingVertScaleData, vertScaleRegion, vertScale);
}
