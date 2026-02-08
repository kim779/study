// CandlePattern.cpp : implementation file
//
// #######################################################################################
// Last Reference Document of a Candle Pattern Logic
//		- Apply Company : Daewoo Securities
//		- Date			: 2003. 07. 04
//		- Method		: Relative Evaluation
//---------------------------------------------------------------------------------------
// Author	: Jeong SeoJeong
// Add Date : 2005/04/30 ~ 2005/05/19
// #######################################################################################


#include "stdafx.h"
#include "CandlePattern.h"

#include "PatternDBItems.h"		// Pattern DB Items
#include "PatternDefine.h"
#include <afxtempl.h>			// for CArray
#include <math.h>				// for fabs

// 2008.02.20 by LYH >>
#include "resource.h"
#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.02.20 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCandlePattern

CCandlePattern::CCandlePattern()
{
// 2008.02.20 by LYH >>
	m_hOcxWnd = NULL;
// 2008.02.20 by LYH <<
}

CCandlePattern::~CCandlePattern()
{
}


// ***************************************************************************************
//	Get Basic Candle Data 
// ======================================================================================>
// *************************************
// Get Height of a Candle
// *************************************
double CCandlePattern::GetCandleHeight(			LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	return ( pstCandleData->dHigh[nDataPos] - pstCandleData->dLow[nDataPos]);
}

// *************************************
// Get Average Height of a Candle
// *************************************
double CCandlePattern::GetCandleAvgHeight(		LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	int nMinDataCnt;
	double dAverage = 0, dSum = 0;

	nMinDataCnt = min(NEED_PERIOD_10, nDataPos+1);

	for ( int i = 0; i < nMinDataCnt; i++)
	{
		dSum += GetCandleHeight( pstCandleData, nDataPos-i);
	}

	dAverage = dSum / (double)nMinDataCnt;

	return dAverage;
}

// *************************************
// Get Height of a Candle Body 
// *************************************
double CCandlePattern::GetCandleBodyHeight(		LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	return fabs( pstCandleData->dOpen[nDataPos] - pstCandleData->dClose[nDataPos]);
}

// *************************************
// Get High Value of a Candle Body
// *************************************
double CCandlePattern::GetCandleBodyHighValue(	LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	return max( pstCandleData->dOpen[nDataPos], pstCandleData->dClose[nDataPos]);
}

// *************************************
// Get Low Value of a Candle Body
// *************************************
double CCandlePattern::GetCandleBodyLowValue(	LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	return min( pstCandleData->dOpen[nDataPos], pstCandleData->dClose[nDataPos]);
}	

// *************************************
// Get Middle Value of a Candle Body
// *************************************
double CCandlePattern::GetCandleBodyMidValue(	LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	return ( ( pstCandleData->dOpen[nDataPos] + pstCandleData->dClose[nDataPos]) / 2);
}

// *************************************
// Get Shadow-Up of a Candle
// *************************************
double CCandlePattern::GetCandleShadowUp(		LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	return ( pstCandleData->dHigh[nDataPos] - GetCandleBodyHighValue( pstCandleData, nDataPos));
}

// *************************************
// Get Shadow-Down of a Candle
// *************************************
double CCandlePattern::GetCandleShadowDown(		LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	return ( GetCandleBodyLowValue( pstCandleData, nDataPos) - pstCandleData->dLow[nDataPos]);
}

// *************************************
// Check White Candle
// *************************************
BOOL CCandlePattern::IsCandleWhite(				LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	if ( pstCandleData->dClose[nDataPos] > pstCandleData->dOpen[nDataPos])
		return TRUE;
	else
		return FALSE;
}

// *************************************
// Check Black Candle
// *************************************
BOOL CCandlePattern::IsCandleBlack(				LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	if ( pstCandleData->dClose[nDataPos] < pstCandleData->dOpen[nDataPos])
		return TRUE;
	else
		return FALSE;
}

// *************************************
// Check Long Candle
// *************************************
BOOL CCandlePattern::IsCandleLong(				LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	int nMinDataCnt;
	double dAverage = 0, dSum = 0;

	nMinDataCnt = min(NEED_PERIOD_10, nDataPos+1);

	for ( int i = 0; i < nMinDataCnt; i++)
	{
		dSum += GetCandleBodyHeight( pstCandleData, nDataPos-i);
	}

	dAverage = dSum / (double)nMinDataCnt;

	if ( GetCandleBodyHeight( pstCandleData, nDataPos) > dAverage*2 )
		return TRUE;
	else 
		return FALSE;
}

// *************************************
// Check Short Candle
// *************************************
BOOL CCandlePattern::IsCandleShort(				LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	int nMinDataCnt;
	double dAverage = 0, dSum = 0;

	nMinDataCnt = min(NEED_PERIOD_10, nDataPos+1);

	for ( int i = 0; i < nMinDataCnt; i++)
	{
		dSum += GetCandleBodyHeight( pstCandleData, nDataPos-i);
	}

	dAverage = dSum / (double)nMinDataCnt;

	if ( GetCandleBodyHeight( pstCandleData, nDataPos) < dAverage/2 )
		return TRUE;
	else 
		return FALSE;
}

// *************************************
// Check Doji Candle
// *************************************
BOOL CCandlePattern::IsCandleDoji(				LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	if ( pstCandleData->dOpen[nDataPos] == pstCandleData->dClose[nDataPos])
		return TRUE;
	else
		return FALSE;
}

// *************************************
// Check Marubozu Candle
// *************************************
BOOL CCandlePattern::IsCandleMarubozu(			LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	if ( GetCandleShadowUp( pstCandleData, nDataPos) == 0 && GetCandleShadowDown( pstCandleData, nDataPos) == 0)
		return TRUE;
	else
		return FALSE;
}

// *************************************
// Check Trend-Up Candle
// *************************************
BOOL CCandlePattern::IsCandleTrendUp(			LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	int nMinDataCnt;
	double dAverage = 0, dSum = 0;

	if ( nDataPos+1 < NEED_PERIOD_20)
		return FALSE;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = 0; i < nMinDataCnt; i++)
	{
		dSum += pstCandleData->dClose[nDataPos-i];
	}

	dAverage = dSum / (double)nMinDataCnt;

	if ( pstCandleData->dClose[nDataPos] >= dAverage )
		return TRUE;
	else 
		return FALSE;
}

// *************************************
// Check Trend-Down Candle
// *************************************
BOOL CCandlePattern::IsCandleTrendDown(			LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	int nMinDataCnt;
	double dAverage = 0, dSum = 0;

	if ( nDataPos+1 < NEED_PERIOD_20)
		return FALSE;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = 0; i < nMinDataCnt; i++)
	{
		dSum += pstCandleData->dClose[nDataPos-i];
	}

	dAverage = dSum / (double)nMinDataCnt;

	if ( pstCandleData->dClose[nDataPos] < dAverage )
		return TRUE;
	else 
		return FALSE;
}

// *************************************
// Check Gap-Up Candle
// *************************************
BOOL CCandlePattern::IsCandleGapUp(				LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	if ( nDataPos-1 < 0)
		return FALSE;

	if ( pstCandleData->dOpen[nDataPos] > GetCandleBodyHighValue( pstCandleData, nDataPos-1))
		return TRUE;
	else
		return FALSE;
}

// *************************************
// Check Gap-Down Candle
// *************************************
BOOL CCandlePattern::IsCandleGapDown(			LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	if ( nDataPos-1 < 0)
		return FALSE;

	if ( pstCandleData->dOpen[nDataPos] < GetCandleBodyLowValue( pstCandleData, nDataPos-1))
		return TRUE;
	else
		return FALSE;
}

// *************************************
// Check Harami Candle
// *************************************
BOOL CCandlePattern::IsCandleHarami(			LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	if ( nDataPos-1 < 0)
		return FALSE;

	if ( GetCandleBodyHighValue( pstCandleData, nDataPos) < GetCandleBodyHighValue(	pstCandleData, nDataPos-1) &&
		 GetCandleBodyLowValue(	pstCandleData, nDataPos)  > GetCandleBodyLowValue(	pstCandleData, nDataPos-1) )
		return TRUE;
	else
		return FALSE;
}

// *************************************
// Check Engulfing Candle
// *************************************
BOOL CCandlePattern::IsCandleEngulfing(			LPSTCANDLE_DATA pstCandleData, int nDataPos)
{
	if ( nDataPos-1 < 0)
		return FALSE;

	if ( GetCandleBodyHighValue( pstCandleData, nDataPos) > GetCandleBodyHighValue(	pstCandleData, nDataPos-1) &&
		 GetCandleBodyLowValue(	pstCandleData, nDataPos)  < GetCandleBodyLowValue(	pstCandleData, nDataPos-1) )
		return TRUE;
	else
		return FALSE;	
}
// <======================================================================================




// ***************************************************************************************
//	Find Candle Pattern
// =====================================================================================>>
// ******************************************************************
// ## 01. Hammer							(망치형)  
// ******************************************************************													
int CCandlePattern::CDFind_Hammer(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i)												&&
			GetCandleShadowUp(	pstCandleData, i) < GetCandleHeight(	pstCandleData, i)*0.1	&&
			GetCandleShadowDown(pstCandleData, i) > GetCandleBodyHeight(pstCandleData, i)*2		&&
			GetCandleHeight(	pstCandleData, i) > GetCandleAvgHeight(	pstCandleData, i)		&&
			!(IsCandleDoji(		pstCandleData, i))												)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_HAMMER, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 02. Hanging Man						(교수형)
// ******************************************************************
int CCandlePattern::CDFind_HangingMan(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i)												&&
			GetCandleShadowUp(	pstCandleData, i) < GetCandleHeight(	pstCandleData, i)*0.1	&&
			GetCandleShadowDown(pstCandleData, i) > GetCandleBodyHeight(pstCandleData, i)*2		&&
			GetCandleHeight(	pstCandleData, i) > GetCandleAvgHeight(	pstCandleData, i)		&&
			!(IsCandleDoji(		pstCandleData, i))												)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_HANGINGMAN, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 03. Inverted Hammer					(역망치형)         			                            		
// ******************************************************************													
int CCandlePattern::CDFind_InvertedHammer(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i)												&&
			GetCandleShadowDown(pstCandleData, i) < GetCandleHeight(	pstCandleData, i)*0.1	&&
			GetCandleShadowUp(	pstCandleData, i) > GetCandleBodyHeight(pstCandleData, i)*2		&&
			GetCandleHeight(	pstCandleData, i) > GetCandleAvgHeight(	pstCandleData, i)		&&
			!(IsCandleDoji(		pstCandleData, i))												)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_INVERTEDHAMMER, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 04. Shooting Star						(유성형)           			                            		
// ******************************************************************
int CCandlePattern::CDFind_ShootingStar(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i)												&&
			GetCandleShadowDown(pstCandleData, i) < GetCandleHeight(	pstCandleData, i)*0.1	&&
			GetCandleShadowUp(	pstCandleData, i) > GetCandleBodyHeight(pstCandleData, i)*2		&&
			GetCandleHeight(	pstCandleData, i) > GetCandleAvgHeight(	pstCandleData, i)		&&
			!(IsCandleDoji(		pstCandleData, i))												)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_SHOOTINGSTAR, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 05. Bullish Belt Hold Line			(상승샅바형)       			
// ******************************************************************													
int CCandlePattern::CDFind_Bullish_BeltHoldLine(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i)											&&
			IsCandleLong(		pstCandleData, i)											&&
			IsCandleWhite(		pstCandleData, i)											&&
			pstCandleData->dOpen[i]	== pstCandleData->dLow[i]								&&
			GetCandleShadowUp(	pstCandleData, i) > 0										&&
			GetCandleShadowUp(	pstCandleData, i) < GetCandleHeight( pstCandleData, i)*0.1	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_BELTHOLDLINE, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 06. Bearish Belt Hold Line			(하락샅바형)       			                                	
// ******************************************************************													
int CCandlePattern::CDFind_Bearish_BeltHoldLine(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i)											&&
			IsCandleLong(		pstCandleData, i)											&&
			IsCandleBlack(		pstCandleData, i)											&&
			pstCandleData->dOpen[i]	== pstCandleData->dHigh[i]								&&
			GetCandleShadowDown(pstCandleData, i) > 0										&&
			GetCandleShadowDown(pstCandleData, i) < GetCandleHeight( pstCandleData, i)*0.1	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_BELTHOLDLINE, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 07. Long White Body					(장대양봉)         			
// ******************************************************************													
int CCandlePattern::CDFind_Long_WhiteBody(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i)											&&
			IsCandleLong(		pstCandleData, i)											&&
			IsCandleWhite(		pstCandleData, i)											&&
			GetCandleShadowUp(	pstCandleData, i) < GetCandleHeight( pstCandleData, i)*0.1	&&
			GetCandleShadowDown(pstCandleData, i) < GetCandleHeight( pstCandleData, i)*0.1	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_LONG_WHITEBODY, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 08. Long Black Body					(장대음봉)         			                            		
// ******************************************************************													
int CCandlePattern::CDFind_Long_BlackBody(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i)											&&
			IsCandleLong(		pstCandleData, i)											&&
			IsCandleBlack(		pstCandleData, i)											&&
			GetCandleShadowUp(	pstCandleData, i) < GetCandleHeight( pstCandleData, i)*0.1	&&
			GetCandleShadowDown(pstCandleData, i) < GetCandleHeight( pstCandleData, i)*0.1	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_LONG_BLACKBODY, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 09. Bullish Harami					(상승잉태형)       			                            		
