// AxMConfigBar.cpp : implementation file
//

#include "stdafx.h"
#include "AxMConfigBar.h"

//#include "../../../../h/axisgenv.h"
//#include "../../../../h/axisgwin.h"
//#include "../../../../h/jmcode.h"
//#include "../../../../h/axisvar.h"

#include "../../h/axisgenv.h"
#include "../../h/axisgwin.h"
#include "../../h/jmcode.h"
#include "../../h/axisvar.h"

#include "TargetItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char *gszClassName = "AxMConfigBarClass";
/////////////////////////////////////////////////////////////////////////////
// CAxMConfigBar

CAxMConfigBar::CAxMConfigBar(CWnd* view, CWnd* parent, CFont* font, LPCTSTR info)
{
	m_view = view;
	m_parent = parent;
	m_font = font;

	_barChart *barChart = (_barChart*)info;

	XRegisterWindowClass(gszClassName);

	m_bar = NULL;
	
	m_autoLabel = m_autoGroup = NULL;
	m_btnPlay = m_btnReversePlay = m_btnPause = m_btnFF = m_btnStop = NULL;
	m_auto = m_manual = NULL;
	m_spin = NULL;
	
	
	m_graphCount = barChart->grpcnt;
	m_regionCount = barChart->rgncnt;

	m_graphInfo = new char[m_graphCount*SZ_GRAPH + m_regionCount*SZ_REGION];
	CopyMemory(m_graphInfo, barChart->minfo, m_graphCount*SZ_GRAPH + m_regionCount*SZ_REGION);
	CopyMemory(m_userGraph, barChart->ulist, MAX_UGRP);
	
	m_currentChartKind = barChart->mkind;
	
	m_autoPlayInterval = 3000;
	m_autoIndex = -1;
	
	ZeroMemory(&m_lastKind, sizeof(KIND));	
}

CAxMConfigBar::~CAxMConfigBar()
{
	//ClearCodeMaster();
	delete [] m_graphInfo;
	SAFE_DELETE(m_targetItems);

	while (!m_imageList.IsEmpty())
		delete m_imageList.RemoveHead();
}

BEGIN_MESSAGE_MAP(CAxMConfigBar, CWnd)
	//{{AFX_MSG_MAP(CAxMConfigBar)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_AXUSER, OnAxUser)
	ON_MESSAGE(WM_AXDELKIND, OnAxDelKind)
	ON_MESSAGE(WM_AXRELOAD, OnAxReload)
	ON_MESSAGE(WM_AXGETCTRL, OnAxGetCtrl)
	ON_MESSAGE(WM_AXSETCTRL, OnAxSetCtrl)
	ON_MESSAGE(WM_AXADDGRP, OnAxAddGraph)
	ON_MESSAGE(WM_AXGETUGRP, OnAxGetUserGraph)
	ON_MESSAGE(WM_AXDROPITEM, OnAxDropItem)
	ON_MESSAGE(WM_AXRMOUSE, OnAxRMouse)
	ON_MESSAGE(WM_AXITEMCLICK, OnAxItemClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMConfigBar message handlers

BOOL CAxMConfigBar::Create(CWnd* parent, LPRECT lpRect, UINT id)
{
	return CWnd::Create(gszClassName, "", WS_VISIBLE | WS_CHILD, *lpRect, parent, id);
}

int CAxMConfigBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_targetItems = new CAxTargetItem(GetTargetItemFileName());
	//**m_targetItems->LoadItemFromFile();
	
	GetChartKindList(m_chartList);
	InitBar();

	m_treeJP->SelectItem(m_hTreeYH);
	m_treeJP->EnsureVisible(m_hTreeYH);

	return 0;
}

void CAxMConfigBar::OnDestroy() 
{
	CWnd::OnDestroy();	
	SAFE_DELETE_WND(m_bar);
}

BOOL CAxMConfigBar::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CAxMConfigBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_bar) ResizeBar();
	Invalidate();
}

void CAxMConfigBar::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	switch (nIDEvent) 
	{
	case IDT_AUTOSTART:
		m_parent->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, mctrlAutotran), true);
		m_autoIndex = GetNextCheckItem(m_autoIndex);
		if (m_autoIndex == -1 && GetSelectItemCount()) 
		{
			m_autoIndex = GetNextCheckItem(-1);
		}
		if (m_autoIndex != -1) 
		{
			SelectKindItem(m_autoIndex);
			SetTimer(IDT_AUTOPLAY, m_autoPlayInterval, NULL);
		}
		else 
		{
			StopAutoPlay();
		}
		break;
	case IDT_AUTOPLAY:
		m_autoIndex = GetNextCheckItem(m_autoIndex);
		if (m_autoIndex == -1 && GetSelectItemCount()) 
		{
			m_autoIndex = GetNextCheckItem(-1);
		}
		if (m_autoIndex != -1) 
		{
			SelectKindItem(m_autoIndex);
			SetTimer(nIDEvent, m_autoPlayInterval, NULL);
		}
		else 
		{
			StopAutoPlay();
		}
		break;
	case IDT_AUTOSTOP:
		m_parent->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, mctrlAutotran), false);
		KillTimer(IDT_AUTOSTART);
		KillTimer(IDT_AUTOPLAY);		
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////////
void CAxMConfigBar::DeleteChart(int delkind)
{
	SendMessage(WM_AXDELKIND, delkind);
}

void CAxMConfigBar::ReloadGraph(int kind)
{
	SendMessage(WM_AXRELOAD, kind, 0);
	
	BOOL bEnable;
	switch (kind)
	{
	case GK_JPN:	case GK_BAR:	case GK_LIN:	case GK_POLE:
	case GK_BALANCE:	case GK_AVOL:
	//case GK_EQV:	case GK_CDV:	
		bEnable = TRUE;
		break;
	default:
		bEnable = FALSE;
		break;
	}
	
	SendMessage(WM_AXSETCTRL, bEnable, 0);
}

void CAxMConfigBar::ReloadGraph(LPSTR pGrp)
{
	SendMessage(WM_AXRELOAD, 0, (LONG)pGrp);
}

void CAxMConfigBar::AddGraph(int kind)
{
	SendMessage(WM_AXADDGRP, 0, kind);
}

