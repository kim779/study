// Tool.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOL_H__803BA62C_2B4E_4650_B986_F9B182091888__INCLUDED_)
#define AFX_TOOL_H__803BA62C_2B4E_4650_B986_F9B182091888__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/ToolBaseData.h"
#include "AnalysisData.h"

class CElement;
class CCoordinateCalculator;

class CTool  
{
protected:
	CTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	void SetAnalysisData(CAnalysisData* pAnalysisData);
	void SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType);
	void SetProcessStarting(const bool bStarting);
	void SetAutoPrice(const bool bIsAutoPrice);
	void SetPenColor(const COLORREF& color);
	void SetTextColor(const COLORREF& color);
	void SetLineExt(const int& nLeft,const int& nRight);

	bool DoesProcessStarting() const;
	bool DoesElements() const;
	CElementCommonEnvironment& GetElementCommonEnvironment();

	void Invalidate();

	virtual void Initialize();
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point) = 0;
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);
	virtual bool OnMouseMove(CDC* pDC, const CPoint& point,int nFlags) = 0;
	virtual void OnRButtonUp(CDC* pDC, const CPoint& point);
	virtual void OnDraw(CDC* pDC) = 0;

	// 좌표 계산을 위한 data.
	CCoordinateCalculatorData GetCoordinateCalculatorData(const CPoint& point) const;
	virtual CCoordinateCalculator& GetCoordinateCalculator() const;

protected:
	virtual bool PointInDrawingRegion(const CPoint& point) const;
	// 저장하기
	virtual void SavePoints(const CPoints& points) = 0;
	virtual CAnalysis::TOOLTYPE GetToolType() const = 0;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual bool IsMakeElement(const CPoint& point) const;
	virtual CElement* MakeElement(const CElementCommonEnvironment& environment);

	bool IsRelativePointType() const;
	bool IsAutoPrice() const;
	virtual CAnalysisData* GetAnalysisData() const;
	const CDrawingRegions& GetDrawingRegions() const;
	CDrawingRegions& GetDrawingRegions();
	CElementCommonEnvironment GetElementCommonEnvironment() const;

	CDisplayAttributes GetDisplayAttributes() const;
	CVertScaleType GetVertScaleType(const CPoint& point) const;

	// 좌표.
	CCoordinate GetPosCoordinate(const CPoint& point) const;
	CPoints GetPoints(const CPoint& point) const;
	CPoints GetPointsAndSetDataType(const CPoint& point);
	CPoint GetDrawingPointAndSetDataType(const CPoint& point);
	CPoints RefreshPointsAndSetDataType(const CPoint& point);

	void SetDrawingRegions(const CPoint& point);
	void ChangeCursor(const CToolEnumData::ModifyType type);
	void ChangeCursor(const CToolEnumData::DrawType type);

	void MakeElement(const CPoint& drawingPoint);	

private:
	CAnalysisData* m_pAnalysisData;
	bool m_bProcessStarting; // 그리기 시작을 나타낸다.
	CDrawingRegions m_drawingRegions; // 그리기에 필요한 영역들.

	CElementCommonEnvironment m_commonEnvironment;

	bool IsNullMainBlock() const;
	CPoints GetPoints(const CPoint& point, CCoordinateCalculatorData& calData) const;
	CPoint GetDrawingPoint(const CPoint& point, CCoordinateCalculatorData& calData) const;
	void SetDataTypeEnvironment(const CCoordinateCalculatorData& calData);

protected:
	HWND m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

#endif // !defined(AFX_TOOL_H__803BA62C_2B4E_4650_B986_F9B182091888__INCLUDED_)
