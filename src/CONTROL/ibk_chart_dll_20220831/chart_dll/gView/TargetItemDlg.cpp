// TargetItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgview.h"
#include "TargetItemDlg.h"
#include "ConfigBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTargetItemDlg dialog


CTargetItemDlg::CTargetItemDlg(CWnd* pParent)
	: CDialog(CTargetItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTargetItemDlg)
	//}}AFX_DATA_INIT

	m_bar = (CConfigBar*)pParent;
	ZeroMemory(&m_lastKind, sizeof(KIND));
	m_selectKind.Empty();
}


void CTargetItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTargetItemDlg)
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_LIST2, m_listSelect);
	DDX_Control(pDX, IDC_LIST1, m_listSource);
	DDX_Control(pDX, IDC_UP, m_btnUp);
	DDX_Control(pDX, IDC_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_REMOVEALL, m_btnRemoveAll);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_ADDALL, m_btnAddAll);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_STATIC3, m_label3);
	DDX_Control(pDX, IDC_STATIC2, m_label2);
	DDX_Control(pDX, IDC_STATIC1, m_label1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTargetItemDlg, CDialog)
	//{{AFX_MSG_MAP(CTargetItemDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_AXITEMCLICK, OnAxItemClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetItemDlg message handlers

BOOL CTargetItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControls();	
	m_items = new CAxTargetItem();
	m_items->LoadItemFromFile();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTargetItemDlg::SetKind(LPCTSTR str)
{
	m_selectKind = str;
}

void CTargetItemDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	delete m_items;	
}

void CTargetItemDlg::InitControls()
{
	m_btnAdd.LoadBitmap(m_bar->GetImageName("GTB_ADD"));
	m_btnRemove.LoadBitmap(m_bar->GetImageName("GTB_REMOVE"));

	m_btnAddAll.LoadBitmap(m_bar->GetImageName("GTB_ADDALL"));
	m_btnRemoveAll.LoadBitmap(m_bar->GetImageName("GTB_REMOVEALL"));

	m_btnUp.LoadBitmap(m_bar->GetImageName("GTB_UP"));
	m_btnDown.LoadBitmap(m_bar->GetImageName("GTB_DOWN"));

	const int btnWidth = 30, btnHeight = 18;

	m_btnAdd.SetWindowPos(NULL, 0, 0, btnWidth, btnHeight, SWP_NOMOVE);
	m_btnRemove.SetWindowPos(NULL, 0, 0, btnWidth, btnHeight, SWP_NOMOVE);
	m_btnAddAll.SetWindowPos(NULL, 0, 0, btnWidth, btnHeight, SWP_NOMOVE);
	m_btnRemoveAll.SetWindowPos(NULL, 0, 0, btnWidth, btnHeight, SWP_NOMOVE);

	const int moveBtnSize = 18;
	m_btnUp.SetWindowPos(NULL, 0, 0, moveBtnSize, moveBtnSize, SWP_NOMOVE);
	m_btnDown.SetWindowPos(NULL, 0, 0, moveBtnSize, moveBtnSize, SWP_NOMOVE);

	InitKindTree();
	InitListControls();

	SetWindowLong(m_tree.m_hWnd, GWL_USERDATA, (LONG)&m_listSource);
	SetMessageHandler();
}

void CTargetItemDlg::InitKindTree()
{
	DWORD style = GetWindowLong(m_bar->m_treeKind->m_hWnd, GWL_STYLE);
	SetWindowLong(m_tree.m_hWnd, GWL_STYLE, style);

	HTREEITEM hItem = m_bar->SetGroupList(&m_tree, m_selectKind);

	/**
	if (hItem) {
		if (m_tree.GetParentItem(hItem)) {
			m_tree.Expand(m_tree.GetParentItem(hItem), TVE_EXPAND);
		}

		m_tree.SelectItem(hItem);
		m_tree.EnsureVisible(hItem);
		if (m_tree.ItemHasChildren(hItem)) {
			m_tree.Expand(hItem, TVE_EXPAND);
		}
	}
	**/
}

void CTargetItemDlg::InitListControls()
{
	InitListControl(m_listSource);
	InitListControl(m_listSelect);
}

void CTargetItemDlg::InitListControl(CListCtrl& list)
{
	list.InsertColumn(0, "Á¾¸ñ", LVCFMT_LEFT, 60);
	list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	DWORD style = ::GetWindowLong(list.m_hWnd, GWL_STYLE);
	style |= LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS;
	::SetWindowLong(list.m_hWnd, GWL_STYLE, style);
}

void CTargetItemDlg::SetMessageHandler()
{
	CWnd* child = GetWindow(GW_CHILD);

	while (child) {
		child->SendMessage(AXM_SETMSGHANDLER, (WPARAM)this);	
		child = child->GetWindow(GW_HWNDNEXT);
	}
}

LRESULT CTargetItemDlg::OnAxItemClick(WPARAM wParam, LPARAM lParam)
{
	BEGIN_AXM_MESSAGE_MAP((CWnd*)lParam, 
		&m_tree,							OnAxTreeItemClick)
		AXM_MESSAGE_MAP(&m_btnAdd,			OnAxAddButtonClick)
		AXM_MESSAGE_MAP(&m_btnRemove,		OnAxRemoveButtonClick)
		AXM_MESSAGE_MAP(&m_btnAddAll,		OnAxAddAllButtonClick)
		AXM_MESSAGE_MAP(&m_btnRemoveAll,	OnAxRemoveAllButtonClick)
		AXM_MESSAGE_MAP(&m_btnUp,			OnAxUpButtonClick)
		AXM_MESSAGE_MAP(&m_btnDown,			OnAxDownButtonClick)
	END_AXM_MESSAGE_MAP
}

