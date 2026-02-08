// WavePattern.cpp : implementation file
//
// #######################################################################################
// Last Reference Document of a Wave Pattern Logic
//		- Apply Company : ---
//		- Date			: ---
//		- Method		: ---
//---------------------------------------------------------------------------------------
// Author	: Jeong SeoJeong
// Add Date : 2005/02/12 ~ 2005/03/05
// #######################################################################################


#include "stdafx.h"
#include "WavePattern.h"

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
// CWavePattern

CWavePattern::CWavePattern()
{
// 2008.02.20 by LYH >>
	m_hOcxWnd = NULL;
// 2008.02.20 by LYH <<
}

CWavePattern::~CWavePattern()
{
}


// **********************************************
// Get Regist & Support Line 
// Note : dNextPtAllowableError -> 다음 주요점과의 허용오차(Default:1%)
// **********************************************
BOOL CWavePattern::Get_RegistNSupportLine(	CObject *pObjHighLowPtData, CObject *pObjRegistNSupportData, double dNextPtAllowableError)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData			= (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;
	CArrayObjPt(CRegistNSupportData) paryRegistNSupportData = (CArrayObjPt(CRegistNSupportData))pObjRegistNSupportData;

	CHighLowPtData *pHighLowPtData, *pHighLowPtTemp;
	int		nHigh, nLow, nPointCnt;
	
	// 주요 고점저항선, 저점지지선을 구한다
	CRegistNSupportData *pRegistNSupportData = NULL;
	int		nSeq1, nSeq2;
	double	dYFirst = 0, dYSecond = 0, dValTemp = 0;
	double	dPermitHigh, dPermitLow;				// 허용범위 상하
	double	dGradient;								// 기울기
	BOOL	bFirstGradient, bSatisfyCond;			// 첫번째기울기, 저항지지 조건만족
	double	dXNext=0, dYNext=0;						// 좌표
	
	pHighLowPtData = paryHighLowPtData->GetAt(0);
	if ( pHighLowPtData ) 
	{
		if ( pHighLowPtData->m_nPointType == CHANGINGPT_HIGHTOLOW)
		{
			nHigh = 0;
			nLow  = 1;	
		}
		else
		{
			nHigh = 1;
			nLow  = 0;
		}
	}
	else
	{
		nHigh = 0;
		nLow  = 0;
	}
	
	nPointCnt = paryHighLowPtData->GetSize();
	int i = 0,k = 0;

	// 고점 저항선 -----------------------------------------------------------------(1)
	for( i = nHigh; i < nPointCnt; i+=2)
	{
		pHighLowPtData = paryHighLowPtData->GetAt(i);
		if ( !pHighLowPtData ) continue;
		
		if ( pHighLowPtData->m_nPointType == CHANGINGPT_HIGHTOLOW)
		{
			nSeq1	= pHighLowPtData->m_nDataSeq;
			dYFirst	= pHighLowPtData->m_dHigh;
			bFirstGradient	= TRUE;
			bSatisfyCond	= FALSE;
			
			for ( k=i+2; k < nPointCnt; k+=2)		// 다음 고점
			{
				pHighLowPtTemp = paryHighLowPtData->GetAt(k);
				if ( !pHighLowPtTemp) continue;
				
				if ( pHighLowPtTemp->m_nPointType == CHANGINGPT_HIGHTOLOW)
				{
					dValTemp = pHighLowPtTemp->m_dHigh;
					
					if ( bFirstGradient)	// 첫번째기울기
					{
						nSeq2	 = pHighLowPtTemp->m_nDataSeq;
						dYSecond = dValTemp;
						bFirstGradient = FALSE;
						
						// 기울기(Gradient)를 구한다.
						dGradient = GetGradient(nSeq1, dYFirst, nSeq2, dYSecond);
					}
					else
					{
						// Point2 Temp
						dXNext = pHighLowPtTemp->m_nDataSeq;
						dYNext = dGradient * (dXNext - nSeq1) + dYFirst;
						
						dPermitHigh = dYNext + dYNext * dNextPtAllowableError / 100;
						dPermitLow	= dYNext - dYNext * dNextPtAllowableError / 100;
						
						if (dValTemp <= dPermitHigh && dValTemp >= dPermitLow)
						{
							nSeq2	= pHighLowPtTemp->m_nDataSeq;
							bSatisfyCond = TRUE;
							
							if ( dValTemp > dYNext)
							{
								dYSecond = dValTemp;
								
								// 기울기(Gradient)를 구한다.
								dGradient = GetGradient(nSeq1, dYFirst, nSeq2, dYSecond);
							}
							else
							{
								dYSecond = dYNext;
							}
						}
						else
						{
							i = k-4;
							break;
						}
					}
				}
			} // for-end
			
			if ( bSatisfyCond)
			{
				pRegistNSupportData = new CRegistNSupportData;
				pRegistNSupportData->m_nRSLineType	= LINE_HIGHREGIST;
				pRegistNSupportData->m_nDataSeq1	= nSeq1;		// 첫번째데이터
				pRegistNSupportData->m_dYPoint1		= dYFirst;
				pRegistNSupportData->m_nDataSeq2	= nSeq2;		// 두번째데이터
				pRegistNSupportData->m_dYPoint2		= dYSecond;
				paryRegistNSupportData->Add(pRegistNSupportData);
				
				i = k-4;
			}
		}
	} // for-end
	// -----------------------------------------------------------------------------(1)
	
	
	// 저점 지지선 -----------------------------------------------------------------(2)
	for( i = nLow; i < nPointCnt; i+=2)
	{
		pHighLowPtData = paryHighLowPtData->GetAt(i);
		if ( !pHighLowPtData ) continue;
		
		if ( pHighLowPtData->m_nPointType == CHANGINGPT_LOWTOHIGH)
		{
			nSeq1	= pHighLowPtData->m_nDataSeq;
			dYFirst	= pHighLowPtData->m_dLow;
			bFirstGradient	= TRUE;
			bSatisfyCond	= FALSE;
			
			for ( k=i+2; k < nPointCnt; k+=2)		// 다음 고점
			{
				pHighLowPtTemp = paryHighLowPtData->GetAt(k);
				if ( !pHighLowPtTemp) continue;
				
				if ( pHighLowPtTemp->m_nPointType == CHANGINGPT_LOWTOHIGH)
				{
					dValTemp = pHighLowPtTemp->m_dLow;
					
					if ( bFirstGradient)	// 첫번째기울기
					{
						nSeq2	 = pHighLowPtTemp->m_nDataSeq;
						dYSecond = dValTemp;
						bFirstGradient = FALSE;
						
						// 기울기(Gradient)를 구한다.
						dGradient = GetGradient(nSeq1, dYFirst, nSeq2, dYSecond);
					}
					else
					{
						// Point2 Temp
						dXNext = pHighLowPtTemp->m_nDataSeq;
						dYNext = dGradient * (dXNext - nSeq1) + dYFirst;
						
						dPermitHigh = dYNext + dYNext * dNextPtAllowableError / 100;
						dPermitLow	= dYNext - dYNext * dNextPtAllowableError / 100;
						
						if (dValTemp <= dPermitHigh && dValTemp >= dPermitLow)
						{
							nSeq2	= pHighLowPtTemp->m_nDataSeq;
							bSatisfyCond = TRUE;
							
							if ( dValTemp < dYNext)
							{
								dYSecond = dValTemp;
								
								// 기울기(Gradient)를 구한다.
								dGradient = GetGradient(nSeq1, dYFirst, nSeq2, dYSecond);
							}
							else
							{
								dYSecond = dYNext;
							}
						}
						else
						{
							i = k-4;
							break;
						}
					}
				}
			} // for-end
			
			if ( bSatisfyCond)
			{
				pRegistNSupportData = new CRegistNSupportData;
				pRegistNSupportData->m_nRSLineType	= LINE_LOWSUPPORT;
				pRegistNSupportData->m_nDataSeq1	= nSeq1;		// 첫번째데이터
				pRegistNSupportData->m_dYPoint1		= dYFirst;
				pRegistNSupportData->m_nDataSeq2	= nSeq2;		// 두번째데이터
				pRegistNSupportData->m_dYPoint2		= dYSecond;
				paryRegistNSupportData->Add(pRegistNSupportData);
				
				i = k-4;
			}
		}
	} // for-end
	// -----------------------------------------------------------------------------(2)

	return TRUE;
}


