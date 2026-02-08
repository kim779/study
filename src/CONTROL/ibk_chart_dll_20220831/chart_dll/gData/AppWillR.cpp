#include "stdafx.h"
#include "AppWillR.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppWillR::CAppWillR(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
	
	Calculate(0,0);
}

CAppWillR::~CAppWillR()
{

}

bool CAppWillR::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, kk = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic* gBasic = NULL;
	int	nday = (int)m_pCInfo->awValue[0];
	int	nMim = nday;

	m_iNSignal = (int)m_pCInfo->awValue[1];

	if (nday <= 0)
		return false;

	int	nPos = 0, nDataSize = m_iDataSize;

	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0)	return false;
	
	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{		
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

		if (nCount >= nMim)
		{
			nStart = nStart + nday - 1;

			double	tmpMax = DBL_MIN, tmpMin = DBL_MAX;

			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				tmpMax = DBL_MIN, tmpMin = DBL_MAX;

				for ( jj = 0 ; jj < nday ; jj++ )
				{
					gBasic = m_pOrgData->GetGraphData(ii + nPos - jj);
					
					val = gBasic->m_iHigh;
					pVal = gBasic->m_iLow;

					if (val > tmpMax)
						tmpMax = val;

					if (pVal < tmpMin)
						tmpMin = pVal;
				}

				gBasic = m_pOrgData->GetGraphData(ii + nPos);

				if (tmpMin == tmpMax)
					m_apdVal[0][ii + nPos] = -100.0;
				else
					m_apdVal[0][ii + nPos] = (tmpMax - (double)gBasic->m_iClose) / (double)(tmpMax - tmpMin) * -100.0;

				if (m_apdVal[0][ii + nPos] > 0)
					m_apdVal[0][ii] = 0.0;
				else if (m_apdVal[0][ii + nPos] < -100.0)
					m_apdVal[0][ii + nPos] = -100.0;

				m_apdVal[0][ii + nPos] *= 100.0;
			}			

			if (!bLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nStart;	
				m_aiNLimit[0] = nMim - 1;
			}
		}
	}

	return true;
}
