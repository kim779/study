// ElementFactory.cpp: implementation of the CElementFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElementFactory.h"

#include <assert.h>
#include "Line.h"
#include "FiboGann.h"
#include "BalanceBasic.h"	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CElementFactory

CElement* CElementFactory::Copy(CElement* pElement)
{	
	CElementPointList elementPointList;
	pElement->GetPoints(elementPointList);
	if(elementPointList.GetCount() <= 0)
	{
		return NULL;
	}

	const CAnalysis::TOOLTYPE eToolType = (CAnalysis::TOOLTYPE)pElement->GetElementType();	
	CTextPosition *pPosition = pElement->GetTextPosition();
	CElementCommonEnvironment environment = pElement->GetCommonEnvironment();
	double dChannel = 0.0;
	switch(eToolType)
	{
		case CAnalysis::EN_ANALYSIS_MOVE_CROSS:
		//case CAnalysis::EN_ANALYSIS_DOWN_CROSS:
			return Make_HorzVertText(eToolType,(*elementPointList.GetAt(0)), (CTextHorzVertPosition*) pPosition, environment);
		case CAnalysis::EN_ANALYSIS_VERT_LINE:
			return Make_VertText(eToolType, (*elementPointList.GetAt(0)), (CTextVertPosition*) pPosition, environment);
		case CAnalysis::EN_ANALYSIS_HORZ_LINE:
			return Make_HorzText(eToolType, (*elementPointList.GetAt(0)), (CTextHorzPosition*) pPosition, environment);
		case CAnalysis::EN_ANALYSIS_CANDLE_LINE:
			return Make_ShowText(eToolType, (*elementPointList.GetAt(0)), (CTextShowPosition*) pPosition, environment);
		case CAnalysis::EN_ANALYSIS_FIBO_TIME:
			//return new CFiboTime((*elementPointList.GetAt(0)), environment);// 2008.08.20 김진순 피보나치 확장
			return Make_ShowText(eToolType, (*elementPointList.GetAt(0)), (CTextShowPosition*) pPosition, environment);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case CAnalysis::EN_ANALYSIS_LINE:
		case CAnalysis::EN_ANALYSIS_TRISECT:
		case CAnalysis::EN_ANALYSIS_QUARTER:
		case CAnalysis::EN_ANALYSIS_PERIOD_SUMMARY:
			return Make_HorzText(eToolType, (*elementPointList.GetAt(0)), (*elementPointList.GetAt(1)), (CTextHorzPosition*) pPosition, environment);
		case CAnalysis::EN_ANALYSIS_FIBO_RETRACE:
		case CAnalysis::EN_ANALYSIS_ANGLE_LINE:
		case CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE:
		case CAnalysis::EN_ANALYSIS_FIBO_CIRCLE:
		case CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE:					
		case CAnalysis::EN_ANALYSIS_GANN_FAN45:
		case CAnalysis::EN_ANALYSIS_GANN_FAN135:
		case CAnalysis::EN_ANALYSIS_GANN_FAN225:
		case CAnalysis::EN_ANALYSIS_GANN_FAN315:
			return Make_ShowText(eToolType, (*elementPointList.GetAt(0)), (*elementPointList.GetAt(1)), (CTextShowPosition*) pPosition, environment);
		case CAnalysis::EN_ANALYSIS_GANN_LINE45:
		case CAnalysis::EN_ANALYSIS_GANN_LINE315:
		case CAnalysis::EN_ANALYSIS_GANN_GRID:
		case CAnalysis::EN_ANALYSIS_CYCLE_LINES:
		case CAnalysis::EN_ANALYSIS_SPEED_LINE:
		case CAnalysis::EN_ANALYSIS_SPEED_FAN:
		case CAnalysis::EN_ANALYSIS_SPEED_ARC:
		case CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION:
		case CAnalysis::EN_ANALYSIS_RAFF_REGRESSION:
		case CAnalysis::EN_ANALYSIS_CANDLE_BONG:		
		case CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE:		
			return Make_NoText(eToolType, (*elementPointList.GetAt(0)), (*elementPointList.GetAt(1)), environment,dChannel);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case CAnalysis::EN_ANALYSIS_ANDREWS_PITCHFORK:
			return new CAndrewsPitchfork((*elementPointList.GetAt(0)), (*elementPointList.GetAt(1)), (*elementPointList.GetAt(2)), environment);	
		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
		case CAnalysis::EN_ANALYSIS_BALANCE_BASIC:
			return Make_ShowText(eToolType, (*elementPointList.GetAt(0)), (CTextShowPosition*) pPosition, environment);
		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case CAnalysis::EN_ANALYSIS_PENCIL_LINE:	//2006/12/01 - 현재 연필추세선은 복사 되지 않는다
			return NULL;	
	}
	
	assert(false);
	return NULL;
}

