// AppMgjy.cpp: implementation of the CAppMgjy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppMgjy.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppMgjy::CAppMgjy(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;
	m_apdVal[0] = new double[m_iDataSize];

	Calculate(0,0);
}

CAppMgjy::~CAppMgjy()
{

}

bool CAppMgjy::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;

	int	ii = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic* gBasic = NULL;
	int	nPos = 0, nDataSize = m_iDataSize;

	if (bLast)
	{
		nDataSize = 1;
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
		for ( ii = nStart ; ii < nDataSize ; ii++ )
		{
			gBasic = m_pOrgData->GetGraphData(ii + nPos);
			m_apdVal[0][ii + nPos] = gBasic->m_iMgjy;
		}			

		if (!bLast || m_aiPIndex[0] == -1)
			m_aiPIndex[0] = nStart;		
	}

	return true;
}