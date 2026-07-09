// WorkSpaceBar.cpp: implementation of the CWorkSpaceBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axisWork.h"
#include "WorkSpaceBar.h"
#include "mapvar.h"
#include "mainvar.h"
#include "form_w.h"
#include <imm.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	RIGHT_GAP	3
#define	BOTTOM_GAP	25
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
//	tree에 추가 예제
//			struct _mapTree	mapTree;
//			strcpy(mapTree.mapn, "WM333300");
//			m_TreeServer.insert("WM333300", m_TreeServer.m_treeArr.Add(mapTree));
//		
//
//

CWorkSpaceBar::CWorkSpaceBar()
{
	m_pDragImg = (CImageList *) NULL;
	m_bOffline = false;
	m_bWait = false;

	m_pchTip = NULL;
	m_pwchTip = NULL;
}

CWorkSpaceBar::~CWorkSpaceBar()
{
	if(m_pwchTip != NULL)
		delete m_pwchTip;
	
	if(m_pchTip != NULL)
		delete m_pchTip;

	if (m_pDragImg)
		delete m_pDragImg;
	m_SymTbl.removeAll();
	::DeleteObject(m_hTreeBitmap);
	::DeleteObject(m_hListBitmap);
	::DeleteObject(m_hOpenBitmap);
	::DeleteObject(m_hReloadBitmap);
	::DeleteObject(m_hMapOnlyBitmap);
}

BEGIN_MESSAGE_MAP(CWorkSpaceBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CWorkSpaceBar)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_BEGINDRAG, ID_WRK_LISTSYM, OnList1BeginDrag)
	ON_NOTIFY(LVN_BEGINDRAG, ID_WRK_LISTTMP, OnList2BeginDrag)
	ON_NOTIFY(LVN_BEGINDRAG, ID_WRK_LISTSVR, OnList3BeginDrag)
	ON_NOTIFY(TVN_BEGINDRAG, ID_WRK_SYMBOL, OnSTreeBeginDrag)
	ON_BN_CLICKED(ID_WRK_MAPONLYBTN, OnMapOnly)
	ON_BN_CLICKED(ID_WRK_RELOADBTN, OnReload)
	ON_BN_CLICKED(ID_WRK_OPENBTN, OnOpenMap)
	ON_BN_CLICKED(ID_WRK_TREEBTN, OnTreeBtnClick)
	ON_BN_CLICKED(ID_WRK_DEPTHBTN, OnDepthBtnClick)
	ON_NOTIFY(NM_DBLCLK, ID_WRK_SERVER, OnServerDblClk)
	ON_NOTIFY(NM_DBLCLK, ID_WRK_CLIENT, OnClientDblClk)
	ON_NOTIFY(NM_DBLCLK, ID_WRK_TEMPLATE, OnTmplDblClk)
	ON_NOTIFY(TCN_SELCHANGE, ID_WRK_TAB, OnTabSelChange)
	ON_CBN_SELENDOK(ID_WRK_GROUPCBOTMP, OnTmplSelChange)
	ON_CBN_SELENDOK(ID_WRK_GROUPCBOSVR, OnSvrSelChange)
	ON_CBN_SELENDOK(ID_WRK_GROUPCBOSYM, OnSymGrpSelChange)
	ON_CBN_SELENDOK(ID_WRK_PROJECTCBOSYM, OnSymPrjSelChange)
	ON_NOTIFY(NM_DBLCLK, ID_WRK_LISTSVR, OnSvrDblClk)
	ON_NOTIFY(NM_CLICK, ID_WRK_LISTSVR, OnListSvrClick)
	ON_NOTIFY(NM_CLICK, ID_WRK_LISTTMP, OnListTmpClick)
	ON_NOTIFY(NM_DBLCLK, ID_WRK_LISTTMP, OnTmpDblClk)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

void CWorkSpaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CSizingControlBarG::OnPaint() for painting messages
}

int CWorkSpaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_openBtn.Create(NULL, "\nOpen", WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(53,2,73,22), this, ID_WRK_OPENBTN))
		return -1;

	if (!m_depthBtn.Create(NULL, "D", WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(76,2,96,22), this, ID_WRK_DEPTHBTN))
		return -1;

	if (!m_treeBtn.Create(NULL, "\nTree/List", WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(30,2,50,22), this, ID_WRK_TREEBTN))
		return -1;

	if (!m_ReloadBtn.Create(NULL, "\nReload", WS_CHILD | WS_VISIBLE, CRect(7,2,27,22), this, ID_WRK_RELOADBTN))
		return -1;

	if (!m_MapOnlyBtn.Create(NULL, "\nMapOnly", WS_CHILD | WS_VISIBLE, CRect(30,2,50,22), this, ID_WRK_MAPONLYBTN))
		return -1;

	if (!m_TreeFile.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_TABSTOP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS | TVS_SHOWSELALWAYS, 
		CRect(0,0,0,0), this, ID_WRK_CLIENT))
		return -1;

	if (!m_TreeServer.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_TABSTOP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 
		CRect(0,0,0,0), this, ID_WRK_SERVER))
		return -1;

	if (!m_TreeTemplate.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_TABSTOP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
		CRect(0,0,0,0), this, ID_WRK_TEMPLATE))
		return -1;

	if (!m_TreeSymbol.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_TABSTOP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
		CRect(0,0,0,0), this, ID_WRK_SYMBOL))
		return -1;
	
	if (!m_fnames.Create(WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL | ES_UPPERCASE, CRect(0,0,0,0), this, ID_WRK_FNAMES))
		return -1;

	if (!m_groupCBO_sym.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS
		,CRect(0,0,0,0), this, ID_WRK_GROUPCBOSYM ))
		return -1;

	if (!m_groupCBO_tmp.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS
		,CRect(0,0,0,0), this, ID_WRK_GROUPCBOTMP ))
		return -1;

	if (!m_groupCBO_svr.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS
		,CRect(0,0,0,0), this, ID_WRK_GROUPCBOSVR ))
		return -1;

	if (!m_projectCBO.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS
		,CRect(0,0,0,0), this, ID_WRK_PROJECTCBOSYM ))
		return -1;

	if (!m_ioCBO.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS
		,CRect(0,0,0,0), this, ID_WRK_IOCBOSYM ))
		return -1;

	if (!m_List_sym.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | LVS_REPORT | LVS_ALIGNTOP | LVS_NOCOLUMNHEADER ,
		CRect(0), this, ID_WRK_LISTSYM))
		return -1;

	if (!m_List_tmp.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | LVS_REPORT | LVS_ALIGNTOP | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SORTASCENDING,
		CRect(0), this, ID_WRK_LISTTMP))
		return -1;

	if (!m_List_svr.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | LVS_REPORT | LVS_ALIGNTOP | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SORTASCENDING,
		CRect(0), this, ID_WRK_LISTSVR))
		return -1;

	if (!m_TabCtrl.Create(WS_CHILD | WS_VISIBLE | TCS_TOOLTIPS | TCS_BOTTOM | TCS_FOCUSNEVER | TCS_FIXEDWIDTH,
		CRect(0,0,0,0), this, ID_WRK_TAB))
		return -1;
	
	if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
		if (!m_font.CreatePointFont(80, "MS Sans Serif"))
			return -1;
	if (!m_tabFont.CreateFont(12, 0, 0, 0, FW_SEMIBOLD, FALSE,
			false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "MS Sans Setif"))
		return -1;

	m_TreeServer.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	m_TreeTemplate.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	m_TreeFile.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	m_List_sym.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	m_List_svr.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	m_List_tmp.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);

	m_ReloadBtn.SetFont(&m_font);
	m_fnames.SetFont(&m_font);
	m_openBtn.SetFont(&m_font);
	m_treeBtn.SetFont(&m_font);
	m_depthBtn.SetFont(&m_font);
	m_groupCBO_sym.SetFont(&m_font);
	m_groupCBO_tmp.SetFont(&m_font);
	m_groupCBO_svr.SetFont(&m_font);
	m_projectCBO.SetFont(&m_font);
	m_ioCBO.SetFont(&m_font);

	m_treeBtn.SetCheck(false);
	m_depthBtn.SetCheck(false);
	m_TabCtrl.SetFont(&m_tabFont);

	CBitmap		cBitmap;
	if (cBitmap.Attach(LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_TABIMG),
								  IMAGE_BITMAP, 0, 0,
								  LR_DEFAULTSIZE|LR_CREATEDIBSECTION)))
	m_tabImg.Create(17, 17, ILC_COLOR24|ILC_MASK, 4, 0);
	//m_tabImg.Create(IDB_TABIMG, 16, 0 , RGB(0, 255, 255));
	m_tabImg.Add(&cBitmap, RGB(0, 255, 255));
	cBitmap.Detach();
	m_TabCtrl.SetImageList(&m_tabImg);

	m_hTreeBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_TREEBTN));
	m_treeBtn.m_hBitmap = m_hTreeBitmap;

	m_hListBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_LISTBTN));
	m_treeBtn.m_hBitmap2 = m_hListBitmap;

	m_hTwoDepthBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_TWODEPTHBTN));
	m_depthBtn.m_hBitmap = m_hTwoDepthBitmap;

	m_hOpenBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_OPENBTN));
	m_openBtn.m_hBitmap = m_hOpenBitmap;

	m_hMapOnlyBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_MAPONLYBTN));
	m_MapOnlyBtn.m_hBitmap = m_hMapOnlyBitmap;

	m_hReloadBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_RELOADBTN));
	m_ReloadBtn.m_hBitmap = m_hReloadBitmap;

	m_fnames.LimitText(8);
	initialize();

	return 0;
}

void CWorkSpaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarG::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(rc);
	rc.DeflateRect(3, 23, RIGHT_GAP - 2, 0);
	m_TabCtrl.MoveWindow(rc);
	/*if (cx < 380)
		m_TabCtrl.SetItemSize(CSize(cx / 4 - 3, 18));
	else
		m_TabCtrl.SetItemSize(CSize(92, 18));*/

	switch(m_TabCtrl.GetCurSel())
	{
	case 0: // Server
		{
			GetClientRect(rc);
			if (m_treeBtn.GetCheck())
			{
				rc.DeflateRect(5, 25, RIGHT_GAP, 0);
				rc.bottom = 120;
				m_groupCBO_svr.MoveWindow(rc);
				GetClientRect(rc);
				rc.DeflateRect(5, 47, RIGHT_GAP, BOTTOM_GAP);
				m_List_svr.MoveWindow(rc);
				m_List_svr.SetColumnWidth(-1, rc.Width());
			}
			else
			{
				rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
				m_TreeServer.MoveWindow(rc);
			}
		}
		break;
	case 1: // Client
		{
			GetClientRect(rc);
			rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
			m_TreeFile.MoveWindow(rc);
		}
		break;
	case 2: // Symbol
		{
			if (m_treeBtn.GetCheck())
			{
				GetClientRect(rc);
				rc.DeflateRect(5, 45, RIGHT_GAP, 0);
				rc.bottom = 150;
				m_groupCBO_sym.MoveWindow(rc);
				rc.DeflateRect(0, -20, 80, 20);
				m_projectCBO.MoveWindow(rc);
				rc.left = rc.right + 3;
				rc.right = rc.left + 77;
				m_ioCBO.MoveWindow(rc);
				GetClientRect(rc);
				rc.DeflateRect(5, 67, RIGHT_GAP, BOTTOM_GAP);
				m_List_sym.MoveWindow(rc);
				m_List_sym.SetColumnWidth(-1, rc.Width());
			}
			else
			{
				GetClientRect(&rc);
				rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
				rc.left = rc.right - 77;
				rc.bottom = rc.top + 20;
				m_ioCBO.MoveWindow(rc);
				GetClientRect(&rc);
				rc.DeflateRect(5, 45, RIGHT_GAP, BOTTOM_GAP);
				m_TreeSymbol.MoveWindow(rc);
			}
		}
		break;
	case 3: // Template
		{
			GetClientRect(rc);

			if (m_treeBtn.GetCheck())
			{
				rc.DeflateRect(5, 25, RIGHT_GAP, 0);
				rc.bottom = 120;
				m_groupCBO_tmp.MoveWindow(rc);
				GetClientRect(rc);
				rc.DeflateRect(5, 47, RIGHT_GAP, BOTTOM_GAP);
				m_List_tmp.MoveWindow(rc);
				m_List_tmp.SetColumnWidth(-1, rc.Width());
			}
			else
			{
				rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
				m_TreeTemplate.MoveWindow(rc);
				m_TreeTemplate.Invalidate();
			}
		}
		break;
	}
}

