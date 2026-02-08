#pragma once
#include "pointtool.h"
#include "PointDrawer.h"
#include "BalanceBasicDrawer.h"

class CBalanceBasicTool :public COnePointMoveTool
{
public:
	CBalanceBasicTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	virtual void Initialize();

	void SetTextPosition(const CTextShowPosition& position);
	void SetTextPosition(const bool bShow);
public:
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);
protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual COnePointDrawer& GetOnePointDrawer();
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();

private:
	CTextShowPosition m_textPosition;
	CBalanceBasicDrawer m_balanceBasicDrawer;
};