LPCTSTR CAxMConfigBar::GetUserGraph()
{
	return (LPCTSTR)SendMessage(WM_AXGETUGRP);
}

void CAxMConfigBar::ChangePalette()
{
	CString ini = GetTabPath() + "palette.ini";

	char buffer[MAX_PATH];
	GetPrivateProfileString("General", "Palette", "Blue", buffer, MAX_PATH, ini);
	m_bar->ChangePalette(buffer, 
		m_parent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 2));
}

void CAxMConfigBar::InitBar()
{
	m_bar = new CAxMBarCtrl();
	m_bar->Create(CRect(0, 0, 10, 10), IDC_BAR, this);
	m_bar->SetImageFolder(GetImagePath());
	
	m_bar->AddTab("지표");
	m_bar->AddTab("자동");
	
	m_bar->SetTabIndex(0);
	
	m_bar->AddButton("유형/지표", MBAR_TREE);
	m_bar->AddButton("관심종목",  MBAR_TREE);
	m_bar->AddButton("보유종목",  MBAR_TREE);
//	m_bar->AddButton("뉴스특징주",    MBAR_LIST);

	m_bar->SetButtonIndex(0);
	
	m_treeJP = (CAxMTreeCtrl*)m_bar->GetButtonControl(0);
	m_treeInter = (CAxMTreeCtrl*)m_bar->GetButtonControl(1);
	m_treeHave  = (CAxMTreeCtrl*)m_bar->GetButtonControl(2);
//	m_listFeature = (CAxMListCtrl*)m_bar->GetButtonControl(3);

	SetWindowLong(m_treeHave->m_hWnd, GWL_STYLE, ~TVS_HASBUTTONS & (TVS_SINGLEEXPAND | 
		GetWindowLong(m_treeHave->m_hWnd, GWL_STYLE)));
	
	m_bar->SetTabIndex(1);
	
	CWnd* wnd;
	m_panel = (CAxMStaticCtrl*) m_bar->AddControl(MBAR_STATIC,  MBARAL_BOTTOM,		0, "68", "==");

	wnd = m_bar->AddControl(MBAR_STATIC,	MBARAL_TOP,			0, "19", "대상종목");
	m_btnTargetItem = (CAxMSpeedButton*)m_bar->AddControl(MBAR_SPDBTN, MBARAL_NONE, 0, "18", "@대상종목 사용자설정");
	m_btnTargetItem->SetParent(wnd);
	m_btnTargetItem->LoadBitmap(GetImageName("검색"));

	m_treeKind = (CAxMTreeCtrl*)m_bar->AddControl(MBAR_TREE,	MBARAL_TOP,			0, "180");

	
	wnd = m_bar->AddControl(MBAR_STATIC, MBARAL_TOP, 0, "19", "종목");
	m_btnCheckAll = (CAxMSpeedButton*)m_bar->AddControl(MBAR_SPDBTN, MBARAL_NONE, 2, "13", "@전체선택/해제");
	m_btnCheckAll->SetParent(wnd);
	m_btnCheckAll->LoadBitmap(GetImageName("GTB_CHECK"));
	m_btnCheckAll->SetCheckStyle(TRUE);
	
	m_listKind = (CAxMListCtrl*)m_bar->AddControl(MBAR_LIST,	MBARAL_CLIENT,		0);
	
	// self control
	m_autoLabel = (CAxMStaticCtrl*)m_bar->AddControl(MBAR_STATIC, MBARAL_NONE, 3, "19", "조회간격");
	m_autoLabel->SetParent(m_panel);

	m_autoGroup = (CAxMStaticCtrl*)m_bar->AddControl(MBAR_STATIC, MBARAL_NONE, 24, "30");
	m_autoGroup->SetParent(m_panel);

	m_manual = (CAxMRadioButton*)m_bar->AddControl(MBAR_RADIO, MBARAL_NONE, 1, "18", "수동");
	m_auto   = (CAxMRadioButton*)m_bar->AddControl(MBAR_RADIO, MBARAL_NONE, 1, "18", "자동");
	m_manual->SetCheck(TRUE);

	m_manual->SetParent(m_autoGroup);
	m_auto->SetParent(m_autoGroup);

	m_autoInterval = (CAxMEdit*)m_bar->AddControl(MBAR_EDIT, MBARAL_NONE, 1, "25");
	m_autoInterval->SetParent(m_autoGroup);
	
	CString s;
	s.Format("%d", m_autoPlayInterval / 1000);
	m_autoInterval->SetWindowText(s);

	SetWindowLong(m_autoInterval->m_hWnd, GWL_STYLE, GetWindowLong(m_autoInterval->m_hWnd, GWL_STYLE) | ES_NUMBER | ES_RIGHT | ES_READONLY);

	m_spin   = (CAxMSpinCtrl*)m_bar->AddControl(MBAR_SPIN, MBARAL_NONE, 1, "18");
	m_spin->SetParent(m_autoGroup);

	m_btnPlay = (CAxMSpeedButton*)m_bar->AddControl(MBAR_SPDBTN, MBARAL_NONE, 30, "20", "@다음");
	SetAutoPlayButtons(m_btnPlay, "PLAY", FALSE);

	m_btnReversePlay = (CAxMSpeedButton*)m_bar->AddControl(MBAR_SPDBTN, MBARAL_NONE, 30, "20", "@이전");
	SetAutoPlayButtons(m_btnReversePlay, "이전", FALSE);

	m_btnPause = (CAxMSpeedButton*)m_bar->AddControl(MBAR_SPDBTN, MBARAL_NONE, 30, "20", "@잠시멈춤");
	SetAutoPlayButtons(m_btnPause, "PAUSE", TRUE);
	m_btnPause->LoadBitmap(GetImageName("GTB_PAUSE"));
	m_btnPause->EnableWindow(FALSE);

	m_btnFF = (CAxMSpeedButton*)m_bar->AddControl(MBAR_SPDBTN, MBARAL_NONE, 30, "20", "@계속");
	m_btnFF->LoadBitmap(GetImageName("GTB_CONTINUE"));
	SetAutoPlayButtons(m_btnFF, "CONTINUE");

	m_btnStop = (CAxMSpeedButton*)m_bar->AddControl(MBAR_SPDBTN, MBARAL_NONE, 30, "20", "@멈춤");
	m_btnStop->LoadBitmap(GetImageName("GTB_STOP"));
	SetAutoPlayButtons(m_btnStop, "STOP");
	
	InitTreeControls();
	InitListControls();

	SetWindowLong(m_treeKind->m_hWnd, GWL_USERDATA, (LONG)m_listKind);

	SetGroupList(m_treeKind);
	
	ResizeBar();
	m_bar->SetMessageHandler(this, TRUE);
	m_bar->SetTabIndex(0);
}