void CWorkSpaceBar::initialize()
{
	m_TabCtrl.InsertItem(0, "", 0);
	m_TabCtrl.InsertItem(1, "", 1);
	m_TabCtrl.InsertItem(2, "", 2);
	m_TabCtrl.InsertItem(3, "", 3);
	m_TabCtrl.SetItemSize(CSize(40, 20));

	m_TreeFile.PopulateTree();
	m_TreeFile.SetAllowDragDrop(TRUE);
	m_TreeFile.SetAutoRefresh(FALSE);
	m_TreeFile.SetFileHideFlags(FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | 
		FILE_ATTRIBUTE_OFFLINE);

	DWORD dwStyle = m_List_sym.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_List_sym.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	m_List_tmp.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	m_List_svr.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	m_List_svr.EnableToolTips();
	m_List_tmp.EnableToolTips();
	m_List_sym.EnableToolTips();

	addColumn(&m_List_sym, "", 0, 150);
	addColumn(&m_List_sym, "", 1, 150);
	addColumn(&m_List_tmp, "", 0, 150);
	addColumn(&m_List_svr, "", 0, 150);

	m_ioCBO.SetItemData(m_ioCBO.AddString("Input"), EIO_INPUT);
	m_ioCBO.SetItemData(m_ioCBO.AddString("Output"), EIO_OUTPUT);
	m_ioCBO.SetCheck(0, FALSE);

	m_SymTbl.m_root = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	m_SymTbl.m_bOffline = m_bOffline;
	m_SymTbl.loadProject();
	m_SymTbl.loadFile();
	m_SymTbl.fillCombo(&m_projectCBO);
	m_SymTbl.fillTree(&m_TreeSymbol);
	if (m_projectCBO.GetCount())
	{
		m_projectCBO.SetCurSel(0);
		OnSymPrjSelChange();
	}

	m_TreeServer.m_nTwoDepth   = m_nSvrDepth;
	m_TreeTemplate.m_nTwoDepth = m_nTmpDepth;
}

void CWorkSpaceBar::OnTabSelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	CRect rc;
	m_TreeSymbol.ShowWindow(SW_HIDE);
	m_TreeServer.ShowWindow(SW_HIDE);
	m_TreeFile.ShowWindow(SW_HIDE);
	m_TreeTemplate.ShowWindow(SW_HIDE);
	m_fnames.ShowWindow(SW_HIDE);
	m_openBtn.ShowWindow(SW_HIDE);
	m_groupCBO_sym.ShowWindow(SW_HIDE);
	m_groupCBO_svr.ShowWindow(SW_HIDE);
	m_groupCBO_tmp.ShowWindow(SW_HIDE);
	m_projectCBO.ShowWindow(SW_HIDE);
	m_ioCBO.ShowWindow(SW_HIDE);
	m_List_sym.ShowWindow(SW_HIDE);
	m_List_svr.ShowWindow(SW_HIDE);
	m_List_tmp.ShowWindow(SW_HIDE);
	m_treeBtn.ShowWindow(SW_SHOW);
	m_depthBtn.ShowWindow(SW_HIDE);
	m_MapOnlyBtn.ShowWindow(SW_HIDE);

	switch(m_TabCtrl.GetCurSel())
	{
	case 0:	//	server
		
		GetClientRect(&rc);
		if (!m_treeBtn.GetCheck())
		{
			m_depthBtn.ShowWindow(SW_SHOW);
			m_depthBtn.SetCheck(m_TreeServer.m_nTwoDepth != 3);
			m_openBtn.ShowWindow(SW_SHOW);
			m_TreeServer.ShowWindow(SW_SHOW);
			rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
			m_TreeServer.MoveWindow(rc);
			m_TreeServer.Invalidate();
		}
		else
		{
			m_groupCBO_svr.ShowWindow(SW_SHOW);
			m_List_svr.ShowWindow(SW_SHOW);
			rc.DeflateRect(5, 25, RIGHT_GAP, 0);
			rc.bottom = 120;
			m_groupCBO_svr.MoveWindow(rc);
			GetClientRect(rc);
			rc.DeflateRect(5, 47, RIGHT_GAP, BOTTOM_GAP);
			m_List_svr.MoveWindow(rc);
			m_List_svr.SetColumnWidth(-1, rc.Width());
		}
		break;

	case 1:	//	client
		m_treeBtn.ShowWindow(SW_HIDE);
		m_TreeFile.ShowWindow(SW_SHOW);
		m_MapOnlyBtn.ShowWindow(SW_SHOW);
		
		GetClientRect(&rc);
		rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
		m_TreeFile.MoveWindow(rc);
		m_TreeFile.Invalidate();
		break;

	case 2:	//	Symbol
		m_ioCBO.ShowWindow(SW_SHOW);
		if (!m_treeBtn.GetCheck())
		{
			m_TreeSymbol.ShowWindow(SW_SHOW);
			GetClientRect(&rc);
			rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
			rc.left = rc.right - 77;
			rc.bottom = rc.top + 80;
			m_ioCBO.MoveWindow(rc);
			GetClientRect(&rc);
			rc.DeflateRect(5, 45, RIGHT_GAP, BOTTOM_GAP);
			m_TreeSymbol.MoveWindow(rc);
			m_TreeSymbol.Invalidate();
		}
		else
		{
			m_groupCBO_sym.ShowWindow(SW_SHOW);
			m_List_sym.ShowWindow(SW_SHOW);
			m_projectCBO.ShowWindow(SW_SHOW);
			
			GetClientRect(&rc);
			rc.DeflateRect(5, 45, RIGHT_GAP, 0);
			rc.bottom = 150;
			m_groupCBO_sym.MoveWindow(rc);
			
			rc.DeflateRect(0, -20, 80, 20);
			m_projectCBO.MoveWindow(rc);
			rc.left = rc.right + 3;
			rc.right = rc.left + 77;
			m_ioCBO.MoveWindow(rc);
			
			GetClientRect(&rc);
			rc.DeflateRect(5, 67, RIGHT_GAP, BOTTOM_GAP);
			m_List_sym.MoveWindow(rc);
		}
		break;

	case 3: //	Template
		GetClientRect(&rc);
		if (!m_treeBtn.GetCheck())
		{
			m_depthBtn.ShowWindow(SW_SHOW);
			m_depthBtn.SetCheck(m_TreeTemplate.m_nTwoDepth != 3);
			m_openBtn.ShowWindow(SW_SHOW);
			m_TreeTemplate.ShowWindow(SW_SHOW);
			rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
			m_TreeTemplate.MoveWindow(rc);
			m_TreeTemplate.Invalidate();
		}
		else
		{
			m_treeBtn.ShowWindow(SW_SHOW);
			m_groupCBO_tmp.ShowWindow(SW_SHOW);
			m_List_tmp.ShowWindow(SW_SHOW);
			
			rc.DeflateRect(5, 25, RIGHT_GAP, 0);
			rc.bottom = 120;
			m_groupCBO_tmp.MoveWindow(rc);
			GetClientRect(&rc);
			rc.DeflateRect(5, 47, RIGHT_GAP, BOTTOM_GAP);
			m_List_tmp.MoveWindow(rc);
		}
		break;
	}
}

