// AppBandWidth.cpp: implementation of the CAppBandWidth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppBandWidth.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppBandWidth::CAppBandWidth(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	m_apdVal[0] = new double[m_iDataSize];
	for ( int ii = 0 ; ii < 3 ; ii++ )
	{
		m_pBollinger[ii] = new double[m_iDataSize];
	}
	
	Calculate(0,0);
}

CAppBandWidth::~CAppBandWidth()
{
	for ( int ii = 0 ; ii < 3 ; ii++ )
	{
		delete m_pBollinger[ii];
	}
}

bool CAppBandWidth::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0) {
		return false;
	}
	
	int	ii = 0, jj = 0, nSize = 0, nStart = -1, nCount = 0;
	CGrpBasic*	gBasic = NULL;
	int	nday = (int)m_pCInfo->awValue[0];
	int	nmulti = (int)m_pCInfo->awValue[1];
	int	nPos = 0, nDataSize = m_iDataSize;
	int	nMim = nday;

	m_iNSignal = (int)m_pCInfo->awValue[2];

	if (nday <= 0) {
		return false;
	}

	bool	bOrgLast = bLast;
	bLast = false;
	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0) {
		return false;
	}

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
				nStart = CDataMgr::CalcSWMA(pTemp[0], &m_pBollinger[1][nPos], nDataSize, nStart, nday); 
			else if (m_iCOption & GC_MAEXP)
				nStart = CLineData::CalcEMA(pTemp[0], &m_pBollinger[1][nPos], nDataSize, nStart, nday, bLast);
			else
				nStart = CDataMgr::CalcSMA(pTemp[0], &m_pBollinger[1][nPos], nDataSize, nStart, nday);
			
			double	deflect = 0.0, deviation = 0.0;

			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				val = m_pBollinger[1][ii+nPos];
			
				if (val == GD_UNUSED)
					continue;

				deflect = 0.0;

				for ( jj = 0  ; jj < nday ; jj++ )
				{			
					deviation = pow(pTemp[0][ii - jj] - val, 2.0);
					deviation /= double(nday);
					deflect += deviation;
				}

				deflect = sqrt(deflect);

				m_pBollinger[0][ii + nPos] = val + double(nmulti) * deflect;
				m_pBollinger[2][ii + nPos] = val - double(nmulti) * deflect;
				if (m_pBollinger[1][ii + nPos] == 0)
					m_apdVal[0][ii + nPos] = 0;
				else
					m_apdVal[0][ii + nPos] = ((m_pBollinger[0][ii + nPos] - m_pBollinger[2][ii + nPos])*10000.0) / m_pBollinger[1][ii + nPos];
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
