// mapView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "mapView.h"

#include "h/axisvar.h"
#include "h/mainvar.h"
#include "amCc/LibBuild.h"
#include "download.h"
#include <Imm.h>

// CMapView
#define	ID_MAP_TREE		1
#define	ID_MAP_LIST		2
#define	ID_MAP_COMBO		3
#define	ID_MAP_EDIT		4

IMPLEMENT_DYNAMIC(CMapView, CDockablePane)

CMapView::CMapView()
{
	m_isTree = true;
	m_isTwoDepth = false;
	m_usid = _T("");
	m_maptreegubn = ID_MAPTREE;
}

CMapView::~CMapView()
{
}


BEGIN_MESSAGE_MAP(CMapView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_TREE, OnTree)
	ON_COMMAND(ID_OPEN, OnOpen)
	ON_COMMAND(ID_TWODEPTH, OnTwoDepth)
	ON_COMMAND(ID_LIST, OnList)
	ON_CBN_SELENDOK(ID_MAP_COMBO, OnSelChange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_REFRESH, ID_LIST, OnUpdateCommand)
	ON_MESSAGE(WM_USER+100, &CMapView::OnMessage)
	ON_MESSAGE(WM_USER, &CMapView::OnDownload)
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CMapView 메시지 처리기입니다.

int CMapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect	rectDummy;

	rectDummy.SetRectEmpty();
	// treeview mode
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_tree.Create(dwViewStyle, rectDummy, this, ID_MAP_TREE))
	{
		TRACE0(_T("서버맵 트리뷰를 만들지 못했습니다.\n"));
		return -1;
	}
	m_tree.Initialize(m_dir);

	// listview mode
	// group combo
	m_group.Create(WS_CHILD | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS, rectDummy, this, ID_MAP_COMBO);

	// list ctrl
	if (!m_list.Create(WS_CHILD| LVS_REPORT | LVS_ALIGNTOP | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SORTASCENDING |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rectDummy, this, ID_MAP_LIST))
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

	// toolbar_list
	m_wndToolBarList.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MAPVIEW_LIST);
	m_wndToolBarList.LoadToolBar(IDR_MAPVIEW_LIST, 0, 0, TRUE /* 잠금 */);

	m_wndToolBarList.SetPaneStyle(m_wndToolBarList.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBarList.SetPaneStyle(m_wndToolBarList.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBarList.SetOwner(this);
	m_wndToolBarList.SetRouteCommandsViaFrame(FALSE);	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.

	if (!m_edit.Create(WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL | ES_UPPERCASE, rectDummy, this, ID_MAP_EDIT))
	{
		TRACE0(_T("맵 입력 에디트를 만들지 못했습니다.\n"));
		return -1;
	}
	m_edit.LimitText(L_MAPN);

	setFont();
	adjustLayout();
	//OnRefresh();

	this->m_group.SetCurSel(0);
	return 0;
}


void CMapView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	adjustLayout();
}

void CMapView::adjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect	rect;
	int	cy = m_wndToolBarTree.CalcFixedLayout(FALSE, TRUE).cy;

	GetClientRect(rect);
	if (m_isTree)
	{
		m_wndToolBarList.ShowWindow(SW_HIDE);
		m_group.ShowWindow(SW_HIDE);
		m_list.ShowWindow(SW_HIDE);
		m_wndToolBarTree.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), cy, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_tree.SetWindowPos(NULL, rect.left, rect.top+cy, rect.Width(), rect.Height()-cy, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
	}
	else
	{
		m_wndToolBarTree.ShowWindow(SW_HIDE);
		m_tree.ShowWindow(SW_HIDE);
		m_wndToolBarList.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), cy, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_group.SetWindowPos(NULL, rect.left, rect.top+cy, rect.Width(), 100, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_list.SetWindowPos(NULL, rect.left, rect.top+(cy*2), rect.Width(), rect.Height()-(cy*2), SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		m_list.SetColumnWidth(0, rect.Width());
	}
}

