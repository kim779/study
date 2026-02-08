// Grp_Data.cpp: implementation of the CGrp_Data class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_TickEx.h"
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
	m_szFrame = sz_cgTickEx;
}

CGrp_Data::~CGrp_Data()
{
	_vDataQue.clear();
}

int CGrp_Data::AttachGraphData(char* data, int count, int mgap, int lastTick)
{
	m_dGap = mgap;
	m_iLastTick = lastTick;

	if (count == 0)
		return m_szFrame;

	const int pjgga = m_pGrpWnd->m_pJgga;
	const gsl::span<struct _cdTick2> arrspan((struct _cdTick2*)data, count);
	for_each(arrspan.begin(), arrspan.end(), [&](const auto item) {
		auto& it = _vDataQue.emplace_back(std::make_unique<struct _cgTickEx>());

		it->epr = CharToInt(CString(item.epr, sizeof(item.epr)));
		it->trn = CharToInt(CString(item.trn, sizeof(item.trn)));
		it->time.hh = unsigned char(CharToInt(CString(&item.date[0], 2)));
		it->time.mm = unsigned char(CharToInt(CString(&item.date[2], 2)));
		it->time.ss = unsigned char(CharToInt(CString(&item.date[4], 2)));
	});
	return sz_cdTick2;
}

bool CGrp_Data::UpdateRTM()
{
	char	indexRTM[16]{};
	int	hhRTM{}, mmRTM{}, ssRTM{};

	sprintf(indexRTM, "%s", m_pGrpWnd->m_ctim.GetString());
	if (strlen(indexRTM) != 6)
		return false;

	hhRTM = short(CharToInt(CString(&indexRTM[0], 2)));
	mmRTM = unsigned char(CharToInt(CString(&indexRTM[2], 2)));
	ssRTM = unsigned char(CharToInt(CString(&indexRTM[4], 2)));

	const int endIDX = _vDataQue.size() - 1;
	if (endIDX < 0)
	{
		auto& gTickEx = _vDataQue.emplace_back(std::make_unique<struct _cgTickEx>());
		gTickEx->epr = atoi(m_pGrpWnd->m_jgga);
		gTickEx->trn = atoi(m_pGrpWnd->m_cvol);
		gTickEx->time.hh = hhRTM;
		gTickEx->time.mm = mmRTM;
		gTickEx->time.ss = ssRTM;
		return true;
	}

	if (m_dIndex == CDI_TICK)
	{
		if (m_iLastTick < m_dGap)
		{
			auto& gTickEx = _vDataQue.at(endIDX);
			gTickEx->epr = atoi(m_pGrpWnd->m_jgga);
			gTickEx->trn += atoi(m_pGrpWnd->m_cvol);

			m_iLastTick++;
			return false;
		}
		else
		{
			auto& gTickEx = _vDataQue.emplace_back(std::make_unique<struct _cgTickEx>());
			gTickEx->epr = atoi(m_pGrpWnd->m_jgga);
			gTickEx->trn = atoi(m_pGrpWnd->m_cvol);
			gTickEx->time.hh = hhRTM;
			gTickEx->time.mm = mmRTM;
			gTickEx->time.ss = ssRTM;

			m_iLastTick = 1;

			if (!m_pGrpWnd->m_bExpand)	// 무한 추가 여부
			{
				_vDataQue.erase(_vDataQue.begin());
				return false;
			}
		}
	}
	else
	{
		bool	bCreate = false;
		int	checkGAP{};

		hhRTM = short(CharToInt(CString(&indexRTM[0], 2)));
		mmRTM = unsigned char(CharToInt(CString(&indexRTM[2], 2)));
		ssRTM = unsigned char(CharToInt(CString(&indexRTM[4], 2)));

		auto& gTickEx = _vDataQue.at(endIDX);
		checkGAP = (hhRTM - gTickEx->time.hh) * 3600;
		checkGAP += (mmRTM - gTickEx->time.mm) * 60;
		checkGAP += (ssRTM - gTickEx->time.ss);

		if (checkGAP < 0 || m_dGap <= checkGAP)
			bCreate = true;

		if (bCreate)
		{
			auto& gTick = _vDataQue.emplace_back(std::make_unique<struct _cgTickEx>());
			gTick->epr = atoi(m_pGrpWnd->m_jgga);
			gTick->trn = atoi(m_pGrpWnd->m_cvol);
			gTick->time.hh = hhRTM;
			gTick->time.mm = mmRTM;
			gTick->time.ss = ssRTM;

			if (!m_pGrpWnd->m_bExpand)	// 무한 추가 여부
			{
				_vDataQue.erase(_vDataQue.begin());
			}
		}
		else
		{
			gTickEx->epr = atoi(m_pGrpWnd->m_jgga);
			gTickEx->trn = atoi(m_pGrpWnd->m_cvol);
			gTickEx->time.hh = hhRTM;
			gTickEx->time.mm = mmRTM;
			gTickEx->time.ss = 0;

			return false;
		}
	}

	return true;
}



struct _cgTickEx* CGrp_Data::GetGraphData(size_t index)
{
	if (index < 0 || index > _vDataQue.size() - 1)
		return nullptr;

	return _vDataQue.at(index).get();
}

_cgTickEx* CGrp_Data::GetLastData()
{
	const int endIDX = _vDataQue.size() - 1;
	if (endIDX < 0)
		return nullptr;

	return (_vDataQue.rbegin())->get();
}

int CGrp_Data::CharToInt(CString sdata)
{
	sdata.Trim();
	return atoi(sdata);
}

double CGrp_Data::CharToDouble(CString sdata)
{
	sdata.Trim();
	return atof(sdata);
}
