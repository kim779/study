#include "stdafx.h"
#include "AppLFI.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppLFI::CAppLFI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
	
	Calculate(0,0);
}

CAppLFI::~CAppLFI()
{

}

bool CAppLFI::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0;
	double	val = 0.0, pVal = 0.0;
	CGrpBasic* gBasic = NULL;
	int	nPos = 0, nDataSize = m_iDataSize;
	int	nStart = -1, nCount = 0;
	int	nday = 1;
	int	nMim = nday;


	m_iNSignal = (int)m_pCInfo->awValue[0];

	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;		
	}

	if (nPos < 0)	return false;

	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{
		gBasic = m_pOrgData->GetGraphData(ii + nPos);
		
		if (gBasic->m_iClose != GD_UNUSED)
		{
			if (nStart < 0)
			{
				nStart = ii;
				break;
			}
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nMim)
		{
			if (bLast)
			{
				pVal = m_apdVal[0][nPos - 1];

				if (pVal == GD_UNUSED)
					pVal = 0.0;
			}
			else
				pVal = 0.0;

			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				val = (double)(gBasic->m_iClose - gBasic->m_iOpen);

				if (val > 0)
					m_apdVal[0][ii + nPos] = pVal + val * 3.0;
				else
					m_apdVal[0][ii + nPos] = pVal + val * 2.0;
				
				pVal = m_apdVal[0][ii + nPos];
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

