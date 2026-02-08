// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__98CEB7AA_96D5_45F3_B751_6B3BEA50FFB2__INCLUDED_)
#define AFX_LINE_H__98CEB7AA_96D5_45F3_B751_6B3BEA50FFB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointElement.h"
#include "LineDrawer.h"

// 추세선 객체
class CLine : public CTwoPointElement
{
public:
	CLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2,
		const CAnalysis::TOOLTYPE p_eToolType, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CLineDrawer m_lineDrawer;

protected:
	CAnalysis::TOOLTYPE					m_eToolType;		// Graph Type 속성이다. 초기설정후 불변한다.
protected:
	virtual bool						ElementInRegion( const CRect &p_rctGraphArea, const CPoint &p_ptStart,
											const CPoint &p_ptEnd, const CPoint &p_ptCursor) const;
	virtual CToolEnumData::ModifyType	GetModifyType( const CRect &p_rctGraphArea, const CPoint &p_ptStart,
											const CPoint &p_ptEnd, const CPoint &p_ptCursor) const;
};

// ----------------------------------------------------------------------------
// 수직선 객체
class CVertLine : public CVertElement
{
public:
	CVertLine(const CTextVertPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();

private:
	CTextVertPosition m_textPosition;
	CVertLineDrawer m_vertLineDrawer;
};

// ----------------------------------------------------------------------------
// 수평선 객체
class CHorzLine : public COnePointElement
{
public:
	CHorzLine(const CTextHorzPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;
	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType);

protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();

	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const;

private:
	CTextHorzPosition m_textPosition;
	CHorzLineDrawer m_horzLineDrawer;

	bool PointInElementSelectableRegion(const CRect& region, const CPoint& regionPt, const CPoint& point) const;
};

// ----------------------------------------------------------------------------
// 십자선 객체
class CCrossLine : public CVertElement
{
public:
	CCrossLine(const CTextHorzVertPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;
	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType);
	virtual void DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData);

protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();

	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const;
private:
	void ElementModify_X(const CCoordinate& modifyPoint);
	void ElementModify_Y(const CCoordinate& modifyPoint);

private:
	CTextHorzVertPosition m_textPosition;
	CCrossLineDrawer m_crossLineDrawer;
};

// ----------------------------------------------------------------------------
// 등분 관련 객체
class CEqualDivisionLine : public CTwoPointElement
{
public:
	CEqualDivisionLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

	virtual const CEqualDivisionLineDrawer* GetEqualDivisionLineDrawer() const = 0;

private:
	CTextHorzPosition m_textPosition;

	CToolEnumData::ModifyType GetModifyTypeLine(const CRect& region,const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
};

// ----------------------------------------------------------------------------
// 삼등분선 객체
class CTrisectionLine : public CEqualDivisionLine
{
public:
	CTrisectionLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	virtual const CEqualDivisionLineDrawer* GetEqualDivisionLineDrawer() const;

private:
	CTrisectionLineDrawer m_trisectionLineDrawer;
};

// ----------------------------------------------------------------------------
// 사등분선 객체
class CQuadrisectLine : public CEqualDivisionLine
{
public:
	CQuadrisectLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);
	
	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	virtual const CEqualDivisionLineDrawer* GetEqualDivisionLineDrawer() const;

private:
	CQuadrisectLineDrawer m_quadrisectLineDrawer;
};

