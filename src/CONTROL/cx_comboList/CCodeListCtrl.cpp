// CCodeListCtrl.cpp: 구현 파일
//

#include "stdafx.h"
#include "CCodeListCtrl.h"
#include <afxtempl.h>

// CCodeListCtrl

// Below styles MUST be present in a report ctrl
#define MUST_STYLE			(WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LVS_REPORT)
#define MUST_EX_STYLE		(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES)

// Below styles MUST NOT be present in a report ctrl
#define MUST_NOT_STYLE		(LVS_EDITLABELS | LVS_ICON | LVS_SMALLICON | LVS_LIST | LVS_NOSCROLL)
#define MUST_NOT_EX_STYLE	(0)

#define COL_CODE		1    //종목코드
#define COL_NAME		2   //종목명

struct ROWINFO
{
	DWORD dwData{};
	DWORD dwStates{};
	CArray<int, int> aImages;
	CStringArray aTexts;
};

//////////////////////////////////////////////////////////////////////////
// CItemData class is used for store extra information
//////////////////////////////////////////////////////////////////////////
class CItemData
{
public:
	CItemData() { dwData = 0; }
	void InsertColumn(int nColumn);
	void DeleteColumn(int nColumn);
	DWORD dwData; // The actual 32-bit user data stores here
	CArray<COLORREF, COLORREF> aTextColors; // Sub item text colors
	CArray<COLORREF, COLORREF> aBkColors; // Sub item backgroud colors
};

void CItemData::InsertColumn(int nColumn)
{
	aTextColors.InsertAt(nColumn, ::GetSysColor(COLOR_WINDOWTEXT));
	aBkColors.InsertAt(nColumn, ::GetSysColor(COLOR_WINDOW));
}

void CItemData::DeleteColumn(int nColumn)
{
	aTextColors.RemoveAt(nColumn);
	aBkColors.RemoveAt(nColumn);
}

///////////////////////////////////////////////////////////////////////
// A set of functions used for item text format determining
///////////////////////////////////////////////////////////////////////
namespace _ITEM_COMPARE_FUNCS
{
	BOOL _IsDecNumber(const CString& str, double& f);				//문자열이 십진수인지 확인
	int _DecNumberCompare(double f1, double f2);						//두 십진수 비교
	BOOL _IsHexNumber(const CString& str, DWORD& dw);			//문자열이 16진수인지 확인
	int _HexNumberCompare(DWORD dw1, DWORD dw2);			//두 16진수 비교
	BOOL _IsDate(const CString& str, COleDateTime& date);			//문자열이 날짜인지 확인
	int _DateCompare(const COleDateTime& date1, const COleDateTime& date2);  //두 날짜 비교
};

BOOL _ITEM_COMPARE_FUNCS::_IsDecNumber(const CString& str, double& f)
{
	if (str.IsEmpty())
		return FALSE;

	LPTSTR p;
	f = _tcstod(str, &p);
	return (*p == _T('\0') || (*p == _T('%') && p[1] == _T('\0')));
}

int _ITEM_COMPARE_FUNCS::_DecNumberCompare(double f1, double f2)
{
	if (f1 < f2)
		return -1;

	if (f1 > f2)
		return 1;

	return 0;
}

BOOL _ITEM_COMPARE_FUNCS::_IsHexNumber(const CString& str, DWORD& dw)
{
	if (str.IsEmpty())
		return FALSE;

	LPTSTR p;
	dw = _tcstoul(str, &p, 16);
	return *p == _T('\0');
}

int _ITEM_COMPARE_FUNCS::_HexNumberCompare(DWORD dw1, DWORD dw2)
{
	if (dw1 > dw2)
		return 1;

	if (dw1 < dw2)
		return -1;

	return 0;
}

BOOL _ITEM_COMPARE_FUNCS::_IsDate(const CString& str, COleDateTime& date)
{
	return date.ParseDateTime(str);
}

int _ITEM_COMPARE_FUNCS::_DateCompare(const COleDateTime& date1, const COleDateTime& date2)
{
	if (date1 < date2)
		return -1;

	if (date1 > date2)
		return 1;

	return 0;
}

IMPLEMENT_DYNAMIC(CCodeListCtrl, CListCtrl)

CCodeListCtrl::CCodeListCtrl(CWnd* pWizard, CString sItems) : m_pWndEdit(nullptr)
{
	m_pWndEdit = std::make_unique<CEdit>();
	VERIFY(m_pWndEdit != NULL);
	m_pszSeparator = NULL;
	m_bAllowEdit = FALSE;
	m_ptEditting.x = -1;
	m_ptEditting.y = -1;
	m_nChkStyle = RC_CHKBOX_NORMAL;
	m_dwPrevEditFmt = ES_LEFT;
	m_nSortCol = -1;
	m_bSortAscending = TRUE;
	m_res = 0;
	if (pWizard)
		m_pWizard = pWizard;
	m_items = sItems;
}

CCodeListCtrl::~CCodeListCtrl()
{
	if (m_pszSeparator != NULL)
		delete[] m_pszSeparator;
}

BEGIN_MESSAGE_MAP(CCodeListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(NM_CLICK, &CCodeListCtrl::OnNMClick)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
//	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CCodeListCtrl 메시지 처리기
void CCodeListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	else if (lplvcd->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		//const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(lplvcd->nmcd.dwItemSpec));
		//ASSERT(p != NULL);
	/*	ASSERT(lplvcd->iSubItem >= 0 && lplvcd->iSubItem < p->aTextColors.GetSize());
		lplvcd->clrText = p->aTextColors[lplvcd->iSubItem];
		lplvcd->clrTextBk = p->aBkColors[lplvcd->iSubItem];*/



		//////////////////////////////
		int nItem = (int)lplvcd->nmcd.dwItemSpec;
		int nSubItem = lplvcd->iSubItem;

		const gsl::not_null<CItemData*> p =
			(CItemData*)(CListCtrl::GetItemData(nItem));

		ASSERT(nSubItem >= 0 && nSubItem < p->aTextColors.GetSize());
		lplvcd->clrText = p->aTextColors[nSubItem];
		lplvcd->clrTextBk = p->aBkColors[nSubItem];

		// 컬럼2 + 호버 행 → PNG 그리기
		if (nSubItem == 2 && nItem == m_nHoverItem
			&& !m_imgClose.IsNull())
		{
			CDC* pDC = CDC::FromHandle(lplvcd->nmcd.hdc);

			// 컬럼2 영역
			CRect rcItem;
			GetSubItemRect(nItem, 2, LVIR_BOUNDS, rcItem);

			// 이미지 크기 = 행 높이에 맞게
			int nSize = rcItem.Height() - 4;
			int nX = rcItem.left + (rcItem.Width() - nSize) / 2;
			int nY = rcItem.top + 2;

			// PNG 그리기 (알파채널 적용)
			m_imgClose.Draw(pDC->m_hDC,
				nX, nY, nSize, nSize,
				0, 0,
				m_imgClose.GetWidth(),
				m_imgClose.GetHeight());

			*pResult = CDRF_SKIPDEFAULT;
			return;
		}

		*pResult = CDRF_DODEFAULT;


		///////////////////////////
	
	}
}

void CCodeListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	const NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int COL = pNMListView->iSubItem;
	SortItems(COL, COL == m_nSortCol ? !m_bSortAscending : TRUE);
	*pResult = 0; //0을 입력하면 이벤트가 정상이라고 시스템에 알린다.
}

void CCodeListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	_MouseClkMonitor(WM_LBUTTONDOWN, nFlags, point, TRUE);
}

void CCodeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	_MouseClkMonitor(WM_LBUTTONDBLCLK, nFlags, point, TRUE);

	LVHITTESTINFO hitInfo;
	hitInfo.pt = point;
	SubItemHitTest(&hitInfo);

	int nItem = hitInfo.iItem;
	int nSubItem = hitInfo.iSubItem;

	// 종목코드(0) or 종목명(1) 컬럼만
	if (nItem >= 0 && (nSubItem == 0 || nSubItem == 1))
	{
		CString scode = GetItemText(nItem, COL_CODE);
		CString sname = GetItemText(nItem, COL_NAME);

		CString stmp;
		stmp.Format("%s\t%s", scode, sname);

		// 기존 방식과 동일하게 WM_USER + WPARAM 으로
		GetParent()->SendMessage(WM_USER,
			MAKEWPARAM(MSG_DBL_CLICK, 0),
			(LPARAM)(LPSTR)(LPCTSTR)stmp);
	}
}

void CCodeListCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{
	_MouseClkMonitor(WM_MBUTTONDOWN, nFlags, point, FALSE);
}

void CCodeListCtrl::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	_MouseClkMonitor(WM_MBUTTONDBLCLK, nFlags, point, FALSE);
}

void CCodeListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	_MouseClkMonitor(WM_RBUTTONDOWN, nFlags, point, FALSE);
}

void CCodeListCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	_MouseClkMonitor(WM_RBUTTONDBLCLK, nFlags, point, FALSE);
}

int CCodeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style &= ~MUST_NOT_STYLE;
	lpCreateStruct->style |= MUST_STYLE;
	if(CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetExtendedStyle(GetExtendedStyle());
	ASSERT(GetHeaderCtrl() != NULL);

	//CString text{};
	//text = m_items;
	//for (int idx; (idx = text.Find('\t')) != -1; )
	//{
	//	item = text.Left(idx++);
	//	text = text.Mid(idx);
	//	AddString(item);
	//	size = dc->GetOutputTextExtent(item);
	//	width = max(width, size.cx);
	//}

	return 0;
}

void CCodeListCtrl::OnDestroy()
{
	DeleteAllItems();
	m_pWndEdit->DestroyWindow();
	m_imgList.DeleteImageList();
	m_headerImgList.DeleteImageList();
	CListCtrl::OnDestroy();
}

BOOL CCodeListCtrl::_IsValidIndex(int nIndex) const
{
	return nIndex >= 0 && nIndex < CListCtrl::GetItemCount();
}

BOOL CCodeListCtrl::SetColumnHeader(const CString& strHeadings)
{
	DeleteAllItems();
	DeleteAllColumns();
	EndEdit(TRUE);
	BOOL bInserted = FALSE;
	CStringArray aLong, aShort;
	_StringSplit(strHeadings, aLong, _T(';'));

	for (int i = 0; i < aLong.GetSize(); i++)
	{
		_StringSplit(aLong[i], aShort, _T(','));
		if (aShort.GetSize() > 0)
		{
			const int WIDTH = aShort.GetSize() > 1 ? _ttoi(aShort[1]) : 100;
			int nFormat = aShort.GetSize() > 2 ? _ttoi(aShort[2]) : 0;

			if (nFormat == 1)
				nFormat = LVCFMT_CENTER;
			else if (nFormat == 2)
				nFormat = LVCFMT_RIGHT;
			else
				nFormat = LVCFMT_LEFT;

			bInserted |= (InsertColumn(GetColumnCount(), aShort[0], nFormat, WIDTH) >= 0);
		}
	}
	return bInserted;
}

BOOL CCodeListCtrl::DeleteItem(int nItem, BOOL bSelectNextItem)
{
	EndEdit(m_ptEditting.x != nItem);
	if (bSelectNextItem)
		SetItemStates(nItem + 1, RC_ITEM_SELECTED);

	_FreeItemMemory(nItem);
	return CListCtrl::DeleteItem(nItem);
}

int CCodeListCtrl::DeleteAllItems(DWORD dwStates)
{
	EndEdit(FALSE);
	int nItemCount = CListCtrl::GetItemCount();
	if (dwStates & RC_ITEM_ALL)
	{
		LockWindowUpdate();
		for (int i = 0; i < nItemCount; i++)
			_FreeItemMemory(i);
		CListCtrl::DeleteAllItems();
		UnlockWindowUpdate();
		return nItemCount;
	}

	int nDelCount = 0;
	LockWindowUpdate();
	for (int i = 0; i < nItemCount; i++)
	{
		if (ExamItemStates(i, dwStates))
		{
			DeleteItem(i--);
			nItemCount--;
			nDelCount++;
		}
	}
	UnlockWindowUpdate();
	return nDelCount;
}

void CCodeListCtrl::SortItems(int nColumn, BOOL bAscending)
{
	EndEdit(TRUE);
	if (nColumn < 0 || nColumn >= GetColumnCount() || !IsSortable())
		return;

	// do the sorting	
	m_nSortCol = nColumn;
	m_bSortAscending = bAscending;

	BOOL bEnd = FALSE;
	int nSep1 = -1;
	int nSep2 = _FindSeparator(-1, nColumn);
	do
	{
		if (nSep2 < 0)
		{
			nSep2 = GetItemCount();
			bEnd = TRUE;
		}
		_PartialSort(nSep1 + 1, nSep2 - 1);
		nSep1 = _FindSeparator(nSep2 - 1, nColumn);
		nSep2 = _FindSeparator(nSep1, nColumn);
	} while (!bEnd && nSep1 >= 0);

	GetParent()->SendMessage(WM_ITEM_SORTED, (WPARAM)m_nSortCol, (LPARAM)m_bSortAscending);
}

BOOL CCodeListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpText)
{
	EndEdit(TRUE);
	_UnsetSortedColumn();
	return CListCtrl::SetItemText(nItem, nSubItem, lpText);
}

int CCodeListCtrl::GetColumnCount() const
{
	return GetHeaderCtrl()->GetItemCount();
}

int CCodeListCtrl::InsertItem(const LVITEM* pItem)
{
	EndEdit(TRUE);
	_UnsetSortedColumn();
	const int IDX = CListCtrl::InsertItem(pItem);
	if (IDX >= 0)
		_AllocItemMemory(IDX);
	return IDX;
}

int CCodeListCtrl::InsertItem(int nIndex, LPCTSTR lpText)
{
	EndEdit(TRUE);
	_UnsetSortedColumn();
	const int IDX = CListCtrl::InsertItem(nIndex, lpText);
	if (IDX >= 0)
		_AllocItemMemory(IDX);
	return IDX;
}

