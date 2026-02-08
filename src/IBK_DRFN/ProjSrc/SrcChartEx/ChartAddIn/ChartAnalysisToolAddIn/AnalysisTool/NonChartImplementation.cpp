// NonChartImplementation.cpp: implementation of the CNonChartImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NonChartImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNonChartImplementation::CNonChartImplementation()
{

}

CNonChartImplementation::~CNonChartImplementation()
{

}

bool CNonChartImplementation::GetAutoTrendData(const CCalculateBaseData::CALCULATETYPE eCalType, 
		const CString& strPacketName, IPacketManager* pPacketList, 
		CList<IPacket*, IPacket*>* pCalculateForPacketList)
{
/*
	IPacket* pSignalPacket = NULL;
	CGraphCalculateData graphCalculateData(pSignalPacket);
	CSubGraphPacketList subGraphPacketList;
	graphCalculateData.AddTail(&subGraphPacketList);

	CChartFormulateData chartFormulateData;
	GetChartFormulateData(eCalType, strPacketName, pPacketList, chartFormulateData);

	if(chartFormulateData.GetCalculateType() != CCalculateBaseData::TRDATA_CAL)
		return false;

	m_Indicator = m_indiSingleton.FindNonChartFormulate();
	return Calculate(chartFormulateData, &graphCalculateData, NULL, pCalculateForPacketList);
*/
	return false;
}

/*
bool CNonChartImplementation::Calculate(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo,
		CList<IPacket*, IPacket*>* pCalculatePacketList)
{
	if(pCalculatePacketList->GetCount() > 0)
		pCalculatePacketList->RemoveAll();

	if(!Calculate(chartFormulateData, pGraphCalculateData, pIndicatorInfo))
		return false;

	return AddCalculatePacketList(pGraphCalculateData, pCalculatePacketList);
}

bool CNonChartImplementation::Calculate(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	m_CalculateData.SetPoint(GetDataPoint(chartFormulateData, pGraphCalculateData));

	CString strSubGraphNames = chartFormulateData.GetSubGraphNames();
	if(strSubGraphNames.IsEmpty())
		return false;

	if(strSubGraphNames.Right(1) != ',')
		strSubGraphNames += ',';

	while(!strSubGraphNames.IsEmpty()) 
	{
		CString strSubGraphName = CDataConversion::GetStringData(strSubGraphNames, ",");
		if(strSubGraphName.IsEmpty())
			continue;

		m_CalculateData.SetPacketNamesForSave(strSubGraphName);
		m_Indicator->GetData(chartFormulateData, pGraphCalculateData, pIndicatorInfo, m_CalculateData);
	}

	return true;
}

bool CNonChartImplementation::AddCalculatePacketList(CGraphCalculateData* pGraphCalculateData,
		CList<IPacket*, IPacket*>* pCalculatePacketList)
{
	if(pGraphCalculateData == NULL || pCalculatePacketList == NULL)
		return false;

	if(pGraphCalculateData->GetHeadPosition() == NULL)
		return false;

	CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetHead();
	if(pSubGraphPacketList == NULL)
		return false;

	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	while(pos != NULL)
	{
		pCalculatePacketList->AddTail(pSubGraphPacketList->GetNext(pos));
	}

	return (pCalculatePacketList->GetCount() > 0);
}

CPoint CNonChartImplementation::GetDataPoint(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData)
{
	IPacketManager* pPacketList = chartFormulateData.GetPacketList();
	if(pPacketList == NULL)
		return (-1, -1);

	CPoint point = pPacketList->GetUpdateRange();
	CCalculateBaseData::CALCULATETYPE timetype = chartFormulateData.GetCalculateType();
	if(timetype == CCalculateBaseData::REALDATA_CAL) 
		GetDataPointWhenREALAPPEND(pPacketList, pGraphCalculateData, point);
	else if(timetype == CCalculateBaseData::TRDATA_CAL && point.x != 0) {
		point.x = 0;
		point.y = pPacketList->GetMaxCount();
	}
	return point;
}

void CNonChartImplementation::GetDataPointWhenREALAPPEND(IPacketManager* pPacketList,
		CGraphCalculateData* pGraphCalculateData, CPoint& point)
{
	if(pGraphCalculateData->GetCount() < 0)
		return ;
	CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
	if(!pSubGraphList)
		return;

	IPacket* pPacket = pSubGraphList->GetHeadPacket();
	if(pPacket == NULL || pPacket->GetCount() <= 0)
	{
		point.x = 0;
		point.y = pPacketList->GetMaxCount();
	}
	//그림이 그려지고 있지 않았을때 실시간으로 받았던 Data는 계산이 되지 않는다.
	//다시 화면을 띄웠을때 계산되지 않았던 영역은 계산하라고 넘어오지 않는다.
	//여기서 계산되지 않았던 영역을 계산하기 위해 계산해야 할 영역을 다시 잡는다.
	else if(pPacket != NULL) {
		int nCalculatedDataCount = pPacket->GetCount();
		if(nCalculatedDataCount != point.x)
			point.x = (nCalculatedDataCount - 1);
	}
}

//sy test
void CNonChartImplementation::GetChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType, 
		const CString& strFunctionName, IPacketManager* pPacketList, 
		CChartFormulateData& chartFormulateData) const
{
	chartFormulateData.SetCalculateType(eCalType);
	chartFormulateData.SetFunctionName(strFunctionName);
	chartFormulateData.SetPacketList(pPacketList);

	chartFormulateData.SetPacketNames("종가,고가,저가,");
	chartFormulateData.SetSubGraphNames(strFunctionName + ",");
	chartFormulateData.SetViewableSubGraphNames(strFunctionName + ",");
}
*/
