// ExListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ExListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl

CExListCtrl::CExListCtrl()
{
	//m_bFullSelect = TRUE;
	m_bFullSelect = FALSE;
}

CExListCtrl::~CExListCtrl()
{
}


BEGIN_MESSAGE_MAP(CExListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnItemClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CExListCtrl::SetAddColumn(int nItem, int nSubItem, LPCSTR strText)
{
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_TEXT;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.pszText = (LPSTR)strText;
	lvColumn.cx = GetStringWidth(lvColumn.pszText);
    
	if (lvColumn.mask & LVCF_SUBITEM)
	{
		if (nSubItem != -1)
			lvColumn.iSubItem = nSubItem;
		else
			lvColumn.iSubItem = nItem;
	}
	
	return InsertColumn(nItem, &lvColumn);
}

BOOL CExListCtrl::SetAddItem(int nItem, int nSubItem, LPCSTR strText,
						  BOOL bCheckStyle, BOOL bAdd, int nImageIndex)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPSTR)strText;

	if (m_bFullSelect)
		SetFullSelect();

	if (nImageIndex != -1)
	{
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage = nImageIndex;
	}

	if (bAdd == TRUE)
	{
		if (bCheckStyle)
			SetCheckStyle();
		return InsertItem(&lvItem);
	}

	return SetItem(&lvItem);
}

int CExListCtrl::GetSelItem()
{
	int nIndex = -1;

	int nCnt = GetSelectedCount();
	for(int i=0; i < nCnt; i++)
	{
		nIndex = GetNextItem(-1, LVNI_SELECTED);
	}

	return nIndex;
}

void CExListCtrl::SetDataItem(int nItem, QuickList stQuickList, BOOL bAdd)
{
	SetAddItem(nItem, 0, stQuickList.strQuickName, TRUE, bAdd);
	SetAddItem(nItem, 1, stQuickList.strFileName);
	SetAddItem(nItem, 2, stQuickList.strFilePath);
	SetAddItem(nItem, 3, stQuickList.strFolderPath);
	SetAddItem(nItem, 4, stQuickList.strType);
	SetAddItem(nItem, 5, (stQuickList.bAlertReg == TRUE)?"TRUE":"FALSE");

	
		SetCheck(nItem, stQuickList.bCheck);
	
}

void CExListCtrl::SetDataItem(int nItem, CString strType, XMLTreeInfoFile	stXMLTreeInfoFile)
{
	SetAddItem(nItem, 0, stXMLTreeInfoFile.strFileName, TRUE, TRUE);
	SetAddItem(nItem, 1, stXMLTreeInfoFile.strFilePath);
	SetAddItem(nItem, 2, stXMLTreeInfoFile.strFolderPath);
	SetAddItem(nItem, 3, stXMLTreeInfoFile.strAlertReg);
	SetAddItem(nItem, 4, strType);
	
	SetCheck(nItem, stXMLTreeInfoFile.bQuickReg);
}

void CExListCtrl::GetDataItem(int nItem, QuickList &stQuickList)
{
	stQuickList.strQuickName = GetItemText(nItem, 0);
	stQuickList.strFileName = GetItemText(nItem, 1);
	stQuickList.strFilePath = GetItemText(nItem, 2);
	stQuickList.strFolderPath = GetItemText(nItem, 3);

	stQuickList.strType = GetItemText(nItem, 4);
	stQuickList.bAlertReg = (GetItemText(nItem, 5) == "TRUE")?TRUE:FALSE;

	stQuickList.bCheck = GetCheck(nItem);
}

void CExListCtrl::GetDataItem(int nItem, CString &strType, XMLTreeInfoFile &stXMLTreeInfoFile)
{
	stXMLTreeInfoFile.strFileName = GetItemText(nItem, 0);
	stXMLTreeInfoFile.strFilePath = GetItemText(nItem, 1);
	stXMLTreeInfoFile.strFolderPath = GetItemText(nItem, 2);
	stXMLTreeInfoFile.strAlertReg = GetItemText(nItem, 3);
	strType = GetItemText(nItem, 4);

	stXMLTreeInfoFile.bQuickReg = GetCheck(nItem);
}

void CExListCtrl::OnItemClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nItem = pNMListView->iItem;
	BOOL bCheck = GetCheck(nItem);
	if (m_bFullSelect)
	{
		bCheck = GetCheck(nItem);
		if(bCheck)
			SetCheck(nItem, FALSE);
		else
			SetCheck(nItem, TRUE);
	}

	*pResult = 0;
}

void CExListCtrl::OnReceiveItemCheck(WPARAM wParam, LPARAM lParam)
{
	int nSelItem = (int)lParam;
	BOOL bCheck = (BOOL)wParam;

	if(nSelItem < 0)
		return;

	SetCheck(nSelItem, !bCheck);
	Update(nSelItem);
}



void CExListCtrl::AutoSize()
{
	CHeaderCtrl* pHD = (CHeaderCtrl*)GetDlgItem(0);

	pHD->ModifyStyle(0, HDS_HOTTRACK);  //헤더 컨트롤에 핫 트래킹 설정
	int nCount = pHD->GetItemCount(); 
	
	for(int i =0; i < nCount; i++)
	{
		SetColumnWidth(i, LVSCW_AUTOSIZE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Explain : insert되는 header의 size를 client size에 맞게 resizing한다.
/////////////////////////////////////////////////////////////////////////////
void CExListCtrl::CalculateColSize()
{
	double dTotWidth = 0.00;
	double dColWidth = 0.00;
	double dRealWidth = 0.00;
	double dModWidth = 0.00;

	CRect rect;
	GetClientRect(rect);

	int nWidth = rect.right - rect.left;

	SetColumnWidth(0, nWidth);

	/*
	for(int i=0; i<m_nColCnt; i++)
		dRealWidth += GetColumnWidth(i);

	for(i=0; i<m_nColCnt; i++)
	{
		dColWidth = GetColumnWidth(i);
		dColWidth = dColWidth * dTotWidth / dRealWidth;
		dModWidth += dColWidth - (int)dColWidth;
		SetColumnWidth(i, (int)dColWidth);

		if( i == (m_nColCnt - 1) )
		{
			dColWidth = dColWidth + dModWidth;
			SetColumnWidth(i, (int)dColWidth);
		}
	}
	*/
}



/////////////////////////////////////////////////////////////////////////////
// Explain : Insert CheckBox
// Date : 2002-12-12
/////////////////////////////////////////////////////////////////////////////
void CExListCtrl::SetCheckStyle()
{
	DWORD dwStyle;
	dwStyle = GetExtendedStyle();
	dwStyle = dwStyle | LVS_EX_CHECKBOXES; /*| LVS_EX_FULLROWSELECT | LVS_EX_TRACKSELECT |*/
				 //LVS_EX_ONECLICKACTIVATE | LVS_EX_HEADERDRAGDROP;
	SetExtendedStyle(dwStyle); 
}

/////////////////////////////////////////////////////////////////////////////
// Explain : Item 선택시 특정 Column만 선택되지 않고 Item에 있는 모든 Column이 
//				 Select되게 한다.
// Date : 2002-12-12
/////////////////////////////////////////////////////////////////////////////
BOOL CExListCtrl::SetFullSelect()
{
	BOOL bSet;

	DWORD dwStyle = GetExtendedStyle() | LVS_EX_FULLROWSELECT;
	bSet = SetExtendedStyle(dwStyle); 
	
	return bSet;
}