int CCodeListCtrl::InsertItem(UINT nMask, int nItem, LPCTSTR lpItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
{
	EndEdit(TRUE);
	_UnsetSortedColumn();
	const int IDX = CListCtrl::InsertItem(nMask, nItem, lpItem, nState, nStateMask, nImage, lParam);
	if (IDX >= 0)
		_AllocItemMemory(IDX);
	return IDX;
}

void CCodeListCtrl::SetGridLines(BOOL bSet)
{
	DWORD dwStype = GetExtendedStyle();
	if (bSet)
		dwStype |= LVS_EX_GRIDLINES;
	else
		dwStype &= ~LVS_EX_GRIDLINES;

	SetExtendedStyle(dwStype);
}

int CCodeListCtrl::InsertColumn(int nCol, const LVCOLUMN* pColumn)
{
	EndEdit(TRUE);
	const int IDX = CListCtrl::InsertColumn(nCol, pColumn);
	if (IDX >= 0)
		_UpdateColumn(IDX, TRUE);
	return IDX;
}

int CCodeListCtrl::InsertColumn(int nCol, LPCTSTR lpColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	EndEdit(TRUE);
	const int IDX = CListCtrl::InsertColumn(nCol, lpColumnHeading, nFormat, nWidth, nSubItem);
	if (IDX >= 0)
		_UpdateColumn(IDX, TRUE);
	return IDX;
}

BOOL CCodeListCtrl::DeleteColumn(int nCol)
{
	EndEdit(TRUE);
	const BOOL RES = CListCtrl::DeleteColumn(nCol);
	if (RES)
		_UpdateColumn(nCol, FALSE);
	return RES;
}

void CCodeListCtrl::SetCheckboxeStyle(int nStyle)
{
	m_nChkStyle = nStyle;
	EndEdit(TRUE);
	DWORD dwStype = GetExtendedStyle();
	if (nStyle == 0)
	{
		dwStype &= ~LVS_EX_CHECKBOXES;
	}
	else
	{
		dwStype |= LVS_EX_CHECKBOXES;
		_EnsureSingleCheck(-1);
	}

	SetExtendedStyle(dwStype);
}

int CCodeListCtrl::MoveTo(int nItem, int nNewPosition)
{
	if (!_IsValidIndex(nItem))
		return -1;

	EndEdit(TRUE);
	const int ITEMS = CListCtrl::GetItemCount();
	nNewPosition = max(0, nNewPosition);
	nNewPosition = min(ITEMS - 1, nNewPosition);

	if (nItem == nNewPosition)
		return nNewPosition;

	_UnsetSortedColumn();

	// Backup all states and attributes
	const int COLS = GetColumnCount();
	const DWORD STATES = GetItemStates(nItem);
	const DWORD DATA = CListCtrl::GetItemData(nItem);
	CArray<int, int> aImages;
	CStringArray aTexts;
	aImages.SetSize(COLS);
	aTexts.SetSize(COLS);
	for (int i = 0; i < COLS; i++)
	{
		aImages[i] = GetItemImage(nItem, i);
		aTexts[i] = GetItemText(nItem, i);
	}

	// Delete the item
	CListCtrl::DeleteItem(nItem);

	// Insert a new item to the new position
	const int IDX = CListCtrl::InsertItem(nNewPosition, "");

	// Restore all states & attributes to the newly inserted item
	for (int j = 0; j < COLS; j++)
	{
		CListCtrl::SetItemText(IDX, j, aTexts[j]);
		SetItemImage(IDX, j, aImages[j]);
	}
	CListCtrl::SetItemData(IDX, DATA);
	SetItemStates(IDX, STATES);
	return IDX;
}

int CCodeListCtrl::MoveUp(int nItem, int nCount)
{
	return MoveTo(nItem, nItem - nCount);
}

BOOL CCodeListCtrl::MoveDown(int nItem, int nCount)
{
	return MoveTo(nItem, nItem + nCount);
}

BOOL CCodeListCtrl::SwapItems(int nItem1, int nItem2)
{
	EndEdit(TRUE);
	if (!_IsValidIndex(nItem1) || !_IsValidIndex(nItem2))
		return FALSE;

	if (nItem1 == nItem2)
		return TRUE;

	_UnsetSortedColumn();

	// record previous states first
	const DWORD STATES1 = GetItemStates(nItem1);
	const DWORD STATES2 = GetItemStates(nItem2);
	const DWORD DATA1 = CListCtrl::GetItemData(nItem1);
	const DWORD DATA2 = CListCtrl::GetItemData(nItem2);

	// swap item texts and images
	for (int i = 0; i < GetColumnCount(); i++)
	{
		CString str = GetItemText(nItem1, i);
		CListCtrl::SetItemText(nItem1, i, GetItemText(nItem2, i));
		CListCtrl::SetItemText(nItem2, i, str);
		const UINT nImg = GetItemImage(nItem1, i);
		SetItemImage(nItem1, i, GetItemImage(nItem2, i));
		SetItemImage(nItem2, i, nImg);
	}

	// swap item data
	CListCtrl::SetItemData(nItem1, DATA2);
	CListCtrl::SetItemData(nItem2, DATA1);

	// restore states
	SetItemStates(nItem1, STATES2);
	SetItemStates(nItem2, STATES1);

	return TRUE;
}

int CCodeListCtrl::MoveToTop(int nItem)
{
	return MoveTo(nItem, -1);
}

int CCodeListCtrl::MoveToBottom(int nItem)
{
	return MoveTo(nItem, INT_MAX);
}

BOOL CCodeListCtrl::SetItemText(int nItem, int nSubItem, TCHAR val)
{
	return SetItemText(nItem, nSubItem, CString(val));
}

BOOL CCodeListCtrl::SetItemText(int nItem, int nSubItem, INT val)
{
	return SetItemText(nItem, nSubItem, (LONG)val);
}

BOOL CCodeListCtrl::SetItemText(int nItem, int nSubItem, UINT val)
{
	return SetItemText(nItem, nSubItem, (ULONG)val);
}

BOOL CCodeListCtrl::SetItemText(int nItem, int nSubItem, DOUBLE val, int nPrecision)
{
	CString sText, sFmt;

	if (nPrecision >= 0)
	{
		sFmt.Format(_T("%%.%df"), nPrecision);
		sText.Format(sFmt, val);
	}
	else
	{
		sText.Format(_T("%f"), val);
		sText.TrimRight(_T('0'));
		sText.TrimRight(_T('.'));
	}

	return SetItemText(nItem, nSubItem, sText);
}

BOOL CCodeListCtrl::SetItemText(int nItem, int nSubItem, const COleDateTime& dateTime, DWORD dwFlags)
{
	return SetItemText(nItem, nSubItem, dateTime.GetStatus() == COleDateTime::valid ? dateTime.Format(dwFlags) : _T(""));
}

BOOL CCodeListCtrl::SetItemText(int nItem, int nSubItem, LONG val)
{
	CString str;
	str.Format(_T("%d"), val);
	return SetItemText(nItem, nSubItem, str);
}

BOOL CCodeListCtrl::SetItemText(int nItem, int nSubItem, ULONG val)
{
	CString str;
	str.Format(_T("%u"), val);
	return SetItemText(nItem, nSubItem, str);
}

BOOL CCodeListCtrl::DeleteAllColumns()
{
	while (GetColumnCount() > 0)
		DeleteColumn(0);
	return TRUE;
}

LPCTSTR CCodeListCtrl::GetSortSeparator() const
{
	return m_pszSeparator;
}

void CCodeListCtrl::SetSortSeparator(LPCTSTR lpSortSeparator)
{
	if (m_pszSeparator != NULL)
	{
		delete[] m_pszSeparator;
		m_pszSeparator = NULL;
	}

	if (lpSortSeparator != NULL)
		m_pszSeparator = _tcsdup(lpSortSeparator);
}

int CCodeListCtrl::InsertItemEx(int nItem, LPCTSTR lpText, ...)
{
	EndEdit(TRUE);
	_UnsetSortedColumn();
	const int IDX = CListCtrl::InsertItem(nItem, lpText);
	if (IDX < 0)
		return -1;

	va_list list;
	va_start(list, lpText);

	for (int iColumn = 1; iColumn < GetColumnCount(); iColumn++)
	{
		LPCTSTR lp = va_arg(list, LPCTSTR);
		if (lp != NULL)
		{
			CListCtrl::SetItemText(IDX, iColumn, lp);
		}
		else
		{
			break;
		}
	}

	va_end(list);

	if (IDX >= 0)
		_AllocItemMemory(IDX);

	return IDX;
}

BOOL CCodeListCtrl::_IsEditVisible() const
{
	return m_bAllowEdit
		&& m_ptEditting.x >= 0 && m_ptEditting.x < CListCtrl::GetItemCount()
		&& m_ptEditting.y >= 0 && m_ptEditting.y < GetColumnCount();
}

void CCodeListCtrl::SetEditable(BOOL bSet)
{
	if (!bSet)
		EndEdit(TRUE);
	m_bAllowEdit = bSet;
}

BOOL CCodeListCtrl::IsEditable() const
{
	return m_bAllowEdit;
}

BOOL CCodeListCtrl::_ItemCheckMonitor(int nIndex, BOOL bBefore, BOOL bAfter, UINT nMsg)
{
	if (!_IsValidIndex(nIndex)
		|| m_nChkStyle == RC_CHKBOX_NONE
		|| (bBefore && bAfter)
		|| (!bBefore && !bAfter))
	{
		return FALSE;
	}

	if (m_nChkStyle == RC_CHKBOX_SINGLE)
	{
		// Only one item can be checked
		if (!bBefore && bAfter)
		{
			SetAllItemStates(RC_ITEM_CHECKED, RC_ITEM_UNCHECKED); // uncheck all
			SetItemStates(nIndex, RC_ITEM_CHECKED);
		}
	}
	else if (m_nChkStyle == RC_CHKBOX_DISABLED)
	{
		// Cannot change the item checked/unchecked states by mouse clicks
		SetItemStates(nIndex, bBefore ? RC_ITEM_CHECKED : RC_ITEM_UNCHECKED);
		return TRUE;
	}

	SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED); // unselect all
	SetItemStates(nIndex, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
	GetParent()->SendMessage(WM_ON_CHKBOX, (WPARAM)nIndex, (LPARAM)nMsg);
	return TRUE;
}

int CCodeListCtrl::GetCheckboxStyle() const
{
	return m_nChkStyle;
}

void CCodeListCtrl::_StringSplit(const CString& str, CStringArray& arr, TCHAR chDelimitior)
{
	int nStart = 0, nEnd = 0;
	arr.RemoveAll();

	while (nEnd < str.GetLength())
	{
		// determine the paragraph ("xxx,xxx,xxx;")
		nEnd = str.Find(chDelimitior, nStart);
		if (nEnd == -1)
		{
			// reached the end of string
			nEnd = str.GetLength();
		}

		CString s = str.Mid(nStart, nEnd - nStart);
		if (!s.IsEmpty())
			arr.Add(s);

		nStart = nEnd + 1;
	}
}

BOOL CCodeListCtrl::SetSortable(BOOL bSet)
{
	if (!HasColumnHeader())
		return FALSE;

	LONG lStyle = ::GetWindowLong(GetHeaderCtrl()->GetSafeHwnd(), GWL_STYLE);
	if (bSet)
	{
		lStyle |= HDS_BUTTONS;
	}
	else
	{
		lStyle &= ~HDS_BUTTONS;
		_UnsetSortedColumn();
	}
	::SetWindowLong(GetHeaderCtrl()->GetSafeHwnd(), GWL_STYLE, lStyle);
	CListCtrl::GetHeaderCtrl()->RedrawWindow();
	return TRUE;
}

DWORD CCodeListCtrl::SetExtendedStyle(DWORD dwNewStyle)
{
	dwNewStyle &= ~MUST_NOT_EX_STYLE;
	dwNewStyle |= MUST_EX_STYLE;
	return CListCtrl::SetExtendedStyle(dwNewStyle);
}

BOOL CCodeListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~MUST_NOT_STYLE;
	cs.style |= MUST_STYLE;
	return CListCtrl::PreCreateWindow(cs);
}