void CAxMConfigBar::SetAutoPlayButtons(CAxMSpeedButton* btn, LPCTSTR imageName, BOOL check)
{
	btn->SetParent(m_panel);
	btn->LoadBitmap(GetImageName(imageName));
	btn->SetCheckStyle(check);
}

void CAxMConfigBar::ResizeBar(LPRECT lpRect)
{
	CRect rc;
	if (lpRect) 
		rc = lpRect;
	else
		GetClientRect(&rc);
	
	if (!m_bar) return;

	m_bar->SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

	if (m_bar->GetTabIndex() == 1) 
	{
		CRect rcPanel;

		m_panel->GetClientRect(&rcPanel);
		m_panel->SetWindowPos(&CWnd::wndTopMost, 0, 0, rc.Width() - 6, rcPanel.Height(), SWP_NOOWNERZORDER | SWP_SHOWWINDOW | SWP_NOMOVE);
		m_panel->GetClientRect(&rc);

		const int targetItemButtonWidth = 18, targetItemButtonHeight = 18;
		m_btnTargetItem->SetWindowPos(NULL, rc.Width() - targetItemButtonWidth - 2, 0, 
			targetItemButtonWidth, targetItemButtonHeight, SWP_SHOWWINDOW);

		const int checkAllButtonWidth = 13, checkAllButtonHeight = 13;
		m_btnCheckAll->SetWindowPos(NULL, 5, 4, checkAllButtonWidth, checkAllButtonHeight, SWP_SHOWWINDOW);
		
		m_autoLabel->SetWindowPos(NULL, 3, 3, rc.Width() - 6, 18, SWP_SHOWWINDOW);
		m_autoGroup->SetWindowPos(NULL, 3,22, rc.Width() - 6, 22, SWP_SHOWWINDOW);

		const int radioWidth = 45, radioHeight = 16;
		const int radioStart = 15;

		m_manual->SetWindowPos(NULL, radioStart, 4, radioWidth, radioHeight, SWP_SHOWWINDOW);
		m_auto->SetWindowPos(NULL, radioStart + radioWidth, 4, radioWidth, radioHeight, SWP_SHOWWINDOW);

		int left = radioStart + radioWidth * 2 + 10;
		m_autoInterval->SetWindowPos(NULL, left, 3, 30, 16, SWP_SHOWWINDOW);
		m_spin->SetWindowPos(NULL, left + 31, 3, 13, 16, SWP_SHOWWINDOW);

		const int btnWidth = 25, btnHeight = 20;
		int btnGap, btnCount;
		BOOL bAuto = m_auto->GetCheck();

		if (bAuto) 
		{
			btnGap = 2;
			btnCount = 4;
			m_btnPlay->LoadBitmap(GetImageName("GTB_PLAY"));
			m_btnPlay->SetWindowText("@자동실행");
		}
		else 
		{
			btnGap = 6;
			btnCount = 2;
			m_btnPlay->LoadBitmap(GetImageName("다음"));
			m_btnPlay->SetWindowText("@다음");
		}

		m_autoInterval->EnableWindow(bAuto);
		m_spin->EnableWindow(bAuto);

		left = (int)(rc.Width() / 2.0 - ((btnWidth * (btnCount * 1.0) + (btnCount - 1) * btnGap) / 2));
		int btnTop = rc.Height() - btnHeight - 2;

		m_btnReversePlay->SetWindowPos(NULL, left, btnTop, btnWidth, btnHeight, !bAuto ? SWP_SHOWWINDOW: SWP_HIDEWINDOW);
		if (!bAuto)
			left += btnWidth + btnGap; 
		m_btnPlay->SetWindowPos(NULL, left, btnTop, btnWidth, btnHeight, SWP_SHOWWINDOW);

		left += btnWidth + btnGap; 
		m_btnPause->SetWindowPos(NULL, left, btnTop, btnWidth, btnHeight, bAuto ? SWP_SHOWWINDOW: SWP_HIDEWINDOW);

		left += btnWidth + btnGap;
		m_btnFF->SetWindowPos(NULL, left, btnTop, btnWidth, btnHeight, bAuto ? SWP_SHOWWINDOW: SWP_HIDEWINDOW);
		
		left += btnWidth + btnGap;
		m_btnStop->SetWindowPos(NULL, left, btnTop, btnWidth, btnHeight, bAuto ? SWP_SHOWWINDOW: SWP_HIDEWINDOW);

		ResizeListColumn(m_listKind);
	}
}

void CAxMConfigBar::InitListControl(CAxMListCtrl* list, BOOL bCheckStyle)
{
	list->InsertColumn(0, "코드", LVCFMT_LEFT, 60);
	list->InsertColumn(1, "종목명", LVCFMT_LEFT, 120);
	list->SetExtendedStyle(LVS_EX_FULLROWSELECT | (bCheckStyle ? LVS_EX_CHECKBOXES : 0));

	DWORD style = ::GetWindowLong(list->m_hWnd, GWL_STYLE);
	style |= LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
	::SetWindowLong(list->m_hWnd, GWL_STYLE, style);
}

void CAxMConfigBar::InitListControls()
{
	InitListControl(m_listKind, TRUE);
//	InitListControl(m_listFeature, FALSE);
}

