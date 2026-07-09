// mybar.cpp : implementation file
//

#include "stdafx.h"
#include "mybar.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	RIGHT_GAP	3
#define	BOTTOM_GAP	25
/////////////////////////////////////////////////////////////////////////////
// CMyBar

CMyBar::CMyBar()
{
	m_pchTip = NULL;
	m_pwchTip = NULL;
}

CMyBar::~CMyBar()
{
	if(m_pwchTip != NULL)
		delete m_pwchTip;
	
	if(m_pchTip != NULL)
		delete m_pchTip;
}


BEGIN_MESSAGE_MAP(CMyBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CMyBar)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_WRK_TREEBTN, OnTreeBtnClick)
	ON_NOTIFY(NM_DBLCLK, ID_WRK_LISTUSER, OnListDblClk)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

void CMyBar::OnListDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nitem = 0;
	POSITION pos = m_List_user.GetFirstSelectedItemPosition();

	if(pos != NULL)
	{
		while(pos)
			nitem = m_List_user.GetNextSelectedItem(pos);

		//대화창을 하나 띄운다.
	}	
}

void CMyBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CSizingControlBarG::OnPaint() for painting messages
}

int CMyBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_TreeChat.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_TABSTOP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 
		CRect(0,0,0,0), this, IDD_CHATTREE))
		return -1;
	m_TreeChat.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);

	//if (!m_serverlist.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_SERVERIN))
	//	return -1;
	//listctrl

	if (!m_List_user.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | LVS_REPORT | LVS_ALIGNTOP, CRect(0), this, ID_WRK_LISTUSER))
//	if (!m_List_user.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0), this, ID_WRK_LISTUSER))
//		return -1;
	m_List_user.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	DWORD	dwStyle = m_List_user.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_List_user.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	m_List_user.EnableToolTips();

	m_List_user.InsertColumn(0, _T("Symbol"), LVCFMT_CENTER, 80);
	m_List_user.InsertColumn(1, _T("Description"), LVCFMT_LEFT, 90);

	//tree/list
	if (!m_treeBtn.Create(NULL, "\nTree/List", WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(30,3,50,23), this, ID_WRK_TREEBTN))
		return -1;


	//reload
	if (!m_ReloadBtn.Create(NULL, "\nReload", WS_CHILD | WS_VISIBLE, CRect(7,3,27,23), this, ID_WRK_RELOADBTN))
		return -1;

	m_hTreeBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_TREE_TYPE));
	m_treeBtn.m_hBitmap = m_hTreeBitmap;

	m_hListBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_LIST_TYPE));
	m_treeBtn.m_hBitmap2 = m_hListBitmap;

	m_hReloadBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_REFRESH_TYPE));
	m_ReloadBtn.m_hBitmap = m_hReloadBitmap;

	m_treeBtn.SetCheck(false);

	initialize();
	return 0;
}

void CMyBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarG::OnSize(nType, cx, cy);
	CRect rc;
	GetClientRect(rc);
	rc.DeflateRect(3, 25, RIGHT_GAP - 2, 0);
	
	m_TreeChat.MoveWindow(rc);
	m_List_user.MoveWindow(rc);

	m_List_user.SetColumnWidth(0, rc.Width()/3);
	m_List_user.SetColumnWidth(1, rc.Width()/3 * 2);
	
}

void CMyBar::OnTreeBtnClick()
{
	CRect rc;
	if(m_treeBtn.GetCheck())
		m_treeBtn.SetCheck(false);
	else
		m_treeBtn.SetCheck(true);

	CheckTreeList();
}

LRESULT CMyBar::OnMessage(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

BOOL CMyBar::addColumn(CListCtrl *LC, LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) str;
	lvc.cx      = LC->GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;
	return LC->InsertColumn(item, &lvc);
}


void CMyBar::AddItem(CListCtrl *LC, int item, stUser* pdata)
{
	CString str;

	LVITEM data;
	data.mask = LVIF_TEXT;
	data.iItem = item;
	data.iSubItem = 0;
	str.Format(_T("%s"), pdata->strgubn);
	data.pszText = (LPTSTR) str.operator LPCTSTR();
	LC->InsertItem(&data);
	LC->SetItemData(item, (DWORD)pdata);
	str.Format(_T("%s"), pdata->strName);
	data.iSubItem = 1;
	data.pszText = (LPTSTR) str.operator LPCTSTR();
	LC->SetItem(&data);
}

void CMyBar::initialize()
{
	m_TreeChat.LoadFile();
	m_TreeChat.InitTree();
	
	CheckTreeList();
	CString strtemp, strtmp;

	stUser* p_data = NULL;
	for(int ii = 0 ; ii < m_TreeChat.m_userTree.GetSize() ; ii++)
	{  
		p_data = new stUser;
		p_data->strgubn = m_TreeChat.m_userTree.GetAt(ii)->m_mgub;
		p_data->strName = m_TreeChat.m_userTree.GetAt(ii)->m_name;
		AddItem(&m_List_user, ii, p_data);
	}
}

void CMyBar::CheckTreeList()
{
	if(m_treeBtn.GetCheck())
	{

		m_List_user.ShowWindow(SW_SHOW);
		m_TreeChat.ShowWindow(SW_HIDE);
	}
	else
	{
		m_List_user.ShowWindow(SW_HIDE);
		m_TreeChat.ShowWindow(SW_SHOW);
	}
}