// ToolSingleton.h: interface for the CToolSingleton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSINGLETON_H__D89CC208_663A_4BA1_94C1_205CC118F006__INCLUDED_)
#define AFX_TOOLSINGLETON_H__D89CC208_663A_4BA1_94C1_205CC118F006__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/BlockBaseEnum2.h"			// for CAnalysis
#include "BlockBaseData.h"

class CAnalysisData;
class CLineExtEnvironment;
class CTextEnvironment;
class CTool;
class CSelectingTool;
class CCrossLineMoveTool;
class CCrossLineDownTool;
class CVertLineTool;
class CHorzLineTool;
class CLineTool;
class CTrisectionLineTool;
class CQuadrisectLineTool;
class CFiboSemicircleTool;
class CFiboCircleTool;
class CFiboPenLineTool;
class CFiboRetraceTool;
class CFiboTimeTool;
class CGannLine45Tool;
class CGannLine315Tool;
class CGannFan45Tool;
class CGannFan135Tool;
class CGannFan225Tool;
class CGannFan315Tool;
class CGannGridTool;
class CAndrewsPitchforkTool;
class CCycleLinesTool;
class CSpeedLineTool;
class CSpeedFanTool;
class CSpeedArcTool;
class CLinearRegressionTool;
class CRaffRegressionTool;
class CAngleLineTool;
class CCandleLineTool;
class CCandleBongTool;
class CPencilLineTool;
class CPeriodSumLineTool;
class CElliotLineTool;
class CAdjustRatioTool;
class CBalanceBasicTool;	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청

class CToolSingleton  
{
public:
	CToolSingleton(CAnalysisData* pAnalysisData);
	~CToolSingleton();
	
	CTool* FindTool(const CAnalysis::TOOLTYPE eToolType);

	void InitializeAllTool();

	void SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType);
	void SetAutoPrice(const bool bIsAutoPrice);
	void SetBaseColor(const COLORREF& penColor, const COLORREF& textColor);
	void SetBasePenColor(const COLORREF& color);
	void SetBasePenThickness(const int nThickness);
	
	//by jwp(2006/09/29)--	 
	void SetLineExtEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nLeftExt, const int& nRightExt);
	void SetLineExtEnvironment(const int& nLeftExt, const int& nRightExt);

	void SetEquiDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nEquiDiv);
	void SetPriceRangeDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nPriceRangeDiv);	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	void SetLineDrawingInfo(const CAnalysis::TOOLTYPE eToolType, const COLORREF crLine, const int nStyle, const int nWeight);
	void SetTextPosition(const CAnalysis::TOOLTYPE eToolType, const bool bLeft, const bool bRight, const bool bTop, const bool bBottom, const bool bRightRate=true);
	void SetTextEnvironment(const CAnalysis::TOOLTYPE eToolType, const CTextEnvironment& textEnvironment);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);
	void SetFiboCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strFiboCoefficient);//// 2008.08.20 김진순 피보나치 확장	
	void SetBalanceCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strFiboCoefficient); // 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청

private:
	CAnalysis::COORDINATETYPE m_eCoordinateType;

	CSelectingTool* m_pSelectingTool;
	CCrossLineMoveTool* m_pCrossLineMoveTool;
	CCrossLineDownTool* m_pCrossLineDownTool;
	CVertLineTool* m_pVertLineTool;
	CHorzLineTool* m_pHorzLineTool;

	CLineTool *	m_pLineTool;

	CTrisectionLineTool* m_pTrisectionLineTool;
	CQuadrisectLineTool* m_pQuadrisectLineTool;
	CFiboSemicircleTool* m_pFiboSemicircleTool;
	CFiboCircleTool* m_pFiboCircleTool;
	CFiboPenLineTool* m_pFiboPenLineTool;
	CFiboRetraceTool* m_pFiboRetraceTool;
	CFiboTimeTool* m_pFiboTimeTool;
	CGannLine45Tool* m_pGannLine45Tool;
	CGannLine315Tool* m_pGannLine315Tool;
	CGannFan45Tool* m_pGannFan45Tool;
	CGannFan135Tool* m_pGannFan135Tool;
	CGannFan225Tool* m_pGannFan225Tool;
	CGannFan315Tool* m_pGannFan315Tool;
	CGannGridTool* m_pGannGridTool;
	CAndrewsPitchforkTool* m_pAndrewsTool;
	CCycleLinesTool* m_pCycleLinesTool;
	CSpeedLineTool* m_pSpeedLineTool;
	CSpeedFanTool* m_pSpeedFanTool;
	CSpeedArcTool* m_pSpeedArcTool;
	CLinearRegressionTool* m_pLinearRegressionTool;
	CRaffRegressionTool* m_pRaffRegressionTool;
	CAngleLineTool* m_pAngleLineTool;
	CCandleLineTool* m_pCandleLineTool;
	CCandleBongTool* m_pCandleBongTool;
	CPencilLineTool* m_pPencilLineTool;
	CPeriodSumLineTool* m_pPeriodLineTool;
	CElliotLineTool* m_pElliotLineTool;
	CAdjustRatioTool* m_pAdjustRatioTool;
	CBalanceBasicTool* m_pBalanceBasicTool;		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청

	void InitializeTool(CTool* pTool);
	void SetEquiDivEnv(CTool* pTool,const int& nEquiDiv);
	void SetPriceRangeDivEnv(CTool* pTool,const int& nPriceRangeDiv);	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	void SetLineExtEnv(CTool* pTool,const int& bLeftExt,const int& nRightExt);
	void SetCoordinateType(CTool* pTool, const CAnalysis::COORDINATETYPE eCoordinateType);
	void SetAutoPrice(CTool* pTool, const bool bIsAutoPrice);
	void SetBaseColor(CTool* pTool, const COLORREF& penColor, const COLORREF& textColor);
	void SetBasePenColor(CTool* pTool, const COLORREF& color);
	void SetBasePenThickness(CTool* pTool, const int nThickness);
	void SetBasePenStyle(CTool* pTool, const int nStyle);
	void SetFiboCoefficientEnv(CTool* pTool,const CString& strFiboCoefficient);//// 2008.08.20 김진순 피보나치 확장	
	void SetBalanceCoefficientEnv(CTool* pTool,const CString& strBalanceCoefficient); // 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
};

#endif // !defined(AFX_TOOLSINGLETON_H__D89CC208_663A_4BA1_94C1_205CC118F006__INCLUDED_)
