// ScaleAndGraphDrawer.h: interface for the CScaleAndGraphDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEANDGRAPHDRAWER_H__29B02BC2_7D46_461E_98B6_E6BF0523D030__INCLUDED_)
#define AFX_SCALEANDGRAPHDRAWER_H__29B02BC2_7D46_461E_98B6_E6BF0523D030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseData.h"

class CDrawingHorzScaleData;
class CDrawingVertScaleData;
class CHorzScaleRegion;
class CVertScaleRegion;
class CHorizontalScale;
class CVerticalScale;
class CXScaleManager;

class CBlockScaleDrawer
{
public:
	// RQ별 Unit정보 관리 : 복수종목 - ojtaso (20070601)
	void DrawHorzScale(CDC* pDC, CDrawingHorzScaleData* pDrawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, LPCTSTR lpszRQ, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	void DrawVertScale(CDC* pDC, CDrawingVertScaleData* pDrawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

private:
	// RQ별 Unit정보 관리 : 복수종목 - ojtaso (20070601)
	void GetDrawingHorzScaleData(CDrawingHorzScaleData* pDrawingHorzScaleData, const CString& scaleData, CXScaleManager* pxScaleManager, LPCTSTR lpszRQ) const;
	CScaleBaseData::HORZSCALEDRAWERTYPE GetHorzScaleDrawerType(CDrawingHorzScaleData* pDrawingHorzScaleData, const CString& scaleData, CXScaleManager* pxScaleManager, LPCTSTR lpszRQ) const;
	void GetDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData, const CString& scaleData) const;
	CScaleBaseData::VERTSCALEDRAWERTYPE GetVertScaleDrawerType( const CString& scalePacketName, const CString &p_strIndicatorName) const;
	CScaleBaseData::VERTSCALEDRAWERTYPE GetVertScaleDrawerType( const CString &p_strIndicatorName) const;

};

#endif // !defined(AFX_SCALEANDGRAPHDRAWER_H__29B02BC2_7D46_461E_98B6_E6BF0523D030__INCLUDED_)
