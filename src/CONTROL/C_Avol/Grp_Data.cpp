#include "stdafx.h"
#include "C_Avol.h"
#include "Grp_Data.h"
#include "GrpWnd.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGrp_Data::CGrp_Data(class CGrpWnd *pGrpWnd, int dKind, int dKey, int dIndex)
{
	m_pGrpWnd = pGrpWnd;
	m_dKind = dKind;
	m_dKey = dKey;
	m_dIndex = dIndex;
	m_szFrame = sz_cgBong;
}

CGrp_Data::~CGrp_Data()
{
	_DataQue.clear();
}

int CGrp_Data::AttachGraphData(char* data, int count, int mgap)
{
	if (m_szFrame <= 0)
		return false;

	m_dGap = mgap;

//	if (!CreateDataQue(count))
//		return -1;

	struct	_cgBong*	cgBong = nullptr;	
	const int		nSize = sz_cdBong;
	
	struct	_cdBong* cdBong = (struct _cdBong*)&data;
	for (int ii = 0; ii < count; ii++, cdBong++)
	{
//		pQue = m_DataQue.GetAt(ii);

		auto pQue = _DataQue.emplace_back(CString(' ', m_szFrame));
		cgBong = (struct _cgBong*)pQue.GetString();
	
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

		//dPos += nSize;
	}

	return nSize;
}

bool CGrp_Data::UpdateRTM()
{
	bool	bResponse = false;
	char	indexRTM[16]{};
	int	checkGAP{};
	int	hhRTM{}, mmRTM{}, ssRTM{};

	struct	_cgBong* pInput{};
	const int endIDX = _DataQue.size() - 1;

	bool	bCreate = false;
	sprintf(indexRTM, "%s", m_pGrpWnd->m_ctim.GetString());
	//char*	pData = (char *) 0;

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

		pInput = (struct _cgBong *)_DataQue.at(endIDX).GetString();
		checkGAP = (hhRTM - pInput->index.time.hh) * 3600;
		checkGAP += (mmRTM - pInput->index.time.mm) * 60;
		checkGAP += (ssRTM - pInput->index.time.ss);

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
		//pData = new char[m_szFrame];
		//ZeroMemory(pData, m_szFrame);
		pInput = (struct _cgBong *)CString(' ', m_szFrame).GetString();
	}

	char*	pQue = nullptr;		
	pQue = _DataQue.at(endIDX).GetBuffer();	
	struct _cgBong* gLastInput = (struct _cgBong *)pQue;	
	
	switch (m_dIndex)
	{	
	case CDI_MIN:
		if (bCreate)
		{
			pInput->fpr = pInput->hpr = pInput->lpr = pInput->epr = atoi(m_pGrpWnd->m_jgga);
			pInput->trn = atoi(m_pGrpWnd->m_gvol);
			pInput->index.time.hh = hhRTM;
			pInput->index.time.mm = mmRTM;

			if (m_dGap < 60)
			{
				const int	ss = ssRTM;
				int	mok = 0;
				mok = ss / m_dGap;
				pInput->index.time.ss = mok * m_dGap;
			}
			else
				pInput->index.time.ss = 0;

		}
		else
		{
			const int	jgga = atoi(m_pGrpWnd->m_jgga);
			gLastInput->epr = jgga;
			if (gLastInput->lpr > jgga)	gLastInput->lpr = jgga;
			if (gLastInput->hpr < jgga)	gLastInput->hpr = jgga;
			gLastInput->trn += atof(m_pGrpWnd->m_gvol);			
		}
		break;
	case CDI_DAY:
		gLastInput->fpr = atoi(m_pGrpWnd->m_siga);
		gLastInput->lpr = atoi(m_pGrpWnd->m_jega);
		gLastInput->hpr = atoi(m_pGrpWnd->m_koga);
		gLastInput->epr = atoi(m_pGrpWnd->m_jgga);
		gLastInput->trn = atof(m_pGrpWnd->m_gvol);		
		break;
	case CDI_TICK:
		{
			pInput->index.time.hh = hhRTM;
			pInput->index.time.mm = mmRTM;
			pInput->index.time.ss = ssRTM;
			const int	jgga = atoi(m_pGrpWnd->m_jgga);
			pInput->epr = jgga;
			if (pInput->lpr > jgga)	pInput->lpr = jgga;
			if (pInput->hpr < jgga)	pInput->hpr = jgga;
			pInput->trn = atof(m_pGrpWnd->m_gvol);			
		}
		break;
	}

	if (bCreate)
	{
		struct _cgIndex*	pIndex = &(pInput->index);
		pIndex->date.yy = short(CharToInt(&m_pGrpWnd->m_szDate[0], 4));
		pIndex->date.mm = unsigned char(CharToInt(&m_pGrpWnd->m_szDate[4], 2));
		pIndex->date.dd = unsigned char(CharToInt(&m_pGrpWnd->m_szDate[6], 2));		

		_DataQue.emplace_back((char*)pInput);

		if (m_pGrpWnd->m_bCtrl)
		{
			//pData = _DataQue.at(0);
			//delete [] pData;
			_DataQue.erase(_DataQue.begin());
		}
		else
		{
			if (m_pGrpWnd->m_dCount < (int)_DataQue.size())
			{
				_DataQue.erase(_DataQue.begin());
			}

			m_pGrpWnd->m_totalDay = m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = gsl::narrow_cast<int>(_DataQue.size());
			m_pGrpWnd->m_dispPos = 0;			
		}
		
		bResponse = true;		
	}

	return bResponse;
}

//bool CGrp_Data::CreateDataQue(int count)
//{
//	if (m_szFrame <= 0)
//		return false;
//
//	char* pData{};
//	for (int ii = 0; ii < count; ii++)
//	{
//		pData = new char[m_szFrame];
//		ZeroMemory(pData, m_szFrame);
//		m_DataQue.Add(pData);
//	}
//
//	return true;
//}

char* CGrp_Data::GetGraphData(int index)
{
	if (index < 0 || index >= gsl::narrow_cast<int>(_DataQue.size()))
		return (char *) 0;

	return _DataQue.at(index).GetBuffer();
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
