// GrpData.cpp: implementation of the CGrpData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cx_MiniChart.h"
#include "GrpData.h"
#include "GrpWnd.h"
#include "MainWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	stHH	9
#define	stMM	0
#define	edHH	15
#define	edMM	30

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrpData::CGrpData(class CGrpWnd *pGrpWnd)
{
	m_pGrpWnd = pGrpWnd;
	m_dIndex = CDI_MIN;
	m_dGap = 60;


	m_szFrame = sz_CGMJ;
	m_startTime = 9*60;
}

CGrpData::~CGrpData()
{
	_vDataQue.clear();
}

int CGrpData::AttachGraphData(int index, bool bIndex, int count, int discard, char *pData, CString code, CString siga, CString jgga)
{
	char	token[32]{}, * pQue{};
	struct _cgMJ* cgMJ{};
	int	dPos = 0;

	if (bIndex)
	{
		if (m_pGrpWnd->m_mj.fixmode)
		{
			m_startTime = stHH*60 + stMM;
			int total = (edHH - stHH)*60 + (edMM - stMM) + 1;
			if (count > 0)
			{
				GetToken(&pData[dPos], token);
				const int hh = unsigned char(CharToInt(&token[0], 2));
				const int mm = unsigned char(CharToInt(&token[2], 2));
				m_startTime = hh*60 + mm;
				if (total < count)
					total = count;
			}
			
			if (!CreateDataQue(total))	return -1;
		}
		else
		{
			if (!CreateDataQue(count))
				return -1;

			for (int ii = 0; ii < count; ii++)
			{
				const auto& cgMJ = _vDataQue.at(ii);

				//cgMJ = (struct _cgMJ*)pQue;
				for (int jj = 0; jj < 2; jj++)
				{
					const int tLen = GetToken(&pData[dPos], token);
					dPos += tLen;

					switch (jj)
					{
					case 0:
						cgMJ->time.hh = unsigned char(CharToInt(&token[0], 2));
						cgMJ->time.mm = unsigned char(CharToInt(&token[2], 2));
						cgMJ->time.ss = unsigned char(CharToInt(&token[4], 2));

						if (ii == 0)
							m_startTime = cgMJ->time.hh * 60 + cgMJ->time.mm;
						break;
					case 1:
						break;
					}
				}
			}
		}

		return 0;
	}

	m_pDigit[index] = 2;
	m_pJgga[index] = atoi(jgga);
	m_pDsga[index] = atoi(siga);
	m_pName[index].Format("%s", m_pGrpWnd->m_mj.jisu[index].name);
	m_pCode[index].Format("%s", code);
	m_pSymbol[index].Format("%s", R_CURR);

	m_pName[index].TrimLeft(" ");		m_pName[index].TrimRight(" ");
	m_pCode[index].TrimLeft(" ");		m_pCode[index].TrimRight(" ");
	m_pSymbol[index].TrimLeft(" ");	m_pSymbol[index].TrimRight(" ");

	int	cmp{};
	if (m_pGrpWnd->m_mj.compare & CMP_CSIGA)
		cmp = atoi(siga);
	else
		cmp = atoi(jgga);

	int	iIndex{}, hh{}, mm{}, ss{};
	for (int ii = discard; ii < count; ii++)
	{
		for (int jj = 0; jj < 2; jj++)
		{
			const int tLen = GetToken(&pData[dPos], token);
			dPos += tLen;

			switch (jj)
			{
			case 0:
				hh = unsigned char(CharToInt(&token[0], 2));
				mm = unsigned char(CharToInt(&token[2], 2));
				ss = unsigned char(CharToInt(&token[4], 2));

				iIndex = GetInsertIndex(hh*60+mm);
				break;
			case 1:
				{
					const auto& cgMJ = _vDataQue.at(iIndex);
					cgMJ->val[index] = atoi(token);
				}
				break;
			}
		}

		const auto& cgMJ = _vDataQue.at(iIndex);
		if (cmp == 0)
		{
			cgMJ->ratio[index] = 0;
		}
		else
		{
			if (cgMJ->val[index] == 0)
				cgMJ->ratio[index] = 0;
			else
				cgMJ->ratio[index] = ((cgMJ->val[index] - cmp)*10000) / cmp;
		}
	}

	return m_szFrame;
}