// 객체를 생성한다. --> 한개의 point
CElement* CElementFactory::Make(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point, 
								const CTextPosition* pPosition, const CElementCommonEnvironment& environment)
{
	switch(eToolType){
	case CAnalysis::EN_ANALYSIS_MOVE_CROSS:
	//case CAnalysis::EN_ANALYSIS_DOWN_CROSS:
		return Make_HorzVertText(eToolType, point, (CTextHorzVertPosition*) pPosition, environment);
	case CAnalysis::EN_ANALYSIS_VERT_LINE:
		return Make_VertText(eToolType, point, (CTextVertPosition*) pPosition, environment);
	case CAnalysis::EN_ANALYSIS_HORZ_LINE:
		return Make_HorzText(eToolType, point, (CTextHorzPosition*) pPosition, environment);
	case CAnalysis::EN_ANALYSIS_CANDLE_LINE:
		return Make_ShowText(eToolType, point, (CTextShowPosition*) pPosition, environment);
	case CAnalysis::EN_ANALYSIS_FIBO_TIME:
		//return new CFiboTime(point, environment);// 2008.08.20 김진순 피보나치 확장
		return Make_ShowText(eToolType, point, (CTextShowPosition*) pPosition, environment);
	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
	case CAnalysis::EN_ANALYSIS_BALANCE_BASIC:		
		return Make_ShowText(eToolType, point, (CTextShowPosition*) pPosition, environment);
	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<
	}

	assert(false);
	return NULL;
}

// --> 두개의 point저장
CElement* CElementFactory::Make(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, 
								const CCoordinate& point2, const CTextPosition* pPosition, 
								const CElementCommonEnvironment& environment,double dChannel)
{
	switch(eToolType)
	{
		case CAnalysis::EN_ANALYSIS_LINE:
		case CAnalysis::EN_ANALYSIS_TRISECT:
		case CAnalysis::EN_ANALYSIS_QUARTER:
		case CAnalysis::EN_ANALYSIS_PERIOD_SUMMARY:
			return Make_HorzText(eToolType, point1, point2, (CTextHorzPosition*) pPosition, environment);
		case CAnalysis::EN_ANALYSIS_FIBO_RETRACE:
		case CAnalysis::EN_ANALYSIS_ANGLE_LINE:
		case CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE:
		case CAnalysis::EN_ANALYSIS_FIBO_CIRCLE:
		case CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE:
		case CAnalysis::EN_ANALYSIS_GANN_FAN45:
		case CAnalysis::EN_ANALYSIS_GANN_FAN135:
		case CAnalysis::EN_ANALYSIS_GANN_FAN225:
		case CAnalysis::EN_ANALYSIS_GANN_FAN315:
			return Make_ShowText(eToolType, point1, point2, (CTextShowPosition*) pPosition, environment);
		case CAnalysis::EN_ANALYSIS_GANN_LINE45:
		case CAnalysis::EN_ANALYSIS_GANN_LINE315:		
		case CAnalysis::EN_ANALYSIS_GANN_GRID:
		case CAnalysis::EN_ANALYSIS_CYCLE_LINES:
		case CAnalysis::EN_ANALYSIS_SPEED_LINE:
		case CAnalysis::EN_ANALYSIS_SPEED_FAN:
		case CAnalysis::EN_ANALYSIS_SPEED_ARC:
		case CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION:
		case CAnalysis::EN_ANALYSIS_RAFF_REGRESSION:
		case CAnalysis::EN_ANALYSIS_CANDLE_BONG:		
		case CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE:		
			return Make_NoText(eToolType, point1, point2, environment,dChannel);
	}

	assert(false);
	return NULL;
}

