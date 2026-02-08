// Line.cpp: implementation of the CLine class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Line.h"

#include <assert.h>
#include "DataMath.h"
#include "DrawingData.h"
#include "CoordinateCalculator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CLine

CLine::CLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2,
			 const CAnalysis::TOOLTYPE p_eToolType, const CElementCommonEnvironment& environment):
			 CTwoPointElement(point1, point2, environment),
			 m_textPosition(position),
			 m_lineDrawer(p_eToolType)
{
	m_eToolType = p_eToolType;
}

CToolEnumData::ElementType CLine::GetElementType() const
{
	return CToolEnumData::Line;
}

// protected ==================================================================
const CTextPosition* CLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CLine::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CLine::GetTwoPointDrawer()
{
	return m_lineDrawer;
}

bool CLine::ElementInRegion( const CRect &p_rctGraphArea, const CPoint &p_ptStart, const CPoint &p_ptEnd, const CPoint &p_ptCursor) const
{
	BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();

	CRect rctLineRect = CElement::GetLineRect( p_rctGraphArea, p_ptStart, p_ptEnd,bLeftExt,bRightExt);
	//rctLineRect.InflateRect(2,2,2,2);
	rctLineRect.InflateRect(1,1,1,1);
	if( rctLineRect.Width()		< 4) rctLineRect.InflateRect( CSize( ( 4 - rctLineRect.Width()) / 2,0));
	if( rctLineRect.Height()	< 4) rctLineRect.InflateRect( CSize( 0,( 4 - rctLineRect.Height()) / 2));
	return ( rctLineRect.PtInRect( p_ptCursor) ? true : false);
}

CToolEnumData::ModifyType CLine::GetModifyType( const CRect &p_rctGraphArea, const CPoint &p_ptStart, const CPoint &p_ptEnd, const CPoint &p_ptCursor) const
{
	// 1. 먼저 제어점의 위치를 확인토록 한다.
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints( p_ptStart, p_ptEnd, p_ptCursor);
	if( sidePointModifyType != CToolEnumData::MODIFY_NONE) 
		return sidePointModifyType;

	BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();
	
	// 2. Line의 추세방향을 기반으로 Cursor가 Line상(대각선)에 위치했는지 확인한다.
	CRect rctExtLineRect = CElement::GetLineRect(p_rctGraphArea,p_ptStart,p_ptEnd,bLeftExt,bRightExt);
	if( 0 < ( p_ptEnd.x - p_ptStart.x) * ( p_ptEnd.y - p_ptStart.y))
		return GetModifyType_Line( rctExtLineRect.TopLeft(), rctExtLineRect.BottomRight(), p_ptCursor);
	
	return GetModifyType_Line(	CPoint( rctExtLineRect.left, rctExtLineRect.bottom),
								CPoint( rctExtLineRect.right, rctExtLineRect.top), p_ptCursor);
}

///////////////////////////////////////////////////////////////////////////////
// class CVertLine

CVertLine::CVertLine(const CTextVertPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment):
	CVertElement(point, environment),
	m_textPosition(position)
{
}

CToolEnumData::ElementType CVertLine::GetElementType() const
{
	return CToolEnumData::Vert_Line;
}

// protected ==================================================================
const CTextPosition* CVertLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CVertLine::GetTextPosition()
{
	return &m_textPosition;
}

COnePointDrawer& CVertLine::GetOnePointDrawer()
{
	return m_vertLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CHorzLine

CHorzLine::CHorzLine(const CTextHorzPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment):
	COnePointElement(point, environment),
	m_textPosition(position)
{
}

// public =====================================================================
CToolEnumData::ElementType CHorzLine::GetElementType() const
{
	return CToolEnumData::Horz_Line;
}

// 해당 modifyType에 맞게 변경
void CHorzLine::ElementModify(const CElementModifyData& elementModifyData)
{
	assert(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_Y);
	COnePointElement::SetPoint(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
}

void CHorzLine::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}
void CHorzLine::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
}

// protected ==================================================================
const CTextPosition* CHorzLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CHorzLine::GetTextPosition()
{
	return &m_textPosition;
}

COnePointDrawer& CHorzLine::GetOnePointDrawer()
{
	return m_horzLineDrawer;
}

// 수정할 type 가져오기
CToolEnumData::ModifyType CHorzLine::GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const
{
	if(PointInElementSelectableRegion(displayData.GetDisplayRegion(), elementDrawingPt, point))
		return CToolEnumData::MODIFY_Y;
	return CToolEnumData::MODIFY_NONE;
}

// private ====================================================================
// 선택할수 있는 범위
bool CHorzLine::PointInElementSelectableRegion(const CRect& region, const CPoint& regionPt, const CPoint& point) const
{
	return CElement::PointInElementSelectableRegion(CPoint(region.left, regionPt.y), CPoint(region.right, regionPt.y), point);
}

///////////////////////////////////////////////////////////////////////////////
// class CCrossLine

CCrossLine::CCrossLine(const CTextHorzVertPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment):
	CVertElement(point, environment),
	m_textPosition(position)
{
}

// public =====================================================================
CToolEnumData::ElementType CCrossLine::GetElementType() const
{
	return CToolEnumData::Cross_Line;
}

// 해당 modifyType에 맞게 변경
void CCrossLine::ElementModify(const CElementModifyData& elementModifyData)
{
	//>> (2008/12/16 - Seung-Lyong Park) 분석도구 복사
	if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_CENTER){
		CCoordinate modifyPoint(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
		
		if(modifyPoint.GetX() == 0.0 || modifyPoint.GetIndex() < 0)
		{
			modifyPoint = elementModifyData.GetModifyPoint(COnePointElement::GetPoint());
			if(modifyPoint.GetX() == 0.0 || modifyPoint.GetIndex() < 0)
				return;
		}

		COnePointElement::SetPoint(modifyPoint);
//		COnePointElement::SetPoint(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
	}
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_X)
		ElementModify_X(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_Y)
		ElementModify_Y(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
	else
		assert(false);
	//<< (2008/12/16 - Seung-Lyong Park) 분석도구 복사
}

