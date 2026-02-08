#include "stdafx.h"
#include "BalanceBasic.h"


///////////////////////////////////////////////////////////////////////////////
// class CBalanceBasic

CBalanceBasic::CBalanceBasic(const CCoordinate& point, const CElementCommonEnvironment& environment):
	CVertElement(point, environment)
{
	m_textPosition.SetShow(true);
}

CBalanceBasic::CBalanceBasic(const CTextShowPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment):
	CVertElement(point, environment),
	m_textPosition(position)		
{
}

CToolEnumData::ElementType CBalanceBasic::GetElementType() const
{
	return CToolEnumData::Balance_Basic;
}

// protected ==================================================================
const CTextPosition* CBalanceBasic::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CBalanceBasic::GetTextPosition()
{
	return &m_textPosition;
}
//>>

COnePointDrawer& CBalanceBasic::GetOnePointDrawer()
{
	return m_BalanceBasicTimeDrawer;
}

CToolEnumData::ModifyType CBalanceBasic::GetModifyType(const CDisplayData& displayData,
												   const CPoint& elementDrawingPt, const CPoint& point) const
{
	CToolEnumData::ModifyType eResult = CVertElement::GetModifyType( displayData, elementDrawingPt, point);
	if( CToolEnumData::MODIFY_NONE != eResult) return eResult;
	
	CPoint gbLinePoint = elementDrawingPt;
	for( int i = 1; i < _GB_VALUE_COUNT + 1; i++)
	{
		if( m_environment.GetLineExt().GetRightExt())
			if( m_BalanceBasicTimeDrawer.GetPoint( displayData.IsRelativePointType(), CRegion( displayData.GetCoordinateCalRegion(), displayData.GetDisplayAttributes()), elementDrawingPt, i, gbLinePoint))
				if( CVertElement::GetModifyType( displayData, gbLinePoint, point) == CToolEnumData::MODIFY_X)
					return CToolEnumData::MODIFY_CENTER;
				
				if( m_environment.GetLineExt().GetLeftExt())
					if( m_BalanceBasicTimeDrawer.GetPoint( displayData.IsRelativePointType(), CRegion( displayData.GetCoordinateCalRegion(), displayData.GetDisplayAttributes()), elementDrawingPt, -i, gbLinePoint))
						if( CVertElement::GetModifyType( displayData, gbLinePoint, point) == CToolEnumData::MODIFY_X)
							return CToolEnumData::MODIFY_CENTER;
	}
	
	return CToolEnumData::MODIFY_NONE;
}

void CBalanceBasic::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}
// private ====================================================================
CDisplayData CBalanceBasic::GetDisplayData(const CCoordinateCalculatorData& calData) const
{
	return CDisplayData(CElement::GetCommonEnvironment().GetDataType().GetCoordinateType(),
		CGraphPartRegion(calData.GetRegion(), calData.GetRegion()),
		CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()));
}
