// Element.h: interface for the CElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENT_H__3D8FCDC2_A93B_411F_9E52_627F2C81711C__INCLUDED_)
#define AFX_ELEMENT_H__3D8FCDC2_A93B_411F_9E52_627F2C81711C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/ElementEnvironment.h"
#include "../Include_Analysis/ToolBaseData.h"

class CPoints;
class CCoordinate;
class CDrawingRegions;
class CCoordinateCalculator;
class CCoordinateCalculatorData;
class CExtractingDataOfModifyType;
class CSelectedElementDrawingData;
class CElementModifyData;
class COrder;
class CElementPointList;

#include <math.h>
class CElement  
{
public:
	CElement(const CElementCommonEnvironment& environment);
	virtual ~CElement()	{}

	CString GetSavedElementData(const COrder& order) const;
	CElementCommonEnvironment GetCommonEnvironment() const;
	CElementCommonEnvironment& GetCommonEnvironment();
	CAnalysis::COORDINATETYPE GetCoordinateType() const;

	void ChangePenColor(const COLORREF& color);
	void ChangeTextColor(const COLORREF& color);
	CPoint GetAbsoluteDrawingPoint(const CCoordinateCalculatorData& calData, const CCoordinate& point) const;
	void ChangePenThick(int nNewWidth);	//[A00000552]alzioyes_20110628 element 선두께 변경하기.

public:
	virtual CToolEnumData::ElementType GetElementType() const = 0;	
	virtual void GetPoints(CElementPointList& elementPointList) = 0;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CToolEnumData::ModifyType GetModifyType(CExtractingDataOfModifyType* pExtractingData) const = 0;
	virtual CCoordinate GetCoordinate(const CToolEnumData::ModifyType eModifyType) const = 0;
	virtual void ElementModify(const CElementModifyData& elementModifyData) = 0;
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData) = 0;
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType) = 0;
	
	virtual void Draw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
	virtual void DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData) = 0;
	virtual void DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData) = 0;

//protected:
public:
	// 그릴 절대좌표 가져오기
	CPoint GetAbsolutePoint(const CCoordinateCalculatorData& calData, const CCoordinate& point) const;
	CPoint GetSelectedDrawPoint(const CSelectedElementDrawingData& selectedDrawingData, const CCoordinate& point) const;
	CPoints GetSelectedDrawPoints(const CSelectedElementDrawingData& selectedDrawingData, const CCoordinate& point) const;
	CPoints GetSelectedDrawPoints(const CCoordinateCalculatorData& calData, const CCoordinate& point) const;

	// 2011.02.18 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	static CString GetSavedPoint(const CCoordinate& point);
	// 2011.02.18 by SYS <<

protected:
	void CreatePen(const bool bIsElement, CPen& pen);

	// 선택할수 있는 범위
	bool PointInElementSelectableRegion(const CPoint& regionPoint1, const CPoint& regionPoint2, const CPoint& point) const;
	bool PointInElementSelectableFrameRect(const CPoint& regionPoint1, const CPoint& regionPoint2, const CPoint& point) const;
	CRect GetLineRect(const CRect &p_rctGraphArea,const CPoint &ptStart,const CPoint &ptEnd,BOOL bLeftExt = FALSE,BOOL bRightExt = FALSE) const;

	virtual CString GetSavedPoints() const = 0;
	virtual void RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions,const CCoordinateCalculatorData& calData) = 0;

	CElementCommonEnvironment m_environment;

private:
	CCoordinateCalculator& GetCoordinateCalculator() const;

	CString GetSavedIndex(const COrder& order) const;
	CString GetSavedElement() const;
	CString GetSavedToolType() const;
	CString GetSavedPoint() const;
	CString GetSavedTextPosition() const;
	CString GetSavedLineExt() const;

//	void DrawGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
};

#endif // !defined(AFX_ELEMENT_H__3D8FCDC2_A93B_411F_9E52_627F2C81711C__INCLUDED_)
