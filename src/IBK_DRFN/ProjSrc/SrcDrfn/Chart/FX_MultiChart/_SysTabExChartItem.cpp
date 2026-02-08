#include "stdafx.h"
#include "./TabSystemTradingEx.h"

CSysTabExChartItem::CSysTabExChartItem()
{
	m_pChartItem = NULL;
}

long CSysTabExChartItem::GetChartItem()
{
	return (long)m_pChartItem;
}

void CSysTabExChartItem::SetChartItem(long dwNewChartItem) 
{
	m_pChartItem = (CChartItem*)dwNewChartItem;
}

BOOL CSysTabExChartItem::AddPacketAndGraph () 
{
	//return m_pChartItem->AddPacketAndGraph();
	CChartItemData *pChartItemData = m_pChartItem->GetPrimaryChartItemData();
	return pChartItemData->AddPacketAndGraphAnal();
}

BOOL CSysTabExChartItem::SetPacketData(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset) 
{
	return m_pChartItem->SetPacketData(szPacketName, szData, nLength, bReset);
}

void CSysTabExChartItem::SetNOnePageDataCount(long propVal) 
{
	m_pChartItem->SetNOnePageDataCount(propVal);
}

void CSysTabExChartItem::ShowInnerToolBar(BOOL p_bShow) 
{
	m_pChartItem->ShowInnerToolBar(p_bShow);
}

BOOL CSysTabExChartItem::InvokeAddIn (LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData) 
{
	return m_pChartItem->InvokeAddIn (p_szAddInDllName, p_nCommandType, p_lData) ;
}

BOOL CSysTabExChartItem::AddChartBlock (LPCTSTR lpChartName)
{
	return m_pChartItem->AddChartBlock (lpChartName);
}

CString CSysTabExChartItem::GetPacketName()
{
	//return m_pChartItem->GetPacketName();
	CChartItemData *pChartItemData = m_pChartItem->GetPrimaryChartItemData();
	return pChartItemData->GetPacketName();
}

