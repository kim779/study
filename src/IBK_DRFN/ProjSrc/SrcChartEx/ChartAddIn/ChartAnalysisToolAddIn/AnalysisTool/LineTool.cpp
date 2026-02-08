// LineTool.cpp: implementation of the CLineTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CLineTool -- 추세선

CLineTool::CLineTool(CAnalysisData* pAnalysisData, const CAnalysis::TOOLTYPE p_eToolType, 
					 const CAnalysis::COORDINATETYPE eCoordinateType):
	CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true, true),
	m_lineDrawer(p_eToolType)
{
	m_eToolType = p_eToolType;
}

void CLineTool::SetTextPosition(const CTextHorzPosition& position)
{
	m_textPosition = position;
}

void CLineTool::SetTextPosition(const bool bLeft, const bool bRight, const bool bRightRate)
{
	m_textPosition.SetLeft(bLeft);
	m_textPosition.SetRight(bRight);
	m_textPosition.SetRightRate(bRightRate);
}

void CLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CLineTool::GetToolType() const
{
	return m_eToolType;
}

const CTextPosition* CLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CLineTool::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CLineTool::GetTwoPointDrawer()
{
	return m_lineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CVertLineTool -- 수직선

CVertLineTool::CVertLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	COnePointMoveTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), false, true)
{
}

void CVertLineTool::SetTextPosition(const CTextVertPosition& position)
{
	m_textPosition = position;
}

void CVertLineTool::SetTextPosition(const bool bTop, const bool bBottom)
{
	m_textPosition.SetTop(bTop);
	m_textPosition.SetBottom(bBottom);
}

void CVertLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CVertLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_VERT_LINE;
}

const CTextPosition* CVertLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CVertLineTool::GetTextPosition()
{
	return &m_textPosition;
}

COnePointDrawer& CVertLineTool::GetOnePointDrawer()
{
	return m_vertLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CHorzLineTool -- 수평선

CHorzLineTool::CHorzLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	COnePointMoveTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), false, true)
{
}

void CHorzLineTool::SetTextPosition(const CTextHorzPosition& position)
{
	m_textPosition = position;
}

void CHorzLineTool::SetTextPosition(const bool bLeft, const bool bRight)
{
	m_textPosition.SetLeft(bLeft);
	m_textPosition.SetRight(bRight);
}

void CHorzLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CHorzLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_HORZ_LINE;
}

const CTextPosition* CHorzLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CHorzLineTool::GetTextPosition()
{
	return &m_textPosition;
}

COnePointDrawer& CHorzLineTool::GetOnePointDrawer()
{
	return m_horzLineDrawer;
}

// 그리는 영역들에 속한 point 인지의 여부.
/*bool CHorzLineTool::PointInDrawingRegion(const CPoint& point) const
{
	if(!CTool::GetDrawingRegions().PointInDrawingRegionsInColumn(point))
		return false;

	int nRowIndex1 = CTool::GetDrawingRegions().GetRowNumber(point);
	int nRowIndex2 = CTool::GetDrawingRegions().GetRowNumber(CMoveTool::GetPoints().GetDrawingPoint());
	return (nRowIndex1 == nRowIndex2);
}*/

///////////////////////////////////////////////////////////////////////////////
// class CCrossLineTool -- 십자선

CCrossLineTool::CCrossLineTool():
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), false, true, false, true)
{
}

void CCrossLineTool::SetTextPosition(const CTextHorzVertPosition& position)
{
	m_textPosition = position;
}

void CCrossLineTool::SetTextPosition(const bool bLeft, const bool bRight, const bool bTop, const bool bBottom)
{
	m_textPosition.SetLeft(bLeft);
	m_textPosition.SetRight(bRight);
	m_textPosition.SetTop(bTop);
	m_textPosition.SetBottom(bBottom);
}

void CCrossLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
const CTextPosition* CCrossLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CCrossLineTool::GetTextPosition()
{
	return &m_textPosition;
}

COnePointDrawer& CCrossLineTool::GetOnePointDrawer()
{
	return m_crossLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CCrossLineMoveTool -- mouse move 시

CCrossLineMoveTool::CCrossLineMoveTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	COnePointMoveTool(pAnalysisData, eCoordinateType),
	CCrossLineTool()
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CCrossLineMoveTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_MOVE_CROSS;
}

