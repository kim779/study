#include "stdafx.h"
#include "C_UserGrp.h"
#include "Grp_Data.h"
#include "GrpWnd.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGrp_Data::CGrp_Data(class CGrpWnd *pGrpWnd, int dIndex, int dGap)
{
	m_pGrpWnd = pGrpWnd;	
	m_szFrame = sz_DATA;
	m_dIndex = dIndex;
	m_dGap = dGap;
}

CGrp_Data::~CGrp_Data()
{
//	InitInfo(0);
	//m_arItemRts.RemoveAll();
	////_mData.clear();


	//for (auto& item : _vData)
	//	item->vData.clear();
	//_vData.clear();

}

int CGrp_Data::AttachGraphData(char* data, int nHead, int nData, int iSize, int dSize)
{
	m_bRealTime = false;
	int	nInfo = nHead;		// 실질적인 _cgNVal.val[n] 개수
	if (m_pGrpWnd->m_b3NChart)	// 3쌍을 만듦
		nInfo += 2;

	m_szFrame = sz_cgNVALx + (nInfo - 1)*sizeof(double);
//	if (!CreateDataQue(nData))
//		return -1;

	InitInfo(nInfo);

	int	dPos = 0;
	// index data
	struct _cgNVal	*gVal = nullptr;
	struct _DataInfo *cInfo = nullptr;
	if (m_pGrpWnd->m_bStringIndex)		// 비시계열 하단틱
	{
		CString	sIndex;

		m_arItemRts.RemoveAll();
		if (m_pGrpWnd->m_bItemRts)	// 항목별 실시간 처리심볼(하단틱 \t 심볼)
		{
			for (int ii = 0, index = 0; ii < nData ; ii++)
			{
				auto& item = _mData.emplace(std::make_pair(ii, std::make_unique<struct _cgNValx>()));
				sIndex.Format("%s", CString(&data[dPos], iSize));
				sIndex.Trim();
				index = sIndex.Find("\t");
				if (index > 0)
				{
					sprintf(item.first->second->index.name.str, "%s",  (char *)sIndex.Left(index).GetString());
					m_arItemRts.Add(sIndex.Mid(index+1));
				}
				dPos += iSize;
			}
		}
		else
		{
			for (int ii = 0 ; ii < nData ; ii++)
			{
				auto& item = _mData.emplace(std::make_pair(ii, std::make_unique<struct _cgNValx>()));
				sIndex.Format("%s", CString(&data[dPos], iSize));
				sIndex.Trim();
				sprintf(item.first->second->index.name.str, "%s", (char*)sIndex.GetString());
				
				dPos += iSize;
			}
		}
	}
	else
	{
		for (int ii = 0 ; ii < nData ; ii++)
		{
			auto& item = _mData.emplace(std::make_pair(ii, std::make_unique<struct _cgNValx>()));
			item.first->second->index.year.date.yy = short(CharToInt(&data[dPos], 4));
			item.first->second->index.year.date.mm = unsigned char(CharToInt(&data[dPos + 4], 2));
			item.first->second->index.year.date.dd = unsigned char(CharToInt(&data[dPos + 6], 2));
			item.first->second->index.year.time.hh = unsigned char(CharToInt(&data[dPos + 8], 2));
			item.first->second->index.year.time.mm = unsigned char(CharToInt(&data[dPos + 10], 2));
			item.first->second->index.year.time.ss = unsigned char(CharToInt(&data[dPos + 12], 2));
			dPos += iSize;
		}
	}

	// graph data
	int	dIndex = 0;
	for (int ii = 0; ii < nHead; ii++, dIndex++)
	{
		cInfo = (_DataInfo*)&data[dPos];
		dPos += sz_DATAINFO;
		m_pInfo[dIndex]->name.Format("%s", CString(cInfo->name, sizeof(cInfo->name)));	m_pInfo[dIndex]->name.TrimRight();
		m_pInfo[dIndex]->RtsCode.Format("%s", CString(cInfo->rtscode, sizeof(cInfo->rtscode)));	m_pInfo[dIndex]->RtsCode.TrimRight();
		m_pInfo[dIndex]->RtsSymbol.Format("%s", CString(cInfo->symbol, sizeof(cInfo->symbol)));	m_pInfo[dIndex]->RtsSymbol.TrimRight();
		switch (cInfo->rtsSign[0])
		{
		case '1':
			m_pInfo[dIndex]->RtsSign = RM_MINUS;
			break;			
		case '0':
			m_pInfo[dIndex]->RtsSign = RM_NO;
			break;			
		case ' ':
			m_pInfo[dIndex]->RtsSign = RM_REMOVE;
			break;
		}
		m_pInfo[dIndex]->digit = CharToInt(cInfo->digit, sizeof(cInfo->digit));
		m_pInfo[dIndex]->pivot = CharToInt(cInfo->pivot, sizeof(cInfo->pivot));
		if (m_pInfo[dIndex]->digit > 0)
			m_pInfo[dIndex]->power = pow(10.0, m_pInfo[dIndex]->digit);
		else
			m_pInfo[dIndex]->power = 1.;
		
		if (m_pGrpWnd->GetGrpKindFromConfig(ii) == IGK_2D3NBAR)
		{
			CString	str;
			for (int jj = 0; jj < nData; jj++)
			{
				const auto item = _mData.find(jj);
				if (item != _mData.end())
				{
					str = CString(&data[dPos], dSize);	
					GetNValue(item->second.get(), str, dIndex, 3);	// data \t data \t data
					dPos += dSize;
				}
			}
			dIndex += 2;	//
		}
		else
		{
			for (int jj = 0; jj < nData; jj++)
			{
				const auto& item = _mData.find(jj);
				item->second->vData.emplace_back( CharToDouble(&data[dPos], dSize));

				dPos += dSize;
			}
		}
	}

	for (auto& item : _mData)
	{
		_vData.emplace_back(item.second);
	}
	_mData.clear();

	// extra check
	if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
	{
		for (int ii = 0; ii < nInfo; ii++)
		{
			if (!m_pInfo[ii]->RtsCode.IsEmpty() && !m_pInfo[ii]->RtsSymbol.IsEmpty())
			{
				m_bRealTime = true;
				break;
			}
		}
	}

	if (m_pGrpWnd->m_bItemRts)	m_bRealTime = true;

	return dPos;
}