// **********************************************
// Header & Shoulder
// **********************************************
BOOL CWavePattern::Find_HeaderNShoulders( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6, *pHighLowPtData7;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = pHighLowPtData7 = NULL;
	int		nPointCnt;
	int		nMininumData = 7;	// Head & Shoulder를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;
		pHighLowPtData7 = paryHighLowPtData->GetAt(i+6);	if ( !pHighLowPtData7 ) continue;

		if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData5->m_dLow > pHighLowPtData7->m_dLow))
		{
			if ( (pHighLowPtData2->m_dHigh < pHighLowPtData4->m_dHigh) && (pHighLowPtData4->m_dHigh > pHighLowPtData6->m_dHigh))
			{
				dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
												pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
				
				dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5
				
				dGradient3 = GetModifyGradient( pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow, 
												pHighLowPtData7->m_nDataSeq, pHighLowPtData7->m_dLow);	// 5-7

				dGradient4 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 2-6

				if ( dGradient1 > dGradient2 && dGradient2 > dGradient3)
				{
					if ( fabs(dGradient2) <= 0.3 && fabs(dGradient4) <= 0.3)
					{
						double dGradA, dGradB;
						double dYDrawPt1_1, dYDrawPt1_2;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt2;
						
						dGradA = GetGradient(	pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh, 
												pHighLowPtData7->m_nDataSeq, pHighLowPtData7->m_dLow);	// 6-7
						
						dGradB = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

						// First Draw Data
						dYDrawPt1_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
						dYDrawPt1_2 = dGradB * (pHighLowPtData7->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

						// Second Neck Data
						if ( (dGradA - dGradB) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData3->m_dLow - pHighLowPtData6->m_dHigh) + 
											dGradA*pHighLowPtData6->m_nDataSeq - dGradB*pHighLowPtData3->m_nDataSeq ) / ( dGradA-dGradB);
							dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData6->m_nDataSeq) + pHighLowPtData6->m_dHigh;
						}

						// Neck Base Data2
						dNeckBaseYPt2 = dGradB * (pHighLowPtData4->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

						// Next HighLow Changing Point
						double dNextChangingYPt = ZERO;
						if ( pHighLowPtData7)
						{
							if ( pHighLowPtData7->m_nPointType == CHANGINGPT_HIGHTOLOW)
								dNextChangingYPt = pHighLowPtData7->m_dHigh;
							else
								dNextChangingYPt = pHighLowPtData7->m_dLow;
						}


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_HEADNSHOULDER,				pHighLowPtData1->m_nDataSeq,	pHighLowPtData7->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData7->m_nDataSeq,	dYDrawPt1_2,
												ZERO,							ZERO,							ZERO,							ZERO,
												pHighLowPtData4->m_nDataSeq,	pHighLowPtData4->m_dHigh,		dXNeckPt2,						dYNeckPt2,
												pHighLowPtData4->m_dHigh,		dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}

// **********************************************
// Reversal Header & Shoulder
// **********************************************
BOOL CWavePattern::Find_R_HeaderNShoulders( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6, *pHighLowPtData7;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = pHighLowPtData7 = NULL;
	int		nPointCnt;
	int		nMininumData = 7;	// Reversal Head & Shoulder를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;
		pHighLowPtData7 = paryHighLowPtData->GetAt(i+6);	if ( !pHighLowPtData7 ) continue;

		if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData5->m_dHigh < pHighLowPtData7->m_dHigh))
		{
			if ( (pHighLowPtData2->m_dLow > pHighLowPtData4->m_dLow) && (pHighLowPtData4->m_dLow < pHighLowPtData6->m_dLow))
			{
				dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
												pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
				
				dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
				
				dGradient3 = GetModifyGradient( pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh, 
												pHighLowPtData7->m_nDataSeq, pHighLowPtData7->m_dHigh);	// 5-7

				dGradient4 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 2-6

				if ( dGradient1 < dGradient2 && dGradient2 < dGradient3)
				{
					if ( fabs(dGradient2) <= 0.3 && fabs(dGradient4) <= 0.3)
					{
						double dGradA, dGradB;
						double dYDrawPt1_1, dYDrawPt1_2;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt2;
						
						dGradA = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
						
						dGradB = GetGradient(	pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow, 
												pHighLowPtData7->m_nDataSeq, pHighLowPtData7->m_dHigh);	// 6-7
						
						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						dYDrawPt1_2 = dGradA * (pHighLowPtData7->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

						// Second Neck Data
						if ( (dGradA - dGradB) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData6->m_dLow - pHighLowPtData3->m_dHigh) + 
											dGradA*pHighLowPtData3->m_nDataSeq - dGradB*pHighLowPtData6->m_nDataSeq ) / ( dGradA-dGradB);
							dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						}

						// Neck Base Data2
						dNeckBaseYPt2 = dGradA * (pHighLowPtData4->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_R_HEADNSHOULDER,			pHighLowPtData1->m_nDataSeq,	pHighLowPtData7->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData7->m_nDataSeq,	dYDrawPt1_2,
												ZERO,							ZERO,							ZERO,							ZERO,
												pHighLowPtData4->m_nDataSeq,	pHighLowPtData4->m_dLow,		dXNeckPt2,						dYNeckPt2,
												pHighLowPtData4->m_dLow,		dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Double Top					(쌍봉형)
// **********************************************
BOOL CWavePattern::Find_DoubleTop( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = NULL;
	int		nPointCnt;
	int		nMininumData = 5;	// Double Top를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;

		if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData3->m_dLow > pHighLowPtData5->m_dLow))
		{
			if ( (pHighLowPtData3->m_nDataSeq - pHighLowPtData1->m_nDataSeq) >= (pHighLowPtData5->m_nDataSeq - pHighLowPtData3->m_nDataSeq))
			{
				dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
												pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
				
				dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5
				
				dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

				if ( fabs(dGradient3) <= 0.3)
				{
					if ( dGradient1 > dGradient3 && dGradient3 > dGradient2)
					{
						double dGradA, dGradB;
						double dYDrawPt1_1, dYDrawPt1_2;
						double dYDrawPt2_1, dYDrawPt2_2;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 4-5

						dGradB = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

						// First Draw Data
						dYDrawPt1_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
						dYDrawPt1_2 = dGradB * (pHighLowPtData5->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
						dYDrawPt2_2 = dGradB * (pHighLowPtData5->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

						// Second Neck Data
						if ( (dGradA - dGradB) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData3->m_dLow - pHighLowPtData4->m_dHigh) + 
											dGradA*pHighLowPtData4->m_nDataSeq - dGradB*pHighLowPtData3->m_nDataSeq ) / ( dGradA-dGradB);
							dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData4->m_nDataSeq) + pHighLowPtData4->m_dHigh;
						}

						// Neck Base Data2
						dNeckBaseYPt2 = dGradB * (pHighLowPtData2->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_DOUBLETOP,					pHighLowPtData1->m_nDataSeq,	pHighLowPtData5->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData5->m_nDataSeq,	dYDrawPt1_2,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					pHighLowPtData5->m_nDataSeq,	dYDrawPt2_2,
												pHighLowPtData2->m_nDataSeq,	pHighLowPtData2->m_dHigh,		dXNeckPt2,						dYNeckPt2,
												pHighLowPtData2->m_dHigh,		dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}

// **********************************************
// Reversal Double Top			(역쌍봉형)
// **********************************************
BOOL CWavePattern::Find_R_DoubleTop( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = NULL;
	int		nPointCnt;
	int		nMininumData = 5;	// Reversal Double Top를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;

		if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData3->m_dHigh < pHighLowPtData5->m_dHigh))
		{
			if ( (pHighLowPtData3->m_nDataSeq - pHighLowPtData1->m_nDataSeq) >= (pHighLowPtData5->m_nDataSeq - pHighLowPtData3->m_nDataSeq))
			{
				dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
												pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
				
				dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
				
				dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

				if ( fabs(dGradient3) <= 0.3)
				{
					if ( dGradient1 < dGradient3 && dGradient3 < dGradient2)
					{
						double dGradA, dGradB;
						double dYDrawPt1_1, dYDrawPt1_2;
						double dYDrawPt2_1, dYDrawPt2_2; 
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

						dGradB = GetGradient(	pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 4-5

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						dYDrawPt1_2 = dGradA * (pHighLowPtData5->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;
						dYDrawPt2_2 = dGradA * (pHighLowPtData5->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;

						// Second Neck Data
						if ( (dGradA - dGradB) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData4->m_dLow - pHighLowPtData3->m_dHigh) + 
											dGradA*pHighLowPtData3->m_nDataSeq - dGradB*pHighLowPtData4->m_nDataSeq ) / ( dGradA-dGradB);
							dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						}

						// Neck Base Data2
						dNeckBaseYPt2 = dGradA * (pHighLowPtData2->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_R_DOUBLETOP,				pHighLowPtData1->m_nDataSeq,	pHighLowPtData5->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData5->m_nDataSeq,	dYDrawPt1_2,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					pHighLowPtData5->m_nDataSeq,	dYDrawPt2_2,
												pHighLowPtData2->m_nDataSeq,	pHighLowPtData2->m_dLow,		dXNeckPt2,						dYNeckPt2,
												pHighLowPtData2->m_dLow,		dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bull Isosceles Triangles		(강세이등변삼각형)
// **********************************************
BOOL CWavePattern::Find_Bull_IsoTriangles( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bull Isosceles Triangles를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData4->m_dHigh < pHighLowPtData6->m_dHigh))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 4-6

			if ( dGradient3 < -0.3 && dGradient2 > 0.3 && (fabs(dGradient3)-fabs(dGradient2) < 0.3))
			{
				if ( (pHighLowPtData2->m_dHigh - pHighLowPtData1->m_dLow) < 2*(pHighLowPtData2->m_dHigh - pHighLowPtData3->m_dLow))
				{
					if ( dGradient1 > dGradient2 && dGradient3 < dGradient4)
					{
						double dGradA, dGradB, dGradA_L, dGradB_L;
						double dYDrawPt1_1;
						double dYDrawPt2_1;
						double dXCrossPt1, dYCrossPt1;
						double dXNeckPt1, dYNeckPt1;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt1, dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

						dGradB = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

						dGradA_L = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 5-6

						dGradB_L = GetGradient(	pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
												pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh);	// 1-2

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

						if ( (dGradA - dGradB) != 0)
						{
							dXCrossPt1 = ( ( pHighLowPtData3->m_dLow - pHighLowPtData2->m_dHigh) + 
											dGradA*pHighLowPtData2->m_nDataSeq - dGradB*pHighLowPtData3->m_nDataSeq ) / ( dGradA-dGradB);
							dYCrossPt1 = dGradA*(dXCrossPt1-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
						}

						// First Neck Data
						if ( (dGradB - dGradB_L) != 0)
						{
							dXNeckPt1 = ( ( pHighLowPtData1->m_dLow - pHighLowPtData3->m_dLow) + 
											dGradB*pHighLowPtData3->m_nDataSeq - dGradB_L*pHighLowPtData1->m_nDataSeq ) / ( dGradB-dGradB_L);
							dYNeckPt1 = dGradB*(dXNeckPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
						}
						// Second Neck Data
						if ( (dGradA - dGradA_L) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData5->m_dLow - pHighLowPtData2->m_dHigh) + 
											dGradA*pHighLowPtData2->m_nDataSeq - dGradA_L*pHighLowPtData5->m_nDataSeq ) / ( dGradA-dGradA_L);
							dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
						}

						// Neck Base Data1
						dNeckBaseYPt1 = dGradB * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
						// Neck Base Data2
						dNeckBaseYPt2 = dGradA * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_BULL_ISOTRIANGLE,			pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					dXCrossPt1,						dYCrossPt1,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					dXCrossPt1,						dYCrossPt1,
												dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
												dNeckBaseYPt1,					dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}

// **********************************************
// Bear Isosceles Triangles		(약세이등변삼각형)
// **********************************************
BOOL CWavePattern::Find_Bear_IsoTriangles( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bear Isosceles Triangles를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData4->m_dLow > pHighLowPtData6->m_dLow))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 4-6

			if ( dGradient2 < -0.3 && dGradient3 > 0.3 && (fabs(dGradient2)-fabs(dGradient3) < 0.3))
			{
				if ( (pHighLowPtData1->m_dHigh - pHighLowPtData2->m_dLow) < 2*(pHighLowPtData3->m_dHigh - pHighLowPtData2->m_dLow))
				{
					if ( dGradient1 < dGradient2 && dGradient3 > dGradient4)
					{
						double dGradA, dGradB, dGradA_H, dGradB_H;
						double dYDrawPt1_1;
						double dYDrawPt2_1;
						double dXCrossPt1, dYCrossPt1;
						double dXNeckPt1, dYNeckPt1;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt1, dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5

						dGradB = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

						dGradA_H = GetGradient(	pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
												pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow);	// 1-2

						dGradB_H = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 5-6

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;

						if ( (dGradA - dGradB) != 0)
						{
							dXCrossPt1 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData3->m_dHigh) + 
											dGradA*pHighLowPtData3->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradA-dGradB);
							dYCrossPt1 = dGradA*(dXCrossPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						}

						// First Neck Data
						if ( (dGradA_H - dGradA) != 0)
						{
							dXNeckPt1 = ( ( pHighLowPtData3->m_dHigh - pHighLowPtData1->m_dHigh) + 
											dGradA_H*pHighLowPtData1->m_nDataSeq - dGradA*pHighLowPtData3->m_nDataSeq ) / ( dGradA_H-dGradA);
							dYNeckPt1 = dGradA_H*(dXNeckPt1-pHighLowPtData1->m_nDataSeq) + pHighLowPtData1->m_dHigh;
						}
						// Second Neck Data
						if ( (dGradB_H - dGradB) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData5->m_dHigh) + 
											dGradB_H*pHighLowPtData5->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradB_H-dGradB);
							dYNeckPt2 = dGradB_H*(dXNeckPt2-pHighLowPtData5->m_nDataSeq) + pHighLowPtData5->m_dHigh;
						}

						// Neck Base Data1
						dNeckBaseYPt1 = dGradA * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						// Neck Base Data2
						dNeckBaseYPt2 = dGradB * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_BEAR_ISOTRIANGLE,			pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					dXCrossPt1,						dYCrossPt1,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					dXCrossPt1,						dYCrossPt1,
												dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
												dNeckBaseYPt1,					dNeckBaseYPt2	);


					}
				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bull Triangles				(상승삼각형)
// **********************************************
BOOL CWavePattern::Find_Bull_Triangles( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bull Triangles를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData4->m_dHigh < pHighLowPtData6->m_dHigh))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 4-6

			if ( fabs(dGradient3) < 0.3 && dGradient2 > 0.3 )
			{
				if ( dGradient1 > dGradient2 && dGradient3 < dGradient4)
				{
					double dGradA, dGradB, dGradA_L, dGradB_L;
					double dYDrawPt1_1;
					double dYDrawPt2_1;
					double dXCrossPt1, dYCrossPt1;
					double dXNeckPt1, dYNeckPt1;
					double dXNeckPt2, dYNeckPt2;
					double dNeckBaseYPt1, dNeckBaseYPt2;

					dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

					dGradB = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

					dGradA_L = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 5-6

					dGradB_L = GetGradient(	pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh);	// 1-2

					// First Draw Data
					dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;

					// Second Draw Data
					dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

					if ( (dGradA - dGradB) != 0)
					{
						dXCrossPt1 = ( ( pHighLowPtData3->m_dLow - pHighLowPtData2->m_dHigh) + 
										dGradA*pHighLowPtData2->m_nDataSeq - dGradB*pHighLowPtData3->m_nDataSeq ) / ( dGradA-dGradB);
						dYCrossPt1 = dGradA*(dXCrossPt1-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
					}

					// First Neck Data
					if ( (dGradB - dGradB_L) != 0)
					{
						dXNeckPt1 = ( ( pHighLowPtData1->m_dLow - pHighLowPtData3->m_dLow) + 
										dGradB*pHighLowPtData3->m_nDataSeq - dGradB_L*pHighLowPtData1->m_nDataSeq ) / ( dGradB-dGradB_L);
						dYNeckPt1 = dGradB*(dXNeckPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
					}
					// Second Neck Data
					if ( (dGradA - dGradA_L) != 0)
					{
						dXNeckPt2 = ( ( pHighLowPtData5->m_dLow - pHighLowPtData2->m_dHigh) + 
										dGradA*pHighLowPtData2->m_nDataSeq - dGradA_L*pHighLowPtData5->m_nDataSeq ) / ( dGradA-dGradA_L);
						dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
					}

					// Neck Base Data1
					dNeckBaseYPt1 = dGradB * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
					// Neck Base Data2
					dNeckBaseYPt2 = dGradA * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;


					AddNewWavePatternData(	pObjWavePatternData,
											PTN_BULL_TRIANGLE,				pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
											pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					dXCrossPt1,						dYCrossPt1,
											pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					dXCrossPt1,						dYCrossPt1,
											dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
											dNeckBaseYPt1,					dNeckBaseYPt2	);

				}
			}
		}
	}

	return TRUE;
}

// **********************************************
// Bear Triangles				(하락삼각형)
// **********************************************
BOOL CWavePattern::Find_Bear_Triangles( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bear Triangles를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData4->m_dLow > pHighLowPtData6->m_dLow))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 4-6

			if ( dGradient2 < -0.3 && fabs(dGradient3) < 0.3 )
			{
				if ( dGradient1 < dGradient2 && dGradient3 > dGradient4)
				{
					double dGradA, dGradB, dGradA_H, dGradB_H;
					double dYDrawPt1_1;
					double dYDrawPt2_1;
					double dXCrossPt1, dYCrossPt1;
					double dXNeckPt1, dYNeckPt1;
					double dXNeckPt2, dYNeckPt2;
					double dNeckBaseYPt1, dNeckBaseYPt2;

					dGradA = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5

					dGradB = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

					dGradA_H = GetGradient(	pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow);	// 1-2

					dGradB_H = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 5-6

					// First Draw Data
					dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

					// Second Draw Data
					dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;

					if ( (dGradA - dGradB) != 0)
					{
						dXCrossPt1 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData3->m_dHigh) + 
										dGradA*pHighLowPtData3->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradA-dGradB);
						dYCrossPt1 = dGradA*(dXCrossPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
					}

					// First Neck Data
					if ( (dGradA_H - dGradA) != 0)
					{
						dXNeckPt1 = ( ( pHighLowPtData3->m_dHigh - pHighLowPtData1->m_dHigh) + 
										dGradA_H*pHighLowPtData1->m_nDataSeq - dGradA*pHighLowPtData3->m_nDataSeq ) / ( dGradA_H-dGradA);
						dYNeckPt1 = dGradA_H*(dXNeckPt1-pHighLowPtData1->m_nDataSeq) + pHighLowPtData1->m_dHigh;
					}
					// Second Neck Data
					if ( (dGradB_H - dGradB) != 0)
					{
						dXNeckPt2 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData5->m_dHigh) + 
										dGradB_H*pHighLowPtData5->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradB_H-dGradB);
						dYNeckPt2 = dGradB_H*(dXNeckPt2-pHighLowPtData5->m_nDataSeq) + pHighLowPtData5->m_dHigh;
					}

					// Neck Base Data1
					dNeckBaseYPt1 = dGradA * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
					// Neck Base Data2
					dNeckBaseYPt2 = dGradB * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;


					AddNewWavePatternData(	pObjWavePatternData,
											PTN_BEAR_TRIANGLE,				pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
											pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					dXCrossPt1,						dYCrossPt1,
											pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					dXCrossPt1,						dYCrossPt1,
											dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
											dNeckBaseYPt1,					dNeckBaseYPt2	);


				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bullish Wide Triangles		(상승확장삼각형)
// **********************************************
BOOL CWavePattern::Find_Bullish_WideTriangles( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6, *pHighLowPtData7;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = pHighLowPtData7 = NULL;
	int		nPointCnt;
	int		nMininumData = 7;	// Bullish Wide Triangles를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4, dGradient5, dGradient6;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;
		pHighLowPtData7 = paryHighLowPtData->GetAt(i+6);	if ( !pHighLowPtData7 ) continue;

		if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData5->m_dLow > pHighLowPtData7->m_dLow))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

			dGradient3 = GetModifyGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow, 
											pHighLowPtData7->m_nDataSeq, pHighLowPtData7->m_dLow);	// 5-7
			
			dGradient4 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

			dGradient5 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 4-6

			dGradient6 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 2-6

			if ( dGradient4 > 0.3 && dGradient5 > 0.3 && (fabs(dGradient4-dGradient5) < 0.3) )
			{
				if ( dGradient2 < -0.3 && (fabs(dGradient6)-fabs(dGradient2) < 0.3) )
				{
					if ( dGradient1 > dGradient2 && dGradient2 > dGradient3)
					{
						double dGradA, dGradB, dGradB_H;
						double dYDrawPt1_1, dYDrawPt1_2;
						double dYDrawPt2_1, dYDrawPt2_2; 
						double dXNeckPt1, dYNeckPt1;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt1, dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 2-6

						dGradB = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

						dGradB_H = GetGradient(	pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh, 
												pHighLowPtData7->m_nDataSeq, pHighLowPtData7->m_dLow);	// 6-7

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
						dYDrawPt1_2 = dGradA * (pHighLowPtData7->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
						dYDrawPt2_2 = dGradB * (pHighLowPtData7->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

						// First Neck Data
						if ( (dGradB_H - dGradB) != 0)
						{
							dXNeckPt1 = ( ( pHighLowPtData3->m_dLow - pHighLowPtData6->m_dHigh) + 
											dGradB_H*pHighLowPtData6->m_nDataSeq - dGradB*pHighLowPtData3->m_nDataSeq ) / ( dGradB_H-dGradB);
							dYNeckPt1 = dGradB_H*(dXNeckPt1-pHighLowPtData6->m_nDataSeq) + pHighLowPtData6->m_dHigh;
						}
						// Senond Neck Data
						dXNeckPt2 = dXNeckPt1;
						dYNeckPt2 = dYNeckPt1;

						// Neck Base Data1
						dNeckBaseYPt1 = dGradB * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
						// Neck Base Data2
						dNeckBaseYPt2 = dGradA * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_BULLISH_WIDETRIANGLE,		pHighLowPtData1->m_nDataSeq,	pHighLowPtData7->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData7->m_nDataSeq,	dYDrawPt1_2,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					pHighLowPtData7->m_nDataSeq,	dYDrawPt2_2,
												dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
												dNeckBaseYPt1,					dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}

// **********************************************
// Bearish Wide Triangles		(하락확장삼각형)
// **********************************************
BOOL CWavePattern::Find_Bearish_WideTriangles( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6, *pHighLowPtData7;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = pHighLowPtData7 = NULL;
	int		nPointCnt;
	int		nMininumData = 7;	// Bearish Wide Triangles를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4, dGradient5, dGradient6;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;
		pHighLowPtData7 = paryHighLowPtData->GetAt(i+6);	if ( !pHighLowPtData7 ) continue;

		if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData5->m_dHigh < pHighLowPtData7->m_dHigh))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5

			dGradient3 = GetModifyGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh, 
											pHighLowPtData7->m_nDataSeq, pHighLowPtData7->m_dHigh);	// 5-7
			
			dGradient4 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

			dGradient5 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 4-6

			dGradient6 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 2-6

			if ( dGradient4 < -0.3 && dGradient5 < -0.3 && (fabs(dGradient4-dGradient5) < 0.3) )
			{
				if ( dGradient2 > 0.3 && (fabs(dGradient2)-fabs(dGradient6) < 0.3) )
				{
					if ( dGradient1 < dGradient2 && dGradient2 < dGradient3)
					{
						double dGradA, dGradB, dGradA_L;
						double dYDrawPt1_1, dYDrawPt1_2;
						double dYDrawPt2_1, dYDrawPt2_2; 
						double dXNeckPt1, dYNeckPt1;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt1, dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
						
						dGradB = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 2-6

						dGradA_L = GetGradient(	pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow, 
												pHighLowPtData7->m_nDataSeq, pHighLowPtData7->m_dHigh);	// 6-7

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						dYDrawPt1_2 = dGradA * (pHighLowPtData7->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;
						dYDrawPt2_2 = dGradB * (pHighLowPtData7->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;

						// First Neck Data
						if ( (dGradA - dGradA_L) != 0)
						{
							dXNeckPt1 = ( ( pHighLowPtData6->m_dLow - pHighLowPtData3->m_dHigh) + 
											dGradA*pHighLowPtData3->m_nDataSeq - dGradA_L*pHighLowPtData6->m_nDataSeq ) / ( dGradA-dGradA_L);
							dYNeckPt1 = dGradA*(dXNeckPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						}
						// Senond Neck Data
						dXNeckPt2 = dXNeckPt1;
						dYNeckPt2 = dYNeckPt1;

						// Neck Base Data1
						dNeckBaseYPt1 = dGradA * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						// Neck Base Data2
						dNeckBaseYPt2 = dGradB * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_BEARISH_WIDETRIANGLE,		pHighLowPtData1->m_nDataSeq,	pHighLowPtData7->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData7->m_nDataSeq,	dYDrawPt1_2,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					pHighLowPtData7->m_nDataSeq,	dYDrawPt2_2,
												dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
												dNeckBaseYPt1,					dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bull Wedge					(상향쐐기형)
// **********************************************
BOOL CWavePattern::Find_Bull_Wedge( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bull Wedge를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		//if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData4->m_dLow > pHighLowPtData6->m_dLow))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 4-6

			if ( dGradient1 > 0.3 && dGradient2 < dGradient3 && (fabs(dGradient3-dGradient2) > 0.3))
			{
				if ( dGradient1 < dGradient2 && dGradient3 > dGradient4)
				{
					double dGradA, dGradB, dGradA_H, dGradB_H;
					double dYDrawPt1_1;
					double dYDrawPt2_1;
					double dXCrossPt1, dYCrossPt1;
					double dXNeckPt1, dYNeckPt1;
					double dXNeckPt2, dYNeckPt2;
					double dNeckBaseYPt1, dNeckBaseYPt2;

					dGradA = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5

					dGradB = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

					dGradA_H = GetGradient(	pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow);	// 1-2

					dGradB_H = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 5-6

					// Compare Dimension of Gradient
					if ( dGradA > dGradB)	continue;


					// First Draw Data
					dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

					// Second Draw Data
					dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;

					if ( (dGradA - dGradB) != 0)
					{
						dXCrossPt1 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData3->m_dHigh) + 
										dGradA*pHighLowPtData3->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradA-dGradB);
						dYCrossPt1 = dGradA*(dXCrossPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
					}

					// First Neck Data
					if ( (dGradA_H - dGradA) != 0)
					{
						dXNeckPt1 = ( ( pHighLowPtData3->m_dHigh - pHighLowPtData1->m_dHigh) + 
										dGradA_H*pHighLowPtData1->m_nDataSeq - dGradA*pHighLowPtData3->m_nDataSeq ) / ( dGradA_H-dGradA);
						dYNeckPt1 = dGradA_H*(dXNeckPt1-pHighLowPtData1->m_nDataSeq) + pHighLowPtData1->m_dHigh;
					}
					// Second Neck Data
					if ( (dGradB_H - dGradB) != 0)
					{
						dXNeckPt2 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData5->m_dHigh) + 
										dGradB_H*pHighLowPtData5->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradB_H-dGradB);
						dYNeckPt2 = dGradB_H*(dXNeckPt2-pHighLowPtData5->m_nDataSeq) + pHighLowPtData5->m_dHigh;
					}

					// Neck Base Data1
					dNeckBaseYPt1 = dGradA * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
					// Neck Base Data2
					dNeckBaseYPt2 = dGradB * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;


					AddNewWavePatternData(	pObjWavePatternData,
											PTN_BULL_WEDGE,					pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
											pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					dXCrossPt1,						dYCrossPt1,
											pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					dXCrossPt1,						dYCrossPt1,
											dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
											dNeckBaseYPt1,					dNeckBaseYPt2	);

				}
			}
		}
	}

	return TRUE;
}

// **********************************************
// Bear Wedge					(하향쐐기형)
// **********************************************
BOOL CWavePattern::Find_Bear_Wedge( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bear Wedge를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		//if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData4->m_dHigh < pHighLowPtData6->m_dHigh))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 4-6

			if ( dGradient1 < -0.3 && dGradient2 > dGradient3 && (fabs(dGradient2-dGradient3) > 0.3))
			{
				if ( dGradient1 > dGradient2 && dGradient3 < dGradient4)
				{
					double dGradA, dGradB, dGradA_L, dGradB_L;
					double dYDrawPt1_1;
					double dYDrawPt2_1;
					double dXCrossPt1, dYCrossPt1;
					double dXNeckPt1, dYNeckPt1;
					double dXNeckPt2, dYNeckPt2;
					double dNeckBaseYPt1, dNeckBaseYPt2;

					dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4
					
					dGradB = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

					dGradA_L = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 5-6

					dGradB_L = GetGradient(	pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh);	// 1-2

					// Compare Dimension of Gradient
					if ( dGradA > dGradB)	continue;


					// First Draw Data
					dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;

					// Second Draw Data
					dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

					if ( (dGradA - dGradB) != 0)
					{
						dXCrossPt1 = ( ( pHighLowPtData3->m_dLow - pHighLowPtData2->m_dHigh) + 
										dGradA*pHighLowPtData2->m_nDataSeq - dGradB*pHighLowPtData3->m_nDataSeq ) / ( dGradA-dGradB);
						dYCrossPt1 = dGradA*(dXCrossPt1-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
					}

					// First Neck Data
					if ( (dGradB - dGradB_L) != 0)
					{
						dXNeckPt1 = ( ( pHighLowPtData1->m_dLow - pHighLowPtData3->m_dLow) + 
										dGradB*pHighLowPtData3->m_nDataSeq - dGradB_L*pHighLowPtData1->m_nDataSeq ) / ( dGradB-dGradB_L);
						dYNeckPt1 = dGradB*(dXNeckPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
					}
					// Second Neck Data
					if ( (dGradA - dGradA_L) != 0)
					{
						dXNeckPt2 = ( ( pHighLowPtData5->m_dLow - pHighLowPtData2->m_dHigh) + 
										dGradA*pHighLowPtData2->m_nDataSeq - dGradA_L*pHighLowPtData5->m_nDataSeq ) / ( dGradA-dGradA_L);
						dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
					}

					// Neck Base Data1
					dNeckBaseYPt1 = dGradB * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
					// Neck Base Data2
					dNeckBaseYPt2 = dGradA * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;


					AddNewWavePatternData(	pObjWavePatternData,
											PTN_BEAR_WEDGE,					pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
											pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					dXCrossPt1,						dYCrossPt1,
											pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					dXCrossPt1,						dYCrossPt1,
											dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
											dNeckBaseYPt1,					dNeckBaseYPt2	);

				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bull Square Flag				(상승사각깃발형)
// **********************************************
BOOL CWavePattern::Find_Bull_SquareFlag( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bull Square Flag를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData4->m_dHigh < pHighLowPtData6->m_dHigh))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 4-6

			if ( dGradient3 <= 0 && dGradient2 <= 0 && (fabs(dGradient3-dGradient2) <= 0.3))
			{
				if ( (pHighLowPtData2->m_dHigh - pHighLowPtData1->m_dLow) > 2*(pHighLowPtData2->m_dHigh - pHighLowPtData3->m_dLow))
				{
					if ( dGradient1 > dGradient2 && dGradient3 < dGradient4)
					{
						double dGradA, dGradB, dGradA_L;
						double dYDrawPt1_1, dYDrawPt1_2;
						double dYDrawPt2_1, dYDrawPt2_2;
						double dXNeckPt1, dYNeckPt1;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt1, dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

						dGradB = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

						dGradA_L = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 5-6

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
						dYDrawPt1_2 = dGradA * (pHighLowPtData6->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
						dYDrawPt2_2 = dGradB * (pHighLowPtData6->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

						// First Neck Data
						dXNeckPt1 = pHighLowPtData2->m_nDataSeq;
						dYNeckPt1 = pHighLowPtData2->m_dHigh;
						// Second Neck Data
						if ( (dGradA - dGradA_L) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData5->m_dLow - pHighLowPtData2->m_dHigh) + 
											dGradA*pHighLowPtData2->m_nDataSeq - dGradA_L*pHighLowPtData5->m_nDataSeq ) / ( dGradA-dGradA_L);
							dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
						}

						// Neck Base Data1
						dNeckBaseYPt1 = pHighLowPtData2->m_dHigh;
						// Neck Base Data2
						dNeckBaseYPt2 = pHighLowPtData1->m_dLow;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_BULL_SQUAREFLAG,			pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData6->m_nDataSeq,	dYDrawPt1_2,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					pHighLowPtData6->m_nDataSeq,	dYDrawPt2_2,
												dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
												dNeckBaseYPt1,					dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bear Square Flag				(하락사각깃발형)
// **********************************************
BOOL CWavePattern::Find_Bear_SquareFlag( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bear Square Flag를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData4->m_dLow > pHighLowPtData6->m_dLow))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 4-6

			if ( dGradient2 >= 0 && dGradient3 >= 0 && (fabs(dGradient2-dGradient3) <= 0.3))
			{
				if ( (pHighLowPtData1->m_dHigh - pHighLowPtData2->m_dLow) > 2*(pHighLowPtData3->m_dHigh - pHighLowPtData2->m_dLow))
				{
					if ( dGradient1 < dGradient2 && dGradient3 > dGradient4)
					{
						double dGradA, dGradB, dGradB_H;
						double dYDrawPt1_1, dYDrawPt1_2;
						double dYDrawPt2_1, dYDrawPt2_2;
						double dXNeckPt1, dYNeckPt1;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt1, dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5

						dGradB = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

						dGradB_H = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 5-6

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						dYDrawPt1_2 = dGradA * (pHighLowPtData6->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;
						dYDrawPt2_2 = dGradB * (pHighLowPtData6->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;

						// First Neck Data
						dXNeckPt1 = pHighLowPtData2->m_nDataSeq;
						dYNeckPt1 = pHighLowPtData2->m_dLow;
						// Second Neck Data
						if ( (dGradB_H - dGradB) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData5->m_dHigh) + 
											dGradB_H*pHighLowPtData5->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradB_H-dGradB);
							dYNeckPt2 = dGradB_H*(dXNeckPt2-pHighLowPtData5->m_nDataSeq) + pHighLowPtData5->m_dHigh;
						}

						// Neck Base Data1
						dNeckBaseYPt1 = pHighLowPtData2->m_dLow;
						// Neck Base Data2
						dNeckBaseYPt2 = pHighLowPtData1->m_dHigh;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_BEAR_SQUAREFLAG,			pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData6->m_nDataSeq,	dYDrawPt1_2,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					pHighLowPtData6->m_nDataSeq,	dYDrawPt2_2,
												dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
												dNeckBaseYPt1,					dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bull Triangle Flag			(상승삼각깃발형)
// **********************************************
BOOL CWavePattern::Find_Bull_TriangleFlag( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bull Triangle Flag를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData4->m_dHigh < pHighLowPtData6->m_dHigh))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 4-6

			if ( dGradient3 < -0.3 && dGradient2 > 0.3 )
			{
				if ( (pHighLowPtData2->m_dHigh - pHighLowPtData1->m_dLow) > 2*(pHighLowPtData2->m_dHigh - pHighLowPtData3->m_dLow))
				{
					if ( dGradient1 > dGradient2 && dGradient3 < dGradient4)
					{
						double dGradA, dGradB, dGradA_L;
						double dYDrawPt1_1;
						double dYDrawPt2_1;
						double dXCrossPt1, dYCrossPt1;
						double dXNeckPt1, dYNeckPt1;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt1, dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

						dGradB = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

						dGradA_L = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 5-6

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

						if ( (dGradA - dGradB) != 0)
						{
							dXCrossPt1 = ( ( pHighLowPtData3->m_dLow - pHighLowPtData2->m_dHigh) + 
											dGradA*pHighLowPtData2->m_nDataSeq - dGradB*pHighLowPtData3->m_nDataSeq ) / ( dGradA-dGradB);
							dYCrossPt1 = dGradA*(dXCrossPt1-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
						}

						// First Neck Data
						dXNeckPt1 = pHighLowPtData1->m_nDataSeq;
						dYNeckPt1 = pHighLowPtData1->m_dLow;
						// Second Neck Data
						if ( (dGradA - dGradA_L) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData5->m_dLow - pHighLowPtData2->m_dHigh) + 
											dGradA*pHighLowPtData2->m_nDataSeq - dGradA_L*pHighLowPtData5->m_nDataSeq ) / ( dGradA-dGradA_L);
							dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
						}

						// Neck Base Data1
						dNeckBaseYPt1 = pHighLowPtData1->m_dLow;
						// Neck Base Data2
						dNeckBaseYPt2 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_BULL_TRIANGLEFLAG,			pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					dXCrossPt1,						dYCrossPt1,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					dXCrossPt1,						dYCrossPt1,
												dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
												dNeckBaseYPt1,					dNeckBaseYPt2	);


					}
				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bear Triangle Flag			(하락삼각깃발형)
// **********************************************
BOOL CWavePattern::Find_Bear_TriangleFlag( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bear Triangle Flag를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData4->m_dLow > pHighLowPtData6->m_dLow))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 4-6

			if ( dGradient2 < -0.3 && dGradient3 > 0.3 )
			{
				if ( (pHighLowPtData1->m_dHigh - pHighLowPtData2->m_dLow) > 2*(pHighLowPtData3->m_dHigh - pHighLowPtData2->m_dLow))
				{
					if ( dGradient1 < dGradient2 && dGradient3 > dGradient4)
					{
						double dGradA, dGradB, dGradB_H;
						double dYDrawPt1_1;
						double dYDrawPt2_1;
						double dXCrossPt1, dYCrossPt1;
						double dXNeckPt1, dYNeckPt1;
						double dXNeckPt2, dYNeckPt2;
						double dNeckBaseYPt1, dNeckBaseYPt2;

						dGradA = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
												pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5

						dGradB = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
												pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

						dGradB_H = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh, 
												pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 5-6

						// First Draw Data
						dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

						// Second Draw Data
						dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;

						if ( (dGradA - dGradB) != 0)
						{
							dXCrossPt1 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData3->m_dHigh) + 
											dGradA*pHighLowPtData3->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradA-dGradB);
							dYCrossPt1 = dGradA*(dXCrossPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
						}

						// First Neck Data
						dXNeckPt1 = pHighLowPtData1->m_nDataSeq;
						dYNeckPt1 = pHighLowPtData1->m_dHigh;
						// Second Neck Data
						if ( (dGradB_H - dGradB) != 0)
						{
							dXNeckPt2 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData5->m_dHigh) + 
											dGradB_H*pHighLowPtData5->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradB_H-dGradB);
							dYNeckPt2 = dGradB_H*(dXNeckPt2-pHighLowPtData5->m_nDataSeq) + pHighLowPtData5->m_dHigh;
						}

						// Neck Base Data1
						dNeckBaseYPt1 = pHighLowPtData1->m_dHigh;
						// Neck Base Data2
						dNeckBaseYPt2 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;


						AddNewWavePatternData(	pObjWavePatternData,
												PTN_BEAR_TRIANGLEFLAG,			pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
												pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					dXCrossPt1,						dYCrossPt1,
												pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					dXCrossPt1,						dYCrossPt1,
												dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
												dNeckBaseYPt1,					dNeckBaseYPt2	);

					}
				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bull Rectangle				(상승직사각형)
// **********************************************
BOOL CWavePattern::Find_Bull_Rectangle( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bull Rectangle를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dLow < pHighLowPtData3->m_dLow) && (pHighLowPtData4->m_dHigh < pHighLowPtData6->m_dHigh))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 4-6

			if ( fabs(dGradient3) <= 0.3 && fabs(dGradient2) <= 0.3 && (fabs(fabs(dGradient3)-fabs(dGradient2)) <= 0.3) )
			{
				if ( dGradient1 > dGradient2 && dGradient3 < dGradient4)
				{
					double dGradA, dGradB, dGradA_L, dGradB_L;
					double dYDrawPt1_1, dYDrawPt1_2;
					double dYDrawPt2_1, dYDrawPt2_2;
					double dXNeckPt1, dYNeckPt1;
					double dXNeckPt2, dYNeckPt2;
					double dNeckBaseYPt1, dNeckBaseYPt2;

					dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

					dGradB = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow);	// 3-5

					// First Draw Data
					dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
					dYDrawPt1_2 = dGradA * (pHighLowPtData6->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;

					// Second Draw Data
					dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
					dYDrawPt2_2 = dGradB * (pHighLowPtData6->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;

					dGradA_L = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dHigh);	// 5-6

					dGradB_L = GetGradient(	pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dLow, 
											pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh);	// 1-2

					// First Neck Data
					if ( (dGradB - dGradB_L) != 0)
					{
						dXNeckPt1 = ( ( pHighLowPtData1->m_dLow - pHighLowPtData3->m_dLow) + 
										dGradB*pHighLowPtData3->m_nDataSeq - dGradB_L*pHighLowPtData1->m_nDataSeq ) / ( dGradB-dGradB_L);
						dYNeckPt1 = dGradB*(dXNeckPt1-pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
					}
					// Second Neck Data
					if ( (dGradA - dGradA_L) != 0)
					{
						dXNeckPt2 = ( ( pHighLowPtData5->m_dLow - pHighLowPtData2->m_dHigh) + 
										dGradA*pHighLowPtData2->m_nDataSeq - dGradA_L*pHighLowPtData5->m_nDataSeq ) / ( dGradA-dGradA_L);
						dYNeckPt2 = dGradA*(dXNeckPt2-pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
					}

					// Neck Base Data1
					dNeckBaseYPt1 = dGradB * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dLow;
					// Neck Base Data2
					dNeckBaseYPt2 = dGradA * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;


					AddNewWavePatternData(	pObjWavePatternData,
											PTN_BULL_RECTANGLE,				pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
											pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData6->m_nDataSeq,	dYDrawPt1_2,
											pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					pHighLowPtData6->m_nDataSeq,	dYDrawPt2_2,
											dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
											dNeckBaseYPt1,					dNeckBaseYPt2	);

				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// Bear Rectangle				(하락직사각형)
// **********************************************
BOOL CWavePattern::Find_Bear_Rectangle( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5, *pHighLowPtData6;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = pHighLowPtData6 = NULL;
	int		nPointCnt;
	int		nMininumData = 6;	// Bear Rectangle를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3, dGradient4;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;
		pHighLowPtData6 = paryHighLowPtData->GetAt(i+5);	if ( !pHighLowPtData6 ) continue;

		if ( (pHighLowPtData1->m_dHigh > pHighLowPtData3->m_dHigh) && (pHighLowPtData4->m_dLow > pHighLowPtData6->m_dLow))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 1-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

			dGradient4 = GetModifyGradient( pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 4-6

			if ( fabs(dGradient2) <= 0.3 && fabs(dGradient3) <= 0.3 && (fabs(fabs(dGradient2)-fabs(dGradient3)) <= 0.3) )
			{
				if ( dGradient1 < dGradient2 && dGradient3 > dGradient4)
				{
					double dGradA, dGradB, dGradA_H, dGradB_H;
					double dYDrawPt1_1, dYDrawPt1_2;
					double dYDrawPt2_1, dYDrawPt2_2; 
					double dXNeckPt1, dYNeckPt1;
					double dXNeckPt2, dYNeckPt2;
					double dNeckBaseYPt1, dNeckBaseYPt2;

					dGradA = GetGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh);	// 3-5

					dGradB = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

					dGradA_H = GetGradient(	pHighLowPtData1->m_nDataSeq, pHighLowPtData1->m_dHigh, 
											pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow);	// 1-2

					dGradB_H = GetGradient(	pHighLowPtData5->m_nDataSeq, pHighLowPtData5->m_dHigh, 
											pHighLowPtData6->m_nDataSeq, pHighLowPtData6->m_dLow);	// 5-6

					// First Draw Data
					dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
					dYDrawPt1_2 = dGradA * (pHighLowPtData6->m_nDataSeq - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;

					// Second Draw Data
					dYDrawPt2_1 = dGradB * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;
					dYDrawPt2_2 = dGradB * (pHighLowPtData6->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;

					// First Neck Data
					if ( (dGradA_H - dGradA) != 0)
					{
						dXNeckPt1 = ( ( pHighLowPtData3->m_dHigh - pHighLowPtData1->m_dHigh) + 
										dGradA_H*pHighLowPtData1->m_nDataSeq - dGradA*pHighLowPtData3->m_nDataSeq ) / ( dGradA_H-dGradA);
						dYNeckPt1 = dGradA_H*(dXNeckPt1-pHighLowPtData1->m_nDataSeq) + pHighLowPtData1->m_dHigh;
					}
					// Second Neck Data
					if ( (dGradB_H - dGradB) != 0)
					{
						dXNeckPt2 = ( ( pHighLowPtData2->m_dLow - pHighLowPtData5->m_dHigh) + 
										dGradB_H*pHighLowPtData5->m_nDataSeq - dGradB*pHighLowPtData2->m_nDataSeq ) / ( dGradB_H-dGradB);
						dYNeckPt2 = dGradB_H*(dXNeckPt2-pHighLowPtData5->m_nDataSeq) + pHighLowPtData5->m_dHigh;
					}

					// Neck Base Data1
					dNeckBaseYPt1 = dGradA * (dXNeckPt1 - pHighLowPtData3->m_nDataSeq) + pHighLowPtData3->m_dHigh;
					// Neck Base Data2
					dNeckBaseYPt2 = dGradB * (dXNeckPt1 - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;


					AddNewWavePatternData(	pObjWavePatternData,
											PTN_BEAR_RECTANGLE,				pHighLowPtData1->m_nDataSeq,	pHighLowPtData6->m_nDataSeq,		
											pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData6->m_nDataSeq,	dYDrawPt1_2,
											pHighLowPtData1->m_nDataSeq,	dYDrawPt2_1,					pHighLowPtData6->m_nDataSeq,	dYDrawPt2_2,
											dXNeckPt1,						dYNeckPt1,						dXNeckPt2,						dYNeckPt2,
											dNeckBaseYPt1,					dNeckBaseYPt2	);


				}
			}
		}
	}

	return TRUE;
}


// **********************************************
// V Top
// **********************************************
BOOL CWavePattern::Find_V_Top( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = NULL;
	int		nPointCnt;
	int		nMininumData = 5;	// V Top를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_HIGHTOLOW) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;

		if ( (pHighLowPtData1->m_dHigh < pHighLowPtData3->m_dHigh) && (pHighLowPtData3->m_dHigh > pHighLowPtData5->m_dHigh))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh);	// 2-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 3-4
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

			if ( dGradient1 >= 0.6 && dGradient2 <= -0.6 && fabs(dGradient3) <= 0.1 )
			{
				double dGradA;
				double dYDrawPt1_1, dYDrawPt1_2;

				dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dLow, 
										pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dLow);	// 2-4

				// First Draw Data
				dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;
				dYDrawPt1_2 = dGradA * (pHighLowPtData5->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dLow;


				AddNewWavePatternData(	pObjWavePatternData,
										PTN_V_TOP,						pHighLowPtData1->m_nDataSeq,	pHighLowPtData5->m_nDataSeq,		
										pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData5->m_nDataSeq,	dYDrawPt1_2,
										ZERO,							ZERO,							ZERO,							ZERO,
										ZERO,							ZERO,							ZERO,							ZERO,
										ZERO,							ZERO	);

			}
		}
	}

	return TRUE;
}


// **********************************************
// V Bottom
// **********************************************
BOOL CWavePattern::Find_V_Bottom( CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	CArrayObjPt(CHighLowPtData) paryHighLowPtData = (CArrayObjPt(CHighLowPtData))pObjHighLowPtData;

	CHighLowPtData *pHighLowPtData1, *pHighLowPtData2, *pHighLowPtData3, *pHighLowPtData4, *pHighLowPtData5;
	pHighLowPtData1 = pHighLowPtData2 = pHighLowPtData3 = pHighLowPtData4 = pHighLowPtData5 = NULL;
	int		nPointCnt;
	int		nMininumData = 5;	// V Bottom를 구하기 위한 최소 데이터

	double	dGradient1, dGradient2, dGradient3;		// 기울기
	
	nPointCnt = paryHighLowPtData->GetSize();

	for(int i = 0; i <= nPointCnt-nMininumData; i++)
	{
		pHighLowPtData1 = paryHighLowPtData->GetAt(i);		if ( !pHighLowPtData1 ) continue;
		if ( pHighLowPtData1->m_nPointType != CHANGINGPT_LOWTOHIGH) continue;

		pHighLowPtData2 = paryHighLowPtData->GetAt(i+1);	if ( !pHighLowPtData2 ) continue;
		pHighLowPtData3 = paryHighLowPtData->GetAt(i+2);	if ( !pHighLowPtData3 ) continue;
		pHighLowPtData4 = paryHighLowPtData->GetAt(i+3);	if ( !pHighLowPtData4 ) continue;
		pHighLowPtData5 = paryHighLowPtData->GetAt(i+4);	if ( !pHighLowPtData5 ) continue;

		if ( (pHighLowPtData1->m_dLow > pHighLowPtData3->m_dLow) && (pHighLowPtData3->m_dLow < pHighLowPtData5->m_dLow))
		{
			dGradient1 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow);	// 2-3
			
			dGradient2 = GetModifyGradient(	pHighLowPtData3->m_nDataSeq, pHighLowPtData3->m_dLow, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 3-4
			
			dGradient3 = GetModifyGradient( pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
											pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

			if ( dGradient1 <= -0.6 && dGradient2 >= 0.6 && fabs(dGradient3) <= 0.1 )
			{
				double dGradA;
				double dYDrawPt1_1, dYDrawPt1_2;

				dGradA = GetGradient(	pHighLowPtData2->m_nDataSeq, pHighLowPtData2->m_dHigh, 
										pHighLowPtData4->m_nDataSeq, pHighLowPtData4->m_dHigh);	// 2-4

				// First Draw Data
				dYDrawPt1_1 = dGradA * (pHighLowPtData1->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;
				dYDrawPt1_2 = dGradA * (pHighLowPtData5->m_nDataSeq - pHighLowPtData2->m_nDataSeq) + pHighLowPtData2->m_dHigh;


				AddNewWavePatternData(	pObjWavePatternData,
										PTN_V_BOTTOM,					pHighLowPtData1->m_nDataSeq,	pHighLowPtData5->m_nDataSeq,		
										pHighLowPtData1->m_nDataSeq,	dYDrawPt1_1,					pHighLowPtData5->m_nDataSeq,	dYDrawPt1_2,
										ZERO,							ZERO,							ZERO,							ZERO,
										ZERO,							ZERO,							ZERO,							ZERO,
										ZERO,							ZERO	);

			}
		}
	}

	return TRUE;
}

// **********************************************
// Add Wave Pattern Data
// **********************************************
void CWavePattern::AddNewWavePatternData(	CObject* pObjWavePatternData,
											int nPatternType,		int nStartSeq,			int nEndSeq,		
											double dDrawSeq1_1,		double dDrawYPt1_1,		double dDrawSeq1_2,		double dDrawYPt1_2,	
											double dDrawSeq2_1,		double dDrawYPt2_1,		double dDrawSeq2_2,		double dDrawYPt2_2,
											double dNeckSeq1,		double dNeckYPt1,		double dNeckSeq2,		double dNeckYPt2,
											double dNeckBaseYPt1,	double dNeckBaseYPt2	)
{
	CArrayObjPt(CWavePatternData) paryWavePatternData = (CArrayObjPt(CWavePatternData))pObjWavePatternData;

	CWavePatternData *pWavePatternData = NULL;
	pWavePatternData = new CWavePatternData;
	
	pWavePatternData->m_nPatternType	=	nPatternType;		// Pattern Type
	pWavePatternData->m_nStartSeq		=	nStartSeq;			// Start Sequence of a Pattern
	pWavePatternData->m_nEndSeq			=	nEndSeq;			// End Sequence of a Pattern
	
	pWavePatternData->m_dDrawSeq1_1		=	dDrawSeq1_1;		// First Draw Data
	pWavePatternData->m_dDrawYPt1_1		=	dDrawYPt1_1;
	pWavePatternData->m_dDrawSeq1_2		=	dDrawSeq1_2;
	pWavePatternData->m_dDrawYPt1_2		=	dDrawYPt1_2;
	
	pWavePatternData->m_dDrawSeq2_1		=	dDrawSeq2_1;		// Second Draw Data
	pWavePatternData->m_dDrawYPt2_1		=	dDrawYPt2_1;
	pWavePatternData->m_dDrawSeq2_2		=	dDrawSeq2_2;
	pWavePatternData->m_dDrawYPt2_2		=	dDrawYPt2_2;
	
	pWavePatternData->m_dNeckSeq1		=	dNeckSeq1;			// First Neck Data
	pWavePatternData->m_dNeckYPt1		=	dNeckYPt1;
	pWavePatternData->m_dNeckSeq2		=	dNeckSeq2;			// Second Neck Data
	pWavePatternData->m_dNeckYPt2		=	dNeckYPt2;
	
	pWavePatternData->m_dNeckBaseYPt1	=	dNeckBaseYPt1;
	pWavePatternData->m_dNeckBaseYPt2	=	dNeckBaseYPt2;
	
	paryWavePatternData->Add(pWavePatternData);
}

// **********************************************
// Find All Wave Pattern
// **********************************************
void CWavePattern::Find_All_WavePattern(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData)
{
	// ## 1-1	Header & Shoulder
	Find_HeaderNShoulders(		pObjHighLowPtData, pObjWavePatternData);
	// ## 1-2	Reversal Header & Shoulder
	Find_R_HeaderNShoulders(	pObjHighLowPtData, pObjWavePatternData);

	// ## 2-1	DoubleTop					(쌍봉형)
	Find_DoubleTop(				pObjHighLowPtData, pObjWavePatternData);
	// ## 2-2	Reversal DoubleTop			(역쌍봉형)
	Find_R_DoubleTop(			pObjHighLowPtData, pObjWavePatternData);

	// ## 3-1	Bull Isosceles Triangles	(강세이등변삼각형)
	Find_Bull_IsoTriangles(		pObjHighLowPtData, pObjWavePatternData);
	// ## 3-2	Bear Isosceles Triangles	(약세이등변삼각형)
	Find_Bear_IsoTriangles(		pObjHighLowPtData, pObjWavePatternData);

	// ## 4-1	Bull Triangles				(상승삼각형)
	Find_Bull_Triangles(		pObjHighLowPtData, pObjWavePatternData);
	// ## 4-2	Bear Triangles				(하락삼각형)
	Find_Bear_Triangles(		pObjHighLowPtData, pObjWavePatternData);

	// ## 5-1	Bullish Wide Triangles		(상승확장삼각형)
	Find_Bullish_WideTriangles( pObjHighLowPtData, pObjWavePatternData);
	// ## 5-2	Bearish Wide Triangles		(하락확장삼각형)
	Find_Bearish_WideTriangles( pObjHighLowPtData, pObjWavePatternData);

	// ## 6-1	Bull Wedge					(상향쐐기형)
	Find_Bull_Wedge(			pObjHighLowPtData, pObjWavePatternData);
	// ## 6-2	Bear Wedge					(하향쐐기형)
	Find_Bear_Wedge(			pObjHighLowPtData, pObjWavePatternData);

	// ## 7-1	Bull Square Flag			(상승사각깃발형)
	Find_Bull_SquareFlag(		pObjHighLowPtData, pObjWavePatternData);
	// ## 7-2	Bear Square Flag			(하락사각깃발형)
	Find_Bear_SquareFlag(		pObjHighLowPtData, pObjWavePatternData);

	// ## 8-1	Bull Triangle Flag			(상승삼각깃발형)
	Find_Bull_TriangleFlag(		pObjHighLowPtData, pObjWavePatternData);
	// ## 8-2	Bear Triangle Flag			(하락삼각깃발형)
	Find_Bear_TriangleFlag(		pObjHighLowPtData, pObjWavePatternData);

	// ## 9-1	Bull Rectangle				(상승직사각형)
	Find_Bull_Rectangle(		pObjHighLowPtData, pObjWavePatternData);
	// ## 9-2	Bear Rectangle				(하락직사각형)
	Find_Bear_Rectangle(		pObjHighLowPtData, pObjWavePatternData);

	// ## 10-1	V Top
	Find_V_Top(					pObjHighLowPtData, pObjWavePatternData);
	// ## 10-2	V Bottom
	Find_V_Bottom(				pObjHighLowPtData, pObjWavePatternData);
}


double CWavePattern::GetGradient(double dXPos1, double dYPos1, double dXPos2, double dYPos2)
{
	double dGradient=0;

	if( (dXPos2 - dXPos1) != 0)
	{
		dGradient = (dYPos2 - dYPos1) / (dXPos2 - dXPos1);
	}

	return dGradient;
}

double CWavePattern::GetModifyGradient(double dXPos1, double dYPos1, double dXPos2, double dYPos2)
{
	double	dGradient=0;

	if( (dXPos2 - dXPos1) != 0 && (dYPos1 != 0))
	{
		dGradient = (dYPos2 - dYPos1) / dYPos1 / (dXPos2 - dXPos1) * 100;
	}

	return dGradient;
}


// **********************************************
// Get Pattern Name
// **********************************************
CString CWavePattern::GetPatternName(int nPatternType)
{
	ML_SET_LANGUAGE_RES();

	CString strPatternName;
	strPatternName =_T("");

// 2008.02.20 by LYH >>
	// Get Pattern Name
	switch (nPatternType)
	{
	case PTN_HEADNSHOULDER:			// Head & Shoulder				(머리어깨형)
		//strPatternName = STR_HEADNSHOULDER;
		strPatternName.LoadString(IDS_HEADNSHOULDER);
		break;
	case PTN_R_HEADNSHOULDER:		// Reversal Head & Shoulder		(역머리어깨형)
		//strPatternName = STR_R_HEADNSHOULDER;
		strPatternName.LoadString(IDS_R_HEADNSHOULDER);
		break;
	case PTN_DOUBLETOP:				// Double Top					(쌍봉형)
		//strPatternName = STR_DOUBLETOP;
		strPatternName.LoadString(IDS_DOUBLETOP);
		break;
	case PTN_R_DOUBLETOP:			// Reversal Double Top			(역쌍봉형)
		//strPatternName = STR_R_DOUBLETOP;
		strPatternName.LoadString(IDS_R_DOUBLETOP);
		break;
		
	case PTN_BULL_ISOTRIANGLE:		// Bull Isosceles Triangles		(강세이등변삼각형)
		//strPatternName = STR_BULL_ISOTRIANGLE;
		strPatternName.LoadString(IDS_BULL_ISOTRIANGLE);
		break;
	case PTN_BEAR_ISOTRIANGLE:		// Bear Isosceles Triangles		(약세이등변삼각형)
		//strPatternName = STR_BEAR_ISOTRIANGLE;
		strPatternName.LoadString(IDS_BEAR_ISOTRIANGLE);
		break;
		
	case PTN_BULL_TRIANGLE:			// Bull Triangles				(상승삼각형)
		//strPatternName = STR_BULL_TRIANGLE;
		strPatternName.LoadString(IDS_BULL_TRIANGLE);
		break;
	case PTN_BEAR_TRIANGLE:			// Bear Triangles				(하락삼각형)
		//strPatternName = STR_BEAR_TRIANGLE;
		strPatternName.LoadString(IDS_BEAR_TRIANGLE);
		break;
		
	case PTN_BULLISH_WIDETRIANGLE:	// Bullish Wide Triangles		(상승확장삼각형)
		//strPatternName = STR_BULLISH_WIDETRIANGLE;
		strPatternName.LoadString(IDS_BULLISH_WIDETRIANGLE);
		break;
	case PTN_BEARISH_WIDETRIANGLE:	// Bearish Wide Triangles		(하락확장삼각형)
		//strPatternName = STR_BEARISH_WIDETRIANGLE;
		strPatternName.LoadString(IDS_BEARISH_WIDETRIANGLE);
		break;
		
	case PTN_BULL_WEDGE:			// Bull Wedge					(상향쐐기형)
		//strPatternName = STR_BULL_WEDGE;
		strPatternName.LoadString(IDS_BULL_WEDGE);
		break;
	case PTN_BEAR_WEDGE:			// Bear Wedge					(하향쐐기형)
		//strPatternName = STR_BEAR_WEDGE;
		strPatternName.LoadString(IDS_BEAR_WEDGE);
		break;
		
	case PTN_BULL_SQUAREFLAG:		// Bull Square Flag				(상승사각깃발형)
		//strPatternName = STR_BULL_SQUAREFLAG;
		strPatternName.LoadString(IDS_BULL_SQUAREFLAG);
		break;
	case PTN_BEAR_SQUAREFLAG:		// Bear Square Flag				(하락사각깃발형)
		//strPatternName = STR_BEAR_SQUAREFLAG;
		strPatternName.LoadString(IDS_BEAR_SQUAREFLAG);
		break;
		
	case PTN_BULL_TRIANGLEFLAG:		// Bull Triangle Flag			(상승삼각깃발형)
		//strPatternName = STR_BULL_TRIANGLEFLAG;
		strPatternName.LoadString(IDS_BULL_TRIANGLEFLAG);
		break;
	case PTN_BEAR_TRIANGLEFLAG:		// Bear Triangle Flag			(하락삼각깃발형)
		//strPatternName = STR_BEAR_TRIANGLEFLAG;
		strPatternName.LoadString(IDS_BEAR_TRIANGLEFLAG);
		break;
		
	case PTN_BULL_RECTANGLE:		// Bull Rectangle				(상승직사각형)
		//strPatternName = STR_BULL_RECTANGLE;
		strPatternName.LoadString(IDS_BULL_RECTANGLE);
		break;
	case PTN_BEAR_RECTANGLE:		// Bear Rectangle				(하락직사각형)
		//strPatternName = STR_BEAR_RECTANGLE;
		strPatternName.LoadString(IDS_BEAR_RECTANGLE);
		break;
		
	case PTN_V_TOP:					// V Top						(단봉천정형)
		//strPatternName = STR_V_TOP;
		strPatternName.LoadString(IDS_V_TOP);
		break;
	case PTN_V_BOTTOM:				// V Bottom						(단봉바닥형)
		//strPatternName = STR_V_BOTTOM;
		strPatternName.LoadString(IDS_V_BOTTOM);
		break;
	}
// 2008.02.20 by LYH <<
	return	strPatternName;
}

// **********************************************
// Get Pattern Trend
// **********************************************
int CWavePattern::GetPatternTrend(int nPatternType)
{
	int nPatternTrend = 0;

	// Get Pattern Trend
	switch (nPatternType)
	{
	case PTN_R_HEADNSHOULDER:		// Reversal Head & Shoulder		(역머리어깨형)
	case PTN_R_DOUBLETOP:			// Reversal Double Top			(역쌍봉형)
	case PTN_BEARISH_WIDETRIANGLE:	// Bearish Wide Triangles		(하락확장삼각형)
	case PTN_V_TOP:					// V Top						(단봉천정형)
		nPatternTrend = UPWARD_REVERSE;
		break;

	case PTN_HEADNSHOULDER:			// Head & Shoulder				(머리어깨형)
	case PTN_DOUBLETOP:				// Double Top					(쌍봉형)
	case PTN_BULLISH_WIDETRIANGLE:	// Bullish Wide Triangles		(상승확장삼각형)
	case PTN_V_BOTTOM:				// V Bottom						(단봉바닥형)
		nPatternTrend = DNWARD_REVERSE;
		break;

	case PTN_BULL_ISOTRIANGLE:		// Bull Isosceles Triangles		(강세이등변삼각형)
	case PTN_BULL_TRIANGLE:			// Bull Triangles				(상승삼각형)
	case PTN_BEAR_WEDGE:			// Bear Wedge					(하향쐐기형)
	case PTN_BULL_SQUAREFLAG:		// Bull Square Flag				(상승사각깃발형)
	case PTN_BULL_TRIANGLEFLAG:		// Bull Triangle Flag			(상승삼각깃발형)
	case PTN_BULL_RECTANGLE:		// Bull Rectangle				(상승직사각형)
		nPatternTrend = UPWARD_CONTINUE;
		break;

	case PTN_BEAR_ISOTRIANGLE:		// Bear Isosceles Triangles		(약세이등변삼각형)
	case PTN_BEAR_TRIANGLE:			// Bear Triangles				(하락삼각형)
	case PTN_BULL_WEDGE:			// Bull Wedge					(상향쐐기형)
	case PTN_BEAR_SQUAREFLAG:		// Bear Square Flag				(하락사각깃발형)
	case PTN_BEAR_TRIANGLEFLAG:		// Bear Triangle Flag			(하락삼각깃발형)
	case PTN_BEAR_RECTANGLE:		// Bear Rectangle				(하락직사각형)
		nPatternTrend = DNWARD_CONTINUE;
		break;
	}

	return	nPatternTrend;
}

// **********************************************
// Get Pattern Trend Name
// **********************************************
CString CWavePattern::GetPatternTrendName(int nPatternType)
{
	ML_SET_LANGUAGE_RES();

	int nPatternTrend = GetPatternTrend( nPatternType);

	CString strRtnName;
	strRtnName =_T("");

// 2008.02.20 by LYH >>
	// Get Pattern Trend
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