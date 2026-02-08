// FiboGann.cpp: implementation of the CFiboGann class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiboGann.h"

#include "CoordinateCalculator.h"
#include "DataMath.h"
#include "DrawingData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CFiboArc

CFiboArc::CFiboArc(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

// protected ==================================================================
// 해당 영역에 Element가 있는지의 여부
bool CFiboArc::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	int nRadius = GetRadius(startPoint, endPoint, 2);
	CPoint drawingSPt = GetDrawingRegion_StartPoint(startPoint, endPoint, nRadius);
	CPoint drawingEPt = GetDrawingRegion_EndPoint(startPoint, endPoint, nRadius);

	return CTwoPointElement::ElementInRegion(region, drawingSPt, drawingEPt, point);
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CFiboArc::GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType(region, drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	int nRadius_Point = CMath::Round(CMath::Distance(drawingEndPt, point));
	if(nRadius_Point <= 0)
		return CToolEnumData::MODIFY_NONE;

	for(int nIndex = 0; nIndex < 4; nIndex++){
		if(PointInCircleAround(nRadius_Point, GetRadius(drawingStartPt, drawingEndPt, nIndex)))
			return CToolEnumData::MODIFY_CENTER;
	}

	return CToolEnumData::MODIFY_NONE;
}

// ----------------------------------------------------------------------------
CPoint CFiboArc::GetDrawingRegion_EndPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const
{
	if(startPoint.x <= endPoint.x)
		return CPoint((endPoint.x + nRadius), endPoint.y);
	return CPoint((endPoint.x - nRadius), endPoint.y);
}

int CFiboArc::GetPoint_ApplyRadius(const int nOrgData, const int nRadius, const bool bPlusVector) const
{
	if(bPlusVector)
		return (nOrgData + nRadius);
	return (nOrgData - nRadius);
}

// private ====================================================================
// point가 원의 둘레에 속한 지의 유무.
bool CFiboArc::PointInCircleAround(const int nSourceRadius, const int nTargetRadius) const
{
	if(nSourceRadius <= 0 || nTargetRadius <= 0)
		return false;

	CSelectableRegionCalculator selectableRegionCalculator;
	return (nSourceRadius >= (nTargetRadius - selectableRegionCalculator.GetProximityRange())
		&& nSourceRadius <= (nTargetRadius + selectableRegionCalculator.GetProximityRange()));
}

CPoint CFiboArc::GetDrawingRegion_StartPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const
{
	int nX = GetDrawingRegion_StartPoint(startPoint.x, endPoint.x, nRadius);
	int nY = GetDrawingRegion_StartPoint(startPoint.y, endPoint.y, nRadius);
	return CPoint(nX, nY);
}

int CFiboArc::GetDrawingRegion_StartPoint(const int nStartPoint, const int nEndPoint, const int nRadius) const
{
	int nDistanceOfTwoPoints = nEndPoint - nStartPoint;
	if(abs(nDistanceOfTwoPoints) >= nRadius)
		return nStartPoint;

	if(nDistanceOfTwoPoints >= 0) // startPoint <= endPoint
		return (nEndPoint - nRadius);

	return (nEndPoint + nRadius);
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboSemicircle

CFiboSemicircle::CFiboSemicircle(const CTextShowPosition& position,const CCoordinate& point1, const CCoordinate& point2, 
								 const CElementCommonEnvironment& environment):
	CFiboArc(point1, point2, environment),
	m_textPosition(position)
{
}

CToolEnumData::ElementType CFiboSemicircle::GetElementType() const
{
	return CToolEnumData::Fibo_Semicircle;
}

// protected ==================================================================
const CTextPosition* CFiboSemicircle::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboSemicircle::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CFiboSemicircle::GetTwoPointDrawer()
{
	return m_fiboSemicircleDrawer;
}

// ----------------------------------------------------------------------------
int CFiboSemicircle::GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const
{
	return m_fiboSemicircleDrawer.GetRadius(pt1, pt2, nIndex);
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboCircle

CFiboCircle::CFiboCircle(const CTextShowPosition& position,const CCoordinate& point1, const CCoordinate& point2, 
						 const CElementCommonEnvironment& environment):
	CFiboArc(point1, point2, environment),
	m_textPosition(position)
{
}

CToolEnumData::ElementType CFiboCircle::GetElementType() const
{
	return CToolEnumData::Fibo_Circle;
}

// protected ==================================================================
const CTextPosition* CFiboCircle::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboCircle::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CFiboCircle::GetTwoPointDrawer()
{
	return m_fiboCircleDrawer;
}

// ----------------------------------------------------------------------------
int CFiboCircle::GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const
{
	return m_fiboCircleDrawer.GetRadius(pt1, pt2, nIndex);
}

CPoint CFiboCircle::GetDrawingRegion_EndPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const
{
	CPoint drawingEPt = CFiboArc::GetDrawingRegion_EndPoint(startPoint, endPoint, nRadius);

	if(startPoint.y <= endPoint.y)
		drawingEPt.y += nRadius;
	else
		drawingEPt.y -= nRadius;

	return drawingEPt;
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboPenLine

CFiboPenLine::CFiboPenLine(const CTextShowPosition& position,const CCoordinate& point1, const CCoordinate& point2, 
						   const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment),
	m_textPosition(position)
{
}

CToolEnumData::ElementType CFiboPenLine::GetElementType() const
{
	return CToolEnumData::Fibo_Fan_Line;
}

// protected ==================================================================
const CTextPosition* CFiboPenLine::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboPenLine::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CFiboPenLine::GetTwoPointDrawer()
{
	return m_fiboPenLineDrawer;
}

// ----------------------------------------------------------------------------
// 해당 영역에 Element가 있는지의 여부
bool CFiboPenLine::ElementInRegion(const CRect& region, const CPoint& startPoint, 
								   const CPoint& endPoint, const CPoint& point) const
{
	if(CTwoPointElement::ElementInRegion(region, startPoint, endPoint, point))
		return true;

	if(startPoint.x > region.right)
		return false;

	return true;
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CFiboPenLine::GetModifyType(const CRect& region, const CPoint& drawingStartPt, 
													  const CPoint& drawingEndPt, const CPoint& point) const
{
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType(region, drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	for(int nIndex = 0; nIndex < 3; nIndex++){
		CPoint fiboPoint(0, 0);
		if(!m_fiboPenLineDrawer.GetPoint(region, drawingStartPt, drawingEndPt, nIndex, fiboPoint))
			break;

		modifyType = GetModifyType(drawingStartPt, drawingEndPt, fiboPoint, point);
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;
	}

	return CToolEnumData::MODIFY_NONE;
}

// private ====================================================================
CToolEnumData::ModifyType CFiboPenLine::GetModifyType(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& fiboPoint, const CPoint& point) const
{
	if(drawingStartPt.x <= drawingEndPt.x)
		return CTwoPointElement::GetModifyType_Line(drawingStartPt, fiboPoint, point);

	return CTwoPointElement::GetModifyType_Line(drawingEndPt, fiboPoint, point);
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboRetrace

CFiboRetrace::CFiboRetrace(const CTextShowPosition& position, const CCoordinate& point1, 
						   const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment),
	m_textPosition(position)
{
}

CToolEnumData::ElementType CFiboRetrace::GetElementType() const
{
	return CToolEnumData::Fibo_Retrace;
}

// protected ==================================================================
const CTextPosition* CFiboRetrace::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboRetrace::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CFiboRetrace::GetTwoPointDrawer()
{
	return m_fiboRetraceDrawer;
}

// ----------------------------------------------------------------------------
// 해당 영역에 Element가 있는지의 여부
bool CFiboRetrace::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	if(CTwoPointElement::ElementInRegion(region, startPoint, endPoint, point))
		return true;

	return (startPoint.x <= region.right);
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CFiboRetrace::GetModifyType(const CRect& region, const CPoint& drawingStartPt, 
													  const CPoint& drawingEndPt, const CPoint& point) const
{
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType(region, drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	CPoint ptStart(drawingStartPt);
	CPoint ptEnd(drawingEndPt);
	BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();
	if(bLeftExt == TRUE || bRightExt == TRUE)
	{
		if(bLeftExt == TRUE && bRightExt == TRUE)
		{
			ptStart.x = region.left;
			ptEnd.x = region.right;
		}
		else if(bLeftExt == TRUE && bRightExt == FALSE)
		{
			ptStart.x = region.left;			
		}
		else if(bLeftExt == FALSE && bRightExt == TRUE)
		{
			ptEnd.x = region.right;
		}
	}

	for(int nIndex = 0; nIndex < m_fiboRetraceDrawer.GetCount(); nIndex++)
	{
		CPoint fiboPoint(0, 0);
		if(!m_fiboRetraceDrawer.GetPoint(region, ptStart, ptEnd, nIndex, fiboPoint))
			break;
		
		CToolEnumData::ModifyType modifyType = GetModifyType(ptStart.x, ptEnd.x, fiboPoint, point);
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;
	}

	return CToolEnumData::MODIFY_NONE;
}

// private ====================================================================
CToolEnumData::ModifyType CFiboRetrace::GetModifyType(const int nX1, const int nX2, 
													  const CPoint& fiboPoint, const CPoint& point) const
{
	return CTwoPointElement::GetModifyType_Line(CPoint((nX1 < nX2 ? nX1: nX2), fiboPoint.y), fiboPoint, point);
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboTime

CFiboTime::CFiboTime(const CCoordinate& point, const CElementCommonEnvironment& environment):
	CVertElement(point, environment)
{
	m_textPosition.SetShow(true);
}

CFiboTime::CFiboTime(const CTextShowPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment):
	CVertElement(point, environment),
	m_textPosition(position)		// 2008.08.20 김진순 피보나치 확장
{
}

CToolEnumData::ElementType CFiboTime::GetElementType() const
{
	return CToolEnumData::Fibo_Time;
}

// protected ==================================================================
// 2008.08.20 김진순 피보나치 확장<<
const CTextPosition* CFiboTime::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboTime::GetTextPosition()
{
	return &m_textPosition;
}
//>>

COnePointDrawer& CFiboTime::GetOnePointDrawer()
{
	return m_fiboTimeDrawer;
}

CToolEnumData::ModifyType CFiboTime::GetModifyType(const CDisplayData& displayData, 
												   const CPoint& elementDrawingPt, const CPoint& point) const
{
	int nIndex = -1;
	CPoint fiboPoint = elementDrawingPt;

	while(fiboPoint.x <= displayData.GetDisplayRegion().right){
		if(!m_fiboTimeDrawer.GetPoint(displayData, elementDrawingPt, ++nIndex, fiboPoint))
			break;

		if(nIndex > 5 && elementDrawingPt.x == fiboPoint.x)
			break;

		if(CVertElement::GetModifyType(displayData, fiboPoint, point) == CToolEnumData::MODIFY_X)
			return CToolEnumData::MODIFY_X;
	}

	return CToolEnumData::MODIFY_NONE;
}

// 실제 그릴좌표. (중심점 or 시작점)
CPoint CFiboTime::GetGripPoint(const CCoordinateCalculatorData& calData)
{
	// 시작점
	CPoint point = COnePointElement::GetGripPoint(calData);
	CPoint fiboPoint(point);
	m_fiboTimeDrawer.GetPoint(GetDisplayData(calData), point, 0, fiboPoint);

	return CPoint(fiboPoint.x, point.y);
}

void CFiboTime::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}
// private ====================================================================
CDisplayData CFiboTime::GetDisplayData(const CCoordinateCalculatorData& calData) const
{
	return CDisplayData(CElement::GetCommonEnvironment().GetDataType().GetCoordinateType(), 
		CGraphPartRegion(calData.GetRegion(), calData.GetRegion()), 
		CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()));
}

///////////////////////////////////////////////////////////////////////////////
// class CGann

CGann::CGann(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

// protected ==================================================================
CTwoPointDrawer& CGann::GetTwoPointDrawer()
{
	return (CTwoPointDrawer&) GetGannDrawer();
}

// ----------------------------------------------------------------------------
// 해당 영역에 Element가 있는지의 여부
bool CGann::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& /*point*/) const
{
	// 오른쪽 영역
	if(IsInAscendingPowers(region.right, startPoint.x, endPoint.x))
		return false;
	// 왼쪽 영역
	if(IsInAscendingPowers(endPoint.x, startPoint.x, region.left))
		return false;
	// 위쪽 영역
	if(IsInAscendingPowers(endPoint.y, startPoint.y, region.top))
		return false;
	// 아래쪽 영역
	if(IsInAscendingPowers(region.bottom, startPoint.y, endPoint.y))
		return false;

	return true;
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CGann::GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	// element의 두점
	CToolEnumData::ModifyType modifyType = GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	// line
	return GetModifyType_Lines(region, drawingStartPt, drawingEndPt, point);
}

// private ====================================================================
// 오름 차순 여부 --> nData1 <= nData2 <= nData3
bool CGann::IsInAscendingPowers(const int nData1, const int nData2, const int nData3) const
{
	return (nData1 <= nData2 && nData2 <= nData3);
}

// modify type 구하기.  -> element의 두점
CToolEnumData::ModifyType CGann::GetModifyType_SidePoints(const CPoint& pt1, const CPoint& pt2, const CPoint& point) const
{
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_SidePoints(pt1, pt2, point);

	if(modifyType == CToolEnumData::MODIFY_START)
		return CToolEnumData::MODIFY_CENTER;
	return modifyType;
}

///////////////////////////////////////////////////////////////////////////////
// class CGannLine

CGannLine::CGannLine(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment,int nGannType):
	CGann(point1, point2, environment),
	m_nGannLineType(nGannType)
{
}

CToolEnumData::ElementType CGannLine::GetElementType() const
{
	if(m_nGannLineType == 0)
	{
		return CToolEnumData::Gann_Line1;
	}
	else
	{
		return CToolEnumData::Gann_Line2;
	}
}

// protected ==================================================================
CGannDrawer& CGannLine::GetGannDrawer()
{
	return m_gannLineDrawer;
}

CToolEnumData::ModifyType CGannLine::GetModifyType_Lines(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	CPoint realEndPoint;
	if(!m_gannLineDrawer.GetLineEndPoint(region, startPoint, endPoint, realEndPoint))
		return CToolEnumData::MODIFY_NONE;

	return CTwoPointElement::GetModifyType_Line(startPoint, realEndPoint, point);
}

///////////////////////////////////////////////////////////////////////////////
// class CGannFan

CGannFan::CGannFan(const CTextShowPosition& position,const CCoordinate& point1, 
				   const CCoordinate& point2, const CElementCommonEnvironment& environment,int nGannType):
	CGann(point1, point2, environment),
	m_nGannFanType(nGannType),
	m_textPosition(position)
{
}

CToolEnumData::ElementType CGannFan::GetElementType() const
{
	if(m_nGannFanType==0)
		return CToolEnumData::Gann_Fan1;
	else if(m_nGannFanType==1)
		return CToolEnumData::Gann_Fan2;
	else if(m_nGannFanType==2)
		return CToolEnumData::Gann_Fan3;
	else 
		return CToolEnumData::Gann_Fan4;
}

// protected ==================================================================
CGannDrawer& CGannFan::GetGannDrawer()
{
	return m_gannFanDrawer;
}

const CTextPosition* CGannFan::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CGannFan::GetTextPosition()
{
	return &m_textPosition;
}
// ----------------------------------------------------------------------------
// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CGannFan::GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	// line
	return GetModifyType_Lines(region, drawingStartPt, drawingEndPt, point);
}

// modify type 구하기. -> line
CToolEnumData::ModifyType CGannFan::GetModifyType_Lines(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	for(int nIndex = 0; nIndex < m_gannFanDrawer.GetCount(); nIndex++){
		CPoint realEndPoint;
		if(!m_gannFanDrawer.GetLineEndPoint(region, startPoint, endPoint, nIndex, realEndPoint))
			continue;

		CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_Line(startPoint, realEndPoint, point);
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;
	}
	
	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CGannGrid

CGannGrid::CGannGrid(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CTwoPointElement(point1, point2, environment)
{
}

CToolEnumData::ElementType CGannGrid::GetElementType() const
{
	return CToolEnumData::Gann_Grid;
}

// protected ==================================================================
CTwoPointDrawer& CGannGrid::GetTwoPointDrawer()
{
	return m_gannGridDrawer;
}

// ----------------------------------------------------------------------------
// 해당 영역에 Element가 있는지의 여부
bool CGannGrid::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& /*point*/) const
{
	if(ElementInRegion(region.top, region.bottom, startPoint.y))
		return true;

	return ElementInRegion(region.top, region.bottom, endPoint.y);
}

// 두점을 element 영역에 해당 되는지 체크
CToolEnumData::ModifyType CGannGrid::GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	// element의 두점
	CToolEnumData::ModifyType modifyType = CTwoPointElement::GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	// start point 방향
	modifyType = GetModifyType_SideLines(region, drawingStartPt, drawingEndPt, point);
	if(modifyType != CToolEnumData::MODIFY_NONE)
		return modifyType;

	// end point 방향
	return GetModifyType_SideLines(region, drawingEndPt, drawingStartPt, point);
}


// private ====================================================================
// 해당 영역에 선상에 data가 존재하는지를 본다.
bool CGannGrid::ElementInRegion(const int minRegion, const int maxRegion, const int data) const
{
	return (data >= minRegion && data <= maxRegion);
}

// ----------------------------------------------------------------------------
// drawingPt1 방향의 GannGrid line의 modifyType 구하기
CToolEnumData::ModifyType CGannGrid::GetModifyType_SideLines(const CRect& region, 
		const CPoint& drawingPt1, const CPoint& drawingPt2, const CPoint& point) const
{
	int nIndex = 0;
	while(true){
		CToolEnumData::ModifyType modifyType = CToolEnumData::MODIFY_NONE;
		if(!GetModifyType_Lines(region, drawingPt1, drawingPt2, nIndex, point, modifyType))
			break;

		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;
		nIndex++;	
	}

	return CToolEnumData::MODIFY_NONE;
}

// index번의 선에 대한 modify type 구하기.
// -> drawingStartPt와 drawingEndPt로 이루어진 선과 평행인 선 -1
//    1선이 drawingStartPt.x 에 대칭인 선 - 2
bool CGannGrid::GetModifyType_Lines(const CRect& region, const CPoint& linePt1, const CPoint& linePt2, 
		const int nIndex, const CPoint& point, CToolEnumData::ModifyType& modifyType) const
{
	CPoint startPt, endPt;
	if(!m_gannGridDrawer.GetLineEndPoints(region, linePt1, linePt2, nIndex, startPt, endPt))
		return false;

	// 1선
	modifyType = CTwoPointElement::GetModifyType_Line(startPt, endPt, point);
	// 2선
	if(modifyType == CToolEnumData::MODIFY_NONE)
		modifyType = CTwoPointElement::GetModifyType_Line(GetSymmetryPointOfXAxis(linePt1, startPt), GetSymmetryPointOfXAxis(linePt1, endPt), point);

	return true;
}

// x축에 대칭인 좌표
CPoint CGannGrid::GetSymmetryPointOfXAxis(const CPoint& orgPt, const CPoint& point) const
{
	return CPoint(point.x, (orgPt.y + (orgPt.y - point.y)));
}
