// SROC.cpp: implementation of the CSROC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "SROC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSROC::CSROC()
{
	m_strName = "SROC";
}

CSROC::~CSROC()
{

}

CString CSROC::GetInputs(long lType)
{
//	return "Numeric, Numeric, Numeric=close";
	return "Numeric, Numeric, Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.7
 Modifier		: 
 Comments		: {{{ SROC = 당일 EMA / n일전EMA*100 }}}
-----------------------------------------------------------------------------*/
double CSROC::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod1 = 	_variant_t(varArg1);
	long lPeriod2 = 	_variant_t(varArg2);
	double dValue =		_variant_t(varArg3);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);		
		pIController->SetVariables(1,"Opt1",NumericSeries,0);		
		pIController->SetVariables(2,"Opt2",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesSROC(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod1 ,lPeriod2
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetITempMemMng());
}
