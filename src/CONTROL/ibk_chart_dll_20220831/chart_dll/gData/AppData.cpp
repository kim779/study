// AppData.cpp: implementation of the CAppData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppData.h"
#include "OrgData.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppData::CAppData(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
{
	m_pOrgData = pOrgData;
	m_strName = _T("");
	m_iDtKey = dKey;
	m_iGKind = gKind;

	m_bCalc = false;
	m_iCOption = cOption;	

	m_pCInfo = new _chartinfo;
	ZeroMemory(m_pCInfo, SZ_CHARTINFO);
	if (pCInfo)
		CopyMemory(m_pCInfo, pCInfo, SZ_CHARTINFO);
	for (int ii = 0; ii < 12; ii++)
	{
		m_apdVal[ii] = NULL;
		m_aiPIndex[ii] = -1;
	}

	m_dMax = DBL_MIN;
	m_dMin = DBL_MAX;

	m_iDataSize = pOrgData->GetArraySize();
	m_iNSignal = -1;
}

CAppData::~CAppData()
{
	for (int ii = 0; ii < 12; ii++)
	{
		if (m_apdVal[ii])
		{
			delete[] m_apdVal[ii];
			m_apdVal[ii] = NULL;
		}
	}

	if (m_pCInfo)
		delete m_pCInfo;
}

void CAppData::SetVariable(struct _chartinfo *pCInfo, int cOption)
{
	if (pCInfo)
		CopyMemory(m_pCInfo, pCInfo, SZ_CHARTINFO);

	if (cOption >= 0)
		m_iCOption = cOption;
}

double CAppData::GetVal(int lindex, int vindex)
{
	if (lindex >= GetDataCount() || lindex < 0 || vindex >= m_iDataSize)
		return GD_UNUSED;

	return m_apdVal[lindex][vindex];
}

bool CAppData::CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce)
{
	if (!m_bCalc || bForce)
	{
		if (!Calculate(start, end, bForce))
			return false;
	}

	*Min = m_dMin;
	*Max = m_dMax;

	return true;
}

int CAppData::GetDataCount()
{ 
	int	nCount = 0;

	if (m_iNSignal > 0)
		nCount = m_iNLine + 1;
	else
		nCount = m_iNLine;
	return nCount;
}
