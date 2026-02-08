// FiboGann.h: interface for the CFiboGann class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIBOGANN_H__09C6571C_A41C_40B3_8838_D5EE3E98F528__INCLUDED_)
#define AFX_FIBOGANN_H__09C6571C_A41C_40B3_8838_D5EE3E98F528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointElement.h"
#include "FiboGannDrawer.h"

// 피보나치 호 객체
class CFiboArc : public CTwoPointElement
{
public:
	CFiboArc(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

protected:
	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

	virtual int GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const = 0;
	virtual CPoint GetDrawingRegion_EndPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const;

	int GetPoint_ApplyRadius(const int nOrgData, const int nRadius, const bool bPlusVector) const;

private:
	bool PointInCircleAround(const int nSourceRadius, const int nTargetRadius) const;

	CPoint GetDrawingRegion_StartPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const;
	int GetDrawingRegion_StartPoint(const int nStartPoint, const int nEndPoint, const int nRadius) const;
};

// 피보나치 반원 객체
class CFiboSemicircle : public CFiboArc
{
public:
	CFiboSemicircle(const CTextShowPosition& position,const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();

	virtual int GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
private:
	CFiboSemicircleDrawer m_fiboSemicircleDrawer;
	CTextShowPosition m_textPosition;
};

// 피보나치 원 객체
class CFiboCircle : public CFiboArc
{
public:
	CFiboCircle(const CTextShowPosition& position,const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();

	virtual int GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const;
	virtual CPoint GetDrawingRegion_EndPoint(const CPoint& startPoint, const CPoint& endPoint, const int nRadius) const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
private:
	CFiboCircleDrawer m_fiboCircleDrawer;
	CTextShowPosition m_textPosition;
};

// 피보나치 펜라인 객체
class CFiboPenLine : public CTwoPointElement
{
public:
	CFiboPenLine(const CTextShowPosition& position,const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;
protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	
	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
private:
	CFiboPenLineDrawer m_fiboPenLineDrawer;
	CTextShowPosition m_textPosition;
	CToolEnumData::ModifyType GetModifyType(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& fiboPoint, const CPoint& point) const;
};

// 피보나치 되돌림 객체
class CFiboRetrace : public CTwoPointElement
{
public:
	CFiboRetrace(const CTextShowPosition& position, const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;
protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();
		
	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;	
private:
	CFiboRetraceDrawer m_fiboRetraceDrawer;
	CTextShowPosition m_textPosition;
	CToolEnumData::ModifyType GetModifyType(const int nX1, const int nX2, const CPoint& fiboPoint, const CPoint& point) const;
};

// 피보 시간대 객체
class CFiboTime : public CVertElement
{
public:
	CFiboTime(const CCoordinate& point, const CElementCommonEnvironment& environment);
	CFiboTime(const CTextShowPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment);// 2008.08.20 김진순 피보나치 확장
	
	virtual CToolEnumData::ElementType GetElementType() const;
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
protected:
	virtual const CTextPosition* GetTextPosition() const;	// 2008.08.20 김진순 피보나치 확장
	virtual CTextPosition* GetTextPosition();

	virtual COnePointDrawer& GetOnePointDrawer();
	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const;
	virtual CPoint GetGripPoint(const CCoordinateCalculatorData& calData);
private:
	CDisplayData GetDisplayData(const CCoordinateCalculatorData& calData) const;

private:
	CFiboTimeDrawer m_fiboTimeDrawer;
	CTextShowPosition m_textPosition;		// 2008.08.20 김진순 피보나치 확장
};

// ----------------------------------------------------------------------------
// 갠 객체
class CGann : public CTwoPointElement
{
public:
	CGann(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();
	virtual CGannDrawer& GetGannDrawer() = 0;

	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType_Lines(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const = 0;

private:
	bool IsInAscendingPowers(const int nData1, const int nData2, const int nData3) const;

	CToolEnumData::ModifyType GetModifyType_SidePoints(const CPoint& pt1, const CPoint& pt2, const CPoint& point) const;
};

// 갠라인 객체
class CGannLine : public CGann
{
public:
	CGannLine(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment,int nGannType);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CGannDrawer& GetGannDrawer();

	virtual CToolEnumData::ModifyType GetModifyType_Lines(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;

private:
	int			m_nGannLineType;
	CGannLineDrawer m_gannLineDrawer;
};

// 갠펜 객체
class CGannFan : public CGann
{
public:
	CGannFan(const CTextShowPosition& position,const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment,int nGannType);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CGannDrawer& GetGannDrawer();

	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType_Lines(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;

private:
	CTextShowPosition m_textPosition;
	int			m_nGannFanType;
	CGannFanDrawer m_gannFanDrawer;
};

// 갠그리드 객체
class CGannGrid : public CTwoPointElement
{
public:
	CGannGrid(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment);

	virtual CToolEnumData::ElementType GetElementType() const;

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer();

	virtual bool ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const;
	virtual CToolEnumData::ModifyType GetModifyType(const CRect& region, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const;

private:
	CGannGridDrawer m_gannGridDrawer;

	bool ElementInRegion(const int minRegion, const int maxRegion, const int data) const;

	// drawingPt1 방향의 GannAngel line의 modifyType 구하기
	CToolEnumData::ModifyType GetModifyType_SideLines(const CRect& region, 
		const CPoint& drawingPt1, const CPoint& drawingPt2, const CPoint& point) const;
	// index번의 선에 대한 modify type 구하기.
	bool GetModifyType_Lines(const CRect& region, const CPoint& linePt1, const CPoint& linePt2, 
		const int nIndex, const CPoint& point, CToolEnumData::ModifyType& modifyType) const;
	// x축에 대칭인 좌표
	CPoint GetSymmetryPointOfXAxis(const CPoint& orgPt, const CPoint& point) const;
};

#endif // !defined(AFX_FIBOGANN_H__09C6571C_A41C_40B3_8838_D5EE3E98F528__INCLUDED_)