BOOL CCodeListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (!_IsEditVisible())
		{
			// disable user from check/uncheck the checkboxes using space key
			// things get nasty if the user is HOLDING the space bar down
			if (pMsg->wParam == VK_SPACE)
				return TRUE;
		}
		else
		{
			POINT pt = m_ptEditting;
			switch (pMsg->wParam)
			{
			case VK_ESCAPE: // Cancel edit
				EndEdit(FALSE);
				return TRUE;

			case VK_RETURN: // Commit edit
				EndEdit(TRUE);
				return TRUE;

			case VK_TAB: // switch edit sub items
				if (pt.y == GetColumnCount() - 1)
					pt.y = 0;
				else
					pt.y++;
				EndEdit(TRUE);
				StartEdit(pt.x, pt.y);
				return TRUE;

			case VK_UP: // edit upper item
				if (pt.x > 0)
				{
					pt.x--;
					EndEdit(TRUE);
					StartEdit(pt.x, pt.y);
					EnsureVisible(pt.x, FALSE);
					return TRUE;
				}
				break;

			case VK_DOWN: // edit lower item
				if (pt.x < CListCtrl::GetItemCount() - 1)
				{
					pt.x++;
					EndEdit(TRUE);
					StartEdit(pt.x, pt.y);
					EnsureVisible(pt.x, FALSE);
					return TRUE;
				}
				break;

			default:
				break;
			}
		}
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

CEdit* CCodeListCtrl::GetEditControl()
{
	return m_pWndEdit.get();
}

BOOL CCodeListCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	ASSERT(pParentWnd != NULL);

	dwStyle &= ~MUST_NOT_STYLE;
	dwStyle |= MUST_STYLE;

	const gsl::not_null<CWnd*> pwnd = pParentWnd;
	
	return CListCtrl::Create(dwStyle, rect, pwnd, nID);
}

void CCodeListCtrl::PreSubclassWindow()
{
	LONG lStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	lStyle &= ~MUST_NOT_STYLE;
	lStyle |= MUST_STYLE;
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle);
	SetExtendedStyle(0);
	//	ASSERT(GetHeaderCtrl() != NULL);
	CListCtrl::PreSubclassWindow();
}

BOOL CCodeListCtrl::IsSortable() const
{
	if (!HasColumnHeader())
		return FALSE;

	const LONG lStyle = ::GetWindowLong(GetHeaderCtrl()->GetSafeHwnd(), GWL_STYLE);
	return (lStyle & HDS_BUTTONS) != 0;
}

BOOL CCodeListCtrl::HasColumnHeader() const
{
	return (GetStyle() & LVS_NOCOLUMNHEADER) == 0;
}

void CCodeListCtrl::ResizeToFitParent()
{
	// resize the list ctrl to fit parent client area
	if (GetSafeHwnd() != NULL)
	{
		CRect rect;
		GetParent()->GetClientRect(&rect);
		MoveWindow(&rect);
	}
}

BOOL CCodeListCtrl::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	dwRemove &= ~MUST_EX_STYLE;
	dwRemove |= MUST_NOT_EX_STYLE;
	dwAdd &= ~MUST_NOT_EX_STYLE;
	dwAdd |= MUST_EX_STYLE;
	return CListCtrl::ModifyStyleEx(dwRemove, dwAdd, nFlags);
}

BOOL CCodeListCtrl::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	dwRemove &= ~MUST_STYLE;
	dwRemove |= MUST_NOT_STYLE;
	dwAdd &= ~MUST_NOT_STYLE;
	dwAdd |= MUST_STYLE;
	return CListCtrl::ModifyStyle(dwRemove, dwAdd, nFlags);
}

