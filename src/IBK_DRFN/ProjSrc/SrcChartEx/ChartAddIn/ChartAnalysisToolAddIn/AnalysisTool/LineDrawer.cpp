// LineDrawer.cpp: implementation of the CLineDrawer class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineDrawer.h"

#include "DrawingData.h"
#include "Conversion.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CLineDrawer

CLineDrawer::CLineDrawer( const CAnalysis::TOOLTYPE p_eToolType)
{
	m_eToolType = p_eToolType;
}

void CLineDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	BOOL bLeftLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetLeftExt();
	BOOL bRightLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetRightExt();
	CDrawer::DrawLineExt( pDC, pt1, pt2,bLeftLineExt,bRightLineExt);
}

void CLineDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CTextHorzPosition* pTextPosition = (CTextHorzPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	CString strText;

	CPoint ptStart = drawingData.GetStartPoints().GetDrawingPoint();
	CPoint ptEnd   = drawingData.GetEndPoints().GetDrawingPoint();
	if (pTextPosition->GetLeft())
	{
		strText = drawingData.GetText_Y( drawingData.GetLeftPoints().GetSavingPoint().GetY());
		CDrawer::TEXTPOSITION eTextPosition = CDrawer::LEFT;
		CDrawer::DrawTextOut( pDC, eTextPosition, strText, drawingData.GetLeftPoints().GetDrawingPoint());
	}
	if( pTextPosition->GetRight())
	{
		double dLeft = drawingData.GetStartPoints().GetSavingPoint().GetY();
		double dRight = drawingData.GetEndPoints().GetSavingPoint().GetY();

//		double dLeft = drawingData.GetLeftPoints().GetSavingPoint().GetY();
//		double dRight = drawingData.GetRightPoints().GetSavingPoint().GetY();

		strText = drawingData.GetText_Y( drawingData.GetRightPoints().GetSavingPoint().GetY());
		if (pTextPosition->GetRightRate())
		{
			if (dLeft != 0)
			{
				//	등락율 = ((끝점 - 시작점) / 시작점) * 100
				double dRate = ((dRight - dLeft) / dLeft) * 100.0;
				CString strTemp = drawingData.GetText_Y( drawingData.GetRightPoints().GetSavingPoint().GetY());
				strText.Format(_T("%s(%.2f%%)"), strTemp, dRate);
			}
		}

		CDrawer::TEXTPOSITION eTextPosition = CDrawer::RIGHT;
		CDrawer::DrawTextOut( pDC, eTextPosition, strText, drawingData.GetRightPoints().GetDrawingPoint());
	}
}

///////////////////////////////////////////////////////////////////////////////
// class CHorzLineDrawer -- 수평선 그리기

CHorzLineDrawer::CHorzLineDrawer():
	TEXT_COLOR(RGB(0, 0, 0)), 
	TEXT_BK_COLOR(RGB(227, 254, 226))
{
}

// protected ==================================================================
bool CHorzLineDrawer::IsDrawing(const CRect& region, const CPoint& point) const
{
	return (region.top < point.y && region.bottom > point.y);
}

void CHorzLineDrawer::DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion)
{
	if(!isCurrentRegion)
		return;

	DrawShape(pDC, displayData.GetDisplayRegion(), point);
}

void CHorzLineDrawer::DrawText(CDC* pDC, const COnePointDrawingData& drawingData)
{
	if(!CHorzLineDrawer::IsDrawing(drawingData.GetCurrentRegion(), drawingData.GetPoints().GetDrawingPoint()))
		return;

	CTextHorzPosition* pTextPosition = (CTextHorzPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	CString strPoint = drawingData.GetText_Y();
	if(strPoint.IsEmpty())  
		return;

	if(pTextPosition->GetLeft())
		DrawText(pDC, strPoint, drawingData, true);
	if(pTextPosition->GetRight())
		DrawText(pDC, strPoint, drawingData, false);
}

CRect CHorzLineDrawer::GetGripRegion(const CRect& region, const CPoint& point)
{
	return CDrawer::GetGripRegion(CPoint(CMath::Center(region.left, region.right), point.y));
}

// ----------------------------------------------------------------------------
void CHorzLineDrawer::DrawShape(CDC* pDC, const CRect& region, const CPoint& point)
{
	if(!IsDrawing(region, point))
		return;

	CDrawer::DrawLine(pDC, CPoint(region.left, point.y), CPoint(region.right, point.y));
}

// private ====================================================================
void CHorzLineDrawer::DrawText(CDC* pDC, const CString& textData, const COnePointDrawingData& drawingData, const bool bLeft)
{
	CRect textRegion = GetTextRegion(drawingData.GetCurrentBlockRegion(), 
		pDC->GetTextExtent(textData), drawingData.GetPoints().GetDrawingPoint().y, bLeft);
	//CDrawer::DrawRectangle(pDC, textRegion, drawingData.GetTextColor(), drawingData.GetTextBackgroundColor());
	CDrawer::DrawRectangle(pDC, textRegion, TEXT_COLOR, TEXT_BK_COLOR);

	//수치를 표현할 영역이 색이 고정이기 때문에 Text 색도 고정으로 해준다.
	CDrawer::DrawTextOut(pDC, textData, textRegion, TEXT_COLOR);
}

CRect CHorzLineDrawer::GetTextRegion(const CBlockRegion& blockRegion, const CSize& textSize, const int nY, const bool bLeft) const
{
	CRect region(0, 0, 0, 0);

	if(bLeft)
		region = GetTextRegion_Left(blockRegion, textSize);
	else
		region = GetTextRegion_Right(blockRegion, textSize);

	if(!region.IsRectEmpty())
		region.OffsetRect(0, (nY - (region.top + region.Height()/2)));

	return region;
}

CRect CHorzLineDrawer::GetTextRegion_Left(const CBlockRegion& blockRegion, const CSize& textSize) const
{
	if(textSize == CSize(0, 0))
		return CRect(0, 0, 0, 0);

	CRect region = CDrawer::GetTextRegion(textSize, blockRegion.GetFullGraphRegion_TopLeft(), false);

	// text 영역이 scale 영역보다 클 경우 scale 영역을 기준으로 text 영역을 구한다.
	int nX = blockRegion.GetVertScaleRegion().GetLeftScaleRegion().left - region.left;
	if(nX > 0)
		region.OffsetRect(nX, 0);

	return region;
}

CRect CHorzLineDrawer::GetTextRegion_Right(const CBlockRegion& blockRegion, const CSize& textSize) const
{
	if(textSize == CSize(0, 0))
		return CRect(0, 0, 0, 0);

	CRect region = CDrawer::GetTextRegion(textSize, blockRegion.GetFullGraphRegion_TopRight());

	// text 영역이 scale 영역보다 클 경우 scale 영역을 기준으로 text 영역을 구한다.
	int nX = blockRegion.GetVertScaleRegion().GetRightScaleRegion().right - region.right;
	if(nX < 0)
		region.OffsetRect(nX, 0);

	return region;
}

///////////////////////////////////////////////////////////////////////////////
// class CCrossLineDrawer -- 십자선 그리기

bool CCrossLineDrawer::IsDrawing(const CRect& region, const CPoint& point) const
{
	return CVertLineDrawer::IsDrawing(region, point) || CHorzLineDrawer::IsDrawing(region, point);
	//return CVertLineDrawer::IsDrawing(region, point) && CHorzLineDrawer::IsDrawing(region, point);
}

void CCrossLineDrawer::SelectedDraw_VertLine(CDC* pDC, const CDrawingRegions& drawingRegions, const CPoint& point)
{
	int oldDrawMode = pDC->GetROP2();
	pDC->SetROP2(R2_NOT);
	for(int nIndex = 0; nIndex < drawingRegions.GetCount(); nIndex++)
	{
		CRect region = drawingRegions.GetFullGraphRegion(nIndex);
		CDrawer::SelectClipRegion(pDC, region);
		CVertLineDrawer::DrawShape_Line(pDC, region, point);
		CDrawer::SelectClipRegion(pDC);
	}
	pDC->SetROP2(oldDrawMode);
}

void CCrossLineDrawer::SelectedDraw_HorzLine(CDC* pDC, const CDrawingRegions& drawingRegions, const CPoint& point)
{
	CRect region = drawingRegions.GetCurrentFullGraphRegion();
	int oldDrawMode = pDC->GetROP2();
	pDC->SetROP2(R2_NOT);

	CDrawer::SelectClipRegion(pDC, region);
	CHorzLineDrawer::DrawShape(pDC, region, point);
	CDrawer::SelectClipRegion(pDC);

	pDC->SetROP2(oldDrawMode);
}

// protected ==================================================================
void CCrossLineDrawer::DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion)
{
	CVertLineDrawer::DrawShape(pDC, drawingData, displayData, point, isCurrentRegion);
	CHorzLineDrawer::DrawShape(pDC, drawingData, displayData, point, isCurrentRegion);
}

