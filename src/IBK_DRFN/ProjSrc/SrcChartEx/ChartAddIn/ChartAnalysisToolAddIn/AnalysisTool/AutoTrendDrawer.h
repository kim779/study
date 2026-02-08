// AutoTrendDrawer.h: interface for the CAutoTrendDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOTRENDDRAWER_H__20D74A39_EB74_4491_B6E2_82C746AF01EF__INCLUDED_)
#define AFX_AUTOTRENDDRAWER_H__20D74A39_EB74_4491_B6E2_82C746AF01EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Drawer.h"

class CCoordinateCalculatorData;
class CAutoTrendDrawingCalculatorData;
class CRegion;

class CAutoTrendDrawer : public CDrawer
{
public:
	CAutoTrendDrawer();

	void SetPenColor(const COLORREF& color);
	void SetPenThickness(const int nThickness);

	COLORREF GetPenColor() const;
	int GetPenThickness() const;

	bool Drawing(CDC* pDC, const CAutoTrendDrawingCalculatorData& calData, const CPoint& point, const int nDrawMode);

private:
	COLORREF m_penColor;
	int m_nPenThickness;

	void DrawShape(CDC* pDC, const CAutoTrendDrawingCalculatorData& calData, const CPoint& point);
	void DrawLine(CDC* pDC, const CRegion& region, const int nCurrentIndex, const CPoint& leftPt, const CPoint& rightPt, CPoint& realRightPt);
	void DrawText(CDC* pDC, const CAutoTrendDrawingCalculatorData& calData, const int nCurrentDataIndex, 
		const CPoint& highPt, const CPoint& lowPt, const CPoint& realHighPt, const CPoint& realLowPt);

	CRegion GetRegion(const CCoordinateCalculatorData& calData) const;
	CPoint GetPoint(const CRegion& region, const int nDataIndex, const CPoint& line1_pt1, const CPoint& line1_pt2) const;
	CRect GetTextRegion(const CSize& size, const CRect& noFrameGraphRegion) const;
	CString GetText(const CAutoTrendDrawingCalculatorData& calData, 
		const int nCurrentDataIndex, const CPoint& highPt, const CPoint& lowPt) const;
	//void GetTextFont(CDC* pDC, CFont& font) const;
};

#endif // !defined(AFX_AUTOTRENDDRAWER_H__20D74A39_EB74_4491_B6E2_82C746AF01EF__INCLUDED_)
