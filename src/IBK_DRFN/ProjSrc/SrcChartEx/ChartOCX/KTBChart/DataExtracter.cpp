// DataExtracter.cpp: implementation of the CDataExtracter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataExtracter.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
#include "BlockBaseData.h"
#include "PacketList.h"
#include "PacketBase.h"
#include "MainBlock.h"
#include "Conversion.h"
#include "ViewData.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define COUNT		16

static IMetaTable::E_METACODE pPacketName[] =
{
	IMetaTable::C0_OPEN,
	IMetaTable::C0_HIGH,
	IMetaTable::C0_LOW,
	IMetaTable::C0_CLOSE,
	IMetaTable::C0_CLOSE,
	IMetaTable::C3_PRICE_MA_1,
	IMetaTable::C3_PRICE_MA_2,
	IMetaTable::C3_PRICE_MA_3,
	IMetaTable::C3_PRICE_MA_4,
	IMetaTable::C3_PRICE_MA_5,
	IMetaTable::C3_PRICE_MA_6,
	IMetaTable::C0_VOLUME,
	IMetaTable::C3_VOLUME_MA_1,
	IMetaTable::C3_VOLUME_MA_2,
	IMetaTable::C3_VOLUME_MA_3,
	IMetaTable::C3_VOLUME_MA_4,
	IMetaTable::C0_DATE_TIME
};

/*
CString CDataExtracter::GetData(const int nStartIndex,
							 const int nEndIndex,
							 const int nDataStart,
							 const int nDataEnd,
							 CPacketList *pPacketList,
							 CMainBlock* pMainBlock,
							 int& nMaxLength)
{
	//자료일자나 가격이나 거래량이 없을때는 어떻게 해야되지??
	//nStartIndex에는 화면에서의 index만 있다.
	int nRealStart, nRealEnd;
	GetRealDataIndex(nStartIndex, nEndIndex, nDataStart, nDataEnd, nRealStart, nRealEnd);


	if(nRealStart <= 0 || nRealEnd <= 0)
		return "";

	if(nRealEnd >= pPacketList->GetMaxCount())
		nRealEnd = pPacketList->GetMaxCount() - 1;

	CString strData = "";
	strData += MakeDateString( _MTEXT( C6_START_DATE), GetDate(nRealStart, pPacketList));
	strData += MakeDateString( _MTEXT( C6_END_DATE), GetDate(nRealEnd, pPacketList));

	strData += MakeString( _MTEXT( C6_MAXIMUM_PRICE), GetHighestPrice(nRealStart, nRealEnd, pPacketList), nMaxLength);
	strData += MakeString( _MTEXT( C6_MINIMUM_PRICE), GetLowestPrice(nRealStart, nRealEnd, pPacketList), nMaxLength);

	double nHighestVolume, nLowestVolume;
	GetHighestLowestVolume(nRealStart, nRealEnd, pPacketList, nHighestVolume, nLowestVolume);

	strData += MakeString( _MTEXT( C6_MAXIMUM_VOLUME), nHighestVolume, nMaxLength);
	strData += MakeString( _MTEXT( C6_MINIMUM_VOLUME), nLowestVolume, nMaxLength);

	//시,고,저,종,거래량 이동평균 구하기
	for(int i = 0 ; i< 4 ; i++)
		strData += MakeString( _MTEXTN( pPacketName[i]),
							  GetMA(nRealStart, nRealEnd, GetPacket( _MTEXTN( pPacketName[i]), pPacketList)),
							  nMaxLength);

	//가격이동평균, 거래량 이동평균의 이동평균 구하기 
	for(i = 5 ; i< COUNT ; i++)
		strData += MakeString(GetTitle( _MTEXTN( pPacketName[i]), pMainBlock),
							 GetMA(nRealStart, nRealEnd, GetPacket( _MTEXTN( pPacketName[i]), pPacketList)),
							 nMaxLength);

	return strData;
}
*/

