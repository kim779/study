// Signal.cpp: implementation of the CSignal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Signal.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "PacketList.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "Conversion.h"
#include "ChartFormlateBaseData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSignal::CSignal()
{

}

CSignal::~CSignal()
{

}

//sy 2006.04.13.
// signal packet 얻기
bool CSignal::GetSignalDataPacket(const CString& strGraphName, CPacketList* pPacketList, CPacket*& pSignalPacket) const
{
	pSignalPacket = NULL;
	if(pPacketList == NULL || strGraphName.IsEmpty())
		return false;

	pSignalPacket = pPacketList->GetBaseData(strGraphName + SIGNAL_PACKET);
	return (pSignalPacket != NULL);
}
//sy end

//SignalData 얻기.
void CSignal::GetSignalDatas(const CString& strFuncName, CPacketList* pPacketList, CDoublePacketData *&pSignalData)
{
	CString strPacketName = strFuncName + SIGNAL_PACKET;
	CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return ;

	pSignalData = pPacket->GetnumericDataList();
}

/*
void CSignal::CalculateSignal(const CString& strFuncName, const CString& strSubGraphNames, CPacketList* pPacketList, CIndicatorList* pIndicatorList, CPacket* pSignalPacket)
{
	// (2004.06.07, 배승원) Signal Packet은 GetSignalPacket()에서 Clear되면,
	//		지표가 이미 계산된 경우의 IsIndicatorCalculated()에서 바로 Skip시 Signal이 Clear되는 오류가 발생한다.
	//		따라서, 이겨기 아닌 Signal을 실제 계산할 때 Clear시키도록 한다.
	pSignalPacket->ClearData();

	CString subGraphNames = strSubGraphNames;
	if(subGraphNames.Right(1) != ",")
		subGraphNames += ",";

	CPoint point = GetPoint(pPacketList, pSignalPacket);
	
	int nCount = CDataConversion::GetStringDataCount(strSubGraphNames, ",");
	if(nCount <=0 ) return;
	else if(nCount > 1)  {//두개의 Graph으로 계산.	
		CList<double, double> graph1;
		CList<double, double> graph2;
		CPacket* pPacket1 = pPacketList->GetBaseData(CDataConversion::GetStringData(subGraphNames, ","));
		CPacket* pPacket2 = pPacketList->GetBaseData(CDataConversion::GetStringData(subGraphNames, ","));
		if(pPacket1 == NULL || pPacket2 == NULL)
			return ;
		GetSignalTypesbyUsingGraphDatas(point, pPacket1->GetnumericDataList(), pPacket2->GetnumericDataList(), pSignalPacket);
	}
	else { //기준선과 Graph로 계산.
		CPacket* pPacket = pPacketList->GetBaseData(subGraphNames);

		CList<double, double>* pBaseLineList = GetBaseLineList(strFuncName, pIndicatorList);
		if(pBaseLineList == NULL || pPacket == NULL)
			return;

		if(pBaseLineList->GetCount() == 1 && nCount == 1) //base1, graph1
			GetSignalTypesbyUsingGraphDataandOneBaseLine(point, pBaseLineList, pPacket->GetnumericDataList(), pSignalPacket);
		else if(pBaseLineList->GetCount() == 2 && nCount == 1) //base2,graph1
			GetSignalTypesbyUsingGraphDataandTwoBaseLine(point, pBaseLineList, pPacket->GetnumericDataList(), pSignalPacket);
		else
			GetSignalTypesbyUsingGraphDataandMinMaxBaseLine(point, pBaseLineList, pPacket->GetnumericDataList(), pSignalPacket);
	}
	CList<double, double>* pList = pSignalPacket->GetnumericDataList();
}
*/

/*
CList<double, double>* CSignal::GetBaseLineList(const CString& strFuncName, CIndicatorList* pIndicatorList)
{
	if(pIndicatorList == NULL) return NULL;

 	CIndicatorInfo* pIndicatorInfo = pIndicatorList->GetIndicatorInfo( EILT_USER, strFuncName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if(pIndicatorInfo == NULL)	return NULL;

	return pIndicatorInfo->GetBaseLineList();
}
*/

