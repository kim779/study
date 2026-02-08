// ToolListData.cpp: implementation of the Tool List Data Info.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolListData.h"

//data
const int NUM_PACKETNAMEITEM = 24;
// (2008/1/8 - Seung-Won, Bae) for MultiLanguage
IMetaTable::E_METACODE ePacketNameItem[ NUM_PACKETNAMEITEM]	=
{
	IMetaTable::C0_SECOND_TIME,
	IMetaTable::C0_DATE_TIME,
	IMetaTable::C0_TEXT,
	IMetaTable::C0_OPEN,
	IMetaTable::C0_HIGH,
	IMetaTable::C0_LOW,
	IMetaTable::C0_CLOSE,
	IMetaTable::C0_VOLUME,
	IMetaTable::C0_ACCUMULATED_VOLUME,
	IMetaTable::C0_LOCK,
	IMetaTable::C0_ACCOUNT_RATE,
	IMetaTable::C0_EXCHANGE_RATE,
	IMetaTable::C0_TOTAL_ITEM_COUNT,
	IMetaTable::C0_UP_ITEM_COUNT,
	IMetaTable::C0_DOWN_ITEM_COUNT,
	IMetaTable::C0_ADL,
	IMetaTable::C0_MOBV,
	IMetaTable::C0_UP_ITEM_VOLUME,
	IMetaTable::C0_DOWN_ITEM_VOLUME,
	IMetaTable::C0_GUIDE_ITEM_VOLUME,
	IMetaTable::C0_W52_NEW_HIGH_ITEM_COUNT,
	IMetaTable::C0_W52_NEW_LOW_ITEM_COUNT,
	IMetaTable::C0_MARKET_STOCK_COUNT,
	IMetaTable::C0_DEALED_STOCK_COUNT,
};
