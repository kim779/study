#include "stdafx.h"
#include "AppVR.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppVR::CAppVR(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	Calculate(0,0);
}

CAppVR::~CAppVR()
{

}

bool CAppVR::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, kk = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic* gBasic = NULL;
	int	nday = (int)m_pCInfo->awValue[0];
	int	nMim = nday + 1;
	int	nPos = 0, nDataSize = m_iDataSize;

	m_iNSignal = (int)m_pCInfo->awValue[1];

	if (nday <= 0)
		return false;

	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0) return false;
		
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
			nStart = nStart + nday;

			double	UpSum = 0.0, DnSum = 0.0, EqSum = 0.0;
			double	trn = 0.0;

			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				EqSum = DnSum = UpSum = 0.0;
				
				for ( jj = 0 ; jj < nday ; jj++ )
				{
					gBasic = m_pOrgData->GetGraphData(ii + nPos - jj);
					val = (double)gBasic->m_iClose;
					trn = (double)gBasic->m_dVolume;
					gBasic = m_pOrgData->GetGraphData(ii + nPos - jj - 1);
					pVal = (double)gBasic->m_iClose;						
					
					if (val > pVal)
						UpSum += trn;
					else if (val < pVal)
						DnSum += trn;
					else
						EqSum = trn;
				}

				UpSum += (EqSum * 0.5);
				DnSum += (EqSum * 0.5);
				
				if (DnSum == 0.0)
				{
					m_apdVal[0][ii + nPos] = 0.0;
				}
				else 
					m_apdVal[0][ii + nPos] = (UpSum / DnSum) * 100.0 * 100.0;				
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
