// PointElement.h: interface for the CPointElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTELEMENT_H__DB479331_2418_4D88_9BC6_BF05919F8F89__INCLUDED_)
#define AFX_POINTELEMENT_H__DB479331_2418_4D88_9BC6_BF05919F8F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"
#include "Coordinate.h"

#include "AnalysisData.h"

class COnePointDrawer;
class CTwoPointDrawer;
class CThreePointDrawer;
class CMultiPointDrawer;
class COnePointDrawingData;
class CTwoPointDrawingData;
class CThreePointDrawingData;
class CMultiPointDrawingData;
class CDisplayData;
class CGraphPartRegion;
class CRegion;


// 저장될 자표가 1개인 경우.
// --> 십자선, 수직선, 수평선, 시간대, 갠그리드
class COnePointElement : public CElement
{
public:
	COnePointElement(const CCoordinate& point, const CElementCommonEnvironment& environment);

	virtual void GetPoints(CElementPointList& elementPointList);
	virtual CToolEnumData::ModifyType GetModifyType(CExtractingDataOfModifyType* pExtractingData) const;
	virtual CCoordinate GetCoordinate(const CToolEnumData::ModifyType eModifyType) const;	

	virtual void DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
	virtual void DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData);

protected:
	CCoordinate& GetPoint();
	CCoordinate GetPoint() const;
	void SetPoint(const CCoordinate& point);
	COnePointDrawingData GetOnePointDrawingData(const CSelectedElementDrawingData& selectedDrawingData) const;

	CToolEnumData::ModifyType GetModifyType_Point(const CPoint& drawingPt, const CPoint& point) const;
	CPoint GetSelectedDrawPoint(const CSelectedElementDrawingData& selectedDrawingData) const;

	virtual CString GetSavedPoints() const;
	virtual void RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
	
	virtual COnePointDrawer& GetOnePointDrawer() = 0;	
	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const = 0;
	virtual CPoint GetGripPoint(const CCoordinateCalculatorData& calData);

private:
	CCoordinate m_point;

	COnePointDrawingData GetOnePointDrawingData(const CDrawingRegions* pDrawingRegions, const CCoordinateCalculatorData& calData, const CPoints& Points) const;
	CDisplayData GetDisplayData(CExtractingDataOfModifyType* pExtractingData, const int nIndex) const;
};


// 저장될 좌표가 2개인 경우.
// --> 추세선, 삼등분선, 사등분선, 피보나치 반원(원), 펜라인, 피보나치 조정대
class CTwoPointElement : public CElement
{
public:
	CTwoPointElement(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual void GetPoints(CElementPointList& elementPointList);
	virtual CToolEnumData::ModifyType GetModifyType(CExtractingDataOfModifyType* pExtractingData) const;
	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType);
	virtual CCoordinate GetCoordinate(const CToolEnumData::ModifyType eModifyType) const;

	virtual void DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
	virtual void DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData);

protected:
	CCoordinate& GetStartPoint();
	CCoordinate GetStartPoint() const;
	CCoordinate& GetEndPoint();
	CCoordinate GetEndPoint() const;
	CString GetSavedPoint(const CCoordinate& point) const;
	void SetStartPoint(const CCoordinate& point);
	void SetEndPoint(const CCoordinate& point);

	virtual CString GetSavedPoints() const;
	virtual void RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);

	virtual CTwoPointDrawer& GetTwoPointDrawer() = 0;
	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData,const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

	CToolEnumData::ModifyType GetModifyType_SidePoints(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
	CToolEnumData::ModifyType GetModifyType_Line(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
	CToolEnumData::ModifyType GetModifyType_Rect(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

private:
	CCoordinate m_startPoint;
	CCoordinate m_endPoint;

	// 그릴 절대좌표 가져오기
	void GetAbsolutePoints(const CCoordinateCalculatorData& calData, CPoint& startPoint, CPoint& endPoint) const;
	CTwoPointDrawingData GetTwoPointDrawingData(const CGraphPartRegion& graphPartRegion, const CCoordinateCalculatorData& calData) const;
	CTwoPointDrawingData GetTwoPointDrawingData(const CSelectedElementDrawingData& selectedDrawingData) const;
	CDisplayData GetDisplayData(CExtractingDataOfModifyType* pExtractingData, const int nIndex) const;

// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
public:
	static BOOL	UpdatePointsForPriceRangeOption( const CCoordinateCalculatorData& calData, CCoordinate &p_startPoint, CCoordinate &p_endPoint, double &dMax, double &dMin);
};

// 저장될 좌표가 3개인 경우.
// --> 엔드류 피치포크
class CThreePointElement : public CElement
{
public:
	CThreePointElement(const CCoordinate& point1, const CCoordinate& point2, const CCoordinate& point3, const CElementCommonEnvironment& environment);

	virtual void GetPoints(CElementPointList& elementPointList);
	virtual CToolEnumData::ModifyType GetModifyType(CExtractingDataOfModifyType* pExtractingData) const;
	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType);
	virtual CCoordinate GetCoordinate(const CToolEnumData::ModifyType eModifyType) const;	

	virtual void DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
	virtual void DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData);

protected:
	virtual CString GetSavedPoints() const;
	virtual void RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);

	virtual CThreePointDrawer& GetThreePointDrawer() = 0;
	virtual bool ElementInRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const = 0;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const;