int CGrp_Data::RealTimeData(int index, CString sTime, CString sVal, bool bInsert)
{
	int	Response = RTS_NO;
	char	indexRTM[16]{};
	int	checkGAP = 0;
	const	int	endIDX = _vData.size() - 1;
	bool	bCreate = false;

	if (_vData.size() == 0)
		return -1;

	sprintf(indexRTM, "%s", (char *)sTime.GetString());
	if (strlen(indexRTM) != 6)
		return Response;

	const	int	hhRTM = unsigned char(CharToInt(&indexRTM[0], 2));
	const	int	mmRTM = unsigned char(CharToInt(&indexRTM[2], 2));
	const	int	ssRTM = unsigned char(CharToInt(&indexRTM[4], 2));

	const auto& gLastVal = *(_vData.rbegin());

	if (endIDX >= 0)
	{
		switch (m_dIndex)
		{
		case CDI_TICK:
			if (m_dGap == 0)	// 틱차트
			{
				if (bInsert)
					bCreate = true;
				break;
			}
		case CDI_MIN:
			{
				checkGAP =  (hhRTM - gLastVal->index.year.time.hh) * 3600;
				checkGAP += (mmRTM - gLastVal->index.year.time.mm) * 60;
				checkGAP += (ssRTM - gLastVal->index.year.time.ss);

				if (checkGAP < 0 || m_dGap <= checkGAP)
					bCreate = true;
			}
			break;
		default:
			break;;
		}		
	}
	else
	{
		bCreate = true;
	}
	
	const auto& gVal = bCreate ? _vData.emplace_back(std::make_shared<struct _cgNValx>()) : *_vData.rbegin();
	
	if (bCreate && _vData.size() > 1)
	{
		auto gLastVal = *(_vData.rbegin());	
		if (gLastVal->vData.size() <= 0)
			gLastVal = *(_vData.rbegin() + 1);
		
		gVal->vData = gLastVal->vData;
	}	

	switch (m_dIndex)
	{
	case CDI_TICK:
		if (bCreate)
		{	
			//gVal->vData.resize(index);
			gVal->vData[index] = atoi(sVal);
			gVal->index.year.time.hh = hhRTM;
			gVal->index.year.time.mm = mmRTM;
			gVal->index.year.time.ss = ssRTM;
			Response = RTS_INSERT;
			break;
		}
	case CDI_MIN:
		if (bCreate)
		{
			gVal->vData[index] = atoi(sVal);
			gVal->index.year.time.hh = hhRTM;
			gVal->index.year.time.mm = mmRTM;
			gVal->index.year.time.ss = ssRTM;
			if (m_dGap >= 60)
			{
				gVal->index.year.time.ss = 0;
			}
			else if (m_dGap % 10 == 0)
			{
				gVal->index.year.time.ss -= (gVal->index.year.time.ss / 10);
			}
			Response = RTS_INSERT;
		}
		else
		{			
			gVal->vData[index] = atof(sVal);
			Response = RTS_LAST;
		}
		break;
	case CDI_DAY:
	default:
		if (bCreate)
		{
			gVal->index.year.time.hh = hhRTM;
			gVal->index.year.time.mm = mmRTM;
			gVal->index.year.time.ss = ssRTM;
			gVal->vData[index] = atoi(sVal);
			Response = RTS_INSERT;
		}
		else
		{
			gVal->vData[index] = atof(sVal);
			Response = RTS_LAST;
		}
		break;
	}

	// insert new data at last index
	if (bCreate)
	{
		if (_vData.size() >= 2)
		{
			if (m_pGrpWnd->m_bCtrl)	// 20080911
			{
				// 요청 개수가 0이 아닐경우 요청개수만큼 쌓고 그 이상은 처음 삭제 끝에 추가
				if (m_pGrpWnd->m_ReqCnt > 0 && m_pGrpWnd->m_ReqCnt < gsl::narrow_cast<int>(_vData.size()))
				{
					_vData.erase(_vData.begin());
				}
				else if (m_pGrpWnd->m_ShowCnt > 0)	// 보여주는 개수 설정시
				{
					if (m_pGrpWnd->m_ShowCnt > m_pGrpWnd->m_totalDay && m_pGrpWnd->m_totalDay == m_pGrpWnd->m_dispDay)
					{
						m_pGrpWnd->m_totalDay = m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = _vData.size();
						m_pGrpWnd->m_dispPos = 0;
					}
					else				// 보여주는 위치만 변경
					{
						m_pGrpWnd->m_totalDay = _vData.size();
						m_pGrpWnd->m_dispEnd++;
						m_pGrpWnd->m_dispPos++;
					}
					m_pGrpWnd->MessageToParent(CM_CTRL, MAKEWPARAM(CTRL_UpdateScr, 0), 0);
				}
				else
				{
					if (30 > m_pGrpWnd->m_totalDay && m_pGrpWnd->m_totalDay == m_pGrpWnd->m_dispDay)
					{
						m_pGrpWnd->m_totalDay = m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = _vData.size();
						m_pGrpWnd->m_dispPos = 0;
					}
					else				// 보여주는 위치만 변경
					{
						m_pGrpWnd->m_totalDay = _vData.size();
						m_pGrpWnd->m_dispEnd++;
						m_pGrpWnd->m_dispPos++;
					}
					m_pGrpWnd->MessageToParent(CM_CTRL, MAKEWPARAM(CTRL_UpdateScr, 0), 0);
				}
			}
			else
			{	// 요청 개수가 0이 아닐경우 요청개수만큼 쌓고 그 이상은 처음 삭제 끝에 추가
				if (m_pGrpWnd->m_ReqCnt > 0 && m_pGrpWnd->m_ReqCnt < gsl::narrow_cast<int>( _vData.size() )  )
				{
					_vData.erase(_vData.begin());
				}

				m_pGrpWnd->m_totalDay = m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = _vData.size();
				m_pGrpWnd->m_dispPos = 0;			
			}
		}
		else
		{
			m_pGrpWnd->m_totalDay = m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = _vData.size();
			m_pGrpWnd->m_dispPos = 0;			
		}
	}

	return Response;
}

