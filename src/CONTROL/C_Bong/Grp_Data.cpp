// Grp_Data.cpp: implementation of the CGrp_Data class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_Bong.h"
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

	m_szFrame = sz_cgBong;
}

CGrp_Data::~CGrp_Data()
{
	char *pData;
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

	struct _cdBong	*cdBong;
	struct _cgBong	*cgBong;

	int	dPos = 0;
	char	*pQue;
	for (int ii = 0; ii < count; ii++)
	{
		pQue = m_DataQue.GetAt(ii);

		cgBong = (struct _cgBong*)pQue;
		cdBong = (struct _cdBong*) &data[dPos];

		cgBong->epr = CharToInt(cdBong->epr, sizeof(cdBong->epr));
		cgBong->fpr = CharToInt(cdBong->fpr, sizeof(cdBong->fpr));
		cgBong->hpr = CharToInt(cdBong->hpr, sizeof(cdBong->hpr));
		cgBong->lpr = CharToInt(cdBong->lpr, sizeof(cdBong->lpr));
		cgBong->trn = CharToDouble(cdBong->trn, sizeof(cdBong->trn));

		cgBong->index.date.yy = short(CharToInt(&cdBong->date[0], 4));
		cgBong->index.date.mm = unsigned char(CharToInt(&cdBong->date[4], 2));
		cgBong->index.date.dd = unsigned char(CharToInt(&cdBong->date[6], 2));
		cgBong->index.time.hh = unsigned char(CharToInt(&cdBong->date[8], 2));
		cgBong->index.time.mm = unsigned char(CharToInt(&cdBong->date[10], 2));
		cgBong->index.time.ss = unsigned char(CharToInt(&cdBong->date[12], 2));

		dPos += sz_cdBong;
	}

	return sz_cdBong;
}

