// FracPortion.cpp: implementation of the CFracPortion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "FracPortion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFracPortion::CFracPortion()
{
	m_strName = "FRACPORTION";
}

CFracPortion::~CFracPortion()
{

}

CString CFracPortion::GetInputs(long lType)
{
	return "Numeric";
}

double CFracPortion::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	double dIntiger = 0;
	return modf( dValue, &dIntiger);
}
