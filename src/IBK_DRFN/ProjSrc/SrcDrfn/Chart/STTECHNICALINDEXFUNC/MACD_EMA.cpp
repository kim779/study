// MACD_EMA.cpp: implementation of the CMACD_EMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MACD_EMA.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMACD_EMA::CMACD_EMA()
{
	m_strName = "MACD_EMA";

}

CMACD_EMA::~CMACD_EMA()
{

}

CString CMACD_EMA::GetInputs(long lType)
{
	return "";
}


double CMACD_EMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	return 0;	
}
