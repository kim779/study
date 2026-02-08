// PriceChLower.cpp: implementation of the CPriceChLower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "PriceChLower.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPriceChLower::CPriceChLower()
{
	m_strName = "PRICECHLOWER";
}

CPriceChLower::~CPriceChLower()
{

}

CString CPriceChLower::GetInputs(long lType)
{
	return "Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ lower channel = 전일 기준 n일 간의 저가의 최소값 }}}
-----------------------------------------------------------------------------*/
double CPriceChLower::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	return yesPriceChLower(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetITempMemMng());

}
