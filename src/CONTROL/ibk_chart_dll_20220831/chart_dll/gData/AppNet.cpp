// AppNet.cpp: implementation of the CAppNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppNet.h"
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

CAppNet::CAppNet(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CAppData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_netQue.RemoveAll();	

	m_dayMAV = 5;
	m_gap = 5; 
	m_netCnt = 20;

	_netData* netData;
	if (m_netQue.GetSize() == 0)
	{
		for (int ii = 0; ii < m_pOrgData->GetArraySize(); ii++)
		{
			netData = new _netData();
			m_netQue.Add(netData);
		}
	}

	for (int ii = 0; ii < 12; ii++)
		m_pIndex[ii] = -1;	
}

CAppNet::~CAppNet()
{
	struct	_netData* netData;
	for (int ii = 0; ii <= m_netQue.GetUpperBound(); ii++)
	{
		netData = m_netQue.GetAt(ii);
		delete [] netData;
		netData = NULL;
	}
	m_netQue.RemoveAll();
}

bool CAppNet::Calculate(int start, int end, bool bForce)
{
	if (m_bCalc && !bForce)
		return true;

	if (m_pCInfo->awValue[0] > 0)
	{
		m_dayMAV = int(m_pCInfo->awValue[0]);
		if (m_dayMAV <= 0)
			m_dayMAV = 5;
	}
	if (m_pCInfo->awValue[1] > 0)
	{
		m_gap = int(m_pCInfo->awValue[1]);
		if (m_gap < 0 || m_gap > 20)
			m_gap = 20;
	}
	if (m_pCInfo->awValue[2] > 0)
	{
		m_netCnt = int(m_pCInfo->awValue[2]);
		if (m_netCnt <= 0 || m_netCnt > MAX_NET)
			m_netCnt = MAX_NET;
	}

	for (int day = m_dayMAV, idx = 0; idx < m_netCnt; day += m_gap, idx++)
	{
		if (m_iCOption & GC_MAWEIGHT)	// 가중이평
			m_pIndex[idx] = CalculateSWMA(idx, day);
		else if (m_iCOption & GC_MAEXP)	// 지수이평
			m_pIndex[idx] = CalculateEMA(idx, day);
		else
			m_pIndex[idx] = CalculateSMA(idx, day);

		if (m_pIndex[idx] < 0)
		{
			struct	_netData* netData;
			int totalCnt = m_pOrgData->GetArraySize();
			for (int jj = 0; jj < totalCnt; jj++)
			{
				netData = (struct _netData *)m_netQue.GetAt(jj);
				netData->ma[idx] = GD_UNUSED;
			}
		}
	}

	m_bCalc = true;

	return true;
}

int CAppNet::CalculateSMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	int	ii;
	CGrpBasic* gBasic;
	struct	_netData* netData = nullptr;
	
	double* pvDBL = new double[totalCnt +1];
	for (ii = 0; ii < totalCnt; ii++)
	{
		gBasic = m_pOrgData->GetGraphData(ii);
		pvDBL[ii] = double(gBasic->m_iClose);
	}

	for (ii = 0; ii < nDay - 1 && ii < totalCnt; ii++)
	{
		netData = m_netQue.GetAt(ii);
		netData->ma[idx] = GD_UNUSED;
	}

	double	valTOT = 0;
	int	nCount = 0;

	for ( ii = 0 ; ii < totalCnt ; ii++ )
	{
		netData = (struct _netData *)m_netQue.GetAt(ii);
		netData->ma[idx] = GD_UNUSED;

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
		return idx;
	}

	 if (netData != nullptr)
	netData->ma[idx] = valTOT/double(nDay);
	ii++;

	for ( ; ii < totalCnt ; ii++ )
	{
		netData = (struct _netData *)m_netQue.GetAt(ii);
		valTOT += pvDBL[ii] - pvDBL[ii - nDay];
		netData->ma[idx] = valTOT / double(nDay);		
	}


	delete [] pvDBL;
	pvDBL = NULL;

	return idx;
}