void CAxMConfigBar::InitTreeControls()
{
	DWORD style;
	style = ::GetWindowLong(m_treeJP->m_hWnd, GWL_STYLE);
	style &= ~TVS_HASBUTTONS;
	style &= ~TVS_LINESATROOT;
	style &= ~TVS_HASLINES;
	::SetWindowLong(m_treeJP->m_hWnd, GWL_STYLE, style);
	
	SetImageList2Tree(m_treeJP, "GTB_TREE.BMP");

	m_treeJP->DeleteAllItems();
	m_hTreeYH = m_treeJP->InsertGroup("유형", 4);
	// 2009.11.13 : CHANGE : "지표" --> "지표(찾기)"로 변경
	//m_hTreeJP = m_treeJP->InsertGroup("지표", 4);
	m_hTreeJP = m_treeJP->InsertGroup("지표(찾기)", 4);

	InitItem_YH();
	InitItem_JP();

	m_treeJP->Expand(m_hTreeYH, TVE_EXPAND);
	m_treeJP->Expand(m_hTreeJP, TVE_EXPAND);

	style = ::GetWindowLong(m_treeKind->m_hWnd, GWL_STYLE);
	style |= TVS_SHOWSELALWAYS;
	::SetWindowLong(m_treeKind->m_hWnd, GWL_STYLE, style);


	LoadInterestItems(m_treeInter);
	LoadHaveItems(m_treeHave);
//	LoadFeatureItems(m_listFeature);
}


void CAxMConfigBar::InitItem_YH()
{
	const struct _item
	{
		char	name[32];
		int		kind;
	}item[] = {
		{"봉차트",	GK_JPN}, 
		{"선차트",	GK_LIN}, 
		{"매물차트",	GK_AVOL}, 
		{"일목균형표",	GK_BALANCE}, 
		{"CandleVolume",	GK_CDV}, 
		{"EquiVolume",	GK_EQV},
		{"삼선전환도",	GK_THREE},
		{"P&F차트",	GK_PNF}, 
		{"역시계곡선",	GK_CLOCK}, 
		{"PV차트",	GK_PV},
		{"바차트",	GK_BAR}, 
		{"POLE차트",	GK_POLE}, 
		{"Kagi차트",	GK_KAGI},
		{"Renko차트",	GK_RENKO}
	};

	BOOL check;
	
	for (int i = 0; i < ARRAY_SIZE(item); i++) {
		check = m_currentChartKind == item[i].kind;
		m_yhlist.SetAt(m_treeJP->InsertRadioButton(item[i].name, m_hTreeYH, check), item[i].kind);
	}
	EnableChartKindAid(m_currentChartKind);
}

void CAxMConfigBar::InitItem_JP()
{
	LPCTSTR ginfo = &m_graphInfo[m_regionCount * SZ_REGION];
	
	int	index;
	int check;

	CString menuname;
	CStringList menuList;
	POSITION pos, posChart;
	LPCHARTKIND item;

	GetMenunames(menuList);
	GetMenunameBookmark(menuname);

	m_hsub[0] = m_treeJP->InsertGroup(menuname, 5, m_hTreeJP);

	
	for (int i = 0; i < MAX_UGRP; i++) {
		if (m_userGraph[i]) {
			for (posChart = m_chartList.GetHeadPosition(); posChart; ) {
				item = m_chartList.GetNext(posChart);

				if (item->kind == m_userGraph[i]) {
					check = GetCheckAlreadyChartKind(ginfo, item->kind);
					m_ujpList.SetAt(m_treeJP->InsertCheckBox(item->name, m_hsub[0], check), item->kind);
					break;
				}
			}
		}
	}
	
	for (pos = menuList.GetHeadPosition(), i = 1; pos; i++) {
		menuname = menuList.GetNext(pos);
		m_hsub[i] = m_treeJP->InsertGroup(menuname, 5, m_hTreeJP);
		
		index = 0;
		
		while (-1 != (index = GetChartKindIndex(i, index))) {
			posChart = m_chartList.FindIndex(index);
			item = m_chartList.GetAt(posChart);
			check = GetCheckAlreadyChartKind(ginfo, item->kind);
			m_jpList.SetAt(m_treeJP->InsertCheckBox(item->name, m_hsub[i], check), item->kind);
			index++;
		}
	}
}




int CAxMConfigBar::GetChartKindIndex(int group, int baseIndex)
{
	POSITION pos;
	LPCHARTKIND item;

	for (int i = baseIndex; i < m_chartList.GetCount(); i++) {
		pos = m_chartList.FindIndex(i);
		item = m_chartList.GetAt(pos);
		if (item->group == group)
			return i;
	}
	return -1;
}


BOOL CAxMConfigBar::GetCheckAlreadyChartKind(LPCTSTR ginfo, int kind)
{
	for (int i = 0; i < m_graphCount; i++) {
		if (kind == ((PGRAPH)&ginfo[SZ_GRAPH * i])->wGKind) {
			return TRUE;
		}
	}
	return FALSE;
}


COLORREF CAxMConfigBar::GetColor(UINT index)
{
	return (COLORREF)m_parent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), index);
}

void CAxMConfigBar::SetImageList2Tree(CAxMTreeCtrl* tree, LPCTSTR imagePath)
{
	CString path = GetImagePath() + imagePath;
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hBitmap) {
		CBitmap bitmap;
		
		bitmap.DeleteObject();
		bitmap.Attach(hBitmap);

		CImageList* il = new CImageList;
		m_imageList.AddTail(il);

		il->Create(14, 16, ILC_COLOR16 | ILC_MASK, 0, 1);
		il->Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();

		tree->SetImageList(il, TVSIL_NORMAL);
	}
}

void CAxMConfigBar::EnableChartKindAid(int index)
{
	BOOL bEnable;

	switch (index) {
		case GK_JPN:
		case GK_BAR:
		case GK_LIN:
		case GK_POLE:
		case GK_AVOL:
		case GK_BALANCE:
			bEnable = TRUE;
			break;
		default:
			bEnable = FALSE;
	}

	GetParent()->SendMessage(WM_AXUSER, bEnable, 0);
}

LRESULT CAxMConfigBar::OnAxTreeJPItemClick(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hItem = (HTREEITEM)wParam;
	if (m_treeJP->IsCheckBox(hItem)) {
		int mainChartKind = m_parent->SendMessage(GEV_VIEW, MAKEWPARAM(viewChartInfo, getMainGrpInfo));
		if (!mainChartKind) {
			return -1L;
		}
	}
	return 0L;
}

