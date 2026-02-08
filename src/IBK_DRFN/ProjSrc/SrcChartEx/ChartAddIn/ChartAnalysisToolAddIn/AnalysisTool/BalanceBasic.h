#pragma once
#include "pointelement.h"
#include "BalanceBasicDrawer.h"

class CBalanceBasic : public CVertElement
{
public:
	CBalanceBasic(const CCoordinate& point, const CElementCommonEnvironment& environment);
	CBalanceBasic(const CTextShowPosition& position, const CCoordinate& point, const CElementCommonEnvironment& environment);// 2008.08.20 김진순 피보나치 확장

	virtual CToolEnumData::ElementType GetElementType() const;
	virtual void ElementModify_YValue(const CElementModifyData& elementModifyData);
protected:
	virtual const CTextPosition* GetTextPosition() const;	// 2008.08.20 김진순 피보나치 확장
	virtual CTextPosition* GetTextPosition();

	virtual COnePointDrawer& GetOnePointDrawer();
	virtual CToolEnumData::ModifyType GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const;
private:
	CDisplayData GetDisplayData(const CCoordinateCalculatorData& calData) const;

private:
	CBalanceBasicDrawer m_BalanceBasicTimeDrawer;
	CTextShowPosition m_textPosition;	
};

