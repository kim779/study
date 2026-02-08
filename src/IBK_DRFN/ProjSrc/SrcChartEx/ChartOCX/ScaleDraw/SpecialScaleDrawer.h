// SpecialScaleDrawer.h: interface for the CSpecialScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPECIALSCALEDRAWER_H__C7CA349C_AAAB_4CE3_B8F6_E05B1ECCAEE1__INCLUDED_)
#define AFX_SPECIALSCALEDRAWER_H__C7CA349C_AAAB_4CE3_B8F6_E05B1ECCAEE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PriceScaleDrawer.h"

class CDrawingVertScaleData;
class CScaleDrawingRegion;
class CDisplayDataMinMax;

// "대기매물" scale
class CSaleScaleDrawer : public CPriceScaleDrawer  
{
public:
	CSaleScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

private:
	void DrawTextOrGrid_Volumn(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CRect& scaleTextRegion, const CVerticalScale& vertScale);
	void DrawTextOrGrid_Price(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
//	bool DrawTextOrGrid_PricePacket(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);

	bool GetSaleData(CPacket* pPacket, const int nHeight, int& nTotalCount, double& dOnePtWidth, double& dTotalData);
	int GetDataCount(const CList<double, double>* pDataList) const;
	double GetTotalData(const CList<double, double>* pDataList) const;
	double GetPercentData(const double& dTotalData, const double& dData) const;
};

// "MarketProfile" Scale
class CMarketProfileVertScaleDrawer : public CPriceScaleDrawer  
{
public:
	CMarketProfileVertScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

private:
	CString GetPacketType(CPacket* pPacket) const;
};

#endif // !defined(AFX_SPECIALSCALEDRAWER_H__C7CA349C_AAAB_4CE3_B8F6_E05B1ECCAEE1__INCLUDED_)
