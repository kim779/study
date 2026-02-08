#include "stdafx.h"
#include "AppADLine.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppADLine::CAppADLine(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	Calculate(0,0);
}

CAppADLine::~CAppADLine()
{

}

bool CAppADLine::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nStart = -1, nCount = 0;
	double	val1 = 0.0, val2 = 0.0, sum = 0.0;
	CGrpBasic* gBasic = NULL;
	int	nPos = 0, nDataSize = m_iDataSize;
	int	nday = 1;

	m_iNSignal = (int)m_pCInfo->awValue[0];

	if (bLast)
	{
		nDataSize = nday;
		nPos = m_iDataSize - nDataSize;		
	}

	if (nPos < 0)
		return false;
	
	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{
		gBasic = m_pOrgData->GetGraphData(ii + nPos);		

		if (gBasic->m_iClose == GD_UNUSED)
			continue;

		if (bLast)
		{
			if ((ii + nPos) >= 0)
			{
				sum = m_apdVal[0][ii + nPos - 1];

				if (sum == GD_UNUSED)
					sum = 0.0;
			}
		}
		else
		{
			if (m_aiPIndex[0] < 0)
				m_aiPIndex[0] = ii;		
		}
		
		
		val1 = (double)((gBasic->m_iClose - gBasic->m_iLow) - (gBasic->m_iHigh - gBasic->m_iClose));
		val2 = (double)gBasic->m_iHigh - (double)gBasic->m_iLow;

		if (val2 != 0.0)
			sum += (val1 / val2) * (double)gBasic->m_dVolume;
	
		m_apdVal[0][ii + nPos] = sum;
	}

	return true;
}


