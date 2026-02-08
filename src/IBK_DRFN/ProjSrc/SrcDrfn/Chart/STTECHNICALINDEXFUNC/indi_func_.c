#include "indi_func.h"

double yesStd(long lIsUpdateData, long lIndex, long lPeriod, 
	UNKNOWN_DATA_TYPE SRC_VAR,
	TEMP_DATA_TYPE TEMP_VAR)
{
	int i;
	double ldRetVal = NONAVAILABLE;
	double ldAvg, ldVal1, ldVal2;

/*  gm0604 blocked 2005.9.12
//	if (lIndex + 1 < lPeriod || lPeriod < 1)
//	{
//		return ldRetVal;
//	}
*/
	
	ldAvg = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, TEMP_VAR);

	/* gm0604 added 2005.9.12*/
	if(ldAvg==NONAVAILABLE)
	{
		return ldRetVal;
	}
	/*//////////////////////////////*/

	ldVal1 = 0;
	for (i = 0; i < lPeriod; i++)
	{
		ldVal2 = GET_AT(SRC_VAR, lIndex - i) - ldAvg;
		ldVal1 = ldVal1 + (ldVal2 * ldVal2);
	}
		
	ldVal1 = ldVal1 / lPeriod;

	ldRetVal = sqrt(ldVal1);

	return ldRetVal;
}

double yesAccum(long lIsUpdateData, long lIndex, UNKNOWN_DATA_TYPE SRC_VAR, 
	TEMP_DATA_TYPE TEMP_VAR)
{
/*	int i;*/
	double ldRetVal = NONAVAILABLE;
	double ldCurrVal;
	double *ldpTemp;
	double *ldpSum;
	double *ldpLastIndex;
	double *ldpLastValue;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;

	ldpSum = &ldpTemp[0];
	ldpLastIndex = &ldpTemp[1];
	ldpLastValue = &ldpTemp[2];

	ldCurrVal = GET_AT(SRC_VAR, lIndex);

	if (lIndex > 0 && *ldpLastIndex == lIndex)
	{
		*ldpSum = *ldpSum - *ldpLastValue + ldCurrVal;
	}
	else
	{
		if (lIndex == 0)
		{
			*ldpSum = ldCurrVal;
		}
		else
		{
			*ldpSum = *ldpSum + ldCurrVal;
		}
	}
	
	ldRetVal = *ldpSum;

	*ldpLastIndex = lIndex;	
	*ldpLastValue = ldCurrVal;

	return ldRetVal;
}