void CMapView::OnRefresh()
{
	CString tmps;
	CDownload* download = new CDownload();

	m_tree.RemoveAll();

	download->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 2001);
	switch (m_maptreegubn)
	{
	case ID_MAPTREE:
		wccSetTempPath(tmps, DN_FILE);
		DeleteFile(tmps);
		download->Download(m_usid, _T(""), _T(""), tmps, TRGB_FORMALL, 0, 0, false);
		break;
	case ID_TRLATOUT:
		wccSetTempPath(tmps, TR_FILE);
		DeleteFile(tmps);
		download->Download(m_usid, _T(""), _T(""), tmps, TRGB_TRLAYOUT, 0, 0, false);
		break;
	}
}

void CMapView::OnTree()
{
	m_isTree = false;
	adjustLayout();
}

void CMapView::OnOpen()
{
	if (m_isTree)
		m_tree.OpenMap();
}

void CMapView::OnTwoDepth()
{
	m_isTwoDepth = !m_isTwoDepth;
	m_tree.SetDepth(m_isTwoDepth ? L_SELC : L_TGID);
}

void CMapView::OnList()
{
	m_isTree = true;
	adjustLayout();
}

void CMapView::OnSelChange()
{
	//struct	_mapTree	mt;
	int	idx = (m_group.GetCurSel() != CB_ERR) ? (int)m_group.GetItemData(m_group.GetCurSel()) : -1;

	m_list.DeleteAllItems();
	/*for (int ii = 0; ii < m_tree.m_treeArr.GetSize(); ii++)
	{
		mt = m_tree.m_treeArr.GetAt(ii);
		if (idx == -1 || atoi(mt.mgub) == idx)
			m_list.AddItem(ii, 0, mt.mapd, ii, mt.mapn);
	}*/
}

void CMapView::OnUpdateCommand(CCmdUI *pCmdUI)
{
	switch (pCmdUI->m_nID)
	{
	case ID_LIST:
		pCmdUI->SetCheck(!m_isTree);
		pCmdUI->Enable(TRUE);
		break;

	case ID_TWODEPTH:
		pCmdUI->SetCheck(m_isTwoDepth);
		pCmdUI->Enable(TRUE);
		break;

	default:
		pCmdUI->Enable(TRUE);
		break;
	}
}

afx_msg LRESULT CMapView::OnMessage(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))
	{
	case 1:	// download & open
	case 2: // download
		{
			if (LOWORD(wp) == 1)
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, 0);
		}
		break;

	case 4:		// get mapname
//		return (LRESULT)m_tree.m_treeArr.GetAt((int)lp).mapn;

	case 5:		// open map
		m_tree.OpenMap((int)lp);
		break;

	case 6:		// open map
		m_tree.OpenMap((char*)lp);
		break;
	}

	return 0;
}

afx_msg LRESULT CMapView::OnDownload(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case ID_USR_TRLIST:
		LoadMapTree(m_usid, m_maptreegubn);
		break;
	}
		
	return 0;
}


int CMapView::GetFiles(CStringArray& files)
{
	if (m_isTree)
		return m_tree.GetFiles(files);
	return m_list.GetFiles(files);
}

void CMapView::OpenMap(CString maps)
{
	m_tree.OpenMap(maps);
}

int CMapView::IsTwoDepth()
{
	return m_isTwoDepth ? 1 : 0;
}

void CMapView::SetTwoDepth(bool twoDepth)
{
	m_isTwoDepth = twoDepth;
	m_tree.SetDepth(m_isTwoDepth ? L_SELC : L_TGID, false);
}

void CMapView::setFont()
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

	m_group.SetFont(&m_font);
	m_tree.SetFont(&m_font);
	m_list.SetFont(&m_font);
	m_edit.SetFont(&m_font);
}