void CCodeListCtrl::_MouseClkMonitor(UINT nMsg, UINT nFlags, CPoint point, BOOL bTriggerEdit)
{
	LVHITTESTINFO hti;
	hti.pt = point;
	const int IDX = SubItemHitTest(&hti);
	const BOOL BEFORE = CListCtrl::GetCheck(IDX) > 0;
	const BOOL WAS_EDIT = _IsEditVisible();
	EndEdit(TRUE);
	const BOOL WASACTIVE = bTriggerEdit ? ExamItemStates(IDX, RC_ITEM_FOCUSED | RC_ITEM_SELECTED) : FALSE;

	switch (nMsg)
	{
	case WM_LBUTTONDOWN:
		CListCtrl::OnLButtonDown(nFlags, point);
		break;

	case WM_LBUTTONDBLCLK:
		CListCtrl::OnLButtonDblClk(nFlags, point);
		break;

	case WM_MBUTTONDOWN:
		CListCtrl::OnMButtonDown(nFlags, point);
		break;

	case WM_MBUTTONDBLCLK:
		CListCtrl::OnMButtonDblClk(nFlags, point);
		break;

	case WM_RBUTTONDOWN:
		CListCtrl::OnRButtonDown(nFlags, point);
		break;

	case WM_RBUTTONDBLCLK:
		CListCtrl::OnRButtonDblClk(nFlags, point);
		break;

	default:
		break;
	}

	const BOOL STATSCHANGED = _ItemCheckMonitor(IDX, BEFORE, CListCtrl::GetCheck(IDX) > 0, nMsg);

	if (bTriggerEdit && m_bAllowEdit && !STATSCHANGED && !WAS_EDIT && WASACTIVE)
		StartEdit(IDX, hti.iSubItem);
}

BOOL CCodeListCtrl::SetItemImage(int nItem, int nSubItem, int nImageIndex)
{
	return CListCtrl::SetItem(nItem, nSubItem, LVIF_IMAGE, NULL, nImageIndex, 0, 0, 0);
}

int CCodeListCtrl::GetItemImage(int nItem, int nSubItem) const
{
	LVITEM lvi;
	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;
	lvi.mask = LVIF_IMAGE;
	return CListCtrl::GetItem(&lvi) ? lvi.iImage : -1;
}

CImageList* CCodeListCtrl::SetImageList(CImageList* pImageList)
{
	return CListCtrl::SetImageList(pImageList, LVSIL_SMALL);
}

CImageList* CCodeListCtrl::GetImageList() const
{
	return CListCtrl::GetImageList(LVSIL_SMALL);
}

CImageList* CCodeListCtrl::SetImageList(UINT nBitmapID, COLORREF crMask)
{
	m_imgList.DeleteImageList();
	m_imgList.Create(nBitmapID, 16, 4, crMask);
	return CListCtrl::SetImageList(&m_imgList, LVSIL_SMALL);
}

BOOL CCodeListCtrl::StartEdit(int nItem, int nSubItem)
{
	if (!m_bAllowEdit || !_IsValidIndex(nItem) || nSubItem < 0 || nSubItem >= GetColumnCount())
		return FALSE;

	if (m_ptEditting.x == nItem && m_ptEditting.y == nSubItem)
		return TRUE;

	EndEdit(TRUE);
	m_ptEditting.x = nItem;
	m_ptEditting.y = nSubItem;
	SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED); // unselect all
	SetItemStates(m_ptEditting.x, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);

	// determine editbox font and alignment
	const DWORD FMT = _GetHeaderTextFormat(nSubItem);
	if (FMT != m_dwPrevEditFmt)
	{
		m_dwPrevEditFmt = FMT;

		// Funny thing:
		// Changing CEdit style among ES_LEFT, ES_CENTER, ES_RIGHT at runtime works
		// sometimes and fails other times. It just cannot guarantee to be succeed.
		// So I decided to destroy and recreate the CEdit every time when the text
		// format changes.
		if (m_pWndEdit->GetSafeHwnd() != NULL)
			m_pWndEdit->DestroyWindow();

		if (!m_pWndEdit->Create(ES_AUTOHSCROLL | ES_NOHIDESEL | WS_CHILD | WS_BORDER | FMT, CRect(0, 0, 1, 1), this, 0))
			return FALSE;
	}
	else
	{
		if (m_pWndEdit->GetSafeHwnd() == NULL
			&& !m_pWndEdit->Create(ES_AUTOHSCROLL | ES_NOHIDESEL | WS_CHILD | WS_BORDER | FMT, CRect(0, 0, 1, 1), this, 0))
		{
			return FALSE;
		}
	}

	m_pWndEdit->SetFont(GetFont());

	CRect rcEdit;
	ListView_GetSubItemRect(GetSafeHwnd(), m_ptEditting.x, m_ptEditting.y, LVIR_LABEL, &rcEdit);
	if (m_ptEditting.y > 0 && GetImageList() != NULL && GetItemImage(m_ptEditting.x, m_ptEditting.y) >= 0)
		rcEdit.DeflateRect(16, 0, 0, 0);

	// Move the editbox to that grid, obtain text from the grid, display the
	// editbox, and, finally, highlights all text in the editbox and set the
	// windows focus to the editbox.
	m_pWndEdit->MoveWindow(&rcEdit);
	m_pWndEdit->SetWindowText(GetItemText(m_ptEditting.x, m_ptEditting.y));
	m_pWndEdit->ShowWindow(SW_SHOW);
	m_pWndEdit->SetSel(0, -1);
	m_pWndEdit->SetFocus();
	return TRUE;
}

BOOL CCodeListCtrl::EndEdit(BOOL bCommit)
{
	if (!_IsEditVisible())
		return FALSE;

	CString str;
	m_pWndEdit->GetWindowText(str);
	BOOL bChanged = bCommit && str.Compare(GetItemText(m_ptEditting.x, m_ptEditting.y)) != 0;
	if (bChanged)
	{
		CListCtrl::SetItemText(m_ptEditting.x, m_ptEditting.y, str);
		_UnsetSortedColumn();
		GetParent()->SendMessage(WM_EDIT_COMMITTED, (WPARAM)m_ptEditting.x, (LPARAM)m_ptEditting.y);
	}
	m_pWndEdit->ShowWindow(SW_HIDE);
	m_ptEditting.x = -1;
	m_ptEditting.y = -1;

	return bChanged;
}

BOOL CCodeListCtrl::SetHeaderImage(int nColumn, int nImageIndex, BOOL bLeftSide)
{
	if (GetHeaderCtrl()->GetImageList() == NULL)
		CListCtrl::GetHeaderCtrl()->SetImageList(GetImageList());

	HDITEM hi;
	::memset(&hi, 0, sizeof(HDITEM));
	hi.mask = HDI_FORMAT;
	if (!GetHeaderCtrl()->GetItem(nColumn, &hi))
		return FALSE;

	hi.mask |= HDI_IMAGE;
	hi.fmt |= HDF_IMAGE;

	if (!bLeftSide)
		hi.fmt |= HDF_BITMAP_ON_RIGHT; // draw the image on right side of text

	hi.iImage = nImageIndex;
	return CListCtrl::GetHeaderCtrl()->SetItem(nColumn, &hi);
}

int CCodeListCtrl::GetSortedColumn() const
{
	return m_nSortCol;
}

BOOL CCodeListCtrl::IsSortAscending() const
{
	return m_bSortAscending;
}

void CCodeListCtrl::_UnsetSortedColumn()
{
	m_nSortCol = -1;
	m_bSortAscending = TRUE;
}

DWORD CCodeListCtrl::_GetHeaderTextFormat(int nColumn) const
{
	if (!HasColumnHeader())
		return ES_LEFT;

	HDITEM hd;
	hd.mask = HDI_FORMAT;
	if (!GetHeaderCtrl()->GetItem(nColumn, &hd))
		return ES_LEFT;

	if (hd.fmt & HDF_CENTER)
	{
		return ES_CENTER;
	}
	else if (hd.fmt & HDF_RIGHT)
	{
		return ES_RIGHT;
	}
	else
	{
		return ES_LEFT;
	}
}

int CCodeListCtrl::GetHeaderImage(int nColumn) const
{
	HDITEM hi;
	::memset(&hi, 0, sizeof(HDITEM));
	hi.mask = HDI_IMAGE;
	return !GetHeaderCtrl()->GetItem(nColumn, &hi) ? hi.iImage : -1;
}