void CWorkSpaceBar::SetMainInfo(CString dir, CString path, int child, bool request)
{
	m_TreeServer.Initialize(dir, path, child, request);
	m_TreeTemplate.Initialize(dir, path, child, request, true);

	OnReload();
	m_TabCtrl.SetCurSel(3);
	
	OnReload();
	if (!m_bOffline)
		m_TabCtrl.SetCurSel(0);
	else
		m_TabCtrl.SetCurSel(1);
	OnTabSelChange(NULL, NULL);
}

void CWorkSpaceBar::OnServerDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_TreeServer.m_count = m_TreeServer.GetSelectedItemCount();
	if (m_TreeServer.m_count == 0)
	{
		*pResult = 0;
		return;
	}
	m_TreeServer.m_index = 0;
	//m_TreeServer.sendData(false);
	m_TreeServer.OpenMap();
	m_TreeServer.m_files.RemoveAll();
	*pResult = 0;
}

void CWorkSpaceBar::OnTmplDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_TreeTemplate.m_count = m_TreeTemplate.GetSelectedItemCount();
	if (m_TreeTemplate.m_count == 0)
	{
		*pResult = 0;
		return;
	}
	m_TreeTemplate.m_index = 0;
	m_TreeTemplate.m_bOpen = true;
	//m_TreeTemplate.sendData(false);
	m_TreeTemplate.OpenMap();
	m_TreeTemplate.m_files.RemoveAll();
	*pResult = 0;
}

void CWorkSpaceBar::OnSvrDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_List_svr.GetFirstSelectedItemPosition();
	int idx = m_List_svr.GetItemData(m_List_svr.GetNextSelectedItem(pos));
	m_TreeServer.OpenMap(idx);	
	*pResult = 0;
}


void CWorkSpaceBar::OnTmpDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_List_tmp.GetFirstSelectedItemPosition();
	int idx = m_List_tmp.GetItemData(m_List_svr.GetNextSelectedItem(pos));
	m_TreeTemplate.OpenMap(idx);	
	*pResult = 0;
}

void CWorkSpaceBar::OnListSvrClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_List_svr.GetFirstSelectedItemPosition();
	int idx;
	if (pos)
	{
		idx = m_List_svr.GetItemData(m_List_svr.GetNextSelectedItem(pos));
		if (idx >= 0)
		{
			m_path = m_TreeServer.getNameOfIdx(idx);
			//AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_PREVIEW, (long)m_path.operator LPCTSTR());
			SetTimer(1, 500, NULL);
		}
	}
	*pResult = 0;
}

void CWorkSpaceBar::OnListTmpClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_List_tmp.GetFirstSelectedItemPosition();
	int idx;
	if (pos)
	{
		idx = m_List_tmp.GetItemData(m_List_tmp.GetNextSelectedItem(pos));
		if (idx >= 0)
		{
			m_path = m_TreeTemplate.getNameOfIdx(idx);
			//AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_PREVIEW, (long)m_path.operator LPCTSTR());
			SetTimer(1, 500, NULL);
		}
	}
	*pResult = 0;
}

void CWorkSpaceBar::OnClientDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}


void CWorkSpaceBar::OnOpenMap()
{
	switch(m_TabCtrl.GetCurSel())
	{
	case 0:	//	server
		if (!m_bOffline)
		{
			AfxGetApp()->DoWaitCursor(1);
			CString sName;
			m_fnames.GetWindowText(sName);
			m_TreeServer.m_files.RemoveAll();
			while(!sName.IsEmpty())
			{
				CString name = Parser(sName, ";");
				if (!name.IsEmpty())
					m_TreeServer.AddFiles(name);
			}
			m_TreeServer.OpenMap();
			AfxGetApp()->DoWaitCursor(-1);
		}
		break;
	case 1:	//	client
		/*m_path = m_TreeFile.ItemToPath(m_TreeFile.GetSelectedItem());
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_FILEOPEN, (long)m_path.operator LPCTSTR());*/
		break;
	case 3:	//	template
		if (!m_bOffline)
		{
			AfxGetApp()->DoWaitCursor(1);
			CString sName;
			m_fnames.GetWindowText(sName);
			m_TreeTemplate.m_files.RemoveAll();
			while(!sName.IsEmpty())
			{
				CString name = Parser(sName, ";");
				if (!name.IsEmpty())
				{
					m_TreeTemplate.m_name = name;
					m_TreeTemplate.OpenMap();
				}
			}
			AfxGetApp()->DoWaitCursor(-1);
		}
		break;
	}
}


