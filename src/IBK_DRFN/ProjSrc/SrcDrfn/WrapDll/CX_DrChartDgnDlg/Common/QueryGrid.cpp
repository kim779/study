// MenuGridCtrl.cpp: implementation of the CQueryGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QueryGrid.h"
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

CQueryGrid::CQueryGrid()
{
	m_nColCount = 0;
}

CQueryGrid::~CQueryGrid()
{
}

void CQueryGrid::Initialize()
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

//	LPTSTR lpszCols[] = {_T("ID"), _T("Type"), _T("Size"), _T("ARRAY"), _T("BYTEORD"), _T("COUNT ITEM NAME"), NULL};
	LPTSTR lpszCols[] = {_T("ID"), _T("Type/Arr"), _T("Size/CntName"), _T("ABS"), NULL};
	int pColWidth[] = {170, 65, 100, 65};
	m_nColCount = sizeof(pColWidth)/sizeof(int);

	LV_COLUMN   lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	for (int i = 0; lpszCols[i]!=NULL; i++)
	{
		lvColumn.cx = pColWidth[i];
		lvColumn.pszText = lpszCols[i];
		InsertColumn(i,&lvColumn);
	}

	CTypeData::GetArrTypeName(DATA_GUBUN, &m_ArrCorpName);
	CTypeData::GetArrTypeID(DATA_GUBUN, &m_ArrCorpID);

	CTypeData::GetArrTypeName(DATA_TYPE, &m_ArrCPName);
	CTypeData::GetArrTypeID(DATA_TYPE, &m_ArrCPID);

	CTypeData::GetArrTypeName(DATA_BOOL, &m_ArrGradeName);
	CTypeData::GetArrTypeID(DATA_BOOL, &m_ArrGradeID);

	SetInitData();
}

void CQueryGrid::SetInitData()
{
	CItemInfo* lp = new CItemInfo();
	lp->SetImage(4);
	lp->SetItemText(_T("ROOT BLOCK"));
	CTreeItem * pRoot = InsertRootItem(lp);
	SetChildrenFlag(pRoot, TRUE);
}

void CQueryGrid::ChangeRootName(CString strName, COLORREF color)
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

CSuperGridCtrl::CTreeItem *CQueryGrid::InsertNewNode(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant)
{
	CString strData;

	CItemInfo* pInfo = new CItemInfo;
	pInfo->SetItemText(L"");
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, COLIDX_NAME);

	strData = m_ArrGradeName.GetAt(0);
	pInfo->AddSubItemText(strData);
	pInfo->SetControlType(pInfo->CONTROLTYPE::combobox, COLIDX_ARRAY);
	pInfo->SetListData(COLIDX_ARRAY, &m_ArrGradeName, &m_ArrGradeID);

	pInfo->AddSubItemText(L"");
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, COLIDX_CNTNAME);

	CTreeItem *pNewItem = NULL;
	if (bParant)
		pNewItem = CSuperGridCtrl::InsertItem(pParentItem, pInfo, TRUE);
	else
		pNewItem = CSuperGridCtrl::InsertItemAfter(pParentItem, pInfo, TRUE);

	SetChildrenFlag(pNewItem, TRUE);
	return pNewItem;
}

CSuperGridCtrl::CTreeItem *CQueryGrid::InsertNewItem(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant)
{
	CString strData;

	CItemInfo* pInfo = new CItemInfo;
	pInfo->SetItemText(L"");
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, COLIDX_NAME);

	strData = m_ArrCPName.GetAt(0);
	pInfo->AddSubItemText(strData);
	pInfo->SetControlType(pInfo->CONTROLTYPE::combobox, COLIDX_TYPE);
	pInfo->SetListData(COLIDX_TYPE, &m_ArrCPName, &m_ArrCPID);

	pInfo->AddSubItemText(_T(""));
	pInfo->SetControlType(pInfo->CONTROLTYPE::edit, COLIDX_SIZE);

	strData = m_ArrGradeName.GetAt(0);
	pInfo->AddSubItemText(strData);
	pInfo->SetControlType(pInfo->CONTROLTYPE::combobox, COLIDX_BYTEORD);
	pInfo->SetListData(COLIDX_BYTEORD, &m_ArrGradeName, &m_ArrGradeID);

	CTreeItem *pNewItem = NULL;
	if (bParant)
		pNewItem = CSuperGridCtrl::InsertItem(pParentItem, pInfo, TRUE);
	else
		pNewItem = CSuperGridCtrl::InsertItemAfter(pParentItem, pInfo, TRUE);

	return pNewItem;
}

CSuperGridCtrl::CTreeItem *CQueryGrid::InsertNewSeparator(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant)
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


