// Grp_Data.cpp: implementation of the CGrp_Data class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_VolLine1.h"
#include "Grp_Data.h"
#include "GrpWnd.h"
#include "../../h/axiscgrp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrp_Data::CGrp_Data(class CGrpWnd *pGrpWnd, int dKind, int dKey, int dIndex)
{
	m_pGrpWnd = pGrpWnd;
	m_dKind = dKind;
	m_dKey = dKey;
	m_dIndex = dIndex;

	m_DataQue.RemoveAll();
	m_szFrame = sz_cgVAL2;
}

CGrp_Data::~CGrp_Data()
{
	char* pData{};

	for (int ii = 0; ii <= m_DataQue.GetUpperBound(); ii++)
	{
		pData = m_DataQue.GetAt(ii);
		delete[] pData;
		pData = NULL;
	}
	
	m_DataQue.RemoveAll();
}

int CGrp_Data::AttachGraphData(char* data, int count, int mgap)
{
	m_dGap = mgap;

	if (!CreateDataQue(count))
		return -1;

	struct _cdValue2* cdValue2{};
	struct _cgVal2* cgVal2{};

	int	dPos = 0;
	char* pQue{};
	const int	nSize = sz_cdValue2;

	for (int ii = 0; ii < count; ii++)
	{
		pQue = m_DataQue.GetAt(ii);

		cgVal2 = (struct _cgVal2*)pQue;
		cdValue2 = (struct _cdValue2*) &data[dPos];

		cgVal2->val[0] = CharToInt(cdValue2->yval, sizeof(cdValue2->yval));
		cgVal2->val[1] = CharToInt(cdValue2->xval, sizeof(cdValue2->xval));
		
		cgVal2->index.date.yy = short(CharToInt(&cdValue2->date[0], 4));
		cgVal2->index.date.mm = unsigned char(CharToInt(&cdValue2->date[4], 2));
		cgVal2->index.date.dd = unsigned char(CharToInt(&cdValue2->date[6], 2));
		cgVal2->index.time.hh = unsigned char(CharToInt(&cdValue2->date[8], 2));
		cgVal2->index.time.mm = unsigned char(CharToInt(&cdValue2->date[10], 2));
		cgVal2->index.time.ss = unsigned char(CharToInt(&cdValue2->date[12], 2));
		dPos += nSize;
	}

	return nSize;
}