void CCrossLineDrawer::DrawText(CDC* pDC, const COnePointDrawingData& drawingData)
{
	COnePointDrawingData realDrawingData = drawingData;
	CTextHorzVertPosition* pHorzVertPosition = (CTextHorzVertPosition*) drawingData.GetTextPosition();
	realDrawingData.SetTextPosition((CTextPosition*) (&pHorzVertPosition->GetVert()));
	CVertLineDrawer::DrawText(pDC, realDrawingData);
	realDrawingData.SetTextPosition((CTextPosition*) (&pHorzVertPosition->GetHorz()));
	CHorzLineDrawer::DrawText(pDC, realDrawingData);
}

CRect CCrossLineDrawer::GetGripRegion(const CRect& region, const CPoint& point)
{
	return CDrawer::GetGripRegion(point);
}

///////////////////////////////////////////////////////////////////////////////
// class CEqualDivisionLineDrawer

void CEqualDivisionLineDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	// 가로선
	//TRACE("CEqualDivisionLineDrawer::DrawShape[%d,%d]\n",pt1.x,pt1.y);
	BOOL bLeftLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetLeftExt();
	BOOL bRightLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetRightExt();
	BOOL bEquiDivType = (BOOL)drawingData.GetEquiDivType();
	int nCount = GetCount(bEquiDivType);
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		int y = GetPoint_Y(pt1.y, pt2.y, nIndex,bEquiDivType);
		CDrawer::DrawLineExt(pDC, CPoint(pt1.x, y), CPoint(pt2.x, y),bLeftLineExt,bRightLineExt);
	}
}

void CEqualDivisionLineDrawer::DrawShadow(CDC* pDC, const CRect& /*region*/, const CPoint& pt1, const CPoint& pt2)
{
	// 사선
	CDrawer::DrawLine(pDC, pt1, pt2);
}

void CEqualDivisionLineDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CTextHorzPosition* pTextPosition = (CTextHorzPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;
	if(pTextPosition->GetLeft())
		DrawText_Left(pDC, drawingData);
	if(pTextPosition->GetRight())
		DrawText_Right(pDC, drawingData);
}

// 해당 index번의 y 좌표.
int CEqualDivisionLineDrawer::GetPoint_Y(const int nY1, const int nY2, const int nIndex,BOOL bEquiDiv) const
{
	if(nY1 > nY2)
		return GetDrawingPoint_Y(nY2, (nY2 - nY1), nIndex,bEquiDiv);

	return GetDrawingPoint_Y(nY1, (nY1 - nY2), nIndex,bEquiDiv);
}

// private ====================================================================
void CEqualDivisionLineDrawer::DrawText_Left(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CPoints points_L = drawingData.GetLeftPoints();
	CPoints points_R = drawingData.GetRightPoints();

	BOOL bEquiDivType = (BOOL)drawingData.GetEquiDivType();
	for(int nIndex = 0; nIndex < GetCount(bEquiDivType); nIndex++)
	{
		CPoints points = GetPoints(drawingData.GetVertScaleType(), points_L, points_R, nIndex,bEquiDivType);
		CDrawer::DrawTextOut(pDC, CDrawer::LEFT, drawingData.GetText_Y(points.GetSavingPoint().GetY()), points.GetDrawingPoint());
	}
}

void CEqualDivisionLineDrawer::DrawText_Right(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CSize maxSize = pDC->GetTextExtent("100%");

	CPoint point_L = drawingData.GetLeftPoints().GetDrawingPoint();
	CPoint point_R = drawingData.GetRightPoints().GetDrawingPoint();
	
	BOOL bEquiDivType = (BOOL)drawingData.GetEquiDivType();
	int nTotalCount = GetCount(bEquiDivType);
	for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
	{
		CDrawer::DrawTextOut(pDC, CDrawer::RIGHT, GetRightText(nTotalCount -(nIndex +1),bEquiDivType), 
			CPoint(point_R.x, GetPoint_Y(point_R.y, point_L.y, nIndex,bEquiDivType)), maxSize);
	}
}

// ----------------------------------------------------------------------------
CString CEqualDivisionLineDrawer::GetRightText(const int nIndex,BOOL bEquiDivType) const
{
	CString strCoefficient = CDataConversion::DoubleToString(GetCoefficient(nIndex,bEquiDivType) *100.0);
	if(strCoefficient.IsEmpty())
		return "";

	return strCoefficient + "%";
}

