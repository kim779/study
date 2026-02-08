// DrawingData.cpp: implementation of the CDrawingData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawingData.h"

#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CDrawingData

CDrawingData& CDrawingData::operator=(const CDrawingData& drawingData)
{
	if(this == &drawingData)
		return *this;

	m_dataType = drawingData.GetDataType();
	m_pTextPosition = drawingData.GetTextPosition();
	m_lineExtEnvironment = drawingData.GetLineExtEnvironment();
	m_displayAttributes = drawingData.GetDisplayAttributes();
	m_vertScaleType = drawingData.GetVertScaleType();
	m_nEquiDivType = drawingData.GetEquiDivType();
	m_nPriceRangeDivType = drawingData.GetPriceRangeDivType();	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.

	// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
	m_hOcxWnd = drawingData.m_hOcxWnd;

	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	m_eChartMode = drawingData.m_eChartMode;

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_eDateUnit		= drawingData.m_eDateUnit;
	// 2011.01.27 by SYS <<

	return *this;
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CDrawingData::CDrawingData(const CDataTypeEnvironment& dataType,
// 						   const int &nEquiDiv, 
// 						   const int &nPriceRangeDiv,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 						   CTextPosition* pTextPosition, 
// 						   const CLineExtEnvironment& lineExtEnvironment, 
// 							const CDisplayAttributes& displayAttributes, 
// 							const CVertScaleType& vertScaleType):
CDrawingData::CDrawingData(const CDataTypeEnvironment& dataType,
						   const int &nEquiDiv, 
						   const int &nPriceRangeDiv,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
						   CTextPosition* pTextPosition, 
						   const CLineExtEnvironment& lineExtEnvironment, 
						   const CDisplayAttributes& displayAttributes, 
						   const CVertScaleType& vertScaleType,
						   const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):

// 2011.01.27 by SYS <<
	m_dataType(dataType),
	m_nEquiDivType(nEquiDiv),
	m_nPriceRangeDivType( nPriceRangeDiv),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_pTextPosition(pTextPosition),
	m_lineExtEnvironment(lineExtEnvironment),
	m_displayAttributes(displayAttributes),
	m_vertScaleType(vertScaleType)
{
	// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
	m_hOcxWnd = dataType.GetOcxHwnd();

	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	m_eChartMode = dataType.GetChartMode();

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_eDateUnit		= p_eDateUnit;
	// 2011.01.27 by SYS <<
}

CDrawingData::CDrawingData(const CDrawingData& drawingData):
	m_dataType(drawingData.GetDataType()),
	m_nEquiDivType(drawingData.GetEquiDivType()),
	m_nPriceRangeDivType( drawingData.GetPriceRangeDivType()),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_pTextPosition(drawingData.GetTextPosition()),
	m_lineExtEnvironment(drawingData.GetLineExtEnvironment()),
	m_displayAttributes(drawingData.GetDisplayAttributes()),
	m_vertScaleType(drawingData.GetVertScaleType())
{
	// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
	m_hOcxWnd = drawingData.m_hOcxWnd;

	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	m_eChartMode = drawingData.m_eChartMode;

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_eDateUnit		= drawingData.m_eDateUnit;
	// 2011.01.27 by SYS <<
}

// public =====================================================================
void CDrawingData::SetTextPosition(CTextPosition* pTextPosition)
{
	m_pTextPosition = pTextPosition;
}

// ----------------------------------------------------------------------------
bool CDrawingData::IsRelativePointType() const
{
	return m_dataType.IsRelativePointType();
}

CDataTypeEnvironment CDrawingData::GetDataType() const
{
	return m_dataType;
}

CTextPosition* CDrawingData::GetTextPosition() const
{
	return m_pTextPosition;
}

CLineExtEnvironment CDrawingData::GetLineExtEnvironment() const
{
	return m_lineExtEnvironment;
}

CDisplayAttributes CDrawingData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CVertScaleType CDrawingData::GetVertScaleType() const
{
	return m_vertScaleType;
}

int CDrawingData::GetEquiDivType() const
{
	return m_nEquiDivType;
}

// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
int CDrawingData::GetPriceRangeDivType() const
{
	return m_nPriceRangeDivType;
}

// ----------------------------------------------------------------------------
CString CDrawingData::GetText_Y(const double& dData, const bool bIsFull) const
{
	// 상대 좌표형 - data 형태
	if(IsRelativePointType())
		return GetText_RelativePoint_Y(dData, bIsFull);
	
	// 절대 좌표형 - pixel 형태
	return GetStringData_Int((int)dData);
}

// ----------------------------------------------------------------------------
COLORREF CDrawingData::GetTextColor() const
{
	if(m_pTextPosition == NULL)
		return RGB(0, 0, 0);

	return m_pTextPosition->GetTextEnvironment().GetColor();
}

// protected ==================================================================
CString CDrawingData::GetStringData_Int(const int nData) const
{
	CString strData;
	strData.Format("%d", nData);
	return strData;
}


// private ====================================================================
// 상대 좌표형 - data 형태
CString CDrawingData::GetText_RelativePoint_Y(const double& dData, const bool bIsFull) const
{
	if(bIsFull)
	{
// 2008.10.15 by LYH >> 8,32bit지원
		CString strVert = m_dataType.GetStrVert();
		if(strVert != "")
			return CDataConversion::GetStringThousandFormat(dData, strVert);
// 2008.10.15 by LYH <<
		return CDataConversion::GetStringThousandFormat(dData, m_dataType.GetVert());
	}

	CString strRealData, strRealDataType;
	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(dData, m_dataType.GetVert(), strRealData, strRealDataType, TRUE);
	return strRealData;
}

///////////////////////////////////////////////////////////////////////////////
// class COnePointDrawingData

COnePointDrawingData& COnePointDrawingData::operator=(const COnePointDrawingData& drawingData)
{
	if(this == &drawingData)
		return *this;

	m_pDrawingRegions = drawingData.GetDrawingRegions();
	m_points = drawingData.GetPoints();
	return *this;
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//COnePointDrawingData::COnePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, 
//										   CLineExtEnvironment* pLineExtEnvironment, 
//										   const CDrawingRegions* pDrawingRegions, const CDisplayAttributes& displayAttributes, 
//										   const CVertScaleType& vertScaleType, const CPoints& points):
//		CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment,displayAttributes, vertScaleType),
COnePointDrawingData::COnePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, 
											CLineExtEnvironment* pLineExtEnvironment, 
											const CDrawingRegions* pDrawingRegions, const CDisplayAttributes& displayAttributes, 
											const CVertScaleType& vertScaleType, const CPoints& points,
											const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):
		CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment,displayAttributes, vertScaleType, p_eDateUnit),
// 2011.01.27 by SYS <<

	m_pDrawingRegions(const_cast<CDrawingRegions*> (pDrawingRegions)),
	m_points(points)
{
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// COnePointDrawingData::COnePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, 
// 										   CLineExtEnvironment* pLineExtEnvironment, 
// 											CDrawingRegions* pDrawingRegions, const CDisplayAttributes& displayAttributes, 
// 											const CVertScaleType& vertScaleType, const CPoints& points):
// 	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType),
COnePointDrawingData::COnePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, 
											CLineExtEnvironment* pLineExtEnvironment, 
											CDrawingRegions* pDrawingRegions, const CDisplayAttributes& displayAttributes, 
											const CVertScaleType& vertScaleType, const CPoints& points,
											const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):
		CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType, p_eDateUnit),
