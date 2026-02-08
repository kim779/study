// BandWidth.cpp: implementation of the CBandWidth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "BandWidth.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBandWidth::CBandWidth()
{
	m_strName = "BANDWIDTH";
}

CBandWidth::~CBandWidth()
{

}

CString CBandWidth::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.21
 Modifier		: 
 Comments		: {{{ Band width = (upper band - lower band) / middle band * 100 }}}
-----------------------------------------------------------------------------*/
double CBandWidth::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	double dStdDev = _variant_t(varArg2);
	double dValue = _variant_t(varArg3);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);	
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesBandWidth(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,dStdDev
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}