// ----------------------------------------------------------------------------
CPoints CEqualDivisionLineDrawer::GetPoints(const CVertScaleType& vertScaleType, const CPoints& points1, 
											const CPoints& points2, const int nIndex,BOOL bEquiDiv) const
{
	if(nIndex == 0)
		return points1;
	else if(nIndex == (GetCount(bEquiDiv)-1))
		return CPoints(points2.GetSavingPoint(), CPoint(points1.GetDrawingPoint().x, points2.GetDrawingPoint().y));

	CPoint drawPoint(points1.GetDrawingPoint().x, GetPoint_Y(points1.GetDrawingPoint().y, points2.GetDrawingPoint().y, nIndex,bEquiDiv));
	double saving_y = GetSavingPoint_Y(vertScaleType, points1, points2, drawPoint.y);

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == saving_y) return points1;

	// 분차트 근접시간 적용 - ojtaso (20080811)
	// 20081007 JS.Kim	사용자시간대 선택
	CCoordinate savePoint(points1.GetSavingPoint().GetX(), saving_y, points1.GetSavingPoint().GetTimeDiff(), points1.GetSavingPoint().GetDateType(), 0);
	return CPoints(savePoint, drawPoint);
}

double CEqualDivisionLineDrawer::GetSavingPoint_Y(const CVertScaleType& vertScaleType, const CPoints& points1, 
												  const CPoints& points2, const int nDrawingPoint_Y) const
{
	CRect rect(points1.GetDrawingPoint(), points2.GetDrawingPoint());
	rect.NormalizeRect();

	CDisplayDataVertMinMax displayDataMinMax(points1.GetSavingPoint().GetY(), points2.GetSavingPoint().GetY(), vertScaleType);
	displayDataMinMax.NormalizeMinMax();

	double y = CDataConversion::AbsoluteToRelative_Y(rect, nDrawingPoint_Y, displayDataMinMax.GetMin_ScaleType(), displayDataMinMax.GetMax_ScaleType());

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == y) return -HUGE_VAL;

	return vertScaleType.ApplyExp(y);
}

int CEqualDivisionLineDrawer::GetDrawingPoint_Y(const int origin, const int nHeight, const int nIndex,BOOL bEquiDiv) const
{
	int nLength = CMath::Round((double)nHeight * GetCoefficient(nIndex,bEquiDiv));
	return (origin - nLength);
}

///////////////////////////////////////////////////////////////////////////////
// class CTrisectionLineDrawer

int CTrisectionLineDrawer::GetCount(BOOL bEquiDivType) const
{
	if(bEquiDivType == TRUE)
	{
		return 3;
	}

	return 4;
}

// protected ==================================================================
double CTrisectionLineDrawer::GetCoefficient(const int nIndex,BOOL bEquiDivType) const
{
	if(bEquiDivType == TRUE)
	{
		switch(nIndex)
		{
		case 0:
			return 0.0;
		case 1:
			return 0.5;
		case 2:
			return 1.0;
		}
	}
	else
	{
		switch(nIndex)
		{
		case 0:
			return 0.0;
		case 1:
			return 0.33;
		case 2:
			return 0.67;
		case 3:
			return 1.0;
		}
	}	

	assert(false);
	return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
// class CQuadrisectLineDrawer

int CQuadrisectLineDrawer::GetCount(BOOL bEquiDivType) const
{
	if(bEquiDivType == TRUE)
	{
		return 3;
	}

	return 5;
}

// protected ==================================================================
double CQuadrisectLineDrawer::GetCoefficient(const int nIndex,BOOL bEquiDivType) const
{
	if(bEquiDivType == TRUE)
	{
		switch(nIndex)
		{
		case 0:
			return 0.0;
		case 1:
			return 0.5;
		case 2:
			return 1.0;
		}
	}
	else
	{
		switch(nIndex)
		{
		case 0:
			return 0.0;
		case 1:
			return 0.25;
		case 2:
			return 0.5;
		case 3:
			return 0.75;
		case 4:
			return 1.0;
		}
	}

	assert(false);
	return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
// class CAndrewsPitchforkDrawer

// 중심의 점과 사이드의 점을 정열한다.
// 가장 왼쪽에 있는 점 == 중심점(C)
void CAndrewsPitchforkDrawer::GetPoints(const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, CPoint& centerPt, CPoint& sidePt1, CPoint& sidePt2) const
{
	if(pt1.x <= pt2.x)
	{
		if(pt1.x <= pt3.x)
		{
			centerPt = pt1;
			sidePt1 = pt2;
			sidePt2 = pt3;
		}
		else
		{
			centerPt = pt3;
			sidePt1 = pt1;
			sidePt2 = pt2;
		}
	}
	else
	{
		if(pt2.x <= pt3.x)
		{
			centerPt = pt2;
			sidePt1 = pt1;
			sidePt2 = pt3;
		}
		else
		{
			centerPt = pt3;
			sidePt1 = pt1;
			sidePt2 = pt2;
		}
	}
}

// pt1에 관한 선의 end 좌표.
CPoint CAndrewsPitchforkDrawer::GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3) const
{
	CPoint centerPt = pt1, sidePt1 = pt2, sidePt2 = pt3;
	GetPoints(pt1, pt2, pt3, centerPt, sidePt1, sidePt2);
	CPoint middlePt = GetMiddlePointInPoints(sidePt1, sidePt2);

	if(pt1 == centerPt)
		return GetLineEndPoint(region, pt1, middlePt);

	return GetLineEndPoint(region, pt1, GetPointInLine(centerPt, middlePt, pt1));
}

// protected ==================================================================
void CAndrewsPitchforkDrawer::DrawShape(CDC* pDC, const CRegion& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3)
{
	CPoint centerPt = pt1, sidePt1 = pt2, sidePt2 = pt3;
	GetPoints(pt1, pt2, pt3, centerPt, sidePt1, sidePt2);
	CPoint middlePt = GetMiddlePointInPoints(sidePt1, sidePt2);

	DrawLine(pDC, region.GetRect(), centerPt, middlePt);
	DrawLine(pDC, region.GetRect(), sidePt1, GetPointInLine(centerPt, middlePt, sidePt1));
	DrawLine(pDC, region.GetRect(), sidePt2, GetPointInLine(centerPt, middlePt, sidePt2));
}

// private ====================================================================
// 사이드의 두개의 점(S1, S2)을 잇는 선의 가운데 점(M)
// S1 -----M----- S2
CPoint CAndrewsPitchforkDrawer::GetMiddlePointInPoints(const CPoint& pt1, const CPoint& pt2) const
{
	int x = pt1.x + (pt2.x - pt1.x)/2;
	int y = pt1.y + (pt2.y - pt1.y)/2;

	return CPoint(x, y);
}

// 기울기가 중심점(C)과 가운데점(M) 의 기울기와 같고 사이드점(S1, S2)를 지나는 점(S1', S2')
CPoint CAndrewsPitchforkDrawer::GetPointInLine(const CPoint& baseLinePt1, const CPoint& baseLinePt2, const CPoint& point) const
{
	double dAngle = CDrawer::GetLineAngle(baseLinePt1, baseLinePt2);
	double dIntercept = CDrawer::GetLineIntercept(dAngle, point);

	int x = point.x + 30;
	int y = CMath::Round((dAngle * (double)x) + dIntercept);

	return CPoint(x, y);
}

// pt1과 pt2를 지나는 선과 region의 오른쪽 세로 선과의 교점
CPoint CAndrewsPitchforkDrawer::GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2) const
{
	CPoint point;
	if(CDrawer::Intersection(pt1, pt2, CPoint(region.right, region.top), CPoint(region.right, region.bottom), point))
		return point;

	return pt1;
}

