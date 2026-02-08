// MenuGridCtrl.cpp: implementation of the CFidQueryGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FidQueryGrid.h"
#include "../supergrid/ComboInListView.h"

#include "../resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFidQueryGrid::CFidQueryGrid()
{
	m_nColCount = 0;
}

CFidQueryGrid::~CFidQueryGrid()
{
}

void CFidQueryGrid::Initialize()
{
	if (!m_ImageList.Create(IDB_FOLDERS, 16, 1, RGB(0, 255, 255)))
		return ;

	SetImageList(&m_ImageList, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);

	SetSelBackColor(RGB(171, 195, 211));
	SetSelTextColor(RGB(0, 0, 0));

	SetExtendedStyle(LVS_EX_GRIDLINES);

	LPTSTR lpszCols[] = {_T("ID"), _T("ChartID/Array"), _T("RealID/GridSymbol"), _T("ABS/GridCnt"), NULL};
	int pColWidth[] = {140, 80, 100, 60};
	m_nColCount = sizeof(pColWidth)/sizeof(int);

	LV_COLUMN   lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	int i = 0;
	for (i = 0; lpszCols[i]!=NULL; i++)
	{
		lvColumn.cx = pColWidth[i];
		lvColumn.pszText = lpszCols[i];
		InsertColumn(i,&lvColumn);
	}

	char *szData[] = {_T("FALSE"), _T("TRUE"), NULL};
	for (i=0; i<2; i++)
	{
		m_ArrBoolIndex.Add(i);
		m_ArrBool.Add(szData[i]);
	}

	SetInitData();
}

void CFidQueryGrid::SetInitData()
{
	CItemInfo* lp = new CItemInfo();
	lp->SetImage(4);
	lp->SetItemText(_T("ROOT BLOCK"));
	CTreeItem * pRoot = InsertRootItem(lp);
	SetChildrenFlag(pRoot, TRUE);
}

void CFidQueryGrid::ChangeRootName(CString strName, COLORREF color)
{
	CSuperGridCtrl::CTreeItem *pItem = NULL;
	pItem = GetRootItem(0);
	if (pItem)
	{
		CItemInfo* pInfo = GetData(pItem);
		if (pInfo)
		{
			pInfo->SetItemText(strName, color);
			UpdateData(pItem, pInfo);
		}
	}
}

CSuperGridCtrl::CTreeItem *CFidQueryGrid::InsertNewNode(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant)
{
	CString strData;

	CItemInfo* pInfo = new CItemInfo;
	pInfo->SetItemText(L"");
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, 0);

	pInfo->AddSubItemText(_T("FALSE"));
	pInfo->SetControlType(pInfo->CONTROLTYPE::combobox, 1);
	pInfo->SetListData(1, &m_ArrBool, &m_ArrBoolIndex);

	pInfo->AddSubItemText(L"");
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, 2);

	pInfo->AddSubItemText(L"");
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, 3);

	CTreeItem *pNewItem = NULL;
	if (bParant)
		pNewItem = CSuperGridCtrl::InsertItem(pParentItem, pInfo, TRUE);
	else
		pNewItem = CSuperGridCtrl::InsertItemAfter(pParentItem, pInfo, TRUE);

	SetChildrenFlag(pNewItem, TRUE);
	return pNewItem;
}

CSuperGridCtrl::CTreeItem *CFidQueryGrid::InsertNewItem(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant)
{
	CString strData;

	CItemInfo* pInfo = new CItemInfo;
	pInfo->SetItemText(L"");
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, 0);

	pInfo->AddSubItemText(_T(""));
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, 1);

	pInfo->AddSubItemText(_T(""));
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, 2);

	pInfo->AddSubItemText(_T("FALSE"));
	pInfo->SetControlType(pInfo->CONTROLTYPE::combobox, 3);
	pInfo->SetListData(3, &m_ArrBool, &m_ArrBoolIndex);

	CTreeItem *pNewItem = NULL;
	if (bParant)
		pNewItem = CSuperGridCtrl::InsertItem(pParentItem, pInfo, TRUE);
	else
		pNewItem = CSuperGridCtrl::InsertItemAfter(pParentItem, pInfo, TRUE);

	return pNewItem;
}

CSuperGridCtrl::CTreeItem *CFidQueryGrid::InsertNewSeparator(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant)
{
	CItemInfo* pInfo = new CItemInfo;
	pInfo->SetItemText(L"------------------------");
	
	pInfo->AddSubItemText("Separator");

	CTreeItem *pNewItem = NULL;
	if (bParant)
		pNewItem = CSuperGridCtrl::InsertItem(pParentItem, pInfo, TRUE);
	else
		pNewItem = CSuperGridCtrl::InsertItemAfter(pParentItem, pInfo, TRUE);

	return pNewItem;
}


CSuperGridCtrl::CTreeItem *CFidQueryGrid::GetSelectedNodeItem()
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1) return NULL;
	CTreeItem *pParentItem = (CTreeItem *)GetItemData(nIndex);
	if (!ItemHasChildren(pParentItem)) {
		pParentItem = GetParentItem(pParentItem);
	}
	return pParentItem;
}

CSuperGridCtrl::CTreeItem *CFidQueryGrid::GetSelectedTreeItem()
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1) return NULL;
	CTreeItem *pItem = (CTreeItem *)GetItemData(nIndex);

	return pItem;
}

COLORREF CFidQueryGrid::GetCellRGB()
{
	return RGB(255,202,255);
}


int CFidQueryGrid::GetIcon(const CTreeItem* pItem)
{
	if(pItem!=NULL)
	{
		int n = GetData(pItem)->GetImage();
		if(n!=-1)
			return n;
		
		int iImage = 0;
		if(ItemHasChildren(pItem))
		{
			IsCollapsed(pItem) ? iImage = 1/*close icon*/:iImage = 0;/*open icon*/
		}
		else
			iImage = 2;//doc icon
		return iImage;
	}
	return 0;
}


BOOL CFidQueryGrid::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	if (CSuperGridCtrl::IsRoot(pItem)) return FALSE;
	return TRUE;
}
