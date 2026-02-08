// AroonUp.cpp: implementation of the CAroonUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "AroonUp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAroonUp::CAroonUp()
{
		m_strName = "AROONUP";
}

CAroonUp::~CAroonUp()
{

}

CString CAroonUp::GetInputs(long lType)
{
	return "Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.5
 Modifier		: 
 Comments		: {{{ 고가유지기간 : 고가의 최고가가 고가인 날과 당일 일수 차 }}}
				  {{{ Aroon Up =  (N-고가유지기간)/N*100 }}}
-----------------------------------------------------------------------------*/
double CAroonUp::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	return yesAroonUp(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetITempMemMng());
}