void CCrossLine::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
}
void CCrossLine::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}
// ----------------------------------------------------------------------------
void CCrossLine::DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData)
{
	if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_CENTER)
	{
		COnePointDrawingData drawingData = COnePointElement::GetOnePointDrawingData(selectedDrawingData);
		m_crossLineDrawer.DrawTool(pDC, drawingData, R2_NOT);
	}
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_X)
		m_crossLineDrawer.SelectedDraw_VertLine(pDC, *selectedDrawingData.GetDrawingRegions(), 
			COnePointElement::GetSelectedDrawPoint(selectedDrawingData));
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_Y)
		m_crossLineDrawer.SelectedDraw_HorzLine(pDC, *selectedDrawingData.GetDrawingRegions(), 
			COnePointElement::GetSelectedDrawPoint(selectedDrawingData));
	else
		assert(false);
}

// protected ==================================================================
const CTextPosition* CCrossLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CCrossLine::GetTextPosition()
{
	return &m_textPosition;
}

COnePointDrawer& CCrossLine::GetOnePointDrawer()
{
	return m_crossLineDrawer;
}

// 수정할 type 가져오기
CToolEnumData::ModifyType CCrossLine::GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const
{
	bool bHorz = CElement::PointInElementSelectableRegion(CPoint(elementDrawingPt.x, displayData.GetDisplayRegion().top), CPoint(elementDrawingPt.x, displayData.GetDisplayRegion().bottom), point);
	bool bVert = CElement::PointInElementSelectableRegion(CPoint(displayData.GetDisplayRegion().left, elementDrawingPt.y), CPoint(displayData.GetDisplayRegion().right, elementDrawingPt.y), point);

	if(bHorz && bVert)
		return CToolEnumData::MODIFY_CENTER;
	else if(bHorz && !bVert)
		return CToolEnumData::MODIFY_X;
	else if(!bHorz && bVert)
		return CToolEnumData::MODIFY_Y;

	return CToolEnumData::MODIFY_NONE;
}

// private ====================================================================
void CCrossLine::ElementModify_X(const CCoordinate& modifyPoint)
{
	COnePointElement::GetPoint().SetX(modifyPoint.GetX(), modifyPoint.GetIndex());
}

void CCrossLine::ElementModify_Y(const CCoordinate& modifyPoint)
{
	COnePointElement::GetPoint().SetY(modifyPoint.GetY());
}

///////////////////////////////////////////////////////////////////////////////
// class CEqualDivisionLine

CEqualDivisionLine::CEqualDivisionLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment),
	m_textPosition(position)
{
}

// protected ==================================================================
const CTextPosition* CEqualDivisionLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CEqualDivisionLine::GetTextPosition()
{
	return &m_textPosition;
}

CToolEnumData::ModifyType CEqualDivisionLine::GetModifyType(const CRect& region, const CPoint& drawingStartPt, 
															const CPoint& drawingEndPt, const CPoint& point) const
{
	CToolEnumData::ModifyType eModifyType = CTwoPointElement::GetModifyType(region, drawingStartPt, drawingEndPt, point);
	if(eModifyType != CToolEnumData::MODIFY_NONE)
		return eModifyType;

	return GetModifyTypeLine(region,drawingStartPt, drawingEndPt, point);
}

// private ====================================================================
CToolEnumData::ModifyType CEqualDivisionLine::GetModifyTypeLine(const CRect& region,const CPoint& drawingStartPt, 
																const CPoint& drawingEndPt, const CPoint& point) const
{
	const CEqualDivisionLineDrawer* pDrawer = GetEqualDivisionLineDrawer();
	if(pDrawer == NULL)
		return CToolEnumData::MODIFY_NONE;

	CPoint ptStart(drawingStartPt);
	CPoint ptEnd(drawingEndPt);
	BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();
	if(bLeftExt == TRUE)
	{
		ptStart.x = region.left;
	}

	if(bRightExt == TRUE)
	{
		ptEnd.x = region.right;
	}

	BOOL bEquiDivType = (BOOL)GetCommonEnvironment().GetEquiDiv();
	
	int nCount = pDrawer->GetCount(bEquiDivType);
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		int y = pDrawer->GetPoint_Y(ptStart.y, ptEnd.y, nIndex,bEquiDivType);
		CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_Line(CPoint(ptStart.x, y), CPoint(ptEnd.x, y), point);
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;
	}

	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CTrisectionLine

CTrisectionLine::CTrisectionLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CEqualDivisionLine(position, point1, point2, environment)
{
}

CToolEnumData::ElementType CTrisectionLine::GetElementType() const
{
	return CToolEnumData::Trisection;
}

// protected ==================================================================
CTwoPointDrawer& CTrisectionLine::GetTwoPointDrawer()
{
	return m_trisectionLineDrawer;
}

