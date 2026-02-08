// CoordinateCalculator.cpp: implementation of the CCoordinateCalculator class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoordinateCalculator.h"

#include <assert.h>

#include "../Include_Analysis/CoordinateCalculatorData.h"

#include "Coordinate.h"
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
// class CCoordinateCalculator

// 절대 좌표(Absolute) -> 상대 좌표(Relative) : 저장을 위한 좌표
CCoordinate CCoordinateCalculator::AbsoluteToRelative(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	CCoordinate cdResult;
	IPacket *pIPacket = calData.GetPacket_X();
	if( !IsCalculator( pIPacket)) 
	{
		cdResult = CCoordinate(point);
	}
	else
	{
		// XScale : 관련 Interface로 사용 - vntsorl(20070327)
//		int index = CDataConversion::GetPacketDataIndex_Date( calData.GetRegion(), point, 
//					calData.GetDisplayDataStart(), calData.GetDisplayDataEnd());

		//>> (2008/12/26 - Seung-Lyong, Park) 메모리릭 제거
//		int nIndex = calData.GetIChartManager()->FindViewDataIndex_Pt("DEFAULT", point.x);
		//<< (2008/12/26 - Seung-Lyong, Park) 메모리릭 제거
		
		double x = 0.0;
//		pIPacket->GetDataPastandFuture(nIndex, x);
		x = GetData_X(calData, point, 0);
		double temp = CDataConversion::AbsoluteToRelative_Y(calData.GetRegion(), point.y, 
					calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());

		// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
		if( -HUGE_VAL == temp) cdResult = CCoordinate(point);
		else
		{
			double y = calData.ApplyExp(temp);
			// 20081007 JS.Kim	사용자시간대 선택
			cdResult = CCoordinate(x, y, pIPacket->GetBaseTimeDifference());
		}
	}
	if( pIPacket) pIPacket->Release();
	return cdResult;
}

// 상대 좌표 -> 절대 좌표 : 저장된 좌표를 이용해 그리기위한 좌표
CPoint CCoordinateCalculator::RelativeToAbsolute(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == calData.GetDisplayDataMax_ScaleType() || -HUGE_VAL == calData.GetDisplayDataMin_ScaleType()) return point.GetPixel();

	CPoint ptResult;
	IPacket *pIPacket = calData.GetPacket_X();
	if(!IsCalculator( pIPacket)) 
	{
		ptResult = point.GetPixel();
	}
	else
	{
		int nCenter_x;
		double dX = 0.0;
		dX = point.GetX();
		// 20081007 JS.Kim	사용자시간대 선택<<
		int nPckTimeDiff = pIPacket->GetBaseTimeDifference();
		if( nPckTimeDiff != point.GetTimeDiff() )
			dX = pIPacket->CalcTimeDiff(dX, nPckTimeDiff - point.GetTimeDiff());
		//>>

		if( calData.IsValidAddInInterface())
		{
			int nXPos = calData.m_pIChartManager->GetXFromDouble(calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn(), dX, nCenter_x);
			int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", nXPos);
			ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
					calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
					calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
					nIndex,	calData.ApplyLog(point.GetY()));
			ptResult.x = nXPos;
		}
		//<< (2008/12/26 - Seung-Lyong, Park) 메모리릭 제거
	}
	if( pIPacket) pIPacket->Release();
	return ptResult;
}

// 절대 좌표 -> 절대 좌표 : 그리기 위한 좌표
CPoint CCoordinateCalculator::AbsoluteToAbsolute(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	CPoint ptResult;
	IPacket *pIPacket = calData.GetPacket_X();
	if( !IsCalculator( pIPacket)) ptResult = point;
	else
	{
		CCoordinate relativePt = AbsoluteToRelative(calData, point);
		CPoint drawingPoint = RelativeToAbsolute(calData, relativePt);
		ptResult = drawingPoint;
	}
	if( pIPacket) pIPacket->Release();
	return ptResult;
}

