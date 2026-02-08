// CoordinateCalculator.h: interface for the CCoordinateCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDINATECALCULATOR_H__4514D7F6_F1A7_4081_B2BA_BE4243553ED1__INCLUDED_)
#define AFX_COORDINATECALCULATOR_H__4514D7F6_F1A7_4081_B2BA_BE4243553ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

class CCoordinate;
class CCoordinateCalculatorData;
class CRegion;

// 절대/상대 좌표를 변경
class CCoordinateCalculator
{
public:
	// 절대 좌표(Absolute) -> 상대 좌표(Relative) : 저장을 위한 좌표
	CCoordinate AbsoluteToRelative(const CCoordinateCalculatorData& calData, const CPoint& point) const;
	// 절대 좌표(Absolute) -> 상대 좌표(Relative) : 현재 종가점 위치 저장을 위한 좌표
	CCoordinate AbsoluteToRelativeAutoPrice(const CCoordinateCalculatorData& calData, const CPoint& point) const;
	// 상대 좌표 -> 절대 좌표 : 저장된 좌표를 이용해 그리기위한 좌표
	CPoint RelativeToAbsolute(const CCoordinateCalculatorData& calData, const CCoordinate& coordinate) const;	
	int RelativeToAbsolute_Y(const CCoordinateCalculatorData& calData,int nIndex,const double& dYValue) const;
	// 절대 좌표 -> 절대 좌표 : 그리기 위한 좌표
	CPoint AbsoluteToAbsolute(const CCoordinateCalculatorData& calData, const CPoint& point) const;

	// 두점간의 간격
	CCoordinate GetGap(const CCoordinateCalculatorData& calData, const CPoint& point1, const CPoint& point2) const;
	// 현재 point의 index를 가져오기.
	int GetCurrentDataIndex(const CRegion& region, const CPoint& point) const;
	// 그릴 x좌표 가져오기.
	int GetDrawingPoint_X(const CRegion& region, const int nCurrentIndex) const;

protected:
	virtual double GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const = 0;	// XScale : 관련 Interface로 사용 - vntsorl(20070410)
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const = 0;
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const = 0;

private:
	bool IsCalculator(IPacket* pPacket) const;

	// 분차트 근접시간 적용 - ojtaso (20080811)
	int GetDisplayDataCurrentIndex(IPacket* pPacket, const double dData, const CString strType = _T("")) const;
public:
	CPoint RelativeToAbsolute_NoAutoPrice(const CCoordinateCalculatorData& calData, const CCoordinate& point) const;
};

class CCoordinateCalculator_AutoPrice : public CCoordinateCalculator
{
protected:
	virtual double GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const;	// XScale : 관련 Interface로 사용 - vntsorl(20070410)
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const;
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const;

private:
	bool IsAutoPriceCalculator(IPacket* pPacket, const int nIndex) const;
};

class CCoordinateCalculator_NoAutoPrice : public CCoordinateCalculator
{
protected:
	virtual double GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const;	// XScale : 관련 Interface로 사용 - vntsorl(20070410)
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const;
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const;
};

class CCoordinateCalculatorFinder
{
public:
	static CCoordinateCalculator& FindCoordinateCalculator(const bool bIsAutoPrice);

private:
	static CCoordinateCalculator_AutoPrice m_autoPriceCal;
	static CCoordinateCalculator_NoAutoPrice m_noAutoPriceCal;
};

// ----------------------------------------------------------------------------
// element 선택을 위해 필요한 영역을 가져오기 위한 class
class CSelectableRegionCalculator
{
public:
	CSelectableRegionCalculator();

	// 선택할 영역의 범위
	int GetProximityRange() const;

	// 두점을 잇는 선을 두르는 영역.
	void GetLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const;
	// 원점: origin, 반지름: l인 원에 point가 있는지 검사
	bool IsPointInCircle(const CPoint& origin, const CPoint& point) const;
	bool IsPointInCircle(const CPoint& origin, const int nRadius, const CPoint& point) const;

private:
	int m_nProximityRange;

	// 수평선
	CRect GetHorzLineRegion(const CPoint& point1, const CPoint& point2) const;
	// 수직선
	CRect GetVertLineRegion(const CPoint& point1, const CPoint& point2) const;
	// 사선
	void GetAngleLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const;

	// 좌표 이동
	POINT GetPointOnMovedOrigin(const CPoint& origin, const CPoint& point) const;
	// 두좌표와 수직으로 특정 길이 만큼 떨어진 좌표.
	void GetVerticalPointsFromLength(const CPoint& pt1, const CPoint& pt2, CPoint& selectableRegionPt1, CPoint& selectableRegionPt2) const;

	// 좌표의 기울기.
	double GetLineAngle(const CPoint& point1, const CPoint& point2) const;
	// 수직의 기울기.
	double GetLineVerticalAngle(const double& dAngle) const;
};

#endif // !defined(AFX_COORDINATECALCULATOR_H__4514D7F6_F1A7_4081_B2BA_BE4243553ED1__INCLUDED_)