double yesAccumN(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	int i, j;
	int liChkFlag = 0;
	double ldRetVal = NONAVAILABLE;
	double ldCurrVal;
	double ldSrc;
	double *ldpTemp;
	double *ldpSum;
	double *ldpLastIndex;
	double *ldpLastValue;

	if (lIndex + 1 < lPeriod || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;

	ldpSum = &ldpTemp[0];
	ldpLastIndex = &ldpTemp[1];
	ldpLastValue = &ldpTemp[2];

	ldCurrVal = GET_AT(SRC_VAR, lIndex);
	if (ldCurrVal == NONAVAILABLE)
	{
		*ldpSum = 0.;
		*ldpLastIndex = lIndex;
		*ldpLastValue = NONAVAILABLE;

		return NONAVAILABLE;
	}	

	if (lIndex >= lPeriod)
	{
		*ldpSum = *ldpSum + ldCurrVal;

		if (lIndex == *ldpLastIndex)
		{
			*ldpSum = *ldpSum - *ldpLastValue;
		}
		else
		{
			ldSrc = GET_AT(SRC_VAR, lIndex - lPeriod);
			if (ldSrc != NONAVAILABLE)
			{
				*ldpSum = *ldpSum - ldSrc;
			}
			else
			{
				*ldpLastIndex = lIndex;
				*ldpLastValue = ldCurrVal;

				return NONAVAILABLE;
			}
		}
	}
	else if ((lIndex + 1) == lPeriod)
	{
		if (lIndex == *ldpLastIndex)
		{
			*ldpSum = *ldpSum - *ldpLastValue + ldCurrVal;
		}
		else
		{
			*ldpSum = 0.;
			for (i = lIndex, j = 0;  j < lPeriod && i >= 0; i--, j++)
			{
				ldSrc = GET_AT(SRC_VAR, i);
				if (ldSrc != NONAVAILABLE)
				{
					*ldpSum = *ldpSum + ldSrc;
				}
				else
				{
					liChkFlag = 1;
				}
			}

			if (liChkFlag)
			{
				*ldpLastIndex = lIndex;
				*ldpLastValue = ldCurrVal;

				return NONAVAILABLE;
			}
		}
	}

	ldRetVal = *ldpSum;
	*ldpLastIndex = lIndex;	
	*ldpLastValue = ldCurrVal;
	
	return ldRetVal;
}

double yesEMA(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, 
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldLastEMA = 0.;
	double ldCurrVal = 0.;
	double *ldpTemp;
	double *ldpFator;
	double *ldpLastIndex;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;
	ldpFator = &ldpTemp[0];
	ldpLastIndex = &ldpTemp[1];

	ldCurrVal = GET_AT(SRC_VAR, lIndex);
	if (ldCurrVal == NONAVAILABLE||ldCurrVal== -NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	/* {{{ 전일 EMA값이 없을때 당일종가값 리턴 }}} */
	//if(*ldpLastIndex == NULL)
	if(lIndex == 0)
	{
		ldRetVal = ldCurrVal;
	}
	else	/*	{{{  EMA  = 전일EMA + 2/(N+1) * (당일종가 - 전일EMA)  }}} */	
	{
		ldLastEMA = GET_AT(OPT_VAR, lIndex - 1);
		*ldpFator = 2. / (lPeriod + 1.);
		ldRetVal = ldLastEMA + *ldpFator * (ldCurrVal - ldLastEMA);
	}
	
	SET_AT(OPT_VAR,lIndex,ldRetVal);
	*ldpLastIndex = lIndex;

/*
	// chart OCX가 과거데이터를 지우기때문에 보정 작업함. 단계 1/1
	//[[
	if(*ldpLastIndex==OCX_MAX_INDEX-1)
	{
		int nLoofCnt = OCX_MAX_INDEX-OCX_DELETE_CNT;
		for(int k=0; k<=nLoofCnt; k++)
		{
			//double ldTmpVal= GET_AT(SRC_VAR, lIndex-k);
			//SET_AT(SRC_VAR, lIndex-50-k, ldTmpVal);
			double ldTmpVal= GET_AT(OPT_VAR, k+OCX_DELETE_CNT);
			SET_AT(OPT_VAR, k, ldTmpVal);
		}
		*ldpLastIndex = nLoofCnt;
	}
	//]]
*/
	return ldRetVal;
}

double yesHighest(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	int i, j;
	double ldRetVal = NONAVAILABLE;
	double ldCurrVal;
	double ldVal;
	double ldMax;
	double *ldpTemp;
	double *ldpLastValue;	/*마지막값*/
	double *ldpLastResult;	/*Max*/
	double *ldpCurrentIndex;

	if (lPeriod < 1)
	{
		return ldRetVal;
	}

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;
	ldpLastValue = &ldpTemp[0];
	ldpLastResult = &ldpTemp[1];
	ldpCurrentIndex = &ldpTemp[2];

	ldCurrVal = GET_AT(SRC_VAR, lIndex);
	if (ldCurrVal == NONAVAILABLE||ldCurrVal == -NONAVAILABLE)
	{
		return NONAVAILABLE;
	}
	if(*ldpCurrentIndex == 0)
	{
		ldRetVal = ldCurrVal;
		goto END_POS;
	}

/*
	if (ldCurrVal == *ldpLastValue)	
	{
		if (ldCurrVal > *ldpLastResult)
		{
			ldRetVal = ldCurrVal;
		}
		else
		{
			ldRetVal = *ldpLastResult;
		}

		goto END_POS;
	}
*/

	if (*ldpCurrentIndex >= lPeriod)
	{
		ldVal = GET_AT(SRC_VAR, lIndex - lPeriod);
				
		/* 빠질데이터가 최고값보다 작다면 연산을 다시할 필요없고 */
		/* 마지막값만 비교하면 된다. */
		if (ldVal < *ldpLastResult) 
		{ 
			if (ldCurrVal > *ldpLastResult)
			{
				ldRetVal = ldCurrVal;
			}
			else
			{
				ldRetVal = *ldpLastResult;
			}

			goto END_POS;
		}
	}

	ldMax = ldCurrVal;
	for (i = lIndex - 1, j = 1;  j < lPeriod && i >= 0 && (j-1)<=*ldpCurrentIndex; i--, j++)
	{
		ldVal = GET_AT(SRC_VAR, i);	
		if (ldVal > ldMax)
		{
			ldMax = ldVal;
		}
	}

	ldRetVal = ldMax;

END_POS:

	*ldpLastValue = ldCurrVal;
	*ldpLastResult = ldRetVal;
	*ldpCurrentIndex += 1;

	return ldRetVal;
}

double yesLowest(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	int i, j;
	double ldRetVal = NONAVAILABLE;
	double ldCurrVal;
	double ldVal;
	double ldMin;
	double *ldpTemp;
	double *ldpLastValue;
	double *ldpLastResult;
	double *ldpCurrentIndex;

/*	if (lIndex + 1 < lPeriod || lPeriod < 1)*/
	if (lPeriod < 1)
	{
		return ldRetVal;
	}

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;

	ldpLastValue = &ldpTemp[0];
	ldpLastResult = &ldpTemp[1];
	ldpCurrentIndex = &ldpTemp[2];

	ldCurrVal = GET_AT(SRC_VAR, lIndex);
	if (ldCurrVal == NONAVAILABLE||ldCurrVal == -NONAVAILABLE)
	{
		return NONAVAILABLE;
	}
	if(lIndex == 0)
	{
		ldRetVal = ldCurrVal;
		goto END_POS;
	}

/*
	if (lIndex == *ldpLastValue)
	{
		if (ldCurrVal < *ldpLastResult)
		{
			ldRetVal = ldCurrVal;
		}
		else
		{
			ldRetVal = *ldpLastResult;
		}

		goto END_POS;
	}
*/

	if (*ldpCurrentIndex >= lPeriod)
	{
		ldVal = GET_AT(SRC_VAR, lIndex - lPeriod);
		
		/* 빠질데이터가 최저값보다 크다면 연산을 다시할 필요없고 */
		/* 마지막값만 비교하면 된다. */
		if (ldVal > *ldpLastResult) 
		{ 
			if (ldCurrVal < *ldpLastResult)
			{
				ldRetVal = ldCurrVal;
			}
			else
			{
				ldRetVal = *ldpLastResult;
			}

			goto END_POS;
		}
	}

	ldMin = ldCurrVal;
	for (i = lIndex - 1, j = 1;  j < lPeriod && i >= 0 && (j-1)<=*ldpCurrentIndex; i--, j++)
	{
		ldVal = GET_AT(SRC_VAR, i);	
		if (ldVal < ldMin)
		{
			ldMin = ldVal;
		}
	}

	ldRetVal = ldMin;

END_POS:

	*ldpLastValue = ldCurrVal;
	*ldpLastResult = ldRetVal;
	*ldpCurrentIndex += 1;
	return ldRetVal;
}

double yesSMA(long lIsUpdateData, long lIndex, long lPeriod, UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	int nPos;
    double dRetValue = NONAVAILABLE;
 	double ldCurrVal = 0;
	double *ldpTemp;
    double *ldpSum;
    double *ldpCurrentIndex;
    double *ldpLastValue;
	double *ldpInit;
	BOOL bSum = FALSE;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return 0.;
	ldpCurrentIndex = &ldpTemp[0];
    ldpSum = &ldpTemp[1];

//	// chart OCX가 과거데이터를 지우기때문에 보정 작업함. 단계 1/2
//	//[[
//	if(*ldpCurrentIndex> lIndex)
//	{
//		*ldpCurrentIndex = lIndex;
//	}
//	//]]

    ldpLastValue = &ldpTemp[2];
	ldpInit = &ldpTemp[3];

//	if(lPeriod==60 && *ldpCurrentIndex==2007)
//	{
//		TRACE("aa\n");
//	}

	ldCurrVal = GET_AT(SRC_VAR, lIndex);
	if (ldCurrVal == NONAVAILABLE||ldCurrVal == -NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	if ((lPeriod-1) > *ldpCurrentIndex )	/* calculation is unavailable, so return NONAVAILABLE without calculation*/
	{
		if(!lIsUpdateData)	*ldpCurrentIndex += 1;
		return NONAVAILABLE;
	}

	if (!*ldpInit&&!lIsUpdateData)	/* At First time, calculate Sum By looping... */
	{
		*ldpSum = 0.;
		for(nPos = (lIndex-lPeriod+1); nPos <= lIndex; nPos++)
		{
			*ldpSum += GET_AT(SRC_VAR, nPos); /* Save Sum Of SrcValue..(Sum += SrcValue)*/
		}
		*ldpInit=1.;
	}
	else
	{
		*ldpSum += ldCurrVal; /* Save Sum Of SrcValue..(Sum += SrcValue)*/
	}

    if (lPeriod <= *ldpCurrentIndex) /* calculation is available*/
    {
        if(!lIsUpdateData)  /* insert */
        {
            *ldpSum = *ldpSum - GET_AT(SRC_VAR, lIndex-lPeriod);
//[[test
//			double td = GET_AT(SRC_VAR, lIndex-lPeriod);
//			if(td==0.00)
//			{
//				td = GET_AT(SRC_VAR, lIndex-lPeriod-1);
//			}
//            *ldpSum = *ldpSum - td;
//]]
			
//			// 보정작업.
//			if( (*ldpCurrentIndex+1%20)==0)
//			{
//				*ldpSum = 0.;
//				for(nPos = (lIndex-lPeriod+1); nPos <= lIndex; nPos++)
//				{
//					*ldpSum += GET_AT(SRC_VAR, nPos); /* Save Sum Of SrcValue..(Sum += SrcValue)*/
//				}
//			}
        }
        else    /* update */
        {         
			*ldpSum = *ldpSum - *ldpLastValue;
        }
        dRetValue = *ldpSum / lPeriod;
    }
	else if(lPeriod == (*ldpCurrentIndex+1))	/* calculation is available*/
	{
		if(lIsUpdateData)  /* update */
		{
			*ldpSum = *ldpSum - *ldpLastValue;
		}
		dRetValue = *ldpSum / lPeriod;
	}

	*ldpLastValue = ldCurrVal;    /* Save Last Value To Calculate updating */
	if(!lIsUpdateData)
	{
		*ldpCurrentIndex += 1.;
/*
		// chart OCX가 과거데이터를 지우기때문에 보정 작업함. 단계 1/1
		//[[
		if(*ldpCurrentIndex==OCX_MAX_INDEX)
		{
			int nLoofCnt = OCX_MAX_INDEX-OCX_DELETE_CNT;
			for(int k=0; k<=nLoofCnt+1; k++)
			{
				//double ldTmpVal= GET_AT(SRC_VAR, lIndex-k);
				//SET_AT(SRC_VAR, lIndex-50-k, ldTmpVal);
				double ldTmpVal= GET_AT(SRC_VAR, k+OCX_DELETE_CNT);
				SET_AT(SRC_VAR, k, ldTmpVal);
			}
			*ldpCurrentIndex = nLoofCnt;
		}
		//]]
*/

	}
//	if(lPeriod==60 && dRetValue>=159.90)
//	{
//		TRACE("dd\n");
//	}

    return dRetValue;
}

double yesWMA(long lIsUpdateData, long lIndex, long lPeriod, UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	int nPos;
    double dRetValue = NONAVAILABLE;
 	double ldCurrVal = 0.;
	double *ldpTemp;
    double *ldpSum;
	double *ldpSumWeighted;
    double *ldpCurrentIndex;
    double *ldpLastValue;
	double *ldpInit;
	BOOL bSum = FALSE;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return 0.;
	ldpCurrentIndex = &ldpTemp[0];
    ldpSum = &ldpTemp[1];
	ldpSumWeighted = &ldpTemp[2];
    ldpLastValue = &ldpTemp[3];
	ldpInit = &ldpTemp[4];

	ldCurrVal = GET_AT(SRC_VAR, lIndex);
	if (ldCurrVal == NONAVAILABLE||ldCurrVal == -NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	if ((lPeriod-1) > *ldpCurrentIndex )	/* calculation is unavailable, so return NONAVAILABLE without calculation*/
	{
		if(!lIsUpdateData)	*ldpCurrentIndex += 1.;
		return NONAVAILABLE;
	}

	if (!*ldpInit&&!lIsUpdateData)	/* At First time, calculate Sum By looping... */
	{
		long lWeighted = 1;
		*ldpSumWeighted = 0.;
		*ldpSum = 0.;
		for(nPos = (lIndex-lPeriod+1) ; nPos <= lIndex ; nPos++, lWeighted++)
		{
			*ldpSumWeighted += GET_AT(SRC_VAR, nPos)*lWeighted;
			*ldpSum += GET_AT(SRC_VAR, nPos); /* Save Sum Of SrcValue..(Sum += SrcValue)*/			
		}
		*ldpInit=1.;
	}
	else
	{
		*ldpSumWeighted += ldCurrVal*lPeriod;
		*ldpSum += ldCurrVal; /* Save Sum Of SrcValue..(Sum += SrcValue)*/		
	}

    if (lPeriod <= *ldpCurrentIndex) /* calculation is available*/
    {
        if(!lIsUpdateData)  /* insert */
        {
			*ldpSumWeighted = *ldpSumWeighted - *ldpSum + ldCurrVal;
            *ldpSum = *ldpSum - GET_AT(SRC_VAR, lIndex-lPeriod);
        }
        else    /* update */
        {
			*ldpSumWeighted = *ldpSumWeighted - *ldpLastValue*lPeriod;
			*ldpSum = *ldpSum - *ldpLastValue;			
        }
        dRetValue = *ldpSumWeighted * 2. / (double)(lPeriod*(lPeriod+1.));
    }
    else if(lPeriod == (*ldpCurrentIndex+1))	/* calculation is available*/
    {
		if(lIsUpdateData)  /* update */
		{
			*ldpSum = *ldpSum - *ldpLastValue;
			*ldpSumWeighted = *ldpSumWeighted - *ldpLastValue*lPeriod;
		}
        dRetValue = *ldpSumWeighted * 2. / (double)(lPeriod*(lPeriod+1.));
    }

    *ldpLastValue = ldCurrVal;    /* Save Last Value To Calculate updating */
	if(!lIsUpdateData)
	{
		*ldpCurrentIndex += 1.;
/*
		// chart OCX가 과거데이터를 지우기때문에 보정 작업함. 단계 1/1
		//[[
		if(*ldpCurrentIndex==OCX_MAX_INDEX)
		{
			int nLoofCnt = OCX_MAX_INDEX-OCX_DELETE_CNT;
			for(int k=0; k<=nLoofCnt; k++)
			{
				//double ldTmpVal= GET_AT(SRC_VAR, lIndex-k);
				//SET_AT(SRC_VAR, lIndex-50-k, ldTmpVal);
				double ldTmpVal= GET_AT(SRC_VAR, k+OCX_DELETE_CNT);
				SET_AT(SRC_VAR, k, ldTmpVal);
			}
			*ldpCurrentIndex = nLoofCnt;
		}
		//]]
*/
	}	
    return dRetValue;
}


double yesAMA(long lIsUpdateData, long lIndex, long lPeriod, 
			  UNKNOWN_DATA_TYPE SRC_VAR, 
  			  UNKNOWN_DATA_TYPE OPT_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	int nPos;
    double dRetValue = NONAVAILABLE;
	double ldCurrVal = 0;
 	double ldCurrDif = 0;
	double ldLastAMA = 0;
	double ldER = 0;
	double ldSC = 0;
	double ldSumDiffModified = 0;
	double *ldpTemp;
    double *ldpSumDiff;	
    double *ldpCurrentIndex;
    double *ldpLastDif;
	double *ldpInit;
	BOOL bSum = FALSE;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return 0.;
	ldpCurrentIndex = &ldpTemp[0];
    ldpSumDiff = &ldpTemp[1];	
    ldpLastDif = &ldpTemp[2];
	ldpInit = &ldpTemp[3];


	ldCurrVal = GET_AT(SRC_VAR, lIndex);
	if (ldCurrVal == NONAVAILABLE||ldCurrVal == -NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	if ( lPeriod > *ldpCurrentIndex )	
	{	
		if(*ldpCurrentIndex==(lPeriod-1)) /* Set First Value at OPT_VAR*/
		{
			dRetValue = ldCurrVal;
			SET_AT(OPT_VAR,lIndex,ldCurrVal);
		}
	}
	else
	{
		ldCurrDif = abs( long( GET_AT(SRC_VAR, lIndex)-GET_AT(SRC_VAR, lIndex-1)));
		if (!*ldpInit&&!lIsUpdateData)	/* At First time, calculate Sum By looping... */
		{
			*ldpSumDiff = 0.;
			for(nPos = (lIndex-lPeriod+1); nPos <= lIndex; nPos++)
			{
				*ldpSumDiff += abs( long( GET_AT(SRC_VAR, nPos)-GET_AT(SRC_VAR, nPos-1))); /* Save Sum Of SrcValue..(Sum += SrcValue)*/
			}
			*ldpInit=1;
		}
		else
		{
			*ldpSumDiff += ldCurrDif; /* Save Sum Of SrcValue..(Sum += ldCurrDif)*/
		}

		if (lPeriod < *ldpCurrentIndex) /* calculation is available*/
		{
			if(!lIsUpdateData)  /* insert */
			{
				*ldpSumDiff = *ldpSumDiff - abs( long( GET_AT(SRC_VAR, lIndex-lPeriod)-GET_AT(SRC_VAR, lIndex-1-lPeriod)));
			}
			else    /* update */
			{         
				*ldpSumDiff = *ldpSumDiff - *ldpLastDif;
			}
			if(*ldpSumDiff!=0.)
			{
				ldSumDiffModified = *ldpSumDiff;
			}
			else
			{
				ldSumDiffModified = 1.;
			}
			
			ldER = abs( long( GET_AT(SRC_VAR, lIndex)-GET_AT(SRC_VAR, lIndex-lPeriod)))/ldSumDiffModified;
			ldSC = (ldER*((2.0/3.0)-(2.0/31.0))+(2.0/31.0))*(ldER*((2.0/3.0)-(2.0/31.0))+(2.0/31.0));
			ldLastAMA = GET_AT(OPT_VAR,lIndex-1);
			dRetValue = ldLastAMA + ldSC*(ldCurrVal-ldLastAMA);
		}
		else if(lPeriod == (*ldpCurrentIndex))	/* calculation is available*/
		{
			if(lIsUpdateData)  /* update */
			{
				*ldpSumDiff = *ldpSumDiff - *ldpLastDif;
			}
			if(*ldpSumDiff!=0)
			{
				ldSumDiffModified = *ldpSumDiff;
			}
			else
			{
				ldSumDiffModified = 1.;
			}
			ldER = abs( long( GET_AT(SRC_VAR, lIndex)-GET_AT(SRC_VAR, lIndex-lPeriod)))/ldSumDiffModified;
			ldSC = (ldER*((2.0/3.0)-(2.0/31.0))+(2.0/31.0))*(ldER*((2.0/3.0)-(2.0/31.0))+(2.0/31.0));
			ldLastAMA = GET_AT(OPT_VAR,lIndex-1);
			dRetValue = ldLastAMA + ldSC*(ldCurrVal-ldLastAMA);
		}
		*ldpLastDif = ldCurrDif;    /* Save Last Differance To Calculate updating */
	}
	SET_AT(OPT_VAR,lIndex,dRetValue);
	if(!lIsUpdateData)
	{
		*ldpCurrentIndex += 1.;
/*
		// chart OCX가 과거데이터를 지우기때문에 보정 작업함. 단계 1/1
		//[[
		if(*ldpCurrentIndex==OCX_MAX_INDEX)
		{
			int nLoofCnt = OCX_MAX_INDEX-OCX_DELETE_CNT;
			for(int k=0; k<=nLoofCnt; k++)
			{
				//double ldTmpVal= GET_AT(SRC_VAR, lIndex-k);
				//SET_AT(SRC_VAR, lIndex-50-k, ldTmpVal);
				double ldTmpVal= GET_AT(SRC_VAR, k+OCX_DELETE_CNT);
				SET_AT(SRC_VAR, k, ldTmpVal);
			}
			*ldpCurrentIndex = nLoofCnt;
		}
		//]]
*/
	}	
    return dRetValue;
}

double yesCrossUp(long lIsUpdateData, long lIndex, 
		UNKNOWN_DATA_TYPE SRC_VAR1, 
		UNKNOWN_DATA_TYPE SRC_VAR2, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = 0.;

	if (lIndex > 1)
	{
		double dThirdValueOfSrcVar1 = GET_AT(SRC_VAR1, lIndex - 2);
		double dThirdValueOfSrcVar2 = GET_AT(SRC_VAR2, lIndex - 2);
		double dSecondValueOfSrcVar1 = GET_AT(SRC_VAR1, lIndex - 1);
		double dSecondValueOfSrcVar2 = GET_AT(SRC_VAR2, lIndex - 1);
		if(dThirdValueOfSrcVar1==NONAVAILABLE||dThirdValueOfSrcVar2==NONAVAILABLE)
		{
			return 0.;
			//return NONAVAILABLE;
		}

		if (GET_AT(SRC_VAR1, lIndex) > GET_AT(SRC_VAR2, lIndex) &&
			dSecondValueOfSrcVar1 <= dSecondValueOfSrcVar2)
		{
			ldRetVal = 1.;
		}
	}
	else
	{
		return 0.;
		//return NONAVAILABLE;
	}

	return ldRetVal;
}

double yesCrossDown(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE SRC_VAR1, 
		UNKNOWN_DATA_TYPE SRC_VAR2, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = 0.;

	if (lIndex > 1)
	{
		double dThirdValueOfSrcVar1 = GET_AT(SRC_VAR1, lIndex - 2);
		double dThirdValueOfSrcVar2 = GET_AT(SRC_VAR2, lIndex - 2);
		double dSecondValueOfSrcVar1 = GET_AT(SRC_VAR1, lIndex - 1);
		double dSecondValueOfSrcVar2 = GET_AT(SRC_VAR2, lIndex - 1);
		if(dThirdValueOfSrcVar1==NONAVAILABLE||dThirdValueOfSrcVar2==NONAVAILABLE)
		{
			return 0.;
			//return NONAVAILABLE;
		}

		if (GET_AT(SRC_VAR1, lIndex) < GET_AT(SRC_VAR2, lIndex) &&
			dSecondValueOfSrcVar1 >= dSecondValueOfSrcVar2)
		{
			ldRetVal = 1.;
		}
	}
	else
	{
		return 0.;
		//return NONAVAILABLE;
	}

	return ldRetVal;		
}

typedef struct
{
	int iIdx;
	double dValue;
} NTH_INFO;

#define NTH_MAX_CNT		400
#define NTH_ORDER_ASC	0
#define NTH_ORDER_DESC	1
#define NTH_TYPE_BAR	0
#define NTH_TYPE_VALUE	1

static int NthSorting(NTH_INFO *ptpNthList, int piCnt, int piOrder)
{
	int i, j;
	NTH_INFO ltNthTmp;

	if (piOrder == NTH_ORDER_ASC)
	{
		for (i = 0; i < piCnt - 1; i++)
		{
			for (j = i + 1; j < piCnt; j++)
			{
				if (ptpNthList[i].dValue > ptpNthList[j].dValue)
				{
					ltNthTmp = ptpNthList[i];
					ptpNthList[i] = ptpNthList[j];
					ptpNthList[j] = ltNthTmp;
				}
			} 
		}
	}
	else
	{
		for (i = 0; i < piCnt - 1; i++)
		{
			for (j = i + 1; j < piCnt; j++)
			{
				if (ptpNthList[i].dValue < ptpNthList[j].dValue)
				{
					ltNthTmp = ptpNthList[i];
					ptpNthList[i] = ptpNthList[j];
					ptpNthList[j] = ltNthTmp;
				}
			} 
		}
	}

	return 1;
}

static double ComputeNth(long lIndex, long lNth, long lPeriod,
        UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR, 
		long lOrder, long lType)
{
	int i, j;
	int liAllocFlag = 0;
	double ldRetVal = NONAVAILABLE;
	NTH_INFO ltpNThArray[NTH_MAX_CNT];
	NTH_INFO *ltpNthList;
	
	if (lNth > lPeriod || lNth < 1 || lPeriod < 1)
	{
		return ldRetVal;
	}

	if (lIndex + 1 < lPeriod)
	{
		return ldRetVal;
	}
	
	if (lPeriod > NTH_MAX_CNT && lIndex >= NTH_MAX_CNT)
	{
		liAllocFlag = 1;
		ltpNthList = (NTH_INFO *)malloc(sizeof(NTH_INFO) * lPeriod);
		if (ltpNthList == NULL)
		{
			return ldRetVal;
		}
	}
	else
	{
		ltpNthList = ltpNThArray;
	}

	/* 
		데이터 크기와 몇번째값을 구하는게 정해져 있지 않기 때문에 매번 다시 
		계산한다.
	*/
	for (i = lIndex, j = 0;  j < lPeriod && i >= 0; i--, j++)
	{
		ltpNthList[j].iIdx = i;
		ltpNthList[j].dValue = GET_AT(SRC_VAR, i);
	}

	if (j > 1 && j >= lNth)
	{
		NthSorting(ltpNthList, j, lOrder);
	}
	else
	{
		if (lNth == 1) 
		{
			if (lType == NTH_TYPE_BAR)
				return ltpNthList[0].iIdx;
			else
				return ltpNthList[0].dValue;
		}
		else return ldRetVal;
	}

	if (liAllocFlag)
	{
		free(ltpNthList);
	}

	if (lType == NTH_TYPE_BAR)
		return ltpNthList[lNth - 1].iIdx;
	else
		return ltpNthList[lNth - 1].dValue;
}

double yesNthHighestBar(long lIsUpdateData, long lIndex, long lNth, 
		long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	return ComputeNth(lIndex, lNth, lPeriod, SRC_VAR, TEMP_VAR, 
		NTH_ORDER_DESC, NTH_TYPE_BAR);
}

double yesNthHighest(long lIsUpdateData, long lIndex, long lNth, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	return ComputeNth(lIndex, lNth, lPeriod, SRC_VAR, TEMP_VAR, 
		NTH_ORDER_DESC, NTH_TYPE_VALUE);
}

double yesNthLowestBar(long lIsUpdateData, long lIndex, long lNth, 
		long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	return ComputeNth(lIndex, lNth, lPeriod, SRC_VAR, TEMP_VAR, 
		NTH_ORDER_ASC, NTH_TYPE_BAR);
}

double yesNthLowest(long lIsUpdateData, long lIndex, long lNth, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	return ComputeNth(lIndex, lNth, lPeriod, SRC_VAR, TEMP_VAR, 
		NTH_ORDER_ASC, NTH_TYPE_VALUE);
}

#undef NTH_MAX_CNT
#undef NTH_ORDER_ASC
#undef NTH_ORDER_DESC
#undef NTH_TYPE_BAR
#undef NTH_TYPE_VALUE

double yesMRO(long lIsUpdateData, long lIndex, long lPeriod, long lOccur,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	int i, j;
	int liCount;
	double ldRetVal = NONAVAILABLE;

	if (lOccur > lPeriod || lOccur < 1 || lPeriod < 1)
	{
		return ldRetVal;
	}

	if (lIndex + 1 < lPeriod)
	{
		return ldRetVal;
	}

	ldRetVal = -1.;

	for (i = lIndex, j = 0, liCount = 0;  j < lPeriod && i >= 0; i--, j++)
	{
		if (GET_AT(SRC_VAR, i) != 0.)
		{
			liCount++;
			if (liCount == lOccur)
			{
				ldRetVal = j;
				break;
			}
		}
	}

	return ldRetVal;
}

#define SWING_PRICE_HIGH	0
#define SWING_PRICE_LOW		1
#define SWING_TYPE_VALUE	0
#define	SWING_TYPE_BAR		1

static double ComputeSwing(long lIndex, long lOccur, long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR,
		long lPrice, long lType)
{
	int i, j;
	int liCount;
	double ldRetVal = NONAVAILABLE;
	double ldBasePrice;

	if (lOccur > lPeriod || 
		lRightStrength + lLeftStrength >= lPeriod ||
		lOccur < 1 || lPeriod < 1 || lLeftStrength < 0 || 
		lRightStrength < 0)
	{
		return ldRetVal;
	}

	if (lIndex + 1 < lPeriod)
	{
		return ldRetVal;
	}

	ldRetVal = -1.;

	liCount = 0;
	for (i = lIndex - lRightStrength; 
		(i - lLeftStrength) >= (lIndex - (lPeriod - 1)) ; i--)
	{
		ldBasePrice = GET_AT(SRC_VAR, i);

		if (lPrice == SWING_PRICE_HIGH)
		{
			for (j = 1 ; j <= lRightStrength; j++)
			{
				if (ldBasePrice <= GET_AT(SRC_VAR, i + j))
				{
					goto FOR_END;
				}
			}
	 
			for (j = 1 ; j <= lLeftStrength; j++)
			{
				if (ldBasePrice <= GET_AT(SRC_VAR, i - j))
				{
					goto FOR_END;
				}
			}
		}
		else
		{
			for (j = 1 ; j <= lRightStrength; j++)
			{
				if (ldBasePrice >= GET_AT(SRC_VAR, i + j))
				{
					goto FOR_END;
				}
			}
	 
			for (j = 1 ; j <= lLeftStrength; j++)
			{
				if (ldBasePrice >= GET_AT(SRC_VAR, i - j))
				{
					goto FOR_END;
				}
			}

		}

		liCount++;
		if (liCount == lOccur)
		{
			if (lType == SWING_TYPE_VALUE)
			{
				ldRetVal = ldBasePrice;
			}
			else
			{
				ldRetVal = lIndex - i;
			}

			break;
		}

FOR_END:
		continue;	
	}

	return ldRetVal;	
}

double yesSwingHigh(long lIsUpdateData, long lIndex, long lOccur, long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	return ComputeSwing(lIndex, lOccur, lPeriod,
			lLeftStrength, lRightStrength, SRC_VAR, TEMP_VAR,
			SWING_PRICE_HIGH, SWING_TYPE_VALUE);
}

double yesSwingHighBar(long lIsUpdateData, long lIndex, long lOccur, 
		long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	return ComputeSwing(lIndex, lOccur, lPeriod,
			lLeftStrength, lRightStrength, SRC_VAR, TEMP_VAR,
			SWING_PRICE_HIGH, SWING_TYPE_BAR);
}

double yesSwingLow(long lIsUpdateData, long lIndex, long lOccur, long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	return ComputeSwing(lIndex, lOccur, lPeriod,
			lLeftStrength, lRightStrength, SRC_VAR, TEMP_VAR,
			SWING_PRICE_LOW, SWING_TYPE_VALUE);
}

double yesSwingLowBar(long lIsUpdateData, long lIndex, long lOccur, 
		long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR)
{
	return ComputeSwing(lIndex, lOccur, lPeriod,
			lLeftStrength, lRightStrength, SRC_VAR, TEMP_VAR,
			SWING_PRICE_LOW, SWING_TYPE_BAR);
}

#undef SWING_PRICE_HIGH
#undef SWING_PRICE_LOW
#undef SWING_TYPE_VALUE
#undef SWING_TYPE_BAR

double yesCorrelation(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR1, UNKNOWN_DATA_TYPE SRC_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	int i, j;
	double ldRetVal = NONAVAILABLE;
	double ldAvgVal1;
	double ldAvgVal2;
	double ldStdVal1;
	double ldStdVal2;
	double ldSum;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldAvgVal1 = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, 
					GET_SUB_TEMP(TEMP_VAR, 1));
	ldAvgVal2 = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, 
					GET_SUB_TEMP(TEMP_VAR, 2));
	ldStdVal1 = yesStd(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, 
					GET_SUB_TEMP(TEMP_VAR, 3));
	ldStdVal2 = yesStd(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, 
					GET_SUB_TEMP(TEMP_VAR, 4));

	ldSum = 0.;
	for (i = lIndex, j = 0;  j < lPeriod && i >= 0; i--, j++)
	{
		ldSum = ldSum + (GET_AT(SRC_VAR1, i) * GET_AT(SRC_VAR2, i));
	}

	ldSum = ldSum / lPeriod - (ldAvgVal1 * ldAvgVal2);

	if (ldStdVal1 * ldStdVal2 > 0)
	{
		ldRetVal = (ldSum ) / (ldStdVal1 * ldStdVal2);
	}
	
	return ldRetVal;
}

/*
double yesWMA(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	int i, j;
	double ldRetVal = NONAVAILABLE;
	double ldSum;
	double ldCSum;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}
	
	ldSum = 0.;
	ldCSum = 0.;
	for (i = lIndex, j = 0;  j < lPeriod && i >= 0; i--, j++)
	{
		ldSum = ldSum + GET_AT(SRC_VAR, i) * (lPeriod - j);
		ldCSum = ldCSum + lPeriod - j;
	}

	if (ldCSum > 0)
	{
		ldRetVal = ldSum / ldCSum;	
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}
*/


/******************************************************************************/
/********************************* 지표함수 ***********************************/
/******************************************************************************/
/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.20
 Modifier		: 
 Comments		: AD Line = [(종가-시가) / (고가-저가)] * 거래량 + 전일 AD Line
-----------------------------------------------------------------------------*/
double yesAccDist(long lIsUpdateData, long lIndex, 
		UNKNOWN_DATA_TYPE OPEN_VAR, 
		UNKNOWN_DATA_TYPE HIGH_VAR, 
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldOpen;
	double ldHigh;
	double ldLow;
	double ldClose;
	double ldVolume;
	double ldVal;
	double ldADLineLast;


	if(lIndex < 1)
	{
		return NONAVAILABLE;
	}

	ldOpen = GET_AT(OPEN_VAR, lIndex);
	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldVolume = GET_AT(VOLUME_VAR, lIndex);
	ldADLineLast = GET_AT(OPT_VAR, lIndex-1);

	if (ldHigh-ldLow)
	{
		ldVal = ((ldClose-ldOpen) / (ldHigh-ldLow)) * ldVolume;
	}
	else
	{
		ldVal = (ldClose-ldOpen) * ldVolume;
	}

	ldRetVal = ldVal + ldADLineLast;
	SET_AT(OPT_VAR,lIndex,ldRetVal);
	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: {{{ ADX = DX의 n일 이동평균 }}}
				  {{{ DX = (PDI-MDI)의 절대값 / (PDI+MDI) * 100 }}}
-----------------------------------------------------------------------------*/
double yesADX(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR, 
		UNKNOWN_DATA_TYPE CLOSE_VAR, 
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		UNKNOWN_DATA_TYPE OPT_VAR5,
		UNKNOWN_DATA_TYPE OPT_VAR6,
		UNKNOWN_DATA_TYPE OPT_VAR7,
		UNKNOWN_DATA_TYPE OPT_VAR8,
		UNKNOWN_DATA_TYPE OPT_VAR9,
		UNKNOWN_DATA_TYPE OPT_VAR10,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldPDI;
	double ldMDI;
	double ldDX;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldPDI = yesPDI(lIsUpdateData, lIndex, lPeriod, HIGH_VAR, LOW_VAR, CLOSE_VAR, 
		OPT_VAR1, OPT_VAR2, OPT_VAR7, OPT_VAR8,GET_SUB_TEMP(TEMP_VAR, 1) );

	ldMDI = yesMDI(lIsUpdateData, lIndex, lPeriod, HIGH_VAR, LOW_VAR, CLOSE_VAR, 
		OPT_VAR3, OPT_VAR4, OPT_VAR9, OPT_VAR10,GET_SUB_TEMP(TEMP_VAR, 2));

	if(lIndex < lPeriod)
		return NONAVAILABLE;

	ldDX = fabs(ldPDI-ldMDI) / (ldPDI+ldMDI) *100.;
	SET_AT(OPT_VAR5, lIndex, ldDX);

	ldRetVal = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR5, OPT_VAR6, GET_SUB_TEMP(TEMP_VAR, 3));

	
/*
	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldHighOld = GET_AT(HIGH_VAR, lIndex - 1);
	ldLowOld = GET_AT(LOW_VAR, lIndex - 1);

	ldVal1 = (ldHigh - ldHighOld > 0 && ldHigh - ldHighOld > ldLowOld - ldLow ?
				ldHigh - ldHighOld : 0.); 
	ldVal2 = (ldLowOld - ldLow > 0 && ldHigh - ldHighOld < ldLowOld - ldLow ?
				ldLowOld - ldLow : 0.);

	SET_AT(OPT_VAR1, lIndex, ldVal1);
	SET_AT(OPT_VAR2, lIndex, ldVal2);

	ldVal3 = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal4 = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	if(ldVal3 == NONAVAILABLE || ldVal4 == NONAVAILABLE)
		return NONAVAILABLE;

	if (ldVal3 + ldVal4)
	{
		ldVal5 = fabs(ldVal3 - ldVal4) / (ldVal3 + ldVal4) * 100;
	}
	else
	{
		ldVal5 = 0.;
	}

	SET_AT(OPT_VAR3, lIndex, ldVal5);

	ldRetVal = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR3, 
				GET_SUB_TEMP(TEMP_VAR, 3));*/


/*
	if (lIndex + 1 < (lPeriod * 2))
	{
		ldRetVal = NONAVAILABLE;
	}
*/

	return ldRetVal;
}

double yesATR(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, 
		UNKNOWN_DATA_TYPE LOW_VAR, 
		UNKNOWN_DATA_TYPE CLOSE_VAR, 
		UNKNOWN_DATA_TYPE OPT_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldClose;
	double ldCloseOld;
	double ldVal1;
	double ldVal2;
	double ldVal3;
	double ldVal4;

	// (2006/3/30 - Seung-Won, Bae) Check Index for ldCloseOld
	if( lPeriod < 1 || lIndex < 1)
	{
		return NONAVAILABLE;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex - 1);

	ldVal1 = ldHigh - ldLow;
	ldVal2 = fabs(ldCloseOld - ldHigh);
	ldVal3 = fabs(ldCloseOld - ldLow);

	ldVal4 = (ldVal1 > ldVal2 ? (ldVal1 > ldVal3 ? ldVal1 : ldVal3) : 
				(ldVal2 > ldVal3 ? ldVal2 : ldVal3));

	SET_AT(OPT_VAR, lIndex, ldVal4);
	
/*
	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}
*/

	ldRetVal = yesSMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	if (lIndex + 1 < lPeriod + 1)
	{
		return NONAVAILABLE;
	}

	return ldRetVal;
}


double yesBollBandDown(long lIsUpdateData, long lIndex, long lPeriod, 
		double dStdDev,
		UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double dValue;

	if (lPeriod < 1) /* || lIndex + 1 < lPeriod) */
	{
		return NONAVAILABLE;
	}

	dValue = GET_AT(SRC_VAR, lIndex);

	ldRetVal = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1)) - 
				(dStdDev * yesStd(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
					GET_SUB_TEMP(TEMP_VAR, 2)));

	return ldRetVal;
}

double yesBollBandUp(long lIsUpdateData, long lIndex, long lPeriod, 
		double dStdDev,
		UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldRetVal = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1)) + 
				(dStdDev * yesStd(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
					GET_SUB_TEMP(TEMP_VAR, 2)));

	return ldRetVal;
}