// ******************************************************************
int CCandlePattern::CDFind_Bullish_Harami(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)		&&
			IsCandleBlack(		pstCandleData, i-1)		&&
			IsCandleLong(		pstCandleData, i-1)		&&
			IsCandleWhite(		pstCandleData, i)		&&
			IsCandleHarami(		pstCandleData, i)		)
		{
		
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_HARAMI, i-1, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 10. Bearish Harami					(하락잉태형)       			                            		
// ******************************************************************
int CCandlePattern::CDFind_Bearish_Harami(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-1)		&&
			IsCandleWhite(		pstCandleData, i-1)		&&
			IsCandleLong(		pstCandleData, i-1)		&&
			IsCandleBlack(		pstCandleData, i)		&&
			IsCandleHarami(		pstCandleData, i)		)
		{
		
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_HARAMI, i-1, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 11. Bullish Engulfing					(상승장악형)       			
// ******************************************************************
int CCandlePattern::CDFind_Bullish_Engulfing(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)		&&
			IsCandleBlack(		pstCandleData, i-1)		&&
			IsCandleWhite(		pstCandleData, i)		&&
			IsCandleLong(		pstCandleData, i)		&&
			IsCandleEngulfing(	pstCandleData, i)		)
		{
		
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_ENGULFING, i-1, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 12. Bearish Engulfing					(하락장악형)       			                                	
// ******************************************************************
int CCandlePattern::CDFind_Bearish_Engulfing(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-1)		&&
			IsCandleWhite(		pstCandleData, i-1)		&&
			IsCandleBlack(		pstCandleData, i)		&&
			IsCandleLong(		pstCandleData, i)		&&
			IsCandleEngulfing(	pstCandleData, i)		)
		{
		
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_ENGULFING, i-1, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 13. Bullish Counterattack Line		(상승반격형)       			
// ******************************************************************
int CCandlePattern::CDFind_Bullish_CounterattackLine(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)																		&&
			IsCandleBlack(		pstCandleData, i-1)																		&&
			IsCandleLong(		pstCandleData, i-1)																		&&
			IsCandleWhite(		pstCandleData, i)																		&&
			fabs( pstCandleData->dClose[i] - pstCandleData->dClose[i-1]) < 	GetCandleAvgHeight(	pstCandleData, i)*0.1	)
		{
		
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_COUNTERATTACKLINE, i-1, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 14. Bearish Counterattack Line		(하락반격형)       			
// ******************************************************************
int CCandlePattern::CDFind_Bearish_CounterattackLine(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-1)																		&&
			IsCandleWhite(		pstCandleData, i-1)																		&&
			IsCandleLong(		pstCandleData, i-1)																		&&
			IsCandleBlack(		pstCandleData, i)																		&&
			fabs( pstCandleData->dClose[i] - pstCandleData->dClose[i-1]) < 	GetCandleAvgHeight(	pstCandleData, i)*0.1	)
		{
		
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_COUNTERATTACKLINE, i-1, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 15. Piercing Pattern					(관통형)
// ******************************************************************
int CCandlePattern::CDFind_PiercingPattern(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)									&&
			IsCandleBlack(		pstCandleData, i-1)									&&
			IsCandleLong(		pstCandleData, i-1)									&&
			IsCandleWhite(		pstCandleData, i)									&&
			IsCandleLong(		pstCandleData, i)									&&

			pstCandleData->dOpen[i] < pstCandleData->dLow[i-1]						&&
			pstCandleData->dClose[i] >= GetCandleBodyMidValue( pstCandleData, i-1)	&&
			pstCandleData->dClose[i] < pstCandleData->dOpen[i-1]					)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_PIERCINGPATTERN, i-1, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 16. Dark Cloud Cover					(먹구름형)         			                            		
// ******************************************************************
int CCandlePattern::CDFind_DarkCloudCover(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-1)									&&
			IsCandleWhite(		pstCandleData, i-1)									&&
			IsCandleLong(		pstCandleData, i-1)									&&
			IsCandleBlack(		pstCandleData, i)									&&
			IsCandleLong(		pstCandleData, i)									&&

			pstCandleData->dOpen[i] > pstCandleData->dHigh[i-1]						&&
			pstCandleData->dClose[i] <= GetCandleBodyMidValue( pstCandleData, i-1)	&&
			pstCandleData->dClose[i] > pstCandleData->dOpen[i-1]					)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_DARKCLOUDCOVER, i-1, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 17. Morning Star						(샛별형)           			                            			
// ******************************************************************
int CCandlePattern::CDFind_MorningStar(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)														&&
			IsCandleLong(		pstCandleData, i-2)														&&
			IsCandleBlack(		pstCandleData, i-2)														&&
			IsCandleShort(		pstCandleData, i-1)														&&
			GetCandleBodyHighValue( pstCandleData, i-1) < GetCandleBodyLowValue( pstCandleData, i-2)	&&
			IsCandleWhite(		pstCandleData, i)														&&
			pstCandleData->dClose[i] > pstCandleData->dClose[i-2]										)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_MORNINGSTAR, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 18. Evening Star						(석별형)           			                        			
// ******************************************************************
int CCandlePattern::CDFind_EveningStar(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)														&&
			IsCandleLong(		pstCandleData, i-2)														&&
			IsCandleWhite(		pstCandleData, i-2)														&&
			IsCandleShort(		pstCandleData, i-1)														&&
			GetCandleBodyLowValue( pstCandleData, i-1) > GetCandleBodyHighValue( pstCandleData, i-2)	&&
			IsCandleBlack(		pstCandleData, i)														&&
			pstCandleData->dClose[i] < pstCandleData->dClose[i-2]										)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_EVENINGSTAR, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 19. Morning Doji Star					(십자샛별형)       			                            		
// ******************************************************************
int CCandlePattern::CDFind_MorningDojiStar(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)														&&
			IsCandleLong(		pstCandleData, i-2)														&&
			IsCandleBlack(		pstCandleData, i-2)														&&
			IsCandleDoji(		pstCandleData, i-1)														&&
			GetCandleBodyHighValue( pstCandleData, i-1) < GetCandleBodyLowValue( pstCandleData, i-2)	&&
			IsCandleWhite(		pstCandleData, i)														&&
			pstCandleData->dClose[i] > pstCandleData->dClose[i-2]										)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_MORNINTDOJISTAR, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 20. Evening Doji Star					(십자석별형)       			                            		
// ******************************************************************
int CCandlePattern::CDFind_EveningDojiStar(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)														&&
			IsCandleLong(		pstCandleData, i-2)														&&
			IsCandleWhite(		pstCandleData, i-2)														&&
			IsCandleDoji(		pstCandleData, i-1)														&&
			GetCandleBodyLowValue( pstCandleData, i-1) > GetCandleBodyHighValue( pstCandleData, i-2)	&&
			IsCandleBlack(		pstCandleData, i)														&&
			pstCandleData->dClose[i] < pstCandleData->dClose[i-2]										)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_EVENINGDOJISTAR, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 21. Abandoned Baby In Downtrend		(상승기아형)       			
// ******************************************************************
int CCandlePattern::CDFind_AbandonedBabyInDowntrend(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)						&&
			IsCandleLong(		pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-2)						&&
			IsCandleDoji(		pstCandleData, i-1)						&&
			pstCandleData->dHigh[i-1] < pstCandleData->dLow[i-2]		&&
			IsCandleWhite(		pstCandleData, i)						&&
			pstCandleData->dLow[i] > pstCandleData->dHigh[i-1]			&&
			pstCandleData->dClose[i] > pstCandleData->dClose[i-2]		)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_ABANDONEDBABY_INDOWNTREND, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 22. Abandoned Baby In Uptrend			(하락기아형)       			
// ******************************************************************
int CCandlePattern::CDFind_AbandonedBabyInUptrend(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)						&&
			IsCandleLong(		pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleDoji(		pstCandleData, i-1)						&&
			pstCandleData->dLow[i-1] > pstCandleData->dHigh[i-2]		&&
			IsCandleBlack(		pstCandleData, i)						&&
			pstCandleData->dHigh[i] < pstCandleData->dLow[i-1]			&&
			pstCandleData->dClose[i] < pstCandleData->dClose[i-2]		)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_ABANDONEDBABY_INUPTREND, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 23. Three Indise Up					(상승잉태확인형)   			
// ******************************************************************
int CCandlePattern::CDFind_ThreeIndiseUp(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-2)						&&
			IsCandleLong(		pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-1)						&&
			IsCandleHarami(		pstCandleData, i-1)						&&
			IsCandleWhite(		pstCandleData, i)						&&
			pstCandleData->dClose[i] > pstCandleData->dClose[i-1]		)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_THREE_INSIDEUP, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 24. Three Indise Down					(하락잉태확인형)   			                            		
// ******************************************************************
int CCandlePattern::CDFind_ThreeIndiseDown(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleLong(		pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-1)						&&
			IsCandleHarami(		pstCandleData, i-1)						&&
			IsCandleBlack(		pstCandleData, i)						&&
			pstCandleData->dClose[i] < pstCandleData->dClose[i-1]		)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_THREE_INSIDEDOWN, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 25. Three Outside Up					(상승장악확인형)   			                            		
// ******************************************************************
int CCandlePattern::CDFind_ThreeOutsideUp(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-1)						&&
			IsCandleLong(		pstCandleData, i-1)						&&
			IsCandleEngulfing(	pstCandleData, i-1)						&&
			IsCandleWhite(		pstCandleData, i)						&&
			pstCandleData->dClose[i] > pstCandleData->dClose[i-1]		)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_THREE_OUTSIDEUP, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 26. Three Outside Down				(하락장악확인형)   			                                	
// ******************************************************************
int CCandlePattern::CDFind_ThreeOutsideDown(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-1)						&&
			IsCandleLong(		pstCandleData, i-1)						&&
			IsCandleEngulfing(	pstCandleData, i-1)						&&
			IsCandleBlack(		pstCandleData, i)						&&
			pstCandleData->dClose[i] < pstCandleData->dClose[i-1]		)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_THREE_OUTSIDEDOWN, i-2, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 27. Upside Gap Two Crows In Uptrend	(까마귀형)         			
// ******************************************************************
int CCandlePattern::CDFind_UpsideGapTwoCrowsInUptrend(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleLong(		pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-1)						&&
			pstCandleData->dClose[i-1] > pstCandleData->dClose[i-2]		&&
			IsCandleBlack(		pstCandleData, i)						&&
			IsCandleEngulfing(	pstCandleData, i)						&&
			pstCandleData->dClose[i] > pstCandleData->dClose[i-2]		)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_UPSIDEGAPTWOCROWS_INUPTREND, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 28. Three White Soldier				(적삼병)           			
// ******************************************************************
int CCandlePattern::CDFind_ThreeWhiteSoldier(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-1)						&&
			IsCandleWhite(		pstCandleData, i)						&&

			pstCandleData->dOpen[i-1] >= pstCandleData->dOpen[i-2]		&&
			pstCandleData->dOpen[i-1] <= pstCandleData->dClose[i-2]		&&

			pstCandleData->dOpen[i] >= pstCandleData->dOpen[i-1]		&&
			pstCandleData->dOpen[i] <= pstCandleData->dClose[i-1]		&&

			pstCandleData->dClose[i-1] >= pstCandleData->dClose[i-2]	&&
			pstCandleData->dClose[i]   >= pstCandleData->dClose[i-1]	&&

			pstCandleData->dHigh[i-1] >= pstCandleData->dHigh[i-2]		&&
			pstCandleData->dHigh[i]   >= pstCandleData->dHigh[i-1]		)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_THREE_WHITESOLDIER, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 29. Three Black Corws					(흑삼병)           			                                	
// ******************************************************************
int CCandlePattern::CDFind_ThreeBlackCorws(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-1)						&&
			IsCandleBlack(		pstCandleData, i)						&&

			pstCandleData->dOpen[i-1] >= pstCandleData->dClose[i-2]		&&
			pstCandleData->dOpen[i-1] <= pstCandleData->dOpen[i-2]		&&

			pstCandleData->dOpen[i] >= pstCandleData->dClose[i-1]		&&
			pstCandleData->dOpen[i] <= pstCandleData->dOpen[i-1]		&&

			pstCandleData->dClose[i-1] < pstCandleData->dClose[i-2]		&&
			pstCandleData->dClose[i]   < pstCandleData->dClose[i-1]		&&

			pstCandleData->dLow[i-1] < pstCandleData->dLow[i-2]			&&
			pstCandleData->dLow[i]   < pstCandleData->dLow[i-1]			)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_THREE_BLACKCROW, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 30. Stalled Pattern					(지연형)           			                            		
// ******************************************************************
int CCandlePattern::CDFind_StalledPattern(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-1)						&&
			IsCandleWhite(		pstCandleData, i)						&&

			pstCandleData->dOpen[i-1] >= pstCandleData->dOpen[i-2]		&&
			pstCandleData->dOpen[i-1] <= pstCandleData->dClose[i-2]		&&

			pstCandleData->dClose[i-1] > pstCandleData->dClose[i-2]		&&
			pstCandleData->dClose[i]   > pstCandleData->dClose[i-1]		&&

			pstCandleData->dHigh[i-1] > pstCandleData->dHigh[i-2]		&&
			pstCandleData->dHigh[i]   > pstCandleData->dHigh[i-1]		&&
			
			IsCandleShort(		pstCandleData, i)						&&
			IsCandleGapUp(		pstCandleData, i)						)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_STALLEDPATTERN, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 31. Advance Block						(블록형)
// ******************************************************************
int CCandlePattern::CDFind_AdvanceBlock(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)												&&
			IsCandleWhite(		pstCandleData, i-2)												&&
			IsCandleWhite(		pstCandleData, i-1)												&&
			IsCandleWhite(		pstCandleData, i)												&&

			pstCandleData->dOpen[i-1] >= pstCandleData->dOpen[i-2]								&&
			pstCandleData->dOpen[i-1] <= pstCandleData->dClose[i-2]								&&

			pstCandleData->dOpen[i] >= pstCandleData->dOpen[i-1]								&&
			pstCandleData->dOpen[i] <= pstCandleData->dClose[i-1]								&&

			pstCandleData->dClose[i-1] > pstCandleData->dClose[i-2]								&&
			pstCandleData->dClose[i]   > pstCandleData->dClose[i-1]								&&

			pstCandleData->dHigh[i-1] > pstCandleData->dHigh[i-2]								&&
			pstCandleData->dHigh[i]   > pstCandleData->dHigh[i-1]								&&

			GetCandleBodyHeight( pstCandleData, i-1) < GetCandleBodyHeight( pstCandleData, i-2)	&&
			GetCandleBodyHeight( pstCandleData, i)   < GetCandleBodyHeight( pstCandleData, i-1)	)
			
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_ADVANCEBLOCK, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 32. Upside Gap Tasuki					(상승타스키갭형)   			                            		
// ******************************************************************
int CCandlePattern::CDFind_UpsideGapTasuki(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)					&&
			IsCandleWhite(		pstCandleData, i-2)					&&
			IsCandleWhite(		pstCandleData, i-1)					&&
			IsCandleGapUp(		pstCandleData, i-1)					&&
			IsCandleBlack(		pstCandleData, i)					&&

			pstCandleData->dOpen[i] >= pstCandleData->dOpen[i-1]	&&
			pstCandleData->dOpen[i] <= pstCandleData->dClose[i-1]	&&

			pstCandleData->dClose[i] < pstCandleData->dOpen[i-1]	&&
			pstCandleData->dClose[i] > pstCandleData->dClose[i-2]	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_UPSIDEGAP_TASUKI, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 33. Downside Gap Tasuki				(하락타스키갭형)   			                                	
// ******************************************************************
int CCandlePattern::CDFind_DownsideGapTasuki(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)					&&
			IsCandleBlack(		pstCandleData, i-2)					&&
			IsCandleBlack(		pstCandleData, i-1)					&&
			IsCandleGapDown(	pstCandleData, i-1)					&&
			IsCandleWhite(		pstCandleData, i)					&&

			pstCandleData->dOpen[i] >= pstCandleData->dClose[i-1]	&&
			pstCandleData->dOpen[i] <= pstCandleData->dOpen[i-1]	&&

			pstCandleData->dClose[i] > pstCandleData->dOpen[i-1]	&&
			pstCandleData->dClose[i] < pstCandleData->dClose[i-2]	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_DNSIDEGAP_TASUKI, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 34. Mat-Hold							(매트형)