// --> 세개의 point저장
CElement* CElementFactory::Make(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, 
								const CCoordinate& point2, const CCoordinate& point3, 
		const CTextPosition* /*pPosition*/, const CElementCommonEnvironment& environment)
{
	switch(eToolType){
	case CAnalysis::EN_ANALYSIS_ANDREWS_PITCHFORK:
		return new CAndrewsPitchfork(point1, point2, point3, environment);
	}

	assert(false);
	return NULL;
}

CElement* CElementFactory::Make(const CAnalysis::TOOLTYPE eToolType,int nPointsCount,const CPoints* pPointsList,
								const CElementCommonEnvironment& environment)
{
	if(eToolType == CAnalysis::EN_ANALYSIS_PENCIL_LINE)
		return new CPencilLine(nPointsCount,pPointsList,environment);

	assert(false);
	return NULL;
}

CElement* CElementFactory::Make(const CAnalysis::TOOLTYPE eToolType,const CDequeList<CCoordinate>& pointList,const CElementCommonEnvironment& environment)
{
	if(eToolType == CAnalysis::EN_ANALYSIS_PENCIL_LINE)
	{	
		return new CPencilLine(pointList,environment);
	}

	assert(false);
	return NULL;
}

// 객체를 삭제한다. Make에서 new를 했으므로..
void CElementFactory::Delete(CElement* pElement)
{
	delete pElement;
}

// private ====================================================================
// --> 한개의 point
CElement* CElementFactory::Make_HorzVertText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point, 
											 const CTextHorzVertPosition* pPosition, const CElementCommonEnvironment& environment)
{
	assert(pPosition != NULL);

	switch(eToolType){
	case CAnalysis::EN_ANALYSIS_MOVE_CROSS:
	case CAnalysis::EN_ANALYSIS_DOWN_CROSS:
		return new CCrossLine(*pPosition, point, environment);
	}

	assert(false);
	return NULL;
}

CElement* CElementFactory::Make_VertText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point, 
										 const CTextVertPosition* pPosition, const CElementCommonEnvironment& environment)
{
	assert(pPosition != NULL);

	switch(eToolType){
	case CAnalysis::EN_ANALYSIS_VERT_LINE:
		return new CVertLine(*pPosition, point, environment);
	}

	assert(false);
	return NULL;
}

CElement* CElementFactory::Make_HorzText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point, 
										 const CTextHorzPosition* pPosition, const CElementCommonEnvironment& environment)
{
	assert(pPosition != NULL);

	switch(eToolType){
	case CAnalysis::EN_ANALYSIS_HORZ_LINE:
		return new CHorzLine(*pPosition, point, environment);
	}

	assert(false);
	return NULL;
}

// ----------------------------------------------------------------------------
// --> 두개의 point저장
CElement* CElementFactory::Make_ShowText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, 
										 const CCoordinate& point2, const CTextShowPosition* pPosition, const CElementCommonEnvironment& environment)
{
	assert(pPosition != NULL);

	switch(eToolType){
	case CAnalysis::EN_ANALYSIS_FIBO_RETRACE:
		return new CFiboRetrace(*pPosition, point1, point2, environment);
	case CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE:
		return new CFiboSemicircle(*pPosition,point1, point2, environment);
	case CAnalysis::EN_ANALYSIS_FIBO_CIRCLE:
		return new CFiboCircle(*pPosition,point1, point2, environment);
	case CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE:
		return new CFiboPenLine(*pPosition,point1, point2, environment);
	case CAnalysis::EN_ANALYSIS_ANGLE_LINE:
		return new CAngleLine(*pPosition, point1, point2, environment);	
	case CAnalysis::EN_ANALYSIS_GANN_FAN45:
	case CAnalysis::EN_ANALYSIS_GANN_FAN135:
	case CAnalysis::EN_ANALYSIS_GANN_FAN225:
	case CAnalysis::EN_ANALYSIS_GANN_FAN315:
		return new CGannFan(*pPosition,point1, point2, environment,(eToolType-CAnalysis::EN_ANALYSIS_GANN_FAN45));
	}

	assert(false);
	return NULL;
}

