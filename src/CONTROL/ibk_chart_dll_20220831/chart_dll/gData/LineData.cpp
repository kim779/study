#include "stdafx.h"
#include "LineData.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#include <math.h>
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CLineData::CLineData(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CAppData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_bShift = true;

	int	nSize = sizeof(m_aiNLimit) / sizeof(int);
	int	ii = 0;

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		m_aiNLimit[ii] = 0;
	}

}

CLineData::~CLineData()
{

}

bool CLineData::Calculate(int start, int end, bool bForce)
{
	if (m_bCalc && !bForce)
		return true;

	int	nCount = sizeof(m_apdVal) / sizeof(double*);

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		if (m_apdVal[ii] != NULL)
		{			
			for ( int jj = 0 ; jj < m_iDataSize ; jj++ )
			{			
				m_apdVal[ii][jj] = GD_UNUSED;
			}
		}
	}

	if (!CalcuData())
	{		
		return false;
	}

	nCount = GetDataCount();
	
	if (nCount == 2 && m_iNSignal > 0)
	{
		if (m_apdVal[1] == NULL)		
			m_apdVal[1] = new double[m_iDataSize];

		for ( int ii = 0 ; ii < m_iDataSize ; ii++ )
		{
			m_apdVal[1][ii] = GD_UNUSED;
		}
		
		if (m_aiPIndex[0] >= 0)
		{
			int	nCount = m_iDataSize - m_aiPIndex[0];

			if (nCount >= m_iNSignal)
			{
				if (m_iCOption & GC_MAWEIGHT)
					m_aiPIndex[1] = CDataMgr::CalcSWMA(m_apdVal[0], m_apdVal[1], m_iDataSize, m_aiPIndex[0], m_iNSignal);
				else if (m_iCOption & GC_MAEXP)
					m_aiPIndex[1] = CDataMgr::CalcEMA(m_apdVal[0], m_apdVal[1], m_iDataSize, m_aiPIndex[0], m_iNSignal);
				else
					m_aiPIndex[1] = CDataMgr::CalcSMA(m_apdVal[0], m_apdVal[1], m_iDataSize, m_aiPIndex[0], m_iNSignal);

				m_aiNLimit[1] = m_aiNLimit[0] + m_iNSignal - 1;
			}
		}
	}
	else
	{
		if (nCount == 1 && m_apdVal[1])
		{
			delete [] m_apdVal[1];
			m_apdVal[1] = NULL;
		}
	}
		
	m_bCalc = true;
	return true;
}

bool CLineData::CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce)
{
	if (!m_bCalc || bForce)
		if (!Calculate(start, end))
			return false;

	double	dVal = 0.0;
	int	nCount = 0;
	int	nStart = -1;

	if (m_iNSignal > 0)
		nCount = m_iNLine + 1;
	else
		nCount = m_iNLine;

	m_dMin = INT_MAX;
	m_dMax = INT_MIN;

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		for ( int jj = start ; jj < end ; jj++ )
		{			
			dVal = m_apdVal[ii][jj];			

			if (dVal == GD_UNUSED)
				continue;

			if (nStart < 0)
			{
				m_dMin = m_dMax = dVal;
				nStart = jj;
			}			
			
			if (dVal > m_dMax)
				m_dMax = dVal;
			if (dVal < m_dMin)
				m_dMin = dVal;
		}
	}

	if (m_dMin == INT_MAX || m_dMax == INT_MIN)
		return false;

	if (m_iGKind == GK_MACDOS)
	{
		if (m_dMax < 0)
			m_dMax = 0.0;

		if (m_dMin > 0)
			m_dMin = 0.0;
	}

	if (nStart < 0)
		return false;

	*Max = m_dMax;
	*Min = m_dMin;	
	return true;
}

