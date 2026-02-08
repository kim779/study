// Pie.cpp: implementation of the CPie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pie.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPie::CPie()
{
	m_strName = "PIE";
}

CPie::~CPie()
{

}

CString CPie::GetInputs(long lType)
{
	return "";
}

double CPie::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	return 3.14159265358979323846;
}
