// SinH.cpp: implementation of the CSinH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "SinH.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSinH::CSinH()
{
	m_strName = "SINH";
}

CSinH::~CSinH()
{

}

CString CSinH::GetInputs(long lType)
{
	return "Numeric";
}

double CSinH::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	return sinh(dValue * 3.14159265358979323846 / 180);
}