LRESULT CAxMConfigBar::OnAxUser(WPARAM wParam, LPARAM lParam)
{
	if (!m_treeJP)
		return 0L;
	if ((CWnd*)wParam != m_treeJP)
		return 0L;
	
	HTREEITEM hItem = m_treeJP->GetSelectedItem();

	int index;
	LRESULT result = 0;
	if (m_yhlist.Lookup(hItem, index)) {
		SelectYH(index);
	}
	else if (m_jpList.Lookup(hItem, index)) {
		result = SelectJP(hItem, index, m_ujpList);
		if (result < 0) 
			m_treeJP->SetCheckBox(hItem, FALSE);
	}
	else if (m_ujpList.Lookup(hItem, index)) {
		result = SelectJP(hItem, index, m_jpList);
		if (result < 0)
			m_treeJP->SetCheckBox(hItem, FALSE);
	}

	return result;
}

LRESULT CAxMConfigBar::OnAxDelKind(WPARAM wParam, LPARAM lParam)
{
	if (!m_treeJP)	return 0L;

	int kind = wParam;
	SetCheckJP(m_jpList, kind, FALSE);
	SetCheckJP(m_ujpList, kind, FALSE);

	return 0L;
}

void CAxMConfigBar::SetCheckJP(CTreeMap& list, int kind, BOOL bCheck)
{
	HTREEITEM hItem;
	int value;

	for (POSITION pos = list.GetStartPosition(); pos; ) {
		list.GetNextAssoc(pos, hItem, value);
		if (value == kind) {
			m_treeJP->SetCheckBox(hItem, bCheck);
			break;
		}
	}
}

void CAxMConfigBar::SelectYH(int index)
{
	if (m_currentChartKind == index)
		return;

	m_currentChartKind = index;
	GetParent()->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, cfgMainChart), MAKELPARAM(index, 0));
	EnableChartKindAid(index);
}

LRESULT CAxMConfigBar::SelectJP(HTREEITEM hItem, int index, CTreeMap& apply)
{
	BOOL bCheck;
	m_treeJP->GetCheckBox(hItem, bCheck);
	CWnd* pWnd = m_parent;

	if (bCheck) {
		if (!pWnd->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, aidAddCfg), index)) {
			return -1L;
		}
	}
	else {
		if (!pWnd->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, aidDelCfg), index)) {
			return -1L;
		}
	}
 
	int value;

	for (POSITION pos = apply.GetStartPosition(); pos; ) {
		apply.GetNextAssoc(pos, hItem,  value);
		if (value == index) {
			m_treeJP->SetCheckBox(hItem, bCheck);
			break;
		}
	}
	return 0L;
}

LRESULT CAxMConfigBar::OnAxReload(WPARAM wParam, LPARAM lParam)
{
	if (!m_treeJP) return 0L;

	HTREEITEM	hItem;
	int		value;
	int		kind = wParam;
	
	for (POSITION pos = m_yhlist.GetStartPosition(); pos; ) {
		m_yhlist.GetNextAssoc(pos, hItem, value);
		if (value == kind) {
			m_treeJP->SetRadioButton(hItem);
			m_currentChartKind = kind;
			return 0L;
		}
	}

	_barChart	*barChart = (_barChart *)lParam;

	m_graphCount = barChart->grpcnt;
	m_regionCount = barChart->rgncnt;

	if (m_graphInfo)
		delete [] m_graphInfo;
	m_graphInfo = new char[m_graphCount*SZ_GRAPH + m_regionCount*SZ_REGION];
	CopyMemory(m_graphInfo, barChart->minfo, m_graphCount*SZ_GRAPH + m_regionCount*SZ_REGION);

	LPCHARTKIND item;
	BOOL check;
	LPCTSTR ginfo = &m_graphInfo[m_regionCount * SZ_REGION];

	for (pos = m_jpList.GetStartPosition(); pos; ) {
		m_jpList.GetNextAssoc(pos, hItem, value);
		check = GetCheckAlreadyChartKind(ginfo, value);
		m_treeJP->SetCheckBox(hItem, check);
	}

	for (pos = m_ujpList.GetStartPosition(); pos; )	{
		m_ujpList.GetNextAssoc(pos, hItem, value);
		m_treeJP->DeleteItem(hItem);
	}
	m_ujpList.RemoveAll();

	CopyMemory(m_userGraph, barChart->ulist, MAX_UGRP);

	for (int i = 0; i < MAX_UGRP; i++) {
		if (m_userGraph[i]) {
			for (pos = m_chartList.GetHeadPosition(); pos; ) {
				item = m_chartList.GetNext(pos);

				if (item->kind == m_userGraph[i]) {
					check = GetCheckAlreadyChartKind(ginfo, item->kind);
					m_ujpList.SetAt(m_treeJP->InsertCheckBox(item->name, m_hsub[0], check), item->kind);
					break;
				}
			}
		}
	}

	return 0L;
}

LRESULT CAxMConfigBar::OnAxSetCtrl(WPARAM wParam, LPARAM lParam)
{
	/** 유형과 지표가 하나의 TreeCtrl에 들어감으로 인해 Disable 필요 없어짐.
	if (m_treeJP)
		m_treeJP->EnableWindow(wParam);
	**/

	return 0L;
}

LRESULT CAxMConfigBar::OnAxGetCtrl(WPARAM wParam, LPARAM lParam)
{
	if (m_treeJP)
		return m_treeJP->IsWindowEnabled();
	else
		return 0L;
}

LRESULT CAxMConfigBar::OnAxAddGraph(WPARAM wParam, LPARAM lParam)
{
	if (!m_treeJP) return FALSE;

	int kind = lParam;
	SetCheckJP(m_jpList, kind, TRUE);
	SetCheckJP(m_ujpList, kind, TRUE);

	return TRUE;
}

LRESULT CAxMConfigBar::OnAxGetUserGraph(WPARAM wParam, LPARAM lParam)
{
	if (!m_treeJP) return 0L;

	int index = 0, value;
	HTREEITEM hItem;

	ZeroMemory(m_userGraph, sizeof(m_userGraph));
	hItem = m_treeJP->GetChildItem(m_hsub[0]);
	while (hItem) {
		if (!m_ujpList.Lookup(hItem, value))
			return (LRESULT)m_userGraph;

		m_userGraph[index++] = value;
		hItem = m_treeJP->GetNextItem(hItem, TVGN_NEXT);
	}

	return (LRESULT)m_userGraph;	
}

