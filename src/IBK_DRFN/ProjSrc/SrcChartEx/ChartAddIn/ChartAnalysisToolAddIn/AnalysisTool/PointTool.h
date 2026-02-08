// PointTool.h: interface for the CPointTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTTOOL_H__B37BF4F8_89BB_4F93_9106_07CB444E93B0__INCLUDED_)
#define AFX_POINTTOOL_H__B37BF4F8_89BB_4F93_9106_07CB444E93B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"

class CPoints;
class CDrawingRegions;
class COnePointDrawer;
class CTwoPointDrawer;
class CThreePointDrawer;
class CMultiPointDrawer;
class COnePointDrawingData;
class CTwoPointDrawingData;
class CThreePointDrawingData;
class CMultiPointDrawingData;

class CMoveTool : public CTool
{
protected:
	CMoveTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	virtual void Initialize();
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point);
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);
	virtual bool OnMouseMove(CDC* pDC, const CPoint& point,int nFlags);
	virtual void OnDraw(CDC* pDC);

protected:
	virtual bool PointInDrawingRegion(const CPoint& point) const;
	// 저장하기
	virtual void SavePoints(const CPoints& points);
	// 이전 point에 관한 내용의 그리기.
	virtual void DrawTool(CDC* pDC) = 0;

	CPoints GetPoints() const;

private:
	bool OnMouseMove_InNotDrawingRegion(const CPoint& point);

private:
	BOOL m_bMouseMoveChanged;
	CPoint m_ptMousePos;
	CPoints m_points;
};

class COnePointMoveTool : public CMoveTool
{
protected:
	COnePointMoveTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual COnePointDrawer& GetOnePointDrawer() = 0;
	// 이전 point에 관한 내용의 그리기.
	virtual void DrawTool(CDC* pDC);
	virtual CElement* MakeElement(const CElementCommonEnvironment& environment);

private:
	COnePointDrawingData GetOnePointDrawingData() const;
};

class CTwoPointMoveTool : public CMoveTool
{
protected:
	CTwoPointMoveTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CTwoPointDrawer& GetTwoPointDrawer() = 0;
	// 이전 point에 관한 내용의 그리기.
	virtual void DrawTool(CDC* pDC);
	virtual CElement* MakeElement(const CElementCommonEnvironment& environment);
	// 두번째 좌표를 구하기.
	virtual CPoints GetTwoPoints() const = 0;

//	만일 두번째좌표를 첫번째 좌표보다 더 많이 떨어지게 하고 싶다면 12의 값을 크게줄것 
	CPoints GetPoints(const CPoint& orgPoint, const int nAngle, const int nDataGap = 12) const;
	CPoint GetPoint(const CRegion& region, const CPoint& orgPoint, const int nAngle, const int nDataGap = 8) const;
	CCoordinate GetSavingPoint(const CPoint& regionPoint, const CPoint& savingPoint) const;

	int GetPoint_X(const CRegion& region, const CPoint& point, const int nGap) const;
	int GetPoint_Y(const CPoint& orgPoint, const int x2, const int nAngle) const;

private:
	CTwoPointDrawingData GetTwoPointDrawingData() const;
};

// ----------------------------------------------------------------------------
class CDownTool : public CTool
{
protected:
	CDownTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	virtual void Initialize();
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point);
	virtual bool OnMouseMove(CDC* pDC, const CPoint& point,int nFlags);
	CPoint GetMousePos() const
	{	return m_ptMousePos;	}
protected:
	CPoint m_ptMousePos;
};

class COneOrTwoPointTool : public CDownTool
{
protected:
	COneOrTwoPointTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point);
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);
	virtual bool OnMouseMove(CDC* pDC, const CPoint& point,int nFlags);
	virtual void OnDraw(CDC* pDC);

protected:
	// 저장하기
	virtual void SavePoints(const CPoints& points);

	virtual bool MouseMove(CDC* pDC, const CPoint& point) = 0;
	virtual void Draw_UpPoint(CDC* pDC) = 0;

	CPoints GetUpPoints() const;
	void SetUpPoints(const CPoints& points);

protected:
	CObList m_objPointList;	
private:	
	CPoint m_ptPoint;
	CPoints m_upPoints;    // 끝점
};

// ----------------------------------------------------------------------------
class COnePointDownTool : public COneOrTwoPointTool
{
protected:
	COnePointDownTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);
	~COnePointDownTool();
public:
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point);
	virtual bool OnMouseMove(CDC* pDC, const CPoint& point,int nFlags);
	virtual void OnDraw(CDC* pDC);

protected:
	virtual bool PointInDrawingRegion(const CPoint& point) const;
	virtual COnePointDrawer& GetOnePointDrawer() = 0;
	virtual CElement* MakeElement(const CElementCommonEnvironment& environment);

	virtual bool MouseMove(CDC* pDC, const CPoint& point);
	virtual void Draw_UpPoint(CDC* pDC);

//attributes
private:	
	bool OnMouseMove_InNotDrawingRegion(const CPoint& point);
	COnePointDrawingData GetOnePointDrawingData() const;
};

class CTwoPointDownTool : public COneOrTwoPointTool
{
protected:
	CTwoPointDownTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point);

protected:
	virtual bool PointInDrawingRegion(const CPoint& point) const;
	virtual CTwoPointDrawer& GetTwoPointDrawer() = 0;
	virtual bool IsMakeElement(const CPoint& point) const;
	virtual CElement* MakeElement(const CElementCommonEnvironment& environment);
	
	virtual bool MouseMove(CDC* pDC, const CPoint& point);
	virtual void Draw_UpPoint(CDC* pDC);

	CPoints GetDownPoints() const;

private:	
	CPoints m_downPoints;  // 시작점 == LButtonDown
	CTwoPointDrawingData GetTwoPointDrawingData() const;
};

// ----------------------------------------------------------------------------
class CThreePointDownTool : public CDownTool
{
protected:
	CThreePointDownTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	virtual void Initialize();
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point);
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);
	virtual void OnDraw(CDC* pDC);

protected:
	// 저장하기
	virtual void SavePoints(const CPoints& points);
	virtual bool IsMakeElement(const CPoint& point) const;
	virtual CElement* MakeElement(const CElementCommonEnvironment& environment);

	virtual CThreePointDrawer& GetThreePointDrawer() = 0;

private:
	int m_nPointCount;
	CPoints m_points1;
	CPoints m_points2;
	CPoints m_points3;

	bool IsEndPoint() const;

	CThreePointDrawingData GetThreePointDrawingData() const;
};

// ----------------------------------------------------------------------------
class CMultiPointDownTool : public CDownTool
{
protected:
	CMultiPointDownTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	virtual void Initialize();
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point);
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);
	virtual bool OnMouseMove(CDC* pDC, const CPoint& point,int nFlags);
	virtual void OnDraw(CDC* pDC);

protected:
	virtual void SavePoints(const CPoints & point);
	virtual bool IsMakeElement(const CPoint& point) const;
	virtual CElement* MakeElement(const CElementCommonEnvironment& environment);

	virtual CMultiPointDrawer& GetMultiPointDrawer() = 0;
	
private:
	int m_nPontsCount;
	CPoints m_ptListPoints[500];

	bool IsEndPoint() const;
	CMultiPointDrawingData GetMultiPointDrawingData() const;
};

#endif // !defined(AFX_POINTTOOL_H__B37BF4F8_89BB_4F93_9106_07CB444E93B0__INCLUDED_)
