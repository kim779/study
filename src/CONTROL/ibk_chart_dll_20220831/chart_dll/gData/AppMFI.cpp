#include "stdafx.h"
#include "AppMFI.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppMFI::CAppMFI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
		
	Calculate(0,0);
}

CAppMFI::~CAppMFI()
{

}



// 2005.12.29 by macho
// CalcuData --> 값 계산 (음수) 

#define UNUSE(a)	{ a = GD_UNUSED; }
#define USED(a)		(a != GD_UNUSED)

bool CAppMFI::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, kk = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic* gBasic = NULL;
	int	nday = (int)m_pCInfo->awValue[0];
	int	nMim = nday + 1;

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
	
	double *pTemp[2];
	int	nSize = sizeof(pTemp) / sizeof(double*);

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pTemp[ii] = new double[nDataSize];
	}

	for (ii = 0; ii < nDataSize; ii++) {
		gBasic = m_pOrgData->GetGraphData(ii + nPos);

		for (jj = 0; jj < nSize; jj++) UNUSE(pTemp[jj][ii]);

		if (USED(gBasic->m_iClose))
		{
			if (nStart < 0) nStart = ii;

			val = (gBasic->m_iHigh * 1.0 + gBasic->m_iLow + gBasic->m_iClose) / 3.0;

			if (USED(pVal)) {
				if (val > pVal) {
					pTemp[0][ii] = val * gBasic->m_dVolume;
					pTemp[1][ii] = 0;
				}
				else if (val < pVal) {
					pTemp[0][ii] = 0;
					pTemp[1][ii] = val * gBasic->m_dVolume;
				}
				else {
					pTemp[0][ii] = pTemp[1][ii] = 0;
				}
			}
			pVal = val;
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nMim)
		{
			nStart = nStart + nday;

			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				val = 0.0;
				pVal = 0.0;

				for ( jj = 0 ; jj < nday ; jj++ )  
				{
					if ((pVal != GD_UNUSED) && (val != GD_UNUSED)) {
						pVal += pTemp[0][ii - jj];
						val += pTemp[1][ii - jj];
					}
				}

				if (val != 0)
				{
					val = pVal / val;
					
					if (1.0 + val)
						m_apdVal[0][ii + nPos] = 100.0 - 100.0 / (1.0 + val);
					else
						m_apdVal[0][ii + nPos] = 0.0;
				}
				else
					m_apdVal[0][ii + nPos] = 0.0;

				m_apdVal[0][ii + nPos] *= 100.0;

			}

			if (!bLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nStart;
				m_aiPIndex[0] = nMim - 1;
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