// ----------------------------------------------------------------------------
void CAndrewsPitchforkDrawer::DrawLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	CPoint point = GetLineEndPoint(region, pt1, pt2);
	if(point == pt1)
		return;

	CDrawer::DrawLine(pDC, pt1, point);
}

///////////////////////////////////////////////////////////////////////////////
// class CCycleLinesDrawer -- 사이클 구간 그리기

//public ==================================================================
int CCycleLinesDrawer::GetPoints_Index1()	const
{ return m_nPointIndex1;	}
int CCycleLinesDrawer::GetPoints_Index2()	const
{ return m_nPointIndex2;	}

// protected ==================================================================
void CCycleLinesDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	//CGraphPartRegion graphPartRegion = drawingData.GetDiaplayRegion//GetDrawingRegions()->GetGraphPartRegion(0);

	//CDrawer::SelectClipRegion(pDC, graphPartRegion.GetFull());
	DrawFigure_Data(pDC,drawingData.GetDisplayData(),pt1,pt2);
	//CDrawer::SelectClipRegion(pDC);	
}

// private ====================================================================
// data 형태의 그리기.
void CCycleLinesDrawer::DrawFigure_Data(CDC* pDC, const CDisplayData& displayData, const CPoint& pt1, const CPoint& pt2)
{
	CRegion coordinateCalRegion = GetCoordinateCalRegion(displayData);
	int nIndex1 = CDrawer::GetCurrentDataIndex(coordinateCalRegion, pt1);
	int nIndex2 = CDrawer::GetCurrentDataIndex(coordinateCalRegion, pt2);
	
	m_nPointIndex1 = nIndex1;
	m_nPointIndex2 = nIndex2;
	int nGap = abs(nIndex1 - nIndex2);
	if(nGap == 0)
	{
		return;
	}

	//CGraphPartRegion graphPartRegion = displayData.GetGraphPartRegion();
	//CDrawer::SelectClipRegion(pDC, graphPartRegion.GetFull());
	int nAccCount = 0;
	bool bResult1 = false;
	bool bResult2 = false;
	int drawPt_X1 = -1;
	int drawPt_X2 = -1;
	while(true)
	{
		bResult1 = coordinateCalRegion.DoesIndexInDisplayAttributes(nIndex1 + nAccCount);
		if(bResult1)
		{
			drawPt_X1 = CDrawer::GetDrawingPoint_X(coordinateCalRegion, nIndex1 + nAccCount);
			CDrawer::DrawLine(pDC, CPoint(drawPt_X1, displayData.GetDisplayRegion().top), 
									CPoint(drawPt_X1, displayData.GetDisplayRegion().bottom));
		}

		bResult2 = coordinateCalRegion.DoesIndexInDisplayAttributes(nIndex1 - nAccCount);
		if(bResult1 && nAccCount != 0)
		{
			drawPt_X2 = CDrawer::GetDrawingPoint_X(coordinateCalRegion, nIndex1 - nAccCount);
			CDrawer::DrawLine(pDC, CPoint(drawPt_X2, displayData.GetDisplayRegion().top), 
										CPoint(drawPt_X2, displayData.GetDisplayRegion().bottom));			
		}

		if(!bResult1 && !bResult2)
			break;

		nAccCount += nGap;
	}
	//CDrawer::SelectClipRegion(pDC);
}

CRegion CCycleLinesDrawer::GetCoordinateCalRegion(const CDisplayData& displayData) const
{
	return CRegion(displayData.GetCoordinateCalRegion(), displayData.GetDisplayAttributes());
}

bool CCycleLinesDrawer::GetIndexInDisplayAttributes(const CDisplayData& displayData,int nIndex) const
{
	CRegion coordinateCalRegion = GetCoordinateCalRegion(displayData);
	return coordinateCalRegion.DoesIndexInDisplayAttributes(nIndex);
}

int CCycleLinesDrawer::GetDrawingPoint_X(const CDisplayData& displayData,int nIndex) const
{
	CRegion coordinateCalRegion = GetCoordinateCalRegion(displayData);
	return CDrawer::GetDrawingPoint_X(coordinateCalRegion, nIndex);
}
///////////////////////////////////////////////////////////////////////////////
// class CSpeedLineDrawer

int CSpeedLineDrawer::GetCount() const
{
	return 5;
}

void CSpeedLineDrawer::GetPoints(const CPoint& pt1, const CPoint& pt2, CPoint& leftPt, CPoint& rightPt) const
{
	leftPt = pt1;
	rightPt = pt2;

	if(pt1.x > pt2.x)
	{
		leftPt = pt2;
		rightPt = pt1;
	}
}

// 해당 index번의 교차점.
bool CSpeedLineDrawer::GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const
{
	// 수직선
	if(pt1.x == pt2.x)
	{
		point = CPoint(pt1.x, pt2.y);
		return true;
	}

	// 수평선
	if(pt1.y == pt2.y)
	{
		point = CPoint(pt1.x, pt2.y);
		return true;
	}

	// 사선
	int x = -1;
	int y = -1;
	CPoint coefPoint;
	bool bReturn = false;
	if(nIndex < 3)
	{
		y = CMath::Round((pt1.y - pt2.y) * GetCoefficient(nIndex));
		coefPoint = CPoint(pt2.x, pt2.y + y);
		bReturn = CDrawer::Intersection(pt1, coefPoint, CPoint(max(pt1.x,pt2.x),min(pt1.y,pt2.y)), 
								CPoint(max(pt1.x,pt2.x),max(pt1.y,pt2.y)), point);
	}
	else
	{
		x = abs(CMath::Round((pt1.x - pt2.x))) * GetCoefficient(nIndex);
		coefPoint = CPoint(pt2.x+x, pt2.y);
		bReturn = CDrawer::Intersection(pt1, coefPoint, CPoint(max(pt1.x,coefPoint.x),min(pt1.y,pt2.y)), 
								CPoint(max(pt1.x,coefPoint.x),max(pt1.y,pt2.y)), point);
	}
	
	return bReturn;
}