// ******************************************************************
int CCandlePattern::CDFind_MatHold(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;
	double dHighest;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
// 07.01.13 Modify. 수정 이준옥, 이유:이운교수정요구
//		dHighest = pstCandleData->dHigh[i-1];
//		for ( int j = 2; j < nMinDataCnt; j++)
//		{
//			if ( dHighest < pstCandleData->dHigh[i-j])
//				dHighest = pstCandleData->dHigh[i-j];
//		}

		dHighest = GetCandleBodyHeight( pstCandleData, i-1);
		for ( int j = 2; j < 5; j++)
		{

			if ( dHighest < GetCandleBodyHeight( pstCandleData, i-j))
				dHighest = GetCandleBodyHeight( pstCandleData, i-j);
		}

		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-4)							&&
			IsCandleWhite(		pstCandleData, i-4)							&&
			IsCandleLong(		pstCandleData, i-4)							&&

			pstCandleData->dClose[i-3]	> pstCandleData->dClose[i-4]		&&

			IsCandleBlack(		pstCandleData, i-3)							&&
			IsCandleBlack(		pstCandleData, i-2)							&&
			IsCandleBlack(		pstCandleData, i-1)							&&

			pstCandleData->dOpen[i-2]	< pstCandleData->dOpen[i-3]			&&
			pstCandleData->dOpen[i-1]	< pstCandleData->dOpen[i-2]			&&
			
			pstCandleData->dClose[i-2]	< pstCandleData->dClose[i-3]		&&
			pstCandleData->dClose[i-1]	< pstCandleData->dClose[i-2]		&&

			pstCandleData->dOpen[i]		> pstCandleData->dOpen[i-1]			&&
			
			IsCandleWhite(		pstCandleData, i)							&&
			IsCandleLong(		pstCandleData, i)							&&

			pstCandleData->dClose[i] > dHighest								)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_MATHOLD, i-4, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 35. Bullish Tri Star					(상승십자별형)     			
// ******************************************************************
int CCandlePattern::CDFind_Bullish_TriStar(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)														&&
			IsCandleDoji(		pstCandleData, i-2)														&&
			IsCandleDoji(		pstCandleData, i-1)														&&

			GetCandleBodyHighValue(	pstCandleData, i-1)	< GetCandleBodyLowValue( pstCandleData, i-2)	&&
			IsCandleDoji(		pstCandleData, i)														&&
			GetCandleBodyLowValue(	pstCandleData, i)	> GetCandleBodyHighValue( pstCandleData, i-1)	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_TRISTAR, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 36. Bearish Tri Star					(하락십자별형)     			                            		
// ******************************************************************
int CCandlePattern::CDFind_Bearish_TriStar(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)														&&
			IsCandleDoji(		pstCandleData, i-2)														&&
			IsCandleDoji(		pstCandleData, i-1)														&&

			GetCandleBodyLowValue(	pstCandleData, i-1)	> GetCandleBodyHighValue( pstCandleData, i-2)	&&
			IsCandleDoji(		pstCandleData, i)														&&
			GetCandleBodyHighValue(	pstCandleData, i)	< GetCandleBodyLowValue( pstCandleData, i-1)	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_TRISTAR, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 37. Unique Three River Bottom			(희귀삼천바닥형)   			                            		
// ******************************************************************
int CCandlePattern::CDFind_UniqueThreeRiverBottom(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)					&&
			IsCandleBlack(		pstCandleData, i-2)					&&
			IsCandleLong(		pstCandleData, i-2)					&&
			IsCandleBlack(		pstCandleData, i-1)					&&
			IsCandleHarami(		pstCandleData, i-1)					&&

			pstCandleData->dLow[i-1] < pstCandleData->dLow[i]		&&
			pstCandleData->dLow[i-1] < pstCandleData->dLow[i-2]		&&

			IsCandleWhite(		pstCandleData, i)					&&
			
			pstCandleData->dOpen[i]  > pstCandleData->dLow[i-1]		&&
			pstCandleData->dClose[i] < pstCandleData->dOpen[i-1]	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_UNIQUETHREERIVERBOTTOM, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 38. Identical Three Crows				(동일흑삼병형)     			
// ******************************************************************
int CCandlePattern::CDFind_IdenticalThreeCrows(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)																		&&
			IsCandleBlack(		pstCandleData, i-2)																		&&
			IsCandleBlack(		pstCandleData, i-1)																		&&
			IsCandleBlack(		pstCandleData, i)																		&&

			fabs( pstCandleData->dOpen[i-1] - pstCandleData->dClose[i-2]) < GetCandleAvgHeight(	pstCandleData, i)*0.1	&&
			fabs( pstCandleData->dOpen[i]   - pstCandleData->dClose[i-1]) < GetCandleAvgHeight(	pstCandleData, i)*0.1	&&

			pstCandleData->dClose[i-1]	< pstCandleData->dClose[i-2]													&&
			pstCandleData->dClose[i]	< pstCandleData->dClose[i-1] 													)
			
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_IDENTICAL_THREECROWS, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 39. Bullish Breakaway					(상승탈출형)       			                                	
// ******************************************************************
int CCandlePattern::CDFind_Bullish_Breakaway(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-4)										&&
			IsCandleBlack(		pstCandleData, i-4)										&&
			IsCandleLong(		pstCandleData, i-4)										&&

			IsCandleBlack(		pstCandleData, i-3)										&&
			IsCandleBlack(		pstCandleData, i-1)										&&
			IsCandleWhite(		pstCandleData, i)										&&
			IsCandleLong(		pstCandleData, i)										&&

			pstCandleData->dClose[i-4]	> pstCandleData->dOpen[i-3]						&&

			GetCandleBodyHighValue( pstCandleData, i-2) < pstCandleData->dOpen[i-3]		&&
			GetCandleBodyHighValue( pstCandleData, i-2) > pstCandleData->dOpen[i-1]		&&

			GetCandleBodyLowValue( pstCandleData, i-2) < pstCandleData->dClose[i-3]		&&
			GetCandleBodyLowValue( pstCandleData, i-2) > pstCandleData->dClose[i-1]		&&

			pstCandleData->dClose[i] < pstCandleData->dClose[i-4] 						&&
			pstCandleData->dClose[i] > pstCandleData->dOpen[i-3]						)	
			
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_BREAKAWAY, i-4, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 40. Bearish Breakaway					(하락탈출형)       			                                	
// ******************************************************************
int CCandlePattern::CDFind_Bearish_Breakaway(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-4)										&&
			IsCandleWhite(		pstCandleData, i-4)										&&
			IsCandleLong(		pstCandleData, i-4)										&&

			IsCandleWhite(		pstCandleData, i-3)										&&
			IsCandleWhite(		pstCandleData, i-1)										&&
			IsCandleBlack(		pstCandleData, i)										&&
			IsCandleLong(		pstCandleData, i)										&&

			pstCandleData->dClose[i-4]	< pstCandleData->dOpen[i-3]						&&

			GetCandleBodyLowValue( pstCandleData, i-2) > pstCandleData->dOpen[i-3]		&&
			GetCandleBodyLowValue( pstCandleData, i-2) < pstCandleData->dOpen[i-1]		&&

			GetCandleBodyHighValue( pstCandleData, i-2) > pstCandleData->dClose[i-3]	&&
			GetCandleBodyHighValue( pstCandleData, i-2) < pstCandleData->dClose[i-1]	&&

			pstCandleData->dClose[i] > pstCandleData->dClose[i-4] 						&&
			pstCandleData->dClose[i] < pstCandleData->dOpen[i-3]						)	
			
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_BREAKAWAY, i-4, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 41. Two Crows							(약세까마귀형)     			                                	
// ******************************************************************
int CCandlePattern::CDFind_TwoCrows(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleLong(		pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-1)						&&
			pstCandleData->dClose[i-1] > pstCandleData->dClose[i-2]		&&
			IsCandleBlack(		pstCandleData, i)						&&
			
			pstCandleData->dOpen[i] >= pstCandleData->dClose[i-1]		&&
			pstCandleData->dOpen[i] <= pstCandleData->dOpen[i-1]		&&

			pstCandleData->dClose[i] > pstCandleData->dOpen[i-2]		&&
			pstCandleData->dClose[i] < pstCandleData->dClose[i-2]		)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_TWOCORWS, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 42. Three Stars In The South			(남삼성형)         			
// ******************************************************************
int CCandlePattern::CDFind_ThreeStarsInTheSouth(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)					&&
			IsCandleBlack(		pstCandleData, i-2)					&&
			IsCandleLong(		pstCandleData, i-2)					&&

			IsCandleBlack(		pstCandleData, i-1)					&&
			IsCandleBlack(		pstCandleData, i)					&&

			pstCandleData->dOpen[i-1] > pstCandleData->dClose[i-2]	&&
			pstCandleData->dOpen[i-1] < pstCandleData->dOpen[i-2]	&&

			pstCandleData->dOpen[i] > pstCandleData->dClose[i-1]	&&
			pstCandleData->dOpen[i] < pstCandleData->dOpen[i-1]		&&

			pstCandleData->dClose[i-1] > pstCandleData->dClose[i]	&&
			pstCandleData->dClose[i-1] < pstCandleData->dClose[i-2]	&&

			pstCandleData->dLow[i-1] < pstCandleData->dLow[i]		&&		
			pstCandleData->dLow[i-1] > pstCandleData->dLow[i-2]		)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_THREESTARS_INTHESOUTH, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 43. Concealing Baby Swallow			(애기제비감추기형) 			
// ******************************************************************
int CCandlePattern::CDFind_ConcealingBabySwallow(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check                                                                                                
		if (IsCandleTrendDown(	pstCandleData, i-3)													&&
			IsCandleBlack(		pstCandleData, i-3)													&&
			IsCandleLong(		pstCandleData, i-3)													&&
			IsCandleMarubozu(	pstCandleData, i-3)													&&
                                                                    								  
			IsCandleBlack(		pstCandleData, i-2)													&&
			IsCandleLong(		pstCandleData, i-2)													&&
			IsCandleMarubozu(	pstCandleData, i-2)													&&
                                                                    								  
			IsCandleBlack(		pstCandleData, i-1)													&&
                                                                    								  
			IsCandleBlack(		pstCandleData, i)													&&
			IsCandleLong(		pstCandleData, i)													&&
			IsCandleMarubozu(	pstCandleData, i)													&&
                                                                    								  
			pstCandleData->dClose[i-2] > pstCandleData->dOpen[i-1]									&&
                                                                                                
			GetCandleShadowUp( pstCandleData, i-1) > GetCandleBodyHeight( pstCandleData, i-1)*2		&&
                                                                                                
			pstCandleData->dClose[i] < pstCandleData->dLow[i-1]  									&&
			pstCandleData->dOpen[i] > pstCandleData->dHigh[i-1] 									)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_CONCEALINGBABY_SWALLOW, i-3, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 44. Stick Sandwich					(막대샌드위치형)   			
// ******************************************************************
int CCandlePattern::CDFind_StickSandwich(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)													&&
			IsCandleBlack(		pstCandleData, i-2)													&&
			GetCandleShadowDown(pstCandleData, i-2) == 0											&&

			IsCandleWhite(		pstCandleData, i-1)													&&

			IsCandleBlack(		pstCandleData, i)													&&
			GetCandleShadowDown(pstCandleData, i) == 0												&&

			pstCandleData->dOpen[i-1] > pstCandleData->dClose[i-2]									&&
			pstCandleData->dLow[i-1]  > pstCandleData->dClose[i-2]									&&

			pstCandleData->dOpen[i]	  > max( pstCandleData->dHigh[i-1], pstCandleData->dHigh[i-2])	&&

			pstCandleData->dClose[i]  == pstCandleData->dClose[i-2]									)
			
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_STICKSANDWICH, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 45. Bullish Kicking					(상승박차형)	   			                            		
// ******************************************************************													
int CCandlePattern::CDFind_Bullish_Kicking(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_3)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_3;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleBlack(		pstCandleData, i-1)					&&
			IsCandleLong(		pstCandleData, i-1)					&&
			IsCandleMarubozu(	pstCandleData, i-1)					&&

			IsCandleWhite(		pstCandleData, i)					&&
			IsCandleLong(		pstCandleData, i)					&&
			IsCandleMarubozu(	pstCandleData, i)					&&

			pstCandleData->dLow[i] > pstCandleData->dHigh[i-1]		)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_KICKING, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 46. Bearish Kicking					(하락박차형)       			                            		
// ******************************************************************													
int CCandlePattern::CDFind_Bearish_Kicking(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_3)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_3;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleWhite(		pstCandleData, i-1)					&&
			IsCandleLong(		pstCandleData, i-1)					&&
			IsCandleMarubozu(	pstCandleData, i-1)					&&

			IsCandleBlack(		pstCandleData, i)					&&
			IsCandleLong(		pstCandleData, i)					&&
			IsCandleMarubozu(	pstCandleData, i)					&&

			pstCandleData->dLow[i-1] > pstCandleData->dHigh[i]		)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_KICKING, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 47. Homing Pigeon						(전서구형)         			                            		
// ******************************************************************													
int CCandlePattern::CDFind_HomingPigeon(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)					&&
			IsCandleBlack(		pstCandleData, i-1)					&&
			IsCandleLong(		pstCandleData, i-1)					&&
			IsCandleBlack(		pstCandleData, i)					&&
			IsCandleHarami(		pstCandleData, i)					)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_HOMINGPIGEON, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 48. Ladder Bottom						(사다리바닥형)     			                            		
// ******************************************************************
int CCandlePattern::CDFind_LadderBottom(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-4)													&&
			IsCandleBlack(		pstCandleData, i-4)													&&
			IsCandleBlack(		pstCandleData, i-3)													&&
			IsCandleBlack(		pstCandleData, i-2)													&&

			pstCandleData->dOpen[i-3] >= pstCandleData->dClose[i-4] 								&&
			pstCandleData->dOpen[i-3] <= pstCandleData->dOpen[i-4]									&&

			pstCandleData->dOpen[i-2] >= pstCandleData->dClose[i-3] 								&&
			pstCandleData->dOpen[i-2] <= pstCandleData->dOpen[i-3]									&&

			pstCandleData->dClose[i-3] > pstCandleData->dClose[i-2] 								&&
			pstCandleData->dClose[i-3] < pstCandleData->dClose[i-4]									&&

			pstCandleData->dLow[i-3] > pstCandleData->dLow[i-2] 									&&
			pstCandleData->dLow[i-3] < pstCandleData->dLow[i-4]										&&

			IsCandleBlack(		pstCandleData, i-1)													&&
			pstCandleData->dClose[i-1] < pstCandleData->dClose[i-2]									&&
			GetCandleShadowUp( pstCandleData, i-1) > GetCandleBodyHeight( pstCandleData, i-1)*2		&&

			IsCandleWhite(		pstCandleData, i)													&&
			pstCandleData->dOpen[i-1] < pstCandleData->dOpen[i]										&&

			pstCandleData->dClose[i] > max( pstCandleData->dHigh[i-1], pstCandleData->dHigh[i-2])	)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_LADDERBOTTOM, i-4, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 49. Matching Low						(동일저점형)       			                            		