// 두점간의 간격
CCoordinate CCoordinateCalculator::GetGap(const CCoordinateCalculatorData& calData, const CPoint& point1, const CPoint& point2) const
{
//	CCoordinate cdResult;
//	// packet 이 없을 경우엔 그냥 좌표로 한다!!
//	IPacket *pIPacket = calData.GetPacket_X();
//	if(!IsCalculator( pIPacket)) cdResult = CCoordinate((double)(point2.x - point1.x), (double)(point2.y - point1.y));
//	else
//	{
//		CCoordinate relativePt1 = AbsoluteToRelative(calData, point1);
//		CCoordinate relativePt2 = AbsoluteToRelative(calData, point2);
//		int nIndex = GetDisplayDataCurrentIndex( pIPacket, (int)relativePt2.GetX()) - GetDisplayDataCurrentIndex( pIPacket, (int)relativePt1.GetX());
//		cdResult = CCoordinate((double)nIndex, (relativePt2.GetY() - relativePt1.GetY()));
//	}
//	if( pIPacket) pIPacket->Release();
//	return cdResult;

	CCoordinate cdResult;
	// packet 이 없을 경우엔 그냥 좌표로 한다!!
	IPacket *pIPacket = calData.GetPacket_X();
	// 20081007 JS.Kim	사용자시간대 선택
	if( !IsCalculator( pIPacket)) cdResult = CCoordinate((double)(point2.x - point1.x), (double)(point2.y - point1.y), pIPacket->GetBaseTimeDifference());
	else
	{
		// XScale : 관련 Interface로 사용 - vntsorl(20070327)
		CCoordinate AbsolutePt1 = AbsoluteToAbsolute(calData, point1);
		CCoordinate AbsolutePt2 = AbsoluteToAbsolute(calData, point2);
		CCoordinate relativePt1 = AbsoluteToRelative(calData, point1);
		CCoordinate relativePt2 = AbsoluteToRelative(calData, point2);
		cdResult = CCoordinate((AbsolutePt2.GetX() - AbsolutePt1.GetX()), (relativePt2.GetY() - relativePt1.GetY()), pIPacket->GetBaseTimeDifference());
	}
	if( pIPacket) pIPacket->Release();
	return cdResult;
}
/*
// 현재 point의 index를 가져오기.
int CCoordinateCalculator::GetCurrentDataIndex(const CRegion& region, const CPoint& point) const
{
	return CDataConversion::GetPacketDataIndex_Date(region.GetDrawingRegion(), point, 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock());
}

// 그릴 x좌표 가져오기.
int CCoordinateCalculator::GetDrawingPoint_X(const CRegion& region, const int nCurrentIndex) const
{
	return CDataConversion::RelativeToAbsolute_X(region.GetDrawingRegion(), 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock(), nCurrentIndex);
}

*/

double CCoordinateCalculator::GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const
{
	int nBlockColumn, nCenter_x;
	int nRow, nCol;
	double dReturn = 0;
	if( calData.IsValidAddInInterface())
	{
		calData.m_pIChartManager->GetBlockIndex(point, nRow, nCol);
		dReturn = calData.m_pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x);
	}
	return dReturn;
}

// private ====================================================================
bool CCoordinateCalculator::IsCalculator(IPacket* pPacket) const
{
	return (pPacket != NULL && pPacket->GetDataCount() > 0);
}

// ----------------------------------------------------------------------------
int CCoordinateCalculator::GetDisplayDataCurrentIndex(IPacket* pPacket, const int nData) const
{
	if(pPacket == NULL)
		return -1;
	
	return pPacket->GetIndexFromData(nData);
}

///////////////////////////////////////////////////////////////////////////////
// class CSelectableRegionCalculator


CSelectableRegionCalculator::CSelectableRegionCalculator():
	m_nProximityRange(4)
{
}

// 선택할 영역의 범위
int CSelectableRegionCalculator::GetProximityRange() const
{
	return m_nProximityRange;
}

// 두점을 잇는 선을 두르는 영역.
void CSelectableRegionCalculator::GetLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const
{
	// 수평선
	if(point1.y == point2.y){
		CRect temp = GetHorzLineRegion(point1, point2);
		region.CreateRectRgn(temp.left, temp.top, temp.right, temp.bottom);
	}
	// 수직선
	else if(point1.x == point2.x){
		CRect temp = GetVertLineRegion(point1, point2);
		region.CreateRectRgn(temp.left, temp.top, temp.right, temp.bottom);
	}
	// 사선
	else
		GetAngleLineRegion(point1, point2, region);
}

