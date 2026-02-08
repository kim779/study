#include "stdafx.h"
#include "AppMACD.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAppMACD::CAppMACD(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}

	memset(m_adPrevVal, 0x00, sizeof(m_adPrevVal));
	Calculate(0,0);
}

CAppMACD::~CAppMACD()
{

}

bool CAppMACD::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	//int ii = 0, jj = 0, nStart = -1, nCount = 0;
	//double	val = 0.0, pVal = 0.0;
	//CGrpBasic* gBasic = NULL;
	
	int nsday = (int)m_pCInfo->awValue[0];
	int nlday = (int)m_pCInfo->awValue[1];
	int nSignal = (int)m_pCInfo->awValue[2];
	int nDataSize = m_iDataSize;
	int nPos = 0;
	int nMim = 0;

	if (m_iGKind == GK_MACDOS)
	{
		nMim = nlday + nSignal - 1;
		m_iNSignal = 0;
	}
	else
	{
		nMim = nlday;
		m_iNSignal = nSignal;
	}	

	if (nsday <= 0 || nlday <= 0 || nsday > nlday)
		return false;

	bool bOrgLast = bLast;

//	if (m_iCOption & GC_MAEXP)
//		bLast = false;
//
//	if (bLast)
//	{
//		nDataSize = nMim;
//		nPos = m_iDataSize - nDataSize;
//	}
//
//	if (nPos < 0)
//		return false;

	double* apdTemp[4]{ 0, };
	double* pdOutput = nullptr;
	int nSize = sizeof(apdTemp) / sizeof(double*);

	int ii = 0;
	for (ii = 0 ; ii < nSize ; ii++ )
	{
		apdTemp[ii] = new double[nDataSize];
	}

	int nStart = -1;
	CGrpBasic* gBasic;
	for (ii = 0 ; ii < nDataSize ; ii++ )
	{
		gBasic = m_pOrgData->GetGraphData(ii + nPos);

		for (int jj = 0 ; jj < nSize ; jj++ )
		{
			apdTemp[jj][ii] = GD_UNUSED;
		}
		
		if (gBasic->m_iClose != GD_UNUSED)
		{	
			if (nStart < 0)
				nStart = ii;				
		
			apdTemp[0][ii] = (double)gBasic->m_iClose;
		}
	}

	int nCount = 0;
	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		int	nn = 0;

		if (nCount >= nMim)
		{
/*			if (m_iCOption & GC_MAWEIGHT)
			{
				CDataMgr::CalcSWMA(apdTemp[0], apdTemp[1], nDataSize, nStart, nsday);
				nn = CDataMgr::CalcSWMA(apdTemp[0], apdTemp[2], nDataSize, nStart, nlday);
			}
			else if (m_iCOption & GC_MAEXP)
			{
				int	nTemp[2] = {nsday, nlday};

				for ( ii = 0 ; ii < 2 ; ii++ )
				{
					if (bLast)
					{
						if (bShift)
							m_adPrevVal[ii * 2] = m_adPrevVal[ii * 2 + 1];
						
						apdTemp[ii + 1][nTemp[ii] - 2] = m_adPrevVal[ii * 2];
						nn = CLineData::CalcEMA(apdTemp[0], apdTemp[ii + 1], nDataSize, nStart, nTemp[ii], bLast);
						m_adPrevVal[ii * 2 + 1] = apdTemp[ii + 1][nn];
					}
					else
					{				
						nn = CLineData::CalcEMA(apdTemp[0], apdTemp[ii + 1], nDataSize, nStart, nTemp[ii], bLast);
						m_adPrevVal[ii * 2] = apdTemp[ii + 1][nDataSize - nMim + (nTemp[ii] - 2)];
						m_adPrevVal[ii * 2 + 1] = apdTemp[ii + 1][nDataSize - nMim + (nTemp[ii] - 1)];
					}
				}
			}
			else
			{
				CDataMgr::CalcSMA(apdTemp[0], apdTemp[1], nDataSize, nStart, nsday);
				nn = CDataMgr::CalcSMA(apdTemp[0], apdTemp[2], nDataSize, nStart, nlday);
			}
*/

			//** macho@2006.01.04
			//** MACD 계산할 때의 이동평균값 계산 오류 수정
			if (m_iCOption & GC_MAWEIGHT) {	// 가중이평
				CDataMgr::CalcSWMA(apdTemp[0], apdTemp[1], nDataSize, nStart, nsday);
				nn = CDataMgr::CalcSWMA(apdTemp[0], apdTemp[2], nDataSize, nStart, nlday);
			}
			else if (m_iCOption & GC_MAEXP) {	// 지수이평
				CDataMgr::CalcEMA(apdTemp[0], apdTemp[1], nDataSize, nStart, nsday);
				nn = CDataMgr::CalcEMA(apdTemp[0], apdTemp[2], nDataSize, nStart, nlday);
			}
			else {				// 단순이평
				CDataMgr::CalcSMA(apdTemp[0], apdTemp[1], nDataSize, nStart, nsday);
				nn = CDataMgr::CalcSMA(apdTemp[0], apdTemp[2], nDataSize, nStart, nlday);
			}
			//** END.
			
		}		

		if (nn >= 0)
		{
			if (m_iGKind == GK_MACD)
			{
				pdOutput = &m_apdVal[0][nPos];
				if (!bOrgLast || m_aiPIndex[0] == -1)
				{
					m_aiPIndex[0] = nn;
					m_aiNLimit[0] = nMim - 1;
				}
			}
			else if (m_iGKind == GK_MACDOS)
			{
				pdOutput = apdTemp[3];
			}

			for ( ii = nn ; ii < nDataSize ; ii++ )
			{		
				if (apdTemp[2][ii] == GD_UNUSED)	continue;

				if (pdOutput != nullptr)
					pdOutput[ii] = (apdTemp[1][ii] - apdTemp[2][ii]);

				if (m_pOrgData->GetUnit() == GU_CODE && pdOutput != nullptr)
					pdOutput[ii] *= 100.0;
			}

			nStart = nn;

			if (m_iGKind == GK_MACDOS)
			{
				if (m_iCOption & GC_MAWEIGHT)
					nn = CDataMgr::CalcSWMA(pdOutput, apdTemp[0], nDataSize, nStart, nSignal);
				else if (m_iCOption & GC_MAEXP)
				{
					if (bLast)
					{
						if (bShift)
							m_adPrevVal[4] = m_adPrevVal[5];

						apdTemp[0][nDataSize - 2] = m_adPrevVal[4];
						nn = CLineData::CalcEMA(pdOutput, apdTemp[0], nDataSize, nStart, nSignal, bLast);
						m_adPrevVal[5] = apdTemp[0][nDataSize - 1];
					}
					else
					{
						nn = CLineData::CalcEMA(pdOutput, apdTemp[0], nDataSize, nStart, nSignal, bLast);
						m_adPrevVal[5] = apdTemp[0][nDataSize - 1];
						m_adPrevVal[4] = apdTemp[0][nDataSize - 2];
					}					
				}
				else
					nn = CDataMgr::CalcSMA(pdOutput, apdTemp[0], nDataSize, nStart, nSignal);					

				if (!bOrgLast || m_aiPIndex[0] == -1)
				{
					m_aiPIndex[0] = nn;
					m_aiNLimit[0] = nMim - 1;
				}

				for ( ii = nn ; ii < nDataSize ; ii++ )
				{
					if (apdTemp[0][ii] == GD_UNUSED || !m_apdVal || !pdOutput)
						continue;

					m_apdVal[0][ii + nPos] = pdOutput[ii] - apdTemp[0][ii];
				}
			}
		}
	}

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		delete [] apdTemp[ii];
		apdTemp[ii] = NULL;
	}

	return true;
}