// protected ==================================================================
void CSpeedLineDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{ 	
	CPoint leftPt, rightPt;
	GetPoints(pt1, pt2, leftPt, rightPt);

	CDrawer::DrawFrameRect(pDC, pt1, pt2);

	CRect rect = drawingData.GetDisplayData().GetDisplayRegion();
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CPoint lineEndPt;
		if(!GetLineEndPoint(rect, leftPt, rightPt, nIndex, lineEndPt))
			break;

		if(nIndex > 0 && nIndex < 3)
		{
			CDrawer::DrawLine(pDC, CPoint(leftPt.x,lineEndPt.y), lineEndPt);		
		}
		else if(nIndex >= 3)
		{
			CDrawer::DrawLine(pDC, CPoint(lineEndPt.x,leftPt.y), lineEndPt);
		}		

		if(nIndex < 3)
		{
			CDrawer::DrawLineExt(pDC, leftPt, lineEndPt,FALSE,TRUE);
		}
	}
}

// private ====================================================================
double CSpeedLineDrawer::GetCoefficient(const int nIndex) const
{
	switch(nIndex)
	{
	case 0:
		return 0.0;
	case 1:
		return 1.0/3.0;
	case 2:
		return 2.0/3.0;
	case 3:
		return -1.0/3.0;
	case 4:
		return -2.0/3.0;
	}
	
	assert(false);
	return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
// class CSpeedFanDrawer

int CSpeedFanDrawer::GetCount() const
{
	return 5;
}

void CSpeedFanDrawer::GetPoints(const CPoint& pt1, const CPoint& pt2, CPoint& leftPt, CPoint& rightPt) const
{
	leftPt = pt1;
	rightPt = pt2;

	if(pt1.x > pt2.x)
	{
		leftPt = pt2;
		rightPt = pt1;
	}
}

// 해당 index번의 교차점.
bool CSpeedFanDrawer::GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, 
									  const int nIndex, CPoint& point) const
{
	// 수직선
	if(pt1.x == pt2.x)
	{
		point = CPoint(pt1.x, pt2.y);
		return true;
	}

	// 수평선
	if(pt1.y == pt2.y)
	{
		point = CPoint(pt1.x, pt2.y);
		return true;
	}

	// 사선
	int x = -1;
	int y = -1;
	CPoint coefPoint;
	bool bReturn = false;
	if(nIndex < 3)
	{
		y = CMath::Round((pt1.y - pt2.y) * GetCoefficient(nIndex));
		coefPoint = CPoint(pt2.x, pt2.y + y);
		bReturn = CDrawer::Intersection(pt1, coefPoint, CPoint(max(pt1.x,pt2.x),min(pt1.y,pt2.y)), 
								CPoint(max(pt1.x,pt2.x),max(pt1.y,pt2.y)), point);
	}
	else
	{
		x = abs(CMath::Round((pt1.x - pt2.x))) * GetCoefficient(nIndex);
		coefPoint = CPoint(pt2.x+x, pt2.y);
		bReturn = CDrawer::Intersection(pt1, coefPoint, CPoint(max(pt1.x,coefPoint.x),min(pt1.y,pt2.y)), 
								CPoint(max(pt1.x,coefPoint.x),max(pt1.y,pt2.y)), point);
	}
	
	return bReturn;
}

// protected ==================================================================
void CSpeedFanDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	CPoint leftPt, rightPt;
	GetPoints(pt1, pt2, leftPt, rightPt);

	CDrawer::DrawFrameRect(pDC, pt1, pt2);

	CRect rect = drawingData.GetDisplayData().GetDisplayRegion();
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CPoint lineEndPt;
		if(!GetLineEndPoint(rect, leftPt, rightPt, nIndex, lineEndPt))
			break;

		if(nIndex > 0 && nIndex < 3)
		{
			CDrawer::DrawLine(pDC, CPoint(leftPt.x,lineEndPt.y), lineEndPt);
		}
		else if(nIndex >= 3)
		{
			CDrawer::DrawLine(pDC, CPoint(lineEndPt.x,leftPt.y), lineEndPt);
		}

		CDrawer::DrawLine(pDC, leftPt, lineEndPt);
	}
}

// private ====================================================================
double CSpeedFanDrawer::GetCoefficient(const int nIndex) const
{
	switch(nIndex)
	{
	case 0:
		return 0.0;
	case 1:
		return 1.0/3.0;
	case 2:
		return 2.0/3.0;
	case 3:
		return -1.0/3.0;
	case 4:
		return -2.0/3.0;
	}
	
	assert(false);
	return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
// class CSpeedArcDrawer

int CSpeedArcDrawer::GetCount() const
{
	return 5;
}

void CSpeedArcDrawer::GetPoints(const CPoint& pt1, const CPoint& pt2, CPoint& leftPt, CPoint& rightPt) const
{
	leftPt = pt1;
	rightPt = pt2;

	if(pt1.x > pt2.x)
	{
		leftPt = pt2;
		rightPt = pt1;
	}
}

// 해당 index번의 교차점.
bool CSpeedArcDrawer::GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, 
									  const int nIndex, CPoint& point) const
{
	// 수직선
	if(pt1.x == pt2.x)
	{
		point = CPoint(pt1.x, pt2.y);
		return true;
	}

	// 수평선
	if(pt1.y == pt2.y)
	{
		point = CPoint(pt1.x, pt2.y);
		return true;
	}

	// 사선
	int x = -1;
	int y = -1;
	CPoint coefPoint;
	bool bReturn = false;
	if(nIndex < 3)
	{
		y = CMath::Round((pt1.y - pt2.y) * GetCoefficient(nIndex));
		coefPoint = CPoint(pt2.x, pt2.y + y);
		bReturn = CDrawer::Intersection(pt1, coefPoint, CPoint(max(pt1.x,pt2.x),min(pt1.y,pt2.y)), 
								CPoint(max(pt1.x,pt2.x),max(pt1.y,pt2.y)), point);
	}
	else
	{
		x = abs(CMath::Round((pt1.x - pt2.x))) * GetCoefficient(nIndex);
		coefPoint = CPoint(pt2.x+x, pt2.y);
		bReturn = CDrawer::Intersection(pt1, coefPoint, CPoint(max(pt1.x,coefPoint.x),min(pt1.y,pt2.y)), 
								CPoint(max(pt1.x,coefPoint.x),max(pt1.y,pt2.y)), point);
	}
	
	return bReturn;
}