void CSignal::GetPacketDataList(const CString& strGraphName, const CString &strSubGraphName, CPacketList *pPacketList, CDoublePacketData &dRealDataList)
{
	if(pPacketList == NULL) return;

	CPacket* pPacket = NULL;
	pPacket = pPacketList->GetBaseData(strSubGraphName);

	if(pPacket == NULL)	return;

	if(pPacketList->GetnumericDataList(pPacket)->GetCount() <= 0) return;
	MakeRealPacket(strGraphName, pPacketList->GetnumericDataList(pPacket), dRealDataList);
}

CPoint CSignal::GetPoint(CPacketList* pPacketList, CPacket* pSignalPacket)
{
	CPoint point(-1, -1);
	if(pPacketList == NULL || pSignalPacket == NULL)
		return point;

	CPacket* pPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));//"종가"
	if(pPacket == NULL)
		return point;
	if(pSignalPacket->GetCount() == 0)
		point.x = 0;
	else
		point.x = pSignalPacket->GetCount() - 1;
	point.y = pPacketList->GetMaxCount();
	
	return point;
}

void CSignal::SetData(const CPoint &point, const int nUpdateIndex, const double dData, CPacket* pPacket)
{
	if(point.x == 0)
		pPacket->AppendData(dData);
	else
		pPacket->UpdateData(dData, nUpdateIndex, false);
}



void CSignal::GetSignalTypesbyUsingGraphDatas(const CPoint& point, CDoublePacketData *pGraphData2, CDoublePacketData *pGraphData1, CPacket* pSignalPacket)
{
	if(pGraphData1 == NULL || pGraphData2 == NULL) return;
	// 비교할 값 = graphData1, 비교할 기준값 = graphData2
	// --> ex) Macd Signal : 
	//         매수 -> Macd가 Signal 상향돌파, 매도 -> Macd가 Signal 하향돌파
	//         graphData1 --> Macd,   graphData2 --> Signal

	// 처음으로 "0"아닌 값은 무조건 none이다!
	int nFirstNonZeroValue = GetFirstNonZeroValueIndex(pGraphData2);
	const int nCnt = pGraphData1->GetCount();

	POSITION prepos1 = NULL, prepos2 = NULL;
	POSITION pos1 = NULL;
	POSITION pos2 = NULL;

	if(point.x == 0)
	{
		pos1 = pGraphData1->GetHeadPosition();
		pos2 = pGraphData2->GetHeadPosition();
		if(pos1 == NULL || pos2 == NULL)
			return ;

	}
	else
	{
		if(pGraphData1->GetCount() < 2 || pGraphData2->GetCount() < 2)
			return ;
		prepos1 = pGraphData1->FindIndex(point.x - 1);
		prepos2 = pGraphData2->FindIndex(point.x - 1);
		pos1 = pGraphData1->FindIndex(point.x);
		pos2 = pGraphData2->FindIndex(point.x);

		if(prepos1 == NULL || prepos2 == NULL || pos1 == NULL || pos2 == NULL)
			return ;

	}
	for(int i = point.x; i < nCnt; i++){
		if(i >= (nFirstNonZeroValue+1)) {
		// 이전, 현재기준이 될값(graphData2), 이전, 현재비교할 값(graphData1)
			double nCurrentData = GetCurDataFromList( pos1, pGraphData1 );
			double nCurrentBaseData = GetCurDataFromList(pos2, pGraphData2);
			double nOldData = GetCurDataFromList(prepos1, pGraphData1);
			double nOldBaseData = GetCurDataFromList(prepos2, pGraphData2);
			SetData(point, i, GetSignalType3(nOldBaseData, nCurrentBaseData, nOldData, nCurrentData), pSignalPacket);
		}
		else
		{
			prepos1 = pos1; prepos2 = pos2;
			pGraphData1->GetNext(pos1);
			pGraphData2->GetNext(pos2);
			SetData(point, i, 0.0, pSignalPacket);
		}
	}

}

void CSignal::GetSignalTypesbyUsingGraphDataandOneBaseLine(const CPoint& point, CDoublePacketData *pBaseLineList, CDoublePacketData *pGraphData, CPacket* pSignalPacket)
{
	if(pBaseLineList == NULL || pGraphData == NULL) return;
	// 처음으로 "0"아닌 값은 무조건 none이다!
	POSITION pos = pBaseLineList->GetHeadPosition();
	if(pos == NULL)
		return;
	
	double nBaseline = pBaseLineList->GetAt(pos);
	GetSignalDatabyUsingGraphDataandOneBaseLine(point, nBaseline, pGraphData, pSignalPacket);
}