LRESULT CAxMConfigBar::OnAxDropItem(WPARAM wParam, LPARAM lParam)
{
	if (!m_treeJP) return 0L;

	HTREEITEM hDragItem = (HTREEITEM)wParam, hDropItem = (HTREEITEM)lParam;
	LPCHARTKIND item;

	if (hDropItem == m_hsub[0]) {
		int index;
		if (!m_jpList.Lookup(hDragItem, index))
			return 0L;

		HTREEITEM hItem;
		int value;

		for (POSITION pos = m_ujpList.GetStartPosition(); pos; ) {
			m_ujpList.GetNextAssoc(pos, hItem, value);
			if (value == index)
				return 0L;
		}

		item = NULL;
		for (pos = m_chartList.GetHeadPosition(); pos; ) {
			item = m_chartList.GetNext(pos);
			if (item->kind == index)
				break;
		}
		
		if (item) {
			BOOL check;
			m_treeJP->GetCheckBox(hDragItem, check);
			m_ujpList.SetAt(m_treeJP->InsertCheckBox(item->name, m_hsub[0], check), index);

			m_treeJP->Expand(m_hsub[0], TVE_EXPAND);
		}
	}
	else if (hDragItem != hDropItem) {
		int index;
		if (!m_ujpList.Lookup(hDragItem, index))
			return 0L;

		m_treeJP->DeleteItem(hDragItem);
		m_ujpList.RemoveKey(hDragItem);

		m_treeJP->Expand(m_hsub[0], TVE_EXPAND);
	}

	return 0L;	
}

LRESULT CAxMConfigBar::OnAxRMouse(WPARAM wParam, LPARAM lParam)
{
	if (!m_treeJP) return 0L;

	int index, value, check;
	HTREEITEM hItem = m_treeJP->GetSelectedItem();
	POSITION pos;
	LPCHARTKIND item = NULL;

	switch (wParam) {
		case getBookMark:
			if (m_jpList.Lookup(hItem, index)) {
				for (POSITION pos = m_ujpList.GetStartPosition(); pos; ) {
					m_ujpList.GetNextAssoc(pos, hItem, value);

					if (value == index)
						return 1L;
				}
				return 2L;
			}
			else if (m_ujpList.Lookup(hItem, index))
				return 3L;
			else if (m_treeJP->GetItemText(hItem) == "지표(찾기)")
			{
				// 2009.11.12 : ADD : 지표 항목에 마우스로 클릭할 경우 지표 검색을 위한
				// 메뉴를 띄운다.
				return 4L;
			}
			break;
		case addBookMark:
			m_jpList.Lookup(hItem, index);

			for (pos = m_chartList.GetHeadPosition(); pos; ) {
				item = m_chartList.GetNext(pos);
				if (item->kind == index)
					break;
			}
			if (item) {
				m_treeJP->GetCheckBox(hItem, check);
				m_ujpList.SetAt(m_treeJP->InsertCheckBox(item->name, m_hsub[0], check), index);
				m_treeJP->Expand(m_hsub[0], TVE_EXPAND);
				m_view->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, BookMarkCfg), 0);
			}
			break;
		case delBookMark:
			if (!m_ujpList.Lookup(hItem, index))
				return 0L;

			m_treeJP->DeleteItem(hItem);
			m_ujpList.RemoveKey(hItem);

			m_treeJP->Expand(m_hsub[0], TVE_EXPAND);
			m_view->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, BookMarkCfg), 0);
			break;
		case configIndc:
			if (!m_jpList.Lookup(hItem, index)) {
				if (!m_ujpList.Lookup(hItem, index))
					return 0L;
			}

			m_view->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgSIndc), index);
			break;
	}
	return 0L;
}

LRESULT CAxMConfigBar::OnAxItemClick(WPARAM wParam, LPARAM lParam)
{
	BEGIN_AXM_MESSAGE_MAP((CWnd*)lParam, 
		m_treeKind,		OnAxTreeKindItemClick)
		AXM_MESSAGE_MAP(m_listKind,		OnAxListKindItemClick)
		AXM_MESSAGE_MAP(m_btnPlay,		OnAxPlayButtonClick)
		AXM_MESSAGE_MAP(m_btnReversePlay,	OnAxReversePlayButtonClick)
		AXM_MESSAGE_MAP(m_btnPause,		OnAxPauseButtonClick)
		AXM_MESSAGE_MAP(m_btnFF,		OnAxFFButtonClick)
		AXM_MESSAGE_MAP(m_btnStop,		OnAxStopButtonClick)
		AXM_MESSAGE_MAP(m_spin,			OnAxSpinButtonClick)
		AXM_MESSAGE_MAP(m_autoInterval,		OnAxAutoEditChange)
		AXM_MESSAGE_MAP(m_auto,			OnAxAutoClick)
		AXM_MESSAGE_MAP(m_manual,		OnAxManualClick)
		AXM_MESSAGE_MAP(m_btnCheckAll,		OnAxCheckAllButtonClick)
		AXM_MESSAGE_MAP(m_btnTargetItem,	OnAxTargetItemButtonClick)
		AXM_MESSAGE_MAP(m_bar,			OnAxBarClick)
		AXM_MESSAGE_MAP(m_treeInter,		OnAxInterItemClick)
		AXM_MESSAGE_MAP(m_listFeature,		OnAxFeatureItemClick)
		AXM_MESSAGE_MAP(m_treeHave,		OnAxHaveItemClick)
		AXM_MESSAGE_MAP(m_treeJP,		OnAxTreeJPItemClick)
	END_AXM_MESSAGE_MAP
}

LRESULT CAxMConfigBar::OnAxBarClick(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam)) {
		case BARN_TABCHANGE:
			StopAutoPlay();
			ResizeBar();
			break;
		case BARN_BTNCHANGE:
			OnCfgButtonChange(HIWORD(wParam));
			break;
	}
	return 0L;
}

LRESULT CAxMConfigBar::OnAxTreeKindItemClick(WPARAM wParam, LPARAM lParam)
{
	CAxMTreeCtrl* tree = (CAxMTreeCtrl*)lParam;

	StopAutoPlay();
	SetGroupItem(tree, (HTREEITEM)wParam);
	ResizeListColumn(m_listKind);
	
	return 0L;
}