int CGrp_Data::RealTimeItemData(int index, CString sVal)
{
	auto& gVal = _vData.at(index);
	gVal->vData[0] = atoi(sVal);

	return RTS_LAST;
}

struct _cgNValx* CGrp_Data::GetGraphData(int index)
{
	if (index < 0 || index > gsl::narrow_cast<int>(_vData.size() - 1) )
		return nullptr;

	return _vData.at(index).get();
}

int CGrp_Data::CharToInt(const char* cnvB, int cnvL)
{
	return atoi(CString(cnvB, cnvL).Trim());
}

double CGrp_Data::CharToDouble(const char* cnvB, int cnvL)
{
	return atof(CString(cnvB, cnvL).Trim());
}

void CGrp_Data::InitInfo(int nHead)
{
	m_pInfo.clear();
	if (nHead <= 0)
		return;

	m_nInfo = nHead;

	for (int ii = 0; ii < m_nInfo; ii++)
	{
		const auto& item = m_pInfo.emplace_back(std::make_unique<_ItemInfo>());
		item->name = "";
		item->RtsCode = "";
		item->RtsSymbol = "";
		item->RtsSign = RM_REMOVE;
		item->digit = 0;
		item->region = 0;
	}
}

void CGrp_Data::GetNValue(struct _cgNValx *gNVal, CString str, int startIndex, int nCount)
{
	auto& vData = gNVal->vData;

	std::vector<CString> vArr;
	AxStd::SplitSub(vArr, str, "\t");
	
//	int ii = 0;
	for (auto item : vArr)
		vData.emplace_back(atof(item));
}