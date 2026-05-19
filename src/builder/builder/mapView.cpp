// mapView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "mapView.h"

#include "../h/axisvar.h"
#include "h/mainvar.h"
#include "awWcc/libWcc.h"
#include "awSock/download.h"
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
	m_kind = viewMAP;
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
	DWORD_PTR dwStyle = m_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
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
	if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISOFFLINE))	// offline
		return;

	CString tmps;
	CDownload* dn;

	m_tree.RemoveAll();
	m_section.Lock();

	CString UserID = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_USERID, 0), 0);
	dn = new CDownload();
	if (!dn->Create())
	{
		delete dn;
		m_section.Unlock();
		return;
	}

	switch (m_kind)
	{
	case viewTRLAYOUT:
		wccSetTempPath(tmps, TR_FILE);
		if (!dn->Download(UserID, _T(""), _T(""), tmps, 5))
		{
			delete dn;
			m_section.Unlock();
			return;
		}
		break;
	case viewMAP:
	default:
		wccSetTempPath(tmps, DN_FILE);
		if (!dn->Download(UserID, _T(""), _T(""), tmps, 2))
		{
			delete dn;
			m_section.Unlock();
			return;
		}
		break;
	}
	delete dn;

	m_section.Unlock();

	m_tree.LoadFile(tmps);

	// fill combo
	m_group.ResetContent();
	for (int ii = 0; ii < m_tree.m_treeArr.GetSize(); ii++)
	{
		tmps = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETGROUP, atoi(m_tree.m_treeArr.GetAt(ii).mgub));
		if (m_group.FindString(0, tmps) < 0)
			m_group.SetItemData(m_group.AddString(tmps), atoi(m_tree.m_treeArr.GetAt(ii).mgub));
	}
	if (m_group.GetCount())
		m_group.SetCurSel(0);
	OnSelChange();
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
	struct	_mapTree	mt;
	int	idx = (m_group.GetCurSel() != CB_ERR) ? (int)m_group.GetItemData(m_group.GetCurSel()) : -1;

	m_list.DeleteAllItems();
	for (int ii = 0; ii < m_tree.m_treeArr.GetSize(); ii++)
	{
		mt = m_tree.m_treeArr.GetAt(ii);
		if (idx == -1 || atoi(mt.mgub) == idx)
			m_list.AddItem(ii, 0, mt.mapd, ii, mt.mapn);
	}
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
			CMapStringToString* files = (CMapStringToString *)lp;

			if (files->GetCount() <= 0)
				break;

			CString	fulls, tmps;
			//BOOL	is_template = HIWORD(wp);
			int mode = HIWORD(wp);

			m_section.Lock();
			CString UserID = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_USERID, 0), 0);
			CDownload* dn = new CDownload();
			if (!dn->Create())
			{
				delete dn;
				m_section.Unlock();
				break;
			}
			for (POSITION pos = files->GetStartPosition(); pos; )
			{
				files->GetNextAssoc(pos, fulls, tmps);
				if (!dn->Download(UserID, _T(""), fulls, tmps, 0, mode))
				//if (!dn->Download(fulls, tmps, is_template))
				{
					delete dn;
					m_section.Unlock();
					return 0;
				}
			}
			delete dn;
			m_section.Unlock();

			if (LOWORD(wp) == 1)
			{
				//if (is_template)
				//	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, 1);
				//else
				//	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, 0);
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, mode);
			}
		}
		break;

	case 3:	// template download......................move template view
		{
			int	pos;
			CString tmps, fulls;

			tmps = (char *)lp;
			if (tmps.IsEmpty())
				return 0;

			m_section.Lock();
			pos = tmps.Find(';');
			if (pos == -1)
			{
				m_section.Unlock();
				break;
			}

			fulls = tmps.Left(pos++);
			tmps = tmps.Mid(pos);
			if (fulls.IsEmpty() || tmps.IsEmpty())
			{
				m_section.Unlock();
				break;
			}

			CString UserID = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_USERID, 0), 0);
			CDownload* dn = new CDownload();
			if (!dn->Create())
			{
				delete dn;
				m_section.Unlock();
				break;
			}
			dn->Download(UserID, _T(""), fulls, tmps, 1);
			delete dn;
			m_section.Unlock();
		}
		break;

	case 4:		// get mapname
		return (LRESULT)m_tree.m_treeArr.GetAt((int)lp).mapn;

	case 5:		// open map
		m_tree.OpenMap((int)lp, (int)'R');
		break;

	case 6:		// open map
		m_tree.OpenMap((char *)lp, (int)'R');
		break;
	case 7: // trlayout download
	{
		CMapStringToString* files = (CMapStringToString *)lp;

		if (files->GetCount() <= 0)
			break;

		CString	fulls, tmps;
		int mode = HIWORD(wp);

		m_section.Lock();
		CString UserID = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_USERID, 0), 0);
		CDownload* dn = new CDownload();
		if (!dn->Create())
		{
			delete dn;
			m_section.Unlock();
			break;
		}
		for (POSITION pos = files->GetStartPosition(); pos;)
		{
			files->GetNextAssoc(pos, fulls, tmps);
			if (!dn->Download(UserID, _T(""), fulls, tmps, 0, mode, -1, true, 'T'))
			{
				delete dn;
				m_section.Unlock();
				return 0;
			}
		}
		delete dn;
		m_section.Unlock();

		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_NEWTRLAYOUT, (LPARAM)tmps.operator LPCSTR());
	}
	break;
	case 8: // download
	{
		CMapStringToString* files = (CMapStringToString *)lp;

		if (files->GetCount() <= 0)
			break;

		CString	fulls, tmps;
		int mode = HIWORD(wp);

		m_section.Lock();
		CString UserID = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_USERID, 0), 0);
		CDownload* dn = new CDownload();
		if (!dn->Create())
		{
			delete dn;
			m_section.Unlock();
			break;
		}
		for (POSITION pos = files->GetStartPosition(); pos;)
		{
			files->GetNextAssoc(pos, fulls, tmps);
			if (!dn->Download(UserID, _T(""), fulls, tmps, 0, mode, -1, true, 'T'))
			{
				delete dn;
				m_section.Unlock();
				return 0;
			}
		}
		delete dn;
		m_section.Unlock();

		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_LOADTRLAYOUT, (LPARAM)tmps.operator LPCSTR());
	}
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

CString CMapView::LoadForm(CString mapName)
{
	return m_tree.LoadForm(mapName);
}

void CMapView::SetMapTreeKind(int kind, bool bReflash)
{
	m_kind = kind;
	m_tree.m_kind = kind;
	if (bReflash)
		OnRefresh();
}

void CMapView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//CDockablePane::OnContextMenu(pWnd, point);
}