// 원점: origin, 반지름: l인 원에 point가 있는지 검사
bool CSelectableRegionCalculator::IsPointInCircle(const CPoint& origin, const CPoint& point) const
{
	return (pow(m_nProximityRange*2, 2) >= (pow(origin.x - point.x, 2) + pow(origin.y - point.y, 2)));
}

bool CSelectableRegionCalculator::IsPointInCircle(const CPoint& origin, const int nRadius, const CPoint& point) const
{
	return (pow(nRadius, 2) >= (pow(origin.x - point.x, 2) + pow(origin.y - point.y, 2)));
}


// private ====================================================================
// 수평선
CRect CSelectableRegionCalculator::GetHorzLineRegion(const CPoint& point1, const CPoint& point2) const
{
	assert(point1.y == point2.y);

	CRect selectableRegion(point1.x, (point1.y - m_nProximityRange), point2.x, (point1.y + m_nProximityRange +1));
	selectableRegion.NormalizeRect();

	return selectableRegion;
}

// 수직선
CRect CSelectableRegionCalculator::GetVertLineRegion(const CPoint& point1, const CPoint& point2) const
{
	assert(point1.x == point2.x);

	CRect selectableRegion((point1.x - m_nProximityRange), point1.y, (point1.x + m_nProximityRange +1), point2.y);
	selectableRegion.NormalizeRect();

	return selectableRegion;
}

// 사선
void CSelectableRegionCalculator::GetAngleLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const
{
	assert(point1.x != point2.x && point1.y != point2.y);

	// (0, 0)을 기준으로 한 ProximityRange 떨어진 두개의 좌표 구하기.
	CPoint basePt1, basePt2;
	GetVerticalPointsFromLength(point1, point2, basePt1, basePt2);

	POINT selectableRegionPoints[4];
	selectableRegionPoints[0] = GetPointOnMovedOrigin(point1, basePt1);
	selectableRegionPoints[1] = GetPointOnMovedOrigin(point1, basePt2);
	selectableRegionPoints[2] = GetPointOnMovedOrigin(point2, basePt2);
	selectableRegionPoints[3] = GetPointOnMovedOrigin(point2, basePt1);

	region.CreatePolygonRgn(selectableRegionPoints, 4, ALTERNATE);
}

// ----------------------------------------------------------------------------
// 좌표 이동
POINT CSelectableRegionCalculator::GetPointOnMovedOrigin(const CPoint& origin, const CPoint& point) const
{
	POINT movePoint;
	movePoint.x = origin.x + point.x;
	movePoint.y = origin.y + point.y;

	return movePoint;
}

// 특정 길이 만큼 떨어진 좌표 구히기
// --> 기울기가 a 이고 (0, 0)을 지나고 (0, 0)에서 특정길이 +- L 떨어지 좌표 구하기.
// --> x1 = k, y1 = a*k,    x2 = -k, y2 = a*-k
// --> k = |(L*L)/(a*a + 1)| 의 루트값
void CSelectableRegionCalculator::GetVerticalPointsFromLength(const CPoint& pt1, const CPoint& pt2, CPoint& selectableRegionPt1, CPoint& selectableRegionPt2) const
{
	// 두점의 수직기울기 구한다. == 현재 구할 두 좌표의 기울기
	double lineAngle = GetLineVerticalAngle(GetLineAngle(pt1, pt2));

	double x = 0.0;
	x = sqrt(fabs(pow(m_nProximityRange, 2) / (pow(lineAngle, 2) +1)));

	selectableRegionPt1 = CPoint(CMath::Round(x), CMath::Round(lineAngle * x));
	selectableRegionPt2 = CPoint(-selectableRegionPt1.x, -selectableRegionPt1.y);
}

// ----------------------------------------------------------------------------
// 좌표의 기울기.
// --> 두점의 기울기 : (y2-y1)/(x2-x1)
double CSelectableRegionCalculator::GetLineAngle(const CPoint& point1, const CPoint& point2) const
{
	return ((double)(point2.y - point1.y) / (double)(point2.x - point1.x));
}

// 수직의 기울기.
// --> -1/a
double CSelectableRegionCalculator::GetLineVerticalAngle(const double& dAngle) const
{
	assert(dAngle != 0.0);
	return (-1.0/dAngle);
}