double yesBW(long lIsUpdateData, long lIndex, 
		long lMaPeriod, long lRocPeriod, long lStoPeriod1, long lStoPeriod2,
		UNKNOWN_DATA_TYPE HIGH_VAR, 
		UNKNOWN_DATA_TYPE LOW_VAR, 
		UNKNOWN_DATA_TYPE CLOSE_VAR, 
		UNKNOWN_DATA_TYPE OPT_VAR1, 
		UNKNOWN_DATA_TYPE OPT_VAR2, 
		UNKNOWN_DATA_TYPE OPT_VAR3, 
		UNKNOWN_DATA_TYPE OPT_VAR4, 
		UNKNOWN_DATA_TYPE OPT_VAR5, 
		UNKNOWN_DATA_TYPE OPT_VAR6, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldClose;
	double ldCloseOld;
	double ldVal1;
	double ldEma9;
	double ldEma12;
	double ldEma26;
	double ldEmaStoPeriod2;
	double ldEmaTmp;
	double ldLowest;
	double ldHighest;

	if (lMaPeriod < 1 || lRocPeriod < 1 || lStoPeriod1 < 1 || lStoPeriod2 < 1)
	{
		return NONAVAILABLE;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex - lRocPeriod);

	ldEma12 = yesEMA(lIsUpdateData, lIndex, 12, CLOSE_VAR, OPT_VAR3,
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldEma26 = yesEMA(lIsUpdateData, lIndex, 26, CLOSE_VAR, OPT_VAR4,
				GET_SUB_TEMP(TEMP_VAR, 2));

	if (ldEma12 == NONAVAILABLE || ldEma26 == NONAVAILABLE)
	{
		SET_AT(OPT_VAR1, lIndex, NONAVAILABLE);
	}
	else
	{
		SET_AT(OPT_VAR1, lIndex, ldEma12 - ldEma26);
	}

	ldEma9 = yesEMA(lIsUpdateData, lIndex, 9, OPT_VAR1,  OPT_VAR5,
				GET_SUB_TEMP(TEMP_VAR, 3));

	if ((ldEma12 - ldEma26) > ldEma9)
	{
		ldVal1 = 1;
	}
	else
	{
		ldVal1 = -1;
	}

	if (ldClose > yesSMA(lIsUpdateData, lIndex, lMaPeriod, CLOSE_VAR,
		GET_SUB_TEMP(TEMP_VAR, 4)))
	{
		ldVal1 += 1;
	}
	else
	{
		ldVal1 -= 1;
	}

	if (ldCloseOld)
	{
		if ((ldClose - ldCloseOld) / ldCloseOld * 100. > 0.)
		{
			ldVal1 += 1;
		}
		else
		{
			ldVal1 -= 1;
		}
	}
	else
	{
		ldVal1 -= 1;
	}

	ldLowest = yesLowest(lIsUpdateData, lIndex, lStoPeriod1, LOW_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 5));
	ldHighest = yesHighest(lIsUpdateData, lIndex, lStoPeriod1, HIGH_VAR, 
					GET_SUB_TEMP(TEMP_VAR, 6));

	if (ldLowest == NONAVAILABLE || ldHighest == NONAVAILABLE)
	{
		ldEmaTmp = NONAVAILABLE;
	}
	else if (ldHighest - ldLowest)
	{
		ldEmaTmp = (ldClose - ldLowest) / (ldHighest - ldLowest) * 100.;
	}
	else
	{
		ldEmaTmp = 0.;
	}
	
	SET_AT(OPT_VAR2, lIndex, ldEmaTmp);

	ldEmaStoPeriod2 = 
		yesEMA(lIsUpdateData, lIndex, lStoPeriod2, OPT_VAR2, OPT_VAR6,
			GET_SUB_TEMP(TEMP_VAR, 7));

	if (ldEmaStoPeriod2 == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}
	else if (ldEmaStoPeriod2 > 50.)
	{
		ldVal1 += 1;
	}
	else
	{
		ldVal1 -= 1;
	}

	if (lIndex + 1 < 34 || lIndex + 1 < lRocPeriod ||
		lIndex + 1 < lStoPeriod1 + lStoPeriod2)
	{
		return NONAVAILABLE;
	}

	return ldVal1;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.20
 Modifier		: 
 Comments		: CCI = (TP-TPSMA)/(AVEDEV*0.015)
				  TP SMA = TP의 n일 단순이동평균
				 AVEDEV = (Σ|(TPi-TPSMA)|) / n
-----------------------------------------------------------------------------*/
double yesCCI(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2, 
		UNKNOWN_DATA_TYPE OPT_VAR3, 
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldTP;
	double ldTPSMA;
	double ldSum = 0.;
	double ldTPi;
	double ldAvedev;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldTP = GET_AT(SRC_VAR ,lIndex);

	ldTPSMA = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	if(lIndex < lPeriod-1)
		return NONAVAILABLE;

	for(int i=lIndex-lPeriod+1; i<lIndex+1; i++)
	{
		ldTPi = GET_AT(SRC_VAR ,i);
		ldSum = ldSum + fabs(ldTPi - ldTPSMA);
	}

	ldAvedev = ldSum/lPeriod;

	if (ldAvedev > 0.)
	{
		ldRetVal = (ldTP- ldTPSMA) / (ldAvedev * 0.015);
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

double yesCO(long lIsUpdateData, long lIndex, 
			 long lShortPeriod, long lLongPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, 
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldClose;
	double ldVolume;
	double ldVal;

/*	기간보다 작은 값일 경우에도 ema계산을 하도록 수정 LHE
	if (lIndex + 1 < lLongPeriod)
	{
		return NONAVAILABLE;
	}
*/
	if(lIndex < 0)
		return NONAVAILABLE;

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldVolume = GET_AT(VOLUME_VAR, lIndex);

	if ((ldHigh - ldLow) && ldVolume)
	{
		ldVal = ((ldClose - ldLow) - (ldHigh - ldClose)) /
				(ldHigh - ldLow) * ldVolume;
	}
	else
	{
		ldVal = 0.;
	}

	SET_AT(OPT_VAR1, lIndex, ldVal);

	ldVal = yesAccum(lIsUpdateData, lIndex, OPT_VAR1, 
				GET_SUB_TEMP(TEMP_VAR, 1));  

	SET_AT(OPT_VAR2, lIndex, ldVal);

	ldRetVal = yesEMA(lIsUpdateData, lIndex, lShortPeriod, OPT_VAR2, OPT_VAR3, 
					GET_SUB_TEMP(TEMP_VAR, 2)) -
				yesEMA(lIsUpdateData, lIndex, lLongPeriod, OPT_VAR2, OPT_VAR4, 
					GET_SUB_TEMP(TEMP_VAR, 3));


	return ldRetVal;
}

double yesCSar(long lIsUpdateData, long lIndex, double dAF, double dMaxAF, 
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldClose;
	double *ldpTemp;
	double *ldpTempSub;
	double *ldpLastIndex;
	double *ldpLastResult;
	double *ldpBeforeResult;
	double *ldpBeforeHighValue;
	double *ldpBeforeLowValue;
	double *ldpLastHighValue;
	double *ldpLastLowValue;
	double *ldpLastPos;
	double *ldpBeforePos;
	double *ldpBeforeAF;
	double *ldpLastAF;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return NONAVAILABLE;

	ldpLastIndex = &ldpTemp[0];
	ldpLastResult = &ldpTemp[1];
	ldpBeforeResult = &ldpTemp[2];
	ldpBeforeHighValue = &ldpTemp[3];
	ldpBeforeLowValue = &ldpTemp[4];
	ldpLastPos = &ldpTemp[5];
	ldpBeforePos = &ldpTemp[6];
	ldpLastHighValue = &ldpTemp[7];
	ldpLastLowValue = &ldpTemp[8];

    ldpTempSub = GET_TEMP_DBL(GET_SUB_TEMP(TEMP_VAR, 1));
    if (ldpTempSub == NULL) return NONAVAILABLE;
	ldpBeforeAF = &ldpTempSub[0];
	ldpLastAF = &ldpTempSub[1];

	/***
	YES 에서는 0, 1, 2 까지는 계산이 안되고, 3번째는 0 ~ 3 까지의
	최고값을 리턴한다. 
	그날 그날 넘어가는 갯수가 다름. 먼가 공식이 있는듯함.
	***/
	if (lIndex < 3)
	{
		ldHigh = GET_AT(HIGH_VAR, lIndex);
		ldLow = GET_AT(LOW_VAR, lIndex); 

		if (lIndex == 0)
		{
			*ldpBeforeHighValue = ldHigh;
			*ldpBeforeLowValue = ldLow;
			*ldpLastHighValue = ldHigh; 
			*ldpLastLowValue = ldLow;

			*ldpBeforeAF = dAF;
			*ldpLastAF = dAF;
		}
		else
		{
			if ( lIndex != *ldpLastIndex)
			{
				*ldpBeforeHighValue = *ldpLastHighValue; 
				*ldpBeforeLowValue = *ldpLastLowValue;
			}
			else
			{
				*ldpLastHighValue = *ldpBeforeHighValue; 
				*ldpLastLowValue = *ldpBeforeLowValue;
			}

			if (ldHigh > *ldpBeforeHighValue) *ldpLastHighValue = ldHigh;
			if (ldLow < *ldpBeforeLowValue) *ldpLastLowValue = ldLow;
		}

		*ldpLastIndex = lIndex;
		*ldpLastResult = *ldpLastHighValue;
		*ldpLastPos = -1.;
		*ldpBeforeResult = *ldpLastResult;
		*ldpBeforePos = *ldpLastPos;

		if (lIndex == 3)
		{
			return *ldpLastResult;	
		}
		else
		{
			return NONAVAILABLE;
		}
	}

	if (lIndex != *ldpLastIndex)
	{
		*ldpBeforeResult = *ldpLastResult;
		*ldpBeforePos = *ldpLastPos;
		*ldpBeforeHighValue = *ldpLastHighValue; 
		*ldpBeforeLowValue = *ldpLastLowValue;
		*ldpBeforeAF = *ldpLastAF;
	}
	else
	{
		*ldpLastHighValue = *ldpBeforeHighValue; 
		*ldpLastLowValue = *ldpBeforeLowValue;
		*ldpLastAF = *ldpBeforeAF;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex); 
	ldClose = GET_AT(CLOSE_VAR, lIndex); 

	if (*ldpLastPos == 1. && ldClose <= *ldpBeforeResult)
	{
		*ldpLastPos = -1.;
	}
	else if (*ldpLastPos == -1. && ldClose >= *ldpBeforeResult)
	{
		*ldpLastPos = 1.;
	}

	if (*ldpLastPos == 1.)
	{
		if (*ldpBeforePos != 1.)
		{
			*ldpLastResult = *ldpLastLowValue;
			*ldpLastAF = dAF;
			*ldpLastHighValue = ldHigh;
			*ldpLastLowValue = ldLow;
		}
		else
		{
			*ldpLastResult = *ldpBeforeResult + *ldpLastAF *
						(*ldpLastHighValue - *ldpBeforeResult);

			if (ldHigh > *ldpLastHighValue)
			{
				*ldpLastHighValue = ldHigh;
			}

			if (*ldpLastHighValue > *ldpBeforeHighValue &&
				*ldpLastAF < dMaxAF)
			{
				*ldpLastAF = *ldpLastAF + 
				(dAF < (dMaxAF - *ldpLastAF) ? dAF : (dMaxAF - *ldpLastAF));
			}
		}

		if (*ldpLastResult > ldClose && 
			*ldpBeforeResult < GET_AT(CLOSE_VAR, lIndex - 1))
		{
			*ldpLastResult = *ldpLastHighValue;
			*ldpLastPos = -1.;	
			*ldpLastAF = dAF;
			*ldpLastHighValue = ldHigh;
			*ldpLastLowValue = ldLow;
		}
	}
	else
	{
		if (*ldpBeforePos != -1.)
		{
			*ldpLastResult = *ldpLastHighValue;
			*ldpLastAF = dAF;
			*ldpLastHighValue = ldClose;
			*ldpLastLowValue = ldClose;
		}
		else
		{
			*ldpLastResult = *ldpBeforeResult + *ldpLastAF *
						(*ldpLastLowValue - *ldpBeforeResult);

			if (ldLow < *ldpLastLowValue)
			{
				*ldpLastLowValue = ldLow;
			}

			if (*ldpLastLowValue < *ldpBeforeLowValue &&
				*ldpLastAF < dMaxAF)
			{
				*ldpLastAF = *ldpLastAF + 
				(dAF < (dMaxAF - *ldpLastAF) ? dAF : (dMaxAF - *ldpLastAF));
			}
		}

		if (*ldpLastResult < ldClose && 
			*ldpBeforeResult > GET_AT(CLOSE_VAR, lIndex - 1))
		{
			*ldpLastResult = *ldpLastLowValue;
			*ldpLastPos = 1.;	
			*ldpLastAF = dAF;
			*ldpLastHighValue = ldHigh;
			*ldpLastLowValue = ldLow;
		}
	}

	*ldpLastIndex = lIndex;
	ldRetVal = *ldpLastResult;

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.22
 Modifier		: 
 Comments		: CV = ( A - B ) / B * 100
				  A = (고가-저가)의 n일 지수이동평균
				  B = n일 전의 (고가-저가)의 n일 지수 이동평균				
-----------------------------------------------------------------------------*/
double yesCV(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldVal;
	double ldVal1;
	double ldVal2;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	
	ldVal = ldHigh - ldLow;
	
	SET_AT(OPT_VAR1, lIndex, ldVal);

	ldVal1 = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, OPT_VAR3,
				GET_SUB_TEMP(TEMP_VAR, 1));

	SET_AT(OPT_VAR2, lIndex, ldVal1);

	if (lIndex + 1 < lPeriod * 2)
	{
		return NONAVAILABLE;
	}

/*	ldVal1 = ldVal;*/
	ldVal2 = GET_AT(OPT_VAR2, lIndex - lPeriod);

	if (ldVal2 == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	if (ldVal2)
	{
		ldRetVal = (ldVal1 - ldVal2) / ldVal2 * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

double yesDiMinus(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldClose;
	double ldHighOld;
	double ldLowOld;
	double ldCloseOld;
	double ldVal;
	double ldVal1;
	double ldVal2;
	double ldVal3;
	double ldMa1;
	double ldMa2;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldHighOld = GET_AT(HIGH_VAR, lIndex - 1);
	ldLowOld = GET_AT(LOW_VAR, lIndex - 1);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex - 1);

	if (ldLowOld - ldLow > 0. && ldHigh - ldHighOld < ldLowOld - ldLow)
	{
		ldVal = ldLowOld - ldLow;
	}
	else
	{
		ldVal = 0.;
	}

	SET_AT(OPT_VAR1, lIndex, ldVal);

	ldVal1 = ldHigh - ldLow;
	ldVal2 = fabs(ldCloseOld - ldHigh);
	ldVal3 = fabs(ldCloseOld - ldLow);
	ldVal = (ldVal1 > ldVal2 ? (ldVal1 > ldVal3 ? ldVal1 : ldVal3) :
			(ldVal2 > ldVal3 ? ldVal2 : ldVal3));

	SET_AT(OPT_VAR2, lIndex, ldVal);

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldMa1 = yesSMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldMa2 = yesSMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	/* 이전데이터를 참조하는 루틴이 있으므로 기간을 한건 추가 */
	if (lIndex + 1 < lPeriod + 1)
	{
		return NONAVAILABLE;
	}

	if (ldMa2)
	{
		ldRetVal = ldMa1 / ldMa2 * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}	

	return ldRetVal;
}

double yesDiPlus(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldClose;
	double ldHighOld;
	double ldLowOld;
	double ldCloseOld;
	double ldVal;
	double ldVal1;
	double ldVal2;
	double ldVal3;
	double ldMa1;
	double ldMa2;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldHighOld = GET_AT(HIGH_VAR, lIndex - 1);
	ldLowOld = GET_AT(LOW_VAR, lIndex - 1);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex - 1);

	if (ldHigh - ldHighOld > 0. && ldHigh - ldHighOld > ldLowOld - ldLow)
	{
		ldVal = ldHigh - ldHighOld;
	}
	else
	{
		ldVal = 0.;
	}

	SET_AT(OPT_VAR1, lIndex, ldVal);

	ldVal1 = ldHigh - ldLow;
	ldVal2 = fabs(ldCloseOld - ldHigh);
	ldVal3 = fabs(ldCloseOld - ldLow);
	ldVal = (ldVal1 > ldVal2 ? (ldVal1 > ldVal3 ? ldVal1 : ldVal3) :
			(ldVal2 > ldVal3 ? ldVal2 : ldVal3));

	SET_AT(OPT_VAR2, lIndex, ldVal);

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldMa1 = yesSMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldMa2 = yesSMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	/* 이전데이터를 참조하는 루틴이 있으므로, 기간을 한건 추가 */
	if (lIndex + 1 < lPeriod + 1)
	{
		return NONAVAILABLE;
	}

	if (ldMa2)
	{
		ldRetVal = ldMa1 / ldMa2 * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}	

	return ldRetVal;
}

double yesDisparity(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldTmp;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldClose = GET_AT(SRC_VAR, lIndex);
	
	ldTmp = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	if(ldTmp == NONAVAILABLE || lIndex < lPeriod)
		return NONAVAILABLE;

	if (ldTmp)
	{
		ldRetVal = ldClose / ldTmp * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.12
 Modifier		: 
 Comments		: {{{ EOM = ( Midpoint Move / Box Ratio )의 n일 EMA }}}
						Midpoint Move = (금일고가+금일저가)/2 - (전일고가+전일저가)/2
						Box Ratio = (거래량/10000) / (금일고가-금일저가)
-----------------------------------------------------------------------------*/
double yesEOM(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldVolume;
	double ldHighOld;
	double ldLowOld;
	double ldVal;
	double ldValue;



	if (lIndex < 1)
	{
		return NONAVAILABLE;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldVolume = GET_AT(VOLUME_VAR, lIndex);
	ldHighOld = GET_AT(HIGH_VAR, lIndex - 1);
	ldLowOld = GET_AT(LOW_VAR, lIndex - 1);

	ldVal = ((ldHigh-ldLow) ? (ldHigh-ldLow) : 1);
	
	ldValue = ((ldHigh + ldLow) / 2. - (ldHighOld + ldLowOld) / 2.) / 
					((ldVolume / 10000.) / ldVal);

	SET_AT(OPT_VAR1, lIndex, ldValue);
	
	ldRetVal = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 1) );

/*
	if (ldVolume && (ldHigh - ldLow))
	{
		ldRetVal = ((ldHigh + ldLow) / 2. - (ldHighOld + ldLowOld) / 2.) / 
					(ldVolume / 10000. / (ldHigh - ldLow));
	}
	else
	{
		ldRetVal = 0.;
	}
*/

	return ldRetVal;
}

double yesEnvelopeDown(long lIsUpdateData, long lIndex, long lPeriod, 
		double dPercent,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldMa;


/* Modify LHE, EMA -> SMA
	ldMa = yesEMA(lIsUpdateData, lIndex, lPeriod, CLOSE_VAR, 
			GET_SUB_TEMP(TEMP_VAR, 1));
*/
	ldMa = yesSMA(lIsUpdateData, lIndex, lPeriod, CLOSE_VAR, GET_SUB_TEMP(TEMP_VAR, 1));
	ldRetVal = ldMa - ldMa * (dPercent / 100.);

	return ldRetVal;
}

double yesEnvelopeUp(long lIsUpdateData, long lIndex, long lPeriod, 
		double dPercent,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldMa;

/* Modify LHE, EMA -> SMA
	ldMa = yesEMA(lIsUpdateData, lIndex, lPeriod, CLOSE_VAR, 
			GET_SUB_TEMP(TEMP_VAR, 1));
*/
	ldMa = yesSMA(lIsUpdateData, lIndex, lPeriod, CLOSE_VAR, GET_SUB_TEMP(TEMP_VAR, 1));
	
	ldRetVal = ldMa + ldMa * (dPercent / 100.);	

	return ldRetVal;
}

double yesLRL(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;

	double ldSrc;
	double ldVal1;
	double ldVal2;
	double ldVal3;
	double ldVal4;
	double ldVal5;
	double ldVal6;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	SET_AT(OPT_VAR1, lIndex, 1);
	ldVal1 = yesAccum(lIsUpdateData, lIndex, OPT_VAR1, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(OPT_VAR2, lIndex, ldVal1);

	ldSrc = GET_AT(SRC_VAR, lIndex);
	if (ldSrc == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	SET_AT(OPT_VAR3, lIndex, ldSrc * ldVal1);
	SET_AT(OPT_VAR4, lIndex, ldVal1 * ldVal1);

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldVal2 = yesSMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, 
				GET_SUB_TEMP(TEMP_VAR, 2));
	ldVal3 = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 3));
	ldVal4 = yesSMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR3, 
				GET_SUB_TEMP(TEMP_VAR, 4));
	ldVal5 = yesSMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR4, 
				GET_SUB_TEMP(TEMP_VAR, 5));
	ldVal6 = (ldVal5 - (ldVal2 * ldVal2));

	if (ldVal6 * (ldVal1 - ldVal2))
	{
		ldRetVal = (ldVal4 - (ldVal2 * ldVal3)) / 
			ldVal6 * (ldVal1 - ldVal2) + ldVal3;
	}
	else
	{
		//ldRetVal = 0.;
		ldRetVal = ldVal3;
	}	

	return ldRetVal;
}

double yesLRS(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;

	double ldSrc;
	double ldVal1;
	double ldVal2;
	double ldVal3;
	double ldVal4;
	double ldVal5;
	double ldVal6;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	/***
	B = (N*∑xy-∑x*∑y) / (N*∑x²-(∑x)²)
	***/

    SET_AT(OPT_VAR1, lIndex, 1);
    ldVal1 = yesAccum(lIsUpdateData, lIndex, OPT_VAR1, 
				GET_SUB_TEMP(TEMP_VAR, 1));
    SET_AT(OPT_VAR2, lIndex, ldVal1);
    SET_AT(OPT_VAR4, lIndex, ldVal1 * ldVal1); 

	ldSrc = GET_AT(SRC_VAR, lIndex);
	if (ldSrc == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

    SET_AT(OPT_VAR3, lIndex, ldSrc * ldVal1);

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldVal2 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, 
				GET_SUB_TEMP(TEMP_VAR, 2));
	ldVal3 = yesAccumN(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 3));
	ldVal4 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR3, 
				GET_SUB_TEMP(TEMP_VAR, 4));
	ldVal5 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR4, 
				GET_SUB_TEMP(TEMP_VAR, 5));

	ldVal6 = (lPeriod * ldVal5 - (ldVal2 * ldVal2)); 
	if (ldVal6)
	{
		ldRetVal = (lPeriod * ldVal4 - ldVal2 * ldVal3) / ldVal6;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

double yesMACD(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;

	if (lShortPeriod < 1 || lLongPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldRetVal = yesEMA(lIsUpdateData, lIndex, lShortPeriod, CLOSE_VAR, OPT_VAR1,
				GET_SUB_TEMP(TEMP_VAR, 1)) 
			- yesEMA(lIsUpdateData, lIndex, lLongPeriod, CLOSE_VAR, OPT_VAR2, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	if (lIndex + 1 < lShortPeriod || lIndex + 1 < lLongPeriod)
	{
		return NONAVAILABLE;
	}

	return ldRetVal;
}

double yesMACD_OSC(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldEma1;
	double ldEma2;
	double ldEma3;

	if (lShortPeriod < 1 || lLongPeriod < 1 || lPeriod < 1)
	{
		SET_AT(OPT_VAR, lIndex, NONAVAILABLE);

		return NONAVAILABLE;
	}

	ldEma1 = yesEMA(lIsUpdateData, lIndex, lShortPeriod, CLOSE_VAR, OPT_VAR2,
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldEma2 = yesEMA(lIsUpdateData, lIndex, lLongPeriod, CLOSE_VAR, OPT_VAR3,
				GET_SUB_TEMP(TEMP_VAR, 2));

/*
	if (ldEma1 == NONAVAILABLE || ldEma2 == NONAVAILABLE ||
		lIndex + 1 < lShortPeriod || lIndex + 1 < lLongPeriod)
	{
		SET_AT(OPT_VAR, lIndex, NONAVAILABLE);

		return NONAVAILABLE;
	}
*/

	SET_AT(OPT_VAR, lIndex, (ldEma1 - ldEma2));

	ldEma3 = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR, OPT_VAR4,
				GET_SUB_TEMP(TEMP_VAR, 3));
	if (lIndex + 1 < lShortPeriod + lPeriod  - 1 || 
		lIndex + 1 < lLongPeriod + lPeriod - 1)
	{
		return NONAVAILABLE;
	}

	ldRetVal = (ldEma1 - ldEma2) - ldEma3;

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.12
 Modifier		: 
 Comments		: MFI = 100 - ( 100 / (1+((PMF+0.0001)/(NMF+0.0001)) )
				  PM = TP상승일의 (거래량*TP)
				  NM = TP하락일의 (거래량*TP)
-----------------------------------------------------------------------------*/
double yesMFI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVolume;
	double ldVal1;
	double ldVal2;
	double ldCurrentSrcVar;
	double *ldpTemp;
	double *ldpLastSrcVar;


/*	if ( (lPeriod < 1) || (lIndex < 1) || (lIndex + 1 < lPeriod) || (lIndex + 1 < lPeriod + 1))*/
	if ( (lPeriod < 1) || (lIndex < 1) )
	{
		return NONAVAILABLE;
	}

	ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;
    ldpLastSrcVar = &ldpTemp[0];

	ldCurrentSrcVar = GET_AT(SRC_VAR, lIndex);
	ldVolume = GET_AT(VOLUME_VAR, lIndex);

	ldVal1 = (ldCurrentSrcVar > *ldpLastSrcVar ? ldVolume * ldCurrentSrcVar : 0.);
	ldVal2 = (ldCurrentSrcVar < *ldpLastSrcVar ? ldVolume * ldCurrentSrcVar : 0.);

	SET_AT(OPT_VAR1, lIndex, ldVal1);
	SET_AT(OPT_VAR2, lIndex, ldVal2);

	ldRetVal = 	
		100. - 100. / (1. + 
		(yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, 
			GET_SUB_TEMP(TEMP_VAR, 1))+ 0.0001) /
		(yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, 
			GET_SUB_TEMP(TEMP_VAR, 2)) + 0.0001));

	*ldpLastSrcVar = ldCurrentSrcVar;
	return ldRetVal;
}


double yesMI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		UNKNOWN_DATA_TYPE OPT_VAR5,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldVal1;
	double ldVal2;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);

	SET_AT(OPT_VAR1, lIndex, ldHigh - ldLow);
	ldVal1 = yesEMA(lIsUpdateData, lIndex, 9, OPT_VAR1, OPT_VAR4,
				GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(OPT_VAR2, lIndex, ldVal1);

	ldVal2 = yesEMA(lIsUpdateData, lIndex, 9, OPT_VAR2, OPT_VAR5,
				GET_SUB_TEMP(TEMP_VAR, 2));
	if (ldVal2 == NONAVAILABLE)
	{
		SET_AT(OPT_VAR3, lIndex, NONAVAILABLE);

		return NONAVAILABLE;
	}
	
	if (ldVal2)
	{
		SET_AT(OPT_VAR3, lIndex, ldVal1 / ldVal2);
	}
	else
	{
		SET_AT(OPT_VAR3, lIndex, 0.);
	}

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldRetVal = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR3, 
				GET_SUB_TEMP(TEMP_VAR, 3));

	if (lIndex + 1 < 9 + 9 + lPeriod  - 1)
	{
		return NONAVAILABLE;
	}

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.11
 Modifier		: 
 Comments		: {{{ Momentum = 당일종가 - k일전 종가 }}}
-----------------------------------------------------------------------------*/
double yesMomentum(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldCloseOld;

	if (lIndex < lPeriod || lPeriod < 0)
	{
		return NONAVAILABLE;
	}

	ldClose = GET_AT(SRC_VAR, lIndex);
	ldCloseOld = GET_AT(SRC_VAR, lIndex - lPeriod);

	if (ldCloseOld)
	{
/*		ldRetVal = ldClose / ldCloseOld * 100.;*/
		ldRetVal = ldClose - ldCloseOld;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

double yesNVI(long lIsUpdateData, long lIndex, 
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldVolume;
	double *ldpTemp;
	double *ldpLastIndex;
	double *ldpLastClose;
	double *ldpLastVolume;
	double *ldpLastResult;
	double *ldpBeforeClose;
	double *ldpBeforeVolume;
	double *ldpBeforeResult;

	ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;
    ldpLastIndex = &ldpTemp[0];
    ldpLastClose = &ldpTemp[1];
    ldpLastVolume = &ldpTemp[2];
    ldpLastResult = &ldpTemp[3];
    ldpBeforeClose = &ldpTemp[4];
    ldpBeforeVolume = &ldpTemp[5];
    ldpBeforeResult = &ldpTemp[6];

	ldClose = GET_AT(CLOSE_VAR, lIndex);	
	ldVolume = GET_AT(VOLUME_VAR, lIndex);	

	if (lIndex > 0)
	{
		if (lIndex != *ldpLastIndex)
		{
			*ldpBeforeClose = *ldpLastClose;
			*ldpBeforeVolume = *ldpLastVolume;
			*ldpBeforeResult = *ldpLastResult;
		}

		if (ldVolume < *ldpBeforeVolume)
		{
			if (*ldpBeforeClose)
			{
				ldRetVal = *ldpBeforeResult + 
						(ldClose - *ldpBeforeClose) / *ldpBeforeClose * 100.;
			}
			else
			{
				ldRetVal = 0.;							
			}
		}
		else
		{
			ldRetVal = *ldpBeforeResult;
		}
	}
	else
	{
		ldRetVal = 100.;
	}

	*ldpLastIndex = lIndex;
	*ldpLastClose = ldClose;
	*ldpLastVolume = ldVolume;
	*ldpLastResult = ldRetVal;

	return ldRetVal;
}

double yesOBV(long lIsUpdateData, long lIndex, 
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldVolume;
	double ldCloseOld;
	double ldVal;

	if (lIndex < 1)
	{
		return NONAVAILABLE;
	}

	ldClose = GET_AT(SRC_VAR, lIndex);
	ldVolume = GET_AT(VOLUME_VAR, lIndex);
	ldCloseOld = GET_AT(SRC_VAR, lIndex - 1);

	ldVal = (ldClose > ldCloseOld ? ldVolume : 
			(ldClose == ldCloseOld ? 0. : -ldVolume));

	SET_AT(OPT_VAR, lIndex, ldVal);

	ldRetVal = yesAccum(lIsUpdateData, lIndex, OPT_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	return ldRetVal;

}

double yesOSCP(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal1;
	double ldVal2;

	if (lShortPeriod < 1 || lLongPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldVal1 = yesSMA(lIsUpdateData, lIndex, lShortPeriod, CLOSE_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal2 = yesSMA(lIsUpdateData, lIndex, lLongPeriod, CLOSE_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 2));
	
	if (lIndex + 1 < lShortPeriod || lIndex + 1 < lLongPeriod)
	{
		return NONAVAILABLE;
	}

	if (ldVal1)
	{
		ldRetVal = (ldVal1 - ldVal2) / ldVal1 * 100.;
	}	
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

double yesOSCV(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal1;
	double ldVal2;

	if (lShortPeriod < 1 || lLongPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldVal1 = yesSMA(lIsUpdateData, lIndex, lShortPeriod, VOLUME_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal2 = yesSMA(lIsUpdateData, lIndex, lLongPeriod, VOLUME_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 2));
	
	if (lIndex + 1 < lShortPeriod || lIndex + 1 < lLongPeriod)
	{
		return NONAVAILABLE;
	}

	if (ldVal1)
	{
		ldRetVal = (ldVal1 - ldVal2) / ldVal1 * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

/* {{{ ROC지표사용 }}}
double yesPROC(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldCloseOld;
	if (lIndex < lPeriod || lPeriod < 1)
	{
		return NONAVAILABLE;
	}	
	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex - lPeriod);
	if (ldCloseOld)
	{
		ldRetVal = (ldClose - ldCloseOld) / ldCloseOld * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}
	return ldRetVal;
}*/

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ ROC = (당일종가 / n일전종가) * 100 }}}
-----------------------------------------------------------------------------*/
double yesROC(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldCloseOld;
	if (lIndex < lPeriod || lPeriod < 1)
	{
		return NONAVAILABLE;
	}	
	ldClose = GET_AT(SRC_VAR, lIndex);
	ldCloseOld = GET_AT(SRC_VAR, lIndex - lPeriod);
	if (ldCloseOld)
	{
		ldRetVal = (ldClose / ldCloseOld) * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}
	return ldRetVal;
}

double yesPVI(long lIsUpdateData, long lIndex, 
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldVolume;
	double *ldpTemp;
	double *ldpLastIndex;
	double *ldpLastClose;
	double *ldpLastVolume;
	double *ldpLastResult;
	double *ldpBeforeClose;
	double *ldpBeforeVolume;
	double *ldpBeforeResult;

	ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;
    ldpLastIndex = &ldpTemp[0];
    ldpLastClose = &ldpTemp[1];
    ldpLastVolume = &ldpTemp[2];
    ldpLastResult = &ldpTemp[3];
    ldpBeforeClose = &ldpTemp[4];
    ldpBeforeVolume = &ldpTemp[5];
    ldpBeforeResult = &ldpTemp[6];

	ldClose = GET_AT(SRC_VAR, lIndex);	
	ldVolume = GET_AT(VOLUME_VAR, lIndex);	

	if (lIndex > 0)
	{
		if (lIndex != *ldpLastIndex)
		{
			*ldpBeforeClose = *ldpLastClose;
			*ldpBeforeVolume = *ldpLastVolume;
			*ldpBeforeResult = *ldpLastResult;
		}

		if (ldVolume > *ldpBeforeVolume)
		{
			if (*ldpBeforeClose)
			{
				ldRetVal = *ldpBeforeResult + 
						(ldClose - *ldpBeforeClose) / *ldpBeforeClose * 100.;
			}
			else
			{
				ldRetVal = 0.;
			}
		}
		else
		{
			ldRetVal = *ldpBeforeResult;
		}
	}
	else
	{
		ldRetVal = 100.;
	}

	*ldpLastIndex = lIndex;
	*ldpLastClose = ldClose;
	*ldpLastVolume = ldVolume;
	*ldpLastResult = ldRetVal;

	return ldRetVal;
}

double yesPVT(long lIsUpdateData, long lIndex, 
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldCloseOld;
	double ldVolume;
	double ldVal;

	if (lIndex < 1)
	{
		return NONAVAILABLE;
	}

	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldVolume = GET_AT(VOLUME_VAR, lIndex);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex - 1);

	if (ldCloseOld * ldVolume)
	{
		ldVal = (ldClose - ldCloseOld) / ldCloseOld * ldVolume;
	}
	else
	{
		ldVal = 0.;
	}

	SET_AT(OPT_VAR, lIndex, ldVal);

	ldRetVal = yesAccum(lIsUpdateData, lIndex, OPT_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	return ldRetVal;
}

double yesRSI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldCloseOld;
	double ldVal1;
	double ldVal2;
	double ldMa1;
	double ldMa2;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex - 1);

	ldVal1 = ldClose - ldCloseOld;

	if (ldVal1 > 0.)
	{
		ldVal2 = ldVal1;
	}
	else
	{
		ldVal1 = 0.;
		ldVal2 = ldCloseOld - ldClose;
	}

	SET_AT(OPT_VAR1, lIndex, ldVal1);
	SET_AT(OPT_VAR2, lIndex, ldVal2);

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}
	
	ldMa1 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldMa2 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, 
				GET_SUB_TEMP(TEMP_VAR, 2));
	if (ldMa2)
	{
		ldRetVal = ldMa1 / ldMa2 * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	if (lIndex + 1 < lPeriod + 1)
	{
		return NONAVAILABLE;
	}

	return ldRetVal;
}

double yesSar(long lIsUpdateData, long lIndex, double dAF, double dMaxAF, 
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldClose;
	double *ldpTemp;
	double *ldpTempSub;
	double *ldpLastIndex;
	double *ldpLastResult;
	double *ldpBeforeResult;
	double *ldpBeforeHighValue;
	double *ldpBeforeLowValue;
	double *ldpLastHighValue;
	double *ldpLastLowValue;
	double *ldpLastPos;
	double *ldpBeforePos;
	double *ldpBeforeAF;
	double *ldpLastAF;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return NONAVAILABLE;

	ldpLastIndex = &ldpTemp[0];
	ldpLastResult = &ldpTemp[1];
	ldpBeforeResult = &ldpTemp[2];
	ldpBeforeHighValue = &ldpTemp[3];
	ldpBeforeLowValue = &ldpTemp[4];
	ldpLastPos = &ldpTemp[5];
	ldpBeforePos = &ldpTemp[6];
	ldpLastHighValue = &ldpTemp[7];
	ldpLastLowValue = &ldpTemp[8];

    ldpTempSub = GET_TEMP_DBL(GET_SUB_TEMP(TEMP_VAR, 1));
    if (ldpTempSub == NULL) return NONAVAILABLE;
	ldpBeforeAF = &ldpTempSub[0];
	ldpLastAF = &ldpTempSub[1];

	/***
	YES 에서는 0, 1, 2 까지는 계산이 안되고, 3번째는 0 ~ 3 까지의
	최고값을 리턴한다. 
	그날 그날 넘어가는 갯수가 다름. 먼가 공식이 있는듯함.
	***/
	if (lIndex < 5)
	{
		ldHigh = GET_AT(HIGH_VAR, lIndex);
		ldLow = GET_AT(LOW_VAR, lIndex); 

		if (lIndex == 0)
		{
			*ldpBeforeHighValue = ldHigh;
			*ldpBeforeLowValue = ldLow;
			*ldpLastHighValue = ldHigh; 
			*ldpLastLowValue = ldLow;

			*ldpBeforeAF = dAF;
			*ldpLastAF = dAF;
		}
		else
		{
			if ( lIndex != *ldpLastIndex)
			{
				*ldpBeforeHighValue = *ldpLastHighValue; 
				*ldpBeforeLowValue = *ldpLastLowValue;
			}
			else
			{
				*ldpLastHighValue = *ldpBeforeHighValue; 
				*ldpLastLowValue = *ldpBeforeLowValue;
			}

			if (ldHigh > *ldpBeforeHighValue) *ldpLastHighValue = ldHigh;
			if (ldLow < *ldpBeforeLowValue) *ldpLastLowValue = ldLow;
		}

		*ldpLastIndex = lIndex;
		*ldpLastResult = *ldpLastHighValue;
		*ldpLastPos = -1.;
		*ldpBeforeResult = *ldpLastResult;
		*ldpBeforePos = *ldpLastPos;

		if (lIndex == 4)
		{
			return *ldpLastResult;	
		}
		else
		{
			return NONAVAILABLE;
		}
	}

	if (lIndex != *ldpLastIndex)
	{
		*ldpBeforeResult = *ldpLastResult;
		*ldpBeforePos = *ldpLastPos;
		*ldpBeforeHighValue = *ldpLastHighValue; 
		*ldpBeforeLowValue = *ldpLastLowValue;
		*ldpBeforeAF = *ldpLastAF;
	}
	else
	{
		*ldpLastHighValue = *ldpBeforeHighValue; 
		*ldpLastLowValue = *ldpBeforeLowValue;
		*ldpLastAF = *ldpBeforeAF;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex); 
	ldClose = GET_AT(CLOSE_VAR, lIndex); 

	if (*ldpLastPos == 1. && ldLow <= *ldpBeforeResult)
	{
		*ldpLastPos = -1.;
	}
	else if (*ldpLastPos == -1. && ldHigh >= *ldpBeforeResult)
	{
		*ldpLastPos = 1.;
	}

	if (*ldpLastPos == 1.)
	{
		if (*ldpBeforePos != 1.)
		{
			*ldpLastResult = *ldpLastLowValue;
			*ldpLastAF = dAF;
			*ldpLastHighValue = ldHigh;
			*ldpLastLowValue = ldLow;
		}
		else
		{
			*ldpLastResult = *ldpBeforeResult + *ldpLastAF *
						(*ldpLastHighValue - *ldpBeforeResult);

			if (ldHigh > *ldpLastHighValue)
			{
				*ldpLastHighValue = ldHigh;
			}

			if (*ldpLastHighValue > *ldpBeforeHighValue &&
				*ldpLastAF < dMaxAF)
			{
				*ldpLastAF = *ldpLastAF + 
				(dAF < (dMaxAF - *ldpLastAF) ? dAF : (dMaxAF - *ldpLastAF));
			}
		}

		if (*ldpLastResult > ldLow && 
			*ldpBeforeResult < GET_AT(LOW_VAR, lIndex - 1))
		{
			*ldpLastResult = *ldpLastHighValue;
			*ldpLastPos = -1.;	
			*ldpLastAF = dAF;
			*ldpLastHighValue = ldHigh;
			*ldpLastLowValue = ldLow;
		}
	}
	else
	{
		if (*ldpBeforePos != -1.)
		{
			*ldpLastResult = *ldpLastHighValue;
			*ldpLastAF = dAF;
			*ldpLastHighValue = ldHigh;
			*ldpLastLowValue = ldLow;
		}
		else
		{
			*ldpLastResult = *ldpBeforeResult + *ldpLastAF *
						(*ldpLastLowValue - *ldpBeforeResult);

			if (ldLow < *ldpLastLowValue)
			{
				*ldpLastLowValue = ldLow;
			}

			if (*ldpLastLowValue < *ldpBeforeLowValue &&
				*ldpLastAF < dMaxAF)
			{
				*ldpLastAF = *ldpLastAF + 
				(dAF < (dMaxAF - *ldpLastAF) ? dAF : (dMaxAF - *ldpLastAF));
			}
		}

		if (*ldpLastResult < ldHigh && 
			*ldpBeforeResult > GET_AT(HIGH_VAR, lIndex - 1))
		{
			*ldpLastResult = *ldpLastLowValue;
			*ldpLastPos = 1.;	
			*ldpLastAF = dAF;
			*ldpLastHighValue = ldHigh;
			*ldpLastLowValue = ldLow;
		}
	}

	*ldpLastIndex = lIndex;
	ldRetVal = *ldpLastResult;

	return ldRetVal;
}

double yesSONAR(long lIsUpdateData, long lIndex, long lPeriod, long mPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal;

//	if (lIndex + 1 < lPeriod || lPeriod < 1 || lIndex + 1 < mPeriod || mPeriod < 1)
	if (lIndex < 1)
	{
		return NONAVAILABLE;
	}
	
	ldVal = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, OPT_VAR2,
					GET_SUB_TEMP(TEMP_VAR, 1));
	
	SET_AT(OPT_VAR1, lIndex, ldVal);

//	if (lIndex + 1 < lPeriod + mPeriod)
	if (lIndex + 1 < mPeriod)
	{
		return NONAVAILABLE;
	}

	ldRetVal = ldVal - GET_AT(OPT_VAR1, lIndex - mPeriod);

	return ldRetVal;
}

double yesSPrice(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE OPEN_VAR,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;

	if (lIndex < lPeriod || lPeriod < 0)
	{
		return NONAVAILABLE;
	}

	ldRetVal = (GET_AT(OPEN_VAR, lIndex - lPeriod) +
				GET_AT(HIGH_VAR, lIndex - lPeriod) +
				GET_AT(LOW_VAR, lIndex - lPeriod) +
				GET_AT(CLOSE_VAR, lIndex - lPeriod)) / 4.;

	return ldRetVal;
}

double yesSimrido(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldVal = GET_AT(CLOSE_VAR, lIndex) > GET_AT(CLOSE_VAR, lIndex - 1);

	SET_AT(OPT_VAR, lIndex, ldVal);

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldRetVal = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1)) *
				100. / lPeriod;;
		
	if (lIndex + 1 < lPeriod + 1)
	{
		return NONAVAILABLE;
	}

	return ldRetVal;
}

