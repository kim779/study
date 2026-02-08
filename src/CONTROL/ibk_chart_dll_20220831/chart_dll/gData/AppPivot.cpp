#include "stdafx.h"
#include "AppPivot.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"
#include <math.h>
#include <float.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 데이터 순서
// 0 : Pivot
// 1 : 1차 저항 
// 2 : 1차 지지
// 3 : 2차 저항 
// 4 : 2차 지지

CAppPivot::CAppPivot(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 5;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	Calculate(0,0);
}

CAppPivot::~CAppPivot()
{
}

bool CAppPivot::CalcuData(bool bShift, bool bLast)
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
				pBasic = m_pOrgData->GetGraphData(ii + nPos);

				if (ii == nStart)
				{
					gBasic = pBasic;
					continue;
				}
				
				val = m_apdVal[0][ii + nPos] = (double)(gBasic->m_iClose + gBasic->m_iHigh + gBasic->m_iLow) / 3.0;
				m_apdVal[1][ii + nPos] = val * 2.0 - (double)gBasic->m_iLow;
				m_apdVal[2][ii + nPos] = val * 2.0 - (double)gBasic->m_iHigh;
				m_apdVal[3][ii + nPos] = val + (double)gBasic->m_iHigh - (double)gBasic->m_iLow;
				m_apdVal[4][ii + nPos] = val - (double)gBasic->m_iHigh + (double)gBasic->m_iLow;					

				gBasic = pBasic;
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