bool CLineData::CalculateLastIndex(bool bShift, bool bIncrease)
{
	int	ii = 0, jj = 0, nCount = 0, nPos = 0;
	int	nMim = m_iNSignal;
	int	nDataSize = nMim;

	if (bIncrease)
		IncreaseData();
//	if (bShift)
	else if (bShift)
		ShiftData();

	CalcuLastData(bShift);

	if (m_iNSignal > 0 && m_aiPIndex[0] >= 0 && m_apdVal[0][m_iDataSize - 1] != GD_UNUSED
		&& (m_iDataSize - m_aiPIndex[0]) >= m_iNSignal)
	{		
		nPos = m_iDataSize - nDataSize;
		if (nPos < 0) return false;
				
		if (m_iCOption & GC_MAWEIGHT)
			CDataMgr::CalcSWMA(&m_apdVal[0][nPos], &m_apdVal[1][nPos], nDataSize, 0, m_iNSignal);
		else if (m_iCOption & GC_MAEXP)
			CDataMgr::CalcEMA(m_apdVal[0], m_apdVal[1], m_iDataSize, 0, m_iNSignal);
		else
			CDataMgr::CalcSMA(&m_apdVal[0][nPos], &m_apdVal[1][nPos], nDataSize, 0, m_iNSignal);
	}

	m_bCalc = true;
	return true;
}

void CLineData::ShiftData()
{
	if (!m_bShift)
		return;

//	int	ii = 0, jj = 0;
	double*	pTemp = new double[m_iDataSize];
	int	nCount = 0;

	if (m_iNSignal > 0) 
		nCount = m_iNLine + 1;
	else 
		nCount = m_iNLine;

	for (int  ii = 0 ; ii < nCount ; ii++ )
	{
		memcpy(pTemp, &m_apdVal[ii][1], sizeof(double) * (m_iDataSize - 1));
		pTemp[m_iDataSize - 1] = GD_UNUSED;
		memcpy(m_apdVal[ii], pTemp, sizeof(double) * m_iDataSize);

		if (m_aiPIndex[ii] != -1)
		{
			m_aiPIndex[ii] = (m_aiPIndex[ii] > 0) ? m_aiPIndex[ii] - 1 : 0;
			for (int jj = 0; jj < m_aiPIndex[ii]; jj++)
			{
				m_apdVal[ii][jj] = GD_UNUSED;
			}
		}
	}

	delete [] pTemp;
	pTemp = NULL;	
}

void CLineData::IncreaseData()
{
//	if (!m_bShift)
//		return;

	int iDataSizeOld = m_iDataSize;
	m_iDataSize = m_pOrgData->GetArraySize();

	

	int	nCount = 0;
	if (m_iNSignal > 0) 
		nCount = m_iNLine + 1;
	else 
		nCount = m_iNLine;

	for (int  ii = 0 ; ii < nCount ; ii++ )
	{
		double*	pdTemp = new double[m_iDataSize];
		ASSERT(pdTemp);
		memcpy(pdTemp, m_apdVal[ii], iDataSizeOld * sizeof(double));
		pdTemp[m_iDataSize - 1] = GD_UNUSED;
		delete m_apdVal[ii];
		m_apdVal[ii] = pdTemp;
	}
}

void CLineData::CalcuLastData(bool bShift)
{
	if (m_bShift)
		CalcuData(bShift, true);
	else
		CalcuData(false, !bShift);
}

int CLineData::CalcEMA(double *source, double *dest, int count, int start, int size, bool bLast)
{
	if (!bLast)
		return CDataMgr::CalcEMA(source, dest, count, start, size);
	
	int	ii = start + size - 1;
	double	val = dest[ii - 1];
	double	Ramda = 2.0 / double(size + 1);
	
	if (val == GD_UNUSED)	// 처음 구하는 위치의 바로 앞의 값이 없다면
	{
		double	sum = pow(1-Ramda, size-1) * source[start];
		for (int jj = ii ; jj > ii - size + 1; jj--)
			sum += pow(1-Ramda, ii-jj) * Ramda * source[jj];
		dest[ii] = sum;		
		ii++;
	}
	
	for ( ; ii < count; ii++)
		dest[ii] = Ramda * source[ii] + (1-Ramda) * dest[ii-1];
	return start + size - 1;
}

int CLineData::CalcEMA(double *source, double* prev, double *dest, int size)
{
	double	Ramda = 2.0 / double(size + 1);	
	*dest = Ramda * (*source) + (1.0 - Ramda) * (*prev);
	return size - 1;
}