int CAppNet::CalculateSWMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	float	div = 0;
	for (int jj = 1; jj <= nDay; jj++)
		div += jj;

	int	ii;
	CGrpBasic* gBasic;
	struct	_netData* netData;
	
	double* pvDBL = new double[totalCnt +1];
	for (ii = 0; ii < totalCnt; ii++)
	{
		gBasic = m_pOrgData->GetGraphData(ii);
		pvDBL[ii] = double(gBasic->m_iClose);
	}

	for (ii = 0; ii < nDay - 1 && ii < totalCnt; ii++)
	{
		netData = (struct _netData *)m_netQue.GetAt(ii);
		netData->ma[idx] = GD_UNUSED;
	}

	double	valTOT;
	for (ii = nDay - 1; ii < totalCnt; ii++)
	{
		gBasic = m_pOrgData->GetGraphData(ii);
		netData = (struct _netData *)m_netQue.GetAt(ii);

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
			netData->ma[idx] = GD_UNUSED;
			continue;
		}
		netData->ma[idx] = valTOT/double(div);
	}
	delete [] pvDBL;
	pvDBL = NULL;

	return idx;
}

int CAppNet::CalculateEMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	CGrpBasic* gBasic;
	struct	_netData* netData;

	double* pvDBL = new double[totalCnt + 1]{ 0., };
	
	int ii = 0, start = 0;
	for (ii = 0; ii < totalCnt; ii++)
	{
		gBasic = m_pOrgData->GetGraphData(ii);
		pvDBL[ii] = double(gBasic->m_iClose);

		if (pvDBL[ii] == GD_UNUSED)
		{
			netData = (struct _netData *)m_netQue.GetAt(ii);
			netData->ma[idx] = GD_UNUSED;
			start++;
		}
	}
		
	for (ii = start; ii < start + nDay - 1 && ii < totalCnt; ii++)
	{
		netData = (struct _netData *)m_netQue.GetAt(ii);
		netData->ma[idx] = GD_UNUSED;
	}

	if (start + nDay - 1 >= totalCnt)
	{
		delete[] pvDBL;
		return -1;
	}
	
	double Ramda = 2.0 / (static_cast<double>(nDay) + 1);
	ii = start + nDay - 1;
	double	sum = pow(1-Ramda, static_cast<double>(nDay)-1) * pvDBL[start];
	for (int jj = ii; jj > ii - nDay + 1; jj--)
	{
		sum += pow(1 - Ramda, static_cast<double>(ii) - static_cast<double>(jj)) * Ramda * pvDBL[jj];
	}

	struct	_netData* PreNetData = (struct _netData *)m_netQue.GetAt(ii);
	PreNetData->ma[idx] = sum;

	for (ii = ii+1; ii < totalCnt; ii++)
	{
		netData = (struct _netData *)m_netQue.GetAt(ii);
		netData->ma[idx] = Ramda * pvDBL[ii] + (1-Ramda) * PreNetData->ma[idx];
		PreNetData = netData;
	}

	delete [] pvDBL;
	pvDBL = NULL;

	return idx;
}

