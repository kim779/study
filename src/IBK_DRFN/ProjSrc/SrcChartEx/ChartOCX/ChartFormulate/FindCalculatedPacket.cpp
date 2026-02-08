// FindCalculatedPacket.cpp: implementation of the CFindCalculatedPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "chartformulate.h"
#include "FindCalculatedPacket.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "PacketList.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//PacketDll에 요청한 Graph에 대한 계산된Data가 저장되어 있는지 확인.
//strSubGraphName에 data가 있으면 IsSubGraphIntoPacketListWhenThereisSubGraphName()호출.
//strSubGraphName에 data가 없으면 IsSubGraphIntoPacketListWhenThereisnotSubGraphName()호출.
bool CFindCalculatedPacket::IsSubGraphIntoPacketList(const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
	IsSubGraphIntoPacketListWhenThereisSubGraphName(chartFormulateData, CalculateData);
	if(chartFormulateData.GetCalculateType() != CCalculateBaseData::TRDATA_CAL || chartFormulateData.GetSubGraphNames().GetLength() > 0)
		return false;
	else
		return true;
}

//strSubGraphName이 들어왔을때 Packet이 있는지 확인.
void CFindCalculatedPacket::IsSubGraphIntoPacketListWhenThereisSubGraphName(const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
	int nIndex = CDataConversion::GetStringDataCount(chartFormulateData.GetSubGraphNames(), ",");
	if(nIndex == 0)
		return ;
	CString orgStr = CDataConversion::GetStringData(chartFormulateData.GetSubGraphNames(), ",");
	orgStr.TrimLeft();
	orgStr.TrimRight();
	IsSubGraphIntoPacketListbyNEWandINSERTandAPPEND(orgStr, chartFormulateData, CalculateData);
}



//New, Insert, Append에 따라서 다시계산해야하는지 아닌지에 대한 경우가 다름.
//packet의 타입이 Append의 경우 packet(New성분일때 계산했던 Data)이 있더라도 계산을 다시 해야한다.
void CFindCalculatedPacket::IsSubGraphIntoPacketListbyNEWandINSERTandAPPEND(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
//25
	if(strSubGraphName.CompareNoCase( _MTEXT( C3_BIG_NEWS)) == 0 || strSubGraphName.CompareNoCase( _MTEXT( C3_LOCK)) == 0) 
		WhenPacketIsStringData(strSubGraphName, chartFormulateData, CalculateData);
	else
		WhenPacketIsIntData(strSubGraphName, chartFormulateData, CalculateData);
}


//nStandardCount = maxcount
//nMycount = 내 리스트 카운트
//후행스팬과 선행스팬의 경우 그냥 count로 비교하면 계속 다시 계산하게 된다.
//Append일 경우, 예) datacount가 26일때..
//max = 26 , index = 25, nStandrdcount = 26
//nMycount = 1, 조건 = 26
//이런 경우는 true리턴.
bool CFindCalculatedPacket::IsFinishCalulationWhenNew(const CString &strSubGraphName, 
													  const int nStandardCount, const int nMyCount, 
													  CList<double, double>& ConditionList)
{
	if(strSubGraphName.CompareNoCase( _MTEXT( C3_POST_SPAN)) == 0) {
		if(nStandardCount == (nMyCount + ConditionList.GetAt(ConditionList.FindIndex(1)) - 1))
			return true;
	}
	//마지막 데이타를 포함해서 26개니까..실질적으로는 25개의 데이타가 더해진것.
	else if(strSubGraphName.CompareNoCase( _MTEXT( C3_PRE_SPAN_1)) == 0 || strSubGraphName.CompareNoCase( _MTEXT( C3_PRE_SPAN_2)) == 0 ) {
		if(nStandardCount == (nMyCount - ConditionList.GetAt(ConditionList.FindIndex(1)) - 1))
			return true;
	}
	
//	else if(strSubGraphName == _MTEXT( C2_OVERHANGING_SUPPLY))
// (2006.12.06) Sunghwan Ahn
	else if( g_iMetaTable.IsVolumeForSale(strSubGraphName))
// (2006.12.06) Sunghwan Ahn
	{
		if((nMyCount-2) == ConditionList.GetAt(ConditionList.FindIndex(0)))
			return true;
	}
	else {
		if(nStandardCount == nMyCount)
			return true;
	}
	return false;
}

//이건 사용하지 않고 있음.
//후행스팬과 선행스팬의 경우 그냥 count로 비교하면 계속 다시 계산하게 된다.
bool CFindCalculatedPacket::IsFinishCalulationWhenInsertAppend(const CString &strSubGraphName, const int nStandardCount, const int nMyCount, CList<double, double>& ConditionList)
{
	if(strSubGraphName.CompareNoCase( _MTEXT( C3_POST_SPAN)) == 0) {
		if(nStandardCount == (nMyCount + ConditionList.GetAt(ConditionList.FindIndex(1))))
			return true;
	}
	else if(strSubGraphName.CompareNoCase( _MTEXT( C3_PRE_SPAN_1)) == 0 || strSubGraphName.CompareNoCase( _MTEXT( C3_PRE_SPAN_2)) == 0 ) {
		if(nStandardCount == (nMyCount - ConditionList.GetAt(ConditionList.FindIndex(1))))
			return true;
	}
	else {
		if(nStandardCount == nMyCount)
			return true;
	}
	return false;
}

void CFindCalculatedPacket::WhenPacketIsStringData(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	CCalculateBaseData::CALCULATETYPE timetype = chartFormulateData.GetCalculateType();

	CList<CString, CString>* pList = pPacketList->GetstrDataList(pPacketList->GetBaseData(strSubGraphName));

	if(timetype == CCalculateBaseData::TRDATA_CAL) {
//		if(pPacketList->GetBaseData(strSubGraphName) == NULL || pList == NULL || pList->GetCount() <= 0) 
//			CalculateData.SetPacketNameForCal(chartFormulateData.GetSubGraphNames());
//		else if(!IsFinishCalulationWhenNew(strSubGraphName, pPacketList->GetMaxCount(), pList->GetCount(), ConditionList))
//			CalculateData.SetPacketNamesForCal(chartFormulateData.GetSubGraphNames());	

	}
	//real은 무조건 다시 계산한다.
//	else if(timetype == CCalculateBaseData::REALDATA_CAL) 
//		CalculateData.SetPacketNameForCal(chartFormulateData.GetSubGraphNames());

}

void CFindCalculatedPacket::WhenPacketIsIntData(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	CCalculateBaseData::CALCULATETYPE timetype = chartFormulateData.GetCalculateType();

	CPacket* pPacket = pPacketList->GetBaseData(strSubGraphName);
	if(pPacket == NULL)
		return;

	CList<double, double>* pList = pPacket->GetnumericDataList();
	if(timetype == CCalculateBaseData::TRDATA_CAL) {
//		if(pPacketList->GetBaseData(strSubGraphName) == NULL) 
//			CalculateData.SetPacketNameForCal(chartFormulateData.GetSubGraphNames());
//		else if(!IsFinishCalulationWhenNew(strSubGraphName, pPacketList->GetMaxCount(), pList->GetCount(), ConditionList))
//			CalculateData.SetPacketNamesForCal(ChartFormulateData.GetSubGraphNames());	
	}
	//real은 무조건 다시 계산한다.
//	else if(timetype == CCalculateBaseData::REALDATA_CAL) 
//		CalculateData.SetPacketNameForCal(chartFormulateData.GetSubGraphNames());

}
