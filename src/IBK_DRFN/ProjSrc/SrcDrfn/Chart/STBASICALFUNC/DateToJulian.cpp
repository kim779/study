// DateToJulian.cpp: implementation of the CDateToJulian class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "DateToJulian.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDateToJulian::CDateToJulian()
{
	m_strName = "DATETOJULIAN";
}

CDateToJulian::~CDateToJulian()
{

}

CString CDateToJulian::GetName(long lType)
{
	return m_strName;
}

CString CDateToJulian::GetInputs(long lType)
{
	return "Numeric";
}

double CDateToJulian::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lDateValue = _variant_t(varArg1);
	
	int i;
	int liJulian;
	int liYear;
	int liMonth;
	int liDay;
	int liTmp;
	int lipLastDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (lDateValue < 19000101) return 0.;

	liTmp = lDateValue;
	liYear = lDateValue / 10000;
	liTmp = lDateValue - (liYear * 10000);
	liMonth = liTmp / 100;
	liTmp = lDateValue - (liYear * 10000) - (liMonth * 100);
	liDay = liTmp;

	liJulian = 0;
	for ( i = 1900; i < liYear; i++)
	{
		if (i % 400 == 0 || 
			((i % 100) != 0 && (i % 4) == 0))
		{
			lipLastDay[1] = 29;
		}
		else
		{
			lipLastDay[1] = 28;
		}

		if (lipLastDay[1] == 28)
		{
			liJulian += 365;
		}
		else
		{
			liJulian += 366;
		}
	}

	if (i % 400 == 0 || 
		((i % 100) != 0 && (i % 4) == 0))
	{
		lipLastDay[1] = 29;
	}
	else
	{
		lipLastDay[1] = 28;
	}

	for (i = 1; i < liMonth; i++)
	{
		liJulian += lipLastDay[i - 1];
	}

	liJulian += liDay;

	return liJulian;
}
