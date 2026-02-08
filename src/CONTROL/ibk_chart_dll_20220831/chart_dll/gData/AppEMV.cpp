#include "stdafx.h"
#include "AppEMV.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppEMV::CAppEMV(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
	
	Calculate(0,0);
}

CAppEMV::~CAppEMV()
{

}

bool CAppEMV::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, nStart = -1, nCount = 0;
	int	nday = (int)m_pCInfo->awValue[0];
	double	val1 = 0.0, val2 = 0.0, sum = 0.0;
	CGrpBasic* gBasic = NULL;	
	CGrpBasic* pBasic = NULL;	
	int	nMim = nday + 1;
	
	m_iNSignal = (int)m_pCInfo->awValue[1];

	if (nday <= 0)
		return false;

	int	nPos = 0, nDataSize = m_iDataSize;

	bool	bOrgLast = bLast;
	if (m_iCOption & GC_MAEXP) bLast = false;
	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0)	return false;
	
	double *pTemp[1];
	int	nSize = sizeof(pTemp) / sizeof(double*);
	double	valMM = 0.0, valBOXR = 0.0;

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pTemp[ii] = new double[nDataSize];
	}

	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{
		gBasic = m_pOrgData->GetGraphData(ii + nPos);

		pTemp[0][ii] = GD_UNUSED;

		if (gBasic->m_iClose != GD_UNUSED)
		{	
			if (pBasic == NULL)
			{
				pBasic = gBasic;
				continue;
			}

			if (nStart < 0)
				nStart = ii;
			
			valMM = ((double)(gBasic->m_iHigh + gBasic->m_iLow) / 2.0) 
				- ((double)(pBasic->m_iHigh + pBasic->m_iLow) / 2.0);

			if (gBasic->m_iHigh == gBasic->m_iLow)
				pTemp[0][ii] = 0;
			else
			{
				valBOXR = ((double)(gBasic->m_dVolume)) 
					/ (double)(gBasic->m_iHigh - gBasic->m_iLow);

				if (valBOXR == 0.0)
					pTemp[0][ii] = 0;
				else
					pTemp[0][ii] = valMM / valBOXR * 100.0;
			}

			pBasic = gBasic;
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= (nMim - 1))
		{
			double	*pOutput = NULL;

			if (m_iCOption & GC_MAWEIGHT)
				nStart = CDataMgr::CalcSWMA(pTemp[0], &m_apdVal[0][nPos], nDataSize, nStart, nday);
			else if (m_iCOption & GC_MAEXP)
				nStart = CLineData::CalcEMA(pTemp[0], &m_apdVal[0][nPos], nDataSize, nStart, nday, bLast);
			else
				nStart = CDataMgr::CalcSMA(pTemp[0], &m_apdVal[0][nPos], nDataSize, nStart, nday);
			
			if (!bOrgLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nStart;
				m_aiNLimit[0] = nMim - 1;
			}
		}
	}

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		delete [] pTemp[ii];
		pTemp[ii] = NULL;
	}
	return true;
}
