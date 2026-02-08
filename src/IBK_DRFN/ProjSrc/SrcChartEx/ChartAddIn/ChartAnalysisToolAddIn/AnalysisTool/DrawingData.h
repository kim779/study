// DrawingData.h: interface for the CDrawingData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGDATA_H__D985A43E_9613_4A27_9508_E07694328DCB__INCLUDED_)
#define AFX_DRAWINGDATA_H__D985A43E_9613_4A27_9508_E07694328DCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnalysisData.h"

class CTextPosition;
class CLineExtEnvironment;

class CDrawingData  
{
public:
	CDrawingData& operator=(const CDrawingData& drawingData);

public:

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//CDrawingData(const CDataTypeEnvironment& dataType, const int &pEquiDiv, const int &pPriceRangeDiv, CTextPosition* pTextPosition, 
	//			const CLineExtEnvironment& lineExtEnvironment, 
	//			const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType);
	CDrawingData(const CDataTypeEnvironment& dataType, const int &pEquiDiv, const int &pPriceRangeDiv, CTextPosition* pTextPosition, 
		const CLineExtEnvironment& lineExtEnvironment, 
		const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	// 2011.01.27 by SYS <<
	CDrawingData(const CDrawingData& drawingData);

	void SetTextPosition(CTextPosition* pTextPosition);
	void SetLineExtEnv(CLineExtEnvironment* pLineExtEnv);

	bool IsRelativePointType() const;
	CDataTypeEnvironment GetDataType() const;
	CTextPosition* GetTextPosition() const;
	CLineExtEnvironment GetLineExtEnvironment() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CVertScaleType GetVertScaleType() const;
	int GetEquiDivType() const;
	int GetPriceRangeDivType() const;		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.

	CString GetText_Y(const double& dData, const bool bIsFull = true) const;
	COLORREF GetTextColor() const;

protected:
	CString GetStringData_Int(const int nData) const;

private:
	int m_nEquiDivType;
	int m_nPriceRangeDivType;		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	CDataTypeEnvironment m_dataType;
	CTextPosition* m_pTextPosition;
	CLineExtEnvironment m_lineExtEnvironment;
	CDisplayAttributes m_displayAttributes;
	CVertScaleType m_vertScaleType; // 세로 scale type (log, invert)

	CString GetText_RelativePoint_Y(const double& dData, const bool bIsFull) const;

// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CScaleBaseData::HORZSCALEDRAWERTYPE m_eDateUnit;
	CScaleBaseData::HORZSCALEDRAWERTYPE GetDateUnitType(void) const
	{	
		return m_eDateUnit;
	}
	//void								SetDateUnitType(CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit);
	// 2011.01.27 by SYS <<
};

class COnePointDrawingData : public CDrawingData
{
public:
	COnePointDrawingData& operator=(const COnePointDrawingData& drawingData);

public:
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	COnePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
// 		const CDrawingRegions* pDrawingRegions, const CDisplayAttributes& displayAttributes, 
// 		const CVertScaleType& vertScaleType, const CPoints& points);
// 	COnePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
// 		CDrawingRegions* pDrawingRegions, const CDisplayAttributes& displayAttributes, 
// 		const CVertScaleType& vertScaleType, const CPoints& points);
	COnePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
		const CDrawingRegions* pDrawingRegions, const CDisplayAttributes& displayAttributes, 
		const CVertScaleType& vertScaleType, const CPoints& points,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	COnePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
		CDrawingRegions* pDrawingRegions, const CDisplayAttributes& displayAttributes, 
		const CVertScaleType& vertScaleType, const CPoints& points,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	// 2011.01.27 by SYS <<
	COnePointDrawingData(const COnePointDrawingData& drawingData);

	bool IsDrawing() const;
	CDrawingRegions* GetDrawingRegions() const;
	CPoints GetPoints() const;
	CCoordinate GetSavingPoint() const;
	CPoint GetDrawingPoint() const;

	CRect GetCurrentRegion() const;
	CPoint GetFirstRegion_TopRight() const;
	CPoint GetLastRegion_BottomRight() const;

	CBlockRegion GetCurrentBlockRegion() const;
	CBlockRegion GetHeadBlockRegion() const;
	CBlockRegion GetTailBlockRegion() const;

	CString GetText_X() const;
	CString GetText_Y() const;

private:
	CDrawingRegions* m_pDrawingRegions;
	CPoints m_points;