void CSignal::GetSignalDatabyUsingGraphDataandOneBaseLine(const CPoint& point, const double nBaseline, CDoublePacketData *pGraphData, CPacket* pSignalPacket)
{
	if(pGraphData == NULL) return;
	int nFirstNonZeroValue = GetFirstNonZeroValueIndex(pGraphData);
	const int nCnt = pGraphData->GetCount();

	POSITION prepos = NULL, pos = NULL;
	if(point.x == 0)
	{
		pos = pGraphData->GetHeadPosition();
		if(pos == NULL)
			return ;

	}
	else
	{
		if(pGraphData->GetCount() < 2)
			return ;
		prepos = pGraphData->FindIndex(point.x - 1);
		pos = pGraphData->FindIndex(point.x);

		if(prepos == NULL || pos == NULL)
			return ;

	}

	for(int i = point.x; i < nCnt; i++){
		if(i >= (nFirstNonZeroValue+1)) {
			double nCurrentData = GetCurDataFromList(pos, pGraphData);
			double nOldData = GetCurDataFromList(prepos, pGraphData);
			SetData(point, i, GetSignalType(nBaseline, nOldData, nCurrentData), pSignalPacket);
		}
		else
		{
			prepos = pos; 
			pGraphData->GetNext(pos);
			SetData(point, i, 0.0, pSignalPacket);
		}
	}
}

void CSignal::GetSignalTypesbyUsingGraphDataandTwoBaseLine(const CPoint& point, CDoublePacketData *pBaseLineList, CDoublePacketData *pGraphData, CPacket* pSignalPacket)
{
	if(pBaseLineList == NULL || pGraphData == NULL) return;
	
	POSITION pos = pBaseLineList->GetHeadPosition();
	if(pos == NULL)
		return;
	double nUpBaseline, nDownBaseline;
	double first = pBaseLineList->GetNext(pos);
	double second = pBaseLineList->GetNext(pos);

	if(first > second) {
		nUpBaseline = first;
		nDownBaseline = second;
	}
	else {
		nUpBaseline = second;
		nDownBaseline = first;
	}

	GetSignalDatabyUsingGraphDataandTwoBaseLine(point, nUpBaseline, nDownBaseline, pGraphData, pSignalPacket);
}

void CSignal::GetSignalTypesbyUsingGraphDataandMinMaxBaseLine(const CPoint& point, CDoublePacketData *pBaseLineList, CDoublePacketData *pGraphData, CPacket* pSignalPacket){
	if(pBaseLineList == NULL || pGraphData == NULL) return;
	double nUpBaseline, nDownBaseline;

	int order = 0;
	POSITION pos = pBaseLineList->GetHeadPosition();
	while(pos) {
		double temp = pBaseLineList->GetNext(pos);

		if(order == 0)
			nUpBaseline = nDownBaseline = temp;
		else {
			if(nDownBaseline > temp)
				nDownBaseline = temp;
			else if(nUpBaseline < temp)
				nUpBaseline = temp;
		}
		order++;
	}

	GetSignalDatabyUsingGraphDataandTwoBaseLine(point, nUpBaseline, nDownBaseline, pGraphData, pSignalPacket);
}

void CSignal::GetSignalDatabyUsingGraphDataandTwoBaseLine(const CPoint& point, const double nUpBaseline, const double nDownBaseline, CDoublePacketData *pGraphData, CPacket* pSignalPacket)
{
	if(pGraphData == NULL) return;
	// 처음으로 "0"아닌 값은 무조건 none이다!
	int nFirstNonZeroValue = GetFirstNonZeroValueIndex(pGraphData);
	const int nCnt = pGraphData->GetCount();
	
	POSITION prepos = NULL, pos = NULL;
	if(point.x == 0)
	{
		pos = pGraphData->GetHeadPosition();
		if(pos == NULL)
			return ;

	}
	else
	{
		if(pGraphData->GetCount() < 2)
			return ;
		prepos = pGraphData->FindIndex(point.x);
		pos = pGraphData->FindIndex(point.x);

		if(prepos == NULL || pos == NULL)
			return ;

	}

	for(int i = point.x; i < nCnt; i++){
		if(i >= (nFirstNonZeroValue+1)) {
			double nOldData = GetCurDataFromList(prepos, pGraphData);
			double nCurrentData = GetCurDataFromList(pos, pGraphData);
			SetData(point, i, GetSignalType(nUpBaseline, nDownBaseline, nOldData, nCurrentData), pSignalPacket);
		}
		else
		{
			prepos = pos;
			pGraphData->GetNext(pos);
			SetData(point, i, 0.0, pSignalPacket);
		}
	}
}


