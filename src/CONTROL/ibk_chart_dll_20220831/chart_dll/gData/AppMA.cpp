// AppMA.cpp: implementation of the CAppMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppMA.h"
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppMA::CAppMA(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CAppData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_arMA.RemoveAll();

	_maData* maData;
	for (int ii = 0; ii < m_pOrgData->GetArraySize(); ii++)
	{
		maData = new _maData;
		m_arMA.Add(maData);
	}
}

CAppMA::~CAppMA()
{
	struct	_maData* maData;
	for (int ii = 0; ii <= m_arMA.GetUpperBound(); ii++)
	{
		maData = m_arMA.GetAt(ii);
		delete[] maData;
		maData = NULL;
	}
	m_arMA.RemoveAll();
}

bool CAppMA::Calculate(int start, int end, bool bForce)
{
	if (m_bCalc && !bForce)
		return true;

	DWORD	chkbit = 0x00000001;
	for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
	{
		if (!(m_pCInfo->wCOption & chkbit))
		{
			m_aiPIndex[ii] = -1;
			continue;
		}

		//if (m_iDataSize < int(m_pCInfo->awValue[ii]) || int(m_pCInfo->awValue[ii]) <= 0)
		if (int(m_pCInfo->awValue[ii]) <= 0)
			continue;
		else if (m_iDataSize < int(m_pCInfo->awValue[ii]))
		{
			struct	_maData* maData;
			int totalCnt = m_pOrgData->GetArraySize();
			for (int jj = 0; jj < totalCnt; jj++)
			{
				maData = (struct _maData *)m_arMA.GetAt(jj);
				maData->ma[ii] = GD_UNUSED;
			}
		}

		if (m_iCOption & GC_MAWEIGHT)	// 가중이평
			m_aiPIndex[ii] = CalculateSWMA(ii, int(m_pCInfo->awValue[ii]));
		else if (m_iCOption & GC_MAEXP)	// 지수이평
			m_aiPIndex[ii] = CalculateEMA(ii, int(m_pCInfo->awValue[ii]));
		else				// 단순이평
			m_aiPIndex[ii] = CalculateSMA(ii, int(m_pCInfo->awValue[ii]));

		if (m_aiPIndex[ii] < 0)
		{
			struct	_maData* maData;
			int totalCnt = m_pOrgData->GetArraySize();
			for (int jj = 0; jj < totalCnt; jj++)
			{
				maData = (struct _maData *)m_arMA.GetAt(jj);
				maData->ma[ii] = GD_UNUSED;
			}
		}
	}

	m_bCalc = true;

	return true;
}

int CAppMA::CalculateSMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	int	ii;
	CGrpBasic* gBasic;
	struct	_maData* maData = nullptr;
	
	double* pvDBL = new double[totalCnt +1];
	switch (m_iGKind)
	{
	case GK_PMA:
		switch (m_iCOption & (GC_MAOPEN|GC_MAHIGH|GC_MALOW|GC_MAMID|GC_MAREP))
		{
		case GC_MAOPEN:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iOpen);
			}
			break;
		case GC_MAHIGH:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iHigh);
			}
			break;
		case GC_MALOW:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iLow);
			}
			break;
		case GC_MAMID:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double((gBasic->m_iHigh + gBasic->m_iLow))/2;
			}
			break;
		case GC_MAREP:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iHigh + gBasic->m_iLow + gBasic->m_iClose) / 3;
			}
			break;
		default:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iClose);
			}
			break;
		}
		break;
	case GK_VMA:
		for (ii = 0; ii < totalCnt; ii++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			pvDBL[ii] = double(gBasic->m_dVolume);
		}
		break;
	case GK_AMA:
		for (ii = 0; ii < totalCnt; ii++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			pvDBL[ii] = double(gBasic->m_dTrdMoney);
		}
		break;
	}

	for (ii = 0; ii < nDay - 1 && ii < totalCnt; ii++)
	{
		maData = (struct _maData *)m_arMA.GetAt(ii);
		maData->ma[idx] = GD_UNUSED;
	}

	double	valTOT = 0;
	int	nCount = 0;

	for ( ii = 0 ; ii < totalCnt ; ii++ )
	{
		maData = (struct _maData *)m_arMA.GetAt(ii);
		maData->ma[idx] = GD_UNUSED;

		if (pvDBL[ii] != GD_UNUSED)
		{
			valTOT += pvDBL[ii];
			nCount++;

			if (nCount == nDay)
				break;
		}
	}

	if (nCount != nDay)
	{
		delete [] pvDBL;
		pvDBL = NULL;
		return -1;
	}

	if (maData != nullptr)
		maData->ma[idx] = valTOT/double(nDay);
	ii++;

	for ( ; ii < totalCnt ; ii++ )
	{
		maData = (struct _maData *)m_arMA.GetAt(ii);
		valTOT += pvDBL[ii] - pvDBL[ii - nDay];
		maData->ma[idx] = valTOT / double(nDay);		
	}	

	delete [] pvDBL;
	pvDBL = NULL;

	return idx;
}