// ******************************************************************
int CCandlePattern::CDFind_MatchingLow(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)						&&
			IsCandleBlack(		pstCandleData, i-1)						&&
			IsCandleLong(		pstCandleData, i-1)						&&

			IsCandleBlack(		pstCandleData, i)						&&
			pstCandleData->dClose[i] == pstCandleData->dClose[i-1]		)
			
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_MATCHINGLOW, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 50. Bullish Side-By-Side White Lines	(상승나란히형)     			
// ******************************************************************
int CCandlePattern::CDFind_Bullish_SideBySideWhiteLines(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)					&&
			IsCandleWhite(		pstCandleData, i-2)					&&
			IsCandleLong(		pstCandleData, i-2)					&&
			
			IsCandleWhite(		pstCandleData, i-1)					&&
			pstCandleData->dOpen[i-1] > pstCandleData->dClose[i-2]	&&

			IsCandleWhite(		pstCandleData, i)					&&

			fabs( GetCandleBodyHeight( pstCandleData, i) - GetCandleBodyHeight( pstCandleData, i-1)) 
																		< GetCandleBodyHeight( pstCandleData, i)*0.1	&&

			fabs( pstCandleData->dOpen[i] - pstCandleData->dOpen[i-1])  < GetCandleAvgHeight( pstCandleData, i)*0.1	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_SIDEBYSIDE_WHITELINE, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 51. Bearish Side-By-Side White Lines	(하락나란히형)     			
// ******************************************************************
int CCandlePattern::CDFind_Bearish_SideBySideWhiteLines(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)					&&
			IsCandleBlack(		pstCandleData, i-2)					&&
			IsCandleLong(		pstCandleData, i-2)					&&
			
			IsCandleWhite(		pstCandleData, i-1)					&&
			pstCandleData->dClose[i-1] < pstCandleData->dClose[i-2]	&&

			IsCandleWhite(		pstCandleData, i)					&&

			fabs( GetCandleBodyHeight( pstCandleData, i) - GetCandleBodyHeight( pstCandleData, i-1)) 
																		< GetCandleBodyHeight( pstCandleData, i)*0.1	&&

			fabs( pstCandleData->dClose[i] - pstCandleData->dClose[i-1])< GetCandleAvgHeight( pstCandleData, i)*0.1	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_SIDEBYSIDE_WHITELINE, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 52. Rising Three Methods				(상승삼법형)       			
// ******************************************************************
int CCandlePattern::CDFind_RisingThreeMethods(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-4)										&&
			IsCandleWhite(		pstCandleData, i-4)										&&
			IsCandleLong(		pstCandleData, i-4)										&&

			IsCandleBlack(		pstCandleData, i-3)										&&
			IsCandleBlack(		pstCandleData, i-1)										&&

			GetCandleBodyHighValue( pstCandleData, i-2) < pstCandleData->dOpen[i-3]		&&
			GetCandleBodyHighValue( pstCandleData, i-2) > pstCandleData->dOpen[i-1]		&&

			GetCandleBodyLowValue( pstCandleData, i-2) < pstCandleData->dClose[i-3]		&&
			GetCandleBodyLowValue( pstCandleData, i-2) > pstCandleData->dClose[i-1]		&&

			pstCandleData->dOpen[i-3]	< pstCandleData->dHigh[i-4]						&&
			pstCandleData->dClose[i-1]	> pstCandleData->dLow[i-4]						&&

			IsCandleWhite(		pstCandleData, i)										&&
			IsCandleLong(		pstCandleData, i)										&&

			pstCandleData->dClose[i] > pstCandleData->dClose[i-4] 						&&
			pstCandleData->dClose[i] > pstCandleData->dOpen[i-3]						)	
			
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_RISING_THREEMETHODS, i-4, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 53. Falling Three Methods				(하락삼법형)       			                                	
// ******************************************************************
int CCandlePattern::CDFind_FallingThreeMethods(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-4)										&&
			IsCandleBlack(		pstCandleData, i-4)										&&
			IsCandleLong(		pstCandleData, i-4)										&&

			IsCandleBlack(		pstCandleData, i-3)										&&
			IsCandleBlack(		pstCandleData, i-1)										&&

			GetCandleBodyLowValue( pstCandleData, i-2) > pstCandleData->dOpen[i-3]		&&
			GetCandleBodyLowValue( pstCandleData, i-2) < pstCandleData->dOpen[i-1]		&&

			GetCandleBodyHighValue( pstCandleData, i-2) > pstCandleData->dClose[i-3]	&&
			GetCandleBodyHighValue( pstCandleData, i-2) < pstCandleData->dClose[i-1]	&&

			pstCandleData->dOpen[i-3]	> pstCandleData->dLow[i-4]						&&
			pstCandleData->dClose[i-1]	< pstCandleData->dHigh[i-4]						&&

			IsCandleBlack(		pstCandleData, i)										&&
			IsCandleLong(		pstCandleData, i)										&&

			pstCandleData->dClose[i] < pstCandleData->dClose[i-4] 						&&
			pstCandleData->dClose[i] < pstCandleData->dOpen[i-3]						)	
			
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_FALLING_THREEMETHODS, i-4, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 54. Bullish Separating Lines			(상승갈림길형)     			
// ******************************************************************
int CCandlePattern::CDFind_Bullish_SeparatingLines(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-1)						&&
			IsCandleBlack(		pstCandleData, i-1)						&&
			IsCandleLong(		pstCandleData, i-1)						&&

			IsCandleWhite(		pstCandleData, i)						&&
			IsCandleLong(		pstCandleData, i)						&&

			fabs( pstCandleData->dOpen[i] - pstCandleData->dOpen[i-1]) < GetCandleAvgHeight( pstCandleData, i)*0.1	)
			
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_SEPARATINGLINES, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 55. Bearish Separating Lines			(하락갈림길형)     			
// ******************************************************************
int CCandlePattern::CDFind_Bearish_SeparatingLines(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)						&&
			IsCandleWhite(		pstCandleData, i-1)						&&
			IsCandleLong(		pstCandleData, i-1)						&&

			IsCandleBlack(		pstCandleData, i)						&&
			IsCandleLong(		pstCandleData, i)						&&

			fabs( pstCandleData->dOpen[i] - pstCandleData->dOpen[i-1]) < GetCandleAvgHeight( pstCandleData, i)*0.1	)
			
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_SEPARATINGLINES, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 56. Bullish Three-Line Strike			(강세삼선반격형)   			
// ******************************************************************
int CCandlePattern::CDFind_Bullish_ThreeLineStrike(	LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check.
		if (IsCandleTrendUp(	pstCandleData, i-3)						&& 
			IsCandleWhite(		pstCandleData, i-3)						&&	
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-1)						&&

			pstCandleData->dClose[i-2] > pstCandleData->dClose[i-3]		&&
			pstCandleData->dClose[i-2] < pstCandleData->dClose[i-1] 	&&
			
			pstCandleData->dLow[i-2] > pstCandleData->dLow[i-3]			&&
			pstCandleData->dLow[i-2] < pstCandleData->dLow[i-1] 		&&
			
			pstCandleData->dHigh[i-2] > pstCandleData->dHigh[i-3]		&&
			pstCandleData->dHigh[i-2] < pstCandleData->dHigh[i-1] 		&&

			pstCandleData->dOpen[i-2] < pstCandleData->dClose[i-3]		&&
			pstCandleData->dOpen[i-2] > pstCandleData->dOpen[i-3]		&&

			pstCandleData->dOpen[i-1] < pstCandleData->dClose[i-2]		&&
			pstCandleData->dOpen[i-1] > pstCandleData->dOpen[i-2]		&&

			IsCandleBlack(		pstCandleData, i)						&&
			IsCandleLong(		pstCandleData, i)						&&

			pstCandleData->dOpen[i]	> pstCandleData->dClose[i-1]		&&
			pstCandleData->dClose[i] < pstCandleData->dOpen[i-3]		)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_THREELINESTRIKE, i-3, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 57. Bearish Three-Line Strike			(약세삼선반격형)   			
// ******************************************************************
int CCandlePattern::CDFind_Bearish_ThreeLineStrike(	LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check.
		if (IsCandleTrendDown(	pstCandleData, i-3)						&& 
			IsCandleBlack(		pstCandleData, i-3)						&&	
			IsCandleBlack(		pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-1)						&&

			pstCandleData->dClose[i-2] < pstCandleData->dClose[i-3]		&&
			pstCandleData->dClose[i-2] > pstCandleData->dClose[i-1] 	&&
			
			pstCandleData->dLow[i-2] < pstCandleData->dLow[i-3]			&&
			pstCandleData->dLow[i-2] > pstCandleData->dLow[i-1] 		&&
			
			pstCandleData->dHigh[i-2] < pstCandleData->dHigh[i-3]		&&
			pstCandleData->dHigh[i-2] > pstCandleData->dHigh[i-1] 		&&

			pstCandleData->dOpen[i-2] < pstCandleData->dOpen[i-3]		&&
			pstCandleData->dOpen[i-2] > pstCandleData->dClose[i-3]		&&

			pstCandleData->dOpen[i-1] < pstCandleData->dOpen[i-2]		&&
			pstCandleData->dOpen[i-1] > pstCandleData->dClose[i-2]		&&

			IsCandleWhite(		pstCandleData, i)						&&
			IsCandleLong(		pstCandleData, i)						&&

			pstCandleData->dOpen[i]	< pstCandleData->dClose[i-1]		&&
			pstCandleData->dClose[i] > pstCandleData->dOpen[i-3]		)
		{

			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BEARISH_THREELINESTRIKE, i-3, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 58. Upside Gap Three Methods			(상승갭삼법형)     			
// ******************************************************************
int CCandlePattern::CDFind_UpsideGapThreeMethods(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendUp(	pstCandleData, i-2)						&&
			IsCandleWhite(		pstCandleData, i-2)						&&
			IsCandleLong(		pstCandleData, i-2)						&&

			IsCandleWhite(		pstCandleData, i-1)						&&
			IsCandleLong(		pstCandleData, i-1)						&&
			pstCandleData->dOpen[i-1] > pstCandleData->dClose[i-2]		&&

			IsCandleBlack(		pstCandleData, i)						&&

			pstCandleData->dOpen[i] < pstCandleData->dClose[i-1]		&&
			pstCandleData->dOpen[i] > pstCandleData->dOpen[i-1]			&&

			pstCandleData->dClose[i] < pstCandleData->dClose[i-2]		)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_UPSIDEGAP_THREEMETHODS, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 59. Downside Gap Three Methods		(하락갭삼법형)     			
// ******************************************************************
int CCandlePattern::CDFind_DownsideGapThreeMethods(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-2)						&&
			IsCandleBlack(		pstCandleData, i-2)						&&
			IsCandleLong(		pstCandleData, i-2)						&&

			IsCandleBlack(		pstCandleData, i-1)						&&
			IsCandleLong(		pstCandleData, i-1)						&&
			pstCandleData->dOpen[i-1] < pstCandleData->dClose[i-2]		&&

			IsCandleWhite(		pstCandleData, i)						&&

			pstCandleData->dOpen[i] > pstCandleData->dClose[i-1]		&&
			pstCandleData->dOpen[i] < pstCandleData->dOpen[i-1]			&&

			pstCandleData->dClose[i] > pstCandleData->dClose[i-2]		)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_DNSIDEGAP_THREEMETHODS, i-2, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 60. On Neck							(걸침형)           			
// ******************************************************************
int CCandlePattern::CDFind_OnNeck(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)					&&
			IsCandleBlack(		pstCandleData, i-1)					&&
			IsCandleLong(		pstCandleData, i-1)					&&
			IsCandleWhite(		pstCandleData, i)					&&
			GetCandleShadowUp(	pstCandleData, i) == 0				&&

			pstCandleData->dOpen[i] < pstCandleData->dLow[i-1]		&&
			pstCandleData->dClose[i] == pstCandleData->dLow[i-1]	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_ON_NECK, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 61. In Neck							(진입형)           			                    				
// ******************************************************************
int CCandlePattern::CDFind_InNeck(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)																&&
			IsCandleBlack(		pstCandleData, i-1)																&&
			IsCandleLong(		pstCandleData, i-1)																&&
			IsCandleWhite(		pstCandleData, i)																&&
			
			pstCandleData->dOpen[i] < pstCandleData->dLow[i-1]													&&
			pstCandleData->dClose[i] - pstCandleData->dClose[i-1] >= 0											&&
			pstCandleData->dClose[i] - pstCandleData->dClose[i-1] < GetCandleAvgHeight( pstCandleData, i)*0.1	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_IN_NECK, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 62. Thrusting Line					(공격형)           			
// ******************************************************************
int CCandlePattern::CDFind_ThrustingLine(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i-1)									&&
			IsCandleBlack(		pstCandleData, i-1)									&&
			IsCandleLong(		pstCandleData, i-1)									&&
			IsCandleWhite(		pstCandleData, i)									&&

			pstCandleData->dOpen[i] < pstCandleData->dLow[i-1]						&&
			pstCandleData->dClose[i] > pstCandleData->dClose[i-1]					&&
			pstCandleData->dClose[i] < GetCandleBodyMidValue( pstCandleData, i-1)	)
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_THRUSTINGLINE, i-1, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
}

// ******************************************************************
// ## 63. Bullish GraveStone				(강세 비석형)
// ******************************************************************
int CCandlePattern::CDFind_Bullish_GraveStone(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
/*
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i)												&&
			GetCandleShadowDown(pstCandleData, i) < GetCandleHeight( pstCandleData, i)*0.1		&&
			IsCandleDoji(		pstCandleData, i)												&&
			GetCandleHeight(	pstCandleData, i) > GetCandleAvgHeight(	pstCandleData, i)		) 
		{
			
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_GRAVESTONE, i, i);
			
			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
*/
	return RTN_NONE;
}