/*	yesStochasticsD -> yesStoSlowD	*/
double yesStochasticsD(long lIsUpdateData, long lIndex, long lPeriod1, 
		long lPeriod2, long lPeriod3,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		UNKNOWN_DATA_TYPE OPT_VAR5,
		UNKNOWN_DATA_TYPE OPT_VAR6,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal1;
	double ldVal2;
	double ldVal3;
	double ldVal4;

	if (lPeriod1 < 1 || lPeriod2 < 1 || lPeriod3 < 1)
	{
		return NONAVAILABLE;
	}

/*
	if (lIndex + 1 < lPeriod1)
//	{
//		return NONAVAILABLE;
//	}
*/

	ldVal1 = yesLowest(lIsUpdateData, lIndex, lPeriod1, LOW_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal2 = yesHighest(lIsUpdateData, lIndex, lPeriod1, HIGH_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	SET_AT(OPT_VAR1, lIndex, ldVal1);
	SET_AT(OPT_VAR2, lIndex, ldVal2);

	// gm0604 modified..2005.9.12
	if(ldVal1==NONAVAILABLE||ldVal2==NONAVAILABLE)
	{
		ldVal3 = NONAVAILABLE;
	}
	else if (ldVal2 - ldVal1)
	{
		ldVal3 = (GET_AT(SRC_VAR, lIndex) - ldVal1) / (ldVal2 - ldVal1)*100.;
	}
	else
	{
		ldVal3 = 0.;
	}

	SET_AT(OPT_VAR3, lIndex, ldVal3);

	ldVal4 = yesEMA(lIsUpdateData, lIndex, lPeriod2, OPT_VAR3, OPT_VAR5,
				GET_SUB_TEMP(TEMP_VAR, 3));	

/*	if (lIndex + 1 < lPeriod1 + lPeriod2 - 1)
	{
		return NONAVAILABLE;
	}*/

	SET_AT(OPT_VAR4, lIndex, ldVal4);

	ldRetVal = yesEMA(lIsUpdateData, lIndex, lPeriod3, OPT_VAR4, OPT_VAR6,
				GET_SUB_TEMP(TEMP_VAR, 4));

/*	if (lIndex + 1 < lPeriod1 + lPeriod2 + lPeriod3 - 2)
	{
		return NONAVAILABLE;
	}*/

	return ldRetVal;	
}

/*	yesStochasticsK -> yesStoSlowK	*/
double yesStochasticsK(long lIsUpdateData, long lIndex, long lPeriod1, 
		long lPeriod2,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal1;
	double ldVal2;
	double ldVal3;

	if (lPeriod1 < 1 || lPeriod2 < 1)
	{
		return NONAVAILABLE;
	}

/*
	if (lIndex + 1 < lPeriod1)
	{
		return NONAVAILABLE;
	}
*/

	ldVal1 = yesLowest(lIsUpdateData, lIndex, lPeriod1, LOW_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal2 = yesHighest(lIsUpdateData, lIndex, lPeriod1, HIGH_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	SET_AT(OPT_VAR1, lIndex, ldVal1);
	SET_AT(OPT_VAR2, lIndex, ldVal2);

	// gm0604 modified..2005.9.12
	if(ldVal1==NONAVAILABLE||ldVal2==NONAVAILABLE)
	{
		ldVal3 = NONAVAILABLE;
	}
	else if (ldVal2 - ldVal1)
	{
		ldVal3 = (GET_AT(SRC_VAR, lIndex) - ldVal1) / (ldVal2 - ldVal1)*100.;
	}
	else
	{
		ldVal3 = 0.;
	}

	SET_AT(OPT_VAR3, lIndex, ldVal3);

	ldRetVal = yesEMA(lIsUpdateData, lIndex, lPeriod2, OPT_VAR3, OPT_VAR4,
				GET_SUB_TEMP(TEMP_VAR, 3));

/*
	if (lIndex + 1 < lPeriod1 + lPeriod2  - 1)
	{
		return NONAVAILABLE;
	}
*/

	return ldRetVal;
}

double yesTRIX(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		UNKNOWN_DATA_TYPE OPT_VAR5,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal1;
	double ldVal2;
	double ldVal3;
	double *ldpTemp;
	double *ldpLastIndex;
	double *ldpLastValue3;
	double *ldpBeforeValue3;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;
    ldpLastIndex = &ldpTemp[0];
    ldpLastValue3 = &ldpTemp[1];
    ldpBeforeValue3 = &ldpTemp[2];

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldVal1 = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, OPT_VAR3,
				GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(OPT_VAR1, lIndex, ldVal1);

	ldVal2 = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, OPT_VAR4,
				GET_SUB_TEMP(TEMP_VAR, 2));
	if (lIndex + 1 < lPeriod * 2 - 1)
	{
		return NONAVAILABLE;
	}

	SET_AT(OPT_VAR2, lIndex, ldVal2);

	ldVal3 = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, OPT_VAR5,
				GET_SUB_TEMP(TEMP_VAR, 3));
	if (ldVal3 == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	if (lIndex + 1 < lPeriod * 3 - 1)
	{
		*ldpLastIndex = lIndex;
		*ldpLastValue3 = ldVal3;

		return NONAVAILABLE;
	}

	if (*ldpLastIndex > 0 && lIndex != *ldpLastIndex)
	{
		*ldpBeforeValue3 = *ldpLastValue3;
	}
	
	if (*ldpBeforeValue3)
	{
		ldRetVal = (ldVal3 - *ldpBeforeValue3) / *ldpBeforeValue3 * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	*ldpLastIndex = lIndex;
	*ldpLastValue3 = ldVal3;


	return ldRetVal;
}

double yesVHF(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal;
	double ldHighest;
	double ldLowest;
	double ldAccumN;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldVal = fabs(GET_AT(CLOSE_VAR, lIndex) - GET_AT(CLOSE_VAR, lIndex - 1));

	SET_AT(OPT_VAR, lIndex, ldVal);

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}

	ldHighest = yesHighest(lIsUpdateData, lIndex, lPeriod, CLOSE_VAR, 
					GET_SUB_TEMP(TEMP_VAR, 2));
	ldLowest = yesLowest(lIsUpdateData, lIndex, lPeriod, CLOSE_VAR, 
					GET_SUB_TEMP(TEMP_VAR, 3));
	ldAccumN = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR, 
					GET_SUB_TEMP(TEMP_VAR, 1));
	if (ldAccumN)
	{
		ldRetVal = fabs(ldHighest - ldLowest) / ldAccumN;
	}
	else
	{
		ldRetVal = 0.;
	}

	if (lIndex + 1 < lPeriod + 1)
	{
		return NONAVAILABLE;
	}

	return ldRetVal;
}

double yesVR(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldCloseOld;
	double ldVolume;
	double ldVal1;
	double ldVal2;
	double ldVal3;
	double ldAccumN1;
	double ldAccumN2;
	double ldAccumN3;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldClose = GET_AT(CLOSE_VAR, lIndex);
	ldVolume = GET_AT(VOLUME_VAR, lIndex);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex - 1);

	ldVal1 = (ldClose > ldCloseOld ? ldVolume : 0.);
	ldVal2 = (ldClose < ldCloseOld ? ldVolume : 0.);
	ldVal3 = (ldClose == ldCloseOld ? ldVolume : 0.);

	SET_AT(OPT_VAR1, lIndex, ldVal1);
	SET_AT(OPT_VAR2, lIndex, ldVal2);
	SET_AT(OPT_VAR3, lIndex, ldVal3);

	if (lIndex + 1 < lPeriod)
	{
		return NONAVAILABLE;
	}
	
	ldAccumN1 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, 
					GET_SUB_TEMP(TEMP_VAR, 1));
	ldAccumN2 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, 
					GET_SUB_TEMP(TEMP_VAR, 2));
	ldAccumN3 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR3, 
					GET_SUB_TEMP(TEMP_VAR, 3));

	if (lIndex + 1 < lPeriod + 1)
	{
		return NONAVAILABLE;
	}

	if (ldAccumN2 + ldAccumN3 / 2.)
	{
		ldRetVal = (ldAccumN1 + ldAccumN3 / 2.) /
					(ldAccumN2 + ldAccumN3 / 2.) * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;	
}

