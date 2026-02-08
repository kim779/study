#include "stdafx.h"
#include "AppDMI.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppDMI::CAppDMI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	if (m_iGKind == GK_ADX)
	{
		m_iNLine = 1;

		for ( int ii = 0 ; ii < m_iNLine ; ii++ )
		{
			m_apdVal[ii] = new double[m_iDataSize];
		}
	}
	else
	{
		m_iNLine = 2;

		for ( int ii = 0 ; ii < m_iNLine ; ii++ )
		{
			m_apdVal[ii] = new double[m_iDataSize];
		}
	}	
	
	memset(m_prevVal, 0x00, sizeof(m_prevVal));
	Calculate(0,0);
}

CAppDMI::~CAppDMI()
{

}

bool CAppDMI::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nSize = 0, nStart = -1, nCount = 0;
	CGrpBasic* gBasic = NULL;
	CGrpBasic* pBasic = NULL;
	int	nday = (int)m_pCInfo->awValue[0];
	int	nMim = 0;
	int	nPos = 0, nDataSize = m_iDataSize;

	if (m_iGKind == GK_ADX)	
	{
		m_iNSignal = (int)m_pCInfo->awValue[1];
		nMim = 2 * nday;
	}
	else
	{
		nMim = nday + 1;
		m_iNSignal = 0;
	}
	
	if (nday <= 0)
		return false;

	bool	bOrgLast = bLast;
	if (m_iCOption & GC_MAEXP) bLast = false;
	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0) return false;
	
	double *pTemp[9];
	double	tr1 = 0.0, tr2 = 0.0, tr3 = 0.0;
	double	PDM = 0.0, MDM = 0.0, TR = 0.0;
	double	*pOutput[3];
	
	nSize = sizeof(pTemp) / sizeof(double*);

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pTemp[ii] = new double[m_iDataSize];
	}	 
		
	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{
		gBasic = m_pOrgData->GetGraphData(ii + nPos);

		for ( jj = 0 ; jj < nSize ; jj++ )
		{
			pTemp[jj][ii] = GD_UNUSED;
		}
		
		if (gBasic->m_iClose != GD_UNUSED)
		{
			if (nStart < 0)
				nStart = ii;			
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nMim)
		{
			int	nn = 0;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);

				if (ii == nStart)
				{
					pBasic = gBasic;
					continue;
				}
				
				double P = gBasic->m_iHigh - pBasic->m_iHigh;
				double M = pBasic->m_iLow - gBasic->m_iLow;

				PDM = (P > 0 && P > M) ? P : 0;
				MDM = (M > 0 && P < M) ? M : 0;

				tr1 = fabs(gBasic->m_iHigh - gBasic->m_iLow);
				tr2 = fabs(gBasic->m_iHigh - pBasic->m_iClose);
				tr3 = fabs(gBasic->m_iLow - pBasic->m_iClose);
				tr1 = max(tr1, tr2);
				TR = max(tr1, tr3);

				pTemp[0][ii] = PDM;
				pTemp[1][ii] = MDM;
				pTemp[2][ii] = TR;

				pBasic = gBasic;
			}

			nStart += 1;

			for ( ii = 0 ; ii < 3 ; ii++ )
			{
				if (m_iCOption & GC_MAWEIGHT)
					nn = CDataMgr::CalcSWMA(pTemp[ii], pTemp[ii + 3], nDataSize, nStart, nday);
				else if (m_iCOption & GC_MAEXP)
				{					
					if (bLast)
					{
						if (bShift)
							m_prevVal[ii * 2] = m_prevVal[ii * 2 + 1];
						
						pTemp[ii + 1][nday - 2] = m_prevVal[ii * 2];						
						nn = CLineData::CalcEMA(pTemp[ii], pTemp[ii + 3], nDataSize, nStart, nday, bLast);
						m_prevVal[ii * 2 + 1] = pTemp[ii + 1][nn];						
					}
					else
					{	
						nn = CLineData::CalcEMA(pTemp[ii], pTemp[ii + 3], nDataSize, nStart, nday, bLast);
						m_prevVal[ii * 2] = pTemp[ii + 1][nDataSize - nMim + (nday - 2)];
						m_prevVal[ii * 2 + 1] = pTemp[ii + 1][nDataSize - nMim + (nday - 1)];
					}					
				}
					
				else
					nn = CDataMgr::CalcSMA(pTemp[ii], pTemp[ii + 3], nDataSize, nStart, nday);
				
			}

			if (m_iGKind == GK_ADX)
			{
				pOutput[0] = pTemp[6];
				pOutput[1] = pTemp[7];
				pOutput[2] = pTemp[8];
			}
			else
			{
				if (!bOrgLast  || m_aiPIndex[0] == -1)
				{
					m_aiPIndex[0] = m_aiPIndex[1] = nn;
					m_aiNLimit[0] = m_aiNLimit[1] = nMim;
				}
				pOutput[0] = &m_apdVal[0][nPos];
				pOutput[1] = &m_apdVal[1][nPos];
				pOutput[2] = NULL;
			}			
			
			for ( ii = nn ; ii < nDataSize ; ii++ )
			{
				if (pTemp[5][ii] == 0.0)
				{
					pOutput[0][ii] = 100.0;
					pOutput[1][ii] = 100.0;
				}
				else
				{
					pOutput[0][ii] = (pTemp[3][ii] * 100.0 * 100.0) / pTemp[5][ii];
					pOutput[1][ii] = (pTemp[4][ii] * 100.0 * 100.0) / pTemp[5][ii];
				}				
			}

			nStart = nn;

			if (m_iGKind == GK_ADX)
			{
				nCount = nDataSize - nStart;

				if (nCount >= nday)
				{
					double	sDI = 0.0, dDI = 0.0;

					for ( ii = nn ; ii < nDataSize ; ii++ )
					{
						dDI = fabs(pOutput[0][ii] - pOutput[1][ii]);
						sDI = pOutput[0][ii] + pOutput[1][ii];

						if (!sDI)
							pOutput[2][ii] = 100.0 * 100.0;
						else
							pOutput[2][ii] = (dDI / sDI) * 100.0 * 100.0;
					}
				
					if (m_iCOption & GC_MAWEIGHT)
						nn = CDataMgr::CalcSWMA(pOutput[2], &m_apdVal[0][nPos], nDataSize, nStart, nday);
					else if (m_iCOption & GC_MAEXP)
						nn = CLineData::CalcEMA(pOutput[2], &m_apdVal[0][nPos], nDataSize, nStart, nday, bLast);
					else
						nn = CDataMgr::CalcSMA(pOutput[2], &m_apdVal[0][nPos], nDataSize, nStart, nday);

					if (!bOrgLast || m_aiPIndex[0] == -1)
					{
						m_aiPIndex[0] = nn;
						m_aiNLimit[0] = nMim;
					}
				}
			}
		}
	}

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		delete [] pTemp[ii];
		pTemp[ii] = NULL;
	}	
	return true;
}