int CAppMA::CalculateSWMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	float	div = 0;
	for (int jj = 1; jj <= nDay; jj++)
		div += jj;

	int	ii;
	CGrpBasic* gBasic;
	struct	_maData* maData;
	
	double* pvDBL = new double[totalCnt +1];
	switch (m_iGKind)
	{
	case GK_PMA:
		switch (m_iCOption & (GC_MAOPEN|GC_MAHIGH|GC_MALOW|GC_MAMID|GC_MAREP))
		{
		case GC_MAOPEN:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iOpen);
			}
			break;
		case GC_MAHIGH:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iHigh);
			}
			break;
		case GC_MALOW:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iLow);
			}
			break;
		case GC_MAMID:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iHigh + gBasic->m_iLow)/2;
			}
			break;
		case GC_MAREP:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iHigh + gBasic->m_iLow + gBasic->m_iClose)/3;
			}
			break;
		default:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iClose);
			}
			break;
		}
		break;
	case GK_VMA:
		for (ii = 0; ii < totalCnt; ii++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			pvDBL[ii] = double(gBasic->m_dVolume);
		}
		break;
	case GK_AMA:
		for (ii = 0; ii < totalCnt; ii++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			pvDBL[ii] = double(gBasic->m_dTrdMoney);
		}
		break;
	}

	for (ii = 0; ii < nDay - 1 && ii < totalCnt; ii++)
	{
		maData = m_arMA.GetAt(ii);
		maData->ma[idx] = GD_UNUSED;
	}

	double	valTOT;
	for (ii = nDay - 1; ii < totalCnt; ii++)
	{
		gBasic = m_pOrgData->GetGraphData(ii);
		maData = m_arMA.GetAt(ii);

		valTOT = 0.0;
		for (int jj = ii, kk = nDay; jj > ii - nDay; jj--,kk--)
		{
			if (pvDBL[jj] == GD_UNUSED)
			{
				valTOT = GD_UNUSED;
				break;
			}
			valTOT += pvDBL[jj]*kk;
		}

		if (valTOT == GD_UNUSED)
		{
			maData->ma[idx] = GD_UNUSED;
			continue;
		}
		maData->ma[idx] = valTOT/double(div);
	}
	delete [] pvDBL;
	pvDBL = NULL;

	return idx;
}

int CAppMA::CalculateEMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	CGrpBasic* gBasic;
	struct	_maData* maData;
	
	int	ii;
	double* pvDBL = new double[totalCnt +1];
	switch (m_iGKind)
	{
	case GK_PMA:
		switch (m_iCOption & (GC_MAOPEN|GC_MAHIGH|GC_MALOW|GC_MAMID|GC_MAREP))
		{
		case GC_MAOPEN:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iOpen);
			}
			break;
		case GC_MAHIGH:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iHigh);
			}
			break;
		case GC_MALOW:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iLow);
			}
			break;
		case GC_MAMID:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iHigh + gBasic->m_iLow)/2;
			}
			break;
		case GC_MAREP:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iHigh + gBasic->m_iLow + gBasic->m_iClose)/3;
			}
			break;
		default:
			for (ii = 0; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				pvDBL[ii] = double(gBasic->m_iClose);
			}
			break;
		}
		break;
	case GK_VMA:
		for (ii = 0; ii < totalCnt; ii++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			pvDBL[ii] = double(gBasic->m_dVolume);
		}
		break;
	case GK_AMA:
		for (ii = 0; ii < totalCnt; ii++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			pvDBL[ii] = double(gBasic->m_dTrdMoney);
		}
		break;
	}

	int	start = 0;
	for (ii = 0; ii < totalCnt; ii++)
	{
		if (pvDBL[ii] == GD_UNUSED)
		{
			maData = m_arMA.GetAt(ii);
			maData->ma[idx] = GD_UNUSED;
			start++;
		}
		else
			break;
	}
	
	for (ii = start; ii < start + nDay - 1 && ii < totalCnt; ii++)
	{
		maData = (struct _maData *)m_arMA.GetAt(ii);
		maData->ma[idx] = GD_UNUSED;
	}

	double Ramda = 2.0 / double(nDay + 1);
	ii = start + nDay - 1;
	if (ii >= totalCnt)
	{
		delete [] pvDBL;
		pvDBL = NULL;
		return -1;
	}

	double	sum = pow(1-Ramda, nDay-1) * pvDBL[start];
	for (int jj = ii ; jj > ii - nDay + 1; jj--)
		sum += pow(1-Ramda, ii-jj) * Ramda * pvDBL[jj];

	struct	_maData* PremaData = (struct _maData *)m_arMA.GetAt(ii);
	PremaData->ma[idx] = sum;

	for (ii = ii+1; ii < totalCnt; ii++)
	{
		maData = (struct _maData *)m_arMA.GetAt(ii);
		maData->ma[idx] = Ramda * pvDBL[ii] + (1-Ramda) * PremaData->ma[idx];
		PremaData = maData;
	}

	delete [] pvDBL;
	pvDBL = NULL;

	return idx;
}