double yesVROC(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVolume;
	double ldVolumeOld;

	if (lIndex < lPeriod || lPeriod < 0)
	{
		return NONAVAILABLE;
	}

	ldVolume = GET_AT(VOLUME_VAR, lIndex);
	ldVolumeOld = GET_AT(VOLUME_VAR, lIndex - lPeriod);

	if (ldVolumeOld)
	{
		/*구차트 부분
		ldRetVal = (ldVolume - ldVolumeOld) / ldVolumeOld * 100.;
		*/
		// 신차트로 수정(2006.04.19)
		ldRetVal = ldVolume / (ldVolumeOld+0.0001) * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;	
}

double yesWC(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldSum;

	ldSum = GET_AT(CLOSE_VAR, lIndex) * 2. +
			GET_AT(HIGH_VAR, lIndex) +
			GET_AT(LOW_VAR, lIndex);

	if (ldSum)
	{
		ldRetVal = ldSum / 4.;	
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

double yesWILLA(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldClose;
	double ldCloseOld;
	double ldVal1;
	double ldVal2;

	if (lIndex < 1)
	{
		return NONAVAILABLE;
	}

	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldClose = GET_AT(SRC_VAR, lIndex);
	ldCloseOld = GET_AT(SRC_VAR, lIndex - 1);

	ldVal1 = (ldClose < ldCloseOld ? 
				(ldClose - (ldCloseOld > ldHigh ? ldCloseOld : ldHigh)) : 0.);
	ldVal2 = (ldClose > ldCloseOld ? 
				(ldClose - (ldCloseOld < ldLow ? ldCloseOld : ldLow)) : ldVal1);

	SET_AT(OPT_VAR, lIndex, ldVal2);

	ldRetVal = yesAccum(lIsUpdateData, lIndex, OPT_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	return ldRetVal;
}

double yesWILLR(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldVal1;
	double ldVal2;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldVal1 = yesHighest(lIsUpdateData, lIndex, lPeriod, HIGH_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal2 = yesLowest(lIsUpdateData, lIndex, lPeriod, LOW_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	if (lIndex + 1 < lPeriod) 
	{
		return NONAVAILABLE;
	}

	ldClose = GET_AT(SRC_VAR, lIndex);

	if (ldVal1 - ldVal2)
	{
		ldRetVal = (ldVal1 - ldClose) / (ldVal1 - ldVal2) * (-100.);
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.4
 Modifier		: 
 Comments		: 강에너지 = 당일고가 - 당일시가
				  약에너지 = 당일시가 - 당일저가
				  AR = [ n 일간의 강에너지 합/ n일간의  약에너지 합 ] * 100
------------------------------------------------------------------------------*/
double yesARatio(long lIsUpdateData, long lIndex, long lPeriod, 
				 UNKNOWN_DATA_TYPE HIGH_VAR,
				 UNKNOWN_DATA_TYPE LOW_VAR,
				 UNKNOWN_DATA_TYPE OPEN_VAR,
				 UNKNOWN_DATA_TYPE SRC_VAR,		//Strong Energe
				 UNKNOWN_DATA_TYPE SRC_VAR1,	//weak Energe
				 TEMP_DATA_TYPE TEMP_VAR)
{
	int nPos;
    double dRetValue = NONAVAILABLE;
    double *ldpTemp;
    double *ldpStrongSum;
	double *ldpWeakSum;
    double *ldpSLastValue;
	double *ldpWLastValue;
	double dHigh;
	double dLow;
	double dOpen;
	double dStrong;
	double dWeak;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return 0.;
    ldpStrongSum = &ldpTemp[0];	
	ldpWeakSum = &ldpTemp[1];
    ldpSLastValue = &ldpTemp[2];
	ldpWLastValue = &ldpTemp[3];

	dHigh = GET_AT(HIGH_VAR,lIndex);
	dLow = GET_AT(LOW_VAR, lIndex);
	dOpen = GET_AT(OPEN_VAR, lIndex);

	dStrong = dHigh - dOpen;
	dWeak = dOpen - dLow;
	SET_AT(SRC_VAR, lIndex, dStrong);
	SET_AT(SRC_VAR1, lIndex, dWeak);

	if (GET_AT(SRC_VAR, lIndex) == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	if ((lPeriod-1) > lIndex)	/* calculation is unavailable, so return NONAVAILABLE without calculation*/
	{
		return NONAVAILABLE;
	}

	if (!*ldpStrongSum)	/* At First time, calculate Sum By looping... */
	{
		for(nPos = 0; nPos <= lIndex; nPos++)
		{
			*ldpStrongSum += GET_AT(SRC_VAR, nPos); /* Save Sum Of SrcValue..(Sum += SrcValue)*/
			*ldpWeakSum += GET_AT(SRC_VAR1, nPos);
		}
	}
	else
	{
		*ldpStrongSum += GET_AT(SRC_VAR, lIndex); /* Save Sum Of SrcValue..(Sum += SrcValue)*/
		*ldpWeakSum += GET_AT(SRC_VAR1, lIndex);
	}

    if (lPeriod <= lIndex) /* calculation is available*/
    {
        if(!lIsUpdateData)  /* insert */
        {
            *ldpStrongSum = *ldpStrongSum - GET_AT(SRC_VAR, lIndex-lPeriod);
			*ldpWeakSum = *ldpWeakSum - GET_AT(SRC_VAR1, lIndex-lPeriod);
        }
        else    /* update */
        {
             *ldpStrongSum = *ldpStrongSum - *ldpSLastValue;
			 *ldpWeakSum = *ldpWeakSum - *ldpWLastValue;
        }
		if(*ldpWeakSum)
		{
			dRetValue = (*ldpStrongSum / *ldpWeakSum) * 100;
		}
		else
		{
			dRetValue = 100;
		}        
    }
    else if(lPeriod == (lIndex+1))	/* calculation is available*/
    {
		if(lIsUpdateData)  /* update */
		{
             *ldpStrongSum = *ldpStrongSum - *ldpSLastValue;
			 *ldpWeakSum = *ldpWeakSum - *ldpWLastValue;
		}
		if(*ldpWeakSum)
		{
			dRetValue = (*ldpStrongSum / *ldpWeakSum) * 100;
		}
		else
		{
			dRetValue = 100;
		}        
    }
   	
    *ldpSLastValue = GET_AT(SRC_VAR, lIndex);    /* Save Last Value To Calculate updating */
	*ldpWLastValue = GET_AT(SRC_VAR1, lIndex);
    return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.5
 Modifier		: 
 Comments		: {{{ 강인기 = 당일고가 - 전일종가 }}}
				  {{{ 약인기 = 전일종가 - 당일저가 }}}
				  {{{ BR = [ n 일간의 강인기 합/ n일간의  약인기 합 ] * 100 }}}
------------------------------------------------------------------------------*/
double yesBRatio(long lIsUpdateData, long lIndex, long lPeriod, 
				 UNKNOWN_DATA_TYPE HIGH_VAR,
				 UNKNOWN_DATA_TYPE LOW_VAR,
				 UNKNOWN_DATA_TYPE CLOSE_VAR,
				 UNKNOWN_DATA_TYPE SRC_VAR,		//Strong 
				 UNKNOWN_DATA_TYPE SRC_VAR1,	//weak 
				 TEMP_DATA_TYPE TEMP_VAR)
{
	int nPos;
    double dRetValue = NONAVAILABLE;
    double *ldpTemp;
    double *ldpStrongSum;
	double *ldpWeakSum;
    double *ldpSLastValue;
	double *ldpWLastValue;
	double dHigh;
	double dLow;
	double dCloseOld;
	double dStrong;
	double dWeak;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return 0.;
    ldpStrongSum = &ldpTemp[0];	
	ldpWeakSum = &ldpTemp[1];
    ldpSLastValue = &ldpTemp[2];
	ldpWLastValue = &ldpTemp[3];

	if( 1 > lIndex)	/* calculation is start at index 2... */
	{
		return NONAVAILABLE;
	}

	dHigh = GET_AT(HIGH_VAR,lIndex);
	dLow = GET_AT(LOW_VAR, lIndex);
	dCloseOld = GET_AT(CLOSE_VAR, lIndex-1);

	dStrong = dHigh - dCloseOld;
	dWeak = dCloseOld - dLow;
	SET_AT(SRC_VAR, lIndex, dStrong);
	SET_AT(SRC_VAR1, lIndex, dWeak);

	if (GET_AT(SRC_VAR, lIndex) == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	if ((lPeriod-1) > lIndex)	/* calculation is unavailable, so return NONAVAILABLE without calculation*/
	{
		return NONAVAILABLE;
	}

	if (!*ldpStrongSum)	/* At First time, calculate Sum By looping... */
	{
		for(nPos = 0; nPos <= lIndex; nPos++)
		{
			*ldpStrongSum += GET_AT(SRC_VAR, nPos); /* Save Sum Of SrcValue..(Sum += SrcValue)*/
			*ldpWeakSum += GET_AT(SRC_VAR1, nPos);
		}
	}
	else
	{
		*ldpStrongSum += GET_AT(SRC_VAR, lIndex); /* Save Sum Of SrcValue..(Sum += SrcValue)*/
		*ldpWeakSum += GET_AT(SRC_VAR1, lIndex);
	}

    if (lPeriod <= lIndex) /* calculation is available*/
    {
        if(!lIsUpdateData)  /* insert */
        {
            *ldpStrongSum = *ldpStrongSum - GET_AT(SRC_VAR, lIndex-lPeriod);
			*ldpWeakSum = *ldpWeakSum - GET_AT(SRC_VAR1, lIndex-lPeriod);
        }
        else    /* update */
        {
             *ldpStrongSum = *ldpStrongSum - *ldpSLastValue;
			 *ldpWeakSum = *ldpWeakSum - *ldpWLastValue;
        }
		if(*ldpWeakSum)
		{
	        dRetValue = (*ldpStrongSum / *ldpWeakSum) * 100;
		}
		else
		{
			dRetValue = 100;
		}
    }
    else if(lPeriod == (lIndex+1))	/* calculation is available*/
    {
		if(lIsUpdateData)  /* update */
		{
             *ldpStrongSum = *ldpStrongSum - *ldpSLastValue;
			 *ldpWeakSum = *ldpWeakSum - *ldpWLastValue;
		}
		if(*ldpWeakSum)
		{
	        dRetValue = (*ldpStrongSum / *ldpWeakSum) * 100;
		}
		else
		{
			dRetValue = 100;
		}
    }
   	
    *ldpSLastValue = GET_AT(SRC_VAR, lIndex);    /* Save Last Value To Calculate updating */
	*ldpWLastValue = GET_AT(SRC_VAR1, lIndex);

    return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.5
 Modifier		: 
 Comments		: {{{ 고가유지기간 : 고가의 최고가가 고가인 날과 당일 일수 차 }}}
				  {{{ Aroon Up =  (N-고가유지기간)/N*100 }}}
------------------------------------------------------------------------------*/
double yesAroonUp(long lIsUpdateData, long lIndex, long lPeriod, 
				 UNKNOWN_DATA_TYPE HIGH_VAR,
				 TEMP_DATA_TYPE TEMP_VAR)
{
	int nPos;
    double dRetValue = NONAVAILABLE;
    double *ldpTemp;
    double *ldpTopPrice;
	double *ldpTopDate;
	double dHigh;
	double dMaintainPeriod;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return 0.;
    ldpTopPrice = &ldpTemp[0];	
	ldpTopDate = &ldpTemp[1];
	
	if (lIndex == 0)	/* calculation is unavailable, so return NONAVAILABLE without calculation*/
	{
		*ldpTopPrice = GET_AT(HIGH_VAR,lIndex);
		*ldpTopDate = lIndex;
		return NONAVAILABLE;
	}

	if(lIndex < lPeriod) 
	{
		dHigh = GET_AT(HIGH_VAR, lIndex);
		if(dHigh >= *ldpTopPrice)
		{
			*ldpTopPrice = dHigh;
			*ldpTopDate = lIndex;
		}
		return NONAVAILABLE;
	}

	if(lIndex == lPeriod)
	{
		dHigh = GET_AT(HIGH_VAR, lIndex);
		if(dHigh >= *ldpTopPrice)
		{
			*ldpTopPrice = dHigh;
			*ldpTopDate = lIndex;
		}
		dMaintainPeriod= lIndex - *ldpTopDate;    	
		dRetValue = (lPeriod - dMaintainPeriod)/lPeriod*100;
	}
	else if(lIndex > lPeriod)
	{
		dHigh = GET_AT(HIGH_VAR, lIndex);
		if(dHigh >= *ldpTopPrice)
		{
			*ldpTopPrice = dHigh;
			*ldpTopDate = lIndex;
		}
		else if(*ldpTopDate < lIndex-lPeriod)
		{
			*ldpTopPrice = GET_AT(HIGH_VAR, lIndex);
			for(nPos=lIndex-lPeriod; nPos<=lIndex; nPos++)
			{
				dHigh = GET_AT(HIGH_VAR, nPos);
				if(dHigh >= *ldpTopPrice)
				{
					*ldpTopPrice = dHigh;
					*ldpTopDate = nPos;
				}
			}
		}
		dMaintainPeriod= lIndex - *ldpTopDate;    	
		dRetValue = (lPeriod - dMaintainPeriod)/lPeriod*100;
	}

    return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.5
 Modifier		: 
 Comments		: {{{ 저가유지기간 : 저가의 최저가가 저가인 날과 당일 일수 차 }}}
				  {{{ Aroon Down =  (N-저가유지기간)/N*100 }}}
------------------------------------------------------------------------------*/
double yesAroonDown(long lIsUpdateData, long lIndex, long lPeriod, 
				 UNKNOWN_DATA_TYPE LOW_VAR,
				 TEMP_DATA_TYPE TEMP_VAR)
{
	int nPos;
    double dRetValue = NONAVAILABLE;
    double *ldpTemp;
    double *ldpLowestPrice;
	double *ldpLowestDate;
	double dLow;
	double dMaintainPeriod;

    ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return 0.;
    ldpLowestPrice = &ldpTemp[0];	
	ldpLowestDate = &ldpTemp[1];
	
	if (lIndex == 0)	/* calculation is unavailable, so return NONAVAILABLE without calculation*/
	{
		*ldpLowestPrice = GET_AT(LOW_VAR,lIndex);
		*ldpLowestDate = lIndex;
		return NONAVAILABLE;
	}

	if(lIndex < lPeriod) 
	{
		dLow = GET_AT(LOW_VAR, lIndex);
		if(dLow <= *ldpLowestPrice)
		{
			*ldpLowestPrice = dLow;
			*ldpLowestDate = lIndex;
		}
		return NONAVAILABLE;
	}

	if(lIndex == lPeriod)
	{
		dLow = GET_AT(LOW_VAR, lIndex);
		if(dLow <= *ldpLowestPrice)
		{
			*ldpLowestPrice = dLow;
			*ldpLowestDate = lIndex;
		}
		dMaintainPeriod= lIndex - *ldpLowestDate;    	
		dRetValue = (lPeriod - dMaintainPeriod)/lPeriod*100;
	}
	else if(lIndex > lPeriod)
	{
		dLow = GET_AT(LOW_VAR, lIndex);
		if(dLow <= *ldpLowestPrice)
		{
			*ldpLowestPrice = dLow;
			*ldpLowestDate = lIndex;
		}
		else if(*ldpLowestDate < lIndex-lPeriod)
		{
			*ldpLowestPrice = GET_AT(LOW_VAR, lIndex);
			for(nPos=lIndex-lPeriod; nPos<=lIndex; nPos++)
			{
				dLow = GET_AT(LOW_VAR, nPos);
				if(dLow <= *ldpLowestPrice)
				{
					*ldpLowestPrice = dLow;
					*ldpLowestDate = nPos;
				}
			}
		}
		dMaintainPeriod= lIndex - *ldpLowestDate;    	
		dRetValue = (lPeriod - dMaintainPeriod)/lPeriod*100;
	}

    return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.7
 Modifier		: 
 Comments		: {{{ SROC = 당일 EMA / n일전EMA*100 }}}
-----------------------------------------------------------------------------*/
double yesSROC(long lIsUpdateData, long lIndex, 
			   long lPeriod1, long lPeriod2,
			   UNKNOWN_DATA_TYPE SRC_VAR1,
			   UNKNOWN_DATA_TYPE SRC_VAR2,
			   UNKNOWN_DATA_TYPE OPT_VAR1,
			   UNKNOWN_DATA_TYPE OPT_VAR2,
			   TEMP_DATA_TYPE TEMP_VAR)
{
    double dRetValue = NONAVAILABLE;
	double dDayEMA;
	double dPreEMA;

	if (lPeriod1 < 1)
	{
		return NONAVAILABLE;
	}

   	if (lIndex < lPeriod2)
	{
		dDayEMA = yesEMA(lIsUpdateData, lIndex, lPeriod1, SRC_VAR1, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));
		SET_AT(SRC_VAR2, lIndex, dDayEMA);
		return NONAVAILABLE;
	}

	dDayEMA = yesEMA(lIsUpdateData, lIndex, lPeriod1, SRC_VAR1, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(SRC_VAR2, lIndex, dDayEMA);
	dPreEMA = GET_AT(SRC_VAR2, lIndex-lPeriod2);
	
	dRetValue = dDayEMA / dPreEMA * 100;
	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.8
 Modifier		: 
 Comments		: {{{ Force Index = [ (당일종가 - 전일종가) * 당일거래량 ]의 지수이동평균 }}}
				  {{{ EMA = 전일EMA + 2/(N+1) * (당일종가 - 전일EMA) }}}	
-----------------------------------------------------------------------------*/
double yesForceIndex(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE VOLUME_VAR,
				UNKNOWN_DATA_TYPE SRC_VAR1, 
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR1,				
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dClose;
	double dCloseOld;
	double dVolume;
	double dValue;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	if(0 == lIndex)
	{  
		return NONAVAILABLE;
	}

	dClose = GET_AT(SRC_VAR1,lIndex);
	dCloseOld = GET_AT(SRC_VAR1, lIndex-1);
	dVolume = GET_AT(VOLUME_VAR, lIndex);

	dValue = (dClose - dCloseOld) * dVolume;
	SET_AT(SRC_VAR2, lIndex, dValue);
	dRetValue = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, OPT_VAR1,GET_SUB_TEMP(TEMP_VAR, 1));
	
	return dRetValue;
	
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.8
 Modifier		: 
 Comments		: {{{ DEMA = EMA의 EMA}}}
-----------------------------------------------------------------------------*/
double yesDEMA(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR1,
				UNKNOWN_DATA_TYPE OPT_VAR2,				
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dEMA1;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	dEMA1 = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(SRC_VAR2, lIndex, dEMA1);
	dRetValue = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 2));
	
	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.11
 Modifier		: 
 Comments		: {{{ DEMA2 = 2*EMA - 2중EMA}}}
-----------------------------------------------------------------------------*/
double yesDEMA2(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR1,
				UNKNOWN_DATA_TYPE OPT_VAR2,				
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dEMA1;
	double dEMA2;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	dEMA1 = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(SRC_VAR2, lIndex, dEMA1);
	dEMA2 = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 2));
	dRetValue = 2 * dEMA1 - dEMA2;
	
	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.11
 Modifier		: 
 Comments		: {{{ TEMA = EMA의 EMA의 EMA }}}
-----------------------------------------------------------------------------*/
double yesTEMA(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE SRC_VAR3,
				UNKNOWN_DATA_TYPE OPT_VAR1,
				UNKNOWN_DATA_TYPE OPT_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR3,				
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dEMA;
	double dDEMA;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	dEMA = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(SRC_VAR2, lIndex, dEMA);
	dDEMA = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 2));
	SET_AT(SRC_VAR3, lIndex, dDEMA);
	dRetValue = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR3, OPT_VAR3, GET_SUB_TEMP(TEMP_VAR, 3));
	
	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.11
 Modifier		: 
 Comments		: {{{ TEMA2 = (3*EMA) - (3*2중EMA) + 3중EMA }}}
-----------------------------------------------------------------------------*/
double yesTEMA2(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE SRC_VAR3,
				UNKNOWN_DATA_TYPE OPT_VAR1,
				UNKNOWN_DATA_TYPE OPT_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR3,				
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dEMA;
	double dDEMA;
	double dTEMA;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	dEMA = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(SRC_VAR2, lIndex, dEMA);
	dDEMA = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 2));
	SET_AT(SRC_VAR3, lIndex, dDEMA);
	dTEMA = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR3, OPT_VAR3, GET_SUB_TEMP(TEMP_VAR, 3));

	dRetValue = ( 3 * dEMA) - (3*dDEMA) + dTEMA;
	
	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.11
 Modifier		: 
 Comments		: {{{ RMI = 100 - 100 / (1+ 상승모멘텀평균/하락모멘텀평균) }}}
-----------------------------------------------------------------------------*/
double yesRMI(long lIsUpdateData, long lIndex, 
			  long lPeriod, long lKPeriod,
				UNKNOWN_DATA_TYPE SRC_VAR,
				UNKNOWN_DATA_TYPE RISE_VAR,
				UNKNOWN_DATA_TYPE FALL_VAR,
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dMomentum;
	double dRiseMA;
	double dFallMA;

	dMomentum = yesMomentum(lIsUpdateData, lIndex, lKPeriod, SRC_VAR, GET_SUB_TEMP(TEMP_VAR, 1));
	

	if(dMomentum == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}


	if(dMomentum >0)
	{
		SET_AT(RISE_VAR, lIndex, dMomentum);
		SET_AT(FALL_VAR, lIndex, 0);
	}
	else
	{
		SET_AT(FALL_VAR, lIndex, dMomentum);
		SET_AT(RISE_VAR, lIndex, 0);
	}

	dRiseMA = yesSMA(lIsUpdateData, lIndex, lPeriod, RISE_VAR, GET_SUB_TEMP(TEMP_VAR, 2));
	dFallMA = yesSMA(lIsUpdateData, lIndex, lPeriod, FALL_VAR, GET_SUB_TEMP(TEMP_VAR, 3));
	dFallMA = fabs(dFallMA);

	if(dRiseMA == NONAVAILABLE || dFallMA == NONAVAILABLE)
	{
		return NONAVAILABLE;
	}

	if(dFallMA == 0)
	{
		dRetValue = 100;
	}
	else
	{
		dRetValue = 100 - 100 / (1 + dRiseMA/dFallMA);
	}

	return dRetValue;

}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ 이격도(지수) = 당일종가 / N일 지수이동평균 * 100 }}}
-----------------------------------------------------------------------------*/
double yesDisparity_EMA(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,		
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldClose;
	double ldTmp;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldClose = GET_AT(SRC_VAR, lIndex);
	
	ldTmp = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, OPT_VAR1,
				GET_SUB_TEMP(TEMP_VAR, 1));

	if(ldTmp == NONAVAILABLE || lIndex < lPeriod)
		return NONAVAILABLE;

	if (ldTmp)
	{
		ldRetVal = ldClose / ldTmp * 100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ MACD Osc, 이격도, ROC, Stockasric 지표조합지표 }}}
-----------------------------------------------------------------------------*/
double yesBinaryWave(long lIsUpdateData, long lIndex,
					 long lShortPeriod, long lLongPeriod, long lSignalPeriod, 
					 long lDisparityPeriod, long lROCPeriod, 
					 long lStoKPeriod, long lStoDPeriod,
					 UNKNOWN_DATA_TYPE HIGH_VAR,
					 UNKNOWN_DATA_TYPE LOW_VAR,
					 UNKNOWN_DATA_TYPE CLOSE_VAR,
					 UNKNOWN_DATA_TYPE MACD_VAL1,
					 UNKNOWN_DATA_TYPE MACD_VAL2,
					 UNKNOWN_DATA_TYPE MACD_VAL3,
					 UNKNOWN_DATA_TYPE MACD_VAL4,
					 UNKNOWN_DATA_TYPE STO_VAL1,
					 UNKNOWN_DATA_TYPE STO_VAL2,
					 UNKNOWN_DATA_TYPE STO_VAL3,
					 UNKNOWN_DATA_TYPE STO_VAL4,
					 TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dMACD_OSC;
	double dROC;
	double dDisparity;
	double dSTO_Fast;
	int nMACDNum;
	int nROCNum;
	int nDisparityNum;
	int nSTO_Fast;

	dMACD_OSC = yesMACD_OSC(lIsUpdateData, lIndex, lShortPeriod, lLongPeriod, lSignalPeriod
							, CLOSE_VAR, MACD_VAL1, MACD_VAL2, MACD_VAL3, MACD_VAL4,GET_SUB_TEMP(TEMP_VAR, 1));
	dROC = yesROC(lIsUpdateData, lIndex, lROCPeriod, CLOSE_VAR, GET_SUB_TEMP(TEMP_VAR, 2));
	dDisparity = yesDisparity(lIsUpdateData, lIndex, lDisparityPeriod, CLOSE_VAR, GET_SUB_TEMP(TEMP_VAR, 3));
/*	dSTO_Fast = yesStochasticsK(lIsUpdateData, lIndex, lStoKPeriod, lStoDPeriod, HIGH_VAR, LOW_VAR, CLOSE_VAR, STO_VAL1, STO_VAL2, STO_VAL3, GET_SUB_TEMP(TEMP_VAR, 4));*/
	dSTO_Fast = yesStoFastD(lIsUpdateData, lIndex, lStoKPeriod, lStoDPeriod
							, HIGH_VAR, LOW_VAR, CLOSE_VAR, STO_VAL1, STO_VAL2, STO_VAL3, STO_VAL4, GET_SUB_TEMP(TEMP_VAR, 4));

	if(dMACD_OSC == NONAVAILABLE || dROC == NONAVAILABLE || dDisparity == NONAVAILABLE || dSTO_Fast == NONAVAILABLE)
		return NONAVAILABLE;
	
	if(dMACD_OSC >0) 		
		nMACDNum = 1;
	else
		nMACDNum = -1;

	if(dROC>100)
		nROCNum = 1;
	else	
		nROCNum = -1;

	if(dDisparity>100)	
		nDisparityNum = 1;
	else	
		nDisparityNum = -1;

	if(dSTO_Fast>50)
		nSTO_Fast = 1;
	else
		nSTO_Fast = -1;

	dRetValue = nMACDNum + nROCNum + nDisparityNum + nSTO_Fast;

	return dRetValue;
}


/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ MACD Osc, 이격도, ROC, Stockasric 지표조합지표 }}}
-----------------------------------------------------------------------------*/
double yes7BinaryWave(long lIsUpdateData, long lIndex,
					 UNKNOWN_DATA_TYPE HIGH_VAR,
					 UNKNOWN_DATA_TYPE LOW_VAR,
					 UNKNOWN_DATA_TYPE CLOSE_VAR,
					 UNKNOWN_DATA_TYPE VOLUME_VAR,
					 UNKNOWN_DATA_TYPE MACD_VAL1,
					 UNKNOWN_DATA_TYPE MACD_VAL2,
					 UNKNOWN_DATA_TYPE MACD_VAL3,
					 UNKNOWN_DATA_TYPE MACD_VAL4,
					 UNKNOWN_DATA_TYPE STO_VAL1,
					 UNKNOWN_DATA_TYPE STO_VAL2,
					 UNKNOWN_DATA_TYPE STO_VAL3,
					 UNKNOWN_DATA_TYPE STO_VAL4,
					 UNKNOWN_DATA_TYPE CO_VAL1,
					 UNKNOWN_DATA_TYPE CO_VAL2,
					 UNKNOWN_DATA_TYPE CO_VAL3,
					 UNKNOWN_DATA_TYPE CO_VAL4,
					 UNKNOWN_DATA_TYPE CCI_VAL1,
					 UNKNOWN_DATA_TYPE CCI_VAL2,
					 UNKNOWN_DATA_TYPE CCI_VAL3,
					 UNKNOWN_DATA_TYPE SRC_VAL,
					 UNKNOWN_DATA_TYPE OPT_VAR1,					 
					 TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dCO;
	double dEMA;
	double dMACD_OSC;
	double dROC;
	double dSTO_Fast;
	double dCCI;
	double dPSAR;
/*CCI 매개변수 변경*/
	double dHigh;
	double dLow;
	double dClose;
	double dVal;

	dHigh = GET_AT(HIGH_VAR, lIndex);
	dLow = GET_AT(LOW_VAR, lIndex);
	dClose = GET_AT(CLOSE_VAR, lIndex);
	dVal = (dHigh + dLow + dClose)/3;
	SET_AT(SRC_VAL, lIndex, dVal);

	dCO = yesCO(lIsUpdateData,lIndex,3,10,HIGH_VAR,LOW_VAR,CLOSE_VAR,VOLUME_VAR, CO_VAL1, CO_VAL2, CO_VAL3, CO_VAL4,GET_SUB_TEMP(TEMP_VAR, 1));
	dEMA = yesEMA(lIsUpdateData,lIsUpdateData,9,CLOSE_VAR,OPT_VAR1,GET_SUB_TEMP(TEMP_VAR, 2));
	dMACD_OSC = yesMACD_OSC(lIsUpdateData, lIndex, 12, 26, 9
						, CLOSE_VAR, MACD_VAL1,MACD_VAL2, MACD_VAL3, MACD_VAL4,GET_SUB_TEMP(TEMP_VAR, 3));
	dROC = yesROC(lIsUpdateData, lIndex, 12, CLOSE_VAR, GET_SUB_TEMP(TEMP_VAR, 4));
	dSTO_Fast = yesStoFastD(lIsUpdateData, lIndex, 5, 3
						, HIGH_VAR, LOW_VAR, CLOSE_VAR, STO_VAL1, STO_VAL2, STO_VAL3, STO_VAL4, GET_SUB_TEMP(TEMP_VAR, 5));
/*	dCCI = yesCCI(lIsUpdateData,lIndex,8,HIGH_VAR,LOW_VAR,CLOSE_VAR,CCI_VAL1,CCI_VAL2,CCI_VAL3, GET_SUB_TEMP(TEMP_VAR, 6));*/
	dCCI = yesCCI(lIsUpdateData,lIndex,8,SRC_VAL,CCI_VAL1,CCI_VAL2,CCI_VAL3, GET_SUB_TEMP(TEMP_VAR, 6));
	dPSAR = yesSar(lIsUpdateData,lIndex,0.02,2.0,HIGH_VAR,LOW_VAR,CLOSE_VAR,GET_SUB_TEMP(TEMP_VAR, 7));
	
	if(	   dCO == NONAVAILABLE 
		|| dEMA == NONAVAILABLE 
		|| dMACD_OSC == NONAVAILABLE 
		|| dROC == NONAVAILABLE 
		|| dSTO_Fast == NONAVAILABLE 
		|| dCCI == NONAVAILABLE 
		|| dPSAR == NONAVAILABLE 		)
		return NONAVAILABLE;

	dRetValue =	  (dCO >0 ? 1 : -1) 
				+ (dEMA < GET_AT(CLOSE_VAR,lIndex) ? 1 : -1) 
				+ (dMACD_OSC >0 ? 1 : -1) 
				+ (dROC>100 ? 1 : -1)
				+ (dSTO_Fast>50 ? 1 : -1)
				+ (dCCI>0 ? 1 : -1)
				+ (dPSAR>GET_AT(CLOSE_VAR,lIndex) ? 1 : -1)	;

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ upper channel = 전일 기준 n일 간의 고가의 최대값 }}}
-----------------------------------------------------------------------------*/
double yesPriceChUpper(long lIsUpdateData, long lIndex, long lPeriod,
				UNKNOWN_DATA_TYPE HIGH_VAR,
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;

	if(lIndex +1 < lPeriod || lPeriod < 1)
		return NONAVAILABLE;

	dRetValue = yesHighest(lIsUpdateData, lIndex-1, lPeriod, HIGH_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ lower channel = 전일 기준 n일 간의 저가의 최소값 }}}
