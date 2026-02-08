// MainChartFormulateImplementation.cpp: implementation of the CMainChartFormulateImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainChartFormulateImplementation.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "ChartFormulateData.h"
#include "PacketList.h"
#include "Conversion.h"
#include "Indicator.h"
#include "IndicatorList.h"
#include "IndicatorFactory.h"
#include "GetIndicatorInfo.h"
#include "FindCalculatedPacket.h"
#include "CalculateData.h"
#include "PacketPointerList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainChartFormulateImplementation::CMainChartFormulateImplementation()
{
}

CMainChartFormulateImplementation::~CMainChartFormulateImplementation()
{
}

//main에서 호출.
bool CMainChartFormulateImplementation::CalculateIndicator(const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo*& pIndicatorInfo)
{
	CString strGraphName = chartFormulateData.GetGraphName();
	CString strIndicatorName = chartFormulateData.GetIndicatorName();

	if( graphCalculateData == NULL || !IsThisDaegiMemul( strIndicatorName, chartFormulateData.GetCalculateType()))
		return false;
	
	bool bGetInfo = false;
	if(pIndicatorInfo == NULL)
	{
		bGetInfo = true;
		pIndicatorInfo = chartFormulateData.GetIndicatorList()->GetGraphIndicatorInfo( EILT_USER, strGraphName, strIndicatorName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	}

	// (2006/9/6 - Seung-Won, Bae) Indicator for Graph
	CIndicator *&pIndicator = graphCalculateData->GetIndicator();
	if( pIndicator) if( pIndicator->GetID() != g_iMetaTable.GetIndicatorID( strIndicatorName))
	{
		delete pIndicator;
		pIndicator = NULL;
	}
	if( !pIndicator)
	{
		IMetaTable::EDDC_INDICATOR_ID eID = ( IMetaTable::EDDC_INDICATOR_ID)g_iMetaTable.GetIndicatorID( strIndicatorName);
		if( g_iMetaTable.IsSTIndexIndicator( eID)) 
			pIndicator = m_indiSingleton.CreateSTIndexCalculator( eID);
		else 
			pIndicator = m_indiSingleton.Create( eID, chartFormulateData.GetOcxHwnd());
	}
	if( !pIndicator) return bGetInfo;	//계산이 필요없는 지표의 경우.

	return Calculate(pIndicator, chartFormulateData, graphCalculateData, pIndicatorInfo);
}

bool CMainChartFormulateImplementation::Calculate(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	if(pIndicator == NULL)
		return false;

	m_CalculateData.SetPoint(GetDataPoint(chartFormulateData, graphCalculateData));
	if(m_CalculateData.GetPoint().x < 0)
		return false;
	
	if(IsCalculateSubGraph( chartFormulateData.GetIndicatorName()))
		return Calculate_SubGraph(pIndicator, chartFormulateData, graphCalculateData, pIndicatorInfo);

	return Calculate_Graph(pIndicator, chartFormulateData, graphCalculateData, pIndicatorInfo);
}

// graph 별 계산 (모든 subgraph를 계산한다.)
bool CMainChartFormulateImplementation::Calculate_Graph(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	assert(pIndicator != NULL);

	pIndicator->GetData(chartFormulateData, graphCalculateData, pIndicatorInfo, m_CalculateData);
	if(graphCalculateData->GetCount() < 0)
		return false;
	return (graphCalculateData->GetHead()->GetHeadPacket() != NULL);
}

// subgraph 별 계산 (subgraph를 하나씩 계산한다.)
bool CMainChartFormulateImplementation::Calculate_SubGraph(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	assert(pIndicator != NULL);

	int nCount = CDataConversion::GetStringDataCount(chartFormulateData.GetSubGraphNames(), ",");
	if(nCount <= 0)
		return false;

	CString strSubGraphNames = chartFormulateData.GetSubGraphNames();
	for(int nIndex = 0 ; nIndex < nCount; nIndex++){
		CString strSubGraphName = CDataConversion::GetStringData(strSubGraphNames, ",");
		if(strSubGraphName.IsEmpty())
			return false;
		m_CalculateData.SetPacketNamesForSave(strSubGraphName);
		pIndicator->GetData(chartFormulateData, graphCalculateData, pIndicatorInfo, m_CalculateData);
	}
	return true;
}

CPoint CMainChartFormulateImplementation::GetDataPoint(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(pPacketList == NULL)
		return (-1, -1);

	CPoint point = pPacketList->GetUpdateRange();
	CCalculateBaseData::CALCULATETYPE timetype = chartFormulateData.GetCalculateType();
	if(timetype == CCalculateBaseData::REALDATA_CAL) 
		GetDataPointWhenREALAPPEND(pPacketList, pGraphCalculateData, point);
	else if(timetype == CCalculateBaseData::TRDATA_CAL && point.x != 0
		|| timetype == CCalculateBaseData::SCROLL_CAL)
	{
		point.x = 0;
		point.y = pPacketList->GetMaxCount();
	}

	//InsertType은 new인데 숨김으로 체크되어있던 Graph가 숨겨져 있을동안에 받은 Data를 계산하고 저장하기 위해서는 
	//중간에서 계산하게 하고 싶어도 계산되지 않은 영역이 Insert인지 Append 인지 알수가 없기 때문에 
	//처음부터 계산할수 밖에 없군.
	//첨부터 다시 계산해야 한다. 계산후 저장할때 packet이 가지고 있는 Insert 타입은 New이기 때문이징..
	//매매리포트, 시스템 트레이더를 위한 영역.
/*	if(point.x == -1 || point.y == -1 || bSaveFlag == false) {
//		CDataList* pDataList = ptrDataList.GetAt(ptrDataList.FindIndex(0));
		CDataList* pDataList = ptrDataList.GetHead();
		point.x = 0;
		point.y = pDataList->GetCount();
	}*/
	return point;
}

void CMainChartFormulateImplementation::GetDataPointWhenREALAPPEND(CPacketList* pPacketList,
																   CGraphCalculateData* pGraphCalculateData,
																   CPoint& point)
{
	if(pGraphCalculateData->GetCount() < 0)
		return ;
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
	CSubGraphPacketList* pSubGraphList = NULL;

	// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
	CIndicator *pIndicator = pGraphCalculateData->GetIndicator();
	if( pIndicator) switch( pIndicator->GetID())
	{
		case IMetaTable::EDDC_OCX_PRICEMACHART:
		case IMetaTable::EDDC_OCX_VOLUMEMACHART:
			pSubGraphList = pGraphCalculateData->GetAt( ( ( CMA *)pIndicator)->GetFirstVisibleSubGraph());
			break;
	}

	if( !pSubGraphList) pSubGraphList = pGraphCalculateData->GetHead();

	CPacket* pPacket = NULL;
	if( pSubGraphList) pPacket = pSubGraphList->GetHeadPacket();
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

/*
void CMainChartFormulateImplementation::Initialize_Packet(const CString& strGraphName)
{
//	CIndicator* pIndicator = m_indiSingleton.Find(strGraphName);
//	if(pIndicator == NULL)
//		return;

//	pIndicator->Initialize_Packet();
}

// 공식안에 있는 packet 만을 삭제한다.


void CMainChartFormulateImplementation::DeleteDataInPacket(CPacketList* pPacketList, const CString& strGraphName)
{
//	if(pPacketList == NULL || strGraphName.IsEmpty())
//		return;

//	CIndicator* pIndicator = m_indiSingleton.Find(strGraphName);
//	if(pIndicator != NULL)
//		pIndicator->DeleteDataInPacket(pPacketList);
}
*/

/*
void CMainChartFormulateImplementation::DeletePacket(const CString& strGraphName, const CString& strSubGraphName,
			CPacketList* pPacketList, CIndicatorList* pIndicatorList)
{
	if(pPacketList == NULL || pIndicatorList == NULL) 
		return ;

	CList<double, double> ConditionList;
	CList<CString, CString> SubGraphNameList;
	CList<CString, CString> PacketNameList;
	CString strPacketName = "";
	CString strNewSubGraphName = strSubGraphName;

	CGetIndicatorInfo::GetIndicatorInfoFromIndicatorConditionDLL(strPacketName, strGraphName, pIndicatorList, ConditionList, PacketNameList, SubGraphNameList); 
	//예외적인 지표의경우 PacketName을 만든다.
//	CMakePacketName::MakePacketNameWhenExceptionalIndicator(strGraphName, strPacketName, ConditionList, strNewSubGraphName, SubGraphNameList); 

	if(strNewSubGraphName.Right(1) != ",")
		strNewSubGraphName += ",";

	int nCount = CDataConversion::GetStringDataCount(strNewSubGraphName, ",");
	for(int i = 0 ; i < nCount ; i++) {
		pPacketList->ClearIndicatorData(CDataConversion::GetStringData(strNewSubGraphName, ","));
	}
}
*/

// scroll 이동시 계산을 할지의 여부
// "대기매물"시만 계산한다.
bool CMainChartFormulateImplementation::IsThisDaegiMemul(const CString &strGraphName, const CCalculateBaseData::CALCULATETYPE calculatetype)
{
	if(calculatetype != CCalculateBaseData::SCROLL_CAL)
		return true;

	// (2006.12.06) Sunghwan Ahn
	//	return (strGraphName.Find( _MTEXT( C2_OVERHANGING_SUPPLY)) >= 0);
	return g_iMetaTable.IsVolumeForSale( strGraphName) ? true : false;
}

// subGraph 별로 계산할지의 여부
// "가격 이동평균", "거래량 이동평균", "이격도"
bool CMainChartFormulateImplementation::IsCalculateSubGraph(const CString& strIndicatorName) const
{
	if( strIndicatorName.IsEmpty()) return false;

	return (strIndicatorName == _MTEXT( C2_PRICE_MA) || strIndicatorName == _MTEXT( C2_VOLUME_MA));
}
