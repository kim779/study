// BRatio.cpp: implementation of the CBRatio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BRatio.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBRatio::CBRatio()
{
	m_strName = "B_RATIO";
}

CBRatio::~CBRatio()
{

}

CString CBRatio::GetInputs(long lType)
{
	return "Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.5
 Modifier		: 
 Comments		: {{{ 강인기 = 당일고가 - 전일종가 }}}
				  {{{ 약인기 = 전일종가 - 당일저가 }}}
				  {{{ BR = [ n 일간의 강인기 합/ n일간의  약인기 합 ] * 100 }}}
-----------------------------------------------------------------------------*/
double CBRatio::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries, 0);		
		pIController->SetVariables(1,"Value2",NumericSeries, 0);	
	}
	return yesBRatio(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());
}