bool CAppMA::CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce)
{
	if (!m_bCalc || bForce)
		Calculate(start, end);

	m_dMax = DBL_MIN;
	m_dMin = DBL_MAX;

	if (m_arMA.GetSize() == 0 || m_pOrgData->GetArraySize() != m_arMA.GetSize())
		return false;

	double	dVal;
	struct _maData* maData;
	for (int ii = start; ii < end; ii++)
	{
		maData = m_arMA.GetAt(ii);
		if (maData == NULL) {
			//Debug("maData is null");
			return false;
		}
		for (int jj = 0; jj < MAX_LINE; jj++)
		{
			if (m_aiPIndex[jj] < 0 || maData->ma[jj] == GD_UNUSED)
				continue;

			dVal = maData->ma[jj];
			if (dVal > m_dMax)
				m_dMax = dVal;

			if (dVal < m_dMin)
				m_dMin = dVal;
		}
	}

	*Min = m_dMin;
	*Max = m_dMax;

	return true;
}

bool CAppMA::CalculateLastIndex(bool bShift, bool bIncrease)
{
	if (bShift)
	{
		int	endidx = m_arMA.GetUpperBound();
		struct	_maData *maData = new _maData;
		for (int jj = 0; jj < MAX_LINE; jj++)
			maData->ma[jj] = GD_UNUSED;
		m_arMA.Add(maData);

		if (!bIncrease)
		{
			maData = m_arMA.GetAt(0);
			delete[] maData;
			maData = NULL;
			m_arMA.RemoveAt(0);
		}
		else
		{
			m_iDataSize = m_pOrgData->GetArraySize();
		}
	}

	DWORD	chkbit = 0x00000001;
	for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
	{
		if (!(m_pCInfo->wCOption & chkbit))
		{
			m_aiPIndex[ii] = -1;
			continue;
		}

		if (m_iDataSize < int(m_pCInfo->awValue[ii]) || int(m_pCInfo->awValue[ii]) <= 0)
			continue;

		if (m_iCOption & GC_MAWEIGHT)	// 가중이평
			m_aiPIndex[ii] = CalculateLastSWMA(ii, int(m_pCInfo->awValue[ii]));
		else if (m_iCOption & GC_MAEXP)	// 지수이평
			m_aiPIndex[ii] = CalculateLastEMA(ii, int(m_pCInfo->awValue[ii]));
		else				// 단순이평
			m_aiPIndex[ii] = CalculateLastSMA(ii, int(m_pCInfo->awValue[ii]));
	}

	m_bCalc = true;

	return true;
}

