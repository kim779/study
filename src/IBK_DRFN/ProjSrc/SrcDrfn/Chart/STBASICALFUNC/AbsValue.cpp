// AbsValue.cpp: implementation of the CAbsValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbsValue.h"
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbsValue::CAbsValue()
{
	m_strName0 = "ABSVALUE";
	m_strName1 = "ABS";
}

CAbsValue::~CAbsValue()
{

}

CString CAbsValue::GetName(long lType)
{
	switch(lType)
	{
	case YES_LANG: 
		return m_strName1; break;
	default:// TS_LANG
		return m_strName0;	break;
	}
}

CString CAbsValue::GetInputs(long lType)
{
	return "Numeric";
}

double CAbsValue::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	return fabs(dValue);
}