bool CAppNet::CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce)
{
	if (!m_bCalc || bForce)
		Calculate(start, end);

	m_dMax = DBL_MIN;
	m_dMin = DBL_MAX;

	if (m_netQue.GetSize() == 0)
		return false;

	double	dVal;
	struct _netData* netData;
	for (int ii = start; ii < end; ii++)
	{
		netData = m_netQue.GetAt(ii);
		for (int idx = 0; idx < m_netCnt; idx++)
		{
			if (m_pIndex[idx] < 0 || netData->ma[idx] == GD_UNUSED)
				continue;

			dVal = netData->ma[idx];
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

bool CAppNet::CalculateLastIndex(bool bShift, bool bIncrease)
{
	if (bShift)
	{
		struct	_netData *netData = new _netData;
		for (int jj = 0; jj < m_netCnt; jj++)
			netData->ma[jj] = GD_UNUSED;
		m_netQue.Add(netData);

		if (!bIncrease)
		{
			netData = m_netQue.GetAt(0);
			delete netData;
			netData = NULL;
			m_netQue.RemoveAt(0);
		}
	}

	DWORD	chkbit = 0x00000001;
	for (int day = m_dayMAV, idx = 0; idx < m_netCnt; day += m_gap, idx++)
	{
		if (m_iDataSize < day)
		{
			m_pIndex[idx] = -1;
			continue;
		}

		if (m_iCOption & GC_MAWEIGHT)	// 가중이평
			m_pIndex[idx] = CalculateLastSWMA(idx, day);
		else if (m_iCOption & GC_MAEXP)	// 지수이평
			m_pIndex[idx] = CalculateLastEMA(idx, day);
		else				// 단순이평
			m_pIndex[idx] = CalculateLastSMA(idx, day);
	}

	m_bCalc = true;

	return true;
}

int CAppNet::CalculateLastSMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	int	ii;
	CGrpBasic* gBasic;
	struct	_netData* netData;
	
	gBasic = m_pOrgData->GetGraphData(totalCnt - nDay);
	if (gBasic->m_iClose == GD_UNUSED)
		return -1;

	double	valTOT = 0.0;
	netData = (struct _netData *)m_netQue.GetAt(totalCnt - 1);
	for (ii = totalCnt - nDay; ii < totalCnt; ii++)
	{
		gBasic = m_pOrgData->GetGraphData(ii);
		valTOT += double(gBasic->m_iClose);
		netData->ma[idx] = valTOT/double(nDay);
	}

	return idx;
}

int CAppNet::CalculateLastSWMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	int	ii, jj;
	CGrpBasic* gBasic;
	struct	_netData* netData;
	
	gBasic = m_pOrgData->GetGraphData(totalCnt - nDay);
	if (gBasic->m_iClose == GD_UNUSED)
		return -1;

	float	div = 0;
	for (jj = 1; jj <= nDay; jj++)
		div += jj;

	double	valTOT = 0.0;
	netData = (struct _netData *)m_netQue.GetAt(totalCnt - 1);
	for (ii = totalCnt - nDay, jj = 1; ii < totalCnt; ii++, jj++)
	{
		gBasic = m_pOrgData->GetGraphData(ii);
		valTOT += double(gBasic->m_iClose)*jj;
	}
	netData->ma[idx] = valTOT/double(div);

	return idx;
}

int CAppNet::CalculateLastEMA(int idx, int nDay)
{
	int totalCnt = m_pOrgData->GetArraySize();
	if (idx < 0 || nDay <= 0 || nDay > totalCnt)
		return -1;

	CGrpBasic* gBasic;
	gBasic = m_pOrgData->GetGraphData(totalCnt - nDay);
	if (gBasic->m_iClose == GD_UNUSED)
		return -1;

	double Ramda = 2.0 / (static_cast<double>(nDay) + 1);
	struct	_netData	*PreNetData = (struct _netData *)m_netQue.GetAt(totalCnt - 2);
	struct	_netData	*netData = (struct _netData *)m_netQue.GetAt(totalCnt - 1);
	if (PreNetData->ma[idx] == GD_UNUSED)
	{
		int	jj;
		double	sum = 0;
		sum = pow(1-Ramda, static_cast<double>(nDay)-1) * gBasic->m_iClose;
		for (jj = totalCnt - 1 ; jj > totalCnt - nDay; jj--)
		{
			gBasic = m_pOrgData->GetGraphData(jj);
			sum += pow(1-Ramda, static_cast<double>(totalCnt)-static_cast<double>(jj)-1) * Ramda * gBasic->m_iClose;
		}

		netData->ma[idx] = sum;		
	}
	else
	{
		gBasic = m_pOrgData->GetGraphData(totalCnt - 1);
		netData->ma[idx] = Ramda * gBasic->m_iClose + (1-Ramda) * PreNetData->ma[idx];
	}

	return idx;
}

double CAppNet::GetVal(int lindex, int vindex)
{
	double	ret = 0.0;

	_netData*	netData = NULL;

	if (lindex < 0 || lindex >= 30)
		return GD_UNUSED;

	netData = m_netQue.GetAt(vindex);
	ret = netData->ma[lindex];

	return ret;
}
