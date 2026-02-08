// PointDrawer.cpp: implementation of the CPointDrawer class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PointDrawer.h"

#include "DrawingData.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class COnePointDrawer

void COnePointDrawer::DrawTool(CDC* pDC, const COnePointDrawingData& drawingData, const int drawMode)
{
	if(!drawingData.IsDrawing())
		return;

	DrawShapeAndSetMode(pDC, drawingData, drawMode);	
	DrawTextAndSetMode(pDC, drawingData);
}

void COnePointDrawer::DrawElement(CDC* pDC, const COnePointDrawingData& drawingData, const int drawMode)
{
	if(!drawingData.IsDrawing())
		return;

	DrawShapeAndSetMode(pDC, drawingData, drawMode);
	DrawTextAndSetMode(pDC, drawingData);
}

void COnePointDrawer::DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CPoint& point)
{
	DrawGrip(pDC, drawingRegions, point);
}

// protected ==================================================================
void COnePointDrawer::DrawShadow(CDC* /*pDC*/, const CDisplayData& /*displayData*/, const CPoint& /*point*/, const bool /*isCurrentRegion*/)
{
}

void COnePointDrawer::DrawText(CDC* /*pDC*/, const COnePointDrawingData& /*drawingData*/)
{
}

// private ====================================================================
void COnePointDrawer::DrawShapeAndSetMode(CDC* pDC, const COnePointDrawingData& drawingData, const int drawMode)
{
	int oldDrawMode = pDC->SetROP2(drawMode);
	for(int nIndex = 0; nIndex < drawingData.GetDrawingRegions()->GetCount(); nIndex++)
	{
		CGraphPartRegion graphPartRegion = drawingData.GetDrawingRegions()->GetGraphPartRegion(nIndex);
		CDisplayData displayData(drawingData.GetDataType().GetCoordinateType(), 
			graphPartRegion, drawingData.GetDisplayAttributes());

		CDrawer::SelectClipRegion(pDC, graphPartRegion.GetFull());
		// 2008.08.20 김진순 피보나치 확장	COnePointDrawingData 정보를 이용하여 피보계수 구하기 때문에 수정<<
		//DrawShape(pDC, displayData, drawingData.GetDrawingPoint(), 
		//	(drawingData.GetDrawingRegions()->GetBlockRegionList().GetCurrentNumber() == nIndex));
		DrawShape(pDC, drawingData, displayData, drawingData.GetDrawingPoint(), 
			(drawingData.GetDrawingRegions()->GetBlockRegionList().GetCurrentNumber() == nIndex));
		//>>
		CDrawer::SelectClipRegion(pDC);
	}
	pDC->SetROP2(oldDrawMode);
}

void COnePointDrawer::DrawTextAndSetMode(CDC* pDC, const COnePointDrawingData& drawingData)
{
	CDrawer::SelectNewTextMode(pDC, drawingData.GetTextColor());

	DrawText(pDC, drawingData);

	CDrawer::SelectOrgTextMode(pDC);
}