const CTextPosition* CCrossLineMoveTool::GetTextPosition() const
{
	return CCrossLineTool::GetTextPosition();
}

CTextPosition* CCrossLineMoveTool::GetTextPosition()
{
	return CCrossLineTool::GetTextPosition();
}

COnePointDrawer& CCrossLineMoveTool::GetOnePointDrawer()
{
	return CCrossLineTool::GetOnePointDrawer();
}

///////////////////////////////////////////////////////////////////////////////
// class CCrossLineDownTool -- mouse down 시

CCrossLineDownTool::CCrossLineDownTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	COnePointDownTool(pAnalysisData, eCoordinateType),
	CCrossLineTool()
{
}

// public =====================================================================
void CCrossLineDownTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	// 객체를 만들지 않는다.
	CTool::Initialize();
}

// protected ==================================================================
CAnalysis::TOOLTYPE CCrossLineDownTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_DOWN_CROSS;
}

const CTextPosition* CCrossLineDownTool::GetTextPosition() const
{
	return CCrossLineTool::GetTextPosition();
}

CTextPosition* CCrossLineDownTool::GetTextPosition()
{
	return CCrossLineTool::GetTextPosition();
}

COnePointDrawer& CCrossLineDownTool::GetOnePointDrawer()
{
	return CCrossLineTool::GetOnePointDrawer();
}

///////////////////////////////////////////////////////////////////////////////
// class CTrisectionLineTool -- 삼등분선

CTrisectionLineTool::CTrisectionLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true, true)
{
}

void CTrisectionLineTool::SetTextPosition(const CTextHorzPosition& position)
{
	m_textPosition = position;
}

void CTrisectionLineTool::SetTextPosition(const bool bLeft, const bool bRight)
{
	m_textPosition.SetLeft(bLeft);
	m_textPosition.SetRight(bRight);
}

void CTrisectionLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CTrisectionLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_TRISECT;
}

const CTextPosition* CTrisectionLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CTrisectionLineTool::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CTrisectionLineTool::GetTwoPointDrawer()
{
	return m_trisectionLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CQuadrisectLineTool -- 삼등분선

CQuadrisectLineTool::CQuadrisectLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true, true)
{
}

void CQuadrisectLineTool::SetTextPosition(const CTextHorzPosition& position)
{
	m_textPosition = position;
}

void CQuadrisectLineTool::SetTextPosition(const bool bLeft, const bool bRight)
{
	m_textPosition.SetLeft(bLeft);
	m_textPosition.SetRight(bRight);
}

void CQuadrisectLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CQuadrisectLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_QUARTER;
}

const CTextPosition* CQuadrisectLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CQuadrisectLineTool::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CQuadrisectLineTool::GetTwoPointDrawer()
{
	return m_quadrisectLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CAndrewsPitchforkTool -- 엔드류 피치포크

CAndrewsPitchforkTool::CAndrewsPitchforkTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CThreePointDownTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CAndrewsPitchforkTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_ANDREWS_PITCHFORK;
}

CThreePointDrawer& CAndrewsPitchforkTool::GetThreePointDrawer()
{
	return m_andrewsDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CCycleLinesTool -- 사이클 구간

CCycleLinesTool::CCycleLinesTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CTwoPointDownTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CCycleLinesTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_CYCLE_LINES;
}

CTwoPointDrawer& CCycleLinesTool::GetTwoPointDrawer()
{
	return m_cycleLinesDrawer;
}

/*CPoints CCycleLinesTool::GetTwoPoints() const
{
	return CTwoPointDownTool::GetPoints(CMoveTool::GetPoints().GetDrawingPoint(), 0);
}*/

///////////////////////////////////////////////////////////////////////////////
// class CSpeedLineTool -- 스피드 라인

CSpeedLineTool::CSpeedLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CTwoPointDownTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CSpeedLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_SPEED_LINE;
}

CTwoPointDrawer& CSpeedLineTool::GetTwoPointDrawer()
{
	return m_speedLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CSpeedFanTool 

CSpeedFanTool::CSpeedFanTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType)
	
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CSpeedFanTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_SPEED_FAN;
}