int CSignal::GetFirstNonZeroValueIndex( CDoublePacketData *pGraphData1)
{
	if( !pGraphData1) return 0;

	const int nCnt = pGraphData1->GetCount();
	POSITION pos = pGraphData1->GetHeadPosition();
	if(pos == NULL)
		return 0;
	for(int start = 0; start < nCnt; start++)
//		if(pGraphData1->GetAt(pGraphData1->FindIndex(start)) != 0)
		if(pGraphData1->GetNext(pos) != 0)
			return start;
	return pGraphData1->GetCount()-1;

}

CGraphBaseData::SignalTYPE CSignal::GetSignalType3(const double oldBaseData, const double nowBaseData, const double oldData, const double nowData)
{
	// 비교할 값 = xxxData (graphData1), 비교할 기준값 = xxxBaseData (graphData2)
	if(oldData <= oldBaseData && nowData > nowBaseData)
		return CGraphBaseData::BUY; // 매수
	if(oldData >= oldBaseData && nowData < nowBaseData)
		return CGraphBaseData::SELL;// 매도

	return CGraphBaseData::NONE;

}

CGraphBaseData::SignalTYPE CSignal::GetSignalType(const double baseline, const double oldData, const double nowData)
{
	if(oldData <= baseline && nowData > baseline)
		return CGraphBaseData::BUY; // 매수
	if(oldData >= baseline && nowData < baseline)
		return CGraphBaseData::SELL; // 매도

	return CGraphBaseData::NONE;
}

CGraphBaseData::SignalTYPE CSignal::GetSignalType(const double upBaseline, const double downBaseline, const double oldData, const double nowData)
{
	// 하향 기준선 = downBaseline, 상향 기준선 = upBaseline
	if(oldData <= downBaseline && nowData > downBaseline) 
		return CGraphBaseData::BUY; // 매수

	if(oldData >= upBaseline && nowData < upBaseline) 
		return CGraphBaseData::SELL; // 매도

	return CGraphBaseData::NONE;
}

double CSignal::GetCurDataFromList( POSITION& pos, CDoublePacketData *pGraphData)
{
	if(pGraphData == NULL || pos == NULL)
		return 0;

	return pGraphData->GetNext(pos);
}

void CSignal::MakeRealPacket(const CString& strGraphName, CDoublePacketData *pDataList, CDoublePacketData &dRealDataList)
{
	if(pDataList == NULL)
		return;

	for(POSITION pos = pDataList->GetHeadPosition(); pos ; ) {
		double data = pDataList->GetNext(pos);
		dRealDataList.AddTail(data);
	}

/*
	//*0.01해 줘야 함.
	if(DoReCalculation(strGraphName)) {
		for(POSITION pos = pDataList->GetHeadPosition(); pos ; ) {
			double data = pDataList->GetNext(pos);
			dRealDataList.AddTail((int)(data * 0.01));
		}
	}
	else {
		for(POSITION pos = pDataList->GetHeadPosition(); pos ; ) {
			double data = pDataList->GetNext(pos);
			dRealDataList.AddTail((int)data);
		}
	}
*/
}

/*
bool CSignal::DoReCalculation(const CString &strGraphName)
{
	for(int i = 0; i < GRAPH_COUNT; i++) {
		if(strGraphName.CompareNoCase( g_szGraphNameList[ i]) == 0) {
			switch(i){
				case GRAPH_PSYCH:
				case GRAPH_NEWPSYCH:
				case GRAPH_MI:
				case GRAPH_MOMENTUM:
				case GRAPH_RSI:
				case GRAPH_STOCHASTICS_FAST:
				case GRAPH_STOCHASTICS_SLOW:
				case GRAPH_WILLIAM:
				case GRAPH_RELATIVE_STRENGTH_COMPARATIVE:
				case GRAPH_VHF:
					return true;
			}
		}
	}
	return false;
}
*/