-----------------------------------------------------------------------------*/
double yesPriceChLower(long lIsUpdateData, long lIndex, long lPeriod,
				UNKNOWN_DATA_TYPE LOW_VAR,
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;

	if(lIndex +1 < lPeriod || lPeriod < 1)
		return NONAVAILABLE;

	dRetValue = yesLowest(lIsUpdateData, lIndex-1, lPeriod, LOW_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ starc center line = 종가 b일 단순이동평균 }}}
-----------------------------------------------------------------------------*/
double yesStarcCenter(long lIsUpdateData, long lIndex, long lPeriod,
				UNKNOWN_DATA_TYPE SRC_VAR1,
				TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	
	if (lPeriod < 1) /* || lIndex + 1 < lPeriod */
	{
		return NONAVAILABLE;
	}

	dRetValue = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, 
				GET_SUB_TEMP(TEMP_VAR, 1));

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ starc upper line = center line + 2*ATR }}}
-----------------------------------------------------------------------------*/
double yesStarcUpper(long lIsUpdateData, long lIndex, long lPeriod, long lPeriod1,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR1,
					UNKNOWN_DATA_TYPE SRC_VAR2,
					TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dATR;
	double dCenterLine;
	
	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	dATR = yesATR(lIsUpdateData, lIndex, lPeriod, HIGH_VAR, LOW_VAR, SRC_VAR1, SRC_VAR2, GET_SUB_TEMP(TEMP_VAR, 1));
	dCenterLine = yesStarcCenter(lIsUpdateData, lIndex, lPeriod1, SRC_VAR1, GET_SUB_TEMP(TEMP_VAR, 2));
	dRetValue = dCenterLine + 2 * dATR;
	
	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ starc Lower line = center line - 2*ATR }}}