int CAppMA::CalculateLastSMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	int	ii;
	CGrpBasic* gBasic;
	
	gBasic = m_pOrgData->GetGraphData(totalCnt - nDay);
	if (gBasic->m_iClose == GD_UNUSED)
		return -1;

	double	valTOT = 0.0;
	struct _maData* maData = (struct _maData *)m_arMA.GetAt(totalCnt - 1);

	switch (m_iGKind)
	{
	case GK_PMA:
		switch (m_iCOption & (GC_MAOPEN|GC_MAHIGH|GC_MALOW|GC_MAMID|GC_MAREP))
		{
		case GC_MAOPEN:
			for (ii = totalCnt - nDay; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iOpen);
			}
			break;
		case GC_MAHIGH:
			for (ii = totalCnt - nDay; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iHigh);
			}
			break;
		case GC_MALOW:
			for (ii = totalCnt - nDay; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iLow);
			}
			break;
		case GC_MAMID:
			for (ii = totalCnt - nDay; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iHigh + gBasic->m_iLow)/2;
			}
			break;
		case GC_MAREP:
			for (ii = totalCnt - nDay; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iHigh + gBasic->m_iLow + gBasic->m_iClose)/3;
			}
			break;
		default:
			for (ii = totalCnt - nDay; ii < totalCnt; ii++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iClose);
			}
			break;
		}
		break;
	case GK_VMA:
		for (ii = totalCnt - nDay; ii < totalCnt; ii++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			valTOT += double(gBasic->m_dVolume);
		}
		break;
	case GK_AMA:
		for (ii = totalCnt - nDay; ii < totalCnt; ii++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			valTOT += double(gBasic->m_dTrdMoney);
		}
		break;
	}
	maData->ma[idx] = valTOT/double(nDay);

	return idx;
}

int CAppMA::CalculateLastSWMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	CGrpBasic* gBasic = m_pOrgData->GetGraphData(totalCnt - nDay);
	if (gBasic->m_iClose == GD_UNUSED)
		return -1;

	int	ii, jj;
	float	div = 0;
	double	valTOT = 0.0;

	for (jj = 1; jj <= nDay; jj++)
		div += jj;

	struct	_maData* maData = (struct _maData *)m_arMA.GetAt(totalCnt - 1);
	switch (m_iGKind)
	{
	case GK_PMA:
		switch (m_iCOption & (GC_MAOPEN|GC_MAHIGH|GC_MALOW|GC_MAMID|GC_MAREP))
		{
		case GC_MAOPEN:
			for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iOpen)*jj;
			}
			break;
		case GC_MAHIGH:
			for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iHigh)*jj;
			}
			break;
		case GC_MALOW:
			for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iLow)*jj;
			}
			break;
		case GC_MAMID:
			for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iHigh + gBasic->m_iLow)/2*jj;
			}
			break;
		case GC_MAREP:
			for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iHigh + gBasic->m_iLow + gBasic->m_iClose)/3*jj;
			}
			break;
		default:
			for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
			{
				gBasic = m_pOrgData->GetGraphData(ii);
				valTOT += double(gBasic->m_iClose)*jj;
			}
			break;
		}
		break;
	case GK_VMA:
		for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			valTOT += double(gBasic->m_dVolume)*jj;
		}
		break;
	case GK_AMA:
		for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
		{
			gBasic = m_pOrgData->GetGraphData(ii);
			valTOT += double(gBasic->m_dTrdMoney)*jj;
		}
		break;
	}
	maData->ma[idx] = valTOT/double(div);

	return idx;
}