CImageList* CCodeListCtrl::SetHeaderImageList(CImageList* pImageList)
{
	return CListCtrl::GetHeaderCtrl()->SetImageList(pImageList);
}

CImageList* CCodeListCtrl::SetHeaderImageList(UINT nBitmapID, COLORREF crMask)
{
	m_headerImgList.Create(nBitmapID, 16, 4, crMask);
	return SetHeaderImageList(&m_headerImgList);
}

BOOL CCodeListCtrl::ExamItemStates(int nItem, DWORD dwStates) const
{
	if (dwStates == RC_ITEM_NONE)
		return FALSE;

	if (dwStates & RC_ITEM_ALL)
		return TRUE;

	return (GetItemStates(nItem) & dwStates) == dwStates;
}

int CCodeListCtrl::GetItemCount(DWORD dwStates) const
{
	const int ITEMS = CListCtrl::GetItemCount();
	if (dwStates == RC_ITEM_ALL)
		return ITEMS;

	int nCount = 0;
	for (int i = 0; i < ITEMS; i++)
	{
		if (ExamItemStates(i, dwStates))
			nCount++;
	}
	return nCount;
}

int CCodeListCtrl::GetFirstItem(DWORD dwStates, int nStartAfter) const
{
	const int ITEMS = CListCtrl::GetItemCount();
	nStartAfter = max(-1, nStartAfter);
	for (int i = nStartAfter + 1; i < ITEMS; i++)
	{
		if (ExamItemStates(i, dwStates))
			return i;
	}
	return -1;
}

int CCodeListCtrl::GetLastItem(DWORD dwStates, int nStartBefore) const
{
	const int ITEMS = CListCtrl::GetItemCount();
	nStartBefore = nStartBefore < 0 ? ITEMS - 1 : min(ITEMS, nStartBefore);
	for (int i = nStartBefore - 1; i >= 0; i--)
	{
		if (ExamItemStates(i, dwStates))
			return i;
	}
	return -1;
}

void CCodeListCtrl::InvertItems(int nType)
{
	const int ITEMS = CListCtrl::GetItemCount();
	if (nType == RC_INVERT_SELECTION)
	{
		for (int i = 0; i < ITEMS; i++)
			SetItemStates(i, ExamItemStates(i, RC_ITEM_SELECTED) ? RC_ITEM_UNSELECTED : RC_ITEM_SELECTED);
	}
	else if (nType == RC_INVERT_CHECKMARK)
	{
		for (int i = 0; i < ITEMS; i++)
			SetItemStates(i, ExamItemStates(i, RC_ITEM_CHECKED) ? RC_ITEM_UNCHECKED : RC_ITEM_CHECKED);
	}
}

DWORD CCodeListCtrl::GetItemStates(int nItem) const
{
	DWORD dwStates = 0;

	if (CListCtrl::GetItemState(nItem, LVIS_SELECTED))
		dwStates |= RC_ITEM_SELECTED;
	else
		dwStates |= RC_ITEM_UNSELECTED;

	if (CListCtrl::GetCheck(nItem) > 0)
		dwStates |= RC_ITEM_CHECKED;
	else
		dwStates |= RC_ITEM_UNCHECKED;

	if (CListCtrl::GetItemState(nItem, LVIS_FOCUSED))
		dwStates |= RC_ITEM_FOCUSED;
	else
		dwStates |= RC_ITEM_UNFOCUSED;

	return dwStates;
}

BOOL CCodeListCtrl::SetItemStates(int nItem, DWORD dwNewStates)
{
	const BOOL bResult = _SetItemStatesNoVerify(nItem, dwNewStates);
	if (dwNewStates & RC_ITEM_CHECKED)
		_EnsureSingleCheck(nItem);
	return bResult;
}

int CCodeListCtrl::SetAllItemStates(DWORD dwOldStates, DWORD dwNewStates)
{
	if (dwNewStates == RC_ITEM_NONE || dwOldStates == RC_ITEM_NONE)
		return 0;

	int nCount = 0;
	const int ITEMS = CListCtrl::GetItemCount();
	for (int i = 0; i < ITEMS; i++)
	{
		if (dwOldStates == RC_ITEM_ALL || ExamItemStates(i, dwOldStates))
		{
			SetItemStates(i, dwNewStates);
			nCount++;
		}
	}

	if (dwNewStates & RC_ITEM_CHECKED)
		_EnsureSingleCheck(-1);

	return nCount;
}

void CCodeListCtrl::_EnsureSingleCheck(int nItem)
{
	if (m_nChkStyle == RC_CHKBOX_SINGLE)
	{
		const int FIRST = nItem < 0 ? GetFirstItem(RC_ITEM_CHECKED) : nItem;
		if (FIRST >= 0)
		{
			const int ITEMS = CListCtrl::GetItemCount();
			for (int i = FIRST + 1; i < ITEMS; i++)
			{
				if (CListCtrl::GetCheck(i))
					CListCtrl::SetCheck(i, FALSE);
			}
		}
	}
}

BOOL CCodeListCtrl::_SetItemStatesNoVerify(int nItem, DWORD dwNewStates)
{
	if (dwNewStates == RC_ITEM_NONE)
		return FALSE;

	BOOL bResult = TRUE;

	if (dwNewStates & RC_ITEM_SELECTED)
		bResult &= CListCtrl::SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);

	if (dwNewStates & RC_ITEM_UNSELECTED)
		bResult &= CListCtrl::SetItemState(nItem, 0, LVIS_SELECTED);

	if (dwNewStates & RC_ITEM_CHECKED)
		bResult &= CListCtrl::SetCheck(nItem, TRUE);

	if (dwNewStates & RC_ITEM_UNCHECKED)
		bResult &= CListCtrl::SetCheck(nItem, FALSE);

	if (dwNewStates & RC_ITEM_FOCUSED)
		bResult &= CListCtrl::SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);

	if (dwNewStates & RC_ITEM_UNFOCUSED)
		bResult &= CListCtrl::SetItemState(nItem, 0, LVIS_FOCUSED);

	return bResult;
}

BOOL CCodeListCtrl::SetCheck(int nItem, BOOL bCheck)
{
	const BOOL RES = CListCtrl::SetCheck(nItem, bCheck);
	if (RES && bCheck)
		_EnsureSingleCheck(nItem);
	return RES;
}

CString CCodeListCtrl::GetHeaderText(int nColumn) const
{
	if (!HasColumnHeader())
		return _T("");

	HDITEM hd{};
	TCHAR  szBuffer[256] = _T("");
	hd.mask = HDI_TEXT;
	hd.pszText = szBuffer;
	hd.cchTextMax = 255;
	return GetHeaderCtrl()->GetItem(nColumn, &hd) ? hd.pszText : _T("");
}

BOOL CCodeListCtrl::SetHeaderText(int nColumn, LPCTSTR lpText)
{
	if (!HasColumnHeader())
		return FALSE;

	LPTSTR psz = NULL;
	if (lpText == NULL)
	{
		psz = new TCHAR[1];
		*psz = _T('\0');
	}
	else
	{
		psz = _tcsdup(lpText);
	}
	HDITEM hd;
	hd.mask = HDI_TEXT;
	hd.pszText = psz;
	hd.cchTextMax = _tcslen(psz);
	const BOOL RES = CListCtrl::GetHeaderCtrl()->SetItem(nColumn, &hd);
	delete[] psz;
	return RES;
}