CSuperGridCtrl::CTreeItem *CQueryGrid::GetSelectedNodeItem()
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1) return NULL;
	CTreeItem *pParentItem = (CTreeItem *)GetItemData(nIndex);
	if (!ItemHasChildren(pParentItem)) {
		pParentItem = GetParentItem(pParentItem);
	}
	return pParentItem;
}

CSuperGridCtrl::CTreeItem *CQueryGrid::GetSelectedTreeItem()
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1) return NULL;
	CTreeItem *pItem = (CTreeItem *)GetItemData(nIndex);

	return pItem;
}

COLORREF CQueryGrid::GetCellRGB()
{
	return RGB(255,202,255);
}


int CQueryGrid::GetIcon(const CTreeItem* pItem)
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


BOOL CQueryGrid::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	if (CSuperGridCtrl::IsRoot(pItem)) return FALSE;
	return TRUE;
}





/////////////////////////////////////////////////////////////////
//
//	메뉴구성 코드 정보.
//
/////////////////////////////////////////////////////////////////

char	*CTypeData::g_szGubunName[] = { _T("Block"), _T("Field"), NULL };
int		CTypeData::g_nGubunID[] = { 0, 1 };
char	*CTypeData::g_szTypeName[] = { _T("char"), _T("integer"), _T("bool"), _T("long"), _T("double"), _T("string"), NULL };
int		CTypeData::g_nTypeID[] = { 1, 2, 3, 4, 5, 6 };
char	*CTypeData::g_szBoolName[] = { _T("FALSE"), _T("TRUE"), NULL };
int		CTypeData::g_nBoolID[] = { 0, 1 };

void CTypeData::GetArrTypeName(int nType, CStringArray *pArr)
{
	int i=0;
	switch (nType)
	{
	case DATA_GUBUN:
		for (i=0; g_szGubunName[i] != NULL; i++)
		{
			pArr->Add(g_szGubunName[i]);
		}
		break;
	case DATA_TYPE:
		for (i=0; g_szTypeName[i] != NULL; i++)
		{
			pArr->Add(g_szTypeName[i]);
		}
		break;
	case DATA_BOOL:
		for (i=0; g_szBoolName[i] != NULL; i++)
		{
			pArr->Add(g_szBoolName[i]);
		}
		break;
	}
}

void CTypeData::GetArrTypeID(int nType, CDWordArray *pArr)
{
	int i=0;
	switch (nType)
	{
	case DATA_GUBUN:
		for (i=0; g_szGubunName[i] != NULL; i++)
		{
			pArr->Add(g_nGubunID[i]);
		}
		break;
	case DATA_TYPE:
		for (i=0; g_szTypeName[i] != NULL; i++)
		{
			pArr->Add(g_nTypeID[i]);
		}
		break;
	case DATA_BOOL:
		for (i=0; g_szBoolName[i] != NULL; i++)
		{
			pArr->Add(g_nBoolID[i]);
		}
		break;
	}
}

CString CTypeData::GetTypeName(int nType, int nID)
{
	CString strData;
	int i=0;
	switch (nType)
	{
	case DATA_GUBUN:
		for (i=0; g_szGubunName[i] != NULL; i++)
		{
			if (g_nGubunID[i] == nID)
			{
				strData.Format("%s", g_szGubunName[i]);
				return strData;
			}
		}
		break;
	case DATA_TYPE:
		for (i=0; g_szTypeName[i] != NULL; i++)
		{
			if (g_nTypeID[i] == nID)
			{
				strData.Format("%s", g_szTypeName[i]);
				return strData;
			}
		}
		break;
	case DATA_BOOL:
		for (i=0; g_szBoolName[i] != NULL; i++)
		{
			if (g_nBoolID[i] == nID)
			{
				strData.Format("%s", g_szBoolName[i]);
				return strData;
			}
		}
		break;
	}

	return "";
}

int CTypeData::GetTypeID(int nType, LPCSTR szName)
{
	CString strData;
	int i=0;
	switch (nType)
	{
	case DATA_GUBUN:
		for (i=0; g_szGubunName[i] != NULL; i++)
		{
			if (strcmp(szName, g_szGubunName[i]) == 0)
				return g_nGubunID[i];
		}
		break;
	case DATA_TYPE:
		for (i=0; g_szTypeName[i] != NULL; i++)
		{
			if (strcmp(szName, g_szTypeName[i]) == 0)
				return g_nTypeID[i];
		}
		break;
	case DATA_BOOL:
		for (i=0; g_szBoolName[i] != NULL; i++)
		{
			if (strcmp(szName, g_szBoolName[i]) == 0)
				return g_nBoolID[i];
		}
		break;
	}

	return -1;
}