-----------------------------------------------------------------------------*/
double yesStarcLower(long lIsUpdateData, long lIndex, long lPeriod, long lPeriod1,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR1,
					UNKNOWN_DATA_TYPE SRC_VAR2,
					TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dATR;
	double dCenterLine;
	
	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	dATR = yesATR(lIsUpdateData, lIndex, lPeriod, HIGH_VAR, LOW_VAR, SRC_VAR1, SRC_VAR2, GET_SUB_TEMP(TEMP_VAR, 1));
	dCenterLine = yesStarcCenter(lIsUpdateData, lIndex, lPeriod1, SRC_VAR1, GET_SUB_TEMP(TEMP_VAR, 2));
	dRetValue = dCenterLine - 2 * dATR;

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ Formula = ATAN(a) * 180 / Pi }}}
				  {{{ a = 당일의 n일 SMA - 전일의 n일 SMA}}}
-----------------------------------------------------------------------------*/
double yesFormula(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE SRC_VAR1,
					UNKNOWN_DATA_TYPE SRC_VAR2,
					TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dSMA;
	double dPreSMA;
	double dValA;
	double dPi = 3.14;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	dSMA = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, GET_SUB_TEMP(TEMP_VAR,1));
	if(dSMA == NONAVAILABLE || lIndex < lPeriod)
	{
		SET_AT(SRC_VAR2, lIndex, dSMA);
		return NONAVAILABLE;
	}
	SET_AT(SRC_VAR2, lIndex, dSMA);
	dPreSMA = GET_AT(SRC_VAR2, lIndex-1);
	dValA = dSMA - dPreSMA;
	dRetValue = atan(dValA) * 180/dPi;
	
	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ DRF = (BP + SP) / [2*(고가-저가)] * 100 }}}
				  {{{ BP = 고가 - [(고가-전일종가)/(고가-저가)] }}}
				  {{{ SP = 종가 - 저가 }}}
				  {{{ 고가 - 저가 == 0? 0.0001}}}

-----------------------------------------------------------------------------*/
double yesDRF(long lIsUpdateData, long lIndex,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE CLOSE_VAR,
					TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dHigh;
	double dLow;
	double dClose;
	double dCloseOld;
	double dBP;
	double dSP;
	double dVal;
	double dHighMinusLow;

	if(lIndex < 1)
		return NONAVAILABLE;

	dHigh = GET_AT(HIGH_VAR, lIndex);
	dLow = GET_AT(LOW_VAR, lIndex);
	dClose = GET_AT(CLOSE_VAR, lIndex);
	dCloseOld = GET_AT(CLOSE_VAR, lIndex-1);


	dHighMinusLow = ((dHigh-dLow) ? (dHigh-dLow) : 0.0001);

	dBP = dHigh - (dHigh-dCloseOld)/dHighMinusLow;
	dSP = dClose - dLow;	
	dVal = 2 * dHighMinusLow;
	dRetValue = (dBP+dSP) / dVal * 100;

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ NCO = 당일종가 / n일 전 종가 * 100 }}}
-----------------------------------------------------------------------------*/
double yesNCO(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE CLOSE_VAR,
					TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dClose;
	double dCloseOld;

	if(lIndex < lPeriod)
		return NONAVAILABLE;

	dClose = GET_AT(CLOSE_VAR, lIndex);
	dCloseOld = GET_AT(CLOSE_VAR, lIndex-lPeriod);

	dRetValue = dClose/dCloseOld * 100;

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ CMF = AD / n일 거래량 합 }}}
				  {{{ 가중거래량 = [(종가-저가) - (고가-종가)] / (고가-저가) * 거래량 }}}
				  {{{ AD = 가중거래량의 n일 합 }}}
-----------------------------------------------------------------------------*/
double yesCMF(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE CLOSE_VAR,
					UNKNOWN_DATA_TYPE VOLUME_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR,
					TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dHigh;
	double dLow;
	double dClose;
	double dVolume;
	double dVal;
	double dVal2;
	double dWeightVolume;
	double dAddVolume;
	double dAD;

	dHigh = GET_AT(HIGH_VAR,lIndex);
	dLow = GET_AT(LOW_VAR,lIndex);
	dClose = GET_AT(CLOSE_VAR,lIndex);
	dVolume = GET_AT(VOLUME_VAR,lIndex);

	dVal = (dClose-dLow) - (dHigh-dClose);
	dVal2 = ((dHigh-dLow) ? (dHigh-dLow) : 1);
	dWeightVolume = dVal / dVal2 * dVolume;

	SET_AT(SRC_VAR, lIndex, dWeightVolume);

	dAD = yesAccumN(lIsUpdateData, lIndex, lPeriod, SRC_VAR, GET_SUB_TEMP(TEMP_VAR, 1)); 
	dAddVolume = yesAccumN(lIsUpdateData, lIndex, lPeriod , VOLUME_VAR, GET_SUB_TEMP(TEMP_VAR, 2));

	if(lIndex < lPeriod || dAD == NONAVAILABLE || dAddVolume == NONAVAILABLE)
		return NONAVAILABLE;

	dRetValue = dAD / dAddVolume;

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ Elder Ray Bear = 저가 - n일 종가 지수이동평균 }}}
-----------------------------------------------------------------------------*/
double yesElderRayBear(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR,
					UNKNOWN_DATA_TYPE OPT_VAR1,					
					TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dLow;
	double dEMA;

	dLow = GET_AT(LOW_VAR, lIndex);
	dEMA = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));

	if(lIndex < lPeriod || dEMA == NONAVAILABLE)
		return NONAVAILABLE;

	dRetValue = dLow - dEMA;

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ Elder Ray Bull = 당일고가 - n일 종가 지수이동평균 }}}
-----------------------------------------------------------------------------*/
double yesElderRayBull(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR,
					UNKNOWN_DATA_TYPE OPT_VAR1,					
					TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dHigh;
	double dEMA;

	dHigh = GET_AT(HIGH_VAR, lIndex);
	dEMA = yesEMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));

	if(lIndex < lPeriod || dEMA == NONAVAILABLE)
		return NONAVAILABLE;

	dRetValue = dHigh - dEMA;

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.20
 Modifier		: 
 Comments		: {{{ 심리도 = (n일 중 상승일 수 / n) * 100 }}}
-----------------------------------------------------------------------------*/
double yesPsyhologicalLine(long lIsUpdateData, long lIndex, long lPeriod,
							UNKNOWN_DATA_TYPE SRC_VAR1,
							UNKNOWN_DATA_TYPE SRC_VAR2,
							TEMP_DATA_TYPE TEMP_VAR)
{
	double dRetValue = NONAVAILABLE;
	double dClose;
	double dCloseOld;
	double dRiseNum;

	if(lIndex <1 || lPeriod <1)
		return NONAVAILABLE;

	dClose = GET_AT(SRC_VAR1, lIndex);
	dCloseOld = GET_AT(SRC_VAR1, lIndex-1);
	if(dCloseOld < dClose)
		SET_AT(SRC_VAR2, lIndex, 1);
	else
		SET_AT(SRC_VAR2, lIndex, 0);

	dRiseNum = yesAccumN(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, GET_SUB_TEMP(TEMP_VAR, 1));
	
	if(lIndex < lPeriod)
		return NONAVAILABLE;

	dRetValue = (dRiseNum / lPeriod) * 100;

	return dRetValue;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.20
 Modifier		: 
 Comments		: {{{ fast %K = (당일종가-n일간의최저가) / (n일간의최고가-n일간의최저가) * 100 }}}
-----------------------------------------------------------------------------*/
double yesStoFastK(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double dClose;
	double ldLowest;
	double ldHighest;

	if (lPeriod < 1 /*|| lIndex + 1 < lPeriod*/ )
	{
		return NONAVAILABLE;
	}

	dClose = GET_AT(SRC_VAR, lIndex);
	ldLowest = yesLowest(lIsUpdateData, lIndex, lPeriod, LOW_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldHighest = yesHighest(lIsUpdateData, lIndex, lPeriod, HIGH_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	SET_AT(OPT_VAR1, lIndex, ldLowest);
	SET_AT(OPT_VAR2, lIndex, ldHighest);

	/* gm0604 modified..2005.9.12*/
	if(ldLowest==NONAVAILABLE||ldHighest==NONAVAILABLE)
	{
		ldRetVal = NONAVAILABLE;
	}
	else if (ldHighest - ldLowest)
	{
		ldRetVal = (dClose - ldLowest) / (ldHighest - ldLowest)*100.;
	}
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.20
 Modifier		: 
 Comments		: {{{ fast %D = %K를 n일 지수이동평균 }}}
-----------------------------------------------------------------------------*/
double yesStoFastD(long lIsUpdateData, long lIndex, long lPeriod1, 
		long lPeriod2,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double dClose;
	double ldLowest;
	double ldHighest;
	double ldVal3;

	if (lPeriod1 < 1 || lPeriod2 < 1)
	{
		return NONAVAILABLE;
	}

	dClose = GET_AT(SRC_VAR, lIndex);
	ldLowest = yesLowest(lIsUpdateData, lIndex, lPeriod1, LOW_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 1));
	ldHighest = yesHighest(lIsUpdateData, lIndex, lPeriod1, HIGH_VAR, 
				GET_SUB_TEMP(TEMP_VAR, 2));

	SET_AT(OPT_VAR1, lIndex, ldLowest);
	SET_AT(OPT_VAR2, lIndex, ldHighest);

	/* gm0604 modified..2005.9.12*/
	if(ldHighest==NONAVAILABLE||ldLowest==NONAVAILABLE)
	{
		ldVal3 = NONAVAILABLE;
	}
	else if (ldHighest - ldLowest)
	{
		ldVal3 = (dClose - ldLowest) / (ldHighest - ldLowest)*100.;
	}
	else
	{
		ldVal3 = 0.;
	}

	SET_AT(OPT_VAR3, lIndex, ldVal3);

	ldRetVal = yesEMA(lIsUpdateData, lIndex, lPeriod2, OPT_VAR3, OPT_VAR4,
				GET_SUB_TEMP(TEMP_VAR, 3));

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.20
 Modifier		: 
 Comments		: {{{ TD = DM의 n일 합 }}}
				  {{{ DM = 종가 - (n-1)일전 종가 }}}
-----------------------------------------------------------------------------*/
double yesTD(long lIsUpdateData, long lIndex, long lPeriod, 
			UNKNOWN_DATA_TYPE SRC_VAR1,
			UNKNOWN_DATA_TYPE SRC_VAR2,
			TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double dClose;
	double dCloseOld;
	double dDM;

	if(lIndex < lPeriod || lPeriod < 1)
		return NONAVAILABLE;

	dClose = GET_AT(SRC_VAR1, lIndex);
	dCloseOld = GET_AT(SRC_VAR1, lIndex-lPeriod+1);
	dDM = dClose - dCloseOld;
	SET_AT(SRC_VAR2, lIndex, dDM);
	
	ldRetVal = yesAccumN(lIsUpdateData, lIndex, lPeriod, SRC_VAR2, GET_SUB_TEMP(TEMP_VAR, 1));

	return ldRetVal;	
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.20
 Modifier		: 
 Comments		: {{{ TDI = TD의 절대값 + AbsDM의 n일 합 - AbsDM의 2n일 합 }}}
-----------------------------------------------------------------------------*/
double yesTDI(long lIsUpdateData, long lIndex, long lPeriod, 
			UNKNOWN_DATA_TYPE SRC_VAR,
			UNKNOWN_DATA_TYPE OPT_VAR1,
			UNKNOWN_DATA_TYPE OPT_VAR2,
			TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double dClose;
	double dCloseOld;
	double dDM;
	double dAbsDM;
	double dTD;
	double dSumAM;
	double dSumAM2;

	if(lPeriod < 1 || lIndex < lPeriod) /* || lIndex < 2*lPeriod-1  */
		return NONAVAILABLE;

	dClose = GET_AT(SRC_VAR, lIndex);
	dCloseOld = GET_AT(SRC_VAR, lIndex-lPeriod+1);
	dDM = dClose - dCloseOld;
	dAbsDM = fabs(dDM);
	SET_AT(OPT_VAR1, lIndex, dDM);
	SET_AT(OPT_VAR2, lIndex, dAbsDM);
	
	dTD = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));
	dSumAM = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 2));
	dSumAM2 = yesAccumN(lIsUpdateData, lIndex, 2*lPeriod, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 3));

	ldRetVal = fabs(dTD) + dSumAM - dSumAM2;	

	return ldRetVal;
}

double yesMarketPosition(long lIsUpdateData, long lIndex, long lPeriod, 
			UNKNOWN_DATA_TYPE SRC_VAR,
			UNKNOWN_DATA_TYPE OPT_VAR1,
			UNKNOWN_DATA_TYPE OPT_VAR2,
			UNKNOWN_DATA_TYPE OPT_VAR3,
			TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double *ldpTemp;
	double dTD;
	double dTDI;
	double *ldpPosition;

	ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return 0.;
    ldpPosition = &ldpTemp[0];

	if(lIndex < lPeriod || lPeriod < 1)
	{
		*ldpPosition = 0;
		return NONAVAILABLE;
	}
	
	dTD = yesTD(lIsUpdateData, lIndex, lPeriod, SRC_VAR, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 2));
	dTDI = yesTDI(lIsUpdateData, lIndex-1, lPeriod, SRC_VAR, OPT_VAR2, OPT_VAR3, GET_SUB_TEMP(TEMP_VAR, 3));

	if(dTD == NONAVAILABLE || dTDI == NONAVAILABLE || lIndex < 2*lPeriod-1)
		return NONAVAILABLE;

	if(dTDI > 0)
	{
		if(dTD > 0)
		{
			*ldpPosition = 1;
		}
		else
		{
			*ldpPosition = -1;
		}
	}
	ldRetVal = *ldpPosition;
		
	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.21
 Modifier		: 
 Comments		: {{{ Band%B = (종가 - 볼린저 하한선) / (볼린저 상한선 - 볼린저 하한선) * 100 }}}
