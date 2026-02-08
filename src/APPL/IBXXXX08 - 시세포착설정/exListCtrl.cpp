#include "stdafx.h"
#include "IBXXXX08.h"
#include "exListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CListCtrlSort::CListCtrlSort(CListCtrl* pList, const int nCol)
{
	m_pList = pList;
	ASSERT(m_pList);
	if (m_pList == nullptr)
		return;

	const int totItems = m_pList->GetItemCount();

	for (int ii = 0; ii < totItems; ii++)
	{
		const DWORD dw = m_pList->GetItemData(ii);
		CString txt = m_pList->GetItemText(ii, nCol);
		const auto& item = _vsort.emplace_back(std::make_unique<CSortItem>(dw, txt));
		m_pList->SetItemData(ii, (DWORD)item.get());
	}
}

CListCtrlSort::~CListCtrlSort()
{
	_vsort.clear();
}

void CListCtrlSort::Sort(BOOL bAsc, SortDataType dtType)
{
	const long lParamSort = bAsc ? dtType : -dtType;
	m_pList->SortItems(Compare, lParamSort);
}

int CALLBACK CListCtrlSort::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortItem *pItem1 = (CSortItem*)lParam1;
	CSortItem *pItem2 = (CSortItem*)lParam2;
	ASSERT(pItem1 && pItem2);
	if (pItem1 && pItem2)
	{
		const int nOrder = (lParamSort < 0) ? -1 : 1;
		const SortDataType dtType = (SortDataType)(lParamSort * nOrder);

		switch (dtType)
		{
		case SortDataType::dtINT:
			return (atol(pItem1->m_txt) - atol(pItem2->m_txt)) * nOrder;
		case SortDataType::dtSTRING:
			return pItem1->m_txt.Compare(pItem2->m_txt) * nOrder;
		default:
			ASSERT("Error: attempt to sort a column without type.");
			return 0;
		}
	}
	return 0;
}
//============================================================================================
CExListCtrl::CExListCtrl()
{
	m_bSort = true;
	m_nSortItem = 0;
	m_bSortAsc = TRUE;
}

CExListCtrl::~CExListCtrl()
{
}


BEGIN_MESSAGE_MAP(CExListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CExListCtrl::SetScroll(int index, bool bSel /* = true*/)
{
	if (bSel)
		SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	
	CRect rc;
	GetItemRect(0, rc, LVIR_LABEL);

	CSize size;
	size.cx = 0;
	size.cy = rc.Height() * (index - GetTopIndex());
	Scroll(size);
}

void CExListCtrl::SetFullRowSelect()
{
	const DWORD styles = LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(m_hWnd, styles, styles);  
}

void CExListCtrl::SetGridLines()
{
	const DWORD styles = LVS_EX_GRIDLINES | LVS_OWNERDRAWFIXED;
	ListView_SetExtendedListViewStyleEx(m_hWnd, styles, styles);  
}
void CExListCtrl::ListSort()
{
	CWaitCursor	wait;
	CListCtrlSort sc(this, m_nSortItem);
	sc.Sort(m_bSortAsc, CListCtrlSort::SortDataType::dtSTRING);	
	AfxGetApp()->RestoreWaitCursor();
}

void CExListCtrl::ListSort(int col)
{
	m_bSortAsc = TRUE;
	m_nSortItem = col;
	ListSort();
}

void CExListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (!m_bSort)
		return;

	if (pNMListView->iSubItem == m_nSortItem)
	{
		m_bSortAsc = !m_bSortAsc;
	}
	else
	{
		m_nSortItem = pNMListView->iSubItem;
		m_bSortAsc = TRUE;
	}
	ListSort();
	
	*pResult = 0;
}