	CString GetText_RelativePoint_X() const;
};

class CTwoPointDrawingData : public CDrawingData
{
public:
	CTwoPointDrawingData& operator=(const CTwoPointDrawingData& drawingData);

public:
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	CTwoPointDrawingData(const CDataTypeEnvironment& dataType,const int& nEquiDiv,const int& nPriceRangeDiv,CTextPosition* pTextPosition, const CLineExtEnvironment& lineExtEnvironment, 
// 		const CGraphPartRegion& graphPartRegion, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType);
// 	CTwoPointDrawingData(const CDataTypeEnvironment& dataType,const int& nEquiDiv,const int& nPriceRangeDiv,CTextPosition* pTextPosition, const CLineExtEnvironment& lineExtEnvironment, 
// 		const CGraphPartRegion& graphPartRegion, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
// 		const CPoints& startPoints, const CPoints& endPoints);
// 	CTwoPointDrawingData(const CDataTypeEnvironment& dataType,const CLineExtEnvironment& lineExtEnvironment, 
// 		const CGraphPartRegion& graphPartRegion, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
// 		const CPoints* pPointsList, const int& nCount);
	CTwoPointDrawingData(const CDataTypeEnvironment& dataType,const int& nEquiDiv,const int& nPriceRangeDiv,CTextPosition* pTextPosition, const CLineExtEnvironment& lineExtEnvironment, 
		const CGraphPartRegion& graphPartRegion, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	CTwoPointDrawingData(const CDataTypeEnvironment& dataType,const int& nEquiDiv,const int& nPriceRangeDiv,CTextPosition* pTextPosition, const CLineExtEnvironment& lineExtEnvironment, 
		const CGraphPartRegion& graphPartRegion, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
		const CPoints& startPoints, const CPoints& endPoints,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	CTwoPointDrawingData(const CDataTypeEnvironment& dataType,const CLineExtEnvironment& lineExtEnvironment, 
		const CGraphPartRegion& graphPartRegion, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
		const CPoints* pPointsList, const int& nCount,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	// 2011.01.27 by SYS <<
	CTwoPointDrawingData(const CTwoPointDrawingData& drawingData);

	void SetStartPoints(const CPoints& points);
	void SetEndPoints(const CPoints& points);

	bool IsDrawing() const;
	CGraphPartRegion GetGraphPartRegion() const;
	CRect GetDiaplayRegion() const;
	CPoints GetStartPoints() const;
	CPoints GetEndPoints() const;
	void  SetStartYPoints_Pos(const int &nStartYPos);
	void  SetEndYPoints_Pos(const int &nEndYPos);
	void SetStartYPoints_Val(const double& dStartYVal);
	void SetEndYPoints_Val(const double& dEndYVal);

	CPoints GetLeftPoints() const;
	CPoints GetRightPoints() const;
	CDisplayData GetDisplayData() const;

private:
	CGraphPartRegion m_graphPartRegion;
	CPoints m_startPoints;
	CPoints m_endPoints;
};

class CThreePointDrawingData : public CDrawingData
{
public:
	CThreePointDrawingData& operator=(const CThreePointDrawingData& drawingData);

public:

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	CThreePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
// 		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType);
// 	CThreePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
// 		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
// 		const int nPointCount, const CPoints& points1, const CPoints& points2, const CPoints& point3);
	CThreePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	CThreePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
		const int nPointCount, const CPoints& points1, const CPoints& points2, const CPoints& point3,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	// 2011.01.27 by SYS <<
	CThreePointDrawingData(const CThreePointDrawingData& drawingData);

	void SetPointsCount(const int nPointsCount);
	void SetPoints1(const CPoints& points);
	void SetPoints2(const CPoints& points);
	void SetPoints3(const CPoints& points);

	bool IsDrawing() const;
	CRect GetRect() const;
	int GetPointsCount() const;

	CPoints GetPoints1() const;
	CPoints GetPoints2() const;
	CPoints GetPoints3() const;

private:
	CRect m_rect;
	int m_nPointsCount;
	CPoints m_points1;
	CPoints m_points2;
	CPoints m_points3;
};

class CMultiPointDrawingData : public CDrawingData
{
public:
	CMultiPointDrawingData& operator=(const CMultiPointDrawingData& drawingData);

public:
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	CMultiPointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
// 		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType);
// 	CMultiPointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
// 		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
// 		const int nPointCount,const CPoints* plistPoint);
	CMultiPointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	CMultiPointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, CLineExtEnvironment* pLineExtEnvironment, 
		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
		const int nPointCount,const CPoints* plistPoint,
		const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit);
	// 2011.01.27 by SYS <<
	CMultiPointDrawingData(const CMultiPointDrawingData& drawingData);

	bool IsDrawing() const;
	CRect GetRect() const;
	int GetPointsCount() const;
	CPoints GetPoints(int nIndex) const;

	// (2008/8/15 - Seung-Won, Bae) Support moving of pencel draw object.
	CPoints *GetPoints( void)						{	return m_ptListPoints;	}
	void	SetPointsCount( int p_nPointsCount)		{	m_nPointsCount = p_nPointsCount;	}
private:
	CRect m_rect;
	
	int m_nPointsCount;
	CPoints m_ptListPoints[500];
};

#endif // !defined(AFX_DRAWINGDATA_H__D985A43E_9613_4A27_9508_E07694328DCB__INCLUDED_)
