// DataOfPacketPage.cpp: implementation of the CDataOfPacketPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DataOfPacketPage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "PacketSuperGrid.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataOfPacketPage::CDataOfPacketPage()
{

}

CDataOfPacketPage::~CDataOfPacketPage()
{

}

//넘어온 Data로 PacketList 초기화.
void CDataOfPacketPage::AddData(const int nCheck, const CString& szpacket, const CString& szField, const CString& szLength, const CString& szType, const CString& szReal, CPacketSuperGrid *PacketList)
{
	//수정되기 이전에 Real을 사용하기로 한 Packet 아이템들.
	if(nCheck == 1 && szReal.Compare( _LTEXT6( C6_YES)) == 0) 
		m_strOldRealPacketCount += szpacket + ";";

	PacketList->AddItem(nCheck, szpacket, szField, szLength, szType, szReal);
}

CString CDataOfPacketPage::GetAllDataFromListCtrl(CPacketSuperGrid* PacketList)
{
	CString strInfoOfAllPacketItems = "";
	int Count = PacketList->GetItemCount();

	for(int i = 0; i< Count ; i++) {
		CString parent = PacketList->GetItemText(i, 0);
		if(parent.Compare("Packet") == 0 || parent.Compare( _LTEXT6( C6_USER_DEFINE)) == 0 || parent.Compare( _LTEXT6( C6_INPUT_ITEM)) == 0)
			;
		else {
			//Check or no Check를 확인.
			if(PacketList->GetCheck(i) != 0 ) 
				strInfoOfAllPacketItems += "1;";
			else 
				strInfoOfAllPacketItems += "0;";
		
			//subitem 내용을 string에 넣음.
			for(int y=0 ; y< 5; y++)
			{
				//ocx로 보내기 위한 String
				if( y == 1 || y == 2)			strInfoOfAllPacketItems += PacketList->GetItemText(i, y) + ";";
				else if( y == 0)				strInfoOfAllPacketItems += CString( _CRTM0( PacketList->GetItemText(i, y))) + ";";
				else if( y == 3 || y == 4)		strInfoOfAllPacketItems += CString( _LTOM6( PacketList->GetItemText(i, y))) + ";";
				if( y == 4) strInfoOfAllPacketItems += "\r\n";
			}
		}		
	}
	return strInfoOfAllPacketItems;
}

CString CDataOfPacketPage::GetUseRealPacketItems(const CString& strSavedData)
{
	if(strSavedData.IsEmpty())
		return "";

	CString _szData = strSavedData;
	
	if(_szData.Right(2) != "\r\n")
		_szData += "\r\n";

	CString strResult = "";
	int nCount = CDataConversion::GetStringDataCount(_szData, "\r\n");
	for(int i =0 ; i< nCount; i++) {
		CString szItemData = CDataConversion::GetStringData(_szData, "\r\n");
		int nCheck = atoi(CDataConversion::GetStringData(szItemData, ";"));
		CString szpacket = CDataConversion::GetStringData(szItemData, ";");
		CString szField = CDataConversion::GetStringData(szItemData, ";");
		CString szLength = CDataConversion::GetStringData(szItemData, ";");
		CString szType = CDataConversion::GetStringData(szItemData, ";");
		CString szReal = CDataConversion::GetStringData(szItemData, ";");

		if(nCheck == 1 && szReal.Compare( _MTEXT( C6_YES)) == 0) 
			strResult +=  szpacket+ ";";
	}
	return strResult;
}

void CDataOfPacketPage::Init(const CString &strReceiveDataFromOcx, CPacketSuperGrid *PacketList)
{
	if(strReceiveDataFromOcx.IsEmpty())
		return ;

	CString _szData = strReceiveDataFromOcx;
	if(_szData.Right(2) != "\r\n") _szData += "\r\n";

	int nCount = CDataConversion::GetStringDataCount(_szData, "\r\n");
	for(int i =0 ; i< nCount; i++)
	{
		CString szItemData = CDataConversion::GetStringData(_szData, "\r\n");
		int nCheck = atoi(CDataConversion::GetStringData(szItemData, ";"));
		CString szpacket = CDataConversion::GetStringData(szItemData, ";");
		CString szField = CDataConversion::GetStringData(szItemData, ";");
		CString szLength = CDataConversion::GetStringData(szItemData, ";");
		CString szType = CDataConversion::GetStringData(szItemData, ";");
		CString szReal = CDataConversion::GetStringData(szItemData, ";");
		AddData(nCheck, _MTOL0( szpacket), szField, szLength, _MTOL6( szType), _MTOL6( szReal), PacketList);	
	}
}