// ******************************************************************
// ## 64. Bullish DragonFly					(강세 잠자리형)
// ******************************************************************
int CCandlePattern::CDFind_Bullish_DragonFly(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
/*
	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePatternData* pCandlePatternData = NULL;
	BOOL bFind = FALSE;

	int nMinDataCnt;

	if ( pstCandleData->nDataCnt < NEED_PERIOD_20)
		return RTN_FEWDATA;

	nMinDataCnt = NEED_PERIOD_20;

	for ( int i = nMinDataCnt-1; i < pstCandleData->nDataCnt; i++)
	{
		// Condition Check
		if (IsCandleTrendDown(	pstCandleData, i)												&&
			GetCandleShadowUp(	pstCandleData, i) < GetCandleHeight( pstCandleData, i)*0.1		&&
			IsCandleDoji(		pstCandleData, i)												&&
			GetCandleHeight(	pstCandleData, i) > GetCandleAvgHeight(	pstCandleData, i)		)
		{
	
			// Add Candle Pattern Data
			pCandlePatternData = new CCandlePatternData;
			SetCandlePatternData( pCandlePatternData, pstCandleData, CPTN_BULLISH_DRAGONFLY, i, i);

			paryCandlePatternData->Add( pCandlePatternData);
			bFind = TRUE;
		}
	}

	if ( bFind)		return RTN_FIND;
	else			return RTN_NONE;
*/
	return RTN_NONE;
}
// <<=====================================================================================

// *****************************************************
// Set Candle Pattern Data
// *****************************************************
void CCandlePattern::SetCandlePatternData(	CObject* pObjCandlePatternData, LPSTCANDLE_DATA pstCandleData, 
											int nCandlePtnType, int nStPos, int nEndPos)
{
	CCandlePatternData* pCandlePatternData = (CCandlePatternData*)pObjCandlePatternData;

	pCandlePatternData->m_dTime				= pstCandleData->dTime[nEndPos];
	pCandlePatternData->m_nCandlePtnType	= nCandlePtnType;
	pCandlePatternData->m_nStartPos			= nStPos;
	pCandlePatternData->m_nEndPos			= nEndPos;
	pCandlePatternData->m_dHigh				= pstCandleData->dHigh[nEndPos];
	pCandlePatternData->m_dLow				= pstCandleData->dLow[nEndPos];

	// Close Price for Accumulation rise and fall rate
	for ( int i = nEndPos; i < pstCandleData->nDataCnt; i++)
	{
		if ( i-nEndPos < ACCUMULATION_PERIOD)
		{
			pCandlePatternData->m_dClose[i-nEndPos] = pstCandleData->dClose[i];
		}
 	}
}

// *****************************************************
// Find Candle Pattern Data
// *****************************************************
void CCandlePattern::CDFind_All_CandlePattern(	LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData)
{
	// ## 01. Hammer							(망치형)           														
	CDFind_Hammer(						pstCandleData, pObjCandlePatternData);
	// ## 02. Hanging Man						(교수형)           			                            		
	CDFind_HangingMan(					pstCandleData, pObjCandlePatternData);
	// ## 03. Inverted Hammer					(역망치형)         			                            		
	CDFind_InvertedHammer(				pstCandleData, pObjCandlePatternData);
	// ## 04. Shooting Star						(유성형)           			                            		
	CDFind_ShootingStar(				pstCandleData, pObjCandlePatternData);
	// ## 05. Bullish Belt Hold Line			(상승샅바형)       			
	CDFind_Bullish_BeltHoldLine(		pstCandleData, pObjCandlePatternData);
	// ## 06. Bearish Belt Hold Line			(하락샅바형)       			                                	
	CDFind_Bearish_BeltHoldLine(		pstCandleData, pObjCandlePatternData);
	// ## 07. Long White Body					(장대양봉)         			
	CDFind_Long_WhiteBody(				pstCandleData, pObjCandlePatternData);
	// ## 08. Long Black Body					(장대음봉)         			                            		
	CDFind_Long_BlackBody(				pstCandleData, pObjCandlePatternData);
	// ## 09. Bullish Harami					(상승잉태형)       			                            		
	CDFind_Bullish_Harami(				pstCandleData, pObjCandlePatternData);
	// ## 10. Bearish Harami					(하락잉태형)       			                            		
	CDFind_Bearish_Harami(				pstCandleData, pObjCandlePatternData);
	// ## 11. Bullish Engulfing					(상승장악형)       			
	CDFind_Bullish_Engulfing(			pstCandleData, pObjCandlePatternData);
	// ## 12. Bearish Engulfing					(하락장악형)       			                                	
	CDFind_Bearish_Engulfing(			pstCandleData, pObjCandlePatternData);
	// ## 13. Bullish Counterattack Line		(상승반격형)       			
	CDFind_Bullish_CounterattackLine(	pstCandleData, pObjCandlePatternData);
	// ## 14. Bearish Counterattack Line		(하락반격형)       			
	CDFind_Bearish_CounterattackLine(	pstCandleData, pObjCandlePatternData);
	// ## 15. Piercing Pattern					(관통형)           			
	CDFind_PiercingPattern(				pstCandleData, pObjCandlePatternData);
	// ## 16. Dark Cloud Cover					(먹구름형)         			                            		
	CDFind_DarkCloudCover(				pstCandleData, pObjCandlePatternData);
	// ## 17. Morning Star						(샛별형)           			                            		
	CDFind_MorningStar(					pstCandleData, pObjCandlePatternData);
	// ## 18. Evening Star						(석별형)           			                        			
	CDFind_EveningStar(					pstCandleData, pObjCandlePatternData);
	// ## 19. Morning Doji Star					(십자샛별형)       			                            		
	CDFind_MorningDojiStar(				pstCandleData, pObjCandlePatternData);
	// ## 20. Evening Doji Star					(십자석별형)       			                            		
	CDFind_EveningDojiStar(				pstCandleData, pObjCandlePatternData);
	// ## 21. Abandoned Baby In Downtrend		(상승기아형)       			
	CDFind_AbandonedBabyInDowntrend(	pstCandleData, pObjCandlePatternData);
	// ## 22. Abandoned Baby In Uptrend			(하락기아형)       			
	CDFind_AbandonedBabyInUptrend(		pstCandleData, pObjCandlePatternData);
	// ## 23. Three Indise Up					(상승잉태확인형)   			
	CDFind_ThreeIndiseUp(				pstCandleData, pObjCandlePatternData);
	// ## 24. Three Indise Down					(하락잉태확인형)   			                            		
	CDFind_ThreeIndiseDown(				pstCandleData, pObjCandlePatternData);
	// ## 25. Three Outside Up					(상승장악확인형)   			                            		
	CDFind_ThreeOutsideUp(				pstCandleData, pObjCandlePatternData);
	// ## 26. Three Outside Down				(하락장악확인형)   			                                	
	CDFind_ThreeOutsideDown(			pstCandleData, pObjCandlePatternData);
	// ## 27. Upside Gap Two Crows In Uptrend	(까마귀형)         			
	CDFind_UpsideGapTwoCrowsInUptrend(	pstCandleData, pObjCandlePatternData);
	// ## 28. Three White Soldier				(적삼병)           			
	CDFind_ThreeWhiteSoldier(			pstCandleData, pObjCandlePatternData);
	// ## 29. Three Black Corws					(흑삼병)           			                                	
	CDFind_ThreeBlackCorws(				pstCandleData, pObjCandlePatternData);
	// ## 30. Stalled Pattern					(지연형)           			                            		
	CDFind_StalledPattern(				pstCandleData, pObjCandlePatternData);
	// ## 31. Advance Block						(블락형)           			                            		
	CDFind_AdvanceBlock(				pstCandleData, pObjCandlePatternData);
	// ## 32. Upside Gap Tasuki					(상승타스키갭형)   			                            		
	CDFind_UpsideGapTasuki(				pstCandleData, pObjCandlePatternData);
	// ## 33. Downside Gap Tasuki				(하락타스키갭형)   			                                	
	CDFind_DownsideGapTasuki(			pstCandleData, pObjCandlePatternData);
	// ## 34. Mat-Hold							(매트형)           			
	CDFind_MatHold(						pstCandleData, pObjCandlePatternData);
	// ## 35. Bullish Tri Star					(상승십자별형)     			
	CDFind_Bullish_TriStar(				pstCandleData, pObjCandlePatternData);
	// ## 36. Bearish Tri Star					(하락십자별형)     			                            		
	CDFind_Bearish_TriStar(				pstCandleData, pObjCandlePatternData);
	// ## 37. Unique Three River Bottom			(희귀삼천바닥형)   			                            		
	CDFind_UniqueThreeRiverBottom(		pstCandleData, pObjCandlePatternData);
	// ## 38. Identical Three Corws				(동일흑삼병형)     			
	CDFind_IdenticalThreeCrows(			pstCandleData, pObjCandlePatternData);
	// ## 39. Bullish Breakaway					(상승탈출형)       			                                	
	CDFind_Bullish_Breakaway(			pstCandleData, pObjCandlePatternData);
	// ## 40. Bearish Breakaway					(하락탈출형)       			                                	
	CDFind_Bearish_Breakaway(			pstCandleData, pObjCandlePatternData);
	// ## 41. Two Crows							(약세까마귀형)     			                                	
	CDFind_TwoCrows(					pstCandleData, pObjCandlePatternData);
	// ## 42. Three Stars In The South			(남삼성형)         			
	CDFind_ThreeStarsInTheSouth(		pstCandleData, pObjCandlePatternData);
	// ## 43. Concealing Baby Swallow			(애기제비감추기형) 			
	CDFind_ConcealingBabySwallow(		pstCandleData, pObjCandlePatternData);
	// ## 44. Stick Sandwich					(막대샌드위치형)   			
	CDFind_StickSandwich(				pstCandleData, pObjCandlePatternData);
	// ## 45. Bullish Kicking					(상승박차형)	   			                            		
	CDFind_Bullish_Kicking(				pstCandleData, pObjCandlePatternData);
	// ## 46. Bearish Kicking					(하락박차형)       			                            		
	CDFind_Bearish_Kicking(				pstCandleData, pObjCandlePatternData);
	// ## 47. Homing Pigeon						(전서구형)         			                            		
	CDFind_HomingPigeon(				pstCandleData, pObjCandlePatternData);
	// ## 48. Ladder Bottom						(사다리바닥형)     			                            		
	CDFind_LadderBottom(				pstCandleData, pObjCandlePatternData);
	// ## 49. Matching Low						(동일저점형)       			                            		
	CDFind_MatchingLow(					pstCandleData, pObjCandlePatternData);
	// ## 50. Bullish Side-By-Side White Lines	(상승나란히형)     			
	CDFind_Bullish_SideBySideWhiteLines(pstCandleData, pObjCandlePatternData);
	// ## 51. Bearish Side-By-Side White Lines	(하락나란히형)     			
	CDFind_Bearish_SideBySideWhiteLines(pstCandleData, pObjCandlePatternData);
	// ## 52. Rising Three Methods				(상승삼법형)       			
	CDFind_RisingThreeMethods(			pstCandleData, pObjCandlePatternData);
	// ## 53. Falling Three Methods				(하락삼법형)       			                                	
	CDFind_FallingThreeMethods(			pstCandleData, pObjCandlePatternData);
	// ## 54. Bullish Separating Lines			(상승갈림길형)     			
	CDFind_Bullish_SeparatingLines(		pstCandleData, pObjCandlePatternData);
	// ## 55. Bearish Separating Lines			(하락갈림길형)     			
	CDFind_Bearish_SeparatingLines(		pstCandleData, pObjCandlePatternData);
	// ## 56. Bullish Three-Line Strike			(강세삼선반격형)   			
	CDFind_Bullish_ThreeLineStrike(		pstCandleData, pObjCandlePatternData);
	// ## 57. Bearish Three-Line Strike			(약세삼선반격형)   			
	CDFind_Bearish_ThreeLineStrike(		pstCandleData, pObjCandlePatternData);
	// ## 58. Upside Gap Three Methods			(상승갭삼법형)     			
	CDFind_UpsideGapThreeMethods(		pstCandleData, pObjCandlePatternData);
	// ## 59. Downside Gap Three Methods		(하락갭삼법형)     			
	CDFind_DownsideGapThreeMethods(		pstCandleData, pObjCandlePatternData);
	// ## 60. On Neck							(걸침형)           			
	CDFind_OnNeck(						pstCandleData, pObjCandlePatternData);
	// ## 61. In Neck							(진입형)           			                    				
	CDFind_InNeck(						pstCandleData, pObjCandlePatternData);
	// ## 62. Thrusting Line					(공격형)           			
 	CDFind_ThrustingLine(				pstCandleData, pObjCandlePatternData);
//	// ## 63. Bullish GraveStone				(강세 비석형)
//	CDFind_Bullish_GraveStone(			pstCandleData, pObjCandlePatternData);
//	// ## 64. Bullish DragonFly					(강세 잠자리형)
//	CDFind_Bullish_DragonFly(			pstCandleData, pObjCandlePatternData);
}