BOOL CMapView::PreTranslateMessage(MSG* pMsg)
{
	CRect	rc;
	BOOL	offline = (BOOL)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISOFFLINE);

	if (pMsg->message == WM_LBUTTONDOWN)
	{
		DWORD	dwConversion, dwSentence;

		HIMC hImc = ImmGetContext(this->m_hWnd);
		if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
			ImmSetConversionStatus(hImc, dwConversion & ~IME_CMODE_HANGEUL, dwSentence);
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (m_edit.IsWindowVisible())
				m_edit.ShowWindow(SW_HIDE);
			if (m_edit.GetWindowTextLength() == L_MAPN && !offline)
			{
				CString	sName;

				m_edit.GetWindowText(sName);
				if (!m_isTree)
					m_list.AddFiles(sName);
				OpenMap(sName);
			}
			m_edit.SetWindowText(_T(""));
		}
		
		if ((::GetKeyState(VK_CONTROL) & 0x8000))
		{
			if (pMsg->wParam == _T('c') || pMsg->wParam == _T('C'))
				m_edit.Copy();
			else if (pMsg->wParam == _T('v') || pMsg->wParam == _T('V'))
			{
				if (!m_edit.IsWindowVisible())
				{
					if (m_isTree)
						m_tree.GetWindowRect(&rc);
					else
						m_list.GetWindowRect(&rc);
					ScreenToClient(&rc);
					rc.OffsetRect(2, 2);
					rc.right = rc.left + 80;
					rc.bottom = rc.top + 20;
					m_edit.MoveWindow(rc);
					m_edit.SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_DRAWFRAME | SWP_FRAMECHANGED);
					m_edit.ShowWindow(SW_SHOW);
					m_edit.SetFocus();
				}
				m_edit.Paste();
			}
			else if (pMsg->wParam == _T('x') || pMsg->wParam == _T('X'))
			{
				if (!m_edit.IsWindowVisible())
				{
					if (m_isTree)
						m_tree.GetWindowRect(&rc);
					else
						m_list.GetWindowRect(&rc);
					ScreenToClient(&rc);
					rc.OffsetRect(2, 2);
					rc.right = rc.left + 80;
					rc.bottom = rc.top + 20;
					m_edit.MoveWindow(rc);
					m_edit.SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_DRAWFRAME | SWP_FRAMECHANGED);
					m_edit.ShowWindow(SW_SHOW);
					m_edit.SetFocus();
				}
				m_edit.Cut();
			}
			else if (pMsg->wParam == _T('z') || pMsg->wParam == _T('Z'))
				m_edit.Undo();
		}

		if (!(::GetKeyState(VK_CONTROL) & 0x8000) && isalnum((int)pMsg->wParam) && !m_edit.IsWindowVisible() && !offline)
		{
			if (pMsg->wParam >= 0x70 && pMsg->wParam <= 0x7B)
				return CDockablePane::PreTranslateMessage(pMsg);

			if (m_isTree)
				m_tree.GetWindowRect(&rc);
			else
				m_list.GetWindowRect(&rc);
			ScreenToClient(&rc);
			rc.OffsetRect(2, 2);
			rc.right = rc.left + 80;
			rc.bottom = rc.top + 20;
			m_edit.MoveWindow(rc);
			m_edit.SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_DRAWFRAME | SWP_FRAMECHANGED);
			m_edit.ShowWindow(SW_SHOW);
			m_edit.SetWindowText(_T(""));
			m_edit.SetFocus();
			m_edit.PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		}
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}

void CMapView::LoadMapTree(CString usid, int maptreekind)
{
	CString tmps;
	m_usid = usid;
	m_maptreegubn = maptreekind;
	m_tree.m_maptreekind = maptreekind;

	if (maptreekind == ID_MAPTREE)
		wccSetTempPath(tmps, DN_FILE);
	else
		wccSetTempPath(tmps, TR_FILE);

	m_tree.LoadFile(tmps);
	//OnRefresh();
}


void CMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	
	CDockablePane::OnRButtonDown(nFlags, point);
}


void CMapView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_tree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	/*if (point != CPoint(-1, -1))
	{
		// 클릭한 항목을 선택합니다.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}*/

	pWndTree->SetFocus();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMapView::SetMapTreeGubn(int treekind)
{
	m_maptreegubn = treekind;
	m_tree.m_maptreekind = treekind;
}