void CWorkSpaceBar::OnReload()
{
	AfxGetApp()->DoWaitCursor(1);
	switch(m_TabCtrl.GetCurSel())
	{
	case 0: //	server
		if (!m_bOffline)
		{
			if (m_TreeServer.openList(&m_groupCBO_svr))
			{
				if (m_groupCBO_svr.GetCount())
				{
					m_groupCBO_svr.SetCurSel(0);
					OnSvrSelChange();
				}
			}
			else
			{
				m_bOffline = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETOFFLINE, 1);
			}
		}
		break;
	case 1: //	client
		{
			m_TreeFile.Refresh();
		}
		break;
	case 2:	//	Symbol
		m_SymTbl.m_bReceive = !m_bOffline;
		m_SymTbl.loadProject();
		m_SymTbl.loadFile();
		m_SymTbl.fillCombo(&m_projectCBO);
		m_SymTbl.fillTree(&m_TreeSymbol);
		if (m_projectCBO.GetCount())
		{
			m_projectCBO.SetCurSel(0);
			OnSymPrjSelChange();
		}
		break;
	case 3: //	Template
		if (!m_bOffline)
		{
			if (m_TreeTemplate.openList(&m_groupCBO_tmp))
			{
				if (m_groupCBO_tmp.GetCount())
				{
					m_groupCBO_tmp.SetCurSel(0);
					OnTmplSelChange();
				}
			}
			else
			{
				m_bOffline = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETOFFLINE, 1);
			}
		}
		break;
	}
	AfxGetApp()->DoWaitCursor(-1);
}

LRESULT CWorkSpaceBar::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 1:	//	server selection names
		{
			CString sFnames;
			/*if (m_TreeServer.m_files.GetSize())
				sFnames = m_TreeServer.m_files.GetAt(0);*/
			int cnt = (m_TabCtrl.GetCurSel() == 0?m_TreeServer.m_files.GetSize():m_TreeTemplate.m_files.GetSize());

			for (int ii = 0; ii < cnt; ii++)
			{
				if (ii == 0)
				{
					if (m_TabCtrl.GetCurSel() == 0)
						m_path = m_TreeServer.setMapPath(m_TreeServer.m_files.GetAt(ii));
					else
						m_path = m_TreeTemplate.setMapPath(m_TreeTemplate.m_files.GetAt(ii));
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_PREVIEW, (long)m_path.operator LPCTSTR());
				}
				if (m_TabCtrl.GetCurSel() == 0)
					sFnames += m_TreeServer.m_files.GetAt(ii);
				else
					sFnames += m_TreeTemplate.m_files.GetAt(ii);
				if (ii != cnt - 1)
					sFnames += ";";
			}
			if (!sFnames.IsEmpty())
				m_fnames.SetWindowText(sFnames);
		}
		break;
	}
	return 0;
}

void CWorkSpaceBar::OnSymGrpSelChange()
{
	if (m_groupCBO_sym.GetCurSel() != CB_ERR)
		m_SymTbl.fillList(&m_List_sym, (CsymbolSET*)m_groupCBO_sym.GetItemData(m_groupCBO_sym.GetCurSel()));
}

void CWorkSpaceBar::OnSymPrjSelChange()
{
	if (m_projectCBO.GetCurSel() != CB_ERR)
	{
		m_SymTbl.fillCombo((Cproject*)m_projectCBO.GetItemData(m_projectCBO.GetCurSel()), 
			&m_groupCBO_sym);
		if (m_groupCBO_sym.GetCount())
		{
			m_groupCBO_sym.SetCurSel(0);
			OnSymGrpSelChange();
		}
	}
}

void CWorkSpaceBar::OnTmplSelChange()
{
	if (m_groupCBO_tmp.GetCurSel() != CB_ERR)
		m_TreeTemplate.fillList(&m_List_tmp, m_groupCBO_tmp.GetItemData(m_groupCBO_tmp.GetCurSel()));
	else
		m_TreeTemplate.fillList(&m_List_tmp, -1);
}

void CWorkSpaceBar::OnSvrSelChange()
{
	if (m_groupCBO_svr.GetCurSel() != CB_ERR)
		m_TreeServer.fillList(&m_List_svr, m_groupCBO_svr.GetItemData(m_groupCBO_svr.GetCurSel()));
	else
		m_TreeServer.fillList(&m_List_svr, -1);
}

BOOL CWorkSpaceBar::addColumn(CListCtrl *LC, LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) str;
	lvc.cx      = LC->GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return LC->InsertColumn(item, &lvc);
}

void CWorkSpaceBar::OnList1BeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;
	if (m_List_sym.GetSelectedCount() <= 0)
		return;

	CPoint	pt;
	m_pDragImg = createDragImage(&m_List_sym, &pt);
	if (m_pDragImg == (CImageList *)NULL)
		return;

	CPoint	startpt = pNMListView->ptAction;
	startpt -= pt;
	AfxGetMainWnd()->PostMessage(WM_STATUS, AWS_SDRAG, (LPARAM) 0);

	m_pDragImg->BeginDrag(0, CPoint(0,0)/*startpt*/);
	m_pDragImg->DragEnter(GetDesktopWindow(), startpt);
	AfxGetMainWnd()->SetCapture();
}

void CWorkSpaceBar::OnList2BeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;
	if (m_List_tmp.GetSelectedCount() <= 0)
		return;

	CPoint	pt;
	m_pDragImg = createDragImage(&m_List_tmp, &pt);
	if (m_pDragImg == (CImageList *)NULL)
		return;

	CPoint	startpt = pNMListView->ptAction;
	startpt -= pt;
	AfxGetMainWnd()->PostMessage(WM_STATUS, AWS_TDRAG, (LPARAM) 0);

	POSITION pos = m_List_tmp.GetFirstSelectedItemPosition();
	if (pos)
		//m_TreeTemplate.setFileName(m_groupCBO_tmp.GetItemData(m_groupCBO_tmp.GetCurSel()), m_List_tmp.GetItemText(m_List_tmp.GetNextSelectedItem(pos), 0));
		m_TreeTemplate.setFileName(m_List_tmp.GetItemData(m_List_tmp.GetNextSelectedItem(pos)));
	m_pDragImg->BeginDrag(0, CPoint(0, 0)/*startpt*/);
	m_pDragImg->DragEnter(GetDesktopWindow(), startpt);
	AfxGetMainWnd()->SetCapture();
}

