// TextScaleDrawer.h: interface for the CTextScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTSCALEDRAWER_H__6A8DD297_8CC2_4941_AF6F_8B90B1F27144__INCLUDED_)
#define AFX_TEXTSCALEDRAWER_H__6A8DD297_8CC2_4941_AF6F_8B90B1F27144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HorzScaleDrawer.h"
#include "VertScaleDrawer.h"

class CScaleDrawingRegion;
class CDrawingHorzScaleData;
class CDrawingVertScaleData;

// <text> Type의 가로 scale 그리기
class CTextHorzScaleDrawer : public CHorzScaleDrawer
{
public:
	CTextHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale);

private:
	// 가로형 쓰기
	void DrawTextAndCompartLine(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth, 
		const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos);
	// 세로형 쓰기
	void DrawTextAndCompartLine_VERT(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth, 
		const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos);
	// 중앙형 쓰기(가로)
	void DrawTextAndCompartLine_CenterHorz(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth,
		const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos);
	
	bool GetChangeTextData(const CScaleBaseData::TEXTCOMPARTTYPE eTextCompart, CList<CString, CString>* pData, POSITION& pos, CString& strData) const;
	// scale text의 그리는 위치
	int GetTextPosition_x(const int org_x, const double& textRegionWidth, const int textWidth, const bool isCenter = true) const;
	int GetTextCenterPosition_x(const int textPosition, const int textWidth) const;

// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
protected:
	virtual bool DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ);
	virtual bool DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ);
};

// <text> Type의 세로 scale 그리기
class CTextVertScaleDrawer : public CVertScaleDrawer
{
public:
	CTextVertScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

private:
	void DrawTextOrGrid(CDC* pDC, CPacket* pPacket, const CRect& region1, const CRect& region2, const COLORREF& textColor);

};

#endif // !defined(AFX_TEXTSCALEDRAWER_H__6A8DD297_8CC2_4941_AF6F_8B90B1F27144__INCLUDED_)
