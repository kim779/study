#include "stdafx.h"
#include "AppNCO.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppNCO::CAppNCO(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
	
	Calculate(0,0);
}

CAppNCO::~CAppNCO()
{

}

bool CAppNCO::CalcuData(bool bShift, bool bLast)
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

	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{		
		if (nStart < 0)
		{
			gBasic = m_pOrgData->GetGraphData(ii + nPos);

			if (gBasic->m_iClose != GD_UNUSED)
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
			nStart = nStart + nday;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				val = (double)gBasic->m_iClose;
				gBasic = m_pOrgData->GetGraphData(ii + nPos - nday);
				pVal = (double)gBasic->m_iClose;
				m_apdVal[0][ii + nPos] = val - pVal;
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