LRESULT CTargetItemDlg::OnAxTreeItemClick(WPARAM wParam, LPARAM lParam)
{
	CAxMTreeCtrl* tree = (CAxMTreeCtrl*)lParam;
	
	m_bar->SetGroupItem(tree, (HTREEITEM)wParam);

	if (!tree->ItemHasChildren((HTREEITEM)wParam)) {
		DWORD param = tree->GetItemData((HTREEITEM)wParam);
		
		m_bar->GetKindData(param, &m_lastKind);

		CString items = m_items->GetItem(m_lastKind);
		ProcessSelectedItem(items);
	}
	
	ResizeListColumn();
	return 0L;
}

LRESULT CTargetItemDlg::OnAxAddButtonClick(WPARAM wParam, LPARAM lParam)
{
	AddItem();
	return 0L;
}

LRESULT CTargetItemDlg::OnAxRemoveButtonClick(WPARAM wParam, LPARAM lParam)
{
	RemoveItem();
	return 0L;
}

LRESULT CTargetItemDlg::OnAxAddAllButtonClick(WPARAM wParam, LPARAM lParam)
{
	AddAllItem();
	return 0L;
}

LRESULT CTargetItemDlg::OnAxRemoveAllButtonClick(WPARAM wParam, LPARAM lParam)
{
	RemoveAllItem();
	return 0L;
}

LRESULT CTargetItemDlg::OnAxUpButtonClick(WPARAM wParam, LPARAM lParam)
{
	MoveUpItem();
	return 0L;
}

LRESULT CTargetItemDlg::OnAxDownButtonClick(WPARAM wParam, LPARAM lParam)
{
	MoveDownItem();
	return 0L;
}

void CTargetItemDlg::ResizeListColumn(CAxMListCtrl* list /* = NULL */)
{
	if (list) {
		CRect rc;
		list->GetClientRect(&rc);
		list->SetColumnWidth(0, rc.Width());
	}
	else {
		ResizeListColumn(&m_listSource);
		ResizeListColumn(&m_listSelect);
	}
}

void CTargetItemDlg::AddItem()
{
	ExchangeItem(m_listSource, m_listSelect, TRUE);
}

void CTargetItemDlg::AddAllItem()
{
	ExchangeItem(m_listSource, m_listSelect, FALSE);
}


void CTargetItemDlg::RemoveItem()
{
	ExchangeItem(m_listSelect, m_listSource, TRUE);
}

void CTargetItemDlg::RemoveAllItem()
{
	ExchangeItem(m_listSelect, m_listSource, FALSE);
}


void CTargetItemDlg::MoveUpItem()
{
	MoveListItem(m_listSelect, -1);
}

void CTargetItemDlg::MoveDownItem()
{
	MoveListItem(m_listSelect, 1);
}


void CTargetItemDlg::ExchangeItem(CAxMListCtrl& src, CAxMListCtrl& dest, BOOL selectOnly)
{
	if (!src.GetItemCount())
		return;
	if (selectOnly && !src.GetSelectedCount())
		return;
	
	CString item;
	for (int i = 0; i < src.GetItemCount(); i++) {
		if (!selectOnly || src.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED) {
			item = src.GetItemText(i, 0);
			dest.InsertItem(dest.GetItemCount(), item);
		}
	}
	if (!selectOnly) {
		src.DeleteAllItems();
	}
	else {
		for (i = src.GetItemCount() - 1; i >= 0; i--) {
			if (src.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED) {
				src.DeleteItem(i);
			}
		}
	}
	
	Modified();
}

void CTargetItemDlg::MoveListItem(CAxMListCtrl& list, int distance)
{
	if (list.GetSelectedCount() != 1)
		return;

	CString item;
	for (int i = 0; i < list.GetItemCount(); i++) {
		if (list.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED) {
			if (i + distance < 0 || i + distance >= list.GetItemCount()) return;
			item = list.GetItemText(i, 0);
			list.DeleteItem(i);
			list.InsertItem(i + distance, item);
			list.SetItemState(i + distance, LVIS_SELECTED, LVIS_SELECTED);
			Modified();
			return;
		}
	}
}

int CTargetItemDlg::FindText(CAxMListCtrl& list, LPCTSTR text)
{
	for (int i = 0; i < list.GetItemCount(); i++) {
		if (list.GetItemText(i, 0) == text)
			return i;
	}
	return -1;
}

void CTargetItemDlg::Modified()
{
	m_modified = TRUE;

	m_items->SetItem(m_lastKind, m_listSelect);
}

void CTargetItemDlg::OnOK() 
{
	m_items->SaveItemToFile();
	
	CDialog::OnOK();
}

void CTargetItemDlg::ProcessSelectedItem(CString items)
{
	m_listSelect.DeleteAllItems();
	
	int find;
	CString code, name;
	
	while (TRUE) {
		find = items.Find(';');
		if (find < 1) 
			break;

		code = items.Left(find);
		items.Delete(0, find + 1);

		name = m_bar->GetCodeName(code);
		if (!name.IsEmpty()) {
			m_listSelect.InsertItem(m_listSelect.GetItemCount(), code + SPACE + name);
		}
	}

	for (int i = 0; i < m_listSelect.GetItemCount(); i++) {
		code = m_listSelect.GetItemText(i, 0);
		find = code.Find(SPACE);
		code = code.Left(find);

		for (int j = 0; j < m_listSource.GetItemCount(); j++) {
			if (code == m_listSource.GetItemText(j, 0).Left(find)) {
				m_listSource.DeleteItem(j);
				break;
			}
		}
	}
}

