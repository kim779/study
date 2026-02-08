// FiboGannTool.cpp: implementation of the CFiboGannTool class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiboGannTool.h"

#include "CoordinateCalculator.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CFiboSemicircleTool -- 피보나치 반원

CFiboSemicircleTool::CFiboSemicircleTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)
{
}

void CFiboSemicircleTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

void CFiboSemicircleTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CFiboSemicircleTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE;
}

CTwoPointDrawer& CFiboSemicircleTool::GetTwoPointDrawer()
{
	return m_fiboSemicircleDrawer;
}

const CTextPosition* CFiboSemicircleTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboSemicircleTool::GetTextPosition()
{
	return &m_textPosition;
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboCircleTool -- 피보나치 원

CFiboCircleTool::CFiboCircleTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)
{
}

void CFiboCircleTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

void CFiboCircleTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CFiboCircleTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_FIBO_CIRCLE;
}

CTwoPointDrawer& CFiboCircleTool::GetTwoPointDrawer()
{
	return m_fiboCircleDrawer;
}

const CTextPosition* CFiboCircleTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboCircleTool::GetTextPosition()
{
	return &m_textPosition;
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboPenLineTool -- 피보나치 펜라인

CFiboPenLineTool::CFiboPenLineTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)	
{
}

void CFiboPenLineTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

void CFiboPenLineTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}
// protected ==================================================================
CAnalysis::TOOLTYPE CFiboPenLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE;
}

CTwoPointDrawer& CFiboPenLineTool::GetTwoPointDrawer()
{
	return m_fiboPenLineDrawer;
}

const CTextPosition* CFiboPenLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboPenLineTool::GetTextPosition()
{
	return &m_textPosition;
}
///////////////////////////////////////////////////////////////////////////////
// class CFiboRetraceTool -- 피보 되돌림(조정대)

CFiboRetraceTool::CFiboRetraceTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)	
{
}

void CFiboRetraceTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

void CFiboRetraceTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}

void CFiboRetraceTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
bool CFiboRetraceTool::IsMakeElement(const CPoint& point) const
{
	if(CTwoPointDownTool::GetDownPoints().GetDrawingPoint().y == point.y)
		return false;

	return CTwoPointDownTool::IsMakeElement(point);
}

CAnalysis::TOOLTYPE CFiboRetraceTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_FIBO_RETRACE;
}

const CTextPosition* CFiboRetraceTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboRetraceTool::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CFiboRetraceTool::GetTwoPointDrawer()
{
	return m_fiboRetraceDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboTimeTool -- 피보 시간대

CFiboTimeTool::CFiboTimeTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: COnePointMoveTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)
{
}

// 2008.08.20 김진순 피보나치 확장
void CFiboTimeTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

// 2008.08.20 김진순 피보나치 확장
void CFiboTimeTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}

// 20080916 김진순  분석도구 그리고 나면 무조건 연속그리기해제 하기 위해 <<
void CFiboTimeTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	COnePointMoveTool::OnLButtonUp(pDC, point);
	CTool::SetProcessStarting(false);
}
//>>

// protected ==================================================================
CAnalysis::TOOLTYPE CFiboTimeTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_FIBO_TIME;
}

COnePointDrawer& CFiboTimeTool::GetOnePointDrawer()
{
	return m_fiboTimeDrawer;
}

// 2008.08.20 김진순 피보나치 확장<<
const CTextPosition* CFiboTimeTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CFiboTimeTool::GetTextPosition()
{
	return &m_textPosition;
}
//>>

///////////////////////////////////////////////////////////////////////////////
// class CGannTool -- 갠

CGannTool::CGannTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointMoveTool(pAnalysisData, eCoordinateType)
{
}

// 20080916 김진순  분석도구 그리고 나면 무조건 연속그리기해제 하기 위해 <<
void CGannTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	CTwoPointMoveTool::OnLButtonUp(pDC, point);
	CTool::SetProcessStarting(false);
}
//>>

