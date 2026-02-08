// ToolandRButtonItemMgr.cpp: implementation of the CToolandRButtonItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ToolandRButtonItemMgr.h"

#include "SuperGridCtrl.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolandRButtonItemMgr::CToolandRButtonItemMgr()
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CToolandRButtonItemMgr::~CToolandRButtonItemMgr()
{
	POSITION pos = m_ToolRButtonItemList.GetHeadPosition();
	while(pos) {
		CItemInfo* pItemInfo = (CItemInfo* )m_ToolRButtonItemList.GetNext(pos);
		if(pItemInfo != NULL)
			delete pItemInfo;
	}
	m_ToolRButtonItemList.RemoveAll();
}

void CToolandRButtonItemMgr::Init()
{
	for(int i = 0 ; i < IMetaTable::m_nToolOptionCount; i++)
	{
		CItemInfo* pItemInfo = new CItemInfo();
		pItemInfo->SetItemText( _LTEXT4( m_eToolOptions[ i]));  //add items text
		SetSubItemInfo(i, _LTEXT4( m_eToolOptions[i]), pItemInfo); //add subitem text
		m_ToolRButtonItemList.AddTail(pItemInfo);
	}
}

void CToolandRButtonItemMgr::SetSubItemInfo(const int nIndex, const CString &strItemName, CItemInfo *pItemInfo)
{
	//add subitem text
	int nColumCount = 2;
	for(int y=0 ; y< nColumCount ; y++) {
		if(y == CToolandRButtonItemMgr::FIRST_SUBITEM)	
			SetFirstSubItemInfo(nIndex, strItemName, pItemInfo);
		else if(y == CToolandRButtonItemMgr::SECOND_SUBITEM)	
			SetSecondSubItemInfo(nIndex, strItemName, pItemInfo);
	}
}

void CToolandRButtonItemMgr::SetFirstSubItemInfo(const int nIndex, const CString &strItemName, CItemInfo *pItemInfo)
{
	switch(nIndex) {
		case EXTENSION_REDUCTION:
			SetFirst_Extension_Recuction_SubItem(strItemName, pItemInfo);
			break;
		case NUMERICAL_INQUIRY:
			SetFirst_Numerical_Inquiry_SubItem(strItemName, pItemInfo);
			break;
		case VIEWDATA:
			SetFirst_ViewData_SubItem(strItemName, pItemInfo);
			break;
		case NUMERICAL_INQUIRY_DATAVIEW:
			SetFirst_Numerical_Inquiry_DataView_SubItem(strItemName, pItemInfo);
			break;
		case INDICATOR_SEQUNCE_INQUIRY:
			SetFirst_Indicator_Sequance_Inquiry_SubItem(strItemName, pItemInfo);
			break;
		case AUTO_LINE:
			SetFirst_Auto_Line_SubItem(strItemName, pItemInfo);
			break;
		case ANALYTICALTOOL:
			SetFirst_AnalyticalTool_SubItem(strItemName, pItemInfo);
			break;
		case STOCKPRICEPREDICTION:
			SetFirst_StockPrice_Prediction_SubItem(strItemName, pItemInfo);
			break;
		case Raider:
			SetFirst_Raider_SubItem(strItemName, pItemInfo);
			break;
		default:
			pItemInfo->AddSubItemText(_T(""));      // 2 zero based subitems...
			break;
	}
}

void CToolandRButtonItemMgr::SetSecondSubItemInfo(const int nIndex, const CString &strItemName, CItemInfo *pItemInfo)
{
	switch(nIndex) {
		case VIEWDATA:
			SetSecond_ViewData_SubItem(strItemName, pItemInfo);
			break;
		case EXTENSION_REDUCTION:
			SetSecond_Extension_Recuction_SubItem(strItemName, pItemInfo);
			break;
		case NUMERICAL_INQUIRY:
			SetSecond_Numerical_Inquiry_SubItem(strItemName, pItemInfo);
			break;
		default:
			pItemInfo->AddSubItemText(_T(""));      // 2 zero based subitems...
			break;
	}
}

void CToolandRButtonItemMgr::SetFirst_Extension_Recuction_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_COUNT));      // 1 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 0 from subitems...really col #4 in view, live with it

	CStringList list;
	//initalized list for the combobox
	list.AddTail( _LTEXT4( C4_COUNT));
	list.AddTail( _LTEXT4( C4_RATE));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);
}

void CToolandRButtonItemMgr::SetFirst_Numerical_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_NOT_USE_POPUP));      // 1 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 1 from subitems...really col #4 in view, live with it
	CStringList list;
	//initalized list for the combobox
	list.AddTail( _LTEXT4( C4_USE_POPUP));
	list.AddTail( _LTEXT4( C4_NOT_USE_POPUP));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);
}

void CToolandRButtonItemMgr::SetFirst_ViewData_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_GRAPH_ORDER));      // 1 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 1 from subitems...really col #4 in view, live with it

	CStringList list;
	list.AddTail( _LTEXT4( C4_GRAPH_ORDER));
	list.AddTail( _LTEXT4( C4_PACKET_ORDER));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);
}

void CToolandRButtonItemMgr::SetFirst_Numerical_Inquiry_DataView_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText(_T("Status Bar"));      // 1 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 1 from subitems...really col #4 in view, live with it
	CStringList list ;
	//initalized list for the combobox
	list.AddTail( _LTEXT4( C4_OVER_DLG));
	list.AddTail("Status Bar");
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);	
}