LRESULT CAxMConfigBar::OnAxListKindItemClick(WPARAM wParam, LPARAM lParam)
{
	int index = LOWORD(wParam);
	BOOL checkOnly = HIWORD(wParam) == 0;

	if (GetSelectItemCount() == m_listKind->GetItemCount()) 
	{
		m_btnCheckAll->SetDown(TRUE);
	}
	else if (m_btnCheckAll->GetDown()) 
	{
		m_btnCheckAll->SetDown(FALSE);
	}

	SaveSelectedItems();

	if (m_auto->GetCheck() && m_btnPlay->GetDown()) 
	{
		StopAutoPlay();	//KillTimer(IDT_AUTOPLAY);
	}

	if (checkOnly) return 0L;

	m_autoIndex = index;

	SelectItem(m_listKind->GetItemData(index), m_listKind->GetItemText(index, 0));

	if (m_auto->GetCheck() && m_btnPlay->GetDown()) 
	{
		m_view->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, mctrlAutotran), false);
		SetTimer(IDT_AUTOSTART, m_autoPlayInterval, NULL);	
	}
		
	return 0L;
}

void CAxMConfigBar::ResizeListColumn(CAxMListCtrl* list)
{
	if (list) {
		CRect rc;
		list->GetClientRect(&rc);
		const int codeWidth = 60;
		list->SetColumnWidth(0, codeWidth);
		list->SetColumnWidth(1, rc.Width() - codeWidth);
	}
	
}

CString CAxMConfigBar::GetImageName(LPCTSTR image)
{
	return GetImagePath() + image + ".bmp";
}

LRESULT CAxMConfigBar::OnAxPlayButtonClick(WPARAM wParam, LPARAM lParam)
{
	if (m_auto->GetCheck()) {
		if (!m_btnPlay->GetDown()) {
			m_btnPause->SetDown(FALSE);
			StopAutoPlay();
		}
		else {
			if (GetSelectItemCount() == 0) {
				m_btnPlay->SetDown(FALSE);
				return 0L;
			}
			m_autoIndex = -1;
			StartAutoPlay();
		}
	}
	else if (m_manual->GetCheck()) {
		if (GetSelectItemCount() == 0) {
			return 0L;
		}

		m_autoIndex = GetNextCheckItem(m_autoIndex);
		if (m_autoIndex == -1) {
			m_autoIndex = GetNextCheckItem(-1);
		}
		if (m_autoIndex != -1) {
			SelectKindItem(m_autoIndex);
		}
	}
	
	return 0L;
}

LRESULT CAxMConfigBar::OnAxReversePlayButtonClick(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_manual->GetCheck());

	m_autoIndex = GetPrevCheckItem(m_autoIndex);
	if (m_autoIndex == -1) {
		m_autoIndex = GetPrevCheckItem(m_listKind->GetItemCount());
	}
	if (m_autoIndex != -1) {
		SelectKindItem(m_autoIndex);
	}

	return 0L;
}


LRESULT CAxMConfigBar::OnAxPauseButtonClick(WPARAM wParam, LPARAM lParam)
{
	static int oldIndex = -1;
	if (m_btnPause->GetDown() && !m_btnPlay->GetDown())
		m_btnPause->SetDown(FALSE);

	if (m_btnPlay->GetDown()) {
		if (m_btnPause->GetDown()) {
			oldIndex = m_autoIndex;
			StopAutoPlay();
		}
		else {
			m_autoIndex = oldIndex;
			StartAutoPlay();
		}
	}
	return 0L;
}

LRESULT CAxMConfigBar::OnAxFFButtonClick(WPARAM wParam, LPARAM lParam)
{
	const int ffOffset = 5;
	if (m_btnPlay->GetDown()) {
		m_autoIndex += ffOffset;
		if (m_autoIndex >= m_listKind->GetItemCount())
			m_autoIndex = 0;
		if (m_btnPause->GetDown()) {
			m_btnPause->SetDown(FALSE);
		}
		StartAutoPlay();
	}
	else {
		int index = -1, orgIndex = -2;
		for (int i = 0; i < m_listKind->GetItemCount(); i++) {
			if (m_listKind->GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED) {
				orgIndex = i;
				index = i + ffOffset;
				break;
			}
		}

		if (index == -1)
			index = 0;
		else if (index >= m_listKind->GetItemCount()) {
			if (orgIndex == m_listKind->GetItemCount() - 1) 
				index = 0;
			else
				index = m_listKind->GetItemCount() - 1;
		}

		if (index < 0 || index >= m_listKind->GetItemCount())
			index = 0;

		SelectKindItem(index);
	}
	return 0L;
}

LRESULT CAxMConfigBar::OnAxStopButtonClick(WPARAM wParam, LPARAM lParam)
{
	if (m_btnPlay->GetDown())
		m_btnPlay->SetDown(FALSE);
	if (m_btnPause->GetDown())
		m_btnPause->SetDown(FALSE);

	StopAutoPlay();
	return 0L;
}

void CAxMConfigBar::StartAutoPlay()
{
	SetTimer(IDT_AUTOSTART, 100, NULL);	
}

void CAxMConfigBar::StopAutoPlay()
{
	SetTimer(IDT_AUTOSTOP, 1, NULL);//KillTimer(IDT_AUTOPLAY);

	if (m_btnPlay->GetDown() && !m_btnPause->GetDown())
		m_btnPlay->SetDown(FALSE);
	m_autoIndex = -1;
}

LRESULT CAxMConfigBar::OnAxSpinButtonClick(WPARAM wParam, LPARAM lParam)
{
	int iDelta = (int)wParam;
	
	m_autoPlayInterval -= (iDelta * 1000);

	if (m_autoPlayInterval < 2000)
		m_autoPlayInterval = 2000;
	else if (m_autoPlayInterval > 10000)
		m_autoPlayInterval = 10000;

	CString s;
	s.Format("%d", m_autoPlayInterval / 1000);
	m_autoInterval->SetWindowText(s);

	return 0L;
}

LRESULT CAxMConfigBar::OnAxAutoEditChange(WPARAM wParam, LPARAM lParam)
{
	int iDelta = (int)wParam;
	
	m_autoPlayInterval = (iDelta * 1000);

	return 0L;
}

LRESULT CAxMConfigBar::OnAxAutoClick(WPARAM wParam, LPARAM lParam)
{
	ResizeBar();
	m_btnPlay->SetCheckStyle(TRUE);
	m_btnPause->EnableWindow(TRUE);
	return 0L;
}

