// LineDrawer.h: interface for the CLineDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEDRAWER_H__F1C75F39_C436_4C43_B2BC_28E18AB8E456__INCLUDED_)
#define AFX_LINEDRAWER_H__F1C75F39_C436_4C43_B2BC_28E18AB8E456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/BlockBaseEnum2.h"		// for CAnalysis
#include "PointDrawer.h"

class CVertScaleType;
// 추세선 그리기
class CLineDrawer : public CTwoPointDrawer
{
public:
	CLineDrawer( const CAnalysis::TOOLTYPE p_eToolType = CAnalysis::EN_ANALYSIS_LINE);
	void SetLineToolType(const CAnalysis::TOOLTYPE eToolType){	m_eToolType = eToolType;	}
	const CAnalysis::TOOLTYPE GetLineToolType()				 {	return m_eToolType;			}	
protected:
	CAnalysis::TOOLTYPE		m_eToolType;	
public:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);
};

// ----------------------------------------------------------------------------
// 수평선 그리기
class CHorzLineDrawer : virtual public COnePointDrawer
{
public:
	CHorzLineDrawer();

protected:
	virtual bool IsDrawing(const CRect& region, const CPoint& point) const;
	virtual void DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion);
	virtual void DrawText(CDC* pDC, const COnePointDrawingData& drawingData);
	virtual CRect GetGripRegion(const CRect& region, const CPoint& point);

	void DrawShape(CDC* pDC, const CRect& region, const CPoint& point);

private:
	void DrawText(CDC* pDC, const CString& textData, const COnePointDrawingData& drawingData, const bool bTop);
	CRect GetTextRegion(const CBlockRegion& blockRegion, const CSize& textSize, const int nY, const bool bLeft) const;
	CRect GetTextRegion_Left(const CBlockRegion& blockRegion, const CSize& textSize) const;
	CRect GetTextRegion_Right(const CBlockRegion& blockRegion, const CSize& textSize) const;

private:
	const COLORREF TEXT_COLOR;
	const COLORREF TEXT_BK_COLOR;
};

// ----------------------------------------------------------------------------
// 십자선 그리기
class CCrossLineDrawer : public CVertLineDrawer, CHorzLineDrawer
{
public:
	void SelectedDraw_VertLine(CDC* pDC, const CDrawingRegions& drawingRegions, const CPoint& point);
	void SelectedDraw_HorzLine(CDC* pDC, const CDrawingRegions& drawingRegions, const CPoint& point);

protected:
	virtual bool IsDrawing(const CRect& region, const CPoint& point) const;
	virtual void DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion);
	virtual void DrawText(CDC* pDC, const COnePointDrawingData& drawingData);
	virtual CRect GetGripRegion(const CRect& region, const CPoint& point);
};

// ----------------------------------------------------------------------------
// 등분 그리기
class CEqualDivisionLineDrawer : public CTwoPointDrawer
{
public:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawShadow(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);

	virtual int GetCount(BOOL bEquiDiv) const = 0;
	int GetPoint_Y(const int nY1, const int nY2, const int nIndex,BOOL bEquiDiv) const;

protected:
	virtual double GetCoefficient(const int nIndex,BOOL bEquiDiv) const = 0;

private:
	void DrawText_Left(CDC* pDC, const CTwoPointDrawingData& drawingData);
	void DrawText_Right(CDC* pDC, const CTwoPointDrawingData& drawingData);

	CString GetRightText(const int nIndex,BOOL bEquiDivType) const;
	CPoints GetPoints(const CVertScaleType& vertScaleType, const CPoints& points1, const CPoints& points2, const int nIndex,BOOL bEquiDiv) const;
	double GetSavingPoint_Y(const CVertScaleType& vertScaleType, const CPoints& points1, const CPoints& points2, const int nDrawingPoint_Y) const;
	int GetDrawingPoint_Y(const int origin, const int nHeight, const int nIndex,BOOL bEquiDiv) const;
};

// ----------------------------------------------------------------------------
// 삼등분선 그리기
class CTrisectionLineDrawer : public CEqualDivisionLineDrawer
{
public:
	virtual int GetCount(BOOL bEquiDivType) const;
protected:
	virtual double GetCoefficient(const int nIndex,BOOL bEquiDivType = FALSE) const;
};

// ----------------------------------------------------------------------------
// 사등분선 그리기
class CQuadrisectLineDrawer : public CEqualDivisionLineDrawer
{
public:
	virtual int GetCount(BOOL bEquiDivType = FALSE) const;
protected:
	virtual double GetCoefficient(const int nIndex,BOOL bEquiDivType = FALSE) const;
};

// ----------------------------------------------------------------------------
// 엔드류 피치포크 그리기
class CAndrewsPitchforkDrawer : public CThreePointDrawer
{
public:
	void GetPoints(const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, CPoint& centerPt, CPoint& sidePt1, CPoint& sidePt2) const;
	CPoint GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3) const;

protected:
	virtual void DrawShape(CDC* pDC, const CRegion& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3);

private:
	CPoint GetMiddlePointInPoints(const CPoint& pt1, const CPoint& pt2) const;
	CPoint GetPointInLine(const CPoint& baseLinePt1, const CPoint& baseLinePt2, const CPoint& point) const;
	CPoint GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2) const;

	void DrawLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
};