void CToolandRButtonItemMgr::SetFirst_Indicator_Sequance_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText(_T("10"));      // 1 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 1 from subitems...really col #4 in view, live with it
	CStringList list;
	//initalized list for the combobox
	list.AddTail("10");
	list.AddTail("20");
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);
}

void CToolandRButtonItemMgr::SetFirst_Auto_Line_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_NO_DATA_DLG));      // 1 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 1 from subitems...really col #4 in view, live with it

	CStringList list;
	list.AddTail( _LTEXT4( C4_WITH_DATA_DLG));
	list.AddTail( _LTEXT4( C4_NO_DATA_DLG));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);
}

void CToolandRButtonItemMgr::SetFirst_StockPrice_Prediction_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText(_T("5"));      // 1 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 1 from subitems...really col #4 in view, live with it
	//initalized list for the combobox
	CStringList list;
	list.AddTail("5");
	list.AddTail("10");
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);
}

void CToolandRButtonItemMgr::SetSecond_Extension_Recuction_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_10_UNIT));      // 2 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 1);//col # 1 from subitems...really col #4 in view, live with it

	CStringList list;
	list.AddTail( _LTEXT4( C4_10_UNIT));
	list.AddTail( _LTEXT4( C4_20_UNIT));
	list.AddTail( _LTEXT4( C4_30_UNIT));
	list.AddTail( _LTEXT4( C4_40_UNIT));
	list.AddTail( _LTEXT4( C4_50_UNIT));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(1, &list);
}

void CToolandRButtonItemMgr::SetSecond_Numerical_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_WITH_DATA_DLG2));      // 2 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 1);//col # 1 from subitems...really col #4 in view, live with it

	CStringList list;
	list.AddTail( _LTEXT4( C4_WITH_DATA_DLG2));
	list.AddTail( _LTEXT4( C4_NO_DATA_DLG2));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(1, &list);
}

void CToolandRButtonItemMgr::SetSecond_ViewData_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_NOT_COLORED_SIGN));      // 2 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 1);//col # 1 from subitems...really col #4 in view, live with it

	CStringList list;
	list.AddTail( _LTEXT4( C4_NOT_COLORED_SIGN));
	list.AddTail( _LTEXT4( C4_COLORED_SIGN));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(1, &list);
}

CItemInfo* CToolandRButtonItemMgr::GetItemInfowithItemName(const CString &strItemName)
{
	POSITION pos = m_ToolRButtonItemList.GetHeadPosition();
	while(pos) {
		CItemInfo* pItemInfo = (CItemInfo* )m_ToolRButtonItemList.GetNext(pos);
		CString strItem = pItemInfo->GetItemText(); 
		if(strItemName.CompareNoCase(strItem) == 0)
			return pItemInfo;
	}
	return NULL;
}

void CToolandRButtonItemMgr::ChangeItemInfo(const CString& strItmeName, LV_ITEM *plvItem)
{
	CString Item = (CString)plvItem->pszText;

	POSITION pos = m_ToolRButtonItemList.GetHeadPosition();
	while(pos) {
		CItemInfo* pItemInfo = (CItemInfo* )m_ToolRButtonItemList.GetNext(pos);
		CString strItem = pItemInfo->GetItemText(); 
		if(strItmeName.CompareNoCase(strItem) == 0) {
			if(pItemInfo!=NULL) {
				if(plvItem->iSubItem==0)
					pItemInfo->SetItemText(Item);
				else //subitem data 
					pItemInfo->SetSubItemText(plvItem->iSubItem-1, Item);
			}
		}			
	}
}

CItemInfo* CToolandRButtonItemMgr::ChangeItemInfo(const CString &strItemName, const CString &strLimit1, const CString &strLimit2)
{
	CString strName = _MTOL4( strItemName);

	POSITION pos = m_ToolRButtonItemList.GetHeadPosition();
	while(pos) {
		CItemInfo* pItemInfo = (CItemInfo* )m_ToolRButtonItemList.GetNext(pos);
		CString strItem = pItemInfo->GetItemText(); 
		if(strName.CompareNoCase(strItem) == 0) {
			if(pItemInfo!=NULL) {
				pItemInfo->SetSubItemText(0, _MTOL4( strLimit1));
				pItemInfo->SetSubItemText(1, _MTOL4( strLimit2));
			}
			return pItemInfo;
		}			
	}
	return NULL;
}

void CToolandRButtonItemMgr::SetFirst_AnalyticalTool_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_NO_DATA_DLG));      // 2 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 1 from subitems...really col #4 in view, live with it

	CStringList list;
	list.AddTail( _LTEXT4( C4_WITH_DATA_DLG));
	list.AddTail( _LTEXT4( C4_NO_DATA_DLG));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);
}


void CToolandRButtonItemMgr::SetFirst_Raider_SubItem(const CString &strItemName, CItemInfo *pItemInfo)
{
	pItemInfo->AddSubItemText( _LTEXT4( C4_BUTTON_TYPE));      // 2 zero based subitems...
	pItemInfo->SetControlType(pItemInfo->CONTROLTYPE::combobox, 0);//col # 1 from subitems...really col #4 in view, live with it

	CStringList list;
	list.AddTail( _LTEXT4( C4_BUTTON_TYPE));
	list.AddTail( _LTEXT4( C4_FLAT_TYPE));
	list.AddTail("");
	list.AddTail("");
	pItemInfo->SetListData(0, &list);
}
