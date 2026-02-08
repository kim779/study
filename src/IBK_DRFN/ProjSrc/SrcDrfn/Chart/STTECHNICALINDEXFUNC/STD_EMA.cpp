// STD_EMA.cpp: implementation of the CSTD_EMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STD_EMA.h"
#include "indi_func.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTD_EMA::CSTD_EMA()
{
	m_strName = "STD_EMA";

}

CSTD_EMA::~CSTD_EMA()
{

}
CString CSTD_EMA::GetInputs(long lType)
{
	return "";
}


double CSTD_EMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	return 0;
}