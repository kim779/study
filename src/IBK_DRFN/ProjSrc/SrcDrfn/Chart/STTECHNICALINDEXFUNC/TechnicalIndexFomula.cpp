//#ifndef __TECHNICALINDEXFOMULA
//#define __TECHNICALINDEXFOMULA

#include <math.h>

#define CLIENTST_VERSION
/* Server */
#ifdef SERVERST_VERSION
#define IUNKNOWNDATAITEM	int
#define GET_AT(x,y)			GetAt(x,y)
#define SET_AT(x,y,z)		SetAt(x,y,z)
#endif

/* Client */
#ifdef CLIENTST_VERSION
#define IUNKNOWNDATAITEM	CIDataItem*
#define GET_AT(x,y)			(x)->GetAt(y)
#define SET_AT(x,y,z)		(x)->SetAt(y,z)
#define ITEMPITEM			CITempItem*
#define GET_SUBTEMP(x,y)	(x)->GetSubTemp(y)
#define GET_DBL_ARRAY(x,y)	(x)->GetDblArray(y)
#endif

#define SZ_CNTOFTEMP_MA		3

double MA(long lIndex, long lPeriod, IUNKNOWNDATAITEM SRC, ITEMPITEM TEMP)
{
	double dRetValue = 0;


	double *pdbTemp = GET_DBL_ARRAY(TEMP,SZ_CNTOFTEMP_MA);
	double *pdSum = pdbTemp;
	double *pdLastIndex = pdbTemp+1;
	double *pdLastValue = pdbTemp+2;


	*pdSum += GET_AT(SRC,lIndex);
	SET_AT(pTemp,lIndexSum,GET_AT(pTemp,lIndexSum)+GET_AT(SRC,lIndex));	/* Save Sum Of SrcValue..(Sum += SrcValue)*/
	
	if(lPeriod<=lIndex) /* calculation is available*/
	{
		if(GET_AT(pTemp,lIndexLastIndex)<lIndex)	/* history */
		{
			SET_AT(pTemp,lIndexSum,GET_AT(pTemp,lIndexSum)-GET_AT(SRC,lIndex-lPeriod));
		}
		else	/* in real time */
		{
			SET_AT(pTemp,lIndexSum,GET_AT(pTemp,lIndexSum)-GET_AT(pTemp,lIndexLastValue));
		}
		dRetValue = GET_AT(pTemp,lIndexSum)/lPeriod;
	}
	else if(lPeriod==(lIndex+1))
	{
		dRetValue = GET_AT(pTemp,lIndexSum)/lPeriod;
	}

	SET_AT(pTemp,lIndexLastIndex,lIndex);	/* Save Last Index.. */
	SET_AT(pTemp,lIndexLastValue,GET_AT(SRC,lIndex));	/* Save Last Value To Calculate In RealTime.. */

	return dRetValue;
}


/*
Input : Period(Numeric);
Var : value1(0), value2(0);

value1 = (H+L+C)/3;
value2 = ma(value1,Period);

_CCI = (value1-value2)/(ma(abs(value1-value2),Period)*0.015);
*/

double CCI(long lIndex, long lPeriod
		   , IUNKNOWNDATAITEM pHigh
		   , IUNKNOWNDATAITEM pLow
		   , IUNKNOWNDATAITEM pClose
		   , IUNKNOWNDATAITEM pSrc1
		   , IUNKNOWNDATAITEM pSrc2
		   , IUNKNOWNDATAITEM pSrc3
		   , IUNKNOWNDATAITEM pTemp)
{
	double dRetValue = 0;

	SET_AT(pSrc1,lIndex, (GET_AT(pHigh,lIndex)+GET_AT(pLow,lIndex)+GET_AT(pClose,lIndex))/3);
	SET_AT(pSrc2,lIndex,MA(lIndex,lPeriod,pSrc1,pTemp));

	SET_AT(pSrc3,lIndex, fabs(GET_AT(pSrc1,lIndex)-GET_AT(pSrc2,lIndex)));
	dRetValue = (GET_AT(pSrc1,lIndex)-GET_AT(pSrc2,lIndex))/(MA(lIndex,lPeriod,pSrc3,pTemp)*0.015);

	return dRetValue;	
}


//#endif /*__TECHNICALINDEXFOMULA*/