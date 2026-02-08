#include "stdafx.h"
#include "ExListCtrl.h"

#include "listEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_EDITCELL 1001

ListData::ListData(CString sData)
{
	m_sData = sData;
}
ListData::~ListData()
{
}

///////////////////////////////////////////////////////////
CExListCtrl::CExListCtrl()
{
	m_FixCol = -1;
	m_bFix = false;
	m_bSort = true;

	m_nSortItem = 0;
	m_bSortAsc = TRUE;
	//**m_Header.Detach();
	m_Ptr.RemoveAll();
	m_bIsEdit = false;
	m_bEditEnable = false;
	m_pEdit = NULL;
	m_disEditCol = -1;
	m_bInter = false;
  	m_curCol = 1;
}

CExListCtrl::~CExListCtrl()
{
	FreePtr();
	DeleteEdit();
}

BEGIN_MESSAGE_MAP(CExListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExListCtrl::InitHeaderData(CString* strHead, int* width, int cnt, int align)
{
	for (int ii = 0; ii < cnt; ii++)
	{
		InsertColumn(ii, strHead[ii], align, width[ii], 0);
	}
}

void CExListCtrl::SetInterest(bool bInter)
{
	m_bInter = bInter;
}

void CExListCtrl::DisableEditColumn(int col)
{
	m_disEditCol = col;
}

void CExListCtrl::SetDisableSort(bool bSort /* = false */)
{
	m_bSort = bSort;
}

void CExListCtrl::AddItem(CString* strData, int cnt)
{
	const int row = GetItemCount();

	for (int ii = 0; ii < cnt; ii++)
	{
		if (ii == 0)
			InsertItem(row, strData[ii]);
		else
			SetItemText(row, ii, strData[ii]);
	}
}

void CExListCtrl::HeaderFix(bool bFix, int col)
{
	m_bFix = bFix;
	m_FixCol = col;
}

int CExListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
			
	return 0;
}

BOOL CExListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	const HD_NOTIFY   *pHDN = (HD_NOTIFY*)lParam;

	if (!m_bFix)
		return CListCtrl::OnNotify(wParam, lParam, pResult);

        if (pHDN->hdr.code == HDN_BEGINTRACKW 
		|| pHDN->hdr.code == HDN_BEGINTRACKA)
	{
		if (m_FixCol == -1 || pHDN->iItem == m_FixCol)
		{
			*pResult = TRUE;	// disable tracking
			return TRUE;		// Processed message
		}
	}
 
        return CListCtrl::OnNotify(wParam, lParam, pResult);
}


void CExListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

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

void CExListCtrl::GetListState(int& col, BOOL& asc)
{
	col = m_nSortItem;
	asc = m_bSortAsc;
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

void CExListCtrl::SetEditLabels()
{
	m_bEditEnable = true;
}

void CExListCtrl::SetScroll(int index)
{
	SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	CRect rc;
	GetItemRect(0, rc, LVIR_LABEL);

	CSize size;
	size.cx = 0;
	size.cy = rc.Height() * (index - GetTopIndex());
	Scroll(size);
}

void CExListCtrl::SetScroll(int index, bool bSel)
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

void CExListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl::OnLButtonDown(nFlags, point);

	int index{};
	int colNum = 0;
	
	if ((index = hitTestEx(point, &colNum)) != -1)
	{
		if (m_bEditEnable)
		{
			LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
			if (m_disEditCol == colNum)
			{
				style &= ~LVS_EDITLABELS;
				SetWindowLong(m_hWnd, GWL_STYLE, style);
				return;
			}
			else
			{
				style |= LVS_EDITLABELS;
				SetWindowLong(m_hWnd, GWL_STYLE, style);
				EditSubItem(index, colNum);
			}
		}
	}
}

void CExListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (GetFocus() != this) 
		SetFocus();
    
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CExListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (GetFocus() != this) 
		SetFocus();

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CExListCtrl::hitTestEx(const CPoint& Point, int* pColumn)
{
	int row = HitTest(Point, NULL);

	if (pColumn)
		*pColumn = 0;

	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;
  
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if (bottom > GetItemCount())
		bottom = GetItemCount();

	/**
	if (!m_Header)
		m_Header.SubclassDlgItem(0, this);

	int nColCount = m_Header.GetItemCount();
	**/

	const int nColCount = GetHeaderCtrl()->GetItemCount();

	CRect rect;
	int colNum = 0;
	int colWidth = 0;

	for(; row <= bottom; row++)
	{
		GetItemRect(row, &rect, LVIR_BOUNDS);
		if (rect.PtInRect(Point))
		{
			for (colNum = 0; colNum < nColCount; colNum++)
			{
				colWidth = GetColumnWidth(colNum);
				if (Point.x >= rect.left && Point.x <= (rect.left + colWidth))
				{
					if (pColumn)
						*pColumn = colNum;
					return row;
				}
				rect.left += colWidth;
			}
		}
	}
	return -1;
}

CEdit* CExListCtrl::EditSubItem(int item, int col)
{
	//**int nColCnt = m_Header.GetItemCount();
	const int nColCnt = GetHeaderCtrl()->GetItemCount();
	if (col >= nColCnt || GetColumnWidth(col) < 5)
		return (CEdit *)NULL;

	int offset = 0;
	for (int ii = 0; ii < col; ii++)
		offset += GetColumnWidth(ii);

	CRect iRect, cRect;
	GetItemRect(item, &iRect, LVIR_BOUNDS);
	GetClientRect (&cRect);

	CSize size;
	if (offset + iRect.left < 0 || offset + iRect.left > cRect.right)
	{
		if (offset + iRect.left > 0)
			size.cx = -(offset - iRect.left);
		else
			size.cx = offset - iRect.left;
		size.cy = 0;
		Scroll(size);
		iRect.left -= size.cx;
	}

	LV_COLUMN lvCol;
	lvCol.mask = LVCF_FMT;
	GetColumn(col, &lvCol);
	DWORD dwStyle{};
	if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else 
		dwStyle = ES_CENTER;

	iRect.left += offset+4;
	iRect.right = iRect.left + GetColumnWidth(col) - 3;
	if (iRect.right > cRect.right)
		iRect.right = cRect.right;
	
	dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	DeleteEdit();
	m_pEdit = new CListEdit(this, item, col, GetItemText(item, col));
	m_pEdit->SetOwner(GetOwner());
	m_pEdit->Create(dwStyle, iRect, this, IDC_EDITCELL);
	SetEditState(true);
	int limit = 9;
	switch (col)
	{
	case 1:
		limit = 12;
		break;
	case 2:
	case 3:
		limit = 9;
		break;
	}
	m_pEdit->SetLimitText(limit);
	return m_pEdit;
}

void CExListCtrl::SetItemDataEx(int row, CString str)
{
	ListData* pData = new ListData(str);
	m_Ptr.Add(pData);
	SetItemData(row, (DWORD)m_Ptr[m_Ptr.GetSize()-1]);
}

void CExListCtrl::GetItemDataEx(int row, CString& str)
{
	const ListData* pData = (ListData *)GetItemData(row);
	str = pData->m_sData;
}

void CExListCtrl::FreePtr()
{
	for (int ii = 0; ii < m_Ptr.GetSize(); ii++)
	{
		ListData* pData = (ListData *)m_Ptr.GetAt(ii);
		delete pData;
	}
	m_Ptr.RemoveAll();
}

void CExListCtrl::SetSingleSelect()
{
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE); 
	dwStyle |= LVS_SINGLESEL;
 	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
}

void CExListCtrl::SetMultiSelect()
{
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE); 
	dwStyle &= ~(LVS_SINGLESEL);
 	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT);
}

void CExListCtrl::SetItemStateEx(int row, bool bSel)
{
	if (bSel)
		SetItemState(row, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	else
		SetItemState(row, 0, LVIS_SELECTED);
}

bool CExListCtrl::GetEditState()
{
	return m_bIsEdit;
}

void CExListCtrl::SetEditState(bool bIsEdit)
{
	m_bIsEdit = bIsEdit;
}

void CExListCtrl::DeleteEdit()
{
	if (m_pEdit)
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}
}

void CExListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	*pResult = 0;
}


BOOL CExListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (GetFocus() != this) 
		SetFocus();
    	
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}
