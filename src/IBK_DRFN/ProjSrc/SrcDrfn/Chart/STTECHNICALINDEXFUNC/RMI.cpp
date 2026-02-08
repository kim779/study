// RMI.cpp: implementation of the CRMI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "RMI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMI::CRMI()
{
	m_strName = "RMI";
}

CRMI::~CRMI()
{

}

CString CRMI::GetInputs(long lType)
{
//	return "Numeric, Numeric, Numeric=Close";
	return "Numeric, Numeric, Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.11
 Modifier		: 
 Comments		: {{{ RMI = 100 - 100 / (1+ »ó½Â¸ð¸àÅÒÆò±Õ/ÇÏ¶ô¸ð¸àÅÒÆò±Õ) }}}
-----------------------------------------------------------------------------*/
double CRMI::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	long lKPeriod = _variant_t(varArg2);
	double dValue = _variant_t(varArg3);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);
		pIController->SetVariables(1,"Value2", NumericSeries,0);		
		pIController->SetVariables(2,"Value3", NumericSeries,0);		
	}	
	pIController->SetVarHolderData(0,0,dValue);	
	return yesRMI(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod, lKPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetITempMemMng());
}
