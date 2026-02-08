#include "stdafx.h"
#include "AppRSI.h"
#include "OrgData.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppRSI::CAppRSI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
	
	Calculate(0,0);
}

CAppRSI::~CAppRSI()
{

}

bool CAppRSI::CalcuData(bool bShift, bool bLast)
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
				nStart = ii;
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nMim)
		{
			double	upSum = 0.0, dnSum = 0.0, RS = 0.0;
			nStart = nStart + nday;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				upSum = 0.0;
				dnSum = 0.0;
				for ( jj = 0 ; jj < nday ; jj++ )
				{
					gBasic = m_pOrgData->GetGraphData(ii + nPos -jj);
					val = (double)gBasic->m_iClose;
					gBasic = m_pOrgData->GetGraphData(ii + nPos - jj - 1);
					pVal = (double)gBasic->m_iClose;

					if (val > pVal)
						upSum += val - pVal;
					else	
						dnSum += pVal - val;		

				}

				if (dnSum == 0.0)
					RS = 500.0;
				else
					RS = upSum / dnSum;

				m_apdVal[0][ii + nPos] = (100.0 - (100.0 / (1.0 + RS))) * 100.0;
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