private:
	CCoordinate m_point1;
	CCoordinate m_point2;
	CCoordinate m_point3;

	// 그릴 절대좌표 가져오기
	void GetAbsolutePoints(const CCoordinateCalculatorData& calData, CPoint& pt1, CPoint& pt2, CPoint& pt3) const;
	CThreePointDrawingData GetThreePointDrawingData(const CRect& rect, const CCoordinateCalculatorData& calData) const;
	CThreePointDrawingData GetThreePointDrawingData(const CSelectedElementDrawingData& selectedDrawingData) const;

	CToolEnumData::ModifyType GetModifyType_SidePoints(const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const;
};

// 다중추세선 
class CMultiPointElement : public CElement
{
public:
	CMultiPointElement(const int& nPointsCount,const CPoints* pPointsList,const CElementCommonEnvironment& environment);
	CMultiPointElement(const CDequeList<CCoordinate>& pointList,const CElementCommonEnvironment& environment);
	
	virtual void GetPoints(CElementPointList& elementPointList);
	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType) ;
	virtual CCoordinate GetCoordinate(const CToolEnumData::ModifyType eModifyType) const;

	virtual void DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
	virtual void DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData);

protected:
	CCoordinate& GetDrawingPoint(int nIndex);
	CCoordinate GetDrawingPoint(int nIndex) const;
	CString GetSavedPoint(const CCoordinate& point) const;
	void AddDrawingPoint(const CCoordinate& point);

	virtual CString GetSavedPoints() const;
	virtual void RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);

	virtual CMultiPointDrawer& GetMultiPointDrawer() = 0;
	virtual CToolEnumData::ModifyType GetModifyType(CExtractingDataOfModifyType* pExtractingData) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region,const CPoint& drawingStartPt, const CPoint& drawingEndPt,const CPoint& point) const;
private:
	int m_nPointsCount;
	CPoints m_ptPointsList[500];
	
	// 그릴 절대좌표 가져오기
	void GetAbsolutePoints(const CCoordinateCalculatorData& calData, CPoint& drawingPoint,int nIndex) const;
	CMultiPointDrawingData GetMultiPointDrawingData(const CRect& rect, const CCoordinateCalculatorData& calData) const;
	CMultiPointDrawingData GetMultiPointDrawingData(const CSelectedElementDrawingData& selectedDrawingData) const;
	
	CToolEnumData::ModifyType GetModifyType_SidePoints(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;	
};

// ----------------------------------------------------------------------------
class CVertElement : public COnePointElement
{
public:
	CVertElement(const CCoordinate& point, const CElementCommonEnvironment& environment);

	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType);

protected:
	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const;
	CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& elementDrawingPt, const CPoint& point) const;

private:
	bool PointInElementSelectableRegion(const CRect& region, const CPoint& regionPt, const CPoint& point) const;
};

#endif // !defined(AFX_POINTELEMENT_H__DB479331_2418_4D88_9BC6_BF05919F8F89__INCLUDED_)