// 2011.01.27 by SYS <<
	m_pDrawingRegions(pDrawingRegions),
	m_points(points)
{
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//COnePointDrawingData::COnePointDrawingData(const COnePointDrawingData& drawingData):
//		CDrawingData(drawingData.GetDataType(),-1,-1,drawingData.GetTextPosition(), drawingData.GetLineExtEnvironment(), 
//					drawingData.GetDisplayAttributes(),drawingData.GetVertScaleType(),
COnePointDrawingData::COnePointDrawingData(const COnePointDrawingData& drawingData):
		CDrawingData(drawingData.GetDataType(),-1,-1,drawingData.GetTextPosition(), drawingData.GetLineExtEnvironment(), 
					drawingData.GetDisplayAttributes(),drawingData.GetVertScaleType(), 
					drawingData.GetDateUnitType()),
// 2011.01.27 by SYS <<

	m_pDrawingRegions(drawingData.GetDrawingRegions()),
	m_points(drawingData.GetPoints())
{
}

// public =====================================================================
bool COnePointDrawingData::IsDrawing() const
{
	return (m_pDrawingRegions != NULL && m_pDrawingRegions->GetCount() > 0 && !GetCurrentRegion().IsRectEmpty());
}

CDrawingRegions* COnePointDrawingData::GetDrawingRegions() const
{
	return m_pDrawingRegions;
}

CPoints COnePointDrawingData::GetPoints() const
{
	return m_points;
}

CCoordinate COnePointDrawingData::GetSavingPoint() const
{
	return m_points.GetSavingPoint();
}

CPoint COnePointDrawingData::GetDrawingPoint() const
{
	return m_points.GetDrawingPoint();
}

// ----------------------------------------------------------------------------
CRect COnePointDrawingData::GetCurrentRegion() const
{
	if(m_pDrawingRegions == NULL)
		return CRect(0, 0, 0, 0);

	return m_pDrawingRegions->GetCurrentFullGraphRegion();
}

CPoint COnePointDrawingData::GetFirstRegion_TopRight() const
{
	if(m_pDrawingRegions == NULL)
		return CPoint(0, 0);

	CRect region = m_pDrawingRegions->GetFullGraphRegion(0);
	return CPoint(region.right, region.top);
}

CPoint COnePointDrawingData::GetLastRegion_BottomRight() const
{
	if(m_pDrawingRegions == NULL)
		return CPoint(0, 0);

	CRect region = m_pDrawingRegions->GetFullGraphRegion(m_pDrawingRegions->GetCount()-1);
	return CPoint(region.right, region.bottom);
}

// ----------------------------------------------------------------------------
CBlockRegion COnePointDrawingData::GetCurrentBlockRegion() const
{
	if(m_pDrawingRegions == NULL)
		return CBlockRegion();

	return m_pDrawingRegions->GetBlockRegionList().GetCurrentAt();
}

CBlockRegion COnePointDrawingData::GetHeadBlockRegion() const
{
	if(m_pDrawingRegions == NULL)
		return CBlockRegion();

	return m_pDrawingRegions->GetBlockRegionList().GetHead();
}

CBlockRegion COnePointDrawingData::GetTailBlockRegion() const
{
	if(m_pDrawingRegions == NULL)
		return CBlockRegion();

	return m_pDrawingRegions->GetBlockRegionList().GetTail();
}

// ----------------------------------------------------------------------------
CString COnePointDrawingData::GetText_X() const
{
	// 상대 좌표형 - data 형태
	if(CDrawingData::IsRelativePointType())
		return GetText_RelativePoint_X();
	
	// 절대 좌표형 - pixel 형태
	return CDrawingData::GetStringData_Int(m_points.GetSavingPoint().GetX());
}

CString COnePointDrawingData::GetText_Y() const
{
	return CDrawingData::GetText_Y(m_points.GetSavingPoint().GetY());
}

// private ====================================================================
CString COnePointDrawingData::GetText_RelativePoint_X() const
{
	//
	double dData = m_points.GetSavingPoint().GetX();
	CDataTypeEnvironment dataType = CDrawingData::GetDataType();
	// <자료일자> 형
	if(dataType.IsDateType())
	{
		// 2011.01.25 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//return CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDrawingData::GetStringData_Int((int)dData), 
		//	dataType.GetHorz(), dataType.GetDateCompart(), dataType.GetTimeCompart());
		CString strType	= dataType.GetHorz();
		
		CString strFormat = _T("YYYYMMDD");
		if (strType == _T("YYYYMMDDHHMMSS"))
		{
			//CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = dataType.GetDateUnitType();
			CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = CDrawingData::GetDateUnitType();

			switch(eDateUnit)
			{
			case CScaleBaseData::HORZ_DAILY:		// 일
			case CScaleBaseData::HORZ_WEEKLEY:		// 주 
				strFormat = _T("YYYYMMDD");		
				break;
			case CScaleBaseData::HORZ_MONTHLY:		// 월 
				strFormat = _T("YYYYMM");		
				break;
			case CScaleBaseData::HORZ_TIME_ONE:		// 분
			case CScaleBaseData::HORZ_TIME_TWO:
				strFormat = _T("MMDDHHMM");
				break;
			case CScaleBaseData::HORZ_TICK:			// 틱 
			case CScaleBaseData::HORZ_TIME_SECOND:
				strFormat = _T("DDHHMMSS");
				break;
			}
		}

		CString strData			= CDataConversion::DoubleToString(dData);
		CString strDateCompart	= dataType.GetDateCompart();
		CString strTimeCompart	= dataType.GetTimeCompart();

		if (strType == _T("YYYYMMDDHHMMSS"))
			return CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strData, strFormat, strDateCompart, strTimeCompart);
		
		return CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strData, strType, strDateCompart, strTimeCompart);
		// 2011.01.25 by SYS <<
	}

	// <Data> 형
	return CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dData), dataType.GetHorz());
}

