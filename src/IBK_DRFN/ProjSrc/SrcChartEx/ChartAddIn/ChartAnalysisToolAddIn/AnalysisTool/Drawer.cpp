// Drawer.cpp: implementation of the CDrawer class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Drawer.h"

#include "AnalysisData.h"
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
// class CDrawer

CDrawer::CDrawer():
	nGripSize(4),
	m_gripBrushColor(RGB(255, 255, 255)), 
	m_nOrgDrawMode(R2_COPYPEN), 
	m_nOrgBkMode(TRANSPARENT), 
	m_orgTextColor(RGB(0, 0, 0))
{
}

// public =====================================================================
// grip 색.
void CDrawer::SetGripBrushColor(const COLORREF& color)
{
	m_gripBrushColor = color;
}

void CDrawer::DrawRectangle(CDC* pDC, const CRect& region)
{
	pDC->Rectangle(region);
}

// protected ==================================================================
// 그리는 영역을 정한다.
void CDrawer::SelectClipRegion(CDC* pDC, const CRect& rect)
{
	CRgn rgn;
	rgn.CreateRectRgn((int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom+1);
	pDC->SelectClipRgn(&rgn);
}

// 그리는 영역기본값으로 돌린다.
void CDrawer::SelectClipRegion(CDC* pDC)
{
	pDC->SelectClipRgn(NULL);
}

void CDrawer::SelectNewDrawingMode(CDC* pDC, const CRect& rect, const int nDrawMode)
{
	SelectClipRegion(pDC, rect);
	m_nOrgDrawMode = pDC->SetROP2(nDrawMode);
}

void CDrawer::SelectOrgDrawingMode(CDC* pDC)
{
	SelectClipRegion(pDC);
	pDC->SetROP2(m_nOrgDrawMode);
}

void CDrawer::SelectNewTextMode(CDC* pDC, const COLORREF& textColor)
{
	// text 는 항상 R2_COPYPEN 으로 한다.
	m_nOrgDrawMode = pDC->SetROP2(R2_COPYPEN);
	m_nOrgBkMode = pDC->SetBkMode(TRANSPARENT);
	m_orgTextColor = pDC->SetTextColor(textColor);
}

void CDrawer::SelectOrgTextMode(CDC* pDC)
{
	pDC->SetROP2(m_nOrgDrawMode);
	pDC->SetBkMode(m_nOrgBkMode);
	pDC->SetTextColor(m_orgTextColor);
}

// ----------------------------------------------------------------------------
void CDrawer::DrawLineExt(CDC* pDC, const CPoint& moveToPt, const CPoint& lineToPt,BOOL bLeft,BOOL bRight)
{
	if( bLeft != TRUE && bRight != TRUE)
	{
		pDC->MoveTo(moveToPt);
		pDC->LineTo(lineToPt);
		return;
	}

	CPoint ptStart( moveToPt), ptEnd( lineToPt);

	CPoint ptTemp;
	// 2.1 먼저 기본 방향성을 확인하기 위한 양축의 변화량을 확인한다.
	if(bLeft == TRUE && bRight == FALSE && ptEnd.x < ptStart.x)
	{
		ptTemp = ptStart;
		ptStart = ptEnd;
		ptEnd = ptTemp;
	}
	
	if(bRight == TRUE && bLeft == FALSE && ptEnd.x < ptStart.x)
	{
		ptTemp = ptStart;
		ptStart = ptEnd;
		ptEnd = ptTemp;
	}

	double dDX = ptEnd.x - ptStart.x;
	double dDY = ptEnd.y - ptStart.y;
	// 2.2 Logic을 간단히 하기 위하여 우하단(+,+)의 방향으로 조정하고 Flag을 관리한다.
	BOOL bXTransformation = ( dDX < 0);
	if( bXTransformation)
	{
		dDX = -dDX;
		ptEnd.x = ptStart.x + ( int)dDX;
	}
	BOOL bYTransformation = ( dDY < 0);
	if( bYTransformation)
	{
		dDY = -dDY;
		ptEnd.y = ptStart.y + ( int)dDY;
	}
	// 2.3 확장을 처리한다.
	// 2.3.1 각이 크기를 감안하여 진행 방향확장을 처리한다.
	CPoint ptExtEnd( ptEnd);
	CPoint ptExtStart( ptStart);
	if(bRight == TRUE)
	{
		if( dDX < dDY)
		{
			ptExtEnd.y = 10000;
			ptExtEnd.x = int( ( dDX / dDY) * ( ptExtEnd.y - ptStart.y) + ptStart.x);
		}
		else if( dDY < dDX)
		{
			ptExtEnd.x = 10000;
			ptExtEnd.y = int( ( dDY / dDX) * ( ptExtEnd.x - ptStart.x) + ptStart.y);
		}
	}

	// 2.3.1 각이 크기를 감안하여 진행 역방향확장을 처리한다.		
	if(bLeft == TRUE)
	{
		if( dDX < dDY)
		{
			ptExtStart.y = -10000;
			ptExtStart.x = int( ( dDX / dDY) * ( ptExtStart.y - ptStart.y) + ptStart.x);
		}
		else if( dDY < dDX)
		{
			ptExtStart.x = -10000;
			ptExtStart.y = int( ( dDY / dDX) * ( ptExtStart.x - ptStart.x) + ptStart.y);
		}
	}
	// 2.4 기본 방향으로 조정한다.
	if( bXTransformation)
	{
		dDX	= -dDX;
		ptEnd.x			= ptStart.x - ( int)dDX;
		ptExtStart.x	= ptStart.x - ( ptExtStart.x	- ptStart.x);
		ptExtEnd.x		= ptStart.x - ( ptExtEnd.x		- ptStart.x);
	}
	if( bYTransformation)
	{
		dDY = -dDY;
		ptEnd.y			= ptStart.y - ( int)dDY;
		ptExtStart.y	= ptStart.y - ( ptExtStart.y	- ptStart.y);
		ptExtEnd.y		= ptStart.y - ( ptExtEnd.y		- ptStart.y);
	}
	// 2.5 확장된 Point를 Drawing Point로 지정한다.
	ptStart	= ptExtStart;
	ptEnd	= ptExtEnd;

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);
}

