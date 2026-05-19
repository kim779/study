
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "axisMBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_MAPONLY, OnMapOnly)
	ON_UPDATE_COMMAND_UI_RANGE(ID_REFRESH, ID_MAPONLY, OnUpdateCommand)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 메시지 처리기

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect	rectDummy;

	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_tree.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0(_T("파일 뷰를 만들지 못했습니다.\n"));
		return -1;      // 만들지 못했습니다.
	}

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 잠금 */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	adjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	adjustLayout();
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_tree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
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
	}

	pWndTree->SetFocus();
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::adjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect	rect;

	GetClientRect(rect);

	int cy = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), cy, SWP_NOACTIVATE | SWP_NOZORDER);
	m_tree.SetWindowPos(NULL, rect.left, rect.top + cy, rect.Width(), rect.Height() - cy, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("Attribute...."));

}

void CFileView::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditClear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	CRect	rect;

	m_tree.GetWindowRect(rect);
	ScreenToClient(rect);

	rect.InflateRect(1, 1);
	dc.Draw3dRect(rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_tree.SetFocus();
}

void CFileView::OnRefresh()
{
	m_tree.Refresh();
}

void CFileView::OnMapOnly()
{
	UINT	style = m_wndToolBar.GetButtonStyle(1);
	
	if (style & TBBS_PRESSED)
		style &= ~TBBS_PRESSED;
	else
		style |= TBBS_PRESSED;

	m_wndToolBar.SetButtonStyle(1, style);
	m_tree.SetMapOnly();
	m_tree.Refresh();
}

void CFileView::OnUpdateCommand(CCmdUI *pCmdUI)
{
	switch (pCmdUI->m_nID)
	{
	case ID_REFRESH:
		pCmdUI->Enable(TRUE);
		break;

	case ID_MAPONLY:
		pCmdUI->SetCheck(m_tree.m_mapOnly);
		pCmdUI->Enable(TRUE);
		break;
	}
}