///////////////////////////////////////////////////////////////////////////////
// class CTwoPointDrawingData

CTwoPointDrawingData& CTwoPointDrawingData::operator=(const CTwoPointDrawingData& drawingData)
{
	if(this == &drawingData)
		return *this;

	m_graphPartRegion = drawingData.GetGraphPartRegion();
	m_startPoints = drawingData.GetStartPoints();
	m_endPoints = drawingData.GetEndPoints();

	return *this;
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)

// CTwoPointDrawingData::CTwoPointDrawingData(const CDataTypeEnvironment& dataType,
// 										   const int &nEquiDiv,
// 										   const int &nPriceRangeDiv,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 										   CTextPosition* pTextPosition, 
// 										   const CLineExtEnvironment& lineExtEnvironment, 
// 											const CGraphPartRegion& graphPartRegion, 
// 											const CDisplayAttributes& displayAttributes, 
// 											const CVertScaleType& vertScaleType):
// 	CDrawingData( dataType, nEquiDiv, nPriceRangeDiv, pTextPosition, lineExtEnvironment,displayAttributes, vertScaleType),
CTwoPointDrawingData::CTwoPointDrawingData(const CDataTypeEnvironment& dataType,
										   const int &nEquiDiv,
										   const int &nPriceRangeDiv,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
										   CTextPosition* pTextPosition, 
										   const CLineExtEnvironment& lineExtEnvironment, 
										   const CGraphPartRegion& graphPartRegion, 
										   const CDisplayAttributes& displayAttributes, 
										   const CVertScaleType& vertScaleType,
										   const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):
	CDrawingData( dataType, nEquiDiv, nPriceRangeDiv, pTextPosition, lineExtEnvironment,displayAttributes, vertScaleType, p_eDateUnit),