// *****************************************************
// Get Candle Pattern Name
// *****************************************************
CString CCandlePattern::GetCandlePatternName(int nCandlePatternType)
{
	ML_SET_LANGUAGE_RES();

	CString strCandlePatternName;
	strCandlePatternName =_T("");

// 2008.02.20 by LYH >>
	// Get Pattern Name
	switch (nCandlePatternType)
	{
	case CPTN_HAMMER:							// Hammer							(망치형)          		
		//strCandlePatternName = STR_HAMMER;							
		strCandlePatternName.LoadString(IDS_HAMMER);
		break;
	case CPTN_HANGINGMAN:						// Hanging Man						(교수형)                                                      
		//strCandlePatternName = STR_HANGINGMAN;						
		strCandlePatternName.LoadString(IDS_HANGINGMAN);						
		break;
	case CPTN_INVERTEDHAMMER:					// Inverted Hammer					(역망치형)                                                    
		//strCandlePatternName = STR_INVERTEDHAMMER;					
		strCandlePatternName.LoadString(IDS_INVERTEDHAMMER);					
		break;
	case CPTN_SHOOTINGSTAR:						// Shooting Star					(유성형)                                                      
		//strCandlePatternName = STR_SHOOTINGSTAR;				
		strCandlePatternName.LoadString(IDS_SHOOTINGSTAR);				
		break;
	case CPTN_BULLISH_BELTHOLDLINE:				// Bullish Belt Hold Line			(상승샅바형)                                                  
		//strCandlePatternName = STR_BULLISH_BELTHOLDLINE;			
		strCandlePatternName.LoadString(IDS_BULLISH_BELTHOLDLINE);			
		break;
	case CPTN_BEARISH_BELTHOLDLINE:				// Bearish Belt Hold Line			(하락샅바형)                                                  
		//strCandlePatternName = STR_BEARISH_BELTHOLDLINE;		
		strCandlePatternName.LoadString(IDS_BEARISH_BELTHOLDLINE);		
		break;
	case CPTN_LONG_WHITEBODY:					// Long White Body					(장대양봉)                                                    
		//strCandlePatternName = STR_LONG_WHITEBODY;					
		strCandlePatternName.LoadString(IDS_LONG_WHITEBODY);					
		break;
	case CPTN_LONG_BLACKBODY:					// Long Black Body					(장대음봉)                                                    
		//strCandlePatternName = STR_LONG_BLACKBODY;					
		strCandlePatternName.LoadString(IDS_LONG_BLACKBODY);					
		break;
	case CPTN_BULLISH_HARAMI:					// Bullish Harami					(상승잉태형)                                                  
		//strCandlePatternName = STR_BULLISH_HARAMI;					
		strCandlePatternName.LoadString(IDS_BULLISH_HARAMI);					
		break;
	case CPTN_BEARISH_HARAMI:					// Bearish Harami					(하락잉태형)                                                  
		//strCandlePatternName = STR_BEARISH_HARAMI;					
		strCandlePatternName.LoadString(IDS_BEARISH_HARAMI);					
		break;
	case CPTN_BULLISH_ENGULFING:				// Bullish Engulfing				(상승장악형)                                                  
		//strCandlePatternName = STR_BULLISH_ENGULFING;				
		strCandlePatternName.LoadString(IDS_BULLISH_ENGULFING);				
		break;
	case CPTN_BEARISH_ENGULFING:				// Bearish Engulfing				(하락장악형)                                                  
		//strCandlePatternName = STR_BEARISH_ENGULFING;				
		strCandlePatternName.LoadString(IDS_BEARISH_ENGULFING);				
		break;
	case CPTN_BULLISH_COUNTERATTACKLINE:		// Bullish Counterattack Line		(상승반격형)                                                  
		//strCandlePatternName = STR_BULLISH_COUNTERATTACKLINE;		
		strCandlePatternName.LoadString(IDS_BULLISH_COUNTERATTACKLINE);		
		break;
	case CPTN_BEARISH_COUNTERATTACKLINE:		// Bearish Counterattack Line		(하락반격형)                                                  
		//strCandlePatternName = STR_BEARISH_COUNTERATTACKLINE;		
		strCandlePatternName.LoadString(IDS_BEARISH_COUNTERATTACKLINE);		
		break;
	case CPTN_PIERCINGPATTERN:					// Piercing Pattern					(관통형)                                                      
		//strCandlePatternName = STR_PIERCINGPATTERN;				    
		strCandlePatternName.LoadString(IDS_PIERCINGPATTERN);				    
		break;
	case CPTN_DARKCLOUDCOVER:					// Dark Cloud Cover					(먹구름형)                                                    
		//strCandlePatternName = STR_DARKCLOUDCOVER;					
		strCandlePatternName.LoadString(IDS_DARKCLOUDCOVER);					
		break;
	case CPTN_MORNINGSTAR:						// Morning Star						(샛별형)                                                      
		//strCandlePatternName = STR_MORNINGSTAR;					    
		strCandlePatternName.LoadString(IDS_MORNINGSTAR);					    
		break;
	case CPTN_EVENINGSTAR:						// Evening Star						(석별형)                                                      
		//strCandlePatternName = STR_EVENINGSTAR;				    
		strCandlePatternName.LoadString(IDS_EVENINGSTAR);				    
		break;
	case CPTN_MORNINTDOJISTAR:					// Morning Doji Star				(십자샛별형)                                                  
		//strCandlePatternName = STR_MORNINTDOJISTAR;				    
		strCandlePatternName.LoadString(IDS_MORNINTDOJISTAR);				    
		break;
	case CPTN_EVENINGDOJISTAR:					// Evening Doji Star				(십자석별형)                                                  
		//strCandlePatternName = STR_EVENINGDOJISTAR;				    
		strCandlePatternName.LoadString(IDS_EVENINGDOJISTAR);				    
		break;
	case CPTN_ABANDONEDBABY_INDOWNTREND:		// Abandoned Baby In Downtrend		(상승기아형)                                                  
		//strCandlePatternName = STR_ABANDONEDBABY_INDOWNTREND;		
		strCandlePatternName.LoadString(IDS_ABANDONEDBABY_INDOWNTREND);		
		break;
	case CPTN_ABANDONEDBABY_INUPTREND:			// Abandoned Baby In Uptrend		(하락기아형)                                                  
		//strCandlePatternName = STR_ABANDONEDBABY_INUPTREND;		    
		strCandlePatternName.LoadString(IDS_ABANDONEDBABY_INUPTREND);		    
		break;
	case CPTN_THREE_INSIDEUP:					// Three Indise Up					(상승잉태확인형)                                              
		//strCandlePatternName = STR_THREE_INSIDEUP;					
		strCandlePatternName.LoadString(IDS_THREE_INSIDEUP);					
		break;
	case CPTN_THREE_INSIDEDOWN:					// Three Indise Down				(하락잉태확인형)                                              
		//strCandlePatternName = STR_THREE_INSIDEDOWN;			
		strCandlePatternName.LoadString(IDS_THREE_INSIDEDOWN);			
		break;
	case CPTN_THREE_OUTSIDEUP:					// Three Outside Up					(상승장악확인형)                                              
		//strCandlePatternName = STR_THREE_OUTSIDEUP;				    
		strCandlePatternName.LoadString(IDS_THREE_OUTSIDEUP);				    
		break;
	case CPTN_THREE_OUTSIDEDOWN:				// Three Outside Down				(하락장악확인형)                                              
		//strCandlePatternName = STR_THREE_OUTSIDEDOWN;				
		strCandlePatternName.LoadString(IDS_THREE_OUTSIDEDOWN);				
		break;
	case CPTN_UPSIDEGAPTWOCROWS_INUPTREND:		// Upside Gap Two Crows In Uptrend	(까마귀형)                                                    
		//strCandlePatternName = STR_UPSIDEGAPTWOCROWS_INUPTREND;	    
		strCandlePatternName.LoadString(IDS_UPSIDEGAPTWOCROWS_INUPTREND);	    
		break;
	case CPTN_THREE_WHITESOLDIER:				// Three White Soldier				(적삼병)                                                      
		//strCandlePatternName = STR_THREE_WHITESOLDIER;				
		strCandlePatternName.LoadString(IDS_THREE_WHITESOLDIER);				
		break;
	case CPTN_THREE_BLACKCROW:					// Three Black Corws				(흑삼병)                                                      
		//strCandlePatternName = STR_THREE_BLACKCROW;				    
		strCandlePatternName.LoadString(IDS_THREE_BLACKCROW);				    
		break;
	case CPTN_STALLEDPATTERN:					// Stalled Pattern					(지연형)                                                      
		//strCandlePatternName = STR_STALLEDPATTERN;					
		strCandlePatternName.LoadString(IDS_STALLEDPATTERN);					
		break;
	case CPTN_ADVANCEBLOCK:						// Advance Block					(블락형)                                                      
		//strCandlePatternName = STR_ADVANCEBLOCK;					
		strCandlePatternName.LoadString(IDS_ADVANCEBLOCK);					
		break;
	case CPTN_UPSIDEGAP_TASUKI:					// Upside Gap Tasuki				(상승타스키갭형)                                              
		//strCandlePatternName = STR_UPSIDEGAP_TASUKI;				
		strCandlePatternName.LoadString(IDS_UPSIDEGAP_TASUKI);				
		break;
	case CPTN_DNSIDEGAP_TASUKI:					// Downside Gap Tasuki				(하락타스키갭형)                                              
		//strCandlePatternName = STR_DNSIDEGAP_TASUKI;				
		strCandlePatternName.LoadString(IDS_DNSIDEGAP_TASUKI);				
		break;
	case CPTN_MATHOLD:							// Mat-Hold							(매트형)                                                      
		//strCandlePatternName = STR_MATHOLD;						    
		strCandlePatternName.LoadString(IDS_MATHOLD);						    
		break;
	case CPTN_BULLISH_TRISTAR:					// Bullish Tri Star					(상승십자별형)                                                
		//strCandlePatternName = STR_BULLISH_TRISTAR;				    
		strCandlePatternName.LoadString(IDS_BULLISH_TRISTAR);				    
		break;
	case CPTN_BEARISH_TRISTAR:					// Bearish Tri Star					(하락십자별형)                                                
		//strCandlePatternName = STR_BEARISH_TRISTAR;				    
		strCandlePatternName.LoadString(IDS_BEARISH_TRISTAR);				    
		break;
	case CPTN_UNIQUETHREERIVERBOTTOM:			// Unique Three River Bottom		(희귀삼천바닥형)                                              
		//strCandlePatternName = STR_UNIQUETHREERIVERBOTTOM;			
		strCandlePatternName.LoadString(IDS_UNIQUETHREERIVERBOTTOM);			
		break;
	case CPTN_IDENTICAL_THREECROWS:				// Identical Three Corws			(동일흑삼병형)                                                
		//strCandlePatternName = STR_IDENTICAL_THREECROWS;			
		strCandlePatternName.LoadString(IDS_IDENTICAL_THREECROWS);			
		break;
	case CPTN_BULLISH_BREAKAWAY:				// Bullish Breakaway				(상승탈출형)                                                  
		//strCandlePatternName = STR_BULLISH_BREAKAWAY;				
		strCandlePatternName.LoadString(IDS_BULLISH_BREAKAWAY);				
		break;
	case CPTN_BEARISH_BREAKAWAY:				// Bearish Breakaway				(하락탈출형)                                                  
		//strCandlePatternName = STR_BEARISH_BREAKAWAY;				
		strCandlePatternName.LoadString(IDS_BEARISH_BREAKAWAY);				
		break;
	case CPTN_TWOCORWS:							// Two Crows						(약세까마귀형)                                                
		//strCandlePatternName = STR_TWOCORWS;						
		strCandlePatternName.LoadString(IDS_TWOCORWS);						
		break;
	case CPTN_THREESTARS_INTHESOUTH:			// Three Stars In The South			(남삼성형)                                                    
		//strCandlePatternName = STR_THREESTARS_INTHESOUTH;			
		strCandlePatternName.LoadString(IDS_THREESTARS_INTHESOUTH);			
		break;
	case CPTN_CONCEALINGBABY_SWALLOW:			// Concealing Baby Swallow			(애기제비감추기형)                                            
		//strCandlePatternName = STR_CONCEALINGBABY_SWALLOW;			
		strCandlePatternName.LoadString(IDS_CONCEALINGBABY_SWALLOW);			
		break;
	case CPTN_STICKSANDWICH:					// Stick Sandwich					(막대샌드위치형)                                              
		//strCandlePatternName = STR_STICKSANDWICH;					
		strCandlePatternName.LoadString(IDS_STICKSANDWICH);					
		break;
	case CPTN_BULLISH_KICKING:					// Bullish Kicking					(상승박차형)	                                              
		//strCandlePatternName = STR_BULLISH_KICKING;				    
		strCandlePatternName.LoadString(IDS_BULLISH_KICKING);				    
		break;
	case CPTN_BEARISH_KICKING:					// Bearish Kicking					(하락박차형)                                                  
		//strCandlePatternName = STR_BEARISH_KICKING;				    
		strCandlePatternName.LoadString(IDS_BEARISH_KICKING);				    
		break;
	case CPTN_HOMINGPIGEON:						// Homing Pigeon					(전서구형)                                                    
		//strCandlePatternName = STR_HOMINGPIGEON;				
		strCandlePatternName.LoadString(IDS_HOMINGPIGEON);				
		break;
	case CPTN_LADDERBOTTOM:						// Ladder Bottom					(사다리바닥형)                                                
		//strCandlePatternName = STR_LADDERBOTTOM;					
		strCandlePatternName.LoadString(IDS_LADDERBOTTOM);					
		break;
	case CPTN_MATCHINGLOW:						// Matching Low						(동일저점형)                                                  
		//strCandlePatternName = STR_MATCHINGLOW;					    
		strCandlePatternName.LoadString(IDS_MATCHINGLOW);					    
		break;
	case CPTN_BULLISH_SIDEBYSIDE_WHITELINE:		// Bullish Side-By-Side White Lines	(상승나란히형)                                                
		//strCandlePatternName = STR_BULLISH_SIDEBYSIDE_WHITELINE;	
		strCandlePatternName.LoadString(IDS_BULLISH_SIDEBYSIDE_WHITELINE);	
		break;
	case CPTN_BEARISH_SIDEBYSIDE_WHITELINE:		// Bearish Side-By-Side White Lines	(하락나란히형)                                                
		//strCandlePatternName = STR_BEARISH_SIDEBYSIDE_WHITELINE;	
		strCandlePatternName.LoadString(IDS_BEARISH_SIDEBYSIDE_WHITELINE);	
		break;
	case CPTN_RISING_THREEMETHODS:				// Rising Three Methods				(상승삼법형)                                                  
		//strCandlePatternName = STR_RISING_THREEMETHODS;			    
		strCandlePatternName.LoadString(IDS_RISING_THREEMETHODS);			    
		break;
	case CPTN_FALLING_THREEMETHODS:				// Falling Three Methods			(하락삼법형)                                                  
		//strCandlePatternName = STR_FALLING_THREEMETHODS;			
		strCandlePatternName.LoadString(IDS_FALLING_THREEMETHODS);			
		break;
	case CPTN_BULLISH_SEPARATINGLINES:			// Bullish Separating Lines			(상승갈림길형)                                                
		//strCandlePatternName = STR_BULLISH_SEPARATINGLINES;		    
		strCandlePatternName.LoadString(IDS_BULLISH_SEPARATINGLINES);		    
		break;
	case CPTN_BEARISH_SEPARATINGLINES:			// Bearish Separating Lines			(하락갈림길형)                                                
		//strCandlePatternName = STR_BEARISH_SEPARATINGLINES;		    
		strCandlePatternName.LoadString(IDS_BEARISH_SEPARATINGLINES);		    
		break;
	case CPTN_BULLISH_THREELINESTRIKE:			// Bullish Three-Line Strike		(강세삼선반격형)                                              
		//strCandlePatternName = STR_BULLISH_THREELINESTRIKE;		    
		strCandlePatternName.LoadString(IDS_BULLISH_THREELINESTRIKE);		    
		break;
	case CPTN_BEARISH_THREELINESTRIKE:			// Bearish Three-Line Strike		(약세삼선반격형)                                              
		//strCandlePatternName = STR_BEARISH_THREELINESTRIKE;		    
		strCandlePatternName.LoadString(IDS_BEARISH_THREELINESTRIKE);		    
		break;
	case CPTN_UPSIDEGAP_THREEMETHODS:			// Upside Gap Three Methods			(상승갭삼법형)                                                
		//strCandlePatternName = STR_UPSIDEGAP_THREEMETHODS;			
		strCandlePatternName.LoadString(IDS_UPSIDEGAP_THREEMETHODS);			
		break;
	case CPTN_DNSIDEGAP_THREEMETHODS:			// Downside Gap Three Methods		(하락갭삼법형)                                                
		//strCandlePatternName = STR_DNSIDEGAP_THREEMETHODS;			
		strCandlePatternName.LoadString(IDS_DNSIDEGAP_THREEMETHODS);			
		break;
	case CPTN_ON_NECK:							// On Neck							(걸침형)                                                      
		//strCandlePatternName = STR_ON_NECK;						    
		strCandlePatternName.LoadString(IDS_ON_NECK);						    
		break;
	case CPTN_IN_NECK:							// In Neck							(진입형)                                                      
		//strCandlePatternName = STR_IN_NECK;						    
		strCandlePatternName.LoadString(IDS_IN_NECK);						    
		break;
	case CPTN_THRUSTINGLINE:					// Thrusting Line					(공격형->Thrusitng Line)                                                      
		strCandlePatternName = STR_THRUSTINGLINE;                   
		break;
		
//	case CPTN_BULLISH_GRAVESTONE:				// Bullish GraveStone				(강세 비석형)
//		strCandlePatternName = STR_BULLISH_GRAVESTONE;
//		break;
//	case CPTN_BULLISH_DRAGONFLY:				// Bullish DragonFly				(강세 잠자리형)
//		strCandlePatternName = STR_BULLISH_DRAGONFLY;
//		break;

	}
// 2008.02.20 by LYH <<
	return	strCandlePatternName;
}


