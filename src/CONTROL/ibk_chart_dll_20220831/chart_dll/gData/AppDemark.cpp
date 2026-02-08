#include "stdafx.h"
#include "AppDemark.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 데이터 순서
// 0 : 목표고가 
// 1 : 목표저가

CAppDemark::CAppDemark(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 2;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	Calculate(0,0);
}

CAppDemark::~CAppDemark()
{
}

bool CAppDemark::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nSize = 0, nStart = -1, nCount = 0;
	CGrpBasic* gBasic = NULL;
	CGrpBasic* pBasic = NULL;
	int	nPos = 0, nDataSize = m_iDataSize;
	double	val = 0;
	int	nday = 1;

	int	nMim = nday + 1;

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
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);

				if (ii == nStart)
				{
					pBasic = gBasic;
					continue;
				}

				if (pBasic->m_iOpen > pBasic->m_iClose)
					val = (double)(pBasic->m_iHigh + pBasic->m_iLow + pBasic->m_iClose + pBasic->m_iLow) / 2.0;
				else if (pBasic->m_iOpen < pBasic->m_iClose)
					val = (double)(pBasic->m_iHigh + pBasic->m_iLow + pBasic->m_iClose + pBasic->m_iHigh) / 2.0;
				else
					val = (double)(pBasic->m_iHigh + pBasic->m_iLow + pBasic->m_iClose + pBasic->m_iClose) / 2.0;
			
				m_apdVal[0][ii + nPos] = val - (double)pBasic->m_iLow;
				m_apdVal[1][ii + nPos] = val - (double)pBasic->m_iHigh;
				
				pBasic = gBasic;
			}

			nStart += 1;
			
			if (!bLast)
			{
				for ( ii = 0 ; ii < m_iNLine ; ii++ )
				{
					m_aiPIndex[ii] = nStart;
					m_aiNLimit[ii] = nMim - 1;
				}
			}
		}
	}
	
	return true;
}
