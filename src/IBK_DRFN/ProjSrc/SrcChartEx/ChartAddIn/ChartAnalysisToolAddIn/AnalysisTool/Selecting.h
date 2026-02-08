// Selecting.h: interface for the CSelecting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTING_H__5B9E3409_01BC_4C83_8883_465FC70136DA__INCLUDED_)
#define AFX_SELECTING_H__5B9E3409_01BC_4C83_8883_465FC70136DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"

class CDrawingRegions;
class CCoordinate;

// element 선택 tool
class CSelectingTool : public CTool
{
public:
	CSelectingTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	virtual void Initialize();
	virtual CElement* OnLButtonDown(CDC* pDC, const CPoint& point);
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);
	virtual bool OnMouseMove(CDC* pDC, const CPoint& point,int nFlags);
	virtual void OnDraw(CDC* pDC);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	// 그리는 영역들에 속한 point 인지의 여부.
	//virtual bool PointInDrawingRegion(const CPoint& point) const;
	// 저장하기
	virtual void SavePoints(const CPoints& points);

public:
	CPoint GetDrawingPoint(const CPoint& point) const;
	CElementModifyData GetElementModifyData(const CPoint& point) const;
	void	SetSelectToModify( CElement* p_pSelectedElement, CToolEnumData::ModifyType p_TypeModify)
			{	m_pSelectedElement = p_pSelectedElement;	m_TypeModify = p_TypeModify;	}
//>> (2008/12/16 - Seung-Lyong Park)
	CSelectedElementDrawingData GetSelectedElementDrawData();
	void	SetBtDownPoint(const CPoint& point);
//<< (2008/12/16 - Seung-Lyong Park)

private:
	CToolEnumData::ModifyType m_TypeModify;
	CElement* m_pSelectedElement;
	bool m_bOldSelectedPtUsed;
	CPoints m_oldSelectedPt;
	CPoint m_lBtDownPoint;

	// 현재 선택된 element가 있는지의 여부
	bool IsSelectedElement() const;
	// 수정할 element를 찾은 경우 
	bool IsFindedModifyElement(CElement* pElement, const CToolEnumData::ModifyType modifyType) const;
	// element 가 data 형태 여부
	bool IsRelativePointType() const;
	// 좌표
	CCoordinate GetSavingPoint(const CPoint& point) const;
	CPoint GetSelectedElementDrawPoint(const CCoordinateCalculatorData& calData, const CPoint& point) const;
	// 선택된 element를 그리기 위해 필요한 data
	CSelectedElementDrawingData GetSelectedElementDrawData(const CPoints& points);
	// element를 modify 할 때 필요한 data
	void GetElementModifyData_Data(const CPoint& point, CElementModifyData& elementModifyData) const;
	void GetElementModifyData_Pixel(const CPoint& point, CElementModifyData& elementModifyData) const;

	// element를 찾을지의 여부.
	bool IsFindElement();
	// element 찾기
	bool FindElement(const CPoint& point);
	CElement* FindElement(const CPoint& point, CToolEnumData::ModifyType& modifyType);

	// element 수정
	bool ModifyElement(CDC* pDC, const CPoint& point);
	bool Modify_InDrawingRegion(CDC* pDC, const CPoint& point);
	void DrawNewToolDrawing(CDC* pDC, const CPoints& points);
	void Draw_EndPoint(CDC* pDC);

};

#endif // !defined(AFX_SELECTING_H__5B9E3409_01BC_4C83_8883_465FC70136DA__INCLUDED_)
