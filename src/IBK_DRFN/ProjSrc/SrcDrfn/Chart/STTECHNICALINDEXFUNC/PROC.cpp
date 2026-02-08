// PROC.cpp: implementation of the CPROC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Include_ST/STControllerDef.h"
#include "PROC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPROC::CPROC()
{
	m_strName = "PROC";
}

CPROC::~CPROC()
{

}

CString CPROC::GetInputs(long lType)
{
	return "Numeric";
}

double CPROC::Func(CISTControllerD *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	return yesPROC(pIController->IsUpdateData()
		,pIController->BarIndex()
		, lPeriod
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetITempMemMng());

}

