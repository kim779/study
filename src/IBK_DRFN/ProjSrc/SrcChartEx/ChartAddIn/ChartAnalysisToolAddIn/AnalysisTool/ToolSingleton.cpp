// ToolSingleton.cpp: implementation of the CToolSingleton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolSingleton.h"

#include "../Include_Analysis/ElementEnvironment.h"
#include "Selecting.h"
#include "LineTool.h"
#include "FiboGannTool.h"
#include "BalanceBasicTool.h"	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CToolSingleton

CToolSingleton::CToolSingleton(CAnalysisData* pAnalysisData)
{
	m_pSelectingTool = new CSelectingTool(pAnalysisData);
	m_pCrossLineMoveTool = new CCrossLineMoveTool(pAnalysisData);
	m_pCrossLineDownTool = new CCrossLineDownTool(pAnalysisData);
	m_pVertLineTool = new CVertLineTool(pAnalysisData);
	m_pHorzLineTool = new CHorzLineTool(pAnalysisData);

	m_pLineTool	= new CLineTool( pAnalysisData, CAnalysis::EN_ANALYSIS_LINE);

	m_pTrisectionLineTool = new CTrisectionLineTool(pAnalysisData);
	m_pQuadrisectLineTool = new CQuadrisectLineTool(pAnalysisData);
	m_pFiboSemicircleTool = new CFiboSemicircleTool(pAnalysisData);
	m_pFiboCircleTool = new CFiboCircleTool(pAnalysisData);
	m_pFiboPenLineTool = new CFiboPenLineTool(pAnalysisData);
	m_pFiboRetraceTool = new CFiboRetraceTool(pAnalysisData);
	m_pFiboTimeTool = new CFiboTimeTool(pAnalysisData);
	m_pGannLine45Tool = new CGannLine45Tool(pAnalysisData);
	m_pGannLine315Tool = new CGannLine315Tool(pAnalysisData);
	m_pGannFan45Tool = new CGannFan45Tool(pAnalysisData);
	m_pGannFan135Tool = new CGannFan135Tool(pAnalysisData);
	m_pGannFan225Tool = new CGannFan225Tool(pAnalysisData);
	m_pGannFan315Tool = new CGannFan315Tool(pAnalysisData);
	m_pGannGridTool = new CGannGridTool(pAnalysisData);
	m_pAndrewsTool = new CAndrewsPitchforkTool(pAnalysisData);
	m_pCycleLinesTool = new CCycleLinesTool(pAnalysisData);
	m_pSpeedLineTool = new CSpeedLineTool(pAnalysisData);
	m_pSpeedFanTool = new CSpeedFanTool(pAnalysisData);
	m_pSpeedArcTool = new CSpeedArcTool(pAnalysisData);
	m_pLinearRegressionTool = new CLinearRegressionTool(pAnalysisData);
	m_pRaffRegressionTool = new CRaffRegressionTool(pAnalysisData);
	m_pAngleLineTool = new CAngleLineTool(pAnalysisData);
	m_pCandleLineTool = new CCandleLineTool(pAnalysisData);
	m_pCandleBongTool = new CCandleBongTool(pAnalysisData);
	m_pPencilLineTool = new CPencilLineTool(pAnalysisData);
	m_pPeriodLineTool = new CPeriodSumLineTool(pAnalysisData);
	m_pElliotLineTool = new CElliotLineTool(pAnalysisData);
	m_pAdjustRatioTool = new CAdjustRatioTool(pAnalysisData);
	m_pBalanceBasicTool = new CBalanceBasicTool(pAnalysisData);		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

CToolSingleton::~CToolSingleton()
{
	delete m_pSelectingTool;
	delete m_pCrossLineMoveTool;
	delete m_pCrossLineDownTool;
	delete m_pVertLineTool;
	delete m_pHorzLineTool;

	delete m_pLineTool;

	delete m_pTrisectionLineTool;
	delete m_pQuadrisectLineTool;
	delete m_pFiboSemicircleTool;
	delete m_pFiboCircleTool;
	delete m_pFiboPenLineTool;
	delete m_pFiboRetraceTool;
	delete m_pFiboTimeTool;
	delete m_pGannLine45Tool;
	delete m_pGannLine315Tool;
	delete m_pGannFan45Tool;
	delete m_pGannFan135Tool;
	delete m_pGannFan225Tool;
	delete m_pGannFan315Tool;
	delete m_pGannGridTool;
	delete m_pAndrewsTool;
	delete m_pCycleLinesTool;
	delete m_pSpeedLineTool;
	delete m_pSpeedFanTool;
	delete m_pSpeedArcTool;
	delete m_pLinearRegressionTool;
	delete m_pRaffRegressionTool;
	delete m_pAngleLineTool;
	delete m_pCandleLineTool;
	delete m_pCandleBongTool;
	delete m_pPencilLineTool;
	delete m_pPeriodLineTool;
	delete m_pElliotLineTool;
	delete m_pAdjustRatioTool;

	delete m_pBalanceBasicTool;		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

// 현재 맞는 객체의 Tool의 주소를 찾는다.
CTool* CToolSingleton::FindTool(const CAnalysis::TOOLTYPE eToolType)
{
	switch(eToolType){
	case CAnalysis::EN_ANALYSIS_SELECTION:		// 선택
		return m_pSelectingTool;
	case CAnalysis::EN_ANALYSIS_MOVE_CROSS:		// 십자선 mouse move
		return m_pCrossLineMoveTool;
	case CAnalysis::EN_ANALYSIS_DOWN_CROSS:		// 십자선 mouse down
		return m_pCrossLineDownTool;
	case CAnalysis::EN_ANALYSIS_VERT_LINE:		// 수직선
		return m_pVertLineTool;
	case CAnalysis::EN_ANALYSIS_HORZ_LINE:		// 수평선
		return m_pHorzLineTool;	
	case CAnalysis::EN_ANALYSIS_LINE:			// 추세선
		return m_pLineTool;
	case CAnalysis::EN_ANALYSIS_TRISECT:		// 삼등분선
		return m_pTrisectionLineTool;
	case CAnalysis::EN_ANALYSIS_QUARTER:		// 사등분선
		return m_pQuadrisectLineTool;
	case CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE:// 피보나치 반원
		return m_pFiboSemicircleTool;
	case CAnalysis::EN_ANALYSIS_FIBO_CIRCLE:	// 피보나치 원
		return m_pFiboCircleTool;
	case CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE:	// 피보나치 팬라인
		return m_pFiboPenLineTool;	
	case CAnalysis::EN_ANALYSIS_FIBO_RETRACE:	// 피보나치 되돌림
		return m_pFiboRetraceTool;
	case CAnalysis::EN_ANALYSIS_FIBO_TIME:		// 피보나치 시간대
		return m_pFiboTimeTool;
	case CAnalysis::EN_ANALYSIS_GANN_LINE45:	// 갠라인(45)
		return m_pGannLine45Tool;
	case CAnalysis::EN_ANALYSIS_GANN_LINE315:	// 갠라인(315)
		return m_pGannLine315Tool;
	case CAnalysis::EN_ANALYSIS_GANN_FAN45:		// 갠펜(우상45도)
		return m_pGannFan45Tool;
	case CAnalysis::EN_ANALYSIS_GANN_FAN135:	// 갠펜(좌상45도)
		return m_pGannFan135Tool;
	case CAnalysis::EN_ANALYSIS_GANN_FAN225:	// 갠펜(좌하45도)
		return m_pGannFan225Tool;
	case CAnalysis::EN_ANALYSIS_GANN_FAN315:	// 갠펜(우하45도)
		return m_pGannFan315Tool;
	case CAnalysis::EN_ANALYSIS_GANN_GRID:		// 갠그리드
		return m_pGannGridTool;
	case CAnalysis::EN_ANALYSIS_ANDREWS_PITCHFORK:	// 엔드류 피치포크
		return m_pAndrewsTool;
	case CAnalysis::EN_ANALYSIS_CYCLE_LINES:	// 사이클 구간
		return m_pCycleLinesTool;
	case CAnalysis::EN_ANALYSIS_SPEED_LINE:		// 스피드 라인
		return m_pSpeedLineTool;
	case CAnalysis::EN_ANALYSIS_SPEED_FAN:		// 스피드 Fan영역
		return m_pSpeedFanTool;
	case CAnalysis::EN_ANALYSIS_SPEED_ARC:		// 스피드 호
		return m_pSpeedArcTool;
	case CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION:	// 선형회귀선
		return m_pLinearRegressionTool;
	case CAnalysis::EN_ANALYSIS_RAFF_REGRESSION:	// 선형회귀채널
		return m_pRaffRegressionTool;
	case CAnalysis::EN_ANALYSIS_ANGLE_LINE:		// 각도추세선
		return m_pAngleLineTool;
	case CAnalysis::EN_ANALYSIS_CANDLE_LINE:	// 봉중심선
		return m_pCandleLineTool;
	case CAnalysis::EN_ANALYSIS_CANDLE_BONG:	// 봉추세선
		return m_pCandleBongTool;
	case CAnalysis::EN_ANALYSIS_PENCIL_LINE:	// 연필추세선
		return m_pPencilLineTool;
	case CAnalysis::EN_ANALYSIS_PERIOD_SUMMARY:	// 기간요약
		return m_pPeriodLineTool;
	case CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE:	// Elliot
		return m_pElliotLineTool;
	case CAnalysis::EN_ANALYSIS_ADJUST_RATIO:	// AdjustRatio
		return m_pAdjustRatioTool;
	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
	case CAnalysis::EN_ANALYSIS_BALANCE_BASIC:	// 일목수치
		return m_pBalanceBasicTool;
	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<
	}

	return NULL;
}

// ----------------------------------------------------------------------------
// 모든 Tool을 초기화 한다.
void CToolSingleton::InitializeAllTool()
{
	InitializeTool(m_pSelectingTool);
	InitializeTool(m_pCrossLineMoveTool);
	InitializeTool(m_pCrossLineDownTool);
	InitializeTool(m_pVertLineTool);
	InitializeTool(m_pHorzLineTool);

	InitializeTool( m_pLineTool);

	InitializeTool(m_pTrisectionLineTool);
	InitializeTool(m_pQuadrisectLineTool);
	InitializeTool(m_pFiboSemicircleTool);
	InitializeTool(m_pFiboCircleTool);
	InitializeTool(m_pFiboPenLineTool);
	InitializeTool(m_pFiboRetraceTool);
	InitializeTool(m_pFiboTimeTool);
	InitializeTool(m_pGannLine45Tool);
	InitializeTool(m_pGannLine315Tool);
	InitializeTool(m_pGannFan45Tool);
	InitializeTool(m_pGannFan135Tool);
	InitializeTool(m_pGannFan225Tool);
	InitializeTool(m_pGannFan315Tool);
	InitializeTool(m_pGannGridTool);
	InitializeTool(m_pAndrewsTool);
	InitializeTool(m_pCycleLinesTool);
	InitializeTool(m_pSpeedLineTool);
	InitializeTool(m_pSpeedFanTool);
	InitializeTool(m_pSpeedArcTool);
	InitializeTool(m_pLinearRegressionTool);
	InitializeTool(m_pRaffRegressionTool);
	InitializeTool(m_pAngleLineTool);
	InitializeTool(m_pCandleLineTool);
	InitializeTool(m_pCandleBongTool);
	InitializeTool(m_pPencilLineTool);
	InitializeTool(m_pPeriodLineTool);
	InitializeTool(m_pElliotLineTool);
	InitializeTool(m_pAdjustRatioTool);

	InitializeTool(m_pBalanceBasicTool);	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

void CToolSingleton::SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType)
{
	SetCoordinateType(m_pSelectingTool, eCoordinateType);
	SetCoordinateType(m_pCrossLineMoveTool, eCoordinateType);
	SetCoordinateType(m_pCrossLineDownTool, eCoordinateType);
	SetCoordinateType(m_pVertLineTool, eCoordinateType);
	SetCoordinateType(m_pHorzLineTool, eCoordinateType);

	SetCoordinateType( m_pLineTool,	eCoordinateType);

	SetCoordinateType(m_pTrisectionLineTool, eCoordinateType);
	SetCoordinateType(m_pQuadrisectLineTool, eCoordinateType);
	SetCoordinateType(m_pFiboSemicircleTool, eCoordinateType);
	SetCoordinateType(m_pFiboCircleTool, eCoordinateType);
	SetCoordinateType(m_pFiboPenLineTool, eCoordinateType);
	SetCoordinateType(m_pFiboRetraceTool, eCoordinateType);
	SetCoordinateType(m_pFiboTimeTool, eCoordinateType);
	SetCoordinateType(m_pGannLine45Tool, eCoordinateType);
	SetCoordinateType(m_pGannLine315Tool, eCoordinateType);
	SetCoordinateType(m_pGannFan45Tool, eCoordinateType);
	SetCoordinateType(m_pGannFan135Tool, eCoordinateType);
	SetCoordinateType(m_pGannFan225Tool, eCoordinateType);
	SetCoordinateType(m_pGannFan315Tool, eCoordinateType);
	SetCoordinateType(m_pGannGridTool, eCoordinateType);
	SetCoordinateType(m_pAndrewsTool, eCoordinateType);
	SetCoordinateType(m_pCycleLinesTool, eCoordinateType);
	SetCoordinateType(m_pSpeedLineTool, eCoordinateType);
	SetCoordinateType(m_pSpeedFanTool, eCoordinateType);
	SetCoordinateType(m_pSpeedArcTool, eCoordinateType);
	SetCoordinateType(m_pLinearRegressionTool, eCoordinateType);
	SetCoordinateType(m_pRaffRegressionTool, eCoordinateType);
	SetCoordinateType(m_pAngleLineTool, eCoordinateType);
	SetCoordinateType(m_pCandleLineTool, eCoordinateType);
	SetCoordinateType(m_pCandleBongTool, eCoordinateType);
	SetCoordinateType(m_pPencilLineTool, eCoordinateType);
	SetCoordinateType(m_pPeriodLineTool, eCoordinateType);
	SetCoordinateType(m_pElliotLineTool, eCoordinateType);
	SetCoordinateType(m_pAdjustRatioTool, eCoordinateType);

	SetCoordinateType(m_pBalanceBasicTool, eCoordinateType);	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

void CToolSingleton::SetAutoPrice(const bool bIsAutoPrice)
{
	SetAutoPrice(m_pSelectingTool, bIsAutoPrice);
	SetAutoPrice(m_pCrossLineMoveTool, bIsAutoPrice);
	SetAutoPrice(m_pCrossLineDownTool, bIsAutoPrice);
	SetAutoPrice(m_pVertLineTool, bIsAutoPrice);
	SetAutoPrice(m_pHorzLineTool, bIsAutoPrice);

	SetAutoPrice(m_pLineTool, bIsAutoPrice);
	
	SetAutoPrice(m_pTrisectionLineTool, bIsAutoPrice);
	SetAutoPrice(m_pQuadrisectLineTool, bIsAutoPrice);
	SetAutoPrice(m_pFiboSemicircleTool, bIsAutoPrice);
	SetAutoPrice(m_pFiboCircleTool, bIsAutoPrice);
	SetAutoPrice(m_pFiboPenLineTool, bIsAutoPrice);
	SetAutoPrice(m_pFiboRetraceTool, bIsAutoPrice);
	SetAutoPrice(m_pFiboTimeTool, bIsAutoPrice);
	SetAutoPrice(m_pGannLine45Tool, bIsAutoPrice);
	SetAutoPrice(m_pGannLine315Tool, bIsAutoPrice);
	SetAutoPrice(m_pGannFan45Tool, bIsAutoPrice);
	SetAutoPrice(m_pGannFan135Tool, bIsAutoPrice);
	SetAutoPrice(m_pGannFan225Tool, bIsAutoPrice);
	SetAutoPrice(m_pGannFan315Tool, bIsAutoPrice);
	SetAutoPrice(m_pGannGridTool, bIsAutoPrice);
	SetAutoPrice(m_pAndrewsTool, bIsAutoPrice);
	SetAutoPrice(m_pCycleLinesTool, bIsAutoPrice);
	SetAutoPrice(m_pSpeedLineTool, bIsAutoPrice);
	SetAutoPrice(m_pSpeedFanTool, bIsAutoPrice);
	SetAutoPrice(m_pSpeedArcTool, bIsAutoPrice);
	SetAutoPrice(m_pLinearRegressionTool, bIsAutoPrice);
	SetAutoPrice(m_pRaffRegressionTool, bIsAutoPrice);
	SetAutoPrice(m_pAngleLineTool, bIsAutoPrice);
	SetAutoPrice(m_pCandleLineTool, bIsAutoPrice);
	SetAutoPrice(m_pCandleBongTool, bIsAutoPrice);
	SetAutoPrice(m_pPencilLineTool, bIsAutoPrice);
	SetAutoPrice(m_pPeriodLineTool, bIsAutoPrice);
	SetAutoPrice(m_pElliotLineTool, bIsAutoPrice);
	SetAutoPrice(m_pAdjustRatioTool, bIsAutoPrice);

	SetAutoPrice(m_pBalanceBasicTool, bIsAutoPrice);	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

void CToolSingleton::SetBaseColor(const COLORREF& penColor, const COLORREF& textColor)
{
	// Text Color가 고정인 Tool
	SetBasePenColor(m_pCrossLineMoveTool, penColor);
	SetBasePenColor(m_pCrossLineDownTool, penColor);
	SetBasePenColor(m_pVertLineTool, penColor);
	SetBasePenColor(m_pHorzLineTool, penColor);

	// Text Color가 고정이 아닌 Tool
	SetBaseColor(m_pSelectingTool, penColor, textColor);
	SetBaseColor( m_pLineTool, penColor, textColor);

	SetBaseColor(m_pTrisectionLineTool, penColor, textColor);
	SetBaseColor(m_pQuadrisectLineTool, penColor, textColor);
	SetBaseColor(m_pFiboSemicircleTool, penColor, textColor);
	SetBaseColor(m_pFiboCircleTool, penColor, textColor);
	SetBaseColor(m_pFiboPenLineTool, penColor, textColor);
	SetBaseColor(m_pFiboRetraceTool, penColor, textColor);
	SetBaseColor(m_pFiboTimeTool, penColor, textColor);
	SetBaseColor(m_pGannLine45Tool, penColor, textColor);
	SetBaseColor(m_pGannLine315Tool, penColor, textColor);
	SetBaseColor(m_pGannFan45Tool, penColor, textColor);
	SetBaseColor(m_pGannFan135Tool, penColor, textColor);
	SetBaseColor(m_pGannFan225Tool, penColor, textColor);
	SetBaseColor(m_pGannFan315Tool, penColor, textColor);
	SetBaseColor(m_pGannGridTool, penColor, textColor);
	SetBaseColor(m_pAndrewsTool, penColor, textColor);
	SetBaseColor(m_pCycleLinesTool, penColor, textColor);
	SetBaseColor(m_pSpeedLineTool, penColor, textColor);
	SetBaseColor(m_pSpeedFanTool, penColor, textColor);
	SetBaseColor(m_pSpeedArcTool, penColor, textColor);
	SetBaseColor(m_pLinearRegressionTool, penColor, textColor);
	SetBaseColor(m_pRaffRegressionTool, penColor, textColor);
	SetBaseColor(m_pAngleLineTool, penColor, textColor);
	SetBaseColor(m_pCandleLineTool, penColor, textColor);
	SetBaseColor(m_pCandleBongTool, penColor, textColor);
	SetBaseColor(m_pPencilLineTool, penColor, textColor);
	SetBaseColor(m_pPeriodLineTool, penColor, textColor);
	SetBaseColor(m_pElliotLineTool, penColor, textColor);
	SetBaseColor(m_pAdjustRatioTool, penColor, textColor);

	SetBaseColor(m_pBalanceBasicTool, penColor, textColor);		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

void CToolSingleton::SetBasePenColor(const COLORREF& color)
{
	SetBasePenColor(m_pSelectingTool, color);
	SetBasePenColor(m_pCrossLineMoveTool, color);
	SetBasePenColor(m_pCrossLineDownTool, color);
	SetBasePenColor(m_pVertLineTool, color);
	SetBasePenColor(m_pHorzLineTool, color);

	SetBasePenColor( m_pLineTool, color);

	SetBasePenColor(m_pTrisectionLineTool, color);
	SetBasePenColor(m_pQuadrisectLineTool, color);
	SetBasePenColor(m_pFiboSemicircleTool, color);
	SetBasePenColor(m_pFiboCircleTool, color);
	SetBasePenColor(m_pFiboPenLineTool, color);
	SetBasePenColor(m_pFiboRetraceTool, color);
	SetBasePenColor(m_pFiboTimeTool, color);
	SetBasePenColor(m_pGannLine45Tool, color);
	SetBasePenColor(m_pGannLine315Tool, color);
	SetBasePenColor(m_pGannFan45Tool, color);
	SetBasePenColor(m_pGannFan135Tool, color);
	SetBasePenColor(m_pGannFan225Tool, color);
	SetBasePenColor(m_pGannFan315Tool, color);
	SetBasePenColor(m_pGannGridTool, color);
	SetBasePenColor(m_pAndrewsTool, color);
	SetBasePenColor(m_pCycleLinesTool, color);
	SetBasePenColor(m_pSpeedLineTool, color);
	SetBasePenColor(m_pSpeedFanTool, color);
	SetBasePenColor(m_pSpeedArcTool, color);
	SetBasePenColor(m_pLinearRegressionTool, color);
	SetBasePenColor(m_pRaffRegressionTool, color);
	SetBasePenColor(m_pAngleLineTool, color);
	SetBasePenColor(m_pCandleLineTool, color);
	SetBasePenColor(m_pCandleBongTool, color);
	SetBasePenColor(m_pPencilLineTool, color);
	SetBasePenColor(m_pPeriodLineTool, color);
	SetBasePenColor(m_pElliotLineTool, color);
	SetBasePenColor(m_pAdjustRatioTool, color);

	SetBasePenColor(m_pBalanceBasicTool, color);	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

void CToolSingleton::SetBasePenThickness(const int nThickness)
{
	SetBasePenThickness(m_pSelectingTool, nThickness);
	SetBasePenThickness(m_pCrossLineMoveTool, nThickness);
	SetBasePenThickness(m_pCrossLineDownTool, nThickness);
	SetBasePenThickness(m_pVertLineTool, nThickness);
	SetBasePenThickness(m_pHorzLineTool, nThickness);

	SetBasePenThickness( m_pLineTool, nThickness);

	SetBasePenThickness(m_pTrisectionLineTool, nThickness);
	SetBasePenThickness(m_pQuadrisectLineTool, nThickness);
	SetBasePenThickness(m_pFiboSemicircleTool, nThickness);
	SetBasePenThickness(m_pFiboCircleTool, nThickness);
	SetBasePenThickness(m_pFiboPenLineTool, nThickness);
	SetBasePenThickness(m_pFiboRetraceTool, nThickness);
	SetBasePenThickness(m_pFiboTimeTool, nThickness);
	SetBasePenThickness(m_pGannLine45Tool, nThickness);
	SetBasePenThickness(m_pGannLine315Tool, nThickness);
	SetBasePenThickness(m_pGannFan45Tool, nThickness);
	SetBasePenThickness(m_pGannFan135Tool, nThickness);
	SetBasePenThickness(m_pGannFan225Tool, nThickness);
	SetBasePenThickness(m_pGannFan315Tool, nThickness);
	SetBasePenThickness(m_pGannGridTool, nThickness);
	SetBasePenThickness(m_pAndrewsTool, nThickness);
	SetBasePenThickness(m_pCycleLinesTool, nThickness);
	SetBasePenThickness(m_pSpeedLineTool, nThickness);
	SetBasePenThickness(m_pSpeedFanTool, nThickness);
	SetBasePenThickness(m_pSpeedArcTool, nThickness);
	SetBasePenThickness(m_pLinearRegressionTool, nThickness);
	SetBasePenThickness(m_pRaffRegressionTool, nThickness);
	SetBasePenThickness(m_pAngleLineTool, nThickness);
	SetBasePenThickness(m_pCandleLineTool, nThickness);
	SetBasePenThickness(m_pCandleBongTool, nThickness);
	SetBasePenThickness(m_pPencilLineTool, nThickness);
	SetBasePenThickness(m_pPeriodLineTool, nThickness);
	SetBasePenThickness(m_pElliotLineTool, nThickness);
	SetBasePenThickness(m_pAdjustRatioTool, nThickness);

	SetBasePenThickness(m_pBalanceBasicTool, nThickness);	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

void CToolSingleton::SetLineExtEnvironment(const int& nLeftExt,const int& nRightExt)
{
	SetLineExtEnv(m_pSelectingTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pCrossLineMoveTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pCrossLineDownTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pVertLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pHorzLineTool, nLeftExt,nRightExt);

	SetLineExtEnv( m_pLineTool,nLeftExt,nRightExt);

	SetLineExtEnv(m_pTrisectionLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pQuadrisectLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pFiboSemicircleTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pFiboCircleTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pFiboPenLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pFiboRetraceTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pFiboTimeTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pGannLine45Tool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pGannLine315Tool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pGannFan45Tool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pGannFan135Tool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pGannFan225Tool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pGannFan315Tool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pGannGridTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pAndrewsTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pCycleLinesTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pSpeedLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pSpeedFanTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pSpeedArcTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pLinearRegressionTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pRaffRegressionTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pAngleLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pCandleLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pCandleBongTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pPencilLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pPeriodLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pElliotLineTool, nLeftExt,nRightExt);
	SetLineExtEnv(m_pAdjustRatioTool, nLeftExt,nRightExt);

	SetLineExtEnv(m_pBalanceBasicTool, nLeftExt, nRightExt);	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
}

void CToolSingleton::SetLineDrawingInfo(const CAnalysis::TOOLTYPE eToolType, const COLORREF crLine, const int nStyle, const int nWeight)
{
	if(eToolType == CAnalysis::EN_ANALYSIS_MOVE_CROSS)
	{
		SetBaseColor(m_pCrossLineMoveTool,crLine,crLine);
		SetBasePenStyle(m_pCrossLineMoveTool,nStyle);
		SetBasePenThickness(m_pCrossLineMoveTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_DOWN_CROSS)
	{
		SetBaseColor(m_pCrossLineDownTool,crLine,crLine);
		SetBasePenStyle(m_pCrossLineDownTool,nStyle);
		SetBasePenThickness(m_pCrossLineDownTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_VERT_LINE)
	{
		SetBaseColor(m_pVertLineTool,crLine,crLine);
		SetBasePenStyle(m_pVertLineTool,nStyle);
		SetBasePenThickness(m_pVertLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_HORZ_LINE)
	{
		SetBaseColor(m_pHorzLineTool,crLine,crLine);
		SetBasePenStyle(m_pHorzLineTool,nStyle);
		SetBasePenThickness(m_pHorzLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_LINE)
	{
		SetBaseColor(m_pLineTool,crLine,crLine);
		SetBasePenStyle(m_pLineTool,nStyle);
		SetBasePenThickness(m_pLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_TRISECT)
	{
		SetBaseColor(m_pTrisectionLineTool,crLine,crLine);
		SetBasePenStyle(m_pTrisectionLineTool,nStyle);
		SetBasePenThickness(m_pTrisectionLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_QUARTER)
	{
		SetBaseColor(m_pQuadrisectLineTool,crLine,crLine);
		SetBasePenStyle(m_pQuadrisectLineTool,nStyle);
		SetBasePenThickness(m_pQuadrisectLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE)
	{
		SetBaseColor(m_pFiboSemicircleTool,crLine,crLine);
		SetBasePenStyle(m_pFiboSemicircleTool,nStyle);
		SetBasePenThickness(m_pFiboSemicircleTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_CIRCLE)
	{
		SetBaseColor(m_pFiboCircleTool,crLine,crLine);
		SetBasePenStyle(m_pFiboCircleTool,nStyle);
		SetBasePenThickness(m_pFiboCircleTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE)
	{
		SetBaseColor(m_pFiboPenLineTool,crLine,crLine);
		SetBasePenStyle(m_pFiboPenLineTool,nStyle);
		SetBasePenThickness(m_pFiboPenLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_RETRACE)
	{
		SetBaseColor(m_pFiboRetraceTool,crLine,crLine);
		SetBasePenStyle(m_pFiboRetraceTool,nStyle);
		SetBasePenThickness(m_pFiboRetraceTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_TIME)
	{
		SetBaseColor(m_pFiboTimeTool,crLine,crLine);
		SetBasePenStyle(m_pFiboTimeTool,nStyle);
		SetBasePenThickness(m_pFiboTimeTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_LINE45)
	{
		SetBaseColor(m_pGannLine45Tool,crLine,crLine);
		SetBasePenStyle(m_pGannLine45Tool,nStyle);
		SetBasePenThickness(m_pGannLine45Tool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_LINE315)
	{
		SetBaseColor(m_pGannLine315Tool,crLine,crLine);
		SetBasePenStyle(m_pGannLine315Tool,nStyle);
		SetBasePenThickness(m_pGannLine315Tool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_FAN45)
	{
		SetBaseColor(m_pGannFan45Tool,crLine,crLine);
		SetBasePenStyle(m_pGannFan45Tool,nStyle);
		SetBasePenThickness(m_pGannFan45Tool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_FAN135)
	{
		SetBaseColor(m_pGannFan135Tool,crLine,crLine);
		SetBasePenStyle(m_pGannFan135Tool,nStyle);
		SetBasePenThickness(m_pGannFan135Tool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_FAN225)
	{
		SetBaseColor(m_pGannFan225Tool,crLine,crLine);
		SetBasePenStyle(m_pGannFan225Tool,nStyle);
		SetBasePenThickness(m_pGannFan225Tool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_FAN315)
	{
		SetBaseColor(m_pGannFan315Tool,crLine,crLine);
		SetBasePenStyle(m_pGannFan315Tool,nStyle);
		SetBasePenThickness(m_pGannFan315Tool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_GRID)
	{
		SetBaseColor(m_pGannGridTool,crLine,crLine);
		SetBasePenStyle(m_pGannGridTool,nStyle);
		SetBasePenThickness(m_pGannGridTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_ANDREWS_PITCHFORK)
	{
		SetBaseColor(m_pAndrewsTool,crLine,crLine);
		SetBasePenStyle(m_pAndrewsTool,nStyle);
		SetBasePenThickness(m_pAndrewsTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE)
	{
		SetBaseColor(m_pElliotLineTool,crLine,crLine);
		SetBasePenStyle(m_pElliotLineTool,nStyle);
		SetBasePenThickness(m_pElliotLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_CYCLE_LINES)
	{
		SetBaseColor(m_pCycleLinesTool,crLine,crLine);
		SetBasePenStyle(m_pCycleLinesTool,nStyle);
		SetBasePenThickness(m_pCycleLinesTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_CUT_LINE)
	{
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_SPEED_LINE)
	{
		SetBaseColor(m_pSpeedLineTool,crLine,crLine);
		SetBasePenStyle(m_pSpeedLineTool,nStyle);
		SetBasePenThickness(m_pSpeedLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_SPEED_FAN)
	{
		SetBaseColor(m_pSpeedFanTool,crLine,crLine);
		SetBasePenStyle(m_pSpeedFanTool,nStyle);
		SetBasePenThickness(m_pSpeedFanTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_SPEED_ARC)
	{
		SetBaseColor(m_pSpeedArcTool,crLine,crLine);
		SetBasePenStyle(m_pSpeedArcTool,nStyle);
		SetBasePenThickness(m_pSpeedArcTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION)
	{
		SetBaseColor(m_pLinearRegressionTool,crLine,crLine);
		SetBasePenStyle(m_pLinearRegressionTool,nStyle);
		SetBasePenThickness(m_pLinearRegressionTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_RAFF_REGRESSION)
	{
		SetBaseColor(m_pRaffRegressionTool,crLine,crLine);
		SetBasePenStyle(m_pRaffRegressionTool,nStyle);
		SetBasePenThickness(m_pRaffRegressionTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_ANGLE_LINE)
	{
		SetBaseColor(m_pAngleLineTool,crLine,crLine);
		SetBasePenStyle(m_pAngleLineTool,nStyle);
		SetBasePenThickness(m_pAngleLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_CANDLE_LINE)
	{
		SetBaseColor(m_pCandleLineTool,crLine,crLine);
		SetBasePenStyle(m_pCandleLineTool,nStyle);
		SetBasePenThickness(m_pCandleLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_CANDLE_BONG)
	{
		SetBaseColor(m_pCandleBongTool,crLine,crLine);
		SetBasePenStyle(m_pCandleBongTool,nStyle);
		SetBasePenThickness(m_pCandleBongTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_PENCIL_LINE)
	{
		SetBaseColor(m_pPencilLineTool,crLine,crLine);
		SetBasePenStyle(m_pPencilLineTool,nStyle);
		SetBasePenThickness(m_pPencilLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_PERIOD_SUMMARY)
	{
		SetBaseColor(m_pPeriodLineTool,crLine,crLine);
		SetBasePenStyle(m_pPeriodLineTool,nStyle);
		SetBasePenThickness(m_pPeriodLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE)
	{
		SetBaseColor(m_pElliotLineTool,crLine,crLine);
		SetBasePenStyle(m_pElliotLineTool,nStyle);
		SetBasePenThickness(m_pElliotLineTool,nWeight);
	}
	else if(eToolType == CAnalysis::EN_ANALYSIS_ADJUST_RATIO)
	{
		SetBaseColor(m_pAdjustRatioTool,crLine,crLine);
		SetBasePenStyle(m_pAdjustRatioTool,nStyle);
		SetBasePenThickness(m_pAdjustRatioTool,nWeight);
	}
	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
	else if(eToolType == CAnalysis::EN_ANALYSIS_BALANCE_BASIC)
	{
		SetBaseColor(m_pBalanceBasicTool,crLine,crLine);
		SetBasePenStyle(m_pBalanceBasicTool,nStyle);
		SetBasePenThickness(m_pBalanceBasicTool,nWeight);
	}
	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<
}

void CToolSingleton::SetTextPosition(const CAnalysis::TOOLTYPE eToolType, const bool bLeft, const bool bRight, const bool bTop, const bool bBottom, const bool bRightRate)
{
	// 십자선, 추세선, 수직선, 수평선, 삼등분선, 사등분선, 피보나치 되돌림, 
	if(eToolType == CAnalysis::EN_ANALYSIS_MOVE_CROSS && m_pCrossLineMoveTool != NULL)
		m_pCrossLineMoveTool->SetTextPosition(bLeft, bRight, bTop, bBottom);
	else if(eToolType == CAnalysis::EN_ANALYSIS_DOWN_CROSS && m_pCrossLineDownTool != NULL)
		m_pCrossLineDownTool->SetTextPosition(bLeft, bRight, bTop, bBottom);
	else if(eToolType == CAnalysis::EN_ANALYSIS_VERT_LINE && m_pVertLineTool != NULL)
		m_pVertLineTool->SetTextPosition(bTop, bBottom);
	else if(eToolType == CAnalysis::EN_ANALYSIS_HORZ_LINE && m_pHorzLineTool != NULL)
		m_pHorzLineTool->SetTextPosition(bLeft, bRight);
	else if( eToolType == CAnalysis::EN_ANALYSIS_LINE && m_pLineTool != NULL)
		m_pLineTool->SetTextPosition(bLeft, bRight, bRightRate);
	else if(eToolType == CAnalysis::EN_ANALYSIS_TRISECT && m_pTrisectionLineTool != NULL)
		m_pTrisectionLineTool->SetTextPosition(bLeft, bRight);
	else if(eToolType == CAnalysis::EN_ANALYSIS_QUARTER && m_pQuadrisectLineTool != NULL)
		m_pQuadrisectLineTool->SetTextPosition(bLeft, bRight);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_RETRACE && m_pFiboRetraceTool != NULL)
		m_pFiboRetraceTool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE && m_pFiboSemicircleTool != NULL)
		m_pFiboSemicircleTool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_CIRCLE && m_pFiboCircleTool != NULL)
		m_pFiboCircleTool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE && m_pFiboPenLineTool != NULL)
		m_pFiboPenLineTool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_FAN45 && m_pGannFan45Tool != NULL)
		m_pGannFan45Tool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_FAN135 && m_pGannFan135Tool != NULL)
		m_pGannFan135Tool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_FAN225 && m_pGannFan225Tool != NULL)
		m_pGannFan225Tool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_GANN_FAN315 && m_pGannFan315Tool != NULL)
		m_pGannFan315Tool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_ANGLE_LINE && m_pAngleLineTool != NULL)
		m_pAngleLineTool->SetTextPosition(bLeft);
	else if(eToolType == CAnalysis::EN_ANALYSIS_CANDLE_LINE && m_pCandleLineTool != NULL)
		m_pCandleLineTool->SetTextPosition(bLeft);
}

void CToolSingleton::SetTextEnvironment(const CAnalysis::TOOLTYPE eToolType, const CTextEnvironment& textEnvironment)
{
	// 십자선, 추세선, 수직선, 수평선, 삼등분선, 사등분선, 피보나치 되돌림, 
	if(eToolType == CAnalysis::EN_ANALYSIS_MOVE_CROSS && m_pCrossLineMoveTool != NULL)
		m_pCrossLineMoveTool->SetTextEnvironment(textEnvironment);
	else if(eToolType == CAnalysis::EN_ANALYSIS_DOWN_CROSS && m_pCrossLineDownTool != NULL)
		m_pCrossLineDownTool->SetTextEnvironment(textEnvironment);
	else if(eToolType == CAnalysis::EN_ANALYSIS_VERT_LINE && m_pVertLineTool != NULL)
		m_pVertLineTool->SetTextEnvironment(textEnvironment);
	else if(eToolType == CAnalysis::EN_ANALYSIS_HORZ_LINE && m_pHorzLineTool != NULL)
		m_pHorzLineTool->SetTextEnvironment(textEnvironment);

	else if( eToolType == CAnalysis::EN_ANALYSIS_LINE && m_pLineTool != NULL)
		m_pLineTool->SetTextEnvironment(textEnvironment);

	else if(eToolType == CAnalysis::EN_ANALYSIS_TRISECT && m_pTrisectionLineTool != NULL)
		m_pTrisectionLineTool->SetTextEnvironment(textEnvironment);
	else if(eToolType == CAnalysis::EN_ANALYSIS_QUARTER && m_pQuadrisectLineTool != NULL)
		m_pQuadrisectLineTool->SetTextEnvironment(textEnvironment);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_RETRACE && m_pFiboRetraceTool != NULL)
		m_pFiboRetraceTool->SetTextEnvironment(textEnvironment);
	else if(eToolType == CAnalysis::EN_ANALYSIS_ANGLE_LINE && m_pAngleLineTool != NULL)
		m_pAngleLineTool->SetTextEnvironment(textEnvironment);
	else if(eToolType == CAnalysis::EN_ANALYSIS_CANDLE_LINE && m_pCandleLineTool != NULL)
		m_pCandleLineTool->SetTextEnvironment(textEnvironment);
}

void CToolSingleton::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	// 십자선, 추세선, 수직선, 수평선, 삼등분선, 사등분선, 피보나치 되돌림, 
	if(m_pCrossLineMoveTool != NULL)
		m_pCrossLineMoveTool->SetTextEnvironment(textEnvironment);
	if(m_pCrossLineDownTool != NULL)
		m_pCrossLineDownTool->SetTextEnvironment(textEnvironment);
	if(m_pVertLineTool != NULL)
		m_pVertLineTool->SetTextEnvironment(textEnvironment);
	if(m_pHorzLineTool != NULL)
		m_pHorzLineTool->SetTextEnvironment(textEnvironment);
	if(m_pLineTool	!= NULL) 
		m_pLineTool->SetTextEnvironment(textEnvironment);
	if(m_pTrisectionLineTool != NULL)
		m_pTrisectionLineTool->SetTextEnvironment(textEnvironment);
	if(m_pQuadrisectLineTool != NULL)
		m_pQuadrisectLineTool->SetTextEnvironment(textEnvironment);
	if(m_pFiboRetraceTool != NULL)
		m_pFiboRetraceTool->SetTextEnvironment(textEnvironment);
	if(m_pAngleLineTool != NULL)
		m_pAngleLineTool->SetTextEnvironment(textEnvironment);
	if(m_pCandleLineTool != NULL)
		m_pCandleLineTool->SetTextEnvironment(textEnvironment);
}

void CToolSingleton::SetLineExtEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nLeftExt, const int& nRightExt)
{
	// 추세선, 삼등분선, 사등분선, 피보나치 되돌림, 
	if( eToolType == CAnalysis::EN_ANALYSIS_LINE)
		SetLineExtEnv(m_pLineTool,nLeftExt,nRightExt);
	else if(eToolType == CAnalysis::EN_ANALYSIS_TRISECT)
		SetLineExtEnv(m_pTrisectionLineTool,nLeftExt,nRightExt);
	else if(eToolType == CAnalysis::EN_ANALYSIS_QUARTER)
		SetLineExtEnv(m_pQuadrisectLineTool,nLeftExt,nRightExt);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_RETRACE)
		SetLineExtEnv(m_pFiboRetraceTool,nLeftExt,nRightExt);
	else if(eToolType == CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION)
		SetLineExtEnv(m_pLinearRegressionTool,nLeftExt,nRightExt);
	else if(eToolType == CAnalysis::EN_ANALYSIS_RAFF_REGRESSION)
		SetLineExtEnv(m_pLinearRegressionTool,nLeftExt,nRightExt);
}

void CToolSingleton::SetEquiDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nEquiDiv)
{
	if(eToolType == CAnalysis::EN_ANALYSIS_TRISECT)
		SetEquiDivEnv(m_pTrisectionLineTool,nEquiDiv);
	else if(eToolType == CAnalysis::EN_ANALYSIS_QUARTER)
		SetEquiDivEnv(m_pQuadrisectLineTool,nEquiDiv);
}

// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
void CToolSingleton::SetPriceRangeDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nPriceRangeDiv)
{
	if(eToolType == CAnalysis::EN_ANALYSIS_TRISECT)
		SetPriceRangeDivEnv(m_pTrisectionLineTool,nPriceRangeDiv);
	else if(eToolType == CAnalysis::EN_ANALYSIS_QUARTER)
		SetPriceRangeDivEnv(m_pQuadrisectLineTool,nPriceRangeDiv);
}

// 2008.08.20 김진순 피보나치 확장	<<
void CToolSingleton::SetFiboCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strFiboCoefficient)
{
	CString strCoefficient = strFiboCoefficient;
	if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE)	// 피보나치 반원
		SetFiboCoefficientEnv(m_pFiboSemicircleTool, strCoefficient);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_CIRCLE)	// 피보나치 원
		SetFiboCoefficientEnv(m_pFiboCircleTool, strCoefficient);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE)	// 피보나치 팬라인
		SetFiboCoefficientEnv(m_pFiboPenLineTool, strCoefficient);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_RETRACE)	// 피보나치 되돌림
		SetFiboCoefficientEnv(m_pFiboRetraceTool, strCoefficient);
	else if(eToolType == CAnalysis::EN_ANALYSIS_FIBO_TIME)		// 피보나치 시간대
		SetFiboCoefficientEnv(m_pFiboTimeTool, strCoefficient);
}
//>>

// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
void CToolSingleton::SetBalanceCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strBalanceCoefficient)
{
	CString strCoefficient = strBalanceCoefficient;
	if(eToolType == CAnalysis::EN_ANALYSIS_BALANCE_BASIC)	
		SetFiboCoefficientEnv(m_pFiboSemicircleTool, strCoefficient);
}
// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<

// private ====================================================================
void CToolSingleton::InitializeTool(CTool* pTool)
{
	if(pTool == NULL)
		return;

	pTool->Initialize();
}

void CToolSingleton::SetCoordinateType(CTool* pTool, const CAnalysis::COORDINATETYPE eCoordinateType)
{
	if(pTool == NULL)
		return;

	pTool->SetCoordinateType(eCoordinateType);
}

void CToolSingleton::SetAutoPrice(CTool* pTool, const bool bIsAutoPrice)
{
	if(pTool == NULL)
		return;

	pTool->SetAutoPrice(bIsAutoPrice);
}

void CToolSingleton::SetBaseColor(CTool* pTool, const COLORREF& penColor, const COLORREF& textColor)
{
	if(pTool == NULL)
		return;

	pTool->SetPenColor(penColor);
	pTool->SetTextColor(textColor);
}

void CToolSingleton::SetBasePenColor(CTool* pTool, const COLORREF& color)
{
	if(pTool == NULL)
		return;

	pTool->SetPenColor(color);
}

void CToolSingleton::SetBasePenThickness(CTool* pTool, const int nThickness)
{
	if(pTool == NULL)
		return;

	pTool->GetElementCommonEnvironment().GetPen().SetWidth(nThickness);
}

void CToolSingleton::SetBasePenStyle(CTool* pTool, const int nStyle)
{
	if(pTool == NULL)
		return;

	pTool->GetElementCommonEnvironment().GetPen().SetStyle((CToolEnumData::PenStyle) nStyle);
}

void CToolSingleton::SetLineExtEnv(CTool* pTool,const int& nLeftExt,const int& nRightExt)
{
	if(pTool == NULL)
		return;

	pTool->GetElementCommonEnvironment().SetLineExt(nLeftExt,nRightExt);
}

void CToolSingleton::SetEquiDivEnv(CTool* pTool,const int& nEquiDiv)
{
	if(pTool == NULL)
		return;

	pTool->GetElementCommonEnvironment().SetEquiDiv(nEquiDiv);
}

// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
void CToolSingleton::SetPriceRangeDivEnv(CTool* pTool,const int& nPriceRangeDiv)
{
	if(pTool == NULL)
		return;

	pTool->GetElementCommonEnvironment().SetPriceRangeDiv(nPriceRangeDiv);
}

// 2008.08.20 김진순 피보나치 확장	<<
void CToolSingleton::SetFiboCoefficientEnv(CTool* pTool,const CString& strFiboCoefficient)
{
	if(pTool == NULL)
		return;

	pTool->GetElementCommonEnvironment().GetDataType().SetExtData(strFiboCoefficient);
}

// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
void CToolSingleton::SetBalanceCoefficientEnv(CTool* pTool,const CString& strBalanceCoefficient)
{
	if(pTool == NULL)
		return;
	
	pTool->GetElementCommonEnvironment().GetDataType().SetExtData(strBalanceCoefficient);
}
// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<