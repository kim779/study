#include "stdafx.h"
#include "BalanceBasicTool.h"
#include "PointTool.h"

class CAnalysisData;

CBalanceBasicTool::CBalanceBasicTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: COnePointMoveTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)
{
}

void CBalanceBasicTool::Initialize()
{
	CMoveTool::Initialize();
	
	CTool::GetElementCommonEnvironment().SetLineExt(TRUE, TRUE);
}

void CBalanceBasicTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

void CBalanceBasicTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}

void CBalanceBasicTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	COnePointMoveTool::OnLButtonUp(pDC, point);
	CTool::SetProcessStarting(false);
}

CAnalysis::TOOLTYPE CBalanceBasicTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_BALANCE_BASIC;
}

COnePointDrawer& CBalanceBasicTool::GetOnePointDrawer()
{
	return m_balanceBasicDrawer;
}

const CTextPosition* CBalanceBasicTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CBalanceBasicTool::GetTextPosition()
{
	return &m_textPosition;
}