CElement* CElementFactory::Make_ShowText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point,const CTextShowPosition* pPosition, const CElementCommonEnvironment& environment)
{
//	assert(pPosition != NULL);

	switch(eToolType){
	case CAnalysis::EN_ANALYSIS_CANDLE_LINE:
		assert(pPosition != NULL);
		return new CCandleLine(*pPosition, point, environment);	
	// 2008.08.20 김진순 피보나치 확장 <<
	case CAnalysis::EN_ANALYSIS_FIBO_TIME:
		if( pPosition )
			return new CFiboTime(*pPosition, point, environment);
		else
			return new CFiboTime(point, environment);
	//>>
		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
	case CAnalysis::EN_ANALYSIS_BALANCE_BASIC:
		return new CBalanceBasic(*pPosition, point, environment);
		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<
	}

	assert(false);
	return NULL;
}

CElement* CElementFactory::Make_HorzText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, const CCoordinate& point2, 
										 const CTextHorzPosition* pPosition, const CElementCommonEnvironment& environment)
{
	assert(pPosition != NULL);

	switch(eToolType)
	{
		case CAnalysis::EN_ANALYSIS_LINE:
			return new CLine(*pPosition, point1, point2, eToolType, environment);
		case CAnalysis::EN_ANALYSIS_TRISECT:
			return new CTrisectionLine(*pPosition, point1, point2, environment);
		case CAnalysis::EN_ANALYSIS_QUARTER:
			return new CQuadrisectLine(*pPosition, point1, point2, environment);
		case CAnalysis::EN_ANALYSIS_PERIOD_SUMMARY:
			return new CPeriodSumLine(*pPosition, point1, point2, environment);
	}

	assert(false);
	return NULL;
}

CElement* CElementFactory::Make_NoText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, 
									   const CCoordinate& point2, const CElementCommonEnvironment& environment,double dChannel)
{
	switch(eToolType){	
	case CAnalysis::EN_ANALYSIS_GANN_LINE45:
	case CAnalysis::EN_ANALYSIS_GANN_LINE315:
		return new CGannLine(point1, point2, environment,(eToolType-CAnalysis::EN_ANALYSIS_GANN_LINE45));	
	case CAnalysis::EN_ANALYSIS_GANN_GRID:
		return new CGannGrid(point1, point2, environment);
	case CAnalysis::EN_ANALYSIS_CYCLE_LINES:
		return new CCycleLines(point1, point2, environment);
	case CAnalysis::EN_ANALYSIS_SPEED_LINE:
		return new CSpeedLine(point1, point2, environment);	
	case CAnalysis::EN_ANALYSIS_SPEED_FAN:
		return new CSpeedFan(point1, point2, environment);	
	case CAnalysis::EN_ANALYSIS_SPEED_ARC:
		return new CSpeedArc(point1, point2, environment);		
	case CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION:
		return new CLinearRegression(point1, point2, environment);		
	case CAnalysis::EN_ANALYSIS_RAFF_REGRESSION:
		return new CRaffRegression(point1, point2, environment,dChannel);	
	case CAnalysis::EN_ANALYSIS_CANDLE_BONG:
		return new CCandleBong(point1, point2, environment);	
	case CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE:
		return new CElliotLine(point1, point2, environment);	
	}

	assert(false);
	return NULL;
}