LRESULT CAxMConfigBar::OnAxManualClick(WPARAM wParam, LPARAM lParam)
{
	ResizeBar();
	m_btnPlay->SetCheckStyle(FALSE);
	m_btnPause->EnableWindow(FALSE);
	m_btnPause->SetDown(FALSE);
	StopAutoPlay();

	if (m_listKind->GetItemCount() > 0) {
		m_listKind->SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		m_listKind->EnsureVisible(0, FALSE);
		m_listKind->SetItemState(0, 0, LVIS_SELECTED);
	}
	return 0L;
}

int CAxMConfigBar::GetSelectItemCount()
{
	int count = 0;

	for (int i = 0; i < m_listKind->GetItemCount(); i++) {
		if (GetListKindItemCheck(i))
			count++;
	}
	return count;
}

CString CAxMConfigBar::GetSelectItems()
{
	CString item, items(_T(""));
	int find;

	for (int i = 0; i < m_listKind->GetItemCount(); i++) {
		if (GetListKindItemCheck(i)) {
			item = m_listKind->GetItemText(i, 0);
			find = item.Find(SPACE);
			items += item.Left(find);
			items += ';';
		}
	}
	return items;
}

void CAxMConfigBar::SaveSelectedItems()
{
	return;
	if (m_listKind->GetItemCount()) {
		CString items = GetSelectItems();
		m_targetItems->SetItem(m_lastKind, items);
		m_targetItems->SaveItemToFile();
	}
}

LRESULT CAxMConfigBar::OnAxCheckAllButtonClick(WPARAM wParam, LPARAM lParam)
{
	SetListKindItemCheck(m_btnCheckAll->GetDown());

	SaveSelectedItems();
	return 0L;
}

void CAxMConfigBar::SetListKindItemCheck(BOOL check)
{
	for (int i = 0; i < m_listKind->GetItemCount(); i++) {
		SetListKindItemCheck(i, check);
	}
}

void CAxMConfigBar::SetListKindItemCheck(int index, BOOL check)
{
	LVITEM item;
	ZeroMemory(&item, sizeof(LVITEM));
	item.stateMask = LVIS_STATEIMAGEMASK;
	item.state = (((int)check & 1) + 1) << 12;
	m_listKind->SendMessage(LVM_SETITEMSTATE, index, (LPARAM)&item);
}

BOOL CAxMConfigBar::GetListKindItemCheck(int index)
{
	return ListView_GetCheckState(m_listKind->m_hWnd, index);
}

int CAxMConfigBar::GetNextCheckItem(int index)
{
	for (int i = index + 1; i < m_listKind->GetItemCount(); i++) {
		if (GetListKindItemCheck(i)) {
			return i;
		}
	}

	return -1;
}

int CAxMConfigBar::GetPrevCheckItem(int index)
{
	for (int i = index - 1; i >= 0; i--) {
		if (GetListKindItemCheck(i)) {
			return i;
		}
	}

	return -1;
}

void CAxMConfigBar::SelectKindItem(int index)
{
	CString code = m_listKind->GetItemText(index, 0);
	if (code.IsEmpty()) return;
	
	m_listKind->SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
	m_listKind->EnsureVisible(index, FALSE);
	SelectItem(m_listKind->GetItemData(index), code);	
}

extern HINSTANCE self;
LRESULT CAxMConfigBar::OnAxTargetItemButtonClick(WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst = AfxGetInstanceHandle();
	AfxSetResourceHandle(self);
	CTargetItemDlg dlg(this);

	dlg.SetKind(GetSelectedItemString(m_treeKind));
	if (dlg.DoModal() == IDOK) {
		//**m_targetItems->LoadItemFromFile();
		OnTargetItemUserConfig();
		ProcessTargetItem();
	}
	AfxSetResourceHandle(hInst);
	
	return 0L;
}

void CAxMConfigBar::ProcessTargetItem()
{
	if (!m_lastKind.kind && !m_lastKind.subkind && !m_lastKind.index)
		return;

	SetListKindItemCheck(TRUE);
	m_btnCheckAll->SetDown(TRUE);
}

LRESULT CAxMConfigBar::OnAxInterItemClick(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hItem = (HTREEITEM)wParam;
	CAxMTreeCtrl* tree = (CAxMTreeCtrl*)lParam;

	if (tree->ItemHasChildren(hItem)) return 0L;

	CString code = tree->GetItemText(hItem);
	SelectItem(tree->GetItemData(hItem), code);
	return 0L;
}

LRESULT CAxMConfigBar::OnAxFeatureItemClick(WPARAM wParam, LPARAM lParam)
{
	int index = LOWORD(wParam);
	if (index <= m_listFeature->GetItemCount())
		SelectItem(m_listFeature->GetItemData(index), m_listFeature->GetItemText(index, 0));
	return 0L;
}

LRESULT CAxMConfigBar::OnAxHaveItemClick(WPARAM wParam, LPARAM lParam)
{
	m_treeHave->SelectItem((HTREEITEM)wParam);
	RequestHaveItem((HTREEITEM)wParam);
	return 0L;
}


void CAxMConfigBar::SelectItem(UINT kind, LPCTSTR code)
{
	CString cd(code), str;
	int find = cd.Find(SPACE);
	if (find > 0)
		cd = cd.Left(find);

	str.Format("%d;%s", kind, cd);
	OnSelectItem(str);
}

CString CAxMConfigBar::GetSelectedItemString(CAxMTreeCtrl* tree)
{
	CString str, sResult(_T(""));

	HTREEITEM hItem = tree->GetSelectedItem();

	while (hItem) {
		str = tree->GetItemText(hItem);
		if (sResult.IsEmpty()) {
			sResult = str;
		}
		else {
			sResult.Insert(0, "\\");
			sResult.Insert(0, str);
		}
		hItem = tree->GetParentItem(hItem);
	}
	return sResult;
}

void CAxMConfigBar::OnCfgButtonChange(int index)
{
	switch (index) {
		case 3: // Interest Item
			RefreshInterestItem();
			break;
		case 7: // Feature Item
			if (m_listFeature->GetItemCount() == 0) 
				RequestFeatureItems();
			break;
	}
}