bool CGrp_Data::UpdateRTM(bool &bChangeDisplay)
{
	bool	bResponse = false;
	char	indexRTM[16]{};
	int	checkGAP{};
	int	hhRTM{}, mmRTM{}, ssRTM{};
	struct _cgVal2* gVal2{};
	int	endIDX = m_DataQue.GetSize() - 1;
	const int	nSize = sz_cgVAL2;
	bool	bCreate = false;
	char*	pData = NULL;
	bool	bAdd = false;

	bChangeDisplay = false;
	sprintf(indexRTM, "%s", m_pGrpWnd->m_ctim.GetString()); 
	if (m_DataQue.GetSize() < 1)
	{
		if (m_dIndex != CDI_MIN && m_dIndex != CDI_TICK)
			return false;
//		if (m_pGrpWnd->m_bCtrl)
//			return false;

		hhRTM = short(CharToInt(&indexRTM[0], 2));
		mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
		ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));

		pData = new char[m_szFrame];
		ZeroMemory(pData, m_szFrame);
		gVal2 = (struct _cgVal2 *) pData;	
		m_DataQue.Add((char*)gVal2);

		gVal2->val[0] = atoi(m_pGrpWnd->m_val[0]);
		gVal2->val[1] = atoi(m_pGrpWnd->m_val[1]);
		gVal2->index.time.hh = hhRTM;
		gVal2->index.time.mm = mmRTM;

		if (m_dGap >= 60 || ssRTM < 30)
			gVal2->index.time.ss = 0;
		else
			gVal2->index.time.ss = 30;

		m_pGrpWnd->m_totalDay = m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = m_DataQue.GetSize();
		m_pGrpWnd->m_dispPos = 0;			

		return true;
	}

	if (m_DataQue.GetSize() >= 1)
	{
		switch (m_dIndex)
		{
		case CDI_DAY:
		case CDI_WEEK:
		case CDI_MON:
			if (strlen(indexRTM) != 6)
				return bResponse;
			break;
		case CDI_MIN:
			if (strlen(indexRTM) != 6)
				return bResponse;

			hhRTM = short(CharToInt(&indexRTM[0], 2));
			mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
			ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));

			gVal2 = (struct _cgVal2 *)m_DataQue.GetAt(endIDX);
			checkGAP = (hhRTM - gVal2->index.time.hh) * 3600;
			checkGAP += (mmRTM - gVal2->index.time.mm) * 60;
			checkGAP += (ssRTM - gVal2->index.time.ss);

			if (checkGAP < 0 || m_dGap <= checkGAP)
				bCreate = true;
			break;
		case CDI_TICK:
			hhRTM = short(CharToInt(&indexRTM[0], 2));
			mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
			ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));
			bCreate = true;
			break;
		default:
			return bResponse;
		}

		if (bCreate)
		{
			pData = new char[m_szFrame];
			ZeroMemory(pData, m_szFrame);
			gVal2 = (struct _cgVal2 *) pData;		
		}
	}
	else
	{
		bCreate = true;
		pData = new char[m_szFrame];
		ZeroMemory(pData, m_szFrame);
		gVal2 = (struct _cgVal2 *) pData;	
		m_DataQue.Add((char*)gVal2);
		endIDX = m_DataQue.GetSize() - 1;
		bAdd = true;
	}

	struct _cgVal2 *gLastVal2 = (struct _cgVal2 *)m_DataQue.GetAt(endIDX);

	switch (m_dIndex)
	{	
	case CDI_MIN:
		if (bCreate)
		{			
			gVal2->val[0] = atoi(m_pGrpWnd->m_val[0]);
			gVal2->val[1] = atoi(m_pGrpWnd->m_val[1]);
			gVal2->index.time.hh = hhRTM;
			gVal2->index.time.mm = mmRTM;

			if (m_dGap >= 60 || ssRTM < 30)
				gVal2->index.time.ss = 0;
			else
				gVal2->index.time.ss = 30;
		}
		else
		{
			const int	jgga = atoi(m_pGrpWnd->m_val[0]);
			gLastVal2->val[0] = jgga;
			if (gLastVal2->val[0] > jgga)	gLastVal2->val[0] = jgga;
			if (gLastVal2->val[0] < jgga)	gLastVal2->val[0] = jgga;
			gLastVal2->val[1] += atof(m_pGrpWnd->m_val[1]);
		}
		break;
	case CDI_DAY:
		gLastVal2->val[0] = atoi(m_pGrpWnd->m_val[0]);
		gLastVal2->val[1] = atoi(m_pGrpWnd->m_val[1]);
		break;
	default:
		{
/*
			gLastVal2->val[0] = jgga;
			if (gLastVal2->val[0] > jgga)
				gLastVal2->val[0] = jgga;
			if (gLastVal2->val[0] < jgga)
				gLastVal2->val[0] = jgga;
			gLastVal2->val[1] += atof(m_pGrpWnd->m_val[1]);
*/
			const int jgga = atoi(m_pGrpWnd->m_val[0]);
			gVal2->val[0] = jgga;
			if (gVal2->val[0] > jgga)
				gVal2->val[0] = jgga;
			if (gVal2->val[0] < jgga)
				gVal2->val[0] = jgga;
			gVal2->val[1] += atof(m_pGrpWnd->m_val[1]);

		}
		break;
	}

	if (bCreate)
	{
		struct _cgIndex*	pIndex = &(((struct _cgVal2*)pData)->index);
		pIndex->date.yy = short(CharToInt(&m_pGrpWnd->m_szDate[0], 4));
		pIndex->date.mm = unsigned char(CharToInt(&m_pGrpWnd->m_szDate[4], 2));
		pIndex->date.dd = unsigned char(CharToInt(&m_pGrpWnd->m_szDate[6], 2));	
		
		// Ãß°¡
		pIndex->time.hh = hhRTM;
		pIndex->time.mm = mmRTM;
		pIndex->time.ss = ssRTM;	

	}
	// insert new data at last index
	if (bCreate && !bAdd)
	{
		m_DataQue.Add(pData);

		if (m_pGrpWnd->m_bCtrl)
		{
			if (m_pGrpWnd->m_dCount < m_DataQue.GetSize())
			{
				pData = m_DataQue.GetAt(0);
				delete [] pData;
				m_DataQue.RemoveAt(0);
			}
			else
			{
				m_pGrpWnd->m_totalDay = m_DataQue.GetSize();
				if (m_pGrpWnd->m_InitDataCnt == 0 && m_pGrpWnd->m_totalDay < m_pGrpWnd->m_dDay)
				{
					m_pGrpWnd->m_dispPos = 0;
					m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = m_DataQue.GetSize();
				}
				else
				{
					m_pGrpWnd->m_dispPos++;
					m_pGrpWnd->m_dispEnd++;
				}
				bChangeDisplay = true;
			}
		}
		else
		{
			if (m_pGrpWnd->m_dCount < m_DataQue.GetSize())
			{
				pData = m_DataQue.GetAt(0);
				delete [] pData;
				m_DataQue.RemoveAt(0);
			}

			m_pGrpWnd->m_totalDay = m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = m_DataQue.GetSize();
			m_pGrpWnd->m_dispPos = 0;			
		}

		bResponse = true;
	}

	return bResponse;
}

bool CGrp_Data::CreateDataQue(int count)
{
	if (m_szFrame <= 0)
		return false;

	char* pData{};
	for (int ii = 0; ii < count; ii++)
	{
		pData = new char[m_szFrame];
		ZeroMemory(pData, m_szFrame);
		m_DataQue.Add(pData);
	}

	return true;
}

bool CGrp_Data::AddGraphData(char* data, int dLen)
{
	char* pData = new char [dLen + 1];

	CopyMemory(pData, data, dLen);
	pData[dLen] = '\0';

	m_DataQue.Add(pData);

	return true;
}

bool CGrp_Data::AddGraphData(int index, char* data, int dLen)
{
	if (index < 0 || index > m_DataQue.GetUpperBound())
		return false;

	char* pData = new char [dLen + 1];
	CopyMemory(pData, data, dLen);
	pData[dLen] = '\0';

	m_DataQue.InsertAt(index, pData);

	return true;
}

char* CGrp_Data::GetGraphData(int index)
{
	if (index < 0 || index > m_DataQue.GetUpperBound())
		return (char *) 0;

	return m_DataQue.GetAt(index);
}

int CGrp_Data::CharToInt(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';

	return atoi(rWb);
}

double CGrp_Data::CharToDouble(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';

	return atof(rWb);
}