void CDrawer::DrawLine(CDC* pDC, const CPoint& moveToPt, const CPoint& lineToPt)
{
	pDC->MoveTo(moveToPt);
	pDC->LineTo(lineToPt);
}

void CDrawer::DrawPolygonLine(CDC* pDC, const LPPOINT pPoint, const int& nCount)
{
	pDC->Polygon(pPoint,nCount);
}

void CDrawer::DrawFrameRect(CDC* pDC, const CRect& region)
{
	CPoint point(region.TopLeft());
	pDC->MoveTo(point);
	point.x = region.right;
	pDC->LineTo(point);
	point.y = region.bottom;
	pDC->LineTo(point);
	point.x = region.left;
	pDC->LineTo(point);
	point.y = region.top;
	pDC->LineTo(point);
}

void CDrawer::DrawFrameRect(CDC* pDC, const CPoint& moveToPt, const CPoint& lineToPt)
{
	CPoint point(lineToPt);
	pDC->MoveTo(moveToPt);
	point.y = moveToPt.y;
	pDC->LineTo(point);
	point.y = lineToPt.y;
	pDC->LineTo(point);
	point.x = moveToPt.x;
	pDC->LineTo(point);
	point.y = moveToPt.y;
	pDC->LineTo(point);
}

void CDrawer::DrawRect(CDC* pDC, const CRect& region, const COLORREF& color, const bool bIsFill)
{
	CBrush brush(color);
	if(bIsFill)
		pDC->FillRect(region, &brush);
	else
		pDC->FrameRect(region, &brush);

	brush.DeleteObject();
}

void CDrawer::DrawRectangle(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor, const bool bIsFill)
{
	CBrush newBrush(brushColor);
	CBrush* pOldBrush = NULL;
	if(bIsFill)
		pOldBrush = pDC->SelectObject(&newBrush);
	else
		pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	pDC->Rectangle(region);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	newBrush.DeleteObject();
	newPen.DeleteObject();
}

// ----------------------------------------------------------------------------
void CDrawer::DrawTextOut(CDC* pDC, const TEXTPOSITION eTextPosition, const CString& data, const CPoint& point, const CSize& maxSize)
{
	CPoint position = GetTextPosition(eTextPosition, point, pDC->GetTextExtent(data), maxSize);
//2008.10.06 by LYH >> 추세선 수치가 좌측 혹은 우측 밖으로 넘어가지 않도록 보완
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) != ERROR)
	{
		CSize textSize = pDC->GetTextExtent(data);
		//right
		//2008.10.20 by LYH >> 추세선 left text 오류 수정
		//if(rectClip.right < (position.x + textSize.cx + 5))
		if(rectClip.right < (position.x + textSize.cx + 5) && position.x < rectClip.right)
		{
			position.x = rectClip.right - textSize.cx - 5;
			position.y = position.y - 3;
		}
		//2008.10.20 by LYH <<

		//left
		//2008.10.20 by LYH >> 추세선 left text 오류 수정
		//if( position.x < 5)
		if( (position.x+textSize.cx)> 0 && position.x < 5)
		//2008.10.20 by LYH <<
		{
			position.x = 5;
			position.y = position.y + 7;
		}
	}