void CWorkSpaceBar::OnList3BeginDrag(NMHDR *pNMHDR, LRESULT *pResult) // server map
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;
	if (m_List_svr.GetSelectedCount() <= 0)
		return;

	CPoint	pt;
	m_pDragImg = createDragImage(&m_List_svr, &pt);
	if (m_pDragImg == (CImageList *)NULL)
		return;

	CPoint	startpt = pNMListView->ptAction;
	startpt -= pt;
	AfxGetMainWnd()->PostMessage(WM_STATUS, AWS_MDRAG, (LPARAM) 0);

	POSITION pos = m_List_svr.GetFirstSelectedItemPosition();
	if (pos)
		m_TreeServer.m_files.RemoveAll();
	while(pos)
	{
		int nItem = m_List_svr.GetNextSelectedItem(pos);
		m_TreeServer.setFileName(m_List_svr.GetItemData(nItem));
	}
	m_pDragImg->BeginDrag(0, CPoint(0, 0)/*startpt*/);
	m_pDragImg->DragEnter(GetDesktopWindow(), startpt);
	AfxGetMainWnd()->SetCapture();
}

CImageList* CWorkSpaceBar::createDragImage(CListCtrl *LC, LPPOINT lpPoint)
{
	if (LC->GetSelectedCount() <= 0)
		return NULL;

	CRect	singleRc, completeRc;
	int	width, itop, ibottom;

	LC->GetClientRect(singleRc);
	completeRc.SetRectEmpty();
	width = singleRc.Width();
	itop  = LC->GetTopIndex() - 1;
	ibottom = LC->GetTopIndex() + LC->GetCountPerPage() - 1;

	if (ibottom > (LC->GetItemCount() - 1))
		ibottom = LC->GetItemCount()-1;

	while ((itop = LC->GetNextItem(itop, LVNI_SELECTED)) != -1)
	{
		if (itop > ibottom)
			break; 

		LC->GetItemRect(itop, singleRc, LVIR_BOUNDS);
		singleRc.left  = max(0, singleRc.left);
		singleRc.right = min(width, singleRc.right);
		completeRc.UnionRect(completeRc, singleRc);
	}
		
	CClientDC dcClient(this);
	CDC	memDC;
	CBitmap bitmap;

	if (!memDC.CreateCompatibleDC(&dcClient))
		return NULL;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, completeRc.Width(), completeRc.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
 	memDC.FillSolidRect(0, 0, completeRc.Width(), completeRc.Height(), RGB(255, 255,255));

	itop = LC->GetTopIndex() - 1;
	while ((itop = LC->GetNextItem(itop, LVNI_SELECTED)) != -1)
	{	
		if (itop > ibottom)
			return NULL;

		CPoint	pt;
		CImageList* pSingleImageList = LC->CreateDragImage(itop, &pt);
		if (pSingleImageList)
		{
			LC->GetItemRect(itop, singleRc, LVIR_BOUNDS);
			pSingleImageList->Draw(&memDC, 0, CPoint(singleRc.left - completeRc.left,
						singleRc.top - completeRc.top), ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
		}
	}

 	memDC.SelectObject(pOldMemDCBitmap);
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(completeRc.Width(), completeRc.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&bitmap, RGB(0, 255, 0)); 
	bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = completeRc.left;
		lpPoint->y = completeRc.top;
	}

	return pCompleteImageList;
}

CImageList* CWorkSpaceBar::createDragImage(CSymbolTreeCtrl *TC, LPPOINT lpPoint)
{
	if (TC->GetSelectedItemCount() <= 0)
		return NULL;

	CRect singleRc, completeRc = CRect(0, 0, 0, 0);
	
	HTREEITEM hitem = TC->getFirstSelectedItem();
	
	while (hitem)
	{
		if (TC->ItemHasChildren(hitem))
		{
			hitem = TC->getNextSelectedItem(hitem);
			continue;
		}

		TC->GetItemRect(hitem, &singleRc, TRUE);
		singleRc.InflateRect(0, 0, 25, 0);
		if (completeRc.IsRectEmpty())
			completeRc = singleRc;
		else
		{
			completeRc.bottom += singleRc.Height();
			completeRc.right = max(singleRc.right, completeRc.right);
		}

		hitem = TC->getNextSelectedItem(hitem);
	}

	CClientDC dcClient(this);
	CDC	memDC;
	CBitmap bitmap;
	int	nCnt = 0;

	if (!memDC.CreateCompatibleDC(&dcClient))
		return NULL;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, completeRc.Width(), completeRc.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
 	memDC.FillSolidRect(0, 0, completeRc.Width(), completeRc.Height(), RGB(255, 255,255));

	hitem = TC->getFirstSelectedItem();

	while (hitem)
	{
		if (TC->ItemHasChildren(hitem))
		{
			hitem = TC->getNextSelectedItem(hitem);
			continue;
		}

		CImageList* pSingleImageList = TC->CreateDragImage(hitem);
		if (pSingleImageList)
		{
			TC->GetItemRect(hitem, &singleRc, TRUE);
			pSingleImageList->Draw(&memDC, 0, CPoint(0, singleRc.Height() * nCnt), ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
			nCnt++;
		}
		hitem = TC->getNextSelectedItem(hitem);
	}

	memDC.SelectObject(pOldMemDCBitmap);
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(completeRc.Width(), completeRc.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&bitmap, RGB(0, 255, 0)); 
	bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = completeRc.left;
		lpPoint->y = completeRc.top;
	}

	return pCompleteImageList;
}

int CWorkSpaceBar::GetSelectedCount()
{
	BYTE io;
	if (m_ioCBO.GetCheck(0))
		io = EIO_INPUT;
	else if (m_ioCBO.GetCheck(1))
		io = EIO_OUTPUT;
	if (m_ioCBO.GetCheck(0) && m_ioCBO.GetCheck(1))
		io = EIO_INOUT;
	if (!m_treeBtn.GetCheck())
		return m_SymTbl.GetSelectedCount(&m_TreeSymbol, io);
	return m_SymTbl.GetSelectedCount(&m_List_sym, io);
}

void CWorkSpaceBar::DeleteDragImg()
{
	m_pDragImg->DragLeave(GetDesktopWindow());
	m_pDragImg->EndDrag();
	m_pDragImg->DeleteImageList();
	if (m_pDragImg)
	{
		delete m_pDragImg;
		m_pDragImg = NULL;
	}
}

