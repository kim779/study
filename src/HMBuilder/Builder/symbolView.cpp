// symbolView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "symbolView.h"

#include "h/form_w.h"
#include "h/mainvar.h"

// CSymbolView

#define	ID_SYM_TREE		1
#define	ID_SYM_LIST		2
#define	ID_SYM_PCOMBO		3
#define	ID_SYM_GCOMBO		4
#define	ID_SYM_ICOMBO		5

#define	W_IOCB			77

IMPLEMENT_DYNAMIC(CSymbolView, CDockablePane)

CSymbolView::CSymbolView()
{
	m_isTree = true;
}

CSymbolView::~CSymbolView()
{
}


BEGIN_MESSAGE_MAP(CSymbolView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_TREE, OnTree)
	ON_COMMAND(ID_LIST, OnList)
	ON_CBN_SELENDOK(ID_SYM_PCOMBO, OnProjectSelChange)
	ON_CBN_SELENDOK(ID_SYM_GCOMBO, OnGroupSelChange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_REFRESH, ID_LIST, OnUpdateCommand)
END_MESSAGE_MAP()

// CSymbolView 메시지 처리기입니다.

int CSymbolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect	rectDummy;

	rectDummy.SetRectEmpty();
	// treeview mode
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_tree.Create(dwViewStyle, rectDummy, this, ID_SYM_TREE))
	{
		TRACE0(_T("심볼 트리뷰를 만들지 못했습니다.\n"));
		return -1;
	}

	if (!m_ioCB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS , rectDummy, this, ID_SYM_ICOMBO))
		return -1;
	m_ioCB.SetItemData(m_ioCB.AddString(_T("Input")), EIO_INPUT);
	m_ioCB.SetItemData(m_ioCB.AddString(_T("Output")), EIO_OUTPUT);
	m_ioCB.SetCheck(0, FALSE);

	// listview mode
	// group combo
	m_projectCB.Create(WS_CHILD | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS, rectDummy, this, ID_SYM_PCOMBO);
	m_groupCB.Create(WS_CHILD | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS, rectDummy, this, ID_SYM_GCOMBO);

	// list ctrl
	if (!m_list.Create(WS_CHILD| LVS_REPORT | LVS_ALIGNTOP | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SORTASCENDING |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rectDummy, this, ID_SYM_LIST))
	{
		TRACE0(_T("서버맵 리스트뷰를 만들지 못했습니다.\n"));
		return -1;
	}
	DWORD	dwStyle = m_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	m_list.EnableToolTips();
	m_list.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);

	// toolbar_tree
	m_wndToolBarTree.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MAPVIEW);
	m_wndToolBarTree.LoadToolBar(IDR_MAPVIEW, 0, 0, TRUE /* 잠금 */);

	m_wndToolBarTree.SetPaneStyle(m_wndToolBarTree.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBarTree.SetPaneStyle(m_wndToolBarTree.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBarTree.SetOwner(this);
	m_wndToolBarTree.SetRouteCommandsViaFrame(FALSE);	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBarTree.RemoveButton(3);
	m_wndToolBarTree.RemoveButton(2);

	// toolbar_list
	m_wndToolBarList.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MAPVIEW_LIST);
	m_wndToolBarList.LoadToolBar(IDR_MAPVIEW_LIST, 0, 0, TRUE /* 잠금 */);

	m_wndToolBarList.SetPaneStyle(m_wndToolBarList.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBarList.SetPaneStyle(m_wndToolBarList.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBarList.SetOwner(this);
	m_wndToolBarList.SetRouteCommandsViaFrame(FALSE);	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.

	m_root = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);

	setFont();
	adjustLayout();
	OnRefresh();
	return 0;
}

void CSymbolView::adjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect	rect;
	int	cy = m_wndToolBarTree.CalcFixedLayout(FALSE, TRUE).cy;

	GetClientRect(rect);
	if (m_isTree)
	{
		m_wndToolBarList.ShowWindow(SW_HIDE);
		m_projectCB.ShowWindow(SW_HIDE);
		m_groupCB.ShowWindow(SW_HIDE);
		m_list.ShowWindow(SW_HIDE);

		m_wndToolBarTree.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), cy, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_ioCB.SetWindowPos(NULL, rect.left, rect.top+cy, rect.Width(), 100, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_tree.SetWindowPos(NULL, rect.left, rect.top+(cy*2), rect.Width(), rect.Height()-(cy*2), SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
	}
	else
	{
		m_wndToolBarTree.ShowWindow(SW_HIDE);
		m_tree.ShowWindow(SW_HIDE);

		m_wndToolBarList.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), cy, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_ioCB.SetWindowPos(NULL, rect.right-W_IOCB, rect.top+cy, W_IOCB, 100, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_projectCB.SetWindowPos(NULL, rect.left, rect.top+cy, rect.Width()-W_IOCB, 100, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_groupCB.SetWindowPos(NULL, rect.left, rect.top+(cy*2), rect.Width(), 100, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_list.SetWindowPos(NULL, rect.left, rect.top+(cy*3), rect.Width(), rect.Height()-(cy*3), SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_list.SetColumnWidth(0, rect.Width());
	}
	InvalidateRect(rect, TRUE);
}

void CSymbolView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	adjustLayout();
}

void CSymbolView::OnRefresh()
{
	Csymbol*	symbol;
	CsymbolSET*	symSET;
	Cgroup*		group;
	Croot*		root;
	Cproject*	project;
	HTREEITEM	hItemProject;
	HTREEITEM	hItemGroup;

	m_symTbl.Initialize(m_root, AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISOFFLINE) ? false : true);
	m_symTbl.LoadProject();
	m_symTbl.LoadFile();

	m_projectCB.ResetContent();
	for (int ii = 0; ii < m_symTbl.m_arSET.GetSize(); ii++)
	{
		project = (Cproject *)m_symTbl.m_arSET.GetAt(ii);
		m_projectCB.SetItemData(m_projectCB.AddString(project->m_name), (DWORD)project);
	}

	m_tree.DeleteAllItems();
	for (int ii = 0; ii < m_symTbl.m_arSET.GetSize(); ii++)
	{
		project = (Cproject *)m_symTbl.m_arSET.GetAt(ii);
		hItemProject = m_tree.InsertItem(project->m_name, 0, 0);
		m_tree.SetItemData(hItemProject, (DWORD)project);

		for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
		{
			root = project->m_arPRO.GetAt(jj);
			for (int kk = 0; kk < root->m_arROOT.GetSize(); kk++)
			{
				group = root->m_arROOT.GetAt(kk);

				for (int ll = 0; ll < group->m_arGROUP.GetSize(); ll++)
				{
					symSET = group->m_arGROUP.GetAt(ll);
					hItemGroup = m_tree.InsertItem(symSET->m_name, 0, 0, hItemProject);
					m_tree.SetItemData(hItemGroup, (DWORD)symSET);

					for (int mm = 0; mm < symSET->m_arSYM.GetSize(); mm++)
					{
						symbol = symSET->m_arSYM.GetAt(mm);
						HTREEITEM hItem = m_tree.InsertItem(symbol->m_head + _T("  (") + symbol->m_name + _T(")"), 0, 0, hItemGroup);
						m_tree.SetItemData(hItem, mm);
						m_tree.SetItemImage(hItem, 3, 3);
					}
				}
			}
		}
	}

	if (m_projectCB.GetCount())
	{
		m_projectCB.SetCurSel(0);
		OnProjectSelChange();
	}
}

void CSymbolView::OnTree()
{
	m_isTree = false;
	adjustLayout();
}

void CSymbolView::OnList()
{
	m_isTree = true;
	adjustLayout();
}

void CSymbolView::OnProjectSelChange()
{
	if (m_projectCB.GetCurSel() != CB_ERR)
	{
		CsymbolSET*	symSET;
		Cgroup*		group;
		Croot*		root;
		Cproject*	project = (Cproject *)m_projectCB.GetItemData(m_projectCB.GetCurSel());

		m_groupCB.ResetContent();
		for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
		{
			root = project->m_arPRO.GetAt(jj);
			for (int kk = 0; kk < root->m_arROOT.GetSize(); kk++)
			{
				group = root->m_arROOT.GetAt(kk);
				for (int ll = 0; ll < group->m_arGROUP.GetSize(); ll++)
				{
					symSET = group->m_arGROUP.GetAt(ll);
					m_groupCB.SetItemData(m_groupCB.AddString(symSET->m_name), (DWORD)symSET);
				}
			}
		}

		if (m_groupCB.GetCount() > 0)
		{
			m_groupCB.SetCurSel(0);
			OnGroupSelChange();
		}
	}
}

void CSymbolView::OnGroupSelChange()
{
	if (m_groupCB.GetCurSel() == CB_ERR)
		return;

	Csymbol*  symbol;
	CsymbolSET* symSET = (CsymbolSET*)m_groupCB.GetItemData(m_groupCB.GetCurSel());
	int	item  = 0;

	m_symTbl.SetCurrect(symSET);
	m_list.DeleteAllItems();
	for (int ii = 0; ii < symSET->m_arSYM.GetSize(); ii++, item++)
	{
		symbol = symSET->m_arSYM.GetAt(ii);
		m_list.AddItem(item, 0, symbol->m_head, (DWORD)symbol);
		m_list.AddItem(item, 1, symbol->m_name, (DWORD)symbol);
	}

}

void CSymbolView::OnUpdateCommand(CCmdUI *pCmdUI)
{
	switch (pCmdUI->m_nID)
	{
	case ID_LIST:
		pCmdUI->SetCheck(!m_isTree);
		pCmdUI->Enable(TRUE);
		break;

	default:
		pCmdUI->Enable(TRUE);
		break;
	}
}

bool CSymbolView::GetSelectedSymbol(CPtrArray* arr)
{
	BYTE	iok = 0;

	if (m_ioCB.GetCheck(0))
		iok = EIO_INPUT;
	else if (m_ioCB.GetCheck(1))
		iok = EIO_OUTPUT;
	if (m_ioCB.GetCheck(0) && m_ioCB.GetCheck(1))
		iok = EIO_INOUT;

	arr->RemoveAll();
	if (m_isTree)
	{
		HTREEITEM hitem;
		CsymbolSET*	symSET;
		int	idx, count = m_tree.GetSelectedItemCount();
		for (int ii = 0; ii < count; ii++)
		{
			hitem = m_tree.GetSelectedItem(ii);
			symSET = (CsymbolSET*)m_tree.GetItemData(m_tree.GetParentItem(hitem));
			idx = (int)m_tree.GetItemData(hitem);
			if (!m_symTbl.m_default)
				symSET->m_arSYM.GetAt(idx)->m_iok = iok;
			else if (m_symTbl.m_default && symSET->m_arSYM.GetAt(idx)->m_iok <= 0)
				idx = -1;
			if (idx >= 0)
				arr->Add(symSET->m_arSYM.GetAt(idx));
		}
	}
	return m_symTbl.m_default;
}

void CSymbolView::setFont()
{
	::DeleteObject(m_font.Detach());

	LOGFONT	lf;
	NONCLIENTMETRICS info;

	afxGlobalData.fontRegular.GetLogFont(&lf);

	info.cbSize = sizeof(info);
	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_font.CreateFontIndirect(&lf);

	m_projectCB.SetFont(&m_font);
	m_groupCB.SetFont(&m_font);
	m_ioCB.SetFont(&m_font);

	m_tree.SetFont(&m_font);
	m_list.SetFont(&m_font);
}