POSITION CDataExtracter::FindPosition(const int nIndex, const CList<double, double>& pSourceList) const
{
	int nTailIndex = pSourceList.GetCount() -1;
	if(nIndex < 0 || nIndex > nTailIndex)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = pSourceList.GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			pSourceList.GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = pSourceList.GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			pSourceList.GetNext(pos);
		}
	}

	return NULL;
}

double CDataExtracter::GetHighestPrice(const int nStartIndex,
									const int nEndIndex,
									CPacketList *pPacketList)
{
	CList<double, double>* pDataList = GetPacket( _MTEXTN( pPacketName[1]), pPacketList);
	if(pDataList == NULL || pDataList->GetCount() <= 0) 
		return -1;

	//sy 2002.10.4
	POSITION pos = FindPosition(nStartIndex, *pDataList);
	if(pos == NULL)
		return -1;

	double nHighest = pDataList->GetAt(pos);
	for(int i = nStartIndex ;  i <= nEndIndex ; i++) {
		pos = FindPosition(i, *pDataList);
		if(pos == NULL)
			break;
		double data = pDataList->GetAt(pos);
		if(data > nHighest)
			nHighest = data;
	}
	return nHighest;
}


double CDataExtracter::GetLowestPrice(const int nStartIndex,
									const int nEndIndex,
									CPacketList *pPacketList)
{
	CList<double, double>* pDataList = GetPacket( _MTEXTN( pPacketName[2]), pPacketList);
	if(pDataList == NULL || pDataList->GetCount() <= 0) 
		return -1;

	//sy 2002.10.4
	POSITION pos = FindPosition(nStartIndex, *pDataList);
	if(pos == NULL)
		return -1;

	double nLowest = pDataList->GetAt(pos);
	for(int i = nStartIndex ;  i <= nEndIndex ; i++) {
		pos = FindPosition(i, *pDataList);
		if(pos == NULL)
			break;
		double data = pDataList->GetAt(pos);
		if(data < nLowest)
			nLowest = data;
	}
	return nLowest;
}


void CDataExtracter::GetHighestLowestVolume(const int nStartIndex,
									const int nEndIndex,
									CPacketList *pPacketList,
									double& nHighestVolume,
									double& nLowestVolume)
{
	CList<double, double>* pDataList = GetPacket( _MTEXTN( pPacketName[10]), pPacketList);
	if(pDataList == NULL || pDataList->GetCount() <= 0) {
		nHighestVolume = -1;
		nLowestVolume = -1;
		return;
	}

	//sy 2002.10.4
	POSITION pos = FindPosition(nStartIndex, *pDataList);
	if(pos == NULL)
		return;

	double nLowest = pDataList->GetAt(pos);
	double nHighest = pDataList->GetAt(pos);

	for(int i = nStartIndex ;  i <= nEndIndex ; i++) {
		POSITION pos = FindPosition(nStartIndex, *pDataList);
		if(pos == NULL)
			break;

		double data = pDataList->GetAt(pos);
		if(data < nLowest)
			nLowest = data;
		if(data > nHighest)
			nHighest = data;
	}

	nHighestVolume = nHighest;
	nLowestVolume = nLowest;
}


double CDataExtracter::GetMA(const int nStartIndex,
							 const int nEndIndex,
							 CList<double, double>* pDataList)
{
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return -1;

	int ndataCount = nEndIndex - nStartIndex + 1;

	double ntotalvalue = -1;
	for(int i = nStartIndex ;  i <= nEndIndex ; i++){
		POSITION pos = FindPosition(i, *pDataList);
		if(pos == NULL)
			break;
		ntotalvalue += pDataList->GetAt(pos);
	}

	return ntotalvalue / ndataCount;
}

CList<double, double>* CDataExtracter::GetPacket(const CString &strSubGraphName, CPacketList *pPacketList)
{
	CPacket* pData = pPacketList->GetBaseData(strSubGraphName);

	if(pData == NULL)
		return NULL;

	return pData->GetnumericDataList();
}

