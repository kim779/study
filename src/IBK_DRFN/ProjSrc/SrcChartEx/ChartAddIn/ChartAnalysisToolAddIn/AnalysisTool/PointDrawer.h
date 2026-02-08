// PointDrawer.h: interface for the CPointDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTDRAWER_H__9664F79D_D0A2_47B3_ACDE_1B0EFFBA2330__INCLUDED_)
#define AFX_POINTDRAWER_H__9664F79D_D0A2_47B3_ACDE_1B0EFFBA2330__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Drawer.h"
class COnePointDrawingData;
class CTwoPointDrawingData;
class CThreePointDrawingData;
class CMultiPointDrawingData;
class CDisplayData;
class CBlockRegion;

// 그릴 좌표가 1개인 경우
class COnePointDrawer : virtual public CDrawer
{
public:
	void DrawTool(CDC* pDC, const COnePointDrawingData& drawingData, const int drawMode = R2_NOT);
	void DrawElement(CDC* pDC, const COnePointDrawingData& drawingData, const int drawMode = R2_COPYPEN);
	void DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CPoint& point);

protected:
	virtual bool IsDrawing(const CRect& region, const CPoint& point) const = 0;
	// 도형을 그리는 함수
	// -> fiboTimeDrawer 에서 좌표를 구하기 때문에 CRect에서 CDisplayData로 변경함.
	// 2008.08.20 김진순 피보나치 확장	COnePointDrawingData 정보를 이용하여 피보계수 구하기 때문에 수정<<
	//virtual void DrawShape(CDC* pDC, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion) = 0;
	virtual void DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion) = 0;
	//>>
	virtual void DrawShadow(CDC* pDC, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion);
	virtual void DrawText(CDC* pDC, const COnePointDrawingData& drawingData);
	virtual CRect GetGripRegion(const CRect& region, const CPoint& point) = 0;

private:
	void DrawShapeAndSetMode(CDC* pDC, const COnePointDrawingData& drawingData, const int drawMode);
	void DrawTextAndSetMode(CDC* pDC, const COnePointDrawingData& drawingData);
	void DrawGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CPoint& point);
};

// 그릴 좌표가 2개인 경우
class CTwoPointDrawer : virtual public CDrawer
{
public:
	void DrawTool(CDC* pDC,const CTwoPointDrawingData& drawingData, const int drawMode = R2_NOT);
	void DrawElement(CDC* pDC,const CTwoPointDrawingData& drawingData, const int drawMode = R2_COPYPEN);
	void DrawShadowAndGrip(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);

protected:
	virtual bool IsDrawing(const CRect& region, const CPoint& pt1, const CPoint& pt2) const;
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2) = 0;
	virtual void DrawShadow(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);

private:
	void DrawShaph_Tool(CDC* pDC, const CTwoPointDrawingData& drawingData);
	void DrawShaph_Element(CDC* pDC,const CTwoPointDrawingData& drawingData);
	void DrawTextAndSetMode(CDC* pDC, const CTwoPointDrawingData& drawingData);
	void DrawGrip(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
};

// 그릴 좌표가 3개인 경우
class CThreePointDrawer : public CDrawer
{
public:
	void DrawTool(CDC* pDC, const CThreePointDrawingData& drawingData, const int drawMode = R2_NOT);
	void DrawElement(CDC* pDC, const CThreePointDrawingData& drawingData, const int drawMode = R2_COPYPEN);
	void DrawShadowAndGrip(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3);

protected:
	virtual void DrawTool(CDC* pDC, const CRegion& region, const CPoints& points);
	virtual void DrawTool(CDC* pDC, const CRegion& region, const CPoints& points1, const CPoints& points2);

	virtual void DrawShape(CDC* pDC, const CRegion& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3) = 0;
private:
	void DrawShaph_Tool(CDC* pDC, const CThreePointDrawingData& drawingData);
	void DrawShaph_Element(CDC* pDC, const CThreePointDrawingData& drawingData);
	void DrawGrip(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3);

	void DrawCross(CDC* pDC, const CPoint& centerPt);
};

// 다중 Drawing
class CMultiPointDrawer : public CDrawer
{
public:
	void DrawTool(CDC* pDC, const CMultiPointDrawingData& drawingData, const int drawMode = R2_NOT);
	void DrawElement(CDC* pDC, const CMultiPointDrawingData& drawingData, const int drawMode = R2_COPYPEN);
	void DrawShadowAndGrip(CDC* pDC, const CRect& region, const CPoint& pt);

protected:
	virtual void DrawTool(CDC* pDC, const CRegion& region, const CPoints& points);
	virtual void DrawTool(CDC* pDC, const CRegion& region, const CPoints& points1, const CPoints& points2);

	virtual void DrawShape(CDC* pDC,const CMultiPointDrawingData& drawingData) = 0;
private:
	void DrawShaph_Tool(CDC* pDC, const CMultiPointDrawingData& drawingData);
	void DrawShaph_Element(CDC* pDC, const CMultiPointDrawingData& drawingData);
	void DrawGrip(CDC* pDC, const CRect& region, const CPoint& pt);
};

// ----------------------------------------------------------------------------
// 수직선 그리기
class CVertLineDrawer : virtual public COnePointDrawer
{
public:
	CVertLineDrawer();

protected:
	virtual bool IsDrawing(const CRect& region, const CPoint& point) const;
	virtual void DrawText(CDC* pDC, const COnePointDrawingData& drawingData);
	virtual CRect GetGripRegion(const CRect& region, const CPoint& point);

	void DrawShape_Line(CDC* pDC, const CRect& region, const CPoint& point);	
	// 2008.08.20 김진순 피보나치 확장	COnePointDrawingData 정보를 이용하여 피보계수 구하기 때문에 수정<<
	//virtual void DrawShape(CDC* pDC, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion);
	virtual void DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion);
	//>>
	CRect GetTextRegion(const CBlockRegion& blockRegion, const CSize& textSize, const int nX, const bool bTop) const;

private:
	CRect GetTextRegion(CDC* pDC, const CString& textData, const COnePointDrawingData& drawingData, const bool bTop) const;
	CRect GetTextRegion_Top(const CBlockRegion& blockRegion, const CSize& textSize) const;
	CRect GetTextRegion_Bottom(const CBlockRegion& blockRegion, const CSize& textSize) const;

private:
	void DrawText(CDC* pDC, const CString& textData, const COnePointDrawingData& drawingData, const bool bTop);

private:
	const COLORREF TEXT_COLOR;
	const COLORREF TEXT_BK_COLOR;
};

#endif // !defined(AFX_POINTDRAWER_H__9664F79D_D0A2_47B3_ACDE_1B0EFFBA2330__INCLUDED_)
