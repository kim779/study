// editListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "editListCtrl.h"
#include "itemEdit.h"
#include "itemCombo.h"


#define	ID_ITEM_EDIT	0x1234
#define	ID_ITEM_COMBO	0x1235

// CEditListCtrl

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)

CEditListCtrl::CEditListCtrl()
{

}

CEditListCtrl::~CEditListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeyDown)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
END_MESSAGE_MAP()



// CEditListCtrl 메시지 처리기입니다.

BOOL CEditListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS;	
	return CListCtrl::PreCreateWindow(cs);
}

int CEditListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetExtendedStyle( GetExtendedStyle() | LVS_EX_CHECKBOXES );
	return 0;
}

void CEditListCtrl::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pNMKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	switch (pNMKeyDown->wVKey)
	{
	case VK_DELETE: 
		{
			int	nItem = GetSelectionMark();
			if (nItem!=-1) // valid item 					
				DeleteItem(nItem);
		}
		break;

	default:
		break;
	}
	*pResult = 0;
}

void CEditListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (GetFocus() != this) 
		SetFocus();

	LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMListView->iItem == -1)
		return;

	if (GetParent()->SendMessage(WM_USER + 1, 0, MAKELPARAM(pNMListView->iItem, pNMListView->iSubItem)))
	{
		if (getType(pNMListView->iSubItem) == _T('2'))
			makeComboItem(pNMListView->iItem, pNMListView->iSubItem);
		else if (getType(pNMListView->iSubItem) == _T('1'))
			makeEditItem (pNMListView->iItem, pNMListView->iSubItem);
	}

	*pResult = 0;
}

void CEditListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* plvDispInfo =  reinterpret_cast<LV_DISPINFO*>(pNMHDR);
 	LV_ITEM* plvItem = &plvDispInfo->item;

	if (plvItem->iItem != -1 &&  plvItem->pszText)	// valid item && valid text
	{
		SetItemText( plvItem->iItem, plvItem->iSubItem, plvItem->pszText);				
		if (plvItem->iSubItem != 0)
		{
			char szText[0x10+1];
			GetItemText(plvItem->iItem, 0, szText, 0x10);				
			SetItemText(plvItem->iItem, 0, szText);				
		}	
	}

	*pResult = 0;
}

char CEditListCtrl::getType(int col)
{
	if (m_type.GetLength() <= col)
		return _T(' ');

	return m_type.GetAt(col);
}

CComboBox* CEditListCtrl::makeComboItem(int nItem, int nSubItem)
{
	if (!EnsureVisible(nItem, TRUE)) 
		return NULL;

	CRect	rect, rc;
	int	offset = 0;
	CString	str = GetItemText(nItem, nSubItem);

	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	GetClientRect(rc);
	if (offset + rect.left < 0 || offset + rect.left > rc.right)
	{
		CSize	size;

		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}
	
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nSubItem);
	if (rect.right > rc.right) 
	   rect.right = rc.right;

	rect.bottom += 30 * rect.Height();//dropdown area
	
	CComboBox* pCombo = new CItemCombo(nItem, nSubItem, &m_strList);
	pCombo->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL, rect, this, ID_ITEM_COMBO);
	pCombo->ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	pCombo->ShowDropDown();
	pCombo->SelectString(-1, str.GetBuffer(1));
	return pCombo;
}

CEdit* CEditListCtrl::makeEditItem( int nItem, int nSubItem)
{
	if (!EnsureVisible(nItem, TRUE))
		return NULL;	

	CRect	rect, rc;
	int	offset = 0;
	DWORD	dwStyle;
	LV_COLUMN	lvcol;

	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	GetClientRect(rc);
	if ( offset + rect.left < 0 || offset + rect.left > rc.right)
	{
		CSize	size(offset + rect.left, 0);		
		Scroll(size);
		rect.left -= size.cx;
	}
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nSubItem);
	if (rect.right > rc.right) 
	   rect.right = rc.right;

	// Get Column alignment	
	lvcol.mask = LVCF_FMT;
	GetColumn(nSubItem, &lvcol);

	if ((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else 
		dwStyle = ES_CENTER;	

	
	dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	CEdit *pEdit = new CItemEdit(nItem, nSubItem, GetItemText(nItem, nSubItem));

	pEdit->Create(dwStyle, rect, this, ID_ITEM_EDIT);	
	pEdit->SetSel(0, -1);
	return pEdit;
}
