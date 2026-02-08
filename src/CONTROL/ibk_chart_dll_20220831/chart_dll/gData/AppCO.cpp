#include "stdafx.h"
#include "AppCO.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppCO::CAppCO(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}	

	memset(m_prevVal, 0x00, sizeof(m_prevVal));
	Calculate(0,0);	
}

CAppCO::~CAppCO()
{

}

bool CAppCO::CalcuData(bool bShift, bool bLast)
{

	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nStart = -1, nCount = 0;
	int	nsday = (int)m_pCInfo->awValue[0];
	int	nlday = (int)m_pCInfo->awValue[1];
	double	val1 = 0.0, val2 = 0.0, sum = 0.0;
	CGrpBasic* gBasic = NULL;	
	int	nMim = nlday;

	m_iNSignal = (int)m_pCInfo->awValue[2];

	if (nsday <= 0 || nlday <= 0 || nlday <= nsday)
		return false;

	int	nPos = 0, nDataSize = m_iDataSize;

	bool	bOrgLast = bLast;
	if (m_iCOption & GC_MAEXP) bLast = false;
	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;

		if (bShift)
			m_prevVal[4] = m_prevVal[5];

		sum = m_prevVal[4];
	}

	if (nPos < 0)	return false;

	double*	pTemp[3];
	int	nSize = sizeof(pTemp) / sizeof(double*);

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pTemp[ii] = new double[nDataSize];
	}
	
	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{
		for ( jj = 0 ; jj  < nSize ; jj++ )
		{
			pTemp[jj][ii] = GD_UNUSED;
		}

		gBasic = m_pOrgData->GetGraphData(ii + nPos);
		
		if (gBasic->m_iClose != GD_UNUSED)
		{			
			if (nStart < 0)
				nStart = ii;
						
			val1 = double(gBasic->m_iClose - gBasic->m_iLow) - double(gBasic->m_iHigh - gBasic->m_iClose);
			val2 = double(gBasic->m_iHigh - gBasic->m_iLow);

			if (val2 != 0)
			{
				sum += ((val1 / val2) * (double)gBasic->m_dVolume);
			}
		
			pTemp[0][ii] = sum;
		}
	}

	if (!bLast)
	{
		if (nDataSize - nMim - 1 >= 0)
		{
			m_prevVal[4] = pTemp[0][nDataSize - nMim - 1];
			m_prevVal[5] = pTemp[0][nDataSize - nMim];
		}
		else
		{
			m_prevVal[4] = 0;
			m_prevVal[5] = 0;
		}
	}
	else
	{
		if (nStart >= 0)
			m_prevVal[5] = pTemp[0][nStart];
		else
			m_prevVal[5] = 0;
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nMim)
		{			
			if (m_iCOption & GC_MAWEIGHT)
			{
				CDataMgr::CalcSWMA(pTemp[0], pTemp[1], nDataSize, nStart, nsday);
				nStart = CDataMgr::CalcSWMA(pTemp[0], pTemp[2], nDataSize, nStart, nlday);
			}
			else if (m_iCOption & GC_MAEXP)
			{
				int	nn = 0;
				int	nTemp[2] = {nsday, nlday};

				for ( ii = 0 ; ii < 2 ; ii++ )
				{
					if (bLast)
					{
						if (bShift)
							m_prevVal[ii * 2] = m_prevVal[ii * 2 + 1];
						
						pTemp[ii + 1][nTemp[ii] - 2] = m_prevVal[ii * 2];						
						nn = CLineData::CalcEMA(pTemp[0], pTemp[ii + 1], nDataSize, nStart, nTemp[ii], bLast);
						m_prevVal[ii * 2 + 1] = pTemp[ii + 1][nn];						
					}
					else
					{				
						nn = CLineData::CalcEMA(pTemp[0], pTemp[ii + 1], nDataSize, nStart, nTemp[ii], bLast);						
						m_prevVal[ii * 2] = pTemp[ii + 1][nDataSize - nMim + (nTemp[ii] - 2)];
						m_prevVal[ii * 2 + 1] = pTemp[ii + 1][nDataSize - nMim + (nTemp[ii] - 1)];
					}
				}

				nStart = nn;
			}
			else
			{
				CDataMgr::CalcSMA(pTemp[0], pTemp[1], nDataSize, nStart, nsday);
				nStart = CDataMgr::CalcSMA(pTemp[0], pTemp[2], nDataSize, nStart, nlday);
			}

			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				val1 = pTemp[1][ii];
				val2 = pTemp[2][ii];
			
				m_apdVal[0][ii + nPos] = val1 - val2;
			}

			if (!bOrgLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nStart;
				m_aiNLimit[0] = nMim - 1;
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