// protected ==================================================================
CPoints CGannTool::GetTwoPoints() const
{
	return CTwoPointMoveTool::GetPoints(CMoveTool::GetPoints().GetDrawingPoint(), 45);
}


///////////////////////////////////////////////////////////////////////////////
// class CGannLineTool -- 갠라인

CGannLineTool::CGannLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CTwoPointDrawer& CGannLineTool::GetTwoPointDrawer()
{
	return m_gannLineDrawer;
}

const CGannDrawer& CGannLineTool::GetGannDrawer() const
{
	return m_gannLineDrawer;
}


///////////////////////////////////////////////////////////////////////////////
// class CGannLine45Tool -- 갠라인(45)

CGannLine45Tool::CGannLine45Tool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannLineTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CGannLine45Tool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_GANN_LINE45;
}


///////////////////////////////////////////////////////////////////////////////
// class CGannLine45Tool -- 갠라인(315 == -45)

CGannLine315Tool::CGannLine315Tool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannLineTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CGannLine315Tool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_GANN_LINE315;
}

CPoints CGannLine315Tool::GetTwoPoints() const
{
	return CTwoPointMoveTool::GetPoints(CMoveTool::GetPoints().GetDrawingPoint(), -45);
}


///////////////////////////////////////////////////////////////////////////////
// class CGannFanTool -- 갠펜

CGannFanTool::CGannFanTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)
{
}

void CGannFanTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

void CGannFanTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}
// protected ==================================================================
CTwoPointDrawer& CGannFanTool::GetTwoPointDrawer()
{
	return m_gannFanDrawer;
}

const CGannDrawer& CGannFanTool::GetGannDrawer() const
{
	return m_gannFanDrawer;
}

const CTextPosition* CGannFanTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CGannFanTool::GetTextPosition()
{
	return &m_textPosition;
}
///////////////////////////////////////////////////////////////////////////////
// class CGannFan45Tool -- 갠펜(45)

CGannFan45Tool::CGannFan45Tool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannFanTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CGannFan45Tool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_GANN_FAN45;
}

///////////////////////////////////////////////////////////////////////////////
// class CGannFan135Tool -- 갠펜(135)

CGannFan135Tool::CGannFan135Tool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannFanTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CGannFan135Tool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_GANN_FAN135;
}

CPoints CGannFan135Tool::GetTwoPoints() const
{
	return CTwoPointMoveTool::GetPoints(CMoveTool::GetPoints().GetDrawingPoint(), 135, -8);
}

///////////////////////////////////////////////////////////////////////////////
// class CGannFan225Tool -- 갠펜(225)

CGannFan225Tool::CGannFan225Tool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannFanTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CGannFan225Tool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_GANN_FAN225;
}

CPoints CGannFan225Tool::GetTwoPoints() const
{
	return CTwoPointMoveTool::GetPoints(CMoveTool::GetPoints().GetDrawingPoint(), 225, -8);
}


///////////////////////////////////////////////////////////////////////////////
// class CGannFan315Tool -- 갠펜(315 == -45)

CGannFan315Tool::CGannFan315Tool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannFanTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CGannFan315Tool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_GANN_FAN315;
}

CPoints CGannFan315Tool::GetTwoPoints() const
{
	return CTwoPointMoveTool::GetPoints(CMoveTool::GetPoints().GetDrawingPoint(), -45);
}

///////////////////////////////////////////////////////////////////////////////
// class CGannGridTool -- 갠그리드

CGannGridTool::CGannGridTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType)
	: CGannTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CGannGridTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_GANN_GRID;
}

CTwoPointDrawer& CGannGridTool::GetTwoPointDrawer()
{
	return m_gannGridDrawer;
}

const CGannDrawer& CGannGridTool::GetGannDrawer() const
{
	return m_gannGridDrawer;
}
