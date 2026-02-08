#include "stdafx.h"
#include "AppABRatio.h"
#include "OrgData.h"
//#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppABRatio::CAppABRatio(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 2;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	Calculate(0,0);
}

CAppABRatio::~CAppABRatio()
{
}

bool CAppABRatio::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nSize = 0, nStart = -1, nCount = 0;
	CGrpBasic*	gBasic = NULL;
	CGrpBasic*	pBasic = NULL;
	int	nPos = 0, nDataSize = m_iDataSize;
	double	val = 0;
	int	nday1 = (int)m_pCInfo->awValue[0];
	int	nday2 = (int)m_pCInfo->awValue[1];
	int	nday = ((nday1 > nday2) ? nday1 : nday2);
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
			double	fPar = 0, fChi = 0.0;
			int	nn = nStart;
			
			nStart = nn + nday1 - 1;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				fPar = 0, fChi = 0.0;
				
				for ( jj = 0 ; jj < nday1 ; jj++ )
				{
					gBasic = m_pOrgData->GetGraphData(ii + nPos - jj);			
					
					fChi += (double)(gBasic->m_iHigh - gBasic->m_iOpen);
					fPar += (double)(gBasic->m_iOpen - gBasic->m_iLow);					
				}

				if (fPar == 0)
					m_apdVal[0][ii + nPos] = 0.0;
				else
					m_apdVal[0][ii + nPos] = (fChi / fPar) * 100.0;

				m_apdVal[0][ii + nPos] *= 100.0;
			}

			if (!bLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nStart;
				m_aiNLimit[0] = nday1 - 1;
			}

			nStart = nn + nday2;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				fPar = 0, fChi = 0.0;
				
				for ( jj = 0 ; jj < nday2 ; jj++ )
				{
					gBasic = m_pOrgData->GetGraphData(ii + nPos - jj);
					pBasic = m_pOrgData->GetGraphData(ii + nPos - jj - 1);			
					
					fChi += (double)(gBasic->m_iHigh - pBasic->m_iClose);
					fPar += (double)(pBasic->m_iClose - gBasic->m_iLow);					
				}

				if (fPar == 0)
					m_apdVal[1][ii + nPos] = 0.0;
				else
					m_apdVal[1][ii + nPos] = (fChi / fPar) * 100.0;

				m_apdVal[1][ii + nPos] *= 100.0;
			}
			
			if (!bLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[1] = nStart;
				m_aiNLimit[1] = nday2;
			}
		}
	}
	
	return true;
}
