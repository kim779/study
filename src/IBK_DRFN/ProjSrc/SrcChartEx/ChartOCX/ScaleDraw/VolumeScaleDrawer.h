// VolumeScaleDrawer.h: interface for the CVolumeScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOLUMESCALEDRAWER_H__E5D6F23F_24C3_432B_86C7_D8EA23130A4F__INCLUDED_)
#define AFX_VOLUMESCALEDRAWER_H__E5D6F23F_24C3_432B_86C7_D8EA23130A4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VertScaleDrawer.h"

class CDrawingVertScaleData;
class CDisplayDataMinMax;
class CVertScaleRegion;


// "거래량" => 세로
class CVolumeVertScaleDrawer : public CVertScaleDrawer
{
public:
	CVolumeVertScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

};

#endif // !defined(AFX_VOLUMESCALEDRAWER_H__E5D6F23F_24C3_432B_86C7_D8EA23130A4F__INCLUDED_)