// ----------------------------------------------------------------------------
// 사이클 구간 그리기
class CCycleLinesDrawer : public CTwoPointDrawer
{
public:
	int GetPoints_Index1()	const;
	int GetPoints_Index2()	const;
	int GetDrawingPoint_X(const CDisplayData& displayData,int nIndex) const;
	bool GetIndexInDisplayAttributes(const CDisplayData& displayData,int nIndex) const;
protected:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);	
private:
	void DrawFigure_Data(CDC* pDC, const CDisplayData& displayData, const CPoint& pt1, const CPoint& pt2);	
	CRegion GetCoordinateCalRegion(const CDisplayData& displayData) const;
//Attributes
private:
	int m_nPointIndex1;
	int m_nPointIndex2;
};

// ----------------------------------------------------------------------------
// 스피드 라인 그리기
class CSpeedLineDrawer : public CTwoPointDrawer
{
public:
	int GetCount() const;

	void GetPoints(const CPoint& pt1, const CPoint& pt2, CPoint& leftPt, CPoint& rightPt) const;
	// 해당 index번의 교차점.
	bool GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;

protected:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);

private:
	double GetCoefficient(const int nIndex) const;
};

// ----------------------------------------------------------------------------
// 스피드 FAN 그리기
class CSpeedFanDrawer : public CTwoPointDrawer
{
public:
	int GetCount() const;

	void GetPoints(const CPoint& pt1, const CPoint& pt2, CPoint& leftPt, CPoint& rightPt) const;
	// 해당 index번의 교차점.
	bool GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;

protected:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
private:
	double GetCoefficient(const int nIndex) const;
};

// ----------------------------------------------------------------------------
// 스피드 ARC 그리기
class CSpeedArcDrawer : public CTwoPointDrawer
{
public:
	int GetCount() const;

	// 호의 반지름 구하기.
	int GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const;
	
	void GetPoints(const CPoint& pt1, const CPoint& pt2, CPoint& leftPt, CPoint& rightPt) const;
	// 해당 index번의 교차점.
	bool GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;	
protected:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawArc(CDC* pDC, const CRect& region, const int startY, const int endY);
private:
	double GetCoefficient(const int nIndex) const;
	CRect GetSemicircleRegion(const CPoint& centerPt, const int length) const;
	void DrawSemicircle(CDC* pDC, const CRect& region, const int startX, const int endX);	
};

// ----------------------------------------------------------------------------
// 선형회귀선 그리기
class CLinearRegressionDrawer : public CTwoPointDrawer
{
public:	
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);	
	virtual void DrawShadow(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
};

// ----------------------------------------------------------------------------
// 선형회귀채널 그리기
class CRaffRegressionDrawer : public CTwoPointDrawer
{
public:	
	CRaffRegressionDrawer ();
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawShadow(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
	CRegion GetCoordinateCalRegion(const CDisplayData& displayData) const;
	void SetChannel(double dChannel)	{	m_dChannel = dChannel;		}
	double GetChannel() const;
private:
	double m_dChannel;
};

// ----------------------------------------------------------------------------
// 각도추세선 그리기
class CAngleLineDrawer : public CTwoPointDrawer
{
public:
	CAngleLineDrawer();	
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);
};

// ----------------------------------------------------------------------------
// Candle중심선 그리기
class CCandleLineDrawer : public COnePointDrawer
{
public:
	CCandleLineDrawer();

protected:
	virtual void DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion);
	virtual void DrawText(CDC* pDC, const COnePointDrawingData& drawingData);	
	virtual CRect GetGripRegion(const CRect& region, const CPoint& point);
private:
	bool IsDrawing(const CRect& region, const CPoint& point) const;
};

// ----------------------------------------------------------------------------
// 허수봉 그리기
class CCandleBongDrawer : public CTwoPointDrawer
{
protected:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
private:
	bool IsDrawing(const CRect& region, const CPoint& point) const;

};

// ----------------------------------------------------------------------------
// Pencil 그리기
class CPencilLineDrawer : public CMultiPointDrawer
{
protected:
	virtual void DrawShape(CDC* pDC, const CMultiPointDrawingData& drawingData);	
};

// ----------------------------------------------------------------------------
// PeriodSum 그리기
class CPeriodSumLineDrawer : public CTwoPointDrawer
{
public:	
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);
private:
	CRegion GetCoordinateCalRegion(const CDisplayData& displayData) const;
};

// ----------------------------------------------------------------------------
// ElliotLineDrawer 
class CElliotLineDrawer : public CTwoPointDrawer
{
public:	
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
	
	// 해당 index번의 교차점.
	bool GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex,const CPoint pointFrom, CPoint& point) const;
private:
	double GetCoefficient(const int nIndex) const;
	int GetCount() const;
};

// ----------------------------------------------------------------------------
// 조정백분율 그리기
class CAdjustRatioDrawer : public CThreePointDrawer
{
protected:
	virtual void DrawShape(CDC* pDC, const CRegion& region, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3);
};
#endif // !defined(AFX_LINEDRAWER_H__F1C75F39_C436_4C43_B2BC_28E18AB8E456__INCLUDED_)
