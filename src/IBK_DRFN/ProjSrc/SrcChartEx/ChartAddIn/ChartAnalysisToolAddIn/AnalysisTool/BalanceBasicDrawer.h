#pragma once
#include "pointdrawer.h"
#include "AnalysisGlobalDefine.h"

//그리기
class CBalanceBasicDrawer : public CVertLineDrawer
{
public:
	// 좌표를 구하기.
	bool GetPoint( const bool p_bIsRelativePointType, const CRegion &region, const CPoint& startPoint, const int nIndex, CPoint& gbLinePoint) const;

protected:
	virtual bool IsDrawing(const CRect& region, const CPoint& point) const;
	virtual void DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion);
	virtual void DrawText(CDC* pDC, const COnePointDrawingData& drawingData);
	void DrawText( CDC* pDC, const COnePointDrawingData& drawingData, CString strIndex, int nIndex);

private:
	// 그리기.
	void DrawFigure_Data(CDC* pDC, const CDisplayData& displayData, const CPoint& point, BOOL bLeftExt, BOOL bRightExt);
	// 좌표를 구하기.

	// 피보나치 계수를 가져오기.
	int GetCoefficient(const int nIndex) const;

public:
	CDWordArray		m_arrCoefficient;
	CString			m_strCoefficient;
};

