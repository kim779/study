// CSar.cpp: implementation of the CCSar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "CSar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCSar::CCSar()
{
	m_strName = "CSAR";
}

CCSar::~CCSar()
{

}

CString CCSar::GetInputs(long lType)
{
	return "Numeric,Numeric";
}

double CCSar::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dAF = _variant_t(varArg1);
	double dMaxAF = _variant_t(varArg2);
	return yesCSar(pIController->IsUpdateData()
				,pIController->BarIndex()
				,dAF, dMaxAF
				,pIController->GetRawData(RAWHIGHDATA)
				,pIController->GetRawData(RAWLOWDATA)
				,pIController->GetRawData(RAWCLOSEDATA)
				,pIController->GetITempMemMng());
}

