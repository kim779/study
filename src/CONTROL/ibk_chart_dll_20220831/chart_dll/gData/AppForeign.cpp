// AppForeign.cpp: implementation of the CAppForeign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppForeign.h"
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

CAppForeign::CAppForeign(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;
	m_apdVal[0] = new double[m_iDataSize];

	Calculate(0,0);
}

CAppForeign::~CAppForeign()
{

}

//** add macro by macho
#define ASSIGN_CHART_DATA(caseConstant, memberVariable) case caseConstant: \
	for (ii = nStart; ii < nDataSize; ii++) {\
		gBasic = m_pOrgData->GetGraphData(ii + nPos); \
		m_apdVal[0][ii + nPos] = gBasic->memberVariable;\
	}\
	if (!bLast || m_aiPIndex[0] == -1) \
		m_aiPIndex[0] = nStart; \
	break;

bool CAppForeign::CalcuData(bool bShift, bool bLast)
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
		switch (m_iGKind)
		{
			/** remarking by macho <begin>
		case GK_FEXRATE:
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				m_apdVal[0][ii + nPos] = (double)gBasic->m_iFrgnExRate;
			}			

			if (!bLast || m_aiPIndex[0] == -1)
				m_aiPIndex[0] = nStart;		
			break;
		case GK_FOBRATE:
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				m_apdVal[0][ii + nPos] = (double)gBasic->m_iFrgnObRate;
			}			

			if (!bLast || m_aiPIndex[0] == -1)
				m_aiPIndex[0] = nStart;		
			break;
		case GK_FRGNVOL:
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				m_apdVal[0][ii + nPos] = gBasic->m_iFrgnVol;
			}			

			if (!bLast || m_aiPIndex[0] == -1)
				m_aiPIndex[0] = nStart;		
			break;
		case GK_FRGNAMT:
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				m_apdVal[0][ii + nPos] = gBasic->m_iFrgnAmt;
			}			
			
			if (!bLast || m_aiPIndex[0] == -1)
				m_aiPIndex[0] = nStart;		
			break;
		case GK_ORGNVOL:
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				m_apdVal[0][ii + nPos] = gBasic->m_iOrgnVol;
				
			}			
			
			if (!bLast || m_aiPIndex[0] == -1)
				m_aiPIndex[0] = nStart;		
			break;
		case GK_ORGNAMT:	
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				gBasic = m_pOrgData->GetGraphData(ii + nPos);
				m_apdVal[0][ii + nPos] = gBasic->m_iOrgnAmt;
			}			
			
			if (!bLast || m_aiPIndex[0] == -1)
				m_aiPIndex[0] = nStart;		
			break;
		remarking by macho  <end> **/
		ASSIGN_CHART_DATA(GK_FRGNHAVE, m_frgnHave);
		ASSIGN_CHART_DATA(GK_FRGNRATE, m_frgnRate);
		ASSIGN_CHART_DATA(GK_PSNLPBUY, m_psnlPBuy);
		ASSIGN_CHART_DATA(GK_FRGNPBUY, m_frgnPBuy);
		ASSIGN_CHART_DATA(GK_ORGNPBUY, m_orgnPBuy);
		ASSIGN_CHART_DATA(GK_PSNLTBUY, m_psnlTBuy);
		ASSIGN_CHART_DATA(GK_FRGNTBUY, m_frgnTBuy);
		ASSIGN_CHART_DATA(GK_ORGNTBUY, m_orgnTBuy);
		default:
			return false;
		}
	}

	return true;
}

bool CAppForeign::CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce)
{
	bool result = CLineData::CalculateMinMax(start, end, Min, Max, bForce);

	if (result) {
		switch (m_iGKind) {
			case GK_PSNLPBUY: case GK_FRGNPBUY: case GK_ORGNPBUY:
			case GK_PSNLTBUY: case GK_FRGNTBUY: case GK_ORGNTBUY:
				if (*Min * *Max > 0) {
					if (*Max > 0)
						*Min = -*Max / 10;
					else if (*Min < 0)
						*Max = -*Min / 10;
				}
				break;
		}
	}

	return result;
}