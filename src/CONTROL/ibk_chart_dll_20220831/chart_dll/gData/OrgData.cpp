// OrgData.cpp: implementation of the COrgData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrgData.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"
#include <float.h>
#include  <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define MAX_DATA_COUNT	9999

int GetToken1(char *pData, char *token)
{
	int	idx = 0;
	token[0] = 0x00;
	for(;;)
	{
		switch (pData[idx])
		{
		case '\t':
			token[idx++] = 0x00;
			return idx;
		case '\n':
			token[idx++] = 0x00;
			return idx;
		default:
			token[idx] = pData[idx];
			idx++;
			break;
		}
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrgData::COrgData(char *pData, class CDataMgr *pDataMgr)
{
	m_pDataMgr = pDataMgr;
	struct _dataH	*pDataH = (_dataH *)pData;

	m_strCODX = CString(pDataH->rcode, sizeof(pDataH->rcode));
	m_strCODX.TrimRight(" ");

	m_iDtKey = pDataH->dkey;
	m_iDtUnit = pDataH->dunit;
	m_iDtIndex = pDataH->dindex;
	
	m_iDtPattern = dpBASIC;

	m_dMax = DBL_MIN;
	m_dMin = DBL_MAX;
	m_dVMax = DBL_MIN;
	m_dVMin = DBL_MAX;
	m_dAMax = DBL_MIN;
	m_dAMin = DBL_MAX;
	m_dSahg = 0;
	m_dHahg = 0;

	m_bCalc		= false;
	m_bCalcV	= false;
	m_bCalcA	= false;
	m_bDateAdd	= false;
	m_bRealtime	= true;
//	m_bNoDelete	= false;
	m_cStockKind = 1;

	m_iLastTick = CharToInt(pDataH->ltic, sizeof(pDataH->ltic));
	m_iMGap = -1;
	m_iTGap = -1;

	if (m_iDtIndex == GI_MINUTE)
		m_iMGap = CharToInt(pDataH->lgap, sizeof(pDataH->lgap));
	else if (m_iDtIndex == GI_TICK)
		m_iTGap = CharToInt(pDataH->lgap, sizeof(pDataH->lgap));

	char	date[16];
	sprintf_s(date, sizeof(date),  _T("%.*s"), sizeof(pDataH->pday), pDataH->pday);
	m_iRyy = CharToInt(&date[0], 4);
	m_iRmm = CharToInt(&date[4], 2);
	m_iRdd = CharToInt(&date[6], 2);
}

COrgData::~COrgData()
{
	DetachGraphData();
}

int COrgData::GetArraySize()
{
	return m_arDataQue.GetSize();
}

class CGrpBasic* COrgData::GetGraphData(int iIndex)
{
	if (iIndex < 0 || iIndex > m_arDataQue.GetUpperBound())
		return NULL;

	return m_arDataQue.GetAt(iIndex);
}

//
// 조회된 그래픽 데이터를 파싱한다. 
//
bool COrgData::AttachGraphData(int iCount, int iDummy, char* pcData, int& riDataL, char* pcCurDate, bool bTodayData)
{
//	Debug(m_strCODX);
	if (riDataL <= 0 || (iDummy + iCount) <= 0)
		return false;

	if (iCount * 6 > riDataL) 
		return false;

	if (iCount > 0)
	{
		char	token[128]{ 0, };
		GetToken(&pcData[0], token);

		if (strlen(token) == 8)
		{
			short		yy = short(CharToInt(&token[0], 4));
			unsigned char	mm = unsigned char(CharToInt(&token[4], 2));
			unsigned char	dd = unsigned char(CharToInt(&token[6], 2));
			if (!yy || !mm || !dd) 
				return false;
		}
		else 
			return false;
	}

	if (!CreateDataQue(iDummy + iCount))
		return false;

	if (iDummy + iCount != GetArraySize())
	{
		TRACE("Created graph data iCount invalid....\n");
		return false;
	}

	CGrpBasic* pgbData = nullptr;
	for (int ii = 0; ii < iDummy; ii++)
	{
		pgbData = m_arDataQue.GetAt(ii);
		pgbData->m_bDummy = true;
	}

	char	token[128];
	int	index = 0;
	// 해외
	if (m_iDtUnit == GU_FOREIGN)
	{
		for (int ii = iDummy; ii < iDummy + iCount; ii++)
		{
			pgbData = m_arDataQue.GetAt(ii);
			pgbData->m_wRate = 0;
			pgbData->m_dwCheck = 0;
			pgbData->m_dTrdMoney = 0;

			for (int jj = 0; jj < 6; jj++)
			{
				int tLen = GetToken(&pcData[index], token);
				index += tLen;

				switch (jj)
				{
				case 0:
					pgbData->m_index.date.yy = short(CharToInt(&token[0], 4));
					pgbData->m_index.date.mm = unsigned char(CharToInt(&token[4], 2));
					pgbData->m_index.date.dd = unsigned char(CharToInt(&token[6], 2));
					break;
				case 1:	pgbData->m_iOpen = atoi(token);
					break;
				case 2:	pgbData->m_iHigh = atoi(token);
					break;
				case 3:	pgbData->m_iLow = atoi(token);
					break;
				case 4:	pgbData->m_iClose = atoi(token);
					break;
				case 5:	pgbData->m_dVolume = atof(token);
					break;
				}
			}
		}
	}
	// 주식 & (일봉 | 주봉 | 월봉)
	else if (m_iDtUnit == GU_CODE && (m_iDtIndex == GI_DAY || m_iDtIndex == GI_WEEK || m_iDtIndex == GI_MONTH))
	{
		for (int ii = iDummy; ii < iDummy + iCount; ii++)
		{
			pgbData = m_arDataQue.GetAt(ii);
			for (int jj = 0; jj < 10; jj++)
			{
				int tLen = GetToken(&pcData[index], token);
				index += tLen;

				switch (jj)
				{
				case 0:
					pgbData->m_index.date.yy = short(CharToInt(&token[0], 4));
					pgbData->m_index.date.mm = unsigned char(CharToInt(&token[4], 2));
					pgbData->m_index.date.dd = unsigned char(CharToInt(&token[6], 2));
					break;
				case 1:
					pgbData->m_index.time.hh = unsigned char(CharToInt(&token[0], 2));
					pgbData->m_index.time.mm = unsigned char(CharToInt(&token[2], 2));
					pgbData->m_index.time.ss = unsigned char(CharToInt(&token[4], 2));
					break;
				case 2:	pgbData->m_iOpen = atoi(token);
					break;
				case 3:	pgbData->m_iHigh = atoi(token);
					break;
				case 4:	pgbData->m_iLow = atoi(token);
					break;
				case 5:	pgbData->m_iClose = atoi(token);
					break;
				case 6:	pgbData->m_dVolume = atof(token);
					break;
				case 7:	pgbData->m_dTrdMoney = atof(token);
					break;
				case 8:
					pgbData->m_dwCheck = atoi(token);
					break;
				case 9:	pgbData->m_wRate = atoi(token);
					break;
				}
			}
		}
	}
	// 2007.01.01 schbang : ADR차트 
	// 업종 & 일봉
	else if (m_iDtUnit == GU_INDEX && m_iDtIndex == GI_DAY)
	{
		for (int ii = iDummy; ii < iDummy + iCount; ii++)
		{
			pgbData = m_arDataQue.GetAt(ii);
			for (int jj = 0; jj < 10; jj++)
			{
				int tLen = GetToken(&pcData[index], token);
				index += tLen;

				switch (jj)
				{
				case 0:
					pgbData->m_index.date.yy = short(CharToInt(&token[0], 4));
					pgbData->m_index.date.mm = unsigned char(CharToInt(&token[4], 2));
					pgbData->m_index.date.dd = unsigned char(CharToInt(&token[6], 2));
					break;
				case 1:
					pgbData->m_index.time.hh = unsigned char(CharToInt(&token[0], 2));
					pgbData->m_index.time.mm = unsigned char(CharToInt(&token[2], 2));
					pgbData->m_index.time.ss = unsigned char(CharToInt(&token[4], 2));
					break;
				case 2:	pgbData->m_iOpen = atoi(token);
					break;
				case 3:	pgbData->m_iHigh = atoi(token);
					break;
				case 4:	pgbData->m_iLow = atoi(token);
					break;
				case 5:	pgbData->m_iClose = atoi(token);
					break;
				case 6:	pgbData->m_dVolume = atof(token);
					break;
				case 7:	pgbData->m_dTrdMoney = atof(token);
					break;
				case 8:	pgbData->m_iUpCnt = atoi(token);
					break;
				case 9:	pgbData->m_iDnCnt = atoi(token);
					break;
				}
			}
		}
	}
	// 선물 or 옵션 or ...
	// 20090916 : CHANGE : 상품 선물 처리
	//else if (m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT)
	else if (m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT ||
		     m_iDtUnit == GU_CFUTURE)
	{
		for (int ii = iDummy; ii < iDummy + iCount; ii++)
		{
			pgbData = m_arDataQue.GetAt(ii);
			for (int jj = 0; jj < 9; jj++)
			{
				int tLen = GetToken(&pcData[index], token);
				index += tLen;

				switch (jj)
				{
				case 0:
					pgbData->m_index.date.yy = short(CharToInt(&token[0], 4));
					pgbData->m_index.date.mm = unsigned char(CharToInt(&token[4], 2));
					pgbData->m_index.date.dd = unsigned char(CharToInt(&token[6], 2));
					break;
				case 1:
					pgbData->m_index.time.hh = unsigned char(CharToInt(&token[0], 2));
					pgbData->m_index.time.mm = unsigned char(CharToInt(&token[2], 2));
					pgbData->m_index.time.ss = unsigned char(CharToInt(&token[4], 2));
					break;
				case 2:	pgbData->m_iOpen = atoi(token);
					break;
				case 3:	pgbData->m_iHigh = atoi(token);
					break;
				case 4:	pgbData->m_iLow = atoi(token);
					break;
				case 5:	pgbData->m_iClose = atoi(token);
					break;
				case 6:	pgbData->m_dVolume = atof(token);
					break;
				case 7:	pgbData->m_dTrdMoney = atof(token);
					break;
				case 8:	pgbData->m_iMgjy = atoi(token);
					break;
				}
			}
		}
	}
	
	else if (m_iDtUnit == GU_FFUTURE) // 20090916 : ADD : 상품 선물 처리
	{
		for (int ii = iDummy; ii < iDummy + iCount; ii++)
		{
			pgbData = m_arDataQue.GetAt(ii);
			for (int jj = 0; jj < 9; jj++)
			{
				int tLen = GetToken(&pcData[index], token);
				index += tLen;

				switch (jj)
				{
				case 0:
					pgbData->m_index.date.yy = short(CharToInt(&token[0], 4));
					pgbData->m_index.date.mm = unsigned char(CharToInt(&token[4], 2));
					pgbData->m_index.date.dd = unsigned char(CharToInt(&token[6], 2));
					break;
				case 1:
					pgbData->m_index.time.hh = unsigned char(CharToInt(&token[0], 2));
					pgbData->m_index.time.mm = unsigned char(CharToInt(&token[2], 2));
					pgbData->m_index.time.ss = unsigned char(CharToInt(&token[4], 2));
					break;
				case 2:	pgbData->m_iOpen = atoi(token)/100;
					break;
				case 3:	pgbData->m_iHigh = atoi(token)/100;
					break;
				case 4:	pgbData->m_iLow = atoi(token)/100;
					break;
				case 5:	pgbData->m_iClose = atoi(token)/100;
					break;
				case 6:	pgbData->m_dVolume = atof(token);
					break;
				case 7:	pgbData->m_dTrdMoney = atof(token);
					break;
				case 8:	pgbData->m_iMgjy = atoi(token);
					break;
				}
			}
		}
	}
	else
	{
		for (int ii = iDummy; ii < iDummy + iCount; ii++)
		{
			pgbData = m_arDataQue.GetAt(ii);
			for (int jj = 0; jj < 8; jj++)
			{
				int tLen = GetToken(&pcData[index], token);
				index += tLen;

				switch (jj)
				{
				case 0:
					pgbData->m_index.date.yy = short(CharToInt(&token[0], 4));
					pgbData->m_index.date.mm = unsigned char(CharToInt(&token[4], 2));
					pgbData->m_index.date.dd = unsigned char(CharToInt(&token[6], 2));
					break;
				case 1:
					pgbData->m_index.time.hh = unsigned char(CharToInt(&token[0], 2));
					pgbData->m_index.time.mm = unsigned char(CharToInt(&token[2], 2));
					pgbData->m_index.time.ss = unsigned char(CharToInt(&token[4], 2));
					break;
				case 2:	pgbData->m_iOpen = atoi(token);
					break;
				case 3:	pgbData->m_iHigh = atoi(token);
					break;
				case 4:	pgbData->m_iLow = atoi(token);
					break;
				case 5:	pgbData->m_iClose = atoi(token);
					break;
				case 6:	pgbData->m_dVolume = atof(token);
					break;
				case 7:	pgbData->m_dTrdMoney = atof(token);
					break;
				}
			}
		}
	}

	m_iCyy = 0;
	if (strlen(pcCurDate) >= 6)
	{
		m_iCyy = short(CharToInt(&pcCurDate[0], 4));
		m_iCmm = unsigned char(CharToInt(&pcCurDate[4], 2));
		m_iCdd = unsigned char(CharToInt(&pcCurDate[6], 2));
	}
	if (m_iCyy == 0)
	{
		m_iCyy = m_iRyy;
		m_iCmm = m_iRmm;
		m_iCdd = m_iRdd;
	}

	CString stmp, strhour, strmin;
	stmp.Format("%s", pcCurDate);
	stmp.Trim();
	strhour = stmp.Right(4);
	strhour = strhour.Left(2);
	strmin = stmp.Right(2);
	m_iShh = atoi(strhour);
	m_iSmm = atoi(strmin);

	//if (strlen(pcCurDate) >= 12)
	//{
	//	m_iShh = short(CharToInt(&pcCurDate[8], 2));
	//	m_iSmm = unsigned char(CharToInt(&pcCurDate[10], 2));
	//}
	//else
	//{
	//	m_iShh = 9;
	//	m_iSmm = 0;
	//}

	// 업종 & 일봉
	if (m_iDtUnit == GU_INDEX && m_iDtIndex == GI_DAY)
	{
		if (pgbData != nullptr)
		{
			int	rdate = m_iRyy*100 + m_iRmm*10 + m_iRdd;
			int	cdate = m_iCyy*100 + m_iCmm*10 + m_iCdd;
			if (rdate >= cdate)
			{
				if (pgbData->m_index.date.yy != m_iCyy ||
					pgbData->m_index.date.mm != m_iCmm ||
					pgbData->m_index.date.dd != m_iCdd)
					m_bDateAdd = true;
			}
			else
			{
				m_bRealtime = false;
			}
		}
	}
	else if (m_iDtIndex == GI_MINUTE || m_iDtIndex == GI_TICK)
	{
		int	cDate = m_iCyy*10000 + m_iCmm*100 + m_iCdd;
		int	rDate = m_iRyy*10000 + m_iRmm*100 + m_iRdd;

		if ((rDate < cDate) && !bTodayData)
			m_bRealtime = false;
	}
	else
	{
		int	cDate = m_iCyy*10000 + m_iCmm*100 + m_iCdd;
		int	rDate = m_iRyy*10000 + m_iRmm*100 + m_iRdd;

		if (rDate < cDate)
			m_bRealtime = false;
	}

	return true;
}

//** add macro by macho 
#define IF_ASSIGN(ifCondition, memberVariable) if(ifCondition) {\
		index += GetToken1(&pData[index], token);\
		gBasic->memberVariable = atoi(token);\
	}

bool COrgData::AttachGraphData2(int count, int dummy, char *pData, int &dataL, int id)
{
	if (dataL <= 0 || (dummy + count) <= 0)
		return false;

	int nKind = 0;

	BOOL frgnHave, frgnRate, psnlPBuy, frgnPBuy, orgnPBuy, psnlTBuy, frgnTBuy, orgnTBuy;

	frgnHave = id & FRGNHAVE;
	frgnRate = id & FRGNRATE;
	psnlPBuy = id & PSNLPBUY;
	frgnPBuy = id & FRGNPBUY;
	orgnPBuy = id & ORGNPBUY;
	psnlTBuy = id & PSNLTBUY;
	frgnTBuy = id & FRGNTBUY;
	orgnTBuy = id & ORGNTBUY;

	nKind = 0;

	if (frgnHave) nKind++;
	if (frgnRate) nKind++;
	if (psnlPBuy) nKind++;
	if (frgnPBuy) nKind++;
	if (orgnPBuy) nKind++;
	if (psnlTBuy) nKind++;
	if (frgnTBuy) nKind++;
	if (orgnTBuy) nKind++;

	if (count*nKind*2 > dataL){
		return false;
	}

	CGrpBasic* gBasic;
	if (dummy + count != GetArraySize())
	{
		TRACE("Created graph data count invalid....\n");
		return false;
	}

	char	token[128];
	int	ii, index = 0;
	for (ii = dummy; ii < dummy + count; ii++)
	{
		gBasic = m_arDataQue.GetAt(ii);

		//** macho add
		IF_ASSIGN(frgnHave, m_frgnHave);
		IF_ASSIGN(frgnRate, m_frgnRate);
		IF_ASSIGN(psnlPBuy, m_psnlPBuy);
		IF_ASSIGN(frgnPBuy, m_frgnPBuy);
		IF_ASSIGN(orgnPBuy, m_orgnPBuy);
		IF_ASSIGN(psnlTBuy, m_psnlTBuy);
		IF_ASSIGN(frgnTBuy, m_frgnTBuy);
		IF_ASSIGN(orgnTBuy, m_orgnTBuy);
	}
	return true;
}

// SE785000 data - 주식, 일별 데이타
bool COrgData::AttachMapData(int iCount, char* pcData, int& riDataL, char* pcCurDate, bool bTodayData)
{
	if (riDataL <= 0) 
		return false;

	if (iCount <= 0)
		return false;

	if (iCount * 6 > riDataL) 
		return false;

	if (iCount > 0)
	{
		char	token[128];
		GetToken(&pcData[0], token);

		if (strlen(token) == 8)
		{
			short		yy = short(CharToInt(&token[0], 4));
			unsigned char	mm = unsigned char(CharToInt(&token[4], 2));
			unsigned char	dd = unsigned char(CharToInt(&token[6], 2));
			if (!yy || !mm || !dd) 
				return false;
		}
		else 
			return false;
	}

	CGrpBasic* pgbData;
	char	token[128];
	int		ii, jj;
	int		index = 0;
	
	bool	bContinue = true;
	int		StartPos = 0;
	for (ii = 0; ii < iCount && bContinue; ii++)
	{
		StartPos = index;
		for (jj = 0; jj < 10 && bContinue; jj++)
		{
			int tLen = GetToken(&pcData[index], token);
			index += tLen;

			switch (jj)
			{
			case 2:	// open
				if (atoi(token) > 0)
					bContinue = false;
				break;
			}
		}
	}

	index = StartPos;
	ii--;
	iCount -= ii;

	if (iCount <= 0 || !CreateDataQue(iCount))
		return false;

	if (iCount != GetArraySize())
	{
		TRACE("Created graph data iCount invalid....\n");
		return false;
	}

	for (ii = 0; ii < iCount; ii++)
	{
		pgbData = m_arDataQue.GetAt(ii);
		for (jj = 0; jj < 10; jj++)
		{
			int tLen = GetToken(&pcData[index], token);
			index += tLen;

			switch (jj)
			{
			case 0:
				pgbData->m_index.date.yy = short(CharToInt(&token[0], 4));
				pgbData->m_index.date.mm = unsigned char(CharToInt(&token[4], 2));
				pgbData->m_index.date.dd = unsigned char(CharToInt(&token[6], 2));
				break;
			case 1:
				pgbData->m_index.time.hh = unsigned char(CharToInt(&token[0], 2));
				pgbData->m_index.time.mm = unsigned char(CharToInt(&token[2], 2));
				pgbData->m_index.time.ss = unsigned char(CharToInt(&token[4], 2));
				break;
			case 2:	pgbData->m_iOpen = atoi(token);
				break;
			case 3:	pgbData->m_iHigh = atoi(token);
				break;
			case 4:	pgbData->m_iLow = atoi(token);
				break;
			case 5:	pgbData->m_iClose = atoi(token);
				break;
			case 6:	pgbData->m_dVolume = atof(token);
				break;
			case 7:	pgbData->m_dTrdMoney = atof(token);
				break;
			case 8:
				pgbData->m_dwCheck = atoi(token);
				break;
			case 9:	pgbData->m_wRate = atoi(token);
				break;
			}
		}
	}

	m_iCyy = short(CharToInt(&pcCurDate[0], 4));
	m_iCmm = unsigned char(CharToInt(&pcCurDate[4], 2));
	m_iCdd = unsigned char(CharToInt(&pcCurDate[6], 2));

	{
		int	cDate = m_iCyy*10000 + m_iCmm*100 + m_iCdd;
		int	rDate = m_iRyy*10000 + m_iRmm*100 + m_iRdd;

		if (rDate < cDate)
			m_bRealtime = false;
	}

	return true;
}

void COrgData::DetachGraphData()
{
	for (int ii = 0; ii <= m_arDataQue.GetUpperBound(); ii++)
	{
		CGrpBasic* pGB = m_arDataQue.GetAt(ii);
		delete[] pGB;
		pGB = NULL;
	}
	m_arDataQue.RemoveAll();
}

bool COrgData::CreateDataQue(int date)
{
	int	ii; 
//	char	*pData;

	switch (m_iDtPattern)
	{
	case dpBASIC:
		for (ii = 0; ii < date; ii++)
		{
			//pData = new char[sz_gBASIC];
			//ZeroMemory(pData, sz_gBASIC)
			CGrpBasic* pGB = new CGrpBasic;
			m_arDataQue.Add(pGB);
		}
		break;
	case dpVIDX:
		break;
	case dpNONE:
	default:
		return false;
	}
	return true;

}

int COrgData::UpdateRTM(bool& rbIncrease)
{
	int iResponse = RTM_NO;

	if (!m_bRealtime)
		return iResponse;

	char acIndexRtm[16];
	sprintf_s(acIndexRtm, sizeof(acIndexRtm), _T("%s"), m_pDataMgr->GetCTIM());

	int iRtmHH = 0, iRtmMM = 0, iRtmSS = 0;

	switch (m_iDtIndex)
	{
	case GI_DAY:
	case GI_WEEK:
	case GI_MONTH:
		break;
	case GI_MINUTE:
	case GI_TICK:
		iRtmHH = short(CharToInt(&acIndexRtm[0], 2));
		iRtmMM = unsigned char(CharToInt(&acIndexRtm[2], 2));
		iRtmSS = unsigned char(CharToInt(&acIndexRtm[4], 2));
		break;
	default:
		return iResponse;
	}
	
	int iEndIdx = GetArraySize();
	if (iEndIdx < 1)
		return iResponse;

	CGrpBasic* pgbEnd = m_arDataQue.GetAt(iEndIdx - 1);

	int iCheckGap = (iRtmHH - pgbEnd->m_index.time.hh) * 3600;
	iCheckGap += (iRtmMM - pgbEnd->m_index.time.mm) * 60;
	iCheckGap += (iRtmSS - pgbEnd->m_index.time.ss);

	double dVal;
	CString strTemp;
	switch (m_iDtUnit)
	{
	case GU_CODE:
	case GU_COFUT:
	case GU_COOPT:
		strTemp.Format("%.f", fabs(atof(m_pDataMgr->GetCurr())));
		m_pDataMgr->SetCurr(strTemp);
		switch (m_iDtIndex)
		{
		case GI_MINUTE:	
		case GI_TICK:
			break;
		default:
			strTemp.Format("%.f", fabs(atof(m_pDataMgr->GetSiga())));
			m_pDataMgr->SetSiga(strTemp);
			strTemp.Format("%.f", fabs(atof(m_pDataMgr->GetKoga())));
			m_pDataMgr->SetKoga(strTemp);
			strTemp.Format("%.f", fabs(atof(m_pDataMgr->GetJega())));
			m_pDataMgr->SetJega(strTemp);
			break;
		}
		break;
	case GU_FFUTURE:	// 20090919 : ADD : 상품 선물은 XXXX 형식으로 서버에서 소수점 2자리가 붙어서 온다.
		strTemp.Format("%.f", fabs(atof(m_pDataMgr->GetCurr())));
		m_pDataMgr->SetCurr(strTemp);
		switch (m_iDtIndex)
		{
		case GI_MINUTE:	
		case GI_TICK:
			break;
		default:
			strTemp.Format("%.f", fabs(atof(m_pDataMgr->GetSiga())));
			m_pDataMgr->SetSiga(strTemp);
			strTemp.Format("%.f", fabs(atof(m_pDataMgr->GetKoga())));
			m_pDataMgr->SetKoga(strTemp);
			strTemp.Format("%.f", fabs(atof(m_pDataMgr->GetJega())));
			m_pDataMgr->SetJega(strTemp);
			break;
		}
		break;
	case GU_INDEX:	
		dVal = fabs(atof(m_pDataMgr->GetCurr()) * 100.0);
		strTemp.Format("%.f", dVal);
		m_pDataMgr->SetCurr(strTemp);
		break;
	case GU_FUTURE:
	case GU_OPTION:
	case GU_FOREIGN:
	case GU_CFUTURE:	// 20090917 : ADD : 채권, 통화 선물은 XXXX.XX 형식
		dVal = fabs(atof(m_pDataMgr->GetCurr()) * 100.0);
		strTemp.Format("%.f", dVal);
		m_pDataMgr->SetCurr(strTemp);
		switch (m_iDtIndex)
		{
		case GI_MINUTE:
		case GI_TICK:
			break;
		default:
			dVal = fabs(atof(m_pDataMgr->GetSiga()) * 100.0);
			strTemp.Format("%.f", dVal);
			m_pDataMgr->SetSiga(strTemp);
			dVal = fabs(atof(m_pDataMgr->GetKoga()) * 100.0);
			strTemp.Format("%.f", dVal);
			m_pDataMgr->SetKoga(strTemp);
			dVal = fabs(atof(m_pDataMgr->GetJega()) * 100.0);
			strTemp.Format("%.f", dVal);
			m_pDataMgr->SetJega(strTemp);
			break;
		}
		break;
	default:
		break;
	}

	CGrpBasic* pgbNew = NULL;
	iResponse = RTM_UPDATE;
	if (m_bDateAdd)
	{
		pgbNew = new CGrpBasic;

		pgbNew->m_index.date.yy = m_iCyy;
		pgbNew->m_index.date.mm = m_iCmm;
		pgbNew->m_index.date.dd = m_iCdd;
		pgbNew->m_index.time.hh = 0;
		pgbNew->m_index.time.mm = 0;
		pgbNew->m_index.time.ss = 0;

		pgbNew->m_iClose = pgbNew->m_iOpen = pgbNew->m_iHigh = pgbNew->m_iLow = atoi(m_pDataMgr->GetCurr());
		pgbNew->m_dVolume = atof(m_pDataMgr->GetGvol());
		pgbNew->m_dTrdMoney = atof(m_pDataMgr->GetGamt());
		pgbNew->m_iMgjy = atoi(m_pDataMgr->GetMgjy());

		pgbNew->m_dwCheck = 0;
		pgbNew->m_wRate = 0;

		m_arDataQue.InsertAt(iEndIdx, pgbNew);
		//if (m_arDataQue.GetSize() > MAX_DATA_COUNT)
		{
			CGrpBasic* pgbDel = m_arDataQue.GetAt(0);
			delete pgbDel;
			pgbDel = NULL;
			m_arDataQue.RemoveAt(0);
		}

		m_bDateAdd = false;

		return RTM_SHIFT;
	}

	switch (m_iDtIndex)
	{
	case GI_MINUTE:
		//if (iCheckGap > 0 || pgbEnd->m_index.date.dd != m_iCdd || pgbEnd->m_bDummy)	// add
		if (iCheckGap > 0 || iCheckGap < -7200 || pgbEnd->m_bDummy)	// add
		{
			pgbNew = new CGrpBasic;

			pgbNew->m_index.date.yy = m_iCyy;
			pgbNew->m_index.date.mm = m_iCmm;
			pgbNew->m_index.date.dd = m_iCdd;

			SetTime(m_iMGap, iRtmHH, iRtmMM, iRtmSS, 
				pgbNew->m_index.time.hh, pgbNew->m_index.time.mm, pgbNew->m_index.time.ss);

			pgbNew->m_dVolume = atof(m_pDataMgr->GetCvol());
			pgbNew->m_iMgjy = atoi(m_pDataMgr->GetMgjy());
			pgbNew->m_iClose = atoi(m_pDataMgr->GetCurr());
			pgbNew->m_iOpen = pgbNew->m_iClose;
			pgbNew->m_iHigh = pgbNew->m_iClose;
			pgbNew->m_iLow = pgbNew->m_iClose;
		}
		else
		{
			pgbEnd->m_dVolume += atof(m_pDataMgr->GetCvol());
			pgbEnd->m_iMgjy = atoi(m_pDataMgr->GetMgjy());
			pgbEnd->m_iClose = atoi(m_pDataMgr->GetCurr());
			if (pgbEnd->m_iClose > pgbEnd->m_iHigh)
				pgbEnd->m_iHigh = pgbEnd->m_iClose;
			else if (pgbEnd->m_iClose < pgbEnd->m_iLow)
				pgbEnd->m_iLow = pgbEnd->m_iClose;
		}
		break;
	case GI_TICK:
		if (m_iLastTick < m_iTGap && !pgbEnd->m_bDummy)
		{
			pgbEnd->m_dVolume += atof(m_pDataMgr->GetCvol());
			pgbEnd->m_iMgjy = atoi(m_pDataMgr->GetMgjy());
			pgbEnd->m_iClose = atoi(m_pDataMgr->GetCurr());
			if (pgbEnd->m_iClose > pgbEnd->m_iHigh)
				pgbEnd->m_iHigh = pgbEnd->m_iClose;
			else if (pgbEnd->m_iClose < pgbEnd->m_iLow)
				pgbEnd->m_iLow = pgbEnd->m_iClose;

			m_iLastTick++;
		}
		else	// add
		{
			pgbNew = new CGrpBasic;

			//Debug("Add Data");

			pgbNew->m_index.date.yy = m_iCyy;
			pgbNew->m_index.date.mm = m_iCmm;
			pgbNew->m_index.date.dd = m_iCdd;
			pgbNew->m_index.time.hh = iRtmHH;
			pgbNew->m_index.time.mm = iRtmMM;
			pgbNew->m_index.time.ss = iRtmSS;

			pgbNew->m_dVolume = atof(m_pDataMgr->GetCvol());
			pgbNew->m_iMgjy = atoi(m_pDataMgr->GetMgjy());			
			pgbNew->m_iClose = atoi(m_pDataMgr->GetCurr());
			pgbNew->m_iOpen = pgbNew->m_iClose;
			pgbNew->m_iHigh = pgbNew->m_iClose;
			pgbNew->m_iLow = pgbNew->m_iClose;

			m_iLastTick = 1;
		}
		break;
	case GI_WEEK:
	case GI_MONTH:
		pgbEnd->m_dVolume += atoi(m_pDataMgr->GetCvol());
		iResponse = 1;
		pgbEnd->m_iClose = atoi(m_pDataMgr->GetCurr());
		if (pgbEnd->m_iClose > pgbEnd->m_iHigh)
			pgbEnd->m_iHigh = pgbEnd->m_iClose;
		if (pgbEnd->m_iClose < pgbEnd->m_iLow)
			pgbEnd->m_iLow = pgbEnd->m_iClose;
		break;
	default:	// GI_DAY
		if (atof(m_pDataMgr->GetGvol()) > 0)
			pgbEnd->m_dVolume = atof(m_pDataMgr->GetGvol());
		if (atof(m_pDataMgr->GetGamt()) > 0)
			pgbEnd->m_dTrdMoney = atof(m_pDataMgr->GetGamt());
		if (atoi(m_pDataMgr->GetMgjy()) > 0)
			pgbEnd->m_iMgjy = atoi(m_pDataMgr->GetMgjy());

		switch (m_iDtUnit)
		{
		case GU_INDEX:
			pgbEnd->m_iClose = atoi(m_pDataMgr->GetCurr());
			if (pgbEnd->m_iClose > pgbEnd->m_iHigh)
				pgbEnd->m_iHigh = pgbEnd->m_iClose;
			if (pgbEnd->m_iClose < pgbEnd->m_iLow)
				pgbEnd->m_iLow = pgbEnd->m_iClose;
			break;
		case GU_FUTURE:
		case GU_OPTION:	
		case GU_COFUT:
		case GU_COOPT:
		case GU_FOREIGN:
		case GU_CFUTURE:	// 20090917 : ADD : 채권, 통화 선물 처리
		case GU_FFUTURE:	// 20090919 : ADD : 상품 선물 처리
			pgbEnd->m_iClose = atoi(m_pDataMgr->GetCurr());
			{
				int iLow = atoi(m_pDataMgr->GetJega());
				if (iLow > 0)
				{
					pgbEnd->m_iOpen = atoi(m_pDataMgr->GetSiga());
					pgbEnd->m_iHigh = atoi(m_pDataMgr->GetKoga());
					pgbEnd->m_iLow = iLow;
				}
				else
				{
					if (pgbEnd->m_iClose > pgbEnd->m_iHigh)
						pgbEnd->m_iHigh = pgbEnd->m_iClose;
					if (pgbEnd->m_iClose < pgbEnd->m_iLow)
						pgbEnd->m_iLow = pgbEnd->m_iClose;
				}
			}
			break;
		default:
			pgbEnd->m_iClose = atoi(m_pDataMgr->GetCurr());
			pgbEnd->m_iOpen = atoi(m_pDataMgr->GetSiga());
			pgbEnd->m_iHigh = atoi(m_pDataMgr->GetKoga());
			pgbEnd->m_iLow = atoi(m_pDataMgr->GetJega());
			break;
		}

		break;
	}

	// insert new data at last index
	if (pgbNew)
	{
		m_arDataQue.InsertAt(iEndIdx, pgbNew);
		//if (m_arDataQue.GetSize() > MAX_DATA_COUNT)
		{
			CGrpBasic* pgbDel = m_arDataQue.GetAt(0);
			m_arDataQue.RemoveAt(0);
			delete pgbDel;
			pgbDel = NULL;
		}
		iResponse = RTM_SHIFT;
	}

	return iResponse;
}

int COrgData::UpdateCntRTM()
{
	if (m_iDtIndex != GI_DAY || m_iDtUnit != GU_INDEX)
		return RTM_NO;

	int endIDX = GetArraySize();
	if (endIDX < 1)	return RTM_NO;

	struct	_gBasic *gBasicEnd = (struct _gBasic *) m_arDataQue.GetAt(endIDX -1);
//	gBasicEnd->ucnt = m_pDataMgr->m_ucnt;
//	gBasicEnd->dcnt = m_pDataMgr->m_dcnt;

	return RTM_UPDATE;
}

int COrgData::GetDataGap()
{
	switch (m_iDtIndex)
	{
	case GI_MINUTE:
		return m_iMGap;
	case GI_TICK:
		return m_iTGap;
	default:
		break;
	}

	return -1;
}

// 현재가/등락/전일가/상한가/하한가
void COrgData::SetCurr(CString sData)
{
	CString	sSise = sData;

	int	index = sSise.Find("\t");
	CString	sVal = sSise.Left(index);
	CString	sCurr = sVal;
	m_dCurr = fabs(atof(sVal));

	sSise = sSise.Mid(index + 1);
	index = sSise.Find("\t");
	sVal = sSise.Left(index);
	m_dUdyl = atof(sVal);

	sSise = sSise.Mid(index + 1);
	index = sSise.Find("\t");
	sVal = sSise.Left(index);
	if (!sVal.IsEmpty())
		m_dPCurr = fabs(atof(sVal));

	sSise = sSise.Mid(index + 1);
	index = sSise.Find("\t");
	sVal = sSise.Left(index);
	if (!sVal.IsEmpty())
		m_dSahg = fabs(atof(sVal));

	sVal = sSise.Mid(index + 1);
	if (!sVal.IsEmpty())
		m_dHahg = fabs(atof(sVal));

	m_iSign = 0;
	if (!sCurr.IsEmpty())
	{
		if (sCurr.GetAt(0) == '+')
			m_iSign = 1;
		else if (sCurr.GetAt(0) == '-')
			m_iSign = -1;
	}
}

void COrgData::SetStock(CString sData)
{
	m_dStock = atof(sData);
}

void COrgData::SetStockKind(char cKind)
{
	m_cStockKind = cKind;
}

int COrgData::CharToInt(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';
	return atoi(rWb);
}

double COrgData::CharToDouble(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';
	return atof(rWb);
}

bool COrgData::CalculateMinMax(int iStart, int iEnd, double *pdMin, double *pdMax, int iCOption, int iCKind, bool bForce)
{
	bool bRet = false; 
	switch (m_iDtPattern)
	{
	case dpBASIC:
		switch (iCKind)
		{
		case cVOL:
			m_bCalcV = CalculateBasic(iStart, iEnd, pdMin, pdMax, iCOption, iCKind);
			m_dVMax = *pdMax;
			m_dVMin = *pdMin;
			break;
		case cAMT:
			m_bCalcA = CalculateBasic(iStart, iEnd, pdMin, pdMax, iCOption, iCKind);
			m_dAMax = *pdMax;
			m_dAMin = *pdMin;
			break;
		default:
			m_bCalc = CalculateBasic(iStart, iEnd, pdMin, pdMax, iCOption, iCKind);
			m_dMax = *pdMax;
			m_dMin = *pdMin;
			break;
		}
		break;
	case dpTICK:
		switch (iCKind)
		{
		case cVOL:
			m_bCalcV = CalculateTick(iStart, iEnd, pdMin, pdMax, iCOption, iCKind);
			m_dVMax = *pdMax;
			m_dVMin = *pdMin;
			break;
		default:
			m_bCalc = CalculateBasic(iStart, iEnd, pdMin, pdMax, iCOption, iCKind);
			m_dMax = *pdMax;
			m_dMin = *pdMin;
			break;
		}
		break;
	case dpVIDX:
		break;
	default:
		break;
	}

	switch (iCKind)
	{
	case cVOL:
		*pdMin = m_dVMin;
		*pdMax = m_dVMax;
		return m_bCalcV;
	case cAMT:
		*pdMin = m_dAMin;
		*pdMax = m_dAMax;
		return m_bCalcA;
	default:
		*pdMin = m_dMin;
		*pdMax = m_dMax;
		return m_bCalc;
	}

	return true;
}

bool COrgData::CalculateLastIndex(int cOption, int cKind)
{
	return true;
}

bool COrgData::CalculateBasic(int iStart, int iEnd, double* pdMin, double* pdMax, int iCOption, int iCKind)
{
	if (iStart < 0 || m_arDataQue.GetSize() < iEnd)
		return false;

	int ii = 0;
	double	dMin = DBL_MAX;
	double	dMax = DBL_MIN;
	CGrpBasic* gBasic;

	for (ii = iStart; ii < iEnd; ii++)
	{
		gBasic = m_arDataQue.GetAt(ii);
		if (gBasic->m_iClose != GD_UNUSED)
			break;
	}

	switch (iCKind)
	{
	case cVOL:
		for (; ii < iEnd; ii++)
		{
			gBasic = m_arDataQue.GetAt(ii);

			if (gBasic->m_dVolume > dMax)
				dMax = gBasic->m_dVolume;
			if (gBasic->m_dVolume < dMin)
				dMin = gBasic->m_dVolume;
		}
		break;
	case cAMT:
		for (; ii < iEnd; ii++)
		{
			gBasic = m_arDataQue.GetAt(ii);

			if (gBasic->m_dTrdMoney > dMax)
				dMax = gBasic->m_dTrdMoney;
			if (gBasic->m_dTrdMoney < dMin)
				dMin = gBasic->m_dTrdMoney;
		}
		break;
	case cLIN:
		for (; ii < iEnd; ii++)
		{
			gBasic = m_arDataQue.GetAt(ii);

			if (gBasic->m_iClose > dMax)
				dMax = gBasic->m_iClose;
			if (gBasic->m_iClose < dMin)
				dMin = gBasic->m_iClose;
		}
		break;
	default:
		for (; ii < iEnd; ii++)
		{
			gBasic = m_arDataQue.GetAt(ii);

			if (gBasic->m_iHigh > dMax)
				dMax = gBasic->m_iHigh;
			if (gBasic->m_iLow < dMin)
				dMin = gBasic->m_iLow;
		}
		break;
	}

	if (dMax == DBL_MIN || dMin == DBL_MAX)
		return false;

	*pdMax = dMax;
	*pdMin = dMin;

	return true;
}

bool COrgData::CalculateTick(int start, int end, double *Min, double *Max, int cOption, int cKind)
{
	if (start < 0 || m_arDataQue.GetSize() <= end)
		return false;

	int	ii = 0;
	double	min = DBL_MAX;
	double	max = DBL_MIN;
	struct	_gTick	*gTick;

	for (ii = start; ii < end; ii++)
	{
		gTick = (struct _gTick *)m_arDataQue.GetAt(ii);
		if (gTick->epr != GD_UNUSED)
			break;
	}

	if (cKind == cVOL)
	{
		for (; ii < end; ii++)
		{
			gTick = (struct _gTick *)m_arDataQue.GetAt(ii);

			if (gTick->trn > max)	max = gTick->trn;
			if (gTick->trn < min)	min = gTick->trn;
		}
	}
	else
	{
		for (; ii < end; ii++)
		{
			gTick = (struct _gTick *)m_arDataQue.GetAt(ii);

			if (gTick->epr > max)	max = gTick->epr;
			if (gTick->epr < min)	min = gTick->epr;
		}
	}

	if (max == DBL_MIN || min == DBL_MAX)
		return false;

	*Max = max;
	*Min = min;

	return true;
}

int COrgData::GetToken(char *pData, char *token)
{
	int	idx = 0;
	token[0] = 0x00;
	for(;;)
	{
		switch (pData[idx])
		{
		case '\t':
			token[idx++] = 0x00;
			return idx;
		case '\n':
			token[idx++] = 0x00;
			return idx;
		default:
			token[idx] = pData[idx];
			idx++;
			break;
		}
	}

	return 0;
}


void COrgData::SetTime(int min, int cHH, int cMM, int cSS, unsigned char &nHH, unsigned char &nMM, unsigned char &nSS)
{
	//if (cHH == m_iShh && cMM == m_iSmm && cSS == 0)  //check
	//{
	//	nHH = m_iShh;
	//	nMM = m_iSmm + 1;
	//	nSS = 0;
	//	return;
	//}

	int	gap = (cHH - m_iShh)*60 + (cMM - m_iSmm);
	if (min > 900)	// 30초봉
	{
		if (30 < cSS)
		{
			gap += 1;
			nSS = 0;
		}
		else
		{
			nSS = 30;
		}
		gap = m_iSmm + gap;
		nHH = m_iShh + gap/60;
		nMM = gap%60;

		return;
	}

	if (gap%min == 0)
	{
		if (cSS == 0)
			gap = m_iSmm + gap;
		else
			gap = m_iSmm + gap + min;
	}
	else
	{
		gap = m_iSmm + (gap/min)*min + min;
	}

	nHH = m_iShh + gap/60;
	nMM = gap%60;
	nSS = 0;	
}