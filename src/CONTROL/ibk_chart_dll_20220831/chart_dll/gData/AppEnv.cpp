#include "stdafx.h"
#include "AppEnv.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppEnv::CAppEnv(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 3;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	Calculate(0,0);
}

CAppEnv::~CAppEnv()
{
}

bool CAppEnv::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nSize = 0, nStart = -1, nCount = 0;
	CGrpBasic* gBasic = NULL;
	int	nday = (int)m_pCInfo->awValue[0];
	double	nmulti = (double)m_pCInfo->awValue[1] / 100.0;
	int	nPos = 0, nDataSize = m_iDataSize;
	int	nMim = nday;
	
	if (nday <= 0)
		return false;

	bool	bOrgLast = bLast;
	bLast = false;
	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0)	return false;

	double *pTemp[1];
	double	val = 0;

	nSize = sizeof(pTemp) / sizeof(double*);

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

			pTemp[0][ii] = (double)gBasic->m_iClose;
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nMim)
		{
			if (m_iCOption & GC_MAWEIGHT)
				nStart = CDataMgr::CalcSWMA(pTemp[0], &m_apdVal[1][nPos], nDataSize, nStart, nday);
			else if (m_iCOption & GC_MAEXP)
				nStart = CLineData::CalcEMA(pTemp[0], &m_apdVal[1][nPos], nDataSize, nStart, nday, bLast);
			else
				nStart = CDataMgr::CalcSMA(pTemp[0], &m_apdVal[1][nPos], nDataSize, nStart, nday);
		
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				val = m_apdVal[1][ii+nPos];
			
				if (val == GD_UNUSED)
					continue;

				m_apdVal[0][ii + nPos] = val * double(1 + nmulti);
				m_apdVal[2][ii + nPos] = val * double(1 - nmulti);
			}

			if (!bOrgLast)
			{
				for ( ii = 0 ; ii < m_iNLine ; ii++ )
				{
					m_aiPIndex[ii] = nStart;
					m_aiNLimit[ii] = nMim - 1;
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