// *****************************************************
// Get Candle Pattern Trend
// *****************************************************
int CCandlePattern::GetCandlePatternTrend(int nCandlePatternType)
{
	int nPatternTrend = 0;

	// Get Pattern Trend
	switch (nCandlePatternType)
	{
	case CPTN_HAMMER:							// Hammer							(망치형)  
	case CPTN_INVERTEDHAMMER:					// Inverted Hammer					(역망치형)  
	case CPTN_BULLISH_BELTHOLDLINE:				// Bullish Belt Hold Line			(상승샅바형)
	case CPTN_BULLISH_HARAMI:					// Bullish Harami					(상승잉태형)
	case CPTN_BULLISH_ENGULFING:				// Bullish Engulfing				(상승장악형) 
	case CPTN_BULLISH_COUNTERATTACKLINE:		// Bullish Counterattack Line		(상승반격형)
	case CPTN_PIERCINGPATTERN:					// Piercing Pattern					(관통형) 
	case CPTN_MORNINGSTAR:						// Morning Star						(샛별형) 
	case CPTN_MORNINTDOJISTAR:					// Morning Doji Star				(십자샛별형) 
	case CPTN_ABANDONEDBABY_INDOWNTREND:		// Abandoned Baby In Downtrend		(상승기아형)
	case CPTN_THREE_INSIDEUP:					// Three Indise Up					(상승잉태확인형)
	case CPTN_THREE_OUTSIDEUP:					// Three Outside Up					(상승장악확인형)
	case CPTN_THREE_WHITESOLDIER:				// Three White Soldier				(적삼병) 	
	case CPTN_BULLISH_TRISTAR:					// Bullish Tri Star					(상승십자별형) 
	case CPTN_UNIQUETHREERIVERBOTTOM:			// Unique Three River Bottom		(희귀삼천바닥형) 
	case CPTN_BULLISH_BREAKAWAY:				// Bullish Breakaway				(상승탈출형) 
	case CPTN_THREESTARS_INTHESOUTH:			// Three Stars In The South			(남삼성형)
	case CPTN_CONCEALINGBABY_SWALLOW:			// Concealing Baby Swallow			(애기제비감추기형) 
	case CPTN_STICKSANDWICH:					// Stick Sandwich					(막대샌드위치형) 
	case CPTN_BULLISH_KICKING:					// Bullish Kicking					(상승박차형)
	case CPTN_HOMINGPIGEON:						// Homing Pigeon					(전서구형) 
	case CPTN_LADDERBOTTOM:						// Ladder Bottom					(사다리바닥형)                                                
	case CPTN_MATCHINGLOW:						// Matching Low						(동일저점형) 
//	case CPTN_BULLISH_GRAVESTONE:				// Bullish GraveStone				(강세 비석형)	
//	case CPTN_BULLISH_DRAGONFLY:				// Bullish DragonFly				(강세 잠자리형)									        		
		nPatternTrend = UPWARD_REVERSE;
		break;
		
	case CPTN_HANGINGMAN:						// Hanging Man						(교수형)	
	case CPTN_SHOOTINGSTAR:						// Shooting Star					(유성형)	
	case CPTN_BEARISH_BELTHOLDLINE:				// Bearish Belt Hold Line			(하락샅바형)
	case CPTN_BEARISH_HARAMI:					// Bearish Harami					(하락잉태형)
	case CPTN_BEARISH_ENGULFING:				// Bearish Engulfing				(하락장악형)
	case CPTN_BEARISH_COUNTERATTACKLINE:		// Bearish Counterattack Line		(하락반격형) 
	case CPTN_DARKCLOUDCOVER:					// Dark Cloud Cover					(먹구름형) 
	case CPTN_EVENINGSTAR:						// Evening Star						(석별형) 
	case CPTN_EVENINGDOJISTAR:					// Evening Doji Star				(십자석별형)
	case CPTN_ABANDONEDBABY_INUPTREND:			// Abandoned Baby In Uptrend		(하락기아형)
	case CPTN_THREE_INSIDEDOWN:					// Three Indise Down				(하락잉태확인형)
	case CPTN_THREE_OUTSIDEDOWN:				// Three Outside Down				(하락장악확인형) 
	case CPTN_UPSIDEGAPTWOCROWS_INUPTREND:		// Upside Gap Two Crows In Uptrend	(까마귀형) 
	case CPTN_THREE_BLACKCROW:					// Three Black Corws				(흑삼병)	
	case CPTN_BEARISH_TRISTAR:					// Bearish Tri Star					(하락십자별형)
	case CPTN_IDENTICAL_THREECROWS:				// Identical Three Corws			(동일흑삼병형) 
	case CPTN_BEARISH_BREAKAWAY:				// Bearish Breakaway				(하락탈출형)  
	case CPTN_TWOCORWS:							// Two Crows						(약세까마귀형) 
	case CPTN_BEARISH_KICKING:					// Bearish Kicking					(하락박차형)					 			
		nPatternTrend = DNWARD_REVERSE;
		break;
		
	case CPTN_LONG_WHITEBODY:					// Long White Body					(장대양봉)
	case CPTN_STALLEDPATTERN:					// Stalled Pattern					(지연형)	
	case CPTN_ADVANCEBLOCK:						// Advance Block					(블락형)
	case CPTN_UPSIDEGAP_TASUKI:					// Upside Gap Tasuki				(상승타스키갭형)
	case CPTN_MATHOLD:							// Mat-Hold							(매트형)
	case CPTN_BULLISH_SIDEBYSIDE_WHITELINE:		// Bullish Side-By-Side White Lines	(상승나란히형) 	
	case CPTN_RISING_THREEMETHODS:				// Rising Three Methods				(상승삼법형)  
	case CPTN_BULLISH_SEPARATINGLINES:			// Bullish Separating Lines			(상승갈림길형)
	case CPTN_BULLISH_THREELINESTRIKE:			// Bullish Three-Line Strike		(강세삼선반격형)
	case CPTN_UPSIDEGAP_THREEMETHODS:			// Upside Gap Three Methods			(상승갭삼법형) 
		nPatternTrend = UPWARD_CONTINUE;
		break;
		
	case CPTN_LONG_BLACKBODY:					// Long Black Body					(장대음봉) 
	case CPTN_DNSIDEGAP_TASUKI:					// Downside Gap Tasuki				(하락타스키갭형)
	case CPTN_BEARISH_SIDEBYSIDE_WHITELINE:		// Bearish Side-By-Side White Lines	(하락나란히형) 
	case CPTN_FALLING_THREEMETHODS:				// Falling Three Methods			(하락삼법형)  
	case CPTN_BEARISH_SEPARATINGLINES:			// Bearish Separating Lines			(하락갈림길형) 
	case CPTN_BEARISH_THREELINESTRIKE:			// Bearish Three-Line Strike		(약세삼선반격형) 
	case CPTN_DNSIDEGAP_THREEMETHODS:			// Downside Gap Three Methods		(하락갭삼법형)
	case CPTN_ON_NECK:							// On Neck							(걸침형)  
	case CPTN_IN_NECK:							// In Neck							(진입형)  
	case CPTN_THRUSTINGLINE:					// Thrusting Line					(공격형)  	
		nPatternTrend = DNWARD_CONTINUE;
		break;						
	}

	return	nPatternTrend;
}


// *****************************************************
// Get Candle Pattern Trend Name
// *****************************************************
CString CCandlePattern::GetCandlePatternTrendName(int nCandlePatternType)
{
	int nPatternTrend = GetCandlePatternTrend( nCandlePatternType);

	CString strRtnName;
	strRtnName =_T("");

	// Get Pattern Trend
// 2008.02.20 by LYH >>
	switch (nPatternTrend)
	{
	case UPWARD_REVERSE:
		//strRtnName = STR_UPWARD_REVERSE;
		strRtnName.LoadString(IDS_UPWARD_REVERSE);
		break;

	case DNWARD_REVERSE:
		//strRtnName = STR_DNWARD_REVERSE;
		strRtnName.LoadString(IDS_DOWNWARD_REVERSE);
		break;

	case UPWARD_CONTINUE:
		//strRtnName = STR_UPWARD_CONTINUE;
		strRtnName.LoadString(IDS_UPWARD_CONTINUE);
		break;

	case DNWARD_CONTINUE:
		//strRtnName = STR_DNWARD_CONTINUE;
		strRtnName.LoadString(IDS_DOWNWARD_CONTINUE);
		break;
	}
// 2008.02.20 by LYH <<
	return strRtnName;
}

// *****************************************************
// Get Candle Pattern Trust
// *****************************************************
int CCandlePattern::GetCandlePatternTrust(int nCandlePatternType)
{
	int nPatternLevel = 0;

	// Get Pattern Trend
	switch (nCandlePatternType)
	{
	case CPTN_HAMMER:							// Hammer							(망치형)  
	case CPTN_MORNINGSTAR:						// Morning Star						(샛별형) 
	case CPTN_MORNINTDOJISTAR:					// Morning Doji Star				(십자샛별형) 
	case CPTN_THREE_INSIDEUP:					// Three Indise Up					(상승잉태확인형)
	case CPTN_THREE_OUTSIDEUP:					// Three Outside Up					(상승장악확인형)
	case CPTN_CONCEALINGBABY_SWALLOW:			// Concealing Baby Swallow			(애기제비감추기형) 
	case CPTN_HOMINGPIGEON:						// Homing Pigeon					(전서구형) 
		nPatternLevel = UPWARD_LEVEL3;
		break;
	case CPTN_INVERTEDHAMMER:					// Inverted Hammer					(역망치형)  
	case CPTN_BULLISH_BELTHOLDLINE:				// Bullish Belt Hold Line			(상승샅바형)
	case CPTN_LONG_WHITEBODY:					// Long White Body					(장대양봉)
	case CPTN_BULLISH_HARAMI:					// Bullish Harami					(상승잉태형)
	case CPTN_BULLISH_ENGULFING:				// Bullish Engulfing				(상승장악형) 
	case CPTN_BULLISH_COUNTERATTACKLINE:		// Bullish Counterattack Line		(상승반격형)
	case CPTN_THREE_WHITESOLDIER:				// Three White Soldier				(적삼병) 	
	case CPTN_STALLEDPATTERN:					// Stalled Pattern					(지연형)	
	case CPTN_MATHOLD:							// Mat-Hold							(매트형)
	case CPTN_BULLISH_TRISTAR:					// Bullish Tri Star					(상승십자별형) 
	case CPTN_UNIQUETHREERIVERBOTTOM:			// Unique Three River Bottom		(희귀삼천바닥형) 
	case CPTN_LADDERBOTTOM:						// Ladder Bottom					(사다리바닥형)                                                
	case CPTN_MATCHINGLOW:						// Matching Low						(동일저점형) 
	case CPTN_BULLISH_SIDEBYSIDE_WHITELINE:		// Bullish Side-By-Side White Lines	(상승나란히형) 	
	case CPTN_BULLISH_THREELINESTRIKE:			// Bullish Three-Line Strike		(강세삼선반격형)
	case CPTN_UPSIDEGAP_THREEMETHODS:			// Upside Gap Three Methods			(상승갭삼법형) 
	case CPTN_ADVANCEBLOCK:						// Advance Block					(블락형)
		nPatternLevel = UPWARD_LEVEL2;
		break;
	case CPTN_PIERCINGPATTERN:					// Piercing Pattern					(관통형) 
	case CPTN_ABANDONEDBABY_INDOWNTREND:		// Abandoned Baby In Downtrend		(상승기아형)
	case CPTN_UPSIDEGAP_TASUKI:					// Upside Gap Tasuki				(상승타스키갭형)
	case CPTN_BULLISH_BREAKAWAY:				// Bullish Breakaway				(상승탈출형) 
	case CPTN_THREESTARS_INTHESOUTH:			// Three Stars In The South			(남삼성형)
	case CPTN_STICKSANDWICH:					// Stick Sandwich					(막대샌드위치형) 
	case CPTN_BULLISH_KICKING:					// Bullish Kicking					(상승박차형)
	case CPTN_RISING_THREEMETHODS:				// Rising Three Methods				(상승삼법형)  
	case CPTN_BULLISH_SEPARATINGLINES:			// Bullish Separating Lines			(상승갈림길형)
//	case CPTN_BULLISH_GRAVESTONE:				// Bullish GraveStone				(강세 비석형)	
//	case CPTN_BULLISH_DRAGONFLY:				// Bullish DragonFly				(강세 잠자리형)									        		
		nPatternLevel = UPWARD_LEVEL1;
		break;
	case CPTN_HANGINGMAN:						// Hanging Man						(교수형)	
	case CPTN_BEARISH_ENGULFING:				// Bearish Engulfing				(하락장악형)
	case CPTN_EVENINGSTAR:						// Evening Star						(석별형) 
	case CPTN_EVENINGDOJISTAR:					// Evening Doji Star				(십자석별형)
	case CPTN_THREE_INSIDEDOWN:					// Three Indise Down				(하락잉태확인형)
	case CPTN_THREE_OUTSIDEDOWN:				// Three Outside Down				(하락장악확인형) 
	case CPTN_IDENTICAL_THREECROWS:				// Identical Three Corws			(동일흑삼병형) 
		nPatternLevel = DNWARD_LEVEL3;
		break;						
	case CPTN_SHOOTINGSTAR:						// Shooting Star					(유성형)	
	case CPTN_BEARISH_BELTHOLDLINE:				// Bearish Belt Hold Line			(하락샅바형)
	case CPTN_LONG_BLACKBODY:					// Long Black Body					(장대음봉) 
	case CPTN_BEARISH_HARAMI:					// Bearish Harami					(하락잉태형)
	case CPTN_BEARISH_COUNTERATTACKLINE:		// Bearish Counterattack Line		(하락반격형) 
	case CPTN_UPSIDEGAPTWOCROWS_INUPTREND:		// Upside Gap Two Crows In Uptrend	(까마귀형) 
	case CPTN_THREE_BLACKCROW:					// Three Black Corws				(흑삼병)	
	case CPTN_BEARISH_TRISTAR:					// Bearish Tri Star					(하락십자별형)
	case CPTN_DNSIDEGAP_THREEMETHODS:			// Downside Gap Three Methods		(하락갭삼법형)
	case CPTN_TWOCORWS:							// Two Crows						(약세까마귀형) 
		nPatternLevel = DNWARD_LEVEL2;
		break;						
	case CPTN_DARKCLOUDCOVER:					// Dark Cloud Cover					(먹구름형) 
	case CPTN_ABANDONEDBABY_INUPTREND:			// Abandoned Baby In Uptrend		(하락기아형)
	case CPTN_BEARISH_BREAKAWAY:				// Bearish Breakaway				(하락탈출형)  
	case CPTN_DNSIDEGAP_TASUKI:					// Downside Gap Tasuki				(하락타스키갭형)
	case CPTN_BEARISH_KICKING:					// Bearish Kicking					(하락박차형)	
	case CPTN_BEARISH_SIDEBYSIDE_WHITELINE:		// Bearish Side-By-Side White Lines	(하락나란히형) 
	case CPTN_FALLING_THREEMETHODS:				// Falling Three Methods			(하락삼법형)  
	case CPTN_BEARISH_SEPARATINGLINES:			// Bearish Separating Lines			(하락갈림길형) 
	case CPTN_BEARISH_THREELINESTRIKE:			// Bearish Three-Line Strike		(약세삼선반격형) 
	case CPTN_ON_NECK:							// On Neck							(걸침형)  
	case CPTN_IN_NECK:							// In Neck							(진입형)  
	case CPTN_THRUSTINGLINE:					// Thrusting Line					(공격형)  	
		nPatternLevel = DNWARD_LEVEL1;
		break;						
	}

	return	nPatternLevel;
}