CTwoPointDrawer& CSpeedFanTool::GetTwoPointDrawer()
{
	return m_speedFanDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CSpeedArcTool

CSpeedArcTool::CSpeedArcTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType)
	
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CSpeedArcTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_SPEED_ARC;
}

CTwoPointDrawer& CSpeedArcTool::GetTwoPointDrawer()
{
	return m_speedArcDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CLinearRegressionTool

CLinearRegressionTool::CLinearRegressionTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType)
	
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CLinearRegressionTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION;
}

CTwoPointDrawer& CLinearRegressionTool::GetTwoPointDrawer()
{
	return m_linearRegressionDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CRaffRegressionTool

CRaffRegressionTool::CRaffRegressionTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType)
	
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CRaffRegressionTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_RAFF_REGRESSION;
}

CTwoPointDrawer& CRaffRegressionTool::GetTwoPointDrawer()
{
	return m_raffRegressionDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CAngleLineTool

CAngleLineTool::CAngleLineTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)	
{
}

void CAngleLineTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

void CAngleLineTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}

void CAngleLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CAngleLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_ANGLE_LINE;
}

const CTextPosition* CAngleLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CAngleLineTool::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CAngleLineTool::GetTwoPointDrawer()
{
	return m_angleLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CCandleLineTool

CCandleLineTool::CCandleLineTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: COnePointMoveTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true)	
{
}

void CCandleLineTool::SetTextPosition(const CTextShowPosition& position)
{
	m_textPosition = position;
}

void CCandleLineTool::SetTextPosition(const bool bShow)
{
	m_textPosition.SetShow(bShow);
}

void CCandleLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CCandleLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_CANDLE_LINE;
}

const CTextPosition* CCandleLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CCandleLineTool::GetTextPosition()
{
	return &m_textPosition;
}

COnePointDrawer& CCandleLineTool::GetOnePointDrawer()
{
	return m_candleLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CCandleBongTool 

CCandleBongTool::CCandleBongTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType)
	
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CCandleBongTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_CANDLE_BONG;
}

CTwoPointDrawer& CCandleBongTool::GetTwoPointDrawer()
{
	return m_candleBongDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CPencilLineTool 

CPencilLineTool::CPencilLineTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CMultiPointDownTool(pAnalysisData, eCoordinateType)
	
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CPencilLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_PENCIL_LINE;
}

CMultiPointDrawer& CPencilLineTool::GetMultiPointDrawer()
{
	return m_pencilLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CPeriodSumLineTool -- 기간요약

CPeriodSumLineTool::CPeriodSumLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CTwoPointDownTool(pAnalysisData, eCoordinateType),
	m_textPosition(CTextEnvironment(RGB(0, 0, 0)), true, true)
{
}

void CPeriodSumLineTool::SetTextPosition(const CTextHorzPosition& position)
{
	m_textPosition = position;
}

void CPeriodSumLineTool::SetTextPosition(const bool bLeft, const bool bRight)
{
	m_textPosition.SetLeft(bLeft);
	m_textPosition.SetRight(bRight);
}

void CPeriodSumLineTool::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textPosition.SetTextEnvironment(textEnvironment);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CPeriodSumLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_PERIOD_SUMMARY;
}

const CTextPosition* CPeriodSumLineTool::GetTextPosition() const
{
	return &m_textPosition;
}

CTextPosition* CPeriodSumLineTool::GetTextPosition()
{
	return &m_textPosition;
}

CTwoPointDrawer& CPeriodSumLineTool::GetTwoPointDrawer()
{
	return m_periodSumLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CElliotLineTool 

CElliotLineTool::CElliotLineTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType)
	: CTwoPointDownTool(pAnalysisData, eCoordinateType)
	
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CElliotLineTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE;
}

CTwoPointDrawer& CElliotLineTool::GetTwoPointDrawer()
{
	return m_elliotLineDrawer;
}

///////////////////////////////////////////////////////////////////////////////
// class CAdjustRatioTool -- 

CAdjustRatioTool::CAdjustRatioTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CThreePointDownTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
CAnalysis::TOOLTYPE CAdjustRatioTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_ADJUST_RATIO;
}

CThreePointDrawer& CAdjustRatioTool::GetThreePointDrawer()
{
	return m_adjustRatioDrawer;
}