-----------------------------------------------------------------------------*/
double yesBandB(long lIsUpdateData, long lIndex, 
				long lPeriod, double dStdDev,
				UNKNOWN_DATA_TYPE CLOSE_VAR,
				UNKNOWN_DATA_TYPE SRC_VAR,
				TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double dClose;
	double ldBBUpper;
	double ldBBLower;
	
	if (lPeriod < 1) /* || lIndex + 1 < lPeriod) */
	{
		return NONAVAILABLE;
	}

	dClose = GET_AT(CLOSE_VAR, lIndex);

	ldBBUpper = yesBollBandUp(lIsUpdateData, lIndex, lPeriod, dStdDev, SRC_VAR, GET_SUB_TEMP(TEMP_VAR, 1));
	ldBBLower = yesBollBandDown(lIsUpdateData, lIndex, lPeriod, dStdDev, SRC_VAR, GET_SUB_TEMP(TEMP_VAR, 2));

	ldRetVal = (dClose - ldBBLower) / (ldBBUpper - ldBBLower) * 100;

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.21
 Modifier		: 
 Comments		: {{{ Band width = (upper band - lower band) / middle band }}}
-----------------------------------------------------------------------------*/
double yesBandWidth(long lIsUpdateData, long lIndex, 
				long lPeriod, double dStdDev,
				UNKNOWN_DATA_TYPE SRC_VAR,
				TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldBBUpper;
	double ldBBCenter;
	double ldBBLower;
	
	if (lPeriod < 1) /* || lIndex + 1 < lPeriod) */
	{
		return NONAVAILABLE;
	}

	ldBBUpper = yesBollBandUp(lIsUpdateData, lIndex, lPeriod, dStdDev, SRC_VAR, GET_SUB_TEMP(TEMP_VAR, 1));
	ldBBCenter = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, GET_SUB_TEMP(TEMP_VAR, 2));
	ldBBLower = yesBollBandDown(lIsUpdateData, lIndex, lPeriod, dStdDev, SRC_VAR, GET_SUB_TEMP(TEMP_VAR, 3));

	ldRetVal = (ldBBUpper - ldBBLower) / ldBBCenter;

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: {{{ ADXR = ADX의 n일 이동평균 }}}
-----------------------------------------------------------------------------*/
double yesADXR(long lIsUpdateData, long lIndex, long lPeriod1, long lPeriod2,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR, UNKNOWN_DATA_TYPE CLOSE_VAR, 
		UNKNOWN_DATA_TYPE ADX_OPT1,
		UNKNOWN_DATA_TYPE ADX_OPT2,
		UNKNOWN_DATA_TYPE ADX_OPT3,
		UNKNOWN_DATA_TYPE ADX_OPT4,
		UNKNOWN_DATA_TYPE ADX_OPT5,
		UNKNOWN_DATA_TYPE ADX_OPT6,
		UNKNOWN_DATA_TYPE ADX_OPT7,
		UNKNOWN_DATA_TYPE ADX_OPT8,
		UNKNOWN_DATA_TYPE ADX_OPT9,
		UNKNOWN_DATA_TYPE ADX_OPT10,
		UNKNOWN_DATA_TYPE EMA_OPT1,
		UNKNOWN_DATA_TYPE EMA_OPT2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldADX;

	if (lIndex < 1 || lPeriod1 < 1 || lPeriod2 < 1)
	{
		return NONAVAILABLE;
	}
	
	ldADX = yesADX(lIsUpdateData, lIndex, lPeriod1, HIGH_VAR, LOW_VAR, CLOSE_VAR,
		ADX_OPT1, ADX_OPT2, ADX_OPT3, ADX_OPT4, ADX_OPT5, ADX_OPT6, ADX_OPT7, ADX_OPT8, ADX_OPT9, ADX_OPT10, GET_SUB_TEMP(TEMP_VAR, 1));

	SET_AT(EMA_OPT1, lIndex, ldADX);
	
	if(ldADX == NONAVAILABLE)
		return NONAVAILABLE;

	ldRetVal = yesEMA(lIsUpdateData, lIndex, lPeriod2, EMA_OPT1, EMA_OPT2, GET_SUB_TEMP(TEMP_VAR, 2));

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: {{{ PDI = PDMn / TRn * 100 }}}
				  {{{ PDMn = PDM의 n일 이동평균 }}}
-----------------------------------------------------------------------------*/
double yesPDI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR, 
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldHighOld;
	double ldLowOld;
	double ldCloseOld;
	double dValue1;
	double dValue2;
	double dValue3;
	double ldTR;
	double ldTRn;
	double ldPDM;
	double ldPDMn;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}
	
	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldHighOld = GET_AT(HIGH_VAR, lIndex - 1);
	ldLowOld = GET_AT(LOW_VAR, lIndex - 1);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex-1);

	ldPDM = (ldHigh - ldHighOld > 0 && ldHigh - ldHighOld > ldLowOld - ldLow ?
				ldHigh - ldHighOld : 0.); 
	SET_AT(OPT_VAR1, lIndex, ldPDM);
	ldPDMn = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, OPT_VAR3, GET_SUB_TEMP(TEMP_VAR, 1));

	dValue1 = fabs(ldHigh-ldLow);
	dValue2 = fabs(ldCloseOld-ldHigh);
	dValue3 = fabs(ldCloseOld-ldLow);
	ldTR = (dValue1 > dValue3? ((dValue1 > dValue2)? dValue1 : dValue2) : dValue3);

	SET_AT(OPT_VAR2, lIndex, ldTR);
	ldTRn = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, OPT_VAR4, GET_SUB_TEMP(TEMP_VAR, 2));

	ldRetVal = ldPDMn / ldTRn * 100;

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: {{{ MDI = MDMn / TRn * 100 }}}
				  {{{ MDMn = MDM의 n일 이동평균 }}}
-----------------------------------------------------------------------------*/
double yesMDI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR, 
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldHigh;
	double ldLow;
	double ldHighOld;
	double ldLowOld;
	double ldCloseOld;
	double dValue1;
	double dValue2;
	double dValue3;
	double ldMDM;
	double ldMDMn;
	double ldTR;
	double ldTRn;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}
	
	ldHigh = GET_AT(HIGH_VAR, lIndex);
	ldLow = GET_AT(LOW_VAR, lIndex);
	ldHighOld = GET_AT(HIGH_VAR, lIndex - 1);
	ldLowOld = GET_AT(LOW_VAR, lIndex - 1);
	ldCloseOld = GET_AT(CLOSE_VAR, lIndex-1);

	ldMDM = (ldLowOld - ldLow > 0 && ldHigh - ldHighOld < ldLowOld - ldLow ?
				ldLowOld - ldLow : 0.);

	SET_AT(OPT_VAR1, lIndex, ldMDM);
	ldMDMn = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, OPT_VAR3, GET_SUB_TEMP(TEMP_VAR, 1));
	
	dValue1 = fabs(ldHigh-ldLow);
	dValue2 = fabs(ldCloseOld-ldHigh);
	dValue3 = fabs(ldCloseOld-ldLow);
	ldTR = (dValue1 > dValue3? ((dValue1 > dValue2)? dValue1 : dValue2) : dValue3);

	SET_AT(OPT_VAR2, lIndex, ldTR);
	ldTRn = yesEMA(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, OPT_VAR4,GET_SUB_TEMP(TEMP_VAR, 2));

	ldRetVal = ldMDMn / ldTRn * 100;

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: {{{ NDI = PDI - MDI }}}
-----------------------------------------------------------------------------*/
double yesNDI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE PDI_OPT1,
		UNKNOWN_DATA_TYPE PDI_OPT2,
		UNKNOWN_DATA_TYPE PDI_OPT3,
		UNKNOWN_DATA_TYPE PDI_OPT4,
		UNKNOWN_DATA_TYPE MDI_OPT1,
		UNKNOWN_DATA_TYPE MDI_OPT2,
		UNKNOWN_DATA_TYPE MDI_OPT3,
		UNKNOWN_DATA_TYPE MDI_OPT4,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldPDI;
	double ldMDI;

	if (lIndex < 1 || lPeriod < 1)
	{
		return NONAVAILABLE;
	}
	
	ldPDI = yesPDI(lIsUpdateData, lIndex, lPeriod, HIGH_VAR, LOW_VAR, CLOSE_VAR, 
		PDI_OPT1, PDI_OPT2, PDI_OPT3, PDI_OPT4, GET_SUB_TEMP(TEMP_VAR, 1));

	ldMDI = yesMDI(lIsUpdateData, lIndex, lPeriod, HIGH_VAR, LOW_VAR, CLOSE_VAR, 
		MDI_OPT1, MDI_OPT2, MDI_OPT3, MDI_OPT4, GET_SUB_TEMP(TEMP_VAR, 2));

	ldRetVal = ldPDI - ldMDI;

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: 
-----------------------------------------------------------------------------*/
double yesQstic(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE OPEN_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR1,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double *ldpTemp;
	double *ldpSum;
	double lValue;

	ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;
	ldpSum = &ldpTemp[0];

	lValue = GET_AT(CLOSE_VAR, lIndex) - GET_AT(OPEN_VAR, lIndex);
	SET_AT(SRC_VAR1, lIndex, lValue);

	ldRetVal = yesSMA(lIsUpdateData, lIndex,lPeriod, SRC_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: 
-----------------------------------------------------------------------------*/
double yesDPO(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double dValue1;
	double dValue2;
	double dClose;
	double dValue3;
	double ldSMA;

	if(lPeriod < 1)
		return NONAVAILABLE;
	
	dValue1 = ((double)lPeriod/2.) + 0.5 +1.;
	dValue2 = (double)lIndex - dValue1;
	dClose = GET_AT(SRC_VAR, lIndex);
	ldSMA = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(OPT_VAR, lIndex,ldSMA);
	/* 2005.12.22 gm0604 modified Start*/
	if(dValue2<0) 
		return NONAVAILABLE;
	/* 2005.12.22 gm0604 modified End*/
	dValue3 = GET_AT(OPT_VAR, ( long)dValue2);
	
	if(ldSMA == NONAVAILABLE || dValue3 == NONAVAILABLE )
		return NONAVAILABLE;

	ldRetVal = dClose - dValue3;

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.25
 Modifier		: 
 Comments		: 
-----------------------------------------------------------------------------*/
double yesStandardError(long lIsUpdateData, long lIndex, long lPeriod,
						UNKNOWN_DATA_TYPE SRC_VAR,
						UNKNOWN_DATA_TYPE OPT_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR2,
						UNKNOWN_DATA_TYPE OPT_VAR3,
						UNKNOWN_DATA_TYPE OPT_VAR4,
						UNKNOWN_DATA_TYPE OPT_VAR5,
						TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double *ldpTemp;
	double *ldpX;
	double dClose;
	double ldSumX;
	double ldSumY;
	double ldSumXY;
	double ldSumX2;
	double ldSumY2;
	double dValue1;
	double dValue2;
	double dValue3;
	double dValue4;

	ldpTemp = GET_TEMP_DBL(TEMP_VAR);
    if (ldpTemp == NULL) return ldRetVal;
	ldpX = &ldpTemp[0];

	if(lPeriod < 1 || lPeriod == 2)
		return NONAVAILABLE;

	dClose = GET_AT(SRC_VAR, lIndex);

	*ldpX = *ldpX +1;
	ldSumX = *ldpX;
	ldSumY = dClose;
	ldSumXY = *ldpX * dClose;
	ldSumX2 = *ldpX * *ldpX;
	ldSumY2 = dClose * dClose;

	SET_AT(OPT_VAR1, lIndex, ldSumX);
	SET_AT(OPT_VAR2, lIndex, ldSumY);
	SET_AT(OPT_VAR3, lIndex, ldSumXY);
	SET_AT(OPT_VAR4, lIndex, ldSumX2);
	SET_AT(OPT_VAR5, lIndex, ldSumY2);

	ldSumX = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 2));
	ldSumY = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 3));
	ldSumXY = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR3, GET_SUB_TEMP(TEMP_VAR, 4));
	ldSumX2 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR4, GET_SUB_TEMP(TEMP_VAR, 5));
	ldSumY2 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR5, GET_SUB_TEMP(TEMP_VAR, 6));

	if(lIndex < lPeriod)
		return NONAVAILABLE;

	dValue1 = 1/(lPeriod * (lPeriod - 2.));
	dValue2 = lPeriod * ldSumY2 - (ldSumY * ldSumY);
	dValue3 = lPeriod * ldSumXY - (ldSumX * ldSumY);
	dValue4 = lPeriod * ldSumX2 - (ldSumX * ldSumX);

	if(dValue4 == 0)
		return NONAVAILABLE;

	ldRetVal = sqrt(dValue1 * (dValue2 - (dValue3*dValue3)/dValue4));

	return ldRetVal;
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.11
 Modifier		: 
 Comments		: {{{ Standard Error Bands Center = LRI}}}
-----------------------------------------------------------------------------*/
double yesSterrBnCenter(long lIsUpdateData, long lIndex, long lPeriod,
						UNKNOWN_DATA_TYPE SRC_VAR1,	/* close */
						UNKNOWN_DATA_TYPE OPT_VAR1, 
						UNKNOWN_DATA_TYPE OPT_VAR2, /* x */
						UNKNOWN_DATA_TYPE OPT_VAR3, /* xy */
						UNKNOWN_DATA_TYPE OPT_VAR4, /* x² */
						TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;

	double ldVal1;
	double ldVal2;	/* ∑x */
	double ldVal3;	/* ∑y */
	double ldVal4;
	double ldLRS;

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	/**		B = (N*∑xy-∑x*∑y) / (N*∑x²-(∑x)²)	**/
	ldLRS = yesLRS(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR1, OPT_VAR2, 
		OPT_VAR3, OPT_VAR4, GET_SUB_TEMP(TEMP_VAR, 1));

	ldVal1 = GET_AT(OPT_VAR2,lIndex);

	ldVal2 = yesAccumN(lIsUpdateData, lIndex, lPeriod, OPT_VAR2, GET_SUB_TEMP(TEMP_VAR, 3));
	ldVal3 = yesAccumN(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, GET_SUB_TEMP(TEMP_VAR, 4));

	/*	 A=(∑y-B*∑x)/n	*/
	ldVal4 = (ldVal3 - ldLRS*ldVal2) / lPeriod;

	/*	Y = A + Bx	 */
	ldRetVal = ldVal4 + ldLRS * ldVal1;

	return ldRetVal;	

}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.11
 Modifier		: 
 Comments		: {{{ Standard Error Bands Upper = center line + 2*STERR}}}
-----------------------------------------------------------------------------*/
double yesSterrBnUpper(long lIsUpdateData, long lIndex, long lPeriod, double dStdDev,
						UNKNOWN_DATA_TYPE SRC_VAR1,	/* close */
						UNKNOWN_DATA_TYPE OPT_VAR1, 
						UNKNOWN_DATA_TYPE OPT_VAR2, 
						UNKNOWN_DATA_TYPE OPT_VAR3,
						UNKNOWN_DATA_TYPE OPT_VAR4,
						UNKNOWN_DATA_TYPE OPT_VAR5,
						UNKNOWN_DATA_TYPE OPT_VAR6,
						UNKNOWN_DATA_TYPE OPT_VAR7,
						UNKNOWN_DATA_TYPE OPT_VAR8,
						UNKNOWN_DATA_TYPE OPT_VAR9,
						TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	
	double ldVal1;
	double ldVal2;	

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldVal1 = yesSterrBnCenter(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR1, OPT_VAR2, OPT_VAR3,
		OPT_VAR4, GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal2 = yesStandardError(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR5, OPT_VAR6, OPT_VAR7,
		OPT_VAR8, OPT_VAR9, GET_SUB_TEMP(TEMP_VAR, 2));


	ldRetVal = ldVal1 + dStdDev * ldVal2;

	return ldRetVal;	

}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.11
 Modifier		: 
 Comments		: {{{ Standard Error Bands Lower = center line - 2*STERR}}}
-----------------------------------------------------------------------------*/
double yesSterrBnLower(long lIsUpdateData, long lIndex, long lPeriod, double dStdDev,
						UNKNOWN_DATA_TYPE SRC_VAR1,	/* close */
						UNKNOWN_DATA_TYPE OPT_VAR1, 
						UNKNOWN_DATA_TYPE OPT_VAR2, 
						UNKNOWN_DATA_TYPE OPT_VAR3,
						UNKNOWN_DATA_TYPE OPT_VAR4,
						UNKNOWN_DATA_TYPE OPT_VAR5,
						UNKNOWN_DATA_TYPE OPT_VAR6,
						UNKNOWN_DATA_TYPE OPT_VAR7,
						UNKNOWN_DATA_TYPE OPT_VAR8,
						UNKNOWN_DATA_TYPE OPT_VAR9,
						TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	
	double ldVal1;
	double ldVal2;	

	if (lPeriod < 1)
	{
		return NONAVAILABLE;
	}

	ldVal1 = yesSterrBnCenter(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR1, OPT_VAR2, OPT_VAR3,
		OPT_VAR4, GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal2 = yesStandardError(lIsUpdateData, lIndex, lPeriod, SRC_VAR1, OPT_VAR5, OPT_VAR6, OPT_VAR7,
		OPT_VAR8, OPT_VAR9, GET_SUB_TEMP(TEMP_VAR, 2));


	ldRetVal = ldVal1 - dStdDev * ldVal2;

	return ldRetVal;	

}

double yesMAO(long lIsUpdateData, long lIndex, long lShortPeriod, 
						long lLongPeriod,
						UNKNOWN_DATA_TYPE SRC_VAR,
						UNKNOWN_DATA_TYPE OPT_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR2,
						TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;

	if (lShortPeriod < 1 || lLongPeriod < 1)
	{
		return NONAVAILABLE;
	}

	double ldShtMA = yesSMA(lIsUpdateData, lIndex, lShortPeriod, SRC_VAR,
		GET_SUB_TEMP(TEMP_VAR, 1));
	double ldLngMA = yesSMA(lIsUpdateData, lIndex, lLongPeriod, SRC_VAR, 
		GET_SUB_TEMP(TEMP_VAR, 2));

	if(ldShtMA == NONAVAILABLE || ldLngMA == NONAVAILABLE)
		return ldRetVal;

	ldRetVal = ldShtMA - ldLngMA;

	return ldRetVal;
}

double yesSIGMA(long lIsUpdateData, long lIndex, long lPeriod, 
					   UNKNOWN_DATA_TYPE SRC_VAR, 
					   TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double dSRCVal;
	double dSMAVal;
	double dSTDVal;

	if (lPeriod < 1) /* || lIndex + 1 < lPeriod) */
	{
		return NONAVAILABLE;
	}

	dSRCVal = GET_AT(SRC_VAR, lIndex);

	dSMAVal = yesSMA(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
		GET_SUB_TEMP(TEMP_VAR, 1));

	dSTDVal = yesStd(lIsUpdateData, lIndex, lPeriod, SRC_VAR, 
		GET_SUB_TEMP(TEMP_VAR, 2));

	if(dSMAVal == NONAVAILABLE || dSTDVal == NONAVAILABLE)
		return ldRetVal;

	ldRetVal = (dSRCVal - dSMAVal) / dSTDVal;

	return ldRetVal;
}

double yesOSCP(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR)
{
	double ldRetVal = NONAVAILABLE;
	double ldVal1;
	double ldVal2;

	if (lShortPeriod < 1 || lLongPeriod < 1)
	{
		return NONAVAILABLE;
	}

	//ldVal1 = yesSMA(lIsUpdateData, lIndex, lShortPeriod, CLOSE_VAR, 
	//			GET_SUB_TEMP(TEMP_VAR, 1));
	//ldVal2 = yesSMA(lIsUpdateData, lIndex, lLongPeriod, CLOSE_VAR, 
	//			GET_SUB_TEMP(TEMP_VAR, 2));

	// 단순이평에서 지수이평으로 수정됨
	ldVal1 = yesEMA(lIsUpdateData, lIndex, lShortPeriod, CLOSE_VAR, OPT_VAR1,
		GET_SUB_TEMP(TEMP_VAR, 1));
	ldVal2 = yesEMA(lIsUpdateData, lIndex, lLongPeriod, CLOSE_VAR, OPT_VAR2,
		GET_SUB_TEMP(TEMP_VAR, 2));	
	//if (lIndex + 1 < lShortPeriod || lIndex + 1 < lLongPeriod)
	//{
	//	return NONAVAILABLE;
	//}

	if (ldVal1)
	{
		ldRetVal = (ldVal1 - ldVal2) / ldVal1 * 100.;
	}	
	else
	{
		ldRetVal = 0.;
	}

	return ldRetVal;
}

double yesSROC(long lIsUpdateData, long lIndex, 
			   long lPeriod1, long lPeriod2,
			   UNKNOWN_DATA_TYPE SRC_VAR1,
			   UNKNOWN_DATA_TYPE OPT_VAR1,
			   UNKNOWN_DATA_TYPE OPT_VAR2,
			   TEMP_DATA_TYPE TEMP_VAR)
{
    double dRetValue = NONAVAILABLE;
	double dDayEMA;
	double dPreEMA;

	if (lPeriod1 < 1)
	{
		return NONAVAILABLE;
	}

	dDayEMA = yesEMA(lIsUpdateData, lIndex, lPeriod1, SRC_VAR1, OPT_VAR1, GET_SUB_TEMP(TEMP_VAR, 1));
	SET_AT(OPT_VAR2, lIndex, dDayEMA);

	if (lIndex < lPeriod2)
	{
		return NONAVAILABLE;
	}

	dPreEMA = GET_AT(OPT_VAR2, lIndex-lPeriod2);
	
	dRetValue = (dDayEMA / dPreEMA) * 100.;
	return dRetValue;
}