void CWorkSpaceBar::OnTreeBtnClick()
{
	CRect rc;
	if (m_treeBtn.GetCheck())
	{
		m_treeBtn.SetCheck(false);
		m_fnames.ShowWindow(SW_HIDE);
		switch (m_TabCtrl.GetCurSel())
		{
		case 0:		// server
			m_openBtn.ShowWindow(SW_SHOW);
			m_depthBtn.ShowWindow(SW_SHOW);
			m_depthBtn.SetCheck(m_TreeServer.m_nTwoDepth != 3);
			m_groupCBO_svr.ShowWindow(SW_HIDE);
			m_List_svr.ShowWindow(SW_HIDE);
			m_TreeServer.ShowWindow(SW_SHOW);
			GetClientRect(rc);
			rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
			m_TreeServer.MoveWindow(rc);
			m_TreeServer.Invalidate();
			break;
		case 2:		// symbol
			m_TreeSymbol.ShowWindow(SW_SHOW);
			m_groupCBO_sym.ShowWindow(SW_HIDE);
			m_projectCBO.ShowWindow(SW_HIDE);
			m_List_sym.ShowWindow(SW_HIDE);
			GetClientRect(rc);
			rc.DeflateRect(5, 45, RIGHT_GAP, BOTTOM_GAP);
			m_TreeSymbol.MoveWindow(rc);
			m_TreeSymbol.Invalidate();
			break;
		case 3:		// template
			m_openBtn.ShowWindow(SW_SHOW);
			m_depthBtn.ShowWindow(SW_SHOW);
			m_depthBtn.SetCheck(m_TreeTemplate.m_nTwoDepth != 3);
			m_groupCBO_tmp.ShowWindow(SW_HIDE);
			m_List_tmp.ShowWindow(SW_HIDE);
			m_TreeTemplate.ShowWindow(SW_SHOW);
			GetClientRect(rc);
			rc.DeflateRect(5, 25, RIGHT_GAP, BOTTOM_GAP);
			m_TreeTemplate.MoveWindow(rc);
			m_TreeTemplate.Invalidate();
			break;
		}
	}
	else
	{
		m_treeBtn.SetCheck(true);
		m_fnames.ShowWindow(SW_HIDE);
		
		GetClientRect(rc);
		rc.DeflateRect(5, 25, 5, 0);
		rc.bottom = 120;
		
		switch (m_TabCtrl.GetCurSel())
		{
		case 0:		// server
			m_openBtn.ShowWindow(SW_HIDE);
			m_depthBtn.ShowWindow(SW_HIDE);
			m_TreeServer.ShowWindow(SW_HIDE);
			m_groupCBO_svr.ShowWindow(SW_SHOW);
			m_List_svr.ShowWindow(SW_SHOW);
			m_groupCBO_svr.MoveWindow(rc);
			GetClientRect(rc);
			rc.DeflateRect(5, 47, RIGHT_GAP, BOTTOM_GAP);
			m_List_svr.MoveWindow(rc);
			m_List_svr.SetColumnWidth(-1, rc.Width());
			break;
		case 2:		// symbol
			m_groupCBO_sym.ShowWindow(SW_SHOW);
			m_projectCBO.ShowWindow(SW_SHOW);
			m_List_sym.ShowWindow(SW_SHOW);
			m_TreeSymbol.ShowWindow(SW_HIDE);
			GetClientRect(rc);
			rc.DeflateRect(5, 45, RIGHT_GAP, 0);
			rc.bottom = 150;
			m_groupCBO_sym.MoveWindow(rc);
			rc.DeflateRect(0, -20, 80, 20);
			m_projectCBO.MoveWindow(rc);
			rc.left = rc.right + 3;
			rc.right = rc.left + 77;
			m_ioCBO.MoveWindow(rc);
			GetClientRect(rc);
			rc.DeflateRect(5, 67, RIGHT_GAP, BOTTOM_GAP);
			m_List_sym.MoveWindow(rc);
			m_List_sym.SetColumnWidth(-1, rc.Width());
			break;
		case 3:		// tempate
			m_openBtn.ShowWindow(SW_HIDE);
			m_depthBtn.ShowWindow(SW_HIDE);
			m_TreeTemplate.ShowWindow(SW_HIDE);
			m_groupCBO_tmp.ShowWindow(SW_SHOW);
			m_List_tmp.ShowWindow(SW_SHOW);
			m_groupCBO_tmp.MoveWindow(rc);
			GetClientRect(rc);
			rc.DeflateRect(5, 47, RIGHT_GAP, BOTTOM_GAP);
			m_List_tmp.MoveWindow(rc);
			m_List_tmp.SetColumnWidth(-1, rc.Width());
			break;
		}
	}
}

CString CWorkSpaceBar::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CWorkSpaceBar::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case 1:
		KillTimer(nIDEvent);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_PREVIEW, (long)m_path.operator LPCTSTR());
		break;
	}	
	CSizingControlBarG::OnTimer(nIDEvent);
}


BOOL CWorkSpaceBar::PreTranslateMessage(MSG* pMsg) 
{
	CRect rc;
	if (pMsg->message == WM_LBUTTONDOWN)
		ChangeHangulMode(this->m_hWnd, false);
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (m_fnames.IsWindowVisible())
				m_fnames.ShowWindow(SW_HIDE);
			if (m_fnames.GetWindowTextLength() == 8 && !m_bOffline)
			{
				CString sName;
				m_fnames.GetWindowText(sName);
				AfxGetApp()->DoWaitCursor(1);

				if (m_TabCtrl.GetCurSel() == 0)
					m_TreeServer.OpenMap(sName);
				else if (m_TabCtrl.GetCurSel() == 3)
					m_TreeTemplate.OpenMap(sName);

				AfxGetApp()->DoWaitCursor(-1);
			}
		}
		
		if ((::GetKeyState(VK_CONTROL) & 0x8000))
		{
			if (pMsg->wParam == 'c' || pMsg->wParam == 'C')
				m_fnames.Copy();
			else if (pMsg->wParam == 'v' || pMsg->wParam == 'V')
			{
				if (!m_fnames.IsWindowVisible())
				{
					if (m_treeBtn.GetCheck())
						rc = CRect(10, 50, 90, 70);
					else
						rc = CRect(10, 30, 90, 50);
					m_fnames.MoveWindow(rc);
					m_fnames.SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_DRAWFRAME | SWP_FRAMECHANGED);
					m_fnames.ShowWindow(SW_SHOW);
					m_fnames.SetFocus();
				}
				m_fnames.Paste();
			}
			else if (pMsg->wParam == 'x' || pMsg->wParam == 'X')
			{
				if (!m_fnames.IsWindowVisible())
				{
					if (m_treeBtn.GetCheck())
						rc = CRect(10, 50, 90, 70);
					else
						rc = CRect(10, 30, 90, 50);
					m_fnames.MoveWindow(rc);
					m_fnames.SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_DRAWFRAME | SWP_FRAMECHANGED);
					m_fnames.ShowWindow(SW_SHOW);
					m_fnames.SetFocus();
				}
				m_fnames.Cut();
			}
			else if (pMsg->wParam == 'z' || pMsg->wParam == 'Z')
				m_fnames.Undo();
		}

		if (!(::GetKeyState(VK_CONTROL) & 0x8000) && 
			isalnum(pMsg->wParam) && 
			!m_fnames.IsWindowVisible() &&
			(m_TabCtrl.GetCurSel() == 0 || m_TabCtrl.GetCurSel() == 3) &&
			!m_bOffline)
		{
			if (pMsg->wParam >= 0x70 && pMsg->wParam <= 0x7B)
				return CSizingControlBarG::PreTranslateMessage(pMsg);
			if (m_treeBtn.GetCheck())
				rc = CRect(10, 50, 90, 70);
			else
				rc = CRect(10, 30, 90, 50);
			m_fnames.MoveWindow(rc);
			m_fnames.SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_DRAWFRAME | SWP_FRAMECHANGED);
			m_fnames.ShowWindow(SW_SHOW);
			m_fnames.SetWindowText("");
			m_fnames.SetFocus();
			m_fnames.PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		}
	}

	return CSizingControlBarG::PreTranslateMessage(pMsg);
}

