// PriceScaleDrawer.h: interface for the CPriceScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRICESCALEDRAWER_H__5655406E_6E44_4D9B_992F_6BDE16A86AA7__INCLUDED_)
#define AFX_PRICESCALEDRAWER_H__5655406E_6E44_4D9B_992F_6BDE16A86AA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VertScaleDrawer.h"

class CDrawingVertScaleData;
class CScaleDrawingRegion;
class CDisplayDataMinMax;

class CPriceScaleDrawer : public CVertScaleDrawer
{
public:
	CPriceScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

protected:
	void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale, const CString& dataType);
	void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale, const CString& dataType);
	//2007.01.22 Scale사용자 설정,호가단위별
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	bool GetPriceDrawingEnvironment(const CDisplayDataMinMax& displayDataMinMax, const CString& strPacketType, 
		const int totalCount, double& dGap, int& start, int& end, bool& bround, const CVerticalScale& vertScale,
		const double dHogaUnit);
};

#endif // !defined(AFX_PRICESCALEDRAWER_H__5655406E_6E44_4D9B_992F_6BDE16A86AA7__INCLUDED_)