const CHeaderCtrl* CCodeListCtrl::GetHeaderCtrl() const
{
	// Yes, CListCtrl already provides "GetHeaderCtrl", but not const.	
	// I desperately need a "const" version of "GetHeaderCtrl" because that's the
	// only way to make "GetColumnCount" const, which, in turn, is being called by
	// A LOT of const member functions. So if "GetHeaderCtrl" is not const, there will
	// be almost no const member function at all in this class. Terrible.
	return (const CHeaderCtrl*)(CWnd::FromHandle(ListView_GetHeader(GetSafeHwnd())));
}

void CCodeListCtrl::_FreeItemMemory(int nItem)
{
	CItemData* p = (CItemData*)(CListCtrl::GetItemData(nItem));
	if (p != NULL)
	{
		CListCtrl::SetItemData(nItem, 0);
		delete p;
	}
}

void CCodeListCtrl::_AllocItemMemory(int nItem)
{
	ASSERT(_IsValidIndex(nItem));
	const int COLS = GetColumnCount();
	ASSERT(COLS > 0);
	CItemData* pData = new CItemData;
	pData->dwData = CListCtrl::GetItemData(nItem);
	pData->aTextColors.SetSize(COLS);
	pData->aBkColors.SetSize(COLS);
	for (int i = 0; i < COLS; i++)
	{
		pData->aTextColors[i] = ::GetSysColor(COLOR_WINDOWTEXT);
		pData->aBkColors[i] = ::GetSysColor(COLOR_WINDOW);
	}
	CListCtrl::SetItemData(nItem, (DWORD)pData);
}

void CCodeListCtrl::_UpdateColumn(int nColumn, BOOL bInsert)
{
	const int ITEMS = GetItemCount();
	for (int i = 0; i < ITEMS; i++)
	{
		CItemData* p = (CItemData*)(CListCtrl::GetItemData(i));
		ASSERT(p != NULL);
		if (bInsert)
			p->InsertColumn(nColumn);
		else
			p->DeleteColumn(nColumn);
	}
}

BOOL CCodeListCtrl::SetItemData(int nItem, DWORD dwData)
{
	CItemData* p = (CItemData*)(CListCtrl::GetItemData(nItem));
	if (p == NULL)
		return FALSE;
	p->dwData = dwData;
	return CListCtrl::SetItemData(nItem, (DWORD)p);
}

DWORD CCodeListCtrl::GetItemData(int nItem) const
{
	const CItemData* p = (CItemData*)(CListCtrl::GetItemData(nItem));
	return p == NULL ? 0 : p->dwData;
}

BOOL CCodeListCtrl::GetItem(LVITEM* pItem) const
{
	const BOOL RES = CListCtrl::GetItem(pItem);
	if (RES && (pItem->mask & LVIF_PARAM) != 0)
	{
		const CItemData* p = (CItemData*)(pItem->lParam);
		if (p != NULL)
			pItem->lParam = p->dwData;
	}
	return RES;
}

BOOL CCodeListCtrl::SetItem(const LVITEM* pItem)
{
	if (pItem == NULL)
		return FALSE;

	LVITEM li;
	memcpy(&li, pItem, sizeof(LVITEM));
	if ((li.mask & LVIF_PARAM) != 0)
	{
		CItemData* p = (CItemData*)(CListCtrl::GetItemData(li.iItem));
		if (p != NULL)
		{
			p->dwData = li.lParam;
			li.lParam = (LPARAM)p;
		}
	}
	return CListCtrl::SetItem(&li);
}

BOOL CCodeListCtrl::SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem, int nImage, UINT nState, UINT nStateMask, LPARAM lParam)
{
	if ((nMask & LVIF_PARAM) != 0)
	{
		CItemData* p = (CItemData*)(CListCtrl::GetItemData(nItem));
		if (p != NULL)
		{
			p->dwData = lParam;
			lParam = (LPARAM)p;
		}
	}
	return CListCtrl::SetItem(nItem, nSubItem, nMask, lpszItem, nImage, nState, nStateMask, lParam);
}

void CCodeListCtrl::SetItemTextColor(int nItem, int nSubItem, COLORREF color, BOOL bRedraw)
{
	if (color == COLOR_INVALID)
		color = ::GetSysColor(COLOR_WINDOWTEXT);
	const int ROWS = GetItemCount();
	const int COLS = GetColumnCount();
	const BOOL bRowValid = nItem >= 0 && nItem < ROWS;
	const BOOL bColValid = nSubItem >= 0 && nSubItem < COLS;

	if (bRowValid && bColValid)
	{
		// apply to individual grid
		const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(nItem));
		p->aTextColors[nSubItem] = color;
	}
	else if (bRowValid && !bColValid)
	{
		// apply to whole row for the existing item
		const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(nItem));
		for (int i = 0; i < COLS; i++)
			p->aTextColors[i] = color;
	}
	else if (!bRowValid && bColValid)
	{
		// apply to whole column for all existing items
		for (int i = 0; i < ROWS; i++)
		{
			const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(i));
			p->aTextColors[nSubItem] = color;
		}
	}
	else
	{
		// apply to whole table for all existing items
		for (int i = 0; i < ROWS; i++)
		{
			const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(i));
			for (int j = 0; j < COLS; j++)
				p->aTextColors[j] = color;
		}
	}

	if (bRedraw)
		RedrawWindow();
}

void CCodeListCtrl::SetItemBkColor(int nItem, int nSubItem, COLORREF color, BOOL bRedraw)
{
	if (color == COLOR_INVALID)
		color = ::GetSysColor(COLOR_WINDOW);
	const int ROWS = GetItemCount();
	const int COLS = GetColumnCount();
	const BOOL bRowValid = nItem >= 0 && nItem < ROWS;
	const BOOL bColValid = nSubItem >= 0 && nSubItem < COLS;

	if (bRowValid && bColValid)
	{
		// apply to individual grid
		const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(nItem));
		p->aBkColors[nSubItem] = color;
	}
	else if (bRowValid && !bColValid)
	{
		// apply to whole row for the existing item
		const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(nItem));
		for (int i = 0; i < COLS; i++)
			p->aBkColors[i] = color;
	}
	else if (!bRowValid && bColValid)
	{
		// apply to whole column for all existing items
		for (int i = 0; i < ROWS; i++)
		{
			const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(i));
			p->aBkColors[nSubItem] = color;
		}
	}
	else
	{
		// apply to whole table for all existing items
		for (int i = 0; i < ROWS; i++)
		{
			const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(i));
			for (int j = 0; j < COLS; j++)
				p->aBkColors[j] = color;
		}
	}

	if (bRedraw)
		RedrawWindow();
}

COLORREF CCodeListCtrl::GetItemTextColor(int nItem, int nSubItem) const
{
	if (!_IsValidIndex(nItem) || nSubItem < 0 || nSubItem >= GetColumnCount())
		return COLOR_INVALID;
	const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(nItem));
	return p->aTextColors[nSubItem];
}

COLORREF CCodeListCtrl::GetItemBkColor(int nItem, int nSubItem) const
{
	if (!_IsValidIndex(nItem) || nSubItem < 0 || nSubItem >= GetColumnCount())
		return COLOR_INVALID;
	const gsl::not_null< CItemData*> p = (CItemData*)(CListCtrl::GetItemData(nItem));
	return p->aBkColors[nSubItem];
}