// protected ==================================================================
void CSpeedArcDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{			
	CPoint leftPt, rightPt;
	GetPoints(pt1, pt2, leftPt, rightPt);

	CDrawer::DrawFrameRect(pDC, pt1, pt2);

	CRect rectArc;
	CRect rect = drawingData.GetDisplayData().GetDisplayRegion();
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CPoint lineEndPt;
		if(!GetLineEndPoint(rect, leftPt, rightPt, nIndex, lineEndPt))
			break;

		if(nIndex > 0 && nIndex < 3)
		{
			rectArc = GetSemicircleRegion(rightPt, GetRadius(leftPt, rightPt, nIndex));
			if(rectArc.IsRectEmpty())
				break;

			CDrawer::DrawLine(pDC, CPoint(leftPt.x,lineEndPt.y), lineEndPt);
		}
		else if(nIndex >= 3)
		{
			CDrawer::DrawLine(pDC, CPoint(lineEndPt.x,leftPt.y), lineEndPt);
		}	

		if(nIndex > 0 && nIndex < 3)
			DrawArc(pDC, rectArc, rightPt.y, leftPt.y);		
	}
}

// private ====================================================================
double CSpeedArcDrawer::GetCoefficient(const int nIndex) const
{
	switch(nIndex)
	{
	case 0:
		return 0.0;
	case 1:
		return 1.0/3.0;
	case 2:
		return 2.0/3.0;
	case 3:
		return -1.0/3.0;
	case 4:
		return -2.0/3.0;
	}
	
	assert(false);
	return 0.0;
}

// protected ==================================================================
void CSpeedArcDrawer::DrawArc(CDC* pDC, const CRect& region, const int startY, const int endY)
{
	if(startY >= endY)
		DrawSemicircle(pDC, region, region.right, region.left);
	else
		DrawSemicircle(pDC, region, region.left, region.right);
}

// private ====================================================================
// 호를 그릴 영역을 구한다.
CRect CSpeedArcDrawer::GetSemicircleRegion(const CPoint& centerPt, const int length) const
{
	CRect rect(centerPt.x -length, centerPt.y -length, centerPt.x +length, centerPt.y +length);
	rect.NormalizeRect();
	return rect;
}

void CSpeedArcDrawer::DrawSemicircle(CDC* pDC, const CRect& region, const int startX, const int endX)
{
	pDC->Arc(region, CPoint(startX, region.CenterPoint().y), CPoint(endX, region.CenterPoint().y));
}

// 호의 반지름 구하기.
int CSpeedArcDrawer::GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const
{
	return CMath::Round(CMath::Distance(pt2, pt1) * GetCoefficient(nIndex));
}

///////////////////////////////////////////////////////////////////////////////
// class CLinearRegressionDrawer

void CLinearRegressionDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	if(pt1.x == pt2.x)
		return;

	CRect region = drawingData.GetDiaplayRegion();
	
	BOOL bLeftLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetLeftExt();
	BOOL bRightLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetRightExt();
	
	CDrawer::DrawLineExt( pDC, pt1, pt2,bLeftLineExt,bRightLineExt);
}

void CLinearRegressionDrawer::DrawShadow(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	CPoint ptStart(pt1);
	CPoint ptEnd(pt1);

	ptStart.y = region.top;
	ptEnd.y = region.bottom;
	CDrawer::DrawLine( pDC, ptStart, ptEnd);

	ptStart.x = pt2.x;
	ptEnd.x = pt2.x;
	CDrawer::DrawLine( pDC, ptStart, ptEnd);
}

///////////////////////////////////////////////////////////////////////////////
// class CRaffRegressionDrawer

CRaffRegressionDrawer::CRaffRegressionDrawer()
{
	m_dChannel = 0.0;
}

void CRaffRegressionDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	CRect region = drawingData.GetDiaplayRegion();
	
	BOOL bLeftLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetLeftExt();
	BOOL bRightLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetRightExt();	
	CDrawer::DrawLineExt( pDC, pt1, pt2,bLeftLineExt,bRightLineExt);

	CCoordinate coordStart = drawingData.GetStartPoints().GetSavingPoint();
	if(m_dChannel == 0.0)	//최초 설정시에 10%값을 넣어준다
		m_dChannel = coordStart.GetY()/10.0;

	CRegion coordinateCalRegion = GetCoordinateCalRegion(drawingData.GetDisplayData());
	int nIndex1 = CDrawer::GetCurrentDataIndex(coordinateCalRegion, pt1);
	int nIndex2 = CDrawer::GetCurrentDataIndex(coordinateCalRegion, pt2);

	int nGap = abs(nIndex1 - nIndex2);
	if(nGap == 0)
		return;
	
	int nChannel = m_dChannel*pt1.y/coordStart.GetY();
	CDrawer::DrawLineExt( pDC, CPoint(pt1.x,pt1.y + nChannel),CPoint(pt2.x,pt2.y + nChannel),bLeftLineExt,bRightLineExt);
	CDrawer::DrawLineExt( pDC, CPoint(pt1.x,pt1.y - nChannel),CPoint(pt2.x,pt2.y - nChannel),bLeftLineExt,bRightLineExt);
}

void CRaffRegressionDrawer::DrawShadow(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	CPoint ptStart(pt1);
	CPoint ptEnd(pt1);

	ptStart.y = region.top;
	ptEnd.y = region.bottom;
	CDrawer::DrawLine( pDC, ptStart, ptEnd);

	ptStart.x = pt2.x;
	ptEnd.x = pt2.x;
	CDrawer::DrawLine( pDC, ptStart, ptEnd);
}

CRegion CRaffRegressionDrawer::GetCoordinateCalRegion(const CDisplayData& displayData) const
{
	return CRegion(displayData.GetCoordinateCalRegion(), displayData.GetDisplayAttributes());
}

double CRaffRegressionDrawer::GetChannel() const
{	
	return m_dChannel;
}

///////////////////////////////////////////////////////////////////////////////
// class CAngleLineDrawer

CAngleLineDrawer::CAngleLineDrawer()
{	
}

void CAngleLineDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	BOOL bLeftLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetLeftExt();
	BOOL bRightLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetRightExt();
	CDrawer::DrawLineExt( pDC, pt1, pt2,FALSE,bLeftLineExt);

	long lLength = (long)sqrt(pow(pt1.x - pt2.x,2) + pow(pt1.y - pt2.y,2));
	CPoint ptStart(pt1);	
	CPoint ptEnd(pt2);	
	CPoint ptTempEnd(pt1.x + lLength, pt1.y);	
	CDrawer::DrawLineExt( pDC, pt1,ptTempEnd,FALSE,bLeftLineExt);

	CRect arcRect;	
	arcRect.SetRect(pt1.x - (int)(lLength *1/ 3.0), 
					pt1.y - (int)(lLength *1/ 3.0), 
					pt1.x + (int)(lLength *1/ 3.0), 
					pt1.y + (int)(lLength *1/ 3.0));

	ptStart.x = pt1.x + abs(pt1.x - ptTempEnd.x) *1 / 3;		
	if(pt2.x >= pt1.x)
	{
		ptEnd.x  = pt1.x + abs(pt1.x - pt2.x) *1/ 3;
	}
	else 
	{
		ptEnd.x  = pt1.x - abs(pt1.x - pt2.x) *1/ 3;
	}

	if(pt2.y <= pt1.y)
	{
		ptEnd.y  = pt1.y - abs(pt1.y - pt2.y) *1/ 3;
	}
	else 
	{
		ptEnd.y  = pt1.y + abs(pt1.y - pt2.y) *1/ 3;
	}
	
	pDC->Arc(arcRect,ptStart,ptEnd);	
}

void CAngleLineDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CTextShowPosition* pTextPosition = (CTextShowPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;
	
	if(pTextPosition->IsShow())
	{
		CPoint ptStart = drawingData.GetStartPoints().GetDrawingPoint();
		CPoint ptEnd   = drawingData.GetEndPoints().GetDrawingPoint();

		CString strAngle;
		double dAngleValue = 0.0;
		double dAngle = 0.0;
		if(ptEnd.y==ptStart.y)
		{
			dAngle = 0.0;
		}
		else
		{
			dAngleValue = (fabs((double)(ptEnd.y-ptStart.y))/fabs((double)(ptEnd.x-ptStart.x)));
			dAngle = (atan(dAngleValue) * 57.2957795130823);
			if(ptEnd.x != ptStart.x && abs(ptEnd.y - ptStart.y) <= 2 && dAngle < 0.18 && dAngleValue < 0.003)
			{
				dAngle = 180.0f;
			}
			else
			{				
				if(ptEnd.y < ptStart.y && ptEnd.x <= ptStart.x)
				{
					dAngle = 180.0f - dAngle;
				}
				else if(ptEnd.y > ptStart.y && ptEnd.x <= ptStart.x)
				{
					dAngle = 180.0f + dAngle;
				}
				else if(ptEnd.y > ptStart.y && ptEnd.x >= ptStart.x)
				{
					dAngle = 360.0f - dAngle;
				}
			}
		}			
		
		strAngle.Format(_T("%.lf˚"),dAngle);

		CDrawer::TEXTPOSITION eTextPosition = CDrawer::BOTTOM_RIGHT;
		CDrawer::DrawTextOut( pDC, eTextPosition, strAngle,ptStart);
	}	
}

///////////////////////////////////////////////////////////////////////////////
// class CCandleLineDrawer -- Candle중심선 그리기

CCandleLineDrawer::CCandleLineDrawer()
{
}

// protected ==================================================================
void CCandleLineDrawer::DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion)
{
	if(!isCurrentRegion)
		return;

	CRect region(displayData.GetDisplayRegion());
	if(!IsDrawing(region, point))
		return;

	CDrawer::DrawLine(pDC, CPoint(point.x, point.y), CPoint(region.right, point.y));
}

void CCandleLineDrawer::DrawText(CDC* pDC, const COnePointDrawingData& drawingData)
{
	if(!IsDrawing(drawingData.GetCurrentRegion(), drawingData.GetPoints().GetDrawingPoint()))
		return;

	CTextShowPosition* pTextPosition = (CTextShowPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	CString strPoint = drawingData.GetText_Y();
	if(strPoint.IsEmpty())  
		return;

	if(pTextPosition->IsShow())
	{
		CPoint ptDrawing = drawingData.GetPoints().GetDrawingPoint();
		CDrawer::TEXTPOSITION eTextPosition = CDrawer::BOTTOM_RIGHT;
		CDrawer::DrawTextOut( pDC, eTextPosition, strPoint,ptDrawing);
	}
}

bool CCandleLineDrawer::IsDrawing(const CRect& region, const CPoint& point) const
{
	return (region.top < point.y && region.bottom > point.y);
}

CRect CCandleLineDrawer::GetGripRegion(const CRect& region, const CPoint& point)
{
	return CDrawer::GetGripRegion(point);
}

///////////////////////////////////////////////////////////////////////////////
// class CCandleBongDrawer -- CandleBong 그리기

// protected ==================================================================
void CCandleBongDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData,const CPoint& pt1, const CPoint& pt2)
{
	CRect region(drawingData.GetDiaplayRegion());
	if(!IsDrawing(region, pt1))
		return;

	if(!IsDrawing(region, pt2))
		return;

	CRect rectBong;
	rectBong.left = min(pt1.x,pt2.x) - abs((pt1.x-pt2.x)/2)+1;
	rectBong.right = max(pt1.x,pt2.x) - abs((pt1.x-pt2.x)/2);
	rectBong.top = min(pt1.y,pt2.y);
	rectBong.bottom = max(pt1.y,pt2.y);

	COLORREF crBong = RGB(225,0,0);
	if(pt1.y < pt2.y)
		crBong = RGB(0,0,255);

	if(rectBong.Width() > 1 && rectBong.Height() > 1)
	{
		CDrawer::DrawRect(pDC,rectBong,crBong);
	}
	else
	{
		CPoint point2(pt2);
		if(rectBong.Width() <= 1)
			point2.x = pt1.x;

		if(rectBong.Height() <= 1)
			point2.y = pt1.y;
		CDrawer::DrawLine(pDC,pt1,point2);
	}
}

bool CCandleBongDrawer::IsDrawing(const CRect& region, const CPoint& point) const
{
	return (region.top < point.y && region.bottom > point.y);
}

///////////////////////////////////////////////////////////////////////////////
// class CPencilLineDrawer

void CPencilLineDrawer::DrawShape(CDC* pDC,const CMultiPointDrawingData& drawingData)
{	
	int nIndex = -1;
	CPoint ptStart;
	CPoint ptEnd;
	CPoints points;
	for(int i = 0;i<drawingData.GetPointsCount();i++)
	{
		points = drawingData.GetPoints(i);
		ptEnd = points.GetDrawingPoint();

		if(nIndex >= 0)
			CDrawer::DrawLine(pDC,ptStart,ptEnd);

		ptStart = ptEnd;
		nIndex++;
	}
}

///////////////////////////////////////////////////////////////////////////////
// class CPeriodSumLineDrawer

// protected ==================================================================
void CPeriodSumLineDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	CPoint ptFrom(pt1);
	CPoint ptTo(pt2);

	ptTo.y = ptFrom.y;
	CDrawer::DrawLine(pDC,ptFrom,ptTo);

	ptFrom.x = pt2.x;
	ptTo.y = pt2.y;
	CDrawer::DrawLine(pDC,ptFrom,ptTo);
}

void CPeriodSumLineDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CTextHorzPosition* pTextPosition = (CTextHorzPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	CPoint ptStart = drawingData.GetStartPoints().GetDrawingPoint();
	CPoint ptEnd   = drawingData.GetEndPoints().GetDrawingPoint();

	int nGap = 0;
	CString strLabel;	
	CString strStart;
	CString strEnd;	
	if(pTextPosition->GetLeft())
	{	
		CRegion coordinateCalRegion = GetCoordinateCalRegion(drawingData.GetDisplayData());
		int nIndex1 = CDrawer::GetCurrentDataIndex(coordinateCalRegion, ptStart);
		int nIndex2 = CDrawer::GetCurrentDataIndex(coordinateCalRegion, ptEnd);

		nGap = abs(nIndex1 - nIndex2);
		if(nGap == 0)
		{
			return;
		}

		strStart.Format("%.lf",drawingData.GetStartPoints().GetSavingPoint().GetX());
		strEnd.Format("%.lf",drawingData.GetEndPoints().GetSavingPoint().GetX());

		strLabel.Format("Count:%d(%s - %s)",nGap,strStart,strEnd);
		CDrawer::DrawTextOut( pDC, 
			strLabel,CRect(ptStart.x,ptStart.y+2,ptEnd.x,ptStart.y+2+pDC->GetTextExtent(strLabel).cy));
	}
	if( pTextPosition->GetRight())
	{
		double dStart = drawingData.GetStartPoints().GetSavingPoint().GetY();
		double dEnd = drawingData.GetEndPoints().GetSavingPoint().GetY();
		strStart = drawingData.GetText_Y(dStart);
		strEnd = drawingData.GetText_Y(dEnd);

		if(dStart < dEnd)
		{
			strLabel.Format("Ratio:+%.2f(%s -> %s)",fabs(dEnd - dStart)/dStart *100.0,strStart,strEnd);
		}	
		else if(dStart > dEnd)
		{
			strLabel.Format("Ratio:-%.2f(%s -> %s)",fabs(dEnd - dStart)/dStart *100.0,strStart,strEnd);
		}
		else 
		{
			strLabel.Format("Ratio:%.2f(%s -> %s)",fabs(dEnd - dStart)/dStart *100.0,strStart,strEnd);
		}

		nGap = min(ptStart.y,ptEnd.y) + abs(ptStart.y-ptEnd.y)/2;
		CDrawer::DrawTextOut( pDC, 
			strLabel,CRect(max(ptStart.x,ptEnd.x)+2,nGap,max(ptStart.x,ptEnd.x)+2+pDC->GetTextExtent(strLabel).cx,nGap+pDC->GetTextExtent(strLabel).cy));		
	}
}

CRegion CPeriodSumLineDrawer::GetCoordinateCalRegion(const CDisplayData& displayData) const
{
	return CRegion(displayData.GetCoordinateCalRegion(), displayData.GetDisplayAttributes());
}

///////////////////////////////////////////////////////////////////////////////
// class CElliotLineDrawer

// 해당 index번의 교차점.
bool CElliotLineDrawer::GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex,const CPoint pointFrom, CPoint& point) const
{
	double dElliotRatio = GetCoefficient(nIndex);
	int nElliottDirect[9] = {1,-1,1,-1,1,-1,1,-1};

	CPoint ptFrom(pt1);
	CPoint ptTo(pt2);
	
	int nXUnit = (int)(abs(ptTo.x-ptFrom.x));
	int nYUnit = (int)(abs(ptTo.y-ptFrom.y));
	if(nYUnit <=1 || nXUnit <=1)
	{	
		return false;
	}

	BOOL bReverseX = FALSE;	
	if(ptFrom.x > ptTo.x)
	{
		CPoint ptTemp;
		ptTemp = ptFrom;
		ptFrom = ptTo;
		ptTo = ptTemp;
		bReverseX = TRUE;
	}

	int nReverseY = 1;
	if(ptFrom.y > ptTo.y)
	{
		nReverseY = -1;
	}

	point.x = pointFrom.x + (int)(nXUnit * dElliotRatio);		
	point.y = pointFrom.y + (int)(nYUnit * dElliotRatio*nElliottDirect[nIndex]*nReverseY);
	
	return true;
}

// protected ==================================================================
void CElliotLineDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	int nElliottDirect[9] = {1,-1,1,-1,1,-1,1,-1};

	CPoint ptFrom(pt1);
	CPoint ptTo(pt2);
	
	int nXUnit = (int)(abs(ptTo.x-ptFrom.x));
	int nYUnit = (int)(abs(ptTo.y-ptFrom.y));
	if(nYUnit <=1 || nXUnit <=1)
	{	
		return;
	}

	CPoint ptTemp;
	BOOL bReverseX = FALSE;	
	if(ptFrom.x > ptTo.x)
	{
		ptTemp = ptFrom;
		ptFrom = ptTo;
		ptTo = ptTemp;
		bReverseX = TRUE;
	}

	int nReverseY = 1;
	if(ptFrom.y > ptTo.y)
	{
		nReverseY = -1;
	}

	CDrawer::DrawLine(pDC,ptFrom,ptTo);		
	ptFrom = ptTo;
	for(int i=1;i<GetCount();i++)
	{
		ptTo.x = ptFrom.x + (int)(nXUnit * GetCoefficient(i));
		ptTo.y = ptFrom.y + (int)(nYUnit * GetCoefficient(i)*nElliottDirect[i]*nReverseY);
		CDrawer::DrawLine(pDC,ptFrom,ptTo);	
		ptFrom = ptTo;
	}
}

double CElliotLineDrawer::GetCoefficient(const int nIndex) const
{
	switch(nIndex)
	{
	case 0:
		return 1.0;
	case 1:
		return 0.382;//0번 파동의 38.2%,혹은 61.8만큼	
	case 2:
		return 1.618;
	case 3:
		return 0.618;//3번파동의 38.2%만큼 되돌림
	case 4:
		return 1.0;//0번길이 만큼 혹은,0번에서 2번까지의 61.8만큼
	case 5:
		return 1.382;
	case 6:
		return 0.618;
	case 7:
		return 1.618;	
	}

	assert(false);
	return 0.0;
}

int CElliotLineDrawer::GetCount() const
{
	return 8;
}

///////////////////////////////////////////////////////////////////////////////
// class CAdjustRatioDrawer

// protected ==================================================================
void CAdjustRatioDrawer::DrawShape(CDC* pDC, const CRegion& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3)
{
}