const CEqualDivisionLineDrawer* CTrisectionLine::GetEqualDivisionLineDrawer() const
{
	return &m_trisectionLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CQuadrisectLine

CQuadrisectLine::CQuadrisectLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CEqualDivisionLine(position, point1, point2, environment)
{
}

CToolEnumData::ElementType CQuadrisectLine::GetElementType() const
{
	return CToolEnumData::Quadrisect;
}

// protected ==================================================================
CTwoPointDrawer& CQuadrisectLine::GetTwoPointDrawer()
{
	return m_quadrisectLineDrawer;
}

const CEqualDivisionLineDrawer* CQuadrisectLine::GetEqualDivisionLineDrawer() const
{
	return &m_quadrisectLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CAndrewsPitchfork

CAndrewsPitchfork::CAndrewsPitchfork(const CCoordinate& point1, const CCoordinate& point2, const CCoordinate& point3, const CElementCommonEnvironment& environment):
	CThreePointElement(point1, point2, point3, environment)
{
}

CToolEnumData::ElementType CAndrewsPitchfork::GetElementType() const
{
	return CToolEnumData::AndrewsPitchfork;
}

// protected ==================================================================
CThreePointDrawer& CAndrewsPitchfork::GetThreePointDrawer()
{
	return m_andrewsDrawer;
}

bool CAndrewsPitchfork::ElementInRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const
{
	CPoint centerPt = pt1, sidePt1 = pt2, sidePt2 = pt3;
	m_andrewsDrawer.GetPoints(pt1, pt2, pt3, centerPt, sidePt1, sidePt2);

	return (centerPt.x < region.right && point.x >= (centerPt.x - 10));
}

CToolEnumData::ModifyType CAndrewsPitchfork::GetModifyType(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const
{
	if(PointInElementSelectableRegion(region, pt1, pt2, pt3, point)
		|| PointInElementSelectableRegion(region, pt2, pt1, pt3, point)
		|| PointInElementSelectableRegion(region, pt3, pt1, pt2, point))
		return CToolEnumData::MODIFY_CENTER;

	return CThreePointElement::GetModifyType(region, pt1, pt2, pt3, point);
}

// private ====================================================================
bool CAndrewsPitchfork::PointInElementSelectableRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const
{
	CPoint pt1_end = m_andrewsDrawer.GetLineEndPoint(region, pt1, pt2, pt3);
	if(pt1 == pt1_end)
		return false;

	return CElement::PointInElementSelectableRegion(pt1, pt1_end, point);
}

///////////////////////////////////////////////////////////////////////////////
// class CCycleLines

CCycleLines::CCycleLines(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

CToolEnumData::ElementType CCycleLines::GetElementType() const
{
	return CToolEnumData::Cycle_Line;
}

// protected ==================================================================
CTwoPointDrawer& CCycleLines::GetTwoPointDrawer()
{
	return m_cycleLinesDrawer;
}

bool CCycleLines::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	CRect pointRegion(startPoint, endPoint);
	pointRegion.NormalizeRect();

	if(pointRegion.Width() > region.Width())
		return false;

	pointRegion.InflateRect(MARGIN, MARGIN);
	return (pointRegion.left < point.x && pointRegion.right > point.x);
}

// 수정할 type 가져오기
CToolEnumData::ModifyType CCycleLines::GetModifyType(const CDisplayData& displayData, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	// element의 두점
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	return GetModifyType_Lines(displayData, drawingStartPt, drawingEndPt, point);
}

/*CToolEnumData::ModifyType CCycleLines::GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	// element의 두점
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	return GetModifyType_Lines(region, drawingStartPt, drawingEndPt, point);
}*/

// private ====================================================================
CToolEnumData::ModifyType CCycleLines::GetModifyType_Lines(const CDisplayData& displayData, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{	
	int nIndex1 = m_cycleLinesDrawer.GetPoints_Index1();
	int nIndex2 = m_cycleLinesDrawer.GetPoints_Index2();

	int nGap = abs(nIndex1 - nIndex2);
	if(nGap == 0)
	{
		return CToolEnumData::MODIFY_NONE;
	}

	CRect region(displayData.GetDisplayRegion());
	int nAccCount = 0;
	bool bResult1 = false;
	bool bResult2 = false;
	int drawPt_X1 = -1;
	int drawPt_X2 = -1;

	CPoint topPt;
	CPoint bottomPt;
	topPt.y = region.top;
	bottomPt.y = region.bottom;

	while(true)
	{
		bResult1 = m_cycleLinesDrawer.GetIndexInDisplayAttributes(displayData,nIndex1 + nAccCount);
		if(bResult1)
		{
			topPt.x = m_cycleLinesDrawer.GetDrawingPoint_X(displayData,nIndex1 + nAccCount);
			bottomPt.x = topPt.x;

			if(CElement::PointInElementSelectableRegion(topPt, bottomPt, point))
				return CToolEnumData::MODIFY_CENTER;
		}

		bResult2 = m_cycleLinesDrawer.GetIndexInDisplayAttributes(displayData,nIndex1 - nAccCount);
		if(bResult1)
		{
			topPt.x = m_cycleLinesDrawer.GetDrawingPoint_X(displayData,nIndex1 - nAccCount);
			bottomPt.x = topPt.x;

			if(CElement::PointInElementSelectableRegion(topPt, bottomPt, point))
				return CToolEnumData::MODIFY_CENTER;			
		}

		if(!bResult1 && !bResult2)
			break;		

		nAccCount += nGap;
	}

	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CSpeedLine

CSpeedLine::CSpeedLine(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

CToolEnumData::ElementType CSpeedLine::GetElementType() const
{
	return CToolEnumData::Speed_Line;
}

// protected ==================================================================
CTwoPointDrawer& CSpeedLine::GetTwoPointDrawer()
{
	return m_speedLineDrawer;
}

bool CSpeedLine::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	CPoint leftPt = startPoint;
	if(startPoint.x > endPoint.x)
		leftPt = endPoint;

	return (leftPt.x < region.right && point.x >= (leftPt.x - 10));
}

CToolEnumData::ModifyType CSpeedLine::GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	// element의 두점
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	CToolEnumData::ModifyType frameRectModifyType = GetModifyType_Rect( drawingStartPt, drawingEndPt, point);
	if( frameRectModifyType != CToolEnumData::MODIFY_NONE) 
		return frameRectModifyType;

	CPoint leftPt, rightPt;
	m_speedLineDrawer.GetPoints(drawingStartPt, drawingEndPt, leftPt, rightPt);
	
	int nModifyType = CToolEnumData::MODIFY_NONE;
	CRect rcGraphArea(region);
	rcGraphArea.left = min(drawingStartPt.x,drawingEndPt.x);
	for(int nIndex = 0; nIndex < m_speedLineDrawer.GetCount(); nIndex++)
	{
		CPoint lineEndPt;
		if(!m_speedLineDrawer.GetLineEndPoint(region, leftPt, rightPt, nIndex, lineEndPt))
			continue;

		if(nIndex > 0 && nIndex < 3)
		{
			if(CElement::PointInElementSelectableRegion(CPoint(leftPt.x,lineEndPt.y), lineEndPt, point))
				return CToolEnumData::MODIFY_CENTER;
		}
		else if(nIndex >= 3)
		{
			if(CElement::PointInElementSelectableRegion(CPoint(lineEndPt.x,leftPt.y), lineEndPt, point))
				return CToolEnumData::MODIFY_CENTER;
		} 

		if(nIndex < 3)
		{			
			// 2. Line의 추세방향을 기반으로 Cursor가 Line상(대각선)에 위치했는지 확인한다.
			CRect rctExtLineRect = CElement::GetLineRect( rcGraphArea, drawingStartPt, lineEndPt,FALSE,TRUE);
			if( 0 < ( lineEndPt.x - drawingStartPt.x) * ( lineEndPt.y - drawingStartPt.y))
				nModifyType = GetModifyType_Line( rctExtLineRect.TopLeft(), rctExtLineRect.BottomRight(), point);
			else		
				nModifyType = GetModifyType_Line(	CPoint( rctExtLineRect.left, rctExtLineRect.bottom),
													CPoint( rctExtLineRect.right, rctExtLineRect.top), point);

			if(nModifyType != CToolEnumData::MODIFY_NONE)
			{
				return (CToolEnumData::ModifyType)nModifyType;
			}
		}
	}
	
	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboFan

CSpeedFan::CSpeedFan(const CCoordinate& point1, const CCoordinate& point2, 
					   const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

CToolEnumData::ElementType CSpeedFan::GetElementType() const
{
	return CToolEnumData::Speed_Fan;
}

// protected ==================================================================
CTwoPointDrawer& CSpeedFan::GetTwoPointDrawer()
{
	return m_speedFanDrawer;
}

// ----------------------------------------------------------------------------
// 해당 영역에 Element가 있는지의 여부
bool CSpeedFan::ElementInRegion(const CRect& region, const CPoint& startPoint, 
								   const CPoint& endPoint, const CPoint& point) const
{
	if(CTwoPointElement::ElementInRegion(region, startPoint, endPoint, point))
		return true;

	if(startPoint.x > region.right)
		return false;

	return true;
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CSpeedFan::GetModifyType(const CRect& region, const CPoint& drawingStartPt, 
													  const CPoint& drawingEndPt, const CPoint& point) const
{
	// 1. 먼저 제어점의 위치를 확인토록 한다.
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints( drawingStartPt, drawingEndPt, point);
	if( sidePointModifyType != CToolEnumData::MODIFY_NONE) 
		return sidePointModifyType;

	CToolEnumData::ModifyType frameRectModifyType = GetModifyType_Rect( drawingStartPt, drawingEndPt, point);
	if( frameRectModifyType != CToolEnumData::MODIFY_NONE) 
		return frameRectModifyType;

	CPoint leftPt, rightPt;
	m_speedFanDrawer.GetPoints(drawingStartPt, drawingEndPt, leftPt, rightPt);

	for(int nIndex = 0; nIndex < m_speedFanDrawer.GetCount(); nIndex++)
	{
		CPoint lineEndPt;
		if(!m_speedFanDrawer.GetLineEndPoint(region, leftPt, rightPt, nIndex, lineEndPt))
			continue;

		if(nIndex > 0 && nIndex < 3)
		{
			if(CElement::PointInElementSelectableRegion(CPoint(leftPt.x,lineEndPt.y), lineEndPt, point))
				return CToolEnumData::MODIFY_CENTER;
		}
		else if(nIndex >= 3)
		{
			if(CElement::PointInElementSelectableRegion(CPoint(lineEndPt.x,leftPt.y), lineEndPt, point))
				return CToolEnumData::MODIFY_CENTER;
		} 

		if(nIndex < 3)
		{
			if(CElement::PointInElementSelectableRegion(leftPt, lineEndPt, point))
				return CToolEnumData::MODIFY_CENTER;
		}
	}
	
	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboArc

CSpeedArc::CSpeedArc(const CCoordinate& point1, const CCoordinate& point2, 
					   const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

CToolEnumData::ElementType CSpeedArc::GetElementType() const
{
	return CToolEnumData::Speed_Arc;
}

// protected ==================================================================
CTwoPointDrawer& CSpeedArc::GetTwoPointDrawer()
{
	return m_speedArcDrawer;
}

// ----------------------------------------------------------------------------
// 해당 영역에 Element가 있는지의 여부

bool CSpeedArc::ElementInRegion( const CRect& region, const CPoint& startPoint, 
								 const CPoint& endPoint, const CPoint& point) const
{
	int nRadius = GetRadius(startPoint, endPoint, 2);
	CPoint drawingSPt = GetDrawingRegion_StartPoint(startPoint, endPoint, nRadius);
	CPoint drawingEPt = GetDrawingRegion_EndPoint(startPoint, endPoint, nRadius);

	return CTwoPointElement::ElementInRegion(region, drawingSPt, drawingEPt, point);
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CSpeedArc::GetModifyType(const CRect& region, const CPoint& drawingStartPt, 
													  const CPoint& drawingEndPt, const CPoint& point) const
{
	// 1. 먼저 제어점의 위치를 확인토록 한다.
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints( drawingStartPt, drawingEndPt, point);
	if( sidePointModifyType != CToolEnumData::MODIFY_NONE) 
		return sidePointModifyType;

	CToolEnumData::ModifyType frameRectModifyType = GetModifyType_Rect( drawingStartPt, drawingEndPt, point);
	if( frameRectModifyType != CToolEnumData::MODIFY_NONE) 
		return frameRectModifyType;

	CPoint leftPt, rightPt;
	m_speedArcDrawer.GetPoints(drawingStartPt, drawingEndPt, leftPt, rightPt);
	
	int nRadius_Point = CMath::Round(CMath::Distance(drawingEndPt, point));
	if(nRadius_Point <= 0)
		return CToolEnumData::MODIFY_NONE;

	for(int nIndex = 0; nIndex < m_speedArcDrawer.GetCount(); nIndex++)
	{
		CPoint lineEndPt;
		if(!m_speedArcDrawer.GetLineEndPoint(region, leftPt, rightPt, nIndex, lineEndPt))
			continue;

		if(nIndex > 0 && nIndex < 3)
		{
			if(CElement::PointInElementSelectableRegion(CPoint(leftPt.x,lineEndPt.y), lineEndPt, point))
				return CToolEnumData::MODIFY_CENTER;
		}
		else if(nIndex >= 3)
		{
			if(CElement::PointInElementSelectableRegion(CPoint(lineEndPt.x,leftPt.y), lineEndPt, point))
				return CToolEnumData::MODIFY_CENTER;
		} 

		if(nIndex > 0 && nIndex < 3)
		{
			if(PointInCircleAround(nRadius_Point, GetRadius(drawingStartPt, drawingEndPt, nIndex)))
				return CToolEnumData::MODIFY_CENTER;
		}
	}
	
	return CToolEnumData::MODIFY_NONE;
}

CPoint CSpeedArc::GetDrawingRegion_EndPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const
{
	if(startPoint.x <= endPoint.x)
		return CPoint((endPoint.x + nRadius), endPoint.y);
	return CPoint((endPoint.x - nRadius), endPoint.y);
}

// private ====================================================================
bool CSpeedArc::PointInCircleAround(const int nSourceRadius, const int nTargetRadius) const
{
	if(nSourceRadius <= 0 || nTargetRadius <= 0)
		return false;

	CSelectableRegionCalculator selectableRegionCalculator;
	return (nSourceRadius >= (nTargetRadius - selectableRegionCalculator.GetProximityRange())
		&& nSourceRadius <= (nTargetRadius + selectableRegionCalculator.GetProximityRange()));
}

CPoint CSpeedArc::GetDrawingRegion_StartPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const
{
	int nX = GetDrawingRegion_StartPoint(startPoint.x, endPoint.x, nRadius);
	int nY = GetDrawingRegion_StartPoint(startPoint.y, endPoint.y, nRadius);
	return CPoint(nX, nY);
}

int CSpeedArc::GetDrawingRegion_StartPoint(const int nStartPoint, const int nEndPoint, const int nRadius) const
{
	int nDistanceOfTwoPoints = nEndPoint - nStartPoint;
	if(abs(nDistanceOfTwoPoints) >= nRadius)
		return nStartPoint;

	if(nDistanceOfTwoPoints >= 0) // startPoint <= endPoint
		return (nEndPoint - nRadius);

	return (nEndPoint + nRadius);
}

int CSpeedArc::GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const
{
	return m_speedArcDrawer.GetRadius(pt1, pt2, nIndex);
}

///////////////////////////////////////////////////////////////////////////////
// class CLinearRegression

CLinearRegression::CLinearRegression(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

void CLinearRegression::ElementModify(const CElementModifyData& elementModifyData)
{	
	//LinearRegression은 무조건 Y값을 수정해야 한다 이유 - 평균값이 변경되기 때문
	switch(elementModifyData.GetModifyType())
	{
	case CToolEnumData::MODIFY_START:		
		CTwoPointElement::SetStartPoint(elementModifyData.GetModifyPoint(CTwoPointElement::GetStartPoint()));
		break;
	case CToolEnumData::MODIFY_END:
		CTwoPointElement::SetEndPoint(elementModifyData.GetModifyPoint(CTwoPointElement::GetEndPoint()));		
		break;
	case CToolEnumData::MODIFY_CENTER:
		{
			// (2008/8/12 - Seung-Won, Bae) Do not move to outside as future.
			CCoordinate startPoint, endPoint;
			startPoint = elementModifyData.GetModifyPoint( CTwoPointElement::GetStartPoint());
			endPoint = elementModifyData.GetModifyPoint( CTwoPointElement::GetEndPoint());

			if( startPoint.GetX() == 0.0 || startPoint.GetIndex() < 0
				|| endPoint.GetX() == 0.0 || endPoint.GetIndex() < 0)
				return;

			CTwoPointElement::SetStartPoint( startPoint);
			CTwoPointElement::SetEndPoint( endPoint);		
			break;
		}
	default:
		assert(false);
	}
}

void CLinearRegression::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
	//LinearRegression은 무조건 Y값을 수정해야 한다 이유 - 평균값이 변경되기 때문
	switch(eModifyType)
	{
	case CToolEnumData::MODIFY_START:
	case CToolEnumData::MODIFY_END:
	case CToolEnumData::MODIFY_CENTER:
		CTwoPointElement::GetStartPoint().SetY(coordValue.GetX());
		CTwoPointElement::GetEndPoint().SetY(coordValue.GetY());
		break;
	default:
		assert(false);
	}
}

void CLinearRegression::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}

CToolEnumData::ElementType CLinearRegression::GetElementType() const
{
	return CToolEnumData::Linear_Regression;
}

CCoordinate CLinearRegression::GetCoordinate(const CToolEnumData::ModifyType eModifyType) const
{
	CCoordinate coordValue;
	if(eModifyType == CToolEnumData::MODIFY_START)
	{
		return GetStartPoint();
	}
	else if(eModifyType == CToolEnumData::MODIFY_END)
	{
		return GetEndPoint();
	}

	return coordValue;
}

// protected ==================================================================
CTwoPointDrawer& CLinearRegression::GetTwoPointDrawer()
{
	return m_linearRegressionDrawer;
}

/*bool CLinearRegression::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	CRect pointRegion(startPoint, endPoint);
	pointRegion.NormalizeRect();

	if(pointRegion.Width() > region.Width())
		return false;

	pointRegion.InflateRect(MARGIN, MARGIN);
	return (pointRegion.left < point.x && pointRegion.right > point.x);
}*/

CToolEnumData::ModifyType CLinearRegression::GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	// element의 두점
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	CPoint ptStart(drawingStartPt);
	CPoint ptEnd(drawingStartPt);
	ptStart.y = region.top;
	ptEnd.y = region.bottom;
	if(CElement::PointInElementSelectableRegion(ptStart, ptEnd, point))
		return CToolEnumData::MODIFY_CENTER;

	ptStart.x = drawingEndPt.x;
	ptEnd.x = drawingEndPt.x;
	if(CElement::PointInElementSelectableRegion(ptStart, ptEnd, point))
		return CToolEnumData::MODIFY_CENTER;

	BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();
			
	CRect rctExtLineRect = CElement::GetLineRect(region,drawingStartPt,drawingEndPt,bLeftExt,bRightExt);	
	return GetModifyType_Line(	CPoint( rctExtLineRect.left, rctExtLineRect.bottom),
								CPoint( rctExtLineRect.right, rctExtLineRect.top), point);
}

///////////////////////////////////////////////////////////////////////////////
// class CRaffRegression

CRaffRegression::CRaffRegression(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment,double dChannel):
	CTwoPointElement(point1, point2, environment)
{
	m_raffRegressionDrawer.SetChannel(dChannel);
}

void CRaffRegression::ElementModify(const CElementModifyData& elementModifyData)
{	
	//RaffRegression은 무조건 Y값을 수정해야 한다 이유 - 평균값이 변경되기 때문
	switch(elementModifyData.GetModifyType())
	{
	case CToolEnumData::MODIFY_START:		
		CTwoPointElement::SetStartPoint(elementModifyData.GetModifyPoint(CTwoPointElement::GetStartPoint()));
		break;
	case CToolEnumData::MODIFY_END:
		CTwoPointElement::SetEndPoint(elementModifyData.GetModifyPoint(CTwoPointElement::GetEndPoint()));		
		break;
	case CToolEnumData::MODIFY_CENTER:
		{
			// (2008/8/12 - Seung-Won, Bae) Do not move to outside as future.
			CCoordinate startPoint, endPoint;
			startPoint = elementModifyData.GetModifyPoint( CTwoPointElement::GetStartPoint());
			endPoint = elementModifyData.GetModifyPoint( CTwoPointElement::GetEndPoint());

			if( startPoint.GetX() == 0.0 || startPoint.GetIndex() < 0
				|| endPoint.GetX() == 0.0 || endPoint.GetIndex() < 0)
				return;

			CTwoPointElement::SetStartPoint( startPoint);
			CTwoPointElement::SetEndPoint( endPoint);		
			break;
		}
	default:
		assert(false);
	}
}

void CRaffRegression::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
	//LinearRegression은 무조건 Y값을 수정해야 한다 이유 - 평균값이 변경되기 때문
	switch(eModifyType)
	{
	case CToolEnumData::MODIFY_START:
	case CToolEnumData::MODIFY_END:
	case CToolEnumData::MODIFY_CENTER:
		CTwoPointElement::GetStartPoint().SetY(coordValue.GetX());
		CTwoPointElement::GetEndPoint().SetY(coordValue.GetY());
		break;
	default:
		assert(false);
	}
}

void CRaffRegression::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}

CToolEnumData::ElementType CRaffRegression::GetElementType() const
{
	return CToolEnumData::Raff_Regression;
}

CCoordinate CRaffRegression::GetCoordinate(const CToolEnumData::ModifyType eModifyType) const
{
	CCoordinate coordValue;
	if(eModifyType == CToolEnumData::MODIFY_START)
		return GetStartPoint();
	else if(eModifyType == CToolEnumData::MODIFY_END)
		return GetEndPoint();

	return coordValue;
}

// protected ==================================================================
CTwoPointDrawer& CRaffRegression::GetTwoPointDrawer()
{
	return m_raffRegressionDrawer;
}

/*bool CRaffRegression::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	CRect pointRegion(startPoint, endPoint);
	pointRegion.NormalizeRect();

	if(pointRegion.Width() > region.Width())
		return false;

	pointRegion.InflateRect(MARGIN, MARGIN);
	return (pointRegion.left < point.x && pointRegion.right > point.x);
}*/

CToolEnumData::ModifyType CRaffRegression::GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	// element의 두점
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	CPoint ptStart(drawingStartPt);
	CPoint ptEnd(drawingStartPt);
	ptStart.y = region.top;
	ptEnd.y = region.bottom;
	if(CElement::PointInElementSelectableRegion(ptStart, ptEnd, point))
		return CToolEnumData::MODIFY_CENTER;

	ptStart.x = drawingEndPt.x;
	ptEnd.x = drawingEndPt.x;
	if(CElement::PointInElementSelectableRegion(ptStart, ptEnd, point))
		return CToolEnumData::MODIFY_CENTER;

	BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();
			
	CRect rctExtLineRect = CElement::GetLineRect(region,drawingStartPt,drawingEndPt,bLeftExt,bRightExt);	
	if(CElement::PointInElementSelectableRegion(CPoint(rctExtLineRect.left,rctExtLineRect.top),
												CPoint(rctExtLineRect.right,rctExtLineRect.bottom), point))
		return CToolEnumData::MODIFY_CENTER;
	//modifyType = GetModifyType_Line(CPoint( rctExtLineRect.left, rctExtLineRect.bottom),
	//								CPoint( rctExtLineRect.right, rctExtLineRect.top), point);	
	//if(modifyType != CToolEnumData::MODIFY_NONE)
	//	return modifyType;

	if(m_raffRegressionDrawer.GetChannel() != 0.0)
	{
		int nChannel = m_raffRegressionDrawer.GetChannel()*drawingStartPt.y/GetStartPoint().GetY();
		ptStart = drawingStartPt;
		ptEnd = drawingEndPt;
		rctExtLineRect = CElement::GetLineRect(region,CPoint(ptStart.x,ptStart.y+nChannel),CPoint(ptEnd.x,ptEnd.y+nChannel),bLeftExt,bRightExt);	
		if(CElement::PointInElementSelectableRegion(CPoint(rctExtLineRect.left,rctExtLineRect.top),
													CPoint(rctExtLineRect.right,rctExtLineRect.bottom), point))
			return CToolEnumData::MODIFY_CENTER;
		//modifyType = GetModifyType_Line(CPoint( rctExtLineRect.left, rctExtLineRect.bottom),
		//								CPoint( rctExtLineRect.right, rctExtLineRect.top), point);
		//if(modifyType != CToolEnumData::MODIFY_NONE)
		//	return modifyType;

		rctExtLineRect = CElement::GetLineRect(region,CPoint(ptStart.x,ptStart.y-nChannel),CPoint(ptEnd.x,ptEnd.y-nChannel),bLeftExt,bRightExt);
		if(CElement::PointInElementSelectableRegion(CPoint(rctExtLineRect.left,rctExtLineRect.top),
													CPoint(rctExtLineRect.right,rctExtLineRect.bottom), point))
			return CToolEnumData::MODIFY_CENTER;
		//modifyType = GetModifyType_Line(CPoint( rctExtLineRect.left, rctExtLineRect.bottom),
		//								CPoint( rctExtLineRect.right, rctExtLineRect.top), point);
		//if(modifyType != CToolEnumData::MODIFY_NONE)
		//	return modifyType;
	}	

	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CAngleLine
CAngleLine::CAngleLine(const CTextShowPosition& position,const CCoordinate& point1,const CCoordinate& point2,const CElementCommonEnvironment& environment):
			 CTwoPointElement(point1, point2, environment),
			 m_textPosition(position)
{
	
}

CToolEnumData::ElementType CAngleLine::GetElementType() const
{
	return CToolEnumData::Angle_Line;
}

// protected ==================================================================
const CTextPosition* CAngleLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CAngleLine::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CAngleLine::GetTwoPointDrawer()
{
	return m_angleLineDrawer;
}

bool CAngleLine::ElementInRegion(const CRect &p_rctGraphArea,const CPoint &p_ptStart,const CPoint &p_ptEnd, const CPoint &p_ptCursor) const
{
	/*BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();

	long lLength = (long)sqrt(pow(p_ptStart.x - p_ptEnd.x,2) + pow(p_ptStart.y - p_ptEnd.y,2));	
	CPoint ptTempEnd(p_ptStart.x + lLength, p_ptEnd.y);	

	CRect rctLineRect = CElement::GetLineRect( p_rctGraphArea, p_ptStart, ptTempEnd,FALSE,bLeftExt);
	rctLineRect.InflateRect(1,1,1,1);
	if( rctLineRect.Width()		< 4) rctLineRect.InflateRect( CSize( ( 4 - rctLineRect.Width()) / 2,	0));
	if( rctLineRect.Height()	< 4) rctLineRect.InflateRect( CSize( 0,( 4 - rctLineRect.Height()) / 2));
	return rctLineRect.PtInRect( p_ptCursor);*/

	return true;
}

CToolEnumData::ModifyType CAngleLine::GetModifyType(const CRect &p_rctGraphArea,const CPoint &p_ptStart,const CPoint &p_ptEnd, const CPoint &p_ptCursor) const
{
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints( p_ptStart, p_ptEnd, p_ptCursor);
	if( sidePointModifyType != CToolEnumData::MODIFY_NONE) 
		return sidePointModifyType;

	BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();
	
	CToolEnumData::ModifyType modifyType;		
	CRect rctExtLineRect = CElement::GetLineRect(p_rctGraphArea,p_ptStart,p_ptEnd,FALSE,bLeftExt);
	if( 0 < ( p_ptEnd.x - p_ptStart.x) * ( p_ptEnd.y - p_ptStart.y))
	{
		modifyType = GetModifyType_Line( rctExtLineRect.TopLeft(), rctExtLineRect.BottomRight(), p_ptCursor);
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;
	}
	
	modifyType = GetModifyType_Line(CPoint( rctExtLineRect.left, rctExtLineRect.bottom),
									CPoint( rctExtLineRect.right, rctExtLineRect.top), p_ptCursor);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;
	
	CPoint ptStart(p_ptStart);	
	CPoint ptEnd(p_ptEnd);	
	long lLength = (long)sqrt(pow(ptStart.x - ptEnd.x,2) + pow(ptStart.y - ptEnd.y,2));	
	CPoint ptTempEnd(ptStart.x + lLength, ptStart.y);	
	if(bLeftExt == TRUE)
	{
		ptTempEnd.x = p_rctGraphArea.right;
	}
	if(CElement::PointInElementSelectableRegion(ptStart, ptTempEnd, p_ptCursor))
		return CToolEnumData::MODIFY_CENTER;
	
	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CCandleLine

CCandleLine::CCandleLine(const CTextShowPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment):
	COnePointElement(point, environment),
	m_textPosition(position)
{
}

// public =====================================================================
CToolEnumData::ElementType CCandleLine::GetElementType() const
{
	return CToolEnumData::Candle_Line;
}

// 해당 modifyType에 맞게 변경
void CCandleLine::ElementModify(const CElementModifyData& elementModifyData)
{
	assert(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_START || 
			elementModifyData.GetModifyType() == CToolEnumData::MODIFY_CENTER);

	switch(elementModifyData.GetModifyType())
	{
	case CToolEnumData::MODIFY_START:		
		COnePointElement::SetPoint(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
		break;
	case CToolEnumData::MODIFY_CENTER:
		COnePointElement::SetPoint(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
		break;
	default:
		assert(false);
	}	
}

void CCandleLine::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
	switch(eModifyType)
	{
	case CToolEnumData::MODIFY_START:		
	case CToolEnumData::MODIFY_CENTER:
		COnePointElement::GetPoint().SetX(coordValue.GetX(),0);
		COnePointElement::GetPoint().SetY(coordValue.GetY());
		break;
	default:
		assert(false);
	}	
}

void CCandleLine::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}
// protected ==================================================================
const CTextPosition* CCandleLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CCandleLine::GetTextPosition()
{
	return &m_textPosition;
}

COnePointDrawer& CCandleLine::GetOnePointDrawer()
{
	return m_candleLineDrawer;
}

// 수정할 type 가져오기
CToolEnumData::ModifyType CCandleLine::GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const
{
	CToolEnumData::ModifyType pointModifyType = GetModifyType_Point( elementDrawingPt, point);
	if( pointModifyType != CToolEnumData::MODIFY_NONE) 
		return pointModifyType;

	CPoint ptEnd(elementDrawingPt);
	ptEnd.x = displayData.GetDisplayRegion().right;
	if(CElement::PointInElementSelectableRegion(elementDrawingPt,ptEnd, point))
		return CToolEnumData::MODIFY_CENTER;

	return CToolEnumData::MODIFY_NONE;
}

// private ====================================================================
// 선택할수 있는 범위
bool CCandleLine::PointInElementSelectableRegion(const CRect& region, const CPoint& regionPt, const CPoint& point) const
{
	return CElement::PointInElementSelectableRegion(CPoint(region.left, regionPt.y), CPoint(region.right, regionPt.y), point);
}

///////////////////////////////////////////////////////////////////////////////
// class CCandleBong

CCandleBong::CCandleBong(const CCoordinate& point1, const CCoordinate& point2, 
					   const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

CToolEnumData::ElementType CCandleBong::GetElementType() const
{
	return CToolEnumData::Candle_Bong;
}

// protected ==================================================================
CTwoPointDrawer& CCandleBong::GetTwoPointDrawer()
{
	return m_candleBongDrawer;
}

// ----------------------------------------------------------------------------
// 해당 영역에 Element가 있는지의 여부
bool CCandleBong::ElementInRegion(const CRect& region, const CPoint& startPoint, 
								   const CPoint& endPoint, const CPoint& point) const
{
	if(CTwoPointElement::ElementInRegion(region, startPoint, endPoint, point))
		return true;

	if(startPoint.x > region.right)
		return false;

	return true;
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CCandleBong::GetModifyType(const CRect& region, const CPoint& drawingStartPt, 
													  const CPoint& drawingEndPt, const CPoint& point) const
{
	// 1. 먼저 제어점의 위치를 확인토록 한다.
	CPoint ptDrawingEnd(drawingStartPt);
	ptDrawingEnd.y = drawingEndPt.y;
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints( drawingStartPt, ptDrawingEnd, point);
	if( sidePointModifyType != CToolEnumData::MODIFY_NONE) 
		return sidePointModifyType;

	CRect rcBong;
	rcBong.left = min(drawingStartPt.x,drawingEndPt.x);
	rcBong.right = max(drawingStartPt.x,drawingEndPt.x);
	rcBong.top = min(drawingStartPt.y,drawingEndPt.y);
	rcBong.bottom = max(drawingStartPt.y,drawingEndPt.y);
	if(rcBong.PtInRect(point))
		return CToolEnumData::MODIFY_CENTER;
	
	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CPencilLine

CPencilLine::CPencilLine(const int& nPointsCount,const CPoints* pPointsList,const CElementCommonEnvironment& environment):
	CMultiPointElement(nPointsCount,pPointsList,environment)
{	
}

CPencilLine::CPencilLine(const CDequeList<CCoordinate>& pointList,const CElementCommonEnvironment& environment):
	CMultiPointElement(pointList,environment)
{
}

CToolEnumData::ElementType CPencilLine::GetElementType() const
{
	return CToolEnumData::Pencil_Line;
}

// protected ==================================================================
CMultiPointDrawer& CPencilLine::GetMultiPointDrawer()
{
	return m_pencilLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CPeriodSumLine
CPeriodSumLine::CPeriodSumLine(const CTextHorzPosition& position,const CCoordinate& point1,const CCoordinate& point2,const CElementCommonEnvironment& environment):
			 CTwoPointElement(point1, point2, environment),
			 m_textPosition(position)
{
	
}

CToolEnumData::ElementType CPeriodSumLine::GetElementType() const
{
	return CToolEnumData::Period_Summary;
}

// protected ==================================================================
const CTextPosition* CPeriodSumLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CPeriodSumLine::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CPeriodSumLine::GetTwoPointDrawer()
{
	return m_periodSumLineDrawer;
}

bool CPeriodSumLine::ElementInRegion(const CRect &p_rctGraphArea,const CPoint &p_ptStart,const CPoint &p_ptEnd, const CPoint &p_ptCursor) const
{
	CRect rctLineRect = CElement::GetLineRect( p_rctGraphArea, p_ptStart, p_ptEnd,FALSE,FALSE);
	rctLineRect.InflateRect(1,1,1,1);
	if( rctLineRect.Width()		< 4) rctLineRect.InflateRect( CSize( ( 4 - rctLineRect.Width()) / 2,	0));
	if( rctLineRect.Height()	< 4) rctLineRect.InflateRect( CSize( 0,( 4 - rctLineRect.Height()) / 2));
	return ( rctLineRect.PtInRect( p_ptCursor) ? true : false);
}

CToolEnumData::ModifyType CPeriodSumLine::GetModifyType(const CRect &p_rctGraphArea,const CPoint &p_ptStart,const CPoint &p_ptEnd, const CPoint &p_ptCursor) const
{
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints( p_ptStart, p_ptEnd, p_ptCursor);
	if( sidePointModifyType != CToolEnumData::MODIFY_NONE) 
		return sidePointModifyType;

	CPoint ptStart(p_ptStart);	
	CPoint ptEnd(p_ptEnd);
	
	ptEnd.y = p_ptStart.y;	
	if(CElement::PointInElementSelectableRegion(ptStart, ptEnd, p_ptCursor))
		return CToolEnumData::MODIFY_CENTER;

	ptEnd.y = p_ptEnd.y;	
	ptStart.x = p_ptEnd.x;
	if(CElement::PointInElementSelectableRegion(ptStart, ptEnd, p_ptCursor))
		return CToolEnumData::MODIFY_CENTER;
	
	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CElliotLine

CElliotLine::CElliotLine(const CCoordinate& point1, const CCoordinate& point2, 
						 const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

CToolEnumData::ElementType CElliotLine::GetElementType() const
{
	return CToolEnumData::ElliotWave_Line;
}

// protected ==================================================================
CTwoPointDrawer& CElliotLine::GetTwoPointDrawer()
{
	return m_elliotLineDrawer;
}

// ----------------------------------------------------------------------------
// 해당 영역에 Element가 있는지의 여부
bool CElliotLine::ElementInRegion(const CRect& region, const CPoint& startPoint, 
								   const CPoint& endPoint, const CPoint& point) const
{
	/*if(CTwoPointElement::ElementInRegion(region, startPoint, endPoint, point))
		return true;

	if((startPoint.x < endPoint.x && point.x < startPoint.x) ||
		(startPoint.x > endPoint.x && point.x > startPoint.x))
		return false;*/

	return true;
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CElliotLine::GetModifyType(const CRect& region, const CPoint& drawingStartPt, 
													  const CPoint& drawingEndPt, const CPoint& point) const
{
	// 1. 먼저 제어점의 위치를 확인토록 한다.
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints( drawingStartPt, drawingEndPt, point);
	if( sidePointModifyType != CToolEnumData::MODIFY_NONE) 
		return sidePointModifyType;

	if(CElement::PointInElementSelectableRegion(drawingStartPt,drawingEndPt, point))
		return CToolEnumData::MODIFY_CENTER;

	CPoint elliotFromPoint(drawingEndPt);
	CPoint elliotToPoint(0,0);

	if(drawingStartPt.x > drawingEndPt.x)
	{
		elliotFromPoint.x = drawingStartPt.x;
		elliotFromPoint.y = drawingStartPt.y;
	}

	for(int nIndex=1;nIndex<8;nIndex++)
	{
		if(!m_elliotLineDrawer.GetPoint(region, drawingStartPt, drawingEndPt,nIndex,elliotFromPoint,elliotToPoint))
			break;

		if(CElement::PointInElementSelectableRegion(elliotFromPoint,elliotToPoint, point))
			return CToolEnumData::MODIFY_CENTER;

		elliotFromPoint = elliotToPoint;
	}
	
	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CAdjustRatio

CAdjustRatio::CAdjustRatio(const CCoordinate& point1, const CCoordinate& point2, const CCoordinate& point3, const CElementCommonEnvironment& environment):
	CThreePointElement(point1, point2, point3, environment)
{
}

CToolEnumData::ElementType CAdjustRatio::GetElementType() const
{
	return CToolEnumData::Adjust_Ratio;
}

// protected ==================================================================
CThreePointDrawer& CAdjustRatio::GetThreePointDrawer()
{
	return m_adjustRatioDrawer;
}

bool CAdjustRatio::ElementInRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const
{
	CPoint centerPt = pt1, sidePt1 = pt2, sidePt2 = pt3;
	//m_adjustRatioDrawer.GetPoints(pt1, pt2, pt3, centerPt, sidePt1, sidePt2);

	return (centerPt.x < region.right && point.x >= (centerPt.x - 10));
}

CToolEnumData::ModifyType CAdjustRatio::GetModifyType(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const
{
	if(PointInElementSelectableRegion(region, pt1, pt2, pt3, point)
		|| PointInElementSelectableRegion(region, pt2, pt1, pt3, point)
		|| PointInElementSelectableRegion(region, pt3, pt1, pt2, point))
		return CToolEnumData::MODIFY_CENTER;

	return CThreePointElement::GetModifyType(region, pt1, pt2, pt3, point);
}

// private ====================================================================
bool CAdjustRatio::PointInElementSelectableRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const
{
	CPoint pt1_end(0,0);// = m_adjustRatioDrawer.GetLineEndPoint(region, pt1, pt2, pt3);
	if(pt1 == pt1_end)
		return false;

	return CElement::PointInElementSelectableRegion(pt1, pt1_end, point);
}
