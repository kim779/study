// Random.cpp: implementation of the CRandom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "Random.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRandom::CRandom()
{
	m_strName = "RANDOM";
}

CRandom::~CRandom()
{

}

CString CRandom::GetInputs(long lType)
{
	return "Numeric";
}

double CRandom::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lValue = _variant_t(varArg1);
	srand( (unsigned)time( NULL ) );	
	return (rand()%(lValue+1));
}
