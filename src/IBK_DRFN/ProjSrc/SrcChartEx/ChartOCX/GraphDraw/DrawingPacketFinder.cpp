// DrawingPacketFinder.cpp: implementation of the CDrawingPacketFinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawingPacketFinder.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "PacketBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CDrawingPacketFinder

CPacket* CDrawingPacketFinder::GetPracticalPacket(const CString& strIndicatorName, const CList<CPacket*, CPacket*>& packetList) const
{
	if(packetList.GetCount() <= 0)
		return NULL;

	// "가격차트" -> "종가"
	// (2007/3/9 - Seung-Won, Bae) Do not use the Fixed Packet name.
	if( strIndicatorName == _MTEXT( C2_PRICE_CHART) && 4 == packetList.GetCount())
		return packetList.GetTail();
	// "선가격차트" -> "종가"	20121026 이상신.
	else if (strIndicatorName == _MTEXT( C2_LINE_PRICE_MA))
		return packetList.GetTail();
	// "대기매물", "Zig Zag", "꺽은선차트" -> 마지막 packet
	else if( g_iMetaTable.IsViewVertMinMax_EndPacket(strIndicatorName))
		return packetList.GetTail();

	return packetList.GetHead();
}

CPacket* CDrawingPacketFinder::GetPacket(const CList<CPacket*, CPacket*>& packetList, const CString& strPacketName) const
{
	POSITION pos = packetList.GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = packetList.GetNext(pos);
		if(pPacket->GetName() == strPacketName)
			return pPacket;
	}
	return NULL;
}

CPacket* CDrawingPacketFinder::GetPacket(const CList<CPacket*, CPacket*>& packetList, const int nIndex) const
{
	POSITION pos = packetList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;

	return packetList.GetAt(pos);
}

bool CDrawingPacketFinder::GetPacketData(const CPacket* pPacket, const int nIndex, double& dData) const
{
	return false;
}