void COnePointDrawer::DrawGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CPoint& point)
{
	CBrush newBrush(CDrawer::GetGripBrushColor());
	CBrush* pOldBlush = pDC->SelectObject(&newBrush);

	for(int nIndex = 0; nIndex < drawingRegions.GetCount(); nIndex++)
	{
		CRect region = drawingRegions.GetFullGraphRegion(nIndex);
		CDrawer::SelectClipRegion(pDC, region);
		CDrawer::DrawRectangle(pDC, GetGripRegion(region, point));
		CDrawer::SelectClipRegion(pDC);
	}

	pDC->SelectObject(pOldBlush);
	newBrush.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// class CTwoPointDrawer

void CTwoPointDrawer::DrawTool(CDC* pDC, const CTwoPointDrawingData& drawingData, const int drawMode)
{
	if(!drawingData.IsDrawing())
		return;

	CDrawer::SelectNewDrawingMode(pDC, drawingData.GetDiaplayRegion(), drawMode);

	DrawShaph_Tool(pDC, drawingData);

	CDrawer::SelectOrgDrawingMode(pDC);
}

void CTwoPointDrawer::DrawElement(CDC* pDC,const CTwoPointDrawingData& drawingData, const int drawMode)
{
	if(!drawingData.IsDrawing())
		return;

	CDrawer::SelectNewDrawingMode(pDC, drawingData.GetDiaplayRegion(), drawMode);

	DrawShaph_Element(pDC,drawingData);

	CDrawer::SelectOrgDrawingMode(pDC);
}

void CTwoPointDrawer::DrawShadowAndGrip(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	if(!IsDrawing(region, pt1, pt2))
		return;

	CDrawer::SelectClipRegion(pDC, region);

	DrawShadow(pDC, region, pt1, pt2);
	DrawGrip(pDC, region, pt1, pt2);

	CDrawer::SelectClipRegion(pDC);
}

// protected ==================================================================
bool CTwoPointDrawer::IsDrawing(const CRect& region, const CPoint& pt1, const CPoint& pt2) const
{
	return (!region.IsRectEmpty() && pt1 != pt2);
}

void CTwoPointDrawer::DrawShadow(CDC* /*pDC*/, const CRect& /*region*/, const CPoint& /*pt1*/, const CPoint& /*pt2*/)
{
}

void CTwoPointDrawer::DrawText(CDC* /*pDC*/, const CTwoPointDrawingData& /*drawingData*/)
{
}

// private ====================================================================
void CTwoPointDrawer::DrawShaph_Tool(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	DrawShape(pDC, drawingData, drawingData.GetStartPoints().GetDrawingPoint(), drawingData.GetEndPoints().GetDrawingPoint());
	//DrawShape(pDC, drawingData.GetDisplayData(), drawingData.GetStartPoints().GetDrawingPoint(), drawingData.GetEndPoints().GetDrawingPoint());
	DrawShadow(pDC, drawingData.GetDiaplayRegion(), drawingData.GetStartPoints().GetDrawingPoint(), drawingData.GetEndPoints().GetDrawingPoint());
}

void CTwoPointDrawer::DrawShaph_Element(CDC* pDC,const CTwoPointDrawingData& drawingData)
{
	DrawShape(pDC, drawingData, drawingData.GetStartPoints().GetDrawingPoint(), drawingData.GetEndPoints().GetDrawingPoint());
	//DrawShape(pDC, drawingData, drawingData.GetStartPoints().GetDrawingPoint(), drawingData.GetEndPoints().GetDrawingPoint());
	DrawTextAndSetMode(pDC, drawingData);
}

void CTwoPointDrawer::DrawTextAndSetMode(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CDrawer::SelectNewTextMode(pDC, drawingData.GetTextColor());

	DrawText(pDC, drawingData);

	CDrawer::SelectOrgTextMode(pDC);
}

void CTwoPointDrawer::DrawGrip(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	CBrush newBrush(CDrawer::GetGripBrushColor());
	CBrush* pOldBlush = pDC->SelectObject(&newBrush);

	CDrawer::DrawRectangle(pDC, CDrawer::GetGripRegion(pt1));
	CDrawer::DrawRectangle(pDC, CDrawer::GetGripRegion(pt2));

	pDC->SelectObject(pOldBlush);
	newBrush.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// class CThreePointDrawer

void CThreePointDrawer::DrawTool(CDC* pDC, const CThreePointDrawingData& drawingData, const int drawMode)
{
	CDrawer::SelectNewDrawingMode(pDC, drawingData.GetRect(), drawMode);

	DrawShaph_Tool(pDC, drawingData);

	CDrawer::SelectOrgDrawingMode(pDC);
}

void CThreePointDrawer::DrawElement(CDC* pDC, const CThreePointDrawingData& drawingData, const int drawMode)
{
	CDrawer::SelectNewDrawingMode(pDC, drawingData.GetRect(), drawMode);

	DrawShaph_Element(pDC, drawingData);

	CDrawer::SelectOrgDrawingMode(pDC);
}

void CThreePointDrawer::DrawShadowAndGrip(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3)
{
	CDrawer::SelectClipRegion(pDC, region);

	DrawGrip(pDC, region, pt1, pt2, pt3);

	CDrawer::SelectClipRegion(pDC);
}

// protected ==================================================================
void CThreePointDrawer::DrawTool(CDC* pDC, const CRegion& region, const CPoints& points)
{
	CPoint point = points.GetDrawingPoint();
	CRect drawingRect(point, CPoint(point.x +1, point.y +1));
	drawingRect.InflateRect(MARGIN*2, MARGIN*2);
	DrawCross(pDC, point);
}

void CThreePointDrawer::DrawTool(CDC* pDC, const CRegion& region, const CPoints& points1, const CPoints& points2)
{
	DrawTool(pDC, region, points1);
	DrawTool(pDC, region, points2);
}

void CThreePointDrawer::DrawGrip(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3)
{
	CBrush newBrush(CDrawer::GetGripBrushColor());
	CBrush* pOldBlush = pDC->SelectObject(&newBrush);

	CDrawer::DrawRectangle(pDC, CDrawer::GetGripRegion(pt1));
	CDrawer::DrawRectangle(pDC, CDrawer::GetGripRegion(pt2));
	CDrawer::DrawRectangle(pDC, CDrawer::GetGripRegion(pt3));

	pDC->SelectObject(pOldBlush);
	newBrush.DeleteObject();
}

// private ====================================================================
void CThreePointDrawer::DrawShaph_Tool(CDC* pDC, const CThreePointDrawingData& drawingData)
{
	CRegion region(drawingData.GetRect(), drawingData.GetDisplayAttributes());
	switch(drawingData.GetPointsCount())
	{
	case 3:
		DrawTool(pDC, region, drawingData.GetPoints3());
	case 2:
		DrawTool(pDC, region, drawingData.GetPoints2());
	case 1:
		DrawTool(pDC, region, drawingData.GetPoints1());
	}
}

void CThreePointDrawer::DrawShaph_Element(CDC* pDC, const CThreePointDrawingData& drawingData)
{
	CRegion region(drawingData.GetRect(), drawingData.GetDisplayAttributes());
	DrawShape(pDC, region, drawingData.GetPoints1().GetDrawingPoint(), drawingData.GetPoints2().GetDrawingPoint(), drawingData.GetPoints3().GetDrawingPoint());
}

// ----------------------------------------------------------------------------
void CThreePointDrawer::DrawCross(CDC* pDC, const CPoint& centerPt)
{
	CDrawer::DrawLine(pDC, CPoint(centerPt.x, centerPt.y -7), CPoint(centerPt.x, centerPt.y +8));
	CDrawer::DrawLine(pDC, CPoint(centerPt.x -7, centerPt.y), CPoint(centerPt.x +8, centerPt.y));
}

///////////////////////////////////////////////////////////////////////////////
// class CMultiPointDrawer

void CMultiPointDrawer::DrawTool(CDC* pDC, const CMultiPointDrawingData& drawingData, const int drawMode)
{
	CDrawer::SelectNewDrawingMode(pDC, drawingData.GetRect(), drawMode);
	DrawShape(pDC, drawingData);	
	CDrawer::SelectOrgDrawingMode(pDC);
}

void CMultiPointDrawer::DrawElement(CDC* pDC, const CMultiPointDrawingData& drawingData, const int drawMode)
{
	CDrawer::SelectNewDrawingMode(pDC, drawingData.GetRect(), drawMode);
	DrawShape(pDC, drawingData);
	CDrawer::SelectOrgDrawingMode(pDC);
}

void CMultiPointDrawer::DrawShadowAndGrip(CDC* pDC, const CRect& region, const CPoint& pt)
{
	CDrawer::SelectClipRegion(pDC, region);
	DrawGrip(pDC, region, pt);
	CDrawer::SelectClipRegion(pDC);
}

// protected ==================================================================
void CMultiPointDrawer::DrawTool(CDC* pDC, const CRegion& region, const CPoints& points)
{
	/*CPoint point = points.GetDrawingPoint();
	CRect drawingRect(point, CPoint(point.x +1, point.y +1));
	drawingRect.InflateRect(MARGIN*2, MARGIN*2);
	DrawCross(pDC, point);*/
}

void CMultiPointDrawer::DrawTool(CDC* pDC, const CRegion& region, const CPoints& points1, const CPoints& points2)
{
	//DrawTool(pDC, region, points1);
	//DrawTool(pDC, region, points2);
}

void CMultiPointDrawer::DrawGrip(CDC* pDC, const CRect& region, const CPoint& pt)
{
	CBrush newBrush(CDrawer::GetGripBrushColor());
	CBrush* pOldBlush = pDC->SelectObject(&newBrush);

	CDrawer::DrawRectangle(pDC, CDrawer::GetGripRegion(pt));
	
	pDC->SelectObject(pOldBlush);
	newBrush.DeleteObject();
}

// private ====================================================================
void CMultiPointDrawer::DrawShaph_Tool(CDC* pDC, const CMultiPointDrawingData& drawingData)
{
	CRegion region(drawingData.GetRect(), drawingData.GetDisplayAttributes());
	DrawTool(pDC, region, drawingData.GetPoints(0));
}

void CMultiPointDrawer::DrawShaph_Element(CDC* pDC, const CMultiPointDrawingData& drawingData)
{
	DrawShape(pDC, drawingData);
}

///////////////////////////////////////////////////////////////////////////////
// class CVertLineDrawer -- 수직선 그리기

CVertLineDrawer::CVertLineDrawer():
	TEXT_COLOR(RGB(0, 0, 0)), 
	TEXT_BK_COLOR(RGB(227, 254, 226))
{
}

// protected ==================================================================
bool CVertLineDrawer::IsDrawing(const CRect& region, const CPoint& point) const
{
	return (region.left < point.x && region.right > point.x);
}

// 2008.08.20 김진순 피보나치 확장	<<
//void CVertLineDrawer::DrawShape(CDC* pDC, const CDisplayData& displayData, const CPoint& point, const bool /*isCurrentRegion*/)
void CVertLineDrawer::DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool /*isCurrentRegion*/)
{
	DrawShape_Line(pDC, displayData.GetDisplayRegion(), point);
}
//>>

void CVertLineDrawer::DrawText(CDC* pDC, const COnePointDrawingData& drawingData)
{
	if(!CVertLineDrawer::IsDrawing(drawingData.GetCurrentRegion(), drawingData.GetPoints().GetDrawingPoint()))
		return;

	CTextVertPosition* pTextPosition = (CTextVertPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	CString strPoint = drawingData.GetText_X();
	if(strPoint.IsEmpty())  
		return;

	if(pTextPosition->GetTop())
		DrawText(pDC, strPoint, drawingData, true);
	if(pTextPosition->GetBottom())
		DrawText(pDC, strPoint, drawingData, false);
}

CRect CVertLineDrawer::GetGripRegion(const CRect& region, const CPoint& point)
{
	return CDrawer::GetGripRegion(CPoint(point.x, CMath::Center(region.top, region.bottom)));
}

// ----------------------------------------------------------------------------
void CVertLineDrawer::DrawShape_Line(CDC* pDC, const CRect& region, const CPoint& point)
{
	if(!IsDrawing(region, point))
		return;

	CDrawer::DrawLine(pDC, CPoint(point.x, region.top), CPoint(point.x, region.bottom));
}

// private ====================================================================
void CVertLineDrawer::DrawText(CDC* pDC, const CString& textData, const COnePointDrawingData& drawingData, const bool bTop)
{
	CString strXDate = drawingData.GetText_X();
	CRect textRegion = GetTextRegion(pDC, textData, drawingData, bTop);
	//CDrawer::DrawRectangle(pDC, textRegion, drawingData.GetTextColor(), drawingData.GetTextBackgroundColor());
	CDrawer::DrawRectangle(pDC, textRegion, TEXT_COLOR, TEXT_BK_COLOR);

	//수치를 표현할 영역이 색이 고정이기 때문에 Text 색도 고정으로 해준다.
	CDrawer::DrawTextOut(pDC, textData, textRegion, TEXT_COLOR);
}

// ----------------------------------------------------------------------------
CRect CVertLineDrawer::GetTextRegion(CDC* pDC, const CString& textData, const COnePointDrawingData& drawingData, const bool bTop) const
{
	CBlockRegion blockRegion = drawingData.GetHeadBlockRegion();
	if(!bTop)
		blockRegion = drawingData.GetTailBlockRegion();

	return GetTextRegion(blockRegion, pDC->GetTextExtent(textData), 
		drawingData.GetPoints().GetDrawingPoint().x, bTop);
}

CRect CVertLineDrawer::GetTextRegion(const CBlockRegion& blockRegion, const CSize& textSize, const int nX, const bool bTop) const
{
	CRect region(0, 0, 0, 0);

	if(bTop)
		region = GetTextRegion_Top(blockRegion, textSize);
	else
		region = GetTextRegion_Bottom(blockRegion, textSize);

	int nGap = nX - (region.left + textSize.cx/2);
	if(!region.IsRectEmpty() && nGap > 0)
		region.OffsetRect((nGap), 0);

	return region;
}

CRect CVertLineDrawer::GetTextRegion_Top(const CBlockRegion& blockRegion, const CSize& textSize) const
{
	if(textSize == CSize(0, 0))
		return CRect(0, 0, 0, 0);

	CRect region = CDrawer::GetTextRegion(textSize, blockRegion.GetFullGraphRegion_TopLeft());

	// text 영역이 scale 영역보다 클 경우 scale 영역을 기준으로 text 영역을 구한다.
	if(blockRegion.GetHorzScaleRegion().GetTopScaleRegion().Height() > region.Height())
		region.OffsetRect(0, -region.Height());

	return region;
}

CRect CVertLineDrawer::GetTextRegion_Bottom(const CBlockRegion& blockRegion, const CSize& textSize) const
{
	if(textSize == CSize(0, 0))
		return CRect(0, 0, 0, 0);

	CRect region = CDrawer::GetTextRegion(textSize, blockRegion.GetFullGraphRegion_BottomLeft(), true, false);

	// text 영역이 scale 영역보다 클 경우 scale 영역을 기준으로 text 영역을 구한다.
	if(blockRegion.GetHorzScaleRegion().GetBottomScaleRegion().Height() < region.Height())
		region.OffsetRect(0, -region.Height());

	return region;
}
