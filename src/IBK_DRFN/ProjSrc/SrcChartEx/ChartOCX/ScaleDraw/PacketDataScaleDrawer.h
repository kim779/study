// PacketDataScaleDrawer.h: interface for the CPacketDataScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETDATASCALEDRAWER_H__F928521A_50E5_41DA_9C34_076BF0925986__INCLUDED_)
#define AFX_PACKETDATASCALEDRAWER_H__F928521A_50E5_41DA_9C34_076BF0925986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VertScaleDrawer.h"

class CDrawingVertScaleData;
class CScaleDrawingRegion;
class CDisplayDataMinMax;

// 일반 데이타
class CPacketDataScaleDrawer : public CVertScaleDrawer  
{
public:
	CPacketDataScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

protected:
	virtual bool IsSingDraw() const;
};

// 백분율(0%~100% 고정)
class CPercentFullScaleDrawer : public CVertScaleDrawer  
{
public:
	CPercentFullScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);
};

// +상하식
class CTopButtomPlusScaleDrawer : public CPacketDataScaleDrawer  
{
public:
	CTopButtomPlusScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

protected:
	virtual bool IsSingDraw() const;
};

#endif // !defined(AFX_PACKETDATASCALEDRAWER_H__F928521A_50E5_41DA_9C34_076BF0925986__INCLUDED_)