BOOL CWorkSpaceBar::OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText = "";
	UINT nID = pNMHDR->idFrom;
	
	// Do not process the message from built in tooltip 
	if( nID == (UINT)m_TabCtrl.m_hWnd &&
		(( pNMHDR->code == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND ) ||
		( pNMHDR->code == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND ) ) )
		return FALSE;
	
	// Get the mouse position
	const MSG* pMessage;
	CPoint pt;
	pMessage = GetCurrentMessage();
	ASSERT ( pMessage );
	pt = pMessage->pt;
	m_TabCtrl.ScreenToClient( &pt );
	
	TCHITTESTINFO	hittestInfo;
	hittestInfo.pt = pt;
	hittestInfo.flags = TCHT_ONITEM;

	int nSelItem = m_TabCtrl.HitTest(&hittestInfo);
	//nSelItem = m_TabCtrl.GetCurSel();

	if (hittestInfo.flags & TCHT_ONITEM)
	{		
		switch(nSelItem)
		{
		case 0:
			strTipText = "Server";
			break;
		case 1:
			strTipText = "Client";
			break;
		case 2:
			strTipText = "Symbols";
			break;
		case 3:
			strTipText = "Template";
			break;
		}
	}

#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip != NULL)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()+1];
		lstrcpyn(m_pchTip, strTipText, strTipText.GetLength() + 1);
		m_pchTip[strTipText.GetLength()] = 0;
		pTTTW->lpszText = (WCHAR*)m_pchTip;
	}
	else
	{
		if(m_pwchTip != NULL)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()+1];
		_mbstowcsz(m_pwchTip, strTipText, strTipText.GetLength() + 1);
		m_pwchTip[strTipText.GetLength()] = 0; // end of text
		pTTTW->lpszText = (WCHAR*)m_pwchTip;
	}
#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip != NULL)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()+1];
		_wcstombsz(m_pchTip, strTipText, strTipText.GetLength() + 1);
		m_pchTip[strTipText.GetLength()] = 0; // end of text
		pTTTA->lpszText = (LPTSTR)m_pchTip;
	}
	else
	{
		if(m_pwchTip != NULL)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()+1];
		lstrcpyn(m_pwchTip, strTipText, strTipText.GetLength() + 1);
		m_pwchTip[strTipText.GetLength()] = 0;
		pTTTA->lpszText = (LPTSTR) m_pwchTip;
	}
#endif
	*pResult = 0;
	
	return TRUE;    // message was handled
}


void CWorkSpaceBar::ChangeHangulMode(HWND hWnd, bool bHangle)
{
	DWORD	dwConversion, dwSentence;

	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (bHangle)
			ImmSetConversionStatus(hImc, dwConversion | IME_CMODE_HANGEUL, dwSentence);
		else
			ImmSetConversionStatus(hImc, dwConversion & ~IME_CMODE_HANGEUL, dwSentence);
	}
}

void CWorkSpaceBar::OnMapOnly()
{
	if (m_MapOnlyBtn.GetCheck())
	{
		m_MapOnlyBtn.SetCheck(false);
		m_TreeFile.SetFileNameMask("*.*");
	}
	else
	{
		m_MapOnlyBtn.SetCheck(true);
		m_TreeFile.SetFileNameMask("????????");
	}
}

void CWorkSpaceBar::OnSTreeBeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	*pResult = 0;

	if (m_TreeSymbol.GetSelectedItemCount() <= 0)
		return;

	CPoint	pt = pNMTreeView->ptDrag;
	m_pDragImg = createDragImage(&m_TreeSymbol, &pt);
	if (m_pDragImg == (CImageList *)NULL)
		return;

	CPoint	startpt = pNMTreeView->ptDrag;
	startpt -= pt;
	AfxGetMainWnd()->PostMessage(WM_STATUS, AWS_SDRAG, (LPARAM) 0);

	m_pDragImg->BeginDrag(0, CPoint(0,0)/*startpt*/);
	m_pDragImg->DragEnter(GetDesktopWindow(), startpt);
	AfxGetMainWnd()->SetCapture();
}


void CWorkSpaceBar::OnDepthBtnClick()
{
	if (m_bOffline)
		return;
	CServerTreeCtrl *pTree = m_TabCtrl.GetCurSel() == 0?&m_TreeServer:&m_TreeTemplate;
	if (m_depthBtn.GetCheck())
	{
		m_depthBtn.SetCheck(false);
		pTree->m_nTwoDepth = 3;
		pTree->InitTree();
	}
	else
	{
		m_depthBtn.SetCheck(true);
		pTree->m_nTwoDepth = 4;
		pTree->InitTree();
	}
}
