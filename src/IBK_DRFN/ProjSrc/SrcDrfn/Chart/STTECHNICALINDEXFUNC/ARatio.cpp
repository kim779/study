// ARatio.cpp: implementation of the CARatio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARatio.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARatio::CARatio()
{
	m_strName = "A_RATIO";
}

CARatio::~CARatio()
{

}

CString CARatio::GetInputs(long lType)
{
	return "Numeric";
}

/*--------------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.4
 Modifier		: 
 Comments		: {{{ 강에너지 = 당일고가 - 당일시가 }}}
				  {{{ 약에너지 = 당일시가 - 당일저가 }}}
				  {{{ AR = [ n 일간의 강에너지 합/ n일간의  약에너지 합 ] * 100 }}}
----------------------------------------------------------------------------------*/
double CARatio::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries, 0);		
		pIController->SetVariables(1,"Value2",NumericSeries, 0);	
	}
	return yesARatio(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetRawData(RAWOPENDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());
}
