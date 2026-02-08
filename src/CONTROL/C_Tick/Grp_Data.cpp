// Grp_Data.cpp: implementation of the CGrp_Data class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_Tick.h"
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
	m_szFrame = sz_cgTick;
}

CGrp_Data::~CGrp_Data()
{
	_vDataQue.clear();
}

int CGrp_Data::AttachGraphData(char* data, int count, int mgap)
{
	m_dGap = mgap;
	if (count == 0)
		return m_szFrame;

	const int	pjgga = m_pGrpWnd->m_pJgga;
	const gsl::span<struct _cdTick> sdata((struct _cdTick*)data, count);
	for_each(sdata.begin(), sdata.end(), [&](const auto item) {
		auto& it = _vDataQue.emplace_back(std::make_unique<struct _cgTick>());	
		it->epr  = atoi(CString((char*)item.epr, 7));
		it->diff = it->epr - pjgga;
		it->udyl = (it->diff * 100) / gsl::narrow_cast<float>(pjgga);
		it->time.hh = unsigned char(CharToInt(&item.date[0], 2));
		it->time.mm = unsigned char(CharToInt(&item.date[2], 2));
		it->time.ss = unsigned char(CharToInt(&item.date[4], 2));
	});
	return sz_cdTick;
}

bool CGrp_Data::UpdateRTM()
{
	char	indexRTM[16]{};
	int	hhRTM{}, mmRTM{}, ssRTM{};

	sprintf(indexRTM, "%s", m_pGrpWnd->m_ctim.GetString());
	hhRTM = short(CharToInt(&indexRTM[0], 2));
	mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
	ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));

	struct _cgTick	*gTick{};
	if (m_dIndex == CDI_TICK)
	{
		auto& gTick = _vDataQue.emplace_back(std::make_unique<struct _cgTick>());
		gTick->epr = atoi(m_pGrpWnd->m_jgga);
		gTick->diff = atoi(m_pGrpWnd->m_diff);
		gTick->udyl = float(atof(m_pGrpWnd->m_udyl));
		gTick->time.hh = hhRTM;
		gTick->time.mm = mmRTM;
		gTick->time.ss = ssRTM;

		if (!m_pGrpWnd->m_bExpand)
		{
			if (m_pGrpWnd->m_dCount < gsl::narrow_cast<int>(_vDataQue.size()))
			{
				_vDataQue.erase(_vDataQue.begin());
			}
			else
			{
				m_pGrpWnd->m_totalDay = _vDataQue.size();
				m_pGrpWnd->m_dispDay = m_pGrpWnd->m_totalDay;
				m_pGrpWnd->m_dispPos = 0;
				m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_totalDay;
			}
		}
	}
	else
	{
		const size_t endIDX = _vDataQue.size() - 1;
		bool	bCreate = false;
		int	checkGAP{};
		if (strlen(indexRTM) != 6)
			return 0;

		hhRTM = short(CharToInt(&indexRTM[0], 2));
		mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
		ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));

		if (endIDX >= 0)
		{
			gTick = (struct _cgTick *)_vDataQue.at(endIDX).get();
			checkGAP = (hhRTM - gTick->time.hh) * 3600;
			checkGAP += (mmRTM - gTick->time.mm) * 60;
			checkGAP += (ssRTM - gTick->time.ss);

			if (checkGAP < 0 || m_dGap <= checkGAP)
				bCreate = true;
		}
		else
			bCreate = true;

		if (bCreate)
		{
			auto tick = _vDataQue.emplace_back(std::make_unique<struct _cgTick>());
			tick->epr = atoi(m_pGrpWnd->m_jgga);
			tick->diff = atoi(m_pGrpWnd->m_diff);
			tick->udyl = float(atof(m_pGrpWnd->m_udyl));
			tick->time.hh = hhRTM;
			tick->time.mm = mmRTM;
			tick->time.ss = ssRTM;

			if (!m_pGrpWnd->m_bExpand)
			{
				_vDataQue.erase(_vDataQue.begin());
			}
		}
		else
		{
			if (gTick)
			{
				gTick->epr = atoi(m_pGrpWnd->m_jgga);
				gTick->diff = atoi(m_pGrpWnd->m_diff);
				gTick->udyl = float(atof(m_pGrpWnd->m_udyl));
				gTick->time.hh = hhRTM;
				gTick->time.mm = mmRTM;
				gTick->time.ss = 0;
			}
			return false;
		}
	}

	return true;
}

struct _cgTick* CGrp_Data::GetGraphData(size_t index)
{
	if (index < 0 || index > _vDataQue.size() - 1)
		return nullptr;

	return _vDataQue.at(index).get();
}

int CGrp_Data::CharToInt(const char* cnvB, int cnvL)
{
	return atoi(CString(cnvB, cnvL).Trim());
}

double CGrp_Data::CharToDouble(const char* cnvB, int cnvL)
{
	return atof(CString(cnvB, cnvL).Trim());
}