// 2011.01.27 by SYS <<

	m_graphPartRegion(graphPartRegion)
{
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CTwoPointDrawingData::CTwoPointDrawingData(const CDataTypeEnvironment& dataType,
// 										   const int &nEquiDiv,
// 										   const int &nPriceRangeDiv,		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 										   CTextPosition* pTextPosition, 
// 										   const CLineExtEnvironment& lineExtEnvironment, 
// 											const CGraphPartRegion& graphPartRegion, 
// 											const CDisplayAttributes& displayAttributes, 
// 											const CVertScaleType& vertScaleType, 
// 											const CPoints& startPoints,
// 											const CPoints& endPoints):
// 	CDrawingData(dataType, nEquiDiv, nPriceRangeDiv, pTextPosition, lineExtEnvironment,displayAttributes, vertScaleType),
CTwoPointDrawingData::CTwoPointDrawingData(const CDataTypeEnvironment& dataType,
											const int &nEquiDiv,
											const int &nPriceRangeDiv,		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
											CTextPosition* pTextPosition, 
											const CLineExtEnvironment& lineExtEnvironment, 
											const CGraphPartRegion& graphPartRegion, 
											const CDisplayAttributes& displayAttributes, 
											const CVertScaleType& vertScaleType, 
											const CPoints& startPoints,
											const CPoints& endPoints,
											const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):
	CDrawingData(dataType, nEquiDiv, nPriceRangeDiv, pTextPosition, lineExtEnvironment,displayAttributes, vertScaleType, p_eDateUnit),
// 2011.01.27 by SYS <<

	m_graphPartRegion(graphPartRegion),
	m_startPoints(startPoints),
	m_endPoints(endPoints)
{
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CTwoPointDrawingData::CTwoPointDrawingData(const CTwoPointDrawingData& drawingData):
// 	CDrawingData(drawingData.GetDataType(),drawingData.GetEquiDivType(),drawingData.GetPriceRangeDivType(),drawingData.GetTextPosition(),drawingData.GetLineExtEnvironment(),
// 		drawingData.GetDisplayAttributes(),	drawingData.GetVertScaleType()),
CTwoPointDrawingData::CTwoPointDrawingData(const CTwoPointDrawingData& drawingData):
	CDrawingData(drawingData.GetDataType(),drawingData.GetEquiDivType(),drawingData.GetPriceRangeDivType(),drawingData.GetTextPosition(),drawingData.GetLineExtEnvironment(), 
		 drawingData.GetDisplayAttributes(),	drawingData.GetVertScaleType(), drawingData.GetDateUnitType()),
// 2011.01.27 by SYS <<

	m_graphPartRegion(drawingData.GetGraphPartRegion()),
	m_startPoints(drawingData.GetStartPoints()),
	m_endPoints(drawingData.GetEndPoints())
{
}

// public =====================================================================
void CTwoPointDrawingData::SetStartPoints(const CPoints& points)
{
	m_startPoints = points;
}

void CTwoPointDrawingData::SetEndPoints(const CPoints& points)
{
	m_endPoints = points;
}

// ----------------------------------------------------------------------------
bool CTwoPointDrawingData::IsDrawing() const
{
	return (m_graphPartRegion.IsDrawing() && m_startPoints.GetDrawingPoint() != m_endPoints.GetDrawingPoint());
}

CGraphPartRegion CTwoPointDrawingData::GetGraphPartRegion() const
{
	return m_graphPartRegion;
}

CRect CTwoPointDrawingData::GetDiaplayRegion() const
{
	return m_graphPartRegion.GetFull();
}

CPoints CTwoPointDrawingData::GetStartPoints() const
{
	return m_startPoints;
}

CPoints CTwoPointDrawingData::GetEndPoints() const
{
	return m_endPoints;
}

void CTwoPointDrawingData::SetStartYPoints_Pos(const int& nStartYPos)
{
	m_startPoints.SetPoints_Point_Y(nStartYPos);
}

void CTwoPointDrawingData::SetEndYPoints_Pos(const int& nEndYPos)
{
	m_endPoints.SetPoints_Point_Y(nEndYPos);
}

void CTwoPointDrawingData::SetStartYPoints_Val(const double& dStartYVal)
{
	m_startPoints.SetPoints_Value_Y(dStartYVal);
}

void CTwoPointDrawingData::SetEndYPoints_Val(const double& dEndYVal)
{
	m_endPoints.SetPoints_Value_Y(dEndYVal);
}

// ----------------------------------------------------------------------------
CPoints CTwoPointDrawingData::GetLeftPoints() const
{
	if(m_startPoints.GetDrawingPoint().x < m_endPoints.GetDrawingPoint().x)
		return m_startPoints;

	return m_endPoints;
}

CPoints CTwoPointDrawingData::GetRightPoints() const
{
	if(m_startPoints.GetDrawingPoint().x < m_endPoints.GetDrawingPoint().x)
		return m_endPoints;

	return m_startPoints;
}

CDisplayData CTwoPointDrawingData::GetDisplayData() const
{
	return CDisplayData(CDrawingData::GetDataType().GetCoordinateType(), 
		m_graphPartRegion, CDrawingData::GetDisplayAttributes());
}

///////////////////////////////////////////////////////////////////////////////
// class CThreePointDrawingData

CThreePointDrawingData& CThreePointDrawingData::operator=(const CThreePointDrawingData& drawingData)
{
	if(this == &drawingData)
		return *this;

	m_rect = drawingData.GetRect();
	m_nPointsCount = drawingData.GetPointsCount();
	m_points1 = drawingData.GetPoints1();
	m_points2 = drawingData.GetPoints2();
	m_points3 = drawingData.GetPoints3();
	return *this;
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CThreePointDrawingData::CThreePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition,
// 											   CLineExtEnvironment* pLineExtEnvironment, const CRect& rect,
// 											   const CDisplayAttributes& displayAttributes, 
// 											   const CVertScaleType& vertScaleType):
// 	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType),
CThreePointDrawingData::CThreePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition,
											   CLineExtEnvironment* pLineExtEnvironment, const CRect& rect,
											   const CDisplayAttributes& displayAttributes, 
											   const CVertScaleType& vertScaleType,
											   const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):
	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType, p_eDateUnit),
// 2011.01.27 by SYS <<
	m_rect(rect),
	m_nPointsCount(0)
{
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CThreePointDrawingData::CThreePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, 
// 											   CLineExtEnvironment* pLineExtEnvironment, 
// 		const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
// 		const int nPointCount, const CPoints& points1, const CPoints& points2, const CPoints& points3):
// 	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType),
CThreePointDrawingData::CThreePointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, 
												CLineExtEnvironment* pLineExtEnvironment, 
												const CRect& rect, const CDisplayAttributes& displayAttributes, const CVertScaleType& vertScaleType, 
												const int nPointCount, const CPoints& points1, const CPoints& points2, const CPoints& points3,
												const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):
	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType, p_eDateUnit),
// 2011.01.27 by SYS <<

	m_rect(rect),
	m_nPointsCount(nPointCount),
	m_points1(points1),
	m_points2(points2),
	m_points3(points3)
{
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CThreePointDrawingData::CThreePointDrawingData(const CThreePointDrawingData& drawingData):
// 	CDrawingData(drawingData.GetDataType(),-1,-1,drawingData.GetTextPosition(), drawingData.GetLineExtEnvironment(),
// 		drawingData.GetDisplayAttributes(),drawingData.GetVertScaleType()),
CThreePointDrawingData::CThreePointDrawingData(const CThreePointDrawingData& drawingData):
	CDrawingData(drawingData.GetDataType(),-1,-1,drawingData.GetTextPosition(), drawingData.GetLineExtEnvironment(),
				 drawingData.GetDisplayAttributes(),drawingData.GetVertScaleType(), drawingData.GetDateUnitType()),
// 2011.01.27 by SYS <<
	m_rect(drawingData.GetRect()),
	m_nPointsCount(drawingData.GetPointsCount()),
	m_points1(drawingData.GetPoints1()),
	m_points2(drawingData.GetPoints2()),
	m_points3(drawingData.GetPoints3())
{
}

// public =====================================================================
void CThreePointDrawingData::SetPointsCount(const int nPointsCount)
{
	m_nPointsCount = nPointsCount;
}

void CThreePointDrawingData::SetPoints1(const CPoints& points)
{
	m_points1 = points;
}

void CThreePointDrawingData::SetPoints2(const CPoints& points)
{
	m_points2 = points;
}

void CThreePointDrawingData::SetPoints3(const CPoints& points)
{
	m_points3 = points;
}

// ----------------------------------------------------------------------------
bool CThreePointDrawingData::IsDrawing() const
{
	return (!m_rect.IsRectEmpty());
}

CRect CThreePointDrawingData::GetRect() const
{
	return m_rect;
}

int CThreePointDrawingData::GetPointsCount() const
{
	return m_nPointsCount;
}

CPoints CThreePointDrawingData::GetPoints1() const
{
	return m_points1;
}

CPoints CThreePointDrawingData::GetPoints2() const
{
	return m_points2;
}

CPoints CThreePointDrawingData::GetPoints3() const
{
	return m_points3;
}

///////////////////////////////////////////////////////////////////////////////
// class CMultiPointDrawingData

CMultiPointDrawingData& CMultiPointDrawingData::operator=(const CMultiPointDrawingData& drawingData)
{
	if(this == &drawingData)
		return *this;

	m_rect = drawingData.GetRect();
	return *this;
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CMultiPointDrawingData::CMultiPointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition,
// 											   CLineExtEnvironment* pLineExtEnvironment, const CRect& rect,
// 											   const CDisplayAttributes& displayAttributes, 
// 											   const CVertScaleType& vertScaleType):
// 	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType),
CMultiPointDrawingData::CMultiPointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition,
											   CLineExtEnvironment* pLineExtEnvironment, const CRect& rect,
											   const CDisplayAttributes& displayAttributes, 
											   const CVertScaleType& vertScaleType,
											   const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):
	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType, p_eDateUnit),
// 2011.01.27 by SYS <<

	m_rect(rect)
{
	m_nPointsCount = 0;
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CMultiPointDrawingData::CMultiPointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, 
// 											   CLineExtEnvironment* pLineExtEnvironment, 
// 		const CRect& rect, const CDisplayAttributes& displayAttributes, 
// 		const CVertScaleType& vertScaleType, 
// 		const int nPointCount,const CPoints* plistPoint):
// 	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType),
CMultiPointDrawingData::CMultiPointDrawingData(const CDataTypeEnvironment& dataType, CTextPosition* pTextPosition, 
												CLineExtEnvironment* pLineExtEnvironment, 
												const CRect& rect, const CDisplayAttributes& displayAttributes, 
												const CVertScaleType& vertScaleType, 
												const int nPointCount,const CPoints* plistPoint,
												const CScaleBaseData::HORZSCALEDRAWERTYPE p_eDateUnit):
	CDrawingData(dataType,-1,-1,pTextPosition,pLineExtEnvironment, displayAttributes, vertScaleType, p_eDateUnit),
// 2011.01.27 by SYS <<
	m_rect(rect)	
{
	m_nPointsCount = nPointCount;
	for(int i=0;i<nPointCount;i++)
	{
		m_ptListPoints[i] = plistPoint[i];
	}
}

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CMultiPointDrawingData::CMultiPointDrawingData(const CMultiPointDrawingData& drawingData):
// 	CDrawingData(drawingData.GetDataType(),-1,-1,drawingData.GetTextPosition(), drawingData.GetLineExtEnvironment(),
// 		drawingData.GetDisplayAttributes(),drawingData.GetVertScaleType()),
CMultiPointDrawingData::CMultiPointDrawingData(const CMultiPointDrawingData& drawingData):
	CDrawingData(drawingData.GetDataType(),-1,-1,drawingData.GetTextPosition(), drawingData.GetLineExtEnvironment(),
				 drawingData.GetDisplayAttributes(),drawingData.GetVertScaleType(), drawingData.GetDateUnitType()),
// 2011.01.27 by SYS <<

	m_rect(drawingData.GetRect())	
{
	m_nPointsCount = drawingData.GetPointsCount();
	for(int i=0;i<drawingData.GetPointsCount();i++)
	{
		m_ptListPoints[i] = drawingData.GetPoints(i);
	}
}

// public =====================================================================

// ----------------------------------------------------------------------------
bool CMultiPointDrawingData::IsDrawing() const
{
	return (!m_rect.IsRectEmpty());
}

CRect CMultiPointDrawingData::GetRect() const
{
	return m_rect;
}

int CMultiPointDrawingData::GetPointsCount() const
{
	return m_nPointsCount;
}

CPoints CMultiPointDrawingData::GetPoints(int nIndex) const
{	
	return m_ptListPoints[nIndex];
}