/*
CString CDataExtracter::GetDate(const int nIndex,
							 CPacketList *pPacketList)
{
	CPacket* pPacket = pPacketList->GetBaseData( _MTEXTN( pPacketName[15]));
	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0) 
		return -1;

	//sy 2002.10.4
	POSITION pos = FindPosition(nIndex, *pDataList);
	if(pos == NULL)
		return "";

	int dData = (int)pDataList->GetAt(pos);
	return CDataConversion( m_hOcxWnd).GetStringFormatTrans(CDataConversion::IntToString(dData), pPacket->GetType());
}
*/

void CDataExtracter::GetRealDataIndex(const int nStartIndex,
									  const int nEndIndex,
									  const int nDataStart,
									  const int nDataEnd,
									  int& nRealStartIndex,
									  int& nRealEndIndex)
{
	if (nStartIndex >= 0) {
		nRealStartIndex = nDataStart + nStartIndex - 1;
		nRealEndIndex = nDataStart + nEndIndex - 1;
	}

	if (nStartIndex > nDataEnd) {
		nRealStartIndex =  -1;
		nRealEndIndex = -1;
	}
}


//수치조회창에 표현하기 위해서는
//일자: 2002/10/19\n
CString CDataExtracter::MakeString(const CString &strTitle,
								   const double dValue,
								   int& nMaxLength)
{
	if(dValue == -1)
		return "";

	CString data = CDataConversion::GetStringThousandFormat(CDataConversion::DoubleToString(dValue, 2));
	int nLength = data.GetLength();
	if(nLength > nMaxLength)
		nMaxLength = nLength;

	return strTitle + ":" + data + "\n"; 
}

CString CDataExtracter::MakeDateString(const CString &strTitle, const CString& strValue)
{
	return strTitle + ":" + strValue + "\n"; 
}

CString CDataExtracter::GetTitle(const CString &strSubGraphName,
								 CMainBlock* pMainBlock)
{
	CViewData viewData; 
	pMainBlock->GetViewDataListInAllBlock( NULL, viewData);
	if(viewData.GetColumnViewDataCount() <= 0)
		return "";

	int nCount = viewData.GetColumnViewDataCount();

	for(int i = 0 ;  i< nCount ; i++) {
		CColumnViewData columndata = viewData.GetAt(i);
		const CList<CRowViewData, CRowViewData&>& rowdatalist = columndata.GetRowViewDataList(); 
		POSITION pos = rowdatalist.GetHeadPosition();
		while(pos) {
			CRowViewData rowdata = rowdatalist.GetNext(pos);
			CString strTitle = GetRowViewData(strSubGraphName, &rowdata);
			if(!strTitle.IsEmpty())
				return strTitle;
		}
	}
	return "";
}

CString CDataExtracter::GetRowViewData(const CString &strSubGraphName,
									   CRowViewData* rowdata)
{
	CSubGraphViewDataList& subgraphlist = rowdata->GetSubGraphViewDataList();
	CList<CSubGraphViewData, CSubGraphViewData&>& subgraphdatalist = subgraphlist.GetSubGraphViewDataList();
	POSITION subPos = subgraphdatalist.GetHeadPosition();
	while(subPos) {
		CSubGraphViewData subgraphdata = subgraphdatalist.GetNext(subPos);
		CString strTitle = GetSubGraphViewData(strSubGraphName, &subgraphdata);
		if(!strTitle.IsEmpty())
			return strTitle;
	}
	return "";
}

CString CDataExtracter::GetSubGraphViewData(const CString &strSubGraphName,
											CSubGraphViewData* subgraphdata)
{
	CString strData = "";
	CString packetNames = subgraphdata->GetPacketNames();
	if (packetNames.Right(1) != ",")
		packetNames += ",";

	int nPacketCount = CDataConversion::GetStringDataCount(packetNames, ",");
	for(int npacket = 0 ; npacket< nPacketCount ; npacket++) {
		CString packetname = CDataConversion::GetStringData(packetNames, ",");
		if(packetname.CompareNoCase(strSubGraphName) == 0)
			return subgraphdata->GetTitle();
	}
	return strData;
}


