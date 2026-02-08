// LogScaleDrawer.h: interface for the CLogScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSCALEDRAWER_H__ACE76613_5652_494C_9991_DEE81A596454__INCLUDED_)
#define AFX_LOGSCALEDRAWER_H__ACE76613_5652_494C_9991_DEE81A596454__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VertScaleDrawer.h"

class CDrawingVertScaleData;
class CScaleDrawingRegion;
class CDisplayDataMinMax;

class CLogScaleDrawer : public CVertScaleDrawer  
{
public:
	CLogScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

private:
	void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CRect& textRegion1, const CRect& textRegion2, const CVerticalScale& vertScale);
};

#endif // !defined(AFX_LOGSCALEDRAWER_H__ACE76613_5652_494C_9991_DEE81A596454__INCLUDED_)
