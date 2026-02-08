// JulianToDate.cpp: implementation of the CJulianToDate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "JulianToDate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJulianToDate::CJulianToDate()
{
	m_strName = "JULIANTODATE";
}

CJulianToDate::~CJulianToDate()
{

}

CString CJulianToDate::GetName(long lType)
{
	return m_strName;
}

CString CJulianToDate::GetInputs(long lType)
{
	return "Numeric";
}

double CJulianToDate::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lDateValue = _variant_t(varArg1);
	
	int i;
	int liJulian;
	int liYear = 0;
	int liMonth = 0;
	int liDay = 0;
	int lipLastDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (lDateValue <= 0) return 0;

	liJulian = 0;	
	for ( i = 1900; i < 3000; i++)
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
			if (liJulian + 365 >= lDateValue)
			{
				break;
			}
			else
			{
				liJulian += 365;
			}
		}
		else
		{
			if (liJulian + 366 >= lDateValue)
			{
				break;
			}
			else
			{
				liJulian += 366;
			}
		}
	}
	
	liYear = i;

	for (i = 1; i <= 12; i++)
	{
		if (liJulian + lipLastDay[i - 1] >= lDateValue)
		{
			break;
		}
		else
		{
			liJulian += lipLastDay[i - 1];
		}
	}

	liMonth = i;

	liDay = lDateValue - liJulian;

	return liYear * 10000 + liMonth * 100 + liDay;
}
