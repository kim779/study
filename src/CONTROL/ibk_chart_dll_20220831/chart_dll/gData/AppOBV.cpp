#include "stdafx.h"
#include "AppOBV.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppOBV::CAppOBV(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	Calculate(0,0);
}

CAppOBV::~CAppOBV()
{

}

bool CAppOBV::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, kk = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic* gBasic = NULL;
	int	nPos = 0, nDataSize = m_iDataSize;
	int	nday = 1;
	int	nMim = nday + 1;

	m_iNSignal = (int)m_pCInfo->awValue[0];

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
			double	USum = 0.0, DSum = 0.0, trn = 0.0;

			nStart = nStart + nday;			

			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				val = (double)gBasic->m_iClose;

				trn = (double)gBasic->m_dVolume;

				gBasic = m_pOrgData->GetGraphData(ii + nPos- nday);
				pVal = (double)gBasic->m_iClose;	

				if (val > pVal)
					USum += trn;
				else if (val < pVal)
					DSum += trn;				
				
				if (!bLast)
					m_apdVal[0][ii + nPos] = USum - DSum;
				else
				{
					pVal = m_apdVal[0][ii + nPos - 1];

					if (pVal == GD_UNUSED)
						m_apdVal[0][ii + nPos] = USum - DSum;
					else
						m_apdVal[0][ii + nPos] = pVal + USum - DSum;
				}
			}
			
			if (bLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nStart;
				m_aiNLimit[0] = nMim -1;
			}
			
		}
	}

	return true;
}

