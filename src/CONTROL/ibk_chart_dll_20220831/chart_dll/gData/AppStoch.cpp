#include "stdafx.h"
#include "AppStoch.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppStoch::CAppStoch(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 2;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	Calculate(0,0);
}

CAppStoch::~CAppStoch()
{

}

bool CAppStoch::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
TRACE("Calc Stoc Start\n");
	int	ii = 0, jj = 0, kk = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic*	gBasic = NULL;
	int	nday[3];
	int	nKind = 0;

	if (m_iGKind == GK_STOCHF)
		nKind = 2;
	else if (m_iGKind == GK_STOCHS)
		nKind = 3;
	else
		return false;

	for ( ii = 0 ; ii < nKind ; ii++ )
	{
		nday[ii] = (int)m_pCInfo->awValue[ii];

		if (nday[ii] <= 0)
			return false;
	}

	int	nPos = 0, nDataSize = m_iDataSize;
	int	nTot = 0;

	for ( ii = 0 ; ii < nKind ; ii++ )
	{
		nTot += nday[ii];
	}

	nTot -= (ii - 2);

	bool	bOrgLast = bLast;
	if (m_iCOption & GC_MAEXP) bLast = false;
	if (bLast)
	{
		nDataSize = nTot;
		nPos = m_iDataSize - nDataSize;		
	}

	if (nPos < 0) return false;
	
	double*	pTemp[3];
	int	nSize = sizeof(pTemp) / sizeof(double*);

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pTemp[ii] = new double[nDataSize];	
	}	
		
	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{		
		for ( jj = 0 ; jj < nSize ; jj++ )
		{
			pTemp[jj][ii] = GD_UNUSED;
		}

		if (nStart < 0)
		{
			gBasic = m_pOrgData->GetGraphData(ii + nPos);

			if (gBasic->m_iClose != GD_UNUSED)
				nStart = ii;				
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nTot)
		{
			double	tmpMax = INT_MIN, tmpMin = INT_MAX;
			int	nTmpStart = 0;

			nTmpStart = nStart + nday[0] - 1;

			for ( ii = nTmpStart ; ii < nDataSize ; ii++ )
			{
				tmpMax = INT_MIN, tmpMin = INT_MAX;

				for ( jj = 0 ; jj < nday[0] ; jj++ )
				{
					gBasic = m_pOrgData->GetGraphData(ii + nPos - jj);
					
					if (jj == 0)
						val = (double)gBasic->m_iClose;

					if (gBasic->m_iHigh > tmpMax)
						tmpMax = (double)gBasic->m_iHigh;
					
					if (gBasic->m_iLow < tmpMin)
						tmpMin = (double)gBasic->m_iLow;
				}				
				
				pTemp[0][ii] = tmpMax - tmpMin;
				pTemp[1][ii] = val - tmpMin;
			}

			double*	pFastPK = NULL;
			double*	pFastPD = NULL;
			double*	pSlowPD = NULL;
			
			if (m_iGKind == GK_STOCHF)
			{
				pFastPK = &m_apdVal[0][nPos];
				pFastPD = &m_apdVal[1][nPos];
				pSlowPD = NULL;
			}
			else if (m_iGKind == GK_STOCHS)
			{
				pFastPK = pTemp[2];
				pFastPD = &m_apdVal[0][nPos];
				pSlowPD = &m_apdVal[1][nPos];
			}
			
			nTmpStart = nStart + nday[0] - 1;

			for ( ii = nTmpStart ; ii < nDataSize ; ii++ )
			{
				if (pTemp[0][ii] == 0)
					pFastPK[ii] = 0;
				else
					pFastPK[ii] = (pTemp[1][ii] / pTemp[0][ii]) * 100.0 ;

				if (pFastPK[ii] > 100.0)
					pFastPK[ii] = 100.0;
				else if (pFastPK[ii] < 0)
					pFastPK[ii] = 0.0;	

				pFastPK[ii] *= 100.0;
			}

			if (!bOrgLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nTmpStart;
				m_aiNLimit[0] = nday[0] - 1;
			}

			nTmpStart = nStart + nday[0] - 1;

			if (m_iCOption & GC_MAWEIGHT)
				nTmpStart = CDataMgr::CalcSWMA(pFastPK, pFastPD, nDataSize, nTmpStart, nday[1]); 
			else if (m_iCOption & GC_MAEXP)
				nTmpStart = CLineData::CalcEMA(pFastPK, pFastPD, nDataSize, nTmpStart, nday[1], bLast); 
			else
				nTmpStart = CDataMgr::CalcSMA(pFastPK, pFastPD, nDataSize, nTmpStart, nday[1]);
			

			if (!bOrgLast || m_aiPIndex[1] == -1)
			{
				m_aiPIndex[1] = nTmpStart;
				m_aiNLimit[1] = nday[0] + nday[1] - 2;
			}
			
			if (m_iGKind == GK_STOCHS)
			{				
				if (!bOrgLast || m_aiPIndex[0] == -1)
				{
					m_aiPIndex[0] = nTmpStart;
					m_aiNLimit[0] = nday[0] + nday[1] - 2;
				}
				
				if (m_iCOption & GC_MAWEIGHT)
					nTmpStart = CDataMgr::CalcSWMA(pFastPD, pSlowPD, nDataSize, nTmpStart, nday[2]);
				else if (m_iCOption & GC_MAEXP)
					nTmpStart = CLineData::CalcEMA(pFastPD, pSlowPD, nDataSize, nTmpStart, nday[2], bLast);
				else
					nTmpStart = CDataMgr::CalcSMA(pFastPD, pSlowPD, nDataSize, nTmpStart, nday[2]);

				if (!bOrgLast || m_aiPIndex[1] == -1)
				{
					m_aiPIndex[1] = nTmpStart;
					m_aiNLimit[1] = nday[0] + nday[1] + nday[2] - 3;
				}
			}			
		}
	}

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		delete [] pTemp[ii];
		pTemp[ii] = NULL;
	}
TRACE("Calc Stoc End\n");
	return true;
}
