// Sonar_EMA.cpp: implementation of the CSonar_EMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sonar_EMA.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSonar_EMA::CSonar_EMA()
{
	m_strName = "Sonar_EMA";
}

CSonar_EMA::~CSonar_EMA()
{

}
CString CSonar_EMA::GetInputs(long lType)
{
//	return "Numeric, Numeric=close";
	return "";
}

double CSonar_EMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	return 0;
}