//2008.10.06 by LYH <<
	pDC->TextOut(position.x, position.y, data);
}

void CDrawer::DrawTextOut(CDC* pDC, const CString& data, const CRect& region, const COLORREF& textColor)
{
	COLORREF orgTextColor = pDC->SetTextColor(textColor);

	DrawTextOut(pDC, data, region);
	
	pDC->SetTextColor(orgTextColor);
}

void CDrawer::DrawTextOut(CDC* pDC, const CString& data, const CRect& region)
{
	CRect realRect = region;
	CSize textSize = pDC->GetTextExtent(data);
	int nOffset_y = (region.Height() - textSize.cy)/2 +1;
	if(nOffset_y > 1)
		realRect.DeflateRect(0, nOffset_y);

	int nHeight = realRect.Height();
	if(textSize.cy > nHeight)
		realRect.bottom += (textSize.cy - nHeight);

	pDC->DrawText(data, realRect, DT_VCENTER | DT_CENTER);
}

void CDrawer::DrawTextOut_Path(CDC* pDC, const CString& data, const CPoint& point)
{
	int nOldMode = pDC->SetBkMode(TRANSPARENT);

	pDC->BeginPath();
	pDC->TextOut(point.x, point.y, data);

	pDC->EndPath();
	pDC->StrokePath();

	pDC->SetBkMode(nOldMode);
}

// ----------------------------------------------------------------------------
// grip 색.
COLORREF CDrawer::GetGripBrushColor() const
{
	return m_gripBrushColor;
}

// grip 영역
CRect CDrawer::GetGripRegion(const CPoint& centerPoint) const
{
	CRect gripRegion(centerPoint, CSize(1, 1));
	gripRegion.InflateRect(nGripSize -1, nGripSize -1);
	return gripRegion;
}

// text 영역.
CRect CDrawer::GetTextRegion(const CSize& textSize, const CPoint& orgPoint, const bool bLeft, const bool bTop) const
{
	if(textSize == CSize(0, 0))
		return CRect(0, 0, 0, 0);

	CRect region = CRect(0, 0, textSize.cx + MARGIN*3/2, textSize.cy + MARGIN);
	if(bLeft)
		region.OffsetRect(CPoint(orgPoint.x, 0));
	else
		region.OffsetRect(CPoint(orgPoint.x - region.Width(), 0));

	if(bTop)
		region.OffsetRect(CPoint(0, orgPoint.y));
	else
		region.OffsetRect(CPoint(0, orgPoint.y +1));

	return region;
}

// ----------------------------------------------------------------------------
// 교차점을 구한다.
CPoint CDrawer::GetLineEndPoint(const CPoint& line1_pt1, const CPoint& line1_pt2, const CPoint& line2_pt1, const CPoint& line2_pt2) const
{
	CPoint point;
	if(Intersection(line1_pt1, line1_pt2, line2_pt1, line2_pt2, point))
		return point;

	return line1_pt1;
}

