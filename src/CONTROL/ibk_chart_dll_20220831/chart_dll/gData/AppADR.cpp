// AppADR.cpp: implementation of the CAppADR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppADR.h"
#include "OrgData.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppADR::CAppADR(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
		
	Calculate(0,0);
}

CAppADR::~CAppADR()
{

}

bool CAppADR::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, kk = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic*	gBasic = NULL;
	int	nday = (int)m_pCInfo->awValue[0];
	int	nMim = 2 * nday - 1;

	m_iNSignal = (int)m_pCInfo->awValue[1];

	if (nday <= 0)
		return false;

	int	nPos = 0, nDataSize = m_iDataSize;

//	if (m_bReCalculation && (m_iCOption & GC_MAEXP))
//		bLast = false;

	if (bLast)
	{
		nDataSize = nMim;
		nPos = m_iDataSize - nDataSize;
	}

	if (nPos < 0)	return false;

	double*	pTemp = NULL;

	pTemp = new double[nDataSize];

	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{		
		pTemp[ii] = GD_UNUSED;
		
		if (nStart < 0)
		{
			gBasic = m_pOrgData->GetGraphData(ii + nPos);

			if (gBasic->m_iUpCnt != 0 && gBasic->m_iDnCnt != 0)
				nStart = ii;			
		}
	}

	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nMim)
		{
			double	upSum = 0.0, dnSum = 0.0;
			nStart = nStart + nday - 1;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				upSum = 0;
				dnSum = 0;

				for ( jj = 0 ; jj < nday ; jj++ )
				{
					gBasic = m_pOrgData->GetGraphData(ii + nPos -jj);
	
					upSum += gBasic->m_iUpCnt;
					dnSum += gBasic->m_iDnCnt;
				}

				if (dnSum == 0.0)
					val = 0.0;
				else
					val = upSum / dnSum * 100.0;

				pTemp[ii] = val;
			}	
			
			if (m_iCOption & GC_MAWEIGHT)
				nStart = CDataMgr::CalcSWMA(pTemp, &m_apdVal[0][nPos], nDataSize, nStart, nday); 
			else if (m_iCOption & GC_MAEXP)
				nStart = CLineData::CalcEMA(pTemp, &m_apdVal[0][nPos], nDataSize, nStart, nday, bLast); 
			else
				nStart = CDataMgr::CalcSMA(pTemp, &m_apdVal[0][nPos], nDataSize, nStart, nday);
			
			if (!bLast)
			{
				m_aiPIndex[0] = nStart;
				m_aiNLimit[0] = nMim - 1;
			}
		}
	}
	else
	{
		delete [] pTemp;
		pTemp = NULL;		
		return false;
	}

	delete [] pTemp;
	pTemp = NULL;		

	return true;

}