bool CGrpData::UpdateRTM(int index, CString val, CString ctim)
{
	if (m_pDigit[index] > 0)
	{
		const double dVal = atof(val);
		if (dVal < 0)	val.Format("%.f", dVal * (-100));
		else		val.Format("%.f", dVal * 100);
	}
	bool	bResponse = false;

	char	indexRTM[16]{};
	int	checkGAP{};
	int	hhRTM{}, mmRTM{}, ssRTM{};


	bool	bCreate = false;
	sprintf(indexRTM, "%s", ctim.GetString());
	switch (m_dIndex)
	{
	case CDI_MIN:
	{
		if (strlen(indexRTM) != 6)
			return bResponse;

		hhRTM = unsigned char(CharToInt(&indexRTM[0], 2));
		mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
		ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));

		if (_vDataQue.size() <= 0)
		{
			bCreate = true;
			break;
		}

		const auto& cgMJ = *(_vDataQue.rbegin());
		checkGAP = (hhRTM - cgMJ->time.hh) * 3600;
		checkGAP += (mmRTM - cgMJ->time.mm) * 60;
		checkGAP += (ssRTM - cgMJ->time.ss);

		if (m_dGap <= checkGAP)
			bCreate = true;
		break;
	}
	default:
		return bResponse;
	}

	auto& gLastMJ = (bCreate == true) ? _vDataQue.emplace_back(std::make_unique<struct _cgMJ>()) : *(_vDataQue.rbegin());

	if (bCreate)
	{
		gLastMJ->time.hh = hhRTM;
		gLastMJ->time.mm = mmRTM;
		gLastMJ->time.ss = 0;
	}
	

	if (m_pDsga[index] == 0)	m_pDsga[index] = atoi(val);

	if (bCreate)
	{
		for (int kk = 0; kk < MAX_JISU; kk++)
		{
			if (kk == index)
			{
				const int cval = GetCompareVal(index);
				gLastMJ->val[index] = atoi(val);
				gLastMJ->ratio[index] = ((gLastMJ->val[index] - cval)*10000) / cval;
			}
			else
				gLastMJ->val[kk] = 0;
		}
	}
	else
	{
		int cval{};
		if (m_pGrpWnd->m_mj.compare & CMP_CSIGA)
		{
			if (m_pDsga[index] == 0)
				m_pDsga[index] = atoi(val);
			cval = m_pDsga[index];
		}
		else
		{
			cval = m_pJgga[index];
		}

		gLastMJ->val[index] = atoi(val);

		if (cval == 0)
			gLastMJ->ratio[index] = 0;
		else
			gLastMJ->ratio[index] = ((gLastMJ->val[index] - cval)*10000) / cval;
	}

	// insert new data at last index
	if (bCreate)
	{
		bResponse = true;
	}

	return bResponse;
}

bool CGrpData::CreateDataQue(int count)
{
	if (m_szFrame <= 0)
		return false;

	if (m_pGrpWnd->m_mj.fixmode)
	{
		for (int ii = 0; ii < count; ii++)
		{
			auto& item = _vDataQue.emplace_back(std::make_unique<struct _cgMJ>());
			item->time.hh = stHH + ii/60;
			item->time.mm = (stMM + ii)%60;		
		}
	}
	else
	{
		for (int ii = 0; ii < count; ii++)
			auto& item = _vDataQue.emplace_back(std::make_unique<struct _cgMJ>());
	}
	return true;
}

int CGrpData::GetCompareVal(int index)
{
	if (m_pGrpWnd->m_mj.compare & CMP_CSIGA)
		return m_pDsga[index];
	else
		return  m_pJgga[index];
}

struct _cgMJ* CGrpData::GetGraphData(int index)
{
	if (index < 0 || index > gsl::narrow_cast<int>(_vDataQue.size() - 1))
		return nullptr;

	return _vDataQue.at(index).get();
}

void CGrpData::Recalculate()
{
	char* pQue{};
	int	cmp[MAX_JISU]{};
	if (m_pGrpWnd->m_mj.compare & CMP_CSIGA)
		CopyMemory(cmp, m_pDsga, sizeof(cmp));
	else
		CopyMemory(cmp, m_pJgga, sizeof(cmp));

	for (size_t ii = 0; ii < _vDataQue.size(); ii++)
	{
		auto & cgMJ = _vDataQue.at(ii);
		for (int jj = 0; jj < MAX_JISU; jj++)
		{
			if (cmp[jj] == 0)	continue;

			if (cgMJ->val[jj] == 0)
				cgMJ->ratio[jj] = 0;
			else
				cgMJ->ratio[jj] = ((cgMJ->val[jj] - cmp[jj])*10000) / cmp[jj];
		}
	}
}

int CGrpData::GetToken(char *pData, char *token)
{
	int	idx = 0;
	token[0] = 0x00;
	for(;;)
	{
		if (idx >= 32)
			return 0;
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

int CGrpData::CharToInt(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';

	return atoi(rWb);
}

double CGrpData::CharToDouble(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';

	return atof(rWb);
}

int CGrpData::GetInsertIndex(int time)
{
	size_t	index = time - m_startTime;

	if (index < 0)
		index = 0;
	if (index >= _vDataQue.size())
		index = _vDataQue.size() - 1;

	return index;
}