// ----------------------------------------------------------------------------
// 엔드류 피치포크 객체
class CAndrewsPitchfork : public CThreePointElement
{
public:
	CAndrewsPitchfork(const CCoordinate& point1, const CCoordinate& point2, const CCoordinate& point3, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CThreePointDrawer& GetThreePointDrawer();
	virtual bool ElementInRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const;

private:
	CAndrewsPitchforkDrawer m_andrewsDrawer;

	bool PointInElementSelectableRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const;
};

// ----------------------------------------------------------------------------
// 사이클 구간 객체
class CCycleLines : public CTwoPointElement
{
public:
	CCycleLines(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
	//virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

private:
	CCycleLinesDrawer m_cycleLinesDrawer;

	CToolEnumData::ModifyType GetModifyType_Lines(const CDisplayData& displayData, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
};

// ----------------------------------------------------------------------------
// 스피드 라인
class CSpeedLine : public CTwoPointElement
{
public:
	CSpeedLine(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

private:
	CSpeedLineDrawer m_speedLineDrawer;	
};

// ----------------------------------------------------------------------------
// 스피드 FAN
class CSpeedFan : public CTwoPointElement
{
public:
	CSpeedFan(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();

	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

private:
	CSpeedFanDrawer m_speedFanDrawer;
};

// ----------------------------------------------------------------------------
// 스피드 ARC
class CSpeedArc : public CTwoPointElement
{
public:
	CSpeedArc(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();

	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
	virtual CPoint GetDrawingRegion_EndPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const;
private:
	CSpeedArcDrawer m_speedArcDrawer;
	bool PointInCircleAround(const int nSourceRadius, const int nTargetRadius) const;
	
	CPoint GetDrawingRegion_StartPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const;
	int GetDrawingRegion_StartPoint(const int nStartPoint, const int nEndPoint, const int nRadius) const;

	int	GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const;
};

// ----------------------------------------------------------------------------
// CLinearRegression 객체
class CLinearRegression : public CTwoPointElement
{
public:
	CLinearRegression(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType);	
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual CCoordinate GetCoordinate(const CToolEnumData::ModifyType eModifyType) const;
	virtual CToolEnumData::ElementType GetElementType() const;	
protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	//virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
private:
	CLinearRegressionDrawer m_linearRegressionDrawer;
};

// ----------------------------------------------------------------------------
// CRaffRegression 객체
class CRaffRegression : public CTwoPointElement
{
public:
	CRaffRegression(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment,double dChannel);

	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType);	
	virtual CCoordinate GetCoordinate(const CToolEnumData::ModifyType eModifyType) const;
	virtual CToolEnumData::ElementType GetElementType() const;	
protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	//virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
private:
	CRaffRegressionDrawer m_raffRegressionDrawer;
};

// ----------------------------------------------------------------------------
// CAngleLine
class CAngleLine : public CTwoPointElement
{
public:
	CAngleLine(const CTextShowPosition& position, const CCoordinate& point1, const CCoordinate& point2,const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;
protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextShowPosition m_textPosition;
	CAngleLineDrawer m_angleLineDrawer;

protected:
	virtual bool						ElementInRegion(const CRect &p_rctGraphArea, const CPoint &p_ptStart,
														const CPoint &p_ptEnd, const CPoint &p_ptCursor) const;
	virtual CToolEnumData::ModifyType	GetModifyType(const CRect &p_rctGraphArea, const CPoint &p_ptStart,
													  const CPoint &p_ptEnd, const CPoint &p_ptCursor) const;
};

// ----------------------------------------------------------------------------
// 캔들 중심선 
class CCandleLine : public COnePointElement
{
public:
	CCandleLine(const CTextShowPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;
	virtual void ElementModify(const CElementModifyData& elementModifyData);
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
	virtual void ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType);

protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();

	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const;

private:
	CTextShowPosition m_textPosition;
	CCandleLineDrawer m_candleLineDrawer;

	bool PointInElementSelectableRegion(const CRect& region, const CPoint& regionPt, const CPoint& point) const;
};

// ----------------------------------------------------------------------------
// 허수봉
class CCandleBong : public CTwoPointElement
{
public:
	CCandleBong(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

private:
	CCandleBongDrawer m_candleBongDrawer;	
};

// ----------------------------------------------------------------------------
// 연필추세선
class CPencilLine : public CMultiPointElement
{
public:
	CPencilLine(const int& nPointsCount,const CPoints* pPointsList,const CElementCommonEnvironment& environment);
	CPencilLine(const CDequeList<CCoordinate>& pointList,const CElementCommonEnvironment& environment);
	
	virtual CToolEnumData::ElementType GetElementType() const;
protected:
	virtual CMultiPointDrawer& GetMultiPointDrawer();
private:
	CPencilLineDrawer m_pencilLineDrawer;
};

// ----------------------------------------------------------------------------
// CPeriodSumLine
class CPeriodSumLine : public CTwoPointElement
{
public:
	CPeriodSumLine(const CTextHorzPosition& position, const CCoordinate& point1, const CCoordinate& point2,const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;
protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CPeriodSumLineDrawer m_periodSumLineDrawer;

protected:
	virtual bool						ElementInRegion(const CRect &p_rctGraphArea, const CPoint &p_ptStart,
														const CPoint &p_ptEnd, const CPoint &p_ptCursor) const;
	virtual CToolEnumData::ModifyType	GetModifyType(const CRect &p_rctGraphArea, const CPoint &p_ptStart,
													  const CPoint &p_ptEnd, const CPoint &p_ptCursor) const;
};

// ----------------------------------------------------------------------------
// CElliotLine
class CElliotLine : public CTwoPointElement
{
public:
	CElliotLine(const CCoordinate& point1, const CCoordinate& point2,const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CElliotLineDrawer m_elliotLineDrawer;

protected:
	CAnalysis::TOOLTYPE					m_eToolType;		// Graph Type 속성이다. 초기설정후 불변한다.
protected:
	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType	GetModifyType( const CRect &p_rctGraphArea, const CPoint &p_ptStart,
											const CPoint &p_ptEnd, const CPoint &p_ptCursor) const;
};

// ----------------------------------------------------------------------------
// 조정 백분율
class CAdjustRatio : public CThreePointElement
{
public:
	CAdjustRatio(const CCoordinate& point1, const CCoordinate& point2, const CCoordinate& point3, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CThreePointDrawer& GetThreePointDrawer();
	virtual bool ElementInRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const;

private:
	CAdjustRatioDrawer m_adjustRatioDrawer;

	bool PointInElementSelectableRegion(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const;
};
#endif // !defined(AFX_LINE_H__98CEB7AA_96D5_45F3_B751_6B3BEA50FFB2__INCLUDED_)
