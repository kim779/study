// VROC.cpp: implementation of the CVROC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "VROC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVROC::CVROC()
{
	m_strName = "VROC";
}

CVROC::~CVROC()
{

}

CString CVROC::GetInputs(long lType)
{
	return "Numeric";
}

double CVROC::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	return yesVROC(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWVOLUMEDATA)
		,pIController->GetITempMemMng());
}

