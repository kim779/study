#include "stdafx.h"
#include "listctrlSort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CListCtrlSort::CListCtrlSort(CListCtrl *pList, const int nCol)
{
	m_pList = pList;
	if (!m_pList)
		return;

	CString	txt;
	DWORD	dw = 0;
	const	int	totItems = m_pList->GetItemCount();
	std::shared_ptr<CSortItem> pItem = nullptr;

	m_sitems.RemoveAll();
	for (int ii = 0; ii < totItems; ii++)
	{
		dw = m_pList->GetItemData(ii);
		txt = m_pList->GetItemText(ii, nCol);
		pItem = std::make_shared<CSortItem>(dw, txt);
		m_sitems.Add(pItem);
		m_pList->SetItemData(ii, (DWORD)pItem.get());
	}
}

CListCtrlSort::~CListCtrlSort()
{
	if (!m_pList)
		return;

	const	int	totItems = m_sitems.GetCount();
	for (int ii = 0; ii < totItems; ii++)
		m_sitems.GetAt(ii).reset();
	m_sitems.RemoveAll();
}

void CListCtrlSort::Sort(BOOL bAsc, SortDataType dtType)
{
	const	long	lParamSort = bAsc ? dtType : -dtType;
	m_pList->SortItems(Compare, lParamSort);
}

int CALLBACK CListCtrlSort::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const	CSortItem *pItem1 = (CSortItem*)lParam1;
	const	CSortItem *pItem2 = (CSortItem*)lParam2;

	if (!pItem1 || !pItem2)
		return 0;

	const	int	nOrder = (lParamSort < 0) ? -1 : 1;
	const	SortDataType dtType = (SortDataType) (lParamSort * nOrder); 

	switch (dtType)
	{
	case SortDataType::dtINT:
		return (atol(pItem1->m_txt) - atol(pItem2->m_txt)) * nOrder;
	case SortDataType::dtSTRING:
		return pItem1->m_txt.Compare(pItem2->m_txt) * nOrder;
	default:
		return 0;
	}
}