bool CDrawer::Intersection(const CPoint& line1_pt1, const CPoint& line1_pt2, const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const
{
	// 하나라도 선이 아니면 false.
	if(line1_pt1 == line1_pt2 || line2_pt1 == line2_pt2)
		return false;

	// 수평선
	CDrawer::INTERSECTION_RESULT result = IsIntersectionPoint_InHorzLine(line1_pt1, line1_pt2,	line2_pt1, line2_pt2, point);
	if(result != CDrawer::NO_RESULT)
		return Result(result);

	// 수직선
	result = IsIntersectionPoint_InVertLine(line1_pt1, line1_pt2, line2_pt1, line2_pt2, point);
	if(result != CDrawer::NO_RESULT)
		return Result(result);

	// 사선
	return GetIntersectionPoint_Slant(line1_pt1, line1_pt2, line2_pt1, line2_pt2, point);
}

// ----------------------------------------------------------------------------
// 좌표의 기울기 : Y = aX + b 중 a
// --> 두점의 기울기 : (y2-y1)/(x2-x1)
double CDrawer::GetLineAngle(const CPoint& point1, const CPoint& point2) const
{
	// X = a (x),  Y = a (o : 기울기 = 0)
	if(point1.x == point2.x || point1.y == point2.y)
		return 0.0;

	// Y = aX + b (o : 기울기 = a)
	return ((double)(point2.y - point1.y) / (double)(point2.x - point1.x));
}

// y 절편 : Y = aX + b 중 b
double CDrawer::GetLineIntercept(const double& dAngle, const CPoint& point) const
{
	return ((double)point.y - (dAngle * (double)point.x));
}

// ----------------------------------------------------------------------------
// 현재 point의 index를 가져오기.
int CDrawer::GetCurrentDataIndex(const CRegion& region, const CPoint& point) const
{
	// XScale : 관련 Interface로 사용 - vntsorl(20070327)
	return CDataConversion::GetPacketDataIndex_Date(region.GetRect(), point, 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock());
//	CCoordinateCalculatorData calData;
//	CPoints points = GetPoints(point, calData);
//	return calData.GetIChartManager()->FindViewDataIndex_Pt(point.x);
}

// 그릴 x좌표 가져오기.
int CDrawer::GetDrawingPoint_X(const CRegion& region, const int nIndex, const bool bIsCenter) const
{
	// XScale : 관련 Interface로 사용 - vntsorl(20070327)
	return CDataConversion::RelativeToAbsolute_X(region.GetRect(), 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock(), nIndex, bIsCenter);
//	CCoordinateCalculatorData calData;
//	CPoints points = GetPoints(point, calData);
//	return calData.GetIChartManager()->GetXPosition(nIndex, 0);
}

/*// 그릴 Y좌표 가져오기.
int CDrawer::GetDrawingPoint_X(const CRegion& region,const double dValue) const
{
	return CDataConversion::RelativeToAbsolute_Y(region.GetRect(),dValue, 
			region.GetDisplayAttributes()
			region.GetDisplayAttributes().GetDataEndInBlock(), nIndex, bIsCenter);
}*/

// private ====================================================================
// 수직선 여부. X = a
bool CDrawer::IsVerticalLine(const CPoint& point1, const CPoint& point2) const
{
	return (point1.x == point2.x);
}

// 수평선 여부. Y = a
bool CDrawer::IsHorizontalLine(const CPoint& point1, const CPoint& point2) const
{
	return (point1.y == point2.y);
}

// ----------------------------------------------------------------------------
//교점 구하기.
// -- 두선 중 수평선
CDrawer::INTERSECTION_RESULT CDrawer::IsIntersectionPoint_InHorzLine(const CPoint& line1_pt1, const CPoint& line1_pt2, 
			const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const
{
	// line1 - 수평선
	bool bIsHorzLine1 = IsHorizontalLine(line1_pt1, line1_pt2);
	bool bIsHorzLine2 = IsHorizontalLine(line2_pt1, line2_pt2);

	// 모두 수평선 아닐 경우.
	if(!bIsHorzLine1 && !bIsHorzLine2)
		return CDrawer::NO_RESULT;
	// line1 수평선일 경우.
	else if(bIsHorzLine1 && !bIsHorzLine2)
		return GetIntersectionPoint_InHorzLine1(line1_pt1, line1_pt2, line2_pt1, line2_pt2, point);
	// line2 수평선일 경우.
	else if(!bIsHorzLine1 && bIsHorzLine2)
		return GetIntersectionPoint_InHorzLine1(line2_pt1, line2_pt2, line1_pt1, line1_pt2, point);

	return CDrawer::FALSE_RESULT;
}

// -- line1 수평선, line2 수평선이 아닌 선.
CDrawer::INTERSECTION_RESULT CDrawer::GetIntersectionPoint_InHorzLine1(const CPoint& line1_pt1, const CPoint& line1_pt2, 
			const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const
{
	if(!IsHorizontalLine(line1_pt1, line1_pt2) || IsHorizontalLine(line2_pt1, line2_pt2))
		return CDrawer::FALSE_RESULT;

	// line2 - 수직선.
	if(IsVerticalLine(line2_pt1, line2_pt2)){
		point = CPoint(line2_pt1.x, line1_pt1.y);
	}
	// line2 - 사선.
	else{
		double dAngle_Line2 = GetLineAngle(line2_pt1, line2_pt2);
		double dx = (line1_pt1.y - GetLineIntercept(dAngle_Line2, line2_pt1)) / dAngle_Line2;
		point = CPoint(CMath::Round(dx), line1_pt1.y);
	}

	return CDrawer::TRUE_RESULT;
}

// -- 두선 중 수직선
CDrawer::INTERSECTION_RESULT CDrawer::IsIntersectionPoint_InVertLine(const CPoint& line1_pt1, const CPoint& line1_pt2, 
			const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const
{
	// line1 - 수직선
	bool bIsVertLine1 = IsVerticalLine(line1_pt1, line1_pt2);
	bool bIsVertLine2 = IsVerticalLine(line2_pt1, line2_pt2);

	// 모두 수직선 아닐 경우.
	if(!bIsVertLine1 && !bIsVertLine2)
		return CDrawer::NO_RESULT;
	// line1 수직선일 경우.
	else if(bIsVertLine1 && !bIsVertLine2)
		return GetIntersectionPoint_InVertLine1(line1_pt1, line1_pt2, line2_pt1, line2_pt2, point);
	// line2 수직선일 경우.
	else if(!bIsVertLine1 && bIsVertLine2)
		return GetIntersectionPoint_InVertLine1(line2_pt1, line2_pt2, line1_pt1, line1_pt2, point);

	return CDrawer::FALSE_RESULT;
}

// -- line1 수직선, line2 수직선이 아닌 선.
CDrawer::INTERSECTION_RESULT CDrawer::GetIntersectionPoint_InVertLine1(const CPoint& line1_pt1, const CPoint& line1_pt2, 
			const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const
{
	if(!IsVerticalLine(line1_pt1, line1_pt2) || IsVerticalLine(line2_pt1, line2_pt2))
		return CDrawer::FALSE_RESULT;

	// line2 - 수평선.
	if(IsVerticalLine(line2_pt1, line2_pt2))
		point = CPoint(line1_pt1.x, line2_pt1.y);
	// line2 - 사선.
	else{
		double dAngle_Line2 = GetLineAngle(line2_pt1, line2_pt2);
		double dy = (dAngle_Line2 * line1_pt1.x) + GetLineIntercept(dAngle_Line2, line2_pt1);
		point = CPoint(line1_pt1.x, CMath::Round(dy));
	}

	return CDrawer::TRUE_RESULT;
}

// --> 두선 모두 사선
bool CDrawer::GetIntersectionPoint_Slant(const CPoint& line1_pt1, const CPoint& line1_pt2, 
			const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const
{
	double dAngle_Line1 = GetLineAngle(line1_pt1, line1_pt2);
	double dAngle_Line2 = GetLineAngle(line2_pt1, line2_pt2);
	double dIntercept_Line1 = GetLineIntercept(dAngle_Line1, line1_pt1);
	double dIntercept_Line2 = GetLineIntercept(dAngle_Line2, line2_pt1);

	if(dAngle_Line1 == dAngle_Line2)
		return false;

	double dx = (dIntercept_Line2 - dIntercept_Line1) / (dAngle_Line1 - dAngle_Line2);
	double dy = dAngle_Line1 * dx + dIntercept_Line1;

	point = CPoint(CMath::Round(dx), CMath::Round(dy));	
	return true;
}

bool CDrawer::Result(const CDrawer::INTERSECTION_RESULT result) const
{
	return (result == CDrawer::TRUE_RESULT);
}

// ----------------------------------------------------------------------------
bool CDrawer::Intersection(double x1, double y1, double x2, double y2,
			double bx1, double by1, double bx2, double by2, double &ix, double &iy) const
{
	double a1, a2;
	int status;

	if (IsSame(x1, y1, bx1, by1) || IsSame(x1, y1, bx2, by2)){
		ix = x1;	iy = y1;
		return true;
	} 
	else if (IsSame(x2, y2, bx1, by1) || IsSame(x2, y2, bx2, by2)){
		ix = x2;	iy = y2;
		return true;
	}
		
	status = 0;
	if ((x1 - x2) == 0) status += 1;	//1
	if ((y1 - y2) == 0) status += 2;	//2
	if ((bx1 - bx2) == 0) status += 4;	//4
	if ((by1 - by2) == 0) status += 8;	//8
	if (!(status & 0x0001)) 
		a1 = ((double)y1 - y2)/((double)x1 - x2);
	if (!(status & 0x0004)) 
		a2 = ((double)by1-by2)/((double)bx1 - bx2);
	switch (status) {
	case 0 : case 10 : case 2 : case 8 :
		ix = (by1 - y1 + a1*x1 - a2*bx1)/(a1-a2);
		iy = a1*(ix - x1) + y1;
		return true;
	case 1 : case 9 : // y = x
		ix = x1;
		iy = a2 * (ix - bx1) + by1;
		return true;
	case 4 : case 6 : // y = bx1
		ix = bx1;
		iy = a1 * (ix - x1) + y1;
		return true;
	case 5 :  
		return false; //조건에 위배됨
	case 3 : case 7 : case 11 : case 12 : case 13 : case 14 : case 15 : 
		return false; //사용자가 검색 필수
	}
	return false;
}

bool CDrawer::IsSame(double x1, double y1, double x2, double y2) const
{
	return (x1 == x2 && y1 == y2);
}

// ----------------------------------------------------------------------------
// text 위치
CPoint CDrawer::GetTextPosition(const CDrawer::TEXTPOSITION eTextPosition, const CPoint& orgPoint, const CSize& textSize, const CSize& maxSize) const
{
	switch( eTextPosition)
	{
		case LEFT:			return CPoint(orgPoint.x - (nGripSize + textSize.cx), orgPoint.y - (textSize.cy / 2));
		case RIGHT:
		{
			CPoint point = orgPoint;
			if(maxSize.cx > textSize.cx) point.x += (maxSize.cx - textSize.cx);
			return CPoint(point.x + nGripSize, point.y - (textSize.cy / 2));
		}
		case TOP:			return CPoint(orgPoint.x - (textSize.cx / 2),			orgPoint.y - (nGripSize + textSize.cy));
		case BOTTOM:		return CPoint(orgPoint.x - (textSize.cx / 2),			orgPoint.y + nGripSize);
		// (2004.12.09, 배승원) 대각선 방향의 TEXT 설정도 제공한다.
		case TOP_LEFT:		return CPoint( orgPoint.x - (nGripSize + textSize.cx),	orgPoint.y - textSize.cy);
		case TOP_RIGHT:		return CPoint( orgPoint.x + nGripSize,					orgPoint.y - textSize.cy);
		case BOTTOM_LEFT:	return CPoint( orgPoint.x - (nGripSize + textSize.cx),	orgPoint.y + 2);
		case BOTTOM_RIGHT:	return CPoint( orgPoint.x + nGripSize,					orgPoint.y + 2);
	}	

	return orgPoint;
}

// ----------------------------------------------------------------------------
/*
void CDrawer::DrawLine(CDC* pDC, const CPoint& moveToPt, const CPoint& lineToPt, const int drawMode)
{
	int oldDrawMode = pDC->GetROP2();
	pDC->SetROP2(drawMode);
	pDC->MoveTo(moveToPt);
	pDC->LineTo(lineToPt);
	pDC->SetROP2(oldDrawMode);
}

void CDrawer::DrawLine(CDC* pDC, const CRect& rect, const CPoint& moveToPt, const CPoint& lineToPt)
{
	SelectClipRegion(pDC, rect);
	pDC->MoveTo(moveToPt);
	pDC->LineTo(lineToPt);
	SelectClipRegion(pDC);
}

void CDrawer::DrawLine(CDC* pDC, const CRect& rect, const CPoint& moveToPt, const CPoint& lineToPt, const int drawMode)
{
	int oldDrawMode = pDC->GetROP2();
	pDC->SetROP2(drawMode);
	SelectClipRegion(pDC, rect);
	pDC->MoveTo(moveToPt);
	pDC->LineTo(lineToPt);
	SelectClipRegion(pDC);
	pDC->SetROP2(oldDrawMode);
}*/

// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
// 중심선을 dot로 그리기 위해 추가함
void CDrawer::DrawStyleLine(CDC* pDC,  const CPoint& moveToPt, const CPoint& lineToPt, const UINT nStyle)
{
	LOGPEN logPen;
	CPen* pCurrentPen = pDC->GetCurrentPen();
	if( pCurrentPen )
		pCurrentPen->GetLogPen( &logPen );
	
	logPen.lopnStyle = nStyle;
	logPen.lopnWidth = CPoint(1,1);
	CPen newPen;
	if( !newPen.CreatePenIndirect( &logPen ) )
		return;
	
	CPen* pOldPen = pDC->SelectObject(&newPen);
	
	pDC->MoveTo( moveToPt );
	pDC->LineTo( lineToPt );
	
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}
// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<