// *****************************************************
// Get Candle Pattern Help Name
// *****************************************************
CString CCandlePattern::GetCandlePatternHelpName(int nCandlePatternType)
{
	CString strCandlePatternName;
	strCandlePatternName =_T("");

	// Get Pattern Name
	switch (nCandlePatternType)
	{
	case CPTN_HAMMER:							// Hammer							(망치형)          		
		strCandlePatternName = STR_HTML_HAMMER;							
		break;
	case CPTN_HANGINGMAN:						// Hanging Man						(교수형)                                                      
		strCandlePatternName = STR_HTML_HANGINGMAN;						
		break;
	case CPTN_INVERTEDHAMMER:					// Inverted Hammer					(역망치형)                                                    
		strCandlePatternName = STR_HTML_INVERTEDHAMMER;					
		break;
	case CPTN_SHOOTINGSTAR:						// Shooting Star					(유성형)                                                      
		strCandlePatternName = STR_HTML_SHOOTINGSTAR;				
		break;
	case CPTN_BULLISH_BELTHOLDLINE:				// Bullish Belt Hold Line			(상승샅바형)                                                  
		strCandlePatternName = STR_HTML_BULLISH_BELTHOLDLINE;			
		break;
	case CPTN_BEARISH_BELTHOLDLINE:				// Bearish Belt Hold Line			(하락샅바형)                                                  
		strCandlePatternName = STR_HTML_BEARISH_BELTHOLDLINE;		
		break;
	case CPTN_LONG_WHITEBODY:					// Long White Body					(장대양봉)                                                    
		strCandlePatternName = STR_HTML_LONG_WHITEBODY;					
		break;
	case CPTN_LONG_BLACKBODY:					// Long Black Body					(장대음봉)                                                    
		strCandlePatternName = STR_HTML_LONG_BLACKBODY;					
		break;
	case CPTN_BULLISH_HARAMI:					// Bullish Harami					(상승잉태형)                                                  
		strCandlePatternName = STR_HTML_BULLISH_HARAMI;					
		break;
	case CPTN_BEARISH_HARAMI:					// Bearish Harami					(하락잉태형)                                                  
		strCandlePatternName = STR_HTML_BEARISH_HARAMI;					
		break;
	case CPTN_BULLISH_ENGULFING:				// Bullish Engulfing				(상승장악형)                                                  
		strCandlePatternName = STR_HTML_BULLISH_ENGULFING;				
		break;
	case CPTN_BEARISH_ENGULFING:				// Bearish Engulfing				(하락장악형)                                                  
		strCandlePatternName = STR_HTML_BEARISH_ENGULFING;				
		break;
	case CPTN_BULLISH_COUNTERATTACKLINE:		// Bullish Counterattack Line		(상승반격형)                                                  
		strCandlePatternName = STR_HTML_BULLISH_COUNTERATTACKLINE;		
		break;
	case CPTN_BEARISH_COUNTERATTACKLINE:		// Bearish Counterattack Line		(하락반격형)                                                  
		strCandlePatternName = STR_HTML_BEARISH_COUNTERATTACKLINE;		
		break;
	case CPTN_PIERCINGPATTERN:					// Piercing Pattern					(관통형)                                                      
		strCandlePatternName = STR_HTML_PIERCINGPATTERN;				    
		break;
	case CPTN_DARKCLOUDCOVER:					// Dark Cloud Cover					(먹구름형)                                                    
		strCandlePatternName = STR_HTML_DARKCLOUDCOVER;					
		break;
	case CPTN_MORNINGSTAR:						// Morning Star						(샛별형)                                                      
		strCandlePatternName = STR_HTML_MORNINGSTAR;					    
		break;
	case CPTN_EVENINGSTAR:						// Evening Star						(석별형)                                                      
		strCandlePatternName = STR_HTML_EVENINGSTAR;				    
		break;
	case CPTN_MORNINTDOJISTAR:					// Morning Doji Star				(십자샛별형)                                                  
		strCandlePatternName = STR_HTML_MORNINTDOJISTAR;				    
		break;
	case CPTN_EVENINGDOJISTAR:					// Evening Doji Star				(십자석별형)                                                  
		strCandlePatternName = STR_HTML_EVENINGDOJISTAR;				    
		break;
	case CPTN_ABANDONEDBABY_INDOWNTREND:		// Abandoned Baby In Downtrend		(상승기아형)                                                  
		strCandlePatternName = STR_HTML_ABANDONEDBABY_INDOWNTREND;		
		break;
	case CPTN_ABANDONEDBABY_INUPTREND:			// Abandoned Baby In Uptrend		(하락기아형)                                                  
		strCandlePatternName = STR_HTML_ABANDONEDBABY_INUPTREND;		    
		break;
	case CPTN_THREE_INSIDEUP:					// Three Indise Up					(상승잉태확인형)                                              
		strCandlePatternName = STR_HTML_THREE_INSIDEUP;					
		break;
	case CPTN_THREE_INSIDEDOWN:					// Three Indise Down				(하락잉태확인형)                                              
		strCandlePatternName = STR_HTML_THREE_INSIDEDOWN;			
		break;
	case CPTN_THREE_OUTSIDEUP:					// Three Outside Up					(상승장악확인형)                                              
		strCandlePatternName = STR_HTML_THREE_OUTSIDEUP;				    
		break;
	case CPTN_THREE_OUTSIDEDOWN:				// Three Outside Down				(하락장악확인형)                                              
		strCandlePatternName = STR_HTML_THREE_OUTSIDEDOWN;				
		break;
	case CPTN_UPSIDEGAPTWOCROWS_INUPTREND:		// Upside Gap Two Crows In Uptrend	(까마귀형)                                                    
		strCandlePatternName = STR_HTML_UPSIDEGAPTWOCROWS_INUPTREND;	    
		break;
	case CPTN_THREE_WHITESOLDIER:				// Three White Soldier				(적삼병)                                                      
		strCandlePatternName = STR_HTML_THREE_WHITESOLDIER;				
		break;
	case CPTN_THREE_BLACKCROW:					// Three Black Corws				(흑삼병)                                                      
		strCandlePatternName = STR_HTML_THREE_BLACKCROW;				    
		break;
	case CPTN_STALLEDPATTERN:					// Stalled Pattern					(지연형)                                                      
		strCandlePatternName = STR_HTML_STALLEDPATTERN;					
		break;
	case CPTN_ADVANCEBLOCK:						// Advance Block					(블락형)                                                      
		strCandlePatternName = STR_HTML_ADVANCEBLOCK;					
		break;
	case CPTN_UPSIDEGAP_TASUKI:					// Upside Gap Tasuki				(상승타스키갭형)                                              
		strCandlePatternName = STR_HTML_UPSIDEGAP_TASUKI;				
		break;
	case CPTN_DNSIDEGAP_TASUKI:					// Downside Gap Tasuki				(하락타스키갭형)                                              
		strCandlePatternName = STR_HTML_DNSIDEGAP_TASUKI;				
		break;
	case CPTN_MATHOLD:							// Mat-Hold							(매트형)                                                      
		strCandlePatternName = STR_HTML_MATHOLD;						    
		break;
	case CPTN_BULLISH_TRISTAR:					// Bullish Tri Star					(상승십자별형)                                                
		strCandlePatternName = STR_HTML_BULLISH_TRISTAR;				    
		break;
	case CPTN_BEARISH_TRISTAR:					// Bearish Tri Star					(하락십자별형)                                                
		strCandlePatternName = STR_HTML_BEARISH_TRISTAR;				    
		break;
	case CPTN_UNIQUETHREERIVERBOTTOM:			// Unique Three River Bottom		(희귀삼천바닥형)                                              
		strCandlePatternName = STR_HTML_UNIQUETHREERIVERBOTTOM;			
		break;
	case CPTN_IDENTICAL_THREECROWS:				// Identical Three Corws			(동일흑삼병형)                                                
		strCandlePatternName = STR_HTML_IDENTICAL_THREECROWS;			
		break;
	case CPTN_BULLISH_BREAKAWAY:				// Bullish Breakaway				(상승탈출형)                                                  
		strCandlePatternName = STR_HTML_BULLISH_BREAKAWAY;				
		break;
	case CPTN_BEARISH_BREAKAWAY:				// Bearish Breakaway				(하락탈출형)                                                  
		strCandlePatternName = STR_HTML_BEARISH_BREAKAWAY;				
		break;
	case CPTN_TWOCORWS:							// Two Crows						(약세까마귀형)                                                
		strCandlePatternName = STR_HTML_TWOCORWS;						
		break;
	case CPTN_THREESTARS_INTHESOUTH:			// Three Stars In The South			(남삼성형)                                                    
		strCandlePatternName = STR_HTML_THREESTARS_INTHESOUTH;			
		break;
	case CPTN_CONCEALINGBABY_SWALLOW:			// Concealing Baby Swallow			(애기제비감추기형)                                            
		strCandlePatternName = STR_HTML_CONCEALINGBABY_SWALLOW;			
		break;
	case CPTN_STICKSANDWICH:					// Stick Sandwich					(막대샌드위치형)                                              
		strCandlePatternName = STR_HTML_STICKSANDWICH;					
		break;
	case CPTN_BULLISH_KICKING:					// Bullish Kicking					(상승박차형)	                                              
		strCandlePatternName = STR_HTML_BULLISH_KICKING;				    
		break;
	case CPTN_BEARISH_KICKING:					// Bearish Kicking					(하락박차형)                                                  
		strCandlePatternName = STR_HTML_BEARISH_KICKING;				    
		break;
	case CPTN_HOMINGPIGEON:						// Homing Pigeon					(전서구형)                                                    
		strCandlePatternName = STR_HTML_HOMINGPIGEON;				
		break;
	case CPTN_LADDERBOTTOM:						// Ladder Bottom					(사다리바닥형)                                                
		strCandlePatternName = STR_HTML_LADDERBOTTOM;					
		break;
	case CPTN_MATCHINGLOW:						// Matching Low						(동일저점형)                                                  
		strCandlePatternName = STR_HTML_MATCHINGLOW;					    
		break;
	case CPTN_BULLISH_SIDEBYSIDE_WHITELINE:		// Bullish Side-By-Side White Lines	(상승나란히형)                                                
		strCandlePatternName = STR_HTML_BULLISH_SIDEBYSIDE_WHITELINE;	
		break;
	case CPTN_BEARISH_SIDEBYSIDE_WHITELINE:		// Bearish Side-By-Side White Lines	(하락나란히형)                                                
		strCandlePatternName = STR_HTML_BEARISH_SIDEBYSIDE_WHITELINE;	
		break;
	case CPTN_RISING_THREEMETHODS:				// Rising Three Methods				(상승삼법형)                                                  
		strCandlePatternName = STR_HTML_RISING_THREEMETHODS;			    
		break;
	case CPTN_FALLING_THREEMETHODS:				// Falling Three Methods			(하락삼법형)                                                  
		strCandlePatternName = STR_HTML_FALLING_THREEMETHODS;			
		break;
	case CPTN_BULLISH_SEPARATINGLINES:			// Bullish Separating Lines			(상승갈림길형)                                                
		strCandlePatternName = STR_HTML_BULLISH_SEPARATINGLINES;		    
		break;
	case CPTN_BEARISH_SEPARATINGLINES:			// Bearish Separating Lines			(하락갈림길형)                                                
		strCandlePatternName = STR_HTML_BEARISH_SEPARATINGLINES;		    
		break;
	case CPTN_BULLISH_THREELINESTRIKE:			// Bullish Three-Line Strike		(강세삼선반격형)                                              
		strCandlePatternName = STR_HTML_BULLISH_THREELINESTRIKE;		    
		break;
	case CPTN_BEARISH_THREELINESTRIKE:			// Bearish Three-Line Strike		(약세삼선반격형)                                              
		strCandlePatternName = STR_HTML_BEARISH_THREELINESTRIKE;		    
		break;
	case CPTN_UPSIDEGAP_THREEMETHODS:			// Upside Gap Three Methods			(상승갭삼법형)                                                
		strCandlePatternName = STR_HTML_UPSIDEGAP_THREEMETHODS;			
		break;
	case CPTN_DNSIDEGAP_THREEMETHODS:			// Downside Gap Three Methods		(하락갭삼법형)                                                
		strCandlePatternName = STR_HTML_DNSIDEGAP_THREEMETHODS;			
		break;
	case CPTN_ON_NECK:							// On Neck							(걸침형)                                                      
		strCandlePatternName = STR_HTML_ON_NECK;						    
		break;
	case CPTN_IN_NECK:							// In Neck							(진입형)                                                      
		strCandlePatternName = STR_HTML_IN_NECK;						    
		break;
	case CPTN_THRUSTINGLINE:					// Thrusting Line					(공격형->Thrusitng Line)                                                      
		strCandlePatternName = STR_HTML_THRUSTINGLINE;                   
		break;
	}

	return	strCandlePatternName;
}