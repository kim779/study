// PDI.cpp: implementation of the CPDI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "PDI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPDI::CPDI()
{
	m_strName = "PDI";
}

CPDI::~CPDI()
{

}


CString CPDI::GetInputs(long lType)
{
	return "Numeric";
}


/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: {{{ PDI = PDMn / TRn * 100 }}}
				  {{{ PDMn = PDM의 n일 이동평균 }}}
-----------------------------------------------------------------------------*/
double CPDI::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod1 = _variant_t(varArg1);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"OptVar1",NumericSeries,0);
		pIController->SetVariables(1,"OptVar2",NumericSeries,0);
		pIController->SetVariables(2,"OptVar3",NumericSeries,0);
		pIController->SetVariables(3,"OptVar4",NumericSeries,0);
	}
	return yesPDI(pIController->IsUpdateData()
		,pIController->BarIndex() ,lPeriod1 
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)	
		,pIController->GetRawData(RAWCLOSEDATA)	
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetVarHolderDataItem(3)
		,pIController->GetITempMemMng());
}