BOOL CCodeListCtrl::_PartialSort(int nStart, int nEnd)
{
	if (nStart >= nEnd || !_IsValidIndex(nStart) || !_IsValidIndex(nEnd))
		return FALSE;

	const int COUNT = nEnd - nStart + 1;
	int i = 0;
	int* aIndices = new int[COUNT];
	for (i = 0; i < COUNT; i++)
		aIndices[i] = nStart + i;

	_QuickSortRecursive(aIndices, 0, COUNT - 1);

	// rearrange items
	const int COLS = GetColumnCount();
	ROWINFO* aRows = new ROWINFO[COUNT];
	for (i = 0; i < COUNT; i++)
	{
		const int n = aIndices[i];
		aRows[i].dwStates = GetItemStates(aIndices[i]);
		aRows[i].dwData = CListCtrl::GetItemData(aIndices[i]);
		aRows[i].aImages.SetSize(COLS);
		aRows[i].aTexts.SetSize(COLS);
		for (int j = 0; j < COLS; j++)
		{
			aRows[i].aImages[j] = GetItemImage(aIndices[i], j);
			aRows[i].aTexts[j] = GetItemText(aIndices[i], j);
		}
	}

	for (i = 0; i < COUNT; i++)
	{
		SetItemStates(nStart + i, aRows[i].dwStates);
		CListCtrl::SetItemData(nStart + i, aRows[i].dwData);
		for (int j = 0; j < COLS; j++)
		{
			SetItemImage(nStart + i, j, aRows[i].aImages[j]);
			CListCtrl::SetItemText(nStart + i, j, aRows[i].aTexts[j]);
		}
	}
	delete[] aRows;
	delete[] aIndices;
	return TRUE;
}

int CCodeListCtrl::_CompareItems(int nItem1, int nItem2)
{
	const CString s1 = GetItemText(nItem1, m_nSortCol);
	const CString s2 = GetItemText(nItem2, m_nSortCol);

	DWORD dw1{}, dw2{};
	if (_ITEM_COMPARE_FUNCS::_IsHexNumber(s1, dw1) && _ITEM_COMPARE_FUNCS::_IsHexNumber(s2, dw2))
		return _ITEM_COMPARE_FUNCS::_HexNumberCompare(dw1, dw2);

	double f1{}, f2{};
	if (_ITEM_COMPARE_FUNCS::_IsDecNumber(s1, f1) && _ITEM_COMPARE_FUNCS::_IsDecNumber(s2, f2))
		return _ITEM_COMPARE_FUNCS::_DecNumberCompare(f1, f2);

	COleDateTime date1, date2;
	if (_ITEM_COMPARE_FUNCS::_IsDate(s1, date1) && _ITEM_COMPARE_FUNCS::_IsDate(s2, date2))
		return _ITEM_COMPARE_FUNCS::_DateCompare(date1, date2);

	// plain text.
	return s1.Compare(s2);
}

int CCodeListCtrl::_FindSeparator(int nStartAfter, int nColumn) const
{
	if (m_pszSeparator == NULL)
		return -1;

	const int ITEMS = GetItemCount();
	for (int i = nStartAfter + 1; i < ITEMS; i++)
	{
		if (GetItemText(i, nColumn) == m_pszSeparator)
			return i;
	}
	return -1;
}

void CCodeListCtrl::_QuickSortRecursive(int* pArr, int nLow, int nHigh)
{
	int i = nHigh, j = nLow;
	const int n = pArr[(nLow + nHigh) / 2];
	do
	{
		if (m_bSortAscending)
		{
			while (_CompareItems(pArr[j], n) < 0)
				j++;

			while (_CompareItems(pArr[i], n) > 0)
				i--;
		}
		else
		{
			while (_CompareItems(pArr[j], n) > 0)
				j++;

			while (_CompareItems(pArr[i], n) < 0)
				i--;
		}

		if (i >= j)
		{
			if (i != j)
			{
				const int nTemp = pArr[i];
				pArr[i] = pArr[j];
				pArr[j] = nTemp;
			}

			i--;
			j++;
		}

	} while (j <= i);

	if (nLow < i)
		_QuickSortRecursive(pArr, nLow, i);

	if (j < nHigh)
		_QuickSortRecursive(pArr, j, nHigh);
}

void CCodeListCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CListCtrl::OnKillFocus(pNewWnd);
	//if(!m_bPopMenu)
	//	GetParent()->SendMessage(WM_CLOSE, 0, 0);
}


void CCodeListCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString stmp{};

	// 클릭한 좌표 얻기
	CPoint ptClick = pNMItemActivate->ptAction;

	// 리스트 컨트롤의 행 확인 (행 찾기)
	//CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_YOUR_LIST_CTRL);
	int nItem =HitTest(ptClick);  // 클릭한 행 인덱스
	if (nItem != -1) // 유효한 행을 클릭한 경우
	{
		// 열 확인
		LVHITTESTINFO hitTestInfo;
		hitTestInfo.pt = ptClick;
		SubItemHitTest(&hitTestInfo);

		int nSubItem = hitTestInfo.iSubItem;  // 클릭한 열 인덱스

		// 행(nItem)과 열(nSubItem)을 이용해 원하는 작업 수행
		CString strMessage;
		strMessage.Format(_T("\r\n\r\nClicked on Row: %d, Column: %d"), nItem, nSubItem);
		OutputDebugString(strMessage);
		if (nSubItem == 4)
		{
			CRect rect;
			GetItemRect(nItem, &rect, LVIR_BOUNDS);
			ClientToScreen(&rect);

			CPoint pt;
			GetCursorPos(&pt);
			//ScreenToClient(&pt);
			m_slog.Format("CCodeListCtrl  top=[%d]\t right=[%d]  pt.x=[%d]  pt.y=[%d]", rect.top, rect.right, pt.x, pt.y);
			OutputDebugString("\r\n" + m_slog);

			m_bPopMenu = TRUE;
			stmp.Format("%s\t%s\n%d\t%d", GetItemText(nItem, COL_CODE), GetItemText(nItem, COL_NAME), rect.right - pt.x, pt.y - rect.top);
			//OutputDebugString("\r\n" + stmp);

			GetParent()->SendMessage(WM_USER, MAKEWPARAM(MSG_POP_MENU, 0), (LPARAM)(LPSTR)(LPCTSTR)stmp);
		}
		else if (nSubItem == 3)
		{
			m_bPopMenu = FALSE;
			PostMessage(WM_KILLFOCUS, 0, 0);
		}
	}

	*pResult = 0;
}


BOOL CCodeListCtrl::LoadClosePng(CString sPath)
{
	m_imgClose.Destroy();
	HRESULT hr = m_imgClose.Load(sPath);
	OutputDebugString("[CCodeListCtrl] LoadClosePng " + sPath + "\n");
	return SUCCEEDED(hr);
}

void CCodeListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = HOVER_DEFAULT;
		_TrackMouseEvent(&tme);
		m_bTracking = TRUE;
	}

	LVHITTESTINFO hitInfo;
	hitInfo.pt = point;
	int nItem = HitTest(&hitInfo);

	if (nItem != m_nHoverItem)
	{
		// 이전 호버 행 선택 해제
		if (m_nHoverItem >= 0)
			SetItemStates(m_nHoverItem, RC_ITEM_UNSELECTED);

		m_nHoverItem = nItem;

		// 새 호버 행 선택
		if (m_nHoverItem >= 0)
			SetItemStates(m_nHoverItem, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);

		Invalidate();

		CString slog;
		slog.Format("[CCodeListCtrl] OnMouseMove hover=%d\n", m_nHoverItem);
		OutputDebugString(slog);
	}

	CListCtrl::OnMouseMove(nFlags, point);
}


LRESULT  CCodeListCtrl::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	OutputDebugString("[CCodeListCtrl] OnMouseLeave\n");
	// 호버 행 선택 해제
	if (m_nHoverItem >= 0)
		SetItemStates(m_nHoverItem, RC_ITEM_UNSELECTED);

	m_bTracking = FALSE;
	m_nHoverItem = -1;
	Invalidate();
	//CListCtrl::OnMouseLeave();
	return 0;
}
