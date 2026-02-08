#include "stdafx.h"
#include "AppRate.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppRate::CAppRate(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}	
	
	Calculate(0,0);
}

CAppRate::~CAppRate()
{

}

bool CAppRate::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;

	int	ii = 0, jj = 0, kk = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic* gBasic = NULL;
	int	nPos = 0, nDataSize = m_iDataSize;

	m_iNSignal = (int)m_pCInfo->awValue[0];

	if (bLast)
	{
		nDataSize = 1;
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
		double	stocks = m_pOrgData->GetStocks();

		if (stocks <= 0.0)
			return false;

		for ( ii = nStart ; ii < nDataSize ; ii++ )
		{
			gBasic = m_pOrgData->GetGraphData(ii + nPos);
			val = (double)gBasic->m_dVolume;
			m_apdVal[0][ii + nPos] = (val * 300.0 / stocks) * 100.0;
		}			

		if (!bLast || m_aiPIndex[0] == -1)
			m_aiPIndex[0] = nStart;		
	}

	return true;
}