bool CGrp_Data::UpdateRTM()
{
	if (m_DataQue.GetSize() < 1)
		return false;

	bool	bResponse = false;
	char	indexRTM[16];
	int	checkGAP;
	int	hhRTM, mmRTM, ssRTM;

	struct _cgBong	*gBong;
	int endIDX = m_DataQue.GetSize() - 1;

	bool	bCreate = false;
	sprintf(indexRTM, "%s", m_pGrpWnd->m_ctim);
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

		hhRTM = unsigned char(CharToInt(&indexRTM[0], 2));
		mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
		ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));

		gBong = (struct _cgBong *)m_DataQue.GetAt(endIDX);
		checkGAP = (hhRTM - gBong->index.time.hh) * 3600;
		checkGAP += (mmRTM - gBong->index.time.mm) * 60;
		checkGAP += (ssRTM - gBong->index.time.ss);

		if (checkGAP < 0 || m_dGap <= checkGAP)
			bCreate = true;
		break;
	case CDI_TICK:
		hhRTM = unsigned char(CharToInt(&indexRTM[0], 2));
		mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
		ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));

		bCreate = true;
		break;
	default:
		return bResponse;
	}

	char*	pData = (char *) 0;
	if (bCreate)
	{
		pData = new char[sz_cgBong];
		ZeroMemory(pData, sz_cgBong);
		gBong = (struct _cgBong *) pData;
	}

	struct _cgBong *gLastBong = (struct _cgBong *)m_DataQue.GetAt(endIDX);

	switch (m_dIndex)
	{
	case CDI_TICK:
		gBong->fpr = gBong->hpr = gBong->lpr = gBong->epr = atoi(m_pGrpWnd->m_jgga);
		gBong->trn = atoi(m_pGrpWnd->m_gvol);
		gBong->index.date.yy = m_yy;
		gBong->index.date.mm = m_mm;
		gBong->index.date.dd = m_dd;
		gBong->index.time.hh = hhRTM;
		gBong->index.time.mm = mmRTM;
		gBong->index.time.ss = ssRTM;

		bResponse = true;
		break;
	case CDI_MIN:
		if (bCreate)
		{
			gBong->fpr = gBong->hpr = gBong->lpr = gBong->epr = atoi(m_pGrpWnd->m_jgga);
			gBong->trn = atoi(m_pGrpWnd->m_gvol);
			gBong->index.date.yy = m_yy;
			gBong->index.date.mm = m_mm;
			gBong->index.date.dd = m_dd;
			
			int	hh, mm;
			GetTime(m_dGap, gLastBong->index.time.hh, gLastBong->index.time.mm, 
				hhRTM, mmRTM, hh, mm);
			gBong->index.time.hh = hh;
			gBong->index.time.mm = mm;
			if (m_dGap >= 60 || ssRTM < 30)
				gBong->index.time.ss = 0;
			else
				gBong->index.time.ss = 30;
		}
		else
		{
			int	jgga = atoi(m_pGrpWnd->m_jgga);
			gLastBong->epr = jgga;
			if (gLastBong->lpr > jgga)	gLastBong->lpr = jgga;
			if (gLastBong->hpr < jgga)	gLastBong->hpr = jgga;
			gLastBong->trn += atof(m_pGrpWnd->m_gvol);
		}
		break;
	case CDI_DAY:
		if (m_pGrpWnd->m_siga.IsEmpty())
		{
			int	jgga = atoi(m_pGrpWnd->m_jgga);
			if (gLastBong->lpr > jgga)	gLastBong->lpr = jgga;
			if (gLastBong->hpr < jgga)	gLastBong->hpr = jgga;
		}
		else
		{
			gLastBong->fpr = atoi(m_pGrpWnd->m_siga);
			gLastBong->lpr = atoi(m_pGrpWnd->m_jega);
			gLastBong->hpr = atoi(m_pGrpWnd->m_koga);
		}
		gLastBong->epr = atoi(m_pGrpWnd->m_jgga);
		gLastBong->trn = atof(m_pGrpWnd->m_gvol);
		break;
	default:
		{
			int	jgga = atoi(m_pGrpWnd->m_jgga);
			gLastBong->epr = jgga;
			if (gLastBong->lpr > jgga)	gLastBong->lpr = jgga;
			if (gLastBong->hpr < jgga)	gLastBong->hpr = jgga;
			gLastBong->trn += atof(m_pGrpWnd->m_gvol);
		}
		break;
	}

	// insert new data at last index
	if (bCreate)
	{
		m_DataQue.Add(pData);

		pData = m_DataQue.GetAt(0);
		delete [] pData;
		m_DataQue.RemoveAt(0);

		bResponse = true;
	}

	return bResponse;
}

bool CGrp_Data::CreateDataQue(int count)
{
	if (m_szFrame <= 0)
		return false;

	char	*pData;
	for (int ii = 0; ii < count; ii++)
	{
		pData = new char[m_szFrame];
		ZeroMemory(pData, m_szFrame);
		m_DataQue.Add(pData);
	}

	return true;
}

char* CGrp_Data::GetGraphData(int index)
{
	if (index < 0 || index > m_DataQue.GetUpperBound())
		return (char *) 0;

	return m_DataQue.GetAt(index);
}

void CGrp_Data::SetDate(char* pDate)
{
	m_yy = short(CharToInt(&pDate[0], 4));
	m_mm = unsigned char(CharToInt(&pDate[4], 2));
	m_dd = unsigned char(CharToInt(&pDate[6], 2));
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

void CGrp_Data::GetTime(int sec, int pHH, int pMM, int cHH, int cMM, int &nHH, int &nMM)
{
	if (sec <= 30)	// 30초봉
	{
		nHH = cHH;
		nMM = cMM;
		return;
	}
	int	min = sec/60;

	int	pTime = pHH*60 + pMM;
	int	cTime = cHH*60 + cMM;
	int	Gap = cTime - pTime;

	if (Gap <= 0)	// 마지막 데이터가 하루전 데이터
	{
		nHH = cHH;
		nMM = cMM;
		return;	
	}

	int	nTerm = Gap / min;
	pMM += nTerm * min;

	nHH = pHH + pMM / 60;
	nMM = pMM % 60;
}
