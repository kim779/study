#include "stdafx.h"
#include "AppCV.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppCV::CAppCV(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
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

CAppCV::~CAppCV()
{

}

bool CAppCV::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nStart = -1, nCount = 0;
	int	nday = (int)m_pCInfo->awValue[0];
	double	val1 = 0.0, val2 = 0.0, sum = 0.0;
	CGrpBasic* gBasic = NULL;	
	int	nMim = 2 * nday;

	m_iNSignal = (int)m_pCInfo->awValue[1];

	if (nday <= 0)
		return false;

	int	nPos = 0, nDataSize = m_iDataSize;

	bool	bOrgLast = bLast;
	if (m_iCOption & GC_MAEXP)
		bLast = false;
	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0)	return false;

	double*	pTemp[2];
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
						
			val1 = double(gBasic->m_iHigh - gBasic->m_iLow);

			pTemp[0][ii] = val1;
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;
		double	nVal = 0;

		if (nCount >= nMim)
		{			
			if (m_iCOption & GC_MAWEIGHT)
				nStart = CDataMgr::CalcSWMA(pTemp[0], pTemp[1], nDataSize, nStart, nday);				
			else if (m_iCOption & GC_MAEXP)
			{
				if (bLast)
				{
					if (bShift)
						m_prevVal[0] = m_prevVal[1];
					
					pTemp[1][nday - 2] = m_prevVal[0];
					nStart = CLineData::CalcEMA(pTemp[0], pTemp[1], nDataSize, nStart, nday, bLast);
					m_prevVal[1] = pTemp[1][nStart];
				}
				else
				{
					nStart = CLineData::CalcEMA(pTemp[0], pTemp[1], nDataSize, nStart, nday, bLast);
					
					m_prevVal[0] = pTemp[1][nDataSize - nMim + (nday - 2)];
					m_prevVal[1] = pTemp[1][nDataSize - nMim + (nday - 1)];
				}
			}
			else
				nStart = CDataMgr::CalcSMA(pTemp[0], pTemp[1], nDataSize, nStart, nday);

			nStart = nStart + nday - 1;			
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				val1 = pTemp[1][ii];
				val2 = pTemp[1][ii - nday + 1];

				if (val1 == GD_UNUSED || val2 == GD_UNUSED)
					continue;

				if (val2)
					sum = (val1 - val2) / val2 * 100.0;

				m_apdVal[0][ii + nPos] = sum * 100.0;
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


