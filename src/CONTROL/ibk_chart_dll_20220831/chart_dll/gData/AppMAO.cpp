#include "stdafx.h"
#include "AppMAO.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppMAO::CAppMAO(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
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

CAppMAO::~CAppMAO()
{

}

bool CAppMAO::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = 0.0;
	CGrpBasic* gBasic = NULL;
	
	int	nsday = (int)m_pCInfo->awValue[0];
	int	nlday = (int)m_pCInfo->awValue[1];
	int	nSignal = (int)m_pCInfo->awValue[2];
	int	nDataSize = m_iDataSize;
	int	nPos = 0;
	int	nMim = 0;

	nMim = nlday;
	m_iNSignal = nSignal;

	if (nsday <= 0 || nlday <= 0 || nsday > nlday)
		return false;

	bool	bOrgLast = bLast;
	if (m_iCOption & GC_MAEXP) bLast = false;

	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0) return false;

	double*	pTemp[4];
	double* pOutput;
	int	nSize = sizeof(pTemp) / sizeof(double*);

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pTemp[ii] = new double[nDataSize];
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
		
			if (m_iGKind == GK_VMAO)
				pTemp[0][ii] = (double)gBasic->m_dVolume;
			else
				pTemp[0][ii] = (double)gBasic->m_iClose;
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		int	nn = -1;

		if (nCount >= nMim)
		{
			if (m_iCOption & GC_MAWEIGHT)
			{
				CDataMgr::CalcSWMA(pTemp[0], pTemp[1], nDataSize, nStart, nsday);
				nn = CDataMgr::CalcSWMA(pTemp[0], pTemp[2], nDataSize, nStart, nlday);
			}
			else if (m_iCOption & GC_MAEXP)
			{
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
			}
			else
			{
				CDataMgr::CalcSMA(pTemp[0], pTemp[1], nDataSize, nStart, nsday);
				nn = CDataMgr::CalcSMA(pTemp[0], pTemp[2], nDataSize, nStart, nlday);
			}
		}		

		if (nn >= 0)
		{
			pOutput = &m_apdVal[0][nPos];
			if (!bOrgLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nn;
				m_aiNLimit[0] = nMim - 1;
			}

			for ( ii = nn ; ii < nDataSize ; ii++ )
			{			
				pOutput[ii] = (pTemp[1][ii] - pTemp[2][ii]);

				if (m_pOrgData->GetUnit() == GU_CODE)
					pOutput[ii] *= 100.0;
				
				if (pTemp[1][ii])
					pOutput[ii] = pOutput[ii] / pTemp[1][ii] * 100.0;
				else
					pOutput[ii] = 0.0;

				if (m_pOrgData->GetUnit() != GU_CODE)
					pOutput[ii] *= 100.0;
			}

			nStart = nn;
		}
	}

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		delete [] pTemp[ii];
		pTemp[ii] = NULL;
	}

	return true;
}

