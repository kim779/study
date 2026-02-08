#include "stdafx.h"
#include "AppParabolic.h"
#include "OrgData.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"
#include <float.h>
#include <math.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppParabolic::CAppParabolic(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	m_tendency = tendencyNO;
	Calculate(0,0);
}

CAppParabolic::~CAppParabolic()
{

}

bool CAppParabolic::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int ii = 0, nStart = -1;
	CGrpBasic* gBasic = NULL;
	CGrpBasic* pBasic = NULL;
	double	maxAF = m_pCInfo->awValue[0] / 100.0;
	double	af = m_pCInfo->awValue[1] / 100.0;
	int	nPos = 0, nDataSize = m_iDataSize;

	for (ii = 0 ; ii < nDataSize ; ii++ )
	{
		if (nStart < 0)
		{
			gBasic = m_pOrgData->GetGraphData(ii + nPos);
			if (gBasic->m_iClose != GD_UNUSED)
				nStart = ii;
		}
	}

	if (nStart < 0)
		return false;

	if ((nDataSize - nStart) >= 2)
	{
		int	tendUP = 0, tendDN = 0;	
		double	SAR, pSAR = 0;
		int	SARIndex = 0, EP;// 극단가격
		
		for ( ii = nStart ; ii < nDataSize ; ii++ )
		{
			gBasic = m_pOrgData->GetGraphData(ii);

			if (ii == nStart)
			{
				pBasic = gBasic;
				continue;
			}
				
			if (pBasic->m_iClose >= gBasic->m_iClose)
			{
				tendUP++; tendDN = 0;

				if (tendUP >= 2)
				{
					m_tendency = tendencyUP;
					break;
				}
			}
			else
			{
				tendDN++; tendUP = 0;

				if (tendDN >= 2)
				{
					m_tendency = tendencyDN;
					break;
				}
			}

			pBasic = gBasic;
		}
			
		if (ii >= nDataSize)
			return false;

		SARIndex = ii - 1;
		SAR = pSAR = (m_tendency == tendencyUP) ? INT_MAX : INT_MIN;

		for ( ii = 0; ii < SARIndex ; ii++ )
		{
			gBasic = m_pOrgData->GetGraphData(ii);

			if (m_tendency == tendencyUP)
			{
				if (gBasic->m_iLow > 0 && gBasic->m_iLow < SAR)
				{
					pSAR = SAR = gBasic->m_iLow;
					EP = gBasic->m_iHigh;
				}
			}
			else if (m_tendency == tendencyDN)
			{
				if (gBasic->m_iHigh > 0 && gBasic->m_iHigh > SAR)
				{
					pSAR = SAR = gBasic->m_iHigh;
					EP = gBasic->m_iLow;
				}
			}
		}				
			
		int	pEP = EP;
		double	aFactor = 0.02;
		m_apdVal[0][SARIndex + nPos] = pSAR + aFactor*(EP - pSAR);
		for (ii = SARIndex + 1 ; ii < nDataSize ; ii++ )
		{
			pSAR = SAR;
			SAR = pSAR + aFactor*(EP - pSAR);
			m_apdVal[0][ii + nPos] = SAR;
			pEP = EP;

			gBasic = m_pOrgData->GetGraphData(ii + nPos);
			if (m_tendency == tendencyUP)	// 상승추세
			{
				if (SAR > gBasic->m_iLow)// 하락전환
				{
					SAR = EP;
					m_apdVal[0][ii + nPos] = SAR;
					EP = gBasic->m_iLow;
					pEP = EP;
					aFactor = 0.02;
					m_tendency = tendencyDN;
				}
				else if (gBasic->m_iHigh > pEP)
				{
			                EP = gBasic->m_iHigh;
			                aFactor = min(aFactor+af, maxAF);
				}
			}
			else	// 하락추세
			{
				if(SAR < gBasic->m_iHigh) // 추세 전환
				{							
					SAR = EP; 
					m_apdVal[0][ii+nPos] = SAR;
					EP = gBasic->m_iHigh;
					pEP = EP;
					aFactor = 0.02;
					m_tendency = tendencyUP;
				}
				else if(gBasic->m_iLow < pEP)
				{
					EP = gBasic->m_iLow;
					aFactor = min(aFactor+af, maxAF);
				}
			}
		}
	}
	
	return true;
}