int CAppMA::CalculateLastEMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	CGrpBasic* gBasic;
	gBasic = m_pOrgData->GetGraphData(totalCnt - nDay);
	if (gBasic->m_iClose == GD_UNUSED)
		return -1;

	double Ramda = 2.0 / double(nDay + 1);
	struct	_maData	*PremaData = (struct _maData *)m_arMA.GetAt(totalCnt - 2);
	struct	_maData	*maData = (struct _maData *)m_arMA.GetAt(totalCnt - 1);
	if (PremaData->ma[idx] == GD_UNUSED)
	{
		int	jj;
		double	sum = 0;
		switch (m_iGKind)
		{
		case GK_PMA:
			switch (m_iCOption & (GC_MAOPEN|GC_MAHIGH|GC_MALOW|GC_MAMID|GC_MAREP))
			{
			case GC_MAOPEN:
				sum = pow(1-Ramda, nDay-1) * gBasic->m_iOpen;
				for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
				{
					gBasic = m_pOrgData->GetGraphData(jj);
					sum += pow(1-Ramda, totalCnt-1-jj) * Ramda * gBasic->m_iOpen;
				}
				break;
			case GC_MAHIGH:
				sum = pow(1-Ramda, nDay-1) * gBasic->m_iHigh;
				for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
				{
					gBasic = m_pOrgData->GetGraphData(jj);
					sum += pow(1-Ramda, totalCnt-1-jj) * Ramda * gBasic->m_iHigh;
				}
				break;
			case GC_MALOW:
				sum = pow(1-Ramda, nDay-1) * gBasic->m_iLow;
				for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
				{
					gBasic = m_pOrgData->GetGraphData(jj);
					sum += pow(1-Ramda, totalCnt-1-jj) * Ramda * gBasic->m_iLow;
				}
				break;
			case GC_MAMID:
				sum = pow(1-Ramda, nDay-1) * ((gBasic->m_iHigh + gBasic->m_iLow)/2);
				for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
				{
					gBasic = m_pOrgData->GetGraphData(jj);
					sum += pow(1-Ramda, totalCnt-1-jj) * Ramda * double(gBasic->m_iHigh + gBasic->m_iLow)/2;
				}
				break;
			case GC_MAREP:
				sum = pow(1-Ramda, nDay-1) * ((gBasic->m_iHigh + gBasic->m_iLow + gBasic->m_iClose)/3);
				for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
				{
					gBasic = m_pOrgData->GetGraphData(jj);
					sum += pow(1-Ramda, totalCnt-1-jj) * Ramda * double(gBasic->m_iHigh + gBasic->m_iLow + gBasic->m_iClose)/3;
				}
				break;
			default:
				sum = pow(1-Ramda, nDay-1) * gBasic->m_iClose;
				for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
				{
					gBasic = m_pOrgData->GetGraphData(jj);
					sum += pow(1-Ramda, totalCnt-1-jj) * Ramda * gBasic->m_iClose;
				}
				break;
			}
			break;
		case GK_VMA:
			sum = pow(1-Ramda, nDay-1) * gBasic->m_dVolume;
			for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
			{
				gBasic = m_pOrgData->GetGraphData(jj);
				sum += pow(1-Ramda, totalCnt-1-jj) * Ramda * gBasic->m_dVolume;
			}
			break;
		case GK_AMA:
			sum = pow(1-Ramda, nDay-1) * gBasic->m_dTrdMoney;
			for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
			{
				gBasic = m_pOrgData->GetGraphData(jj);
				sum += pow(1-Ramda, totalCnt-1-jj) * Ramda * gBasic->m_dTrdMoney;
			}
			break;
		}

		maData->ma[idx] = sum;		
	}
	else
	{
		gBasic = m_pOrgData->GetGraphData(totalCnt - 1);
		switch (m_iGKind)
		{
		case GK_PMA:
			switch (m_iCOption & (GC_MAOPEN|GC_MAHIGH|GC_MALOW|GC_MAMID|GC_MAREP))
			{
			case GC_MAOPEN:
				maData->ma[idx] = Ramda * gBasic->m_iOpen + (1-Ramda) * PremaData->ma[idx];
				break;
			case GC_MAHIGH:
				maData->ma[idx] = Ramda * gBasic->m_iHigh + (1-Ramda) * PremaData->ma[idx];
				break;
			case GC_MALOW:
				maData->ma[idx] = Ramda * gBasic->m_iLow + (1-Ramda) * PremaData->ma[idx];
				break;
			case GC_MAMID:
				maData->ma[idx] = Ramda * double(gBasic->m_iHigh + gBasic->m_iLow)/2 + 
					(1-Ramda) * PremaData->ma[idx];
				break;
			case GC_MAREP:
				maData->ma[idx] = Ramda * double(gBasic->m_iHigh + gBasic->m_iLow + gBasic->m_iClose)/3 + 
					(1-Ramda) * PremaData->ma[idx];
				break;
			default:
				maData->ma[idx] = Ramda * gBasic->m_iClose + (1-Ramda) * PremaData->ma[idx];
				break;
			}
			break;
		case GK_VMA:
			maData->ma[idx] = Ramda * gBasic->m_dVolume + (1-Ramda) * PremaData->ma[idx];
			break;
		case GK_AMA:
			maData->ma[idx] = Ramda * gBasic->m_dTrdMoney + (1-Ramda) * PremaData->ma[idx];
			break;
		}
	}

	return idx;
}

double CAppMA::GetVal(int lindex, int vindex)
{
	double	ret = 0.0;

	_maData*	maData = NULL;	

	//Debug("GetVal = %d, %d", lindex, vindex);

	if (lindex < 0 || lindex >= MAX_LINE)
		return GD_UNUSED;

if (vindex >= m_arMA.GetSize())
	return GD_UNUSED;

	maData = m_arMA.GetAt(vindex);
	ret = maData->ma[lindex];

	return ret;
}