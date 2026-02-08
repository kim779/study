#include "stdafx.h"
#include "AppMmt.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppMmt::CAppMmt(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
	
	Calculate(0,0);
}

CAppMmt::~CAppMmt()
{

}

bool CAppMmt::CalcuData(bool bShift, bool bLast)
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
#if 0 // 우리회사 코드
			nStart = nStart + nday;
			double	nMax = 0.0, nMin = 0.0;

			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				nMax = DBL_MIN;
				nMin = DBL_MAX;			
				
				for ( jj = 0 ; jj < nday ; jj++ )  
				{
					gBasic = (struct _gBasic *)m_pOrgData->GetGraphData((ii + nPos) - jj - 1);

					val = (double)gBasic->m_iClose;

					if (gBasic->hpr > nMax)
						nMax = gBasic->hpr;
					if (gBasic->lpr < nMin)
						nMin = gBasic->lpr;
				}

				gBasic = (struct _gBasic *)m_pOrgData->GetGraphData((ii + nPos) - nday);
				pVal = (double)gBasic->m_iClose;
				gBasic = (struct _gBasic *)m_pOrgData->GetGraphData((ii + nPos));
				val = (double)gBasic->m_iClose;

				if (nMin == nMax)
					m_apdVal[0][ii + nPos] = 0.0;
				else
					m_apdVal[0][ii + nPos] = (val - pVal) * 100.0 / (nMax - nMin);
			}

#elif 0 // 대우코드
			nStart = nStart + nday;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = (struct _gBasic *)m_pOrgData->GetGraphData((ii + nPos) - nday);
				pVal = (double)gBasic->m_iClose;
				gBasic = (struct _gBasic *)m_pOrgData->GetGraphData((ii + nPos));
				val = (double)gBasic->m_iClose;

				m_apdVal[0][ii + nPos] = val - pVal;
			}
#elif 1 // 대신코드
					nStart = nStart + nday;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData((ii + nPos) - nday);
				pVal = (double)gBasic->m_iClose;
				gBasic = m_pOrgData->GetGraphData((ii + nPos));
				val = (double)gBasic->m_iClose;

				if (pVal == 0)
					m_apdVal[0][ii + nPos] = 0;
				else
					m_apdVal[0][ii + nPos] = (val/pVal) * 100.0 * 100.0;
			}

#endif

			if (!bLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nStart;
				m_aiNLimit[0] = nMim - 1;
			}
		}
	}

	return true;
}

